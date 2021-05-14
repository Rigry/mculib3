# pragma once

#include <tuple>
#include "pin.h"
#include "timers.h"

static std::tuple<int, int, int, int, int, int, int> symbols [] = {
                 //A  B  C  D  E  F  G  H
   std::make_tuple(0, 0, 0, 0, 0, 0, 1), // 0   ==A==
   std::make_tuple(1, 0, 0, 1, 1, 1, 1), // 1  ||   ||
   std::make_tuple(0, 0, 1, 0, 0, 1, 0), // 2  F|   |B
   std::make_tuple(0, 0, 0, 0, 1, 1, 0), // 3  ||   ||
   std::make_tuple(1, 0, 0, 1, 1, 0, 0), // 4   ==G==
   std::make_tuple(0, 1, 0, 0, 1, 0, 0), // 5  ||   ||
   std::make_tuple(0, 1, 0, 0, 0, 0, 0), // 6  E|   |C
   std::make_tuple(0, 0, 0, 1, 1, 1, 1), // 7  ||   ||
   std::make_tuple(0, 0, 0, 0, 0, 0, 0), // 8   ==D==  (H)
   std::make_tuple(0, 0, 0, 0, 1, 0, 0), // 9
   std::make_tuple(1, 0, 0, 1, 0, 0, 0), // 10 H
   std::make_tuple(1, 1, 1, 0, 0, 0, 1), // 11 L
   std::make_tuple(1, 1, 1, 1, 1, 1, 1), // 12 Space
   std::make_tuple(1, 1, 1, 1, 1, 1, 0)  // 13 -
};

static std::tuple<int, int, int, int, int, int> indicator [] = {
   std::make_tuple(0, 0, 0, 0, 0, 1),
   std::make_tuple(0, 0, 0, 0, 1, 0),
   std::make_tuple(0, 0, 0, 1, 0, 0), 
   std::make_tuple(0, 0, 1, 0, 0, 0), 
   std::make_tuple(0, 1, 0, 0, 0, 0),
   std::make_tuple(1, 0, 0, 0, 0, 0)
};

enum class Sign {
   Level = 11, Space = 12, Hyphen = 13, E = 14
};

template<class A, class B, class C, class D, class E, class F, class G, class H, class ... K>
class SSI : TickSubscriber
{
private:
   
   uint8_t index {0};
   uint8_t tick {0};
   const uint8_t refresh;
   static constexpr uint8_t qty {sizeof...(K)};
   
   std::tuple<Pin, Pin, Pin, Pin, Pin, Pin, Pin> led 
      = make_pins<mcu::PinMode::Output, A, B, C, D, E, F, G>();

   Pin& h {Pin::make<H, mcu::PinMode::Output>()};

   std::tuple<Pin, Pin, Pin, Pin, Pin, Pin> k = make_pins<mcu::PinMode::Output, K...>();

public:

   volatile uint8_t buffer [qty] = {0};
   volatile bool    point  [qty] = {false};

   void notify() override 
   {
      if (++tick == refresh) {
         tick = 0;
         led = symbols[buffer[index]];
         h = (point[index]) ? false : true;
         k = indicator[index];
         index++;
         index = (index == qty) ? 0 : index;
      }
   }

   SSI (uint8_t refresh = 2_ms)
        : refresh {refresh}
      
   { tick_subscribe(); }

};
