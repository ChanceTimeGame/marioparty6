/* Retail calls GWPlayerStarGet as returning int (no extsh of its result): hide the s16
   prototype from game/gamework.h and declare it without one. */
#define GWPlayerStarGet GWPlayerStarGet_s16
#include "game/gamework.h"
#undef GWPlayerStarGet
int GWPlayerStarGet();

#include "game/board/player.h"
#include "game/board/masu.h"
#include "game/board/camera.h"
#define mbCoinObjDispSet mbCoinObjDispSet_int
#include "game/board/coin.h"
#undef mbCoinObjDispSet
#include "game/board/status.h"
#include "game/board/star.h"
#include "game/board/telop.h"
#include "game/board/wipe.h"
#include "game/board/pause.h"
#include "game/board/tutorial.h"
#include "game/board/capevent.h"
#define mbCapSelect mbCapSelect_s8
#define mbSingleCall mbSingleCall_u8
#include "game/board/capselect.h"
#include "game/board/single.h"
#undef mbCapSelect
#undef mbSingleCall
#include "game/board/scroll.h"
#include "game/board/audio.h"
#include "game/board/branch.h"
#include "game/board/gate.h"
#include "game/board/exit.h"
#include "game/board/math.h"
#include "game/board/object.h"
#include "game/charman.h"
#include "game/memory.h"
#include "game/flag.h"
#include "game/audio.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/hsfformat.h"
#include "game/hu3d.h"
#include "game/sprite.h"
#include "string.h"
#include "math.h"

/* capselect.h/single.h type these s8/u8; retail passes and returns ints. */
int mbCapSelect(void);
int mbSingleCall(int arg0, int arg1);

/* coin.h: the id parameter is s16 (retail extsh). */
void mbCoinObjDispSet(s16 coinId, int dispF);

/* dice.h types these s8; this unit uses the full int result. */
int mbDiceProcExec(int playerNo, int diceType, void *arg2, int *valueTbl, int arg4, int arg5, void *arg6, int arg7);
int mbDiceExec(int playerNo, int diceType, void *arg2, int value, int arg4, int arg5, void *arg6, int arg7);
void mbDiceKill(int playerNo);
void mbDiceObjHit(int playerNo);
int mbDiceMaxGet(int diceType);
int mbDiceValueMaxGet(int diceType);
void mbDiceNumKill(int playerNo);

/* Work kept in the player's collision object (playerWork[].unk24->work). */
typedef struct PlayerColWork_s {
    u8 motStartF : 1;
    u8 resetF : 1;
    u8 snapF : 1;
    u8 noRestF : 1;
    u8 playerNo : 2;
    u8 mode : 2;
    u8 circleF;
    u8 masuId;
    u8 masuIdNext;
    s8 time;
    s8 maxTime;
    float rotYStart;
    float radius;
} PLAYERCOLWORK;

#define PLAYER_COLWORK(playerNo) omObjGetWork(playerWork[playerNo].unk24, PLAYERCOLWORK)

typedef struct PlayerMetalWork_s {
    u8 killF : 1;
    u8 fadeInDoneF : 1;
    u8 fadeOutF : 1;
    u8 effectF : 1;
    u8 playerNo : 2;
    s16 time;
    s16 maxTime;
    s16 meshNo;
    s16 cellNum;
} PLAYERMETALWORK;

typedef struct PlayerBiriQWork_s {
    u8 killF : 1;
    u8 flashDoneF : 1;
    u8 flashF : 1;
    u8 fadeOutF : 1;
    u8 effectF : 1;
    u8 playerNo : 2;
    s16 time;
    s16 maxTime;
    s16 meshNo;
    s16 cellNum;
} PLAYERBIRIQWORK;

typedef struct PlayerParticleData_s {
    s16 time;
    s16 maxTime;
    u8 unk4[4];
    float scaleMax;
    float rotSpeed;
    u8 unk10[0x20 - 0x10];
    int vtxNo;
    u8 unk24[0x2C - 0x24];
    float scale;
    u8 unk30[0x38 - 0x30];
    float zRot;
    HuVecF pos;
    GXColor color;
    s16 animNo;
    u8 unk4E[0x58 - 0x4E];
    u8 dispF;
    u8 unk59[0x5C - 0x59];
} PLAYERPARTICLEDATA;

typedef struct PlayerParticle_s {
    s16 endF;
    u8 unk2[0x1C - 0x2];
    void *work;
    u8 unk20;
    u8 unk21[0x26 - 0x21];
    s16 num;
    u32 initF;
    u8 unk2C[0x34 - 0x2C];
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    u8 unk44[0x54 - 0x44];
    GXColor unk54;
    u8 unk58[0x68 - 0x58];
    PLAYERPARTICLEDATA *data;
} PLAYERPARTICLE;

/* Local prototypes: these differ from effect.h/snpc.h (see the report). */
s16 mbParticleCreate(ANIMDATA *anim, s16 num);
void mbParticleKill(s16 modelId);
void mbParticleHookSet(s16 modelId, void *hook);
void mbObjMetalCreate(s16 objId);
int mbObjMetalKill(s16 objId);
void mbObjMetalTPLvlSet(s16 objId, float tpLvl);
void mbObjMetalColorSet(s16 objId, GXColor shadow, GXColor hilite);
void mbObjBiriQCreate(s16 objId);
int mbObjBiriQKill(s16 objId);
void mbObjBiriQColorSet(s16 objId, int mode, float alpha, GXColor color);

/* Work of mbPlayerMoveMain's walk object. */
typedef struct PlayerMoveWork_s {
    u8 killF : 1;
    u8 type : 2;
    u8 playerNo : 2;
    u8 unk0_0 : 3;
    s16 time;
    s16 maxTime;
} PLAYERMOVEWORK;

/* Work of mbPlayerRotateStart's object. */
typedef struct PlayerRotateWork_s {
    u8 endF : 1;
    s8 playerNo;
    s16 maxTime;
    s16 time;
} PLAYERROTATEWORK;

/* Work of the move number (mbMoveNumCreateColor). */
typedef struct MoveNumWork_s {
    u8 killF : 1;
    u8 dispF : 1;
    u8 playerNo : 2;
    u8 viewF : 1;
} MOVENUMWORK;

static MBPLAYERWORK playerWork[GW_PLAYER_MAX];
static BOOL turnIntrF;
static BOOL blackoutF;
static MBTURNHOOK turnInitHook;
static MBTURNHOOK turnCloseHook;
static GXColor metalShadowColor;
static GXColor metalHiliteColor;
static BOOL playerColSnapF;

static GXColor metalDefaultColor[2] = { { 0x80, 0xBE, 0x8C, 0xFF }, { 0x64, 0x32, 0x82, 0xFF } };

static const int charMdlFileTbl[14] = {
    0x00900000, 0x00380000, 0x00C20000, 0x00F50000, 0x00EE0000, 0x00160000, 0x00E80000,
    0x00320000, 0x00D10000, 0x00B90000, 0x002C0000, 0x00B50000, 0x00B00000, 0x00AB0000
};

static const int charMotDirTbl[14] = {
    0x00930000, 0x003B0000, 0x00C50000, 0x00F80000, 0x00F10000, 0x00190000, 0x00EB0000,
    0x00350000, 0x00D40000, 0x00BC0000, 0x002F0000, 0x00BC0000, 0x00BC0000, 0x00BC0000
};

static const u16 charMotNoTbl[15] = {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0016, 0x0006, 0x0007,
    0x0018, 0x002E, 0x000B, 0x0024, 0x0025, 0x0049, 0x0014
};

static const int tutorialCharNoTbl[GW_PLAYER_MAX] = { 3, 0, 2, 4 };

static const int singleCharNoTbl[GW_PLAYER_MAX] = { 0, 11, 12, 13 };

static inline int GWPlayerTeamGet(int playerNo)
{
    return GwPlayer[playerNo].team;
}

static inline BOOL PartyFGet(void)
{
    BOOL partyF = GwSystem.partyF;
    return partyF;
}

static inline int TimeTurnGet(void)
{
    return GwSystem.timeTurn;
}

static inline GW_PLAYER *GWPlayerGet(int playerNo)
{
    return &GwPlayer[playerNo];
}

static inline BOOL GWTeamGet(void)
{
    return GwSystem.tagF;
}


static void ResetMetalColor(void);
static void PlayerColOMExec(OMOBJ *obj);
static void PlayerMetalKill(int playerNo);
static void PlayerBiriQKill(int playerNo);

void mbPlayerInit(BOOL noEventF)
{
    MBPLAYERWORK *workP = &playerWork[0];
    int i, j;
    int charNo;
    int charI;
    int startMasu;
    int grp;
    MBOBJID objId;
    int motDataNum[20];
    memset(playerWork, 0, sizeof(playerWork));
    ResetMetalColor();
    if(_CheckFlag(FLAG_BOARD_MG)) {
        GwSystem.turnPlayerNo = 0;
    }
    if(noEventF) {
        startMasu = mbMasuFind_AttrIdGet(-1, 0x8000);
        for(i=0; i<GW_PLAYER_MAX; i++) {
            if(_CheckFlag(FLAG_BOARD_TUTORIAL)) {
                GwPlayer[i].comF = TRUE;
                GwPlayerConf[i].type = 1;
            }
            if(PartyFGet()) {
                _CheckFlag(FLAG_BOARD_TUTORIAL);
            } else if(i > 0) {
                GwPlayerConf[i].charNo = singleCharNoTbl[i];
                GwPlayer[i].comF = TRUE;
                GwPlayerConf[i].type = 1;
            }
            GwPlayer[i].charNo = GwPlayerConf[i].charNo;
            GwPlayerConf[i].charNo = GwPlayerConf[i].charNo;
            GwPlayer[i].padNo = GwPlayerConf[i].padNo;
            GwPlayerConf[i].padNo = GwPlayerConf[i].padNo;
            GwPlayer[i].comF = GwPlayerConf[i].type;
            GwPlayerConf[i].type = GwPlayerConf[i].type;
            GwPlayer[i].comDif = GwPlayerConf[i].comDif;
            GwPlayerConf[i].comDif = GwPlayerConf[i].comDif;
            GwPlayer[i].masuId = startMasu;
            GwPlayer[i].masuIdNext = startMasu;
            GwPlayer[i].statusColor = 0;
            GwPlayer[i].diceMode = 0;
            for(j=0; j<3; j++) {
                GwPlayer[i].capsule[j] = -1;
            }
            GwPlayer[i].team = grp = GwPlayerConf[i].grpNo;
            GwPlayerConf[i].grpNo = grp;
            GwPlayer[i].orderNo = i;
            mbPlayerMetalSet(i, FALSE);
            mbPlayerBiriQSet(i, FALSE);
        }
        for(charI=0; charI<14; charI++) {
            if(CharMotionAMemPGet(charI)) {
                if(PartyFGet() || charI != mbSingleTeamCharGet()) {
                    for(j=0; j<GW_PLAYER_MAX; j++) {
                        if(charI == GwPlayer[j].charNo) {
                            break;
                        }
                    }
                    if(j >= GW_PLAYER_MAX) {
                        CharDataClose(charI);
                    }
                }
            }
        }
        for(charI=0; charI<GW_PLAYER_MAX; charI++) {
            if(!CharMotionAMemPGet(GwPlayer[charI].charNo)) {
                CharMotionInit(GwPlayer[charI].charNo);
            }
        }
        if(!PartyFGet()) {
            charI = mbSingleTeamCharGet();
            if(!CharMotionAMemPGet(charI)) {
                CharMotionInit(charI);
            }
        }
        GwSystem.playerMode = 0;
    }
    for(i=0; i<GW_PLAYER_MAX; i++, workP++) {
        GW_PLAYER *playerP;
        workP->startTurnHook = workP->endTurnHook = NULL;
        workP->rotateObj = workP->unk18 = workP->unk1C = NULL;
        playerP = GWPlayerGet(i);
        playerP->circuitOrder = workP->playerNo = i;
        GwPlayerConf[i].type = GwPlayer[i].comF;
        GwPlayerConf[i].padNo = GwPlayer[i].padNo;
        GwPlayerConf[i].grpNo = GWPlayerTeamGet(i);
        GwPlayerConf[i].comDif = GwPlayer[i].comDif;
        charNo = GwPlayer[i].charNo;
        GwPlayerConf[i].charNo = charNo;
        for(j=0; j<15; j++) {
            motDataNum[j] = charMotDirTbl[charNo]|charMotNoTbl[j];
        }
        motDataNum[j] = -1;
        objId = workP->objId = mbObjCharCreate(charNo, charMdlFileTbl[charNo], motDataNum, FALSE);
        mbPlayerMotionVoiceOnSet(i, 7, FALSE);
        mbPlayerMotionVoiceOnSet(i, 12, FALSE);
        mbPlayerMotionVoiceOnSet(i, 8, FALSE);
        mbPlayerMotionVoiceOnSet(i, 13, FALSE);
        workP->unk24 = omAddObjEx(mbObjMan, 256, 0, 0, -1, PlayerColOMExec);
        omObjGetWork(workP->unk24, PLAYERCOLWORK)->playerNo = i;
        omObjGetWork(workP->unk24, PLAYERCOLWORK)->resetF = TRUE;
        omObjGetWork(workP->unk24, PLAYERCOLWORK)->masuIdNext = GwPlayer[i].masuId;
        mbPlayerMatClone(i);
        workP->motNo = 1;
        mbObjMotionSet(objId, workP->motNo, 0x40000001);
        GwPlayer[i].dispLightF = TRUE;
        GwPlayer[i].masuIdPrev = -1;
        mbPlayerWorkGet(i)->unk10_3 = TRUE;
        CharModelDataClose(charNo);
    }
    mbPlayerColSnapSet(FALSE);
    if(PartyFGet()) {
        mbPlayerPosResetAll();
    } else {
        for(i=0; i<GW_PLAYER_MAX; i++) {
            if(i > 0) {
                GwPlayer[i].masuId = 0;
                GwPlayer[i].masuIdNext = 0;
                GwPlayer[i].masuIdPrev = 0;
                mbPlayerDispSet(i, FALSE);
            } else {
                mbPlayerPosReset(i);
            }
        }
    }
    CharEffectLayerSet(5);
}

void mbPlayerClose(void)
{
    MBPLAYERWORK *workP = &playerWork[0];
    int i;
    for(i=0; i<GW_PLAYER_MAX; i++, workP++) {
        GW_PLAYER *playerP = GWPlayerGet(i);
        if(workP->objId != MB_OBJ_NONE) {
            PlayerMetalKill(i);
            PlayerBiriQKill(i);
            mbObjKill(workP->objId);
            workP->objId = MB_OBJ_NONE;
        }
        if(workP->unk4C) {
            void *matCopy = workP->unk4C;
            HuMemDirectFree(matCopy);
            workP->unk4C = NULL;
        }
        mbDiceNumKill(i);
    }
}

MBPLAYERWORK *mbPlayerWorkGet(int playerNo)
{
    return &playerWork[playerNo];
}

void mbPlayerTurnInitHookSet(MBTURNHOOK hook)
{
    turnInitHook = hook;
}

void mbPlayerTurnCloseHookSet(MBTURNHOOK hook)
{
    turnCloseHook = hook;
}

void mbPlayerStartTurnHookSet(int playerNo, MBPLAYERTURNHOOK hook)
{
    playerWork[playerNo].startTurnHook = hook;
}

void mbPlayerEndTurnHookSet(int playerNo, MBPLAYERTURNHOOK hook)
{
    playerWork[playerNo].endTurnHook = hook;
}

void mbPlayerMoveHookSet(int playerNo, MBPLAYERMOVEHOOK hook)
{
    playerWork[playerNo].moveHook = hook;
}

static void PlayerTurn(int playerNo);

void mbTurnExec(BOOL intrF)
{
    int i, j;
    int orderNo;
    turnIntrF = intrF;
    blackoutF = FALSE;
    for(i=GwSystem.turnPlayerNo; i<GW_PLAYER_MAX; i++) {
        GwSystem.turnPlayerNo = i;
        orderNo = 1;
        GwPlayer[i].orderNo = 0;
        for(j=0; j<GW_PLAYER_MAX; j++) {
            if(i != j) {
                GwPlayer[j].orderNo = orderNo++;
            }
            mbPlayerMotionSet(j, 1, 0x40000001);
            GwPlayer[j].masuIdNext = GwPlayer[j].masuId;
        }
        PlayerTurn(i);
        turnIntrF = FALSE;
    }
}

