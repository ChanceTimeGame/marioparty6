/* Retail's later resultDll files do not carry math.h's sqrtf statics in .rodata:
 * keep include/math.h out and declare what this file uses. */
#define _MATH_H
#define M_PI 3.141592653589793
double sin(double x);
double cos(double x);
float sqrtf(float x);
double atan2(double y, double x);
#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/frand.h"
/* GWPlayerStarGet is called with an int result (no extsh) */
#define GWPlayerStarGet GWPlayerStarGet_proto
#include "game/gamework.h"
#undef GWPlayerStarGet
s32 GWPlayerStarGet(GW_PLAYER_ID playerNo);
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/init.h"
#include "game/main.h"
#include "game/memory.h"
#include "game/mgdata.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/saveload.h"
#include "game/sprite.h"
#include "game/window.h"
#include "game/wipe.h"
#include "humath.h"
#include "string.h"

#include "dolphin.h"

#include "datadir_enum.h"
#include "REL/resultDll.h"

/* The esp* sprite calls (game/esprite.h) and CharFXPlay/CharFXStop are called
 * unprototyped: their headers stay out of this unit. */

/* this file */
void fn_1_49C4(void);
void fn_1_4D48(void);
void fn_1_523C(void);
void fn_1_60F8(void);
void fn_1_62F0(void);
void fn_1_6B78(void);
void fn_1_71F0(void);
void fn_1_73D4(s32 teamNo, s32 num);
void fn_1_7520(s32 teamNo, s32 num);
void fn_1_766C(s32 teamNo, s32 num);
u32 fn_1_7750(s16 charNo1, s16 charNo2);
s32 fn_1_7840(s32 team, s32 no);
s16 fn_1_78B4(s32 team);

/* other files of resultDll */
s32 fn_1_356C(void);
void fn_1_3640(s32 statId);
void fn_1_3698(void);
void fn_1_3FB0(s16 *coinNum);
void fn_1_4694(void);
void fn_1_47A8(void);
s32 fn_1_C45C(s32 dataNum);
extern s32 lbl_1_bss_624;

typedef struct ResultTeam_s {
    s32 frameSprId; /* 0x00 */
    s32 teamSprId; /* 0x04 */
    s32 coinSprId; /* 0x08 */
    s32 coinNumSprId[3]; /* 0x0C */
    s32 coinTotalSprId[4]; /* 0x18 */
    s32 starSprId[4]; /* 0x28 */
    s32 nameSprId; /* 0x38 */
    s32 charMdlId[2]; /* 0x3C */
    s32 charMotId[2][3]; /* 0x44 */
    s32 mdlId[3]; /* 0x5C */
} RESULT_TEAM; /* size 0x68 */

/* team file .bss, reverse definition order */
/* .bss in reverse definition order */
float lbl_1_bss_B94[3];
float lbl_1_bss_B8C[2];
s32 lbl_1_bss_B88;
RESULT_TEAM lbl_1_bss_AB8[2];
s32 lbl_1_bss_950[9][10];
float lbl_1_bss_7E8[9][10];
float lbl_1_bss_680[9][10];
s32 lbl_1_bss_678[2];
s32 lbl_1_bss_670[2];
s16 lbl_1_bss_66C[2];
s16 lbl_1_bss_668[2];
s32 lbl_1_bss_664;
s32 lbl_1_bss_660;
float lbl_1_bss_65C;
float lbl_1_bss_658;
s16 lbl_1_bss_654;
s32 lbl_1_bss_64C[2];
s32 lbl_1_bss_644[2]; /* unreferenced */
s32 lbl_1_bss_640;
s32 lbl_1_bss_63C; /* unreferenced */
s32 lbl_1_bss_638;

