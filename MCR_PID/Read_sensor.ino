#include "Define.h"

volatile unsigned int sensor;
unsigned int sensorValue[10];
unsigned int sensorMin[10] = {350, 420, 515, 380, 345, 505, 490, 520, 500, 490};
unsigned int sensorMax[10] = {525, 580, 679, 558, 534, 680, 666, 673, 658, 659};
unsigned int RsensorMin[10] = {0, 0, 0, 280, 335, 276, 270, 0, 0, 0};
unsigned int RsensorMax[10] = {1000, 1000, 1000, 680, 700, 640, 500, 1000, 1000, 1000};
volatile int iSensorBefore;
volatile unsigned char isCalib = 0;
volatile int iServoPwm;
volatile char isRealSensor = 0;
unsigned char sensor_inp(unsigned char MASK) // hàm mặt nạ để che cảm biến
{
  // sensor = read_sensor();
  return (sensor & MASK);
}
//-------------------------------------------------------------------//
void read_sensor() // hàm đọc cảm biến
{
  unsigned char temp = 0;
  unsigned int sum = 0;
  unsigned long avg = 0;
  int i, iP, iD;
  int kp;
  int kd;
  int iRet;
  if (isRealSensor == 1)
  {
    sensorValue[0] = 0;
    sensorValue[1] = 0;
    sensorValue[2] = 0;
    sensorValue[3] = 1023 - analogRead(SENSOR_12);
    sensorValue[4] = 1023 - analogRead(SENSOR_11);
    sensorValue[5] = 1023 - analogRead(SENSOR_14);
    sensorValue[6] = 1023 - analogRead(SENSOR_13);
    sensorValue[7] = 0;
    sensorValue[8] = 0;
    sensorValue[9] = 0;
  }
  else
  {
    sensorValue[0] = 1023 - analogRead(SENSOR_1);
    sensorValue[1] = 1023 - analogRead(SENSOR_2);
    sensorValue[2] = 1023 - analogRead(SENSOR_3);
    sensorValue[3] = 1023 - analogRead(SENSOR_4);
    sensorValue[4] = 1023 - analogRead(SENSOR_5);
    sensorValue[5] = 1023 - analogRead(SENSOR_6);
    sensorValue[6] = 1023 - analogRead(SENSOR_7);
    sensorValue[7] = 1023 - analogRead(SENSOR_8);
    sensorValue[8] = 1023 - analogRead(SENSOR_9);
    sensorValue[9] = 1023 - analogRead(SENSOR_10);
  }
  for (int j = 0; j < 10; j++)
  {
    if (isRealSensor == 1)
    {

      if (sensorValue[j] < RsensorMin[j])
        sensorValue[j] = RsensorMin[j];
      if (sensorValue[j] > RsensorMax[j])
        sensorValue[j] = RsensorMax[j];
      sensorValue[j] = map(sensorValue[j], RsensorMin[j], RsensorMax[j], 0, 1000);
    }
    else if (isCalib == 0)
    {

      if (sensorValue[j] < sensorMin[j])
        sensorValue[j] = sensorMin[j];
      if (sensorValue[j] > sensorMax[j])
        sensorValue[j] = sensorMax[j];
      sensorValue[j] = map(sensorValue[j], sensorMin[j], sensorMax[j], 0, 1000);
    }
    if ((j > 0) && (j < 9))
    {
      temp = temp << 1;
      if (sensorValue[j] > 800)
      {
        temp |= 0x01;
      }
      else if (sensorValue[j] < 650)
      {
        temp &= 0xfe;
      }
      sensor = temp;
    }
  }
  for (int j = 0; j < 10; j++)
  {
    avg += (long)(sensorValue[j]) * ((j + 1) * 1000);
    sum += sensorValue[j];
  }
  i = (int)((avg / sum) - 5500);
  kp = 1;
  kd = 1;
  iP = kp * i;
  iD = kd * (iSensorBefore - i);
  iRet = (iP - iD);
  if ((iRet < -4500))
  {
    iRet = 0;
  }
  iServoPwm = iRet / 40;
  if (isRealSensor == 1)
  {
    iServoPwm = iServoPwm * 2 / 3;
    iServoPwm -= 4;
  }
  iSensorBefore = i;

  //Serial.println(iServoPwm);
}
void readSensorMin()
{
  digitalWrite(LED_PWM, HIGH);
  delay(50);
  isCalib = 1;
  read_sensor();
  if (sensorMin[0] < sensorValue[0])
  {
    for (int i = 0; i < 10; i++)
    {
      sensorMin[i] = sensorMin[i] + 1;
    }
  }
  else
  {
    for (int i = 0; i < 10; i++)
    {
      sensorMin[i] = sensorMin[i] - 1;
    }
  }

  if (sensorMax[5] < sensorValue[5])
  {
    for (int i = 0; i < 10; i++)
    {
      sensorMax[i] = sensorMax[i] + 1;
    }
  }
  else if(sensorMax[5] > (sensorMin[5] + 100))
  {
    for (int i = 0; i < 10; i++)
    {
      sensorMax[i] = sensorMax[i] - 1;
    }
  }
  isCalib = 0;
}