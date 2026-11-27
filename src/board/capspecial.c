/* @@HEAD@@ */
#include "game/board/capspecial.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/capevent.h"
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

/* The miracle capsule's sprite effects (ev_CapMiracleSprCreate), one per slot. */
typedef struct CapMiracleSprWork_s {
    /* 0x00 */ int activeF;
    /* 0x04 */ int sprId;
    /* 0x08 */ int backSprId;
    /* 0x0C */ int charSprId[6];
    /* 0x24 */ int mode;
    /* 0x28 */ int time;
    /* 0x2C */ int hideF;
    /* 0x30 */ float scale;
    /* 0x34 */ float t;
    /* 0x38 */ HuVecF pos;
} CAPMIRACLESPRWORK; /* size 0x44 */

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

static int miracleTradeOrderTbl[3][32] = {
    { 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 1, -1 },
    { 3, 2, 0, 1, 0, 2, 0, 1, 3, 0, 4, 2, 3, 0, 1, 0, 1, 5, 2, 4, -1 },
    { 3, 0, 4, 2, 0, 5, 1, 2, 0, 3, 0, 1, 4, 1, 3, 1, 2, 3, 4, 5, -1 },
};

static int miracleLeftCharOrderTbl[32] = { 1, 2, 1, 3, 1, 0, 1, 3, 0, 2, -1 };

