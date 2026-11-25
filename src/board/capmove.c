/* @@HEAD@@ */
#include "game/board/capmove.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/exit.h"
#include "game/board/guide.h"
#include "game/board/masu.h"
#include "game/board/object.h"
#include "game/board/player.h"
#include "game/board/roulette.h"
#include "game/board/star.h"
#include "game/board/window.h"
#include "game/board/wipe.h"

#include "game/gamework.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/process.h"
#include "math.h"
#include "string.h"

/* Capsule event work (capevent.c), the process property of every capsule event. Size 0xBF4. */
typedef struct CapEventWork_s {
    /* 0x000 */ int playerNo;
    /* 0x004 */ int unk4;
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
    /* 0xBE4 */ u8 unkBE4[0xBF4 - 0xBE4];
} CAPEVENTWORK;

/* capevent.h and capsule.h prototypes as retail calls them here (local declarations; see report). */
void mbev_CapWait(CAPEVENTWORK *work);
void *mbev_CapEffExplodeCreate(void);
void *mbev_CapEffGlowCreate(void);
void *mbev_CapEffRingCreate(void);
void mbev_CapEffGlowBlendModeSet(void *eff, u8 mode);
int mbev_CapEffGlowDispGet(void *eff);
int mbev_CapEffExplodeAnimGet(void *eff);
void mbev_CapEffDustExplodeAdd(void *eff, HuVecF *pos);
int mbev_CapObjCreate(void *objList, int dataNum, int *motData, int arg3, int arg4, int arg5);
s16 mbev_CapPlayerMotionCreate(void *objList, int playerNo, int dataNum);
void mbev_CapRandomBonusCoin(int playerNo, int capNo, int arg2);
int mbev_CapBonusCoinCheck(int playerNo);
int mbev_CapPlayerMasuNumGet(int masuId);
void mbev_CapBubbleHookCall(int arg0, int arg1, int arg2, int arg3, int arg4);
void mbev_CapBezierGetV(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *out, float t);
void mbev_CapPlayerMotShiftWait(int playerNo, int motNo, int attr, int arg3);
int mbCapEffUseCreate(int playerNo, int capNo);
int mbCapEffUseModeGet(int playerNo);

static HuVecF capsuleCameraOfs = { 0.0f, 100.0f, 0.0f };

static HuVecF hanachanPlayerOfs[14] = {
    { 0.0f, -130.0f, 30.000002f },
    { 0.0f, -135.0f, 30.000002f },
    { -20.0f, -120.00001f, 80.0f },
    { 0.0f, -130.0f, 30.000002f },
    { 0.0f, -120.00001f, 40.0f },
    { -20.0f, -130.0f, 80.0f },
    { 0.0f, -140.0f, 30.000002f },
    { 0.0f, -100.0f, 50.0f },
    { 0.0f, -104.99999f, 70.0f },
    { -10.0f, -130.0f, 30.000002f },
    { 0.0f, -100.0f, 50.0f },
    { -10.0f, -130.0f, 30.000002f },
    { -10.0f, -130.0f, 30.000002f },
    { -10.0f, -130.0f, 30.000002f },
};

static void ev_CapHanachanOMExec(OMOBJ *obj);
static void ev_CapEffKinokoCreate(CAPEVENTWORK *work);

