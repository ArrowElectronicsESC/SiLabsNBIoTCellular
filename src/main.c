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

#include <stdio.h>
#include <stdlib.h>
#include "em_device.h"
#include "em_cmu.h"
#include "em_chip.h"
#include "em_emu.h"

#include "display.h"
#include "textdisplay.h"
#include "retargettextdisplay.h"

#include "xbee/platform.h"
#include "xbee/device.h"
#include "xbee/ipv4.h"
#include "xbee/atcmd.h"
#include "xbee/byteorder.h"

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


/*
 *********************************************************************************************************
 *********************************************************************************************************
 *                                             LOCAL DEFINES
 *********************************************************************************************************
 *********************************************************************************************************
 */

#define  MAIN_START_TASK_PRIO              21u
#define  MAIN_START_TASK_STK_SIZE          2048u

#define  LOCAL_IN_TASK_PRIO             	 26u //Below Cellular priority to prevent conflict
#define  LOCAL_IN_TASK_STK_SIZE				     1024u

#define  CELL_TX_TASK_PRIO             	   20u
#define  CELL_TX_TASK_STK_SIZE				     1024u

//ECB TODO Add Cell RX here


#define LOCAL_IN_BUFF_LEN                 256u //This will be the max command length


#define  XBEE_TARGET_BAUD                 115200
#define  TEMP_UPLOAD_PERIOD               30 /* (Seconds) */
#define  LED_DOWNLOAD_PERIOD              10 /* (Seconds) */

/*
 *********************************************************************************************************
 *********************************************************************************************************
 *                                        LOCAL GLOBAL VARIABLES
 *********************************************************************************************************
 *********************************************************************************************************
 */

/* Start Task Stack.                                    */
static  CPU_STK  mainStartTaskStk[MAIN_START_TASK_STK_SIZE];
/* Start Task TCB.                                      */
static  OS_TCB   mainStartTaskTCB;

/* Local in Task Stack and TCB                                  */
static  CPU_STK  localInTaskStk[LOCAL_IN_TASK_STK_SIZE];
static  OS_TCB   localInTaskTCB;

/* Cell TX Task Stack and TCB                                  */
static  CPU_STK  cellTXTaskStk[CELL_TX_TASK_STK_SIZE];
static  OS_TCB   cellTXTaskTCB;

//ECB TODO add Cell Rx here

xbee_dev_t myXbee;
GnssSln_t sln;

OS_SEM        CellTXBuffSem; //Flag to say data is ready in local_in_buff
OS_MUTEX      CellTXBuffMutex; //Mutex to prevent concurrent access to the buffer AND count

char local_in_buff[LOCAL_IN_BUFF_LEN] = {NULL}; //Buffer to transfer data between tasks
uint32_t local_in_buff_count = 0; //lenght of data in buffer - Must be updated w/ every snprintf or strcpy

//ECB Todo add Cell RX and/or wifi details here

/*
 *********************************************************************************************************
 *********************************************************************************************************
 *                                       LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************
 *********************************************************************************************************
 */

static void  mainStartTask (void  *p_arg);
static void  localInTask (void  *p_arg);
static void  cellTXTask (void  *p_arg);
static void  createDataStreams(void); //Not used
static void  printDeviceDescription(void);
static void  initCommandLayer(void);
static int   updateLeds(void); //Not Used
static int   updateTemp(void); //Not Used
static int   initXBee(void);
void clearScreen(void);

void clearScreen(void){ //Its sloppy - I know
	int i, j;
	for(i = 0; i < 20;i++){
		//for(j = 0; j < 20; j++){
			printf("                     ");//Print 20 spaces to "Clear" the screen
		//}
	}
}

int main(void)
{
  RTOS_ERR err;

  CPU_Init();                                                   /* Initialize CPU.                                      */
  BSP_SystemInit();                                             /* Initialize System.                                   */

  OSInit(&err);                                                 /* Initialize the Kernel.                               */
                                                                /*   Check error code.                                  */
  APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

  /* Initialize the display module. */
  DISPLAY_Init();
  /* Retarget stdio to a text display. */
  if (RETARGET_TextDisplayInit() != TEXTDISPLAY_EMSTATUS_OK) {
    while (1) ;
  }
  /* Setup LED1 */
  setupRgbLed1();
  /* Setup temperature and RH sensor */
  initTempSensor();
  //Create Mutex for Cell TX Buffer
  OSMutexCreate(&CellTXBuffMutex,
                "CELL Tx Buff/Cnt MTX",
                 &err);
  //Create data ready sem
  OSSemCreate(&CellTXBuffSem,
              "Cell Tx Ready",
              0,
              &err);
  OSTaskCreate(&mainStartTaskTCB,                            /* Create the Start Task.                               */
               "Main Start Task",
               mainStartTask,
               DEF_NULL,
               MAIN_START_TASK_PRIO,
               &mainStartTaskStk[0],
               (MAIN_START_TASK_STK_SIZE / 10u),
               MAIN_START_TASK_STK_SIZE,
               0u,
               0u,
               DEF_NULL,
               (OS_OPT_TASK_STK_CLR),
               &err);


  /*   Check error code.                                  */
  APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);

  OSStart(&err);                                                /* Start the kernel.                                    */
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
 * @brief Custom configuration of XBee to work correctly with Hologram SIM card
 * @retval 0  Successfully configured XBee
 * @retval -EIO Could not configure XBeee
 ******************************************************************************/
