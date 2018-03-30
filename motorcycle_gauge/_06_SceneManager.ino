namespace SceneManager {

int counter = 0;
int battery = 0;
int pin = 0;
int powerPin = 0;
double timer = 0;
bool currentState = true;
int currentScene = 0;
bool interrupted = false;
unsigned long lastTime = millis();
unsigned long lastTimeFuel = millis();
int x = 0;



// SCENES

AbstractScene* sceneTrip = new SceneTrip();
AbstractScene* sceneTripTotal = new SceneTripTotal();
AbstractScene* sceneTimer = new SceneTimer();
AbstractScene* sceneRpm = new SceneRpm();

//AbstractScene* sceneBattery = new SceneBattery();

AbstractScene* scenes[4] = {sceneTrip, sceneTripTotal, sceneTimer, sceneRpm};

void loadState() {

  currentScene = EEPROM.read(16);
  currentScene = isnan(currentScene) == true || currentScene > 3 ? 0 : currentScene;

}

void saveState() {
  EEPROM.write(16, currentScene);
}

void next() {
  //Serial.println("next()");
  currentScene++;
  if (currentScene == 4)
    currentScene = 0;

  //Serial.print("new scene");
  //Serial.println(currentScene);
}

void action() {
  //Serial.println(F("action()");
  scenes[currentScene]->action();
}



void draw() {


  if (millis() - lastTime > 17) {


    //Serial.println("scenemanager.draw()");
    // update tachometer
    // just for graphics
    Tachometer::update();

    if (millis() - lastTimeFuel   > 1000) {

      //Batterymeter::update();
      Fuelmeter::update();
      lastTimeFuel = millis();
      if (interrupted == true) {
        interrupted = false;
        digitalWrite(LED_BUILTIN, HIGH);
      }


    }

    u8g2.clearBuffer();



    scenes[currentScene]->draw();


    // draw bitmap
    
    // read fuel level && battery



    u8g2.sendBuffer();

    lastTime = millis();
  }



}

void onButton() {
  //Serial.print("button ");
  counter++;
  double _time = millis() - timer;
  int state = digitalRead(SceneManager::pin);
  if (state == 0 && SceneManager::currentState == true) {
    timer = millis();
    SceneManager::currentState = false;
  }
  if (state == 1 && SceneManager::currentState == false) {

    //
    timer = millis();
    if (_time < 500 && _time > 50) {
      SceneManager::next();
    }
    if (_time > 500) {
      SceneManager::action();
    }

    SceneManager::currentState = true;
  }
}

void onPower() {
  //int v = digitalRead(powerPin);
  //Odometer::saveState();
  //SceneManager::saveState();
  //int power = digitalRead(powerPin);
  if (interrupted == false) {
    //Serial.print("LOW!!");
    digitalWrite(LED_BUILTIN, LOW);
    interrupted = true;
    Odometer::saveState();
    SceneManager::saveState();
  }
  //Serial.print("power=");
  //Serial.println(power);
}

void init ( int pin , int powerPin) {
  SceneManager::pin = pin;
  SceneManager::powerPin = powerPin;
  attachInterrupt(digitalPinToInterrupt(pin), onButton, CHANGE);
  attachInterrupt(digitalPinToInterrupt(powerPin), onPower, FALLING);
}



}
