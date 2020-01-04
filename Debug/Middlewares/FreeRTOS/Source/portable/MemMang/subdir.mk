################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/FreeRTOS/Source/portable/MemMang/heap_4.c 

OBJS += \
./Middlewares/FreeRTOS/Source/portable/MemMang/heap_4.o 

C_DEPS += \
./Middlewares/FreeRTOS/Source/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/FreeRTOS/Source/portable/MemMang/heap_4.o: ../Middlewares/FreeRTOS/Source/portable/MemMang/heap_4.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/CMSIS/Include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Tests" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Wrappers/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Core/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/System/Runtime/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/System/Debugging/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Middlewares/FreeRTOS/Source/include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/FreeRTOS/Source/portable/MemMang/heap_4.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

