#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Styx";
const char* password = "framboos";
const char* mqtt_server="192.168.137.1";
const int mqttPort = 9999;
const char* mqttUser = "mqtt username";
const char* mqttPassword = "mqtt password";

WiFiClient espClient;
PubSubClient client(espClient);

#define DOOR_SENSOR_PIN 27

int currentDoorState;
int lastDoorState;

void setup() {

Serial.begin(115200);
pinMode(DOOR_SENSOR_PIN, INPUT_PULLUP);
currentDoorState = digitalRead(DOOR_SENSOR_PIN);
WiFi.begin(ssid, password);
  
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.println("Connecting to WiFi..");
}

Serial.println("Connected to the WiFi network");
client.setServer(mqtt_server, mqttPort);

while (!client.connected()) {
Serial.println("Connecting to MQTT...");
  
if (client.connect("ESP32Client")) {
Serial.println("connected");
} else {
Serial.print("failed with state ");
Serial.print(client.state());
delay(2000);
}
}
  
client.publish("mqtt/test", "Hello from ESP32learning");

}



void loop() {
lastDoorState = currentDoorState;
currentDoorState = digitalRead(DOOR_SENSOR_PIN);
if (lastDoorState == LOW && currentDoorState == HIGH) {
client.publish("mqtt/test","Deur open");
}
else
if (lastDoorState == HIGH && currentDoorState == LOW) {
client.publish("mqtt/test","Deur dicht");
delay(500);
client.loop();
}
}
