<<<<<<< HEAD
How to build PlatformIO based project
=====================================

1. [Install PlatformIO Core](https://docs.platformio.org/page/core.html)
2. Download [development platform with examples](https://github.com/platformio/platform-freescalekinetis/archive/develop.zip)
3. Extract ZIP archive
4. Run these commands:

```shell
# Change directory to example
$ cd platform-freescalekinetis/examples/zephyr-blink

# Build project
$ pio run

# Upload firmware
$ pio run --target upload

# Build specific environment
$ pio run -e frdm_k64f

# Upload firmware for the specific environment
$ pio run -e frdm_k64f --target upload

# Clean build files
$ pio run --target clean
```
=======
# zephyr-freedom-PWM
Controle de LED e motor usando PWM na placa Freedom com VSCode, Zephyr, MCUXpresso e PlatformIO.

**IMPORTANTE**:

- SUBA OS CÓDIGOS A CADA TESTE REALIZADO COM SUCESSO, DE FORMA **INCREMENTAL**.

- DESCREVA OS RESULTADOS OBTIDOS, INCLUINDO TAMBÉM SUAS **EVIDÊNCIAS**.

# 🏎️ Roteiro de Desenvolvimento e Testes

Este repositório contém os passos de validação técnica para o controle de motores utilizando a placa **Freedom KL25Z** com o sistema operacional **Zephyr**.

## 🛠️ Configuração Inicial (Setup)
Antes de iniciar, certifique-se de ter configurado o ambiente seguindo os guias abaixo:
* [Instalação: VSCode + Zephyr para Freedom](https://cliff-collision-333.notion.site/Instala-o-1f1a7f4c2a2b81aca6e4ce96cf7171b3)
* [Atualização para Zephyr 4.2](https://cliff-collision-333.notion.site/Atualiza-o-para-o-Zephyr-4-2-27aa7f4c2a2b802a9a3efac5c8691a84)

---

## 🟢 1. Hello World Zephyr (GPIO Básico)
**Objetivo:** Validar a instalação do Toolchain e a comunicação básica com a placa.
*Referência: [Exemplo Pisca LED RTOS](https://cliff-collision-333.notion.site/Exemplo-Pisca-LED-RTOS-1f1a7f4c2a2b8142bf66ddc7dbf306f2)*

| ID | Teste | Resultado Esperado |
|:---:|:---|:---|
| 1.1 | LED Verde (1s) | O LED onboard verde deve piscar com intervalo de 1000ms. |
| 1.2 | LED Verde (2s) | O LED onboard verde deve piscar com intervalo de 2000ms. |

---

## 📚 2. Integração de Biblioteca PWM (`pwm_z42`)
**Objetivo:** Validar a biblioteca de hardware e a configuração de Timers (TPM).
*Referência: [Tutorial Biblioteca PWM](https://cliff-collision-333.notion.site/PWM-Variar-a-Intensidade-do-LED-Zephyr-4-2-1f1a7f4c2a2b8115acb8c0a408af78ce) e [Artigo Embarcados](https://embarcados.com.br/biblioteca-pwm-para-frdm-kl25z/)*

| ID | Teste | Resultado Esperado |
|:---:|:---|:---|
| 2.1 | LED Laranja Fixo | O LED RGB deve exibir cor laranja constante (mix de PWM). |
| 2.2 | LED Laranja Piscante | O sinal PWM deve ser habilitado/desabilitado a cada 1s. |
| 2.3 | **(Opcional)** LED Externo | LED conectado ao pino escolhido deve variar intensidade via PWM. |

---

### 💡 Teste Opcional: Validação com LED Externo
Este teste é recomendado caso os motores não girem posteriormente, servindo para isolar se o problema é o sinal lógico da Freedom ou a potência da Ponte H.
O foco é validar se o sinal PWM está saindo corretamente do microcontrolador antes de usar na Ponte H. O LED é uma excelente ferramenta para confirmar visualmente o Duty Cycle suportado pelo PWM.

* [Exemplo LED Externo](https://www.makerhero.com/blog/aprenda-a-piscar-um-led-com-arduino/?srsltid=AfmBOorP4sIpK4d2xrkZ4Wp7kBNDfmBs22xvHokEJPNuZa-ezOg2Y-B-)

**Esquema de Ligação:**
* **Anodo (Perna longa):** Conectar ao pino **PTB2** (ou pino PWM escolhido).
* **Catodo (Perna curta):** Conectar ao **GND** da Freedom (através de um resistor de 220Ω a 1kΩ para proteção).

| ID | Descrição do Teste | Resultado Esperado | Resultado Obtido |
|:---|:---|:---|:---|
| **2.3.1** | Variar Brilho (Fade) | O LED deve aumentar e diminuir a intensidade suavemente conforme o valor de `CnV` enviado. | |
| **2.3.2** | Verificação de Nível | Com `CnV` em 1000 (Max), o LED deve brilhar intensamente; com 0, deve apagar (ou vice-versa, dependendo da lógica). | |

**Por que fazer este teste?**
1.  **Confirmação de MUX:** Garante que o pino físico foi corretamente configurado para saída PWM.
2.  **Referência de Tensão:** Confirma que o GND está com a mesma referência.
3.  **Depuração de Código:** Se o LED interno (onboard) funciona mas o externo não, o erro está na definição do pino ou no uso do registrador (Vide [Artigo Embarcados](https://embarcados.com.br/biblioteca-pwm-para-frdm-kl25z/)*).

**Dica:** Se o LED externo funcionar mas o motor não, verifique o **GND comum**. O LED usa o GND da própria placa, enquanto o motor precisa que o GND da bateria "converse" com o GND da Freedom para entender o sinal PWM.

---

## ⚡ 3. Validação de Hardware: Ponte H

* [Guia de Uso da Ponte H L298N](https://blog.eletrogate.com/guia-definitivo-de-uso-da-ponte-h-l298n/)

**Objetivo:** Isolar problemas elétricos e garantir que os motores/bateria estão operacionais.

> **⚠️ Configuração de Hardware:**
> 1. Jumpers inseridos em **ENA** e **ENB**.
> 2. Conectar pinos **IN1** e **IN3** diretamente ao **VCC (5V)** da placa.

| ID | Teste | Resultado Esperado |
|:---:|:---|:---|
| 3.1 | Movimento Independente | Os motores devem girar em velocidade máxima ao receber alimentação. |

---

## 🏁 4. Integração Final (Controle Digital)
**Objetivo:** Controlar velocidade e direção via software Zephyr.

> **⚠️ Configuração de Hardware:**
> 1. **Remover** jumpers de ENA/ENB.
> 2. Conectar **ENA -> PINO PWM1** e **ENB -> PINO PWM2** (Pinos PWM escolhidos).
> 3. Conectar **IN1/IN2/IN3/IN4** aos pinos digitais configurados no código (também é possível deixar com a configuração utilizada no teste 3 somente para movimento de ambos motores para frente).
> 4. **IMPORTANTE:** Unir o **GND** da bateria com o **GND** da KL25Z.

| ID | Teste | Resultado Esperado |
|:---:|:---|:---:|
| 4.1 | Teste Motor Único | Apenas um motor deve variar velocidade usando um pino PWM. |
| 4.2 | Teste Bi-Motor | Ambos os motores operando em sincronia para movimento em linha reta. |

---
>>>>>>> 49f3cdba3838525a6e9a0f03271bac4c0bf2d583
