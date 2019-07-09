/*
 * uart.h
 *
 *  Created on: Jun 24, 2019
 *      Author: 132792
 */

#ifndef SRC_UART_H_
#define SRC_UART_H_
#include "em_gpio.h"

// defines
#define DEFAULTBAUD 	38400
#define RXPORT			gpioPortH
#define RXPIN 			5
#define TXPORT			gpioPortH
#define TXPIN			4

// function declarations
void uartInit();
void uartSend(char * message);

#endif /* SRC_UART_H_ */
