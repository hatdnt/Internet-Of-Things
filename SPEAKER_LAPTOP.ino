int speakerPin = 16; // GPIO yang terhubung ke basis transistor

void setup() {
  pinMode(speakerPin, OUTPUT);
}

void loop() {
  // Mainkan nada dering sederhana
  playMelody();
  
  delay(5000); // Tunggu 5 detik sebelum mengulang nada dering
}

void playMelody() {
  // Nada dering "do-re-mi"
  tone(speakerPin, 262, 500); // C4 (do) 500ms
  delay(500);
  tone(speakerPin, 294, 500); // D4 (re) 500ms
  delay(500);
  tone(speakerPin, 330, 500); // E4 (mi) 500ms
  delay(500);
  
  // Jeda
  noTone(speakerPin);
  delay(500);
  
  // Nada dering "mi-re-do"
  tone(speakerPin, 330, 500); // E4 (mi) 500ms
  delay(500);
  tone(speakerPin, 294, 500); // D4 (re) 500ms
  delay(500);
  tone(speakerPin, 262, 500); // C4 (do) 500ms
  delay(500);
  
  // Akhiri dengan nada tinggi
  tone(speakerPin, 523, 1000); // C5 (do tinggi) 1 detik
  delay(1000);
  
  noTone(speakerPin); // Hentikan nada
}


// Kolektor Transistor ke kabel merah speaker.
// Emitter Transistor ke GND.
// kabel hitam speaker ke pin 3.3V di ESP32.
// Basis Transistor ke salah satu GPIO ESP32 melalui resistor 10kΩ.
// Dioda 1A 100V antara kolektor transistor dan VCC (3.3V), dengan anoda pada kolektor dan katoda pada VCC.