void fn_1_49C4(void)
{
    s16 i;
    HUWINID winId;
    s16 comNum;
    BOOL endF;
    s32 statId;
    HUPROCESS *proc;

    endF = FALSE;
    proc = HuPrcCurrentGet();
    fn_1_47A8();
    for (i = comNum = 0; i < 4; i++) {
        if (GwPlayerConf[i].type != GW_PLAYER_TYPE_MAN) {
            comNum++;
        }
    }
    if (comNum == 4) {
        lbl_1_bss_638 = TRUE;
    } else {
        lbl_1_bss_638 = FALSE;
    }
    for (i = 0; i < 2; i++) {
        lbl_1_bss_B8C[i] = 1.0f;
        lbl_1_bss_B94[i] = 1.0f;
    }
    fn_1_523C();
    HuPrcChildCreate(fn_1_4D48, 100, 0x2000, 0, proc);
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 20);
    HuAudSStreamPlay(70);
    while (WipeCheck()) {
        HuPrcVSleep();
    }
    HuDataDirClose(fn_1_C45C(DATA_result));
    HuPrcSleep(10);
    if (!lbl_1_bss_624) {
        HuPrcSleep(20);
    }
    fn_1_62F0();
    for (i = 0; i < 2; i++) {
        lbl_1_bss_66C[i] = fn_1_78B4(i);
    }
    fn_1_3698();
    for (i = 0; i < 2; i++) {
        lbl_1_bss_668[i] = fn_1_78B4(i);
    }
    fn_1_6B78();
    winId = HuWinCreate(-10000.0f, 405.0f, 320, 40, 0);
    HuWinMesSpeedSet(winId, 0);
    HuWinBGTPLvlSet(winId, 0.0f);
    HuWinPriSet(winId, 5);
    HuWinAttrSet(winId, HUWIN_ATTR_ALIGN_CENTER);
    if (!lbl_1_bss_638) {
        HuWinMesSet(winId, 0x10000);
    }
    HuPrcSleep(4);
    statId = fn_1_356C();
    while (1) {
        if (lbl_1_bss_638) {
            endF = TRUE;
        } else {
            for (i = 0; i < 4; i++) {
                if (HuPadStatGet(i) == 0 && (HuPadBtnDown[i] & PAD_BUTTON_A)) {
                    HuAudFXPlay(2);
                    endF = TRUE;
                    break;
                }
            }
        }
        if (endF) {
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
    fn_1_4694();
    omSysPauseEnable(TRUE);
    fn_1_60F8();
    HuDataDirClose(fn_1_C45C(DATA_result));
    omOvlReturnEx(1, 1);
    HuPrcEnd();
    while (1) {
        HuPrcVSleep();
    }
}

void fn_1_4D48(void)
{
    s16 i;
    s16 j;
    float rotA[2];
    float rotB[2];
    float posZ;
    float posY;
    float posX;

    posZ = 0.0f;
    posY = 0.0f;
    posX = 0.0f;
    for (i = 0; i < 2; i++) {
        rotA[i] = 0.0f;
        rotB[i] = 0.0f;
    }
    while (1) {
        if (lbl_1_bss_638) {
            lbl_1_bss_624 = TRUE;
        } else {
            lbl_1_bss_624 = FALSE;
            for (i = 0; i < 4; i++) {
                if (HuPadStatGet(i) == 0 && (HuPadBtn[i] & PAD_BUTTON_A)) {
                    lbl_1_bss_624 = TRUE;
                }
            }
        }
        for (i = 0; i < 2; i++) {
            rotA[i] += 1.2f * lbl_1_bss_B8C[i];
            if (rotA[i] >= 360.0f) {
                rotA[i] -= 360.0f;
            }
            rotB[i] += lbl_1_bss_B94[i];
            if (rotB[i] >= 360.0f) {
                rotB[i] -= 360.0f;
            }
            Hu3DModelRotSet(lbl_1_bss_AB8[0].mdlId[1], 0.0f, rotA[0], 0.0f);
            Hu3DModelRotSet(lbl_1_bss_AB8[1].mdlId[1], 0.0f, rotA[1], 0.0f);
            Hu3DModelRotSet(lbl_1_bss_AB8[0].mdlId[2], 0.0f, rotB[0], 0.0f);
            Hu3DModelRotSet(lbl_1_bss_AB8[1].mdlId[2], 0.0f, rotB[1], 0.0f);
        }
        for (i = 0; i < 9; i++) {
            for (j = 0; j < 10; j++) {
                lbl_1_bss_7E8[i][j] += 1.0f;
                lbl_1_bss_680[i][j] += 1.0f;
                if (lbl_1_bss_7E8[i][j] > 616.0f) {
                    lbl_1_bss_7E8[i][j] = -80.0f;
                }
                if (lbl_1_bss_680[i][j] > 520.0f) {
                    lbl_1_bss_680[i][j] = -80.0f;
                }
                espPosSet(lbl_1_bss_950[i][j], lbl_1_bss_7E8[i][j], lbl_1_bss_680[i][j]);
            }
        }
        HuPrcVSleep();
    }
}

float lbl_1_data_3F8[4] = { 318.0f, 165.0f, 258.0f, 340.0f };
float lbl_1_data_408[4] = { 448.0f, 135.0f, 388.0f, 310.0f };
float lbl_1_data_418[4] = { 428.0f, 185.0f, 368.0f, 360.0f };
float lbl_1_data_428[4] = { 428.0f, 215.0f, 368.0f, 390.0f };
float lbl_1_data_438[4] = { 138.0f, 115.0f, 78.0f, 290.0f };
float lbl_1_data_448[4] = { 98.0f, 83.0f, 38.0f, 258.0f };
float lbl_1_data_458[4] = { 258.0f, 193.0f, 198.0f, 368.0f };
HuVecF lbl_1_data_468[4] = { { 178.0f, 195.0f, 9000.0f }, { 338.0f, 195.0f, 9000.0f }, { 118.0f, 370.0f, 9000.0f }, { 278.0f, 370.0f, 9000.0f } };
HuVecF lbl_1_data_498[2] = { { 258.0f, 130.0f, 9000.0f }, { 198.0f, 305.0f, 9000.0f } };
HuVecF lbl_1_data_4B0[2] = { { 403.0f, 185.0f, 9000.0f }, { 343.0f, 360.0f, 9000.0f } };
HuVecF lbl_1_data_4C8[2] = { { 403.0f, 215.0f, 9000.0f }, { 343.0f, 390.0f, 9000.0f } };
float lbl_1_data_4E0[4] = { 133.0f, 200.0f, 73.0f, 375.0f };
HuVecF lbl_1_data_4F0[2] = { { 1.0f, 1.0f, 1.0f }, { 0.75f, 0.75f, 0.75f } };
float lbl_1_data_508 = 1.0f;
s32 lbl_1_data_50C[2] = { -80, -80 };

void fn_1_523C(void)
{
    s32 id;
    s32 i;
    s32 j;
    s32 ofs;
    s32 charNo;
    s32 num;
    s32 lang;
    u32 mesId;
    s32 fileOfs;
    s32 charNoTbl[2];
    HuVecF pos;

    lang = GWLanguageGet();
    if (lang == 1) {
        id = espEntry(fn_1_C45C(DATANUM(DATA_result, 5)), 180, 0);
        lbl_1_bss_B88 = id;
        espPosSet(id, 288.0f, 60.0f);
        espDrawNoSet(id, 64);
    } else {
        id = espEntry(fn_1_C45C(DATANUM(DATA_result, 7)), 180, 0);
        lbl_1_bss_B88 = id;
        espPosSet(id, 288.0f, 60.0f);
        espDrawNoSet(id, 64);
    }
    for (i = 0; i < 2; i++) {
        ofs = i * 2;
        num = fn_1_78B4(i);
        id = espEntry(fn_1_C45C(i + DATANUM(DATA_result, 13)), 180, 0);
        lbl_1_bss_AB8[i].frameSprId = id;
        espPosSet(id, lbl_1_data_3F8[ofs], lbl_1_data_3F8[ofs + 1]);
        espScaleSet(id, 0.93f, 0.93f);
        espDrawNoSet(id, 64);
        id = espEntry(fn_1_C45C(DATANUM(DATA_result, 3)), 150, 0);
        lbl_1_bss_AB8[i].teamSprId = id;
        espPosSet(id, lbl_1_data_408[ofs], lbl_1_data_408[ofs + 1]);
        espBankSet(id, ofs + num);
        espDrawNoSet(id, 64);
        id = espEntry(fn_1_C45C(DATANUM(DATA_result, 4)), 130, 0);
        lbl_1_bss_AB8[i].coinSprId = id;
        espPosSet(id, lbl_1_data_438[ofs], lbl_1_data_438[ofs + 1]);
        espDrawNoSet(id, 64);
        espScaleSet(id, 0.0, 0.0);
        espAttrSet(id, HUSPR_ATTR_DISPOFF);
        if (lang == 1) {
            id = espEntry(fn_1_C45C(i + DATANUM(DATA_result, 15)), 130, 0);
            lbl_1_bss_AB8[i].nameSprId = id;
            espPosSet(id, lbl_1_data_458[ofs], lbl_1_data_458[ofs + 1]);
            espDrawNoSet(id, 64);
        } else {
            id = espEntry(fn_1_C45C(i + DATANUM(DATA_result, 17)), 130, 0);
            lbl_1_bss_AB8[i].nameSprId = id;
            espPosSet(id, lbl_1_data_458[ofs], lbl_1_data_458[ofs + 1]);
            espDrawNoSet(id, 64);
        }
        for (j = 0; j < 3; j++) {
            if (j == 0) {
                id = espEntry(fn_1_C45C(DATANUM(DATA_result, 0)), 120, 10);
                lbl_1_bss_AB8[i].coinNumSprId[j] = id;
            } else {
                id = espEntry(fn_1_C45C(DATANUM(DATA_result, 0)), 120, 0);
                lbl_1_bss_AB8[i].coinNumSprId[j] = id;
            }
            espPosSet(id, lbl_1_data_448[ofs] + j * 18, lbl_1_data_448[ofs + 1]);
            espDrawNoSet(id, 64);
            espScaleSet(id, 0.0, 0.0);
            espAttrSet(id, HUSPR_ATTR_DISPOFF);
        }
        for (j = 0; j < 4; j++) {
            if (j == 0) {
                id = espEntry(fn_1_C45C(DATANUM(DATA_result, 1)), 120, 10);
                lbl_1_bss_AB8[i].coinTotalSprId[j] = id;
            } else {
                id = espEntry(fn_1_C45C(DATANUM(DATA_result, 1)), 120, 0);
                lbl_1_bss_AB8[i].coinTotalSprId[j] = id;
            }
            espPosSet(id, lbl_1_data_418[ofs] + j * 17, lbl_1_data_418[ofs + 1]);
            espDrawNoSet(id, 64);
        }
        for (j = 0; j < 4; j++) {
            if (j == 0) {
                id = espEntry(fn_1_C45C(DATANUM(DATA_result, 1)), 120, 10);
                lbl_1_bss_AB8[i].starSprId[j] = id;
            } else {
                id = espEntry(fn_1_C45C(DATANUM(DATA_result, 1)), 120, 0);
                lbl_1_bss_AB8[i].starSprId[j] = id;
            }
            espPosSet(id, lbl_1_data_428[ofs] + j * 17, lbl_1_data_428[ofs + 1]);
            espDrawNoSet(id, 64);
        }
        lbl_1_bss_678[i] = GWPlayerCoinGet(fn_1_7840(i, 0));
        lbl_1_bss_670[i] = GWPlayerStarGet(fn_1_7840(i, 0));
        fn_1_73D4(i, lbl_1_bss_678[i]);
        fn_1_7520(i, lbl_1_bss_670[i]);
        fn_1_766C(i, 0);
        for (j = 0; j < 2; j++) {
            charNo = GwPlayerConf[fn_1_7840(i, j)].charNo;
            fileOfs = charNo * 4;
            charNoTbl[j] = GwPlayerConf[fn_1_7840(i, j)].charNo;
            id = CharModelCreate(charNo, 2);
            lbl_1_bss_AB8[i].charMdlId[j] = id;
            lbl_1_bss_AB8[i].charMotId[j][0] = CharMotionCreate(charNo, DATANUM(DATA_mariomot, 0));
            lbl_1_bss_AB8[i].charMotId[j][1] = CharMotionCreate(charNo, DATANUM(DATA_mariomot, 36));
            lbl_1_bss_AB8[i].charMotId[j][2] = CharMotionCreate(charNo, DATANUM(DATA_mariomot, 37));
            CharMotionDataClose(charNo);
            Hu3DMotionSet(id, lbl_1_bss_AB8[i].charMotId[j][0]);
            Hu3DModelAttrSet(id, HU3D_MOTATTR_LOOP);
            Hu3DModelCameraSet(id, HU3D_CAM0);
            Hu3D2Dto3D(&lbl_1_data_468[j + i * 2], HU3D_CAM0, &pos);
            Hu3DModelPosSetV(id, &pos);
            Hu3DModelScaleSetV(id, &lbl_1_data_4F0[num]);
            Hu3DModelLayerSet(id, 1);
        }
        id = Hu3DModelCreateData(fn_1_C45C(DATANUM(DATA_result, 22)));
        lbl_1_bss_AB8[i].mdlId[0] = id;
        Hu3DModelCameraSet(id, HU3D_CAM0);
        Hu3D2Dto3D(&lbl_1_data_498[i], HU3D_CAM0, &pos);
        Hu3DModelPosSetV(id, &pos);
        Hu3DModelLayerSet(id, 1);
        Hu3DModelScaleSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrSet(id, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(id, HU3D_MOTATTR_LOOP);
        id = Hu3DModelCreateData(fn_1_C45C(DATANUM(DATA_result, 25)));
        lbl_1_bss_AB8[i].mdlId[1] = id;
        Hu3DModelCameraSet(id, HU3D_CAM0);
        Hu3D2Dto3D(&lbl_1_data_4B0[i], HU3D_CAM0, &pos);
        Hu3DModelScaleSet(id, 0.6f, 0.6f, 0.6f);
        Hu3DModelPosSetV(id, &pos);
        Hu3DModelLayerSet(id, 1);
        id = Hu3DModelCreateData(fn_1_C45C(DATANUM(DATA_result, 26)));
        lbl_1_bss_AB8[i].mdlId[2] = id;
        Hu3DModelCameraSet(id, HU3D_CAM0);
        Hu3D2Dto3D(&lbl_1_data_4C8[i], HU3D_CAM0, &pos);
        Hu3DModelScaleSet(id, 0.5f, 0.5f, 0.5f);
        Hu3DModelPosSetV(id, &pos);
        Hu3DModelLayerSet(id, 1);
        mesId = fn_1_7750(charNoTbl[0], charNoTbl[1]);
        lbl_1_bss_64C[i] = HuWinCreate(lbl_1_data_4E0[ofs], lbl_1_data_4E0[ofs + 1], 240, 32, 0);
        HuWinBGTPLvlSet(lbl_1_bss_64C[i], 255.0f);
        HuWinPriSet(lbl_1_bss_64C[i], 0);
        HuWinAttrSet(lbl_1_bss_64C[i], HUWIN_ATTR_ALIGN_CENTER);
        HuWinMesSpeedSet(lbl_1_bss_64C[i], 0);
        HuWinMesSet(lbl_1_bss_64C[i], mesId);
    }
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 10; j++) {
            if (GWMgTimeGet() == 0) {
                id = espEntry(fn_1_C45C(DATANUM(DATA_result, 19)), 256, 0);
                lbl_1_bss_950[i][j] = id;
            } else {
                id = espEntry(fn_1_C45C(DATANUM(DATA_result, 20)), 256, 0);
                lbl_1_bss_950[i][j] = id;
            }
            lbl_1_bss_7E8[i][j] = lbl_1_data_50C[0] + (9 - j) * 80;
            lbl_1_bss_680[i][j] = lbl_1_data_50C[1] + i * 80;
            espPosSet(id, lbl_1_bss_7E8[i][j], lbl_1_bss_680[i][j]);
            espDrawNoSet(id, 64);
        }
    }
    HuSprExecLayerCameraSet(64, HU3D_CAM0, 1);
}

