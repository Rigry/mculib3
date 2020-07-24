// #include <windows>
// const int NotUsed = system( "color 20" );
#define STM32F030x6
#define F_OSC   8000000UL
#define F_CPU   48000000UL
#include "periph_rcc.h"
#include "pin.h"
#include "timers.h"
#include "flash.h"
// #include "button_old.h"
// #include "modbus_master.h"
// #include "adc.h"
// #include "syscfg_f0.h"
// #include "exti_f0.h"
// #include "wiegan.h"
// #include "button.h"
// #include "pwm_.h"
// #include "encoder.h"
// #include "step_motor.h"
#include "clock.h"
// #include "example/example_adc.h"
// #include "example/example_modbus_master.h"
// #include "example_flash.h"
// #include "example_safe_flash.h"


/// эта функция вызываеться первой в startup файле
extern "C" void init_clock ()
{
   // FLASH::set (FLASH::Latency::_1);

    mcu::make_reference<mcu::Periph::RCC>()
        .set (mcu::RCC:: AHBprescaler::AHBnotdiv)
        .set (mcu::RCC:: APBprescaler::APBnotdiv)
        .set (mcu::RCC::  SystemClock::CS_PLL)
        .set (mcu::RCC::    PLLsource::HSIdiv2)
        .set (mcu::RCC::PLLmultiplier::_12)
        .on_PLL()
        .wait_PLL_ready();
}

using EN    = mcu::PA2;
using DIR   = mcu::PA3;
using STEP  = mcu::PA1;
using Left  = mcu::PA4;
using Right = mcu::PA5;


int main()
{
    
    volatile decltype(auto) clock = Clock::make(); 
    
    decltype(auto) led = Pin::make<mcu::PC8,mcu::PinMode::Output>();
    Timer timer{200};

   

    


    
    while(1){
      
    led ^= timer.event();
    //   __WFI();
    }

}
