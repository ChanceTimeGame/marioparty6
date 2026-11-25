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
void mbev_CapBezierGetV(float t, HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *out);
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

void mbev_CapHanachan(void)
{
    Mtx hookMtx;
    HuVecF pts[5];
    int motData[8];
    HuVecF playerPos;
    HuVecF masuPos;
    HuVecF objPos;
    HuVecF bez2;
    HuVecF bez1;
    HuVecF bez0;
    HuVecF playerPosStart;
    HuVecF hookPos;
    HuVecF pos;
    HuVecF effPos;
    int motId[2];
    int masuIdCur;
    CAPEVENTWORK *work;
    OMOBJ *obj;
    float t;
    float angle;
    float ofsZ;
    float radius;
    float ofsY;
    float ofsX;
    float s;
    float speed;
    float angleStart;
    int i;
    int hanaObj;
    int masuId;
    int kinokoObj;
    int seNo;
    int missNo;
    int missF;
    HuVecF *effPosP;

    work = HuPrcCurrentGet()->property;
    work->effExplode = mbev_CapEffExplodeCreate();
    HuPrcVSleep();
    work->effGlow = mbev_CapEffGlowCreate();
    HuPrcVSleep();
    work->effRing = mbev_CapEffRingCreate();
    HuPrcVSleep();
    motId[0] = mbev_CapPlayerMotionCreate(work->objList, work->playerNo, 0x930022);
    HuPrcVSleep();
    motId[1] = mbev_CapPlayerMotionCreate(work->objList, work->playerNo, 0x93003F);
    HuPrcVSleep();
    mbev_CapWait(work);
    motData[0] = 0xD0002;
    motData[1] = 0xD0003;
    motData[2] = -1;
    hanaObj = mbev_CapObjCreate(work->objList, 0xD0001, motData, 0, 5, 0);
    mbObjLayerSet(hanaObj, 3);
    mbObjMotionSet(hanaObj, 1, 0x40000001);
    mbObjDispSet(hanaObj, FALSE);
    kinokoObj = mbev_CapObjCreate(work->objList, 0xC0044, NULL, 0, 5, 0);
    mbObjDispSet(kinokoObj, FALSE);
    HuPrcVSleep();
    mbPlayerPosGet(work->playerNo, &playerPos);
    playerPosStart = playerPos;
    masuIdCur = GwPlayer[work->playerNo].masuId;
    for(i=1; i<=mbMasuRawNumGet(); i++) {
        if(mbMasuTypeGet(i) == 7) {
            masuId = i;
            break;
        }
    }
    if(i > mbMasuRawNumGet()) {
        masuId = GwPlayer[work->playerNo].masuId;
    }
    mbMasuPosGet(masuId, &masuPos);
    mbObjPosSetV(kinokoObj, &playerPos);
    mbCameraMoveObj(kinokoObj, NULL, &capsuleCameraOfs, -1.0f, -1.0f, 21);
    mbCameraMoveWait();
    obj = omAddObjEx(mbObjMan, -32768, 0, 0, -1, ev_CapHanachanOMExec);
    obj->work[0] = 0;
    obj->work[1] = 0;
    work->objId = hanaObj;
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(CAPEVENTWORK), HU_MEMNUM_OVL);
    memcpy(obj->data, work, sizeof(CAPEVENTWORK));
    seNo = mbAudFXPlay(0x41C);
    objPos.x = playerPos.x;
    objPos.y = 250.0f+playerPos.y;
    objPos.z = playerPos.z-100.0f;
    pts[0] = playerPosStart;
    pts[0].y += 250.0f;
    pts[1] = pts[0];
    pts[1].y -= 200.0f;
    pts[1].z += 500.0f;
    pts[2] = pts[0];
    pts[2].y += 300.0f;
    pts[3] = pts[0];
    pts[3].y += 200.0f;
    pts[3].z -= 300.0f;
    pts[4] = objPos;
    mbCapEffUseCreate(work->playerNo, work->capNo);
    omVibrate(work->playerNo, 20, 4, 4);
    while(mbCapEffUseModeGet(work->playerNo) < 2) {
        HuPrcVSleep();
    }
    mbObjDispSet(hanaObj, TRUE);
    for(i=0; i<=90.0f; i++) {
        t = i/90.0f;
        mbObjRotSet(hanaObj, 45.0f-(405.0f*t), 0.0f, 0.0f);
        if(t < 0.5f) {
            t = HuSin(90.0f*(2.0f*t));
            mbev_CapBezierGetV(t, &pts[0], &pts[1], &pts[2], &pos);
        } else {
            t = HuCos(90.0f-(90.0f*(2.0f*(t-0.5f))));
            mbev_CapBezierGetV(t, &pts[2], &pts[3], &pts[4], &pos);
        }
        mbObjPosSetV(hanaObj, &pos);
        HuPrcVSleep();
    }
    mbev_CapRandomBonusCoin(work->playerNo, work->capNo, 1);
    mbPlayerRotateStart(work->playerNo, 180, 15);
    while(!mbPlayerRotateCheck(work->playerNo)) {
        HuPrcVSleep();
    }
    HuPrcSleep(15);
    mbAudFXPlay(0x3BC);
    mbWinCreate(2, 0x350003, 1);
    mbWinTopWait();
    missNo = 0;
    if(missNo == 0) {
        missF = FALSE;
    } else {
        missF = TRUE;
    }
    if(missF) {
        mbAudFXPlay(0x40F);
        mbAudFXPlay(0x3F6);
        if(seNo != -1) {
            mbAudFXStop(seNo);
        }
        if(!mbExitCheck()) {
            obj->work[0]++;
        }
        effPos = objPos;
        effPosP = &effPos;
        mbev_CapEffDustExplodeAdd(work->effExplode, effPosP);
        mbObjDispSet(hanaObj, FALSE);
        do {
            HuPrcVSleep();
        } while(mbev_CapEffExplodeAnimGet(work->effExplode));
        mbPlayerRotateStart(work->playerNo, 0, 15);
        while(!mbPlayerRotateCheck(work->playerNo)) {
            HuPrcVSleep();
        }
        mbCameraPlayerViewSet(work->playerNo, 0);
        mbCameraMoveWait();
        mbMusBoardPlay();
    } else {
        mbev_CapBubbleHookCall(3, hanaObj, 1, 0, 0);
        mbWinCreate(2, 0x350004, 1);
        mbWinTopWait();
        bez0 = objPos;
        PSVECSubtract(&objPos, &playerPos, &pos);
        if(PSVECMag(&pos) > 0.0f) {
            PSVECNormalize(&pos, &pos);
        }
        bez2.x = playerPos.x+(50.0f*pos.x);
        bez2.y = 200.0f+playerPos.y;
        bez2.z = (playerPos.z+(50.0f*pos.z))-100.0f;
        bez1 = objPos;
        bez1.y -= 200.0f;
        bez1.z += 50.0f;
        ofsZ = hanachanPlayerOfs[GwPlayer[work->playerNo].charNo].z;
        ofsY = hanachanPlayerOfs[GwPlayer[work->playerNo].charNo].y;
        ofsX = hanachanPlayerOfs[GwPlayer[work->playerNo].charNo].x;
        angle = 0.0f;
        mbPlayerMotionShiftSet(work->playerNo, 4, 0.0f, 8.0f, 0);
        mbPlayerColSnapPlayerSet(work->playerNo, FALSE);
        for(i=0; i<=30.0f; i++) {
            t = i/30.0f;
            mbev_CapBezierGetV(t, &bez0, &bez1, &bez2, &objPos);
            mbObjPosSetV(hanaObj, &objPos);
            pos = playerPosStart;
            pos.y += 200.0*HuSin(90.0f*t);
            mbPlayerPosSetV(work->playerNo, &pos);
            mbPlayerRotSet(work->playerNo, 0.0f, 180.0f-(180.0f*t), 0.0f);
            HuPrcVSleep();
        }
        objPos = bez2;
        mbObjPosSetV(hanaObj, &objPos);
        mbObjMotionShiftSet(hanaObj, 2, 0.0f, 8.0f, 0x40000001);
        if(motId[1] != -1) {
            mbPlayerMotionShiftSet(work->playerNo, motId[1], 0.0f, 15.0f, 0x40000001);
        }
        omVibrate(work->playerNo, 20, 7, 3);
        mbPlayerPosGet(work->playerNo, &playerPos);
        for(i=0; i<=24.0f; i++) {
            t = i/24.0f;
            Hu3DModelObjMtxGet(mbObjModelIDGet(hanaObj), "itemhook_C", hookMtx);
            hookPos.x = hookMtx[0][3]+(ofsZ*HuSin(angle));
            hookPos.y = hookMtx[1][3]+ofsY;
            hookPos.z = hookMtx[2][3]+(ofsZ*HuCos(angle));
            pos.x = playerPos.x+(t*(hookPos.x-playerPos.x));
            pos.y = playerPos.y+(t*(hookPos.y-playerPos.y));
            pos.z = playerPos.z+(t*(hookPos.z-playerPos.z));
            pos.y -= 0.25*(100.0*HuSin(180.0f*t));
            mbPlayerPosSetV(work->playerNo, &pos);
            mbPlayerRotGet(work->playerNo, &pos);
            pos.x = t*ofsX;
            mbPlayerRotSetV(work->playerNo, &pos);
            pos = objPos;
            pos.y -= 0.25*(100.0*HuSin(180.0f*t));
            mbObjPosSetV(hanaObj, &pos);
            HuPrcVSleep();
        }
        omVibrate(work->playerNo, 180, 4, 4);
        bez2 = objPos;
        angleStart = angle;
        for(i=0; i<90.0f; i++) {
            t = i/90.0f;
            angle = angleStart+(180.0f*t);
            objPos.x = bez2.x+(300.0*(HuSin(angle)*HuSin(90.0f*t)));
            objPos.z = bez2.z+(300.0*(HuCos(angle)*HuSin(90.0f*t)));
            objPos.y = bez2.y+(550.0f*t);
            mbObjPosSet(hanaObj, objPos.x, objPos.y, objPos.z);
            mbObjRotSet(hanaObj, 0.0f, angle, 0.0f);
            Hu3DModelObjMtxGet(mbObjModelIDGet(hanaObj), "itemhook_C", hookMtx);
            hookMtx[1][3] += ofsY;
            hookMtx[0][3] += ofsZ*HuSin(angle);
            hookMtx[2][3] += ofsZ*HuCos(angle);
            pos.x = hookMtx[0][3];
            pos.y = hookMtx[1][3];
            pos.z = hookMtx[2][3];
            mbPlayerPosSetV(work->playerNo, &pos);
            mbPlayerRotSet(work->playerNo, ofsX, angle, 0.0f);
            HuPrcVSleep();
        }
        mbObjDispSet(hanaObj, FALSE);
        mbPlayerDispSet(work->playerNo, FALSE);
        mbWipeDissolveFadeOutTime(1);
        mbev_PlayerColMasuSet(work->playerNo, masuId, TRUE);
        GwPlayer[work->playerNo].masuId = masuId;
        objPos = masuPos;
        objPos.y += 750.0f;
        mbObjPosSet(hanaObj, objPos.x, objPos.y, objPos.z);
        mbObjRotSet(hanaObj, 0.0f, 0.0f, 0.0f);
        playerPos = masuPos;
        playerPos.y += 650.0f;
        mbCameraMoveMasu(masuId, NULL, &capsuleCameraOfs, -1.0f, -1.0f, -1);
        mbCameraMoveWait();
        mbWipeDissolveFadeIn();
        omVibrate(work->playerNo, 180, 4, 4);
        bez2 = masuPos;
        bez2.y += 100.0f;
        bez2.z -= 50.0f;
        mbObjDispSet(hanaObj, TRUE);
        mbPlayerDispSet(work->playerNo, TRUE);
        for(i=0; i<150.0f; i++) {
            t = i/150.0f;
            angle = 180.0f+(360.0f*t);
            radius = 200.0*HuSin(180.0f*t);
            objPos.x = bez2.x+(2.0*(radius*HuSin(angle)));
            objPos.z = bez2.z+(radius*HuCos(angle));
            objPos.y = bez2.y+(650.0f*(1.0f-t));
            mbObjPosSet(hanaObj, objPos.x, objPos.y, objPos.z);
            mbObjRotSet(hanaObj, 0.0f, 180.0f+angle, 0.0f);
            playerPos.x = objPos.x+(50.0*HuSin(180.0f+angle));
            playerPos.y = objPos.y-100.0f;
            playerPos.z = objPos.z+(50.0*HuCos(180.0f+angle));
            Hu3DModelObjMtxGet(mbObjModelIDGet(hanaObj), "itemhook_C", hookMtx);
            hookMtx[1][3] += ofsY;
            hookMtx[0][3] += ofsZ*HuSin(180.0f+angle);
            hookMtx[2][3] += ofsZ*HuCos(180.0f+angle);
            pos.x = hookMtx[0][3];
            pos.y = hookMtx[1][3];
            pos.z = hookMtx[2][3];
            mbPlayerPosSetV(work->playerNo, &pos);
            mbPlayerRotSet(work->playerNo, ofsX, 180.0f+angle, 0.0f);
            HuPrcVSleep();
        }
        mbPlayerMotionShiftSet(work->playerNo, 1, 0.0f, 8.0f, 0x40000001);
        mbMasuPosGet(masuId, &playerPos);
        for(i=0; i<8; i++) {
            t = 1.0f-(i/7.0f);
            Hu3DModelObjMtxGet(mbObjModelIDGet(hanaObj), "itemhook_C", hookMtx);
            hookMtx[1][3] += ofsY;
            hookMtx[0][3] += ofsZ*HuSin(180.0f+angle);
            hookMtx[2][3] += ofsZ*HuCos(180.0f+angle);
            pos.x = playerPos.x+(t*(hookMtx[0][3]-playerPos.x));
            pos.y = playerPos.y+(t*(hookMtx[1][3]-playerPos.y));
            pos.z = playerPos.z+(t*(hookMtx[2][3]-playerPos.z));
            mbPlayerPosSetV(work->playerNo, &pos);
            mbPlayerRotSet(work->playerNo, t*ofsX, 180.0f+angle, 0.0f);
            HuPrcVSleep();
        }
        mbPlayerPosSet(work->playerNo, playerPos.x, playerPos.y, playerPos.z);
        mbPlayerRotSet(work->playerNo, 0.0f, 180.0f+angle, 0.0f);
        mbPlayerColSnapPlayerSet(work->playerNo, TRUE);
        mbObjMotionShiftSet(hanaObj, 1, 0.0f, 8.0f, 0x40000001);
        for(i=0; i<30.0f; i++) {
            HuPrcVSleep();
        }
        mbPlayerMotionShiftSet(work->playerNo, 1, 0.0f, 8.0f, 0x40000001);
        mbev_CapBubbleHookCall(3, hanaObj, 1, 0, 0);
        mbWinCreate(2, 0x350005, 1);
        mbWinTopWait();
        mbPlayerPosGet(work->playerNo, &pos);
        bez2.x = pos.x;
        bez2.y = objPos.y;
        bez2.z = pos.z;
        radius = fabs(pos.z-objPos.z);
        for(i=0; i<120.0f; i++) {
            t = i/120.0f;
            speed = 650.0f;
            angle = 180.0f+(360.0f*t);
            if(t < 0.2f) {
                s = 5.0f*t;
            }
            if(s > 1.0f) {
                t = 1.0f;
            }
            objPos.x = bez2.x+((radius+(2.0f*(3.0f*(100.0f*t))))*HuSin(angle));
            objPos.z = bez2.z+((radius+(3.0f*(100.0f*t)))*HuCos(angle));
            objPos.y = bez2.y+(t*speed);
            mbObjPosSet(hanaObj, objPos.x, objPos.y, objPos.z);
            mbObjRotSet(hanaObj, 0.0f, (180.0f+angle)-(90.0f*s), 0.0f);
            HuPrcVSleep();
        }
        mbObjDispSet(hanaObj, FALSE);
        if(!mbExitCheck()) {
            obj->work[0]++;
        }
        if(seNo != -1) {
            mbAudFXStop(seNo);
        }
        mbPlayerRotateStart(work->playerNo, 0, 15);
        while(!mbPlayerRotateCheck(work->playerNo)) {
            HuPrcVSleep();
        }
        mbCameraMoveWait();
        mbev_CapPlayerMotShiftWait(work->playerNo, 1, 0x40000001, 1);
        mbCameraPlayerViewSet(work->playerNo, 0);
        mbCameraMoveWait();
        if(mbPlayerStarGet(work->playerNo) >= 999) {
            if(!GwSystem.curTime) {
                mbWinCreate(0, 0x27000A, mbGuideSpeakerNoGet());
            } else {
                mbWinCreate(0, 0x27000B, mbGuideSpeakerNoGet());
            }
            mbWinTopWait();
        } else {
            mbev_StarMasu(work->playerNo, GwPlayer[work->playerNo].masuId);
        }
    }
    HuPrcEnd();
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