void fn_1_60F8(void)
{
    s32 i;
    s32 j;

    espKill(lbl_1_bss_B88);
    for (i = 0; i < 2; i++) {
        espKill(lbl_1_bss_AB8[i].frameSprId);
        espKill(lbl_1_bss_AB8[i].teamSprId);
        espKill(lbl_1_bss_AB8[i].coinSprId);
        espKill(lbl_1_bss_AB8[i].nameSprId);
        for (j = 0; j < 3; j++) {
            espKill(lbl_1_bss_AB8[i].coinNumSprId[j]);
        }
        for (j = 0; j < 4; j++) {
            espKill(lbl_1_bss_AB8[i].coinTotalSprId[j]);
            espKill(lbl_1_bss_AB8[i].starSprId[j]);
        }
        Hu3DModelKill(lbl_1_bss_AB8[i].mdlId[0]);
        Hu3DModelKill(lbl_1_bss_AB8[i].mdlId[1]);
        Hu3DModelKill(lbl_1_bss_AB8[i].mdlId[2]);
        HuWinKill(lbl_1_bss_64C[i]);
    }
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 10; j++) {
            espKill(lbl_1_bss_950[i][j]);
        }
    }
    CharModelKill(-1);
}

#pragma auto_inline reset
#pragma dont_inline reset
/* part 3 needs, beyond part 1's HEAD:
 * - team file (0x4D48-0x7A17): RESULT_TEAM for lbl_1_bss_AB8 (part 2 uses it too), part 3's own
 *   .bss 640/654-664 and the team-name table (.data 0x514, string 0x582);
 * - battle file (0x7A18 on, with part 4): its structs, .bss 0xBA0-0x14A4 and .data 0x590-0x7AB. */
