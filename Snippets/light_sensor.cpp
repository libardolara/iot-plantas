// Wire.begin(sda, scl), d2 --> gpio4, d1 --> gpio5

#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter(0x23);

void setup(){
  Serial.begin(9600);
  // On esp8266 devices you can select SCL and SDA pins using Wire.begin(SDA, SCL);
  Wire.begin(4,5);
  lightMeter.begin();
  Serial.println(F("BH1750 Test"));
}

void loop() {
  uint16_t lux = lightMeter.readLightLevel();
  Serial.print("Nivel de iluminacion: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);
}
