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
#include "adc.h"
// #include "syscfg_f0.h"
// #include "exti_f0.h"
// #include "wiegan.h"
// #include "button.h"
// #include "pwm_.h"
// #include "encoder.h"
#include "step_motor.h"
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
    struct Flash_data {
        uint32_t data = 0;
    } flash;

    [[maybe_unused]] auto _ = Flash_updater<
        mcu::FLASH::Sector::_10
      , mcu::FLASH::Sector::_9
   >::make (&flash);
    
    
    constexpr auto conversion_on_channel {16};
    struct ADC_{
       ADC_average& control = ADC_average::make<mcu::Periph::ADC1>(conversion_on_channel);
       ADC_channel& u       = control.add_channel<mcu::PA0>();
    }adc{};

    volatile decltype (auto) led = Pin::make<mcu::PC9, mcu::PinMode::Output>();

    adc.control.start();
    
    

   

    // mcu::example::safe_flash();
    // mcu::example::flash();

//    constexpr auto pin_mode = TIM::pin_mode<mcu::Periph::TIM3, mcu::PC8>();
//    constexpr auto channel_ = TIM::channel<mcu::Periph::TIM3, mcu::PC8>();
//    TIM& tim = mcu::make_reference<mcu::Periph::TIM3>();
//    Pin::make<mcu::PC8, pin_mode>();
//    TIM::Channel channel = TIM::channel<mcu::Periph::TIM3, mcu::PC8>();
//    TIM::EnableMask enable_mask = TIM::enable_mask<channel_>();
//    auto& rcc = mcu::make_reference<mcu::Periph::RCC>();
//    rcc.clock_enable<mcu::Periph::TIM3>();
//    tim.template set<channel_>(TIM::CompareMode::InvertedPWMmode)
//           .set_prescaller(143)
//           .set_auto_reload(62499)
//           .update_interrupt_enable()
//           .set_compare(channel_, 5)
//         //   .set_repetition(5 - 1)
//         //   .set_update_generation()
//         //   .update_disable()
//           .set(TIM::OnePulseMode::counterStop)
//           .template preload_enable<channel_>()
//           .compare_enable(enable_mask)
//           .main_output_enable()
//           .auto_reload_enable()
//           .counter_enable();


    auto step_motor = Step_motor<EN, DIR, STEP, Right, Left>();

    constexpr auto _3V {3 * 16 * 4095/3.3}; 

    Timer timer{200_ms};
    Timer switch_{200_ms};

    uint32_t data{flash.data};
    bool write{false};
    
    while(1){
      
        step_motor();
        
        if (switch_.event())
            data++;
        led ^= timer.event();
        if (adc.u < _3V and not write) {
            flash.data = data;
            write = true;
        } else if (adc.u > _3V and write)
        {
            write = false;
        }
        
    //   __WFI();
    }

}
