#include "game/board/effect.h"
#include "game/board/object.h"
#include "game/board/exit.h"
#include "game/board/math.h"
#include "game/board/malloc.h"
#include "game/board/telop.h"
#include "game/memory.h"
#include "game/sprite.h"
#include "game/frand.h"
#include "game/main.h"
#include "game/init.h"
#include "game/disp.h"

#include "string.h"

typedef struct mbParticleSort_s {
    s32 key;
    s32 idx;
} MBPARTICLESORT;

static OMOBJ *fadeOMObj;
static OMOBJ *confettiOMObj;
static MBPARTICLESORT *tempColorBuf;
static int tempColorNum;

typedef struct fadeEffWork_s {
    unsigned killF : 1;
    unsigned pauseF : 1;
    u8 alpha;
    s16 time;
    s16 maxTime;
    HU3D_MODELID modelId;
    GXColor color;
    float speed;
} FADEEFFWORK;

static void FadeOMExec(OMOBJ *obj);
static void FadeDraw(HU3D_MODEL *modelP, Mtx *mtx);

void mbEffInit(void)
{
    fadeOMObj = NULL;
    confettiOMObj = NULL;
    tempColorBuf = HuMemDirectMallocNum(HEAP_HEAP, 0x4000, HU_MEMNUM_OVL);
    tempColorNum = 0;
}

void mbEffClose(void)
{
    if(tempColorBuf) {
        HuMemDirectFree(tempColorBuf);
        tempColorBuf = NULL;
    }
}

void mbEffFadeOutSet(s16 maxTime)
{
    FADEEFFWORK *work;
    float maxTimeF;
    if(!fadeOMObj) {
        return;
    }
    if(maxTime <= 0) {
        maxTime = 1;
    }
    work = omObjGetWork(fadeOMObj, FADEEFFWORK);
    work->maxTime = maxTime;
    OSs16tof32(&maxTime, &maxTimeF);
    work->speed = -work->color.a/maxTimeF;
    work->pauseF = FALSE;
    work->time = work->maxTime;
}

void mbEffFadeCreate(s16 maxTime, u8 alpha)
{
    FADEEFFWORK *work;
    if(fadeOMObj) {
        work = omObjGetWork(fadeOMObj, FADEEFFWORK);
        work->killF = TRUE;
        while(fadeOMObj) {
            HuPrcVSleep();
        }
    }
    fadeOMObj = omAddObjEx(mbObjMan, 32000, 0, 0, -1, FadeOMExec);
    omSetStatBit(fadeOMObj, OM_STAT_NOPAUSE|OM_STAT_SPRPAUSE);
    if(maxTime <= 0) {
        maxTime = 1;
    }
    work = omObjGetWork(fadeOMObj, FADEEFFWORK);
    work->killF = FALSE;
    work->pauseF = FALSE;
    work->color.r = 0;
    work->color.g = 0;
    work->color.b = 0;
    work->color.a = 0;
    work->alpha = alpha;
    work->speed = (float)(alpha-work->color.a)/maxTime;
    work->time = maxTime;
    work->maxTime = maxTime;
    work->modelId = Hu3DHookFuncCreate(FadeDraw);
    Hu3DModelCameraSet(work->modelId, 4);
    Hu3DModelLayerSet(work->modelId, 1);
}

BOOL mbEffFadeDoneCheck(void)
{
    FADEEFFWORK *work;
    if(!fadeOMObj) {
        return TRUE;
    }
    work = omObjGetWork(fadeOMObj, FADEEFFWORK);
    return (work->pauseF) ? TRUE : FALSE;
}

void mbEffFadeCameraSet(u16 bit)
{
    if(fadeOMObj) {
        FADEEFFWORK *work = omObjGetWork(fadeOMObj, FADEEFFWORK);
        Hu3DModelCameraSet(work->modelId, bit);
    }
}

BOOL mbEffFadeCheck(void)
{
    return (fadeOMObj != NULL) ? FALSE : TRUE;
}

static void FadeOMExec(OMOBJ *obj)
{
    FADEEFFWORK *work = omObjGetWork(obj, FADEEFFWORK);
    float alpha;
    if(work->killF || mbExitCheck()) {
        if(work->modelId != HU3D_MODELID_NONE) {
            Hu3DModelKill(work->modelId);
        }
        fadeOMObj = NULL;
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    if(work->pauseF) {
        return;
    }
    OSu8tof32(&work->color.a, &alpha);
    alpha += work->speed;
    OSf32tou8(&alpha, &work->color.a);
    if(work->time > 0) {
        work->time--;
        return;
    }
    if(work->speed > 0) {
        work->pauseF = TRUE;
        work->color.a = work->alpha;
    } else {
        work->killF = TRUE;
    }
}

static void FadeDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    static GXColor colorN = { 0xFF, 0xFF, 0xFF, 0xFF };
    Mtx44 proj;
    Mtx modelview;
    float x1, x2, y1, y2;
    FADEEFFWORK *work;
    if(!fadeOMObj) {
        return;
    }
    work = omObjGetWork(fadeOMObj, FADEEFFWORK);
    x1 = 0.0f;
    x2 = HU_FB_WIDTH;
    y1 = 0.0f;
    y2 = HU_FB_HEIGHT;
    MTXOrtho(proj, y1, y2, x1, x2, 0, 10);
    GXSetProjection(proj, GX_ORTHOGRAPHIC);
    MTXIdentity(modelview);
    GXLoadPosMtxImm(modelview, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetViewport(0, 0, x2, 1.0f+y2, 0, 1);
    GXSetScissor(0, 0, x2, 1.0f+y2);
    GXClearVtxDesc();
    GXSetChanMatColor(GX_COLOR0A0, work->color);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_U16, 0);
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition2u16(x1, y1);
    GXPosition2u16(x2, y1);
    GXPosition2u16(x2, y2);
    GXPosition2u16(x1, y2);
    GXEnd();
    GXSetChanMatColor(GX_COLOR0A0, colorN);
}

