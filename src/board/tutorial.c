#include "game/board/tutorial.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/exit.h"
#include "game/board/guide.h"
#include "game/board/masu.h"
#include "game/board/math.h"
#include "game/board/object.h"
#include "game/board/player.h"
#include "game/board/telop.h"
#include "game/board/window.h"
#include "game/board/wipe.h"

#include "game/armem.h"
#include "game/esprite.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/mgdata.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/sprite.h"
#include "game/window.h"
#include "game/wipe.h"

#include "math.h"
#include "string.h"

#define TUTORIAL_SPR_MAX 16
#define TUTORIAL_SPRGRP_MAX 16
#define TUTORIAL_MDL_MAX 32
#define TUTORIAL_WIN_MAX 32

typedef struct TutorialCallWork_s {
    /* 0x00 */ int scene;
    /* 0x04 */ int count;
    /* 0x08 */ int result;
    /* 0x0C */ int stat;
    /* 0x10 */ int mode;
} TUTORIALCALLWORK;

typedef struct TutorialWinData_s {
    /* 0x00 */ s8 stat;
    /* 0x01 */ s8 delay;
    /* 0x02 */ s16 pageNum;
} TUTORIALWINDATA;

typedef struct TutorialMgCallData_s {
    /* 0x00 */ BOOL activeF;
    /* 0x04 */ int unk4;
    /* 0x08 */ s16 sprId[8];
    /* 0x18 */ OMOBJ *obj[4];
} TUTORIALMGCALLDATA;

typedef struct TutorialMgCallWork_s {
    /* 0x00 */ u8 killF : 1;
    /* 0x00 */ u8 slideF : 1;
    /* 0x00 */ u8 mode : 3;
    /* 0x00 */ u8 no : 3;
    /* 0x01 */ u8 dispF : 1;
    /* 0x02 */ s16 winNo;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 time;
    /* 0x08 */ s16 maxTime;
    /* 0x0C */ int mes;
} TUTORIALMGCALLWORK;

typedef struct TutorialGuideSe_s {
    int mess;
    int seId;
} TUTORIALGUIDESE;

static s16 tutorialSprId[TUTORIAL_SPR_MAX];
static s16 tutorialSprGrpId[TUTORIAL_SPRGRP_MAX];
static s16 tutorialMdlId[TUTORIAL_MDL_MAX];
static TUTORIALCALLWORK tutorialCallWork;
static TUTORIALWINDATA tutorialWinData[TUTORIAL_WIN_MAX];
static TUTORIALMGCALLDATA tutorialMgCallData;

static HUPROCESS *tutorialWatchProc;
static HUPROCESS *tutorialMainProc;
static BOOL tutorialExitReqF;
static BOOL tutorialExitOnF;
static s16 tutorialMgCallCursorPos;
static void (*tutorialMain)(void);
static OMOBJ *tutorialGuideObj;

