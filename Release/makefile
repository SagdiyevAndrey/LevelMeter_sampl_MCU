################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32U5xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include ADF4351/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := LevelMeter_sampl_MCU
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
LevelMeter_sampl_MCU.elf \

MAP_FILES += \
LevelMeter_sampl_MCU.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
LevelMeter_sampl_MCU.list \

OBJCOPY_BIN += \
LevelMeter_sampl_MCU.bin \


# All Target
all: main-build

# Main-build Target
main-build: LevelMeter_sampl_MCU.elf secondary-outputs

# Tool invocations
LevelMeter_sampl_MCU.elf LevelMeter_sampl_MCU.map: $(OBJS) $(USER_OBJS) D:\Programming\STM32CubeIDE\LevelMeter_sampl_MCU\STM32U575VITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "LevelMeter_sampl_MCU.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m33 -T"D:\Programming\STM32CubeIDE\LevelMeter_sampl_MCU\STM32U575VITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="LevelMeter_sampl_MCU.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

LevelMeter_sampl_MCU.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "LevelMeter_sampl_MCU.list"
	@echo 'Finished building: $@'
	@echo ' '

LevelMeter_sampl_MCU.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "LevelMeter_sampl_MCU.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) LevelMeter_sampl_MCU.bin LevelMeter_sampl_MCU.elf LevelMeter_sampl_MCU.list LevelMeter_sampl_MCU.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
