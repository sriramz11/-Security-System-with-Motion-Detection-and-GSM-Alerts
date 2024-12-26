/*
 * uart.h
 *
 *  Created on: Dec 11, 2024
 *      Author: srira
 */
#ifndef UART_H_
#define UART_H_

/* INCLUDE STANDARD HEADERS */
#include <stdint.h>

/* INCLUDE STM32F0 SPECIFIC HEADERS */
#include "stm32f091xc.h"
#include "stm32f0xx.h"

/* CONFIGURATION DEFINITIONS */
#define TX                         2       /* UART TX PIN (PA2) */
#define RX                         3       /* UART RX PIN (PA3) */
#define NVIC_PRIORITY_USART2       1       /* INTERRUPT PRIORITY FOR USART2 */

#define SYS_CLOCK                  24000000 /* SYSTEM CLOCK FREQUENCY IN HZ */
#define Baud_rate                  19200     /* UART BAUD RATE */
#define BACKSPACE                  0x08     /* ASCII CODE FOR BACKSPACE */

/* FUNCTION DECLARATIONS */
void UART_Initialize(void);              /* INITIALIZE UART2 */
void UART_PrintString(const char* s);    /* PRINT A STRING OVER UART */
void UART_PrintHex(uint8_t value);       /* PRINT HEX VALUE OVER UART */
void UART_HandleBackspace(void);         /* HANDLE BACKSPACE CHARACTER */
void UART_ShowMenu(void);                /* DISPLAY USER MENU */
void UART_ProcessCommand(char cmd);      /* PROCESS USER COMMANDS RECEIVED OVER UART */
void UART_AlertMovement(void);           /* ALERT USER OF DETECTED MOVEMENT */

#endif /* UART_H_ */

