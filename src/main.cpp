#include <Arduino.h>
#include <Servo.h>
//蓝牙模块定义
#include <SoftwareSerial.h>
Servo servo1;
Servo servo2;
SoftwareSerial MyBlue(16, 17); // RX | TX
//蓝牙接收值
int r;

//电机PWM，依次对应 LF,LB,RB,RF  L-Left R-Right F-Front B-Back
int M_PWM[4] = {9, 10, 11, 12};
//控制电机正反转，依次对应 LF,LB,RB,RF,(一个电机需要两个，对应IN1,IN2)
int M_Dir[8] = {22, 23, 24, 25, 26, 27, 28, 29};

//PWM的全局变量
int v_pwm[4] = {120, 120, 120, 120};
//正反转的全局变量
int v_dir[4] = {1, 1, 1, 1};
void pick()
{
  servo1.write(100);
  delay(15);
  servo2.write(100);
  delay(15);
}
void Throw_Rubbish()
{
  servo1.write(100);
  delay(15);
  servo2.write(100);
  delay(15);
}

/*
 * 函数作用：控制一路电机的速度和正反转
 * 输入值：ch-电机号（0,1,2,3），dir-正反转（1,0），vel-速度（0~255）
 * 返回值：无
 */
void Change_PWM(int ch, int dir, int vel = -1)
{
  digitalWrite(M_Dir[2 * ch], dir);
  digitalWrite(M_Dir[2 * ch + 1], !dir);
  if (vel != -1)
    v_pwm[ch] = vel;
}

/*
 * 函数作用：电机正反转初始化
 * 输入值：无（使用全局变量）
 * 返回值：无
 */
void Dir_Init()
{
  for (int i = 0; i < 8; i++)
  {
    pinMode(M_Dir[i], OUTPUT);
  }
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(M_Dir[2 * i], v_dir[i]);
    digitalWrite(M_Dir[2 * i + 1], !v_dir[i]);
  }
}

/*
 * 函数作用：维持输出四路pwm
 * 输入值：无（使用全局变量）
 * 返回值：无
 */
void Go_PWM()
{
  analogWrite(M_PWM[0], v_pwm[0]);
  analogWrite(M_PWM[1], v_pwm[1]);
  analogWrite(M_PWM[2], v_pwm[2]);
  analogWrite(M_PWM[3], v_pwm[3]);
}

/*
 * 函数作用：向前走
 * 返回值：无
 */
void Straight()
{
  Change_PWM(0, 1, 200);
  Change_PWM(1, 1, 200);
  Change_PWM(2, 1, 200);
  Change_PWM(3, 1, 200);
  Go_PWM();
}

/*
 * 函数作用：缓慢向前走
 * 返回值：无
 */
void Slow_Straight()
{
  Change_PWM(0, 1, 100);
  Change_PWM(1, 1, 100);
  Change_PWM(2, 1, 100);
  Change_PWM(3, 1, 100);
  Go_PWM();
}

/*
 * 函数作用：向后倒
 * 返回值：无
 */
void Back()
{
  Change_PWM(0, 0, 200);
  Change_PWM(1, 0, 200);
  Change_PWM(2, 0, 200);
  Change_PWM(3, 0, 200);
  Go_PWM();
}

/*
 * 函数作用：缓慢向后倒
 * 返回值：无
 */
void Slow_Back()
{
  Change_PWM(0, 0, 100);
  Change_PWM(1, 0, 100);
  Change_PWM(2, 0, 100);
  Change_PWM(3, 0, 100);
  Go_PWM();
}

/*
 * 函数作用：停车
 * 返回值：无
 */
void Stop()
{
  Change_PWM(0, 1, 0);
  Change_PWM(1, 1, 0);
  Change_PWM(2, 1, 0);
  Change_PWM(3, 1, 0);
  Go_PWM();
}

/*
 * 函数作用：向左转
 * 返回值：无
 */
void Turn_Left()
{
  Change_PWM(0, 0, 255);
  Change_PWM(1, 1, 160);
  Change_PWM(2, 1, 160);
  Change_PWM(3, 1, 160);
  Go_PWM();
}

/*
 * 函数作用：向右转
 * 返回值：无
 */
void Turn_Right()
{
  Change_PWM(0, 1, 160);
  Change_PWM(1, 1, 160);
  Change_PWM(2, 1, 160);
  Change_PWM(3, 0, 255);
  Go_PWM();
}

//单遍程序
void setup()
{
  servo1.attach(44);
  servo2.attach(45);
  servo1.write(100);
  servo2.write(100);
  Serial3.begin(9600); //开启串口通信,调试用
  Serial.begin(9600);
  Dir_Init();
}

//循环程序
void loop()
{
  if (Serial3.available())
  {
    r = Serial3.read();
    Serial.println(r);
  }
  switch (r - 1)
  {
  case 48:
    Straight();
    break;
  case 49:
    Slow_Straight();
    break;
  case 50:
    Turn_Left();
    break;
  case 51:
    Back();
    break;
  case 52:
    Slow_Back();
    break;
  case 53:
    Turn_Right();
    break;
  case 54:
    Stop();
    break;
  case 55:
    Turn_Left();
    break;
  case 56:
    pick();
    break;
  case 57:
    Throw_Rubbish();
    break;
  }
}