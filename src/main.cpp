#include <Arduino.h>
#include <Servo.h>
#include <SoftwareSerial.h>

//初始化蓝牙模块
SoftwareSerial MyBlue(16, 17); // RX | TX

//初始化舵机模块
Servo servo1;
Servo servo2;
int r;
int M_PWM[4] = {9, 10, 11, 12}; //电机PWM，依次对应 LF,LB,RB,RF  L-Left R-Right F-Front B-Back
int M_Dir[8] = {22, 23, 24, 25, 26, 27, 29, 28}; //控制电机正反转，依次对应 LF,LB,RB,RF,(一个电机需要两个，对应IN1,IN2)

// PWM的全局变量
int v_pwm[4] = {120, 120, 120, 120};
int v_dir[4] = {1, 1, 1, 1};

//音乐
void hei_ren_tai_guan();
#define N 250000  //tone时间
#define pin 18
#define m 250  //delay时间
int f[3][7] = {{392, 440, 494}, {523, 587, 659, 698, 784, 880, 988}, {1047}}; //频率，低（音符）5,6,7，中1,2,3,4,5,6,7，高1
int i;


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
void Attack2()
{
  Change_PWM(0, 1, 120);
  Change_PWM(1, 1, 120);
  Change_PWM(2, 1, 255);
  Change_PWM(3, 1, 255);
  Go_PWM();
}

void Attack3()
{
  Change_PWM(0, 1, 255);
  Change_PWM(1, 1, 255);
  Change_PWM(2, 1, 120);
  Change_PWM(3, 1, 120);
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
  Change_PWM(0, 1, 86);
  Change_PWM(1, 1, 86);
  Change_PWM(2, 1, 86);
  Change_PWM(3, 1, 86);
  Go_PWM();
}



