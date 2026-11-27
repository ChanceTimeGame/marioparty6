/* @@HEAD@@ */
#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mgdata.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/sprite.h"
#include "game/window.h"
#include "game/wipe.h"
#include "humath.h"
#include "math.h"
#include "dolphin.h"

#include "datadir_enum.h"

/* The esp* sprite calls (game/esprite.h) and CharFXPlay/CharFXStop are called
 * unprototyped: their headers stay out of this unit. */

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* lbl_1_bss_488: one result row per player (0x58 each) */
typedef struct ResultPlayer_s {
    s32 rankEspId; /* 0x00 rank before the minigame */
    s32 rankNewEspId; /* 0x04 rank after the minigame */
    s32 charEspId; /* 0x08 bank = rank */
    s32 coinAddEspId; /* 0x0C */
    s32 coinAddNumEspId[3]; /* 0x10 */
    s32 starNumEspId[4]; /* 0x1C */
    s32 coinNumEspId[4]; /* 0x2C */
    s32 charMdlId; /* 0x3C */
    s32 charMotId[3]; /* 0x40 */
    s32 crownMdlId; /* 0x4C */
    s32 starMdlId; /* 0x50 */
    s32 coinMdlId; /* 0x54 */
} RESULT_PLAYER; /* size 0x58 */

void ObjectSetup(void);
void fn_1_560(void);
void fn_1_940(void);
void fn_1_1074(void);
void fn_1_1E8C(void);
void fn_1_2068(void);
void fn_1_2878(void);
void fn_1_2EBC(void);
void fn_1_31F0(s32 playerNo, s32 num);
void fn_1_333C(s32 playerNo, s32 num);
void fn_1_3488(s32 playerNo, s32 num);
s32 fn_1_356C(void);
void fn_1_3640(s32 statId);
void fn_1_3698(void);
void fn_1_3FB0(s16 *coinNum);
s32 fn_1_45D0(s32 playerNo);
void fn_1_4694(void);
void fn_1_47A8(void);
void fn_1_49C4(void);
void fn_1_7A18(void);
s32 fn_1_C45C(s32 dataNum);

/* .bss of the first source file (0x0-0x634), laid out in reverse definition order */
OMOBJ *lbl_1_bss_630;
OMOBJMAN *lbl_1_bss_62C;
s16 lbl_1_bss_628;
s32 lbl_1_bss_624;
s32 lbl_1_bss_620;
s16 lbl_1_bss_61C;
float lbl_1_bss_60C[4];
float lbl_1_bss_5FC[4];
s16 lbl_1_bss_5F4[4];
s16 lbl_1_bss_5EC[4];
s32 lbl_1_bss_5E8;
RESULT_PLAYER lbl_1_bss_488[4];
s32 lbl_1_bss_320[9][10];
float lbl_1_bss_1B8[9][10];
float lbl_1_bss_50[9][10];
s32 lbl_1_bss_40[4];
s32 lbl_1_bss_30[4];
s32 lbl_1_bss_2C;
s32 lbl_1_bss_28;
float lbl_1_bss_24;
float lbl_1_bss_20;
s16 lbl_1_bss_1C;
float lbl_1_bss_18;
float lbl_1_bss_14;
float lbl_1_bss_10;
float lbl_1_bss_C;
s32 lbl_1_bss_8;
float lbl_1_bss_4;
s32 lbl_1_bss_0;

