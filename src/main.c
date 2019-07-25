/***************************************************************************//**
 * @file
 * @brief
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * This example establishes a connection to Digi Remote Manager and writes
 * the temperature read by the temperature sensor to the stream Temperature.
 * It also attempts to read 3 eight-bit RGB values of the format R,G,B
 * (with the range 0-255) from the stream called RGB.
 ******************************************************************************/
/***************************************************************************//**
 * @Note This example establishes a https connection, but doesn't provide the server
 * certificate validation.Production code should implement certificate validation.
 ******************************************************************************/

// fix xbee_config.c: test hologram first
// check that APN is set correctly
#include <stdio.h>
#include <stdlib.h>
#include "em_device.h"
#include "em_cmu.h"
#include "em_chip.h"
#include "em_emu.h"

#ifndef LCDLOG
#include "uart.h"	// include for serial terminal printing
#endif

#include "display.h"
#include "textdisplay.h"
#include "retargettextdisplay.h"

#include "xbee/platform.h"
#include "xbee/device.h"
#include "xbee/ipv4.h"
#include "xbee/atcmd.h"
#include "xbee/byteorder.h"
#include "xbee/atmode.h"

#include "http_client.h"
#include "temp_sensor.h"
#include "leds.h"
#include "gnss.h"
/* Defined in project settings, configures the XBee at runtime */
#ifdef XBEE_DEMO_CONFIG
#include "xbee_config.h"
#endif

#include  <bsp_os.h>
#include  "bsp.h"

#include  <cpu/include/cpu.h>
#include  <common/include/common.h>
#include  <kernel/include/os.h>

#include  <common/include/lib_def.h>
#include  <common/include/rtos_utils.h>
#include  <common/include/toolchains.h>

#include "iot_config.h"

//#include <retargetserial.h> // retarget serial
/*
 *********************************************************************************************************
 *********************************************************************************************************
 *                                             LOCAL DEFINES
 *********************************************************************************************************
 *********************************************************************************************************
 */

#define  MAIN_START_TASK_PRIO              	21u
#define  MAIN_START_TASK_STK_SIZE          	2048u

#define  LOCAL_IN_TASK_PRIO             	26u //Below Cellular priority to prevent conflict
#define  LOCAL_IN_TASK_STK_SIZE				1024u

#define  CELL_TX_TASK_PRIO             	   	20u
#define  CELL_TX_TASK_STK_SIZE				1024u

//ECB TODO Add Cell RX here

#define LOCAL_IN_BUFF_LEN                 	256u //This will be the max command length

#define  XBEE_TARGET_BAUD                 	115200
#define  TEMP_UPLOAD_PERIOD               	30 /* (Seconds) */
#define  LED_DOWNLOAD_PERIOD              	10 /* (Seconds) */

#define BUTTON_PORT							gpioPortC
#define BUTTON0_PIN							8u
#define BUTTON1_PIN 						9u
#define RESPONSE_BUFF_SIZE 108
#define CARRIERPROFILE						XBEE_CONFIG_CARRIER_ATT				// xbee_config.h enum
//#define LCDLOG									// establishes LCD or Terminal logging

/*
 *********************************************************************************************************
 *********************************************************************************************************
 *                                        LOCAL GLOBAL VARIABLES
 *********************************************************************************************************
 *********************************************************************************************************
 */

/* Start Task Stack.                                    */
static CPU_STK mainStartTaskStk[MAIN_START_TASK_STK_SIZE];
/* Start Task TCB.                                      */
static OS_TCB mainStartTaskTCB;

/* Local in Task Stack and TCB                                  */
static CPU_STK localInTaskStk[LOCAL_IN_TASK_STK_SIZE];
static OS_TCB localInTaskTCB;

/* Cell TX Task Stack and TCB                                  */
static CPU_STK cellTXTaskStk[CELL_TX_TASK_STK_SIZE];
static OS_TCB cellTXTaskTCB;

//ECB TODO add Cell Rx here

static xbee_dev_t myXbee;
GnssSln_t sln;

OS_SEM CellTXBuffSem; //Flag to say data is ready in local_in_buff
OS_MUTEX CellTXBuffMutex; //Mutex to prevent concurrent access to the buffer AND count

char local_in_buff[LOCAL_IN_BUFF_LEN] = { 0 }; //Buffer to transfer data between tasks
uint32_t local_in_buff_count = 0; //length of data in buffer - Must be updated w/ every snprintf or strcpy

//ECB Todo add Cell RX and/or wifi details here

