/* ************************************************************************ */
/* ************************************************************************ */
/*                        (C) Copyright 2025 by                             */
/*                              HexaMix                                     */
/*                         All rights reserved                              */
/*                                                                          */
/* ************************************************************************ */
/* ************************************************************************ */

/* ************************************************************************ */
/* ************************************************************************ */
/*                        FILE INCLUSION MANAGEMENT                         */
/* ************************************************************************ */
/* ************************************************************************ */
/* Protection against bad and multiple inclusions                           */
/* ************************************************************************ */

#ifndef SegmentDriver_GCFG_H
#define SegmentDriver_GCFG_H

/* ************************************************************************ */
/* ************************************************************************ */
/* Export of Segment pins definitions                                       */
/* ************************************************************************ */
/* ************************************************************************ */

#define cu8SEGMENT_COUNT            ((uint8_t) 8U)

#define cenuSegment_LATCH_Port      ((GPIO_t) GPIOD)
#define cu8Segment_LATCH_Pin        ((uint8_t) 2U)

#define cenuSegment_A_Port          ((GPIO_t) GPIOA)
#define cu8Segment_A_Pin            ((uint8_t) 0U)
#define cenuSegment_B_Port          ((GPIO_t) GPIOA)
#define cu8Segment_B_Pin            ((uint8_t) 1U)
#define cenuSegment_C_Port          ((GPIO_t) GPIOA)
#define cu8Segment_C_Pin            ((uint8_t) 2U)
#define cenuSegment_D_Port          ((GPIO_t) GPIOA)
#define cu8Segment_D_Pin            ((uint8_t) 3U)
#define cenuSegment_E_Port          ((GPIO_t) GPIOA)
#define cu8Segment_E_Pin            ((uint8_t) 4U)
#define cenuSegment_F_Port          ((GPIO_t) GPIOA)
#define cu8Segment_F_Pin            ((uint8_t) 5U)
#define cenuSegment_G_Port          ((GPIO_t) GPIOA)
#define cu8Segment_G_Pin            ((uint8_t) 6U)
#define cenuSegment_H_Port          ((GPIO_t) GPIOA)
#define cu8Segment_H_Pin            ((uint8_t) 7U)

extern const uint8_t au8SegmentDigits[10];

/* ************************************************************************ */

#endif /* #ifndef SegmentDriver_GCFG_H */

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
