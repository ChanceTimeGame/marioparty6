#include "game/board/telop.h"
#include "game/board/audio.h"
#include "game/board/exit.h"
#include "game/board/math.h"
#include "game/board/pause.h"
#include "game/board/status.h"

#include "game/gamework.h"
#include "game/flag.h"
#include "game/esprite.h"
#include "game/sprite.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/data.h"
#include "game/audio.h"
#include "game/wipe.h"
#include "math.h"

int abs(int x);

/* Retail sees HuARDirCheck returning a pointer (li 0; cmplw); armem.h declares AMEM_PTR (u32). */
void *HuARDirCheck(unsigned int dir);
void HuARDirFree(unsigned int dir);
u32 HuAR_DVDtoARAM(unsigned int dir);
s32 HuARDMACheck(void);

typedef struct TelopWork_s {
    u8 killF : 1;
    u8 mode : 3;
    s8 playerNo;
    s8 telopNo;
    u8 unk3;
    s8 delay;
    s16 time;
    s16 maxTime;
} TELOPWORK;

typedef struct TelopLastTurnWork_s {
    u8 killF : 1;
    u8 mode : 3;
    u8 lastF : 1;
    u8 delay;
    s16 time;
    HUSPR_GROUPID grpId;
    HUSPRID sprId[3];
} TELOPLASTTURNWORK;

typedef struct TelopTimeWork_s {
    u8 killF : 1;
    u8 mode : 3;
    s16 time;
    s16 maxTime;
} TELOPTIMEWORK;

typedef struct TauntWork_s {
    u8 killF : 1;
} TAUNTWORK;

typedef struct TelopTimeChangeWork_s {
    u8 killF : 1;
    u8 wipeF : 1;
    u8 mode : 4;
    s16 time;
    s16 maxTime;
} TELOPTIMECHANGEWORK;

static const unsigned int telopFileTbl[27] = {
    0x5004D, 0x5004E, 0x5004F, 0x50050, 0x50051, 0x50052, 0x50053, 0x50054,
    0x50055, 0x50056, 0x50058, 0x50057, 0x50057, 0x50057, 0x5005A, 0x50059,
    0x50044, 0x50045, 0x50046, 0x50047, 0x50048, 0x50049, 0x5004C, 0x5004B,
    0x5004A, 0x50044, 0x50044
};

static const int tauntSeTbl[14] = {
    0x24D, 0x235, 0x2C5, 0x325, 0x2F5, 0x1ED, 0x30D,
    0x205, 0x2DD, 0x295, 0x21D, 0x295, 0x295, 0x295
};

static unsigned int telopTurnFileTbl[3] = { 0x50073, 0x50074, 0x50073 };
static unsigned int telopTurnLastFileTbl[3] = { 0x50072, 0x50074, 0x50072 };

static HuVec2f telopTurnLastSprOfsTbl[2][3] = {
    { { -24, 0 }, { 0, 0 }, { 24, 0 } },
    { { 0, 0 }, { 0, 0 }, { 0, 0 } }
};

static HuVec2f telopTurnSprOfsTbl[6][3] = {
    { { -24, 0 }, { 0, 0 }, { 24, 0 } },
    { { 32, 0 }, { -104, 0 }, { 32, 0 } },
    { { -24, 0 }, { 0, 0 }, { 24, 0 } },
    { { -24, 0 }, { 0, 0 }, { 24, 0 } },
    { { -24, 0 }, { 0, 0 }, { 24, 0 } },
    { { -24, 0 }, { 0, 0 }, { 24, 0 } }
};

static s16 telopTurnSprPrioTbl[3] = { 1400, 1000, 1400 };
static s8 telopTurnSprBankTbl[3] = { 0, 0, 1 };

static HuVec2f telopTimeSprOfsTbl[8] = {
    { 0, 0 }, { 0, 0 }, { -88, 8 }, { 0, 80 },
    { 88, 8 }, { -88, 8 }, { 0, 80 }, { 88, 8 }
};

static HuVec2f telopTimeNewSprOfsTbl[6] = {
    { 0, 0 }, { 0, -8 }, { 0, -8 }, { 0, -8 }, { 0, -8 }, { 0, -8 }
};

static HuVec2f telopTimeStarSprOfsTbl[6][3] = {
    { { -88, 8 }, { 0, 80 }, { 88, 8 } },
    { { -80, 40 }, { 0, 80 }, { 80, 40 } },
    { { -80, 40 }, { 0, 80 }, { 80, 40 } },
    { { -80, 40 }, { 0, 80 }, { 80, 40 } },
    { { -80, 40 }, { 0, 80 }, { 80, 40 } },
    { { -80, 40 }, { 0, 80 }, { 80, 40 } }
};

