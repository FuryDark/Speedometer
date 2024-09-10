#include<LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

int timer1;
int timer2;

float Time;

int flag1 = 0;
int flag2 = 0;

double distance = 22.3;
float Speed;

#define trig1 A1
#define echo1 A0
#define trig2 A2
#define echo2 A3
#define button 8

int buzzer = 11;

void setup(){
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.print("Vehicle Speed");
  lcd.setCursor(2,1);
  lcd.print("Measurement");
  delay(2000);
  lcd.clear();
}

//Calculating the distance of the object via ultrasonic sensor
double calcDist(int trig,int echo){
  double duration;
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  double distance = (duration * 0.034)/2;
  return distance;
}

//Calculating the speed by taking readings from both sensors and getting the time interval between the readings.
void loop() {
  if (digitalRead(button) == 1){
    Serial.print("Start " );
    Serial.println(millis());
    if(calcDist(trig1,echo1) < 40 && flag1==0){timer1 = millis(); flag1=1;}
//    Serial.print("After If 1 ");
//    Serial.println(millis());
    if(calcDist(trig2,echo2) < 40 && flag2==0){timer2 = millis(); flag2=1;}
//    Serial.print("After If 2  ");
//    Serial.println(millis());
    if (flag1==1 && flag2==1){
      if(timer1 > timer2){Time = timer1 - timer2;}
      else if(timer2 > timer1){Time = timer2 - timer1;}
     Time=Time/1000;//convert millisecond to second
     Speed=(distance/Time);//v=d/t
    // Speed=(Speed*18)/500 ;//Conversion of speed to kilometers per hour
    }
    
    if(Speed==0){ 
      lcd.setCursor(0, 1); 
      if(flag1==0 && flag2==0){lcd.print("No car  detected");}
      else{
//        String search[4] = {"Searching     ","Searching.    ","Searching..    ","Searching...    "};
//        for(int i=0;i<=3;i++){
          lcd.print("Searching...    ");
//          delay(500);
//          lcd.clear();
//          lcd.setCursor(0, 1);
//            Serial.print("After Searching ");
//    Serial.println(millis());
//        }
      } 
    }
    else{
      lcd.clear(); 
      lcd.setCursor(0, 0); 
      lcd.print("Speed:");
      lcd.print(Speed,1);
      lcd.print("cm/s  ");
      lcd.setCursor(0, 1); 
      if(Speed > 20 ){lcd.print("  Over Speeding  "); tone(buzzer, 4000, 400);}
      else{lcd.print("  Safe Speed   "); }    
      delay(2000);
      Speed = 0;
      flag1 = 0;
      flag2 = 0;    
     }
  }
  else{
    lcd.clear();
  }
}
