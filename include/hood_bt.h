#ifndef HOOD_BT_H
#define HOOD_BT_H

#include "hood.h"

#define HOOD_CMD_MINUS 0
#define HOOD_CMD_PLUS 1
#define HOOD_CMD_LIGHT 2


void hood_bt_init();

void hood_bt_halt();

void hood_bt_process(Hood &current);

void hood_bt_send_command(uint8_t cmd);

#endif