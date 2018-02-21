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

  double distance = 50000000.0;//EEPROM.readDouble(2);


  
  double tripMeter = 0.0;//EEPROM.readDouble(3);
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

  void draw(){
    Serial.println(F("odometer.draw"));
  }

  void save(){

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
    save();
  }

  void onMagnet(){

    int st = digitalRead(pinId);
    if(st == LOW && state == false){

      // start 0-100km/h counter //
      if(speed == 0 && speedCounterStarted==false){
          speedCounter = micros();
          speedCounterStarted=true;
      }

      if(speed >= 100 && speedCounterStarted == true){
        speedCounterStarted = false;
        Serial.print(F("0-100 time:"));
        Serial.println((micros()-speedCounter)/1000000);
      }

      double dist = (Odometer::wheelCircumference/100)/amountOfMagnets;
      Odometer::distance+=dist;
      Odometer::tripMeter+=dist;

      double timePassed = micros()-Odometer::lastTime;
      Odometer::speed = ((dist)/(timePassed/1000000) * 3600)/1000;
      Serial.println(Odometer::speed);
      Odometer::lastTime = micros();
      state = true;

      // save tripmeter data if time since last saved is more
      // than 5 minutes and speed is less than 10km/h.

      if(micros()-lastSaved > 300000000 && speed < 10){

        lastSaved = micros();
        save();
      }

    }

    if(st == HIGH && state == true){

      state = false;
    }


  }

  void init(int pin){


    if(distance != distance || tripMeter != tripMeter){    // if NaN

      tripMeter = 0;
      distance = 54000000;
    };


    pinId = pin;
    Serial.print(F("odometer.init"));
    Serial.println(Odometer::distance);
    pinMode(pinId,INPUT);
    attachInterrupt(digitalPinToInterrupt(pinId), onMagnet, CHANGE);


  }

}
