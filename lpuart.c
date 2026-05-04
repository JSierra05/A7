/**
  ******************************************************************************
  * @file           : lpuart.c
  * @brief          : Header for led.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  */

#include "lpuart.h"

static uint32_t LPUARTDIV = 8888; // 115.2 kHz baud Rate

void LPUART_init( void )
{
	PWR->CR2 		|=  (PWR_CR2_IOSV);				// power avail on PG[15:2] (LPUART1)
	RCC->AHB2ENR 	|=  (RCC_AHB2ENR_GPIOGEN);   	// enable GPIOG clock
	RCC->APB1ENR2 	|=  (RCC_APB1ENR2_LPUART1EN); // enable LPUART clock bridge

	// TX - PG7, RX - PG8
	// Configure GPIOG — AF mode, push-pull, no pull, high speed
	GPIOG->MODER 	&= ~(GPIO_MODER_MODE7 |
							  GPIO_MODER_MODE8);
	GPIOG->MODER 	|=	 (GPIO_MODER_MODE7_1 |
							  GPIO_MODER_MODE8_1);		// Alternate Function Mode
	GPIOG->OSPEEDR |=  (GPIO_OSPEEDR_OSPEED7 |
							  GPIO_OSPEEDR_OSPEED8);	// High Speed
	GPIOG->OTYPER 	&= ~(GPIO_OTYPER_OT7 |
							  GPIO_OTYPER_OT8);			// Push-Pull Output
	GPIOG->PUPDR 	&= ~(GPIO_PUPDR_PUPD7 |
							  GPIO_PUPDR_PUPD8);			// No Pull Up / Down
	// select LPUART1_TX (AF8) on PG7 (AFRL)
	GPIOG->AFR[0]  |= GPIO_AFRL_AFSEL7_3;
	// select LPUART1_RX (AF8) on PG8 (AFRH)
	GPIOG->AFR[1]  |= (GPIO_AFRH_AFSEL8_3);

	// Configure LPUART1

	LPUART1->CR1 &= ~(USART_CR1_M1 | USART_CR1_M0); // 8-bit data
	LPUART1->CR1 |= USART_CR1_UE;                   // enable LPUART1
	LPUART1->CR1 |= (USART_CR1_TE | USART_CR1_RE);  // enable xmit & recv
	LPUART1->CR1 |= USART_CR1_RXNEIE;        // enable LPUART1 recv interrupt
	LPUART1->ISR &= ~(USART_ISR_RXNE);       // clear Recv-Not-Empty flag

	//Set baud rate register
	LPUART1->BRR &= ~0xFFFF;	// Clear BRR
	LPUART1->BRR |= LPUARTDIV; // Set BRR to desired Baud Rate

	NVIC->ISER[2] = (1 << (LPUART1_IRQn & 0x1F));   // enable LPUART1 ISR
	__enable_irq();                          			// enable global interrupts
}

void LPUART_print( const char* message ) {
   uint16_t iStrIdx = 0;

   while ( message[iStrIdx] != 0 ) {

      while(!(LPUART1->ISR & USART_ISR_TXE)) 	// Wait for empty xmit buffer
         ;

      LPUART1->TDR = message[iStrIdx];       	// Send this character

	iStrIdx++;                             		// Advance index to next char
   }
}

void LPUART_ESC_print( const char* message ) {
   uint16_t iStrIdx = 0;

   while ( !(LPUART1->ISR & USART_ISR_TXE) ) 	// Wait for empty xmit buffer
      ;

   // Send ESC character first

   LPUART1->TDR = 0x1B;

   // Send the rest of the escape sequence

   while ( message[iStrIdx] != 0 ) {
      while ( !(LPUART1->ISR & USART_ISR_TXE) )	// Wait for empty xmit buffer
         ;

      LPUART1->TDR = message[iStrIdx];				// Send this character
      iStrIdx++;											// Advance index to next char
   }
}
//sends sequence to position cursor at row and col
void LPUART_cursor( uint8_t row, uint8_t col){
	char buf[12];
	int i = 0;
	buf[i++] = '[';
	if (row >= 10) buf[i++] = '0' + (row/10);
	buf[i++] = '0' + (row%10);
	buf[i++] = ';';
	if (col >= 10) buf[i++] = '0' + (col/10);
	buf[i++] = '0' + (col % 10);
	buf[i++] = 'H';
	buf[i] = 0;
	LPUART_ESC_print(buf);
}


