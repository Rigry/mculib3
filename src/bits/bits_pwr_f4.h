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
      bool      DBP  :1; // Bit 8 DBP: Disable RTC domain write protection
      bool      FPDS :1; // Bit 9 FPDS: Flash power-down in Stop mode
      uint32_t       :4; // Bits 13:10 Reserved, must be kept at reset value
      bool      VOS  :1; // Bit 14 VOS: Regulator voltage scaling output selection
      uint32_t       :17; // Bits 31:15 Reserved, must be kept at reset value
   }__attribute__((packed));

   struct CSR {
      bool WUF    :1; // Bit 0 WUF: Wakeup flag
      bool SBF    :1; // Bit 1 SBF: Standby flag
      bool PVDO   :1; // Bit 2 PVDO: PVD output
      bool BRR    :1; // Bit 3 BRR: Backup regulator ready
      uint32_t    :4; // Bits 7:4 Reserved, must be kept at reset value
      bool EWUP1  :1; // Enable WKUP pin
      bool BRE    :1; // Bit 9 BRE: Backup regulator enable
      uint32_t    :4; // Bits 13:10 Reserved, must be kept at reset value
      bool VOSRDY :1; // Bit 14 VOSRDY: Regulator voltage scaling output selection ready bit
      uint32_t    :17; // Bits 31:15 Reserved, must be kept at reset value.
   }__attribute__((packed));

} //namespace mcu::PWR_bits