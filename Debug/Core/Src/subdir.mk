################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/main.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c 

OBJS += \
./Core/Src/main.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Core/Src/main.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/main.o: ../Core/Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/CMSIS/Include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Tests" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Wrappers/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Core/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/System/Runtime/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/System/Debugging/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Middlewares/FreeRTOS/Source/include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/main.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/stm32f4xx_hal_msp.o: ../Core/Src/stm32f4xx_hal_msp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/CMSIS/Include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Tests" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Wrappers/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Core/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/System/Runtime/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/System/Debugging/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Middlewares/FreeRTOS/Source/include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/stm32f4xx_hal_msp.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/stm32f4xx_it.o: ../Core/Src/stm32f4xx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/CMSIS/Include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Tests" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Wrappers/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Core/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/System/Runtime/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/System/Debugging/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Middlewares/FreeRTOS/Source/include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/stm32f4xx_it.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/syscalls.o: ../Core/Src/syscalls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/CMSIS/Include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Tests" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Wrappers/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Core/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/System/Runtime/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/System/Debugging/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Middlewares/FreeRTOS/Source/include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/syscalls.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/sysmem.o: ../Core/Src/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/CMSIS/Include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Tests" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Wrappers/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Core/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/System/Runtime/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/System/Debugging/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Middlewares/FreeRTOS/Source/include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/sysmem.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/system_stm32f4xx.o: ../Core/Src/system_stm32f4xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/CMSIS/Include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Tests" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Wrappers/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Core/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/System/Runtime/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/System/Debugging/Inc" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Middlewares/FreeRTOS/Source/include" -I"C:/Users/Zviad/STM32CubeIDE/workspace_1.0.1/Mikiot-fw/Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/system_stm32f4xx.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

