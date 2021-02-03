#include "Define.h"
extern volatile int iServoPwm;
extern unsigned int cnt0, cnt1, cnt2;
extern volatile unsigned int sensor;
extern int handle_center;
extern unsigned long lastTime;
volatile unsigned char Ldir;
volatile unsigned char Rdir;
extern volatile unsigned int distance;
extern unsigned char isRunning;
extern unsigned char DIREC;
extern unsigned char NPS;
extern volatile unsigned char RFIDpos;
//--------------------------------------------------------------------------//
char check_crosshalfline() // hàm kiểm tra nữa line ngang
{
  if ((check_crossline() == 0))
  {
    if (sensor_inp(0xf0) == 0xf0)
      return 1;
    else if (sensor_inp(0x0f) == 0x0f)
      return 2;
    else
      return 0;
  }
  else
    return 0;
}
//Chuyen lan trai
//////////////////////////////////////////////////////////////////////////////////////////

void runforwardline(int tocdo) // hàm chạy bám line
{
  digitalWrite(LED_PWM, HIGH);
  unsigned int temp;
  temp = sensor_inp(0xff);
  //Serial.println(temp, BIN);
  switch (temp)
  {
  case 0x18: // 00011000
  case 0x3c: // 00111100
  case 0x38: // 00111000
  case 0x1C: // 00011100
    brake(0);
    handleAndSpeed(iServoPwm, tocdo);
    break;
  case 0x80: // 100000000
  case 0xc0: // 110000000
  case 0xe0: // 11100000
    brake(0);
    handleAndSpeed(iServoPwm, tocdo);
    break;
  case 0x01: // 00000001
  case 0x03: // 00000011
  case 0x07: // 00000111

    brake(0);
    handleAndSpeed(iServoPwm, tocdo);
    break;
  case 0x00: //00000000

  //  handleAndSpeed(0, 0);
  //  brake(0);
  //  break;
  case 0xf0: // 11110000
  case 0xf8: // 11111000
  case 0xfc: // 11111100
  case 0x0f: // 00001111
  case 0x1f: // 00011111
  case 0x3f: // 00111111
  case 0xff: // 11111111
    handleAndSpeed(0, tocdo);
    brake(0);
    break;
  default:
    brake(0);
    handleAndSpeed(iServoPwm, tocdo);
    break;
  }
}
void runBackwardLine(int tocdo) // hàm chạy bám line
{
  digitalWrite(LED_PWM, HIGH);
  brake(0);
  handleAndSpeed(iServoPwm, tocdo);
}
//---------------------------------------------------------------------------//
// hàm bẻ góc đồng thời tính vi sai cho 2 bánh
void handleAndSpeed(int angle, int speed1)
{
  int speedLeft;
  int speedRight;
  speedLeft = speed1 + angle;
  speedRight = speed1 - angle;
  speed_run(speedLeft, speedRight);
}
//--------------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------//
void speed_run(int speedDC_left, int speedDC_right) // hàm truyền vào tốc độ động cơ trái + phải
{
  if (speedDC_left > 255)
    speedDC_left = 255;
  if (speedDC_left < -255)
    speedDC_left = -255;
  if (speedDC_right > 255)
    speedDC_right = 255;
  if (speedDC_right < -255)
    speedDC_right = -255;
  if (speedDC_left < 0)
  {
    analogWrite(LPWM, -speedDC_left);
    digitalWrite(LDIR, HIGH);
    Ldir = 0;
  }
  else if (speedDC_left >= 0)
  {
    speedDC_left = speedDC_left;
    analogWrite(LPWM, speedDC_left);
    digitalWrite(LDIR, LOW);
    Ldir = 1;
  }
  if (speedDC_right < 0)
  {
    analogWrite(RPWM, -speedDC_right);
    digitalWrite(RDIR, LOW);
    Rdir = 0;
  }
  else if (speedDC_right >= 0)
  {
    speedDC_right = speedDC_right;
    analogWrite(RPWM, speedDC_right);
    digitalWrite(RDIR, HIGH);
    Rdir = 1;
  }
}
//-------------------------------------------------------------//
int brake(int _en)
{
  if (_en == 1)
  {
    digitalWrite(RBRAKE, HIGH);
    digitalWrite(LBRAKE, HIGH);
  }
  else
  {
    digitalWrite(RBRAKE, LOW);
    digitalWrite(LBRAKE, LOW);
  }
}
//-------------------------------------------------------------//
unsigned long getTimer()
{
  return (millis() - lastTime);
}
//-------------------------------------------------------------//
void resetTimer()
{
  lastTime = millis();
}
//-------------------------------------------------------------//

