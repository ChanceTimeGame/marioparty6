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

#include "game/data.h"
#include "game/frand.h"
#include "game/hsfformat.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/process.h"
#include "datadir_enum.h"
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

/* Part 2 (fork 6) declarations. */
/* Board coin objects take s16 ids here (the extsh before each call); coin.h has int. */
s16 mbCoinCreate2(void);
s16 mbCoinObjCreate(int digit, int color);
void mbCoinObjNumDec(s16 id);
void mbCoinObjKill(s16 id);
void mbCoinObjPosSet(s16 id, float x, float y, float z);
void mbCoinObjPosSetV(s16 id, HuVecF *pos);
void mbCoinObjPosGet(s16 id, HuVecF *pos);
void mbCoinObjRotSet(s16 id, float x, float y, float z);
void mbCoinObjScaleSet(s16 id, float x, float y, float z);
void mbCoinObjDispSet(s16 id, int dispF);
void mbCoinObjLayerSet(s16 id, u16 layer);
int mbCoinAddExec(int playerNo, int coinNum);
/* Retail calls sprintf unprototyped (no crclr). */
int sprintf();

typedef struct DiceSNpcNumWork_s {
    unsigned killF : 1;
    unsigned dispF : 1;
    unsigned unkF : 1;
    u8 value;
} DICESNPCNUMWORK;

typedef struct DiceParticleData_s {
    s16 time; /* 0x00 */
    u8 unk2[6];
    HuVecF vel; /* 0x08 */
    float scaleBase; /* 0x14 */
    float accelY; /* 0x18 */
    float velDecay; /* 0x1C */
    float scaleSpeed; /* 0x20 */
    float unk24;
    float rotSpeed; /* 0x28 */
    float scale; /* 0x2C */
    float unk30; /* 0x30 */
    float unk34; /* 0x34 */
    float zRot; /* 0x38 */
    HuVecF pos; /* 0x3C */
    GXColor color; /* 0x48 */
    s16 animNo; /* 0x4C */
    u8 unk4E[0x5C-0x4E];
} DICEPARTICLEDATA;

typedef struct DiceParticle_s {
    s16 endF; /* 0x00 */
    s16 mode; /* 0x02 */
    u8 unk4[0x14-0x4];
    float unk14; /* 0x14 */
    u8 unk18[0x20-0x18];
    u8 unk20; /* 0x20 */
    u8 unk21[0x26-0x21];
    s16 num; /* 0x26 */
    u32 initF; /* 0x28 */
    u8 unk2C[0x68-0x2C];
    DICEPARTICLEDATA *data; /* 0x68 */
} DICEPARTICLE;

static char diceMatchCoinStr[8];

static void DiceNumObjReset(int playerNo)
{
    int i;
    for(i=0; i<3; i++) {
        OMOBJ *obj = diceNumOMObj[playerNo][i];
        if(obj) {
            DICENUMWORK *work = omObjGetWork(obj, DICENUMWORK);
            work->rotF = TRUE;
            work->updateF = TRUE;
            work->time = 0;
            work->maxTime = 30;
            obj->trans.x = obj->rot.x;
            obj->trans.y = obj->rot.y;
            obj->trans.z = obj->rot.z;
            if(fabsf2(obj->rot.x-obj->scale.x) > 25.0f) {
                obj->rot.y -= -50.0f;
            } else if(fabsf2(obj->rot.z-obj->scale.z) > 25.0f) {
                obj->rot.y += -50.0f;
            }
            obj->rot.x = obj->scale.x;
            obj->rot.z = obj->scale.z;
            obj->objFunc = DiceNumObjOMExec;
        }
    }
}

