#pragma once

#include "periph_pwr.h"
#include "periph_rtc.h"

#if defined(USE_MOCK_RTC)
using RTC = mock::RTC;
#else
using RTC = mcu::RTC;
#endif

template <class Date>
class Clock 
{
   RTC& rtc;
   Date& date_;

   Clock(RTC& rtc, Date& date_) : rtc{rtc}, date_{date_}{}

   uint32_t time_mask (Date date)
   {
      return ( (static_cast<uint32_t>(date.hour   / 10) << 20) 
             | (static_cast<uint32_t>(date.hour   % 10) << 16)
             | (static_cast<uint32_t>(date.minute / 10) << 12) 
             | (static_cast<uint32_t>(date.minute % 10) << 8)
             | (static_cast<uint32_t>(date.second / 10) << 4)
             | (static_cast<uint32_t>(date.second % 10)) );
   }

   uint32_t date_mask (Date date)
   {
      return ( (static_cast<uint32_t>(date.year  / 10) << 20)
             | (static_cast<uint32_t>(date.year  % 10) << 16)  
             | (static_cast<uint32_t>(date.day       ) << 13)
             | (static_cast<uint32_t>(date.month / 10) << 12)
             | (static_cast<uint32_t>(date.month % 10) << 8)
             | (static_cast<uint32_t>(date.date  / 10) << 4)
             | (static_cast<uint32_t>(date.date  % 10)) );
   }

public:
   
   static auto& make (Date& date)
   {
      static Clock clock {
         mcu::make_reference<mcu::Periph::RTC>(),
         date
      };

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
                  .set_time(clock.time_mask(date))
                  .set_date(clock.date_mask(date))
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

   void set_time(Date& date) 
   {
      rtc.off_protection()
         .on_initialization()
         .wait_on_initialization()
         .set_time(time_mask(date))
         .set_date(date_mask(date))
         .off_initialization()
         .on_protection();
   }

   void save_date()
   {
      date_.hour   = rtc.read_hour  ();
      date_.minute = rtc.read_minute();
      date_.day    = rtc.read_day   ();      
      date_.date   = rtc.read_date  (); 
      date_.month  = rtc.read_month ();
      date_.year   = rtc.read_year  ();  
   }

   Date date_struct(){save_date(); return date_;}

};