#include "game/board/dice.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/effect.h"
#include "game/board/exit.h"
#include "game/board/guide.h"
#include "game/board/math.h"
#include "game/board/player.h"
#include "game/board/telop.h"
#include "game/board/window.h"

#include "game/data.h"
#include "game/frand.h"
#include "game/hsfformat.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/process.h"
#include "game/sprite.h"

#include "datadir_enum.h"
#include "math.h"
#include "string.h"

/* Board coin objects take s16 ids here (the extsh before each call); coin.h has int. */
s16 mbCoinCreate2(void);
s16 mbCoinObjCreate(int digit, int color);
void mbCoinObjNumDec(s16 id);
void mbCoinObjKill(s16 id);
void mbCoinObjPosSet(s16 id, float x, float y, float z);
void mbCoinObjPosSetV(s16 id, HuVecF *pos);
void mbCoinObjPosGet(s16 id, HuVecF *pos);
void mbCoinObjRotSet(s16 id, float x, float y, float z);
void mbCoinObjScaleSet(s16 id, float x, float y, float z);
void mbCoinObjDispSet(s16 id, int dispF);
void mbCoinObjLayerSet(s16 id, u16 layer);
int mbCoinAddExec(int playerNo, int coinNum);
/* Retail calls sprintf unprototyped (no crclr). */
int sprintf();
/* snpc.h has void *arg2: the colour is passed by value. */
void mbObjBiriQColorSet(s16 modelId, int mode, GXColor color, float time);

#define DICE_PLAYERNO_NULL 4
#define DICE_MAX (DICE_PLAYERNO_NULL+1)

#define DICEEFF_PUFF_MAX 50
#define DICEEFF_TRI_MAX 20

typedef struct DiceEff_s {
    HuVecF rot;
    float radius;
    HuVecF scale;
} DICEEFF;

typedef struct DiceWork_s {
    int playerNo; /* 0x00 */
    int diceType; /* 0x04 */
    int max; /* 0x08 */
    int no; /* 0x0C */
    u8 unk10[0x30-0x10];
    s8 result[3]; /* 0x30 */
    u8 unk33[0x48-0x33];
    DICEEFF *eff; /* 0x48 */
    u8 unk4C[4];
    void *dlBuf; /* 0x50 */
    u32 dlSize; /* 0x54 */
    ANIMDATA *animEffPuff; /* 0x58 */
    ANIMDATA *animEffTri; /* 0x5C */
    s8 puffTime; /* 0x60 */
    s8 triTime; /* 0x61 */
} DICEWORK;

typedef struct DiceNumWork_s {
    unsigned killF : 1;
    unsigned rotF : 1;
    unsigned updateF : 1;
    unsigned modelNo : 2;
    unsigned bendF : 1;
    unsigned playerNo : 2;
    s8 value;
    u8 color;
    s16 bendMode;
    s16 time;
    s16 maxTime;
} DICENUMWORK;

typedef struct DiceNumVtx_s {
    HuVecF pos;
    float weight;
} DICENUMVTX;

typedef struct DiceSNpcNumWork_s {
    unsigned killF : 1;
    unsigned dispF : 1;
    unsigned unkF : 1;
    u8 value;
} DICESNPCNUMWORK;

typedef struct DiceParticleData_s {
    s16 time; /* 0x00 */
    u8 unk2[6];
    HuVecF vel; /* 0x08 */
    float scaleBase; /* 0x14 */
    float accelY; /* 0x18 */
    float velDecay; /* 0x1C */
    float scaleSpeed; /* 0x20 */
    float unk24;
    float rotSpeed; /* 0x28 */
    float scale; /* 0x2C */
    float unk30; /* 0x30 */
    float unk34; /* 0x34 */
    float zRot; /* 0x38 */
    HuVecF pos; /* 0x3C */
    GXColor color; /* 0x48 */
    s16 animNo; /* 0x4C */
    u8 unk4E[0x5C-0x4E];
} DICEPARTICLEDATA;

typedef struct DiceParticle_s {
    s16 endF; /* 0x00 */
    s16 mode; /* 0x02 */
    u8 unk4[0x14-0x4];
    float unk14; /* 0x14 */
    u8 unk18[0x20-0x18];
    u8 unk20; /* 0x20 */
    u8 unk21[0x26-0x21];
    s16 num; /* 0x26 */
    u32 initF; /* 0x28 */
    u8 unk2C[0x68-0x2C];
    DICEPARTICLEDATA *data; /* 0x68 */
} DICEPARTICLE;

static int diceResult[DICE_MAX];
static HUPROCESS *diceProc[DICE_MAX];
static OMOBJ *diceFadeOMObj[DICE_MAX];

static void *diceHitHook[DICE_MAX] = {};
static void *dicePadBtnHook[DICE_MAX] = {};
static void *diceMotHook[DICE_MAX] = {};
static OMOBJ *diceNumOMObj[DICE_MAX][3] = {};
static HuVecF dice1PosTbl[1] = {
    0, 300, 0
};
static HuVecF dice2PosTbl[2] = {
    -200, 300, 0,
    200, 300, 0
};
static HuVecF dice3PosTbl[3] = {
    -200, 300, 0,
    200, 300, 0,
    0, 300, 0
};
static HuVecF *dicePosTbl[3] = { dice1PosTbl, dice2PosTbl, dice3PosTbl };
static int diceKuriboResultTbl[7] = { 0, 3, 5, 10, 20, 30, 40 };
static int diceDicePatapataResultTbl[8] = { 1, 3, 5, 10, 15, 20, 25, -1 };
static s8 diceMaxTbl[21] = { 1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3 };
static s8 diceValueMaxTbl[21] = { 10, 10, 10, 5, 10, 1, 1, 7, 0, 0, 1, 5, 5, 6, 10, 10, 10, 10, 10, 10, 10 };
static s8 diceSingleValueMaxTbl[21] = { 6, 6, 6, 3, 10, 1, 1, 7, 0, 0, 1, 5, 5, 6, 6, 10, 10, 10, 10, 10, 10 };