static int miracleRightCharOrderTbl[2][32] = {
    { 2, 0, 2, 1, 2, 1, 2, 0, 2, 0, -1 },
    { 1, 0, 2, 0, 2, 0, 1, 0, 2, 0, -1 },
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

static int kettouPlayerMotTbl[4] = { 0x8E0027, 0x93001F, 0x8E0078, 0x93004B };

static s32 kettouMotId[12];
static s16 mgResultData[5];

static int donkeyMotTbl[12] = {
    0xE000F, 0xE0011, 0xE0012, 0xE0015, 0xE0016, 0xE0013, 0xE0014, 0xE0018, 0xE0019, 0xE001A, 0xE001B, -1
};

static s32 donkeyMgFile[2] = { 0x5008C, 0x5008C };
static s8 donkeyDiceTbl[8] = { 0, 1, 2, 3, 4, -1 };

static int donkeyDiceResultTbl[5] = { 5, 10, 20, 30, -1 };
static int donkeyRouletteBankTbl[10] = { 0, 1, 0, 1, 0, 1, 0, 1, 0, 2 };

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
    work->effGlow = mbev_CapEffGlowCreate();
    work->effCoin = mbev_CapEffCoinCreate();
    mbev_CapEffCoinGlowSet(work->effCoin, work->effGlow);
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
    teresaObjId = mbev_CapObjCreate(&work->objWork, 0x130008, teresaMotTbl, 0, 5, 0);
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
    motId[0] = mbev_CapPlayerMotionCreate(&work->objWork, targetNo, 0x930017);
    motId[1] = mbev_CapPlayerMotionCreate(&work->objWork, targetNo, 0x930022);
    motId[2] = mbev_CapPlayerMotionCreate(&work->objWork, targetNo, 0x93006E);
    mbPlayerMotionShiftSet(targetNo, motId[0], 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
    hookObjId = mbev_CapObjCreate(&work->objWork, 0xC0049, NULL, 0, 0, 0);
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
            coinId = mbev_CapEffCoinAdd(work->effCoin, &coinPos, &coinVel, 0.75f, 4.9f, 30, 0xFFFF);
            if(coinId >= 0) {
                mbev_CapEffCoinMaxYSet(work->effCoin, coinId, 300.0f+pos.y);
            }
            mbPlayerCoinAdd(targetNo, -1);
            mbAudFXPlay(14);
            HuPrcVSleep();
        }
        mbAudFXPlay(15);
        while(mbev_CapEffCoinNumGet(work->effCoin) > 0) {
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
            mbev_CapCoinAdd(work->effCoin, playerNo, stealCoin, 1);
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
    work->effGlow = mbev_CapEffGlowCreate();
    playerNo = work->playerNo;
    masuId = GwPlayer[playerNo].masuId;
    mbPlayerPosGet(playerNo, &playerPos);
    mbMasuPosGet(masuId, &masuPos);
    for(i = 0; i < 4; i++) {
        mbPlayerPosGet(i, &playerPosTbl[i]);
    }
    mbCameraPlayerViewSet(playerNo, 0);
    mbev_CapPlayerRotate(playerNo, 0.0f);
    objId[0] = mbev_CapObjCreate(&work->objWork, 0x110000, MiracleGuideMotTbl[0], 0, 5, 0);
    mbObjMotionSet(objId[0], 3, 0);
    mbObjDispSet(objId[0], FALSE);
    objId[1] = mbev_CapObjCreate(&work->objWork, 0x11001B, MiracleGuideMotTbl[1], 0, 5, 0);
    mbObjHookSet(objId[1], "itemhook_R", mbev_CapObjCreate(&work->objWork, 0x110035, NULL, 0, 5, 0));
    HuPrcVSleep();
    mbObjMotionSet(objId[1], 3, 0);
    mbObjDispSet(objId[1], FALSE);
    work->evWork[0] = objId[0];
    work->evWork[1] = objId[1];
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
            mbev_CapObjClose(&work->objWork, objId[i]);
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
    mbev_CapPlayerPosSet(&work->objWork, playerNo, startMasuId, &ofs);
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
    work->effCoinMan = mbev_CapCoinManCreate();
    work->effStarMan = mbev_CapStarManCreate();
    mbStatusDispForceSetAll(FALSE);
    work->evWork[0] = guideObjId;
    work->evWork[1] = startMasuId;
    ev_CapMiracleRun(work);
    mbWipeSpecialFadeInCreate(3, 1);
    for(i = 0; i < 2; i++) {
        mbObjDispSet(objId[i], FALSE);
    }
    mbMasuPosGet(masuId, &masuPos);
    mbPlayerPosSetV(playerNo, &masuPos);
    mbPlayerMotionSet(playerNo, 1, HU3D_MOTATTR_LOOP);
    mbev_CapPlayerPosSet(&work->objWork, playerNo, masuId, NULL);
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
    objId[0] = work->evWork[0];
    objId[1] = work->evWork[1];
    motId[0] = mbev_CapPlayerMotionCreate(&work->objWork, playerNo, 0x930017);
    motId[1] = mbev_CapPlayerMotionCreate(&work->objWork, playerNo, 0x93004E);
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
            glowId = mbev_CapEffGlowAdd(work->effGlow, pos, vel, 60.0f*(1.0f+(0.3f*CAP_RANDF())), 100.0f*(0.15f+(0.05f*CAP_RANDF())), 0.05f+(0.02f*CAP_RANDF()), -0.08166666f, color);
            mbev_CapEffGlowKinokoTimeSet(work->effGlow, glowId, 1, 90);
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
                glowId = mbev_CapEffGlowAdd(work->effGlow, pos, vel, 60.0f*(1.0f+(0.3f*CAP_RANDF())), 100.0f*(0.15f+(0.05f*CAP_RANDF())), 0.05f+(0.02f*CAP_RANDF()), -0.08166666f, miracleMasuEffColorTbl[j][mbRandMod(3)]);
                mbev_CapEffGlowKinokoTimeSet(work->effGlow, glowId, 2, 60.0f*(0.5f+(0.5f*CAP_RANDF())));
            }
        }
        for(j = 0; j < 2; j++) {
            pos.x = masuPos.x+(2.0f*(100.0f*(-0.5f+CAP_RANDF())));
            pos.y = masuPos.y+(100.0f*CAP_RANDF());
            pos.z = masuPos.z+(2.0f*(100.0f*(-0.5f+CAP_RANDF())));
            vel.x = vel.y = vel.z = 0.0f;
            mbev_CapEffColorSet(&color, mbRandMod(0x8000));
            glowId = mbev_CapEffGlowAdd(work->effGlow, pos, vel, 60.0f*(1.0f+(0.3f*CAP_RANDF())), 100.0f*(0.15f+(0.05f*CAP_RANDF())), 0.05f+(0.02f*CAP_RANDF()), -0.08166666f, color);
            mbev_CapEffGlowKinokoTimeSet(work->effGlow, glowId, 1, 90);
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
            glowId = mbev_CapEffGlowAdd(work->effGlow, pos, vel, 60.0f*(1.0f+(0.3f*CAP_RANDF())), 100.0f*(0.15f+(0.05f*CAP_RANDF())), 0.05f+(0.02f*CAP_RANDF()), -0.08166666f, color);
            mbev_CapEffGlowKinokoTimeSet(work->effGlow, glowId, 1, 90);
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
                glowId = mbev_CapEffGlowAdd(work->effGlow, pos, vel, 60.0f*(1.0f+(0.3f*CAP_RANDF())), 100.0f*(0.15f+(0.05f*CAP_RANDF())), 0.05f+(0.02f*CAP_RANDF()), -0.08166666f, miracleMasuEffColorTbl[j][mbRandMod(3)]);
                mbev_CapEffGlowKinokoTimeSet(work->effGlow, glowId, 2, 60.0f*(0.5f+(0.5f*CAP_RANDF())));
            }
        }
        for(j = 0; j < 2; j++) {
            pos.x = playerPos.x+(2.0f*(100.0f*(-0.5f+CAP_RANDF())));
            pos.y = playerPos.y+(2.0f*(100.0f*(-0.5f+CAP_RANDF())));
            pos.z = playerPos.z+(2.0f*(100.0f*(-0.5f+CAP_RANDF())));
            vel.x = vel.y = vel.z = 0.0f;
            mbev_CapEffColorSet(&color, mbRandMod(0x8000));
            glowId = mbev_CapEffGlowAdd(work->effGlow, pos, vel, 60.0f*(1.0f+(0.3f*CAP_RANDF())), 100.0f*(0.15f+(0.05f*CAP_RANDF())), 0.05f+(0.02f*CAP_RANDF()), -0.08166666f, color);
            mbev_CapEffGlowKinokoTimeSet(work->effGlow, glowId, 1, 90);
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
#include "game/board/dice.h"
#include "game/board/guide.h"
#include "game/board/status.h"
#include "game/esprite.h"

/* capevent.c; local prototypes (part 2) */


static void ev_CapMiraclePlayerSet(CAPWORK *work, int playerA, int playerB, int masuId);
static void ev_CapMiracleCoinTrade(CAPWORK *work, int playerA, int playerB, int coinA, int coinB);
static void ev_CapMiracleStarTrade(CAPWORK *work, int playerA, int playerB, int starA, int starB);
static u32 ev_CapMiracleMesGet(u32 mesId);
static void ev_CapMiracleWindowFadeOut(int objId, int nextObjId, int maxTime, BOOL inF);
static void ev_CapMiracleWindowFadeIn(int objId, int nextObjId, int maxTime, BOOL inF, int motTime, int motNum, int *motTbl);
static int ev_CapMiracleDiceExec(int playerNo, int objId, int motTime, int motNum, int *motTbl);
static void ev_CapMiracleSprCreate(void);
static void ev_CapMiracleSprUpdate(OMOBJ *obj);
static void ev_CapMiracleSprDestroy(void);
static void ev_CapMiracleTradeCreate(HuVecF *pos, int tradeNo);
static void ev_CapMiracleTradeFocusSet(void);
static void ev_CapMiracleTradeHideSet(void);

static void ev_CapMiracleRun(CAPWORK *work)
{
    int playerTbl[32];
    int tradeTbl[32];
    int charTbl[32];
    Mtx mtx;
    HuVecF objPos;
    HuVecF playerPos;
    HuVecF rot;
    HuVecF pos;
    HuVecF masuPos;
    int order[4];
    HuVec2f posOff;
    HuVec2f posOn;
    int coin2[2];
    int star2[2];
    int coin[2];
    int star[2];
    int playerNo[2];
    BOOL motF[2];
    int playerA;
    int playerB;
    int i;
    int num;
    int guideObj;
    int curPlayer;
    int backObj;
    int charObj;
    int tradeObj;
    int bgObj;
    int sel;
    int level;
    int tradeNo;
    int side;
    int masuId;

    guideObj = work->objId;
    masuId = work->miracleMasuId;
    mbMasuPosGet(masuId, &masuPos);
    curPlayer = work->playerNo;
    playerPos.x = 200.0f + masuPos.x;
    playerPos.y = masuPos.y;
    playerPos.z = masuPos.z;
    mbPlayerPosSetV(curPlayer, &playerPos);
    objPos.x = masuPos.x - 200.0f;
    objPos.y = masuPos.y;
    objPos.z = masuPos.z;
    mbObjPosSetV(guideObj, &objPos);
    Hu3DMotionForceSet(mbObjModelIDGet(guideObj), "head", 0x80, -5.0f);
    Hu3DMotionForceSet(mbObjModelIDGet(guideObj), "head", 0x100, 5.0f);
    pos.x = playerPos.x - 200.0f;
    pos.y = playerPos.y;
    pos.z = playerPos.z - 75.0f;
    mbCameraRotGet(&rot);
    bgObj = mbev_CapObjCreate(&work->objWork, 0x110036, NULL, 0, 0, 0);
    mbObjPosSetV(bgObj, &pos);
    mbObjRotSetV(bgObj, &rot);
    mbObjScaleSet(bgObj, 2.0f, 2.0f, 2.0f);
    Hu3DMotionCalc(mbObjModelIDGet(bgObj));
    Hu3DModelObjMtxGet(mbObjModelIDGet(bgObj), "target", mtx);
    pos.x = mtx[0][3];
    pos.y = mtx[1][3];
    pos.z = mtx[2][3];
    charObj = mbev_CapObjCreate(&work->objWork, 0x110038, NULL, 0, 0, 0);
    mbObjMotionTimeSet(charObj, 0.5f + GwPlayer[curPlayer].charNo);
    mbObjMotionSpeedSet(charObj, 0.0f);
    mbObjPosSetV(charObj, &pos);
    mbObjRotSetV(charObj, &rot);
    mbObjScaleSet(charObj, 2.0f, 2.0f, 2.0f);
    mbObjDispSet(charObj, FALSE);
    mbObjLayerSet(charObj, 3);
    tradeObj = mbev_CapObjCreate(&work->objWork, 0x110039, NULL, 0, 0, 0);
    mbObjMotionTimeSet(tradeObj, 0.5f);
    mbObjMotionSpeedSet(tradeObj, 0.0f);
    mbObjPosSetV(tradeObj, &pos);
    mbObjRotSetV(tradeObj, &rot);
    mbObjScaleSet(tradeObj, 2.0f, 2.0f, 2.0f);
    mbObjDispSet(tradeObj, FALSE);
    mbObjLayerSet(tradeObj, 3);
    backObj = mbev_CapObjCreate(&work->objWork, 0x110037, NULL, 0, 0, 0);
    mbObjAttrSet(backObj, 0x40000001);
    mbObjPosSetV(backObj, &pos);
    mbObjRotSetV(backObj, &rot);
    mbObjDispSet(backObj, TRUE);
    mbObjLayerSet(backObj, 3);
    mbObjScaleSet(backObj, 2.0f, 2.0f, 2.0f);
    ev_CapMiracleSprCreate();
    mbMusBoardFadeOut(0, 0, 1000, 1000, 31, FALSE);
    mbWipeDissolveFadeIn();
    mbObjMotionShiftSet(guideObj, 7, 0.0f, 8.0f, 0x40000001);
    mbAudGuidePlay(950);
    mbWinCreate(2, ev_CapMiracleMesGet(0x3C0000), mbGuideSpeakerNoGet());
    mbWinTopWait();
    mbAudGuidePlay(952);
    mbWinCreate(2, ev_CapMiracleMesGet(0x3C0001), mbGuideSpeakerNoGet());
    mbWinTopWait();
    for(i = 1; i < 5; i++) {
        Hu3DMotionForceSet(mbObjModelIDGet(guideObj), "head", 0x80, -(5 - i));
        Hu3DMotionForceSet(mbObjModelIDGet(guideObj), "head", 0x100, 5 - i);
        HuPrcVSleep();
    }
    Hu3DMotionNoMotReset(mbObjModelIDGet(guideObj), "head", 0x80);
    Hu3DMotionNoMotReset(mbObjModelIDGet(guideObj), "head", 0x100);
    mbAudFXDelaySet(30);
    mbAudFXPlay(1017);
    mbev_CapPlayerMotShiftSet(guideObj, 4, 0, TRUE);
    mbObjMotionShiftSet(guideObj, 1, 0.0f, 8.0f, 0x40000001);
    mbev_CapPlayerOrderGet(order, -1, curPlayer, TRUE);
    for(i = 0, num = 0; i < 32; i++) {
        if(miracleLeftCharOrderTbl[i] == -1) {
            break;
        }
        playerTbl[num] = order[miracleLeftCharOrderTbl[i]];
        charTbl[num] = GwPlayer[playerTbl[num]].charNo;
        num++;
    }
    ev_CapMiracleWindowFadeIn(backObj, charObj, 60, TRUE, 3, num, charTbl);
    sel = ev_CapMiracleDiceExec(curPlayer, charObj, 3, num, charTbl);
    playerA = playerTbl[sel];
    mbAudFXPlay(1136);
    mbStatusPosOffGet(0, (HuVecF *)&posOff);
    mbStatusPosOnGet(0, (HuVecF *)&posOn);
    mbStatusDispForceSet(playerA, TRUE);
    mbStatusMoveTo(playerA, (HuVecF *)&posOff, (HuVecF *)&posOn);
    ev_CapMiracleWindowFadeOut(charObj, backObj, 60, FALSE);
    HuPrcSleep(60);
    mbObjMotionShiftSet(guideObj, 7, 0.0f, 8.0f, 0x40000001);
    mbAudGuidePlay(952);
    mbWinCreate(2, ev_CapMiracleMesGet(0x3C0002), mbGuideSpeakerNoGet());
    mbWinTopWait();
    mbAudFXDelaySet(30);
    mbAudFXPlay(1017);
    mbev_CapPlayerMotShiftSet(guideObj, 4, 0, TRUE);
    mbObjMotionShiftSet(guideObj, 1, 0.0f, 8.0f, 0x40000001);
    mbev_CapPlayerOrderGet(order, playerA, curPlayer, TRUE);
    if(playerA == curPlayer) {
        side = 0;
    } else {
        side = 1;
    }
    for(i = 0, num = 0; i < 32; i++) {
        if(miracleRightCharOrderTbl[side][i] == -1) {
            break;
        }
        playerTbl[num] = order[miracleRightCharOrderTbl[side][i]];
        charTbl[num] = GwPlayer[playerTbl[num]].charNo;
        num++;
    }
    mbObjMotionTimeSet(charObj, 0.5f + charTbl[0]);
    mbObjMotionSpeedSet(charObj, 0.0f);
    ev_CapMiracleWindowFadeIn(backObj, charObj, 60, TRUE, 3, num, charTbl);
    sel = ev_CapMiracleDiceExec(curPlayer, charObj, 3, num, charTbl);
    playerB = playerTbl[sel];
    mbAudFXPlay(1137);
    if(GWTeamGet() && mbev_CapPlayerCheck(playerA, playerB)) {
        HuPrcSleep(60);
        mbObjMotionShiftSet(guideObj, 6, 0.0f, 8.0f, 0x40000001);
        mbAudGuidePlay(951);
        mbWinCreate(2, ev_CapMiracleMesGet(0x3C0005), mbGuideSpeakerNoGet());
        mbWinTopWait();
        mbPlayerWinLoseVoicePlay(curPlayer, 13, 585);
        mbev_CapPlayerMotShiftWait(curPlayer, 13, 0, TRUE);
        mbev_CapPlayerMotShiftWait(curPlayer, 1, 0x40000001, TRUE);
        mbObjMotionShiftSet(guideObj, 1, 0.0f, 8.0f, 0x40000001);
        if(mbStatusDispGet(playerA)) {
            mbStatusDispSet(playerA, FALSE);
        }
        ev_CapMiracleTradeHideSet();
    } else {
        mbStatusPosOffGet(1, (HuVecF *)&posOff);
        mbStatusPosOnGet(1, (HuVecF *)&posOn);
        mbStatusDispForceSet(playerB, TRUE);
        mbStatusMoveTo(playerB, (HuVecF *)&posOff, (HuVecF *)&posOn);
        ev_CapMiracleWindowFadeOut(charObj, backObj, 60, FALSE);
        HuPrcSleep(60);
        mbAudGuidePlay(952);
        mbObjMotionShiftSet(guideObj, 7, 0.0f, 8.0f, 0x40000001);
        mbWinCreate(2, ev_CapMiracleMesGet(0x3C0003), mbGuideSpeakerNoGet());
        mbWinTopWait();
        mbAudFXDelaySet(30);
        mbAudFXPlay(1017);
        mbev_CapPlayerMotShiftSet(guideObj, 4, 0, TRUE);
        mbObjMotionShiftSet(guideObj, 1, 0.0f, 8.0f, 0x40000001);
        level = (GwSystem.turnNo * 3) / GwSystem.turnMax;
        if(level < 0) {
            level = 0;
        }
        if(level > 2) {
            level = 2;
        }
        for(i = 0, num = 0; i < 32; i++) {
            if(miracleTradeOrderTbl[level][i] == -1) {
                break;
            }
            tradeTbl[num] = miracleTradeOrderTbl[level][i];
            num++;
        }
        ev_CapMiracleWindowFadeIn(backObj, tradeObj, 60, TRUE, 3, num, tradeTbl);
        sel = ev_CapMiracleDiceExec(curPlayer, tradeObj, 3, num, tradeTbl);
        tradeNo = tradeTbl[sel];
        if(tradeNo < 0 || tradeNo > 5) {
            tradeNo = 0;
            mbObjMotionTimeSet(tradeObj, 0.5f + tradeNo);
        }
        ev_CapMiracleTradeCreate(&miracleTradePosTbl[1], tradeNo);
        ev_CapMiracleWindowFadeOut(tradeObj, backObj, 60, FALSE);
        HuPrcSleep(60);
        ev_CapMiracleTradeFocusSet();
        mbObjMotionShiftSet(guideObj, 7, 0.0f, 8.0f, 0x40000001);
        mbWinCreate(2, ev_CapMiracleMesGet(0x3C0004), mbGuideSpeakerNoGet());
        mbWinTopWait();
        ev_CapMiracleTradeHideSet();
        HuPrcSleep(60);
        coin[0] = coin2[0] = mbPlayerCoinGet(playerA);
        star[0] = star2[0] = mbPlayerStarGet(playerA);
        coin[1] = coin2[1] = mbPlayerCoinGet(playerB);
        star[1] = star2[1] = mbPlayerStarGet(playerB);
        mbWipeSpecialFadeInCreate(3, 1);
        mbObjDispSet(backObj, FALSE);
        mbObjDispSet(bgObj, FALSE);
        objPos.x = masuPos.x;
        objPos.y = masuPos.y;
        objPos.z = masuPos.z - 150.0f;
        mbObjPosSetV(guideObj, &objPos);
        mbObjRotSet(guideObj, 0.0f, 0.0f, 0.0f);
        mbev_CapPlayerPosSet(&work->objWork, curPlayer, -1, NULL);
        ev_CapMiraclePlayerSet(work, playerA, playerB, masuId);
        mbWipeSpecialFadeOutCreate(3, 60);
        switch(tradeNo) {
            case 0:
                mbAudGuidePlay(949);
                mbWinCreate(2, ev_CapMiracleMesGet(0x3C0006), mbGuideSpeakerNoGet());
                mbWinTopInsertMesSet(mbPlayerNameMesGet(playerA), 0);
                mbWinTopInsertMesSet(0x3C000B, 1);
                mbWinTopInsertMesSet(mbPlayerNameMesGet(playerB), 2);
                mbWinTopWait();
                if(mbPlayerCoinGet(playerA) <= 0) {
                    mbAudGuidePlay(951);
                    mbWinCreate(2, ev_CapMiracleMesGet(0x3C0008), mbGuideSpeakerNoGet());
                    mbWinTopInsertMesSet(mbPlayerNameMesGet(playerA), 0);
                    mbWinTopInsertMesSet(0x3C000B, 1);
                    mbWinTopWait();
                } else {
                    mbObjMotionShiftSet(guideObj, 1, 0.0f, 8.0f, 0x40000001);
                    ev_CapMiracleCoinTrade(work, playerA, playerB, 20, 0);
                }
                break;

            case 1:
                mbAudGuidePlay(949);
                mbWinCreate(2, ev_CapMiracleMesGet(0x3C0007), mbGuideSpeakerNoGet());
                mbWinTopInsertMesSet(mbPlayerNameMesGet(playerA), 0);
                mbWinTopInsertMesSet(mbPlayerNameMesGet(playerB), 1);
                mbWinTopInsertMesSet(0x3C000B, 2);
                mbWinTopWait();
                if(mbPlayerCoinGet(playerA) <= 0 && mbPlayerCoinGet(playerB) <= 0) {
                    mbAudGuidePlay(951);
                    mbWinCreate(2, ev_CapMiracleMesGet(0x3C0009), mbGuideSpeakerNoGet());
                    mbWinTopInsertMesSet(0x3C000B, 0);
                    mbWinTopWait();
                } else {
                    mbObjMotionShiftSet(guideObj, 1, 0.0f, 8.0f, 0x40000001);
                    ev_CapMiracleCoinTrade(work, playerA, playerB, mbPlayerCoinGet(playerA), mbPlayerCoinGet(playerB));
                }
                break;

            case 2:
                mbAudGuidePlay(949);
                mbWinCreate(2, ev_CapMiracleMesGet(0x3C0006), mbGuideSpeakerNoGet());
                mbWinTopInsertMesSet(mbPlayerNameMesGet(playerA), 0);
                mbWinTopInsertMesSet(0x3C000D, 1);
                mbWinTopInsertMesSet(mbPlayerNameMesGet(playerB), 2);
                mbWinTopWait();
                if(mbPlayerStarGet(playerA) <= 0) {
                    mbAudGuidePlay(951);
                    mbWinCreate(2, ev_CapMiracleMesGet(0x3C0008), mbGuideSpeakerNoGet());
                    mbWinTopInsertMesSet(mbPlayerNameMesGet(playerA), 0);
                    mbWinTopInsertMesSet(0x3C000C, 1);
                    mbWinTopWait();
                } else {
                    mbObjMotionShiftSet(guideObj, 1, 0.0f, 8.0f, 0x40000001);
                    ev_CapMiracleStarTrade(work, playerA, playerB, 1, 0);
                }
                break;

            case 3:
                mbAudGuidePlay(949);
                mbWinCreate(2, ev_CapMiracleMesGet(0x3C0006), mbGuideSpeakerNoGet());
                mbWinTopInsertMesSet(mbPlayerNameMesGet(playerA), 0);
                mbWinTopInsertMesSet(0x3C000E, 1);
                mbWinTopInsertMesSet(mbPlayerNameMesGet(playerB), 2);
                mbWinTopWait();
                if(mbPlayerStarGet(playerA) <= 0) {
                    mbAudGuidePlay(951);
                    mbWinCreate(2, ev_CapMiracleMesGet(0x3C0008), mbGuideSpeakerNoGet());
                    mbWinTopInsertMesSet(mbPlayerNameMesGet(playerA), 0);
                    mbWinTopInsertMesSet(0x3C000C, 1);
                    mbWinTopWait();
                } else {
                    mbObjMotionShiftSet(guideObj, 1, 0.0f, 8.0f, 0x40000001);
                    ev_CapMiracleStarTrade(work, playerA, playerB, 2, 0);
                }
                break;

            case 4:
                mbAudGuidePlay(949);
                mbWinCreate(2, ev_CapMiracleMesGet(0x3C0007), mbGuideSpeakerNoGet());
                mbWinTopInsertMesSet(mbPlayerNameMesGet(playerA), 0);
                mbWinTopInsertMesSet(mbPlayerNameMesGet(playerB), 1);
                mbWinTopInsertMesSet(0x3C000C, 2);
                mbWinTopWait();
                if(mbPlayerStarGet(playerA) <= 0 && mbPlayerStarGet(playerB) <= 0) {
                    mbAudGuidePlay(951);
                    mbWinCreate(2, ev_CapMiracleMesGet(0x3C0009), mbGuideSpeakerNoGet());
                    mbWinTopInsertMesSet(0x3C000C, 0);
                    mbWinTopWait();
                } else {
                    mbObjMotionShiftSet(guideObj, 1, 0.0f, 8.0f, 0x40000001);
                    ev_CapMiracleStarTrade(work, playerA, playerB, mbPlayerStarGet(playerA), mbPlayerStarGet(playerB));
                }
                break;

            case 5:
                mbAudGuidePlay(949);
                mbWinCreate(2, ev_CapMiracleMesGet(0x3C0007), mbGuideSpeakerNoGet());
                mbWinTopInsertMesSet(mbPlayerNameMesGet(playerA), 0);
                mbWinTopInsertMesSet(mbPlayerNameMesGet(playerB), 1);
                mbWinTopInsertMesSet(0x3C000F, 2);
                mbWinTopWait();
                if(mbPlayerCoinGet(playerA) <= 0 && mbPlayerCoinGet(playerB) <= 0 && mbPlayerStarGet(playerA) <= 0 && mbPlayerStarGet(playerB) <= 0) {
                    mbAudGuidePlay(951);
                    mbWinCreate(2, ev_CapMiracleMesGet(0x3C0009), mbGuideSpeakerNoGet());
                    mbWinTopInsertMesSet(0x3C000F, 0);
                    mbWinTopWait();
                } else {
                    mbObjMotionShiftSet(guideObj, 1, 0.0f, 8.0f, 0x40000001);
                    ev_CapMiracleCoinTrade(work, playerA, playerB, mbPlayerCoinGet(playerA), mbPlayerCoinGet(playerB));
                    ev_CapMiracleStarTrade(work, playerA, playerB, mbPlayerStarGet(playerA), mbPlayerStarGet(playerB));
                }
                break;
        }
        if(mbStatusDispGet(playerA) != FALSE) {
            mbStatusDispSet(playerA, FALSE);
        }
        if(mbStatusDispGet(playerB) != FALSE) {
            mbStatusDispSet(playerB, FALSE);
        }
        playerNo[0] = playerA;
        playerNo[1] = playerB;
        motF[0] = FALSE;
        motF[1] = FALSE;
        for(i = 0; i < 2; i++) {
            if(star[i] < mbPlayerStarGet(playerNo[i])) {
                mbPlayerWinLoseVoicePlay(playerNo[i], 7, 573);
                mbPlayerMotionShiftSet(playerNo[i], 7, 0.0f, 8.0f, 0);
                motF[i] = TRUE;
            } else if(star[i] > mbPlayerStarGet(playerNo[i])) {
                mbPlayerWinLoseVoicePlay(playerNo[i], 8, 585);
                mbPlayerMotionShiftSet(playerNo[i], 8, 0.0f, 8.0f, 0);
                motF[i] = TRUE;
            } else if(coin[i] < mbPlayerCoinGet(playerNo[i])) {
                mbPlayerWinLoseVoicePlay(playerNo[i], 12, 579);
                mbPlayerMotionShiftSet(playerNo[i], 12, 0.0f, 8.0f, 0);
                motF[i] = TRUE;
            } else if(coin[i] > mbPlayerCoinGet(playerNo[i])) {
                mbPlayerWinLoseVoicePlay(playerNo[i], 13, 585);
                mbPlayerMotionShiftSet(playerNo[i], 13, 0.0f, 8.0f, 0);
                motF[i] = TRUE;
            }
        }
        HuPrcSleep(30);
        do {
            for(i = 0; i < 2; i++) {
                if(motF[i] && !mbPlayerMotionEndCheck(playerNo[i])) {
                    break;
                }
            }
            HuPrcVSleep();
        } while(i < 2);
        if(motF[0]) {
            mbPlayerMotionShiftSet(playerNo[0], 1, 0.0f, 8.0f, 0x40000001);
        }
        if(motF[1]) {
            mbPlayerMotionShiftSet(playerNo[1], 1, 0.0f, 8.0f, 0x40000001);
        }
        mbObjMotionShiftSet(guideObj, 5, 0.0f, 8.0f, 0x40000001);
        mbAudGuidePlay(952);
        mbWinCreate(2, ev_CapMiracleMesGet(0x3C000A), mbGuideSpeakerNoGet());
        mbWinTopWait();
    }
    mbMusBoardFadeOut(0, 0, 1000, 1000, -1, FALSE);
    ev_CapMiracleSprDestroy();
}

static void ev_CapMiraclePlayerSet(CAPWORK *work, int playerA, int playerB, int masuId)
{
    int i;
    HuVecF masuPos;
    HuVecF pos;

    for(i = 0; i < 4; i++) {
        mbPlayerDispSet(i, FALSE);
    }
    mbMasuPosGet(masuId, &masuPos);
    pos.x = masuPos.x - 200.0f;
    pos.y = masuPos.y;
    pos.z = masuPos.z;
    mbPlayerPosSetV(playerA, &pos);
    mbPlayerRotSet(playerA, 0.0f, 30.0f, 0.0f);
    mbPlayerDispSet(playerA, TRUE);
    mbPlayerColSnapPlayerSet(playerA, FALSE);
    pos.x = 200.0f + masuPos.x;
    pos.y = masuPos.y;
    pos.z = masuPos.z;
    mbPlayerPosSetV(playerB, &pos);
    mbPlayerRotSet(playerB, 0.0f, -30.0f, 0.0f);
    mbPlayerDispSet(playerB, TRUE);
    mbPlayerColSnapPlayerSet(playerB, FALSE);
}

static void ev_CapMiracleCoinTrade(CAPWORK *work, int playerA, int playerB, int coinA, int coinB)
{
    int speed;
    int num;
    HuVecF posA;
    HuVecF posB;

    speed = 1;
    mbPlayerPosGet(playerA, &posA);
    posA.y += 150.0f;
    mbPlayerPosGet(playerB, &posB);
    posB.y += 150.0f;
    if(coinA + coinB < 20) {
        speed = 5;
    } else if(coinA + coinB < 40) {
        speed = 4;
    } else if(coinA + coinB < 60) {
        speed = 3;
    } else if(coinA + coinB < 80) {
        speed = 2;
    } else {
        speed = 1;
    }
    do {
        if(coinA > 0 && mbPlayerCoinGet(playerA) > 0) {
            num = mbev_CapCoinManAdd(work->effCoinMan, &posA, &posB, playerB, 1);
            if(num != 0) {
                mbPlayerCoinAdd(playerA, -num);
                coinA -= num;
            }
        }
        if(mbPlayerCoinGet(playerA) <= 0) {
            coinA = 0;
        }
        if(coinB > 0 && mbPlayerCoinGet(playerB) > 0) {
            num = mbev_CapCoinManAdd(work->effCoinMan, &posB, &posA, playerA, 1);
            if(num != 0) {
                mbPlayerCoinAdd(playerB, -num);
                coinB -= num;
            }
        }
        if(mbPlayerCoinGet(playerB) <= 0) {
            coinB = 0;
        }
        HuPrcSleep(speed);
    } while(coinA > 0 || coinB > 0 || mbev_CapCoinManNumGet(work->effCoinMan) > 0);
    mbAudFXPlay(15);
}

static void ev_CapMiracleStarTrade(CAPWORK *work, int playerA, int playerB, int starA, int starB)
{
    int speed;
    int num;
    HuVecF posA;
    HuVecF posB;

    if(starA + starB < 5) {
        speed = 15;
    } else if(starA + starB < 10) {
        speed = 12;
    } else if(starA + starB < 15) {
        speed = 10;
    } else if(starA + starB < 20) {
        speed = 8;
    } else {
        speed = 4;
    }
    mbPlayerPosGet(playerA, &posA);
    posA.y += 150.0f;
    mbPlayerPosGet(playerB, &posB);
    posB.y += 150.0f;
    do {
        if(starA > 0 && mbPlayerStarGet(playerA) > 0) {
            num = mbev_CapStarManAdd(work->effStarMan, &posA, &posB, playerB, 1);
            if(num != 0) {
                mbPlayerStarAdd(playerA, -num);
                starA -= num;
            }
        }
        if(mbPlayerStarGet(playerA) <= 0) {
            starA = 0;
        }
        if(starB > 0 && mbPlayerStarGet(playerB) > 0) {
            num = mbev_CapStarManAdd(work->effStarMan, &posB, &posA, playerA, 1);
            if(num != 0) {
                mbPlayerStarAdd(playerB, -num);
                starB -= num;
            }
        }
        if(mbPlayerStarGet(playerB) <= 0) {
            starB = 0;
        }
        HuPrcSleep(speed);
    } while(starA > 0 || starB > 0 || mbev_CapStarManNumGet(work->effStarMan) > 0);
}

static u32 ev_CapMiracleMesGet(u32 mesId)
{
    if(!GwSystem.curTime) {
        return mesId;
    } else {
        return mesId + 0x10;
    }
}

static void ev_CapMiracleDiceHitHook(int result)
{
    diceHitTimer = 0;
}

static void ev_CapMiracleWindowFadeOut(int objId, int nextObjId, int maxTime, BOOL inF)
{
    int i;
    float t;

    if(inF) {
        mbObjDispSet(objId, TRUE);
        mbObjLayerSet(objId, 3);
        mbObjDispSet(nextObjId, TRUE);
        mbObjAlphaSet(nextObjId, 255);
        mbObjLayerSet(nextObjId, 3);
        for(i = 0; i < maxTime; i++) {
            t = (float)i / maxTime;
            mbObjAlphaSet(nextObjId, 255.0f * t);
            HuPrcVSleep();
        }
        mbObjDispSet(objId, FALSE);
        mbObjDispSet(nextObjId, TRUE);
        mbObjAlphaSet(nextObjId, 255);
        mbObjLayerSet(nextObjId, 3);
    } else {
        mbObjDispSet(objId, TRUE);
        mbObjAlphaSet(objId, 255);
        mbObjLayerSet(objId, 3);
        mbObjDispSet(nextObjId, TRUE);
        mbObjLayerSet(nextObjId, 3);
        for(i = 0; i < maxTime; i++) {
            t = (float)i / maxTime;
            mbObjAlphaSet(objId, 255.0f * (1.0f - t));
            HuPrcVSleep();
        }
        mbObjDispSet(objId, FALSE);
        mbObjDispSet(nextObjId, TRUE);
        mbObjLayerSet(nextObjId, 3);
    }
}

static void ev_CapMiracleWindowFadeIn(int objId, int nextObjId, int maxTime, BOOL inF, int motTime, int motNum, int *motTbl)
{
    int i;
    int time;
    int motNo;
    float t;

    time = 0;
    motNo = 0;
    if(inF) {
        mbObjDispSet(objId, TRUE);
        mbObjLayerSet(objId, 3);
        mbObjDispSet(nextObjId, TRUE);
        mbObjAlphaSet(nextObjId, 255);
        mbObjLayerSet(nextObjId, 3);
        for(i = 0; i < maxTime; i++) {
            t = (float)i / maxTime;
            mbObjAlphaSet(nextObjId, 255.0f * t);
            if(++time >= motTime) {
                time = 0;
                if(++motNo >= motNum) {
                    motNo = 0;
                }
                mbObjMotionTimeSet(nextObjId, 0.5f + motTbl[motNo]);
            }
            HuPrcVSleep();
        }
        mbObjDispSet(objId, FALSE);
        mbObjDispSet(nextObjId, TRUE);
        mbObjAlphaSet(nextObjId, 255);
        mbObjLayerSet(nextObjId, 3);
    } else {
        mbObjDispSet(objId, TRUE);
        mbObjAlphaSet(objId, 255);
        mbObjLayerSet(objId, 3);
        mbObjDispSet(nextObjId, TRUE);
        mbObjLayerSet(nextObjId, 3);
        for(i = 0; i < maxTime; i++) {
            t = (float)i / maxTime;
            mbObjAlphaSet(objId, 255.0f * (1.0f - t));
            if(++time >= motTime) {
                time = 0;
                if(++motNo >= motNum) {
                    motNo = 0;
                }
                mbObjMotionTimeSet(nextObjId, 0.5f + motTbl[motNo]);
            }
            HuPrcVSleep();
        }
        mbObjDispSet(objId, FALSE);
        mbObjDispSet(nextObjId, TRUE);
        mbObjLayerSet(nextObjId, 3);
    }
}

static int ev_CapMiracleDiceExec(int playerNo, int objId, int motTime, int motNum, int *motTbl)
{
    int winId;
    int motNo;
    int time;

    winId = mbWinCreateHelp(0x260002);
    mbDiceExec(playerNo, 6, NULL, -1, FALSE, FALSE, NULL, 0);
    diceHitTimer = 1;
    mbDiceHitHookSet(playerNo, ev_CapMiracleDiceHitHook);
    motNo = mbRandMod(motNum);
    time = 0;
    do {
        if(diceHitTimer) {
            time++;
            if(time >= motTime) {
                time = 0;
                motNo++;
                if(motNo >= motNum) {
                    motNo = 0;
                }
            }
            mbObjMotionTimeSet(objId, 0.5f + motTbl[motNo]);
        }
        HuPrcVSleep();
    } while(!mbDiceKillCheck(playerNo));
    if(winId != -1) {
        mbWinKill(winId);
    }
    return motNo;
}

static void ev_CapMiracleSprCreate(void)
{
    CAPMIRACLESPRWORK *work;
    int j;
    int i;
    OMOBJ *obj;

    obj = omAddObjEx(mbObjMan, 32768, 0, 0, -1, ev_CapMiracleSprUpdate);
    miracleSprObj = obj;
    work = obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(CAPMIRACLESPRWORK) * 6, HU_MEMNUM_OVL);
    memset(work, 0, sizeof(CAPMIRACLESPRWORK) * 6);
    for(i = 0; i < 6; i++, work++) {
        work->activeF = FALSE;
        work->sprId = -1;
        work->backSprId = -1;
        for(j = 0; j < 6; j++) {
            work->charSprId[j] = -1;
        }
        work->mode = 0;
        work->time = 0;
        work->hideF = FALSE;
        work->scale = 0.0f;
        work->t = 0.0f;
    }
}

