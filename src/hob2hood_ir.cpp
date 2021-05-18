#include "Arduino.h"
#include <IRremote.h>

#include "hob2hood_ir.h"
#include "log.h"
#include "config.h"


// IR commands from AEG hob2hood device
const unsigned long H2H_IR_SPEED_0 = 0x055303A3ul;
const unsigned long H2H_IR_SPEED_1 = 0xE3C01BE2ul;
const unsigned long H2H_IR_SPEED_2 = 0xD051C301ul;
const unsigned long H2H_IR_SPEED_3 = 0xC22FFFD7ul;
const unsigned long H2H_IR_SPEED_4 = 0xB9121B29ul;
const unsigned long H2H_IR_LIGHT_ON = 0xE208293Cul;
const unsigned long H2H_IR_LIGHT_OFF = 0x24ACF947ul;

IRrecv irrecv(PIN_IR_RECEIVER);
decode_results results;


void hob2hood_ir_init() {
    // Start the IR receiver
    irrecv.enableIRIn();
}

void hob2hood_ir_halt() {
    irrecv.disableIRIn();
}

void hob2hood_ir_process(Hood &target) {
    // have we received an IR signal?
    if (irrecv.decode(&results)) {
        switch (results.value) {
        case H2H_IR_LIGHT_ON:
            target.light = 1;
            log_print("IR Light On");
            break;
        case H2H_IR_LIGHT_OFF:
            target.light = 0;
            log_print("IR Light Off");
            break;
        case H2H_IR_SPEED_0:
            target.speed = 0;
            log_print("IR Speed 0");
            break;
        case H2H_IR_SPEED_1:
            target.speed = 1;
            log_print("IR Speed 1");
            break;
        case H2H_IR_SPEED_2:
            target.speed = 2;
            log_print("IR Speed 2");
            break;
        case H2H_IR_SPEED_3:
            target.speed = 3;
            log_print("IR Speed 3");
            break;
        case H2H_IR_SPEED_4:
            target.speed = 4;
            log_print("IR Speed 4");
            break;
        default:
            log_print("IR Unknow command: 0x" + String(results.value, HEX));
            break;
        }
        irrecv.resume(); // receive the next value
    }
}