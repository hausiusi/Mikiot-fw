################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Wrappers/Src/subdir.mk
-include System/Runtime/Src/subdir.mk
-include System/Debugging/Src/subdir.mk
-include Middlewares/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Middlewares/FreeRTOS/Source/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Mikiot-fw.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Mikiot-fw.list \


# All Target
all: Mikiot-fw.elf secondary-outputs

# Tool invocations
Mikiot-fw.elf: $(OBJS) $(USER_OBJS) C:\Users\Zviad\STM32CubeIDE\workspace_1.0.1\Mikiot-fw\STM32F407VGTX_FLASH.ld
	arm-none-eabi-gcc -o "Mikiot-fw.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Zviad\STM32CubeIDE\workspace_1.0.1\Mikiot-fw\STM32F407VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Mikiot-fw.map" -Wl,--gc-sections -static  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Mikiot-fw.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Mikiot-fw.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
