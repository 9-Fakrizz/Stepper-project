#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

EEPROMClass  SPEED("eeprom0");
EEPROMClass  CYCLE("eeprom1");
EEPROMClass  LAST_SPEED("eeprom2");#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

EEPROMClass  SPEED("eeprom0");
EEPROMClass  CYCLE("eeprom1");
EEPROMClass  LAST_SPEED("eeprom2");

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   23
#define OLED_CLK   14
#define OLED_DC    12
#define OLED_CS    13
#define OLED_RESET 5
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define button1 19
#define button2 25
#define button3 26
#define button4 16
#define button5 27

const int stepPin = 17; 
const int dirPin = 18;  //mechanical pull-low

int dlay ;
int last_dlay;
uint16_t last_spd;
int but1,but2,but3,but4,but5;
uint16_t spd ;
uint16_t cycle ;
bool task = false;
bool wait = false;
bool manual_mode_state = false;

void ReadButtons(){
     but1 = digitalRead(button1);
     but2 = digitalRead(button2);
     but3 = digitalRead(button3);
     but4 = digitalRead(button4);
     but5 = digitalRead(button5);  
}

void PrintButtons(){
     Serial.print("but1 : ");
     Serial.print(but1);
     Serial.print("  but2 : ");
     Serial.print(but2);
     Serial.print("  but3 : ");
     Serial.print(but3);
     Serial.print("  but4 : ");
     Serial.print(but4);
     Serial.print("  but5 : ");
     Serial.println(but5);
}

void set(){
   
     
     but1 = digitalRead(button1);
     but2 = digitalRead(button2);
     but3 = digitalRead(button3);
     but4 = digitalRead(button4);
     but5 = digitalRead(button5);
     
     display.display();
     display.clearDisplay();

     
     display.setTextSize(1);  
     display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
     display.setCursor(1,1);
     display.print(F("       Property      "));

     
     display.setTextSize(1);  
     display.setTextColor(SSD1306_WHITE);
     display.setCursor(15,20);
     display.print(F("Speeds"));
     display.setCursor(25,35);
     display.setTextSize(2); 
     display.print(spd);

     display.setTextSize(1);  
     display.setCursor(80,20);
     display.print(F("Cycle"));
     display.setCursor(90,35);
     display.setTextSize(2); 
     display.print(cycle);

     if(but1 == LOW){
       if(spd< 9) spd++;
       //else spd = 0;
      }
     if(but2 == LOW){
       if(spd!=0) spd--;
       if(spd == 0){
          manual_mode_state = true;
       }
      }
     
     if(but3 == LOW){
       if(cycle< 12) cycle++;
       else cycle = 1;
      
      }
     if(but4 == LOW){
       if(cycle!=1) cycle--;
       else cycle = 1;
      }

     if(cycle == 0)cycle =1;
     
     if(but5 == LOW ){
        task = true;
      }
       
  }

void manual_mode(){ /////////////////////////////// Manual MODE ////////////////
     
     but1 = digitalRead(button1);
     but2 = digitalRead(button2);
     but3 = digitalRead(button3);
     but4 = digitalRead(button4);
     but5 = digitalRead(button5);
     
     display.display();
     display.clearDisplay();
     display.setTextSize(1);  
     display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
     display.setCursor(1,1);
     display.print(F("    MANUAL MODE     "));

     display.setTextSize(1);  
     display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
     display.setCursor(1,50);
     display.print(F(" (BUTTON3) EXIT    "));

     
     display.setTextSize(1);  
     display.setTextColor(SSD1306_WHITE);
     display.setCursor(0,20);
     display.print(F("(BUTTN1)"));
     display.setCursor(3,30);
     display.setTextSize(1); 
     display.print(F("+Step"));

     display.setTextSize(1);  
     display.setCursor(60,20);
     display.print(F("(BUTTN2)"));
     display.setCursor(60,30);
     display.setTextSize(1); 
     display.print(F("LST-SPD:"));
     display.print(last_spd);

     if(but1 == LOW){
       digitalWrite(dirPin,LOW);
       digitalWrite(stepPin,HIGH);
       delayMicroseconds(1000);  
       digitalWrite(stepPin,LOW); 
       delayMicroseconds(1000);
      }
     if(but2 == LOW){
        delay(100);
        if(last_spd < 6){last_spd +=1;}
        else {last_spd = 1;}
      }
     
     if(but3 == LOW){
         manual_mode_state = false;
         spd = 1;
      }
}

