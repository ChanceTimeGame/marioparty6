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
    HuVecF playerPos;
    HuVecF pos;
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
    HuVecF playerPos;
    HuVecF pos;
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
    HuVecF playerPos;
    HuVecF pos;
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
    HuVecF playerPos;
    HuVecF pos;
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

void mbev_CapDokan(void)
{
    HuVecF masuPos[2];
    HuVecF playerPos0;
    HuVecF playerPos1;
    int motId[2][2];
    int objId[2];
    int playerNo[2];
    int masuId[2];
    CAPEVENTWORK *work;
    float t;
    float scale;
    int i;
    int j;
    int otherNo;

    work = HuPrcCurrentGet()->property;
    mbCapEffUseCreate(work->playerNo, work->capNo);
    omVibrate(work->playerNo, 20, 4, 4);
    HuPrcSleep(20);
    mbev_CapRandomBonusCoin(work->playerNo, work->capNo, 1);
    while(mbCapEffUseModeGet(work->playerNo) >= 0) {
        HuPrcVSleep();
    }
    mbWinCreate(2, 0x350001, -1);
    mbWinTopWait();
    if(mbRouletteCreate(work->playerNo, 3) != FALSE) {
        mbRouletteWait();
    }
    otherNo = mbRouletteResultGet();
    playerNo[0] = work->playerNo;
    playerNo[1] = otherNo;
    masuId[0] = GwPlayer[playerNo[0]].masuId;
    masuId[1] = GwPlayer[playerNo[1]].masuId;
    if(masuId[0] == masuId[1]) {
        mbWinCreate(2, 0x350000, -1);
        mbWinTopWait();
        HuPrcEnd();
    }
    if(mbev_CapPlayerMasuNumGet(masuId[0]) > 1) {
        mbev_PlayerColCircleAdd(playerNo[0], masuId[0], FALSE, 125.0f);
        while(GwPlayer[playerNo[0]].moveF) {
            HuPrcVSleep();
        }
    }
    if(mbev_CapPlayerMasuNumGet(masuId[1]) > 1) {
        mbev_PlayerColCircleAdd(playerNo[1], masuId[1], FALSE, 125.0f);
        while(GwPlayer[playerNo[1]].moveF) {
            HuPrcVSleep();
        }
    }
    mbPlayerPosGet(playerNo[0], &playerPos0);
    mbPlayerPosGet(playerNo[1], &playerPos1);
    motId[0][0] = mbev_CapPlayerMotionCreate(work->objList, playerNo[0], 0x930044);
    HuPrcVSleep();
    motId[0][1] = mbev_CapPlayerMotionCreate(work->objList, playerNo[0], 0x930045);
    HuPrcVSleep();
    motId[1][0] = mbev_CapPlayerMotionCreate(work->objList, playerNo[1], 0x930044);
    HuPrcVSleep();
    motId[1][1] = mbev_CapPlayerMotionCreate(work->objList, playerNo[1], 0x930045);
    HuPrcVSleep();
    mbev_CapWait(work);
    for(i=0; i<2; i++) {
        objId[i] = mbev_CapObjCreate(work->objList, 0xD0004, NULL, 1, 5, 0);
        mbMasuPosGet(masuId[i], &masuPos[i]);
        mbObjDispSet(objId[i], FALSE);
        HuPrcVSleep();
    }
    mbAudFXPlay(0x413);
    for(i=0; i<30.0f; i++) {
        t = i/30.0f;
        for(j=0; j<2; j++) {
            mbMasuPosGet(masuId[j], &masuPos[j]);
            mbObjPosSet(objId[j], masuPos[j].x, masuPos[j].y, masuPos[j].z);
            mbObjScaleSet(objId[j], t+(0.5*HuSin(180.0f*t)), t+(0.5*HuSin(180.0f*t)), t+(0.5*HuSin(180.0f*t)));
            mbObjDispSet(objId[j], TRUE);
            mbPlayerPosSet(playerNo[j], masuPos[j].x, masuPos[j].y+(100.0*(t+(0.5*HuSin(180.0f*t)))), masuPos[j].z);
            mbPlayerColSnapPlayerSet(playerNo[j], FALSE);
        }
        HuPrcVSleep();
    }
    for(i=0; i<2; i++) {
        if(motId[i][0] != -1) {
            mbPlayerMotionShiftSet(playerNo[i], motId[i][0], 0.0f, 8.0f, 0);
        }
        omVibrate(playerNo[i], 20, 7, 3);
    }
    mbAudFXPlay(0x414);
    for(i=0; i<30.0f; i++) {
        t = 1.0f-(i/30.0f);
        for(j=0; j<2; j++) {
            mbMasuPosGet(masuId[j], &masuPos[j]);
            mbObjPosSetV(objId[j], &masuPos[j]);
            mbPlayerPosSet(playerNo[j], masuPos[j].x, 100.0f+masuPos[j].y, masuPos[j].z);
            mbPlayerRotSet(playerNo[j], 0.0f, 720.0f*t, 0.0f);
            scale = 0.5f+(0.5f*t);
        }
        HuPrcVSleep();
    }
    scale = 0.0f;
    mbPlayerScaleSet(playerNo[0], scale, scale, scale);
    mbPlayerScaleSet(playerNo[1], scale, scale, scale);
    for(i=0; i<30.0f; i++) {
        t = i/30.0f;
        for(j=0; j<2; j++) {
            mbMasuPosGet(masuId[j], &masuPos[j]);
            mbObjPosSetV(objId[j], &masuPos[j]);
            mbObjPosSet(objId[j], masuPos[j].x, masuPos[j].y, masuPos[j].z);
            mbObjScaleSet(objId[j], 1.0f, 1.0f-t, 1.0f);
        }
        HuPrcVSleep();
    }
    mbObjDispSet(objId[0], FALSE);
    mbObjDispSet(objId[1], FALSE);
    mbWipeDissolveFadeOutTime(1);
    mbPlayerPosSetV(playerNo[0], &playerPos1);
    GwPlayer[playerNo[0]].masuId = masuId[1];
    mbPlayerPosSetV(playerNo[1], &playerPos0);
    GwPlayer[playerNo[1]].masuId = masuId[0];
    mbCameraMoveMasu(masuId[1], NULL, &capsuleCameraOfs, -1.0f, -1.0f, -1);
    mbCameraMoveWait();
    mbStarMasuDispSet(masuId[1], FALSE);
    mbWipeDissolveFadeIn();
    mbAudFXPlay(0x413);
    mbObjDispSet(objId[0], TRUE);
    mbObjDispSet(objId[1], TRUE);
    for(i=0; i<30.0f; i++) {
        t = i/30.0f;
        for(j=0; j<2; j++) {
            mbMasuPosGet(masuId[j], &masuPos[j]);
            mbObjPosSetV(objId[j], &masuPos[j]);
            mbObjScaleSet(objId[j], t+(0.5*HuSin(180.0f*t)), t+(0.5*HuSin(180.0f*t)), t+(0.5*HuSin(180.0f*t)));
        }
        HuPrcVSleep();
    }
    for(i=0; i<2; i++) {
        if(motId[i][1] != -1) {
            mbPlayerMotionSet(playerNo[i], motId[i][1], 0);
            mbPlayerMotionSpeedSet(playerNo[i], 0.75f);
            mbPlayerScaleSet(playerNo[i], 1.0f, 1.0f, 1.0f);
        }
        omVibrate(playerNo[i], 20, 7, 3);
    }
    for(i=0; i<30.0f; i++) {
        t = i/30.0f;
        for(j=0; j<2; j++) {
            mbMasuPosGet(masuId[j], &masuPos[j]);
            mbObjPosSetV(objId[j], &masuPos[j]);
            mbPlayerPosSet(playerNo[j], masuPos[j^1].x, 100.0f+masuPos[j^1].y, masuPos[j^1].z);
            mbPlayerRotSet(playerNo[j], 0.0f, 720.0f*-t, 0.0f);
            scale = 0.5f+(0.5f*t);
        }
        HuPrcVSleep();
    }
    for(j=0; j<2; j++) {
        mbPlayerMotionShiftSet(playerNo[j], 1, 0.0f, 8.0f, 0x40000001);
    }
    for(i=0; i<30.0f; i++) {
        t = i/30.0f;
        for(j=0; j<2; j++) {
            mbMasuPosGet(masuId[j], &masuPos[j]);
            mbObjPosSetV(objId[j], &masuPos[j]);
            mbObjScaleSet(objId[j], 1.0f, 1.0f-t, 1.0f);
            mbPlayerPosSet(playerNo[j], masuPos[j^1].x, (100.0f*(1.0f-t))+masuPos[j^1].y, masuPos[j^1].z);
            mbPlayerRotSet(playerNo[j], 0.0f, 0.0f, 0.0f);
        }
        HuPrcVSleep();
    }
    for(j=0; j<2; j++) {
        mbPlayerColSnapPlayerSet(playerNo[j], TRUE);
    }
    mbPlayerColFirstSet(playerNo[0]);
    for(j=0; j<2; j++) {
        GwPlayer[playerNo[j]].masuIdNext = masuId[j^1];
    }
    mbObjDispSet(objId[0], FALSE);
    mbObjDispSet(objId[1], FALSE);
    HuPrcEnd();
}

void mbev_CapDokanKill(void)
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
