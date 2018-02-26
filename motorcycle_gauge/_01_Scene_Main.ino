namespace SceneMain{

  double counter = 0.0;
  void draw(){
    
    
    u8g2.setDrawColor(1);
    
    u8g2.setFont(u8g2_font_inb27_mn  );
    
    String speed = String(int(Odometer::speed));
    //Serial.println(speed);
    int w = u8g2.getStrWidth(speed.c_str());
    
    u8g2.setCursor(128-w, 64);
    u8g2.print(speed);
    
    u8g2.setFont(u8g2_font_5x7_tr);
    u8g2.setCursor(128-w,32);
    u8g2.print("km/h");

    u8g2.setFont(u8g2_font_6x10_tn);
    String distance = String(Odometer::distance/1000);
    w = u8g2.getStrWidth(distance.c_str());
    u8g2.setCursor(128-w,16);
    u8g2.print(distance.c_str());


    /*
     * 
     * RPM GRAPHICS
     */
    

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
    
     /*
    // draw rpm meter //
    int totalPixels = 128+64;
    //float proc = (1.0+cos(counter))/2.0; //static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float proc = float(Tachometer::rpm)/ float(Tachometer::maxRPM);
    int pixels = min(proc * (totalPixels),64);
    u8g2.drawBox(0,64-pixels,5,  pixels   );


    
    float proc2 = max(0,proc - (64.0/totalPixels))/(128.0/totalPixels);
    u8g2.drawBox(5,0,round(128*proc2),5);

    
    //Serial.print(counter);

    //u8g2.drawXBM( 0, 0, u8g_logo_width, u8g_logo_height, u8g_logo_bits);
    //u8g2.drawXBM(0,0,zephyr_width,zephyr_height,zephyr_bits);
    */
    

    counter+=0.05;
    if(counter>PI*2)
      counter = 0;//counter-PI*2;
    
  }
}
