#include "game/board/board.h"
#include "game/board/audio.h"
#include "game/board/branch.h"
#include "game/board/camera.h"
#include "game/board/capevent.h"
#include "game/board/capselect.h"
#include "game/board/capsule.h"
#include "game/board/coin.h"
#include "game/board/config.h"
#include "game/board/dice.h"
#include "game/board/effect.h"
#include "game/board/exit.h"
#include "game/board/gate.h"
#include "game/board/last5.h"
#include "game/board/masu.h"
#include "game/board/math.h"
#include "game/board/mgcall.h"
#include "game/board/object.h"
#include "game/board/opening.h"
#include "game/board/pause.h"
#include "game/board/player.h"
#include "game/board/scroll.h"
#include "game/board/single.h"
#include "game/board/snpc.h"
#include "game/board/star.h"
#include "game/board/status.h"
#include "game/board/telop.h"
#include "game/board/tutorial.h"
#include "game/board/window.h"
#include "game/board/wipe.h"

#include "game/gamework.h"
#include "game/flag.h"
#include "game/saveload.h"
#include "game/sprite.h"
#include "game/charman.h"
#include "game/audio.h"
#include "game/data.h"
#include "game/memory.h"
#include "string.h"

void HuDataDirCloseAll(void);

static MBEVENTHOOK ev_NextTime;
static MBEVENTHOOK ev_LoadTime;
static MBEVENTHOOK ev_TurnEnd;
static MBEVENTHOOK ev_TurnStart;
static MBLIGHTHOOK lightResetFunc;
static MBLIGHTHOOK lightSetFunc;
static MBKILLHOOK closeHook;
static MBCREATEHOOK initHook;
int mbSaveNewF;
HUPROCESS *mbMainProc;
OMOBJMAN *mbObjMan;

static OMOVL nextOvl = DLL_NONE;
int lbl_802BFBAC = 20;
int lbl_802BFBB0 = 20;

static void mbOMDestroy(void);
static void mbMain(void);
static void mbMainKill(void);
static void evdef_ChangeTime(void);
static void mbInit(void);
static void mbClose(void);

static inline void GWMgPackSet(int mgPack)
{
    GwSystem.mgPack = mgPack;
}

static inline BOOL mbPartyFGet(void)
{
    BOOL partyF = GwSystem.partyF;
    return partyF;
}

static inline BOOL mbTeamFGet(void)
{
    return GwSystem.tagF;
}

static inline void mbMgTimeSet(int time)
{
    GwMgTime = time;
}

