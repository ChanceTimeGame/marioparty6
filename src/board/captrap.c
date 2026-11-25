/* @@HEAD@@ */
#include "game/board/object.h"
#include "game/board/player.h"
#include "game/board/captrap.h"
#include "game/board/masu.h"
#include "game/board/camera.h"
#include "game/board/audio.h"
#include "game/board/coin.h"
#include "game/board/exit.h"
#include "game/board/math.h"
#include "game/gamework.h"
#include "game/memory.h"
#include "game/flag.h"
#include "game/charman.h"

#include "string.h"

/* The capsule event process work (HuPrcCurrentGet()->property; also the trap hooks' argument). */
typedef struct CapWork_s {
    /* 0x000 */ int playerNo;
    /* 0x004 */ u8 unk4[0xC];
    /* 0x010 */ int masuId;
    /* 0x014 */ u8 unk14[0xC];
    /* 0x020 */ u8 objWork[0xB48]; /* mbev_CapObjCreate's object list */
    /* 0xB68 */ u8 unkB68_7 : 7;
    /* 0xB68 */ u8 shockNoWaitF : 1;
    /* 0xB69 */ u8 unkB69[0x67];
    /* 0xBD0 */ void *explodeEff;
    /* 0xBD4 */ u8 unkBD4[8];
    /* 0xBDC */ void *glowEff;
    /* 0xBE0 */ void *ringHitEff;
} CAPWORK;

typedef struct CapBobleWork_s {
    /* 0x00 */ int playerNo;
    /* 0x04 */ int no;
    /* 0x08 */ int objId;
    /* 0x0C */ int mode;
    /* 0x10 */ int time;
    /* 0x14 */ float height;
    /* 0x18 */ float angle;
    /* 0x1C */ int metalF;
    /* 0x20 */ int endF;
    /* 0x24 */ HuVecF pos;
    /* 0x30 */ HuVecF startPos;
    /* 0x3C */ HuVecF endPos;
    /* 0x48 */ OMOBJ **objP;
} CAPBOBLEWORK; /* size 0x4C */

/* capevent.c; local prototypes, the B0 header's are placeholders (handles are pointers, colours
   and vectors go by value). */
extern u32 mbCapEffNum;
extern s16 *mbCapEffData;
int mbev_CapObjCreate(void *objWork, int dataNum, int *motDataNum, int linkF, int arg4, int arg5);
void *mbev_CapEffGlowFireCreate(void);
int mbev_CapEffGlowAdd(void *eff, HuVecF pos, HuVecF vel, int time, float scale, float speed, float arg6, GXColor color);
int mbev_CapEffGlowDispGet(void *eff);
void mbev_CapEffGlowKill(void *eff);
void *mbev_CapEffRingHitCreate(void);
int mbev_CapEffRingAdd(void *eff, HuVecF pos, HuVecF rot, HuVecF scale, int arg4, int time, int no, GXColor color);
int mbev_CapEffRingDispGet(void *eff);
void mbev_CapEffRingKill(void *eff);
void *mbev_CapEffElectricCreate(void);
int mbev_CapEffElectricAdd(void *eff, HuVecF pos, int arg2, int arg3);
void mbev_CapEffElectricModelSet(void *eff, int objId, int no, HuVecF *ofs);
void mbev_CapEffElectricKill(void *eff);
void *mbev_CapEffExplodeCreate(void);
int mbev_CapEffExplodeAnimGet(void *eff);
void mbev_CapEffDustHeavyAdd(void *eff, HuVecF pos);
void mbev_CapEffColorSet(GXColor *color, int no);
void mbev_CapBiriQShockCreate(int playerNo);
int mbev_CapBiriQShockDelayGet(int playerNo);
void mbev_CapBiriQMetalShockCreate(int playerNo);
void mbev_CapPlayerMotShiftWait(int playerNo, int motNo, int attr, int arg3);

static int ev_CapMasuNumGet(int playerNo);
static void ev_CapBobleOMExec(OMOBJ *obj);

#define CAP_RANDF() (0.0000000037252903f*mbRandMod(0x10000000))


static s32 bomheiMode[4];
static float bomheiRotY[4];

static inline float CapSinDeg(float deg)
{
    return mbSinDeg(deg);
}

static inline float CapCosDeg(float deg)
{
    return mbCosDeg(deg);
}

static inline void CapObjAttrSet(s16 objId, u32 attr)
{
    mbObjAttrSet(objId, attr);
}

static inline int CapMasuNextGet(int playerNo)
{
    return mbPlayerWorkGet(playerNo)->masuNext;
}

static inline s16 CapEffRand(void)
{
    if(++mbCapEffNum >= 1024) {
        mbCapEffNum = 0;
    }
    return mbCapEffData[mbCapEffNum];
}

static inline float CapEffRandF(void)
{
    return (1.0f/32767.0f)*CapEffRand();
}

static inline float CapEffRandHalfF(void)
{
    return CapEffRandF()-0.5f;
}

/* @@PART1@@ */
void mbev_CapBoble(void)
{
    CAPWORK *work = HuPrcCurrentGet()->property;
    OMOBJ *obj[3];
    HuVecF masuPos;
    HuVecF glowPos;
    HuVecF glowVel;
    HuVecF rot;
    int objId[3];
    GXColor color;
    CAPBOBLEWORK *objWork;
    OMOBJ *objP;
    float angleOfs;
    float angle;
    float speed;
    float t;
    float total;
    int i;
    int timer;
    int n;
    int masuNum;
    int masuNumPrev;

    work->glowEff = mbev_CapEffGlowFireCreate();
    HuPrcVSleep();
    mbMasuPosGet(work->masuId, &masuPos);
    angleOfs = 0.0f;
    for(i=0; i<3; i++) {
        objId[i] = mbev_CapObjCreate(work->objWork, 0xC003D, NULL, TRUE, 0, 0);
        mbObjLayerSet(objId[i], 3);
        CapObjAttrSet(objId[i], HU3D_MOTATTR_LOOP);
        mbObjDispSet(objId[i], FALSE);
        angle = angleOfs+(120.0f*i);
        objP = obj[i] = omAddObjEx(mbObjMan, 0x8000, 0, 0, -1, ev_CapBobleOMExec);
        objWork = objP->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(CAPBOBLEWORK), HU_MEMNUM_OVL);
        memset(objWork, 0, sizeof(CAPBOBLEWORK));
        objWork->playerNo = work->playerNo;
        objWork->no = i;
        objWork->objId = objId[i];
        objWork->mode = 0;
        objWork->time = 0;
        objWork->metalF = GwPlayer[work->playerNo].metalF;
        objWork->endF = FALSE;
        objWork->height = 100.0f*(2.0f+CAP_RANDF());
        objWork->angle = angle;
        objWork->pos.x = masuPos.x+(2.0*(100.0*HuSin(angle)));
        objWork->pos.y = 100.0f+masuPos.y;
        objWork->pos.z = masuPos.z+(2.0*(100.0*HuCos(angle)));
        objWork->startPos = objWork->pos;
        objWork->endPos.x = masuPos.x+(50.0f*(-0.5f+CAP_RANDF()));
        objWork->endPos.y = 100.0f+masuPos.y;
        objWork->endPos.z = masuPos.z+(50.0f*(-0.5f+CAP_RANDF()));
        objWork->objP = &obj[i];
        HuPrcVSleep();
    }
    while(ev_CapMasuNumGet(work->playerNo) < 0 || ev_CapMasuNumGet(work->playerNo) > 60) {
        HuPrcVSleep();
    }
    n = timer = 0;
    do {
        HuPrcVSleep();
        timer++;
        if(timer > 1 && n < 3) {
            mbAudFXPlay(0x417);
            objWork = obj[n]->data;
            objWork->mode++;
            timer = 0;
            n++;
        }
        masuNum = ev_CapMasuNumGet(work->playerNo);
    } while(masuNum < 0 || masuNum > 20 || n < 3);
    total = ev_CapMasuNumGet(work->playerNo);
    if(total <= 0.0f) {
        total = 1.0f;
    }
    n = timer = 0;
    do {
        t = 1.0f-((ev_CapMasuNumGet(work->playerNo)-1)/total);
        masuNumPrev = ev_CapMasuNumGet(work->playerNo);
        timer++;
        if(timer > 1 && n < 3) {
            objWork = obj[n]->data;
            objWork->mode++;
            timer = 0;
            n++;
        }
        HuPrcVSleep();
    } while(t < 1.0f && ev_CapMasuNumGet(work->playerNo) <= masuNumPrev);
    for(i=0; i<128; i++) {
        rot.x = 45.0f*CAP_RANDF();
        rot.y = 360.0f*CAP_RANDF();
        speed = 100.0f*(0.21000001f*CAP_RANDF());
        glowPos.x = masuPos.x+(0.5f*(100.0f*(-0.5f+CAP_RANDF())));
        glowPos.y = 100.0f+(masuPos.y+(0.5f*(100.0f*(-0.5f+CAP_RANDF()))));
        glowPos.z = masuPos.z+(0.5f*(100.0f*(-0.5f+CAP_RANDF())));
        glowVel.x = speed*CapSinDeg(rot.x)*CapSinDeg(rot.y);
        glowVel.y = speed*CapCosDeg(rot.x);
        glowVel.z = speed*CapSinDeg(rot.x)*CapCosDeg(rot.y);
        color.r = 128.0f+(127.0f*CAP_RANDF());
        color.g = 64.0f+(63.0f*CAP_RANDF());
        color.b = 32;
        color.a = 192.0f+(63.0f*CAP_RANDF());
        mbev_CapEffGlowAdd(work->glowEff, glowPos, glowVel, 60.0f*(0.8f+(0.3f*CAP_RANDF())), 100.0f*(0.2f+(0.1f*CAP_RANDF())), 3.0f*(-0.5f+CAP_RANDF()), 0.8166667f, color);
        if(i == 64) {
            HuPrcVSleep();
        }
    }
    while(n < 3) {
        timer++;
        if(timer > 1 && n < 3) {
            objWork = obj[n]->data;
            objWork->mode++;
            timer = 0;
            n++;
        }
        HuPrcVSleep();
    }
    do {
        for(i=0; i<3; i++) {
            if(obj[i] != NULL) {
                break;
            }
        }
        HuPrcVSleep();
    } while(i < 3);
    HuPrcEnd();
}

