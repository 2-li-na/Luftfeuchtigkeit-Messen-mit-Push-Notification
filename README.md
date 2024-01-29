# Luftfeuchtigkeit-Messen-mit-Push-Notification

This project is an air quality monitoring system using ESP32 that reads environmental data from a BME680 sensor, displays the information on an OLED display, and indicates air quality levels with an RGB LED ring, and sends air quality alerts via Telegram.

## Features

- **BME680 Sensor**: Measures temperature, humidity, pressure, and gas resistance to calculate air quality.
- **OLED Display**: Shows date, time (fetched from NTP server), temperature, and humidity.
- **RGB LED Ring**: Provides a visual indication of air quality levels.
- **WiFi Connectivity**: Connects to the internet to fetch current time from an NTP server.
- **Telegram Bot Alerts**: Sends messages about air quality status through a Telegram bot.

## Hardware Requirements

- ESP32 development board
- BME680 environmental sensor
- 128x64 OLED display
- Adafruit NeoPixel RGB LED Ring (24 LEDs)

## Software Dependencies

- [Adafruit BME680 Library](https://github.com/adafruit/Adafruit_BME680)
- [Adafruit NeoPixel Library](https://github.com/adafruit/Adafruit_NeoPixel)
- [Seeed OLED Library](https://github.com/Seeed-Studio/OLED_Display_128X64)
- [Universal Arduino Telegram Bot Library](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot)
- WiFi Library (available by default with ESP32 boards in Arduino IDE)
- HTTPClient Library (available by default with ESP32 boards in Arduino IDE)
- WiFiClient, WiFiClientSecure Library (available by default with ESP32 boards in Arduino IDE)
- Time Library (available by default with ESP32 boards in Arduino IDE)
  
## Setup

1. Connect the BME680 sensor and the OLED display to the ESP32 using the I2C protocol.
2. Connect the RGB LED Ring to the ESP32's digital pin as defined in the code.
3. Set your WiFi SSID and password to enable internet connectivity.
4. The ESP32 will connect to the WiFi network and fetch the current time from an NTP server.
5. The sensor data and the current time will be displayed on the OLED screen.
6. The LED ring will change colors based on the air quality reading.
7. Insert your Telegram bot token, and chat ID into the code.

## Configuration

- Set the NTP time offset to match your local time zone. For CET (Central European Time), the offset is set to +1 hour.
- Set up the Telegram bot using BotFather and obtain the token and chat ID.

