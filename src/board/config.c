#include "game/board/config.h"
#include "game/board/audio.h"
#include "game/board/math.h"
#include "game/board/object.h"
#include "game/board/single.h"
#include "game/board/telop.h"
#include "game/board/window.h"
#include "game/board/wipe.h"

#include "game/gamework.h"
#include "game/flag.h"
#include "game/esprite.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/saveload.h"
#include "game/mic.h"
#include "game/object.h"
#include "game/board/malloc.h"
#include "game/board/player.h"
#include "game/sprite.h"
#include "game/memory.h"
#include "game/data.h"
#include "game/hu3d.h"
#include "stdlib.h"
#include "string.h"

#define CONFIG_PANEL_MAX 8
#define CONFIG_PAD_MAX 4

typedef struct ConfigPanel_s {
    /* 0x00 */ int no;
    /* 0x04 */ int value;
    /* 0x08 */ int valueNum;
    /* 0x0C */ s16 panelId;
    /* 0x10 */ BOOL closeF;
    /* 0x14 */ BOOL enableF;
} CONFIG_PANEL; /* size 0x18 */

typedef struct ConfigPad_s {
    /* 0x00 */ int padNo;
    /* 0x04 */ int playerNo;
    /* 0x08 */ int comDif;
    /* 0x0C */ BOOL padF;
    /* 0x10 */ HuVecF ofs;
    /* 0x1C */ s16 panelId;
    /* 0x1E */ s16 sprId[3];
} CONFIG_PAD; /* size 0x24 */

typedef struct PauseCursor_s {
    HuVecF pos; /* 0x00 */
    HuVecF startPos; /* 0x0C */
    HuVecF delta; /* 0x18 */
    BOOL dispF; /* 0x24 */
    int type; /* 0x28 */
    int hiliteMask; /* 0x2C */
    int mask; /* 0x30 */
    int time; /* 0x34 */
    int maxTime; /* 0x38 */
    float tpLvl[4]; /* 0x3C */
    s16 sprId[4]; /* 0x4C */
    s16 hiliteSprId[4]; /* 0x54 */
} PAUSE_CURSOR; /* size 0x5C */

typedef struct PausePlayer_s {
    int padNo; /* 0x00 */
    int playerNo; /* 0x04 */
    int padStat; /* 0x08 */
    int statTime; /* 0x0C */
} PAUSE_PLAYER; /* size 0x10 */

typedef struct PauseWork_s {
    /* 0x000 */ int playerNo;
    /* 0x004 */ int cursorNo;
    /* 0x008 */ int padCursor;
    /* 0x00C */ int padWait;
    /* 0x010 */ int decideF;
    /* 0x014 */ BOOL activeF;
    /* 0x018 */ BOOL exitF;
    /* 0x01C */ int padWinId;
    /* 0x020 */ int winId;
    /* 0x024 */ u8 unk24[0x3C - 0x24];
    /* 0x03C */ int talkTime;
    /* 0x040 */ int talkTimePrev;
    /* 0x044 */ CONFIG_PANEL panel[CONFIG_PANEL_MAX];
    /* 0x104 */ CONFIG_PAD pad[CONFIG_PAD_MAX];
    /* 0x194 */ PAUSE_CURSOR cursor;
    /* 0x1F0 */ PAUSE_PLAYER player[GW_PLAYER_MAX];
} PAUSE_WORK; /* size 0x230 */

typedef struct PausePanel_s {
    int objId; /* 0x00 */
    int labelObjId; /* 0x04 */
    int sprId; /* 0x08 */
    ANIMDATA *anim; /* 0x0C */
    s16 animId[2]; /* 0x10 */
    HuVecF pos; /* 0x14 */
    HuVecF startPos; /* 0x20 */
    HuVecF endPos; /* 0x2C */
    BOOL batsuF; /* 0x38 */
    float scale; /* 0x3C */
    float scaleStart; /* 0x40 */
    float scaleEnd; /* 0x44 */
    float baseScale; /* 0x48 */
    s16 bank; /* 0x4C */
    s16 mode; /* 0x4E */
    s16 time; /* 0x50 */
    s16 maxTime; /* 0x52 */
    s16 delay; /* 0x54 */
    s16 anmTime; /* 0x56 */
    s16 anmMaxTime; /* 0x58 */
} PAUSE_PANEL; /* size 0x5C */

#define PAUSE_PANEL_MAX 20

static BOOL configDoneF;
static int configResult;
static s16 pausePlayer;
static void *pauseDispCopyFb;
static int pauseDispCopyCounter;
static int pauseDispCopyModelId;
static BOOL pauseGuideKillF;
static PAUSE_PANEL *pausePanelWork;
static HUPROCESS *pauseGuideProc;
static BOOL configPadDisable;
static HUPROCESS *configProc;

static HuVecF playerPos;
static PAUSE_WORK pauseWork;
static BOOL playerDispF[GW_PLAYER_MAX];

static int mesSpeedAnmNo[4] = { 2, 1, 0, 0 };
static HuVecF pauseGuidePos = { -0.7f, -0.75f, -750.0f };
static HuVecF pauseGuideQuitPos = { 0.0f, -0.75f, -500.0f };
static int pauseMicValueTbl[3] = { 1, 0, 2 };
static s16 pauseGridPosTbl[CONFIG_PANEL_MAX][2] = {
    { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 },
    { 0, 1 }, { 1, 1 }, { 2, 1 }, { 3, 1 }
};
static int pausePanelFileTbl[CONFIG_PANEL_MAX] = {
    0x80003, 0x80004, 0x80005, 0x80006, 0x80007, 0x80008, 0x80009, 0x8000A
};
static int pausePanelLabelFileTbl[CONFIG_PANEL_MAX] = {
    0x8000B, 0x8000C, 0x8000D, 0x8000E, 0x8000F, 0x80010, 0x80011, 0x80012
};
static s16 pauseValueNumTbl[CONFIG_PANEL_MAX] = { 1, 2, 2, 5, 2, 3, 3, 1 };
static s16 pauseBatsuValueTbl[CONFIG_PANEL_MAX] = { -1, 1, 1, -1, 1, -1, 1, -1 };
static u32 pauseWinMesTbl[CONFIG_PANEL_MAX][5] = {
    { 0x280000, 0x280001, 0x280002, 0, 0 },
    { 0x280003, 0x280004, 0, 0, 0 },
    { 0x280005, 0x280006, 0, 0, 0 },
    { 0x280007, 0x280008, 0x280009, 0x28000A, 0x280017 },
    { 0x28000B, 0x28000C, 0, 0, 0 },
    { 0x28000F, 0x28000E, 0x28000D, 0, 0 },
    { 0x280010, 0x280011, 0x280012, 0, 0 },
    { 0x280013, 0x280028, 0, 0, 0 }
};
static u32 pauseSingleWinMesTbl[CONFIG_PANEL_MAX][5] = {
    { 0x28002A, 0x280001, 0x280002, 0, 0 },
    { 0x280003, 0x280004, 0, 0, 0 },
    { 0x280005, 0x28002B, 0, 0, 0 },
    { 0x280007, 0x280008, 0x280009, 0x28000A, 0x280017 },
    { 0x28000B, 0x28000C, 0, 0, 0 },
    { 0x28000F, 0x28000E, 0x28000D, 0, 0 },
    { 0x280010, 0x280011, 0x280012, 0, 0 },
    { 0x280028, 0x280028, 0, 0, 0 }
};
static HuVecF pauseGuidePos2 = { -0.7f, -0.75f, -750.0f };

static void ConfigKill(void);
static void ConfigMain(void);
static void ConfigOpen(void);
static void ConfigGrowWait(void);
static void ConfigClose(int type);
static void ConfigExec(void);
static BOOL ConfigQuitExec(void);
static void ConfigWinUpdate(int cursor);
static void ConfigPadWinSet(int no);
static void ConfigSettingRead(void);
static void ConfigSettingWrite(void);
static void ConfigPadOpen(void);
static void ConfigPadSprSet(CONFIG_PAD *pad);
static void ConfigPadTimeSet(CONFIG_PAD *pad, float scale);
static void ConfigPadClose(void);
static BOOL ConfigPadMain(void);
static void PauseCursorCreate(void);
static void PauseCursorKill(void);
static void PauseCursorHiliteSet(int type, int mask, int dStk);
static void PauseCursorPosNextSet(int time, float posX, float posY);
static int PauseCursorMove(void);
static int PausePlayerComRead(int flag);
static int PausePadCheck(int padNo);

static inline BOOL GWStorySingleCheck(void);

static inline int GWMgPackGet(void)
{
    if ((u8)GwSystem.mgPack >= 5) {
        GwSystem.mgPack = 0;
    }
    return GwSystem.mgPack;
}

static inline void GWMgPackSet(int mgPack)
{
    GwSystem.mgPack = mgPack;
}

