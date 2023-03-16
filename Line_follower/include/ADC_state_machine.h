#ifndef ADC_MACHINE
#define ADC_MACHINE

#include <stdint.h>
#include <avr/interrupt.h>
#include "ADC.h"

uint8_t _valor_lido(uint8_t sensor);
void tratar_leitura_do_ADC(void);
#endif