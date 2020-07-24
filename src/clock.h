#pragma once

#include "periph_pwr.h"
#include "periph_rtc.h"

#if defined(USE_MOCK_RTC)
using RTC = mock::RTC;
#else
using RTC = mcu::RTC;
#endif

class Clock 
{
   RTC& rtc;

   Clock(RTC& rtc) : rtc{rtc}{}

public:
   static auto& make()
   {
      static Clock clock {
         mcu::make_reference<mcu::Periph::RTC>()
      };

      auto &rcc = REF(RCC);
      rcc.on_LSI()
         .wait_LSI_ready();

      rcc.clock_enable<mcu::Periph::PWR>();
      auto &pwr = REF(PWR);
      pwr.RTC_protection();

      rcc.set(mcu::RCC::RTC_Clock::LSI)
         .clock_enable<mcu::Periph::RTC>();

      // rcc.backup_domain_reset(true)
      //    .backup_domain_reset(false);
      

      // clock.rtc.set(RTC::Hour_format::_24);
      clock.rtc.off_protection()
               .on_initialization()
               .wait_on_initialization()
               .prediv_a(99) 
               .prediv_s(399)
               .off_initialization()
               .on_protection();
      return clock;

   }
};