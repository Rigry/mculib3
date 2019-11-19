#pragma once

#include <cstdint>

namespace mcu::EXTI_bits {

   struct R1 {
      bool  _0 :1;
      bool  _1 :1;
      bool  _2 :1;
      bool  _3 :1;
      bool  _4 :1;
      bool  _5 :1;
      bool  _6 :1;
      bool  _7 :1;
      bool  _8 :1;
      bool  _9 :1;
      bool _10 :1;
      bool _11 :1;
      bool _12 :1;
      bool _13 :1;
      bool _14 :1;
      bool _15 :1;
      bool _16 :1;
      bool _17 :1;
      bool _18 :1;
      bool _19 :1;
      bool _20 :1;
      bool _21 :1;
      bool _22 :1;
      bool _23 :1;
      bool _24 :1;
      bool _25 :1;
      bool _26 :1;
      bool _27 :1;
      bool _28 :1;
      bool _29 :1;
      bool _30 :1;
      bool _31 :1;
   }__attribute__((packed));

   struct R2 {
      bool  _0 :1;
      bool  _1 :1;
      bool  _2 :1;
      bool  _3 :1;
      bool  _4 :1;
      bool  _5 :1;
      bool  _6 :1;
      bool  _7 :1;
      bool  _8 :1;
      bool  _9 :1;
      bool _10 :1;
      bool _11 :1;
      bool _12 :1;
      bool _13 :1;
      bool _14 :1;
      bool _15 :1;
      bool _16 :1;
      bool _17 :1;
      uint32_t :1; // Bit 18 Reserved, must be kept at reset value (0).
      bool _19 :1;
      bool _20 :1;
      bool _21 :1;
      bool _22 :1;
      uint32_t :8; // Bits 30:23 Reserved, must be kept at reset value (0).
      bool _31 :1;
   }__attribute__((packed));
}//namespace mcu::EXTI_bits