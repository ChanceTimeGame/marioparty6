#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mgtimer.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/seqman.h"
#include "string.h"
#include "math.h"
#include "dolphin.h"

#include "datadir_enum.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

void Hu3DMotionTimingHookReset(HU3D_MODELID modelId);

typedef struct M616Work_s {
    OMOBJMAN *objman; /* 0x000 */
    s16 state; /* 0x004 */
    u32 time; /* 0x008 */
    HU3D_MODELID camModel; /* 0x00C */
    HU3D_MOTIONID camMot[17]; /* 0x00E */
    OMOBJ *obj; /* 0x030 */
    float camTime; /* 0x034 */
    float camMaxTime; /* 0x038 */
    HU3D_MODELID stageModel[2]; /* 0x03C */
    HU3D_MODELID daiModel[4]; /* 0x040 */
    HU3D_MOTIONID daiMot[4][13]; /* 0x048 */
    HU3D_MODELID gearModel; /* 0x0B0 */
    HU3D_MOTIONID gearMot[3]; /* 0x0B2 */
    HU3D_MODELID nejiModel[4]; /* 0x0B8 */
    HU3D_MOTIONID nejiMot[6]; /* 0x0C0 */
    HU3D_MODELID baseModel; /* 0x0CC */
    HU3D_MODELID p1stModel; /* 0x0CE */
    s32 charNo[4]; /* 0x0D0 */
    s32 padNo[4]; /* 0x0E0 */
    HU3D_MODELID charModel[4]; /* 0x0F0 */
    HU3D_MOTIONID charMot[4][11]; /* 0x0F8 */
    u16 comChoice[4]; /* 0x150 */
    u32 comTime[4]; /* 0x158 */
    u16 btnOld[4]; /* 0x168 */
    s32 choice[4]; /* 0x170 */
    s32 score[4]; /* 0x180 */
    s32 round; /* 0x190 */
    MGTIMER *timer; /* 0x194 */
    s32 introStep; /* 0x198 */
    HU3D_MODELID effModel[4]; /* 0x19C */
    HuVecF shadowPos; /* 0x1A4 */
    HuVecF shadowTarget; /* 0x1B0 */
    HuVecF shadowUp; /* 0x1BC */
    s32 streamNo; /* 0x1C8 */
} M616WORK; /* size 0x1CC */

M616WORK lbl_1_bss_10;
HuVecF lbl_1_bss_0;

void fn_1_140(s16 mode, s16 frameNo);
void fn_1_160(s16 mode, s16 frameNo);
void fn_1_4BC(s16 mode, s16 frameNo);
void fn_1_4C0(s16 mode, s16 frameNo);
void fn_1_F14(s16 mode, s16 frameNo);
void fn_1_F78(s16 mode, s16 frameNo);
void fn_1_1284(s16 mode, s16 frameNo);
void fn_1_135C(s16 mode, s16 frameNo);
void fn_1_1360(s16 mode, s16 frameNo);
void fn_1_1364(OMOBJ *obj);
void fn_1_139C(HU3D_MODELID modelId, HU3D_MOTIONID motId, BOOL lagF);
void fn_1_1590(void);
void fn_1_1FF4(s32 no);
BOOL fn_1_20D8(void);
void fn_1_2104(s32 playerNo, s32 motNo);
void fn_1_215C(s32 playerNo, s32 motNo);
void fn_1_2254(s32 motNo);
void fn_1_22BC(s32 playerNo, s32 motNo, u32 attr);
void fn_1_23B0(void);
s32 fn_1_2580(s32 playerNo);

extern u32 lbl_1_data_1F0[17];
extern u32 lbl_1_data_234[6];
extern u32 lbl_1_data_24C[13];
extern unsigned int lbl_1_data_280[12];
extern s32 lbl_1_data_2B0[4][2];
extern u32 lbl_1_data_2D0[3];

MGSEQ_PARAM lbl_1_data_0 = {
    0,
    0,
    fn_1_140,
    fn_1_160,
    fn_1_4BC,
    fn_1_4C0,
    fn_1_F14,
    fn_1_F78,
    fn_1_1284,
    fn_1_135C,
    fn_1_1360
};