void mbSingleTurnExec(BOOL intrF)
{
    turnIntrF = intrF;
    blackoutF = FALSE;
    GwSystem.turnPlayerNo = 0;
    GwPlayer[0].orderNo = 0;
    mbPlayerMotionSet(0, 1, 0x40000001);
    PlayerTurn(0);
    turnIntrF = FALSE;
}

static int PlayerViewSet(int playerNo, BOOL viewF, BOOL waitF, BOOL numF);
static BOOL DiceRun(int playerNo);
static void PlayerMoveCall(int playerNo);
static void ev_PlayerStartTurn(int playerNo);
static void ev_PlayerEndTurn(int playerNo);

static void PlayerTurn(int playerNo)
{
    BOOL timeF = FALSE;
    int i;
    BOOL killerF;
    BOOL capStopF;
    GwSystem.turnPlayerNo = playerNo;
    mbPlayerPosReset(playerNo);
    mbPlayerColSnapSet(TRUE);
    mbev_PlayerColMasuSet(playerNo, GwPlayer[playerNo].masuId, TRUE);
    mbStarDispSetAll(TRUE);
    mbStarMasuDispSet(GwPlayer[playerNo].masuId, FALSE);
    if(!turnIntrF) {
        mbCameraPlayerViewSetFast(playerNo, 0);
        mbCameraMoveOnSet(FALSE);
        mbCameraMoveWait();
        mbPlayerMotionSet(playerNo, 1, 0x40000001);
        mbPlayerRotYSet(playerNo, 0.0f);
        if(turnInitHook) {
            turnInitHook(playerNo);
        }
        mbStatusDispForceSetAll(TRUE);
        if(PartyFGet() && playerNo == 0) {
            int timeTurn = GwSystem.timeTurn;
            if(timeTurn > 0) {
                mbTelopTimeCreate();
                timeF = TRUE;
            }
        }
        if(mbWipeSpecialStatGet()) {
            if(playerNo == 0) {
                mbWipeDissolveFadeIn();
            } else {
                mbWipeSpecialFadeOutCreate(5, 42);
            }
        }
        GwPlayer[playerNo].capsuleUse = -1;
        mbPlayerWorkGet(playerNo)->unkE = 0;
        if(timeF) {
            for(i=0; i<100; i++) {
                HuPrcVSleep();
            }
        }
        ev_PlayerStartTurn(playerNo);
        omVibrate(playerNo, 20, 20, 0);
        mbPauseDisableSet(FALSE);
        mbTutorialCall(3);
        if(PartyFGet() || GwSystem.turnNo == 1) {
            mbTelopPlayerCreate(playerNo);
        }
        GwPlayer[playerNo].moveNum = -1;
        GwPlayer[playerNo].skipEventF = FALSE;
        GwSystem.playerMode = 0;
    }
    for(;;) {
        switch(GwSystem.playerMode) {
            case 0:
            case 2:
                _ClearFlag(0x10010);
                killerF = DiceRun(playerNo);
                if(GwPlayer[playerNo].moveNum != 0) {
                    GwSystem.playerMode = 3;
                }
                if(killerF) {
                    mbev_CapKillerMoveCall(playerNo);
                    GwSystem.playerMode = 4;
                    continue;
                }

            case 3:
                mbPlayerColSnapSet(TRUE);
                if(GwPlayer[playerNo].moveNum != 0) {
                    _ClearFlag(0x10010);
                    turnIntrF = PlayerViewSet(playerNo, TRUE, TRUE, TRUE);
                    mbCameraMoveOnSet(TRUE);
                    if(GwPlayer[playerNo].moveNum != 0) {
                        PlayerMoveCall(playerNo);
                    }
                }
                mbPlayerMetalSet(playerNo, FALSE);
                mbPlayerBiriQSet(playerNo, FALSE);
                GwSystem.playerMode = 4;

            case 4:
                GwSystem.playerMode = 5;
                if(_CheckFlag(3) && mbPlayerKettouCheck(playerNo, GwPlayer[playerNo].masuId)) {
                    mbev_CapCallKettou(playerNo, GwPlayer[playerNo].masuId, FALSE);
                }

            case 5:
                turnIntrF = PlayerViewSet(playerNo, turnIntrF, TRUE, FALSE);
                GwSystem.playerMode = 6;
                mbMasuPlayerColorSet(playerNo);
                capStopF = mbev_MasuCapStop(playerNo, GwPlayer[playerNo].masuId);
                if(!capStopF) {
                    GwSystem.playerMode = 7;
                    continue;
                }

            case 6:
                turnIntrF = PlayerViewSet(playerNo, turnIntrF, FALSE, FALSE);
                GwSystem.playerMode = 7;
                mbCameraMoveWait();
                if(!mbev_MasuStop(playerNo, GwPlayer[playerNo].masuId)) {
                    break;
                }

            case 7:
                turnIntrF = PlayerViewSet(playerNo, turnIntrF, FALSE, FALSE);
                mbPlayerRotateStart(playerNo, 0, 15);
                HuPrcSleep(15);
                mbCameraMoveWait();
                mbPlayerMotIdleSet(playerNo);
                break;
        }
        break;
    }
    ev_PlayerEndTurn(playerNo);
    mbTutorialCall(4);
    if(PartyFGet()) {
        if(playerNo != 3) {
            mbWipeSpecialFadeInCreate(5, 1);
        } else {
            mbWipeFadeOut();
        }
    }
    mbPlayerColSnapPlayerSet(playerNo, TRUE);
    if(turnCloseHook) {
        turnCloseHook(playerNo);
    }
}

static int PlayerViewSet(int playerNo, BOOL viewF, BOOL waitF, BOOL numF)
{
    int wipeF = mbWipeSpecialStatGet();
    if(viewF || wipeF) {
        if(!wipeF) {
            mbCameraPlayerViewSet(playerNo, (numF) ? 2 : 0);
        } else {
            mbCameraPlayerViewSetFast(playerNo, (numF) ? 2 : 0);
        }
        if(waitF) {
            mbCameraMoveWait();
        }
        if(numF && GwPlayer[playerNo].moveNum != 0) {
            mbMoveNumCreate(playerNo, TRUE);
        }
        if(wipeF) {
            mbWipeFadeIn();
        }
        viewF = FALSE;
    }
    return viewF;
}

int mbPlayerDiceTypeGet(int diceMode)
{
    int typeTbl[7][2] = {
        { 0, 0 },
        { 1, 1 },
        { 2, 2 },
        { 3, 14 },
        { 4, 0 },
        { 5, 4 },
        { 6, 3 }
    };
    int i;
    for(i=0; i<7; i++) {
        if(diceMode == typeTbl[i][0]) {
            return typeTbl[i][1];
        }
    }
    return 0;
}

static BOOL DiceRun(int playerNo)
{
    BOOL result = FALSE;
    int capsuleNum;
    BOOL skipF = FALSE;
    int diceNo;
    int value;
    int i;
    int diceType;
    int diceMax;
    int valueTbl[4];
    GwPlayer[playerNo].diceNum = 1;
    do {
        capsuleNum = mbPlayerCapsuleNumGet(playerNo);
        if(!PartyFGet()) {
            capsuleNum = 1;
            if(GwSystem.turnNo <= 1) {
                capsuleNum = 0;
            }
        }
        if(GwPlayer[playerNo].capsuleUse == -1 && !skipF && capsuleNum) {
            GwSystem.playerMode = 0;
            diceNo = mbCapSelect();
            if(GwPlayer[playerNo].capsuleUse != -1) {
                GwPlayer[playerNo].capsuleUseNum++;
            }
        } else {
            mbStatusDispSetAll(TRUE);
            while(!mbStatusOffCheckAll()) {
                HuPrcVSleep();
            }
            GwSystem.playerMode = 2;
            if(_CheckFlag(FLAG_BOARD_TUTORIAL)) {
                diceType = mbPlayerDiceTypeGet(GwPlayer[playerNo].diceMode);
                diceMax = mbDiceMaxGet(diceType);
                for(i=0; i<diceMax; i++) {
                    value = mbTutorialCall(5);
                    if(value < 0) {
                        value = mbRandMod(mbDiceValueMaxGet(diceType));
                    }
                    valueTbl[i] = value;
                }
                if(diceMax <= 1) {
                    diceNo = mbDiceExec(playerNo, diceType, NULL, valueTbl[0], 0, 1, NULL, 0);
                } else {
                    diceNo = mbDiceProcExec(playerNo, diceType, NULL, valueTbl, 0, 1, NULL, 0);
                }
                mbTutorialCall(6);
            } else {
                value = -1;
                if(!PartyFGet()) {
                    value = mbSingleCall(0, -1);
                } else if(GwPlayer[playerNo].comF) {
                    if(mbPlayerDiceTypeGet(GwPlayer[playerNo].diceMode) == 14) {
                        value = mbMasuPKinokoValueGet(playerNo, GwPlayer[playerNo].masuId);
                    }
                }
                diceNo = mbDiceExec(playerNo, mbPlayerDiceTypeGet(GwPlayer[playerNo].diceMode), NULL, value, 1, 1, NULL, 0);
            }
        }
        switch(diceNo) {
            case -3:
                if(!PartyFGet() && !_CheckFlag(FLAG_BOARD_TUTORIAL)) {
                    mbSingleCall(1, -1);
                }
                mbDiceKill(playerNo);
                mbev_Scroll(playerNo, 0);
                break;

            case -4:
                if(!PartyFGet() && !_CheckFlag(FLAG_BOARD_TUTORIAL)) {
                    mbSingleCall(1, -1);
                }
                mbDiceKill(playerNo);
                mbev_Scroll(playerNo, 1);
                break;

            case -6:
                if(!PartyFGet() && !_CheckFlag(FLAG_BOARD_TUTORIAL)) {
                    mbSingleCall(1, -1);
                }
                skipF = FALSE;
                mbDiceKill(playerNo);
                mbAudFXPlay(3);
                break;

            case -7:
                skipF = TRUE;
                mbDiceKill(playerNo);
                break;
        }
    } while(diceNo <= 0);
    if(GwPlayer[playerNo].diceMode == 5) {
        result = TRUE;
    }
    GwPlayer[playerNo].diceMode = 0;
    GwPlayer[playerNo].moveNum = diceNo;
    mbMoveNumCreate(playerNo, TRUE);
    mbDiceNumKill(playerNo);
    if(!PartyFGet()) {
        mbSingleCall(3, diceNo);
    }
    return result;
}

static void PlayerMove(void);
static void PlayerMoveDestroy(void);

static void PlayerMoveCall(int playerNo)
{
    MBPLAYERWORK *workP = &playerWork[playerNo];
    mbPlayerColSnapSet(TRUE);
    workP->moveProc = HuPrcChildCreate(PlayerMove, 8205, 24576, 0, mbMainProc);
    workP->moveProc->property = workP;
    HuPrcDestructorSet2(workP->moveProc, PlayerMoveDestroy);
    while(workP->moveProc) {
        HuPrcVSleep();
    }
    _SetFlag(0x10010);
}

static void PlayerColKill(int playerNo);
static void PlayerBiriQFlashSet(int playerNo);

static void PlayerMove(void)
{
    MBPLAYERWORK *workP = HuPrcCurrentGet()->property;
    int playerNo = workP->playerNo;
    BOOL dispF;
    s16 masuNext;
    mbPlayerWorkGet(playerNo)->unkC = 0;
    mbPlayerWorkGet(playerNo)->unk10_3 = TRUE;
    workP->moveHook = NULL;
    do {
        GwPlayer[playerNo].masuIdPrev = GwPlayer[playerNo].masuId;
        if(!_CheckFlag(FLAG_BOARD_DEBUG) || _CheckFlag(FLAG_BOARD_TUTORIAL)) {
            if(mbev_Branch(playerNo, &masuNext)) {
                break;
            }
        } else {
            if(mbev_BranchDebug(playerNo, &masuNext) || masuNext < 0) {
                break;
            }
        }
        masuNext = mbev_GateMasu(playerNo, GwPlayer[playerNo].masuId, masuNext);
        GwPlayer[playerNo].masuIdNext = masuNext;
        mbPlayerWorkGet(playerNo)->masuNext = -1;
        workP->unk6 = masuNext;
        if(workP->stopF) {
            HuPrcSleep(-1);
        }
        PlayerColKill(playerNo);
        mbev_CapCallTrap(playerNo, GwPlayer[playerNo].masuId, masuNext);
        mbev_MasuMasuEnd(masuNext);
        if(workP->moveHook) {
            mbPlayerWorkGet(playerNo)->unkC = 4;
            workP->moveHook(playerNo);
            workP->moveHook = NULL;
        } else {
            mbPlayerMasuMove(playerNo, TRUE);
        }
        mbPlayerWorkGet(playerNo)->unkC = 0;
        mbPlayerWorkGet(playerNo)->unk10_3 = TRUE;
        mbPlayerWorkGet(playerNo)->unkA = 0;
        GwPlayer[playerNo].masuId = masuNext;
        mbTutorialCall(8);
        dispF = !mbMasuDispCheck(masuNext);
        if(!dispF && GwPlayer[playerNo].biriQF) {
            PlayerBiriQFlashSet(playerNo);
            mbev_CapBiriQShockCreate(playerNo);
        }
        if(!mbev_MasuMasuStart(playerNo)) {
            masuNext = GwPlayer[playerNo].masuId;
            if(!mbev_MasuMove(playerNo, masuNext)) {
                dispF = !mbMasuDispCheck(masuNext);
                if(!dispF) {
                    mbAudFXPlay(1006);
                    GwPlayer[playerNo].moveNum--;
                    if(GwPlayer[playerNo].moveNum < 0) {
                        GwPlayer[playerNo].moveNum = 0;
                    }
                    if(GwPlayer[playerNo].moveNum == 0) {
                        mbMoveNumKill(playerNo);
                    }
                }
            }
        }
        if(workP->stopF) {
            workP->stopDoneF = TRUE;
            HuPrcSleep(-1);
        }
        mbTutorialCall(9);
    } while(GwPlayer[playerNo].moveNum != 0);
    workP->moveHook = NULL;
    mbMoveNumKill(playerNo);
    mbPlayerWorkGet(playerNo)->unkC = 0;
    mbPlayerWorkGet(playerNo)->unk10_3 = TRUE;
    if(workP->stopF) {
        mbPlayerRotateStart(playerNo, 0, 15);
        while(!mbPlayerRotateCheck(playerNo)) {
            HuPrcVSleep();
        }
        mbPlayerMotIdleSet(playerNo);
        mbPlayerColOrderReset();
    } else {
        mbPlayerMotIdleSet(playerNo);
    }
    HuPrcEnd();
}

static void PlayerMoveDestroy(void)
{
    MBPLAYERWORK *workP = HuPrcCurrentGet()->property;
    workP->moveProc = NULL;
}

static void ev_PlayerStartTurn(int playerNo)
{
    if(playerWork[playerNo].startTurnHook && playerWork[playerNo].startTurnHook()) {
        playerWork[playerNo].startTurnHook = NULL;
    }
}

static void ev_PlayerEndTurn(int playerNo)
{
    if(playerWork[playerNo].endTurnHook && playerWork[playerNo].endTurnHook()) {
        playerWork[playerNo].endTurnHook = NULL;
    }
}

void mbPlayerMasuMoveTo(int playerNo, int masuId, BOOL waitF)
{
    GwPlayer[playerNo].masuIdNext = masuId;
    mbPlayerMasuMove(playerNo, waitF);
}

void mbPlayerMasuMove(int playerNo, BOOL waitF)
{
    mbPlayerWorkGet(playerNo)->unkA = 1;
    mbPlayerMoveExec(playerNo, NULL, NULL, mbPlayerWalkSpeedGet(), NULL, waitF);
    mbPlayerWorkGet(playerNo)->unkA = 0;
}

void mbPlayerMasuMovePos(int playerNo, HuVecF *pos, BOOL waitF)
{
    mbPlayerMoveExec(playerNo, NULL, pos, mbPlayerWalkSpeedGet(), NULL, waitF);
}

