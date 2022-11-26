/* 
Encoder Simulator Library
Created by Matt Survilo, Nov 25, 2022.
Released into the public domain.
*/
#include <Arduino.h>
#include "EncoderSignalSimulator.h"
using namespace EncoderSignalSimulator;

// enum EncSim::EncoderState { LL = 1, LH = 2, HH = 3, HL = 4 };
// enum EncSim::Direction { CW = 1, CCW = 2 };

// enum portList {
//             na = -1, Port_A, Port_B, Port_C, Port_D, Port_E
//         };


void EncSim::EncoderSimulator(unsigned _chanA_pin, unsigned _chanB_pin,  unsigned _bang_pin)
{
  chanA_pin = _chanA_pin;
  chanB_pin = _chanB_pin;
  bang_pin = _bang_pin;

  pinMode(chanA_pin, OUTPUT);
  pinMode(chanB_pin, OUTPUT);
  pinMode(bang_pin, OUTPUT);

  current_state = EncoderState::LL;
  current_direction = Direction::CW;

  if (bang_pin == 100) {DeActivateBangPin();}
  else {ActivateBangPin(bang_pin);}
}

void EncSim::ActivateBangPin(unsigned _bang_pin)
{
    bang_pin = _bang_pin;
    pinMode(bang_pin, OUTPUT);
    bang = true;
}

void EncSim::SetupDelays(unsigned _min_delay, unsigned _max_delay)
{
    min_delay = _min_delay;
    max_delay = _max_delay;
}

void EncSim::DeActivateBangPin()
{
  bang = false;
}

void EncSim::UpdateDelay(unsigned _max_delay)
{
  max_delay = _max_delay;
}

void EncSim::EnterLL()
{
  digitalWrite(chanA_pin, LOW);
  digitalWrite(chanB_pin, LOW);
  current_state = EncoderState::LL;
}

void EncSim::EnterHL()
{
  digitalWrite(chanA_pin, HIGH);
  digitalWrite(chanB_pin, LOW);
  current_state = EncoderState::HL;
}

void EncSim::EnterHH()
{
  digitalWrite(chanA_pin, HIGH);
  digitalWrite(chanB_pin, HIGH);
  current_state = EncoderState::HH;
}

void EncSim::EnterLH()
{
  digitalWrite(chanA_pin, LOW);
  digitalWrite(chanB_pin, HIGH);
  current_state = EncoderState::LH;
}

void EncSim::bashpins()
{
  if (bang)
  {
      digitalWriteFast(bang_pin,!digitalReadFast(bang_pin));
  }

  if (((micros() - interrupt_timer_start) > total_delay) && 
      (total_delay < .958L * max_delay)) 
  { 
    interrupt_timer_start = micros();
    Serial.println("Inside timer");
    if (current_direction == CW) 
    {
      switch (current_state) 
      {
      case EncoderState::LL:
        EncSim::EnterHL();
        break;
      case EncoderState::HL:
        EncSim::EnterHH();
        break;
      case EncoderState::HH:
        EncSim::EnterLH();
        break;
      case EncoderState::LH:
        EncSim::EnterLL();
        break;
      }
    } 
    
    else if (current_direction == CCW) 
    {
      switch (current_state) 
      {
      case EncoderState::LL:
        EncSim::EnterLH();
        break;
      case EncoderState::LH:
        EncSim::EnterHH();
        break;
      case EncoderState::HH:
        EncSim::EnterHL();
        break;
      case EncoderState::HL:
        EncSim::EnterLL();
        break;
      }
    }
  }
}