/*
 *********************************************************************************************************
 *********************************************************************************************************
 *                                       LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************
 *********************************************************************************************************
 */

static void mainStartTask(void *p_arg);
static void localInTask(void *p_arg);
static void cellTXTask(void *p_arg);
//static void createDataStreams(void); //Not used
static void printDeviceDescription(void);
static void initCommandLayer(void);
//static int updateLeds(void); //Not Used
//static int updateTemp(void); //Not Used
static int initXBee(void);
void clearScreen(void);
void clearTerminal(void);


void clearScreen(void) { //Its sloppy - I know
	int i;
	for (i = 0; i < 20; i++) {
		//for(j = 0; j < 20; j++){
		printf("                     "); //Print 20 spaces to "Clear" the screen
		//}
	}
}

void clearTerminal()
{
	USART_Tx(USART4, 27);
	USART_Tx(USART4, '[');
	USART_Tx(USART4, '2');
	USART_Tx(USART4, 'J');
	USART_Tx(USART4, 27);
	USART_Tx(USART4, '[');
	USART_Tx(USART4, 'H');
}


int main(void) {
	RTOS_ERR err;

	CPU_Init(); /* Initialize CPU.                                      */
	BSP_SystemInit(); /* Initialize System.                                   */

	OSInit(&err); /* Initialize the Kernel.                               */
	/*   Check error code.                                  */
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

	/* Initialize the display module. */
	DISPLAY_Init();
	/* Retarget stdio to a text display. */
	if (RETARGET_TextDisplayInit() != TEXTDISPLAY_EMSTATUS_OK) {
		while (1)
			;
	}
	/* Setup LED1 */
	setupRgbLed1();
	/* Setup temperature and RH sensor */
	initTempSensor();

	/* setup uart if printing to terminal*/
#ifndef LCDLOG
	uartInit();
#endif
	// attempt to use retarget serial to change printf() functionality - didnt work at all haha
//#ifdef TERMINALDEBUG
//	RETARGET_SerialInit();
//	GPIO_PinModeSet(gpioPortE, 1, gpioModePushPull, 1);		// virtual COM port enable
//#endif

	//binary variable to determine whose turn to access data
	OSMutexCreate(&CellTXBuffMutex, "CELL Tx Buff/Cnt MTX", &err);
	//variable says data ready for moving
	OSSemCreate(&CellTXBuffSem, "Cell Tx Ready", 0, &err);
	OSTaskCreate(&mainStartTaskTCB, /* Create the Start Task.                               */
	"Main Start Task", mainStartTask,
	DEF_NULL,
	MAIN_START_TASK_PRIO, &mainStartTaskStk[0],
			(MAIN_START_TASK_STK_SIZE / 10u),
			MAIN_START_TASK_STK_SIZE, 0u, 0u,
			DEF_NULL, (OS_OPT_TASK_STK_CLR ), &err);

	/*   Check error code.                                  */
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

	OSStart(&err); /* Start the kernel.                                    */
	/*   Check error code.                                  */
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

	return (1);
}

/*
 *********************************************************************************************************
 *********************************************************************************************************
 *                                           LOCAL FUNCTIONS
 *********************************************************************************************************
 *********************************************************************************************************
 */

#if defined(XBEE_DEMO_CONFIG) && defined(XBEE_DEMO_HOLOGRAM_SIM)
/***************************************************************************//**
 * @brief Custom configuration of XBee to work correctly with ATT SIM card
 * @retval 0  Successfully configured XBee
 * @retval -EIO Could not configure XBeee
 ******************************************************************************/
