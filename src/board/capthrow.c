/* @@HEAD@@ */
#include "game/board/capthrow.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/capselect.h"
#include "game/board/capsule.h"
#include "game/board/coin.h"
#include "game/board/dice.h"
#include "game/board/exit.h"
#include "game/board/masu.h"
#include "game/board/math.h"
#include "game/board/object.h"
#include "game/board/player.h"
#include "game/board/window.h"
#include "game/board/wipe.h"

#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/process.h"
#include "humath.h"
#include "math.h"
#include "string.h"

/* Capsule event work (capevent.c), the process property of every capsule event. Size 0xBF4. */
typedef struct CapEventWork_s {
    /* 0x000 */ int playerNo;
    /* 0x004 */ int targetNo; /* the other player the capsule acts on */
    /* 0x008 */ int capNo;
    /* 0x00C */ u8 unkC[0x20 - 0xC];
    /* 0x020 */ u8 objList[0xB6C - 0x20]; /* event object list (mbev_CapObjCreate) */
    /* 0xB6C */ int objId;
    /* 0xB70 */ u8 unkB70[0xBD0 - 0xB70];
    /* 0xBD0 */ void *effExplode;
    /* 0xBD4 */ void *effBD4;
    /* 0xBD8 */ void *effSnow;
    /* 0xBDC */ void *effGlow;
    /* 0xBE0 */ void *effRing;
    /* 0xBE4 */ void *effCoin;
    /* 0xBE8 */ void *coinMan;
    /* 0xBEC */ u8 unkBEC[0xBF0 - 0xBEC];
    /* 0xBF0 */ void *effCapLose;
} CAPEVENTWORK;

/* capevent.c prototypes as retail calls them here (local declarations, not capevent.h). */
void mbev_CapWait(CAPEVENTWORK *work);
void *mbev_CapEffExplodeCreate(void);
void *mbev_CapEffGlowCreate(void);
void *mbev_CapEffRingHitCreate(void);
void *mbev_CapEffCoinCreate(void);
void mbev_CapEffCoinGlowSet(void *effCoin, void *effGlow);
int mbev_CapEffCoinAdd(void *eff, HuVecF *pos, HuVecF *vel, float scale, float gravity, int time, int arg6);
void mbev_CapEffGlowCoinAdd(void *eff, HuVecF *pos, HuVecF *rot);
void mbev_CapEffRingHitAdd(void *eff, HuVecF pos, HuVecF rot, HuVecF scale);
int mbev_CapEffExplodeAdd(void *eff, HuVecF pos, HuVecF vel, float scale, float speed, float alpha, GXColor color);
void mbev_CapObjPosSet(void *objList, int objId, int masuId, int arg3);
void mbev_CapPlayerMoveHitCreate(int playerNo, int arg1, int arg2);
void mbev_CapPlayerMoveEjectCreate(int playerNo, int arg1);
void mbev_CapPlayerMoveMinYSet(int playerNo, float minY);
void mbev_CapPlayerMoveVelSet(int playerNo, float speed, HuVecF vel);
int mbev_CapPlayerMoveObjCheck(int playerNo);
void mbev_CapPlayerIdleWait(void);
void mbev_CapBezierGetV(float t, HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *out);
void mbev_CapBezierNormGetV(float t, HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *out);
void mbev_CapVecRotGet(HuVecF *dir, HuVecF *rot);
/* Used by part 3 (wt-p09); these belong in capevent.h (owned by capevent part 1). */
void *mbev_CapMalloc(void *objList, int size);
int mbev_CapPlayerCheck(int playerNo, int otherNo);
int mbev_CapPlayerMotShiftCheck(int playerNo);
void mbev_CapObjMotionSet(int objId, int arg1, int motNo, int arg3, int arg4, u32 attr, int arg6, int arg7);
void mbev_CapPlayerMotionSet(int playerNo, int arg1, int motNo, int arg3, int arg4, u32 attr, int arg6, int arg7);
void *mbev_CapEffSnowCreate(void);
void *mbev_CapEffCapLoseCreate(void);
void mbev_CapEffColorSet(GXColor *color, int no);
int mbev_CapEffGlowAdd(void *eff, HuVecF pos, HuVecF vel, int time, float scale, float speed, float decay, GXColor color);
int mbev_CapEffGlowKinokoTimeSet(void *eff, int no, int arg2, int time);
void mbev_CapEffDustHeavyAdd(void *eff, HuVecF pos);
int mbev_CapEffSnowAdd(void *eff, HuVecF *pos, int time);
void mbev_CapEffCapLoseAdd(void *eff, int playerNo, int num, float speed);
int mbev_CapEffCapLoseNumGet(void *eff);
int mbev_CapCullCheck(int, int);
void mbev_CapPlayerMotShiftSet(int objId, int motNo, u32 attr, BOOL waitF);
void mbev_CapPlayerMoveObjInit(void);
s16 mbev_CapPlayerMotionCreate(void *objList, int playerNo, int dataNum);
int mbev_CapObjCreate(void *objList, int dataNum, int *motData, int arg3, int arg4, int arg5);
void mbev_CapVecChase(float t, HuVecF *a, HuVecF *b, HuVecF *out);
float mbev_CapAngleSumLerp(float t, float start, float end);
int mbev_CapCoinDisp(int playerNo, int coinNum, int arg2, int arg3);
void mbev_CapCoinAdd(void *effCoin, int playerNo, int coinNum, int arg3);
void mbev_CapPlayerMotShiftWait(int playerNo, int motNo, u32 attr, BOOL waitF);
void *mbev_CapCoinManCreate(void);
int mbev_CapCoinManNumGet(void *coinMan);
int mbev_CapCoinManAdd(void *coinMan, HuVecF *start, HuVecF *end, int playerNo, int arg4);

/* Retail calls sprintf unprototyped here (no crclr before the call). */
int sprintf();

#define CAPTHROW_RANDF() ((1.0f/268435456.0f)*mbRandMod(0x10000000))

/* Togezo: the thrown objects (ev_CapTogezoOMExec), obj->data. Size 0x44. */
typedef struct TogezoWork_s {
    /* 0x00 */ int objId;
    /* 0x04 */ int mode;
    /* 0x08 */ int spinF;
    /* 0x0C */ int time;
    /* 0x10 */ int maxTime;
    /* 0x14 */ HuVecF pos;
    /* 0x20 */ HuVecF vel;
    /* 0x2C */ HuVecF rot;
    /* 0x38 */ HuVecF target;
} TOGEZOWORK;

static void ev_CapTogezoOMExec(OMOBJ *obj);
static void KokamekkuObjUpdate(float t, float angle, int *objId, int num, int curNo, int skipNo, HuVecF *center, HuVecF *playerPos);

/* @@PART1@@ */
void mbev_CapTogezo(void)
{
    CAPEVENTWORK *work = HuPrcCurrentGet()->property;
    TOGEZOWORK *objWork;
    OMOBJ *obj;
    HU3D_MODEL *modelP;
    int playerNo;
    int targetNo;
    int objId;
    int masuId;
    int targetMasuId;
    int i;
    int j;
    int coinNum;
    int coinSum;
    int coinTotal;
    float t;
    float angle;
    float dist;
    float elev;
    HuVecF throwPos[3];
    int motId[16];
    HuVecF targetPos;
    HuVecF playerPos;
    HuVecF coinPos;
    HuVecF coinVel;
    HuVecF pos;
    HuVecF rot;
    HuVecF startPos;
    HuVecF scale;

    mbev_CapWait(work);
    work->effGlow = mbev_CapEffGlowCreate();
    work->effRing = mbev_CapEffRingHitCreate();
    work->effCoin = mbev_CapEffCoinCreate();
    mbev_CapEffCoinGlowSet(work->effCoin, work->effGlow);
    mbev_CapPlayerMoveObjInit();
    playerNo = work->playerNo;
    targetNo = work->targetNo;
    masuId = GwPlayer[playerNo].masuId;
    targetMasuId = GwPlayer[targetNo].masuId;
    mbPlayerPosGet(targetNo, &targetPos);
    mbPlayerPosGet(playerNo, &playerPos);
    motId[0] = mbev_CapPlayerMotionCreate(work->objList, playerNo, 0x930017);
    motId[1] = mbev_CapPlayerMotionCreate(work->objList, playerNo, 0x930019);
    motId[2] = mbev_CapPlayerMotionCreate(work->objList, playerNo, 0x930022);
    objId = mbev_CapObjCreate(work->objList, 0xF0000, NULL, 0, 5, 0);
    mbObjDispSet(objId, FALSE);
    obj = omAddObjEx(mbObjMan, 32768, 0, 0, -1, ev_CapTogezoOMExec);
    objWork = obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(TOGEZOWORK), HU_MEMNUM_OVL);
    memset(obj->data, 0, sizeof(TOGEZOWORK));
    objWork->objId = mbev_CapObjCreate(work->objList, 0xF0000, NULL, 1, 0, 0);
    mbObjDispSet(objWork->objId, FALSE);
    objWork->mode = 0;
    objWork->spinF = 1;
    objWork->time = 0;
    objWork->maxTime = 138;
    objWork->pos = playerPos;
    objWork->pos.y += 350.0f;
    objWork->vel.x = objWork->vel.y = objWork->vel.z = 0.0f;
    objWork->rot.x = objWork->rot.y = 0.0f;
    objWork->rot.z = 0.0f;
    objWork->target = playerPos;
    objWork->target.y += 150.0f;
    mbAudFXPlay(0x41F);
    mbPlayerMotionShiftSet(playerNo, motId[0], 0.0f, 8.0f, 0);
    i = mbPlayerObjIDGet(playerNo);
    i = mbObjModelIDGet(i);
    modelP = &Hu3DData[i];
    modelP->motShiftWork.speed = 3.0f;
    HuPrcSleep(120);
    mbPlayerMotionShiftSet(playerNo, motId[1], 0.0f, 8.0f, 0);
    angle = 360.0f*CAPTHROW_RANDF();
    for(i=0; i<3; i++) {
        dist = 100.0f*(1.0f+(1.5f*CAPTHROW_RANDF()));
        throwPos[i].x = playerPos.x+(dist*HuSin(angle));
        throwPos[i].y = playerPos.y+(100.0f*(2.5f+(2.0f*CAPTHROW_RANDF())));
        throwPos[i].z = playerPos.z+(dist*HuCos(angle));
        angle += 60.0f+(60.0f*CAPTHROW_RANDF());
    }
    throwPos[0] = playerPos;
    coinTotal = coinSum = coinNum = 0;
    for(i=0; i<3; i++) {
        if(i == 0) {
            HuPrcSleep(24);
            mbPlayerMotionShiftSet(playerNo, 9, 0.0f, 8.0f, 0);
            mbPlayerPosGet(playerNo, &startPos);
            mbPlayerColSnapPlayerSet(playerNo, FALSE);
        } else {
            obj = omAddObjEx(mbObjMan, 32768, 0, 0, -1, ev_CapTogezoOMExec);
            objWork = obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(TOGEZOWORK), HU_MEMNUM_OVL);
            memset(obj->data, 0, sizeof(TOGEZOWORK));
            objWork->objId = mbev_CapObjCreate(work->objList, 0xF0000, NULL, 1, 0, 0);
            mbObjDispSet(objWork->objId, FALSE);
            objWork->mode = 1;
            objWork->spinF = 0;
            objWork->time = 0;
            objWork->maxTime = 24;
            objWork->pos = throwPos[i];
            objWork->pos.y += 500.0f;
            objWork->vel.x = objWork->vel.y = objWork->vel.z = 0.0f;
            objWork->rot.x = 90.0f;
            objWork->rot.y = 0.0f;
            objWork->rot.z = 60.0f*(-0.5f+CAPTHROW_RANDF());
            objWork->target = throwPos[i];
            objWork->target.y += 150.0f;
            for(j=0; j<=24.0f; j++) {
                t = j/24.0f;
                rot.x = rot.z = 0.0f;
                if(i & 1) {
                    rot.y = 360.0f*t;
                } else {
                    rot.y = 360.0f*-t;
                }
                mbev_CapVecChase(t, &startPos, &throwPos[i], &pos);
                pos.y += 1.5*(100.0*HuSin(180.0f*t));
                mbPlayerPosSetV(playerNo, &pos);
                mbPlayerRotSetV(playerNo, &rot);
                HuPrcVSleep();
            }
            mbPlayerMotionSet(playerNo, 1, 0);
            mbPlayerMotionShiftSet(playerNo, 9, 0.0f, 8.0f, 0);
            mbPlayerPosGet(playerNo, &startPos);
        }
        mbAudFXPlay(0x420);
        if(i < 2) {
            coinNum = 3;
            coinSum += coinNum;
        } else {
            coinNum = 10-coinSum;
        }
        if(mbPlayerCoinGet(playerNo) < coinNum) {
            coinNum = mbPlayerCoinGet(playerNo);
        }
        mbCoinAddDispExec(playerNo, -coinNum, FALSE, TRUE);
        coinTotal += coinNum;
        angle = 360.0f*CAPTHROW_RANDF();
        for(j=0; j<coinNum; j++) {
            angle += 360.0f/coinNum;
            coinPos = startPos;
            coinPos.y += 100.0f;
            elev = 70.0f+(15.0f*CAPTHROW_RANDF());
            t = 65.0f*(0.8f+(0.3f*CAPTHROW_RANDF()));
            coinVel.x = t*(HuSin(angle)*HuCos(elev));
            coinVel.z = t*(HuCos(angle)*HuCos(elev));
            coinVel.y = t*HuSin(elev);
            mbev_CapEffCoinAdd(work->effCoin, &coinPos, &coinVel, 0.75f, 4.9f, 30, 0);
        }
        mbPlayerPosGet(playerNo, &pos);
        pos.y += 150.0f;
        rot.x = 45.0f*CAPTHROW_RANDF();
        rot.y = 360.0f*CAPTHROW_RANDF();
        rot.z = 0.0f;
        mbev_CapEffGlowCoinAdd(work->effGlow, &pos, &rot);
        scale.x = 0.5f;
        scale.y = 3.0f;
        scale.z = 100.0f*(1.0f+(0.25f*CAPTHROW_RANDF()));
        mbev_CapEffRingHitAdd(work->effRing, pos, rot, scale);
        omVibrate(playerNo, 20, 7, 3);
    }
    mbPlayerMotionShiftSet(playerNo, motId[2], 0.0f, 8.0f, 0x40000001);
    for(j=0; j<=36.0f; j++) {
        t = j/36.0f;
        rot.z = 0.0f;
        rot.x = 360.0f*t;
        rot.y = 360.0f*t;
        mbMasuPosGet(masuId, &playerPos);
        mbev_CapVecChase(t, &startPos, &playerPos, &pos);
        pos.y += 5.0*(100.0*HuSin(180.0f*t));
        mbPlayerPosSetV(playerNo, &pos);
        mbPlayerRotSetV(playerNo, &rot);
        HuPrcVSleep();
    }
    mbPlayerMotionShiftSet(playerNo, 6, 0.0f, 8.0f, 0x40000001);
    mbPlayerColSnapPlayerSet(playerNo, TRUE);
    if(coinTotal > 0) {
        mbev_CapCoinDisp(playerNo, -coinTotal, 0, 1);
    } else {
        HuPrcSleep(60);
    }
    if(coinTotal > 0) {
        mbWipeDissolveFadeOutTime(1);
        mbCameraPlayerViewSetFast(targetNo, 0);
        mbCameraMoveWait();
        mbWipeDissolveFadeIn();
        mbev_CapCoinAdd(work->effCoin, targetNo, coinTotal, 1);
    }
    mbev_CapPlayerMotShiftWait(targetNo, 1, 0x40000001, TRUE);
    mbev_CapPlayerMotShiftWait(playerNo, 1, 0x40000001, TRUE);
    HuPrcEnd();
}

