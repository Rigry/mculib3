#pragma once

#include "periph_pwr.h"
#include "periph_exti.h"
#include "function.h"
#include "interrupt.h"

using PWR = mcu::PWR;
using Edge   = mcu::EXTI::Edge;

class Pwr_control
{
   PWR& pwr;
   EXTI& exti;
   Interrupt& interrupt_pvd;
   Callback<>callback;

   using Parent = Pwr_control;

   Pwr_control(PWR& pwr, EXTI& exti, Interrupt& interrupt_pvd) : pwr{pwr}, exti{exti}, interrupt_pvd{interrupt_pvd} {}

   Pwr_control &operator=(const Pwr_control &) = delete;

   void PVD_Interrupt()
   {
      if (callback) callback();
   }

   struct PVD_interrupt : Interrupting
   {
      Parent& parent;
      PVD_interrupt (Parent& parent) : parent(parent) {
          parent.interrupt_pvd.subscribe (this);
      }
      void interrupt() override {parent.PVD_Interrupt();} 
   } PVD_ {*this};

public:

   template <PWR::Threshold threshold>
   static auto& make()
   {
      static Pwr_control pwr_control {
           mcu::make_reference<Periph::PWR>()
         , mcu::make_reference<Periph::EXTI>()
         , get_interrupt<Periph::PWR>()
      };

      pwr_control.exti.enable_interrupt(16);
      pwr_control.exti.set_trigger(Edge::rising, 16);//странная фигня, почему-то наоборот 
      pwr_control.interrupt_pvd.enable();

      auto &rcc = REF(RCC);
      rcc.clock_enable<Periph::PWR>();

      pwr_control.pwr.set(threshold)
                 .enable();


      return pwr_control;
   }

   bool is_lower()
   {
      return pwr.is_lower();
   }

   void set_callback (Callback<> v) {callback = v;}

};
