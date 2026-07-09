//Sketch for handling the sensors data from the DDC
#include "SinricProTemperaturesensor.h"
//#include "SinricProSwitch.h"

#define TEMP_SENSOR_ID    "6a0f46e1baa50bf9bf399860"

#define BUTTON_PIN 35   // GPIO for BUTTON (inverted: LOW = pressed, HIGH = released)
#define LED_PIN   5   // GPIO for LED

#define EVENT_WAIT_TIME   60000

float temperature;                            // actual temperature
float humidity;                               // actual humidity
float lastTemperature;                        // last known temperature (for compare)
float lastHumidity;                           // last known humidity (for compare)

unsigned long lastEvent = (EVENT_WAIT_TIME); 

void handleTemperaturesensor() {

  unsigned long actualMillis = millis();
  if (actualMillis - lastEvent < EVENT_WAIT_TIME) return; //only check every EVENT_WAIT_TIME milliseconds

  uint16_t Temp = getHregdata();
  uint16_t Humid = getIregdata();

  // Get Temperature
  temperature = Temp; 
  // Get Humidity
  humidity = Humid;

  SinricProTemperaturesensor &HTSensor = SinricPro[TEMP_SENSOR_ID];
  bool success = HTSensor.sendTemperatureEvent(temperature, humidity);

  if (success) {
    Serial.printf("Temperature: %2.1f Celsius\tHumidity: %2.1f%%\r\n", temperature, humidity);
  } else {
    Serial.printf("Something went wrong...could not send Temperature & Humidity Events to the server!\r\n");
  }
  lastTemperature = temperature;  // save actual temperature for next compare
  lastHumidity = humidity;        // save actual humidity for next compare
  lastEvent = actualMillis;       // save actual time for next compare
}
