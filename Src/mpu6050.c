/*
 * mpu6050.c
 *
 *  Created on: Dec 11, 2024
 *      Author: srira
 */

#include "mpu6050.h"
#include "uart.h"
#include "i2c.h"
#include "pwm.h"
#include "system_config.h"

/* GLOBAL VARIABLES FOR MOTION DETECTION RANGES */
int16_t min_x = 32767, max_x = -32768;
int16_t min_y = 32767, max_y = -32768;
int16_t min_z = 32767, max_z = -32768;
volatile uint8_t is_alarm_active = 0;

/* FUNCTION TO COMBINE MSB AND LSB INTO A SIGNED 16-BIT INTEGER */
int16_t set_together_int16(uint8_t msb, uint8_t lsb) {
    return (int16_t)((msb << 8) | lsb); /* COMBINE HIGH AND LOW BYTES */
}

/* FUNCTION TO READ AND PRINT RAW ACCELEROMETER DATA */
void GetRawData(void) {
    uint8_t accel_data[6]; /* BUFFER TO HOLD ACCELEROMETER DATA */
    I2C_ReadMultiple(MPU6050_ADDR, ACCEL_XOUT_H, accel_data, 6); /* READ 6 BYTES FROM MPU6050 */

    /* COMBINE HIGH AND LOW BYTES INTO SIGNED 16-BIT VALUES */
    int16_t x = set_together_int16(accel_data[0], accel_data[1]);
    int16_t y = set_together_int16(accel_data[2], accel_data[3]);
    int16_t z = set_together_int16(accel_data[4], accel_data[5]);

    /* PRINT ACCELEROMETER VALUES */
    char buffer[64];
    sprintf(buffer, "X: %d, Y: %d, Z: %d\r\n", x, y, z);
    UART_PrintString(buffer);
}

/* FUNCTION TO CALIBRATE MPU6050 SENSOR */
void calibrateMPU6050(void) {
    UART_PrintString("\r\nStarting new calibration...\r\n");
    UART_PrintString("Keep sensor still for initial calibration\r\n");
    GPIOA->ODR &= ~(1 << 5); /* TURN OFF INDICATOR LED */

    /* RESET CALIBRATION RANGES */
    min_x = 32767; max_x = -32768;
    min_y = 32767; max_y = -32768;
    min_z = 32767; max_z = -32768;

    /* COLLECT SAMPLES TO DETERMINE CALIBRATION RANGES */
    for (int i = 0; i < SAMPLES_PER_RANGE; i++) {
        uint8_t accel_data[6];
        I2C_ReadMultiple(MPU6050_ADDR, ACCEL_XOUT_H, accel_data, 6);

        int16_t x = set_together_int16(accel_data[0], accel_data[1]);
        int16_t y = set_together_int16(accel_data[2], accel_data[3]);
        int16_t z = set_together_int16(accel_data[4], accel_data[5]);

        /* UPDATE MINIMUM AND MAXIMUM VALUES */
        min_x = (x < min_x) ? x : min_x;
        max_x = (x > max_x) ? x : max_x;
        min_y = (y < min_y) ? y : min_y;
        max_y = (y > max_y) ? y : max_y;
        min_z = (z < min_z) ? z : min_z;
        max_z = (z > max_z) ? z : max_z;

        GPIOA->ODR |= (1 << 5); /* BLINK INDICATOR LED */
        GPIOA->ODR &= ~(1 << 5);
    }

    /* APPLY BUFFER TO RANGES TO REDUCE FALSE ALARMS */
    min_x -= RANGE_BUFFER;
    max_x += RANGE_BUFFER;
    min_y -= RANGE_BUFFER;
    max_y += RANGE_BUFFER;
    min_z -= RANGE_BUFFER;
    max_z += RANGE_BUFFER;

    /* PRINT FINAL CALIBRATION VALUES */
    char buffer[128];
    sprintf(buffer, "Calibration complete!\r\nRanges: X:%d to %d, Y:%d to %d, Z:%d to %d\r\n",
            min_x, max_x, min_y, max_y, min_z, max_z);
    UART_PrintString(buffer);

    is_alarm_active = 0; /* RESET ALARM STATUS */
    PlayAlertTone();
}

