#include <ESPmDNS.h>
#include <WiFiUdp.h>

#include "log.h"
#include "config.h"

IPAddress sink;
WiFiUDP udp;

void log_init() {
#ifdef LOG_SINK_HOST
    sink = MDNS.queryHost(LOG_SINK_HOST, 60000);
#endif
}

void log_print(const String &output) {
#ifdef LOG_SINK_HOST
    uint8_t retries = 0;
    while(sink.toString() == "0.0.0.0" && retries < 5) {
        sink = MDNS.queryHost(LOG_SINK_HOST, 5000);
        retries++;
    }
    if(sink.toString() != "0.0.0.0") {
        udp.beginPacket(sink, LOG_SINK_PORT);
        udp.println(output);
        udp.endPacket();
    }
#endif
    Serial.println(output);
}
