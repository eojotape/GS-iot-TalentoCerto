#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "DHTesp.h"

const char* ssid = "Wokwi-GUEST";
const char* password = "";
String serverUrl = "http://pedroesp.loca.lt/dados";

const int DHT_PIN = 15;
const int LDR_PIN = 34;

const int RED_PIN = 12;
const int GREEN_PIN = 13;
const int BLUE_PIN = 14;

DHTesp dht;

void setup() {
  Serial.begin(115200);
  dht.setup(DHT_PIN, DHTesp::DHT22);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);

  conectarWiFi();
}

void conectarWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password);

  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 20) {
    setColor(true, true, true); delay(100);
    setColor(false, false, false); delay(400);
    Serial.print(".");
    tentativas++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✓ WiFi Conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n✗ Falha ao conectar WiFi");
  }
}

void setColor(bool r, bool g, bool b) {
  digitalWrite(RED_PIN, r ? HIGH : LOW);
  digitalWrite(GREEN_PIN, g ? HIGH : LOW);
  digitalWrite(BLUE_PIN, b ? HIGH : LOW);
}

void enviarDados(float temp, float umi, int luz) {
  if (WiFi.status() != WL_CONNECTED) {
    conectarWiFi();
    return;
  }

  HTTPClient http;

  String url = serverUrl +
               "?temp=" + String(temp, 1) +
               "&umi=" + String(umi, 1) +
               "&luz=" + String(luz);

  Serial.println("[HTTP] URL: " + url);

  if (!http.begin(url)) {
    Serial.println("[HTTP] http.begin() falhou");
    return;
  }

  http.addHeader("Bypass-Tunnel-Reminder", "go");

  int httpCode = http.GET();

  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... código: %d\n", httpCode);
    String payload = http.getString();
    Serial.println("[HTTP] Resposta: " + payload);
  } else {
    Serial.printf("[HTTP] GET falhou, código: %d, erro: %s\n",
                  httpCode,
                  http.errorToString(httpCode).c_str());
  }

  http.end();
}

void loop() {
  TempAndHumidity data = dht.getTempAndHumidity();
  float temp = data.temperature;
  float umi = data.humidity;
  int leituraLuz = analogRead(LDR_PIN); 
  
  if (isnan(temp) || isnan(umi)) {
    Serial.println("✗ Erro ao ler sensor DHT!");
    setColor(true, true, false);
    delay(2000);
    return;
  }

  Serial.println("\n========== LEITURAS ==========");
  Serial.println("Temperatura: " + String(temp, 1) + "°C");
  Serial.println("Umidade: " + String(umi, 1) + "%");
  Serial.println("Luz: " + String(leituraLuz) + " (0-4095)");
  Serial.println("==============================");

  if (temp >= 30.0) {
    Serial.println("⚠ ALERTA: Calor Extremo!");
    setColor(true, false, false);

  } else if (leituraLuz < 1500) { 
    Serial.println("⚠ ALERTA: Sala Escura!");
    setColor(true, true, true);
  
  } else if (umi >= 70.0) {
    Serial.println("⚠ ALERTA: Umidade Alta!");
    setColor(false, false, true);

  } else {
    Serial.println("✓ Ambiente Ideal");
    setColor(false, true, false);
  }

  enviarDados(temp, umi, leituraLuz);
  
  Serial.println("\nAguardando próxima leitura...\n");
  delay(5000);
}
