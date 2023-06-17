#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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
#define button4 18
#define button5 27

const int dirPin  = 16 ;
const int stepPin = 17; 

const int rev = 200;
int p =700; 
int but1,but2,but3,but4,but5;
int rnd,frame,dlay,spd = 0;
int cycle = 1;
int i = 0; // for menu
bool select = false;
int blinks = 0;
int blinks_time = 7;
int dodge = 3;

int num_spd[5] = {0,6, 7, 8, 9 };
int num_dlay[5] = {0,860,880,900,920};
int keep,keep2;
bool task = false;


void set(){
   
     
     but1 = digitalRead(button1);
     but2 = digitalRead(button2);
     but3 = digitalRead(button3);
     Serial.print("but3 : ");
     Serial.println(but3);
     display.display();
     display.clearDisplay();
     display.setTextSize(1);  
     display.setTextColor(SSD1306_WHITE);
     display.setCursor(1,56);
     display.print(F(" HIGH(RED)  "));
     display.print(F("LOW(BLUE)"));
     
     display.setTextSize(1);  
     display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
     display.setCursor(1,1);
     display.print(F("       SETTING      "));
    
     
     //////////////////// p o i n t e r /////////////////////////
     
     
     if(i > 0) {
        if(i%4 == 0) i =1;
        i = abs(i)%4;
        Serial.println(i);
      }

    
      if (but1 == LOW) {
        if(select == true){
            if(keep > 0)keep--;
          }
        else{
            if(i == 0) i = 4;
            if(i == 1) i = 4;
            i--;
          }
       }
      
      if (but2 == LOW) {
        if(select == true)keep++;
        else{
           if(i == 0) i = 0;
           i++;
          }
        
       }
      
      if (but3 == LOW) {
         if(select == true)select = false;
         else {
            keep = 0;
            select = true;
          }
         
       }

    
     
     //////////////////////    m e n u    /////////////////////
     if(i == 0){
        if(select == true){
             display.clearDisplay();
             display.setTextSize(1);  
             display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
             display.setCursor(1,28);
             display.print(F("  PLEASE SETTING  "));
             display.display();
             delay(1500);
             select = false;  
        }
      }
     
     display.setTextSize(1);  
     if(i == 1){
        display.setTextColor(SSD1306_BLACK,SSD1306_WHITE);
        if(select == true){
            blinks++;
            blinks = blinks % blinks_time;
            if(blinks > dodge){
                display.setTextColor(SSD1306_WHITE);
              }
            keep = keep % 5;
            spd = num_spd[keep];
            dlay = num_dlay[keep];
            keep2 = keep;
          }
      }
     else display.setTextColor(SSD1306_WHITE);
     display.setCursor(1,17);
     display.print(F(" SPEED >> "));
     display.print(spd);
     display.println(F(" rev/sec"));

    
     display.setTextSize(1);   
     if(i == 2){
        display.setTextColor(SSD1306_BLACK,SSD1306_WHITE);
         if(select == true){
            blinks++;
            blinks = blinks % blinks_time;
            if(blinks > dodge){
                display.setTextColor(SSD1306_WHITE);
              }
             rnd = keep % 9;
          }
      }
     else display.setTextColor(SSD1306_WHITE);
     display.setCursor(1,28);
     display.print(F(" ROUND >> "));
     display.print(rnd);
     display.println(F(" rounds "));

    
     display.setTextSize(1);  
     if(i == 3){
        display.setTextColor(SSD1306_BLACK,SSD1306_WHITE);
         if(select == true){
            if(rnd !=0 && spd != 0)task = true;
            else {
             
             display.clearDisplay();
             display.setTextSize(1);  
             display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
             display.setCursor(1,28);
             display.print(F("  PLEASE SETTING  "));
             display.display();
             delay(1500);
             select = false;
          }
          }
      }
     else display.setTextColor(SSD1306_WHITE);
     display.setCursor(1,39);
     display.println(F(" DONE SETTING      "));
    
     
  }


