// Mendefinisikan pin yang digunakan
const int button = 14;  // Pin untuk tombol
const int led1 = 12;    // Pin untuk LED pertama
const int led2 = 13;    // Pin untuk LED kedua

int buttonState = 0;      // Variabel untuk menyimpan status tombol saat ini
int lastButtonState = 0;  // Variabel untuk menyimpan status tombol terakhir
int pressCount = 0;       // Menghitung jumlah tekan tombol

void setup() {
  // Mengatur mode pin
  pinMode(button, INPUT_PULLDOWN); // Mengatur pin tombol sebagai input dengan resistor pull-down internal
  pinMode(led1, OUTPUT);          // Mengatur pin LED pertama sebagai output
  pinMode(led2, OUTPUT);          // Mengatur pin LED kedua sebagai output
  digitalWrite(led1, LOW);        // Memastikan LED pertama mati saat startup
  digitalWrite(led2, LOW);        // Memastikan LED kedua mati saat startup
}

void loop() {
  buttonState = digitalRead(button);  // Membaca status tombol (HIGH atau LOW)
  
  // Deteksi perubahan status tombol
  if (buttonState == HIGH && lastButtonState == LOW) { // Jika tombol ditekan (HIGH) dan sebelumnya tidak ditekan (LOW)
    delay(50); // Debounce: Tunggu sebentar untuk menghindari pembacaan yang salah dari bouncing tombol
    pressCount++; // Meningkatkan jumlah tekan tombol
    if (pressCount > 3) { // Jika jumlah tekan tombol lebih dari 3
      pressCount = 1; // Reset ke 1 setelah 3 tekan
    }
    
    // Mengatur LED berdasarkan jumlah tekan tombol
    switch (pressCount) {  // Pilih tindakan berdasarkan nilai pressCount
      case 1:
        digitalWrite(led1, HIGH); // Nyalakan LED pertama
        digitalWrite(led2, LOW);  // Matikan LED kedua
        break;
      case 2:
        digitalWrite(led1, LOW);  // Matikan LED pertama
        digitalWrite(led2, HIGH); // Nyalakan LED kedua
        break;
      case 3:
        digitalWrite(led1, LOW);  // Matikan LED pertama
        digitalWrite(led2, LOW);  // Matikan LED kedua
        break;
    }
    delay(500); // Tambahkan delay untuk memastikan perubahan tombol tidak terbaca terlalu cepat
  }
}
