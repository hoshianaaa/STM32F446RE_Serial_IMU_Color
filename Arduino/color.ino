#include <Wire.h>
#include "Adafruit_TCS34725.h"
#define DEBUG 0

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
char GetColor();

//送信データ形式:[header, color]
uint8_t txbuf[] = {255, 'n'};

const int red_led = 12;
const int blue_led = 10;
const int yellow_led = 8;



void setup() {
  
  Serial.begin(115200);
  pinMode(red_led, OUTPUT);
  pinMode(blue_led, OUTPUT);
  pinMode(yellow_led, OUTPUT);
  //カラーセンサセットアップ:失敗で無限ループ
  if (tcs.begin()) {
    if (DEBUG)Serial.println("Found color sensor");
  } else {
    if (DEBUG)Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
}

void loop() {
  txbuf[1] = GetColor();//黄色:'y' 赤:'r' 青:'b' 無し:'n'
  for (int i = 0; i < sizeof(txbuf); i++)Serial.write(txbuf[i]);
}



char GetColor() {

  uint16_t clear, red, green, blue;

  tcs.getRawData(&red, &green, &blue, &clear);

  char color;
  /*
  if (red < 45 && green < 45 && blue > 25){color = 'b';blue_on();}
  else if (red > 50 && green > 30){color = 'y';yellow_on();}
  else if (red > 35 && green  > 25 && blue > 15){color = 'r';red_on();}
  else color = 'n';
  */

  if(46 < blue) {color = 'n';led_off();}
  if(62 > red && 56 > green){color = 'b';blue_on();}
  if(71 > red && 61 > green){color = 'r';red_on();}
  else {color = 'y';yellow_on();}
  
  if (DEBUG) {
    Serial.print(" "); Serial.print(red);
    Serial.print(" "); Serial.print(green);
    Serial.print(" "); Serial.print(blue);
    Serial.print(" "); Serial.println(color);
  }
  return color;
}

void red_on(){
  digitalWrite(red_led, HIGH);
  digitalWrite(blue_led, LOW);
  digitalWrite(yellow_led, LOW);
}

void yellow_on(){
  digitalWrite(red_led, LOW);
  digitalWrite(blue_led, LOW);
  digitalWrite(yellow_led, HIGH);
}

void blue_on(){
  digitalWrite(red_led, LOW);
  digitalWrite(yellow_led, LOW);
  digitalWrite(blue_led, HIGH);
}

void led_off(){
  digitalWrite(red_led, LOW);
  digitalWrite(yellow_led, LOW);
  digitalWrite(blue_led, LOW);
}
