/*
 * pwm.h
 *
 *  Created on: Dec 11, 2024
 *      Author: srira
 */

#ifndef PWM_H_
#define PWM_H_


#include <stdint.h>

#define BUZZER_PIN 5

void PWM_Init(void);
void PWM_SetTone(uint32_t frequency);
void PlayAlertTone(void);



#endif /* PWM_H_ */