static void ConfigPanelPosGet(int no, HuVecF *pos)
{
    pos->x = 0.4f * (pauseGridPosTbl[no][0] - 1.5f);
    pos->y = 0.3f + (-0.5f * (pauseGridPosTbl[no][1] - 0.5f));
    pos->z = 0.0f;
}

static void ConfigPadPosGet(int no, HuVecF *pos)
{
    pos->x = (0.31f * no) - 0.225f;
    pos->y = 0.15f;
    pos->z = 0.0f;
}

int mbConfigExec(int playerNo, s16 guideObjId)
{
    pausePlayer = guideObjId;
    mbObjPosGet(pausePlayer, &playerPos);
    configDoneF = FALSE;
    configResult = FALSE;
    memset(&pauseWork, 0, sizeof(PAUSE_WORK));
    pauseWork.padWinId = pauseWork.winId = MBWIN_NONE;
    pauseWork.playerNo = playerNo;
    pauseWork.cursorNo = -1;
    pauseWork.talkTime = pauseWork.talkTimePrev = FALSE;
    ConfigSettingRead();
    configProc = HuPrcChildCreate(ConfigMain, 0x2012, 0x3800, 0, mbMainProc);
    HuPrcSetStat(configProc, 12);
    HuPrcDestructorSet2(configProc, ConfigKill);
    while (!configDoneF) {
        HuPrcVSleep();
    }
    ConfigSettingWrite();
    return configResult;
}

static void ConfigKill(void)
{
    CONFIG_PANEL *panel;
    int i;
    for (panel = &pauseWork.panel[0], i = 0; i < CONFIG_PANEL_MAX; i++, panel++) {
        if (panel->panelId != 0) {
            mbPausePanelKill((s16)panel->panelId);
            panel->panelId = 0;
        }
    }
    configProc = NULL;
    configDoneF = TRUE;
}

static void ConfigMain(void)
{
    int i;
    PausePlayerComRead(TRUE);
    ConfigOpen();
    ConfigExec();
    if (pauseWork.winId >= 0) {
        mbWinKill(pauseWork.winId);
        pauseWork.winId = MBWIN_NONE;
    }
    ConfigPadWinSet(-1);
    for (i = 0; i < CONFIG_PANEL_MAX; i++) {
        pauseWork.panel[i].closeF = TRUE;
    }
    ConfigClose(2);
    PauseCursorKill();
    pauseWork.activeF = FALSE;
    HuPrcEnd();
}

static void ConfigOpen(void)
{
    CONFIG_PANEL *panel;
    int i;
    HuVecF pos;
    for (panel = &pauseWork.panel[0], i = 0; i < CONFIG_PANEL_MAX; i++, panel++) {
        panel->panelId = 0;
    }
    pauseWork.decideF = FALSE;
    for (panel = &pauseWork.panel[0], i = 0; i < CONFIG_PANEL_MAX; i++, panel++) {
        panel->no = i;
        panel->panelId = mbPausePanelCreate(pausePanelFileTbl[i], pausePanelLabelFileTbl[i]);
        if (!panel->enableF || panel->value == pauseBatsuValueTbl[i]) {
            mbPausePanelBatsuSet((s16)panel->panelId, TRUE);
        }
        ConfigPanelPosGet(i, &pos);
        mbPausePanelPosSet((s16)panel->panelId, pos.x, pos.y);
        mbPausePanelGrowSet((s16)panel->panelId, 16, i * 2, 1.0f);
        panel->valueNum = pauseValueNumTbl[i];
        mbPausePanelBankSet((s16)panel->panelId, panel->value);
    }
    PauseCursorCreate();
    mbPauseGuideMoveSet(pausePlayer, 20, NULL, &pauseGuidePos);
    ConfigGrowWait();
}

static void ConfigGrowWait(void)
{
    CONFIG_PANEL *panel;
    int i;
    BOOL doneF = FALSE;
    do {
        doneF = TRUE;
        for (panel = &pauseWork.panel[0], i = 0; i < CONFIG_PANEL_MAX; i++, panel++) {
            if (panel->panelId != 0 && !mbPausePanelFreezeGet((s16)panel->panelId)) {
                doneF = FALSE;
            }
        }
        if (!doneF) {
            HuPrcVSleep();
        }
    } while (!doneF);
}

static void ConfigClose(int type)
{
    int unused = 0;
    CONFIG_PANEL *panel;
    int i;
    HuVecF pos;
    for (panel = &pauseWork.panel[0], i = 0; i < CONFIG_PANEL_MAX; i++, panel++) {
        if (panel->closeF && panel->panelId != 0) {
            ConfigPanelPosGet(i, &pos);
            mbPausePanelPosSet((s16)panel->panelId, pos.x, pos.y);
            mbPausePanelShrinkSet((s16)panel->panelId, 16, 0);
        }
    }
    if (type != 0) {
        if (type == 1) {
            mbPauseGuideMoveSet(pausePlayer, 20, NULL, &pauseGuideQuitPos);
        } else {
            mbPauseGuideMoveSet(pausePlayer, 20, &playerPos, NULL);
        }
    }
    ConfigGrowWait();
    for (panel = &pauseWork.panel[0], i = 0; i < CONFIG_PANEL_MAX; i++, panel++) {
        if (panel->closeF && panel->panelId != 0) {
            mbPausePanelKill((s16)panel->panelId);
            panel->panelId = 0;
            panel->closeF = FALSE;
        }
    }
}

static inline void ConfigReopen(void)
{
    CONFIG_PANEL *panel;
    int i;
    HuVecF pos;
    for (panel = &pauseWork.panel[0], i = 0; i < CONFIG_PANEL_MAX; i++, panel++) {
        if (panel->panelId <= 0) {
            panel->panelId = mbPausePanelCreate(pausePanelFileTbl[i], pausePanelLabelFileTbl[i]);
            if (!panel->enableF || panel->value == pauseBatsuValueTbl[i]) {
                mbPausePanelBatsuSet((s16)panel->panelId, TRUE);
            }
            ConfigPanelPosGet(i, &pos);
            mbPausePanelPosSet((s16)panel->panelId, pos.x, pos.y);
            mbPausePanelGrowSet((s16)panel->panelId, 16, i * 2, 1.0f);
            mbPausePanelBankSet((s16)panel->panelId, panel->value);
        }
    }
    mbPauseGuideMoveSet(pausePlayer, 20, NULL, &pauseGuidePos);
    ConfigGrowWait();
}

static inline void ConfigPanelValueAdd(int cursor)
{
    int valuePrev;
    if (pauseWork.panel[cursor].valueNum > 1) {
        valuePrev = pauseWork.panel[cursor].value;
        pauseWork.panel[cursor].value++;
        if (pauseWork.panel[cursor].value >= pauseWork.panel[cursor].valueNum) {
            pauseWork.panel[cursor].value = 0;
        }
        if (pauseWork.panel[cursor].no == 6 && pauseWork.panel[cursor].value == 0 && HuMCProbe(1)) {
            pauseWork.panel[cursor].value = 1;
        }
        mbPausePanelAnmNoSet((s16)pauseWork.panel[cursor].panelId, 12, pauseWork.panel[cursor].value);
        if (pauseWork.panel[cursor].value == pauseBatsuValueTbl[pauseWork.panel[cursor].no]) {
            mbPausePanelBatsuSet((s16)pauseWork.panel[cursor].panelId, TRUE);
        } else {
            mbPausePanelBatsuSet((s16)pauseWork.panel[cursor].panelId, FALSE);
        }
    }
}

