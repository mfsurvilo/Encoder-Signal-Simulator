// Encoder Simulator Built on Teensy with 2 Analog Pots to Control "Speed"

#include <Arduino.h>
#include "TeensyTimerTool.h"
#include "EncoderSignalSimulator.h"
using namespace TeensyTimerTool;
using namespace EncoderSignalSimulator;

int PWM1_A = 39;
int PWM1_B = 40;
int PWM2_A = 33;
int PWM2_B = 34;
int BANG_PIN = 13;
int ANALOG0 = 15;
int ANALOG1 = 14;

int analog_in;

int max_delay = 20000; // microseconds
int min_delay = 810;   // microseconds

PeriodicTimer Encoder1Timer, Encoder2Timer;
EncSim Encoder1;



//EncSim Encoder2(PWM2_A, PWM2_B);  // Encoder signal generation on pins

void update_delay(EncSim &EncoderX, int analog_pin) {
  int analog_in = analogRead(analog_pin);
  if (analog_in > 1023 / 2) {
    EncoderX.total_delay =
        map(analog_in, 1023 / 2, 1023, max_delay, min_delay);
    EncoderX.current_direction = EncSim::Direction::CW;
  } else {
    EncoderX.total_delay = map(analog_in, 1023 / 2, 0, max_delay, min_delay);
    EncoderX.current_direction = EncSim::Direction::CCW;
  }
}

void setup() {
  pinMode(A0, INPUT);
  Serial.begin(9600);
  Serial.println("Started Serial Comm");
  Encoder1.EncoderSimulator(PWM1_A, PWM1_B,13);  // Encoder signal generation on pins 
  Encoder1.SetupDelays(min_delay, max_delay);
  Encoder1Timer.begin([] { Encoder1.bashpins(); }, 20); //20microsecond trigger
  // Encoder2Timer.begin([] { Encoder2.bashpins(); }, 20);
}

void loop() 
{
  update_delay(Encoder1, ANALOG0); 
  // update_delay(Encoder2, ANALOG1);
}
