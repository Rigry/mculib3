#pragma once

#define USE_MOCK_GPIO
#include "periph_gpio.h"
#include <iostream>

namespace mock {

std::ostream& operator<< (std::ostream& s, mcu::PinMode v)
{
   return
      v == mcu::PinMode::Input        ? s << "Input"        :
      v == mcu::PinMode::Output       ? s << "Output"       :
      v == mcu::PinMode::Alternate_0  ? s << "Alternate 0"  :
      v == mcu::PinMode::Alternate_1  ? s << "Alternate 1"  :
      v == mcu::PinMode::Alternate_2  ? s << "Alternate 2"  :
      v == mcu::PinMode::Alternate_3  ? s << "Alternate 3"  :
      v == mcu::PinMode::Alternate_4  ? s << "Alternate 4"  :
      v == mcu::PinMode::Alternate_5  ? s << "Alternate 5"  :
      v == mcu::PinMode::Alternate_6  ? s << "Alternate 6"  :
      v == mcu::PinMode::Alternate_7  ? s << "Alternate 7"  :
   #if defined(STM32F4) or defined(STM32F7)
      v == mcu::PinMode::Alternate_8  ? s << "Alternate 8"  :
      v == mcu::PinMode::Alternate_9  ? s << "Alternate 9"  :
      v == mcu::PinMode::Alternate_10 ? s << "Alternate 10" :
      v == mcu::PinMode::Alternate_11 ? s << "Alternate 11" :
      v == mcu::PinMode::Alternate_12 ? s << "Alternate 12" :
      v == mcu::PinMode::Alternate_13 ? s << "Alternate 13" :
      v == mcu::PinMode::Alternate_14 ? s << "Alternate 14" :
      v == mcu::PinMode::Alternate_15 ? s << "Alternate 15" :
   #endif
      s;
}

class GPIO : public mcu::GPIO
{
   std::ostream* process {nullptr};
   GPIO() = default;
public:
   template<mcu::Periph port>
   static GPIO& make()
   {
      static GPIO gpio;
      return gpio;
   }
   void set_stream (std::ostream& s) { process = &s; }
   friend std::ostream& operator<< (std::ostream&, const GPIO&);

   GPIO& set (size_t n) {
      if (process) *process << "установка вывода " << n << " порта " << *this << std::endl;
      static_cast<mcu::GPIO*>(this)->set(n);
      return *this;
   }

   GPIO& clear (size_t n) {
      if (process) *process << "сброс вывода " << n << " порта " << *this << std::endl;
      static_cast<mcu::GPIO*>(this)->clear(n);
      return *this;
   }

   GPIO& toggle (size_t n) {
      if (process)
         *process << "переключение вывода " << n << " порта " << *this
                  << ", а именно " << (this->like_CMSIS().IDR & (1 << n) ? "сброс" : "установка") << std::endl;
      static_cast<mcu::GPIO*>(this)->toggle(n);
      return *this;
   }

   template<class Pin_, mcu::PinMode mode>
   void init() {
      if (process) *process << "инициализация вывода " << Pin_::n << " порта " << *this << " в режиме " << mode << std::endl;
      static_cast<mcu::GPIO*>(this)->init<Pin_,mode>();
   }

   struct Mock {
      GPIO& parent;
      Mock (GPIO& parent) : parent{parent} {}
      void set (size_t n, bool v)
      {
         v ? parent.like_CMSIS().IDR |= (1 << n) : parent.like_CMSIS().IDR &= ~(1 << n);
      }
   } mock {*this};


};

std::ostream& operator<< (std::ostream& s, const GPIO& v)
{
   return
      &v == &GPIO::make<mcu::Periph::GPIOA>() ? s << "GPIOA" :
      &v == &GPIO::make<mcu::Periph::GPIOB>() ? s << "GPIOB" :
      &v == &GPIO::make<mcu::Periph::GPIOC>() ? s << "GPIOC" :
      &v == &GPIO::make<mcu::Periph::GPIOD>() ? s << "GPIOD" :
      &v == &GPIO::make<mcu::Periph::GPIOF>() ? s << "GPIOF" :
   #if defined(STM32F4) or defined(STM32F7)
      &v == &GPIO::make<mcu::Periph::GPIOE>() ? s << "GPIOE" :
      &v == &GPIO::make<mcu::Periph::GPIOG>() ? s << "GPIOG" :
      &v == &GPIO::make<mcu::Periph::GPIOH>() ? s << "GPIOH" :
      &v == &GPIO::make<mcu::Periph::GPIOI>() ? s << "GPIOI" :
   #endif
      s;
}

} // namespace mock {

namespace mcu {
   template<Periph p> std::enable_if_t<p == Periph::GPIOA, mock::GPIO&> make_reference() { return mock::GPIO::make<p>(); }
   template<Periph p> std::enable_if_t<p == Periph::GPIOB, mock::GPIO&> make_reference() { return mock::GPIO::make<p>(); }
   template<Periph p> std::enable_if_t<p == Periph::GPIOC, mock::GPIO&> make_reference() { return mock::GPIO::make<p>(); }
   template<Periph p> std::enable_if_t<p == Periph::GPIOD, mock::GPIO&> make_reference() { return mock::GPIO::make<p>(); }
   template<Periph p> std::enable_if_t<p == Periph::GPIOF, mock::GPIO&> make_reference() { return mock::GPIO::make<p>(); }
#if defined(STM32F4) or defined(STM32F7)
   template<Periph p> std::enable_if_t<p == Periph::GPIOE, mock::GPIO&> make_reference() { return mock::GPIO::make<p>(); }
   template<Periph p> std::enable_if_t<p == Periph::GPIOG, mock::GPIO&> make_reference() { return mock::GPIO::make<p>(); }
   template<Periph p> std::enable_if_t<p == Periph::GPIOH, mock::GPIO&> make_reference() { return mock::GPIO::make<p>(); }
   template<Periph p> std::enable_if_t<p == Periph::GPIOI, mock::GPIO&> make_reference() { return mock::GPIO::make<p>(); }
#endif
}