void mbObjectSetup(int boardNo, MBCREATEHOOK createHook, MBKILLHOOK killHook)
{
    omSysPauseEnable(FALSE);
    mbPauseDisableSet(TRUE);
    mbSaveNewF = !_CheckFlag(FLAG_BOARD_SAVEINIT);
    mbBoardDataDirRead();
    mbMathInit();
    if(mbSaveNewF) {
        if(!_CheckFlag(FLAG_BOARD_TUTORIAL)) {
            if(GWPartyGet() != FALSE) {
                GWMgInstDispSet(GwCommon.partyMgInstDispF);
                GWMgComDispSet(GwCommon.partyMgComDispF);
                GWMgPackSet(GwCommon.partyMgPack);
                GWMessSpeedSet(GwCommon.partyMessSpeed);
                GWSaveModeSet(GwCommon.partySaveMode);
            } else {
                GWMgInstDispSet(GwCommon.storyMgInstDispF);
                GWMgComDispSet(GwCommon.storyMgComDispF);
                GWMgPackSet(GwCommon.storyMgPack);
                GWMessSpeedSet(GwCommon.storyMessSpeed);
                GWSaveModeSet(GwCommon.storySaveMode);
            }
        } else {
            GwSystem.curTime = 0;
            GwMgTime = 0;
        }
        _SetFlag(FLAG_BOARD_SAVEINIT);
        _ClearFlag(FLAG_BOARD_OPENING);
        _ClearFlag(FLAG_BOARD_MG);
        _ClearFlag(FLAG_BOARD_MG_KETTOU);
        _ClearFlag(FLAG_BOARD_MG_DONKEY);
        _ClearFlag(FLAG_BOARD_MG_KOOPA);
        _ClearFlag(FLAG_BOARD_MG_VS);
        GwSystem.turnNo = 1;
        mbStarFlagInit();
        mbStarNextNoSet(0);
        GwSystem.starTotal = 0;
        GwSystem.last5Effect = 0;
        GwSystem.curTime = GwSystem.nextTime = 0;
        if(GWMgTimeGet() == 1) {
            GwSystem.curTime = GwSystem.nextTime = 1;
        }
        GwSystem.timeTurn = 0;
        GwSystem.timeTurnMax = 3;
        _ClearFlag(FLAG_BOARD_DEBUG);
        _ClearFlag(FLAGNUM(FLAG_GROUP_BOARD, 3));
        _ClearFlag(FLAGNUM(FLAG_GROUP_BOARD, 7));
        _ClearFlag(FLAGNUM(FLAG_GROUP_BOARD, 8));
        _ClearFlag(FLAG_BOARD_NOMG);
        _ClearFlag(FLAGNUM(FLAG_GROUP_BOARD, 2));
        _ClearFlag(FLAG_BOARD_DICE_SHORT);
        _ClearFlag(FLAGNUM(FLAG_GROUP_BOARD, 6));
        _ClearFlag(FLAGNUM(FLAG_GROUP_SAVE, 3));
        mbMgCallInit();
        mbStarReset();
        mbMasuCapsuleReset();
        mbev_CapBankCoinInit();
        saveExecF = FALSE;
    }
    if(_CheckFlag(FLAGNUM(FLAG_GROUP_COMMON, 35))) {
        mbMgCallInit();
        mbStarReset();
        _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 35));
        saveExecF = TRUE;
    }
    nextOvl = DLL_NONE;
    _SetFlag(FLAG_BOARD_STAR_RESET);
    _SetFlag(FLAGNUM(FLAG_GROUP_COMMON, 16));
    _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 21));
    _ClearFlag(FLAG_BOARD_PAUSE);
    _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 23));
    _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 26));
    _ClearFlag(FLAG_BOARD_STORY_END);
    _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 29));
    _ClearFlag(FLAG_BOARD_TURN_NOSTART);
    mbConfigPadDisableSet(TRUE);
    initHook = createHook;
    closeHook = killHook;
    lightSetFunc = lightResetFunc = NULL;
    mbSNpcInit();
    mbTutorialInit();
    mbev_CapInit();
    ev_TurnStart = ev_TurnEnd = NULL;
    ev_LoadTime = ev_NextTime = NULL;
    mbev_NextTimeSet(evdef_ChangeTime);
    mbExitWatchCreate();
    mbObjMan = omInitObjMan(64, 8194);
    HuPrcDestructorSet2(mbObjMan, mbOMDestroy);
    omSystemKeyCheckSetup(mbObjMan);
    mbMainProc = HuPrcChildCreate(mbMain, 8208, 32768, 0, mbObjMan);
    HuPrcDestructorSet2(mbMainProc, mbMainKill);
    GwSystem.boardNo = boardNo;
}

static void mbOMDestroy(void)
{
    mbClose();
    if(nextOvl != DLL_NONE) {
        omOvlCall(nextOvl, 0, 0);
        return;
    }
    if(_CheckFlag(FLAGNUM(FLAG_GROUP_COMMON, 18))) {
        OMOVL ovl = omCurrentOvlGet();
        _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 18));
        omOvlGoto(ovl, 0, 0);
        return;
    }
    _ClearFlag(FLAG_BOARD_SAVEINIT);
    if(_CheckFlag(FLAG_BOARD_TURN_NOSTART)) {
        omOvlReturn(1);
    } else if(GWPartyGet() != FALSE && !_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        omOvlGoto(DLL_mdpresultdll, 0, 0);
    } else {
        omOvlReturn(1);
    }
}

static inline BOOL mbTimeDayCheck(void)
{
    return GwSystem.curTime == 0;
}

