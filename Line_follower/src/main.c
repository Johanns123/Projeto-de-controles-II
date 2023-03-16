//===================================
//Inclusao de biblotecas
#include <util/delay.h>
#include <avr/io.h>
//#include <Arduino.h>
#include <avr/interrupt.h>
#include "motores.h"
#include "Encoders.h"
//#include "ADC_state_machine.h"
#include "IR_sensors.h"
#include "controlar_direcao.h"
/*  13 - BIN1 --BIN2 eh automaticamente invertido
*   11 - PWMB
*   10 - PWMA
*   12 - AIN1 --AIN2 eh automaticamente invertido
*/

//===================================
//Mapemaneto de Hardware
//PD2 - INT0 - Interrupcao encoder direito
//PD3 - INT1 - Interrupcao encoder esquerdo
//===================================
//Prototipo das funcoes
void setup();
void loop();
void external_interrupt_init(void);
void setup_hardware(void);
void setup_variaveis(void);
void setup_timer0_interrupt(void);
void f_timers (void);       //funcao de temporizacao das rotinas
void f_timer1(void);
void f_timer2(void);
void f_timer3(void);
void f_timer4(void);
void sub_timer(void);
void testes_uart();
void millis(void);
void system_feedback_monitoring();
void enviar_tempo(void);
int16_t filtro_media_motores_angular(void);

//===================================
//Variaveis globais
velocidades_t velocidades;
uint16_t max_timer1, max_timer2, max_timer2_1, max_timer3, max_timer4;
bool start_timing = 0;
uint32_t millisegundos  = 0;
int8_t velocidade_filtrada_angular = 0;
char buffer[20] = {0};

ISR(TIMER0_OVF_vect)
{

  TCNT0 = 6;
  f_timers();
}


//===================================
//Inicio do codigo
int main()
{
  setup();
  while(1)  loop();
}


// ======================================================================================================
// ===================================== RTOS primitivo =================================================
// ======================================================================================================
// ============================== Parte nao visivel ao usuario ==========================================
// ======================================================================================================


void f_timers (void) /* Funcao e chamada a cada 125us */ 
{

  static uint16_t counter1 = 1, counter2 = 1, counter3 = 1, counter4 = 1;
  
  if(counter1 < max_timer1) 
      counter1++;
  else
  {
      f_timer1();
      counter1 = 1;
  }

  if(counter2 < max_timer2) 
      counter2++;
  else
  {
      f_timer2();
      counter2 = 1;
  }

  if(counter4 < max_timer4) 
      counter4++;
  else
  {
    f_timer4();
    counter4 = 1;
  }


  if(counter3 < max_timer3) 
      counter3++;
  else
  {
      f_timer3();
      counter3 = 1;
  }
}

/* tempo = 250us */
void f_timer1(void) 
{
    atualizar_leitura();
}

/* tempo = 10ms */
void f_timer2(void)
{
    //estabilizar_direcao();
    //run();
    //velocidade_filtrada_linear = filtro_media_exponencial_linear();
    //velocidade_filtrada_angular = filtro_media_exponencial_angular();
    testes_uart();
    sub_timer();
}

/* tempo = 1ms */
void f_timer3(void)
{   
    millis();   // funcao chamada a cada 1ms
    //if(uart.count() > 0)  
    //{
    //    select_menu_IHM();
    //}
}

void f_timer4(void)//a cada 2ms -> sujeito a alteracao de acordo com a inércia dos motores
{
    //sprintf(buffer, "%d\t%d\n", velocidade_filtrada_linear, velocidades.linear);
    //uart.enviar_string(buffer);
    velocidades = _calcular_velocidades_do_robo();
    velocidade_filtrada_angular = filtro_media_motores_angular();
    control_direction();
    //aciona_controlar_direcao = 1;
    
}
void sub_timer(void)
{ 
  static uint16_t counter2_1 = 1;

    if(counter2_1  < max_timer2_1)   counter2_1++;

    else    //20ms
    {
        //speedometer();
        counter2_1 = 1;
    }
}

void setup() 
{
  motor_init();
  setup_hardware();
  setup_variaveis();
  external_interrupt_init();
  setup_timer0_interrupt();
  //adc_setup();
  usart0_config(1, 57600);
  //tratar_leitura_do_ADC();
  start_timing = 1;
  sei();
}

