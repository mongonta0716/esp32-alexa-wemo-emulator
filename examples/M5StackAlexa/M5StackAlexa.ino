// This app is designed for M5Stack LovyanLauncher.
// https://github.com/lovyan03/M5Stack_LovyanLauncher

#include <WiFi.h>
#include <M5Stack.h>
#include <M5StackUpdater.h>

#include "WemoSwitch.h"
#include "WemoManager.h"
#include "CallbackFunction.h"

// prototypes
boolean connectWifi();

//on/off callbacks
void deviceOn();
void deviceOff();
void deviceStatus();

WemoManager wemoManager;
WemoSwitch *device = NULL;

void setup()
{
  Serial.begin(115200);
  M5.begin();
  Wire.begin();
  if(digitalRead(BUTTON_A_PIN) == 0) {
    Serial.println("Will Load menu binary");
    updateFromFS(SD);
    ESP.restart();
  }

  // Attempt to connect to Wifi network:
  // SSID and Password are set in LovyanLauncher.
  Serial.print("Connecting Wifi: ");
  WiFi.begin();
  while (WiFi.status() != WL_CONNECTED) {
    M5.Lcd.print(".");
    delay(500);
  }
  M5.Lcd.println("");
  M5.Lcd.println("WiFi connected");
  M5.Lcd.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  M5.Lcd.println(ip);

  wemoManager.begin();
  // Format: Alexa invocation name, local port no, on callback, off callback
  device = new WemoSwitch("M5Stack", 80, deviceOn, deviceOff, deviceStatus);
  wemoManager.addDevice(*device);

  delay(10);
}

void loop()
{
  wemoManager.serverLoop();
}

void deviceOn() {
  M5.Lcd.writecommand(ILI9341_DISPON);
  M5.Lcd.setBrightness(30);
}

void deviceOff() {
  M5.Lcd.writecommand(ILI9341_DISPOFF);
  M5.Lcd.setBrightness(0);
}

void deviceStatus() {

  String sts = device->getDeviceStatus();
  M5.Lcd.println("Called Status Check:" + sts);
}
