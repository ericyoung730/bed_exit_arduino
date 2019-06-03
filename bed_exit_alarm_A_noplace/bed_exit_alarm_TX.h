/* ir腳位 */
#define ir_1 A0
#define ir_2 A1
#define ir_3 A2
#define ir_4 A3
#define ir_5 11
#define ir_6 10
#define ir_7 8
#define ir_8 9

/* US腳位 */
#define TRIGPIN1 5
#define ECHOPIN1 2
#define TRIGPIN2 4 
#define ECHOPIN2 3 
/* threshold */
#define US_dist_th   3480    // 超聲波開始計數的距離，須在 60公分以內才觸發
#define US_dist_thr  180000   // 超聲波開始計數的距離，須在 60公分以內才觸發
#define US_trig_th    2    // 超聲波連續觸發的門檻，須連續 3次觸發才發出訊號
#define IR_count_th   3    // 紅外線觸發數量的門檻，須大於 4個才算觸發
#define IR_trig_th    4    // 紅外線連續觸發的門檻，須連續 4次觸發才發出訊號

static inline void gpio_Init() {
  /* 配置 ir讀取腳位 */
  /*pinMode(ir_1, INPUT);
  pinMode(ir_2, INPUT);
  pinMode(ir_3, INPUT);
  pinMode(ir_4, INPUT);
  pinMode(ir_5, INPUT);
  pinMode(ir_6, INPUT);
  pinMode(ir_7, INPUT);
  pinMode(ir_8, INPUT);*/

  /* 配置 US腳位 */
  pinMode(TRIGPIN1, OUTPUT);
  pinMode(TRIGPIN2, OUTPUT);
  pinMode(ECHOPIN1, INPUT);
  pinMode(ECHOPIN2, INPUT);
}
