#pragma once

#include <cstdint>

namespace mcu::SYSCFG_bits {
   
   enum Port { PA = 0b0000, PB, PC, PD, PF = 0b0101 }; 

   struct SYSCFG_EXTICR1 {
      Port EXTI0   :4;
      Port EXTI1   :4;
      Port EXTI2   :4;
      Port EXTI3   :4;
      uint32_t     :16;
   }__attribute__((packed));

   struct SYSCFG_EXTICR2 {
      Port EXTI4   :4;
      Port EXTI5   :4;
      Port EXTI6   :4;
      Port EXTI7   :4;
      uint32_t     :16;
   }__attribute__((packed));

   struct SYSCFG_EXTICR3 {
      Port EXTI8   :4;
      Port EXTI9   :4;
      Port EXTI10  :4;
      Port EXTI11  :4;
      uint32_t     :16;
   }__attribute__((packed));

   struct SYSCFG_EXTICR4 {
      Port EXTI12  :4;
      Port EXTI13  :4;
      Port EXTI14  :4;
      Port EXTI15  :4;
      uint32_t     :16;
   }__attribute__((packed));

} //namespace mcu::SYSCFG_bits