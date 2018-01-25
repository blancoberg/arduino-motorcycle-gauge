#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include <EEPROMex.h>

U8G2_KS0108_128X64_F u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*dc=*/ 17, /*cs0=*/ 14, /*cs1=*/ 15, /*cs2=*/ U8X8_PIN_NONE, /* reset=*/  U8X8_PIN_NONE);   // Set R/W to low!

//#include <openGLCD.h>
/*
#include "Adafruit_HT1632.h"

#define HT_DATA 2
#define HT_WR   3
#define HT_CS   4
#define HT_CS2  5


// use this line for single matrix
Adafruit_HT1632LEDMatrix matrix = Adafruit_HT1632LEDMatrix(HT_DATA, HT_WR, HT_CS);
*/
/*
// draw sprite
// x, y, width of bitmap, bytes data
void drawSprite(unsigned char bytes[], int px, int py, int w) {

  for (int i = 0; i < sizeof(bytes) * 8; i++) {
    bool bit = getBit(bytes[i / 8], i % 8);
    if (bit == true) {
      matrix.setPixel(px + i % w, py + i / w);
    }
  }
}
//

// tweenio
//////////////////////////////
// Graphics for big numbers ///

bool getBit(unsigned char byte, int position) // position in range 0-7
{
  return (byte >> position) & 0x1;
}


class MatrixElement {
  public:
    MatrixElement();
    bool visible = true;
    virtual void render(int px, int py);
    virtual void addChild(MatrixElement* elem);
    int x = 0;
    int y = 0;
    int scramble = 1;
    MatrixElement * children[2];
  private:
    int childCount = 0;

};

MatrixElement::MatrixElement() {

}
void MatrixElement::addChild(MatrixElement* elem) {
  children[childCount] = elem;
  childCount++;
};

void MatrixElement::render(int px, int py) {

  for (int i  = 0; i < childCount; i++) {
    //(-scramble + (random() %(scramble*2)));
    children[i]->render(px + x , py + y);
  }
};

class Rectangle : public MatrixElement {
  public:
    int width = 9;
    int height = 10;
    bool fill = false;
    int border = 1;
    Rectangle() : MatrixElement() {};
    void render (int px, int py);
};


void Rectangle::render(int px, int py) {


  if (fill == true) {
    for (int i = 0; i < width - border * 2; i++) {
      for (int k = 0; k < height - border * 2; k++) {
        matrix.setPixel(px + x + i + border, py + y + k + border);
      }
    }
  } else {
    // draw border
    for (int i = 0; i < width; i++) {
      matrix.setPixel(px + x + i, py + y);
      matrix.setPixel(px + x + i, py + y + height - 1);
    }
    for (int i = 0; i < height; i++) {
      matrix.setPixel(px + x, py + y + i);
      matrix.setPixel(px + x + width - 1, py + y + i);
    }
  }

};

class Counter : public MatrixElement {
  public:
    int width = 9;
    int height = 10;
    bool fill = false;
    unsigned int value = 0;
    int border = 1;
    Counter() : MatrixElement() {};
    void render (int px, int py);
  private:
    void drawSifr(int value, int x, int y);

    // sprites for small characters ( 2 bytes per character , width:3)
    const unsigned char character_small[10][2] = {{111, 123}, {146, 36}, {231, 115}, {231, 121}, {237, 73}, {207, 121}, {207, 123}, {39, 73}, {239, 123}, {239, 73}};

};


void  Counter::render(int px, int py) {

  int count = 0;
  int digits[5];
  int number = value;
  while (number > 0)
  {
    int digit = number % 10;
    number /= 10;
    digits[count] = digit;

    count++;
  }
  count--;
  int pos = 0;
  while (count >= 0)
  {
    int digit = digits[count];
    drawSprite(character_small[digit], px + (pos) * 4, py, 3);
    count--;
    pos++;
  }


}
*/



namespace Fuelmeter{
  
  void init(int pin){
    
  }
  
}

namespace GearSelector{
  
}

/*
 * TACHOMETER
 */

namespace Tachometer {

  void onCoilEvent(){

      //Serial.println("coil");
  }
  
  void init(int pin){
    attachInterrupt(digitalPinToInterrupt(pin), onCoilEvent, HIGH);
  }

  
}

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

  double distance = EEPROM.readDouble(2);
  
  
    
  double tripMeter = EEPROM.readDouble(3);
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
    Serial.println("odometer.draw");
  }
  
  void save(){

    EEPROM.writeDouble(2,Odometer::distance);
    EEPROM.writeDouble(3,Odometer::tripMeter);
  }

  void resetSpeed(){
    if(speed != 0){
      speed = 0;
      Serial.println("reset speed");
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
        Serial.print("0-100 time:");
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
    Serial.print("odometer.init");
    Serial.println(Odometer::distance);
    pinMode(pinId,INPUT);
    attachInterrupt(digitalPinToInterrupt(pinId), onMagnet, CHANGE);

    
  }

}

