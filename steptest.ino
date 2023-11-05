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
#define button4 16
#define button5 27

const int stepPin = 17; 
//const int dirPin = 18; 

int dlay ;
int but1,but2,but3,but4,but5;
unsigned int spd,cycle = 1;
bool task = false;

void set(){
   
     
     but1 = digitalRead(button1);
     but2 = digitalRead(button2);
     but3 = digitalRead(button3);
     but4 = digitalRead(button4);
     but5 = digitalRead(button5);
     /*Serial.print("but3 : ");
     Serial.print(but3);
     Serial.print("  but4 : ");
     Serial.print(but4);
     Serial.print("  but5 : ");
     Serial.println(but5);*/
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
       if(spd<=2) spd++;
       //else spd = 0;
      }
     if(but2 == LOW){
       if(spd!=0) spd--;
      }
     
     if(but3 == LOW){
       if(cycle<=11) cycle++;
      
      }
     if(but4 == LOW){
       if(cycle!=1) cycle--;
      }
     
     if(but5 == HIGH ){//pulldown
        task = true;
      }
       
  }


void setup(){

  Serial.begin(9600);

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
  //digitalWrite(dirPin,HIGH);
}
     
void loop() { 
     
     while(task == false){
        set();
        delay(100) ;
      } 
     dlay = 600+(300-(spd*100));
     if(spd == 0){
        dlay = 1000;
      }
       
     for(int i =0;i<200*(cycle-1);i++){
        digitalWrite(stepPin,HIGH); 
        delayMicroseconds(dlay); 
        digitalWrite(stepPin,LOW); 
        delayMicroseconds(dlay);         
      }

      delay(100);
     
      for(int i =0;i<100;i++){
        delay(5);
        for(int j =0;j<2;j++){
          digitalWrite(stepPin,HIGH);
          delayMicroseconds(1000);  
          digitalWrite(stepPin,LOW); 
          delayMicroseconds(1000);
          
        }
      }
      task = false;
}
 
