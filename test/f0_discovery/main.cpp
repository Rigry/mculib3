// #include <windows>
// const int NotUsed = system( "color 20" );
#define STM32F030x6
#define F_OSC   8000000UL
#define F_CPU   48000000UL
#include "periph_rcc.h"
#include "pin.h"
#include "timers.h"
#include "button_old.h"
#include "modbus_master.h"
#include "adc.h"
#include "syscfg_f0.h"
#include "exti_f0.h"
#include "wiegan.h"
#include "button.h"
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

using TX  = mcu::PA2;
using RX  = mcu::PA3;
using RTS = mcu::PA1;


int main()
{
    
    // constexpr auto conversion_on_channel {16};
    // struct ADC_{
    //    ADC_average& control     = ADC_average::make<mcu::Periph::ADC1>(conversion_on_channel);
    //    ADC_channel& uv_level    = control.add_channel<mcu::PA0>();
    //    ADC_channel& temperature = control.add_channel<mcu::PA1>();
    // }adc{};

    // adc.control.start();
    // mcu::example::safe_flash();
    // mcu::example::flash();

    // decltype(auto) counter = Counter::make<mcu::Periph::TIM1, mcu::PA9>(450);
    // uint16_t count{0};

    // decltype(auto) led = Pin::make<mcu::PB0, mcu::PinMode::Output>();
    // decltype(auto) led2 = Pin::make<mcu::PB1, mcu::PinMode::Output>();

    // decltype(auto) in = mcu::Button::make<mcu::PA0>();
    // decltype(auto) in2 = mcu::Button::make<mcu::PA7>();
    
    

    // constexpr bool parity_enable {true};
    // constexpr int  timeout       {100_ms};
    // constexpr UART::Settings set {
    //       not parity_enable
    //     , UART::Parity::even
    //     , UART::DataBits::_8
    //     , UART::StopBits::_1
    //     , UART::Baudrate::BR9600
    // };

    // constexpr auto uov_address {1};

    // struct Modbus {
    //     Register<1, Modbus_function::read_03, 7>  uv_level;
    // } modbus;

    // decltype(auto) modbus_master =
    //     make_modbus_master <mcu::Periph::USART1, TX, RX, RTS> (
    //         timeout, set, modbus
    // );
    // auto &rcc = REF(RCC);
    // rcc.clock_enable<mcu::PA6::periph>();
    // rcc.clock_enable<mcu::Periph::SYSCFG>();
    // decltype(auto) inter = Pin::make<mcu::PA6, mcu::PinMode::Input, mcu::PushPull::No>();
    // decltype(auto) inter1 = Pin::make<mcu::PA7, mcu::PinMode::Input, mcu::PushPull::Down>();
    // auto &syscfg = REF(SYSCFG);
    // syscfg.set_line_interrupt<mcu::PA6>();
    // syscfg.set_line_interrupt<mcu::PA7>();
    // auto &exti = REF(EXTI);
    // exti.enable_interrupt(6);
    // exti.set_trigger(exti.falling, 6);
    // exti.enable_interrupt(7);
    // exti.set_trigger(exti.falling, 7);
    // auto i{0};
    // auto j{0};

    decltype(auto) dir = Pin::make<mcu::PA4, mcu::PinMode::Output>();
    decltype(auto) step = Pin::make<mcu::PA5, mcu::PinMode::Output>();
    decltype(auto) en = Pin::make<mcu::PA6, mcu::PinMode::Output>();
    en = true;
    int time = 1;
    Timer timer{time};
    // decltype(auto) wiegan = Wiegan::make<mcu::PA6, mcu::PA7>();
    decltype(auto) led = Pin::make<mcu::PC8, mcu::PinMode::Output>();
    decltype(auto) led1 = Pin::make<mcu::PC9, mcu::PinMode::Output>();
    // uint32_t card{0};
    using Enter = mcu::PA0;
    auto enter = Button<Enter>();
    enter.set_down_callback([&]{step ^= 1;});
    enter.set_long_push_callback([&]{dir ^= 1;});
    
    while(1){
        // if ( exti.is_interrupt(7)) {
        //     j++;
        //     led ^= 1; 
        //     exti.clear_interrupt_flags<7>();  
        //     }
    //    card = wiegan.get_number();
        // if (exti.is_interrupt(6)) {
        //     led1 ^= 1; 
        //     i++; 
        //     exti.clear_interrupt_flags(6);
        // }
        // if (not exti.is_interrupt(6)) {
        //     led = true; 

        // }
        step ^= timer.event();
       
        // clk = true;
        
        

        // modbus_master();
        // led ^= in;
        // led2 ^= in2;
        // count = counter;
    }

}
