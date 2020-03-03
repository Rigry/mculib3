#define STM32F405xx
#define F_OSC   8000000UL
#define F_CPU   48000000UL

#include "init_clock.h"
#include "periph_rcc.h"
#include "periph_flash.h"
#include "pin.h"
#include "timers.h"
// #include "literals.h"
// #include "adc.h"
// #include "NTC_table.h"
// #include "delay.h"
#include "pwm_.h"
// #include "button_old.h"
#include "encoder_rotary.h"
#include "button.h"
// #include "button_old.h"
// #include "spi.h"
#include "hd44780.h"
#include "string_buffer.h"

using E   = mcu::PC14;       
using RW  = mcu::PC15;       
using RS  = mcu::PC13;      
using DB4 = mcu::PC2;       
using DB5 = mcu::PC3;
using DB6 = mcu::PC0;    
using DB7 = mcu::PC1;


/// эта функция вызываеться первой в startup файле
extern "C" void init_clock () { init_clock<F_OSC, F_CPU>(); }
// extern "C" void init_clock ()
// {
//    mcu::make_reference<mcu::Periph::FLASH>()
//       .set (mcu::FLASH::Latency::_5);

//    mcu::make_reference<mcu::Periph::RCC>()
//       .on_HSE()
//       .wait_HSE_ready()
//       .set      (mcu::RCC::AHBprescaler::AHBnotdiv)
//       .set_APB1 (mcu::RCC::APBprescaler::APBdiv4)
//       .set_APB2 (mcu::RCC::APBprescaler::APBdiv2)
//       .set      (mcu::RCC:: SystemClock::CS_PLL)
//       .set_PLLM<4>()
//       .set_PLLN<168>()
//       .set      (mcu::RCC::     PLLPdiv::_2)
//       // .set_PLLQ<7>()
//       .set      (mcu::RCC::   PLLsource::HSE)
//       .on_PLL()
//       .wait_PLL_ready();
// }

void stepping(int qty);