static void ev_CapMiracleSprUpdate(OMOBJ *obj)
{
    CAPMIRACLESPRWORK *work;
    int i;
    int j;
    float t;
    float scale;
    float rotBase;
    float rot;
    float angle;
    float radius;
    HuVecF pos;

    work = obj->data;
    if(mbExitCheck() || miracleSprObj == NULL) {
        for(i = 0; i < 6; i++, work++) {
            if(work->activeF) {
                if(work->sprId != -1) {
                    espKill(work->sprId);
                }
                if(work->backSprId != -1) {
                    espKill(work->backSprId);
                }
                for(j = 0; j < 6; j++) {
                    if(work->charSprId[j] != -1) {
                        espKill(work->charSprId[j]);
                    }
                }
                work->sprId = -1;
                work->backSprId = -1;
            }
        }
        omDelObjEx(mbObjMan, obj);
        miracleSprObj = NULL;
        return;
    }
    for(i = 0; i < 6; i++, work++) {
        if(!work->activeF) {
            continue;
        }
        switch(work->mode) {
            case 0:
                t = ++work->time / 30.0f;
                if(t < 1.0f) {
                    scale = 1.0 + 5.0 * HuCos(90.0f * t);
                    espScaleSet(work->sprId, scale, scale);
                    espTPLvlSet(work->sprId, t);
                    espZRotSet(work->sprId, 0.0f);
                    scale = t + 0.5 * HuSin(180.0f * t);
                    espScaleSet(work->backSprId, scale, scale);
                    rotBase = 90.0f * t;
                    radius = 300.0f * (1.0f - t);
                    for(j = 0; j < 6; j++) {
                        rot = rotBase + 60.0f * j;
                        angle = rot + rotBase;
                        pos.x = work->pos.x + radius * HuSin(angle);
                        pos.y = work->pos.y + radius * HuCos(angle);
                        espPosSet(work->charSprId[j], pos.x, pos.y);
                        espScaleSet(work->sprId, scale, scale);
                        espTPLvlSet(work->charSprId[j], 0.25f * t);
                        espZRotSet(work->charSprId[j], (1.0f - t) * rot);
                    }
                } else {
                    mbAudFXPlay(1138);
                    scale = 1.0f;
                    espTPLvlSet(work->sprId, 1.0f);
                    espZRotSet(work->sprId, 0.0f);
                    espScaleSet(work->sprId, scale, scale);
                    espScaleSet(work->backSprId, scale, scale);
                    for(j = 0; j < 6; j++) {
                        espScaleSet(work->charSprId[j], 0.0f, 0.0f);
                    }
                    espKill(work->charSprId[1]);
                    work->charSprId[1] = espEntry(0x11003A, 500, 0);
                    espDrawNoSet(work->charSprId[1], 32);
                    espPosSet(work->charSprId[1], work->pos.x, work->pos.y);
                    espScaleSet(work->charSprId[1], 0.0f, 0.0f);
                    espTPLvlSet(work->charSprId[1], 0.25f);
                    espAttrSet(work->charSprId[1], 8);
                    work->mode++;
                    work->time = 0;
                }
                break;

            case 1:
                t = ++work->time / 18.0f;
                scale = 1.0 + 10.0 * HuSin(90.0f * t);
                if(t < 1.0f) {
                    espPosSet(work->charSprId[0], work->pos.x, work->pos.y);
                    espScaleSet(work->charSprId[0], scale, scale);
                    espTPLvlSet(work->charSprId[0], 0.5 * (HuCos(90.0f * t) * HuCos(90.0f * t)));
                    espZRotSet(work->charSprId[0], 0.0f);
                    espPosSet(work->charSprId[1], work->pos.x, work->pos.y);
                    espScaleSet(work->charSprId[1], scale, scale);
                    espTPLvlSet(work->charSprId[1], 0.5 * (HuCos(90.0f * t) * HuCos(90.0f * t)));
                    espZRotSet(work->charSprId[1], 0.0f);
                } else {
                    scale = 1.0f;
                    espScaleSet(work->sprId, scale, scale);
                    espScaleSet(work->backSprId, scale, scale);
                    espScaleSet(work->charSprId[0], 0.0f, 0.0f);
                    espScaleSet(work->charSprId[1], 0.0f, 0.0f);
                    work->mode++;
                    work->time = 0;
                }
                break;

            case 2:
                if(work->hideF) {
                    scale = 1.0f;
                    espScaleSet(work->sprId, scale, scale);
                    espScaleSet(work->backSprId, scale, scale);
                    work->mode = 64;
                    work->time = 0;
                }
                break;

            case 32:
                t = ++work->time / 240.0f;
                if(t >= 1.0f && !work->hideF) {
                    t = fmod(t, 1.0);
                    work->time = work->time - 240.0f;
                }
                scale = 1.0 + 0.5 * fabs(HuSin(1440.0f * t));
                if(t < 1.0f && !work->hideF) {
                    espScaleSet(work->charSprId[0], scale, scale);
                    espTPLvlSet(work->charSprId[0], 0.25 + 0.25 * fabs(HuSin(1440.0f * t)));
                    espZRotSet(work->charSprId[0], 0.0f);
                    espScaleSet(work->charSprId[1], scale, scale);
                    espTPLvlSet(work->charSprId[1], 0.25 + 0.25 * fabs(HuSin(1440.0f * t)));
                    espZRotSet(work->charSprId[0], 0.0f);
                } else {
                    scale = 1.0f;
                    espScaleSet(work->sprId, scale, scale);
                    espScaleSet(work->backSprId, scale, scale);
                    espScaleSet(work->charSprId[0], 0.0f, 0.0f);
                    espScaleSet(work->charSprId[1], 0.0f, 0.0f);
                    work->mode = 64;
                    work->time = 0;
                }
                break;

            case 64:
                t = ++work->time / 30.0f;
                scale = HuCos(90.0f * t);
                if(t < 1.0f) {
                    espPosSet(work->sprId, work->pos.x, work->pos.y - 280.0f * t);
                    espPosSet(work->backSprId, work->pos.x, work->pos.y - 280.0f * t);
                } else {
                    espDispOff(work->sprId);
                    espDispOff(work->backSprId);
                    espDispOff(work->charSprId[0]);
                    espDispOff(work->charSprId[1]);
                    work->mode++;
                    work->time = 0;
                }
                break;
        }
    }
}

/* @@PART3@@ */
#include "game/board/dice.h"
#include "game/board/guide.h"
#include "game/board/mgcall.h"
#include "game/board/pause.h"
#include "game/board/scroll.h"
#include "game/board/status.h"
#include "game/board/telop.h"
#include "game/esprite.h"
int abs(int x);
/* Part 3: the miracle trade sprites' tail, Kettou (duel) and Donkey. */


static int ev_CapKettouStart(CAPWORK *work);
static void ev_CapKettouReturn(CAPWORK *work);
static int ev_CapKettouMesGet(int mesId);
static int ev_CapDonkeyStart(CAPWORK *work);
static void ev_CapDonkeyCoin(CAPWORK *work);
static void ev_CapDonkeyReturn(CAPWORK *work);
static void ev_CapDonkeyOMExec(OMOBJ *obj);

static void ev_CapMiracleSprDestroy(void)
{
    miracleSprObj = NULL;
}

static void ev_CapMiracleTradeCreate(HuVecF *pos, int no)
{
    OMOBJ *obj = miracleSprObj;
    CAPMIRACLESPRWORK *sprWork;
    int i;
    int j;
    int fileNo;

    if(miracleSprObj == NULL) {
        return;
    }
    sprWork = obj->data;
    for(i = 0; i < 6; i++, sprWork++) {
        if(sprWork->activeF == FALSE) {
            break;
        }
    }
    if(i >= 6) {
        return;
    }
    sprWork->activeF = TRUE;
    sprWork->mode = 0;
    sprWork->time = 0;
    sprWork->scale = 0.0f;
    sprWork->t = 0.0f;
    sprWork->pos = *pos;
    if(no < 0) {
        no = 0;
    } else if(no > 5) {
        no = 5;
    }
    fileNo = miracleTradeFileTbl[no];
    sprWork->sprId = espEntry(fileNo, 100, 0);
    espPosSet(sprWork->sprId, pos->x, pos->y);
    espScaleSet(sprWork->sprId, 0.0f, 0.0f);
    espAttrSet(sprWork->sprId, 8);
    espDrawNoSet(sprWork->sprId, 32);
    sprWork->backSprId = espEntry(miracleBackFile, 120, 0);
    espPosSet(sprWork->backSprId, pos->x, pos->y);
    espScaleSet(sprWork->backSprId, 0.0f, 0.0f);
    espAttrSet(sprWork->backSprId, 8);
    espDrawNoSet(sprWork->backSprId, 32);
    for(j = 0; j < 6; j++) {
        sprWork->charSprId[j] = espEntry(fileNo, 100, 0);
        espPosSet(sprWork->charSprId[j], pos->x, pos->y);
        espScaleSet(sprWork->charSprId[j], 0.0f, 0.0f);
        espAttrSet(sprWork->charSprId[j], 8);
        espDrawNoSet(sprWork->charSprId[j], 32);
    }
}

static void ev_CapMiracleTradeFocusSet(void)
{
    OMOBJ *obj = miracleSprObj;
    CAPMIRACLESPRWORK *sprWork;
    int i;

    if(miracleSprObj == NULL) {
        return;
    }
    sprWork = obj->data;
    for(i = 0; i < 6; i++, sprWork++) {
        if(sprWork->activeF) {
            sprWork->mode = 32;
            sprWork->time = 0;
        }
    }
}

static void ev_CapMiracleTradeHideSet(void)
{
    OMOBJ *obj = miracleSprObj;
    CAPMIRACLESPRWORK *sprWork;
    int i;

    if(miracleSprObj == NULL) {
        return;
    }
    sprWork = obj->data;
    for(i = 0; i < 6; i++, sprWork++) {
        if(sprWork->activeF) {
            sprWork->hideF = TRUE;
        }
    }
}