void mbev_CapTogezoKill(void)
{
}

static void ev_CapTogezoOMExec(OMOBJ *obj)
{
    TOGEZOWORK *objWork = obj->data;
    float t;
    float angle;
    float elev;
    float speed;
    float baseAngle;
    HuVecF pos;

    if(mbExitCheck() || obj->work[3]) {
        omDelObjEx(mbObjMan, obj);
        return;
    }
    switch(objWork->mode) {
        case 0:
            t = ++objWork->time/(float)objWork->maxTime;
            objWork->rot.y = 720.0f*t;
            mbObjPosSetV(objWork->objId, &objWork->pos);
            mbObjRotSetV(objWork->objId, &objWork->rot);
            mbObjScaleSet(objWork->objId, t, t, t);
            mbObjDispSet(objWork->objId, TRUE);
            if(t >= 1.0f) {
                objWork->mode++;
                objWork->time = 0;
                objWork->maxTime = 12;
            }
            break;
        case 1:
            t = ++objWork->time/(float)objWork->maxTime;
            elev = t*t;
            objWork->rot.x = mbev_CapAngleSumLerp(t, objWork->rot.x, 90.0f);
            if(objWork->spinF == 0) {
                objWork->rot.y = 360.0f*t;
            }
            mbev_CapVecChase(elev, &objWork->pos, &objWork->target, &pos);
            mbObjPosSetV(objWork->objId, &pos);
            mbObjRotSetV(objWork->objId, &objWork->rot);
            mbObjDispSet(objWork->objId, TRUE);
            if(t >= 1.0f) {
                if(mbRandMod(32768) & 1) {
                    baseAngle = 90.0f;
                } else {
                    baseAngle = 270.0f;
                }
                angle = baseAngle;
                angle += 30.0f*(0.5f-CAPTHROW_RANDF());
                elev = 45.0f+(10.0f*CAPTHROW_RANDF());
                speed = 35.0f;
                objWork->vel.x = speed*(HuSin(angle)*HuCos(elev));
                objWork->vel.z = speed*(HuCos(angle)*HuCos(elev));
                objWork->vel.y = t*HuSin(elev);
                objWork->time = 0;
                objWork->maxTime = 30;
                mbObjPosGet(objWork->objId, &objWork->pos);
                objWork->pos.y += 100.0f;
                objWork->mode++;
                obj->work[1] = 0;
            }
            break;
        default:
            t = ++objWork->time/(float)objWork->maxTime;
            PSVECAdd(&objWork->pos, &objWork->vel, &objWork->pos);
            objWork->vel.y -= 1.633333357671897;
            objWork->rot.z += 2.5f*CAPTHROW_RANDF();
            mbObjPosSetV(objWork->objId, &objWork->pos);
            mbObjRotSetV(objWork->objId, &objWork->rot);
            if(t >= 1.0f) {
                mbObjDispSet(objWork->objId, FALSE);
                omDelObjEx(mbObjMan, obj);
            }
            break;
    }
}

static int kuriboCoinTbl[6] = { 3, 5, 10, 20, 30, 40 };
static s8 diceKuriboNumTbl[5] = { 0, 1, 2, 3, -1 };

void mbev_CapKuribo(void)
{
    CAPEVENTWORK *work = HuPrcCurrentGet()->property;
    int playerNo;
    int kuriboId;
    int targetNo;
    int shadowId;
    int targetObjId;
    int i;
    int coinVal;
    int interval;
    int counter;
    int coinAdd;
    int dispId1;
    int dispId2;
    float t;
    int motData[9];
    HuVecF playerPos;
    HuVecF objPos;
    HuVecF pos;
    HuVecF rot;
    HuVecF targetStartPos;
    HuVecF coinStart;
    HuVecF coinEnd;
    HuVecF dispPos;
    char coinStr[16];
    int coin[2];
    int unused;

    mbev_CapWait(work);
    work->coinMan = mbev_CapCoinManCreate();
    playerNo = work->playerNo;
    targetNo = work->targetNo;
    coin[0] = mbPlayerCoinGet(targetNo);
    coin[1] = mbPlayerCoinGet(playerNo);
    mbPlayerPosGet(targetNo, &targetStartPos);
    mbPlayerPosGet(playerNo, &playerPos);
    motData[0] = 0xF0002;
    motData[1] = 0xF0003;
    motData[2] = -1;
    kuriboId = mbev_CapObjCreate(work->objList, 0xF0001, motData, 0, 5, 0);
    mbObjMotionSet(kuriboId, 1, 0x40000001);
    mbObjDispSet(kuriboId, FALSE);
    shadowId = mbev_CapObjCreate(work->objList, 0xF001A, NULL, 1, 5, 0);
    mbObjDispSet(shadowId, FALSE);
    targetObjId = mbev_CapObjCreate(work->objList, 0xF001B, NULL, 1, 5, 0);
    mbObjDispSet(targetObjId, FALSE);
    mbObjDispSet(kuriboId, TRUE);
    mbObjDispSet(shadowId, TRUE);
    mbAudFXPlay(0x40C);
    for(i=0; i<=60.0f; i++) {
        t = i/60.0f;
        objPos.x = playerPos.x;
        objPos.y = (250.0f+playerPos.y)+(500.0*HuCos(90.0f*t));
        objPos.z = playerPos.z;
        mbObjPosSetV(kuriboId, &objPos);
        mbObjPosSetV(shadowId, &objPos);
        HuPrcVSleep();
    }
    mbObjMotionShiftSet(kuriboId, 2, 0.0f, 8.0f, 0x40000001);
    mbAudFXPlay(0x3D5);
    mbWinCreate(2, 0x360000, 2);
    mbWinTopInsertMesSet(mbPlayerNameMesGet(targetNo), 0);
    mbWinTopWait();
    mbObjMotionShiftSet(kuriboId, 1, 0.0f, 8.0f, 0x40000001);
    if(mbPlayerCoinGet(playerNo) > 0) {
        for(i=1; i<60.0f; i++) {
            t = i/60.0f;
            t = HuSin(90.0f*t);
            objPos.x = playerPos.x-(1.5f*(100.0f*t));
            objPos.y = 250.0f+playerPos.y;
            objPos.z = playerPos.z;
            mbObjPosSetV(kuriboId, &objPos);
            mbObjPosSetV(shadowId, &objPos);
            HuPrcVSleep();
        }
        switch(mbRandMod(32768)%10) {
            case 0:
            case 1:
                coinVal = 0;
                break;
            case 2:
            case 3:
            case 4:
                coinVal = 1;
                break;
            case 5:
            case 6:
            case 7:
                coinVal = 2;
                break;
            default:
                coinVal = 3;
                break;
        }
        coinVal = mbDiceExec(playerNo, 13, diceKuriboNumTbl, coinVal, TRUE, TRUE, NULL, 0);
        coinVal = kuriboCoinTbl[coinVal-1];
        HuPrcSleep(30);
        mbWipeDissolveFadeOutTime(1);
        mbDiceNumKill(playerNo);
        for(i=0; i<4; i++) {
            if(i != playerNo) {
                mbPlayerDispSet(i, FALSE);
            }
        }
        objPos.x = playerPos.x-100.0f;
        objPos.y = 250.0f+playerPos.y;
        objPos.z = playerPos.z;
        mbObjPosSetV(kuriboId, &objPos);
        mbObjPosSetV(shadowId, &objPos);
        mbWipeDissolveFadeIn();
        mbAudFXPlay(0x40C);
        mbPlayerDispSet(targetNo, TRUE);
        mbPlayerColSnapPlayerSet(targetNo, FALSE);
        mbObjDispSet(targetObjId, TRUE);
        for(i=0; i<=60.0f; i++) {
            t = i/60.0f;
            pos.x = 100.0f+playerPos.x;
            pos.y = (250.0f+playerPos.y)+(500.0*HuCos(90.0f*t));
            pos.z = playerPos.z;
            rot.x = rot.z = 0.0f;
            rot.y = 720.0*HuSin(90.0f*t);
            mbPlayerPosSetV(targetNo, &pos);
            mbPlayerRotSetV(targetNo, &rot);
            mbObjPosSetV(targetObjId, &pos);
            HuPrcVSleep();
        }
        mbObjMotionShiftSet(kuriboId, 2, 0.0f, 8.0f, 0x40000001);
        sprintf(coinStr, "%d", coinVal);
        mbWinCreate(2, 0x360001, 2);
        mbWinTopInsertMesSet((u32)coinStr, 0);
        mbWinTopInsertMesSet(mbPlayerNameMesGet(targetNo), 1);
        mbWinTopWait();
        mbObjMotionShiftSet(kuriboId, 1, 0.0f, 8.0f, 0x40000001);
        coin[0] = coinVal;
        if(coin[0] > mbPlayerCoinGet(playerNo)) {
            coin[0] = coinVal = mbPlayerCoinGet(playerNo);
        }
        if(coin[0] > 30) {
            interval = 0;
        } else if(coin[0] > 20) {
            interval = 1;
        } else if(coin[0] > 10) {
            interval = 2;
        } else {
            interval = 3;
        }
        unused = counter = 0;
        do {
            if(coin[0] > 0 && ++counter > interval) {
                mbPlayerPosGet(playerNo, &coinStart);
                mbPlayerPosGet(targetNo, &coinEnd);
                coinStart.y += 150.0f;
                coinEnd.y += 150.0f;
                coinAdd = mbev_CapCoinManAdd(work->coinMan, &coinStart, &coinEnd, targetNo, 1);
                if(coinAdd != 0) {
                    mbPlayerCoinAdd(playerNo, -coinAdd);
                    coin[0] -= coinAdd;
                    counter = 0;
                }
            }
            HuPrcVSleep();
        } while(coin[0] > 0 || mbev_CapCoinManNumGet(work->coinMan) > 0);
        mbAudFXPlay(0xF);
        mbPlayerWinLoseVoicePlay(targetNo, 12, 0x243);
        mbPlayerMotionShiftSet(targetNo, 12, 0.0f, 8.0f, 0);
        mbPlayerPosGet(targetNo, &dispPos);
        dispPos.y += 250.0f;
        dispId1 = mbCoinDispCapsuleCreate(&dispPos, coinVal);
        mbPlayerMotionShiftSet(playerNo, 13, 0.0f, 8.0f, 0);
        mbPlayerPosGet(playerNo, &dispPos);
        dispPos.y += 250.0f;
        dispId2 = mbCoinDispCapsuleCreate(&dispPos, -coinVal);
        while(!mbCoinDispKillCheck(dispId1) || !mbCoinDispKillCheck(dispId2)) {
            HuPrcVSleep();
        }
        mbWipeDissolveFadeOutTime(1);
        for(i=0; i<4; i++) {
            mbPlayerMotionShiftSet(i, 1, 0.0f, 8.0f, 0x40000001);
            mbPlayerDispSet(i, TRUE);
        }
        mbObjDispSet(targetObjId, FALSE);
        mbPlayerPosSetV(targetNo, &targetStartPos);
        mbPlayerColSnapPlayerSet(targetNo, TRUE);
        objPos.x = playerPos.x;
        objPos.y = 250.0f+playerPos.y;
        objPos.z = playerPos.z;
        mbObjPosSetV(kuriboId, &objPos);
        mbObjPosSetV(shadowId, &objPos);
        mbWipeDissolveFadeIn();
    } else {
        mbAudFXPlay(0x3D6);
        mbWinCreate(2, 0x360003, 2);
        mbWinTopWait();
    }
    mbObjMotionShiftSet(kuriboId, 2, 0.0f, 8.0f, 0x40000001);
    mbAudFXPlay(0x3D5);
    mbWinCreate(2, 0x360002, 2);
    mbWinTopWait();
    mbObjMotionShiftSet(kuriboId, 1, 0.0f, 8.0f, 0x40000001);
    mbAudFXPlay(0x40C);
    for(i=0; i<=60.0f; i++) {
        t = i/60.0f;
        objPos.y = (250.0f+playerPos.y)+(500.0*HuSin(90.0f*t));
        mbObjPosSetV(kuriboId, &objPos);
        mbObjPosSetV(shadowId, &objPos);
        HuPrcVSleep();
    }
    mbObjDispSet(kuriboId, FALSE);
    mbObjDispSet(shadowId, FALSE);
    HuPrcEnd();
}

