//sketch for handling digital status
#define Discrete_ID       "6a1078f4c93c9fd0e0f9c0cc" //The device ID of Digital Status
#define Coil_ID           "6a0f47b0f9b5f15fa7db0b21" //The device ID of Coil Switch                       

#include "SinricProSwitch.h"  
                                             
bool IN1PowerState = false;
unsigned long IN1lastChange = 0;
\
bool IN2PowerState = false;
unsigned long IN2lastChange = 0;

void handleDigitalStatus() {
  
  unsigned long IN1actualMillis = millis();
  if (IN1actualMillis - IN1lastChange < 10000) return;          // debounce contact state transitions (same as debouncing a pushbutton)

  bool actualIN1State = digitalstate; //read actual state from the DDC
 
  if (actualIN1State != IN1PowerState) {         // if state has changed
    Serial.printf("Switch is %s now\r\n", actualIN1State?"ON":"OFF");
    IN1PowerState = actualIN1State;              // update last known state
    IN1lastChange = IN1actualMillis;                          // update debounce time
    SinricProSwitch& IN1Switch = SinricPro[Discrete_ID]; // get switch sensor device
    bool success = IN1Switch.sendPowerStateEvent(IN1PowerState);
      if(!success) {
      Serial.printf("Something went wrong...could not send Digital Status Event to the server!\r\n");
        }
  }
}

void handleCoilStatus() {

  unsigned long IN2actualMillis = millis();
  if (IN2actualMillis - IN2lastChange <10000 ) return;          

   bool actualIN2State = coilstate; //This is the status linked from Modbus.h Page
    
if (actualIN2State != IN2PowerState) {         // if state has changed
    Serial.printf("Switch is %s now\r\n", actualIN2State?"ON":"OFF");
    IN2PowerState = actualIN2State;              // update last known state
    IN2lastChange = IN2actualMillis;            // update debounce time
    SinricProSwitch& IN2Switch = SinricPro[Coil_ID]; // get switch sensor device
    bool success = IN2Switch.sendPowerStateEvent(IN2PowerState);
      if(!success) {
      Serial.printf("Something went wrong...could not send Coil Status Event to the server!\r\n");
    }
  }
  
}

