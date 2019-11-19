#pragma once

#include "timers.h"
#include "interrupt.h"
#include "external.h"
#include "limited_int.h"
#include <bitset>

class Wiegan
{
   External& external_D0;
   External& external_D1;
   Interrupt& interrupt_D0;
   Interrupt& interrupt_D1;

   uint32_t card_number{0};
   uint8_t index{0};

   using Parent = Wiegan;

   void D0_Interrupt()
   {
      if (external_D0.is_event()) {
         index++;
         card_number <<= 1;
      }
   }

   void D1_Interrupt()
   {
      if (external_D1.is_event()) {
         index++;
         card_number |= 1;
         card_number <<= 1;
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

   uint32_t get_number()
   {
      return card_number;
   }

   void reset_number()
   {
      
   }
};