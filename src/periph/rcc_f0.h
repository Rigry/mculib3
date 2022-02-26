#pragma once

#include "bits_rcc_f0.h"

namespace mcu {

class RCC {
	volatile RCC_bits::CR      CR;         // clock control register,                offset: 0x00
	volatile RCC_bits::CFGR    CFGR;       // clock configuration register,          offset: 0x04
	volatile uint32_t          CIR;        // clock interrupt register,              offset: 0x08
	volatile uint32_t          APB2RSTR;   // APB2 peripheral reset register,        offset: 0x0C
	volatile uint32_t          APB1RSTR;   // APB1 peripheral reset register,        offset: 0x10
	volatile RCC_bits::AHBENR  AHBENR;     // AHB peripheral clock register,         offset: 0x14
	volatile RCC_bits::APB2ENR APB2ENR;    // APB2 peripheral clock enable register, offset: 0x18
	volatile RCC_bits::APB1ENR APB1ENR;    // APB1 peripheral clock enable register, offset: 0x1C
	volatile RCC_bits::BDCR    BDCR;       // Backup domain control register,        offset: 0x20
	volatile RCC_bits::CSR     CSR;        // clock control & status register,       offset: 0x24
	volatile uint32_t          AHBRSTR;    // AHB peripheral reset register,         offset: 0x28
	volatile uint32_t          CFGR2;      // clock configuration register 2,        offset: 0x2C
	volatile uint32_t          CFGR3;      // clock configuration register 3,        offset: 0x30
	volatile uint32_t          CR2;        // clock control register 2,              offset: 0x34


public:
	using CMSIS_type    = RCC_TypeDef;
	using AHBprescaler  = RCC_bits::CFGR::AHBprescaler;
	using APBprescaler  = RCC_bits::CFGR::APBprescaler;
	using SystemClock   = RCC_bits::CFGR::SystemClock;
	using PLLsource     = RCC_bits::CFGR::PLLsource;
	using PLLmultiplier = RCC_bits::CFGR::PLLmultiplier;
	using RTC_Clock     = RCC_bits::BDCR::Clock;
	using Drive         = RCC_bits::BDCR::Drive;

	auto& like_CMSIS() { return *reinterpret_cast<CMSIS_type*>(this); }

	RCC& set (AHBprescaler  v) { CFGR.HPRE   = v; return *this; }
	RCC& set (APBprescaler  v) { CFGR.PPRE   = v; return *this; }
	RCC& set (SystemClock   v) { CFGR.SW     = v; return *this; }
	RCC& set (PLLsource     v) { CFGR.PLLSRC = v; return *this; }
	RCC& set (PLLmultiplier v) { CFGR.PLLMUL = v; return *this; }
	RCC& set (RTC_Clock     v) { BDCR.RTCSEL = v; return *this; }
	RCC& set (Drive         v) { BDCR.LSEDRV = v; return *this; }

	RCC& on_HSE        () { CR.HSEON = true;         return *this; }
	RCC& wait_HSE_ready() { while (not CR.HSERDY) {} return *this; }
	RCC& on_PLL        () { CR.PLLON = true;         return *this; }
	RCC& wait_PLL_ready() { while (not CR.PLLRDY) {} return *this; }
	RCC& on_LSE        () { BDCR.LSEON = true;       return *this; }
	RCC& wait_LSE_ready() { while (not BDCR.LSERDY){}return *this; }
	RCC& on_LSI        () { CSR.LSION = true;        return *this; }
	RCC& wait_LSI_ready() {while (not CSR.LSIRDY) {} return *this; }
	RCC& backup_domain_reset(bool reset) { BDCR.BDRST = reset; return *this; }
	bool clock_running () {return BDCR.RTCEN;}

	size_t get_APB_clock()
	{
		auto v = CFGR.PPRE;
		return v == APBprescaler::APBnotdiv ? F_CPU     :
				 v == APBprescaler::APBdiv2   ? F_CPU / 2 :
				 v == APBprescaler::APBdiv4   ? F_CPU / 4 :
				 v == APBprescaler::APBdiv8   ? F_CPU / 8 :
														  F_CPU / 16;
	}

	template<Periph p> void clock_enable()
	{
		if      constexpr (p == Periph::GPIOA)  AHBENR .IOPAEN  = true;
		else if constexpr (p == Periph::GPIOB)  AHBENR .IOPBEN  = true;
		else if constexpr (p == Periph::GPIOC)  AHBENR .IOPCEN  = true;
		else if constexpr (p == Periph::GPIOD)  AHBENR .IOPDEN  = true;
		else if constexpr (p == Periph::GPIOF)  AHBENR .IOPFEN  = true;

		else if constexpr (p == Periph::USART1) APB2ENR.USART1EN  = true;

		else if constexpr (p == Periph::TIM1)   APB2ENR.TIM1EN  = true;
		else if constexpr (p == Periph::TIM3)   APB1ENR.TIM3EN  = true;
		else if constexpr (p == Periph::TIM14)  APB1ENR.TIM14EN = true;
		else if constexpr (p == Periph::TIM16)  APB2ENR.TIM16EN = true;
		else if constexpr (p == Periph::TIM17)  APB2ENR.TIM17EN = true;

		else if constexpr (p == Periph::DMA1)         AHBENR.DMAEN = true;
		else if constexpr (p == Periph::DMA1_stream1) AHBENR.DMAEN = true;
		else if constexpr (p == Periph::DMA1_stream2) AHBENR.DMAEN = true;
		else if constexpr (p == Periph::DMA1_stream3) AHBENR.DMAEN = true;
		else if constexpr (p == Periph::DMA1_stream4) AHBENR.DMAEN = true;
		else if constexpr (p == Periph::DMA1_stream5) AHBENR.DMAEN = true;

		else if constexpr (p == Periph::ADC1)   APB2ENR.ADC1EN = true;

		else if constexpr (p == Periph::SYSCFG) APB2ENR.SYSCFGEN = true;

		else if constexpr (p == Periph::RTC)    BDCR.RTCEN = true;

		else if constexpr (p == Periph::PWR)    APB1ENR.PWREN = true;
		
        else static_assert ( always_false_v<decltype(p)>, "допиши clock_enable");
	}

	auto clock (Periph p) { return get_APB_clock(); }
};

#if not defined(USE_MOCK_RCC)
template<Periph p> std::enable_if_t<p == Periph::RCC, RCC&> make_reference() { return *reinterpret_cast<RCC*>(RCC_BASE); }
#endif
} // namespace mcu {