#include "ADC_state_machine.h"

uint8_t sensores_de_tensao[2];

uint8_t _valor_lido(uint8_t sensor) 
{
    return (sensores_de_tensao[sensor]);
}


void tratar_leitura_do_ADC(void)
{/* inicializo no setup na fun��o calibration e em seguida toda
     * vez que ocorre uma conver��o a interrup��o do AD ocorre
     * e ent�o esta fun��o � chamada pelo vetor de interrup��o
     * do AD, obtendo os dados da convers�o em "paralelo" � rotina */
    
    /* Leio primeiro o default que seria o primeiro canal
     * e em seguida fa�o uma l�gica circular de leitura dos canais */
    
  static uint8_t estado = 10;
  
  switch (estado) {
      
    case 0:
      estado = 1;
      sensores_de_tensao[0] = adc_read_service();
      adc_conversion_ch_service(1);
      break;
        
    case 1:
      estado = 2;
      sensores_de_tensao[1] = adc_read_service();
      adc_conversion_ch_service(0);
      break;

    default:
      estado = 0;
      adc_conversion_ch_service(0);
      sensores_de_tensao[0] = adc_read_service();
      break; 
  }    
}


ISR(ADC_vect) {
    tratar_leitura_do_ADC();
}