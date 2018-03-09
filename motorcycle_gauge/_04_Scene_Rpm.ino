class SceneRpm : public AbstractScene{

  public:
    SceneRpm() : AbstractScene() {};
    ~SceneRpm() {};
    void draw();
    

  private:
    double counter = 0.0;
    
};



void SceneRpm::draw(){

  drawRpm();
  drawFuel();
  drawSpeed();
  drawInfo("rpm",String(round(Tachometer::rpm)));
}
