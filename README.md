# 🎯 TalentoCerto - Monitoramento Inteligente para Ambientes de Estudo

> Um sistema IoT baseado em ESP32 para garantir a ergonomia e produtividade em home office e salas de estudo.

![Status do Projeto](https://img.shields.io/badge/Status-Finalizado-green)
![Tech](https://img.shields.io/badge/Tech-ESP32%20|%20NodeRED%20|%20IoT-blue)

## 📺 Demonstração do Projeto

Confira o funcionamento do TalentoCerto na prática, com a explicação do circuito e do dashboard:

[![Assista ao Vídeo](https://img.youtube.com/vi/ID_DO_SEU_VIDEO_AQUI/maxresdefault.jpg)](https://youtu.be/ID_DO_SEU_VIDEO_AQUI)

*(Clique na imagem acima para assistir ao vídeo)*

---

## 📖 Sobre o Projeto

No contexto do "Futuro do Trabalho", a qualidade do ambiente físico impacta diretamente a produtividade e a saúde. Ambientes muito quentes, úmidos ou escuros causam fadiga, problemas respiratórios e falta de foco.

O **TalentoCerto** é uma estação de monitoramento que coleta dados ambientais em tempo real e fornece feedback visual imediato para o usuário tomar ações corretivas, além de registrar históricos via dashboard web.

## 🚀 Funcionalidades

* **Monitoramento Térmico:** Alerta de calor excessivo (DHT22).
* **Monitoramento de Umidade:** Prevenção de mofo e ar "abafado" (DHT22).
* **Monitoramento de Luz:** Detecção de baixa luminosidade para leitura (Módulo LDR).
* **Feedback Visual Imediato:** LED RGB com código de cores prioritário.
* **Dashboard Web:** Visualização de ponteiros (Gauges) em tempo real via Node-RED.
* **Conectividade:** Comunicação HTTP via WiFi e tunelamento para acesso remoto.

## 🛠️ Hardware Utilizado

* **Microcontrolador:** ESP32 DevKit V1
* **Sensores:**
    * DHT22 (Temperatura e Umidade)
    * Módulo LDR (Sensor de Luz - Saída Analógica)
* **Atuadores:**
    * LED RGB (Cátodo Comum)
* **Outros:**
    * Resistores, Jumpers e Protoboard.

## 🚦 Lógica de Alerta (Prioridades)

O sistema utiliza uma hierarquia de prioridades para decidir a cor do LED, garantindo que o problema mais crítico seja atendido primeiro:

| Prioridade | Condição | Cor do LED | Significado |
| :--- | :--- | :--- | :--- |
| **1 (Crítica)** | Temp ≥ 30°C | 🔴 **Vermelho** | Calor excessivo. Risco de fadiga. |
| **2 (Média)** | Luz < Limiar | ⚪ **Branco** | Sala escura. Risco à visão. |
| **3 (Média)** | Umidade ≥ 70% | 🔵 **Azul** | Umidade alta. Risco de mofo/eletrônicos. |
| **4 (Ideal)** | Nenhuma das acima | 🟢 **Verde** | Ambiente confortável e produtivo. |

## 💻 Software e Tecnologias

### 1. Firmware (ESP32)
Desenvolvido em C++ (Arduino Framework).
* **Bibliotecas:** `WiFi.h`, `HTTPClient.h`, `DHTesp.h`.

### 2. Backend & Dashboard (Node-RED)
Utilizamos o **Node-RED** para receber as requisições HTTP, processar os dados e exibi-los em um Dashboard gráfico.
* **Fluxo:** Recebe GET Request -> Filtra Dados -> Atualiza Gauges.

### 3. Tunelamento (Localtunnel)
Para expor o servidor local (Node-RED) para a internet (Wokwi/ESP32), utilizamos o `localtunnel`.

## 📸 Screenshots

### Circuito (Simulação Wokwi)
![Circuito Wokwi](./caminho-para-imagem-do-circuito.png)
*(Substitua esta linha por um print do seu circuito)*

### Dashboard (Node-RED)
![Dashboard](./caminho-para-imagem-do-dashboard.png)
*(Substitua esta linha por um print dos seus gráficos)*

## ⚙️ Como Rodar o Projeto

### Pré-requisitos
* Node.js instalado.
* Node-RED instalado (`npm install -g node-red`).
* Placa ESP32 ou Simulador Wokwi.

### Passo 1: Node-RED
1.  Inicie o Node-RED no terminal: `node-red`.
2.  Acesse `http://localhost:1880`.
3.  Importe o arquivo `flows.json` (disponível neste repositório).
4.  Clique em **Deploy**.

### Passo 2: Criar o Túnel
No terminal, execute o comando para expor a porta do Node-RED:
```bash
npx localtunnel --port 1880 --subdomain talentocerto-iot
```
Copie a URL gerada.

### Passo 3: Configurar o ESP32
Abra o código do ESP32.

Atualize a variável serverUrl com a URL gerada pelo localtunnel.

Faça o upload para a placa ou inicie a simulação.
