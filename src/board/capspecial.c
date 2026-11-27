/* @@HEAD@@ */
#include "game/board/capspecial.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/coin.h"
#include "game/board/exit.h"
#include "game/board/comchoice.h"
#include "game/board/masu.h"
#include "game/board/math.h"
#include "game/board/object.h"
#include "game/board/player.h"
#include "game/board/status.h"
#include "game/board/window.h"
#include "game/board/wipe.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"

#include "humath.h"
#include "math.h"
#include "string.h"

/* The capsule event process work (HuPrcCurrentGet()->property), as captrap.c names it. */
typedef struct CapWork_s {
    /* 0x000 */ int playerNo;
    /* 0x004 */ u8 unk4[0xC];
    /* 0x010 */ int masuId;
    /* 0x014 */ u8 unk14[0xC];
    /* 0x020 */ u8 objWork[0xB48]; /* mbev_CapObjCreate's object list */
    /* 0xB68 */ u8 unkB68[4];
    /* 0xB6C */ int arg[2]; /* passed to the event's sub-steps */
    /* 0xB74 */ u8 unkB74[0x5C];
    /* 0xBD0 */ void *explodeEff;
    /* 0xBD4 */ u8 unkBD4[8];
    /* 0xBDC */ void *glowEff;
    /* 0xBE0 */ void *ringHitEff;
    /* 0xBE4 */ void *coinEff;
    /* 0xBE8 */ void *coinMan;
    /* 0xBEC */ void *starMan;
} CAPWORK;

/* The Teresa screen fade (mbev_CapTeresaFadeCreate). */
typedef struct CapTeresaFadeWork_s {
    /* 0x00 */ void *texBuf;
    /* 0x04 */ u32 texSize;
    /* 0x08 */ int mipmapF;
    /* 0x0C */ float alpha;
    /* 0x10 */ int copyF;
    /* 0x14 */ OMOBJ *obj;
    /* 0x18 */ u32 srcW;
    /* 0x1C */ u32 srcH;
    /* 0x20 */ u32 dstW;
    /* 0x24 */ u32 dstH;
} CAPTERESAFADEWORK; /* size 0x28 */

/* capevent.c; local prototypes, the B0 header's are placeholders. */
void mbev_CapWait(CAPWORK *work);
int mbev_CapObjCreate(void *objWork, int dataNum, int *motDataNum, int linkF, int arg4, int arg5);
s16 mbev_CapPlayerMotionCreate(void *objWork, int playerNo, int dataNum);
void *mbev_CapEffGlowCreate(void);
void *mbev_CapEffCoinCreate(void);
void mbev_CapEffCoinGlowSet(void *coinEff, void *glowEff);
int mbev_CapEffCoinAdd(void *coinEff, HuVecF *pos, HuVecF *vel, float scale, float gravity, int time, int maxNum);
int mbev_CapEffCoinMaxYSet(void *coinEff, int coinId, float maxY);
int mbev_CapEffCoinNumGet(void *coinEff);
int mbev_CapPlayerCheck(int playerNo, int otherNo);
int mbev_CapPlayerComSelKettouGet(int playerNo, int mode, int *choiceList, int num);
void mbev_CapPlayerMotShiftWait(int playerNo, int motNo, int attr, int arg3);
void mbev_CapCoinAdd(void *coinEff, int playerNo, int coinNum, int arg3);
int mbev_CapEffGlowAdd(void *eff, HuVecF pos, HuVecF vel, int time, float scale, float speed, float arg6, GXColor color);
void mbev_CapEffGlowKinokoTimeSet(void *eff, int glowId, int mode, int time);
void mbev_CapEffColorSet(GXColor *color, int no);
void mbev_CapPlayerRotate(int playerNo, float rotY);
void mbev_CapObjClose(void *objWork, int objId);
void mbev_CapPlayerPosSet(void *objWork, int playerNo, int masuId, HuVecF *ofs);
void *mbev_CapCoinManCreate(void);
void *mbev_CapStarManCreate(void);

/* star.h; retail calls mbStarGetExec with only the player (see report) */
int mbStarObjCreate(void);
void mbStarObjPosSetV(int no, HuVecF *pos);
void mbStarObjRotSet(int no, float rotX, float rotY, float rotZ);
void mbStarObjScaleSet(int no, float scaleX, float scaleY, float scaleZ);
void mbStarObjKill(int no);
void mbStarObjDispSet(int no, BOOL dispF);
void mbStarObjDispSetAll(BOOL effF);
void mbStarGetExec();

static inline BOOL GWTeamGet(void)
{
    return GwSystem.tagF;
}

#define CAP_RANDF() (0.0000000037252903f*mbRandMod(0x10000000))

static void ev_CapTeresaFadeMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *material);
static void ev_CapTeresaFadeOMExec(OMOBJ *obj);
static void ev_CapMiracleMasu(CAPWORK *work);
static void ev_CapMiracleRun(CAPWORK *work);

static HuVecF capsuleCameraOfs = { 0.0f, 100.0f, 0.0f };
static HuVecF teresaCameraRot = { -30.0f, 0.0f, 0.0f };
static HuVecF teresaCameraOfs = { 0.0f, 100.0f, 0.0f };
static HuVecF teresaLightPos = { 0.0f, 0.0f, 0.0f };
static HuVecF teresaLightDir = { 0.0f, 1.0f, -1.0f };

static int MiracleGuideMotTbl[2][16] = {
    { 0x110001, 0x110004, 0x110005, 0x110015, 0x110007, 0x11000B, 0x11000C, 0x11000D, 0x110013, -1 },
    { 0x11001C, 0x11001F, 0x110020, 0x11002F, 0x110022, 0x110025, 0x110026, 0x110027, 0x11002D, -1 },
};

static s32 koopaMdlId = -1;
static s32 teresaStealMesId = -1;
static GXColor teresaLightColor = { 255, 190, 255, 255 };

/* .sbss: emitted in reverse definition order */
static CAPTERESAFADEWORK *teresaFadeWork;
static s32 teresaStealCoinNum;
static void (*teresaStealBeginHook)(int playerNo, int objId);
static int (*teresaStealHook)(int startF);
static OMOBJ *miracleSprObj;
static s32 diceHitTimer;

static GXColor miracleMasuEffColorTbl[2][3] = {
    { { 255, 127, 127, 255 }, { 255, 255, 127, 255 }, { 255, 190, 127, 255 } },
    { { 127, 127, 255, 255 }, { 127, 255, 255, 255 }, { 127, 190, 255, 255 } },
};

static int miracleTradeOrderTbl[3][16][2] = {
    {
        { 0, 2 }, { 0, 1 }, { 0, 3 }, { 0, 1 }, { 0, 2 }, { 0, 1 }, { 0, 4 }, { 0, 1 },
        { 0, 1 }, { -1, 0 },
    },
    {
        { 3, 2 }, { 0, 1 }, { 0, 2 }, { 0, 1 }, { 3, 0 }, { 4, 2 }, { 3, 0 }, { 1, 0 },
        { 1, 5 }, { 2, 4 }, { -1, 0 },
    },
    {
        { 3, 0 }, { 4, 2 }, { 0, 5 }, { 1, 2 }, { 0, 3 }, { 0, 1 }, { 4, 1 }, { 3, 1 },
        { 2, 3 }, { 4, 5 }, { -1, 0 },
    },
};

static int miracleLeftCharOrderTbl[16][2] = {
    { 1, 2 }, { 1, 3 }, { 1, 0 }, { 1, 3 }, { 0, 2 }, { -1, 0 },
};

static int miracleRightCharOrderTbl[2][16][2] = {
    {
        { 2, 0 }, { 2, 1 }, { 2, 1 }, { 2, 0 }, { 2, 0 }, { -1, 0 },
    },
    {
        { 1, 0 }, { 2, 0 }, { 2, 0 }, { 1, 0 }, { 2, 0 }, { -1, 0 },
    },
};

