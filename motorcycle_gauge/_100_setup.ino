void setup() {
  
  //analogWrite(A2,LOW);
  //Wire.setClock(700000);
  EEPROM.init();
  Serial.begin(9600);
  
  // Initiate u8g2 
  u8g2.begin();

  SceneManager::loadState();
  Odometer::loadState();

  
  
  
  // Initate components
  
  //Batterymeter::init(PB1); // iniate tachometer on pin #2
  Tachometer::init(PB12); // iniate tachometer on pin #2
  Odometer::init(PB13); // initate odometer on pin #3
  Fuelmeter::init(PA3,PB14);
  SceneManager::init(PB15,PA8); // iniate Navigation Manager on pin #3


  // animate //
  /*
  for(float i = 0;i<100;i++){
    float rpm = Tachometer::maxRPM*0.5 - sin(PI*0.5 + (i/100)*PI*2) * Tachometer::maxRPM*0.5;
    float speed = 100 - sin(PI*0.5 + (i/100)*PI*2) * 100;
    Tachometer::rpm = rpm;
    Odometer::speed = speed;
    //SceneManager::lastTime = millis()+20;
    SceneManager::draw();
    delay(10);
  }
  */
  


  
  
}
