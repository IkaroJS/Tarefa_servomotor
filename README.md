# Controle de Servo Motor com LED RGB no Raspberry Pi Pico

Este projeto demonstra como controlar um **servo motor** e um **LED RGB** usando um **Raspberry Pi Pico**. O servo motor se move suavemente entre **0° e 180°**, enquanto o LED RGB muda de cor para indicar a posição atual do servo.

## Autor
**Ikaro Jesus da Silva**

## Componentes Necessários
- Raspberry Pi Pico
- Servo Motor (5V)
- LED RGB (cátodo comum ou ânodo comum)
- Resistores (220Ω para cada canal do LED RGB, se necessário)
- Protoboard e fios jumper

## Conexões
### **Servo Motor**
- **VCC (Vermelho)** → VBUS (5V) do Raspberry Pi Pico
- **GND (Marrom/Preto)** → GND do Raspberry Pi Pico
- **Sinal (Amarelo/Laranja)** → GPIO **22** do Raspberry Pi Pico

### **LED RGB**
- **Vermelho** → GPIO **13**
- **Verde** → GPIO **11**
- **Azul** → GPIO **12**
- **GND (cátodo comum)** → GND do Raspberry Pi Pico

## Código
O código está escrito em **C** e utiliza a biblioteca `hardware/pwm.h` para controlar tanto o servo motor quanto o LED RGB.

## Funcionalidades
### **Movimentação do Servo**
- **0°** → Pulso de **500µs**
- **90°** → Pulso de **1470µs**
- **180°** → Pulso de **2400µs**

### **Movimentação Suave**
- Incrementos de **5µs** com atraso de **10ms** entre ajustes para transições suaves.

### **Controle do LED RGB**
- **Servo em 180°** → LED **Vermelho**
- **Servo em 90°** → LED **Verde**
- **Servo em 0°** → LED **Azul**
- **Transição Suave** → Durante a movimentação, o LED muda progressivamente de Azul para Vermelho e vice-versa.

## Cálculos
### **Frequência do PWM (Servo Motor)**
O servo motor requer um sinal PWM de **50Hz** (período de 20ms). O Raspberry Pi Pico tem um clock de **125MHz**. Para gerar um sinal de 50Hz, utilizamos a seguinte fórmula:

\[ wrap = \frac{Clock_{Pico}}{Freq_{PWM} \times Divisor} \]

Com um divisor de **64**, obtemos um `wrap` de **39062**, garantindo um período de **20ms**.

### **Controle PWM do LED RGB**
O LED RGB é controlado por **PWM de 8 bits** (0-255) em cada canal:
```c
pwm_set_gpio_level(LED_VERMELHO_PIN, vermelho);
pwm_set_gpio_level(LED_VERDE_PIN, verde);
pwm_set_gpio_level(LED_AZUL_PIN, azul);
```

## Como Executar
1. **Clone o repositório:**
   ```sh
   git clone https://github.com/IkaroJS/Tarefa_servomotor.git
   ```
2. **Entre no diretório do projeto:**
   ```sh
   cd Tarefa_servomotor
   ```
3. **Configure o ambiente de desenvolvimento:**
   - Instale o SDK do Raspberry Pi Pico
   - Utilize o **CMake** para compilar o código
4. **Compile o código:**
   ```sh
   mkdir build && cd build
   cmake ..
   make
   ```
5. **Carregue o firmware no Raspberry Pi Pico:**
   - Conecte o Pico ao computador pressionando o botão **BOOTSEL** e solte após conectar.
   - Copie o arquivo `.uf2` gerado para o **RPI-RP2**.
6. **Observe o funcionamento:**
   - O servo motor se moverá entre **0° e 180°**.
   - O LED RGB indicará a posição do servo com cores diferentes.

   ## Vídeo de demonstração

[Assistir Vídeo](https://drive.google.com/drive/folders/1_tlaEl3lV43GpqqIbEpfxhdka27jECuh?usp=sharing)