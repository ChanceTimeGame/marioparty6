/* @@HEAD@@ */
#include "game/board/capevent.h"
#include "game/board/capsule.h"
#include "game/board/capmove.h"
#include "game/board/capthrow.h"
#include "game/board/captrap.h"
#include "game/board/capspecial.h"
#include "game/board/shopevent.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/exit.h"
#define mbMasuCapsuleGet mbMasuCapsuleGet_s16
#include "game/board/masu.h"
#include "game/board/math.h"
#undef mbMasuCapsuleGet
int mbMasuCapsuleGet(s16 id);
#include "game/board/object.h"
#include "game/board/player.h"
#include "game/board/status.h"
#include "game/board/telop.h"
#include "game/board/window.h"
#include "game/gamework.h"
#include "game/memory.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/data.h"
#include "game/sprite.h"
#include "game/esprite.h"
#include "game/object.h"
#include "game/process.h"
#include "game/pad.h"
#include "game/printfunc.h"

#include "string.h"

/* Later-defined statics the early parts call. */
static void ev_CapCoinAdd(void *eff, int playerNo, int coinNum, int arg3, void (*winFunc)(void));
static void ev_CapCall(CAPWORK *work, BOOL waitF);
static void ev_CapWorkInit(CAPOBJWORK *objWork, int capsuleNo);
static void ev_CapWorkClose(CAPOBJWORK *objWork);

static inline void *CapMalloc(s32 size)
{
    return HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
}

static inline void CapFree(void *ptr)
{
    HuMemDirectFree(ptr);
}

