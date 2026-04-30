#include "mbed.h"

#include "md.hpp"

MD z(PA_11, PA_12, 0);
MD y(PB_1,  PB_2, 0);
MD x(PB_14, PB_13,  0);

DigitalIn button1(PC_2, PullDown); // move
DigitalIn button2(PC_3, PullDown);

DigitalIn x_minus(PA_0, PullDown);
DigitalIn y_minus(PA_4, PullDown);
DigitalIn z_minus(PC_1, PullDown);

DigitalIn x_plus(PA_1, PullDown);
DigitalIn y_plus(PB_0, PullDown);
DigitalIn z_plus(PC_0, PullDown);

DigitalOut air(PA_10);

DigitalOut x_LED(PA_7);
DigitalOut y_LED(PB_6);
DigitalOut z_LED(PC_7);

DigitalIn button_push(PC_13, PullDown); // 押し込み

DigitalIn button_reset(PD_2, PullDown);
DigitalIn button_switch(PC_11, PullDown);

static inline bool pressed(DigitalIn &b) { return b.read() == 1; }

void home_position() {

    z.drive(0.0);
    x.drive(0.3);

    while (!pressed(x_minus)) { ThisThread::sleep_for(1ms); }

    x.drive(0.0);
    y.drive(-0.3);

    while (!pressed(y_minus)) { ThisThread::sleep_for(1ms); }

    y.drive(0.0);    
    air = 0;
};

void get() {
    z.drive(-0.2);
    wait_us(700 * 1000);
    z.drive(0.0);

    air = 1;
    wait_us(700 * 1000);
    z.drive(0.2);
}



int main() {
    x.drive(0.0f);
    y.drive(0.0f);

    while (true) {
        
        // === 1押されるのを待つ → 押してる間はxを動かす ===
        while (!pressed(button1)) { ThisThread::sleep_for(1ms); } // 1待ち
        ThisThread::sleep_for(15ms);                               // 押下デバウンス
        if (!pressed(button1)) continue;                           // ノイズならやり直し

        while (pressed(button1)) {                                 // 押してる間だけ
            x.drive(-0.3f);
            ThisThread::sleep_for(10ms);
        }
        x.drive(0.0f);                                             // 離したら止める
        ThisThread::sleep_for(20ms);                               // 離しデバウンス

        // === 2押されるのを待つ → 押してる間はyを動かす（この間は1は見ない）===
        while (!pressed(button2)) { ThisThread::sleep_for(1ms); }  // 2待ち
        ThisThread::sleep_for(15ms);
        if (!pressed(button2)) continue;

        while (pressed(button2)) {
            y.drive(0.3f);
            ThisThread::sleep_for(10ms);
        }
        y.drive(0.0f);
        ThisThread::sleep_for(20ms);

        get();

        wait_us(1000 * 1000);

        // === 条件達成 → 特定の動作を一回実行 ===
        home_position();


        // リミットスイッチチェック用printf

        // printf(
        //     "button1:%d  button2:%d  x_minus:%d  y_minus:%d  z_minus:%d  "
        //     "x_plus:%d  y_plus:%d  z_plus:%d  button_push:%d  button_reset:%d  button_switch:%d\r\n",
        //     button1.read(),
        //     button2.read(),
        //     x_minus.read(),
        //     y_minus.read(),
        //     z_minus.read(),
        //     x_plus.read(),
        //     y_plus.read(),
        //     z_plus.read(),
        //     button_push.read(),
        //     button_reset.read(),
        //     button_switch.read()
        // );

        ThisThread::sleep_for(10ms);



    }
}