void mbPlayerMasuMoveSpeed(int playerNo, int masuId, s16 maxTime, BOOL waitF)
{
    HuVecF pos;
    mbMasuPosGet(masuId, &pos);
    mbPlayerWorkGet(playerNo)->unkA = 1;
    mbPlayerMoveExec(playerNo, NULL, &pos, maxTime, NULL, waitF);
    mbPlayerWorkGet(playerNo)->unkA = 0;
}

void mbPlayerMoveExec(int playerNo, HuVecF *srcPos, HuVecF *dstPos, s16 maxTime, HuVecF *rot, BOOL waitF)
{
    mbPlayerMoveMain(playerNo, srcPos, dstPos, 0, 1.0f, 0x40000001, maxTime, rot, waitF);
}

static inline void PlayerMoveFlagSet(int playerNo, BOOL flag)
{
    mbPlayerWorkGet(playerNo)->unk10_3 = flag;
}

static inline void PlayerMoveTimeSet(int playerNo, int time)
{
    mbPlayerWorkGet(playerNo)->masuNext = time;
}

static void PlayerMoveOMExec(OMOBJ *obj);

void mbPlayerMoveMain(int playerNo, HuVecF *srcPos, HuVecF *dstPos, u32 motNo, float motSpeed, u32 motAttr, s16 maxTime, HuVecF *rot, BOOL waitF)
{
    BOOL rotF = FALSE;
    OMOBJ *obj;
    PLAYERMOVEWORK *work;
    int type;
    MASU *masuPrevP;
    s16 masuPrev;
    MASU *masuP;
    float rotY;
    HuVecF dir;
    HuVecF rotV;
    obj = playerWork[playerNo].unk18 = omAddObjEx(mbObjMan, 256, 0, 0, -1, PlayerMoveOMExec);
    work = omObjGetWork(obj, PLAYERMOVEWORK);
    if(srcPos != NULL) {
        obj->trans.x = srcPos->x;
        obj->trans.y = srcPos->y;
        obj->trans.z = srcPos->z;
    } else {
        mbPlayerPosGet(playerNo, (HuVecF *)&obj->trans);
    }
    if(dstPos != NULL) {
        obj->rot.x = dstPos->x;
        obj->rot.y = dstPos->y;
        obj->rot.z = dstPos->z;
        work->type = 0;
    } else {
        int masuNext = GwPlayer[playerNo].masuIdNext;
        masuP = mbMasuGet(masuNext);
        masuPrev = GwPlayer[playerNo].masuIdPrev;
        if(masuPrev < 0) {
            type = 0;
        } else {
            masuPrevP = mbMasuGet(masuPrev);
            if((masuPrevP->attr & 0x2) && (masuP->attr & 0x1)) {
                type = 1;
            } else if((masuPrevP->attr & 0x8) && (masuP->attr & 0x4)) {
                type = 2;
            } else {
                type = 0;
            }
        }
        work->type = type;
        mbMasuPosGet(GwPlayer[playerNo].masuIdNext, (HuVecF *)&obj->rot);
    }
    if(motNo == 0) {
        switch(work->type) {
            case 0:
                mbPlayerWorkGet(playerNo)->unkC = 1;
                mbPlayerMotionShiftSet(playerNo, 3, 0.0f, 4.0f, 0x40000001);
                break;

            case 1:
                mbPlayerWorkGet(playerNo)->unkC = 2;
                mbPlayerMotionShiftSet(playerNo, 4, 6.0f, 2.0f, 0);
                maxTime = 24;
                break;

            case 2:
                mbPlayerWorkGet(playerNo)->unkC = 3;
                mbPlayerMotionShiftSet(playerNo, 14, 0.0f, 4.0f, 0x40000001);
                maxTime = 100;
                motSpeed = 2.0f;
                VECSubtract((Vec *)&obj->rot, (Vec *)&obj->trans, &dir);
                maxTime = VECMag(&dir)/15.000001f;
                if(obj->trans.y >= obj->rot.y) {
                    dir.x = -dir.x;
                    dir.y = -dir.y;
                    dir.z = -dir.z;
                    motSpeed = -motSpeed;
                }
                rotV.x = rotV.z = 0.0f;
                rotV.y = HuAtan(dir.x, dir.z);
                mbPlayerRotSetV(playerNo, &rotV);
                rotF = TRUE;
                break;
        }
    } else {
        mbPlayerMotionShiftSet(playerNo, motNo, 0.0f, 4.0f, motAttr);
    }
    mbPlayerMotionSpeedSet(playerNo, motSpeed);
    if(!rotF) {
        if(rot == NULL) {
            VECSubtract((Vec *)&obj->rot, (Vec *)&obj->trans, &rotV);
            rotY = 90.0-HuAtan(rotV.z, rotV.x);
            mbPlayerRotYSet(playerNo, rotY);
        } else {
            mbPlayerRotSetV(playerNo, rot);
        }
    }
    if(srcPos) {
        mbPlayerPosSetV(playerNo, srcPos);
    }
    obj->scale.x = obj->trans.x;
    obj->scale.y = obj->trans.y;
    obj->scale.z = obj->trans.z;
    work->playerNo = playerNo;
    work->time = 0;
    work->maxTime = maxTime;
    if(mbPlayerWorkGet(work->playerNo)->unkA != 0) {
        PlayerMoveTimeSet(work->playerNo, work->maxTime);
    }
    GwPlayer[playerNo].moveF = TRUE;
    if(waitF) {
        while(GwPlayer[playerNo].moveF) {
            HuPrcVSleep();
        }
    }
    mbPlayerWorkGet(playerNo)->unkC = 0;
    mbPlayerWorkGet(playerNo)->unk10_3 = TRUE;
}

static void PlayerMoveOMExec(OMOBJ *obj)
{
    PLAYERMOVEWORK *work = omObjGetWork(obj, PLAYERMOVEWORK);
    float t;
    if(mbExitCheck() || work->killF) {
        GwPlayer[work->playerNo].moveF = FALSE;
        omDelObjEx(HuPrcCurrentGet(), obj);
        playerWork[work->playerNo].unk18 = NULL;
        return;
    }
    work->time++;
    t = (float)work->time/(float)work->maxTime;
    obj->trans.x = obj->scale.x+(t*(obj->rot.x-obj->scale.x));
    obj->trans.y = obj->scale.y+(t*(obj->rot.y-obj->scale.y));
    obj->trans.z = obj->scale.z+(t*(obj->rot.z-obj->scale.z));
    if(mbPlayerWorkGet(work->playerNo)->unkA != 0) {
        PlayerMoveTimeSet(work->playerNo, work->maxTime-work->time);
    }
    if(work->time >= work->maxTime) {
        GwPlayer[work->playerNo].moveF = FALSE;
        mbPlayerPosSet(work->playerNo, obj->rot.x, obj->rot.y, obj->rot.z);
        omDelObjEx(HuPrcCurrentGet(), obj);
        playerWork[work->playerNo].unk18 = NULL;
        return;
    }
    if(work->type != 1) {
        mbPlayerPosSet(work->playerNo, obj->trans.x, obj->trans.y, obj->trans.z);
    } else {
        PlayerMoveFlagSet(work->playerNo, FALSE);
        if(work->time >= work->maxTime-2) {
            t = 1.0f;
            PlayerMoveFlagSet(work->playerNo, TRUE);
        } else {
            t = (float)work->time/(float)(work->maxTime-2);
        }
        mbPlayerPosSet(work->playerNo, obj->trans.x, obj->trans.y+(100.0*(2.0*HuSin(180.0f*t))), obj->trans.z);
        if(work->time == work->maxTime-5) {
            mbPlayerMotionShiftSet(work->playerNo, 5, 2.0f, 2.0f, 0);
        }
    }
}

static void PlayerRotateOMExec(OMOBJ *obj);

void mbPlayerRotateStart(int playerNo, s16 endAngle, s16 maxTime)
{
    OMOBJ *obj;
    PLAYERROTATEWORK *work;
    float diff;
    if(maxTime <= 0) {
        return;
    }
    if(playerWork[playerNo].rotateObj) {
        obj = playerWork[playerNo].rotateObj;
    } else {
        obj = omAddObjEx(mbObjMan, 256, 0, 0, -1, PlayerRotateOMExec);
        playerWork[playerNo].rotateObj = obj;
    }
    work = omObjGetWork(obj, PLAYERROTATEWORK);
    work->endF = FALSE;
    work->maxTime = maxTime;
    work->playerNo = playerNo;
    work->time = 0;
    obj->rot.y = mbPlayerRotYGet(playerNo);
    obj->scale.z = endAngle;
    diff = fmod((float)endAngle-obj->rot.y, 360.0);
    if((s16)diff == 0) {
        mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 5.0f, 0x40000001);
        work->endF = TRUE;
        return;
    }
    if(diff < 0.0f) {
        diff += 360.0f;
    }
    if(diff > 180.0f) {
        diff -= 360.0f;
    }
    obj->scale.y = diff;
    if(fabs(diff) > 5.0) {
        mbPlayerMotionShiftSet(playerNo, 2, 0.0f, 5.0f, 0x40000001);
    } else {
        mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 5.0f, 0x40000001);
    }
}

static void PlayerRotateOMExec(OMOBJ *obj)
{
    PLAYERROTATEWORK *work = omObjGetWork(obj, PLAYERROTATEWORK);
    float rotY;
    float t;
    float s;
    if(work->endF || mbExitCheck()) {
        mbPlayerRotYSet(work->playerNo, obj->scale.z);
        playerWork[work->playerNo].rotateObj = NULL;
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    t = (float)(work->time++)/(float)work->maxTime;
    s = HuSin(90.0f*t);
    rotY = obj->rot.y+(s*obj->scale.y);
    mbPlayerRotYSet(work->playerNo, rotY);
    if(work->time >= work->maxTime) {
        work->endF = TRUE;
        mbPlayerMotionSet(work->playerNo, 1, 0x40000001);
        return;
    }
}

BOOL mbPlayerRotateCheck(int playerNo)
{
    return playerWork[playerNo].rotateObj == NULL;
}

BOOL mbPlayerRotateCheckAll(void)
{
    int i;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        if(playerWork[i].rotateObj != NULL) {
            return FALSE;
        }
    }
    return TRUE;
}

void mbPlayerDiceMotExec(int playerNo)
{
    int time;
    mbPlayerMotionSet(playerNo, 11, 0);
    time = 0;
    do {
        if(time++ == 27) {
            mbDiceObjHit(playerNo);
        }
        HuPrcVSleep();
    } while(!mbPlayerMotionEndCheck(playerNo));
    mbPlayerMotIdleSet(playerNo);
}

static void MoveNumOMExec(OMOBJ *obj);

void mbMoveNumCreateColor(int playerNo, BOOL dispF, int color)
{
    HU3D_CAMERA *cameraP = &Hu3DCamera[0];
    OMOBJ *obj;
    MOVENUMWORK *work;
    int i;
    int coinId;
    float tanFov;
    HuVecF pos;
    HuVecF normPos;
    Mtx lookAt;
    if(!playerWork[playerNo].unk20) {
        obj = omAddObjEx(mbObjMan, 32258, 20, 0, -1, MoveNumOMExec);
        playerWork[playerNo].unk20 = obj;
        omSetStatBit(obj, 0x100);
        work = omObjGetWork(obj, MOVENUMWORK);
        work->dispF = TRUE;
        work->killF = FALSE;
        work->playerNo = playerNo;
        work->viewF = dispF;
        for(i=0; i<20; i++) {
            coinId = mbCoinObjCreate(i%10, color);
            mbCoinObjLayerSet(coinId, 4);
            mbCoinObjDispSet(coinId, FALSE);
            obj->mdlId[i] = coinId;
        }
        mbPlayerPosGet(playerNo, &pos);
        mbPos3DtoNorm(&pos, 1, &normPos);
        obj->trans.y = normPos.y;
        pos.y += 300.0f;
        mbPos3DtoNorm(&pos, 1, &normPos);
        obj->trans.y = normPos.y-obj->trans.y;
        obj->trans.z = normPos.z;
        if(dispF) {
            mbPlayerPosGet(playerNo, &pos);
            pos.y += 300.0f;
            MTXLookAt(lookAt, &cameraP->pos, &cameraP->up, &cameraP->target);
            MTXMultVec(lookAt, &pos, &normPos);
            tanFov = HuSin(0.5f*cameraP->fov)/HuCos(0.5f*cameraP->fov);
            obj->rot.y = normPos.y/(tanFov*normPos.z);
            obj->rot.z = -normPos.z;
        }
    }
}

void mbMoveNumCreate(int playerNo, BOOL dispF)
{
    mbMoveNumCreateColor(playerNo, dispF, 0);
}

static void MoveNumOMExec(OMOBJ *obj)
{
    int digitNum = 0;
    MOVENUMWORK *work = omObjGetWork(obj, MOVENUMWORK);
    HU3D_CAMERA *cameraP = &Hu3DCamera[0];
    HU3D_CAMERA *camera1P = &Hu3DCamera[2];
    int i;
    int digit;
    float tanFov;
    float scaleX;
    float scaleY;
    float rotZ;
    float scale;
    HuVecF pos;
    HuVecF normPos;
    if(work->killF || mbExitCheck()) {
        for(i=0; i<20; i++) {
            if(obj->mdlId[i] != -1) {
                mbCoinObjNumDec(obj->mdlId[i]);
                obj->mdlId[i] = -1;
            }
        }
        omDelObjEx(HuPrcCurrentGet(), obj);
        playerWork[work->playerNo].unk20 = NULL;
        return;
    }
    if(mbPauseEnableCheck()) {
        return;
    }
    mbPlayerPosGet(work->playerNo, &pos);
    mbPos3DtoNorm(&pos, 1, &normPos);
    normPos.y += obj->trans.y;
    normPos.z = obj->trans.z;
    tanFov = HuSin(0.5f*camera1P->fov)/HuCos(0.5f*camera1P->fov);
    scaleX = 1.2f*(tanFov*-normPos.z);
    scaleY = tanFov*-normPos.z;
    normPos.x *= scaleX;
    normPos.y *= scaleY;
    mbVecCopy(&normPos, &pos);
    mbCameraRotGet(&normPos);
    rotZ = -normPos.x;
    for(i=0; i<20; i++) {
        mbCoinObjDispSet(obj->mdlId[i], FALSE);
    }
    if(work->dispF) {
        scale = HuSin(0.5f*cameraP->fov)/HuCos(0.5f*cameraP->fov);
        scale = tanFov/scale;
        digit = GwPlayer[work->playerNo].moveNum/10;
        if(digit != 0) {
            mbCoinObjDispSet(obj->mdlId[digit], TRUE);
            mbCoinObjPosSet(obj->mdlId[digit], pos.x-(60.000004f*scale), pos.y, pos.z);
            mbCoinObjRotSet(obj->mdlId[digit], rotZ, 0.0f, 0.0f);
            mbCoinObjScaleSet(obj->mdlId[digit], scale, scale, scale);
            digitNum++;
        }
        digit = (GwPlayer[work->playerNo].moveNum%10)+10;
        if(digit != 0) {
            mbCoinObjDispSet(obj->mdlId[digit], TRUE);
            if(digitNum == 0) {
                mbCoinObjPosSet(obj->mdlId[digit], pos.x, pos.y, pos.z);
            } else {
                mbCoinObjPosSet(obj->mdlId[digit], pos.x+(60.000004f*scale), pos.y, pos.z);
            }
            mbCoinObjRotSet(obj->mdlId[digit], rotZ, 0.0f, 0.0f);
            mbCoinObjScaleSet(obj->mdlId[digit], scale, scale, scale);
        }
    }
}

void mbMoveNumKill(int playerNo)
{
    MOVENUMWORK *work;
    if(playerWork[playerNo].unk20) {
        work = omObjGetWork(playerWork[playerNo].unk20, MOVENUMWORK);
        work->killF = TRUE;
    }
}

void mbMoveNumDispSet(int playerNo, BOOL dispF)
{
    MOVENUMWORK *work;
    if(playerWork[playerNo].unk20) {
        work = omObjGetWork(playerWork[playerNo].unk20, MOVENUMWORK);
        work->dispF = dispF;
    }
}

