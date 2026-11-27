#ifndef _GAME_BOARD_EFFECT_H
#define _GAME_BOARD_EFFECT_H

#include "game/board/board.h"
#include "game/animdata.h"

#define MB_PARTICLE_BLEND_NORMAL 0
#define MB_PARTICLE_BLEND_ADDCOL 1
#define MB_PARTICLE_BLEND_INVCOL 2

#define MB_PARTICLE_ATTR_LOOP (1 << 0)
#define MB_PARTICLE_ATTR_STOPCNT (1 << 1)
#define MB_PARTICLE_ATTR_ANIM (1 << 3)
#define MB_PARTICLE_ATTR_3D (1 << 4)

typedef struct mbParticleData_s {
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
    HuVecF rot; /* 0x30: rot.z is the screen rotation of a 2D particle */
    HuVecF pos; /* 0x3C */
    GXColor color; /* 0x48 */
    s16 animBank; /* 0x4C */
    s16 animNo; /* 0x4E */
    float animSpeed; /* 0x50 */
    float animTime; /* 0x54 */
    u8 dispF; /* 0x58 */
    u8 pauseF; /* 0x59 */
} MBPARTICLEDATA; /* size 0x5C */

typedef struct mbParticle_s MBPARTICLE;
typedef void (*MBPARTICLEHOOK)(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);

struct mbParticle_s {
    s16 unk00; /* 0x00: board hooks use it as their init state */
    u8 unk02[0x1A];
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
    HuVec2f *stTbl; /* 0x64: four st pairs per animation pattern */
    MBPARTICLEDATA *data; /* 0x68 */
    HuVecF *vtxBuf; /* 0x6C */
    HuVec2f *stBuf; /* 0x70 */
    GXColor *colorBuf; /* 0x74 */
    void *dlBuf; /* 0x78 */
    MBPARTICLEHOOK hook; /* 0x7C */
}; /* size 0x80 */

typedef struct mbParManParam_s {
    s16 maxTime; /* 0x00 */
    s16 unk2;
    float accelRange; /* 0x04 */
    float scaleRange; /* 0x08 */
    float angleRange; /* 0x0C */
    HuVecF gravity; /* 0x10 */
    float speedBase; /* 0x1C */
    float speedDecay; /* 0x20 */
    float scaleBase; /* 0x24 */
    float scaleDecay; /* 0x28 */
    s16 colorNum; /* 0x2C */
    GXColor colorStart[4]; /* 0x2E */
    GXColor colorEnd[4]; /* 0x3E */
} MBPARMANPARAM;

typedef struct mbParMan_s {
    MBPARTICLE particle; /* 0x00 */
    s16 modelId; /* 0x80 */
    s16 attr; /* 0x82 */
    s16 timeLimit; /* 0x84 */
    s16 unk86;
    s16 color; /* 0x88 */
    HuVecF pos; /* 0x8C */
    HuVecF vec; /* 0x98 */
    HuVecF vacuum; /* 0xA4 */
    float vacuumSpeed; /* 0xB0 */
    float accel; /* 0xB4 */
    s32 jitterNo; /* 0xB8 */
    MBPARMANPARAM *param; /* 0xBC */
} MBPARMAN; /* size 0xC0 */

#define MB_PARMAN_ATTR_TIMEUP (1 << 0)
#define MB_PARMAN_ATTR_RANDSPEED90 (1 << 1)
#define MB_PARMAN_ATTR_RANDSPEED70 (1 << 2)
#define MB_PARMAN_ATTR_RANDSPEED100 (1 << 3)
#define MB_PARMAN_ATTR_RANDSCALE90 (1 << 4)
#define MB_PARMAN_ATTR_RANDSCALE70 (1 << 5)
#define MB_PARMAN_ATTR_SCALEJITTER (1 << 6)
#define MB_PARMAN_ATTR_PAUSE (1 << 7)
#define MB_PARMAN_ATTR_RANDANGLE (1 << 8)
#define MB_PARMAN_ATTR_VACUUM (1 << 9)
#define MB_PARMAN_ATTR_RANDTIME90 (1 << 10)
#define MB_PARMAN_ATTR_RANDTIME70 (1 << 11)
#define MB_PARMAN_ATTR_SETCOLOR (1 << 12)

void mbEffInit(void);
void mbEffClose(void);
void mbEffFadeOutSet(s16 maxTime);
void mbEffFadeCreate(s16 maxTime, u8 alpha);
BOOL mbEffFadeDoneCheck(void);
void mbEffFadeCameraSet(u16 bit);
BOOL mbEffFadeCheck(void);
void mbEffConfettiCreate(HuVecF *pos, s16 maxCnt, float width);
void mbEffConfettiKill(void);
void mbEffConfettiReset(void);
HU3D_MODELID mbParticleCreate(ANIMDATA *anim, s16 maxCnt);
void mbParticleKill(HU3D_MODELID modelId);
void mbParticleColorCreate(HU3D_MODELID modelId);
void mbParticleHookSet(HU3D_MODELID modelId, void *hook);
void mbParticleAttrSet(HU3D_MODELID modelId, u8 attr);
void mbParticleAttrReset(HU3D_MODELID modelId, u8 attr);
MBPARTICLEDATA *mbParticleDataCreate(MBPARTICLE *particleP);
int mbParticleUnkTotalGet(void *ptr, int no);
HU3D_MODELID mbParManCreate(ANIMDATA *anim, s16 maxCnt, MBPARMANPARAM *param);
void mbParManKill(HU3D_MODELID modelId);
void mbParticleBlendModeSet(HU3D_MODELID modelId, int blendMode);
void mbParManPosSet(int modelId, float x, float y, float z);
void mbParManVecSet(HU3D_MODELID modelId, float x, float y, float z);
void mbParManRotSet(HU3D_MODELID modelId, float x, float y, float z);
void mbParManAttrSet(HU3D_MODELID modelId, int attr);
void mbParManAttrReset(HU3D_MODELID modelId, int attr);

#endif
