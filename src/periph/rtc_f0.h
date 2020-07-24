#pragma once

#include "bits_rtc_f0.h"

namespace mcu {

   class RTC {
      volatile RTC_bits::TR   TR;         // RTC time register,                      Address offset: 0x00
      volatile RTC_bits::DR   DR;         // RTC date register,                      Address offset: 0x04
      volatile RTC_bits::CR   CR;         // RTC control register,                   Address offset: 0x08                                                                                            
      volatile RTC_bits::ISR  ISR;        // RTC initialization and status register, Address offset: 0x0C
      volatile RTC_bits::PRER PRER;       // RTC prescaler register,                 Address offset: 0x10
               uint32_t       RESERVED1;  // Reserved,                               Address offset: 0x14
               uint32_t       RESERVED2;  // Reserved,                               Address offset: 0x18
      volatile uint32_t       ALRMAR;     // RTC alarm A register,                   Address offset: 0x1C
               uint32_t       RESERVED3;  // Reserved,                               Address offset: 0x20
      volatile RTC_bits::WPR  WPR;        // RTC write protection register,          Address offset: 0x24
      volatile uint32_t       SSR;        // RTC sub second register,                Address offset: 0x28
      volatile uint32_t       SHIFTR;     // RTC shift control register,             Address offset: 0x2C
      volatile uint32_t       TSTR;       // RTC time stamp time register,           Address offset: 0x30
      volatile uint32_t       TSDR;       // RTC time stamp date register,           Address offset: 0x34
      volatile uint32_t       TSSSR;      // RTC time-stamp sub second register,     Address offset: 0x38
      volatile uint32_t       CALR;       // RTC calibration register,               Address offset: 0x3C
      volatile uint32_t       TAFCR;      // RTC tamper and alternate function,      Address offset: 0x40
      volatile uint32_t       ALRMASSR;   // RTC alarm A sub second register,        Address offset: 0x44

   public:

      using CMSIS_type    = RTC_TypeDef;
      using Hour_format   = RTC_bits::Hour_format;
      using Day           = RTC_bits::DR::Day;

      auto& like_CMSIS() { return *reinterpret_cast<CMSIS_type*>(this); } 

      RTC& off_protection         () {WPR.KEY = 0xCA; WPR.KEY = 0x53; return *this;}
      RTC& on_protection          () {WPR.KEY = 0xFE; WPR.KEY = 0x64; return *this;}
      RTC& on_initialization      () {ISR.INIT = true;                return *this;}
      RTC& off_initialization     () {ISR.INIT = false;               return *this;}
      RTC& wait_on_initialization () {while (not ISR.INITF) {}        return *this;}

      RTC& set (Hour_format format)   {CR.FMT = format;        return *this;}
      RTC& prediv_s (uint16_t prediv) {PRER.PREDIV_S = prediv; return *this;}
      RTC& prediv_a (uint16_t prediv) {PRER.PREDIV_A = prediv; return *this;}
   };

#if not defined(USE_MOCK_RTC)
template<Periph p> std::enable_if_t<p == Periph::RTC, RTC&> make_reference() { return *reinterpret_cast<RTC*>(RTC_BASE); }
#endif

}