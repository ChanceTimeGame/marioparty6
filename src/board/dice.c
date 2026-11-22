#include "game/board/dice.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/effect.h"
#include "game/board/exit.h"
#include "game/board/math.h"
#include "game/board/object.h"
#include "game/board/pause.h"
#include "game/board/player.h"
#include "game/board/telop.h"
#include "game/board/window.h"

#include "game/audio.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/memory.h"
#include "game/pad.h"
#include "game/sprite.h"

#include "math.h"
#include "string.h"

/* snpc.h has void *arg2: the colour is passed by value (a stack copy). */
void mbObjBiriQCreate(s16 objId);
void mbObjBiriQColorSet(s16 modelId, int mode, float time, GXColor color);
/* single.h has u8 arg1; retail passes li r4,-1. */
int mbSingleCall(int arg0, int arg1);

static inline BOOL DicePartyFGet(void)
{
    BOOL partyF = GwSystem.partyF;
    return partyF;
}

#define DICEEFF_PUFF_MAX 50
#define DICEEFF_TRI_MAX 20

typedef struct DiceEff_s {
    HuVecF rot;
    float radius;
    HuVecF scale;
} DICEEFF;

typedef struct DiceWork_s {
    int playerNo; //0x00
    int diceType; //0x04
    int max; //0x08
    int no; //0x0C
    s8 valueTbl[DICE_VALUENUM_MAX]; //0x10
    int valueNum; //0x1C
    int tutorialNum[DICE_NUM_MAX]; //0x20
    BOOL padWinF; //0x2C
    s8 result[DICE_NUM_MAX]; //0x30
    HuVecF pos; //0x34
    int color; //0x40
    OMOBJ *obj; //0x44
    DICEEFF *eff; //0x48
    void *unk4C; //0x4C
    void *dlBuf; //0x50
    u32 dlSize; //0x54
    ANIMDATA *animEffPuff; //0x58
    ANIMDATA *animEffTri; //0x5C
    s8 puffTime; //0x60
    s8 triTime; //0x61
} DICEWORK; //0x64

#define DICEOBJ_MODE_FADEIN 0
#define DICEOBJ_MODE_IDLE 1
#define DICEOBJ_MODE_HIT 2
#define DICEOBJ_MODE_FADEOUT 3

typedef struct DiceObjWork_s {
    u8 killF : 1;
    u8 lockF : 1;
    u8 fadeF : 1;
    u8 mode : 3;
    u8 no : 2;
    s16 lockTime; //0x02
    s16 time; //0x04
    s16 maxTime; //0x06
    s16 valueNo; //0x08
    int seNo; //0x0C
} DICEOBJWORK;

typedef struct DiceFadeWork_s {
    u8 killF : 1;
    u8 fadeF : 1;
    s16 playerNo; //0x02
    u16 time; //0x04
    u16 scaleTime; //0x06
} DICEFADEWORK;

typedef struct DiceNumWork_s {
    u8 killF : 1;
    u8 rotF : 1;
    u8 updateF : 1;
    u8 modelNo : 2;
    u8 bendF : 1;
    u8 playerNo : 2;
    s8 value; //0x01
    u8 color; //0x02
    s16 bendMode; //0x04
    s16 time; //0x06
    s16 maxTime; //0x08
} DICENUMWORK;

typedef struct DiceNumVtx_s {
    HuVecF pos;
    float weight;
} DICENUMVTX;

static const int diceObjFileTbl[DICETYPE_MAX] = {
    0x50017,
    0x50018,
    0x50019,
    0x50022,
    0x5001C,
    0x50020,
    0x5001F,
    0x50017,
    0x5001F,
    0x50017,
    0x30003,
    0x5001D,
    0x5001E,
    0x50021,
    0x5001A,
    0x50017,
    0x50017,
    0x50017,
    0x50018,
    0x50017,
    0x50017
};

static const u8 diceFadeFlagTbl[DICETYPE_MAX] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0
};

static const int numberFileTbl[10] = {
    0x5000B,
    0x5000C,
    0x5000D,
    0x5000E,
    0x5000F,
    0x50010,
    0x50011,
    0x50012,
    0x50013,
    0x50014
};

static HUPROCESS *diceProc[DICE_MAX];
static OMOBJ *diceFadeOMObj[DICE_MAX];
static int diceResult[DICE_MAX];

static MBDICEHITHOOK diceHitHook[DICE_MAX] = {};
static MBDICEPADBTNHOOK dicePadBtnHook[DICE_MAX] = {};
static MBDICEMOTHOOK diceMotHook[DICE_MAX] = {};
static OMOBJ *diceNumOMObj[DICE_MAX][DICE_NUM_MAX] = {};
static HuVecF dice1PosTbl[1] = {
    0, 300, 0
};
static HuVecF dice2PosTbl[2] = {
    -200, 300, 0,
    200, 300, 0
};
static HuVecF dice3PosTbl[3] = {
    -200, 300, 0,
    200, 300, 0,
    0, 300, 0
};
static HuVecF *dicePosTbl[3] = { dice1PosTbl, dice2PosTbl, dice3PosTbl };
static int diceKuriboResultTbl[7] = {
    0, 3, 5, 10, 20, 30, 40
};
static int diceDicePatapataResultTbl[8] = {
    1, 3, 5, 10, 15, 20, 25, -1
};
static s8 diceMaxTbl[DICETYPE_MAX] = {
    1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3
};
static s8 diceValueMaxTbl[DICETYPE_MAX] = {
    10, 10, 10, 5, 10, 1, 1, 7, 0, 0, 1, 5, 5, 6, 10, 10, 10, 10, 10, 10, 10
};
static s8 diceSingleValueMaxTbl[DICETYPE_MAX] = {
    6, 6, 6, 3, 10, 1, 1, 7, 0, 0, 1, 5, 5, 6, 6, 10, 10, 10, 10, 10, 10
};

static void DiceProcMain(void);
static void DiceProcDestroy(void);
static u16 DicePlayerPadBtn(int playerNo);

void mbDiceInit(void)
{
    int i, j;
    for(i=0; i<DICE_MAX; i++) {
        diceProc[i] = NULL;
        diceHitHook[i] = NULL;
        dicePadBtnHook[i] = NULL;
        diceMotHook[i] = NULL;
        diceFadeOMObj[i] = NULL;
        for(j=0; j<DICE_NUM_MAX; j++) {
            diceNumOMObj[i][j] = NULL;
        }
    }
}

