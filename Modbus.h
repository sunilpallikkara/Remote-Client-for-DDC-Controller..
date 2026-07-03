//Handling MODBUS communication call

#include <ModbusMaster.h>

// Define RS485 pins
#define MAX485_DE_RE 15
#define RX_PIN 16
#define TX_PIN 17

ModbusMaster node;

extern uint16_t DamperValue; 
extern uint16_t getPercentagedata(); // taking reading from DDCWrite

bool coilstate;
bool digitalstate;

uint16_t Iregdata;
uint16_t Hregdata;

uint16_t getIregdata(){
  return Iregdata;
}

uint16_t getHregdata(){
  return Hregdata;
}


// Callback to enable transmission
void preTransmission() {
  digitalWrite(MAX485_DE_RE, HIGH);
}

// Callback to enable reception
void postTransmission() {
  digitalWrite(MAX485_DE_RE, LOW);
}

void ModbusSetup() {
  //Serial.begin(115200);
  pinMode(MAX485_DE_RE, OUTPUT);
  digitalWrite(MAX485_DE_RE, LOW); // Start in receiving mode

  // Initialize Serial2 for Modbus (Slave ID 1)
  Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  node.begin(1, Serial2);

  // Register the flow control callbacks
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  //pinMode(13, OUTPUT);
  pinMode(5, OUTPUT);
}

  int coilReg = 1; 
  int disReg = 2;
  int tReg = 3;
  int hReg = 4;
  int DampReg = 5;

#define COIL_EVENT_WAIT_TIME   5000               // send event every 5 seconds
#define DISCRETE_EVENT_WAIT_TIME   5000           // send event every 5 seconds
#define HREG_EVENT_WAIT_TIME   3000               // send event every 5 seconds
#define IREG_EVENT_WAIT_TIME   3000               // send event every 5 seconds

void readCoil() {
 // Check and send details every 1 min.
  static unsigned long coillast_millis;
  unsigned long        coilcurrent_millis = millis();
  if (coillast_millis && coilcurrent_millis - coillast_millis < COIL_EVENT_WAIT_TIME) return;
  coillast_millis = coilcurrent_millis;

  // Read 1 coil starting at address 1
  uint8_t result = node.readCoils(coilReg, 1); //Modbus Function Code 0x01 to read coil.  
  if (result == node.ku8MBSuccess) {
    coilstate = node.getResponseBuffer(0); //Buffer to store Modbus Slave response; read getResponseBuffer()
    Serial.println(coilstate ? "ON" : "OFF");
  }
}

void WriteCoilON(){ //The control functioned is linked to DDCWrite.h tab
  uint8_t result = node.writeSingleCoil(coilReg, 1); //Fucntion Code 05
  if (result == node.ku8MBSuccess) {
    Serial.println("Success: 'ON' written to the Coil!");
  } else {
    Serial.print("Error: ");
    Serial.println(result, HEX);
  }
}

void WriteCoilOFF(){//The control functioned is linked to DDCWrite.h tab
  uint8_t result = node.writeSingleCoil(coilReg, 0); //Function Code 05/
  if (result == node.ku8MBSuccess) {
    Serial.println("Success: 'OFF' written to the Coil!");
  } else {
    Serial.print("Error: ");
    Serial.println(result, HEX);
  }
}

void readDiscrete() {
 // Check and send details every 1 min.
  static unsigned long discretelast_millis;
  unsigned long        discretecurrent_millis = millis();
  if (discretelast_millis && discretecurrent_millis - discretelast_millis < DISCRETE_EVENT_WAIT_TIME) return;
  discretelast_millis = discretecurrent_millis;
  uint8_t result;
  result = node.readDiscreteInputs(disReg, 1);   //Function code 02
  if (result == node.ku8MBSuccess) {
    digitalstate = node.getResponseBuffer(0); // 0 or 1
    Serial.println(digitalstate ? "ON" : "OFF");
  }
}
void readHreg() { //Read temperature from DDC
 // Check and send details every 1 min.
  static unsigned long hreglast_millis;
  unsigned long        hregcurrent_millis = millis();
  if (hreglast_millis && hregcurrent_millis - hreglast_millis < HREG_EVENT_WAIT_TIME) return;
  hreglast_millis = hregcurrent_millis;  
  // Read 1 holding register starting at address 0x0000
  uint8_t result = node.readHoldingRegisters(tReg, 1); //Function Code 03
  if (result == node.ku8MBSuccess) {
    Hregdata = node.getResponseBuffer(0);
    Serial.printf("Temp Register Value: %d\n", Hregdata);
  } else {
    Serial.printf("Error: 0x%02X\n", result);
  }   
}

void readIreg() {
 // Check and send details every 1 min.
  static unsigned long ireglast_millis;
  unsigned long        iregcurrent_millis = millis();
  if (ireglast_millis && iregcurrent_millis - ireglast_millis < IREG_EVENT_WAIT_TIME) return;
  ireglast_millis = iregcurrent_millis;  
  // Read 1 holding register starting at address 0x0000
  //result = node.readHoldingRegisters(3, 1);
  uint8_t result = node.readInputRegisters(hReg, 1); //Function Code 04
  if (result == node.ku8MBSuccess) {
    Iregdata = node.getResponseBuffer(0);
    Serial.printf("Humid Register Value: %d\n", Iregdata);
  } else {
    Serial.printf("Error: 0x%02X\n", result);
  }  
}

void WriteHreg() {  
    // Check and send details every 1 min.
  static unsigned long last_millis;
  unsigned long        current_millis = millis();
  if (last_millis && current_millis - last_millis < 30000) return;
  last_millis = current_millis;
  // Function 0x06: Write a single holding register
  // Parameters: (Address, State)
  uint8_t result = node.writeSingleRegister(DampReg, DamperValue); //Fucntion Code 06
  if (result == node.ku8MBSuccess) {
    Serial.println("Success: Holding Register written!");
    //Serial.println(result);
  } else {
    Serial.print("Error: ");
    Serial.println(result, HEX);
  }
}

void Modbusloop(){
  readCoil();
  readDiscrete();
  readHreg();
  readIreg();
  WriteHreg();
 }
 