#include <avr/interrupt.h>
#include "Define.h"

//---------------------------------------------------
extern HardwareSerial Serial;
extern HardwareSerial Serial1;
extern HardwareSerial Serial2;
extern HardwareSerial Serial3;
int speed_run_forward = 180; // tốc độ chạy max 255, min 10
extern volatile unsigned int sensor;
extern volatile int iSensorBefore;
extern volatile int iServoPwm;
unsigned char in, start;
int RC_ANGLE;
extern unsigned int cnt0, cnt1, cnt2;
int handle_center = 81;
unsigned int line;
//unsigned long lastTime, cnt4;
unsigned char pattern = 0;
unsigned char pattern2 = 0;
unsigned char pattern3 = 0;
unsigned char pattern4 = 0;
unsigned char pattern5 = 0;
unsigned char pattern6 = 0;
unsigned char pattern7 = 0;
unsigned char isSerialPrint = 0;
char code;
unsigned char isRunning = 0;
int RF_speed = 0;
unsigned char isLine = 0;
extern unsigned int sensorValue[10];
extern volatile unsigned char mode;
unsigned char pos;
extern volatile int speed_l;
extern volatile int speed_r;
extern volatile unsigned char dir_l;
extern volatile unsigned char dir_r;
extern volatile unsigned char music;
extern volatile unsigned char toGo;
extern volatile unsigned int distance;
extern volatile int realSpeed;
extern volatile int MotorLPulse;
extern volatile int MotorRPulse;
unsigned char toGo2 = 0;
unsigned int wayToGo = 0;
unsigned int step = 0;
extern volatile char isRealSensor;
unsigned char DIREC = 0;
unsigned char NPS = 0;
unsigned char way = 0;
extern volatile unsigned char RFIDpos;
extern volatile unsigned char isPosData;
extern int numberOfLine;
extern int distanceToStop;
extern int speedRun;
extern int distanceToRun;
extern int overDistance;
extern int API;
extern int isAPI1;
extern int isAPI2;
extern int isAPI3;
extern int isAPI4;
extern int isAPI5;
extern int isAPI6;
extern int isAPI7;
unsigned long lastTime = 0;
/*extern unsigned char byteReceived1;
  extern unsigned char byteReceived2;
  extern unsigned char byteReceived3;
  extern unsigned char serialData_Promini[3];
  extern unsigned char index1 = 0;
  extern unsigned char serialData_Microbit[3];
  extern unsigned char index2 = 0;
  extern unsigned char serialData_RFID[3];
  extern unsigned char index3 = 0;*/

