#include "aes_gcm.h"

#define AES_BLOCKLEN 16
#define AES_KEYLEN   16
#define AES_keyExpSize 176

static const u8 sbox[256] = {
  0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
  0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
  0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
  0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
  0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
  0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
  0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
  0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
  0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
  0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
  0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
  0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
  0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
  0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
  0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
  0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16 };
static const u8 Rcon[11] = {0x8d,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36};

static u8 xtime(u8 x) { return (u8)((x << 1) ^ (((x >> 7) & 1) * 0x1b)); }
static void add_round_key(u8 round, u8 *state, const u8 *round_key) { u8 i; for (i = 0; i < 16; i++) state[i] ^= round_key[(round * 16) + i]; }
static void sub_bytes(u8 *state) { u8 i; for (i = 0; i < 16; i++) state[i] = sbox[state[i]]; }
static void shift_rows(u8 *s) { u8 t; t=s[1];s[1]=s[5];s[5]=s[9];s[9]=s[13];s[13]=t; t=s[2];s[2]=s[10];s[10]=t;t=s[6];s[6]=s[14];s[14]=t; t=s[3];s[3]=s[15];s[15]=s[11];s[11]=s[7];s[7]=t; }
static void mix_columns(u8 *s) { u8 i,t,tmp,tm; for (i=0;i<4;i++){ t=s[i*4]; tmp=s[i*4]^s[i*4+1]^s[i*4+2]^s[i*4+3]; tm=s[i*4]^s[i*4+1]; tm=xtime(tm); s[i*4]^=tm^tmp; tm=s[i*4+1]^s[i*4+2]; tm=xtime(tm); s[i*4+1]^=tm^tmp; tm=s[i*4+2]^s[i*4+3]; tm=xtime(tm); s[i*4+2]^=tm^tmp; tm=s[i*4+3]^t; tm=xtime(tm); s[i*4+3]^=tm^tmp; }}

static void key_expansion(u8 *round_key, const u8 *key)
{
    u32 i; u8 t[4], k;
    memcpy(round_key, key, AES_KEYLEN);
    for (i = 4; i < 44; i++) {
        t[0]=round_key[(i-1)*4]; t[1]=round_key[(i-1)*4+1]; t[2]=round_key[(i-1)*4+2]; t[3]=round_key[(i-1)*4+3];
        if ((i % 4) == 0) { k=t[0]; t[0]=sbox[t[1]] ^ Rcon[i/4]; t[1]=sbox[t[2]]; t[2]=sbox[t[3]]; t[3]=sbox[k]; }
        round_key[i*4]=round_key[(i-4)*4]^t[0]; round_key[i*4+1]=round_key[(i-4)*4+1]^t[1]; round_key[i*4+2]=round_key[(i-4)*4+2]^t[2]; round_key[i*4+3]=round_key[(i-4)*4+3]^t[3];
    }
}

static void aes_encrypt_block(const u8 *round_key, u8 *buf)
{
    u8 round;
    add_round_key(0, buf, round_key);
    for (round = 1; round < 10; round++) { sub_bytes(buf); shift_rows(buf); mix_columns(buf); add_round_key(round, buf, round_key); }
    sub_bytes(buf); shift_rows(buf); add_round_key(10, buf, round_key);
}

static void inc32(u8 *x)
{
    u32 n = ((u32)x[12] << 24) | ((u32)x[13] << 16) | ((u32)x[14] << 8) | x[15];
    n++;
    x[12] = (u8)(n >> 24); x[13] = (u8)(n >> 16); x[14] = (u8)(n >> 8); x[15] = (u8)n;
}

static void xor_block(u8 *dst, const u8 *src) { u8 i; for (i = 0; i < 16; i++) dst[i] ^= src[i]; }