// GOC VUONG
/////////////////////////////////////////////////////////////////////////////////////////
int turn180(int tocdo)
{
  cnt0 = 0;
  int status180 = 0;
  distance = 0;
  Serial2.println("!UPT;1;SPE:0#");
  Serial2.println("!UPT;1;STA:0#");
  while (1)
  {
    wdt_reset();
    serial2Process();
    // serial3Process();
    switch (status180)
    {
    case 0:
      if ((cnt0 / 5) < tocdo)
      {
        speed_run(cnt0 / 5, -(cnt0 / 5));
      }
      else
        speed_run(tocdo, -tocdo);
      if ((sensor_inp(0b01111110) == 0x00) || (distance > 270))
      {
        status180 = 1;
      }
      break;
    case 1:
      if ((cnt0 / 5) < tocdo)
      {
        speed_run(cnt0 / 5, -(cnt0 / 5));
      }
      else
        speed_run(tocdo, -tocdo);
      if ((sensor_inp(0b00011000) != 0) || (distance > 270))
      {
        status180 = 2;
      }
      break;
    case 2:
      if ((cnt0 / 5) < tocdo)
      {
        speed_run(cnt0 / 5, -(cnt0 / 5));
      }
      else
        speed_run(tocdo, -tocdo);
      if ((sensor_inp(0b00111100) == 0) || (distance > 350))
      {
        status180 = 3;
        cnt0 = 0;
      }
      break;
    case 3:
      speed_run(tocdo - (cnt0 / 19), -(tocdo - (cnt0 / 19)));
      // speed_run(tocdo, -tocdo);
      if (sensor_inp(0b00111000) != 0)
      {
        isRunning = 1;
        Serial2.println("!UPT;1;CND:" + (String)RFIDpos + (String)NPS + (String)DIREC + "#");
        Serial2.println("!UPT;1;SPE:0#");
        Serial2.println("!UPT;1;STA:0#");
        cnt0 = 0;
        //  return 11;
        status180 = 4;
      }
      break;
    case 4:
      speed_run(0, 0);
      if (cnt0 > 400)
      {
        Serial2.println("!UPT;1;CND:" + (String)RFIDpos + (String)NPS + (String)DIREC + "#");
        Serial2.println("!UPT;1;SPE:400#");
        Serial2.println("!UPT;1;STA:1#");
        return 11;
      }
      break;
    default:
      break;
    }
  }
}
int turnLeft(int tocdo)
{
  cnt1 = 0;
  int statusLeft = 0;
  Serial2.println("!UPT;1;SPE:0#");
  Serial2.println("!UPT;1;STA:0#");
  while (1)
  {
    wdt_reset();
    serial2Process();
    // serial3Process();
    switch (statusLeft)
    {
    case 0:
      if ((cnt0 / 5) < tocdo)
      {
        speed_run(-(cnt0 / 5), cnt0 / 5);
      }
      else
        speed_run(-tocdo, tocdo);
      if (sensor_inp(0b00001111) == 0)
        statusLeft = 1;
      break;
    case 1:
      if ((cnt0 / 5) < tocdo)
      {
        speed_run(-(cnt0 / 5), cnt0 / 5);
      }
      else
        speed_run(-tocdo, tocdo);
      if (sensor_inp(0b11000000) != 0)
        statusLeft = 2;
      break;
    case 2:
      speed_run(-(tocdo - (cnt0 / 15)), (tocdo - (cnt0 / 15)));
      if (sensor_inp(0b00011000) != 0)
      {
        // isRunning = 1;
        Serial2.println("!UPT;1;CND:" + (String)RFIDpos + (String)NPS + (String)DIREC + "#");
        Serial2.println("!UPT;1;SPE:0#");
        Serial2.println("!UPT;1;STA:0#");
        cnt0 = 0;
        statusLeft = 3;
        // return 11;
      }
      break;
    case 3:
      speed_run(0, 0);
      if (cnt0 > 400)
      {
        Serial2.println("!UPT;1;CND:" + (String)RFIDpos + (String)NPS + (String)DIREC + "#");
        Serial2.println("!UPT;1;SPE:400#");
        Serial2.println("!UPT;1;STA:1#");
        return 11;
      }
      break;
    default:
      break;
    }
  }
}
int turnRight(int tocdo)
{
  cnt1 = 0;
  int statusRight = 0;
  Serial2.println("!UPT;1;SPE:0#");
  Serial2.println("!UPT;1;STA:0#");
  while (1)
  {
    wdt_reset();
    serial2Process();
    // serial3Process();
    switch (statusRight)
    {
    case 0:
      if ((cnt0 / 5) < tocdo)
      {
        speed_run((cnt0 / 5), -(cnt0 / 5));
      }
      else
        speed_run(tocdo, -tocdo);
      if (sensor_inp(0b11110000) == 0)
        statusRight = 1;
      break;
    case 1:
      if ((cnt0 / 5) < tocdo)
      {
        speed_run((cnt0 / 5), -(cnt0 / 5));
      }
      else
        speed_run(tocdo, -tocdo);
      if (sensor_inp(0b00000011) != 0)
        statusRight = 2;
      break;
    case 2:
      speed_run((tocdo - (cnt0 / 15)), -(tocdo - (cnt0 / 15)));
      if (sensor_inp(0b00011000) != 0)
      {
        //isRunning = 1;
        Serial2.println("!UPT;1;CND:" + (String)RFIDpos + (String)NPS + (String)DIREC + "#");
        Serial2.println("!UPT;1;SPE:0#");
        Serial2.println("!UPT;1;STA:0#");
        cnt0 = 0;
        statusRight = 3;
        //return 11;
      }
      break;
    case 3:
      speed_run(0, 0);
      if (cnt0 > 400){
        Serial2.println("!UPT;1;CND:" + (String)RFIDpos + (String)NPS + (String)DIREC + "#");
        Serial2.println("!UPT;1;SPE:400#");
        Serial2.println("!UPT;1;STA:1#");
        return 11;
      }
      break;
    default:
      break;
    }
  }
}
int goSlow(int _speed, int _distance)
{
  int statusSlow = 0;
  cnt0 = 0;
  distance = 0;
  while (1)
  {
    wdt_reset();
    serial2Process();
    serial3Process();
    switch (statusSlow)
    {
    case 0:
      if (_distance < distance)
      {
        _distance = distance;
      }
      //  speed_run(map((_distance - distance), 0, _distance, 0, _speed), map((_distance - distance), 0, _distance, 00, _speed));
      runforwardline(map((_distance - distance), 0, _distance, 0, _speed));
      //  speed_run(_speed - (cnt0 / 2), _speed - (cnt0 / 2));
      if (map((_distance - distance), 0, _distance, 0, _speed) <= 5)
      {
        cnt0 = 0;
        return 1;
      }
      if (cnt0 > 400)
      {
        cnt0 = 0;
        return 1;
      }
      break;

    default:
      break;
    }
  }
}

