/****************************************************************************************
 * 
 * @file        ESP32_CO2_TrafficLight_With_LED.ino
 * @author      captain-maramo
 * @brief       Traffic light with ESP32, RGB LED and CCS811 Sensor with I2C comunication
 *
 *
 * @copyright   [MIT License](http://opensource.org/licenses/MIT)
 ****************************************************************************************/

#include "CCS811-SOLDERED.h"
#include "Wire.h"

#define PIN_RED    16
#define PIN_GREEN  17
#define PIN_BLUE   18

CCS_811 ccs811Sensor;

bool isFirstRun = true;

void setup()
{
  setPinModes();
  Wire.begin();
  Serial.begin(115200);
  ccs811Sensor.begin();
}

void loop()
{
  if(isFirstRun){
    initializeFirstRun();
    isFirstRun = false;
  }
  if (ccs811Sensor.dataAvailable())
  {
    int co2 = getCo2Value(ccs811Sensor);
    printCo2ToSerial(co2);
    setLedByCo2(co2);
    delay(5000); // Don't spam the I2C bus
  }
}

int getCo2Value(CCS_811 ccs811Sensor){
  ccs811Sensor.readAlgorithmResults();
  return ccs811Sensor.getCO2();
}

void printCo2ToSerial(int co2){
  Serial.print("CO2["+String(co2)+"]");
}

void setLedByCo2(int co2){
  if(co2<=999){
    // Green
    setLedColor(0, 255, 0);
    printColorToSerial(0, 255, 0);
  }
  else if(co2>=1600){
    // Red
    setLedColor(255, 0, 0);
    printColorToSerial(255, 0, 0);
  }
  else{
    // Yellow
    setLedColor(255, 155, 0);
    printColorToSerial(255, 155, 0);
  }
}

//20 min warum up phase for the sensor to give correct data
void initializeFirstRun(){
  for(int i = 1; i <= 120; i++){
    setLedColor(0,0,255);
    delay(5000);
    setLedColor(0,125,255);
    delay(5000);
    Serial.println("Warmup round: " + String(i) + "/1200");
  }
}

void setLedColor(int R, int G, int B) {
  analogWrite(PIN_RED,   R);
  analogWrite(PIN_GREEN, G);
  analogWrite(PIN_BLUE,  B);
}

void printColorToSerial(int R, int G, int B){
  String red = String(R);
  String green = String(G);
  String blue = String(B);
  Serial.println("Red: " + red +" Green: " + green + " Blue: " + blue);
}

void setPinModes(){
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);
}
