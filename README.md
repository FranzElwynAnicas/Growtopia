🌱 ESP32 Automatic Plant Watering System

This project is a smart plant watering system using ESP32, a capacitive soil moisture sensor, and a 1-way relay-controlled water pump. It includes a web dashboard to monitor soil moisture and manually control watering.

Features:
- Measures soil moisture using Capacitive Soil Moisture Sensor V2.0
- Automatically waters plants if soil is dry
- Web-based dashboard to monitor moisture levels and control pump
- Visual feedback with color-coded moisture levels and icons
- Audio/visual notifications on the web dashboard (optional)

Hardware Requirements:
Component                  Notes
ESP32 WROOM-32             Microcontroller
Capacitive Soil Moisture Sensor V2.0  Analog sensor, connect to 3.3V and GND
1-way Relay Module         Controls the pump
Mini DC Water Pump         Powered by external 5V PSU
Power Supply (PSU)         5V for pump, 3.3V for ESP32
Jumper wires, breadboard   For connections

Wiring:
ESP32 Pinout:
- GPIO 33 → Sensor AO
- GPIO 26 → Relay IN
- Sensor VCC → 3.3V, GND → ESP32 GND
- Relay VCC → 5V PSU, GND → ESP32 GND
- Pump + → Relay NO, Pump - → PSU GND
Note: Always connect ESP32 GND and PSU GND together to share common ground.

Software Setup:
1. Install Arduino IDE (or PlatformIO)
2. Install ESP32 Board Support via Arduino Board Manager
3. Install Required Libraries: WiFi.h (built-in), WebServer.h (built-in)
4. Modify WiFi Credentials in main.ino:
   const char* ssid = "your_wifi_ssid";
   const char* password = "your_wifi_password";
5. Upload the code to your ESP32

Using the Web Dashboard:
- Open Serial Monitor at 115200 baud to see logs
- Connect ESP32 to WiFi
- The IP address of ESP32 will be printed in Serial Monitor
- Open a browser and go to http://<ESP32_IP>
Dashboard Features:
- Soil Moisture: Shows current value, icon, and color
- Pump Control: Manual Water / Stop buttons
- Progress Bar: Visual moisture level
- Updates automatically every 2 seconds

Automatic Watering Logic:
- Soil moisture is read every 2 seconds
- Thresholds (0–4095 for ESP32 ADC):
  ≤ 800 → Dry → Pump ON for minimum 5 seconds
  800–1600 → Moderate
  1600–2400 → Moist
  2400–3200 → Wet
  > 3200 → Saturated → No watering
- Pump runs non-blocking during watering to keep web server responsive

Future Improvements:
- Add multi-plant support
- Integrate IoT notifications (e.g., mobile alerts)
- Add data logging for soil moisture trends
- Connect audio buzzer for watering alerts