/*
 * SCENES
 */

namespace SceneMain{

  double counter = 0.0;
  void draw(){
    
    
    u8g2.clearBuffer();
    
    u8g2.setFont(u8g2_font_inb27_mn  );
    String speed = String(int(Odometer::speed));
    int w = u8g2.getStrWidth(speed.c_str());
    
    u8g2.setCursor(128-w, 64);
    u8g2.print(speed);

    // draw rpm meter //
    int totalPixels = 128+64;
    float proc = (1.0+cos(counter))/2.0; //static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    
    int pixels = min(proc * (totalPixels),64);
    u8g2.drawBox(0,64-pixels,4,  pixels   );

    float proc2 = max(0,proc - (64.0/totalPixels))/(128.0/totalPixels);
    u8g2.drawBox(0,0,round(128*proc2),4);

    
    Serial.print(counter);
    
    u8g2.sendBuffer();

    counter+=0.4;
    if(counter>PI*2)
      counter = counter-PI*2;
    
  }
}



/*
 * 
 * SceneManager
 */

namespace SceneManager{

  int pin = 0;
  double timer = 0;
  int currentState = 1;
  int currentScene = 0;
  unsigned long lastTime = millis();
  void next(){
    Serial.println("next()");
  }

  void action(){
    Serial.println("action()");
  }
  
  void draw(){

    
    if(millis() - lastTime > 50){
      lastTime = millis();
      Serial.println("draw");
      switch(currentScene){
        case 0: 
          SceneMain::draw();  
      }
    
    }
  }

  void onButton(){
    Serial.print("button ");
    double _time = millis()-timer;
    int state = digitalRead(SceneManager::pin);
    if(state == 0 && SceneManager::currentState == 1){
      timer = millis();
      SceneManager::currentState=0;
    }
    if(state == 1 && SceneManager::currentState == 0){
      
      //Serial.print("time");
      //Serial.println(_time);
      timer = millis();
      if(_time < 500){
        SceneManager::next();
      }
      if(_time>500){
        SceneManager::action();
      }
      
      SceneManager::currentState = 1;
    }
  }
  
  void init ( int pin ){
    SceneManager::pin = pin;
    attachInterrupt(digitalPinToInterrupt(pin), onButton, CHANGE);
  }

  
  
}







//U8G2_KS0108_128X64_F u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7, /*enable=*/ 18, /*dc=*/ 17, /*cs0=*/ 15, /*cs1=*/ 16, /*cs2=*/ U8X8_PIN_NONE, /* reset=*/  U8X8_PIN_NONE);   // Set R/W to low!

//gText txtSpeed = gText(GLCD.Width-40, GLCD.CenterY, 8, 1, Messe);

void setup() {
  
  analogWrite(A2,LOW);
    
 // pinMode(6, INPUT);
  
  Serial.begin(9600);
  u8g2.begin();
  
  
  
  //txtSpeed.x = 0;
  Odometer::init(3); // initate odometer on pin #3
  //Tachometer::init(2); // iniate tachometer on pin #2
  SceneManager::init(2); // iniate Navigation Manager on pin #3

}


int val = HIGH;
//double lastTime = 0;
//double lastTimeCheck = 0;


void loop() {
 
  /*
  u8g2.clearBuffer();
  u8g2.drawBox(rand()%10,0,10,10);
  u8g2.sendBuffer();
  delay(200);
  */
  
  
  
  SceneManager::draw();
  //Serial.println(digitalRead(3));
  
  // reset Odometer speed 
  if(micros()-Odometer::lastTime > 2000000){
    
    Odometer::resetSpeed();
  }
  
  //int tempVal = digitalRead(3);
  //Serial.println(tempVal);
  //int tempCoil = analogreadad(7);
  /*
  if (val != tempVal && tempVal == LOW ) {

    // ( 1m / (milliseconds/1000) ) * 3.6 = hastighet km/h
    float speed = (1.0 / ((millis()-lastTime)/1000.0)) * 3.6;
    Serial.println(speed);
    //Serial.println(millis()-lastTime);
    //Serial.println((millis()-lastTime));
    lastTime = millis();
    lastTimeCheck = lastTime;
    val = tempVal;
  }

  if(val != tempVal && tempVal == HIGH){
    lastTimeCheck = millis();
    val = tempVal;
  }
  */
  //float val = analogRead(0);     // read the input pin
  //Serial.println(val);  
  
  

  
  

  //Serial.println(tempVal);
  
  /*
    matrix.clearScreen();

    counter.value+=1;
    // draw elements //
    stage.render(0, 0);
    for (int i = 0; i < 50; i++) {
    //matrix.setPixel(random() % 24, random() % 16);
    }
    matrix.writeScreen();


    delay(20);
  */

}



