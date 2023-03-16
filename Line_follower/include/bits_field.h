#ifndef BITS_FIELD
#define BITS_FIELD

#include <stdio.h>
#include <stdint.h>

typedef struct
{
  uint16_t    PWM_front_motors     : 10;  
  int16_t    PWM_right            : 12;
  int16_t    PWM_left             : 12;
} bits_field;

bits_field variable;

#endif