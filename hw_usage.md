# Hardware usage
Describes the hardware which is used what for

## Contents
- [Timers](#timers)
- [UARTS](#uarts)
- [RTC](#rtc)
- [Systick](#systick-timer)

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

## UARTS<div id="uarts"/>
- [UART1](./Wrappers/Src/mw_uart1.c)
    - **IO:** Tx - GPIOA6, Rx - GPIOA7
    - **Purpose:** commandline input and output interface
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