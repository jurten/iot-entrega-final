#include <WiFi.h>
#include <PubSubClient.h>

// Wifi
const char* ssid = "MCeciLopez 2.4GHz";
const char* password = "00118472430c";

WiFiClient espClient;
PubSubClient client(espClient);

// Telegram bot
//const char* BOTtoken = "7245465622:AAGLuw0TfiApgPxtVdyr-6Zrnu7ML5NaxLk";
//const char* BOTchatid = "6121046171";
//
//WiFiClientSecure client;
//UniversalTelegramBot bot(BOTtoken, client);

// MQTT
const char* mqtt_server = "192.168.0.14";
const int mqtt_port = 1884;
const char* mqtt_client = "ESP32_Client";
const char* mqtt_topic = "esp/temperature";

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

//void scanNetworks() {
//  int n = WiFi.scanNetworks();
//  Serial.println("Scan done");
//  if (n == 0) {
//    Serial.println("No networks found");
//  } else {
//    Serial.print(n);
//    Serial.println(" networks found");
//    for (int i = 0; i < n; ++i) {
//      Serial.print(i + 1);
//      Serial.print(": ");
//      Serial.print(WiFi.SSID(i));
//      Serial.print(" (");
//      Serial.print(WiFi.RSSI(i));
//      Serial.print(")");
//      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
//      delay(10);
//    }
//  }
//}

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
