################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/option/syscall.c 

OBJS += \
./Src/option/syscall.o 

C_DEPS += \
./Src/option/syscall.d 


# Each subdirectory must supply rules for building sources it contributes
Src/option/syscall.o: ../Src/option/syscall.c Src/option/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/option/syscall.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

