void loop() {
 
  // reset Odometer speed 
  if(micros()-Odometer::lastTime > 2000000){
    
    Odometer::resetSpeed();
  }

 
  //double seconds = millis();
  SceneManager::draw();
  //Serial.println(millis()-seconds);

  //delay(500);
  // Oscillate led 
  
  
  

}