void mbev_CapBobleKill(void)
{
}

void mbev_CapBobleTrap(void *arg)
{
    CAPWORK *work = arg;
    int coinNum;
    HuVecF pos;
    HuVecF rot;
    HuVecF playerPos;
    int i;
    float t;

    if(!GwPlayer[work->playerNo].metalF) {
        mbPlayerMoveHookSet(work->playerNo, mbev_CapBobleMove);
        coinNum = mbPlayerCoinGet(work->playerNo);
        if(coinNum > 10) {
            coinNum = 10;
        }
        mbCoinAddDispExec(work->playerNo, -coinNum, FALSE, TRUE);
        mbPlayerPosGet(work->playerNo, &pos);
        pos.y += 250.0f;
        if(coinNum != 0) {
            mbCoinDispCapsuleCreate(&pos, -coinNum);
        }
        omVibrate(work->playerNo, 20, 20, 0);
        if(GwPlayer[work->playerNo].moveNum <= 1 && !_CheckFlag(FLAG_BOARD_DEBUG)) {
            mbPlayerMotionShiftSet(work->playerNo, 15, 0.0f, 8.0f, HU3D_MOTATTR_NONE);
            mbPlayerColSnapPlayerSet(work->playerNo, FALSE);
            mbMoveNumDispSet(work->playerNo, FALSE);
            mbPlayerRotGet(work->playerNo, &rot);
            for(i=1; i<=36.0f; i++) {
                t = i/36.0f;
                mbMasuPosGet(GwPlayer[work->playerNo].masuId, &pos);
                playerPos.x = pos.x;
                playerPos.y = pos.y+(3.0*(100.0*HuSin(180.0f*t)));
                playerPos.z = pos.z;
                mbPlayerPosSetV(work->playerNo, &playerPos);
                mbPlayerRotSet(work->playerNo, 0.0f, rot.y+(720.0f*t), 0.0f);
                HuPrcVSleep();
            }
            mbPlayerColSnapPlayerSet(work->playerNo, TRUE);
            mbPlayerMotionShiftSet(work->playerNo, 1, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
        }
    }
}

void mbev_CapBobleMove(int playerNo)
{
    HuVecF startPos;
    HuVecF endPos;
    HuVecF pos;
    HuVecF rot;
    HuVecF rot2;
    HuVecF objPos;
    HuVecF dir;
    int masuId;
    float dist;
    int i;
    int objId;
    int maxTime;
    int masuNext;
    float t;

    maxTime = 1.75f*mbPlayerWalkSpeedGet();
    masuId = GwPlayer[playerNo].masuId;
    masuNext = GwPlayer[playerNo].masuIdNext;
    mbPlayerPosGet(playerNo, &startPos);
    mbMasuPosGet(masuNext, &endPos);
    PSVECSubtract(&endPos, &startPos, &dir);
    rot.y = HuAtan(dir.x, dir.z);
    rot.x = rot.z = 0.0f;
    rot2 = rot;
    dist = PSVECMag(&dir);
    objId = mbObjCreate(0xC0044, NULL, FALSE);
    objPos = startPos;
    objPos.y += 100.0f;
    mbObjPosSetV(objId, &objPos);
    mbObjDispSet(objId, FALSE);
    mbCameraFocusObjSet(objId);
    mbPlayerMotionSet(playerNo, 15, HU3D_MOTATTR_LOOP);
    mbPlayerColSnapPlayerSet(playerNo, FALSE);
    mbAudFXPlay(0x400);
    for(i=1; i<maxTime; i++) {
        t = (float)i/(float)maxTime;
        mbMasuPosGet(masuNext, &endPos);
        pos.x = startPos.x+(t*(endPos.x-startPos.x));
        pos.y = (startPos.y+(t*(endPos.y-startPos.y)))+(300.0*HuSin(180.0f*t));
        pos.z = startPos.z+(t*(endPos.z-startPos.z));
        mbPlayerPosSetV(playerNo, &pos);
        mbPlayerRotSetV(playerNo, &rot);
        mbPlayerWorkGet(playerNo)->masuNext = maxTime-i;
        objPos.x = startPos.x+(t*(endPos.x-startPos.x));
        objPos.y = 100.0f+(startPos.y+(t*(endPos.y-startPos.y)));
        objPos.z = startPos.z+(t*(endPos.z-startPos.z));
        mbObjPosSetV(objId, &objPos);
        HuPrcVSleep();
    }
    pos = endPos;
    rot = rot;
    mbPlayerPosSetV(playerNo, &pos);
    mbPlayerRotSetV(playerNo, &rot);
    mbPlayerWorkGet(playerNo)->masuNext = 0;
    mbCameraFocusPlayerSet(playerNo);
    mbObjKill(objId);
}

static void ev_CapBobleOMExec(OMOBJ *obj)
{
    CAPBOBLEWORK *work = obj->data;
    float scale;
    float t;
    float s;
    float c;
    float angle;
    HuVecF dir;

    if(mbExitCheck() || work->endF) {
        if(!mbExitCheck()) {
            *work->objP = NULL;
        }
        omDelObjEx(mbObjMan, obj);
        return;
    }
    switch(work->mode) {
        case 1:
            t = ++work->time/3.0f;
            scale = CapSinDeg(90.0f*t);
            mbObjPosSetV(work->objId, &work->pos);
            mbObjScaleSet(work->objId, scale, scale, scale);
            mbObjDispSet(work->objId, TRUE);
            if(t >= 1.0f) {
                scale = 1.0f;
                mbObjScaleSet(work->objId, scale, scale, scale);
                work->mode++;
                work->time = 0;
            }
            break;

        case 3:
            t = ++work->time/15.0f;
            mbPlayerPosGet(work->playerNo, &work->endPos);
            work->endPos.x += 0.5*(100.0*HuSin(work->angle));
            work->endPos.z += 0.5*(100.0*HuCos(work->angle));
            work->pos.x = work->startPos.x+(t*(work->endPos.x-work->startPos.x));
            work->pos.y = work->startPos.y+(t*(work->endPos.y-work->startPos.y))+(work->height*CapSinDeg(180.0f*t));
            work->pos.z = work->startPos.z+(t*(work->endPos.z-work->startPos.z));
            mbObjPosSetV(work->objId, &work->pos);
            scale = 1.0f;
            mbObjScaleSet(work->objId, scale, scale, scale);
            if(t >= 1.0f) {
                if(!work->metalF) {
                    if(work->no == 0) {
                        work->mode++;
                    } else {
                        mbObjDispSet(work->objId, FALSE);
                        work->mode = 99;
                        work->time = 0;
                        work->endF = TRUE;
                    }
                } else {
                    work->mode = 10;
                }
                work->time = 0;
            }
            break;

        case 4:
            mbPlayerPosGet(work->playerNo, &work->endPos);
            work->endPos.x += 0.5*(100.0*HuSin(work->angle));
            work->endPos.z += 0.5*(100.0*HuCos(work->angle));
            mbObjPosSetV(work->objId, &work->endPos);
            t = ++work->time/10.0f;
            scale = 1.0f+CapSinDeg(90.0f*t);
            mbObjScaleSet(work->objId, scale, scale, scale);
            if(t >= 1.0f) {
                scale = 1.0f;
                mbObjScaleSet(work->objId, scale, scale, scale);
                work->mode++;
                work->time = 0;
            }
            break;

        case 5:
            mbPlayerPosGet(work->playerNo, &work->endPos);
            work->endPos.x += 0.5*(100.0*HuSin(work->angle));
            work->endPos.z += 0.5*(100.0*HuCos(work->angle));
            mbObjPosSetV(work->objId, &work->endPos);
            t = ++work->time/30.0f;
            scale = 2.0*HuCos(90.0f*t);
            mbObjScaleSet(work->objId, scale, scale, scale);
            if(t >= 1.0f) {
                mbObjDispSet(work->objId, FALSE);
                work->mode++;
                work->time = 0;
                work->endF = TRUE;
            }
            break;

        case 10:
            t = ++work->time/30.0f;
            work->pos.x = work->endPos.x+(t*(work->startPos.x-work->endPos.x));
            work->pos.y = work->endPos.y+(t*(work->startPos.y-work->endPos.y))+(work->height*CapSinDeg(180.0f*t));
            work->pos.z = work->endPos.z+(t*(work->startPos.z-work->endPos.z));
            mbObjPosSetV(work->objId, &work->pos);
            scale = HuCos(90.0f*t);
            mbObjScaleSet(work->objId, scale, scale, scale);
            PSVECSubtract(&work->startPos, &work->endPos, &dir);
            angle = HuAtan(dir.x, dir.z);
            s = CapSinDeg(angle);
            c = CapCosDeg(angle);
            mbObjRotSet(work->objId, 180.0f*(s*t), 0.0f, 180.0f*(c*t));
            if(t >= 1.0f) {
                mbObjDispSet(work->objId, FALSE);
                work->mode++;
                work->time = 0;
                work->endF = TRUE;
            }
            break;
    }
}

/* .data after ev_CapBobleOMExec's jump table */
static HuVecF biriQEffectOfs = { 0.0f, 100.0f, 0.0f };
char lbl_80248E80[16] = "itemhook_c";

void mbev_CapBiriQ(void)
{
    CAPWORK *work = HuPrcCurrentGet()->property;
    HuVecF pos[3];
    HuVecF vel[3];
    HuVecF masuPos;
    HuVecF rot;
    int objId[3];
    float angle[3];
    float a;
    float t;
    float speed;
    float scale2;
    float total;
    float scale;
    int i;
    int j;
    int masuNum;
    int masuNumPrev;

    for(i=0; i<3; i++) {
        objId[i] = mbev_CapObjCreate(work->objWork, 0xC0043, NULL, TRUE, 0, 0);
        mbObjDispSet(objId[i], FALSE);
        CapObjAttrSet(objId[i], HU3D_MOTATTR_LOOP);
        mbObjLayerSet(objId[i], 3);
    }
    mbMasuPosGet(GwPlayer[work->playerNo].masuIdNext, &masuPos);
    do {
        HuPrcVSleep();
        masuNum = ev_CapMasuNumGet(work->playerNo);
    } while(masuNum < 0 || masuNum > 20);
    a = 360.0f*CAP_RANDF();
    for(i=0; i<3; i++) {
        angle[i] = a+(120.0f*i);
    }
    total = ev_CapMasuNumGet(work->playerNo);
    if(total <= 0.0f) {
        total = 1.0f;
    }
    do {
        t = 1.0f-((ev_CapMasuNumGet(work->playerNo)-1)/total);
        masuNumPrev = ev_CapMasuNumGet(work->playerNo);
        for(i=0; i<3; i++) {
            a = angle[i] += 6.0f;
            pos[i].x = masuPos.x+((100.0f+(2.0f*(100.0f*(1.0f-t))))*HuSin(a));
            pos[i].y = (100.0f+masuPos.y)+(2.0f*(100.0f*(1.0f-t)));
            pos[i].z = masuPos.z+((100.0f+(2.0f*(100.0f*(1.0f-t))))*HuCos(a));
            scale = t;
            mbObjPosSetV(objId[i], &pos[i]);
            mbObjScaleSet(objId[i], scale, scale, scale);
            mbObjDispSet(objId[i], TRUE);
        }
        HuPrcVSleep();
    } while(t < 1.0f && ev_CapMasuNumGet(work->playerNo) <= masuNumPrev);
    if(!GwPlayer[work->playerNo].metalF) {
        for(j=1; j<=30; j++) {
            t = j/30.0f;
            for(i=0; i<3; i++) {
                a = angle[i] += 6.0f;
                mbPlayerPosGet(work->playerNo, &masuPos);
                pos[i].x = masuPos.x+(100.0*HuSin(a));
                pos[i].y = 100.0f+masuPos.y;
                pos[i].z = masuPos.z+(100.0*HuCos(a));
                mbObjPosSetV(objId[i], &pos[i]);
            }
            HuPrcVSleep();
        }
        for(j=1; j<=20; j++) {
            t = j/20.0f;
            scale2 = HuCos(90.0f*t);
            for(i=0; i<3; i++) {
                a = angle[i] += 6.0f;
                mbPlayerPosGet(work->playerNo, &masuPos);
                pos[i].x = masuPos.x+(100.0*(HuSin(a)*HuCos(90.0f*t)));
                pos[i].y = 100.0f+masuPos.y;
                pos[i].z = masuPos.z+(100.0*(HuCos(a)*HuCos(90.0f*t)));
                mbObjPosSetV(objId[i], &pos[i]);
                mbObjScaleSet(objId[i], scale2, scale2, scale2);
            }
            HuPrcVSleep();
        }
    } else {
        for(i=0; i<3; i++) {
            a = angle[i];
            rot.x = 20.0f+(15.0f*CAP_RANDF());
            rot.y = a;
            speed = 100.0f*(0.16000001f*CAP_RANDF());
            if(GwPlayer[work->playerNo].metalF) {
                2.0f*speed;
            }
            vel[i].x = speed*HuSin(rot.x)*HuSin(rot.y);
            vel[i].y = speed*HuCos(rot.x);
            vel[i].z = speed*HuSin(rot.x)*HuCos(rot.y);
        }
        for(j=0; j<60.0f; j++) {
            speed = j/60.0f;
            for(i=0; i<3; i++) {
                PSVECAdd(&pos[i], &vel[i], &pos[i]);
                vel[i].y -= 0.40833336f;
                vel[i].y *= 0.98f;
                mbObjPosSetV(objId[i], &pos[i]);
                if(GwPlayer[work->playerNo].metalF) {
                    mbObjRotGet(objId[i], &rot);
                    rot.z = speed*(180.0f*-(0.25f*vel[i].x));
                    mbObjRotSetV(objId[i], &rot);
                }
                mbObjAlphaSet(objId[i], 255.0f*(1.0f-speed));
            }
            HuPrcVSleep();
        }
    }
    HuPrcEnd();
}

void mbev_CapBiriQKill(void)
{
}

void mbev_CapBiriQTrap(void *arg)
{
    CAPWORK *work = arg;
    if(!GwPlayer[work->playerNo].metalF) {
        mbPlayerBiriQSet(work->playerNo, TRUE);
    }
    if(GwPlayer[work->playerNo].metalF) {
        mbev_CapBiriQMetalShockCreate(work->playerNo);
    } else {
        mbev_CapBiriQShockCreate(work->playerNo);
    }
}

void mbev_CapBiriQMetalShock(void *arg)
{
    CAPWORK *work = arg;
    HuVecF playerPos;
    HuVecF pos;
    HuVecF scale;
    HuVecF vel;
    HuVecF rot;
    GXColor color;
    float speed;
    int i;
    void *electricEff;
    int id;
    int coinNum;
    int motId;
    void *ringEff;
    void *glowEff;
    int metalF;

    motId = -1;
    metalF = GwPlayer[work->playerNo].metalF;
    ringEff = mbev_CapEffRingHitCreate();
    HuPrcVSleep();
    glowEff = mbev_CapEffGlowFireCreate();
    HuPrcVSleep();
    electricEff = mbev_CapEffElectricCreate();
    HuPrcVSleep();
    mbPlayerPosGet(work->playerNo, &playerPos);
    HuPrcVSleep();
    rot.x = 360.0f*CapEffRandF();
    rot.y = 360.0f*CapEffRandF();
    rot.z = 360.0f*CapEffRandF();
    for(i=0; i<3; i++) {
        pos.x = playerPos.x;
        pos.y = 100.0f+playerPos.y;
        pos.z = playerPos.z;
        scale.x = 0.5f;
        scale.y = 2.5f;
        scale.z = 100.0f*(2.0f+(1.5f*CapEffRandF()));
        mbev_CapEffColorSet(&color, CapEffRand());
        mbev_CapEffRingAdd(ringEff, pos, rot, scale, 1, 60.0*(0.3f+(0.15*CapEffRandF())), i, color);
        rot.x += 45.0f+(45.0f*CapEffRandF());
        rot.y += 45.0f+(45.0f*CapEffRandF());
        rot.z += 45.0f+(45.0f*CapEffRandF());
    }
    PSVECAdd(&playerPos, &biriQEffectOfs, &pos);
    id = mbev_CapEffElectricAdd(electricEff, pos, 12, 1);
    mbev_CapEffElectricModelSet(electricEff, mbPlayerObjIDGet(work->playerNo), id, &biriQEffectOfs);
    id = mbev_CapEffElectricAdd(electricEff, pos, 15, 1);
    mbev_CapEffElectricModelSet(electricEff, mbPlayerObjIDGet(work->playerNo), id, &biriQEffectOfs);
    id = mbev_CapEffElectricAdd(electricEff, pos, 18, 1);
    mbev_CapEffElectricModelSet(electricEff, mbPlayerObjIDGet(work->playerNo), id, &biriQEffectOfs);
    HuPrcVSleep();
    for(i=0; i<128; i++) {
        rot.x = (180.0f*CapEffRandF())-90.0f;
        rot.y = 360.0f*CapEffRandF();
        speed = 100.0f*(0.21000001f*CapEffRandF());
        scale.x = playerPos.x+(0.5f*(100.0f*CapEffRandHalfF()));
        scale.y = 100.0f+(playerPos.y+(0.5f*(100.0f*CapEffRandHalfF())));
        scale.z = playerPos.z+(0.5f*(100.0f*CapEffRandHalfF()));
        vel.x = speed*CapSinDeg(rot.x)*CapSinDeg(rot.y);
        vel.y = speed*CapCosDeg(rot.x);
        vel.z = speed*CapSinDeg(rot.x)*CapCosDeg(rot.y);
        color.r = 64.0f+(63.0f*CapEffRandF());
        color.g = 127.0f+(63.0f*CapEffRandF());
        color.b = 192.0f+(63.0f*CapEffRandF());
        color.a = 192.0f+(63.0f*CapEffRandF());
        mbev_CapEffGlowAdd(glowEff, scale, vel, 60.0f*(0.4f+(0.2f*CapEffRandF())), 100.0f*(0.2f+(0.1f*CapEffRandF())), 3.0f*(-0.5f+CAP_RANDF()), 0.0f, color);
        if(i == 64) {
            HuPrcVSleep();
        }
    }
    mbAudFXPlay(0x41D);
    if(!metalF) {
        mbCameraShakeSet(18, 50.0f);
        HuPrcVSleep();
        omVibrate(work->playerNo, 20, 7, 3);
        coinNum = mbPlayerCoinGet(work->playerNo);
        if(coinNum > 5) {
            coinNum = 5;
        }
        mbPlayerPosGet(work->playerNo, &scale);
        scale.y += 250.0f;
        mbCoinAddDispExec(work->playerNo, -coinNum, FALSE, TRUE);
        mbCoinDispCreate(&scale, -coinNum, -1, TRUE);
        HuPrcVSleep();
        motId = mbPlayerMotionCreate(work->playerNo, 0x93001E);
        mbPlayerMotionShiftSet(work->playerNo, motId, 50.0f, 8.0f, HU3D_MOTATTR_NONE);
        while(mbObjMotionShiftIDGet(mbPlayerObjIDGet(work->playerNo)) != -1) {
            HuPrcVSleep();
        }
        while(!mbObjMotionEndCheck(mbPlayerObjIDGet(work->playerNo))) {
            HuPrcVSleep();
        }
        mbev_CapPlayerMotShiftWait(work->playerNo, 1, HU3D_MOTATTR_LOOP, TRUE);
        CharMotionUpdateSet(GwPlayer[work->playerNo].charNo, 0x930018, TRUE);
    }
    while(mbev_CapEffRingDispGet(ringEff)) {
        HuPrcVSleep();
    }
    while(mbev_CapEffGlowDispGet(glowEff)) {
        HuPrcVSleep();
    }
    if(!work->shockNoWaitF) {
        while(mbev_CapBiriQShockDelayGet(work->playerNo) > 0) {
            HuPrcVSleep();
        }
    }
    if(mbPlayerCoinGet(work->playerNo) <= 0 && GwPlayer[work->playerNo].biriQF) {
        mbPlayerBiriQSet(work->playerNo, FALSE);
    }
    if(motId != -1) {
        mbPlayerMotionKill(work->playerNo, motId);
    }
    mbev_CapEffRingKill(ringEff);
    mbev_CapEffGlowKill(glowEff);
    mbev_CapEffElectricKill(electricEff);
}

static int ev_CapMasuNumGet(int playerNo)
{
    return mbev_CapBiriQShockDelayGet(playerNo)+CapMasuNextGet(playerNo);
}

void mbev_CapTumujikun(void)
{
    CAPWORK *work = HuPrcCurrentGet()->property;
    int motTbl[16];
    HuVecF pos;
    HuVecF masuPos;
    HuVecF startPos;
    HuVecF movePos;
    float t;
    float total;
    float t2;
    int objId;
    int objId2;
    int i;
    int masuNum;
    int masuNumPrev;
    int seNo;

    motTbl[0] = 0xC0040;
    motTbl[1] = 0xC0041;
    motTbl[2] = -1;
    objId = mbev_CapObjCreate(work->objWork, 0xC003F, motTbl, FALSE, 0, 0);
    mbObjDispSet(objId, FALSE);
    mbObjLayerSet(objId, 3);
    objId2 = mbev_CapObjCreate(work->objWork, 0xC0042, NULL, FALSE, 5, 0);
    mbObjDispSet(objId2, FALSE);
    mbObjLayerSet(objId2, 3);
    mbMasuPosGet(work->masuId, &masuPos);
    startPos = masuPos;
    startPos.y += 1000.0f;
    pos = masuPos;
    do {
        HuPrcVSleep();
        masuNum = ev_CapMasuNumGet(work->playerNo);
    } while(masuNum < 0 || masuNum > 20);
    mbAudFXPlay(0x437);
    mbObjMotionTimeSet(objId2, 0.0f);
    mbObjMotionSpeedSet(objId2, 0.5f);
    mbObjPosSetV(objId2, &pos);
    mbObjScaleSet(objId2, 2.0f, 2.0f, 2.0f);
    mbObjDispSet(objId2, TRUE);
    HuPrcSleep(15);
    total = ev_CapMasuNumGet(work->playerNo);
    if(total <= 0.0f) {
        total = 1.0f;
    }
    do {
        t2 = 1.0f-((ev_CapMasuNumGet(work->playerNo)-1)/total);
        masuNumPrev = ev_CapMasuNumGet(work->playerNo);
        HuPrcVSleep();
    } while(t2 < 1.0f && ev_CapMasuNumGet(work->playerNo) <= masuNumPrev);
    seNo = mbAudFXPlay(0x437);
    mbObjMotionSet(objId, 1, HU3D_MOTATTR_LOOP);
    mbObjPosSetV(objId, &pos);
    mbObjDispSet(objId, TRUE);
    for(i=0; i<30.0f; i++) {
        t = i/30.0f;
        mbObjScaleSet(objId, 1.5f, 1.5*HuSin(90.0f*t), 1.5f);
        HuPrcVSleep();
    }
    if(!GwPlayer[work->playerNo].metalF) {
        HuPrcSleep(30);
        mbAudFXPlay(0x439);
        total = 60.0f;
        for(i=0; i<total; i++) {
            t2 = i/total;
            t = HuSin(90.0f*t2);
            movePos.x = masuPos.x+(t*(startPos.x-masuPos.x));
            movePos.y = masuPos.y+(t*(startPos.y-masuPos.y));
            movePos.z = masuPos.z+(t*(startPos.z-masuPos.z));
            mbObjPosSetV(objId, &movePos);
            mbObjAlphaSet(objId, 255.0f*(1.0f-t));
            HuPrcVSleep();
        }
        mbObjDispSet(objId, FALSE);
        if(seNo != -1) {
            mbAudFXStop(seNo);
        }
    } else {
        work->explodeEff = mbev_CapEffExplodeCreate();
        for(i=0; i<12.0f; i++) {
            t = i/12.0f;
            mbObjScaleSet(objId, 1.5f, 1.5*HuCos(90.0f*t), 1.5f);
            mbObjAlphaSet(objId2, 255.0f*(1.0f-t));
            HuPrcVSleep();
        }
        mbMasuPosGet(work->masuId, &pos);
        mbev_CapEffDustHeavyAdd(work->explodeEff, pos);
        if(seNo != -1) {
            mbAudFXStop(seNo);
        }
        mbAudFXPlay(0x43A);
        while(mbev_CapEffExplodeAnimGet(work->explodeEff) > 0) {
            HuPrcVSleep();
        }
    }
    HuPrcEnd();
}

/* @@PART2@@ */
#include "game/board/branch.h"
#include "game/board/capsule.h"
#include "game/board/wipe.h"
#include "game/audio.h"
#include "game/process.h"

/* capevent.c; local prototypes (the B0 header's are placeholders). */
void *mbev_CapEffGlowCreate(void);
s16 mbev_CapPlayerMotionCreate(void *objWork, int playerNo, int dataNum);
void mbev_CapPlayerPosSet(void *objWork, int playerNo, int masuId, HuVecF *pos);
void mbev_CapPlayerRotate(int playerNo, float rotY);
void mbev_CapEffRingHitAdd(void *eff, HuVecF pos, HuVecF rot, HuVecF scale);
void mbev_CapEffGlowCoinAdd(void *eff, HuVecF *pos, HuVecF *rot);
int mbev_CapPlayerSquishSet(int *playerNo, int masuId);
void mbev_CapPlayerStunSet(int *playerNo, int num, int arg2);
void mbev_CapPlayerIdleWait(void);
void mbev_CapVecChase(float t, HuVecF *a, HuVecF *b, HuVecF *out);

void mbev_CapTumujikunKill(void)
{
}

void mbev_CapTumujikunTrap(void *arg)
{
    CAPWORK *work = arg;
    HuVecF pos;
    HuVecF rot;
    HuVecF masuPos;
    HuVecF topPos;
    HuVecF startPos;
    HuVecF camPos;
    float t;
    int i;
    int id;
    int masuNum;
    int masuId;
    int objId;
    int motId;
    int start;

    masuId = GwPlayer[work->playerNo].masuId;
    mbPlayerPosGet(work->playerNo, &pos);
    mbPlayerRotGet(work->playerNo, &rot);
    if (GwPlayer[work->playerNo].metalF) {
        motId = mbPlayerMotionCreate(work->playerNo, 0x8E0021);
        mbMoveNumDispSet(work->playerNo, FALSE);
        mbPlayerMotionShiftSet(work->playerNo, motId, 0.0f, 8.0f, 0x40000001);
        mbPlayerColSnapPlayerSet(work->playerNo, FALSE);
        for (i = 0; i < 30.0f; i++) {
            t = i / 30.0f;
            t = HuSin(90.0f * t);
            mbMasuPosGet(masuId, &masuPos);
            topPos = masuPos;
            topPos.y += 400.0f;
            pos = masuPos;
            pos.y = masuPos.y + t * (topPos.y - masuPos.y);
            mbPlayerPosSetV(work->playerNo, &pos);
            HuPrcVSleep();
        }
        for (i = 0; i < 9.0f; i++) {
            mbMasuPosGet(masuId, &masuPos);
            topPos = masuPos;
            topPos.y += 400.0f;
            mbPlayerPosSetV(work->playerNo, &topPos);
            HuPrcVSleep();
        }
        for (i = 0; i < 12.0f; i++) {
            t = i / 12.0f;
            t = HuSin(90.0f * t);
            mbMasuPosGet(masuId, &masuPos);
            topPos = masuPos;
            topPos.y += 400.0f;
            pos = topPos;
            pos.y = topPos.y + t * (masuPos.y - topPos.y);
            mbPlayerPosSetV(work->playerNo, &pos);
            if (i == 6) {
                mbPlayerMotionShiftSet(work->playerNo, 5, 0.0f, 8.0f, 0);
            }
            HuPrcVSleep();
        }
        mbPlayerColSnapPlayerSet(work->playerNo, TRUE);
        while (!mbObjMotionEndCheck(mbPlayerObjIDGet(work->playerNo)) || mbObjMotionShiftIDGet(mbPlayerObjIDGet(work->playerNo)) != -1) {
            HuPrcVSleep();
        }
        mbMoveNumDispSet(work->playerNo, TRUE);
        mbPlayerMotionKill(work->playerNo, motId);
        return;
    }
    objId = mbObjCreate(0xC0044, NULL, FALSE);
    camPos = pos;
    camPos.y += 100.0f;
    mbObjPosSetV(objId, &camPos);
    mbObjDispSet(objId, FALSE);
    mbCameraFocusObjSet(objId);
    motId = mbPlayerMotionCreate(work->playerNo, 0x930022);
    startPos = pos;
    masuPos = topPos = startPos;
    topPos.y += 200.0f;
    startPos.y += 1000.0f;
    mbMoveNumDispSet(work->playerNo, FALSE);
    mbPlayerMotionShiftSet(work->playerNo, motId, 0.0f, 8.0f, 0x40000001);
    mbPlayerColSnapPlayerSet(work->playerNo, FALSE);
    omVibrate(work->playerNo, 20, 4, 4);
    for (i = 0; i < 30.0f; i++) {
        t = i / 30.0f;
        t = HuSin(90.0f * t);
        mbMasuPosGet(masuId, &masuPos);
        topPos = masuPos;
        topPos.y += 200.0f;
        pos.x = masuPos.x + t * (topPos.x - masuPos.x);
        pos.y = masuPos.y + t * (topPos.y - masuPos.y);
        pos.z = masuPos.z + t * (topPos.z - masuPos.z);
        if ((rot.y += 10.0f * t) > 360.0f) {
            rot.y -= 360.0f;
        }
        mbPlayerPosSetV(work->playerNo, &pos);
        mbPlayerRotSetV(work->playerNo, &rot);
        HuPrcVSleep();
    }
    omVibrate(work->playerNo, 90, 7, 3);
    for (i = 0; i < 30.0f; i++) {
        t = i / 30.0f;
        mbMasuPosGet(masuId, &masuPos);
        topPos = masuPos;
        topPos.y += 200.0f;
        pos = topPos;
        pos.y += 0.2f * (100.0 * HuSin(360.0f * t));
        if ((rot.y += 10.0f) > 360.0f) {
            rot.y -= 360.0f;
        }
        mbPlayerPosSetV(work->playerNo, &pos);
        mbPlayerRotSetV(work->playerNo, &rot);
        HuPrcVSleep();
    }
    for (i = 0; i < 45.0f; i++) {
        t = i / 45.0f;
        t = HuSin(90.0f * t);
        mbMasuPosGet(masuId, &masuPos);
        topPos = masuPos;
        topPos.y += 200.0f;
        pos.x = topPos.x + t * (startPos.x - topPos.x);
        pos.y = topPos.y + t * (startPos.y - topPos.y);
        pos.z = topPos.z + t * (startPos.z - topPos.z);
        if ((rot.y += 10.0f + 10.0f * t) > 360.0f) {
            rot.y -= 360.0f;
        }
        mbPlayerPosSetV(work->playerNo, &pos);
        mbPlayerRotSetV(work->playerNo, &rot);
        HuPrcVSleep();
    }
    mbPlayerDispSet(work->playerNo, FALSE);
    mbWipeDissolveFadeOutTime(1);
    masuNum = mbMasuNumGet();
    start = mbRandMod(masuNum);
    for (i = 0; i < masuNum; i++) {
        id = start + i;
        if (id >= masuNum) {
            id -= masuNum;
        }
        if (id == 0) {
            id++;
        }
        if (id == masuId) {
            continue;
        }
        if (mbMasuMAttrGet(id) & mbBranchAttrGet()) {
            continue;
        }
        if (mbCapMasuDispTypeGet(id) == 2) {
            continue;
        }
        if (mbMasuTypeGet(id) == 1 || mbMasuTypeGet(id) == 2) {
            break;
        }
    }
    if (i >= masuNum) {
        for (i = 0; i < masuNum; i++) {
            id = start + i;
            if (id >= masuNum) {
                id -= masuNum;
            }
            if (id == 0) {
                id++;
            }
            if (id == masuId) {
                continue;
            }
            if (mbMasuMAttrGet(id) & mbBranchAttrGet()) {
                continue;
            }
            if (mbCapMasuDispTypeGet(id) == 2 && work->playerNo != mbCapMasuPlayerGet(id)) {
                continue;
            }
            if (mbMasuTypeGet(id) == 1 || mbMasuTypeGet(id) == 2) {
                break;
            }
        }
    }
    if (i >= masuNum) {
        for (i = 0; i < masuNum; i++) {
            id = start + i;
            if (id >= masuNum) {
                id -= masuNum;
            }
            if (id == 0) {
                id++;
            }
            if (id == masuId) {
                continue;
            }
            if (mbMasuMAttrGet(id) & mbBranchAttrGet()) {
                continue;
            }
            if (mbMasuTypeGet(id) == 1 || mbMasuTypeGet(id) == 2) {
                break;
            }
        }
    }
    mbMasuPosGet(id, &masuPos);
    camPos = masuPos;
    camPos.y += 100.0f;
    mbObjPosSetV(objId, &camPos);
    mbCameraMoveOnSet(FALSE);
    mbCameraMoveWait();
    mbCameraMoveOnSet(TRUE);
    mbev_PlayerColMasuSet(work->playerNo, id, TRUE);
    mbWipeDissolveFadeIn();
    mbPlayerDispSet(work->playerNo, TRUE);
    for (i = 0; i < 60.0f; i++) {
        t = i / 60.0f;
        t = HuSin(90.0f * t);
        mbMasuPosGet(id, &masuPos);
        topPos = masuPos;
        topPos.y += 2000.0f;
        pos.x = topPos.x + t * (masuPos.x - topPos.x);
        pos.y = topPos.y + t * (masuPos.y - topPos.y);
        pos.z = topPos.z + t * (masuPos.z - topPos.z);
        if ((rot.y += 30.0f) > 360.0f) {
            rot.y -= 360.0f;
        }
        mbPlayerPosSetV(work->playerNo, &pos);
        mbPlayerRotSetV(work->playerNo, &rot);
        HuPrcVSleep();
    }
    GwPlayer[work->playerNo].masuId = id;
    mbPlayerColSnapPlayerSet(work->playerNo, TRUE);
    GwPlayer[work->playerNo].masuIdNext = id;
    mbPlayerMotionShiftSet(work->playerNo, 6, 0.0f, 8.0f, 0x40000001);
    HuPrcSleep(60);
    mbPlayerMotionShiftSet(work->playerNo, 1, 0.0f, 8.0f, 0x40000001);
    if (GwPlayer[work->playerNo].moveNum > 1) {
        mbMoveNumDispSet(work->playerNo, TRUE);
    }
    mbCameraFocusPlayerSet(work->playerNo);
    mbObjKill(objId);
    mbPlayerMotionKill(work->playerNo, motId);
}

void mbev_CapDossun(void)
{
    CAPWORK *work = HuPrcCurrentGet()->property;
    HuVecF masuPos;
    HuVecF startPos;
    HuVecF pos;
    HuVecF playerPos;
    HuVecF scale;
    HuVecF rot;
    GXColor color;
    float maxTime;
    float t;
    float s;
    float dist;
    float angle;
    float rotX;
    float rotZ;
    int i;
    int objId;
    int time;
    int now;

    work->explodeEff = mbev_CapEffExplodeCreate();
    work->ringHitEff = mbev_CapEffRingHitCreate();
    objId = mbev_CapObjCreate(work->objWork, 0xC003E, 0, 0, 0, 0);
    mbObjAttrSet(objId, 0x40000001);
    mbObjDispSet(objId, FALSE);
    mbObjLayerSet(objId, 3);
    mbMasuPosGet(work->masuId, &masuPos);
    startPos = masuPos;
    startPos.y += 1000.0f;
    if (GwPlayer[work->playerNo].metalF) {
        masuPos.y += 150.0f;
    }
    do {
        HuPrcVSleep();
        time = ev_CapMasuNumGet(work->playerNo);
    } while (time < 0 || time > 20);
    maxTime = ev_CapMasuNumGet(work->playerNo);
    if (maxTime <= 0.0f) {
        maxTime = 1.0f;
    }
    do {
        t = 1.0f - (ev_CapMasuNumGet(work->playerNo) - 1) / maxTime;
        s = HuSin(90.0f * (1.0f - t));
        now = ev_CapMasuNumGet(work->playerNo);
        mbMasuPosGet(work->masuId, &masuPos);
        pos.x = masuPos.x + s * (startPos.x - masuPos.x);
        pos.y = masuPos.y + s * (startPos.y - masuPos.y);
        pos.z = masuPos.z + s * (startPos.z - masuPos.z);
        mbObjPosSetV(objId, &pos);
        mbObjDispSet(objId, TRUE);
        HuPrcVSleep();
    } while (t < 1.0f && ev_CapMasuNumGet(work->playerNo) <= now);
    if (!GwPlayer[work->playerNo].metalF) {
        mbAudFXPlay(0x41E);
        mbAudFXPlay(0x3AB);
        mbev_CapEffDustHeavyAdd(work->explodeEff, masuPos);
        mbObjPosSetV(objId, &masuPos);
        for (i = 0; i < 60.0f; i++) {
            mbMasuPosGet(work->masuId, &masuPos);
            mbObjPosSetV(objId, &masuPos);
            HuPrcVSleep();
        }
        maxTime = 120.0f;
        for (i = 0; i < maxTime; i++) {
            t = i / maxTime;
            s = HuSin(90.0f * t);
            mbMasuPosGet(work->masuId, &masuPos);
            pos.x = masuPos.x + s * (startPos.x - masuPos.x);
            pos.y = masuPos.y + s * (startPos.y - masuPos.y);
            pos.z = masuPos.z + s * (startPos.z - masuPos.z);
            mbObjPosSetV(objId, &pos);
            mbObjDispSet(objId, TRUE);
            HuPrcVSleep();
        }
        mbObjDispSet(objId, FALSE);
    } else {
        mbPlayerPosGet(work->playerNo, &playerPos);
        playerPos.y += 150.0f;
        scale.x = 0.5f;
        scale.y = 3.0f;
        scale.z = 100.0f * (1.0f + 0.25f * CAP_RANDF());
        rot.x = 90.0f + 20.0f * (-0.5f + CAP_RANDF());
        rot.y = 0.0f;
        rot.z = 20.0f * (-0.5f + CAP_RANDF());
        color.r = 255;
        color.g = 255;
        color.b = 127;
        color.a = 255;
        mbev_CapEffRingAdd(work->ringHitEff, playerPos, rot, scale, 1, 12, 2, color);
        dist = 100.0f * (3.0f + 2.0f * CAP_RANDF());
        angle = 360.0f * CAP_RANDF();
        rotX = 180.0 * HuSin(90.0f * angle);
        rotZ = 180.0 * HuCos(90.0f * angle);
        startPos.x = masuPos.x + dist * HuSin(angle);
        startPos.z = masuPos.z + dist * HuSin(angle);
        startPos.y = masuPos.y + 100.0f * (1.0f + CAP_RANDF());
        maxTime = 60.0f;
        for (i = 0; i < maxTime; i++) {
            t = i / maxTime;
            s = HuSin(90.0f * t);
            pos.x = masuPos.x + s * (startPos.x - masuPos.x);
            pos.y = masuPos.y + s * (startPos.y - masuPos.y) + 5.0 * (100.0 * HuSin(180.0f * s));
            pos.z = masuPos.z + s * (startPos.z - masuPos.z);
            mbObjPosSetV(objId, &pos);
            mbObjRotSet(objId, rotX * s, 0.0f, rotZ * s);
            mbObjAlphaSet(objId, 255.0f - 255.0f * s);
            HuPrcVSleep();
        }
    }
    HuPrcEnd();
}

void mbev_CapDossunKill(void)
{
}

void mbev_CapDossunTrap(void *arg)
{
    CAPWORK *work = arg;
    int squishTbl[4];
    int num;
    int i;

    if (!GwPlayer[work->playerNo].metalF) {
        GwPlayer[work->playerNo].moveNum = 0;
        mbMoveNumDispSet(work->playerNo, FALSE);
        for (i = 0; i < 4; i++) {
            if (GwPlayer[i].masuId == GwPlayer[work->playerNo].masuId) {
                omVibrate(i, 20, 7, 3);
            }
        }
        num = mbev_CapPlayerSquishSet(squishTbl, GwPlayer[work->playerNo].masuId);
        HuPrcSleep(60);
        mbCameraPlayerViewSet(work->playerNo, 0);
        HuPrcSleep(60);
        mbev_CapPlayerStunSet(squishTbl, num, 0);
        HuPrcSleep(60);
        mbev_CapPlayerIdleWait();
    }
}

void mbev_CapBomhei(void)
{
    CAPWORK *work = HuPrcCurrentGet()->property;
    HuVecF playerPos;
    HuVecF masuPos;
    HuVecF topPos;
    HuVecF explodePos;
    HuVecF objPos;
    HuVecF rot;
    HuVecF scale;
    HuVecF pos;
    float t;
    int playerNo;
    int objId;
    int hookObjId;
    int explodeObjId;
    int i;

    playerNo = work->playerNo;
    bomheiMode[playerNo] = 0;
    bomheiRotY[playerNo] = 180.0f * (-0.5f + CAP_RANDF());
    work->glowEff = mbev_CapEffGlowCreate();
    HuPrcVSleep();
    work->ringHitEff = mbev_CapEffRingHitCreate();
    HuPrcVSleep();
    objId = mbev_CapObjCreate(work->objWork, 0xC003A, 0, 0, 0, 0);
    mbObjLayerSet(objId, 3);
    mbObjAttrSet(objId, 0x40000001);
    mbObjDispSet(objId, FALSE);
    HuPrcVSleep();
    hookObjId = mbev_CapObjCreate(work->objWork, 0xC003C, 0, 0, 0, 0);
    mbObjAttrSet(hookObjId, 0x40000001);
    mbObjLayerSet(hookObjId, 5);
    mbObjHookSet(objId, lbl_80248E80, hookObjId);
    HuPrcVSleep();
    while (bomheiMode[playerNo] < 1) {
        HuPrcVSleep();
    }
    mbMasuPosGet(work->masuId, &masuPos);
    topPos = masuPos;
    explodePos = topPos;
    masuPos.y += 150.0f;
    topPos.y += 500.0f;
    explodePos.y += 50.0f;
    if (!GwPlayer[playerNo].metalF) {
        mbObjDispSet(objId, TRUE);
        mbObjDispSet(hookObjId, TRUE);
        for (i = 0; i < 40; i++) {
            t = i / 40.0f;
            mbPlayerPosGet(playerNo, &playerPos);
            objPos.x = playerPos.x;
            objPos.y = 100.0f + playerPos.y + 8.0 * (100.0 * HuCos(90.0f * t));
            objPos.z = 50.0f + playerPos.z;
            mbObjPosSet(objId, objPos.x, objPos.y, objPos.z);
            HuPrcVSleep();
        }
        mbObjPosSet(objId, 0.0f, 0.0f, 0.0f);
        mbObjHookSet(mbPlayerObjIDGet(playerNo), CharModelItemHookGet(GwPlayer[playerNo].charNo, 4, 0), objId);
        CharFXPlay(GwPlayer[playerNo].charNo, 0x245);
        mbAudFXPlay(0x3A1);
        bomheiMode[playerNo] = 2;
        while (bomheiMode[playerNo] < 3) {
            HuPrcVSleep();
        }
        mbObjHookReset(mbPlayerObjIDGet(playerNo));
        mbObjHookReset(objId);
        mbObjDispSet(objId, FALSE);
        mbObjDispSet(hookObjId, FALSE);
        explodeObjId = mbev_CapObjCreate(work->objWork, 0xC003B, 0, 0, 0, 0);
        mbObjLayerSet(explodeObjId, 3);
        mbObjScaleSet(explodeObjId, 1.2f, 1.2f, 1.2f);
        mbObjMotionTimeSet(explodeObjId, 5.0f);
        mbObjMotionSpeedSet(explodeObjId, 1.0f);
        mbObjPosSetV(explodeObjId, &explodePos);
        mbAudFXPlay(0x430);
        HuPrcSleep(60);
    } else {
        HuPrcSleep(26);
        mbObjDispSet(objId, TRUE);
        mbObjDispSet(hookObjId, TRUE);
        for (i = 1; i <= 40; i++) {
            t = i / 40.0f;
            mbPlayerPosGet(playerNo, &playerPos);
            objPos.x = playerPos.x;
            objPos.y = 230.0f + playerPos.y + 8.0 * (100.0 * HuCos(90.0f * t));
            objPos.z = playerPos.z;
            mbObjPosSet(objId, objPos.x, objPos.y, objPos.z);
            HuPrcVSleep();
        }
        mbObjPosGet(objId, &objPos);
        pos.x = objPos.x;
        pos.y = 10.0f + objPos.y;
        pos.z = objPos.z;
        rot.x = 90.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
        scale.x = 0.5f;
        scale.y = 3.0f;
        scale.z = 100.0f * (1.0f + 0.25f * CAP_RANDF());
        mbev_CapEffRingHitAdd(work->ringHitEff, pos, rot, scale);
        pos.x = objPos.x;
        pos.y = 20.0f + objPos.y;
        pos.z = objPos.z;
        rot.x = rot.y = rot.z = 0.0f;
        mbev_CapEffGlowCoinAdd(work->glowEff, &pos, &rot);
        mbev_CapEffGlowCoinAdd(work->glowEff, &pos, &rot);
        for (i = 1; i <= 22; i++) {
            t = i / 22.0f;
            pos.x = objPos.x + t * (100.0 * HuSin(bomheiRotY[playerNo]));
            pos.y = objPos.y + 1.5 * (100.0 * HuSin(90.0f * t)) + 100.0 * HuSin(180.0f * t);
            pos.z = objPos.z + t * (100.0 * HuCos(bomheiRotY[playerNo]));
            mbObjPosSetV(objId, &pos);
            mbObjRotSet(objId, 0.0f, 360.0f * t, 0.0f);
            HuPrcVSleep();
        }
        mbObjPosGet(objId, &objPos);
        pos.x = objPos.x + 0.5 * (100.0 * HuSin(180.0f + bomheiRotY[playerNo]));
        pos.y = 100.0f + objPos.y;
        pos.z = objPos.z + 0.5 * (100.0 * HuCos(180.0f + bomheiRotY[playerNo]));
        rot.x = 0.0f;
        rot.y = bomheiRotY[playerNo];
        rot.z = 0.0f;
        scale.x = 0.5f;
        scale.y = 3.0f;
        scale.z = 100.0f * (1.0f + 0.25f * CAP_RANDF());
        mbev_CapEffRingHitAdd(work->ringHitEff, pos, rot, scale);
        pos.x = objPos.x + 0.5 * (100.0 * HuSin(180.0f + bomheiRotY[playerNo]));
        pos.y = 100.0f + objPos.y;
        pos.z = objPos.z + 0.5 * (100.0 * HuCos(180.0f + bomheiRotY[playerNo]));
        rot.x = 90.0f;
        rot.y = 0.0f;
        rot.z = bomheiRotY[playerNo];
        mbev_CapEffGlowCoinAdd(work->glowEff, &pos, &rot);
        mbev_CapEffGlowCoinAdd(work->glowEff, &pos, &rot);
        for (i = 1; i <= 20; i++) {
            t = i / 20.0f;
            pos.x = objPos.x + t * (3.0 * (100.0 * HuSin(bomheiRotY[playerNo])));
            pos.y = objPos.y + 100.0 * HuSin(90.0f * t) + 1.5 * (100.0 * HuSin(180.0f * t));
            pos.z = objPos.z + t * (3.0 * (100.0 * HuCos(bomheiRotY[playerNo])));
            mbObjPosSetV(objId, &pos);
            mbObjRotSet(objId, 0.0f, 720.0f * t, 0.0f);
            HuPrcVSleep();
        }
        mbObjPosGet(objId, &objPos);
        mbObjDispSet(objId, FALSE);
        mbObjDispSet(hookObjId, FALSE);
        explodeObjId = mbev_CapObjCreate(work->objWork, 0xC003B, 0, 0, 0, 0);
        mbObjLayerSet(explodeObjId, 3);
        mbObjScaleSet(explodeObjId, 1.2f, 1.2f, 1.2f);
        mbObjMotionTimeSet(explodeObjId, 5.0f);
        mbObjMotionSpeedSet(explodeObjId, 1.0f);
        mbObjPosSetV(explodeObjId, &objPos);
        mbAudFXPlay(0x430);
        HuPrcSleep(60);
    }
    HuPrcEnd();
}

void mbev_CapBomheiKill(void)
{
}

void mbev_CapBomheiTrap(void *arg)
{
    CAPWORK *work = arg;
    int motId[16];
    HuVecF masuPos;
    HuVecF topPos;
    HuVecF rot;
    HuVecF pos;
    float t;
    int playerNo;
    int masuId;
    int i;
    int num;
    int odd;

    playerNo = work->playerNo;
    masuId = GwPlayer[playerNo].masuId;
    mbev_CapPlayerPosSet(work->objWork, playerNo, masuId, NULL);
    mbev_CapPlayerRotate(playerNo, 0.0f);
    if (!GwPlayer[playerNo].metalF) {
        motId[0] = mbev_CapPlayerMotionCreate(work->objWork, playerNo, 0x930046);
        motId[1] = mbev_CapPlayerMotionCreate(work->objWork, playerNo, 0x930047);
        mbPlayerMotionShiftSet(playerNo, motId[0], 0.0f, 8.0f, 0);
        bomheiMode[playerNo] = 1;
        while (bomheiMode[playerNo] < 2) {
            HuPrcVSleep();
        }
        while (!mbPlayerMotionEndCheck(playerNo)) {
            HuPrcVSleep();
        }
        bomheiMode[playerNo] = 3;
        omVibrate(playerNo, 20, 20, 0);
        mbMoveNumDispSet(playerNo, FALSE);
    } else {
        motId[0] = mbev_CapPlayerMotionCreate(work->objWork, playerNo, 0x930019);
        HuPrcVSleep();
        motId[1] = mbev_CapPlayerMotionCreate(work->objWork, playerNo, 0x93000B);
        HuPrcVSleep();
        motId[2] = mbev_CapPlayerMotionCreate(work->objWork, playerNo, 0x8E0025);
        HuPrcVSleep();
        motId[3] = mbev_CapPlayerMotionCreate(work->objWork, playerNo, 0x930038);
        HuPrcVSleep();
        motId[4] = mbev_CapPlayerMotionCreate(work->objWork, playerNo, 0x930008);
        HuPrcVSleep();
        mbMoveNumDispSet(playerNo, FALSE);
        mbPlayerMotionShiftSet(playerNo, motId[0], 0.0f, 8.0f, 0);
        bomheiMode[playerNo] = 1;
        HuPrcSleep(40);
        mbPlayerMotionShiftSet(playerNo, motId[1], 0.0f, 8.0f, 0);
        HuPrcSleep(30);
        mbev_CapPlayerPosSet(work->objWork, playerNo, -1, NULL);
        mbPlayerColSnapPlayerSet(playerNo, FALSE);
        mbPlayerMotionShiftSet(playerNo, 4, 0.0f, 4.0f, 0);
        mbMasuPosGet(masuId, &masuPos);
        topPos.x = masuPos.x;
        topPos.y = 500.0f + masuPos.y;
        topPos.z = masuPos.z;
        for (i = 1; i <= 26; i++) {
            t = i / 26.0f;
            mbev_CapVecChase(t, &masuPos, &topPos, &pos);
            pos.y += 100.0 * HuSin(180.0f * t);
            mbPlayerPosSetV(playerNo, &pos);
            mbPlayerRotSet(playerNo, 0.0f, bomheiRotY[playerNo] * HuSin(90.0f * t), 0.0f);
            if (i == 10) {
                mbPlayerMotionShiftSet(playerNo, motId[3], 0.0f, 4.0f, 0);
            }
            HuPrcVSleep();
        }
        mbPlayerMotionShiftSet(playerNo, motId[4], 999.0f, 8.0f, 0x40000004);
        for (i = 1; i <= 30.0f; i++) {
            t = i / 30.0f;
            mbMasuPosGet(masuId, &masuPos);
            mbev_CapVecChase(t, &topPos, &masuPos, &pos);
            pos.y += 100.0 * HuSin(180.0f * t);
            mbPlayerPosSetV(playerNo, &pos);
            mbPlayerRotSet(playerNo, 0.0f, bomheiRotY[playerNo] * HuCos(90.0f * t), 0.0f);
            if (i == 21) {
                mbPlayerMotionShiftSet(playerNo, 5, 0.0f, 8.0f, 0);
            }
            HuPrcVSleep();
        }
        mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 8.0f, 0x40000001);
        mbPlayerColSnapPlayerSet(playerNo, TRUE);
        if (GwPlayer[playerNo].moveNum > 1 || _CheckFlag(0x20001)) {
            mbMoveNumDispSet(playerNo, TRUE);
        }
    }
    mbev_CapPlayerPosSet(work->objWork, playerNo, -1, NULL);
    if (!GwPlayer[playerNo].metalF) {
        mbPlayerMoveHookSet(playerNo, mbev_CapBomheiMove);
        odd = GwPlayer[playerNo].moveNum & 1;
        num = odd + GwPlayer[playerNo].moveNum / 2;
        if (!_CheckFlag(0x20001)) {
            if (num < 0) {
                num = 0;
            }
            GwPlayer[playerNo].moveNum = num;
        } else {
            if (num < 1) {
                num = 1;
            }
            GwPlayer[playerNo].moveNum = num;
        }
        if (GwPlayer[playerNo].moveNum <= 1 && !_CheckFlag(0x20001)) {
            mbPlayerMotionShiftSet(playerNo, 15, 0.0f, 8.0f, 0);
            mbPlayerColSnapPlayerSet(playerNo, FALSE);
            mbMoveNumDispSet(playerNo, FALSE);
            mbPlayerRotGet(playerNo, &rot);
            for (i = 1; i <= 36.0f; i++) {
                t = i / 36.0f;
                mbMasuPosGet(GwPlayer[playerNo].masuId, &masuPos);
                pos.x = masuPos.x;
                pos.y = masuPos.y + 3.0 * (100.0 * HuSin(180.0f * t));
                pos.z = masuPos.z;
                mbPlayerPosSetV(playerNo, &pos);
                mbPlayerRotSet(playerNo, 0.0f, rot.y + 720.0f * t, 0.0f);
                HuPrcVSleep();
            }
            mbPlayerColSnapPlayerSet(playerNo, TRUE);
            mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 8.0f, 0x40000001);
        }
    }
}