static TUTORIALGUIDESE tutorialGuideTbl[] = {
    { 0x2A0000, 950 }, { 0x2A0001, 952 }, { 0x2A0007, 952 }, { 0x2A0008, 952 },
    { 0x2A000A, 950 }, { 0x2A000B, 952 }, { 0x2A000C, 952 }, { 0x2A000D, 952 },
    { 0x2A000E, 952 }, { 0x2A000F, 952 }, { 0x2A0010, 952 }, { 0x2A0011, 952 },
    { 0x2A0014, 950 }, { 0x2A0015, 952 }, { 0x2A0016, 950 }, { 0x2A0017, 952 },
    { 0x2A0018, 950 }, { 0x2A0019, 950 }, { 0x2A001B, 952 }, { 0x2A001C, 952 },
    { 0x2A001D, 952 }, { 0x2A001E, 952 }, { 0x2A001F, 950 }, { 0x2A0020, 952 },
    { 0x2A0021, 952 }, { 0x2A0023, 950 }, { 0x2A0024, 952 }, { 0x2A0025, 952 },
    { 0x2A0026, 952 }, { 0x2A0027, 950 }, { 0x2A0028, 952 }, { 0x2A0029, 950 },
    { 0x2A002A, 952 }, { 0x2A002D, 950 }, { 0x2A002E, 952 }, { 0x2A002F, 952 },
    { 0x2A0031, 952 }, { 0x2A0032, 950 }, { 0x2A0033, 952 }, { 0x2A0034, 952 },
    { 0x2A0035, 950 }, { 0x2A0036, 952 }, { 0x2A0038, 952 }, { 0x2A0039, 952 },
    { 0x2A003A, 950 }, { 0x2A003C, 952 }, { 0x2A003E, 952 }, { 0x2A003F, 950 },
    { 0x2A0040, 952 }, { 0x2A0041, 952 }, { 0x2A0042, 952 }, { 0x2A0043, 952 },
    { 0x2A0044, 952 }, { 0x2A0045, 952 }, { 0x2A0046, 952 }, { 0x2A0047, 952 },
    { 0x2A0049, 950 }, { 0x2A004A, 952 }, { 0x2A004C, 950 }, { 0x2A004D, 950 },
    { 0x2A004E, 952 }, { 0x2A004F, 950 }, { 0x2A0050, 950 }, { 0x2A0051, 950 },
    { 0x2A0052, 952 }, { 0x2A0054, 950 }, { 0x2A0055, 950 }, { 0x2A0057, 950 },
    { 0x2A0058, 952 }, { 0x2A0059, 950 }, { 0x2A005B, 952 }, { 0x2A005C, 950 },
    { -1, -1 }
};

static int mgCallWinYOfsTbl[5][4] = {
    { 0, 0, 0, 0 },
    { 238, 0, 0, 0 },
    { 214, 262, 0, 0 },
    { 198, 238, 278, 0 },
    { 193, 223, 253, 283 }
};

static void TutorialWatch(void);
static void TutorialSprClose(void);
static void TutorialSprGrpClose(void);
static void TutorialModelKillAll(void);
static void TutorialWinInit(void);
static void TutorialWinUpdate(void);

void mbTutorialInit(void)
{
    int i;
    tutorialGuideObj = NULL;
    for(i=0; i<TUTORIAL_SPR_MAX; i++) {
        tutorialSprId[i] = -1;
    }
    for(i=0; i<TUTORIAL_SPRGRP_MAX; i++) {
        tutorialSprGrpId[i] = -1;
    }
    for(i=0; i<TUTORIAL_MDL_MAX; i++) {
        tutorialMdlId[i] = -1;
    }
    mbTutorialMainFuncSet(NULL);
    mbTutorialMgCallInit();
    tutorialExitReqF = FALSE;
    tutorialExitOnF = FALSE;
}

void mbTutorialMainFuncSet(void (*func)(void))
{
    tutorialMain = func;
}

void mbTutorialCreate(void)
{
    tutorialCallWork.scene = -1;
    tutorialCallWork.count = 0;
    tutorialCallWork.result = -1;
    tutorialCallWork.stat = 0;
    if(mbSaveNewF) {
        tutorialCallWork.mode = 0;
    }
    _ClearFlag(0x20001);
    _ClearFlag(0x20003);
    _ClearFlag(0x20007);
    _ClearFlag(0x20008);
    _ClearFlag(0x20000);
    tutorialWatchProc = HuPrcChildCreate(TutorialWatch, 8209, 8192, 0, mbMainProc);
    tutorialMainProc = HuPrcChildCreate(tutorialMain, 8209, 8192, 0, mbMainProc);
    HuPrcVSleep();
}

