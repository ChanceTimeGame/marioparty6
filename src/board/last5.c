#include "game/board/last5.h"
#include "game/board/guide.h"
#include "game/board/object.h"
#include "game/board/player.h"
#include "game/board/masu.h"
#include "game/board/camera.h"
#include "game/board/audio.h"
#include "game/board/wipe.h"
#include "game/board/window.h"
#include "game/board/status.h"
#include "game/board/snpc.h"
#include "game/board/dice.h"
#include "game/board/coin.h"
#include "game/board/capsule.h"
#include "game/board/exit.h"
#include "game/board/math.h"
#include "game/gamework.h"
#include "game/memory.h"
#include "game/data.h"
#include "game/frand.h"

typedef struct Last5RouletteWork_s {
    u16 unkF : 1;
    u16 killF : 1;
    u16 unkD : 1;
    u16 unkC : 1;
    u16 rollF : 1;
    u16 stopF : 1;
    u16 diceF : 1;
    s16 time;
    s16 unk4;
    s16 unk6;
    s16 result;
    s16 max;
    s16 num;
} LAST5ROULETTEWORK;

typedef struct Last5CoinWork_s {
    s16 delay;
    float vel;
} LAST5COINWORK;

typedef struct Last5Coin_s {
    HuVecF pos;
    float rotX;
    float rotY;
    u8 unk14[4];
    HuVecF scale;
    u8 unk24[4];
    u8 work[8];
} LAST5COIN;

static OMOBJ *Last5RouletteCreate(int masuId);
static void Last5RouletteKill(OMOBJ *obj);
static void Last5RouletteOMExec(OMOBJ *obj);
static void Last5PlayerOrderGet(int *order, int num);
static void ev_Last5Dice(int playerNo);
static void ev_Last5SDiceMotHook(int playerNo);
static void ev_Last5Coin40(int playerNo, OMOBJ *guideObj);
static void ev_Last5CapsuleAdd5(int playerNo, OMOBJ *rouletteObj, OMOBJ *guideObj);
static void ev_Last5Koopa(int playerNo, OMOBJ *rouletteObj, int koopaMdlId);

static int koopaMotTbl[] = { 0xE0001, 0xE0003, 0xE0004, 0xE0005 };

static HuVec2f statusPosTbl[4][2] = {
    { { -98, 72 }, { 114, 72 } },
    { { -98, 152 }, { 114, 152 } },
    { { -98, 232 }, { 114, 232 } },
    { { -98, 312 }, { 114, 312 } },
};

static HuVec2f statusTeamPosTbl[2][2] = {
    { { -124, 80 }, { 140, 80 } },
    { { -124, 160 }, { 140, 160 } },
};

static u32 rankMesTbl[] = { 0x2E0002, 0x2E0004, 0x2E0006, 0x2E0008 };
static u32 teamRankMesTbl[] = { 0x2E0032, 0x2E0034, 0x2E0006, 0x2E0008 };
static u32 last5EffMesTbl[] = { 0x2E0010, 0x2E0012, 0x2E0014, 0x2E0016 };
static u32 last5EffMes2Tbl[] = { 0x2E001A, 0x2E001C, 0x2E0020, 0x2E0022 };

static OMOBJ *last5RouletteOMObj;

static s8 guideMotTbl[] = { 12, 21, 7, 11, 8, 6, -1 };

/* Inlined in several board units (coin, player, dice, star, snpc): a paired-single copy of a vector. */
static inline void Last5VecCopy(register HuVecF *src, register HuVecF *dst)
{
    register float xy;
    register float z;
    asm {
        psq_l xy, 0(src), 0, 0
        lfs z, 8(src)
        psq_st xy, 0(dst), 0, 0
        stfs z, 8(dst)
    }
}

#define GWTeamGet() ((int)GwSystem.tagF)

static inline int GWPlayerTeamGet(int playerNo)
{
    return GwPlayer[playerNo].team;
}