void loop()
{
  
  //velocidades_t velocidade;
  //velocidade = _calcular_velocidades_do_robo();
  //int w = velocidade.angular;
  //motor_frente();
  //motor_alterar_velocidade_dir(128);
  //motor_alterar_velocidade_esq(128);
  //_delay_ms(500);
  //motor_tras();
  //_delay_ms(500);
  //motor_giro_esquerda();
  //_delay_ms(500);
  //motor_giro_direita();
  //_delay_ms(500);
  //motor_off();
  //_delay_ms(500);
}

void external_interrupt_init(void)
{
  EICRA |= 0x0A; //qualquer mudanca de estado
  EIMSK = 3;  //habilito as duas interrupcoes
}

void setup_hardware(void)
{
  MCUCR &= 0xef;      //habilita pull up quando configurado e desabilita algumas configuracoes previas do MCU

  DDRD &= ~((1<<PD2) | (1<<PD3)); //definicao dos pinos do encoder
  PORTD = 0x00;

  DDRC &= ~((1<<PC2) | (1<<PC1)); //pinos do IR sensor
  PORTC = 0x00;  
}

void setup_timer0_interrupt(void)
{ 
  TCCR0B = 0x02; //TC0 com prescaler de 8
  TCNT0  = 6;          //125us
  TIMSK0 = 0x01; //habilita a interrupcao do TC0
  /*tempo =65536 * Prescaler/Fosc = 65536 * 1024/16000000 = 4, 19s
    tempo = X_bit_timer * Prescaler/Fosc
    Valor inicial de contagem = 256 - tempo_desejado*Fosc/Prescaler = 256 - 0,001*16000000/1024 = 255
    Valor inicial de contagem = X_bit_timer - tempo_desejado*Fosc/Prescaler */
}

void setup_variaveis(void)
{
    max_timer1    = 2,  //250us
    max_timer2    = 80, //10ms
    max_timer2_1 = 2, //20ms
    max_timer3    = 8,  //1ms
    max_timer4    = 16,    //2ms
    
    setup_pwm_e_duty_cycle();
    //estado_da_operacao.modo = MAPEAMENTO;
    variable.PWM_front_motors = 80;//get_pwm(20);//get_pwm(35);
}
int16_t filtro_media_motores_angular(void)
{
    #define NUMERO_DE_AMOSTRAS 10
    static int16_t speed[NUMERO_DE_AMOSTRAS] = {0};
    int16_t acumulador = 0;

    for (int i = NUMERO_DE_AMOSTRAS - 1; i > 0; i--)
        speed[i] = speed[i - 1];

    speed[0] = velocidades.angular;

    for (int i = 0; i < NUMERO_DE_AMOSTRAS; i++)
    {
        acumulador += speed[i];
    }

    return acumulador / NUMERO_DE_AMOSTRAS;
}

void millis(void)
{
    if(start_timing)
        millisegundos++;
}

void testes_uart()
{       

    //sprintf(buffer, "%d\n", millisegundos);
    //uart.enviar_string(buffer);
    //robot_speed();
    system_feedback_monitoring();
}

void robot_speed()
{
    extern int16_t  V_esq, V_dir;
    _enviar_decimal(((V_dir + V_esq) / 2));
    usart0_send_char('\n');
}

extern uint16_t delta_EncR;
extern uint16_t pulso_atualR;

void system_feedback_monitoring()
{        
    //extern int16_t PV_angular, setpoint;
    //extern int16_t  PV_linear;
    //extern int16_t  V_esq, V_dir;
    //extern int16_t delta_v;
    //extern int16_t posicao_relativa;
    //extern int16_t  vp_linear;    
    //sprintf(buffer, "%d\t%d\n", pulso_atualR, (int)(pulso_atualR * 1.34));
    //uart.enviar_string(buffer);

    //sprintf(buffer, "%d\n", setpoint);
    //uart.enviar_string(buffer);   

    //sprintf((char *)buffer, "%d\t%d\n", obter_pulsos_encoder_direito(), obter_pulsos_encoder_esquerdo());
    //sprintf(buffer, "%d\n", velocidades.linear);
    //usart0_send_string((uint8_t *)buffer);    
}

void enviar_tempo(void)
{
    static bool enviado = false;
    if (enviado)
        return;
    enviado = true;
    sprintf(buffer, "%ld\n", millisegundos);
    usart0_send_string((uint8_t *)buffer);
}

// ======================================================================================================
// ===================================== FINAL DO CODIGO ================================================
// ======================================================================================================