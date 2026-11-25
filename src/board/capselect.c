#include "game/board/capselect.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/comchoice.h"
#include "game/board/exit.h"
#include "game/board/masu.h"
#include "game/board/object.h"
#include "game/board/pause.h"
#include "game/board/player.h"
#include "game/board/scroll.h"
#include "game/board/single.h"
#include "game/board/status.h"
#include "game/board/telop.h"
#include "game/board/tutorial.h"
#include "game/board/window.h"
#include "game/board/wipe.h"

#include "game/esprite.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/memory.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/wipe.h"
#include "string.h"
#include "math.h"

/* capsule.h types these with s16 arguments and a one-argument mbCapObjCreate; retail passes ints
   (no extsh) and two arguments. Declared here instead of including capsule.h. */
int mbCapUse(int playerNo, s16 capsuleNo);
int mbCapUseMesGet(int capsuleNo);
s16 mbCapUseModeGet(s16 capsuleNo);
int mbCapDescWinCreate(int capsuleNo);
int mbCapObjCreate(int capsuleNo, int arg1);
void mbCapObjKill(int objId);
void mbCapNumInc(int capsuleNo, int arg1);
int mbCapMasuNextGet(int playerNo);
int mbCapSelectComGet(int playerNo, int *capsuleTbl, int num);
int mbCapSelectDeleteComGet(int playerNo, int *capsuleTbl, int num);

/* capevent.h types the effect handle as both int and void *. */
void *mbev_CapEffExplodeCreate(void);
void mbev_CapEffExplodeKill(void *eff);
int mbev_CapEffExplodeAnimGet(void *eff);
void mbev_CapEffDustExplodeAdd(void *eff, HuVecF pos);

#define CAPSEL_RESULT_SELECT -8
#define CAPSEL_RESULT_NONE -9

typedef struct CapSelectWork_s {
    int num; //0x00
    int selNo; //0x04
    int comResult; //0x08
    int playerNo; //0x0C
    int winNo; //0x10
    s16 winId[3]; //0x14
    s16 objId[6]; //0x1A
    s16 espId[2]; //0x26
    s16 time; //0x2A
    s16 comDelay; //0x2C
    float scale[6]; //0x30
    int extraCapsule; //0x48
    int deleteF; //0x4C
} CAPSELECTWORK; //size 0x50

typedef struct CapSelectShrinkWork_s {
    int playerNo; //0x00
    int objId; //0x04
    int num; //0x08
    s16 objIdTbl[6]; //0x0C
    HuVecF posStart[6]; //0x18
    HuVecF posEnd[6]; //0x60
} CAPSELECTSHRINKWORK; //size 0xA8

typedef struct CapMasuWork_s {
    int no; //0x00
    int masuId; //0x04
    int objId; //0x08
    int angle; //0x0C
    int playerNo; //0x10
    int hideF; //0x14
    float scale; //0x18
    HuVecF pos; //0x1C
} CAPMASUWORK; //size 0x28

static HUPROCESS *ev_CapSelectShrinkProc[GW_PLAYER_MAX] = { NULL };

static int ev_CapSelectObjId[GW_PLAYER_MAX];
static int ev_CapSelectResult[GW_PLAYER_MAX];
static int ev_CapSelectExtra[GW_PLAYER_MAX];
static OMOBJ *ev_CapMasuOMObj[16];

static s8 capSelectResult[GW_PLAYER_MAX];
static int capSelectObjId;
static BOOL ev_CapMasuDispF;
static BOOL ev_CapSelectStoryF;

static void CapSelect(CAPSELECTWORK *work);
static int CapSelectPadExec(CAPSELECTWORK *work);
static void CapSelectShrinkCreate(int playerNo, int objId, CAPSELECTWORK *selWork, HuVecF *posStart, HuVecF *posEnd);
static void CapSelectShrink(void);
static void CapSelectShrinkDestroy(void);
static void CapSelectStoryFSet(BOOL storyF);
static void CapSelectExtraCapsuleGet(int playerNo, int capsuleNo);
static int CapSelectCapsuleGet(int playerNo, int selNo);
static int CapSelectNumGet(int playerNo);
static int CapSelectComGet(int playerNo, BOOL deleteF);
static void CapMasuOMExec(OMOBJ *obj);
static int CapHelpWinCreate(int capsuleNo, BOOL deleteF);

static inline void *CapSelectMalloc(s32 size)
{
    return HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
}

static inline void CapObjAttrPauseSet(s16 objId)
{
    mbObjAttrSet(objId, 0x40000001);
}

