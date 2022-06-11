#include "config.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <IRremote.hpp>
#include <WiFi.h>
#include <aWOT.h>

WiFiServer server(80);
Application app;

void postTransmitNec(Request &request, Response &response)
{
  StaticJsonDocument<256> document;

  response.set("Content-Type", "application/json");
  auto err = deserializeJson(document, request);

  if (err) {
    response.status(400);
    response.print("{\"error\":\"Invalid JSON\"}");
  } else {
    uint16_t address = document["address"];
    uint8_t command = document["command"];
    uint_fast8_t numberOfRepeats = document["numberOfRepeats"];
    bool isRepeat = document["isRepeat"];

    char buffer[256];
    sprintf(buffer,
            "{\"address\":%d,\"command\":%d,\"numberOfRepeats\":%d,"
            "\"isRepeat\":%s}",
            address, command, numberOfRepeats, isRepeat ? "true" : "false");
    Serial.println(buffer);

    IrSender.sendNEC(address, command, numberOfRepeats, isRepeat);

    response.status(200);
    response.print(buffer);
  }
}

void getInputExtra(Request &request, Response &response)
{
  response.set("Content-Type", "application/json");
  response.print("{\"value\":");
  response.print(digitalRead(RING_2_PIN));
  response.print("}");
}

void configureApp()
{
  app.post("/transmit/nec", &postTransmitNec);
  app.get("/input/extra", &getInputExtra);
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RING_1_PIN, OUTPUT);
  pinMode(RING_2_PIN, INPUT);

  Serial.begin(115200);
  IrSender.begin();

  Serial.print("Connecting ");
  WiFi.begin(SECRET_SSID, SECRET_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  configureApp();

  server.begin();
}

void loop()
{
  WiFiClient client = server.available();

  if (client.connected()) {
    app.process(&client);
    client.stop();
  }
}
