#include "game/board/pause.h"
#include "game/board/audio.h"
#include "game/board/effect.h"
#include "game/board/exit.h"
#include "game/board/guide.h"
#include "game/board/math.h"
#include "game/board/object.h"
#include "game/board/single.h"
#include "game/board/telop.h"
#include "game/board/window.h"
#include "game/board/wipe.h"

#include "game/gamework.h"
#include "game/flag.h"
#include "game/esprite.h"
#include "game/sprite.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/memory.h"
#include "game/data.h"
#include "game/audio.h"
#include "game/mgdata.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "datadir_enum.h"
#include "string.h"

/* config.h (placeholder prototypes) is not included: these take int panel ids. */
s16 mbPausePanelCreate(int dataNum, int arg1);
void mbPausePanelPosSet(s16 panelId, float x, float y);
void mbPausePanelBankSet(s16 panelId, int bank);
void mbPausePanelGrowSet(s16 panelId, int arg1, int arg2, float scale);
void mbPausePanelKill(s16 panelId);
int mbConfigExec(int playerNo, s16 guideMdlId);
void mbPauseGuideCreate();
void mbPauseGuideKill();
void mbPauseDispCopyCreate();
void mbPauseDispCopyKill();
int mbMasuNumGet();

#define PAUSE_HOOK_MAX 32
#define PAUSE_MGTYPE_MAX 6
#define PAUSE_MGLIST_MAX 32

typedef struct PauseWork_s {
    s16 sprId[6]; /* 0x00 */
    s16 panelId; /* 0x0C */
    s16 timeGrpId; /* 0x0E */
    int helpWin; /* 0x10 */
    int helpWin2; /* 0x14 */
} PAUSEWORK;

typedef struct PauseMgListEntry_s {
    u8 mgNo;
    u8 flag;
} PAUSEMGLISTENTRY;

typedef struct PauseMgList_s {
    s16 type; /* 0x00 */
    s16 unlockNum; /* 0x02 */
    s16 num; /* 0x04 */
    PAUSEMGLISTENTRY list[PAUSE_MGLIST_MAX]; /* 0x06 */
} PAUSEMGLIST; /* size 0x46 */

typedef struct PauseSingleWork_s {
    int winBlank; /* 0x00 */
    int winName; /* 0x04 */
    int win[2]; /* 0x08 */
    int growTime[2]; /* 0x10 */
    int arrowF[2]; /* 0x18 */
    s16 sprId[14]; /* 0x20 */
} PAUSESINGLEWORK; /* size 0x3C */

static MBPAUSEHOOK pauseHook[PAUSE_HOOK_MAX];
static PAUSEMGLIST pauseMGList[PAUSE_MGTYPE_MAX];
static PAUSESINGLEWORK pauseSingleWork;
static HUPROCESS *pauseProc;
static int pauseHookNum;
static OMOBJ *pauseGuideObj;
static int pausePlayer = -1;
static s8 guideMotTbl[5] = { 3, 12, 11, 23, -1 };
static s8 guideMotSingleTbl[5] = { 3, 12, 11, 23, -1 };
static const u32 HelpWinMesTbl[2] = { 0x260008, 0x280018 };

static void PauseMain(void);
static void PauseDestroy(void);

static inline int GWBoardNoGet(void)
{
    return GwSystem.boardNo;
}

static inline BOOL GWTeamGet(void)
{
    return GwSystem.tagF;
}

static inline int GWMgPackGet(void)
{
    if(GwSystem.mgPack >= 5) {
        GwSystem.mgPack = 0;
    }
    return GwSystem.mgPack;
}

void mbPauseInit(void)
{
    int i;
    pauseProc = NULL;
    pauseHookNum = 0;
    HuPrcAllPause(FALSE);
    Hu3DPauseSet(FALSE);
    HuSprPauseSet(FALSE);
    for(i=0; i<PAUSE_HOOK_MAX; i++) {
        pauseHook[i] = NULL;
    }
}

