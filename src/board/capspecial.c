/* @@HEAD@@ */
#include "game/board/capspecial.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/coin.h"
#include "game/board/exit.h"
#include "game/board/comchoice.h"
#include "game/board/masu.h"
#include "game/board/math.h"
#include "game/board/object.h"
#include "game/board/player.h"
#include "game/board/star.h"
#include "game/board/window.h"
#include "game/board/wipe.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/process.h"

#include "humath.h"
#include "math.h"
#include "stdio.h"
#include "string.h"

/* The capsule event process work (HuPrcCurrentGet()->property), as captrap.c names it. */
typedef struct CapWork_s {
    /* 0x000 */ int playerNo;
    /* 0x004 */ u8 unk4[0xC];
    /* 0x010 */ int masuId;
    /* 0x014 */ u8 unk14[0xC];
    /* 0x020 */ u8 objWork[0xB48]; /* mbev_CapObjCreate's object list */
    /* 0xB68 */ u8 unkB68[0x68];
    /* 0xBD0 */ void *explodeEff;
    /* 0xBD4 */ u8 unkBD4[8];
    /* 0xBDC */ void *glowEff;
    /* 0xBE0 */ void *ringHitEff;
    /* 0xBE4 */ void *coinEff;
} CAPWORK;

/* The Teresa screen fade (mbev_CapTeresaFadeCreate). */
typedef struct CapTeresaFadeWork_s {
    /* 0x00 */ void *texBuf;
    /* 0x04 */ u32 texSize;
    /* 0x08 */ int mipmapF;
    /* 0x0C */ float alpha;
    /* 0x10 */ int copyF;
    /* 0x14 */ OMOBJ *obj;
    /* 0x18 */ u32 srcW;
    /* 0x1C */ u32 srcH;
    /* 0x20 */ u32 dstW;
    /* 0x24 */ u32 dstH;
} CAPTERESAFADEWORK; /* size 0x28 */

/* capevent.c; local prototypes, the B0 header's are placeholders. */
void mbev_CapWait(CAPWORK *work);
int mbev_CapObjCreate(void *objWork, int dataNum, int *motDataNum, int linkF, int arg4, int arg5);
s16 mbev_CapPlayerMotionCreate(void *objWork, int playerNo, int dataNum);
void *mbev_CapEffGlowCreate(void);
void *mbev_CapEffCoinCreate(void);
void mbev_CapEffCoinGlowSet(void *coinEff, void *glowEff);
int mbev_CapPlayerCheck(int playerNo);
int mbev_CapPlayerComSelKettouGet(int playerNo);
void mbev_CapPlayerMotShiftWait(int playerNo, int motNo, int attr, int arg3);
void mbev_CapCoinAdd(int playerNo, int coin);

static void ev_CapTeresaFadeMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *material);
static void ev_CapTeresaFadeOMExec(OMOBJ *obj);
static void ev_CapMiracleMasu(CAPWORK *work);

static HuVecF capsuleCameraOfs = { 0.0f, 100.0f, 0.0f };
static HuVecF teresaCameraRot = { -30.0f, 0.0f, 0.0f };
static HuVecF teresaCameraOfs = { 0.0f, 100.0f, 0.0f };
static HuVecF teresaLightPos = { 0.0f, 0.0f, 0.0f };
static HuVecF teresaLightDir = { 0.0f, 1.0f, -1.0f };

static int MiracleGuideMotTbl[2][16] = {
    { 0x110001, 0x110004, 0x110005, 0x110015, 0x110007, 0x11000B, 0x11000C, 0x11000D, 0x110013, -1 },
    { 0x11001C, 0x11001F, 0x110020, 0x11002F, 0x110022, 0x110025, 0x110026, 0x110027, 0x11002D, -1 },
};

static s32 koopaMdlId = -1;
static s32 teresaStealMesId = -1;
static GXColor teresaLightColor = { 255, 190, 255, 255 };

/* .sbss: emitted in reverse definition order */
static CAPTERESAFADEWORK *teresaFadeWork;
static s32 teresaStealCoinNum;
static void *teresaStealBeginHook;
static void *teresaStealHook;
static OMOBJ *miracleSprObj;
static s32 diceHitTimer;

static GXColor miracleMasuEffColorTbl[6] = {
    { 255, 127, 127, 255 },
    { 255, 255, 127, 255 },
    { 255, 190, 127, 255 },
    { 127, 127, 255, 255 },
    { 127, 255, 255, 255 },
    { 127, 190, 255, 255 },
};

