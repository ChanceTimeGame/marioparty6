/* @@HEAD@@ */
#include "game/board/capthrow.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
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
    /* 0xBD8 */ u8 unkBD8[4];
    /* 0xBDC */ void *effGlow;
    /* 0xBE0 */ void *effRing;
    /* 0xBE4 */ void *effCoin;
    /* 0xBE8 */ u8 unkBE8[0xBF4 - 0xBE8];
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
void mbev_CapPlayerMoveObjInit(void);
s16 mbev_CapPlayerMotionCreate(void *objList, int playerNo, int dataNum);
int mbev_CapObjCreate(void *objList, int dataNum, int *motData, int arg3, int arg4, int arg5);
void mbev_CapVecChase(float t, HuVecF *a, HuVecF *b, HuVecF *out);
float mbev_CapAngleSumLerp(float t, float start, float end);
int mbev_CapCoinDisp(int playerNo, int coinNum, int arg2, int arg3);
void mbev_CapCoinAdd(void *effCoin, int playerNo, int coinNum, int arg3);
void mbev_CapPlayerMotShiftWait(int playerNo, int motNo, u32 attr, BOOL waitF);

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

/* @@PART2@@ */
/* @@PART3@@ */
/* @@END@@ */