void mbPauseCreate(int playerNo)
{
    pauseProc = HuPrcChildCreate(PauseMain, 8210, 14336, 0, mbMainProc);
    HuPrcDestructorSet2(pauseProc, PauseDestroy);
    pausePlayer = playerNo;
    HuPrcSetStat(pauseProc, 12);
    mbPauseSet(TRUE);
}

int mbPauseStartCheck(void)
{
    int i;
    if(omUPauseFlag) {
        return -1;
    }
    if(mbPauseProcCheck()) {
        return -1;
    }
    if(mbPauseDisableGet()) {
        return -1;
    }
    if(_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        return -1;
    }
    if(!mbEffFadeCheck()) {
        return -1;
    }
    if(mbWipeSpecialCheck()) {
        return -1;
    }
    if(mbWipeSpecialStatGet()) {
        return -1;
    }
    for(i=0; i<4; i++) {
        int padNo = GwPlayer[i].padNo;
        if(HuPadStatGet(padNo) == 0) {
            if((GWPartyGet() != FALSE || GwPlayer[i].comF == FALSE) && (HuPadBtnDown[padNo] & PAD_BUTTON_START)) {
                return i;
            }
        }
    }
    return -1;
}

BOOL mbPauseProcCheck(void)
{
    return (pauseProc != NULL) ? TRUE : FALSE;
}

void mbPauseDisableSet(BOOL disableF)
{
    if(disableF) {
        _SetFlag(FLAGNUM(FLAG_GROUP_COMMON, 31));
    } else {
        _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 31));
    }
}

BOOL mbPauseDisableGet(void)
{
    return (_CheckFlag(FLAGNUM(FLAG_GROUP_COMMON, 31))) ? TRUE : FALSE;
}

void mbPauseHookPush(MBPAUSEHOOK hook)
{
    pauseHook[pauseHookNum++] = hook;
}

void mbPauseHookPop(MBPAUSEHOOK hook)
{
    int i;
    for(i=0; i<pauseHookNum; i++) {
        if(pauseHook[i] == hook) {
            break;
        }
    }
    if(i >= pauseHookNum) {
        return;
    }
    for(; i<pauseHookNum-1; i++) {
        pauseHook[i] = pauseHook[i+1];
    }
    pauseHookNum--;
}

static void PauseScreenCreate(PAUSEWORK *work);
static void PauseScreenKill(PAUSEWORK *work);
static int PauseScreenExec(PAUSEWORK *work);

static void PauseMain(void)
{
    PAUSEWORK pauseWork;
    PAUSEWORK *work = &pauseWork;
    s32 statId;
    BOOL cancelF = FALSE;
    BOOL contF = TRUE;
    int i;
    static const unsigned int logoFileTbl[] = {
        DATANUM(DATA_board, 0x44),
        DATANUM(DATA_board, 0x45),
        DATANUM(DATA_board, 0x46),
        DATANUM(DATA_board, 0x47),
        DATANUM(DATA_board, 0x48),
        DATANUM(DATA_board, 0x49),
        DATANUM(DATA_board, 0x4C),
        DATANUM(DATA_board, 0x4B),
        DATANUM(DATA_board, 0x4A),
        DATANUM(DATA_board, 0x44),
        DATANUM(DATA_board, 0x44)
    };
    HuMemHeapDump(HuMemHeapPtrGet(HEAP_DVD), -1);
    pauseGuideObj = NULL;
    work->sprId[0] = espEntry(mbBoardDataNumGet(logoFileTbl[GWBoardNoGet()]), 100, 0);
    espDispOff(work->sprId[0]);
    work->timeGrpId = -1;
    if(GWPartyGet() != FALSE) {
        work->timeGrpId = mbTelopTimeSprCreate();
        mbTelopTimeSprRotSet(work->timeGrpId, 0);
        mbTelopTimeDispSet(work->timeGrpId, FALSE);
        HuSprGrpPosSet(work->timeGrpId, 100.799995f, 216);
        HuSprGrpScaleSet(work->timeGrpId, 0.65f, 0.65f);
    }
    HuDataDirClose(mbBoardDataNumGet(DATA_board));
    statId = mbBGRead(mbBoardDataNumGet(DATA_bpause6));
    mbEffFadeCreate(30, 160);
    for(i=0; i<pauseHookNum; i++) {
        if(pauseHook[i] != NULL) {
            pauseHook[i](FALSE);
        }
    }
    HuPadRumbleAllStop();
    HuPrcSleep(1);
    mbAudFXPlay(5);
    while(!mbEffFadeDoneCheck()) {
        HuPrcVSleep();
    }
    mbPauseGuideCreate();
    mbPauseDispCopyCreate();
    mbBGReadWait(statId);
    while(cancelF == FALSE && contF) {
        PauseScreenCreate(work);
        contF = PauseScreenExec(work);
        PauseScreenKill(work);
        if(contF) {
            cancelF = mbConfigExec(pausePlayer, mbGuideModelGet(pauseGuideObj));
        }
    }
    if(pauseGuideObj) {
        mbGuideKill(pauseGuideObj);
        pauseGuideObj = NULL;
    }
    espKill(work->sprId[0]);
    work->sprId[0] = -1;
    if(work->timeGrpId >= 0) {
        mbTelopTimeSprKill(work->timeGrpId);
        work->timeGrpId = -1;
    }
    mbPauseGuideKill();
    HuDataDirClose(mbBoardDataNumGet(DATA_bpause6));
    mbPauseDispCopyKill();
    if(cancelF) {
        mbExitReq();
        HuPrcSleep(-1);
    }
    statId = mbBGRead(mbBoardDataNumGet(DATA_board));
    mbEffFadeOutSet(30);
    HuPrcSleep(30);
    mbBGReadWait(statId);
    HuPrcEnd();
}

