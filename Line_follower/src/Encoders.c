#include "Encoders.h"
#include "usart0.h"
#include <stdio.h>

#define LARGURA_DO_ROBO 1.51 //dm
#define RAIO_RODA    32.5 //mm


static uint16_t pulsos_encoder_esquerdo = 0;

uint16_t obter_pulsos_encoder_esquerdo(void) 
{
    return pulsos_encoder_esquerdo;
}

static void contar_pulsos_encoder_esquerdo(void) {
    
    pulsos_encoder_esquerdo++; // sentido horario

}

static uint16_t pulsos_encoder_direito = 0;

uint16_t obter_pulsos_encoder_direito(void)
{
    return pulsos_encoder_direito;
}

static void contar_pulsos_encoder_direito(void) {

    pulsos_encoder_direito++;
}

ISR(INT0_vect)
{
    contar_pulsos_encoder_direito();
}

ISR(INT1_vect)
{
    contar_pulsos_encoder_esquerdo();
}

//const float conversao = 1.765;
const float conversao = 10.205;   //rotacao/pulsos -> 65*pi/20pulsos
const float conv_segundos = 500.0;  //quando a base de tempo é de 2ms

extern int8_t buffer[20];

static int16_t calcular_velocidade_roda_direita(void) {
    static uint16_t pulso_atualR, pulso_anteriorlR;
    static uint16_t deltaR = 0, delta_EncR = 0;
    uint16_t vel = 0;
    
    pulso_atualR = obter_pulsos_encoder_direito();
    delta_EncR = pulso_atualR - pulso_anteriorlR; //variacao de pulsos
    deltaR = delta_EncR * conversao;  //distancia em mm (variacao da distância)

    if(!deltaR) 
        vel = 0;  //roda parada
    else
    {
        vel = deltaR * conv_segundos;  //converte para mm/s
        pulso_anteriorlR = pulso_atualR;
        pulso_atualR = 0x00;    //0
    }

    //sprintf(buffer, "%d\n", (int)(vel));
    //uart.enviar_string(buffer);

    return vel;
}


static int16_t calcular_velocidade_roda_esquerda(void) {
    static uint16_t pulso_atualL, pulso_anteriorlL;
    static uint16_t deltaL = 0;
    static uint16_t delta_EncL = 0;
    uint16_t vel = 0;

    pulso_atualL = obter_pulsos_encoder_esquerdo();
    delta_EncL = pulso_atualL - pulso_anteriorlL; //variacao de pulsos
    deltaL = delta_EncL * conversao;  //distancia em dm (variacao da distância)

    if(!deltaL) 
        vel = 0; //roda parada
    else
    {
        vel = deltaL * conv_segundos;  //variacao da distancia * fator de conversão para mm/s
        pulso_anteriorlL = pulso_atualL;
        pulso_atualL = 0x00;
    }

    //sprintf((char *)buffer, "%d\n", (int)(vel));
    //usart0_send_string((uint8_t *)buffer);

    return vel;
}

velocidades_t _calcular_velocidades_do_robo(void)
{
    int16_t v_roda_direita = calcular_velocidade_roda_direita();
    int16_t v_roda_esquerda = calcular_velocidade_roda_esquerda();

    velocidades_t velocidades = {
        //.angular = ((v_roda_direita - v_roda_esquerda) * RAIO_RODA) / (2 * LARGURA_DO_ROBO) * (180/3.14), //rad/s
        .angular = ((v_roda_direita - v_roda_esquerda)) / (2 * 100 * LARGURA_DO_ROBO), // conversao em rad/s
        .linear = (v_roda_direita + v_roda_esquerda) / (2 * 100)     // conversao em dm/s 
    };

    return velocidades;
}
