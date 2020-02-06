#if !defined(_NEW_PULSEIN_H)
#define _NEW_PULSEIN_H

/* 負責超聲波取樣相關的函數 
 * exported function : 
 *  TIMER1_Init() : 初始化Timer相關設定。
 *  new_pulseIn() : 將US1跟US2的量測結果存放於US1_TIME跟US2_TIME全域變數中。執行時間約為55ms。
 *  new_pulseIn_filter() : 將量測結果濾波後儲存於US1_TIME_filtered跟US1_TIME_filtered全域變數中。
 *                      濾波方式 : 當US_TIME大於0時，US_TIME_filtered=US_TIME;
 *                                 當US_TIME等於於0時，US_TIME_filtered維持舊值，直到接收SUCCESS_ZEROS個0才變成0。
 * local function :
 *  TRIGHIGH :
 *  TRIGLOW :
 *  measure_Init() :
 */
//assume the velocity of sound is between 331+0.6*T = 346 m/s (T=25) and 343 m/s (T=20)

#define GAP 45
#define SUCCESS_ZEROS 10
#define Moving_average_m 5

#define TRIGHIGH1  (PORTD |=  0b00100000)  // 將PD5腳位設為HIGH
#define TRIGHIGH2  (PORTD |=  0b00010000)  // 將PD4腳位設為HIGH
#define TRIGLOW1   (PORTD &= ~0b00100000)  // 將PD5腳位設為LOW
#define TRIGLOW2   (PORTD &= ~0b00010000)  // 將PD4腳位設為LOW
int actionEndflag=0;
static volatile uint16_t overflowcounter;
uint8_t zero_cross_number=2;
uint8_t  zero_cross_flag=0;
float sum_slope=0;
float area_ratio=0;
float US_area=0;
float maxinumY=0;
float maxinumX=0;
float pre_maxinumY=0;
float pre_maxinumX=0;
float pre_US_slope_normal=0;
static volatile uint16_t Is_in_while_loop;
volatile uint16_t US1_TIME;
volatile uint16_t US2_TIME;
uint16_t US1_TIME_filtered;
uint16_t US2_TIME_filtered;
int US_TIME_diff;
float US_TIME_diff_normal=0;
float US_slope_normal=0;
float US_diff_slope_normal=0;
double action_length=0;
double action_length_US1=0;
double action_length_US2=0;
double action_length_both=0;
uint16_t US_order=0;
uint16_t slope_order=0;
uint16_t max_US1[5];
uint16_t max_US2[5];
float US_area_psum=0;
float US_area_nsum=0;


uint16_t A_US1_TIME_filtered=0;
uint16_t A_US2_TIME_filtered=0;
uint16_t A_US1_temp=0;
uint16_t A_US2_temp=0;
float a=0.2;//0.6110154703

static inline void TIMER1_Init();
static inline void measure_Init();
static inline void new_pulseIn();
static inline void new_pulseIn_filter();
int action_number=0;
int action_end_number=0;
int action_flag=0;
long action_plot=1;
int slope[10];
long slope_sum=0;
int slope_avg=0;
/* 初始化TIMER1 */
static inline void TIMER1_Init()
{
  noInterrupts();          // disable all interrupts
  TCCR1A = 0;              // TCCR1A Reset
  TCCR1B = 0;              // TCCR1B Reset
  TCNT1 = 0;
   
  TCCR1B |= (1 << CS10);    // no prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  
  interrupts();             // enable all interrupts
}

/* 量測前將TCNT1跟overflowcounter歸零 */
static inline void measure_Init1()
{
  TCNT1 = 0;
  overflowcounter = 0;
  Is_in_while_loop = 1;
  US1_TIME = 0;
  //US2_TIME = 0;
}
static inline void measure_Init2()
{
  TCNT1 = 0;
  overflowcounter = 0;
  Is_in_while_loop = 1;
  //US1_TIME = 0;
  US2_TIME = 0;
}

/* 新的pulseIn，可同時量測兩個hc_sr05的脈波長 */
static inline void new_pulseIn()
{
  TRIGHIGH1;
  delayMicroseconds(30);
  TRIGLOW1;
   measure_Init1();
  do {
    if(overflowcounter) {
      Is_in_while_loop = 0;
      break;
    }
  } while(1);
  delay(30);
  TRIGHIGH2;
  delayMicroseconds(30);
  TRIGLOW2;
   measure_Init2();
  //delayMicroseconds(250);
  do {
    if(overflowcounter) {
      Is_in_while_loop = 0;
      break;
    }
  } while(1);
  delay(30);
}

