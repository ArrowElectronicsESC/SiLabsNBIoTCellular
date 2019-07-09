/*
 * uart.c
 *
 *  Created on: Jun 24, 2019
 *      Author: 132792
 */

#include "uart.h"
#include <em_usart.h>
#include <em_cmu.h>

//****************************************************************************
// UART initialization function for communication with serial terminal UART4
// inputs: none
// outputs: none
//****************************************************************************

void uartInit(){
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_USART4, true);
	CMU_ClockEnable(cmuClock_GPIO, true);

	USART_InitAsync_TypeDef initAsync = USART_INITASYNC_DEFAULT;
	//initAsync.baudrate = DEFAULTBAUD;
	USART_InitAsync(USART4, &initAsync);

	/* Enable I/O and set location */
	USART4->ROUTEPEN = USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN; 				// tx rx pin enable
	USART4->ROUTELOC0 = USART_ROUTELOC0_TXLOC_LOC4 | USART_ROUTELOC0_RXLOC_LOC4;	// tx rx route to USB COM port

	/* Also enable CS and CLK pins if the USART is configured for synchronous mode.
	 * Set GPIO mode. */
	if (USART4->CTRL & USART_CTRL_SYNC) /* if configured for synchronous mode */
	{
	  //USART0->ROUTE |= USART_ROUTE_CSPEN | USART_ROUTE_CLKPEN;
	  GPIO_PinModeSet(TXPORT, TXPIN, gpioModePushPull, 0);
	  GPIO_PinModeSet(RXPORT, RXPIN, gpioModeInput, 0);
//	  GPIO_PinModeSet((GPIO_Port_TypeDef)AF_USART0_CS_PORT(USER_LOCATION), AF_USART0_CS_PIN(USER_LOCATION), gpioModePushPull, 0);
//	  GPIO_PinModeSet((GPIO_Port_TypeDef)AF_USART0_CLK_PORT(USER_LOCATION), AF_USART0_CLK_PIN(USER_LOCATION), gpioModePushPull, 0);
	}
	else /* non synchronous mode */
	{
	  GPIO_PinModeSet(gpioPortE, 1, gpioModePushPull, 1);		// virtual COM port enable
	  /* To avoid false start, configure TX pin as initial high */
	  GPIO_PinModeSet(TXPORT, TXPIN, gpioModePushPull, 1);
	  GPIO_PinModeSet(RXPORT, RXPIN, gpioModeInput, 0);
	}
	USART_Enable(USART4, usartEnable);
}

//****************************************************************************
// send function that sends contents of array
// inputs: pointer to array to send
// outputs: none
//****************************************************************************
void uartSend(char * message){
	uint8_t i = 0;
	while(message[i++] != '\0'){
		USART_Tx(USART4, message[i - 1]);
	}
}