void mbev_CapKuriboKill(void)
{
}

void mbev_CapPakkun(void)
{
    CAPEVENTWORK *work = HuPrcCurrentGet()->property;
    int i;
    int pakkunId;
    int j;
    int coinNum;
    int masuId;
    int seNo;
    float t;
    float angle;
    float dist;
    float speed;
    Mtx mtx;
    int motData[8];
    HuVecF playerPos;
    HuVecF pos;
    HuVecF dir;
    HuVecF vel;
    HuVecF startPos;
    HuVecF objPos;
    int motId[4];
    GXColor color;

    mbev_CapWait(work);
    work->effExplode = mbev_CapEffExplodeCreate();
    work->effCoin = mbev_CapEffCoinCreate();
    mbev_CapPlayerMoveObjInit();
    masuId = GwPlayer[work->playerNo].masuId;
    mbPlayerPosGet(work->playerNo, &playerPos);
    startPos = playerPos;
    motData[0] = 0xF0005;
    motData[1] = 0xF0006;
    motData[2] = 0xF0007;
    motData[3] = -1;
    pakkunId = mbev_CapObjCreate(work->objList, 0xF0004, motData, 0, 5, 0);
    mbev_CapObjPosSet(work->objList, pakkunId, GwPlayer[work->playerNo].masuId, 0);
    objPos = playerPos;
    mbObjMotionSet(pakkunId, 1, 0);
    mbObjDispSet(pakkunId, FALSE);
    motId[0] = mbev_CapPlayerMotionCreate(work->objList, work->playerNo, 0x930048);
    motId[1] = mbev_CapPlayerMotionCreate(work->objList, work->playerNo, 0x930044);
    motId[2] = mbev_CapPlayerMotionCreate(work->objList, work->playerNo, 0x930045);
    motId[3] = mbev_CapPlayerMotionCreate(work->objList, work->playerNo, 0x930017);
    pos.x = 0.0f;
    pos.y = 100.0f;
    pos.z = 0.0f;
    mbCameraMoveMasu(masuId, NULL, &pos, 2000.0f, -1.0f, 21);
    mbPlayerMotionShiftSet(work->playerNo, motId[3], 0.0f, 8.0f, 0);
    HuPrcSleep(60);
    mbev_CapPlayerMotShiftWait(work->playerNo, motId[0], 0, TRUE);
    for(i=0; i<3; i++) {
        for(j=0; j<32; j++) {
            dist = 100.0f*CAPTHROW_RANDF();
            angle = 360.0f*CAPTHROW_RANDF();
            speed = 0.05f*(100.0f*CAPTHROW_RANDF());
            pos.x = playerPos.x+(dist*HuSin(angle));
            pos.y = 50.0f+(playerPos.y+(0.5f*(100.0f*CAPTHROW_RANDF())));
            pos.z = playerPos.z+(dist*HuCos(angle));
            vel.x = speed*HuSin(angle);
            vel.y = 0.0f;
            vel.z = speed*HuCos(angle);
            t = CAPTHROW_RANDF();
            color.r = 192.0f+(63.0f*t);
            color.g = 192.0f+(63.0f*t);
            color.b = 192.0f+(63.0f*t);
            color.a = 127.0f+(63.0f*CAPTHROW_RANDF());
            mbev_CapEffExplodeAdd(work->effExplode, pos, vel, 100.0f*(1.0f+(0.5f*CAPTHROW_RANDF())), -0.5f+CAPTHROW_RANDF(), 0.33f+(0.2f*CAPTHROW_RANDF()), color);
        }
        HuPrcVSleep();
    }
    mbAudFXPlay(0x42D);
    mbObjDispSet(pakkunId, TRUE);
    mbObjMotionSet(pakkunId, 1, 0);
    mbObjMotionTimeSet(pakkunId, 0.0f);
    mbObjMotionSpeedSet(pakkunId, 0.5f);
    mbObjScaleSet(pakkunId, 0.0f, 0.0f, 0.0f);
    for(i=0; i<4; i++) {
        if(i != work->playerNo && masuId == GwPlayer[i].masuId) {
            mbev_CapPlayerMoveHitCreate(i, 1, 0);
        }
    }
    mbPlayerMotionShiftSet(work->playerNo, motId[1], 0.0f, 5.0f, 0);
    mbPlayerColSnapPlayerSet(work->playerNo, FALSE);
    for(i=0; i<10; i++) {
        t = i/9.0f;
        if(t > 1.0f) {
            t = 1.0f;
        }
        mbObjScaleSet(pakkunId, t, t, t);
        Hu3DMotionCalc(mbObjModelIDGet(pakkunId));
        Hu3DModelObjMtxGet(mbObjModelIDGet(pakkunId), "itemhook_C", mtx);
        pos.x = mtx[0][3];
        pos.y = mtx[1][3];
        pos.z = mtx[2][3];
        if(pos.y > playerPos.y) {
            pos.y = playerPos.y;
        }
        PSVECSubtract(&pos, &playerPos, &dir);
        PSVECScale(&dir, &dir, i/9.0f);
        PSVECAdd(&playerPos, &dir, &pos);
        mbPlayerPosSetV(work->playerNo, &pos);
        for(j=0; j<2; j++) {
            dist = 100.0f*CAPTHROW_RANDF();
            angle = 360.0f*CAPTHROW_RANDF();
            speed = 0.05f*(100.0f*CAPTHROW_RANDF());
            pos.x = playerPos.x+(dist*HuSin(angle));
            pos.y = playerPos.y+(0.5f*(100.0f*CAPTHROW_RANDF()));
            pos.z = playerPos.z+(dist*HuCos(angle));
            vel.x = speed*HuSin(angle);
            vel.y = 0.0f;
            vel.z = speed*HuCos(angle);
            t = CAPTHROW_RANDF();
            color.r = 192.0f+(63.0f*t);
            color.g = 192.0f+(63.0f*t);
            color.b = 192.0f+(63.0f*t);
            color.a = 192.0f+(63.0f*CAPTHROW_RANDF());
            mbev_CapEffExplodeAdd(work->effExplode, pos, vel, 100.0f*(2.0f+CAPTHROW_RANDF()), 2.0f*(-0.5f+CAPTHROW_RANDF()), 0.33f+(0.2f*CAPTHROW_RANDF()), color);
        }
        HuPrcVSleep();
    }
    mbObjDispSet(mbPlayerObjIDGet(work->playerNo), FALSE);
    omVibrate(work->playerNo, 20, 7, 3);
    do {
        HuPrcVSleep();
    } while(!mbObjMotionEndCheck(pakkunId) || mbObjMotionShiftIDGet(pakkunId) != -1);
    coinNum = (mbPlayerCoinGet(work->playerNo)+1)/2;
    seNo = mbAudFXPlay(0x42E);
    if(coinNum >= 1) {
        mbObjMotionShiftSet(pakkunId, 2, 0.0f, 8.0f, 0x40000001);
        HuPrcSleep(120);
    }
    if(seNo != -1) {
        mbAudFXStop(seNo);
    }
    mbAudFXPlay(0x42F);
    mbObjMotionShiftSet(pakkunId, 3, 0.0f, 8.0f, 0);
    HuPrcSleep(38);
    Hu3DMotionCalc(mbObjModelIDGet(pakkunId));
    Hu3DModelObjMtxGet(mbObjModelIDGet(pakkunId), "itemhook_C", mtx);
    mbPlayerMotionSet(work->playerNo, motId[2], 0);
    mbObjDispSet(mbPlayerObjIDGet(work->playerNo), TRUE);
    pos.x = startPos.x;
    pos.y = mtx[1][3];
    pos.z = startPos.z;
    mbPlayerPosSetV(work->playerNo, &pos);
    mbev_CapPlayerMoveEjectCreate(work->playerNo, 1);
    mbev_CapPlayerMoveMinYSet(work->playerNo, startPos.y);
    pos.x = 0.0f;
    pos.y = 35.0f;
    pos.z = 0.0f;
    mbev_CapPlayerMoveVelSet(work->playerNo, 3.266667f, pos);
    mbCoinAddDispExec(work->playerNo, -coinNum, FALSE, TRUE);
    do {
        HuPrcVSleep();
    } while(!mbObjMotionEndCheck(pakkunId) || mbObjMotionShiftIDGet(pakkunId) != -1);
    for(i=0; i<15.0f; i++) {
        t = i/15.0f;
        mbObjScaleSet(pakkunId, 1.0f-t, 1.0f-t, 1.0f-t);
        HuPrcVSleep();
    }
    mbObjDispSet(pakkunId, FALSE);
    do {
        for(i=0; i<4; i++) {
            if(!mbev_CapPlayerMoveObjCheck(i)) {
                break;
            }
        }
        HuPrcVSleep();
    } while(i < 4);
    if(coinNum != 0) {
        mbev_CapCoinDisp(work->playerNo, -coinNum, 0, 1);
    }
    if(coinNum != 0) {
        mbWinCreate(2, 0x360004, -1);
    } else {
        mbWinCreate(2, 0x360005, -1);
    }
    mbWinTopWait();
    if(coinNum != 0) {
        mbWipeDissolveFadeOutTime(1);
        pos.x = 0.0f;
        pos.y = 100.0f;
        pos.z = 0.0f;
        mbCameraMoveMasu(GwPlayer[work->targetNo].masuId, NULL, &pos, 2000.0f, -1.0f, -1);
        mbCameraMoveWait();
        mbWipeDissolveFadeIn();
        mbev_CapCoinAdd(work->effCoin, work->targetNo, coinNum, 1);
    }
    mbev_CapPlayerIdleWait();
    HuPrcEnd();
}

void mbev_CapPakkunKill(void)
{
}

static HuVecF jangoPlayerOfs = { 0.0f, -120.0f, 0.0f };