#pragma auto_inline off









void fn_1_62F0(void)
{
    s16 addNum[2] = { 0, 0 };
    s16 countNum[2];
    s16 coinNum[4];
    float scale;
    float addTime;
    s16 i;
    s16 j;
    s16 maxTime;
    s16 time;
    s16 crownTime;

    fn_1_3FB0(coinNum);
    for(i = 0; i < 4; i++) {
        addNum[GwPlayer[i].team] += coinNum[i];
    }
    for(i = 0; i < 2; i++) {
        for(j = 0; j < 3; j++) {
            espAttrReset(lbl_1_bss_AB8[i].coinNumSprId[j], 4);
        }
        fn_1_766C(i, addNum[i]);
        espAttrReset(lbl_1_bss_AB8[i].coinSprId, 4);
    }
    maxTime = (lbl_1_bss_624) ? 3 : 10;
    for(time = 0; time <= maxTime; time++) {
        scale = HuSin((90.0 / maxTime) * time);
        for(i = 0; i < 2; i++) {
            espScaleSet(lbl_1_bss_AB8[i].coinSprId, scale, scale);
            for(j = 0; j < 3; j++) {
                espScaleSet(lbl_1_bss_AB8[i].coinNumSprId[j], scale, scale);
            }
        }
        HuPrcVSleep();
    }
    for(i = 0; i < 2; i++) {
        espScaleSet(lbl_1_bss_AB8[i].coinSprId, 1.0, 1.0);
    }
    for(crownTime = 0; crownTime <= 14;) {
        for(i = 0; i < 2; i++) {
            if(addNum[i] >= 10) {
                Hu3DModelAttrReset(lbl_1_bss_AB8[i].mdlId[0], HU3D_ATTR_DISPOFF);
            }
            scale = 0.1f + HuSin((120.0f / 14.0f) * crownTime) * (0.8 * (1.0 / sin((M_PI * 2) / 3)));
            Hu3DModelScaleSet(lbl_1_bss_AB8[i].mdlId[0], scale, scale, scale);
        }
        crownTime += (lbl_1_bss_624) ? 2 : 1;
        HuPrcVSleep();
    }
    if(!lbl_1_bss_624) {
        HuPrcSleep(15);
    }
    maxTime = 0;
    addTime = maxTime;
    for(i = 0; i < 2; i++) {
        countNum[i] = GWPlayerCoinGet(fn_1_7840(i, 0));
    }
    while(1) {
        addTime += (lbl_1_bss_624) ? 1.0f : 0.2f;
        if(addTime >= 1.0f) {
            for(i = maxTime = 0; i < 2; i++) {
                if(addNum[i] == 0) {
                    fn_1_766C(i, 0);
                    lbl_1_bss_B94[i] = 1.0f;
                    maxTime++;
                } else {
                    countNum[i]++;
                    addNum[i]--;
                    HuAudFXPlay(7);
                    lbl_1_bss_B94[i] = 3.0f;
                }
                fn_1_766C(i, addNum[i]);
                fn_1_73D4(i, countNum[i]);
            }
            addTime = 0.0f;
        }
        if(maxTime >= 2) {
            break;
        }
        HuPrcVSleep();
    }
    HuPrcSleep(30);
    maxTime = (lbl_1_bss_624) ? 3 : 10;
    for(time = 0; time <= maxTime; time++) {
        scale = HuCos((90.0 / maxTime) * time);
        for(i = 0; i < 2; i++) {
            espScaleSet(lbl_1_bss_AB8[i].coinSprId, scale, scale);
            Hu3DModelScaleSet(lbl_1_bss_AB8[i].mdlId[0], scale, scale, scale);
            for(j = 0; j < 3; j++) {
                espScaleSet(lbl_1_bss_AB8[i].coinNumSprId[j], scale, scale);
            }
        }
        HuPrcVSleep();
    }
    for(i = 0; i < 2; i++) {
        espAttrSet(lbl_1_bss_AB8[i].coinSprId, 4);
        Hu3DModelAttrSet(lbl_1_bss_AB8[i].mdlId[0], HU3D_ATTR_DISPOFF);
        for(j = 0; j < 3; j++) {
            espAttrSet(lbl_1_bss_AB8[i].coinNumSprId[j], 4);
        }
    }
}

