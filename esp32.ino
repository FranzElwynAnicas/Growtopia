#include <WiFi.h>
#include <WebServer.h>

// Pin definitions for sensor and pump
#define SENSOR_PIN 33    // Soil moisture sensor analog pin
#define PUMP_PIN   26    // Relay control pin for water pump

WebServer server(80);    // Create web server on port 80

// Global variables to track system state
int moisture = 0;              // Current moisture reading from sensor
bool pumpRunning = false;      // Track if pump is currently active
bool autoWateringEnabled = true; // Auto watering mode (true=enabled, false=disabled)

// Watering parameters
int DRY_THRESHOLD = 800;       // Moisture value below which soil is considered dry
int MIN_WATER_TIME = 5000;     // Minimum watering duration in milliseconds (5 seconds)

// Timing constants
const unsigned long MOISTURE_READ_INTERVAL = 10000;  // Read moisture every 10 seconds
const unsigned long WATERING_COOLDOWN = 1800000;       // 30 minute cooldown after auto watering

// For non-blocking pump control
unsigned long pumpStartTime = 0;
bool autoPumpActive = false;
unsigned long lastAutoWateringTime = 0;  // Track when last auto watering finished

// -----------------------------------------------------
//  ACCESS POINT SETUP
// -----------------------------------------------------
/**
 * Starts the ESP32 as a WiFi Access Point
 * Creates a network that phones/computers can connect to
 */
void startAccessPoint() {
  // Create WiFi network with name "ESP32_Plant" and password "12345678"
  WiFi.softAP("ESP32_Plant", "12345678");  
  Serial.print("Connect to: http://");
  Serial.println(WiFi.softAPIP());  // Print the IP address to connect to
}

// -----------------------------------------------------
//  MOISTURE LEVEL FUNCTIONS
// -----------------------------------------------------
/**
 * Converts raw moisture sensor value to descriptive text
 * @param m Raw analog reading (0-4095)
 * @return String describing moisture level
 */
String getMoistureLevel(int m) {
  if (m <= DRY_THRESHOLD) return "DRY";        // Very dry soil - needs watering
  if (m <= 1600) return "MODERATE";            // Average moisture - OK
  if (m <= 2400) return "MOIST";               // Good moisture level
  if (m <= 3200) return "WET";                 // Very moist - no watering needed
  return "SATURATED";                          // Completely soaked
}

/**
 * Returns emoji icon based on moisture level for visual display
 * @param m Raw analog reading (0-4095)
 * @return String with appropriate emoji
 */
String getMoistureIcon(int m) {
  if (m <= DRY_THRESHOLD) return "💧";  // Water drop - indicates need for water
  if (m <= 1600) return "🌱";           // Plant - moderate condition
  if (m <= 2400) return "✅";           // Check mark - good condition
  if (m <= 3200) return "💦";           // Water waves - wet condition
  return "🌊";                          // Ocean wave - saturated
}

// -----------------------------------------------------
//  PUMP CONTROL FUNCTIONS
// -----------------------------------------------------
/**
 * Turns the water pump ON
 * Uses active-low logic (LOW signal activates relay)
 */
void turnPumpOn() {
  digitalWrite(PUMP_PIN, LOW);   // Set pin LOW to activate relay
  pumpRunning = true;            // Update state flag
  Serial.println("Pump turned ON");
}

/**
 * Turns the water pump OFF  
 * Uses active-low logic (HIGH signal deactivates relay)
 */
void turnPumpOff() {
  digitalWrite(PUMP_PIN, HIGH);  // Set pin HIGH to deactivate relay
  pumpRunning = false;           // Update state flag
  Serial.println("Pump turned OFF");
}

// -----------------------------------------------------
//  WEB SERVER HANDLERS
// -----------------------------------------------------
/**
 * Handles the main webpage request
 * Sends HTML interface to the client browser
 */
void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
<title>Plant Monitor</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body { 
  font-family: 'Arial', sans-serif; 
  text-align: center; 
  padding: 20px; 
  background-color: #e8f5e8; /* Pastel green background */
  margin: 0;
  min-height: 100vh;
}

.container {
  max-width: 500px;
  margin: 0 auto;
}

.card { 
  background: white; 
  padding: 20px; 
  margin: 15px 0; 
  border-radius: 15px; 
  border: 2px solid #c8e6c9; /* Light green border */
  box-shadow: 0 4px 8px rgba(0,0,0,0.1);
  transition: transform 0.2s ease;
}

.card:hover {
  transform: translateY(-2px);
  box-shadow: 0 6px 12px rgba(0,0,0,0.15);
}

.moisture-value { 
  font-size: 3em; 
  margin: 10px 0;
}

.moisture-level {
  font-size: 1.4em;
  font-weight: bold;
  color: #2e7d32;
  margin: 10px 0;
}

button { 
  padding: 12px 20px; 
  margin: 8px; 
  border: none; 
  border-radius: 8px; 
  font-size: 16px;
  font-weight: bold;
  cursor: pointer;
  transition: all 0.3s ease;
  border: 2px solid transparent;
}

button:hover {
  transform: scale(1.05);
}