void mbev_CapJango(void)
{
    CAPEVENTWORK *work = HuPrcCurrentGet()->property;
    int i;
    int jangoId;
    int destMasuId;
    int masuId;
    int seNo;
    float t;
    float s;
    float angle;
    float radius;
    Mtx mtx;
    int motData[16];
    HuVecF p0;
    HuVecF rot;
    HuVecF p2;
    HuVecF p1;
    HuVecF hookPos;
    HuVecF masuPos;
    HuVecF playerRot;
    HuVecF objRot;
    HuVecF pos;

    mbev_CapWait(work);
    masuId = GwPlayer[work->playerNo].masuId;
    for(i=1; i<mbMasuNumGet(); i++) {
        if(mbMasuAttrGet(i) & 0x8000) {
            break;
        }
    }
    if(i < mbMasuNumGet()) {
        destMasuId = i;
    } else {
        destMasuId = masuId;
    }
    motData[0] = 0xF0009;
    motData[1] = 0xF000A;
    motData[2] = -1;
    jangoId = mbev_CapObjCreate(work->objList, 0xF0008, motData, 0, 5, 0);
    mbObjMotionSet(jangoId, 1, 0x40000001);
    mbObjScaleSet(jangoId, 0.5f, 0.5f, 0.5f);
    mbObjDispSet(jangoId, FALSE);
    pos.x = pos.z = 0.0f;
    pos.y = 100.0f;
    mbCameraMoveMasu(masuId, NULL, &pos, -1.0f, -1.0f, 30);
    motData[0] = mbev_CapPlayerMotionCreate(work->objList, work->playerNo, 0x93003F);
    motData[1] = mbev_CapPlayerMotionCreate(work->objList, work->playerNo, 0x930017);
    mbPlayerMotionShiftSet(work->playerNo, motData[1], 0.0f, 8.0f, 0x40000001);
    HuPrcSleep(60);
    mbObjDispSet(jangoId, TRUE);
    mbAudFXPlay(0x415);
    seNo = mbAudFXPlay(0x416);
    for(i=1; i<=120.0f; i++) {
        t = i/120.0f;
        radius = 4.0*(100.0*HuCos(90.0f*t));
        angle = 990.0f*t;
        mbPlayerPosGet(work->playerNo, &p0);
        pos.x = p0.x+(radius*HuSin(angle));
        pos.y = (150.0f+p0.y)+(8.0*(100.0*HuCos(90.0f*t)));
        pos.z = p0.z+(radius*HuCos(angle));
        rot.x = 0.0f;
        rot.y = 90.0f+angle;
        rot.z = 0.0f;
        mbObjPosSetV(jangoId, &pos);
        mbObjRotSetV(jangoId, &rot);
        HuPrcVSleep();
    }
    mbPlayerMotionShiftSet(work->playerNo, motData[0], 0.0f, 8.0f, 0x40000001);
    mbPlayerColSnapPlayerSet(work->playerNo, FALSE);
    mbObjMotionShiftSet(jangoId, 2, 0.0f, 8.0f, 0x40000001);
    omVibrate(work->playerNo, 20, 7, 3);
    for(i=0; i<18.0f; i++) {
        t = i/18.0f;
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
        mbObjRotSetV(jangoId, &rot);
        Hu3DModelObjMtxGet(mbObjModelIDGet(jangoId), "itemhook_oya", mtx);
        p2.x = mtx[0][3]+jangoPlayerOfs.x;
        p2.y = mtx[1][3]+jangoPlayerOfs.y;
        p2.z = mtx[2][3]+jangoPlayerOfs.z;
        mbPlayerPosGet(work->playerNo, &p0);
        mbev_CapVecChase(t, &p0, &p2, &pos);
        mbPlayerPosSetV(work->playerNo, &pos);
        HuPrcVSleep();
    }
    mbObjPosGet(jangoId, &p0);
    p1.x = p0.x;
    p1.y = p0.y;
    p1.z = 500.0f+p0.z;
    p2.x = p0.x;
    p2.y = 1000.0f+p0.y;
    p2.z = 1000.0f+p0.z;
    for(i=1; i<=72.0f; i++) {
        t = i/72.0f;
        mbev_CapBezierGetV(t, &p0, &p1, &p2, &pos);
        mbev_CapBezierNormGetV(t, &p0, &p1, &p2, &rot);
        mbObjPosSetV(jangoId, &pos);
        mbObjRotSetV(jangoId, &rot);
        Hu3DMotionCalc(mbObjModelIDGet(jangoId));
        Hu3DModelObjMtxGet(mbObjModelIDGet(jangoId), "itemhook_oya", mtx);
        pos.x = mtx[0][3]+jangoPlayerOfs.x;
        pos.y = mtx[1][3]+jangoPlayerOfs.y;
        pos.z = mtx[2][3]+jangoPlayerOfs.z;
        mbPlayerPosSetV(work->playerNo, &pos);
        mbPlayerRotSetV(work->playerNo, &rot);
        HuPrcVSleep();
    }
    mbWipeDissolveFadeOutTime(1);
    mbMasuPosGet(destMasuId, &p2);
    p2.y += 2000.0f;
    mbObjPosSetV(jangoId, &p2);
    mbPlayerPosSetV(work->playerNo, &p2);
    mbev_PlayerColMasuSet(work->playerNo, destMasuId, TRUE);
    pos.x = pos.z = 0.0f;
    pos.y = 100.0f;
    mbCameraMoveMasu(destMasuId, NULL, &pos, -1.0f, -1.0f, -1);
    mbCameraMoveWait();
    mbWipeDissolveFadeIn();
    mbMasuPosGet(destMasuId, &p2);
    p2.y += 150.0f;
    p1.x = 200.0f+p2.x;
    p1.y = 300.0f+p2.y;
    p1.z = 200.0f+p2.z;
    p0.x = p2.x-200.0f;
    p0.y = 800.0f+p2.y;
    p0.z = 600.0f+p2.z;
    for(i=1; i<=60.0f; i++) {
        t = i/60.0f;
        mbev_CapBezierGetV(t, &p0, &p1, &p2, &pos);
        mbev_CapBezierNormGetV(t, &p0, &p1, &p2, &rot);
        mbev_CapVecRotGet(&rot, &rot);
        mbObjPosSetV(jangoId, &pos);
        mbObjRotSetV(jangoId, &rot);
        Hu3DMotionCalc(mbObjModelIDGet(jangoId));
        Hu3DModelObjMtxGet(mbObjModelIDGet(jangoId), "itemhook_oya", mtx);
        pos.x = mtx[0][3]+jangoPlayerOfs.x;
        pos.y = mtx[1][3]+jangoPlayerOfs.y;
        pos.z = mtx[2][3]+jangoPlayerOfs.z;
        mbPlayerPosSetV(work->playerNo, &pos);
        mbPlayerRotSetV(work->playerNo, &rot);
        if(i == 52) {
            mbPlayerMotionShiftSet(work->playerNo, 6, 0.0f, 8.0f, 0x40000001);
        }
        HuPrcVSleep();
    }
    Hu3DMotionCalc(mbObjModelIDGet(jangoId));
    Hu3DModelObjMtxGet(mbObjModelIDGet(jangoId), "itemhook_oya", mtx);
    hookPos.x = mtx[0][3]+jangoPlayerOfs.x;
    hookPos.y = mtx[1][3]+jangoPlayerOfs.y;
    hookPos.z = mtx[2][3]+jangoPlayerOfs.z;
    mbMasuPosGet(destMasuId, &masuPos);
    mbPlayerRotGet(work->playerNo, &playerRot);
    mbObjRotGet(jangoId, &objRot);
    mbObjMotionShiftSet(jangoId, 1, 0.0f, 8.0f, 0x40000001);
    mbMasuPosGet(destMasuId, &p0);
    p0.y += 150.0f;
    p1.x = p0.x-200.0f;
    p1.y = p0.y;
    p1.z = 200.0f+p0.z;
    p2.x = 200.0f+p0.x;
    p2.y = 800.0f+p0.y;
    p2.z = 800.0f+p0.z;
    for(i=1; i<=60.0f; i++) {
        t = i/60.0f;
        mbev_CapBezierGetV(t, &p0, &p1, &p2, &pos);
        mbev_CapBezierNormGetV(t, &p0, &p1, &p2, &rot);
        mbev_CapVecRotGet(&rot, &rot);
        s = 2.0f*t;
        if(s > 1.0f) {
            s = 1.0f;
        }
        rot.x = mbev_CapAngleSumLerp(s, objRot.x, rot.x);
        rot.y = mbev_CapAngleSumLerp(s, objRot.y, rot.y);
        rot.z = mbev_CapAngleSumLerp(s, objRot.z, rot.z);
        mbObjPosSetV(jangoId, &pos);
        mbObjRotSetV(jangoId, &rot);
        s = 5.0f*t;
        if(s > 1.0f) {
            s = 1.0f;
        }
        mbMasuPosGet(destMasuId, &masuPos);
        mbev_CapVecChase(s, &hookPos, &masuPos, &pos);
        rot.x = mbev_CapAngleSumLerp(s, playerRot.x, 0.0f);
        rot.y = mbev_CapAngleSumLerp(s, playerRot.y, 0.0f);
        rot.z = mbev_CapAngleSumLerp(s, playerRot.z, 0.0f);
        mbPlayerPosSetV(work->playerNo, &pos);
        mbPlayerRotSetV(work->playerNo, &rot);
        HuPrcVSleep();
    }
    mbObjDispSet(jangoId, FALSE);
    if(seNo != -1) {
        mbAudFXStop(seNo);
    }
    mbMasuPosGet(destMasuId, &p2);
    mbPlayerPosSetV(work->playerNo, &p2);
    mbPlayerRotSet(work->playerNo, 0.0f, 0.0f, 0.0f);
    GwPlayer[work->playerNo].masuId = destMasuId;
    mbPlayerColSnapPlayerSet(work->playerNo, TRUE);
    HuPrcSleep(60);
    mbPlayerMotionShiftSet(work->playerNo, 1, 0.0f, 8.0f, 0x40000001);
    HuPrcEnd();
}

/* @@PART2@@ */
#include "game/board/capsule.h"
#include "game/board/capselect.h"

/* Part 2 additions to HEAD (capevent.c prototypes as retail calls them here; data). */
int mbev_CapCullCheck(int playerNo, int masuId);
void mbev_CapPlayerMotShiftSet(int objId, int motNo, u32 attr, BOOL waitF);

static void KokamekkuObjUpdate(float t, float angle, int *objId, int num, int curNo, int skipNo, HuVecF *center, HuVecF *playerPos);

void mbev_CapJangoKill(void)
{
}

static HuVecF patapataPlayerOfs[14] = {
    { 0.0f, -140.0f, 0.0f },
    { 0.0f, -160.0f, 0.0f },
    { 0.0f, -140.0f, 0.0f },
    { 0.0f, -150.0f, 0.0f },
    { 0.0f, -140.0f, 0.0f },
    { 0.0f, -140.0f, 0.0f },
    { 0.0f, -160.0f, 0.0f },
    { 0.0f, -140.0f, 0.0f },
    { 0.0f, -140.0f, 0.0f },
    { 0.0f, -150.0f, 0.0f },
    { 0.0f, -140.0f, 0.0f },
    { 0.0f, -150.0f, 0.0f },
    { 0.0f, -150.0f, 0.0f },
    { 0.0f, -150.0f, 0.0f },
};