void mbev_CapKettou(void)
{
    CAPWORK *work = HuPrcCurrentGet()->property;
    OMOBJ *guideObj;
    int timeF;
    int guideMdlId;
    int i;
    int diff;
    int rand;
    int total;
    int prob[3];

    mbev_CapWait(work);
    if(!GwSystem.curTime) {
        timeF = 0;
    } else {
        timeF = 1;
    }
    guideObj = mbGuideCreateIn();
    guideMdlId = mbGuideModelGet(guideObj);
    mbObjDispSet(guideMdlId, FALSE);
    for(i = 0; kettouGuideMotTbl[timeF][i] >= 0; i++) {
        kettouMotId[i] = mbObjMotionCreate(guideMdlId, kettouGuideMotTbl[timeF][i]);
    }
    mbObjMotionSet(guideMdlId, kettouMotId[1], HU3D_MOTATTR_LOOP);
    mbPlayerColSnapPlayerSet(work->playerNo, TRUE);
    work->objId = guideMdlId;
    work->guideObj = guideObj;
    if(!work->flag.kettouEndF) {
        if(ev_CapKettouStart(work)) {
            GWMgCoinBonusSet(mgResultData[0], 0);
            GWMgCoinBonusSet(mgResultData[1], 0);
            for(i = 0; i < 4; i++) {
                GwPlayerConf[i].grpNo = 2;
            }
            GwPlayerConf[mgResultData[0]].grpNo = 0;
            GwPlayerConf[mgResultData[1]].grpNo = 1;
            if((GwPlayer[mgResultData[0]].comF && GwPlayer[mgResultData[1]].comF && !GWMgComDispGet()) || mbMgRouletteNumGet(6) <= 0) {
                diff = abs(GwPlayer[mgResultData[0]].comDif-GwPlayer[mgResultData[1]].comDif);
                if(diff < 0) {
                    diff = 0;
                } else if(diff > 3) {
                    diff = 3;
                }
                prob[0] = 15-(diff*4);
                prob[1] = (GwPlayer[mgResultData[0]].comDif*20)+50;
                prob[2] = (GwPlayer[mgResultData[1]].comDif*20)+50;
                total = prob[0]+prob[1]+prob[2];
                rand = mbRandMod(total);
                if(rand < prob[0]) {
                    GWMgCoinBonusSet(mgResultData[0], 0);
                    GWMgCoinBonusSet(mgResultData[1], 0);
                } else if(rand < prob[1]) {
                    GWMgCoinBonusSet(mgResultData[0], 10);
                } else {
                    GWMgCoinBonusSet(mgResultData[1], 10);
                }
                mbWipeFadeOut();
                ev_CapKettouReturn(work);
            } else {
                mbWinCreate(2, ev_CapKettouMesGet(0x3D000C), mbGuideSpeakerNoGet());
                mbWinTopWait();
                mbAudFXDelaySet(30);
                mbAudGuidePlay(0x3B6);
                mbObjMotionShiftSet(guideMdlId, kettouMotId[5], 0.0f, 8.0f, HU3D_MOTATTR_NONE);
                mbev_MgCallKettou();
            }
        }
    } else {
        ev_CapKettouReturn(work);
    }
    if(work->guideObj != NULL) {
        mbGuideKill(guideObj);
    }
    HuPrcEnd();
}

void mbev_CapKettouKill(void)
{
}

