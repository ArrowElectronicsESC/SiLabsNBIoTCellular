/***************************************************************************//**
 * # License
 * 
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is Third Party Software licensed by Silicon Labs from a third party
 * and is governed by the sections of the MSLA applicable to Third Party
 * Software and the additional terms set forth below.
 * 
 ******************************************************************************/

#include "xbee_config.h"
#include "xbee/atmode.h"
#include "../src/uart.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define TX_BREAK_RESPONSE_TIME 3000
#define TX_BREAK_FLUSH_TIME 5000
#define RESPONSE_BUFF_SIZE 108
#define XBEE_DEFAULT_APN "-"


#ifndef XBEE_TARGET_APN
#define XBEE_TARGET_APN XBEE_DEFAULT_APN
#endif

#ifndef XBEE_TARGET_APN
#define TARGET_APN XBEE_DEFAULT_APN
#else
#define TARGET_APN XBEE_TARGET_APN
#endif



/***************************************************************************//**
 * @brief Reads the serial buffer, looking for an OK (called after issuing AT)
 * @param xbee  the object to read from
 * @param buff  A small buffer of size RESPONSE_BUFF_SIZE to put response
 * @return the amount of bytes read, this should always be sizeof("OK\r")
 ******************************************************************************/
int getResponse(xbee_dev_t *xbee, char *buff, int buffSize)
{
  int ret;
  int bytesRead;

  *buff = '\0';
  bytesRead = 0;
  do {
    ret = xbee_atmode_read_response(xbee, buff, buffSize, &bytesRead);
  } while (ret == -EAGAIN);

  if (ret < 0) {
    return ret;
  }

  return bytesRead;
}

/***************************************************************************//**
 * @brief Reads 	the response from the XBee to confirm an OK response
 * @param xbee  	the object to read from
 * @param buff  	A small response buffer
 * @param buffSize  Size of buffer
 * @retval  <0  Error during communication
 * @retval  0 	Successfully received an OK
 ******************************************************************************/
int getOKResponse(xbee_dev_t *xbee, char *buff, int buffSize)
{
  int ret = getResponse(xbee, buff, buffSize);
  if (ret < 0) {
    return ret;
  }
  /* Check that we got an OK from the XBEE */
  if (strcmp(buff, "OK")) {
    return -EIO;
  }
  
  return 0;
}

/***************************************************************************//**
 * @brief Enters command mode
 * @param xbee  the XBee object to use
 * @note  This clears any pending data, make sure all XBee data has been handled
 * before calling this function
 ******************************************************************************/
int enterCommandMode(xbee_dev_t *xbee, char *buff)
{
  uint32_t startTime;
  int ret;
  
  xbee_ser_break(&xbee->serport, TRUE);
  startTime = xbee_millisecond_timer();
  
  while (xbee_millisecond_timer() - startTime < TX_BREAK_FLUSH_TIME) {}
  
  xbee_ser_rx_flush(&xbee->serport); /* Flush any previous data */
  xbee_ser_tx_flush(&xbee->serport); /* Flush any previous data */

  /* We will see data at 9600 baud after break */
  xbee_ser_baudrate(&xbee->serport, 9600);
  
  startTime = xbee_millisecond_timer();
  
  /* After a fixed time, give up on XBee to enter command mode */
  do
  {
    /* We are waiting for an OK response */
    xbee->mode_timer = xbee_millisecond_timer();
    xbee->mode = XBEE_MODE_WAIT_RESPONSE;

    ret = getResponse(xbee, buff, RESPONSE_BUFF_SIZE);

    if (ret < 2) {
      ret = -ETIMEDOUT;
      continue;
    }

    /* Check that we got an OK from the XBEE (skipping modem frame) */
    if (strstr(buff + (ret - 2), "OK") != NULL) {
      ret = 0;
      break;
    }
  } while (xbee_millisecond_timer() - startTime < TX_BREAK_RESPONSE_TIME);

  xbee_ser_break(&xbee->serport, FALSE);

  return ret;
}

/***************************************************************************//**
 * @brief Exits command mode
 * @param xbee  the XBee object to use
 * @param buff  A small buffer of size RESPONSE_BUFF_SIZE to put response
 * @param baud  The baudrate to set this device after exiting command mode
 * @note  This clears any pending data, make sure all XBee data has been handled
 * before calling this function
 ******************************************************************************/
