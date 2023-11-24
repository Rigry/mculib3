# pragma once

#include <tuple>
#include <array>
#include "pin.h"
#include "timers.h"

bool symbols [28][7] = {
                 //A  B  C  D  E  F  G  H
   {0, 0, 0, 0, 0, 0, 1}, // 0   ==A==
   {1, 0, 0, 1, 1, 1, 1}, // 1  ||   ||
   {0, 0, 1, 0, 0, 1, 0}, // 2  F|   |B
   {0, 0, 0, 0, 1, 1, 0}, // 3  ||   ||
   {1, 0, 0, 1, 1, 0, 0}, // 4   ==G==
   {0, 1, 0, 0, 1, 0, 0}, // 5  ||   ||
   {0, 1, 0, 0, 0, 0, 0}, // 6  E|   |C
   {0, 0, 0, 1, 1, 1, 1}, // 7  ||   ||
   {0, 0, 0, 0, 0, 0, 0}, // 8   ==D==  (H)
   {0, 0, 0, 0, 1, 0, 0}, // 9
   {1, 0, 0, 1, 0, 0, 0}, // 10 H
   {1, 1, 1, 0, 0, 0, 1}, // 11 L
   {1, 1, 1, 1, 1, 1, 1}, // 12 Space
   {1, 1, 1, 1, 1, 1, 0}, // 13 -
   {0, 1, 1, 0, 0, 0, 1}, // 14 C
   {1, 1, 1, 1, 1, 1, 1}, // 15 reserved
   {1, 1, 1, 1, 1, 1, 1}, // 16 reserved
   {1, 1, 1, 1, 1, 1, 1}, // 17 reserved
   {1, 1, 1, 1, 1, 1, 1}, // 18 reserved
   {1, 1, 1, 1, 1, 1, 1}, // 19 reserved
   {1, 1, 1, 1, 1, 1, 1}, // 20 reserved

   {1, 1, 1, 1, 1, 1, 0}, // 21 G
   {1, 0, 1, 1, 1, 1, 0}, // 22 B
   {0, 1, 1, 1, 1, 1, 0}, // 23 A
   {1, 1, 1, 1, 1, 0, 0}, // 24 F
   {1, 1, 1, 1, 0, 1, 0}, // 25 E
   {1, 1, 1, 0, 1, 1, 0}, // 26 D
   {1, 1, 0, 1, 1, 1, 0}, // 27 C
};

bool indicator [2][2] = {
   {0, 1},
   {1, 0}
};

enum class Sign {
   Level = 11, Space = 12, Hyphen = 13, Cover = 14
};
enum class Segment {
   G = 21, B = 22, A = 23, F = 24, E = 25, D = 26, C = 27, H = 28
};

template<class A, class B, class C, class D, class E, class F, class G, class H, class K1, class K2>
class SSI : TickSubscriber
{
private:
   
   uint8_t index {0};
   uint8_t tick {0};
   const uint8_t refresh;
   static constexpr uint8_t qty {2};
   
   std::array<Pin, 7> led {
      Pin::make<A, mcu::PinMode::Output>(),
      Pin::make<B, mcu::PinMode::Output>(),
      Pin::make<C, mcu::PinMode::Output>(),
      Pin::make<D, mcu::PinMode::Output>(),
      Pin::make<E, mcu::PinMode::Output>(),
      Pin::make<F, mcu::PinMode::Output>(),
      Pin::make<G, mcu::PinMode::Output>(),
   };
      // = make_pins<mcu::PinMode::Output, A, B, C, D, E, F, G>();

   Pin& h {Pin::make<H, mcu::PinMode::Output>()};

   std::array<Pin, qty> k {
      Pin::make<K1, mcu::PinMode::Output>(),
      Pin::make<K2, mcu::PinMode::Output>()
   };

   Timer timer;

public:

   volatile uint8_t buffer [qty] = {0};
   volatile bool    point  [qty] = {false};

   void notify()  override
   {
      if (++tick == refresh) {
         tick = 0;

         for (auto i = 0; i < 7; i++) {
            led[i] = symbols[buffer[index]][i];
         }


         // led = symbols[buffer[index]];
         h = (point[index]) ? false : true;
         for (auto i = 0; i < 2; i++) {
            k[i] = indicator[index][i];
         }
         // k = indicator[index];
         index++;
         index = (index == qty) ? 0 : index;
      }
   }

   SSI (uint8_t refresh = 3_ms)
        : refresh {refresh}
      
   { 
      tick_subscribe();
      dinamic_test();
      dinamic_test(); 
   }

   void test(Sign sign) {
      for(auto i = 0; i < qty; i++) {
         buffer[i] = static_cast<uint8_t>(sign);
         point[i]  = true;
      }
   }

   void dinamic_test() {
      
      for (auto i = 22; i <= 27; i++) {
         for (auto j = 0; j < qty; j++) {
            buffer[j] = i;
            point[j] ^= 1;
         }
         timer.start(70_ms);
         while (not timer.done()){}
         timer.stop();
      }
   }

};