//TODO ECB configure for AT&T
int configureXbeeForHologramSim(void)
{
  char buff[64];
  uint8_t i;
  uint32_t rxTimeout;
  int ch;
  RTOS_ERR err;

  if (configureXBee(&myXbee,
                    XBEE_TARGET_BAUD,
                    XBEE_CONFIG_MODE_BYPASS,
                    XBEE_CONFIG_CARRIER_AUTO_DETECT,
                    TRUE)) {
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
                    XBEE_TARGET_BAUD,
                    XBEE_CONFIG_MODE_API,
                    XBEE_CONFIG_CARRIER_AUTO_DETECT,
                    TRUE)) {
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
static int initXBee(void)
{
  xbee_serial_t ser1;
  ser1.baudrate = XBEE_TARGET_BAUD;
  /* Initialize the XBee device (identifies device) */
  if (xbee_dev_init(&myXbee, &ser1, NULL, NULL)) {
    printf("\fCould not open communication with XBee\n");
    return -EIO;
  }

#if defined(XBEE_DEMO_CONFIG) && defined(XBEE_DEMO_HOLOGRAM_SIM)
  printf("\fConfigure XBee for \nHologram SIM");
  int retval = configureXbeeForHologramSim();
  if (retval) {
    printf("\fFailed to config XBee\n"
           "for Hologram SIM.\n"
           "\nCheck that the XBee\n"
           "is powered and is\n"
           "connected properly");
    return retval;
  }
#endif

  /* Command layer initialization (AT commands) */
  printf("\fInitializing");
  initCommandLayer();

  /* Check for successful communication with the XBee */
  if (myXbee.hardware_version == 0) {
    printf("\fFailed to initialize\nattempting to\nreconfigure the XBee\n");
#if defined(XBEE_DEMO_CONFIG) && !defined(XBEE_DEMO_HOLOGRAM_SIM)
    if (configureXBee(&myXbee,
                      XBEE_TARGET_BAUD,
                      XBEE_CONFIG_MODE_API,
                      XBEE_CONFIG_CARRIER_AUTO_DETECT,
                      TRUE)) {
      printf("\fFailed to config\n"
             "check that the XBee\n"
             "is powered and is\n"
             "connected properly");
      return -EIO;
    }

    /* Reinitialize with correct settings */
    myXbee.flags &= ~XBEE_DEV_FLAG_CMD_INIT;
    myXbee.flags &= ~XBEE_DEV_FLAG_QUERY_BEGIN;
    printf("\fInitializing");
    initCommandLayer();

#else
    /* Without the config code, we cannot continue */
    printf("\nCould not reconfig,\ninclude config code");
    return -EIO;
#endif
  }

#if defined(XBEE_DEMO_CONFIG) && defined(XBEE_CHANGE_APN)
  /* Attempt to Apply the correct APN as defined by XBEE_TARGET_APN */
  printf("\fAttempting to apply\nthe new APN\n");
  int ret = configureAPN(&myXbee);
  if (ret < 0) {
    printf("\fFailed to apply\nthe new APN\n");
  } else if (ret == 1) {
    printf("The APN was already\nset correctly!\n");
  } else {
    printf("The new APN has been\nset correctly!\n");
  }
#endif

  return 0;
}

/***************************************************************************//**
 * @brief Helps form fixed point values (used for GNSS and temperature)
 ******************************************************************************/
static int strFixedPt(char *buffer,
                      size_t bufsz,
                      int32_t k)
{
  int ret;
  int32_t integer = k / 1000;
  int32_t fraction = k % 1000;

  // Determine the correct precision for the fractional portion
  if ((fraction % 10) == 0) {
    if ((fraction % 100 == 0)) {
      if ((fraction % 1000 == 0)) {
        ret = snprintf(buffer, bufsz, "%ld", integer);
      } else {
        ret = snprintf(buffer, bufsz, "%ld.%01ld", integer, fraction / 100);
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
static void createDataStreams(void)
{
  printf("\fCreating Data Streams\n");
  bool initTemperature = 0;
  bool initRGB = 0;
  int retries = 0;
  int i;
  RTOS_ERR  err;

  /* Use flags to confirm that both streams have been created successfully */
  while (!(initTemperature && initRGB)) {
    checkConnection(&myXbee);

    /* Print information about the connection */
    printf("\033[H" "\nCTS:%d TX:0x%.2X AI:%.1d\n",
           xbee_ser_get_cts(&(myXbee.serport)), getLastTXStatus(),
           myXbee.wpan_dev.flags & WPAN_FLAG_JOINED);

    if (!initTemperature) {
      /*initTemperature = (initCloudDataStream("Temperature", "DOUBLE", "Celsius", 10))
                        ?  0 : 1;*/
    	initTemperature = 1;
    }
    if (!initRGB) {
      /*initRGB = (initCloudDataStream("RGB", "STRING", "RGB", 10))
                ?  0 : 1;*/
    	initRGB = 1;
    }

    OSTimeDly(300, OS_OPT_TIME_DLY, &err);
    printf("\033[H" "\n\n");
    for (i = 0; i < retries; i++) {
      printf(".");
    }
    if ( retries++ > 100 ) {
      printf("\fFailed to create streams\n"
             "make sure your credentials,\n"
             "remote manager URI and\n"
             "permissions allow for posting\n"
             "to remote manager (reset)\n");
      while (1) {
      }           /* Block and wait for reset */
    }
  }

  printf("\fCreated Data Streams!\n");
  OSTimeDly(300, OS_OPT_TIME_DLY, &err);
}

/***************************************************************************//**
 * @brief Pushes new temperature information to the remote manager
 ******************************************************************************/
static int updateTemp(void)
{
  static uint32_t lastSend = 0;
  char data[32];
  char gnssDat[64];
  int ret = 0;
  uint32_t rh = 0;
  int32_t tmp = 0;

  ret = tempSensorReadHumTemp(&rh, &tmp);
  if (ret < 0) {
    return ret;
  }

  /* If we are due to upload data */
  if (TEMP_UPLOAD_PERIOD <= xbee_seconds_timer() - lastSend) {
    if (strFixedPt(data, sizeof(data), tmp) < 0) {
      ret = snprintf(data, sizeof(data), "ERROR: ENCODING");
    }
    /* Check for viable GNSS data */
    if (gnssSln(&sln) == 0 && sln.fixOK) {
      sprintf(gnssDat, "[%i.%07i, %i.%07i, %i.%03i]",
              (sln.lon / 10000000), abs(sln.lon % 10000000), // 10**-7 deg
              (sln.lat / 10000000), abs(sln.lat % 10000000), // 10**-7 deg
              (sln.height / 1000), abs(sln.height % 1000)); // 10**-3 m)
      ret = sendDataToCloud(data, "Temperature", "DOUBLE", gnssDat, 10);
    } else {
      ret = sendDataToCloud(data, "Temperature", "DOUBLE", NULL, 10);
    }
    /* If the send was successful then update timer and display data */
    if (ret == 0) {
      lastSend = xbee_seconds_timer();
      printf("temp (C):\n%.7s             \n", data);
    }
  }
  return ret;
}

/***************************************************************************//**
 * @brief Requests new LED values from the remote manager and then changes LEDs
 ******************************************************************************/
static int updateLeds(void)
{
  static uint32_t lastSend = 0;
  char data[32];
  uint32_t r, g, b;
  int ret;

  if (LED_DOWNLOAD_PERIOD <= xbee_seconds_timer() - lastSend) {
    ret = getDataFromCloud(data, "RGB", 10);
    /* Update colors and display only if we goto valid data */
    if (ret == 0) {
      lastSend = xbee_seconds_timer();

      if (sscanf(data, "%" PRIu32 " , %" PRIu32 " , %" PRIu32, &r, &g, &b) == 3) {
        /* Must be less than or equal to PWM_STEPS (255) */
        if (r > PWM_STEPS) {
          r = PWM_STEPS;
        }
        if (g > PWM_STEPS) {
          g = PWM_STEPS;
        }
        if (b > PWM_STEPS) {
          b = PWM_STEPS;
        }

        setLed1(r, g, b);
        printf("RGB:\n%" PRIu32 ",%" PRIu32 ",%" PRIu32 "      \n", r, g, b);
      } else {
        printf("RGB:\nMalfromed\n");
      }
    }
  }
  return ret;
}

/***************************************************************************//**
 * @brief Helper Function for initializing the command layer
 ******************************************************************************/
static bool isCmdQueryFinished(xbee_dev_t *xbee)
{
  int ret = xbee_cmd_query_status(xbee);

  if (ret == XBEE_COMMAND_LIST_DONE
      || ret == XBEE_COMMAND_LIST_TIMEOUT) {
    return true;
  }
  return false;
}

/***************************************************************************//**
 * @brief Gets basic device information, only calls init once
 ******************************************************************************/
static void initCommandLayer(void)
{
  RTOS_ERR err;
  printf("\fInitializing");
  xbee_cmd_init_device(&myXbee);
  /* Wait for XBee to respond */
  while (!isCmdQueryFinished(&myXbee)) {
    xbee_dev_tick(&myXbee);
    printf(".");
    OSTimeDly(100, OS_OPT_TIME_DLY, &err);
    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE),; );
  }
}


/***************************************************************************//**
 * @brief Reads data in over the UART, stores, and sets semaphore
 ******************************************************************************/
static void localInTask(void *p_arg){
  RTOS_ERR  err;
  CPU_TS  ts;
  PP_UNUSED_PARAM(p_arg);     /* Prevent compiler warning. */
  int ret = 0;
  uint32_t rh = 0;
  int32_t tmp = 0;
  char tmpData[32],rhData[32];

  //Initialize the UART (or SPI) here
  //ECB TODO Add UART config here
  //Clear the buffers here, aquire the mutex
  OSMutexPend((OS_MUTEX *)&CellTXBuffMutex,(OS_TICK)0,(OS_OPT)OS_OPT_PEND_BLOCKING,(CPU_TS*)&ts,(RTOS_ERR*)&err);
  local_in_buff_count = 0;
  OSMutexPost(&CellTXBuffMutex, OS_OPT_POST_NONE, &err);
  while(1){ //Infinite loop that yields when possible
    //ECB TODO Add "IF UART DATA "
    //if(uartRxData>0){}
    ret = tempSensorReadHumTemp(&rh, &tmp);
    if(strFixedPt(tmpData, sizeof(tmpData), tmp) < 0 || ret < 0){
    	strcpy(tmpData,"ERROR_TMP");
    }
    if(strFixedPt(rhData, sizeof(rhData), rh) < 0 || ret < 0){
    	strcpy(rhData,"ERROR_RH");
    }
    //printf("T:%s H:%s\r\n",tmpData,rhData);
    OSMutexPend((OS_MUTEX *)&CellTXBuffMutex,(OS_TICK)0,(OS_OPT)OS_OPT_PEND_BLOCKING,(CPU_TS*)&ts,(RTOS_ERR*)&err);
    //ret = strcpy(local_in_buff, "TESTING THE INPUT");
    ret = snprintf(local_in_buff,LOCAL_IN_BUFF_LEN,"Temp: %s Hum: %s",tmpData,rhData);
    local_in_buff_count = ret;
    OSMutexPost(&CellTXBuffMutex, OS_OPT_POST_NONE, &err);
    OSSemPost(&CellTXBuffSem,
              OS_OPT_POST_1,
              &err);
    OSTimeDly(10, OS_OPT_TIME_DLY, &err);//This will Update as much as possible but needs to yield to TX
  }
}

/***************************************************************************//**
 * @brief takes data from the Local in Buffer, formats, and sends as an HTTP Post
 ******************************************************************************/
static void cellTXTask(void *p_arg){
  RTOS_ERR err;
  CPU_TS ts;
  PP_UNUSED_PARAM(p_arg);
  HttpRequest_t requestType = POST;
  uint32_t iterations = 0;
  char local_in_cpy[LOCAL_IN_BUFF_LEN];
  httpClientInitConnection(&myXbee, FALSE);
  OSTaskCreate(&localInTaskTCB, /* Create the Local in Task.*/
             "Local In Task",
             localInTask,
             DEF_NULL,
             LOCAL_IN_TASK_PRIO,
             &localInTaskStk[0],
             (LOCAL_IN_TASK_STK_SIZE / 10u),
             LOCAL_IN_TASK_STK_SIZE,
             0u,
             0u,
             DEF_NULL,
             (OS_OPT_TASK_STK_CLR),
             &err);
  while(1){
	//setLed1(0, 0, 16000);//Set Blue for idle
	OSTimeDly(MIN_CONN_IVL, OS_OPT_TIME_DLY, &err);//Send data at most 1 time per second
	setLed1(16000, 16000, 0); //Set yellow for pending/processing
    OSSemPend(&CellTXBuffSem, //Wait till data is signaled ready by the Local in function
              0,
              OS_OPT_PEND_BLOCKING,
              &ts,
              &err);
    OSMutexPend((OS_MUTEX *)&CellTXBuffMutex,(OS_TICK)0,(OS_OPT)OS_OPT_PEND_BLOCKING,(CPU_TS*)&ts,(RTOS_ERR*)&err);
    strcpy(local_in_cpy,local_in_buff); //Read in the data and clear the Mutex - Prevents long blocking from cell and lost data
    local_in_buff[0] = NULL; //Clear the buffer, probably not needed but it makes me feel good, so i left it.
    local_in_buff_count=0; //This part and the SEM are probably redundant, but we may use this elsewhere
    OSMutexPost(&CellTXBuffMutex, OS_OPT_POST_NONE, &err);
    //ECB TODO Format data here?
    clearScreen(); //Still sloppy, but it makes the readout pretty
    printf("\033[H" "\tSending Data: %s       \r\n",local_in_cpy);
    setLed1(16000, 0, 0);//Set red for sending
    sendDataToCloud(local_in_cpy, (const char *)NULL,(const char *)requestType,(char *)NULL,-1 );
    setLed1(0, 16000, 0); //Set green for sent//This is usually to quick to see
    printf("\033[H" "\n\n\nSend Success\r\n");//Prints on 4th line
    local_in_cpy[0] = (char)NULL;
    OSSemSet(&CellTXBuffSem,0,&err); //Reset the SEM to ensure we have a new reading on the next send
#ifdef DEV_DATA_LIMIT
    if(++iterations >= DEV_DATA_LIMIT){
    	//Grab the mutex and block - Halt everything relying on this
    	OSMutexPend((OS_MUTEX *)&CellTXBuffMutex,(OS_TICK)0,(OS_OPT)OS_OPT_PEND_BLOCKING,(CPU_TS*)&ts,(RTOS_ERR*)&err);
    	clearScreen();
    	printf("\033[H" "DEV MODE:\r\nMAX DATA SENT\r\nSENT %d UPDATES\r\nHALTING ALL TASKS",iterations);
    	while(1){;}
    }
#endif

  }

}

/***************************************************************************//**
 * @brief Prints relevant information about the connected XBee
 ******************************************************************************/
static void printDeviceDescription(void)
{
  printf("\fHardware Version:\n%x\n"
         "Firmware Version:\n%x\n"
         "Baud Rate:\n%u\n"
         "Connection:\n%d\n",
         (unsigned int) myXbee.hardware_version,
         (unsigned int) myXbee.firmware_version,
         (unsigned int) myXbee.serport.baudrate,
         (WPAN_DEV_IS_JOINED(&(myXbee.wpan_dev))));
}

static void mainStartTask(void *p_arg)
{
  RTOS_ERR  err;

  PP_UNUSED_PARAM(p_arg);                                       /* Prevent compiler warning.                            */

  BSP_TickInit();                                               /* Initialize Kernel tick source.                       */

  Common_Init(&err);                                            /* Call common module initialization example.           */
  APP_RTOS_ASSERT_CRITICAL(err.Code == RTOS_ERR_NONE,; );

  gnssInit();

  /* Give time for the xbee to power up */
  OSTimeDly(1000, OS_OPT_TIME_DLY, &err);
  APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE),; );

  /* Initialize the command layer and print resulting description */
  if (initXBee() != 0) {
    while (1) ; /* Blocks if there was an error with opening communications */
  }

  OSTimeDly(1000, OS_OPT_TIME_DLY, &err);
  APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE),; );

  printDeviceDescription();

  OSTimeDly(3000, OS_OPT_TIME_DLY, &err);
  APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE),; );

  /* Reset the connection flag to guarantee repolling of connection status */
  myXbee.wpan_dev.flags &= ~(WPAN_FLAG_JOINED);

  /* Blocking wait for a cell connection */
  printf("\fWait for cell signal\n");
  do {
    checkConnection(&myXbee);
    OSTimeDly(500, OS_OPT_TIME_DLY, &err);
    printf(".");
  } while (0);//(!(myXbee.wpan_dev.flags & WPAN_FLAG_JOINED));


  printf("\033[H" "\tInitialized\nStarting New Tasks");
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
             "Cell Tx Task",
             cellTXTask,
             DEF_NULL,
             CELL_TX_TASK_PRIO,
             &cellTXTaskStk[0],
             (CELL_TX_TASK_STK_SIZE / 10u),
             CELL_TX_TASK_STK_SIZE,
             0u,
             0u,
             DEF_NULL,
             (OS_OPT_TASK_STK_CLR),
             &err);
}
