// include the library code:
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <DHT.h>
#include <DHT_U.h>


#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

#define DHTPIN 2
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
DHT dht(DHTPIN, DHTTYPE);

float holdTemp = 70.0;
float currentTemp = 70.0;
float newTemp = 70.0;
bool heatON = false;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  dht.begin();
}

void heater(){
    if(!heatON && (holdTemp - currentTemp > 1.0 || holdTemp - currentTemp < -1.0)){
        heatON = true;
    }
}

void checkTemp(){
  static unsigned long lastTime = 0;
  const long interval = 2000;

  unsigned long now = millis();

  if ( now - lastTime > interval){
    newTemp = dht.readTemperature(true);
  }

  if (isnan(newTemp)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  if(currentTemp - newTemp > 1.0 || currentTemp - newTemp < -1.0){
      currentTemp = newTemp;
      heater();
  }
}

void updateLCD(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("TEMP: ");
    lcd.setCursor(0,8);
    lcd.print("HOLD: ");
    lcd.setCursor(1,0);
    lcd.print(holdTemp);
    lcd.setCursor(1,8);
    lcd.print(currentTemp);
}




uint8_t i=0;
void loop() {
  uint8_t buttons = lcd.readButtons();

  if(buttons & BUTTON_UP){
      holdTemp += 1.0;
      updateLCD();
      heater();
  }

  if(buttons & BUTTON_DOWN){
      holdTemp -= 1.0;
      updateLCD();
      heater();
  }
}
