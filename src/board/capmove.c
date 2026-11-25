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
    int useResult;
    int rouletteF;
    int winId;
    int masuNum;

    work = HuPrcCurrentGet()->property;
    useResult = mbCapEffUseCreate(work->playerNo, work->capNo);
    omVibrate(work->playerNo, 20, 4, 4);
    HuPrcSleep(20);
    mbev_CapRandomBonusCoin(work->playerNo, work->capNo, 1);
    while(mbCapEffUseModeGet(work->playerNo) >= 0) {
        HuPrcVSleep();
    }
    mbWinCreate(2, 0x350001, -1);
    mbWinTopWait();
    if((rouletteF = mbRouletteCreate(work->playerNo, 3)) != FALSE) {
        mbRouletteWait();
    }
    otherNo = mbRouletteResultGet();
    playerNo[0] = work->playerNo;
    playerNo[1] = otherNo;
    masuId[0] = GwPlayer[playerNo[0]].masuId;
    masuId[1] = GwPlayer[playerNo[1]].masuId;
    if(masuId[0] == masuId[1]) {
        winId = mbWinCreate(2, 0x350000, -1);
        mbWinTopWait();
        HuPrcEnd();
    }
    if((masuNum = mbev_CapPlayerMasuNumGet(masuId[0])) > 1) {
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
#include "game/board/math.h"
#include "game/board/coin.h"
#include "game/board/dice.h"
#include "game/audio.h"

/* Part 2 local declarations (capevent.h prototypes as retail calls them here; see report). */
void *mbev_CapEffExhaustCreate(void);
void *mbev_CapEffBoostCreate(void);
void mbev_CapEffBoostBlendModeSet(void *eff, u8 mode);
void mbev_CapEffGlowPatSet(void *eff, u8 pat);
void mbev_CapPlayerMoveObjInit(void);
void mbev_CapPlayerMoveObjKill(void);
int mbev_CapPlayerMoveObjCheck(int playerNo);
void mbev_CapPlayerMoveIdleCreate(int playerNo, int time);
void mbev_CapPlayerMoveHitCreate(int playerNo, int arg1, int arg2);
void mbev_CapPlayerIdleWait(void);
s16 mbev_CapMasuLinkNextGet(s16 masuId, HuVecF *pos);
s16 mbev_CapMasuLinkNextRandomGet(s16 masuId, HuVecF *pos);
void mbev_CapStatusDispSetAll(int arg0, int arg1);
void mbev_CapHermiteGetV(float t, HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, HuVecF *out);
float mbev_CapAngleSumLerp(float t, float start, float end);
int mbev_CapCoinDisp(int playerNo, int coinNum, int arg2, int arg3);
void mbev_CapEffDustCloudAdd(void *eff, HuVecF pos);
void mbev_CapEffColorSet(GXColor *color, int no);
int mbev_CapEffGlowAdd(void *eff, HuVecF pos, HuVecF vel, int time, float scale, float speed, float decay, GXColor color);
int mbev_CapEffGlowKinokoAdd(void *eff, HuVecF pos, int time, float speed, float scaleX, float scaleY, float scaleZ, int arg3, GXColor *color);
int mbev_CapEffGlowKinokoTimeSet(void *eff, int no, s16 arg2, s16 time);
int mbev_CapEffExplodeAdd(void *eff, HuVecF pos, HuVecF vel, float f0, float f1, float f2, GXColor color);
int mbev_CapEffExplodeKillerAdd(void *eff, HuVecF pos, HuVecF vel, float f0, float f1, float f2, float f3, GXColor color);
int mbev_CapEffBoostAdd(void *eff, HuVecF pos, HuVecF vel, float f0, float f1, int time, GXColor color);

#define CAPMOVE_RANDF() ((1.0f/268435456.0f)*mbRandMod(0x10000000))

static HuVecF killerOfsTbl[9] = {
    { 0.0f, 0.0f, 0.0f },
    { 80.0f, 0.0f, 80.0f },
    { -80.0f, 0.0f, 80.0f },
    { 80.0f, 0.0f, -80.0f },
    { -80.0f, 0.0f, -80.0f },
    { 100.0f, 0.0f, 0.0f },
    { -100.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 100.0f },
    { 0.0f, 0.0f, -100.0f },
};

static void ev_CapEffKillerDustCreate(CAPEVENTWORK *work, HuVecF *pos, HuVecF *rot);
static void ev_CapEffKillerExplodeCreate(CAPEVENTWORK *work, HuVecF *pos, HuVecF *rot, int num);
static void ev_CapEffKillerBoostCreate(CAPEVENTWORK *work, HuVecF *pos, HuVecF *rot);
static void ev_CapEffKinokoOMExec(OMOBJ *obj);

void mbev_CapHanachanKill(void)
{
}

static void ev_CapHanachanOMExec(OMOBJ *obj)
{
    CAPEVENTWORK *work = obj->data;
    Mtx rotMtx;
    HuVecF pos;
    HuVecF rot;
    HuVecF ofs;
    HuVecF vel;
    GXColor color;
    float speed;
    float scale;
    int no;

    if(mbExitCheck()) {
        omDelObjEx(mbObjMan, obj);
        return;
    }
    switch(obj->work[0]) {
        case 0:
            if(++obj->work[1] & 1) {
                mbObjPosGet(work->objId, &pos);
                mbObjRotGet(work->objId, &rot);
                pos.y += 50.0f;
                ofs.x = 100.0f*(1.5f*(-0.5f+CAPMOVE_RANDF()));
                ofs.y = 100.0f*(-0.5f+CAPMOVE_RANDF());
                ofs.z = -100.0f*(1.0f+(0.5f*CAPMOVE_RANDF()));
                mtxRot(rotMtx, rot.x, rot.y, rot.z);
                PSMTXMultVec(rotMtx, &ofs, &ofs);
                PSVECAdd(&pos, &ofs, &pos);
                vel.x = 0.0f;
                vel.y = 0.0f;
                vel.z = 0.0f;
                mbev_CapEffColorSet(&color, mbRandMod(0x8000));
                no = mbev_CapEffGlowAdd(work->effGlow, pos, vel, 60.0f*(1.0f+(0.5f*CAPMOVE_RANDF())),
                    100.0f*(0.15f+(0.05f*CAPMOVE_RANDF())), 0.05f+(0.02f*CAPMOVE_RANDF()), 0.08166666f, color);
                mbev_CapEffGlowKinokoTimeSet(work->effGlow, no, 1, 90);
            }
            break;

        default:
            omDelObjEx(mbObjMan, obj);
            return;
    }
}

void mbev_CapNKinoko(void)
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
    mbev_CapEffGlowPatSet(work->effGlow, 0);
    mbPlayerPosGet(work->playerNo, &pos);
    pos.y += 250.0f;
    objId = mbev_CapObjCreate(work->objList, 0xC0046, NULL, 0, 0, 0);
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
    mbAudFXPlay(0x411);
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
    GwPlayer[work->playerNo].diceMode = 6;
    omVibrate(work->playerNo, 20, 4, 4);
    for(i=0; i<6.0f; i++) {
        HuPrcVSleep();
    }
    while(mbev_CapEffGlowDispGet(work->effGlow) > 0) {
        HuPrcVSleep();
    }
    HuPrcEnd();
}