static int miracleTradeOrderTbl[3][16][2] = {
    {
        { 0, 2 }, { 0, 1 }, { 0, 3 }, { 0, 1 }, { 0, 2 }, { 0, 1 }, { 0, 4 }, { 0, 1 },
        { 0, 1 }, { -1, 0 },
    },
    {
        { 3, 2 }, { 0, 1 }, { 0, 2 }, { 0, 1 }, { 3, 0 }, { 4, 2 }, { 3, 0 }, { 1, 0 },
        { 1, 5 }, { 2, 4 }, { -1, 0 },
    },
    {
        { 3, 0 }, { 4, 2 }, { 0, 5 }, { 1, 2 }, { 0, 3 }, { 0, 1 }, { 4, 1 }, { 3, 1 },
        { 2, 3 }, { 4, 5 }, { -1, 0 },
    },
};

static int miracleLeftCharOrderTbl[16][2] = {
    { 1, 2 }, { 1, 3 }, { 1, 0 }, { 1, 3 }, { 0, 2 }, { -1, 0 },
};

static int miracleRightCharOrderTbl[2][16][2] = {
    {
        { 2, 0 }, { 2, 1 }, { 2, 1 }, { 2, 0 }, { 2, 0 }, { -1, 0 },
    },
    {
        { 1, 0 }, { 2, 0 }, { 2, 0 }, { 1, 0 }, { 2, 0 }, { -1, 0 },
    },
};

static HuVecF miracleTradePosTbl[3] = {
    { 138.0f, 90.0f, 0.0f },
    { 288.0f, 90.0f, 0.0f },
    { 438.0f, 90.0f, 0.0f },
};

static int miracleTradeFileTbl[6] = { 0x11003B, 0x11003C, 0x11003D, 0x11003E, 0x11003F, 0x110040 };

static s32 miracleBackFile = 0x11003A;

static int kettouGuideMotTbl[2][16] = {
    { 0x110001, 0x110004, 0x110011, 0x110005, 0x110012, 0x110013, 0x110014, -1 },
    { 0x11001C, 0x11001F, 0x11002B, 0x110020, 0x11002C, 0x11002D, 0x11002E, -1 },
};

static int kettouPlayerMotTbl[2][2] = {
    { 0x8E0027, 0x93001F },
    { 0x8E0078, 0x93004B },
};

static s32 kettouMotId[12];
static s32 mgResultData[4];

static int donkeyMotTbl[12] = {
    0xE000F, 0xE0011, 0xE0012, 0xE0015, 0xE0016, 0xE0013, 0xE0014, 0xE0018, 0xE0019, 0xE001A, 0xE001B, -1
};

static s32 donkeyMgFile[2] = { 0x5008C, 0x5008C };
static s8 donkeyDiceTbl[8] = { 0, 1, 2, 3, 4, -1 };

static int donkeyDiceResultTbl[5] = { 5, 10, 20, 30, -1 };
static int donkeyRouletteBankTbl[5][2] = { { 0, 1 }, { 0, 1 }, { 0, 1 }, { 0, 1 }, { 0, 2 } };

static int koopaMotTbl[7] = { 0xE0001, 0xE0005, 0xE0003, 0xE0007, 0xE0004, 0xE000A, -1 };

static s32 koopaMgFile[2] = { 0x5008D, 0x5008D };
static s8 koopaDiceTbl[8] = { 0, 1, 2, 3, 4, -1 };

static int koopaDiceResultTbl[5] = { 5, 10, 20, 30, -1 };
static int koopaLoseMesTbl[3] = { 0x3F000A, 0x3F000B, 0x3F000C };
static int koopaLoseMesTbl2[3] = { 0x3F000A, 0x3F000B, 0x3F000C };

/* @@PART1@@ */
void mbev_CapTeresa(void)
{
}

void mbev_CapTeresaKill(void)
{
}

void mbev_CapTeresaStealSet(int mesId, int coinNum, void *beginHook, void *hook)
{
    teresaStealMesId = mesId;
    teresaStealCoinNum = coinNum;
    teresaStealBeginHook = beginHook;
    teresaStealHook = hook;
}

static inline void *CapTeresaMallocNum(s32 size, u32 num)
{
    return HuMemDirectMallocNum(HEAP_MODEL, size, num);
}

