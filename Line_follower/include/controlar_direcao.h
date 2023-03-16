#ifndef CONTROL
#define CONTROL

#include <stdbool.h>
#include <stdint.h>
#include "PID.h"
#include "PWM.h"
#include "IR_sensors.h"
#include "Encoders.h"
#include "motores.h"
#include "usart0.h"

void atualizar_rotacao_dos_motores(void);
void fora_da_pista(void);
void control_direction (void);

#endif