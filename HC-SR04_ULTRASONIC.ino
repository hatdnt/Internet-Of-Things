#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_I2C_ADDRESS 0x3C

#define TRIGGER_PIN 15
#define ECHO_PIN 13
#define MAX_DISTANCE 200 // Maksimum jarak yang dapat diukur dalam cm

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  Serial.println("Memulai program...");

  // Inisialisasi layar OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println(F("Gagal menginisialisasi OLED"));
    for(;;);  // Hentikan eksekusi jika OLED gagal diinisialisasi
  }
  
  Serial.println("OLED berhasil diinisialisasi.");
  
  display.display();
  delay(2000); // Beri waktu untuk menampilkan pesan awal
  display.clearDisplay();

  // Tampilkan pesan awal di OLED
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("OLED Berhasil");
  display.display();

  // Inisialisasi pin HC-SR04
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIGGER_PIN, LOW);
}

void loop() {
  long duration, distance;

  // Mengirimkan sinyal trigger
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Mengukur durasi sinyal echo
  duration = pulseIn(ECHO_PIN, HIGH);

  if (duration == 0) {
    Serial.println("No echo received");
    distance = 0;
  } else {
    // Menghitung jarak
    distance = (duration / 2) / 29.1; // Menghitung jarak dalam cm
  }

  // Tampilkan hasil di OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  if (distance > MAX_DISTANCE || distance <= 0) {
    display.println("Out of range");
  } else {
    display.print("Distance: ");
    display.print(distance);
    display.println(" cm");
  }
  display.display();

  delay(1000); // Menunggu 1 detik sebelum pengukuran berikutnya
}


// HC-SR04:
// VCC ke 5V
// GND ke GND
// Trig ke GPIO 15 (ESP32)
// Echo ke GPIO 13 (ESP32)

// VCC (OLED) ke 3.3V (ESP32)
// GND (OLED) ke GND (ESP32)
// SCL (OLED) ke GPIO 22 (ESP32)
// SDA (OLED) ke GPIO 21 (ESP32)