void setup(){

  Serial.begin(115200);
  
  Serial.println("Testing EEPROMClass\n");
  if (!SPEED.begin(sizeof(spd))) {
    Serial.println("Failed to initialise SPEED");
    Serial.println("Restarting...");
    delay(1000);
    ESP.restart();
  }
  if (!CYCLE.begin(sizeof(cycle))) {
    Serial.println("Failed to initialise CYCLE");
    Serial.println("Restarting...");
    delay(1000);
    ESP.restart();
  }

   if (!LAST_SPEED.begin(sizeof(last_spd))) {
    Serial.println("Failed to initialise LAST_SPEED");
    Serial.println("Restarting...");
    delay(1000);
    ESP.restart();
  }
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();
  display.clearDisplay();

  pinMode(stepPin,OUTPUT);
  //pinMode(dirPin,OUTPUT);  
  pinMode(button1,INPUT);
  pinMode(button2,INPUT);
  pinMode(button3,INPUT);
  pinMode(button4,INPUT);
  pinMode(button5,INPUT);
  digitalWrite(dirPin,LOW);

  SPEED.get(0, spd);
  SPEED.commit();
  delay(50);
  
  CYCLE.get(0, cycle);
  CYCLE.commit();
  delay(100);
  
  LAST_SPEED.get(0,last_spd);
  LAST_SPEED.commit();
  delay(50);

  Serial.print("spd: ");   Serial.println(spd);
  Serial.print("cycle: "); Serial.println(cycle);
  Serial.print("LAST_SPD: "); Serial.println(last_spd);
  Serial.println("------------------------------------\n");
  
  if(spd > 10 || cycle > 12 || last_spd > 10){
    spd = 1;
    cycle = 1;
    last_spd = 1;
    SPEED.put(0, spd);
    SPEED.commit();
    LAST_SPEED.put(0, last_spd);
    LAST_SPEED.commit();
    CYCLE.put(0, cycle);
    CYCLE.commit();

    SPEED.get(0, spd);
    SPEED.commit();
    delay(50);
    
    CYCLE.get(0, cycle);
    CYCLE.commit();
    delay(100);
    
    LAST_SPEED.get(0,last_spd);
    LAST_SPEED.commit();
    delay(50);
    
  }
  //spd = 1;
  //cycle = 1;
  //last_spd = 1;
  //SPEED.put(0, spd);
  //SPEED.commit();
  //LAST_SPEED.put(0, last_spd);
  //LAST_SPEED.commit();
  //CYCLE.put(0, cycle);
  //CYCLE.commit();

//  SPEED.get(0, spd);
//  LAST_SPEED.get(0, last_spd);
//  CYCLE.get(0, cycle);
  
  Serial.print("spd: ");   Serial.println(spd);
  Serial.print("cycle: "); Serial.println(cycle);
  Serial.print("LAST_SPD: "); Serial.println(last_spd);
  Serial.println("------------------------------------\n");

}
     
