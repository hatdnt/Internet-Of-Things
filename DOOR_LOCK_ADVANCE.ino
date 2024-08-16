#include <Keypad.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definisikan ukuran layar OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_I2C_ADDRESS 0x3C

// Buat objek display OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_I2C_ADDRESS);

// Definisikan ukuran keypad
#define ROW_NUM     4  // Jumlah baris
#define COLUMN_NUM  4  // Jumlah kolom

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pin_rows[ROW_NUM] = {32, 33, 25, 26};
byte pin_column[COLUMN_NUM] = {27, 14, 12, 13};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);
Servo myservo;

// Pin
const int servoPin = 4;
const int buzzerPin = 16;
const int ledRedPin = 17;
const int ledGreenPin = 18;

String inputCode = "";
String correctCode = "1234";  // Sandi awal
bool changePasswordMode = false;
int passwordChangeStep = 0; // 0: Enter old password, 1: Enter new password, 2: Confirm new password
String newPassword = "";

void setup() {
  Serial.begin(115200);

  myservo.attach(servoPin);

  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  myservo.write(0);  // Kunci Pintu
  digitalWrite(ledRedPin, HIGH);  // Red LED menyala (terkunci)
  digitalWrite(ledGreenPin, LOW); // Green LED mati
  
  // Inisialisasi OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println(F("Gagal menginisialisasi OLED"));
    for(;;);  // Hentikan eksekusi jika OLED gagal diinisialisasi
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Masukkan Kode:");
  display.display();
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.println(key);

    if (changePasswordMode) {
      handlePasswordChange(key);
    } else {
      handleCodeEntry(key);
    }
  }
}

void handleCodeEntry(char key) {
  if (key == '#') {  // '#' = Tombol Enter
    if (inputCode == correctCode) {
      unlockDoor();
    } else {
      denyAccess();
    }
    inputCode = "";  // Reset input
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Masukkan Kode:");
    display.display();
  } else if (key == '*') {  // '*' = Tombol Clear
    inputCode = "";
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Masukkan Kode:");
    display.display();
  } else if (key == 'D') {  // 'D' = Tombol Ganti Sandi
    changePasswordMode = true;
    passwordChangeStep = 0;
    inputCode = "";
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Masukkan Sandi Lama:");
    display.display();
  } else {
    inputCode += key;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Kode: ");
    display.print(inputCode);
    display.display();
  }
}

void handlePasswordChange(char key) {
  if (key == '#') {  // '#' = Tombol Enter
    if (passwordChangeStep == 0) { // Check old password
      if (inputCode == correctCode) {
        passwordChangeStep = 1;
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Masukkan Sandi Baru:");
        display.display();
      } else {
        denyAccess();
        changePasswordMode = false;
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Sandi Lama Salah");
        display.display();
        delay(2000);
        resetToCodeEntry();
      }
      inputCode = "";  // Reset input
    } else if (passwordChangeStep == 1) { // Enter new password
      newPassword = inputCode;
      passwordChangeStep = 2;
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Konfirmasi Sandi:");
      display.display();
      inputCode = "";  // Reset input
    } else if (passwordChangeStep == 2) { // Confirm new password
      if (inputCode == newPassword) {
        correctCode = newPassword;
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Sandi Berhasil Diganti!");
        display.display();
        delay(2000);
      } else {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Konfirmasi Gagal!");
        display.display();
        delay(2000);
      }
      resetToCodeEntry();
    }
  } else if (key == '*') {  // '*' = Tombol Clear
    inputCode = "";
    display.clearDisplay();
    display.setCursor(0, 0);
    if (passwordChangeStep == 0) {
      display.print("Masukkan Sandi Lama:");
    } else if (passwordChangeStep == 1) {
      display.print("Masukkan Sandi Baru:");
    } else {
      display.print("Konfirmasi Sandi:");
    }
    display.display();
  } else {
    inputCode += key;
    display.clearDisplay();
    display.setCursor(0, 0);
    if (passwordChangeStep == 0) {
      display.print("Sandi Lama: ");
    } else if (passwordChangeStep == 1) {
      display.print("Sandi Baru: ");
    } else {
      display.print("Konfirmasi Sandi: ");
    }
    display.print(inputCode);
    display.display();
  }
}

void resetToCodeEntry() {
  changePasswordMode = false;
  inputCode = "";
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Masukkan Kode:");
  display.display();
}

void unlockDoor() {
  Serial.println("Akses Diterima");
  digitalWrite(ledRedPin, LOW);
  digitalWrite(ledGreenPin, HIGH);
  myservo.write(90);  // Buka Pintu
  digitalWrite(buzzerPin, HIGH);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Pintu Terbuka");
  display.display();
  delay(1000);
  digitalWrite(buzzerPin, LOW);
  delay(5000);  // Waktu pintu terbuka
  lockDoor();
}

void lockDoor() {
  myservo.write(0);  // Kunci Pintu
  digitalWrite(ledRedPin, HIGH);
  digitalWrite(ledGreenPin, LOW);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Pintu Terkunci");
  display.display();
}

void denyAccess() {
  Serial.println("Akses Ditolak");
  digitalWrite(buzzerPin, HIGH);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Akses Ditolak");
  display.display();
  delay(1000);
  digitalWrite(buzzerPin, LOW);
}