int mbCapSelect(void)
{
    int playerNo = GwSystem.turnPlayerNo;
    CAPSELECTWORK *work;
    int objId;
    int selNo;
    mbCapSelectResultSet(playerNo, -1, -1);
    capSelectObjId = -1;
    if(GWPartyGet() != FALSE) {
        mbStatusDispSetAll(FALSE);
        while(!mbStatusOffCheckAll()) {
            HuPrcVSleep();
        }
    }
    retry:
        while(!mbCapSelectShrinkCheck(playerNo)) {
            HuPrcVSleep();
        }
        work = CapSelectMalloc(sizeof(CAPSELECTWORK));
        memset(work, 0, sizeof(CAPSELECTWORK));
        work->playerNo = playerNo;
        work->deleteF = FALSE;
        work->extraCapsule = -1;
        if(GWPartyGet() == FALSE) {
            CapSelectStoryFSet(TRUE);
        } else {
            CapSelectStoryFSet(FALSE);
        }
        CapSelectExtraCapsuleGet(playerNo, -1);
        capSelectResult[playerNo] = CAPSEL_RESULT_NONE;
        CapSelect(work);
        HuMemDirectFree(work);
        if(capSelectResult[playerNo] >= 0) {
            if(mbCapUse(playerNo, capSelectResult[playerNo])) {
                capSelectResult[playerNo] = -7;
                if(_CheckFlag(FLAGNUM(FLAG_GROUP_COMMON, 29))) {
                    return capSelectResult[playerNo];
                }
                if(mbPlayerBlackoutGet()) {
                    mbCameraPlayerViewSetFast(playerNo, 0);
                    mbCameraMoveWait();
                    WipeCreate(1, 0, 60);
                    mbWipeWait();
                    mbPlayerBlackoutSet(FALSE);
                }
                goto end;
            }
        } else {
            switch(capSelectResult[playerNo]) {
                case -3:
                    mbev_Scroll(playerNo, FALSE);
                    break;

                case -4:
                    mbev_Scroll(playerNo, TRUE);
                    break;

                default:
                    goto end;
            }
        }
        HuPrcVSleep();
        goto retry;
    end:
    while(!mbCapSelectShrinkCheck(playerNo)) {
        HuPrcVSleep();
    }
    mbCapSelectResultGet(playerNo, &objId, &selNo);
    if(objId != -1 && selNo != -1) {
        mbCapObjKill(objId);
    }
    return capSelectResult[playerNo];
}

int mbCapDelete(int extraCapsule, BOOL loopF)
{
    int playerNo = GwSystem.turnPlayerNo;
    CAPSELECTWORK *work;
    int winId;
    void *eff;
    int objId;
    int selNo;
    HuVecF pos;
    mbCapSelectResultSet(playerNo, -1, -1);
    capSelectObjId = -1;
    retry:
        while(!mbCapSelectShrinkCheck(playerNo)) {
            HuPrcVSleep();
        }
        work = CapSelectMalloc(sizeof(CAPSELECTWORK));
        memset(work, 0, sizeof(CAPSELECTWORK));
        work->playerNo = playerNo;
        work->deleteF = TRUE;
        work->extraCapsule = extraCapsule;
        CapSelectStoryFSet(FALSE);
        CapSelectExtraCapsuleGet(playerNo, work->extraCapsule);
        capSelectResult[playerNo] = CAPSEL_RESULT_NONE;
        CapSelect(work);
        HuMemDirectFree(work);
        while(!mbCapSelectShrinkCheck(playerNo)) {
            HuPrcVSleep();
        }
        if(capSelectResult[playerNo] >= 0) {
            winId = mbWinCreateChoice(3, 0x3A0002, -1, 0);
            mbWinTopInsertMesSet(mbCapUseMesGet(capSelectResult[playerNo]), 0);
            if(GwPlayer[playerNo].comF) {
                mbComChoiceLeftSet();
            }
            mbWinWait(winId);
            if(mbWinTopChoiceGet() == 1 || mbWinTopChoiceGet() == -1) {
                if(loopF) {
                    capSelectResult[playerNo] = CAPSEL_RESULT_NONE;
                } else {
                    capSelectResult[playerNo] = -1;
                }
            } else {
                mbCapSelectResultGet(playerNo, &objId, &selNo);
                mbObjPosGet(objId, &pos);
                mbAudFXPlay(0x3FD);
                eff = mbev_CapEffExplodeCreate();
                mbev_CapEffDustExplodeAdd(eff, pos);
                mbObjDispSet(objId, FALSE);
                while(mbev_CapEffExplodeAnimGet(eff) > 0) {
                    HuPrcVSleep();
                }
                mbev_CapEffExplodeKill(eff);
                omVibrate(playerNo, 20, 4, 4);
                if(GWPartyGet() == FALSE) {
                    mbSingleCall(6, 0);
                }
            }
        } else {
            switch(capSelectResult[playerNo]) {
                case -3:
                    mbev_Scroll(playerNo, FALSE);
                    goto retry;

                case -4:
                    mbev_Scroll(playerNo, TRUE);
                    goto retry;

                default:
                    goto end;
            }
        }
        if(capSelectResult[playerNo] == CAPSEL_RESULT_NONE && loopF) {
            goto retry;
        }
    end:
    mbCapSelectResultGet(playerNo, &objId, &selNo);
    if(objId != -1 && selNo != -1) {
        mbCapObjKill(objId);
    }
    return capSelectResult[playerNo];
}

