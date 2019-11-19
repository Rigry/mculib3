#pragma once

#include "bits_syscfg_f0.h"

namespace mcu {

class SYSCFG {
   volatile uint32_t                    SYSCFG_CFGR1;   //SYSCFG configuration register 1
   volatile SYSCFG_bits::SYSCFG_EXTICR1 SYSCFG_EXTICR1; //SYSCFG external interrupt configuration register 1
   volatile SYSCFG_bits::SYSCFG_EXTICR2 SYSCFG_EXTICR2; //SYSCFG external interrupt configuration register 2
   volatile SYSCFG_bits::SYSCFG_EXTICR3 SYSCFG_EXTICR3; //SYSCFG external interrupt configuration register 3
   volatile SYSCFG_bits::SYSCFG_EXTICR4 SYSCFG_EXTICR4; //SYSCFG external interrupt configuration register 4
   volatile uint32_t                    SYSCFG_CFGR2;   //SYSCFG configuration register 2

public:
   using CMSIS_type = SYSCFG_TypeDef;
   using Port       = SYSCFG_bits::Port;

   auto &like_CMSIS() { return *reinterpret_cast<CMSIS_type *>(this); }

   template <class Pin>
   SYSCFG& set_line_interrupt();

};

#if not defined(USE_MOCK_SYSCFG)
SFINAE(SYSCFG,SYSCFG) make_reference() {return *reinterpret_cast<SYSCFG*>(SYSCFG_BASE);}
#endif

template <class Pin>
SYSCFG& SYSCFG::set_line_interrupt()
{
        if constexpr (std::is_same_v<Pin, PA0>) SYSCFG_EXTICR1.EXTI0 = SYSCFG::Port::PA;
   else if constexpr (std::is_same_v<Pin, PA1>) SYSCFG_EXTICR1.EXTI1 = SYSCFG::Port::PA;
   else if constexpr (std::is_same_v<Pin, PA2>) SYSCFG_EXTICR1.EXTI2 = SYSCFG::Port::PA;
   else if constexpr (std::is_same_v<Pin, PA3>) SYSCFG_EXTICR1.EXTI3 = SYSCFG::Port::PA;
   else if constexpr (std::is_same_v<Pin, PA4>) SYSCFG_EXTICR2.EXTI4 = SYSCFG::Port::PA;
   else if constexpr (std::is_same_v<Pin, PA5>) SYSCFG_EXTICR2.EXTI5 = SYSCFG::Port::PA;
   else if constexpr (std::is_same_v<Pin, PA6>) SYSCFG_EXTICR2.EXTI6 = SYSCFG::Port::PA;
   else if constexpr (std::is_same_v<Pin, PA7>) SYSCFG_EXTICR2.EXTI7 = SYSCFG::Port::PA;
   else if constexpr (std::is_same_v<Pin, PB1>) SYSCFG_EXTICR1.EXTI1 = SYSCFG::Port::PB;
   else if constexpr (std::is_same_v<Pin, PB5>) SYSCFG_EXTICR2.EXTI5 = SYSCFG::Port::PB;
   else static_assert ( always_false_v<Pin>, "допиши линии прерываний пинов");

   return *this;
}

} //namespace mcu

