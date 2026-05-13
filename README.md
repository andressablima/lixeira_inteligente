# 🗑️ Lixeira Inteligente — ODS 11

Protótipo de IoT para monitoramento volumétrico de resíduos sólidos urbanos, desenvolvido como projeto final da disciplina de **Objetos Inteligentes Conectados** do curso de Análise e Desenvolvimento de Sistemas da Universidade Presbiteriana Mackenzie.

A solução utiliza um sensor ultrassônico para medir o nível de preenchimento da lixeira em tempo real, aciona LEDs indicadores conforme o nível detectado e publica alertas via protocolo MQTT, contribuindo para a meta 11.6 do **ODS 11: Cidades e Comunidades Sustentáveis**.

---

## 📋 Sumário

- [Funcionamento](#funcionamento)
- [Hardware](#hardware)
- [Como reproduzir](#como-reproduzir)
- [Protocolo MQTT](#protocolo-mqtt)
- [Estrutura do repositório](#estrutura-do-repositório)

---

## Funcionamento

O ESP32 lê continuamente a distância medida pelo sensor HC-SR04 posicionado na tampa da lixeira. Com base na distância, o sistema determina o nível de preenchimento e aciona o LED correspondente:

| Distância medida | Nível | LED acionado | Mensagem MQTT |
|---|---|---|---|
| > 30 cm | Normal | 🟢 Verde (GPIO 2) | `OK` |
| 10 cm – 30 cm | Atenção (80%) | 🟡 Amarelo (GPIO 4) | `80% CHEIA` |
| < 10 cm | Crítico (100%) | 🔴 Vermelho (GPIO 16) | `100% CHEIA` |

O ciclo de leitura e publicação ocorre a cada **1 segundo**. Se a conexão Wi-Fi não estiver disponível, o sistema continua operando localmente (LEDs e Serial Monitor), sem publicar no broker.

---

## Hardware

### Lista de componentes

| Componente | Especificação | Quantidade |
|---|---|---|
| Microcontrolador | ESP32 DevKit C v4 | 1 |
| Sensor ultrassônico | HC-SR04 (2–400 cm, 40 kHz) | 1 |
| LED verde | 5 mm, 20 mA | 1 |
| LED amarelo | 5 mm, 20 mA | 1 |
| LED vermelho | 5 mm, 20 mA | 1 |
| Resistor | 220 Ω, ±5% | 3 |
| Protoboard | Mini breadboard | 1 |
| Jumpers | Macho-macho | — |

### Mapeamento de pinos

| Componente | Pino ESP32 | Função |
|---|---|---|
| HC-SR04 TRIG | GPIO 12 | Saída — pulso 10 µs |
| HC-SR04 ECHO | GPIO 14 | Entrada — mede duração |
| HC-SR04 VCC | 5V | Alimentação |
| HC-SR04 GND | GND | Terra |
| LED Verde | GPIO 2 | Saída — nível normal |
| LED Amarelo | GPIO 4 | Saída — nível de atenção |
| LED Vermelho | GPIO 16 | Saída — alerta crítico |

---

## Como reproduzir

### Simulação (Wokwi)

1. Acesse [wokwi.com](https://wokwi.com) e crie um projeto com ESP32
2. Monte o circuito conforme o mapeamento de pinos acima
3. Cole o código de `/firmware/lixeiraint.ino` no editor
4. Clique em **Start Simulation**
5. Abra o Serial Monitor para acompanhar as leituras

### Hardware físico

1. Monte o circuito na protoboard conforme o mapeamento de pinos
2. Instale a [Arduino IDE 2.x](https://www.arduino.cc/en/software)
3. Adicione o suporte ao ESP32: `Arquivo → Preferências → URLs adicionais`:  
   `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
4. Instale o pacote **esp32 by Espressif Systems** via Gerenciador de Placas
5. Instale a biblioteca **PubSubClient** (v2.8) via Gerenciador de Bibliotecas
6. Abra `/firmware/lixeiraint.ino` e faça o upload para o ESP32

---

## Protocolo MQTT

O projeto utiliza o protocolo **MQTT (Message Queuing Telemetry Transport)** para comunicação com a nuvem via TCP/IP.

### Configuração

| Parâmetro | Valor |
|---|---|
| Broker | `broker.hivemq.com` |
| Porta | `1883` (TCP) |
| Porta WebSocket | `8884` |
| Tópico de publicação | `ods11/lixeira/status` |
| QoS | 0 |
| Client ID | `LixeiraInteligente_Projeto_ODS11` |

### Monitorar as mensagens em tempo real

1. Acesse [hivemq.com/demos/websocket-client/](https://www.hivemq.com/demos/websocket-client/)
2. Host: `broker.hivemq.com` · Port: `8884` → **Connect**
3. Em Subscriptions → Topic: `ods11/lixeira/status` → **Subscribe**
4. As mensagens aparecerão em tempo real enquanto o protótipo estiver rodando

### Biblioteca utilizada

- **PubSubClient v2.8** — Nick O'Leary  
  [github.com/knolleary/pubsubclient](https://github.com/knolleary/pubsubclient)

---

## Estrutura do repositório

```
lixeira-inteligente-ods11/
├── firmware/
│   └── lixeiraint.ino  
└── README.md
```

---

## Autores

Projeto desenvolvido por **Andressa Bezerra de Lima** e **Leandro Carlos Fernandes**  
Universidade Presbiteriana Mackenzie — Análise e Desenvolvimento de Sistemas - 2026