//Andy changed to ATT config
int configureXbeeForHologramSim(void) {
	char buff[64];
	uint8_t i;
	uint32_t rxTimeout;
	int ch;
	RTOS_ERR err;

	if (configureXBee(&myXbee,
	XBEE_TARGET_BAUD, XBEE_CONFIG_MODE_BYPASS, XBEE_CONFIG_CARRIER_AUTO_DETECT,					// default bypass, auto_detect, true
	FALSE)) {
		return -EIO;
	} else {
		/* Disable echo so that response to AT+URAT=7 command will be predictable */
		OSTimeDly(8000, OS_OPT_TIME_DLY, &err);
		xbee_ser_write(&myXbee.serport, "ATE0\r", 10);
		OSTimeDly(1000, OS_OPT_TIME_DLY, &err);
		xbee_ser_rx_flush(&myXbee.serport);

		/* Disable NB-IoT */
		xbee_ser_write(&myXbee.serport, "AT+URAT=7\r", 10);

		/* Wait to receive "OK" response */
		rxTimeout = xbee_millisecond_timer();
		i = 0;
		while (xbee_millisecond_timer() - rxTimeout < 2000) {
			ch = xbee_ser_getchar(&myXbee.serport);
			if (ch >= 0) {
				buff[i] = ch;
				i++;
				if (i >= strlen("\r\nOK\r\n")) {
					buff[i] = '\0';
					break;
				}
			}
		}
	}
	/* Make sure the device responded to the AT+URAT=7 command with "OK" */
	if (strncmp(buff, "\r\nOK\r\n", strlen("\r\nOK\r\n"))) {
		return -EIO;
	}
	/* Place the XBee into API mode */
	if (configureXBee(&myXbee,
	XBEE_TARGET_BAUD, XBEE_CONFIG_MODE_API, XBEE_CONFIG_CARRIER_RESERVED,			// api mode, user defined carrier profile, no factory reset
	FALSE)) {
		return -EIO;
	}
	/* Reinitialize with correct settings */
	myXbee.flags &= ~XBEE_DEV_FLAG_CMD_INIT;
	myXbee.flags &= ~XBEE_DEV_FLAG_QUERY_BEGIN;

	return 0;
}
#endif

/***************************************************************************//**
 * @brief A top level function to call all init. and config. functions for XBee
 ******************************************************************************/
static int initXBee(void) {
	xbee_serial_t ser1;
	ser1.baudrate = XBEE_TARGET_BAUD;
	/* Initialize the XBee device (identifies device) */
	if (xbee_dev_init(&myXbee, &ser1, NULL, NULL)) {
#ifdef LCDLOG
		printf("\fCould not open communication with XBee\n");
		return -EIO;
#else
		char message[] = "Could not open communication with XBee\r\n";
		for(int i = 0; i < sizeof(message) / sizeof(char); i++){
			USART_Tx(USART4, message[i]);
		}
		return -EIO;
#endif
	}

#if defined(XBEE_DEMO_CONFIG) && defined(XBEE_DEMO_HOLOGRAM_SIM)
#ifdef LCDLOG
		printf("\fConfigure XBee\n");
		int retval = configureXbeeForHologramSim();
		if (retval) {
			printf("\fFailed to config XBee\n"
					"\nCheck that the XBee\n"
					"is powered and is\n"
					"connected properly");
			return retval;
		}
#else
		char message[100] = "\fConfigure XBee\r\n\r\n";		// waste of memory i know, ill come back
		uartSend(message);
		int retval = configureXbeeForHologramSim();
		if (retval) {
		strcpy(message, "Failed to config XBee "
				"for Hologram SIM. "
				"Check that the XBee "
				"is powered and is "
				"connected properly\r\n");
		uartSend(message);
		return retval;
		}
#endif
#endif

	/* Command layer initialization (AT commands) */
#ifdef LCDLOG
	printf("\fInitializing\n");
#else
	strcpy(message, "Initializing\r\n");
	uartSend(message);
#endif
	initCommandLayer();

	/* Check for successful communication with the XBee */
	if (myXbee.hardware_version == 0) {
#ifdef LCDLOG
		printf("\fFailed to initialize\nattempting to\nreconfigure the XBee\n");
#else
		strcpy(message, "Failed to initialize, attempting to reconfigure the XBee\r\n");
		uartSend(message);
#endif

#if defined(XBEE_DEMO_CONFIG) && !defined(XBEE_DEMO_HOLOGRAM_SIM)
		if (configureXBee(&myXbee,
						XBEE_TARGET_BAUD,
						XBEE_CONFIG_MODE_API,
						XBEE_CONFIG_CARRIER_AUTO_DETECT,
						TRUE)) {
#ifdef LCDLOG
			printf("\fFailed to config\n"
					"check that the XBee\n"
					"is powered and is\n"
					"connected properly");
#else
			strcpy(message, "Failed to config "
					"check that the XBee "
					"is powered and is "
					"connected properly\r\n")
			uartSend(message);
			return -EIO;
#endif
		}
		/* Reinitialize with correct settings */
		myXbee.flags &= ~XBEE_DEV_FLAG_CMD_INIT;
		myXbee.flags &= ~XBEE_DEV_FLAG_QUERY_BEGIN;
#ifdef LCDLOG
		printf("\fInitializing");
#else
		strcpy(message, "Initializing\r\n");
		uartSend(message);
#endif
		initCommandLayer();
#else
		/* Without the config code, we cannot continue */
#ifdef LCDLOG
		printf("\nCould not reconfig,\ninclude config code");
#else
		strcpy(message, "Could not reconfig, include config code\r\n");
		uartSend(message);
#endif
		return -EIO;
#endif

	}

#if defined(XBEE_DEMO_CONFIG) && defined(XBEE_CHANGE_APN)
	/* Attempt to Apply the correct APN as defined by XBEE_TARGET_APN */
#ifdef LCDLOG
	printf("\fAttempting to apply\nthe new APN\n");
#else
	strcpy(message, "Attempting to apply the new APN\r\n");
	uartSend(message);
#endif
	int ret = configureAPN(&myXbee);
	if (ret < 0) {
#ifdef LCDLOG
		printf("\fFailed to apply\nthe new APN\n");
#else
		strcpy(message, "Failed to apply the new APN \r\n\r\n");
		uartSend(message);
#endif
	} else if (ret == 1) {
#ifdef LCDLOG
		printf("The APN was already\nset correctly!\n");
#else
		strcpy(message, "The APN was already set correctly!\r\n\r\n");
		uartSend(message);
#endif
	} else {
#ifdef LCDLOG
		printf("The new APN has been\nset correctly!\n");
#else
		strcpy(message, "the new APN has been set correctly!\r\n\r\n");
		uartSend(message);
#endif
	}
#endif
	return 0;
}