static char diceMatchCoinStr[8];

/* First half (fork 5): rough placeholders so the unit compiles. */
void mbDiceInit(void)
{
}

int mbDiceProcExec(int arg0, int arg1, void *arg2, void *arg3, int arg4, int arg5, void *arg6, int arg7)
{
    return 0;
}

int mbDiceExec(int arg0, int arg1, void *arg2, int arg3, int arg4, int arg5, void *arg6, int arg7)
{
    return 0;
}

void mbDicePlayerExec(int arg0, int arg1)
{
}

void mbDiceChanceTradeExec(int arg0)
{
}

void mbDiceChanceCharExec(int arg0, void *arg1)
{
}

void mbDiceFadeSet(int arg0)
{
}

void mbDiceKill(int arg0)
{
}

void mbDiceClose(void)
{
}

int mbDiceKillCheck(int arg0)
{
    return 0;
}

int mbDiceKillCheckAll(void)
{
    return 0;
}

int mbDiceResultGet(int arg0)
{
    return diceResult[arg0];
}

void mbDiceHitHookSet(int arg0, void *arg1)
{
    diceHitHook[arg0] = arg1;
}

void mbDiceObjHit(int arg0)
{
}

void mbDicePadBtnHookSet(int arg0, void *arg1)
{
    dicePadBtnHook[arg0] = arg1;
}

void mbDiceMotHookSet(int arg0, void *arg1)
{
    diceMotHook[arg0] = arg1;
}

int mbDiceTypeGet(int arg0)
{
    return 0;
}

void mbDiceTutorialNumSet(int arg0, int arg1)
{
}

int mbDiceTutorialNumGet(int arg0)
{
    return 0;
}

s16 mbDiceValueNoGet(int arg0)
{
    return 0;
}

s8 mbDiceMaxGet(int arg0)
{
    return 0;
}

s8 mbDiceValueMaxGet(int arg0)
{
    return 0;
}

static void DiceNumObjOMExec(OMOBJ *obj);
static void DiceNumObjBendOMExec(OMOBJ *obj);
static void DiceNumObjShrinkOMExec(OMOBJ *obj);

int mbDiceNumObjCreate(u8 arg0, HuVecF *arg1, HuVecF *arg2, int arg3, u8 arg4, int arg5)
{
    return 0;
}

static void DiceNumObjOMExec(OMOBJ *obj)
{
}

static void DiceNumObjBendOMExec(OMOBJ *obj)
{
}

void mbDiceNumObjKill(void *arg0)
{
}

static void DiceNumObjShrinkOMExec(OMOBJ *obj)
{
}

/* Second half (fork 6). */
static void DiceNumObjReset(int playerNo)
{
    int i;
    for(i=0; i<3; i++) {
        OMOBJ *obj = diceNumOMObj[playerNo][i];
        if(obj) {
            DICENUMWORK *work = omObjGetWork(obj, DICENUMWORK);
            work->rotF = TRUE;
            work->updateF = TRUE;
            work->time = 0;
            work->maxTime = 30;
            obj->trans.x = obj->rot.x;
            obj->trans.y = obj->rot.y;
            obj->trans.z = obj->rot.z;
            if(fabsf2(obj->rot.x-obj->scale.x) > 25.0f) {
                obj->rot.y -= -50.0f;
            } else if(fabsf2(obj->rot.z-obj->scale.z) > 25.0f) {
                obj->rot.y += -50.0f;
            }
            obj->rot.x = obj->scale.x;
            obj->rot.z = obj->scale.z;
            obj->objFunc = DiceNumObjOMExec;
        }
    }
}