void mbev_CapTeresaFadeCreate(int objId)
{
    int mdlId;
    HU3D_MODEL *modelP;
    HSF_DATA *hsf;
    HSF_MATERIAL *material;
    int i;

    mdlId = mbObjModelIDGet(objId);
    modelP = &Hu3DData[mdlId];
    hsf = modelP->hsf;
    material = hsf->material;
    Hu3DModelMatHookSet(mdlId, ev_CapTeresaFadeMatHook);
    for(i = 0; i < hsf->materialNum; i++, material++) {
        material->flags |= 0x2000;
    }
    teresaFadeWork = CapTeresaMallocNum(sizeof(CAPTERESAFADEWORK), modelP->mallocNo);
    memset(teresaFadeWork, 0, sizeof(CAPTERESAFADEWORK));
    teresaFadeWork->mipmapF = TRUE;
    teresaFadeWork->alpha = 255.0f;
    teresaFadeWork->copyF = FALSE;
    teresaFadeWork->srcW = 640;
    teresaFadeWork->srcH = 480;
    teresaFadeWork->dstW = 320;
    teresaFadeWork->dstH = 240;
    teresaFadeWork->obj = omAddObjEx(mbObjMan, 32768, 0, 0, -1, ev_CapTeresaFadeOMExec);
    teresaFadeWork->texSize = GXGetTexBufferSize(teresaFadeWork->dstW, teresaFadeWork->dstH, GX_TF_RGB565, GX_FALSE, 0);
    teresaFadeWork->texBuf = CapTeresaMallocNum(teresaFadeWork->texSize, modelP->mallocNo);
    memset(teresaFadeWork->texBuf, 0, teresaFadeWork->texSize);
    DCFlushRange(teresaFadeWork->texBuf, teresaFadeWork->texSize);
}

void mbev_CapTeresaFadeKill(int objId)
{
    Hu3DModelMatHookSet(mbObjModelIDGet(objId), NULL);
    if(teresaFadeWork) {
        HuMemDirectFree(teresaFadeWork->texBuf);
        HuMemDirectFree(teresaFadeWork);
        teresaFadeWork = NULL;
    }
}

static void ev_CapTeresaFadeMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *material)
{
    CAPTERESAFADEWORK *work;
    HU3D_CAMERA *cameraP;
    float fov;
    GXColor color;
    GXTexObj texObj;
    Mtx lightMtx;
    Mtx invCamMtx;
    Mtx objMtx;
    Mtx texMtx;

    work = teresaFadeWork;
    if(work) {
        if(work->copyF == FALSE) {
            GXDrawDone();
            GXSetTexCopySrc(0, 0, work->srcW, work->srcH);
            GXSetTexCopyDst(work->dstW, work->dstH, GX_TF_RGB565, work->mipmapF);
            GXCopyTex(work->texBuf, GX_FALSE);
            GXPixModeSync();
            work->copyF = TRUE;
        }
        if(material->attrNum == 1) {
            GXSetNumTexGens(2);
            GXSetNumTevStages(2);
            GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_1);
            GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            Hu3DMatLightSet(drawObj->model, 0, material->hiliteScale);
            cameraP = &Hu3DCamera[Hu3DCameraNo];
            fov = cameraP->fov;
            if(fov <= 0.0f) {
                fov = 30.0f;
            }
            C_MTXLightPerspective(lightMtx, fov, 1.2f, 0.5f, -0.5f, 0.5f, 0.5f);
            PSMTXInverse(Hu3DCameraMtx, invCamMtx);
            PSMTXConcat(invCamMtx, drawObj->matrix, objMtx);
            PSMTXConcat(lightMtx, Hu3DCameraMtx, texMtx);
            PSMTXConcat(texMtx, objMtx, texMtx);
            GXLoadTexMtxImm(texMtx, GX_TEXMTX1, GX_MTX3x4);
            GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
            color.r = color.g = color.b = 255;
            color.a = teresaFadeWork->alpha;
            GXSetTevColor(GX_TEVREG2, color);
            GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
            GXSetTevKAlphaSel(GX_TEVSTAGE1, GX_TEV_KASEL_1);
            GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_TEXC, GX_CC_CPREV, GX_CC_A2, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
            GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXInitTexObj(&texObj, work->texBuf, work->dstW, work->dstH, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
            GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
            GXLoadTexObj(&texObj, GX_TEXMAP1);
        } else {
            return;
        }
    }
}

static void ev_CapTeresaFadeOMExec(OMOBJ *obj)
{
    if(mbExitCheck() || !teresaFadeWork) {
        omDelObjEx(mbObjMan, obj);
    } else {
        teresaFadeWork->copyF = FALSE;
    }
}

void mbev_CapTeresaFadeSet(float alpha)
{
    if(teresaFadeWork) {
        if(alpha < 0.0f) {
            alpha = 0.0f;
        }
        if(alpha > 255.0f) {
            alpha = 255.0f;
        }
        teresaFadeWork->alpha = alpha;
    }
}

/* @@PART2@@ */
/* @@PART3@@ */
/* @@PART4@@ */
/* @@END@@ */
