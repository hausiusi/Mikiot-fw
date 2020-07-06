# Changelog
All notable changes to this project will be documented in this file.
The newest changes are on the top chronologically, the oldest - on the bottom.

## [Unreleased]
[Chick-Jumps-Off-Cliff-IOT]

---
## [0.2.1] - 2020-07-06
[Feathered-IOT]

#### Fixed
- NormalOpen button didn't work on the first try

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