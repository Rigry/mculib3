#pragma once

#include <array>
#include <type_traits>
#include "periph_syscfg.h"
#include "periph_exti.h"
#include "pin.h"
#include "net_buffer.h"
#include "interrupt.h"


using SYSCFG = mcu::SYSCFG;
using EXTI   = mcu::EXTI;


class External
{
   using Edge = mcu::EXTI::Edge;
   using Periph = mcu::Periph;
protected:
   Pin& external_pin;
   SYSCFG& syscfg;
   EXTI& exti;
   const int n;

   External (
      Pin& external_pin, SYSCFG& syscfg, EXTI& exti, int n
   ) : external_pin{external_pin}, syscfg{syscfg}, exti{exti}, n{n}
   {}

   External &operator=(const External &) = delete;
public:

   template <class Pin_, Edge edge = Edge::falling> 
   static auto& make ()
   {
      static External external {
         Pin::make<Pin_, mcu::PinMode::Input>(), 
         mcu::make_reference<Periph::SYSCFG>(), 
         mcu::make_reference<Periph::EXTI>(),
         Pin_::n
      };
      
      auto &rcc = REF(RCC);
      external.exti.enable_interrupt(Pin_::n);
      external.exti.set_trigger(edge, Pin_::n);
      get_external_interrupt(Pin_::n).enable();

      rcc.clock_enable<Periph::SYSCFG>();

      return external;
   }

   bool is_event()
   {
      return exti.is_interrupt(n);
   }

};