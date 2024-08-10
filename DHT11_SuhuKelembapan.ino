#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

// Definisikan ukuran layar OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Alamat I2C dari layar OLED
#define OLED_I2C_ADDRESS 0x3C

// Pin dan tipe sensor DHT
#define DHTPIN 4
#define DHTTYPE DHT11

// Buat objek display OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_I2C_ADDRESS);

// Buat objek sensor DHT
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(115200);
  Serial.println("Memulai program...");

  // Inisialisasi layar OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println(F("Gagal menginisialisasi OLED"));
    for(;;);  // Hentikan eksekusi jika OLED gagal diinisialisasi
  }
  
  Serial.println("OLED berhasil diinisialisasi.");
  
  // Bersihkan layar OLED
  display.display();
  display.clearDisplay();
  
  // Inisialisasi sensor DHT
  dht.begin();
}

void loop() {
  // Baca suhu dan kelembapan dari DHT11
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Periksa apakah pembacaan berhasil
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println(F("Gagal membaca dari sensor DHT11!"));
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Sensor Error");
  } else {
    // Tampilkan suhu dan kelembapan di Serial Monitor
    Serial.print(F("Suhu: "));
    Serial.print(temperature);
    Serial.print(F(" °C  "));
    Serial.print(F("Kelembapan: "));
    Serial.print(humidity);
    Serial.println(F(" %"));

    // Tampilkan suhu dan kelembapan di OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Suhu: ");
    display.print(temperature);
    display.print(" C");
    
    display.setCursor(0, 16);
    display.print("Kelembapan: ");
    display.print(humidity);
    display.print(" %");
  }

  // Perbarui tampilan OLED
  display.display();

  // Delay sebelum pembacaan berikutnya
  delay(2000);
}



// VCC (OLED) ke 3.3V (ESP32)
// GND (OLED) ke GND (ESP32)
// SCL (OLED) ke GPIO 22 (ESP32)
// SDA (OLED) ke GPIO 21 (ESP32)
// VCC (DHT11) -> 3.3V atau 5V
// GND (DHT11) -> GND
// Data (DHT11) -> GPIO 4 (atau sesuaikan dengan pin yang Anda gunakan)
// Pull-up resistor 10kΩ (jika diperlukan) antara VCC dan Data.