static void ConfigExec(void)
{
    int dirMaskTbl[CONFIG_PANEL_MAX] = { 6, 7, 7, 5, 10, 11, 11, 9 };
    CONFIG_PANEL *panel;
    HuVecF pos;
    int i;
    int dStk;
    int padNo;
    int cursor;
    int cursorPrev;
    int move;
    int unk40;
    BOOL padStatF;

    ConfigPadWinSet(0);
    pauseWork.activeF = TRUE;
    cursorPrev = -1;
    while (!pauseWork.exitF) {
        HuPrcVSleep();
        mbPauseGuideTalkSet(pausePlayer);
        padNo = GwPlayer[pauseWork.playerNo].padNo;
        padStatF = HuPadStatGet(padNo) != 0;
        move = PauseCursorMove();
        if (move != 0 && cursorPrev >= 0) {
            if ((HuPadBtnDown[padNo] & PAD_BUTTON_A) && !padStatF && move < 10) {
                pauseWork.decideF = TRUE;
            }
            continue;
        }
        cursorPrev = cursor = pauseWork.cursorNo;
        if (cursor < 0) {
            cursor = 0;
        }
        if (!pauseWork.decideF) {
            dStk = HuPadDStk[padNo] & dirMaskTbl[cursor];
            if (padStatF) {
                dStk = 0;
            }
            if (cursorPrev >= 0) {
                if (dStk & PAD_BUTTON_UP) {
                    mbAudFXPlay(0);
                    cursor = (cursor + 4) & 7;
                } else if (dStk & PAD_BUTTON_DOWN) {
                    mbAudFXPlay(0);
                    cursor = (cursor - 4) & 7;
                }
                if (dStk & PAD_BUTTON_RIGHT) {
                    mbAudFXPlay(0);
                    cursor = (cursor + 1) & 7;
                } else if (dStk & PAD_BUTTON_LEFT) {
                    mbAudFXPlay(0);
                    cursor = (cursor - 1) & 7;
                }
            }
            pauseWork.cursorNo = cursor;
            PauseCursorHiliteSet(0, dirMaskTbl[cursor], HuPadDStk[padNo]);
            if (cursorPrev != cursor) {
                ConfigPanelPosGet(cursor, &pos);
                PauseCursorPosNextSet(12, pos.x, pos.y);
                if (cursorPrev >= 0) {
                    mbPausePanelSizeSet((s16)pauseWork.panel[cursorPrev].panelId, 12, 1.0f);
                }
                if (cursor >= 0) {
                    mbPausePanelSizeSet((s16)pauseWork.panel[cursor].panelId, 12, 1.2f);
                }
                ConfigWinUpdate(cursor);
            }
        }
        unk40 = 0;
        if ((HuPadBtnDown[padNo] & PAD_BUTTON_A) && !padStatF) {
            pauseWork.decideF = TRUE;
        }
        if (pauseWork.decideF && mbPausePanelFreezeGet((s16)pauseWork.panel[cursor].panelId) != FALSE) {
            pauseWork.decideF = FALSE;
            if (!pauseWork.panel[cursor].enableF) {
                mbAudFXPlay(4);
            } else if (cursor == 0) {
                mbAudFXPlay(2);
                pos.x = -0.225f;
                pos.y = 0.15f;
                pos.z = 0.0f;
                pos.x += -0.425f;
                mbPausePanelSlideSet((s16)pauseWork.panel[0].panelId, 16, &pos);
                for (move = 1; move < CONFIG_PANEL_MAX; move++) {
                    pauseWork.panel[move].closeF = TRUE;
                }
                PauseCursorHiliteSet(-1, 0, 0);
                ConfigClose(0);
                ConfigPadOpen();
                while (!ConfigPadMain()) {
                    HuPrcVSleep();
                    mbPauseGuideTalkSet(pausePlayer);
                }
                mbAudFXPlay(3);
                PauseCursorHiliteSet(-1, 0, 0);
                ConfigPadClose();
                pauseWork.decideF = FALSE;
                ConfigReopen();
                pauseWork.cursorNo = -1;
                HuPrcVSleep();
            } else if (cursor == 7) {
                mbAudFXPlay(1);
                if (ConfigQuitExec()) {
                    mbWipeFadeOut();
                    _SetFlag(0x10021);
                    pauseWork.exitF = TRUE;
                } else {
                    pauseWork.decideF = FALSE;
                    ConfigReopen();
                    ConfigPadWinSet(0);
                    pauseWork.cursorNo = -1;
                }
            } else {
                mbAudFXPlay(1);
                ConfigPanelValueAdd(cursor);
                if (cursor == 5) {
                    GWMessSpeedSet(mesSpeedAnmNo[pauseWork.panel[5].value]);
                } else if (cursor == 4) {
                    GWVibrateSet(pauseWork.panel[4].value == 0);
                    omVibrate(pauseWork.playerNo, 20, 7, 3);
                } else if (cursor == 6) {
                    HuMCMicSet(pauseMicValueTbl[pauseWork.panel[6].value]);
                }
                ConfigWinUpdate(cursor);
            }
        }
        if ((HuPadBtnDown[padNo] & PAD_BUTTON_B) && !padStatF && !pauseWork.exitF) {
            pauseWork.talkTime = 0;
            pauseWork.talkTimePrev = TRUE;
            mbPauseGuideTalkSet(pausePlayer);
            pauseWork.exitF = TRUE;
            mbAudFXPlay(3);
            OSReport("-------------------- Config Exit! ----------------------\n");
        }
    }
    PauseCursorHiliteSet(-1, 0, 0);
}

static BOOL ConfigQuitExec(void)
{
    int i;
    int winId;
    int choice;
    MBWIN *winP;
    HuVecF pos;
    BOOL comF;
    int padStat;
    float ofs;

    for (i = 0; i < CONFIG_PANEL_MAX; i++) {
        pauseWork.panel[i].closeF = TRUE;
    }
    PauseCursorHiliteSet(-1, 0, 0);
    ConfigWinUpdate(-1);
    ConfigPadWinSet(-1);
    ConfigClose(1);
    mbObjMotionShiftSet(pausePlayer, 11, 0.0f, 8.0f, 0x40000001);
    comF = GwPlayer[pauseWork.playerNo].comF;
    GwPlayer[pauseWork.playerNo].comF = FALSE;
    GwPlayerConf[pauseWork.playerNo].type = 0;
    padStat = HuPadStatGet(GwPlayer[pauseWork.playerNo].padNo);
    if (GWPartyGet() != FALSE) {
        if (SLSaveFlagGet() && saveExecF) {
            winId = mbWinCreate(3, 0x280019, -1);
            mbWinPriSet(winId, 100);
            mbWinPlayerDisable(winId, pauseWork.playerNo);
            mbWinMesSpeedSet(winId, 0);
            winP = mbWinGet(winId);
            winP->size.x = 400.0f;
            mbWinCenterGet(winId, (HuVec2f *)&pos);
            mbWinPosSet(winId, pos.x, 100);
            mbWinAttrReset(winId, 0x10);
            mbWinWait(winId);
        }
        winId = mbWinCreateChoice(3, 0x280014, -1, 0);
    } else {
        winId = mbWinCreateChoice(3, 0x280029, -1, 0);
    }
    mbWinPriSet(winId, 100);
    mbWinPlayerDisable(winId, pauseWork.playerNo);
    mbWinMesSpeedSet(winId, 0);
    winP = mbWinGet(winId);
    winP->size.x = 400.0f;
    mbWinCenterGet(winId, (HuVec2f *)&pos);
    mbWinPosSet(winId, pos.x, 100);
    if (GWPartyGet() != FALSE) {
        ConfigPadWinSet(2);
    } else {
        ConfigPadWinSet(3);
    }
    if (padStat == 0) {
        mbWinAttrReset(winId, 0x10);
        mbWinWait(winId);
        choice = mbWinChoiceGet(winId);
        ConfigPadWinSet(-1);
        if (choice > 0) {
            mbObjMotionShiftSet(pausePlayer, 7, 0.0f, 12.0f, 0x40000001);
            for (i = 0; i < 30; i++) {
                HuPrcVSleep();
            }
            GWPartyGet() == FALSE;
        } else if (choice == 0) {
            mbObjPosGet(pausePlayer, &pos);
            mbObjMotionShiftSet(pausePlayer, 23, 0.0f, 8.0f, 0);
            mbAudGuidePlay(950);
            for (i = 0; i < 10; i++) {
                HuPrcVSleep();
            }
            for (i = 0; i <= 30; i++) {
                ofs = (1.0f / 30.0f) * i;
                ofs = 100.0f * (0.5f * mbSinDeg(180.0f * ofs));
                mbObjPosSet(pausePlayer, pos.x, pos.y + ofs, pos.z);
                HuPrcVSleep();
            }
        }
    } else {
        choice = 0;
        mbWinKill(winId);
        ConfigPadWinSet(-1);
    }
    GwPlayer[pauseWork.playerNo].comF = comF;
    GwPlayerConf[pauseWork.playerNo].type = comF;
    configResult = FALSE;
    if (choice > 0) {
        configResult = TRUE;
        if (GWPartyGet() == FALSE) {
            mbSingleSaveFlush((choice > 1) ? -1 : 0);
        }
    }
    return configResult;
}

static void ConfigWinUpdate(int cursor)
{
    int i;
    u32 *mesTbl;
    int mesNo;
    HuVec2f pos;
    if (pauseWork.winId >= 0) {
        mbWinKill(pauseWork.winId);
        pauseWork.winId = MBWIN_NONE;
    }
    if (cursor >= 0) {
        if (GWPartyGet() != FALSE) {
            mesTbl = pauseWinMesTbl[cursor];
        } else {
            mesTbl = pauseSingleWinMesTbl[cursor];
        }
        if (cursor == 0) {
            pauseWork.winId = mbWinCreateTime(8, mesTbl[0], -1);
            for (i = 0; i < GW_PLAYER_MAX; i++) {
                if (GwPlayer[pauseWork.player[i].playerNo].comF) {
                    mesNo = 2;
                } else {
                    mesNo = 1;
                }
                mbWinInsertMesSet(pauseWork.winId, pauseWinMesTbl[cursor][mesNo], i);
            }
        } else {
            pauseWork.winId = mbWinCreateTime(8, mesTbl[pauseWork.panel[cursor].value], -1);
        }
        mbWinPosGet(pauseWork.winId, &pos);
        mbWinPosSet(pauseWork.winId, 144, pos.y);
        mbWinPause(pauseWork.winId);
        pauseWork.talkTime = 30;
    }
}