static uint16_t US1_history; // 儲存連續0的數量
static uint16_t US2_history; // 儲存連續0的數量
/* 預設:連續10個0時才轉成low */
static inline void A_pulseIn_filter()
{
  if(A_US1_TIME_filtered==0)
  {
    A_US1_TIME_filtered=US1_TIME;
    A_US1_temp=A_US1_TIME_filtered;
  }
  else
  {
    A_US1_TIME_filtered=US1_TIME*a+A_US1_temp*(1-a);
    A_US1_temp=A_US1_TIME_filtered;
  }

  if(A_US2_TIME_filtered==0)
  {
    A_US2_TIME_filtered=US2_TIME;
    A_US2_temp=A_US2_TIME_filtered;
  }
  else
  {
    A_US2_TIME_filtered=US2_TIME*a+A_US2_temp*(1-a);
    A_US2_temp=A_US2_TIME_filtered;
  }
  
}

static inline void new_pulseIn_filter()
{
  /* 若US1不等於0 */
  if(US1_TIME) {
    US1_TIME_filtered = US1_TIME;
    US1_history = 0;
  }
  /* 若US1等於0 */
  else if(US1_history < 1000) US1_history++;
  
  if(US1_history > SUCCESS_ZEROS) US1_TIME_filtered = 0;
  
  /* 若US2不等於0 */
  if(US2_TIME) {
    US2_TIME_filtered = US2_TIME;
    US2_history = 0;
  }
  /* 若US1等於0 */
  else if(US2_history < 1000) US2_history++;
  if(US2_history > SUCCESS_ZEROS) US2_TIME_filtered = 0;
}
static inline void max_filter()
{
  uint16_t US1_MAX=0; 
  uint16_t US2_MAX=0; 
  max_US1[US_order]=US1_TIME;
  max_US2[US_order]=US2_TIME;
  if(US_order==4){
      for(int i=0;i<5;i++){
          if(US1_MAX>max_US1[i]){
             US1_TIME_filtered=US1_MAX;
          }
          else{
             US1_MAX=max_US1[i];
             US1_TIME_filtered=US1_MAX;
          }
          if(US2_MAX>max_US2[i]){
             US2_TIME_filtered=US2_MAX;
          }
          else{
            US2_MAX=max_US2[i];
            US2_TIME_filtered=US2_MAX;
          }
      }
       /*for(int i=0;i<5;i++){
        Serial.print(max_US1[i]);
        Serial.print(",");
      }*/
      /*Serial.print(US1_TIME_filtered/65535.0);
      Serial.print(",");
      Serial.println(US2_TIME_filtered/65535.0);*/
      //Serial.print(",");
      for(int i=0;i<4;i++){
        max_US1[i]=max_US1[i+1];
        max_US2[i]=max_US2[i+1];
      }
  }
  else{
  US_order++;
  }
}
static inline void US_diff()
{
  US_TIME_diff=US1_TIME_filtered/2-US2_TIME_filtered/2;
  US_TIME_diff_normal=US_TIME_diff/65535.0;
  //Serial.print(US_TIME_diff_normal);
  //Serial.print(",");
  /*Serial.print(US1_TIME_filtered);
  Serial.print(",");
  Serial.println(US2_TIME_filtered);*/
  
}
void check_action_end()
{
  if(slope_avg==0)
  {
    action_end_number++;
  }
  else
  {
    action_end_number=0;
  }
  if(action_end_number==5)
  {
    if(pre_maxinumY!=0)
      {
        sum_slope+=(maxinumY-pre_maxinumY)/(maxinumX-pre_maxinumX);
         Serial.println(maxinumY);
         Serial.println(pre_maxinumY);
         Serial.println(maxinumX);
         Serial.println(pre_maxinumX);
      }
    area_ratio=US_area_psum/(US_area_psum+US_area_nsum);

    Serial.println(US_area_psum);
    Serial.println(",");
    Serial.println(US_area_nsum);
    Serial.println(",");
    action_flag=0;
    actionEndflag=1;
    Serial.print("zero_cross_number : ");
    Serial.println(zero_cross_number);
    Serial.print("area_ratio : ");
    Serial.println(area_ratio);
    Serial.print("sum_slope : ");
    Serial.println(sum_slope);
    Serial.print("action_length : ");
    Serial.println(action_length);
    Serial.print("action_length_US1/action_length : ");
    Serial.println(action_length_US1/action_length);
    Serial.print("action_length_US2/action_length : ");
    Serial.println(action_length_US2/action_length);
    Serial.print("action_length_both/action_length : ");
    Serial.println(action_length_both/action_length);
    //Serial.println("action end ! ");
    action_plot=1;
    action_end_number=0;
    US_area_psum=0;
    US_area_nsum=0;
    action_length=0;
    zero_cross_number=2;
    
     pre_maxinumY=0;
      pre_maxinumX=0;
      maxinumY=0;
      maxinumX=0;
      action_length_US1=0;
      action_length_US2=0;
      action_length_both=0;
  }
  
}
void inline static area_cal()
{
  if(US_area>0)
  US_area_psum+=US_area;
  else
  US_area_nsum-=US_area;

  
}
void inline static zero_cross_count(float US_slope_normal)
{
  if(US_slope_normal>0.1||US_slope_normal<-0.1)
  zero_cross_flag=1;

  
  if(zero_cross_flag)
  {
      if(pre_US_slope_normal*US_slope_normal<0)
      {
        if(pre_maxinumY!=0)
        {
          sum_slope+=(maxinumY-pre_maxinumY)/(maxinumX-pre_maxinumX);
           Serial.println(maxinumY);
           Serial.println(pre_maxinumY);
           Serial.println(maxinumX);
           Serial.println(pre_maxinumX);
        }
      pre_maxinumY=maxinumY;
      pre_maxinumX=maxinumX;
      Serial.println(pre_maxinumX);
      Serial.println(pre_maxinumY);
      maxinumY=0;
      maxinumX=0;
      zero_cross_flag=0;
      zero_cross_number++;
      }
     else if(US_slope_normal>0)
    {
      if(US_slope_normal>=maxinumY)
      {
        maxinumY=US_slope_normal;
        maxinumX=action_length;
      }
    }
    else if(US_slope_normal<0)
    {
      if(US_slope_normal<=maxinumY)
      {
        maxinumY=US_slope_normal;
        maxinumX=action_length;
      }
    }
    
  }
  pre_US_slope_normal=US_slope_normal;
  
}
static inline void US_slope()
{
  
  slope[slope_order]=US_TIME_diff;
  
  if(slope_order==Moving_average_m-1){
     slope_sum+=slope[slope_order];
     slope_avg=slope_sum/Moving_average_m;
     US_slope_normal=slope_avg/65535.0;
     slope_sum-=slope[0];
     for(int i=0;i<Moving_average_m-1;i++){
       slope[i]=slope[i+1];
     }
    US_diff_slope_normal=US_TIME_diff_normal-US_slope_normal;
    Serial.print("@");
    Serial.print(":");
    Serial.print(US1_TIME_filtered);
    Serial.print(",");
    Serial.print(US2_TIME_filtered);
    Serial.print(",");
    Serial.print(US_TIME_diff);
    Serial.print(",");
    Serial.print(US_TIME_diff_normal);
    Serial.print(",");
    Serial.print(US_slope_normal);
    Serial.print(",");
    Serial.print(US_diff_slope_normal);
    US_diff_slope_normal*=100;
    Serial.print(",");
    Serial.print(slope_sum);
    Serial.print(",");
    Serial.print(slope_avg);
    Serial.print(",");
    Serial.print(US_diff_slope_normal);
    Serial.print(",");
    
    if(US_diff_slope_normal>5 || US_diff_slope_normal<(-5))
    US_area=slope_avg/65535.0;
    else
    US_area=0;
    Serial.print(slope_avg/65535.0);
    Serial.print(",");
    Serial.print(action_plot);
    Serial.print(",");
    Serial.print(US_area);
    Serial.print(",");
//   Serial.print(US_area_psum);
//  Serial.print(",");
//  Serial.print(US_area_nsum);
//  Serial.print(",");
   Serial.print(action_length);
   Serial.print(",");
   Serial.print(zero_cross_number);
   Serial.print(",");
   Serial.print(sum_slope);
   Serial.print(",");
   Serial.print(action_length_US1/action_length);
   Serial.print(",");
   Serial.print(action_length_US2/action_length);
   Serial.print(",");
   Serial.println(area_ratio);
  }
  else{
    slope_sum+=slope[slope_order];
    slope_order++;
  }
  
  if(action_flag==1)
  {
    
    action_length+=0.01;
    if(US1_TIME_filtered>0&&US2_TIME_filtered>0)
    {
       action_length_US1+=0.01;
       action_length_US2+=0.01;
       action_length_both+=0.01;
    }
    else if(US1_TIME_filtered>0)
    action_length_US1+=0.01;
    else if(US2_TIME_filtered>0)
    action_length_US2+=0.01;
    
    area_cal();
    zero_cross_count(US_slope_normal);
    check_action_end();
    
  }
  else{
    if(slope_avg!=0)
    {
      action_number++;
    }
    else
    {
      action_number=0;
    }
    if(action_number==5)
    {
      sum_slope=0;
      action_flag=1;
     action_plot=2;
      //Serial.println("action start ! ");
    }
    
  }
}

void ISR_US1()
{
  /* 紀錄US1的falling時間 */
  US1_TIME = TCNT1 * Is_in_while_loop;
}

void ISR_US2()
{
  /* 紀錄US2的falling時間 */
  US2_TIME = TCNT1 * Is_in_while_loop;
}

ISR (TIMER1_OVF_vect)
{
  /* 紀錄overflow發生的次數，並將TCNT1歸零 */
  TCNT1 = 0;
  overflowcounter++;
}

#endif
