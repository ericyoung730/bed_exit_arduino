#if !defined(_DIRECTION_DETECT_H)
#define _DIRECTION_DETECT_H

#define Direction_Undefined 0x0
#define Direction_Leave    0x1
#define Direction_Stay     0x2


/* US1 : echo : 2，頭側
 * US2 : echo : 3，尾側
 */

/* TODO:
 *  1. US1 & US2大於0 ---> US1比US2早結束 ---> US2結束 ---> 0 ---> 判定為尾側
 *                   |-> US2比US1早結束or同時結束 ---> US2結束 ---> 0 ---> 判定為頭側
 *                   |-> US1比US2早結束 ---> US1在US2結束前開始 ---> US1先結束 ---> 0 ---> 判定為尾側
 *                                                            |-> US2先結束or同時 ---> 0 ---> 判定為頭側
 *                   |-> US2比US1早結束 ---> US2在US1結束前開始 ---> US1先結束 ---> 0 ---> 判定為尾側
 *                                                            |-> US2先結束or同時 ---> 0 ---> 判定為頭側
 *  2. US1 or US2大於0 ---> ignore
 */

extern volatile uint16_t US1_TIME;
extern volatile uint16_t US2_TIME;
extern uint16_t US1_TIME_filtered;
extern uint16_t US2_TIME_filtered;

/* declare global var */
static uint16_t US1_TIME_filtered_last = 0;
static uint16_t US2_TIME_filtered_last = 0;
/* 0表示前次Update_Direction_available()回傳的結果是false；1表示前次回傳的結果是true，並會在Update_Direction_available()下一次執行時變回0。 */
static uint8_t last_Update_Direction_available = 0; 

/* exported function */
/* 在S1跟US2都為LOW之前，回傳上次的結果；若US1跟US2都為LOW，則回傳這次的結果。 */
static inline uint8_t Get_Direction();
            
/* local function */
/* 若2個US都還沒有變成0，回傳false；若2個US都為0，且上一次的結果不為0，則回傳true；若2個US都為0，且上一次的結果為0，則回false。 */ 
static inline boolean Update_Direction_available(); 
/* 檢查是否有發生US1跟US2同時為High的情況。若有，回傳true；維持true，直到last_Update_Direction_available = 1時回傳false(代表方向已經重新更新) */
static inline boolean Check_Overlap_Happen();

static volatile uint8_t Last_Direction = Direction_Undefined;
static inline uint8_t Get_Direction(bool reset)
{  
  /* 結果有更新，因此取得新的移動方向 */
  if(reset) {
    Last_Direction = Direction_Stay;
    //Serial.println("I AM THERE");
  }
  else if(Update_Direction_available()) {
    /*  判斷US1 or US2哪個比較晚結束
     *  三種情況 :
     *  1. US2_TIME_filtered_last = 0 --> US1較晚結束 --> Stay
     *  2. US1_TIME_filtered_last = 0 --> US2較晚結束 --> Leave
     *  3. US1_TIME_filtered_last & US2_TIME_filtered_last != 0 --> US1跟US2同時結束 --> 判定為US1較晚結束
     */
     //Serial.print("I AM HERE");
    if(US2_TIME_filtered_last == 0) Last_Direction = Direction_Stay;
    else if(US1_TIME_filtered_last == 0) Last_Direction = Direction_Leave;
    else if(US1_TIME_filtered_last || US2_TIME_filtered_last) Last_Direction = Direction_Stay;
    else Last_Direction = Direction_Undefined;
    
  }
  
  return Last_Direction;
}

static boolean Overlap_Happen = false;  /* 確認是否有發生US1跟US2同時為High的情況，有的話才視作有效。 */
static inline boolean Check_Overlap_Happen()
{
  if(US1_TIME_filtered && US2_TIME_filtered) Overlap_Happen = true;
  return Overlap_Happen;
}

static inline boolean Update_Direction_available()
{
  /* check overlap happen */
  if(Check_Overlap_Happen() == false) return false;
  /* 檢驗2個US是不是0，不是則回傳false */
  if(US1_TIME_filtered || US2_TIME_filtered) {
    
    /* 紀錄最後一次的US1_TIME_filtered跟US2_TIME_filtered */
    US1_TIME_filtered_last = US1_TIME_filtered;
    US2_TIME_filtered_last = US2_TIME_filtered;
    
    last_Update_Direction_available = 0;
    return false;
  }
  
  /* 檢驗上一次呼叫函數的結果是不是1，若是，表示已經更新過方向了，所以回傳false */
  if(last_Update_Direction_available) {
    Overlap_Happen = false;
    return false;
  }
  
  /* 檢驗上一次呼叫函數的結果是不是0，若是，表示還沒更新過方向了，所以回傳true */
  else {
    last_Update_Direction_available++;
    return true;
  }
}

#endif