static void PlayerColCornerSnap(int playerNo, int masuId, int cornerNo);
static void PlayerColInit(int playerNo, int masuId, int cornerNo);

void mbev_PlayerColMasuAllSet(int *masuTbl, BOOL snapF)
{
    int i, j;
    int cornerNo;
    int masuId;
    int circleF;
    s8 orderNo;
    HuVecF pos;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        if(playerWork[i].unk24) {
            if(GwPlayer[i].masuId == 0) {
                continue;
            }
            PLAYER_COLWORK(i)->masuIdNext = GwPlayer[i].masuIdNext;
            if(PLAYER_COLWORK(i)->noRestF) {
                continue;
            }
        }
        if(masuTbl[i] >= 0) {
            mbPlayerMasuCornerSet(i, 0);
            PlayerColCornerSnap(i, masuTbl[i], 0);
            continue;
        }
        if(playerWork[i].unk24 && !PLAYER_COLWORK(i)->snapF) {
            continue;
        }
        masuId = GwPlayer[i].masuId;
        orderNo = GwPlayer[i].orderNo;
        cornerNo = 0;
        for(j=0; j<GW_PLAYER_MAX; j++) {
            if(i != j && masuId == masuTbl[j]) {
                cornerNo++;
            }
        }
        for(j=0; j<GW_PLAYER_MAX; j++) {
            if(i != j && masuTbl[j] < 0 && masuId == GwPlayer[j].masuId && orderNo > GwPlayer[j].orderNo) {
                cornerNo++;
            }
        }
        if(cornerNo == 0) {
            mbMasuPosGet(masuId, &pos);
        } else {
            mbMasuCornerRotPosGet(masuId, cornerNo-1, &pos);
        }
        circleF = PLAYER_COLWORK(i)->circleF;
        PLAYER_COLWORK(i)->circleF = FALSE;
        if(snapF) {
            mbPlayerPosSetV(i, &pos);
            PlayerColCornerSnap(i, masuId, cornerNo);
        } else if(mbPlayerMasuCornerGet(i) != cornerNo || circleF) {
            PlayerColInit(i, masuId, cornerNo);
        }
        mbPlayerMasuCornerSet(i, cornerNo);
    }
}

void mbev_PlayerColMasu(int playerNo, int masuId, BOOL snapF)
{
    int i, j;
    int num;
    int cornerNo;
    int circleF;
    HuVecF pos;
    int orderTbl[GW_PLAYER_MAX];
    int playerTbl[GW_PLAYER_MAX];
    num = 0;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        if(playerNo == i || (GwPlayer[i].masuId != 0 && masuId == GwPlayer[i].masuId)) {
            orderTbl[num] = GwPlayer[i].orderNo;
            if(playerNo == i) {
                orderTbl[num] = -1;
            }
            playerTbl[num] = i;
            num++;
            mbPlayerColSnapPlayerSet(i, TRUE);
            mbPlayerColRestSet(i, TRUE);
        }
    }
    for(i=0; i<num-1; i++) {
        for(j=i+1; j<num; j++) {
            if(orderTbl[i] > orderTbl[j]) {
                cornerNo = orderTbl[i];
                orderTbl[i] = orderTbl[j];
                orderTbl[j] = cornerNo;
                cornerNo = playerTbl[i];
                playerTbl[i] = playerTbl[j];
                playerTbl[j] = cornerNo;
            }
        }
    }
    for(j=0; j<num; j++) {
        i = playerTbl[j];
        cornerNo = j;
        if(cornerNo != 0) {
            mbMasuCornerRotPosGet(masuId, cornerNo-1, &pos);
        } else {
            mbMasuPosGet(masuId, &pos);
        }
        circleF = PLAYER_COLWORK(i)->circleF;
        PLAYER_COLWORK(i)->circleF = FALSE;
        if(snapF) {
            mbPlayerPosSetV(i, &pos);
            PlayerColCornerSnap(i, masuId, cornerNo);
        } else if(mbPlayerMasuCornerGet(i) != cornerNo || circleF) {
            PlayerColInit(i, masuId, cornerNo);
        }
        mbPlayerMasuCornerSet(i, cornerNo);
    }
}

void mbev_PlayerColCircleAdd(int playerNo, int masuId, BOOL snapF, float radius)
{
    int i, j;
    int num;
    int cornerNo;
    float scale;
    HuVecF pos;
    HuVecF dir;
    int orderTbl[GW_PLAYER_MAX];
    int playerTbl[GW_PLAYER_MAX];
    num = 0;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        if(playerNo == i || (GwPlayer[i].masuId != 0 && masuId == GwPlayer[i].masuId)) {
            PLAYER_COLWORK(i)->circleF = TRUE;
            PLAYER_COLWORK(i)->radius = radius;
            orderTbl[num] = GwPlayer[i].orderNo;
            if(playerNo == i) {
                orderTbl[num] = -1;
            }
            playerTbl[num] = i;
            num++;
            mbPlayerColSnapPlayerSet(i, TRUE);
            mbPlayerColRestSet(i, TRUE);
        }
    }
    for(i=0; i<num-1; i++) {
        for(j=i+1; j<num; j++) {
            if(orderTbl[i] > orderTbl[j]) {
                cornerNo = orderTbl[i];
                orderTbl[i] = orderTbl[j];
                orderTbl[j] = cornerNo;
                cornerNo = playerTbl[i];
                playerTbl[i] = playerTbl[j];
                playerTbl[j] = cornerNo;
            }
        }
    }
    for(j=0; j<num; j++) {
        i = playerTbl[j];
        cornerNo = j;
        if(playerNo < 0 && cornerNo == 0) {
            cornerNo = num;
        }
        mbMasuPosGet(masuId, &pos);
        if(cornerNo != 0) {
            mbMasuCornerRotPosGet(masuId, cornerNo-1, &dir);
            VECSubtract(&dir, &pos, &dir);
            scale = radius/VECMag(&dir);
            VECScale(&dir, &dir, scale);
            VECAdd(&pos, &dir, &pos);
        }
        if(snapF) {
            mbPlayerPosSetV(i, &pos);
            PlayerColCornerSnap(i, masuId, cornerNo);
        } else {
            PlayerColInit(i, masuId, cornerNo);
        }
        mbPlayerMasuCornerSet(i, cornerNo);
    }
}

/* ---- part 2 (fork b202) ---- */

void mbev_PlayerColMasuAdd(int playerNo, int masuId, BOOL snapF)
{
    int i;
    int cornerNo;
    int j;
    BOOL circleF;
    s8 orderNo;
    HuVecF pos;

    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(masuId != GwPlayer[i].masuId) {
            continue;
        }
        if(i == playerNo) {
            continue;
        }
        if(playerWork[i].unk24) {
            PLAYER_COLWORK(i)->masuIdNext = GwPlayer[i].masuIdNext;
            if(PLAYER_COLWORK(i)->noRestF) {
                continue;
            }
            if(!PLAYER_COLWORK(i)->snapF) {
                continue;
            }
        }
        orderNo = GwPlayer[i].orderNo;
        cornerNo = 1;
        if(playerNo < 0) {
            cornerNo = 0;
        }
        for(j = 0; j < GW_PLAYER_MAX; j++) {
            if(i != j && masuId == GwPlayer[j].masuId && orderNo > GwPlayer[j].orderNo) {
                cornerNo++;
            }
        }
        if(cornerNo != 0) {
            mbMasuCornerRotPosGet(masuId, cornerNo - 1, &pos);
        } else {
            mbMasuPosGet(masuId, &pos);
        }
        circleF = PLAYER_COLWORK(i)->circleF;
        PLAYER_COLWORK(i)->circleF = FALSE;
        if(snapF) {
            mbPlayerPosSetV(i, &pos);
            PlayerColCornerSnap(i, masuId, cornerNo);
        } else if(cornerNo != mbPlayerMasuCornerGet(i) || circleF) {
            PlayerColInit(i, masuId, cornerNo);
        }
        mbPlayerMasuCornerSet(i, cornerNo);
    }
    if(playerNo >= 0) {
        mbPlayerMasuCornerSet(playerNo, 0);
    }
}

void mbev_PlayerColBall(int masuId, int *playerMasuTbl, HuVecF *posOut)
{
    HuVecF posTbl[GW_PLAYER_MAX];
    int orderTbl[GW_PLAYER_MAX];
    int playerNoTbl[GW_PLAYER_MAX];
    int flagTbl[GW_PLAYER_MAX] = { 0 };
    int i;
    int j;
    int num;
    int temp;

    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(playerMasuTbl[i] >= 0) {
            flagTbl[playerMasuTbl[i]] = TRUE;
        }
    }
    num = 0;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(masuId == GwPlayer[i].masuId || flagTbl[i]) {
            orderTbl[num] = GwPlayer[i].orderNo;
            if(orderTbl[num] && !mbPlayerColSnapGet(i)) {
                orderTbl[num] += 4;
            }
            playerNoTbl[num] = i;
            num++;
        }
    }
    for(i = 0; i < num - 1; i++) {
        for(j = i + 1; j < num; j++) {
            if(orderTbl[i] > orderTbl[j]) {
                temp = orderTbl[i];
                orderTbl[i] = orderTbl[j];
                orderTbl[j] = temp;
                temp = playerNoTbl[i];
                playerNoTbl[i] = playerNoTbl[j];
                playerNoTbl[j] = temp;
            }
        }
    }
    for(j = 0; j < num; j++) {
        i = playerNoTbl[j];
        temp = j;
        if(temp != 0) {
            mbMasuCornerRotPosGet(masuId, temp - 1, &posTbl[i]);
        } else {
            mbMasuPosGet(masuId, &posTbl[i]);
        }
    }
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(playerMasuTbl[i] >= 0) {
            mbVecCopy(&posTbl[playerMasuTbl[i]], &posOut[i]);
        }
    }
}

void mbev_PlayerColMasuSet(int playerNo, int masuId, BOOL snapF)
{
    int masuIdTbl[GW_PLAYER_MAX];
    int i;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        masuIdTbl[i] = -1;
    }
    masuIdTbl[playerNo] = masuId;
    mbev_PlayerColMasuAllSet(masuIdTbl, snapF);
}

static void PlayerColCornerSet(int playerNo, int masuIdNext)
{
    int i;
    int masuIdTbl[GW_PLAYER_MAX];
    int j;
    int cornerNo;
    int masuId;
    BOOL circleF;
    s8 orderNo;
    HuVecF pos;

    for(i = 0; i < GW_PLAYER_MAX; i++) {
        masuIdTbl[i] = -1;
    }
    masuIdTbl[playerNo] = masuIdNext;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(playerWork[i].unk24 == NULL) {
            continue;
        }
        if(GwPlayer[i].masuId == 0) {
            continue;
        }
        PLAYER_COLWORK(i)->masuIdNext = GwPlayer[i].masuIdNext;
        if(i == playerNo) {
            continue;
        }
        if(PLAYER_COLWORK(i)->noRestF) {
            continue;
        }
        if(!PLAYER_COLWORK(i)->snapF) {
            continue;
        }
        masuId = GwPlayer[i].masuId;
        orderNo = GwPlayer[i].orderNo;
        cornerNo = 0;
        for(j = 0; j < GW_PLAYER_MAX; j++) {
            if(i != j && masuId == masuIdTbl[j]) {
                cornerNo++;
            }
        }
        for(j = 0; j < GW_PLAYER_MAX; j++) {
            if(i != j && masuIdTbl[j] < 0 && masuId == GwPlayer[j].masuId && orderNo > GwPlayer[j].orderNo) {
                cornerNo++;
            }
        }
        if(cornerNo == 0) {
            mbMasuPosGet(masuId, &pos);
        } else {
            mbMasuCornerRotPosGet(masuId, cornerNo - 1, &pos);
        }
        circleF = PLAYER_COLWORK(i)->circleF;
        PLAYER_COLWORK(i)->circleF = FALSE;
        if(cornerNo != mbPlayerMasuCornerGet(i) || circleF) {
            PlayerColInit(i, masuId, cornerNo);
        }
        mbPlayerMasuCornerSet(i, cornerNo);
    }
}

static void PlayerColCornerSnap(int playerNo, int masuId, int cornerNo)
{
    MBPLAYERWORK *player = mbPlayerWorkGet(playerNo);
    PLAYERCOLWORK *work = omObjGetWork(player->unk24, PLAYERCOLWORK);
    Mtx masuMtx;
    Mtx masuMtxInv;
    HuVecF posPlayer;
    if(work->snapF) {
        work->motStartF = FALSE;
        work->mode = 0;
        work->resetF = FALSE;
        work->masuId = masuId;
        player->masuCorner = cornerNo;
        mbMasuMtxGet(work->masuId, masuMtx);
        MTXInverse(masuMtx, masuMtxInv);
        mbPlayerPosGet(playerNo, &posPlayer);
        MTXMultVec(masuMtxInv, &posPlayer, &playerWork[playerNo].unk3C);
        MTXMultVec(masuMtxInv, &posPlayer, &playerWork[playerNo].unk24->trans);
        mbPlayerRotYSet(playerNo, 0.0f);
        work->rotYStart = 0.0f;
        GwPlayer[playerNo].moveF = FALSE;
    }
}

static void PlayerColInit(int playerNo, int masuId, int cornerNo)
{
    MBPLAYERWORK *player = mbPlayerWorkGet(playerNo);
    PLAYERCOLWORK *work = omObjGetWork(player->unk24, PLAYERCOLWORK);
    Mtx masuMtx;
    Mtx masuMtxInv;
    HuVecF posPlayer;
    HuVecF posMasu;
    HuVecF dir;
    float scale;
    float rotY;
    if(work->snapF) {
        work->motStartF = FALSE;
        work->mode = 1;
        work->resetF = FALSE;
        work->masuId = masuId;
        player->masuCorner = cornerNo;
        mbMasuMtxGet(work->masuId, masuMtx);
        MTXInverse(masuMtx, masuMtxInv);
        mbPlayerPosGet(playerNo, &posPlayer);
        MTXMultVec(masuMtxInv, &posPlayer, &playerWork[playerNo].unk24->trans);
        GwPlayer[playerNo].moveF = TRUE;
        if(player->masuCorner == 0) {
            mbMasuPosGet(work->masuId, &posMasu);
        } else {
            mbMasuCornerRotPosGet(work->masuId, player->masuCorner - 1, &posMasu);
            if(work->circleF) {
                mbMasuPosGet(work->masuId, &dir);
                VECSubtract(&posMasu, &dir, &posMasu);
                scale = work->radius / VECMag(&posMasu);
                VECScale(&posMasu, &posMasu, scale);
                VECAdd(&posMasu, &dir, &posMasu);
            }
        }
        VECSubtract(&posMasu, &posPlayer, &dir);
        rotY = 90 - HuAtan(dir.z, dir.x);
        mbPlayerRotYSet(playerNo, rotY);
        work->rotYStart = rotY;
    }
}

int mbPlayerColCheck(void)
{
    int i;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        PLAYERCOLWORK *work = PLAYER_COLWORK(i);
        if(work->mode != 0) {
            return FALSE;
        }
    }
    return TRUE;
}

static void PlayerColKill(int playerNo)
{
}

void mbev_PlayerColReserve(int playerNo, int masuId, BOOL snapF)
{
    int masuIdTbl[GW_PLAYER_MAX];
    int i;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        masuIdTbl[i] = -1;
    }
    masuIdTbl[playerNo] = masuId;
    mbev_PlayerColMasuAllSet(masuIdTbl, snapF);
}