static HuVecF miracleTradePosTbl[3] = {
    { 138.0f, 90.0f, 0.0f },
    { 288.0f, 90.0f, 0.0f },
    { 438.0f, 90.0f, 0.0f },
};

static int miracleTradeFileTbl[6] = { 0x11003B, 0x11003C, 0x11003D, 0x11003E, 0x11003F, 0x110040 };

static s32 miracleBackFile = 0x11003A;

static int kettouGuideMotTbl[2][16] = {
    { 0x110001, 0x110004, 0x110011, 0x110005, 0x110012, 0x110013, 0x110014, -1 },
    { 0x11001C, 0x11001F, 0x11002B, 0x110020, 0x11002C, 0x11002D, 0x11002E, -1 },
};

static int kettouPlayerMotTbl[2][2] = {
    { 0x8E0027, 0x93001F },
    { 0x8E0078, 0x93004B },
};

static s32 kettouMotId[12];
static s32 mgResultData[4];

static int donkeyMotTbl[12] = {
    0xE000F, 0xE0011, 0xE0012, 0xE0015, 0xE0016, 0xE0013, 0xE0014, 0xE0018, 0xE0019, 0xE001A, 0xE001B, -1
};

static s32 donkeyMgFile[2] = { 0x5008C, 0x5008C };
static s8 donkeyDiceTbl[8] = { 0, 1, 2, 3, 4, -1 };

static int donkeyDiceResultTbl[5] = { 5, 10, 20, 30, -1 };
static int donkeyRouletteBankTbl[5][2] = { { 0, 1 }, { 0, 1 }, { 0, 1 }, { 0, 1 }, { 0, 2 } };

static int koopaMotTbl[7] = { 0xE0001, 0xE0005, 0xE0003, 0xE0007, 0xE0004, 0xE000A, -1 };

static s32 koopaMgFile[2] = { 0x5008D, 0x5008D };
static s8 koopaDiceTbl[8] = { 0, 1, 2, 3, 4, -1 };

static int koopaDiceResultTbl[5] = { 5, 10, 20, 30, -1 };
static int koopaLoseMesTbl[3] = { 0x3F000A, 0x3F000B, 0x3F000C };
static int koopaLoseMesTbl2[3] = { 0x3F000A, 0x3F000B, 0x3F000C };

