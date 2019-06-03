#include <SoftwareSerial.h>
#include "Timer.h"
#include "bed_exit_alarm_TX.h"
#include "DL_ln33.h"
#include "new_pulseIn.h"
#include "Direction_Detect.h"
int controlPin = 11;  //電晶體開關pin
int analogValue = 0;  //A0讀值
float voltageConnect = 0; //通路電壓
int battery_state = 0; //電池狀態 3=高電量 2=中電量 1=低電量

bool reset_button=false;                  //flag about reset
bool ir=false;                            //flag about ir
bool us=false;                            //表示超音波偵測離床或沒離床
bool Bpartexist=false;

String Bpart_data;                        //data received from Bpart
int speaker=8,battery=A0; // pin set for speaker,place switch,battery detect
 
int B_gap_time=0;                         //count for bpart gap time

Timer T;                                  //alarm time object

long electime = 0;                        //elec time register
long previousMillis = 0;                  //time register
long interval = 2000;                     //time gap

//-------------clear the mys buffer--------------//
void serialFlush() {
  while(mys.available() > 0) {
    char t = mys.read();
  }
}
//---------------------------------------------//
   
//-------------turn off alarm------------------//
void callback(){
  digitalWrite(speaker,LOW);
}
//---------------------------------------------//
void electricity()
{
  analogValue = 0;
  
  /*digitalWrite(controlPin, LOW); //電晶體關
  delay(1000);
  voltageOpen = 0.0048*analogRead(0);
  delay(1000);*/
  
  digitalWrite(controlPin, HIGH); //電晶體開
  delay(100);
  voltageConnect = 0.0048*analogRead(0);
  //delay(100);
//判斷電池電量
  if(voltageConnect>=3.8){
    battery_state=3;
    ln33_transmit_data(0x00, 0x03);
    Serial.print("battery_state = ");
    Serial.println(battery_state);
  }
  else if(voltageConnect>=3.6 && voltageConnect<3.8){
    battery_state=2;
    ln33_transmit_data(0x00, 0x02);
    Serial.print("battery_state = ");
    Serial.println(battery_state);
  }
  else{
    battery_state=1;
    ln33_transmit_data(0x00, 0x01);
    Serial.print("battery_state = ");
    Serial.println(battery_state);
  }
  digitalWrite(controlPin, LOW); //電晶體關
  Serial.print(voltageConnect);
  Serial.println(" V");
}
//------------US alogrithm---------------------//
static inline void print_Get_Direction()
{
  uint8_t tmp = Get_Direction(reset_button);
  reset_button=false;
  switch(tmp) {
    case Direction_Undefined:
      //Serial.print(0);
      break;
    case Direction_Leave:
      if(us==false)
      {
       us=true;
       ln33_transmit_data(0x01,0x00);
       Serial.print(5);
      } 
      break;
    case Direction_Stay:
     if(us==true)
      {
        us=false;
        ln33_transmit_data(0x10,0x00);
        Serial.print(-5);
      }
      break;
    default :
      break;
  }
}
//---------------------------------------------//

//------------receive data from Bpart----------//

char get_messenger() {  
  byte buff[11]={0}; 
  Bpart_data="";
  while( mys.available() ) {
    for(int t=0 ; t<12 ; t++) 
    {
    buff[t] = mys.read();
    Serial.println(buff[t]);
    }   
    B_gap_time=0;
    
  }
  if(buff[0]==0xFE) {
      if(buff[7] == 0x1) { 
         Bpart_data="ir_leave";
     }
     else if(buff[7] == 0x2) { 
          B_gap_time=0;
          if(Bpartexist==false)
          {
           Bpartexist=true;
           Bpart_data="Bpart";
          }
     }
     else if(buff[7] == 0x3) { 
       
        Serial.println("computer");
        Bpart_data="computer";
     }
     else if(buff[8] == 0x1) {
       Bpart_data="ir_trigger";
     }
      else if(buff[9] == 0x1) { 
      Bpart_data="rail_down";
     }
    }
  
  B_gap_time++;
  //Serial.print("B_gap_time:");
  //Serial.println(B_gap_time);
  if(B_gap_time>800)
  {
    if(Bpartexist==true)
    //ln33_transmit_data(0x00,0x05);
    Bpartexist=false;
  }
  else if(B_gap_time>65530)
  {
    B_gap_time=0;
  }
  serialFlush();
}

//-----------------------------------//

void setup() {
  Serial.begin(115200);
  mys.begin(115200);
  TIMER1_Init();
  gpio_Init();
  ln33_Serial_Init();
  ln33_channel_Init();
  ln33_ID_Init();
  ln33_Restart();
  pinMode(speaker,OUTPUT);
  pinMode(battery,INPUT);
  
  attachInterrupt(0, &ISR_US1, FALLING);
  attachInterrupt(1, &ISR_US2, FALLING);
  
  Serial.println("setup");
  
  
  for(int i = 0; i<3; i++){
    ln33_transmit_register();
    delay(100); 
  }
 /* digitalWrite(speaker,HIGH);
      T.after(9200,callback);//////讓警報維持一段時間
*/
}
static inline void print_Serial_Plotter_nofiltered()
{
  Serial.print(US1_TIME);
  Serial.print(",");
  Serial.println(US2_TIME); 
//   dataString=String(US1_TIME);
//    dataString += ",";
//     dataString +=String(US2_TIME);
}
void loop() {
  
unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) 
  {
     ln33_transmit_register();
     //Serial.println("register");
     previousMillis = currentMillis;
  }
  if(currentMillis - electime > 10000) 
  {
    electricity();
     //ln33_transmit_register();
     //Serial.println("register");
     electime = currentMillis;
  }
  
  T.update();
  get_messenger();
  //electricity();
  new_pulseIn();
  new_pulseIn_filter();
  print_Get_Direction();
  //print_Serial_Plotter_nofiltered();
  
 if(Bpart_data=="ir_trigger")
  { 
    Serial.println("send ir_trigger");
    ln33_transmit_data(0x10,0x01);
    ir=true;
  }
 else if(Bpart_data=="ir_leave") 
   { 
    Serial.println("send ir_leave");
    ln33_transmit_data(0x10,0x10);
    ir=false;
   }
 else if(Bpart_data=="rail_down")
 { 
    ln33_transmit_data(0x11, 0x01);
    delay(1);
    Serial.println("send rali_down");
   if(digitalRead(speaker) == LOW)
   {
     digitalWrite(speaker,HIGH);
     T.after(9200,callback);//讓警報維持一段時間
     Serial.println("alarm3");
   }
 }
 else if(Bpart_data=="Bpart")
 { 
  ln33_transmit_data(0x00,0x04);
  delay(1);
  Serial.println("send Bpart");
 }
 else if(Bpart_data=="computer")
 { 
   us=false;        
   reset_button=true;
 }

 if(digitalRead(speaker) == LOW)
 {
    if(us==true&&ir==true)
    {
      digitalWrite(speaker,HIGH);
      T.after(9200,callback);//////讓警報維持一段時間
      Serial.println("alarm2");
    }
 }
}