void mbev_CapNKinokoKill(void)
{
}

void mbev_CapKillerMove(void)
{
    CAPEVENTWORK *work = HuPrcCurrentGet()->property;
    Mtx rotMtx;
    HuVecF pts[4];
    HuVecF playerPos;
    HuVecF playerRot;
    HuVecF dir;
    HuVecF endPos;
    HuVecF objPos;
    HuVecF objRot;
    HuVecF objScale;
    HuVecF ptCopy;
    HuVecF rotCopy;
    HuVecF movePos;
    HuVecF startPos;
    HuVecF hermStart;
    HuVecF hermEnd;
    HuVecF rotStart;
    HuVecF rotEnd;
    HuVecF tmp;
    HuVecF rot2;
    HuVecF boostPos;
    int masuTbl[4];
    int coinDispId[4];
    int hitF[4];
    int motId[2];
    int masuType;
    int masuAttr;
    s32 statId;
    int playerNo;
    int masuId;
    int coinTotal;
    int i;
    int j;
    int objId;
    int diceNum;
    int vibTime;
    int coin;
    int masuNext;
    int time;
    int boostTime;
    float t;
    float s;
    float scale;

    statId = mbBGRead(0xD0000);
    if(statId != -1) {
        mbBGReadWait(statId);
    }
    work->effExplode = mbev_CapEffExhaustCreate();
    work->effBD4 = mbev_CapEffBoostCreate();
    mbev_CapEffBoostBlendModeSet(work->effBD4, 1);
    mbev_CapPlayerMoveObjInit();
    playerNo = work->playerNo;
    mbPlayerPosGet(playerNo, &playerPos);
    mbPlayerRotGet(playerNo, &playerRot);
    masuId = GwPlayer[playerNo].masuId;
    coinTotal = 0;
    for(i=0; i<4; i++) {
        hitF[i] = 0;
    }
    objId = mbev_CapObjCreate(work->objList, 0xD0000, NULL, 0, 5, 0);
    mbObjDispSet(objId, FALSE);
    motId[0] = mbev_CapPlayerMotionCreate(work->objList, playerNo, 0x930043);
    motId[1] = mbev_CapPlayerMotionCreate(work->objList, playerNo, 0x930042);
    for(i=0; i<4; i++) {
        masuTbl[i] = GwPlayer[playerNo].masuId;
        mbMasuPosGet(masuTbl[i], &pts[i]);
        pts[i].y += 120.00001f;
    }
    masuTbl[2] = mbev_CapMasuLinkNextRandomGet(masuTbl[1], &pts[2]);
    pts[2].y += 120.00001f;
    masuTbl[3] = mbev_CapMasuLinkNextRandomGet(masuTbl[2], &pts[3]);
    pts[3].y += 120.00001f;
    diceNum = mbDiceResultGet(playerNo);
    mbev_CapStatusDispSetAll(1, 0);
    mbPlayerPosGet(playerNo, &startPos);
    mbPlayerRotGet(playerNo, &rotStart);
    mbev_CapHermiteGetV(0.0f, &pts[0], &pts[1], &pts[2], &pts[3], &hermStart);
    objPos = hermStart;
    mbev_CapHermiteGetV(1.0f, &pts[0], &pts[1], &pts[2], &pts[3], &hermEnd);
    PSVECSubtract(&hermEnd, &hermStart, &dir);
    if(PSVECMag(&dir) > 0.0f) {
        PSVECNormalize(&dir, &dir);
    }
    objRot.x = HuAtan(-dir.y, HuMagXZVecF(&dir));
    objRot.y = HuAtan(dir.x, dir.z);
    objRot.z = 0.0f;
    rotEnd = objRot;
    mtxRot(rotMtx, -objRot.x, objRot.y, objRot.z);
    tmp.x = 0.0f;
    tmp.y = 60.000004f;
    tmp.z = -75.0f;
    PSMTXMultVec(rotMtx, &tmp, &tmp);
    PSVECAdd(&objPos, &tmp, &hermStart);
    mbPlayerColSnapPlayerSet(playerNo, FALSE);
    mbPlayerMotionShiftSet(playerNo, 4, 0.0f, 8.0f, 0);
    for(i=0; i<30.0f; i++) {
        float len;
        t = i/30.0f;
        PSVECSubtract(&hermStart, &startPos, &tmp);
        len = PSVECMag(&tmp);
        if(len > 0.0f) {
            PSVECNormalize(&tmp, &tmp);
        }
        PSVECScale(&tmp, &tmp, t*len);
        PSVECAdd(&startPos, &tmp, &playerPos);
        playerPos.y += 200.0*HuSin(180.0f*t);
        playerRot.x = mbev_CapAngleSumLerp(t, rotStart.x, rotEnd.x);
        playerRot.y = mbev_CapAngleSumLerp(t, rotStart.y, rotEnd.y);
        playerRot.z = mbev_CapAngleSumLerp(t, rotStart.z, rotEnd.z);
        mbPlayerPosSetV(playerNo, &playerPos);
        mbPlayerRotSetV(playerNo, &playerRot);
        if(i == 15) {
            mtxRot(rotMtx, objRot.x, objRot.y, objRot.z);
            tmp.x = 0.0f;
            tmp.y = 0.0f;
            tmp.z = -75.0f;
            PSMTXMultVec(rotMtx, &tmp, &tmp);
            PSVECAdd(&objPos, &tmp, &tmp);
            tmp.z += 50.0f;
            mbev_CapEffDustCloudAdd(work->effExplode, tmp);
            mbAudFXPlay(0x419);
            mbObjPosSetV(objId, &objPos);
            mbObjRotSetV(objId, &objRot);
            mbObjScaleSet(objId, 0.0f, 0.0f, 0.0f);
            mbObjDispSet(objId, TRUE);
            for(j=0; j<4; j++) {
                if(j == playerNo || masuId != GwPlayer[j].masuId) {
                    continue;
                }
                mbev_CapPlayerMoveIdleCreate(j, 12);
                mbAudFXPlay(0x423);
                CharFXPlay(GwPlayer[j].charNo, 0x246);
                omVibrate(j, 20, 7, 3);
                omVibrate(playerNo, 20, 7, 3);
                vibTime = 1;
                hitF[j] = 1;
                coin = 20;
                if(mbPlayerCoinGet(j) < coin) {
                    coin = mbPlayerCoinGet(j);
                }
                if(coin > 0) {
                    mbPlayerPosGet(j, &tmp);
                    tmp.y += 250.0f;
                    coinDispId[j] = mbCoinDispCapsuleCreate(&tmp, -coin);
                    mbCoinAddDispExec(j, -coin, FALSE, TRUE);
                    mbCoinAddDispExec(playerNo, coin, FALSE, TRUE);
                    coinTotal += coin;
                }
            }
            mbPlayerMotionShiftSet(playerNo, motId[0], 0.0f, 8.0f, 0);
        } else if(i > 15) {
            s = (i-15.0f)/10.0f;
            if(s > 1.0f) {
                s = 1.0f;
            }
            scale = s+(0.25*HuSin(180.0f*s));
            mbObjScaleSet(objId, scale, scale, scale);
        }
        HuPrcVSleep();
    }
    for(i=0; i<45.0f; i++) {
        t = i/45.0f;
        mbObjPosSetV(objId, &objPos);
        mbObjRotSet(objId, objRot.x, objRot.y+(30.0*HuSin(540.0f*t)), objRot.z);
        mbObjRotGet(objId, &tmp);
        mtxRot(rotMtx, tmp.x, tmp.y, tmp.z);
        tmp.x = 0.0f;
        tmp.y = 60.000004f;
        tmp.z = -75.0f;
        PSMTXMultVec(rotMtx, &tmp, &tmp);
        PSVECAdd(&objPos, &tmp, &playerPos);
        mbObjRotGet(objId, &playerRot);
        mbPlayerPosSetV(playerNo, &playerPos);
        mbPlayerRotSetV(playerNo, &playerRot);
        for(j=0; j<3; j++) {
            tmp = objPos;
            rot2 = objRot;
            rot2.y += 30.0*HuSin(360.0f*t);
            ev_CapEffKillerDustCreate(work, &tmp, &rot2);
        }
        HuPrcVSleep();
    }
    mbObjRotSet(objId, -objRot.x, objRot.y, objRot.z);
    mbObjScaleSet(objId, 1.0f, 1.0f, 1.0f);
    for(i=0; i<30.0f; i++) {
        t = i/60.0f;
        mbObjPosSetV(objId, &objPos);
        mbObjRotSet(objId, objRot.x+(30.0*HuSin(180.0f*t)), objRot.y, objRot.z);
        scale = -(0.2f*HuSin(180.0f*t));
        s = 0.2f*HuSin(180.0f*t);
        mbObjScaleSet(objId, 1.0f+s, 1.0f+s, 1.0f+scale);
        mbObjRotGet(objId, &tmp);
        mtxRot(rotMtx, tmp.x, tmp.y, tmp.z);
        tmp.x = 0.0f;
        tmp.y = 60.000004f;
        tmp.z = -75.0f;
        PSMTXMultVec(rotMtx, &tmp, &tmp);
        PSVECAdd(&objPos, &tmp, &playerPos);
        mbObjRotGet(objId, &playerRot);
        mbPlayerPosSetV(playerNo, &playerPos);
        mbPlayerRotSetV(playerNo, &playerRot);
        HuPrcVSleep();
    }
    rot2 = objRot;
    mtxRot(rotMtx, rot2.x, rot2.y, rot2.z);
    tmp.x = 0.0f;
    tmp.y = 0.0f;
    tmp.z = -120.00001f;
    PSMTXMultVec(rotMtx, &tmp, &tmp);
    PSVECAdd(&objPos, &tmp, &tmp);
    ev_CapEffKillerExplodeCreate(work, &tmp, &rot2, 32);
    boostPos = tmp;
    for(i=0; i<32; i++) {
        ev_CapEffKillerBoostCreate(work, &boostPos, &objRot);
    }
    boostTime = 12;
    mbObjRotGet(objId, &tmp);
    objRot.x = mbev_CapAngleSumLerp(0.5f, objRot.x, tmp.x);
    objRot.y = mbev_CapAngleSumLerp(0.5f, objRot.y, tmp.y);
    objRot.z = mbev_CapAngleSumLerp(0.5f, objRot.z, tmp.z);
    mbObjScaleGet(objId, &objScale);
    mbAudFXPlay(0x422);
    vibTime = 0;
    do {
        float len2;
        masuId = masuTbl[1];
        masuNext = masuTbl[2];
        if(++vibTime%20 == 1) {
            omVibrate(playerNo, 20, 4, 4);
        }
        if(masuNext <= 0 || mbev_CapMasuLinkNextGet(masuNext, NULL) <= 0) {
            break;
        }
        startPos = pts[1];
        hermStart = pts[2];
        ptCopy = pts[0];
        rotCopy = objRot;
        PSVECSubtract(&hermStart, &startPos, &dir);
        len2 = PSVECMag(&dir);
        if(len2 != 0.0f) {
            PSVECNormalize(&dir, &dir);
        }
        rotStart = objRot;
        rotEnd.x = HuAtan(-dir.y, HuMagXZVecF(&dir));
        rotEnd.y = HuAtan(dir.x, dir.z);
        rotEnd.z = 0.0f;
        time = len2/20.0f;
        for(i=1; i<=time; i++) {
            t = (float)i/(float)time;
            mbev_CapHermiteGetV(t, &pts[0], &pts[1], &pts[2], &pts[3], &objPos);
            j = time;
            if(j > 20) {
                j = 20;
            }
            s = (float)i/(float)j;
            if(s > 1.0f) {
                s = 1.0f;
            }
            tmp.x = mbev_CapAngleSumLerp(s, rotStart.x, rotEnd.x);
            tmp.y = mbev_CapAngleSumLerp(s, rotStart.y, rotEnd.y);
            tmp.z = 0.0f;
            objRot = tmp;
            if(objScale.x > 1.0f) {
                objScale.x -= 0.01f;
            } else {
                objScale.x = 1.0f;
            }
            if(objScale.y > 1.0f) {
                objScale.y -= 0.01f;
            } else {
                objScale.y = 1.0f;
            }
            if(objScale.z < 1.0f) {
                objScale.z += 0.01f;
            } else {
                objScale.z = 1.0f;
            }
            mbObjPosSetV(objId, &objPos);
            mbObjRotSetV(objId, &objRot);
            mbObjScaleSetV(objId, &objScale);
            mbObjRotGet(objId, &tmp);
            mtxRot(rotMtx, tmp.x, tmp.y, tmp.z);
            tmp.x = 0.0f;
            tmp.y = 60.000004f;
            tmp.z = -75.0f;
            PSMTXMultVec(rotMtx, &tmp, &tmp);
            PSVECAdd(&objPos, &tmp, &playerPos);
            mbObjRotGet(objId, &playerRot);
            mbPlayerPosSetV(playerNo, &playerPos);
            mbPlayerRotSetV(playerNo, &playerRot);
            if(i & 1) {
                ev_CapEffKillerDustCreate(work, &objPos, &objRot);
            }
            if(--boostTime > 0) {
                rot2 = objRot;
                mtxRot(rotMtx, rot2.x, rot2.y, rot2.z);
                tmp.x = 0.0f;
                tmp.y = 0.0f;
                tmp.z = -120.00001f;
                PSMTXMultVec(rotMtx, &tmp, &tmp);
                PSVECAdd(&objPos, &tmp, &tmp);
                for(j=0; j<16; j++) {
                    ev_CapEffKillerBoostCreate(work, &tmp, &objRot);
                }
            }
            HuPrcVSleep();
        }
        if(mbMasuTypeGet(masuNext) != 0) {
            HuAudFXPlay(0x3EE);
        }
        for(j=0; j<4; j++) {
            if(j == playerNo || hitF[j] == 1 || masuNext != GwPlayer[j].masuId) {
                continue;
            }
            mbAudFXPlay(0x423);
            mbev_CapPlayerMoveHitCreate(j, 1, 1);
            CharFXPlay(GwPlayer[j].charNo, 0x246);
            omVibrate(j, 20, 7, 3);
            omVibrate(playerNo, 20, 7, 3);
            vibTime = 1;
            hitF[j] = 1;
            coin = 20;
            if(mbPlayerCoinGet(j) < coin) {
                coin = mbPlayerCoinGet(j);
            }
            if(coin > 0) {
                mbPlayerPosGet(j, &tmp);
                tmp.y += 250.0f;
                coinDispId[j] = mbCoinDispCapsuleCreate(&tmp, -coin);
                mbCoinAddDispExec(j, -coin, FALSE, TRUE);
                mbCoinAddDispExec(playerNo, coin, FALSE, TRUE);
                coinTotal += coin;
            }
        }
        GwPlayer[playerNo].masuIdPrev = masuId;
        GwPlayer[playerNo].masuId = masuNext;
        GwPlayer[playerNo].masuIdPrev = GwPlayer[playerNo].masuId;
        GwPlayer[playerNo].masuId = masuNext;
        masuType = mbMasuTypeGet(masuNext);
        masuAttr = mbMasuAttrGet(masuNext);
        if(mbMasuDispCheck(masuNext)) {
            GwPlayer[playerNo].moveNum--;
            if(GwPlayer[playerNo].moveNum < 0) {
                GwPlayer[playerNo].moveNum = 0;
            }
            diceNum--;
        }
        for(i=0; i<3; i++) {
            masuTbl[i] = masuTbl[i+1];
            pts[i] = pts[i+1];
        }
        masuTbl[3] = mbev_CapMasuLinkNextRandomGet(masuTbl[2], &pts[3]);
        pts[3].y += 120.00001f;
    } while(diceNum > 0);
    mbMoveNumKill(playerNo);
    mbObjDispSet(objId, FALSE);
    mbev_CapEffDustCloudAdd(work->effExplode, objPos);
    mbAudFXPlay(0x40F);
    mbPlayerMotionShiftSet(playerNo, 4, 0.0f, 8.0f, 0);
    mbPlayerPosGet(playerNo, &playerPos);
    movePos = playerPos;
    for(i=0; i<9; i++) {
        mbMasuPosGet(GwPlayer[playerNo].masuId, &endPos);
        PSVECAdd(&endPos, &killerOfsTbl[i], &endPos);
        for(j=0; j<4; j++) {
            if(j == playerNo) {
                continue;
            }
            mbPlayerPosGet(j, &tmp);
            PSVECSubtract(&endPos, &tmp, &tmp);
            if(PSVECMag(&tmp) < 95.0f) {
                break;
            }
        }
        if(j >= 4) {
            break;
        }
    }
    for(i=0; i<30.0f; i++) {
        t = i/30.0f;
        PSVECSubtract(&endPos, &movePos, &tmp);
        PSVECScale(&tmp, &tmp, t);
        PSVECAdd(&movePos, &tmp, &playerPos);
        playerPos.y += 2.0*(100.0*HuSin(180.0f*t));
        playerRot.x = mbev_CapAngleSumLerp(t, playerRot.x, 0.0f);
        playerRot.y = mbev_CapAngleSumLerp(t, playerRot.y, 0.0f);
        playerRot.z = mbev_CapAngleSumLerp(t, playerRot.z, 0.0f);
        mbPlayerPosSetV(playerNo, &playerPos);
        mbPlayerRotSetV(playerNo, &playerRot);
        HuPrcVSleep();
    }
    mbPlayerPosSetV(playerNo, &endPos);
    mbPlayerMotIdleSet(playerNo);
    mbPlayerColSnapPlayerSet(playerNo, TRUE);
    mbCameraMoveWait();
    for(i=0; i<60.0f; i++) {
        HuPrcVSleep();
    }
    if(coinTotal > 0) {
        mbev_CapCoinDisp(playerNo, coinTotal, 1, 1);
    }
    do {
        for(i=0; i<4; i++) {
            if(!mbev_CapPlayerMoveObjCheck(i)) {
                break;
            }
        }
        HuPrcVSleep();
    } while(i < 4);
    mbev_CapPlayerIdleWait();
    mbev_PlayerColMasuSet(playerNo, GwPlayer[playerNo].masuId, FALSE);
    mbMasuPosGet(GwPlayer[playerNo].masuId, &playerPos);
    mbPlayerPosGet(playerNo, &tmp);
    PSVECSubtract(&playerPos, &tmp, &tmp);
    if(PSVECMag(&tmp) > 25.0f) {
        mbMasuPosGet(GwPlayer[playerNo].masuId, &playerPos);
        mbPlayerMoveExec(playerNo, NULL, &playerPos, 30, NULL, TRUE);
        mbPlayerRotateStart(playerNo, 0, 15);
        while(!mbPlayerRotateCheck(playerNo)) {
            HuPrcVSleep();
        }
    }
    mbev_CapPlayerMoveObjKill();
    HuPrcEnd();
}

