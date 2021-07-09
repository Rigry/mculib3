// #include <windows>
// const int NotUsed = system( "color 20" );
#define STM32F030x6
#define F_OSC   8000000UL
#define F_CPU   48000000UL
#include "periph_rcc.h"
#include "pin.h"
// #include "pwm_.h"
#include "timers.h"
#include "flash.h"
// #include <tuple>
// #include <array>
#include "seven_segment_indicator.h"
#include "button.h"
#include "buzzer.h"


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

using A  = mcu::PB10;
using B  = mcu::PB2;
using C  = mcu::PB0;
using D  = mcu::PA7;
using E  = mcu::PA6;
using F  = mcu::PB12;
using G  = mcu::PB11;
using H  = mcu::PB1;
using K1 = mcu::PB3;
using K2 = mcu::PA15;
using K3 = mcu::PB9;
using K4 = mcu::PB8;
using K5 = mcu::PB7;
using K6 = mcu::PB6;

using LEVEL = mcu::PC13;
using COVER = mcu::PC14;

// time setting buttons
using TIME_UP   = mcu::PB5;
using TIME_DOWN = mcu::PB4;
// temperature setting buttons
using TEMP_UP   = mcu::PA11;
using TEMP_DOWN = mcu::PA10;
// control buttons
using START = mcu::PF7;
using MODE  = mcu::PB13;
using F_EN  = mcu::PA12;

// generator control
using UZ1 = mcu::PA2;
using UZ2 = mcu::PA3;
using UZ3 = mcu::PA4;
// heater control
using HEATER = mcu::PA5;

using LED_M1   = mcu::PB14;
using LED_M2   = mcu::PB15;
using LED_M3   = mcu::PA9;
using LED_F_EN = mcu::PF6;

using BUZZER = mcu::PA8;

// void down (int increment = 1, uint8_t& t) 
// {
//     t++;
// }


int main()
{
    // decltype(auto) k1 = Pin::make<K1_, mcu::PinMode::Output>();
    // k1 = true;

    // auto led = make_pins<mcu::PinMode::Output, A_, B_, C_, D_, E_, F_, G_, H_>();
    

    // std::tuple<int, int, int, int, int, int, int, int> symbols [] = {
    //     std::make_tuple(0, 0, 0, 0, 0, 0, 1, 1),
    //     std::make_tuple(1, 0, 0, 1, 1, 1, 1, 1),
    //     std::make_tuple(0, 0, 1, 0, 0, 1, 0, 1) 
    // };

    // led = symbols[2];

//     constexpr auto conversion_on_channel {16};
// struct ADC_{
//    ADC_average& control     = ADC_average::make<mcu::Periph::ADC1>(conversion_on_channel);
//    ADC_channel& temperature = control.add_channel<mcu::PA0>();
// };

    auto[led_m_1, led_m_2, led_m_3, led_f] = make_pins<mcu::PinMode::Output, LED_M1, LED_M2, LED_M3, LED_F_EN>();
    decltype (auto) pwm = PWM::make<mcu::Periph::TIM1, mcu::PA8>();
    Buzzer buzzer{pwm};

    auto[uz_1, uz_2, uz_3, heater] = make_pins<mcu::PinMode::Output, UZ1, UZ2, UZ3, HEATER>();
    auto[level, cover] = make_pins<mcu::PinMode::Input, LEVEL, COVER>();

    auto time_up   = Button<TIME_UP>();
    auto time_down = Button<TIME_DOWN>();
    auto temp_up   = Button<TEMP_UP>();
    auto temp_down = Button<TEMP_DOWN>();
    auto start     = Button<START>();
    auto mode      = Button<MODE>();
    // auto f_en      = Button<F_EN>();

    volatile uint8_t t{21};
    volatile uint8_t n{59};
    volatile uint8_t p{43};

    time_up.set_down_callback(
        [&]{n++; 
        buzzer.brief();}
    );
    

    // time_up.set_increment_callback(
    //     [&](auto i){down(i, t);
    // });

    

    time_down.set_down_callback(
        [&]{n--; 
        buzzer.brief();}
    );

    temp_up.set_down_callback(
        [&]{t++; 
        buzzer.brief();}
    );

    temp_down.set_down_callback(
        [&]{t--; 
        buzzer.brief();}
    );
    

    SSI<A, B, C, D, E, F, G, H, K1, K2, K3, K4, K5, K6> ssi{};

    // Timer timer {500_ms};
    // Timer timer1 {100_ms};

    volatile bool l{false};
    volatile bool c{false};

// ssi.dinamic_test();
// ssi.dinamic_test();
    

    while(1){

            // l = level;
            // c = cover;
            // led_m_1 = l;
            // led_m_2 = l;
            // led_m_3 = c;
            // led_f   = c;
            // uz_1 = 1;
            // uz_2 = 1;
            // uz_3 = 1;
            // heater = 1;

            // ssi.test(Sign::Level);
            

            // ssi.buffer[0] = n % 10;
            // ssi.buffer[1] = n / 10;
            // ssi.buffer[2] = t % 10;
            // ssi.buffer[3] = t / 10;
            // ssi.buffer[4] = p % 10;
            // ssi.buffer[5] = p / 10;
            // buzzer.longer();

            // ssi.point[1] ^= timer.event();
            // ssi.point[3] ^= timer1.event();
            // led_m_3 = true;
    
    }

    
    
      __WFI();
    

}


