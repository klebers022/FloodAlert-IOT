/********************************************************************
 * Projeto: SafeCap - Monitoramento Térmico IoT (Global Solution 2025)
 * Autor: [Seu Nome Aqui]
 * Placa: DOIT ESP32 DEVKIT V1
 * 
 * Descrição:
 * Este projeto integra sensores IoT para monitorar condições térmicas
 * em tempo real. A cada 10 segundos, o ESP32 envia via MQTT:
 * - ID do grupo SafeCap
 * - ID do dispositivo
 * - IP e MAC do ESP32
 * - Temperatura e umidade (DHT22)
 * - Valor do potenciômetro (simulando ajuste de sensibilidade)
 * 
 * SafeCap: Inovação e cuidado contra o calor extremo.
 ********************************************************************/

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define boardLED 2
#define DHTPIN 12
#define DHTTYPE DHT22
#define POTPIN 34

// Identificadores do projeto SafeCap
const char* ID        = "SafeCap_Grupo4";
const char* moduleID  = "SafeCap_ESP32_A01";

// Wi-Fi (rede do simulador Wokwi)
const char* SSID      = "Wokwi-GUEST";
const char* PASSWORD  = "";

// Broker MQTT da VM da FIAP
const char* BROKER_MQTT  = "172.208.54.189";
const int   BROKER_PORT  = 1883;
const char* mqttUser     = "gs2025";
const char* mqttPassword = "q1w2e3r4";
#define TOPICO_PUBLISH  "2TDS/esp32/teste"


WiFiClient espClient;
PubSubClient MQTT(espClient);
JsonDocument doc;
char buffer[256];
DHT dht(DHTPIN, DHTTYPE);

float temperatura;
float umidade;
int valorPot;

void initWiFi() {
  WiFi.begin(SSID, PASSWORD);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.print("IP: "); Serial.println(WiFi.localIP());
  Serial.print("MAC Address: "); Serial.println(WiFi.macAddress());
}

void reconectaWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconectando Wi-Fi...");
    initWiFi();
  }
}

void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  while (!MQTT.connected()) {
    Serial.println("Conectando ao Broker MQTT...");
    if (MQTT.connect(moduleID, mqttUser, mqttPassword)) {
      Serial.println("Conectado ao Broker!");
    } else {
      Serial.print("Falha na conexão. Estado: ");
      Serial.println(MQTT.state());
      delay(2000);
    }
  }
}

void verificaConexoesWiFiEMQTT() {
  reconectaWiFi();
  if (!MQTT.connected()) {
    initMQTT();
  }
  MQTT.loop();
}

void enviaEstadoOutputMQTT() {
  MQTT.publish(TOPICO_PUBLISH, buffer);
  Serial.println("Mensagem publicada com sucesso!");
}

void piscaLed() {
  digitalWrite(boardLED, HIGH);
  delay(300);
  digitalWrite(boardLED, LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(boardLED, OUTPUT);
  pinMode(POTPIN, INPUT);
  digitalWrite(boardLED, LOW);
  dht.begin();
  initWiFi();
  initMQTT();
}

void loop() {
  verificaConexoesWiFiEMQTT();

  temperatura = dht.readTemperature();
  umidade = dht.readHumidity();
  valorPot = analogRead(POTPIN);

  doc.clear();

  doc["ID"] = ID;
  doc["Sensor"] = moduleID;
  doc["IP"] = WiFi.localIP().toString();
  doc["MAC"] = WiFi.macAddress();

  if (!isnan(temperatura) && !isnan(umidade)) {
    doc["Temperatura"] = temperatura;
    doc["Umidade"] = umidade;
  } else {
    doc["Temperatura"] = "Erro";
    doc["Umidade"] = "Erro";
  }

  doc["Potenciometro"] = valorPot;

  serializeJson(doc, buffer);
  Serial.println(buffer);
  enviaEstadoOutputMQTT();
  piscaLed();
  delay(10000);
}
