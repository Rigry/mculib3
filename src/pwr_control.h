#pragma once

#include "periph_pwr.h"
#include "interrupt.h"

using PWR = mcu::PWR;

class Pwr_control
{
   PWR& pwr;

   Pwr_control(PWR& pwr) : pwr{pwr} {}

   Pwr_control &operator=(const Pwr_control &) = delete; 

public:

   template <PWR::Threshold threshold>
   static auto& make()
   {
      static Pwr_control pwr_control {
         mcu::make_reference<Periph::PWR>()
      };

      auto &rcc = REF(RCC);
      rcc.clock_enable<Periph::PWR>();

      pwr_control.pwr.set(threshold)
                 .enable();

      return pwr_control;
   }

   bool is_lower()
   {
      return pwr.is_lower();
   }

};