static void PlayerColOMExec(OMOBJ *obj)
{
    PLAYERCOLWORK *work = omObjGetWork(obj, PLAYERCOLWORK);
    int playerNo = work->playerNo;
    MBPLAYERWORK *player = mbPlayerWorkGet(playerNo);
    Mtx masuMtx;
    Mtx masuMtxInv;
    HuVecF pos;
    HuVecF dir;
    float weight;
    float rotY;

    if(mbExitCheck()) {
        omDelObjEx(HuPrcCurrentGet(), obj);
        player->unk24 = NULL;
        return;
    }
    if(GwPlayer[playerNo].masuId == 0) {
        return;
    }
    if(work->masuIdNext != GwPlayer[playerNo].masuIdNext) {
        work->masuIdNext = GwPlayer[playerNo].masuIdNext;
        PlayerColCornerSet(playerNo, work->masuIdNext);
    }
    if(!work->snapF || player->unk18 || player->unk1C) {
        work->resetF = TRUE;
        work->mode = 0;
        return;
    }
    if(work->resetF) {
        work->resetF = FALSE;
        work->masuId = GwPlayer[playerNo].masuId;
        work->masuIdNext = GwPlayer[playerNo].masuIdNext;
        mbMasuMtxGet(work->masuId, masuMtx);
        MTXInverse(masuMtx, masuMtxInv);
        mbPlayerPosGet(playerNo, &pos);
        MTXMultVec(masuMtxInv, &pos, &player->unk3C);
        MTXMultVec(masuMtxInv, &pos, &obj->trans);
        player->unk3C.y = obj->trans.y = 0.0f;
    }
    switch(work->mode) {
        case 0:
            obj->trans.x = player->unk3C.x;
            obj->trans.y = player->unk3C.y;
            obj->trans.z = player->unk3C.z;
            break;

        case 1:
            if(!work->motStartF) {
                mbPlayerMotionSet(playerNo, 3, HU3D_MOTATTR_LOOP);
                work->motStartF = TRUE;
                work->time = 0;
                work->maxTime = 12;
                obj->rot.x = obj->trans.x;
                obj->rot.y = obj->trans.y;
                obj->rot.z = obj->trans.z;
            }
            if(player->masuCorner == 0) {
                player->unk3C.x = player->unk3C.y = player->unk3C.z = 0.0f;
            } else {
                mbMasuCornerPosGet(work->masuId, player->masuCorner - 1, &player->unk3C);
                if(work->circleF) {
                    weight = work->radius / VECMag(&player->unk3C);
                    VECScale(&player->unk3C, &player->unk3C, weight);
                }
            }
            if(work->time > work->maxTime) {
                work->mode = 2;
                work->time = 0;
                work->maxTime = 8;
                obj->trans.x = player->unk3C.x;
                obj->trans.y = player->unk3C.y;
                obj->trans.z = player->unk3C.z;
                mbPlayerMotionShiftSet(playerNo, 1, 0, 8, HU3D_MOTATTR_LOOP);
            } else {
                weight = (float)(work->time++) / work->maxTime;
                VECSubtract(&player->unk3C, &obj->rot, &dir);
                obj->trans.x = obj->rot.x + (weight * (player->unk3C.x - obj->rot.x));
                obj->trans.y = obj->rot.y + (weight * (player->unk3C.y - obj->rot.y));
                obj->trans.z = obj->rot.z + (weight * (player->unk3C.z - obj->rot.z));
            }
            break;

        case 2:
            if(work->time > work->maxTime) {
                mbPlayerRotYSet(playerNo, 0);
                GwPlayer[playerNo].moveF = FALSE;
                work->mode = 0;
            }
            weight = (float)(work->time++) / work->maxTime;
            rotY = mbAngleEaseOut(work->rotYStart, 0, weight);
            mbPlayerRotYSet(playerNo, rotY);
            break;
    }
    mbMasuMtxGet(work->masuId, masuMtx);
    MTXMultVec(masuMtx, &obj->trans, &pos);
    mbPlayerPosSetV(playerNo, &pos);
}

void mbev_PlayerColSet(int playerNo, int masuId)
{
    int i;
    int cornerNo;
    HuVecF pos;
    if(playerColSnapF) {
        for(i = 0; i < GW_PLAYER_MAX; i++) {
            if(GwPlayer[i].moveF) {
                continue;
            }
            if(masuId != GwPlayer[i].masuId) {
                continue;
            }
            cornerNo = mbPlayerMasuCornerGet(i);
            if(cornerNo == 0) {
                mbMasuPosGet(masuId, &pos);
            } else {
                mbMasuCornerRotPosGet(masuId, cornerNo - 1, &pos);
            }
            mbPlayerPosSetV(i, &pos);
            PlayerColCornerSnap(i, masuId, cornerNo);
        }
    }
}

void mbPlayerColSnapSet(BOOL snapF)
{
    int i;
    BOOL flag = (snapF) ? TRUE : FALSE;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(playerWork[i].unk24) {
            PLAYERCOLWORK *work = PLAYER_COLWORK(i);
            work->snapF = flag;
        }
    }
    playerColSnapF = snapF;
}

void mbPlayerColSnapPlayerSet(int playerNo, BOOL snapF)
{
    BOOL flag = (snapF) ? TRUE : FALSE;
    if(playerWork[playerNo].unk24) {
        PLAYERCOLWORK *work = PLAYER_COLWORK(playerNo);
        work->snapF = flag;
    }
}

int mbPlayerColSnapGet(int playerNo)
{
    PLAYERCOLWORK *work = PLAYER_COLWORK(playerNo);
    return work->snapF;
}

void mbPlayerColRestSet(int playerNo, BOOL restF)
{
    BOOL flag = (restF) ? FALSE : TRUE;
    if(playerWork[playerNo].unk24) {
        PLAYERCOLWORK *work = PLAYER_COLWORK(playerNo);
        work->noRestF = flag;
    }
}

void mbPlayerColFirstSet(int playerNo)
{
    int i;
    int orderNo = 1;
    GwPlayer[playerNo].orderNo = 0;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(playerNo != i) {
            GwPlayer[i].orderNo = orderNo++;
        }
    }
}

void mbPlayerColOrderReset(void)
{
    s8 playerNo[GW_PLAYER_MAX];
    s8 orderNo[GW_PLAYER_MAX];
    s8 fixF[GW_PLAYER_MAX];
    int i, j, k;
    int num;
    s16 masuId;

    memset(fixF, 0, GW_PLAYER_MAX);
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        mbPlayerColRestSet(i, TRUE);
        if(GwPlayer[i].masuId == 0) {
            continue;
        }
        if(fixF[i]) {
            continue;
        }
        masuId = GwPlayer[i].masuId;
        playerNo[0] = i;
        orderNo[0] = GwPlayer[i].orderNo;
        num = 1;
        for(j = 0; j < GW_PLAYER_MAX; j++) {
            if(i != j && masuId == GwPlayer[j].masuId) {
                playerNo[num] = j;
                orderNo[num] = GwPlayer[j].orderNo;
                num++;
            }
        }
        if(num > 1) {
            for(j = 0; j < num - 1; j++) {
                for(k = j + 1; k < num; k++) {
                    if(orderNo[j] > orderNo[k]) {
                        s8 temp = orderNo[j];
                        orderNo[j] = orderNo[k];
                        orderNo[k] = temp;
                    }
                }
            }
        }
        for(j = 0; j < num; j++) {
            GwPlayer[playerNo[j]].orderNo = orderNo[mbPlayerMasuCornerGet(playerNo[j])];
            fixF[playerNo[j]] = TRUE;
        }
    }
}

static char *eyeMatNameTbl[14][2] = {
    "eye1", "eye2",
    "eye1", "eye2",
    "mat14", "mat16",
    "eye1", "eye2",
    "Clswario_eye_l1_AUTO14", "Clswario_eye_l1_AUTO15",
    "m_donkey_eye4", "m_donkey_eye5",
    "mat65", "mat66",
    "Clswaluigi_eye_l1_AUTO1", "Clswaluigi_eye_l1_AUTO2"
};

void mbPlayerEyeMatDarkSet(int playerNo, BOOL darkF)
{
    BOOL validF;
    HU3D_MODELID modelId = mbObjModelIDGet(mbPlayerObjIDGet(playerNo));
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    HSF_DATA *hsf = modelP->hsf;
    HSF_MATERIAL *matP = hsf->material;
    HSF_MATERIAL *matCopy = playerWork[playerNo].unk4C;
    if(darkF) {
        char **name = &eyeMatNameTbl[GwPlayer[playerNo].charNo][0];
        int i, j;
        for(i = 0; i < hsf->materialNum; i++, matP++, matCopy++) {
            validF = TRUE;
            for(j = 0; j < matP->attrNum; j++) {
                HSF_ATTRIBUTE *attrP = &hsf->attribute[matP->attr[j]];
                if(strcmp(name[0], attrP->bitmap->name) == 0 || strcmp(name[1], attrP->bitmap->name) == 0) {
                    validF = FALSE;
                }
            }
            if(validF) {
                if(darkF) {
                    matP->color[0] *= 0.0f;
                    matP->color[1] *= 0.0f;
                    matP->color[2] *= 0.0f;
                } else {
                    matP->color[0] = matCopy->color[0];
                    matP->color[1] = matCopy->color[1];
                    matP->color[2] = matCopy->color[2];
                }
            }
        }
    } else {
        memcpy(hsf->material, matCopy, hsf->materialNum * sizeof(HSF_MATERIAL));
    }
    DCStoreRange(hsf->material, hsf->materialNum * sizeof(HSF_MATERIAL));
}

static inline void *PlayerMalloc(s32 size)
{
    return HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
}

void mbPlayerMatClone(int playerNo)
{
    HU3D_MODELID modelId = mbObjModelIDGet(mbPlayerObjIDGet(playerNo));
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    HSF_DATA *hsf = modelP->hsf;
    HSF_MATERIAL *matP = PlayerMalloc(hsf->materialNum * sizeof(HSF_MATERIAL));
    memcpy(matP, hsf->material, hsf->materialNum * sizeof(HSF_MATERIAL));
    playerWork[playerNo].unk4C = matP;
}

static void PlayerMetalKill(int playerNo);
static void MetalEffectCreate(OMOBJ *obj);
static void MetalEffectHook(HU3D_MODEL *modelP, PLAYERPARTICLE *particleP, Mtx mtx);
static void PlayerBiriQEffectSet(int playerNo, BOOL effectF);
static void PlayerBiriQFlashSet(int playerNo);
static void PlayerBiriQKill(int playerNo);
static void BiriQEffectCreate(OMOBJ *obj);
static void BiriQEffect1Hook(HU3D_MODEL *modelP, PLAYERPARTICLE *particleP, Mtx mtx);
static void BiriQEffect2Hook(HU3D_MODEL *modelP, PLAYERPARTICLE *particleP, Mtx mtx);

static inline void PlayerFree(void *ptr)
{
    HuMemDirectFree(ptr);
}