BOOL mbDiceNumStopCheck(int playerNo)
{
    int i;
    for(i=0; i<3; i++) {
        if(diceNumOMObj[playerNo][i]) {
            DICENUMWORK *work = omObjGetWork(diceNumOMObj[playerNo][i], DICENUMWORK);
            if(work->updateF) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

void mbDiceNumKill(int playerNo)
{
    int i;
    if(playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    for(i=0; i<3; i++) {
        if(diceNumOMObj[playerNo][i]) {
            DICENUMWORK *work = omObjGetWork(diceNumOMObj[playerNo][i], DICENUMWORK);
            work->killF = TRUE;
            diceNumOMObj[playerNo][i] = NULL;
        }
    }
}

void mbDiceNumShrinkSet(int playerNo)
{
    int i;
    if(playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    for(i=0; i<3; i++) {
        OMOBJ *obj = diceNumOMObj[playerNo][i];
        if(obj) {
            DICENUMWORK *work = omObjGetWork(obj, DICENUMWORK);
            work->time = 0;
            work->maxTime = 30;
            obj->objFunc = DiceNumObjShrinkOMExec;
        }
    }
}

void mbDiceStub(void)
{
}

OMOBJ *mbDiceSNpcNumCreate(int playerNo, HuVecF *pos)
{
    OMOBJ *obj;
    int i;
    OMOBJ *result = NULL;
    int value = 0;
    int color = 0;
    BOOL foundF = FALSE;
    HuVecF rot;
    HuVecF ofs;
    if(playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    for(i=0; i<3; i++) {
        obj = diceNumOMObj[playerNo][i];
        if(obj) {
            DICENUMWORK *work = omObjGetWork(obj, DICENUMWORK);
            value += work->value;
            if(!foundF) {
                color = work->color;
                mbVecCopy(&obj->rot, &rot);
                VECSubtract(&rot, pos, &ofs);
                foundF = TRUE;
            }
            work->killF = TRUE;
            diceNumOMObj[playerNo][i] = NULL;
        }
    }
    if(foundF) {
        result = mbDiceSNpcNumObjCreate(pos, &ofs, value, FALSE, color);
    }
    return result;
}

static void ev_DiceZoromeCoin(int playerNo, int coinNum);
static void DiceZoromeEffHook(HU3D_MODEL *modelP, DICEPARTICLE *particleP, Mtx mtx);

static void DiceNumObjBendStart(int playerNo)
{
    OMOBJ **numObj;
    int i;
    for(i=0, numObj=diceNumOMObj[playerNo]; i<3; numObj++, i++) {
        OMOBJ *obj = *numObj;
        if(obj) {
            DICENUMWORK *work = omObjGetWork(obj, DICENUMWORK);
            work->updateF = TRUE;
            work->bendF = TRUE;
            work->time = 0;
            work->maxTime = 120;
            obj->mdlId[2] = mbParticleCreate(HuSprAnimRead(HuDataSelHeapReadNum(mbBoardDataNumGet(DATANUM(DATA_board, 0x64)), HU_MEMNUM_OVL, HEAP_MODEL)), 100);
            mbParticleHookSet(obj->mdlId[2], DiceZoromeEffHook);
            Hu3DModelCameraSet(obj->mdlId[2], 2);
            Hu3DModelLayerSet(obj->mdlId[2], 5);
            Hu3DModelPosSet(obj->mdlId[2], obj->rot.x, 30.000002f+obj->rot.y, 50.0f+obj->rot.z);
            obj->objFunc = DiceNumObjBendOMExec;
        }
    }
}

static void DiceNumObjBendStop(int playerNo)
{
    int i;
    DICENUMWORK *work;
    int j;
    OMOBJ *obj;
    for(i=0; i<3; i++) {
        obj = diceNumOMObj[playerNo][i];
        if(obj) {
            work = omObjGetWork(obj, DICENUMWORK);
            for(j=0; j<2; j++) {
                if(obj->mdlId[j] > 0) {
                    static GXColor biriQColor = { 0, 0, 0, 0 };
                    mbObjBiriQColorSet(obj->mdlId[j], 0, biriQColor, 0.0f);
                }
            }
            if(obj->mdlId[2] >= 0) {
                mbParticleKill(obj->mdlId[2]);
                obj->mdlId[2] = -1;
            }
            work->bendF = FALSE;
        }
    }
}

static void ev_DiceZorome(DICEWORK *work)
{
    int coin;
    int streamId;
    HuVecF playerPos;
    switch(work->max) {
        case 2:
            if(work->result[0] == 7) {
                coin = 30;
            } else {
                coin = 10;
            }
            streamId = 40;
            break;

        case 3:
            if(work->result[0] == 7) {
                coin = 50;
            } else {
                coin = 30;
            }
            streamId = 41;
            break;

        default:
            return;
    }
    sprintf(diceMatchCoinStr, "%d", coin);
    mbMusPauseFadeOut(0, TRUE, 1000);
    mbPlayerPosGet(work->playerNo, &playerPos);
    DiceNumObjBendStart(work->playerNo);
    HuPrcSleep(10);
    mbMusJingleWait(mbMusJinglePlay(streamId));
    mbMusPauseFadeOut(0, FALSE, 1000);
    mbAudGuidePlay(950);
    mbWinCreate(2, 0x26000A, mbGuideSpeakerNoGet());
    mbWinTopInsertMesSet(MESSNUM_PTR(diceMatchCoinStr), 0);
    mbWinTopWait();
    ev_DiceZoromeCoin(work->playerNo, coin);
    DiceNumObjBendStop(work->playerNo);
    while(!mbDiceNumStopCheck(work->playerNo)) {
        HuPrcVSleep();
    }
    mbMusBoardPlay();
}

static u8 diceInEffAnimTbl[8] = { 0, 1, 2, 2, 3, 3, 3, 3 };

static void ev_DiceZoromeCoin(int playerNo, int coinNum)
{
    int i;
    int id;
    int num;
    int activeNum = 0;
    int coinNo;
    int coinId[64];
    float velY[64];
    HuVecF pos;
    HuVecF playerPos;
    float rotY;
    for(i=0; i<64; i++) {
        coinId[i] = 0;
    }
    mbPlayerPosGet(playerNo, &playerPos);
    num = coinNum;
    while(num != 0 || activeNum != 0) {
        if(num != 0) {
            for(i=0; i<64; i++) {
                if(coinId[i] == 0) {
                    break;
                }
            }
            if(i < 64) {
                coinId[i] = mbCoinCreate2();
                velY[i] = -13.333334f;
                id = coinId[i];
                num--;
                pos.x = playerPos.x+(60.000004f*(frandf()-0.5f));
                pos.y = playerPos.y+(50.0f*frandf());
                pos.z = playerPos.z+(60.000004f*(frandf()-0.5f));
                pos.y += 800.0f;
                mbCoinObjPosSetV(id, &pos);
                rotY = 360.0f*frandf();
                mbCoinObjRotSet(id, 40.0f*(frandf()-0.5f), rotY, 0.0f);
                mbCoinObjScaleSet(id, 0.7f, 0.7f, 0.7f);
            }
        }
        activeNum = 0;
        for(i=0; i<64; i++) {
            if(coinId[i] != 0) {
                coinNo = coinId[i];
                activeNum++;
                mbCoinObjPosGet(coinNo, &pos);
                velY[i] += -0.5444445f;
                pos.y += velY[i];
                mbCoinObjPosSetV(coinNo, &pos);
                if(pos.y < 100.0f+playerPos.y) {
                    mbCoinObjKill(coinNo);
                    coinId[i] = 0;
                }
            }
        }
        HuPrcVSleep();
    }
    mbCoinAddExec(playerNo, coinNum);
}

static u8 diceMatchEffAnimBankTbl[16] = { 0, 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };
static float diceMatchEffScale[4] = { 1.0f, 1.0f, 1.0f, 0.6f };
static GXColor diceMatchEffColorTbl[8] = {
    { 220, 64, 64, 0 },
    { 64, 220, 64, 0 },
    { 220, 220, 64, 0 },
    { 64, 64, 220, 0 },
    { 220, 64, 220, 0 },
    { 64, 220, 220, 0 },
    { 220, 120, 64, 0 },
    { 64, 120, 220, 0 },
};

static void DiceZoromeEffHook(HU3D_MODEL *modelP, DICEPARTICLE *particleP, Mtx mtx)
{
    int i;
    int num;
    DICEPARTICLEDATA *dataP;
    int no;
    int colorNo;
    float angle;
    float elev;
    float t;
    float alpha;
    HuVecF ofs;
    num = 3;
    if(particleP->initF == 0 || particleP->endF != 0) {
        num = 0.5f*particleP->num;
        particleP->initF = 1;
        particleP->endF = 0;
        particleP->unk20 = 1;
        particleP->unk14 = 0.0f;
    }
    for(dataP=particleP->data, i=0; i<particleP->num; i++, dataP++) {
        if(num <= 0) {
            break;
        }
        if(dataP->time != 0) {
            continue;
        }
        no = diceMatchEffAnimBankTbl[mbRandMod(16)];
        dataP->animNo = no;
        dataP->pos.x = dataP->pos.y = dataP->pos.z = 0.0f;
        dataP->pos.y += particleP->unk14;
        angle = 360.0f*frandf();
        elev = (1.6f*frandf())-0.8f;
        elev = 90.0f*(elev*fabsf2(elev));
        dataP->vel.x = HuSin(angle)*HuCos(elev);
        dataP->vel.y = HuSin(elev);
        dataP->vel.z = HuCos(angle)*HuCos(elev);
        VECScale(&dataP->vel, &dataP->vel, (1.0f/60.0f)*(100.0f+(100.0f*(2.0f*frandf()))));
        VECScale(&dataP->vel, &ofs, 20.0f);
        VECAdd(&dataP->pos, &ofs, &dataP->pos);
        VECScale(&dataP->vel, &dataP->vel, 0.25f);
        dataP->scale = diceMatchEffScale[no]*(25.0f*(0.5f+(0.5f*frandf())));
        dataP->scaleBase = dataP->scale;
        colorNo = mbRandMod(8);
        t = 0.2f+(0.5f*frandf());
        dataP->color.r = (t*(255.0f-diceMatchEffColorTbl[colorNo].r))+diceMatchEffColorTbl[colorNo].r;
        dataP->color.g = (t*(255.0f-diceMatchEffColorTbl[colorNo].g))+diceMatchEffColorTbl[colorNo].g;
        dataP->color.b = (t*(255.0f-diceMatchEffColorTbl[colorNo].b))+diceMatchEffColorTbl[colorNo].b;
        dataP->color.a = mbRandMod(70)+150;
        dataP->accelY = dataP->color.a;
        dataP->color.a = 0;
        dataP->zRot = 0.0f;
        if(dataP->animNo < 3) {
            dataP->zRot = 360.0f*frandf();
        }
        dataP->time = mbRandMod(15)+15;
        num--;
    }
    switch(particleP->mode) {
        case 0:
            for(dataP=particleP->data, i=0; i<particleP->num; i++, dataP++) {
                if(dataP->time == 0) {
                    continue;
                }
                VECScale(&dataP->vel, &dataP->vel, 0.96f);
                VECAdd(&dataP->pos, &dataP->vel, &dataP->pos);
                dataP->vel.y += 0.08333334f;
                dataP->scale -= 0.4f;
            }
            break;

        case 1:
            for(dataP=particleP->data, i=0; i<particleP->num; i++, dataP++) {
                if(dataP->time == 0) {
                    continue;
                }
                VECAdd(&dataP->pos, &dataP->vel, &dataP->pos);
                dataP->vel.y += -0.41666672f;
            }
            break;
    }
    for(dataP=particleP->data, i=0; i<particleP->num; i++, dataP++) {
        if(dataP->time == 0) {
            continue;
        }
        dataP->time--;
        if(dataP->time < 10) {
            dataP->scale *= 0.7f;
            if(dataP->time == 0) {
                dataP->color.a = 0;
                dataP->scale = 0.0f;
            }
        } else {
            alpha = dataP->color.a;
            alpha = alpha+(1.0f+(0.2f*(dataP->accelY-alpha)));
            dataP->color.a = alpha;
        }
    }
}

static void DiceInEffHook(HU3D_MODEL *modelP, DICEPARTICLE *particleP, Mtx mtx);
static void DiceInDotEffHook(HU3D_MODEL *modelP, DICEPARTICLE *particleP, Mtx mtx);

static s16 DiceInEffCreate(void)
{
    int modelId = mbParticleCreate(HuSprAnimRead(HuDataSelHeapReadNum(mbBoardDataNumGet(DATANUM(DATA_board, 0x64)), HU_MEMNUM_OVL, HEAP_MODEL)), 80);
    mbParticleHookSet(modelId, DiceInEffHook);
    Hu3DModelLayerSet(modelId, 5);
    mbParticleAttrSet(modelId, 16);
    mbParticleColorCreate(modelId);
    return modelId;
}

static s16 DiceInDotEffCreate(void)
{
    int modelId = mbParticleCreate(HuSprAnimRead(HuDataSelHeapReadNum(mbBoardDataNumGet(DATANUM(DATA_board, 0x64)), HU_MEMNUM_OVL, HEAP_MODEL)), 150);
    mbParticleHookSet(modelId, DiceInDotEffHook);
    Hu3DModelLayerSet(modelId, 5);
    return modelId;
}

static void DiceHitEffSet(s16 modelId)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    DICEPARTICLE *particleP = modelP->hookData;
    particleP->endF = 1;
    particleP->mode = 1;
}

static float diceInEffScaleRatio[4] = { 1.0f, 1.0f, 0.7f, 0.4f };
static GXColor diceInEffColorTbl[8] = {
    { 220, 100, 100, 0 },
    { 100, 220, 100, 0 },
    { 220, 220, 100, 0 },
    { 100, 100, 220, 0 },
    { 220, 100, 220, 0 },
    { 100, 220, 220, 0 },
    { 220, 150, 100, 0 },
    { 100, 150, 220, 0 },
};

static void DiceInEffHook(HU3D_MODEL *modelP, DICEPARTICLE *particleP, Mtx mtx)
{
    int i;
    int no;
    DICEPARTICLEDATA *dataP;
    int colorNo;
    float angle;
    float elev;
    float t;
    if(particleP->initF == 0 || particleP->endF != 0) {
        dataP = particleP->data;
        switch(particleP->mode) {
            case 0:
                for(i=0; i<particleP->num; i++, dataP++) {
                    no = diceInEffAnimTbl[mbRandMod(4)];
                    dataP->animNo = no;
                    colorNo = mbRandMod(8);
                    dataP->color.r = diceInEffColorTbl[colorNo].r+mbRandMod(20);
                    dataP->color.g = diceInEffColorTbl[colorNo].g+mbRandMod(20);
                    dataP->color.b = diceInEffColorTbl[colorNo].b+mbRandMod(20);
                    dataP->color.a = 250;
                    dataP->pos.x = dataP->pos.y = dataP->pos.z = 0.0f;
                    angle = 360.0f*frandf();
                    t = (1.6f*frandf())-0.8f;
                    elev = 90.0f*(t*fabsf2(t));
                    dataP->vel.x = HuSin(angle)*HuCos(elev);
                    dataP->vel.y = HuSin(elev);
                    dataP->vel.z = HuCos(angle)*HuCos(elev);
                    VECScale(&dataP->vel, &dataP->vel, (1.0f/60.0f)*(800.0f+(100.0f*(8.0f*frandf()))));
                    dataP->scale = diceInEffScaleRatio[no]*(60.0f*(0.5f+(0.7f*frandf())));
                    dataP->scaleBase = dataP->scale;
                    dataP->scaleSpeed = 0.5f;
                    dataP->unk30 = 360.0f*frandf();
                    dataP->unk34 = 360.0f*frandf();
                    dataP->rotSpeed = 7.0f;
                    dataP->time = mbRandMod(20)+20;
                    dataP->accelY = 0.0f;
                    dataP->velDecay = 0.93f;
                }
                break;

            case 1:
                for(i=0; i<particleP->num; i++, dataP++) {
                    dataP->pos.x = dataP->pos.y = dataP->pos.z = 0.0f;
                    dataP->pos.y += 50.0f;
                    angle = 360.0f*frandf();
                    t = 0.7f+(0.3f*frandf());
                    elev = 90.0f*(t*fabsf2(t));
                    dataP->vel.x = HuSin(angle)*HuCos(elev);
                    dataP->vel.y = HuSin(elev);
                    dataP->vel.z = HuCos(angle)*HuCos(elev);
                    VECScale(&dataP->vel, &dataP->vel, (1.0f/60.0f)*(300.0f+(100.0f*(3.0f*frandf()))));
                    dataP->vel.y *= 1.5f;
                    dataP->scale = 0.7f*dataP->scaleBase;
                    dataP->scaleSpeed = 0.3f;
                    if(dataP->color.r < 150) {
                        dataP->color.r -= 30;
                    }
                    if(dataP->color.g < 150) {
                        dataP->color.g -= 30;
                    }
                    if(dataP->color.b < 150) {
                        dataP->color.b -= 30;
                    }
                    dataP->color.a = 255;
                    dataP->time = mbRandMod(20)+20;
                    dataP->accelY = -0.66666675f;
                    dataP->velDecay = 1.0f;
                }
                break;
        }
        particleP->initF = 1;
        particleP->endF = 0;
    }
    for(dataP=particleP->data, i=0; i<particleP->num; i++, dataP++) {
        if(dataP->time == 0) {
            continue;
        }
        VECScale(&dataP->vel, &dataP->vel, dataP->velDecay);
        VECAdd(&dataP->pos, &dataP->vel, &dataP->pos);
        dataP->vel.y += dataP->accelY;
        dataP->scale += dataP->scaleSpeed;
        dataP->zRot += dataP->rotSpeed;
        dataP->time--;
        if(dataP->time < 10) {
            dataP->color.a *= 0.8f;
            if(dataP->time == 0) {
                dataP->color.a = 0;
                dataP->scale = 0.0f;
            }
        }
    }
}

static float diceInDotEffScaleRatio[4] = { 1.0f, 1.0f, 0.7f, 0.4f };
static GXColor diceInDotEffColorTbl[8] = {
    { 220, 64, 64, 0 },
    { 64, 220, 64, 0 },
    { 220, 220, 64, 0 },
    { 64, 64, 220, 0 },
    { 220, 64, 220, 0 },
    { 64, 220, 220, 0 },
    { 220, 120, 64, 0 },
    { 64, 120, 220, 0 },
};

static void DiceInDotEffHook(HU3D_MODEL *modelP, DICEPARTICLE *particleP, Mtx mtx)
{
    int i;
    DICEPARTICLEDATA *dataP;
    int no;
    int colorNo;
    float angle;
    float elev;
    float t;
    if(particleP->initF == 0 || particleP->endF != 0) {
        dataP = particleP->data;
        switch(particleP->mode) {
            case 0:
                for(i=0; i<particleP->num; i++, dataP++) {
                    no = 3;
                    dataP->animNo = no;
                    dataP->pos.x = dataP->pos.y = dataP->pos.z = 0.0f;
                    angle = 360.0f*frandf();
                    t = (1.6f*frandf())-0.8f;
                    elev = 90.0f*(t*fabsf2(t));
                    dataP->vel.x = HuSin(angle)*HuCos(elev);
                    dataP->vel.y = HuSin(elev);
                    dataP->vel.z = HuCos(angle)*HuCos(elev);
                    VECScale(&dataP->vel, &dataP->vel, (1.0f/60.0f)*(500.0f+(100.0f*(9.0f*frandf()))));
                    dataP->scale = diceInDotEffScaleRatio[no]*(60.0f*(0.5f+(0.7f*frandf())));
                    dataP->scaleBase = dataP->scale;
                    colorNo = mbRandMod(8);
                    t = 0.2f+(0.4f*frandf());
                    dataP->color.r = (t*(255.0f-diceInDotEffColorTbl[colorNo].r))+diceInDotEffColorTbl[colorNo].r;
                    dataP->color.g = (t*(255.0f-diceInDotEffColorTbl[colorNo].g))+diceInDotEffColorTbl[colorNo].g;
                    dataP->color.b = (t*(255.0f-diceInDotEffColorTbl[colorNo].b))+diceInDotEffColorTbl[colorNo].b;
                    dataP->color.a = mbRandMod(50)+100;
                    dataP->time = mbRandMod(20)+20;
                }
                break;

            case 1:
                for(i=0; i<particleP->num; i++, dataP++) {
                    dataP->pos.x = dataP->pos.y = dataP->pos.z = 0.0f;
                    dataP->pos.y += 50.0f;
                    angle = 360.0f*frandf();
                    t = 0.7f+(0.3f*frandf());
                    elev = 90.0f*(t*fabsf2(t));
                    dataP->vel.x = HuSin(angle)*HuCos(elev);
                    dataP->vel.y = HuSin(elev);
                    dataP->vel.z = HuCos(angle)*HuCos(elev);
                    VECScale(&dataP->vel, &dataP->vel, (1.0f/60.0f)*(300.0f+(100.0f*(3.0f*frandf()))));
                    dataP->vel.y *= 1.5f;
                    dataP->scale = 0.8f*dataP->scaleBase;
                    dataP->color.a = mbRandMod(50)+120;
                    dataP->time = mbRandMod(10)+20;
                }
                break;
        }
        particleP->initF = 1;
        particleP->endF = 0;
        particleP->unk20 = 1;
    }
    switch(particleP->mode) {
        case 0:
            for(dataP=particleP->data, i=0; i<particleP->num; i++, dataP++) {
                if(dataP->time == 0) {
                    continue;
                }
                VECScale(&dataP->vel, &dataP->vel, 0.92f);
                VECAdd(&dataP->pos, &dataP->vel, &dataP->pos);
                dataP->vel.y += -0.22222224f;
                dataP->scale -= 0.15f;
            }
            break;

        case 1:
            for(dataP=particleP->data, i=0; i<particleP->num; i++, dataP++) {
                if(dataP->time == 0) {
                    continue;
                }
                VECAdd(&dataP->pos, &dataP->vel, &dataP->pos);
                dataP->vel.y += -0.41666672f;
            }
            break;
    }
    for(dataP=particleP->data, i=0; i<particleP->num; i++, dataP++) {
        if(dataP->time == 0) {
            continue;
        }
        dataP->time--;
        if(dataP->time < 6) {
            dataP->color.a *= 0.7f;
            if(dataP->time == 0) {
                dataP->color.a = 0;
                dataP->scale = 0.0f;
            }
        }
    }
}

static float diceInEffRotYRatio[3] = {
    (1.0f/1.0f), (1.0f/2.0f), (1.0f/3.0f)
};

static void DiceObjEffCreate(DICEWORK *work)
{
    int hitOrderTbl[20] = {
        0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2
    };
    float posY;
    DICEEFF *eff;

    void *dlBuf;
    int i;
    work->eff = eff = HuMemDirectMallocNum(HEAP_HEAP, (DICEEFF_PUFF_MAX+DICEEFF_TRI_MAX)*sizeof(DICEEFF), HU_MEMNUM_OVL);
    memset(eff, 0, (DICEEFF_PUFF_MAX+DICEEFF_TRI_MAX)*sizeof(DICEEFF));
    for(i=0; i<50; i++, eff++) {
        eff->rot.x = (15.0f*frandf())+30.0f;
        eff->rot.y = 360.0f*frandf();
        eff->radius = 100.0f*(1.0f+frandf());
    }
    for(i=0; i<100; i++) {
        int idx1, idx2;
        int temp;
        idx1 = mbRandMod(20);
        idx2 = mbRandMod(20);
        temp = hitOrderTbl[idx1];
        hitOrderTbl[idx1] = hitOrderTbl[idx2];
        hitOrderTbl[idx2] = temp;
    }
    posY = 0;
    for(i=0; i<20; i++, eff++) {
        eff->rot.x = 0;
        eff->rot.y = posY;
        eff->radius = 150;
        eff->scale.x = diceInEffRotYRatio[hitOrderTbl[i]];
        eff->scale.y = 1;
        eff->scale.z = 1+(2*frandf());
        posY += (360*eff->scale.x)/12;
    }
    dlBuf = HuMemDirectMallocNum(HEAP_HEAP, 4096, HU_MEMNUM_OVL);
    DCInvalidateRange(dlBuf, 4096);
    GXBeginDisplayList(dlBuf, 4096);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-20, 0, -20);
    GXTexCoord2f32(0, 0);
    GXPosition3f32(20, 0, -20);
    GXTexCoord2f32(1, 0);
    GXPosition3f32(20, 0, 20);
    GXTexCoord2f32(1, 1);
    GXPosition3f32(-20, 0, 20);
    GXTexCoord2f32(0, 1);
    GXEnd();
    work->dlSize = GXEndDisplayList();
    work->dlBuf = HuMemDirectMallocNum(HEAP_HEAP, work->dlSize, HU_MEMNUM_OVL);
    memcpy(work->dlBuf, dlBuf, work->dlSize);
    HuMemDirectFree(dlBuf);
    DCFlushRangeNoSync(work->dlBuf, work->dlSize);
    work->animEffPuff = HuSprAnimRead(HuDataSelHeapReadNum(mbBoardDataNumGet(DATANUM(DATA_board, 0x5D)), HU_MEMNUM_OVL, HEAP_MODEL));
    work->animEffTri = HuSprAnimRead(HuDataSelHeapReadNum(mbBoardDataNumGet(DATANUM(DATA_board, 0x62)), HU_MEMNUM_OVL, HEAP_MODEL));
}

static void DiceObjEffKill(DICEWORK *work)
{
    if(work->dlBuf != NULL) {
        HuMemDirectFree(work->dlBuf);
        work->dlBuf = NULL;
    }
    if(work->eff != NULL) {
        HuMemDirectFree(work->eff);
        work->eff = NULL;
    }
    if(work->animEffPuff != NULL) {
        HuSprAnimKill(work->animEffPuff);
        work->animEffPuff = NULL;
    }
    if(work->animEffTri != NULL) {
        HuSprAnimKill(work->animEffTri);
        work->animEffTri = NULL;
    }
}

static void DiceObjEffPuffDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    OMOBJ *obj = modelP->hookData;
    DICEWORK *work = obj->data;
    GXColor color = { 0, 0, 0, 255 };
    DICEEFF *eff;
    int i;
    float time = work->puffTime/45.0f;
    if(time > 1) {
        time = 1;
    }
    HuSprTexLoad(work->animEffPuff, 0, GX_TEXMAP0, GX_REPEAT, GX_REPEAT, GX_NEAR);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ONE);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    color.a = (1-time)*255;
    GXSetTevColor(GX_TEVREG0, color);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetCullMode(GX_CULL_NONE);
    for(eff=work->eff, i=0; i<DICEEFF_PUFF_MAX; i++, eff++) {
        Mtx modelview;
        Mtx rot;
        Mtx trans;
        HuVecF pos;
        static HuVecF back = { 0, 0, -1 };
        mtxRot(rot, eff->rot.x, eff->rot.y, eff->rot.z);
        MTXMultVec(rot, &back, &pos);
        VECScale(&pos, &pos, HuSin(time*90)*eff->radius);
        MTXTrans(trans, obj->trans.x+pos.x, obj->trans.y+pos.y+50, obj->trans.z+pos.z);
        MTXConcat(trans, rot, modelview);
        MTXConcat(*mtx, modelview, modelview);
        GXLoadPosMtxImm(modelview, GX_PNMTX0);
        GXCallDisplayList(work->dlBuf, work->dlSize);
    }
    if(!mbPauseEnableCheck()) {
        if(++work->puffTime >= 45u) {
            modelP->attr |= HU3D_ATTR_DISPOFF;
        }
    }
}

static void DiceObjEffTriDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    OMOBJ *obj = modelP->hookData;
    DICEWORK *work = obj->data;
    GXColor color = { 255, 192, 64, 255 };
    float time = work->triTime/16.0f;
    float alpha;
    DICEEFF *eff;
    int i;
    if(time > 1) {
        time = 1;
    }
    HuSprTexLoad(work->animEffTri, 0, GX_TEXMAP0, GX_REPEAT, GX_REPEAT, GX_NEAR);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    alpha = 255*(4*(1-time));
    if(alpha > 255) {
        alpha = 255;
    }
    color.a = alpha;
    GXSetChanMatColor(GX_COLOR0A0, color);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetCullMode(GX_CULL_BACK);
    for(eff=work->eff+DICEEFF_PUFF_MAX, i=0; i<DICEEFF_TRI_MAX; i++, eff++) {
        Mtx modelview;
        Mtx trans;
        Mtx rot;
        Mtx scale;
        HuVecF pos;
        static HuVecF back = { 0, 0, -1 };
        MTXScale(scale, (2*eff->scale.x)+time, 2*eff->scale.y, 2*eff->scale.z);
        mtxRot(rot, eff->rot.x, eff->rot.y, eff->rot.z);
        MTXMultVec(rot, &back, &pos);
        VECScale(&pos, &pos, HuSin(time*90)*eff->radius);
        MTXTrans(trans, obj->trans.x+pos.x, obj->trans.y+pos.y-50, obj->trans.z+pos.z);
        MTXConcat(rot, scale, modelview);
        MTXConcat(trans, modelview, modelview);
        MTXConcat(*mtx, modelview, modelview);
        GXLoadPosMtxImm(modelview, GX_PNMTX0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-20, 0, -30);
        GXTexCoord2f32(0, 0);
        GXPosition3f32(20, 0, -30);
        GXTexCoord2f32(1, 0);
        GXPosition3f32(20, 0, 10);
        GXTexCoord2f32(1, 1);
        GXPosition3f32(-20, 0, 10);
        GXTexCoord2f32(0, 1);
        GXEnd();
    }
    if(!mbPauseEnableCheck()) {
        if(++work->triTime >= 16) {
            modelP->attr |= HU3D_ATTR_DISPOFF;
        }
    }
}

static float diceCloudRotYRatio2[3] = {
    (1.0f/1.0f), (1.0f/2.0f), (1.0f/3.0f)
};

static DICENUMVTX *DiceNumObjMdlCopy(int modelId)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    HSF_DATA *hsf = modelP->hsf;
    HSF_OBJECT *obj = hsf->object;
    HuVecF *pos;
    DICENUMVTX *vtx;
    int size;
    DICENUMVTX *vtxBuf;

    int i, j;
    float posMin, posMax;
    float sizeY;
    for(i=0; i<hsf->objectNum; i++, obj++) {
        if(obj->type == HSF_OBJ_MESH) {
            size = obj->mesh.vertex->count*sizeof(DICENUMVTX);
            pos = obj->mesh.vertex->data;
            posMin = posMax = pos->y;
            pos++;
            for(j=1; j<obj->mesh.vertex->count; j++, pos++) {
                if(pos->y > posMax) {
                    posMax = pos->y;
                }
                if(pos->y < posMin) {
                    posMin = pos->y;
                }
            }
            sizeY = posMax-posMin;
            vtxBuf = vtx = HuMemDirectMallocNum(HEAP_MODEL, size, modelP->mallocNo);
            pos = obj->mesh.vertex->data;
            for(j=0; j<obj->mesh.vertex->count; j++, pos++, vtx++) {
                vtx->pos = *pos;
                vtx->weight = (pos->y-posMin)/sizeY;
            }
            break;
        }
    }
    return vtxBuf;
}

