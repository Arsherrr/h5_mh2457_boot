#ifndef __DISPLAY_DBI_H__
#define __DISPLAY_DBI_H__

typedef struct DBIPacketStruct_s DBIPacketStruct;

#include "Display.h"

// Make DBI Packet
#define MakeDBIPacket(command, ...)                           \
    {                                                         \
        .Command = command,                                   \
        .Length  = __ARGC__(__VA_ARGS__),                     \
        .Payload = (uint8_t*)(const uint8_t[]) {__VA_ARGS__}, \
    }

#define MakeDBIPacketRead(command, dummy, length) \
    {                                             \
        .IsRead  = true,                          \
        .Command = command,                       \
        .Dummy   = dummy,                         \
        .Length  = length,                        \
    }

#define MakeDBIDelay(delay) \
    {                       \
        .IsDelay = true,    \
        .Length  = delay,   \
    }

#define MakeDBIPacketEnd() \
    {                      \
        .IsEnd = true,     \
    }

typedef struct DBIPacketStruct_s {
    uint16_t Command : 8;

    uint8_t Dummy : 4;

    bool IsRead : 1;
    bool IsDelay : 1;
    bool IsEnd : 1;

    uint16_t Length;
    uint8_t* Payload;
} DBIPacketStruct;

extern uint32_t DisplayDBIReadID(DisplayStruct* display, DBIPacketStruct* readSequence);

extern bool DisplayDBIConstractor(DisplayStruct* display);

#endif
