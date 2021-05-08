#ifndef HOOD_H
#define HOOD_H

#include <stdint.h>

#define UNKNOW 255

typedef struct hood_struct
{
    uint8_t speed = UNKNOW;
    uint8_t light = UNKNOW;
} Hood;

#endif