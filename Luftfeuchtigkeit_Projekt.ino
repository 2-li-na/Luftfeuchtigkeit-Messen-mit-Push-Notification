#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <Adafruit_NeoPixel.h>
#include <SeeedOLED.h>
// #include <U8g2lib.h>
#include <time.h>

//WiFi Infos
const char* ssid = "MagentaWLAN-YECH";
const char* password = "85815231439396745295";

//define I2C pins for sensor
#define I2C_SDA 21
#define I2C_SCL 22

//Define RGB LED Ring
#define LED_PIN 19
#define NUM_LEDS 24
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

//instance of BME680 sensor
Adafruit_BME680 bme;

//NTP Time settings
const long gmtOffset_sec = 3600; // +1 hour for CET
const int daylightOffset_sec = 3600; //Additional +1 hour for CEST

//Wifi Connection Function
void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) { // Try to connect for a certain number of attempts
    delay(1000);
    Serial.print(".");
    attempts++;
  }

  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to WiFi.");
    return;
  }

  Serial.println("");
  Serial.println("WiFi Connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

//Display on OLED 
void updateDisplay(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  char buffer[32];

  SeeedOled.clearDisplay();
  SeeedOled.setNormalDisplay();
  SeeedOled.setPageMode();

  strftime(buffer, sizeof(buffer), "%d-%m-%Y", &timeinfo);
  SeeedOled.setTextXY(0,0); // Line 0
  SeeedOled.putString(buffer);

  strftime(buffer, sizeof(buffer), "Uhr: %H:%M:%S", &timeinfo);
  SeeedOled.setTextXY(1,0); // Line 1
  SeeedOled.putString(buffer);

  sprintf(buffer, "Temp: %.1f C", bme.temperature);
  SeeedOled.setTextXY(2,0); // Line 2
  SeeedOled.putString(buffer);

  sprintf(buffer, "Hum: %.1f %%", bme.humidity);
  SeeedOled.setTextXY(3,0); // Line 3
  SeeedOled.putString(buffer);
}

//Air Quality Check
  float calculateAirQuality() {
    return bme.humidity;
    return bme.temperature;
    return bme.pressure / 100.0;
    return bme.gas_resistance / 1000.0;
  }

  void setRingColor(uint8_t red, uint8_t green, uint8_t blue) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(red, green, blue));
  }
  strip.show();
  }



void setup() {
  //start I2C communication
  Wire.begin(I2C_SDA, I2C_SCL);

  //start serial communication
  Serial.begin(115200);
  while(!Serial);

  //Initialize BME680 sensor
  if(!bme.begin(0x76)){
    Serial.println("Could not find valid sensor!");
    while(1);
  }

//Configuration on how BME680 sensor reads data
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150);

//Initiliaze RGB LED Ring
  strip.begin();
// Initialize all pixels to 'off'
  strip.show();

  connectToWiFi();

// Initiate NTP
  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org", "time.nist.gov");

//Wait for the time to be fetched
  struct tm timeinfo;
  while(!getLocalTime(&timeinfo)) {
    Serial.println("Fetching time from NTP server...");
    delay(1000);
  }
// Initialize OLED display
    SeeedOled.init();  

    //Habing these or not doesn't change anything
    // SeeedOled.clearDisplay();          
    // SeeedOled.setNormalDisplay();      
    // SeeedOled.setPageMode();          

    // connectToWiFi();
}


void loop() {
  //Tell BME680 to begin measurement
  unsigned long startTime = millis();
  if(!bme.performReading()){
    Serial.println("Failed to perform reading...");
    return;
  }
  Serial.print("Temperature = ");
  Serial.print(bme.temperature);
  Serial.println(" *C");

  Serial.print("Humidity = ");
  Serial.print(bme.humidity);
  Serial.println(" %");

  Serial.print("Pressure = ");
  Serial.print(bme.pressure / 100.0);
  Serial.println(" hPa");

  Serial.print("Gas = ");
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.println(" KOhms");

  // updateDisplay();
  delay(2000); // Delay between readings

//Calculate air Quality
  float airQualityValue = calculateAirQuality(); 

// update RGB LED RING based on air quality
  updateLEDRing(airQualityValue);
  delay(2000);
}

void updateLEDRing(float airQualityValue) {
  float lowQ = 10.0;
  // float moderateQ = 30.0;

  if(airQualityValue < lowQ) {
    setRingColor(255, 0, 0); //RED Color
  } else {
    setRingColor(0,255,0); //Green
  }

  updateDisplay();
  delay(2000); 
}