void setup()
{
  //---------------PWM------------------------//
  pinMode(LDIR, OUTPUT);
  pinMode(RDIR, OUTPUT);
  pinMode(LED13, OUTPUT);
  pinMode(LED_PWM, OUTPUT);
  pinMode(RBRAKE, OUTPUT);
  pinMode(LBRAKE, OUTPUT);
  digitalWrite(RDIR, LOW);
  digitalWrite(LDIR, LOW);
  digitalWrite(RBRAKE, HIGH);
  digitalWrite(LBRAKE, HIGH);
  digitalWrite(LED_PWM, LOW);
  //---------------Port Sensor------------------------//
  pinMode(3, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  //-------------------------------------------------//
  timer_init();
  exInterrupt_init();
  speed_run(0, 0); // dừng 2 bánh
  pattern = 0;
  pattern6 = 0;
  pattern2 = 0;
  pattern3 = 0;
  pattern4 = 0;
  pattern5 = 0;
  pos = 0;
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(115200);
  // Serial1.write(0xff);
  // Serial1.write(0x00);
  // Serial1.write(0xef);
  wayToGo = 1;
  step = 0;
  isRealSensor = 0;
  wdt_enable(WDTO_500MS);
  readSensorMin();
}

void loop() // vòng lặp, không dùng delay() trong này
{
  wdt_reset();
  //serial2Process();
  //serial3Process();
  /* digitalWrite(LED_PWM, HIGH);
  Serial.println(sensor, BIN);
  Serial.print(sensorValue[0]);
        Serial.print("  ");
        Serial.print(sensorValue[1]);
        Serial.print("  ");
        Serial.print(sensorValue[2]);
        Serial.print("  ");
        Serial.print(sensorValue[3]);
        Serial.print("  ");
        Serial.print(sensorValue[4]);
        Serial.print("  ");
        Serial.print(sensorValue[5]);
        Serial.print("  ");
        Serial.print(sensorValue[6]);
        Serial.print("  ");
        Serial.print(sensorValue[7]);
        Serial.print("  ");
        Serial.print(sensorValue[8]);
        Serial.print("  ");
        Serial.print(sensorValue[9]);
        Serial.println();*/
  serial0Process();
  if (millis() > (lastTime + 1000))
  {
    lastTime = millis();
    String s = "#A" + (String)API + "L" + (String)numberOfLine + "S" + String(distance) + "!";
    Serial.print(s);
  }
  switch (API)
  {
  case 1:
    API_1();
    break;
  case 2:
    API_2();
    break;
  case 3:
    API_3();
    break;
  case 4:
    API_4();
    break;
  case 5:
    API_5();
    break;
  case 6:
    API_6();
    break;
  case 7:
    API_7();
    break;
  default:
    break;
  }
}
//--------------------------------------------------------------------------//
void API_1()
{
  pattern = 0;
  distance = 0;
  while (isAPI1)
  {
    if (millis() > (lastTime + 1000))
    {
      lastTime = millis();
      String s = "#A" + (String)API + "L" + (String)numberOfLine + "S" + String(distance) + "!";
      Serial.print(s);
    }
    serial0Process();
    wdt_reset();
    switch (pattern)
    {
    case 0:
      if (realSpeed < 10)
      {
        pattern = 1;
      }

      else
        pattern = 3;
      //    Serial1.write(0xff);
      //    Serial1.write(0x03);
      //    Serial1.write(0xef);
      digitalWrite(LED_PWM, 1);
      DIREC = 0;
      cnt0 = 0;
      break;
    case 1:
      runforwardline(0);
      if (cnt0 > 200)
      {
        pattern = 2;
        cnt0 = 0;
        RFIDpos = 0;
        isPosData = 1;
        cnt0 = 0;
        line = 0;
      }
      break;
    case 2:
      if ((distanceToRun - distance) > 200)
      {
        if ((cnt0 / 5) < speedRun)
          runforwardline(cnt0 / 5);
        else
          runforwardline(speedRun);
      }
      else
      {
        goSlow(speedRun, 200);
        speed_run(0, 0);
        distanceToRun = 0;
        pattern = 100;
        API = 0;
        isAPI1 = 0;
      }
      break;
    case 3:
      if ((distanceToRun - distance) > 200){
        runforwardline(speedRun);
      }
      else
      {
        goSlow(speedRun, 200);
        speed_run(0, 0);
        distanceToRun = 0;
        pattern = 100;
        API = 0;
        isAPI1 = 0;
        Serial.println("#DONE!");
      }
      break;
    default:
      pattern = 100;
      break;
    }
  }
}
void API_2()
{
  pattern2 = 0;
  distance = 0;
  while (isAPI2 == 1)
  {
    if (millis() > (lastTime + 1000))
    {
      lastTime = millis();
      String s = "#A" + (String)API + "L" + (String)numberOfLine + "S" + String(distance) + "!";
      Serial.print(s);
    }
    wdt_reset();
    switch (pattern2)
    {
    case 0:
      digitalWrite(LED_PWM, 1);
      runforwardline(0);
      if (realSpeed == 0)
      {
        pattern2 = 1;
        //   DIREC = 0;
        cnt0 = 0;
      }
      break;
    case 1:
      turnLeft(50);
      //  turnRight(speedRun);
      pattern2 = 100;
      runforwardline(0);
      speed_run(0, 0);
      isAPI2 = 0;
      API = 0;
      Serial.println("#DONE!");
      break;
    default:
      pattern2 = 100;
      break;
    }
  }
}
void API_3()
{
  pattern3 = 0;
  distance = 0;
  while (isAPI3 == 1)
  {
    if (millis() > (lastTime + 1000))
    {
      lastTime = millis();
      String s = "#A" + (String)API + "L" + (String)numberOfLine + "S" + String(distance) + "!";
      Serial.print(s);
    }
    wdt_reset();
    switch (pattern3)
    {
    case 0:
      //   Serial1.write(0xff);
      //   Serial1.write(0x03);
      //   Serial1.write(0xef);
      digitalWrite(LED_PWM, 1);
      runforwardline(0);
      if (realSpeed == 0)
      {
        pattern3 = 1;
        DIREC = 0;
        cnt0 = 0;
      }
      break;
    case 1:
      turnRight(50);
      pattern3 = 100;
      runforwardline(0);
      isAPI3 = 0;
      API = 0;
      Serial.println("#DONE!");
      break;
    default:
      pattern3 = 100;
      break;
    }
  }
}
void API_4()
{
  pattern4 = 0;
  distance = 0;
  while (isAPI4 == 1)
  {
    if (millis() > (lastTime + 1000))
    {
      lastTime = millis();
      String s = "#A" + (String)API + "L" + (String)numberOfLine + "S" + String(distance) + "!";
      Serial.print(s);
    }
    wdt_reset();
    switch (pattern4)
    {
    case 0:
      //   Serial1.write(0xff);
      //   Serial1.write(0x03);
      //   Serial1.write(0xef);
      digitalWrite(LED_PWM, 1);
      runforwardline(0);
      if (realSpeed == 0)
      {
        pattern4 = 1;
        //  DIREC = 0;
        cnt0 = 0;
      }
      break;
    case 1:
      turn180(50);
      pattern4 = 100;
      runforwardline(0);
      isAPI4 = 0;
      API = 0;
      Serial.println("#DONE!");
      break;
    default:
      pattern4 = 100;
      break;
    }
  }
}
void API_5()
{
  pattern5 = 0;
  distance = 0;
  while (isAPI5 == 1)
  {
    if (millis() > (lastTime + 1000))
    {
      lastTime = millis();
      String s = "#A" + (String)API + "L" + (String)numberOfLine + "S" + String(distance) + "!";
      Serial.print(s);
    }
    wdt_reset();
    switch (pattern5)
    {
    case 0:
      //   Serial1.write(0xff);
      //   Serial1.write(0x03);
      //   Serial1.write(0xef);
      digitalWrite(LED_PWM, 1);

      if (realSpeed == 0)
      {
        runforwardline(0);
        isAPI5 = 0;
        API = 0;
        pattern5 = 100;
      }
      else
      {
        pattern5 = 1;
        // DIREC = 0;
        cnt0 = 0;
      }
      break;
    case 1:
      goSlow(speedRun, distanceToStop);
      isAPI5 = 0;
      API = 0;
      pattern5 = 100;
      Serial.println("#DONE!");
      break;
    default:
      pattern5 = 100;
      break;
    }
  }
}
void API_6()
{
  pattern6 = 0;
  distance = 0;
  while (isAPI6)
  {
    if (millis() > (lastTime + 1000))
    {
      lastTime = millis();
      String s = "#A" + (String)API + "L" + (String)numberOfLine + "S" + String(distance) + "!";
      Serial.print(s);
    }
    serial0Process();
    wdt_reset();
    switch (pattern6)
    {
    case 0:
      if (realSpeed < 10)
      {
        pattern6 = 1;
      }

      else
        pattern6 = 3;
      //    Serial1.write(0xff);
      //    Serial1.write(0x03);
      //    Serial1.write(0xef);
      digitalWrite(LED_PWM, 1);
      //  DIREC = 0;
      cnt0 = 0;
      break;
    case 1:
      runBackwardLine(0);
      if (cnt0 > 200)
      {
        pattern6 = 2;
        cnt0 = 0;
        RFIDpos = 0;
        isPosData = 1;
        cnt0 = 0;
      }
      break;
    case 2:
      if ((distanceToRun - distance) > 50)
      {
        if ((cnt0 / 5) < speedRun)
          runBackwardLine(-(cnt0 / 5));
        else
          runBackwardLine(-speedRun);
      }
      else
      {
        goBackSlow(-speedRun, 200);
        speed_run(0, 0);
        distanceToRun = 0;
        pattern6 = 100;
        API = 0;
        isAPI6 = 0;
      }
      break;
    case 3:
      if ((distanceToRun - distance) > 50)
        runBackwardLine(-speedRun);
      else
      {
        goBackSlow(-speedRun, 200);
        speed_run(0, 0);
        pattern6 = 100;
        API = 0;
        isAPI6 = 0;
        Serial.println("#DONE!");
      }
      break;
    default:
      pattern6 = 100;
      break;
    }
  }
}
void API_7()
{
  pattern7 = 0;
  distance = 0;
  while (isAPI7 == 1)
  {
    if (millis() > (lastTime + 1000))
    {
      lastTime = millis();
      String s = "#A" + (String)API + "L" + (String)numberOfLine + "S" + String(distance) + "!";
      Serial.print(s);
    }
    if(distance >= overDistance){
      pattern7 = 100;
      API = 0;
      isAPI7 = 0;
      runforwardline(0);
      Serial.println("#DONE!");
      return;
    }
    serial0Process();
    wdt_reset();
    switch (pattern7)
    {
    case 0:
      //   Serial1.write(0xff);
      //   Serial1.write(0x03);
      //   Serial1.write(0xef);
      digitalWrite(LED_PWM, 1);
      speed_run(0, 0);
      pattern7 = 1;
      DIREC = 0;
      cnt0 = 0;
      break;
    case 1:
      runforwardline(0);
      if (cnt0 > 200)
      {
        pattern7 = 2;
        cnt0 = 0;
        RFIDpos = 0;
        isPosData = 1;
        cnt0 = 0;
        line = 0;
      }
      break;
    case 2:
      if ((sensor == 0xff) && (isLine == 0))
      {
        numberOfLine--;
        isLine = 1;
      }
      else if (sensor_inp(0b11000011) == 0x00)
        isLine = 0;
      if (numberOfLine == 0)
      {
        RFIDpos = 2;
        isPosData = 1;
        pattern7 = 3;
        cnt0 = 0;
        line = 0;
      }
      if ((cnt0 / 5) < speedRun)
        runforwardline(cnt0 / 5);
      else
        runforwardline(speedRun);
      break;
    case 3:
      //  if (cnt0 > 100)
      // {
      //distance = 0;
      goSlow(speedRun, 310);
      //   DIREC = 1;
      pattern7 = 100;
      API = 0;
      isAPI7 = 0;
      runforwardline(0);
      Serial.println("#DONE!");
      //  }
      //  runforwardline(speed_run_forward);
      break;
    default:
      break;
    }
  }
}