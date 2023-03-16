#include "PID.h"
#include "usart0.h" //apagar apos teste
#include <stdio.h>

#define ANGULAR 0
#define LINEAR  1


void acoes_de_controle(float *proporcional, float *integral, float *derivativo, int16_t error, int8_t dado);
int16_t filtro_derivativo(int16_t derivacao);

char *string;

float kp_ang = {11.0},    // aumentar se o robô não estiver fazendo curva
      ki_ang = {0.000004},// aumentar se o robô estiver oscilando muito
      kd_ang = {0.0004};  // aumentar se na passagem de reta para curva e vice-versay o robô balançar muito

float kp_line = {50},
      kd_line = {0.00000},
      ki_line = {0.004};

/*Sera necessário alterar as constantes do controle PID*/

int16_t PID(int16_t error, int8_t dado) /* Algoritmo de controle PID usando os sensores frontais */
{   
    int16_t correcao = 0;

    static float  proporcional = 0;
    static float  integral     = 0;
    static float  derivativo   = 0;

    acoes_de_controle(&proporcional, &integral, &derivativo, error, dado);
    correcao = (proporcional + integral + derivativo);
    
    return correcao; 

} /* end PID */

void acoes_de_controle(float *proporcional, float *integral, float *derivativo, int16_t error, int8_t dado)
{
    static int16_t erroAnterior     = 0;
    static int16_t acao_integrativa = 0;
    static int16_t acao_derivativa  = 0; 
    static int16_t acao_derivativa_filtrada = 0;

    if(!error)
        acao_integrativa = 0;
    acao_integrativa += error;
    
    acao_derivativa = error - erroAnterior;
    erroAnterior    = error;  

    acao_derivativa_filtrada = filtro_derivativo(acao_derivativa);

    //sprintf(string, "%d\t%d\n", acao_integrativa, acao_derivativa_filtrada);
    //usart0_send_string((int8_t *)string);   //apagar apos teste

    if(dado == ANGULAR)
    {
        *proporcional = (kp_ang*error);  
        *integral     = (ki_ang*acao_integrativa);
        *derivativo   = (kd_ang*acao_derivativa_filtrada);
    }

    else if(dado == LINEAR)
    {
        *proporcional = (kp_line*error);  
        *integral     = (ki_line*acao_integrativa);
        *derivativo   = (kd_line*acao_derivativa_filtrada);
    }
    
}


int16_t filtro_derivativo(int16_t derivacao)
{
    #define NUMERO_DE_AMOSTRAS 10
    static int16_t erro_derivativo[NUMERO_DE_AMOSTRAS] = {0};
    int16_t acumulador = 0;

    for (int i = NUMERO_DE_AMOSTRAS - 1; i > 0; i--)
        erro_derivativo[i] = erro_derivativo[i - 1];

    erro_derivativo[0] = derivacao;

    for (int i = 0; i < NUMERO_DE_AMOSTRAS; i++)
    {
        acumulador += erro_derivativo[i];
    }

    return acumulador / NUMERO_DE_AMOSTRAS;
}