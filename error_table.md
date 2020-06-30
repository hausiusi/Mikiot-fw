Error table
-----------

|  ID  |     Type           |         Description                                                                                 |
|:----:|--------------------|-----------------------------------------------------------------------------------------------------|
|  000 | InitError          | RTC initialization error                                                                            |
|  001 | SetupError         | RTC set time error                                                                                  |
|  002 | SetupError         | RTC set date error                                                                                  |
|  003 | InitError          | IO Timer11 initialization error                                                                     |
|  004 | VerificationError  | BLOB verification error                                                                             |
|  005 | CmdlineError       | Entered BLOB is longer than maximum allowed length from command line                                |
|  006 | CmdlineError       | Entered BLOB byte is not in valid hex format for command line                                       |
|  007 | BlobProcessorError | Delay requires 32 bit long argument for delay. The length or arguments must be exactly 4 bytes long |
|  008 | BlobProcessorError | Couldn't find provided group id                                                                     |
|  009 | BlobProcessorError | Couldn't find provided member id                                                                    |
|  010 | BlobProcessorError | BLOB funcion pointer couldn't find (is NULL)                                                        |
