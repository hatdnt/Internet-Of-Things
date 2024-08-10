#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "?";
const char* password = "1sampai8";

const char* botToken = "6381821744:AAFTfBD68UJbQk-EFP0u5KXK2mZxraucjtk";
const char* chatId = "1613025158";

const int pirPin = 14;
const int ledPin = 12;
const int buzzerPin = 13;

void sendTelegramMessage(String message) {
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.telegram.org/bot" + String(botToken) + "/sendMessage?chat_id=" + String(chatId) + "&text=" + message;
    http.begin(url);
    int httpResponseCode = http.GET();
    if(httpResponseCode > 0) {
      Serial.printf("Telegram message sent successfully, response code: %d\n", httpResponseCode);
    } else {
      Serial.printf("Error sending message: %s\n", http.errorToString(httpResponseCode).c_str());
    }
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(pirPin, INPUT_PULLDOWN);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
}

void loop() {
  int pirState = digitalRead(pirPin);

  if (pirState == HIGH) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    Serial.println("Gerakan terdeteksi!");
    delay(500);
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
    sendTelegramMessage("Gerakan terdeteksi!");
  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
    Serial.println("Tidak ada gerakan.");
  }
  
  delay(100);
}


// OUT (PIR) dihubungkan ke GPIO 14
// VCC (PIR) dihubungkan ke 3.3V
// GND (PIR) dihubungkan ke GND
// Anoda (LED) dihubungkan ke GPIO 12 (melalui resistor 220Ω)
// Katoda (LED) dihubungkan ke GND
// Positif (BUZZER) dihubungkan ke GPIO 13
// Negatif (BUZZER) dihubungkan ke GND