static void CapSelect(CAPSELECTWORK *work)
{
    s16 temp = -1;
    int objId;
    int selNo;
    HuVec2f winPos;
    HuVec2f helpWinPos;
    HuVecF objPos;
    HuVecF playerPos;
    HuVecF basePos;
    float angle[6];
    HuVecF screenPos[6];
    HuVecF startPos[6];
    HuVecF modelPos[6];
    int i;
    int j;
    int playerNo = work->playerNo;
    s16 prevSelNo;
    s16 capsuleNo;
    s16 helpCapsule;
    s16 capsulePrev;
    int dir;
    float scale;
    float weight;
    float angleBase;
    float angleStep;

    work->selNo = prevSelNo = 0;
    capsulePrev = capsuleNo = helpCapsule = CapSelectCapsuleGet(work->playerNo, work->selNo);
    angleBase = angleStep = 0.0f;
    for(i=0; i<3; i++) {
        work->winId[i] = -1;
    }
    if(GwPlayerConf[work->playerNo].type != GW_PLAYER_TYPE_MAN) {
        work->comResult = CapSelectComGet(work->playerNo, work->deleteF);
        if(work->deleteF != FALSE && work->comResult < 0) {
            work->comResult = (frand() & 0x7FFF) % mbPlayerCapsuleNumGet(work->playerNo);
        }
        if(!_CheckFlag(FLAG_BOARD_TUTORIAL)) {
            work->comDelay = 30;
        } else {
            work->comDelay = 120;
        }
    } else {
        work->comResult = -2;
    }
    if(work->deleteF == FALSE) {
        mbStatusDispFocusSet(work->playerNo, TRUE);
    }
    mbCapSelectResultGet(playerNo, &objId, &selNo);
    capSelectObjId = objId;
    mbPlayerPosGet(work->playerNo, &playerPos);
    playerPos.y += 100.0f;
    basePos.x = playerPos.x;
    basePos.y = playerPos.y;
    basePos.z = playerPos.z-125.0f;
    work->num = CapSelectNumGet(work->playerNo);
    for(i=0; i<6; i++) {
        work->objId[i] = -1;
    }
    if(work->extraCapsule != -1) {
        work->selNo = prevSelNo = work->num-1;
        angleBase = work->selNo*(360.0f/work->num);
    }
    if(selNo != -1) {
        work->selNo = prevSelNo = selNo;
        angleBase = work->selNo*(360.0f/work->num);
    }
    capsulePrev = capsuleNo = helpCapsule = CapSelectCapsuleGet(work->playerNo, work->selNo);
    for(i=0; i<work->num; i++) {
        angle[i] = 360.0f-(i*(360.0f/work->num));
        modelPos[i].x = basePos.x+(125.0*HuSin(angleBase+angle[i]));
        modelPos[i].y = 150.0f+basePos.y;
        modelPos[i].z = basePos.z+(125.0*HuCos(angleBase+angle[i]));
        startPos[i] = playerPos;
        if(objId != -1 && selNo == i) {
            work->objId[i] = objId;
            mbObjLayerSet(work->objId[i], 4);
            CapObjAttrPauseSet(work->objId[i]);
            mbObjPosGet(work->objId[i], &startPos[i]);
        } else {
            work->objId[i] = mbCapObjCreate(CapSelectCapsuleGet(work->playerNo, i), 0);
            mbObjLayerSet(work->objId[i], 4);
            mbObjCameraSet(work->objId[i], 2);
            CapObjAttrPauseSet(work->objId[i]);
            mbObjMotionSpeedSet(work->objId[i], 0.0f);
        }
    }
    for(i=0; i<2; i++) {
        work->espId[i] = espEntry(0xC0026, 1400, 0);
        espDispOff(work->espId[i]);
        espScaleSet(work->espId[i], 0.5f, 0.5f);
        espDrawNoSet(work->espId[i], 32);
        if(i == 0) {
            espZRotSet(work->espId[i], 180.0f);
        } else {
            espZRotSet(work->espId[i], 360.0f);
        }
    }
    mbAudFXPlay(0x401);
    for(i=0; i<=12u; i++) {
        weight = i/12.0f;
        for(j=0; j<work->num; j++) {
            if(j == work->selNo) {
                work->scale[j] = mbCapSelectGrow(&startPos[j], &modelPos[j], work->objId[j], weight, 1.0f);
            } else {
                work->scale[j] = mbCapSelectGrow(&startPos[j], &modelPos[j], work->objId[j], weight, 0.75f);
            }
        }
        HuPrcVSleep();
    }
    work->winId[0] = CapHelpWinCreate(helpCapsule, work->deleteF);
    mbWinPosGet(work->winId[0], &helpWinPos);
    mbWinPosSet(work->winId[0], helpWinPos.x, 284);
    for(i=0; i<work->num; i++) {
        mbObjPosGet(work->objId[i], &objPos);
        Hu3D3Dto2D(&objPos, HU3D_CAM0, &screenPos[i]);
    }
    for(i=0; i<2; i++) {
        if(i == 0) {
            espPosSet(work->espId[i], screenPos[work->selNo].x-48.0f, screenPos[work->selNo].y);
        } else {
            espPosSet(work->espId[i], 48.0f+screenPos[work->selNo].x, screenPos[work->selNo].y);
        }
        if(work->num > 1) {
            espDispOn(work->espId[i]);
        }
    }
    work->winId[1] = mbCapDescWinCreate(capsuleNo);
    mbWinPosGet(work->winId[1], &winPos);
    work->winNo = 1;
    while(1) {
        if(work->num > 1) {
            for(i=0; i<2; i++) {
                espDispOn(work->espId[i]);
            }
        }
        if(GwPlayerConf[work->playerNo].type != GW_PLAYER_TYPE_MAN && work->comResult == -2) {
            work->comResult = CapSelectComGet(work->playerNo, work->deleteF);
            work->comDelay = 30;
        }
        dir = CapSelectPadExec(work);
        if(capSelectResult[playerNo] == -2 ||
            capSelectResult[playerNo] == -1 ||
            capSelectResult[playerNo] == -3 ||
            capSelectResult[playerNo] == -4 ||
            capSelectResult[playerNo] == -5 ||
            capSelectResult[playerNo] == -7) {
            break;
        }
        if(capSelectResult[playerNo] == CAPSEL_RESULT_SELECT) {
            capSelectResult[playerNo] = CapSelectCapsuleGet(work->playerNo, work->selNo);
            break;
        }
        capsuleNo = CapSelectCapsuleGet(work->playerNo, work->selNo);
        if(work->selNo != prevSelNo || capsuleNo != capsulePrev) {
            mbObjMotionTimeSet(work->objId[prevSelNo], 0.0f);
            mbObjMotionSpeedSet(work->objId[prevSelNo], 0.0f);
            scale = work->scale[prevSelNo];
            mbObjScaleSet(work->objId[prevSelNo], scale, scale, scale);
            work->winId[work->winNo+1] = mbCapDescWinCreate(capsuleNo);
            angleBase = prevSelNo*(360.0f/work->num);
            if(dir > 0) {
                angleStep = 360.0f/work->num;
            } else {
                angleStep = -(360.0f/work->num);
            }
            for(i=0; i<2; i++) {
                espDispOff(work->espId[i]);
            }
            for(i=0; i<=12u; i++) {
                weight = i/12.0f;
                for(j=0; j<work->num; j++) {
                    modelPos[j].x = basePos.x+(125.0*HuSin(angleBase+angle[j]+(weight*angleStep)));
                    modelPos[j].y = 150.0f+basePos.y;
                    modelPos[j].z = basePos.z+(125.0*HuCos(angleBase+angle[j]+(weight*angleStep)));
                    mbObjPosSetV(work->objId[j], &modelPos[j]);
                    scale = work->scale[j];
                    if(j == work->selNo) {
                        scale = (0.75f*scale)+(0.25f*(weight*scale));
                    } else if(j == prevSelNo) {
                        scale = scale-(0.25f*(weight*scale));
                    } else {
                        scale = 0.75f*scale;
                    }
                    mbObjScaleSet(work->objId[j], scale, scale, scale);
                }
                if(prevSelNo != work->selNo) {
                    if(dir < 0) {
                        mbWinPosSet(work->winId[(work->winNo^1)+1], winPos.x-(576.0f*weight), winPos.y);
                        mbWinPosSet(work->winId[work->winNo+1], (576.0f+winPos.x)-(576.0f*weight), winPos.y);
                    } else {
                        mbWinPosSet(work->winId[(work->winNo^1)+1], winPos.x+(576.0f*weight), winPos.y);
                        mbWinPosSet(work->winId[work->winNo+1], (winPos.x-576.0f)+(576.0f*weight), winPos.y);
                    }
                }
                HuPrcVSleep();
            }
            mbWinKill(work->winId[(work->winNo^1)+1]);
            work->winId[(work->winNo^1)+1] = -1;
            work->winNo ^= 1;
            work->time = 0;
            prevSelNo = work->selNo;
            capsulePrev = capsuleNo;
        }
        if(CapSelectCapsuleGet(work->playerNo, prevSelNo) != 47) {
            mbObjMotionSpeedSet(work->objId[prevSelNo], 1.0f);
        }
        if(work->winId[0] >= 0 && helpCapsule != capsuleNo) {
            helpCapsule = capsuleNo;
            if(work->deleteF == FALSE) {
                mbWinKill(work->winId[0]);
                work->winId[0] = CapHelpWinCreate(capsuleNo, work->deleteF);
                mbWinPosGet(work->winId[0], &helpWinPos);
                mbWinPosSet(work->winId[0], helpWinPos.x, 284);
            }
        }
        scale = work->scale[work->selNo]*(1+(0.2f*fabs(HuSin((90.0f*work->time)/12.0f))));
        mbObjScaleSet(work->objId[work->selNo], scale, scale, scale);
        for(i=0; i<2; i++) {
            scale = 0.5+(0.1f*fabs(HuSin((90.0f*work->time)/12.0f)));
            espScaleSet(work->espId[i], scale, scale);
        }
        work->time++;
        HuPrcVSleep();
    }
    for(i=0; i<2; i++) {
        espDispOff(work->espId[i]);
    }
    for(i=0; i<work->num; i++) {
        mbObjPosGet(work->objId[i], &modelPos[i]);
        startPos[i] = playerPos;
    }
    if(capSelectResult[playerNo] >= 0) {
        mbCapSelectResultSet(playerNo, work->objId[work->selNo], work->selNo);
        startPos[work->selNo].x = playerPos.x;
        startPos[work->selNo].y = 150.0f+playerPos.y;
        startPos[work->selNo].z = playerPos.z;
    } else {
        mbCapSelectResultSet(playerNo, -1, -1);
    }
    mbCapSelectResultGet(playerNo, &objId, &selNo);
    capSelectObjId = objId;
    mbAudFXPlay(0x402);
    CapSelectShrinkCreate(playerNo, objId, work, startPos, modelPos);
    for(i=0; i<3; i++) {
        if(work->winId[i] >= 0) {
            mbWinKill(work->winId[i]);
            work->winId[i] = -1;
        }
    }
    for(i=0; i<2; i++) {
        if(work->espId[i] >= 0) {
            espKill(work->espId[i]);
            work->espId[i] = -1;
        }
    }
    if(_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        mbTutorialCall(16);
    }
    if(work->deleteF == FALSE && capSelectResult[playerNo] != -3 && capSelectResult[playerNo] != -4 && GWPartyGet() != FALSE) {
        mbStatusDispFocusSet(work->playerNo, FALSE);
    }
}