OM_CAMERA_VIEW lbl_1_data_0 = { { 0, 200, 0 }, { 0, 0, 0 }, 1600 };
OM_CAMERA_VIEW lbl_1_data_1C = { { 0, 200, 0 }, { 0, 0, 0 }, 1600 };
OM_CAMERA_VIEW lbl_1_data_38 = { { 0, 200, 0 }, { 0, 0, 0 }, 1600 };
float lbl_1_data_54[8] = { 169, 171, 407, 171, 169, 334, 407, 334 };
float lbl_1_data_74[8] = { 229, 141, 467, 141, 229, 304, 467, 304 };
float lbl_1_data_94[8] = { 204, 191, 442, 191, 204, 354, 442, 354 };
float lbl_1_data_B4[8] = { 204, 221, 442, 221, 204, 384, 442, 384 };
float lbl_1_data_D4[8] = { 99, 123, 337, 123, 99, 286, 337, 286 };
float lbl_1_data_F4[8] = { 59, 91, 297, 91, 59, 254, 297, 254 };
HuVecF lbl_1_data_114[4] = { { 119, 221, 9000 }, { 357, 221, 9000 }, { 119, 384, 9000 }, { 357, 384, 9000 } };
HuVecF lbl_1_data_144[4] = { { 219, 106, 9000 }, { 457, 106, 9000 }, { 219, 269, 9000 }, { 457, 269, 9000 } };
HuVecF lbl_1_data_174[4] = { { 179, 191, 9000 }, { 417, 191, 9000 }, { 179, 354, 9000 }, { 417, 354, 9000 } };
HuVecF lbl_1_data_1A4[4] = { { 179, 221, 9000 }, { 417, 221, 9000 }, { 179, 384, 9000 }, { 417, 384, 9000 } };
HuVecF lbl_1_data_1D4[4] = { { 1, 1, 1 }, { 0.75f, 0.75f, 0.75f }, { 0.5f, 0.5f, 0.5f }, { 0.3f, 0.3f, 0.3f } };
float lbl_1_data_204[4] = { 1, 0.98f, 0.96f, 0.94f };

/* @@PART1@@ */
int _prolog(void)
{
    const VoidFunc *ctors = _ctors;
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtors = _dtors;
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    s32 i;
    HU3D_LIGHTID lightId;
    GXColor unusedColor = { 20 };

    OSReport("******* RESULT ObjectSetup *********\n");
    lbl_1_bss_62C = omInitObjMan(50, 8192);
    omSystemKeyCheckSetup(lbl_1_bss_62C);
    omSysPauseEnable(FALSE);
    HuWinInit(0);
    lbl_1_bss_628 = GwSystem.mgNo;
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        GwPlayerConf[i].grpNo = GwPlayer[i].team;
    }
    HuDataDirClose(MgDataTbl[lbl_1_bss_628].dataDir);
    if (!GWPartyGet()) {
        omOvlReturnEx(1, 1);
        return;
    }
    switch (MgDataTbl[GwSystem.mgNo].type) {
        case MG_TYPE_KOOPA:
        case MG_TYPE_KETTOU:
        case MG_TYPE_DONKEY:
            omOvlReturnEx(1, 1);
            return;
    }
    if (lbl_1_bss_628 <= -1) {
        lbl_1_bss_628 = 0;
    }
    Hu3DCameraCreate(HU3D_CAM0 | HU3D_CAM1);
    Hu3DCameraPerspectiveSet(HU3D_CAM0, 5.0f, 1000.0f, 30000.0f, 1.2f);
    Hu3DCameraViewportSet(HU3D_CAM0, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(HU3D_CAM0, 0, 0, 640, 480);
    Hu3DCameraPerspectiveSet(HU3D_CAM1, 5.0f, 10000.0f, 30000.0f, 1.2f);
    Hu3DCameraViewportSet(HU3D_CAM1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(HU3D_CAM1, 0, 0, 640, 480);
    omCameraViewSetMulti(HU3D_CAM0, &lbl_1_data_0);
    omCameraViewSetMulti(HU3D_CAM1, &lbl_1_data_1C);
    lbl_1_bss_630 = omAddObjEx(lbl_1_bss_62C, 32730, 100, 50, -1, omOutViewMulti);
    omOutViewMultiCntSet(lbl_1_bss_630, 2);
    lightId = Hu3DGLightCreate(0.0f, 100.0f, 1000.0f, 0.0f, -0.5f, -1.0f, 255, 255, 255);
    Hu3DGLightInfinitytSet(lightId);
    lbl_1_bss_624 = 0;
    if (MgDataTbl[GwSystem.mgNo].type != MG_TYPE_BATTLE) {
        if (GwSystem.tagF) {
            HuPrcChildCreate(fn_1_49C4, 100, 0x3000, 0, (HUPROCESS *)lbl_1_bss_62C);
        } else {
            HuPrcChildCreate(fn_1_560, 100, 0x3000, 0, (HUPROCESS *)lbl_1_bss_62C);
        }
    } else {
        HuPrcChildCreate(fn_1_7A18, 100, 0x3000, 0, (HUPROCESS *)lbl_1_bss_62C);
    }
    HuWinInit(1);
    lbl_1_bss_61C = frandmod(4);
}

void fn_1_560(void)
{
    s16 i;
    s16 comNum;
    HUWINID winId;
    s16 statId;
    s32 doneF;
    HUPROCESS *proc;

    doneF = 0;
    proc = HuPrcCurrentGet();
    fn_1_47A8();
    for (i = comNum = 0; i < 4; i++) {
        if (GwPlayerConf[i].type != 0) {
            comNum++;
        }
    }
    if (comNum == 4) {
        lbl_1_bss_0 = 1;
    } else {
        lbl_1_bss_0 = 0;
    }
    lbl_1_bss_620 = 1;
    lbl_1_bss_8 = 0;
    for (i = 0; i < 4; i++) {
        lbl_1_bss_60C[i] = 1.0f;
        lbl_1_bss_5FC[i] = 1.0f;
    }
    fn_1_1074();
    HuPrcChildCreate(fn_1_940, 100, 0x2000, 0, (HUPROCESS *)lbl_1_bss_62C);
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 20);
    HuAudSStreamPlay(70);
    while (WipeCheck()) {
        HuPrcVSleep();
    }
    HuDataDirClose(fn_1_C45C(DATANUM(DATA_result, 0)));
    HuPrcSleep(10);
    if (!lbl_1_bss_624) {
        HuPrcSleep(20);
    }
    fn_1_2068();
    for (i = 0; i < 4; i++) {
        lbl_1_bss_5F4[i] = fn_1_45D0(i);
    }
    fn_1_3698();
    for (i = 0; i < 4; i++) {
        lbl_1_bss_5EC[i] = fn_1_45D0(i);
    }
    fn_1_2878();
    lbl_1_bss_8 = 1;
    winId = HuWinCreate(-10000.0f, 400.0f, 320, 40, 0);
    HuWinMesSpeedSet(winId, 0);
    HuWinBGTPLvlSet(winId, 0.0f);
    HuWinPriSet(winId, 5);
    HuWinAttrSet(winId, 0x800);
    if (!lbl_1_bss_0) {
        HuWinMesSet(winId, 0x10000);
    }
    HuPrcSleep(4);
    statId = fn_1_356C();
    while (1) {
        if (lbl_1_bss_0) {
            doneF = 1;
        } else {
            for (i = 0; i < 4; i++) {
                if (!HuPadStatGet(i) && (HuPadBtnDown[i] & PAD_BUTTON_A)) {
                    HuAudFXPlay(2);
                    doneF = 1;
                    break;
                }
            }
        }
        if (doneF) {
            break;
        }
        HuPrcVSleep();
    }
    HuWinKill(winId);
    fn_1_3640(statId);
    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
    HuAudSStreamAllFadeOut(1000);
    while (WipeCheck()) {
        HuPrcVSleep();
    }
    lbl_1_bss_620 = 0;
    lbl_1_bss_8 = 0;
    fn_1_1E8C();
    HuDataDirClose(fn_1_C45C(DATANUM(DATA_result, 0)));
    fn_1_4694();
    omSysPauseEnable(TRUE);
    omOvlReturnEx(1, 1);
    while (1) {
        HuPrcVSleep();
    }
}