static void TutorialWatch(void)
{
    s16 winNo;
    u16 btn;
    HuVec2f size;
    HuPrcSleep(60);
    TutorialWinInit();
    winNo = mbWinCreateHelp(0x2A0048);
    mbWinMesMaxSizeGet(winNo, &size);
    mbWinPosSet(winNo, 288.0f-((s16)size.x/2), 400);
    tutorialExitOnF = TRUE;
    while(!tutorialExitReqF) {
        btn = HuPadBtnDown[0];
        TutorialWinUpdate();
        mbWinDispSet(winNo, tutorialExitOnF);
        if(!mbWipeSpecialCheck() && !mbWipeSpecialStatGet() && (btn & PAD_BUTTON_START) && tutorialExitOnF) {
            tutorialExitReqF = TRUE;
            break;
        }
        HuPrcVSleep();
    }
    mbPauseEnableSet();
    while(HuARDMACheck()) {
        HuPrcVSleep();
    }
    mbMusFadeOutSpeed(0, 1000);
    if(tutorialExitOnF) {
        if(GWPartyGet() != FALSE) {
            WipeColorSet(0, 0, 0);
        } else {
            WipeColorSet(0, 0, 0);
        }
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
        mbWipeWait();
    }
    while(mbMusCheck(0)) {
        HuPrcVSleep();
    }
    mbWinKill(winNo);
    HuPrcKill(tutorialMainProc);
    tutorialMainProc = NULL;
    TutorialSprClose();
    TutorialSprGrpClose();
    TutorialModelKillAll();
    mbTutorialMgCallClose();
    if(tutorialGuideObj) {
        mbTutorialGuideClose(tutorialGuideObj);
    }
    mbExitReq();
    HuPrcSleep(-1);
}

int mbTutorialCall(int scene)
{
    if(!_CheckFlag(0x1000E)) {
        return -1;
    }
    if(tutorialMainProc == NULL) {
        return -1;
    }
    while(tutorialCallWork.stat == 0) {
        HuPrcVSleep();
    }
    if(tutorialCallWork.stat < 0) {
        return -1;
    }
    if(tutorialCallWork.scene >= 0) {
        if(tutorialCallWork.scene != scene) {
            return -1;
        }
        tutorialCallWork.count--;
        if(tutorialCallWork.count > 0) {
            return -1;
        }
    }
    tutorialCallWork.count = scene;
    tutorialCallWork.result = -1;
    tutorialCallWork.stat = 0;
    while(tutorialCallWork.stat == 0) {
        HuPrcVSleep();
    }
    return tutorialCallWork.result;
}

void mbTutorialCallEnd(void)
{
    tutorialCallWork.stat = -1;
}

void mbTutorialVSleep(void)
{
    if(mbTutorialExitReqGet()) {
        HuPrcSleep(-1);
    }
    HuPrcVSleep();
}

void mbTutorialCallWait(int scene)
{
    tutorialCallWork.scene = scene;
    tutorialCallWork.count = 0;
    tutorialCallWork.stat = 1;
    while(tutorialCallWork.stat != 0) {
        mbTutorialVSleep();
    }
}

void mbTutorialMultiCall(int scene, int count)
{
    tutorialCallWork.scene = scene;
    tutorialCallWork.count = count;
    tutorialCallWork.stat = 1;
    while(tutorialCallWork.stat != 0) {
        mbTutorialVSleep();
    }
}

void mbTutorialCallResult(int scene, int result)
{
    tutorialCallWork.scene = scene;
    tutorialCallWork.count = 0;
    tutorialCallWork.stat = 1;
    while(tutorialCallWork.stat != 0) {
        mbTutorialVSleep();
    }
    mbTutorialResultSet(result);
}

void mbTutorialMesCall(int scene, u32 mess)
{
    tutorialCallWork.scene = scene;
    tutorialCallWork.count = 0;
    tutorialCallWork.stat = 1;
    while(tutorialCallWork.stat != 0) {
        mbTutorialVSleep();
    }
    mbTutorialWinMesExec(mess);
}

void mbTutorialMesMasuCall(int scene, u32 mess, int dataNum)
{
    tutorialCallWork.scene = scene;
    tutorialCallWork.count = 0;
    tutorialCallWork.stat = 1;
    while(tutorialCallWork.stat != 0) {
        mbTutorialVSleep();
    }
    mbTutorialWinMesMasuExec(mess, dataNum);
}

void mbTutorialTurnCall(int turnNo)
{
    tutorialCallWork.scene = 5;
    tutorialCallWork.count = 0;
    tutorialCallWork.stat = 1;
    while(tutorialCallWork.stat != 0) {
        mbTutorialVSleep();
    }
    mbTutorialResultSet(turnNo-1);
}