typedef struct confettiEffData_s {
    s16 time;
    u8 alpha;
    u8 ambNo;
    HuVecF pos;
    HuVecF rot;
    HuVecF vel;
    HuVecF rotVel;
} CONFETTIEFFDATA;

typedef struct confettiEffWork_s {
    unsigned killF : 1;
    unsigned pauseF : 1;
    s8 addNum;
    s8 time;
    s8 delay;
    s16 maxCnt;
    MBOBJID objId;
    HU3D_MODELID hookMdlId;
    CONFETTIEFFDATA *data;
} CONFETTIEFFWORK;

static void EffConfettiOMExec(OMOBJ *obj);
static void EffConfettiAdd(OMOBJ *obj);
static void EffConfettiUpdate(OMOBJ *obj);
static void EffConfettiDraw(HU3D_MODEL *modelP, Mtx *mtx);

void mbEffConfettiCreate(HuVecF *pos, s16 maxCnt, float width)
{
    CONFETTIEFFWORK *work;
    OMOBJ *obj;
    int i;
    CONFETTIEFFDATA *data;
    if(confettiOMObj) {
        mbEffConfettiReset();
        HuPrcSleep(17);
    }
    confettiOMObj = obj = omAddObjEx(mbObjMan, 257, 0, 0, -1, EffConfettiOMExec);
    work = omObjGetWork(obj, CONFETTIEFFWORK);
    work->killF = FALSE;
    work->pauseF = FALSE;
    work->maxCnt = maxCnt;
    work->addNum = 1;
    work->time = 0;
    work->delay = 10;
    work->hookMdlId = Hu3DHookFuncCreate(EffConfettiDraw);
    Hu3DModelCameraSet(work->hookMdlId, HU3D_CAM0);
    work->data = HuMemDirectMallocNum(HEAP_HEAP, work->maxCnt*sizeof(CONFETTIEFFDATA), HU_MEMNUM_OVL);
    obj->trans.x = pos->x;
    obj->trans.y = pos->y;
    obj->trans.z = pos->z;
    obj->rot.x = width;
    work->objId = mbObjCreate(mbBoardDataNumGet(0x50002), NULL, FALSE);
    mbObjLayerSet(work->objId, 5);
    mbObjDispSet(work->objId, FALSE);
    for(data=work->data, i=0; i<work->maxCnt; i++, data++) {
        data->time = -1;
    }
}

void mbEffConfettiKill(void)
{
    if(confettiOMObj) {
        omObjGetWork(confettiOMObj, CONFETTIEFFWORK)->killF = TRUE;
    }
}

void mbEffConfettiReset(void)
{
    CONFETTIEFFWORK *work;
    int i;
    CONFETTIEFFDATA *data;
    if(!confettiOMObj) {
        return;
    }
    work = omObjGetWork(confettiOMObj, CONFETTIEFFWORK);
    work->pauseF = TRUE;
    for(data=work->data, i=0; i<work->maxCnt; i++, data++) {
        if(data->time != -1 && data->time > 16) {
            data->time = 16;
        }
    }
}

