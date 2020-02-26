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
#define TRIGHIGH1  (PORTD |=  0b00100000)  // 將PD5腳位設為HIGH
#define TRIGHIGH2  (PORTD |=  0b00010000)  // 將PD4腳位設為HIGH
#define TRIGLOW1   (PORTD &= ~0b00100000)  // 將PD5腳位設為LOW
#define TRIGLOW2   (PORTD &= ~0b00010000)  // 將PD4腳位設為LOW
static volatile uint16_t overflowcounter;
static volatile uint16_t Is_in_while_loop;
volatile uint16_t US1_TIME;
volatile uint16_t US2_TIME;
static inline void TIMER1_Init();
static inline void measure_Init();
static inline void new_pulseIn();
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