void mbTutorialGuideCall(int result)
{
    tutorialCallWork.scene = 13;
    tutorialCallWork.count = 0;
    tutorialCallWork.stat = 1;
    while(tutorialCallWork.stat != 0) {
        mbTutorialVSleep();
    }
    mbTutorialResultSet(result);
}

void mbTutorialCapsuleUseCall(int capsuleNo)
{
    int result;
    tutorialCallWork.scene = 15;
    tutorialCallWork.count = 0;
    tutorialCallWork.stat = 1;
    while(tutorialCallWork.stat != 0) {
        mbTutorialVSleep();
    }
    result = mbPlayerCapsuleFind(GwSystem.turnPlayerNo, capsuleNo);
    mbTutorialResultSet(result);
}

void mbTutorialResultSet(int result)
{
    tutorialCallWork.result = result;
}

int mbTutorialSceneGet(void)
{
    return tutorialCallWork.scene;
}

void mbTutorialModeSet(int mode)
{
    tutorialCallWork.mode = mode;
}

int mbTutorialModeGet(void)
{
    return tutorialCallWork.mode;
}

s16 mbTutorialViewSet(void)
{
    HuVecF rot = { -35, 0, 0 };
    HuVecF ofs = { 0, 200, 0 };
    int masuId = mbMasuFind_AttrIdGet(-1, 0x8000);
    mbCameraMoveMasu(masuId, &rot, &ofs, mbCameraPlayerViewZoomGet(0), -1, -1);
    mbCameraMoveOnSet(FALSE);
    return masuId;
}

void mbTutorialViewMasuSet(s16 masuId)
{
    HuVecF rot = { -35, 0, 0 };
    HuVecF ofs = { 0, 50, 0 };
    mbCameraMoveMasu(masuId, &rot, &ofs, mbCameraPlayerViewZoomGet(0), -1, -1);
    mbCameraMoveOnSet(FALSE);
}

s16 mbTutorialSprCreate(int dataNum)
{
    int i;
    s16 sprId;
    for(i=0; i<TUTORIAL_SPR_MAX; i++) {
        if(tutorialSprId[i] < 0) {
            break;
        }
    }
    sprId = tutorialSprId[i] = espEntry(dataNum, 1500, 0);
    espAttrSet(sprId, 8);
    return sprId;
}

s16 mbTutorialSprDispOn(int dataNum)
{
    s16 sprId = mbTutorialSprCreate(dataNum);
    int i;
    espPosSet(sprId, 288, 144);
    for(i=0; i<=12u; i++) {
        float scale = i/12.0f;
        espScaleSet(sprId, scale, scale);
        mbTutorialVSleep();
    }
    return sprId;
}

void mbTutorialSprKill(s16 sprId)
{
    int i;
    int no;
    for(i=0; i<TUTORIAL_SPR_MAX; i++) {
        if(sprId == tutorialSprId[i]) {
            break;
        }
    }
    no = i;
    espKill(sprId);
    tutorialSprId[no] = -1;
}

void mbTutorialSprDispOff(s16 sprId)
{
    int i;
    for(i=0; i<=12u; i++) {
        float scale = 1-(i/12.0f);
        espScaleSet(sprId, scale, scale);
        mbTutorialVSleep();
    }
    mbTutorialSprKill(sprId);
}

static void TutorialSprClose(void)
{
    int i;
    for(i=0; i<TUTORIAL_SPR_MAX; i++) {
        if(tutorialSprId[i] >= 0) {
            espKill(tutorialSprId[i]);
        }
        tutorialSprId[i] = -1;
    }
}

void mbTutorialSprGrpSet(s16 grpId)
{
    int i;
    s16 id;
    for(i=0; i<TUTORIAL_SPRGRP_MAX; i++) {
        if(tutorialSprGrpId[i] < 0) {
            break;
        }
    }
    id = tutorialSprGrpId[i] = grpId;
}

void mbTutorialSprGrpKill(s16 grpId)
{
    int i;
    int no;
    for(i=0; i<TUTORIAL_SPRGRP_MAX; i++) {
        if(grpId == tutorialSprGrpId[i]) {
            break;
        }
    }
    no = i;
    HuSprGrpKill(grpId);
    tutorialSprId[no] = -1;
}