float lbl_1_data_23C = 1.0f;

void fn_1_940(void)
{
    float unused;
    float coinRot[4];
    float starRot[4];
    s16 i;
    s16 j;

    unused = 0.0f;
    for (i = 0; i < 4; i++) {
        starRot[i] = 0.0f;
        coinRot[i] = 0.0f;
    }
    while (lbl_1_bss_620) {
        if (lbl_1_bss_0) {
            lbl_1_bss_624 = 1;
        } else {
            lbl_1_bss_624 = 0;
            for (i = 0; i < 4; i++) {
                if (!HuPadStatGet(i) && (HuPadBtn[i] & PAD_BUTTON_A)) {
                    lbl_1_bss_624 = 1;
                }
            }
        }
        for (i = 0; i < 4; i++) {
            starRot[i] += 1.2f * lbl_1_bss_5FC[i];
            if (starRot[i] >= 360.0f) {
                starRot[i] -= 360.0f;
            }
            Hu3DModelRotSet(lbl_1_bss_488[i].starMdlId, 0.0f, starRot[i], 0.0f);
            coinRot[i] += lbl_1_bss_60C[i];
            if (coinRot[i] >= 360.0f) {
                coinRot[i] -= 360.0f;
            }
            Hu3DModelRotSet(lbl_1_bss_488[i].coinMdlId, 0.0f, coinRot[i], 0.0f);
        }
        for (i = 0; i < 9; i++) {
            for (j = 0; j < 10; j++) {
                lbl_1_bss_1B8[i][j] += 1.0f;
                lbl_1_bss_50[i][j] += 1.0f;
                if (lbl_1_bss_1B8[i][j] > 616.0f) {
                    lbl_1_bss_1B8[i][j] = -80.0f;
                }
                if (lbl_1_bss_50[i][j] > 520.0f) {
                    lbl_1_bss_50[i][j] = -80.0f;
                }
                espPosSet(lbl_1_bss_320[i][j], lbl_1_bss_1B8[i][j], lbl_1_bss_50[i][j]);
            }
        }
        if (lbl_1_bss_8) {
            lbl_1_bss_10 = HuSin(lbl_1_bss_4);
            lbl_1_bss_4 += 5.0f;
            if (lbl_1_bss_4 > 180.0f) {
                lbl_1_bss_4 -= 180.0f;
            }
            lbl_1_bss_C = HuSin(lbl_1_bss_4);
            lbl_1_data_23C += 0.1f * (lbl_1_bss_C - lbl_1_bss_10);
            for (i = 0; i < 4; i++) {
                if (lbl_1_bss_5EC[i] == 0) {
                    espScaleSet(lbl_1_bss_488[i].rankEspId, 0.95f * lbl_1_data_23C, 0.95f * lbl_1_data_23C);
                    espScaleSet(lbl_1_bss_488[i].rankNewEspId, 0.95f * lbl_1_data_23C, 0.95f * lbl_1_data_23C);
                    espScaleSet(lbl_1_bss_488[i].charEspId, lbl_1_data_23C, lbl_1_data_23C);
                }
            }
        }
        HuPrcVSleep();
    }
    HuPrcEnd();
}

