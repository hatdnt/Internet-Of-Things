// Pin definitions
#define FLAME_SENSOR_PIN 32
#define BUZZER_PIN 26
#define RELAY_PIN 27
#define LED_PIN 4

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Set pin modes
  pinMode(FLAME_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // Initialize outputs
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RELAY_PIN, HIGH); // Set HIGH initially assuming active-low relay
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  int flameDetected = digitalRead(FLAME_SENSOR_PIN);

  // Check for flame detection
  if (flameDetected == LOW) { // Flame sensor gives LOW when flame is detected
    Serial.println("Flame detected!");

    // Turn on buzzer, relay, and LED
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(RELAY_PIN, LOW);  // Activate relay (assuming active-low)
    digitalWrite(LED_PIN, HIGH);
  } else {
    // Turn off buzzer, relay, and LED
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RELAY_PIN, HIGH); // Deactivate relay (assuming active-low)
    digitalWrite(LED_PIN, LOW);
  }

  delay(100); // Small delay to debounce sensor
}