/* @@PART1@@ */
void mbev_CapKinoko(void)
{
    CAPEVENTWORK *work = HuPrcCurrentGet()->property;
    HuVecF pos;
    HuVecF playerPos;
    float t;
    float scale;
    float sinT;
    float y;
    int i;
    int objId;

    mbev_CapWait(work);
    work->effExplode = mbev_CapEffExplodeCreate();
    work->effGlow = mbev_CapEffGlowCreate();
    mbev_CapEffGlowBlendModeSet(work->effGlow, 1);
    mbPlayerPosGet(work->playerNo, &pos);
    pos.y += 250.0f;
    objId = mbev_CapObjCreate(work->objList, 0xC0044, NULL, 0, 0, 0);
    mbObjPosSet(objId, pos.x, pos.y, pos.z);
    mbObjScaleSet(objId, 1.0f, 1.0f, 1.0f);
    mbObjDispSet(objId, FALSE);
    mbCapEffUseCreate(work->playerNo, work->capNo);
    while(mbCapEffUseModeGet(work->playerNo) < 2) {
        HuPrcVSleep();
    }
    mbObjDispSet(objId, TRUE);
    work->objId = objId;
    ev_CapEffKinokoCreate(work);
    for(i=0; i<30.0f; i++) {
        t = i/30.0f;
        scale = 1.0+HuSin(180.0f*t);
        mbObjScaleSet(objId, scale, scale, scale);
        HuPrcVSleep();
    }
    mbev_CapRandomBonusCoin(work->playerNo, work->capNo, 0);
    for(i=0; i<60.0f || mbev_CapBonusCoinCheck(work->playerNo) == 0; i++) {
        t = i/60.0f;
        mbPlayerPosGet(work->playerNo, &pos);
        pos.y += 250.0+(0.1f*(100.0*HuSin(360.0f*t)));
        mbObjPosSet(objId, pos.x, pos.y, pos.z);
        HuPrcVSleep();
    }
    for(i=1; i<=10; i++) {
        t = i/10.0f;
        mbPlayerPosGet(work->playerNo, &playerPos);
        pos.y += t*((250.0f+playerPos.y)-pos.y);
        mbObjPosSet(objId, pos.x, pos.y, pos.z);
        HuPrcVSleep();
    }
    mbAudFXPlay(0x410);
    for(i=0; i<90.0f; i++) {
        t = 1.0f-(i/90.0f);
        y = 50.0f+(200.0f*(t*t));
        sinT = HuSin(180.0f*(t*t));
        mbPlayerPosGet(work->playerNo, &pos);
        pos.x += 1.5*(100.0*(sinT*HuCos(2.0f*(360.0f*t))));
        pos.z += 1.5*(100.0*(sinT*HuSin(2.0f*(360.0f*t))));
        pos.y += y;
        mbObjPosSet(objId, pos.x, pos.y, pos.z);
        mbObjScaleSet(objId, t, t, t);
        HuPrcVSleep();
    }
    GwPlayer[work->playerNo].diceMode = 1;
    omVibrate(work->playerNo, 20, 4, 4);
    for(i=0; i<6.0f; i++) {
        HuPrcVSleep();
    }
    while(mbev_CapEffGlowDispGet(work->effGlow) > 0) {
        HuPrcVSleep();
    }
    HuPrcEnd();
}

void mbev_CapKinokoKill(void)
{
}

void mbev_CapSKinoko(void)
{
    CAPEVENTWORK *work = HuPrcCurrentGet()->property;
    HuVecF pos;
    HuVecF playerPos;
    float t;
    float scale;
    float sinT;
    float y;
    int i;
    int objId;

    mbev_CapWait(work);
    work->effExplode = mbev_CapEffExplodeCreate();
    work->effGlow = mbev_CapEffGlowCreate();
    mbev_CapEffGlowBlendModeSet(work->effGlow, 1);
    mbPlayerPosGet(work->playerNo, &pos);
    pos.y += 250.0f;
    objId = mbev_CapObjCreate(work->objList, 0xC0045, NULL, 0, 0, 0);
    mbObjPosSet(objId, pos.x, pos.y, pos.z);
    mbObjScaleSet(objId, 1.0f, 1.0f, 1.0f);
    mbObjDispSet(objId, FALSE);
    mbCapEffUseCreate(work->playerNo, work->capNo);
    while(mbCapEffUseModeGet(work->playerNo) < 2) {
        HuPrcVSleep();
    }
    mbObjDispSet(objId, TRUE);
    work->objId = objId;
    ev_CapEffKinokoCreate(work);
    for(i=0; i<30.0f; i++) {
        t = i/30.0f;
        scale = 1.0+HuSin(180.0f*t);
        mbObjScaleSet(objId, scale, scale, scale);
        HuPrcVSleep();
    }
    mbev_CapRandomBonusCoin(work->playerNo, work->capNo, 0);
    for(i=0; i<60.0f || mbev_CapBonusCoinCheck(work->playerNo) == 0; i++) {
        t = i/60.0f;
        mbPlayerPosGet(work->playerNo, &pos);
        pos.y += 250.0+(0.1f*(100.0*HuSin(360.0f*t)));
        mbObjPosSet(objId, pos.x, pos.y, pos.z);
        HuPrcVSleep();
    }
    for(i=1; i<=10; i++) {
        t = i/10.0f;
        mbPlayerPosGet(work->playerNo, &playerPos);
        pos.y += t*((250.0f+playerPos.y)-pos.y);
        mbObjPosSet(objId, pos.x, pos.y, pos.z);
        HuPrcVSleep();
    }
    mbAudFXPlay(0x410);
    for(i=0; i<90.0f; i++) {
        t = 1.0f-(i/90.0f);
        y = 50.0f+(200.0f*(t*t));
        sinT = HuSin(180.0f*(t*t));
        mbPlayerPosGet(work->playerNo, &pos);
        pos.x += 1.5*(100.0*(sinT*HuCos(2.0f*(360.0f*t))));
        pos.z += 1.5*(100.0*(sinT*HuSin(2.0f*(360.0f*t))));
        pos.y += y;
        mbObjPosSet(objId, pos.x, pos.y, pos.z);
        mbObjScaleSet(objId, t, t, t);
        HuPrcVSleep();
    }
    GwPlayer[work->playerNo].diceMode = 2;
    omVibrate(work->playerNo, 20, 4, 4);
    for(i=0; i<6.0f; i++) {
        HuPrcVSleep();
    }
    while(mbev_CapEffGlowDispGet(work->effGlow) > 0) {
        HuPrcVSleep();
    }
    HuPrcEnd();
}