static void PauseDestroy(void)
{
    int i;
    if(!mbExitCheck()) {
        for(i=0; i<pauseHookNum; i++) {
            if(pauseHook[i] != NULL) {
                pauseHook[i](TRUE);
            }
        }
        mbPauseSet(FALSE);
    }
    if(GWMgInstDispGet()) {
        _SetFlag(FLAGNUM(FLAG_GROUP_SAVE, 5));
    } else {
        _ClearFlag(FLAGNUM(FLAG_GROUP_SAVE, 5));
    }
    if(GWPartyGet() == FALSE) {
        GwCommon.storyMgInstDispF = GWMgInstDispGet();
        GwCommon.storyMgComDispF = GWMgComDispGet();
        GwCommon.storyMgPack = GWMgPackGet();
        GwCommon.storyMessSpeed = GWMessSpeedGet();
        GwCommon.storySaveMode = GWSaveModeGet();
    } else {
        GwCommon.partyMgInstDispF = GWMgInstDispGet();
        GwCommon.partyMgComDispF = GWMgComDispGet();
        GwCommon.partyMgPack = GWMgPackGet();
        GwCommon.partyMessSpeed = GWMessSpeedGet();
        GwCommon.partySaveMode = GWSaveModeGet();
    }
    pausePlayer = -1;
    pauseProc = NULL;
}

static void PauseScreenSingleCreate(PAUSEWORK *work);
static void PauseScreenSingleKill(PAUSEWORK *work);
static int PauseSingleExec(PAUSEWORK *work);

