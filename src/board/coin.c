#include "game/board/coin.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/effect.h"
#include "game/board/exit.h"
#include "game/board/math.h"
#include "game/board/object.h"
#include "game/board/player.h"
#include "game/board/telop.h"

#include "game/data.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/memory.h"
#include "game/sprite.h"

#include "string.h"
int abs(int x);

#define COIN_GRP_MAX 64
#define COIN_GRP_OBJ_MAX 64
#define COIN_EFF_MAX 64
#define COIN_MDL_MAX 14
#define COIN_DISP_MAX 5

#define COIN_ATTR_USED 0x1
#define COIN_ATTR_LAYER 0xFFFF
#define COIN_ATTR_TYPE 0xF0000
#define COIN_ATTR_DISP 0x100000

typedef struct CoinObjGrp_s {
    int num;
    int attr[COIN_GRP_OBJ_MAX];
    s8 motNo[COIN_GRP_OBJ_MAX];
    MBCOINOBJ obj[COIN_GRP_OBJ_MAX];
} COINOBJGRP; /* size 0x1144 */

typedef struct CoinObjData_s {
    int hookMdlId[3];
    MBOBJID objId[COIN_MDL_MAX];
    HSF_OBJECT *objPtr[COIN_MDL_MAX];
    COINOBJGRP *grp[COIN_GRP_MAX];
} COINOBJDATA; /* size 0x160 */

typedef struct CoinEff_s {
    HuVecF pos;
    s16 parId;
    s16 num;
} COINEFF;

/* Board particle work (hookData of an mbParticleCreate model). */
typedef struct CoinParticleData_s {
    s16 time; /* 0x00 */
    u8 unk02[6];
    HuVecF vel; /* 0x08 */
    float scaleVel; /* 0x14 */
    float velYAdd; /* 0x18 */
    float velScale; /* 0x1C */
    float swaySpeed; /* 0x20 */
    float swayAngle; /* 0x24 */
    float rotVel; /* 0x28 */
    float scale; /* 0x2C */
    u8 unk30[8];
    float rot; /* 0x38 */
    HuVecF pos; /* 0x3C */
    GXColor color; /* 0x48 */
    s16 animBank; /* 0x4C */
    u8 unk4E[0xE];
} COINPARTICLEDATA; /* size 0x5C */

typedef struct CoinParticle_s {
    s16 unk00;
    s16 effNo; /* 0x02 */
    u8 unk04[0x1C];
    u8 unk20; /* 0x20 */
    u8 unk21[5];
    s16 num; /* 0x26 */
    u8 unk28[0x40];
    COINPARTICLEDATA *data; /* 0x68 */
} COINPARTICLE;

typedef struct CoinDispWork_s {
    u8 killF : 1;
    u8 sign : 1;
    u8 mode : 3;
    u8 modelNum;
    s16 no;
    u16 delay;
    u16 time;
    u16 maxTime;
} COINDISPWORK;

typedef struct CoinDispModel_s {
    HuVecF pos;
    HuVecF scale;
    float rot;
} COINDISPMODEL; /* size 0x1C */

#define COINDISP_MODE_ON 0
#define COINDISP_MODE_MAIN 1
#define COINDISP_MODE_NONE 2
#define COINDISP_MODE_OFF 3
#define COINDISP_MODEL_MAX 5

static COINEFF coinEffData[COIN_EFF_MAX];
static COINOBJDATA coinObjData;

static HUPROCESS *coinMdlProc;
static int coin2MdlId;
static int coin1MdlId;

static OMOBJ *coinDispOMObj[COIN_DISP_MAX] = {};

static int numberFileTbl[] = {
    0x5000B,
    0x5000C,
    0x5000D,
    0x5000E,
    0x5000F,
    0x50010,
    0x50011,
    0x50012,
    0x50013,
    0x50014,
    0x50015,
    0x50016
};

static int coinObjFileTbl[COIN_MDL_MAX] = {
    0x50003,
    0x50004,
    0x5000B,
    0x5000C,
    0x5000D,
    0x5000E,
    0x5000F,
    0x50010,
    0x50011,
    0x50012,
    0x50013,
    0x50014,
    0x50015,
    0x50016
};

static char *coinObjNameTbl[COIN_MDL_MAX] = {
    "Rcoin-coin",
    "coin",
    "num0-no_0",
    "num1-no_1",
    "num2-no_2",
    "num3-no_3",
    "num4-no_4",
    "num5-no_5",
    "num6-no_6",
    "num7-no_7",
    "num8-no_8",
    "num9-no_9",
    "plus",
    "plus-minus"
};

static void CoinInit(void);
static void CoinClose(void);
static void CoinMain(void);

static inline MBOBJID CoinMdlCreate(int dataNum)
{
    return mbObjCreate(mbBoardDataNumGet(dataNum), NULL, TRUE);
}

void mbCoinInit(void)
{
    memset(coinDispOMObj, 0, sizeof(coinDispOMObj));
    memset(coinEffData, 0, sizeof(coinEffData));
    coin1MdlId = CoinMdlCreate(0x50003);
    coin2MdlId = CoinMdlCreate(0x50004);
    mbObjDispSet(coin1MdlId, FALSE);
    mbObjDispSet(coin2MdlId, FALSE);
    CoinInit();
    coinMdlProc = HuPrcChildCreate(CoinMain, 8206, 8192, 0, mbMainProc);
}

void mbCoinClose(void)
{
    int i;
    CoinClose();
    for(i = 0; i < COIN_EFF_MAX; i++) {
        if(coinEffData[i].parId > 0) {
            mbParticleKill(coinEffData[i].parId);
            coinEffData[i].parId = 0;
        }
    }
    if(coin1MdlId > 0) {
        mbObjKill(coin1MdlId);
        coin1MdlId = 0;
    }
    if(coin2MdlId > 0) {
        mbObjKill(coin2MdlId);
        coin2MdlId = 0;
    }
    HuPrcKill(coinMdlProc);
}

void mbCoinEffObjCreate(int objId)
{
    HuVecF pos;
    mbObjPosGet(objId, &pos);
    mbCoinEffCreate(&pos);
    mbObjKill(objId);
}