/***************************************************************************//**
 * @brief Helps form fixed point values (used for GNSS and temperature)
 ******************************************************************************/
static int strFixedPt(char *buffer, size_t bufsz, int32_t k) {
	int ret;
	int32_t integer = k / 1000;
	int32_t fraction = k % 1000;

	// Determine the correct precision for the fractional portion
	if ((fraction % 10) == 0) {
		if ((fraction % 100 == 0)) {
			if ((fraction % 1000 == 0)) {
				ret = snprintf(buffer, bufsz, "%ld", integer);
			} else {
				ret = snprintf(buffer, bufsz, "%ld.%01ld", integer,
						fraction / 100);
			}
		} else {
			ret = snprintf(buffer, bufsz, "%ld.%02ld", integer, fraction / 10);
		}
	} else {
		ret = snprintf(buffer, bufsz, "%ld.%03ld", integer, fraction);
	}

	return ret;
}

/***************************************************************************//**
 * @brief Makes the required data streams if they do not already exist on RM
 ******************************************************************************/
//static void createDataStreams(void) {
//
//#ifdef LCDLOG
//	printf("\fCreating Data Streams\n");
//#else
//	char message[100] = "Creating Data Streams\r\n";
//	uartSend(message);
//#endif
//	bool initTemperature = 0;
//	bool initRGB = 0;
//	int retries = 0;
//	int i;
//	RTOS_ERR err;
//
//	/* Use flags to confirm that both streams have been created successfully */
//	while (!(initTemperature && initRGB)) {
//	//	checkConnection(&myXbee);
//
//		/* Print information about the connection */
//#ifdef LCDLOG
//		printf("\033[H" "\nCTS:%d TX:0x%.2X AI:%.1d\n",
//				xbee_ser_get_cts(&(myXbee.serport)), getLastTXStatus(),
//				myXbee.wpan_dev.flags & WPAN_FLAG_JOINED);
//#else
//		int cts = xbee_ser_get_cts(&(myXbee.serport));
//		int txStatus = getLastTXStatus();
//		int AI = myXbee.wpan_dev.flags & WPAN_FLAG_JOINED;
////		strcpy(message, "\033[HCTS:");
////		strcat((((strcat(strcat(message, (char) cts), "TX:0x"),(char) txStatus),"AI:") ,(char) AI),"\r\n");
////		uartSend(message);
//#endif
//
//		if (!initTemperature) {
//			/*initTemperature = (initCloudDataStream("Temperature", "DOUBLE", "Celsius", 10))
//			 ?  0 : 1;*/
//			initTemperature = 1;
//		}
//		if (!initRGB) {
//			/*initRGB = (initCloudDataStream("RGB", "STRING", "RGB", 10))
//			 ?  0 : 1;*/
//			initRGB = 1;
//		}
//
//		OSTimeDly(300, OS_OPT_TIME_DLY, &err);
//		printf("\033[H" "\n\n");
//		for (i = 0; i < retries; i++) {
//			printf(".");
//		}
//		if (retries++ > 100) {
//			printf("\fFailed to create streams\n"
//					"make sure your credentials,\n"
//					"remote manager URI and\n"
//					"permissions allow for posting\n"
//					"to remote manager (reset)\n");
//			while (1) {
//			} /* Block and wait for reset */
//		}
//	}
//
//	printf("\fCreated Data Streams!\n");
//	OSTimeDly(300, OS_OPT_TIME_DLY, &err);
//}
//
///***************************************************************************//**
// * @brief Pushes new temperature information to the remote manager
// ******************************************************************************/
//static int updateTemp(void) {
//	static uint32_t lastSend = 0;
//	char data[32];
//	char gnssDat[64];
//	int ret = 0;
//	uint32_t rh = 0;
//	int32_t tmp = 0;
//
//	ret = tempSensorReadHumTemp(&rh, &tmp);
//	if (ret < 0) {
//		return ret;
//	}
//
//	/* If we are due to upload data */
//	if (TEMP_UPLOAD_PERIOD <= xbee_seconds_timer() - lastSend) {
//		if (strFixedPt(data, sizeof(data), tmp) < 0) {
//			ret = snprintf(data, sizeof(data), "ERROR: ENCODING");
//		}
//		/* Check for viable GNSS data */
//		if (gnssSln(&sln) == 0 && sln.fixOK) {
//			sprintf(gnssDat, "[%i.%07i, %i.%07i, %i.%03i]",
//					(sln.lon / 10000000), abs(sln.lon % 10000000), // 10**-7 deg
//					(sln.lat / 10000000), abs(sln.lat % 10000000), // 10**-7 deg
//					(sln.height / 1000), abs(sln.height % 1000)); // 10**-3 m)
//			ret = sendDataToCloud(data, "Temperature", "DOUBLE", gnssDat, 10);
//		} else {
//			ret = sendDataToCloud(data, "Temperature", "DOUBLE", NULL, 10);
//		}
//		/* If the send was successful then update timer and display data */
//		if (ret == 0) {
//			lastSend = xbee_seconds_timer();
//			printf("temp (C):\n%.7s             \n", data);
//		}
//	}
//	return ret;
//}
//
///***************************************************************************//**
// * @brief Requests new LED values from the remote manager and then changes LEDs
// ******************************************************************************/
//static int updateLeds(void) {
//	static uint32_t lastSend = 0;
//	char data[32];
//	uint32_t r, g, b;
//	int ret;
//
//	if (LED_DOWNLOAD_PERIOD <= xbee_seconds_timer() - lastSend) {
//		ret = getDataFromCloud(data, "RGB", 10);
//		/* Update colors and display only if we goto valid data */
//		if (ret == 0) {
//			lastSend = xbee_seconds_timer();
//
//			if (sscanf(data, "%" PRIu32 " , %" PRIu32 " , %" PRIu32, &r, &g, &b)
//					== 3) {
//				/* Must be less than or equal to PWM_STEPS (255) */
//				if (r > PWM_STEPS) {
//					r = PWM_STEPS;
//				}
//				if (g > PWM_STEPS) {
//					g = PWM_STEPS;
//				}
//				if (b > PWM_STEPS) {
//					b = PWM_STEPS;
//				}
//
//				setLed1(r, g, b);
//				printf("RGB:\n%" PRIu32 ",%" PRIu32 ",%" PRIu32 "      \n", r,
//						g, b);
//			} else {
//				printf("RGB:\nMalfromed\n");
//			}
//		}
//	}
//	return ret;
//}