static void PlayerMetalOMExec(OMOBJ *obj)
{
    PLAYERMETALWORK *work = omObjGetWork(obj, PLAYERMETALWORK);
    BOOL killF = FALSE;
    float weight;
    if(mbExitCheck() || work->killF) {
        if(work->killF) {
            killF = TRUE;
        }
        work->killF = TRUE;
    }
    if(!work->killF) {
        if(!GwPlayer[work->playerNo].metalF && !work->fadeOutF) {
            work->fadeOutF = TRUE;
            work->time = 0;
            work->maxTime = 20;
        }
        if(work->fadeOutF) {
            work->time++;
            weight = (float)work->time / work->maxTime;
            if(weight > 1.0f) {
                weight = 1.0f;
            }
            mbObjMetalTPLvlSet(mbPlayerObjIDGet(work->playerNo), 1.0f - weight);
            if(work->time >= work->maxTime) {
                work->killF = TRUE;
            }
        } else if(!work->fadeInDoneF) {
            work->time++;
            weight = (float)work->time / work->maxTime;
            if(weight > 1.0f) {
                weight = 1.0f;
            }
            mbObjMetalTPLvlSet(mbPlayerObjIDGet(work->playerNo), weight);
            if(work->time >= work->maxTime) {
                work->fadeInDoneF = TRUE;
            }
        }
        if(obj->mdlId[0] >= 0) {
            if(!mbObjGet(mbPlayerObjIDGet(work->playerNo))->disp || !work->effectF) {
                Hu3DModelAttrSet(obj->mdlId[0], HU3D_ATTR_DISPOFF);
            } else {
                Hu3DModelAttrReset(obj->mdlId[0], HU3D_ATTR_DISPOFF);
            }
        }
    }
    if(work->killF) {
        if(!killF) {
            PlayerMetalKill(work->playerNo);
        }
        if(obj->data) {
            PlayerFree(obj->data);
        }
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
}

void mbPlayerMetalSet(int playerNo, BOOL metalF)
{
    OMOBJ *obj;
    PLAYERMETALWORK *work;
    if(metalF) {
        if(playerWork[playerNo].unk28 != NULL) {
            OSReport("------------already METAL!!----------");
        }
        GwPlayer[playerNo].metalF = TRUE;
        obj = playerWork[playerNo].unk28;
        if(obj == NULL) {
            obj = playerWork[playerNo].unk28 = omAddObjEx(mbObjMan, 256, 1, 0, -1, PlayerMetalOMExec);
            omSetStatBit(obj, 0x100);
            mbObjMetalCreate(mbPlayerObjIDGet(playerNo));
            mbObjMetalTPLvlSet(mbPlayerObjIDGet(playerNo), 0);
            mbObjMetalColorSet(mbPlayerObjIDGet(playerNo), metalShadowColor, metalHiliteColor);
        }
        work = omObjGetWork(obj, PLAYERMETALWORK);
        work->playerNo = playerNo;
        work->fadeOutF = FALSE;
        work->fadeInDoneF = FALSE;
        work->effectF = TRUE;
        work->time = 0;
        work->maxTime = 20;
        if(obj->mdlId[0] <= 0) {
            MetalEffectCreate(obj);
        }
        CharModelStepSet(GwPlayer[playerNo].charNo, 6);
    } else {
        GwPlayer[playerNo].metalF = FALSE;
        CharModelStepSet(GwPlayer[playerNo].charNo, 0);
    }
}

static void PlayerMetalKill(int playerNo)
{
    OMOBJ *obj = playerWork[playerNo].unk28;
    if(obj != NULL) {
        PLAYERMETALWORK *work = omObjGetWork(obj, PLAYERMETALWORK);
        work->killF = TRUE;
        playerWork[playerNo].unk28 = NULL;
        mbObjMetalKill(mbPlayerObjIDGet(playerNo));
        mbParticleKill(obj->mdlId[0]);
        obj->mdlId[0] = -1;
    }
}

void mbPlayerEffectSet(int playerNo, BOOL effectF)
{
    OMOBJ *obj = playerWork[playerNo].unk28;
    if(obj) {
        PLAYERMETALWORK *work = omObjGetWork(obj, PLAYERMETALWORK);
        work->effectF = effectF;
    }
    PlayerBiriQEffectSet(playerNo, effectF);
}

static void ResetMetalColor(void)
{
    metalShadowColor = metalDefaultColor[0];
    metalHiliteColor = metalDefaultColor[1];
}

void mbPlayerMetalColorSet(GXColor *shadow, GXColor *hilite)
{
    metalShadowColor = *shadow;
    metalHiliteColor = *hilite;
}

#ifdef __MWERKS__
/* Paired-single element-wise HuVecF multiply. */
static inline void PlayerVecMul(register HuVecF *a, register HuVecF *b, register HuVecF *dst)
{
    register float axy;
    register float bxy;
    register float az;
    register float bz;
    asm {
        psq_l axy, 0(a), 0, 0
        psq_l bxy, 0(b), 0, 0
        psq_l az, 8(a), 1, 0
        psq_l bz, 8(b), 1, 0
        ps_mul axy, axy, bxy
        ps_mul az, az, bz
        psq_st axy, 0(dst), 0, 0
        psq_st az, 8(dst), 1, 0
    }
}
#endif

static float GetBiriQEffectRadius(OMOBJ *obj, int playerNo, int *out)
{
    int i;
    HSF_OBJECT *meshP;
    s16 *cntTbl;
    int cnt;
    int x, y, z;
    HSF_DATA *hsf;
    int vtxNum;
    int no;
    s16 *idxTbl;
    int meshNo;
    int unused;
    int vtxMax;
    HSF_OBJECT *objP;
    HuVecF *vtxP;
    int rand;
    s16 *buf;
    HuVecF scale;
    HuVecF ofs;
    HuVecF pos;
    float radius;

    meshNo = meshNo = -1;
    vtxMax = unused = 0;
    hsf = Hu3DData[mbPlayerModelIDGet(playerNo)].hsf;
    objP = hsf->object;
    for(i = 0; i < hsf->objectNum; i++, objP++) {
        meshP = objP;
        if(meshP->type == 2 && meshP->mesh.vertex->count > 0) {
            vtxNum = meshP->mesh.vertex->count;
            if(vtxMax < vtxNum) {
                vtxMax = vtxNum;
                meshNo = i;
            }
        }
    }
    vtxNum = vtxMax;
    out[0] = meshNo;
    obj->data = PlayerMalloc(2500);
    buf = obj->data;
    cntTbl = buf + 125;
    idxTbl = cntTbl + 125;
    memset(cntTbl, 0, 250);
    meshP = &hsf->object[out[0]];
    VECScale(&meshP->mesh.mesh.min, &ofs, -1.0f);
    VECSubtract(&meshP->mesh.mesh.max, &meshP->mesh.mesh.min, &scale);
    scale.x += 1.0f;
    scale.y += 1.0f;
    scale.z += 1.0f;
    if(scale.x > 0.0f) {
        scale.x = 5.0f / scale.x;
    }
    if(scale.y > 0.0f) {
        scale.y = 5.0f / scale.y;
    }
    if(scale.z > 0.0f) {
        scale.z = 5.0f / scale.z;
    }
    vtxNum = meshP->mesh.vertex->count;
    vtxP = meshP->mesh.vertex->data;
    for(i = 0; i < vtxNum; i++, vtxP++) {
        VECAdd(vtxP, &ofs, &pos);
        PlayerVecMul(&pos, &scale, &pos);
        x = pos.x;
        if(x < 0) {
            x += 5;
        }
        if(x >= 5) {
            x -= 5;
        }
        y = pos.y;
        if(y < 0) {
            y += 5;
        }
        if(y >= 5) {
            y -= 5;
        }
        z = pos.z;
        if(z < 0) {
            z += 5;
        }
        if(z >= 5) {
            z -= 5;
        }
        no = x + y * 5 + z * 25; //IDX
        cnt = cntTbl[no];
        if(cnt < 8) {
            idxTbl[(no * 8) + cnt] = i;
        } else {
            rand = mbRandMod(cnt + 1);
            if(rand < 8) {
                idxTbl[(no * 8) + rand] = i;
            }
        }
        cntTbl[no]++;
    }
    for(i = 0, cnt = 0; i < 125; i++) {
        if(cntTbl[i] > 0) {
            buf[cnt++] = i;
            if(cntTbl[i] > 8) {
                cntTbl[i] = 8;
            }
        }
    }
    out[1] = cnt;
    radius = meshP->mesh.mesh.max.y - meshP->mesh.mesh.min.y;
    return radius;
}

static inline PLAYERPARTICLE *PlayerParticleGet(s16 modelId)
{
    return Hu3DData[modelId].hookData;
}

static void MetalEffectCreate(OMOBJ *obj)
{
    int i;
    HSF_OBJECT *meshP;
    PLAYERMETALWORK *work = omObjGetWork(obj, PLAYERMETALWORK);
    s16 *cntTbl;
    int cnt;
    int x, y, z;
    HSF_DATA *hsf;
    int vtxNum;
    PLAYERPARTICLE *particleP;
    int meshNo;
    int unused;
    int vtxMax;
    HSF_OBJECT *objP;
    HuVecF *vtxP;
    int num;
    int rand;
    int no;
    s16 *buf;
    s16 *idxTbl;
    HuVecF scale;
    HuVecF ofs;
    HuVecF pos;

    num = 5;
    obj->mdlId[0] = mbParticleCreate(HuSprAnimRead(HuDataReadNum(mbBoardDataNumGet(0x5006A), HU_MEMNUM_OVL)), num);
    mbParticleHookSet(obj->mdlId[0], MetalEffectHook);
    Hu3DModelLayerSet(obj->mdlId[0], 5);
    meshNo = meshNo = -1;
    vtxMax = unused = 0;
    hsf = Hu3DData[mbPlayerModelIDGet(work->playerNo)].hsf;
    objP = hsf->object;
    for(i = 0; i < hsf->objectNum; i++, objP++) {
        meshP = objP;
        if(meshP->type == 2 && meshP->mesh.vertex->count > 0) {
            vtxNum = meshP->mesh.vertex->count;
            if(vtxMax < vtxNum) {
                vtxMax = vtxNum;
                meshNo = i;
            }
        }
    }
    vtxNum = vtxMax;
    work->meshNo = meshNo;
    particleP = PlayerParticleGet(obj->mdlId[0]);
    particleP->work = obj;
    for(i = 0; i < num; i++) {
        particleP->data[i].vtxNo = 0;
    }
    obj->data = PlayerMalloc(2500);
    buf = obj->data;
    cntTbl = buf + 125;
    idxTbl = cntTbl + 125;
    memset(cntTbl, 0, 250);
    meshP = &hsf->object[work->meshNo];
    VECScale(&meshP->mesh.mesh.min, &ofs, -1.0f);
    VECSubtract(&meshP->mesh.mesh.max, &meshP->mesh.mesh.min, &scale);
    scale.x += 1.0f;
    scale.y += 1.0f;
    scale.z += 1.0f;
    if(scale.x > 0.0f) {
        scale.x = 5.0f / scale.x;
    }
    if(scale.y > 0.0f) {
        scale.y = 5.0f / scale.y;
    }
    if(scale.z > 0.0f) {
        scale.z = 5.0f / scale.z;
    }
    vtxNum = meshP->mesh.vertex->count;
    vtxP = meshP->mesh.vertex->data;
    for(i = 0; i < vtxNum; i++, vtxP++) {
        VECAdd(vtxP, &ofs, &pos);
        PlayerVecMul(&pos, &scale, &pos);
        x = pos.x;
        if(x < 0) {
            x += 5;
        }
        if(x >= 5) {
            x -= 5;
        }
        y = pos.y;
        if(y < 0) {
            y += 5;
        }
        if(y >= 5) {
            y -= 5;
        }
        z = pos.z;
        if(z < 0) {
            z += 5;
        }
        if(z >= 5) {
            z -= 5;
        }
        no = x + y * 5 + z * 25;
        cnt = cntTbl[no];
        if(cnt < 8) {
            idxTbl[(no * 8) + cnt] = i;
        } else {
            rand = mbRandMod(cnt + 1);
            if(rand < 8) {
                idxTbl[(no * 8) + rand] = i;
            }
        }
        cntTbl[no]++;
    }
    for(i = 0, cnt = 0; i < 125; i++) {
        if(cntTbl[i] > 0) {
            buf[cnt++] = i;
            if(cntTbl[i] > 8) {
                cntTbl[i] = 8;
            }
        }
    }
    work->cellNum = cnt;
}

static void MetalEffectHook(HU3D_MODEL *modelP, PLAYERPARTICLE *particleP, Mtx mtx)
{
    PLAYERPARTICLEDATA *dataP;
    int i;
    PLAYERMETALWORK *work;
    HSF_OBJECT *meshP = NULL;
    int no;
    OMOBJ *obj;
    s16 *buf;
    s16 *cntTbl;
    HSF_DATA *hsf;
    s16 *idxTbl;
    BOOL dispF;
    BOOL initF;
    int rand;
    int unused;
    float weight;
    Mtx objMtx;
    HuVecF dir;
    HuVecF pos;
    HuVecF camPos;

    obj = particleP->work;
    work = omObjGetWork(obj, PLAYERMETALWORK);
    unused = -1;
    initF = FALSE;

    if(!particleP->initF) {
        dataP = particleP->data;
        for(i = 0; i < particleP->num; i++, dataP++) {
            dataP->scale = 0;
            dataP->color.a = 0;
            dataP->time = 0;
        }
        particleP->initF = TRUE;
        particleP->unk34 = 10;
        particleP->unk38 = 2;
        particleP->unk3C = 8;
        particleP->unk40 = 15;
        particleP->unk54.r = particleP->unk54.g = particleP->unk54.b = particleP->unk54.a = 255;
        particleP->unk20 = 1;
        initF = TRUE;
    }
    MTXInverse(Hu3DCameraMtx, objMtx);
    camPos.x = objMtx[0][3];
    camPos.y = objMtx[1][3];
    camPos.z = objMtx[2][3];
    hsf = Hu3DData[mbPlayerModelIDGet(work->playerNo)].hsf;
    meshP = &hsf->object[work->meshNo];
    Hu3DModelObjMtxGet(mbPlayerModelIDGet(work->playerNo), meshP->name, objMtx);
    dataP = particleP->data;
    for(i = 0; i < particleP->num; i++, dataP++) {
        if(dataP->time == 0) {
            if(work->fadeOutF) {
                continue;
            }
            dataP->time++;
            buf = obj->data;
            cntTbl = buf + 125;
            idxTbl = cntTbl + 125;
            no = buf[mbRandMod(work->cellNum)];
            rand = mbRandMod(cntTbl[no]);
            no = idxTbl[(no * 8) + rand];
            dataP->vtxNo = no;
            dataP->color.r = mbRandMod(120) + 120;
            dataP->color.g = mbRandMod(120) + 120;
            dataP->color.b = mbRandMod(120) + 120;
            dataP->color.a = mbRandMod(48) + 100;
            dataP->scaleMax = 30.0f + (60.0f * frandf());
            dataP->scale = 0;
            dataP->rotSpeed = (6.0f * frandf()) - 3.0f;
            dataP->zRot = 360.0f * frandf();
            dataP->maxTime = mbRandMod(30) + 30;
            dataP->animNo = mbRandMod(2);
            dataP->dispF = TRUE;
            if(initF) {
                dataP->scaleMax = 60.0f + (80.0f * frandf());
                dataP->maxTime = mbRandMod(40) + 40;
            }
        }
        dispF = TRUE;
        dataP->time++;
        weight = (float)dataP->time / dataP->maxTime;
        dataP->scale = dataP->scaleMax * mbSinDeg(180.0f * weight);
        dataP->zRot += dataP->rotSpeed;
        if(weight > 0.8f) {
            dataP->color.a *= 0.9f;
        }
        if(weight > 0.6f) {
            dispF = FALSE;
        }
        if(dataP->time >= dataP->maxTime) {
            dataP->time = 0;
            dataP->color.a = 0;
            dataP->scale = 0;
            dataP->dispF = FALSE;
        }
        if(dispF) {
            MTXMultVec(objMtx, &((HuVecF *)meshP->mesh.vertex->data)[dataP->vtxNo], &pos);
            VECSubtract(&camPos, &pos, &dir);
            weight = VECMag(&dir);
            if(weight > 0.0f) {
                VECScale(&dir, &dir, 200.0f / weight);
            }
            VECAdd(&pos, &dir, &dataP->pos);
        }
    }
}

static void PlayerBiriQOMExec(OMOBJ *obj)
{
    PLAYERBIRIQWORK *work = omObjGetWork(obj, PLAYERBIRIQWORK);
    BOOL killF = FALSE;
    BOOL dispF = TRUE;
    if(mbExitCheck() || work->killF) {
        if(work->killF) {
            killF = TRUE;
        }
        work->killF = TRUE;
    }
    if(!work->killF) {
        if(!GwPlayer[work->playerNo].biriQF && !work->fadeOutF) {
            work->fadeOutF = TRUE;
            work->time = 0;
            work->maxTime = 20;
        }
        if(work->fadeOutF) {
            work->killF = TRUE;
        } else if(work->flashF) {
            work->time = 0;
            work->maxTime = 24;
            work->flashF = FALSE;
            work->flashDoneF = FALSE;
        } else if(!work->flashDoneF) {
            GXColor color = { 255, 255, 255, 255 };
            int modeTbl[4] = { 1, 3, 2, 3 };
            float alpha = 1.0f;
            int mode = modeTbl[(work->time >> 1) & 3];
            if(mode == 0) {
                alpha = 0.0f;
            }
            mbObjBiriQColorSet(mbPlayerObjIDGet(work->playerNo), mode, alpha, color);
            if(work->time >= work->maxTime) {
                work->flashDoneF = TRUE;
                mbObjBiriQColorSet(mbPlayerObjIDGet(work->playerNo), 0, 0.0f, color);
                if(obj->mdlId[0] < 0) {
                    BiriQEffectCreate(obj);
                }
            }
            work->time++;
            dispF = FALSE;
        } else {
            GXColor color = { 255, 255, 255, 255 };
            int modeTbl[4] = { 1, 3, 2, 3 };
            float alpha = 0.1f;
            int mode = modeTbl[(work->time >> 1) & 3];
            if(mode == 0) {
                alpha = 0.0f;
            }
            mbObjBiriQColorSet(mbPlayerObjIDGet(work->playerNo), mode, alpha, color);
            work->time++;
        }
        if(obj->mdlId[0] >= 0) {
            if(!mbObjGet(mbPlayerObjIDGet(work->playerNo))->disp || !work->effectF || !dispF) {
                Hu3DModelAttrSet(obj->mdlId[0], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrSet(obj->mdlId[1], HU3D_ATTR_DISPOFF);
            } else {
                Hu3DModelAttrReset(obj->mdlId[0], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrReset(obj->mdlId[1], HU3D_ATTR_DISPOFF);
            }
        }
    }
    if(work->killF) {
        if(!killF) {
            PlayerBiriQKill(work->playerNo);
        }
        if(obj->data) {
            PlayerFree(obj->data);
        }
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
}

void mbPlayerBiriQSet(int playerNo, BOOL biriQF)
{
    OMOBJ *obj;
    PLAYERBIRIQWORK *work;
    if(biriQF) {
        if(playerWork[playerNo].unk2C != NULL) {
            OSReport("------------already BiriQ!!----------");
        }
        GwPlayer[playerNo].biriQF = TRUE;
        obj = playerWork[playerNo].unk2C;
        if(obj == NULL) {
            GXColor color = { 255, 255, 255, 255 };
            obj = playerWork[playerNo].unk2C = omAddObjEx(mbObjMan, 256, 2, 0, -1, PlayerBiriQOMExec);
            omSetStatBit(obj, 0x100);
            obj->mdlId[0] = obj->mdlId[1] = -1;
            mbObjBiriQCreate(mbPlayerObjIDGet(playerNo));
            mbObjBiriQColorSet(mbPlayerObjIDGet(playerNo), 1, 0.0f, color);
        }
        work = omObjGetWork(obj, PLAYERBIRIQWORK);
        work->playerNo = playerNo;
        work->fadeOutF = FALSE;
        work->flashDoneF = FALSE;
        work->time = 0;
        work->maxTime = 20;
        work->effectF = TRUE;
        PlayerBiriQFlashSet(playerNo);
    } else {
        GwPlayer[playerNo].biriQF = FALSE;
    }
}

static void PlayerBiriQFlashSet(int playerNo)
{
    OMOBJ *obj = playerWork[playerNo].unk2C;
    if(obj != NULL) {
        PLAYERBIRIQWORK *work = omObjGetWork(obj, PLAYERBIRIQWORK);
        work->flashF = TRUE;
    }
}

static void PlayerBiriQKill(int playerNo)
{
    OMOBJ *obj = playerWork[playerNo].unk2C;
    if(obj != NULL) {
        PLAYERBIRIQWORK *work = omObjGetWork(obj, PLAYERBIRIQWORK);
        work->killF = TRUE;
        playerWork[playerNo].unk2C = NULL;
        mbObjBiriQKill(mbPlayerObjIDGet(playerNo));
        if(obj->mdlId[0] >= 0) {
            mbParticleKill(obj->mdlId[0]);
            obj->mdlId[0] = -1;
        }
        if(obj->mdlId[1] >= 0) {
            mbParticleKill(obj->mdlId[1]);
            obj->mdlId[1] = -1;
        }
    }
}

static void PlayerBiriQEffectSet(int playerNo, BOOL effectF)
{
    OMOBJ *obj = playerWork[playerNo].unk2C;
    if(obj) {
        PLAYERBIRIQWORK *work = omObjGetWork(obj, PLAYERBIRIQWORK);
        work->effectF = effectF;
    }
}

static void BiriQEffectCreate(OMOBJ *obj)
{
    PLAYERBIRIQWORK *work = omObjGetWork(obj, PLAYERBIRIQWORK);
    int num;
    PLAYERPARTICLE *particleP;
    int out[2];
    float radius = GetBiriQEffectRadius(obj, work->playerNo, out);
    work->meshNo = out[0];
    work->cellNum = out[1];
    num = 21.0f * ((1.0f / 150.0f) * radius);
    obj->mdlId[0] = mbParticleCreate(HuSprAnimRead(HuDataReadNum(mbBoardDataNumGet(0x5006B), HU_MEMNUM_OVL)), num);
    obj->mdlId[1] = mbParticleCreate(HuSprAnimRead(HuDataReadNum(mbBoardDataNumGet(0x5006C), HU_MEMNUM_OVL)), num);
    mbParticleHookSet(obj->mdlId[0], BiriQEffect1Hook);
    mbParticleHookSet(obj->mdlId[1], BiriQEffect2Hook);
    Hu3DModelLayerSet(obj->mdlId[0], 3);
    Hu3DModelLayerSet(obj->mdlId[1], 3);
    particleP = PlayerParticleGet(obj->mdlId[0]);
    particleP->work = obj;
    particleP->endF = 0;
    particleP = PlayerParticleGet(obj->mdlId[1]);
    particleP->work = PlayerParticleGet(obj->mdlId[0]);
    particleP->endF = 0;
}

static void BiriQEffect1Hook(HU3D_MODEL *modelP, PLAYERPARTICLE *particleP, Mtx mtx)
{
    PLAYERPARTICLEDATA *dataP;
    int i;
    PLAYERBIRIQWORK *work;
    HSF_OBJECT *meshP = NULL;
    int no;
    OMOBJ *obj;
    s16 *buf;
    s16 *cntTbl;
    HSF_DATA *hsf;
    s16 *idxTbl;
    BOOL dispF;
    int rand;
    BOOL initF;
    int unused;
    float weight;
    Mtx objMtx;
    HuVecF dir;
    HuVecF pos;
    HuVecF camPos;

    obj = particleP->work;
    work = omObjGetWork(obj, PLAYERBIRIQWORK);
    unused = -1;
    initF = FALSE;

    if(particleP->endF == 0) {
        dataP = particleP->data;
        for(i = 0; i < particleP->num; i++, dataP++) {
            dataP->scale = 0;
            dataP->color.a = 0;
            dataP->time = 0;
        }
        particleP->endF = 1;
        initF = TRUE;
    }
    MTXInverse(Hu3DCameraMtx, objMtx);
    camPos.x = objMtx[0][3];
    camPos.y = objMtx[1][3];
    camPos.z = objMtx[2][3];
    hsf = Hu3DData[mbPlayerModelIDGet(work->playerNo)].hsf;
    meshP = &hsf->object[work->meshNo];
    Hu3DModelObjMtxGet(mbPlayerModelIDGet(work->playerNo), meshP->name, objMtx);
    dataP = particleP->data;
    for(i = 0; i < particleP->num; i++, dataP++) {
        if(dataP->time == 0) {
            if(work->fadeOutF) {
                continue;
            }
            dataP->time++;
            buf = obj->data;
            cntTbl = buf + 125;
            idxTbl = cntTbl + 125;
            no = buf[mbRandMod(work->cellNum)];
            rand = mbRandMod(cntTbl[no]);
            no = idxTbl[(no * 8) + rand];
            dataP->vtxNo = no;
            dataP->color.r = mbRandMod(40) + 30;
            dataP->color.g = mbRandMod(40) + 90;
            dataP->color.b = mbRandMod(40) + 180;
            dataP->color.a = mbRandMod(58) + 130;
            dataP->scaleMax = 30.0f + (60.0f * frandf());
            dataP->scale = 0;
            dataP->zRot = 360.0f * frandf();
            dataP->maxTime = mbRandMod(12) + 12;
            dataP->animNo = mbRandMod(4);
            dataP->dispF = TRUE;
        }
        dispF = TRUE;
        dataP->time++;
        weight = (float)dataP->time / dataP->maxTime;
        dataP->scale = dataP->scaleMax * (0.1f + (0.9f * mbSinDeg(180.0f * weight)));
        if(dataP->time & 1) {
            dataP->animNo = mbRandMod(4);
            if(mbRandMod(100) < 25) {
                dataP->zRot += 180.0f;
            }
        }
        if(weight > 0.8f) {
            dataP->color.a *= 0.9f;
        }
        if(weight > 0.6f) {
            dispF = FALSE;
        }
        if(dataP->time >= dataP->maxTime) {
            dataP->time = 0;
            dataP->color.a = 0;
            dataP->scale = 0;
            dataP->dispF = FALSE;
        }
        if(dispF) {
            MTXMultVec(objMtx, &((HuVecF *)meshP->mesh.vertex->data)[dataP->vtxNo], &pos);
            VECSubtract(&camPos, &pos, &dir);
            weight = VECMag(&dir);
            if(weight > 0.0f) {
                VECScale(&dir, &dir, 18.0f / weight);
            }
            VECAdd(&pos, &dir, &dataP->pos);
        }
    }
}

static void BiriQEffect2Hook(HU3D_MODEL *modelP, PLAYERPARTICLE *particleP, Mtx mtx)
{
    PLAYERPARTICLEDATA *dataP;
    int i;
    int alpha;
    PLAYERPARTICLE *srcP;
    if(particleP->endF == 0) {
        dataP = particleP->data;
        for(i = 0; i < particleP->num; i++, dataP++) {
            dataP->scale = 0;
            dataP->color.a = 0;
            dataP->time = 0;
        }
        particleP->endF = 1;
        particleP->unk20 = 1;
    }
    srcP = particleP->work;
    memcpy(particleP->data, srcP->data, particleP->num * sizeof(PLAYERPARTICLEDATA));
    dataP = particleP->data;
    for(i = 0; i < particleP->num; i++, dataP++) {
        dataP->color.r = 255;
        dataP->color.g = 255;
        dataP->color.b = 255;
        alpha = 1.2f * dataP->color.a;
        if(alpha > 255) {
            alpha = 255;
        }
        dataP->color.a = alpha;
    }
}

void mbPlayerSwap(int playerNo1, int playerNo2)
{
    OMOBJ *colObj1 = mbPlayerWorkGet(playerNo1)->unk24;
    OMOBJ *colObj2 = mbPlayerWorkGet(playerNo2)->unk24;
    GW_PLAYER_CONF tempConf;
    MBPLAYERWORK tempWork;
    GW_PLAYER tempPlayer;
    tempConf = GwPlayerConf[playerNo1];
    GwPlayerConf[playerNo1] = GwPlayerConf[playerNo2];
    GwPlayerConf[playerNo2] = tempConf;
    tempPlayer = GwPlayer[playerNo1];
    GwPlayer[playerNo1] = GwPlayer[playerNo2];
    GwPlayer[playerNo2] = tempPlayer;
    memcpy(&tempWork, mbPlayerWorkGet(playerNo1), sizeof(MBPLAYERWORK));
    memcpy(mbPlayerWorkGet(playerNo1), mbPlayerWorkGet(playerNo2), sizeof(MBPLAYERWORK));
    memcpy(mbPlayerWorkGet(playerNo2), &tempWork, sizeof(MBPLAYERWORK));
    mbPlayerWorkGet(playerNo1)->unk24 = colObj1;
    mbPlayerWorkGet(playerNo2)->unk24 = colObj2;
    GwPlayer[playerNo1].padNo = GwPlayerConf[playerNo1].padNo;
    GwPlayerConf[playerNo1].padNo = GwPlayerConf[playerNo1].padNo;
    GwPlayer[playerNo2].padNo = GwPlayerConf[playerNo2].padNo;
    GwPlayerConf[playerNo2].padNo = GwPlayerConf[playerNo2].padNo;
}

/* ---- part 3 (fork b203) ---- */


u32 mbPlayerNameMesGet(int playerNo)
{
    u32 nameTbl[14] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13
    };
    return nameTbl[GwPlayer[playerNo].charNo];
}

char *mbPlayerNameGet(int playerNo)
{
    char *nameTbl[14] = {
        "Mario",
        "Luigi",
        "Peach",
        "Yoshi",
        "Wario",
        "Daisy",
        "Waluigi",
        "Kinopio",
        "Teresa",
        "Mini Koopa",
        "Kinopiko",
        NULL,
        NULL,
        NULL
    };
    return nameTbl[GwPlayer[playerNo].charNo];
}

static s8 tagIdTbl[10*11] = {
     -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
     -1, -1, 10, 11, 12, 13, 14, 15, 16, 17, 18,
     -1, -1, -1, 19, 20, 21, 22, 23, 24, 25, 26,
     -1, -1, -1, -1, 27, 28, 29, 30, 31, 32, 33,
     -1, -1, -1, -1, -1, 34, 35, 36, 37, 38, 39,
     -1, -1, -1, -1, -1, -1, 40, 41, 42, 43, 44,
     -1, -1, -1, -1, -1, -1, -1, 45, 46, 47, 48,
     -1, -1, -1, -1, -1, -1, -1, -1, 49, 50, 51,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, 52, 53,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 54,
};

int mbPlayerTagNameMesGet(int teamNo)
{
    int charNo1 = GwPlayer[mbPlayerTeamFindPlayer(teamNo, 0)].charNo;
    int charNo2 = GwPlayer[mbPlayerTeamFindPlayer(teamNo, 1)].charNo;
    int temp;
    int tagId;
    if(charNo1 > charNo2) {
        temp = charNo1;
        charNo1 = charNo2;
        charNo2 = temp;
    }
    tagId = tagIdTbl[charNo2 + charNo1 * 11];
    OSReport("%d:%d->%d\n", charNo1, charNo2, tagId);
    if(tagId == -1) {
        return 0x30037;
    } else {
        return tagId + 0x30000;
    }
}

void mbPlayerAmbSet(int playerNo, float ambR, float ambG, float ambB)
{
    mbObjAmbSet(mbPlayerObjIDGet(playerNo), ambR, ambG, ambB);
}

s16 mbPlayerObjIDGet(int playerNo)
{
    return playerWork[playerNo].objId;
}

s16 mbPlayerModelIDGet(int playerNo)
{
    return mbObjModelIDGet(playerWork[playerNo].objId);
}

BOOL mbPlayerAllComCheck(void)
{
    int i;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(!GwPlayer[i].comF) {
            return FALSE;
        }
    }
    return TRUE;
}

int mbPlayerTeamFind(int playerNo)
{
    int i;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(i == playerNo) {
            continue;
        }
        if(GWPlayerTeamGet(playerNo) == GWPlayerTeamGet(i)) {
            break;
        }
    }
    return i;
}

int mbPlayerTeamFindOther(int playerNo)
{
    int i;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(i == playerNo) {
            continue;
        }
        if(GWPlayerTeamGet(playerNo) != GWPlayerTeamGet(i)) {
            break;
        }
    }
    return i;
}