/* @@PART1@@ */
void mbev_CapTeresa(void)
{
    CAPWORK *work;
    int playerNo;
    int teresaObjId;
    int targetNo;
    int i;
    int choice;
    int coinPlayerNum;
    int starPlayerNum;
    int masuId;
    int hookObjId;
    int starNo;
    int lightId;
    int alpha;
    int tempNo;
    HU3D_LIGHT *lightP;
    int otherNum;
    int playerMasuId;
    int linkMasuId;
    int maxCoin;
    int stealCoin;
    int mashNum;
    int coinId;
    int capIdx;
    int validNum;
    int starNum;
    int helpWinId;
    int choiceNo;
    int retryF;
    int musFadeF;
    float t;
    float rate;
    float angleX;
    float angleY;
    Mtx hookMtx;
    int teresaMotTbl[16];
    GXColor lightColor[8];
    HuVecF playerPos;
    HuVecF targetPos;
    HuVecF teresaPos;
    HuVecF dir;
    HuVecF pos;
    HuVecF aimPos;
    HuVecF coinPos;
    HuVecF coinVel;
    int otherPlayer[4];
    int motId[3];
    int choiceList[4];
    char coinStr[16];

    work = HuPrcCurrentGet()->property;
    musFadeF = FALSE;
    mbev_CapWait(work);
    if(!GwSystem.curTime) {
        mbPlayerMotionShiftSet(work->playerNo, 1, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
        mbWinCreate(2, 0x3B0000, 10);
        mbWinTopWait();
        HuPrcEnd();
        return;
    }
    work->glowEff = mbev_CapEffGlowCreate();
    work->coinEff = mbev_CapEffCoinCreate();
    mbev_CapEffCoinGlowSet(work->coinEff, work->glowEff);
    playerNo = work->playerNo;
    playerMasuId = GwPlayer[playerNo].masuId;
    mbPlayerPosGet(playerNo, &playerPos);
    linkMasuId = mbMasuAttrFindLink(playerMasuId, 0x2000);
    if(linkMasuId != -1) {
        mbMasuPosGet(linkMasuId, &teresaPos);
        VECSubtract(&teresaPos, &playerPos, &dir);
        dir.y = 0.0f;
        if(VECMag(&dir) > 0.0f) {
            VECNormalize(&dir, &dir);
        }
        VECScale(&dir, &dir, 300.0f);
        VECAdd(&playerPos, &dir, &teresaPos);
        teresaPos.y += 125.0f;
    } else {
        teresaPos = playerPos;
        teresaPos.y -= 100.0f;
        teresaPos.z -= 200.0f;
    }
    VECSubtract(&teresaPos, &playerPos, &dir);
    teresaMotTbl[0] = 0x130009;
    teresaMotTbl[1] = 0x13000B;
    teresaMotTbl[2] = 0x13000A;
    teresaMotTbl[3] = -1;
    teresaObjId = mbev_CapObjCreate(work->objWork, 0x130008, teresaMotTbl, 0, 5, 0);
    mbObjMotionSet(teresaObjId, 1, HU3D_MOTATTR_LOOP);
    mbObjLayerSet(teresaObjId, 4);
    mbObjScaleSet(teresaObjId, 2.0f, 2.0f, 2.0f);
    mbObjPosSetV(teresaObjId, &teresaPos);
    mbObjRotSet(teresaObjId, 0.0f, 180.0+HuAtan(dir.x, dir.z), 0.0f);
    mbev_CapTeresaFadeCreate(teresaObjId);
    mbPlayerRotSet(playerNo, 0.0f, HuAtan(dir.x, dir.z), 0.0f);
    for(i = 0; i < 4; i++) {
        if(i != playerNo) {
            mbPlayerDispSet(i, FALSE);
        }
    }
    pos = teresaCameraRot;
    pos.y = 180.0+HuAtan(dir.x, dir.z);
    mbCameraMovePlayer(playerNo, &pos, &teresaCameraOfs, 1500.0f, -1.0f, -1);
    mbCameraMoveWait();
    mbMusBoardFadeOut(0, 0, 1000, 1000, 32, FALSE);
    mbWipeDissolveFadeIn();
    if(!GwSystem.curTime) {
        mbAudFXPlay(0x39D);
        mbWinCreate(2, 0x3B0000, 10);
        mbWinTopWait();
        goto end;
    }
    if(mbPlayerCoinGet(playerNo) < 5) {
        mbAudFXPlay(0x39D);
        mbWinCreate(2, 0x3B0001, 10);
        mbWinTopWait();
        goto end;
    }
    coinPlayerNum = starPlayerNum = 0;
    if(mbPlayerCoinGet(playerNo) >= 5) {
        for(i = 0; i < 4; i++) {
            if(!mbev_CapPlayerCheck(i, playerNo) && mbPlayerCoinGet(i) > 0) {
                coinPlayerNum++;
            }
        }
    }
    if(mbPlayerCoinGet(playerNo) >= 40) {
        for(i = 0; i < 4; i++) {
            if(!mbev_CapPlayerCheck(i, playerNo) && mbPlayerStarGet(i) > 0) {
                starPlayerNum++;
            }
        }
    }
    for(i = 0, otherNum = 0; i < 4; i++) {
        if(i != playerNo) {
            otherPlayer[otherNum] = i;
            otherNum++;
        }
    }
    for(i = 0, starNum = 0; i < 4; i++) {
        if(!mbev_CapPlayerCheck(i, playerNo) && mbPlayerStarGet(i) > 0) {
            starNum++;
        }
    }
    if(coinPlayerNum <= 0 && starPlayerNum <= 0 && mbPlayerCoinGet(playerNo) < 40 && starNum >= 1) {
        mbAudFXPlay(0x39D);
        mbWinCreate(2, 0x3B0001, 10);
        mbWinTopWait();
        goto end;
    }
    if(coinPlayerNum <= 0 && starPlayerNum <= 0) {
        mbAudFXPlay(0x39D);
        mbWinCreate(2, 0x3B0002, 10);
        mbWinTopWait();
        goto end;
    }
    if(teresaStealMesId == -1) {
        mbAudFXPlay(0x39D);
        mbWinCreate(2, 0x3B0003, 10);
        mbWinTopWait();
    } else {
        mbAudFXPlay(0x39D);
        mbWinCreate(2, 0x3B0004, 10);
        mbWinTopWait();
    }
    do {
        if(teresaStealMesId == -1) {
            mbWinCreateChoice(1, 0x3B0005, 10, 0);
            if(coinPlayerNum == 0) {
                mbWinTopChoiceDisable(0);
            }
            if(starPlayerNum == 0) {
                mbWinTopChoiceDisable(1);
            }
            if(GwPlayer[playerNo].comF) {
                if(coinPlayerNum != 0 && starPlayerNum != 0) {
                    mbComChoiceListDownSet(1);
                } else {
                    mbComChoiceListDownSet(0);
                }
            }
            mbWinTopWait();
            choice = mbWinTopChoiceGet();
            if(choice == 2 || choice == -1) {
                mbWinCreate(2, 0x3B000F, 10);
                mbWinTopWait();
                targetNo = -1;
                goto end;
            }
        } else {
            mbWinCreateChoice(1, 0x3B0006, 10, 0);
            sprintf(coinStr, "%d", teresaStealCoinNum);
            mbWinTopInsertMesSet(teresaStealMesId, 0);
            mbWinTopInsertMesSet((u32)coinStr, 1);
            if(coinPlayerNum == 0) {
                mbWinTopChoiceDisable(0);
            }
            if(starPlayerNum == 0) {
                mbWinTopChoiceDisable(1);
            }
            if(mbPlayerCoinGet(playerNo) < teresaStealCoinNum) {
                mbWinTopChoiceDisable(2);
            }
            if(GwPlayer[playerNo].comF) {
                if(coinPlayerNum != 0 && starPlayerNum != 0) {
                    mbComChoiceListDownSet(1);
                } else {
                    mbComChoiceListDownSet(0);
                }
            }
            mbWinTopWait();
            choice = mbWinTopChoiceGet();
            if(choice == 3 || choice == -1) {
                mbWinCreate(2, 0x3B000F, 10);
                mbWinTopWait();
                targetNo = -1;
                goto end;
            }
            if(choice == 2) {
                break;
            }
        }
        mbWinCreateChoice(1, 0x3B0007, 10, 0);
        mbWinTopInsertMesSet(mbPlayerNameMesGet(otherPlayer[0]), 0);
        mbWinTopInsertMesSet(mbPlayerNameMesGet(otherPlayer[1]), 1);
        mbWinTopInsertMesSet(mbPlayerNameMesGet(otherPlayer[2]), 2);
        for(i = 0; i < 3; i++) {
            choiceList[i] = otherPlayer[i];
        }
        if(choice == 0) {
            for(i = 0; i < 3; i++) {
                if(mbPlayerCoinGet(otherPlayer[i]) <= 0) {
                    mbWinTopChoiceDisable(i);
                    choiceList[i] = -1;
                }
            }
        } else {
            for(i = 0; i < 3; i++) {
                if(mbPlayerStarGet(otherPlayer[i]) <= 0) {
                    mbWinTopChoiceDisable(i);
                    choiceList[i] = -1;
                }
            }
        }
        for(i = 0; i < 3; i++) {
            if(GWTeamGet() && mbev_CapPlayerCheck(playerNo, otherPlayer[i])) {
                mbWinTopChoiceDisable(i);
                choiceList[i] = -1;
            }
        }
        for(i = 0, validNum = 0; i < 3; i++) {
            if(choiceList[i] >= 0) {
                validNum++;
            }
        }
        if(GwPlayer[playerNo].comF) {
            if(choice == 0) {
                tempNo = mbev_CapPlayerComSelKettouGet(playerNo, 0, choiceList, 3);
                mbComChoiceListDownSet(tempNo);
            } else {
                tempNo = mbev_CapPlayerComSelKettouGet(playerNo, 1, choiceList, 3);
                mbComChoiceListDownSet(tempNo);
            }
        }
        mbWinTopWait();
        choiceNo = mbWinTopChoiceGet();
        retryF = FALSE;
        if(choiceNo == -1) {
            retryF = TRUE;
        } else if(choiceNo < 3) {
            targetNo = otherPlayer[choiceNo];
        } else {
            tempNo = mbRandMod(3);
            targetNo = -1;
            for(i = 0; i < 3; i++) {
                if(choiceList[tempNo] >= 0) {
                    targetNo = choiceList[tempNo];
                    break;
                }
                tempNo++;
                if(tempNo >= 3) {
                    tempNo = 0;
                }
            }
        }
    } while(retryF);
    if(teresaStealMesId != -1 && choice == 2) {
        mbCoinAddExec(playerNo, -teresaStealCoinNum);
    } else if(choice == 0) {
        mbCoinAddExec(playerNo, -5);
    } else {
        mbCoinAddExec(playerNo, -40);
    }
    mbAudFXPlay(0x39D);
    mbWinCreate(2, 0x3B0008, 10);
    mbWinTopWait();
    for(i = 1; i <= 60.0f; i++) {
        t = i/60.0f;
        mbev_CapTeresaFadeSet(255.0f*(1.0f-t));
        HuPrcVSleep();
    }
    mbev_CapTeresaFadeSet(0.0f);
    if(teresaStealMesId != -1 && choice == 2) {
        if(teresaStealHook != NULL) {
            teresaStealHook(TRUE);
        } else {
            mbWipeDissolveFadeOutTime(1);
        }
        teresaStealBeginHook(playerNo, teresaObjId);
        goto stealEnd;
    }
    mbWipeDissolveFadeOutTime(1);
    for(i = 0, capIdx = -1; i < mbPlayerCapsuleMaxGet(); i++) {
        if(mbPlayerCapsuleGet(targetNo, i) == 31) {
            capIdx = i;
            break;
        }
    }
    masuId = GwPlayer[targetNo].masuId;
    mbPlayerPosGet(targetNo, &targetPos);
    mbev_PlayerColMasu(targetNo, masuId, TRUE);
    for(i = 0; i < 4; i++) {
        mbPlayerDispSet(i, TRUE);
        if(i != targetNo) {
            mbPlayerDispSet(i, FALSE);
        }
    }
    mbCameraPlayerViewSetFast(targetNo, 0);
    mbCameraMoveWait();
    mbMasuPosGet(masuId, &pos);
    pos.y += 300.0f;
    if(capIdx != -1) {
        pos.y -= 150.0f;
        pos.z -= 200.0f;
    }
    mbObjPosSetV(teresaObjId, &pos);
    mbObjRotSet(teresaObjId, 0.0f, 0.0f, 0.0f);
    mbObjDispSet(teresaObjId, FALSE);
    motId[0] = mbev_CapPlayerMotionCreate(work->objWork, targetNo, 0x930017);
    motId[1] = mbev_CapPlayerMotionCreate(work->objWork, targetNo, 0x930022);
    motId[2] = mbev_CapPlayerMotionCreate(work->objWork, targetNo, 0x93006E);
    mbPlayerMotionShiftSet(targetNo, motId[0], 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
    hookObjId = mbev_CapObjCreate(work->objWork, 0xC0049, NULL, 0, 0, 0);
    mbObjHookSet(mbPlayerObjIDGet(targetNo), CharModelItemHookGet(GwPlayer[targetNo].charNo, 4, 0), hookObjId);
    mbObjDispSet(hookObjId, FALSE);
    for(lightP = Hu3DGlobalLight, i = 0; i < 8; i++, lightP++) {
        if(lightP->type != -1) {
            lightColor[i] = lightP->color;
            lightP->color.r *= 0.5f;
            lightP->color.g *= 0.5f;
            lightP->color.b *= 0.5f;
        }
    }
    lightId = -1;
    mbStarObjDispSetAll(FALSE);
    mbWipeDissolveFadeIn();
    if(capIdx != -1) {
        mbWinCreate(2, 0x3B000A, -1);
        mbWinTopInsertMesSet(mbPlayerNameMesGet(targetNo), 0);
        mbWinTopPlayerDisable(targetNo);
        mbWinTopWait();
        mbObjLayerSet(hookObjId, 5);
        mbPlayerLayerSet(targetNo, 5);
        mbObjDispSet(teresaObjId, TRUE);
        for(i = 1; i <= 60.0f; i++) {
            t = i/60.0f;
            mbev_CapTeresaFadeSet(255.0f*t);
            HuPrcVSleep();
        }
        mbev_CapTeresaFadeSet(255.0f);
        mbPlayerRotateStart(targetNo, 180, 15);
        while(!mbPlayerRotateCheck(targetNo)) {
            HuPrcVSleep();
        }
        omVibrate(targetNo, 20, 7, 3);
        mbPlayerMotionShiftSet(targetNo, motId[2], 0.0f, 8.0f, 0);
        mbObjDispSet(hookObjId, TRUE);
        for(i = 1; i < 18.0f; i++) {
            t = i/18.0f;
            mbObjScaleSet(hookObjId, t, t, t);
            HuPrcVSleep();
        }
        mbObjScaleSet(hookObjId, 1.0f, 1.0f, 1.0f);
        while(!mbPlayerMotionEndCheck(targetNo)) {
            HuPrcVSleep();
        }
        lightId = Hu3DLLightCreateV(mbObjModelIDGet(teresaObjId), &teresaLightPos, &teresaLightDir, &teresaLightColor);
        Hu3DLLightSpotSet(mbObjModelIDGet(teresaObjId), lightId, 0.001f, GX_SP_SHARP);
        Hu3DLLightStaticSet(mbObjModelIDGet(teresaObjId), lightId, TRUE);
        Hu3DLLightInfinitytSet(mbObjModelIDGet(teresaObjId), lightId);
        Hu3DMotionCalc(mbObjModelIDGet(mbPlayerObjIDGet(playerNo)));
        Hu3DModelObjMtxGet(mbObjModelIDGet(mbPlayerObjIDGet(playerNo)), CharModelItemHookGet(GwPlayer[playerNo].charNo, 4, 0), hookMtx);
        pos.x = hookMtx[0][3];
        pos.y = hookMtx[1][3];
        pos.z = hookMtx[2][3];
        aimPos.x = pos.x;
        aimPos.y = pos.y-100.0f;
        aimPos.z = 200.0f+pos.z;
        Hu3DLLightPosAimSetV(mbObjModelIDGet(teresaObjId), lightId, &pos, &aimPos);
        Hu3DLLightPosAngleSet(mbObjModelIDGet(teresaObjId), lightId, pos.x, pos.y, pos.z, -45.0f, 0.0f);
        mbObjMotionShiftSet(teresaObjId, 3, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
        HuPrcSleep(180);
        mbMusBoardFadeOut(0, 0, 1000, 1000, -1, FALSE);
        musFadeF = TRUE;
        choice = -1;
    } else if(choice == 0) {
        mbWinCreate(2, 0x3B0009, -1);
        mbWinTopInsertMesSet(mbPlayerNameMesGet(targetNo), 0);
        mbWinTopPlayerDisable(targetNo);
        mbWinTopWait();
        mbObjDispSet(teresaObjId, TRUE);
        for(i = 1; i <= 60.0f; i++) {
            t = i/60.0f;
            mbev_CapTeresaFadeSet(255.0f*t);
            HuPrcVSleep();
        }
        mbev_CapTeresaFadeSet(255.0f);
        mbPlayerMotionShiftSet(targetNo, motId[1], 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
        mbObjMotionShiftSet(teresaObjId, 2, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
        helpWinId = mbWinCreateHelp(0x3B0011);
        mbPlayerColSnapPlayerSet(targetNo, FALSE);
        for(i = 0; i < 30.0f; i++) {
            t = i/30.0f;
            mbMasuPosGet(masuId, &pos);
            pos.y += 100.0f*(3.0f-(2.0f*t));
            alpha = 255.0f-(127.0f*t);
            mbObjPosSetV(teresaObjId, &pos);
            mbev_CapTeresaFadeSet(alpha);
            mbMasuPosGet(masuId, &pos);
            pos.y += 50.0f*t;
            mbPlayerPosSetV(targetNo, &pos);
            HuPrcVSleep();
        }
        for(i = 0, mashNum = 0; i < 120.0f; i++) {
            t = i/120.0f;
            mbMasuPosGet(masuId, &pos);
            pos.y += 100.0+(20.0*HuSin(1440.0f*t));
            mbObjPosSetV(teresaObjId, &pos);
            mbev_CapTeresaFadeSet(alpha);
            if(HuPadBtnDown[GwPlayer[targetNo].padNo] & PAD_BUTTON_A) {
                mashNum++;
                if(alpha > 0) {
                    alpha--;
                }
            }
            alpha += 2.5*HuSin(360.0f*t);
            if(alpha < 64 && (i & 7) == 0) {
                alpha++;
            }
            if(alpha < 32) {
                alpha = 32;
            } else if(alpha > 255) {
                alpha = 255;
            }
            mbMasuPosGet(masuId, &pos);
            pos.y += 50.0f;
            mbPlayerPosSetV(targetNo, &pos);
            HuPrcVSleep();
        }
        mbWinKill(helpWinId);
        for(i = 0; i < 30.0f; i++) {
            t = 1.0f-(i/30.0f);
            mbMasuPosGet(masuId, &pos);
            pos.y += 100.0f+(100.0f*(2.0f-(2.0f*t)));
            mbObjPosSetV(teresaObjId, &pos);
            if(alpha < 255) {
                alpha += 25;
            }
            if(alpha > 255) {
                alpha = 255;
            }
            mbev_CapTeresaFadeSet(alpha);
            mbMasuPosGet(masuId, &pos);
            pos.y += 50.0f*t;
            mbPlayerPosSetV(targetNo, &pos);
            HuPrcVSleep();
        }
        mbPlayerMotionShiftSet(targetNo, 6, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
        mbPlayerColSnapPlayerSet(targetNo, TRUE);
        mbObjMotionShiftSet(teresaObjId, 1, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
        if(GwSystem.turnNo <= 10) {
            maxCoin = 20;
        } else if(GwSystem.turnNo <= 20) {
            maxCoin = 25;
        } else if(GwSystem.turnNo <= 30) {
            maxCoin = 30;
        } else if(GwSystem.turnNo <= 40) {
            maxCoin = 35;
        } else {
            maxCoin = 40;
        }
        if(!GwPlayer[targetNo].comF) {
            rate = 1.0f-(mashNum/32.0f);
        } else {
            rate = 1.0f-(0.1f+(GwPlayer[targetNo].comDif*(0.2f+(0.1f*CAP_RANDF()))));
        }
        if(rate < 0.1f) {
            rate = 0.1f;
        } else if(rate > 1.0f) {
            rate = 1.0f;
        }
        stealCoin = rate*maxCoin;
        if(stealCoin > mbPlayerCoinGet(targetNo)) {
            stealCoin = mbPlayerCoinGet(targetNo);
        }
        omVibrate(targetNo, 20, 7, 3);
        mbMasuPosGet(masuId, &pos);
        for(i = 0; i < stealCoin; i++) {
            angleY = 360.0f*CAP_RANDF();
            coinPos = pos;
            coinPos.y += 100.0f;
            angleX = 70.0f+(15.0f*CAP_RANDF());
            t = 65.0f*(0.8f+(0.3f*CAP_RANDF()));
            coinVel.x = t*(HuSin(angleY)*HuCos(angleX));
            coinVel.z = t*(HuCos(angleY)*HuCos(angleX));
            coinVel.y = t*HuSin(angleX);
            coinId = mbev_CapEffCoinAdd(work->coinEff, &coinPos, &coinVel, 0.75f, 4.9f, 30, 0xFFFF);
            if(coinId >= 0) {
                mbev_CapEffCoinMaxYSet(work->coinEff, coinId, 300.0f+pos.y);
            }
            mbPlayerCoinAdd(targetNo, -1);
            mbAudFXPlay(14);
            HuPrcVSleep();
        }
        mbAudFXPlay(15);
        while(mbev_CapEffCoinNumGet(work->coinEff) > 0) {
            HuPrcVSleep();
        }
    } else {
        mbWinCreate(2, 0x3B000A, -1);
        mbWinTopInsertMesSet(mbPlayerNameMesGet(targetNo), 0);
        mbWinTopPlayerDisable(targetNo);
        mbWinTopWait();
        mbObjDispSet(teresaObjId, TRUE);
        for(i = 1; i <= 60.0f; i++) {
            t = i/60.0f;
            mbev_CapTeresaFadeSet(255.0f*t);
            HuPrcVSleep();
        }
        mbev_CapTeresaFadeSet(255.0f);
        starNo = mbStarObjCreate();
        mbStarObjDispSet(starNo, FALSE);
        mbPlayerMotionShiftSet(targetNo, motId[1], 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
        mbObjMotionShiftSet(teresaObjId, 2, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
        mbPlayerColSnapPlayerSet(targetNo, FALSE);
        for(i = 0; i < 30.0f; i++) {
            t = i/30.0f;
            mbMasuPosGet(masuId, &pos);
            pos.y += 100.0f*(3.0f-(2.0f*t));
            alpha = 255.0f-(127.0f*t);
            mbObjPosSetV(teresaObjId, &pos);
            mbev_CapTeresaFadeSet(alpha);
            mbMasuPosGet(masuId, &pos);
            pos.y += 50.0f*t;
            mbPlayerPosSetV(targetNo, &pos);
            HuPrcVSleep();
        }
        for(i = 0; i < 30.0f; i++) {
            t = 1.0f-(i/30.0f);
            mbMasuPosGet(masuId, &pos);
            pos.y += 100.0f+(100.0f*(2.0f-(2.0f*t)));
            mbObjPosSetV(teresaObjId, &pos);
            if(alpha < 255) {
                alpha += 25;
            }
            if(alpha > 255) {
                alpha = 255;
            }
            mbev_CapTeresaFadeSet(alpha);
            mbMasuPosGet(masuId, &pos);
            pos.y += 50.0f*t;
            mbPlayerPosSetV(targetNo, &pos);
            HuPrcVSleep();
        }
        mbPlayerMotionShiftSet(targetNo, 6, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
        mbPlayerColSnapPlayerSet(targetNo, TRUE);
        mbPlayerStarAdd(targetNo, -1);
        omVibrate(targetNo, 20, 20, 0);
        for(i = 0; i <= 60.0f; i++) {
            t = i/60.0f;
            t = HuSin(90.0f*t);
            mbMasuPosGet(masuId, &pos);
            pos.y += 3.0f*(100.0f*t);
            mbStarObjPosSetV(starNo, &pos);
            mbStarObjRotSet(starNo, 0.0f, 360.0f*t, 0.0f);
            mbStarObjScaleSet(starNo, t, t, t);
            mbStarObjDispSet(starNo, TRUE);
            mbMasuPosGet(masuId, &pos);
            pos.y += 300.0f;
            pos.z -= 2.0f*(100.0f*t);
            mbObjPosSetV(teresaObjId, &pos);
            HuPrcVSleep();
        }
        for(i = 0; i <= 60.0f; i++) {
            t = i/60.0f;
            t = HuSin(90.0f*t);
            mbMasuPosGet(masuId, &pos);
            pos.y += 300.0f+(5.0f*(100.0f*t));
            mbStarObjPosSetV(starNo, &pos);
            mbStarObjRotSet(starNo, 0.0f, 720.0f*t, 0.0f);
            HuPrcVSleep();
        }
        mbStarObjKill(starNo);
    }
    mbWipeDissolveFadeOutTime(1);
    mbStarObjDispSetAll(TRUE);
    for(lightP = Hu3DGlobalLight, i = 0; i < 8; i++, lightP++) {
        if(lightP->type != -1) {
            lightP->color = lightColor[i];
        }
    }
    if(lightId != -1) {
        Hu3DLLightKill(mbObjModelIDGet(teresaObjId), lightId);
    }
    if(capIdx != -1) {
        mbPlayerCapsuleRemove(targetNo, capIdx);
        GwPlayer[targetNo].capsuleUseNum++;
    }
    mbPlayerLayerSet(targetNo, 3);
    mbObjHookObjReset(mbPlayerObjIDGet(targetNo), CharModelItemHookGet(GwPlayer[targetNo].charNo, 4, 0));
    mbObjDispSet(hookObjId, FALSE);
    mbPlayerPosSetV(targetNo, &targetPos);
    mbPlayerRotSet(targetNo, 0.0f, 0.0f, 0.0f);
    mbPlayerMotionSet(targetNo, 1, HU3D_MOTATTR_LOOP);
stealEnd:
    mbObjMotionShiftSet(teresaObjId, 1, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
    VECSubtract(&teresaPos, &playerPos, &dir);
    mbObjPosSetV(teresaObjId, &teresaPos);
    mbObjRotSet(teresaObjId, 0.0f, 180.0+HuAtan(dir.x, dir.z), 0.0f);
    mbev_CapTeresaFadeSet(0.0f);
    mbev_PlayerColMasu(playerNo, GwPlayer[playerNo].masuId, TRUE);
    mbPlayerRotSet(playerNo, 0.0f, HuAtan(dir.x, dir.z), 0.0f);
    for(i = 0; i < 4; i++) {
        mbPlayerDispSet(i, TRUE);
        if(i != playerNo) {
            mbPlayerDispSet(i, FALSE);
        }
    }
    pos = teresaCameraRot;
    pos.y = 180.0+HuAtan(dir.x, dir.z);
    mbCameraMovePlayer(playerNo, &pos, &teresaCameraOfs, 1500.0f, -1.0f, -1);
    mbCameraMoveWait();
    if(teresaStealMesId != -1 && choice == 2) {
        if(teresaStealHook != NULL) {
            teresaStealHook(FALSE);
        } else {
            mbWipeDissolveFadeIn();
        }
    } else {
        mbWipeDissolveFadeIn();
    }
    for(i = 1; i <= 60.0f; i++) {
        t = i/60.0f;
        mbev_CapTeresaFadeSet(255.0f*t);
        HuPrcVSleep();
    }
    mbev_CapTeresaFadeSet(255.0f);
    switch(choice) {
        case 0:
            mbAudFXPlay(0x39D);
            mbWinCreate(2, 0x3B000B, 10);
            mbWinTopWait();
            mbPlayerRotateStart(playerNo, 180.0+HuAtan(dir.x, dir.z), 15);
            while(!mbPlayerRotateCheck(playerNo)) {
                HuPrcVSleep();
            }
            mbev_CapCoinAdd(work->coinEff, playerNo, stealCoin, 1);
            mbAudFXPlay(0x39D);
            mbWinCreate(2, 0x3B000D, 10);
            mbWinTopWait();
            break;

        case 1:
            mbAudFXPlay(0x39D);
            mbWinCreate(2, 0x3B000C, 10);
            mbWinTopWait();
            mbMusFadeOutSpeed(1, 1000);
            while(mbMusCheck(1)) {
                HuPrcVSleep();
            }
            mbPlayerRotateStart(playerNo, 180.0+HuAtan(dir.x, dir.z), 15);
            while(!mbPlayerRotateCheck(playerNo)) {
                HuPrcVSleep();
            }
            mbStarGetExec(playerNo);
            mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
            mbMusBoardPlay();
            musFadeF = TRUE;
            mbAudFXPlay(0x39D);
            mbWinCreate(2, 0x3B000D, 10);
            mbWinTopWait();
            break;

        case 2:
            mbAudFXPlay(0x39D);
            mbWinCreate(2, 0x3B000E, 10);
            mbWinTopWait();
            break;

        default:
            mbAudFXPlay(0x39E);
            mbWinCreate(2, 0x3B0010, 10);
            mbWinTopWait();
            mbPlayerRotateStart(playerNo, 180.0+HuAtan(dir.x, dir.z), 15);
            while(!mbPlayerRotateCheck(playerNo)) {
                HuPrcVSleep();
            }
            mbev_CapPlayerMotShiftWait(playerNo, 13, 0, 1);
            break;
    }
end:
    if(!musFadeF) {
        mbMusBoardFadeOut(0, 0, 1000, 1000, -1, FALSE);
    }
    mbWipeDissolveFadeOutTime(1);
    mbObjDispSet(teresaObjId, FALSE);
    mbev_CapTeresaFadeKill(teresaObjId);
    mbPlayerRotSet(playerNo, 0.0f, 0.0f, 0.0f);
    mbPlayerMotionSet(playerNo, 1, HU3D_MOTATTR_LOOP);
    if(!mbMasuDispCheck(GwPlayer[playerNo].masuId) || GwPlayer[playerNo].moveNum > 1) {
        mbCameraPlayerViewSetFast(playerNo, 2);
    } else {
        mbCameraPlayerViewSetFast(playerNo, 0);
    }
    mbCameraMoveWait();
    for(i = 0; i < 4; i++) {
        mbPlayerDispSet(i, TRUE);
    }
    HuDataDirClose(0x130000);
    HuPrcEnd();
}

void mbev_CapTeresaKill(void)
{
}

void mbev_CapTeresaStealSet(int mesId, int coinNum, void *beginHook, void *hook)
{
    teresaStealMesId = mesId;
    teresaStealCoinNum = coinNum;
    teresaStealBeginHook = beginHook;
    teresaStealHook = hook;
}

static inline void *CapTeresaMallocNum(s32 size, u32 num)
{
    return HuMemDirectMallocNum(HEAP_MODEL, size, num);
}

void mbev_CapTeresaFadeCreate(int objId)
{
    int mdlId;
    HU3D_MODEL *modelP;
    HSF_DATA *hsf;
    HSF_MATERIAL *material;
    int i;

    mdlId = mbObjModelIDGet(objId);
    modelP = &Hu3DData[mdlId];
    hsf = modelP->hsf;
    material = hsf->material;
    Hu3DModelMatHookSet(mdlId, ev_CapTeresaFadeMatHook);
    for(i = 0; i < hsf->materialNum; i++, material++) {
        material->flags |= 0x2000;
    }
    teresaFadeWork = CapTeresaMallocNum(sizeof(CAPTERESAFADEWORK), modelP->mallocNo);
    memset(teresaFadeWork, 0, sizeof(CAPTERESAFADEWORK));
    teresaFadeWork->mipmapF = TRUE;
    teresaFadeWork->alpha = 255.0f;
    teresaFadeWork->copyF = FALSE;
    teresaFadeWork->srcW = 640;
    teresaFadeWork->srcH = 480;
    teresaFadeWork->dstW = 320;
    teresaFadeWork->dstH = 240;
    teresaFadeWork->obj = omAddObjEx(mbObjMan, 32768, 0, 0, -1, ev_CapTeresaFadeOMExec);
    teresaFadeWork->texSize = GXGetTexBufferSize(teresaFadeWork->dstW, teresaFadeWork->dstH, GX_TF_RGB565, GX_FALSE, 0);
    teresaFadeWork->texBuf = CapTeresaMallocNum(teresaFadeWork->texSize, modelP->mallocNo);
    memset(teresaFadeWork->texBuf, 0, teresaFadeWork->texSize);
    DCFlushRange(teresaFadeWork->texBuf, teresaFadeWork->texSize);
}

void mbev_CapTeresaFadeKill(int objId)
{
    Hu3DModelMatHookSet(mbObjModelIDGet(objId), NULL);
    if(teresaFadeWork) {
        HuMemDirectFree(teresaFadeWork->texBuf);
        HuMemDirectFree(teresaFadeWork);
        teresaFadeWork = NULL;
    }
}

static void ev_CapTeresaFadeMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *material)
{
    CAPTERESAFADEWORK *work;
    HU3D_CAMERA *cameraP;
    float fov;
    GXColor color;
    GXTexObj texObj;
    Mtx lightMtx;
    Mtx invCamMtx;
    Mtx objMtx;
    Mtx texMtx;

    work = teresaFadeWork;
    if(work) {
        if(work->copyF == FALSE) {
            GXDrawDone();
            GXSetTexCopySrc(0, 0, work->srcW, work->srcH);
            GXSetTexCopyDst(work->dstW, work->dstH, GX_TF_RGB565, work->mipmapF);
            GXCopyTex(work->texBuf, GX_FALSE);
            GXPixModeSync();
            work->copyF = TRUE;
        }
        if(material->attrNum == 1) {
            GXSetNumTexGens(2);
            GXSetNumTevStages(2);
            GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_1);
            GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            Hu3DMatLightSet(drawObj->model, 0, material->hiliteScale);
            cameraP = &Hu3DCamera[Hu3DCameraNo];
            fov = cameraP->fov;
            if(fov <= 0.0f) {
                fov = 30.0f;
            }
            C_MTXLightPerspective(lightMtx, fov, 1.2f, 0.5f, -0.5f, 0.5f, 0.5f);
            PSMTXInverse(Hu3DCameraMtx, invCamMtx);
            PSMTXConcat(invCamMtx, drawObj->matrix, objMtx);
            PSMTXConcat(lightMtx, Hu3DCameraMtx, texMtx);
            PSMTXConcat(texMtx, objMtx, texMtx);
            GXLoadTexMtxImm(texMtx, GX_TEXMTX1, GX_MTX3x4);
            GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
            color.r = color.g = color.b = 255;
            color.a = teresaFadeWork->alpha;
            GXSetTevColor(GX_TEVREG2, color);
            GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
            GXSetTevKAlphaSel(GX_TEVSTAGE1, GX_TEV_KASEL_1);
            GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_TEXC, GX_CC_CPREV, GX_CC_A2, GX_CC_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
            GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXInitTexObj(&texObj, work->texBuf, work->dstW, work->dstH, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
            GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
            GXLoadTexObj(&texObj, GX_TEXMAP1);
        } else {
            return;
        }
    }
}

static void ev_CapTeresaFadeOMExec(OMOBJ *obj)
{
    if(mbExitCheck() || !teresaFadeWork) {
        omDelObjEx(mbObjMan, obj);
    } else {
        teresaFadeWork->copyF = FALSE;
    }
}

void mbev_CapTeresaFadeSet(float alpha)
{
    if(teresaFadeWork) {
        if(alpha < 0.0f) {
            alpha = 0.0f;
        }
        if(alpha > 255.0f) {
            alpha = 255.0f;
        }
        teresaFadeWork->alpha = alpha;
    }
}

void mbev_CapMiracle(void)
{
    CAPWORK *work;
    int playerNo;
    int masuId;
    int i;
    int startMasuId;
    int guideObjId;
    HuVecF playerPosTbl[4];
    HuVecF playerPos;
    HuVecF rot;
    HuVecF ofs;
    HuVecF masuPos;
    HuVecF objPos;
    HuVecF cameraRot;
    int objId[2];

    work = HuPrcCurrentGet()->property;
    mbev_CapWait(work);
    work->glowEff = mbev_CapEffGlowCreate();
    playerNo = work->playerNo;
    masuId = GwPlayer[playerNo].masuId;
    mbPlayerPosGet(playerNo, &playerPos);
    mbMasuPosGet(masuId, &masuPos);
    for(i = 0; i < 4; i++) {
        mbPlayerPosGet(i, &playerPosTbl[i]);
    }
    mbCameraPlayerViewSet(playerNo, 0);
    mbev_CapPlayerRotate(playerNo, 0.0f);
    objId[0] = mbev_CapObjCreate(work->objWork, 0x110000, MiracleGuideMotTbl[0], 0, 5, 0);
    mbObjMotionSet(objId[0], 3, 0);
    mbObjDispSet(objId[0], FALSE);
    objId[1] = mbev_CapObjCreate(work->objWork, 0x11001B, MiracleGuideMotTbl[1], 0, 5, 0);
    mbObjHookSet(objId[1], "itemhook_R", mbev_CapObjCreate(work->objWork, 0x110035, NULL, 0, 5, 0));
    HuPrcVSleep();
    mbObjMotionSet(objId[1], 3, 0);
    mbObjDispSet(objId[1], FALSE);
    work->arg[0] = objId[0];
    work->arg[1] = objId[1];
    ev_CapMiracleMasu(work);
    mbWipeDissolveFadeOutTime(1);
    for(i = 1; i < mbMasuNumGet(); i++) {
        if(mbMasuAttrGet(i) & 0x8000) {
            break;
        }
    }
    if(i < mbMasuNumGet()) {
        startMasuId = i;
    } else {
        startMasuId = 1;
    }
    if(!GwSystem.curTime) {
        guideObjId = objId[0];
    } else {
        guideObjId = objId[1];
    }
    for(i = 0; i < 2; i++) {
        if(guideObjId != objId[i]) {
            mbev_CapObjClose(work->objWork, objId[i]);
        }
    }
    HuPrcSleep(3);
    for(i = 0; i < 4; i++) {
        if(i != playerNo) {
            mbPlayerDispSet(i, FALSE);
        }
    }
    mbMasuPosGet(startMasuId, &masuPos);
    mbPlayerPosSetV(playerNo, &masuPos);
    mbPlayerMotionSet(playerNo, 1, HU3D_MOTATTR_LOOP);
    ofs.x = 200.0f;
    ofs.y = 0.0f;
    ofs.z = 0.0f;
    mbev_CapPlayerPosSet(work->objWork, playerNo, startMasuId, &ofs);
    objPos.x = masuPos.x-200.0f;
    objPos.y = 200.0f+masuPos.y;
    objPos.z = 0.0f;
    mbObjPosSetV(guideObjId, &objPos);
    mbObjRotSet(guideObjId, 0.0f, 0.0f, 0.0f);
    mbObjMotionSet(guideObjId, 1, HU3D_MOTATTR_LOOP);
    mbCameraRotGet(&cameraRot);
    ofs.x = 0.0f;
    ofs.y = 150.0f;
    ofs.z = 0.0f;
    rot.x = -20.0f;
    rot.y = rot.z = 0.0f;
    mbCameraMoveMasu(startMasuId, &rot, &ofs, 1500.0f, -1.0f, -1);
    mbCameraMoveWait();
    work->coinMan = mbev_CapCoinManCreate();
    work->starMan = mbev_CapStarManCreate();
    mbStatusDispForceSetAll(FALSE);
    work->arg[0] = guideObjId;
    work->arg[1] = startMasuId;
    ev_CapMiracleRun(work);
    mbWipeSpecialFadeInCreate(3, 1);
    for(i = 0; i < 2; i++) {
        mbObjDispSet(objId[i], FALSE);
    }
    mbMasuPosGet(masuId, &masuPos);
    mbPlayerPosSetV(playerNo, &masuPos);
    mbPlayerMotionSet(playerNo, 1, HU3D_MOTATTR_LOOP);
    mbev_CapPlayerPosSet(work->objWork, playerNo, masuId, NULL);
    mbPlayerColSnapPlayerSet(playerNo, TRUE);
    for(i = 0; i < 4; i++) {
        mbPlayerMotionSet(i, 1, HU3D_MOTATTR_LOOP);
        mbPlayerPosSetV(i, &playerPosTbl[i]);
        mbPlayerRotSet(i, 0.0f, 0.0f, 0.0f);
        mbPlayerColSnapPlayerSet(i, TRUE);
        mbPlayerDispSet(i, TRUE);
    }
    ofs.x = 0.0f;
    ofs.y = 100.0f;
    ofs.z = 0.0f;
    mbCameraMoveMasu(masuId, &cameraRot, &ofs, -1.0f, -1.0f, -1);
    mbCameraMoveWait();
    mbWipeSpecialFadeOutCreate(3, 60);
    HuPrcEnd();
}

void mbev_CapMiracleKill(void)
{
}

static void ev_CapMiracleMasu(CAPWORK *work)
{
    int glowId;
    int masuId;
    int i;
    int j;
    int k;
    int playerNo;
    float t;
    HuVecF objPos[2];
    HuVecF objRot[2];
    HuVecF playerPos;
    HuVecF masuPos;
    HuVecF vel;
    HuVecF pos;
    int seNo[4];
    int objId[2];
    int motId[2];
    GXColor color;

    playerNo = work->playerNo;
    masuId = GwPlayer[playerNo].masuId;
    mbPlayerPosGet(playerNo, &playerPos);
    mbMasuPosGet(masuId, &masuPos);
    objId[0] = work->arg[0];
    objId[1] = work->arg[1];
    motId[0] = mbev_CapPlayerMotionCreate(work->objWork, playerNo, 0x930017);
    motId[1] = mbev_CapPlayerMotionCreate(work->objWork, playerNo, 0x93004E);
    mbCameraMoveWait();
    omVibrate(playerNo, 300, 4, 4);
    pos.x = 0.0f;
    pos.y = 100.0f;
    pos.z = 0.0f;
    mbCameraMoveMasu(masuId, NULL, &pos, -1.0f, -1.0f, 60);
    mbPlayerMotionShiftSet(playerNo, motId[0], 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
    seNo[0] = mbAudFXPlay(0x443);
    for(i = 0; i < 60.0f; i++) {
        for(j = 0; j < 2; j++) {
            pos.x = masuPos.x+(2.0f*(100.0f*(-0.5f+CAP_RANDF())));
            pos.y = masuPos.y+(100.0f*CAP_RANDF());
            pos.z = masuPos.z+(2.0f*(100.0f*(-0.5f+CAP_RANDF())));
            vel.x = vel.y = vel.z = 0.0f;
            mbev_CapEffColorSet(&color, mbRandMod(0x8000));
            glowId = mbev_CapEffGlowAdd(work->glowEff, pos, vel, 60.0f*(1.0f+(0.3f*CAP_RANDF())), 100.0f*(0.15f+(0.05f*CAP_RANDF())), 0.05f+(0.02f*CAP_RANDF()), -0.08166666f, color);
            mbev_CapEffGlowKinokoTimeSet(work->glowEff, glowId, 1, 90);
        }
        HuPrcVSleep();
    }
    seNo[1] = mbAudFXPlay(0x444);
    for(i = 1; i <= 120.0f; i++) {
        t = i/120.0f;
        for(j = 0; j < 2; j++) {
            objRot[j].x = 0.0f;
            objRot[j].y = 540.0f*t;
            if(j & 1) {
                objRot[j].y += 270.0f;
            } else {
                objRot[j].y += 90.0f;
            }
            objRot[j].z = 0.0f;
            objPos[j].x = masuPos.x+(1.5*(100.0*HuSin(180.0f+objRot[j].y)));
            objPos[j].z = masuPos.z+(1.5*(100.0*HuCos(180.0f+objRot[j].y)));
            objPos[j].y = 100.0f+masuPos.y+(6.0*(100.0*HuCos(90.0f*t)));
            mbObjPosSetV(objId[j], &objPos[j]);
            mbObjRotSetV(objId[j], &objRot[j]);
            mbObjDispSet(objId[j], TRUE);
            for(k = 0; k < 3; k++) {
                pos.x = objPos[j].x+(2.0f*(100.0f*(-0.5f+CAP_RANDF())));
                pos.y = objPos[j].y+(100.0f*CAP_RANDF());
                pos.z = objPos[j].z+(2.0f*(100.0f*(-0.5f+CAP_RANDF())));
                vel.x = vel.y = vel.z = 0.0f;
                glowId = mbev_CapEffGlowAdd(work->glowEff, pos, vel, 60.0f*(1.0f+(0.3f*CAP_RANDF())), 100.0f*(0.15f+(0.05f*CAP_RANDF())), 0.05f+(0.02f*CAP_RANDF()), -0.08166666f, miracleMasuEffColorTbl[j][mbRandMod(3)]);
                mbev_CapEffGlowKinokoTimeSet(work->glowEff, glowId, 2, 60.0f*(0.5f+(0.5f*CAP_RANDF())));
            }
        }
        for(j = 0; j < 2; j++) {
            pos.x = masuPos.x+(2.0f*(100.0f*(-0.5f+CAP_RANDF())));
            pos.y = masuPos.y+(100.0f*CAP_RANDF());
            pos.z = masuPos.z+(2.0f*(100.0f*(-0.5f+CAP_RANDF())));
            vel.x = vel.y = vel.z = 0.0f;
            mbev_CapEffColorSet(&color, mbRandMod(0x8000));
            glowId = mbev_CapEffGlowAdd(work->glowEff, pos, vel, 60.0f*(1.0f+(0.3f*CAP_RANDF())), 100.0f*(0.15f+(0.05f*CAP_RANDF())), 0.05f+(0.02f*CAP_RANDF()), -0.08166666f, color);
            mbev_CapEffGlowKinokoTimeSet(work->glowEff, glowId, 1, 90);
        }
        HuPrcVSleep();
    }
    if(seNo[1] != -1) {
        mbAudFXStop(seNo[1]);
    }
    for(j = 0; j < 2; j++) {
        mbObjMotionShiftSet(objId[j], 9, 0.0f, 8.0f, 0);
    }
    mbPlayerColSnapPlayerSet(playerNo, FALSE);
    mbPlayerMotionShiftSet(playerNo, motId[1], 0.0f, 18.0f, HU3D_MOTATTR_LOOP);
    for(i = 1; i <= 45.0f; i++) {
        t = i/45.0f;
        playerPos.y = masuPos.y+(100.0*HuSin(90.0f*t));
        mbPlayerPosSetV(playerNo, &playerPos);
        for(j = 0; j < 2; j++) {
            pos.x = playerPos.x+(2.0f*(100.0f*(-0.5f+CAP_RANDF())));
            pos.y = playerPos.y+(2.0f*(100.0f*(-0.5f+CAP_RANDF())));
            pos.z = playerPos.z+(2.0f*(100.0f*(-0.5f+CAP_RANDF())));
            vel.x = vel.y = vel.z = 0.0f;
            mbev_CapEffColorSet(&color, mbRandMod(0x8000));
            glowId = mbev_CapEffGlowAdd(work->glowEff, pos, vel, 60.0f*(1.0f+(0.3f*CAP_RANDF())), 100.0f*(0.15f+(0.05f*CAP_RANDF())), 0.05f+(0.02f*CAP_RANDF()), -0.08166666f, color);
            mbev_CapEffGlowKinokoTimeSet(work->glowEff, glowId, 1, 90);
        }
        HuPrcVSleep();
    }
    seNo[2] = mbAudFXPlay(0x445);
    seNo[3] = mbAudFXPlay(0x3FE);
    for(i = 1; i <= 120.0f; i++) {
        t = i/120.0f;
        playerPos.y = 100.0f+masuPos.y+(6.0*(100.0*HuSin(90.0f*t)));
        mbPlayerPosSetV(playerNo, &playerPos);
        mbPlayerRotSet(playerNo, 0.0f, 720.0*HuSin(90.0f*t), 0.0f);
        for(j = 0; j < 2; j++) {
            objRot[j].x = 0.0f;
            objRot[j].y = 720.0*HuSin(90.0f*(t*t));
            if(j & 1) {
                objRot[j].y += 90.0f;
            } else {
                objRot[j].y += 270.0f;
            }
            objRot[j].z = 0.0f;
            objPos[j].x = masuPos.x+(1.5*(100.0*HuSin(180.0f+objRot[j].y)));
            objPos[j].z = masuPos.z+(1.5*(100.0*HuCos(180.0f+objRot[j].y)));
            objPos[j].y = 100.0f+masuPos.y+(6.0*(100.0*HuSin(90.0f*(t*t))));
            mbObjPosSetV(objId[j], &objPos[j]);
            mbObjRotSetV(objId[j], &objRot[j]);
            for(k = 0; k < 3; k++) {
                pos.x = objPos[j].x+(2.0f*(100.0f*(-0.5f+CAP_RANDF())));
                pos.y = objPos[j].y+(100.0f*CAP_RANDF());
                pos.z = objPos[j].z+(2.0f*(100.0f*(-0.5f+CAP_RANDF())));
                vel.x = vel.y = vel.z = 0.0f;
                glowId = mbev_CapEffGlowAdd(work->glowEff, pos, vel, 60.0f*(1.0f+(0.3f*CAP_RANDF())), 100.0f*(0.15f+(0.05f*CAP_RANDF())), 0.05f+(0.02f*CAP_RANDF()), -0.08166666f, miracleMasuEffColorTbl[j][mbRandMod(3)]);
                mbev_CapEffGlowKinokoTimeSet(work->glowEff, glowId, 2, 60.0f*(0.5f+(0.5f*CAP_RANDF())));
            }
        }
        for(j = 0; j < 2; j++) {
            pos.x = playerPos.x+(2.0f*(100.0f*(-0.5f+CAP_RANDF())));
            pos.y = playerPos.y+(2.0f*(100.0f*(-0.5f+CAP_RANDF())));
            pos.z = playerPos.z+(2.0f*(100.0f*(-0.5f+CAP_RANDF())));
            vel.x = vel.y = vel.z = 0.0f;
            mbev_CapEffColorSet(&color, mbRandMod(0x8000));
            glowId = mbev_CapEffGlowAdd(work->glowEff, pos, vel, 60.0f*(1.0f+(0.3f*CAP_RANDF())), 100.0f*(0.15f+(0.05f*CAP_RANDF())), 0.05f+(0.02f*CAP_RANDF()), -0.08166666f, color);
            mbev_CapEffGlowKinokoTimeSet(work->glowEff, glowId, 1, 90);
        }
        HuPrcVSleep();
    }
    if(seNo[0] != -1) {
        mbAudFXStop(seNo[0]);
    }
    if(seNo[2] != -1) {
        mbAudFXStop(seNo[2]);
    }
    if(seNo[3] != -1) {
        mbAudFXStop(seNo[3]);
    }
}

/* @@PART2@@ */
/* placeholder: part 2 owns this */
static void ev_CapMiracleRun(CAPWORK *work)
{
}

/* @@PART3@@ */
/* @@PART4@@ */
/* @@END@@ */
