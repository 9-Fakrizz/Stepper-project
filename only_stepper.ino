const int stepPin = 17; 
//const int dirPin = 18; 
int dlay = 1000;

void setup(){

  Serial.begin(9600);

  pinMode(stepPin,OUTPUT);
  //pinMode(dirPin,OUTPUT);  
  //digitalWrite(dirPin,HIGH);

}
     
void loop() { 
//     for(int i =0;i<1600*5;i++){
//          digitalWrite(stepPin,HIGH); 
//          delayMicroseconds(800); 
//          digitalWrite(stepPin,LOW); 
//          delayMicroseconds(800);
//     }
//     delay(1000);
//     Serial.println("hello");

       for(int i =0;i<200;i++){
          digitalWrite(stepPin,HIGH); 
          delayMicroseconds(dlay); 
          digitalWrite(stepPin,LOW); 
          delayMicroseconds(dlay);
      }
      dlay -=1;
      Serial.println(dlay);
      if(dlay == 50)dlay =1000;
}
 
