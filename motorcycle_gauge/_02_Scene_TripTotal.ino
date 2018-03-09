class SceneTripTotal : public AbstractScene{

  public:
    SceneTripTotal() : AbstractScene() {};
    ~SceneTripTotal() {};
    void draw();

  private:
    double counter = 0.0;
    
};

void SceneTripTotal::draw(){

  drawRpm();
  drawFuel();
  drawSpeed();
  drawInfo("total",String(Odometer::distance/1000));
  
}
