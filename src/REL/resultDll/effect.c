/* Retail's later resultDll files do not carry math.h's sqrtf statics in .rodata:
 * keep include/math.h out and declare what this file uses. */
#define _MATH_H
#define M_PI 3.141592653589793
double sin(double x);
double cos(double x);
float sqrtf(float x);
double atan2(double y, double x);
#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/frand.h"
/* GWPlayerStarGet is called with an int result (no extsh) */
#define GWPlayerStarGet GWPlayerStarGet_proto
#include "game/gamework.h"
#undef GWPlayerStarGet
s32 GWPlayerStarGet(GW_PLAYER_ID playerNo);
#include "game/hu3d.h"
#include "game/init.h"
#include "game/main.h"
#include "game/memory.h"
#include "game/mgdata.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/saveload.h"
#include "game/sprite.h"
#include "game/window.h"
#include "game/wipe.h"
#include "humath.h"
#include "string.h"

#include "dolphin.h"

#include "datadir_enum.h"
#include "REL/resultDll.h"

/* The esp* sprite calls (game/esprite.h) and CharFXPlay/CharFXStop are called
 * unprototyped: their headers stay out of this unit. */

/* this file */
void *fn_1_AB3C(s32 size);
void *fn_1_AB6C(s32 size, u32 num);
void *fn_1_ABA0(s32 size, u32 num);
void *fn_1_ABFC(s32 size, u32 num);
void fn_1_AC54(void);
void fn_1_ACB8(void);
HU3D_MODELID fn_1_AD24(ANIMDATA *anim, s16 maxCnt);
void fn_1_B39C(HU3D_MODELID modelId);
void fn_1_C420(HU3D_MODELID modelId, RESULT_PARTICLEHOOK hook);
s32 fn_1_C45C(s32 dataNum);

/* other files of resultDll */


#pragma dont_inline off
#pragma auto_inline on

typedef struct ResultParticleSort_s {
    s32 key;
    s32 idx;
} RESULT_PARTICLESORT;

/* .bss: defined at the end of the file (after the function statics) */
extern s32 lbl_1_bss_14AC;
extern RESULT_PARTICLESORT *lbl_1_bss_14A8;

void *fn_1_AB3C(s32 size)
{
    return HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
}

void *fn_1_AB6C(s32 size, u32 num)
{
    return HuMemDirectMallocNum(HEAP_MODEL, size, num);
}

void *fn_1_ABA0(s32 size, u32 num)
{
    void *ptr = HuMemDirectMallocNum(HEAP_MODEL, size, num);
    memset(ptr, 0, size);
    return ptr;
}

void *fn_1_ABFC(s32 size, u32 num)
{
    void *ptr = HuMemDirectMallocNum(HEAP_MODEL, size, num);
    DCFlushRange(ptr, size);
    return ptr;
}

static inline void fn_1_ResultFree(void *ptr)
{
    HuMemDirectFree(ptr);
}

void fn_1_AC54(void)
{
    lbl_1_bss_14A8 = fn_1_AB3C(0x4000);
    lbl_1_bss_14AC = 0;
    OSReport("------------------ResultEffectInit-------------------\n");
}

void fn_1_ACB8(void)
{
    if(lbl_1_bss_14A8) {
        fn_1_ResultFree(lbl_1_bss_14A8);
        lbl_1_bss_14A8 = NULL;
    }
    OSReport("------------------ResultEffectClose-------------------\n");
}

static void fn_1_B5D8(HU3D_MODEL *modelP, Mtx *mtx);

