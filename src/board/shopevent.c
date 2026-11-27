#include "game/board/shopevent.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/capevent.h"
#include "game/board/capselect.h"
#include "game/board/capsule.h"
#include "game/board/coin.h"
#include "game/board/comchoice.h"
#include "game/board/exit.h"
#define mbMasuFind_TypeStepGet mbMasuFind_TypeStepGet_s16
#include "game/board/masu.h"
#undef mbMasuFind_TypeStepGet
#include "game/board/math.h"
#include "game/board/object.h"
#include "game/board/pause.h"
#include "game/board/player.h"
#include "game/board/scroll.h"
#include "game/board/status.h"
#include "game/board/telop.h"
#include "game/board/tutorial.h"
#include "game/board/window.h"
#include "game/data.h"
#include "game/esprite.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/sprite.h"
#include "humath.h"
#include "math.h"
#include "stdio.h"
#include "string.h"

/* masu.h: retail uses the result as an int */
int mbMasuFind_TypeStepGet(s16 id, s16 type);

typedef struct ShopWork_s {
    /* 0x00 */ int objId;
    /* 0x04 */ int no;
    /* 0x08 */ int masuId;
    /* 0x0C */ int linkMasuId;
    /* 0x10 */ BOOL pathF;
    /* 0x14 */ int pathMasuId;
    /* 0x18 */ int unk18;
    /* 0x1C */ int unk1C;
    /* 0x20 */ BOOL motF;
    /* 0x24 */ BOOL openWaitF;
    /* 0x28 */ BOOL openF;
    /* 0x2C */ int backObjId[8];
    /* 0x4C */ int backType[8];
    /* 0x6C */ BOOL backDispF[8];
    /* 0x8C */ HuVecF shopPos;
    /* 0x98 */ HuVecF objPos;
    /* 0xA4 */ HuVecF frontPos;
} SHOPWORK; /* size 0xB0 */

typedef struct ShopEvWork_s {
    int playerNo;
    int masuId;
} SHOPEVWORK;

typedef struct ShopItem_s {
    int capsuleNo;
    int cost;
    u32 mes;
    char costStr[16];
} SHOPITEM; /* size 0x1C */

static void ev_ShopOMExec(OMOBJ *obj);
static void ev_ShopOpenSet(int no, BOOL openF);
static void ev_Shop(SHOPEVWORK *evWork);
static int ev_ShopSelect(SHOPEVWORK *evWork, SHOPITEM *item, int itemNum, int speakerNo);
static u32 ev_ShopMesGet(u32 mes);

