#ifndef _REL_RESULTDLL_H
#define _REL_RESULTDLL_H

#include "dolphin.h"
#include "game/hu3d.h"

/* lbl_1_bss_488: one result row per player (0x58 each) */
typedef struct ResultPlayer_s {
    s32 rankEspId; /* 0x00 rank before the minigame */
    s32 rankNewEspId; /* 0x04 rank after the minigame */
    s32 charEspId; /* 0x08 bank = rank */
    s32 coinAddEspId; /* 0x0C */
    s32 coinAddNumEspId[3]; /* 0x10 */
    s32 starNumEspId[4]; /* 0x1C */
    s32 coinNumEspId[4]; /* 0x2C */
    s32 charMdlId; /* 0x3C */
    s32 charMotId[3]; /* 0x40 */
    s32 crownMdlId; /* 0x4C */
    s32 starMdlId; /* 0x50 */
    s32 coinMdlId; /* 0x54 */
} RESULT_PLAYER; /* size 0x58 */

typedef struct ResultParticleData_s {
    s16 time; /* 0x00 */
    s16 aliveF; /* 0x02 */
    s16 unk04; /* 0x04 */
    s16 cameraBit; /* 0x06 */
    HuVecF vel; /* 0x08 */
    HuVecF accel; /* 0x14 */
    float speedDecay; /* 0x20 */
    float colorIdx; /* 0x24 */
    float scaleBase; /* 0x28 */
    float scale; /* 0x2C */
    HuVecF rot; /* 0x30 */
    HuVecF pos; /* 0x3C */
    GXColor color; /* 0x48 */
    s16 animBank; /* 0x4C */
    s16 animNo; /* 0x4E */
    float animSpeed; /* 0x50 */
    float animTime; /* 0x54 */
    u8 dispF : 1; /* 0x58 */
    u8 pauseF : 1;
} RESULT_PARTICLEDATA; /* size 0x5C */

typedef struct ResultParticle_s RESULT_PARTICLE;
typedef void (*RESULT_PARTICLEHOOK)(HU3D_MODEL *modelP, RESULT_PARTICLE *particleP, Mtx mtx);

struct ResultParticle_s {
    s16 unk00;
    s16 unk02; /* 0x02 */
    HuVecF unk04;
    HuVecF unk10;
    void *work; /* 0x1C */
    u8 blendMode; /* 0x20 */
    u8 attr; /* 0x21 */
    s16 sortF; /* 0x22 */
    s16 modelId; /* 0x24 */
    s16 num; /* 0x26 */
    u32 count; /* 0x28 */
    u32 prevCounter; /* 0x2C */
    u32 prevCount; /* 0x30 */
    s32 tevColorIn[4]; /* 0x34 */
    s32 tevAlphaIn[4]; /* 0x44 */
    GXColor tevColor1; /* 0x54 */
    GXColor tevColor2; /* 0x58 */
    u32 dlSize; /* 0x5C */
    ANIMDATA *anim; /* 0x60 */
    HuVec2f *stTbl; /* 0x64 */
    RESULT_PARTICLEDATA *data; /* 0x68 */
    HuVecF *vtxBuf; /* 0x6C */
    HuVec2f *stBuf; /* 0x70 */
    GXColor *colorBuf; /* 0x74 */
    void *dlBuf; /* 0x78 */
    RESULT_PARTICLEHOOK hook; /* 0x7C */
}; /* size 0x80 */

#define RESULT_PARTICLE_ATTR_LOOP (1 << 0)
#define RESULT_PARTICLE_ATTR_STOPCNT (1 << 1)
#define RESULT_PARTICLE_ATTR_ANIM (1 << 3)
#define RESULT_PARTICLE_ATTR_3D (1 << 4)

#endif
