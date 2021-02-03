#include <Arduino.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <pins_arduino.h>
#define RDIR 4    // chiều quay động cơ phải
#define LDIR 5   // chiều quay động cơ trái
#define LPWM 6    // tốc độ động cơ trái
#define RPWM 7    // tốc độ động cơ phải
#define LED13 13   // Chân led4 trên mạch
#define SENSOR_1 A0
#define SENSOR_2 A1
#define SENSOR_3 A2
#define SENSOR_4 A3
#define SENSOR_5 A4
#define SENSOR_6 A5
#define SENSOR_7 A6
#define SENSOR_8 A7
#define SENSOR_9 A8
#define SENSOR_10 A9
#define SENSOR_11 A10
#define SENSOR_12 A11
#define SENSOR_13 A12
#define SENSOR_14 A13
#define SENSOR_15 A14
#define CHARGE_SENSOR A15
#define LSPEED 2
#define RSPEED 3
#define LED_PWM 8
#define LBRAKE 9
#define RBRAKE 10
#define MOTOR_RELAY 11
#define CHARGE_RELAY 12
#define CHARGE_GND_RELAY 20
#define SW1 27
#define SW2 25
#define SW3 23
#define SW4 21


extern HardwareSerial Serial;
extern HardwareSerial Serial1;
extern HardwareSerial Serial2;
extern HardwareSerial Serial3;



char check_crossline();
char check_crosshalfline();
int left_lance(int tocdo);
int right_lance(int tocdo);
int turnLeft(int tocdo);
int turnLeft_2(int tocdo);
int turnRight(int tocdo);
int turn180(int tocdo);
int goSlow(int _speed, int _distance);
void runforwardline(int tocdo) ;
void runBackwardLine(int tocdo) ;
void handleAndSpeed(int angle, int speed1);
void speed_run(int speedDC_left, int speedDC_right);
void handle(int _angle);
unsigned long getTimer();
void resetTimer();
void read_sensor();
unsigned char sensor_inp(unsigned char MASK);
void timer_init();
void exInterrupt_init();
void exIRS1();
int brake(int _en);
void serial1Process();
void serial2Process();
void serial3Process();
void readSensorMin();