static u32 pausePadWinMesTbl[4] = { 0x280015, 0x280016, 0x280016, 0x280016 };
static HuVec2f pausePadWinPosTbl[4] = {
    { 64.0f, 288.0f },
    { 64.0f, 288.0f },
    { 0.0f, 196.0f },
    { 0.0f, 212.0f }
};

static void ConfigPadWinSet(int no)
{
    HuVec2f pos;
    if (pauseWork.padWinId >= 0) {
        mbWinKill(pauseWork.padWinId);
        pauseWork.padWinId = MBWIN_NONE;
    }
    if (no >= 0) {
        pauseWork.padWinId = mbWinCreateHelp(pausePadWinMesTbl[no]);
        mbWinCenterGet(pauseWork.padWinId, &pos);
        pos.x += pausePadWinPosTbl[no].x;
        pos.y = pausePadWinPosTbl[no].y;
        mbWinPosSet(pauseWork.padWinId, pos.x, pos.y);
    }
}

static void ConfigSettingRead(void)
{
    int mic;
    int i;
    pauseWork.panel[0].value = 0;
    pauseWork.panel[1].value = (GWMgInstDispGet() != FALSE) ? 0 : 1;
    pauseWork.panel[2].value = (GWMgComDispGet() != FALSE) ? 0 : 1;
    pauseWork.panel[3].value = GWMgPackGet();
    pauseWork.panel[4].value = (GWVibrateGet() != FALSE) ? 0 : 1;
    pauseWork.panel[5].value = mesSpeedAnmNo[GWMessSpeedGet()];
    mic = HuMCMicGet();
    for (i = 0; i < 3; i++) {
        if (mic == pauseMicValueTbl[i]) {
            break;
        }
    }
    if (i >= 3) {
        i = 0;
    }
    pauseWork.panel[6].value = i;
    if (i == 0 && HuMCProbe(1)) {
        pauseWork.panel[6].value = 1;
    }
    for (i = 0; i < CONFIG_PANEL_MAX; i++) {
        pauseWork.panel[i].enableF = TRUE;
    }
    if (GWPartyGet() == FALSE) {
        pauseWork.panel[0].enableF = FALSE;
        pauseWork.panel[2].value = 1;
        pauseWork.panel[2].enableF = FALSE;
    }
    if (!configPadDisable) {
        pauseWork.panel[0].enableF = FALSE;
    }
}

static void ConfigSettingWrite(void)
{
    GWMgInstDispSet(pauseWork.panel[1].value == 0);
    if (GWPartyGet() != FALSE) {
        GWMgComDispSet(pauseWork.panel[2].value == 0);
    }
    GWMgPackSet(pauseWork.panel[3].value);
    GWVibrateSet(pauseWork.panel[4].value == 0);
    GWMessSpeedSet(mesSpeedAnmNo[pauseWork.panel[5].value]);
    if (pauseWork.panel[6].value == 0 && HuMCProbe(1)) {
        pauseWork.panel[6].value = 1;
    }
    HuMCMicSet(pauseMicValueTbl[pauseWork.panel[6].value]);
}

static int pauseCharPanelFileTbl[14] = {
    0x80014, 0x80015, 0x80016, 0x80017, 0x80018, 0x80019, 0x8001A,
    0x8001B, 0x8001C, 0x8001D, 0x8001E, 0x8001D, 0x8001D, 0x8001D
};
static int configPadSprFileTbl[3] = { 0x80013, 0x8001F, 0x8001F };
static s16 configPadSprOfsTbl[3][2] = {
    { 0, 0 },
    { 20, -30 },
    { 42, -70 }
};

static void ConfigPadOpen(void)
{
    CONFIG_PAD *pad;
    int i;
    int j;
    int sprId;
    HuVecF pos;
    float t;
    int unkC;
    float stepX;
    int time[CONFIG_PAD_MAX];
    int doneNum;

    pauseWork.padCursor = -1;
    pauseWork.decideF = FALSE;
    PausePlayerComRead(FALSE);
    for (pad = &pauseWork.pad[0], i = 0; i < CONFIG_PAD_MAX; i++, pad++) {
        for (j = 0; j < 3; j++) {
            pad->sprId[j] = -1;
        }
        pad->playerNo = pauseWork.player[i].playerNo;
        pad->padNo = pauseWork.player[i].padNo;
    }
    pos.x = -0.225f;
    pos.y = 0.15f;
    pos.z = 0.0f;
    t = pos.x;
    pos.x = 0.395f;
    pos.y = 0.15f;
    pos.z = 0.0f;
    stepX = (1.0f / 3.0f) * (pos.x - t);
    for (pad = &pauseWork.pad[0], unkC = -1, i = 0; i < CONFIG_PAD_MAX; i++, pad++) {
        pad->comDif = GwPlayer[pad->playerNo].comDif;
        pad->padF = PausePadCheck(pad->padNo);
        if (!GwPlayer[pad->playerNo].comF) {
            pad->comDif = 4;
        }
        pad->panelId = mbPausePanelCreate(pauseCharPanelFileTbl[GwPlayer[pad->playerNo].charNo], 0);
        ConfigPadPosGet(i, &pos);
        mbPausePanelPosSet((s16)pad->panelId, pos.x, pos.y);
        mbPausePanelGrowSet((s16)pad->panelId, 16, i * 4, 0.95f);
        mbNormPosto2D(&pos, &pos);
        pad->ofs.x = -22.0f;
        pad->ofs.y = -30.0f;
        pad->ofs.z = 0.0f;
        PSVECAdd(&pos, &pad->ofs, &pos);
        for (j = 0; j < 3; j++) {
            sprId = espEntry(mbBoardDataNumGet(configPadSprFileTbl[j]), 100 - j, 0);
            pad->sprId[j] = sprId;
            espAttrSet(sprId, 8);
            espPosSet(sprId, pos.x + configPadSprOfsTbl[j][0], pos.y + configPadSprOfsTbl[j][1]);
        }
        ConfigPadSprSet(pad);
        ConfigPadTimeSet(pad, 0.0f);
    }
    for (i = 0; i < CONFIG_PAD_MAX; i++) {
        time[i] = 0;
    }
    doneNum = 0;
    while (doneNum < CONFIG_PAD_MAX) {
        doneNum = 0;
        for (pad = &pauseWork.pad[0], i = 0; i < CONFIG_PAD_MAX; i++, pad++) {
            if (mbPausePanelFreezeGet((s16)pad->panelId)) {
                time[i]++;
                t = (1.0f / 12.0f) * time[i];
                if (t > 1.0f) {
                    t = 1.0f;
                    doneNum++;
                }
                t = mbSinDeg(90.0f * t);
                ConfigPadTimeSet(pad, t);
            }
        }
        HuPrcVSleep();
    }
}

static void ConfigPadSprSet(CONFIG_PAD *pad)
{
    int sprId;
    int bank;
    sprId = pad->sprId[0];
    bank = 0;
    if (pad->comDif < 4) {
        bank = 1;
    }
    espBankSet(sprId, bank);
    sprId = pad->sprId[1];
    bank = pad->padNo;
    if (pad->comDif < 4) {
        bank = 4;
    }
    espBankSet(sprId, bank);
    sprId = pad->sprId[2];
    if (pad->comDif < 4) {
        espDispOn(sprId);
        espBankSet(sprId, pad->comDif + 5);
    } else {
        espDispOff(sprId);
    }
}

static void ConfigPadTimeSet(CONFIG_PAD *pad, float scale)
{
    HuVecF pos;
    float panelScale;
    int sprId;
    mbPausePanelPosGet((s16)pad->panelId, &pos);
    mbNormPosto2D(&pos, &pos);
    panelScale = mbPausePanelScaleGet((s16)pad->panelId);
    pos.x += pad->ofs.x * panelScale;
    pos.y += pad->ofs.y * panelScale;
    sprId = pad->sprId[0];
    espScaleSet(sprId, scale, scale);
    espPosSet(sprId, pos.x + (configPadSprOfsTbl[0][0] * scale), pos.y + (configPadSprOfsTbl[0][1] * scale));
    sprId = pad->sprId[1];
    espScaleSet(sprId, scale, scale);
    espPosSet(sprId, pos.x + (configPadSprOfsTbl[1][0] * scale), pos.y + (configPadSprOfsTbl[1][1] * scale));
    sprId = pad->sprId[2];
    espScaleSet(sprId, scale, scale);
    espPosSet(sprId, pos.x + (configPadSprOfsTbl[2][0] * scale), pos.y + (configPadSprOfsTbl[2][1] * scale));
}

