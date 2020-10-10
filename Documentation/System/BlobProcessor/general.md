BlobProcessor
=============


Contains
---------
* [bp_helper.h](#bp_helper)
* [bp_groups.c/.h](#bp_groups)
* [bp_player.c/.h](#bp_player)
* [bpg_actions.c](#bpg_actions)
* [bpg_inouts.c](#bpg_inouts)
* [bpg_params.c](#bpg_params)
* [bpg_inits.c](#bpg_inits)
* [bpg_operators.c](#bpg_operators)


### <a name="bp_helper"></a>bp_helper.h
The file contains all structures which are required for BlobProcessor to work.
* _blob_data - Data type of the blob
* _blob - Blob structure, contains blob data, length and counter
* bfp_array_t - Array of the blob function pointers
* bp_groups_enum_t - Blob player group id Enumerator.

### <a name="bp_groups"></a>bp_groups.c/.h
The file contains function bp_groups_get_actual_function. It calls initialization methods and 
executes function with group and function ID 

### <a name="bp_player"></a>bp_player.c/.h
bp_player.c file contains two internal and one externally visible function. 
* _collect_and_log_failure_details function as the name suggests, designed for logging.
* bp_player_play function takes pointer of the blob byte array and executes in while loop, 
  each iteration it creates blob object retrieving function pointer with blob group id and 
  function id.

### <a name="bpg_actions"></a>bpg_actions.c
bpg_actions.c file contains all functions with action category. 

* bpt_actions_init - function passes blob function pointer and length of the byte array.
* _delay - delay action function. Loops into the while loop with vTaskDelay, untils 
  certain time limit is reached. 
* _jump - jump action function, jumps to the specific address of the blob execution.
* _print - print action function, prints a blob on serial interface with debug_p function. 

### <a name="bpg_inouts"></a>bpg_inouts.c 
Unimplemented

### <a name="bpg_params"></a>bpg_params.c 
Unimplemented 

### <a name="bpg_inits"></a>bpg_inits.c
Unimplemented

### <a name="bpg_operators"></a>bpg_operators.c
bpg_operators.c module contains functions for conditionals.
* bpt_conditions_init - asignes function and argument length to the bp_operator structure
* _if - function implements basic if conditional operator.
* _snilfe - Skips next if last function return value equals


General Description
-------------------
The document describes the general idea of the blob and working principles of the BlobProcessor module. 

BlobProcessor (which is placed in the system layer) is a core module behind a blob functionality. The module unites multiple libraries, 
most important from this is bp_helper.h, it contains all structures and enumerations for BlobProcesor to work. For easy explanation, let's use an example 
from the user perspective. A user writes a program on his computer, the program then will be compiled to a binary file. The blob program is limited with 
predefined functions and operators, such as delay, jump, print, and conditional operators like if, snilfe (Skip next if last function equals). 
All user programs should use only functions  and conditional operators which are already implemented in firmware or combine it to create more sophisticated
applications. 
The compilation process is straightforward, everything is compiled in one flow, with replacing function and conditional operators names to their id's. 
The compiled code is sent with serial UART interface to MCU which has mikiot-fw firmware in it. All command decoding functionality is implemented in
UI/Src/cmdline.c file. After receiving and decoding the blob command, data is passed to _play_blob function which is located also in cmdline.c file. 

A binary blob is just a byte array, which will contain some program logic or an entire program. Blob implementation in the mikiot-fw allows us 
to add or modify some logic functionality in runtime, without interpreting code, which obviously is a slow and resource-heavy process for MCU.

Blob array consists of the following parts: length, counter, data, crc.

- **length:** is a 16-bit number showing whole blob length in bytes
- **counter:** as `length` it is a 16-bit number as well and showing on which address of the blob array currently the blob player is processing. 
  Its idea is very similar to PC (Program counter) in the processor.
- **data:** is the functional part of blob where the real "program" goes. It contains the following parts: group_id, member_id, args_length, and args.
  - **group_id:** selects the group of function arrays
  - **member_id:** selects the member (function) from the selected group
  - **args_length:** shows the lengths of arguments for the function
  - **args:** arguments for the selected function
- **crc:** cyclic redundancy check to be sure that blob contains valid data.

During runtime, probably the easiest way to see how the blob processor works is to run the `blob` command from CLI. The command must be followed by 
space and then hex bytes, separated by a space character.
example: "blob 15 00 00 00 00 00 04 00 AA 10 00 00 00 00 04 00 AA 02 00 00 11". 
To process this command in CLI the `_Play_blob` function takes care. It ensures that received data is not damaged or unsupported while converting the 
string hex data into a byte array. Finally function calls  bp_player_play (function is located in System/BlobProcessor/Src/bp_player.c) function and passes
created byte array. 
After verification bp_player_play function, goes to the cyclic execution of the blob flow, building blob structure array and retrieving 
appropriate function with bp_groups_get_actual_function (function is located in System/BlobProcessor/Src/bp_groups.c) with it's group and member id.
