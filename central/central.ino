#include <WiFi.h>
#include <PubSubClient.h>

// Wifi
const char* ssid = "Carrie Mathison";
const char* password = "1122332211";

WiFiClient espClient;
PubSubClient client(espClient);

// MQTT
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char* mqtt_client = "ESP32_Client";
const char* mqtt_topic = "esp/supercomplex/system/temperature";

// Pin
#define LED_PIN 2

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(mqtt_client)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void initWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      delay(1000);
    }
    Serial.print("connected. IP: ");
    Serial.println(WiFi.localIP());
  }

void setup() {
  Serial.begin(115200);
  delay(2000);

  initWiFi();

  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float temp = random(10, 40);
  char message[6]; // Enough for "-40.00\0"
  
  // Convert float to string with 2 decimal places
  dtostrf(temp, 5, 2, message);
 
  client.publish(mqtt_topic, message);
  Serial.print("Sent temperature: ");
  Serial.println(message);

  delay(5000);
}