float mbCapSelectGrow(HuVecF *posStart, HuVecF *posEnd, s16 objId, float weight, float scaleBase)
{
    float scale = HuSin(90.0f*weight);
    HuVecF pos;
    pos.y = posStart->y+(scale*(posEnd->y-posStart->y));
    pos.x = posStart->x+(weight*(posEnd->x-posStart->x));
    pos.z = posStart->z+(weight*(posEnd->z-posStart->z));
    if(objId != capSelectObjId) {
        scale = weight;
    } else {
        scale = 1.0f;
    }
    mbObjPosSetV(objId, &pos);
    mbObjScaleSet(objId, scale*scaleBase, scale*scaleBase, scale*scaleBase);
    return scale;
}

static inline u16 CapSelectPadRead(CAPSELECTWORK *work)
{
    u16 btn;
    if(GwPlayerConf[work->playerNo].type == GW_PLAYER_TYPE_MAN) {
        int padNo = GwPlayer[work->playerNo].padNo;
        btn = HuPadBtnDown[(u16)padNo];
        if(mbPadStkXGet((u16)padNo) < -20) {
            btn |= PAD_BUTTON_LEFT;
        } else if(mbPadStkXGet((u16)padNo) > 20) {
            btn |= PAD_BUTTON_RIGHT;
        }
    } else {
        btn = 0;
        if(--work->comDelay == 0) {
            if(work->comResult == -1) {
                btn |= PAD_BUTTON_B;
            } else if(work->comResult == work->selNo) {
                btn |= PAD_BUTTON_A;
            } else if(work->selNo < work->comResult) {
                btn |= PAD_BUTTON_RIGHT;
            } else {
                btn |= PAD_BUTTON_LEFT;
            }
            work->comDelay = 20;
        }
    }
    return btn;
}

