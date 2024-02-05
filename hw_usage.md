# Hardware usage
Describes the hardware which is used what for

## Contents
- [Timers](#timers)
- [UARTS](#uarts)
- [RTC](#rtc)
- [Systick](#systick-timer)
- [ADC](#adc)

## Timers<div name="timers"/>
- [TIM4](./Debugging/Src/performance.c)
     - Unused, must be changed the purpose perhaps to PWM
- [TIM5](./Debugging/Src/performance.c)
     - **IO:** N/A
     - **Puspose:** performance measurement with us precision
     - **Speed:** counter increments 1000000 times a second
- [TIM9](./Wrappers/Src/mw_timebase.c)
     - **IO:** N/A
     - **Purpose:** counts ticks for "basetime" with ms precision (32 bit variable)
     - **Speed:** 1000 interrupts per second
- [TIM11] (./Managers/mgr_inputs.c)
     - **IO:** N/A
     - **Purpose:** counts ticks for input detection with ms precision
     - **Speed:** 1000 interrupts per second

## UARTS<div id="uarts"/>
- [UART1,2,6](./Wrappers/Src/mw_uart.c)
    - **UART1**			
        - **IO:** Tx - GPIOB6, Rx - GPIOB7 
        - **Purpose:** commandline input and output interface
        - **Speed:** variable (default: 115200)
    - **UART2**
        - **IO:** Tx - GPIOD5, Rx - GPIOD6 
        - **Purpose:** GPRS serial communication interface
        - **Speed:** variable (default: 57600)
    - **UART6**
        - **IO:** Tx - GPIOC6, Rx - GPIOC7 
        - **Purpose:** Applink interface
        - **Speed:** variable (default: 115200)        

## RTC<div id="rtc"/>
- [Integrated RTC](./Wrappers/Src/mw_rtc.c)
    - **IO:** PC14-OSC32_IN, PC15-OSC32_OUT
    - **Purpose:** real time clock for the system
    - **Subsecond precision:** 1/256

## Systick timer<div id="systick-timer"/>
- [Systick timer](./Core/Inc/FreeRTOSConfig.h)
    - **IO:** N/A
    - **Purpose:** OS system tick
    - **Speed:** 1000Hz

## ADC<div id="adc"/>
- [adc1](./Wrappers/Src/mw_adc.c)
    - **IO:** IN1 - PB0
    - **Purpose:** Measure analog signal (can be checked from CLI: adc start voltage delay 1000)
    - **Resolution:** 12b