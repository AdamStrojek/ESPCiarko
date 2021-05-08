#include "Arduino.h"
#include <IRremote.h>

#include "hob2hood_ir.h"
#include "config.h"


// IR commands from AEG hob2hood device
const unsigned long H2H_IR_SPEED[] = {
    0x055303A3l,
    0xE3C01BE2l,
    0xD051C301l,
    0xC22FFFD7l,
    0xB9121B29l,
};
const uint8_t H2H_IR_SPEED_LEN = 5;
const long H2H_IR_LIGHT_ON = 0xE208293C;
const long H2H_IR_LIGHT_OFF = 0x24ACF947;

IRrecv irrecv(PIN_IR_RECEIVER);
decode_results results;


void hob2hood_ir_init() {
    // Start the IR receiver
    irrecv.enableIRIn();
}

void hob2hood_ir_process(Hood &target) {
    // have we received an IR signal?
    if (irrecv.decode(&results)) {
        Serial.println("Received IR command: ");
        Serial.println(results.value, HEX); // display it on serial monitor in hexadecimal

        for(uint8_t i = 0; i < H2H_IR_SPEED_LEN; i++) {
            if(H2H_IR_SPEED[i] == results.value) {
                target.speed = i;
                goto RESUME;
            }
        }

        switch (results.value) {
        case H2H_IR_LIGHT_ON:
            target.light = 1;
            break;
        case H2H_IR_LIGHT_OFF:
            target.light = 0;
            break;
        }
RESUME:
        irrecv.resume(); // receive the next value
    }
}