void mbev_CapKillerMoveKill(void)
{
}

static void ev_CapEffKillerDustCreate(CAPEVENTWORK *work, HuVecF *pos, HuVecF *rot)
{
    Mtx rotMtx;
    HuVecF effPos;
    HuVecF vel;
    GXColor color;
    float c;
    float speed;

    mtxRot(rotMtx, rot->x, rot->y, rot->z);
    effPos.x = 0.0f;
    effPos.y = 0.0f;
    effPos.z = -180.0f;
    PSMTXMultVec(rotMtx, &effPos, &effPos);
    PSVECAdd(pos, &effPos, &effPos);
    speed = 10.0f*(0.5f+(0.5f*CAPMOVE_RANDF()));
    vel.x = speed*HuSin(180.0f+rot->y)*HuCos(rot->x);
    vel.y = speed*HuSin(rot->x);
    vel.z = speed*HuCos(180.0f+rot->y)*HuCos(rot->x);
    c = CAPMOVE_RANDF();
    color.r = 32.0f+(32.0f*c);
    color.g = 32.0f+(32.0f*c);
    color.b = 32.0f+(32.0f*c);
    color.a = 192.0f+(63.0f*CAPMOVE_RANDF());
    mbev_CapEffExplodeKillerAdd(work->effExplode, effPos, vel,
        100.0f*(1.0f+(0.5f*CAPMOVE_RANDF())),
        -0.5f+CAPMOVE_RANDF(),
        100.0f*(0.25f+(0.25f*CAPMOVE_RANDF())),
        0.5f+(0.25f*CAPMOVE_RANDF()), color);
}

