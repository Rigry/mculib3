#pragma once

#include "periph.h"

#if defined(STM32F0)
   #include "exti_f0.h"
#elif defined(STM32F1)
   #include "exti_f1.h"
#elif defined(STM32F4)
   #include "exti_f4.h"
#elif defined(STM32F7)
   #error exti for STM32F7 not realized
#endif