HU3D_MODELID fn_1_AD24(ANIMDATA *anim, s16 maxCnt)
{
    HU3D_MODELID modelId;
    HU3D_MODEL *modelP;
    RESULT_PARTICLE *particleP;
    RESULT_PARTICLEDATA *dataP;
    HuVec2f *stP;
    ANIMLAYER *layerP;
    ANIMBMP *bmpP;
    s16 bmpFmt;
    s16 i;
    int vtxNo;
    float invW, invH;
    void *dlBuf;
    HuVecF *vtxBuf;

    modelId = Hu3DHookFuncCreate(fn_1_B5D8);
    Hu3DModelCameraSet(modelId, HU3D_CAM0);
    modelP = &Hu3DData[modelId];
    modelP->hookData = particleP = fn_1_ABA0(sizeof(RESULT_PARTICLE), modelP->mallocNo);
    particleP->anim = anim;
    anim->useNum++;
    particleP->num = maxCnt;
    particleP->blendMode = 0;
    particleP->prevCounter = -1;
    particleP->modelId = modelId;
    bmpFmt = particleP->anim->bmp->dataFmt & 0xF;
    if(bmpFmt == ANIM_BMP_I8 || bmpFmt == ANIM_BMP_I4) {
        particleP->tevColorIn[0] = GX_CC_ZERO;
        particleP->tevColorIn[1] = GX_CC_ONE;
        particleP->tevColorIn[2] = GX_CC_RASC;
        particleP->tevColorIn[3] = GX_CC_ZERO;
    } else {
        particleP->tevColorIn[0] = GX_CC_ZERO;
        particleP->tevColorIn[1] = GX_CC_TEXC;
        particleP->tevColorIn[2] = GX_CC_RASC;
        particleP->tevColorIn[3] = GX_CC_ZERO;
    }
    particleP->tevAlphaIn[0] = GX_CA_ZERO;
    particleP->tevAlphaIn[1] = GX_CA_TEXA;
    particleP->tevAlphaIn[2] = GX_CA_RASA;
    particleP->tevAlphaIn[3] = GX_CA_ZERO;
    particleP->data = dataP = fn_1_ABA0(maxCnt*sizeof(RESULT_PARTICLEDATA), modelP->mallocNo);
    for(i=0; i<maxCnt; i++, dataP++) {
        dataP->cameraBit = HU3D_CAM_ALL;
        *(u32 *)&dataP->color = 0xFFFFFFFF;
        dataP->dispF = TRUE;
    }
    particleP->vtxBuf = vtxBuf = fn_1_AB6C(maxCnt*sizeof(HuVecF)*4, modelP->mallocNo);
    particleP->stBuf = stP = fn_1_AB6C(maxCnt*sizeof(HuVec2f)*4, modelP->mallocNo);
    particleP->stTbl = fn_1_AB6C(anim->patNum*sizeof(HuVec2f)*4, modelP->mallocNo);
    for(i=0; i<anim->patNum; i++) {
        layerP = anim->pat[i].layer;
        bmpP = &anim->bmp[layerP->bmpNo];
        invW = 1.0f/bmpP->sizeX;
        invH = 1.0f/bmpP->sizeY;
        stP = &particleP->stTbl[i*4];
        stP[0].x = invW*(layerP->startX+layerP->vtx[0]);
        stP[0].y = invH*(layerP->startY+layerP->vtx[1]);
        stP[1].x = invW*(layerP->startX+layerP->vtx[2]);
        stP[1].y = invH*(layerP->startY+layerP->vtx[3]);
        stP[2].x = invW*(layerP->startX+layerP->vtx[4]);
        stP[2].y = invH*(layerP->startY+layerP->vtx[5]);
        stP[3].x = invW*(layerP->startX+layerP->vtx[6]);
        stP[3].y = invH*(layerP->startY+layerP->vtx[7]);
    }
    particleP->dlBuf = dlBuf = fn_1_ABFC((maxCnt*96)+128, modelP->mallocNo);
    GXBeginDisplayList(dlBuf, 0x20000);
    GXBegin(GX_QUADS, GX_VTXFMT0, maxCnt*4);
    for(i=0; i<maxCnt; i++) {
        vtxNo = i*4;
        GXPosition1x16(vtxNo);
        GXColor1x16(i);
        GXTexCoord1x16(vtxNo);
        GXPosition1x16(vtxNo+1);
        GXColor1x16(i);
        GXTexCoord1x16(vtxNo+1);
        GXPosition1x16(vtxNo+2);
        GXColor1x16(i);
        GXTexCoord1x16(vtxNo+2);
        GXPosition1x16(vtxNo+3);
        GXColor1x16(i);
        GXTexCoord1x16(vtxNo+3);
    }
    particleP->dlSize = GXEndDisplayList();
    return modelId;
}