static void ConfigPadClose(void)
{
    CONFIG_PANEL *panel;
    CONFIG_PAD *pad;
    int no;
    int i;
    int j;
    HuVecF pos;
    float t;
    float stepX;

    panel = &pauseWork.panel[0];
    if (panel->panelId != 0) {
        no = panel->no;
        ConfigPanelPosGet(no, &pos);
        mbPausePanelSlideSet((s16)panel->panelId, 20, &pos);
        mbPausePanelSizeSet((s16)panel->panelId, 20, 1.2f);
    }
    pos.x = -0.225f;
    pos.y = 0.15f;
    pos.z = 0.0f;
    t = pos.x;
    pos.x = 0.395f;
    pos.y = 0.15f;
    pos.z = 0.0f;
    stepX = (1.0f / 3.0f) * (pos.x - t);
    for (i = 0; i < CONFIG_PAD_MAX; i++) {
        mbPausePanelShrinkSet((s16)pauseWork.pad[i].panelId, 20, 0);
    }
    for (i = 0; !mbPausePanelFreezeGet((s16)pauseWork.pad[0].panelId); i++) {
        HuPrcVSleep();
        t = (1.0f / 12.0f) * i;
        if (t > 1.0f) {
            t = 1.0f;
        }
        t = mbCosDeg(90.0f * t);
        for (pad = &pauseWork.pad[0], j = 0; j < CONFIG_PAD_MAX; j++, pad++) {
            ConfigPadTimeSet(pad, t);
        }
    }
    for (pad = &pauseWork.pad[0], i = 0; i < CONFIG_PAD_MAX; i++, pad++) {
        if (pad->panelId != 0) {
            mbPausePanelKill((s16)pad->panelId);
            pad->panelId = 0;
        }
        for (j = 0; j < 3; j++) {
            if (pad->sprId[j] >= 0) {
                espKill(pad->sprId[j]);
                pad->sprId[j] = -1;
            }
        }
    }
}

static BOOL ConfigPadMain(void)
{
    HuVecF pos;
    int cursorPrev;
    int dStk;
    BOOL result = FALSE;
    int dirMaskTbl[CONFIG_PAD_MAX] = { 2, 3, 3, 1 };
    BOOL updateF;
    CONFIG_PAD *pad;
    int padNo;
    BOOL padOffF;
    int cursor;
    int max;
    int i;

    padNo = GwPlayer[pauseWork.playerNo].padNo;
    updateF = PausePlayerComRead(FALSE);
    padOffF = !PausePadCheck(padNo);
    if (PauseCursorMove()) {
        if (HuPadBtnDown[padNo] & PAD_BUTTON_A) {
            pauseWork.decideF = 12;
        }
        if (padOffF) {
            pauseWork.decideF = 0;
        }
        if (pauseWork.decideF) {
            pauseWork.decideF--;
        }
        pauseWork.padWait = 0;
        return FALSE;
    }
    cursorPrev = cursor = pauseWork.padCursor;
    if (cursor < 0) {
        cursor = 0;
        pauseWork.padCursor = cursor;
    }
    dStk = HuPadDStk[padNo] & dirMaskTbl[cursor];
    if (padOffF) {
        dStk = 0;
    }
    if (dStk & PAD_BUTTON_RIGHT) {
        mbAudFXPlay(0);
        cursor = (cursor + 1) & 3;
    } else if (dStk & PAD_BUTTON_LEFT) {
        mbAudFXPlay(0);
        cursor = (cursor - 1) & 3;
    }
    pauseWork.padCursor = cursor;
    PauseCursorHiliteSet(1, dirMaskTbl[cursor], HuPadDStk[padNo]);
    pad = &pauseWork.pad[pauseWork.padCursor];
    if (cursor != cursorPrev) {
        ConfigPadPosGet(cursor, &pos);
        PauseCursorPosNextSet(12, pos.x, pos.y);
    }
    if (cursorPrev >= 0) {
        pad = &pauseWork.pad[cursorPrev];
    }
    if (HuPadBtnDown[padNo] & PAD_BUTTON_A) {
        pauseWork.decideF = 8;
    }
    if (padOffF) {
        pauseWork.decideF = 0;
    }
    if (!pauseWork.padWait && pauseWork.decideF) {
        max = 4;
        pad->comDif++;
        pauseWork.padWait = 16;
        pauseWork.decideF = 0;
        mbAudFXPlay(1);
        if (!GWBankFlagGet(3) && pad->comDif == max - 1) {
            pad->comDif++;
        }
        if (pad->comDif > max) {
            pad->comDif = 0;
        }
        if (pad->comDif == max) {
            if (PausePadCheck(pad->padNo)) {
                GwPlayer[pad->playerNo].comF = FALSE;
                GwPlayerConf[pad->playerNo].type = 0;
            } else {
                pad->comDif = 0;
            }
        }
        if (pad->comDif < max) {
            GwPlayer[pad->playerNo].comF = TRUE;
            GwPlayerConf[pad->playerNo].type = 1;
            GwPlayer[pad->playerNo].comDif = pad->comDif;
            GwPlayerConf[pad->playerNo].comDif = pad->comDif;
        }
        updateF = TRUE;
        ConfigPadSprSet(pad);
        mbPausePanelAnmNoSet((s16)pad->panelId, 12, 0);
    }
    for (pad = &pauseWork.pad[0], i = 0; i < CONFIG_PAD_MAX; i++, pad++) {
        BOOL padF = PausePadCheck(pad->padNo);
        if (!padF && pad->padF) {
            GwPlayer[pad->playerNo].comF = TRUE;
            GwPlayerConf[pad->playerNo].type = 1;
            pad->comDif = GwPlayer[pad->playerNo].comDif;
            ConfigPadSprSet(pad);
        } else if (padF && !pad->padF) {
            pad->comDif = 4;
            GwPlayer[pad->playerNo].comF = FALSE;
            GwPlayerConf[pad->playerNo].type = 0;
            ConfigPadSprSet(pad);
        }
        pad->padF = padF;
    }
    if (updateF) {
        ConfigWinUpdate(pauseWork.cursorNo);
    }
    if ((HuPadBtnDown[padNo] & PAD_BUTTON_B) && !padOffF) {
        result = TRUE;
    }
    if (pauseWork.padWait) {
        pauseWork.padWait--;
    }
    if (pauseWork.decideF) {
        pauseWork.decideF--;
    }
    return result;
}

static s16 pauseCursorSprOfsTbl[2][4][2] = {
    { { 0, -62 }, { 0, 68 }, { 62, 0 }, { -62, 0 } },
    { { 0, -20 }, { 0, 20 }, { 43, -60 }, { -45, -60 } },
};

static int pauseCursorMaskTbl[4] = { 8, 4, 2, 1 };

static void PauseCursorCreate(void)
{
    int i;
    PAUSE_CURSOR *cursor = &pauseWork.cursor;
    int bankTbl[4] = { 1, 3, 2, 0 };
    int sprId;

    cursor->dispF = FALSE;
    cursor->hiliteMask = 0;
    cursor->mask = 0;
    cursor->type = -1;
    cursor->time = -1;
    cursor->maxTime = -1;
    cursor->pos.x = cursor->pos.y = cursor->pos.z = 0.0f;
    for (i = 0; i < 4; i++) {
        cursor->tpLvl[i] = 0.0f;
        cursor->sprId[i] = sprId = espEntry(mbBoardDataNumGet(0x80020), 90, bankTbl[i] + 4);
        espAttrSet(sprId, 8);
        espPosSet(sprId, cursor->pos.x, cursor->pos.y);
        espDispOff(sprId);
        cursor->hiliteSprId[i] = sprId = espEntry(mbBoardDataNumGet(0x80020), 89, bankTbl[i]);
        espAttrSet(sprId, 8);
        espPosSet(sprId, cursor->pos.x, cursor->pos.y);
        espDispOff(sprId);
    }
}

static void PauseCursorKill(void)
{
    PAUSE_CURSOR *cursor = &pauseWork.cursor;
    int i;

    for (i = 0; i < 4; i++) {
        if (cursor->sprId[i] >= 0) {
            espKill(cursor->sprId[i]);
        }
        if (cursor->hiliteSprId[i] >= 0) {
            espKill(cursor->hiliteSprId[i]);
        }
        cursor->sprId[i] = cursor->hiliteSprId[i] = -1;
    }
}

