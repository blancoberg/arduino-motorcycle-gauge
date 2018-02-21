namespace SceneMain{

  double counter = 0.0;
  void draw(){
    
    
    u8g2.clearBuffer();
    
    u8g2.setFont(u8g2_font_inb27_mn  );
    
    String speed = String(int(Odometer::speed));
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
    
    u8g2.sendBuffer();

    counter+=0.4;
    if(counter>PI*2)
      counter = counter-PI*2;
    
  }
}
