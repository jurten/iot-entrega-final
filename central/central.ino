
#include <WiFi.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Time.h>

// Wifi credentials
const char* ssid = "";
const char* password = "";

// Telegram bot
const char* BOTtoken = "";
const char* BOTchatid = "";

WiFiClient wifiClient;
UniversalTelegramBot bot(BOTtoken, client);

// MQTT
const char* mqtt_server = ""; //localhost
const int mqtt_port = 1883;
const char* mqtt_topic = "invernadero/temperatura";
PubSubClient client(wifiClient);

// Variables
bool ventanaAbierta = false;
bool motorActivo = false;
unsigned long lastTimeActive = 0;
const long outputActiveTime = 10000;
unsigned long lastMsgTime = 0;
const long interval = 300000; // 5 min


void setup() {
  Serial.begin(115200);
  delay(1000);

  // Setup WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Setup Telegram bot
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  
  Serial.print("ESP32 local IP Address: ");
  Serial.println(WiFi.localIP());

  // MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void abrirVentana(float temperature) {
  digitalWrite(LED_PIN, HIGH); // Turn on LED (simulate motor running)
  ventanaAbierta = true;
  motorActivo = true;
  lastTimeActive = millis();
}

void cerrarVentana(float temperature) {
  digitalWrite(LED_PIN, HIGH); // Turn on LED (simulate motor running)
  ventanaAbierta = false;
  motorActivo = true;
  lastTimeActive = millis();
}

void enviarEstado(float temperature) {
  String estadoVentanas = ventanaAbierta ? "abiertas" : "cerradas";
  String mensaje = "Temperatura: " + String(temperature) + "°C - Ventanas: " + estadoVentanas;
  bot.sendMessage(BOTchatid, mensaje, "");
}

void sendWelcomeMessage(String BOTchatid, String from_name) {
  String welcome = "Hola, " + from_name + "!\n";
  welcome += "Comandos.\n\n";
  welcome += "/estado: para ver el estado de las ventanas y la temperatura del invernadero\n";
  welcome += "/abrir: para abrir las ventanas\n";
  welcome += "/cerrar: para cerrar las ventanas\n";
  bot.sendMessage(BOTchatid, welcome, "");
}

void handleTelegramMessage(String text, String from_name, float temperature, String BOTchatid) {
  unsigned long currentMillis = millis();
  
  if (text == "/estado") {
    enviarEstado(temperature);
    Serial.println("User requested status. Sent status update.");
  } else if (text == "/abrir") {
    if (currentMillis - lastTimeActive > outputActiveTime) {
      if (!ventanaAbierta) {
        abrirVentana(temperature);
        bot.sendMessage(BOTchatid, "Abriendo ventanas. " + String(temperature) + "°C", "");
        Serial.println("User asked to open windows. Windows now open.");
      } else {
        bot.sendMessage(BOTchatid, "Ventanas ya abiertas. " + String(temperature) + "°C", "");
        Serial.println("User asked to open windows. Windows already open.");
      }
    } else {
      bot.sendMessage(BOTchatid, "Motor ya funcionando. " + String(temperature) + "°C", "");
      Serial.println("User asked to open windows. Motor already running.");
    }
  } else if (text == "/cerrar") {
    if (currentMillis - lastTimeActive > outputActiveTime) {
      if (ventanaAbierta) {
        cerrarVentana(temperature);
        bot.sendMessage(BOTchatid, "Cerrando ventanas. " + String(temperature) + "°C", "");
        Serial.println("User asked to close windows. Windows now closed.");
      } else {
        bot.sendMessage(BOTchatid, "Ventanas ya cerradas. " + String(temperature) + "°C", "");
        Serial.println("User asked to close windows. Windows already closed.");
      }
    } else {
      bot.sendMessage(BOTchatid, "Motor ya funcionando. " + String(temperature) + "°C", "");
      Serial.println("User asked to close windows. Motor already running.");
    }
  } else {
    sendWelcomeMessage(BOTchatid, from_name);
    Serial.println("Sent welcome message to: " + from_name);
  }
}

void MapForMQTT()

void loop() {
  client.loop();
  char temp[10];
  float temperature = random(10, 40); // Simulate temperature between 10°C and 40°C
  sprintf(temp, "%u.10s", temperature, 10); // map temperature to char array for MQTT
  unsigned long currentMillis = millis();

  if (temperature > 30 && !ventanaAbierta && !motorActivo) {
    client.Publish(mqtt_topic, temp);
    client.Publish(mqtt_topic, Time.now()); // TODO: Buscar correcto formato para InfluxDB
    abrirVentana(temperature);
  } else if (temperature < 20 && ventanaAbierta && !motorActivo) {
    client.Publish(mqtt_topic, temp);
    client.Publish(mqtt_topic, Time.now());
    cerrarVentana(temperature);
  }

  // Send regular updates every 5 minutes
  if (currentMillis - lastMsgTime > interval) {
    enviarEstado(temperature);
    lastMsgTime = currentMillis;
  }

  // Check for Telegram messages
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  while (numNewMessages) {
    for (int i = 0; i < numNewMessages; i++) {
      String text = bot.messages[i].text;
      String BOTchatid = bot.messages[i].chat_id;
      String from_name = bot.messages[i].from_name;
      handleTelegramMessage(text, from_name, temperature, BOTchatid);
    }
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  }

  // Reset motor status after the outputActiveTime
  if (motorActivo && currentMillis - lastTimeActive > outputActiveTime) {
    motorActivo = false;
    digitalWrite(LED_PIN, LOW);
  }
}