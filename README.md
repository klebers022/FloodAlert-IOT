
# 🌊 Flood Alert - IoT Monitoring System

## 🚨 Descrição
O **Flood Alert IoT** é uma solução desenvolvida para auxiliar na prevenção de enchentes, oferecendo monitoramento em tempo real de variáveis ambientais críticas, como:

- 🌡️ **Temperatura**
- 💧 **Umidade**
- 🚩 **Nível de Água**

Esses dados são capturados por sensores conectados a um **ESP32**, enviados via **MQTT** e exibidos em um **Dashboard Node-RED**, permitindo à Defesa Civil e órgãos responsáveis tomarem decisões rápidas e eficientes.

---

## 🛠️ Tecnologias Utilizadas
- 🔌 **ESP32 - DOIT DEVKIT V1**
- 📶 **Wi-Fi**
- 📡 **MQTT Broker** (FIAP ou local)
- 🔗 **Node-RED**
- 📊 **Node-RED Dashboard**
- 🌡️ **DHT22 (Temperatura e Umidade)**
- 🚰 **Simulador de nível de água (Potenciômetro)**

---

## ⚙️ Funcionamento

### 📍 Dados capturados:
- **ID do grupo**
- **ID do dispositivo**
- **IP e MAC do ESP32**
- **Temperatura (°C)**
- **Umidade (%)**
- **Nível de água (%)**

### 🔄 Fluxo dos dados:
1. O ESP32 coleta os dados dos sensores.
2. Envia via MQTT no tópico:  
   ```bash
   2TDS/FloodAlert/Dados
   ```
3. O Node-RED recebe os dados, realiza o parsing JSON e exibe no dashboard.
4. O dashboard exibe indicadores em tempo real e pode gerar alertas.

---

## 🚀 Instalação e Execução

### 1️⃣ No ESP32 (Plataforma: Wokwi ou PlatformIO)
- Configurar:
  - `SSID`: `Wokwi-GUEST` ou sua rede local
  - `BROKER_MQTT`: IP do broker (FIAP ou local)
- Fazer upload do código para o ESP32.

### 2️⃣ No Node-RED
- Importar o fluxo JSON fornecido no repositório.
- Verificar se o broker MQTT está conectado corretamente.
- Acessar o dashboard em:  
  ```bash
  http://localhost:1880/ui
  ```

---

## 📦 Estrutura do Projeto

```
📁 FloodAlert-IoT
├── 📄 main.ino          → Código do ESP32
├── 📄 fluxo-node-red.json → Fluxo do Node-RED
├── 📄 README.md         → Documentação
└── 📄 imagens/          → Prints do dashboard e fluxos
```

---

## 📊 Exemplo de Payload Enviado:

```json
{
  "ID_Grupo": "FloodAlert_Grupo4",
  "ID_Modulo": "ESP32_FA01",
  "IP": "192.168.x.x",
  "MAC": "24:0A:C4:00:01:10",
  "Temperatura": 24,
  "Umidade": 40,
  "NivelAgua": 60
}
```


---

## 👨‍💻 Desenvolvido por
- Kleber da Silva
- Lucas Rainha 
- Nicolas Barruti
**FIAP | Global Solution 2025 - Flood Alert IoT**

---

