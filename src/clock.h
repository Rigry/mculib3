#pragma once

#include "periph_pwr.h"
#include "periph_rtc.h"

#if defined(USE_MOCK_RTC)
using RTC = mock::RTC;
#else
using RTC = mcu::RTC;
#endif

struct Date {
   uint16_t year;
   uint16_t month;
   uint16_t day;
   uint16_t date;
   uint16_t hour;
   uint16_t minute;
   uint16_t second;
};

class Clock 
{
   RTC& rtc;
   Date date_;

   Clock(RTC& rtc) : rtc{rtc}{}

   uint32_t time_mask (uint16_t hour, uint16_t minute, uint16_t second)
   {
      return ( (static_cast<uint32_t>(hour   / 10) << 20) 
             | (static_cast<uint32_t>(hour   % 10) << 16)
             | (static_cast<uint32_t>(minute / 10) << 12) 
             | (static_cast<uint32_t>(minute % 10) << 8)
             | (static_cast<uint32_t>(second / 10) << 4)
             | (static_cast<uint32_t>(second % 10)) );
   }

   uint32_t date_mask (uint16_t year, uint16_t day, uint16_t month, uint16_t date)
   {
      return ( (static_cast<uint32_t>(year  / 10) << 20)
             | (static_cast<uint32_t>(year  % 10) << 16)  
             | (static_cast<uint32_t>(day       ) << 13)
             | (static_cast<uint32_t>(month / 10) << 12)
             | (static_cast<uint32_t>(month % 10) << 8)
             | (static_cast<uint32_t>(date  / 10) << 4)
             | (static_cast<uint32_t>(date  % 10)) );
   }

public:
   static auto& make(uint16_t year, uint16_t month, uint16_t day, uint16_t date, uint16_t hour, uint16_t minute, uint16_t second)
   {
      static Clock clock {
         mcu::make_reference<mcu::Periph::RTC>()
      };

      clock.date_.year = year;
      clock.date_.month = month;
      clock.date_.day = day;
      clock.date_.date = date;
      clock.date_.hour = hour;
      clock.date_.minute = minute;
      clock.date_.second = second;

      auto &rcc = REF(RCC);
      rcc.on_LSI()
         .wait_LSI_ready();

      if (not rcc.clock_running()) {
      rcc.clock_enable<mcu::Periph::PWR>();
      auto &pwr = REF(PWR);
      pwr.RTC_protection();
      rcc.set(mcu::RCC::RTC_Clock::LSI);
      rcc.clock_enable<mcu::Periph::RTC>();

      clock.rtc.off_protection()
               .on_initialization()
               .wait_on_initialization()
               .prediv_a(111) 
               .prediv_s(346)
               .set_time(clock.time_mask(hour, minute, second))
               .set_date(clock.date_mask(year, day, month, date))
               .off_initialization()
               .on_protection();
      }
      return clock;

   }

   uint16_t hour  (){ return rtc.read_hour  (); }
   uint16_t minute(){ return rtc.read_minute(); }
   uint16_t second(){ return rtc.read_second(); }

   uint16_t year  (){ return rtc.read_year (); }
   uint16_t month (){ return rtc.read_month(); }
   uint16_t date  (){ return rtc.read_date (); }
   uint16_t day   (){ return rtc.read_day  (); }

   
};