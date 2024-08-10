#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definisikan ukuran layar OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Alamat I2C dari layar OLED
#define OLED_I2C_ADDRESS 0x3C

// Buat objek display OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_I2C_ADDRESS);

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
  
  display.display();
  delay(2000); // Beri waktu untuk menampilkan pesan awal
  display.clearDisplay();

  // Tampilkan pesan awal di OLED
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("OLED Berhasil");
  display.display();
}

void loop() {
  
}


// VCC (OLED) ke 3.3V (ESP32)
// GND (OLED) ke GND (ESP32)
// SCL (OLED) ke GPIO 22 (ESP32)
// SDA (OLED) ke GPIO 21 (ESP32)