static void PauseCursorHiliteSet(int type, int mask, int hiliteMask)
{
    PAUSE_CURSOR *cursor = &pauseWork.cursor;
    int i;

    cursor->dispF = FALSE;
    if (type >= 0) {
        cursor->dispF = TRUE;
    }
    if (cursor->type < 0 && type >= 0) {
        cursor->time = -1;
        cursor->maxTime = -1;
    }
    cursor->hiliteMask = hiliteMask;
    cursor->mask = mask;
    cursor->type = type;
    for (i = 0; i < 4; i++) {
        espDispOff(cursor->hiliteSprId[i]);
        if (cursor->dispF) {
            espDispOn(cursor->sprId[i]);
            if (hiliteMask & pauseCursorMaskTbl[i]) {
                espDispOn(cursor->hiliteSprId[i]);
            }
        } else {
            espDispOff(cursor->sprId[i]);
        }
    }
}

static void PauseCursorPosNextSet(int time, float posX, float posY)
{
    PAUSE_CURSOR *cursor = &pauseWork.cursor;
    int i;
    HuVecF pos;

    if (cursor->time < 0 || time <= 0) {
        cursor->time = 0;
        cursor->maxTime = 0;
        cursor->pos.x = posX;
        cursor->pos.y = posY;
        for (i = 0; i < 4; i++) {
            cursor->tpLvl[i] = 0.0f;
            if (cursor->mask & pauseCursorMaskTbl[i]) {
                cursor->tpLvl[i] = 1.0f;
            }
        }
    } else {
        cursor->time = cursor->maxTime = time;
        mbVecCopy(&cursor->pos, &cursor->startPos);
        cursor->delta.x = posX - cursor->startPos.x;
        cursor->delta.y = posY - cursor->startPos.y;
        cursor->delta.z = 0.0f;
    }
    mbNormPosto2D(&cursor->pos, &pos);
    for (i = 0; i < 4; i++) {
        espPosSet(cursor->sprId[i], pos.x + pauseCursorSprOfsTbl[cursor->type][i][0], pos.y + pauseCursorSprOfsTbl[cursor->type][i][1]);
        espTPLvlSet(cursor->sprId[i], cursor->tpLvl[i]);
        espPosSet(cursor->hiliteSprId[i], pos.x + pauseCursorSprOfsTbl[cursor->type][i][0], pos.y + pauseCursorSprOfsTbl[cursor->type][i][1]);
        espTPLvlSet(cursor->hiliteSprId[i], cursor->tpLvl[i]);
    }
}

static int PauseCursorMove(void)
{
    PAUSE_CURSOR *cursor = &pauseWork.cursor;
    int i;
    float t;
    HuVecF pos;

    if (cursor->time <= 0) {
        return 0;
    }
    cursor->time--;
    t = (float)(cursor->maxTime - cursor->time) / cursor->maxTime;
    t = mbSinDeg(90.0f * t);
    PSVECScale(&cursor->delta, &pos, t);
    PSVECAdd(&cursor->startPos, &pos, &cursor->pos);
    for (i = 0; i < 4; i++) {
        if (cursor->mask & pauseCursorMaskTbl[i]) {
            if (cursor->tpLvl[i] < 1.0f) {
                cursor->tpLvl[i] += 0.1f;
            }
            if (cursor->tpLvl[i] > 1.0f) {
                cursor->tpLvl[i] = 1.0f;
            }
        } else {
            if (cursor->tpLvl[i] > 0.0f) {
                cursor->tpLvl[i] -= 0.1f;
            }
            if (cursor->tpLvl[i] < 0.0f) {
                cursor->tpLvl[i] = 0.0f;
            }
        }
    }
    mbNormPosto2D(&cursor->pos, &pos);
    for (i = 0; i < 4; i++) {
        espPosSet(cursor->sprId[i], pos.x + pauseCursorSprOfsTbl[cursor->type][i][0], pos.y + pauseCursorSprOfsTbl[cursor->type][i][1]);
        espTPLvlSet(cursor->sprId[i], cursor->tpLvl[i]);
        espPosSet(cursor->hiliteSprId[i], pos.x + pauseCursorSprOfsTbl[cursor->type][i][0], pos.y + pauseCursorSprOfsTbl[cursor->type][i][1]);
        espTPLvlSet(cursor->hiliteSprId[i], cursor->tpLvl[i]);
    }
    return cursor->time;
}

static BOOL PausePlayerComRead(BOOL initF)
{
    PAUSE_PLAYER *player = pauseWork.player;
    BOOL result = FALSE;
    int i;
    int j;
    int temp;

    if (initF) {
        for (i = 0; i < 4; i++) {
            player[i].playerNo = i;
            player[i].padNo = GwPlayer[i].padNo;
            player[i].padStat = 0;
            player[i].statTime = 0;
        }
        for (i = 0; i < 3; i++) {
            for (j = i + 1; j < 4; j++) {
                if (player[i].padNo > player[j].padNo || (player[i].padNo == player[j].padNo && player[i].playerNo > player[j].playerNo)) {
                    temp = player[i].padNo;
                    player[i].padNo = player[j].padNo;
                    player[j].padNo = temp;
                    temp = player[i].playerNo;
                    player[i].playerNo = player[j].playerNo;
                    player[j].playerNo = temp;
                }
            }
        }
        if (GWPartyGet() != FALSE) {
            for (i = 0; i < 4; i++) {
                player[i].padStat = HuPadStatGet(player[i].padNo);
                if (player[i].padStat != 0 && !GwPlayer[player[i].playerNo].comF) {
                    GwPlayer[player[i].playerNo].comF = TRUE;
                    GwPlayerConf[player[i].playerNo].type = 1;
                }
            }
        }
    }
    if (GWPartyGet() == FALSE) {
        return FALSE;
    }
    for (i = 0; i < 4; i++) {
        temp = HuPadStatGet(player[i].padNo);
        if (temp != player[i].padStat) {
            player[i].statTime++;
        } else {
            player[i].statTime = 0;
        }
        if (player[i].statTime >= 4) {
            if (temp == 0) {
                GwPlayer[player[i].playerNo].comF = FALSE;
                GwPlayerConf[player[i].playerNo].type = 0;
                result = TRUE;
                player[i].padStat = temp;
            } else if (temp == -1) {
                GwPlayer[player[i].playerNo].comF = TRUE;
                GwPlayerConf[player[i].playerNo].type = 1;
                result = TRUE;
                player[i].padStat = temp;
            }
        }
    }
    return result;
}

static BOOL PausePadCheck(int no)
{
    PAUSE_PLAYER *player = pauseWork.player;
    return player[no].padStat != -1;
}

static void PauseDispCopyDraw(HU3D_MODEL *modelP, Mtx *mtx);

void mbPauseDispCopyCreate(void)
{
    u32 size;
    int i;

    pauseDispCopyFb = NULL;
    pauseDispCopyCounter = 0;
    size = GXGetTexBufferSize(320, 240, GX_TF_RGB565, GX_FALSE, 0);
    if (!GWStorySingleCheck) {
        pauseDispCopyFb = mbMallocFlush(size);
    } else {
        pauseDispCopyFb = mbMallocFlushModel(size);
    }
    pauseDispCopyModelId = Hu3DHookFuncCreate(PauseDispCopyDraw);
    Hu3DModelCameraSet(pauseDispCopyModelId, 4);
    Hu3DModelLayerSet(pauseDispCopyModelId, 2);
    HuPrcVSleep();
    for (i = 0; i < 4; i++) {
        playerDispF[i] = mbObjDispGet(mbPlayerObjIDGet(i));
        mbObjDispSet(mbPlayerObjIDGet(i), FALSE);
    }
}

void mbPauseDispCopyKill(void)
{
    int i;

    pauseDispCopyCounter = 0;
    HuMemDirectFree(pauseDispCopyFb);
    pauseDispCopyFb = NULL;
    Hu3DModelKill(pauseDispCopyModelId);
    for (i = 0; i < 4; i++) {
        if (playerDispF[i]) {
            mbObjDispSet(mbPlayerObjIDGet(i), TRUE);
        }
    }
}

static void PauseDispCopyDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    if (pauseDispCopyCounter == 0) {
        GXSetTexCopySrc(0, 0, 640, 480);
        GXSetTexCopyDst(320, 240, GX_TF_RGB565, GX_TRUE);
        GXCopyTex(pauseDispCopyFb, GX_FALSE);
        GXPixModeSync();
        pauseDispCopyCounter++;
    } else {
        Mtx modelview;
        Mtx44 proj;
        GXTexObj texObj;

        MTXOrtho(proj, 0, 480, 0, 640, 0, 100);
        GXSetProjection(proj, GX_ORTHOGRAPHIC);
        GXSetViewport(0, 0, 640, 480, 0, 1);
        GXSetScissor(0, 0, 640, 480);
        MTXIdentity(modelview);
        GXLoadPosMtxImm(modelview, GX_PNMTX0);
        GXSetCullMode(GX_CULL_NONE);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
        GXSetNumTexGens(1);
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
        GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
        GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        GXInitTexObj(&texObj, pauseDispCopyFb, 320, 240, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_TRUE);
        GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 8);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3s16(0, 0, -50);
        GXTexCoord2s16(0, 0);
        GXPosition3s16(640, 0, -50);
        GXTexCoord2s16(256, 0);
        GXPosition3s16(640, 480, -50);
        GXTexCoord2s16(256, 256);
        GXPosition3s16(0, 480, -50);
        GXTexCoord2s16(0, 256);
        GXEnd();
    }
}

