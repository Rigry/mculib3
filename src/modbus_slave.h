#pragma once

#include <functional>
#include "timers.h"
#include "table_crc.h"
#include "uart.h"
#include "interrupt.h"
#include "modbus_common.h"
#include <cstring>
#include <array>

#if defined(USE_MOCK_UART)
using UART_ = mock::UART;
#else
using UART_ = ::UART;
#endif


template <class InRegs_t, class OutRegs_t, size_t coils_qty = 0>
class Modbus_slave : TickSubscriber
{
    UART_& uart;
    Interrupt& interrupt_usart;
    Interrupt& interrupt_DMA_channel; 

    int time {0}; // выдержка времени для модбаса 
    int modbus_time {0};

    const uint8_t address;
    uint8_t func;
    uint16_t first_reg{0};
    uint16_t last_reg {0};
    uint16_t qty_reg  {0};
    uint8_t  qty_byte {0};
    uint16_t data;
    uint16_t crc{0};
    size_t last_message_size;


    uint16_t crc16 (uint8_t* data, uint8_t length);
    uint8_t  set_high_bit (uint8_t);

    bool check_CRC  ();
    bool check_value();
    bool check_regs (uint16_t qty_reg_device); // for 03 16
    bool check_reg  (uint16_t qty_reg_device); // for 05

    void answer_error (Modbus_error_code);
    template <class fun> void answer_03 (fun request);
    template <class function> void answer_16 (function reaction);
    void answer_05();

    void uartInterrupt()
    {
        if (uart.is_rx_IDLE()) 
            tick_subscribe();
    }
    void dmaInterrupt()
    {
        if (uart.is_tx_complete())
            uart.receive();
    }

    void notify() override 
    {
        time++;
    }

    using Parent = Modbus_slave;

    struct uart_interrupt : Interrupting
    {
        Parent& parent;
        uart_interrupt (Parent& parent) : parent(parent) {
            parent.interrupt_usart.subscribe (this);
        }
        void interrupt() override {parent.uartInterrupt();} 
    } uart_ {*this};

    struct dma_interrupt : Interrupting
    {
        Parent& parent;
        dma_interrupt (Parent& parent) : parent(parent) {
            parent.interrupt_DMA_channel.subscribe (this);
        }
        void interrupt() override {parent.dmaInterrupt();} 
    } dma_ {*this};

public:

    static constexpr uint16_t InRegQty  = sizeof (InRegs_t) / 2;
    static constexpr uint16_t OutRegQty = sizeof(OutRegs_t) / 2;

    union {
        InRegs_t inRegs;
        uint16_t arInRegs[InRegQty];
    };
    union {
        OutRegs_t outRegs;
        uint16_t arOutRegs[OutRegQty];
    };
    union {
        InRegs_t inRegsMin;
        uint16_t arInRegsMin[InRegQty];
    };
    union {
        InRegs_t inRegsMax;
        uint16_t arInRegsMax[InRegQty];
    };
    std::array<std::function<void(bool)>, coils_qty> force_single_coil_05;

    bool signed_[InRegQty] {};
    
    Modbus_slave (
          uint8_t address
        , UART_& uart
        , Interrupt& interrupt_usart
        , Interrupt& interrupt_DMA_channel
    ) : uart                  {uart}
      , interrupt_usart       {interrupt_usart}
      , interrupt_DMA_channel {interrupt_DMA_channel}
      , address               {address}
      , arInRegs    {}
      , outRegs     {}
      , arInRegsMin {}
      , arInRegsMax {}
    {}

    template <mcu::Periph usart, class TXpin,  class RXpin, class RTSpin> 
    static auto& make (uint8_t address, UART_::Settings set)
    {
        auto& uart_ref = UART_::make<usart, TXpin, RXpin, RTSpin>();

        static Modbus_slave<InRegs_t, OutRegs_t, coils_qty> modbus {
              address
            , uart_ref
            , get_interrupt<usart>()
            , get_interrupt<USART::default_stream<TXpin>()>()
        };

        modbus.modbus_time = set.baudrate == UART_::Baudrate::BR9600  ? 4 :
                                    set.baudrate == UART_::Baudrate::BR14400 ? 3 :
                                    set.baudrate == UART_::Baudrate::BR19200 ? 2 :
                                    set.baudrate == UART_::Baudrate::BR28800 ? 2 : 1;

        modbus.uart.init(set);
        modbus.uart.receive();
        return modbus;
    }


    template <class function, class fun>
    void operator() (function reaction, fun request = [](){});
    auto& buffer(){return uart.buffer;}

};

#define GET_ADR(struct, reg)     (offsetof(struct, reg) / 2)





































template <class InRegs_t, class OutRegs_t, size_t coils_qty>
template <class function, class fun>
inline void Modbus_slave<InRegs_t, OutRegs_t, coils_qty>::operator() (function reaction, fun request)
{
    if (uart.is_receiving()) {
        time = 0;
        tick_unsubscribe();
        return;
    }
    
    if (time < modbus_time) 
        return;

    time = 0;
    tick_unsubscribe();
    
    if (uart.buffer.size() < 8) {
        uart.receive();
        return;
    }

    if (uart.buffer.front() != address) {
        uart.receive();
        return;
    }
    if (not check_CRC()) {
        uart.receive();
        return;
    }
    uart.buffer.pop_front(); // adr
    func = uart.buffer.pop_front();
    switch (Modbus_function(func)) {
        case Modbus_function::read_03 : answer_03(request);         break;
        case Modbus_function::write_16: answer_16(reaction); break;
        case Modbus_function::force_coil_05: answer_05();    break;
        default: answer_error (Modbus_error_code::wrong_func);
    }
}




