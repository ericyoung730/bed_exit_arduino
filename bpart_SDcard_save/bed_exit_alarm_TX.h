/* ir腳位 */
#define ir_1 A0
#define ir_2 A1
#define ir_3 A2
#define ir_4 A3
#define ir_5 5
#define ir_6 4
#define ir_7 3
#define ir_8 2

#define control2 10
#define control1 9
#define IR_count_th   4    // 紅外線觸發數量的門檻，須大於 4個才算觸發
#define IR_trig_th    4  // 紅外線連續觸發的門檻，須連續 4次觸發才發出訊號

static inline void gpio_Init() {
  /* 配置 ir讀取腳位 */
  pinMode(ir_1, INPUT);
  pinMode(ir_2, INPUT);
  pinMode(ir_3, INPUT);
  pinMode(ir_4, INPUT);
  pinMode(ir_5, INPUT);
  pinMode(ir_6, INPUT);
  pinMode(ir_7, INPUT);
  pinMode(ir_8, INPUT);
pinMode(control1, OUTPUT);
pinMode(control2, OUTPUT);
}