static void TutorialSprGrpClose(void)
{
    int i;
    for(i=0; i<TUTORIAL_SPRGRP_MAX; i++) {
        if(tutorialSprGrpId[i] >= 0) {
            HuSprGrpKill(tutorialSprGrpId[i]);
        }
        tutorialSprGrpId[i] = -1;
    }
}

s16 mbTutorialModelCreate(int dataNum, BOOL linkF)
{
    int i;
    s16 mdlId;
    for(i=0; i<TUTORIAL_MDL_MAX; i++) {
        if(tutorialMdlId[i] < 0) {
            break;
        }
    }
    mdlId = tutorialMdlId[i] = mbObjCreate(dataNum, NULL, linkF);
    return mdlId;
}

void mbTutorialModelKill(s16 mdlId)
{
    int i;
    int no;
    for(i=0; i<TUTORIAL_MDL_MAX; i++) {
        if(mdlId == tutorialMdlId[i]) {
            break;
        }
    }
    no = i;
    mbObjKill(mdlId);
    tutorialMdlId[no] = -1;
}

static void TutorialModelKillAll(void)
{
    int i;
    for(i=0; i<TUTORIAL_MDL_MAX; i++) {
        if(tutorialMdlId[i] >= 0) {
            mbObjKill(tutorialMdlId[i]);
        }
        tutorialMdlId[i] = -1;
    }
}

OMOBJ *mbTutorialGuideCreate(s8 *motTbl, BOOL screenF)
{
    tutorialGuideObj = mbGuideCreateFlag(NULL, motTbl, screenF, FALSE, TRUE);
    mbObjDispSet(mbGuideModelGet(tutorialGuideObj), FALSE);
    mbGuideMotionNextSet(tutorialGuideObj, 1);
    mbGuideMotionShiftSet(tutorialGuideObj, 1, FALSE);
    return tutorialGuideObj;
}

void mbTutorialGuideClose(OMOBJ *obj)
{
    mbGuideKill(obj);
    tutorialGuideObj = NULL;
}

OMOBJ *mbTutorialGuideGet(void)
{
    return tutorialGuideObj;
}

BOOL mbTutorialExitReqGet(void)
{
    return tutorialExitReqF;
}

void mbTutorialExitSet(void)
{
    tutorialExitReqF = TRUE;
}

void mbTutorialExitOnSet(BOOL flag)
{
    tutorialExitOnF = flag;
}

static void TutorialWinInit(void)
{
    int i;
    for(i=0; i<TUTORIAL_WIN_MAX; i++) {
        tutorialWinData[i].delay = tutorialWinData[i].stat = -1;
        tutorialWinData[i].pageNum = -1;
    }
}

static void TutorialWinUpdate(void)
{
    int i;
    for(i=0; i<TUTORIAL_WIN_MAX; i++) {
        if(winData[i].grpId != -1) {
            if(tutorialWinData[i].stat < 0) {
                tutorialWinData[i].stat = tutorialWinData[i].pageNum = 0;
                tutorialWinData[i].delay = 0;
            }
            if(winData[i].stat == 2) {
                tutorialWinData[i].stat = 1;
                tutorialWinData[i].delay = 1;
            } else if(winData[i].stat != 0 && tutorialWinData[i].stat != 0) {
                if(tutorialWinData[i].delay != 0) {
                    tutorialWinData[i].delay--;
                } else {
                    tutorialWinData[i].stat = 0;
                    tutorialWinData[i].pageNum++;
                }
            }
        } else {
            tutorialWinData[i].stat = tutorialWinData[i].pageNum = -1;
        }
    }
}

BOOL mbTutorialWinWait(int winNo)
{
    MBWIN *win;
    BOOL result = FALSE;
    while(1) {
        mbTutorialVSleep();
        if(mbWinDoneCheck(winNo)) {
            break;
        }
        win = mbWinGet(winNo);
        if(win->winId < 0) {
            break;
        }
        if(tutorialWinData[win->winId].stat < 0) {
            break;
        }
        if(tutorialWinData[win->winId].pageNum > 0) {
            tutorialWinData[win->winId].pageNum--;
            result = TRUE;
            break;
        }
    }
    return result;
}