static int CapSelectPadExec(CAPSELECTWORK *work)
{
    int playerNo = work->playerNo;
    int dir = 0;
    u16 btn = CapSelectPadRead(work);
    if(mbPauseProcCheck()) {
        btn = 0;
        return dir;
    }
    if((btn & PAD_BUTTON_LEFT) && work->num > 1) {
        mbAudFXPlay(0);
        if(--work->selNo < 0) {
            work->selNo = work->num-1;
        }
        dir = -1;
    }
    if((btn & PAD_BUTTON_RIGHT) && work->num > 1) {
        mbAudFXPlay(0);
        if(++work->selNo >= work->num) {
            work->selNo = 0;
        }
        dir = 1;
    }
    if(btn & PAD_BUTTON_A) {
        capSelectResult[playerNo] = CAPSEL_RESULT_SELECT;
        mbAudFXPlay(1);
        return dir;
    }
    if((btn & PAD_BUTTON_Y) && GWPartyGet() != FALSE) {
        capSelectResult[playerNo] = -4;
        mbAudFXPlay(1);
        return dir;
    }
    if(btn & PAD_BUTTON_X) {
        capSelectResult[playerNo] = -3;
        mbAudFXPlay(1);
        return dir;
    }
    if(btn & PAD_BUTTON_B) {
        capSelectResult[playerNo] = -7;
        mbAudFXPlay(3);
        return dir;
    }
    return dir;
}

void mbCapSelectResultSet(int playerNo, int objId, int selNo)
{
    ev_CapSelectObjId[playerNo] = objId;
    ev_CapSelectResult[playerNo] = selNo;
}

void mbCapSelectResultGet(int playerNo, int *objId, int *selNo)
{
    if(objId != NULL) {
        *objId = ev_CapSelectObjId[playerNo];
    }
    if(selNo != NULL) {
        *selNo = ev_CapSelectResult[playerNo];
    }
}

void mbCapSelectResultReset(int playerNo)
{
    ev_CapSelectObjId[playerNo] = -1;
    ev_CapSelectResult[playerNo] = -1;
}

static void CapSelectShrinkCreate(int playerNo, int objId, CAPSELECTWORK *selWork, HuVecF *posStart, HuVecF *posEnd)
{
    CAPSELECTSHRINKWORK *work;
    int i;
    ev_CapSelectShrinkProc[playerNo] = HuPrcChildCreate(CapSelectShrink, 0x2005, 0x4000, 0, mbMainProc);
    HuPrcDestructorSet2(ev_CapSelectShrinkProc[playerNo], CapSelectShrinkDestroy);
    work = CapSelectMalloc(sizeof(CAPSELECTSHRINKWORK));
    ev_CapSelectShrinkProc[playerNo]->property = work;
    memset(work, 0, sizeof(CAPSELECTSHRINKWORK));
    work->playerNo = playerNo;
    work->objId = objId;
    work->num = selWork->num;
    for(i=0; i<work->num; i++) {
        work->objIdTbl[i] = selWork->objId[i];
        work->posStart[i] = posStart[i];
        work->posEnd[i] = posEnd[i];
    }
}

