/*#include <Wire.h>  
//#include <Adafruit_GFX.h>  
#include <Adafruit_SSD1306.h>  
#define OLED_RESET -1  
Adafruit_SSD1306 display(OLED_RESET);  
  */
  #include <avr/pgmspace.h>
#include "new_pulseIn.h"
#include "Direction_Detect.h"
#include "databaseStruct.h"
#include <SPI.h>
#include <SD.h>
#include <DS3231.h>
DS3231  rtc(SDA, SCL);  //SDA = A4 SCL = A5
Time t;

float feat[7] = {0};
resultVector resultDistance[AC_NUMBER];
const float ref_feat[][7] PROGMEM= {
  {3.0,53.6,2.2,99.0,79.8,15.2,6.1},
{3.0,100.0,-0.7,77.0,89.6,46.8,46.8},
{4.0,73.4,-3.3,111.0,77.5,20.7,11.7},
{3.0,55.1,-1.1,73.0,82.2,12.3,5.5},
{3.0,43.1,3.0,79.0,75.9,27.8,13.9},
{3.0,49.3,-2.5,54.0,74.1,31.5,20.4},
{5.0,82.2,-2.8,114.0,86.0,28.1,22.8},
{4.0,99.2,-0.8,79.0,89.9,39.2,39.2},
{3.0,60.7,3.9,93.0,74.2,19.4,4.3},
{3.0,96.4,0.1,89.0,82.0,38.2,32.6},
{3.0,47.9,1.6,73.0,71.2,34.2,16.4},
{3.0,57.3,-1.4,63.0,79.4,25.4,17.5},
{3.0,46.3,1.3,84.0,76.2,25.0,10.7},
{3.0,86.3,-1.1,70.0,78.6,34.3,28.6},
{3.0,52.4,3.0,67.0,76.1,35.8,23.9},
{2.0,100.0,0.0,63.0,74.6,7.9,7.9},
{3.0,60.2,1.5,85.0,70.6,32.9,12.9},
{3.0,56.8,-2.5,57.0,70.2,24.6,8.8},
{2.0,100.0,0.0,64.0,75.0,0.0,0.0},
{2.0,100.0,0.0,59.0,76.3,0.0,0.0},
{2.0,100.0,0.0,26.0,69.2,0.0,0.0},
{2.0,100.0,0.0,22.0,45.5,0.0,0.0},
{2.0,100.0,0.0,38.0,78.9,0.0,0.0},
{3.0,100.0,0.2,43.0,53.5,0.0,0.0},
{2.0,100.0,0.0,15.0,46.7,0.0,0.0},
{2.0,0.0,0.0,45.0,13.3,86.7,13.3},
{3.0,0.7,-0.1,99.0,19.2,91.9,19.2},
{6.0,14.7,0.9,168.0,31.5,97.0,31.5},
{2.0,0.0,0.0,45.0,20.0,88.9,20.0},
{3.0,0.4,-1.6,78.0,12.8,87.2,12.8},
{5.0,5.1,-1.8,202.0,14.9,92.1,13.9},
{3.0,2.6,-0.0,138.0,15.2,96.4,15.2},
{5.0,86.3,-2.4,333.0,94.3,27.9,25.2},
{4.0,59.7,0.8,208.0,92.8,44.7,41.3},
{3.0,50.9,0.6,205.0,75.1,35.1,16.6},
{2.0,100.0,0.0,16.0,50.0,0.0,0.0},
{4.0,64.7,3.2,207.0,76.8,32.9,24.2},
{2.0,100.0,0.0,44.0,81.8,0.0,0.0},
{2.0,100.0,0.0,60.0,78.3,0.0,0.0},
{4.0,72.8,-3.2,185.0,73.0,35.1,21.6},
{6.0,53.1,-1.5,276.0,87.0,35.9,26.8},
{3.0,60.6,0.4,270.0,80.7,50.7,37.4},
{3.0,57.0,0.4,323.0,80.2,50.5,35.0},
{3.0,38.6,2.7,43.0,60.5,39.5,18.6},
{3.0,71.8,-4.0,76.0,63.2,30.3,14.5},
{4.0,65.4,4.1,83.0,43.4,22.9,0.0},
{4.0,82.9,4.1,77.0,75.3,24.7,16.9},
{3.0,72.7,1.5,72.0,72.2,34.7,18.1},
{3.0,81.0,-1.1,101.0,62.4,33.7,8.9},
{3.0,66.3,1.3,76.0,65.8,34.2,10.5},
{3.0,82.2,-1.1,95.0,70.5,38.9,22.1},
{3.0,55.2,1.6,75.0,64.0,20.0,0.0},
{3.0,72.7,-1.1,87.0,72.4,37.9,20.7},
{3.0,78.8,1.2,92.0,55.4,46.7,13.0},
{3.0,79.6,-1.3,77.0,55.8,36.4,13.0},
{3.0,61.7,1.0,86.0,70.9,32.6,14.0},
{3.0,67.6,-1.3,72.0,79.2,43.1,33.3},
{4.0,51.0,5.6,83.0,63.9,36.1,15.7},
{5.0,45.0,-2.5,79.0,70.9,40.5,21.5},
{3.0,58.5,1.5,78.0,66.7,38.5,16.7},
{3.0,71.8,-1.8,61.0,67.2,50.8,32.8},
{4.0,63.9,1.5,70.0,68.6,31.4,14.3},
{4.0,83.3,1.3,94.0,95.7,54.3,53.2},
{3.0,68.8,0.8,69.0,92.8,40.6,37.7},
{5.0,42.1,-4.6,85.0,81.2,61.2,49.4},
{3.0,54.3,1.2,80.0,76.3,51.3,31.3},
{5.0,38.1,-0.7,110.0,50.0,50.9,9.1},
{4.0,37.0,2.3,75.0,73.3,44.0,21.3},
{4.0,32.1,-0.8,109.0,58.7,54.1,20.2},
{4.0,73.1,0.9,98.0,78.6,38.8,25.5},
{5.0,58.7,-1.1,104.0,78.8,50.0,37.5},
{3.0,72.1,1.3,90.0,68.9,32.2,7.8},
{4.0,90.1,-1.7,94.0,77.7,35.1,24.5},
{3.0,77.2,7.5,65.0,60.0,33.8,7.7},
{3.0,59.2,-1.8,85.0,87.1,57.6,48.2},
{3.0,65.3,1.3,76.0,80.3,39.5,23.7},
{3.0,87.2,-0.9,107.0,75.7,46.7,25.2},
{4.0,15.5,0.7,18.0,27.8,66.7,11.1},
{3.0,90.0,-0.0,16.0,75.0,62.5,56.3},
{3.0,55.7,1.5,70.0,84.3,42.9,31.4},
{5.0,55.2,0.5,97.0,58.8,62.9,25.8},
{4.0,64.6,-1.9,91.0,67.0,44.0,25.3},
{2.0,100.0,0.0,8.0,62.5,0.0,0.0},
{2.0,100.0,0.0,39.0,92.3,0.0,0.0},
{2.0,100.0,0.0,24.0,79.2,0.0,0.0},
{2.0,100.0,0.0,14.0,78.6,0.0,0.0},
{2.0,100.0,0.0,8.0,62.5,0.0,0.0},
{2.0,100.0,0.0,9.0,66.7,0.0,0.0},
{2.0,100.0,0.0,70.0,90.0,0.0,0.0},
{2.0,100.0,0.0,76.0,96.1,0.0,0.0},
{2.0,100.0,0.0,117.0,97.4,0.0,0.0},
{6.0,0.7,4.1,155.0,15.5,93.5,15.5},
{6.0,7.4,-3.7,142.0,34.5,94.4,33.8},
{2.0,0.0,0.0,122.0,19.7,95.1,19.7},
{8.0,18.6,-2.4,125.0,36.8,92.8,34.4},
{6.0,2.4,-1.7,126.0,36.5,92.9,36.5},
{6.0,1.6,-1.1,121.0,38.0,98.3,38.0},
{6.0,1.4,1.1,133.0,17.3,96.2,17.3},
{9.0,2.4,7.9,120.0,43.3,86.7,43.3},
{6.0,0.6,4.1,157.0,17.2,94.9,17.2},
{5.0,1.0,0.5,66.0,43.9,97.0,43.9},
{6.0,17.3,6.7,55.0,20.0,90.9,16.4},
{6.0,10.6,-1.4,134.0,33.6,89.6,30.6},
{3.0,0.0,0.1,57.0,43.9,86.0,43.9},
{3.0,4.1,-1.9,55.0,20.0,90.9,20.0},
{6.0,53.7,8.3,210.0,84.8,28.1,18.6},
{2.0,100.0,0.0,8.0,62.5,0.0,0.0},
{2.0,0.0,0.0,8.0,0.0,62.5,0.0},
{8.0,84.4,-1.0,213.0,92.5,23.9,18.8},
{5.0,59.6,5.3,156.0,93.6,28.8,24.4},
{2.0,100.0,0.0,21.0,66.7,0.0,0.0},
{4.0,38.0,-8.1,29.0,17.2,34.5,0.0},
{4.0,69.4,10.2,127.0,94.5,11.8,8.7},
{4.0,86.1,3.0,195.0,88.2,14.9,9.2},
{5.0,73.6,1.6,213.0,91.1,17.4,13.6},
{3.0,20.8,3.5,39.0,56.4,53.8,17.9},
{5.0,99.5,2.0,133.0,91.0,8.3,8.3},
{2.0,100.0,0.0,35.0,82.9,0.0,0.0},
{2.0,0.0,0.0,17.0,0.0,88.2,0.0},
{6.0,41.0,-3.5,215.0,86.0,28.8,20.5},
{2.0,100.0,0.0,5.0,60.0,0.0,0.0},
{13.0,78.7,-7.0,230.0,89.6,23.0,18.3}
};
const byte lable[AC_NUMBER] PROGMEM={
  1,
2,
1,
2,
1,
2,
1,
2,
1,
2,
1,
2,
1,
2,
1,
2,
1,
2,
3,
3,
3,
3,
3,
3,
3,
3,
3,
3,
3,
3,
3,
3,
1,
1,
1,
3,
1,
3,
3,
1,
1,
1,
1,
1,
2,
1,
2,
1,
2,
1,
2,
1,
2,
1,
2,
1,
2,
1,
2,
1,
2,
1,
2,
1,
2,
1,
2,
1,
2,
1,
2,
1,
2,
1,
2,
1,
2,
1,
2,
1,
2,
2,
3,
3,
3,
3,
3,
3,
3,
3,
3,
3,
3,
3,
3,
3,
3,
3,
3,
3,
3,
3,
3,
3,
3,
1,
3,
3,
1,
1,
3,
1,
1,
1,
1,
1,
1,
3,
3,
1,
3,
1
};
byte printAC = 0;
byte Election[AC_TYPE] = {0};
int max = 0;

