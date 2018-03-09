class SceneBattery : public AbstractScene{

  public:
    SceneBattery() : AbstractScene() {};
    ~SceneBattery() {};
    void draw();
    

  private:
    double counter = 0.0;
    
};



void SceneBattery::draw(){

  drawRpm();
  drawFuel();
  drawInfo("battery",String(Batterymeter::value) + "v");
}
