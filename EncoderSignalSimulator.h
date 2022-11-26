/* 
Encoder Simulator Library
Created by Matt Survilo, Nov 25, 2022.
Released into the public domain.
*/

#ifndef EncoderSignalSimulator_H
#define EncoderSignalSimulator_H

#include "Arduino.h"
namespace EncoderSignalSimulator
{
class EncSim {
    public:    
        void EncoderSimulator(unsigned, unsigned, unsigned = 100);
        void ActivateBangPin(unsigned);
        void DeActivateBangPin();
        void SetupDelays(unsigned, unsigned);
        void bashpins();
        void UpdateDelay(unsigned);

        unsigned long total_delay;
        int interrupt_timer_start;
        unsigned max_delay;
        unsigned min_delay;
        
        bool bang;
        
        enum EncoderState { LL = 1, LH = 2, HH = 3, HL = 4 };
        enum Direction { CW = 1, CCW = 2 };

        EncoderState current_state;
        Direction current_direction;

        unsigned chanA_pin, chanB_pin, bang_pin;
    private:
        void EnterLL();
        void EnterHL();
        void EnterHH();
        void EnterLH();

};

};
#endif