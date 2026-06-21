#ifndef RES_H_
#define RES_H_

#include <stdint.h>

typedef struct {
    uint32_t id;
    uint32_t offset;
    uint32_t size;
    uint16_t width;
    uint16_t height;
    uint8_t  format;
    const char *source_path;
} jl_resource_info_t;

#define JL_RESOURCE_COUNT 119
#define JL_RESOURCE_BIN_PATH "res.bin"

int perfect_hash(const char* key);

extern const jl_resource_info_t g_jl_resources[JL_RESOURCE_COUNT];

#endif /* RES_H_ */
