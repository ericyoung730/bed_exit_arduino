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

#define GAP 45
#define SUCCESS_ZEROS 10

#define TRIGHIGH  (PORTD |=  0b00110000)  // 將PD4跟PD5腳位設為HIGH
#define TRIGLOW   (PORTD &= ~0b00110000)  // 將PD4跟PD5腳位設為LOW

static volatile uint16_t overflowcounter;

static volatile uint16_t Is_in_while_loop;
volatile uint16_t US1_TIME;
volatile uint16_t US2_TIME;
uint16_t US1_TIME_filtered;
uint16_t US2_TIME_filtered;

static inline void TIMER1_Init();
static inline void measure_Init();
static inline void new_pulseIn();
static inline void new_pulseIn_filter();

/* 初始化TIMER1 */
static inline void TIMER1_Init()
{
  TCCR1A = 0;
  TCCR1B = 0x1;
  TIMSK1 = 0b00000001;
  TCNT1 = 0;
}

/* 量測前將TCNT1跟overflowcounter歸零 */
static inline void measure_Init()
{
  TCNT1 = 0;
  overflowcounter = 0;
  Is_in_while_loop = 1;
  US1_TIME = 0;
  US2_TIME = 0;
}

/* 新的pulseIn，可同時量測兩個hc_sr05的脈波長 */
static inline void new_pulseIn()
{
  TRIGHIGH;
  delayMicroseconds(30);
  TRIGLOW;
  measure_Init();
  do {
    if(overflowcounter) {
      Is_in_while_loop = 0;
      break;
    }
  } while(1);
  delay(GAP);
}

static uint16_t US1_history; // 儲存連續0的數量
static uint16_t US2_history; // 儲存連續0的數量
/* 預設:連續10個0時才轉成low */
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