void loop() { 

//  if(spd > 10 || cycle > 12 || last_spd > 10){
//    spd = 1;
//    cycle = 1;
//    last_spd = 1;
//    SPEED.put(0, spd);
//    SPEED.commit();
//    LAST_SPEED.put(0, last_spd);
//    LAST_SPEED.commit();
//    CYCLE.put(0, cycle);
//    CYCLE.commit();
//
//    SPEED.get(0, spd);
//    LAST_SPEED.get(0, last_spd);
//    CYCLE.get(0, cycle);   
//  }
 ReadButtons();
 //PrintButtons(); //   Uncomment this to check Buttons State via Serial Monitor !
 //Serial.println("manual_mode_state : " + String(manual_mode_state));
 //////////////////////////////////// Check Manual Mode ///////////////////////////////
 while(manual_mode_state == true)manual_mode();
 
 /////////////////////////////////// Setting Functions //////////////////////////////////
 while(task == false && manual_mode_state == false){
    set();
    delay(120) ;
  } 
  ///////////////////////////// set complete //////////////////////////////
 dlay = 550 -(spd * 50);  
 last_dlay = 1400 - (last_spd * 200);

//  Serial.println("after set :");
//  Serial.print("spd: ");   Serial.println(spd);
//  Serial.print("cycle: "); Serial.println(cycle);
//  Serial.print("LAST_SPD: "); Serial.println(last_spd);
//  Serial.println("------------------------------------\n");
 ///////////////////////////////////////// Start Spin ///////////////////////////
  if(wait == false && task == true){
             
        for(int i =0;i<800*(cycle-1) ;i++){             
          digitalWrite(stepPin,HIGH); 
          delayMicroseconds(dlay); 
          digitalWrite(stepPin,LOW); 
          delayMicroseconds(dlay);         
        }
      
        for(int i =0;i<400 ;i++){           
          digitalWrite(stepPin,HIGH); 
          delayMicroseconds(dlay); 
          digitalWrite(stepPin,LOW); 
          delayMicroseconds(dlay);         
        }
       
    
     ///////////////////////////////////////// Last spin (Slow speed) ///////////////
        for(int i =0;i<400;i++){
          digitalWrite(stepPin,HIGH);
          delayMicroseconds(last_dlay);  
          digitalWrite(stepPin,LOW); 
          delayMicroseconds(last_dlay);
        } 
  
        SPEED.put(0, spd);
        SPEED.commit();
        delay(50);
      
        CYCLE.put(0, cycle);
        CYCLE.commit();
        delay(100);
        
        LAST_SPEED.put(0, last_spd);
        LAST_SPEED.commit();
        delay(50);
          
  }
 //////////////////////////////////////// Check reed switch /////////////////////
  but5 = digitalRead(button5);
  if(but5 == HIGH ){ /////////////////////  open the case (reed switch off) then back to setting fuctions ///////////
    task = false;
    wait = false;
    //Serial.println("Case is Opened !");
  }
  else{
    wait = true;
    //Serial.println("Case still Closed....");  
    display.display();
    display.clearDisplay();
    display.setTextSize(1);  
    display.setCursor(0,1);
    display.print(F("PLEASE, OPEN "));
    display.setTextSize(1);  
    display.setCursor(0,15);
    display.print(F("THE CASE .... "));
  }
  display.display();
  display.clearDisplay();
  
  
//  Serial.println("FINALLY :");
//  Serial.print("spd: ");   Serial.println(spd);
//  Serial.print("cycle: "); Serial.println(cycle);
//  Serial.print("LAST_SPD: "); Serial.println(last_spd);
//  Serial.println("------------------------------------\n");
  
  
}
 

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   23
#define OLED_CLK   14
#define OLED_DC    12
#define OLED_CS    13
#define OLED_RESET 5
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define button1 19
#define button2 25
#define button3 26
#define button4 16
#define button5 27

const int stepPin = 17; 
const int dirPin = 18;  //mechanical pull-low

int dlay ;
int last_dlay;
int last_spd;
int but1,but2,but3,but4,but5;
uint32_t spd ;
uint32_t cycle ;
bool task = false;
bool wait = false;
bool manual_mode_state = false;

