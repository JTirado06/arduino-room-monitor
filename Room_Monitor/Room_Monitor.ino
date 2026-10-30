#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

const int DHT_PIN = 2;
DHT dht(DHT_PIN, DHT11);
const int LIGHT_PIN = A0;

// Display width, height, I2C connection, no reset pin
Adafruit_SSD1306 display(128, 64, &Wire, -1);

const unsigned long READ_INTERVAL = 2000;
unsigned long lastRead = 0;

void setup() {
  
  Serial.begin(9600);
  dht.begin();
  Serial.println("Room monitor starting...");

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED initialization failed."));
    while (true) {
      //stop if display memory could not be allocated
    }
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(F("ROOM MONITOR"));
  display.println(F("Starting..."));
  display.display();
}

void loop() {
  
  unsigned long now = millis();

  if (now - lastRead >= READ_INTERVAL) {
    lastRead = now;

    float humidity = dht.readHumidity();
    float temperatureC = dht.readTemperature();
    int light = analogRead(LIGHT_PIN);

    display.clearDisplay();

    display.setCursor(0,0);
    display.println(F("ROOM MONITOR"));
    display.drawLine(0, 12, 127, 12, SSD1306_WHITE);

    if (isnan(humidity) || isnan(temperatureC)) {
      display.setCursor(0,20);
      display.println(F("DHT11 read failed"));
      Serial.println("DHT11 read failed");
    } else {
    float temperatureF = temperatureC * 9.0 / 5.0 + 32.0;

    display.setCursor(0, 20);
    display.print(F("Temp: "));
    display.print(temperatureF, 1);
    display.println(F(" F"));

    display.setCursor(0,32);
    display.print(F("Humidity: "));
    display.print(humidity, 1);
    display.println(F(" %"));

    Serial.print(F("Temp F: "));
    Serial.print(temperatureF, 1);
    Serial.print(F(" | Humidity: "));
    Serial.print(humidity, 1);
    Serial.print(F(" | "));
    
    }

    display.setCursor(0,44);
    display.print(F("Light: "));
    display.print(light);

    display.display();

    Serial.print(F("Light: "));
    Serial.println(light);
  }
}