static CAPEVENT_DATA ev_CapsuleData[60] = {
    { mbev_CapKinoko, mbev_CapKinokoKill, NULL, 2, 0, -1, 0 },
    { mbev_CapSKinoko, mbev_CapSKinokoKill, NULL, 2, 0, -1, 0 },
    { mbev_CapPKinoko, mbev_CapPKinokoKill, NULL, 2, 0, -1, 0 },
    { mbev_CapMKinoko, mbev_CapMKinokoKill, NULL, 2, 0, -1, 0 },
    { mbev_CapKiller, mbev_CapKillerKill, NULL, 2, 0, -1, 0 },
    { mbev_CapDokan, mbev_CapDokanKill, NULL, 2, 0, 0xD0000, 0 },
    { mbev_CapHanachan, mbev_CapHanachanKill, NULL, 2, 0, 0xD0000, 0 },
    { mbev_CapNKinoko, mbev_CapNKinokoKill, NULL, 2, 0, -1, 0 },
    { NULL, NULL, NULL, 0, 0, -1, 0 },
    { NULL, NULL, NULL, 0, 0, -1, 0 },
    { mbev_CapTogezo, mbev_CapTogezoKill, NULL, 1, 1, 0xF0000, 0 },
    { mbev_CapKuribo, mbev_CapKuriboKill, NULL, 1, 1, 0xF0000, 0 },
    { mbev_CapPakkun, mbev_CapPakkunKill, NULL, 1, 2, 0xF0000, 0 },
    { mbev_CapJango, mbev_CapJangoKill, NULL, 1, 1, 0xF0000, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { mbev_CapKokamekku, mbev_CapKokamekkuKill, NULL, 1, 1, 0xF0000, 0 },
    { mbev_CapKamekku, mbev_CapKamekkuKill, NULL, 1, 1, 0xF0000, 0 },
    { mbev_CapThrowman, mbev_CapThrowmanKill, NULL, 1, 1, 0xF0000, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { mbev_CapBoble, mbev_CapBobleKill, (void (*)(CAPWORK *))mbev_CapBobleTrap, 1, 1, -1, 1 },
    { mbev_CapBiriQ, mbev_CapBiriQKill, (void (*)(CAPWORK *))mbev_CapBiriQTrap, 0, 1, -1, 1 },
    { mbev_CapTumujikun, mbev_CapTumujikunKill, (void (*)(CAPWORK *))mbev_CapTumujikunTrap, 0, 1, -1, 1 },
    { mbev_CapDossun, mbev_CapDossunKill, (void (*)(CAPWORK *))mbev_CapDossunTrap, 1, 1, -1, 1 },
    { mbev_CapBomhei, mbev_CapBomheiKill, (void (*)(CAPWORK *))mbev_CapBomheiTrap, 1, 1, -1, 1 },
    { mbev_CapPatapata, mbev_CapPatapataKill, NULL, 1, 1, 0xF0000, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { mbev_CapNull, mbev_CapNullKill, NULL, 0, 1, -1, 0 },
    { mbev_CapNull, mbev_CapNullKill, NULL, 1, 1, -1, 0 },
    { mbev_CapNull, mbev_CapNullKill, NULL, 1, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { mbev_CapKillerMove, mbev_CapKillerMoveKill, NULL, 2, 1, -1, 0 },
    { mbev_CapKettou, mbev_CapKettouKill, NULL, 0, 1, 0x110000, 0 },
    { mbev_CapMiracle, mbev_CapMiracleKill, NULL, 0, 1, 0x110000, 0 },
    { mbev_CapKoopa, mbev_CapKoopaKill, NULL, 2, 0, 0xE0000, 0 },
    { mbev_CapDonkey, mbev_CapDonkeyKill, NULL, 0, 1, 0xE0000, 0 },
    { mbev_CapNull, mbev_CapNullKill, NULL, 0, 1, -1, 0 },
    { mbev_CapTeresa, mbev_CapTeresaKill, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { mbev_CapDebugCam, mbev_CapDebugCamKlll, NULL, 0, 1, -1, 0 },
    { mbev_CapDebugWarp, mbev_CapDebugWarpKill, NULL, 0, 1, -1, 0 },
    { mbev_CapDebugPosSelect, mbev_CapDebugPosSelectKill, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
};

/* Unreferenced in this unit. */
static HuVecF ev_CapsuleUnusedOfs[10] = {
    { 0.0f, -10.0f, -20.0f },
    { 0.0f, -10.0f, -20.0f },
    { 0.0f, -30.000002f, -20.0f },
    { 0.0f, -10.0f, -20.0f },
    { 0.0f, -10.0f, -20.0f },
    { 0.0f, 10.0f, -10.0f },
    { 0.0f, -30.000002f, -20.0f },
    { 0.0f, -10.0f, -20.0f },
    { 0.0f, -10.0f, -20.0f },
    { 0.0f, -10.0f, -20.0f },
};

static GXColor ev_CapsuleRandomColorTbl[7] = {
    { 255, 127, 127, 255 },
    { 255, 127, 64, 255 },
    { 255, 255, 127, 255 },
    { 127, 255, 127, 255 },
    { 127, 127, 255, 255 },
    { 64, 64, 255, 255 },
    { 255, 127, 255, 255 },
};

static HuVecF ev_CapsuleViewOfs = { 0.0f, 100.0f, 0.0f };

/* Defined at the end of PART1 (after mbev_CapDebugCam's strings in .data, after "%d" in .sdata). */
static s32 ev_CapEffRingFile[6];
static GXColor ev_CapEffElectricColor[4];
static HuVecF basePos[4];
static float baseST1[8];
static float baseST2[8];
static GXColor capsuleRingColor;

static int kettouCoinLose = 10;
static int kettouOppCoinLose = 5;
static int capsuleMasuType = -1;
static int capsuleMasuId = -1;
static int capsulePlayer = -1;
static int capsuleMasuPlayer = -1;
static int capsuleEventMasu = -1;
static int capsuleEventPlayer = -1;
static int capsuleEventPrevMasu = -1;
static int capsuleEventPrevPlayer = -1;

static HUPROCESS *ev_CapBonusCoinProc[4];
static char ev_CapBonusCoinMes[16];
static HUPROCESS *ev_CapMainProc[8];
static void *ev_CapEffExplodeOMObj[8];
static void *ev_CapEffBoostOMObj[8];
static void *ev_CapEffSnowOMObj[8];
static void *ev_CapEffGlowOMObj[8];
static void *ev_CapEffRingOMObj[8];
static void *ev_CapEffElectricOMObj[8];
static void *ev_CapEffRayOMObj[8];
static void *ev_CapEffMasuHitOMObj[8];
static void *ev_CapEffCoinOMObj[8];
static void *ev_CapEffCoinManOMObj[8];
static void *ev_CapEffStarManOMObj[8];
static void *ev_CapEffCapLoseOMObj[8];
static void *ev_CapEffMoveOMObj[8];

/* .sbss comes out in reverse definition order. */
static int capsuleChoice;
static int ev_CapKillFlag; /* lbl_802C0FD8: cleared by ev_CapKill, read nowhere */
static int bonusCoinNum;
static int bonusCoinWinId;
static CAPHOOK capsuleHook;
s16 *mbCapEffData;
int mbCapEffNum;
static ANIMDATA *electricEffAnim;
static ANIMDATA *ringHitEffAnim2;
static ANIMDATA *ringHitEffAnim1;
static ANIMDATA *boostEffAnim;
static int biriQMasuNum;

/* @@PART1@@ */
int mbev_CapCall(int playerNo, int capsuleNo, BOOL trapF, BOOL moveF)
{
    CAPWORK work;
    int viewNo;
    int masuPlayer;
    int masuId;
    BOOL moveNumF = FALSE;

    masuId = GwPlayer[playerNo].masuId;
    if(trapF) {
        masuPlayer = mbCapValuePlayerGet(capsuleNo);
        capsuleNo = mbCapValueTypeGet(capsuleNo);
        if(!mbCapValidCheck(capsuleNo)) {
            return FALSE;
        }
    } else if(moveF) {
        if(playerNo == capsuleEventPlayer && capsuleEventMasu == GwPlayer[playerNo].masuId) {
            capsuleEventMasu = -1;
            capsuleEventPlayer = -1;
            return FALSE;
        }
        capsuleEventMasu = -1;
        capsuleEventPlayer = -1;
        if(playerNo == capsulePlayer && capsuleMasuId == GwPlayer[playerNo].masuId) {
            capsuleNo = (capsuleMasuPlayer << 8) | capsuleMasuType;
        }
        capsuleMasuType = -1;
        capsuleMasuId = -1;
        capsulePlayer = -1;
        capsuleMasuPlayer = -1;
        if(mbMasuTypeGet(masuId) != 1 && mbMasuTypeGet(masuId) != 2) {
            return FALSE;
        }
        masuPlayer = mbCapValuePlayerGet(capsuleNo);
        capsuleNo = mbCapValueTypeGet(capsuleNo);
        if(playerNo < 0 || playerNo >= 4 || !mbCapValidCheck(capsuleNo)) {
            return FALSE;
        }
        if(mbCapUseModeGet(capsuleNo) != 2) {
            return FALSE;
        }
        if(mbev_CapPlayerCheck(masuPlayer, playerNo)) {
            return TRUE;
        }
    } else {
        if(playerNo == capsuleEventPrevPlayer && capsuleEventPrevMasu == GwPlayer[playerNo].masuId) {
            capsuleEventPrevMasu = -1;
            capsuleEventPrevPlayer = -1;
            return FALSE;
        }
        capsuleEventPrevMasu = -1;
        capsuleEventPrevPlayer = -1;
        if(mbMasuTypeGet(masuId) != 1 && mbMasuTypeGet(masuId) != 2) {
            return FALSE;
        }
        masuPlayer = mbCapValuePlayerGet(capsuleNo);
        capsuleNo = mbCapValueTypeGet(capsuleNo);
        if(playerNo < 0 || playerNo >= 4 || !mbCapValidCheck(capsuleNo)) {
            return FALSE;
        }
        if(mbCapUseModeGet(capsuleNo) == 2) {
            return FALSE;
        }
        if(!trapF && !moveF && mbCapValidCheck(capsuleNo)) {
            GwPlayer[playerNo].capsuleMasuNum++;
            if(GwPlayer[playerNo].capsuleMasuNum > 99) {
                GwPlayer[playerNo].capsuleMasuNum = 99;
            }
        }
        if(mbev_CapPlayerCheck(masuPlayer, playerNo)) {
            return TRUE;
        }
    }
    omVibrate(playerNo, 20, 4, 4);
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = capsuleNo;
    work.trapF = trapF;
    work.unk1C = 0;
    work.moveF = moveF;
    work.masuId = GwPlayer[work.playerNo].masuId;
    work.trapMasuId = -1;
    if(!trapF) {
        work.masuPlayer = masuPlayer;
    }
    memset(&work.flag, 0, 4);
    masuId = GwPlayer[work.playerNo].masuId;
    viewNo = mbCameraPlayerViewNoGet();
    mbAudFXPlay(1035);
    if(!trapF && !moveF) {
        mbev_CapEffOpenCreate(work.playerNo, work.masuId, TRUE, TRUE, TRUE);
    }
    if(ev_CapsuleData[capsuleNo].trapFunc != NULL) {
        ev_CapWorkInit(&work.objWork, -1);
        ev_CapsuleData[capsuleNo].trapFunc(&work);
        ev_CapWorkClose(&work.objWork);
    }
    if(moveF && ev_CapsuleData[work.capsuleNo].trapF && mbCapUseModeGet(capsuleNo) == 2) {
        if((capsuleNo == 22 || capsuleNo == 25) && GwPlayer[playerNo].moveNum <= 1) {
            mbev_CapMoveMasuSet(playerNo, GwPlayer[playerNo].masuId);
        }
        return TRUE;
    }
    switch(ev_CapsuleData[work.capsuleNo].cameraMode) {
        case 1:
            mbCameraPlayerViewSet(work.playerNo, 0);
            break;

        case 2:
            mbCameraPlayerViewSet(work.playerNo, 1);
            break;
    }
    switch(ev_CapsuleData[work.capsuleNo].statusMode) {
        case 0:
            if(!mbev_CapStatusDispCheck(work.playerNo)) {
                mbev_CapStatusDispSetAll(0, 1);
                mbStatusDispFocusSet(work.playerNo, TRUE);
                while(!mbStatusMoveCheck(work.playerNo)) {
                    HuPrcVSleep();
                }
            }
            break;

        case 1:
            mbev_CapStatusDispSetAll(1, 0);
            break;

        case 2:
            mbev_CapStatusDispSetAll(0, 0);
            break;
    }
    mbPlayerRotateStart(work.playerNo, 0, 15);
    ev_CapCall(&work, TRUE);
    if(moveNumF) {
        mbMoveNumDispSet(work.playerNo, TRUE);
    }
    mbev_CapStatusDispSetAll(1, 1);
    if(!moveF) {
        mbev_CapCameraViewSet(work.playerNo, viewNo, moveF);
    } else if(GwPlayer[work.playerNo].moveNum > 1) {
        mbCameraPlayerViewSet(work.playerNo, 2);
    } else {
        mbCameraPlayerViewSet(work.playerNo, 0);
    }
    if(masuId != GwPlayer[work.playerNo].masuId || work.flag.unk80F) {
        return FALSE;
    }
    return FALSE;
}

int mbev_CapCallTrap(int playerNo, int masuId, int trapMasuId)
{
    CAPWORK work;
    int masuPlayer = mbCapValuePlayerGet(mbMasuCapsuleGet(trapMasuId));
    if(mbCapMasuDispTypeGet(trapMasuId) != 2) {
        return FALSE;
    }
    if(mbev_CapPlayerCheck(masuPlayer, playerNo)) {
        return TRUE;
    }
    mbAudFXPlay(1035);
    if(GwPlayer[playerNo].biriQF) {
        biriQMasuNum = 60;
    }
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = mbCapMasuTypeGet(trapMasuId);
    work.trapF = TRUE;
    work.unk1C = TRUE;
    work.moveF = FALSE;
    work.masuId = masuId;
    work.trapMasuId = trapMasuId;
    mbev_CapEffOpenCreate(work.playerNo, work.trapMasuId, TRUE, FALSE, FALSE);
    if(ev_CapsuleData[work.capsuleNo].trapF) {
        ev_CapCall(&work, FALSE);
    }
    return TRUE;
}

void mbev_CapCallKettou(int playerNo, int masuId, int kettouF)
{
    CAPWORK work;
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = 41;
    work.trapF = FALSE;
    work.unk1C = FALSE;
    work.moveF = FALSE;
    work.masuId = masuId;
    work.trapMasuId = -1;
    work.flag.kettouF = kettouF;
    HuPrcSleep(10);
    mbAudFXPlay(1104);
    ev_CapCall(&work, TRUE);
}

void mbev_CapCircuitCallKettou(void)
{
}

void mbev_CapCallDonkey(int playerNo)
{
    CAPWORK work;
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = 44;
    work.trapF = FALSE;
    work.unk1C = FALSE;
    work.moveF = FALSE;
    work.masuId = GwPlayer[playerNo].masuId;
    work.trapMasuId = -1;
    omVibrate(playerNo, 20, 7, 3);
    ev_CapCall(&work, TRUE);
}

void mbev_CapCallKoopa(int playerNo)
{
    CAPWORK work;
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = 43;
    work.trapF = FALSE;
    work.unk1C = FALSE;
    work.moveF = FALSE;
    work.masuId = GwPlayer[playerNo].masuId;
    work.trapMasuId = -1;
    ev_CapCall(&work, TRUE);
}

void MBCapsuleStub5(void)
{
}

void MBCapsuleStub6(void)
{
}

void MBCapsuleStub7(void)
{
}

void mbev_CapCallTeresa(int playerNo, int masuId)
{
    CAPWORK work;
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = 46;
    work.trapF = FALSE;
    work.unk1C = FALSE;
    work.moveF = FALSE;
    work.masuId = masuId;
    work.trapMasuId = -1;
    ev_CapCall(&work, TRUE);
}

void mbev_CapCallMiracle(int playerNo, int masuId)
{
    CAPWORK work;
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = 42;
    work.trapF = FALSE;
    work.unk1C = FALSE;
    work.moveF = FALSE;
    work.masuId = masuId;
    work.trapMasuId = -1;
    ev_CapCall(&work, TRUE);
}

static void ev_CapBiriQShockOMExec(OMOBJ *obj);

void mbev_CapBiriQShockCreate(int playerNo)
{
    OMOBJ *obj;
    CAPWORK work;
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = 21;
    work.trapF = FALSE;
    work.unk1C = FALSE;
    work.moveF = FALSE;
    work.masuId = GwPlayer[playerNo].masuId;
    work.trapMasuId = -1;
    biriQMasuNum = 60;
    obj = omAddObjEx(mbObjMan, 32768, 0, 0, -1, ev_CapBiriQShockOMExec);
    mbev_CapBiriQMetalShock(&work);
    biriQMasuNum = 0;
}

int mbev_CapBiriQShockDelayGet(int playerNo)
{
    if(GwPlayer[playerNo].biriQF) {
        return biriQMasuNum;
    } else {
        return 0;
    }
}

static void ev_CapBiriQShockOMExec(OMOBJ *obj)
{
    if(mbExitCheck() || --biriQMasuNum <= 0) {
        biriQMasuNum = 0;
        omDelObjEx(mbObjMan, obj);
        return;
    }
}

static void ev_CapBiriQMetalShock(void);
static void ev_CapBiriQMetalShockDestroy(void);

void mbev_CapBiriQMetalShockCreate(int playerNo)
{
    HUPROCESS *proc;
    void *mem;
    CAPWORK work;
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = 21;
    work.trapF = FALSE;
    work.unk1C = FALSE;
    work.moveF = FALSE;
    work.masuId = GwPlayer[playerNo].masuId;
    work.trapMasuId = -1;
    work.flag.shockNoWaitF = TRUE;
    biriQMasuNum = 0;
    proc = HuPrcChildCreate(ev_CapBiriQMetalShock, 8196, 24576, 0, mbMainProc);
    mem = HuMemDirectMallocNum(HEAP_HEAP, sizeof(CAPWORK), HU_MEMNUM_OVL);
    proc->property = mem;
    memcpy(proc->property, &work, sizeof(CAPWORK));
    HuPrcDestructorSet2(proc, ev_CapBiriQMetalShockDestroy);
}

static void ev_CapBiriQMetalShock(void)
{
    CAPWORK *work = HuPrcCurrentGet()->property;
    mbev_CapBiriQMetalShock(work);
    HuPrcEnd();
}

static void ev_CapBiriQMetalShockDestroy(void)
{
    CAPWORK *work = HuPrcCurrentGet()->property;
    HuMemDirectFree(work);
}

void mbev_CapRandomBonusCoin(int playerNo, int capsuleNo, BOOL waitF)
{
    mbev_CapBonusCoinCall(playerNo, capsuleNo, -1, waitF);
}

void mbev_CapBonusCoinCall(int playerNo, int capsuleNo, int coinNum, BOOL waitF)
{
    int unused = 0;
    int num = 0;
    BOOL partyF = GwSystem.partyF;
    if(!partyF || _CheckFlag(FLAG_BOARD_TUTORIAL)) {
        return;
    }
    if(coinNum > 0) {
        mbev_CapBonusCoin(playerNo, coinNum, waitF, TRUE);
    } else if(coinNum != 0) {
        num = mbCapBonusCoinNumGet(playerNo, capsuleNo);
        if(num > 0) {
            mbev_CapBonusCoin(playerNo, num, waitF, TRUE);
        }
    }
}

typedef struct CapBonusCoinWork_s {
    int playerNo;
    int coinNum;
    int arg3;
} CAPBONUSCOINWORK;

static void ev_CapBonusCoin(void);
static void ev_CapBonusCoinKill(void);
static void ev_CapBonusCoinWin(void);

void mbev_CapBonusCoin(int playerNo, int coinNum, BOOL waitF, int arg3)
{
    HUPROCESS *proc;
    CAPBONUSCOINWORK *work;
    proc = ev_CapBonusCoinProc[playerNo] = HuPrcChildCreate(ev_CapBonusCoin, 8196, 24576, 0, mbMainProc);
    HuPrcDestructorSet2(proc, ev_CapBonusCoinKill);
    work = proc->property = CapMalloc(sizeof(CAPBONUSCOINWORK));
    memset(work, 0, sizeof(CAPBONUSCOINWORK));
    work->playerNo = playerNo;
    work->coinNum = coinNum;
    work->arg3 = arg3;
    if(waitF) {
        while(!mbev_CapBonusCoinCheck(playerNo)) {
            HuPrcVSleep();
        }
    }
}

BOOL mbev_CapBonusCoinCheck(int playerNo)
{
    if(ev_CapBonusCoinProc[playerNo] != NULL) {
        return FALSE;
    } else {
        return TRUE;
    }
}

static void ev_CapBonusCoin(void)
{
    HUPROCESS *proc = HuPrcCurrentGet();
    CAPBONUSCOINWORK *work = proc->property;
    void *eff;
    bonusCoinNum = work->coinNum;
    bonusCoinWinId = -1;
    eff = mbev_CapEffCoinCreate();
    ev_CapCoinAdd(eff, work->playerNo, work->coinNum, work->arg3, ev_CapBonusCoinWin);
    mbev_CapEffCoinKill(eff);
    if(bonusCoinWinId >= 0) {
        mbWinWait(bonusCoinWinId);
    }
    bonusCoinWinId = -1;
    HuPrcEnd();
}

static void ev_CapBonusCoinKill(void)
{
    HUPROCESS *proc = HuPrcCurrentGet();
    CAPBONUSCOINWORK *work = proc->property;
    ev_CapBonusCoinProc[work->playerNo] = NULL;
    HuMemDirectFree(work);
}

static void ev_CapBonusCoinWin(void)
{
    bonusCoinWinId = mbWinCreate(2, 0x370039, -1);
    sprintf(ev_CapBonusCoinMes, "%d", bonusCoinNum);
    mbWinTopInsertMesSet((u32)ev_CapBonusCoinMes, 0);
}

void mbev_CapMoveMasuSet(int playerNo, int masuId)
{
    capsuleEventPlayer = playerNo;
    capsuleEventMasu = masuId;
}

void mbev_CapStopMasuSet(int playerNo, int masuId)
{
    capsuleEventPrevPlayer = playerNo;
    capsuleEventPrevMasu = masuId;
}

void mbev_CapKettouEndCall(int playerNo)
{
    CAPWORK work;
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = 41;
    work.trapF = FALSE;
    work.unk1C = FALSE;
    work.moveF = FALSE;
    work.masuId = GwPlayer[playerNo].masuId;
    work.trapMasuId = -1;
    work.flag.kettouEndF = TRUE;
    mbPlayerColSnapSet(TRUE);
    ev_CapCall(&work, TRUE);
}

void mbev_CapDonkeyEndCall(int playerNo)
{
    CAPWORK work;
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = 44;
    work.trapF = FALSE;
    work.unk1C = FALSE;
    work.moveF = FALSE;
    work.masuId = GwPlayer[playerNo].masuId;
    work.trapMasuId = -1;
    work.flag.donkeyEndF = TRUE;
    mbPlayerColSnapSet(TRUE);
    ev_CapCall(&work, TRUE);
}

void mbev_CapKoopaEndCall(int playerNo)
{
    CAPWORK work;
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = 43;
    work.trapF = FALSE;
    work.unk1C = FALSE;
    work.moveF = FALSE;
    work.masuId = GwPlayer[playerNo].masuId;
    work.trapMasuId = -1;
    work.flag.koopaEndF = TRUE;
    mbPlayerColSnapSet(TRUE);
    ev_CapCall(&work, TRUE);
}

void mbev_CapVsEndCall(void)
{
}

void mbev_CapKillerCall(void)
{
}

void mbev_CapKillerMultiCall(void)
{
}

void mbev_CapKillerMoveCall(int playerNo)
{
    CAPWORK work;
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = 40;
    work.trapF = FALSE;
    work.unk1C = FALSE;
    work.moveF = TRUE;
    work.masuId = GwPlayer[playerNo].masuId;
    work.trapMasuId = -1;
    ev_CapCall(&work, TRUE);
}

void MBCapsuleStub11(void)
{
}

void MBCapsuleStub12(void)
{
}

int mbev_CapKillerMoveCheck(void)
{
    return TRUE;
}

int mbev_CapKillerMoveCheckAll(void)
{
    return TRUE;
}

int mbev_CapCapGet(void)
{
    return -1;
}

u16 mbev_CapBankCoinGet(void)
{
    return GwSystem.bankCoin;
}

int mbev_CapKettouCoinLoseGet(void)
{
    return kettouCoinLose;
}

int mbev_CapKettouOppCoinLoseGet(void)
{
    return kettouOppCoinLose;
}

int mbev_CapKettouCoinLoseGet2(void)
{
    return kettouCoinLose;
}

int mbev_CapKettouOppCoinLoseGet2(void)
{
    return kettouOppCoinLose;
}

void mbev_CapOpeningAdd(void)
{
}

void mbev_CapKoopaAdd(void)
{
}

void mbev_CapBubbleHookSet(CAPHOOK hook)
{
    capsuleHook = hook;
}

void mbev_CapBubbleHookCall(int arg1, int arg2, int arg3, int arg4, int arg5)
{
    if(capsuleHook != NULL) {
        capsuleHook(-1, arg1, arg2, arg3, arg4, arg5);
    }
}

void mbev_CapBubbleHookCallStory(int type, int arg1, int arg2)
{
    if(capsuleHook != NULL) {
        switch(type) {
            case 0:
                capsuleHook(43, arg1, arg2, 0, 0, 0);
                break;

            case 1:
                capsuleHook(-99, arg1, arg2, 0, 0, 0);
                break;
        }
    }
}

void mbev_CapBankCoinInit(void)
{
    GwSystem.bankCoin = 0;
}

void mbev_CapInit(void)
{
    int i;
    for(i = 0; i < 8; i++) {
        ev_CapMainProc[i] = NULL;
    }
    for(i = 0; i < 8; i++) {
        ev_CapEffExplodeOMObj[i] = NULL;
        ev_CapEffBoostOMObj[i] = NULL;
        ev_CapEffSnowOMObj[i] = NULL;
        ev_CapEffGlowOMObj[i] = NULL;
        ev_CapEffRingOMObj[i] = NULL;
        ev_CapEffElectricOMObj[i] = NULL;
        ev_CapEffRayOMObj[i] = NULL;
        ev_CapEffMasuHitOMObj[i] = NULL;
        ev_CapEffCoinOMObj[i] = NULL;
        ev_CapEffCoinManOMObj[i] = NULL;
        ev_CapEffStarManOMObj[i] = NULL;
        ev_CapEffCapLoseOMObj[i] = NULL;
    }
    mbCapListRead();
    capsuleHook = NULL;
    mbev_ShopEnableSet(FALSE);
    mbCapThrowColCreate(-1);
    mbCapThrowHookSet(NULL);
    mbev_CapTeresaStealSet(-1, 0, 0, 0);
    boostEffAnim = HuSprAnimRead(HuDataReadNum(0xC0035, HU_MEMNUM_OVL));
    HuSprAnimLock(boostEffAnim);
    ringHitEffAnim1 = HuSprAnimRead(HuDataReadNum(0xC0038, HU_MEMNUM_OVL));
    HuSprAnimLock(ringHitEffAnim1);
    ringHitEffAnim2 = HuSprAnimRead(HuDataReadNum(0xC0031, HU_MEMNUM_OVL));
    HuSprAnimLock(ringHitEffAnim2);
    electricEffAnim = HuSprAnimRead(HuDataReadNum(0xC0037, HU_MEMNUM_OVL));
    HuSprAnimLock(electricEffAnim);
    mbCapEffNum = 0;
    mbCapEffData = CapMalloc(1024 * sizeof(s16));
    for(i = 0; i < 1024; i++) {
        mbCapEffData[i] = frand() & 0x7FFF;
    }
    capsuleMasuType = -1;
    capsuleMasuId = -1;
    capsulePlayer = -1;
    capsuleMasuPlayer = -1;
    capsuleEventMasu = -1;
    capsuleEventPlayer = -1;
    capsuleEventPrevMasu = -1;
    capsuleEventPrevPlayer = -1;
    for(i = 0; i < 4; i++) {
        ev_CapBonusCoinProc[i] = NULL;
    }
}

static void ev_CapKill(void)
{
    HUPROCESS *proc = HuPrcCurrentGet();
    CAPWORK *work = proc->property;
    void (*killFunc)(void);
    if(work->capsuleNo != -1 && ev_CapsuleData[work->capsuleNo].killFunc != NULL) {
        killFunc = ev_CapsuleData[work->capsuleNo].killFunc;
        killFunc();
    }
    HuDataDirClose(0x130000);
    HuDataDirClose(0xD0000);
    HuDataDirClose(0xE0000);
    HuDataDirClose(0xF0000);
    HuDataDirClose(0x100000);
    HuDataDirClose(0x110000);
    ev_CapKillFlag = 0;
    ev_CapWorkClose(&work->objWork);
    if(!mbExitCheck()) {
        if(work->effExplode != NULL) {
            mbev_CapEffExplodeKill(work->effExplode);
        }
        if(work->effBoost != NULL) {
            mbev_CapEffBoostKill(work->effBoost);
        }
        if(work->effSnow != NULL) {
            mbev_CapEffSnowKill(work->effSnow);
        }
        if(work->effGlow != NULL) {
            mbev_CapEffGlowKill(work->effGlow);
        }
        if(work->effRing != NULL) {
            mbev_CapEffRingKill(work->effRing);
        }
        if(work->effCoin != NULL) {
            mbev_CapEffCoinKill(work->effCoin);
        }
        if(work->effCoinMan != NULL) {
            mbev_CapCoinManKill(work->effCoinMan);
        }
        if(work->effStarMan != NULL) {
            mbev_CapStarManKill(work->effStarMan);
        }
        if(work->effCapLose != NULL) {
            mbev_CapEffCapLoseKill(work->effCapLose);
        }
    }
    ev_CapMainProc[work->procNo] = NULL;
    _ClearFlag(FLAG_BOARD_STAR_RESET);
    HuMemDirectFree(work);
}

static inline BOOL ev_CapMainProcEndCheck(int procNo)
{
    if(ev_CapMainProc[procNo] != NULL) {
        return FALSE;
    } else {
        return TRUE;
    }
}

static void ev_CapCall(CAPWORK *work, BOOL waitF)
{
    int i;
    HUPROCESS *proc;
    CAPWORK *newWork;
    for(i = 0; i < 8; i++) {
        if(ev_CapMainProc[i] == NULL) {
            break;
        }
    }
    newWork = CapMalloc(sizeof(CAPWORK));
    memset(newWork, 0, sizeof(CAPWORK));
    newWork->playerNo = work->playerNo;
    newWork->masuPlayer = work->masuPlayer;
    newWork->capsuleNo = work->capsuleNo;
    newWork->trapF = work->trapF;
    newWork->unk1C = work->unk1C;
    newWork->moveF = work->moveF;
    newWork->masuId = work->masuId;
    newWork->trapMasuId = work->trapMasuId;
    newWork->procNo = i;
    memcpy(&newWork->flag, &work->flag, 4);
    ev_CapWorkInit(&newWork->objWork, newWork->capsuleNo);
    mbev_CapWait(newWork);
    if(ev_CapsuleData[work->capsuleNo].func != NULL) {
        proc = HuPrcChildCreate(ev_CapsuleData[work->capsuleNo].func, 8196, 24576, 0, mbMainProc);
        ev_CapMainProc[i] = proc;
        proc->property = newWork;
        HuPrcDestructorSet2(proc, ev_CapKill);
        if(waitF) {
            do {
                HuPrcVSleep();
            } while(!ev_CapMainProcEndCheck(newWork->procNo));
            while(mbMusBoardFadeCheck()) {
                HuPrcVSleep();
            }
        }
    }
}

void mbev_CapWait(CAPWORK *work)
{
    CAPOBJWORK *objWork = &work->objWork;
    if(objWork != NULL) {
        if(objWork->bgStatId >= 0) {
            mbBGReadWait(objWork->bgStatId);
            objWork->bgStatId = -1;
        }
        mbCameraMoveWait();
    }
}

static void ev_CapWorkOMExec(OMOBJ *obj);

static void ev_CapWorkInit(CAPOBJWORK *objWork, int capsuleNo)
{
    int i;
    int j;
    memset(objWork, 0, sizeof(CAPOBJWORK));
    for(i = 0; i < 64; i++) {
        for(j = 0; j < 4; j++) {
            objWork->playerMot[i][j] = -1;
        }
        objWork->objId[i] = -1;
        objWork->sprId[i] = -1;
        objWork->mem[i] = NULL;
        objWork->objMasuId[i] = -1;
        objWork->objOfs[i].x = objWork->objOfs[i].y = objWork->objOfs[i].z = 0.0f;
    }
    for(i = 0; i < 4; i++) {
        objWork->playerMasuId[i] = -1;
        objWork->playerOfs[i].x = objWork->playerOfs[i].y = objWork->playerOfs[i].z = 0.0f;
    }
    objWork->obj = omAddObjEx(mbObjMan, 32767, 0, 0, -1, ev_CapWorkOMExec);
    objWork->obj->data = objWork;
    if(capsuleNo >= 0) {
        if(capsuleNo < 0) {
            objWork->bgStatId = -1;
        } else if(ev_CapsuleData[capsuleNo].bgDataNum != -1) {
            objWork->bgStatId = mbBGRead(ev_CapsuleData[capsuleNo].bgDataNum);
        } else {
            objWork->bgStatId = -1;
        }
    }
}

static void ev_CapWorkClose(CAPOBJWORK *objWork)
{
    int i;
    int j;
    if(objWork != NULL) {
        if(objWork->obj != NULL) {
            objWork->obj->work[0] = 1;
        }
        for(i = 0; i < 64; i++) {
            if(!mbExitCheck()) {
                for(j = 0; j < 4; j++) {
                    if(objWork->playerMot[i][j] != -1) {
                        mbPlayerMotionKill(j, objWork->playerMot[i][j]);
                    }
                }
            }
            if(objWork->objId[i] != -1) {
                mbObjKill(objWork->objId[i]);
            }
            if(objWork->sprId[i] != -1) {
                espKill(objWork->sprId[i]);
            }
            if(objWork->mem[i] != NULL) {
                CapFree(objWork->mem[i]);
            }
        }
    }
}

static void ev_CapWorkOMExec(OMOBJ *obj)
{
    CAPOBJWORK *objWork = obj->data;
    HuVecF pos;
    int i;
    if(mbExitCheck() || obj->work[0]) {
        obj->data = NULL;
        omDelObjEx(mbObjMan, obj);
        return;
    }
    for(i = 0; i < 64; i++) {
        if(objWork->objId[i] != -1 && objWork->objMasuId[i] != -1) {
            mbMasuPosGet(objWork->objMasuId[i], &pos);
            VECAdd(&pos, &objWork->objOfs[i], &pos);
            mbObjPosSetV(objWork->objId[i], &pos);
        }
    }
    for(i = 0; i < 4; i++) {
        if(objWork->playerMasuId[i] != -1) {
            mbMasuPosGet(objWork->playerMasuId[i], &pos);
            VECAdd(&pos, &objWork->playerOfs[i], &pos);
            mbPlayerPosSetV(i, &pos);
        }
    }
}

s16 mbev_CapPlayerMotionCreate(CAPOBJWORK *objWork, int playerNo, int dataNum)
{
    int i;
    for(i = 0; i < 64; i++) {
        if(objWork->playerMot[i][playerNo] == -1) {
            break;
        }
    }
    if(i >= 64) {
        return -1;
    }
    objWork->playerMot[i][playerNo] = mbPlayerMotionCreate(playerNo, dataNum);
    HuPrcVSleep();
    return objWork->playerMot[i][playerNo];
}

int mbev_CapObjCreate(CAPOBJWORK *objWork, int dataNum, int *motDataNum, BOOL linkF, int sleepTime, BOOL dirCloseF)
{
    int i;
    int j;
    for(i = 0; i < 64; i++) {
        if(objWork->objId[i] == -1) {
            break;
        }
    }
    if(i >= 64) {
        return -1;
    }
    objWork->objId[i] = mbObjCreate(dataNum, NULL, linkF);
    if(dirCloseF) {
        HuDataDirClose(dataNum);
    }
    mbObjDispSet(objWork->objId[i], FALSE);
    if(sleepTime > 0) {
        HuPrcSleep(sleepTime);
    }
    if(objWork->objId[i] != -1 && motDataNum != NULL) {
        for(j = 0; motDataNum[j] != -1; j++) {
            mbObjMotionCreate(objWork->objId[i], motDataNum[j]);
            if(sleepTime > 0) {
                HuPrcVSleep();
            }
        }
    }
    mbObjDispSet(objWork->objId[i], TRUE);
    mbObjLayerSet(objWork->objId[i], 3);
    return objWork->objId[i];
}

void mbev_CapObjPosSet(CAPOBJWORK *objWork, int objId, int masuId, HuVecF *ofs)
{
    int i;
    for(i = 0; i < 64; i++) {
        if(objWork->objId[i] == objId) {
            objWork->objMasuId[i] = masuId;
            if(ofs != NULL) {
                objWork->objOfs[i] = *ofs;
            } else {
                objWork->objOfs[i].x = objWork->objOfs[i].y = objWork->objOfs[i].z = 0.0f;
            }
        }
    }
}

void mbev_CapPlayerPosSet(CAPOBJWORK *objWork, int playerNo, int masuId, HuVecF *ofs)
{
    objWork->playerMasuId[playerNo] = masuId;
    if(ofs != NULL) {
        objWork->playerOfs[playerNo] = *ofs;
    } else {
        objWork->playerOfs[playerNo].x = objWork->playerOfs[playerNo].y = objWork->playerOfs[playerNo].z = 0.0f;
    }
}

void mbev_CapObjClose(CAPOBJWORK *objWork, int objId)
{
    int i;
    if(objId != -1) {
        for(i = 0; i < 64; i++) {
            if(objWork->objId[i] == objId) {
                mbObjKill(objId);
                objWork->objId[i] = -1;
            }
        }
    }
}

s16 mbev_CapSprCreate(CAPOBJWORK *objWork, unsigned int dataNum, s16 prio, s16 bank)
{
    int i;
    for(i = 0; i < 64; i++) {
        if(objWork->sprId[i] == -1) {
            break;
        }
    }
    if(i >= 64) {
        return -1;
    }
    objWork->sprId[i] = espEntry(dataNum, prio, bank);
    espDrawNoSet(objWork->sprId[i], 32);
    return objWork->sprId[i];
}

void mbev_CapSprClose(CAPOBJWORK *objWork, s16 sprId)
{
    int i;
    if(sprId != -1) {
        for(i = 0; i < 64; i++) {
            if(objWork->sprId[i] == sprId) {
                espKill(sprId);
                objWork->sprId[i] = -1;
            }
        }
    }
}

void *mbev_CapMalloc(CAPOBJWORK *objWork, s32 size)
{
    int i;
    for(i = 0; i < 64; i++) {
        if(objWork->mem[i] == NULL) {
            break;
        }
    }
    if(i >= 64) {
        return NULL;
    }
    objWork->mem[i] = CapMalloc(size);
    return objWork->mem[i];
}

void mbev_CapMallocClose(CAPOBJWORK *objWork, void *ptr)
{
    int i;
    if(ptr != NULL) {
        for(i = 0; i < 64; i++) {
            if(objWork->mem[i] == ptr) {
                HuMemDirectFree(ptr);
                objWork->mem[i] = NULL;
            }
        }
    }
}

typedef struct CapEffOpenWork_s {
    /* 0x00 */ int masuId;
    /* 0x04 */ int time;
    /* 0x08 */ int openF;
    /* 0x0C */ HuVecF ofs;
} CAPEFFOPENWORK;

static void ev_CapEffOpen(void);
static void ev_CapEffOpenKill(void);

void mbev_CapEffOpenCreate(int playerNo, int masuId, BOOL effF, BOOL openF, BOOL keepF)
{
    HUPROCESS *proc;
    CAPEFFOPENWORK *work;
    int value;
    if(effF) {
        proc = HuPrcChildCreate(ev_CapEffOpen, 8196, 24576, 0, mbMainProc);
        HuPrcDestructorSet2(proc, ev_CapEffOpenKill);
        work = CapMalloc(sizeof(CAPEFFOPENWORK));
        proc->property = work;
        memset(work, 0, sizeof(CAPEFFOPENWORK));
        work->masuId = masuId;
        work->time = 25;
        work->ofs.x = work->ofs.y = work->ofs.z = 0.0f;
        if(!openF) {
            work->ofs.y += 100.0f;
            work->openF = FALSE;
        } else {
            work->openF = TRUE;
        }
    }
    if(openF) {
        if(!keepF) {
            mbMasuCapsuleSet(masuId, -1);
        }
    } else {
        value = mbMasuCapsuleGet(masuId);
        capsuleMasuType = mbCapValueTypeGet(value);
        capsuleMasuId = masuId;
        capsulePlayer = playerNo;
        capsuleMasuPlayer = mbCapValuePlayerGet(value);
        mbMasuCapsuleSet(masuId, -1);
    }
}

#define CAP_RANDF() (0.0000000037252903f*mbRandMod(0x10000000))

static void ev_CapEffOpen(void)
{
    HUPROCESS *proc = HuPrcCurrentGet();
    CAPEFFOPENWORK *work = proc->property;
    HuVecF pos;
    HuVecF effPos;
    HuVecF rot;
    HuVecF vel;
    HuVecF scale;
    GXColor color;
    void *rayEff;
    void *ringEff;
    void *masuHitEff;
    int i;
    int j;
    float alpha;

    rayEff = mbev_CapEffRayCreate(1.0f, 0.0f);
    ringEff = mbev_CapEffRingCreate();
    masuHitEff = mbev_CapEffMasuHitCreate();
    mbMasuPosGet(work->masuId, &pos);
    VECAdd(&pos, &work->ofs, &pos);
    scale.x = scale.z = 0.0f;
    scale.y = 5.0f;
    VECAdd(&pos, &scale, &pos);
    if(work->openF) {
        rot.x = -90.0f;
        rot.y = rot.z = 0.0f;
        scale.x = 1.0f;
        scale.y = 2.0f;
        scale.z = 100.0f;
        mbev_CapEffColorSet(&color, mbRandMod(32768));
        mbev_CapEffRingAdd(ringEff, pos, rot, scale, 1, 10, 0, color);
        scale.x = 2.0f;
        scale.y = 2.5f;
        scale.z = 200.0f;
        mbev_CapEffColorSet(&color, mbRandMod(32768));
        mbev_CapEffRingAdd(ringEff, pos, rot, scale, 3, 10, 1, color);
        for(i = 0; i <= work->time; i++) {
            alpha = HuCos(90.0f*((float)i/work->time));
            mbMasuPosGet(work->masuId, &pos);
            VECAdd(&pos, &work->ofs, &pos);
            for(j = 0; j < 3; j++) {
                effPos = pos;
                rot.x = 180.0f*(-0.5f+CAP_RANDF());
                rot.y = 360.0f*CAP_RANDF();
                rot.z = 180.0f*(-0.5f+CAP_RANDF());
                vel.x = 180.0f*(-0.5f+CAP_RANDF());
                vel.y = rot.y+(60.0f*CAP_RANDF());
                vel.z = 180.0f*(-0.5f+CAP_RANDF());
                mbev_CapEffRayAdd(rayEff, &effPos, &rot, &vel, 200.0f*(1.0f+(0.25f*CAP_RANDF())), 10.0f+(5.0f*CAP_RANDF()));
            }
            mbev_CapEffRayAlphaSet(rayEff, alpha);
            for(j = 0; j < 5.0f*alpha; j++) {
                effPos = pos;
                rot.x = 180.0f*(-0.5f+CAP_RANDF());
                rot.y = 360.0f*CAP_RANDF();
                rot.z = 180.0f*(-0.5f+CAP_RANDF());
                vel.x = 180.0f*(-0.5f+CAP_RANDF());
                vel.y = 360.0f*CAP_RANDF();
                vel.z = 180.0f*(-0.5f+CAP_RANDF());
                mbev_CapEffMasuHitAdd(masuHitEff, &effPos, &rot, &vel, 100.0f*(2.0f+CAP_RANDF()), alpha*(50.0f*(1.0f+(0.5f*CAP_RANDF()))), 10.0f+(5.0f*CAP_RANDF()));
            }
            HuPrcVSleep();
        }
    } else {
        mbCameraRotGet(&rot);
        scale.x = 1.0f;
        scale.y = 2.0f;
        scale.z = 100.0f;
        mbev_CapEffColorSet(&color, mbRandMod(32768));
        mbev_CapEffRingAdd(ringEff, pos, rot, scale, 1, 10, 0, color);
        scale.x = 2.0f;
        scale.y = 2.5f;
        scale.z = 200.0f;
        mbev_CapEffColorSet(&color, mbRandMod(32768));
        mbev_CapEffRingAdd(ringEff, pos, rot, scale, 3, 10, 1, color);
        for(i = 0; i <= work->time; i++) {
            alpha = HuCos(90.0f*((float)i/work->time));
            mbMasuPosGet(work->masuId, &pos);
            VECAdd(&pos, &work->ofs, &pos);
            for(j = 0; j < 3; j++) {
                effPos = pos;
                rot.x = 360.0f*CAP_RANDF();
                rot.y = 360.0f*CAP_RANDF();
                rot.z = 360.0f*CAP_RANDF();
                vel.x = rot.x+(60.0f*(-0.5f+CAP_RANDF()));
                vel.y = rot.y+(60.0f*(-0.5f+CAP_RANDF()));
                vel.z = rot.z+(60.0f*(-0.5f+CAP_RANDF()));
                mbev_CapEffRayAdd(rayEff, &effPos, &rot, &vel, 200.0f*(1.0f+(0.25f*CAP_RANDF())), 10.0f+(5.0f*CAP_RANDF()));
            }
            mbev_CapEffRayAlphaSet(rayEff, alpha);
            for(j = 0; j < 5.0f*alpha; j++) {
                effPos = pos;
                rot.x = 360.0f*CAP_RANDF();
                rot.y = 360.0f*CAP_RANDF();
                rot.z = 360.0f*CAP_RANDF();
                vel.x = 360.0f*(-0.5f+CAP_RANDF());
                vel.y = 360.0f*(-0.5f+CAP_RANDF());
                vel.z = 360.0f*(-0.5f+CAP_RANDF());
                mbev_CapEffMasuHitAdd(masuHitEff, &effPos, &rot, &vel, 100.0f*(2.0f+CAP_RANDF()), alpha*(50.0f*(1.0f+(0.5f*CAP_RANDF()))), 10.0f+(5.0f*CAP_RANDF()));
            }
            HuPrcVSleep();
        }
    }
    mbev_CapEffRayKill(rayEff);
    mbev_CapEffRingKill(ringEff);
    mbev_CapEffMasuHitKill(masuHitEff);
    HuPrcEnd();
}

static void ev_CapEffOpenKill(void)
{
    HUPROCESS *proc = HuPrcCurrentGet();
    CAPEFFOPENWORK *work = proc->property;
    HuMemDirectFree(work);
}

void mbev_CapNull(void)
{
    CAPWORK *work = HuPrcCurrentGet()->property;
    HuPrcEnd();
}

void mbev_CapNullKill(void)
{
}

void mbev_CapDebugCam(void)
{
    CAPWORK *work = HuPrcCurrentGet()->property;
    MBCAMERA *camera;
    HuVecF eye;
    HuVecF rot;
    HuVecF eyeOrig;
    HuVecF rotOrig;
    float zoomOrig;
    float zoom;
    float x;
    float y;
    float speed;
    int objId;
    int mode;
    int dispF;
    int frame;

    mbCapEffUseCreate(work->playerNo, work->capsuleNo);
    while(mbCapEffUseModeGet(work->playerNo) >= 0) {
        HuPrcVSleep();
    }
    mbCameraMoveWait();
    camera = mbCameraGet();
    eye.x = eyeOrig.x = camera->target.x+camera->offset.x;
    eye.y = eyeOrig.y = camera->target.y+camera->offset.y;
    eye.z = eyeOrig.z = camera->target.z+camera->offset.z;
    rot.x = rotOrig.x = camera->rot.x;
    rot.y = rotOrig.y = camera->rot.y;
    rot.z = rotOrig.z = camera->rot.z;
    zoom = zoomOrig = camera->zoom;
    mode = 0;
    dispF = FALSE;
    x = 32.0f;
    y = 50.0f;
    speed = 1.0f;
    frame = 0;
    objId = mbev_CapObjCreate(&work->objWork, 0xC0044, NULL, FALSE, 5, FALSE);
    mbObjPosSet(objId, eye.x, eye.y, eye.z);
    mbObjDispSet(objId, FALSE);
    mbCameraFocusObjSet(objId);
    do {
        if(HuPadBtnDown[0] & PAD_BUTTON_X) {
            mode++;
            if(mode > 2) {
                mode = 0;
            }
        }
        if(HuPadBtnDown[0] & PAD_BUTTON_Y) {
            dispF ^= TRUE;
        }
        frame++;
        mbObjDispSet(objId, dispF);
        switch(mode) {
            case 0:
                if(HuPadBtn[0] & PAD_TRIGGER_L) {
                    speed = 2.0f;
                } else if(HuPadBtn[0] & PAD_TRIGGER_R) {
                    speed = 0.5f;
                } else {
                    speed = 1.0f;
                }
                if(fabs(mbPadStkXGet(0)) > 8.0) {
                    eye.x += speed*(0.05f*mbPadStkXGet(0));
                }
                if(fabs(mbPadStkYGet(0)) > 8.0) {
                    eye.z -= speed*(0.05f*mbPadStkYGet(0));
                }
                if(HuPadBtn[0] & PAD_BUTTON_UP) {
                    eye.y += 25.0f*speed;
                }
                if(HuPadBtn[0] & PAD_BUTTON_DOWN) {
                    eye.y -= 25.0f*speed;
                }
                if(HuPadBtn[0] & PAD_BUTTON_A) {
                    zoom += 25.0f*speed;
                }
                if(HuPadBtn[0] & PAD_BUTTON_B) {
                    zoom -= 25.0f*speed;
                }
                if(HuPadBtnDown[0] & PAD_BUTTON_START) {
                    eye = eyeOrig;
                    rot = rotOrig;
                    zoom = zoomOrig;
                }
                fontcolor = 15;
                print8(36.0+x, 84.0+y, 1.5f, "\xFD\x01POSITION MODE");
                break;

            case 1:
                if(HuPadBtn[0] & PAD_TRIGGER_L) {
                    speed = 2.0f;
                } else if(HuPadBtn[0] & PAD_TRIGGER_R) {
                    speed = 0.5f;
                } else {
                    speed = 1.0f;
                }
                if(fabs(mbPadStkXGet(0)) > 8.0) {
                    rot.y += speed*(0.05f*mbPadStkXGet(0));
                }
                if(fabs(mbPadStkYGet(0)) > 8.0) {
                    rot.x += speed*(0.05f*mbPadStkYGet(0));
                }
                if(rot.x >= 360.0f) {
                    rot.x -= 360.0f;
                } else if(rot.x < 0.0f) {
                    rot.x += 360.0f;
                }
                if(rot.y >= 360.0f) {
                    rot.y -= 360.0f;
                } else if(rot.y < 0.0f) {
                    rot.y += 360.0f;
                }
                if(HuPadBtn[0] & PAD_BUTTON_A) {
                    zoom += 25.0f*speed;
                }
                if(HuPadBtn[0] & PAD_BUTTON_B) {
                    zoom -= 25.0f*speed;
                }
                if(HuPadBtnDown[0] & PAD_BUTTON_START) {
                    eye = eyeOrig;
                    rot = rotOrig;
                    zoom = zoomOrig;
                }
                fontcolor = 15;
                print8(36.0+x, 84.0+y, 1.5f, "\xFD\x01ROTATE MODE");
                break;

            default:
                if(fabs(mbPadStkXGet(0)) > 8.0) {
                    x += 0.05f*mbPadStkXGet(0);
                }
                if(fabs(mbPadStkYGet(0)) > 8.0) {
                    y += 0.05f*mbPadStkYGet(0);
                }
                fontcolor = 14;
                print8(36.0+x, 84.0+y, 1.5f, "\xFD\x01""DISP MODE");
                break;
        }
        print8(36.0+x, 96.0+y, 1.5f, "\xFD\x01RX:%.4f RY:%.4f RZ:%.4f", rot.x, rot.y, rot.z);
        print8(36.0+x, 108.0+y, 1.5f, "\xFD\x01""CX:%.4f CY:%.4f CZ:%.4f", eye.x, eye.y, eye.z);
        print8(36.0+x, 120.0+y, 1.5f, "\xFD\x01ZM:%.4f", zoom);
        mbObjPosSet(objId, eye.x, eye.y, eye.z);
        mbCameraEyeSet(eye.x, eye.y, eye.z);
        mbCameraRotSet(rot.x, rot.y, rot.z);
        mbCameraZoomSet(zoom);
        HuPrcVSleep();
    } while(!(HuPadBtn[0] & PAD_TRIGGER_L) || !(HuPadBtn[0] & PAD_TRIGGER_R));
    mbCameraMoveOnSet(TRUE);
    mbCameraMovePos(&eyeOrig, &rotOrig, NULL, zoomOrig, -1.0f, 1);
    mbCameraFocusPlayerSet(work->playerNo);
    HuPrcEnd();
}

/* Data the later parts use; defined here so it follows mbev_CapDebugCam's strings. */
static s32 ev_CapEffRingFile[6] = { 0xC0031, 0xC0032, 0xC0033, 0xC0031, 0xC0038, 0xC0038 };

static GXColor ev_CapEffElectricColor[4] = {
    { 192, 192, 255, 255 },
    { 164, 192, 255, 255 },
    { 192, 255, 255, 255 },
    { 192, 164, 255, 255 },
};

static HuVecF basePos[4] = {
    { -0.5f, 0.5f, 0.0f },
    { 0.5f, 0.5f, 0.0f },
    { 0.5f, -0.5f, 0.0f },
    { -0.5f, -0.5f, 0.0f },
};

static float baseST1[8] = { 0.0f, 0.0f, 0.25f, 0.0f, 0.25f, 0.25f, 0.0f, 0.25f };
static float baseST2[8] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

static GXColor capsuleRingColor = { 255, 255, 127, 255 };

/* @@PART2@@ */
/* @@PART3@@ */
/* @@PART4@@ */
/* @@PART5@@ */
/* @@PART6@@ */
/* @@END@@ */