static void mbMain(void)
{
    BOOL turnIntrF = FALSE;
    int i;
    mbWipeWait();
    if(GwSystem.turnNo > GwSystem.turnMax && !_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 18));
        mbExitReq();
        HuPrcSleep(-1);
    }
    mbInit();
    if(!_CheckFlag(FLAG_BOARD_OPENING)) {
        if(_CheckFlag(FLAG_BOARD_DEBUG) && !_CheckFlag(FLAG_BOARD_TUTORIAL)) {
            if(GWPartyGet() != FALSE) {
                int starNo = mbStarNoRandGet();
                if(starNo >= 0) {
                    mbStarNoSet(starNo);
                }
            }
        } else {
            GwSystem.turnPlayerNo = -1;
            if(!_CheckFlag(FLAG_BOARD_TUTORIAL)) {
                mbev_Opening();
            } else {
                mbMusBoardPlay();
                _ClearFlag(FLAG_BOARD_STAR_RESET);
                mbTutorialCall(0);
            }
        }
        if(GWPartyGet() != FALSE) {
            if(!mbTeamFGet()) {
                for(i=0; i<4; i++) {
                    mbPlayerCoinSet(i, 10);
                }
            } else {
                for(i=0; i<2; i++) {
                    mbPlayerTeamCoinSet(i, 20);
                }
            }
        } else {
            for(i=0; i<4; i++) {
                mbPlayerCoinSet(i, 0);
            }
        }
        GwSystem.turnPlayerNo = 0;
        _SetFlag(FLAG_BOARD_OPENING);
    }
    if(GwSystem.curTime != GwSystem.nextTime) {
        GwSystem.timeTurn = 0;
        mbMgTimeSet(GwSystem.curTime);
        if(ev_NextTime != NULL) {
            mbMusBoardPlay();
            _ClearFlag(FLAG_BOARD_STAR_RESET);
            ev_NextTime();
            mbTelopTimeChangeKill();
            mbCameraMoveStop();
        }
        GwSystem.nextTime = GwSystem.curTime;
    }
    mbMgTimeSet(mbTimeDayCheck() ? 0 : 1);
    if(mbReturnMgCheck()) {
        if(GWPartyGet() == FALSE) {
            turnIntrF = mbev_SingleMgEnd(0);
        } else {
            if(_CheckFlag(FLAG_BOARD_MG)) {
                _ClearFlag(FLAG_BOARD_MG);
            } else if(_CheckFlag(FLAG_BOARD_MG_KETTOU)) {
                mbev_CapKettouEndCall(GwSystem.turnPlayerNo);
                _ClearFlag(FLAG_BOARD_MG_KETTOU);
                turnIntrF = TRUE;
            } else if(_CheckFlag(FLAG_BOARD_MG_DONKEY)) {
                mbev_CapDonkeyEndCall(GwSystem.turnPlayerNo);
                _ClearFlag(FLAG_BOARD_MG_DONKEY);
                turnIntrF = TRUE;
            } else if(_CheckFlag(FLAG_BOARD_MG_KOOPA)) {
                mbev_CapKoopaEndCall(GwSystem.turnPlayerNo);
                _ClearFlag(FLAG_BOARD_MG_KOOPA);
                turnIntrF = TRUE;
            } else if(_CheckFlag(FLAGNUM(FLAG_GROUP_COMMON, 19))) {
                if(ev_LoadTime != NULL) {
                    ev_LoadTime();
                }
                _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 19));
                turnIntrF = TRUE;
            }
            _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 18));
        }
    }
    while(1) {
        if(ev_TurnStart && !turnIntrF) {
            ev_TurnStart();
        }
        if(GwSystem.turnPlayerNo == 0 && !turnIntrF && GwSystem.turnMax-GwSystem.turnNo < 5) {
            if(!_CheckFlag(FLAGNUM(FLAG_GROUP_SAVE, 3)) && GWPartyGet() != FALSE) {
                mbev_Last5();
                _SetFlag(FLAGNUM(FLAG_GROUP_SAVE, 3));
            } else {
                mbTelopLastTurnCreate();
                if(!_CheckFlag(FLAGNUM(FLAG_GROUP_SAVE, 3)) && GWPartyGet() == FALSE) {
                    mbSingleCall(12, 0);
                    _SetFlag(FLAGNUM(FLAG_GROUP_SAVE, 3));
                }
            }
        }
        mbMusBoardPlay();
        _ClearFlag(FLAG_BOARD_STAR_RESET);
        if(_CheckFlag(FLAG_BOARD_TUTORIAL)) {
            mbTutorialCall(1);
            if(mbTutorialExitReqGet()) {
                HuPrcSleep(-1);
            }
        }
        if(GWPartyGet() != FALSE) {
            mbStatusDispForceSetAll(TRUE);
            mbTurnExec(turnIntrF);
            if(ev_TurnEnd) {
                ev_TurnEnd();
            }
            GwSystem.turnNo++;
            GwSystem.timeTurn++;
            GwSystem.turnPlayerNo = 0;
            if(_CheckFlag(FLAG_BOARD_TUTORIAL)) {
                mbTutorialCall(2);
                mbStatusColorAllSet(0);
                mbNextTime();
                continue;
            } else if(_CheckFlag(FLAG_BOARD_NOMG)) {
                turnIntrF = FALSE;
                mbStatusColorAllSet(0);
                mbNextTime();
                continue;
            } else {
                int result;
                GwSystem.turnPlayerNo = -1;
                result = mbev_MgCall();
                mbStatusColorAllSet(0);
                if(!result) {
                    mbNextTimeSet();
                    HuPrcSleep(-1);
                } else {
                    GwSystem.turnPlayerNo = 0;
                    mbNextTime();
                }
            }
        } else {
            mbStatusDispForceSetAll(TRUE);
            mbStatusMasuDispSet(TRUE);
            mbSingleTurnExec(turnIntrF);
            if(ev_TurnEnd) {
                ev_TurnEnd();
            }
            GwSystem.turnNo++;
            if(GwSystem.turnNo > GwSystem.turnMax) {
                mbSingleGameEnd();
                HuPrcSleep(-1);
            }
            GwSystem.turnPlayerNo = 0;
            if(_CheckFlag(FLAG_BOARD_NOMG)) {
                turnIntrF = FALSE;
                mbStatusColorAllSet(0);
                continue;
            } else if(_CheckFlag(FLAG_BOARD_TUTORIAL)) {
                mbStatusColorAllSet(0);
                continue;
            } else {
                GwSystem.turnPlayerNo = -1;
                mbStatusColorAllSet(0);
                GwSystem.turnPlayerNo = 0;
            }
        }
        turnIntrF = FALSE;
    }
}

