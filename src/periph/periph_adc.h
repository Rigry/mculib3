#pragma once

#include "periph.h"

#if defined(STM32F0)
   #include "adc_f0.h"
#elif defined(STM32F1)
   #error adc for STM32F1 not realized
#elif defined(STM32F4)
   #include "adc_f4.h"
   #include "adcc_f4.h"
#elif defined(STM32F7)
   #error adc for STM32F7 not realized
#endif