/***************************************************************************//**
 * @brief Helper Function for initializing the command layer
 ******************************************************************************/
static bool isCmdQueryFinished(xbee_dev_t *xbee) {
	int ret = xbee_cmd_query_status(xbee);

	if (ret == XBEE_COMMAND_LIST_DONE || ret == XBEE_COMMAND_LIST_TIMEOUT) {
		return true;
	}
	return false;
}

/***************************************************************************//**
 * @brief Gets basic device information, only calls init once
 ******************************************************************************/
static void initCommandLayer(void) {
	RTOS_ERR err;

	xbee_cmd_init_device(&myXbee);
	/* Wait for XBee to respond */
	while (!isCmdQueryFinished(&myXbee)) {
		xbee_dev_tick(&myXbee);
#ifdef LCDLOG
		printf(".");
#else
		char message[100] = ".";
		uartSend(message);
#endif
		OSTimeDly(100, OS_OPT_TIME_DLY, &err);
		APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), ;);
	}
#ifndef LCDLOG
	uartSend("\r\n\r\n");
#endif
}

/***************************************************************************//**
 * @brief Reads data in over the UART, stores, and sets semaphore
 ******************************************************************************/
static void localInTask(void *p_arg) {
	RTOS_ERR err;
	CPU_TS ts;
	PP_UNUSED_PARAM(p_arg); /* Prevent compiler warning. */
	int ret = 0;
	uint32_t rh = 0;
	int32_t tmp = 0;
	char tmpData[32], rhData[32];

	//Initialize the UART (or SPI) here
	//ECB TODO Add UART config here
	//Clear the buffers here, aquire the mutex
	OSMutexPend((OS_MUTEX *) &CellTXBuffMutex, (OS_TICK) 0,
			(OS_OPT) OS_OPT_PEND_BLOCKING, (CPU_TS*) &ts, (RTOS_ERR*) &err);
	local_in_buff_count = 0;
	OSMutexPost(&CellTXBuffMutex, OS_OPT_POST_NONE, &err);
	while (1) { //Infinite loop that yields when possible
		//ECB TODO Add "IF UART DATA "
		//if(uartRxData>0){}
		ret = tempSensorReadHumTemp(&rh, &tmp);
		if (strFixedPt(tmpData, sizeof(tmpData), tmp) < 0 || ret < 0) {
			strcpy(tmpData, "ERROR_TMP");
		}
		if (strFixedPt(rhData, sizeof(rhData), rh) < 0 || ret < 0) {
			strcpy(rhData, "ERROR_RH");
		}
		//printf("T:%s H:%s\r\n",tmpData,rhData);
		OSMutexPend((OS_MUTEX *) &CellTXBuffMutex, (OS_TICK) 0,
				(OS_OPT) OS_OPT_PEND_BLOCKING, (CPU_TS*) &ts, (RTOS_ERR*) &err);
		//ret = strcpy(local_in_buff, "TESTING THE INPUT");
		ret = snprintf(local_in_buff, LOCAL_IN_BUFF_LEN, "Temp: %s Hum: %s",
				tmpData, rhData);
		local_in_buff_count = ret;
		OSMutexPost(&CellTXBuffMutex, OS_OPT_POST_NONE, &err);
		OSSemPost(&CellTXBuffSem,
		OS_OPT_POST_1, &err);
		OSTimeDly(10, OS_OPT_TIME_DLY, &err); //This will Update as much as possible but needs to yield to TX
	}
}
/***************************************************************************//**
 * @brief takes data from the Local in Buffer, formats, and sends as an HTTP Post
 ******************************************************************************/
