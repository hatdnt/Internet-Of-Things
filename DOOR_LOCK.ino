#include <Keypad.h>
#include <ESP32Servo.h>

#define ROW_NUM     4  //rows
#define COLUMN_NUM  4  //columns

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
String correctCode = "1234";  // Atur Password

void setup() {
  Serial.begin(115200);

  myservo.attach(servoPin);

  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  myservo.write(0);  // Kunci Pintu
  digitalWrite(ledRedPin, HIGH);  // Red LED on (locked)
  digitalWrite(ledGreenPin, LOW); // Green LED off
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.println(key);
    if (key == '#') {  // '#' = Enter key
      if (inputCode == correctCode) {
        unlockDoor();
      } else {
        denyAccess();
      }
      inputCode = "";  // Reset input
    } else if (key == '*') {  // '*' = Clear key
      inputCode = "";
    } else {
      inputCode += key;
    }
  }
}

void unlockDoor() {
  Serial.println("Access Granted");
  digitalWrite(ledRedPin, LOW);
  digitalWrite(ledGreenPin, HIGH);
  myservo.write(90);  // Buka Pintu
  digitalWrite(buzzerPin, HIGH);
  delay(1000);
  digitalWrite(buzzerPin, LOW);
  delay(5000);  // Waktu pintu terbuka
  lockDoor();
}

void lockDoor() {
  myservo.write(0);  // Kunci Pintu
  digitalWrite(ledRedPin, HIGH);
  digitalWrite(ledGreenPin, LOW);
}

void denyAccess() {
  Serial.println("Access Denied");
  digitalWrite(buzzerPin, HIGH);
  delay(1000);
  digitalWrite(buzzerPin, LOW);
}