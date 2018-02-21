void setup() {
  
  //analogWrite(A2,LOW);
  //Wire.setClock(700000);
  Serial.begin(9600);

  // Initiate u8g2 
  u8g2.begin();
  
  // Initate components
  Odometer::init(PB13); // initate odometer on pin #3
  
  Tachometer::init(PB12); // iniate tachometer on pin #2
  //SceneManager::init(2); // iniate Navigation Manager on pin #3

  
}
