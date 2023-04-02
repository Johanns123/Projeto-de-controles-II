#include "motores.h"

void motor_init(void)
{

  MOTORS_REG      |= MOTOR_DIR_CHN | MOTOR_ESQ_CHN | (1<<PB2) | (1<<PB3);

  MOTORS_PORT &= ~(MOTOR_DIR_CHN | MOTOR_ESQ_CHN);  

  TCCR1A = 0xa1;  //8 bits fast mode
  TCCR1B = 3;     //500 Hz

  TCCR2A = 0xa3;  //8 bits fast mode
  TCCR2B = 5;     //500 Hz
}


void motor_tras(void)
{
  MOTORS_PORT |= MOTOR_DIR_CHN | MOTOR_ESQ_CHN;
}

void motor_giro_direita(void)
{
  MOTORS_PORT &= ~(MOTOR_DIR_CHN);
  MOTORS_PORT |=  MOTOR_ESQ_CHN;
}

void motor_giro_esquerda(void)
{
  MOTORS_PORT |= MOTOR_DIR_CHN;
  MOTORS_PORT &= ~(MOTOR_ESQ_CHN);
}
void motor_frente(void)
{
  MOTORS_PORT &= ~(MOTOR_DIR_CHN | MOTOR_ESQ_CHN);
}
void motor_off(void)
{
  MOTORS_PORT &= ~(MOTOR_DIR_CHN | MOTOR_ESQ_CHN); 
  motor_alterar_velocidade_dir(0);
  motor_alterar_velocidade_esq(0);
}

void motor_alterar_velocidade_dir(unsigned int velocidade_dir)
{
  //analogWrite(PWMA, velocidade_dir);
  OCR1B = velocidade_dir;
}

void motor_alterar_velocidade_esq(unsigned int velocidade_esq)
{
  //analogWrite(PWMB, velocidade_esq);
  OCR2A = velocidade_esq;
}