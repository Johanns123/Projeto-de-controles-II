#ifndef MOTORES_H
#define MOTORES_H
#include <avr/io.h>


#define MOTOR_DIR_CHN    (1<<PB4)
#define MOTOR_ESQ_CHN    (1<<PB5)
#define MOTORS_REG      DDRB
#define MOTORS_PORT     PORTB
#define PWMA             10
#define PWMB             11

//===================================
//Prototipo das funcoes
void motor_init(void);
void motor_frente(void);
void motor_giro_direita(void);
void motor_giro_esquerda(void);
void motor_tras(void);
void motor_off(void);
void motor_alterar_velocidade_dir(unsigned int velocidade_dir);
void motor_alterar_velocidade_esq(unsigned int velocidade_esq);

#endif