static int ev_CapKettouStart(CAPWORK *work)
{
    int i;
    int playerNo;
    int otherNo;
    int coinNum;
    int num;
    int t;
    int espId;
    int comNum;
    int holdTime;
    int dir;
    int masuId;
    int nameSprId;
    int vsSprId;
    int starF;
    float scale;
    float tf;
    float stk;
    float absStk;
    HuVecF masuPos;
    HuVecF guidePos;
    HuVecF pos;
    HuVecF dirVec;
    HuVecF center;
    int list[4];
    int comList[4];
    int numSprId[3];
    int coinAdd[4];
    int motId[4];
    char buf[16];
    int starDisp2;
    int starDisp1;
    int coinNum2;
    int coinDispId;
    int guideMdlId;
    int prize;
    int prevNum;
    int prevDir;
    int helpWinId;
    int btn;
    int payChoice;
    int coinF;
    int coinChoiceF;
    int starChoiceF;
    int helpF;
    int skipF;
    int teamF;

    playerNo = work->playerNo;
    masuId = GwPlayer[playerNo].masuId;
    mbPlayerPosGet(playerNo, &masuPos);
    guideMdlId = work->objId;
    if(work->flag.kettouF) {
        skipF = TRUE;
    } else {
        skipF = FALSE;
        center.x = center.y = center.z = 0.0f;
        for(i = 0, num = 0; i < 4; i++) {
            if(masuId != GwPlayer[i].masuId) {
                continue;
            }
            if(num >= 2) {
                t = mbRandMod(3);
            } else {
                t = mbRandMod(2);
            }
            motId[i] = mbev_CapPlayerMotionCreate(&work->objWork, i, kettouPlayerMotTbl[t]);
            if(t == 2) {
                Hu3DMotionAttrSet(mbObjMotionIDGet(mbPlayerObjIDGet(i), motId[i]), 1);
            }
            mbPlayerPosGet(i, &pos);
            PSVECAdd(&center, &pos, &center);
            if(i != playerNo) {
                list[num] = i;
                num++;
            }
        }
        PSVECScale(&center, &center, 1.0f/(num+1));
        for(i = 0; i < 4; i++) {
            if(masuId == GwPlayer[i].masuId) {
                mbPlayerPosGet(i, &pos);
                PSVECSubtract(&center, &pos, &dirVec);
                mbPlayerRotateStart(i, HuAtan(dirVec.x, dirVec.z), 15);
            }
        }
        do {
            for(i = 0; i < 4; i++) {
                if(masuId == GwPlayer[i].masuId && !mbPlayerRotateCheck(i)) {
                    break;
                }
            }
            HuPrcVSleep();
        } while(i < 4);
        for(i = 0; i < 4; i++) {
            if(masuId == GwPlayer[i].masuId) {
                mbPlayerMotionShiftSet(i, motId[i], 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
            }
        }
        HuPrcSleep(60);
    }
    mbMasuPosGet(masuId, &masuPos);
    mbWipeSpecialFadeInCreate(2, 1);
    for(i = 0; i < 4; i++) {
        mbPlayerMotionSet(i, 1, HU3D_MOTATTR_LOOP);
        mbPlayerRotSet(i, 0.0f, 0.0f, 0.0f);
    }
    guidePos.x = masuPos.x;
    guidePos.y = 200.0f+masuPos.y;
    guidePos.z = masuPos.z-50.0f;
    mbObjPosSetV(guideMdlId, &guidePos);
    mbObjDispSet(guideMdlId, TRUE);
    mbStatusDispForceSetAll(TRUE);
    pos.x = 0.0f;
    pos.y = 100.0f;
    pos.z = 0.0f;
    mbCameraMovePlayer(playerNo, NULL, &pos, 1500.0f, -1.0f, -1);
    mbCameraMoveWait();
    espId = mbev_CapSprCreate(&work->objWork, 0x110041, 120, 0);
    nameSprId = espId;
    espPosSet(espId, 288.0f, 224.0f);
    espTPLvlSet(espId, 0.8f);
    espDispOff(espId);
    espId = mbev_CapSprCreate(&work->objWork, 0x110043, 110, 0);
    vsSprId = espId;
    espPosSet(espId, 230.0f, 224.0f);
    espDispOff(espId);
    espId = mbev_CapSprCreate(&work->objWork, 0x110042, 100, 0);
    numSprId[0] = espId;
    espPosSet(espId, 272.0f, 224.0f);
    espBankSet(espId, 10);
    espDispOff(espId);
    espId = mbev_CapSprCreate(&work->objWork, 0x110042, 100, 0);
    numSprId[1] = espId;
    espPosSet(espId, 312.0f, 224.0f);
    espBankSet(espId, 0);
    espDispOff(espId);
    espId = mbev_CapSprCreate(&work->objWork, 0x110042, 100, 0);
    numSprId[2] = espId;
    espPosSet(espId, 352.0f, 224.0f);
    espBankSet(espId, 1);
    espDispOff(espId);
    helpF = FALSE;
    mbMusBoardFadeOut(0, 0, 1000, 1000, 25, FALSE);
    mbWipeSpecialFadeOutCreate(2, 60);
    mbAudGuidePlay(0x3B6);
    mbWinCreate(2, ev_CapKettouMesGet(0x3D0000), mbGuideSpeakerNoGet());
    mbWinTopWait();
    if(mbPlayerStarGet(playerNo) <= 0 && mbPlayerCoinGet(playerNo) < 40) {
        starF = FALSE;
    } else {
        starF = TRUE;
    }
    if(mbPlayerCoinGet(playerNo) <= 0) {
        coinF = FALSE;
    } else {
        coinF = TRUE;
    }
    if(skipF) {
        for(i = 0, num = 0; i < 4; i++) {
            if((mbPlayerCoinGet(i) > 0 || mbPlayerStarGet(i) > 0)
                && (starF || mbPlayerCoinGet(i) > 0)
                && (coinF || mbPlayerStarGet(i) > 0)
                && (!GWTeamGet() || !mbev_CapPlayerCheck(playerNo, i))
                && i != playerNo) {
                list[num] = i;
                num++;
            }
        }
    } else {
        for(i = 0, num = 0; i < 4; i++) {
            if((mbPlayerCoinGet(i) > 0 || mbPlayerStarGet(i) > 0)
                && (starF || mbPlayerCoinGet(i) > 0)
                && (coinF || mbPlayerStarGet(i) > 0)
                && (!GWTeamGet() || !mbev_CapPlayerCheck(playerNo, i))
                && i != playerNo
                && masuId == GwPlayer[i].masuId) {
                list[num] = i;
                num++;
            }
        }
    }
    if(mbPlayerCoinGet(playerNo) <= 0 && mbPlayerStarGet(playerNo) <= 0) {
        num = 0;
    }
selectPlayer:
    switch(num) {
        case 3:
            do {
                mbWinCreateChoice(1, ev_CapKettouMesGet(0x3D0001), mbGuideSpeakerNoGet(), 0);
                mbWinTopAttrSet(0x10);
                for(i = 0; i < num; i++) {
                    comList[i] = list[i];
                }
                if(GwPlayer[playerNo].comF) {
                    if(starF) {
                        otherNo = mbev_CapPlayerComSelKettouGet(playerNo, 1, comList, num);
                    } else {
                        otherNo = mbev_CapPlayerComSelKettouGet(playerNo, 0, comList, num);
                    }
                    mbComChoiceListDownSet(otherNo);
                }
                mbWinTopInsertMesSet(mbPlayerNameMesGet(list[0]), 0);
                mbWinTopInsertMesSet(mbPlayerNameMesGet(list[1]), 1);
                mbWinTopInsertMesSet(mbPlayerNameMesGet(list[2]), 2);
                mbWinTopWait();
                otherNo = mbWinTopChoiceGet();
                if(otherNo == 4) {
                    mbev_Scroll(playerNo, FALSE);
                    mbev_CapStatusDispSetAll(TRUE, TRUE);
                }
            } while(otherNo == 4);
            if(otherNo >= 3 || otherNo == -1) {
                otherNo = list[mbRandMod(3)];
                if(otherNo < 0) {
                    otherNo = list[0];
                }
            } else {
                otherNo = list[otherNo];
            }
            break;

        case 2:
            do {
                mbWinCreateChoice(1, ev_CapKettouMesGet(0x3D0002), mbGuideSpeakerNoGet(), 0);
                mbWinTopAttrSet(0x10);
                for(i = 0; i < num; i++) {
                    comList[i] = list[i];
                }
                if(GwPlayer[playerNo].comF) {
                    if(starF) {
                        otherNo = mbev_CapPlayerComSelKettouGet(playerNo, 1, comList, num);
                    } else {
                        otherNo = mbev_CapPlayerComSelKettouGet(playerNo, 0, comList, num);
                    }
                    mbComChoiceListDownSet(otherNo);
                }
                mbWinTopInsertMesSet(mbPlayerNameMesGet(list[0]), 0);
                mbWinTopInsertMesSet(mbPlayerNameMesGet(list[1]), 1);
                mbWinTopWait();
                otherNo = mbWinTopChoiceGet();
                if(otherNo == 3) {
                    mbev_Scroll(playerNo, FALSE);
                    mbev_CapStatusDispSetAll(TRUE, TRUE);
                }
            } while(otherNo == 3);
            if(otherNo >= 2 || otherNo == -1) {
                otherNo = list[mbRandMod(2)];
                if(otherNo < 0) {
                    otherNo = list[0];
                }
            } else {
                otherNo = list[otherNo];
            }
            break;

        case 1:
            otherNo = list[0];
            break;

        default:
            teamF = FALSE;
            if(GWTeamGet() && !skipF) {
                for(i = 0, num = 0; i < 4; i++) {
                    if(i != playerNo && GwPlayer[playerNo].masuId == GwPlayer[i].masuId) {
                        list[num] = i;
                num++;
                    }
                }
                if(num == 1 && mbev_CapPlayerCheck(playerNo, list[0])) {
                    teamF = TRUE;
                }
            }
            mbAudGuidePlay(0x3B7);
            if(teamF) {
                mbWinCreate(2, ev_CapKettouMesGet(0x3D0015), mbGuideSpeakerNoGet());
                mbWinTopInsertMesSet(mbPlayerNameMesGet(playerNo), 0);
                mbWinTopInsertMesSet(mbPlayerNameMesGet(list[0]), 1);
                mbWinTopWait();
            } else {
                mbWinCreate(2, ev_CapKettouMesGet(0x3D0014), mbGuideSpeakerNoGet());
                mbWinTopWait();
            }
            mbMusBoardFadeOut(0, 0, 1000, 1000, -1, FALSE);
            mbGuideEnd(work->guideObj, TRUE);
            work->guideObj = NULL;
            return FALSE;
    }
selectPrize:
    mbWinCreateChoice(1, ev_CapKettouMesGet(0x3D0003), mbGuideSpeakerNoGet(), 0);
    if(num < 2) {
        mbWinTopAttrSet(0x10);
        mbWinTopChoiceDisable(2);
    }
    coinChoiceF = starChoiceF = TRUE;
    if(mbPlayerCoinGet(otherNo) <= 0 || !coinF) {
        mbWinTopChoiceDisable(0);
        coinChoiceF = FALSE;
    }
    if(mbPlayerStarGet(otherNo) <= 0 || !starF) {
        mbWinTopChoiceDisable(1);
        starChoiceF = FALSE;
    }
    if(GwPlayer[playerNo].comF) {
        if(coinChoiceF && starChoiceF) {
            mbComChoiceDownSet();
        } else {
            mbComChoiceUpSet();
        }
    }
    mbWinTopWait();
    prize = mbWinTopChoiceGet();
    if(prize == 2 || prize == -1) {
        goto selectPlayer;
    }
    if(prize == 0) {
        prevNum = coinNum = 1;
        sprintf(buf, "%d", coinNum);
        prevDir = holdTime = dir = 0;
        mbWinCreate(2, ev_CapKettouMesGet(0x3D0005), mbGuideSpeakerNoGet());
        mbWinTopWait();
        if(!helpF) {
            espDispOn(nameSprId);
            espDispOn(vsSprId);
            for(i = 0; i < 3; i++) {
                espDispOn(numSprId[i]);
            }
            espBankSet(numSprId[1], 0);
            espBankSet(numSprId[2], 1);
            for(t = 0; t <= 12.0f; t++) {
                tf = t/12.0f;
                scale = HuSin(90.0f*tf);
                espScaleSet(nameSprId, 1.0f, scale);
                espScaleSet(vsSprId, 1.0f, scale);
                for(i = 0; i < 3; i++) {
                    espScaleSet(numSprId[i], 1.0f, scale);
                }
                HuPrcVSleep();
            }
            helpWinId = mbWinCreateHelp(0x3D0016);
            mbWinPosSet(helpWinId, 115, 288);
            helpF = TRUE;
        }
        comNum = mbPlayerCoinGet(playerNo);
        comNum = comNum*CAP_RANDF();
        if(CAP_RANDF() < 0.3f) {
            comNum = mbPlayerCoinGet(playerNo);
        }
        if(comNum > mbPlayerCoinGet(otherNo)) {
            comNum = mbPlayerCoinGet(otherNo);
        }
        if(comNum < 1) {
            comNum = 1;
        }
        if(comNum > 99) {
            comNum = 99;
        }
        while(1) {
            btn = HuPadBtnDown[GwPlayer[playerNo].padNo];
            stk = mbPadStkYGet(GwPlayer[playerNo].padNo);
            if(GwPlayer[playerNo].comF) {
                btn = 0;
                stk = 0.0f;
                if(coinNum < comNum) {
                    stk = 32.0f;
                } else {
                    HuPrcSleep(5);
                    btn = PAD_BUTTON_A;
                }
            }
            if(btn & PAD_BUTTON_A) {
                mbAudFXPlay(2);
                break;
            }
            if(btn & PAD_BUTTON_B) {
                if(helpF) {
                    for(t = 0; t <= 12.0f; t++) {
                        tf = t/12.0f;
                        scale = HuCos(90.0f*tf);
                        espScaleSet(nameSprId, 1.0f, scale);
                        espScaleSet(vsSprId, 1.0f, scale);
                        for(i = 0; i < 3; i++) {
                            espScaleSet(numSprId[i], 1.0f, scale);
                        }
                        HuPrcVSleep();
                    }
                    espDispOff(nameSprId);
                    espDispOff(vsSprId);
                    for(i = 0; i < 3; i++) {
                        espDispOff(numSprId[i]);
                    }
                    mbWinKill(helpWinId);
                    helpF = FALSE;
                }
                goto selectPrize;
            }
            absStk = __fabsf(stk);
            if(absStk >= 8.0) {
                if(stk >= 10.0f) {
                    dir = 1;
                }
                if(stk <= 10.0f) {
                    dir = -1;
                }
                if(dir == 0) {
                    holdTime = 0;
                } else if(prevDir == dir) {
                    holdTime++;
                    if(holdTime > 30.0f) {
                        coinNum += dir;
                        holdTime -= 2;
                    }
                } else {
                    coinNum += dir;
                    holdTime = 0;
                }
                prevDir = dir;
                if(coinNum < 1) {
                    coinNum = 1;
                } else if(coinNum > 99) {
                    coinNum = 99;
                } else if(coinNum > mbPlayerCoinGet(playerNo)) {
                    coinNum = mbPlayerCoinGet(playerNo);
                } else if(coinNum > mbPlayerCoinGet(otherNo)) {
                    coinNum = mbPlayerCoinGet(otherNo);
                }
            } else {
                prevDir = holdTime = dir = 0;
            }
            if(prevNum != coinNum) {
                mbAudFXPlay(0);
                if(coinNum >= 100) {
                    espBankSet(numSprId[1], 9);
                } else {
                    espBankSet(numSprId[1], coinNum/10);
                }
                espBankSet(numSprId[2], coinNum%10);
                prevNum = coinNum;
                if(holdTime < 27.0f) {
                    HuPrcSleep(3);
                }
            }
            HuPrcVSleep();
        }
        if(helpF) {
            for(t = 0; t <= 12.0f; t++) {
                tf = t/12.0f;
                scale = HuCos(90.0f*tf);
                espScaleSet(nameSprId, 1.0f, scale);
                espScaleSet(vsSprId, 1.0f, scale);
                for(i = 0; i < 3; i++) {
                    espScaleSet(numSprId[i], 1.0f, scale);
                }
                HuPrcVSleep();
            }
            espDispOff(nameSprId);
            espDispOff(vsSprId);
            for(i = 0; i < 3; i++) {
                espDispOff(numSprId[i]);
            }
            mbWinKill(helpWinId);
            helpF = FALSE;
        }
        payChoice = 0;
    } else {
        mbWinCreateChoice(1, ev_CapKettouMesGet(0x3D0004), mbGuideSpeakerNoGet(), 0);
        coinChoiceF = starChoiceF = TRUE;
        if(mbPlayerCoinGet(playerNo) < 40) {
            mbWinTopChoiceDisable(0);
            coinChoiceF = FALSE;
        }
        if(mbPlayerStarGet(playerNo) <= 0) {
            mbWinTopChoiceDisable(1);
            starChoiceF = FALSE;
        }
        if(GwPlayer[playerNo].comF) {
            if(coinChoiceF) {
                mbComChoiceUpSet();
            } else {
                mbComChoiceRightSet();
            }
        }
        mbWinTopWait();
        payChoice = mbWinTopChoiceGet();
        if(payChoice == 2 || payChoice == -1) {
            goto selectPrize;
        }
        if(payChoice == 0) {
            coinNum = 40;
        } else {
            coinNum = 0;
        }
    }
    mbev_CapStatusDispSetAll(FALSE, TRUE);
    mbev_CapDuelStatusOnSet(playerNo, otherNo, TRUE);
    if(prize == 0) {
        comNum = coinNum;
        mbAudGuidePlay(0x3B8);
        sprintf(buf, "%d", coinNum);
        mbWinCreate(2, ev_CapKettouMesGet(0x3D0006), mbGuideSpeakerNoGet());
        mbWinTopInsertMesSet((u32)buf, 0);
        mbWinTopWait();
        for(i = 0; i < 4; i++) {
            coinAdd[i] = 0;
        }
        coinAdd[playerNo] = -coinNum;
        coinAdd[otherNo] = -coinNum;
        mbCoinAddAllProcExecV(coinAdd, coinAdd, FALSE);
        sprintf(buf, "%d", coinNum+comNum);
        mbWinCreate(2, ev_CapKettouMesGet(0x3D0009), mbGuideSpeakerNoGet());
        mbWinTopInsertMesSet((u32)buf, 0);
        mbWinTopWait();
        memset(mgResultData, 0, sizeof(mgResultData));
        mgResultData[0] = playerNo;
        mgResultData[1] = otherNo;
        mgResultData[2] = coinNum+comNum;
    } else if(payChoice != 0) {
        mbAudGuidePlay(0x3B8);
        mbWinCreate(2, ev_CapKettouMesGet(0x3D0007), mbGuideSpeakerNoGet());
        mbWinTopWait();
        mbPlayerStarAdd(playerNo, -1);
        mbPlayerStarAdd(otherNo, -1);
        starDisp1 = mbStarDispPlayerCreate(playerNo, -1);
        HuPrcVSleep();
        starDisp2 = mbStarDispPlayerCreate(otherNo, -1);
        do {
            HuPrcVSleep();
        } while(!mbStarDispCheck(starDisp1) || !mbStarDispCheck(starDisp2));
        mbWinCreate(2, ev_CapKettouMesGet(0x3D000A), mbGuideSpeakerNoGet());
        mbWinTopWait();
        memset(mgResultData, 0, sizeof(mgResultData));
        mgResultData[0] = playerNo;
        mgResultData[1] = otherNo;
        mgResultData[3] = 2;
    } else {
        mbAudGuidePlay(0x3B8);
        mbWinCreate(2, ev_CapKettouMesGet(0x3D0008), mbGuideSpeakerNoGet());
        mbWinTopWait();
        coinNum2 = mbCoinAddExec(playerNo, -coinNum);
        mbPlayerStarAdd(otherNo, -1);
        mbPlayerPosGet(playerNo, &pos);
        pos.y += 250.0f;
        coinDispId = mbCoinDispCapsuleCreate(&pos, -coinNum);
        starDisp2 = mbStarDispPlayerCreate(otherNo, -1);
        do {
            HuPrcVSleep();
        } while(!mbCoinDispKillCheck(coinDispId) || !mbStarDispCheck(starDisp2));
        mbWinCreate(2, ev_CapKettouMesGet(0x3D000B), mbGuideSpeakerNoGet());
        mbWinTopWait();
        memset(mgResultData, 0, sizeof(mgResultData));
        mgResultData[0] = playerNo;
        mgResultData[1] = otherNo;
        mgResultData[2] = coinNum;
        mgResultData[3] = 1;
    }
    mbev_CapDuelStatusDispSet(playerNo, otherNo, TRUE);
    return TRUE;
}

static inline s16 CapMgCoinBonusGet(s32 playerNo)
{
    return GwPlayer[playerNo].mgCoinBonus;
}


static void ev_CapKettouReturn(CAPWORK *work)
{
    int viewF;
    int playerNo;
    int masuId;
    int guideMdlId;
    int loser;
    int disablePlayer;
    int winner;
    int i;
    int motId;
    int coinDispId;
    int starDispId;
    HuVecF pos;
    HuVecF guidePos;
    HuVecF pos2;
    char buf[16];

    viewF = FALSE;
    playerNo = work->playerNo;
    masuId = GwPlayer[playerNo].masuId;
    mbev_PlayerColMasu(playerNo, masuId, TRUE);
    mbPlayerPosGet(playerNo, &pos);
    guideMdlId = work->objId;
    disablePlayer = playerNo;
    mbPlayerMotionSet(playerNo, 1, HU3D_MOTATTR_LOOP);
    mbPlayerRotSet(playerNo, 0.0f, 0.0f, 0.0f);
    guidePos.x = pos.x;
    guidePos.y = 200.0f+pos.y;
    guidePos.z = pos.z-50.0f;
    mbObjPosSetV(guideMdlId, &guidePos);
    mbObjDispSet(guideMdlId, TRUE);
    mbStatusDispForceSetAll(TRUE);
    pos2.x = 0.0f;
    pos2.y = 100.0f;
    pos2.z = 0.0f;
    mbCameraPlayerViewSetFast(playerNo, 0);
    mbCameraMoveWait();
    if(CapMgCoinBonusGet(mgResultData[0]) > 0 && CapMgCoinBonusGet(mgResultData[1]) == 0) {
        winner = mgResultData[0];
    } else if(CapMgCoinBonusGet(mgResultData[1]) > 0 && CapMgCoinBonusGet(mgResultData[0]) == 0) {
        winner = mgResultData[1];
    } else {
        winner = -1;
    }
    if(winner == mgResultData[1] && winner != -1 && GwPlayer[mgResultData[0]].masuId != GwPlayer[mgResultData[1]].masuId) {
        mbMasuPosGet(GwPlayer[winner].masuId, &pos);
        guidePos.x = pos.x;
        guidePos.y = 200.0f+pos.y;
        guidePos.z = pos.z-50.0f;
        mbObjPosSetV(guideMdlId, &guidePos);
        mbev_PlayerColMasu(mgResultData[1], GwPlayer[mgResultData[1]].masuId, TRUE);
        mbCameraPlayerViewSetFast(mgResultData[1], 0);
        mbCameraMoveWait();
        viewF = TRUE;
    }
    if(winner == mgResultData[1] && winner != -1) {
        disablePlayer = winner;
    }
    if(work->flag.kettouEndF) {
        mbMusPlay(0, 26, 127, 0);
    }
    mbWipeFadeIn();
    mbPauseDisableSet(FALSE);
    if(winner != -1) {
        if(mgResultData[0] == winner) {
            loser = mgResultData[1];
        } else {
            loser = mgResultData[0];
        }
        if(mgResultData[2] > 0 && mgResultData[3] > 0) {
            mbAudGuidePlay(0x3B8);
            mbWinCreate(2, ev_CapKettouMesGet(0x3D000F), mbGuideSpeakerNoGet());
            mbWinTopInsertMesSet(mbPlayerNameMesGet(winner), 0);
        } else if(mgResultData[3] > 0) {
            mbAudGuidePlay(0x3B8);
            mbWinCreate(2, ev_CapKettouMesGet(0x3D000E), mbGuideSpeakerNoGet());
            mbWinTopInsertMesSet(mbPlayerNameMesGet(winner), 0);
        } else {
            mbAudGuidePlay(0x3B8);
            mbWinCreate(2, ev_CapKettouMesGet(0x3D000D), mbGuideSpeakerNoGet());
            mbWinTopInsertMesSet(mbPlayerNameMesGet(winner), 0);
            sprintf(buf, "%d", mgResultData[2]);
            mbWinTopInsertMesSet((u32)buf, 1);
        }
        mbWinTopPlayerDisable(disablePlayer);
        mbWinTopWait();
        if(mgResultData[2] != 0) {
            mbPlayerPosGet(winner, &pos2);
            pos2.y += 250.0f;
            coinDispId = mbCoinDispCapsuleCreate(&pos2, mgResultData[2]);
            mbCoinAddExec(winner, mgResultData[2]);
            mbPlayerWinLoseVoicePlay(winner, 12, 0x243);
            mbPlayerMotionShiftSet(winner, 12, 0.0f, 8.0f, HU3D_MOTATTR_NONE);
            mbPlayerMotionShiftSet(loser, 13, 0.0f, 8.0f, HU3D_MOTATTR_NONE);
            do {
                HuPrcVSleep();
            } while(mbObjMotionShiftIDGet(mbPlayerObjIDGet(mgResultData[0])) != -1 || !mbPlayerMotionEndCheck(mgResultData[0])
                || mbObjMotionShiftIDGet(mbPlayerObjIDGet(mgResultData[1])) != -1 || !mbPlayerMotionEndCheck(mgResultData[1]));
            for(i = 0; i < 4; i++) {
                mbPlayerMotionShiftSet(i, 1, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
            }
            do {
                HuPrcVSleep();
            } while(!mbCoinDispKillCheck(coinDispId));
        }
        if(mgResultData[3] != 0) {
            mbPlayerStarAdd(winner, mgResultData[3]);
            starDispId = mbStarDispPlayerCreate(winner, mgResultData[3]);
            mbPlayerWinLoseVoicePlay(winner, 7, 0x23D);
            mbPlayerMotionShiftSet(winner, 7, 0.0f, 8.0f, HU3D_MOTATTR_NONE);
            motId = mbev_CapPlayerMotionCreate(&work->objWork, loser, 0x930028);
            mbPlayerMotionShiftSet(loser, motId, 0.0f, 8.0f, HU3D_MOTATTR_NONE);
            do {
                HuPrcVSleep();
            } while(mbObjMotionShiftIDGet(mbPlayerObjIDGet(mgResultData[0])) != -1 || !mbPlayerMotionEndCheck(mgResultData[0])
                || mbObjMotionShiftIDGet(mbPlayerObjIDGet(mgResultData[1])) != -1 || !mbPlayerMotionEndCheck(mgResultData[1])
                || !mbStarDispCheck(starDispId));
            for(i = 0; i < 4; i++) {
                mbPlayerMotionShiftSet(i, 1, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
            }
        }
        if(viewF) {
            mbWipeDissolveFadeOutTime(1);
            mbPlayerPosGet(playerNo, &pos);
            guidePos.x = pos.x;
            guidePos.y = 200.0f+pos.y;
            guidePos.z = pos.z-50.0f;
            mbObjPosSetV(guideMdlId, &guidePos);
            mbCameraPlayerViewSetFast(mgResultData[0], 0);
            mbCameraMoveWait();
            mbWipeDissolveFadeIn();
        }
    } else {
        mbAudGuidePlay(0x3B7);
        if(mgResultData[2] > 0 && mgResultData[3] > 0) {
            mbWinCreate(2, ev_CapKettouMesGet(0x3D0012), mbGuideSpeakerNoGet());
        } else if(mgResultData[3] > 0) {
            mbWinCreate(2, ev_CapKettouMesGet(0x3D0011), mbGuideSpeakerNoGet());
        } else {
            mbWinCreate(2, ev_CapKettouMesGet(0x3D0010), mbGuideSpeakerNoGet());
        }
        mbWinTopPlayerDisable(disablePlayer);
        mbWinTopWait();
        if(mgResultData[2] > 0 && mgResultData[3] > 0) {
            mbCoinAddExec(mgResultData[0], mgResultData[2]);
            mbPlayerStarAdd(mgResultData[1], mgResultData[3]);
        } else if(mgResultData[3] > 0) {
            mbPlayerStarAdd(mgResultData[0], mgResultData[3]/2);
            mbPlayerStarAdd(mgResultData[1], mgResultData[3]/2);
        } else {
            mbCoinAddExec(mgResultData[0], mgResultData[2]/2);
            mbCoinAddExec(mgResultData[1], mgResultData[2]/2);
        }
        mbPlayerWinLoseVoicePlay(playerNo, 13, 0x249);
        mbev_CapPlayerMotShiftWait(playerNo, 13, HU3D_MOTATTR_NONE, TRUE);
        mbev_CapPlayerMotShiftWait(playerNo, 1, HU3D_MOTATTR_LOOP, TRUE);
    }
    mbev_CapPlayerMotShiftSet(guideMdlId, kettouMotId[2], HU3D_MOTATTR_LOOP, TRUE);
    mbAudGuidePlay(0x3B8);
    mbWinCreate(2, ev_CapKettouMesGet(0x3D0013), mbGuideSpeakerNoGet());
    if(viewF) {
        mbWinTopPlayerDisable(playerNo);
    } else {
        mbWinTopPlayerDisable(disablePlayer);
    }
    mbWinTopWait();
    mbMusBoardFadeOut(0, 0, 1000, 1000, -1, FALSE);
    mbGuideEnd(work->guideObj, TRUE);
    work->guideObj = NULL;
}

static int ev_CapKettouMesGet(int mesId)
{
    if(GwSystem.curTime == FALSE) {
        return mesId;
    } else {
        return mesId+0x17;
    }
}

void mbev_CapDonkey(void)
{
    CAPWORK *work = HuPrcCurrentGet()->property;
    int donkeyObjId;
    int i;
    int barrelObjId;
    int camObjId;
    s16 coin;

    mbPlayerMotionShiftSet(work->playerNo, 1, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
    mbev_CapWait(work);
    work->effExplode = mbev_CapEffExplodeCreate();
    HuPrcVSleep();
    work->effCoin = mbev_CapEffCoinCreate();
    HuPrcVSleep();
    donkeyObjId = mbev_CapObjCreate(&work->objWork, 0xE000E, donkeyMotTbl, FALSE, 5, 0);
    mbObjDispSet(donkeyObjId, FALSE);
    barrelObjId = mbev_CapObjCreate(&work->objWork, 0xE0021, NULL, FALSE, 5, 0);
    mbObjDispSet(barrelObjId, FALSE);
    mbObjLayerSet(barrelObjId, 3);
    mbev_CapObjPosSet(&work->objWork, barrelObjId, GwPlayer[work->playerNo].masuId, NULL);
    camObjId = mbev_CapObjCreate(&work->objWork, 0xC0044, NULL, FALSE, 5, 0);
    mbObjDispSet(camObjId, FALSE);
    work->objId = donkeyObjId;
    work->objId2 = barrelObjId;
    work->objId3 = camObjId;
    if(!work->flag.donkeyEndF) {
        if(ev_CapDonkeyStart(work)) {
            if((mbPlayerAllComCheck() && !GWMgComDispGet()) || mbMgRouletteNumGet(7) <= 0) {
                for(i = 0; i < 4; i++) {
                    coin = mbRandMod(10);
                    GWMgCoinBonusSet(i, coin);
                }
                mbWipeFadeOut();
                ev_CapDonkeyCoin(work);
            } else {
                mbWinCreate(2, 0x3E0008, -1);
                mbWinTopWait();
                mbObjMotionShiftSet(donkeyObjId, 10, 0.0f, 8.0f, HU3D_MOTATTR_NONE);
                mbev_MgCallDonkey();
            }
        }
    } else {
        ev_CapDonkeyCoin(work);
    }
    ev_CapDonkeyReturn(work);
    HuPrcEnd();
}

void mbev_CapDonkeyKill(void)
{
}

static int ev_CapDonkeyStart(CAPWORK *work)
{
    int playerNo;
    int masuId;
    int donkeyObjId;
    int barrelObjId;
    int prevNo;
    int camObjId;
    OMOBJ *obj;
    int mgF;
    int i;
    int j;
    int diceNo;
    int no;
    int espId;
    float t;
    float scale;
    float num;
    float speed;
    Mtx mtx;
    HuVecF playerPos;
    HuVecF rot;
    HuVecF pos;
    HuVecF camRot;
    HuVecF masuPos;
    HuVecF ofs;
    HuVecF coinPos;
    HuVecF vel;
    char buf[16];
    int sprId[2];
    int effNo;
    int time;
    int boardNo;

    playerNo = work->playerNo;
    masuId = GwPlayer[playerNo].masuId;
    donkeyObjId = work->objId;
    barrelObjId = work->objId2;
    camObjId = work->objId3;
    mbMusBoardFadeOut(0, 0, 1000, 1000, 29, FALSE);
    mbAudFXPlay(0x454);
    mbev_CapObjPosSet(&work->objWork, barrelObjId, masuId, NULL);
    mbObjDispSet(barrelObjId, TRUE);
    mbObjMotionTimeSet(barrelObjId, 0.0f);
    mbObjMotionSpeedSet(barrelObjId, 1.0f);
    mbMasuPosGet(masuId, &masuPos);
    pos = masuPos;
    playerPos = pos;
    mbObjPosSetV(camObjId, &pos);
    mbCameraRotGet(&camRot);
    mbCameraMoveObj(camObjId, NULL, &capsuleCameraOfs, 1500.0f, -1.0f, 60);
    mbPlayerColSnapPlayerSet(playerNo, FALSE);
    mbAudFXPlay(0x455);
    omVibrate(playerNo, 20, 7, 3);
    do {
        t = mbObjMotionTimeGet(barrelObjId)/mbObjMotionMaxTimeGet(barrelObjId);
        if(t > 1.0f) {
            t = 1.0f;
        }
        Hu3DMotionCalc(mbObjModelIDGet(barrelObjId));
        Hu3DModelObjMtxGet(mbObjModelIDGet(barrelObjId), "itemhook_R", mtx);
        playerPos.x = mtx[0][3];
        playerPos.y = mtx[1][3];
        playerPos.z = mtx[2][3];
        mbPlayerPosSetV(playerNo, &playerPos);
        pos.x = mtx[0][3];
        pos.y = mtx[1][3];
        pos.z = mtx[2][3];
        mbObjPosSetV(camObjId, &pos);
        HuPrcVSleep();
    } while(t < 1.0f);
    mbMasuPosGet(masuId, &masuPos);
    PSVECSubtract(&playerPos, &masuPos, &ofs);
    mbev_CapPlayerPosSet(&work->objWork, playerNo, masuId, &ofs);
    mbAudFXPlay(0x456);
    obj = omAddObjEx(mbObjMan, 32768, 0, 0, -1, ev_CapDonkeyOMExec);
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(CAPWORK), HU_MEMNUM_OVL);
    memcpy(obj->data, work, sizeof(CAPWORK));
    obj->work[0] = obj->work[1] = obj->work[2] = obj->work[3] = 0;
    while(obj->work[0] < 2) {
        HuPrcVSleep();
    }
    mbev_CapPlayerPosSet(&work->objWork, playerNo, -1, &ofs);
    mbPlayerRotGet(playerNo, &rot);
    mbPlayerMotionShiftSet(playerNo, 9, 0.0f, 8.0f, HU3D_MOTATTR_NONE);
    for(i = 1; i <= 30.0f; i++) {
        t = i/30.0f;
        Hu3DModelObjMtxGet(mbObjModelIDGet(barrelObjId), "itemhook_R", mtx);
        playerPos.x = mtx[0][3]-(80.0f*t);
        playerPos.y = mtx[1][3]+(1.5*(100.0*HuSin(180.0f*t)));
        playerPos.z = mtx[2][3]+(80.0f*t);
        mbPlayerPosSetV(playerNo, &playerPos);
        rot.y = mbev_CapAngleLerp(135.0f, rot.y, 10.0f);
        mbPlayerRotSetV(playerNo, &rot);
        HuPrcVSleep();
    }
    while(!obj->work[2]) {
        Hu3DModelObjMtxGet(mbObjModelIDGet(barrelObjId), "itemhook_R", mtx);
        playerPos.x = mtx[0][3]-80.0f;
        playerPos.y = mtx[1][3];
        playerPos.z = 80.0f+mtx[2][3];
        mbPlayerPosSetV(playerNo, &playerPos);
        HuPrcVSleep();
    }
    obj->work[3] = TRUE;
    mbMasuPosGet(masuId, &masuPos);
    PSVECSubtract(&playerPos, &masuPos, &ofs);
    mbev_CapPlayerPosSet(&work->objWork, playerNo, masuId, &ofs);
    mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
    if(CAP_RANDF() < 0.3f) {
        mgF = FALSE;
    } else {
        mgF = TRUE;
    }
    mbev_CapPlayerMotShiftSet(donkeyObjId, 6, HU3D_MOTATTR_NONE, TRUE);
    mbObjMotionShiftSet(donkeyObjId, 1, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
    mbWinCreate(2, 0x3E0000, -1);
    mbWinTopWait();
    mbAudFXPlay(0x3A8);
    mbObjMotionShiftSet(donkeyObjId, 10, 0.0f, 8.0f, HU3D_MOTATTR_NONE);
    HuPrcSleep(30);
    mbAudFXPlay(0x45D);
    espId = mbev_CapSprCreate(&work->objWork, mbBoardDataNumGet(donkeyMgFile[mgF]), 100, mgF^1);
    for(i = 1; i < 60.0f; i++) {
        t = i/60.0f;
        scale = HuSin(90.0f*t);
        espPosSet(espId, 288.0+(50.0*HuCos(90.0f*t)), 240.0-(250.0*HuSin(180.0f*t)));
        espScaleSet(espId, scale, scale);
        espZRotSet(espId, 3.0f*(360.0f*-t));
        HuPrcVSleep();
    }
    for(i = 1; i < 60.0f; i++) {
        t = i/60.0f;
        scale = 1.0+(0.2f*HuSin(720.0f*t));
        espPosSet(espId, 288.0f, 240.0f);
        espScaleSet(espId, scale, scale);
        espZRotSet(espId, 0.0f);
        HuPrcVSleep();
    }
    for(i = 1; i < 18.0f; i++) {
        t = i/18.0f;
        scale = 1.0+(5.0*HuSin(90.0f*t));
        espPosSet(espId, 288.0f, 240.0f);
        espScaleSet(espId, scale, scale);
        espTPLvlSet(espId, 1.0-HuSin(90.0f*t));
        HuPrcVSleep();
    }
    espDispOff(espId);
    mbObjMotionShiftSet(donkeyObjId, 1, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
    if(!mgF) {
        mbWinCreate(2, 0x3E0001, -1);
        mbWinTopWait();
        mbWinCreate(2, 0x3E0002, -1);
        mbWinTopWait();
        mbPlayerRotateStart(playerNo, 0, 15);
        while(!mbPlayerRotateCheck(playerNo)) {
            HuPrcVSleep();
        }
        boardNo = GwSystem.boardNo;
        if(boardNo != 3) {
            diceNo = mbRandMod(5);
        } else {
            diceNo = mbRandMod(4);
        }
        diceNo = mbDiceExec(playerNo, 11, donkeyDiceTbl, diceNo, TRUE, TRUE, NULL, 0);
        if(donkeyDiceResultTbl[diceNo] >= 0) {
            sprintf(buf, "%d", donkeyDiceResultTbl[diceNo]);
            mbWinCreate(2, 0x3E0003, -1);
            mbWinTopInsertMesSet((u32)buf, 0);
            mbWinTopWait();
            mbDiceFadeSet(playerNo);
            mbAudFXPlay(0x3A7);
            mbev_CapPlayerMotShiftSet(donkeyObjId, 10, HU3D_MOTATTR_NONE, TRUE);
            mbPlayerPosGet(playerNo, &ofs);
            for(i = 0; i < donkeyDiceResultTbl[diceNo]; i++) {
                coinPos = ofs;
                coinPos.y += 600.0f;
                coinPos.x += 0.5f*(100.0f*(-0.5f+CAP_RANDF()));
                vel.x = vel.y = vel.z = 0.0f;
                effNo = mbev_CapEffCoinAdd(work->effCoin, &coinPos, &vel, 0.75f, 4.9f, 30, 4);
                if(effNo >= 0) {
                    mbev_CapEffCoinMaxYSet(work->effCoin, effNo, 150.0f+ofs.y);
                }
                HuPrcVSleep();
            }
            while(mbev_CapEffCoinNumGet(work->effCoin) > 0) {
                HuPrcVSleep();
            }
            mbObjMotionShiftSet(donkeyObjId, 1, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
            mbCoinAddDispExec(playerNo, donkeyDiceResultTbl[diceNo], FALSE, TRUE);
            mbev_CapCoinDisp(playerNo, donkeyDiceResultTbl[diceNo], TRUE, TRUE);
        } else {
            mbWinCreate(2, 0x3E0004, -1);
            mbWinTopWait();
            mbMusPauseFadeOut(0, TRUE, -1);
            mbDiceFadeSet(playerNo);
            mbAudFXPlay(0x3A7);
            mbev_CapObjMotionSet(donkeyObjId, 30, 10, 1, 0, HU3D_MOTATTR_LOOP, 1, 1);
            mbStarGetExec(playerNo);
            mbCameraMoveObj(camObjId, &camRot, &capsuleCameraOfs, 1500.0f, -1.0f, 60);
            mbMusPlay(0, 29, 127, 0);
        }
        mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
        return FALSE;
    }
    mbWinCreate(2, 0x3E0005, -1);
    mbWinTopWait();
    mbWinCreate(2, 0x3E0006, -1);
    mbWinTopWait();
    mbAudFXPlay(0x3A8);
    mbev_CapPlayerMotShiftSet(donkeyObjId, 8, HU3D_MOTATTR_NONE, TRUE);
    sprId[0] = mbev_CapSprCreate(&work->objWork, 0xE0022, 100, 0);
    espPosSet(sprId[0], 288.0f, 240.0f);
    espScaleSet(sprId[0], 0.0f, 0.0f);
    sprId[1] = mbev_CapSprCreate(&work->objWork, 0xE0023, 100, 0);
    espPosSet(sprId[1], 304.0f, 240.0f);
    espScaleSet(sprId[1], 0.0f, 0.0f);
    espAttrSet(sprId[1], 1);
    espBankSet(sprId[1], 0);
    for(i = 0; i <= 30.0f; i++) {
        t = i/30.0f;
        scale = HuSin(90.0f*t)+HuSin(180.0f*t);
        for(j = 0; j < 2; j++) {
            espScaleSet(sprId[j], scale, scale);
        }
        espPosSet(sprId[1], 288.0f+(16.0f*scale), 240.0f);
        HuPrcVSleep();
    }
    time = 60.0f+(0.5f*(60.0f*CAP_RANDF()));
    num = 0.0f;
    speed = 0.3f;
    no = prevNo = 0;
    for(i = 0; i <= time; i++) {
        num += speed;
        if(num >= 10.0f) {
            num -= 10.0f;
        }
        no = num;
        if(no >= 10) {
            no = 9;
        }
        espBankSet(sprId[1], donkeyRouletteBankTbl[no]);
        if(no != prevNo) {
            mbAudFXPlay(0x3F1);
        }
        prevNo = no;
        HuPrcVSleep();
    }
    for(i = 0; i <= 60; i++) {
        if(speed > 0.02f) {
            speed -= 0.005f;
        }
        num += speed;
        if(num >= 10.0f) {
            num -= 10.0f;
        }
        no = num;
        if(no >= 10) {
            no = 9;
        }
        espBankSet(sprId[1], donkeyRouletteBankTbl[no]);
        if(no != prevNo) {
            mbAudFXPlay(0x3F1);
        }
        prevNo = no;
        HuPrcVSleep();
    }
    mbAudFXPlay(0x46E);
    for(i = 0; i <= 60; i++) {
        t = i/60.0f;
        t = 1.0+(0.5*HuSin(720.0f*t));
        espScaleSet(sprId[1], t, t);
        espPosSet(sprId[1], 288.0f+(16.0f*t), 240.0f);
        HuPrcVSleep();
    }
    for(i = 0; i <= 30.0f; i++) {
        t = i/30.0f;
        scale = 1.0+(7.0*HuSin(90.0f*t));
        for(j = 0; j < 2; j++) {
            espScaleSet(sprId[j], scale, scale);
            espTPLvlSet(sprId[j], 1.0-HuSin(90.0f*t));
        }
        espPosSet(sprId[1], 288.0f+(16.0f*scale), 240.0f);
        HuPrcVSleep();
    }
    for(j = 0; j < 2; j++) {
        espDispOff(sprId[j]);
    }
    mbObjMotionShiftSet(donkeyObjId, 1, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
    sprintf(buf, "%d", donkeyRouletteBankTbl[no]+1);
    mbWinCreate(2, 0x3E0007, -1);
    mbWinTopInsertMesSet((u32)buf, 0);
    mbWinTopWait();
    memset(mgResultData, 0, sizeof(mgResultData));
    mgResultData[0] = playerNo;
    mgResultData[2] = donkeyRouletteBankTbl[no]+1;
    GWMgCoinBonusSet(0, 0);
    GWMgCoinBonusSet(1, 0);
    GWMgCoinBonusSet(2, 0);
    GWMgCoinBonusSet(3, 0);
    return TRUE;
}

/* @@PART4@@ */
#include "game/board/dice.h"
#include "game/board/effect.h"
#include "game/board/mgcall.h"
#include "game/board/pause.h"
#include "game/board/status.h"
#include "game/board/telop.h"
#include "game/esprite.h"

/* capevent.c; local prototypes as retail calls them here. */

static int ev_CapKoopaStart(CAPWORK *work);
static int ev_CapKoopaCoin(CAPWORK *work);
static int ev_CapKoopaReturn(CAPWORK *work);
static u16 ev_CapKoopaDicePadBtnHook(int playerNo);
static void ev_CapKoopaDiceMotHook(int playerNo);

#define CAPSP_RANDF() (0.0000000037252903f*mbRandMod(0x10000000))

static inline s16 CapSpMgCoinBonusGet(int playerNo)
{
    return GwPlayer[playerNo].mgCoinBonus;
}

static inline int CapSpPlayerTeamGet(int playerNo)
{
    return GwPlayer[playerNo].team;
}

static inline BOOL CapSpTeamGet(void)
{
    return GwSystem.tagF;
}

static void ev_CapDonkeyCoin(CAPWORK *work)
{
    int playerNo;
    int masuId;
    int objId0;
    int objId1;
    int objId2;
    int i;
    int no;
    Mtx mtx;
    HuVecF pos;
    HuVecF eye;
    HuVecF pos2;
    HuVecF masuPos;
    HuVecF ofs;
    HuVecF coinPos;
    HuVecF vel;
    int coinNum[4];
    char mes[16];

    playerNo = work->playerNo;
    masuId = GwPlayer[playerNo].masuId;
    objId0 = work->evWork[0];
    objId1 = work->evWork[1];
    objId2 = work->evWork[2];
    mbStatusDispForceSetAll(TRUE);
    mbev_CapObjPosSet(&work->objWork, objId1, masuId, NULL);
    mbObjDispSet(objId1, TRUE);
    mbObjMotionTimeSet(objId1, 0.0f);
    mbObjMotionSpeedSet(objId1, 1.0f);
    mbMasuPosGet(masuId, &masuPos);
    eye = masuPos;
    pos = eye;
    mbObjPosSetV(objId2, &eye);
    mbObjMotionTimeSet(objId1, mbObjMotionMaxTimeGet(objId1));
    Hu3DMotionCalc(mbObjModelIDGet(objId1));
    HuPrcVSleep();
    Hu3DModelObjMtxGet(mbObjModelIDGet(objId1), "tree_fook", mtx);
    pos.x = mtx[0][3]-80.0f;
    pos.y = mtx[1][3];
    pos.z = 80.0f+mtx[2][3];
    eye.x = mtx[0][3];
    eye.y = mtx[1][3];
    eye.z = mtx[2][3];
    mbObjPosSetV(objId2, &eye);
    mbev_PlayerColMasu(playerNo, GwPlayer[playerNo].masuId, TRUE);
    mbMasuPosGet(masuId, &masuPos);
    PSVECSubtract(&pos, &masuPos, &ofs);
    mbPlayerRotSet(playerNo, 0.0f, 0.0f, 0.0f);
    mbPlayerColSnapPlayerSet(playerNo, FALSE);
    mbev_CapPlayerPosSet(&work->objWork, playerNo, masuId, &ofs);
    mbObjDispSet(objId0, TRUE);
    mbObjRotSet(objId0, 0.0f, -45.0f, 0.0f);
    mbObjMotionSet(objId0, 1, 0x40000001);
    pos2.x = 80.0f+mtx[0][3];
    pos2.y = mtx[1][3];
    pos2.z = mtx[2][3]-80.0f;
    mbObjPosSetV(objId0, &pos2);
    mbMasuPosGet(masuId, &masuPos);
    PSVECSubtract(&pos2, &masuPos, &ofs);
    mbev_CapObjPosSet(&work->objWork, objId0, masuId, &ofs);
    mbCameraEyeSetV(&eye);
    mbCameraMoveObj(objId2, NULL, &capsuleCameraOfs, 1500.0f, -1.0f, 1);
    mbCameraMoveWait();
    if(work->flag.donkeyEndF) {
        mbMusPlay(0, 30, 127, 0);
    }
    mbWipeFadeIn();
    mbPauseDisableSet(FALSE);
    sprintf(mes, "%d", mgResultData[2]);
    mbWinCreate(2, 0x3E0009, -1);
    mbWinTopInsertMesSet((u32)mes, 0);
    mbWinTopWait();
    for(i=0; i<4; i++) {
        coinNum[i] = mgResultData[2]*CapSpMgCoinBonusGet(i);
    }
    do {
        for(i=0; i<4; i++) {
            if(coinNum[i] > 0) {
                mbPlayerPosGet(i, &ofs);
                coinPos = ofs;
                coinPos.y += 1500.0f;
                coinPos.x += 0.5f*(100.0f*(-0.5f+CAPSP_RANDF()));
                vel.x = vel.y = vel.z = 0.0f;
                no = mbev_CapEffCoinAdd(work->effCoin, &coinPos, &vel, 0.75f, 4.9f, 30, 4);
                if(no >= 0) {
                    mbev_CapEffCoinMaxYSet(work->effCoin, no, 150.0f+ofs.y);
                    coinNum[i]--;
                }
            }
        }
        for(i=0; i<4; i++) {
            if(coinNum[i] > 0) {
                break;
            }
        }
        HuPrcVSleep();
    } while(i < 4 || mbev_CapEffCoinNumGet(work->effCoin) > 0);
    for(i=0; i<4; i++) {
        coinNum[i] = mgResultData[2]*CapSpMgCoinBonusGet(i);
    }
    mbCoinAddAllProcExecV(coinNum, coinNum, FALSE);
    if(CapSpMgCoinBonusGet(playerNo) > 0) {
        mbPlayerWinLoseVoicePlay(playerNo, 12, 0x243);
        mbev_CapPlayerMotShiftWait(playerNo, 12, 0, 1);
    } else {
        mbPlayerWinLoseVoicePlay(playerNo, 13, 0x249);
        mbev_CapPlayerMotShiftWait(playerNo, 13, 0, 1);
    }
    mbev_CapPlayerMotShiftWait(playerNo, 1, 0x40000001, 1);
}

static void ev_CapDonkeyReturn(CAPWORK *work)
{
    float t;
    float scale;
    int playerNo;
    int masuId;
    int objId0;
    int objId1;
    int objId2;
    int i;
    HuVecF dustPos;
    HuVecF playerPos;
    HuVecF pos;
    HuVecF objPos;
    Mtx mtx;

    playerNo = work->playerNo;
    masuId = GwPlayer[playerNo].masuId;
    objId0 = work->evWork[0];
    objId1 = work->evWork[1];
    objId2 = work->evWork[2];
    mbPlayerRotateStart(playerNo, 135, 15);
    while(!mbPlayerRotateCheck(playerNo)) {
        HuPrcVSleep();
    }
    mbAudFXPlay(0x3A8);
    mbev_CapPlayerMotShiftSet(objId0, 11, 0, 1);
    mbWinCreate(2, 0x3E000A, -1);
    mbWinTopWait();
    mbObjMotionShiftSet(objId0, 1, 0.0f, 8.0f, 0);
    HuPrcSleep(20);
    mbev_CapObjPosSet(&work->objWork, objId0, -1, NULL);
    mbAudFXPlay(0x46F);
    mbObjMotionShiftSet(objId0, 4, 0.0f, 8.0f, 0x40000001);
    for(i=1; i<=30.0f; i++) {
        t = i/30.0f;
        scale = 1.0-(0.7f*HuSin(90.0f*t));
        Hu3DModelObjMtxGet(mbObjModelIDGet(objId1), "tree_fook", mtx);
        pos.x = 80.0f+mtx[0][3];
        pos.y = mtx[1][3]+(100.0*HuSin(90.0f*t))+(2.5*(100.0*HuSin(180.0f*t)));
        pos.z = mtx[2][3]-80.0f;
        mbObjPosSetV(objId0, &pos);
        mbObjScaleSet(objId0, scale, scale, scale);
        HuPrcVSleep();
    }
    mbev_CapEffDustCloudAdd(work->effExplode, pos);
    HuPrcVSleep();
    mbObjDispSet(objId0, FALSE);
    mbev_CapPlayerPosSet(&work->objWork, playerNo, -1, NULL);
    mbPlayerMotionShiftSet(playerNo, 2, 0.0f, 8.0f, 0x40000001);
    for(i=1; i<=30.0f; i++) {
        t = i/30.0f;
        Hu3DModelObjMtxGet(mbObjModelIDGet(objId1), "tree_fook", mtx);
        playerPos.x = mtx[0][3]-(80.0f*(1.0f-t));
        playerPos.y = mtx[1][3];
        playerPos.z = mtx[2][3]+(80.0f*(1.0f-t));
        mbPlayerPosSetV(playerNo, &playerPos);
        HuPrcVSleep();
    }
    mbCameraPlayerViewSet(playerNo, 0);
    mbPlayerRotateStart(playerNo, 0, 15);
    while(!mbPlayerRotateCheck(playerNo)) {
        HuPrcVSleep();
    }
    mbMusBoardFadeOut(0, 0, 1000, 1000, -1, FALSE);
    mbAudFXPlay(0x476);
    mbObjMotionSpeedSet(objId1, -1.0f);
    do {
        t = mbObjMotionTimeGet(objId1)/mbObjMotionMaxTimeGet(objId1);
        if(t < 0.0f) {
            t = 0.0f;
        }
        Hu3DMotionCalc(mbObjModelIDGet(objId1));
        Hu3DModelObjMtxGet(mbObjModelIDGet(objId1), "tree_fook", mtx);
        playerPos.x = mtx[0][3];
        playerPos.y = mtx[1][3];
        playerPos.z = mtx[2][3];
        mbPlayerPosSetV(playerNo, &playerPos);
        objPos.x = mtx[0][3];
        objPos.y = mtx[1][3];
        objPos.z = mtx[2][3];
        mbObjPosSetV(objId2, &objPos);
        HuPrcVSleep();
    } while(t > 0.0f);
    mbMasuPosGet(masuId, &playerPos);
    mbPlayerPosSetV(playerNo, &playerPos);
    mbPlayerColSnapPlayerSet(playerNo, TRUE);
    mbCameraMoveWait();
}

static void ev_CapDonkeyOMExec(OMOBJ *obj)
{
    CAPWORK *work;
    float t;
    float t2;
    float scaleXZ;
    float scaleY;
    int playerNo;
    int masuId;
    int objId0;
    int objId1;
    int objId2;
    HuVecF pos;
    HuVecF masuPos;
    HuVecF ofs;
    Mtx mtx;

    work = obj->data;
    if(mbExitCheck() || obj->work[3]) {
        omDelObjEx(mbObjMan, obj);
        return;
    }
    playerNo = work->playerNo;
    masuId = GwPlayer[playerNo].masuId;
    objId0 = work->evWork[0];
    objId1 = work->evWork[1];
    objId2 = work->evWork[2];
    switch(obj->work[0]) {
        case 0:
            mbObjDispSet(objId0, TRUE);
            mbObjRotSet(objId0, 0.0f, -45.0f, 0.0f);
            mbObjMotionSet(objId0, 4, 0);
            obj->work[0]++;
            obj->work[1] = 0;
        case 1:
            t = (++obj->work[1])/48.0f;
            Hu3DModelObjMtxGet(mbObjModelIDGet(objId1), "tree_fook", mtx);
            pos.x = 100.0f+(80.0f+mtx[0][3]);
            pos.y = mtx[1][3]+(10.0*(100.0*HuCos(90.0f*t)));
            pos.z = (mtx[2][3]-80.0f)-100.0f;
            mbObjPosSetV(objId0, &pos);
            if(t >= 1.0f) {
                mbObjMotionShiftSet(objId0, 2, 0.0f, 8.0f, 0);
                ((void (*)())CharEffectHipDropCreate)(GwPlayer[playerNo].charNo, &pos);
                obj->work[0]++;
                obj->work[1] = 0;
            }
            break;

        case 2:
            t = (++obj->work[1])/36.0f;
            t2 = (obj->work[1]+1)/36.0f;
            if(t2 >= 1.0f) {
                t2 = 1.0f;
            }
            scaleXZ = 1.0+(0.33f*HuSin(180.0f*t2));
            scaleY = 1.0-(0.33f*HuSin(180.0f*t2));
            mbObjScaleSet(objId1, scaleXZ, scaleY, scaleXZ);
            Hu3DModelObjMtxGet(mbObjModelIDGet(objId1), "tree_fook", mtx);
            pos.x = (80.0f+mtx[0][3])+(100.0*HuCos(90.0f*t));
            pos.y = mtx[1][3]+(2.0*(100.0*HuSin(180.0f*t)));
            pos.z = (mtx[2][3]-80.0f)-(100.0*HuCos(90.0f*t));
            mbObjPosSetV(objId0, &pos);
            if(obj->work[1] == 27) {
                mbObjMotionShiftSet(objId0, 3, 0.0f, 8.0f, 0);
            }
            if(t >= 1.0f) {
                CharModelLandDustCreate(GwPlayer[playerNo].charNo, &pos);
                obj->work[0]++;
                obj->work[1] = 0;
            }
            break;

        case 3:
            t2 = (++obj->work[1]+1)/12.0f;
            if(t2 >= 1.0f) {
                t2 = 1.0f;
            }
            scaleXZ = 1.0+(0.1f*HuSin(180.0f*t2));
            scaleY = 1.0-(0.1f*HuSin(180.0f*t2));
            mbObjScaleSet(objId1, scaleXZ, scaleY, scaleXZ);
            Hu3DModelObjMtxGet(mbObjModelIDGet(objId1), "tree_fook", mtx);
            pos.x = 80.0f+mtx[0][3];
            pos.y = mtx[1][3];
            pos.z = mtx[2][3]-80.0f;
            mbObjPosSetV(objId0, &pos);
            if(mbObjMotionShiftIDGet(objId0) == -1 && mbObjMotionEndCheck(objId0) && t2 >= 1.0f) {
                mbObjMotionShiftSet(objId0, 1, 0.0f, 8.0f, 0x40000001);
                mbMasuPosGet(masuId, &masuPos);
                PSVECSubtract(&pos, &masuPos, &ofs);
                mbev_CapObjPosSet(&work->objWork, objId0, masuId, &ofs);
                obj->work[0]++;
                obj->work[1] = 0;
            }
            break;

        default:
            obj->work[2] = TRUE;
            break;
    }
}

void mbev_CapKoopa(void)
{
    CAPWORK *work;
    int objId;
    int i;

    work = HuPrcCurrentGet()->property;
    mbPlayerMotionShiftSet(work->playerNo, 1, 0.0f, 8.0f, 0x40000001);
    mbev_CapWait(work);
    work->effExplode = mbev_CapEffExplodeCreate();
    HuPrcVSleep();
    work->effCoin = mbev_CapEffCoinCreate();
    HuPrcVSleep();
    objId = mbev_CapObjCreate(&work->objWork, 0xE0000, koopaMotTbl, FALSE, 5, 0);
    mbObjDispSet(objId, FALSE);
    mbPlayerColSnapPlayerSet(work->playerNo, TRUE);
    work->evWork[0] = objId;
    if(!work->flag.koopaEndF) {
        if(ev_CapKoopaStart(work)) {
            if((mbPlayerAllComCheck() && !GWMgComDispGet()) || mbMgRouletteNumGet(4) <= 0) {
                for(i=0; i<4; i++) {
                    GWMgCoinBonusSet(i, mbRandMod(2));
                }
                mbWipeFadeOut();
                ev_CapKoopaCoin(work);
            } else {
                mbWinCreate(2, 0x3F000D, 13);
                mbWinTopWait();
                mbObjMotionShiftSet(objId, 5, 0.0f, 8.0f, 0);
                mbev_MgCallKoopa();
            }
        }
    } else {
        ev_CapKoopaCoin(work);
    }
    ev_CapKoopaReturn(work);
    HuPrcEnd();
}

void mbev_CapKoopaKill(void)
{
}

static int ev_CapKoopaStart(CAPWORK *work)
{
    float s;
    float t;
    float scale;
    float tpLvl;
    int playerNo;
    int masuId;
    int objId;
    int sprId;
    int sprId2;
    int gameNo;
    int result;
    int starDispId;
    int coin;
    int squishNum;
    int loseNo;
    int i;
    HuVecF playerPos;
    HuVecF pos;
    HuVecF dicePos;
    int squishPlayer[4];
    char mes[16];
    int motId[2];

    playerNo = work->playerNo;
    masuId = GwPlayer[playerNo].masuId;
    objId = work->evWork[0];
    mbPlayerRotateStart(playerNo, 0, 15);
    motId[0] = mbev_CapPlayerMotionCreate(&work->objWork, playerNo, 0x930017);
    motId[1] = mbev_CapPlayerMotionCreate(&work->objWork, playerNo, 0x930019);
    sprId = mbev_CapSprCreate(&work->objWork, 0xE0024, 100, 0);
    espPosSet(sprId, 288.0f, 240.0f);
    espScaleSet(sprId, 4.0f, 4.0f);
    espTPLvlSet(sprId, 0.0f);
    espColorSet(sprId, 255, 0, 0);
    espDispOff(sprId);
    espAttrSet(sprId, 8);
    espDrawNoSet(sprId, 0);
    espDispOff(sprId);
    mbMusFadeOutSpeed(0, 1000);
    while(mbMusCheck(0)) {
        HuPrcVSleep();
    }
    while(!mbPlayerRotateCheck(playerNo)) {
        HuPrcVSleep();
    }
    mbPlayerMotionShiftSet(playerNo, motId[0], 0.0f, 8.0f, 0);
    mbCameraPlayerViewSet(playerNo, 0);
    mbEffFadeCreate(30, 160);
    work->flag.koopaMusF = TRUE;
    mbMusPlay(0, 27, 127, 0);
    espDispOn(sprId);
    for(i=0; i<360; i+=2) {
        if(i == 60) {
            mbAudFXPlay(0x453);
        }
        if(i == 90 || i == 270 || i == 450) {
            omVibrate(playerNo, 20, 7, 3);
        }
        s = HuSin(mbAngleWrap(i));
        s = __fabsf(s);
        tpLvl = s;
        espTPLvlSet(sprId, tpLvl);
        HuPrcVSleep();
    }
    espDispOff(sprId);
    mbPlayerMotionShiftSet(playerNo, motId[1], 0.0f, 8.0f, 0);
    mbPlayerPosGet(playerNo, &playerPos);
    mbObjDispSet(objId, TRUE);
    mbObjMotionSet(objId, 2, 0);
    mbObjMotionTimeSet(objId, 30.0f);
    mbObjMotionSpeedSet(objId, 1.0f);
    while(mbObjMotionTimeGet(objId) < 60.0f) {
        t = (mbObjMotionTimeGet(objId)-30.0f)/30.0f;
        mbMasuPosGet(masuId, &pos);
        pos.y += 6.0*(100.0*HuCos(90.0f*t));
        mbObjPosSetV(objId, &pos);
        HuPrcVSleep();
    }
    mbev_CapEffDustHeavyAdd(work->effExplode, pos);
    mbev_CapObjPosSet(&work->objWork, objId, masuId, NULL);
    mbAudFXPlay(0x45F);
    mbev_CapVibrate(1);
    squishNum = mbev_CapPlayerSquishSet(squishPlayer, masuId);
    work->evWork[1] = squishNum;
    for(i=0; i<4; i++) {
        work->evWork[i+2] = squishPlayer[i];
    }
    while(!mbObjMotionEndCheck(objId)) {
        HuPrcVSleep();
    }
    HuPrcVSleep();
    mbObjMotionShiftSet(objId, 1, 0.0f, 8.0f, 0x40000001);
    HuPrcSleep(30);
    mbEffFadeOutSet(30);
    HuPrcSleep(30);
    if(CAPSP_RANDF() < 0.3f) {
        gameNo = 0;
    } else {
        gameNo = 1;
    }
    HuDataDirClose(0xE0000);
    mbAudFXDelaySet(30);
    mbAudFXPlay(0x3CB);
    mbev_CapPlayerMotShiftSet(objId, 3, 0, 1);
    mbObjMotionShiftSet(objId, 1, 0.0f, 8.0f, 0x40000001);
    mbWinCreate(2, 0x3F0000, 13);
    mbWinTopWait();
    if(mbPlayerCoinGet(playerNo) <= 0) {
        mbObjMotionShiftSet(objId, 6, 0.0f, 8.0f, 0x40000001);
        mbWinCreate(2, 0x3F0001, 13);
        mbWinTopWait();
        mbev_CapPlayerMotShiftSet(objId, 1, 0x40000001, 1);
        work->flag.koopaNoCoinF = TRUE;
        return FALSE;
    }
    mbAudFXDelaySet(30);
    mbAudFXPlay(0x3CD);
    mbObjMotionShiftSet(objId, 5, 0.0f, 8.0f, 0);
    HuPrcSleep(30);
    mbAudFXPlay(0x45D);
    sprId2 = mbev_CapSprCreate(&work->objWork, mbBoardDataNumGet(koopaMgFile[gameNo]), 100, gameNo^1);
    for(i=1; i<60.0f; i++) {
        t = i/60.0f;
        scale = HuSin(90.0f*t);
        espPosSet(sprId2, 288.0f, (240.0-(250.0*HuSin(180.0f*t)))+(50.0*HuCos(180.0f*t)));
        espScaleSet(sprId2, scale, scale);
        espZRotSet(sprId2, 3.0f*(360.0f*t));
        HuPrcVSleep();
    }
    for(i=1; i<60.0f; i++) {
        t = i/60.0f;
        scale = 1.0+(0.2f*HuSin(720.0f*t));
        espPosSet(sprId2, 288.0f, 240.0f);
        espScaleSet(sprId2, scale, scale);
        espZRotSet(sprId2, 0.0f);
        HuPrcVSleep();
    }
    for(i=1; i<18.0f; i++) {
        t = i/18.0f;
        scale = 1.0+(5.0*HuSin(90.0f*t));
        espPosSet(sprId2, 288.0f, 240.0f);
        espScaleSet(sprId2, scale, scale);
        espTPLvlSet(sprId2, 1.0-HuSin(90.0f*t));
        HuPrcVSleep();
    }
    espDispOff(sprId2);
    if(gameNo == 0) {
        mbAudFXDelaySet(30);
        mbAudFXPlay(0x3CB);
        mbObjMotionShiftSet(objId, 3, 0.0f, 8.0f, 0x40000001);
        mbWinCreate(2, 0x3F0002, 13);
        mbWinTopWait();
        mbObjMotionShiftSet(objId, 1, 0.0f, 8.0f, 0x40000001);
        mbWinCreate(2, 0x3F0003, 13);
        mbWinTopWait();
        mbObjPosGet(objId, &pos);
        dicePos.x = pos.x;
        dicePos.y = pos.y += 50.0f;
        dicePos.z = pos.z;
        diceHitTimer = 60.0f*(0.33f+CAPSP_RANDF());
        koopaMdlId = objId;
        if(mbBoardNoGet() != 3) {
            result = mbRandMod(5);
        } else {
            result = mbRandMod(4);
        }
        mbDiceExec(-1, 12, koopaDiceTbl, result, FALSE, FALSE, &dicePos, 2);
        mbDicePadBtnHookSet(-1, ev_CapKoopaDicePadBtnHook);
        mbDiceMotHookSet(-1, ev_CapKoopaDiceMotHook);
        while(!mbDiceKillCheck(-1)) {
            HuPrcVSleep();
        }
        result = mbDiceResultGet(-1);
        HuPrcSleep(30);
        if(koopaDiceResultTbl[result] >= 0) {
            sprintf(mes, "%d", -koopaDiceResultTbl[result]);
            mbWinCreate(2, 0x3F0004, 13);
            mbWinTopInsertMesSet((u32)mes, 0);
            mbWinTopWait();
            mbDiceFadeSet(-1);
            mbAudFXDelaySet(30);
            mbAudFXPlay(0x3CD);
            mbev_CapPlayerMotShiftSet(objId, 5, 0, 1);
            if((coin = koopaDiceResultTbl[result]) > mbPlayerCoinGet(playerNo)) {
                coin = mbPlayerCoinGet(playerNo);
            }
            mbCoinAddProcExec(playerNo, -coin, -coin, FALSE);
            mbev_CapPlayerMotShiftSet(objId, 1, 0x40000001, 1);
        } else {
            mbWinCreate(2, 0x3F0005, 13);
            mbWinTopWait();
            mbDiceFadeSet(-1);
            if(mbPlayerStarGet(playerNo) > 0) {
                mbAudFXDelaySet(30);
                mbAudFXPlay(0x3CD);
                mbev_CapPlayerMotShiftSet(objId, 5, 0, 1);
                HuPrcSleep(12);
                starDispId = mbStarDispPlayerCreate(playerNo, -1);
                while(!mbStarDispCheck(starDispId)) {
                    HuPrcVSleep();
                }
                mbPlayerStarAdd(playerNo, -1);
                mbev_CapPlayerMotShiftSet(objId, 1, 0x40000001, 1);
            } else {
                mbAudFXDelaySet(30);
                mbAudFXPlay(0x3CD);
                mbObjMotionShiftSet(objId, 6, 0.0f, 8.0f, 0x40000001);
                mbWinCreate(2, 0x3F0006, 13);
                mbWinTopWait();
                mbev_CapPlayerMotShiftSet(objId, 1, 0x40000001, 1);
            }
        }
        return FALSE;
    } else {
        mbAudFXDelaySet(30);
        mbAudFXPlay(0x3CB);
        mbObjMotionShiftSet(objId, 3, 0.0f, 8.0f, 0x40000001);
        mbWinCreate(2, 0x3F0007, 13);
        mbWinTopWait();
        loseNo = mbRandMod(3);
        mbAudFXDelaySet(30);
        mbAudFXPlay(0x3CD);
        mbObjMotionShiftSet(objId, 5, 0.0f, 8.0f, 0);
        if(!CapSpTeamGet()) {
            mbWinCreate(2, 0x3F0008, 13);
        } else {
            mbWinCreate(2, 0x3F0009, 13);
        }
        mbWinTopInsertMesSet(koopaLoseMesTbl[loseNo], 0);
        mbWinTopWait();
        mbev_CapPlayerMotShiftSet(objId, 1, 0x40000001, 1);
        memset(mgResultData, 0, sizeof(mgResultData));
        mgResultData[4] = loseNo;
        GWMgCoinBonusSet(0, 0);
        GWMgCoinBonusSet(1, 0);
        GWMgCoinBonusSet(2, 0);
        GWMgCoinBonusSet(3, 0);
        return TRUE;
    }
}

static int ev_CapKoopaCoin(CAPWORK *work)
{
    int playerNo;
    int masuId;
    int objId;
    int squishNum;
    int loseNo;
    int loseNum;
    int noneF;
    int removeF;
    int i;
    int j;
    int teamPlayer[2][4];
    HuVecF masuPos;
    int coinNum[4];
    int squishPlayer[4];
    int teamNum[2];
    int teamLoseF[2];

    playerNo = work->playerNo;
    masuId = GwPlayer[playerNo].masuId;
    objId = work->evWork[0];
    mbev_PlayerColMasu(playerNo, masuId, TRUE);
    squishNum = mbev_CapPlayerSquishVoiceSet(squishPlayer, masuId, TRUE);
    work->evWork[1] = squishNum;
    for(i=0; i<4; i++) {
        work->evWork[i+2] = squishPlayer[i];
    }
    mbMasuPosGet(masuId, &masuPos);
    mbObjPosSetV(objId, &masuPos);
    mbObjDispSet(objId, TRUE);
    mbObjMotionSet(objId, 1, 0x40000001);
    mbev_CapObjPosSet(&work->objWork, objId, masuId, NULL);
    mbStatusDispForceSetAll(TRUE);
    mbCameraEyeSetV(&masuPos);
    mbCameraPlayerViewSetFast(playerNo, 0);
    mbCameraMoveWait();
    if(work->flag.koopaEndF) {
        mbMusPlay(0, 27, 127, 0);
    }
    mbWipeFadeIn();
    mbPauseDisableSet(FALSE);
    loseNo = mgResultData[4];
    if(!CapSpTeamGet()) {
        for(i=0, loseNum=0; i<4; i++) {
            if(CapSpMgCoinBonusGet(i) <= 0) {
                loseNum++;
            }
        }
        if(loseNo == 2) {
            for(i=0, noneF=TRUE; i<4; i++) {
                if(CapSpMgCoinBonusGet(i) <= 0 && mbPlayerCapsuleNumGet(i) > 0) {
                    noneF = FALSE;
                }
            }
        } else {
            for(i=0, noneF=TRUE; i<4; i++) {
                if(CapSpMgCoinBonusGet(i) <= 0 && mbPlayerCoinGet(i) > 0) {
                    noneF = FALSE;
                }
            }
        }
    } else {
        teamNum[0] = teamNum[1] = 0;
        for(i=0; i<4; i++) {
            if(CapSpPlayerTeamGet(i) == 0) {
                teamPlayer[0][teamNum[0]] = i;
                teamNum[0]++;
            } else {
                teamPlayer[1][teamNum[1]] = i;
                teamNum[1]++;
            }
        }
        teamLoseF[0] = teamLoseF[1] = loseNum = 0;
        if(CapSpMgCoinBonusGet(teamPlayer[0][0]) <= 0 && CapSpMgCoinBonusGet(teamPlayer[0][1]) <= 0) {
            teamLoseF[0] = TRUE;
            loseNum++;
        }
        if(CapSpMgCoinBonusGet(teamPlayer[1][0]) <= 0 && CapSpMgCoinBonusGet(teamPlayer[1][1]) <= 0) {
            teamLoseF[1] = TRUE;
            loseNum++;
        }
        if(loseNo == 2) {
            for(i=0, noneF=TRUE; i<2; i++) {
                if(teamLoseF[i] && mbPlayerCapsuleNumGet(teamPlayer[i][0]) > 0) {
                    noneF = FALSE;
                }
            }
        } else {
            for(i=0, noneF=TRUE; i<2; i++) {
                if(teamLoseF[i] && mbPlayerCoinGet(teamPlayer[i][0]) > 0) {
                    noneF = FALSE;
                }
            }
        }
    }
    mbAudFXDelaySet(30);
    mbAudFXPlay(0x3CB);
    mbObjMotionShiftSet(objId, 3, 0.0f, 8.0f, 0x40000001);
    if(!CapSpTeamGet()) {
        mbWinCreate(2, 0x3F000E, 13);
    } else {
        mbWinCreate(2, 0x3F000F, 13);
    }
    mbWinTopInsertMesSet(koopaLoseMesTbl2[loseNo], 0);
    mbWinTopWait();
    mbAudFXDelaySet(30);
    mbAudFXPlay(0x3CD);
    mbev_CapPlayerMotShiftSet(objId, 5, 0, 1);
    mbAudFXPlay(0x427);
    if(loseNum == 0) {
        HuPrcSleep(30);
        mbAudFXPlay(0x3CC);
        mbObjMotionShiftSet(objId, 6, 0.0f, 8.0f, 0x40000001);
        if(!CapSpTeamGet()) {
            mbWinCreate(2, 0x3F0012, 13);
        } else {
            mbWinCreate(2, 0x3F0013, 13);
        }
        mbWinTopWait();
    } else if(noneF) {
        HuPrcSleep(30);
        mbAudFXPlay(0x3CC);
        mbObjMotionShiftSet(objId, 6, 0.0f, 8.0f, 0x40000001);
        if(loseNo == 2) {
            mbWinCreate(2, 0x3F0011, 13);
        } else {
            mbWinCreate(2, 0x3F0010, 13);
        }
        mbWinTopWait();
    } else if(!CapSpTeamGet()) {
        switch(loseNo) {
            case 0:
                for(i=0; i<4; i++) {
                    if(CapSpMgCoinBonusGet(i) <= 0) {
                        coinNum[i] = -((mbPlayerCoinGet(i)+1)/2);
                        omVibrate(i, 20, 20, 0);
                    } else {
                        coinNum[i] = 0;
                    }
                }
                mbCoinAddAllProcExecV(coinNum, coinNum, FALSE);
                break;

            case 1:
                for(i=0; i<4; i++) {
                    if(CapSpMgCoinBonusGet(i) <= 0) {
                        coinNum[i] = -mbPlayerCoinGet(i);
                        omVibrate(i, 20, 20, 0);
                    } else {
                        coinNum[i] = 0;
                    }
                }
                mbCoinAddAllProcExecV(coinNum, coinNum, FALSE);
                break;

            default:
                for(i=0; i<4; i++) {
                    if(CapSpMgCoinBonusGet(i) <= 0) {
                        omVibrate(i, 20, 20, 0);
                    }
                }
                for(i=0; i<mbPlayerCapsuleMaxGet(); i++) {
                    for(j=0, removeF=FALSE; j<4; j++) {
                        if(CapSpMgCoinBonusGet(j) <= 0) {
                            mbPlayerCapsuleRemove(j, 0);
                            removeF = TRUE;
                        }
                    }
                    if(removeF) {
                        HuPrcSleep(10);
                    }
                }
                break;
        }
    } else {
        switch(loseNo) {
            case 0:
                for(i=0; i<2; i++) {
                    if(teamLoseF[i]) {
                        mbCoinAddDispExec(teamPlayer[i][0], -(mbPlayerCoinGet(teamPlayer[i][0])/2), FALSE, FALSE);
                        omVibrate(teamPlayer[i][0], 20, 20, 0);
                        omVibrate(teamPlayer[i][1], 20, 20, 0);
                    }
                }
                break;

            case 1:
                for(i=0; i<2; i++) {
                    if(teamLoseF[i]) {
                        mbCoinAddDispExec(teamPlayer[i][0], -mbPlayerCoinGet(teamPlayer[i][0]), FALSE, FALSE);
                        omVibrate(teamPlayer[i][0], 20, 20, 0);
                        omVibrate(teamPlayer[i][1], 20, 20, 0);
                    }
                }
                break;

            default:
                for(i=0; i<2; i++) {
                    if(teamLoseF[i]) {
                        omVibrate(teamPlayer[i][0], 20, 20, 0);
                        omVibrate(teamPlayer[i][1], 20, 20, 0);
                    }
                }
                for(i=0; i<mbPlayerCapsuleMaxGet(); i++) {
                    for(j=0, removeF=FALSE; j<2; j++) {
                        if(teamLoseF[j]) {
                            mbPlayerCapsuleRemove(teamPlayer[j][0], 0);
                            removeF = TRUE;
                        }
                    }
                    if(removeF) {
                        HuPrcSleep(10);
                    }
                }
                break;
        }
    }
    mbev_CapPlayerMotShiftSet(objId, 1, 0x40000001, 1);
    return 0;
}

static int ev_CapKoopaReturn(CAPWORK *work)
{
    float t;
    int playerNo;
    int masuId;
    int objId;
    int squishNum;
    int i;
    HuVecF pos;
    int squishPlayer[4];

    playerNo = work->playerNo;
    masuId = GwPlayer[playerNo].masuId;
    objId = work->evWork[0];
    squishNum = work->evWork[1];
    for(i=0; i<4; i++) {
        squishPlayer[i] = work->evWork[i+2];
    }
    mbAudFXDelaySet(30);
    mbAudFXPlay(0x3CB);
    mbev_CapPlayerMotShiftSet(objId, 3, 0, 1);
    mbObjMotionShiftSet(objId, 1, 0.0f, 8.0f, 0x40000001);
    mbWinCreate(2, 0x3F0014, 13);
    mbWinTopWait();
    mbMusFadeOutSpeed(0, 1000);
    while(mbMusCheck(0)) {
        HuPrcVSleep();
    }
    mbAudFXDelaySet(30);
    mbAudFXPlay(0x3CE);
    mbObjMotionShiftSet(objId, 2, 0.0f, 8.0f, 0);
    while(mbObjMotionShiftIDGet(objId) != -1 || mbObjMotionTimeGet(objId) <= 25.0f) {
        HuPrcVSleep();
    }
    mbev_CapObjPosSet(&work->objWork, objId, -1, NULL);
    for(i=1; i<=24.0f; i++) {
        t = i/24.0f;
        mbMasuPosGet(masuId, &pos);
        pos.y += 6.0*(100.0*HuSin(90.0f*t));
        mbObjPosSetV(objId, &pos);
        HuPrcVSleep();
    }
    mbObjDispSet(objId, FALSE);
    mbMusBoardPlay();
    mbev_CapPlayerStunSet(squishPlayer, squishNum, 0);
    HuPrcSleep(60);
    for(i=0; i<squishNum; i++) {
        mbPlayerMotionShiftSet(squishPlayer[i], 1, 0.0f, 8.0f, 0x40000001);
    }
    if(work->flag.koopaNoCoinF) {
        mbCoinAddProcExec(playerNo, 10, 1, FALSE);
    }
    return 0;
}

static u16 ev_CapKoopaDicePadBtnHook(int playerNo)
{
    if(--diceHitTimer <= 0) {
        return PAD_BUTTON_A;
    } else {
        return 0;
    }
}

static void ev_CapKoopaDiceMotHook(int playerNo)
{
    int i;

    if(koopaMdlId != -1) {
        mbObjMotionSet(koopaMdlId, 4, 0);
        i = 0;
        do {
            if(i++ == 27) {
                mbDiceObjHit(-1);
            }
            HuPrcVSleep();
        } while(!mbObjMotionEndCheck(koopaMdlId));
        mbObjMotionSet(koopaMdlId, 1, 0x40000001);
    }
}

/* @@END@@ */
