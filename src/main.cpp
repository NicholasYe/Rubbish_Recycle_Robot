#include <Arduino.h>
// 引脚定义
#include <Servo.h>
#include <SoftwareSerial.h>
SoftwareSerial MyBlue(16, 17); // RX | TX
Servo servo1;
Servo servo2;
int r;
int M_PWM[4] = {9, 10, 11, 12}; //电机PWM，依次对应 LF,LB,RB,RF  L-Left R-Right F-Front B-Back
int M_Dir[8] = {22, 23, 24, 25, 26, 27, 29, 28}; //控制电机正反转，依次对应 LF,LB,RB,RF,(一个电机需要两个，对应IN1,IN2)
int Switch[8] = {46, 47, 48, 49, 50, 51, 52, 53}; //光电开关 依次为 F,L,B,R  LF,LB,RB,RF
int Gray[2] = {A0, A2}; // 数组为int,与Arduino底层有关，不是错误 F,L,B,R
int v_pwm[4] = {120, 120, 120, 120};
int v_dir[4] = {1, 1, 1, 1};
void hei_ren_tai_guan();
#define N 250000  //tone时间
#define pin 18
#define m 250  //delay时间
int f[3][7] = {{392, 440, 494}, {523, 587, 659, 698, 784, 880, 988}, {1047}}; //频率，低（音符）5,6,7，中1,2,3,4,5,6,7，高1
int i;
// 台上台下状态值
bool on_stage;

/*加一个备用方案
   函数作用：控制一路电机的速度和正反转
   输入值：ch-电机号（0,1,2,3），dir-正反转（1,0），vel-速度（0~255）
   返回值：无
*/
void Change_PWM(int ch, int dir, int vel = -1) {
  digitalWrite(M_Dir[2 * ch], dir);
  digitalWrite(M_Dir[2 * ch + 1], !dir);
  if (vel != -1)  v_pwm[ch] = vel;
}

/*
   函数作用：电机正反转初始化
   输入值：无（使用全局变量）
   返回值：无
*/
void Dir_Init() {
  for (int i = 0; i < 8; i++) {
    pinMode(M_Dir[i], OUTPUT);
  }
  for (int i = 0; i < 4; i++) {
    digitalWrite(M_Dir[2 * i], v_dir[i]);
    digitalWrite(M_Dir[2 * i + 1], !v_dir[i]);
  }
}

/*
   函数作用：维持输出四路pwm
   输入值：无（使用全局变量）
   返回值：无
*/
void Go_PWM() {
  analogWrite(M_PWM[0], v_pwm[0]);
  analogWrite(M_PWM[1], v_pwm[1]);
  analogWrite(M_PWM[2], v_pwm[2]);
  analogWrite(M_PWM[3], v_pwm[3]);
}
/*
   函数作用：快速前进
   输入值：1-向前走；
   返回值：无
*/
void Attack()
{
  Change_PWM(0, 1, 255);
  Change_PWM(1, 1, 255);
  Change_PWM(2, 1, 255);
  Change_PWM(3, 1, 255);
  Go_PWM();
}
void Stop()
{
  Change_PWM(0, 1, 0);
  Change_PWM(1, 1, 0);
  Change_PWM(2, 1, 0);
  Change_PWM(3, 1, 0);
  Go_PWM();
}

/*
   函数作用：向左转
   返回值：无
*/
void Turn_Left() {
  Change_PWM(0, 0, 255);
  Change_PWM(1, 1, 160);
  Change_PWM(2, 1, 160);
  Change_PWM(3, 1, 160);
  Go_PWM();
}

/*
   函数作用：向右转
   返回值：无
*/
void Turn_Right() {
  Change_PWM(0, 1, 160);
  Change_PWM(1, 1, 160);
  Change_PWM(2, 1, 160);
  Change_PWM(3, 0, 255);
  Go_PWM();
}

/*
   函数作用：向后走
   返回值：无
*/
void Back() {
  Change_PWM(0, 0, 100);
  Change_PWM(1, 0, 100);
  Change_PWM(2, 0, 100);
  Change_PWM(3, 0, 100);
  Go_PWM();
}

void Back1() {
  Change_PWM(0, 0, 180);
  Change_PWM(1, 0, 180);
  Change_PWM(2, 0, 180);
  Change_PWM(3, 0, 180);
  Go_PWM();
}

void Back2() {
  Change_PWM(0, 0, 245);
  Change_PWM(1, 0, 245);
  Change_PWM(2, 0, 245);
  Change_PWM(3, 0, 245);
  Go_PWM();
}
/*
   函数作用：向前走
   返回值：无
*/
void Straight() {
  Change_PWM(0, 1, 80);
  Change_PWM(1, 1, 80);
  Change_PWM(2, 1, 80);
  Change_PWM(3, 1, 80);
  Go_PWM();
}

void Slow() {
  Change_PWM(0, 1, 50);
  Change_PWM(1, 1, 50);
  Change_PWM(2, 1, 50);
  Change_PWM(3, 1, 50);
  Go_PWM();
}

void zhuan1(int b)
{
  int a = servo1.read();
  if (a < b)
    for (int i = a; i < b; i++)
    { servo1.write(i);
      delay(16);
    }
  if (a > b)
    for (int i = a; i > b; i--)
    { servo1.write(i);
      delay(16);
    }
  else return;
}
void zhuan2(int b)
{
  int a = servo2.read();
  if (a < b)
    for (int i = a; i < b; i++)
    { servo2.write(i);
      delay(6);
    }
  if (a > b)
    for (int i = a; i > b; i--)
    { servo2.write(i);
      delay(6);
    }
  else return;
}

void pick1()
{
  zhuan2(8);
  zhuan1(90);
  Stop();
  zhuan1(28);
  delay(100);
}

void pick2()
{
  zhuan2(24);
  zhuan1(90);
  Stop();
  zhuan1(28);
  delay(100);
}

void Throw_Rubbish()
{
  zhuan2(100);
  delay(800);
  zhuan1(115);
  zhuan2(60);
}

void setup() {
  servo1.attach(44);
  servo2.attach(45);
  zhuan1(115);
  zhuan2(60);
  Serial3.begin(9600);   //开启串口通信,调试用
  Serial.begin(9600);
  Dir_Init();
}

void loop() {
  if (Serial3.available())
  {
    r = Serial3.read();
    Serial.println(r);
  }
  switch (r - 1)
  { case 48: Straight(); break;
    case 49: Turn_Left(); break;
    case 50: Back(); break;
    case 51: Turn_Right(); break;
    case 52: Stop(); break;
    case 53: Attack(); break;
    case 54: Turn_Left(); break;
    case 55: zhuan1(60); break;
    case 56: Back2(); break;
    case 57: Slow(); delay(330); pick1(); Throw_Rubbish(); break;
    case 58: Slow(); delay(330); pick2(); Throw_Rubbish(); break;
  }
}