void mbev_CapPatapata(void)
{
    Mtx hookMtx;
    int motData[16];
    HuVecF playerPos[2];
    HuVecF objPos[2];
    HuVecF ofs[2];
    HuVecF pos;
    HuVecF playerPosStart;
    int motId[2][2];
    int objId[2];
    int playerNo[2];
    int masuId[2];
    int masuIdSwap[2];
    int seNo[2];
    CAPEVENTWORK *work;
    float t;
    float weight;
    int num;
    int j;
    int i;

    work = HuPrcCurrentGet()->property;
    mbMoveNumDispSet(work->playerNo, FALSE);
    if(GwPlayer[work->playerNo].masuId != GwPlayer[work->targetNo].masuId) {
        while(GwPlayer[work->targetNo].moveF) {
            HuPrcVSleep();
        }
        mbev_PlayerColMasu(work->targetNo, GwPlayer[work->targetNo].masuId, FALSE);
        while(GwPlayer[work->targetNo].moveF) {
            HuPrcVSleep();
        }
    }
    mbev_CapWait(work);
    playerNo[0] = work->playerNo;
    playerNo[1] = work->targetNo;
    masuId[0] = GwPlayer[playerNo[0]].masuId;
    masuId[1] = GwPlayer[playerNo[1]].masuId;
    masuIdSwap[0] = masuId[1];
    masuIdSwap[1] = masuId[0];
    mbPlayerPosGet(playerNo[0], &playerPos[0]);
    mbMasuPosGet(masuId[0], &pos);
    PSVECSubtract(&playerPos[0], &pos, &ofs[0]);
    ofs[0].y = 0.0f;
    mbPlayerPosGet(playerNo[1], &playerPos[1]);
    mbMasuPosGet(masuId[1], &pos);
    PSVECSubtract(&playerPos[1], &pos, &ofs[1]);
    ofs[1].y = 0.0f;
    pos.x = pos.z = 0.0f;
    pos.y = 100.0f;
    mbCameraMoveMasu(masuId[0], NULL, &pos, -1.0f, -1.0f, 30);
    for(i=0; i<2; i++) {
        motData[0] = 0xF000C;
        motData[1] = 0xF000D;
        motData[2] = 0xF000E;
        motData[3] = -1;
        objId[i] = mbev_CapObjCreate(work->objList, 0xF000B, motData, 1, 5, 0);
        mbObjMotionSet(objId[i], 1, 0x40000001);
        mbObjMotionTimeSet(objId[i], mbObjMotionMaxTimeGet(objId[i])*CAPTHROW_RANDF());
        mbObjDispSet(objId[i], FALSE);
    }
    for(i=0; i<2; i++) {
        motId[i][0] = mbev_CapPlayerMotionCreate(work->objList, playerNo[i], 0x93003F);
    }
    if(GwPlayer[playerNo[0]].metalF || masuId[0] == masuId[1]) {
        num = 1;
    } else {
        num = 2;
    }
    for(i=0; i<num; i++) {
        seNo[i] = mbAudFXPlay(0x421);
        mbObjDispSet(objId[i], TRUE);
    }
    if(mbev_CapCullCheck(playerNo[1], masuId[1]) == FALSE) {
        mbPlayerDispSet(playerNo[1], FALSE);
        mbObjDispSet(objId[1], FALSE);
    }
    for(j=0; j<90.0f; j++) {
        for(i=0; i<num; i++) {
            t = j/90.0f;
            mbPlayerPosGet(playerNo[i], &playerPos[i]);
            objPos[i] = playerPos[i];
            objPos[i].y += 150.0+(6.0*(100.0*HuCos(90.0f*t)));
            objPos[i].z -= 100.0f;
            mbObjPosSetV(objId[i], &objPos[i]);
        }
        HuPrcVSleep();
    }
    mbWinCreate(2, 0x360006, 3);
    mbWinTopInsertMesSet(mbPlayerNameMesGet(playerNo[1]), 0);
    mbWinTopWait();
    if(GwPlayer[playerNo[0]].masuId == GwPlayer[playerNo[1]].masuId) {
        mbWinCreate(2, 0x360008, 3);
        mbWinTopInsertMesSet(mbPlayerNameMesGet(playerNo[1]), 0);
        mbWinTopWait();
        goto end;
    }
    for(i=0; i<num; i++) {
        mbObjMotionShiftSet(objId[i], 2, 0.0f, 8.0f, 0x40000001);
        mbObjPosGet(objId[i], &objPos[i]);
        mbPlayerPosGet(playerNo[i], &playerPos[i]);
    }
    omVibrate(playerNo[0], 20, 7, 3);
    omVibrate(playerNo[1], 20, 7, 3);
    for(j=0; j<30.0f; j++) {
        for(i=0; i<num; i++) {
            t = j/30.0f;
            pos = objPos[i];
            pos.y += 100.0*-HuSin(90.0f*t);
            mbObjPosSetV(objId[i], &pos);
            Hu3DMotionCalc(mbObjModelIDGet(objId[i]));
            Hu3DModelObjMtxGet(mbObjModelIDGet(objId[i]), "itemhook_C", hookMtx);
            pos.x = hookMtx[0][3]+patapataPlayerOfs[GwPlayer[playerNo[i]].charNo].x;
            pos.y = hookMtx[1][3]+patapataPlayerOfs[GwPlayer[playerNo[i]].charNo].y;
            pos.z = hookMtx[2][3]+patapataPlayerOfs[GwPlayer[playerNo[i]].charNo].z;
            if(t < 0.5f) {
                weight = 0.0f;
            } else {
                weight = 2.0f*(t-0.5f);
            }
            if(j == 15) {
                mbPlayerMotionShiftSet(playerNo[i], motId[i][0], 0.0f, 8.0f, 0x40000001);
                mbPlayerColSnapPlayerSet(playerNo[i], FALSE);
            }
            mbev_CapVecChase(weight, &playerPos[i], &pos, &pos);
            mbPlayerPosSetV(playerNo[i], &pos);
        }
        HuPrcVSleep();
    }
    for(i=0; i<num; i++) {
        mbObjPosGet(objId[i], &objPos[i]);
    }
    if(GwPlayer[playerNo[0]].metalF) {
        work->effExplode = mbev_CapEffExplodeCreate();
        i = 0;
        for(j=0; j<90.0f; j++) {
            t = j/90.0f;
            pos = objPos[i];
            pos.y += 2.0*(100.0*HuSin(90.0f*(t*t)));
            mbObjPosSetV(objId[i], &pos);
            Hu3DMotionCalc(mbObjModelIDGet(objId[i]));
            Hu3DModelObjMtxGet(mbObjModelIDGet(objId[i]), "itemhook_C", hookMtx);
            pos.x = hookMtx[0][3]+patapataPlayerOfs[GwPlayer[playerNo[i]].charNo].x;
            pos.y = hookMtx[1][3]+patapataPlayerOfs[GwPlayer[playerNo[i]].charNo].y;
            pos.z = hookMtx[2][3]+patapataPlayerOfs[GwPlayer[playerNo[i]].charNo].z;
            mbPlayerPosSetV(playerNo[i], &pos);
            HuPrcVSleep();
        }
        i = 0;
        for(j=0; j<60.0f; j++) {
            t = j/60.0f;
            pos = objPos[i];
            pos.y += 200.0+(0.1f*(100.0*HuSin(720.0f*t)));
            mbObjPosSetV(objId[i], &pos);
            Hu3DMotionCalc(mbObjModelIDGet(objId[i]));
            Hu3DModelObjMtxGet(mbObjModelIDGet(objId[i]), "itemhook_C", hookMtx);
            pos.x = hookMtx[0][3]+patapataPlayerOfs[GwPlayer[playerNo[i]].charNo].x;
            pos.y = hookMtx[1][3]+patapataPlayerOfs[GwPlayer[playerNo[i]].charNo].y;
            pos.z = hookMtx[2][3]+patapataPlayerOfs[GwPlayer[playerNo[i]].charNo].z;
            mbPlayerPosSetV(playerNo[i], &pos);
            HuPrcVSleep();
        }
        mbObjPosGet(objId[i], &objPos[i]);
        mbPlayerPosGet(playerNo[i], &playerPosStart);
        mbObjMotionShiftSet(objId[i], 1, 0.0f, 8.0f, 0x40000001);
        mbPlayerMotionShiftSet(playerNo[i], 4, 0.0f, 8.0f, 0);
        i = 0;
        for(j=1; j<=30.0f; j++) {
            t = j/30.0f;
            t = HuCos(90.0f*t);
            mbMasuPosGet(masuId[i], &pos);
            mbev_CapVecChase(t, &pos, &playerPosStart, &pos);
            mbPlayerPosSetV(playerNo[i], &pos);
            if(i == 10) {
                mbPlayerMotionShiftSet(playerNo[i], 5, 0.0f, 8.0f, 0);
            }
            HuPrcVSleep();
        }
        mbPlayerColSnapPlayerSet(playerNo[i], TRUE);
        mbMasuPosGet(masuId[i], &pos);
        mbev_CapEffDustHeavyAdd(work->effExplode, pos);
        mbPlayerMotionShiftSet(playerNo[i], 1, 0.0f, 8.0f, 0x40000001);
        mbWinCreate(2, 0x360009, 3);
        mbWinTopWait();
        i = 0;
        for(j=0; j<60.0f; j++) {
            t = j/60.0f;
            pos = objPos[i];
            pos.y += 3.0*(100.0*HuSin(90.0f*(t*t)));
            mbObjPosSetV(objId[i], &pos);
            HuPrcVSleep();
        }
        mbObjDispSet(objId[i], FALSE);
        goto dispOn;
    }
    for(j=0; j<90.0f; j++) {
        for(i=0; i<num; i++) {
            t = j/90.0f;
            pos = objPos[i];
            pos.y += 6.0*(100.0*HuSin(90.0f*(t*t)));
            mbObjPosSetV(objId[i], &pos);
            Hu3DMotionCalc(mbObjModelIDGet(objId[i]));
            Hu3DModelObjMtxGet(mbObjModelIDGet(objId[i]), "itemhook_C", hookMtx);
            pos.x = hookMtx[0][3]+patapataPlayerOfs[GwPlayer[playerNo[i]].charNo].x;
            pos.y = hookMtx[1][3]+patapataPlayerOfs[GwPlayer[playerNo[i]].charNo].y;
            pos.z = hookMtx[2][3]+patapataPlayerOfs[GwPlayer[playerNo[i]].charNo].z;
            mbPlayerPosSetV(playerNo[i], &pos);
        }
        HuPrcVSleep();
    }
    mbWipeDissolveFadeOutTime(1);
    for(i=0; i<2; i++) {
        mbPlayerDispSet(playerNo[i], FALSE);
        mbObjDispSet(objId[i], FALSE);
    }
    pos.x = pos.z = 0.0f;
    pos.y = 100.0f;
    mbCameraMoveMasu(masuIdSwap[0], NULL, &pos, -1.0f, -1.0f, -1);
    mbCameraMoveWait();
    mbWipeDissolveFadeIn();
    for(i=0; i<2; i++) {
        mbPlayerDispSet(playerNo[i], TRUE);
        mbObjDispSet(objId[i], TRUE);
    }
    if(mbev_CapCullCheck(playerNo[1], masuIdSwap[1]) == FALSE) {
        mbPlayerDispSet(playerNo[1], FALSE);
        mbObjDispSet(objId[1], FALSE);
    }
    for(j=0; j<90.0f; j++) {
        for(i=0; i<2; i++) {
            t = j/90.0f;
            mbMasuPosGet(masuIdSwap[i], &objPos[i]);
            PSVECAdd(&objPos[i], &ofs[i^1], &objPos[i]);
            objPos[i].y += 150.0f;
            objPos[i].z -= 100.0f;
            pos = objPos[i];
            pos.y += 6.0*(100.0*HuCos(90.0f*t));
            mbObjPosSetV(objId[i], &pos);
            Hu3DMotionCalc(mbObjModelIDGet(objId[i]));
            Hu3DModelObjMtxGet(mbObjModelIDGet(objId[i]), "itemhook_C", hookMtx);
            pos.x = hookMtx[0][3]+patapataPlayerOfs[GwPlayer[playerNo[i]].charNo].x;
            pos.y = hookMtx[1][3]+patapataPlayerOfs[GwPlayer[playerNo[i]].charNo].y;
            pos.z = hookMtx[2][3]+patapataPlayerOfs[GwPlayer[playerNo[i]].charNo].z;
            mbPlayerPosSetV(playerNo[i], &pos);
        }
        HuPrcVSleep();
    }
    for(i=0; i<2; i++) {
        mbObjMotionShiftSet(objId[i], 1, 0.0f, 8.0f, 0x40000001);
        mbPlayerMotionShiftSet(playerNo[i], 1, 0.0f, 8.0f, 0x40000001);
        mbObjPosGet(objId[i], &objPos[i]);
        mbPlayerPosGet(playerNo[i], &playerPos[i]);
    }
    for(j=0; j<18.0f; j++) {
        for(i=0; i<2; i++) {
            t = j/18.0f;
            pos = objPos[i];
            pos.y += 0.5*(100.0*-HuSin(180.0f*t));
            mbObjPosSetV(objId[i], &pos);
            mbPlayerPosGet(playerNo[i], &playerPos[i]);
            mbMasuPosGet(masuIdSwap[i], &pos);
            PSVECAdd(&pos, &ofs[i^1], &pos);
            mbev_CapVecChase(t, &playerPos[i], &pos, &pos);
            mbPlayerPosSetV(playerNo[i], &pos);
        }
        HuPrcVSleep();
    }
    for(i=0; i<2; i++) {
        mbMasuPosGet(masuIdSwap[i], &playerPos[i]);
        PSVECAdd(&playerPos[i], &ofs[i^1], &playerPos[i]);
        mbPlayerPosSetV(playerNo[i], &playerPos[i]);
        GwPlayer[playerNo[i]].masuId = masuIdSwap[i];
        mbPlayerColSnapPlayerSet(playerNo[i], TRUE);
        GwPlayer[playerNo[i]].masuIdNext = masuIdSwap[i];
        mbPlayerDispSet(playerNo[i], TRUE);
    }
end:
    for(i=0; i<2; i++) {
        mbPlayerDispSet(playerNo[i], TRUE);
    }
    mbWinCreate(2, 0x360007, 3);
    mbWinTopWait();
    for(j=0; j<90.0f; j++) {
        for(i=0; i<2; i++) {
            t = j/90.0f;
            pos = objPos[i];
            pos.y += 6.0*(100.0*HuSin(90.0f*t));
            mbObjPosSetV(objId[i], &pos);
        }
        HuPrcVSleep();
    }
dispOn:
    for(i=0; i<2; i++) {
        mbPlayerDispSet(playerNo[i], TRUE);
    }
    for(i=0; i<num; i++) {
        if(seNo[i] != -1) {
            mbAudFXStop(seNo[i]);
        }
    }
    if(GwPlayer[work->playerNo].moveNum > 1) {
        mbMoveNumDispSet(work->playerNo, TRUE);
    }
    HuPrcEnd();
}

void mbev_CapPatapataKill(void)
{
}

static GXColor kokamekkuColorTbl[2] = {
    { 255, 127, 127, 255 },
    { 127, 127, 255, 255 },
};