s32 lbl_1_data_240[2] = { -80, -80 };

void fn_1_1074(void)
{
    HuVecF pos;
    s32 rankNew[4];
    s32 score[4];
    s32 charNo[4];
    s32 unused;
    s32 i;
    s32 j;
    s32 k;
    s32 rank;
    s32 id;
    s32 rankNum;
    s32 language;
    s16 mgCoin;
    s16 mgCoinBonus;

    language = GWLanguageGet();
    if (language == 1) {
        id = espEntry(fn_1_C45C(DATANUM(DATA_result, 5)), 180, 0);
        lbl_1_bss_5E8 = id;
        espPosSet(id, 288.0f, 64.0f);
        espDrawNoSet(id, 64);
    } else {
        id = espEntry(fn_1_C45C(DATANUM(DATA_result, 7)), 180, 0);
        lbl_1_bss_5E8 = id;
        espPosSet(id, 288.0f, 64.0f);
        espDrawNoSet(id, 64);
    }
    for (i = 0; i < 4; i++) {
        k = i * 2;
        rank = fn_1_45D0(i);
        id = espEntry(fn_1_C45C(DATANUM(DATA_result, 9) + rank), 200, 0);
        lbl_1_bss_488[i].rankEspId = id;
        espPosSet(id, lbl_1_data_54[k], lbl_1_data_54[k + 1]);
        espDrawNoSet(id, 64);
        espScaleSet(id, 0.95f, 0.95f);
        id = espEntry(fn_1_C45C(DATANUM(DATA_result, 2)), 150, rank);
        lbl_1_bss_488[i].charEspId = id;
        espPosSet(id, lbl_1_data_74[k], lbl_1_data_74[k + 1]);
        espBankSet(id, rank);
        espDrawNoSet(id, 64);
        espScaleSet(id, lbl_1_data_204[rank], lbl_1_data_204[rank]);
        id = espEntry(fn_1_C45C(DATANUM(DATA_result, 4)), 130, 0);
        lbl_1_bss_488[i].coinAddEspId = id;
        espPosSet(id, lbl_1_data_D4[k], lbl_1_data_D4[k + 1]);
        espBankSet(id, 0);
        espDrawNoSet(id, 64);
        espScaleSet(id, 0.0f, 0.0f);
        espAttrSet(id, 4);
        for (j = 0; j < 3; j++) {
            if (j == 0) {
                id = espEntry(fn_1_C45C(DATANUM(DATA_result, 0)), 120, 10);
                lbl_1_bss_488[i].coinAddNumEspId[j] = id;
            } else {
                id = espEntry(fn_1_C45C(DATANUM(DATA_result, 0)), 120, 0);
                lbl_1_bss_488[i].coinAddNumEspId[j] = id;
            }
            espPosSet(id, lbl_1_data_F4[k] + j * 17, lbl_1_data_F4[k + 1]);
            espDrawNoSet(id, 64);
            espScaleSet(id, 0.0f, 0.0f);
            espAttrSet(id, 4);
        }
        for (j = 0; j < 4; j++) {
            if (j == 0) {
                id = espEntry(fn_1_C45C(DATANUM(DATA_result, 1)), 120, 10);
                lbl_1_bss_488[i].starNumEspId[j] = id;
            } else {
                id = espEntry(fn_1_C45C(DATANUM(DATA_result, 1)), 120, 0);
                lbl_1_bss_488[i].starNumEspId[j] = id;
            }
            espPosSet(id, lbl_1_data_94[k] + j * 17, lbl_1_data_94[k + 1]);
            espDrawNoSet(id, 64);
        }
        for (j = 0; j < 4; j++) {
            if (j == 0) {
                id = espEntry(fn_1_C45C(DATANUM(DATA_result, 1)), 120, 10);
                lbl_1_bss_488[i].coinNumEspId[j] = id;
            } else {
                id = espEntry(fn_1_C45C(DATANUM(DATA_result, 1)), 120, 0);
                lbl_1_bss_488[i].coinNumEspId[j] = id;
            }
            espPosSet(id, lbl_1_data_B4[k] + j * 17, lbl_1_data_B4[k + 1]);
            espDrawNoSet(id, 64);
        }
        lbl_1_bss_40[i] = GWPlayerCoinGet(i);
        lbl_1_bss_30[i] = GWPlayerStarGet(i);
        fn_1_31F0(i, lbl_1_bss_40[i]);
        fn_1_333C(i, lbl_1_bss_30[i]);
        fn_1_3488(i, 0);
        charNo[i] = GwPlayerConf[i].charNo;
        unused = charNo[i] * 4;
        id = CharModelCreate(charNo[i], 2);
        lbl_1_bss_488[i].charMdlId = id;
        lbl_1_bss_488[i].charMotId[0] = CharMotionCreate(charNo[i], DATANUM(DATA_mariomot, 0x00));
        lbl_1_bss_488[i].charMotId[1] = CharMotionCreate(charNo[i], DATANUM(DATA_mariomot, 0x24));
        lbl_1_bss_488[i].charMotId[2] = CharMotionCreate(charNo[i], DATANUM(DATA_mariomot, 0x25));
        CharMotionDataClose(charNo[i]);
        Hu3DMotionSet(id, lbl_1_bss_488[i].charMotId[0]);
        Hu3DModelAttrSet(id, HU3D_MOTATTR_LOOP);
        Hu3DModelCameraSet(id, HU3D_CAM0);
        Hu3D2Dto3D(&lbl_1_data_114[i], HU3D_CAM0, &pos);
        Hu3DModelPosSetV(id, &pos);
        Hu3DModelScaleSetV(id, &lbl_1_data_1D4[rank]);
        Hu3DModelLayerSet(id, 1);
        id = Hu3DModelCreateData(fn_1_C45C(DATANUM(DATA_result, 22)));
        lbl_1_bss_488[i].crownMdlId = id;
        Hu3DModelCameraSet(id, HU3D_CAM0);
        Hu3D2Dto3D(&lbl_1_data_144[i], HU3D_CAM0, &pos);
        Hu3DModelPosSetV(id, &pos);
        Hu3DModelLayerSet(id, 1);
        Hu3DModelScaleSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrSet(id, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(id, HU3D_MOTATTR_LOOP);
        id = Hu3DModelCreateData(fn_1_C45C(DATANUM(DATA_result, 25)));
        lbl_1_bss_488[i].starMdlId = id;
        Hu3DModelCameraSet(id, HU3D_CAM0);
        Hu3D2Dto3D(&lbl_1_data_174[i], HU3D_CAM0, &pos);
        Hu3DModelScaleSet(id, 0.6f, 0.6f, 0.6f);
        Hu3DModelPosSetV(id, &pos);
        Hu3DModelLayerSet(id, 1);
        id = Hu3DModelCreateData(fn_1_C45C(DATANUM(DATA_result, 26)));
        lbl_1_bss_488[i].coinMdlId = id;
        Hu3DModelCameraSet(id, HU3D_CAM0);
        Hu3D2Dto3D(&lbl_1_data_1A4[i], HU3D_CAM0, &pos);
        Hu3DModelScaleSet(id, 0.5f, 0.5f, 0.5f);
        Hu3DModelPosSetV(id, &pos);
        Hu3DModelLayerSet(id, 1);
    }
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 10; j++) {
            if (GWMgTimeGet() == MG_TIME_DAY) {
                id = espEntry(fn_1_C45C(DATANUM(DATA_result, 19)), 256, 0);
                lbl_1_bss_320[i][j] = id;
            } else {
                id = espEntry(fn_1_C45C(DATANUM(DATA_result, 20)), 256, 0);
                lbl_1_bss_320[i][j] = id;
            }
            lbl_1_bss_1B8[i][j] = lbl_1_data_240[0] + (9 - j) * 80;
            lbl_1_bss_50[i][j] = lbl_1_data_240[1] + i * 80;
            espPosSet(id, lbl_1_bss_1B8[i][j], lbl_1_bss_50[i][j]);
            espDrawNoSet(id, 64);
        }
    }
    HuSprExecLayerCameraSet(64, HU3D_CAM0, 1);
    for (i = 0; i < 4; i++) {
        score[i] = (GWPlayerStarGet(i) << 10) | GWPlayerCoinGet(i);
        mgCoin = GwPlayer[i].mgCoin;
        mgCoinBonus = GwPlayer[i].mgCoinBonus;
        score[i] = mgCoinBonus + (mgCoin + score[i]);
    }
    for (i = 0; i < 4; i++) {
        rankNum = 0;
        for (j = 0; j < 4; j++) {
            if (j != i && score[i] < score[j]) {
                rankNum++;
            }
        }
        rankNew[i] = rankNum;
    }
    for (i = 0; i < 4; i++) {
        k = i * 2;
        id = espEntry(fn_1_C45C(DATANUM(DATA_result, 9) + rankNew[i]), 200, 0);
        lbl_1_bss_488[i].rankNewEspId = id;
        espPosSet(id, lbl_1_data_54[k], lbl_1_data_54[k + 1]);
        espDrawNoSet(id, 64);
        espAttrSet(id, 4);
        espScaleSet(id, 0.95f, 0.95f);
    }
}