static void CoinDraw(HU3D_MODEL *modelP, Mtx *mtx);

static void CoinInit(void)
{
    int i;
    int objId;
    for(i = 0; i < COIN_MDL_MAX; i++) {
        objId = mbObjCreate(mbBoardDataNumGet(coinObjFileTbl[i]), NULL, FALSE);
        coinObjData.objId[i] = objId;
        coinObjData.objPtr[i] = Hu3DModelObjPtrGet(mbObjModelIDGet(objId), coinObjNameTbl[i]);
        mbObjDispSet(objId, FALSE);
        if(i >= 2) {
            mbObjMotionSet(objId, 0, 0);
            mbObjMotionTimeSet(objId, 0.5f);
            mbObjMotionSpeedSet(objId, 0.0f);
        }
    }
    for(i = 0; i < COIN_GRP_MAX; i++) {
        coinObjData.grp[i] = NULL;
    }
    coinObjData.hookMdlId[0] = Hu3DHookFuncCreate(CoinDraw);
    Hu3DModelCameraSet(coinObjData.hookMdlId[0], 1);
    Hu3DModelLayerSet(coinObjData.hookMdlId[0], 5);
    coinObjData.hookMdlId[1] = Hu3DHookFuncCreate(CoinDraw);
    Hu3DModelCameraSet(coinObjData.hookMdlId[1], 2);
    Hu3DModelLayerSet(coinObjData.hookMdlId[1], 5);
    coinObjData.hookMdlId[2] = Hu3DHookFuncCreate(CoinDraw);
    Hu3DModelCameraSet(coinObjData.hookMdlId[2], 4);
    Hu3DModelLayerSet(coinObjData.hookMdlId[2], 0);
}

static inline void CoinGrpKill(COINOBJGRP *grp)
{
    HuMemDirectFree(grp);
}

static void CoinClose(void)
{
    int i;
    Hu3DModelKill(coinObjData.hookMdlId[0]);
    coinObjData.hookMdlId[0] = -1;
    Hu3DModelKill(coinObjData.hookMdlId[1]);
    coinObjData.hookMdlId[1] = -1;
    Hu3DModelKill(coinObjData.hookMdlId[2]);
    coinObjData.hookMdlId[2] = -1;
    for(i = 0; i < COIN_GRP_MAX; i++) {
        if(coinObjData.grp[i] != NULL) {
            CoinGrpKill(coinObjData.grp[i]);
            coinObjData.grp[i] = NULL;
        }
    }
    for(i = 0; i < COIN_MDL_MAX; i++) {
        mbObjKill(coinObjData.objId[i]);
        coinObjData.objId[i] = 0;
    }
}

static inline void CoinMtxTransSet(Mtx m, HuVecF *pos)
{
    register HuVecF *src;
    register MtxPtr dst;
    dst = m;
    src = pos;
    asm {
        lfs f4, 0(src)
        lfs f5, 4(src)
        lfs f6, 8(src)
        stfs f4, 0xc(dst)
        stfs f5, 0x1c(dst)
        stfs f6, 0x2c(dst)
    }
}

static void CoinDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    Mtx objMtx;
    int alphaPrev[COIN_MDL_MAX];
    int motNoPrev[COIN_MDL_MAX];
    int mdlId[COIN_MDL_MAX];
    HU3D_MODEL *mdlP[COIN_MDL_MAX];
    int cameraBit = (u16)modelP->cameraBit;
    float motTime[3] = { 0.5f, 1.5f, 2.5f };
    COINOBJGRP **grpP;
    int *attrP;
    MBCOINOBJ *obj;
    int i;
    int j;
    int type;
    int alpha;
    int motNo;

    Hu3DModelObjDrawInit();
    for(i = 0; i < COIN_MDL_MAX; i++) {
        alphaPrev[i] = -1;
        motNoPrev[i] = -1;
        mdlId[i] = mbObjModelIDGet(coinObjData.objId[i]);
        mdlP[i] = &Hu3DData[mdlId[i]];
    }
    for(grpP = &coinObjData.grp[0], i = 0; i < COIN_GRP_MAX; i++, grpP++) {
        if(*grpP != NULL && (*grpP)->num != 0) {
            for(attrP = &(*grpP)->attr[0], j = 0; j < COIN_GRP_OBJ_MAX; j++, attrP++) {
                if(*attrP == 0 || (*attrP & cameraBit) == 0 || (*attrP & COIN_ATTR_DISP) == 0) {
                    continue;
                }
                obj = &(*grpP)->obj[j];
                if(obj->alpha <= 0.0f || 0.0f == obj->scale.x || 0.0f == obj->scale.y || 0.0f == obj->scale.z) {
                    continue;
                }
                        if(0.0f != obj->rot.x) {
                            mbMtxScaleRotXDeg(objMtx, &obj->scale, obj->rot.x);
                        } else {
                            PSMTXScale(objMtx, obj->scale.x, obj->scale.y, obj->scale.z);
                        }
                        if(0.0f != obj->rot.y) {
                            mbMtxRotYDeg(objMtx, obj->rot.y);
                        }
                        if(0.0f != obj->rot.z) {
                            mbMtxRotZDeg(objMtx, obj->rot.z);
                        }
                        CoinMtxTransSet(objMtx, &obj->pos);
                        PSMTXConcat(*mtx, objMtx, objMtx);
                        type = (*attrP & COIN_ATTR_TYPE) >> 16;
                        alpha = 255.0f * obj->alpha;
                        if(alpha != alphaPrev[type]) {
                            alphaPrev[type] = alpha;
                            mbObjAlphaSet(coinObjData.objId[type], alpha);
                        }
                        if(type >= 2) {
                            motNo = (*grpP)->motNo[j];
                            if(motNo != motNoPrev[type]) {
                                motNoPrev[type] = motNo;
                                Hu3DMotionExec(mdlId[type], mdlP[type]->motId, motTime[motNo], FALSE);
                            }
                        }
                        Hu3DModelObjPtrDraw(mdlId[type], coinObjData.objPtr[type], objMtx);
            }
        }
    }
}