void mbTutorialWinClose(int winNo)
{
    MBWIN *win;
    if(mbWinDoneCheck(winNo)) {
        return;
    }
    win = mbWinGet(winNo);
    if(win->winId < 0) {
        return;
    }
    if(tutorialWinData[win->winId].stat < 0) {
        return;
    }
    tutorialWinData[win->winId].stat = tutorialWinData[win->winId].pageNum = 0;
}

void mbTutorialWinMesExec(int mess)
{
    int winNo;
    int i;
    winNo = mbWinCreateTime(5, mess, -1);
    mbWinPlayerDisable(winNo, -1);
    for(i=0; tutorialGuideTbl[i].mess >= 0; i++) {
        if(mess == tutorialGuideTbl[i].mess) {
            mbAudGuidePlay(tutorialGuideTbl[i].seId);
            break;
        }
    }
    do {
        mbGuideMotionShiftSet(mbTutorialGuideGet(), 12, TRUE);
    } while(mbTutorialWinWait(winNo));
    mbWinWait(winNo);
}

void mbTutorialWinMesMasuExec(int mess, int dataNum)
{
    int winNo;
    int i;
    int sprId;
    winNo = mbWinCreateTime(5, mess, -1);
    mbWinPlayerDisable(winNo, -1);
    for(i=0; tutorialGuideTbl[i].mess >= 0; i++) {
        if(mess == tutorialGuideTbl[i].mess) {
            mbAudGuidePlay(tutorialGuideTbl[i].seId);
            break;
        }
    }
    mbGuideMotionShiftSet(mbTutorialGuideGet(), 12, TRUE);
    sprId = mbTutorialSprDispOn(dataNum);
    while(mbTutorialWinWait(winNo)) {
        mbGuideMotionShiftSet(mbTutorialGuideGet(), 12, TRUE);
    }
    mbWinWait(winNo);
    mbTutorialSprDispOff(sprId);
}

int mbTutorialWinCreate(int mess)
{
    int winNo;
    int i;
    winNo = mbWinCreateTime(5, mess, -1);
    mbWinPlayerDisable(winNo, -1);
    for(i=0; tutorialGuideTbl[i].mess >= 0; i++) {
        if(mess == tutorialGuideTbl[i].mess) {
            mbAudGuidePlay(tutorialGuideTbl[i].seId);
            break;
        }
    }
    mbGuideMotionShiftSet(mbTutorialGuideGet(), 12, TRUE);
    return winNo;
}

void mbTutorialWinKeyWait(int winNo)
{
    while(mbTutorialWinWait(winNo)) {
        mbGuideMotionShiftSet(mbTutorialGuideGet(), 12, TRUE);
    }
    mbWinWait(winNo);
}

static void TutorialMgCallListGet(int type, int num, s16 *noTbl);
static void TutorialMgCallOMExec(OMOBJ *obj);
static void TutorialMgCallSlideInSet(OMOBJ *obj);
static BOOL TutorialMgCallSlideInCheck(OMOBJ *obj);
static void TutorialMgCallGrowSet(OMOBJ *obj);
static void TutorialMgCallKill(OMOBJ *obj);

void mbTutorialMgCallInit(void)
{
    memset(&tutorialMgCallData, 0, sizeof(tutorialMgCallData));
}

void mbTutorialMgCallClose(void)
{
    int i;
    if(tutorialMgCallData.activeF) {
        for(i=0; i<3; i++) {
            if(tutorialMgCallData.sprId[i] >= 0) {
                espKill(tutorialMgCallData.sprId[i]);
            }
            tutorialMgCallData.sprId[i] = -1;
        }
        for(i=0; i<4; i++) {
            if(tutorialMgCallData.obj[i]) {
                TutorialMgCallKill(tutorialMgCallData.obj[i]);
            }
            tutorialMgCallData.obj[i] = NULL;
        }
        tutorialMgCallData.activeF = FALSE;
    }
}

