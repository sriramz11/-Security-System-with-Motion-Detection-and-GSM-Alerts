/*
 * system_config.c
 *
 *  Created on: Dec 11, 2024
 *      Author: srira
 */

#include "system_config.h"       /* INCLUDE HEADER FOR SYSTEM CONFIGURATION */


/* FUNCTION TO CONFIGURE SYSTEM CLOCK */
void SystemClock_Config(void){

    RCC->CR |= RCC_CR_HSION;                  /* ENABLE HSI OSCILLATOR */
    while (!(RCC->CR & RCC_CR_HSIRDY));       /* WAIT UNTIL HSI IS READY */

    RCC->CFGR &= ~RCC_CFGR_PLLMUL;            /* CLEAR PLL MULTIPLIER */
    RCC->CFGR |= RCC_CFGR_PLLMUL12;           /* SET PLL MULTIPLIER TO 12 */
    RCC->CFGR &= ~RCC_CFGR_PLLSRC;            /* SELECT HSI/2 AS PLL SOURCE */

    RCC->CR |= RCC_CR_PLLON;                  /* ENABLE PLL */
    while (!(RCC->CR & RCC_CR_PLLRDY));       /* WAIT UNTIL PLL IS READY */

    RCC->CFGR &= ~RCC_CFGR_SW;                /* CLEAR SYSTEM CLOCK SWITCH */
    RCC->CFGR |= RCC_CFGR_SW_PLL;             /* SELECT PLL AS SYSTEM CLOCK */
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL); /* WAIT UNTIL SWITCH IS COMPLETE */

    SystemCoreClockUpdate();                  /* UPDATE SYSTEM CORE CLOCK */
}

/* FUNCTION TO INITIALIZE GPIO PINS */
void GPIO_Init(void)
{

    RCC->AHBENR |= GPIO_PORTA_CLOCK | GPIO_PORTB_CLOCK | GPIO_PORTC_CLOCK; /* ENABLE CLOCKS FOR PORTS A, B, AND C */

    GPIOA->MODER &= ~(3U << (GPIO_PIN_5 * 2)); /* CLEAR MODE FOR PA5 */
    GPIOA->MODER |= (1U << (GPIO_PIN_5 * 2));  /* SET PA5 AS OUTPUT */

    GPIOC->MODER &= ~(3U << (BUTTON_PIN * 2)); /* CLEAR MODE FOR PC13 */
    GPIOC->PUPDR &= ~(3U << (BUTTON_PIN * 2)); /* CLEAR PULL-UP/DOWN FOR PC13 */
    GPIOC->PUPDR |= (1U << (BUTTON_PIN * 2));  /* SET PC13 AS PULL-UP */
}

/* FUNCTION TO INITIALIZE EXTERNAL INTERRUPTS */
void EXTI_Init(void)
{
    RCC->APB2ENR |= SYSCFG_CLOCK;             /* ENABLE CLOCK FOR SYSCFG */

    SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13; /* CLEAR EXTI CONFIGURATION FOR PC13 */
    SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC; /* CONFIGURE EXTI FOR PC13 */

    EXTI->IMR |= (1 << BUTTON_PIN);          /* ENABLE INTERRUPT MASK FOR LINE 13 */
    EXTI->FTSR |= (1 << BUTTON_PIN);         /* ENABLE FALLING EDGE TRIGGER FOR LINE 13 */

    NVIC_EnableIRQ(EXTI4_15_IRQn);            /* ENABLE EXTI4_15 INTERRUPT IN NVIC */
    NVIC_SetPriority(EXTI4_15_IRQn, 0);       /* SET HIGHEST PRIORITY FOR EXTI4_15 */
}

/* FUNCTION FOR SIMPLE DELAY */
void Delay(volatile uint32_t count){
    while (count--) {                         /* DECREMENT COUNTER UNTIL ZERO */
        __NOP();                              /* NO OPERATION FOR TIMING */
    }
}

/* FUNCTION TO CHECK IF BUTTON IS PRESSED */
int is_button_pressed(void){
    return !(GPIOC->IDR & (1 << BUTTON_PIN)); /* RETURN TRUE IF BUTTON IS PRESSED */
}