static float telopTimeSprScaleTbl[8] = { 1, 0.75f, 1, 1, 1 };
static float telopTimeBaseTPLvlTbl[8] = { 1, 1, 1, 1, 1 };

static unsigned int telopTimeBackFileTbl[2] = { 0x5006D, 0x5006E };
static unsigned int telopTimeFileTbl[2] = { 0x5006F, 0x50070 };

static inline float TelopFabs(register float x)
{
    asm {
        fabs x, x
    }
    return x;
}

static OMOBJ *telopTimeChangeOMObj;
static OMOBJ *tauntOMObj;
static OMOBJ *telopTimeOMObj;
static OMOBJ *telopLastTurnOMObj;
static OMOBJ *telopOMObj;

static void TelopInitOMExec(OMOBJ *obj);
static void TelopOMExec(OMOBJ *obj);

void mbTelopCreate(int playerNo, int telopNo, BOOL waitF)
{
    TELOPWORK *work;
    telopOMObj = omAddObjEx(mbObjMan, 0x106, 1, 0, -1, TelopInitOMExec);
    omSetStatBit(telopOMObj, 0x100);
    work = omObjGetWork(telopOMObj, TELOPWORK);
    work->killF = FALSE;
    work->playerNo = playerNo;
    work->mode = 0;
    work->telopNo = telopNo;
    if((playerNo >= 0 && GwPlayer[playerNo].comF) || playerNo < 0) {
        work->delay = 30;
    } else {
        work->delay = 0;
    }
    telopOMObj->mdlId[0] = espEntry(mbBoardDataNumGet(telopFileTbl[telopNo]), 100, 0);
    espDrawNoSet(telopOMObj->mdlId[0], 32);
    if(telopNo < 16) {
        mbAudFXPlay(0x3F3);
    }
    if(waitF) {
        while(telopOMObj) {
            HuPrcVSleep();
        }
    }
}

void mbTelopPlayerCreate(int playerNo)
{
    mbTelopCreate(playerNo, GwPlayer[playerNo].charNo, TRUE);
}

void mbTelopPlayerSkipCreate(int telopNo)
{
    mbTelopCreate(-1, telopNo + 16, FALSE);
}

static void TelopInitOMExec(OMOBJ *obj)
{
    TELOPWORK *work = omObjGetWork(obj, TELOPWORK);
    work->mode = 0;
    work->time = 0;
    work->maxTime = (work->telopNo >= 16) ? 60 : 15;
    espPosSet(obj->mdlId[0], 288, 240);
    espTPLvlSet(obj->mdlId[0], 0);
    espScaleSet(obj->mdlId[0], 0, 0);
    espDispOn(obj->mdlId[0]);
    obj->objFunc = TelopOMExec;
}

