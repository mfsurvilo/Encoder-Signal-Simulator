/*
Encoder Simulator Library
Created by Matt Survilo, Nov 25, 2022.
Released into the public domain.
*/

#ifndef EncoderSignalSimulator_H
#define EncoderSignalSimulator_H

#include "Arduino.h"
#include "TeensyTimerTool.h"

namespace EncoderSignalSimulator
{
class EncSim {
    public:
        enum EncoderState { LL = 1, LH = 2, HH = 3, HL = 4 };
        enum Direction { CW = 1, CCW = 2 };
        void EncoderSimulator(unsigned, unsigned, unsigned = 100, unsigned = 100, unsigned = 10);
        void ActivateBangPin(unsigned);
        void DeActivateBangPin();
        void SetupDelays(unsigned, unsigned);
        void UpdateDelay(unsigned = 50, Direction = CW);
        void SetupAnalogPot(unsigned,unsigned = 10);
        void SetupTimer(unsigned);

        void Stop();
        void Start();

        void UpdateDelay(unsigned);

        int interrupt_timer_start;

        unsigned deired_wave_width;
        unsigned max_wave_width;
        unsigned min_wave_width;

        bool using_bang, using_potentiometer;

        EncoderState current_state;
        Direction current_direction;

        unsigned chanA_pin, chanB_pin, bang_pin, analog_pot_pin, analog_resolution;
        private:
            void EnterLL();
            void EnterHL();
            void EnterHH();
            void EnterLH();

            void bashpins();
    protected:
        TeensyTimerTool::PeriodicTimer Timer;
};

} //namespace EncoderSignalSimulator
#endif