int main()
{
   // constexpr auto conversion_on_channel {16};
   // struct {
   //    ADC_average& control       = ADC_average::make<mcu::Periph::ADC1>(conversion_on_channel);
   //    ADC_channel& temperature   = control.add_channel<mcu::PA2>();
   // } adc{};
   

   // struct {
   //    ADC_average& control       = ADC_average::make<mcu::Periph::ADC2>(conversion_on_channel);
   //    ADC_channel& temperature_2 = control.add_channel<mcu::PA1>();
   // } adc2{};
   // decltype(auto) encoder = Encoder::make<mcu::Periph::TIM8, mcu::PC6, mcu::PC7, true>();
   // decltype(auto) pwm = PWM::make<mcu::Periph::TIM4, mcu::PD14>(490);
   // pwm.out_enable(); 
   // pwm.duty_cycle = 50;
   // pwm.frequency = 20000;
   // volatile decltype (auto) led_blue   = Pin::make<mcu::PD15, mcu::PinMode::Output>();
   // volatile decltype (auto) led_orange = Pin::make<mcu::PD13, mcu::PinMode::Output>();
   // volatile decltype (auto) enter      = mcu::Button::make<mcu::PA8>(); 
   // volatile decltype (auto) led_red    = Pin::make<mcu::PC10, mcu::PinMode::Output>();
   // volatile decltype (auto) led_green  = Pin::make<mcu::PA15, mcu::PinMode::Output>();
   // led_red = false;



   // Timer timer{500};
   // int16_t value;
   
   
   // constexpr auto _2V {2 * 16 * 4095/3.3}; 
   // auto step_pwm {10};

   // constexpr size_t U = 33;
   // constexpr size_t R = 5100;
   // volatile auto t  = NTC::u2904<U,R>[30];
   // volatile auto t2 = NTC::u2904<U,R>[26];

   

   // int temp{0};
   // int temp_2{0};
   

   

   // adc.control.set_callback ([&]{
   //    adc.temperature = adc.temperature / 16;
   //    adc2.temperature_2 = adc2.temperature_2 / 16;
   //    for (size_t i = 0; i <= std::size(NTC::u2904<U,R>) - 2; i++) {
   //       if (adc.temperature < NTC::u2904<U,R>[i] and adc.temperature > NTC::u2904<U,R>[i + 1])
   //          temp = i;
   //    }

   //    for (size_t i = 0; i <= std::size(NTC::u2904<U,R>) - 2; i++) {
   //       if (adc2.temperature_2 < NTC::u2904<U,R>[i] and adc2.temperature_2 > NTC::u2904<U,R>[i + 1])
   //          temp_2 = i;
   //    }
      
   //    // led_red = adc.temperature < t;
   // });
   //  adc.control.start();
   //  adc2.control.start();

   

   // uint16_t count{0};
   // uint16_t current = 0.5;
   // constexpr auto hd44780_pins = HD44780_pins<RS, RW, E, DB4, DB5, DB6, DB7>{};
   // String_buffer lcd;
   // HD44780 hd44780 { HD44780::make(hd44780_pins, lcd.get_buffer()) };
   // lcd.line(0).cursor(2) << "Hi, I'm V17.";
   // lcd.line(1) << "You wanna work?";

   // lcd.line(0) << "t = " << temp;
   // lcd.line(1) << cur;
   // lcd.line(1) << "I = " << current;

   // decltype(auto) dir = Pin::make<mcu::PB7, mcu::PinMode::Output>();
   // decltype(auto) step = Pin::make<mcu::PB5, mcu::PinMode::Output>();
   // decltype(auto) en = Pin::make<mcu::PB6, mcu::PinMode::Output>();
   // decltype(auto) ms_1 = Pin::make<mcu::PB15,mcu::PinMode::Output>();
   // decltype(auto) ms_2 = Pin::make<mcu::PB12,mcu::PinMode::Output>();

   // int time = 1;
   // Timer timer{1};
   // Timer timer_{5};
   // Timer direction{2000};
   // Timer tim{};

   // bool _ {false}; 

   // volatile decltype(auto) pwm = PWM::make<mcu::Periph::TIM3, mcu::PB5>(490);
   // pwm.out_enable();
   // pwm.duty_cycle = 490;
   // pwm.frequency = 800;

   // volatile decltype(auto) pwm_1 = PWM::make<mcu::Periph::TIM2, mcu::PB10>(490);
   
   // pwm_1.duty_cycle = 490;
   // pwm_1.frequency = 10;
   // pwm_1.out_enable();

   // int qty{4};

   // volatile decltype(auto) encoder = Encoder::make<mcu::Periph::TIM8, mcu::PC6, mcu::PC7, true>();
   // encoder.set_minus_callback([&](){dir = false;{step ^= 1;} step ^= 1;  led_red = true;});
   // encoder.set_plus_callback ([&](){dir = true; {step ^= 1;} step ^= 1;  led_red = false;});

   // auto enter = Button<mcu::PA8>();
   // enter.set_down_callback([&]{ _ = _ ? false : true; });


   // using Left = mcu::PB13;
   // auto left = Button<Left>();
   // left.set_down_callback([&]{ 
   //    int _ = qty; 
   //    dir = false; 
   //    while (_){
   //       tim.start(1);
   //       if (tim.done()) {
   //          step ^= 1;
   //          timer.stop();
   //          _--;
   //       }
   //    }
   // });

   // using Rigth = mcu::PB14;
   // auto right = Button<Rigth>();
   // right.set_down_callback([&]{ int _ = qty; dir = true; while (_){step ^= 1;}});

   // decltype(auto) left  = Pin::make<mcu::PB13, mcu::PinMode::Input>();
   // decltype(auto) right = Pin::make<mcu::PB14, mcu::PinMode::Input>();
   // decltype(auto) enter = Pin::make<mcu::PA8, mcu::PinMode::Input>();

   constexpr auto pin_mode = TIM::pin_mode<mcu::Periph::TIM8, mcu::PC6>();
   constexpr auto channel_ = TIM::channel<mcu::Periph::TIM8, mcu::PC6>();
   TIM& tim = mcu::make_reference<mcu::Periph::TIM8>();
   decltype(auto) pulse = Pin::make<mcu::PC6, pin_mode>();
   TIM::Channel channel = TIM::channel<mcu::Periph::TIM8, mcu::PC6>();
   TIM::EnableMask enable_mask = TIM::enable_mask<channel_>();
   mcu::make_reference<mcu::Periph::RCC>().clock_enable<mcu::Periph::TIM8>();
   tim.template set<channel_>(TIM::CompareMode::PWMmode)
          .set_prescaller(7200 - 1)
          .set_auto_reload(350 - 1)
          .set_compare(channel_, 35)
          .set_repetition(5 - 1)
          .set_update_generation()
          .update_disable()
          .set(TIM::OnePulseMode::counterStop)
          .template preload_enable<channel_>()
          .compare_enable(enable_mask)
          .auto_reload_enable()
          .counter_enable();
   
   while(1){

      // if (enter) {
      //    step ^= timer.event();
      // } else 
      //    step = false;
      
      // if (left and not right) {
      //    en = false;
      //    dir = false;
      //    if (_) {
      //       step ^= timer.event();
      //    } else {
      //       step ^= timer_.event();
      //    }
         
      // } else if (not left and not right) {
      //    step = false;
      // }
      // if (right and not left) {
      //    dir = true;
      //    en = false;
      //    if (_) {
      //       step ^= timer.event(); 
      //    } else {
      //       step ^= timer_.event();
      //    }
      // } else if (not right and not left) {
      //    step = false;
      // }

      // if (left and right) {
      //     _ = _ ? false : true; 
      // }

      // count = encoder;

      // lcd.line(0).cursor(2) << count;
      
      // lcd.line(0) << "t = " << temp;
      // led_red = temp >= 30;
      // // cur = adc.current;
      // lcd.line(1) << temp_2;
      // led_red = pwm ^= enter; 
      // pwm.frequency = encoder;
      // value = encoder;
      
      // led_green ^= timer.event();
      // // led_red   ^= enter;
      // led_green = value > 200 ? true : false;
      // pwm.duty_cycle += timer.event() ? step_pwm : 0;
      // step_pwm = (pwm.duty_cycle >= 990 or pwm.duty_cycle <= 10) ? -step_pwm : step_pwm;
      // value = pwm.duty_cycle;
      // pwm.duty_cycle = value >=  ? 0 : value;
      // if (pwm.duty_cycle >= 999)
      //    pwm.duty_cycle = 1;
   }
   
   // decltype(auto) spi = SPI_::make<mcu::Periph::SPI1, mcu::PA7, mcu::PA6, mcu::PA5, mcu::PA4, true>();
   // volatile decltype (auto) MOSI = Pin::make<mcu::PA7, mcu::PinMode::Alternate_5>();
   // volatile decltype (auto) MISO = Pin::make<mcu::PA6, mcu::PinMode::Alternate_5>();
   // volatile decltype (auto) CSK = Pin::make<mcu::PA5, mcu::PinMode::Alternate_5>();
   
   // auto &rcc = REF(RCC);
   // rcc.clock_enable<mcu::Periph::SPI1>();
   // auto &spi = REF(SPI1);
   // spi.set_mode(true)
   //    .set(mcu::SPI::Data_size::_8bits)
   //    .NSS_soft()
   //    .NSS_high()
   //    .set(SPI::Prescaler::div32)
   //    .enable();

   // while (1) {
      
      // spi.send(0x4);
      // while (not spi.is_tx_complete()) {}
   // }

//    volatile decltype (auto) led_blue   = Pin::make<mcu::PD15, mcu::PinMode::Output>();
//    volatile decltype (auto) led_orange = Pin::make<mcu::PD13, mcu::PinMode::Output>();
//    volatile decltype (auto) led_red    = Pin::make<mcu::PD14, mcu::PinMode::Output>();
//    volatile decltype (auto) led_green  = Pin::make<mcu::PD12, mcu::PinMode::Output>();
// //    Timer timer{500};
   
//    constexpr auto conversion_on_channel {16};
//    constexpr auto _2V {2 * 16 * 4095/2.9}; 
//     struct {
//         ADC_average& control     = ADC_average::make<mcu::Periph::ADC1>(conversion_on_channel);
//         ADC_channel& voltage     = control.add_channel<mcu::PA2>();
//         // ADC_channel& temperature = control.add_channel<mcu::PA1>();
//     } adc{};

//     auto& alarm       = Pin::make<mcu::PB0,mcu::PinMode::Output>();

//     adc.control.set_callback ([&]{
//         // led = adc.voltage < _2V;
//     });
//     adc.control.start();

//     Delay delay{};
//     auto& button = Button::make<mcu::PA0>();
//     // int i {0};
    
//    decltype(auto) pwm_blue = PWM::make<mcu::Periph::TIM4, mcu::PD15>();
//    pwm_blue.out_enable();
//    decltype(auto) pwm_green = PWM::make<mcu::Periph::TIM4, mcu::PD12>();
//    pwm_blue.frequency = 26000;
//    pwm_green.frequency = 26000;
//    pwm_green.out_enable();

//    decltype(auto) pwm_red = PWM::make<mcu::Periph::TIM4, mcu::PD14>();
//    pwm_red.frequency = 26000;
//    pwm_red.out_enable();

//    decltype(auto) pwm_orange = PWM::make<mcu::Periph::TIM4, mcu::PD13>();
//    pwm_orange.frequency = 26000;
//    pwm_orange.out_enable();


//    Timer timer {10};
//    Timer timer_ {20};
//    int i {0};
//    int p {0};

//    while(1) {
//       while (i < 100 ) {
//          if (timer.event()) {
//             pwm_red.duty_cycle = pwm_orange.duty_cycle = p++;
//             pwm_green.duty_cycle = pwm_blue.duty_cycle = i++;
//             ++i;
//             ++p;
//          }
//       }
//       while (i > 0) {
//          if (timer_.event()) {
//             pwm_red.duty_cycle = pwm_orange.duty_cycle = p--;
//             pwm_green.duty_cycle = pwm_blue.duty_cycle = i--;
//          }
//       }
// } // while(1) {

   //  while (1) {

      //   while (delay.ms(20)) {led_blue = led_red = true; led_orange = led_green = false;}
      //   while (delay.ms(20)) {led_orange = led_red = true; led_blue = led_green = false;}
      //   while (delay.ms(20)) {led_orange = led_green = true; led_blue = led_red = false;}
      //   while (delay.ms(20)) {led_blue = led_green = true; led_orange = led_red = false;}
      //   if (button) {i += 10; if (i > 100) i = 0;}
      //   pwm_red.duty_cycle = pwm_orange.duty_cycle = i;
      //    pwm_green.duty_cycle = pwm_blue.duty_cycle = i;


   //  }



}

