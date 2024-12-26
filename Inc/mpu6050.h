/*
 * mpu6050.h
 *
 *  Created on: Dec 11, 2024
 *      Author: srira
 */

#ifndef MPU6050_H_
#define MPU6050_H_


#include <stdint.h>
#include <stdio.h>

#include "stm32f091xc.h"
#include "stm32f0xx.h"
// MPU6050 I2C address and registers
#define MPU6050_ADDR 0x68
#define ACCEL_XOUT_H 0x3B

void GetRawData(void); // Read and print accelerometer data

// Calibration settings
#define DEBOUNCE_DELAY 150000
#define BUTTON_PIN 13
#define SAMPLES_PER_RANGE 100
#define CALIBRATION_DELAY 5000
#define RANGE_BUFFER 500

// Function declarations
void calibrateMPU6050(void);
void detectMotion(void);
void EXTI4_15_IRQHandler(void);

int16_t set_together_int16(uint8_t msb, uint8_t lsb);

// External variable declarations
extern int16_t min_x, max_x;
extern int16_t min_y, max_y;
extern int16_t min_z, max_z;
extern volatile uint8_t is_alarm_active;
void mpu6050_init(void);


#endif /* MPU6050_H_ */
