#pragma once

#include <cstdint>

namespace mcu::SYSCFG_bits {
   
   enum Port { PA = 0b0000, PB = 0b001, PC = 0b010, PD = 0b011, PF = 0b0101 }; 

   struct SYSCFG_EXTICR {
      uint32_t EXTI0_4_8_12   :4;
      uint32_t EXTI1_5_9_13   :4;
      uint32_t EXTI2_6_10_14  :4;
      uint32_t EXTI3_7_11_15  :4;
      uint32_t                :16;
   }__attribute__((packed));

} //namespace mcu::SYSCFG_bits