void mbev_CapSKinokoKill(void)
{
}

void mbev_CapPKinoko(void)
{
    CAPEVENTWORK *work = HuPrcCurrentGet()->property;
    HuVecF pos;
    HuVecF playerPos;
    float t;
    float scale;
    float sinT;
    float y;
    int i;
    int objId;

    mbev_CapWait(work);
    work->effExplode = mbev_CapEffExplodeCreate();
    work->effGlow = mbev_CapEffGlowCreate();
    mbev_CapEffGlowBlendModeSet(work->effGlow, 1);
    mbPlayerPosGet(work->playerNo, &pos);
    pos.y += 250.0f;
    objId = mbev_CapObjCreate(work->objList, 0xC0047, NULL, 0, 0, 0);
    mbObjPosSet(objId, pos.x, pos.y, pos.z);
    mbObjScaleSet(objId, 1.0f, 1.0f, 1.0f);
    mbObjDispSet(objId, FALSE);
    mbCapEffUseCreate(work->playerNo, work->capNo);
    while(mbCapEffUseModeGet(work->playerNo) < 2) {
        HuPrcVSleep();
    }
    mbObjDispSet(objId, TRUE);
    work->objId = objId;
    ev_CapEffKinokoCreate(work);
    for(i=0; i<30.0f; i++) {
        t = i/30.0f;
        scale = 1.0+HuSin(180.0f*t);
        mbObjScaleSet(objId, scale, scale, scale);
        HuPrcVSleep();
    }
    mbev_CapRandomBonusCoin(work->playerNo, work->capNo, 0);
    for(i=0; i<60.0f || mbev_CapBonusCoinCheck(work->playerNo) == 0; i++) {
        t = i/60.0f;
        mbPlayerPosGet(work->playerNo, &pos);
        pos.y += 250.0+(0.1f*(100.0*HuSin(360.0f*t)));
        mbObjPosSet(objId, pos.x, pos.y, pos.z);
        HuPrcVSleep();
    }
    for(i=1; i<=10; i++) {
        t = i/10.0f;
        mbPlayerPosGet(work->playerNo, &playerPos);
        pos.y += t*((250.0f+playerPos.y)-pos.y);
        mbObjPosSet(objId, pos.x, pos.y, pos.z);
        HuPrcVSleep();
    }
    mbAudFXPlay(0x410);
    for(i=0; i<90.0f; i++) {
        t = 1.0f-(i/90.0f);
        y = 50.0f+(200.0f*(t*t));
        sinT = HuSin(180.0f*(t*t));
        mbPlayerPosGet(work->playerNo, &pos);
        pos.x += 1.5*(100.0*(sinT*HuCos(2.0f*(360.0f*t))));
        pos.z += 1.5*(100.0*(sinT*HuSin(2.0f*(360.0f*t))));
        pos.y += y;
        mbObjPosSet(objId, pos.x, pos.y, pos.z);
        mbObjScaleSet(objId, t, t, t);
        HuPrcVSleep();
    }
    GwPlayer[work->playerNo].diceMode = 3;
    omVibrate(work->playerNo, 20, 4, 4);
    for(i=0; i<6.0f; i++) {
        HuPrcVSleep();
    }
    while(mbev_CapEffGlowDispGet(work->effGlow) > 0) {
        HuPrcVSleep();
    }
    HuPrcEnd();
}

void mbev_CapPKinokoKill(void)
{
}