/* FUNCTION TO DETECT MOTION */
void detectMotion(void) {
    uint8_t accel_data[6];
    I2C_ReadMultiple(MPU6050_ADDR, ACCEL_XOUT_H, accel_data, 6);

    int16_t x = set_together_int16(accel_data[0], accel_data[1]);
    int16_t y = set_together_int16(accel_data[2], accel_data[3]);
    int16_t z = set_together_int16(accel_data[4], accel_data[5]);

    /* CHECK IF ACCELEROMETER VALUES ARE OUTSIDE CALIBRATED RANGE */
    if (!is_alarm_active && (x < min_x || x > max_x || y < min_y || y > max_y || z < min_z || z > max_z)) {
        UART_PrintString("\r\n!!! Motion detected outside calibrated range !!!\r\n");

        char buffer[64];
        sprintf(buffer, "Values at detection: X=%d Y=%d Z=%d\r\n", x, y, z);
        UART_PrintString(buffer);

        is_alarm_active = 1; /* ACTIVATE ALARM */
        TIM2->CCR1 = TIM2->ARR / 2; /* GENERATE ALERT */
    }

    /* CHECK IF BUTTON IS PRESSED TO RESET CALIBRATION */
    if (is_alarm_active) {
        if (((GPIOC->IDR & (1 << BUTTON_PIN)) >> BUTTON_PIN) != 1) {
            UART_PrintString("Button pressed --security started - Starting recalibration \r\n");
            TIM2->CCR1 = 0; /* TURN OFF ALERT */
            is_alarm_active = 0;
            Delay(CALIBRATION_DELAY);
            calibrateMPU6050();
            UART_ShowMenu();
        }
    }
}

/* FUNCTION TO INITIALIZE MPU6050 */
void mpu6050_init(void) {
    UART_PrintString("Checking MPU6050 connection...\r\n");
    uint8_t WHO_AM_I_REG = 0;
    I2C_ReadMultiple(MPU6050_ADDR, 0x75, &WHO_AM_I_REG, 1);

    if (WHO_AM_I_REG == 0x68) {
        UART_PrintString("MPU6050 connected successfully. WHO_AM_I = ");
        UART_PrintHex(WHO_AM_I_REG);
        UART_PrintString("\r\n");

        I2C_Write(MPU6050_ADDR, 0x6B, 0x00); /* WAKE UP MPU6050 */
        I2C_Write(MPU6050_ADDR, 0x1C, 0x08); /* SET ACCELEROMETER RANGE TO ±4G */
    } else {
        UART_PrintString("MPU6050 connection failed. register WHO_AM_I = ");
        UART_PrintHex(WHO_AM_I_REG);
        UART_PrintString("\r\n");
    }
}

/* INTERRUPT HANDLER FOR BUTTON PRESS */
void EXTI4_15_IRQHandler(void) {
    static uint32_t last_interrupt_time = 0;
    uint32_t current_time = 0;

    if (EXTI->PR & (1 << BUTTON_PIN)) {
        current_time++;

        /* DEBOUNCE CHECK */
        if (current_time - last_interrupt_time > DEBOUNCE_DELAY) {
            if (is_alarm_active) {
                UART_PrintString("Button pressed - Starting recalibration after delay...\r\n");
                PWM_SetTone(0);
                is_alarm_active = 0;

                Delay(CALIBRATION_DELAY);
                calibrateMPU6050();
            }
            last_interrupt_time = current_time;
        }

        EXTI->PR |= (1 << BUTTON_PIN); /* CLEAR INTERRUPT FLAG */
    }
}
