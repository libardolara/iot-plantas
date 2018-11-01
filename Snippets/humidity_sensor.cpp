const int analog = A0;
int soilmoistur  = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  soilmoistur= analogRead (analog);
  Serial.println("Soil Moisture:");
  Serial.println(soilmoistur);

   Serial.println("");

  // Wait a bit before scanning again
  delay(250);
}