static inline void *CoinGrpMalloc(void)
{
    return HuMemDirectMallocNum(HEAP_HEAP, sizeof(COINOBJGRP), HU_MEMNUM_OVL);
}

static inline COINOBJGRP *CoinGrpCreate(void)
{
    COINOBJGRP *grp = CoinGrpMalloc();
    grp->num = 0;
    memset(grp->attr, 0, sizeof(grp->attr));
    memset(grp->motNo, 0, sizeof(grp->motNo));
    return grp;
}

static inline MBCOINID CoinObjCreate(int attr)
{
    int grpNo;
    MBCOINOBJ *obj;
    int slotNo;
    COINOBJGRP *grp;
    int *attrP;
    for(grpNo = 0; grpNo < COIN_GRP_MAX; grpNo++) {
        if(coinObjData.grp[grpNo] != NULL) {
            if(coinObjData.grp[grpNo]->num < COIN_GRP_OBJ_MAX) {
                break;
            }
        } else {
            coinObjData.grp[grpNo] = CoinGrpCreate();
            break;
        }
    }
    grp = coinObjData.grp[grpNo];
    for(attrP = &grp->attr[0], slotNo = 0; slotNo < COIN_GRP_OBJ_MAX; slotNo++, attrP++) {
        if(*attrP == 0) {
            break;
        }
    }
    grp->num++;
    *attrP = attr;
    grp->motNo[slotNo] = 0;
    obj = &grp->obj[slotNo];
    memset(obj, 0, sizeof(MBCOINOBJ));
    obj->scale.x = obj->scale.y = obj->scale.z = 1.0f;
    obj->alpha = 1.0f;
    slotNo |= (grpNo << 6) | 0x4000;
    return slotNo;
}

MBCOINID mbCoinCreate(void)
{
    return CoinObjCreate(COIN_ATTR_DISP | (0 << 16) | COIN_ATTR_USED);
}

MBCOINID mbCoinCreate2(void)
{
    return CoinObjCreate(COIN_ATTR_DISP | (1 << 16) | COIN_ATTR_USED);
}

MBCOINID mbCoinObjCreate(int digit, int motNo)
{
    int id = CoinObjCreate(((digit + 2) << 16) | COIN_ATTR_USED | COIN_ATTR_DISP);
    mbCoinObjMotSet(id, motNo);
    return id;
}

MBCOINID mbCoinObjCreate2(int sign, int motNo)
{
    int id = CoinObjCreate(((sign + 12) << 16) | COIN_ATTR_USED | COIN_ATTR_DISP);
    mbCoinObjMotSet(id, motNo);
    return id;
}

MBCOINOBJ *mbCoinObjGet(MBCOINID id)
{
    COINOBJGRP *grp;
    int grpNo;
    int slotNo;
    id &= 0xFFF;
    grpNo = id >> 6;
    slotNo = id & 0x3F;
    grp = coinObjData.grp[grpNo];
    return &grp->obj[slotNo];
}

void mbCoinObjNumDec(MBCOINID id)
{
    COINOBJGRP *grp;
    int grpNo;
    int slotNo;
    id &= 0xFFF;
    grpNo = id >> 6;
    slotNo = id & 0x3F;
    grp = coinObjData.grp[grpNo];
    grp->attr[slotNo] = 0;
    grp->num--;
}

void mbCoinObjKill(MBCOINID id)
{
    COINOBJGRP *grp;
    int grpNo;
    int slotNo;
    id &= 0xFFF;
    grpNo = id >> 6;
    slotNo = id & 0x3F;
    grp = coinObjData.grp[grpNo];
    mbCoinEffCreate(&grp->obj[slotNo].pos);
    grp->attr[slotNo] = 0;
    grp->num--;
}

void mbCoinObjPosSet(MBCOINID id, float x, float y, float z)
{
    MBCOINOBJ *obj;
    COINOBJGRP *grp;
    int grpNo;
    int slotNo;
    id &= 0xFFF;
    grpNo = id >> 6;
    slotNo = id & 0x3F;
    grp = coinObjData.grp[grpNo];
    obj = &grp->obj[slotNo];
    obj->pos.x = x;
    obj->pos.y = y;
    obj->pos.z = z;
}

void mbCoinObjPosSetV(MBCOINID id, HuVecF *pos)
{
    mbCoinObjPosSet(id, pos->x, pos->y, pos->z);
}

void mbCoinObjPosGet(MBCOINID id, HuVecF *pos)
{
    MBCOINOBJ *obj;
    COINOBJGRP *grp;
    int grpNo;
    int slotNo;
    id &= 0xFFF;
    grpNo = id >> 6;
    slotNo = id & 0x3F;
    grp = coinObjData.grp[grpNo];
    obj = &grp->obj[slotNo];
    mbVecCopy(&obj->pos, pos);
}

void mbCoinObjRotSet(MBCOINID id, float x, float y, float z)
{
    MBCOINOBJ *obj;
    COINOBJGRP *grp;
    int grpNo;
    int slotNo;
    id &= 0xFFF;
    grpNo = id >> 6;
    slotNo = id & 0x3F;
    grp = coinObjData.grp[grpNo];
    obj = &grp->obj[slotNo];
    obj->rot.x = x;
    obj->rot.y = y;
    obj->rot.z = z;
}

void mbCoinObjRotSetV(MBCOINID id, HuVecF *rot)
{
    mbCoinObjRotSet(id, rot->x, rot->y, rot->z);
}

void mbCoinObjRotGet(MBCOINID id, HuVecF *rot)
{
    MBCOINOBJ *obj;
    COINOBJGRP *grp;
    int grpNo;
    int slotNo;
    id &= 0xFFF;
    grpNo = id >> 6;
    slotNo = id & 0x3F;
    grp = coinObjData.grp[grpNo];
    obj = &grp->obj[slotNo];
    mbVecCopy(&obj->rot, rot);
}

