#include "Arduino.h"
#include "BluetoothSerial.h"

#include <Bounce2.h>

BluetoothSerial SerialBT;

uint8_t address[6]  = {0x00, 0x12, 0x6F, 0x9E, 0x4B, 0x99};

const char *pin = "1234";
bool connected;

uint8_t minus_payload[3] = {0xF0, 0x01, 0xC7};
uint8_t plus_payload[3] = {0xF0, 0x02, 0x25};
uint8_t timer_payload[3] = {0xF0, 0x03, 0x7B};
uint8_t light_payload[3] = {0xF0, 0x04, 0xF8};

uint8_t dot = 0;
uint8_t seven_segment = 0;

uint8_t level = 0;
uint8_t target_level = 255;
uint8_t light = 0;

const byte plus_pin = GPIO_NUM_22;
const byte minus_pin = GPIO_NUM_23;

Bounce bounce_plus = Bounce();
Bounce bounce_minus = Bounce();

void setup() {
  Serial.begin(9600);
  SerialBT.setPin(pin);
  SerialBT.begin("ESPCiarko", true);
  Serial.println("Starting ESPCiarko Remote");
  
  // // connect(address) is fast (upto 10 secs max), connect(name) is slow (upto 30 secs max) as it needs
  // // to resolve name to address first, but it allows to connect to different devices with the same name.
  // // Set CoreDebugLevel to Info to view devices bluetooth address and device names
  // // connected = SerialBT.connect(name);
  connected = SerialBT.connect(address);
  
  if(connected) {
    Serial.println("Connected Succesfully!");
  } else {
    while(!SerialBT.connected(10000)) {
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app."); 
    }
  }

  bounce_plus.attach( plus_pin, INPUT_PULLUP ); 
  bounce_plus.interval(5); // interval in ms
  bounce_minus.attach( minus_pin, INPUT_PULLUP ); 
  bounce_minus.interval(5); // interval in ms
}

void loop() {
  if (SerialBT.available() >= 4) {
    (void)SerialBT.read();
    uint8_t b = SerialBT.read();
    uint8_t c = SerialBT.read();
    (void)SerialBT.read();

    dot = b & 0b1000;
    seven_segment = ( b & 0b0111 ) << 4 | (c & 0b1111);
  
    switch (seven_segment)
    {
    case 0b00000000:
      light = 0;
      level = 0;
      break;
    case 0b01110111:
      light = 1;
      level = 0;
      break;
    case 0b01000001:
      level = 1;
      break;
    case 0b00111011:
      level = 2;
      break;
    case 0b01101011:
      level = 3;
      break;
    case 0b01001101:
      level = 4;
      break;
    }

    if( target_level > 4) {
      target_level = level;
    }

    Serial.printf("%d", level);

    if( dot ) {
      Serial.print(".");
    } else {
      Serial.print(" ");
    }

    Serial.printf(" T: %d\n", target_level);

    if( level < target_level ) {
      SerialBT.write(plus_payload, 3);
      // SerialBT.flush();
      Serial.println("PLUS+++++++++++++++++");
    } else if ( level > target_level ) {
      SerialBT.write(minus_payload, 3);
      // SerialBT.flush();
      Serial.println("MINUS----------------");
    }
  }

  bounce_plus.update();
  bounce_minus.update();

  if ( bounce_plus.changed() ) {
    int deboucedInput = bounce_plus.read();
    if ( deboucedInput == HIGH && target_level < 4 ) {
      target_level++;
    }
  }

  if ( bounce_minus.changed() ) {
    int deboucedInput = bounce_minus.read();
    if ( deboucedInput == HIGH && target_level > 0 ) {
      target_level--;
    }
  }

  delay(20);
}