static void mbMainKill(void)
{
    mbMainProc = NULL;
}

void mbChangeTimeSet(void)
{
    GwSystem.nextTime = GwSystem.curTime;
    GwSystem.curTime ^= 1;
}

static inline int mbTimeTurnGet(void)
{
    return GwSystem.timeTurn;
}

BOOL mbNextTimeSet(void)
{
    int timeTurnMax = GwSystem.timeTurnMax;
    if(mbTimeTurnGet() >= timeTurnMax) {
        mbChangeTimeSet();
        return TRUE;
    } else {
        return FALSE;
    }
}

void mbChangeTime(void)
{
    nextOvl = DLL_NONE;
    _SetFlag(FLAGNUM(FLAG_GROUP_COMMON, 18));
    _SetFlag(FLAGNUM(FLAG_GROUP_COMMON, 19));
    mbChangeTimeSet();
    mbExitReq();
    HuPrcSleep(-1);
}

void mbNextTime(void)
{
    if(mbNextTimeSet()) {
        nextOvl = DLL_NONE;
        _SetFlag(FLAGNUM(FLAG_GROUP_COMMON, 18));
        _SetFlag(FLAG_BOARD_MG);
        mbExitReq();
        HuPrcSleep(-1);
    }
}

static void evdef_ChangeTime(void)
{
    mbCameraZoomSet(10000);
    mbCameraRotSet(-45, 0, 0);
    mbCameraCenterSet(0, 0, 0);
    mbCameraMoveOnSet(FALSE);
    mbWipeFadeIn();
    mbTelopTimeChangeCreate();
    while(mbTelopTimeChangeCheck()) {
        HuPrcVSleep();
    }
    mbWipeDissolveFadeOut();
}

void mbOvlCall(int ovl)
{
    nextOvl = ovl;
    mbExitReq();
}