static void PauseScreenCreate(PAUSEWORK *work)
{
    int lang;
    int i;
    int turn;
    HuVec2f center;
    HuVecF pos;
    static const HuVec2f turnNoPos[4] = {
        { -74, -25 },
        { -30, -25 },
        { 30, 25 },
        { 74, 25 }
    };
    static const float turnOfsTbl[6] = { 0, 64, 100, 100, 100, 100 };
    static const int anmNoTbl[3] = { 2, 0, 1 };
    static HuVecF guidePos = { -0.65f, -0.75f, -750 };
    if(GWPartyGet() == FALSE) {
        PauseScreenSingleCreate(work);
        return;
    }
    lang = mbLanguageGet();
    espDispOn(work->sprId[0]);
    espAttrSet(work->sprId[0], HUSPR_ATTR_LINEAR);
    espPosSet(work->sprId[0], 288, 128);
    work->panelId = mbPausePanelCreate(mbBoardDataNumGet(DATA_bpause6), 0);
    mbPausePanelPosSet((s16)work->panelId, 0.65f, 0.0f);
    mbPausePanelBankSet((s16)work->panelId, anmNoTbl[(GWTeamGet() != FALSE) ? 2 : GWPartyGet()]);
    mbPausePanelGrowSet((s16)work->panelId, 16, 0, 1.5f);
    work->sprId[1] = espEntry(mbBoardDataNumGet(DATANUM(DATA_bpause6, 1)), 100, 0);
    espAttrSet(work->sprId[1], HUSPR_ATTR_LINEAR);
    pos.x = 273.6f+turnOfsTbl[lang];
    pos.y = 304;
    espPosSet(work->sprId[1], pos.x, pos.y);
    for(i=0; i<4; i++) {
        work->sprId[i+2] = espEntry(mbBoardDataNumGet(DATANUM(DATA_bpause6, 2)), 95, 0);
        espPosSet(work->sprId[i+2], pos.x+turnNoPos[i].x, pos.y+turnNoPos[i].y);
        espAttrSet(work->sprId[i+2], HUSPR_ATTR_LINEAR);
    }
    turn = GwSystem.turnNo;
    if(turn > 99) {
        turn = 99;
    }
    if(turn/10 != 0) {
        espBankSet(work->sprId[2], turn/10);
    } else {
        espAttrSet(work->sprId[2], HUSPR_ATTR_DISPOFF);
    }
    espBankSet(work->sprId[3], turn%10);
    turn = GwSystem.turnMax;
    if(turn > 99) {
        turn = 99;
    }
    if(turn/10 != 0) {
        espBankSet(work->sprId[4], turn/10);
    } else {
        espAttrSet(work->sprId[4], HUSPR_ATTR_DISPOFF);
    }
    espBankSet(work->sprId[5], turn%10);
    if(pauseGuideObj == NULL) {
        mbNormPosto3D(&guidePos, 4, &pos);
        pauseGuideObj = mbGuideCreateFlag(&pos, guideMotTbl, TRUE, FALSE, TRUE);
        mbObjDispSet(mbGuideModelGet(pauseGuideObj), FALSE);
    }
    if(work->timeGrpId >= 0) {
        mbTelopTimeDispSet(work->timeGrpId, TRUE);
    }
    work->helpWin = work->helpWin2 = -1;
    work->helpWin = mbWinCreateHelp(HelpWinMesTbl[0]);
    mbWinCenterGet(work->helpWin, &center);
    mbWinPosSet(work->helpWin, center.x, 376);
    HuPrcVSleep();
    mbObjDispSet(mbGuideModelGet(pauseGuideObj), TRUE);
}

static void PauseScreenSingleCreate(PAUSEWORK *work)
{
    HuVec2f center;
    HuVecF pos;
    static HuVecF guidePos = { -0.7f, -0.8f, -750 };
    espDispOn(work->sprId[0]);
    espAttrSet(work->sprId[0], HUSPR_ATTR_LINEAR);
    espPosSet(work->sprId[0], 92.159996f, 120);
    espScaleSet(work->sprId[0], 0.5f, 0.5f);
    work->panelId = 0;
    if(pauseGuideObj == NULL) {
        mbNormPosto3D(&guidePos, 4, &pos);
        pauseGuideObj = mbGuideCreateFlag(&pos, guideMotSingleTbl, TRUE, FALSE, TRUE);
    }
    work->helpWin = work->helpWin2 = -1;
    work->helpWin = mbWinCreateHelp(0x260008);
    mbWinCenterGet(work->helpWin, &center);
    mbWinPosSet(work->helpWin, center.x, 376);
    work->helpWin2 = mbWinCreateHelp(0x280027);
    mbWinCenterGet(work->helpWin2, &center);
    mbWinPosSet(work->helpWin2, center.x, 332);
}

static void PauseScreenKill(PAUSEWORK *work)
{
    int i;
    if(GWPartyGet() == FALSE) {
        PauseScreenSingleKill(work);
        return;
    }
    if(work->helpWin != -1) {
        mbWinKill(work->helpWin);
        work->helpWin = -1;
    }
    espDispOff(work->sprId[0]);
    if(work->timeGrpId >= 0) {
        mbTelopTimeDispSet(work->timeGrpId, FALSE);
    }
    for(i=1; i<6; i++) {
        if(work->sprId[i] >= 0) {
            espKill(work->sprId[i]);
        }
        work->sprId[i] = -1;
    }
    if(work->panelId != 0) {
        mbPausePanelKill((s16)work->panelId);
        work->panelId = 0;
    }
}

