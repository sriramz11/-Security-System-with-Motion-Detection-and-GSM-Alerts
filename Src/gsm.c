/*
* gsm.c
*
* Created on: Dec 11, 2024
* Author: srira
*/


#include "gsm.h"
#include "uart.h"
#include <string.h>
#include <stdio.h>

#define GSM_OK_RESPONSE "OK"
#define GSM_BUFFER_SIZE 128

static char gsm_buffer[GSM_BUFFER_SIZE];

/* Initialize the GSM Module */
int GSM_Init(void) {
    UART_PrintString("Initializing GSM Module...\r\n");
    UART_SendString("AT\r\n"); 
    if (!GSM_WaitForResponse(GSM_OK_RESPONSE, 5000)) {
        UART_PrintString("GSM Module Initialization Failed\r\n");
        return 0;
    }

    UART_SendString("ATE0\r\n"); 
    if (!GSM_WaitForResponse(GSM_OK_RESPONSE, 2000)) {
        UART_PrintString("Failed to disable echo\r\n");
        return 0;
    }

    UART_SendString("AT+CMGF=1\r\n"); 
    if (!GSM_WaitForResponse(GSM_OK_RESPONSE, 2000)) {
        UART_PrintString("Failed to set SMS text mode\r\n");
        return 0;
    }

    UART_PrintString("GSM Module Initialization Successful\r\n");
    return 1;
}

/* Send SMS */
void GSM_SendSMS(const char *phone_number, const char *message) {
    char command[64];
    snprintf(command, sizeof(command), "AT+CMGS=\"%s\"\r\n", phone_number);

    UART_SendString(command);
    if (!GSM_WaitForResponse(">", 2000)) {
        UART_PrintString("Failed to enter SMS input mode\r\n");
        return;
    }

    UART_SendString(message);
    UART_SendChar(0x1A); // End SMS with Ctrl+Z

    if (GSM_WaitForResponse(GSM_OK_RESPONSE, 5000)) {
        UART_PrintString("SMS Sent Successfully\r\n");
    } else {
        UART_PrintString("Failed to send SMS\r\n");
    }
}

/* Wait for Response from GSM Module */
int GSM_WaitForResponse(const char *expected_response, int timeout_ms) {
    int index = 0;
    int timeout_counter = 0;

    memset(gsm_buffer, 0, GSM_BUFFER_SIZE);

    while (timeout_counter < timeout_ms) {
        if (UART_ReceiveChar(&gsm_buffer[index])) {
            if (strstr(gsm_buffer, expected_response)) {
                return 1;
            }
            index = (index + 1) % GSM_BUFFER_SIZE;
        }
        delay_ms(1);
        timeout_counter++;
    }

    return 0;
}

