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

#define TRIGHIGH1  (PORTD |=  0b00100000)  // 將PD5腳位設為HIGH
#define TRIGHIGH2  (PORTD |=  0b00010000)  // 將PD4腳位設為HIGH
#define TRIGLOW1   (PORTD &= ~0b00100000)  // 將PD5腳位設為LOW
#define TRIGLOW2   (PORTD &= ~0b00010000)  // 將PD4腳位設為LOW
static volatile uint16_t overflowcounter;

static volatile uint16_t Is_in_while_loop;
volatile uint16_t US1_TIME;
volatile uint16_t US2_TIME;
uint16_t US1_TIME_filtered;
uint16_t US2_TIME_filtered;
uint16_t A_US1_TIME_filtered=0;
uint16_t A_US2_TIME_filtered=0;
uint16_t A_US1_temp=0;
uint16_t A_US2_temp=0;
float a=0.2;//0.6110154703

static inline void TIMER1_Init();
static inline void measure_Init();
static inline void new_pulseIn();
static inline void new_pulseIn_filter();

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