void fn_1_6B78(void)
{
    s32 unused;
    float scale[2];
    float target[2];
    float speed[2];
    s32 motF[2];
    s32 i;
    s32 j;
    s32 doneNum;
    s32 k;

    doneNum = 0;
    unused = 0;
    for(i = 0; i < 2; i++) {
        if(lbl_1_bss_66C[i] > lbl_1_bss_668[i]) {
            motF[i] = 1;
            Hu3DMotionShiftSet(lbl_1_bss_AB8[i].charMdlId[0], lbl_1_bss_AB8[i].charMotId[0][1], 0.0f, 8.0f, HU3D_MOTATTR_NONE);
            Hu3DMotionShiftSet(lbl_1_bss_AB8[i].charMdlId[1], lbl_1_bss_AB8[i].charMotId[1][1], 0.0f, 8.0f, HU3D_MOTATTR_NONE);
        } else if(lbl_1_bss_66C[i] < lbl_1_bss_668[i]) {
            motF[i] = 1;
            Hu3DMotionShiftSet(lbl_1_bss_AB8[i].charMdlId[0], lbl_1_bss_AB8[i].charMotId[0][2], 0.0f, 8.0f, HU3D_MOTATTR_NONE);
            Hu3DMotionShiftSet(lbl_1_bss_AB8[i].charMdlId[1], lbl_1_bss_AB8[i].charMotId[1][2], 0.0f, 8.0f, HU3D_MOTATTR_NONE);
        } else {
            motF[i] = 0;
            doneNum++;
        }
        if(motF[i]) {
            scale[i] = lbl_1_data_4F0[lbl_1_bss_66C[i]].x;
            target[i] = lbl_1_data_4F0[lbl_1_bss_668[i]].x;
            speed[i] = (target[i] - scale[i]) / 30.0f;
        }
    }
    if(doneNum >= 2) {
        lbl_1_bss_640 = 1;
        return;
    }
    lbl_1_bss_664 = 1;
    lbl_1_bss_660 = 0;
    lbl_1_bss_65C = 1.0f;
    lbl_1_bss_658 = 1.0f / 35.0f;
    lbl_1_bss_654 = 0;
    for(k = 0; k < 3; k++) {
        for(j = 0; j < 20; j++) {
            for(i = 0; i < 2; i++) {
                if(motF[i]) {
                    scale[i] += speed[i];
                    Hu3DModelScaleSet(lbl_1_bss_AB8[i].charMdlId[0], scale[i], scale[i], scale[i]);
                    Hu3DModelScaleSet(lbl_1_bss_AB8[i].charMdlId[1], scale[i], scale[i], scale[i]);
                }
            }
            fn_1_71F0();
            HuPrcVSleep();
        }
        for(j = 0; j < 10.0f; j++) {
            for(i = 0; i < 2; i++) {
                if(motF[i]) {
                    scale[i] -= speed[i];
                    Hu3DModelScaleSet(lbl_1_bss_AB8[i].charMdlId[0], scale[i], scale[i], scale[i]);
                    Hu3DModelScaleSet(lbl_1_bss_AB8[i].charMdlId[1], scale[i], scale[i], scale[i]);
                }
            }
            fn_1_71F0();
            HuPrcVSleep();
        }
    }
    for(i = 0; i < 2; i++) {
        espScaleSet(lbl_1_bss_AB8[i].teamSprId, 1.0, 1.0);
        Hu3DMotionShiftSet(lbl_1_bss_AB8[i].charMdlId[0], lbl_1_bss_AB8[i].charMotId[0][0], 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
        Hu3DMotionShiftSet(lbl_1_bss_AB8[i].charMdlId[1], lbl_1_bss_AB8[i].charMotId[1][0], 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
    }
    lbl_1_bss_640 = 1;
}

void fn_1_71F0(void)
{
    s32 i;
    s32 bankF;

    bankF = 0;
    if(lbl_1_bss_664) {
        if(lbl_1_bss_654 == 0) {
            lbl_1_bss_65C -= lbl_1_bss_658;
            if(lbl_1_bss_65C <= 0.0f) {
                lbl_1_bss_65C = 0.0f;
                lbl_1_bss_654 = 1;
                bankF = 1;
            }
        } else {
            lbl_1_bss_65C += lbl_1_bss_658;
            if(lbl_1_bss_65C >= 1.0f) {
                lbl_1_bss_664 = 0;
            }
        }
        for(i = 0; i < 2; i++) {
            if(lbl_1_bss_66C[i] != lbl_1_bss_668[i]) {
                if(bankF) {
                    espBankSet(lbl_1_bss_AB8[i].teamSprId, lbl_1_bss_668[i] + (i * 2));
                }
                espScaleSet(lbl_1_bss_AB8[i].teamSprId, lbl_1_bss_65C, lbl_1_bss_65C);
            }
        }
    }
}

void fn_1_73D4(s32 teamNo, s32 num)
{
    s32 digit[3];
    RESULT_TEAM team;
    s32 rest;

    team = lbl_1_bss_AB8[teamNo];
    if(num > 999) {
        num = 999;
    }
    rest = num;
    digit[0] = num / 100;
    if(digit[0] == 0) {
        espAttrSet(team.starSprId[1], 4);
    } else {
        espAttrReset(team.starSprId[1], 4);
        espBankSet(team.starSprId[1], digit[0]);
    }
    rest = num - digit[0] * 100;
    digit[1] = rest / 10;
    if(digit[1] == 0 && digit[0] == 0) {
        espAttrSet(team.starSprId[2], 4);
    } else {
        espAttrReset(team.starSprId[2], 4);
        espBankSet(team.starSprId[2], digit[1]);
    }
    rest = rest - digit[1] * 10;
    espBankSet(team.starSprId[3], rest);
}

void fn_1_7520(s32 teamNo, s32 num)
{
    s32 digit[3];
    RESULT_TEAM team;
    s32 rest;

    team = lbl_1_bss_AB8[teamNo];
    if(num > 999) {
        num = 999;
    }
    rest = num;
    digit[0] = num / 100;
    if(digit[0] == 0) {
        espAttrSet(team.coinTotalSprId[1], 4);
    } else {
        espAttrReset(team.coinTotalSprId[1], 4);
        espBankSet(team.coinTotalSprId[1], digit[0]);
    }
    rest = num - digit[0] * 100;
    digit[1] = rest / 10;
    if(digit[1] == 0 && digit[0] == 0) {
        espAttrSet(team.coinTotalSprId[2], 4);
    } else {
        espAttrReset(team.coinTotalSprId[2], 4);
        espBankSet(team.coinTotalSprId[2], digit[1]);
    }
    rest = rest - digit[1] * 10;
    espBankSet(team.coinTotalSprId[3], rest);
}

void fn_1_766C(s32 teamNo, s32 num)
{
    s32 digit[3];
    RESULT_TEAM team;
    s32 rest;

    team = lbl_1_bss_AB8[teamNo];
    if(num > 99) {
        num = 99;
    }
    rest = num;
    digit[0] = num / 10;
    if(digit[0] == 0) {
        espAttrSet(team.coinNumSprId[1], 4);
    } else {
        espAttrReset(team.coinNumSprId[1], 4);
        espBankSet(team.coinNumSprId[1], digit[0]);
    }
    rest = rest - digit[0] * 10;
    espBankSet(team.coinNumSprId[2], rest);
}

/* team-name messages: retail indexes [charNo1 * 11 + charNo2] */
s8 lbl_1_data_514[10 * 11] = {
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

u32 fn_1_7750(s16 charNo1, s16 charNo2)
{
    s32 teamNo;

    if(charNo1 > 10 || charNo2 > 10) {
        return 0x30037;
    }
    if(charNo1 < charNo2) {
        teamNo = lbl_1_data_514[charNo1 * 11 + charNo2];
    } else {
        teamNo = lbl_1_data_514[charNo2 * 11 + charNo1];
    }
    OSReport("%d:%d->%d\n", charNo1, charNo2, teamNo);
    if(teamNo == -1) {
        return 0x30037;
    }
    return teamNo + 0x30000;
}

/* fn_1_7840 inlines into fn_1_78B4 (auto inline, as in the team file) */
#pragma auto_inline on
static inline s32 GWPlayerTeamGet(s32 playerNo)
{
    return GwPlayer[playerNo].team;
}

s32 fn_1_7840(s32 team, s32 no)
{
    s32 num;
    s32 i;

    num = 0;
    for(i = 0; i < 4; i++) {
        if(team == GWPlayerTeamGet(i)) {
            if(num == no) {
                return i;
            }
            num++;
        }
    }
    return -1;
}

#pragma auto_inline off

s16 fn_1_78B4(s32 team)
{
    s32 score[2];
    s32 i;
    s32 rank;

    for(i = 0; i < 2; i++) {
        score[i] = GWPlayerCoinGet(fn_1_7840(i, 0)) | (GWPlayerStarGet(fn_1_7840(i, 0)) << 11);
    }
    rank = 0;
    for(i = 0; i < 2; i++) {
        if(i != team && score[team] < score[i]) {
            rank++;
        }
    }
    return rank;
}