void fn_1_1E8C(void)
{
    s32 i;
    s32 j;

    espKill(lbl_1_bss_5E8);
    for (i = 0; i < 4; i++) {
        espKill(lbl_1_bss_488[i].rankEspId);
        espKill(lbl_1_bss_488[i].rankNewEspId);
        espKill(lbl_1_bss_488[i].charEspId);
        espKill(lbl_1_bss_488[i].coinAddEspId);
        for (j = 0; j < 3; j++) {
            espKill(lbl_1_bss_488[i].coinAddNumEspId[j]);
        }
        for (j = 0; j < 4; j++) {
            espKill(lbl_1_bss_488[i].starNumEspId[j]);
            espKill(lbl_1_bss_488[i].coinNumEspId[j]);
        }
        Hu3DModelKill(lbl_1_bss_488[i].crownMdlId);
        Hu3DModelKill(lbl_1_bss_488[i].starMdlId);
        Hu3DModelKill(lbl_1_bss_488[i].coinMdlId);
    }
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 10; j++) {
            espKill(lbl_1_bss_320[i][j]);
        }
    }
    CharModelKill(-1);
}

void fn_1_2068(void)
{
    s16 coin[4];
    s16 coinNum[5];
    float scale;
    float addTime;
    float speed;
    s16 i;
    s16 j;
    s16 maxTime;
    s16 time;
    s16 crownTime;

    fn_1_3FB0(coinNum);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            espAttrReset(lbl_1_bss_488[i].coinAddNumEspId[j], 4);
        }
        fn_1_3488(i, coinNum[i]);
        espAttrReset(lbl_1_bss_488[i].coinAddEspId, 4);
    }
    maxTime = (lbl_1_bss_624) ? 3 : 10;
    for (time = 0; time <= maxTime; time++) {
        scale = HuSin((90.0 / maxTime) * time);
        for (i = 0; i < 4; i++) {
            espScaleSet(lbl_1_bss_488[i].coinAddEspId, scale, scale);
            for (j = 0; j < 3; j++) {
                espScaleSet(lbl_1_bss_488[i].coinAddNumEspId[j], scale, scale);
            }
        }
        HuPrcVSleep();
    }
    for (i = 0; i < 4; i++) {
        espScaleSet(lbl_1_bss_488[i].coinAddEspId, 1.0, 1.0);
    }
    for (crownTime = 0; crownTime <= 14;) {
        for (i = 0; i < 4; i++) {
            if (coinNum[i] >= 10) {
                Hu3DModelAttrReset(lbl_1_bss_488[i].crownMdlId, HU3D_ATTR_DISPOFF);
            }
            scale = 0.1f + HuSin((120.0f / 14.0f) * crownTime) * (0.8 * (1.0 / sin((M_PI * 2) / 3)));
            Hu3DModelScaleSet(lbl_1_bss_488[i].crownMdlId, scale, scale, scale);
        }
        crownTime += (lbl_1_bss_624) ? 2 : 1;
        HuPrcVSleep();
    }
    if (!lbl_1_bss_624) {
        HuPrcSleep(15);
    }
    maxTime = 0;
    addTime = maxTime;
    for (i = 0; i < 4; i++) {
        coin[i] = GWPlayerCoinGet(i);
    }
    while (1) {
        speed = (lbl_1_bss_624) ? 1.0f : 0.2f;
        addTime += speed;
        if (addTime >= 1.0f) {
            for (i = maxTime = 0; i < 4; i++) {
                if (coinNum[i] == 0) {
                    fn_1_3488(i, 0);
                    lbl_1_bss_60C[i] = 1.0f;
                    maxTime++;
                } else {
                    coin[i]++;
                    coinNum[i]--;
                    HuAudFXPlay(7);
                    lbl_1_bss_60C[i] = 3.0f;
                }
                fn_1_3488(i, coinNum[i]);
                fn_1_31F0(i, coin[i]);
            }
            addTime = 0.0f;
        }
        if (maxTime >= 4) {
            break;
        }
        HuPrcVSleep();
    }
    HuPrcSleep(30);
    maxTime = (lbl_1_bss_624) ? 3 : 10;
    for (time = 0; time <= maxTime; time++) {
        scale = HuCos((90.0 / maxTime) * time);
        for (i = 0; i < 4; i++) {
            espScaleSet(lbl_1_bss_488[i].coinAddEspId, scale, scale);
            Hu3DModelScaleSet(lbl_1_bss_488[i].crownMdlId, scale, scale, scale);
            for (j = 0; j < 3; j++) {
                espScaleSet(lbl_1_bss_488[i].coinAddNumEspId[j], scale, scale);
            }
        }
        HuPrcVSleep();
    }
    for (i = 0; i < 4; i++) {
        espAttrSet(lbl_1_bss_488[i].coinAddEspId, 4);
        Hu3DModelAttrSet(lbl_1_bss_488[i].crownMdlId, HU3D_ATTR_DISPOFF);
        for (j = 0; j < 3; j++) {
            espAttrSet(lbl_1_bss_488[i].coinAddNumEspId[j], 4);
        }
    }
}

