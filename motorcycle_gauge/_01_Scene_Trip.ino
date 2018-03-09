static unsigned char fuel_bits[] = {0xe0, 0x0e, 0x6e, 0x60, 0x60, 0xe0, 0xe0 };
//static uint8_t font_small[] = u8g2_font_6x10_tr;

class AbstractScene{
  public:
    
    virtual void next();
    virtual void action();
    virtual void draw();
    virtual void drawRpm();
    virtual void drawFuel();
    virtual void drawSpeed();
    void drawInfo(String header,String body);
   private:
    double counter = 0;
};

void AbstractScene::drawInfo(String header,String body){
  //Serial.println(header);
  //Serial.println(body);
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_micro_mr);
  
  u8g2.setCursor(0, 25);
  u8g2.println(header);

  u8g2.setFont(u8g2_font_victoriabold8_8n);
  u8g2.setCursor(0, 35);
  u8g2.println(body);
}

void AbstractScene::drawFuel(){
  u8g2.setDrawColor(0);
  u8g2.drawXBM( 0, 0, 8, 7, fuel_bits);
  u8g2.setFont(u8g2_font_micro_mr);
  u8g2.setCursor(10, 7);
  u8g2.setDrawColor(1);
  u8g2.print(String(Fuelmeter::value));
}

void AbstractScene::drawSpeed(){

  u8g2.setFont(u8g2_font_inb33_mn  );
    
  String speed = String(int(Odometer::speed));
  int w = u8g2.getStrWidth(speed.c_str());
    
  u8g2.setCursor(128-w, 64);
  u8g2.print(speed);
  
}

void AbstractScene::drawRpm(){

  
  u8g2.setDrawColor(1);


    u8g2.setFont(u8g2_font_micro_mr);
    u8g2.setCursor(113,25);
    //u8g2.print("km/h");
    u8g2.setFont(u8g2_font_inb33_mn  );
    
    String speed = String(int(Odometer::speed));
    int w = u8g2.getStrWidth(speed.c_str());
    
    u8g2.setCursor(128-w, 64);
    //u8g2.print(speed);

    double redLineStep = (Tachometer::redLine)/1000;
    double rpm = Tachometer::rpm;//Tachometer::maxRPM/2 + Tachometer::maxRPM*0.5 * cos(counter);
    double maxRpm = Tachometer::maxRPM;
    double brickSize = round(128.0/(maxRpm/1000.0)-1);
    double bricks = rpm/1000;
    int totalBricks = maxRpm/1000.0;
    
    for(float i = 0.0;i<totalBricks;i+=1.0){

      double proc = min(1.0,(rpm - (1000.0 * i))/1000.0);

      
      if((rpm> (maxRpm)/1000.0 * (i)) && (i < bricks)){

        
        //draw boxes //
        double maximumRpm = 1000*i + proc*1000;

        if(i >= redLineStep){
          // draw redline boxes //
          //u8g2.setDrawColor(((counter*12.3) % 1) < 0.5 ? 0 : 1);
          u8g2.setDrawColor(     round(   0.5+cos(counter*43.2)*0.5    )     );
        }else{
          u8g2.setDrawColor(1);

        }

        u8g2.drawBox((brickSize+1) * i,11-i,round((brickSize)*proc),2+i);


      }else{

        // draw markings
        u8g2.drawVLine((brickSize+1) * i-1,13,1);

      }

          
    }
    
    
    

    counter+=0.05;
    if(counter>PI*2)
      counter = 0;//counter-PI*2;
      
  /*
    u8g2.setFont(u8g2_font_6x10_tn);
    String distance = String(Odometer::distance/1000);
    w = u8g2.getStrWidth(distance.c_str());
    u8g2.setCursor(0,25);
    u8g2.print(distance.c_str());
    */
}



void AbstractScene::draw(){
  
}

void AbstractScene::next(){
  
}

void AbstractScene::action(){
  
}

class SceneTrip : public AbstractScene{

  public:
    SceneTrip() : AbstractScene() {};
    ~SceneTrip() {};
    void draw();
    void action();

  private:
    double counter = 0.0;
    
};

void SceneTrip::action(){
  Odometer::tripMeter = 0;
}
void SceneTrip::draw(){

  drawRpm();
  drawFuel();
  drawSpeed();
  drawInfo("trip",String(Odometer::tripMeter/1000));
}
  



/*
namespace SceneMain{

  double counter = 0.0;
  void draw(){
    
    
    u8g2.setDrawColor(1);
    
    u8g2.setFont(u8g2_font_inb33_mn  );
    
    String speed = String(int(Odometer::speed));
    int w = u8g2.getStrWidth(speed.c_str());
    
    u8g2.setCursor(128-w, 64);
    u8g2.print(speed);
    
    
    u8g2.setFont(u8g2_font_6x10_tn);
    String distance = String(Odometer::distance/1000);
    w = u8g2.getStrWidth(distance.c_str());
    u8g2.setCursor(0,25);
    u8g2.print(distance.c_str());


   
    

    double redLineStep = (Tachometer::redLine)/1000;
    double rpm = Tachometer::maxRPM/2 + Tachometer::maxRPM*0.5 * cos(counter);
    double maxRpm = Tachometer::maxRPM;
    double brickSize = round(128.0/(maxRpm/1000.0)-1);
    double bricks = rpm/1000;
    int totalBricks = maxRpm/1000.0;

    for(float i = 0.0;i<totalBricks;i+=1.0){

      double proc = min(1.0,(rpm - (1000.0 * i))/1000.0);

      
      if((rpm> (maxRpm)/1000.0 * (i)) && (i < bricks)){

        
        //draw boxes //
        double maximumRpm = 1000*i + proc*1000;

        if(i >= redLineStep){
          // draw redline boxes //
          //u8g2.setDrawColor(((counter*12.3) % 1) < 0.5 ? 0 : 1);
          u8g2.setDrawColor(     round(   0.5+cos(counter*43.2)*0.5    )     );
        }else{
          u8g2.setDrawColor(1);

        }

        u8g2.drawBox((brickSize+1) * i,11-i,round((brickSize)*proc),2+i);


      }else{

        // draw markings
        u8g2.drawVLine((brickSize+1) * i-1,13,1);

      }

          
    }
    
    
    

    counter+=0.05;
    if(counter>PI*2)
      counter = 0;//counter-PI*2;
    
  }
}
*/

