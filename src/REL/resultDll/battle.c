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
void fn_1_7A18(void);
void fn_1_7D34(void);
void fn_1_7E0C(void);
void fn_1_86B0(void);
void fn_1_8858(void);
void fn_1_8D4C(void);
void fn_1_924C(void);
void fn_1_95D8(void);
void fn_1_9B90(s32 playerNo, s32 num);
void fn_1_9D18(void);
void fn_1_A238(HuVecF *pos);
void fn_1_A928(HU3D_MODEL *modelP, RESULT_PARTICLE *particleP, Mtx mtx);

/* other files of resultDll */
s32 fn_1_356C(void);
void fn_1_3640(s32 statId);
void fn_1_3698(void);
void fn_1_3FB0(s16 *coinNum);
void fn_1_4694(void);
void fn_1_47A8(void);
void fn_1_AC54(void);
void fn_1_ACB8(void);
HU3D_MODELID fn_1_AD24(ANIMDATA *anim, s16 maxCnt);
void fn_1_B39C(HU3D_MODELID modelId);
void fn_1_C420(HU3D_MODELID modelId, RESULT_PARTICLEHOOK hook);
s32 fn_1_C45C(s32 dataNum);
extern s16 lbl_1_bss_61C;
extern s32 lbl_1_bss_624;

