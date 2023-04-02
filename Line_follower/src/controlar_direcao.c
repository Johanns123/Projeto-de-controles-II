// MÓDULO PARA ESTRATÉGIA DE CONTROLE DE DIREÇÃO DO ROBÔ

#include "controlar_direcao.h"

static void calcular_variavel_de_controle_rotacional(void); 
void atualizar_rotacao_dos_motores(void);
//void fora_da_pista(void);

//#define PID_X   // Ao comentar o PID translacional eh desabilitado
extern int8_t buffer[20];

int16_t u_w = 0;
int16_t erro_w = 0;

int16_t PV_angular = 0, setpoint = 0;

static char saida;

static int16_t posicao_media_sensores_frotais = 0;

extern bool stop;

extern int8_t velocidade_filtrada_angular;
extern uint8_t valor_lido;

extern velocidades_t velocidades;
void control_direction(void) 
{   

    //volta_pra_pista(sensores_frontais.posicao_media());
    posicao_media_sensores_frotais = obter_posicao_media();
    
    fora_da_pista();

    if(!stop)
    {
        calcular_variavel_de_controle_rotacional();
        atualizar_rotacao_dos_motores();

    }
}


void fora_da_pista(void) //ocorrer se apos algumas tentativas ele não voltar para a linha
{
    static uint8_t contador = 0;
    bool teste = 1;

    if(stop){
         contador++;
         if(contador > 10){

            motor_off();
            //if(teste){//prendo meu código   
                posicao_media_sensores_frotais = obter_posicao_media();
                motor_tras();
                motor_alterar_velocidade_dir(80);
                motor_alterar_velocidade_esq(80);
                //envia_dado();
                // if(posicao_media_sensores_frotais != 0){
                //     teste = 0;
                // }
            //}
           
        }
    }
    else{
        contador = 0;
    } 

}

static void calcular_variavel_de_controle_rotacional(void)
{
    static int16_t  vp_angular = 0; //variavel de processo
    setpoint = posicao_media_sensores_frotais;
    vp_angular = velocidade_filtrada_angular;
    erro_w = setpoint - vp_angular; // Variavel de processo (PV)
    u_w    = PID(erro_w, FRONTAIS);  // Variavel manipulada  (MV)
    // sprintf((char *)buffer, "%d\t%d\t%d\n", setpoint, vp_angular, erro_w);
    // usart0_send_string(buffer);
}

void atualizar_rotacao_dos_motores(void) 
{    
    motor_frente();

    variable.PWM_right   = variable.PWM_front_motors + u_w; // Saida do controle translacional corrigindo o PWM em questão
    variable.PWM_left    = variable.PWM_front_motors - u_w;  // Saida do controle rotacional corrigindo o PWM em questão
    calc_pwm_limit(&variable);
    
    motor_alterar_velocidade_dir(variable.PWM_right);
    motor_alterar_velocidade_esq(variable.PWM_left);
}