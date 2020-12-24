#include "wifi.h"
#include <SoftwareSerial.h>
#include <FastLED.h>
//https://www.mydigit.cn/forum.php?mod=viewthread&tid=112534
//https://developer.tuya.com/cn/docs/iot/device-development/tuya-development-board-kit/tuya-sandwich-evaluation-kits/development-guide/arduino-implements-simple-demo?id=K96yyxgetltzj
SoftwareSerial softSerial(12, 14); // RX, TX
#define _SS_MAX_RX_BUFF 300
#define ledpin 13  //开关电源引脚，高电平为开，低电平为关
#define PWM_DIN 5  //esp8266 din引脚，控制色彩
#define NUM_LEDS    7    //我的灯带一共级联了12颗LED

CRGB leds[NUM_LEDS];  

int time_cnt = 0, cnt = 0, init_flag = 0;

void setup() {
  pinMode(ledpin, OUTPUT);   // led I/O初始化
  digitalWrite(ledpin, LOW);
  FastLED.addLeds<WS2812, PWM_DIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);  //可以设置全局亮度，调低亮度不刺眼
  

  //pinMode(PC13, INPUT);     // 重置Wi-Fi按键初始化
  //pinMode(8, OUTPUT);       // Wi-Fi状态指示灯初始化

  softSerial.begin(9600);     // 软件串口初始化
  softSerial.println("myserial init successful!");
  Serial.begin(115200);     // PA3 RX   PA2 TX
  Serial.println("serial init successful!");

  wifi_protocol_init();
}

void loop() {
  // if (init_flag == 0) {
  //   time_cnt++;
  //   if (time_cnt % 6000 == 0) {
  //     time_cnt = 0;
  //     cnt ++;
  //   }
//    wifi_stat_led(&cnt);   // Wi-Fi状态处理
  // }
  wifi_uart_service();
  serialEvent();      // 串口接收处理
//  key_scan();           // 重置配网按键检测

}

void serialEvent() {
  if (softSerial.available()) {
    unsigned char ch = (unsigned char)softSerial.read();
    uart_receive_input(ch);
    //Serial.write(ch);
  }
}

void serialWrite(unsigned char value){
  softSerial.write(value);
}
//开电源引脚
void LED_UP()
{
  digitalWrite(ledpin, HIGH);
  ShowClolor(100,255,255);
}
//关电源引脚
void LED_DOWN()
{
  digitalWrite(ledpin, LOW);
}

//HSV:[0,255]；H（色度：0-360，0X0000-0X0168）;S (饱和：0-1000, 0X0000-0X03E8);V (明度：0-1000，0X0000-0X03E8)
void ShowClolor(unsigned long h,unsigned long s,unsigned long v) {
  int H = map(h,0,360,0,255);
  int S = map(s,0,1000,0,255);
  int V = map(v,0,1000,0,255);
  Serial.println("ccc");
  ShowClolorRaw(H,S,V);
}

void ShowClolorRaw(unsigned long H,unsigned long S,unsigned long V){
  for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(H,S,V); //用HSV色彩空间，色调h，饱和度S，亮度V
      FastLED.show();
    }
}

//设置亮度，fastLED范围[0,255],涂鸦范围[10,1000]
void setBrightness(unsigned long bright_value){
  FastLED.setBrightness(map(bright_value,10,1000,1,255));
  FastLED.show();
}
//白光模式
void setWhite(){
  ShowClolorRaw(128,128,255);
}

//渐变彩虹色
void setRainbow(){
  //debug
  mcu_dp_value_update(DPID_COUNTDOWN,1);
  fill_gradient(leds, 0, CHSV(50, 255,255) , NUM_LEDS, CHSV(150,255,255), LONGEST_HUES);
  FastLED.show();
}

//闪烁渐变
void loopColor(){
  //debug
  //mcu_dp_value_update(DPID_COUNTDOWN,2);
  
  int h = 0;
  while(1){
    for (int i = 0; i < NUM_LEDS; i++) {
          leds[i] = CHSV( h, 255, 255); //用HSV色彩空间，不断改变H即可
          FastLED.show();
      }
    delay(1000);
    h = (h + 1) % 255;
  }
}

int string_int(unsigned char temp_char){
  if(temp_char>='0' and temp_char<='9'){
    return temp_char-'0';
  }
  if(temp_char>='A' and temp_char<='Z'){
    return temp_char - 'A' + 10;
  }
  if(temp_char>='a' and temp_char<='z'){
    return temp_char - 'a' + 10;
  }
}