static void DiceNumObjMdlBend(int modelId, DICENUMVTX *vtx, Mtx mtx1, Mtx mtx2)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    HSF_DATA *hsf = modelP->hsf;
    int i, j;
    HSF_OBJECT *obj = hsf->object;
    for(i=0; i<hsf->objectNum; i++, obj++) {
        if(obj->type == HSF_OBJ_MESH) {
            HuVecF *pos;
            pos = obj->mesh.vertex->data;
            for(j=0; j<obj->mesh.vertex->count; j++, vtx++, pos++) {
                HuVecF vtx1, vtx2;
                MTXMultVec(mtx1, &vtx->pos, &vtx1);
                MTXMultVec(mtx2, &vtx->pos, &vtx2);
                pos->x = vtx1.x+(vtx->weight*(vtx2.x-vtx1.x));
                pos->y = vtx1.y+(vtx->weight*(vtx2.y-vtx1.y));
                pos->z = vtx1.z+(vtx->weight*(vtx2.z-vtx1.z));
            }
            DCStoreRangeNoSync(obj->mesh.vertex->data, obj->mesh.vertex->count*sizeof(HuVecF));
            break;
        }
    }
}

static void DiceSNpcNumUpdate(OMOBJ *obj);

OMOBJ *mbDiceSNpcNumObjCreate(HuVecF *pos, HuVecF *ofs, int value, int flag, int color)
{
    OMOBJ *obj;
    DICESNPCNUMWORK *work;
    int i;
    HuVecF pos3D;
    HuVecF normPos;
    obj = omAddObjEx(mbObjMan, 32258, 20, 0, -1, DiceSNpcNumUpdate);
    omSetStatBit(obj, 0x100);
    work = omObjGetWork(obj, DICESNPCNUMWORK);
    work->dispF = TRUE;
    work->killF = FALSE;
    work->unkF = (u8)flag;
    work->value = (u8)value;
    for(i=0; i<20; i++) {
        int coinId = mbCoinObjCreate(i%10, color);
        mbCoinObjLayerSet(coinId, 4);
        mbCoinObjDispSet(coinId, FALSE);
        obj->mdlId[i] = coinId;
    }
    VECAdd(pos, ofs, &pos3D);
    mbPos3DtoNorm(&pos3D, 1, &normPos);
    obj->rot.z = normPos.z;
    mbVecCopy(pos, &obj->trans);
    mbVecCopy(ofs, &obj->scale);
    return obj;
}

