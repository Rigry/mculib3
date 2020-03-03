// #include "encoder_rotary.h"
#include "pins.h"
#include "button.h"

template <class Enable, class Dir, class Step, class Right_sense, class Left_sense>
class Step_motor : TickSubscriber
{
   enum State_search    {Wait, Work, Left_, Right_slow, Right_fast, Left_slow, Left_fast,  Right_} state_search{State_search::Wait};
   enum Speed           {Slow = 10, Fast = 1}  speed{Speed::Slow};
   enum Direction       {Left = 0, Right = 1} direction{Direction::Left};
   
   Pin& enable {Pin::make<Enable,      mcu::PinMode::Output>()};
   Pin& dir    {Pin::make<Dir,         mcu::PinMode::Output>()};
   Pin& step   {Pin::make<Step,        mcu::PinMode::Output>()};
   Pin& right  {Pin::make<Right_sense, mcu::PinMode::Input> ()};
   Pin& left   {Pin::make<Left_sense,  mcu::PinMode::Input> ()};

   bool finish{false};
   bool stepping{false};
   uint16_t qty_step{0};
   uint16_t coordinate{0};
   uint16_t max_coordinate{0};
   size_t cnt{0};
   size_t tick{1};

public:

   Step_motor(){}

   void operator()()
   {
      switch (state_search)
      {
         case Wait:
         
            state_search = State_search::Left_;
            ahead(Direction::Left, Speed::Fast);
            
         break;
         case Left_:
            if(left) {
               stop();
               coordinate = 0;
               ahead(Direction::Right, Speed::Fast);
               state_search = State_search::Right_;
               // state_search = State_search::Right_fast;
               // reach(200, Direction::Right, Speed::Fast);
            }
         break;
         // case Right_fast:
         //    if(finish) {
         //       stop();
         //       ahead(Direction::Left, Speed::Slow);
         //       state_search = State_search::Left_slow;
         //    }
         // break;
         // case Left_slow:
         //    if(left) {
         //       stop();
         //       coordinate = 0;
         //       ahead(Direction::Right, Speed::Fast);
         //       state_search = State_search::Right_;
         //    }
         // break;
         case Right_:
            if(right) {
               stop();
               max_coordinate = coordinate;
               ahead(Direction::Left, Speed::Fast);
               state_search = State_search::Left_;
               // state_search = State_search::Left_fast;
               // reach(200, Direction::Left, Speed::Fast);
            }
         break;
         // case Left_fast:
         //    if(finish) {
         //       stop();
         //       ahead(Direction::Right, Speed::Slow);
         //       state_search = State_search::Right_slow;
         //    }
         // break;
         // case Right_slow:
         //    if(right) {
         //       stop();
         //       max_coordinate = coordinate;
         //       ahead(Direction::Left, Speed::Fast);
         //       state_search = State_search::Left_;
         //    }
         // break;
         case Work:
            emergency_stop();
         break;
      
      }
   }
   
   void reach (uint16_t qty_step, Direction d, Speed s)
   {
      finish = false;
      this->qty_step = qty_step * 2;
      dir  = d;
      tick = s;
      stepping = true;
      tick_subscribe();
   }

   void ahead(Direction d, Speed s)
   {
      stepping = false;
      finish = false;
      dir  = d;
      tick = s;
      tick_subscribe();
   }

   void stop()
   {
      step = false;
      finish = false;
      tick_unsubscribe();
   }

   void emergency_stop()
   {
      if (left) {
         coordinate = 0;
         stop();
      } else if (right) {
         coordinate = max_coordinate;
         stop();
      }
   }

   void notify() override 
   {
      if (not(cnt++ % tick)){
         step ^= 1;
         if(step)
            coordinate += dir == Direction::Right ? 1 : -1;
         if ((not qty_step--) and stepping) {
            tick_unsubscribe();
            step = false;
            finish = true;
         }
      }
   }  

};