void mbev_CapKokamekku(void)
{
    int motData[16];
    s8 dirTbl[20];
    int capObj[5];
    HuVecF playerPos;
    HuVecF kokaPos;
    HuVecF vel;
    HuVecF pos;
    HuVecF center;
    HuVecF capPos;
    CAPEVENTWORK *work;
    float t;
    float s;
    float angleNext;
    float scale;
    float radius;
    float dist;
    float angleStep;
    float angle;
    float rotY;
    float glowY1;
    float glowY0;
    int targetNo;
    int capIdx;
    int capNo;
    int dirNum;
    int seNo;
    int playerNo;
    int capNum;
    int kokaObj;
    int curNo;
    int j;
    int no;
    int i;
    int nextNo;

    work = HuPrcCurrentGet()->property;
    mbev_CapWait(work);
    work->effGlow = mbev_CapEffGlowCreate();
    playerNo = work->playerNo;
    targetNo = work->targetNo;
    mbPlayerPosGet(playerNo, &playerPos);
    motData[0] = 0xF0010;
    motData[1] = 0xF0011;
    motData[2] = 0xF0012;
    motData[3] = -1;
    kokaObj = mbev_CapObjCreate(work->objList, 0xF000F, motData, 0, 5, 0);
    mbObjMotionSet(kokaObj, 1, 0x40000001);
    mbObjDispSet(kokaObj, FALSE);
    kokaPos = playerPos;
    kokaPos.y += 300.0f;
    mbObjPosSetV(kokaObj, &kokaPos);
    seNo = mbAudFXPlay(0x432);
    omVibrate(work->playerNo, 90, 7, 3);
    for(i=1; i<=90.0f; i++) {
        t = i/90.0f;
        rotY = 2160.0*HuSin(90.0f*t);
        radius = 150.0+(100.0*HuSin(720.0f*(1.0f-t)));
        glowY0 = (150.0f*t)+(100.0*HuSin(1440.0f*t));
        glowY1 = (150.0f*t)+(100.0*-HuSin(1440.0f*t));
        for(j=0; j<1.0f+(3.0f*t); j++) {
            vel.x = vel.y = vel.z = 0.0f;
            pos.x = kokaPos.x+(2.0f*(100.0f*(-0.5f+CAPTHROW_RANDF())));
            pos.y = kokaPos.y+(2.0f*(100.0f*CAPTHROW_RANDF()));
            pos.z = kokaPos.z+(2.0f*(100.0f*(-0.5f+CAPTHROW_RANDF())));
            no = mbev_CapEffGlowAdd(work->effGlow, pos, vel, 60.0f*(1.0f+(0.3f*CAPTHROW_RANDF())),
                100.0f*(0.15f+(0.05f*CAPTHROW_RANDF())), 0.05f+(0.02f*CAPTHROW_RANDF()), -0.08166666f, kokamekkuColorTbl[0]);
            mbev_CapEffGlowKinokoTimeSet(work->effGlow, no, mbRandMod(2)+2, 60.0f*(0.5f+(0.5f*CAPTHROW_RANDF())));
            pos.x = kokaPos.x+(2.0f*(100.0f*(-0.5f+CAPTHROW_RANDF())));
            pos.y = kokaPos.y+(2.0f*(100.0f*CAPTHROW_RANDF()));
            pos.z = kokaPos.z+(2.0f*(100.0f*(-0.5f+CAPTHROW_RANDF())));
            no = mbev_CapEffGlowAdd(work->effGlow, pos, vel, 60.0f*(1.0f+(0.3f*CAPTHROW_RANDF())),
                100.0f*(0.15f+(0.05f*CAPTHROW_RANDF())), 0.05f+(0.02f*CAPTHROW_RANDF()), -0.08166666f, kokamekkuColorTbl[1]);
            mbev_CapEffGlowKinokoTimeSet(work->effGlow, no, mbRandMod(2)+2, 60.0f*(0.5f+(0.5f*CAPTHROW_RANDF())));
        }
        if(t > 0.5f && t <= 1.0f) {
            s = 2.0f*(t-0.5f);
            if(s > 1.0f) {
                s = 1.0f;
            }
            mbObjRotSet(kokaObj, 0.0f, 720.0*HuSin(90.0f*(s*s)), 0.0f);
            mbObjScaleSet(kokaObj, 0.0001f+s, 1.0f, 0.0001f+s);
            mbObjDispSet(kokaObj, TRUE);
        }
        HuPrcVSleep();
    }
    if(seNo != -1) {
        mbAudFXStop(seNo);
    }
    mbAudFXPlay(0x433);
    for(i=0; i<512; i++) {
        pos.x = kokaPos.x+(2.0f*(100.0f*(-0.5f+CAPTHROW_RANDF())));
        pos.y = kokaPos.y+(2.0f*(100.0f*(-0.5f+CAPTHROW_RANDF())));
        pos.z = kokaPos.z+(2.0f*(100.0f*(-0.5f+CAPTHROW_RANDF())));
        rotY = 360.0f*CAPTHROW_RANDF();
        dist = 30.0f*CAPTHROW_RANDF();
        radius = 100.0f*(0.08f+(0.02f*CAPTHROW_RANDF()));
        vel.x = radius*(HuSin(dist)*HuSin(rotY));
        vel.y = radius*HuCos(dist);
        vel.z = radius*(HuSin(dist)*HuCos(rotY));
        mbev_CapEffGlowAdd(work->effGlow, pos, vel, 60.0f*(1.0f+(0.5f*CAPTHROW_RANDF())),
            100.0f*(0.15f+(0.05f*CAPTHROW_RANDF())), 0.05f+(0.02f*CAPTHROW_RANDF()), 0.08166666f, kokamekkuColorTbl[mbRandMod(2)]);
    }
    mbObjRotSet(kokaObj, 0.0f, 0.0f, 0.0f);
    mbObjScaleSet(kokaObj, 1.0f, 1.0f, 1.0f);
    HuPrcSleep(30);
    mbAudFXPlay(0x3A3);
    mbWinCreate(2, 0x36000A, 4);
    mbWinTopInsertMesSet(mbPlayerNameMesGet(targetNo), 0);
    mbWinTopWait();
    capNum = mbPlayerCapsuleNumGet(playerNo);
    if(capNum <= 0) {
        mbWinCreate(2, 0x36000B, 4);
        mbWinTopWait();
        for(i=1; i<60.0f; i++) {
            t = i/60.0f;
            pos.x = kokaPos.x;
            pos.y = kokaPos.y+(500.0*HuSin(90.0f*(t*t)));
            pos.z = kokaPos.z;
            mbObjPosSetV(kokaObj, &pos);
            HuPrcVSleep();
        }
        goto end;
    }
    if(capNum > 1) {
        capIdx = mbRandMod(capNum);
    } else {
        capIdx = 0;
    }
    capObj[0] = mbCapObjCreate(mbPlayerCapsuleGet(playerNo, capIdx), 0);
    mbObjCameraSet(capObj[0], 2);
    mbObjAttrSet(capObj[0], 0x40000001);
    mbObjMotionSpeedSet(capObj[0], 0.0f);
    capNum = 1;
    center.x = playerPos.x;
    center.y = 100.0f+playerPos.y;
    center.z = playerPos.z-125.0f;
    angleNext = 0.0f;
    angle = angleNext;
    angleStep = 360.0f/capNum;
    curNo = nextNo = 0;
    for(i=1; i<=12.0f; i++) {
        t = i/12.0f;
        KokamekkuObjUpdate(t, angle, capObj, capNum, curNo, -1, &center, &playerPos);
        HuPrcVSleep();
    }
    if(capNum > 1) {
        dirNum = 0;
        switch(mbRandMod(3)) {
            case 0:
                dirTbl[dirNum] = 1;
                dirNum++;
                dirTbl[dirNum] = -1;
                dirNum++;
                break;

            case 1:
                dirTbl[dirNum] = -1;
                dirNum++;
                dirTbl[dirNum] = 1;
                dirNum++;
                break;
        }
        no = mbRandMod(capNum);
        if(mbRandMod(0x8000) & 1) {
            for(i=0; i<no; i++) {
                dirTbl[dirNum] = 1;
                dirNum++;
            }
        } else {
            for(i=0; i<no; i++) {
                dirTbl[dirNum] = -1;
                dirNum++;
            }
        }
        no = mbRandMod(capNum);
        if(mbRandMod(0x8000) & 1) {
            for(i=0; i<no; i++) {
                dirTbl[dirNum] = 1;
                dirNum++;
            }
        } else {
            for(i=0; i<no; i++) {
                dirTbl[dirNum] = -1;
                dirNum++;
            }
        }
        dirTbl[dirNum] = 0;
        dirNum++;
        for(no=0; no<dirNum; no++) {
            mbObjMotionSpeedSet(capObj[curNo], 1.0f);
            if(dirTbl[no] == 0) {
                break;
            }
            HuPrcSleep(mbRandMod(20)+10);
            if(dirTbl[no] > 0) {
                angleNext = angle+angleStep;
                nextNo = curNo+1;
            } else {
                angleNext = angle-angleStep;
                nextNo = curNo-1;
            }
            if(angleNext > 360.0f) {
                angleNext -= 360.0f;
            } else if(angleNext < 0.0f) {
                angleNext += 360.0f;
            }
            if(nextNo >= capNum) {
                nextNo -= capNum;
            } else if(nextNo < 0) {
                nextNo += capNum;
            }
            mbObjMotionSpeedSet(capObj[curNo], 0.0f);
            mbObjMotionTimeSet(capObj[curNo], 0.0f);
            for(i=1; i<=12.0f; i++) {
                t = i/12.0f;
                KokamekkuObjUpdate(1.0f, mbev_CapAngleSumLerp(t, angle, angleNext), capObj, capNum, -1, -1, &center, &playerPos);
                for(j=0; j<capNum; j++) {
                    if(j == curNo) {
                        scale = 1.0+(-0.25f*t);
                    } else if(j == nextNo) {
                        scale = 0.75f+(0.25f*t);
                    } else {
                        scale = 0.75f;
                    }
                    mbObjScaleSet(capObj[j], scale, scale, scale);
                }
                HuPrcVSleep();
            }
            angle = angleNext;
            curNo = nextNo;
        }
        HuPrcSleep(mbRandMod(20)+10);
        capNo = mbPlayerCapsuleGet(playerNo, capIdx);
        mbWinCreate(2, 0x36000C, 4);
        mbWinTopInsertMesSet(mbCapUseMesGet(capNo), 0);
        mbWinTopWait();
    } else {
        capNo = mbPlayerCapsuleGet(playerNo, capIdx);
        mbWinCreate(2, 0x36000C, 4);
        mbWinTopInsertMesSet(mbCapUseMesGet(capNo), 0);
        mbWinTopWait();
    }
    mbev_CapPlayerMotShiftSet(kokaObj, 2, 0, 1);
    mbev_CapPlayerMotShiftSet(kokaObj, 3, 0x40000001, 1);
    mbObjPosGet(capObj[curNo], &capPos);
    PSVECSubtract(&capPos, &kokaPos, &pos);
    dist = PSVECMag(&pos);
    mbAudFXPlay(0x434);
    for(i=1; i<=45.0f; i++) {
        t = i/45.0f;
        rotY = 360.0*HuSin(90.0f*(t*t));
        radius = dist*(HuCos(90.0f*t)+(2.0*HuSin(180.0f*t)));
        scale = 1.0f-t;
        pos.x = kokaPos.x+(radius*HuSin(rotY));
        pos.z = kokaPos.z+(radius*HuCos(rotY));
        pos.y = capPos.y+(t*((100.0f+kokaPos.y)-capPos.y));
        if(t < 0.2f) {
            s = 5.0f*t;
            if(s > 1.0f) {
                s = 1.0f;
            }
            pos.x = capPos.x+(s*(pos.x-capPos.x));
            pos.z = capPos.z+(s*(pos.z-capPos.z));
        }
        mbObjPosSetV(capObj[curNo], &pos);
        mbObjScaleSet(capObj[curNo], scale, scale, scale);
        HuPrcVSleep();
    }
    mbev_CapPlayerMotShiftWait(playerNo, 13, 0, 1);
    mbWipeDissolveFadeOutTime(1);
    mbPlayerCapsuleRemove(playerNo, capIdx);
    for(i=0; i<capNum; i++) {
        mbCapObjKill(capObj[i]);
    }
    pos.x = 0.0f;
    pos.y = 100.0f;
    pos.z = 0.0f;
    mbCameraMoveMasu(GwPlayer[targetNo].masuId, NULL, &pos, -1.0f, -1.0f, -1);
    mbPlayerPosGet(targetNo, &kokaPos);
    kokaPos.y += 300.0f;
    mbObjPosSetV(kokaObj, &kokaPos);
    mbObjMotionSet(kokaObj, 1, 0x40000001);
    mbWipeDissolveFadeIn();
    mbAudFXPlay(0x3A3);
    mbWinCreate(2, 0x36000D, 4);
    mbWinTopInsertMesSet(mbPlayerNameMesGet(playerNo), 0);
    mbWinTopInsertMesSet(mbCapUseMesGet(capNo), 1);
    mbWinTopPlayerDisable(targetNo);
    mbWinTopWait();
    mbev_CapPlayerMotShiftSet(kokaObj, 2, 0, 1);
    mbev_CapPlayerMotShiftSet(kokaObj, 3, 0x40000001, 1);
    if(mbPlayerCapsuleNumGet(targetNo) < mbPlayerCapsuleMaxGet()) {
        mbCapCapsuleGet(targetNo, capNo);
        mbPlayerCapsuleAdd(targetNo, capNo);
        mbPlayerWinLoseVoicePlay(targetNo, 12, 0x243);
        mbPlayerMotionShiftSet(targetNo, 12, 0.0f, 8.0f, 0);
        mbWinCreate(2, 0x36000E, -1);
        mbWinTopInsertMesSet(mbCapUseMesGet(capNo), 0);
        mbWinTopPlayerDisable(targetNo);
        mbWinTopWait();
        mbev_CapPlayerMotShiftSet(kokaObj, 1, 0x40000001, 1);
        mbPlayerMotionShiftSet(targetNo, 1, 0.0f, 8.0f, 0x40000001);
    } else {
        mbev_CapPlayerMotShiftSet(kokaObj, 1, 0x40000001, 1);
        mbAudFXPlay(0x3A4);
        mbWinCreate(2, 0x36000F, 4);
        mbWinTopPlayerDisable(targetNo);
        mbWinTopWait();
        mbev_CapPlayerMotShiftWait(targetNo, 13, 0, 1);
        mbPlayerMotionShiftSet(targetNo, 1, 0.0f, 8.0f, 0x40000001);
    }
    mbAudFXPlay(0x3A3);
    mbWinCreate(2, 0x360010, 4);
    mbWinTopPlayerDisable(targetNo);
    mbWinTopWait();
    mbAudFXPlay(0x41B);
    for(i=1; i<60.0f; i++) {
        t = i/60.0f;
        pos.x = kokaPos.x;
        pos.y = kokaPos.y+(500.0*HuSin(90.0f*(t*t)));
        pos.z = kokaPos.z;
        mbObjPosSetV(kokaObj, &pos);
        HuPrcVSleep();
    }
    mbev_CapPlayerMotShiftWait(playerNo, 1, 0x40000001, 1);
end:
    HuPrcEnd();
}

/* @@PART3@@ */
#include "game/board/capsule.h"
#include "game/audio.h"
#include "game/data.h"

