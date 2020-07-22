#pragma once

#include <type_traits>
#include <cstddef>
#include <cstdint>

#if   defined(STM32F030x6) or defined(STM32F051x8)
   #define STM32F0
#elif defined(STM32F103xB)
   #define STM32F1
#elif defined(STM32F405xx)
   #define STM32F4
#elif defined(STM32F746xx)
   #define STM32F7
#else
   #define STM32F0
#endif

#if   defined(STM32F0)
   #include "stm32f0xx.h"
#elif defined(STM32F1)
   #include "stm32f1xx.h"
#elif defined(STM32F4)
   #include "stm32f4xx.h"
#elif defined(STM32F7)
   #include "stm32f7xx.h"
#endif

#undef RCC
#undef RTC
#undef PWR
#undef GPIOA
#undef GPIOB
#undef GPIOC
#undef GPIOD
#undef GPIOE
#undef GPIOF
#undef GPIOG
#undef GPIOH
#undef GPIOI
#undef GPIOJ
#undef GPIOK
#undef AFIO
#undef USART1
#undef USART2
#undef USART3
#undef UART4
#undef UART5
#undef USART6
#undef UART7
#undef UART8
#undef SysTick
#undef TIM1
#undef TIM2
#undef TIM3
#undef TIM4
#undef TIM5
#undef TIM8
#undef TIM14
#undef TIM16
#undef TIM17
#undef FLASH
#undef DMA1
#undef DMA2
#undef ADC
#undef ADC1
#undef ADC2
#undef ADC3
#undef SPI1
#undef SPI2
#undef SPI3
#undef EXTI
#undef SYSCFG


namespace mcu {

enum class Periph {
    RCC, RTC, PWR,
#if   defined(STM32F0)
    GPIOA, GPIOB, GPIOC, GPIOD, GPIOF,
#elif defined(STM32F1)
    GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, AFIO,
#elif defined(STM32F4) || defined(STM32F7)
    GPIOA, GPIOB, GPIOC, GPIOD, GPIOF, GPIOE, GPIOG, GPIOH, GPIOI,
#endif
#if   defined(STM32F0)
    USART1,
#elif defined(STM32F1)
    USART1, USART2, USART3,
#elif defined(STM32F4)
    USART1, USART2, USART3, USART4, USART5, USART6,
#elif defined(STM32F7)
    USART1, USART2, USART3, USART4, USART5, USART6, USART7, USART8, 
#endif
#if defined(STM32F0)
    DMA1, DMA1_stream1, DMA1_stream2, DMA1_stream3, DMA1_stream4, DMA1_stream5,
#elif defined(STM32F1)
    DMA1, DMA1_stream1, DMA1_stream2, DMA1_stream3, DMA1_stream4, DMA1_stream5, DMA1_stream6, DMA1_stream7,
#elif defined(STM32F4) or defined(STM32F7)
    DMA1, DMA2,
    DMA1_stream0, DMA1_stream1, DMA1_stream2, DMA1_stream3, DMA1_stream4, DMA1_stream5, DMA1_stream6, DMA1_stream7,
    DMA2_stream0, DMA2_stream1, DMA2_stream2, DMA2_stream3, DMA2_stream4, DMA2_stream5, DMA2_stream6, DMA2_stream7,
#endif
#if defined(STM32F0) or defined(STM32F1)
    ADC1,
#elif defined(STM32F4) or defined(STM32F7)
    ADC1, ADC2, ADC3,
#endif
    SysTick,
    TIM1, TIM2, TIM3, TIM4, TIM5, TIM14, TIM16, TIM17, TIM8,
    SPI1, SPI2, SPI3,
    FLASH,
    ADCC,
#if defined(STM32F7)
    USB_FS_core,
    USB_HS_core,
    USB_FS_device,
    USB_HS_device,
    USB_FS_IN,
    USB_HS_IN,
    USB_FS_OUT,
    USB_HS_OUT,
#endif

#if defined(STM32F0)
    EXTI, SYSCFG, 
#elif defined(STM32F1) or defined(STM32F4)
    EXTI,
#endif

#if defined(TEST)
    TEST_RCC, TEST_AFIO, TEST_DMA
#endif
};

template<class T>
auto& like_CMSIS (T& p)
{
   return *reinterpret_cast<typename T::CMSIS_type*>(&p);
}

template<class T>
volatile auto& registr (T& v)
{
   return *reinterpret_cast<volatile uint32_t*>(&v);
}

}

template<class T>
constexpr auto always_false_v = std::false_type::value;

#define SFINAE(periph,type) template <Periph p> std::enable_if_t<p == Periph::periph, type&>
#define REF(periph) mcu::make_reference<mcu::Periph::periph>()
#define WRAP(...) (__VA_ARGS__)









#if defined(TEST)
   #include <thread>
#endif

#if not defined(__IO)
   #define __IO volatile
#endif

#if defined(TEST)
    #define IF_TEST_WAIT_MS(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))
    #define VIRTUAL_TEST virtual
#else
    #define IF_TEST_WAIT_MS(ms)
    #define VIRTUAL_TEST
#endif
