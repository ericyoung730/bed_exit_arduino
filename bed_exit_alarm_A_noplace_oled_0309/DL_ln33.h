#include <SoftwareSerial.h>

// 定義 channel常數
#define baud 115200
#define channel 0x14
#define ID 0x7E31
#define Serial_head 0xFE
#define Serial_tail 0xFF
#define Serial_control_channel 0x21
#define Serial_general_channel 0x90
#define Self_control_address 0x0000
#define Master_address 0x1069
// 定義 command常數////////////////////////////////
#define Set_get_address 0x01
#define Set_ID 0x12
#define Set_channel 0x13
#define Set_restart 0x10
// 定義 register sign
#define register_ask 0x31
// 定義 type_data & type_ins
#define type_data 0x0
#define type_ins 0x1
// 定義函式
#define swap_get_high_niddle(x) (x & 0xff)
#define swap_get_low_niddle(x) (x>>8)
#define merge_swap_niddle(high_niddle, low_niddle) ((high_niddle & 0xff) + (((low_niddle<<8) & 0xff00))) 

// 資料形式：
uint16_t Serial_my_address;
uint8_t tmpseq[50];

// 宣告 SoftwareSerial
SoftwareSerial mys(6, 7); // RX TX//////////

static inline void ln33_Serial_Init()
{
  mys.begin(115200);
}

static inline void ln33_channel_Init() {
  uint8_t TX_length = 0x06;
  tmpseq[0] = Serial_head;
  tmpseq[1] = TX_length;
  tmpseq[2] = Serial_general_channel;
  tmpseq[3] = Serial_control_channel;
  tmpseq[4] = swap_get_high_niddle(Self_control_address);
  tmpseq[5] = swap_get_low_niddle(Self_control_address);
  tmpseq[6] = Set_channel;
  tmpseq[7] = channel;
  tmpseq[8] = Serial_tail;
//  Serial.write(tmpseq, TX_length+3);
  mys.write(tmpseq, TX_length+3);
}

static inline void ln33_ID_Init() {
  uint8_t TX_length = 0x07;
  tmpseq[0] = Serial_head;
  tmpseq[1] = TX_length;
  tmpseq[2] = Serial_general_channel;
  tmpseq[3] = Serial_control_channel;
  tmpseq[4] = swap_get_high_niddle(Self_control_address);
  tmpseq[5] = swap_get_low_niddle(Self_control_address);
  tmpseq[6] = Set_ID;
  tmpseq[7] = swap_get_high_niddle(ID);
  tmpseq[8] = swap_get_low_niddle(ID);
  tmpseq[9] = Serial_tail;
//  Serial.write(tmpseq, TX_length+3);
  mys.write(tmpseq, TX_length+3);
}

static inline void ln33_Restart() {
  uint8_t TX_length = 0x05;
  tmpseq[0] = Serial_head;
  tmpseq[1] = TX_length;
  tmpseq[2] = Serial_general_channel;
  tmpseq[3] = Serial_control_channel;
  tmpseq[4] = swap_get_high_niddle(Self_control_address);
  tmpseq[5] = swap_get_low_niddle(Self_control_address);
  tmpseq[6] = Set_restart;
  tmpseq[7] = Serial_tail;
//  Serial.write(tmpseq, TX_length+3);
  mys.write(tmpseq, TX_length+3);
  delay(10000);
}
static inline void ln33_transmit_register()
{
  uint8_t TX_length = 0x05;
  tmpseq[0] = Serial_head;
  tmpseq[1] = TX_length;
  tmpseq[2] = Serial_general_channel;
  tmpseq[3] = Serial_general_channel;
  tmpseq[4] = swap_get_high_niddle(Master_address);
  tmpseq[5] = swap_get_low_niddle(Master_address);
  tmpseq[6] = type_ins;
  tmpseq[7] = Serial_tail;
  mys.write(tmpseq, TX_length+3);
}
static inline bool ln33_check_register(byte b[])
{
  if(b[0] != 0xFE) return false;
  if(b[1] != 0x05) return false;
  if(b[2] != 0x90) return false;
  if(b[3] != 0x90) return false;
  if(b[4] != swap_get_high_niddle(Master_address)) return false;
  if(b[5] != swap_get_low_niddle(Master_address)) return false;
  if(b[6] != register_ask) return false;
  if(b[7] != 0xFF) return false;
  return true;
}
static inline void ln33_transmit_data(byte IR_data, byte rail) {
  uint8_t TX_length = 0x07;
  tmpseq[0] = Serial_head;
  tmpseq[1] = TX_length;
  tmpseq[2] = Serial_general_channel;
  tmpseq[3] = Serial_general_channel;
  tmpseq[4] = swap_get_high_niddle(Master_address);
  tmpseq[5] = swap_get_low_niddle(Master_address);
  tmpseq[6] = type_data;
  tmpseq[7] = IR_data;
  tmpseq[8] = rail;
  tmpseq[9] = Serial_tail;
//  Serial.write(tmpseq, TX_length+3);
  mys.write(tmpseq, TX_length+3);
}