static HuVecF ev_ShopCapsulePlayer[3][3] = {
    { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
    { { 90.0f, 50.0f, 0.0f }, { -90.0f, 50.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
    { { 90.0f, 50.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { -90.0f, 50.0f, 0.0f } },
};
static HuVecF ev_ShopLightPos = { -10000.0f, 10000.0f, -10000.0f };
static HuVecF ev_ShopLightDir = { 1.0f, -1.0f, -1.0f };
static GXColor ev_ShopLightColor = { 255, 255, 255, 255 };
static int ev_ShopSprFileTbl[4] = { 0x50026, 0x50023, 0x50023, 0x50023 };
static HuVecF ev_ShopWinPos = { 288.0f, 176.0f, 0.0f };
static HuVecF ev_ShopCapsulePos[3][3] = {
    { { 288.0f, 170.0f, 1000.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
    { { 214.0f, 170.0f, 1000.0f }, { 362.0f, 170.0f, 1000.0f }, { 0.0f, 0.0f, 0.0f } },
    { { 192.0f, 170.0f, 1000.0f }, { 288.0f, 170.0f, 1000.0f }, { 384.0f, 170.0f, 1000.0f } },
};

static OMOBJ *ev_ShopOMObj[3];

/* .sbss: emitted in reverse definition order */
static int ev_ShopNum;
static MBEV_SHOPHOOK ev_ShopExObjHook;
static BOOL ev_ShopEnableF;

static inline BOOL GWTeamGet(void)
{
    return GwSystem.tagF;
}

static inline SHOPWORK *ev_ShopWorkGet(int no)
{
    OMOBJ *obj = ev_ShopOMObj[no];
    return obj->data;
}

static inline int ev_ShopNoGet(int masuId)
{
    int i;
    for(i = 0; i < ev_ShopNum; i++) {
        OMOBJ *obj = ev_ShopOMObj[i];
        SHOPWORK *work = obj->data;
        if(work->masuId == masuId) {
            return i;
        }
    }
    return -1;
}

static inline void ev_ShopObjLoopSet(MBOBJID objId)
{
    mbObjAttrSet(objId, HU3D_MOTATTR_LOOP);
}

void mbev_ShopEnableSet(BOOL enableF)
{
    ev_ShopEnableF = enableF;
}

void mbev_ShopInit(int dataNum)
{
    mbev_ShopExObjHookSet(NULL);
    mbev_ShopCreate(dataNum, -1);
}

void mbev_ShopExInit(int dataNum, MBEV_SHOPHOOK hook)
{
    mbev_ShopExObjHookSet(hook);
    mbev_ShopCreate(dataNum, -1);
}

void mbev_ShopCreate(int dataNum, int motDataNum)
{
    int i;
    int j;
    int linkMasuId;
    int frontMasuId;
    int nextMasuId;
    int no;
    int masuId;
    SHOPWORK *work;
    OMOBJ *obj;
    HuVecF masuPos;
    HuVecF frontPos;
    HuVecF dir;
    int path[3];
    int motTbl[2];

    ev_ShopEnableF = TRUE;
    for(i = 0; i < 3; i++) {
        ev_ShopOMObj[i] = NULL;
    }
    no = 0;
    motTbl[0] = motDataNum;
    motTbl[1] = -1;
    for(i = 1; i < mbMasuNumGet(); i++) {
        if(mbMasuTypeGet(i) != 9) {
            continue;
        }
        linkMasuId = mbMasuAttrFindLink(i, 0x20);
        if(linkMasuId < 0) {
            break;
        }
        nextMasuId = mbMasuAttrFindLink(linkMasuId, 0x20);
        work = HuMemDirectMallocNum(HEAP_HEAP, sizeof(SHOPWORK), HU_MEMNUM_OVL);
        memset(work, 0, sizeof(SHOPWORK));
        masuId = i;
        frontMasuId = linkMasuId;
        mbMasuPosGet(masuId, &masuPos);
        mbMasuPosGet(frontMasuId, &frontPos);
        if(nextMasuId != -1) {
            work->pathF = TRUE;
            path[0] = masuId;
            path[1] = frontMasuId;
            path[2] = nextMasuId;
            while((nextMasuId = mbMasuAttrFindLink(path[2], 0x20)) > 0) {
                path[0] = path[1];
                path[1] = path[2];
                path[2] = nextMasuId;
            }
            mbMasuPosGet(path[1], &work->frontPos);
            mbMasuPosGet(path[2], &work->shopPos);
            masuPos = work->frontPos;
            frontPos = work->shopPos;
            work->pathMasuId = path[1];
        } else {
            work->pathF = FALSE;
            work->pathMasuId = -1;
            PSVECSubtract(&masuPos, &frontPos, &dir);
            if(PSVECMag(&dir) > 0.0f) {
                PSVECNormalize(&dir, &dir);
            }
            PSVECScale(&dir, &dir, 100.0f);
            PSVECAdd(&frontPos, &dir, &work->frontPos);
            work->frontPos.y = frontPos.y;
            mbMasuPosGet(frontMasuId, &work->shopPos);
        }
        PSVECSubtract(&frontPos, &masuPos, &dir);
        if(PSVECMag(&dir) > 0.0f) {
            PSVECNormalize(&dir, &dir);
        }
        PSVECScale(&dir, &dir, 100.0f);
        PSVECAdd(&frontPos, &dir, &work->objPos);
        if(GwSystem.curTime) {
            work->objPos.y += 30.000002f;
        }
        obj = ev_ShopOMObj[no] = omAddObjEx(mbObjMan, 0x8000, 0, 0, -1, ev_ShopOMExec);
        obj->data = work;
        if(dataNum <= 0) {
            work->objId = -1;
        } else if(motDataNum != -1) {
            work->objId = mbObjCreate(dataNum, motTbl, TRUE);
            work->motF = TRUE;
        } else {
            work->objId = mbObjCreate(dataNum, NULL, TRUE);
            work->motF = FALSE;
        }
        PSVECSubtract(&masuPos, &frontPos, &dir);
        if(work->objId != -1) {
            mbObjPosSetV(work->objId, &frontPos);
            mbObjRotSet(work->objId, 0.0f, HuAtan(dir.x, dir.z), 0.0f);
            mbObjMotionSpeedSet(work->objId, 0.0f);
        }
        work->no = no;
        work->masuId = masuId;
        work->linkMasuId = frontMasuId;
        work->unk18 = 0;
        work->unk1C = 0;
        work->openWaitF = FALSE;
        work->openF = FALSE;
        if(ev_ShopExObjHook != NULL) {
            ev_ShopExObjHook(work->objId, no);
        }
        for(j = 0; j < 8; j++) {
            work->backObjId[j] = -1;
            work->backType[j] = 0;
            work->backDispF[j] = FALSE;
        }
        no++;
    }
    ev_ShopNum = no;
}

void mbev_ShopExObjHookSet(MBEV_SHOPHOOK hook)
{
    ev_ShopExObjHook = hook;
}

void mbev_ShopBackCreate(int dataNum, int motDataNum, int type, BOOL linkF)
{
    mbev_ShopBackMotCreate(dataNum, motDataNum, type, linkF, NULL);
}

void mbev_ShopBackMotCreate(int dataNum, int motDataNum, int type, BOOL linkF, char *hookName)
{
    int i;
    int j;
    OMOBJ *obj;
    SHOPWORK *work;
    int motTbl[16];
    HuVecF pos;
    HuVecF rot;

    for(i = 0; i < 3; i++) {
        obj = ev_ShopOMObj[i];
        if(obj == NULL) {
            continue;
        }
        work = obj->data;
        for(j = 0; j < 8; j++) {
            if(work->backObjId[j] == -1) {
                break;
            }
        }
        if(motDataNum >= 0) {
            motTbl[0] = motDataNum;
            motTbl[1] = -1;
            work->backObjId[j] = mbObjCreate(dataNum, motTbl, linkF);
        } else {
            work->backObjId[j] = mbObjCreate(dataNum, NULL, linkF);
        }
        if(work->objId != -1) {
            mbObjPosGet(work->objId, &pos);
            mbObjRotGet(work->objId, &rot);
        } else {
            pos.x = pos.y = pos.z = 0.0f;
            rot.x = rot.y = rot.z = 0.0f;
        }
        if(hookName == NULL) {
            mbObjPosSetV(work->backObjId[j], &pos);
            mbObjRotSetV(work->backObjId[j], &rot);
        } else if(work->objId != -1) {
            mbObjHookSet(work->objId, hookName, work->backObjId[j]);
        }
        work->backType[j] = type;
        switch(work->backType[j]) {
            case 0:
            case 1:
                mbObjMotionSpeedSet(work->backObjId[j], 0.0f);
                break;

            case 2:
            case 3:
                mbObjMotionSpeedSet(work->backObjId[j], 0.0f);
                break;

            case 4:
                ev_ShopObjLoopSet(work->backObjId[j]);
                break;
        }
        if(work->backType[j] == 1 || work->backType[j] == 3) {
            work->backDispF[j] = TRUE;
        }
        if(work->backDispF[j] == FALSE) {
            mbObjDispSet(work->backObjId[j], FALSE);
        }
    }
}

static void ev_ShopOMExec(OMOBJ *obj)
{
    SHOPWORK *work = obj->data;
    int i;
    float maxTime;

    if(mbExitCheck() || ev_ShopOMObj[work->no] == NULL) {
        omDelObjEx(mbObjMan, obj);
    } else if(work->openWaitF && work->objId != -1) {
        if(work->openF) {
            maxTime = mbObjMotionMaxTimeGet(work->objId);
            if(mbObjMotionTimeGet(work->objId) >= maxTime) {
                work->openWaitF = FALSE;
            }
        } else {
            if(work->motF) {
                maxTime = mbObjMotionMaxTimeGet(work->objId);
                if(mbObjMotionTimeGet(work->objId) >= maxTime) {
                    work->openWaitF = FALSE;
                }
            } else {
                if(mbObjMotionTimeGet(work->objId) <= 0.0f) {
                    work->openWaitF = FALSE;
                }
            }
            if(!work->openWaitF) {
                for(i = 0; i < 8; i++) {
                    if(work->backObjId[i] != -1 && !work->backDispF[i]) {
                        mbObjDispSet(work->backObjId[i], FALSE);
                    }
                }
            }
        }
    }
}

static void ev_ShopOpenSet(int no, BOOL openF)
{
    OMOBJ *obj = ev_ShopOMObj[no];
    SHOPWORK *work = obj->data;
    int i;

    if(work->objId == -1) {
        return;
    }
    work->openWaitF = TRUE;
    work->openF = openF;
    if(work->openF) {
        mbAudFXPlay(0x477);
        mbObjMotionSet(work->objId, 0, 0);
        mbObjMotionSpeedSet(work->objId, 1.0f);
        for(i = 0; i < 8; i++) {
            if(work->backObjId[i] == -1) {
                continue;
            }
            switch(work->backType[i]) {
                case 0:
                case 1:
                case 2:
                case 3:
                    mbObjMotionTimeSet(work->backObjId[i], 0.0f);
                    mbObjMotionSpeedSet(work->backObjId[i], 1.0f);
                    mbObjDispSet(work->backObjId[i], TRUE);
                    break;

                default:
                    mbObjDispSet(work->backObjId[i], TRUE);
                    break;
            }
        }
    } else {
        mbAudFXPlay(0x478);
        if(work->motF) {
            mbObjMotionSet(work->objId, 1, 0);
            mbObjMotionSpeedSet(work->objId, 1.0f);
        } else {
            mbObjMotionSpeedSet(work->objId, -1.0f);
        }
        for(i = 0; i < 8; i++) {
            if(work->backObjId[i] == -1) {
                continue;
            }
            switch(work->backType[i]) {
                case 0:
                case 1:
                    mbObjMotionSpeedSet(work->backObjId[i], -1.0f);
                    break;

                case 2:
                case 3:
                    mbObjMotionTimeSet(work->backObjId[i], 0.0f);
                    mbObjMotionSpeedSet(work->backObjId[i], 1.0f);
                    break;
            }
        }
    }
}

int mbev_Shop(int playerNo, int masuId)
{
    void *buf;
    SHOPEVWORK *evWork;

    mbMoveNumDispSet(playerNo, FALSE);
    buf = HuMemDirectMallocNum(HEAP_HEAP, sizeof(SHOPEVWORK), HU_MEMNUM_OVL);
    evWork = buf;
    memset(evWork, 0, sizeof(SHOPEVWORK));
    evWork->playerNo = playerNo;
    evWork->masuId = masuId;
    mbPauseDisableSet(TRUE);
    ev_Shop(evWork);
    HuMemDirectFree(evWork);
    mbPauseDisableSet(FALSE);
    mbMoveNumDispSet(playerNo, TRUE);
    return 0;
}

static void ev_Shop(SHOPEVWORK *evWork)
{
}

static int ev_ShopSelect(SHOPEVWORK *evWork, SHOPITEM *item, int itemNum, int speakerNo)
{
    return 0;
}

static u32 ev_ShopMesGet(u32 mes)
{
    if(!GwSystem.curTime) {
        return mes;
    }
    return mes + 14;
}
