
/*
#include <zephyr/kernel.h>             // Funções básicas do Zephyr (ex: k_msleep, k_thread, etc.)
#include <zephyr/device.h>             // API para obter e utilizar dispositivos do sistema
#include <zephyr/drivers/gpio.h>       // API para controle de pinos de entrada/saída (GPIO)
#include <pwm_z42.h>                // Biblioteca personalizada com funções de controle do TPM (Timer/PWM Module)
*/

/*
// Define o valor do registrador MOD do TPM para configurar o período do PWM
#define TPM_MODULE 1000         // Define a frequência do PWM fpwm = (TPM_CLK / (TPM_MODULE * PS))
// Valores de duty cycle correspondentes a diferentes larguras de pulso
uint16_t leddesligado  = TPM_MODULE;       
uint16_t duty_50  = TPM_MODULE/2;   // 50% de duty cycle (meio brilho)
uint16_t duty_75  = TPM_MODULE/4; 
uint16_t duty_100  = TPM_MODULE/10; 
//uint16_t duty_100  = TPM_MODULE/10; 
*/


/*
int main(void)
{
    // Inicializa o módulo TPM2 com:
    // - base do TPMx
    // - fonte de clock PLL/FLL (TPM_CLK)
    // - valor do registrador MOD
    // - tipo de clock (TPM_CLK)
    // - prescaler de 1 a 128 (PS)
    // - modo de operação EDGE_PWM
    pwm_tpm_Init(TPM2, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_128, EDGE_PWM);

    // Inicializa o canal 0 do TPM2 para gerar sinal PWM na porta GPIOB_18
    // - modo TPM_PWM_H (nível alto durante o pulso)
    pwm_tpm_Ch_Init(TPM2, 0, TPM_PWM_H, GPIOB, 18);
	pwm_tpm_Ch_Init(TPM2, 1, TPM_PWM_H, GPIOB, 19);

    // Define o valor do duty cycle: nesse caso, duty_100 (LED quase desligado)
     /*pwm_tpm_CnV(TPM2, 0, duty_100);
	 pwm_tpm_CnV(TPM2, 0, duty_50);
	 pwm_tpm_CnV(TPM2, 0, duty_75);
	 pwm_tpm_CnV(TPM2, 0, duty_100);

	 pwm_tpm_CnV(TPM2, 1, duty_100);
	 pwm_tpm_CnV(TPM2, 1, duty_50);
	 pwm_tpm_CnV(TPM2, 1, duty_75);
	 pwm_tpm_CnV(TPM2, 1, duty_100);
	 */
    // Loop infinito

	/*
    for (;;)
    {
		pwm_tpm_CnV(TPM2, 0, duty_100);
		pwm_tpm_CnV(TPM2, 1, duty_50);
		k_msleep(1000); 
		pwm_tpm_CnV(TPM2, 0, duty_75); // O programa poderia alterar o duty cycle dinamicamente aqui se desejado
		pwm_tpm_CnV(TPM2, 1, duty_50);
		k_msleep(1000); // Aguarda 1 segundo antes de repetir o ciclo
		pwm_tpm_CnV(TPM2, 0, duty_100);
		pwm_tpm_CnV(TPM2, 1, duty_50);
		k_msleep(1000); // Aguarda 1 segundo antes de repetir o ciclo
		pwm_tpm_CnV(TPM2, 0, leddesligado);
		pwm_tpm_CnV(TPM2, 1, leddesligado);
		k_msleep(1000); // Aguarda 1 segundo antes de repetir o ciclo
	
    }

    return 0;
}
	*/

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <pwm_z42.h>

#define TPM_MODULE 1000

// ✅ LOCAL 1: DEFINIR VARIÁVEIS DE ENTRADA
// Aqui você receberia a velocidade (0-100%)
uint16_t velocidade = 80;  // ex: 50% de velocidade

uint16_t duty_desligado  = TPM_MODULE; 
uint16_t duty_50  = TPM_MODULE/2;
uint16_t duty_100 = TPM_MODULE/10;

int main(void)
{
    pwm_tpm_Init(TPM2, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_128, EDGE_PWM);
    pwm_tpm_Ch_Init(TPM2, 0, TPM_PWM_H, GPIOA, 1);
    pwm_tpm_Ch_Init(TPM2, 1, TPM_PWM_H, GPIOA, 2);

    const struct device *port_b = DEVICE_DT_GET(DT_NODELABEL(gpiob));
    gpio_pin_configure(port_b, 0, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure(port_b, 1, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure(port_b, 2, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure(port_b, 3, GPIO_OUTPUT_INACTIVE);

    for (;;)
    {
        // ✅ LOCAL 2: CONVERTER VELOCIDADE PARA DUTY CYCLE
        // Mapiar velocidade (0-100%) para duty_cycle (0-TPM_MODULE)
        uint16_t duty_value = (velocidade * TPM_MODULE)/100;

        gpio_pin_set(port_b, 0, 1);  // Direção Motor A
        gpio_pin_set(port_b, 1, 0);
        gpio_pin_set(port_b, 2, 1);  // Direção Motor B
        gpio_pin_set(port_b, 3, 0);

        // ✅ LOCAL 3: USAR O VALOR CONVERTIDO
        pwm_tpm_CnV(TPM2, 0, duty_value);  // Aplicar velocidade Motor A
        pwm_tpm_CnV(TPM2, 1, duty_value);  // Aplicar velocidade Motor B



        k_msleep(100);
    }

    return 0;
}