void mbev_Last5(void)
{
    HuVecF pos;
    HuVecF posDst;
    int order[4];
    int teamPlayer[4];
    int teamOrder[2];
    int masuId;
    OMOBJ *rouletteObj;
    int i;
    int winId;
    OMOBJ *guideObj;
    int unk14 = 0;
    int unk10 = 1;
    int unkC = 0;
    s16 guideMdlId;
    int lastPlayer = 3;
    int mesOfs;
    int j;
    int koopaMdlId = -1;
    int result;
    float ofsY;

    masuId = mbMasuFind_AttrIdGet(-1, 0x8000);
    for(i = 0; i < 4; i++) {
        mbPlayerDispSet(i, FALSE);
    }
    mbSNpcDispSet(FALSE);
    rouletteObj = last5RouletteOMObj = Last5RouletteCreate(masuId);
    mbMasuPosGet(masuId, &pos);
    pos.y += 100.0f;
    mbCameraFocusMasuSet(masuId);
    mbCameraOffsetSet(0.0f, 100.0f, 0.0f);
    mbCameraRotSet(-20.0f, 0.0f, 0.0f);
    mbCameraZoomSet(mbCameraPlayerViewZoomGet(0) - 200.0f);
    mbCameraMoveOnSet(0);
    mbMusPlay(0, 0x21, 127, 0);
    HuDataDirClose(0x30000);
    mbWipeFadeIn();
    mbMasuPosGet(masuId, &pos);
    pos.x += 200.0f;
    pos.z += 100.0f;
    guideObj = mbGuideCreateFlag(&pos, guideMotTbl, FALSE, TRUE, TRUE);
    mbGuideMotionNextSet(guideObj, 1);
    guideMdlId = mbGuideModelGet(guideObj);
    mesOfs = 0;
    if(GwSystem.curTime) {
        mesOfs++;
    }
    mbGuideMotionShiftSet(guideObj, 12, TRUE);
    mbAudGuidePlay(0x3B6);
    winId = mbWinCreate(2, mesOfs + 0x2E0000, mbGuideSpeakerNoGet());
    mbWinPlayerDisable(winId, -1);
    mbWinWait(winId);
    for(i = 0; i < 4; i++) {
        mbStatusCapsuleDispSet(i, FALSE);
    }
    if(GWTeamGet() == FALSE) {
        Last5PlayerOrderGet(order, 4);
        unkC = order[0];
        lastPlayer = order[3];
        for(i = 0; i < 4; i++) {
            mbGuideMotionShiftSet(guideObj, 12, TRUE);
            winId = mbWinCreate(2, mesOfs + rankMesTbl[GwPlayer[order[i]].rank], mbGuideSpeakerNoGet());
            mbWinInsertMesSet(winId, mbPlayerNameMesGet(order[i]), 0);
            mbWinPlayerDisable(winId, -1);
            mbStatusMoveSet(order[i], (HuVecF *)&statusPosTbl[i][0], (HuVecF *)&statusPosTbl[i][1], 1, 15);
            while(!mbStatusMoveCheck(order[i])) {
                HuPrcVSleep();
            }
            mbWinWait(winId);
        }
    } else {
        Last5PlayerOrderGet(teamOrder, 2);
        unk14 = teamOrder[0];
        unk10 = teamOrder[1];
        for(i = 0; i < 2; i++) {
            for(j = 0; j < 2; j++) {
                teamPlayer[j] = mbPlayerTeamFindPlayer(teamOrder[i], j);
            }
            if(i == 1) {
                lastPlayer = teamPlayer[mbRandMod(2)];
                if(GwPlayer[lastPlayer].comF) {
                    lastPlayer = mbPlayerTeamFind(lastPlayer);
                }
            } else if(i == 0) {
                unkC = teamPlayer[mbRandMod(2)];
            }
        }
        for(i = 0; i < 2; i++) {
            mbGuideMotionShiftSet(guideObj, 12, TRUE);
            winId = mbWinCreate(2, mesOfs + teamRankMesTbl[mbPlayerTeamRankGet(teamOrder[i])], mbGuideSpeakerNoGet());
            mbWinInsertMesSet(winId, mbPlayerTagNameMesGet(teamOrder[i]), 0);
            mbWinPlayerDisable(winId, -1);
            mbStatusNoMoveSet(teamOrder[i], (HuVecF *)&statusTeamPosTbl[i][0], (HuVecF *)&statusTeamPosTbl[i][1], 1, 15);
            while(!mbStatusMoveCheck(teamOrder[i])) {
                HuPrcVSleep();
            }
            mbWinWait(winId);
        }
    }
    mbGuideMotionShiftSet(guideObj, 12, TRUE);
    mbAudGuidePlay(0x3B8);
    winId = mbWinCreate(2, mesOfs + 0x2E000A, mbGuideSpeakerNoGet());
    mbWinPlayerDisable(winId, -1);
    mbWinWait(winId);
    if(GWTeamGet() == FALSE) {
        for(i = 0; i < 4; i++) {
            mbStatusPosGet(i, &pos);
            Last5VecCopy(&pos, &posDst);
            posDst.x = statusPosTbl[0][0].x;
            mbStatusMoveSet(i, &pos, &posDst, 1, 15);
        }
    } else {
        for(i = 0; i < 2; i++) {
            mbStatusNoPosGet(i, &pos);
            Last5VecCopy(&pos, &posDst);
            posDst.x = statusTeamPosTbl[0][0].x;
            mbStatusNoMoveSet(i, &pos, &posDst, 1, 15);
        }
    }
    while(!mbStatusOffCheckAll()) {
        HuPrcVSleep();
    }
    for(i = 0; i < 4; i++) {
        mbStatusCapsuleDispSet(i, TRUE);
    }
    mbGuideMotionShiftSet(guideObj, 12, TRUE);
    winId = mbWinCreate(2, mesOfs + 0x2E000C, mbGuideSpeakerNoGet());
    mbWinInsertMesSet(winId, mbPlayerNameMesGet(lastPlayer), 0);
    mbWinPlayerDisable(winId, lastPlayer);
    mbWinWait(winId);
    mbMasuPosGet(masuId, &pos);
    pos.x -= 200.0f;
    pos.z += 100.0f;
    Last5VecCopy(&pos, &posDst);
    mbPlayerColSnapPlayerSet(lastPlayer, 0);
    mbPlayerRotSet(lastPlayer, 0.0f, 0.0f, 0.0f);
    mbPlayerMotionSet(lastPlayer, 6, 0x40000001);
    HuPrcVSleep();
    mbPlayerDispSet(lastPlayer, TRUE);
    for(j = 30, i = 0; i <= j; i++) {
        ofsY = 100.0f * (6.0f * mbSinDeg(80.0f * ((float)(j - i) / j)));
        posDst.y = pos.y + ofsY;
        mbPlayerPosSetV(lastPlayer, &posDst);
        i == j - 4;
        HuPrcVSleep();
    }
    omVibrate(lastPlayer, 20, 7, 3);
    for(i = 0; i < 60; i++) {
        HuPrcVSleep();
    }
    mbPlayerMotIdleSet(lastPlayer);
    mbGuideMotionShiftSet(guideObj, 12, TRUE);
    mbAudGuidePlay(0x3B8);
    winId = mbWinCreate(2, mesOfs + 0x2E000E, mbGuideSpeakerNoGet());
    mbWinPlayerDisable(winId, lastPlayer);
    mbWinWait(winId);
    mbGuideMotionShiftSet(guideObj, 21, TRUE);
    while(!mbGuideMotionCheck(guideObj)) {
        HuPrcVSleep();
    }
    ev_Last5Dice(lastPlayer);
    result = ((LAST5ROULETTEWORK *)rouletteObj->work)->result;
    if(result != 3) {
        mbGuideMotionShiftSet(guideObj, 12, TRUE);
        winId = mbWinCreate(2, mesOfs + last5EffMesTbl[result], mbGuideSpeakerNoGet());
        mbWinPlayerDisable(winId, lastPlayer);
        mbWinWait(winId);
        mbGuideMotionShiftSet(guideObj, 12, TRUE);
        mbAudGuidePlay(0x3B8);
        winId = mbWinCreate(2, mesOfs + last5EffMes2Tbl[result], mbGuideSpeakerNoGet());
        mbWinPlayerDisable(winId, lastPlayer);
        if(result == 2) {
            mbWinInsertMesSet(winId, mbPlayerNameMesGet(lastPlayer), 0);
        }
        mbWinWait(winId);
        switch(result) {
            case 0:
                mbGuideMotionShiftSet(guideObj, 6, TRUE);
                mbGuideMotionStop(guideObj);
                HuPrcSleep(30);
                mbAudGuidePlay(0x3B6);
                winId = mbWinCreate(2, mesOfs + 0x2E001E, mbGuideSpeakerNoGet());
                mbWinPlayerDisable(winId, lastPlayer);
                mbWinWait(winId);
                GwSystem.last5Effect = 1;
                break;

            case 1:
                ev_Last5Coin40(lastPlayer, guideObj);
                break;

            case 2:
                ev_Last5CapsuleAdd5(lastPlayer, rouletteObj, guideObj);
                break;
        }
        mbGuideMotionShiftSet(guideObj, 12, TRUE);
        winId = mbWinCreate(2, mesOfs + 0x2E002A, mbGuideSpeakerNoGet());
        mbWinPlayerDisable(winId, -1);
        mbWinWait(winId);
        mbGuideMotionSet(guideObj, 12, TRUE);
        mbAudGuidePlay(0x3B8);
        winId = mbWinCreate(2, mesOfs + 0x2E002C, mbGuideSpeakerNoGet());
        mbWinPlayerDisable(winId, -1);
        mbWinWait(winId);
        mbGuideMotionSet(guideObj, 7, TRUE);
        mbAudGuidePlay(0x3B6);
        winId = mbWinCreate(2, mesOfs + 0x2E002F, mbGuideSpeakerNoGet());
        mbWinPlayerDisable(winId, -1);
        mbWinWait(winId);
    } else {
        mbGuideMotionShiftSet(guideObj, 8, TRUE);
        mbAudGuidePlay(0x3B8);
        mbGuideMotionNextSet(guideObj, 11);
        winId = mbWinCreate(2, mesOfs + last5EffMesTbl[result], mbGuideSpeakerNoGet());
        mbWinPlayerDisable(winId, lastPlayer);
        mbWinWait(winId);
        while(!mbGuideMotionCheck(guideObj)) {
            HuPrcVSleep();
        }
        winId = mbWinCreate(2, mesOfs + 0x2E0018, mbGuideSpeakerNoGet());
        mbWinPlayerDisable(winId, lastPlayer);
        mbObjPosGet(guideMdlId, &pos);
        mbPlayerRotateStart(lastPlayer, 90, 15);
        mbGuideEnd(guideObj, TRUE);
        guideObj = NULL;
        mbWinWait(winId);
        HuPrcSleep(2);
        koopaMdlId = mbObjCreate(0xE0000, koopaMotTbl, TRUE);
        mbObjLayerSet(koopaMdlId, 3);
        mbObjDispSet(koopaMdlId, FALSE);
        mbObjPosSetV(koopaMdlId, &pos);
        ev_Last5Koopa(lastPlayer, rouletteObj, koopaMdlId);
        winId = mbWinCreate(2, 0x2E002E, 13);
        mbWinPlayerDisable(winId, -1);
        mbWinWait(winId);
        mbObjMotionShiftSet(koopaMdlId, 2, 0.0f, 12.0f, 0);
        mbAudFXPlay(0x3D0);
        winId = mbWinCreate(2, 0x2E0031, 13);
        mbWinPlayerDisable(winId, -1);
        mbWinWait(winId);
    }
    mbMusFadeOutSpeed(0, 1000);
    mbWipeFadeOut();
    while(mbMusCheck(0)) {
        HuPrcVSleep();
    }
    mbPlayerPosReset(lastPlayer);
    for(i = 0; i < 4; i++) {
        mbPlayerDispSet(i, TRUE);
    }
    mbSNpcDispSet(TRUE);
    Last5RouletteKill(rouletteObj);
    last5RouletteOMObj = NULL;
    if(guideObj) {
        mbGuideKill(guideObj);
        guideObj = NULL;
    }
    if(koopaMdlId >= 0) {
        mbObjKill(koopaMdlId);
        koopaMdlId = -1;
    }
    HuPrcVSleep();
}