static void EffConfettiOMExec(OMOBJ *obj)
{
    CONFETTIEFFWORK *work = omObjGetWork(obj, CONFETTIEFFWORK);
    if(work->killF || mbExitCheck()) {
        mbObjKill(work->objId);
        Hu3DModelKill(work->hookMdlId);
        HuMemDirectFree(work->data);
        confettiOMObj = NULL;
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    EffConfettiAdd(obj);
    EffConfettiUpdate(obj);
}

static void EffConfettiAdd(OMOBJ *obj)
{
    CONFETTIEFFWORK *work = omObjGetWork(obj, CONFETTIEFFWORK);
    int j, i;
    CONFETTIEFFDATA *data;
    if(work->pauseF) {
        return;
    }
    if(work->addNum < 5) {
        if(work->time++ > work->delay) {
            work->time = 0;
            work->addNum++;
        }
    }
    for(j=0; j<work->addNum; j++) {
        float angle;
        for(data=work->data, i=0; i<work->maxCnt; i++, data++) {
            if(data->time == -1) {
                break;
            }
        }
        if(i == work->maxCnt) {
            break;
        }
        data->time = mbRandMod(60)+120;
        angle = 360.0f*frandf();
        data->pos.x = (obj->rot.x*mbSinDeg(angle))+obj->trans.x;
        data->pos.y = obj->trans.y;
        data->pos.z = (obj->rot.x*mbCosDeg(angle))+obj->trans.z;
        data->vel.x = 2.0f*(frandf()-0.5f);
        data->vel.y = -6.533334f*frandf();
        data->vel.z = 2.0f*(frandf()-0.5f);
        data->rotVel.x = 8.0f+(20.0f*(frandf()-0.5f));
        data->rotVel.y = 8.0f+(20.0f*(frandf()-0.5f));
        data->rotVel.z = 8.0f+(20.0f*(frandf()-0.5f));
        data->rot.x = 0;
        data->rot.y = 0;
        data->rot.z = 0;
        data->alpha = 255;
        data->ambNo = mbRandMod(6);
    }
}

static void EffConfettiUpdate(OMOBJ *obj)
{
    int i;
    CONFETTIEFFWORK *work = omObjGetWork(obj, CONFETTIEFFWORK);
    BOOL noKillF = FALSE;
    CONFETTIEFFDATA *data;

    for(data=work->data, i=0; i<work->maxCnt; i++, data++) {
        if(data->time == -1) {
            continue;
        }
        if(data->time <= 0) {
            data->time = -1;
            continue;
        }
        data->time--;
        data->pos.x += data->vel.x;
        data->pos.y += data->vel.y;
        data->pos.z += data->vel.z;
        data->rot.x += data->rotVel.x;
        data->rot.y += data->rotVel.y;
        data->rot.z += data->rotVel.z;
        if(data->time < 16) {
            if(data->alpha >= 15) {
                data->alpha -= 15;
            } else {
                data->alpha = 0;
            }
        }
        if(!noKillF) {
            noKillF = TRUE;
        }
    }
    if(noKillF == FALSE && work->pauseF) {
        work->killF = TRUE;
    }
}

static void EffConfettiDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    static HuVecF ambTbl[6] = {
        { 0.1, 0.4, 1 },
        { 0.2, 1, 0.1 },
        { 0.3, 1, 1 },
        { 1, 0.2, 0.1 },
        { 1, 0.2, 0.8 },
        { 1, 8, 0.3 }
    };
    if(!confettiOMObj || mbExitCheck()) {
        return;
    } else {
        CONFETTIEFFWORK *work = omObjGetWork(confettiOMObj, CONFETTIEFFWORK);
        HU3D_MODEL *modelP = &Hu3DData[mbObjModelIDGet(work->objId)];
        int i;
        CONFETTIEFFDATA *data;
        if(!modelP->hsf) {
            return;
        }
        for(data=work->data, i=0; i<work->maxCnt; i++, data++) {
            Mtx final, temp;
            float ambR, ambG, ambB, tpLvl;
            if(data->time == -1) {
                continue;
            }
            mbMtxRotAxisDeg(temp, 'z', data->rot.z);
            mbMtxRotAxisDeg(final, 'x', data->rot.x);
            MTXConcat(temp, final, final);
            mbMtxRotAxisDeg(temp, 'y', data->rot.y);
            MTXConcat(temp, final, final);
            MTXTrans(temp, data->pos.x, data->pos.y, data->pos.z);
            MTXConcat(temp, final, final);
            MTXConcat(*mtx, final, final);
            ambR = ambTbl[data->ambNo].x;
            ambG = ambTbl[data->ambNo].y;
            ambB = ambTbl[data->ambNo].z;
            OSu8tof32(&data->alpha, &tpLvl);
            tpLvl = (1.0f/255.0f)*tpLvl;
            Hu3DModelTPLvlSet(mbObjModelIDGet(work->objId), tpLvl);
            Hu3DModelAmbSet(mbObjModelIDGet(work->objId), ambR, ambG, ambB);
            Hu3DModelObjDraw(mbObjModelIDGet(work->objId), "grid2", final);
        }
    }
}

static void ParticleDraw(HU3D_MODEL *modelP, Mtx *mtx);

