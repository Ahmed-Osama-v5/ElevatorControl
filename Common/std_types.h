#ifndef STD_TYPES_H
#define STD_TYPES_H

#define NULL ((void*)0)

/* Boolean Data Type */
typedef unsigned char boolean;

typedef unsigned char uint8_t;

typedef unsigned int uint16_t;

typedef uint8_t Std_ReturnType_t;


#define TRUE  ((boolean) 1U)
#define FALSE ((boolean) 0U)

#define E_OK        ((Std_ReturnType_t) 0U)
#define E_NOT_OK    ((Std_ReturnType_t) 1U)

#endif /* STD_TYPES_H */