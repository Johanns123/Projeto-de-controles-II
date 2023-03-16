//#include "elementos/SensoresFrontais.h"
//#include "elementos/UART.h"
#include "IR_sensors.h"
#include <stdbool.h>
#include "usart0.h"
#include <stdio.h>

#define NUM_SENSORES 2

// VALORES OBTIDOS NA CALIBRAÇÃO DOS SENSORES
#define VALOR_MAX 224
#define VALOR_MIN 15

extern int8_t buffer[20];
static uint8_t sensores[NUM_SENSORES];
static uint8_t sensores_normalizados[NUM_SENSORES];
bool stop = 0;

extern uint8_t sensores_de_tensao[2];

int16_t obter_posicao_media(void) {
    int16_t numerador = 0;
    int16_t denominador = 0; 
    int16_t posicao_media = 0;
    int8_t posicoes[] = {20, -20};

    for (int i = 0; i < NUM_SENSORES; i++) {
        sensores_normalizados[i] = VALOR_MAX - sensores[i];
        numerador += sensores_normalizados[i] * posicoes[i];
        denominador += sensores_normalizados[i];  
        //sprintf(buffer, "%d\t", sensores[i]); //ver o peso dos sensores
        //uart.enviar_string(buffer);      
    }
    //uart.enviar_caractere('\n');
    

    if(!denominador)
    {
        posicao_media = 0;
        stop = 1;
    }
    else
    {
        posicao_media = numerador / denominador;
        stop = 0;
    }
    //sprintf((char *)buffer, "%d\n", posicao_media);
    //usart0_send_string((uint8_t *)buffer);
    return posicao_media;
}


void atualizar_leitura(void) {
    //======Estabelece o limiar da leitura dos sensores====//
    //função de correção da calibração
    uint8_t valor_lido;

    for (int i = 0; i < NUM_SENSORES; i++)
    {
        valor_lido = _valor_lido(i);
                
        if (valor_lido < VALOR_MIN)
            sensores[i] = VALOR_MIN;        
        else if (valor_lido > VALOR_MAX)
            sensores[i] = VALOR_MAX;
        else            
            sensores[i] = valor_lido;
    }
}