static void PauseScreenSingleKill(PAUSEWORK *work)
{
    espDispOff(work->sprId[0]);
    if(work->helpWin != -1) {
        mbWinKill(work->helpWin);
        work->helpWin = -1;
    }
    if(work->helpWin2 != -1) {
        mbWinKill(work->helpWin2);
        work->helpWin2 = -1;
    }
}

static int PauseScreenExec(PAUSEWORK *work)
{
    int time = 0;
    int padNo;
    int result;
    if(GWPartyGet() == FALSE) {
        return PauseSingleExec(work);
    }
    while(1) {
        HuPrcVSleep();
        padNo = GwPlayer[pausePlayer].padNo;
        if(HuPadStatGet(padNo)) {
            continue;
        }
        if(HuPadBtnDown[padNo] & PAD_BUTTON_A) {
            result = 1;
            mbAudFXPlay(2);
            break;
        }
        time++;
        if(time >= 60) {
            time = 0;
        }
        if(work->timeGrpId >= 0) {
            mbTelopTimeSprRotSet(work->timeGrpId, (1.0f/60.0f)*time);
        }
        if(HuPadBtnDown[padNo] & PAD_BUTTON_START) {
            result = 0;
            break;
        }
    }
    return result;
}

void mbPauseSet(BOOL pauseF)
{
    OMOBJWORK *objWork = mbObjMan->property;
    int i;
    if(pauseF) {
        for(i=0; i<objWork->objMax; i++) {
            if((objWork->objData[i].stat & (OM_STAT_DELETED|OM_STAT_NOPAUSE)) == 0) {
                omSetStatBit(&objWork->objData[i], OM_STAT_PAUSED);
            }
        }
        mbPauseEnableSet();
    } else {
        for(i=0; i<objWork->objMax; i++) {
            if((objWork->objData[i].stat & (OM_STAT_DELETED|OM_STAT_NOPAUSE)) == 0) {
                omResetStatBit(&objWork->objData[i], OM_STAT_PAUSED);
            }
        }
        mbPauseEnableReset();
    }
    HuPrcAllPause(pauseF);
    Hu3DPauseSet(pauseF);
    HuSprPauseSet(pauseF);
    HuAudFXPauseAll(pauseF);
    HuAudSeqPauseAll(pauseF);
    mbMusPauseSet(pauseF);
}

static unsigned int pauseDataDirTbl[6] = {
    DATA_bpause6,
    DATA_bpause6_us,
    DATA_bpause6,
    DATA_bpause6,
    DATA_bpause6,
    DATA_bpause6
};

unsigned int mbPauseDataDirGet(int lang)
{
    if(lang < 0) {
        lang = 0;
    }
    return pauseDataDirTbl[lang];
}

unsigned int mbPauseDataNumGet(unsigned int dataNum)
{
    int lang = mbLanguageGet();
    if((dataNum & 0xFFFF0000) != DATA_bpause6) {
        return dataNum;
    }
    if(lang < 0) {
        lang = 0;
    }
    return (dataNum & 0xFFFF)|mbPauseDataDirGet(lang);
}

static s16 mgTypeTbl[6] = { MG_TYPE_4P, MG_TYPE_1VS3, MG_TYPE_2VS2, MG_TYPE_BATTLE, MG_TYPE_KETTOU, -1 };