BOOL mbReturnMgCheck(void)
{
    if(_CheckFlag(FLAG_BOARD_MG)
        || _CheckFlag(FLAG_BOARD_MG_KETTOU)
        || _CheckFlag(FLAG_BOARD_MG_KOOPA)
        || _CheckFlag(FLAG_BOARD_MG_DONKEY)
        || _CheckFlag(FLAGNUM(FLAG_GROUP_COMMON, 19))
        || _CheckFlag(FLAGNUM(FLAG_GROUP_COMMON, 18))) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static void mbInit(void)
{
    if(!SLSaveFlagGet()) {
        GWSaveModeSet(GW_SAVE_MODE_NEVER);
    }
    if(GWMgInstDispGet()) {
        _SetFlag(FLAGNUM(FLAG_GROUP_SAVE, 5));
    } else {
        _ClearFlag(FLAGNUM(FLAG_GROUP_SAVE, 5));
    }
    if(!mbReturnMgCheck()) {
        _ClearFlag(FLAG_BOARD_MG);
        _ClearFlag(FLAG_BOARD_MG_KETTOU);
        _ClearFlag(FLAG_BOARD_MG_KOOPA);
        _ClearFlag(FLAG_BOARD_MG_DONKEY);
        _ClearFlag(FLAG_BOARD_MG_VS);
        _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 19));
        _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 18));
    }
    if(mbReturnMgCheck()) {
        mbMgCallDataClose();
    }
    HuSprExecLayerCameraSet(32, 2, 7);
    HuSprExecLayerCameraSet(64, 4, 3);
    mbCameraInit();
    mbAudInit();
    mbObjInit();
    mbPauseInit();
    mbWinInit();
    mbEffInit();
    mbWipeSpecialInit();
    mbGateInit();
    mbCapInit();
    mbCoinInit();
    mbBranchInit();
    mbStarInit();
    mbPlayerTurnInitHookSet(NULL);
    mbPlayerTurnCloseHookSet(NULL);
    initHook();
    mbPlayerInit(!mbReturnMgCheck());
    mbStatusInit();
    mbDiceInit();
    mbLightSet();
    CharModelDataClose(-1);
    mbTauntInit();
    Hu3DReflectNoSet(0);
    mbCapMasuObjInit();
    if(GWPartyGet() == FALSE) {
        mbSingleInit();
    }
    if(_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        mbTutorialCreate();
    }
    _SetFlag(FLAGNUM(FLAG_GROUP_COMMON, 21));
}

static void mbClose(void)
{
    HuMemHeapDump(HuMemHeapPtrGet(HEAP_DVD), -1);
    if(!_CheckFlag(FLAGNUM(FLAG_GROUP_COMMON, 21))) {
        mbDirClose();
        mbMathClose();
        return;
    }
    if(GWPartyGet() == FALSE) {
        mbSingleClose();
    }
    mbTauntClose();
    mbAudClose();
    HuAudAllStop();
    mbDiceClose();
    mbStatusClose();
    mbMasuClose();
    mbStarClose();
    mbGateClose();
    fn_8019A620();
    mbScrollClose();
    mbPlayerClose();
    if(closeHook) {
        closeHook();
    }
    mbCoinClose();
    mbEffClose();
    mbObjClose();
    mbWinClose();
    mbDirClose();
    initHook = closeHook = NULL;
    ev_TurnStart = NULL;
    ev_TurnEnd = NULL;
    ev_LoadTime = NULL;
    ev_NextTime = NULL;
    _SetFlag(FLAG_BOARD_STAR_RESET);
    _SetFlag(FLAGNUM(FLAG_GROUP_COMMON, 16));
    _SetFlag(FLAG_BOARD_PAUSE);
    mbMathClose();
}

void mbDirClose(void)
{
    HuDataDirCloseAll();
}

void mbLightFuncSet(MBLIGHTHOOK setFunc, MBLIGHTHOOK resetFunc)
{
    lightSetFunc = setFunc;
    lightResetFunc = resetFunc;
}

void mbLightSet(void)
{
    static HuVecF lightPos = { -500, 500, 2000 };
    static HuVecF lightDir = { 0.5f, -0.5f, -2.0f };
    static GXColor lightColor = { 255, 255, 255, 255 };
    int lightId;
    Hu3DBGColorSet(0, 0, 0);
    if(lightSetFunc) {
        lightSetFunc();
    }
    lightId = Hu3DGLightCreateV(&lightPos, &lightDir, &lightColor);
    Hu3DGLightStaticSet(lightId, FALSE);
    Hu3DGLightInfinitytSet(lightId);
}

void mbLightReset(void)
{
    if(lightResetFunc) {
        lightResetFunc();
    }
    Hu3DBGColorSet(0, 0, 0);
    Hu3DFogClear();
    Hu3DReflectNoSet(0);
}

