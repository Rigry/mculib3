#pragma once

#include "bits_pwr_f0.h"

namespace mcu {

   class PWR {
      volatile PWR_bits::CR  CR;  // Power control register Address offset: 0x00
      volatile PWR_bits::CSR CSR; // Power control/status register Address offset: 0x04

   public:
      using CMSIS_type = PWR_TypeDef;

      auto& like_CMSIS() { return *reinterpret_cast<CMSIS_type*>(this); }

   };
} // namespace mcu