static void cellTXTask(void *p_arg) {
	RTOS_ERR err;
	CPU_TS ts;
	PP_UNUSED_PARAM(p_arg);
	HttpRequest_t requestType = POST;
	uint32_t iterations = 0;
	char message[100];
	char local_in_cpy[LOCAL_IN_BUFF_LEN];
	httpClientInitConnection(&myXbee, FALSE);
	OSTaskCreate(&localInTaskTCB, /* Create the Local in Task.*/
	"Local In Task", localInTask,
	DEF_NULL,
	LOCAL_IN_TASK_PRIO, &localInTaskStk[0], (LOCAL_IN_TASK_STK_SIZE / 10u),
	LOCAL_IN_TASK_STK_SIZE, 0u, 0u,
	DEF_NULL, (OS_OPT_TASK_STK_CLR ), &err);
	/* Reset the connection flag to guarantee repolling of connection status */
	myXbee.wpan_dev.flags &= ~(WPAN_FLAG_JOINED);

	int count = 0;

	while (1) {
		//setLed1(0, 0, 16000);//Set Blue for idle
		OSTimeDly(MIN_CONN_IVL, OS_OPT_TIME_DLY, &err); //Send data at most 1 time per second
		setLed1(16000, 16000, 0); //Set yellow for pending/processing
		OSSemPend(&CellTXBuffSem, //Wait till data is signaled ready by the Local in function
					0,
					OS_OPT_PEND_BLOCKING, &ts, &err);
		OSMutexPend((OS_MUTEX *) &CellTXBuffMutex, (OS_TICK) 0,
				(OS_OPT) OS_OPT_PEND_BLOCKING, (CPU_TS*) &ts, (RTOS_ERR*) &err);
		strcpy(local_in_cpy, local_in_buff); //Read in the data and clear the Mutex - Prevents long blocking from cell and lost data
		local_in_buff[0] = 0; //Clear the buffer, probably not needed but it makes me feel good, so i left it.
		local_in_buff_count = 0; //This part and the SEM are probably redundant, but we may use this elsewhere
		OSMutexPost(&CellTXBuffMutex, OS_OPT_POST_NONE, &err);
		//ECB TODO Format data here?
		clearScreen(); //Still sloppy, but it makes the readout pretty

#ifdef LCDLOG
		printf("\033[H" "\tSending Data: %s       \r\n", local_in_cpy);
#else
		strcpy(message, "Sending Data: ");
		strcat(strcat(message, local_in_cpy), "\r\n");
		uartSend(message);
#endif
		setLed1(16000, 0, 0); //Set red for sending


//		/* print RSSI and carrier profile and nbiot */
//		if(count++ > 2)
//		{
//				  int ret = printConnectionStatus(&myXbee);
//				  if (ret < 0)
//				 {
//					uartSend("Error printing RSSI and Carrier Profile\r\n\r\n");
//				 }
//		}
		int var = sendDataToCloud(local_in_cpy, (const char *) NULL,
				(const char *) requestType, (char *) NULL, -1);
		sprintf(message, "Sent full packet. Error code is %d\r\n\r\n", var);
		uartSend(message);
		setLed1(0, 16000, 0); //Set green for sent This is usually to quick to see
		if(!var)
		{
#ifdef LCDLOG
			printf("\033[H" "\n\n\nSend Success\r\n"); //Prints on 4th line
#else
			strcpy(message, "Send Success\r\n\r\n");
			uartSend(message);
#endif
		}
		else
		{
#ifdef LCDLOG
			printf("\033[H" "\n\n\nError sending. Please try again\r\n");
#else
			strcpy(message, "Error sending. Please try again.\r\n\r\n");
			uartSend(message);
#endif
		}
		local_in_cpy[0] = 0;		// clear buffer
		OSSemSet(&CellTXBuffSem, 0, &err); //Reset the SEM to ensure we have a new reading on the next send
#ifdef DEV_DATA_LIMIT
		if (++iterations >= DEV_DATA_LIMIT) {
			//Grab the mutex and block - Halt everything relying on this
			OSMutexPend((OS_MUTEX *) &CellTXBuffMutex, (OS_TICK) 0,
					(OS_OPT) OS_OPT_PEND_BLOCKING, (CPU_TS*) &ts,
					(RTOS_ERR*) &err);
			clearScreen();
#ifdef LCDLOG
			printf(
					"\033[H" "DEV MODE:\r\nMAX DATA SENT\r\nSENT %d UPDATES\r\nHALTING ALL TASKS\r\n\r\nPress button 0 to send all packets again. Press button 1 to send one more packet.\r\n\r\n",
					iterations);
#else
			strcpy(message, "DEV MODE:\r\nMAX DATA SENT\r\nSENT ");
			char numAsc[2];
			itoa(iterations, numAsc, 10);
			strcat(strcat(message, numAsc), " UPDATES\r\nHALTING ALL TASKS\r\n\r\n");
			uartSend(message);
			strcpy(message, "Press button 0 to send all packets again. Press button 1 to send one more packet.\r\n\r\n");
			uartSend(message);
#endif
			while (1) {
				if(!GPIO_PinInGet(BUTTON_PORT, BUTTON0_PIN)){  // pb0 send all again
					iterations = 0;
					break;
				}
				else if(!GPIO_PinInGet(BUTTON_PORT, BUTTON1_PIN)){	// pb1 send one more
					break;
				}
			}
#ifdef LCDLOG
			clearScreen();
#endif
			OSMutexPost(&CellTXBuffMutex, OS_OPT_POST_NONE, &err);
		}
#endif

	}

}