void ReadButtons(){
     but1 = digitalRead(button1);
     but2 = digitalRead(button2);
     but3 = digitalRead(button3);
     but4 = digitalRead(button4);
     but5 = digitalRead(button5);  
}

void PrintButtons(){
     Serial.print("but1 : ");
     Serial.print(but1);
     Serial.print("  but2 : ");
     Serial.print(but2);
     Serial.print("  but3 : ");
     Serial.print(but3);
     Serial.print("  but4 : ");
     Serial.print(but4);
     Serial.print("  but5 : ");
     Serial.println(but5);
}

void set(){
   
     
     but1 = digitalRead(button1);
     but2 = digitalRead(button2);
     but3 = digitalRead(button3);
     but4 = digitalRead(button4);
     but5 = digitalRead(button5);
     
     display.display();
     display.clearDisplay();

     
     display.setTextSize(1);  
     display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
     display.setCursor(1,1);
     display.print(F("       Property      "));

     
     display.setTextSize(1);  
     display.setTextColor(SSD1306_WHITE);
     display.setCursor(15,20);
     display.print(F("Speeds"));
     display.setCursor(25,35);
     display.setTextSize(2); 
     display.print(spd);

     display.setTextSize(1);  
     display.setCursor(80,20);
     display.print(F("Cycle"));
     display.setCursor(90,35);
     display.setTextSize(2); 
     display.print(cycle);

     if(but1 == LOW){
       if(spd< 10) spd++;
       //else spd = 0;
      }
     if(but2 == LOW){
       if(spd!=0) spd--;
       if(spd == 0){
          manual_mode_state = true;
       }
      }
     
     if(but3 == LOW){
       if(cycle< 12) cycle++;
      
      }
     if(but4 == LOW){
       if(cycle!=1) cycle--;
      }
     
     if(but5 == LOW ){
        task = true;
      }
       
  }

void manual_mode(){ /////////////////////////////// Manual MODE ////////////////
     
     but1 = digitalRead(button1);
     but2 = digitalRead(button2);
     but3 = digitalRead(button3);
     but4 = digitalRead(button4);
     but5 = digitalRead(button5);
     
     display.display();
     display.clearDisplay();
     display.setTextSize(1);  
     display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
     display.setCursor(1,1);
     display.print(F("    MANUAL MODE     "));

     display.setTextSize(1);  
     display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
     display.setCursor(1,50);
     display.print(F(" (BUTTON3) EXIT    "));

     
     display.setTextSize(1);  
     display.setTextColor(SSD1306_WHITE);
     display.setCursor(0,20);
     display.print(F("(BUTTN1)"));
     display.setCursor(3,30);
     display.setTextSize(1); 
     display.print(F("+Step"));

     display.setTextSize(1);  
     display.setCursor(60,20);
     display.print(F("(BUTTN2)"));
     display.setCursor(60,30);
     display.setTextSize(1); 
     display.print(F("LST-SPD:"));
     display.print(last_spd);

     if(but1 == LOW){
       digitalWrite(dirPin,LOW);
       digitalWrite(stepPin,HIGH);
       delayMicroseconds(1000);  
       digitalWrite(stepPin,LOW); 
       delayMicroseconds(1000);
      }
     if(but2 == LOW){
        delay(100);
        if(last_spd < 5){last_spd +=1;}
        else {last_spd = 1;}
      }
     
     if(but3 == LOW){
         manual_mode_state = false;
         spd = 1;
      }
}

