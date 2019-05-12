#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_FeatherOLED.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>


#define PIN 5 //neopixel

Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800); //neopixel

LiquidCrystal_I2C oled(0x27, 16, 2);
int result = 0;            //Used to store highest reading
int sensorPin = A0;        //Pin the sensor is plugged into (analog)
int sensorValue = 0;       //Value of sensor
int lowerLimit = 200;      //The lowest point you wish to begin reading data
int coolDown = 170;        //The highest value the sensor should return to before starting a new reading


void setup() {
strip.begin();// NeOPIXeL
strip.show(); // Initialize all pixels to 'off'  NeoPIXEL
Serial.begin (9600);            //Begin serial for output
pinMode (sensorPin, INPUT);     //Set pin as input
oled.init();
oled.backlight();
} 

void loop() {
sensorValue = analogRead(sensorPin);

if(sensorValue > lowerLimit){ 
  oled.clear();
  oled.setCursor(0,10);//If sensor has gone above the threshold begin reading
  oled.println("Breath Analyzing");
  
}

while(sensorValue > lowerLimit){        //While above threshold find highest value
  if(result < sensorValue){
    result=sensorValue;
   
  }
  sensorValue = analogRead(sensorPin);
}



if(result!=0){  
  
  float v = (result/10) * (5.0/1024.0);
  float mgL = 0.33 * v;
                              
  Serial.print("Result = ");
  Serial.println(result);

  if(result >= 200 && result < 280){
    oled.clear();
    oled.print("You are sober");
    oled.setCursor(0,10);
    oled.print("BAC = ");
    oled.print(mgL);
    theaterChaseSober(strip.Color(127, 127, 127), 50);//white
  }else if (result>=280 && result< 550)
  {
    oled.clear();
    oled.print("I smell booze");
    oled.setCursor(0,10);
    oled.print("BAC = ");
    oled.print(mgL);
    theaterChaseSober(strip.Color(0, 255, 0), 50);
   
  }
  else 
  {
    oled.clear();
    oled.print("You are drunk");
    oled.setCursor(0,10);
    oled.print("BAC = ");
    oled.print(mgL);
    drunk(strip.Color(255, 0, 0), 50); //reddrunk
  }

  
  while(sensorValue>coolDown){      //Wait to "cool" back down so we are ready for another reading
    delay(100);
    sensorValue = analogRead(sensorPin);
  }

  
  result=0;
  oled.clear();
  oled.print("Ready to go!");
   
}

delay(1000);

}


void theaterChaseSober(uint32_t c, uint8_t wait) {
  for (int j=0; j<30; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, strip.Color(0,0,0));        //turn every third pixel off
      }
    }
  }
 }


 void drunk(uint32_t c, uint8_t wait){
  for(int j =0; j< 300;j++){
  for (uint16_t i =0; i< strip.numPixels();i++){
           strip.setPixelColor(i, c);
           
  }
          strip.show();
           delay(wait);
           
            for (uint16_t i =0; i< strip.numPixels();i++){
           strip.setPixelColor(i, 0,0,0);
           
            }
           strip.show();
           delay(wait);


 }
 }