static void DiceSNpcNumUpdate(OMOBJ *obj)
{
    DICESNPCNUMWORK *work = omObjGetWork(obj, DICESNPCNUMWORK);
    BOOL tensF = FALSE;
    HU3D_CAMERA *cameraP = Hu3DCamera;
    int i;
    int digit;
    float tanFov;
    float scale;
    float rotX;
    HuVecF normPos;
    HuVecF pos;
    if(work->killF || mbExitCheck()) {
        for(i=0; i<20; i++) {
            if(obj->mdlId[i] != -1) {
                mbCoinObjNumDec(obj->mdlId[i]);
            }
        }
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    if(mbPauseEnableCheck()) {
        return;
    }
    VECAdd(&obj->trans, &obj->scale, &pos);
    mbPos3DtoNorm(&pos, 1, &normPos);
    normPos.z = obj->rot.z;
    tanFov = HuSin(cameraP[2].fov*0.5f)/HuCos(cameraP[2].fov*0.5f);
    scale = tanFov*-normPos.z;
    normPos.x = normPos.x*(10.0f*scale);
    normPos.y = normPos.y*scale;
    pos = normPos;
    mbCameraRotGet(&normPos);
    rotX = -normPos.x;
    for(i=0; i<20; i++) {
        mbCoinObjDispSet(obj->mdlId[i], FALSE);
    }
    if(work->dispF) {
        scale = tanFov/(HuSin(cameraP->fov*0.5f)/HuCos(cameraP->fov*0.5f));
        digit = work->value/10;
        if(digit != 0) {
            mbCoinObjDispSet(obj->mdlId[digit], TRUE);
            mbCoinObjPosSet(obj->mdlId[digit], pos.x-(60.000004f*scale), pos.y, pos.z);
            mbCoinObjRotSet(obj->mdlId[digit], rotX, 0.0f, 0.0f);
            mbCoinObjScaleSet(obj->mdlId[digit], scale, scale, scale);
            tensF = TRUE;
        }
        digit = (work->value%10)+10;
        if(digit != 0) {
            mbCoinObjDispSet(obj->mdlId[digit], TRUE);
            if(!tensF) {
                mbCoinObjPosSet(obj->mdlId[digit], pos.x, pos.y, pos.z);
            } else {
                mbCoinObjPosSet(obj->mdlId[digit], pos.x+(60.000004f*scale), pos.y, pos.z);
            }
            mbCoinObjRotSet(obj->mdlId[digit], rotX, 0.0f, 0.0f);
            mbCoinObjScaleSet(obj->mdlId[digit], scale, scale, scale);
        }
    }
}

void mbDiceSNpcNumKill(OMOBJ *obj)
{
    DICESNPCNUMWORK *work = omObjGetWork(obj, DICESNPCNUMWORK);
    work->killF = TRUE;
}

void mbDiceSNpcNumDispSet(OMOBJ *obj, int dispF)
{
    DICESNPCNUMWORK *work = omObjGetWork(obj, DICESNPCNUMWORK);
    work->dispF = (u8)dispF;
}

void mbDiceSNpcNumSet(OMOBJ *obj, int value)
{
    DICESNPCNUMWORK *work = omObjGetWork(obj, DICESNPCNUMWORK);
    work->value = (u8)value;
}

void mbDiceSNpcNumPosSet(OMOBJ *obj, HuVecF *pos)
{
    mbVecCopy(pos, &obj->trans);
}

void mbDiceSNpcNumOfsSet(OMOBJ *obj, HuVecF *ofs)
{
    mbVecCopy(ofs, &obj->scale);
}

void mbDiceSNpcNumOfsGet(OMOBJ *obj, HuVecF *ofs)
{
    mbVecCopy(&obj->scale, ofs);
}
