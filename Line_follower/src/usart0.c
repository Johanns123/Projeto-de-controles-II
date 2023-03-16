// ============================================================================
// --- Biblioteca ---
#include "usart0.h"

// ============================================================================
// --- Vari�veis Globais ---
volatile uint8_t usart_run = 1; //1 indica uasart em funcionamento, 0 nao operando
volatile uint16_t rx_buff = 0;  //contador de bytes recebidos
volatile char rx_buffer[SIZEBUFF] = {0}; //buffer para recepcao da mensagem

// ============================================================================
// --- Interrup��es USART ---
ISR(USART_RX_vect)
{
  static uint16_t cnt_buff = 0; //contador de bytes local

  rx_buffer[cnt_buff] = UDR0;
  rx_buff++;      //contador de bytes auxiliar que indica quantos bytes foram recebidos
  cnt_buff++;
  if(cnt_buff >= SIZEBUFF)
    cnt_buff = 0;
}

ISR(USART_TX_vect)
{
  //usart_run = 1;      //indica que ocorreu transmissao
}

void usart0_config(uint8_t speed_mode, uint32_t bps)
{
  if (speed_mode)
  {
    UCSR0A |= (1<<U2X0); 
    UBRR0   = (CLOCK_FREQUENCY/(8UL*bps)) - 1;
  }

  else
  {
    UCSR0A &= ~(1<<U2X0);
    UBRR0 = (CLOCK_FREQUENCY/(16UL*bps)) - 1;
  }

  UCSR0B |= (1<<RXCIE0) | (1<<TXCIE0) | (1<<RXEN0) | (1<<TXEN0);

  UCSR0C = 0x06;  //sem paridade, sem stop bit, 1 byte de trasnmissao 
}

void usart0_send_char(uint8_t carcatere)
{
  UDR0 = carcatere;
  while (!(UCSR0A & (1<<UDRE0))); //aguarda a usart ficar liberada para trasmissao

  //while(!usart_run);
  //UDR0 = carcatere;
  //usart_run = 0;
}

void usart0_send_string(uint8_t *string)
{
  for(int i = 0; string[i] != '\0'; i++)
    usart0_send_char(string[i]);
}

uint8_t usart0_get_data(void)
{
  static uint16_t local_buff = 0;
  static uint8_t byte_rx = 0;

  byte_rx = rx_buffer[local_buff];
  local_buff++;
  rx_buff--;

  if(local_buff >= SIZEBUFF)
    local_buff = 0;

  return byte_rx;
}

uint8_t usart0_counter(void)
{
  return rx_buff;
}

void _enviar_hexadecimal(uint8_t valor)
{   
  uint8_t i,temp;
  for (i=0; i<2; i++)
  {
      temp = (valor & 0xF0)>>4;
      if ( temp <= 9)
          usart0_send_char('0' + temp);
      else
          usart0_send_char('A' + temp - 10);
      valor = valor << 4;    
    }
}

void _enviar_decimal(uint8_t valor) 
{
  static uint8_t cenmil = 0, dezmil = 0, mil = 0, cen = 0, dez = 0, uni = 0;

  cenmil = valor / 100000;
  dezmil = (valor%100000) / 10000;
  mil = (valor % 10000) / 1000;
  cen = (valor%1000) / 100;
  dez = (valor%100)/10;
  uni = (valor%10);
  
  usart0_send_char(cenmil + 48);
  usart0_send_char(dezmil + 48);
  usart0_send_char(mil + 48);
  usart0_send_char(cen + 48);
  usart0_send_char(dez + 48);
  usart0_send_char(uni + 48);
} 