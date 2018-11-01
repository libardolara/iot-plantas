#include <ESP8266WiFi.h>
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient/releases/tag/v2.3
#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter(0x23);
const int analog = A0;
int soilmoistur  = 0;

//-------- Customise these values -----------
const char* ssid = "MarcoCasallasAP";
const char* password = "unodostres4";

#define ORG "1wmrzl"
#define DEVICE_TYPE "ESP8266-MONITOR"
#define DEVICE_ID "M-4371"
#define TOKEN "xQ8!id51WoY0KO&0Tt"
//-------- Customise the above values --------

char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/status/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

WiFiClient wifiClient;
PubSubClient client(server, 1883, NULL, wifiClient);

void setup() {
 Serial.begin(9600);
 Serial.println();
 Wire.begin(D1, D1);
 Wire.begin(4,5);
 lightMeter.begin();
 Serial.print("Connecting to "); Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 } 
 Serial.println("");

 Serial.print("WiFi connected, IP address: "); Serial.println(WiFi.localIP());
}

int counter = 0;

void loop() {

// if (!client.connected()) {
//   Serial.print("Reconnecting client to ");
//   Serial.println(server);
//   while (!client.connect(clientId, authMethod, token)) {
//      Serial.print(".");
//      delay(500);
//   }
//   Serial.println();
// }


client.connect(clientId, authMethod, token);

 soilmoistur= analogRead (analog);
 uint16_t lux = lightMeter.readLightLevel();
 String payload = "{\"d\":{\"Name\":\"Plant\"";
 payload += ",\"light\":";
 payload += lux;
 payload += ",\"humidity\":";
 payload += soilmoistur;
 payload += "}}";
 
 Serial.print("Sending payload: ");
 Serial.println(payload);
 
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }

 delay(5000);
}