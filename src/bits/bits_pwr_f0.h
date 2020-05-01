#pragma once

#include <cstdint>

namespace mcu::PWR_bits {
   
   struct CR {
      enum Threshold {_2_0V = 0b000, _2_1V, _2_3V, _2_5V, _2_6V, _2_7V, _2_8V, _2_9V};
      bool      LPDS :1; // Bit 0 LPDS: Low-power deepsleep
      bool      PDDS :1; // Bit 1 PDDS: Power down deepsleep
      bool      CWUF :1; // Bit 2 CWUF: Clear wakeup flag
      bool      CSBF :1; // Bit 3 CSBF: Clear standby flag
      bool      PVDE :1; // Bit 4 PVDE: Power voltage detector enable
      Threshold PLS  :3; // Bits 7:5 PLS[2:0]: PVD level selection
      bool DBP  :1; // Bit 8 DBP: Disable RTC domain write protection
      uint32_t  :23; // Bits 31:9 Reserved, must be kept at reset value.
   }__attribute__((packed));

   struct CSR {
      bool WUF   :1; // Bit 0 WUF: Wakeup flag
      bool SBF   :1; // Bit 1 SBF: Standby flag
      bool PVDO  :1; // Bit 2 PVDO: PVD output
      bool VIN   :1; // Bit 3 VREFINTRDY: VREFINT reference voltage ready
      uint32_t   :4; // Bits 7:4 Reserved, must be kept at reset value
      bool EWUP1 :1; // Enable WKUPx pin
      bool EWUP2 :1; // Enable WKUPx pin
      uint32_t   :1; // Bit 10 Reserved, must be kept at reset value.
      bool EWUP4 :1; // Enable WKUPx pin
      bool EWUP5 :1; // Enable WKUPx pin (available only on STM32F070xB and STM32F030xC devices)
      bool EWUP6 :1; // Enable WKUPx pin (available only on STM32F070xB and STM32F030xC devices)
      bool EWUP7 :1; // Enable WKUPx pin (available only on STM32F070xB and STM32F030xC devices)
      bool EWUP8 :1; // EWUPx: Enable WKUPx pin
      uint32_t   :16; // Bits 31:16 Reserved, must be kept at reset value.
   }__attribute__((packed));

} //namespace mcu::PWR_bits