typedef struct ResultBattle_s {
    s32 esp0; /* 0x00 */
    s32 esp4; /* 0x04 */
    s32 esp8[3]; /* 0x08 */
    s32 mdl14; /* 0x14 */
    s32 charMdl; /* 0x18 */
    s32 mot[5]; /* 0x1C */
} RESULT_BATTLE; /* size 0x30 */
typedef struct ResultEffect_s {
    HuVecF pos; /* 0x00 */
    s16 modelId; /* 0x0C */
    s16 time; /* 0x0E */
} RESULT_EFFECT; /* size 0x10 */
/* .bss in reverse definition order */
RESULT_EFFECT lbl_1_bss_10A4[64];
s32 lbl_1_bss_1094[4];
s32 lbl_1_bss_1084[4];
float lbl_1_bss_1074[4];
s32 lbl_1_bss_1070;
s32 lbl_1_bss_106C;
RESULT_BATTLE lbl_1_bss_FAC[4];
s32 lbl_1_bss_DAC[128];
s32 lbl_1_bss_BAC[128]; /* unreferenced */
s32 lbl_1_bss_BA8;
ANIMDATA *lbl_1_bss_BA4;
s32 lbl_1_bss_BA0;
float lbl_1_data_590[8] = { 90.2000046f, 148.0f, 220.400009f, 148.0f, 355.600006f, 148.0f, 485.800018f, 148.0f };
float lbl_1_data_5B0[8] = { 90.2000046f, 238.0f, 219.400009f, 238.0f, 354.600006f, 238.0f, 483.800018f, 238.0f };
float lbl_1_data_5D0[8] = { 70.2000046f, 204.0f, 200.400009f, 204.0f, 335.600006f, 204.0f, 465.800018f, 204.0f };
HuVecF lbl_1_data_5F0[4] = { { 90.2000046f, -12.0f, 9000.0f }, { 220.400009f, -12.0f, 9000.0f }, { 355.600006f, -12.0f, 9000.0f }, { 485.800018f, -12.0f, 9000.0f } };
HuVecF lbl_1_data_620[4] = { { 90.2000046f, 368.0f, 9000.0f }, { 220.400009f, 368.0f, 9000.0f }, { 355.600006f, 368.0f, 9000.0f }, { 485.800018f, 368.0f, 9000.0f } };
HuVecF lbl_1_data_650[4] = { { 90.2000046f, 388.0f, 4500.0f }, { 220.400009f, 388.0f, 4500.0f }, { 355.600006f, 388.0f, 4500.0f }, { 485.800018f, 388.0f, 4500.0f } };
HuVecF lbl_1_data_680[4] = { { 1.0f, 1.0f, 1.0f }, { 0.75f, 0.75f, 0.75f }, { 0.5f, 0.5f, 0.5f }, { 0.3f, 0.3f, 0.3f } };
HuVecF lbl_1_data_6B0[4] = { { 1.0f, 1.1f, 1.0f }, { 0.9f, 1.0f, 0.9f }, { 0.8f, 0.9f, 0.8f }, { 0.7f, 0.8f, 0.7f } };
float lbl_1_data_6E0[4] = { 1.0f, 0.9f, 0.8f, 0.7f };
float lbl_1_data_6F0[4] = { 288.0f, 298.0f, 308.0f, 318.0f };
s32 lbl_1_data_700[32] = { 0, 1, 2, 3, 0, 0, 2, 3, 0, 0, 2, 2, 0, 0, 0, 3, 0, 1, 1, 3, 0, 1, 1, 1, 0, 1, 2, 2, 0, 0, 0, 0 }; /* unreferenced */
HuVecF lbl_1_data_780 = { 288.0f, 470.0f, 14000.0f };
float lbl_1_data_78C[4] = { 0.0f, 25.0f, 45.0f, 66.0f };
float lbl_1_data_79C[4] = { 0.0f, 24.0f, 44.0f, 66.0f };
void fn_1_7A18(void)
{
    s16 i;
    HUWINID winId;
    s16 statId;
    s16 comNum;
    BOOL endF;
    HUPROCESS *proc;

    endF = FALSE;
    proc = HuPrcCurrentGet();
    for(i = 0; i < 4; i++) {
        lbl_1_bss_1094[i] = 0;
    }
    fn_1_AC54();
    fn_1_47A8();
    memset(lbl_1_bss_10A4, 0, sizeof(lbl_1_bss_10A4));
    for(i = comNum = 0; i < 4; i++) {
        if(GwPlayerConf[i].type != GW_PLAYER_TYPE_MAN) {
            comNum++;
        }
    }
    if(comNum == 4) {
        lbl_1_bss_BA0 = TRUE;
    } else {
        lbl_1_bss_BA0 = FALSE;
    }
    fn_1_7E0C();
    HuPrcChildCreate(fn_1_7D34, 100, 0x2000, 0, proc);
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 20);
    HuAudSStreamPlay(26);
    while(WipeCheck()) {
        HuPrcVSleep();
    }
    HuDataDirClose(fn_1_C45C(DATA_result));
    HuPrcSleep(10);
    if(!lbl_1_bss_624) {
        HuPrcSleep(20);
    }
    fn_1_8858();
    fn_1_8D4C();
    fn_1_924C();
    fn_1_95D8();
    fn_1_3698();
    winId = HuWinCreate(-10000.0f, 400.0f, 320, 40, 0);
    HuWinMesSpeedSet(winId, 0);
    HuWinBGTPLvlSet(winId, 0.0f);
    HuWinPriSet(winId, 5);
    HuWinAttrSet(winId, HUWIN_ATTR_ALIGN_CENTER);
    if(!lbl_1_bss_BA0) {
        HuWinMesSet(winId, 0x10000);
    }
    HuPrcSleep(4);
    statId = fn_1_356C();
    while(1) {
        if(lbl_1_bss_BA0) {
            endF = TRUE;
        } else {
            for(i = 0; i < 4; i++) {
                if(HuPadStatGet(i) == 0 && (HuPadBtnDown[i] & PAD_BUTTON_A)) {
                    HuAudFXPlay(2);
                    endF = TRUE;
                    break;
                }
            }
        }
        if(endF) {
            break;
        }
        HuPrcVSleep();
    }
    HuWinKill(winId);
    fn_1_ACB8();
    fn_1_3640(statId);
    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
    HuAudSStreamAllFadeOut(1000);
    while(WipeCheck()) {
        HuPrcVSleep();
    }
    fn_1_86B0();
    fn_1_4694();
    omSysPauseEnable(TRUE);
    HuDataDirClose(fn_1_C45C(DATA_result));
    omOvlReturnEx(1, 1);
    HuPrcEnd();
}

void fn_1_7D34(void)
{
    HuVecF pos;
    s32 i;

    pos.z = 0.0f;
    pos.y = 0.0f;
    pos.x = 0.0f;
    while(1) {
        if(lbl_1_bss_BA0) {
            lbl_1_bss_624 = TRUE;
        } else {
            lbl_1_bss_624 = FALSE;
            for(i = 0; i < 4; i++) {
                if(HuPadStatGet(i) == 0 && (HuPadBtn[i] & PAD_BUTTON_A)) {
                    lbl_1_bss_624 = TRUE;
                }
            }
        }
        HuPrcVSleep();
    }
}