BOOL mbDiceNumStopCheck(int playerNo)
{
    int i;
    for(i=0; i<3; i++) {
        if(diceNumOMObj[playerNo][i]) {
            DICENUMWORK *work = omObjGetWork(diceNumOMObj[playerNo][i], DICENUMWORK);
            if(work->updateF) {
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
    for(i=0; i<3; i++) {
        if(diceNumOMObj[playerNo][i]) {
            DICENUMWORK *work = omObjGetWork(diceNumOMObj[playerNo][i], DICENUMWORK);
            work->killF = TRUE;
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
    for(i=0; i<3; i++) {
        OMOBJ *obj = diceNumOMObj[playerNo][i];
        if(obj) {
            DICENUMWORK *work = omObjGetWork(obj, DICENUMWORK);
            work->time = 0;
            work->maxTime = 30;
            obj->objFunc = DiceNumObjShrinkOMExec;
        }
    }
}

void mbDiceStub(void)
{
}

OMOBJ *mbDiceSNpcNumCreate(int playerNo, HuVecF *pos)
{
    OMOBJ *obj;
    int i;
    OMOBJ *result = NULL;
    int value = 0;
    int color = 0;
    BOOL foundF = FALSE;
    HuVecF rot;
    HuVecF ofs;
    if(playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    for(i=0; i<3; i++) {
        obj = diceNumOMObj[playerNo][i];
        if(obj) {
            DICENUMWORK *work = omObjGetWork(obj, DICENUMWORK);
            value += work->value;
            if(!foundF) {
                color = work->color;
                mbVecCopy(&obj->rot, &rot);
                VECSubtract(&rot, pos, &ofs);
                foundF = TRUE;
            }
            work->killF = TRUE;
            diceNumOMObj[playerNo][i] = NULL;
        }
    }
    if(foundF) {
        result = mbDiceSNpcNumObjCreate(pos, &ofs, value, FALSE, color);
    }
    return result;
}

static void ev_DiceZoromeCoin(int playerNo, int coinNum);
static void DiceZoromeEffHook(HU3D_MODEL *modelP, DICEPARTICLE *particleP, Mtx mtx);

static void DiceNumObjBendStart(int playerNo)
{
    OMOBJ **numObj;
    int i;
    for(i=0, numObj=diceNumOMObj[playerNo]; i<3; numObj++, i++) {
        OMOBJ *obj = *numObj;
        if(obj) {
            DICENUMWORK *work = omObjGetWork(obj, DICENUMWORK);
            work->updateF = TRUE;
            work->bendF = TRUE;
            work->time = 0;
            work->maxTime = 120;
            obj->mdlId[2] = mbParticleCreate(HuSprAnimRead(HuDataSelHeapReadNum(mbBoardDataNumGet(DATANUM(DATA_board, 0x64)), HU_MEMNUM_OVL, HEAP_MODEL)), 100);
            mbParticleHookSet(obj->mdlId[2], DiceZoromeEffHook);
            Hu3DModelCameraSet(obj->mdlId[2], 2);
            Hu3DModelLayerSet(obj->mdlId[2], 5);
            Hu3DModelPosSet(obj->mdlId[2], obj->rot.x, 30.000002f+obj->rot.y, 50.0f+obj->rot.z);
            obj->objFunc = DiceNumObjBendOMExec;
        }
    }
}

static void DiceNumObjBendStop(int playerNo)
{
    int i;
    DICENUMWORK *work;
    int j;
    OMOBJ *obj;
    for(i=0; i<3; i++) {
        obj = diceNumOMObj[playerNo][i];
        if(obj) {
            work = omObjGetWork(obj, DICENUMWORK);
            for(j=0; j<2; j++) {
                if(obj->mdlId[j] > 0) {
                    static GXColor biriQColor = { 0, 0, 0, 0 };
                    mbObjBiriQColorSet(obj->mdlId[j], 0, 0.0f, biriQColor);
                }
            }
            if(obj->mdlId[2] >= 0) {
                mbParticleKill(obj->mdlId[2]);
                obj->mdlId[2] = -1;
            }
            work->bendF = FALSE;
        }
    }
}

static void ev_DiceZorome(DICEWORK *work)
{
    int coin;
    int streamId;
    HuVecF playerPos;
    switch(work->max) {
        case 2:
            if(work->result[0] == 7) {
                coin = 30;
            } else {
                coin = 10;
            }
            streamId = 40;
            break;

        case 3:
            if(work->result[0] == 7) {
                coin = 50;
            } else {
                coin = 30;
            }
            streamId = 41;
            break;

        default:
            return;
    }
    sprintf(diceMatchCoinStr, "%d", coin);
    mbMusPauseFadeOut(0, TRUE, 1000);
    mbPlayerPosGet(work->playerNo, &playerPos);
    DiceNumObjBendStart(work->playerNo);
    HuPrcSleep(10);
    mbMusJingleWait(mbMusJinglePlay(streamId));
    mbMusPauseFadeOut(0, FALSE, 1000);
    mbAudGuidePlay(950);
    mbWinCreate(2, 0x26000A, mbGuideSpeakerNoGet());
    mbWinTopInsertMesSet(MESSNUM_PTR(diceMatchCoinStr), 0);
    mbWinTopWait();
    ev_DiceZoromeCoin(work->playerNo, coin);
    DiceNumObjBendStop(work->playerNo);
    while(!mbDiceNumStopCheck(work->playerNo)) {
        HuPrcVSleep();
    }
    mbMusBoardPlay();
}

static u8 diceInEffAnimTbl[8] = { 0, 1, 2, 2, 3, 3, 3, 3 };

static void ev_DiceZoromeCoin(int playerNo, int coinNum)
{
    int i;
    int id;
    int num;
    int activeNum = 0;
    int coinNo;
    int coinId[64];
    float velY[64];
    HuVecF pos;
    HuVecF playerPos;
    float rotY;
    for(i=0; i<64; i++) {
        coinId[i] = 0;
    }
    mbPlayerPosGet(playerNo, &playerPos);
    num = coinNum;
    while(num != 0 || activeNum != 0) {
        if(num != 0) {
            for(i=0; i<64; i++) {
                if(coinId[i] == 0) {
                    break;
                }
            }
            if(i < 64) {
                coinId[i] = mbCoinCreate2();
                velY[i] = -13.333334f;
                id = coinId[i];
                num--;
                pos.x = playerPos.x+(60.000004f*(frandf()-0.5f));
                pos.y = playerPos.y+(50.0f*frandf());
                pos.z = playerPos.z+(60.000004f*(frandf()-0.5f));
                pos.y += 800.0f;
                mbCoinObjPosSetV(id, &pos);
                rotY = 360.0f*frandf();
                mbCoinObjRotSet(id, 40.0f*(frandf()-0.5f), rotY, 0.0f);
                mbCoinObjScaleSet(id, 0.7f, 0.7f, 0.7f);
            }
        }
        activeNum = 0;
        for(i=0; i<64; i++) {
            if(coinId[i] != 0) {
                coinNo = coinId[i];
                activeNum++;
                mbCoinObjPosGet(coinNo, &pos);
                velY[i] += -0.5444445f;
                pos.y += velY[i];
                mbCoinObjPosSetV(coinNo, &pos);
                if(pos.y < 100.0f+playerPos.y) {
                    mbCoinObjKill(coinNo);
                    coinId[i] = 0;
                }
            }
        }
        HuPrcVSleep();
    }
    mbCoinAddExec(playerNo, coinNum);
}

static u8 diceMatchEffAnimBankTbl[16] = { 0, 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };
static float diceMatchEffScale[4] = { 1.0f, 1.0f, 1.0f, 0.6f };
static GXColor diceMatchEffColorTbl[8] = {
    { 220, 64, 64, 0 },
    { 64, 220, 64, 0 },
    { 220, 220, 64, 0 },
    { 64, 64, 220, 0 },
    { 220, 64, 220, 0 },
    { 64, 220, 220, 0 },
    { 220, 120, 64, 0 },
    { 64, 120, 220, 0 },
};

static void DiceZoromeEffHook(HU3D_MODEL *modelP, DICEPARTICLE *particleP, Mtx mtx)
{
    int i;
    int num;
    DICEPARTICLEDATA *dataP;
    int no;
    int colorNo;
    float angle;
    float elev;
    float t;
    float alpha;
    HuVecF ofs;
    num = 3;
    if(particleP->initF == 0 || particleP->endF != 0) {
        num = 0.5f*particleP->num;
        particleP->initF = 1;
        particleP->endF = 0;
        particleP->unk20 = 1;
        particleP->unk14 = 0.0f;
    }
    for(dataP=particleP->data, i=0; i<particleP->num; i++, dataP++) {
        if(num <= 0) {
            break;
        }
        if(dataP->time != 0) {
            continue;
        }
        no = diceMatchEffAnimBankTbl[mbRandMod(16)];
        dataP->animNo = no;
        dataP->pos.x = dataP->pos.y = dataP->pos.z = 0.0f;
        dataP->pos.y += particleP->unk14;
        angle = 360.0f*frandf();
        elev = (1.6f*frandf())-0.8f;
        elev = 90.0f*(elev*fabsf2(elev));
        dataP->vel.x = HuSin(angle)*HuCos(elev);
        dataP->vel.y = HuSin(elev);
        dataP->vel.z = HuCos(angle)*HuCos(elev);
        VECScale(&dataP->vel, &dataP->vel, (1.0f/60.0f)*(100.0f+(100.0f*(2.0f*frandf()))));
        VECScale(&dataP->vel, &ofs, 20.0f);
        VECAdd(&dataP->pos, &ofs, &dataP->pos);
        VECScale(&dataP->vel, &dataP->vel, 0.25f);
        dataP->scale = diceMatchEffScale[no]*(25.0f*(0.5f+(0.5f*frandf())));
        dataP->scaleBase = dataP->scale;
        colorNo = mbRandMod(8);
        t = 0.2f+(0.5f*frandf());
        dataP->color.r = (t*(255.0f-diceMatchEffColorTbl[colorNo].r))+diceMatchEffColorTbl[colorNo].r;
        dataP->color.g = (t*(255.0f-diceMatchEffColorTbl[colorNo].g))+diceMatchEffColorTbl[colorNo].g;
        dataP->color.b = (t*(255.0f-diceMatchEffColorTbl[colorNo].b))+diceMatchEffColorTbl[colorNo].b;
        dataP->color.a = mbRandMod(70)+150;
        dataP->accelY = dataP->color.a;
        dataP->color.a = 0;
        dataP->zRot = 0.0f;
        if(dataP->animNo < 3) {
            dataP->zRot = 360.0f*frandf();
        }
        dataP->time = mbRandMod(15)+15;
        num--;
    }
    switch(particleP->mode) {
        case 0:
            for(dataP=particleP->data, i=0; i<particleP->num; i++, dataP++) {
                if(dataP->time == 0) {
                    continue;
                }
                VECScale(&dataP->vel, &dataP->vel, 0.96f);
                VECAdd(&dataP->pos, &dataP->vel, &dataP->pos);
                dataP->vel.y += 0.08333334f;
                dataP->scale -= 0.4f;
            }
            break;

        case 1:
            for(dataP=particleP->data, i=0; i<particleP->num; i++, dataP++) {
                if(dataP->time == 0) {
                    continue;
                }
                VECAdd(&dataP->pos, &dataP->vel, &dataP->pos);
                dataP->vel.y += -0.41666672f;
            }
            break;
    }
    for(dataP=particleP->data, i=0; i<particleP->num; i++, dataP++) {
        if(dataP->time == 0) {
            continue;
        }
        dataP->time--;
        if(dataP->time < 10) {
            dataP->scale *= 0.7f;
            if(dataP->time == 0) {
                dataP->color.a = 0;
                dataP->scale = 0.0f;
            }
        } else {
            alpha = dataP->color.a;
            alpha = alpha+(1.0f+(0.2f*(dataP->accelY-alpha)));
            dataP->color.a = alpha;
        }
    }
}

static void DiceInEffHook(HU3D_MODEL *modelP, DICEPARTICLE *particleP, Mtx mtx);
static void DiceInDotEffHook(HU3D_MODEL *modelP, DICEPARTICLE *particleP, Mtx mtx);

static s16 DiceInEffCreate(void)
{
    int modelId = mbParticleCreate(HuSprAnimRead(HuDataSelHeapReadNum(mbBoardDataNumGet(DATANUM(DATA_board, 0x64)), HU_MEMNUM_OVL, HEAP_MODEL)), 80);
    mbParticleHookSet(modelId, DiceInEffHook);
    Hu3DModelLayerSet(modelId, 5);
    mbParticleAttrSet(modelId, 16);
    mbParticleColorCreate(modelId);
    return modelId;
}

static s16 DiceInDotEffCreate(void)
{
    int modelId = mbParticleCreate(HuSprAnimRead(HuDataSelHeapReadNum(mbBoardDataNumGet(DATANUM(DATA_board, 0x64)), HU_MEMNUM_OVL, HEAP_MODEL)), 150);
    mbParticleHookSet(modelId, DiceInDotEffHook);
    Hu3DModelLayerSet(modelId, 5);
    return modelId;
}

static void DiceHitEffSet(s16 modelId)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    DICEPARTICLE *particleP = modelP->hookData;
    particleP->endF = 1;
    particleP->mode = 1;
}

static float diceInEffScaleRatio[4] = { 1.0f, 1.0f, 0.7f, 0.4f };
static GXColor diceInEffColorTbl[8] = {
    { 220, 100, 100, 0 },
    { 100, 220, 100, 0 },
    { 220, 220, 100, 0 },
    { 100, 100, 220, 0 },
    { 220, 100, 220, 0 },
    { 100, 220, 220, 0 },
    { 220, 150, 100, 0 },
    { 100, 150, 220, 0 },
};

static void DiceInEffHook(HU3D_MODEL *modelP, DICEPARTICLE *particleP, Mtx mtx)
{
    int i;
    int no;
    DICEPARTICLEDATA *dataP;
    int colorNo;
    float angle;
    float elev;
    float t;
    if(particleP->initF == 0 || particleP->endF != 0) {
        dataP = particleP->data;
        switch(particleP->mode) {
            case 0:
                for(i=0; i<particleP->num; i++, dataP++) {
                    no = diceInEffAnimTbl[mbRandMod(4)];
                    dataP->animNo = no;
                    colorNo = mbRandMod(8);
                    dataP->color.r = diceInEffColorTbl[colorNo].r+mbRandMod(20);
                    dataP->color.g = diceInEffColorTbl[colorNo].g+mbRandMod(20);
                    dataP->color.b = diceInEffColorTbl[colorNo].b+mbRandMod(20);
                    dataP->color.a = 250;
                    dataP->pos.x = dataP->pos.y = dataP->pos.z = 0.0f;
                    angle = 360.0f*frandf();
                    t = (1.6f*frandf())-0.8f;
                    elev = 90.0f*(t*fabsf2(t));
                    dataP->vel.x = HuSin(angle)*HuCos(elev);
                    dataP->vel.y = HuSin(elev);
                    dataP->vel.z = HuCos(angle)*HuCos(elev);
                    VECScale(&dataP->vel, &dataP->vel, (1.0f/60.0f)*(800.0f+(100.0f*(8.0f*frandf()))));
                    dataP->scale = diceInEffScaleRatio[no]*(60.0f*(0.5f+(0.7f*frandf())));
                    dataP->scaleBase = dataP->scale;
                    dataP->scaleSpeed = 0.5f;
                    dataP->unk30 = 360.0f*frandf();
                    dataP->unk34 = 360.0f*frandf();
                    dataP->rotSpeed = 7.0f;
                    dataP->time = mbRandMod(20)+20;
                    dataP->accelY = 0.0f;
                    dataP->velDecay = 0.93f;
                }
                break;

            case 1:
                for(i=0; i<particleP->num; i++, dataP++) {
                    dataP->pos.x = dataP->pos.y = dataP->pos.z = 0.0f;
                    dataP->pos.y += 50.0f;
                    angle = 360.0f*frandf();
                    t = 0.7f+(0.3f*frandf());
                    elev = 90.0f*(t*fabsf2(t));
                    dataP->vel.x = HuSin(angle)*HuCos(elev);
                    dataP->vel.y = HuSin(elev);
                    dataP->vel.z = HuCos(angle)*HuCos(elev);
                    VECScale(&dataP->vel, &dataP->vel, (1.0f/60.0f)*(300.0f+(100.0f*(3.0f*frandf()))));
                    dataP->vel.y *= 1.5f;
                    dataP->scale = 0.7f*dataP->scaleBase;
                    dataP->scaleSpeed = 0.3f;
                    if(dataP->color.r < 150) {
                        dataP->color.r -= 30;
                    }
                    if(dataP->color.g < 150) {
                        dataP->color.g -= 30;
                    }
                    if(dataP->color.b < 150) {
                        dataP->color.b -= 30;
                    }
                    dataP->color.a = 255;
                    dataP->time = mbRandMod(20)+20;
                    dataP->accelY = -0.66666675f;
                    dataP->velDecay = 1.0f;
                }
                break;
        }
        particleP->initF = 1;
        particleP->endF = 0;
    }
    for(dataP=particleP->data, i=0; i<particleP->num; i++, dataP++) {
        if(dataP->time == 0) {
            continue;
        }
        VECScale(&dataP->vel, &dataP->vel, dataP->velDecay);
        VECAdd(&dataP->pos, &dataP->vel, &dataP->pos);
        dataP->vel.y += dataP->accelY;
        dataP->scale += dataP->scaleSpeed;
        dataP->zRot += dataP->rotSpeed;
        dataP->time--;
        if(dataP->time < 10) {
            dataP->color.a *= 0.8f;
            if(dataP->time == 0) {
                dataP->color.a = 0;
                dataP->scale = 0.0f;
            }
        }
    }
}

static float diceInDotEffScaleRatio[4] = { 1.0f, 1.0f, 0.7f, 0.4f };
static GXColor diceInDotEffColorTbl[8] = {
    { 220, 64, 64, 0 },
    { 64, 220, 64, 0 },
    { 220, 220, 64, 0 },
    { 64, 64, 220, 0 },
    { 220, 64, 220, 0 },
    { 64, 220, 220, 0 },
    { 220, 120, 64, 0 },
    { 64, 120, 220, 0 },
};

static void DiceInDotEffHook(HU3D_MODEL *modelP, DICEPARTICLE *particleP, Mtx mtx)
{
    int i;
    DICEPARTICLEDATA *dataP;
    int no;
    int colorNo;
    float angle;
    float elev;
    float t;
    if(particleP->initF == 0 || particleP->endF != 0) {
        dataP = particleP->data;
        switch(particleP->mode) {
            case 0:
                for(i=0; i<particleP->num; i++, dataP++) {
                    no = 3;
                    dataP->animNo = no;
                    dataP->pos.x = dataP->pos.y = dataP->pos.z = 0.0f;
                    angle = 360.0f*frandf();
                    t = (1.6f*frandf())-0.8f;
                    elev = 90.0f*(t*fabsf2(t));
                    dataP->vel.x = HuSin(angle)*HuCos(elev);
                    dataP->vel.y = HuSin(elev);
                    dataP->vel.z = HuCos(angle)*HuCos(elev);
                    VECScale(&dataP->vel, &dataP->vel, (1.0f/60.0f)*(500.0f+(100.0f*(9.0f*frandf()))));
                    dataP->scale = diceInDotEffScaleRatio[no]*(60.0f*(0.5f+(0.7f*frandf())));
                    dataP->scaleBase = dataP->scale;
                    colorNo = mbRandMod(8);
                    t = 0.2f+(0.4f*frandf());
                    dataP->color.r = (t*(255.0f-diceInDotEffColorTbl[colorNo].r))+diceInDotEffColorTbl[colorNo].r;
                    dataP->color.g = (t*(255.0f-diceInDotEffColorTbl[colorNo].g))+diceInDotEffColorTbl[colorNo].g;
                    dataP->color.b = (t*(255.0f-diceInDotEffColorTbl[colorNo].b))+diceInDotEffColorTbl[colorNo].b;
                    dataP->color.a = mbRandMod(50)+100;
                    dataP->time = mbRandMod(20)+20;
                }
                break;

            case 1:
                for(i=0; i<particleP->num; i++, dataP++) {
                    dataP->pos.x = dataP->pos.y = dataP->pos.z = 0.0f;
                    dataP->pos.y += 50.0f;
                    angle = 360.0f*frandf();
                    t = 0.7f+(0.3f*frandf());
                    elev = 90.0f*(t*fabsf2(t));
                    dataP->vel.x = HuSin(angle)*HuCos(elev);
                    dataP->vel.y = HuSin(elev);
                    dataP->vel.z = HuCos(angle)*HuCos(elev);
                    VECScale(&dataP->vel, &dataP->vel, (1.0f/60.0f)*(300.0f+(100.0f*(3.0f*frandf()))));
                    dataP->vel.y *= 1.5f;
                    dataP->scale = 0.8f*dataP->scaleBase;
                    dataP->color.a = mbRandMod(50)+120;
                    dataP->time = mbRandMod(10)+20;
                }
                break;
        }
        particleP->initF = 1;
        particleP->endF = 0;
        particleP->unk20 = 1;
    }
    switch(particleP->mode) {
        case 0:
            for(dataP=particleP->data, i=0; i<particleP->num; i++, dataP++) {
                if(dataP->time == 0) {
                    continue;
                }
                VECScale(&dataP->vel, &dataP->vel, 0.92f);
                VECAdd(&dataP->pos, &dataP->vel, &dataP->pos);
                dataP->vel.y += -0.22222224f;
                dataP->scale -= 0.15f;
            }
            break;

        case 1:
            for(dataP=particleP->data, i=0; i<particleP->num; i++, dataP++) {
                if(dataP->time == 0) {
                    continue;
                }
                VECAdd(&dataP->pos, &dataP->vel, &dataP->pos);
                dataP->vel.y += -0.41666672f;
            }
            break;
    }
    for(dataP=particleP->data, i=0; i<particleP->num; i++, dataP++) {
        if(dataP->time == 0) {
            continue;
        }
        dataP->time--;
        if(dataP->time < 6) {
            dataP->color.a *= 0.7f;
            if(dataP->time == 0) {
                dataP->color.a = 0;
                dataP->scale = 0.0f;
            }
        }
    }
}

static float diceInEffRotYRatio[3] = {
    (1.0f/1.0f), (1.0f/2.0f), (1.0f/3.0f)
};

static inline void *DiceEffMalloc(u32 size)
{
    void *ptr = HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
    return ptr;
}

static void DiceObjEffCreate(DICEWORK *work)
{
    void *dlBuf;
    int hitOrderTbl[20] = {
        0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2
    };
    float posY;
    DICEEFF *eff;
    int i;
    work->eff = eff = DiceEffMalloc((DICEEFF_PUFF_MAX+DICEEFF_TRI_MAX)*sizeof(DICEEFF));
    memset(eff, 0, (DICEEFF_PUFF_MAX+DICEEFF_TRI_MAX)*sizeof(DICEEFF));
    for(i=0; i<50; i++, eff++) {
        eff->rot.x = (15.0f*frandf())+30.0f;
        eff->rot.y = 360.0f*frandf();
        eff->radius = 100.0f*(1.0f+frandf());
    }
    for(i=0; i<100; i++) {
        int idx1, idx2;
        int temp;
        idx1 = mbRandMod(20);
        idx2 = mbRandMod(20);
        temp = hitOrderTbl[idx1];
        hitOrderTbl[idx1] = hitOrderTbl[idx2];
        hitOrderTbl[idx2] = temp;
    }
    posY = 0;
    for(i=0; i<20; i++, eff++) {
        eff->rot.x = 0;
        eff->rot.y = posY;
        eff->radius = 150;
        eff->scale.x = diceInEffRotYRatio[hitOrderTbl[i]];
        eff->scale.y = 1;
        eff->scale.z = 1+(2*frandf());
        posY += (360*eff->scale.x)/12;
    }
    dlBuf = DiceEffMalloc(4096);
    DCInvalidateRange(dlBuf, 4096);
    GXBeginDisplayList(dlBuf, 4096);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-20, 0, -20);
    GXTexCoord2f32(0, 0);
    GXPosition3f32(20, 0, -20);
    GXTexCoord2f32(1, 0);
    GXPosition3f32(20, 0, 20);
    GXTexCoord2f32(1, 1);
    GXPosition3f32(-20, 0, 20);
    GXTexCoord2f32(0, 1);
    GXEnd();
    work->dlSize = GXEndDisplayList();
    work->dlBuf = DiceEffMalloc(work->dlSize);
    memcpy(work->dlBuf, dlBuf, work->dlSize);
    HuMemDirectFree(dlBuf);
    DCFlushRangeNoSync(work->dlBuf, work->dlSize);
    work->animEffPuff = HuSprAnimRead(HuDataSelHeapReadNum(mbBoardDataNumGet(DATANUM(DATA_board, 0x5D)), HU_MEMNUM_OVL, HEAP_MODEL));
    work->animEffTri = HuSprAnimRead(HuDataSelHeapReadNum(mbBoardDataNumGet(DATANUM(DATA_board, 0x62)), HU_MEMNUM_OVL, HEAP_MODEL));
}

static void DiceObjEffKill(DICEWORK *work)
{
    if(work->dlBuf != NULL) {
        HuMemDirectFree(work->dlBuf);
        work->dlBuf = NULL;
    }
    if(work->eff != NULL) {
        HuMemDirectFree(work->eff);
        work->eff = NULL;
    }
    if(work->animEffPuff != NULL) {
        HuSprAnimKill(work->animEffPuff);
        work->animEffPuff = NULL;
    }
    if(work->animEffTri != NULL) {
        HuSprAnimKill(work->animEffTri);
        work->animEffTri = NULL;
    }
}

static void DiceObjEffPuffDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    int i;
    OMOBJ *obj = modelP->hookData;
    DICEWORK *work = obj->data;
    GXColor color = { 0, 0, 0, 255 };
    DICEEFF *eff;
    float time = work->puffTime/45.0f;
    if(time > 1) {
        time = 1;
    }
    HuSprTexLoad(work->animEffPuff, 0, GX_TEXMAP0, GX_REPEAT, GX_REPEAT, GX_NEAR);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ONE);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    color.a = (1-time)*255;
    GXSetTevColor(GX_TEVREG0, color);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetCullMode(GX_CULL_NONE);
    for(eff=work->eff, i=0; i<DICEEFF_PUFF_MAX; i++, eff++) {
        Mtx modelview;
        Mtx rot;
        Mtx trans;
        HuVecF pos;
        static HuVecF back = { 0, 0, -1 };
        mtxRot(rot, eff->rot.x, eff->rot.y, eff->rot.z);
        MTXMultVec(rot, &back, &pos);
        VECScale(&pos, &pos, HuSin(time*90)*eff->radius);
        MTXTrans(trans, obj->trans.x+pos.x, obj->trans.y+pos.y+50, obj->trans.z+pos.z);
        MTXConcat(trans, rot, modelview);
        MTXConcat(*mtx, modelview, modelview);
        GXLoadPosMtxImm(modelview, GX_PNMTX0);
        GXCallDisplayList(work->dlBuf, work->dlSize);
    }
    if(!mbPauseEnableCheck()) {
        if(++work->puffTime >= 45u) {
            modelP->attr |= HU3D_ATTR_DISPOFF;
        }
    }
}

static void DiceObjEffTriDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    int i;
    OMOBJ *obj = modelP->hookData;
    DICEWORK *work = obj->data;
    GXColor color = { 255, 192, 64, 255 };
    float time = work->triTime/16.0f;
    float alpha;
    DICEEFF *eff;
    if(time > 1) {
        time = 1;
    }
    HuSprTexLoad(work->animEffTri, 0, GX_TEXMAP0, GX_REPEAT, GX_REPEAT, GX_NEAR);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    alpha = 255*(4*(1-time));
    if(alpha > 255) {
        alpha = 255;
    }
    color.a = alpha;
    GXSetChanMatColor(GX_COLOR0A0, color);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetCullMode(GX_CULL_BACK);
    for(eff=work->eff+DICEEFF_PUFF_MAX, i=0; i<DICEEFF_TRI_MAX; i++, eff++) {
        Mtx modelview;
        Mtx trans;
        Mtx rot;
        Mtx scale;
        HuVecF pos;
        static HuVecF back = { 0, 0, -1 };
        MTXScale(scale, (2*eff->scale.x)+time, 2*eff->scale.y, 2*eff->scale.z);
        mtxRot(rot, eff->rot.x, eff->rot.y, eff->rot.z);
        MTXMultVec(rot, &back, &pos);
        VECScale(&pos, &pos, HuSin(time*90)*eff->radius);
        MTXTrans(trans, obj->trans.x+pos.x, obj->trans.y+pos.y-50, obj->trans.z+pos.z);
        MTXConcat(rot, scale, modelview);
        MTXConcat(trans, modelview, modelview);
        MTXConcat(*mtx, modelview, modelview);
        GXLoadPosMtxImm(modelview, GX_PNMTX0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-20, 0, -30);
        GXTexCoord2f32(0, 0);
        GXPosition3f32(20, 0, -30);
        GXTexCoord2f32(1, 0);
        GXPosition3f32(20, 0, 10);
        GXTexCoord2f32(1, 1);
        GXPosition3f32(-20, 0, 10);
        GXTexCoord2f32(0, 1);
        GXEnd();
    }
    if(!mbPauseEnableCheck()) {
        if(++work->triTime >= 16) {
            modelP->attr |= HU3D_ATTR_DISPOFF;
        }
    }
}

static float diceCloudRotYRatio2[3] = {
    (1.0f/1.0f), (1.0f/2.0f), (1.0f/3.0f)
};

static DICENUMVTX *DiceNumObjMdlCopy(int modelId)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    HSF_DATA *hsf = modelP->hsf;
    HSF_OBJECT *obj = hsf->object;
    HuVecF *pos;
    DICENUMVTX *vtx;
    int size;
    DICENUMVTX *vtxBuf;

    int i, j;
    float posMin, posMax;
    float sizeY;
    for(i=0; i<hsf->objectNum; i++, obj++) {
        if(obj->type == HSF_OBJ_MESH) {
            size = obj->mesh.vertex->count*sizeof(DICENUMVTX);
            pos = obj->mesh.vertex->data;
            posMin = posMax = pos->y;
            pos++;
            for(j=1; j<obj->mesh.vertex->count; j++, pos++) {
                if(pos->y > posMax) {
                    posMax = pos->y;
                }
                if(pos->y < posMin) {
                    posMin = pos->y;
                }
            }
            sizeY = posMax-posMin;
            vtxBuf = vtx = HuMemDirectMallocNum(HEAP_MODEL, size, modelP->mallocNo);
            pos = obj->mesh.vertex->data;
            for(j=0; j<obj->mesh.vertex->count; j++, pos++, vtx++) {
                vtx->pos = *pos;
                vtx->weight = (pos->y-posMin)/sizeY;
            }
            break;
        }
    }
    return vtxBuf;
}

