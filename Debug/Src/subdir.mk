################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/12c.c \
../Src/main.c \
../Src/mpu6050.c \
../Src/pwm.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_config.c \
../Src/uart.c 

OBJS += \
./Src/12c.o \
./Src/main.o \
./Src/mpu6050.o \
./Src/pwm.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_config.o \
./Src/uart.o 

C_DEPS += \
./Src/12c.d \
./Src/main.d \
./Src/mpu6050.d \
./Src/pwm.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_config.d \
./Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F0 -DNUCLEO_F091RC -DSTM32F091RCTx -DSTM32F091xC -c -I../Inc -I../CMSIS -O0 -ffunction-sections -fdata-sections -Wall -Werror -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/12c.cyclo ./Src/12c.d ./Src/12c.o ./Src/12c.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/mpu6050.cyclo ./Src/mpu6050.d ./Src/mpu6050.o ./Src/mpu6050.su ./Src/pwm.cyclo ./Src/pwm.d ./Src/pwm.o ./Src/pwm.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/system_config.cyclo ./Src/system_config.d ./Src/system_config.o ./Src/system_config.su ./Src/uart.cyclo ./Src/uart.d ./Src/uart.o ./Src/uart.su

.PHONY: clean-Src