void setup(){

  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();
  delay(2000); // Pause for 2 seconds

  display.clearDisplay();
  
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  
  pinMode(button1,INPUT);
  pinMode(button2,INPUT);
  pinMode(button3,INPUT);
  pinMode(button4,INPUT);
  pinMode(button5,INPUT);

 
  
}

          
          
void loop() { 

 
     while(task == false){
        set();
        Serial.print("keep2 : ");
        Serial.print(keep2);
        Serial.print("   round : ");
        Serial.print(rnd);
        Serial.print("  dlay : ");
        Serial.println(dlay);
        delay(150);
      } 
    
     for(int i =0;i<rnd;i++){

           
           digitalWrite(dirPin,HIGH);
           for(int j =0;j<200;j++){
              digitalWrite(dirPin,HIGH);
              digitalWrite(stepPin,HIGH); 
              delayMicroseconds(dlay); 
              digitalWrite(stepPin,LOW); 
              delayMicroseconds(dlay);
           }

          
           but1 = digitalRead(button1);
           but2 = digitalRead(button2);
           but3 = digitalRead(button3);
           but4 = digitalRead(button4);
           but5 = digitalRead(button5);
  
           
           if(but1 == LOW){
              frame++;
              if(frame%1 == 0){
                rnd++;
                frame = 0;
              }
            }
           if(but2 == LOW){
              frame++;
              if(frame%1 == 0 && rnd > 0){
                rnd--;
                frame =0;
              }
            }
           if(but3 == LOW){
              frame++;
              if(frame%1 == 0){
                cycle--;
                i = i -200;
                frame = 0;
              }
            }
           if(but4 == LOW){
              frame++;
              if(frame%3 == 0){
                spd = num_spd[keep2++];
                dlay = num_dlay[keep2++];
                frame = 0;
              }
            }
           if(but5 == LOW){
              frame++;
              if(frame%3 == 0){
                if(keep2 != 1){
                    spd = num_spd[keep2--];
                    dlay = num_dlay[keep2--];
                  }
                frame = 0;
              }
            }
            
          //if(i%2 == 0) cycle++;
          cycle++;
          display.display();
          display.clearDisplay();
          display.setTextSize(1);  
          display.setTextColor(SSD1306_WHITE);
     
          display.setTextSize(2);  
          display.setTextColor(SSD1306_WHITE);
          display.setCursor(8,10);
          display.println(F("Cycle> "));
    
           display.setTextSize(4);  
           display.setCursor(94,15);
           display.setTextColor(SSD1306_BLACK,SSD1306_WHITE); 
           display.print(F(" "));

           display.setTextSize(3);  
           display.setCursor(85,10);
           display.setTextColor(SSD1306_BLACK,SSD1306_WHITE); 
           display.print(F("  "));

           display.setTextSize(3);  
           display.setCursor(85,25);
           display.setTextColor(SSD1306_BLACK,SSD1306_WHITE); 
           display.print(F("  "));
           
           display.setTextSize(4);  
           display.setCursor(90,15);
           display.setTextColor(SSD1306_BLACK,SSD1306_WHITE); 
           display.print(F(""));
           display.println(cycle);
           
           display.setTextSize(1);  
           display.setTextColor(SSD1306_WHITE);
           display.setCursor(1,30);
           display.print(F(" Rounds : "));
           display.println(rnd);
      
           display.setTextSize(1);  
           display.setTextColor(SSD1306_WHITE);
           display.setCursor(1,40);
           display.print(F(" Speeds : "));
           display.println(spd);


           delay(2);
      }
      delay(500);
      for(int i = 0; i < 200 ;i++){
        digitalWrite(dirPin,HIGH);
        digitalWrite(stepPin,HIGH); 
        delayMicroseconds(1000); 
        digitalWrite(stepPin,LOW); 
        delayMicroseconds(1000); 
        
      }
      delay(2000);
      ESP.restart();
 
}
