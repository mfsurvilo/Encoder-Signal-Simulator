/* 
Encoder Simulator Cpp Library
Created by Matt Survilo, Nov 25, 2022.
Released into the public domain.
*/

#include <Arduino.h>
#include "TeensyTimerTool.h"
#include "EncoderSignalSimulator.h"
using namespace EncoderSignalSimulator;

void EncSim::EncoderSimulator(unsigned _chanA_pin, unsigned _chanB_pin,  unsigned _bang_pin, unsigned _analog_pot_pin, unsigned _pot_resolution)
{
  chanA_pin = _chanA_pin;
  chanB_pin = _chanB_pin;
  bang_pin = _bang_pin;
  analog_pot_pin = _analog_pot_pin;
  analog_resolution = _pot_resolution;

  pinMode(chanA_pin, OUTPUT);
  pinMode(chanB_pin, OUTPUT);
  pinMode(bang_pin, OUTPUT);

  current_state = EncoderState::LL;
  current_direction = Direction::CW;

  if (bang_pin == 100) {DeActivateBangPin();}
  else {ActivateBangPin(bang_pin);}

  if (analog_pot_pin != 100) {SetupAnalogPot(analog_pot_pin, analog_resolution);}
}

void EncSim::SetupAnalogPot(unsigned _analog_pot_pin, unsigned _resolution)
{
  analog_pot_pin = _analog_pot_pin;
  pinMode(analog_pot_pin, INPUT);
  using_potentiometer = true;
  analog_resolution = pow(2,_resolution);
}

void EncSim::SetupTimer(unsigned _trigger_delay)
{
  Timer.begin([this] { this->bashpins(); }, _trigger_delay);
}

void EncSim::SetupDelays(unsigned _min_wave_width, unsigned _max_wave_width)
{
    min_wave_width = _min_wave_width;
    max_wave_width = _max_wave_width;
}

void EncSim::ActivateBangPin(unsigned _bang_pin)
{
  bang_pin = _bang_pin;
  pinMode(bang_pin, OUTPUT);
  using_bang = true;
}

void EncSim::DeActivateBangPin()
{
  using_bang = false;
}

void EncSim::Stop()
{
  Timer.stop();
}

void EncSim::Start()
{
  Timer.start();
}

void EncSim::UpdateDelay(unsigned _raw_delay, Direction _current_direction)
{
  if (using_potentiometer)
  {
    unsigned analog_in = analogRead(analog_pot_pin);
    if (analog_in > analog_resolution / 2) {
      deired_wave_width = map(analog_in, analog_resolution / 2, 1023, max_wave_width, min_wave_width);
      current_direction = EncSim::Direction::CW;
    } else {
      deired_wave_width = map(analog_in, 1023 / 2, 0, max_wave_width, min_wave_width);
      current_direction = EncSim::Direction::CCW;
    }
  }
  else
  {
    deired_wave_width = _raw_delay;
    current_direction = _current_direction;
  }
}

void EncSim::bashpins()
{
  if (using_bang)
  {
      digitalWriteFast(bang_pin,!digitalReadFast(bang_pin));
  }

  if (((micros() - interrupt_timer_start) > deired_wave_width) &&
      (deired_wave_width < .958L * max_wave_width))
  {
    interrupt_timer_start = micros();
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