static void gcm_mult(u8 *x, const u8 *h)
{
    u8 z[16] = {0};
    u8 v[16];
    u16 i;
    memcpy(v, h, 16);
    for (i = 0; i < 128; i++) {
        if ((x[i >> 3] >> (7 - (i & 7))) & 1) xor_block(z, v);
        if (v[15] & 1) { u8 j, carry = 0; for (j = 0; j < 16; j++) { u8 nc = v[j] & 1; v[j] = (v[j] >> 1) | (carry << 7); carry = nc; } v[0] ^= 0xe1; }
        else { u8 j, carry = 0; for (j = 0; j < 16; j++) { u8 nc = v[j] & 1; v[j] = (v[j] >> 1) | (carry << 7); carry = nc; } }
    }
    memcpy(x, z, 16);
}

static void ghash_block(aes_gcm_ctx_t *ctx, const u8 *block)
{
    xor_block(ctx->ghash, block);
    gcm_mult(ctx->ghash, ctx->h);
}

static void add_bits(u32 *hi, u32 *lo, u32 bits)
{
    u32 old = *lo;
    *lo += bits;
    if (*lo < old) (*hi)++;
}

void aes_gcm_init(aes_gcm_ctx_t *ctx, const u8 key[AES_GCM_KEY_BYTES], const u8 nonce[AES_GCM_NONCE_BYTES])
{
    memset(ctx, 0, sizeof(*ctx));
    key_expansion(ctx->round_key, key);
    aes_encrypt_block(ctx->round_key, ctx->h);
    memcpy(ctx->y, nonce, AES_GCM_NONCE_BYTES);
    ctx->y[15] = 1;
    memcpy(ctx->ctr, ctx->y, 16);
    inc32(ctx->ctr);
    ctx->stream_used = AES_BLOCKLEN;
}

void aes_gcm_decrypt_update(aes_gcm_ctx_t *ctx, u8 *data, u32 len)
{
    u8 block[16];
    u32 i, n;
    while (len) {
        n = (len >= 16) ? 16 : len;
        memset(block, 0, 16);
        memcpy(block, data, n);
        ghash_block(ctx, block);
        for (i = 0; i < n; i++) {
            if (ctx->stream_used >= AES_BLOCKLEN) { memcpy(ctx->stream, ctx->ctr, 16); aes_encrypt_block(ctx->round_key, ctx->stream); inc32(ctx->ctr); ctx->stream_used = 0; }
            data[i] ^= ctx->stream[ctx->stream_used++];
        }
        add_bits(&ctx->text_bits_hi, &ctx->text_bits_lo, n * 8);
        data += n;
        len -= n;
    }
}

s8 aes_gcm_decrypt_finish(aes_gcm_ctx_t *ctx, const u8 tag[AES_GCM_TAG_BYTES])
{
    u8 s[16];
    u8 calc[16];
    u8 diff = 0;
    u8 i;

    memset(s, 0, sizeof(s));
    s[4] = (u8)(ctx->aad_bits_hi >> 24); s[5] = (u8)(ctx->aad_bits_hi >> 16); s[6] = (u8)(ctx->aad_bits_hi >> 8); s[7] = (u8)ctx->aad_bits_hi;
    s[8] = (u8)(ctx->text_bits_hi >> 24); s[9] = (u8)(ctx->text_bits_hi >> 16); s[10] = (u8)(ctx->text_bits_hi >> 8); s[11] = (u8)ctx->text_bits_hi;
    s[12] = (u8)(ctx->text_bits_lo >> 24); s[13] = (u8)(ctx->text_bits_lo >> 16); s[14] = (u8)(ctx->text_bits_lo >> 8); s[15] = (u8)ctx->text_bits_lo;
    ghash_block(ctx, s);
    memcpy(calc, ctx->y, 16);
    aes_encrypt_block(ctx->round_key, calc);
    xor_block(calc, ctx->ghash);

    for (i = 0; i < AES_GCM_TAG_BYTES; i++) diff |= (u8)(calc[i] ^ tag[i]);
    return diff ? -1 : 0;
}
