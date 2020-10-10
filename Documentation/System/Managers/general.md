Managers
========


Contains
---------
* [mgr_adc.c/h]( mgr_adc)
* [mgr_button.c/h](mgr_button)
* [mgr_rtc.c/h](mgr_rtc)
* [mgr_tasks.c/h]( mgr_tasks)


### <a name="mgr_adc"></a> mgr_adc.c/h
Library contains helper functions and structures for easy ADC(Analog Digital Converter) management.
* adc_uiconf_t - Structure stores ADC initialization state, measured values and 
 working parameters.
* mgr_adc_init - Initializes ADC and sets initialised bit in acd_uiconfig structure, 
 to prevent reinitialization again.
* mgr_adc_deinit - deinitializes ACD and sets initialized bit in acd_uiconfig back to false.
* mgr_adc_print_voltage - Prints measured voltage on serial output.
* mgr_adc_print_value - Prints measured ADC value on serial output.
* mgr_adc_get_uiconf - Returns pointer of the adc_uiconf_t type structure. 
* adc_ht_detected - Function is a "weak" here to force (or inform in this case) the user to have their implementation in UI side. 
  Real implementation of these functions we have in `System/Runtime/t_adc.c` file.
* adc_tc_detected - Function is a "weak" here to force (or inform in this case) the user to have their implementation in UI side. 
  Real implementation of these functions we have in `System/Runtime/t_adc.c` file.

### <a name="mgr_button"></a> mgr_button.c/h
* btn_enum_t - Structure describes type of the button, normal open or normal close.
* _btn_state - Structure intended for storing button on or off state.
* btn_t - Structure defines various button settings for GPIO(General-Purpose Input/Output), NVIC(Nested Vector Interrupt Controller), 
 debuncing, type and state.
* `_buttons` - Array contains `btn_t` type objects, every button should be defined here. 
 Consider to not collide the same pins on different ports, or upgrade the function find_button.
* mgr_button_init - Initialize all buttons defined in `_buttons` array, argument _tim_period_us is timer interrupt time in milliseconds
* _button_pressed - Temporary function to test the result of button press/release detection.
* key_press_handle - Function executed by pin change interrupt function. Interrupt function is located in the `/System/Runtime/t_iomgr.c` file.
 Function reads the button state and changes button state status in _buttonns array.
* btn_t* _find_button(uint16_t gpio_pin) - finds changed pin with scanning whole `_buttons` array.
* mgr_button_detect_state(uint16_t gpio_pin - Detect state of the function.
* _tim_init - Static function, Initializes timer11
* _timebase_init - Static function enables timer11 RCC (Reset and Clock Control) clock, sets priority and anables NVIC interrupt.
* `mw_tim11_reset_clk` - Static function enables timer11 clock.
* TIM1_TRG_COM_TIM11_IRQHandler - Function is Timer IRQ (Interrupt Requests) handler.

### <a name="mgr_rtc"></a>mgr_rtc.c/h
* days_in_a_month - Array that contains maximum possible number of days in month. Array is used to check the maximum axeptance number of the month.
* ASSERT_DATE_ELEMENT - Macro definition checks if month, day, year, hour, minutes, second are in accepted range.
* mgr_rtc_init - Function initializes RTC(Real Time Clock), hour format is passed as argument. Actually the function is a wrapper mw_rtc_init function from `/Wrappers/mw_rtc` library.
* mgr_rtc_set - Function sets RTC with time and date parameters.
 Actually the function is a wrapper of the mw_rtc_set function from the `/Wrappers/mw_rtc` library.
* `mgr_rtc_get_time` - Function is a wrapper of the mw_rtc_get_time from `/Wrappers/mw_rtc` library and returns current time from RTC.
* mgr_rtc_get_date - Function is a wrapper of the mw_rtc_get_date from `/Wrappers/mw_rtc` library and returns current time from RTC.
* mgr_rtc_set_strf - Function sets date and time from string parameter. Format of the string is: day-month-year-hour-minute. Year is set from values 0 to 99 and an hour with a 24 hour format.
* mgr_rtc_print_date - Function prints date and time on debug output
* _set_week_day - Function calculates and sets weekday in the rtc_date_t object.

### <a name="mgr_tasks"></a>mgr_tasks.c/h
* tasks_statuses_t - Structure for storing current task count, task previous and current statuses
* mgr_tasks_get_system_state - Function allocates memory for storing statuses of tasks. First, make a copy of task statuses if it already exists, if not - skip. 
  Allocate memory only if it is needed. If the above operations are successful, save this previously measured task status states in the buffer to have a reference for the next check. 
  Update the last task allocation length which after this line is the current tasks allocation length. Allocate memory only if it is needed. 
  Buffer allocation length is the same as for tasks during the previous check 
* mgr_tasks_get_last_measured - Returns FreeRTOS systems tasks state 


General Description
-------------------

### ADC
`mgr_adc` library contains ADC start, stop, and functions for returning and printing measured data. Every general configuration is defined in ioconfig.h file, 
so ADC port and pin are defined also in ioconfig.h . 
ADC works with continuous mode, using the DMA(Direct Memory Access) module. For more detail overview please look general description in Documentation/Wrappers/general.md

### Button
For button debouncing timer11 is used. Temporary all timer settings are locally in mgr_button.c file. In the future, this timer will be generalized.

### RTC
In most cases, the library is a wrapper of the mw_rtc library from the Wrappers module. Additionally, it validates for setting date and time parameters in the function 
mgr_rtc_set_strf and prints date/time in a nice human-readable way.

### Tasks
It's a core library for managing tasks, Allocates memory, and stores FreeRTOS tasks. Retrieving tasks statuses. Manages previous and current tasks statuses.
