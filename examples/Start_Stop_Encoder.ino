/*
Encoder Simulator Built on Teensy with an 
analog potentiometer to set the speed and direction.
*/

#include "EncoderSignalSimulator.h"

using namespace EncoderSignalSimulator;

#define PWM1_A 39
#define PWM1_B 40
#define BANG_PIN 13
#define ANALOG0 15
#define POT_RESOLUTION 10  // bits

int max_delay = 40000;    // uS
int min_delay = 1620;     // uS
int timer_trigger = 20;   // uS

int start;  // mS

EncSim FakeEncoder;

void setup() {
  FakeEncoder.EncoderSimulator(PWM1_A, PWM1_B, BANG_PIN, ANALOG0, POT_RESOLUTION);
  FakeEncoder.SetupDelays(min_delay/2, max_delay/2);
  FakeEncoder.SetupTimer(timer_trigger);
  start = millis();
}

void loop()
{
if ((millis()- start > 5000) && (millis()- start < 10000))
  {
    FakeEncoder.Stop();  // Stops interrupt calling.
  }

  if (millis()- start > 10000)
  {
    FakeEncoder.Start();
  }

  FakeEncoder.UpdateDelay();
}
