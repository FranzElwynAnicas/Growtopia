esp32#include <WiFi.h>
#include <WebServer.h>

#define SENSOR_PIN 33
#define PUMP_PIN 26

// Replace with your WiFi credentials
const char* ssid = "franz_iot";
const char* password = "ranznetpogi";

// Create web server on port 80
WebServer server(80);

int moisture = 0;
bool pumpRunning = false;

// ESP32 ADC 0-4095 scale thresholds
int DRY_THRESHOLD = 800;       // dry soil → pump ON
int MODERATE_THRESHOLD = 1600; // moderately moist
int MOIST_THRESHOLD = 2400;    // moist
int WET_THRESHOLD = 3200;      // quite wet

int MIN_WATER_TIME = 5000; // minimum watering time in milliseconds (5 seconds)

void setup() {
  Serial.begin(115200);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);  // pump OFF at start
  
  Serial.println("🌱 Plant Watering System Started");
  Serial.println("=================================");
  
  // Connect to WiFi
  connectToWiFi();
  
  // Setup web server routes
  server.on("/", handleRoot);           // Main dashboard
  server.on("/data", handleData);       // JSON data for AJAX
  server.on("/water", handleWater);     // Manual watering
  server.on("/stop", handleStop);       // Stop watering
  
  server.begin();
  Serial.println("✅ Web server started!");
  Serial.println("📱 Open http://" + WiFi.localIP().toString() + " in your browser");
  Serial.println("---------------------------------");
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi Connected!");
    Serial.print("📡 IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ WiFi Connection Failed!");
  }
}

String getMoistureLevel(int moisture) {
  if (moisture <= DRY_THRESHOLD) return "DRY";
  else if (moisture <= MODERATE_THRESHOLD) return "MODERATE";
  else if (moisture <= MOIST_THRESHOLD) return "MOIST";
  else if (moisture <= WET_THRESHOLD) return "WET";
  else return "SATURATED";
}

String getMoistureIcon(int moisture) {
  if (moisture <= DRY_THRESHOLD) return "🟤";
  else if (moisture <= MODERATE_THRESHOLD) return "🟡";
  else if (moisture <= MOIST_THRESHOLD) return "🟢";
  else if (moisture <= WET_THRESHOLD) return "🔵";
  else return "💧";
}

String getMoistureColor(int moisture) {
  if (moisture <= DRY_THRESHOLD) return "#ff4444";
  else if (moisture <= MODERATE_THRESHOLD) return "#ffaa00";
  else if (moisture <= MOIST_THRESHOLD) return "#44ff44";
  else if (moisture <= WET_THRESHOLD) return "#4444ff";
  else return "#8888ff";
}

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Plant Monitoring</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { 
      font-family: Arial, sans-serif; 
      text-align: center; 
      background: #f5f5f5;
      margin: 0;
      padding: 20px;
    }
    .container {
      max-width: 600px;
      margin: 0 auto;
    }
    .card {
      background: white;
      padding: 20px;
      margin: 15px 0;
      border-radius: 15px;
      box-shadow: 0 4px 6px rgba(0,0,0,0.1);
    }
    .moisture-value {
      font-size: 3em;
      font-weight: bold;
      margin: 10px 0;
    }
    .moisture-level {
      font-size: 1.5em;
      margin: 10px 0;
    }
    .pump-status {
      font-size: 1.2em;
      padding: 10px;
      border-radius: 10px;
      margin: 10px 0;
    }
    .pump-on {
      background: #ff4444;
      color: white;
    }
    .pump-off {
      background: #44ff44;
      color: white;
    }
    button {
      background: #4CAF50;
      border: none;
      color: white;
      padding: 15px 32px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 16px;
      margin: 10px 5px;
      cursor: pointer;
      border-radius: 8px;
    }
    button:disabled {
      background: #cccccc;
      cursor: not-allowed;
    }
    .last-update {
      color: #666;
      font-size: 0.9em;
      margin-top: 20px;
    }
    .progress-bar {
      width: 100%;
      height: 30px;
      background: #e0e0e0;
      border-radius: 15px;
      margin: 20px 0;
      overflow: hidden;
    }
    .progress-fill {
      height: 100%;
      transition: width 0.5s ease;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>🌱 Plant Monitoring Dashboard</h1>
    
    <div class="card">
      <h2>Soil Moisture</h2>
      <div class="moisture-value" id="moistureValue">--</div>
      <div class="progress-bar">
        <div class="progress-fill" id="moistureBar" style="width: 0%; background: #666;"></div>
      </div>
      <div class="moisture-level" id="moistureLevel">Loading...</div>
    </div>
    
    <div class="card">
      <h2>Pump Control</h2>
      <div class="pump-status" id="pumpStatus">Loading...</div>
      <button onclick="waterNow()" id="waterBtn">💧 Water Now</button>
      <button onclick="stopWater()" id="stopBtn">🛑 Stop</button>
    </div>
    
    <div class="last-update" id="lastUpdate"></div>
  </div>

  <script>
    function updateData() {
      fetch('/data')
        .then(response => response.json())
        .then(data => {
          document.getElementById('moistureValue').textContent = data.moisture;
          document.getElementById('moistureLevel').innerHTML = 
            data.icon + ' ' + data.level;
          
          // Update progress bar
          const moisturePercent = (data.moisture / 4095 * 100).toFixed(1);
          const bar = document.getElementById('moistureBar');
          bar.style.width = moisturePercent + '%';
          bar.style.background = data.color;
          
          // Update pump status
          const pumpStatus = document.getElementById('pumpStatus');
          pumpStatus.textContent = data.pumpRunning ? '🚰 PUMP RUNNING' : '💤 PUMP OFF';
          pumpStatus.className = data.pumpRunning ? 'pump-status pump-on' : 'pump-status pump-off';
          
          // Update buttons
          document.getElementById('waterBtn').disabled = data.pumpRunning;
          document.getElementById('stopBtn').disabled = !data.pumpRunning;
          
          document.getElementById('lastUpdate').textContent = 
            'Last update: ' + new Date().toLocaleTimeString();
        })
        .catch(error => {
          console.error('Error fetching data:', error);
        });
    }
    
    function waterNow() {
      fetch('/water');
      setTimeout(updateData, 1000);
    }
    
    function stopWater() {
      fetch('/stop');
      setTimeout(updateData, 1000);
    }
    
    // Update every 2 seconds
    setInterval(updateData, 2000);
    updateData(); // Initial load
  </script>
</body>
</html>
)rawliteral";
  
  server.send(200, "text/html", html);
}

