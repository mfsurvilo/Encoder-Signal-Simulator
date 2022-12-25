// Encoder Simulator Built on Teensy with 2 Analog Pots to Control "Speed"

#include "TeensyTimerTool.h"
using namespace TeensyTimerTool;
#include <EncoderSignalSimulator.h>

//Bash pins not oop


const int PWM1_A = 39;
const int PWM1_B = 40;

int analog_in;
volatile int checking = 4;
volatile int total_delay;
int half_max_delay = 300;
volatile int start;
enum EncoderState 
  { LL = 1,
    LH = 2,
    HH = 3,
    HL = 4  
  };
//IntervalTimer Encoder1_Timer;
//IntervalTimer Encoder2_Timer;
PeriodicTimer Encoder1Timer;

volatile EncoderState current_state = EncoderState::LL;


void EncoderCheck()
{
  if (((millis()-start) > total_delay) && (total_delay < .95* 2 * half_max_delay))
  {

  switch(current_state) 
    {
      case EncoderState::LL: 
      digitalWrite(PWM1_A, HIGH);
      digitalWrite(PWM1_B, LOW);
      current_state = EncoderState::HL;
      break;
      
      case EncoderState::HL: 
      digitalWrite(PWM1_A, HIGH);
      digitalWrite(PWM1_B, HIGH);
      current_state = EncoderState::HH;
      break;
      
      case EncoderState::HH : 
      digitalWrite(PWM1_A, LOW);
      digitalWrite(PWM1_B, HIGH);      
      current_state = EncoderState::LH;
      break;
      
      case EncoderState::LH : 
      digitalWrite(PWM1_A, LOW);
      digitalWrite(PWM1_B, LOW);      
      current_state = EncoderState::LL;
      break;     
    }    
    start = millis();
    }
}


void setup()
{
  pinMode(PWM1_A, OUTPUT);
  pinMode(PWM1_B, OUTPUT);
  pinMode(PWM2_A, OUTPUT);
  pinMode(PWM2_B, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600);
  Serial.println("Started Serial Comm");
  
  Encoder1Timer.begin(EncoderCheck, 15000);
//  Encoder2Timer.begin(EncoderCheck, 15000);




}





void loop() 
{
  analog_in = analogRead(A0);
  total_delay = 2*map(analog_in, 0, 1023, 0, half_max_delay);
  Serial.println(total_delay);
  
  //Serial.println(total_delay);
}
