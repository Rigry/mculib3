#pragma once

#include "timers.h"
#include "interrupt.h"
#include "external.h"
#include "timers.h"
#include <bitset>
#include <array>

class Wiegan : TickSubscriber
{
   External& external_D0;
   External& external_D1;
   Interrupt& interrupt_D0;
   Interrupt& interrupt_D1;

   int time {0}; // выдержка времени для протокола

   bool enable_{true};

   uint32_t card_number{0};
   uint8_t index{0};
   std::array<uint16_t, 40> numder;

   using Parent = Wiegan;

   void D0_Interrupt()
   {
      if (external_D0.is_event() and enable_) {
         tick_subscribe();
         numder[index++] = 0;
         index %= 40;
         time = 0;
      }
   }

   void D1_Interrupt()
   {
      if (external_D1.is_event() and enable_) {
         tick_subscribe();
         numder[index++] = 1;
         index %= 40;
         time = 0;
      }
   }

   void notify() override 
   {
      time++;
      if (time > 250) {
         time = 0;
         tick_unsubscribe();
         get_number();
      }
   }

   struct D0_interrupt : Interrupting
   {
      Parent& parent;
      D0_interrupt (Parent& parent) : parent(parent) {
          parent.interrupt_D0.subscribe (this);
      }
      void interrupt() override {parent.D0_Interrupt();} 
   } D0_ {*this};

   struct D1_interrupt : Interrupting
   {
      Parent& parent;
      D1_interrupt (Parent& parent) : parent(parent) {
          parent.interrupt_D1.subscribe (this);
      }
      void interrupt() override {parent.D1_Interrupt();} 
   } D1_ {*this};

   Wiegan (
        External& external_D0
       ,External& external_D1
       ,Interrupt& interrupt_D0
       ,Interrupt& interrupt_D1
   ) :  external_D0 {external_D0}
     ,  external_D1 {external_D1}
     ,  interrupt_D0 {interrupt_D0}
     ,  interrupt_D1 {interrupt_D1}
   {}

public:

   template <class D0, class D1> 
   static auto& make ()
   {
      static Wiegan wiegan {
         External::make<D0>(),
         External::make<D1>(),
         get_external_interrupt(D0::n),
         get_external_interrupt(D1::n)
      };

      return wiegan;
   }

   void get_number()
   {
      card_number = 0;
      if (index == 26 or index == 34) {
         for (auto i = 1; i < (index - 1); i++) {
            card_number |= (numder[i] << ((index - 2) - i));
         }
      }
      index = 0;
   }

   uint16_t get_high_bits()
   {
      return card_number >> 16;
   }

   uint16_t get_low_bits()
   {
      return static_cast<uint16_t>(card_number);
   }

   void reset_number()
   {
      index = 0;
      card_number = 0;
   }

   bool new_card()
   {
      return (card_number > 0);
   }

   void enable(bool v = true)
   {
      enable_ = v;
   }
};