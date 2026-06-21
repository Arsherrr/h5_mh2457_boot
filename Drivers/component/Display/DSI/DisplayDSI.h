#ifndef __DISPLAY_DSI_H__
#define __DISPLAY_DSI_H__

#include "Display.h"

// Make DSI Packet
#define MakeDSIPacket(...)                                                                                                                                     \
    {                                                                                                                                                          \
        .DataType = __ARGC__(__VA_ARGS__) < 3 ? (__ARGC__(__VA_ARGS__) == 1 ? DSIDataTypeDCSShortWrite : DSIDataTypeDCSShortWrite1) : DSIDataTypeDCSLongWrite, \
        .Length   = __ARGC__(__VA_ARGS__),                                                                                                                     \
        .Payload  = (uint8_t*)(const uint8_t[]) {__VA_ARGS__},                                                                                                 \
    }

#define MakeDSIGenericPacket(...)                                                                                                                      \
    {                                                                                                                                                  \
        .DataType = __ARGC__(__VA_ARGS__) < 3 ? (__ARGC__(__VA_ARGS__) == 1 ? DSIDataTypeShortWrite1 : DSIDataTypeShortWrite2) : DSIDataTypeLongWrite, \
        .Length   = __ARGC__(__VA_ARGS__),                                                                                                             \
        .Payload  = (uint8_t*)(const uint8_t[]) {__VA_ARGS__},                                                                                         \
    }

#define MakeDSIDelay(delay)          \
    {                                \
        .DataType = DSIDataTypeNull, \
        .Length   = delay,           \
        .Payload  = NULL,            \
    }

#define MakeDSIPacketRead(length, command) \
    {                                      \
        .DataType = DSIDataTypeDCSRead,    \
        .Length   = length,                \
        .Command  = command,               \
    }

#define MakeDSIPacketEnd()          \
    {                               \
        .DataType = DSIDataTypeEnd, \
    }

extern uint32_t DisplayDSIReadID(DisplayStruct* display, const DSIPacketStruct* readSequence);

extern bool DisplayDSIConstractor(DisplayStruct* display, DSIStruct* dsiHandle);

extern bool DisplayDSIInit(DisplayStruct* display);

#endif
