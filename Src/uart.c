/*
* uart.c
*
* Created on: Dec 11, 2024
* Author: srira
*/


#include "uart.h" /* INCLUDE HEADER FOR UART FUNCTIONS */

/* STATIC VARIABLE TO TRACK MENU MODE (0 = MENU, 1 = MONITORING MODE) */
static uint8_t menu_mode = 0;

/* MENU TEXT STRING DISPLAYED TO USER */
static const char* MENU_TEXT = "\r\n==== Dorm Room Security System ====\r\n"
                              "1: Print Current Values of MPU6050\r\n"
                              "2: Recalibrate sensor\r\n"
                              "3: About project\r\n"
                              "4: Exit monitoring\r\n"
                              "Enter choice (1-4): ";

/* FUNCTION TO INITIALIZE UART2 */
void UART_Initialize(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN; /* ENABLE CLOCK FOR GPIOA */
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN; /* ENABLE CLOCK FOR USART2 */

    GPIOA->MODER &= ~((3U << (TX * 2)) | (3U << (RX * 2))); /* CLEAR MODER FOR PA2 (TX) AND PA3 (RX) */
    GPIOA->MODER |= (2U << (TX * 2)) | (2U << (RX * 2)); /* SET PA2 AND PA3 TO ALTERNATE FUNCTION MODE */

    GPIOA->AFR[0] |= (1U << (TX * 4)) | (1U << (RX * 4)); /* CONFIGURE ALTERNATE FUNCTION FOR PA2 AND PA3 */


    USART2->BRR = SYS_CLOCK / Baud_rate; /* SET BAUD RATE FOR USART2 */

    USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE | USART_CR1_RXNEIE; /* ENABLE TX, RX, AND INTERRUPT */

    NVIC_EnableIRQ(USART2_IRQn); /* ENABLE NVIC INTERRUPT FOR USART2 */

    NVIC_SetPriority(USART2_IRQn, NVIC_PRIORITY_USART2); /* SET PRIORITY FOR USART2 INTERRUPT */

    UART_PrintString("\r\nDorm Room Security System Initialized\r\n"); /* PRINT SYSTEM INITIALIZATION MESSAGE */
    UART_ShowMenu(); /* DISPLAY THE MAIN MENU */
}

/* FUNCTION TO PRINT A STRING OVER UART */
void UART_PrintString(const char* s) {
    while (*s) { /* LOOP THROUGH EACH CHARACTER IN THE STRING */
        while (!(USART2->ISR & USART_ISR_TXE)); /* WAIT UNTIL TX BUFFER IS EMPTY */
        USART2->TDR = *s++; /* TRANSMIT THE CURRENT CHARACTER */
    }
}

/* FUNCTION TO PRINT A BYTE VALUE AS HEX OVER UART */
void UART_PrintHex(uint8_t value) {
    char hex_chars[] = "0123456789ABCDEF"; /* ARRAY OF HEX CHARACTERS */
    char buffer[5] = "0x00"; /* BUFFER TO HOLD FORMATTED HEX STRING */
    buffer[2] = hex_chars[(value >> 4) & 0x0F]; /* GET HIGH NIBBLE */
    buffer[3] = hex_chars[value & 0x0F]; /* GET LOW NIBBLE */
    UART_PrintString(buffer); /* PRINT THE HEX STRING */
}

/* FUNCTION TO HANDLE BACKSPACE INPUT */
void UART_HandleBackspace(void){

    UART_PrintString("\b \b"); /* MOVE CURSOR BACK, PRINT SPACE, MOVE BACK AGAIN */
}



/* FUNCTION TO DISPLAY THE MENU TEXT */
void UART_ShowMenu(void){
    UART_PrintString(MENU_TEXT); /* PRINT THE MENU STRING */

}




/* FUNCTION TO ALERT USER ABOUT MOVEMENT */
void UART_AlertMovement(void)
{
    if (menu_mode == 1) { /* ONLY ALERT IF IN MONITORING MODE */
        UART_PrintString("\r\n!!! ALERT: Object movement detected !!!\r\n"); /* PRINT ALERT MESSAGE */
    }
}

/*  */
/* USART2 INTERRUPT HANDLER */
void USART2_IRQHandler(void) {
    if (USART2->ISR & USART_ISR_RXNE) { /* CHECK IF DATA IS RECEIVED */
        char received = USART2->RDR; /* READ RECEIVED CHARACTER */

        if (received == BACKSPACE) { /* CHECK IF RECEIVED CHARACTER IS BACKSPACE */
            UART_HandleBackspace(); /* HANDLE BACKSPACE INPUT */
        } else {
            while (!(USART2->ISR & USART_ISR_TXE)); /* WAIT UNTIL TX BUFFER IS EMPTY */
            USART2->TDR = received; /* ECHO THE RECEIVED CHARACTER */

            UART_ProcessCommand(received); /* PROCESS THE COMMAND */
        }
    }
}