void fn_1_B39C(HU3D_MODELID modelId)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    RESULT_PARTICLE *particleP = modelP->hookData;
    HuSprAnimKill(particleP->anim);
    HuMemDirectFreeNum(HEAP_MODEL, modelP->mallocNo);
    modelP->hsf = NULL;
}

static void fn_1_B404(RESULT_PARTICLESORT *buf, int num)
{
    static int effColorNum[32];
    static int effColorNo[32];
    int lo, hi, sp;
    int i, j;
    s32 pivot;
    s32 key, idx;
    RESULT_PARTICLESORT *pi, *pj;
    lo = 0;
    hi = num-1;
    sp = 0;
    while(1) {
        if(hi-lo <= 10) {
            if(sp == 0) {
                break;
            }
            sp--;
            lo = effColorNum[sp];
            hi = effColorNo[sp];
        }
        pivot = buf[(lo+hi) >> 1].key;
        i = lo;
        j = hi;
        pi = &buf[lo];
        pj = &buf[hi];
        while(1) {
            while(pi->key < pivot) {
                i++;
                pi++;
            }
            while(pivot < pj->key) {
                j--;
                pj--;
            }
            if(i >= j) {
                break;
            }
            idx = pj->idx;
            pj->idx = pi->idx;
            pi->idx = idx;
            key = pj->key;
            pj->key = pi->key;
            pi->key = key;
            i++;
            j--;
            pi++;
            pj--;
        }
        if(i-lo > hi-j) {
            if(i-lo > 10) {
                effColorNum[sp] = lo;
                effColorNo[sp] = i-1;
                sp++;
            }
            lo = j+1;
        } else {
            if(hi-j > 10) {
                effColorNum[sp] = j+1;
                effColorNo[sp] = hi;
                sp++;
            }
            hi = i-1;
        }
    }
    for(i=1; i<num; i++) {
        pi = &buf[i];
        key = pi->key;
        idx = pi->idx;
        for(j=i-1, pj=pi-1; j>=0 && buf[j].key > key; j--, pj--) {
            pj[1] = pj[0];
        }
        pj[1].key = key;
        pj[1].idx = idx;
    }
}

static inline void ParticleVtxSet(register HuVecF *dst, HuVecF *srcIn, HuVecF *posIn, float scaleIn)
{
    float posBuf[4];
    float scaleBuf[2];
    register HuVecF *src;
    register float *pos;
    register float *scale;
    register float pxy, pzx, pyz, sc;
    register float v0, v1, v2;
    posBuf[0] = posBuf[3] = posIn->x;
    posBuf[1] = posIn->y;
    posBuf[2] = posIn->z;
    scaleBuf[0] = scaleBuf[1] = scaleIn;
    src = srcIn;
    pos = posBuf;
    scale = scaleBuf;
    asm {
        psq_l sc, 0(scale), 0, 0
        psq_l pxy, 0(pos), 0, 0
        psq_l pzx, 8(pos), 0, 0
        psq_l pyz, 4(pos), 0, 0
        psq_l v0, 0(src), 0, 0
        psq_l v1, 8(src), 0, 0
        psq_l v2, 16(src), 0, 0
        ps_madd v0, v0, sc, pxy
        ps_madd v1, v1, sc, pzx
        ps_madd v2, v2, sc, pyz
        psq_st v0, 0(dst), 0, 0
        psq_st v1, 8(dst), 0, 0
        psq_st v2, 16(dst), 0, 0
        psq_l v0, 24(src), 0, 0
        psq_l v1, 32(src), 0, 0
        psq_l v2, 40(src), 0, 0
        ps_madd v0, v0, sc, pxy
        ps_madd v1, v1, sc, pzx
        ps_madd v2, v2, sc, pyz
        psq_st v0, 24(dst), 0, 0
        psq_st v1, 32(dst), 0, 0
        psq_st v2, 40(dst), 0, 0
    }
}

