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
#include "button.h"
// #include "pwm_.h"
// #include "encoder.h"
// #include "step_motor.h"
#include "clock.h"
// #include "example/example_adc.h"
// #include "example/example_modbus_master.h"
// #include "example_flash.h"
// #include "example_safe_flash.h"
#include "string_buffer.h"
#include "hd44780.h"


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

using RS    = mcu::PA0;  
using E     = mcu::PA1;       
using RW    = mcu::PA2;           
using DB4   = mcu::PA3;       
using DB5   = mcu::PA4;
using DB6   = mcu::PA5;    
using DB7   = mcu::PA6;

using Enter = mcu::PC14;

constexpr auto day_of_week = std::array {
    "no", "Пн", "Вт", "Ср", "Чт", "Пт", "Сб", "Вск"
};

constexpr std::string_view day_of_week_to_string(int i) {
    return day_of_week[i];
}

int main()
{
    struct Date {
        uint16_t year   = 20;
        uint16_t month  = 8;
        uint16_t day    = 5;
        uint16_t date   = 7;
        uint16_t hour   = 10;
        uint16_t minute = 30;
        uint16_t second = 0;
    }date;

    volatile decltype(auto) clock = Clock<Date>::make(date); 
    
    decltype(auto) led = Pin::make<mcu::PC8,mcu::PinMode::Output>();
    Timer timer{200};
    volatile uint16_t hour{0};

    constexpr auto hd44780_pins = HD44780_pins<RS, RW, E, DB4, DB5, DB6, DB7>{};
    String_buffer lcd {};
    HD44780 hd44780 { HD44780::make(hd44780_pins, lcd.get_buffer()) };

    auto enter = Button<Enter>();
    enter.set_down_callback([&]{
        date.hour = 19;
        date.minute = 20;
        date.day = 6;
        date.date = 1;
        date.month = 8;
        clock.set_time(date);
    });

    Timer delay{3_s};
    while(not delay.done()){}

    while(1){
      
    led ^= timer.event();
    lcd.line(0).cursor(4)._10(clock.hour());
    lcd.line(0).cursor(6) << ":";
    lcd.line(0).cursor(7)._10(clock.minute());
    lcd.line(0).cursor(9) << ":";
    lcd.line(0).cursor(10)._10(clock.second());
    lcd.line(1).cursor(0) << day_of_week_to_string(clock.day());
    lcd.line(1).cursor(8)._10(clock.date());
    lcd.line(1).cursor(10) << ".";
    lcd.line(1).cursor(11)._10(clock.month());
    lcd.line(1).cursor(13) << ".";
    lcd.line(1).cursor(14)._10(clock.year());

    //   __WFI();
    }

}


