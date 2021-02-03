#include <avr/iom2560.h>
#include "Define.h"
unsigned int cnt0, cnt1, cnt2;
volatile unsigned int distance;
volatile int realSpeed;
volatile int MotorLPulse;
volatile int MotorRPulse;
extern volatile unsigned char Ldir;
extern volatile unsigned char Rdir;
void timer_init() {
  // Timer/Counter 2 initialization
  // Clock source: System Clock
  // Clock value: 15.625 kHz
  // Mode: Normal top=0xFF
  // OC2A output: Disconnected
  // OC2B output: Disconnected
  // Timer Period: 4.992 ms
  ASSR = (0 << EXCLK) | (0 << AS2);
  TCCR2A = (0 << COM2A1) | (0 << COM2A0) | (0 << COM2B1) | (0 << COM2B0) | (0 << WGM21) | (0 << WGM20);
  TCCR2B = (0 << WGM22) | (1 << CS22) | (1 << CS21) | (1 << CS20);
  TCNT2 = 0xB2;
  OCR2A = 0x00;
  OCR2B = 0x00;
  // Timer/Counter 2 Interrupt(s) initialization
  TIMSK2 = (0 << OCIE2B) | (0 << OCIE2A) | (1 << TOIE2);
}
ISR (TIMER2_OVF_vect) {
  TCNT2 = 0xB2;
  read_sensor();
  cnt0++;
  cnt1++;
  cnt2++;
  if(cnt2 >= 20){
    cnt2 = 0;
    if(Rdir == 0){
     realSpeed = (MotorLPulse - MotorRPulse); 
    }
    else if(Ldir == 0){
     realSpeed = (MotorRPulse - MotorLPulse); 
    }
    else realSpeed = (MotorLPulse + MotorRPulse)/2;
    if (realSpeed < 0)
    {
      realSpeed = -realSpeed;
    }
  //  distance += realSpeed;
    realSpeed = (139*realSpeed)/10;
   // realSpeed = 400;
    MotorLPulse = 0;
    MotorRPulse = 0;
  }
  wdt_reset();
}
void exInterrupt_init(){
  distance = 0;
  realSpeed = 0;
  MotorLPulse = 0;
  MotorRPulse = 0;
  attachInterrupt(digitalPinToInterrupt(2), exIRS1, RISING);
  attachInterrupt(digitalPinToInterrupt(3), exIRS2, RISING);
}
void exIRS1(){
MotorLPulse++;
distance++;
}
void exIRS2(){
MotorRPulse++;
}