HU3D_MODELID mbParticleCreate(ANIMDATA *anim, s16 maxCnt)
{
    HU3D_MODELID modelId;
    HU3D_MODEL *modelP;
    MBPARTICLE *particleP;
    MBPARTICLEDATA *dataP;
    HuVec2f *stP;
    ANIMLAYER *layerP;
    ANIMBMP *bmpP;
    s16 bmpFmt;
    int vtxNo;
    s16 i;
    float invW, invH;
    void *dlBuf;

    modelId = Hu3DHookFuncCreate(ParticleDraw);
    Hu3DModelCameraSet(modelId, HU3D_CAM0);
    modelP = &Hu3DData[modelId];
    modelP->hookData = particleP = mbMallocNum(sizeof(MBPARTICLE), modelP->mallocNo);
    particleP->anim = anim;
    anim->useNum++;
    particleP->num = maxCnt;
    particleP->blendMode = MB_PARTICLE_BLEND_NORMAL;
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
    particleP->data = dataP = mbMallocNum(maxCnt*sizeof(MBPARTICLEDATA), modelP->mallocNo);
    for(i=0; i<maxCnt; i++, dataP++) {
        dataP->cameraBit = HU3D_CAM_ALL;
        *(u32 *)&dataP->color = 0xFFFFFFFF;
        dataP->dispF = TRUE;
    }
    particleP->vtxBuf = HuMemDirectMallocNum(HEAP_MODEL, maxCnt*sizeof(HuVecF)*4, modelP->mallocNo);
    particleP->stBuf = HuMemDirectMallocNum(HEAP_MODEL, maxCnt*sizeof(HuVec2f)*4, modelP->mallocNo);
    particleP->stTbl = HuMemDirectMallocNum(HEAP_MODEL, anim->patNum*sizeof(HuVec2f)*4, modelP->mallocNo);
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
    particleP->dlBuf = dlBuf = mbMallocFlushModelNum((maxCnt*24)+128, modelP->mallocNo);
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

void mbParticleKill(HU3D_MODELID modelId)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    MBPARTICLE *particleP = modelP->hookData;
    HuSprAnimKill(particleP->anim);
    HuMemDirectFreeNum(HEAP_MODEL, modelP->mallocNo);
    modelP->hsf = NULL;
}

void mbParticleColorCreate(HU3D_MODELID modelId)
{
    MBPARTICLE *particleP = Hu3DData[modelId].hookData;
    particleP->sortF = TRUE;
    if(!particleP->colorBuf) {
        particleP->colorBuf = HuMemDirectMallocNum(HEAP_MODEL, particleP->num*sizeof(GXColor), Hu3DData[modelId].mallocNo);
    }
}

