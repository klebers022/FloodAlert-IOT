/********************************************************************
 * Projeto: Flood Alert - Defesa Civil (Global Solution 2025)
 * Autor: [Seu Nome Aqui]
 * Placa: DOIT ESP32 DEVKIT V1
 * 
 * Descrição:
 * Este projeto monitora variáveis ambientais em tempo real.
 * O ESP32 envia dados via MQTT contendo:
 * - ID do Grupo e do Dispositivo
 * - IP e MAC do ESP32
 * - Temperatura e umidade (DHT22)
 * - Valor do potenciômetro (simulando nível da água)
 * 
 * Objetivo: Enviar dados para o backend da Defesa Civil
 * para alimentar alertas e mapas de risco no sistema Flood Alert.
 ********************************************************************/

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// -----------------------------
// Configurações de hardware
// -----------------------------
#define LED_ONBOARD 2
#define DHTPIN 12
#define DHTTYPE DHT22
#define POTPIN 34

// -----------------------------
// Identificação do dispositivo
// -----------------------------
const char* ID_GRUPO   = "FloodAlert_Grupo4";
const char* ID_MODULO  = "ESP32_FA01";

// -----------------------------
// Configuração Wi-Fi
// -----------------------------
const char* SSID       = "Wokwi-GUEST"; // Use Wokwi ou sua rede local
const char* PASSWORD   = "";

// -----------------------------
// Configuração Broker MQTT (FIAP ou local)
// -----------------------------
const char* BROKER_MQTT  = "172.208.54.189";  
const int   BROKER_PORT  = 1883;
const char* mqttUser     = "gs2025";
const char* mqttPassword = "q1w2e3r4";

#define TOPICO_PUBLISH "2TDS/FloodAlert/Dados" 

// -----------------------------
// Objetos globais
// -----------------------------
WiFiClient espClient;
PubSubClient MQTT(espClient);
DHT dht(DHTPIN, DHTTYPE);
JsonDocument doc;
char buffer[256];

float temperatura;
float umidade;
int nivelAgua;

// -----------------------------
// Funções Wi-Fi
// -----------------------------
void initWiFi() {
  WiFi.begin(SSID, PASSWORD);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Wi-Fi conectado!");
  Serial.print("IP: "); Serial.println(WiFi.localIP());
  Serial.print("MAC: "); Serial.println(WiFi.macAddress());
}

void reconectaWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconectando Wi-Fi...");
    initWiFi();
  }
}

// -----------------------------
// Funções MQTT
// -----------------------------
void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  while (!MQTT.connected()) {
    Serial.print("Conectando ao Broker MQTT...");
    if (MQTT.connect(ID_MODULO, mqttUser, mqttPassword)) {
      Serial.println(" ✅ Conectado!");
    } else {
      Serial.print("❌ Falha (Estado: ");
      Serial.print(MQTT.state());
      Serial.println("). Tentando novamente...");
      delay(2000);
    }
  }
}

void verificaConexoes() {
  reconectaWiFi();
  if (!MQTT.connected()) {
    initMQTT();
  }
  MQTT.loop();
}

// -----------------------------
// Função para envio de dados
// -----------------------------
void enviaDados() {
  serializeJson(doc, buffer);
  MQTT.publish(TOPICO_PUBLISH, buffer);
  Serial.println("📤 Dados enviados MQTT:");
  Serial.println(buffer);
}

// -----------------------------
// Setup
// -----------------------------
void setup() {
  Serial.begin(115200);
  pinMode(LED_ONBOARD, OUTPUT);
  dht.begin();
  initWiFi();
  initMQTT();
}

// -----------------------------
// Loop principal
// -----------------------------
void loop() {
  verificaConexoes();

  temperatura = dht.readTemperature();
  umidade = dht.readHumidity();
  nivelAgua = analogRead(POTPIN); // Simula nível de água com potenciômetro (0 a 4095)

  doc.clear();
  doc["ID_Grupo"] = ID_GRUPO;
  doc["ID_Modulo"] = ID_MODULO;
  doc["IP"] = WiFi.localIP().toString();
  doc["MAC"] = WiFi.macAddress();

  if (!isnan(temperatura) && !isnan(umidade)) {
    doc["Temperatura"] = temperatura;
    doc["Umidade"] = umidade;
  } else {
    doc["Temperatura"] = "Erro";
    doc["Umidade"] = "Erro";
  }

  doc["NivelAgua"] = nivelAgua;

  enviaDados();

  digitalWrite(LED_ONBOARD, HIGH);
  delay(300);
  digitalWrite(LED_ONBOARD, LOW);

  delay(10000); // Envia a cada 10 segundos
}
