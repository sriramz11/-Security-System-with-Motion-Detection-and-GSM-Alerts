/*
 * system_config.h
 *
 *  Created on: Dec 11, 2024
 *      Author: srira
 */
#ifndef SYSTEM_CONFIG_H_
#define SYSTEM_CONFIG_H_

#include "stm32f091xc.h"         /* INCLUDE STM32F091 MICROCONTROLLER DEFINITIONS */

#include "stm32f0xx.h"           /* INCLUDE STM32F0 SPECIFIC HEADER */
#include "uart.h"
#include "mpu6050.h"

/* DEFINITIONS */
#define GPIO_PIN_5               5          /* DEFINE PIN 5 FOR GPIO */
#define BUTTON_PIN              13         /* DEFINE PIN 13 FOR GPIO */
#define GPIO_PORTA_CLOCK         RCC_AHBENR_GPIOAEN  /* ENABLE CLOCK FOR PORT A */
#define GPIO_PORTB_CLOCK         RCC_AHBENR_GPIOBEN  /* ENABLE CLOCK FOR PORT B */
#define GPIO_PORTC_CLOCK         RCC_AHBENR_GPIOCEN  /* ENABLE CLOCK FOR PORT C */
#define SYSCFG_CLOCK             RCC_APB2ENR_SYSCFGCOMPEN /* ENABLE CLOCK FOR SYSCFG */
/* CONFIGURE SYSTEM CLOCK SETTINGS */
void SystemClock_Config(void);

/* INITIALIZE GPIO PINS */
void GPIO_Init(void);

/* INITIALIZE EXTERNAL INTERRUPTS */
void EXTI_Init(void);

/* IMPLEMENT A SIMPLE DELAY USING A VOLATILE COUNTER */
void Delay(volatile uint32_t count);

/* CHECK IF THE BUTTON IS PRESSED */
int is_button_pressed(void);

#endif /* SYSTEM_CONFIG_H_ */

