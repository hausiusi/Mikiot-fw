################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../System/Debugging/Src/error.c \
../System/Debugging/Src/exceptions.c \
../System/Debugging/Src/performance.c 

OBJS += \
./System/Debugging/Src/error.o \
./System/Debugging/Src/exceptions.o \
./System/Debugging/Src/performance.o 

C_DEPS += \
./System/Debugging/Src/error.d \
./System/Debugging/Src/exceptions.d \
./System/Debugging/Src/performance.d 


# Each subdirectory must supply rules for building sources it contributes
System/Debugging/Src/error.o: ../System/Debugging/Src/error.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/CMSIS/Include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Tests" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Wrappers/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Core/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/System/Runtime/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/System/Debugging/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Middlewares/FreeRTOS/Source/include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"System/Debugging/Src/error.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
System/Debugging/Src/exceptions.o: ../System/Debugging/Src/exceptions.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/CMSIS/Include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Tests" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Wrappers/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Core/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/System/Runtime/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/System/Debugging/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Middlewares/FreeRTOS/Source/include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"System/Debugging/Src/exceptions.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
System/Debugging/Src/performance.o: ../System/Debugging/Src/performance.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/CMSIS/Include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Tests" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Wrappers/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Core/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/System/Runtime/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/System/Debugging/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Middlewares/FreeRTOS/Source/include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"System/Debugging/Src/performance.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