/***************************************************************************//**
 * @brief Prints relevant information about the connected XBee
 ******************************************************************************/
static void printDeviceDescription(void) {
#ifdef LCDLOG
	printf("\fHardware Version:\n%x\n"
			"Firmware Version:\n%x\n"
			"Baud Rate:\n%u\n"
			"Connection:\n%d\n", (unsigned int) myXbee.hardware_version,
			(unsigned int) myXbee.firmware_version,
			(unsigned int) myXbee.serport.baudrate,
			(WPAN_DEV_IS_JOINED(&(myXbee.wpan_dev))));
#else
	char message[100];
	sprintf(message, "\fHardware Version: %x\r\n"
				"Firmware Version: %x\r\n"
				"Baud Rate: %u\r\n"
				"Connection: %d\r\n\r\n", (unsigned int) myXbee.hardware_version,
				(unsigned int) myXbee.firmware_version,
				(unsigned int) myXbee.serport.baudrate,
				(WPAN_DEV_IS_JOINED(&(myXbee.wpan_dev))));
	uartSend(message);
#endif
}
//********************************************
// Main task
// inputs: none
// outputs: none
//********************************************
static void mainStartTask(void *p_arg) {
	RTOS_ERR err;
	int ret;

	char message[100];

	PP_UNUSED_PARAM(p_arg); /* Prevent compiler warning.                            */

	BSP_TickInit(); /* Initialize Kernel tick source.                       */

	Common_Init(&err); /* Call common module initialization example.           */

#ifdef LCDLOG
	clearScreen();
	printf("Starting Application...\r\n\r\n");
#else
	clearTerminal();
	strcpy(message, "Starting Application...\r\n\r\n");
	uartSend(message);
#endif
	/* button initialization */
	CMU_ClockEnable(cmuClock_GPIO, true);
	GPIO_PinModeSet(BUTTON_PORT, BUTTON0_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(BUTTON_PORT, BUTTON1_PIN, gpioModeInput, 0);
	//GPIO_PinModeSet(gpioPortH, 10, gpioModePushPull, 0); // red LED0 for debugging

	APP_RTOS_ASSERT_CRITICAL(err.Code == RTOS_ERR_NONE, ;);

	gnssInit();

	/* Give time for the xbee to power up */
	OSTimeDly(1000, OS_OPT_TIME_DLY, &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), ;);

	/* Initialize the command layer and print resulting description */
	if (initXBee() != 0) {
		while (1); /* Blocks if there was an error with opening communications */
	}

	OSTimeDly(1000, OS_OPT_TIME_DLY, &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), ;);

	printDeviceDescription();

	OSTimeDly(3000, OS_OPT_TIME_DLY, &err);
	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), ;);

	/* Reset the connection flag to guarantee repolling of connection status */
	myXbee.wpan_dev.flags &= ~(WPAN_FLAG_JOINED);

	/* Blocking wait for a cell connection */