void handleData() {
  String json = "{";
  json += "\"moisture\":" + String(moisture) + ",";
  json += "\"level\":\"" + getMoistureLevel(moisture) + "\",";
  json += "\"icon\":\"" + getMoistureIcon(moisture) + "\",";
  json += "\"color\":\"" + getMoistureColor(moisture) + "\",";
  json += "\"pumpRunning\":" + String(pumpRunning ? "true" : "false");
  json += "}";
  
  server.send(200, "application/json", json);
}

void handleWater() {
  pumpRunning = true;
  digitalWrite(PUMP_PIN, HIGH);
  server.send(200, "text/plain", "Watering started");
  
  // Auto stop after minimum time (non-blocking for web server)
  unsigned long startTime = millis();
  while (millis() - startTime < MIN_WATER_TIME) {
    server.handleClient(); // Keep handling web requests
    delay(100);
  }
  
  digitalWrite(PUMP_PIN, LOW);
  pumpRunning = false;
}

void handleStop() {
  digitalWrite(PUMP_PIN, LOW);
  pumpRunning = false;
  server.send(200, "text/plain", "Watering stopped");
}

void loop() {
  server.handleClient(); // Handle web requests
  
  // Read moisture sensor
  moisture = analogRead(SENSOR_PIN);
  
  // Print to serial
  Serial.print("📶 | Moisture: ");
  Serial.print(moisture);
  Serial.print(" | Level: ");
  Serial.print(getMoistureIcon(moisture));
  Serial.print(" ");
  Serial.print(getMoistureLevel(moisture));
  Serial.print(" | Action: ");
  
  // Automatic watering logic
  if (moisture <= DRY_THRESHOLD && !pumpRunning) {
    Serial.println("AUTO WATERING 💦");
    pumpRunning = true;
    digitalWrite(PUMP_PIN, HIGH);
    
    unsigned long startTime = millis();
    while (millis() - startTime < MIN_WATER_TIME) {
      server.handleClient(); // Keep handling web requests while watering
      moisture = analogRead(SENSOR_PIN); // Update moisture reading
      delay(100);
    }
    
    digitalWrite(PUMP_PIN, LOW);
    pumpRunning = false;
    Serial.println("✅ Auto watering completed");
  } 
  else {
    Serial.println("No watering needed");
  }
  
  delay(2000);  // check every 2 seconds
}