static void DiceNumObjMdlBend(int modelId, DICENUMVTX *vtx, Mtx mtx1, Mtx mtx2)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    HSF_DATA *hsf = modelP->hsf;
    int i, j;
    HSF_OBJECT *obj = hsf->object;
    for(i=0; i<hsf->objectNum; i++, obj++) {
        if(obj->type == HSF_OBJ_MESH) {
            HuVecF *pos;
            pos = obj->mesh.vertex->data;
            for(j=0; j<obj->mesh.vertex->count; j++, vtx++, pos++) {
                HuVecF vtx1, vtx2;
                MTXMultVec(mtx1, &vtx->pos, &vtx1);
                MTXMultVec(mtx2, &vtx->pos, &vtx2);
                pos->x = vtx1.x+(vtx->weight*(vtx2.x-vtx1.x));
                pos->y = vtx1.y+(vtx->weight*(vtx2.y-vtx1.y));
                pos->z = vtx1.z+(vtx->weight*(vtx2.z-vtx1.z));
            }
            DCStoreRangeNoSync(obj->mesh.vertex->data, obj->mesh.vertex->count*sizeof(HuVecF));
            break;
        }
    }
}

static void DiceSNpcNumUpdate(OMOBJ *obj);

OMOBJ *mbDiceSNpcNumObjCreate(HuVecF *pos, HuVecF *ofs, int value, int flag, int color)
{
    OMOBJ *obj;
    DICESNPCNUMWORK *work;
    int i;
    HuVecF pos3D;
    HuVecF normPos;
    obj = omAddObjEx(mbObjMan, 32258, 20, 0, -1, DiceSNpcNumUpdate);
    omSetStatBit(obj, 0x100);
    work = omObjGetWork(obj, DICESNPCNUMWORK);
    work->dispF = TRUE;
    work->killF = FALSE;
    work->unkF = (u8)flag;
    work->value = (u8)value;
    for(i=0; i<20; i++) {
        int coinId = mbCoinObjCreate(i%10, color);
        mbCoinObjLayerSet(coinId, 4);
        mbCoinObjDispSet(coinId, FALSE);
        obj->mdlId[i] = coinId;
    }
    VECAdd(pos, ofs, &pos3D);
    mbPos3DtoNorm(&pos3D, 1, &normPos);
    obj->rot.z = normPos.z;
    mbVecCopy(pos, &obj->trans);
    mbVecCopy(ofs, &obj->scale);
    return obj;
}