static void CapSelectShrink(void)
{
    CAPSELECTSHRINKWORK *work = HuPrcCurrentGet()->property;
    HuVecF temp;
    float scaleOfs;
    float rotY;
    float t;
    float scale;
    float rot;
    int i;
    int j;
    if(capSelectObjId != -1) {
        mbObjScaleGet(capSelectObjId, &temp);
        scaleOfs = temp.x-1.0f;
        mbObjRotGet(capSelectObjId, &temp);
        rotY = temp.y;
        if(rotY > 180.0f) {
            rotY -= 360.0f;
        }
    } else {
        scaleOfs = 1.0f;
        rotY = 0.0f;
    }
    for(i=0; i<=9u; i++) {
        t = i/9.0f;
        scale = 1.0f+(scaleOfs*(1.0f-t));
        rot = rotY*(1.0f-t);
        for(j=0; j<work->num; j++) {
            if(work->objIdTbl[j] == capSelectObjId) {
                mbCapSelectGrow(&work->posStart[j], &work->posEnd[j], work->objIdTbl[j], 1.0f-t, scale);
                mbObjRotSet(work->objIdTbl[j], 0.0f, rot, 0.0f);
            } else {
                mbCapSelectGrow(&work->posStart[j], &work->posEnd[j], work->objIdTbl[j], 1.0f-t, 0.75f);
            }
        }
        HuPrcVSleep();
    }
    HuPrcEnd();
}

static void CapSelectShrinkDestroy(void)
{
    CAPSELECTSHRINKWORK *work = HuPrcCurrentGet()->property;
    int i;
    for(i=0; i<work->num; i++) {
        if(work->objIdTbl[i] != work->objId && work->objIdTbl[i] != -1) {
            mbCapObjKill(work->objIdTbl[i]);
        }
    }
    HuMemDirectFree(work);
    ev_CapSelectShrinkProc[work->playerNo] = NULL;
}