template <class InReg, class OutRegs_t, size_t coils_qty>
uint8_t Modbus_slave<InReg, OutRegs_t, coils_qty>::set_high_bit(uint8_t func)
{
    return (func | 0b10000000);
}

template <class InReg, class OutRegs_t, size_t coils_qty>
bool Modbus_slave<InReg, OutRegs_t, coils_qty>::check_CRC()
{
    auto high = uart.buffer.pop_back();
     auto low  = uart.buffer.pop_back();
    auto [low_, high_] = CRC16(uart.buffer.begin(), uart.buffer.end());
    return (high == high_) and (low == low_);
}

template <class InReg, class OutRegs_t, size_t coils_qty>
bool Modbus_slave<InReg, OutRegs_t, coils_qty>::check_regs(uint16_t qty_reg_device)
{
    uart.buffer >> first_reg;
    uart.buffer >> qty_reg; 
    last_reg = first_reg + qty_reg - 1;
    qty_byte = qty_reg * 2;
    return (last_reg <= (qty_reg_device - 1));
}

template <class InReg, class OutRegs_t, size_t coils_qty>
bool Modbus_slave<InReg, OutRegs_t, coils_qty>::check_reg(uint16_t qty_reg_device)
{
    uart.buffer >> first_reg;
    return first_reg < qty_reg_device;
}

template <class InReg, class OutRegs_t, size_t coils_qty>
void Modbus_slave<InReg, OutRegs_t, coils_qty>::answer_error(Modbus_error_code code)
{
    uart.buffer.clear();
    
    if (code == Modbus_error_code::wrong_func)
        uart.buffer << address << set_high_bit(func) << static_cast<uint8_t>(code);
    else if (code == Modbus_error_code::wrong_reg)
        uart.buffer << address << set_high_bit(func) << static_cast<uint8_t>(code);
    else if (code == Modbus_error_code::wrong_value)
        uart.buffer << address << set_high_bit(func) << static_cast<uint8_t>(code);
        
    auto [low_, high_] = CRC16(uart.buffer.begin(), uart.buffer.end());
    uart.buffer << low_ << high_;
    uart.transmit();
}

template <class InReg, class OutRegs_t, size_t coils_qty>
template <class fun>
void Modbus_slave<InReg, OutRegs_t, coils_qty>::answer_03(fun request)
{
    if (not check_regs(OutRegQty)) {
        answer_error(Modbus_error_code::wrong_reg);
        return;
    }
    uart.buffer.clear();
    // TODO определить оператор вместо статик каста
    uart.buffer << address << static_cast<uint8_t>(Modbus_function::read_03) << qty_byte;
    while(qty_reg--) {
        request(first_reg);
        uart.buffer << arOutRegs[first_reg++];
    }
    auto [low_, high_] = CRC16(uart.buffer.begin(), uart.buffer.end());
    uart.buffer << low_ << high_;

    uart.transmit();
}

template <class InReg, class OutRegs_t, size_t coils_qty>
template <class function>
void Modbus_slave<InReg, OutRegs_t, coils_qty>::answer_16(function reaction)
{
    if (not check_regs(InRegQty)) {
        answer_error(Modbus_error_code::wrong_reg);
        return;
    }

    uart.buffer >> qty_byte;

    if (not check_value()) {
        answer_error(Modbus_error_code::wrong_value);
        return;
    }
    for (uint16_t i = 0; i < qty_reg; i++) {
        reaction (first_reg + i);
    }
    uart.buffer.clear();
    uart.buffer << address << func << first_reg << qty_reg;
    auto [low_, high_] = CRC16(uart.buffer.begin(), uart.buffer.end());
    uart.buffer << low_ << high_;
    uart.transmit();
}

template <class InReg, class OutRegs_t, size_t coils_qty>
void Modbus_slave<InReg, OutRegs_t, coils_qty>::answer_05()
{
    if (not check_reg(coils_qty)) {
        answer_error(Modbus_error_code::wrong_reg);
        return;
    }

    uint16_t value;
    uart.buffer >> value;

    enum {off = 0, on = 0x00FF};

    if (value != 0 and value != 0xFF00) {
        answer_error(Modbus_error_code::wrong_value);
        return;
    }

    auto& callback = force_single_coil_05[first_reg];
    if (callback)
        callback(value == 0xFF00);

    uart.buffer.set_size(8); // данные в ответе те же
    uart.transmit();
}

template <class InRegs_t, class OutRegs_t, size_t coils_qty>
bool Modbus_slave<InRegs_t, OutRegs_t, coils_qty>::check_value()
{
    for (uint16_t i = 0; i < qty_reg; i++) {
        uart.buffer >> data;
        if (signed_[first_reg + i]) {
            if ((int16_t(data) < int16_t(arInRegsMin[first_reg + i]) and int16_t(arInRegsMin[first_reg + i]) != 0) or
                 (int16_t(data) > int16_t(arInRegsMax[first_reg + i]) and int16_t(arInRegsMax[first_reg + i]) != 0))
                return false;
            arInRegs[first_reg + i] = int16_t(data);
        } else {
            if ((data < arInRegsMin[first_reg + i] and arInRegsMin[first_reg + i] != 0) or
                 (data > arInRegsMax[first_reg + i] and arInRegsMax[first_reg + i] != 0))
                return false;
            arInRegs[first_reg + i] = data;
        }
    }

    return true;
}