static int PauseSingleMGListGet(PAUSEMGLIST *list)
{
    int i;
    int total;
    PAUSEMGLIST *entry;
    int j;
    int type;
    memset(list, 0, sizeof(PAUSEMGLIST)*PAUSE_MGTYPE_MAX);
    for(i=0; i<PAUSE_MGTYPE_MAX; i++) {
        list[i].type = i;
    }
    for(i=0; MgDataTbl[i].ovl != 0xFFFF; i++) {
        type = 6;
        if(MgDataTbl[i].flag & MG_FLAG_RARE) {
            type = 5;
            if(MgDataTbl[i].ovl == 0x52) {
                type = 6;
            }
        } else {
            for(j=0; j<5; j++) {
                if(mgTypeTbl[j] == MgDataTbl[i].type) {
                    type = j;
                    break;
                }
            }
        }
        if(type < 6) {
            entry = &list[type];
            entry->list[entry->num].mgNo = i;
            if(GWMgUnlockGet(MgNoGet(MgDataTbl[i].ovl)+601)) {
                entry->list[entry->num].flag = 1;
            }
            if(mbSingleMgUnlockGet(MgNoGet(MgDataTbl[i].ovl)+601)) {
                entry->list[entry->num].flag |= 2;
            }
            if(entry->list[entry->num].flag != 0) {
                entry->unlockNum++;
            }
            entry->num++;
        }
    }
    total = 0;
    for(i=0; i<PAUSE_MGTYPE_MAX; i++) {
        total += list[i].unlockNum;
    }
    return total;
}

static void PauseSingleSprCreate(PAUSEMGLIST *list, PAUSESINGLEWORK *work)
{
    int unlockNum;
    int i;
    int tens;
    int num;
    int lang;
    HuVec2f size;
    HuVecF pos;
    static unsigned int fileTbl[14] = {
        DATANUM(DATA_bpause6, 0x2C),
        DATANUM(DATA_bpause6, 0x28),
        DATANUM(DATA_bpause6, 0x29),
        DATANUM(DATA_bpause6, 0x29),
        DATANUM(DATA_bpause6, 0x2B),
        DATANUM(DATA_bpause6, 0x2B),
        DATANUM(DATA_bpause6, 0x2A),
        DATANUM(DATA_bpause6, 0x2A),
        DATANUM(DATA_bpause6, 0x27),
        DATANUM(DATA_bpause6, 0x26),
        DATANUM(DATA_bpause6, 0x26),
        DATANUM(DATA_bpause6, 0x26),
        DATANUM(DATA_bpause6, 0x20),
        DATANUM(DATA_bpause6, 0x20)
    };
    static HuVec2f posTbl[14] = {
        { 288, 136 },
        { 472, 72 },
        { 424, 72 },
        { 520, 72 },
        { 424, 72 },
        { 520, 72 },
        { 460, 72 },
        { 484, 72 },
        { 480, 152 },
        { 504, 152 },
        { 524, 152 },
        { 542, 152 },
        { 288, 172 },
        { 288, 312 }
    };
    static s16 langOfsTbl[6][6] = {
        { -48, 48, -48, 48, -12, 12 },
        { -28, -28, -28, -28, -64, -40 },
        { -28, -28, -28, -28, -64, -40 },
        { -28, -28, -28, -28, -64, -40 },
        { -28, -28, -28, -28, -64, -40 },
        { -28, -28, -28, -28, -64, -40 }
    };
    unlockNum = PauseSingleMGListGet(list);
    memset(work, 0, sizeof(PAUSESINGLEWORK));
    for(i=0; i<14; i++) {
        work->sprId[i] = espEntry(mbBoardDataNumGet(fileTbl[i]), 100, 0);
        espAttrSet(work->sprId[i], HUSPR_ATTR_LINEAR);
        espPosSet(work->sprId[i], posTbl[i].x, posTbl[i].y);
    }
    espBankSet(work->sprId[12], 1);
    espBankSet(work->sprId[13], 3);
    espDispOff(work->sprId[12]);
    espDispOff(work->sprId[13]);
    espPriSet(work->sprId[0], 2000);
    work->growTime[0] = work->growTime[1] = 0;
    work->arrowF[0] = work->arrowF[1] = 0;
    lang = mbLanguageGet();
    for(i=0; i<6; i++) {
        espPosSet(work->sprId[i+2], langOfsTbl[lang][i]+472, 72);
    }
    espBankSet(work->sprId[3], 1);
    espBankSet(work->sprId[5], 1);
    espDispOff(work->sprId[6]);
    i = mbSingleStepGet();
    if(i <= 1) {
        espDispOff(work->sprId[2]);
        espDispOff(work->sprId[3]);
    } else {
        espDispOff(work->sprId[4]);
        espDispOff(work->sprId[5]);
    }
    if(i < 0) {
        i = 0;
    }
    if(i >= mbMasuNumGet()) {
        i = 0;
    }
    num = i;
    tens = num/10;
    if(tens > 0) {
        espDispOn(work->sprId[6]);
        espBankSet(work->sprId[6], tens);
    }
    num -= tens*10;
    if(num > 0) {
        if(i <= 5) {
            num += 9;
        }
        espBankSet(work->sprId[7], num);
    }
    for(i=0; i<4; i++) {
        espScaleSet(work->sprId[i+8], 1.2f, 1.2f);
    }
    espBankSet(work->sprId[9], 10);
    espDispOff(work->sprId[11]);
    i = 0;
    tens = unlockNum/10;
    if(tens > 0) {
        espDispOn(work->sprId[11]);
        espBankSet(work->sprId[10], tens);
        i++;
    }
    num = unlockNum-(tens*10);
    if(num > 0) {
        espBankSet(work->sprId[i+10], num);
    }
    pos.x = 288;
    pos.y = 240;
    pos.z = 0;
    work->winBlank = mbWinCreateBlank();
    size.x = 540;
    size.y = 120;
    mbWinSizeSet(work->winBlank, size.x, size.y);
    pos.x += 0.5f*-size.x;
    pos.y += 0.5f*-size.y;
    mbWinPosSet(work->winBlank, pos.x, pos.y);
    pos.x += 4;
    pos.y += 4;
    for(i=0; i<2; i++) {
        work->win[i] = mbWinCreateHelp(0x28001A);
        mbWinMesMaxSizeGet(work->win[i], &size);
        mbWinSizeSet(work->win[i], 268, size.y);
        mbWinPosSet(work->win[i], pos.x+((i & 1) ? 268 : 0), pos.y);
    }
    work->winName = mbWinCreateHelp(0x28001F);
    mbWinMesMaxSizeGet(work->winName, &size);
    pos.x = 288;
    pos.y = 240;
    mbWinPosSet(work->winName, pos.x-(0.5f*size.x), (pos.y-108)-(0.5f*size.y));
}

