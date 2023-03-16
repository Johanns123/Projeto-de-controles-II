
// ============================================================================
// --- Frequ�ncia do MCU ---
#define CLOCK_FREQUENCY 16000000UL
#define SIZEBUFF   64


// ============================================================================
// --- Bibliotecas ---
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

// ============================================================================
// --- Prot�tipo das Fun��es USART ---
void usart0_config(uint8_t speed_mode, uint32_t bps);
void usart0_send_char(uint8_t carcatere);
void usart0_send_string(uint8_t *string);
uint8_t usart0_get_data(void);
uint8_t usart0_counter(void);
void _enviar_hexadecimal(uint8_t valor);
void _enviar_decimal(uint8_t valor);


/* ============================================================================

                                       _
                                      / \
                                     |oo >      <-- (USART, I love it!)
                                     _\=/_
                    ___         #   /  _  \   #
                   /<> \         \\//|/.\|\\//
                 _|_____|_        \/  \_/  \/
                | | === | |          |\ /|
                |_|  0  |_|          \_ _/
                 ||  0  ||           | | |
                 ||__*__||           | | |
                |* \___/ *|          []|[]
                /=\ /=\ /=\          | | |
________________[_]_[_]_[_]_________/_]_[_\_______________________________


============================================================================ */
/* --- Final do Arquivo usart0.h --- */