int mbPlayerTeamFindPlayer(int teamNo, int memberNo)
{
    int i;
    int no;
    no = 0;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(teamNo != GWPlayerTeamGet(i)) {
            continue;
        }
        if(no == memberNo) {
            return i;
        }
        no++;
    }
    return -1;
}

int mbPlayerTeamFindOpp(int playerNo)
{
    if(!GWTeamGet()) {
        return playerNo;
    }
    return mbPlayerTeamFind(playerNo);
}

int mbPlayerTeamCheckSame(int playerNo1, int playerNo2)
{
    BOOL result = FALSE;
    if(GWTeamGet()) {
        if(GWPlayerTeamGet(playerNo1) == GWPlayerTeamGet(playerNo2)) {
            result = TRUE;
        }
    } else if(playerNo1 == playerNo2) {
        result = TRUE;
    }
    return result;
}

int mbPlayerTeamGet(int playerNo)
{
    if(!GWTeamGet()) {
        return playerNo;
    }
    return GwPlayer[playerNo].team;
}

float mbPlayerWalkSpeedGet(void)
{
    return 20;
}

void mbPlayerLayerSet(int playerNo, int layer)
{
    mbObjLayerSet(mbPlayerObjIDGet(playerNo), layer);
}

void mbPlayerCameraSet(int playerNo, u16 bit)
{
    mbObjCameraSet(mbPlayerObjIDGet(playerNo), bit);
}

void mbPlayerCullRadiusSet(int playerNo, float radius)
{
    mbObjCullRadiusSet(mbPlayerObjIDGet(playerNo), radius);
}

void mbPlayerStubValSet(int playerNo, BOOL value)
{
}

void mbPlayerPosReset(int playerNo)
{
    HuVecF pos;
    mbMasuPosGet(GwPlayer[playerNo].masuId, &pos);
    mbPlayerPosSetV(playerNo, &pos);
    PlayerColCornerSnap(playerNo, GwPlayer[playerNo].masuId, 0);
}

void mbPlayerPosResetAll(void)
{
    int i, j;
    int cornerNo;
    s16 masuId;
    s8 orderNo;
    HuVecF pos;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        orderNo = GwPlayer[i].orderNo;
        masuId = GwPlayer[i].masuId;
        cornerNo = 0;
        for(j = 0; j < GW_PLAYER_MAX; j++) {
            if(i != j && masuId == GwPlayer[j].masuId && orderNo > GwPlayer[j].orderNo) {
                cornerNo++;
            }
        }
        mbPlayerMasuCornerSet(i, cornerNo);
        if(cornerNo == 0) {
            mbMasuPosGet(masuId, &pos);
        } else {
            mbMasuCornerRotPosGet(masuId, cornerNo - 1, &pos);
        }
        mbPlayerPosSetV(i, &pos);
        PlayerColCornerSnap(i, masuId, cornerNo);
    }
}

void mbPlayerMtxSet(int playerNo, Mtx *matrix)
{
    mbObjMtxSet(mbPlayerObjIDGet(playerNo), *matrix);
}

void mbPlayerMtxGet(int playerNo, void *matrix)
{
    mbObjMtxGet(mbPlayerObjIDGet(playerNo), matrix);
}

void mbPlayerPosSetV(int playerNo, HuVecF *pos)
{
    mbObjPosSetV(mbPlayerObjIDGet(playerNo), pos);
}

void mbPlayerPosSet(int playerNo, float posX, float posY, float posZ)
{
    mbObjPosSet(mbPlayerObjIDGet(playerNo), posX, posY, posZ);
}

void mbPlayerPosGet(int playerNo, HuVecF *pos)
{
    mbObjPosGet(mbPlayerObjIDGet(playerNo), pos);
}

void mbPlayerRotSetV(int playerNo, HuVecF *rot)
{
    mbObjRotSetV(mbPlayerObjIDGet(playerNo), rot);
}

void mbPlayerRotSet(int playerNo, float rotX, float rotY, float rotZ)
{
    mbObjRotSet(mbPlayerObjIDGet(playerNo), rotX, rotY, rotZ);
}

void mbPlayerRotGet(int playerNo, HuVecF *rot)
{
    mbObjRotGet(mbPlayerObjIDGet(playerNo), rot);
}

void mbPlayerRotYSet(int playerNo, float rotY)
{
    rotY = fmod(rotY, 360);
    if(rotY < 0) {
        rotY += 360;
    }
    mbObjRotYSet(mbPlayerObjIDGet(playerNo), rotY);
}

float mbPlayerRotYGet(int playerNo)
{
    return mbObjRotYGet(mbPlayerObjIDGet(playerNo));
}

void mbPlayerScaleSetV(int playerNo, HuVecF *scale)
{
    mbObjScaleSetV(mbPlayerObjIDGet(playerNo), scale);
}

void mbPlayerScaleSet(int playerNo, float scaleX, float scaleY, float scaleZ)
{
    mbObjScaleSet(mbPlayerObjIDGet(playerNo), scaleX, scaleY, scaleZ);
}

void mbPlayerScaleGet(int playerNo, HuVecF *scale)
{
    mbObjScaleGet(mbPlayerObjIDGet(playerNo), scale);
}

void mbPlayerMotionSet(int playerNo, int motNo, int attr)
{
    GW_PLAYER *playerP = GWPlayerGet(playerNo);
    if(motNo == playerWork[playerNo].motNo) {
        return;
    }
    mbObjMotionSet(mbPlayerObjIDGet(playerNo), motNo, attr);
    playerWork[playerNo].motNo = motNo;
    if(motNo == 10) {
        mbObjOffsetSet(mbPlayerObjIDGet(playerNo), 0, 4, 0);
    } else {
        mbObjOffsetSet(mbPlayerObjIDGet(playerNo), 0, 0, 0);
    }
}