void fn_1_2878(void)
{
    s32 unused;
    s32 changeF[4];
    float speed[4];
    float speed2[4];
    float scaleEnd[4];
    float scale[4];
    s32 i;
    s32 j;
    s32 k;
    s32 sameNum;

    sameNum = 0;
    unused = 0;
    for (i = 0; i < 4; i++) {
        if (lbl_1_bss_5F4[i] > lbl_1_bss_5EC[i]) {
            changeF[i] = 1;
            Hu3DMotionShiftSet(lbl_1_bss_488[i].charMdlId, lbl_1_bss_488[i].charMotId[1], 0.0f, 8.0f, 0);
            if (lbl_1_bss_5EC[i] != 0) {
                CharFXStop(GwPlayerConf[i].charNo, 0x23F);
                CharFXPlay(GwPlayerConf[i].charNo, 0x243);
            }
        } else if (lbl_1_bss_5F4[i] < lbl_1_bss_5EC[i]) {
            changeF[i] = 1;
            Hu3DMotionShiftSet(lbl_1_bss_488[i].charMdlId, lbl_1_bss_488[i].charMotId[2], 0.0f, 8.0f, 0);
        } else {
            changeF[i] = 0;
            sameNum++;
        }
        if (changeF[i]) {
            scale[i] = lbl_1_data_1D4[lbl_1_bss_5F4[i]].x;
            scaleEnd[i] = lbl_1_data_1D4[lbl_1_bss_5EC[i]].x;
            speed[i] = (scaleEnd[i] - scale[i]) / 20.0f;
            speed2[i] = (scaleEnd[i] - scale[i]) / 30.0f;
        }
    }
    if (sameNum >= 4) {
        for (i = 0; i < 4; i++) {
            espAttrSet(lbl_1_bss_488[i].rankEspId, 4);
            espAttrReset(lbl_1_bss_488[i].rankNewEspId, 4);
        }
        lbl_1_bss_8 = 1;
        return;
    }
    lbl_1_bss_2C = 1;
    lbl_1_bss_28 = 0;
    lbl_1_bss_24 = 1.0f;
    lbl_1_bss_20 = 0.028571429f;
    lbl_1_bss_1C = 0;
    lbl_1_bss_18 = 1.0f;
    lbl_1_bss_14 = 0.028571429f;
    for (k = 0; k < 3; k++) {
        for (j = 0; j < 20; j++) {
            for (i = 0; i < 4; i++) {
                if (changeF[i]) {
                    scale[i] += speed2[i];
                    Hu3DModelScaleSet(lbl_1_bss_488[i].charMdlId, scale[i], scale[i], scale[i]);
                }
            }
            fn_1_2EBC();
            HuPrcVSleep();
        }
        for (j = 0; j < 10.0f; j++) {
            for (i = 0; i < 4; i++) {
                if (changeF[i]) {
                    scale[i] -= speed2[i];
                    Hu3DModelScaleSet(lbl_1_bss_488[i].charMdlId, scale[i], scale[i], scale[i]);
                }
            }
            fn_1_2EBC();
            HuPrcVSleep();
        }
    }
    for (i = 0; i < 4; i++) {
        espScaleSet(lbl_1_bss_488[i].charEspId, lbl_1_data_204[lbl_1_bss_5EC[i]], lbl_1_data_204[lbl_1_bss_5EC[i]]);
        Hu3DMotionShiftSet(lbl_1_bss_488[i].charMdlId, lbl_1_bss_488[i].charMotId[0], 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
    }
    lbl_1_bss_8 = 1;
}

void fn_1_2EBC(void)
{
    s32 i;
    s32 bankF;

    bankF = 0;
    if (lbl_1_bss_2C) {
        if (lbl_1_bss_1C == 0) {
            lbl_1_bss_24 -= lbl_1_bss_20;
            lbl_1_bss_18 -= lbl_1_bss_14;
            if (lbl_1_bss_24 <= 0.0f) {
                lbl_1_bss_24 = 0.0f;
                lbl_1_bss_18 = 0.0f;
                lbl_1_bss_1C = 1;
                bankF = 1;
            }
        } else {
            lbl_1_bss_24 += lbl_1_bss_20;
            lbl_1_bss_18 += lbl_1_bss_14;
            if (lbl_1_bss_24 >= 1.0f) {
                lbl_1_bss_2C = 0;
            }
        }
        for (i = 0; i < 4; i++) {
            if (lbl_1_bss_5F4[i] != lbl_1_bss_5EC[i]) {
                if (bankF) {
                    espBankSet(lbl_1_bss_488[i].charEspId, lbl_1_bss_5EC[i]);
                    espAttrSet(lbl_1_bss_488[i].rankEspId, 4);
                    espAttrReset(lbl_1_bss_488[i].rankNewEspId, 4);
                }
                espScaleSet(lbl_1_bss_488[i].charEspId, lbl_1_bss_24 * lbl_1_data_204[lbl_1_bss_5EC[i]],
                    lbl_1_bss_24 * lbl_1_data_204[lbl_1_bss_5EC[i]]);
                if (lbl_1_bss_1C == 0) {
                    espTPLvlSet(lbl_1_bss_488[i].rankEspId, lbl_1_bss_18);
                } else {
                    espTPLvlSet(lbl_1_bss_488[i].rankNewEspId, lbl_1_bss_18);
                }
            }
        }
    }
}

/* @@PART2@@ */
/* @@PART3@@ */
/* @@PART4@@ */
/* @@END@@ */
