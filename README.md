
## 🛠️ Software Setup

### 1. Environment Setup
1. Install [Arduino IDE](https://www.arduino.cc/en/software) or [PlatformIO](https://platformio.org/)
2. Add ESP32 board support in Arduino IDE:
   - File → Preferences → Additional Boards Manager URLs:
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Tools → Board → Boards Manager → Search "ESP32" → Install

### 2. Required Libraries
- `WiFi.h` (built into ESP32 core)
- `WebServer.h` (built into ESP32 core)

### 3. Upload Code
1. Open the provided code in Arduino IDE
2. Select board: **ESP32 Dev Module**
3. Select correct COM port
4. Click **Upload**

## 📶 Network Configuration

The ESP32 operates in **Access Point Mode**:
- **SSID**: `ESP32_Plant`
- **Password**: `12345678`
- **IP Address**: Typically `192.168.4.1` (displayed in Serial Monitor)

### To Connect:
1. Upload the code to ESP32
2. Open Serial Monitor at **115200 baud**
3. Look for the IP address in the output
4. On your phone/computer, connect to WiFi network "ESP32_Plant"
5. Open browser and go to: `http://192.168.4.1`

## 🌐 Web Dashboard Guide

### Dashboard Layout:
- **Soil Moisture Card**: Shows current moisture level with emoji icon
- **Auto Watering Card**: Toggle automatic watering on/off
- **Manual Control Card**: Trigger immediate watering

### Features:
- **Real-time Updates**: Data refreshes every 3 seconds
- **Visual Indicators**: 
  - Color-coded status boxes (green=enabled, red=disabled)
  - Emoji icons for different moisture levels
  - Progress/cooldown indicators
- **Responsive Design**: Works on mobile and desktop

## ⚙️ Configuration Parameters

### Moisture Thresholds (0-4095):
| Range | Level | Icon | Action |
|-------|-------|------|--------|
| ≤ 800 | DRY | 💧 | Auto-watering triggers |
| 801-1600 | MODERATE | 🌱 | Optimal range |
| 1601-2400 | MOIST | ✅ | Good moisture |
| 2401-3200 | WET | 💦 | No watering needed |
| > 3200 | SATURATED | 🌊 | Soil too wet |

### Timing Settings:
- **Moisture Read Interval**: 10 seconds
- **Minimum Water Time**: 5 seconds
- **Watering Cooldown**: 30 minutes (prevents over-watering)

## 🔄 System Operation

### Automatic Mode:
1. System reads moisture every 10 seconds
2. If moisture ≤ 800 (DRY) AND auto mode is enabled AND no cooldown active:
   - Pump turns ON for minimum 5 seconds
   - Cooldown timer starts (30 minutes)
   - Web interface shows cooldown countdown

### Manual Mode:
1. Click "Water Now" button on dashboard
2. Pump runs for 5 seconds (minimum duration)
3. Can be used even when auto mode is disabled

## 📊 Serial Monitor Output

The Serial Monitor provides debugging information:
- System startup status
- WiFi connection details
- Moisture readings with levels
- Pump activation/deactivation logs
- Cooldown status updates

## 🔧 Customization

### Modify in Code:
1. **Moisture Threshold**: Change `DRY_THRESHOLD` value
2. **Watering Duration**: Adjust `MIN_WATER_TIME` (in milliseconds)
3. **Cooldown Period**: Change `WATERING_COOLDOWN` value
4. **Read Interval**: Modify `MOISTURE_READ_INTERVAL`

### Example Customization:
```cpp
// For more frequent watering:
int DRY_THRESHOLD = 1000;      // Water when moderately dry
int MIN_WATER_TIME = 8000;     // Water for 8 seconds
int WATERING_COOLDOWN = 900000; // 15 minute cooldown (900000 ms)
```

## ⚠️ Troubleshooting

### Common Issues:

#### Can't connect to ESP32 WiFi:
- Ensure ESP32 is powered and code is uploaded
- Check Serial Monitor for IP address
- Restart your phone/computer's WiFi
- Verify WiFi credentials in the code (SSID: "ESP32_Plant", Password: "12345678")

#### Pump not turning on:
- Verify relay wiring (NO vs NC terminals)
- Check power supply to pump (5V recommended)
- Ensure common ground between ESP32 and PSU
- Confirm relay module is getting proper voltage
- Check if pump is clogged or jammed

#### Inaccurate moisture readings:
- Calibrate sensor in water (min) and dry air (max)
- Check for loose connections
- Ensure sensor is properly inserted in soil
- Avoid touching sensor with fingers (skin oils affect readings)
- Test sensor in known conditions (water = low value, air = high value)

## 🚀 Future Enhancements

Planned improvements:
- **Data Logging**: Track moisture trends over time
- **Multiple Sensors**: Support for multiple plants
- **Mobile Notifications**: Alert when watering occurs
- **Weather Integration**: Skip watering if rain is forecast
- **Battery Power**: Solar-powered operation option
- **Cloud Integration**: Remote monitoring via smartphone app
- **Advanced Scheduling**: Time-based watering schedules
- **Sensor Calibration**: Web-based calibration interface

## 📄 License

This project is open-source. Feel free to modify and distribute.

## 🙏 Acknowledgments

- ESP32 Arduino Core developers
- WebServer library maintainers
- Open-source hardware community
- All contributors and testers of plant monitoring systems

---

**Happy Planting!** 🌿💧
