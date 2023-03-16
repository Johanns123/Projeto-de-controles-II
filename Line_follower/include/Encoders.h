#ifndef ENCODER
#define ENCODER

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>


typedef struct {
    int16_t angular;
    int16_t linear;
} velocidades_t;

uint16_t obter_pulsos_encoder_esquerdo(void);
uint16_t obter_pulsos_encoder_direito(void);
velocidades_t _calcular_velocidades_do_robo(void);

#endif