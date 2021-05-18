#include <Arduino.h>
#include <BluetoothSerial.h>
#include <CircularBuffer.h>

#include "hood_bt.h"
#include "log.h"
#include "config.h"


BluetoothSerial SerialBT;
uint8_t address[6] = HOOD_BT_ADDRESS;
const char *pin = HOOD_BT_PIN;
bool connected;

uint8_t bt_payload[4][3] = {
    {0xF0, 0x01, 0xC7},  // MINUS
    {0xF0, 0x02, 0x25},  // PLUS
    {0xF0, 0x04, 0xF8},  // LIGHT
    {0xF0, 0x03, 0x7B}   // TIMER
};

uint8_t dot = 0;
uint8_t seven_segment = 0;
uint8_t prev = 0;

const uint8_t SEVEN_SEGMENT_BLANK = 0b0000000;
const uint8_t SEVEN_SEGMENT_NUM[] = {
    0b1110111,  // 0
    0b1000001,  // 1
    0b0111011,  // 2
    0b1101011,  // 3
    0b1001101,  // 4
};
const uint8_t SEVEN_SEGMENT_LEN = 5;

CircularBuffer<uint8_t,10> cmds_queue;

void hood_bt_init() {
    SerialBT.setPin(pin);
    SerialBT.begin("ESPCiarko", true);

    // // connect(address) is fast (upto 10 secs max), connect(name) is slow (upto 30 secs max) as it needs
    // // to resolve name to address first, but it allows to connect to different devices with the same name.
    // // Set CoreDebugLevel to Info to view devices bluetooth address and device names
    // // connected = SerialBT.connect(name);
    connected = SerialBT.connect(address);
  
    if(connected) {
        log_print("BT: Connected Succesfully!");
    } else {
        while(!SerialBT.connected(10000)) {
            log_print("BT: Failed to connect. Make sure remote device is available and in range, then restart app."); 
        }
    }
}

void hood_bt_halt() {
    SerialBT.disconnect();
}

void hood_bt_process(Hood &current) {
    if (SerialBT.available() >= 4) {
        (void)SerialBT.read();  // Packet type? Always 0x00
        seven_segment = SerialBT.read() << 4;  // first 4bits of 7 segment display
        seven_segment |= SerialBT.read();  // second 4bits of 7 segment display
        (void)SerialBT.read();  // CRC8?

        if( seven_segment != prev ) {
            prev = seven_segment;
            dot = seven_segment & 0b10000000;
            seven_segment &= 0b01111111;

            if( seven_segment == SEVEN_SEGMENT_BLANK) {
                current.speed = 0;
                current.light = 0;
            } else {
                for(uint8_t i = 0; i < SEVEN_SEGMENT_LEN; i++) {
                    if(seven_segment == SEVEN_SEGMENT_NUM[i]) {
                        current.speed = i;
                    }
                }

                if( current.speed == 0 ) {
                    current.light = 1;
                }
            }
        }

        if(!cmds_queue.isEmpty()) {
            SerialBT.write(bt_payload[cmds_queue.shift()], 3);
        }
    }
}

void hood_bt_send_command(uint8_t cmd) {
    log_print("Queue command " + String(cmd, 10));
    cmds_queue.push(cmd);
}
