#ifndef STD_TYPES_H
#define STD_TYPES_H

#include <stdint.h>

#define NULL ((void*)0)

/* Boolean Data Type */
typedef unsigned char boolean;

#define True  ((boolean)1)
#define False ((boolean)0)

typedef uint8_t Std_ReturnType_t;

#define E_OK        ((Std_ReturnType_t) 0U)
#define E_NOT_OK    ((Std_ReturnType_t) 1U)

#endif /* STD_TYPES_H */