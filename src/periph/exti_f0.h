#pragma once

#include "bits_exti_f0.h"
#include <bitset>

namespace mcu {

class EXTI
{
    volatile EXTI_bits::R1    IMR;     // Interrupt mask register            offset: 0x00
    volatile EXTI_bits::R1    EMR;     // Event mask register                offset: 0x04
    volatile EXTI_bits::R2    RTSR;    // Rising trigger selection register  offset: 0x08
    volatile EXTI_bits::R2    FTSR;    // Falling trigger selection register offset: 0x0C
    volatile EXTI_bits::R2    SWIER;   // Software interrupt event register  offset: 0x10
    volatile EXTI_bits::R2    PR;      // Pending register                   offset: 0x14
public:
   using CMSIS_type  = EXTI_TypeDef;
   enum Edge {rising, falling, both};

   EXTI& set_trigger    (Edge, int n, bool v = true);
   EXTI& toggle_trigger (Edge, int n);
   EXTI& enable_interrupt (int n) { registr(IMR) |= 1 << n; return *this; }
   bool is_interrupt      (int n) { return registr(PR) & 1 << n; } 
   template<int first, int last = 0>
   EXTI& clear_interrupt_flags();

   EXTI& clear_interrupt_flags(int first);

};

#if not defined(USE_MOCK_EXTI)
SFINAE(EXTI,EXTI) make_reference() {return *reinterpret_cast<EXTI*>(EXTI_BASE);}
#endif

EXTI& EXTI::set_trigger(Edge edge, int n, bool v)
{
    auto mask = 1 << n;
    if (edge == rising or edge == both) {
        if (v)
            registr(RTSR) |= mask;
        else
            registr(RTSR) &= ~mask;
    }

    if (edge == falling or edge == both) {
        if (v)
            registr(FTSR) |= mask;
        else
            registr(FTSR) &= ~mask;
    }
    
    return *this;
}


EXTI& EXTI::toggle_trigger(Edge edge, int n)
{
    auto mask = 1 << n;
    if (edge == rising or edge == both)
        registr(RTSR) ^= mask;

    if (edge == falling or edge == both)
        registr(FTSR) ^= mask;
    
    return *this;
}

template<int first, int last = 0>
EXTI& EXTI::clear_interrupt_flags()
{
    static_assert (
        first >= 0 and first <= 15 and last >=0 and last <= 15,
        "first and last ust be from 0 to 15"
    );

    std::bitset<32> mask;
    mask[first] = true;
    if (last > first)
        for (auto i = first + 1; i <= last; i++)
            mask[i] = true;
 
    registr(PR) = mask.to_ulong();

    // if      (first == 0) PR._0 = true;
    // else if (first == 1) PR._1 = true;
    // else if (first == 2) PR._2 = true;
    // else if (first == 3) PR._3 = true;
    // else if (first == 4) PR._4 = true;
    // else if (first == 5) PR._5 = true;
    // else if (first == 6) PR._6 = false;
    // else if (first == 7) PR._7 = true;

    return *this;
}


EXTI& EXTI::clear_interrupt_flags(int first)
{
    
    if      (first == 0) registr(PR) = EXTI_PR_PR0_Msk;
    else if (first == 1) registr(PR) = EXTI_PR_PR1_Msk;
    else if (first == 2) registr(PR) = EXTI_PR_PR2_Msk;
    else if (first == 3) registr(PR) = EXTI_PR_PR3_Msk;
    else if (first == 4) registr(PR) = EXTI_PR_PR4_Msk;
    else if (first == 5) registr(PR) = EXTI_PR_PR5_Msk;
    else if (first == 6) registr(PR) = EXTI_PR_PR6_Msk;
    else if (first == 7) registr(PR) = EXTI_PR_PR7_Msk;

    return *this;
}

} //namespace mcu