/* capevent.c prototypes as retail calls them here; not yet in HEAD. */
void *mbev_CapMalloc(void *objList, int size);
int mbev_CapPlayerCheck(int playerNo, int otherNo);
int mbev_CapPlayerMotShiftCheck(int playerNo);
void mbev_CapObjMotionSet(int objId, int arg1, int motNo, int arg3, int arg4, u32 attr, int arg6, int arg7);
void mbev_CapPlayerMotionSet(int playerNo, int arg1, int motNo, int arg3, int arg4, u32 attr, int arg6, int arg7);
void *mbev_CapEffSnowCreate(void);
void *mbev_CapEffCapLoseCreate(void);
void mbev_CapEffColorSet(GXColor *color, int no);
int mbev_CapEffGlowAdd(void *eff, HuVecF pos, HuVecF vel, int time, float scale, float speed, float decay, GXColor color);
void mbev_CapEffDustHeavyAdd(void *eff, HuVecF pos);
int mbev_CapEffSnowAdd(void *eff, HuVecF *pos, int time);
void mbev_CapEffCapLoseAdd(void *eff, int playerNo, int num, float speed);
int mbev_CapEffCapLoseNumGet(void *eff);
/* Kamekku's afterimage trail (OMOBJ data of ev_CapKamekkuOMExec). Size 0x120. */
typedef struct CapKamekkuWork_s {
    /* 0x000 */ int objId;
    /* 0x004 */ int mode;
    /* 0x008 */ int histNo;
    /* 0x00C */ int time;
    /* 0x010 */ int motF;
    /* 0x014 */ int burstF;
    /* 0x018 */ int unk18;
    /* 0x01C */ float tpLvl[16];
    /* 0x05C */ HuVecF hist[16];
    /* 0x11C */ CAPEVENTWORK *capWork;
} CAPKAMEKKUWORK;

static void KokamekkuObjUpdate(float t, float rotOfs, int *objId, int num, int selNo, int skipNo, HuVecF *center, HuVecF *start);
static void ev_CapKamekkuOMExec(OMOBJ *obj);

static inline BOOL CapTeamFGet(void)
{
    return GwSystem.tagF;
}

void mbev_CapKokamekkuKill(void)
{
}

static void KokamekkuObjUpdate(float t, float rotOfs, int *objId, int num, int selNo, int skipNo, HuVecF *center, HuVecF *start)
{
    HuVecF endPos;
    HuVecF pos;
    float angle;
    float scale;
    int i;
    for(i=0; i<num; i++) {
        if(i == skipNo) {
            continue;
        }
        angle = 360.0f-(i*(360.0f/num));
        endPos.x = center->x+(125.0*HuSin(angle+rotOfs));
        endPos.y = 150.0f+center->y;
        endPos.z = center->z+(125.0*HuCos(angle+rotOfs));
        if(t < 1.0f) {
            pos.y = start->y+((endPos.y-start->y)*HuSin(90.0f*t));
            pos.x = start->x+(t*(endPos.x-start->x));
            pos.z = start->z+(t*(endPos.z-start->z));
        } else {
            pos = endPos;
        }
        if(selNo != -1) {
            if(i == selNo) {
                scale = 1.0f;
            } else {
                scale = 0.75f;
            }
            scale *= t;
            mbObjScaleSet(objId[i], scale, scale, scale);
        }
        mbObjPosSetV(objId[i], &pos);
    }
}

static GXColor kamekkuColorTbl[6] = {
    { 127, 255, 255, 255 },
    { 255, 127, 255, 255 },
    { 255, 255, 127, 255 },
    { 127, 127, 255, 255 },
    { 127, 255, 127, 255 },
    { 255, 127, 127, 255 },
};

void mbev_CapKamekku(void)
{
    CAPEVENTWORK *work = HuPrcCurrentGet()->property;
    int motTbl[16];
    HuVecF playerPos;
    HuVecF rot;
    HuVecF basePos;
    HuVecF glowVel;
    HuVecF pos;
    HuVecF glowPos;
    float t;
    float angle;
    float radius;
    int glowNo;
    int dummyObjId;
    int seNo;
    int i;
    int j;
    int playerNo;
    int partnerNo;
    int targetNo;
    CAPKAMEKKUWORK *objWork;
    int objId;
    int masuId;
    int num1;
    int num2;
    s8 *masuTbl1;
    s8 *masuTbl2;
    OMOBJ *obj;
    HU3D_MODEL *modelP;
    HSF_DATA *hsf;
    HSF_MATERIAL *material;

    mbev_CapWait(work);
    work->effGlow = mbev_CapEffGlowCreate();
    dummyObjId = mbev_CapObjCreate(work->objList, 0xF001B, NULL, 0, 5, 0);
    mbObjDispSet(dummyObjId, FALSE);
    playerNo = work->playerNo;
    targetNo = work->targetNo;
    for(i=0, partnerNo=-1; i<4; i++) {
        if(playerNo != i && mbev_CapPlayerCheck(i, playerNo)) {
            partnerNo = i;
        }
    }
    mbPlayerPosGet(playerNo, &playerPos);
    motTbl[0] = 0xF0014;
    motTbl[1] = 0xF0015;
    motTbl[2] = 0xF0016;
    motTbl[3] = 0xF0017;
    motTbl[4] = 0xF0018;
    motTbl[5] = -1;
    objId = mbev_CapObjCreate(work->objList, 0xF0013, motTbl, 0, 5, 0);
    mbObjMotionSet(objId, 1, HU3D_MOTATTR_LOOP);
    mbObjDispSet(objId, FALSE);
    basePos = playerPos;
    basePos.y += 200.0f;
    mbObjPosSetV(objId, &basePos);
    obj = omAddObjEx(mbObjMan, 32768, 16, 0, -1, ev_CapKamekkuOMExec);
    obj->stat |= OM_STAT_MODELPAUSE;
    objWork = obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(CAPKAMEKKUWORK), HU_MEMNUM_OVL);
    memset(objWork, 0, sizeof(CAPKAMEKKUWORK));
    objWork->objId = objId;
    objWork->mode = 0;
    objWork->histNo = 0;
    objWork->motF = 0;
    objWork->burstF = 0;
    objWork->unk18 = 0;
    objWork->capWork = work;
    for(i=0; i<16; i++) {
        obj->mdlId[i] = Hu3DModelCreateData(0xF001F);
        Hu3DModelCameraSet(obj->mdlId[i], HU3D_CAM0);
        Hu3DModelAttrSet(obj->mdlId[i], HU3D_MOTATTR_LOOP);
        t = 0.25f*((16-i)/16.0f);
        Hu3DModelTPLvlSet(obj->mdlId[i], t);
        Hu3DModelLayerSet(obj->mdlId[i], 3);
        Hu3DModelAttrSet(obj->mdlId[i], HU3D_ATTR_DISPOFF);
        Hu3DModelScaleSet(obj->mdlId[i], 1.2f, 1.2f, 1.2f);
        Hu3DMotionTimeSet(obj->mdlId[i], Hu3DMotionMaxTimeGet(obj->mdlId[i])-(0.35f*i));
        modelP = &Hu3DData[obj->mdlId[i]];
        hsf = modelP->hsf;
        material = hsf->material;
        for(j=0; j<hsf->materialNum; j++, material++) {
            material->flags |= 0x10;
        }
        objWork->tpLvl[i] = t;
    }
    seNo = mbAudFXPlay(0x428);
    for(i=1; i<=180.0f; i++) {
        t = i/180.0f;
        radius = 4.0*(100.0*HuCos(90.0f*t));
        angle = 630.0f*t;
        mbPlayerPosGet(work->playerNo, &playerPos);
        pos.x = basePos.x+(radius*HuSin(angle));
        pos.y = basePos.y+(4.5*(100.0*HuCos(90.0f*t)));
        pos.z = basePos.z+(radius*HuCos(angle));
        rot.x = 0.0f;
        rot.y = 90.0f+angle;
        rot.z = 0.0f;
        mbObjPosSetV(objId, &pos);
        mbObjRotSetV(objId, &rot);
        mbObjDispSet(objId, TRUE);
        for(j=0; j<4; j++) {
            glowPos.x = pos.x+(0.75f*(100.0f*(-0.5f+CAPTHROW_RANDF())));
            glowPos.y = pos.y+(0.75f*(100.0f*(-0.5f+CAPTHROW_RANDF())));
            glowPos.z = pos.z+(0.75f*(100.0f*(-0.5f+CAPTHROW_RANDF())));
            glowVel.x = glowVel.y = glowVel.z = 0.0f;
            glowNo = mbev_CapEffGlowAdd(work->effGlow, glowPos, glowVel, 60.0f*(1.0f+(0.3f*CAPTHROW_RANDF())),
                100.0f*(0.15f+(0.05f*CAPTHROW_RANDF())), 0.05f+(0.02f*CAPTHROW_RANDF()), 0.08166666f, kamekkuColorTbl[mbRandMod(6)]);
            mbev_CapEffGlowKinokoTimeSet(work->effGlow, glowNo, 1, 90);
        }
        HuPrcVSleep();
    }
    if(seNo != -1) {
        mbAudFXStop(seNo);
    }
    mbAudFXPlay(0x3C7);
    mbWinCreate(2, 0x360011, 5);
    mbWinTopInsertMesSet(mbPlayerNameMesGet(targetNo), 0);
    mbWinTopWait();
    masuTbl1 = mbev_CapMalloc(work->objList, 256);
    memset(masuTbl1, 0, 256);
    for(i=1, num1=0; i<mbMasuNumGet(); i++) {
        if(mbCapMasuDispTypeGet(i) == 1 && playerNo == mbCapMasuPlayerGet(i)) {
            masuTbl1[num1] = i;
            num1++;
        }
    }
    if(CapTeamFGet() && num1 <= 0 && partnerNo >= 0) {
        for(i=1, num1=0; i<mbMasuNumGet(); i++) {
            if(mbCapMasuDispTypeGet(i) == 1 && partnerNo == mbCapMasuPlayerGet(i)) {
                masuTbl1[num1] = i;
                num1++;
            }
        }
    }
    masuTbl2 = mbev_CapMalloc(work->objList, 256);
    memset(masuTbl2, 0, 256);
    for(i=1, num2=0; i<mbMasuNumGet(); i++) {
        if(mbCapMasuDispTypeGet(i) == 2 && playerNo == mbCapMasuPlayerGet(i)) {
            masuTbl2[num2] = i;
            num2++;
        }
    }
    if(CapTeamFGet() && num2 <= 0 && partnerNo >= 0) {
        for(i=1, num2=0; i<mbMasuNumGet(); i++) {
            if(mbCapMasuDispTypeGet(i) == 2 && partnerNo == mbCapMasuPlayerGet(i)) {
                masuTbl2[num2] = i;
                num2++;
            }
        }
    }
    if(num1 <= 0 && num2 <= 0) {
        mbAudFXPlay(0x3C8);
        mbWinCreate(2, 0x360012, 5);
        mbWinTopWait();
        for(i=1; i<60.0f; i++) {
            t = i/60.0f;
            pos.x = basePos.x;
            pos.y = basePos.y+(500.0*HuSin(90.0f*(t*t)));
            pos.z = basePos.z;
            mbObjPosSetV(objId, &pos);
            HuPrcVSleep();
        }
    } else {
        if(num1 > 0) {
            masuId = masuTbl1[mbRandMod(num1)];
        } else {
            masuId = masuTbl2[mbRandMod(num2)];
        }
        mbWipeDissolveFadeOutTime(1);
        pos.x = pos.z = 0.0f;
        pos.y = 100.0f;
        mbCameraMoveMasu(masuId, NULL, &pos, -1.0f, -1.0f, -1);
        mbCameraMoveWait();
        mbMasuPosGet(masuId, &pos);
        basePos.x = pos.x;
        basePos.y = 200.0f+pos.y;
        basePos.z = pos.z;
        mbObjPosSetV(objId, &basePos);
        for(i=0; i<4; i++) {
            mbPlayerDispSet(i, FALSE);
        }
        mbWipeDissolveFadeIn();
        mbAudFXPlay(0x429);
        mbObjMotionShiftSet(objId, 2, 0.0f, 8.0f, HU3D_MOTATTR_NONE);
        while(mbObjMotionShiftIDGet(objId) != -1) {
            HuPrcVSleep();
        }
        objWork->mode++;
        while(!mbObjMotionEndCheck(objId)) {
            HuPrcVSleep();
        }
        objWork->motF = TRUE;
        mbev_CapObjMotionSet(objId, 0, 3, 1, 0, HU3D_MOTATTR_LOOP, 1, 1);
        while(mbObjMotionTimeGet(objId) < (mbObjMotionMaxTimeGet(objId)/2.0f) || mbObjMotionShiftIDGet(objId) != -1) {
            HuPrcVSleep();
        }
        omVibrate(playerNo, 20, 7, 3);
        omVibrate(targetNo, 20, 7, 3);
        mbMasuPosGet(masuId, &pos);
        mbCapAutoThrow(&pos, &pos, &pos, targetNo, masuId, mbCapValueTypeGet((int)mbMasuCapsuleGet(masuId)), 1, 1.0f);
        HuPrcSleep(30);
        mbWipeDissolveFadeOutTime(1);
        pos.x = pos.z = 0.0f;
        pos.y = 100.0f;
        mbCameraMoveMasu(GwPlayer[playerNo].masuId, NULL, &pos, -1.0f, -1.0f, -1);
        mbCameraMoveWait();
        mbPlayerPosGet(playerNo, &pos);
        basePos.x = pos.x;
        basePos.y = 200.0f+pos.y;
        basePos.z = pos.z;
        mbObjPosSetV(objId, &basePos);
        for(i=0; i<4; i++) {
            mbPlayerDispSet(i, TRUE);
        }
        mbWipeDissolveFadeIn();
        mbObjMotionShiftSet(objId, 5, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
        mbev_CapPlayerMotionSet(playerNo, 0, 13, 1, 0, HU3D_MOTATTR_LOOP, 1, 1);
        mbAudFXPlay(0x3C7);
        mbWinCreate(2, 0x360013, 5);
        mbWinTopInsertMesSet(mbPlayerNameMesGet(targetNo), 0);
        mbWinTopWait();
        mbObjMotionShiftSet(objId, 1, 5.0f, 8.0f, HU3D_MOTATTR_LOOP);
        obj->work[3] = TRUE;
        mbPlayerMotionSet(targetNo, 1, HU3D_MOTATTR_LOOP);
        mbObjPosGet(objId, &basePos);
        for(i=1; i<=60.0f; i++) {
            t = i/60.0f;
            pos.x = basePos.x;
            pos.y = basePos.y+(500.0*HuSin(90.0f*(t*t)));
            pos.z = basePos.z;
            mbObjPosSetV(objId, &pos);
            HuPrcVSleep();
        }
    }
    HuPrcEnd();
}