void hei_ren_tai_guan()
{
  tone(pin, f[1][1], N); //2
  delay(m);
  tone(pin, f[1][0], N); //1
  delay(m);
  tone(pin, f[0][2], N); //7
  delay(m);
  tone(pin, f[0][0], N); //5
  delay(m);


  tone(pin, f[0][1], N); //6
  delay(m);
  tone(pin, 0, N);  //
  delay(m);
  tone(pin, f[0][1], N); //6
  delay(m);
  tone(pin, f[1][2], N); //3
  delay(m);
  tone(pin, f[1][1], N); delay(m); //2
  tone(pin, 0, N);  delay(m); //
  tone(pin, f[1][0], N);  delay(m); //1
  tone(pin, 0, N);  delay(m); //
  tone(pin, f[0][2], N); delay(m); //7
  tone(pin, 0, N);   delay(m); //
  tone(pin, f[0][2], N);  delay(m); //7
  tone(pin, f[0][2], N);  delay(m); //7
  tone(pin, f[1][1], N);  delay(m); //2
  tone(pin, 0, N);  delay(m); //
  tone(pin, f[1][0], N);  delay(m); //1
  tone(pin, f[0][2], N);  delay(m); //7

  tone(pin, f[0][1], N);  delay(m); //6
  tone(pin, 0, N);   delay(m);
  tone(pin, f[0][1], N);  delay(m); //6
  tone(pin, f[2][0], N); delay(m); //1
  tone(pin, f[1][6], N);  delay(m); //7
  tone(pin, f[2][0], N);  delay(m); //1
  tone(pin, f[1][6], N);  delay(m); //7
  tone(pin, f[2][0], N);  delay(m); //1
  tone(pin, f[0][1], N);  delay(m); //6
  tone(pin, 0, N);  delay(m);
  tone(pin, f[0][1], N);  delay(m); //6
  tone(pin, f[2][0], N);  delay(m); //1
  tone(pin, f[1][6], N);  delay(m); //7
  tone(pin, f[2][0], N);  delay(m); //1
  tone(pin, f[1][6], N);  delay(m); //7
  tone(pin, f[2][0], N);  delay(m); //1
  //重复3次
  noTone(pin);
  for (i = 0; i < 4; i++)
  {
    tone(pin, f[1][0], N);  //1
    delay(m);
  }
  for (i = 0; i < 4; i++)
  {
    tone(pin, f[1][2], N);  //3
    delay(m);
  }
  for (i = 0; i < 4; i++)
  {
    tone(pin, f[1][1], N);  //2
    delay(m);
  }
  for (i = 0; i < 4; i++)
  {
    tone(pin, f[1][4], N);  //5
    delay(m);
  }
  for (i = 0; i < 12; i++)
  {
    tone(pin, f[1][5], N);  //6
    delay(m);
  }
  tone(pin, f[1][1], N); delay(m); //2
  tone(pin, f[1][0], N);  delay(m); //1
  tone(pin, f[0][2], N);  delay(m); //7
  tone(pin, f[0][0], N);  delay(m); //5
  for (i = 0; i < 2; i++)
  {
    tone(pin, f[0][1], N); delay(m); //6
    tone(pin, 0, N); delay(m);
    tone(pin, f[0][1], N);  delay(m); //6
    tone(pin, f[1][2], N);  delay(m); //3
    tone(pin, f[1][1], N);  delay(m); //2
    tone(pin, 0, N);  delay(m);
    tone(pin, f[1][0], N); delay(m); //1
    tone(pin, 0, N);  delay(m);
    tone(pin, f[0][2], N);  delay(m); //7
    tone(pin, 0, N);  delay(m);
    tone(pin, f[0][2], N); delay(m); //7
    tone(pin, f[0][2], N);  delay(m); //7
    tone(pin, f[1][1], N); delay(m); //2
    tone(pin, 0, N); delay(m);
    tone(pin, f[1][0], N);  delay(m); //1
    tone(pin, f[0][2], N);  delay(m); //7
    tone(pin, f[0][1], N);  delay(m); //6
    tone(pin, 0, N); delay(m);
    tone(pin, f[0][1], N);  delay(m); //6
    tone(pin, f[2][0], N);  delay(m); //1
    tone(pin, f[1][6], N);  delay(m); //7
    tone(pin, f[2][0], N);  delay(m); //1
    tone(pin, f[1][6], N);  delay(m); //7
    tone(pin, f[2][0], N);  delay(m); //1
    tone(pin, f[0][1], N);  delay(m); //6
    tone(pin, 0, N); delay(m);
    tone(pin, f[0][1], N);  delay(m); //6
    tone(pin, f[2][0], N);  delay(m); //7
    tone(pin, f[2][0], N);  delay(m); //1
    tone(pin, f[2][0], N);  delay(m); //7
    tone(pin, f[2][0], N);  delay(m); //1
    for (i = 0; i < 4; i++)
    {
      tone(pin, f[1][0], N);  //1
      delay(m);
    }
    for (i = 0; i < 4; i++)
    {
      tone(pin, f[1][2], N);  //3
      delay(m);
    }
    for (i = 0; i < 4; i++)
    {
      tone(pin, f[1][1], N);  //2
      delay(m);
    }
    for (i = 0; i < 4; i++)
    {
      tone(pin, f[1][4], N);  //5
      delay(m);
    }
    for (i = 0; i < 12; i++)
    {
      tone(pin, f[1][5], N);
      delay(m);
    }
  }
  delay(1000);
}

void pick()
{
  servo1.write(5);
  delay(2000);
  servo1.write(120);
  delay(2000);
}

void Throw_Rubbish()
{
  servo1.write(100);
  delay(15);
  servo1.write(100);
  delay(15);
}

void setup() {
  servo1.attach(44);
  servo1.write(120);
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
    case 55: hei_ren_tai_guan(); break;
    case 56: Back2(); break;
    case 58: pick(); break;
    case 59: Throw_Rubbish(); break;
  }
}