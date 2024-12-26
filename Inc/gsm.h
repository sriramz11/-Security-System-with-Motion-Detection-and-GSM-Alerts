/*
 * gsm.h
 *
 *  Created on: Dec 11, 2024
 *      Author: srira
 */
#ifndef GSM_H
#define GSM_H

#include <stdint.h>

/* Initialize GSM Module */
int GSM_Init(void);

/* Send SMS to a specific phone number */
void GSM_SendSMS(const char *phone_number, const char *message);

/* Wait for a specific response from GSM Module */
int GSM_WaitForResponse(const char *expected_response, int timeout_ms);

#endif // GSM_H