void mbev_CapKamekkuKill(void)
{
}

static void ev_CapKamekkuOMExec(OMOBJ *obj)
{
    CAPKAMEKKUWORK *work = obj->data;
    CAPEVENTWORK *capWork = work->capWork;
    Mtx mtx;
    HuVecF hookPos;
    HuVecF pos;
    HuVecF vel;
    GXColor color;
    float t;
    float angle;
    float speed;
    int i;
    int prevNo;
    int histNo;

    if(work->mode >= 5 || obj->work[3] || mbExitCheck()) {
        for(i=0; i<obj->mdlcnt; i++) {
            if(obj->mdlId[i] != HU3D_MODELID_NONE) {
                Hu3DModelKill(obj->mdlId[i]);
            }
            obj->mdlId[i] = HU3D_MODELID_NONE;
        }
        omDelObjEx(mbObjMan, obj);
        return;
    }
    if(!work->burstF && work->motF && mbObjMotionShiftIDGet(work->objId) == -1 && mbObjMotionTimeGet(work->objId) > 29.0f) {
        Hu3DModelObjMtxGet(mbObjModelIDGet(work->objId), "itemhook_T", mtx);
        hookPos.x = mtx[0][3];
        hookPos.y = mtx[1][3];
        hookPos.z = mtx[2][3];
        for(i=0, angle=0.0f; i<192; i++) {
            angle += 10.0f*(1.0f+CAPTHROW_RANDF());
            pos.x = hookPos.x+(25.0f*(-0.5f+CAPTHROW_RANDF()));
            pos.y = hookPos.y+(25.0f*(-0.5f+CAPTHROW_RANDF()));
            pos.z = 25.0f+hookPos.z;
            speed = 2.5f*(0.2f+(1.8f*CAPTHROW_RANDF()));
            vel.x = speed*HuSin(angle);
            vel.y = speed*HuCos(angle);
            vel.z = 0.0f;
            t = CAPTHROW_RANDF();
            mbev_CapEffColorSet(&color, mbRandMod(0x8000));
            mbev_CapEffGlowAdd(capWork->effGlow, pos, vel, 100.0f*(0.3f+(0.1f*CAPTHROW_RANDF())), 60.0f*(0.5f+CAPTHROW_RANDF()), 0.0f, 0.0f, color);
        }
        work->burstF = TRUE;
        work->mode = 4;
    }
    switch(work->mode) {
        case 0:
            break;

        case 1:
            Hu3DModelObjMtxGet(mbObjModelIDGet(work->objId), "itemhook_T", mtx);
            for(i=0; i<obj->mdlcnt; i++) {
                work->hist[i].x = mtx[0][3];
                work->hist[i].y = mtx[1][3];
                work->hist[i].z = mtx[2][3];
                Hu3DModelAttrReset(obj->mdlId[i], HU3D_ATTR_DISPOFF);
            }
            work->mode++;
            work->time = 0;

        case 2:
            t = ++work->time/60.0f;
            if(t > 1.0f) {
                t = 1.0f;
            }
            Hu3DModelObjMtxGet(mbObjModelIDGet(work->objId), "itemhook_T", mtx);
            work->hist[work->histNo].x = mtx[0][3];
            work->hist[work->histNo].y = mtx[1][3];
            work->hist[work->histNo].z = mtx[2][3];
            histNo = work->histNo;
            if(++work->histNo >= obj->mdlcnt) {
                work->histNo = 0;
            }
            for(i=0; i<obj->mdlcnt; i++) {
                prevNo = histNo-i;
                if(prevNo < 0) {
                    prevNo += obj->mdlcnt;
                }
                Hu3DModelPosSet(obj->mdlId[i], work->hist[prevNo].x, work->hist[prevNo].y, work->hist[prevNo].z);
                Hu3DModelTPLvlSet(obj->mdlId[i], work->tpLvl[i]*t);
            }
            if(t >= 1.0f) {
                work->mode++;
            }
            break;

        case 3:
            Hu3DModelObjMtxGet(mbObjModelIDGet(work->objId), "itemhook_T", mtx);
            work->hist[work->histNo].x = mtx[0][3];
            work->hist[work->histNo].y = mtx[1][3];
            work->hist[work->histNo].z = mtx[2][3];
            histNo = work->histNo;
            if(++work->histNo >= obj->mdlcnt) {
                work->histNo = 0;
            }
            for(i=0; i<obj->mdlcnt; i++) {
                prevNo = histNo-i;
                if(prevNo < 0) {
                    prevNo += obj->mdlcnt;
                }
                Hu3DModelPosSet(obj->mdlId[i], work->hist[prevNo].x, work->hist[prevNo].y, work->hist[prevNo].z);
            }
            break;

        case 4:
            Hu3DModelObjMtxGet(mbObjModelIDGet(work->objId), "itemhook_T", mtx);
            work->hist[work->histNo].x = mtx[0][3];
            work->hist[work->histNo].y = mtx[1][3];
            work->hist[work->histNo].z = mtx[2][3];
            histNo = work->histNo;
            if(++work->histNo >= obj->mdlcnt) {
                work->histNo = 0;
            }
            for(i=0; i<obj->mdlcnt; i++) {
                prevNo = histNo-i;
                if(prevNo < 0) {
                    prevNo += obj->mdlcnt;
                }
                if((work->tpLvl[i] *= 0.9f) < 0.01f) {
                    work->tpLvl[i] = 0.0f;
                }
                Hu3DModelPosSet(obj->mdlId[i], work->hist[prevNo].x, work->hist[prevNo].y, work->hist[prevNo].z);
                Hu3DModelTPLvlSet(obj->mdlId[i], work->tpLvl[i]);
            }
            break;

        case 5:
            obj->work[3] = TRUE;
            break;
    }
}

void mbev_CapThrowman(void)
{
    CAPEVENTWORK *work = HuPrcCurrentGet()->property;
    HuVecF playerPos;
    HuVecF pos;
    HuVecF objPos;
    HuVecF vel;
    int motId[3];
    float t;
    float alphaT;
    float sinT;
    int i;
    int objId;
    HU3D_MODEL *modelP;
    int masuId;
    int seNo;

    mbev_CapWait(work);
    work->effExplode = mbev_CapEffExplodeCreate();
    work->effSnow = mbev_CapEffSnowCreate();
    work->effCapLose = mbev_CapEffCapLoseCreate();
    motId[0] = mbev_CapPlayerMotionCreate(work->objList, work->playerNo, 0x930017);
    motId[1] = mbev_CapPlayerMotionCreate(work->objList, work->playerNo, 0x930019);
    motId[2] = mbev_CapPlayerMotionCreate(work->objList, work->playerNo, 0x93002E);
    masuId = GwPlayer[work->playerNo].masuId;
    mbPlayerPosGet(work->playerNo, &playerPos);
    seNo = mbAudFXPlay(0x474);
    for(i=0; i<120.0f; i++) {
        if((i & 3) == 0) {
            pos.x = playerPos.x+(200.0f*(CAPTHROW_RANDF()-0.5f));
            pos.y = playerPos.y+(600.0f*(1.0f+(0.2f*CAPTHROW_RANDF())));
            pos.z = playerPos.z+(200.0f*(CAPTHROW_RANDF()-0.5f));
            mbev_CapEffSnowAdd(work->effSnow, &pos, 60.0f*(3.5f+CAPTHROW_RANDF()));
        }
        HuPrcVSleep();
    }
    mbPlayerMotionShiftSet(work->playerNo, motId[0], 0.0f, 8.0f, HU3D_MOTATTR_NONE);
    i = mbPlayerObjIDGet(work->playerNo);
    i = mbObjModelIDGet(i);
    modelP = &Hu3DData[i];
    modelP->motShiftWork.speed = 3.0f;
    i = 0;
    do {
        HuPrcVSleep();
        i++;
        if((i & 3) == 0) {
            pos.x = playerPos.x+(200.0f*(CAPTHROW_RANDF()-0.5f));
            pos.y = playerPos.y+(600.0f*(1.0f+(0.2f*CAPTHROW_RANDF())));
            pos.z = playerPos.z+(200.0f*(CAPTHROW_RANDF()-0.5f));
            mbev_CapEffSnowAdd(work->effSnow, &pos, 60.0f*(3.5f+CAPTHROW_RANDF()));
        }
    } while(!mbev_CapPlayerMotShiftCheck(work->playerNo) || !mbPlayerMotionEndCheck(work->playerNo));
    mbPlayerMotionShiftSet(work->playerNo, motId[1], 0.0f, 8.0f, HU3D_MOTATTR_NONE);
    objId = mbev_CapObjCreate(work->objList, 0xF0019, NULL, 0, 5, 0);
    objPos = playerPos;
    objPos.y += 800.0f;
    mbObjPosSet(objId, objPos.x, objPos.y, objPos.z);
    mbObjLayerSet(objId, 5);
    vel.x = vel.y = vel.z = 0.0f;
    do {
        HuPrcVSleep();
        mbPlayerPosGet(work->playerNo, &playerPos);
        PSVECAdd(&objPos, &vel, &objPos);
        vel.y -= 2.4500003f;
        mbObjPosSet(objId, objPos.x, objPos.y, objPos.z);
    } while(objPos.y > playerPos.y);
    objPos.y = 10.0f+playerPos.y;
    mbObjPosSet(objId, objPos.x, objPos.y, objPos.z);
    if(seNo != -1) {
        mbAudFXStop(seNo);
    }
    mbAudFXPlay(0x42B);
    mbev_CapEffDustHeavyAdd(work->effExplode, playerPos);
    objPos = playerPos;
    mbObjPosSet(objId, objPos.x, objPos.y, objPos.z);
    mbPlayerMotionSet(work->playerNo, 10, HU3D_MOTATTR_NONE);
    mbPlayerMotionSpeedSet(work->playerNo, 0.0f);
    mbPlayerMotionTimeSet(work->playerNo, 20.0f);
    for(i=0; i<4; i++) {
        if(i != work->playerNo && masuId == GwPlayer[i].masuId) {
            mbPlayerMotionShiftSet(i, 6, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
        }
        if(masuId == GwPlayer[i].masuId) {
            omVibrate(i, 20, 7, 3);
        }
    }
    if(mbPlayerCapsuleNumGet(work->playerNo) > 0) {
        mbev_CapEffCapLoseAdd(work->effCapLose, work->playerNo, mbPlayerCapsuleMaxGet(), 100.0f);
        for(i=0; i<mbPlayerCapsuleMaxGet(); i++) {
            mbPlayerCapsuleRemove(work->playerNo, 0);
        }
        do {
            HuPrcVSleep();
        } while(mbev_CapEffCapLoseNumGet(work->effCapLose) > 0);
        HuPrcSleep(60);
        mbWinCreate(2, 0x360014, -1);
        mbWinTopWait();
    } else {
        HuPrcSleep(60);
    }
    omVibrate(work->playerNo, 120, 4, 4);
    HuAudFXPlay(0x43A);
    for(i=0; i<120.0f; i++) {
        t = i/120.0f;
        sinT = HuSin(90.0f*t);
        if(t < 0.5f) {
            alphaT = 0.0f;
        } else {
            alphaT = 2.0f*(t-0.5f);
        }
        mbObjScaleSet(objId, 1.0f+t, 1.0f-t, 1.0f+t);
        mbObjAlphaSet(objId, 255.0f*(1.0f-alphaT));
        HuPrcVSleep();
    }
    mbPlayerMotionSpeedSet(work->playerNo, 1.0f);
    do {
        HuPrcVSleep();
    } while(!mbPlayerMotionEndCheck(work->playerNo));
    mbPlayerMotionShiftSet(work->playerNo, 6, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
    for(i=0; i<60.0f; i++) {
        HuPrcVSleep();
    }
    mbev_CapPlayerIdleWait();
    HuPrcEnd();
}

void mbev_CapThrowmanKill(void)
{
}
/* @@END@@ */
