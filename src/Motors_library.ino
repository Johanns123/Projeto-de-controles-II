//===================================
//Inclusao de biblotecas
#include <util/delay.h>
#include <avr/io.h>


/*  13 - BIN1 --BIN2 eh automaticamente invertido
*   11 - PWMB
*   10 - PWMA
*   12 - AIN1 --AIN2 eh automaticamente invertido
  */

//===================================
//Mapemaneto de Hardware
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
void setup_pwm_setFreq(unsigned char option);

//===================================
//Inicio do codigo
void setup() 
{
  motor_init();
}

void loop()
{

  motor_frente();
  motor_alterar_velocidade_dir(128);
  motor_alterar_velocidade_esq(128);
  _delay_ms(500);
  motor_tras();
  _delay_ms(500);
  motor_giro_esquerda();
  _delay_ms(500);
  motor_giro_direita();
  _delay_ms(500);
  motor_off();
  _delay_ms(500);
}

void motor_init(void)
{

  MOTORS_REG      |= MOTOR_DIR_CHN | MOTOR_ESQ_CHN | (1<<PB2) | (1<<PB3);

  MOTORS_PORT &= ~(MOTOR_DIR_CHN | MOTOR_ESQ_CHN);  

  TCCR1A = 0xa1;  //8 bits fast mode
  TCCR1B = 3; //500 Hz

  TCCR2A = 0xa3;  //8 bits fast mode
  TCCR2B = 5; //500 Hz
}


void motor_frente(void)
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
void motor_tras(void)
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
