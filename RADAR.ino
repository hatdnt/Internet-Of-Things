#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_I2C_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_I2C_ADDRESS);

#define TRIG_PIN 15
#define ECHO_PIN 13

const int servoPin = 4;

// Definisi servo dan lebar pulse
Servo myServo;
const int minPulseWidth = 500;  // 0.5 ms
const int maxPulseWidth = 2500; // 2.5 ms

// WiFi credentials
const char* ssid = "?";
const char* password = "1sampai8";

// WebSocket server
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Variabel untuk menyimpan alamat IP
String ipAddress;

// Fungsi untuk menangani pesan WebSocket dari klien
void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.println("Client connected");
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.println("Client disconnected");
  }
}

// Setup WiFi and WebSocket
void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  ipAddress = WiFi.localIP().toString();
  Serial.print("IP Address: ");
  Serial.println(ipAddress);

  // Pasang servo dan atur rentang lebar pulse
  myServo.attach(servoPin, minPulseWidth, maxPulseWidth);
  myServo.setPeriodHertz(50);
  
  // Initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println(F("Failed to initialize OLED"));
    for(;;);
  }

  display.display();
  delay(2000);
  display.clearDisplay();

  // Initialize HC-SR04 pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // WebSocket setup
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  // File HTML untuk tampilan radar
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Ultrasonic Radar</title>
    <style>
        body {
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            height: 100vh;
            margin: 0;
            background-color: black;
            color: green;
            font-family: Arial, sans-serif;
        }
        #radar {
            background-color: black;
            width: 100%;
            max-width: 500px;
            height: auto;
            border-radius: 50%;
            box-shadow: 0 0 20px green;
        }
        .info {
            margin-top: 20px;
            color: green;
        }
        .info span {
            margin-right: 20px;
        }
        @media (max-width: 768px) {
            #radar {
                max-width: 300px;
            }
        }
    </style>
</head>
<body>
    <canvas id="radar" width="500" height="500"></canvas>
    <div class="info">
        <span id="degrees">Degrees: 0</span>
        <span id="distance">Distance: 0</span>
    </div>

    <script>
        const canvas = document.getElementById('radar');
        const ctx = canvas.getContext('2d');
        const radarCenter = canvas.width / 2;
        const radarRadius = radarCenter;

        function drawRadar(angle, distance) {
            ctx.clearRect(0, 0, canvas.width, canvas.height);

            // Draw radar background circles
            ctx.strokeStyle = 'green';
            ctx.lineWidth = 2;
            for (let i = 1; i <= 4; i++) {
                ctx.beginPath();
                ctx.arc(radarCenter, radarCenter, radarRadius * i / 4, 0, 2 * Math.PI);
                ctx.stroke();
            }

            // Draw radar lines
            for (let i = 0; i < 360; i += 30) {
                const x = radarCenter + radarRadius * Math.cos(i * Math.PI / 180);
                const y = radarCenter + radarRadius * Math.sin(i * Math.PI / 180);
                ctx.beginPath();
                ctx.moveTo(radarCenter, radarCenter);
                ctx.lineTo(x, y);
                ctx.stroke();
            }

            // Draw radar sweep
            ctx.fillStyle = 'rgba(0, 255, 0, 0.2)';
            ctx.beginPath();
            ctx.moveTo(radarCenter, radarCenter);
            ctx.arc(radarCenter, radarCenter, radarRadius, (angle - 10) * Math.PI / 180, angle * Math.PI / 180);
            ctx.lineTo(radarCenter, radarCenter);
            ctx.fill();

            // Draw detected point
            const x = radarCenter + distance * radarRadius / 100 * Math.cos(angle * Math.PI / 180);
            const y = radarCenter + distance * radarRadius / 100 * Math.sin(angle * Math.PI / 180);
            ctx.fillStyle = 'red';
            ctx.beginPath();
            ctx.arc(x, y, 5, 0, 2 * Math.PI);
            ctx.fill();
        }

        // WebSocket connection
        const ws = new WebSocket(`ws://${window.location.host}/ws`);
        ws.onmessage = (event) => {
            const [angle, distance] = event.data.split(',');
            drawRadar(Number(angle), Number(distance));
            document.getElementById('degrees').innerText = `Degrees: ${angle}`;
            document.getElementById('distance').innerText = `Distance: ${Math.round(distance)}`;
        };
    </script>
</body>
</html>
    )rawliteral");
  });

  server.begin();
}

void loop() {
  for (int angle = 0; angle <= 180; angle++) {
    int pulseWidth = map(angle, 0, 180, minPulseWidth, maxPulseWidth);
    myServo.writeMicroseconds(pulseWidth);
    delay(15);

    long duration = getDistance();
    float distance = duration * 0.034 / 2;

    displayData(angle, distance);
    sendWebSocketData(angle, distance);
  }

  for (int angle = 180; angle >= 0; angle--) {
    int pulseWidth = map(angle, 0, 180, minPulseWidth, maxPulseWidth);
    myServo.writeMicroseconds(pulseWidth);
    delay(15);

    long duration = getDistance();
    float distance = duration * 0.034 / 2;

    displayData(angle, distance);
    sendWebSocketData(angle, distance);
  }

  ws.cleanupClients(); // Membersihkan koneksi WebSocket yang tidak aktif
}

void sendWebSocketData(int angle, float distance) {
  String message = String(angle) + "," + String(distance);
  ws.textAll(message); // Mengirim data ke semua klien yang terhubung
}

void displayData(int angle, float distance) {
  // Display on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("IP: ");
  display.print(ipAddress);
  display.setCursor(0, 16);
  display.print("Angle: ");
  display.print(angle);
  display.print(" deg");
  display.setCursor(0, 32);
  display.print("Distance: ");
  display.print(distance);
  display.print(" cm");
  display.display();
}

long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration;
}

// HC-SR04:
// VCC ke 5V (ESP32)
// GND ke GND (ESP32)
// Trig ke GPIO 15 (ESP32)
// Echo ke GPIO 13 (ESP32)

// SG90 Servo:
// Kawat kuning (sinyal) ke GPIO 4 (ESP32)
// Kawat merah (VCC) ke 5V (ESP32)
// Kawat coklat (GND) ke GND (ESP32)

// OLED 0.96 inch:
// VCC ke 3.3V (ESP32)
// GND ke GND (ESP32)
// SCL ke GPIO 22 (ESP32)
// SDA ke GPIO 21 (ESP32)