void mbCoinObjScaleSet(MBCOINID id, float x, float y, float z)
{
    MBCOINOBJ *obj;
    COINOBJGRP *grp;
    int grpNo;
    int slotNo;
    id &= 0xFFF;
    grpNo = id >> 6;
    slotNo = id & 0x3F;
    grp = coinObjData.grp[grpNo];
    obj = &grp->obj[slotNo];
    obj->scale.x = x;
    obj->scale.y = y;
    obj->scale.z = z;
}

void mbCoinObjScaleSetV(MBCOINID id, HuVecF *scale)
{
    mbCoinObjScaleSet(id, scale->x, scale->y, scale->z);
}

void mbCoinObjScaleGet(MBCOINID id, HuVecF *scale)
{
    MBCOINOBJ *obj;
    COINOBJGRP *grp;
    int grpNo;
    int slotNo;
    id &= 0xFFF;
    grpNo = id >> 6;
    slotNo = id & 0x3F;
    grp = coinObjData.grp[grpNo];
    obj = &grp->obj[slotNo];
    mbVecCopy(&obj->scale, scale);
}

void mbCoinObjAlphaSet(MBCOINID id, float alpha)
{
    MBCOINOBJ *obj;
    COINOBJGRP *grp;
    int grpNo;
    int slotNo;
    id &= 0xFFF;
    grpNo = id >> 6;
    slotNo = id & 0x3F;
    grp = coinObjData.grp[grpNo];
    obj = &grp->obj[slotNo];
    obj->alpha = alpha;
}

float mbCoinObjAlphaGet(MBCOINID id)
{
    MBCOINOBJ *obj;
    COINOBJGRP *grp;
    int grpNo;
    int slotNo;
    id &= 0xFFF;
    grpNo = id >> 6;
    slotNo = id & 0x3F;
    grp = coinObjData.grp[grpNo];
    obj = &grp->obj[slotNo];
    return obj->alpha;
}

void mbCoinObjDispSet(MBCOINID id, BOOL dispF)
{
    COINOBJGRP *grp;
    int grpNo;
    int slotNo;
    id &= 0xFFF;
    grpNo = id >> 6;
    slotNo = id & 0x3F;
    grp = coinObjData.grp[grpNo];
    grp->attr[slotNo] &= ~COIN_ATTR_DISP;
    if(dispF) {
        grp->attr[slotNo] |= COIN_ATTR_DISP;
    }
}

BOOL mbCoinObjDispGet(MBCOINID id)
{
    COINOBJGRP *grp;
    int grpNo;
    int slotNo;
    id &= 0xFFF;
    grpNo = id >> 6;
    slotNo = id & 0x3F;
    grp = coinObjData.grp[grpNo];
    return (grp->attr[slotNo] & COIN_ATTR_DISP) == COIN_ATTR_DISP;
}

void mbCoinObjLayerSet(MBCOINID id, u16 layer)
{
    COINOBJGRP *grp;
    int grpNo;
    int slotNo;
    id &= 0xFFF;
    grpNo = id >> 6;
    slotNo = id & 0x3F;
    grp = coinObjData.grp[grpNo];
    grp->attr[slotNo] &= ~COIN_ATTR_LAYER;
    grp->attr[slotNo] |= layer;
}

void mbCoinObjMotSet(MBCOINID id, int motNo)
{
    COINOBJGRP *grp;
    int grpNo;
    int slotNo;
    id &= 0xFFF;
    grpNo = id >> 6;
    slotNo = id & 0x3F;
    grp = coinObjData.grp[grpNo];
    grp->motNo[slotNo] = motNo;
}

static void CoinEffCreate(int effNo, HuVecF *pos);

static void CoinMain(void)
{
    HuVecF pos = { 0.0f, 0.0f, 0.0f };
    COINEFF *eff;
    COINOBJGRP **grpP;
    COINOBJGRP *grp;
    int i;
    int j;
    int k;
    int num;
    for(i = 0; i < 5; i++) {
        CoinEffCreate(i, &pos);
    }
    for(j = 0, k = 0; k < 2; j++) {
        if(coinObjData.grp[j] == NULL) {
            coinObjData.grp[j] = CoinGrpCreate();
            k++;
        }
    }
    while(1) {
        num = 0;
        for(eff = &coinEffData[i], k = i; k < COIN_EFF_MAX; k++, eff++) {
            if(eff->parId != 0) {
                num++;
                if(eff->num == 0) {
                    mbParticleKill(eff->parId);
                    eff->parId = 0;
                }
            }
        }
        num = 0;
        for(grpP = &coinObjData.grp[j], k = j; k < COIN_GRP_MAX; k++, grpP++) {
            if(*grpP != NULL && (*grpP)->num == 0) {
                grp = *grpP;
                HuMemDirectFree(grp);
                *grpP = NULL;
            }
        }
        HuPrcVSleep();
    }
}

static u8 coinEffColorTbl[4][4] = {
    { 220, 220, 64, 120 },
    { 220, 220, 220, 120 },
    { 64, 160, 220, 140 },
    { 220, 140, 140, 140 }
};

static u8 coinEffColorNoTbl[16] = {
    0, 1, 0, 1,
    0, 1, 0, 1,
    0, 0, 2, 3,
    2, 3, 2, 3
};

static u8 coinEffBankTbl[16] = {
    2, 2, 2, 2,
    2, 3, 3, 3,
    2, 2, 2, 2,
    2, 3, 3, 3
};

static u8 coinEffBankTbl2[16] = {
    2, 2, 2, 2,
    2, 3, 3, 3,
    2, 3, 2, 3,
    2, 3, 0, 1
};

static inline float CoinFabs(register float x)
{
    asm {
        fabs x, x
    }
    return x;
}

