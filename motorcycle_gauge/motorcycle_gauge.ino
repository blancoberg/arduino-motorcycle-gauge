/*
 * add speed sensor   x
 * add rpm sensor     x
 * add fuel sensor     x
 * complete SceneManager
 * complete all scenes with its actions
 * add time rtc
 * test all sensors on the bike
 * make drawing of the final board
 * attach wire connectors
 */


#include <U8g2lib.h>


//#ifdef U8X8_HAVE_HW_SPI
//#include <SPI.h>
//#endif
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// testa andra pins 

//#include <EEPROMex.h>

//U8G2_KS0108_128X64_F u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*dc=*/ 17, /*cs0=*/ 14, /*cs1=*/ 15, /*cs2=*/ U8X8_PIN_NONE, /* reset=*/  U8X8_PIN_NONE);   // Set R/W to low!
//U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);

//U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ PA0, /* data=*/ PA1, /* cs=*/ PA4, /* dc=*/ PA3, /* reset=*/ PA2); // stm32 software spi
U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ PA2, /* dc=*/ PA1, /* reset=*/ PA0); // arduino uno hardware

//U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 3, /* data=*/ 4, /* cs=*/ 7, /* dc=*/ 6, /* reset=*/ 5); // Arduino UNO


//U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 8, /* reset=*/ 9); // arduino uno hardware


namespace Fuelmeter{

  int fuelPin = 0;
  int controlPin = 0;
  int value = 0;
  int resFull = 10;
  int resEmpty = 100;
  void update(){

    digitalWrite(controlPin,HIGH);
    //delay(100);
    value = analogRead(fuelPin);
    
    double voltageOut = (value/4095.0) * 3.3;
    if(voltageOut==0)
      voltageOut= 0.001;
    double R1 = (3.3*50)/voltageOut - 50-30;

    double proc = min(1,max(0,1.0-((R1-resFull)/(resEmpty-resFull))));
    //double proc = (R1-resFull)/(resFull-resEmpty);
    Serial.print("fuel resistance:");
    Serial.println(R1);
    Serial.print("fuel procent");
    
    value = proc*100;
    Serial.println(value);
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
  int maxRPM = 5000;
  int redLine = 4000;
  int rpm = 0;
  double rpmSmooth = 0.0;
  double cosCounter = 0.0;
  boolean state = false;
  int coilPin = 0;
  void onCoilEvent(){

      
      
        int v = digitalRead(coilPin);

        if(v == HIGH && state == false){
          rpm = 1.0/ (   (micros()-lastTime)/1000000.0  ) * 60.0;
          lastTime = micros();
          state = true;
        }

        if(v == LOW && state == true){
          state = false;
        }
        //Serial.print("time:");
        //Serial.println(micros()-lastTime);
        
                                               

     
      //rpm = random(0,maxRPM);
  }

  void update(){

    //Serial.println(digitalRead(PB11));
      //Serial.println(cosCounter);
    //int v = digitalRead(PB12);
    //Serial.println(v);
    //cosCounter+=0.11;
    //if(cosCounter>PI*2)
      //cosCounter = cosCounter-PI*2;
  
    //rpm = (  (1+cos(cosCounter)  )/2) * maxRPM;
    rpmSmooth+=(rpm-rpmSmooth)/5.0;
    if(rpm> redLine){
      //double ledd = (   cos((micros()/1000000.0) * PI*15  )+1   )/2;
      //digitalWrite(12,round(ledd));
    }
    else{

       //digitalWrite(12,LOW);
    }

    if(micros()-lastTime>1000000){
      rpm = 0;
    }
    
    
  }
  
  void init(int pin){

    coilPin = pin;
    //pinMode(pin,INPUT);
    //pinMode(pin,INPUT);
    //attachInterrupt(pin, onCoilEvent, CHANGE);
  }

  
}













