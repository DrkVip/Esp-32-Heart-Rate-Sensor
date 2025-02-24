#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include "MAX30105.h"

// WiFi credentials
const char* ssid = "OnePlus Nord CE 4";
const char* password = "2444666668888888";

// MAX30105 sensor object
MAX30105 sensor;

// Web server object
WebServer server(80);

// Variables to store heart rate and SpO2 (placeholder values)
float heart_rate = 0.0; // Global variable for heart rate
float spo2 = 0.0;      // Global variable for SpO2

void handleData(); // Function declaration
void handleRoot();
void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // WiFi setup
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Web server setup
  server.on("/", handleRoot);
  server.on("/data", handleData); // Register the /data endpoint
  server.begin();
  Serial.println("Web server started!");

  // Sensor setup
  if (sensor.begin(Wire, I2C_SPEED_STANDARD)) {
    sensor.setup();                       // Load default settings
    sensor.setPulseAmplitudeRed(0x1F);    // Set IR pulse amplitude
    sensor.setPulseAmplitudeIR(0x1F);     // Set Red pulse amplitude
    sensor.setPulseAmplitudeGreen(0);     // Disable green LED
    Serial.println("Sensor initialized.");
  } else {
    Serial.println("Sensor not found. Halting.");
    while (1);
  }
}

void loop() {
  // Handle web server requests
  server.handleClient();

  // Check if new data is available from the sensor
  if (sensor.check() == true) {
    float current_value_red = sensor.getRed();
    float current_value_ir = sensor.getIR();

    // Process sensor data (you can implement further calculations for heart rate and SpO2)
    processSensorData(current_value_red, current_value_ir);
  }
}

// Function to process sensor data (replace with actual algorithms)
void processSensorData(float red, float ir) {
  // Example placeholder logic
  heart_rate = red / 1000.0;  // Simulated heart rate calculation
  spo2 = ir / 1000.0;         // Simulated SpO2 calculation

  Serial.print("Red: ");
  Serial.print(red);
  Serial.print(", IR: ");
  Serial.print(ir);
  Serial.print(", Heart Rate: ");
  Serial.print(heart_rate);
  Serial.print(", SpO2: ");
  Serial.println(spo2);
}

// Handle root web request
void handleRoot() {
    String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
        <title>MAX30105 Data Visualization</title>
        <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
        <style>
            canvas {
                width: 40%;
                margin: 20px auto;
            }
            body {
                font-weight: 100;
                font-family: 'Brush Script MT', cursive;
                text-align: center;
                margin: 0;
                padding: 0;
            }
            .data-display {
                margin: 20px;
                font-size: 1.2em;
                font-weight: bold;
            }
        </style>
    </head>
    <body>
        <h1>Heart Rate Sensor</h1>
        <canvas id="heartRateChart"></canvas>
        <canvas id="spo2Chart"></canvas>
        <div class="data-display">
            <p>Heart Rate (BPM): <span id="heartRateValue">--</span></p>
            <p>SpO2 (%): <span id="spo2Value">--</span></p>
        </div>
        <script>
            // Initialize charts
            const heartRateCtx = document.getElementById('heartRateChart').getContext('2d');
            const spo2Ctx = document.getElementById('spo2Chart').getContext('2d');

            const heartRateChart = new Chart(heartRateCtx, {
                type: 'line',
                data: {
                    labels: [], // Time labels
                    datasets: [{
                        label: 'Heart Rate (BPM)',
                        data: [],
                        borderColor: 'red',
                        fill: false,
                        tension: 0.1
                    }]
                },
                options: {
                    scales: {
                        x: { title: { display: true, text: 'Time (s)' } },
                        y: { title: { display: true, text: 'Heart Rate (BPM)' } }
                    }
                }
            });

            const spo2Chart = new Chart(spo2Ctx, {
                type: 'line',
                data: {
                    labels: [], // Time labels
                    datasets: [{
                        label: 'SpO2 (%)',
                        data: [],
                        borderColor: 'blue',
                        fill: false,
                        tension: 0.1
                    }]
                },
                options: {
                    scales: {
                        x: { title: { display: true, text: 'Time (s)' } },
                        y: { title: { display: true, text: 'SpO2 (%)' } }
                    }
                }
            });

            // Real-time data variables
            let timeCounter = 0;
            const heartRateValue = document.getElementById('heartRateValue');
            const spo2Value = document.getElementById('spo2Value');

            // Fetch and update data every second
            setInterval(() => {
                fetch('/data')
                    .then(response => response.json())
                    .then(data => {
                        // Update charts
                        heartRateChart.data.labels.push(timeCounter);
                        heartRateChart.data.datasets[0].data.push(data.heart_rate);
                        if (heartRateChart.data.labels.length > 20) {
                            heartRateChart.data.labels.shift();
                            heartRateChart.data.datasets[0].data.shift();
                        }
                        heartRateChart.update();

                        spo2Chart.data.labels.push(timeCounter);
                        spo2Chart.data.datasets[0].data.push(data.spo2);
                        if (spo2Chart.data.labels.length > 20) {
                            spo2Chart.data.labels.shift();
                            spo2Chart.data.datasets[0].data.shift();
                        }
                        spo2Chart.update();

                        // Update displayed values
                        heartRateValue.textContent = data.heart_rate.toFixed(2);
                        spo2Value.textContent = data.spo2.toFixed(2);

                        // Increment time counter
                        timeCounter++;
                    })
                    .catch(err => console.error('Error fetching data:', err));
            }, 1000);
        </script>
    </body>
    </html>
    )rawliteral";

    server.send(200, "text/html", html);
}
// Handle data web request
void handleData() {
    String json = "{";
    json += "\"heart_rate\": " + String(heart_rate) + ",";
    json += "\"spo2\": " + String(spo2);
    json += "}";

    server.send(200, "application/json", json);
}