void mbCoinEffCreate(HuVecF *pos)
{
    HuVecF ofs;
    COINEFF *eff;
    int colorNo;
    COINPARTICLEDATA *data;
    int zero;
    int effNo;
    int i;
    COINPARTICLE *particle;
    int cnt;
    float angleY;
    float angleX;
    mbAudFXPlay(7);
    effNo = -1;
    for(eff = &coinEffData[0], i = 0; i < COIN_EFF_MAX; i++, eff++) {
        if(eff->parId != 0) {
            if(eff->num == 0) {
                effNo = i;
                break;
            }
            if(eff->num <= 80 && CoinFabs(pos->x - eff->pos.x) < 300.0f && CoinFabs(pos->y - eff->pos.y) < 300.0f && CoinFabs(pos->z - eff->pos.z) < 300.0f) {
                effNo = i;
                break;
            }
        } else if(effNo < 0) {
            effNo = i;
            break;
        }
    }
    if(effNo < 0) {
        return;
    }
    CoinEffCreate(effNo, pos);
    eff = &coinEffData[effNo];
    zero = 0;
    particle = Hu3DData[eff->parId].hookData;
    PSVECSubtract(pos, &eff->pos, &ofs);
    cnt = 20;
    eff->num += cnt;
    for(data = particle->data, i = 0; i < particle->num && cnt != 0; i++, data++) {
        if(data->time == 0) {
            colorNo = coinEffColorNoTbl[mbRandMod(16)];
            data->color.r = coinEffColorTbl[colorNo][0] + mbRandMod(20);
            data->color.g = coinEffColorTbl[colorNo][1] + mbRandMod(20);
            data->color.b = coinEffColorTbl[colorNo][2] + mbRandMod(20);
            data->color.a = coinEffColorTbl[colorNo][3] + mbRandMod(20);
            data->animBank = coinEffBankTbl[mbRandMod(32)];
            data->pos.x = data->pos.y = data->pos.z = 0.0f;
            angleY = 360.0f * frandf();
            angleX = (1.7f * frandf()) - 0.7f;
            angleX = 90.0f * (angleX * CoinFabs(angleX));
            data->vel.x = mbSinDeg(angleY) * mbCosDeg(angleX);
            data->vel.y = mbSinDeg(angleX);
            data->vel.z = mbCosDeg(angleY) * mbCosDeg(angleX);
            PSVECScale(&data->vel, &data->pos, 100.0f * (0.5f * frandf()));
            PSVECScale(&data->vel, &data->vel, 0.016666668f * (300.0f + (100.0f * (4.0f * frandf()))));
            PSVECAdd(&data->pos, &ofs, &data->pos);
            data->swaySpeed = 0.016666668f * (100.0f * (0.7f * (0.2f + (0.3f * frandf()))));
            data->swayAngle = 90.0f + angleY;
            data->scale = 70.0f * (0.5f + (0.5f * frandf()));
            data->scaleVel = -1.3f;
            data->rot = 360.0f * frandf();
            data->rotVel = (20.0f * frandf()) - 10.0f;
            data->time = mbRandMod(10) + 20;
            data->velYAdd = 0.44444448f;
            data->velScale = 0.92f;
            cnt--;
        }
    }
}

static void CoinEffHook(HU3D_MODEL *modelP, COINPARTICLE *particle, Mtx mtx);

static void CoinEffCreate(int effNo, HuVecF *pos)
{
    COINEFF *eff;
    COINPARTICLE *particle;
    COINPARTICLEDATA *data;
    int i;
    eff = &coinEffData[effNo];
    if(eff->parId <= 0) {
        eff->parId = mbParticleCreate(HuSprAnimDataRead(mbBoardDataNumGet(0x50065)), 100);
        mbParticleHookSet(eff->parId, CoinEffHook);
        Hu3DModelLayerSet(eff->parId, 5);
        eff->pos.x = pos->x;
        eff->pos.y = pos->y;
        eff->pos.z = pos->z;
        Hu3DModelPosSetV(eff->parId, pos);
        eff->num = 0;
        particle = Hu3DData[eff->parId].hookData;
        for(data = particle->data, i = 0; i < particle->num; i++, data++) {
            data->scale = 0.0f;
            data->color.a = 0;
            data->time = 0;
        }
        particle->unk20 = 1;
        particle->effNo = effNo;
    } else if(eff->num == 0) {
        eff->pos.x = pos->x;
        eff->pos.y = pos->y;
        eff->pos.z = pos->z;
        Hu3DModelPosSetV(eff->parId, pos);
    }
}

static void CoinEffHook(HU3D_MODEL *modelP, COINPARTICLE *particle, Mtx mtx)
{
    COINPARTICLEDATA *data;
    int num;
    int dataNum;
    int i;
    num = 0;
    dataNum = particle->num;
    for(data = particle->data, i = 0; i < particle->num; i++, data++) {
        if(data->time != 0) {
            num++;
            PSVECScale(&data->vel, &data->vel, data->velScale);
            PSVECAdd(&data->pos, &data->vel, &data->pos);
            data->vel.y += data->velYAdd;
            data->vel.x += data->swaySpeed * mbSinDeg(data->swayAngle);
            data->vel.z += data->swaySpeed * mbCosDeg(data->swayAngle);
            data->swayAngle += 5.0f;
            data->swaySpeed += 0.02777778f;
            data->scale += data->scaleVel;
            data->rot += data->rotVel;
            data->time--;
            if(data->time < 10) {
                data->color.a *= 0.8f;
                if(data->time == 0) {
                    data->color.a = 0;
                    data->scale = 0.0f;
                    num--;
                }
            }
        }
    }
    coinEffData[particle->effNo].num = num;
}

static void CoinDispOMExec(OMOBJ *obj);
static void CoinDispCreate(OMOBJ *obj, int coinNum);
static void CoinDispObjUpdate(OMOBJ *obj);