.auto-on { 
  background: #4caf50; 
  color: white; 
  border: 2px solid #388e3c;
}

.auto-off { 
  background: #f44336; 
  color: white; 
  border: 2px solid #d32f2f;
}

.cooldown { 
  background: #ffb74d; 
  color: white; 
  padding: 8px 12px; 
  border-radius: 8px; 
  font-size: 14px;
  border: 2px solid #f57c00;
  display: inline-block;
  margin-top: 10px;
}

.status-box {
  padding: 12px;
  border-radius: 8px;
  margin: 10px 0;
  font-weight: bold;
  border: 2px solid;
}

.pump-running {
  background: #ffebee;
  color: #c62828;
  border-color: #ef5350;
}

.pump-off {
  background: #e8f5e8;
  color: #2e7d32;
  border-color: #4caf50;
}

.auto-enabled {
  background: #e8f5e8;
  color: #2e7d32;
  border-color: #4caf50;
}

.auto-disabled {
  background: #ffebee;
  color: #c62828;
  border-color: #ef5350;
}

h1 {
  color: #2e7d32;
  margin-bottom: 10px;
  text-shadow: 1px 1px 2px rgba(0,0,0,0.1);
}

.subtitle {
  color: #558b2f;
  margin-bottom: 30px;
  font-style: italic;
}
</style>
</head>
<body>

<div class="container">
  <h1>🌱 Plant Monitor</h1>
  <div class="subtitle">Smart Plant Care System</div>

  <!-- Moisture Status Card -->
  <div class="card">
    <h2>Soil Moisture</h2>
    <div class="moisture-value" id="moistureIcon">--</div>
    <div class="moisture-level" id="moistureLevel">Loading...</div>
    <div id="cooldownStatus"></div>
  </div>

  <!-- Auto Watering Control Card -->
  <div class="card">
    <h2>Auto Watering</h2>
    <div class="status-box" id="autoStatus">Loading...</div>
    <div>
      <button onclick="enableAutoWater()" id="autoOnBtn" class="auto-on">✅ Enable Auto</button>
      <button onclick="disableAutoWater()" id="autoOffBtn" class="auto-off">❌ Disable Auto</button>
    </div>
  </div>

  <!-- Manual Control Card -->
  <div class="card">
    <h2>Manual Control</h2>
    <div class="status-box" id="pumpStatus">Loading...</div>
    <button onclick="waterNow()" id="waterBtn">💧 Water Now</button>
  </div>
</div>

<script>
/**
 * Fetches current system data from ESP32 and updates the webpage
 */
function updateData() {
  fetch('/data').then(r => r.json()).then(data => {
    // Update moisture display
    document.getElementById('moistureIcon').textContent = data.icon;
    document.getElementById('moistureLevel').textContent = data.level;
    
    // Update pump status with styling
    let pumpElement = document.getElementById('pumpStatus');
    pumpElement.textContent = data.pumpRunning ? "🚰 PUMP RUNNING" : "💤 PUMP OFF";
    pumpElement.className = data.pumpRunning ? "status-box pump-running" : "status-box pump-off";
    
    // Update auto watering status with styling
    let autoElement = document.getElementById('autoStatus');
    autoElement.textContent = data.autoWatering ? "🔄 AUTO WATERING: ENABLED" : "⏸️ AUTO WATERING: DISABLED";
    autoElement.className = data.autoWatering ? "status-box auto-enabled" : "status-box auto-disabled";
    
    // Update cooldown status
    let cooldownElement = document.getElementById('cooldownStatus');
    if (data.inCooldown) {
      cooldownElement.textContent = "⏳ Cooldown: " + data.cooldownLeft + "s remaining";
      cooldownElement.className = "cooldown";
    } else {
      cooldownElement.textContent = "";
    }
    
    // Enable/disable buttons based on current state
    document.getElementById('waterBtn').disabled = data.pumpRunning;
    document.getElementById('autoOnBtn').disabled = data.autoWatering;
    document.getElementById('autoOffBtn').disabled = !data.autoWatering;
    
    // Style disabled buttons
    document.getElementById('waterBtn').style.opacity = data.pumpRunning ? "0.6" : "1";
    document.getElementById('autoOnBtn').style.opacity = data.autoWatering ? "0.6" : "1";
    document.getElementById('autoOffBtn').style.opacity = !data.autoWatering ? "0.6" : "1";
  });
}

// Button click handlers - send commands to ESP32
function waterNow() { fetch('/water'); }
function enableAutoWater() { fetch('/auto/on'); }
function disableAutoWater() { fetch('/auto/off'); }

// Update data every 3 seconds and initialize
setInterval(updateData, 3000);
updateData();
</script>

</body></html>
)rawliteral";

  server.send(200, "text/html", html);  // Send HTML to client
}

/**
 * Provides current system data in JSON format for the webpage
 * Called by JavaScript every 3 seconds to update the interface
 */