static void PauseGuideMain(void);
static void PauseGuideDestroy(void);

void mbPauseGuideCreate(void)
{
    pausePanelWork = mbMalloc(PAUSE_PANEL_MAX * sizeof(PAUSE_PANEL));
    pauseGuideKillF = FALSE;
    pauseGuideProc = HuPrcChildCreate(PauseGuideMain, 0x2011, 0x2000, 0, mbMainProc);
    HuPrcDestructorSet2(pauseGuideProc, PauseGuideDestroy);
    HuPrcSetStat(pauseGuideProc, 12);
}

void mbPauseGuideKill(void)
{
    pauseGuideKillF = TRUE;
}

static inline float PauseFabs(register float x)
{
    asm {
        fabs x, x
    }
    return x;
}

static void PauseGuideMain(void)
{
    Mtx lookAt;
    Mtx rotMtx;
    Mtx invRot;
    HuVecF pos;
    HuVecF rot;
    PAUSE_PANEL *panel;
    int i;
    BOOL endF;
    BOOL priF;
    HU3D_CAMERA *cameraP;
    float t;
    float anmT;
    float weight;
    float scale;
    float sprScale;

    for (i = 0; i < 16; i++) {
        if ((1 << i) & 4) {
            break;
        }
    }
    cameraP = &Hu3DCamera[i];
    while (!pauseGuideKillF) {
        panel = pausePanelWork;
        for (i = 0; i < PAUSE_PANEL_MAX; i++, panel++) {
            if (!panel->objId) {
                continue;
            }
            endF = FALSE;
            panel->time++;
            if (panel->delay) {
                t = 0.0f;
                if (panel->time >= panel->delay) {
                    panel->delay = panel->time = 0;
                    if (panel->mode == 4) {
                        mbAudFXPlay(52);
                    } else if (panel->mode == 5) {
                        mbAudFXPlay(53);
                    }
                }
            } else {
                t = 1.0f;
                if (panel->time < panel->maxTime && panel->maxTime > 0) {
                    t = (float)panel->time / panel->maxTime;
                } else {
                    panel->time = panel->maxTime = 0;
                    endF = TRUE;
                }
                switch (panel->mode) {
                    case 0:
                    case 1:
                    case 2:
                        break;
                    case 4:
                        mbObjRotGet(panel->objId, &pos);
                        pos.y = -500.0f * (1.0f - t);
                        if (endF) {
                            pos.y = 0.0f;
                            if (panel->labelObjId && panel->batsuF) {
                                mbObjDispSet(panel->labelObjId, TRUE);
                            }
                        }
                        mbObjRotSetV(panel->objId, &pos);
                        mbObjDispSet(panel->objId, TRUE);
                        if (panel->sprId >= 0) {
                            espDispOn(panel->sprId);
                        }
                        break;
                    case 5:
                        mbObjRotGet(panel->objId, &pos);
                        pos.y = 500.0f * t;
                        if (panel->labelObjId) {
                            mbObjDispSet(panel->labelObjId, FALSE);
                        }
                        if (endF) {
                            pos.y = 0.0f;
                            mbObjDispSet(panel->objId, FALSE);
                            if (panel->sprId >= 0) {
                                espDispOff(panel->sprId);
                            }
                        }
                        panel->scale = mbCosDeg(90.0f * t);
                        panel->scaleEnd = panel->scaleStart = panel->scale;
                        mbObjRotSetV(panel->objId, &pos);
                        break;
                }
            }
            if (panel->anmMaxTime) {
                if (panel->labelObjId) {
                    mbObjDispSet(panel->labelObjId, FALSE);
                }
                mbObjRotGet(panel->objId, &pos);
                panel->anmTime++;
                if (panel->anmTime < panel->anmMaxTime && panel->anmMaxTime > 0) {
                    anmT = (float)panel->anmTime / panel->anmMaxTime;
                    pos.y = 180.0f * anmT;
                } else {
                    panel->anmTime = panel->anmMaxTime = 0;
                    Hu3DAnmNoSet(panel->animId[0], panel->bank);
                    pos.y = 0.0f;
                    if (panel->labelObjId && panel->batsuF) {
                        mbObjDispSet(panel->labelObjId, TRUE);
                    }
                }
                mbObjRotSetV(panel->objId, &pos);
            }
            if (endF) {
                panel->mode = 0;
                mbVecCopy(&panel->endPos, &panel->pos);
                mbVecCopy(&panel->endPos, &panel->startPos);
                panel->scaleStart = panel->scale = panel->scaleEnd;
            } else {
                weight = mbSinDeg(90.0f * t);
                PSVECSubtract(&panel->endPos, &panel->startPos, &pos);
                PSVECScale(&pos, &pos, weight);
                PSVECAdd(&panel->startPos, &pos, &panel->pos);
                panel->scale = panel->scaleStart + weight * (panel->scaleEnd - panel->scaleStart);
            }
            priF = FALSE;
            if (panel->scaleEnd >= 1.2f) {
                priF = TRUE;
            }
            mbNormPosto3D(&panel->pos, 4, &pos);
            mbObjPosSetV(panel->objId, &pos);
            if (panel->labelObjId) {
                mbObjPosSetV(panel->labelObjId, &pos);
            }
            if (panel->sprId >= 0) {
                pos.y -= 100.0f * (0.3f * (panel->scale * panel->baseScale));
                mbPos3DtoNorm(&pos, 4, &pos);
                mbNormPosto2D(&pos, &pos);
                espPosSet(panel->sprId, pos.x, pos.y);
                if (priF) {
                    espPriSet(panel->sprId, 99);
                } else {
                    espPriSet(panel->sprId, 100);
                }
            }
            scale = panel->scale * panel->baseScale;
            mbObjScaleSet(panel->objId, 0.75f * scale, 0.75f * scale, 0.75f * scale);
            if (panel->labelObjId) {
                mbObjScaleSet(panel->labelObjId, 0.8f * (0.75f * scale), 0.8f * (0.75f * scale), 0.75f * scale);
            }
            if (panel->sprId >= 0) {
                sprScale = 0.85f * scale;
                if (panel->anmMaxTime) {
                    t = (float)panel->anmTime / panel->anmMaxTime;
                    weight = PauseFabs(mbCosDeg(180.0f * t));
                    espScaleSet(panel->sprId, sprScale, sprScale * weight);
                    if (t >= 0.5f) {
                        espBankSet(panel->sprId, panel->bank);
                    }
                    weight = 255.0f * weight;
                    espColorSet(panel->sprId, weight, weight, weight);
                } else {
                    espScaleSet(panel->sprId, sprScale, sprScale);
                    espColorSet(panel->sprId, 255, 255, 255);
                }
            }
            mbObjRotGet(panel->objId, &rot);
            mtxRot(rotMtx, rot.x, rot.y, rot.z);
            PSMTXInverse(rotMtx, invRot);
            mbObjPosGet(panel->objId, &pos);
            MTXLookAt(lookAt, &cameraP->pos, &cameraP->up, &pos);
            lookAt[0][3] = lookAt[1][3] = lookAt[2][3] = 0.0f;
            PSMTXInverse(lookAt, lookAt);
            mbObjMtxSet(panel->labelObjId, lookAt);
            PSMTXConcat(invRot, lookAt, lookAt);
            PSMTXConcat(lookAt, rotMtx, lookAt);
            mbObjMtxSet(panel->objId, lookAt);
        }
        HuPrcVSleep();
    }
    HuPrcEnd();
}

static void PauseGuideDestroy(void)
{
    int i;
    void *work;

    if (pausePanelWork) {
        for (i = 0; i < PAUSE_PANEL_MAX; i++) {
        }
        work = pausePanelWork;
        HuMemDirectFree(work);
        pausePanelWork = NULL;
    }
    pauseGuideProc = NULL;
}

static inline void PausePanelAttrLoopSet(MBOBJID objId)
{
    mbObjAttrSet(objId, 0x40000001);
}

static inline void PausePanelAttrSet(MBOBJID objId)
{
    mbObjAttrSet(objId, 0x200000);
}

