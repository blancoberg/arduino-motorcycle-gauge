/*
 *

 *
  ODOMETER

  load distance variable from memory    x
  load tripmeter from memory  x
  save variables after every x revolutions  x

  reset speed if nothing has changed for 2 seconds.

  save variables to EEPROM when > 5 min has passed and speed it lower than 10 km / h.   x
  that will give it enough time when stopping the bike to save any changed data.    x

  if onManget when speed==0, start speedCounter   x
    when speed >= 100 save time and run event     x


 */

namespace Odometer {

  double distance = 0.0; //0.0f;//EEPROM.readDouble(2);
  double tripMeter = 0.0;//EEPROM.readDouble(3);
  
  double accelerationTime = 0;
  
  int amountOfMagnets = 1;                          // amount of magnets used on the wheel
  int wheelDiameter = 70;                           // wheel diameter in mm
  double wheelCircumference = wheelDiameter * PI;
  int pinId = 0;
  double speed = 0;
  double lastTime = 0;
  double lastSaved = 0;
  boolean speedCounterStarted = false;
  double speedCounter = 0;
  boolean state = false;

  void EEPROM_writeDouble(int ee, double value)
  {
     byte* p = (byte*)(void*)&value;
     for (int i = 0; i < sizeof(value); i++)
         EEPROM.write(ee++, *p++);
  }
  
  double EEPROM_readDouble(int ee)
  {
     double value = 0.0;
     byte* p = (byte*)(void*)&value;
     for (int i = 0; i < sizeof(value); i++)
         *p++ = EEPROM.read(ee++);
     return value;
  }
  
  void draw(){
    //Serial.println(F("odometer.draw"));
  }

  void loadState(){

    
    distance = EEPROM_readDouble(0);
    tripMeter = EEPROM_readDouble(8);


    Serial.print(" distance =");
    Serial.println(String(round(distance)));

    Serial.print(" tripMeter =");
    Serial.println(String(round(tripMeter)));
    
    distance = isnan(distance) == true ? 0 : distance;
    tripMeter =  isnan(tripMeter) == true? 0 : tripMeter;
    
  }
  
  void saveState(){

      if(millis()-lastSaved > 1000){ // handle bouncing when power is switched off
        
        
        u8g2.setPowerSave(1);
        EEPROM_writeDouble(0,distance);
        EEPROM_writeDouble(8,tripMeter);
        lastSaved = millis();
      }
      
//    //EEPROM.writeDouble(2,Odometer::distance);
   // EEPROM.writeDouble(3,Odometer::tripMeter);
  }

  void resetSpeed(){
    if(speed != 0){
      speed = 0;
      Serial.println(F("reset speed"));
    }

  }

  void resetTrip(){
    Odometer::tripMeter = 0;
    //save();
  }

  void onMagnet(){

    

      // start 0-100km/h counter //
      if(speed == 0 && speedCounterStarted==false){
          speedCounter = micros();
          speedCounterStarted=true;
      }

      if(speedCounterStarted == true){
        accelerationTime = micros()-speedCounter;
      }

      if(speed >= 100 && speedCounterStarted == true){
        speedCounterStarted = false;
        accelerationTime = (micros()-speedCounter);
        //Serial.print(F("0-100 time:"));
        //Serial.println((micros()-speedCounter)/1000000);
      }

      double dist = (Odometer::wheelCircumference/100)/amountOfMagnets;
      Odometer::distance+=dist;
      Odometer::tripMeter+=dist;

      double timePassed = micros()-Odometer::lastTime;
      Odometer::speed = ((dist)/(timePassed/1000000) * 3600)/1000;
      //Serial.println(Odometer::speed);
      Odometer::lastTime = micros();
      state = true;

     


  }

  void init(int pin){


    


    pinId = pin;
    //Serial.print(F("odometer.init"));
    //Serial.println(Odometer::distance);
    pinMode(pinId,INPUT);
    attachInterrupt(digitalPinToInterrupt(pinId), onMagnet, FALLING);


  }

}
