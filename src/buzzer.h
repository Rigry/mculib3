#include "pwm_.h"

class Buzzer : TickSubscriber
{
   PWM& pwm;

   uint16_t qty{0};
   size_t cnt{0};
   size_t tick{1};

public:

   Buzzer(PWM& pwm) : pwm {pwm} {
      pwm.duty_cycle = 490;
      pwm.frequency = 1047;
   }
   
   void brief ()
   {
      tick = 50;
      tick_subscribe();
   }

   void longer ()
   {
      tick = 500;
      tick_subscribe();
   }

   void notify() override 
   {
      if (not(cnt++ % tick)){
         pwm.out_enable();
      }
      if (cnt == tick) {
         cnt = 0;
         pwm.out_disable();
         tick_unsubscribe();
      }
   }  

};