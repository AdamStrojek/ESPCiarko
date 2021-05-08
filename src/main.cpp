#include "Arduino.h"

#include "hood_bt.h"
#include "hob2hood_ir.h"
#include "hood.h"


Hood current;
Hood target;


void setup() {
    Serial.begin(9600);

    Serial.println("Starting ESPCiarko Remote");
    hood_bt_init();

    hob2hood_ir_init();
}

void loop() {
    hood_bt_process(current);

    hob2hood_ir_process(target);

    if(target.speed != UNKNOW) {
        if(current.speed == target.speed) {
            target.speed = UNKNOW;
        } else if(current.speed > target.speed) {
            hood_bt_send_command(HOOD_CMD_MINUS);
        } else if(current.speed > target.speed) {
            hood_bt_send_command(HOOD_CMD_PLUS);
        }
    }

    if(target.light != UNKNOW) {
        if( current.light == target.light) {
            target.light = UNKNOW;
        } else {
            hood_bt_send_command(HOOD_CMD_LIGHT);
            current.light = target.light;
        }
    }

    Serial.printf("S: %d, T: %d\n", current.speed, target.speed);
    delay(20);
}
