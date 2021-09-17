void Init(){
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("====================");
    lcd.setCursor(7, 1);
    lcd.print("ESP32");  
    lcd.setCursor(4, 2);
    lcd.print("Duco Monitor"); 
    lcd.setCursor(0, 3);
    lcd.print("====================");
    return;
}

void connecting(){
      lcd.clear();
      lcd.setCursor(5, 1);
      lcd.print("Connecting");  
      for(int i=5; i<=15;i++){
          lcd.createChar(0, p1);
          lcd.createChar(1, p2);
          lcd.createChar(2, p3);
        switch (i){
          case 5:
          lcd.setCursor(5,2);
          lcd.write(0);  
          break;
          case 6:
          lcd.setCursor(6,2);
          lcd.write(1);  
          break;
          case 7:
          lcd.setCursor(7,2);
          lcd.write(1);  
          break;
          case 8:
          lcd.setCursor(8,2);
          lcd.write(1);  
          break;
          case 9:
          lcd.setCursor(9,2);
          lcd.write(1);  
          break;
          case 10:
          lcd.setCursor(10,2);
          lcd.write(1);  
          break;
          case 11:
          lcd.setCursor(11,2);
          lcd.write(1);  
          break;
          case 12:
          lcd.setCursor(12,2);
          lcd.write(1);  
          break;
          case 13:
          lcd.setCursor(13,2);
          lcd.write(1); 
          case 14:
          lcd.setCursor(14,2);
          lcd.write(2); 
          break;
        }
        delay(500);
    }
    return;
}

void Connected(){
   lcd.clear();
   lcd.setCursor(0,1);
   lcd.print(WiFi.localIP());
}
