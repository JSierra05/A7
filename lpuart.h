/**
  ******************************************************************************
  * @file           : lpuart.h
  * @brief          : Header for led.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  */

#include "stm32l4xx_hal.h"

/*------------------ Define to prevent recursive inclusion -------------------*/
#ifndef INC_LPUART_H_
#define INC_LPUART_H_

/*----------------------------- Port Definitions -----------------------------*/

#define UARTPORT GPIOG

/*------------------------------ Pin Definitions -----------------------------*/

#define UART1_TX GPIO_PIN_7
#define UART1_RX GPIO_PIN_8


/*------------------------------ Function Shells -----------------------------*/

void LPUART_init( void );
void LPUART_print( const char *message );
void LPUART_ESC_print( const char *message );
void LPUART_cursor( uint8_t row, uint8_t col);

#endif /* INC_LPUART_H_ */