BOOL mbCapSelectShrinkCheck(int playerNo)
{
    if(ev_CapSelectShrinkProc[playerNo] == NULL) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static void CapSelectStoryFSet(BOOL storyF)
{
    ev_CapSelectStoryF = storyF;
}

static void CapSelectExtraCapsuleGet(int playerNo, int capsuleNo)
{
    ev_CapSelectExtra[playerNo] = capsuleNo;
}

static int CapSelectCapsuleGet(int playerNo, int selNo)
{
    if(ev_CapSelectStoryF) {
        if(selNo == 0) {
            return 47;
        } else if(selNo == 1) {
            return 48;
        } else {
            return mbPlayerCapsuleGet(playerNo, selNo-2);
        }
    } else {
        if(selNo == mbPlayerCapsuleNumGet(playerNo)) {
            return ev_CapSelectExtra[playerNo];
        } else {
            return mbPlayerCapsuleGet(playerNo, selNo);
        }
    }
}

static int CapSelectNumGet(int playerNo)
{
    if(ev_CapSelectStoryF) {
        return mbPlayerCapsuleNumGet(playerNo)+2;
    } else if(ev_CapSelectExtra[playerNo] >= 0) {
        return mbPlayerCapsuleNumGet(playerNo)+1;
    } else {
        return mbPlayerCapsuleNumGet(playerNo);
    }
}

static int CapSelectComGet(int playerNo, BOOL deleteF)
{
    int capsuleTbl[6];
    int num = CapSelectNumGet(playerNo);
    int i;
    for(i=0; i<num; i++) {
        capsuleTbl[i] = CapSelectCapsuleGet(playerNo, i);
    }
    if(!deleteF) {
        return mbCapSelectComGet(playerNo, capsuleTbl, num);
    } else {
        return mbCapSelectDeleteComGet(playerNo, capsuleTbl, num);
    }
}

void fn_8019A618(void)
{
}

void fn_8019A61C(void)
{
}

void fn_8019A620(void)
{
}

void fn_8019A624(void)
{
}

void fn_8019A628(void)
{
}

void fn_8019A62C(void)
{
}

void mbCapMasuExec(int playerNo, int masuId)
{
    int capsuleNo;
    CAPMASUWORK *work;
    int i;
    int slot;
    int winId;
    int result;
    OMOBJ *obj;
    BOOL cameraF = FALSE;
    if(GwSystem.turnNo < GwSystem.turnMax) {
        for(i=0; i<16; i++) {
            obj = ev_CapMasuOMObj[i];
            if(obj != NULL) {
                work = obj->data;
                if(work->masuId == masuId) {
                    break;
                }
            }
        }
        work->playerNo = playerNo;
        work->hideF = TRUE;
        work->scale = 0.0f;
        mbObjDispSet(work->objId, FALSE);
        if(!_CheckFlag(FLAG_BOARD_TUTORIAL)) {
            capsuleNo = mbCapMasuNextGet(playerNo);
        } else {
            capsuleNo = mbTutorialCall(21);
            if(capsuleNo < 0) {
                capsuleNo = mbCapMasuNextGet(playerNo);
            }
        }
        result = -1;
        slot = -1;
        mbMoveNumDispSet(playerNo, FALSE);
        mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 8.0f, 0x40000001);
        mbPlayerRotateStart(playerNo, 0, 15);
        while(!mbPlayerRotateCheck(playerNo)) {
            HuPrcVSleep();
        }
        mbCapCapsuleGet(playerNo, capsuleNo);
        if(GWPartyGet() == FALSE) {
            mbSingleCall(5, capsuleNo);
        }
        if(mbPlayerCapsuleNumGet(playerNo) >= mbPlayerCapsuleMaxGet()) {
            winId = mbWinCreate(2, 0x3A0000, -1);
            mbWinTopInsertMesSet(mbCapUseMesGet(capsuleNo), 0);
            mbWinWait(winId);
            mbCameraPlayerViewSet(playerNo, 0);
            cameraF = TRUE;
            winId = mbWinCreate(2, 0x3A0001, -1);
            mbWinTopInsertMesSet(mbCapUseMesGet(capsuleNo), 0);
            mbWinWait(winId);
            do {
                result = mbCapDelete(capsuleNo, TRUE);
                switch(result) {
                    default:
                        if(result != capsuleNo) {
                            for(i=0; i<mbPlayerCapsuleMaxGet(); i++) {
                                if(result == mbPlayerCapsuleGet(playerNo, i)) {
                                    slot = i;
                                }
                            }
                            if(slot != -1) {
                                mbPlayerCapsuleRemove(playerNo, slot);
                            }
                        } else {
                            result = -1;
                            slot = -2;
                        }
                        break;

                    case -3:
                        mbev_Scroll(playerNo, FALSE);
                        slot = -1;
                        break;

                    case -4:
                        mbev_Scroll(playerNo, TRUE);
                        slot = -1;
                        break;

                    case -7:
                        slot = -2;
                        break;
                }
            } while(slot == -1);
            if(slot >= 0) {
                mbPlayerCapsuleAdd(playerNo, capsuleNo);
                mbCapNumInc(capsuleNo, 0);
                mbPlayerWinLoseVoicePlay(playerNo, 12, 0x243);
                mbPlayerMotionShiftSet(playerNo, 12, 0.0f, 8.0f, 0);
                winId = mbWinCreate(2, 0x3A0003, -1);
                mbWinTopInsertMesSet(mbCapUseMesGet(result), 0);
                mbWinTopInsertMesSet(mbCapUseMesGet(capsuleNo), 1);
                mbWinWait(winId);
            } else {
                winId = mbWinCreate(2, 0x3A0004, -1);
                mbWinTopInsertMesSet(mbCapUseMesGet(capsuleNo), 0);
                mbWinWait(winId);
            }
        } else {
            mbPlayerCapsuleAdd(playerNo, capsuleNo);
            mbCapNumInc(capsuleNo, 0);
            mbPlayerWinLoseVoicePlay(playerNo, 12, 0x243);
            mbPlayerMotionShiftSet(playerNo, 12, 0.0f, 8.0f, 0);
            winId = mbWinCreate(2, 0x3A0000, -1);
            mbWinTopInsertMesSet(mbCapUseMesGet(capsuleNo), 0);
            mbWinWait(winId);
            while(!mbPlayerMotionEndCheck(playerNo) || mbObjMotionShiftIDGet(mbPlayerObjIDGet(playerNo)) != -1) {
                HuPrcVSleep();
            }
        }
        if(_CheckFlag(FLAG_BOARD_TUTORIAL)) {
            mbTutorialCall(22);
        }
        mbMoveNumDispSet(playerNo, TRUE);
        if(cameraF) {
            mbCameraPlayerViewSet(playerNo, 2);
        }
    }
}

void mbCapMasuObjInit(void)
{
    int i;
    for(i=0; i<16; i++) {
        ev_CapMasuOMObj[i] = NULL;
    }
    mbCapMasuObjCreateAll();
    ev_CapMasuDispF = TRUE;
}

void mbCapMasuObjClose(void)
{
    int i;
    for(i=0; i<16; i++) {
        ev_CapMasuOMObj[i] = NULL;
    }
}

void mbCapMasuObjCreateAll(void)
{
    int i;
    for(i=1; i<=mbMasuRawNumGet(); i++) {
        if(mbMasuTypeGet(i) == 8) {
            mbCapMasuObjCreate(i);
        }
    }
}

void mbCapMasuObjCreate(int masuId)
{
    CAPMASUWORK *work;
    int i;
    int j;
    OMOBJ *obj;
    for(i=0; i<16; i++) {
        if(ev_CapMasuOMObj[i] == NULL) {
            break;
        }
    }
    obj = ev_CapMasuOMObj[i] = omAddObjEx(mbObjMan, 260, 0, 0, -1, CapMasuOMExec);
    work = obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(CAPMASUWORK), HU_MEMNUM_OVL);
    memset(work, 0, sizeof(CAPMASUWORK));
    work->no = i;
    work->masuId = masuId;
    work->objId = mbObjCreate(0xC0021, NULL, TRUE);
    mbObjLayerSet(work->objId, 3);
    CapObjAttrPauseSet(work->objId);
    work->angle = frand() & 0x7FFF;
    work->playerNo = -1;
    work->hideF = FALSE;
    work->scale = 1.0f;
    mbMasuPosGet(work->masuId, &work->pos);
    mbObjPosSetV(work->objId, &work->pos);
    for(j=0; j<4; j++) {
        if(work->masuId == GwPlayer[j].masuId) {
            break;
        }
    }
    if(j < 4 || GwSystem.turnNo >= GwSystem.turnMax) {
        work->hideF = TRUE;
        work->scale = 0.0f;
        mbObjDispSet(work->objId, FALSE);
    }
}

