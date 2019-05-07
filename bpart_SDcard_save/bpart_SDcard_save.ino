#include "bed_exit_alarm_TX.h"
#include "MPU6050.h"
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <DS3231.h>
DS3231  rtc(SDA, SCL);  //SDA = A4 SCL = A5
MPU6050 accelgyro;
int16_t ay; //the acc value of bar leanning
const int chipSelect = 10; //CS = 10
String dataString = "";
String fileName = "bsave.txt";
Time t;
int last_IR;

long previousMillis_MPU = 0;
long previousMillis_ir = 0;
long mpu_interval = 500;
long ir_interval = 100;

static byte current_railing_status = 0;  //0 for save ; 1 for trigger
static byte last_railing_status = 0;

static uint8_t tmp_IR = 0;
static byte last_IR_status = 0x0;        //0 for save ; 1 for trigger
static byte current_IR_status = 0x0;

void setup() {
  Serial.begin(115200);
  Serial.println("setup");
  Wire.begin();             // active i2c
  gpio_Init();              // active gpio
  rtc.begin();
  rtc.setTime(0,0,0);
  accelgyro.initialize();
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
      
  delay(1000);
   
}



void loop() {
 t=rtc.getTime();
/*****************MPU6050*********************************/
  if(millis()-previousMillis_MPU>mpu_interval) {
    Serial.println(int(t.sec));
    delay(1);
    ay = accelgyro.getAccelerationY();
    //Serial.println(ay);
    if(abs(ay) > 7000) {               //save condition
      current_railing_status = 0; 
      delay(1);
    }
    else{
      current_railing_status = 1;      //trigger condition
      delay(1);  
    }
    if(last_railing_status == 0){
      if(current_railing_status==1){
        last_railing_status=1;
        Serial.println("send rail");
        dataString="rail :";
        dataString +=String(t.hour);
        dataString +=":";
        dataString +=String(t.min);
        dataString +=":";
        dataString +=String(t.sec);
        File dataFile = SD.open(fileName, FILE_WRITE);
      if (dataFile) {
          dataFile.println(dataString);
          dataFile.close();
      }
      }   
    delay(1);
    }
    last_railing_status=current_railing_status;
    previousMillis_MPU=millis();
  }

/*****************IR_sensor*************************************/
  tmp_IR = digitalRead(ir_1);
  tmp_IR += digitalRead(ir_2);
  tmp_IR += digitalRead(ir_3);
  tmp_IR += digitalRead(ir_4);
  tmp_IR += digitalRead(ir_5);
  tmp_IR += digitalRead(ir_6);
  tmp_IR += digitalRead(ir_7);
  tmp_IR += digitalRead(ir_8);
  /*Serial.println("tmp_IR");
  Serial.println(digitalRead(ir_1));
  Serial.println(digitalRead(ir_2));
  Serial.println(digitalRead(ir_3));
  Serial.println(digitalRead(ir_4));
  Serial.println(digitalRead(ir_5));
  Serial.println(digitalRead(ir_6));
  Serial.println(digitalRead(ir_7));
  Serial.println(digitalRead(ir_8));*/
  //Serial.println(tmp_IR);
  if(tmp_IR >= IR_count_th)
    last_IR++;
  if(millis()-previousMillis_ir>ir_interval){
      if(last_IR >= IR_trig_th) 
         current_IR_status = 0x1;  
      else  
         current_IR_status = 0x0;
      previousMillis_ir=millis();
      last_IR = 0;
    }
    if(last_IR_status==0x1){
      if(current_IR_status==0x0){
         delay(1);
         Serial.println("send ir leave");
         dataString="ir leave :";
         dataString +=String(t.hour);
         dataString +=":";
         dataString +=String(t.min);
         dataString +=":";
         dataString +=String(t.sec);
         File dataFile = SD.open(fileName, FILE_WRITE);
      if (dataFile) {
          dataFile.println(dataString);
          dataFile.close();
      }
      }
    }
    else if(last_IR_status==0x0){
         if(current_IR_status==0x1){ 
            delay(1);
            Serial.println("send ir trigger ");
            dataString="ir trig :";
            dataString +=String(t.hour);
            dataString +=":";
            dataString +=String(t.min);
            dataString +=":";
            dataString +=String(t.sec);
            File dataFile = SD.open(fileName, FILE_WRITE);
         if (dataFile) {
          dataFile.println(dataString);
          dataFile.close();
      }
         }
    }
    last_IR_status = current_IR_status;  
}