static void ParticleColorCopy(MBPARTICLESORT *buf, int num)
{
    static int effColorNum[32];
    static int effColorNo[32];
    int lo, hi, sp;
    int i, j;
    s32 pivot;
    s32 key, idx;
    MBPARTICLESORT *pi, *pj;
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

static inline void ParticleVtxSet(register float *scale, register float *pos, register HuVecF *src, register HuVecF *dst)
{
#ifdef __MWERKS__
    register float pxy, pzx, pyz, sc;
    register float v0, v1, v2;
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
#endif
}

static inline void ParticleVtxClear(register float *zero, register HuVecF *dst)
{
#ifdef __MWERKS__
    asm {
        psq_l f0, 0(zero), 0, 0
        psq_st f0, 0(dst), 0, 0
        psq_st f0, 8(dst), 0, 0
        psq_st f0, 16(dst), 0, 0
        psq_st f0, 24(dst), 0, 0
        psq_st f0, 32(dst), 0, 0
        psq_st f0, 40(dst), 0, 0
    }
#endif
}

static inline void ParticleSTCopy(register HuVec2f *src, register HuVec2f *dst)
{
#ifdef __MWERKS__
    asm {
        psq_l f0, 0(src), 0, 0
        psq_l f1, 8(src), 0, 0
        psq_l f2, 16(src), 0, 0
        psq_l f3, 24(src), 0, 0
        psq_st f0, 0(dst), 0, 0
        psq_st f1, 8(dst), 0, 0
        psq_st f2, 16(dst), 0, 0
        psq_st f3, 24(dst), 0, 0
    }
#endif
}

static void ParticleDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    static Vec basePos[] = {
        { -0.5f,  0.5f, 0.0f },
        {  0.5f,  0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f },
        { -0.5f, -0.5f, 0.0f }
    };
    MBPARTICLE *particleP;
    ANIMDATA *anim;
    BOOL flag3D;
    HuVec2f *stBuf;
    MBPARTICLEDATA *dataP;
    s32 i;
    HuVecF *vtxBuf;
    HuVecF *vtxSrc;
    u32 j;
    ANIMBANK *animBank;
    ANIMFRAME *animFrame;
    Mtx mtxInv;
    Mtx mtxRot;
    HuVecF tempVtx[4];
    HuVecF initVtx[4];
    ROMtx basePosMtx;
    float pos[4];
    float scale[2];
    MBPARTICLESORT *sortP;
    s16 num;
    s32 key;
    float x, y, z;
    float depth;

    particleP = modelP->hookData;
    anim = particleP->anim;
    if(HmfInverseMtxF3X3(*mtx, mtxInv) == FALSE) {
        PSMTXIdentity(mtxInv);
    }
    PSMTXReorder(mtxInv, basePosMtx);
    if(Hu3DPauseF == FALSE || (modelP->attr & HU3D_ATTR_NOPAUSE)) {
        if(particleP->hook && particleP->prevCounter != GlobalCounter) {
            MBPARTICLEHOOK hook = particleP->hook;
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
        sortP = tempColorBuf;
        for(i=0; i<num; i++, dataP++, sortP++) {
            depth = (dataP->pos.x*x)+(dataP->pos.y*y)+(dataP->pos.z*z);
            key = *(s32 *)&depth;
            if(key < 0) {
                key = -(key & 0x7FFFFFFF);
            }
            sortP->key = key;
            sortP->idx = i;
        }
        ParticleColorCopy(tempColorBuf, num);
    } else {
        particleP->sortF = FALSE;
    }
    dataP = particleP->data;
    vtxBuf = particleP->vtxBuf;
    stBuf = particleP->stBuf;
    PSMTXROMultVecArray(basePosMtx, &basePos[0], initVtx, 4);
    flag3D = FALSE;
    if(particleP->attr & MB_PARTICLE_ATTR_3D) {
        flag3D = TRUE;
    }
    for(i=0; i<particleP->num; i++, dataP++) {
        if(particleP->sortF) {
            dataP = &particleP->data[tempColorBuf[i].idx];
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
                    float s = 0.5f*mbSinDeg(dataP->rot.z);
                    float c = 0.5f*mbCosDeg(dataP->rot.z);
                    basePos[0].x = basePos[3].y = -s-c;
                    basePos[0].y = basePos[1].x = -s+c;
                    basePos[1].y = basePos[2].x = s+c;
                    basePos[2].y = basePos[3].x = s-c;
                    PSMTXROMultVecArray(basePosMtx, &basePos[0], tempVtx, 4);
                    vtxSrc = tempVtx;
                }
            } else {
                if(0.0f == dataP->rot.x && 0.0f == dataP->rot.y && 0.0f == dataP->rot.z) {
                    vtxSrc = basePos;
                } else {
                    mbMtxRot(mtxRot, dataP->rot.x, dataP->rot.y, dataP->rot.z);
                    PSMTXMultVecArray(mtxRot, &basePos[0], tempVtx, 4);
                    vtxSrc = tempVtx;
                }
            }
            pos[0] = pos[3] = dataP->pos.x;
            pos[1] = dataP->pos.y;
            pos[2] = dataP->pos.z;
            scale[0] = scale[1] = dataP->scale;
            ParticleVtxSet(scale, pos, vtxSrc, vtxBuf);
        } else {
            static float nullVec[2] = { 0.0f, 0.0f };
            ParticleVtxClear(nullVec, vtxBuf);
        }
        vtxBuf += 4;
        animBank = &anim->bank[dataP->animBank];
        animFrame = &animBank->frame[dataP->animNo];
        if((particleP->attr & MB_PARTICLE_ATTR_ANIM) && !dataP->pauseF) {
            if(Hu3DPauseF == FALSE || (modelP->attr & HU3D_ATTR_NOPAUSE)) {
                for(j=0; j<(s32)dataP->animSpeed*minimumVcount; j++) {
                    dataP->animTime += 1.0f;
                    if(dataP->animTime >= animFrame->time) {
                        dataP->animNo++;
                        dataP->animTime -= animFrame->time;
                        animFrame = &animBank->frame[dataP->animNo];
                        if(dataP->animNo >= animBank->timeNum || animFrame->time == -1) {
                            if(!(particleP->attr & MB_PARTICLE_ATTR_LOOP)) {
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
                        if(!(particleP->attr & MB_PARTICLE_ATTR_LOOP)) {
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
        DCFlushRangeNoSync(particleP->data, particleP->num*sizeof(MBPARTICLEDATA));
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
        GXSetArray(GX_VA_CLR0, &particleP->data->color, sizeof(MBPARTICLEDATA));
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
        case MB_PARTICLE_BLEND_NORMAL:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
            break;
        case MB_PARTICLE_BLEND_ADDCOL:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
            break;
        case MB_PARTICLE_BLEND_INVCOL:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_ZERO, GX_BL_INVDSTCLR, GX_LO_NOOP);
            break;
    }
    HuSprTexLoad(particleP->anim, 0, GX_TEXMAP0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    GXCallDisplayList(particleP->dlBuf, particleP->dlSize);
    totalPolyCnt += particleP->num;
    if(shadowModelDrawF == FALSE) {
        if(!(particleP->attr & MB_PARTICLE_ATTR_STOPCNT) && Hu3DPauseF == FALSE) {
            particleP->count++;
        }
        if(particleP->prevCount != 0 && particleP->prevCount <= particleP->count) {
            if(particleP->attr & MB_PARTICLE_ATTR_LOOP) {
                particleP->count = 0;
            } else {
                particleP->count = particleP->prevCount;
            }
        }
        particleP->prevCounter = GlobalCounter;
    }
}

void mbParticleHookSet(HU3D_MODELID modelId, void *hook)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    MBPARTICLE *particleP = modelP->hookData;
    particleP->hook = hook;
}

void mbParticleAttrSet(HU3D_MODELID modelId, u8 attr)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    MBPARTICLE *particleP = modelP->hookData;
    particleP->attr |= attr;
}

void mbParticleAttrReset(HU3D_MODELID modelId, u8 attr)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    MBPARTICLE *particleP = modelP->hookData;
    particleP->attr &= ~attr;
}

MBPARTICLEDATA *mbParticleDataCreate(MBPARTICLE *particleP)
{
    MBPARTICLEDATA *dataP;
    int i;
    for(dataP=particleP->data, i=0; i<particleP->num; i++, dataP++) {
        if(dataP->pauseF) {
            dataP->pauseF = FALSE;
            dataP->dispF = TRUE;
            dataP->animTime = 0;
            return dataP;
        }
    }
    return NULL;
}

typedef struct unkTotalData2_s {
    u8 unk0[2];
    s16 num;
    u8 unk4[8];
} UNKTOTALDATA2;

typedef struct unkTotalData_s {
    s16 num;
    UNKTOTALDATA2 *data;
} UNKTOTALDATA;

typedef struct unkTotal_s {
    u8 unk0[8];
    UNKTOTALDATA *data;
} UNKTOTAL;

int mbParticleUnkTotalGet(void *ptr, int no)
{
    int i;
    int num = 0;
    UNKTOTALDATA *data = &((UNKTOTAL *)ptr)->data[no];

    for(i=0; i<data->num; i++) {
        UNKTOTALDATA2 *data2 = &data->data[i];
        num += data2->num;
    }
    return num;
}

static void ParManFunc(HU3D_MODEL *modelP, MBPARMAN *parManP, Mtx mtx);

HU3D_MODELID mbParManCreate(ANIMDATA *anim, s16 maxCnt, MBPARMANPARAM *param)
{
    HU3D_MODELID modelId;
    MBPARTICLE *particleP;
    HU3D_MODEL *modelP;
    MBPARMAN *parManP;
    s16 i;
    MBPARTICLEDATA *dataP;
    modelId = mbParticleCreate(anim, maxCnt);
    modelP = &Hu3DData[modelId];
    particleP = modelP->hookData;
    parManP = mbMallocNum(sizeof(MBPARMAN), modelP->mallocNo);
    memcpy(parManP, particleP, sizeof(MBPARTICLE));
    HuMemDirectFree(particleP);
    modelP->hookData = parManP;
    mbParticleHookSet(modelId, ParManFunc);
    for(dataP=parManP->particle.data, i=0; i<parManP->particle.num; i++, dataP++) {
        dataP->scale = 0.0f;
    }
    parManP->modelId = modelId;
    parManP->param = param;
    parManP->attr = 0;
    parManP->pos.x = parManP->pos.y = parManP->pos.z = 0;
    parManP->vec.x = 0;
    parManP->vec.y = 1;
    parManP->vec.z = 1;
    parManP->vacuum.x = 0;
    parManP->vacuum.y = 0;
    parManP->vacuum.z = 0;
    parManP->vacuumSpeed = 1;
    parManP->accel = 0;
    parManP->timeLimit = 0;
    return modelId;
}

void mbParManKill(HU3D_MODELID modelId)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    MBPARTICLE *particleP = modelP->hookData;
    HuSprAnimKill(particleP->anim);
    HuMemDirectFreeNum(HEAP_MODEL, modelP->mallocNo);
    modelP->hsf = NULL;
}

void mbParticleBlendModeSet(HU3D_MODELID modelId, int blendMode)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    MBPARTICLE *particleP = modelP->hookData;
    particleP->blendMode = blendMode;
}

void mbParManPosSet(int modelId, float x, float y, float z)
{
    Hu3DModelPosSet(modelId, x, y, z);
}

void mbParManVecSet(HU3D_MODELID modelId, float x, float y, float z)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    MBPARMAN *parManP = modelP->hookData;
    parManP->vec.x = x;
    parManP->vec.y = y;
    parManP->vec.z = z;
}