static inline void ParticleVtxClear(register float *zero, register HuVecF *dst)
{
    register float z;
    asm {
        psq_l z, 0(zero), 0, 0
        psq_st z, 0(dst), 0, 0
        psq_st z, 8(dst), 0, 0
        psq_st z, 16(dst), 0, 0
        psq_st z, 24(dst), 0, 0
        psq_st z, 32(dst), 0, 0
        psq_st z, 40(dst), 0, 0
    }
}

static inline void ParticleSTCopy(register HuVec2f *src, register HuVec2f *dst)
{
    register float st0, st1, st2, st3;
    asm {
        psq_l st0, 0(src), 0, 0
        psq_l st1, 8(src), 0, 0
        psq_l st2, 16(src), 0, 0
        psq_l st3, 24(src), 0, 0
        psq_st st0, 0(dst), 0, 0
        psq_st st1, 8(dst), 0, 0
        psq_st st2, 16(dst), 0, 0
        psq_st st3, 24(dst), 0, 0
    }
}

static inline float ParticleDepthGet(float dirX, float dirY, float dirZ, float vz, float vy, float vx)
{
    vx *= dirX;
    vy *= dirY;
    vz *= dirZ;
    return vx+vy+vz;
}

static void fn_1_B5D8(HU3D_MODEL *modelP, Mtx *mtx)
{
    static float nullVec[2] = { 0.0f, 0.0f };
    static Vec basePos[] = {
        { -0.5f,  0.5f, 0.0f },
        {  0.5f,  0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f },
        { -0.5f, -0.5f, 0.0f }
    };
    RESULT_PARTICLE *particleP;
    RESULT_PARTICLEDATA *dataP;
    s32 i;
    ANIMBANK *animBank;
    ANIMFRAME *animFrame;
    Mtx mtxInv;
    Mtx mtxRotate;
    HuVecF tempVtx[4];
    HuVecF initVtx[4];
    ROMtx basePosMtx;
    HuVecF *vtxBuf;
    u32 j;
    RESULT_PARTICLEHOOK hook;
    ANIMDATA *anim;
    BOOL flag3D;
    HuVec2f *stBuf;
    RESULT_PARTICLESORT *sortP;
    float x, y, z;
    float depth;
    s32 key;
    int num;
    HuVecF *vtxSrc;

    particleP = modelP->hookData;
    anim = particleP->anim;
    if(HmfInverseMtxF3X3(*mtx, mtxInv) == FALSE) {
        PSMTXIdentity(mtxInv);
    }
    PSMTXReorder(mtxInv, basePosMtx);
    if(Hu3DPauseF == FALSE || (modelP->attr & HU3D_ATTR_NOPAUSE)) {
        if(particleP->hook && particleP->prevCounter != GlobalCounter) {
            hook = particleP->hook;
            hook(modelP, particleP, *mtx);
        }
    } else if(particleP->prevCounter == -1) {
        return;
    }
    if(particleP->sortF && particleP->colorBuf) {
        num = particleP->num;
        x = mtxInv[0][2];
        y = mtxInv[1][2];
        z = mtxInv[2][2];
        dataP = particleP->data;
        sortP = lbl_1_bss_14A8;
        for(i=0; i<num; i++, dataP++, sortP++) {
            depth = ParticleDepthGet(x, y, z, dataP->pos.z, dataP->pos.y, dataP->pos.x);
            key = *(s32 *)&depth;
            if(key < 0) {
                key = -(key & 0x7FFFFFFF);
            }
            sortP->key = key;
            sortP->idx = i;
        }
        fn_1_B404(lbl_1_bss_14A8, num);
    } else {
        particleP->sortF = FALSE;
    }
    dataP = particleP->data;
    vtxBuf = particleP->vtxBuf;
    stBuf = particleP->stBuf;
    PSMTXROMultVecArray(basePosMtx, &basePos[0], initVtx, 4);
    flag3D = FALSE;
    if(particleP->attr & RESULT_PARTICLE_ATTR_3D) {
        flag3D = TRUE;
    }
    for(i=0; i<particleP->num; i++, dataP++) {
        if(particleP->sortF) {
            dataP = &particleP->data[lbl_1_bss_14A8[i].idx];
            particleP->colorBuf[i] = dataP->color;
        }
        if(dataP->scale && (dataP->cameraBit & Hu3DCameraBit) && dataP->dispF) {
            if(!flag3D) {
                if(0.0f == dataP->rot.z) {
                    vtxSrc = initVtx;
                } else {
                    static Vec basePos[] = {
                        { -0.5f,  0.5f, 0.0f },
                        {  0.5f,  0.5f, 0.0f },
                        {  0.5f, -0.5f, 0.0f },
                        { -0.5f, -0.5f, 0.0f }
                    };
                    float s = 0.5*HuSin(dataP->rot.z);
                    float c = 0.5*HuCos(dataP->rot.z);
                    HuVecF *rotPos = basePos;
                    rotPos[0].x = rotPos[3].y = -s-c;
                    rotPos[0].y = rotPos[1].x = -s+c;
                    rotPos[1].y = rotPos[2].x = s+c;
                    rotPos[2].y = rotPos[3].x = s-c;
                    PSMTXROMultVecArray(basePosMtx, rotPos, tempVtx, 4);
                    vtxSrc = tempVtx;
                }
            } else {
                if(0.0f == dataP->rot.x && 0.0f == dataP->rot.y && 0.0f == dataP->rot.z) {
                    vtxSrc = basePos;
                } else {
                    mtxRot(mtxRotate, dataP->rot.x, dataP->rot.y, dataP->rot.z);
                    PSMTXMultVecArray(mtxRotate, &basePos[0], tempVtx, 4);
                    vtxSrc = tempVtx;
                }
            }
            ParticleVtxSet(vtxBuf, vtxSrc, &dataP->pos, dataP->scale);
        } else {
            ParticleVtxClear(nullVec, vtxBuf);
        }
        vtxBuf += 4;
        animBank = &anim->bank[dataP->animBank];
        animFrame = &animBank->frame[dataP->animNo];
        if((particleP->attr & RESULT_PARTICLE_ATTR_ANIM) && !dataP->pauseF) {
            if(Hu3DPauseF == FALSE || (modelP->attr & HU3D_ATTR_NOPAUSE)) {
                for(j=0; j<(s32)dataP->animSpeed*minimumVcount; j++) {
                    dataP->animTime += 1.0f;
                    if(dataP->animTime >= animFrame->time) {
                        dataP->animNo++;
                        dataP->animTime -= animFrame->time;
                        animFrame = &animBank->frame[dataP->animNo];
                        if(dataP->animNo >= animBank->timeNum || animFrame->time == -1) {
                            if(!(particleP->attr & RESULT_PARTICLE_ATTR_LOOP)) {
                                dataP->dispF = FALSE;
                                dataP->pauseF = TRUE;
                                dataP->animNo = animBank->timeNum-1;
                            } else {
                                dataP->animNo = 0;
                            }
                        }
                    }
                }
                dataP->animTime += (dataP->animSpeed*minimumVcount)-(s32)j;
                if(dataP->animTime >= animFrame->time) {
                    dataP->animNo++;
                    dataP->animTime -= animFrame->time;
                    animFrame = &animBank->frame[dataP->animNo];
                    if(dataP->animNo >= animBank->timeNum || animFrame->time == -1) {
                        if(!(particleP->attr & RESULT_PARTICLE_ATTR_LOOP)) {
                            dataP->dispF = FALSE;
                            dataP->pauseF = TRUE;
                            dataP->animNo = animBank->timeNum-1;
                        } else {
                            dataP->animNo = 0;
                        }
                    }
                }
            }
        }
        ParticleSTCopy(&particleP->stTbl[anim->bank[dataP->animBank].frame[dataP->animNo].pat*4], stBuf);
        stBuf += 4;
    }
    DCFlushRangeNoSync(particleP->vtxBuf, particleP->num*sizeof(HuVecF)*4);
    if(particleP->sortF) {
        DCFlushRangeNoSync(particleP->colorBuf, particleP->num*sizeof(GXColor));
    } else {
        DCFlushRangeNoSync(particleP->data, particleP->num*sizeof(RESULT_PARTICLEDATA));
    }
    DCFlushRange(particleP->stBuf, particleP->num*sizeof(HuVec2f)*4);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, particleP->tevAlphaIn[0], particleP->tevAlphaIn[1], particleP->tevAlphaIn[2], particleP->tevAlphaIn[3]);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetZCompLoc(0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetArray(GX_VA_POS, particleP->vtxBuf, sizeof(HuVecF));
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    if(particleP->sortF && particleP->colorBuf) {
        GXSetArray(GX_VA_CLR0, particleP->colorBuf, sizeof(GXColor));
    } else {
        GXSetArray(GX_VA_CLR0, &particleP->data->color, sizeof(RESULT_PARTICLEDATA));
    }
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetArray(GX_VA_TEX0, particleP->stBuf, sizeof(HuVec2f));
    GXSetCullMode(GX_CULL_NONE);
    GXLoadPosMtxImm(*mtx, 0);
    if(shadowModelDrawF != 0) {
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ONE, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    } else {
        GXSetTevColorIn(GX_TEVSTAGE0, particleP->tevColorIn[0], particleP->tevColorIn[1], particleP->tevColorIn[2], particleP->tevColorIn[3]);
        if(modelP->attr & HU3D_ATTR_ZWRITE_OFF) {
            GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
        } else {
            GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
        }
    }
    GXSetTevColor(GX_TEVREG0, particleP->tevColor1);
    GXSetTevColor(GX_TEVREG1, particleP->tevColor2);
    switch(particleP->blendMode) {
        case 0:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
            break;
        case 1:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
            break;
        case 2:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_ZERO, GX_BL_INVDSTCLR, GX_LO_NOOP);
            break;
    }
    HuSprTexLoad(particleP->anim, 0, GX_TEXMAP0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    GXCallDisplayList(particleP->dlBuf, particleP->dlSize);
    totalPolyCnt += particleP->num;
    if(shadowModelDrawF == FALSE) {
        if(!(particleP->attr & RESULT_PARTICLE_ATTR_STOPCNT) && Hu3DPauseF == FALSE) {
            particleP->count++;
        }
        if(particleP->prevCount != 0 && particleP->prevCount <= particleP->count) {
            if(particleP->attr & RESULT_PARTICLE_ATTR_LOOP) {
                particleP->count = 0;
            } else {
                particleP->count = particleP->prevCount;
            }
        }
        particleP->prevCounter = GlobalCounter;
    }
}

void fn_1_C420(HU3D_MODELID modelId, RESULT_PARTICLEHOOK hook)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    RESULT_PARTICLE *particleP = modelP->hookData;
    particleP->hook = hook;
}

static HuVec2f lbl_1_data_948[4] = {
    { 0.0f, 0.0f },
    { 1.0f, 0.0f },
    { 0.0f, 0.25f },
    { 1.0f, 0.25f }
};

static s32 lbl_1_data_968[6] = {
    DATA_result, DATA_result, DATA_result, DATA_result, DATA_result, DATA_result
};

s32 fn_1_C45C(s32 dataNum)
{
    s32 languageNo = GWLanguageGet();
    if(languageNo < 0) {
        languageNo = 0;
    } else if(languageNo > 5) {
        languageNo = 5;
    }
    switch((u32)DIRNUM(dataNum)) {
        case DATA_result:
            dataNum = FILENUM(dataNum) | lbl_1_data_968[languageNo];
            break;
    }
    return dataNum;
}

s32 lbl_1_bss_14AC;
RESULT_PARTICLESORT *lbl_1_bss_14A8;
