const int button = 2;
const int led = 16;
const int led2 = 4;

int statusButton = 0;

void setup() {
  pinMode(button, INPUT);
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
}

void loop() {
  statusButton = digitalRead(button);

  if (statusButton == HIGH) {
    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);
    digitalWrite(led2, HIGH);
    delay(500);
    digitalWrite(led2, LOW);
  }

  else {
    digitalWrite(led, LOW);
  }

}
