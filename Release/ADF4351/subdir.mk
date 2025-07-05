################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/Programming/STM32CubeIDE/ADF4351/ADF4351.c 

OBJS += \
./ADF4351/ADF4351.o 

C_DEPS += \
./ADF4351/ADF4351.d 


# Each subdirectory must supply rules for building sources it contributes
ADF4351/ADF4351.o: D:/Programming/STM32CubeIDE/ADF4351/ADF4351.c ADF4351/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32U575xx -c -I../Core/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../Drivers/CMSIS/Include -I"D:/Programming/STM32CubeIDE/ADF4351" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-ADF4351

clean-ADF4351:
	-$(RM) ./ADF4351/ADF4351.cyclo ./ADF4351/ADF4351.d ./ADF4351/ADF4351.o ./ADF4351/ADF4351.su

.PHONY: clean-ADF4351

