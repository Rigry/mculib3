#include "encoder_rotary.h"

template<class Encoder>
class Step_motor : TickSubscriber
{
   Pin& enable;
   Pin& direction;
   Pin& step;
   Encoder& encoder;

   Step_motor(Pin& enable, Pin& direction, Pin& step, Encoder& encoder) 
      : enable    {enable}
      , direction {direction}
      , step      {step}
      , encoder   {encoder}
   {}

public:

void notify() override {
   step ^= 1;
}

};