static void PauseSingleMGTypeSet(PAUSEMGLIST *list, PAUSESINGLEWORK *work, int type, int ofs)
{
    PAUSEMGLIST *entry;
    int i;
    int idx;
    u32 mes;
    u32 statMes;
    int lang;
    HuVec2f size;
    static const float nameScaleTbl[6] = { 1.0f, 0.85f, 0.85f, 0.85f, 0.85f, 0.85f };
    static u32 insertMesTbl[6] = { 0x28001F, 0x280020, 0x280021, 0x280022, 0x280023, 0x280026 };
    static int insertMesTbl2[8] = { 0, 0, 1, 1, 2, 2, 3, 3 };
    static int insertMesTbl3[8] = { 4, 4, 5, 5, 6, 6, 7, 7 };
    entry = &list[type];
    lang = mbLanguageGet();
    mbWinCenterInsertGet(work->winName, insertMesTbl[type]);
    mbWinScaleSet(work->winName, nameScaleTbl[lang], 1.0f);
    mbWinMesMaxSizeGet(work->winName, &size);
    mbWinPosSet(work->winName, 288-((0.5f*size.x)*nameScaleTbl[lang]), 136-(0.5f*size.y));
    for(i=0; i<8; i++) {
        idx = i+ofs;
        mes = (u32)" ";
        statMes = 0x28001D;
        if(idx < entry->num) {
            if(entry->list[idx].flag != 0) {
                mes = MgDataTbl[entry->list[idx].mgNo].nameMes;
                if(entry->list[idx].flag >= 2) {
                    statMes = 0x28001E;
                }
            } else {
                mes = 0x28001C;
            }
        }
        mbWinInsertMesSet(work->win[i & 1], mes, insertMesTbl2[i]);
        mbWinInsertMesSet(work->win[i & 1], statMes, insertMesTbl3[i]);
    }
    for(i=0; i<2; i++) {
        mbWinCenterInsertGet(work->win[i], 0x28001A);
    }
    work->arrowF[0] = work->arrowF[1] = TRUE;
    if(ofs <= 0) {
        work->arrowF[0] = FALSE;
    }
    if(ofs+8 >= entry->num) {
        work->arrowF[1] = FALSE;
    }
}