s16 mbPlayerMotionGet(int playerNo)
{
    return playerWork[playerNo].motNo;
}

void mbPlayerMotionShiftSet(int playerNo, int motNo, float start, float end, int attr)
{
    GW_PLAYER *playerP = GWPlayerGet(playerNo);
    if(motNo == playerWork[playerNo].motNo) {
        return;
    }
    mbObjMotionShiftSet(mbPlayerObjIDGet(playerNo), motNo, start, end, attr);
    playerWork[playerNo].motNo = motNo;
    if(motNo == 10) {
        mbObjOffsetSet(mbPlayerObjIDGet(playerNo), 0, 4, 0);
    } else {
        mbObjOffsetSet(mbPlayerObjIDGet(playerNo), 0, 0, 0);
    }
}

int mbPlayerMotionCreate(int playerNo, int dataNum)
{
    return mbObjMotionCreate(mbPlayerObjIDGet(playerNo), dataNum);
}

int mbPlayerMotionKill(int playerNo, int motNo)
{
    mbObjMotionKill(mbPlayerObjIDGet(playerNo), motNo);
    return TRUE;
}

void mbPlayerMotionSpeedSet(int playerNo, float speed)
{
    mbObjMotionSpeedSet(mbPlayerObjIDGet(playerNo), speed);
}

void mbPlayerMotionTimeSet(int playerNo, float time)
{
    mbObjMotionTimeSet(mbPlayerObjIDGet(playerNo), time);
}

float mbPlayerMotionTimeGet(int playerNo)
{
    return mbObjMotionTimeGet(mbPlayerObjIDGet(playerNo));
}

float mbPlayerMotionMaxTimeGet(int playerNo)
{
    return mbObjMotionMaxTimeGet(mbPlayerObjIDGet(playerNo));
}

void mbPlayerMotionStartEndSet(int playerNo, float start, float end)
{
    mbObjMotionStartEndSet(mbPlayerObjIDGet(playerNo), start, end);
}

void mbPlayerAttrSet(int playerNo, int attr)
{
    MBOBJID objId = mbPlayerObjIDGet(playerNo);
    mbObjAttrSet(objId, attr);
}

void mbPlayerAttrReset(int playerNo, int attr)
{
    MBOBJID objId = mbPlayerObjIDGet(playerNo);
    mbObjAttrReset(objId, attr);
}

void mbPlayerMotionVoiceOnSet(int playerNo, int motNo, BOOL voiceOn)
{
    mbObjMotionVoiceOnSet(mbPlayerObjIDGet(playerNo), motNo, voiceOn);
}

BOOL mbPlayerMotionEndCheck(int playerNo)
{
    int objId = mbPlayerObjIDGet(playerNo);
    return mbObjMotionShiftIDGet(objId) < 0 && mbObjMotionEndCheck(objId);
}

BOOL mbPlayerMotionEndCheckAll(void)
{
    int i;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(!mbPlayerMotionEndCheck(i)) {
            return FALSE;
        }
    }
    return TRUE;
}

void mbPlayerMotionEndWait(int playerNo)
{
    while(!mbPlayerMotionEndCheck(playerNo)) {
        HuPrcVSleep();
    }
}

void mbPlayerMotIdleSet(int playerNo)
{
    mbPlayerMotionShiftSet(playerNo, 1, 0, 8, HU3D_MOTATTR_LOOP);
}

void mbPlayerCoinSet(int playerNo, int coinNum)
{
    if(!GWTeamGet()) {
        GWPlayerCoinSet(playerNo, coinNum);
    } else {
        GWPlayerCoinSet(mbPlayerTeamFindPlayer(GWPlayerTeamGet(playerNo), 0), coinNum);
    }
}

int mbPlayerCoinGet(int playerNo)
{
    if(!GWTeamGet()) {
        return GWPlayerCoinGet(playerNo);
    } else {
        return GWPlayerCoinGet(mbPlayerTeamFindPlayer(GWPlayerTeamGet(playerNo), 0));
    }
}

void mbPlayerCoinAdd(int playerNo, int coinNum)
{
    GW_PLAYER *playerP;
    if(GWTeamGet()) {
        playerNo = mbPlayerTeamFindPlayer(GWPlayerTeamGet(playerNo), 0);
    }
    playerP = GWPlayerGet(playerNo);
    if(coinNum > 0 && playerP->coinTotal < 999) {
        playerP->coinTotal += coinNum;
        if(playerP->coinTotal > 999) {
            playerP->coinTotal = 999;
        }
    }
    mbPlayerCoinSet(playerNo, coinNum + mbPlayerCoinGet(playerNo));
}

void mbPlayerTeamCoinSet(int teamNo, int coinNum)
{
    GWPlayerCoinSet(mbPlayerTeamFindPlayer(teamNo, 0), coinNum);
}

s16 mbPlayerTeamCoinGet(int teamNo)
{
    return GWPlayerCoinGet(mbPlayerTeamFindPlayer(teamNo, 0));
}

int mbPlayerMaxCoinGet(void)
{
    int maxCoin = 0;
    int i;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(mbPlayerCoinGet(i) >= maxCoin) {
            maxCoin = mbPlayerCoinGet(i);
        }
    }
    return maxCoin;
}

void mbPlayerStarSet(int playerNo, int starNum)
{
    if(!GWTeamGet()) {
        GWPlayerStarSet(playerNo, starNum);
    } else {
        GWPlayerStarSet(mbPlayerTeamFindPlayer(GWPlayerTeamGet(playerNo), 0), starNum);
    }
}

int mbPlayerStarGet(int playerNo)
{
    if(!GWTeamGet()) {
        return GWPlayerStarGet(playerNo);
    } else {
        return GWPlayerStarGet(mbPlayerTeamFindPlayer(GWPlayerTeamGet(playerNo), 0));
    }
}

void mbPlayerStarAdd(int playerNo, int starNum)
{
    int num;
    mbAudFXPlay(8);
    num = starNum + mbPlayerStarGet(playerNo);
    if(num < 0) {
        num = 0;
    }
    mbPlayerStarSet(playerNo, num);
}

void mbPlayerGrpStarSet(int grp, int starNum)
{
    GWPlayerStarSet(mbPlayerTeamFindPlayer(grp, 0), starNum);
}

s16 mbPlayerGrpStarGet(int grp)
{
    return GWPlayerStarGet(mbPlayerTeamFindPlayer(grp, 0));
}

int mbPlayerBestPathGet(void)
{
    int i;
    int bestPlayer;
    int len;
    int minLen;
    minLen = 9999;
    bestPlayer = -1;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        len = mbMasuFind_TypeStepGet2(GwPlayer[i].masuId, 7, 1, 1);
        if(len < minLen) {
            minLen = len;
            bestPlayer = i;
        }
    }
    if(bestPlayer < 0) {
        return mbRandMod(4);
    } else {
        return bestPlayer;
    }
}

int mbPlayerRankGet(int playerNo)
{
    int score[GW_PLAYER_MAX];
    int i;
    int rank;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        score[i] = mbPlayerCoinGet(i) | (mbPlayerStarGet(i) * 1024);
    }
    rank = 0;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(i != playerNo && score[playerNo] < score[i]) {
            rank++;
        }
    }
    return rank;
}

s16 mbPlayerTeamRankGet(int teamNo)
{
    int score[2];
    int i;
    int rank;
    for(i = 0; i < 2; i++) {
        score[i] = mbPlayerTeamCoinGet(i) | (mbPlayerGrpStarGet(i) * 2048);
    }
    rank = 0;
    for(i = 0; i < 2; i++) {
        if(i != teamNo && score[teamNo] < score[i]) {
            rank++;
        }
    }
    return rank;
}

void mbPlayerCapsuleUseSet(int capsuleNo)
{
    GwPlayer[GwSystem.turnPlayerNo].capsuleUse = capsuleNo;
}

int mbPlayerCapsuleUseGet(void)
{
    return GwPlayer[GwSystem.turnPlayerNo].capsuleUse;
}

int mbPlayerCapsuleMaxGet(void)
{
    return (GWTeamGet() == FALSE) ? 3 : 5;
}

static inline s8 *PlayerCapsulePtrGet(int playerNo, int index)
{
    if(!GWTeamGet()) {
        return &GwPlayer[playerNo].capsule[index];
    } else {
        int memberNo = (index < 3) ? 0 : 1;
        int grp = GWPlayerTeamGet(playerNo);
        int no = -1;
        int j;
        for(j = 0; j < GW_PLAYER_MAX; j++) {
            if(grp == GWPlayerTeamGet(j)) {
                no++;
                if(no == memberNo) {
                    break;
                }
            }
        }
        if(j >= GW_PLAYER_MAX) {
            return NULL;
        }
        return &GwPlayer[j].capsule[index - (memberNo * 3)];
    }
}

int mbPlayerCapsuleAdd(int playerNo, int capsuleNo)
{
    GW_PLAYER *playerP = GWPlayerGet(playerNo);
    int max = mbPlayerCapsuleMaxGet();
    int i;
    for(i = 0; i < max; i++) {
        if(mbPlayerCapsuleGet(playerNo, i) != -1) {
            continue;
        }
        *PlayerCapsulePtrGet(playerNo, i) = (s8)capsuleNo;
        return i;
    }
    return -1;
}

int mbPlayerCapsuleRemove(int playerNo, int index)
{
    int capsuleNo = mbPlayerCapsuleGet(playerNo, index);
    GW_PLAYER *playerP = GWPlayerGet(playerNo);
    int max;
    int i;
    if(capsuleNo == -1) {
        return capsuleNo;
    }
    max = mbPlayerCapsuleMaxGet();
    for(i = index; i < max - 1; i++) {
        *PlayerCapsulePtrGet(playerNo, i) = *PlayerCapsulePtrGet(playerNo, i + 1);
    }
    for(; i < max; i++) {
        *PlayerCapsulePtrGet(playerNo, i) = -1;
    }
    return capsuleNo;
}

int mbPlayerCapsuleFind(int playerNo, int capsuleNo)
{
    int max = mbPlayerCapsuleMaxGet();
    int i;
    for(i = 0; i < max; i++) {
        if(capsuleNo == mbPlayerCapsuleGet(playerNo, i)) {
            return i;
        }
    }
    return -1;
}

s8 mbPlayerCapsuleGet(int playerNo, int index)
{
    return *PlayerCapsulePtrGet(playerNo, index);
}

s8 mbPlayerTeamCapsuleGet(int teamNo, int index)
{
    return mbPlayerCapsuleGet(mbPlayerTeamFindPlayer(teamNo, 0), index);
}

static inline int PlayerCountCapsules(int playerNo)
{
    int i;
    for(i = 0; i < 3; i++) {
        if(GwPlayer[playerNo].capsule[i] == -1) {
            break;
        }
    }
    return i;
}

int mbPlayerCapsuleNumGet(int playerNo)
{
    int num = PlayerCountCapsules(playerNo);
    if(GWTeamGet()) {
        int otherPlayer = mbPlayerTeamFind(playerNo);
        num += PlayerCountCapsules(otherPlayer);
        if(num > 5) {
            num = 5;
        }
    }
    return num;
}

int mbPlayerTeamCapsuleNumGet(int teamNo)
{
    return mbPlayerCapsuleNumGet(mbPlayerTeamFindPlayer(teamNo, 0));
}

BOOL mbPlayerKettouCheck(int playerNo, s16 masuId)
{
    int i;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(playerNo != i && masuId == GwPlayer[i].masuId) {
            return TRUE;
        }
    }
    return FALSE;
}

void mbPlayerWinLoseVoicePlay(int playerNo, int motNo, int seId)
{
    MBOBJ *objP = mbObjGet(mbPlayerObjIDGet(playerNo));
    u8 charNo = GwPlayer[playerNo].charNo;
    CharWinLoseVoicePlay(charNo, objP->motId[motNo], seId);
}

int mbPlayerVoicePanPlay(int playerNo, s16 seId)
{
    mbObjSePlay(mbPlayerObjIDGet(playerNo), seId);
}

int mbPlayerVoicePlay(int playerNo, s16 seId)
{
    HuVecF pos;
    u8 pan;
    mbPlayerPosGet(playerNo, &pos);
    pan = mbAudFXPosPanGet(&pos);
    return CharFXPlayVolPan(GwPlayer[playerNo].charNo, seId, 127, pan);
}

void mbPlayerDispSet(int playerNo, BOOL dispF)
{
    if(GwPlayer[playerNo].masuId == 0) {
        dispF = FALSE;
    }
    mbObjDispSet(mbPlayerObjIDGet(playerNo), dispF);
}

int mbPlayerDispGet(int playerNo)
{
    return mbObjDispGet(mbPlayerObjIDGet(playerNo));
}

GXColor mbPlayerColorGet(int playerNo)
{
    GXColor color[13] = {
        { 227, 67, 67, 255 },
        { 68, 67, 227, 255 },
        { 241, 158, 220, 255 },
        { 67, 228, 68, 255 },
        { 138, 60, 180, 255 },
        { 227, 228, 68, 255 },
        { 192, 192, 192, 255 },
        { 227, 227, 227, 255 },
        { 40, 227, 227, 255 },
        { 227, 139, 40, 255 },
        { 180, 40, 40, 255 },
        { 40, 180, 40, 255 },
        { 40, 40, 180, 255 }
    };
    return color[GwPlayer[playerNo].charNo];
}

/* Unreferenced in retail: 16 bytes of 0xFF follow mbPlayerColorGet's table in .rodata. */
static const GXColor playerColorWhiteTbl[4] = {
    { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }
};

void mbPlayerBlackoutSet(BOOL value)
{
    blackoutF = value;
}

BOOL mbPlayerBlackoutGet(void)
{
    return blackoutF;
}

void mbPlayerMasuCornerSet(int playerNo, s8 cornerNo)
{
    playerWork[playerNo].masuCorner = cornerNo;
}

s8 mbPlayerMasuCornerGet(int playerNo)
{
    return playerWork[playerNo].masuCorner;
}

static void MasuCoinExec(int playerNo, int coinNum)
{
    HuVecF pos;
    s8 dispId;
    BOOL idleF;
    if(coinNum < 0) {
        omVibrate(playerNo, 20, 4, 4);
    }
    mbPlayerRotateStart(playerNo, 0, 15);
    if(GwSystem.last5Effect == 1) {
        coinNum *= 3;
    }
    mbPlayerPosGet(playerNo, &pos);
    pos.y += 250;
    if(coinNum >= 0) {
        mbAudFXPlay(0x44D);
    } else {
        mbAudFXPlay(0x44E);
    }
    dispId = mbCoinDispMasuCreate(&pos, coinNum, 0);
    while(playerWork[playerNo].rotateObj != NULL) {
        HuPrcVSleep();
    }
    if(coinNum >= 0) {
        mbPlayerWinLoseVoicePlay(playerNo, 12, 0x243);
        mbPlayerMotionShiftSet(playerNo, 12, 0, 4, HU3D_MOTATTR_NONE);
    } else {
        mbPlayerWinLoseVoicePlay(playerNo, 13, 0x249);
        mbPlayerMotionShiftSet(playerNo, 13, 0, 4, HU3D_MOTATTR_NONE);
    }
    mbCoinAddExec(playerNo, coinNum);
    mbCameraMoveWait();
    idleF = FALSE;
    while(!mbCoinDispKillCheck(dispId) || !idleF) {
        if(mbPlayerMotionEndCheck(playerNo) && !idleF) {
            mbPlayerMotIdleSet(playerNo);
            idleF = TRUE;
        }
        HuPrcVSleep();
    }
}

void mbPlayerPlusMasuExec(int playerNo)
{
    mbCameraMoveOnSet(TRUE);
    mbCameraPlayerViewSet(playerNo, 0);
    MasuCoinExec(playerNo, 3);
}

void mbPlayerCapCoinMasuExec(int playerNo)
{
    mbCameraMoveOnSet(TRUE);
    mbCameraPlayerViewSet(playerNo, 0);
    MasuCoinExec(playerNo, 5);
}

void mbPlayerMinusMasuExec(int playerNo)
{
    mbCameraMoveOnSet(TRUE);
    mbCameraPlayerViewSet(playerNo, 0);
    MasuCoinExec(playerNo, -3);
}
