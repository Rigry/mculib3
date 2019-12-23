#pragma once

#include "timers.h"
#include "interrupt.h"
#include "external.h"
#include "limited_int.h"
#include <bitset>
#include <ios>

class Wiegan
{
   External& external_D0;
   External& external_D1;
   Interrupt& interrupt_D0;
   Interrupt& interrupt_D1;

   uint32_t card_number{0};
   uint32_t card_temp_high{0};
   uint32_t card_temp_low{0};
   uint8_t index{0};
   bool get{false};

   using Parent = Wiegan;

   void D0_Interrupt()
   {
      if (external_D0.is_event()) {
         index++;
         if (index>31)		
	      {
		      card_temp_high |= ((0x80000000 & card_temp_low)>>31);	
		      card_temp_high <<= 1;
		      card_temp_low <<=1;
	      } else
            card_temp_low <<= 1;
      }
   }

   void D1_Interrupt()
   {
      if (external_D1.is_event()) {
         index++;
         if (index>31)
	      {
		      card_temp_high |= ((0x80000000 & card_temp_low)>>31);	
		      card_temp_high <<= 1;
            card_temp_low |= 1;
		      card_temp_low <<=1;
	      } else {
            card_temp_low |= 1;
            card_temp_low <<= 1;
         }
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
      if (not get) {
         if (index == 26) {							// EM tag
	         card_number = (card_temp_low & 0x1FFFFFE) >>1;
         } else if (index == 34) {								// Mifare 
	      	card_temp_high = card_temp_high & 0x03;	// only need the 2 LSB of the codehigh
	      	card_temp_high <<= 30;							// shift 2 LSB to MSB		
	      	card_temp_low >>=1;
	      	card_number = (card_temp_high | card_temp_low);
	      } else {
            index = 0;
            card_temp_high = 0;
            card_temp_low = 0;
            card_number = 0;
         }
         get = true;
      }
								
      return card_number; // EM tag or Mifare without parity bits
   }

   uint32_t get_high_bits()
   {
      return card_number >> 16;
   }

   uint32_t get_low_bits()
   {
      return static_cast<uint16_t>(card_number);
   }

   void reset_number()
   {
      get = false;
      index = 0;
      card_temp_high = 0;
      card_temp_low = 0;
      card_number = 0;
   }
};