static void ev_CapEffKillerExplodeCreate(CAPEVENTWORK *work, HuVecF *pos, HuVecF *rot, int num)
{
    Mtx rotMtx;
    HuVecF effPos;
    HuVecF vel;
    GXColor color;
    int i;
    int angle;
    int c;

    mtxRot(rotMtx, rot->x, rot->y, rot->z);
    for(i=0; i<num; i++) {
        effPos.x = pos->x;
        effPos.y = pos->y;
        effPos.z = pos->z;
        angle = (360.0f/num)*i;
        vel.x = (0.075f*(100.0*HuSin(angle)))*(0.9f+(0.1f*CAPMOVE_RANDF()));
        vel.y = (0.075f*(100.0*HuCos(angle)))*(0.9f+(0.1f*CAPMOVE_RANDF()));
        vel.z = 0.0f;
        PSMTXMultVec(rotMtx, &vel, &vel);
        c = 63.0f*CAPMOVE_RANDF();
        color.r = c+32;
        color.g = c+32;
        color.b = c+32;
        color.a = 192.0f+(63.0f*CAPMOVE_RANDF());
        mbev_CapEffExplodeAdd(work->effExplode, effPos, vel, 200.0f, 0.1f, 0.33f, color);
    }
}

static void ev_CapEffKillerBoostCreate(CAPEVENTWORK *work, HuVecF *pos, HuVecF *rot)
{
    HuVecF effPos;
    HuVecF vel;
    HuVecF effRot;
    GXColor color;
    float speed;

    effPos.x = pos->x+(50.0f*(-0.5f+CAPMOVE_RANDF()));
    effPos.y = pos->y+(50.0f*(-0.5f+CAPMOVE_RANDF()));
    effPos.z = pos->z+(50.0f*(-0.5f+CAPMOVE_RANDF()));
    speed = 30.000002f*(1.0f+(0.5f*CAPMOVE_RANDF()));
    effRot.x = rot->x+(50.0f*(-0.5f+CAPMOVE_RANDF()));
    effRot.y = rot->y+(50.0f*(-0.5f+CAPMOVE_RANDF()));
    effRot.z = rot->z;
    vel.x = speed*HuSin(180.0f+effRot.y)*HuCos(effRot.x);
    vel.y = speed*HuSin(effRot.x);
    vel.z = speed*HuCos(180.0f+effRot.y)*HuCos(effRot.x);
    color.r = 64.0f+(192.0f*CAPMOVE_RANDF());
    color.g = 64.0f+(192.0f*CAPMOVE_RANDF());
    color.b = 64.0f+(192.0f*CAPMOVE_RANDF());
    color.a = 128.0f+(64.0f*CAPMOVE_RANDF());
    mbev_CapEffBoostAdd(work->effBD4, effPos, vel,
        50.0f*(1.0f+(0.5f*CAPMOVE_RANDF())),
        2.0f*(-0.5f+CAPMOVE_RANDF()),
        60.0f*(1.0f+(0.5f*CAPMOVE_RANDF())),
        color);
}