void mbParManRotSet(HU3D_MODELID modelId, float x, float y, float z)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    MBPARMAN *parManP = modelP->hookData;
    Mtx rot;
    mbMtxRot(rot, x, y, z);
    parManP->vec.x = rot[0][2];
    parManP->vec.y = rot[1][2];
    parManP->vec.z = rot[2][2];
}

void mbParManAttrSet(HU3D_MODELID modelId, int attr)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    MBPARMAN *parManP = modelP->hookData;
    parManP->attr |= attr;
}

void mbParManAttrReset(HU3D_MODELID modelId, int attr)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    MBPARMAN *parManP = modelP->hookData;
    parManP->attr &= ~attr;
}

static float baseScale[] = {
    1.0f, 0.9f, 0.7f, 0.5f,
    0.5f, 0.7f, 0.9f, 1.0f
};

static void ParManFunc(HU3D_MODEL *modelP, MBPARMAN *parManP, Mtx mtx)
{
    MBPARTICLEDATA *dataP;
    MBPARTICLEDATA *dataEnd;
    s16 circleIdx;
    s16 i;
    MBPARMANPARAM *param;
    GXColor *colorStart;
    GXColor *colorEnd;
    HuVecF vacuumAccel;
    HuVecF vacuumDist;
    HuVecF vecDir;
    HuVecF vel;
    HuVecF dir;
    HuVecF up;
    float weight;
    float c;
    float s;
    float angleStart;
    float upRot;
    float accelVal;
    float rot;
    float scale;
    s16 colorIdx;

    param = parManP->param;
    if(Hu3DPauseF && !(modelP->attr & HU3D_ATTR_NOPAUSE)) {
        return;
    }
    dataP = parManP->particle.data;
    if(parManP->attr & MB_PARMAN_ATTR_RANDTIME90) {
        accelVal = param->accelRange*0.9+frandmod((u32)param->accelRange*0.1*1000.0)/1000.0f;
    } else if(parManP->attr & MB_PARMAN_ATTR_RANDTIME70) {
        accelVal = param->accelRange*0.7+frandmod((u32)param->accelRange*0.3*1000.0)/1000.0f;
    } else {
        accelVal = param->accelRange;
    }
    parManP->accel += accelVal;
    circleIdx = 0;
    dataEnd = &parManP->particle.data[parManP->particle.num];
    if(parManP->attr & MB_PARMAN_ATTR_RANDANGLE) {
        angleStart = frandmod((u32)(360.0f/param->accelRange)*100)/100;
    }
    if(parManP->accel >= 1.0f && !(parManP->attr & MB_PARMAN_ATTR_TIMEUP)) {
        for(; dataP < dataEnd; dataP++) {
            if(dataP->aliveF) {
                continue;
            }
            dataP->aliveF = TRUE;
            scale = param->scaleBase;
            if(parManP->attr & MB_PARMAN_ATTR_RANDSCALE90) {
                scale = scale*0.9+(frandmod((u32)(scale*0.1*1000.0))/1000.0f);
            } else if(parManP->attr & MB_PARMAN_ATTR_RANDSCALE70) {
                scale = scale*0.7+(frandmod((u32)(scale*0.3*1000.0))/1000.0f);
            }
            dataP->scaleBase = scale;
            dataP->scale = scale;
            vel.x = frandmod((u32)(param->scaleRange*2.0f))-param->scaleRange;
            vel.y = frandmod((u32)(param->scaleRange*2.0f))-param->scaleRange;
            vel.z = frandmod((u32)(param->scaleRange*2.0f))-param->scaleRange;
            if(0.0f == vel.x && 0.0f == vel.y && 0.0f == vel.z) {
                vel.z = 1.0f;
            }
            VECNormalize(&vel, &vel);
            VECScale(&vel, &vel, param->scaleRange);
            mbVecCopy(&vel, &dataP->pos);
            VECNormalize(&parManP->vec, &vecDir);
            if(parManP->attr & MB_PARMAN_ATTR_RANDANGLE) {
                upRot = angleStart+(360.0f/param->accelRange)*circleIdx;
                rot = param->angleRange;
            } else {
                upRot = frandmod(360);
                if(param->angleRange) {
                    rot = frandmod((u32)param->angleRange);
                } else {
                    rot = 0.0f;
                }
            }
            if(vecDir.x * vecDir.x < 0.000001 && vecDir.z * vecDir.z < 0.000001) {
                up.x = 1.0f;
                up.y = up.z = 0.0f;
            } else {
                if(vecDir.y * vecDir.y > 0.000001) {
                    dir.x = vecDir.x;
                    dir.y = 0.0f;
                    dir.z = vecDir.z;
                } else {
                    dir.x = vecDir.x;
                    dir.y = 1.0f;
                    dir.z = vecDir.z;
                }
                VECCrossProduct(&dir, &vecDir, &up);
            }
            if(0.0f == up.x && 0.0f == up.y && 0.0f == up.z) {
                up.z = 1.0f;
            }
            VECNormalize(&up, &up);
            s = mbSinDeg(upRot);
            c = mbCosDeg(upRot);
            dir.x = up.x * (vecDir.x * vecDir.x + c * (1.0f - vecDir.x * vecDir.x))
                + up.y * (vecDir.x * vecDir.y * (1.0f - c) - vecDir.z * s)
                + up.z * (vecDir.x * vecDir.z * (1.0f - c) + vecDir.y * s);
            dir.y = up.x * (vecDir.x * vecDir.y * (1.0f - c) + vecDir.z * s)
                + up.y * (vecDir.y * vecDir.y + c * (1.0f - vecDir.y * vecDir.y))
                + up.z * (vecDir.y * vecDir.z * (1.0f - c) - vecDir.x * s);
            dir.z = up.x * (vecDir.x * vecDir.z * (1.0f - c) - vecDir.y * s)
                + up.y * (vecDir.y * vecDir.z * (1.0f - c) + vecDir.x * s)
                + up.z * (vecDir.z * vecDir.z + c * (1.0f - vecDir.z * vecDir.z));
            VECCrossProduct(&dir, &vecDir, &up);
            s = mbSinDeg(rot);
            c = mbCosDeg(rot);
            dir.x = vecDir.x * (up.x * up.x + c * (1.0f - up.x * up.x))
                + vecDir.y * (up.x * up.y * (1.0f - c) - up.z * s)
                + vecDir.z * (up.x * up.z * (1.0f - c) + up.y * s);
            dir.y = vecDir.x * (up.x * up.y * (1.0f - c) + up.z * s)
                + vecDir.y * (up.y * up.y + c * (1.0f - up.y * up.y))
                + vecDir.z * (up.y * up.z * (1.0f - c) - up.x * s);
            dir.z = vecDir.x * (up.x * up.z * (1.0f - c) - up.y * s)
                + vecDir.y * (up.y * up.z * (1.0f - c) + up.x * s)
                + vecDir.z * (up.z * up.z + c * (1.0f - up.z * up.z));
            if(0.0f == dir.x && 0.0f == dir.y && 0.0f == dir.z) {
                dir.z = 1.0f;
            }
            VECNormalize(&dir, &dir);
            s = param->speedBase;
            if(parManP->attr & MB_PARMAN_ATTR_RANDSPEED90) {
                s = s*0.9+frandmod((u32)(s*0.1*1000.0))/1000.0f;
            } else if(parManP->attr & MB_PARMAN_ATTR_RANDSPEED70) {
                s = s*0.7+frandmod((u32)(s*0.3*1000.0))/1000.0f;
            } else if(parManP->attr & MB_PARMAN_ATTR_RANDSPEED100) {
                s = frandmod((u32)(s*1000.0f))/1000.0f;
            }
            VECScale(&dir, &dataP->vel, s);
            dataP->accel = param->gravity;
            dataP->speedDecay = param->speedDecay;
            if(parManP->attr & MB_PARMAN_ATTR_SETCOLOR) {
                dataP->colorIdx = colorIdx = parManP->color;
            } else {
                dataP->colorIdx = colorIdx = frandmod(param->colorNum);
            }
            dataP->color = param->colorStart[colorIdx];
            dataP->time = 0;
            parManP->accel -= 1.0f;
            circleIdx++;
            if(parManP->accel <= 0.0f) {
                break;
            }
        }
    }
    parManP->accel = 0.0f;
    if(parManP->timeLimit != 0) {
        parManP->timeLimit--;
        if(parManP->timeLimit == 0) {
            parManP->attr |= MB_PARMAN_ATTR_TIMEUP;
        }
    }
    if(Hu3DPauseF && !(modelP->attr & HU3D_ATTR_NOPAUSE)) {
        return;
    }
    for(dataP=parManP->particle.data, i=0; i<parManP->particle.num; i++, dataP++) {
        if(!dataP->aliveF) {
            continue;
        }
        param = parManP->param;
        if(parManP->attr & MB_PARMAN_ATTR_SCALEJITTER) {
            dataP->scale = dataP->scaleBase*baseScale[(parManP->jitterNo+i)&7];
        } else {
            dataP->scale = dataP->scaleBase;
        }
        if(parManP->attr & MB_PARMAN_ATTR_PAUSE) {
            continue;
        }
        VECAdd(&dataP->pos, &dataP->vel, &dataP->pos);
        VECAdd(&dataP->pos, &dataP->accel, &dataP->pos);
        VECScale(&dataP->vel, &dataP->vel, dataP->speedDecay);
        VECAdd(&param->gravity, &dataP->accel, &dataP->accel);
        if(parManP->attr & MB_PARMAN_ATTR_VACUUM) {
            VECSubtract(&parManP->vacuum, &dataP->pos, &vacuumAccel);
            if(0.0f == vacuumAccel.x && 0.0f == vacuumAccel.y && 0.0f == vacuumAccel.z) {
                vacuumAccel.z = 1.0f;
            }
            VECNormalize(&vacuumAccel, &vacuumAccel);
            VECScale(&vacuumAccel, &vacuumAccel, parManP->vacuumSpeed);
            VECAdd(&vacuumAccel, &dataP->accel, &dataP->accel);
            VECAdd(&dataP->vel, &dataP->accel, &vacuumAccel);
            VECSubtract(&parManP->vacuum, &dataP->pos, &vacuumDist);
            if(VECSquareMag(&vacuumDist) <= VECSquareMag(&vacuumAccel)) {
                dataP->scale = 0.0f;
                continue;
            }
        }
        dataP->scaleBase *= param->scaleDecay;
        weight = (float)dataP->time/param->maxTime;
        if(weight > 1.0f) {
            weight = 1.0f;
        }
        colorIdx = dataP->colorIdx;
        colorStart = &param->colorStart[colorIdx];
        colorEnd = &param->colorEnd[colorIdx];
        dataP->color.r = colorStart->r+(weight*(colorEnd->r-colorStart->r));
        dataP->color.g = colorStart->g+(weight*(colorEnd->g-colorStart->g));
        dataP->color.b = colorStart->b+(weight*(colorEnd->b-colorStart->b));
        dataP->color.a = colorStart->a+(weight*(colorEnd->a-colorStart->a));
        if(dataP->scale < 0.01 || dataP->time >= param->maxTime) {
            dataP->aliveF = FALSE;
            dataP->scale = 0.0f;
        }
        dataP->time++;
    }
    parManP->jitterNo++;
}