void mbTutorialMgCallExec(int type)
{
    TUTORIALMGCALLDATA *data = &tutorialMgCallData;
    int i;
    TUTORIALMGCALLWORK *work;
    int delay;
    int num;
    int nextMaxTime;
    int nextTime;
    int speed;
    MG_DATA *mgData;
    int nextDelay;
    int focusNo;
    int focusOfs;
    float t;
    int unused;
    s16 noTbl[4];
    for(i=0; i<3; i++) {
        data->sprId[i] = -1;
    }
    for(i=0; i<4; i++) {
        data->obj[i] = NULL;
    }
    data->activeF = TRUE;
    num = 4;
    TutorialMgCallListGet(type, num, noTbl);
    data->sprId[0] = espEntry(mbBoardDataNumGet(0x50087), 100, 0);
    espPosSet(data->sprId[0], 288, 230.4f);
    espDrawNoSet(data->sprId[0], 32);
    data->sprId[1] = espEntry(mbBoardDataNumGet(0x50088), 102, 0);
    espPosSet(data->sprId[1], 288, 238.4f);
    espDrawNoSet(data->sprId[1], 32);
    espTPLvlSet(data->sprId[1], 0.5f);
    data->sprId[2] = espEntry(mbBoardDataNumGet(0x50089), 101, 0);
    espPosSet(data->sprId[2], 288, 246.4f);
    espDrawNoSet(data->sprId[2], 32);
    espTPLvlSet(data->sprId[2], 0.5f);
    espDispOff(data->sprId[2]);
    for(i=1; i<=30; i++) {
        t = 1-(i/30.0f);
        espPosSet(data->sprId[0], 288, 230.4f+(480*t));
        espPosSet(data->sprId[1], 288, 238.4f+(480*t));
        mbTutorialVSleep();
    }
    for(i=0; i<num; i++) {
        mgData = NULL;
        if(noTbl[i] >= 0) {
            mgData = &MgDataTbl[noTbl[i]];
        }
        data->obj[i] = omAddObjEx(mbObjMan, 257, 0, 0, -1, TutorialMgCallOMExec);
        work = omObjGetWork(data->obj[i], TUTORIALMGCALLWORK);
        work->dispF = TRUE;
        work->no = i;
        data->obj[i]->trans.y = mgCallWinYOfsTbl[num][i];
        unused = 0;
        work->winNo = mbWinCreateHelp(0x2A003D);
        work->mes = -1;
        if(mgData) {
            work->mes = mgData->nameMes;
        }
        mbWinPriSet(work->winNo, 90);
        TutorialMgCallSlideInSet(data->obj[i]);
    }
    tutorialMgCallCursorPos = -1;
    while(!TutorialMgCallSlideInCheck(data->obj[0])) {
        mbTutorialVSleep();
    }
    tutorialMgCallCursorPos = 0;
    focusNo = mbRandMod(num);
    delay = mbRandMod(30)+90;
    focusOfs = (-7+sqrtf(49+(8.0f*delay)))/2;
    speed = (focusNo-focusOfs)%num;
    if(speed < 0) {
        speed += num;
    }
    nextDelay = (num*12)+(speed*4);
    nextTime = nextMaxTime = 4;
    for(i=0; i<nextDelay+delay; i++) {
        if(--nextTime == 0) {
            tutorialMgCallCursorPos = (tutorialMgCallCursorPos+1)%num;
            if(i > nextDelay) {
                nextMaxTime++;
                nextTime = nextMaxTime;
            } else {
                nextTime = nextMaxTime;
            }
            espPosSet(data->sprId[2], 288, data->obj[tutorialMgCallCursorPos]->trans.y);
            espDispOn(data->sprId[2]);
            mbAudFXPlay(1009);
        }
        mbTutorialVSleep();
    }
    tutorialMgCallCursorPos = focusNo;
    espPosSet(data->sprId[2], 288, data->obj[tutorialMgCallCursorPos]->trans.y);
    TutorialMgCallGrowSet(data->obj[tutorialMgCallCursorPos]);
    mbAudFXPlay(1134);
    HuPrcSleep(120);
}