static void DiceProcExec(int playerNo, int diceType, s8 *valueTbl, int *tutorialNum, BOOL padWinF, BOOL waitF, HuVecF *pos, int color)
{
    DICEWORK *work;
    int i;
    HuVecF playerPos;
    if(playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    diceProc[playerNo] = HuPrcChildCreate(DiceProcMain, 8203, 24576, 0, mbMainProc);
    HuPrcDestructorSet2(diceProc[playerNo], DiceProcDestroy);
    work = HuMemDirectMallocNum(HEAP_HEAP, sizeof(DICEWORK), HU_MEMNUM_OVL);
    diceProc[playerNo]->property = work;
    memset(work, 0, sizeof(DICEWORK));
    work->playerNo = playerNo;
    work->no = 0;
    work->diceType = diceType;
    work->max = mbDiceMaxGet(diceType);
    if(playerNo < GW_PLAYER_MAX) {
        dicePadBtnHook[playerNo] = DicePlayerPadBtn;
        diceMotHook[playerNo] = mbPlayerDiceMotExec;
    } else {
        dicePadBtnHook[playerNo] = NULL;
        diceMotHook[playerNo] = NULL;
    }
    if(!pos) {
        if(playerNo < GW_PLAYER_MAX) {
            mbPlayerPosGet(work->playerNo, &playerPos);
            work->pos.x = playerPos.x;
            work->pos.y = playerPos.y;
            work->pos.z = playerPos.z;
        } else {
            work->pos.x = work->pos.y = work->pos.z = 0;
        }
    } else {
        work->pos = *pos;
    }
    for(i=0; valueTbl[i] >= 0; i++) {
        work->valueTbl[i] = (s8)valueTbl[i];
    }
    work->valueNum = i;
    if(tutorialNum) {
        for(i=0; i<DICE_NUM_MAX; i++) {
            work->tutorialNum[i] = tutorialNum[i];
        }
    } else {
        work->tutorialNum[0] = work->tutorialNum[1] = work->tutorialNum[2] = -1;
    }
    work->padWinF = padWinF;
    work->color = color;
    if(playerNo < GW_PLAYER_MAX) {
        GwPlayer[playerNo].diceNum = work->max;
    }
    if(waitF) {
        while(!mbDiceKillCheck(playerNo)) {
            HuPrcVSleep();
        }
    }
}

int mbDiceProcExec(int playerNo, int diceType, s8 *valueTbl, int *tutorialNum, BOOL padWinF, BOOL waitF, HuVecF *pos, int color)
{
    s8 valueTblNew[DICE_VALUENUM_MAX];
    int i;
    if(!valueTbl) {
        int max = mbDiceValueMaxGet(diceType);
        for(i=0; i<max; i++) {
            valueTblNew[i] = i;
        }
        valueTblNew[i] = -1;
        DiceProcExec(playerNo, diceType, valueTblNew, tutorialNum, padWinF, waitF, pos, color);
    } else {
        DiceProcExec(playerNo, diceType, valueTbl, tutorialNum, padWinF, waitF, pos, color);
    }
    if(waitF) {
        return mbDiceResultGet(playerNo);
    } else {
        return -1;
    }
}

int mbDiceExec(int playerNo, int diceType, s8 *valueTbl, int tutorialNum, BOOL padWinF, BOOL waitF, HuVecF *pos, int color)
{
    int tutorialNumTbl[DICE_NUM_MAX];
    if(tutorialNum < 0) {
        return mbDiceProcExec(playerNo, diceType, valueTbl, NULL, padWinF, waitF, pos, color);
    }
    tutorialNumTbl[0] = tutorialNum;
    tutorialNumTbl[1] = -1;
    tutorialNumTbl[2] = -1;
    return mbDiceProcExec(playerNo, diceType, valueTbl, tutorialNumTbl, padWinF, waitF, pos, color);
}

int mbDicePlayerExec(int playerNo, int diceType)
{
    return mbDiceProcExec(playerNo, diceType, NULL, NULL, TRUE, TRUE, NULL, 0);
}

int mbDiceChanceTradeExec(int playerNo)
{
    s8 valueTbl[8];
    int i;
    for(i=0; i<7; i++) {
        valueTbl[i] = i;
    }
    valueTbl[i] = -1;
    DiceProcExec(playerNo, DICETYPE_CHANCE_TRADE, valueTbl, NULL, TRUE, TRUE, NULL, 0);
    return mbDiceResultGet(playerNo);
}

int mbDiceChanceCharExec(int playerNo, int *playerNoTbl)
{
    s8 valueTbl[GW_PLAYER_MAX+1];
    int num;
    int i;
    for(num=0, i=0; i<GW_PLAYER_MAX; i++) {
        if(playerNoTbl[i] >= 0) {
            valueTbl[num++] = GwPlayer[playerNoTbl[i]].charNo;
        }
    }
    valueTbl[num] = -1;
    DiceProcExec(playerNo, DICETYPE_CHANCE_CHAR, valueTbl, NULL, TRUE, TRUE, NULL, 0);
    return mbDiceResultGet(playerNo);
}

static void DiceHelpWinCreate(DICEWORK *work);
static void DiceObjCreate(DICEWORK *work);
static void DiceReadyWait(DICEWORK *work);
static void DiceExec(DICEWORK *work);
static void ev_DiceZorome(DICEWORK *work);
static void DiceObjEffCreate(DICEWORK *work);
static void DiceObjEffKill(DICEWORK *work);
static void DiceNumObjReset(int playerNo);

static void DiceProcMain(void)
{
    int i;
    DICEWORK *work = HuPrcCurrentGet()->property;
    int result;
    memset(&work->result[0], 0, DICE_NUM_MAX);
    DiceObjEffCreate(work);
    for(i=0; i<work->max; i++) {
        if(work->padWinF) {
            DiceHelpWinCreate(work);
        }
        DiceObjCreate(work);
        DiceReadyWait(work);
        DiceExec(work);
        if(work->padWinF) {
            mbWinTopKill();
        }
        result = diceResult[work->playerNo];
        if(diceResult[work->playerNo] == DICE_RESULT_FAIL2 ||
            result == DICE_RESULT_FAIL1 ||
            result == DICE_RESULT_FAIL5 ||
            result == DICE_RESULT_Y ||
            result == DICE_RESULT_X ||
            result == DICE_RESULT_CAPSULESEL) {
            HuPrcEnd();
        }
        if(diceMotHook[work->playerNo]) {
            diceMotHook[work->playerNo](work->playerNo);
        }
        while(work->obj) {
            HuPrcVSleep();
        }
        work->no++;
    }
    while(!mbDiceNumStopCheck(work->playerNo)) {
        HuPrcVSleep();
    }
    if(work->max >= 2 && DicePartyFGet() && _CheckFlag(FLAG_BOARD_OPENING) && work->diceType <= DICETYPE_ORDER) {
        for(i=0; i<work->max-1; i++) {
            if(work->result[i] != work->result[i+1]) {
                break;
            }
        }
        if(i >= work->max-1) {
            ev_DiceZorome(work);
        }
    }
    if(work->max >= 2) {
        HuPrcSleep(30);
        DiceNumObjReset(work->playerNo);
        while(!mbDiceNumStopCheck(work->playerNo)) {
            HuPrcVSleep();
        }
    }
    diceResult[work->playerNo] = 0;
    for(i=0; i<work->max; i++) {
        diceResult[work->playerNo] += work->result[i];
    }
    HuPrcEnd();
}

static void DiceProcDestroy(void)
{
    DICEWORK *work = HuPrcCurrentGet()->property;
    mbDiceKill(work->playerNo);
    DiceObjEffKill(work);
    diceProc[work->playerNo] = NULL;
    diceHitHook[work->playerNo] = NULL;
    diceMotHook[work->playerNo] = NULL;
    dicePadBtnHook[work->playerNo] = NULL;
    HuMemDirectFree(work);
}

static void DiceHelpWinCreate(DICEWORK *work)
{
    u32 mess;
    int diceType;
    if(_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        return;
    }
    diceType = work->diceType;
    if(diceType == DICETYPE_PATAPATA_COIN ||
        work->diceType == 6 ||
        diceType == DICETYPE_CHANCE_TRADE ||
        diceType == DICETYPE_CHANCE_CHAR ||
        diceType == 11 ||
        diceType == 15 ||
        diceType == 19 ||
        diceType == 20 ||
        diceType == DICETYPE_KURIBO ||
        diceType == DICETYPE_BLOCK) {
        mess = 0x260002;
    } else {
        {
            if(work->no == 0) {
                int capsuleNum = mbPlayerCapsuleNumGet(work->playerNo);
                if(DicePartyFGet() == FALSE) {
                    capsuleNum = TRUE;
                    if(GwSystem.turnNo <= 1) {
                        capsuleNum = FALSE;
                    }
                }
                if(mbPlayerCapsuleUseGet() != -1 || !capsuleNum) {
                    mess = 0x260001;
                    if(DicePartyFGet() == FALSE) {
                        mess = 0x26000E;
                    }
                } else {
                    mess = 0x260000;
                    if(DicePartyFGet() == FALSE) {
                        mess = 0x26000D;
                    }
                }
            } else {
                mess = 0x260002;
            }
        }
    }
    mbWinCreateHelp(mess);
}

static void DiceObjOMExec(OMOBJ *obj);
static void DiceFadeOMExec(OMOBJ *obj);
static void DiceObjKillSet(OMOBJ *obj);
static void DiceKill(DICEWORK *work);
static s16 DiceInEffCreate(void);
static s16 DiceInDotEffCreate(void);
static void DiceHitEffSet(s16 modelId);
static void DiceObjEffPuffDraw(HU3D_MODEL *modelP, Mtx *mtx);
static void DiceObjEffTriDraw(HU3D_MODEL *modelP, Mtx *mtx);

static void DiceObjCreate(DICEWORK *work)
{
    MBCAMERA *cameraP = mbCameraGet();
    OMOBJ *obj;
    DICEOBJWORK *objWork;
    float time;
    HuVecF parManVec;
    obj = work->obj = omAddObjEx(mbObjMan, 258, 5, 0, -1, DiceObjOMExec);
    omSetStatBit(obj, OM_STAT_MODELPAUSE);
    obj->data = work;
    if(work->diceType != DICETYPE_BLOCK) {
        obj->mdlId[0] = mbObjCreate(mbBoardDataNumGet(diceObjFileTbl[work->diceType]), NULL, FALSE);
    } else {
        obj->mdlId[0] = mbObjCreate(mbBoardDataNumGet(diceObjFileTbl[work->diceType]), NULL, FALSE);
    }
    mbObjLayerSet(obj->mdlId[0], 4);
    mbObjCameraSet(obj->mdlId[0], 2);
    mbAudFXPlay(1007);
    if(mbObjMotionIDGet(obj->mdlId[0], 0) >= 0) {
        mbObjMotionSet(obj->mdlId[0], 0, 0);
        mbObjMotionSpeedSet(obj->mdlId[0], 0);
    }
    time = mbRandMod(work->valueNum)+0.5f;
    mbObjMotionTimeSet(obj->mdlId[0], time);
    mbObjAlphaSet(obj->mdlId[0], 0);
    objWork = omObjGetWork(obj, DICEOBJWORK);
    objWork->killF = FALSE;
    objWork->lockF = FALSE;
    objWork->fadeF = diceFadeFlagTbl[work->diceType];
    objWork->mode = DICEOBJ_MODE_FADEIN;
    objWork->no = work->no;
    objWork->time = 0;
    objWork->maxTime = 20;
    objWork->lockTime = 0;
    objWork->valueNo = time;
    obj->trans.x = work->pos.x;
    obj->trans.y = work->pos.y+250;
    obj->trans.z = work->pos.z;
    mbObjPosSet(obj->mdlId[0], obj->trans.x, obj->trans.y, obj->trans.z);
    obj->mdlId[1] = DiceInEffCreate();
    obj->mdlId[2] = DiceInDotEffCreate();
    Hu3DModelPosSet(obj->mdlId[1], obj->trans.x, obj->trans.y, obj->trans.z);
    Hu3DModelPosSet(obj->mdlId[2], obj->trans.x, obj->trans.y, obj->trans.z);
    VECSubtract(&cameraP->pos, &obj->trans, &parManVec);
    obj->mdlId[3] = Hu3DHookFuncCreate(DiceObjEffPuffDraw);
    Hu3DModelCameraSet(obj->mdlId[3], HU3D_CAM0);
    Hu3DModelLayerSet(obj->mdlId[3], 5);
    Hu3DModelDispOff(obj->mdlId[3]);
    Hu3DData[obj->mdlId[3]].hookData = obj;
    work->puffTime = 0;
    obj->mdlId[4] = Hu3DHookFuncCreate(DiceObjEffTriDraw);
    Hu3DModelCameraSet(obj->mdlId[4], HU3D_CAM0);
    Hu3DModelLayerSet(obj->mdlId[4], 5);
    Hu3DModelDispOff(obj->mdlId[4]);
    Hu3DData[obj->mdlId[4]].hookData = obj;
    work->triTime = 0;
}

static void DiceObjOMExec(OMOBJ *obj)
{
    DICEOBJWORK *objWork = omObjGetWork(obj, DICEOBJWORK);
    DICEWORK *work = obj->data;
    int i;
    float time;
    float scale;
    HuVecF modelPos;
    HuVecF pos;
    if(objWork->killF || mbExitCheck()) {
        if(objWork->seNo >= 0) {
            HuAudFXStop(objWork->seNo);
            objWork->seNo = -1;
        }
        if(obj->mdlId[0] >= 0) {
            mbObjKill(obj->mdlId[0]);
            obj->mdlId[0] = -1;
        }
        mbParticleKill(obj->mdlId[1]);
        mbParticleKill(obj->mdlId[2]);
        obj->mdlId[1] = -1;
        obj->mdlId[2] = -1;
        for(i=3; i<=4; i++) {
            if(obj->mdlId[i] >= 0) {
                Hu3DModelKill(obj->mdlId[i]);
                obj->mdlId[i] = -1;
            }
        }
        obj->data = NULL;
        omDelObjEx(HuPrcCurrentGet(), obj);
        work->obj = NULL;
        return;
    }
    switch(objWork->mode) {
        case DICEOBJ_MODE_FADEIN:
            mbObjAlphaSet(obj->mdlId[0], 255*((float)objWork->time/objWork->maxTime));
            if(++objWork->time > objWork->maxTime) {
                objWork->mode = DICEOBJ_MODE_IDLE;
                objWork->time = 0;
                objWork->maxTime = 12;
                objWork->seNo = mbAudFXPlay(1005);
            }
            break;

        case DICEOBJ_MODE_HIT:
            time = (float)objWork->time/objWork->maxTime;
            if(time > 1) {
                time = 1;
            }
            objWork->time++;
            scale = HuSin(time*180.0f);
            mbObjScaleSet(obj->mdlId[0], 1+scale, 1-(0.5f*scale), 1+scale);
            modelPos.y = obj->trans.y+(150*scale);
            mbObjPosSet(obj->mdlId[0], obj->trans.x, modelPos.y, obj->trans.z);
            if(objWork->time == 8) {
                mbObjPosGet(obj->mdlId[0], &modelPos);
                if(work->diceType != DICETYPE_CHANCE_TRADE &&
                    work->diceType != DICETYPE_CHANCE_CHAR &&
                    work->diceType != 11 &&
                    work->diceType != 12 &&
                    work->diceType != 6 &&
                    work->diceType != DICETYPE_BLOCK) {
                    if(work->playerNo < GW_PLAYER_MAX) {
                        mbPlayerPosGet(work->playerNo, &pos);
                    } else {
                        pos.x = work->pos.x;
                        pos.y = work->pos.y;
                        pos.z = work->pos.z;
                    }
                    VECAdd(&pos, &dicePosTbl[work->max-1][work->no], &pos);
                    if(work->diceType != DICETYPE_KURIBO) {
                        if(work->no == 2) {
                            pos.z += 100;
                        }
                        diceNumOMObj[work->playerNo][work->no] = mbDiceNumObjCreate(work->playerNo, &modelPos, &pos, work->result[work->no], work->color, TRUE);
                    } else {
                        diceNumOMObj[work->playerNo][work->no] = mbDiceNumObjCreate(work->playerNo, &modelPos, &pos, diceKuriboResultTbl[work->result[work->no]], work->color, TRUE);
                    }
                }
            }
            if(objWork->time > objWork->maxTime) {
                objWork->mode = DICEOBJ_MODE_FADEOUT;
                objWork->time = 0;
                objWork->maxTime = 18;
            }
            break;

        case DICEOBJ_MODE_FADEOUT:
            time = (float)objWork->time/15;
            if(time > 1) {
                time = 1;
            }
            if(++objWork->time > objWork->maxTime) {
                if(objWork->fadeF) {
                    diceFadeOMObj[work->playerNo] = omAddObjEx(mbObjMan, 258, 2, 0, -1, DiceFadeOMExec);
                    omSetStatBit(diceFadeOMObj[work->playerNo], OM_STAT_MODELPAUSE);
                    diceFadeOMObj[work->playerNo]->mdlId[0] = obj->mdlId[0];
                    obj->mdlId[0] = -1;
                }
                DiceObjKillSet(obj);
            }
            if(!objWork->fadeF) {
                mbObjAlphaSet(obj->mdlId[0], 255*(1-time));
            }
            break;
    }
    if(!objWork->lockF && objWork->mode < DICEOBJ_MODE_HIT && work->valueNum > 1) {
        if(work->diceType != DICETYPE_ORDER) {
            if(objWork->lockTime++ > 4) {
                s16 valueTbl[DICE_VALUENUM_MAX];
                int valueNum;
                objWork->lockTime = 0;
                valueNum = 0;
                for(i=0; i<work->valueNum; i++) {
                    if(i != objWork->valueNo) {
                        valueTbl[valueNum++] = i;
                    }
                }
                objWork->valueNo = valueTbl[mbRandMod(valueNum)];
            }
        } else {
            if(objWork->lockTime++ > 20) {
                objWork->lockTime = 0;
                objWork->valueNo++;
                if(objWork->valueNo >= work->valueNum) {
                    objWork->valueNo = 0;
                }
            }
        }
        mbObjMotionTimeSet(obj->mdlId[0], work->valueTbl[objWork->valueNo]+0.5f);
    }
}

static void DiceObjKillSet(OMOBJ *obj)
{
    DICEOBJWORK *objWork = omObjGetWork(obj, DICEOBJWORK);
    objWork->killF = TRUE;
}

static void DiceFadeOMExec(OMOBJ *obj)
{
    DICEFADEWORK *objWork = omObjGetWork(obj, DICEFADEWORK);
    float scale;
    float time;
    if(objWork->killF || mbExitCheck()) {
        if(obj->mdlId[0] >= 0) {
            mbObjKill(obj->mdlId[0]);
            obj->mdlId[0] = -1;
        }
        omDelObjEx(HuPrcCurrentGet(), obj);
        diceFadeOMObj[objWork->playerNo] = NULL;
        return;
    }
    objWork->scaleTime++;
    if(objWork->scaleTime >= 30) {
        objWork->scaleTime -= 30;
    }
    time = (1.0f/30.0f)*objWork->scaleTime;
    scale = 1.15f+(0.15f*mbSinDeg((360*time)-90));
    mbObjScaleSet(obj->mdlId[0], scale, scale, scale);
    if(objWork->fadeF) {
        time = objWork->time/15.0f;
        if(time > 1) {
            time = 1;
        }
        if(++objWork->time > 16) {
            objWork->killF = TRUE;
        }
        mbObjAlphaSet(obj->mdlId[0], 255*(1-time));
    }
}

void mbDiceFadeSet(int playerNo)
{
    OMOBJ *obj;
    if(playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    obj = diceFadeOMObj[playerNo];
    if(obj) {
        DICEFADEWORK *objWork = omObjGetWork(obj, DICEFADEWORK);
        objWork->fadeF = TRUE;
        objWork->time = 0;
    }
}

void mbDiceKill(int playerNo)
{
    if(diceProc[playerNo]) {
        DICEWORK *work = diceProc[playerNo]->property;
        DiceKill(work);
        HuPrcKill(diceProc[playerNo]);
    }
}

void mbDiceClose(void)
{
    int i;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        mbDiceKill(i);
    }
}

static void DiceKill(DICEWORK *work)
{
    if(work->obj) {
        DiceObjKillSet(work->obj);
    }
}

static void DiceExec(DICEWORK *work)
{
    u16 btn = 0;
    int diceType;
    diceResult[work->playerNo] = 0;
    while(1) {
        do {
            HuPrcVSleep();
        } while(mbPauseProcCheck());
        if(dicePadBtnHook[work->playerNo]) {
            btn = dicePadBtnHook[work->playerNo](work->playerNo);
        }
        if(btn == PAD_BUTTON_A) {
            break;
        }
        if(work->no != 0 || !_CheckFlag(FLAG_BOARD_OPENING)) {
            continue;
        }
        diceType = work->diceType;
        if(work->diceType == 6 ||
            diceType == 15 ||
            diceType == 19 ||
            diceType == 20 ||
            diceType == DICETYPE_KURIBO ||
            (u32)(diceType-DICETYPE_PATAPATA_COIN) <= 11-DICETYPE_PATAPATA_COIN) {
            continue;
        }
        if(btn == PAD_BUTTON_Y && DicePartyFGet()) {
            diceResult[work->playerNo] = DICE_RESULT_Y;
            mbAudFXPlay(1);
            break;
        }
        if(btn == PAD_BUTTON_X) {
            diceResult[work->playerNo] = DICE_RESULT_X;
            mbAudFXPlay(1);
            break;
        }
        if(btn == PAD_BUTTON_B && work->playerNo < GW_PLAYER_MAX) {
            int capsuleNum = mbPlayerCapsuleNumGet(work->playerNo);
            if(!DicePartyFGet()) {
                capsuleNum = TRUE;
                if(GwSystem.turnNo <= 1) {
                    capsuleNum = FALSE;
                }
            }
            if(mbPlayerCapsuleUseGet() == -1 && capsuleNum) {
                diceResult[work->playerNo] = DICE_RESULT_CAPSULESEL;
                break;
            }
        }
    }
}

static u16 DicePlayerPadBtn(int playerNo)
{
    int valueNo;
    u16 btn;
    if(!GwPlayer[playerNo].comF) {
        btn = HuPadBtnDown[GwPlayer[playerNo].padNo];
    } else {
        btn = PAD_BUTTON_A;
        if(mbDiceTypeGet(playerNo) == DICETYPE_ORDER && mbDiceTutorialNumGet(playerNo) >= 0) {
            valueNo = mbDiceTutorialNumGet(playerNo)-1;
            if(valueNo < 0) {
                valueNo += mbDiceValueMaxGet(DICETYPE_ORDER);
            }
            if(valueNo != mbDiceValueNoGet(playerNo)) {
                btn = 0;
            } else {
                mbDiceTutorialNumSet(playerNo, -1);
            }
        }
    }
    return btn;
}

static void DiceReadyWait(DICEWORK *work)
{
    DICEOBJWORK *objWork = omObjGetWork(work->obj, DICEOBJWORK);
    while(objWork->mode != DICEOBJ_MODE_IDLE) {
        HuPrcVSleep();
    }
}

static void DiceObjHit(DICEWORK *work)
{
    OMOBJ *obj = work->obj;
    DICEOBJWORK *objWork = omObjGetWork(work->obj, DICEOBJWORK);
    objWork->mode = DICEOBJ_MODE_HIT;
    objWork->lockF = TRUE;
    objWork->time = 0;
    objWork->maxTime = 12;
    if(work->playerNo >= 0 && work->playerNo < GW_PLAYER_MAX) {
        omVibrate(work->playerNo, 20, 7, 3);
    }
    if(objWork->seNo >= 0) {
        HuAudFXStop(objWork->seNo);
        objWork->seNo = -1;
    }
    DiceHitEffSet(obj->mdlId[1]);
    DiceHitEffSet(obj->mdlId[2]);
    Hu3DModelDispOn(obj->mdlId[4]);
    if(work->diceType != DICETYPE_CHANCE_TRADE && work->diceType != DICETYPE_CHANCE_CHAR && work->diceType != 11 && work->diceType != 12) {
        if(!DicePartyFGet() && work->playerNo == 0 && !_CheckFlag(FLAG_BOARD_TUTORIAL)) {
            work->tutorialNum[work->no] = mbSingleCall(2, -1);
        }
        if(work->tutorialNum[work->no] >= 0) {
            work->result[work->no] = work->tutorialNum[work->no]+1;
            mbObjMotionTimeSet(obj->mdlId[0], work->tutorialNum[work->no]+0.5f);
        } else {
            work->result[work->no] = work->valueTbl[objWork->valueNo]+1;
        }
        if(work->diceType == DICETYPE_PATAPATA_COIN) {
            work->result[work->no] = diceDicePatapataResultTbl[work->result[work->no]-1];
        }
    } else {
        if(work->tutorialNum[work->no] >= 0) {
            work->result[work->no] = work->tutorialNum[work->no];
            mbObjMotionTimeSet(obj->mdlId[0], work->result[work->no]+0.5f);
        } else {
            work->result[work->no] = mbObjMotionTimeGet(obj->mdlId[0])+0.5f;
        }
    }
    mbAudFXPlay(1008);
    if(diceHitHook[work->playerNo]) {
        diceHitHook[work->playerNo](work->result[work->no]);
    }
}

BOOL mbDiceKillCheck(int playerNo)
{
    if(playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    return diceProc[playerNo] == NULL;
}

BOOL mbDiceKillCheckAll(void)
{
    int i;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        if(!mbDiceKillCheck(i)) {
            return FALSE;
        }
    }
    return TRUE;
}

int mbDiceResultGet(int playerNo)
{
    if(playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    return diceResult[playerNo];
}

void mbDiceHitHookSet(int playerNo, MBDICEHITHOOK hook)
{
    if(playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    diceHitHook[playerNo] = hook;
}

void mbDiceObjHit(int playerNo)
{
    if(playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    DiceObjHit(diceProc[playerNo]->property);
}

void mbDicePadBtnHookSet(int playerNo, MBDICEPADBTNHOOK hook)
{
    if(playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    dicePadBtnHook[playerNo] = hook;
}

void mbDiceMotHookSet(int playerNo, MBDICEMOTHOOK hook)
{
    if(playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    diceMotHook[playerNo] = hook;
}

int mbDiceTypeGet(int playerNo)
{
    DICEWORK *work;
    if(playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    work = diceProc[playerNo]->property;
    return work->diceType;
}

void mbDiceTutorialNumSet(int playerNo, int num)
{
    DICEWORK *work;
    if(playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    work = diceProc[playerNo]->property;
    work->tutorialNum[work->no] = num;
}

int mbDiceTutorialNumGet(int playerNo)
{
    DICEWORK *work;
    if(playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    work = diceProc[playerNo]->property;
    return work->tutorialNum[work->no];
}

int mbDiceValueNoGet(int playerNo)
{
    DICEWORK *work;
    if(playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    work = diceProc[playerNo]->property;
    return omObjGetWork(work->obj, DICEOBJWORK)->valueNo;
}

int mbDiceMaxGet(int diceType)
{
    return diceMaxTbl[diceType];
}

int mbDiceValueMaxGet(int diceType)
{
    if(DicePartyFGet() == FALSE) {
        return diceSingleValueMaxTbl[diceType];
    } else {
        return diceValueMaxTbl[diceType];
    }
}

static void DiceNumObjOMExec(OMOBJ *obj);
static DICENUMVTX *DiceNumObjMdlCopy();
static void DiceNumObjMdlBend();

static void DiceNumPosSet(HuVecF *src, HuVecF *dst)
{
    HU3D_CAMERA *cameraP = &Hu3DCamera[0];
    Mtx lookAt;
    HuVecF normPos;
    HuVecF camPos;
    float tanFov;
    float x, y;
    MTXLookAt(lookAt, &cameraP->pos, &cameraP->up, &cameraP->target);
    MTXMultVec(lookAt, src, &camPos);
    tanFov = HuSin(cameraP->fov*0.5f)/HuCos(cameraP->fov*0.5f);
    x = camPos.x/(tanFov*camPos.z);
    y = camPos.y/(tanFov*camPos.z);
    normPos.x = -2000*(x*tanFov);
    normPos.y = -2000*(y*tanFov);
    normPos.z = -2000;
    MTXInverse(lookAt, lookAt);
    MTXMultVec(lookAt, &normPos, dst);
}

OMOBJ *mbDiceNumObjCreate(int playerNo, HuVecF *pos1, HuVecF *pos2, int value, int color, BOOL followF)
{
    static GXColor biriQColor = { 255, 255, 255, 255 };
    DICENUMWORK *objWork;
    DICENUMVTX **vtx;
    MBCAMERA *cameraP;
    int modelNo;
    MBOBJID modelId;
    HU3D_MODELID tempMdlId;
    int digit;
    OMOBJ *obj;
    Mtx rot;

    cameraP = mbCameraGet();
    if(value > 99) {
        value = 99;
    }
    obj = omAddObjEx(mbObjMan, 258, 3, 0, -1, DiceNumObjOMExec);
    omSetStatBit(obj, OM_STAT_MODELPAUSE);
    obj->data = vtx = HuMemDirectMallocNum(HEAP_HEAP, sizeof(DICENUMVTX *)*2, HU_MEMNUM_OVL);
    memset(obj->data, 0, sizeof(DICENUMVTX *)*2);
    objWork = omObjGetWork(obj, DICENUMWORK);
    objWork->killF = FALSE;
    objWork->rotF = FALSE;
    objWork->updateF = TRUE;
    objWork->value = value;
    objWork->time = 0;
    objWork->maxTime = 30;
    objWork->playerNo = playerNo;
    objWork->color = color;
    modelNo = 0;
    digit = value;
    MTXRotDeg(rot, 'y', cameraP->rot.y);
    while(1) {
        modelId = (s16)mbObjCreate(mbBoardDataNumGet(numberFileTbl[digit%10]), NULL, FALSE);
        vtx[modelNo] = DiceNumObjMdlCopy(mbObjModelIDGet(modelId));
        mbObjPosSetV(modelId, pos1);
        mbObjMotionSet(modelId, 0, 0);
        mbObjMotionSpeedSet(modelId, 0);
        mbObjMotionTimeSet(modelId, color+0.5f);
        mbObjScaleSet(modelId, 1, 1, 1);
        mbObjMtxSet(modelId, rot);
        tempMdlId = mbObjModelIDGet(modelId);
        mbObjCameraSet(modelId, 2);
        Hu3DModelLayerSet(tempMdlId, 4);
        mbObjBiriQCreate(modelId);
        mbObjBiriQColorSet(modelId, 1, 0, biriQColor);
        obj->mdlId[modelNo] = modelId;
        modelNo++;
        digit = digit/10;
        if(digit == 0) {
            break;
        }
    }
    objWork->modelNo = modelNo;
    for(; modelNo<2; modelNo++) {
        obj->mdlId[modelNo] = -1;
    }
    obj->trans.x = obj->scale.x = pos1->x;
    obj->trans.y = obj->scale.y = pos1->y;
    obj->trans.z = obj->scale.z = pos1->z;
    if(pos2) {
        obj->rot.x = pos2->x;
        obj->rot.y = pos2->y;
        obj->rot.z = pos2->z;
        if(fabsf2(obj->rot.x-obj->trans.x) > 25) {
            obj->rot.y += -50;
        } else if(fabsf2(obj->rot.z-obj->trans.z) > 25) {
            obj->rot.y -= -50;
        }
    } else {
        obj->rot.x = pos1->x;
        obj->rot.y = pos1->y;
        obj->rot.z = pos1->z;
    }
    if(!followF) {
        DiceNumPosSet(&obj->rot, &obj->rot);
        DiceNumPosSet(&obj->trans, &obj->scale);
    }
    return obj;
}

static void DiceNumObjOMExec(OMOBJ *obj)
{
    static GXColor biriQColor = { 255, 255, 255, 255 };
    DICENUMWORK *objWork = omObjGetWork(obj, DICENUMWORK);
    DICENUMVTX **vtx = obj->data;
    float time;
    HuVecF modelPos;
    HuVecF bendPos;
    HuVecF basePos;
    HuVecF dir;
    int i;
    Mtx rot1, rot2;
    if(objWork->killF || mbExitCheck()) {
        mbDiceNumObjKill(obj);
        return;
    }
    if(!objWork->updateF) {
        return;
    }
    if(++objWork->time >= objWork->maxTime) {
        objWork->updateF = FALSE;
    }
    time = (float)objWork->time/objWork->maxTime;
    dir.z = HuCos(time*90);
    dir.y = HuSin(time*90);
    dir.x = HuSin(time*180);
    for(i=0; i<2; i++) {
        float angle;
        if(obj->mdlId[i] < 0) {
            continue;
        }
        angle = 450*time;
        if(angle > 360) {
            angle = 360;
        }
        MTXRotDeg(rot2, 'y', angle);
        angle = (450*time)-90;
        if(angle < 0) {
            angle = 0;
        }
        MTXRotDeg(rot1, 'y', angle);
        DiceNumObjMdlBend(mbObjModelIDGet(obj->mdlId[i]), vtx[i], rot1, rot2);
        if(objWork->modelNo < 2) {
            bendPos.x = obj->rot.x;
            bendPos.y = obj->rot.y;
            bendPos.z = obj->rot.z;
        } else {
            Mtx modelMtx;
            mbObjMtxGet(obj->mdlId[i], modelMtx);
            if(i != 0) {
                basePos.x = -50*1.2f;
                basePos.y = basePos.z = 0;
            } else {
                basePos.x = 50*1.2f;
                basePos.y = basePos.z = 0;
            }
            MTXMultVec(modelMtx, &basePos, &bendPos);
            bendPos.x += obj->rot.x;
            bendPos.y += obj->rot.y;
            bendPos.z += obj->rot.z;
        }
        modelPos.x = obj->trans.x+(time*(bendPos.x-obj->trans.x));
        if(!objWork->rotF) {
            modelPos.y = obj->trans.y+(time*(bendPos.y-obj->trans.y))+(100*(1.1f*HuSin(time*180)));
        } else {
            modelPos.y = obj->trans.y+(time*(bendPos.y-obj->trans.y));
        }
        modelPos.z = obj->trans.z+(time*(bendPos.z-obj->trans.z));
        mbObjPosSetV(obj->mdlId[i], &modelPos);
        if(objWork->rotF) {
            float alpha = time-0.3f;
            if(alpha > 0) {
                alpha *= 2.5f;
                alpha *= alpha;
                if(alpha > 1) {
                    alpha = 1;
                }
                mbObjBiriQColorSet(obj->mdlId[i], 1, alpha, biriQColor);
            }
        }
    }
}

static void DiceNumObjBendOMExec(OMOBJ *obj)
{
    DICENUMWORK *objWork = omObjGetWork(obj, DICENUMWORK);
    DICENUMVTX **vtx = obj->data;
    float angle;
    float time;
    float t;
    float rotAngle;
    float posY;
    Mtx rot1, rot2;
    HuVecF pos;
    int i;
    if(objWork->killF || mbExitCheck()) {
        mbDiceNumObjKill(obj);
        return;
    }
    if(!objWork->updateF) {
        return;
    }
    t = 0;
    switch(objWork->bendMode) {
        case 0:
            time = (float)objWork->time/objWork->maxTime;
            if(objWork->value == 10) {
                rotAngle = 30;
            } else {
                rotAngle = 60;
            }
            MTXRotDeg(rot2, 'z', rotAngle*HuSin(time*720));
            MTXIdentity(rot1);
            posY = obj->rot.y;
            if(++objWork->time > objWork->maxTime) {
                if(objWork->bendF) {
                    objWork->bendMode = 1;
                    objWork->time = 0;
                    objWork->maxTime = 45;
                } else {
                    objWork->updateF = FALSE;
                }
            }
            t = fmod(2*time, 1.0);
            break;

        case 1:
            t = (float)objWork->time/objWork->maxTime;
            angle = t*450;
            if(angle > 360) {
                angle = 360;
            }
            MTXRotDeg(rot2, 'y', angle);
            angle = (450*t)-90;
            if(angle < 0) {
                angle = 0;
            }
            MTXRotDeg(rot1, 'y', angle);
            posY = obj->rot.y+(100*(2*(HuSin(t*180))));
            if(++objWork->time > objWork->maxTime) {
                if(objWork->bendF) {
                    objWork->bendMode = 0;
                    objWork->time = 0;
                    objWork->maxTime = 120;
                } else {
                    objWork->updateF = FALSE;
                }
            }
            break;
    }
    for(i=0; i<2; i++) {
        GXColor color = { 255, 255, 255, 255 };
        if(obj->mdlId[i] >= 0) {
            DiceNumObjMdlBend(mbObjModelIDGet(obj->mdlId[i]), vtx[i], rot1, rot2);
            mbObjPosGet(obj->mdlId[i], &pos);
            pos.y = posY;
            mbObjPosSetV(obj->mdlId[i], &pos);
            mbObjBiriQColorSet(obj->mdlId[i], 1, fabsf2(0.4f*mbSinDeg(360*t)), color);
        }
    }
    if(obj->mdlId[2] >= 0) {
        ((HU3D_PARTICLE *)Hu3DData[obj->mdlId[2]].hookData)->unk_10.y = posY-obj->rot.y;
    }
}

void mbDiceNumObjKill(OMOBJ *obj)
{
    int i;
    for(i=0; i<2; i++) {
        if(obj->mdlId[i] != -1) {
            mbObjKill(obj->mdlId[i]);
        }
    }
    omDelObjEx(HuPrcCurrentGet(), obj);
}

static void DiceNumObjShrinkOMExec(OMOBJ *obj)
{
    DICENUMWORK *objWork = omObjGetWork(obj, DICENUMWORK);
    HuVecF pos;
    float time;
    float scaleX, scaleY;
    float posY;
    int i;
    if(objWork->time > objWork->maxTime) {
        objWork->killF = TRUE;
    }
    if(objWork->killF || mbExitCheck()) {
        mbDiceNumObjKill(obj);
        return;
    }
    time = (float)(objWork->time++)/objWork->maxTime;
    scaleX = HuSin(time*270);
    scaleY = HuSin((time*270)+180);
    posY = obj->rot.y+(100*time);
    for(i=0; i<2; i++) {
        if(obj->mdlId[i] >= 0) {
            mbObjScaleSet(obj->mdlId[i], 1+scaleX, 1+scaleY, 1);
            mbObjPosGet(obj->mdlId[i], &pos);
            pos.y = posY;
            mbObjPosSetV(obj->mdlId[i], &pos);
        }
    }
}

/* ---- Part 2 (wt-b206): placeholders so the unit compiles; the coordinator takes part 2 from that branch. ---- */

static void DiceNumObjReset(int playerNo)
{
    int i;
    for(i=0; i<DICE_NUM_MAX; i++) {
        OMOBJ *obj = diceNumOMObj[playerNo][i];
        if(obj) {
            DICENUMWORK *objWork = omObjGetWork(obj, DICENUMWORK);
            objWork->rotF = TRUE;
            objWork->updateF = TRUE;
            objWork->time = 0;
            objWork->maxTime = 30;
            obj->trans.x = obj->rot.x;
            obj->trans.y = obj->rot.y;
            obj->trans.z = obj->rot.z;
            obj->rot.x = obj->scale.x;
            obj->rot.z = obj->scale.z;
            obj->objFunc = DiceNumObjOMExec;
        }
    }
}

BOOL mbDiceNumStopCheck(int playerNo)
{
    int i;
    for(i=0; i<DICE_NUM_MAX; i++) {
        if(diceNumOMObj[playerNo][i]) {
            DICENUMWORK *objWork = omObjGetWork(diceNumOMObj[playerNo][i], DICENUMWORK);
            if(objWork->updateF) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

void mbDiceNumKill(int playerNo)
{
    int i;
    if(playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    for(i=0; i<DICE_NUM_MAX; i++) {
        if(diceNumOMObj[playerNo][i]) {
            DICENUMWORK *objWork = omObjGetWork(diceNumOMObj[playerNo][i], DICENUMWORK);
            objWork->killF = TRUE;
            diceNumOMObj[playerNo][i] = NULL;
        }
    }
}

void mbDiceNumShrinkSet(int playerNo)
{
    int i;
    if(playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    for(i=0; i<DICE_NUM_MAX; i++) {
        OMOBJ *obj = diceNumOMObj[playerNo][i];
        if(obj) {
            DICENUMWORK *objWork = omObjGetWork(obj, DICENUMWORK);
            objWork->time = 0;
            objWork->maxTime = 30;
            obj->objFunc = DiceNumObjShrinkOMExec;
        }
    }
}

void mbDiceStub(void)
{
}

OMOBJ *mbDiceSNpcNumCreate(int playerNo, HuVecF *pos)
{
    return NULL;
}

static void ev_DiceZorome(DICEWORK *work)
{
    (void)DiceNumObjBendOMExec;
}

static s16 DiceInEffCreate(void)
{
    return -1;
}

static s16 DiceInDotEffCreate(void)
{
    return -1;
}

static void DiceHitEffSet(s16 modelId)
{
}

static void DiceObjEffCreate(DICEWORK *work)
{
}

static void DiceObjEffKill(DICEWORK *work)
{
}

static void DiceObjEffPuffDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
}

static void DiceObjEffTriDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
}

static DICENUMVTX *DiceNumObjMdlCopy(int modelId)
{
    return NULL;
}

static void DiceNumObjMdlBend(int modelId, DICENUMVTX *vtx, Mtx mtx1, Mtx mtx2)
{
}

OMOBJ *mbDiceSNpcNumObjCreate(HuVecF *pos, HuVecF *ofs, int value, int flag, int color)
{
    return NULL;
}

void mbDiceSNpcNumKill(OMOBJ *obj)
{
}

void mbDiceSNpcNumDispSet(OMOBJ *obj, int dispF)
{
}

void mbDiceSNpcNumSet(OMOBJ *obj, int value)
{
}

void mbDiceSNpcNumPosSet(OMOBJ *obj, HuVecF *pos)
{
}

void mbDiceSNpcNumOfsSet(OMOBJ *obj, HuVecF *ofs)
{
}

void mbDiceSNpcNumOfsGet(OMOBJ *obj, HuVecF *ofs)
{
}