static int rouletteFileTbl[] = { 0x30000, 0x30001, 0x30002 };
static int rouletteChanceTbl[] = { 3, 3, 3, 1, 0 };

static inline void *Last5Malloc(int size)
{
    return HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
}

static inline void Last5Free(void *ptr)
{
    HuMemDirectFree(ptr);
}

static inline void Last5RouletteNext(OMOBJ *obj)
{
    LAST5ROULETTEWORK *work = (LAST5ROULETTEWORK *)obj->work;
    s8 *tbl = obj->data;
    int i;
    int no;
    if(work->num <= 0) {
        work->num = work->max;
    }
    for(i = 0; i < 20; i++) {
        no = mbRandMod(work->num);
        if(work->result != tbl[no]) {
            break;
        }
    }
    work->num--;
    i = tbl[no];
    tbl[no] = tbl[work->num];
    tbl[work->num] = i;
    work->result = i;
}

static OMOBJ *Last5RouletteCreate(int masuId)
{
    OMOBJ *obj;
    LAST5ROULETTEWORK *work;
    int i;
    int j;
    int no;
    s8 *tbl;
    HuVecF pos;

    obj = omAddObjEx(mbObjMan, 256, 3, 0, -1, Last5RouletteOMExec);
    omSetStatBit(obj, 0x100);
    work = (LAST5ROULETTEWORK *)obj->work;
    memset(work, 0, sizeof(LAST5ROULETTEWORK));
    for(i = 0; i < 3; i++) {
        int mdlId = mbObjCreate(rouletteFileTbl[i], NULL, FALSE);
        obj->mdlId[i] = mdlId;
        mbObjLayerSet(mdlId, 3);
        mbObjAttrSet(mdlId, 0x40000001);
    }
    mbMasuPosGet(masuId, &pos);
    pos.z -= 50.0f;
    mbObjPosSetV(obj->mdlId[0], &pos);
    mbObjScaleSet(obj->mdlId[0], 2.0f, 2.0f, 2.0f);
    mbObjHookSet(obj->mdlId[0], "target", obj->mdlId[1]);
    mbObjDispSet(obj->mdlId[2], FALSE);
    mbObjMotionSpeedSet(obj->mdlId[2], 0.0f);
    work->result = 0;
    for(i = 0, j = 0; i < 4; i++) {
        j += rouletteChanceTbl[i];
    }
    work->max = work->num = j;
    tbl = Last5Malloc(work->max);
    obj->data = tbl;
    for(i = 0, no = 0; i < 4; i++) {
        for(j = 0; j < rouletteChanceTbl[i]; j++) {
            tbl[no++] = i;
        }
    }
    for(i = 0; i < 20; i++) {
        Last5RouletteNext(obj);
    }
    return obj;
}

