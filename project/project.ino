int measurePin = A5;
int ledPower = 12;

unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__SOFTSERIAL
#include <SoftwareSerial.h>

#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_SERIAL_RX 2
#define REMOTEXY_SERIAL_TX 3
#define REMOTEXY_SERIAL_SPEED 9600


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,1,0,33,0,137,0,10,13,1,
  129,0,1,49,21,4,17,82,97,119,
  32,83,105,103,110,97,108,0,129,0,
  1,59,18,5,17,86,111,108,116,97,
  103,101,0,129,0,1,70,24,4,17,
  68,117,115,116,32,68,101,110,115,105,
  116,121,0,129,0,2,4,18,6,79,
  83,116,97,121,32,83,97,102,101,0,
  129,0,8,16,47,9,24,65,105,114,
  32,80,117,114,105,102,105,101,114,0,
  67,4,29,47,23,7,2,26,11,67,
  4,29,69,23,7,2,26,11,2,0,
  19,30,21,9,2,26,31,31,79,78,
  0,79,70,70,0,67,4,29,58,23,
  7,2,26,11 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t switch_1; // =1 if switch ON and =0 if OFF 

    // output variables
  char text_signal[11];  // string UTF8 end zero 
  char text_dust[11];  // string UTF8 end zero 
  char text_volt[11];  // string UTF8 end zero 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////
#define PIN_SWITCH_1 11

void setup() 
{
  RemoteXY_Init (); 
  pinMode (PIN_SWITCH_1, OUTPUT);
  pinMode(ledPower,OUTPUT);
}

void loop() 
{ RemoteXY_Handler ();
  digitalWrite(PIN_SWITCH_1, (RemoteXY.switch_1==1)?LOW:HIGH);
  digitalWrite(ledPower,LOW);
  delayMicroseconds(samplingTime);
  
  voMeasured = analogRead(measurePin);
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH);
  delayMicroseconds(sleepTime);
  
  calcVoltage = voMeasured*(5.0/1024);
  dustDensity = 0.17*calcVoltage-0.1;

  if ( dustDensity < 0)
  {dustDensity = 0.00;}
  
 dtostrf(voMeasured, 0, 1, RemoteXY.text_signal);
 dtostrf(calcVoltage, 0, 1, RemoteXY.text_volt);
 dtostrf(dustDensity, 0, 1, RemoteXY.text_dust);
 delay(1000);
}