int _prolog(void)
{
    const VoidFunc *ctors = _ctors;
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    fn_1_1590();
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

s32 fn_1_A0(s32 streamNo, s32 bgmNo)
{
    s32 result = streamNo;
    if (result == -1 && (GameMesStatGet(MgSeqGameMesIdGet()) & GAMEMES_STAT_FXPLAY)) {
        result = HuAudBGMPlay(bgmNo);
    }
    return result;
}

void fn_1_104(s32 streamNo)
{
    if (streamNo != -1) {
        HuAudSStreamFadeOut(streamNo, 100);
    }
}

void fn_1_140(s16 mode, s16 frameNo)
{
    MgSeqModeNext();
}

void fn_1_160(s16 mode, s16 frameNo)
{
    s32 i;
    HU3D_MODELID modelId;
    HU3D_MODELID modelId2;
    Mtx mtx;
    HuVecF pos;
    HuVecF rot;
    HuVecF scale;
    HuVecF pos2;
    HuVecF rot2;
    HuVecF scale2;

    if (frameNo == 0) {
        for (i = 0; i < 4; i++) {
            fn_1_215C(i, 5);
            if (i != 0) {
                Hu3DModelHookSet(lbl_1_bss_10.nejiModel[i], "P1st", lbl_1_bss_10.charModel[i]);
                fn_1_22BC(i, 0, HU3D_MOTATTR_LOOP);
            }
            else {
                fn_1_22BC(i, 4, HU3D_MOTATTR_LOOP);
                Hu3DMotionTimeSet(lbl_1_bss_10.p1stModel, 0.0f);
                Hu3DMotionTimingHookSet(lbl_1_bss_10.p1stModel, fn_1_139C);
            }
        }
        fn_1_1FF4(0);
        lbl_1_bss_10.introStep = 0;
        MgSeqModeChangeOff();
        lbl_1_bss_10.streamNo = HuAudSStreamPlay(83);
    }
    modelId = lbl_1_bss_10.charModel[0];
    Hu3DModelObjMtxGet(lbl_1_bss_10.p1stModel, "P1stmov", mtx);
    Hu3DMtxTransGet(mtx, &pos);
    Hu3DMtxRotGet(mtx, &rot);
    Hu3DMtxScaleGet(mtx, &scale);
    Hu3DModelPosSetV(modelId, &pos);
    Hu3DModelRotSetV(modelId, &rot);
    Hu3DModelScaleSetV(modelId, &scale);
    OSReport("pos %f, %f, %f\n", pos.x, pos.y, pos.z);
    Hu3DModelObjPosGet(lbl_1_bss_10.p1stModel, "P1stmov", &lbl_1_bss_0);
    if (fn_1_20D8()) {
        Hu3DMotionTimingHookReset(lbl_1_bss_10.p1stModel);
        Hu3DModelHookReset(lbl_1_bss_10.p1stModel);
        Hu3DModelHookSet(lbl_1_bss_10.nejiModel[0], "P1st", lbl_1_bss_10.charModel[0]);
        modelId2 = lbl_1_bss_10.charModel[0];
        pos2.x = 0.0f;
        pos2.y = 0.0f;
        pos2.z = 0.0f;
        rot2.x = 0.0f;
        rot2.y = 0.0f;
        rot2.z = 0.0f;
        scale2.x = 1.0f;
        scale2.y = 1.0f;
        scale2.z = 1.0f;
        Hu3DModelPosSetV(modelId2, &pos2);
        Hu3DModelRotSetV(modelId2, &rot2);
        Hu3DModelScaleSetV(modelId2, &scale2);
        fn_1_1FF4(1);
        for (i = 0; i < 4; i++) {
            fn_1_22BC(i, 0, HU3D_MOTATTR_LOOP);
            lbl_1_bss_10.btnOld[i] = 0;
        }
        fn_1_23B0();
        lbl_1_bss_10.state = 1;
        lbl_1_bss_10.time = 0;
        MgSeqModeNext();
    }
}

void fn_1_4BC(s16 mode, s16 frameNo)
{
}

void fn_1_4C0(s16 mode, s16 frameNo)
{
    s32 win[4];
    s32 i;
    s32 choice;
    u16 btn;
    s32 j;
    HU3D_MODELID modelId;
    s32 winNum;
    s32 sameNum;
    HU3D_MOTIONID motId;

    switch (lbl_1_bss_10.state) {
        case 0:
            if (lbl_1_bss_10.time == 0) {
                for (i = 0; i < 4; i++) {
                    fn_1_22BC(i, 0, HU3D_MOTATTR_LOOP);
                    lbl_1_bss_10.btnOld[i] = 0;
                }
                fn_1_23B0();
            }
            if (lbl_1_bss_10.time >= 120) {
                lbl_1_bss_10.state = 1;
                lbl_1_bss_10.time = 0;
            }
            else {
                lbl_1_bss_10.time++;
            }
            break;
        case 1:
            for (i = 0; i < 4; i++) {
                fn_1_22BC(i, 9, HU3D_MOTATTR_LOOP);
                lbl_1_bss_10.choice[i] = 0;
                fn_1_215C(i, 5);
            }
            MgTimerParamSet(lbl_1_bss_10.timer, 300, 0, 0);
            MgTimerModeOnSet(lbl_1_bss_10.timer, 1);
            lbl_1_bss_10.state = 2;
            lbl_1_bss_10.time = 0;
            break;
        case 2:
            if (MgTimerDoneCheck(lbl_1_bss_10.timer)) {
                lbl_1_bss_10.state = 3;
                lbl_1_bss_10.time = 0;
                break;
            }
            for (i = 0; i < 4; i++) {
                choice = lbl_1_bss_10.choice[i];
                if (GwPlayerConf[i].type == 1) {
                    if (lbl_1_bss_10.time >= lbl_1_bss_10.comTime[i]) {
                        choice = fn_1_2580(i);
                    }
                    else {
                        choice = 0;
                    }
                }
                else {
                    btn = HuPadBtn[lbl_1_bss_10.padNo[i]];
                    btn &= 0x6300;
                    switch (btn & ~lbl_1_bss_10.btnOld[i]) {
                        case 0:
                            break;
                        case 0x100:
                            choice = 1;
                            break;
                        case 0x200:
                            choice = 2;
                            break;
                        case 0x4000:
                            choice = 3;
                            break;
                        case 0x2000:
                            choice = 4;
                            break;
                        default:
                            OSReport("\x93\xAF\x8E\x9E\x81\x49\x81\x49\x81\x49\n");
                            if ((btn & ~lbl_1_bss_10.btnOld[i]) & 0x100) {
                                choice = 1;
                            }
                            else if ((btn & ~lbl_1_bss_10.btnOld[i]) & 0x200) {
                                choice = 2;
                            }
                            else if ((btn & ~lbl_1_bss_10.btnOld[i]) & 0x4000) {
                                choice = 3;
                            }
                            else if ((btn & ~lbl_1_bss_10.btnOld[i]) & 0x2000) {
                                choice = 4;
                            }
                            break;
                    }
                    lbl_1_bss_10.btnOld[i] = btn;
                }
                if (choice != lbl_1_bss_10.choice[i]) {
                    fn_1_22BC(i, 10, 0);
                    omVibrate(i, 10, 7, 3);
                    fn_1_215C(i, 12);
                    HuAudFXPlay(1741);
                }
                lbl_1_bss_10.choice[i] = choice;
                motId = lbl_1_bss_10.charMot[i][10];
                modelId = lbl_1_bss_10.charModel[i];
                if (Hu3DMotionShiftIDGet(modelId) < 0 && motId == Hu3DMotionIDGet(modelId) && Hu3DMotionEndCheck(modelId)) {
                    fn_1_22BC(i, 9, 0);
                    OSReport("push end\n");
                }
            }
            lbl_1_bss_10.time++;
            break;
        case 3:
            if (lbl_1_bss_10.time == 0) {
                for (i = 0; i < 4; i++) {
                    fn_1_22BC(i, 0, HU3D_MOTATTR_LOOP);
                    {
                        s32 motTbl[5] = { 7, 0, 1, 2, 3 };
                        if (lbl_1_bss_10.choice[i] != 0) {
                            fn_1_215C(i, motTbl[lbl_1_bss_10.choice[i]]);
                        }
                    }
                }
            }
            else if (lbl_1_bss_10.time >= 60) {
                lbl_1_bss_10.state = 4;
                lbl_1_bss_10.time = 0;
                break;
            }
            lbl_1_bss_10.time++;
            break;
        case 4:
            winNum = 0;
            OSReport("entry ... %d, %d, %d, %d\n", lbl_1_bss_10.choice[0], lbl_1_bss_10.choice[1], lbl_1_bss_10.choice[2], lbl_1_bss_10.choice[3]);
            for (i = 0; i < 4; i++) {
                if (lbl_1_bss_10.choice[i] == 0) {
                    win[i] = 0;
                }
                else {
                    sameNum = 0;
                    for (j = 0; j < 4; j++) {
                        if (lbl_1_bss_10.choice[j] == lbl_1_bss_10.choice[i]) {
                            sameNum++;
                        }
                    }
                    if (sameNum == 1) {
                        win[i] = 1;
                    }
                    else {
                        win[i] = 0;
                    }
                }
                if (win[i] == 1) {
                    winNum++;
                }
            }
            if (winNum != 0) {
                for (i = 0; i < 4; i++) {
                    if (win[i] == 1) {
                        lbl_1_bss_10.score[i]++;
                        fn_1_22BC(i, 7, 0);
                        CharFXPlay(lbl_1_bss_10.charNo[i], 579);
                        fn_1_2104(i, lbl_1_bss_10.score[i]);
                        switch (lbl_1_bss_10.choice[i]) {
                            case 1:
                                fn_1_215C(i, 8);
                                break;
                            case 2:
                                fn_1_215C(i, 9);
                                break;
                            case 3:
                                fn_1_215C(i, 10);
                                break;
                            case 4:
                                fn_1_215C(i, 11);
                                break;
                        }
                    }
                    else {
                        fn_1_22BC(i, 8, 0);
                    }
                }
                fn_1_2254(0);
                HuAudFXPlay(1742);
                HuAudFXPlay(1743);
                lbl_1_bss_10.state = 5;
                lbl_1_bss_10.time = 0;
                break;
            }
            for (i = 0; i < 4; i++) {
                fn_1_22BC(i, 8, 0);
            }
            if (lbl_1_bss_10.round == 9) {
                lbl_1_bss_10.state = 8;
                lbl_1_bss_10.time = 0;
                break;
            }
            lbl_1_bss_10.round++;
            lbl_1_bss_10.state = 0;
            lbl_1_bss_10.time = 0;
            break;
        case 5:
            for (i = 0; i < 4; i++) {
                if (Hu3DMotionEndCheck(lbl_1_bss_10.charModel[i])) {
                    fn_1_22BC(i, 0, HU3D_MOTATTR_LOOP);
                }
            }
            for (i = 0; i < 4; i++) {
                if (!Hu3DMotionEndCheck(lbl_1_bss_10.nejiModel[i])) {
                    break;
                }
            }
            if (i == 4) {
                HuAudFXPlay(1745);
                HuAudFXPlay(1744);
                for (i = 0; i < 4; i++) {
                    if (lbl_1_bss_10.score[i] == 3) {
                        break;
                    }
                }
                if (i != 4 || lbl_1_bss_10.round == 9) {
                    lbl_1_bss_10.state = 8;
                    lbl_1_bss_10.time = 0;
                    break;
                }
                lbl_1_bss_10.round++;
                lbl_1_bss_10.state = 0;
                lbl_1_bss_10.time = 0;
                break;
            }
            lbl_1_bss_10.time++;
            break;
        case 8:
            MgSeqModeNext();
            break;
    }
}

void fn_1_F14(s16 mode, s16 frameNo)
{
    if (frameNo == 0) {
        fn_1_104(lbl_1_bss_10.streamNo);
        OSReport("enter sequence ... finish\n");
    }
}

void fn_1_F78(s16 mode, s16 frameNo)
{
    s32 i;
    s32 winNum = 0;
    s16 gearMot = -1;
    s16 winner[4] = { -1, -1, -1, -1 };
    BOOL noWinner = TRUE;

    if (frameNo == 0) {
        for (i = 0; i < 4; i++) {
            if (lbl_1_bss_10.score[i] == 3) {
                winner[winNum++] = lbl_1_bss_10.charNo[i];
                if (!_CheckFlag(0x1000F)) {
                    GwPlayer[i].mgCoinBonus = 10;
                }
                noWinner = FALSE;
            }
        }
        MgSeqWinnerSet(winner[0], winner[1], winner[2], winner[3]);
        if (!noWinner) {
            for (i = 0; i < 4; i++) {
                if (lbl_1_bss_10.score[i] == 1) {
                    fn_1_2104(i, 4);
                    if (gearMot != 1) {
                        gearMot = 1;
                    }
                }
                else if (lbl_1_bss_10.score[i] == 2) {
                    fn_1_2104(i, 5);
                    if (gearMot == -1) {
                        gearMot = 2;
                    }
                }
                fn_1_215C(i, 5);
            }
            if (gearMot != -1) {
                fn_1_2254(gearMot);
                HuAudFXPlay(1742);
                HuAudFXPlay(1743);
            }
            {
                s32 mask = 0;
                s32 camTbl[16] = { 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
                for (i = 0; i < 4; i++) {
                    if (lbl_1_bss_10.score[i] == 3) {
                        mask |= 1 << i;
                    }
                }
                fn_1_1FF4(camTbl[mask]);
            }
        }
        OSReport("enter sequence ... end\n");
    }
    else {
        for (i = 0; i < 4; i++) {
            if (!Hu3DMotionEndCheck(lbl_1_bss_10.nejiModel[i])) {
                break;
            }
        }
        if (i == 4) {
            HuAudFXPlay(1745);
            HuAudFXPlay(1744);
            MgSeqModeNext();
        }
    }
}

void fn_1_1284(s16 mode, s16 frameNo)
{
    s32 i;
    if (frameNo == 0) {
        for (i = 0; i < 4; i++) {
            if (lbl_1_bss_10.score[i] == 3) {
                fn_1_22BC(i, 2, 0);
                Hu3DMotionTimeSet(lbl_1_bss_10.effModel[i], 0.0f);
                Hu3DModelAttrReset(lbl_1_bss_10.effModel[i], HU3D_ATTR_DISPOFF);
            }
            else {
                fn_1_22BC(i, 3, 0);
            }
        }
        OSReport("enter sequence ... result\n");
    }
}

void fn_1_135C(s16 mode, s16 frameNo)
{
}

void fn_1_1360(s16 mode, s16 frameNo)
{
}

void fn_1_1364(OMOBJ *obj)
{
    lbl_1_bss_10.camTime = Hu3DMotionTimeGet(lbl_1_bss_10.camModel);
}

void fn_1_139C(HU3D_MODELID modelId, HU3D_MOTIONID motId, BOOL lagF)
{
    if (lagF == TRUE) {
        switch (lbl_1_bss_10.introStep) {
            case 0:
                fn_1_22BC(0, 1, 0);
                lbl_1_bss_10.introStep++;
                CharModelVoiceFlagSet(lbl_1_bss_10.charNo[0], FALSE);
                break;
            case 1:
                fn_1_22BC(0, 6, 0);
                lbl_1_bss_10.introStep++;
                break;
            case 2:
                fn_1_22BC(0, 4, HU3D_MOTATTR_LOOP);
                CharModelLandDustCreate(lbl_1_bss_10.charNo[0], &lbl_1_bss_0);
                lbl_1_bss_10.introStep++;
                CharModelVoiceFlagSet(lbl_1_bss_10.charNo[0], TRUE);
                break;
            case 3:
                CharModelVoiceFlagSet(lbl_1_bss_10.charNo[0], FALSE);
                fn_1_22BC(0, 5, 0);
                lbl_1_bss_10.introStep++;
                break;
            case 4:
                fn_1_22BC(0, 6, 0);
                lbl_1_bss_10.introStep++;
                break;
            case 5:
                CharModelVoiceFlagSet(lbl_1_bss_10.charNo[0], TRUE);
                fn_1_22BC(0, 0, 0);
                CharModelLandDustCreate(lbl_1_bss_10.charNo[0], &lbl_1_bss_0);
                lbl_1_bss_10.introStep++;
                break;
        }
    }
}

void fn_1_1590(void)
{
    s32 i;
    s32 j;
    HU3D_LIGHTID lightId;
    s32 charNo;
    HuVecF aim;
    HuVecF pos;

    memset(&lbl_1_bss_10, 0, sizeof(lbl_1_bss_10));
    lbl_1_bss_10.camModel = HU3D_MODELID_NONE;
    lbl_1_bss_10.objman = omInitObjMan(100, 0x2000);
    omGameSysInit(lbl_1_bss_10.objman);
    lbl_1_bss_10.shadowPos.x = 0.0f;
    lbl_1_bss_10.shadowPos.y = 3000.0f;
    lbl_1_bss_10.shadowPos.z = 0.0f;
    lbl_1_bss_10.shadowUp.x = 0.0f;
    lbl_1_bss_10.shadowUp.y = 0.0f;
    lbl_1_bss_10.shadowUp.z = 1.0f;
    lbl_1_bss_10.shadowTarget.x = 0.0f;
    lbl_1_bss_10.shadowTarget.y = -100.0f;
    lbl_1_bss_10.shadowTarget.z = 0.0f;
    Hu3DShadowCreate(30.0f, 1.0f, 13000.0f);
    Hu3DShadowPosSet(&lbl_1_bss_10.shadowPos, &lbl_1_bss_10.shadowUp, &lbl_1_bss_10.shadowTarget);
    Hu3DShadowColSet(50, 50, 50);
    Hu3DShadowSizeSet(192);
    Hu3DShadowTPLvlSet(0.7f);
    Hu3DCameraCreate(HU3D_CAM0);
    Hu3DCameraPerspectiveSet(HU3D_CAM0, 45.0f, 1.0f, 10000.0f, 1.2f);
    lbl_1_bss_10.obj = omAddObjEx(lbl_1_bss_10.objman, 0x3000, 0, 0, -1, fn_1_1364);
    for (i = 0; i < 17; i++) {
        lbl_1_bss_10.camMot[i] = Hu3DMotionCreate(HuDataSelHeapReadNum(lbl_1_data_1F0[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    lbl_1_bss_10.baseModel = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m616, 0x14), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelShadowSet(lbl_1_bss_10.baseModel);
    lbl_1_bss_10.p1stModel = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m616, 0x0C), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelShadowSet(lbl_1_bss_10.p1stModel);
    lbl_1_bss_10.effModel[0] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m616, 0x29), HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_10.effModel[1] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m616, 0x2A), HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_10.effModel[2] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m616, 0x2B), HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_10.effModel[3] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m616, 0x2C), HU_MEMNUM_OVL, HEAP_MODEL));
    for (i = 0; i < 4; i++) {
        Hu3DModelAttrSet(lbl_1_bss_10.effModel[i], HU3D_ATTR_DISPOFF);
    }
    lbl_1_bss_10.stageModel[0] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m616, 0x0D), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_10.stageModel[0], HU3D_MOTATTR_LOOP);
    Hu3DModelShadowMapObjSet(lbl_1_bss_10.stageModel[0], "616erande-stage01");
    lbl_1_bss_10.stageModel[1] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m616, 0x0E), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_10.stageModel[1], HU3D_MOTATTR_LOOP);
    Hu3DModelShadowMapObjSet(lbl_1_bss_10.stageModel[1], "616erande-stage001");
    for (i = 0; i < 4; i++) {
        lbl_1_bss_10.daiModel[i] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m616, 0x15), HU_MEMNUM_OVL, HEAP_MODEL));
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 13; j++) {
            lbl_1_bss_10.daiMot[i][j] = Hu3DJointMotion(lbl_1_bss_10.daiModel[i], HuDataSelHeapReadNum(lbl_1_data_24C[j], HU_MEMNUM_OVL, HEAP_MODEL));
        }
    }
    Hu3DModelHookSet(lbl_1_bss_10.baseModel, "p1daiiti", lbl_1_bss_10.daiModel[0]);
    Hu3DModelHookSet(lbl_1_bss_10.baseModel, "p2daiiti", lbl_1_bss_10.daiModel[1]);
    Hu3DModelHookSet(lbl_1_bss_10.baseModel, "p3daiiti", lbl_1_bss_10.daiModel[2]);
    Hu3DModelHookSet(lbl_1_bss_10.baseModel, "p4daiiti", lbl_1_bss_10.daiModel[3]);
    lbl_1_bss_10.gearModel = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m616, 0x0F), HU_MEMNUM_OVL, HEAP_MODEL));
    for (i = 0; i < 3; i++) {
        lbl_1_bss_10.gearMot[i] = Hu3DJointMotion(lbl_1_bss_10.gearModel, HuDataSelHeapReadNum(lbl_1_data_2D0[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    lbl_1_bss_10.nejiModel[0] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m616, 0x13), HU_MEMNUM_OVL, HEAP_MODEL));
    for (i = 1; i < 4; i++) {
        lbl_1_bss_10.nejiModel[i] = Hu3DModelLink(lbl_1_bss_10.nejiModel[0]);
    }
    for (i = 0; i < 6; i++) {
        lbl_1_bss_10.nejiMot[i] = Hu3DJointMotion(lbl_1_bss_10.nejiModel[0], HuDataSelHeapReadNum(lbl_1_data_234[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    for (i = 0; i < 4; i++) {
        Hu3DModelShadowMapObjSet(lbl_1_bss_10.nejiModel[i], "p1neji_sha");
    }
    Hu3DModelHookSet(lbl_1_bss_10.baseModel, "p1nejiiti", lbl_1_bss_10.nejiModel[0]);
    Hu3DModelHookSet(lbl_1_bss_10.baseModel, "p2nejiiti", lbl_1_bss_10.nejiModel[1]);
    Hu3DModelHookSet(lbl_1_bss_10.baseModel, "p3nejiiti", lbl_1_bss_10.nejiModel[2]);
    Hu3DModelHookSet(lbl_1_bss_10.baseModel, "p4nejiiti", lbl_1_bss_10.nejiModel[3]);
    fn_1_2104(0, 0);
    fn_1_2104(1, 0);
    fn_1_2104(2, 0);
    fn_1_2104(3, 0);
    {
        HuVecF lightPos = { 100, 800, 1000 };
        HuVecF lightDir = { 0.3f, -0.8f, 0.3f };
        HuVecF lightTarget = { 20, 45, 1000 };
        GXColor lightColor = { 255, 255, 255, 255 };
        lightId = Hu3DGLightCreateV(&lightPos, &lightDir, &lightColor);
        Hu3DGLightInfinitytSet(lightId);
        Hu3DGLightStaticSet(lightId, TRUE);
        aim.x = 0.0f;
    aim.y = 0.0f;
    aim.z = 0.0f;
    pos.x = 1.0f;
    pos.y = 7825.0f;
    pos.z = 9582.0f;
    Hu3DGLightPosAimSetV(lightId, &pos, &aim);
    }
    for (i = 0; i < 4; i++) {
        lbl_1_bss_10.padNo[i] = GwPlayerConf[i].padNo;
        charNo = lbl_1_bss_10.charNo[i] = GwPlayerConf[i].charNo;
        lbl_1_bss_10.padNo[i] = GwPlayerConf[i].padNo;
        lbl_1_bss_10.charModel[i] = CharModelMotListCreate(charNo, 4, lbl_1_data_280, lbl_1_bss_10.charMot[i]);
        Hu3DModelShadowSet(lbl_1_bss_10.charModel[i]);
        CharMotionVoiceOnSet(charNo, 36, FALSE);
        CharMotionVoiceOnSet(charNo, 37, FALSE);
    }
    lbl_1_bss_10.timer = MgTimerCreate(0);
    CharEffectLayerSet(3);
    fn_1_1FF4(1);
    lbl_1_bss_10.streamNo = -1;
    MgSeqCreate(&lbl_1_data_0);
}

void fn_1_1FF4(s32 no)
{
    HU3D_MODELID modelId;
    HU3D_MOTIONID motId;

    motId = lbl_1_bss_10.camMot[no];
    modelId = Hu3DModelCameraCreate(motId, HU3D_CAM0);
    Hu3DCameraMotionStart(modelId, HU3D_CAM0);
    lbl_1_bss_10.camTime = 0.0f;
    lbl_1_bss_10.camMaxTime = Hu3DMotionMaxTimeGet(modelId);
    if (lbl_1_bss_10.camModel != HU3D_MODELID_NONE) {
        Hu3DModelKill(lbl_1_bss_10.camModel);
    }
    lbl_1_bss_10.camModel = modelId;
    OSReport("start camera motion ... idx:%d time:%f\n", no, lbl_1_bss_10.camMaxTime);
}

BOOL fn_1_20D8(void)
{
    return Hu3DMotionEndCheck(lbl_1_bss_10.camModel);
}

void fn_1_2104(s32 playerNo, s32 motNo)
{
    Hu3DMotionSet(lbl_1_bss_10.nejiModel[playerNo], lbl_1_bss_10.nejiMot[motNo]);
}

void fn_1_215C(s32 playerNo, s32 motNo)
{
    if (motNo == 12 || lbl_1_bss_10.daiMot[playerNo][motNo] != Hu3DMotionIDGet(lbl_1_bss_10.daiModel[playerNo])) {
        Hu3DMotionSet(lbl_1_bss_10.daiModel[playerNo], lbl_1_bss_10.daiMot[playerNo][motNo]);
        Hu3DModelAttrSet(lbl_1_bss_10.daiModel[playerNo], HU3D_MOTATTR_LOOP);
        OSReport("base motion time ... %f\n", Hu3DMotionMaxTimeGet(lbl_1_bss_10.daiModel[playerNo]));
    }
}

void fn_1_2254(s32 motNo)
{
    Hu3DMotionSet(lbl_1_bss_10.gearModel, lbl_1_bss_10.gearMot[motNo]);
    OSReport("gear motion time ... %f\n", Hu3DMotionMaxTimeGet(lbl_1_bss_10.gearModel));
}

void fn_1_22BC(s32 playerNo, s32 motNo, u32 attr)
{
    float start = 0.0f;
    HU3D_MOTIONID motId = Hu3DMotionIDGet(lbl_1_bss_10.charModel[playerNo]);
    if (motNo == 10 || motId != lbl_1_bss_10.charMot[playerNo][motNo]) {
        CharMotionShiftSet(lbl_1_bss_10.charNo[playerNo], lbl_1_bss_10.charMot[playerNo][motNo], start, 8.0f, attr);
    }
}

void fn_1_23B0(void)
{
    s32 i;
    u16 btn;
    s32 *comDif;

    for (i = 0; i < 4; i++) {
        if (GwPlayerConf[i].type == 1) {
            comDif = lbl_1_data_2B0[GwPlayerConf[i].comDif];
            if (lbl_1_bss_10.round == 0) {
                do {
                    btn = frand() % 5;
                } while (btn == 0);
            }
            else if (frand() % 100 < comDif[1]) {
                btn = lbl_1_bss_10.comChoice[i];
            }
            else {
                do {
                    btn = frand() % 5;
                } while (btn == lbl_1_bss_10.comChoice[i] || btn == 0);
            }
            lbl_1_bss_10.comChoice[i] = btn;
            lbl_1_bss_10.comTime[i] = frand() % 270;
            OSReport("com %d : btn...%d time ...%d\n", i, lbl_1_bss_10.comChoice[i], lbl_1_bss_10.comTime[i]);
        }
    }
}

s32 fn_1_2580(s32 playerNo)
{
    return lbl_1_bss_10.comChoice[playerNo];
}

u32 lbl_1_data_1F0[17] = {
    DATANUM(DATA_m616, 0x00), DATANUM(DATA_m616, 0x01), DATANUM(DATA_m616, 0x02), DATANUM(DATA_m616, 0x03),
    DATANUM(DATA_m616, 0x06), DATANUM(DATA_m616, 0x04), DATANUM(DATA_m616, 0x07), DATANUM(DATA_m616, 0x08),
    DATANUM(DATA_m616, 0x07), DATANUM(DATA_m616, 0x05), DATANUM(DATA_m616, 0x0B), DATANUM(DATA_m616, 0x09),
    DATANUM(DATA_m616, 0x0B), DATANUM(DATA_m616, 0x0A), DATANUM(DATA_m616, 0x0B), DATANUM(DATA_m616, 0x09),
    DATANUM(DATA_m616, 0x0B)
};

u32 lbl_1_data_234[6] = {
    DATANUM(DATA_m616, 0x16), DATANUM(DATA_m616, 0x17), DATANUM(DATA_m616, 0x18),
    DATANUM(DATA_m616, 0x19), DATANUM(DATA_m616, 0x1A), DATANUM(DATA_m616, 0x1B)
};

u32 lbl_1_data_24C[13] = {
    DATANUM(DATA_m616, 0x1C), DATANUM(DATA_m616, 0x1D), DATANUM(DATA_m616, 0x1E), DATANUM(DATA_m616, 0x1F),
    DATANUM(DATA_m616, 0x21), DATANUM(DATA_m616, 0x20), DATANUM(DATA_m616, 0x22), DATANUM(DATA_m616, 0x23),
    DATANUM(DATA_m616, 0x24), DATANUM(DATA_m616, 0x25), DATANUM(DATA_m616, 0x26), DATANUM(DATA_m616, 0x27),
    DATANUM(DATA_m616, 0x28)
};

unsigned int lbl_1_data_280[12] = {
    DATANUM(DATA_mariomot, 0x00), DATANUM(DATA_mariomot, 0x03), DATANUM(DATA_mariomot, 0x06),
    DATANUM(DATA_mariomot, 0x28), DATANUM(DATA_mariomot, 0x6F), DATANUM(DATA_mariomot, 0x70),
    DATANUM(DATA_mariomot, 0x04), DATANUM(DATA_mariomot, 0x24), DATANUM(DATA_mariomot, 0x25),
    DATANUM(DATA_mario, 0x9A), DATANUM(DATA_mario, 0x9B), 0
};

s32 lbl_1_data_2B0[4][2] = {
    { 80, 50 },
    { 85, 30 },
    { 90, 10 },
    { 100, 0 }
};

u32 lbl_1_data_2D0[3] = { DATANUM(DATA_m616, 0x10), DATANUM(DATA_m616, 0x11), DATANUM(DATA_m616, 0x12) };
