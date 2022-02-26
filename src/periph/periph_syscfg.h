#pragma once

#include "periph.h"

#if defined(STM32F0)
   #include "syscfg_f0.h"
#elif defined(STM32F1)
   #error exti for STM32F1 not realized
#elif defined(STM32F4)
   #error exti for STM32F4 not realized
#elif defined(STM32F7)
   #error exti for STM32F7 not realized
#endif