static void TelopOMExec(OMOBJ *obj)
{
    TELOPWORK *work = omObjGetWork(obj, TELOPWORK);
    float t;
    int padNo;
    if(work->killF || mbExitCheck()) {
        espKill(obj->mdlId[0]);
        telopOMObj = NULL;
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    switch(work->mode) {
        case 0:
            if(++work->time >= work->maxTime) {
                work->mode = 1;
            }
            t = (float)work->time / work->maxTime;
            espTPLvlSet(obj->mdlId[0], t);
            espScaleSet(obj->mdlId[0], t, t);
            break;

        case 1:
            if(work->delay != 0) {
                work->delay--;
                break;
            }
            if(work->playerNo < 0) {
                work->mode = 2;
                work->time = 0;
                work->maxTime = (work->telopNo >= 16) ? 60 : 30;
                if(work->telopNo < 16) {
                    mbAudFXPlay(0x3F4);
                }
            } else {
                padNo = GwPlayer[work->playerNo].padNo;
                if((HuPadBtnDown[padNo] & PAD_BUTTON_A) || GwPlayer[work->playerNo].comF) {
                    work->mode = 2;
                    work->time = 0;
                    work->maxTime = (work->telopNo >= 16) ? 30 : 15;
                    if(work->telopNo < 16) {
                        mbAudFXPlay(0x3F4);
                    }
                }
            }
            break;

        case 2:
            if(++work->time >= work->maxTime) {
                work->killF = TRUE;
            }
            t = (float)work->time / work->maxTime;
            espTPLvlSet(obj->mdlId[0], 1 - t);
            espScaleSet(obj->mdlId[0], 1 + t, 1 + t);
            break;
    }
}

BOOL mbTelopCheck(void)
{
    return telopOMObj == NULL;
}

static void TelopLastTurnOMExec(OMOBJ *obj);
static void TelopLastTurnPauseHook(BOOL dispF);

void mbTelopLastTurnCreate(void)
{
    TELOPLASTTURNWORK *work;
    OMOBJ *obj;
    int i;
    BOOL lastF;
    int turnLeft;
    int lang;
    turnLeft = GwSystem.turnMax - GwSystem.turnNo;
    obj = omAddObjEx(mbObjMan, 0, 0, 0, -1, TelopLastTurnOMExec);
    telopLastTurnOMObj = obj;
    work = omObjGetWork(obj, TELOPLASTTURNWORK);
    work->killF = FALSE;
    work->delay = 0;
    work->time = 0;
    work->grpId = HuSprGrpCreate(3);
    if(turnLeft == 0) {
        work->lastF = TRUE;
        lastF = TRUE;
    } else {
        work->lastF = FALSE;
        lastF = FALSE;
    }
    lang = mbLanguageGet();
    for(i = 0; i < 3; i++) {
        if(lastF) {
            mbSprCreate(mbBoardDataNumGet(telopTurnLastFileTbl[i]), telopTurnSprPrioTbl[i], NULL, &work->sprId[i]);
        } else {
            mbSprCreate(mbBoardDataNumGet(telopTurnFileTbl[i]), telopTurnSprPrioTbl[i], NULL, &work->sprId[i]);
        }
        HuSprGrpMemberSet(work->grpId, i, work->sprId[i]);
        HuSprAttrSet(work->grpId, i, HUSPR_ATTR_LINEAR);
        HuSprBankSet(work->grpId, i, telopTurnSprBankTbl[i]);
        if(lastF) {
            HuSprPosSet(work->grpId, i, telopTurnLastSprOfsTbl[lastF][i].x, telopTurnLastSprOfsTbl[lastF][i].y);
        } else {
            HuSprPosSet(work->grpId, i, telopTurnSprOfsTbl[lang][i].x, telopTurnSprOfsTbl[lang][i].y);
        }
    }
    if(!work->lastF) {
        HuSprBankSet(work->grpId, 1, turnLeft - 1);
    } else {
        HuSprDispOff(work->grpId, 1);
    }
    obj->trans.x = 0;
    HuSprGrpTPLvlSet(work->grpId, obj->trans.x);
    HuSprGrpPosSet(work->grpId, 288, 96);
    mbAudFXPlay(0x46C);
    mbPauseHookPush(TelopLastTurnPauseHook);
}

static void TelopLastTurnOMExec(OMOBJ *obj)
{
    TELOPLASTTURNWORK *work = omObjGetWork(obj, TELOPLASTTURNWORK);
    float t;
    if(work->killF || mbExitCheck()) {
        HuSprGrpKill(work->grpId);
        mbPauseHookPop(TelopLastTurnPauseHook);
        telopLastTurnOMObj = NULL;
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    if(work->delay != 0) {
        work->delay--;
        return;
    }
    switch(work->mode) {
        case 0:
            obj->trans.x += 0.033333335f;
            if(obj->trans.x > 1) {
                obj->trans.x = 1;
                work->mode = 1;
            }
            HuSprGrpTPLvlSet(work->grpId, obj->trans.x);
            break;

        case 1:
            t = 0.0125f * work->time;
            obj->trans.y = 1 + 0.5f * TelopFabs(mbSinDeg(720 * t));
            if(work->lastF) {
                HuSprGrpScaleSet(work->grpId, obj->trans.y, obj->trans.y);
            } else {
                HuSprScaleSet(work->grpId, 1, obj->trans.y, obj->trans.y);
            }
            if(t >= 1) {
                work->mode = 2;
                work->delay = 90;
            } else {
                work->time++;
            }
            break;

        case 2:
            obj->trans.x -= 0.033333335f;
            if(obj->trans.x < 0) {
                obj->trans.x = 0;
                work->killF = TRUE;
            }
            HuSprGrpTPLvlSet(work->grpId, obj->trans.x);
            break;
    }
}

static void TelopLastTurnPauseHook(BOOL dispF)
{
    TELOPLASTTURNWORK *work = omObjGetWork(telopLastTurnOMObj, TELOPLASTTURNWORK);
    int i;
    for(i = 0; i < 3; i++) {
        if(dispF) {
            HuSprDispOn(work->grpId, i);
        } else {
            HuSprDispOff(work->grpId, i);
        }
        if(work->lastF) {
            HuSprDispOff(work->grpId, 1);
        }
    }
}

static void TelopTimeOMExec(OMOBJ *obj);
static void TelopTimePauseHook(BOOL dispF);

void mbTelopTimeCreate(void)
{
    OMOBJ *obj;
    TELOPTIMEWORK *work;
    obj = omAddObjEx(mbObjMan, 0, 1, 0, -1, TelopTimeOMExec);
    telopTimeOMObj = obj;
    omSetStatBit(obj, 0x100);
    work = omObjGetWork(obj, TELOPTIMEWORK);
    work->killF = FALSE;
    work->time = work->maxTime = 0;
    work->mode = 0;
    work->maxTime = 120;
    obj->mdlId[0] = mbTelopTimeSprCreate();
    HuSprGrpPosSet(obj->mdlId[0], 288, 224);
    mbTelopTimeTPLvlSet(obj->mdlId[0], 1);
    mbPauseHookPush(TelopTimePauseHook);
}

static void TelopTimeOMExec(OMOBJ *obj)
{
    TELOPTIMEWORK *work = omObjGetWork(obj, TELOPTIMEWORK);
    float t;
    float rot;
    if(work->killF || mbExitCheck()) {
        mbTelopTimeSprKill(obj->mdlId[0]);
        obj->mdlId[0] = HUSPR_GROUP_NONE;
        telopTimeOMObj = NULL;
        omDelObjEx(HuPrcCurrentGet(), obj);
        mbPauseHookPop(TelopTimePauseHook);
        return;
    }
    switch(work->mode) {
        case 0:
            work->time++;
            t = (float)work->time / work->maxTime;
            rot = 2 * t;
            mbTelopTimeSprRotSet(obj->mdlId[0], rot);
            if(work->time >= work->maxTime) {
                work->time = 0;
                work->maxTime = 30;
                work->mode++;
            }
            break;

        case 1:
            work->time++;
            t = (float)work->time / work->maxTime;
            mbTelopTimeTPLvlSet(obj->mdlId[0], 1 - t);
            if(work->time >= work->maxTime) {
                work->mode++;
                work->killF = TRUE;
            }
            break;
    }
}

static void TelopTimePauseHook(BOOL dispF)
{
    if(telopTimeOMObj) {
        mbTelopTimeDispSet(telopTimeOMObj->mdlId[0], dispF);
    }
}

static inline int TimeTurnMaxGet(void)
{
    return GwSystem.timeTurnMax;
}

static inline int TimeTurnGet(void)
{
    return GwSystem.timeTurn;
}

HUSPR_GROUPID mbTelopTimeSprCreate(void)
{
    HUSPR_GROUPID grpId;
    int timeTurnMax;
    int timeLeft;
    int turnLeft;
    int lang;
    int i;
    HUSPRID sprId;

    timeTurnMax = TimeTurnMaxGet();
    timeLeft = timeTurnMax - TimeTurnGet();
    turnLeft = (GwSystem.turnMax - GwSystem.turnNo) + 1;
    if(turnLeft < timeLeft) {
        timeLeft = turnLeft;
    }
    lang = mbLanguageGet();
    grpId = HuSprGrpCreate(8);
    i = 0;
    if(GwSystem.curTime) {
        i++;
    }
    mbSprCreate(mbBoardDataNumGet(telopTimeBackFileTbl[i]), 100, NULL, &sprId);
    HuSprGrpMemberSet(grpId, 0, sprId);
    HuSprTPLvlSet(grpId, 0, 0.6f);
    mbSprCreate(mbBoardDataNumGet(telopTimeFileTbl[i]), 99, NULL, &sprId);
    HuSprGrpMemberSet(grpId, 1, sprId);
    for(i = 0; i < 3; i++) {
        mbSprCreate(mbBoardDataNumGet(0x50071), 98, NULL, &sprId);
        HuSprGrpMemberSet(grpId, i + 2, sprId);
        mbSprCreate(mbBoardDataNumGet(0x50071), 97, NULL, &sprId);
        HuSprGrpMemberSet(grpId, i + 5, sprId);
        HuSprAttrSet(grpId, i + 5, HUSPR_ATTR_LINEAR | HUSPR_ATTR_ADDCOL);
    }
    for(i = 0; i < 8; i++) {
        HuSprAttrSet(grpId, i, HUSPR_ATTR_LINEAR);
        HuSprPosSet(grpId, i, telopTimeSprOfsTbl[i].x, telopTimeSprOfsTbl[i].y);
        HuSprScaleSet(grpId, i, telopTimeSprScaleTbl[i], telopTimeSprScaleTbl[i]);
    }
    HuSprPosSet(grpId, 1, telopTimeNewSprOfsTbl[lang].x, telopTimeNewSprOfsTbl[lang].y);
    for(i = 0; i < 3; i++) {
        HuSprPosSet(grpId, i + 2, telopTimeStarSprOfsTbl[lang][i].x, telopTimeStarSprOfsTbl[lang][i].y);
        HuSprPosSet(grpId, i + 5, telopTimeStarSprOfsTbl[lang][i].x, telopTimeStarSprOfsTbl[lang][i].y);
    }
    mbTelopTimeStarSet(grpId, timeLeft);
    mbTelopTimeTPLvlSet(grpId, 1);
    return grpId;
}

void mbTelopTimeSprKill(HUSPR_GROUPID grpId)
{
    HuSprGrpKill(grpId);
}

void mbTelopTimeSprRotSet(HUSPR_GROUPID grpId, float t)
{
    HUSPRITE *sprP;
    HUSPR_GROUP *grpP;
    int i;
    BOOL offF;
    float value;
    float scale;

    grpP = &HuSprGrpData[grpId];
    for(i = 0; i < 3; i++) {
        offF = FALSE;
        sprP = &HuSprData[grpP->sprId[i + 2]];
        if(sprP->bank & 1) {
            offF = TRUE;
        }
        if(offF) {
            HuSprTPLvlSet(grpId, i + 2, 0.8f);
            HuSprScaleSet(grpId, i + 2, 1, 1);
            HuSprZRotSet(grpId, i + 2, 0);
            HuSprTPLvlSet(grpId, i + 5, 0);
            HuSprScaleSet(grpId, i + 5, 0, 0);
        } else {
            value = 1 + 0.2f * TelopFabs(mbSinDeg(360 * t));
            HuSprTPLvlSet(grpId, i + 2, 1);
            HuSprScaleSet(grpId, i + 2, value, value);
            value = 30 * mbSinDeg(360 * t);
            HuSprZRotSet(grpId, i + 2, value);
            value = fmodf(2 * t, 1.0f);
            scale = 1 + TelopFabs(mbSinDeg(90 * value));
            HuSprTPLvlSet(grpId, i + 5, 0.8f * (1 - value));
            HuSprScaleSet(grpId, i + 5, scale, scale);
            if(value < 0.001) {
                HuSprScaleSet(grpId, i + 5, 0, 0);
            }
        }
    }
}

void mbTelopTimeStarSet(HUSPR_GROUPID grpId, int num)
{
    int i;
    int bank;
    int offNum;
    offNum = 3 - num;
    for(i = 0; i < 3; i++) {
        bank = 0;
        if(GwSystem.curTime) {
            bank += 2;
        }
        if(i < offNum) {
            bank++;
        }
        HuSprBankSet(grpId, i + 2, bank);
        HuSprBankSet(grpId, i + 5, bank);
        HuSprTPLvlSet(grpId, i + 5, 0);
    }
}

void mbTelopTimeTPLvlSet(HUSPR_GROUPID grpId, float tpLvl)
{
    int i;
    for(i = 0; i < 8; i++) {
        HuSprTPLvlSet(grpId, i, tpLvl * telopTimeBaseTPLvlTbl[i]);
    }
}

void mbTelopTimeDispSet(HUSPR_GROUPID grpId, BOOL dispF)
{
    int i;
    for(i = 0; i < 8; i++) {
        if(dispF) {
            HuSprDispOn(grpId, i);
        } else {
            HuSprDispOff(grpId, i);
        }
    }
}

s8 mbPadStkXGet(int padNo)
{
    s8 stkX = HuPadStkX[padNo];
    s8 subStkX = HuPadSubStkX[padNo];
    if(abs(stkX) > abs(subStkX)) {
        if(abs(stkX) < 8) {
            return 0;
        }
        return stkX;
    } else {
        if(abs(subStkX) < 8) {
            return 0;
        }
        return subStkX;
    }
}

s8 mbPadStkYGet(int padNo)
{
    s8 stkY = HuPadStkY[padNo];
    s8 subStkY = HuPadSubStkY[padNo];
    if(abs(stkY) > abs(subStkY)) {
        if(abs(stkY) < 8) {
            return 0;
        }
        return stkY;
    } else {
        if(abs(subStkY) < 8) {
            return 0;
        }
        return subStkY;
    }
}

static int tauntSeNo[4] = { -1, -1, -1, -1 };

static void TauntOMExec(OMOBJ *obj);

void mbTauntInit(void)
{
    int i;
    tauntOMObj = omAddObjEx(mbObjMan, 0x7E00, 0, 0, -1, TauntOMExec);
    for(i = 0; i < 4; i++) {
        tauntSeNo[i] = -1;
    }
    _SetFlag(FLAGNUM(FLAG_GROUP_COMMON, 16));
}

void mbTauntClose(void)
{
    TAUNTWORK *work;
    if(tauntOMObj) {
        work = omObjGetWork(tauntOMObj, TAUNTWORK);
        work->killF = TRUE;
        _SetFlag(FLAGNUM(FLAG_GROUP_COMMON, 16));
    }
}

static void TauntOMExec(OMOBJ *obj)
{
    TAUNTWORK *work = omObjGetWork(obj, TAUNTWORK);
    int i;
    int padNo;
    int charNo;
    BOOL tauntF;
    if(work->killF || mbExitCheck()) {
        for(i = 0; i < 4; i++) {
            if(tauntSeNo[i] >= 0) {
                mbAudFXStop(tauntSeNo[i]);
                tauntSeNo[i] = -1;
            }
        }
        tauntOMObj = NULL;
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    for(i = 0; i < 4; i++) {
        if(tauntSeNo[i] >= 0 && HuAudFXStatusGet(tauntSeNo[i]) == 0) {
            tauntSeNo[i] = -1;
        }
    }
    if(mbPauseProcCheck() || _CheckFlag(FLAGNUM(FLAG_GROUP_COMMON, 16)) || WipeCheck() || GwSystem.turnPlayerNo == -1 || GWPartyGet() == FALSE || _CheckFlag(FLAGNUM(FLAG_GROUP_COMMON, 14))) {
        return;
    }
    for(i = 0; i < 4; i++) {
        if(i == GwSystem.turnPlayerNo || GwPlayer[i].comF) {
            continue;
        }
        padNo = GwPlayer[i].padNo;
        charNo = GwPlayer[i].charNo;
        tauntF = (charNo == 10) ? TRUE : GWBankFlagGet(charNo + 36);
        if(tauntF && tauntSeNo[padNo] < 0 && (HuPadBtnDown[padNo] & PAD_TRIGGER_L)) {
            tauntSeNo[padNo] = mbAudFXPlay(tauntSeTbl[charNo]);
        }
    }
}

int mbLanguageGet(void)
{
    static int languageTbl[6][2] = {
        { 0, 0 }, { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 }
    };
    int lang = GWLanguageGet();
    int i;
    for(i = 0; i < 6; i++) {
        if(lang == languageTbl[i][0]) {
            break;
        }
    }
    return languageTbl[i][1];
}

void mbLanguageSet(int lang)
{
    static int languageTbl[6][2] = {
        { 0, 0 }, { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 }
    };
    int i;
    for(i = 0; i < 6; i++) {
        if(lang == languageTbl[i][0]) {
            break;
        }
    }
    GWLanguageSet(languageTbl[i][1]);
}

static unsigned int boardDataDirTbl[6] = { 0x50000, 0x60000, 0x50000, 0x50000, 0x50000, 0x50000 };

static inline unsigned int BoardDataDirGet(int lang)
{
    if(lang < 0) {
        lang = 0;
    }
    return boardDataDirTbl[lang];
}

int mbBoardDataNumGet(int dataNum)
{
    int lang = mbLanguageGet();
    if((dataNum & 0xFFFF0000) != 0x50000) {
        return mbPauseDataNumGet(dataNum);
    }
    if(lang < 0) {
        lang = 0;
    }
    return (dataNum & 0xFFFF) | BoardDataDirGet(lang);
}

void mbBoardDataDirRead(void)
{
    int lang = mbLanguageGet();
    int i;
    if(lang < 0) {
        lang = 0;
    }
    for(i = 0; i < 6; i++) {
        if(BoardDataDirGet(i) == BoardDataDirGet(lang)) {
            continue;
        }
        if(HuARDirCheck(BoardDataDirGet(i)) != NULL) {
            HuARDirFree(BoardDataDirGet(i));
        }
        if(HuDataReadChk(BoardDataDirGet(i)) >= 0) {
            HuDataDirClose(BoardDataDirGet(i));
        }
    }
    if(HuARDirCheck(BoardDataDirGet(lang)) == NULL) {
        HuAR_DVDtoARAM(BoardDataDirGet(lang));
        while(HuARDMACheck());
    }
}

static HuVec2f telopTimeChangeSprOfsTbl[8] = {
    { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 16 },
    { 0, 16 }, { 0, 0 }, { 0, 0 }, { 0, 0 }
};

static unsigned int telopTimeChangeBackFileTbl[2] = { 0x5006D, 0x5006E };
static unsigned int telopTimeChangeFileTbl[2] = { 0x5006F, 0x50070 };

static void TelopTimeChangeOMExec(OMOBJ *obj);

void mbTelopTimeChangeCreate(void)
{
    OMOBJ *obj;
    TELOPTIMECHANGEWORK *work;
    int lang;
    int timeTurnMax;
    int timeLeft;
    int i;
    int j;

    obj = omAddObjEx(mbObjMan, 0, 8, 0, -1, TelopTimeChangeOMExec);
    telopTimeChangeOMObj = obj;
    omSetStatBit(obj, 0x100);
    work = omObjGetWork(obj, TELOPTIMECHANGEWORK);
    work->killF = FALSE;
    work->mode = 0;
    work->time = 0;
    work->wipeF = FALSE;
    work->maxTime = 16;
    lang = mbLanguageGet();
    timeTurnMax = TimeTurnMaxGet();
    timeLeft = timeTurnMax - TimeTurnGet();
    i = (GwSystem.turnMax - GwSystem.turnNo) + 1;
    if(i < timeLeft) {
        timeLeft = i;
    }
    obj->trans.x = 0;
    obj->mdlId[0] = espEntry(mbBoardDataNumGet(telopTimeChangeBackFileTbl[GwSystem.nextTime]), 101, 0);
    obj->mdlId[1] = espEntry(mbBoardDataNumGet(telopTimeChangeBackFileTbl[GwSystem.curTime]), 99, 0);
    obj->mdlId[2] = espEntry(mbBoardDataNumGet(telopTimeChangeBackFileTbl[GwSystem.curTime]), 97, 0);
    obj->mdlId[3] = espEntry(mbBoardDataNumGet(telopTimeChangeFileTbl[GwSystem.nextTime]), 100, 0);
    obj->mdlId[4] = espEntry(mbBoardDataNumGet(telopTimeChangeFileTbl[GwSystem.curTime]), 96, 0);
    for(i = 0; i < 5; i++) {
        espAttrSet(obj->mdlId[i], HUSPR_ATTR_LINEAR);
        espPosSet(obj->mdlId[i], 288 + telopTimeChangeSprOfsTbl[i].x, 240 + telopTimeChangeSprOfsTbl[i].y);
        espTPLvlSet(obj->mdlId[i], 0);
    }
    espPosSet(obj->mdlId[3], 288 + telopTimeNewSprOfsTbl[lang].x, 240 + telopTimeNewSprOfsTbl[lang].y);
    espPosSet(obj->mdlId[4], 288 + telopTimeNewSprOfsTbl[lang].x, 240 + telopTimeNewSprOfsTbl[lang].y);
    timeLeft = 3 - timeLeft;
    for(j = 0; j < 3; j++) {
        i = 0;
        if(GwSystem.curTime) {
            i += 2;
        }
        if(j < timeLeft) {
            i++;
        }
        obj->mdlId[j + 5] = espEntry(mbBoardDataNumGet(0x50071), 98, i);
        espAttrSet(obj->mdlId[j + 5], HUSPR_ATTR_LINEAR);
        espDispOff(obj->mdlId[j + 5]);
        espPosSet(obj->mdlId[j + 5], 288 + telopTimeStarSprOfsTbl[lang][j].x, 240 + telopTimeStarSprOfsTbl[lang][j].y);
        espScaleSet(obj->mdlId[j + 5], 0.75f, 0.75f);
    }
    mbAudFXPlay(0x46C);
}

static void TelopTimeChangeOMExec(OMOBJ *obj)
{
    TELOPTIMECHANGEWORK *work = omObjGetWork(obj, TELOPTIMECHANGEWORK);
    int i;
    int j;
    int lang;
    float t;
    float angle;
    float scale;
    float posY;
    if(work->killF || mbExitCheck()) {
        for(j = 0; j < 8; j++) {
            espKill(obj->mdlId[j]);
            obj->mdlId[j] = 0;
        }
        telopTimeChangeOMObj = NULL;
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    lang = mbLanguageGet();
    work->time++;
    if(work->time > work->maxTime) {
        work->time = work->maxTime;
    }
    t = (float)work->time / work->maxTime;
    switch(work->mode) {
        case 0:
            espTPLvlSet(obj->mdlId[0], t);
            espTPLvlSet(obj->mdlId[3], t);
            obj->trans.x = 288;
            obj->trans.y = 256;
            espPosSet(obj->mdlId[3], obj->trans.x, obj->trans.y);
            espPosSet(obj->mdlId[4], obj->trans.x, obj->trans.y);
            if(work->time >= work->maxTime) {
                obj->trans.z = 2;
                work->time = 0;
                work->maxTime = 40;
                work->mode++;
            }
            break;

        case 1:
            angle = 90 * t;
            scale = 1 - 0.75f * t;
            espTPLvlSet(obj->mdlId[0], 1 - t);
            espTPLvlSet(obj->mdlId[3], 1 - t);
            posY = 720 - 480 * mbSinDeg(90 - angle);
            espPosSet(obj->mdlId[0], 288 + 480 * mbCosDeg(90 - angle), posY);
            espZRotSet(obj->mdlId[0], 4 * angle);
            espScaleSet(obj->mdlId[0], scale, scale);
            scale = 1 - 0.75f * (1 - t);
            espTPLvlSet(obj->mdlId[1], t);
            posY = 720 - 480 * mbSinDeg(180 - angle);
            espPosSet(obj->mdlId[1], 288 + 480 * mbCosDeg(180 - angle), posY);
            espZRotSet(obj->mdlId[1], 4 * (angle - 90));
            espScaleSet(obj->mdlId[1], scale, scale);
            scale = 1 - 0.75f * t;
            espZRotSet(obj->mdlId[3], -angle);
            obj->trans.z += 0.5f;
            obj->trans.y += obj->trans.z;
            espPosSet(obj->mdlId[3], obj->trans.x, obj->trans.y);
            espScaleSet(obj->mdlId[3], scale, scale);
            if(work->time + 12 > work->maxTime) {
                angle = work->maxTime - work->time;
                angle *= 0.083333336f;
                scale = 1 + 2 * angle;
                espScaleSet(obj->mdlId[4], scale, scale);
                espTPLvlSet(obj->mdlId[4], 1 - angle);
            }
            if(work->time >= work->maxTime) {
                espAttrSet(obj->mdlId[2], HUSPR_ATTR_ADDCOL);
                work->time = 0;
                work->maxTime = 30;
                work->mode++;
            }
            break;

        case 2:
            scale = 1 + 0.5f * t;
            espTPLvlSet(obj->mdlId[2], 1 - t);
            espScaleSet(obj->mdlId[2], scale, scale);
            for(i = 0; i < 3; i++) {
                espDispOn(obj->mdlId[i + 5]);
            }
            if(work->time < work->maxTime) {
                break;
            }
            work->time = 0;
            work->maxTime = 30;
            work->mode++;

        case 3:
            if(work->time >= work->maxTime) {
                work->mode = 4;
            }
            break;

        case 4:
            work->wipeF = TRUE;
            if(WipeCheck()) {
                work->time = 0;
                work->maxTime = 30;
                work->mode = 5;
            }
            break;

        case 5:
            if(!WipeCheck()) {
                obj->trans.x = 504;
                obj->trans.y = 84;
                espPosSet(obj->mdlId[1], obj->trans.x, obj->trans.y);
                espPosSet(obj->mdlId[4], obj->trans.x + 0.5f * telopTimeNewSprOfsTbl[lang].x, obj->trans.y + 0.5f * telopTimeNewSprOfsTbl[lang].y);
                espScaleSet(obj->mdlId[1], 0.5f, 0.5f);
                espScaleSet(obj->mdlId[4], 0.5f, 0.5f);
                espTPLvlSet(obj->mdlId[1], 0.65f);
                for(i = 0; i < 3; i++) {
                    espPosSet(obj->mdlId[i + 5], obj->trans.x + 0.5f * telopTimeStarSprOfsTbl[lang][i].x, obj->trans.y + 0.5f * telopTimeStarSprOfsTbl[lang][i].y);
                    espScaleSet(obj->mdlId[i + 5], 0.375f, 0.375f);
                    espTPLvlSet(obj->mdlId[i + 5], 0.85f);
                }
            }
            break;

        case 10:
            espTPLvlSet(obj->mdlId[1], 1 - t);
            espTPLvlSet(obj->mdlId[4], 1 - t);
            if(work->time >= work->maxTime) {
                work->mode++;
                work->killF = TRUE;
            }
            break;
    }
}

void mbTelopTimeChangeKill(void)
{
    TELOPTIMECHANGEWORK *work;
    if(telopTimeChangeOMObj) {
        work = omObjGetWork(telopTimeChangeOMObj, TELOPTIMECHANGEWORK);
        work->killF = TRUE;
    }
}

BOOL mbTelopTimeChangeCheck(void)
{
    TELOPTIMECHANGEWORK *work;
    if(telopTimeChangeOMObj == NULL) {
        return FALSE;
    }
    work = omObjGetWork(telopTimeChangeOMObj, TELOPTIMECHANGEWORK);
    return !work->wipeF;
}
