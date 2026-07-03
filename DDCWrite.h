//Handling writing of data to coil and holding register.

#define DAMPER_ID  "6a14746bc93c9fd0e0fca593" 

VControl &VControl = SinricPro[DAMPER_ID]; //This is for sending analog Votage 0-10V to the DDC controller

/*************
 * Variables *
 ***********************************************
 * Global variables to store the device states *
 ***********************************************/

// PercentageController
int globalPercentage;
bool globalPowerState;

uint16_t DamperValue = 0;
uint16_t getPercentagedata(){ //Share the values of DamperValue to Modbus.h
  return DamperValue;
}

/*************
 * Callbacks *
 *************/

// PercentageController
bool onSetPercentage(const String &deviceId, int &percentage) {
  Serial.printf("[Device: %s]: Percentage level set to %d\r\n", deviceId.c_str(), percentage);
  globalPercentage = percentage;
  DamperValue = percentage;
  Serial.println(DamperValue);
  return true; // request handled properly
}

bool onAdjustPercentage(const String &deviceId, int &percentageDelta) {
  globalPercentage += percentageDelta; // calculate absolute power level
  Serial.printf("[Device: %s]: Percentage level changed about %i to %d\r\n", deviceId.c_str(), percentageDelta, globalPercentage);
  percentageDelta = globalPercentage; // return absolute power level
  Serial.println(percentageDelta);
  return true;                   // request handled properly
}

// PercentageController
void updatePercentage(int percentage) {
  VControl.sendSetPercentageEvent(percentage); 
}

// TO WRITE ON & OFF TO THE COIL
bool onPowerState(const String &deviceId, bool &state) {
  Serial.printf("Output %s turned %s\r\n", deviceId.c_str(), state?"on":"off");
  globalPowerState = state; 
   if (state){
  WriteCoilON();
  digitalWrite(5, HIGH); 
  }
  else{
  WriteCoilOFF();
  digitalWrite(5, LOW); 
  }     
  return true; // request handled properly
}