static void Last5RouletteKill(OMOBJ *obj)
{
    LAST5ROULETTEWORK *work = (LAST5ROULETTEWORK *)obj->work;
    work->killF = TRUE;
}

static void Last5RouletteOMExec(OMOBJ *obj)
{
    LAST5ROULETTEWORK *work = (LAST5ROULETTEWORK *)obj->work;
    int i;
    if(mbExitCheck() || work->killF) {
        for(i = 0; i < 3; i++) {
            if(obj->mdlId[i] >= 0) {
                mbObjKill(obj->mdlId[i]);
                obj->mdlId[i] = -1;
            }
        }
        if(obj->data) {
            Last5Free(obj->data);
            obj->data = NULL;
        }
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    if(work->rollF && !work->stopF) {
        work->time++;
        if((work->time & 3) == 0) {
            Last5RouletteNext(obj);
            mbObjMotionTimeSet(obj->mdlId[2], 0.5f + work->result);
        }
    }
}

static void Last5PlayerOrderGet(int *order, int num)
{
    int rank[4];
    int i;
    int j;
    int temp;

    for(i = 0; i < num; i++) {
        order[i] = i;
    }
    for(i = 0; i < num - 1; i++) {
        j = i + mbRandMod(num - i);
        temp = order[j];
        order[j] = order[i];
        order[i] = temp;
    }
    if(GWTeamGet() == FALSE) {
        if(GwPlayer[order[num - 1]].comF) {
            for(i = num - 1; i >= 0; i--) {
                if(!GwPlayer[order[i]].comF) {
                    temp = order[num - 1];
                    order[num - 1] = order[i];
                    order[i] = temp;
                    break;
                }
            }
        }
    } else {
        if(GwPlayer[mbPlayerTeamFindPlayer(order[num - 1], 0)].comF && GwPlayer[mbPlayerTeamFindPlayer(order[num - 1], 1)].comF) {
            for(i = num - 1; i >= 0; i--) {
                if(!GwPlayer[mbPlayerTeamFindPlayer(order[i], 0)].comF || !GwPlayer[mbPlayerTeamFindPlayer(order[i], 1)].comF) {
                    temp = order[num - 1];
                    order[num - 1] = order[i];
                    order[i] = temp;
                    break;
                }
            }
        }
    }
    for(i = 0; i < num; i++) {
        if(num == 4) {
            rank[i] = GwPlayer[order[i]].rank;
        } else {
            rank[i] = mbPlayerTeamRankGet(order[i]);
        }
    }
    for(i = 0; i < num - 1; i++) {
        for(j = i + 1; j < num; j++) {
            if(rank[i] > rank[j]) {
                temp = rank[j];
                rank[j] = rank[i];
                rank[i] = temp;
                temp = order[j];
                order[j] = order[i];
                order[i] = temp;
            }
        }
    }
}

static void ev_Last5Dice(int playerNo)
{
    OMOBJ *obj = last5RouletteOMObj;
    LAST5ROULETTEWORK *work;
    mbObjHookReset(obj->mdlId[0]);
    mbObjDispSet(obj->mdlId[1], FALSE);
    mbObjDispSet(obj->mdlId[2], TRUE);
    mbObjHookSet(obj->mdlId[0], "target", obj->mdlId[2]);
    ((LAST5ROULETTEWORK *)obj->work)->rollF = TRUE;
    ((LAST5ROULETTEWORK *)obj->work)->diceF = TRUE;
    mbDiceProcExec(playerNo, 6, NULL, NULL, 1, 0, NULL, 0);
    mbDiceMotHookSet(playerNo, ev_Last5SDiceMotHook);
    while(!mbDiceKillCheck(playerNo)) {
        HuPrcVSleep();
    }
    mbAudFXPlay(0x3FB);
}

static void ev_Last5SDiceMotHook(int playerNo)
{
    int time;
    mbPlayerMotionSet(playerNo, 11, 0);
    time = 0;
    do {
        if(time++ == 27) {
            ((LAST5ROULETTEWORK *)last5RouletteOMObj->work)->stopF = TRUE;
            mbDiceObjHit(playerNo);
        }
        HuPrcVSleep();
    } while(!mbPlayerMotionEndCheck(playerNo));
    mbPlayerMotIdleSet(playerNo);
}

static void ev_Last5Coin40(int playerNo, OMOBJ *guideObj)
{
    int unk8 = 0;
    int coinNum;
    int i;
    int num;
    LAST5COIN *coinP;
    LAST5COINWORK *workP;
    HuVecF pos;
    s16 coinId[40];

    coinNum = 1;
    mbGuideMotionShiftSet(guideObj, 6, TRUE);
    mbGuideMotionStop(guideObj);
    HuPrcSleep(48);
    coinNum = 40;
    mbPlayerPosGet(playerNo, &pos);
    for(i = 0; i < coinNum; i++) {
        coinId[i] = mbCoinCreate2();
        mbCoinObjDispSet(coinId[i], FALSE);
        coinP = (LAST5COIN *)mbCoinObjGet(coinId[i]);
        coinP->pos.x = pos.x + (60.000004f * (frandf() - 0.5f));
        coinP->pos.y = 800.0f + (pos.y + (50.0f * frandf()));
        coinP->pos.z = pos.z + (60.000004f * (frandf() - 0.5f));
        coinP->rotX = 40.0f * (frandf() - 0.5f);
        coinP->rotY = 360.0f * frandf();
        coinP->scale.x = coinP->scale.y = coinP->scale.z = 0.7f;
        workP = (LAST5COINWORK *)coinP->work;
        workP->delay = (float)(i * 30) / coinNum;
        workP->vel = -13.333334f;
    }
    num = coinNum;
    while(num != 0) {
        for(i = 0, num = 0; i < coinNum; i++) {
            if(coinId[i] == 0) {
                continue;
            }
            num++;
            coinP = (LAST5COIN *)mbCoinObjGet(coinId[i]);
            workP = (LAST5COINWORK *)coinP->work;
            if(workP->delay != 0) {
                workP->delay--;
                continue;
            }
            mbCoinObjDispSet(coinId[i], TRUE);
            workP->vel += -0.5444445f;
            coinP->pos.y += workP->vel;
            if(coinP->pos.y < 100.0f + pos.y) {
                mbCoinObjKill(coinId[i]);
                coinId[i] = 0;
            }
        }
        HuPrcVSleep();
    }
    mbGuideMotionShiftSet(guideObj, 1, TRUE);
    mbPlayerWinLoseVoicePlay(playerNo, 12, 0x243);
    mbPlayerMotionShiftSet(playerNo, 12, 0.0f, 12.0f, 0);
    mbCoinAddProcExec(playerNo, coinNum, TRUE, TRUE);
    mbPlayerMotionEndWait(playerNo);
    mbPlayerMotIdleSet(playerNo);
}

static inline void Last5RouletteDispSet(OMOBJ *obj, BOOL dispF)
{
    LAST5ROULETTEWORK *work = (LAST5ROULETTEWORK *)obj->work;
    mbObjDispSet(obj->mdlId[0], dispF);
    if(work->diceF) {
        mbObjDispSet(obj->mdlId[2], dispF);
    } else {
        mbObjDispSet(obj->mdlId[1], dispF);
    }
}

static void ev_Last5CapsuleAdd5(int playerNo, OMOBJ *rouletteObj, OMOBJ *guideObj)
{
    HuVecF masuPos;
    HuVecF pos;
    s16 *masuList[3];
    int masuNum[3];
    int unk10;
    int unkC = 0;
    int unk8 = 1;
    int capNum;
    int num;
    int i;
    int type;
    int mode;
    int masuId;
    int capsuleNo;
    s16 *masuTbl;
    int capList[33];

    mbGuideMotionShiftSet(guideObj, 6, TRUE);
    while(!mbGuideMotionCheck(guideObj)) {
        HuPrcVSleep();
    }
    mbCameraStackPush();
    mbWipeDissolveFadeOut();
    mbPlayerDispSet(playerNo, FALSE);
    Last5RouletteDispSet(rouletteObj, FALSE);
    capNum = mbCapRandomListGet(capList, 33);
    for(i = 0, num = 0; i < capNum; i++) {
        mode = mbCapUseModeGet(capList[i]);
        if(mode == 1 || mode == 2) {
            capList[num++] = capList[i];
        }
    }
    capNum = num;
    if(capNum < 5) {
        for(; capNum < 5; capNum++) {
            capList[capNum] = capList[0];
        }
    }
    for(type = 0; type < 3; type++) {
        masuList[type] = Last5Malloc(512);
        masuNum[type] = 0;
    }
    for(i = 1, unk10 = 0; i < mbMasuNumGet(); i++) {
        if(!mbCapThrowMasuCheck(i)) {
            continue;
        }
        type = 0;
        if(mbCapMasuDispTypeGet(i) == 0) {
            type = 0;
        } else if(!mbPlayerTeamCheckSame(playerNo, mbCapMasuPlayerGet(i))) {
            type = 1;
        } else {
            type = 2;
        }
        masuList[type][masuNum[type]++] = i;
    }
    pos.x = pos.y = masuPos.z = 0.0f;
    for(type = 0, i = 0; i < 5; i++) {
        for(; type < 3; type++) {
            if(masuNum[type] != 0) {
                break;
            }
        }
        if(type >= 3) {
            break;
        }
        masuTbl = masuList[type];
        num = mbRandMod(masuNum[type]--);
        masuId = masuTbl[num];
        masuTbl[num] = masuTbl[masuNum[type]];
        num = mbRandMod(capNum--);
        capsuleNo = capList[num];
        capList[num] = capList[capNum];
        mbCameraFocusMasuSet(masuId);
        mbCameraRotSet(-30.0f, 0.0f, 0.0f);
        mbCameraZoomSet(mbCameraPlayerViewZoomGet(1));
        mbCameraMoveOnSet(0);
        mbWipeDissolveFadeIn();
        mbMasuPosGet(masuId, &masuPos);
        mbCapAutoThrow(&pos, &pos, &masuPos, playerNo, masuId, capsuleNo, 1, 1.0f);
        HuPrcSleep(2);
        mbWipeDissolveFadeOut();
    }
    for(type = 0; type < 3; type++) {
        Last5Free(masuList[type]);
    }
    mbPlayerDispSet(playerNo, TRUE);
    Last5RouletteDispSet(rouletteObj, TRUE);
    mbCameraFocusReset();
    HuPrcVSleep();
    mbCameraStackPop(0);
    HuPrcVSleep();
    mbWipeDissolveFadeIn();
    mbPlayerWinLoseVoicePlay(playerNo, 12, 0x243);
    mbPlayerMotionShiftSet(playerNo, 12, 0.0f, 12.0f, 0);
    mbPlayerMotionEndWait(playerNo);
    mbPlayerMotIdleSet(playerNo);
}

static void ev_Last5Koopa(int playerNo, OMOBJ *rouletteObj, int koopaMdlId)
{
    int i;
    int value;
    int coinSum;
    int winId;
    float t;
    HuVecF pos;
    HuVecF playerPos;
    int order[5];

    mbObjPosGet(koopaMdlId, &pos);
    mbObjRotSet(koopaMdlId, 0.0f, -5.0f, 0.0f);
    mbObjMotionSet(koopaMdlId, 4, 0);
    mbObjMotionTimeSet(koopaMdlId, 45.0f);
    mbObjMotionSpeedSet(koopaMdlId, 0.0f);
    mbObjDispSet(koopaMdlId, TRUE);
    for(i = 18, value = TRUE; i >= 0; i--) {
        if(i < 8 && value) {
            mbObjMotionSpeedSet(koopaMdlId, 1.0f);
            mbAudFXPlay(0x45F);
            mbPlayerMotionShiftSet(playerNo, 9, 0.0f, 6.0f, 0);
            value = FALSE;
        }
        t = (1.0f / 18.0f) * i;
        mbObjPosSet(koopaMdlId, pos.x, pos.y + (100.0f * (5.0f * mbSinDeg(60.0f * t))), pos.z);
        HuPrcVSleep();
    }
    while(!mbObjMotionEndCheck(koopaMdlId)) {
        HuPrcVSleep();
    }
    mbPlayerMotIdleSet(playerNo);
    mbObjMotionShiftSet(koopaMdlId, 2, 0.0f, 12.0f, 0);
    mbAudFXPlay(0x3CB);
    winId = mbWinCreate(2, 0x2E0022, 13);
    mbWinPlayerDisable(winId, playerNo);
    mbWinWait(winId);
    while(!mbObjMotionEndCheck(koopaMdlId)) {
        HuPrcVSleep();
    }
    mbObjMotionShiftSet(koopaMdlId, 1, 0.0f, 12.0f, 0x40000001);
    winId = mbWinCreate(2, 0x2E0024, 13);
    mbWinPlayerDisable(winId, playerNo);
    mbWinWait(winId);
    mbAudFXPlay(0x3CF);
    mbObjMotionShiftSet(koopaMdlId, 3, 0.0f, 12.0f, 0);
    while(!mbObjMotionEndCheck(koopaMdlId)) {
        HuPrcVSleep();
    }
    mbCameraStackPush();
    mbWipeDissolveFadeOut();
    mbObjMotionSet(koopaMdlId, 1, 0x40000001);
    mbPlayerRotSet(playerNo, 0.0f, 0.0f, 0.0f);
    mbPlayerPosGet(playerNo, &playerPos);
    for(i = 0; i < 4; i++) {
        mbPlayerDispSet(i, TRUE);
    }
    mbPlayerPosResetAll();
    mbObjDispSet(koopaMdlId, FALSE);
    Last5RouletteDispSet(rouletteObj, FALSE);
    for(i = 0; i <= 4; i++) {
        order[i] = -1;
    }
    coinSum = 0;
    if(GWTeamGet() == FALSE) {
        for(i = 0; i < 4; i++) {
            coinSum += mbPlayerCoinGet(i);
            order[i] = i;
        }
        coinSum /= 4;
    } else {
        for(i = 0; i < 2; i++) {
            coinSum += mbPlayerTeamCoinGet(i);
            order[i] = mbPlayerTeamFindPlayer(i, 0);
            if(GwPlayer[order[i]].comF) {
                value = mbPlayerTeamFindPlayer(i, 1);
                if(!GwPlayer[value].comF) {
                    order[i] = value;
                }
            }
            if(i == GWPlayerTeamGet(playerNo)) {
                order[i] = playerNo;
            }
        }
        coinSum /= 2;
    }
    for(i = 0; order[i] >= 0; i++) {
        mbCameraPlayerViewSetFast(order[i], 0);
        mbStatusDispFocusSet(order[i], TRUE);
        mbWipeDissolveFadeIn();
        value = coinSum - mbPlayerCoinGet(order[i]);
        if(value > 0) {
            mbPlayerWinLoseVoicePlay(order[i], 12, 0x243);
            mbPlayerMotionShiftSet(order[i], 12, 0.0f, 12.0f, 0);
        } else if(value < 0) {
            mbPlayerWinLoseVoicePlay(order[i], 13, 0x249);
            mbPlayerMotionShiftSet(order[i], 13, 0.0f, 12.0f, 0);
        }
        mbCoinAddProcExec(order[i], value, value - 1, TRUE);
        mbWipeDissolveFadeOut();
        mbPlayerMotionSet(order[i], 1, 0x40000001);
        mbStatusDispForceSet(order[i], FALSE);
    }
    for(i = 0; i < 4; i++) {
        mbPlayerDispSet(i, FALSE);
    }
    mbPlayerDispSet(playerNo, TRUE);
    mbPlayerPosSetV(playerNo, &playerPos);
    mbObjDispSet(koopaMdlId, TRUE);
    Last5RouletteDispSet(rouletteObj, TRUE);
    mbCameraStackPop(0);
    HuPrcVSleep();
    mbWipeDissolveFadeIn();
}
