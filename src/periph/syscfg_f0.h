#pragma once

#include "bits_syscfg_f0.h"

namespace mcu {

class SYSCFG {
   volatile uint32_t                    SYSCFG_CFGR1;     //SYSCFG configuration register 1
   volatile SYSCFG_bits::SYSCFG_EXTICR  SYSCFG_EXTICR[4]; //SYSCFG external interrupt configuration register 1, 2, 3, 4
   volatile uint32_t                    SYSCFG_CFGR2;     //SYSCFG configuration register 2

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
        if constexpr (std::is_same_v<Pin, PA0>) SYSCFG_EXTICR[0].EXTI0_4_8_12  = SYSCFG::Port::PA;
   else if constexpr (std::is_same_v<Pin, PA1>) SYSCFG_EXTICR[0].EXTI1_5_9_13  = SYSCFG::Port::PA;
   else if constexpr (std::is_same_v<Pin, PA2>) SYSCFG_EXTICR[0].EXTI2_6_10_14 = SYSCFG::Port::PA;
   else if constexpr (std::is_same_v<Pin, PA3>) SYSCFG_EXTICR[0].EXTI3_7_11_15 = SYSCFG::Port::PA;
   else if constexpr (std::is_same_v<Pin, PA4>) SYSCFG_EXTICR[1].EXTI0_4_8_12  = SYSCFG::Port::PA;
   else if constexpr (std::is_same_v<Pin, PA5>) SYSCFG_EXTICR[1].EXTI1_5_9_13  = SYSCFG::Port::PA;
   else if constexpr (std::is_same_v<Pin, PA6>) SYSCFG_EXTICR[1].EXTI2_6_10_14 = SYSCFG::Port::PA;
   else if constexpr (std::is_same_v<Pin, PA7>) SYSCFG_EXTICR[1].EXTI3_7_11_15 = SYSCFG::Port::PA;
   else if constexpr (std::is_same_v<Pin, PA8>) SYSCFG_EXTICR[2].EXTI0_4_8_12  = SYSCFG::Port::PA;
   else if constexpr (std::is_same_v<Pin, PA9>) SYSCFG_EXTICR[2].EXTI1_5_9_13  = SYSCFG::Port::PA;

   else if constexpr (std::is_same_v<Pin, PB0>) SYSCFG_EXTICR[0].EXTI0_4_8_12  = SYSCFG::Port::PB;
   else if constexpr (std::is_same_v<Pin, PB1>) SYSCFG_EXTICR[1].EXTI1_5_9_13  = SYSCFG::Port::PB;
   else if constexpr (std::is_same_v<Pin, PB2>) SYSCFG_EXTICR[0].EXTI2_6_10_14 = SYSCFG::Port::PB;
   else if constexpr (std::is_same_v<Pin, PB3>) SYSCFG_EXTICR[0].EXTI3_7_11_15 = SYSCFG::Port::PB;
   else if constexpr (std::is_same_v<Pin, PB4>) SYSCFG_EXTICR[1].EXTI0_4_8_12  = SYSCFG::Port::PB;
   else if constexpr (std::is_same_v<Pin, PB5>) SYSCFG_EXTICR[1].EXTI1_5_9_13  = SYSCFG::Port::PB;
   else if constexpr (std::is_same_v<Pin, PB6>) SYSCFG_EXTICR[1].EXTI2_6_10_14 = SYSCFG::Port::PB;
   else if constexpr (std::is_same_v<Pin, PB7>) SYSCFG_EXTICR[1].EXTI3_7_11_15 = SYSCFG::Port::PB;
   else if constexpr (std::is_same_v<Pin, PB8>) SYSCFG_EXTICR[2].EXTI0_4_8_12  = SYSCFG::Port::PB;
   else if constexpr (std::is_same_v<Pin, PB9>) SYSCFG_EXTICR[2].EXTI1_5_9_13  = SYSCFG::Port::PB;
   else static_assert ( always_false_v<Pin>, "допиши линии прерываний пинов");

   return *this;
}

} //namespace mcu

