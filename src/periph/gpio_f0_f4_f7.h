#pragma once

#include "bits_gpio_f0_f4_f7.h"

namespace mcu
{

enum class PinMode
{
   Input,
   Output,
   Analog,
   Alternate_0,
   Alternate_1,
   Alternate_2,
   Alternate_3,
   Alternate_4,
   Alternate_5,
   Alternate_6,
   Alternate_7,
#if defined(STM32F4) or defined(STM32F7)
   Alternate_8,
   Alternate_9,
   Alternate_10,
   Alternate_11,
   Alternate_12,
   Alternate_13,
   Alternate_14,
   Alternate_15,
#endif
#if defined(STM32F4)
   USART1_TX = Alternate_7,
   USART1_RX = Alternate_7,
   USART2_TX = Alternate_7,
   USART2_RX = Alternate_7,
   USART3_TX = Alternate_7,
   USART3_RX = Alternate_7,
   USART4_TX = Alternate_8,
   USART4_RX = Alternate_8,
   USART5_TX = Alternate_8,
   USART5_RX = Alternate_8,
   USART6_TX = Alternate_8,
   USART6_RX = Alternate_8,
#endif
};

enum class PushPull
{
   No, Up, Down
};

class GPIO
{
   volatile GPIO_bits::MODER MODER;     // mode register,                offset: 0x00
   volatile GPIO_bits::OTYPER OTYPER;   // output type register,         offset: 0x04
   volatile GPIO_bits::OSPEEDR OSPEEDR; // output speed register,        offset: 0x08
   volatile GPIO_bits::PUPDR PUPDR;     // pull-up/pull-down register,   offset: 0x0C
   volatile GPIO_bits::DR IDR;          // input data register,          offset: 0x10
   volatile GPIO_bits::DR ODR;          // output data register,         offset: 0x14
   volatile uint32_t BSRR;              // bit set/reset register,       offset: 0x18
   volatile uint32_t LCKR;              // configuration lock register,  offset: 0x1C
   volatile GPIO_bits::AFR AFR;         // alternate function registers, offset: 0x20-0x24
#if defined(STM32F0)
   volatile uint32_t BRR; // bit reset register,           offset: 0x28
#endif

public:
   using CMSIS_type = GPIO_TypeDef;
   using PullResistor = GPIO_bits::PUPDR::PullResistor;
   using Mode = GPIO_bits::MODER::Mode;
   using AF = GPIO_bits::AFR::AF;

   auto &like_CMSIS() { return *reinterpret_cast<CMSIS_type *>(this); }

   void set(size_t n) { BSRR = (1 << n); }
   void clear(size_t n) { BSRR = (1 << (n + 16)); }
   bool is_set(size_t n) { return IDR.reg & (1 << n); }
   void toggle(size_t n) { is_set(n) ? clear(n) : set(n); }
   void atomic_write(uint32_t value) { BSRR = value; }

