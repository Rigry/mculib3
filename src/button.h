#pragma once

#include "timers.h"
#include "literals.h"
#include "function.h"
#include "pin.h"
#include "interrupt.h"
// #include "periph_exti.h"
#include "periph_rcc.h"


// суть
// класс работает от прерываний
// запоминает коллбеки на действия
// down, up, click, long_push, increment
// и вызывает эти коллбеки, когда действие наступило
// аналогично для двух кнопок одновременно

// для двух кнопок
// не реагируем, если связанная кнопка нажата

// Для истории
// Выяснилось при отладке, что из-за дребезга контактов
// прерывание вызывается несколько раз, что можно поправить отключением прерываний
// после срабатывания первого
// Но, подход с прерываниями также приводит к тому,
// что нельзя будет сделать 2 кнопки на PA0 и PB0
// поскольку прерывание одно на номер пина.
// Поэтому решено отказаться от привязки к внешним прерываниям
// для кнопок это совсем не критично

// TODO уж больно тут копипастный метод notify в обоих классах
// стоит посмотреть как вынести общий алгоритм в будущем

struct Button_event {
    virtual void set_down_callback      (Callback<>)    = 0;
    virtual void set_up_callback        (Callback<>)    = 0;
    virtual void set_click_callback     (Callback<>)    = 0;
    virtual void set_long_push_callback (Callback<>)    = 0;
    virtual void set_increment_callback (Callback<int>) = 0;
};




template<class Pin_, bool inverted = false>
class Button : public Button_event, private TickSubscriber {
public:
    void set_down_callback      (Callback<> v)    override { down_callback      = v; }
    void set_up_callback        (Callback<> v)    override { up_callback        = v; }
    void set_click_callback     (Callback<> v)    override { click_callback     = v; }
    void set_long_push_callback (Callback<> v)    override { long_push_callback = v; }
    void set_increment_callback (Callback<int> v) override { increment_callback = v; }

    bool tied {false};

    Button() { tick_subscribe(); }

    bool is_push() { return inverted ? not pin.is_set() : pin.is_set(); }



private:
    Callback<>    down_callback;
    Callback<>    up_callback;
    Callback<>    click_callback;
    Callback<>    long_push_callback;
    Callback<int> increment_callback;
    Pin& pin {Pin::make<Pin_,mcu::PinMode::Input>()};
    size_t tick_cnt {0};
    int increment   {1};
    bool down_executed      {false};
    bool long_push_executed {false};

    void notify() override { 
        if (not is_push() and tick_cnt == 0)
            return;

        

        tick_cnt++;

        if (tick_cnt >= 10_ms and tick_cnt <= 500_ms and not down_executed) {
            if(not is_push()) {
                down_executed = true;
                execute_if (not tied, down_callback);
                return;
            }
        }

        if (tick_cnt >= 1_s and not long_push_executed) {
            long_push_executed = true;
            execute_if (not tied, long_push_callback);
            return;
        }

        if (not is_push()) {
            execute_if (not tied and down_executed, up_callback);
            execute_if (not tied and down_executed and not long_push_executed, click_callback);
            tick_cnt = 0;
            increment = 1;
            down_executed      = false;
            long_push_executed = false;
            return;
        }

        if (not increment_callback)
            return;
        auto in_range = [](auto v, auto min, auto max) {
            return (v > min) and (v <= max);
        };
        if (in_range(tick_cnt, 1_s, 2_s) and not (tick_cnt % 200)) {
            increment_callback(1);
            return;
        }
        if (in_range(tick_cnt, 2_s, 3_s) and not (tick_cnt % 100)) {
            increment_callback(1);
            return;
        }
        if (in_range(tick_cnt, 3_s, 4_s) and not (tick_cnt % 50)) {
            increment_callback(1);
            return;
        }
        if (in_range(tick_cnt, 4_s, 5_s) and not (tick_cnt % 25)) {
            increment_callback(1);
            return;
        }
        if ((tick_cnt > 5000) and not (tick_cnt % 25)) {
            increment_callback(increment);
            if (not (tick_cnt % 1000))
                increment++;
        }
    }
};


template<class Pin1, bool inverted1, class Pin2, bool inverted2>
class Tied_buttons : public Button_event, private TickSubscriber {
public:
    void set_down_callback      (Callback<> v)    override { down_callback      = v; }
    void set_up_callback        (Callback<> v)    override { up_callback        = v; }
    void set_click_callback     (Callback<> v)    override { click_callback     = v; }
    void set_long_push_callback (Callback<> v)    override { long_push_callback = v; }
    void set_increment_callback (Callback<int> v) override { increment_callback = v; }

    Tied_buttons (
          Button<Pin1, inverted1>& button1
        , Button<Pin2, inverted2>& button2
    ) : button1 {button1}
      , button2 {button2}
    {
        tick_subscribe();
    }
private:
    Callback<>    down_callback;
    Callback<>    up_callback;
    Callback<>    click_callback;
    Callback<>    long_push_callback;
    Callback<int> increment_callback;
    size_t tick_cnt {0};
    bool down_executed      {false};
    bool long_push_executed {false};

    Button<Pin1, inverted1>& button1;
    Button<Pin2, inverted2>& button2;


    void notify() override {
        if (not (button1.is_push() and button2.is_push()) and tick_cnt == 0)
            return;
        
        if (not button1.is_push() and not button2.is_push() ) {
            execute_if (down_executed, up_callback);
            execute_if (down_executed and not long_push_executed, click_callback);

            tick_cnt = 0;
            down_executed      = false;
            long_push_executed = false;
            button1.tied = false;
            button2.tied = false;
            return;
        }

        button1.tied = button2.tied = button1.is_push() and button2.is_push();

        tick_cnt++;

        if (tick_cnt >= 10_ms and not down_executed) {
            down_executed = true;
            execute (down_callback);
            return;
        }

        if (tick_cnt >= 1_s and not long_push_executed) {
            long_push_executed = true;
            execute (long_push_callback);
            return;
        }
        // TODO increment_callback
    }
};



