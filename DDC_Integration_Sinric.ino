// The Sketch is developed and implemented by Sunil Pallikkara Balakrishnan
// https:cloudcontrol.tech
// sunil@cloudcontrol.tech
// Using Sinric.pro as the IoT cloud platform

//#define ENABLE_DEBUG

#ifdef ENABLE_DEBUG
#define DEBUG_ESP_PORT Serial
#define NODEBUG_WEBSOCKETS
#define NDEBUG
#endif

#include <Arduino.h>
#include <WiFi.h>

#include "SinricPro.h"

#include <DNSServer.h>
#include <WiFiManager.h>  //https://github.com/tzapu/WiFiManager
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define APP_KEY "7d4abec3-7596-4b2d-80bf-1d3680161a53"
#define APP_SECRET "e8a9e04c-abc5-4602-92fd-a7815f16f04d-6a318f30-d88b-420f-bae9-c90eb47b1475"

#define BAUD_RATE 115200
#define SAMPLE_EVERY_SEC 60

#include "Modbus.h"
#include "Blink.h"
#include "DDCStatus.h"
#include "DDCSensors.h"
#include "VControl.h"
#include "DDCWrite.h"

WiFiManager wm;

// Connection flags
volatile bool wifiConnected = false;
volatile bool sinricConnected = false;
volatile bool sinricnotConnected = false;

const int buttonPin = 0;
volatile unsigned long lastPressTime = 0;
volatile bool longPressTriggered = false;

void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");

  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP 

  wm.setConfigPortalBlocking(false);
  wm.setConfigPortalTimeout(120);

  //automatically connect using saved credentials if they exist
  //If connection fails it starts an access point with the specified name
    if(wm.autoConnect("CC-Integrator")){
        Serial.println("connected...:)");
    }

  WiFi.setSleep(false);
  WiFi.setAutoReconnect(true);
  //WiFi.begin(WIFI_SSID, WIFI_PASS);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 60) {
    Serial.printf(".");
    delay(500);
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;   
    Serial.printf("\nConnected!\n[WiFi]: IP-Address is %s\n", WiFi.localIP().toString().c_str());    
  } else {
    Serial.println("\nFailed to connect to WiFi!");    
  } 
}

void IRAM_ATTR handleInterrupt() {
  if (digitalRead(buttonPin) == LOW) {
    lastPressTime = millis();
  } else {
    // If released, check duration
    if (millis() - lastPressTime > 10000) {
      longPressTriggered = true;   
    }
  }
}

void ButtonSetup() {
  //Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), handleInterrupt, CHANGE);
}

void ButtonPress() { //Flash butto is configured for erasing the wifi settings
  if (longPressTriggered) {
    Serial.println("WiFi Erase Activation is detected");
    longPressTriggered = false;
    Serial.println("Erasing Config, restarting");
        wm.resetSettings();
        ESP.restart();
  }
}

void setupSinricPro() {
  // add device to SinricPro
  SinricProTemperaturesensor &HTSensor = SinricPro[TEMP_SENSOR_ID];
  SinricProSwitch& IN1Switch = SinricPro[Discrete_ID]; //To read discrete status
  SinricProSwitch& IN2Switch = SinricPro[Coil_ID]; // To read coil status
  IN2Switch.onPowerState(onPowerState); // To write to the same coil

    // PercentageController
  VControl.onSetPercentage(onSetPercentage);
  VControl.onAdjustPercentage(onAdjustPercentage);

    SinricPro.onConnected([]() {
    Serial.println("Connected to SinricPro");
  sinricConnected = true;
  });

  SinricPro.onDisconnected([]() {
    Serial.println("Disconnected from SinricPro");
    sinricnotConnected = true;
  });

  SinricPro.begin(APP_KEY, APP_SECRET);
}

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("\nStarting Remote Client for DDC");

  // Setup WiFi and SinricPro
  setupWiFi();
  setupSinricPro();
  ButtonSetup();
  ModbusSetup();  

 pinMode(13, OUTPUT);
 pinMode(5, OUTPUT);  
}

void loop() {
  SinricPro.handle();
  wm.process();
  ButtonPress();
  handleTemperaturesensor();
  handleDigitalStatus();
  handleCoilStatus();
  Modbusloop();
   
  unsigned long currentTime = millis();  
   if (wifiConnected){
    SlowBlink();    
   }
    if (WiFi.status() != WL_CONNECTED){
      FastBlink();
   }
  }
