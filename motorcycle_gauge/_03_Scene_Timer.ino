class SceneTimer : public AbstractScene{

  public:
    SceneTimer() : AbstractScene() {};
    ~SceneTimer() {};
    void draw();
    

  private:
    double counter = 0.0;
    
};



void SceneTimer::draw(){

  drawRpm();
  drawFuel();
  drawSpeed();
  drawInfo("0-100",String(Odometer::accelerationTime/1000000));
}
