#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

EEPROMClass  SPEED("eeprom0");
EEPROMClass  CYCLE("eeprom1");

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
     display.print(F(" (BUTTON 3) EXIT    "));

     
     display.setTextSize(1);  
     display.setTextColor(SSD1306_WHITE);
     display.setCursor(3,20);
     display.print(F("(BUTTN 1)"));
     display.setCursor(20,35);
     display.setTextSize(2); 
     display.print(F("+"));

     display.setTextSize(1);  
     display.setCursor(65,20);
     display.print(F("(BUTTN 2)"));
     display.setCursor(80,35);
     display.setTextSize(2); 
     display.print(("-"));

     if(but1 == LOW){
       digitalWrite(dirPin,LOW);
       digitalWrite(stepPin,HIGH);
       delayMicroseconds(1000);  
       digitalWrite(stepPin,LOW); 
       delayMicroseconds(1000);
      }
     if(but2 == LOW){
       digitalWrite(dirPin,HIGH);
       digitalWrite(stepPin,HIGH);
       delayMicroseconds(1000);  
       digitalWrite(stepPin,LOW); 
       delayMicroseconds(1000);
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
    Serial.println("Failed to initialise NAMES");
    Serial.println("Restarting...");
    delay(1000);
    ESP.restart();
  }
  if (!CYCLE.begin(0x100)) {
    Serial.println("Failed to initialise HEIGHT");
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

  Serial.print("spd: ");   Serial.println(spd);
  Serial.print("cycle: "); Serial.println(cycle);
  Serial.println("------------------------------------\n");
  
  if(spd > 10 || cycle > 12){
    spd = 1;
    cycle = 1;  
  }
  
  Serial.print("spd: ");   Serial.println(spd);
  Serial.print("cycle: "); Serial.println(cycle);
  Serial.println("------------------------------------\n");

}
     
void loop() { 

 ReadButtons();
 //PrintButtons(); //   Uncomment this to check Buttons State via Serial Monitor !
 Serial.println("manual_mode_state : " + String(manual_mode_state));
 //////////////////////////////////// Check Manual Mode ///////////////////////////////
 while(manual_mode_state == true)manual_mode();
 
 /////////////////////////////////// Setting Functions //////////////////////////////////
 while(task == false && manual_mode_state == false){
    set();
    delay(120) ;
  } 
  ///////////////////////////// set complete //////////////////////////////
 dlay = 1100 -(100 * spd);
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
          delayMicroseconds(1500);  
          digitalWrite(stepPin,LOW); 
          delayMicroseconds(1500);
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
  CYCLE.put(0, cycle);
  CYCLE.commit();

  SPEED.get(0, spd);
  CYCLE.get(0, cycle);

  
  Serial.print("spd: ");   Serial.println(spd);
  Serial.print("cycle: "); Serial.println(cycle);
  Serial.println("------------------------------------\n");
  
}
 
