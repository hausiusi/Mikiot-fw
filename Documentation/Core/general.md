Core
=====

Contains main.c, main.h, ioconfig.h, freeRTOS and HAL configuration files

### defines.h
Contains some general defines, that do not fall into IO configuration and Mikiot project configuration category.

### FreeRTOSConfig.h
Contains FreeRTOS configurations and defines. For detailed description please refer to [Configuration reference](https://www.freertos.org/a00110.html#configUSE_PORT_OPTIMISED_TASK_SELECTION)

### ioconfig.h
ioconfig.h there are defined physical IO pins used by different peripherals.

### mikiot_config.h
mikiot_config.h file contains the general configuration of the Mikiot project for different compilation options.

### tm32f4xx_hal_conf.h
tm32f4xx_hal_conf.h is one of the frequently used file and mainly intended for template configuration of the STM32CubeMX environment.
allowing to customize the drivers for a given application, enable or disable peripheral modules with comment/uncomment peripheral defines.

### main.c/.h
main.c contains the application entry point. Calls several initialization functions and starts OS.
In general it calls some Mikiot project initialization functions like:	mw_init *[see: Wrappers.redefines](../../Wrappers/Inc/redefines.h)*,
mw_internal_clock_init [see: Wrappers.clock](../../Wrappers/Inc/mw_clock.h), and starts Mikiot startup tasks and 
vTaskStartScheduler [see: System.Runtime.OsLoader](../../System/Runtime/Inc/os_loader.h) via os_start.

### stm32f4xx_hal_msp.c
UM1718 User manual - STM32CubeMX for STM32 configuration and initialization C code generation - Section 6.1.3 

The stm32f4xx_hal_msp.c (MSP = MCU Support package) file defines all initialization functions to configure the peripheral instances according to 
the user configuration (pin allocation, enabling of clock, use of DMA and Interrupts). 

### stm32f4xx_it.c/.h
File contains the exceptions handler and peripherals interrupt service routine for HAL STM32CubeMX environment.

### syscalls.c
The syscalls.c file contains unimplemented posix syscall functions.

### sysmem.c 
File is intended for manual memory allocations. The Mikiot project uses FreeRTOS, which handles memory automatically, 
so adding or changing functionlity to this file is not recommended.

### system_stm32f4xx.c
system_stm32f4xx.c is the one of the important file, it provides all low level initialization. Before anything is executed, 
the initialization is needed to ensure safe and proper hardware state, according to the docstring:

This file provides two functions and one global variable to be called from 
user application:
* SystemInit(): This function is called at startup just after reset and before branch to main program. 
  This call is made inside the "startup_stm32f4xx.s" file.
* SystemCoreClock variable: Contains the core clock (HCLK), it can be used by the user application to 
  setup the SysTick timer or configure other parameters.                                    
* SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must be called whenever the core 
  clock is changed during program execution.