void mbev_CapMKinoko(void)
{
    CAPEVENTWORK *work = HuPrcCurrentGet()->property;
    HuVecF pos;
    HuVecF playerPos;
    float t;
    float scale;
    float sinT;
    float y;
    int i;
    int objId;

    mbev_CapWait(work);
    work->effExplode = mbev_CapEffExplodeCreate();
    work->effGlow = mbev_CapEffGlowCreate();
    mbev_CapEffGlowBlendModeSet(work->effGlow, 1);
    mbPlayerPosGet(work->playerNo, &pos);
    pos.y += 250.0f;
    objId = mbev_CapObjCreate(work->objList, 0xC0048, NULL, 0, 0, 0);
    mbObjPosSet(objId, pos.x, pos.y, pos.z);
    mbObjScaleSet(objId, 1.0f, 1.0f, 1.0f);
    mbObjDispSet(objId, FALSE);
    mbCapEffUseCreate(work->playerNo, work->capNo);
    while(mbCapEffUseModeGet(work->playerNo) < 2) {
        HuPrcVSleep();
    }
    mbObjDispSet(objId, TRUE);
    work->objId = objId;
    ev_CapEffKinokoCreate(work);
    for(i=0; i<30.0f; i++) {
        t = i/30.0f;
        scale = 1.0+HuSin(180.0f*t);
        mbObjScaleSet(objId, scale, scale, scale);
        HuPrcVSleep();
    }
    mbev_CapRandomBonusCoin(work->playerNo, work->capNo, 0);
    for(i=0; i<60.0f || mbev_CapBonusCoinCheck(work->playerNo) == 0; i++) {
        t = i/60.0f;
        mbPlayerPosGet(work->playerNo, &pos);
        pos.y += 250.0+(0.1f*(100.0*HuSin(360.0f*t)));
        mbObjPosSet(objId, pos.x, pos.y, pos.z);
        HuPrcVSleep();
    }
    for(i=1; i<=10; i++) {
        t = i/10.0f;
        mbPlayerPosGet(work->playerNo, &playerPos);
        pos.y += t*((250.0f+playerPos.y)-pos.y);
        mbObjPosSet(objId, pos.x, pos.y, pos.z);
        HuPrcVSleep();
    }
    mbAudFXPlay(0x410);
    for(i=0; i<90.0f; i++) {
        t = 1.0f-(i/90.0f);
        y = 50.0f+(200.0f*(t*t));
        sinT = HuSin(180.0f*(t*t));
        mbPlayerPosGet(work->playerNo, &pos);
        pos.x += 1.5*(100.0*(sinT*HuCos(2.0f*(360.0f*t))));
        pos.z += 1.5*(100.0*(sinT*HuSin(2.0f*(360.0f*t))));
        pos.y += y;
        mbObjPosSet(objId, pos.x, pos.y, pos.z);
        mbObjScaleSet(objId, t, t, t);
        HuPrcVSleep();
    }
    GwPlayer[work->playerNo].diceMode = 4;
    mbPlayerMetalSet(work->playerNo, TRUE);
    omVibrate(work->playerNo, 20, 4, 4);
    mbAudFXPlay(0x431);
    while(mbev_CapEffGlowDispGet(work->effGlow) > 0) {
        HuPrcVSleep();
    }
    HuPrcEnd();
}

void mbev_CapMKinokoKill(void)
{
}

void mbev_CapKiller(void)
{
    CAPEVENTWORK *work = HuPrcCurrentGet()->property;

    mbev_CapWait(work);
    mbCapEffUseCreate(work->playerNo, work->capNo);
    omVibrate(work->playerNo, 20, 4, 4);
    HuPrcSleep(20);
    mbev_CapRandomBonusCoin(work->playerNo, work->capNo, 1);
    while(mbCapEffUseModeGet(work->playerNo) >= 0) {
        HuPrcVSleep();
    }
    GwPlayer[work->playerNo].diceMode = 5;
    HuPrcEnd();
}

void mbev_CapKillerKill(void)
{
}

/* @@PART2@@ */
void mbev_CapHanachanKill(void)
{
}

static void ev_CapHanachanOMExec(OMOBJ *obj)
{
}

void mbev_CapNKinoko(void)
{
}

void mbev_CapNKinokoKill(void)
{
}

void mbev_CapKillerMove(void)
{
}

void mbev_CapKillerMoveKill(void)
{
}

static void ev_CapEffKinokoCreate(CAPEVENTWORK *work)
{
}

/* @@END@@ */