void mbCapCapsuleGet(int playerNo, int capsuleNo)
{
    int objId;
    int i;
    float t;
    float scale;
    float s;
    HuVecF pos;
    HuVecF pos2;
    objId = mbCapObjCreate(capsuleNo, 0);
    mbObjAttrSet(objId, 0x40000001);
    mbObjCameraSet(objId, 2);
    mbObjLayerSet(objId, 4);
    mbPlayerPosGet(playerNo, &pos);
    pos.y += 250.0f;
    mbObjPosSetV(objId, &pos);
    for(i=0; i<15.0f; i++) {
        t = i/15.0f;
        mbObjScaleSet(objId, t, t, t);
        HuPrcVSleep();
    }
    mbObjScaleSet(objId, 1.0f, 1.0f, 1.0f);
    HuPrcSleep(15);
    mbAudFXPlay(0x410);
    for(i=0; i<33.0f; i++) {
        t = i/33.0f;
        scale = HuCos(90.0f*t);
        s = HuSin(90.0f*t);
        pos2.x = pos.x+(2.0*(100.0*(HuSin(270.0f*s)*HuSin(180.0f*t))));
        pos2.z = pos.z+(2.0*(100.0*(HuCos(270.0f*s)*HuSin(180.0f*t))));
        pos2.y = pos.y-(1.5*(100.0*HuSin(90.0f*t)));
        mbObjPosSetV(objId, &pos2);
        mbObjScaleSet(objId, scale, scale, scale);
        HuPrcVSleep();
    }
    mbAudFXPlay(0x479);
    mbCapObjKill(objId);
}

void mbCapMasuDispSet(BOOL dispF)
{
    ev_CapMasuDispF = dispF;
}

static void CapMasuOMExec(OMOBJ *obj)
{
    CAPMASUWORK *work = obj->data;
    int i;
    if(mbExitCheck() || ev_CapMasuOMObj[work->no] == NULL) {
        omDelObjEx(mbObjMan, obj);
        return;
    }
    if(work->hideF) {
        for(i=0; i<4; i++) {
            if(work->masuId == GwPlayer[i].masuId) {
                break;
            }
        }
        if(i >= 4 && GwSystem.turnNo < GwSystem.turnMax && ev_CapMasuDispF) {
            work->scale += 1.0f/30.0f;
            mbObjDispSet(work->objId, TRUE);
            if(work->scale >= 1.0f) {
                work->hideF = FALSE;
                work->scale = 1.0f;
            }
        }
    }
    if(GwSystem.turnNo >= GwSystem.turnMax) {
        ev_CapMasuDispF = FALSE;
    }
    for(i=0; i<4; i++) {
        if(work->masuId == GwPlayer[i].masuIdNext && GwSystem.turnPlayerNo != i) {
            break;
        }
    }
    if(i < 4 || !ev_CapMasuDispF) {
        work->scale -= 1.0f/15.0f;
        mbObjDispSet(work->objId, TRUE);
        if(work->scale <= 0.0f) {
            mbObjDispSet(work->objId, FALSE);
            work->hideF = TRUE;
            work->scale = 0.0f;
        }
    }
    mbMasuPosGet(work->masuId, &work->pos);
    work->pos.y += (100.0f*work->scale)+(work->scale*(10.0*HuSin(work->angle)));
    mbObjPosSetV(work->objId, &work->pos);
    mbObjScaleSet(work->objId, work->scale, work->scale, work->scale);
    if((work->angle += 2) > 360) {
        work->angle -= 360;
    }
}

static int CapHelpWinCreate(int capsuleNo, BOOL deleteF)
{
    int winId;
    if(deleteF == FALSE) {
        if(GWPartyGet() != FALSE) {
            switch(mbCapUseModeGet(capsuleNo)) {
                case 0:
                    winId = mbWinCreateHelp(0x37002C);
                    break;

                case 1:
                case 2:
                    winId = mbWinCreateHelp(0x37002D);
                    break;

                case 3:
                    winId = mbWinCreateHelp(0x37002E);
                    break;
            }
        } else {
            switch(mbCapUseModeGet(capsuleNo)) {
                case 0:
                    winId = mbWinCreateHelp(0x370030);
                    break;

                case 1:
                case 2:
                    winId = mbWinCreateHelp(0x370031);
                    break;

                case 3:
                    winId = mbWinCreateHelp(0x370032);
                    break;
            }
        }
    } else {
        if(GWPartyGet() != FALSE) {
            winId = mbWinCreateHelp(0x37002F);
        } else {
            winId = mbWinCreateHelp(0x370033);
        }
    }
    mbWinAttrSet(winId, 0x800);
    return winId;
}
