#ifndef IR_SENSORS
#define IR_SENSORS

#include <stdint.h>
#include <avr/io.h>
//#include "ADC_state_machine.h"

void atualizar_leitura(void);
int16_t obter_posicao_media(void);
#endif