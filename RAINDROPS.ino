#include <ESP32Servo.h>

const int rainSensorPin = 4; // Pin sensor hujan digital
const int servoPin = 15;     // Pin servo

Servo myServo;

void setup() {
  pinMode(rainSensorPin, INPUT);
  myServo.attach(servoPin);
  myServo.write(0); // Posisi awal servo (0 derajat)
  Serial.begin(115200);
  Serial.println("Program dimulai");
}

void loop() {
  int sensorValue = digitalRead(rainSensorPin); // Baca nilai dari sensor hujan

  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);

  if (sensorValue == LOW) {
    // Jika sensor mendeteksi hujan (nilai LOW)
    Serial.println("Hujan terdeteksi");
    myServo.write(90); // Putar servo ke 90 derajat
  } else {
    // Jika sensor tidak mendeteksi hujan
    Serial.println("Tidak ada hujan");
    myServo.write(0);  // Kembali ke posisi awal
  }

  delay(1000); // Tunggu 1 detik sebelum membaca nilai lagi
}


// Raindrop Sensor MH-RD:
// VCC → 3.3V (ESP32)
// GND → GND (ESP32)
// DO (Digital Output) → GPIO 4 (ESP32)

// Servo SG90:
// VCC → 5V (Jika tidak tersedia 5V pada ESP32, gunakan sumber eksternal dengan GND bersama ESP32)
// GND → GND (ESP32)
// Signal → GPIO 15 (ESP32)