int exitCommandMode(xbee_dev_t *xbee, char *buff)
{
  uint32_t startTime;
  int ret;

  /* Exit command mode and continue with execution */
  xbee_atmode_send_request(xbee, "CN");
  ret = getOKResponse(xbee, buff, RESPONSE_BUFF_SIZE);
  if (ret < 0) {
    goto ERR_EXIT;
  }

  /* Block wait for a second to make sure we left command mode */
  startTime = xbee_millisecond_timer();
  while (xbee_millisecond_timer() - startTime  < 1000) {}


  ret = 0;
ERR_EXIT:
  /* Flush any previous data */
  xbee_ser_rx_flush(&xbee->serport);

  return ret;
}
/***************************************************************************//**
 * @brief prints rssi and network connected to
 * @param xbee  the object to read from
 * @return 0 if successful -1 if unsuccessful
 ******************************************************************************/
int printConnectionStatus(xbee_dev_t *xbee)
{
	int ret;
	char buff[RESPONSE_BUFF_SIZE];
	char message[100];
	RTOS_ERR err;
	/* Make sure we are operating on a valid xbee */
	if (xbee == NULL) {
		return -EINVAL;
	}
	/* Make sure we are operating on a valid serial port */
	if (xbee_ser_invalid(&xbee->serport)) {
		return -EINVAL;
	}

	ret = enterCommandMode(xbee, buff);
	if ( ret < 0)
	{
		exitCommandMode(xbee, buff);
		/* Switch to the original baud rate */
		xbee_ser_baudrate(&xbee->serport, 115200);
		return -1;
	}


	/*print cellular network*/
	ret = xbee_atmode_send_request(xbee, "CP");
	ret = getResponse(xbee, buff, RESPONSE_BUFF_SIZE);
	if ( ret < 0)
	{
		uartSend("Error Sending to XBee\r\n\r\n");
		exitCommandMode(xbee, buff);
		/* Switch to the original baud rate */
		xbee_ser_baudrate(&xbee->serport, 115200);
		return -1;
	}
	if(!strcmp(buff, "0"))
	{
#ifdef LCDLOG
		printf("Carrier Profile: Set to autodetect\r\n\r\n");
#else
		uartSend("Carrier Profile: Set to autodetect\r\n\r\n");
#endif
	}
	else if(!strcmp(buff, "1"))
	{
#ifdef LCDLOG
		printf("Carrier Profile: No Profile\r\n\r\n");
#else
		uartSend("Carrier Profile: No Profile\r\n\r\n");
#endif
	}

	else if(!strcmp(buff, "2"))
	{
#ifdef LCDLOG
		printf("Connecting to ATT\r\n\r\n");
#else
		uartSend("Connecting to ATT\r\n\r\n");
#endif
	}
	else if(!strcmp(buff, "3"))
	{
#ifdef LCDLOG
		printf("Connecting to Verizon\r\n\r\n");
#else
		uartSend("Connecting to Verizon\r\n\r\n");
#endif
	}

//	/* enable RSSI pwm output */
//	ret = xbee_atmode_send_request(xbee, "P01");
//	ret = getResponse(xbee, buff, RESPONSE_BUFF_SIZE);
//	sprintf(message, "Setting PWM RSSI pin success: %s\r\n\r\n", buff);
//	uartSend(message);
//	if ( ret < 0)
//	{
//		uartSend("Error Sending to XBee\r\n\r\n");
//		exitCommandMode(xbee, buff);
//		/* Switch to the original baud rate */
//		xbee_ser_baudrate(&xbee->serport, 115200);
//		return -1;
//	}
//	RTOS_ERR err;
//	/* receive signal strength*/
//	OSTimeDly(3000, OS_OPT_TIME_DLY, &err);
//	ret = xbee_atmode_send_request(xbee, "DB");
//	ret = getResponse(xbee, buff, RESPONSE_BUFF_SIZE);
//	sprintf(message, "Receive Signal Strength: %s\r\n\r\n", buff);
//	uartSend(message);
//	if ( ret < 0)
//	{
//		uartSend("Error Sending to XBee\r\n\r\n");
//		exitCommandMode(xbee, buff);
//		/* Switch to the original baud rate */
//		xbee_ser_baudrate(&xbee->serport, 115200);
//		return -1;
//	}
//
//
	/* enable all nbiot bands */
	OSTimeDly(3000, OS_OPT_TIME_DLY, &err);
	ret = xbee_atmode_send_request(xbee, "BN0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
	ret = getResponse(xbee, buff, RESPONSE_BUFF_SIZE);
	sprintf(message, "Setting bandmask enabling all NBIoT bands: %s\r\n\r\n", buff);
	uartSend(message);
	if ( ret < 0)
	{
		uartSend("Error Sending to XBee\r\n\r\n");
		exitCommandMode(xbee, buff);
		/* Switch to the original baud rate */
		xbee_ser_baudrate(&xbee->serport, 115200);
		return -1;
	}

	/* check nbiot bands */
		OSTimeDly(3000, OS_OPT_TIME_DLY, &err);
		ret = xbee_atmode_send_request(xbee, "BN");
		ret = getResponse(xbee, buff, RESPONSE_BUFF_SIZE);
		sprintf(message, "Checking bandmask enabling all NBIoT bands: %s\r\n\r\n", buff);
		uartSend(message);
		if ( ret < 0)
		{
			uartSend("Error Sending to XBee\r\n\r\n");
			exitCommandMode(xbee, buff);
			/* Switch to the original baud rate */
			xbee_ser_baudrate(&xbee->serport, 115200);
			return -1;
		}

	/* Read network technology */										// may not work with current radio
	ret = xbee_atmode_send_request(xbee, "N#");
	ret = getResponse(xbee, buff, RESPONSE_BUFF_SIZE);
	sprintf(message, "Network technology configured to: %s\r\n\r\n", buff);
	uartSend(message);
	if ( ret < 0)
	{
		uartSend("Error Sending to XBee\r\n\r\n");
		exitCommandMode(xbee, buff);
		/* Switch to the original baud rate */
		xbee_ser_baudrate(&xbee->serport, 115200);
		return -1;
	}

	/* change to NB-IoT */
	ret = xbee_atmode_send_request(xbee, "N#3");
	ret = getResponse(xbee, buff, RESPONSE_BUFF_SIZE);
	sprintf(message, "Change to NBIOT: %s\r\n\r\n", buff);
	uartSend(message);
	if ( ret < 0)
	{
		uartSend("Error Sending to XBee\r\n\r\n");
		exitCommandMode(xbee, buff);
		/* Switch to the original baud rate */
		xbee_ser_baudrate(&xbee->serport, 115200);
		return -1;
	}

/* Write settings to non-volatile memory */
	xbee_atmode_send_request(xbee, "WR");
	ret = getOKResponse(xbee, buff, RESPONSE_BUFF_SIZE);
	sprintf(message, "Writing settings to memory: %s\r\n\r\n", buff);
	uartSend(message);
	if (ret < 0)
	{
		uartSend("Error writing to xbee memory\r\n\r\n");
		exitCommandMode(xbee, buff);
		/* Switch to the original baud rate */
		xbee_ser_baudrate(&xbee->serport, 115200);
		return -1;
	}

	return 0;
}


/***************************************************************************//**
 * @brief Reads the APN and compares to the correct APN
 * @param xbee  the object to read from
 * @retval  <0  Error during communication
 * @retval  0 Successfully set the correct APN
 ******************************************************************************/
int configureAPN(xbee_dev_t *xbee)
{
  char buff[RESPONSE_BUFF_SIZE];
  int ret;
  uint32_t startBaud;
  char message[100];
  /* Make sure we are operating on a valid xbee */
  if (xbee == NULL) {
    return -EINVAL;
  }
  /* Make sure we are operating on a valid serial port */
  if (xbee_ser_invalid(&xbee->serport)) {
    return -EINVAL;
  }

  /* Remember the original baud */
  startBaud = xbee->serport.baudrate;

  /* Enter in command mode at 9600 baud */
  ret = enterCommandMode(xbee, buff);
  if ( ret < 0) {
    goto ERR_EXIT;
  }

  /* Read the existing APN */
  xbee_atmode_send_request(xbee, "AN");
  ret = getResponse(xbee, buff, RESPONSE_BUFF_SIZE);
  if ( ret < 0) {
    goto ERR_EXIT;
  }
  sprintf(message, "Existing APN set as %s\r\n\r\n", buff);
  uartSend(message);

  /* Check if the APN is what we expect, if it isn't try to set it */
  if (strcmp(buff, "arrowiotdemo.com.attz"))
  {
    /* try to set the APN */
    xbee_atmode_send_request(xbee, "AN""arrowiotdemo.com.attz");
    ret = getOKResponse(xbee, buff, RESPONSE_BUFF_SIZE);
    if (ret < 0) {
      goto ERR_EXIT;
    }
    strcpy(message, "New APN set as arrowiotdemo.com.attz\r\n\r\n");
    uartSend(message);

    /* Write settings to non-volatile memory */
    xbee_atmode_send_request(xbee, "WR");
    ret = getOKResponse(xbee, buff, RESPONSE_BUFF_SIZE);
    if (ret < 0)
    {
    	uartSend("Error writing to xbee memory\r\n\r\n");
    	goto ERR_EXIT;
    }
    ret = 0;
  }

  else {
    /* The APN was already set properly */
    ret = 1;
  }
  /* print RSSI and carrier profile and nbiot */
  ret = printConnectionStatus(xbee);
  if (ret < 0)
  {
	uartSend("Error printing RSSI and Carrier Profile\r\n\r\n");
	goto ERR_EXIT;
  }
  ret = 0;
ERR_EXIT:
  exitCommandMode(xbee, buff);
  /* Switch to the original baud rate */
  xbee_ser_baudrate(&xbee->serport, startBaud);

  return ret;

}

int configureXBee(xbee_dev_t *xbee,
                  int baud,
                  enum xbee_mode mode,
                  enum xbee_carrier_profile carrierProfile,
                  bool resetParam)
{
  char buff[RESPONSE_BUFF_SIZE];
  int baudAT;
  int ret;

  /* Make sure we are operating on a valid xbee */
  if (xbee == NULL) {
    return -EINVAL;
  }
  /* Make sure we are operating on a valid serial port */
  if (xbee_ser_invalid(&xbee->serport)) {
    return -EINVAL;
  }

  /* Enter in command mode at 9600 baud */
  ret = enterCommandMode(xbee, buff);
  if ( ret < 0) {
    goto ERR_EXIT;
  }

  if (resetParam) {
    /* Sends a factory reset command */
    xbee_atmode_send_request(xbee, "RE");
    ret = getOKResponse(xbee, buff, RESPONSE_BUFF_SIZE);
    if (ret < 0) {
      goto ERR_EXIT;
    }
  }
  
  /* Sends a API mode change command */
  sprintf(buff, "AP%X", (int) mode);
  xbee_atmode_send_request(xbee, buff);
  ret = getOKResponse(xbee, buff, RESPONSE_BUFF_SIZE);
  if (ret < 0) {
    goto ERR_EXIT;
  }

  /* Sends a carrier profile change command */
  sprintf(buff, "CP%X", (int) carrierProfile);
  xbee_atmode_send_request(xbee, buff);
  ret = getOKResponse(xbee, buff, RESPONSE_BUFF_SIZE);
  if (ret < 0) {
    goto ERR_EXIT;
  }

  switch (baud)
   {
     case 2400:
       baudAT = 0x1;
       break;
     case 4800:
       baudAT = 0x2;
       break;
     case 9600:
       baudAT = 0x3;
       break;
     case 19200:
       baudAT = 0x4;
       break;
     case 38400:
       baudAT = 0x5;
       break;
     case 57600:
       baudAT = 0x6;
       break;
     case 115200:
       baudAT = 0x7;
       break;
     case 230400:
       baudAT = 0x8;
       break;
     case 460800:
       baudAT = 0x9;
       break;
     case 921600:
       baudAT = 0xA;
       break;
     default:
       ret = -EINVAL;
       goto ERR_EXIT;
   }

  /* Send a command to change baudrate */
  sprintf(buff, "BD%X", baudAT);
  xbee_atmode_send_request(xbee, buff);
  ret = getOKResponse(xbee, buff, RESPONSE_BUFF_SIZE);
  if (ret < 0) {
    goto ERR_EXIT;
  }

  /* Write settings to non-volatile memory */
  xbee_atmode_send_request(xbee, "WR");
  ret = getOKResponse(xbee, buff, RESPONSE_BUFF_SIZE);
  if (ret < 0) {
    goto ERR_EXIT;
  }

  ret = 0;

ERR_EXIT:
  exitCommandMode(xbee, buff);
  /* Switch to the original baud rate */
  xbee_ser_baudrate(&xbee->serport, baud);
  
  return ret;
}

