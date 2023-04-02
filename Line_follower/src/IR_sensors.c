//#include "elementos/SensoresFrontais.h"
//#include "elementos/UART.h"
#include "IR_sensors.h"
#include <stdbool.h>
#include "usart0.h"
#include <stdio.h>

#define NUM_SENSORES 2

// VALORES OBTIDOS NA CALIBRAÇÃO DOS SENSORES
#define VALOR_MAX 1
#define VALOR_MIN 0

extern int8_t buffer[20];
static uint8_t sensores[NUM_SENSORES];
static uint8_t sensores_normalizados[NUM_SENSORES];
uint8_t valor_lido;

bool stop = 0;

uint8_t sensores_de_tensao[2];

int16_t obter_posicao_media(void) {
    int16_t numerador = 0;
    int16_t denominador = 0; 
    int16_t posicao_media = 0;
    int8_t posicoes[] = {20, -20};

    for (int i = 0; i < NUM_SENSORES; i++) {
        sensores_normalizados[i] = VALOR_MAX - sensores[i];
        numerador += sensores_normalizados[i] * posicoes[i];
        denominador += sensores_normalizados[i];  
        //sprintf((char *)buffer, "%d\t", sensores_normalizados[i]); //ver o peso dos sensores
        //usart0_send_string((uint8_t *)buffer);      
    }
    //usart0_send_char('\n');
    

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
    // sprintf((char *)buffer, "%d\n", posicao_media);
    // usart0_send_string((uint8_t *)buffer);
    return posicao_media;
}


void atualizar_leitura(void) {
    sensores_de_tensao[0] = PINC & (1<<2);
    sensores_de_tensao[1] = PINC & (1<<1);
    //======Estabelece o limiar da leitura dos sensores====//
    //função de correção da calibração
    //uint8_t valor_lido;

    for (int i = 0; i < NUM_SENSORES; i++)
    {
        valor_lido = sensores_de_tensao[i];
                
        if (valor_lido < VALOR_MIN)
            sensores[i] = VALOR_MIN;        
        else if (valor_lido > VALOR_MAX)
            sensores[i] = VALOR_MAX;
        else            
            sensores[i] = valor_lido;
    }
}