s16 mbCoinDispCreate(HuVecF *pos, int coinNum, int signNum, BOOL seF)
{
    OMOBJ *obj;
    COINDISPWORK *work;
    s8 i;
    for(i = 1; i < COIN_DISP_MAX; i++) {
        if(!coinDispOMObj[i]) {
            break;
        }
    }
    if(i >= COIN_DISP_MAX) {
        return MBCOIN_DISP_NONE;
    }
    if(coinNum > 999) {
        coinNum = 999;
    } else if(coinNum < -999) {
        coinNum = -999;
    }
    obj = omAddObjEx(mbObjMan, 261, 5, 0, -1, CoinDispOMExec);
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, COINDISP_MODEL_MAX * sizeof(COINDISPMODEL), HU_MEMNUM_OVL);
    omSetStatBit(obj, OM_STAT_MODELPAUSE);
    coinDispOMObj[i] = obj;
    work = omObjGetWork(obj, COINDISPWORK);
    work->killF = FALSE;
    if(coinNum != 0) {
        work->sign = (coinNum < 0) ? 1 : 0;
    } else {
        work->sign = (signNum < 0) ? 1 : 0;
    }
    work->mode = COINDISP_MODE_ON;
    work->no = i;
    work->delay = 0;
    work->time = 0;
    obj->trans.x = pos->x;
    obj->trans.y = pos->y;
    obj->trans.z = pos->z;
    obj->rot.x = 0.0f;
    obj->rot.y = 0.01f;
    CoinDispCreate(obj, coinNum);
    CoinDispObjUpdate(obj);
    if(seF) {
        if(!work->sign) {
            mbAudFXPlay(1127);
        } else {
            mbAudFXPlay(1128);
        }
    }
    return work->no;
}

s16 mbCoinDispMasuCreate(HuVecF *pos, int coinNum, BOOL seF)
{
    return mbCoinDispCreate(pos, coinNum, 1, seF);
}

s16 mbCoinDispCapsuleCreate(HuVecF *pos, int coinNum)
{
    return mbCoinDispCreate(pos, coinNum, 1, TRUE);
}

static void CoinDispCreate(OMOBJ *obj, int coinNum)
{
    COINDISPWORK *work = omObjGetWork(obj, COINDISPWORK);
    COINDISPMODEL *model = obj->data;
    int modelNum;
    int digit;
    int digitVal;
    BOOL dispF = FALSE;
    float motTime;
    int i;

    obj->mdlId[0] = mbCoinCreate();
    if(work->sign) {
        motTime = 2.5f;
    } else {
        motTime = 1.5f;
    }
    obj->mdlId[1] = mbCoinObjCreate2(work->sign, (work->sign) ? 2 : 1);
    digitVal = 100;
    work->modelNum = 0;
    modelNum = 2;
    coinNum = abs(coinNum);
    for(i = 0; i < 3; i++) {
        digit = coinNum / digitVal;
        if(i == 2) {
            dispF = TRUE;
        }
        if(dispF || digit != 0) {
            dispF = TRUE;
            obj->mdlId[modelNum] = mbCoinObjCreate(digit, (work->sign) ? 2 : 1);
            modelNum++;
        }
        coinNum -= digit * digitVal;
        digitVal /= 10;
    }
    work->modelNum = modelNum;
    for(model = obj->data, i = 0; i < work->modelNum; i++, model++) {
        model->pos.x = model->pos.y = model->pos.z = 0.0f;
        model->rot = 0.0f;
        model->scale.x = model->scale.y = model->scale.z = 0.001f;
        mbCoinObjLayerSet(obj->mdlId[i], 2);
    }
}

static void CoinDispOn(OMOBJ *obj);
static void CoinDispMain(OMOBJ *obj);
static void CoinDispOff(OMOBJ *obj);
static void CoinDispObjKill(OMOBJ *obj);

