#pragma once

#include "periph.h"

#if defined(STM32F0)
   #include "pwr_f0.h"
#elif defined(STM32F4)
   #include "pwr_f4.h"
#endif