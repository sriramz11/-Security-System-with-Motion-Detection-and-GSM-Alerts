/*
 * 12c.h
 *
 *  Created on: Dec 11, 2024
 *      Author: srira
 */

#ifndef I2C_H_
#define I2C_H_



#include <stdint.h>

void I2C_Init(void);
void I2C_Write(uint8_t addr, uint8_t reg, uint8_t data);
void I2C_ReadMultiple(uint8_t addr, uint8_t reg, uint8_t *buffer, uint8_t size);
#define GPIOB_PIN_8               8  // SCL Pin
#define GPIOB_PIN_9               9  // SDA Pin
#define GPIO_AF1                  1  // Alternate Function 1 for I2C
#define GPIO_MODE_AF              2  // Alternate Function Mode
#define GPIO_OSPEED_FAST          2  // Fast Speed
#define GPIO_OTYPE_OPEN_DRAIN     1  // Open Drain Output Type

// I2C Timing Configuration for 100 kHz
#define I2C_TIMING_100KHZ         0x10805E89

#endif /* I2C_H_ */
