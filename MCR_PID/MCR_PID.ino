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
unsigned long lastTime, cnt4;
unsigned char pattern = 0;
unsigned char pattern2 = 0;
unsigned char pattern3 = 0;
unsigned char pattern4 = 0;
unsigned char pattern5 = 0;
unsigned char pattern6 = 0;
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
  Serial.begin(115200);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(115200);
  Serial1.write(0xff);
  Serial1.write(0x00);
  Serial1.write(0xef);
  wayToGo = 1;
  step = 0;
  isRealSensor = 0;
  wdt_enable(WDTO_500MS);
}

void loop() // vòng lặp, không dùng delay() trong này
{
  wdt_reset();
  serial2Process();
  serial3Process();
  speed_run_forward = 80;
  switch (mode)
  {
  case 0:
    step = 0;
    line = 0;
    isSerialPrint = 0;
    pos = 0;
    pattern = 0;
    pattern6 = 0;
    pattern2 = 0;
    pattern3 = 0;
    pattern4 = 0;
    pattern5 = 0;
    isRealSensor = 0;
    // readSensorMin();
    brake(0);
    speed_run(speed_l, speed_r);
    break;
  case 1:
    // Serial.println(pattern);
    if (isRunning == 0)
    {
      toGo2 = toGo;
    }

    if (pos == 0)
    {
      switch (toGo2)
      {
      case 0:
        isRunning = 0;
        digitalWrite(LED_PWM, LOW);
        step = 0;
        line = 0;
        isSerialPrint = 0;
        pattern = 0;
        pattern6 = 0;
        pattern2 = 0;
        pattern3 = 0;
        pattern4 = 0;
        pattern5 = 0;
        isRealSensor = 0;
        readSensorMin();
        speed_run(0, 0);
        brake(1);
        break;
      case 7:
        isRunning = 1;
        brake(0);
        way = 1;
        way1();
        break;
      case 9:
        isRunning = 1;
        brake(0);
        way2();
        way = 2;
        break;
      default:
        break;
      }
    }
    if (pos == 7)
    {
      switch (toGo2)
      {
      case 0:
        isRunning = 1;
        brake(0);
        way3();
        way = 3;
        break;
      case 7:
        isRunning = 0;
        digitalWrite(LED_PWM, LOW);
        step = 0;
        line = 0;
        isSerialPrint = 0;
        pattern = 0;
        pattern6 = 0;
        pattern2 = 0;
        pattern3 = 0;
        pattern4 = 0;
        pattern5 = 0;
        isRealSensor = 0;
        readSensorMin();
        speed_run(0, 0);
        brake(1);
        break;
      case 9:
        isRunning = 1;
        brake(0);
        way5();
        way = 5;
        break;
      default:
        break;
      }
    }
    if (pos == 9)
    {
      switch (toGo2)
      {
      case 0:
        isRunning = 1;
        brake(0);
        way4();
        way = 4;
        break;
      case 7:
        isRunning = 1;
        brake(0);
        way6();
        way = 6;
        break;
      case 9:
        isRunning = 0;
        digitalWrite(LED_PWM, LOW);
        step = 0;
        line = 0;
        isSerialPrint = 0;
        pattern = 0;
        pattern6 = 0;
        pattern2 = 0;
        pattern3 = 0;
        pattern4 = 0;
        pattern5 = 0;
        isRealSensor = 0;
        readSensorMin();
        speed_run(0, 0);
        brake(1);
        break;
      default:
        break;
      }
    }
    break;

  case 2:
    break;
  case 3:

    pattern = 0;
    pattern6 = 0;
    pattern2 = 0;
    pattern3 = 0;
    pattern4 = 0;
    pattern5 = 0;
    // isRealSensor = 1;
    digitalWrite(LED_PWM, HIGH);
    // runBackwardLine(-speed_run_forward);
    //   Serial.println(sensor, BIN);
    /*Serial.print(sensorValue[0]);
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
    // Serial.println(analogRead(CHARGE_SENSOR));
    Serial.println(realSpeed);
    brake(0);
    break;
  case 100:
    readSensorMin();
    digitalWrite(LED_PWM, 0);
    // if((cnt0%100) == 0) Serial2.println(1);
    Serial.println(distance);
    brake(0);
    break;
  default:
    // Serial2.println(1);
    readSensorMin();
    digitalWrite(LED_PWM, LOW);
    brake(1);
    speed_run(0, 0);
    break;
  }
}
//--------------------------------------------------------------------------//
void way1()
{
  switch (pattern)
  {
  case 0:
    Serial1.write(0xff);
    Serial1.write(0x03);
    Serial1.write(0xef);
    digitalWrite(LED_PWM, 1);
    speed_run(0, 0);
    pattern = 1;
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
    if (((sensor == 0xff) && (isLine == 0)) || (RFIDpos == 2))
    {
      line++;
      isLine = 1;
    }
    else if (sensor_inp(0b11000011) == 0x00)
      isLine = 0;
    if (line == 1)
    {
      RFIDpos = 2;
      isPosData = 1;
      pattern = 3;
      cnt0 = 0;
      line = 0;
    }
    if ((cnt0 / 5) < speed_run_forward)
      runforwardline(cnt0 / 5);
    else
      runforwardline(speed_run_forward);
    break;
  case 3:
    //  if (cnt0 > 100)
    // {
    goSlow(speed_run_forward, 310);
    DIREC = 1;
    turnLeft(50);
    pattern = 4;
    cnt0 = 0;
    //  }
    //  runforwardline(speed_run_forward);
    break;

  case 4:
    if (((sensor == 0xff) && (isLine == 0)) || (RFIDpos == 1))
    {
      line++;
      isLine = 1;
    }
    else if (sensor_inp(0b11000011) == 0x00)
      isLine = 0;
    if (line == 1)
    {
      // Serial2.println(2);
      RFIDpos = 1;
      isPosData = 1;
      pattern = 5;
      cnt0 = 0;
      line = 0;
    }
    if ((cnt0 / 5) < speed_run_forward)
      runforwardline(cnt0 / 5);
    else
      runforwardline(speed_run_forward);
    break;
  case 5:
    goSlow(speed_run_forward, 310);
    DIREC = 0;
    turnRight(50);
    pattern = 6;
    cnt0 = 0;
    break;
  case 6:
    if (((sensor == 0xff) && (isLine == 0)) || (RFIDpos == 4))
    {
      line++;
      isLine = 1;
    }
    else if (sensor_inp(0b11000011) == 0x00)
      isLine = 0;
    if (line == 1)
    {
      // Serial2.println(2);
      RFIDpos = 4;
      isPosData = 1;
      pattern = 70;
      cnt0 = 0;
      line = 0;
    }
    if ((cnt0 / 5) < speed_run_forward)
      runforwardline(cnt0 / 5);
    else
      runforwardline(speed_run_forward);
    break;
  case 70:
    if (cnt0 > 100)
    {
      pattern = 7;
      cnt0 = 0;
    }
    runforwardline(speed_run_forward);
    break;
  case 7:
    if (((sensor == 0xff) && (isLine == 0)) || (RFIDpos == 7))
    {
      line++;
      isLine = 1;
    }
    else if (sensor_inp(0b11000011) == 0x00)
      isLine = 0;
    if (line == 1)
    {
      // Serial2.println(7);
      Serial1.write(0xff);
      Serial1.write(0x02);
      Serial1.write(0xef);
      pattern = 8;
      cnt0 = 0;
      line = 0;
    }
    runforwardline(speed_run_forward);
    break;
  case 8:
    goSlow(speed_run_forward, 330);
    pattern = 100;
    cnt0 = 0;
    break;
  case 100:
    pos = 7;
    RFIDpos = 7;
    isRunning = 0;
    realSpeed = 0;
    isPosData = 1;
    runforwardline(0);
    break;
  default:
    pattern = 100;
    break;
  }
}
void way2()
{
  switch (pattern2)
  {
  case 0:
    Serial1.write(0xff);
    Serial1.write(0x03);
    Serial1.write(0xef);
    digitalWrite(LED_PWM, 1);
    speed_run(0, 0);
    pattern2 = 1;
    DIREC = 0;
    cnt0 = 0;
    break;
  case 1:
    runforwardline(0);
    if (cnt0 > 200)
    {
      pattern2 = 2;
      cnt0 = 0;
      RFIDpos = 0;
      isPosData = 1;
      cnt0 = 0;
      line = 0;
    }
    break;
  case 2:
    if (((sensor == 0xff) && (isLine == 0))||(RFIDpos == 2))
    {
      line++;
      isLine = 1;
    }
    else if (sensor_inp(0b11000011) == 0x00)
      isLine = 0;
    if (line == 1)
    {
      RFIDpos = 2;
      isPosData = 1;
      pattern2 = 3;
      cnt0 = 0;
      line = 0;
    }
    if ((cnt0 / 5) < speed_run_forward)
      runforwardline(cnt0 / 5);
    else
      runforwardline(speed_run_forward);
    break;
  case 3:
    //  if (cnt0 > 100)
    // {
    goSlow(speed_run_forward, 310);
    DIREC = 2;
    turnRight(50);
    //Serial2.println("!UPT;1;CND:" + (String)RFIDpos + (String)NPS + (String)DIREC + "#");
    pattern2 = 4;
    cnt0 = 0;
    //  }
    //  runforwardline(speed_run_forward);
    break;
  case 4:
    if (((sensor == 0xff) && (isLine == 0))||(RFIDpos == 3))
    {
      line++;
      isLine = 1;
    }
    else if (sensor_inp(0b11000011) == 0x00)
      isLine = 0;
    if (line == 1)
    {
      // Serial2.println(2);
      RFIDpos = 3;
      isPosData = 1;
      pattern2 = 5;
      cnt0 = 0;
      line = 0;
    }
    if ((cnt0 / 5) < speed_run_forward)
      runforwardline(cnt0 / 5);
    else
      runforwardline(speed_run_forward);
    break;
  case 5:
    goSlow(speed_run_forward, 310);
    DIREC = 0;
    turnLeft(50);
    //Serial2.println("!UPT;1;CND:" + (String)RFIDpos + (String)NPS + (String)DIREC + "#");
    pattern2 = 6;
    cnt0 = 0;
    break;
  case 6:
    if (((sensor == 0xff) && (isLine == 0))||(RFIDpos == 6))
    {
      line++;
      isLine = 1;
    }
    else if (sensor_inp(0b11000011) == 0x00)
      isLine = 0;
    if (line == 1)
    {
      // Serial2.println(2);
      RFIDpos = 6;
      isPosData = 1;
      pattern2 = 70;
      cnt0 = 0;
      line = 0;
    }
    if ((cnt0 / 5) < speed_run_forward)
      runforwardline(cnt0 / 5);
    else
      runforwardline(speed_run_forward);
    break;
  case 70:
    if (cnt0 > 100)
    {
      pattern2 = 7;
      cnt0 = 0;
    }
    runforwardline(speed_run_forward);
    break;
  case 7:
    if (((sensor == 0xff) && (isLine == 0))||(RFIDpos == 9))
    {
      line++;
      isLine = 1;
    }
    else if (sensor_inp(0b11000011) == 0x00)
      isLine = 0;
    if (line == 1)
    {
      // Serial2.println(7);
      Serial1.write(0xff);
      Serial1.write(0x02);
      Serial1.write(0xef);
      pattern2 = 8;
      cnt0 = 0;
      line = 0;
    }
    runforwardline(speed_run_forward);
    break;
  case 8:
    goSlow(speed_run_forward, 330);
    pattern2 = 100;
    cnt0 = 0;
    break;
  case 100:
    pos = 9;
    RFIDpos = 9;
    isRunning = 0;
    realSpeed = 0;
    isPosData = 1;
    runforwardline(0);
    break;
  default:
    pattern2 = 100;
    break;
  }
}
void way3()
{
  switch (pattern3)
  {
  case 0:
    Serial1.write(0xff);
    Serial1.write(0x03);
    Serial1.write(0xef);
    cnt0 = 0;
    line = 0;
    step = 0;
    pattern3 = 1;
    digitalWrite(LED_PWM, HIGH);
    break;
  case 1:
    if ((cnt0 / 5) < 50)
    {
      speed_run(-(cnt0 / 5), -(cnt0 / 5));
    }
    else
    {
      speed_run(-50, -50);
      pattern3 = 2;
      cnt0 = 0;
      digitalWrite(LED_PWM, HIGH);
    }
    break;
  case 2:
    speed_run(-(50 - (cnt0 / 2)), -(50 - (cnt0 / 2)));
    if ((cnt0 / 2) >= 50)
    {
      //Serial2.println(7);
      RFIDpos = 7;
      isPosData = 1;
      cnt0 = 0;
      pattern3 = 3;
    }
    break;
  case 3:
    DIREC = 3;
    turn180(50);
    pattern3 = 4;
    cnt0 = 0;
    line = 0;
    break;
  case 4:
    if (((sensor == 0xff) && (isLine == 0))||(RFIDpos == 4))
    {
      line++;
      isLine = 1;
    }
    else if (sensor_inp(0b11000011) == 0x00)
      isLine = 0;
    if (line == 1)
    {
      //  Serial2.println(4);
      RFIDpos = 4;
      isPosData = 1;
      pattern3 = 5;
      cnt0 = 0;
      line = 0;
    }
    if ((cnt0 / 5) < speed_run_forward)
      runforwardline(cnt0 / 5);
    else
      runforwardline(speed_run_forward);
    break;
  case 5:
    goSlow(speed_run_forward, 310);
    DIREC = 2;
    turnLeft(50);
    pattern3 = 6;
    cnt0 = 0;
    break;
  case 6:
    if (((sensor == 0xff) && (isLine == 0))||(RFIDpos == 5))
    {
      line++;
      isLine = 1;
    }
    else if (sensor_inp(0b11000011) == 0x00)
      isLine = 0;
    if (line == 1)
    {
      //Serial2.println(5);
      RFIDpos = 5;
      isPosData = 1;
      pattern3 = 7;
      cnt0 = 0;
      line = 0;
    }
    if ((cnt0 / 5) < speed_run_forward)
      runforwardline(cnt0 / 5);
    else
      runforwardline(speed_run_forward);
    break;
  case 7:
    goSlow(speed_run_forward, 310);
    DIREC = 3;
    turnRight(50);

    // Serial2.println("!UPT;1;CND:" + (String)RFIDpos + (String)NPS + (String)DIREC + "#");
    pattern3 = 8;
    cnt0 = 0;
    break;
  case 8:
    if (((sensor == 0xff) && (isLine == 0))||(RFIDpos == 2))
    {
      line++;
      isLine = 1;
    }
    else if (sensor_inp(0b11000011) == 0x00)
      isLine = 0;
    if (line == 1)
    {
      // Serial2.println(2);
      RFIDpos = 2;
      isPosData = 1;
      pattern3 = 90;
      distance = 0;
      cnt0 = 0;
      line = 0;
    }
    if ((cnt0 / 5) < speed_run_forward)
      runforwardline(cnt0 / 5);
    else
      runforwardline(speed_run_forward);
    break;
  case 90:
    if (distance > 700)
    {
      DIREC = 3;
      RFIDpos = 0;
      isPosData = 1;
      pattern3 = 9;
      distance = 0;
      cnt0 = 0;
      line = 0;
      break;
    }
    else
      runforwardline(speed_run_forward);
    break;
  case 9:
    if ((distance / 4) < (speed_run_forward - 10))
    {
      runforwardline(speed_run_forward - (distance / 4));
    }
    else
    {
      runforwardline(0);
      DIREC = 0;
      turn180(43);
      //   Serial2.println("!UPT;1;CND:" + (String)RFIDpos + (String)NPS + (String)DIREC + "#");
      pattern3 = 10;
      cnt0 = 0;
      distance = 0;
    }
    break;
  case 10:
    isRealSensor = 1;
    if ((cnt0 / 20) < 10)
      runBackwardLine(-(cnt0 / 20));
    else
      runBackwardLine(-10);
    if (distance > 75)
    {
      cnt0 = 0;
      pattern3 = 11;
    }
    break;
  case 11:
    runBackwardLine(-(10 - (cnt0 / 40)));
    if (cnt0 >= 400)
    {
      cnt0 = 0;
      pattern3 = 13;
    }
    if (distance > 93)
    {
      cnt0 = 0;
      pattern3 = 13;
    }
    break;
  case 13:
    Serial1.write(0xff);
    Serial1.write(0x04);
    Serial1.write(0xef);
    DIREC = 0;
    //  Serial2.println("!UPT;1;CND:" + (String)RFIDpos + (String)NPS + (String)DIREC + "#");
    cnt0 = 0;
    pattern3 = 100;
    break;
  case 100:
    isRealSensor = 0;
    pos = 0;
    isRunning = 0;
    RFIDpos = 0;
    isPosData = 1;
    runforwardline(0);
    break;
  default:
    pattern3 = 100;
    break;
  }
}
void way4()
{
  switch (pattern4)
  {
  case 0:
    Serial1.write(0xff);
    Serial1.write(0x03);
    Serial1.write(0xef);
    cnt0 = 0;
    line = 0;
    step = 0;
    pattern4 = 1;
    digitalWrite(LED_PWM, HIGH);
    break;
  case 1:
    if ((cnt0 / 5) < 50)
    {
      speed_run(-(cnt0 / 5), -(cnt0 / 5));
    }
    else
    {
      speed_run(-50, -50);
      pattern4 = 2;
      cnt0 = 0;
      digitalWrite(LED_PWM, HIGH);
    }
    break;
  case 2:
    speed_run(-(50 - (cnt0 / 2)), -(50 - (cnt0 / 2)));
    if ((cnt0 / 2) >= 50)
    {
      //Serial2.println(7);
      RFIDpos = 9;
      isPosData = 1;
      cnt0 = 0;
      pattern4 = 3;
    }
    break;
  case 3:
    DIREC = 3;
    turn180(50);
    pattern4 = 4;
    cnt0 = 0;
    line = 0;
    break;
  case 4:
    if (((sensor == 0xff) && (isLine == 0))||(RFIDpos == 6))
    {
      line++;
      isLine = 1;
    }
    else if (sensor_inp(0b11000011) == 0x00)
      isLine = 0;
    if (line == 1)
    {
      //  Serial2.println(4);
      RFIDpos = 6;
      isPosData = 1;
      pattern4 = 5;
      cnt0 = 0;
      line = 0;
    }
    if ((cnt0 / 5) < speed_run_forward)
      runforwardline(cnt0 / 5);
    else
      runforwardline(speed_run_forward);
    break;
  case 5:
    goSlow(speed_run_forward, 310);
    DIREC = 1;
    turnRight(50);
    pattern4 = 6;
    cnt0 = 0;
    break;
  case 6:
    if (((sensor == 0xff) && (isLine == 0))||(RFIDpos == 5))
    {
      line++;
      isLine = 1;
    }
    else if (sensor_inp(0b11000011) == 0x00)
      isLine = 0;
    if (line == 1)
    {
      //Serial2.println(5);
      RFIDpos = 5;
      isPosData = 1;
      pattern4 = 7;
      cnt0 = 0;
      line = 0;
    }
    if ((cnt0 / 5) < speed_run_forward)
      runforwardline(cnt0 / 5);
    else
      runforwardline(speed_run_forward);
    break;
  case 7:
    goSlow(speed_run_forward, 310);
    DIREC = 3;
    turnLeft(50);
    //Serial2.println("!UPT;1;CND:" + (String)RFIDpos + (String)NPS + (String)DIREC + "#");
    pattern4 = 8;
    cnt0 = 0;
    break;
  case 8:
    if (((sensor == 0xff) && (isLine == 0))||(RFIDpos == 2))
    {
      line++;
      isLine = 1;
    }
    else if (sensor_inp(0b11000011) == 0x00)
      isLine = 0;
    if (line == 1)
    {
      // Serial2.println(2);
      RFIDpos = 2;
      isPosData = 1;
      pattern4 = 90;
      distance = 0;
      cnt0 = 0;
      line = 0;
    }
    if ((cnt0 / 5) < speed_run_forward)
      runforwardline(cnt0 / 5);
    else
      runforwardline(speed_run_forward);
    break;
  case 90:
    if (distance > 700)
    {
      DIREC = 3;
      RFIDpos = 0;
      isPosData = 1;
      pattern4 = 9;
      distance = 0;
      cnt0 = 0;
      line = 0;
      break;
    }
    else
      runforwardline(speed_run_forward);
    break;
  case 9:
    if ((distance / 4) < (speed_run_forward - 10))
    {
      runforwardline(speed_run_forward - (distance / 4));
    }
    else
    {
      runforwardline(0);
      DIREC = 0;
      turn180(43);
     // Serial2.println("!UPT;1;CND:" + (String)RFIDpos + (String)NPS + (String)DIREC + "#");
      pattern4 = 10;
      cnt0 = 0;
      distance = 0;
    }
    break;
  case 10:
    isRealSensor = 1;
    if ((cnt0 / 20) < 10)
      runBackwardLine(-(cnt0 / 20));
    else
      runBackwardLine(-10);
    if (distance > 75)
    {
      cnt0 = 0;
      pattern4 = 11;
    }
    break;
  case 11:
    runBackwardLine(-(10 - (cnt0 / 40)));
    if (cnt0 >= 400)
    {
      cnt0 = 0;
      pattern4 = 13;
    }
    if (distance > 93)
    {
      cnt0 = 0;
      pattern4 = 13;
    }
    break;
  case 13:
    Serial1.write(0xff);
    Serial1.write(0x04);
    Serial1.write(0xef);
    DIREC = 0;
//    Serial2.println("!UPT;1;CND:" + (String)RFIDpos + (String)NPS + (String)DIREC + "#");
    cnt0 = 0;
    pattern4 = 100;
    break;
  case 100:
    isRealSensor = 0;
    pos = 0;
    isRunning = 0;
    RFIDpos = 0;
    isPosData = 1;
    runforwardline(0);
    break;
  default:
    pattern4 = 100;
    break;
  }
}
void way5()
{
  switch (pattern5)
  {
  case 0:
    Serial1.write(0xff);
    Serial1.write(0x03);
    Serial1.write(0xef);
    cnt0 = 0;
    line = 0;
    step = 0;
    pattern5 = 1;
    digitalWrite(LED_PWM, HIGH);
    isRealSensor = 1;
    break;
  case 1:
    if ((cnt0 / 5) < 50)
    {
      runBackwardLine(-(cnt0 / 5));
    }
    else
    {
      runBackwardLine(-50);
      pattern5 = 2;
      cnt0 = 0;
      digitalWrite(LED_PWM, HIGH);
    }
    break;
  case 2:
    runBackwardLine(-(50 - (cnt0 / 2)));
    if ((cnt0 / 2) >= 50)
    {
      //Serial2.println(7);
      RFIDpos = 7;
      isPosData = 1;
      cnt0 = 0;
      pattern5 = 3;
      isRealSensor = 0;
    }
    break;
  case 3:
    turn180(50);
    pattern5 = 4;
    cnt0 = 0;
    line = 0;
    break;
  case 4:
    if ((sensor == 0xff) && (isLine == 0))
    {
      line++;
      isLine = 1;
    }
    else if (sensor_inp(0b11000011) == 0x00)
      isLine = 0;
    if (line == 1)
    {
      // Serial2.println(4);
      RFIDpos = 4;
      isPosData = 1;
      pattern5 = 5;
      cnt0 = 0;
      line = 0;
    }
    if ((cnt0 / 5) < speed_run_forward)
      runforwardline(cnt0 / 5);
    else
      runforwardline(speed_run_forward);
    break;
  case 5:
    if (cnt0 > 100)
    {
      goSlow(speed_run_forward, 100);
      turnLeft(50);
      pattern5 = 6;
      cnt0 = 0;
    }
    runforwardline(speed_run_forward);
    break;
  case 6:
    if ((sensor == 0xff) && (isLine == 0))
    {
      line++;
      isLine = 1;
    }
    else if (sensor_inp(0b11000011) == 0x00)
      isLine = 0;
    if ((line == 1) && (isSerialPrint == 0))
    {
      isSerialPrint = 1;
      // Serial2.println(5);
      RFIDpos = 5;
      isPosData = 1;
    }
    if (line == 2)
    {
      // Serial2.println(6);
      RFIDpos = 6;
      isPosData = 1;
      isSerialPrint = 0;
      pattern5 = 7;
      cnt0 = 0;
      line = 0;
    }
    if ((cnt0 / 5) < speed_run_forward)
      runforwardline(cnt0 / 5);
    else
      runforwardline(speed_run_forward);
    break;
  case 7:
    if (cnt0 > 100)
    {
      goSlow(speed_run_forward, 100);
      turnLeft(50);
      pattern5 = 8;
      cnt0 = 0;
    }
    runforwardline(speed_run_forward);
    break;
  case 8:
    if ((sensor == 0xff) && (isLine == 0))
    {
      line++;
      isLine = 1;
    }
    else if (sensor_inp(0b11000011) == 0x00)
      isLine = 0;
    if (line == 1)
    {
      // Serial2.println(9);
      RFIDpos = 9;
      isPosData = 1;
      Serial1.write(0xff);
      Serial1.write(0x02);
      Serial1.write(0xef);
      pattern5 = 9;
      cnt0 = 0;
      line = 0;
    }
    if ((cnt0 / 5) < speed_run_forward)
      runforwardline(cnt0 / 5);
    else
      runforwardline(speed_run_forward);
    break;
  case 9:
    if (cnt0 > 70)
    {
      goSlow(speed_run_forward, 100);
      pattern5 = 100;
      cnt0 = 0;
    }
    runforwardline(speed_run_forward);
    break;
  case 100:
    runforwardline(0);
    pos = 9;
    break;
  default:
    pattern5 = 100;
    break;
  }
}
void way6()
{
  switch (pattern6)
  {
  case 0:
    Serial1.write(0xff);
    Serial1.write(0x03);
    Serial1.write(0xef);
    cnt0 = 0;
    line = 0;
    step = 0;
    pattern6 = 1;
    digitalWrite(LED_PWM, HIGH);
    break;
  case 1:
    if ((cnt0 / 5) < 50)
    {
      speed_run(-(cnt0 / 5), -(cnt0 / 5));
    }
    else
    {
      speed_run(-50, -50);
      pattern6 = 2;
      cnt0 = 0;
      digitalWrite(LED_PWM, HIGH);
    }
    break;
  case 2:
    speed_run(-(50 - (cnt0 / 2)), -(50 - (cnt0 / 2)));
    if ((cnt0 / 2) >= 50)
    {
      //  Serial2.println(9);
      RFIDpos = 9;
      isPosData = 1;
      cnt0 = 0;
      pattern6 = 3;
    }
    break;
  case 3:
    turn180(50);
    pattern6 = 4;
    cnt0 = 0;
    line = 0;
    break;
  case 4:
    if ((sensor == 0xff) && (isLine == 0))
    {
      line++;
      isLine = 1;
    }
    else if (sensor_inp(0b11000011) == 0x00)
      isLine = 0;
    if (line == 1)
    {
      // Serial2.println(6);
      RFIDpos = 6;
      isPosData = 1;
      pattern6 = 5;
      cnt0 = 0;
      line = 0;
    }
    if ((cnt0 / 5) < speed_run_forward)
      runforwardline(cnt0 / 5);
    else
      runforwardline(speed_run_forward);
    break;
  case 5:
    if (cnt0 > 100)
    {
      goSlow(speed_run_forward, 100);
      turnRight(50);
      pattern6 = 6;
      cnt0 = 0;
    }
    runforwardline(speed_run_forward);
    break;
  case 6:
    if ((sensor == 0xff) && (isLine == 0))
    {
      line++;
      isLine = 1;
    }
    else if (sensor_inp(0b11000011) == 0x00)
      isLine = 0;
    if ((line == 1) && (isSerialPrint == 0))
    {
      isSerialPrint = 1;
      // Serial2.println(5);
      RFIDpos = 5;
      isPosData = 1;
    }
    if (line == 2)
    {
      //Serial2.println(4);
      RFIDpos = 4;
      isPosData = 1;
      pattern6 = 7;
      cnt0 = 0;
      line = 0;
    }
    if ((cnt0 / 5) < speed_run_forward)
      runforwardline(cnt0 / 5);
    else
      runforwardline(speed_run_forward);
    break;
  case 7:
    if (cnt0 > 100)
    {
      goSlow(speed_run_forward, 100);
      turnRight(50);
      pattern6 = 8;
      cnt0 = 0;
    }
    runforwardline(speed_run_forward);
    break;
  case 8:
    if ((sensor == 0xff) && (isLine == 0))
    {
      line++;
      isLine = 1;
    }
    else if (sensor_inp(0b11000011) == 0x00)
      isLine = 0;
    if (line == 1)
    {
      // Serial2.println(7);
      RFIDpos = 7;
      isPosData = 1;
      Serial1.write(0xff);
      Serial1.write(0x02);
      Serial1.write(0xef);
      pattern6 = 9;
      cnt0 = 0;
      line = 0;
    }
    if ((cnt0 / 5) < speed_run_forward)
      runforwardline(cnt0 / 5);
    else
      runforwardline(speed_run_forward);
    break;
  case 9:
    if (cnt0 > 70)
    {
      goSlow(speed_run_forward, 100);
      pattern6 = 100;
      cnt0 = 0;
    }
    runforwardline(speed_run_forward);
    break;
  case 100:
    pos = 7;
    runforwardline(0);
    break;
  default:
    pattern6 = 100;
    break;
  }
}
