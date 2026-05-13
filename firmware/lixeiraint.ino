#include <WiFi.h>
#include <PubSubClient.h>

// Conectividade
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com"; // Servidor público solicitado

WiFiClient espClient;
PubSubClient client(espClient);

// Definindo os Pinos
const int trigPin = 12;      // Sensor TRIG
const int echoPin = 14;      // Sensor ECHO
const int ledVerde = 2;      // LED Verde
const int ledAmarelo = 4;    // LED Amarelo
const int ledVermelho = 16;   // LED Vermelho (Pino 16 conforme sua foto)

void setup_wifi() {
  delay(10);
  Serial.print("\nConectando em ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // Tenta conectar
  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 20) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWi-Fi Conectado com sucesso!");
  } else {
    Serial.println("\nWi-Fi offline (Modo de teste local)");
  }
}

void reconnect() {
  while (!client.connected() && WiFi.status() == WL_CONNECTED) {
    Serial.print("Tentando conexão MQTT...");
    if (client.connect("LixeiraInteligente_Projeto_ODS11")) {
      Serial.println("Conectado ao Broker!");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200); // Velocidade para ESP32
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) reconnect();
    client.loop();
  }

  //Medindo Distância 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracao = pulseIn(echoPin, HIGH);
  int distancia = duracao * 0.034 / 2;

  //Exibição no Monitor 
  Serial.print("Distancia capturada: ");
  Serial.print(distancia);
  Serial.println(" cm");

  //Alertas e Atuadores 
  
  // 1. Nível Crítico (100% Cheia)
  if (distancia < 10) {
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVerde, LOW);
    Serial.println(">>> Alerta Máximo: LIXEIRA 100% CHEIA! <<<");
    if (client.connected()) client.publish("ods11/lixeira/status", "100% Cheia");
  } 
  
  // 2. Nível de Atenção (80% Cheia)
  else if (distancia <= 30) { 
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledAmarelo, HIGH); // Agora o Amarelo deve acender!
    digitalWrite(ledVerde, LOW);
    Serial.println("--- Aviso preventivo: Lixeira 80% cheia ---");
    if (client.connected()) client.publish("ods11/lixeira/status", "80% Cheia");
  } 
  
  // 3. Nível Normal
  else {
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVerde, HIGH);
    Serial.println("Status: Normal - Espaço disponível.");
    if (client.connected()) client.publish("ods11/lixeira/status", "OK");
  }

  delay(1000); // Aguarda 1 segundo para a próxima leitura
}