#ifdef LCDLOG
	printf("\fWait for cell signal\n");
#else
	strcpy(message, "Wait for cell signal\r\n");
	uartSend(message);
#endif
	do {
		checkConnection(&myXbee);
		OSTimeDly(500, OS_OPT_TIME_DLY, &err);
#ifdef LCDLOG
		printf(".");
#else
		strcpy(message, ".");
		uartSend(message);
#endif
	} while (!(myXbee.wpan_dev.flags & WPAN_FLAG_JOINED));		// make sure cell connection formed before moving on
#ifdef LCDLOG
	clearScreen();
#else
	uartSend("\r\n\r\n");
#endif

#ifdef LCDLOG
	printf("\033[H""Initialized. Starting New Tasks\r\n\r\nPress any button to continue\r\n\r\n");
#else
	strcpy(message, "Initialized. Starting New Tasks\r\nPress any button to continue\r\n\r\n");
	uartSend(message);
#endif
	while(1)
	{
		if(!GPIO_PinInGet(BUTTON_PORT, BUTTON0_PIN))
		{
			break;
		}
		else if(!GPIO_PinInGet(BUTTON_PORT, BUTTON1_PIN))
		{
			break;
		}
	}
#ifdef LCDLOG
	printf("Attempting to send...Please wait\r\n\r\n");
#else
	strcpy(message, "Attempting to send... Please wait\r\n\r\n");
	uartSend(message);
#endif
	OSTimeDly(500, OS_OPT_TIME_DLY, &err);
//	Moved this to cellTxTask to prevent input being lost during cellular config
//  OSTaskCreate(&localInTaskTCB, /* Create the Local in Task.*/
//             "Local In Task",
//             localInTask,
//             DEF_NULL,
//             LOCAL_IN_TASK_PRIO,
//             &localInTaskStk[0],
//             (LOCAL_IN_TASK_STK_SIZE / 10u),
//             LOCAL_IN_TASK_STK_SIZE,
//             0u,
//             0u,
//             DEF_NULL,
//             (OS_OPT_TASK_STK_CLR),
//             &err);

	OSTaskCreate(&cellTXTaskTCB, /* Create the Local in Task.*/
	"Cell Tx Task", cellTXTask,
	DEF_NULL,
	CELL_TX_TASK_PRIO, &cellTXTaskStk[0], (CELL_TX_TASK_STK_SIZE / 10u),
	CELL_TX_TASK_STK_SIZE, 0u, 0u,
	DEF_NULL, (OS_OPT_TASK_STK_CLR ), &err);
}