const int chipSelect = 10; //CS = 10
String dataString = "";
String fileName = "";
extern volatile uint16_t US1_TIME;
extern volatile uint16_t US2_TIME;
unsigned long pretime=0;
unsigned long interval=20000;
extern int save_switch = 1;  // 1 for stop save data ;0 for start save data
int button = 1;  // 1 for stop save data ;0 for start save data
bool writing=false; 

void setup() {
   /*if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
   // Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // init done  
  display.setTextColor(WHITE);  
  // Show image buffer on the display hardware.  
  // Since the buffer is intialized with an Adafruit splashscreen  
  // internally, this will display the splashscreen.  
  display.display();  
 
  // Clear the buffer.  
   delay(2000);
   */  
  rtc.begin();

 
  Serial.begin(115200);
  TIMER1_Init();
 // rtc.setTime(15, 58,0 );     // Set the time to 12:00:00 (24hr format)
  pinMode(9, OUTPUT);    //output reset
  pinMode(4, OUTPUT);    //trig2
  pinMode(5, OUTPUT);    //trig1 
  pinMode(2, INPUT);     //echo1
  pinMode(3, INPUT);     //echo2  
  pinMode(8,INPUT_PULLUP);  //pullup pin8
  attachInterrupt(0, ISR_US1, FALLING);
  attachInterrupt(1, ISR_US2, FALLING);

  digitalWrite(9,HIGH);

  delay(2000);
  ////////////////check SD card module//////////////////
  if (!SD.begin(chipSelect)) {
    Serial.println(F("Card failed, or not present"));
    delay(2000);
    // don't do anything more:
     for(int i=0;i<7;i++){
    Serial.println(pgm_read_float(&(ref_feat[0][i])));
    Serial.println(lable[0]);
  }
    return;
  }
  Serial.println(F("card initialized."));
  delay(2000);
  Serial.println(F("card initialized."));

 
  //////////////////////////////////////////////////////
 
}
static inline void print_Serial_Plotter_nofiltered()
{
  /*Serial.print(US1_TIME);
  Serial.print(",");
  Serial.print(US2_TIME); 
  Serial.print(",");*/
   dataString=String(US1_TIME);
    dataString += ",";
     dataString +=String(US2_TIME);
}
void knnElection()
{
  for(int i = 0;i < AC_NUMBER;i++){
    float compare_feat[7]={0};
    for(int j=0;j<7;j++){
      compare_feat[j]=pgm_read_float(&(ref_feat[i][j]));
    }
     resultDistance[i].result = EulerDistance(feat,compare_feat);
     resultDistance[i].Type = pgm_read_byte(&(lable[i]));
     Serial.print(resultDistance[i].result);
     Serial.print(",");
     Serial.println(resultDistance[i].Type);
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
void loop() {
  if(millis()-pretime>interval)
  {
    pretime=millis();
    digitalWrite(9,LOW);
    delay(100);
    //Serial.print("here");
    digitalWrite(9,HIGH);
  } 
  if(digitalRead(8)==0){
    save_switch=!save_switch;
    delay(200);
  }
  
  //save_switch = digitalRead(8); //read switch
                //read US data
  //A_pulseIn_filter();
  //curtime=millis();
  
  t=rtc.getTime();
 /* Serial.print(int(t.hour));
  Serial.print(":");
  Serial.print(int(t.min));
  Serial.print(":");
  Serial.println(int(t.sec));*/
  //delay(1000);
  //print_Serial_Plotter_nofiltered();
  if(actionEndflag==1)
  {
    save_switch=1;
  }
 if(save_switch){
  writing=false;
  actionEndflag=0;
 }  
 else if(int(t.min)%10==0){
      if (int(t.sec)==0){
          t=rtc.getTime();
    /*      display.clearDisplay(); 
          display.setTextSize(2);             // Normal 1:1 pixel scale
          display.setTextColor(WHITE);        // Draw white text
          display.setCursor(0,0);             // Start at top-left corner
          display.println(F("Work!"));
          display.display(); 
          delay(2000);*/
      /*      display.clearDisplay(); 
          display.setTextSize(2);             // Normal 1:1 pixel scale
          display.setTextColor(WHITE);        // Draw white text
          display.setCursor(0,0);             // Start at top-left corner
          display.println(t.min);
          display.display(); */
          fileName=String(t.hour);
          fileName+=String("_");
          fileName+=String(t.min);
          fileName+=String("_");
          fileName+=String(t.sec);
          fileName.concat(".txt");
          Serial.println(fileName);
          }
          //Serial.println(int(t.min));
          //Serial.println(int(t.sec));
          
      char Filename[fileName.length()+1];
      fileName.toCharArray(Filename,sizeof(Filename));
      writing=true;
      File dataFile = SD.open(Filename, FILE_WRITE);
      new_pulseIn();
      max_filter();
      US_diff();
      US_slope();
      print_Serial_Plotter_nofiltered();
      if (dataFile) {
          dataFile.println(dataString);
          dataFile.close();
      // print to the serial port too:
      //Serial.println(dataString);
      }
 }
 else{ 
    if(writing==false){
       t=rtc.getTime();
       fileName=String(t.hour);
       fileName+=String("_");
       fileName+=String(t.min);
       fileName+=String("_");
       fileName+=String("_");
       fileName+=String(t.sec);
       
       fileName.concat(".txt");
       Serial.println(fileName);
    }
    char Filename[fileName.length()+1];
    fileName.toCharArray(Filename,sizeof(Filename));
    writing=true;
    File dataFile = SD.open(Filename, FILE_WRITE);
    new_pulseIn();
    print_Serial_Plotter_nofiltered();
    max_filter();
    US_diff();
    US_slope();
    if(actionEndflag==1){
      for(int i=0;i<7;i++){
     //   feat[i]=knn_feature[i];
        Serial.print(feat[i]);
        Serial.print(" ");
      }
      knnElection();
      
    }
    
    if (dataFile) {
       dataFile.println(dataString);
       dataFile.close();
       // print to the serial port too:
       //Serial.println(dataString);
    }
 }
  //Serial.println();
}
