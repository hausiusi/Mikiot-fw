Debugging
=============


Contains
---------
* [debug.c/h](#debug)
* [error.c/h](#error)
* [exceptions.c/h](#exceptions)
* [overall_state.c/h](#overall_state)
* [performance.c/h](#performance)


### <a name="debug"></a>debug.c/.h
Library defines string formatting functions, structures and main debug function.
Contains macro definitions for switching and controlling between various debug levels.
* formats_t - structure for string formating functions.
* functions _lu, _l, _lu, _l, _x, _p, _s, _c, _f for string formatting modes, 
  _formats_ array links this functions to it's names "lu", "l", "u", "i", "x", "p", "s", "c", "f". 
  It's the same as the standard C string formatting options.
* dbg_buff - buffer allocated for debugging.
* debugf - Is the main function, to output debug string to users. Functions iterates through 
  string formating parameters of the debug buffer and runs appropriate formatting function.
  Iteration continues untill function finds string terminaltor '\0' in debug string.
* debug_set_level - Sets debug level with appropriate number, number is defined with names in debug.h 
  like : DBG_ANY, DBG_ERROR, DBG_WARNING, DBG_NOTE, DBG_INFO.
* debug_get_level - Gets debug level.

### <a name="error"></a>error.c/.h
Library defines error structures, types and error report functionality
* error_report - The Macro function definition. Gets file path, line and function name where 
  error happened, using __FILE__, __LINE__ and __FUNCTION__ macro. 
* error_enum_t - Structure defines error types. 
* error_t - The error structure definition.
* error_report_ function is a wrapper of the debug_error function, With itself calls to 
  the debugf function with macro definition.

### <a name="exceptions"></a>exceptions.c/.h
Unimplemented

### <a name="overall_state"></a>overall_state.c/.h
Library defines functionality for getting overall status of FreeRTOS tasks.
* overall_t - Structure of the FreeRTOS task overall status.
* overal_state_analyze - Function checks status of the overall state's base time and rts, 
  iterates over all tasks and prints task name, which has the closest possibility for overflow.

### <a name="performance"></a>performance.c/.h
Library includes FreeRTOS and mikiot-fw system monitor timers initialization, get count, 
disable timers and performance measurer functions which monitors various parameters of the system.
Implements function execution time measurement and loging functionality.

* prf_func_exect_time_get - macro definition gets execution time of the function.

* prf_func_exect_time_log - macro definition logs execution time of of the function.

* prf_timer_osmeasurer_init - Function initializes timer4 for FreeRTOS system timer.

* prf_timer_osmeasurer_count_get - Function gets timer count of the FreeRTOS timer timer4. 

* prf_start_measurement - Function initializes timer5 for mikiot-fw with _timer_measurer_init 
  and resets counter

* uint32_t prf_end_measurement - Function stops time measuring, with disabling timer5 clock.

* prf_get_info - Function takes pointer of the perfinfo_t type structure and fills it with 
  various mcu system parameters.

* _timer_measurer_init - Function inilializes time measurer of the timer5, with using 
  _timer5_32_bit_init function.

* _timer5_32_bit_init - Function initializes timer5, function takes frequency as an argument.

* _timer4_16_bit_init - Function initializes timer4, function takes frequency as an argument.

* _timer5_cnt_reset - Function resets counter of the timer5 

* _timer4_cnt_reset - Function resets counter of the timer4



General Description
-------------------
The document describes the general idea of the Debugging module and how it works.

The debugging module contains several libraries for various debug, warning, error, or just informational functionality.
debug.c/h is the core library of the debugging module, with debug.h defines, certain debug function can be activated
with a certain debug level, with itself variable `debug_level` can be changed in runtime with `debug_set_level` function. 

#### Debug levels definition in debug.h file
``` c
#define DBG_ANY     1
#define DBG_ERROR   1
#define DBG_WARNING 2
#define DBG_NOTE    3
#define DBG_INFO    4
```
debug.c file implements **debugf** function which is the core debug function of the Debugging module. 
For every debug, error, warning or informational output function uses debugf, with just adding their formatting and
logical functionality on top of it.

All string formatting functions and its mechanism is implemented in debug.c file and all these functions are united 
in formats array.

#### The formats array
```c
static formats_t formats[] = {
		{"lu", _lu},
		{"l", _l},
		{"u", _lu},
		{"i", _l},
		{"x", _x},
		{"p", _p},
		{"s", _s},
		{"c", _c},
		{"f", _f},
};
```
Array links function left to its name right. For example, the first item in the array is the function _lu (unsigned long) with the name lu.
As you guessed already, the string formatting function has the same names as in standard C printf string formatting.

Error reporting, handling, and describing functionality is implemented in error.c/h file. in error.h file.
For tracing errors in runtime, the error_report macro function definition is implemented in error.h file. the Macro gets the file path, 
line, and function name where the error happened. For better distinguishing, between different errors, 
error categorization is implemented in error_enum_t enumeration.

#### THe error_enum_t enumeration
```c
typedef enum {
	InitError = 0,
	SetupError = 1,
	WriteError = 2,
	ReadError = 3,
	VerificationError = 4,
	CmdlineError = 5,
	BlobProcessorError = 6,
	AdcError = 7,
	UartError = 8,
} error_enum_t;
```

And the `error_t` structure implements the error type. With this implementation, all errors will have its unique ID, error type
and a buffer for additional information. 
**All Error ID's are listed and described in the error_table.md file of the project**
The core function of the error library is the error_report_ function, simply it uses debug_error which itself uses debugf function with a macro definition. 

For FreeRTOS tasks overall status monitoring, the performance library is implemented in the Debugging module. 
The performance library implements the overall_t structure for storing each task's status. 

#### The `overall_t` type 
```c
typedef struct _overall {
	uint32_t rtc_ok :1;
	uint32_t base_time_ok :1;
	uint32_t base_ticks;
	uint32_t os_ticks;
	tasks_statuses_t* task_statuses;
} overall_t;
```

THe overal_state_analyze function checks the status of the overall state's base time and rts, iterates over all tasks and 
prints task name, which has the closest possibility for overflow.

For more functional debugging and monitoring, it's important to monitor any function in a runtime. So to measure each function execution timer 
one of the 32-bit timers (in our case timer5) is used. The performance.c/h library contains functions for initializing performance measure timer, 
start measuring, get timer count end measure function execution time. Additionally, there is the prf_get_info function in the performance.c file for 
getting information about the general system HAL_RCC(Hardware Abstraction Layer of Reset and Clock Control unit) clock and counter state. 

For function execution time measurement and logging functionality, prf_func_exect_time_get and prf_func_exect_time_log macro 
functions is defined in the performance.h file. As the name sugets prf_func_exect_time_get macro definition gets execution time 
of the function and prf_func_exect_time_log definition logs execution time of the function.