static void CoinDispOMExec(OMOBJ *obj)
{
    COINDISPWORK *work = omObjGetWork(obj, COINDISPWORK);
    if(work->killF || mbExitCheck()) {
        CoinDispObjKill(obj);
        coinDispOMObj[work->no] = NULL;
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    if(work->delay == 0) {
        switch(work->mode) {
            case COINDISP_MODE_ON:
                CoinDispOn(obj);
                break;

            case COINDISP_MODE_MAIN:
                CoinDispMain(obj);
                break;

            case COINDISP_MODE_OFF:
                CoinDispOff(obj);
                break;
        }
    } else {
        work->delay--;
    }
    CoinDispObjUpdate(obj);
}

static void CoinDispObjUpdate(OMOBJ *obj)
{
    COINDISPWORK *work = omObjGetWork(obj, COINDISPWORK);
    COINDISPMODEL *model = obj->data;
    Mtx rotMtx;
    HuVecF pos;
    HuVecF pos2D;
    float rotY;
    float z;
    float scale;
    int i;
    mbCameraRotGet(&pos2D);
    rotY = pos2D.y;
    PSMTXRotRad(rotMtx, 'Y', 0.017453292f * rotY);
    for(i = 0; i < work->modelNum; i++, model++) {
        PSMTXMultVec(rotMtx, &model->pos, &pos2D);
        PSVECAdd(&obj->trans, &pos2D, &pos);
        mbPos3Dto2D(&pos, &pos2D);
        z = pos2D.z;
        pos2D.z = 800.0f;
        mbPos2Dto3D(&pos2D, &pos);
        scale = 800.0f / z;
        mbCoinObjPosSetV(obj->mdlId[i], &pos);
        mbCoinObjRotSet(obj->mdlId[i], 0.0f, model->rot + rotY, 0.0f);
        mbCoinObjScaleSet(obj->mdlId[i], model->scale.x * scale, model->scale.y * scale, model->scale.z * scale);
    }
}

static void CoinDispOn(OMOBJ *obj)
{
    COINDISPWORK *work = omObjGetWork(obj, COINDISPWORK);
    COINDISPMODEL *model = obj->data;
    float s;
    float scale;
    int i;
    s = mbSinDeg(work->time);
    scale = s;
    obj->rot.x = 405.0f * s;
    model->scale.x = model->scale.y = model->scale.z = scale;
    model->pos.x = model->pos.y = model->pos.z = 0.0f;
    model->rot = obj->rot.x;
    if(work->time < 90) {
        work->time += 6;
        return;
    }
    work->mode = COINDISP_MODE_MAIN;
    work->time = 0;
    work->maxTime = 30;
    for(model = ((COINDISPMODEL *)obj->data) + 1, i = 1; i < work->modelNum; i++, model++) {
        model->scale.x = model->scale.y = model->scale.z = scale;
        model->pos.x = model->pos.y = model->pos.z = 0.0f;
        model->rot = obj->rot.x;
    }
}

static void CoinDispMain(OMOBJ *obj)
{
    COINDISPWORK *work = omObjGetWork(obj, COINDISPWORK);
    COINDISPMODEL *model = obj->data;
    float weight = (float)work->time / work->maxTime;
    float ofsX = -0.5f * (120.00001f * (work->modelNum - 1));
    float s = mbSinDeg(90.0f * weight);
    float angle;
    float sy;
    int i;
    obj->rot.x = 45.0f + (315.0f * s);
    angle = weight * (180.0f + (30.0f * (work->modelNum - 1)));
    for(i = 0; i < work->modelNum; i++, model++) {
        sy = mbSinDeg(angle);
        if(sy < 0.0f) {
            sy = 0.0f;
        }
        model->pos.x = s * ofsX;
        model->pos.y = 200.0f * sy;
        model->pos.z = 0.0f;
        model->rot = obj->rot.x;
        ofsX += 120.00001f;
        angle -= 30.0f;
    }
    if(++work->time > work->maxTime) {
        work->mode = COINDISP_MODE_OFF;
        work->time = 0;
        work->maxTime = 24;
        work->delay = 30;
    }
}

static void CoinDispOff(OMOBJ *obj)
{
    COINDISPWORK *work = omObjGetWork(obj, COINDISPWORK);
    COINDISPMODEL *model = obj->data;
    float weight = (float)work->time / work->maxTime;
    float angle;
    float s;
    float posY;
    float angleBase;
    int i;
    obj->rot.x = 90.0f + (270.0f * mbSinDeg(90.0f * weight));
    angleBase = weight * (90.0f + (30.0f * (work->modelNum - 1)));
    for(i = 0; i < work->modelNum; i++, model++) {
        angle = angleBase - (30.0f * i);
        if(angle < 0.0f) {
            angle = 0.0f;
        } else if(angle > 90.0f) {
            angle = 90.0f;
        }
        s = mbSinDeg(angle);
        if(work->sign) {
            posY = -100.0f * s;
        } else {
            posY = 100.0f * s;
        }
        model->pos.y = posY;
        model->rot = obj->rot.x;
        mbCoinObjAlphaSet(obj->mdlId[i], mbCosDeg(angle));
    }
    if(++work->time > work->maxTime) {
        for(i = 0; i < work->modelNum; i++) {
            mbCoinObjDispSet(obj->mdlId[i], FALSE);
        }
        work->killF = TRUE;
    }
}

static void CoinDispObjKill(OMOBJ *obj)
{
    COINDISPWORK *work = omObjGetWork(obj, COINDISPWORK);
    int i;
    for(i = 0; i < work->modelNum; i++) {
        if(obj->mdlId[i] >= 0) {
            mbCoinObjNumDec(obj->mdlId[i]);
        }
        obj->mdlId[i] = -1;
    }
}

void mbCoinDispKill(s16 no)
{
    if(no <= 0 || no >= COIN_DISP_MAX) {
        return;
    }
    if(coinDispOMObj[no]) {
        omObjGetWork(coinDispOMObj[no], COINDISPWORK)->killF = TRUE;
    }
}

BOOL mbCoinDispKillCheck(s16 no)
{
    if(no <= 0 || no >= COIN_DISP_MAX) {
        return TRUE;
    }
    if(coinDispOMObj[no]) {
        COINDISPWORK *work = omObjGetWork(coinDispOMObj[no], COINDISPWORK);
        return FALSE;
    } else {
        return TRUE;
    }
}

static inline int CoinAdd(int playerNo, int coinNum, BOOL fastF)
{
    int i;
    int coinChg;
    int coinAdd;
    int coinDiff;
    int coinNew;
    int delay;
    s16 seId;
    if(abs(coinNum) >= 50) {
        delay = 1;
    } else if(abs(coinNum) >= 20) {
        delay = 3;
    } else {
        delay = 6;
    }
    coinNew = coinNum + mbPlayerCoinGet(playerNo);
    coinAdd = coinNum;
    if(coinNew > 999) {
        coinAdd = 999 - mbPlayerCoinGet(playerNo);
    } else if(coinNew < 0) {
        coinAdd = -mbPlayerCoinGet(playerNo);
    }
    coinDiff = coinAdd;
    if(!fastF) {
        coinChg = (coinAdd >= 0) ? 1 : -1;
        seId = (coinChg > 0) ? 7 : 14;
        for(i = 0; i < abs(coinAdd); i++) {
            mbPlayerCoinAdd(playerNo, coinChg);
            mbAudFXPlay(seId);
            HuPrcSleep(delay);
        }
    } else {
        mbPlayerCoinAdd(playerNo, coinDiff);
    }
    return coinDiff;
}

int mbCoinAddProcExec(int playerNo, int coinNum, int signNum, BOOL fastF)
{
    HuVecF pos;
    int coinDiff;
    int dispNo;
    coinDiff = CoinAdd(playerNo, coinNum, fastF);
    if(coinDiff != 0 || signNum != 0) {
        mbAudFXPlay(15);
        mbPlayerPosGet(playerNo, &pos);
        pos.y += 250.0f;
        dispNo = mbCoinDispCreate(&pos, coinDiff, signNum, TRUE);
        while(!mbCoinDispKillCheck(dispNo)) {
            HuPrcVSleep();
        }
    }
    return coinDiff;
}

int mbCoinAddDispExec(int playerNo, int coinNum, BOOL dispF, BOOL fastF)
{
    HuVecF pos;
    int coinDiff;
    s16 dispNo;
    coinDiff = CoinAdd(playerNo, coinNum, fastF);
    if(coinDiff != 0) {
        mbAudFXPlay(15);
    }
    if(dispF && coinDiff != 0) {
        mbPlayerPosGet(playerNo, &pos);
        pos.y += 250.0f;
        dispNo = mbCoinDispCapsuleCreate(&pos, coinDiff);
        while(!mbCoinDispKillCheck(dispNo)) {
            HuPrcVSleep();
        }
    }
    return coinDiff;
}

int mbCoinAddExec(int playerNo, int coinNum)
{
    return mbCoinAddDispExec(playerNo, coinNum, FALSE, FALSE);
}

int mbStatTeamMinValGet(int teamNo, int base, int max, int *value, int *out)
{
    int playerNo[2];
    int total;
    int i;
    int j;
    int player;
    total = base;
    for(i = 0; i < 2; i++) {
        player = mbPlayerTeamFindPlayer(teamNo, i);
        playerNo[i] = player;
        total += value[player];
        out[player] = value[player];
    }
    if(total > max) {
        if(value[playerNo[0]] > value[playerNo[1]]) {
            player = playerNo[0];
            playerNo[0] = playerNo[1];
            playerNo[1] = player;
        }
        total = base;
        for(j = 0; j < 2; j++) {
            player = playerNo[j];
            if(total + value[player] > max) {
                out[player] = max - total;
                total = max;
            } else {
                total += value[player];
            }
        }
    } else if(total < 0) {
        if(value[playerNo[0]] < value[playerNo[1]]) {
            player = playerNo[0];
            playerNo[0] = playerNo[1];
            playerNo[1] = player;
        }
        total = base;
        for(j = 0; j < 2; j++) {
            player = playerNo[j];
            if(total + value[player] < 0) {
                out[player] = -total;
                total = 0;
            } else {
                total += value[player];
            }
        }
    }
    return total - base;
}

static inline BOOL mbTeamFGet(void)
{
    return GwSystem.tagF;
}

static void CoinAddAllProc(int *coinNumIn, BOOL fastF, int *coinDiff)
{
    int coinNum[GW_PLAYER_MAX];
    int delay[GW_PLAYER_MAX];
    int time[GW_PLAYER_MAX];
    int coinChg[GW_PLAYER_MAX];
    int activeF[GW_PLAYER_MAX];
    int i;
    int num;
    int coinNew;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        coinNum[i] = coinNumIn[i];
        activeF[i] = FALSE;
    }
    if(!mbTeamFGet()) {
        for(i = 0; i < GW_PLAYER_MAX; i++) {
            coinNew = coinNum[i] + mbPlayerCoinGet(i);
            if(coinNew > 999) {
                coinNum[i] = 999 - mbPlayerCoinGet(i);
            } else if(coinNew < 0) {
                coinNum[i] = -mbPlayerCoinGet(i);
            }
            coinDiff[i] = coinNum[i];
            activeF[i] = TRUE;
        }
    } else {
        for(i = 0; i < 2; i++) {
            num = mbPlayerTeamFindPlayer(i, 0);
            activeF[num] = TRUE;
            coinNum[num] = mbStatTeamMinValGet(i, mbPlayerTeamCoinGet(i), 999, coinNum, coinDiff);
        }
    }
    if(!fastF) {
        for(i = 0; i < GW_PLAYER_MAX; i++) {
            if(activeF[i]) {
                if(abs(coinNum[i]) >= 50) {
                    delay[i] = 1;
                } else if(abs(coinNum[i]) >= 20) {
                    delay[i] = 3;
                } else {
                    delay[i] = 6;
                }
                time[i] = delay[i];
                coinChg[i] = (coinNum[i] >= 0) ? 1 : -1;
            }
        }
        do {
            num = 0;
            for(i = 0; i < GW_PLAYER_MAX; i++) {
                if(activeF[i] && coinNum[i] != 0) {
                    if(--time[i] == 0) {
                        mbPlayerCoinAdd(i, coinChg[i]);
                        coinNum[i] -= coinChg[i];
                        mbAudFXPlay(7);
                        time[i] = delay[i];
                    }
                    num++;
                }
            }
            HuPrcVSleep();
        } while(num != 0);
    } else {
        for(i = 0; i < GW_PLAYER_MAX; i++) {
            if(activeF[i]) {
                mbPlayerCoinAdd(i, coinNum[i]);
            }
        }
    }
    mbAudFXPlay(15);
}