void handleData() {
  // Calculate cooldown time left
  int cooldownLeft = 0;
  bool inCooldown = false;
  
  if (lastAutoWateringTime > 0) {
    unsigned long cooldownRemaining = WATERING_COOLDOWN - (millis() - lastAutoWateringTime);
    if (cooldownRemaining > 0 && cooldownRemaining <= WATERING_COOLDOWN) {
      inCooldown = true;
      cooldownLeft = cooldownRemaining / 1000;
    }
  }

  // Build JSON response with current system state
  String json = "{";
  json += "\"level\":\"" + getMoistureLevel(moisture) + "\",";      // Moisture text
  json += "\"icon\":\"" + getMoistureIcon(moisture) + "\",";        // Moisture emoji
  json += "\"pumpRunning\":" + String(pumpRunning ? "true" : "false") + ",";  // Pump state
  json += "\"autoWatering\":" + String(autoWateringEnabled ? "true" : "false") + ","; // Auto mode
  json += "\"inCooldown\":" + String(inCooldown ? "true" : "false") + ","; // Cooldown state
  json += "\"cooldownLeft\":" + String(cooldownLeft); // Cooldown time left
  json += "}";
  server.send(200, "application/json", json);  // Send JSON data
}

/**
 * Handles manual watering request from web interface
 * Turns pump on for fixed duration then automatically turns off
 */
void handleWater() {
  if (!pumpRunning) {
    Serial.println("Manual watering started");
    turnPumpOn();
    server.send(200, "text/plain", "OK");  // Acknowledge request
    
    // Keep pump running for minimum watering time
    unsigned long startTime = millis();
    while (millis() - startTime < MIN_WATER_TIME) {
      delay(100);  // Small delay to prevent blocking
    }
    
    turnPumpOff();
    Serial.println("Manual watering stopped");
  }
}

/**
 * Enables automatic watering mode
 * System will automatically water when soil is dry
 */
void handleAutoOn() {
  autoWateringEnabled = true;
  server.send(200, "text/plain", "OK");
  Serial.println("Auto watering ENABLED");
}

/**
 * Disables automatic watering mode  
 * System will only monitor moisture, not water automatically
 */
void handleAutoOff() {
  autoWateringEnabled = false;
  server.send(200, "text/plain", "OK");
  Serial.println("Auto watering DISABLED");
}

// -----------------------------------------------------
//  ARDUINO SETUP FUNCTION - RUNS ONCE AT STARTUP
// -----------------------------------------------------
void setup() {
  Serial.begin(115200);           // Start serial communication for debugging
  pinMode(PUMP_PIN, OUTPUT);      // Set pump pin as output
  turnPumpOff();                  // Ensure pump starts in OFF state

  Serial.println("🌱 Plant Watering System Starting...");
  Serial.println("📊 Moisture reading interval: 10 seconds");
  Serial.println("❄️ Watering cooldown: 30 seconds");
  
  startAccessPoint();             // Create WiFi access point

  // Register URL handlers for web server
  server.on("/", handleRoot);         // Main webpage
  server.on("/data", handleData);     // JSON data endpoint
  server.on("/water", handleWater);   // Manual watering
  server.on("/auto/on", handleAutoOn);  // Enable auto mode
  server.on("/auto/off", handleAutoOff); // Disable auto mode

  server.begin();                 // Start web server
  Serial.println("✅ Web server started - System Ready!");
}

// -----------------------------------------------------
//  ARDUINO LOOP FUNCTION - RUNS REPEATEDLY
// -----------------------------------------------------
void loop() {
  server.handleClient();  // Handle incoming web requests
  
  // Handle automatic pump timing (non-blocking)
  if (autoPumpActive && pumpRunning) {
    if (millis() - pumpStartTime >= MIN_WATER_TIME) {
      turnPumpOff();
      autoPumpActive = false;
      lastAutoWateringTime = millis();  // Start cooldown period
      Serial.println("✅ Auto watering completed - Cooldown started");
    }
  }
  
  // Read moisture sensor every 10 seconds (optimized for battery/power)
  static unsigned long lastMoistureRead = 0;
  if (millis() - lastMoistureRead > MOISTURE_READ_INTERVAL) {
    moisture = analogRead(SENSOR_PIN);  // Read current moisture level
    lastMoistureRead = millis();        // Update last read time
    
    Serial.print("📊 Moisture reading: ");
    Serial.print(moisture);
    Serial.print(" - ");
    Serial.println(getMoistureLevel(moisture));
    
    // Check if we're in cooldown period after auto watering
    bool inCooldown = (lastAutoWateringTime > 0) && 
                     (millis() - lastAutoWateringTime < WATERING_COOLDOWN);
    
    /**
     * Automatic watering logic:
     * - Only if auto watering is enabled
     * - Only if pump is not already running  
     * - Only if soil is dry (below threshold)
     * - Only if NOT in cooldown period
     */
    if (autoWateringEnabled && !pumpRunning && moisture <= DRY_THRESHOLD && !inCooldown) {
      Serial.println("🔄 Auto watering activated - Dry soil detected");
      turnPumpOn();
      pumpStartTime = millis();
      autoPumpActive = true;
    } else if (inCooldown) {
      Serial.println("⏳ Cooldown active - Auto watering paused");
    }
  }
  
  delay(100);  // Small delay to prevent overwhelming the processor
}
