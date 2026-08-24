#ifndef _AES_GCM_H_
#define _AES_GCM_H_

#include "include.h"

#define AES_GCM_KEY_BYTES   16
#define AES_GCM_NONCE_BYTES 12
#define AES_GCM_TAG_BYTES   16

typedef struct {
    u8 round_key[176];
    u8 h[16];
    u8 y[16];
    u8 ctr[16];
    u8 ghash[16];
    u8 stream[16];
    u32 stream_used;
    u32 aad_bits_hi;
    u32 aad_bits_lo;
    u32 text_bits_hi;
    u32 text_bits_lo;
} aes_gcm_ctx_t;

void aes_gcm_init(aes_gcm_ctx_t *ctx, const u8 key[AES_GCM_KEY_BYTES], const u8 nonce[AES_GCM_NONCE_BYTES]);
void aes_gcm_decrypt_update(aes_gcm_ctx_t *ctx, u8 *data, u32 len);
s8 aes_gcm_decrypt_finish(aes_gcm_ctx_t *ctx, const u8 tag[AES_GCM_TAG_BYTES]);

#endif