void mbCoinAddAllProcExecV(int *coinNum, int *signNum, BOOL fastF)
{
    int coinDiff[GW_PLAYER_MAX];
    int dispNo[GW_PLAYER_MAX];
    HuVecF pos;
    int i;
    int waitF;
    CoinAddAllProc(coinNum, fastF, coinDiff);
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        dispNo[i] = 0;
        if(coinDiff[i] != 0 || signNum[i] != 0) {
            mbPlayerPosGet(i, &pos);
            pos.y += 250.0f;
            dispNo[i] = mbCoinDispCreate(&pos, coinDiff[i], signNum[i], TRUE);
        }
    }
    do {
        waitF = FALSE;
        for(i = 0; i < GW_PLAYER_MAX; i++) {
            if(dispNo[i] != 0 && !mbCoinDispKillCheck(dispNo[i])) {
                waitF = TRUE;
            }
        }
        HuPrcVSleep();
    } while(waitF);
}

void mbCoinAddAllProcExec(int coinNumP1, int coinNumP2, int coinNumP3, int coinNumP4, BOOL dispF, BOOL fastF)
{
    int coinNum[GW_PLAYER_MAX];
    int coinDiff[GW_PLAYER_MAX];
    HuVecF pos;
    int i;
    int dispNo;
    coinNum[0] = coinNumP1;
    coinNum[1] = coinNumP2;
    coinNum[2] = coinNumP3;
    coinNum[3] = coinNumP4;
    CoinAddAllProc(coinNum, fastF, coinDiff);
    if(dispF) {
        for(i = 0; i < GW_PLAYER_MAX; i++) {
            if(coinDiff[i] != 0) {
                mbPlayerPosGet(i, &pos);
                pos.y += 250.0f;
                dispNo = mbCoinDispCapsuleCreate(&pos, coinDiff[i]);
            }
        }
        while(!mbCoinDispKillCheck(dispNo)) {
            HuPrcVSleep();
        }
    }
}

void mbCoinAddAllExec(int coinNumP1, int coinNumP2, int coinNumP3, int coinNumP4)
{
    int coinNum[GW_PLAYER_MAX];
    int coinDiff[GW_PLAYER_MAX];
    coinNum[0] = coinNumP1;
    coinNum[1] = coinNumP2;
    coinNum[2] = coinNumP3;
    coinNum[3] = coinNumP4;
    CoinAddAllProc(coinNum, FALSE, coinDiff);
}