void mbPauseEnableSet(void)
{
    _SetFlag(FLAGNUM(FLAG_GROUP_COMMON, 23));
}

void mbPauseEnableReset(void)
{
    _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 23));
}

BOOL mbPauseEnableCheck(void)
{
    return _CheckFlag(FLAGNUM(FLAG_GROUP_COMMON, 23)) ? TRUE : FALSE;
}

s32 mbBGRead(int dataNum)
{
    return HuDataDirReadAsync(dataNum);
}

void mbBGReadWait(s32 statId)
{
    if(statId == -1) {
        return;
    }
    while(HuDataGetAsyncStat(statId) == FALSE) {
        HuPrcVSleep();
    }
}

void mbev_NextTimeSet(MBEVENTHOOK func)
{
    ev_NextTime = func;
}

void mbev_LoadTimeSet(MBEVENTHOOK func)
{
    ev_LoadTime = func;
}

void mbev_TurnStartSet(MBEVENTHOOK func)
{
    ev_TurnStart = func;
}

void mbev_TurnEndSet(MBEVENTHOOK func)
{
    ev_TurnEnd = func;
}

void fn_8014C3B4(int value)
{
    lbl_802BFBAC = value;
}

void fn_8014C3BC(int value)
{
    lbl_802BFBB0 = value;
}

int fn_8014C3C4(void)
{
    return lbl_802BFBAC;
}

int fn_8014C3CC(void)
{
    return lbl_802BFBB0;
}

static inline int GWPlayerHandicapGet(int playerNo)
{
    return GwPlayer[playerNo].handicap;
}

void mbSaveInit(int boardNo)
{
    int i, j;
    GwSystem.boardNo = boardNo;
    _ClearFlag(FLAG_BOARD_SAVEINIT);
    _ClearFlag(FLAG_MG_PRACTICE);
    GwSystem.mgNo = 0;
    GwSystem.subGameNo = 0;
    GwSystem.hiddenBlockMasuId = 0;
    memset(GwSystem.boardWork, 0, sizeof(GwSystem.boardWork));
    GwSystem.turnNo = 1;
    for(i=0; i<4; i++) {
        GwPlayer[i].coin = 0;
        if(GWPartyGet() == FALSE || _CheckFlag(FLAG_BOARD_TUTORIAL)) {
            GwPlayer[i].star = 0;
            GwSystem.tagF = FALSE;
        } else {
            GwPlayer[i].star = GWPlayerHandicapGet(i);
        }
        mbMasuPlayerPrizeReset(i);
        GwPlayer[i].coinTotalMg = 0;
        GwPlayer[i].coinTotal = 0;
        GwPlayer[i].coinMax = 0;
        GwPlayer[i].starMax = 0;
        GwPlayer[i].coinBattle = 0;
        GwPlayer[i].mgCoin = 0;
        GwPlayer[i].mgCoinBonus = 0;
        GwPlayer[i].capsuleUseNum = 0;
        for(j=0; j<3; j++) {
            GwPlayer[i].capsule[j] = -1;
        }
    }
}

void mbSaveStoryInit(int arg0, int arg1, int arg2)
{
    mbSingleSaveInit(arg0, arg1, arg2);
    GwMgTime = 0;
}

void mbSavePartyInit(int tagF, int bonusStarF, int mgPack, int turnMax, int handicapP1, int handicapP2, int handicapP3, int handicapP4)
{
    GwSystem.partyF = TRUE;
    GwSystem.tagF = tagF;
    GwSystem.storyComDif = 0;
    GwSystem.bonusStarF = bonusStarF;
    GwSystem.mgPack = mgPack;
    GwSystem.turnMax = turnMax;
    GwPlayer[0].handicap = handicapP1;
    GwPlayer[1].handicap = handicapP2;
    GwPlayer[2].handicap = handicapP3;
    GwPlayer[3].handicap = handicapP4;
    _ClearFlag(FLAG_BOARD_TUTORIAL);
    _SetFlag(FLAGNUM(FLAG_GROUP_SAVE, 5));
    _SetFlag(FLAGNUM(FLAG_GROUP_COMMON, 13));
    _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 20));
    _ClearFlag(FLAG_INST_DECA);
    GwSystem.curTime = 0;
    GwMgTime = 0;
}

void fn_8014C818(void)
{
}

void fn_8014C81C(void)
{
}
