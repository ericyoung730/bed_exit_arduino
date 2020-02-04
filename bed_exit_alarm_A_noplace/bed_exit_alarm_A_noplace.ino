#include <SoftwareSerial.h>
#include "Timer.h"
#include "bed_exit_alarm_TX.h"
#include "databaseStruct.h"
#include "DL_ln33.h"
#include "new_pulseIn.h"
#include "Direction_Detect.h"
#include <Adafruit_GFX.h>  
#include <Adafruit_SSD1306.h>  
#define OLED_RESET -1  
Adafruit_SSD1306 display(OLED_RESET);  
double feat[7] = {0};
resultVector resultDistance[AC_NUMBER];
const vector PROGMEM AC[AC_NUMBER] = {
  {.DATABASE = {double(309.3392857),double(1457.714286),double(10065.50446),double(6.129464286),double(55.26339286),double(13.46875),double(4068068.261)}, .ClusterType = 1},
  {.DATABASE = {double(825.7678571),double(2467.776786),double(9884.892857),double(36.67857143),double(23.33482143),double(132.4330357),double(2397701.219)}, .ClusterType = 1},
  {.DATABASE = {double(1102.464286),double(1263.352679),double(9267.84375),double(-25.42857143),double(87.48660714),double(48.36160714),double(3884283.586)}, .ClusterType = 1},
  {.DATABASE = {double(588.4375),double(4007.21875),double(12577.80357),double(6.165178571),double(21.38392857),double(20.09375),double(1479518.893)}, .ClusterType = 2},
  {.DATABASE = {double(355.5178571),double(5049.919643),double(12048.26786),double(-1.330357143),double(30.23660714),double(15.07589286),double(1958619.103)}, .ClusterType = 2},
  {.DATABASE = {double(424.25),double(4828.973214),double(12248.66964),double(-17.58482143),double(30.04464286),double(21.13839286),double(2962562.413)}, .ClusterType = 2},
  {.DATABASE = {double(4909.017857),double(3415.321429),double(9043.330357),double(88.04017857),double(-311.5178571),double(-779.3839286),double(68966440.11)}, .ClusterType = 3},
  {.DATABASE = {double(5291.607143),double(2670.169643),double(9021.026786),double(36.91071429),double(-293.4017857),double(-747.6071429),double(80769343.41)}, .ClusterType = 3},
  {.DATABASE = {double(5528.3125),double(2899.5),double(7941.589286),double(-53.38839286),double(-312.8616071),double(-1027.745536),double(77302200.85)}, .ClusterType = 3},
};
int printAC = 0;
int Election[AC_TYPE] = {0};
int max = 0;
long timer1=0;  
const unsigned char PROGMEM low_power [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00,
0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x00,
0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00,
0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x00, 0x00,
0x0F, 0xFF, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
0x0F, 0xE0, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
0x0F, 0x80, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
0x0F, 0x80, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00,
0x0F, 0x80, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00,
0x0F, 0x80, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00,
0x0F, 0x80, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00,
0x0F, 0x80, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00,
0x0F, 0x80, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00,
0x0F, 0x80, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00,
0x0F, 0x80, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00,
0x0F, 0x80, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00,
0x0F, 0x80, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00,
0x0F, 0x80, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00,
0x0F, 0x80, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00,
0x0F, 0x80, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00,
0x0F, 0x80, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
0x0F, 0x80, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
0x0F, 0xE0, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
0x0F, 0xFF, 0x80, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x00, 0x00,
0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x00,
0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00,
0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x00,
0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
int controlPin = 11;  //電晶體開關pin
int analogValue = 0;  //A0讀值
float voltageConnect = 0; //通路電壓
int battery_state = 0; //電池狀態 3=高電量 2=中電量 1=低電量
unsigned long US_time_count=0;
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
void buttoncheck()
{
  if(digitalRead(A1)==LOW)
  {
    if(millis()-timer1>500)
    {
      timer1=millis();
    Serial.println("A1 pressed");
    display.drawBitmap(0, 0,  low_power , 128, 32, 1);  
    
    display.display(); 
    }
  }
  else if(digitalRead(A2)==LOW)
  {
     if(millis()-timer1>500)
    {
      timer1=millis();
          Serial.println("A2 pressed");
    display.clearDisplay(); 
    
    display.display(); 
    }

  }
}
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
   pinMode(A1,INPUT_PULLUP);
  pinMode(A2,INPUT_PULLUP);
  // put your setup code here, to run once:  
   // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // init done  
 
  attachInterrupt(0, &ISR_US1, FALLING);
  attachInterrupt(1, &ISR_US2, FALLING);
  
  Serial.println("setup");
 
  
  for(int i = 0; i<3; i++){
    ln33_transmit_register();
    delay(100); 
  }
   display.setTextColor(WHITE);  
  // Show image buffer on the display hardware.  
  // Since the buffer is intialized with an Adafruit splashscreen  
  // internally, this will display the splashscreen.  
  display.display();  
  delay(2000);  
  
  // Clear the buffer.  
  display.clearDisplay();  
  display.drawBitmap(0, 0,  low_power , 128, 32, 1);  
    
  display.display(); 
 /* digitalWrite(speaker,HIGH);
      T.after(9200,callback);//////讓警報維持一段時間
*/
}
static inline void print_Serial_Plotter_nofiltered()
{
  Serial.print(US1_TIME);
  Serial.print(",");
  Serial.print(US2_TIME); 
  Serial.print(","); 
//   dataString=String(US1_TIME);
//    dataString += ",";
//     dataString +=String(US2_TIME);
}
void loop() {
 if(digitalRead(speaker) == LOW)
   {
     digitalWrite(speaker,HIGH);
     T.after(9200,callback);//讓警報維持一段時間
     Serial.println("alarm3");
   }
unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) 
  {
     ln33_transmit_register();
     //Serial.println("register");
     previousMillis = currentMillis;
  }
  if(currentMillis - electime > 10000) 
  {
    //electricity();
     //ln33_transmit_register();
     //Serial.println("register");
     electime = currentMillis;
  }
  
  T.update();
  get_messenger();
  buttoncheck();
  //electricity();
  //Serial.println(millis()-US_time_count) ;
  //US_time_count = millis();
  new_pulseIn();
  new_pulseIn_filter();
  //print_Get_Direction();
  //print_Serial_Plotter_nofiltered();
  max_filter();
  US_diff();
  US_slope();
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

 for(int i = 0;i < AC_NUMBER;i++){
       
        resultDistance[i].result = EulerDistance(feat, AC[i].DATABASE);
        resultDistance[i].Type = AC[i].ClusterType;
      }
      bubbleSort(resultDistance);
      // if have K value do KNN
      //注意 K 最好是奇數,以免造成平手問題,導致傳出兩種數值
      //讀取前K個短的距離的群集,並儲存票數
      Election[0] = 0;
      Election[1] = 0;
      Election[2] = 0;
      
      for(int i = 0;i < K_OF_KNN;i++){
        
        switch(resultDistance[i].Type){
          
        case 1:
          Election[0] = Election[0]+1;
          break;
        case 2:
          Election[1] = Election[1]+1;
          break;
        case 3:
          Election[2] = Election[2]+1;
          break;
          /*case 4:
          Election[3]++;
          break;*/
        default:
          break;
        }
      }
      max = Election[0];
      printAC = 1;
      //找出最多票的群集No.
      for(int i = 1;i < AC_TYPE;i++){
        
        if(Election[i] > max){
          
          max = Election[i];
          printAC = i+1;    //RealTimeAC.ClusterType
        }
        
      }
      
      Serial.println(printAC); 
}