   template <class Pin_, PinMode, PushPull p = PushPull::No>
   void init();

private:
   template <size_t>
   GPIO &set(Mode);
   template <size_t>
   GPIO &set(AF);
   template <size_t>
   GPIO &set(PullResistor);
};

#if not defined(USE_MOCK_GPIO)
template <Periph p>
std::enable_if_t<p == Periph::GPIOA, GPIO &> make_reference()
{
   return *reinterpret_cast<GPIO *>(GPIOA_BASE);
}
template <Periph p>
std::enable_if_t<p == Periph::GPIOB, GPIO &> make_reference() { return *reinterpret_cast<GPIO *>(GPIOB_BASE); }
template <Periph p>
std::enable_if_t<p == Periph::GPIOC, GPIO &> make_reference() { return *reinterpret_cast<GPIO *>(GPIOC_BASE); }
template <Periph p>
std::enable_if_t<p == Periph::GPIOD, GPIO &> make_reference() { return *reinterpret_cast<GPIO *>(GPIOD_BASE); }
template <Periph p>
std::enable_if_t<p == Periph::GPIOF, GPIO &> make_reference() { return *reinterpret_cast<GPIO *>(GPIOF_BASE); }
#if defined(STM32F4) or defined(STM32F7)
template <Periph p>
std::enable_if_t<p == Periph::GPIOE, GPIO &> make_reference()
{
   return *reinterpret_cast<GPIO *>(GPIOE_BASE);
}
template <Periph p>
std::enable_if_t<p == Periph::GPIOG, GPIO &> make_reference() { return *reinterpret_cast<GPIO *>(GPIOG_BASE); }
template <Periph p>
std::enable_if_t<p == Periph::GPIOH, GPIO &> make_reference() { return *reinterpret_cast<GPIO *>(GPIOH_BASE); }
template <Periph p>
std::enable_if_t<p == Periph::GPIOI, GPIO &> make_reference() { return *reinterpret_cast<GPIO *>(GPIOI_BASE); }
#endif
#endif // #if not defined(USE_MOCK_GPIO)

template <size_t n>
GPIO &GPIO::set(Mode v)
{
   if constexpr (n == 0)
   {
      MODER.MODE0 = v;
      return *this;
   }
   else if constexpr (n == 1)
   {
      MODER.MODE1 = v;
      return *this;
   }
   else if constexpr (n == 2)
   {
      MODER.MODE2 = v;
      return *this;
   }
   else if constexpr (n == 3)
   {
      MODER.MODE3 = v;
      return *this;
   }
   else if constexpr (n == 4)
   {
      MODER.MODE4 = v;
      return *this;
   }
   else if constexpr (n == 5)
   {
      MODER.MODE5 = v;
      return *this;
   }
   else if constexpr (n == 6)
   {
      MODER.MODE6 = v;
      return *this;
   }
   else if constexpr (n == 7)
   {
      MODER.MODE7 = v;
      return *this;
   }
   else if constexpr (n == 8)
   {
      MODER.MODE8 = v;
      return *this;
   }
   else if constexpr (n == 9)
   {
      MODER.MODE9 = v;
      return *this;
   }
   else if constexpr (n == 10)
   {
      MODER.MODE10 = v;
      return *this;
   }
   else if constexpr (n == 11)
   {
      MODER.MODE11 = v;
      return *this;
   }
   else if constexpr (n == 12)
   {
      MODER.MODE12 = v;
      return *this;
   }
   else if constexpr (n == 13)
   {
      MODER.MODE13 = v;
      return *this;
   }
   else if constexpr (n == 14)
   {
      MODER.MODE14 = v;
      return *this;
   }
   else if constexpr (n == 15)
   {
      MODER.MODE15 = v;
      return *this;
   }
}

template <size_t n>
GPIO &GPIO::set(AF v)
{
   if constexpr (n == 0)
   {
      AFR.AF0 = v;
      return *this;
   }
   else if constexpr (n == 1)
   {
      AFR.AF1 = v;
      return *this;
   }
   else if constexpr (n == 2)
   {
      AFR.AF2 = v;
      return *this;
   }
   else if constexpr (n == 3)
   {
      AFR.AF3 = v;
      return *this;
   }
   else if constexpr (n == 4)
   {
      AFR.AF4 = v;
      return *this;
   }
   else if constexpr (n == 5)
   {
      AFR.AF5 = v;
      return *this;
   }
   else if constexpr (n == 6)
   {
      AFR.AF6 = v;
      return *this;
   }
   else if constexpr (n == 7)
   {
      AFR.AF7 = v;
      return *this;
   }
   else if constexpr (n == 8)
   {
      AFR.AF8 = v;
      return *this;
   }
   else if constexpr (n == 9)
   {
      AFR.AF9 = v;
      return *this;
   }
   else if constexpr (n == 10)
   {
      AFR.AF10 = v;
      return *this;
   }
   else if constexpr (n == 11)
   {
      AFR.AF10 = v;
      return *this;
   }
   else if constexpr (n == 12)
   {
      AFR.AF12 = v;
      return *this;
   }
   else if constexpr (n == 13)
   {
      AFR.AF13 = v;
      return *this;
   }
   else if constexpr (n == 14)
   {
      AFR.AF14 = v;
      return *this;
   }
   else if constexpr (n == 15)
   {
      AFR.AF15 = v;
      return *this;
   }
}

template <size_t n >
GPIO &GPIO::set(PullResistor p)
{
   if constexpr (n == 0)
   {
      PUPDR.PUPDR0 = p;
      return *this;
   }
   else if constexpr (n == 1)
   {
      PUPDR.PUPDR1 = p;
      return *this;
   }
   else if constexpr (n == 2)
   {
      PUPDR.PUPDR2 = p;
      return *this;
   }
   else if constexpr (n == 3)
   {
      PUPDR.PUPDR3 = p;
      return *this;
   }
   else if constexpr (n == 4)
   {
      PUPDR.PUPDR4 = p;
      return *this;
   }
   else if constexpr (n == 5)
   {
      PUPDR.PUPDR5 = p;
      return *this;
   }
   else if constexpr (n == 6)
   {
      PUPDR.PUPDR6 = p;
      return *this;
   }
   else if constexpr (n == 7)
   {
      PUPDR.PUPDR7 = p;
      return *this;
   }
   else if constexpr (n == 8)
   {
      PUPDR.PUPDR8 = p;
      return *this;
   }
   else if constexpr (n == 9)
   {
      PUPDR.PUPDR9 = p;
      return *this;
   }
   else if constexpr (n == 10)
   {
      PUPDR.PUPDR10 = p;
      return *this;
   }
   else if constexpr (n == 11)
   {
      PUPDR.PUPDR11 = p;
      return *this;
   }
   else if constexpr (n == 12)
   {
      PUPDR.PUPDR12 = p;
      return *this;
   }
   else if constexpr (n == 13)
   {
      PUPDR.PUPDR13 = p;
      return *this;
   }
   else if constexpr (n == 14)
   {
      PUPDR.PUPDR14 = p;
      return *this;
   }
   else if constexpr (n == 15)
   {
      PUPDR.PUPDR15 = p;
      return *this;
   }
}

template <class Pin_, PinMode v, PushPull p>
void GPIO::init()
{
   if constexpr (v == PinMode::Input)
   {
      set<Pin_::n>(Mode::Input);
   }
   else if constexpr (v == PinMode::Output)
   {
      set<Pin_::n>(Mode::Output);
   }
   else if constexpr (v == PinMode::Analog)
   {
      set<Pin_::n>(Mode::Analog);
   }
   else if constexpr (v == PinMode::Alternate_0)
   {
      set<Pin_::n>(Mode::Alternate);
      set<Pin_::n>(AF::_0);
   }
   else if constexpr (v == PinMode::Alternate_1)
   {
      set<Pin_::n>(Mode::Alternate);
      set<Pin_::n>(AF::_1);
   }
   else if constexpr (v == PinMode::Alternate_2)
   {
      set<Pin_::n>(Mode::Alternate);
      set<Pin_::n>(AF::_2);
   }
   else if constexpr (v == PinMode::Alternate_3)
   {
      set<Pin_::n>(Mode::Alternate);
      set<Pin_::n>(AF::_3);
   }
   else if constexpr (v == PinMode::Alternate_4)
   {
      set<Pin_::n>(Mode::Alternate);
      set<Pin_::n>(AF::_4);
   }
   else if constexpr (v == PinMode::Alternate_5)
   {
      set<Pin_::n>(Mode::Alternate);
      set<Pin_::n>(AF::_5);
   }
   else if constexpr (v == PinMode::Alternate_7)
   {
      set<Pin_::n>(Mode::Alternate);
      set<Pin_::n>(AF::_7);

#if defined(STM32F4) or defined(STM32F7)
   }
   else if constexpr (v == PinMode::Alternate_8)
   {
      set<Pin_::n>(Mode::Alternate);
      set<Pin_::n>(AF::_8);
#endif
   }
   if constexpr (p == PushPull::No)
   {
      set<Pin_::n>(PullResistor::No);
   }
   else if constexpr (p == PushPull::Up)
   {
      set<Pin_::n>(PullResistor::Up);
   }
   else if constexpr (p == PushPull::Down)
   {
      set<Pin_::n>(PullResistor::Down);
   }
}

} // namespace mcu