s16 mbPausePanelCreate(int dataNum, u32 sprDataNum)
{
    int j;
    int i;
    PAUSE_PANEL *panel;

    for (i = 1; i < PAUSE_PANEL_MAX; i++) {
        if (pausePanelWork[i].objId <= 0) {
            break;
        }
    }
    panel = &pausePanelWork[i];
    memset(panel, 0, sizeof(PAUSE_PANEL));
    panel->scale = panel->scaleStart = panel->scaleEnd = panel->baseScale = 1.0f;
    panel->pos.z = panel->startPos.z = panel->endPos.z = -500.0f;
    panel->objId = mbObjCreate(mbBoardDataNumGet(0x80025), NULL, FALSE);
    mbObjCameraSet(panel->objId, 4);
    mbObjLayerSet(panel->objId, 4);
    PausePanelAttrLoopSet(panel->objId);
    PausePanelAttrSet(panel->objId);
    mbObjDispSet(panel->objId, FALSE);
    panel->anim = HuSprAnimDataRead(mbBoardDataNumGet(dataNum));
    for (j = 0; j < 2; j++) {
        if (j == 0) {
            panel->animId[0] = Hu3DAnimCreate(panel->anim, mbObjModelIDGet(panel->objId), "S3TCys77120");
        } else {
            panel->animId[j] = Hu3DAnimLink(panel->animId[0], mbObjModelIDGet(panel->objId), "S3TCys77121");
        }
        Hu3DAnmNoSet(panel->animId[j], 0);
    }
    panel->labelObjId = mbObjCreate(mbBoardDataNumGet(0x80024), NULL, TRUE);
    mbObjCameraSet(panel->labelObjId, 4);
    mbObjLayerSet(panel->labelObjId, 4);
    PausePanelAttrSet(panel->labelObjId);
    mbObjDispSet(panel->labelObjId, FALSE);
    panel->sprId = -1;
    if (sprDataNum) {
        panel->sprId = espEntry(mbBoardDataNumGet(sprDataNum), 100, 0);
        espAttrSet(panel->sprId, 8);
        espDispOff(panel->sprId);
    }
    return i;
}

void mbPausePanelKill(s16 panelId)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    int i;

    for (i = 0; i < 2; i++) {
        if (panel->animId[i] >= 0) {
            Hu3DAnimKill(panel->animId[i]);
        }
        panel->animId[i] = -1;
    }
    if (panel->anim) {
        HuSprAnimKill(panel->anim);
    }
    panel->anim = NULL;
    if (panel->objId) {
        mbObjKill(panel->objId);
        panel->objId = 0;
    }
    if (panel->labelObjId) {
        mbObjKill(panel->labelObjId);
        panel->labelObjId = 0;
    }
    if (panel->sprId >= 0) {
        espKill(panel->sprId);
        panel->sprId = -1;
    }
}

void mbPausePanelPosSet(s16 panelId, float posX, float posY)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    panel->pos.x = panel->startPos.x = panel->endPos.x = posX;
    panel->pos.y = panel->startPos.y = panel->endPos.y = posY;
}

void mbPausePanelPosGet(s16 panelId, HuVecF *pos)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    pos->x = panel->pos.x;
    pos->y = panel->pos.y;
    pos->z = 0.0f;
}

void mbPausePanelRotSet(s16 panelId, float rotX, float rotY, float rotZ)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    mbObjRotSet(panel->objId, rotX, rotY, rotZ);
}

void mbPausePanelScaleSet(s16 panelId, float scale)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    panel->scale = panel->scaleStart = panel->scaleEnd = scale;
}

float mbPausePanelScaleGet(s16 panelId)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    return panel->scale;
}

void mbPausePanelBankSet(s16 panelId, int bank)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    panel->bank = bank;
    Hu3DAnmNoSet(panel->animId[0], bank);
    Hu3DAnmNoSet(panel->animId[1], bank);
    if (panel->sprId >= 0) {
        espBankSet(panel->sprId, bank);
    }
}

void mbPausePanelBatsuSet(s16 panelId, BOOL batsuF)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    panel->batsuF = batsuF;
}

BOOL mbPausePanelFreezeGet(s16 panelId)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    BOOL result = FALSE;
    if (panel->mode == 0 && panel->anmMaxTime == 0) {
        result = TRUE;
    }
    return result;
}

void mbPausePanelUnlockSet(s16 panelId)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    panel->mode = 0;
    mbVecCopy(&panel->pos, &panel->startPos);
    mbVecCopy(&panel->pos, &panel->endPos);
    panel->scaleEnd = panel->scaleStart = panel->scale;
    panel->time = panel->maxTime = 0;
}

void mbPausePanelSlideSet(s16 panelId, int time, HuVecF *pos)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    panel->mode = 1;
    panel->maxTime = time;
    panel->time = 0;
    mbVecCopy(pos, &panel->endPos);
    panel->endPos.z = -500.0f;
    mbVecCopy(&panel->pos, &panel->startPos);
}

void mbPausePanelSizeSet(s16 panelId, int time, float scale)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    panel->mode = 2;
    panel->maxTime = time;
    panel->time = 0;
    panel->scaleEnd = scale;
    panel->scaleStart = panel->scale;
}

void mbPausePanelAnmNoSet(s16 panelId, int time, int bank)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    panel->anmMaxTime = time;
    panel->anmTime = 0;
    Hu3DAnmNoSet(panel->animId[0], panel->bank);
    panel->bank = bank;
    Hu3DAnmNoSet(panel->animId[1], bank);
    panel->sprId >= 0;
}

void mbPausePanelGrowSet(s16 panelId, int time, int delay, float scale)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    panel->mode = 4;
    panel->maxTime = time;
    panel->delay = delay + 1;
    panel->time = 0;
    panel->scaleEnd = 1.0f;
    panel->scaleStart = panel->scale = 0.00001f;
    panel->baseScale = scale;
    mbObjDispSet(panel->objId, FALSE);
    if (panel->labelObjId) {
        mbObjDispSet(panel->labelObjId, FALSE);
    }
}

void mbPausePanelShrinkSet(s16 panelId, int time, int delay)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    panel->mode = 5;
    panel->maxTime = time;
    panel->delay = delay + 1;
    panel->time = 0;
}

void mbPauseGuideMoveSet(s16 objId, int time, HuVecF *pos, HuVecF *normPos)
{
    HuVecF end;
    HuVecF start;
    int i;
    float t;
    float dir;

    if (pos == NULL) {
        mbNormPosto3D(normPos, 4, &end);
    } else {
        mbVecCopy(pos, &end);
    }
    mbObjPosGet(objId, &start);
    if (abs((int)(end.x - start.x)) < 200.0f) {
        for (i = 0; i <= time; i++) {
            t = (float)i / time;
            t = mbSinDeg(90.0f * t);
            mbObjPosSet(objId, start.x + t * (end.x - start.x), start.y + t * (end.y - start.y), start.z + t * (end.z - start.z));
            HuPrcVSleep();
        }
    } else {
        time *= 1.5;
        dir = 90.0f;
        if (end.x < start.x) {
            dir = -90.0f;
        }
        mbObjMotionShiftSet(objId, 3, 0.0f, 8.0f, 0x40000001);
        mbObjMotionSpeedSet(objId, 1.2f);
        for (i = 0; i <= time; i++) {
            t = (float)i / time;
            mbObjPosSet(objId, start.x + t * (end.x - start.x), start.y + t * (end.y - start.y), start.z + t * (end.z - start.z));
            t = (3.0f * i) / time;
            if (t > 1.0f) {
                t = 1.0f;
            }
            t = mbSinDeg(90.0f * t);
            mbObjRotSet(objId, 0.0f, dir * t, 0.0f);
            HuPrcVSleep();
        }
        mbObjMotionShiftSet(objId, 1, 0.0f, 12.0f, 0x40000001);
        for (i = 0; i <= 12; i++) {
            t = i / 12.0f;
            if (t > 1.0f) {
                t = 1.0f;
            }
            t = 1.0f - mbSinDeg(90.0f * t);
            mbObjRotSet(objId, 0.0f, dir * t, 0.0f);
            HuPrcVSleep();
        }
        mbObjRotSet(objId, 0.0f, 0.0f, 0.0f);
    }
}

void mbPauseGuideTalkSet(s16 objId)
{
    if (pauseWork.talkTime == 0 && pauseWork.talkTimePrev > 0) {
        pauseWork.talkTime = 1;
    } else if (mbObjMotionShiftIDGet(objId) != -1) {
        return;
    }
    if (pauseWork.talkTimePrev == 0 && pauseWork.talkTime > 0) {
        mbObjMotionShiftSet(objId, 12, 0.0f, 8.0f, 0x40000001);
        mbObjMotionSpeedSet(objId, 2.0f);
    }
    if (pauseWork.talkTime) {
        pauseWork.talkTime--;
        if (pauseWork.talkTime == 0) {
            mbObjMotionShiftSet(objId, 1, 0.0f, 16.0f, 0x40000001);
        }
    }
    pauseWork.talkTimePrev = pauseWork.talkTime;
}

void mbConfigPadDisableSet(BOOL disableF)
{
    configPadDisable = disableF;
}

static inline BOOL GWStorySingleCheck(void)
{
    return GWPartyGet() == FALSE;
}
