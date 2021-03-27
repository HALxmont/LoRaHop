################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/hex.c \
../Src/main.c \
../Src/mainApp.c \
../Src/queue.c \
../Src/stm32f1xx_hal_msp.c \
../Src/stm32f1xx_it.c \
../Src/system_stm32f1xx.c 

OBJS += \
./Src/hex.o \
./Src/main.o \
./Src/mainApp.o \
./Src/queue.o \
./Src/stm32f1xx_hal_msp.o \
./Src/stm32f1xx_it.o \
./Src/system_stm32f1xx.o 

C_DEPS += \
./Src/hex.d \
./Src/main.d \
./Src/mainApp.d \
./Src/queue.d \
./Src/stm32f1xx_hal_msp.d \
./Src/stm32f1xx_it.d \
./Src/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F103xB -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/Inc" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/Drivers/CMSIS/Include" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/lmic" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/stm32" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/aes/ideetron" -I"C:/Users/Dhananjay/stm32workspace/LORA_LMIC_IBM_TRANSMITTER_NODE/aes"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