static void TutorialMgCallListGet(int type, int num, s16 *noTbl)
{
    int i;
    int cnt = 0;
    int no = -1;
    s8 list[64];
    for(i=0; MgDataTbl[i].ovl != 0xFFFF; i++) {
        if((MgDataTbl[i].flag & 0x2C0) == 0 && type == MgDataTbl[i].type) {
            list[cnt++] = i;
        }
    }
    if(cnt > 0) {
        no = list[mbRandMod(cnt)];
    }
    for(i=0; i<num; i++) {
        noTbl[i] = no;
    }
}

static void TutorialMgCallOMExec(OMOBJ *obj)
{
    TUTORIALMGCALLWORK *work = omObjGetWork(obj, TUTORIALMGCALLWORK);
    float t;
    float s;
    HuVec2f size;
    if(work->killF || mbExitCheck()) {
        mbWinKill(work->winNo);
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    if(work->dispF) {
        mbWinDispSet(work->winNo, TRUE);
    } else {
        mbWinDispSet(work->winNo, FALSE);
    }
    switch(work->mode) {
        case 0:
            if(work->time > work->maxTime) {
                work->slideF = FALSE;
                work->mode = 1;
                break;
            }
            t = (float)(work->time++)/work->maxTime;
            s = sin(M_PI*(90.0f*t)/180.0);
            if((work->no & 1) == 0) {
                obj->trans.x = -160+(448*s);
            } else {
                obj->trans.x = 736+(-448*s);
            }
            break;

        case 1:
            if(work->no == tutorialMgCallCursorPos) {
                work->mode = 2;
            }
            break;

        case 2:
            if(work->no != tutorialMgCallCursorPos) {
                work->mode = 3;
                work->time = 0;
                work->maxTime = 8;
            }
            break;

        case 3:
            if(work->time > work->maxTime) {
                work->mode = 1;
                break;
            }
            t = (float)(work->time++)/work->maxTime;
            break;

        case 4:
            if(work->time <= work->maxTime) {
                t = (float)(work->time++)/work->maxTime;
                obj->scale.x = obj->scale.y = 1.0f+(0.2f*sin(M_PI*(720.0f*(1.0f-t))/180.0));
            }
            break;

        case 5:
            if(work->time <= work->maxTime) {
                t = (float)(work->time++)/work->maxTime;
                obj->trans.y = obj->rot.y+(t*(96.0f-obj->rot.y));
                obj->scale.x = obj->scale.y = 0.5+(0.5*cos(M_PI*(90.0f*t)/180.0));
            }
            break;
    }
    mbWinScaleSet(work->winNo, obj->scale.x, obj->scale.y);
    mbWinMesMaxSizeGet(work->winNo, &size);
    mbWinPosSet(work->winNo, obj->trans.x-((size.x/2)*obj->scale.x), obj->trans.y-((size.y/2)*obj->scale.y));
}

static void TutorialMgCallSlideInSet(OMOBJ *obj)
{
    TUTORIALMGCALLWORK *work = omObjGetWork(obj, TUTORIALMGCALLWORK);
    work->slideF = TRUE;
    work->mode = 0;
    work->time = 0;
    work->maxTime = 30;
}

static BOOL TutorialMgCallSlideInCheck(OMOBJ *obj)
{
    TUTORIALMGCALLWORK *work = omObjGetWork(obj, TUTORIALMGCALLWORK);
    if(work->slideF) {
        return FALSE;
    } else {
        return TRUE;
    }
}

static void TutorialMgCallGrowSet(OMOBJ *obj)
{
    TUTORIALMGCALLWORK *work = omObjGetWork(obj, TUTORIALMGCALLWORK);
    work->mode = 4;
    work->time = 0;
    work->maxTime = 90;
    if(work->mes >= 0) {
        mbWinKill(work->winNo);
        work->winNo = mbWinCreateHelp(work->mes);
    }
}

static void TutorialMgCallKill(OMOBJ *obj)
{
    TUTORIALMGCALLWORK *work = omObjGetWork(obj, TUTORIALMGCALLWORK);
    work->killF = TRUE;
}