void mbev_CapBomheiMove(int playerNo)
{
    HuVecF startPos;
    HuVecF endPos;
    HuVecF pos;
    HuVecF rot;
    HuVecF rotStart;
    HuVecF camPos;
    HuVecF dir;
    int masuPrev;
    float mag;
    float t;
    float rotDir;
    int steps;
    int masuNext;
    int i;
    int objId;

    steps = 1.75f * mbPlayerWalkSpeedGet();
    masuPrev = GwPlayer[playerNo].masuId;
    masuNext = GwPlayer[playerNo].masuIdNext;
    mbPlayerPosGet(playerNo, &startPos);
    mbMasuPosGet(masuNext, &endPos);
    VECSubtract(&endPos, &startPos, &dir);
    rot.y = HuAtan(dir.x, dir.z);
    rot.x = rot.z = 0.0f;
    rotStart = rot;
    mag = VECMag(&dir);
    if (mbRandMod(0x8000) & 1) {
        rotDir = 1.0f;
    } else {
        rotDir = -1.0f;
    }
    objId = mbObjCreate(0xC0044, NULL, FALSE);
    camPos = startPos;
    camPos.y += 100.0f;
    mbObjPosSetV(objId, &camPos);
    mbObjDispSet(objId, FALSE);
    mbCameraFocusObjSet(objId);
    mbPlayerMotionSet(playerNo, 15, 0x40000001);
    mbPlayerColSnapPlayerSet(playerNo, FALSE);
    for (i = 1; i < steps; i++) {
        t = (float)i / (float)steps;
        mbMasuPosGet(masuNext, &endPos);
        pos.x = startPos.x + t * (endPos.x - startPos.x);
        pos.y = startPos.y + t * (endPos.y - startPos.y) + 300.0 * HuSin(180.0f * t);
        pos.z = startPos.z + t * (endPos.z - startPos.z);
        rot.y = rotStart.y + 720.0f * t * rotDir;
        mbPlayerPosSetV(playerNo, &pos);
        mbPlayerRotSetV(playerNo, &rot);
        mbPlayerWorkGet(playerNo)->masuNext = steps - i;
        camPos.x = startPos.x + t * (endPos.x - startPos.x);
        camPos.y = 100.0f + (startPos.y + t * (endPos.y - startPos.y));
        camPos.z = startPos.z + t * (endPos.z - startPos.z);
        mbObjPosSetV(objId, &camPos);
        HuPrcVSleep();
    }
    pos = endPos;
    rot = rot;
    mbPlayerPosSetV(playerNo, &pos);
    mbPlayerRotSetV(playerNo, &rot);
    mbPlayerWorkGet(playerNo)->masuNext = 0;
    if (GwPlayer[playerNo].moveNum > 1 || _CheckFlag(0x20001)) {
        mbMoveNumDispSet(playerNo, TRUE);
    }
    if (!mbMasuDispCheck(masuNext)) {
        mbMoveNumDispSet(playerNo, TRUE);
    }
    mbCameraFocusPlayerSet(playerNo);
    mbObjKill(objId);
}
/* @@END@@ */
