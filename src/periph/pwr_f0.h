#pragma once

#include "bits_pwr_f0.h"

namespace mcu {

class PWR {
   volatile PWR_bits::CR  CR;  // Power control register Address offset: 0x00
   volatile PWR_bits::CSR CSR; // Power control/status register Address offset: 0x04

public:
   using CMSIS_type = PWR_TypeDef;
   using Threshold  = PWR_bits::CR::Threshold;

   auto& like_CMSIS() { return *reinterpret_cast<CMSIS_type*>(this); }

   PWR& enable()         {CR.PVDE = true; return *this;}
   PWR& set(Threshold v) {CR.PLS = v;     return *this;}
   bool is_lower()       {return CSR.PVDO;}

};

#if not defined(USE_MOCK_PWR)
SFINAE(PWR, PWR) make_reference() { return *reinterpret_cast<PWR*>(PWR_BASE);  }
#endif
} // namespace mcu