/*#include <Wire.h>  
//#include <Adafruit_GFX.h>  
#include <Adafruit_SSD1306.h>  
#define OLED_RESET -1  
Adafruit_SSD1306 display(OLED_RESET);  
  */
#include "new_pulseIn.h"
#include "Direction_Detect.h"
#include <SPI.h>
#include <SD.h>
#include <DS3231.h>
DS3231  rtc(SDA, SCL);  //SDA = A4 SCL = A5
Time t;

const int chipSelect = 10; //CS = 10
String dataString = "";
String fileName = "";
extern volatile uint16_t US1_TIME;
extern volatile uint16_t US2_TIME;
unsigned long pretime=0;
unsigned long interval=20000;
int save_switch = 1;  // 1 for stop save data ;0 for start save data
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
  rtc.setTime(0, 0, 0);     // Set the time to 12:00:00 (24hr format)
  pinMode(9, OUTPUT);    //output reset
  pinMode(4, OUTPUT);    //trig2
  pinMode(5, OUTPUT);    //trig1 
  pinMode(2, INPUT);     //echo1
  pinMode(3, INPUT);     //echo2  
  pinMode(8,INPUT_PULLUP);  //pullup pin8
  attachInterrupt(0, ISR_US1, FALLING);
  attachInterrupt(1, ISR_US2, FALLING);

  digitalWrite(9,HIGH);
  ////////////////check SD card module//////////////////
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  //////////////////////////////////////////////////////
 
}
static inline void print_Serial_Plotter_nofiltered()
{
  Serial.print(US1_TIME);
  Serial.print(",");
  Serial.print(US2_TIME); 
   dataString=String(US1_TIME);
    dataString += ",";
     dataString +=String(US2_TIME);
}
void loop() {
  if(millis()-pretime>interval)
  {
    pretime=millis();
    digitalWrite(9,LOW);
    delay(100);
    Serial.print("here");
    digitalWrite(9,HIGH);
  }
  save_switch = digitalRead(8); //read switch
                //read US data
  //A_pulseIn_filter();
  //curtime=millis();
  t=rtc.getTime();
  //Serial.println(int(t.min));
  //delay(1000);
  //print_Serial_Plotter_nofiltered();
  
 if(save_switch){
  writing=false;
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
          fileName+=String(t.min);
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
       fileName+=String(t.min);
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
    if (dataFile) {
       dataFile.println(dataString);
       dataFile.close();
       // print to the serial port too:
       //Serial.println(dataString);
    }
 }
  Serial.println();
}