static void DiceSNpcNumUpdate(OMOBJ *obj)
{
    DICESNPCNUMWORK *work = omObjGetWork(obj, DICESNPCNUMWORK);
    BOOL tensF = FALSE;
    HU3D_CAMERA *cameraP = Hu3DCamera;
    int i;
    int digit;
    float rotX;
    float tanFov;
    float scale;
    float tanFov2;
    HuVecF pos;
    HuVecF normPos;
    if(work->killF || mbExitCheck()) {
        for(i=0; i<20; i++) {
            if(obj->mdlId[i] != -1) {
                mbCoinObjNumDec(obj->mdlId[i]);
            }
        }
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    if(mbPauseEnableCheck()) {
        return;
    }
    VECAdd(&obj->trans, &obj->scale, &pos);
    mbPos3DtoNorm(&pos, 1, &normPos);
    normPos.z = obj->rot.z;
    tanFov = HuSin(cameraP[2].fov*0.5f)/HuCos(cameraP[2].fov*0.5f);
    scale = tanFov*-normPos.z;
    normPos.x = normPos.x*(1.2f*scale);
    normPos.y = normPos.y*scale;
    mbVecCopy(&normPos, &pos);
    mbCameraRotGet(&normPos);
    rotX = -normPos.x;
    for(i=0; i<20; i++) {
        mbCoinObjDispSet(obj->mdlId[i], FALSE);
    }
    if(work->dispF) {
        tanFov2 = HuSin(cameraP->fov*0.5f)/HuCos(cameraP->fov*0.5f);
        scale = tanFov/tanFov2;
        digit = work->value/10;
        if(digit != 0) {
            mbCoinObjDispSet(obj->mdlId[digit], TRUE);
            mbCoinObjPosSet(obj->mdlId[digit], pos.x-(60.000004f*scale), pos.y, pos.z);
            mbCoinObjRotSet(obj->mdlId[digit], rotX, 0.0f, 0.0f);
            mbCoinObjScaleSet(obj->mdlId[digit], scale, scale, scale);
            tensF = TRUE;
        }
        digit = (work->value%10)+10;
        if(digit != 0) {
            mbCoinObjDispSet(obj->mdlId[digit], TRUE);
            if(!tensF) {
                mbCoinObjPosSet(obj->mdlId[digit], pos.x, pos.y, pos.z);
            } else {
                mbCoinObjPosSet(obj->mdlId[digit], pos.x+(60.000004f*scale), pos.y, pos.z);
            }
            mbCoinObjRotSet(obj->mdlId[digit], rotX, 0.0f, 0.0f);
            mbCoinObjScaleSet(obj->mdlId[digit], scale, scale, scale);
        }
    }
}

void mbDiceSNpcNumKill(OMOBJ *obj)
{
    DICESNPCNUMWORK *work = omObjGetWork(obj, DICESNPCNUMWORK);
    work->killF = TRUE;
}

void mbDiceSNpcNumDispSet(OMOBJ *obj, int dispF)
{
    DICESNPCNUMWORK *work = omObjGetWork(obj, DICESNPCNUMWORK);
    work->dispF = (u8)dispF;
}

void mbDiceSNpcNumSet(OMOBJ *obj, int value)
{
    DICESNPCNUMWORK *work = omObjGetWork(obj, DICESNPCNUMWORK);
    work->value = (u8)value;
}

void mbDiceSNpcNumPosSet(OMOBJ *obj, HuVecF *pos)
{
    mbVecCopy(pos, &obj->trans);
}

void mbDiceSNpcNumOfsSet(OMOBJ *obj, HuVecF *ofs)
{
    mbVecCopy(ofs, &obj->scale);
}

void mbDiceSNpcNumOfsGet(OMOBJ *obj, HuVecF *ofs)
{
    mbVecCopy(&obj->scale, ofs);
}
