#ifndef ERROR_CODES_H_
#define ERROR_CODES_H_

typedef enum Std_Err_Tag{
    STD_OK                  = 0,
    STD_ERROR               = 1,
    STD_ALLOC_ERROR         = 2,
    STD_REFERENCE_ERROR     = 3,
    STD_PARAMETER_ERROR     = 4,
    STD_BUSY_ERROR          = 5,
    STD_TIMEOUT_ERROR       = 6,
    STD_INTERRUPTED_ERROR   = 7,
    STD_IO_ERROR            = 8
} Std_Err;

#endif /* ERROR_CODES_H_ */