int turnLeft_2(int tocdo)
{
  cnt1 = 0;
  int statusLeft = 0;
  while (1)
  {
    wdt_reset();
    serial2Process();
    // serial3Process();
    switch (statusLeft)
    {
    case 0:
      if ((cnt1) < tocdo)
      {
        speed_run(tocdo - cnt1, tocdo);
      }
      else
        speed_run(0, tocdo);
      if (sensor_inp(0b00011111) == 0)
      {
        statusLeft = 1;
      }

      break;
    case 1:
      if ((cnt1) < tocdo)
      {
        speed_run(tocdo - cnt1, tocdo);
      }
      else
        speed_run(0, tocdo);
      if (sensor_inp(0b11110000) != 0)
      {
        cnt0 = 0;
        statusLeft = 2;
      }

      break;
    case 2:
      // speed_run(-(tocdo - (cnt0 / 15)), (tocdo - (cnt0 / 15)));
      if ((cnt1) < tocdo)
      {
        speed_run(tocdo - cnt1, tocdo - cnt0);
      }
      else
        speed_run(0, tocdo - cnt0);
      if (sensor_inp(0b01111000) != 0)
      {
        speed_run(0, 0);
        cnt0 = 0;
        statusLeft = 3;
        // return 11;
      }
      break;
    case 3:
      if (cnt0 > 400)
        return 11;
      break;
    default:
      break;
    }
  }
}