void setup(){

  Serial.begin(115200);
  
  Serial.println("Testing EEPROMClass\n");
  if (!SPEED.begin(0x100)) {
    Serial.println("Failed to initialise SPEED");
    Serial.println("Restarting...");
    delay(1000);
    ESP.restart();
  }
  if (!CYCLE.begin(0x100)) {
    Serial.println("Failed to initialise CYCLE");
    Serial.println("Restarting...");
    delay(1000);
    ESP.restart();
  }

   if (!LAST_SPEED.begin(0x100)) {
    Serial.println("Failed to initialise LAST_SPEED");
    Serial.println("Restarting...");
    delay(1000);
    ESP.restart();
  }
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();
  display.clearDisplay();

  pinMode(stepPin,OUTPUT);
  //pinMode(dirPin,OUTPUT);  
  pinMode(button1,INPUT);
  pinMode(button2,INPUT);
  pinMode(button3,INPUT);
  pinMode(button4,INPUT);
  pinMode(button5,INPUT);
  digitalWrite(dirPin,LOW);

  SPEED.get(0, spd);
  CYCLE.get(0, cycle);
  LAST_SPEED.get(0,last_spd);

  Serial.print("spd: ");   Serial.println(spd);
  Serial.print("cycle: "); Serial.println(cycle);
  Serial.print("LAST_SPD: "); Serial.println(last_spd);
  Serial.println("------------------------------------\n");
  
  if(spd > 10 || cycle > 12 || last_spd > 10){
    spd = 1;
    cycle = 1;
    last_spd = 1;
      
  }
  
  Serial.print("spd: ");   Serial.println(spd);
  Serial.print("cycle: "); Serial.println(cycle);
  Serial.print("LAST_SPD: "); Serial.println(last_spd);
  Serial.println("------------------------------------\n");

}
     
void loop() { 

 ReadButtons();
 //PrintButtons(); //   Uncomment this to check Buttons State via Serial Monitor !
 //Serial.println("manual_mode_state : " + String(manual_mode_state));
 //////////////////////////////////// Check Manual Mode ///////////////////////////////
 while(manual_mode_state == true)manual_mode();
 
 /////////////////////////////////// Setting Functions //////////////////////////////////
 while(task == false && manual_mode_state == false){
    set();
    delay(120) ;
  } 
  ///////////////////////////// set complete //////////////////////////////
 dlay = 1100 -(spd * 100);
 last_dlay = 2000 - (last_spd * 200);
 ///////////////////////////////////////// Start Spin ///////////////////////////
  if(wait == false && task == true){
             
        for(int i =0;i<800*(cycle-1);i++){
          digitalWrite(stepPin,HIGH); 
          delayMicroseconds(dlay); 
          digitalWrite(stepPin,LOW); 
          delayMicroseconds(dlay);         
        }
    
     
     ///////////////////////////////////////// Last spin (Slow speed) ///////////////
        for(int i =0;i<800;i++){
          digitalWrite(stepPin,HIGH);
          delayMicroseconds(last_dlay);  
          digitalWrite(stepPin,LOW); 
          delayMicroseconds(last_dlay);
        } 
  }
 //////////////////////////////////////// Check reed switch /////////////////////
  but5 = digitalRead(button5);
  if(but5 == HIGH ){ /////////////////////  open the case (reed switch off) then back to setting fuctions ///////////
    task = false;
    wait = false;
    //Serial.println("Case is Opened !");
  }
  else{
    wait = true;
    //Serial.println("Case still Closed....");  
    display.display();
    display.clearDisplay();
    display.setTextSize(1);  
    display.setCursor(0,1);
    display.print(F("PLEASE, OPEN "));
    display.setTextSize(1);  
    display.setCursor(0,15);
    display.print(F("THE CASE .... "));
  }
  display.display();
  display.clearDisplay();
  SPEED.put(0, spd);
  SPEED.commit();
  LAST_SPEED.put(0, last_spd);
  LAST_SPEED.commit();
  CYCLE.put(0, cycle);
  CYCLE.commit();

  SPEED.get(0, spd);
  LAST_SPEED.get(0, last_spd);
  CYCLE.get(0, cycle);
  

  
//  Serial.print("spd: ");   Serial.println(spd);
//  Serial.print("cycle: "); Serial.println(cycle);
//  Serial.print("LAST_SPD: "); Serial.println(last_spd);
//  Serial.println("------------------------------------\n");
  
}
 