static void ev_CapEffKinokoCreate(CAPEVENTWORK *work)
{
    OMOBJ *obj = omAddObjEx(mbObjMan, 32768, 0, 0, -1, ev_CapEffKinokoOMExec);
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(CAPEVENTWORK), HU_MEMNUM_OVL);
    memcpy(obj->data, work, sizeof(CAPEVENTWORK));
}

static void ev_CapEffKinokoOMExec(OMOBJ *obj)
{
    CAPEVENTWORK *work;
    HuVecF pos;
    HuVecF scale;
    GXColor color;
    float s;
    float r;
    int objId;
    int capNo;
    int no;

    if(mbExitCheck() || obj->work[0]) {
        omDelObjEx(mbObjMan, obj);
        return;
    }
    work = obj->data;
    objId = work->objId;
    capNo = work->capNo;
    mbObjPosGet(objId, &pos);
    mbObjScaleGet(objId, &scale);
    s = scale.x;
    if(s > 1.0f) {
        s = 1.0f;
    }
    if(s <= 0.2f) {
        obj->work[0] = 1;
    }
    switch(capNo) {
        case 0:
            r = CAPMOVE_RANDF();
            color.r = 192.0f+(63.0f*r);
            color.g = 192.0f+(63.0f*r);
            color.b = 192.0f+(63.0f*r);
            color.a = 192.0f+(63.0f*CAPMOVE_RANDF());
            break;

        case 1:
            r = CAPMOVE_RANDF();
            color.r = 64.0f+(63.0f*r);
            color.g = 192.0f+(63.0f*r);
            color.b = 192.0f+(63.0f*r);
            color.a = 192.0f+(63.0f*CAPMOVE_RANDF());
            break;

        case 2:
            mbev_CapEffColorSet(&color, mbRandMod(0x8000));
            color.a = 192.0f+(63.0f*CAPMOVE_RANDF());
            break;

        case 3:
            r = CAPMOVE_RANDF();
            color.r = 128.0f+(63.0f*r);
            color.g = 128.0f+(63.0f*r);
            color.b = 128.0f+(63.0f*r);
            color.a = 192.0f+(63.0f*CAPMOVE_RANDF());
            break;

        case 7:
            r = CAPMOVE_RANDF();
            color.r = 128.0f+(32.0f*r);
            color.g = 32.0f+(32.0f*r);
            color.b = 128.0f+(32.0f*r);
            color.a = 128.0f+(63.0f*CAPMOVE_RANDF());
            break;
    }
    no = mbev_CapEffGlowKinokoAdd(work->effGlow, pos, 60.0f*(0.6f+(3.0f*(-0.5f+CAPMOVE_RANDF()))),
        (100.0f*s)*(0.2f+(0.1f*CAPMOVE_RANDF())), 150.0f*s, 150.0f*s, 50.0f*s, 2, &color);
    if(no >= 0) {
        mbev_CapEffGlowKinokoTimeSet(work->effGlow, no, 1, 80.0f+(20.0f*(-0.5f+CAPMOVE_RANDF())));
    }
}

/* @@END@@ */
