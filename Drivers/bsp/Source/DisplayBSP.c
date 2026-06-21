#include "DisplayBSP.h"

DisplayStruct* DisplaySelectSetup(uint32_t* buffer, uint32_t size) {
    if (DSIDisplaySetup(buffer, size))
        return &DSIDisplay;

    return NULL;
}
