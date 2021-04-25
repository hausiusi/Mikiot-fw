# Changelog
All notable changes to this project will be documented in this file.
The newest changes are on the top chronologically, the oldest - on the bottom.

## [Unreleased]
[Learning-To-Fly-IOT]

---

## [0.4.0] - 2021-04-26
[Chick-Jumps-Off-Cliff-IOT]

#### Added
- Blob processor updates: conditional function, jump function, improved group member 
  types and many more
- Tests: blob processor with incorrect blobs, ring-buffer library
- Debug can now print a floating point numbers
- ADC layer, module, manager and voltage measurement
- Possibility to easily configure the UART module
- GPRS: driver, command-line options
- Display thread CPU times (thread load) in task-manager in float
#### Changed
- The name cmdline is changed into cli everywhere
- Handling the button is processed not in interrupt but in the thread now
#### Fixed
- Blob processor memory vulnerabilities
- Compiler warning "array subscript has type 'char'"
- TIM11 was not making interrupts but callback was called from TIM9 and it was creating 
  an impression that the TIM11 interrupt was done
- Bug in ring-buffer when overlap is not supported

---

## [0.2.1] - 2020-07-06
[Feathered-IOT]

#### Fixed
- NormalOpen button didn't work on the first try

---

## [0.2.0] - 2020-06-24
[Feathered-IOT]

#### Added
- Basic IO layer with button management and a debouncer
- Manageable CPU load measurer
- Timebase layer
- System performance monitor
- Function execution time measurer
- Overal state monitor
- Debug levels
- Error table (documentation)
- Hardware usage description (documentation)
#### Changed
- Datetime set-get improvements based on RTC API
- `mw_hal_init` changed to `mw_init`
- `debug_p` changed to `debug_error` in error reporter
#### Fixed
- Fix for sprintf crashes system

---

## [0.1.0] - 2020-04-14
[Chick-IOT]

##### Added
- Initial documentation
- Basic RTC API.
- Basic blob player.
- Busy buffer library.
- Ringbuffer library.
- Basic command line with help (currently works using UART).
- Error reporter layer.
- Basic debug layer.
- UART Rx/Tx interface with DMA.
- FreeRTOS.
- Initial project for STM32F411 MCU.

---