void fn_1_7E0C(void)
{
    s32 fileOfs;
    HuVecF pos3D;
    HuVecF pos2D;
    s32 id;
    s32 i;
    s32 charNo;
    s32 j;
    s32 i2;
    s32 languageNo;

    fn_1_9D18();
    languageNo = GWLanguageGet();
    if(languageNo == 1) {
        id = espEntry(fn_1_C45C(DATANUM(DATA_result, 6)), 180, 0);
        lbl_1_bss_1070 = id;
        espPosSet(id, 288.0, 80.0);
        espDrawNoSet(id, 64);
    } else {
        id = espEntry(fn_1_C45C(DATANUM(DATA_result, 8)), 180, 0);
        lbl_1_bss_1070 = id;
        espPosSet(id, 288.0, 80.0);
        espDrawNoSet(id, 64);
    }
    id = Hu3DModelCreate(HuDataSelHeapReadNum(fn_1_C45C(DATANUM(DATA_result, 24)), HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_106C = id;
    Hu3DModelCameraSet(id, HU3D_CAM0);
    Hu3D2Dto3D(&lbl_1_data_780, HU3D_CAM0, &pos3D);
    Hu3DModelPosSetV(id, &pos3D);
    Hu3DModelAttrSet(id, HU3D_MOTATTR_LOOP);
    Hu3DModelLayerSet(id, 0);
    for(i = 0; i < 4; i++) {
        i2 = i * 2;
        id = espEntry(fn_1_C45C(DATANUM(DATA_result, 2)), 150, 0);
        lbl_1_bss_FAC[i].esp0 = id;
        espPosSet(id, lbl_1_data_590[i2] + lbl_1_bss_1074[i], lbl_1_data_590[i2 + 1] + lbl_1_data_78C[lbl_1_bss_1094[i]]);
        espBankSet(id, lbl_1_bss_1094[i]);
        espDrawNoSet(id, 64);
        espScaleSet(id, 0.0, 0.0);
        id = espEntry(fn_1_C45C(DATANUM(DATA_result, 4)), 130, 1);
        lbl_1_bss_FAC[i].esp4 = id;
        espPosSet(id, lbl_1_data_5B0[i2] + lbl_1_bss_1074[i], lbl_1_data_5B0[i2 + 1] + lbl_1_data_78C[lbl_1_bss_1094[i]]);
        espDrawNoSet(id, 64);
        espScaleSet(id, 0.0, 0.0);
        espAttrSet(id, 4);
        for(j = 0; j < 3; j++) {
            if(j == 0) {
                id = espEntry(fn_1_C45C(DATANUM(DATA_result, 0)), 120, 10);
                lbl_1_bss_FAC[i].esp8[j] = id;
            } else {
                id = espEntry(fn_1_C45C(DATANUM(DATA_result, 0)), 120, 0);
                lbl_1_bss_FAC[i].esp8[j] = id;
            }
            espPosSet(id, lbl_1_bss_1074[i] + (lbl_1_data_5D0[i2] + j * 18), lbl_1_data_5D0[i2 + 1] + lbl_1_data_79C[lbl_1_bss_1094[i]]);
            espDrawNoSet(id, 64);
            espScaleSet(id, 0.0, 0.0);
            espAttrSet(id, 4);
        }
        charNo = GwPlayerConf[lbl_1_bss_1084[i]].charNo;
        fileOfs = charNo * 4;
        id = CharModelCreate(charNo, 2);
        lbl_1_bss_FAC[i].charMdl = id;
        lbl_1_bss_FAC[i].mot[0] = CharMotionCreate(charNo, DATANUM(DATA_mariomot, 0));
        lbl_1_bss_FAC[i].mot[1] = CharMotionCreate(charNo, DATANUM(DATA_mariomot, 0x24));
        lbl_1_bss_FAC[i].mot[2] = CharMotionCreate(charNo, DATANUM(DATA_mariomot, 0x25));
        lbl_1_bss_FAC[i].mot[3] = CharMotionCreate(charNo, DATANUM(DATA_mariomot, 3));
        lbl_1_bss_FAC[i].mot[4] = CharMotionCreate(charNo, DATANUM(DATA_mariomot, 4));
        CharMotionDataClose(charNo);
        Hu3DMotionShiftSet(id, lbl_1_bss_FAC[i].mot[3], 0.0f, 1.0f, HU3D_MOTATTR_NONE);
        Hu3DModelCameraSet(id, HU3D_CAM0);
        pos2D = lbl_1_data_5F0[i];
        pos2D.x += lbl_1_bss_1074[i];
        Hu3D2Dto3D(&pos2D, HU3D_CAM0, &pos3D);
        Hu3DModelPosSetV(id, &pos3D);
        Hu3DModelScaleSetV(id, &lbl_1_data_680[lbl_1_bss_1094[i]]);
        Hu3DModelLayerSet(id, 3);
        id = Hu3DModelCreateData(fn_1_C45C(DATANUM(DATA_result, 23)));
        lbl_1_bss_FAC[i].mdl14 = id;
        Hu3DModelCameraSet(id, HU3D_CAM0);
        pos2D = lbl_1_data_650[i];
        pos2D.x += lbl_1_bss_1074[i];
        Hu3D2Dto3D(&pos2D, HU3D_CAM0, &pos3D);
        Hu3DModelPosSetV(id, &pos3D);
        Hu3DModelScaleSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelLayerSet(id, 1);
    }
    for(i = 0; i < 128; i++) {
        id = Hu3DModelCreateData(fn_1_C45C(DATANUM(DATA_result, 26)));
        lbl_1_bss_DAC[i] = id;
        Hu3DModelScaleSet(id, 0.5f, 0.5f, 0.5f);
        Hu3DModelAttrSet(id, HU3D_ATTR_DISPOFF);
        Hu3DModelCameraSet(id, HU3D_CAM0);
        Hu3DModelLayerSet(id, 3);
    }
    lbl_1_bss_BA4 = HuSprAnimRead(HuDataSelHeapReadNum(fn_1_C45C(DATANUM(DATA_result, 21)), HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprExecLayerCameraSet(64, HU3D_CAM0, 2);
}

void fn_1_86B0(void)
{
    s32 charNo;
    s32 i;
    s32 j;

    espKill(lbl_1_bss_1070);
    Hu3DModelKill(lbl_1_bss_106C);
    for(i = 0; i < 4; i++) {
        espKill(lbl_1_bss_FAC[i].esp0);
        espKill(lbl_1_bss_FAC[i].esp4);
        for(j = 0; j < 3; j++) {
            espKill(lbl_1_bss_FAC[i].esp8[j]);
        }
        charNo = GwPlayerConf[lbl_1_bss_1084[i]].charNo;
        Hu3DModelKill(lbl_1_bss_FAC[i].mdl14);
    }
    for(i = 0; i < 128; i++) {
        Hu3DModelKill(lbl_1_bss_DAC[i]);
    }
    for(i = 0; i < 64; i++) {
        if(lbl_1_bss_10A4[i].modelId > 0) {
            fn_1_B39C(lbl_1_bss_10A4[i].modelId);
            lbl_1_bss_10A4[i].modelId = 0;
        }
    }
    CharModelKill(-1);
}

void fn_1_8858(void)
{
    HuVecF pos[4];
    HuVecF scale[4];
    s32 waitTime[4] = { 0, 30, 60, 90 };
    s32 doneF[4];
    float scaleSpeed[4];
    HuVecF pos3D;
    HuVecF pos2D;
    float speed;
    s32 i;
    s32 num;

    for(i = 0; i < 4; i++) {
        pos[i] = lbl_1_data_5F0[i];
        pos[i].x += lbl_1_bss_1074[i];
        doneF[i] = 0;
        scale[i] = lbl_1_data_6B0[lbl_1_bss_1094[i]];
        scale[i].x = 0.0f;
        scaleSpeed[i] = lbl_1_data_6B0[lbl_1_bss_1094[i]].x / 27.5f;
    }
    speed = (lbl_1_data_620[0].y - lbl_1_data_5F0[0].y) / 27.5f;
    while(1) {
        for(i = num = 0; i < 4; i++) {
            if(doneF[i] && Hu3DMotionEndCheck(lbl_1_bss_FAC[i].charMdl)) {
                Hu3DMotionShiftSet(lbl_1_bss_FAC[i].charMdl, lbl_1_bss_FAC[i].mot[0], 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
            }
            num += doneF[i];
            if(waitTime[i]-- <= 0 && !doneF[i]) {
                pos[i].y += speed;
                Hu3D2Dto3D(&pos[i], HU3D_CAM0, &pos3D);
                Hu3DModelPosSetV(lbl_1_bss_FAC[i].charMdl, &pos3D);
                scale[i].x += scaleSpeed[i];
                if(scale[i].x >= lbl_1_data_6B0[lbl_1_bss_1094[i]].x) {
                    scale[i].x = lbl_1_data_6B0[lbl_1_bss_1094[i]].x;
                }
                espScaleSet(lbl_1_bss_FAC[i].esp0, scale[i].x, scale[i].x);
                Hu3DModelScaleSetV(lbl_1_bss_FAC[i].mdl14, &scale[i]);
                if(pos[i].y >= lbl_1_data_620[i].y) {
                    pos2D = lbl_1_data_620[i];
                    pos2D.x += lbl_1_bss_1074[i];
                    Hu3D2Dto3D(&pos2D, HU3D_CAM0, &pos3D);
                    Hu3DModelPosSetV(lbl_1_bss_FAC[i].charMdl, &pos3D);
                    Hu3DMotionShiftSet(lbl_1_bss_FAC[i].charMdl, lbl_1_bss_FAC[i].mot[4], 0.0f, 8.0f, HU3D_MOTATTR_NONE);
                    doneF[i] = 1;
                }
            }
        }
        if(num >= 4) {
            break;
        }
        HuPrcVSleep();
    }
    HuPrcSleep(20);
}

void fn_1_8D4C(void)
{
    HuVecF pos[128];
    HuVecF rot[128];
    s32 playerNo[128];
    float ySpeed[128];
    s16 coinNum[5];
    s32 coinCnt[4];
    s32 waitTime[4];
    HuVecF pos3D;
    float speed;
    s32 i;
    s32 j;
    s32 total;

    total = 0;
    fn_1_3FB0(coinNum);
    lbl_1_bss_BA8 = coinNum[4];
    for(i = 0; i < 4; i++) {
        waitTime[i] = 0;
        coinCnt[i] = 0;
        total += coinNum[i];
    }
    for(i = 0; i < 128; i++) {
        ySpeed[i] = 0.0f;
        playerNo[i] = -1;
        Hu3DModelAttrSet(lbl_1_bss_DAC[i], HU3D_ATTR_DISPOFF);
    }
    while(1) {
        speed = (lbl_1_bss_624) ? 1.5 : 1.0;
        for(i = 0; i < 4; i++) {
            if(--waitTime[i] <= 0 && coinCnt[i] < coinNum[i]) {
                for(j = 0; j < 128; j++) {
                    if(Hu3DModelAttrGet(lbl_1_bss_DAC[j]) & HU3D_ATTR_DISPOFF) {
                        Hu3DModelAttrReset(lbl_1_bss_DAC[j], HU3D_ATTR_DISPOFF);
                        pos[j] = lbl_1_data_5F0[i];
                        pos[j].x += lbl_1_bss_1074[i];
                        Hu3DModelRotGet(lbl_1_bss_DAC[i], &rot[i]);
                        Hu3D2Dto3D(&pos[j], HU3D_CAM0, &pos3D);
                        Hu3DModelPosSetV(lbl_1_bss_DAC[j], &pos3D);
                        ySpeed[j] = 10.0f;
                        waitTime[i] = 6;
                        coinCnt[i]++;
                        playerNo[j] = i;
                        break;
                    }
                }
            }
        }
        for(i = 0; i < 128; i++) {
            if(!(Hu3DModelAttrGet(lbl_1_bss_DAC[i]) & HU3D_ATTR_DISPOFF)) {
                pos[i].y += speed * ySpeed[i];
                ySpeed[i] += 0.002f * speed;
                pos[i].z = 9000.0f;
                rot[i].y += 5.0f;
                Hu3D2Dto3D(&pos[i], HU3D_CAM0, &pos3D);
                Hu3DModelPosSetV(lbl_1_bss_DAC[i], &pos3D);
                Hu3DModelRotSetV(lbl_1_bss_DAC[i], &rot[i]);
                if(pos[i].y >= lbl_1_data_6F0[lbl_1_bss_1094[playerNo[i]]]) {
                    HuAudFXPlay(7);
                    fn_1_A238(&pos3D);
                    Hu3DModelAttrSet(lbl_1_bss_DAC[i], HU3D_ATTR_DISPOFF);
                    total--;
                }
            }
        }
        if(total <= 0) {
            break;
        }
        HuPrcVSleep();
    }
    for(i = 0; i < 128; i++) {
        Hu3DModelAttrSet(lbl_1_bss_DAC[i], HU3D_ATTR_DISPOFF);
    }
    HuPrcSleep(20);
}

void fn_1_924C(void)
{
    HuVecF pos[32];
    HuVecF rot[32];
    s32 waitTime[32];
    float ySpeed[32];
    HuVecF pos3D;
    float speed;
    s32 i;
    s32 num;

    num = lbl_1_bss_BA8;
    for(i = 0; i < lbl_1_bss_BA8; i++) {
        waitTime[i] = i * 8;
        Hu3DModelAttrReset(lbl_1_bss_DAC[i], HU3D_ATTR_DISPOFF);
        pos[i] = lbl_1_data_5F0[lbl_1_bss_61C];
        pos[i].x += lbl_1_bss_1074[lbl_1_bss_61C];
        Hu3D2Dto3D(&pos[i], HU3D_CAM0, &pos3D);
        Hu3DModelPosSetV(lbl_1_bss_DAC[i], &pos3D);
        ySpeed[i] = 10.0f;
    }
    while(1) {
        speed = (lbl_1_bss_624) ? 1.5 : 1.0;
        for(i = 0; i < lbl_1_bss_BA8; i++) {
            if(waitTime[i]-- <= 0 && !(Hu3DModelAttrGet(lbl_1_bss_DAC[i]) & HU3D_ATTR_DISPOFF)) {
                pos[i].y += speed * ySpeed[i];
                ySpeed[i] += 0.002f * speed;
                pos[i].z = 9000.0f;
                rot[i].y += 5.0f;
                Hu3D2Dto3D(&pos[i], HU3D_CAM0, &pos3D);
                Hu3DModelPosSetV(lbl_1_bss_DAC[i], &pos3D);
                Hu3DModelRotSetV(lbl_1_bss_DAC[i], &rot[i]);
                if(pos[i].y >= lbl_1_data_6F0[lbl_1_bss_1094[lbl_1_bss_61C]]) {
                    HuAudFXPlay(7);
                    fn_1_A238(&pos3D);
                    Hu3DModelAttrSet(lbl_1_bss_DAC[i], HU3D_ATTR_DISPOFF);
                    num--;
                }
            }
        }
        if(num <= 0) {
            break;
        }
        HuPrcVSleep();
    }
    HuPrcSleep(20);
}
float lbl_1_data_7AC[8][4] = {
    { 12, 22, 19, 5 },
    { 10, 24, 24, 8 },
    { 10, 21, 17, 4 },
    { 5, 14, 21, 12 },
    { 8, 17, 18, 9 },
    { 4, 8, 4, 2 },
    { 10, 18, 12, 2 },
    { 0, 0, 0, 0 }
};
GXColor lbl_1_data_82C[4] = {
    { 220, 220, 64, 120 },
    { 220, 220, 220, 120 },
    { 64, 160, 220, 140 },
    { 220, 140, 140, 140 }
};
u8 lbl_1_data_83C[16] = { 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 2, 3, 2, 3, 2, 3 };
u8 lbl_1_data_84C[36] = {
    2, 2, 2, 2, 2, 3, 3, 3, 2, 2, 2, 2, 2, 3, 3, 3,
    2, 2, 2, 2, 2, 3, 3, 3, 2, 3, 2, 3, 2, 3, 0, 1,
    0, 0, 0, 0
};

/* REL copy of the board particle system (board/effect.c) */


void fn_1_95D8(void)
{
    s16 coin[5];
    s16 i;
    s16 j;
    s16 time;
    s16 k;
    float scale;

    fn_1_3FB0(coin);
    for(i=0; i<4; i++) {
        if(i == lbl_1_bss_61C) {
            coin[i] += coin[4];
        }
    }
    for(i=0; i<4; i++) {
        for(j=0; j<3; j++) {
            espAttrReset(lbl_1_bss_FAC[i].esp8[j], 4);
        }
        fn_1_9B90(i, coin[i]);
        espAttrReset(lbl_1_bss_FAC[i].esp4, 4);
    }
    time = (lbl_1_bss_624) ? 3 : 10;
    for(k=0; k<=time; k++) {
        scale = HuSin((90.0/time)*k);
        for(i=0; i<4; i++) {
            espScaleSet(lbl_1_bss_FAC[i].esp4, scale, scale);
            for(j=0; j<3; j++) {
                espScaleSet(lbl_1_bss_FAC[i].esp8[j], scale, scale);
            }
        }
        HuPrcVSleep();
    }
    for(i=0; i<4; i++) {
        espScaleSet(lbl_1_bss_FAC[i].esp4, 1.0, 1.0);
        if(coin[i] != 0) {
            Hu3DMotionShiftSet(lbl_1_bss_FAC[i].charMdl, lbl_1_bss_FAC[i].mot[1], 0.0f, 8.0f, HU3D_MOTATTR_NONE);
        } else {
            Hu3DMotionShiftSet(lbl_1_bss_FAC[i].charMdl, lbl_1_bss_FAC[i].mot[2], 0.0f, 8.0f, HU3D_MOTATTR_NONE);
        }
    }
    HuPrcSleep(100);
    for(i=0; i<4; i++) {
        Hu3DMotionShiftSet(lbl_1_bss_FAC[i].charMdl, lbl_1_bss_FAC[i].mot[0], 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
    }
    for(k=0; k<=time; k++) {
        scale = HuCos((90.0/time)*k);
        for(i=0; i<4; i++) {
            espScaleSet(lbl_1_bss_FAC[i].esp4, scale, scale);
            for(j=0; j<3; j++) {
                espScaleSet(lbl_1_bss_FAC[i].esp8[j], scale, scale);
            }
        }
        HuPrcVSleep();
    }
    for(i=0; i<4; i++) {
        espAttrSet(lbl_1_bss_FAC[i].esp4, 4);
        for(j=0; j<3; j++) {
            espAttrSet(lbl_1_bss_FAC[i].esp8[j], 4);
        }
    }
}

void fn_1_9B90(s32 playerNo, s32 num)
{
    s32 digit[3];
    RESULT_BATTLE work = lbl_1_bss_FAC[playerNo];
    s32 rest;
    if(num > 999) {
        num = 999;
    }
    rest = num;
    digit[0] = num/100;
    if(digit[0] == 0) {
        espBankSet(work.esp8[0], 10);
    } else {
        espAttrReset(work.esp8[0], 4);
        espBankSet(work.esp8[0], digit[0]);
    }
    rest = num-(digit[0]*100);
    digit[1] = rest/10;
    if(digit[1] == 0 && digit[0] == 0) {
        espAttrSet(work.esp8[1], 4);
    } else {
        espAttrReset(work.esp8[1], 4);
        espBankSet(work.esp8[1], digit[1]);
    }
    rest -= digit[1]*10;
    espBankSet(work.esp8[2], rest);
}

void fn_1_9D18(void)
{
    s16 coin[4];
    s16 rankNo[4];
    s16 i;
    s16 j;
    s16 n;
    s16 rank;
    s16 rankCnt;
    s16 pattern;
    s16 temp;
    s16 coinNum;

    for(i=0; i<4; i++) {
        coinNum = GwPlayer[i].mgCoinBonus;
        coin[i] = coinNum;
        lbl_1_bss_1084[i] = i;
    }
    for(i=0; i<4; i++) {
        for(j=rank=0; j<4; j++) {
            if(i != j && coin[i] > coin[j]) {
                rank++;
            }
        }
        lbl_1_bss_1094[i] = rank;
    }
    for(n=1; n<4; n++) {
        for(i=0; i<4-n; i++) {
            if(coin[i] > coin[i+1]) {
                temp = coin[i];
                coin[i] = coin[i+1];
                coin[i+1] = temp;
                temp = lbl_1_bss_1094[i];
                lbl_1_bss_1094[i] = lbl_1_bss_1094[i+1];
                lbl_1_bss_1094[i+1] = temp;
                temp = lbl_1_bss_1084[i];
                lbl_1_bss_1084[i] = lbl_1_bss_1084[i+1];
                lbl_1_bss_1084[i+1] = temp;
            }
        }
    }
    rankNo[0] = 0;
    rankCnt = 0;
    for(i=1; i<4; i++) {
        if(coin[i] != coin[i-1]) {
            rankCnt++;
        }
        rankNo[i] = rankCnt;
    }
    if(rankNo[0] == 0 && rankNo[1] == 1 && rankNo[2] == 2 && rankNo[3] == 3) {
        pattern = 0;
    } else if(rankNo[0] == 0 && rankNo[1] == 0 && rankNo[2] == 1 && rankNo[3] == 2) {
        pattern = 1;
    } else if(rankNo[0] == 0 && rankNo[1] == 0 && rankNo[2] == 1 && rankNo[3] == 1) {
        pattern = 2;
    } else if(rankNo[0] == 0 && rankNo[1] == 0 && rankNo[2] == 0 && rankNo[3] == 1) {
        pattern = 3;
    } else if(rankNo[0] == 0 && rankNo[1] == 1 && rankNo[2] == 1 && rankNo[3] == 2) {
        pattern = 4;
    } else if(rankNo[0] == 0 && rankNo[1] == 1 && rankNo[2] == 1 && rankNo[3] == 1) {
        pattern = 5;
    } else if(rankNo[0] == 0 && rankNo[1] == 1 && rankNo[2] == 2 && rankNo[3] == 2) {
        pattern = 6;
    } else if(rankNo[0] == 0 && rankNo[1] == 0 && rankNo[2] == 0 && rankNo[3] == 0) {
        pattern = 7;
    }
    for(i=0; i<4; i++) {
        lbl_1_bss_1074[i] = lbl_1_data_7AC[pattern][i];
    }
}

static inline float ResultFAbs(register float x)
{
    asm {
        fabs x, x
    }
    return x;
}

void fn_1_A238(HuVecF *pos)
{
    RESULT_PARTICLEDATA *dataP;
    RESULT_EFFECT *effP;
    s32 i;
    RESULT_PARTICLE *particleP;
    s32 no;
    s32 colorNo;
    s32 num;
    float angle;
    float elev;
    s32 unused;
    HuVecF dir;

    no = -1;
    for(effP=lbl_1_bss_10A4, i=0; i<64; i++, effP++) {
        if(effP->modelId) {
            if(effP->time <= 40 && ResultFAbs(pos->x-effP->pos.x) < 300.0f && ResultFAbs(pos->y-effP->pos.y) < 300.0f && ResultFAbs(pos->z-effP->pos.z) < 300.0f) {
                no = i;
                break;
            }
        } else if(no < 0) {
            no = i;
        }
    }
    if(no < 0) {
        return;
    }
    effP = &lbl_1_bss_10A4[no];
    if(effP->modelId <= 0) {
        effP->modelId = fn_1_AD24(lbl_1_bss_BA4, 60);
        fn_1_C420(effP->modelId, fn_1_A928);
        Hu3DModelLayerSet(effP->modelId, 3);
        effP->pos.x = pos->x;
        effP->pos.y = pos->y;
        effP->pos.z = pos->z;
        Hu3DModelPosSetV(effP->modelId, pos);
        effP->time = 0;
        particleP = Hu3DData[effP->modelId].hookData;
        for(dataP=particleP->data, i=0; i<particleP->num; i++, dataP++) {
            dataP->scale = 0.0f;
            dataP->color.a = 0;
            dataP->time = 0;
        }
        particleP->blendMode = 1;
        particleP->unk02 = no;
    }
    unused = 0;
    particleP = Hu3DData[effP->modelId].hookData;
    PSVECSubtract(pos, &effP->pos, &dir);
    num = 20;
    effP->time += num;
    for(dataP=particleP->data, i=0; i<particleP->num && num; i++, dataP++) {
        if(dataP->time) {
            continue;
        }
        colorNo = lbl_1_data_83C[frandmod(16)];
        dataP->color.r = lbl_1_data_82C[colorNo].r+frandmod(20);
        dataP->color.g = lbl_1_data_82C[colorNo].g+frandmod(20);
        dataP->color.b = lbl_1_data_82C[colorNo].b+frandmod(20);
        dataP->color.a = lbl_1_data_82C[colorNo].a+frandmod(20);
        dataP->animBank = lbl_1_data_84C[frandmod(32)];
        dataP->pos.x = dataP->pos.y = dataP->pos.z = 0.0f;
        angle = 360.0f*frandf();
        elev = (1.7f*frandf())-0.7f;
        elev = 90.0f*(elev*ResultFAbs(elev));
        dataP->vel.x = HuSin(angle)*HuCos(elev);
        dataP->vel.y = HuSin(elev);
        dataP->vel.z = HuCos(angle)*HuCos(elev);
        PSVECScale(&dataP->vel, &dataP->pos, 100.0f*(0.5f*frandf()));
        PSVECScale(&dataP->vel, &dataP->vel, (1.0f/60.0f)*(300.0f+(100.0f*(4.0f*frandf()))));
        PSVECAdd(&dataP->pos, &dir, &dataP->pos);
        dataP->speedDecay = (1.0f/60.0f)*(100.0f*(0.7f*(0.2f+(0.3f*frandf()))));
        dataP->colorIdx = 90.0f+angle;
        dataP->scale = 70.0f*(0.5f+(0.5f*frandf()));
        dataP->accel.x = -1.3f;
        dataP->rot.z = 360.0f*frandf();
        dataP->scaleBase = (20.0f*frandf())-10.0f;
        dataP->time = frandmod(10)+20;
        dataP->accel.y = 0.44444448f;
        dataP->accel.z = 0.92f;
        num--;
    }
}

void fn_1_A928(HU3D_MODEL *modelP, RESULT_PARTICLE *particleP, Mtx mtx)
{
    RESULT_PARTICLEDATA *dataP;
    s32 count;
    s32 i;

    count = 0;
    for(dataP=particleP->data, i=0; i<particleP->num; i++, dataP++) {
        if(dataP->time == 0) {
            continue;
        }
        count++;
        PSVECScale(&dataP->vel, &dataP->vel, dataP->accel.z);
        PSVECAdd(&dataP->pos, &dataP->vel, &dataP->pos);
        dataP->vel.y += dataP->accel.y;
        dataP->vel.x += dataP->speedDecay*HuSin(dataP->colorIdx);
        dataP->vel.z += dataP->speedDecay*HuCos(dataP->colorIdx);
        dataP->colorIdx += 5.0f;
        dataP->speedDecay += 0.02777778f;
        dataP->scale += dataP->accel.x;
        dataP->rot.z += dataP->scaleBase;
        dataP->time--;
        if(dataP->time < 10) {
            dataP->color.a *= 0.8f;
            if(dataP->time == 0) {
                dataP->color.a = 0;
                dataP->scale = 0.0f;
                count--;
            }
        }
    }
    lbl_1_bss_10A4[particleP->unk02].time = count;
}

/* Effect: REL copy of board/effect.c (retail file boundary: .text 0xAB3C, .rodata 0x2B8, .data 0x870) */
