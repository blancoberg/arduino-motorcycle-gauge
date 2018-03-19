#include <EEPROM.h>
#include <U8g2lib.h>


#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

/*
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
*/


//U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ PA0, /* data=*/ PA1, /* cs=*/ PA4, /* dc=*/ PA3, /* reset=*/ PA2); // stm32 software spi
//U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 3, /* data=*/ 4, /* cs=*/ 7, /* dc=*/ 6, /* reset=*/ 5); // Arduino UNO
//U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 8, /* reset=*/ 9); // arduino uno hardware
U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ PA2, /* dc=*/ PA1, /* reset=*/ PA0); // STM32 hardware SPI


namespace Batterymeter{
  float value = 0;
  int batteryPin = 0;

  void update(){
    Batterymeter::value = (analogRead(batteryPin)/4095.0) * 3.3;
  }
  
  void init(int pin){
    pinMode(pin,INPUT);
    batteryPin = pin;
  }
}

namespace Fuelmeter{

  int fuelPin = 0;
  int controlPin = 0;
  int value = 0;
  int resFull = 10;
  int resEmpty = 100;
  void update(){

    digitalWrite(controlPin,HIGH);

    double v = analogRead(fuelPin);
    double voltageIn = 3.3;
    //value = analogRead(fuelPin);
    // vOut = R2/(R1+R2) * vIn
    double voltageOut = (v/4095.0) * 3.3;
    
    if(voltageOut==0)
      voltageOut= 0.001;
    //double R1 = (3.3*50)/voltageOut - 50;
    /*
     vOut/vIn = r2/(50+r2)
     (vOut/vIn)*(50+r2) = r2
     50(vOut/vIn)= 1r2 - (vOut/vIn)r2
     50(vOut/vIn)/(1-vOut/vIn) = r2
     */
    double R1 = 47+20; // R1 value + 20 ohm input resistance
    //double R2 = (47 * (voltageOut/voltageIn))/(1-(voltageOut/voltageIn));
    double R2 = (voltageOut*R1)/(voltageIn-voltageOut);
    
    double proc = min(1,max(0,1.0-((R2-resFull)/(resEmpty-resFull))));
    /*
    Serial.print("R1");
    Serial.println(R1);
    Serial.print("R2");
    Serial.println(R2);

    Serial.print("proc");
    Serial.println(proc*100);
    */
    value = proc*100;
    digitalWrite(controlPin,LOW);
  }
  
  void init(int pin,int pinControl){
    pinMode(pinControl,OUTPUT);
    fuelPin = pin;
    controlPin = pinControl;
  }
  
}


/*
 * TACHOMETER
 */

namespace Tachometer {

  unsigned long lastTime = micros();
  double maxRPM = 11000;
  double redLine = 10000;
  int rpm = 0;
  double rpmSmooth = 0.0;
  double cosCounter = 0.0;
  boolean state = false;
  int coilPin = 0;
  bool locked = false;
  int counter = 0;
  
  void onCoilEvent(){

      
            
          
          
       
            
        
        rpm = 1.0/ (   (micros()-lastTime)/1000000.0  ) * 60.0;
        lastTime = micros();
              
            
        
          
        
        
                                                 
            
          
      //rpm = random(0,maxRPM);
  }

  void update(){

    
    // reset rpm 
    if(micros()-lastTime > 500000){
      rpm = 0;
    }
    

    
    
    
  }
  
  void init(int pin){

    coilPin = pin;
    //pinMode(pin,DIGITAL);
    //pinMode(pin,INPUT);
    attachInterrupt(coilPin, onCoilEvent, RISING);
  }

  
}













