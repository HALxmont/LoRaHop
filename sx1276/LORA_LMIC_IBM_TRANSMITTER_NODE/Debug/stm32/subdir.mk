################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../stm32/adc.c \
../stm32/blipper.c \
../stm32/crc.c \
../stm32/debug.c \
../stm32/eeprom.c \
../stm32/hal.c \
../stm32/hw.c \
../stm32/leds.c \
../stm32/spi.c 

OBJS += \
./stm32/adc.o \
./stm32/blipper.o \
./stm32/crc.o \
./stm32/debug.o \
./stm32/eeprom.o \
./stm32/hal.o \
./stm32/hw.o \
./stm32/leds.o \
./stm32/spi.o 

C_DEPS += \
./stm32/adc.d \
./stm32/blipper.d \
./stm32/crc.d \
./stm32/debug.d \
./stm32/eeprom.d \
./stm32/hal.d \
./stm32/hw.d \
./stm32/leds.d \
./stm32/spi.d 


# Each subdirectory must supply rules for building sources it contributes
stm32/%.o: ../stm32/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F103xB -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/Inc" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/Drivers/CMSIS/Include" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/lmic" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/stm32" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/aes/ideetron" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/aes"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


