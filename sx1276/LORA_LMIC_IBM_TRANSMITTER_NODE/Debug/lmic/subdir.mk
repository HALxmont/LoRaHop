################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lmic/lmic.c \
../lmic/oslmic.c \
../lmic/radio.c 

OBJS += \
./lmic/lmic.o \
./lmic/oslmic.o \
./lmic/radio.o 

C_DEPS += \
./lmic/lmic.d \
./lmic/oslmic.d \
./lmic/radio.d 


# Each subdirectory must supply rules for building sources it contributes
lmic/%.o: ../lmic/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F103xB -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/Inc" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/Drivers/CMSIS/Include" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/lmic" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/stm32" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/aes/ideetron" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/aes"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


