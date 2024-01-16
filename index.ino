#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <ArduinoJson.h> 

#define DHTPIN 2    
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
// Replace with your network credentials
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Replace with MQTT broker address, port, username, and password
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
const char* mqtt_username = "polsing";
const char* mqtt_password = "150402";

// Create an instance of the WiFiClient class
WiFiClient espClient;

// Create an instance of the PubSubClient class
PubSubClient client(espClient);
  
// Function to handle messages received from MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String payloadStr;
  for (int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
  }
  //Serial.println(payloadStr);
  // Parse JSON
  const size_t capacity = JSON_OBJECT_SIZE(2); // Adjust the size based on your JSON structure
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, payloadStr);

 if (doc.containsKey("LED1")) {
  bool led1Value = doc["LED1"];
  if(led1Value)
  digitalWrite(16, HIGH);
  else
  digitalWrite(16, LOW);
} 

// Access LED2 value
if (doc.containsKey("LED2")) {
  bool led2Value = doc["LED2"];
  if(led2Value)
  digitalWrite(17, HIGH);
  else
  digitalWrite(17, LOW);
} 
}

void setup() {
  // Start the serial communication
  Serial.begin(115200);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  // Connect to Wi-Fi
  Serial.println("Connecting to WiFi ");
  WiFi.begin(ssid, password,6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected successfully");

  // Set the MQTT broker and credentials
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // Connect to MQTT
  Serial.println("");
  while (!client.connected()){
    Serial.print("Connecting to MQTT: ");
    Serial.println(mqtt_server);
  if (client.connect("mqttx_5c5c2f6c", mqtt_username, mqtt_password)) {
      Serial.println("Connected to mqtt successfully");
      client.subscribe("onLED");  // Subscribe to topic "msg/"
    } else {
      Serial.print("Failed with state ");
      Serial.print(client.state());
      Serial.println("");
      delay(2000);
    }
  }
  
  dht.begin();
  }

void loop() {
  // Reconnect to MQTT if connection is lost
  if (!client.connected()) {
      Serial.print("Connecting to MQTT: ");
      Serial.println(mqtt_server);
  if (client.connect("mqttx_5c5c2f6c", mqtt_username, mqtt_password)) {
      Serial.println("Connected to mqtt successfully");
      client.subscribe("onLED");  // Subscribe to topic "msg/"
    } else {
      Serial.print("Failed with state ");
      Serial.print(client.state());
      Serial.println("");
      delay(2000);
    }
  }
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  DynamicJsonDocument jsonDoc(200);
  jsonDoc["humidity"] = h;
  jsonDoc["temperature_C"] = t;
  jsonDoc["temperature_F"] = f;
  String jsonString;
  serializeJson(jsonDoc, jsonString);
  client.publish("formData", jsonString.c_str());
  
  client.loop();
  delay(1000);
}
