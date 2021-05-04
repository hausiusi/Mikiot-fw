Error table
-----------

| ID  |Type              |Description                                                                                        |
|:---:|:-----------------|:--------------------------------------------------------------------------------------------------|
| 000 |InitError         |RTC initialization error                                                                           |
| 001 |SetupError        |RTC set time error                                                                                 |
| 002 |SetupError        |RTC set date error                                                                                 |
| 003 |InitError         |IO Timer11 initialization error                                                                    |
| 004 |VerificationError |BLOB verification error                                                                            |
| 005 |CmdlineError      |Entered BLOB is longer than maximum allowed length from command line                               |
| 006 |CmdlineError      |Entered BLOB byte is not in valid hex format for command line                                      |
| 007 |BlobProcessorError|Delay requires 32 bit long argument for delay. The length or arguments must be exactly 4 bytes long|
| 008 |BlobProcessorError|Couldn't find provided group id                                                                    |
| 009 |BlobProcessorError|Couldn't find provided member id                                                                   |
| 010 |BlobProcessorError|BLOB funcion pointer couldn't find (is NULL)                                                       |
| 011 |BlobProcessorError|Jump requires 16 bit long argument for delay. The length or arguments must be exactly 2 bytes long |
| 012 |BlobProcessorError|Print function checks if provided string has terminator, by checking if arglength - 1 index is null|
| 013 |BlobProcessorError|snilfe operator requires arguments length to be 4 bytes long (the same as function exit code       |
| 014 |BlobProcessorError|If operator requires 10 bytes long arguments                                                       |
| 015 |AdcError          |ADC initialization failed                                                                          |
| 016 |AdcError          |ADC channel configuration failed                                                                   |
| 017 |AdcError          |ADC error callback detected. DMA will be restarted                                                 |
| 018 |UartError         |Required UART device doesn't exist to enable clock                                                 |
| 019 |UartError         |Required UART device doesn't exist to configure DMA                                                |
| 020 |UartError         |Required UART device doesn't exist to get the config                                               |
| 021 |HeapError         |Couldn't allocate memory for linked list node                                                      |
| 022 |HeapError         |Couldn't allocate enough space for linked list node item                                           |
| 023 |LibError          |Linked list node argument must be not NULL                                                         |
| 024 |HeapError         |Couldn't allocate enough space for linked list iterator item                                       |
| 025 |LibError          |Iterator argument must be not NULL                                                                 |
| 026 |HeapError         |Couldn't allocate heap for mcron arguments                                                         |
