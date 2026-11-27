#include "REL/m616dll/m616.h"
#include "math.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

HuVecF lbl_1_bss_0;

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