static int PauseSingleExec(PAUSEWORK *work)
{
    float scale;
    float t;
    int i;
    int padNo;
    int result;
    int j;
    int repWait = 0;
    int curType = 0;
    int curOfs = 0;
    int newType;
    int newOfs;
    PauseSingleSprCreate(pauseMGList, &pauseSingleWork);
    HuPrcVSleep();
    PauseSingleMGTypeSet(pauseMGList, &pauseSingleWork, 0, 0);
    while(1) {
        HuPrcVSleep();
        padNo = GwPlayer[pausePlayer].padNo;
        if(HuPadStatGet(padNo)) {
            continue;
        }
        if(HuPadBtnDown[padNo] & PAD_BUTTON_A) {
            result = 1;
            mbAudFXPlay(2);
            break;
        }
        if(HuPadBtnDown[padNo] & PAD_BUTTON_START) {
            result = 0;
            break;
        }
        if(repWait) {
            repWait--;
        } else {
            newType = curType;
            newOfs = curOfs;
            if(HuPadBtnRep[padNo] & PAD_TRIGGER_L) {
                newType--;
                if(newType < 0) {
                    newType += 6;
                }
                newOfs = 0;
                repWait = 8;
            } else if(HuPadBtnRep[padNo] & PAD_TRIGGER_R) {
                newType++;
                if(newType >= 6) {
                    newType -= 6;
                }
                newOfs = 0;
                repWait = 8;
            } else if(HuPadDStkRep[padNo] & 4) {
                if(newOfs+8 < pauseMGList[curType].num) {
                    newOfs += 2;
                    pauseSingleWork.growTime[1] = 8;
                }
            } else if(HuPadDStkRep[padNo] & 8) {
                if(newOfs > 0) {
                    newOfs -= 2;
                    if(newOfs < 0) {
                        newOfs = 0;
                    }
                    pauseSingleWork.growTime[0] = 8;
                }
            }
            if(repWait) {
                pauseSingleWork.growTime[0] = pauseSingleWork.growTime[1] = 0;
            }
            if(curType != newType || curOfs != newOfs) {
                mbAudFXPlay(0);
                PauseSingleMGTypeSet(pauseMGList, &pauseSingleWork, newType, newOfs);
                curType = newType;
                curOfs = newOfs;
            }
        }
        for(i=0; i<2; i++) {
            scale = 1.0f;
            if(pauseSingleWork.arrowF[i]) {
                espDispOn(pauseSingleWork.sprId[i+12]);
                espTPLvlSet(pauseSingleWork.sprId[i+12], 1.0f);
            }
            if(pauseSingleWork.growTime[i]) {
                pauseSingleWork.growTime[i]--;
                scale = t = pauseSingleWork.growTime[i]/8.0f;
                scale = 1.5f-(0.5f*mbCosDeg(90*scale));
                if(!pauseSingleWork.arrowF[i]) {
                    espTPLvlSet(pauseSingleWork.sprId[i+12], mbSinDeg(90*t));
                }
            } else if(!pauseSingleWork.arrowF[i]) {
                espDispOff(pauseSingleWork.sprId[i+12]);
            }
            espScaleSet(pauseSingleWork.sprId[i+12], scale, scale);
        }
    }
    for(j=0; j<14; j++) {
        espKill(pauseSingleWork.sprId[j]);
        pauseSingleWork.sprId[j] = -1;
    }
    for(j=0; j<2; j++) {
        mbWinKill(pauseSingleWork.win[j]);
        pauseSingleWork.win[j] = -1;
    }
    mbWinKill(pauseSingleWork.winName);
    pauseSingleWork.winName = -1;
    mbWinKill(pauseSingleWork.winBlank);
    pauseSingleWork.winBlank = -1;
    return result;
}
