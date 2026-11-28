#include "REL/m608dll/m608.h"
#include "humath.h"
#include "math.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* .bss is laid out in reverse definition order */
HuVecF lbl_1_bss_10[1320];
s32 lbl_1_bss_C;
HU3D_MODELID lbl_1_bss_8;
float lbl_1_bss_4;
float lbl_1_bss_0;

MGSEQ_PARAM lbl_1_data_0 = {
    0,
    1,
    fn_1_140,
    fn_1_190,
    fn_1_57C,
    fn_1_580,
    fn_1_1A2C,
    fn_1_1DD8,
    fn_1_2374,
    fn_1_2538,
    fn_1_253C,
};
float lbl_1_data_28[14] = { -35.0f, -35.0f, -35.0f, -25.0f, -35.0f, -35.0f, -35.0f, -5.0f, -35.0f, -10.0f, -5.0f, -10.0f, -10.0f, -10.0f };
float lbl_1_data_60[14] = { -10.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
float lbl_1_data_98[14] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.4f, 1.0f, 1.0f, 0.4f, 1.0f, 1.0f, 1.0f };

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
    lbl_1_bss_0 = 0.0f;
    lbl_1_bss_4 = 0.0f;
    MgSeqModeNext();
}

void fn_1_190(s16 mode, s16 frameNo)
{
    s32 i;
    s32 charNo;
    char *hookName;
    HuVecF pos;

    if (frameNo == 0) {
        fn_1_5E6C(0);
        for (i = 0; i < 4; i++) {
            charNo = lbl_1_bss_3E80.charNo[i];
            CharMotionSet(charNo, lbl_1_bss_3E80.charMot[i][0]);
            Hu3DModelAttrSet(lbl_1_bss_3E80.charModel[i], HU3D_MOTATTR_LOOP);
            Hu3DModelHookSet(lbl_1_bss_3E80.stModel[i], lbl_1_data_5D0[i], lbl_1_bss_3E80.charModel[i]);
            hookName = CharModelItemHookGet(lbl_1_bss_3E80.charNo[i], 4, 2);
            Hu3DModelHookSet(lbl_1_bss_3E80.charModel[i], hookName, lbl_1_bss_3E80.boardModel[i]);
        }
        Hu3DObjHookSet(lbl_1_bss_3E80.charModel[0], "ske_head", fn_1_38F0);
        Hu3DObjHookSet(lbl_1_bss_3E80.charModel[1], "ske_head", fn_1_39B4);
        Hu3DObjHookSet(lbl_1_bss_3E80.charModel[2], "ske_head", fn_1_3A78);
        Hu3DObjHookSet(lbl_1_bss_3E80.charModel[3], "ske_head", fn_1_3B3C);
        OSReport("1P neck ... %f\n", lbl_1_data_28[lbl_1_bss_3E80.charNo[0]]);
        OSReport("2P neck ... %f\n", lbl_1_data_28[lbl_1_bss_3E80.charNo[1]]);
        OSReport("3P neck ... %f\n", lbl_1_data_28[lbl_1_bss_3E80.charNo[2]]);
        OSReport("4P neck ... %f\n", lbl_1_data_28[lbl_1_bss_3E80.charNo[3]]);
        Hu3DObjHookSet(lbl_1_bss_3E80.boardModel[0], "snowboard_txt", fn_1_3F18);
        Hu3DObjHookSet(lbl_1_bss_3E80.boardModel[1], "snowboard_txt", fn_1_400C);
        Hu3DObjHookSet(lbl_1_bss_3E80.boardModel[2], "snowboard_txt", fn_1_40F0);
        Hu3DObjHookSet(lbl_1_bss_3E80.boardModel[3], "snowboard_txt", fn_1_41D4);
        fn_1_5FDC(3, 0);
        fn_1_5FDC(4, 2);
        if (lbl_1_bss_3E80.unk44C == 0) {
            fn_1_5FDC(5, 4);
        }
        fn_1_5FDC(10, 8);
        Hu3DModelObjPosGet(lbl_1_bss_3E80.stModel[1], "p2st", &pos);
        fn_1_5DA4(pos);
        lbl_1_bss_3E80.streamNo = HuAudSStreamPlay(79);
    } else if (fn_1_5F64()) {
        fn_1_614C();
        lbl_1_bss_3E80.playerNo = 0;
        lbl_1_bss_3E80.mode = 0;
        lbl_1_bss_3E80.time = 0;
        MgSeqModeNext();
    }
}

void fn_1_57C(s16 mode, s16 frameNo)
{
}

void fn_1_580(s16 mode, s16 frameNo)
{
    s32 playerNo;
    s32 model;
    s32 charNo;
    s32 i;
    s32 btn;
    s32 j;
    s32 hitF;
    s32 pitch;
    s32 pitch2;
    s32 seNo;
    HuVecF pos;
    HuVecF pos2;
    HuVecF pos3;
    HuVecF pos4;
    HuVecF pos5;
    HuVecF pos6;
    HuVecF pos7;
    HuVecF pos8;
    HuVecF pos9;
    HuVecF pos10;

    playerNo = lbl_1_bss_3E80.playerNo;
    model = lbl_1_bss_3E80.charModel[playerNo];
    charNo = lbl_1_bss_3E80.charNo[playerNo];
    if (frameNo == 0) {
        fn_1_6578();
    }
    Hu3DModelObjPosGet(lbl_1_bss_3E80.kuruModel, "608kurukuru-pmov_null", &pos);
    fn_1_38EC(pos);
    switch (lbl_1_bss_3E80.mode) {
        case 0:
            fn_1_5E6C(1);
            CharMotionSet(charNo, lbl_1_bss_3E80.charMot[playerNo][1]);
            Hu3DModelAttrReset(lbl_1_bss_3E80.charModel[playerNo], HU3D_MOTATTR_LOOP);
            Hu3DMotionSpeedSet(lbl_1_bss_3E80.charModel[playerNo], 0.5f);
            Hu3DAnimSpeedSet(lbl_1_bss_3E80.charModel[playerNo], 1.0f);
            Hu3DObjHookSet(lbl_1_bss_3E80.charModel[playerNo], "ske_head", fn_1_3C84);
            Hu3DObjHookSet(lbl_1_bss_3E80.boardModel[playerNo], "snowboard_txt", fn_1_3E00);
            Hu3DModelObjPosGet(lbl_1_bss_3E80.stModel[playerNo], "p1st", &pos2);
            fn_1_5DA4(pos2);
            lbl_1_bss_3E80.mode = 1;
            lbl_1_bss_3E80.time = 0;
            break;
        case 1:
            if (fn_1_5F64()) {
                Hu3DObjHookSet(lbl_1_bss_3E80.charModel[0], "ske_head", fn_1_3C00);
                Hu3DObjHookSet(lbl_1_bss_3E80.boardModel[0], "snowboard_txt", fn_1_3D7C);
                Hu3DMotionCalc(lbl_1_bss_3E80.charModel[0]);
                Hu3DMotionCalc(lbl_1_bss_3E80.boardModel[0]);
                fn_1_5E6C(3);
                lbl_1_bss_3E80.mode = 4;
                lbl_1_bss_3E80.time = 0;
            } else {
                omSetRot(lbl_1_bss_3E80.charObj[0], 0.0f, lbl_1_bss_0, 0.0f);
                lbl_1_bss_0 += 1.4f;
                lbl_1_bss_4 += lbl_1_data_98[charNo];
                lbl_1_bss_3E80.time++;
            }
            break;
        case 2:
            fn_1_5E6C(3);
            lbl_1_bss_3E80.mode = 4;
            lbl_1_bss_3E80.time = 0;
            break;
        case 3:
            if (lbl_1_bss_3E80.time == 0) {
                Hu3DModelObjPosGet(lbl_1_bss_3E80.stModel[0], "p1st", &pos3);
                fn_1_5DA4(pos3);
                Hu3DObjHookSet(lbl_1_bss_3E80.charModel[playerNo], "ske_head", fn_1_3C00);
                Hu3DObjHookSet(lbl_1_bss_3E80.boardModel[playerNo], "snowboard_txt", fn_1_3E00);
                omSetRot(lbl_1_bss_3E80.charObj[playerNo], 0.0f, 45.0f, 0.0f);
                CharMotionSet(charNo, lbl_1_bss_3E80.charMot[playerNo][2]);
                Hu3DMotionCalc(lbl_1_bss_3E80.charModel[playerNo]);
                fn_1_5F90(0);
                fn_1_5F90(1);
                fn_1_5F90(7);
                fn_1_5F90(8);
                fn_1_5F90(9);
                fn_1_5F90(5);
                fn_1_5FDC(3, 1);
                fn_1_5FDC(4, 3);
                if (lbl_1_bss_3E80.unk44C == playerNo) {
                    fn_1_5FDC(5, 5);
                }
                fn_1_5FDC(10, 8);
                fn_1_605C(9);
                CharMotionShiftSet(charNo, lbl_1_bss_3E80.charMot[playerNo][2], 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
                fn_1_5E6C(3);
                lbl_1_bss_3E80.mode = 4;
                lbl_1_bss_3E80.time = 0;
            } else {
                lbl_1_bss_3E80.time++;
            }
            break;
        case 4:
            if (lbl_1_bss_3E80.time == 0) {
                omSetRot(lbl_1_bss_3E80.charObj[playerNo], 0.0f, 55.0f, 0.0f);
                Hu3DObjHookSet(lbl_1_bss_3E80.charModel[playerNo], "ske_head", fn_1_3C00);
                Hu3DObjHookSet(lbl_1_bss_3E80.boardModel[playerNo], "snowboard_txt", fn_1_3D7C);
                CharMotionShiftSet(charNo, lbl_1_bss_3E80.charMot[playerNo][2], 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
                omVibrate(playerNo, 30, 7, 3);
                for (i = 0; i < 27; i++) {
                    Hu3DModelAttrSet(lbl_1_bss_3E80.unk1C0[i], HU3D_ATTR_DISPOFF);
                }
                lbl_1_bss_3E80.jumpState = 0;
                lbl_1_bss_3E80.rotY = 0.0f;
                lbl_1_bss_3E80.rotYStart = 0.0f;
                lbl_1_bss_3E80.rotYEnd = 0.0f;
                lbl_1_bss_3E80.rotTime = 0;
                lbl_1_bss_3E80.unk3DC = 0;
                lbl_1_bss_3E80.score[lbl_1_bss_3E80.playerNo] = 0;
                Hu3DModelObjPosGet(lbl_1_bss_3E80.stModel[0], "p1st", &pos4);
                fn_1_5DA4(pos4);
                Hu3DLayerHookSet(3, fn_1_437C);
                OSReport("*** wait vibration start\n");
            } else if (lbl_1_bss_3E80.time >= 60) {
                Hu3DModelHookReset(lbl_1_bss_3E80.stModel[0]);
                Hu3DModelHookSet(lbl_1_bss_3E80.kuruModel, "608kurukuru-pmov_null", model);
                Hu3DMotionSet(lbl_1_bss_3E80.kuruModel, lbl_1_bss_3E80.kuruMot);
                Hu3DModelAttrSet(lbl_1_bss_3E80.kuruModel, HU3D_MOTATTR_PAUSE);
                omSetRot(lbl_1_bss_3E80.charObj[playerNo], 0.0f, 0.0f, 0.0f);
                Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
                Hu3DMotionCalc(lbl_1_bss_3E80.kuruModel);
                Hu3DMotionCalc(model);
                Hu3DMotionCalc(lbl_1_bss_3E80.stModel[0]);
                Hu3DMotionCalc(lbl_1_bss_3E80.boardModel[playerNo]);
                Hu3DMotionTimingHookSet(lbl_1_bss_3E80.unk1A8, NULL);
                Hu3DMotionTimingHookSet(lbl_1_bss_3E80.unk1AA, NULL);
                if (playerNo < 3) {
                    Hu3DMotionTimingHookSet(lbl_1_bss_3E80.unk1A8, fn_1_2FD8);
                    Hu3DMotionTimeSet(lbl_1_bss_3E80.unk1A8, 0.0f);
                } else {
                    Hu3DMotionTimingHookSet(lbl_1_bss_3E80.unk1AA, fn_1_2FD8);
                    Hu3DMotionTimeSet(lbl_1_bss_3E80.unk1AA, 0.0f);
                }
                lbl_1_bss_3E80.jumpState = 0;
                lbl_1_bss_3E80.mode = 5;
                lbl_1_bss_3E80.time = 0;
                Hu3DModelObjPosGet(lbl_1_bss_3E80.stModel[0], "p1st", &pos5);
                fn_1_5DA4(pos5);
                OSReport("*** wait vibration end\n");
                break;
            } else {
                Hu3DModelObjPosGet(lbl_1_bss_3E80.stModel[0], "p1st", &pos6);
                fn_1_5DA4(pos6);
            }
            lbl_1_bss_3E80.time++;
            break;
        case 5:
            if (lbl_1_bss_3E80.time == 0) {
                lbl_1_bss_C = 0;
                lbl_1_bss_8 = Hu3DHookFuncCreate(fn_1_38E8);
                Hu3DModelLayerSet(lbl_1_bss_8, 1);
                omSetRot(lbl_1_bss_3E80.charObj[playerNo], 0.0f, 0.0f, 0.0f);
                CharMotionShiftSet(charNo, lbl_1_bss_3E80.charMot[playerNo][2], 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
                if (playerNo == 0) {
                    fn_1_605C(8);
                }
                omSetRot(lbl_1_bss_3E80.charObj[playerNo], 0.0f, 0.0f, 0.0f);
                Hu3DModelAttrReset(lbl_1_bss_3E80.kuruModel, HU3D_MOTATTR_PAUSE);
                HuAudFXPlay(1628);
                OSReport("*** jump start\n");
            }
            if (lbl_1_bss_3E80.time == 19) {
                lbl_1_bss_3E80.seNo = HuAudFXPlay(1623);
                HuAudFXPitchSet(lbl_1_bss_3E80.seNo, 0);
            }
            Hu3DModelObjPosGet(lbl_1_bss_3E80.kuruModel, "608kurukuru-pmov_null", &pos7);
            fn_1_5DA4(pos7);
            if (lbl_1_bss_3E80.time >= 19) {
                pitch = ((lbl_1_bss_3E80.time - 19) * 8191) / 46;
                if (pitch >= 8191) {
                    pitch = 8191;
                }
                HuAudFXPitchSet(lbl_1_bss_3E80.seNo, pitch);
            }
            fn_1_6958(0);
            lbl_1_bss_3E80.time++;
            break;
        case 6:
            btn = 0;
            hitF = FALSE;
            if (GwPlayerConf[playerNo].type == 1) {
                btn = fn_1_6B90(playerNo);
            } else {
                btn = HuPadBtnRep[lbl_1_bss_3E80.padNo[playerNo]];
            }
            Hu3DModelObjPosGet(lbl_1_bss_3E80.kuruModel, "608kurukuru-pmov_null", &pos8);
            fn_1_5DA4(pos8);
            btn &= (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_X | PAD_BUTTON_Y);
            switch (lbl_1_bss_3E80.score[playerNo] % 4) {
                case 0:
                    if (btn == PAD_BUTTON_A) {
                        hitF = TRUE;
                    }
                    break;
                case 1:
                    if (btn == PAD_BUTTON_B) {
                        hitF = TRUE;
                    }
                    break;
                case 2:
                    if (btn == PAD_BUTTON_Y) {
                        hitF = TRUE;
                    }
                    break;
                case 3:
                    if (btn == PAD_BUTTON_X) {
                        hitF = TRUE;
                    }
                    break;
            }
            fn_1_6958((lbl_1_bss_3E80.score[playerNo] % 4) + 1);
            if (hitF) {
                if (lbl_1_bss_3E80.unk3DC < 27) {
                    lbl_1_bss_3E80.score[playerNo]++;
                    if ((lbl_1_bss_3E80.score[playerNo] % 4) == 0) {
                        Hu3DModelObjPosGet(lbl_1_bss_3E80.kuruModel, "pmov", &lbl_1_bss_3E80.unk22C[lbl_1_bss_3E80.unk3DC]);
                        lbl_1_bss_3E80.unk370[lbl_1_bss_3E80.unk3DC]->work[1] = 30;
                        lbl_1_bss_3E80.unk3DC++;
                        pitch2 = (lbl_1_bss_3E80.unk3DC - 1) * 341;
                        if (pitch2 >= 8192) {
                            pitch2 = 8191;
                        }
                        seNo = HuAudFXPlay(1626);
                        HuAudFXPitchSet(seNo, pitch2);
                    }
                    fn_1_6534(playerNo, lbl_1_bss_3E80.score[playerNo] * 90);
                    Hu3DMotionTimeSet(lbl_1_bss_3E80.unk170, 0.0f);
                }
                lbl_1_bss_3E80.rotYStart = lbl_1_bss_3E80.rotYEnd;
                lbl_1_bss_3E80.rotYEnd = lbl_1_bss_3E80.rotYStart - 90.0f;
                lbl_1_bss_3E80.rotY = lbl_1_bss_3E80.rotYStart;
                lbl_1_bss_3E80.rotTime = 0;
                HuAudFXPlay(1625);
            }
            fn_1_42B8();
            lbl_1_bss_3E80.time++;
            break;
        case 7:
            Hu3DModelObjPosGet(lbl_1_bss_3E80.kuruModel, "608kurukuru-pmov_null", &pos9);
            fn_1_5DA4(pos9);
            if (lbl_1_bss_3E80.time == 0) {
                Hu3DModelHookReset(lbl_1_bss_3E80.boardModel[playerNo]);
                fn_1_6958(-1);
                omVibrate(playerNo, 30, 7, 3);
                HuAudFXPlay(1627);
                lbl_1_bss_3E80.seNo = HuAudFXPlay(1623);
                HuAudFXPitchSet(lbl_1_bss_3E80.seNo, 4096);
            }
            if (playerNo == 3) {
                if (lbl_1_bss_3E80.camMaxTime - lbl_1_bss_3E80.camTime <= 240.0f) {
                    MgSeqModeNext();
                }
            } else if (lbl_1_bss_3E80.camMaxTime - lbl_1_bss_3E80.camTime <= 60.0f) {
                lbl_1_bss_3E80.mode = 8;
                lbl_1_bss_3E80.time = 0;
                WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
                HuAudFXFadeOut(lbl_1_bss_3E80.seNo, 1000);
                break;
            }
            fn_1_42B8();
            lbl_1_bss_3E80.time++;
            break;
        case 8:
            if (lbl_1_bss_3E80.time == 0) {
                Hu3DLayerHookReset(3);
                Hu3DModelKill(lbl_1_bss_8);
            }
            if (lbl_1_bss_3E80.time >= 60) {
                Hu3DModelHookReset(model);
                Hu3DModelHookSet(lbl_1_bss_3E80.lsModel[playerNo], lbl_1_data_624[playerNo], model);
                CharMotionSet(charNo, lbl_1_bss_3E80.charMot[playerNo][5]);
                omSetRot(lbl_1_bss_3E80.charObj[playerNo], 0.0f, -90.0f, 0.0f);
                Hu3DModelHookReset(model);
                Hu3DModelHookSet(lbl_1_bss_3E80.lsModel[playerNo], lbl_1_data_684[playerNo], lbl_1_bss_3E80.boardModel[playerNo]);
                for (i = 0; i < 4; i++) {
                    Hu3DModelHookReset(lbl_1_bss_3E80.stModel[i]);
                }
                for (j = 0, i = playerNo + 1; i < 4; j++, i++) {
                    if (j == 0) {
                        omSetRot(lbl_1_bss_3E80.charObj[i], 0.0f, 90.0f, 0.0f);
                    }
                    Hu3DModelHookSet(lbl_1_bss_3E80.stModel[j], lbl_1_data_5D0[j], lbl_1_bss_3E80.charModel[i]);
                }
                Hu3DModelObjPosGet(lbl_1_bss_3E80.kuruModel, "608kurukuru-pmov_null", &pos10);
                fn_1_5DA4(pos10);
                if (playerNo < 3) {
                    fn_1_5E6C(2);
                    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_PREV, 60);
                    lbl_1_bss_3E80.playerNo++;
                    Hu3DObjHookSet(lbl_1_bss_3E80.boardModel[lbl_1_bss_3E80.playerNo], "snowboard_txt", fn_1_3D7C);
                    lbl_1_bss_3E80.mode = 3;
                    lbl_1_bss_3E80.time = 0;
                    break;
                }
                MgSeqModeNext();
            }
            fn_1_42B8();
            lbl_1_bss_3E80.time++;
            break;
    }
    Hu3DMotionCalc(lbl_1_bss_3E80.kuruModel);
    Hu3DMotionCalc(model);
    Hu3DMotionCalc(lbl_1_bss_3E80.boardModel[playerNo]);
}

void fn_1_1A2C(s16 mode, s16 frameNo)
{
    s32 i;
    s16 maxScore;
    s32 winNum;
    s32 humanF;
    s16 winner[4];
    HuVecF pos;

    if (frameNo == 0) {
        if (lbl_1_bss_3E80.score[0] == 0 && lbl_1_bss_3E80.score[1] == 0 && lbl_1_bss_3E80.score[2] == 0 && lbl_1_bss_3E80.score[3] == 0) {
            lbl_1_bss_3E80.winF[0] = lbl_1_bss_3E80.winF[1] = lbl_1_bss_3E80.winF[2] = lbl_1_bss_3E80.winF[3] = 0;
            lbl_1_bss_3E80.newRecordF = 0;
            winner[0] = winner[1] = winner[2] = winner[3] = -1;
        } else {
            maxScore = 0;
            winNum = 0;
            humanF = FALSE;
            for (i = 0; i < 4; i++) {
                if (maxScore < lbl_1_bss_3E80.score[i]) {
                    maxScore = lbl_1_bss_3E80.score[i];
                }
            }
            for (i = 0; i < 4; i++) {
                if (lbl_1_bss_3E80.score[i] == maxScore) {
                    winner[winNum++] = i;
                    lbl_1_bss_3E80.winF[i] = TRUE;
                    if (GwPlayerConf[i].type == 0) {
                        humanF = TRUE;
                    }
                } else {
                    lbl_1_bss_3E80.winF[i] = FALSE;
                }
            }
            for (; winNum < 4; winNum++) {
                winner[winNum] = -1;
            }
            if (humanF && maxScore > lbl_1_bss_3E80.record / 90) {
                OSReport("*** new record ... %d -> %d\n", maxScore, lbl_1_bss_3E80.record);
                lbl_1_bss_3E80.newRecordF = TRUE;
                lbl_1_bss_3E80.record = maxScore;
                MgSeqRecordSet(maxScore * 90);
            }
        }
        MgSeqWinnerSet(winner[0] >= 0 ? lbl_1_bss_3E80.charNo[winner[0]] : -1, winner[1] >= 0 ? lbl_1_bss_3E80.charNo[winner[1]] : -1,
            winner[2] >= 0 ? lbl_1_bss_3E80.charNo[winner[2]] : -1, winner[3] >= 0 ? lbl_1_bss_3E80.charNo[winner[3]] : -1);
        fn_1_104(lbl_1_bss_3E80.streamNo);
    }
    Hu3DModelObjPosGet(lbl_1_bss_3E80.kuruModel, "608kurukuru-pmov_null", &pos);
    fn_1_5DA4(pos);
}

void fn_1_1DD8(s16 mode, s16 frameNo)
{
    s32 i;
    s32 playerNo;
    s32 model;
    s32 charNo;
    s32 score;
    OMOBJ *obj;
    HuVecF pos;
    HuVecF pos2;

    playerNo = lbl_1_bss_3E80.playerNo;
    model = lbl_1_bss_3E80.charModel[playerNo];
    charNo = lbl_1_bss_3E80.charNo[playerNo];
    if (frameNo == 0 && _CheckFlag(0x30002)) {
        for (i = 0; i < 4; i++) {
            score = lbl_1_bss_3E80.score[i] * 90;
            GwPlayer[i].mgScore = score;
        }
        MgSeqModeSet(9);
    } else if (frameNo == 0) {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
        HuAudFXFadeOut(lbl_1_bss_3E80.seNo, 1000);
        lbl_1_bss_3E80.time = 0;
        for (i = 0; i < 60; i++) {
            Hu3DModelObjPosGet(lbl_1_bss_3E80.kuruModel, "608kurukuru-pmov_null", &pos);
            fn_1_5DA4(pos);
            HuPrcVSleep();
        }
        Hu3DModelAttrSet(lbl_1_bss_3E80.unk1A8, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(lbl_1_bss_3E80.unk1AA, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(lbl_1_bss_3E80.mdlId[5], HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(lbl_1_bss_3E80.kuruModel, HU3D_ATTR_DISPOFF);
        Hu3DModelHookReset(lbl_1_bss_3E80.kuruModel);
        Hu3DModelHookReset(model);
        Hu3DModelHookSet(lbl_1_bss_3E80.lsModel[playerNo], lbl_1_data_624[playerNo], model);
        CharMotionSet(charNo, lbl_1_bss_3E80.charMot[playerNo][5]);
        omSetRot(lbl_1_bss_3E80.charObj[playerNo], 0.0f, -90.0f, 0.0f);
        Hu3DModelHookReset(model);
        Hu3DModelHookSet(lbl_1_bss_3E80.lsModel[playerNo], lbl_1_data_684[playerNo], lbl_1_bss_3E80.boardModel[playerNo]);
        fn_1_5E6C(5);
        for (i = 0; i < 4; i++) {
            charNo = lbl_1_bss_3E80.charNo[i] = GwPlayerConf[i].charNo;
            Hu3DModelHookReset(lbl_1_bss_3E80.lsModel[i]);
            Hu3DModelHookSet(lbl_1_bss_3E80.lsModel[i], lbl_1_data_684[i], lbl_1_bss_3E80.boardModel[playerNo]);
            if (lbl_1_bss_3E80.winF[i] == TRUE) {
                Hu3DModelHookSet(lbl_1_bss_3E80.lsBModel[i], lbl_1_data_64C[i], lbl_1_bss_3E80.charModel[i]);
                if (!_CheckFlag(0x1000F)) {
                    GwPlayer[i].mgCoinBonus = 10;
                }
            } else {
                Hu3DModelHookSet(lbl_1_bss_3E80.lsModel[i], lbl_1_data_624[i], lbl_1_bss_3E80.charModel[i]);
            }
            omSetRot(lbl_1_bss_3E80.charObj[i], 0.0f, -90.0f, 0.0f);
            CharMotionSet(charNo, lbl_1_bss_3E80.charMot[i][5]);
            Hu3DModelAttrSet(lbl_1_bss_3E80.charModel[i], HU3D_MOTATTR_LOOP);
        }
        if (!_CheckFlag(0x1000F) && lbl_1_bss_3E80.newRecordF) {
            obj = omAddObjEx(lbl_1_bss_3E80.objman, 16, 0, 0, -1, fn_1_66C0);
            *(float *)&obj->work[0] = 1.0f;
            obj->work[1] = 1;
            obj->work[2] = 0;
        }
        Hu3DModelObjPosGet(lbl_1_bss_3E80.lsModel[1], "p2ls", &pos2);
        fn_1_5DA4(pos2);
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_PREV, 60);
        lbl_1_bss_3E80.time = 0;
        for (i = 0; i < 60; i++) {
            HuPrcVSleep();
        }
        if (!lbl_1_bss_3E80.newRecordF || _CheckFlag(0x1000F)) {
            MgSeqModeNext();
        }
    }
}

void fn_1_2374(s16 mode, s16 frameNo)
{
    s32 i;
    s32 charNo;
    s32 winNum;
    s32 winPlayer;

    winNum = 0;
    if (frameNo == 0) {
        OSReport("###############################################################\n");
        OSReport("###############################################################\n");
        OSReport("###############################################################\n");
        for (i = 0; i < 4; i++) {
            charNo = lbl_1_bss_3E80.charNo[i] = GwPlayerConf[i].charNo;
            CharMotionSet(charNo, lbl_1_bss_3E80.charMot[i][7]);
        }
        for (i = 0; i < 4; i++) {
            charNo = lbl_1_bss_3E80.charNo[i] = GwPlayerConf[i].charNo;
            if (lbl_1_bss_3E80.winF[i] == TRUE) {
                CharMotionShiftSet(charNo, lbl_1_bss_3E80.charMot[i][6], 0.0f, 0.5f, HU3D_MOTATTR_NONE);
                winNum++;
                winPlayer = i;
            } else {
                CharMotionShiftSet(charNo, lbl_1_bss_3E80.charMot[i][7], 0.0f, 0.5f, HU3D_MOTATTR_NONE);
            }
        }
        if (winNum == 1) {
            fn_1_5E6C(winPlayer + 6);
        }
    }
}

void fn_1_2538(s16 mode, s16 frameNo)
{
}

void fn_1_253C(s16 mode, s16 frameNo)
{
}

/* MSL's sqrtf with plain constants (0.5 and 3.0 sit in this unit's .rodata pool) */
static inline float M608Sqrtf(float x)
{
    const double _half = .5;
    const double _three = 3.0;
    volatile float y;
    if (x > 0.0f) {
        double guess = __frsqrte((double)x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        y = (float)(x * guess);
        return y;
    }
    return x;
}

void fn_1_2540(OMOBJ *obj)
{
    HuVecF pos;
    HuVecF target;
    HuVecF up;
    HuVecF dir;
    HuVecF camPos;
    HuVecF camTarget;
    HuVecF newUp;
    HuVecF camUp;
    HuVecF axis;
    float rotX;
    float rotY;
    float rotZ;

    lbl_1_bss_3E80.camTime = Hu3DMotionTimeGet(lbl_1_bss_3E80.camModel);
    Hu3DCameraPosGet(1, &pos, &up, &target);
    Center = target;
    dir.x = pos.x - target.x;
    dir.y = pos.y - target.y;
    dir.z = pos.z - target.z;
    CRot.x = HuAtan(-dir.y, M608Sqrtf((dir.x * dir.x) + (dir.z * dir.z)));
    CRot.y = HuAtan(dir.x, dir.z);
    CRot.z = 0.0f;
    CZoom = M608Sqrtf((dir.z * dir.z) + ((dir.x * dir.x) + (dir.y * dir.y)));
    rotX = CRot.x;
    rotY = CRot.y;
    rotZ = CRot.z;
    camPos.x = Center.x + (CZoom * (HuSin(rotY) * HuCos(rotX)));
    camPos.y = Center.y + (CZoom * -HuSin(rotX));
    camPos.z = Center.z + (CZoom * (HuCos(rotY) * HuCos(rotX)));
    camTarget.x = Center.x;
    camTarget.y = Center.y;
    camTarget.z = Center.z;
    camUp.x = HuSin(rotY) * HuSin(rotX);
    camUp.y = HuCos(rotX);
    camUp.z = HuCos(rotY) * HuSin(rotX);
    PSVECSubtract(&camPos, &camTarget, &axis);
    PSVECNormalize(&axis, &axis);
    newUp.x = (camUp.x * ((axis.x * axis.x) + ((1.0f - (axis.x * axis.x)) * HuCos(rotZ))))
        + (camUp.y * (((axis.x * axis.y) * (1.0 - HuCos(rotZ))) - (axis.z * HuSin(rotZ))))
        + (camUp.z * (((axis.x * axis.z) * (1.0 - HuCos(rotZ))) + (axis.y * HuSin(rotZ))));
    newUp.y = (camUp.y * ((axis.y * axis.y) + ((1.0f - (axis.y * axis.y)) * HuCos(rotZ))))
        + (camUp.x * (((axis.x * axis.y) * (1.0 - HuCos(rotZ))) + (axis.z * HuSin(rotZ))))
        + (camUp.z * (((axis.y * axis.z) * (1.0 - HuCos(rotZ))) - (axis.x * HuSin(rotZ))));
    newUp.z = (camUp.z * ((axis.z * axis.z) + ((1.0f - (axis.z * axis.z)) * HuCos(rotZ))))
        + ((camUp.x * (((axis.x * axis.z) * (1.0 - HuCos(rotZ))) - (axis.y * HuSin(rotZ))))
        + (camUp.y * (((axis.y * axis.z) * (1.0 - HuCos(rotZ))) + (axis.x * HuSin(rotZ)))));
    PSVECNormalize(&newUp, &newUp);
    Hu3DCameraPosSet(1, camPos.x, camPos.y, camPos.z, newUp.x, newUp.y, newUp.z, camTarget.x, camTarget.y, camTarget.z);
}

void fn_1_2FD8(HU3D_MODELID modelId, HU3D_MOTIONID motId, BOOL lagF)
{
    s32 playerNo;
    s32 model;
    s32 charNo;
    float rotY;

    playerNo = lbl_1_bss_3E80.playerNo;
    model = lbl_1_bss_3E80.charModel[playerNo];
    charNo = lbl_1_bss_3E80.charNo[playerNo];
    if (lagF == TRUE) {
        switch (lbl_1_bss_3E80.jumpState) {
            case 0:
                lbl_1_bss_3E80.charObj[playerNo]->objFunc = fn_1_33A4;
                CharMotionShiftSet(charNo, lbl_1_bss_3E80.charMot[playerNo][3], 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
                Hu3DMotionCalc(lbl_1_bss_3E80.charModel[playerNo]);
                fn_1_605C(0);
                OSReport("now timing(%d)\n", lbl_1_bss_3E80.time);
                lbl_1_bss_3E80.mode = 6;
                lbl_1_bss_3E80.time = 0;
                HuAudFXPlay(1624);
                HuAudFXStop(lbl_1_bss_3E80.seNo);
                lbl_1_bss_3E80.jumpState++;
                break;
            case 1:
                CharMotionShiftSet(charNo, lbl_1_bss_3E80.charMot[playerNo][0], 0.0f, 5.0f, HU3D_MOTATTR_LOOP);
                lbl_1_bss_3E80.jumpState++;
                break;
            case 2:
                CharMotionShiftSet(charNo, lbl_1_bss_3E80.charMot[playerNo][4], 0.0f, 8.0f, HU3D_MOTATTR_NONE);
                CharMotionSpeedSet(charNo, 1.0f);
                Hu3DAnimSpeedSet(lbl_1_bss_3E80.charModel[playerNo], 1.0f);
                Hu3DModelAttrReset(lbl_1_bss_3E80.charModel[playerNo], HU3D_MOTATTR_LOOP);
                Hu3DMotionCalc(lbl_1_bss_3E80.charModel[playerNo]);
                lbl_1_bss_3E80.charObj[playerNo]->objFunc = fn_1_34D0;
                lbl_1_bss_3E80.mode = 7;
                lbl_1_bss_3E80.time = 0;
                for (rotY = lbl_1_bss_3E80.rotY; rotY < -360.0f; rotY += 360.0f) {
                }
                if (0.0f == rotY || 360.0f == rotY) {
                    lbl_1_bss_3E80.rotYStart = 0.0f;
                    lbl_1_bss_3E80.rotYEnd = 0.0f;
                } else {
                    lbl_1_bss_3E80.rotYStart = rotY;
                    lbl_1_bss_3E80.rotYEnd = -360.0f;
                }
                lbl_1_bss_3E80.rotY = rotY;
                lbl_1_bss_3E80.rotTime = 0;
                fn_1_605C(1);
                fn_1_605C(7);
                lbl_1_bss_3E80.jumpState++;
                break;
        }
    }
}

void fn_1_33A4(OMOBJ *obj)
{
    lbl_1_bss_3E80.rotY = lbl_1_bss_3E80.rotYStart + (lbl_1_bss_3E80.rotTime * ((lbl_1_bss_3E80.rotYEnd - lbl_1_bss_3E80.rotYStart) / 5.0f));
    if (lbl_1_bss_3E80.rotTime < 5) {
        lbl_1_bss_3E80.rotTime += 1.0f;
    }
    omSetRot(obj, 0.0f, lbl_1_bss_3E80.rotY, 0.0f);
}

void fn_1_34D0(OMOBJ *obj)
{
    s32 charNo;

    lbl_1_bss_3E80.rotY = lbl_1_bss_3E80.rotYStart + (lbl_1_bss_3E80.rotTime * ((lbl_1_bss_3E80.rotYEnd - lbl_1_bss_3E80.rotYStart) / 10.0f));
    if (lbl_1_bss_3E80.rotTime < 10) {
        lbl_1_bss_3E80.rotTime += 1.0f;
    }
    CharMotionSpeedSet(lbl_1_bss_3E80.charNo[lbl_1_bss_3E80.playerNo], 0.5f);
    Hu3DAnimSpeedSet(lbl_1_bss_3E80.charModel[lbl_1_bss_3E80.playerNo], 1.0f);
    omSetRot(obj, 0.0f, lbl_1_bss_3E80.rotY, 0.0f);
    if (Hu3DMotionEndCheck(lbl_1_bss_3E80.charModel[lbl_1_bss_3E80.playerNo]) == TRUE) {
        charNo = lbl_1_bss_3E80.charNo[lbl_1_bss_3E80.playerNo];
        CharMotionShiftSet(charNo, lbl_1_bss_3E80.charMot[lbl_1_bss_3E80.playerNo][0], 0.0f, 5.0f, HU3D_MOTATTR_LOOP);
        OSReport("*** loop ***\n");
        obj->objFunc = NULL;
    }
}


void fn_1_3718(OMOBJ *obj)
{
    s32 objNo;
    float x;
    float y;
    float z;

    objNo = obj->work[0];
    if (lbl_1_bss_3E80.unk3DC > objNo) {
        if (Hu3DModelAttrGet(lbl_1_bss_3E80.unk1C0[objNo]) & HU3D_ATTR_DISPOFF) {
            Hu3DModelAttrReset(lbl_1_bss_3E80.unk1C0[objNo], HU3D_ATTR_DISPOFF);
            Hu3DMotionTimeSet(lbl_1_bss_3E80.unk1C0[objNo], 0.0f);
        }
        if (obj->work[1] != 0) {
            x = lbl_1_bss_10[lbl_1_bss_C - 1].x;
            y = lbl_1_bss_10[lbl_1_bss_C - 1].y;
            z = lbl_1_bss_10[lbl_1_bss_C - 1].z;
            omSetTra(obj, x, y, z);
            obj->work[1]--;
        }
    }
}

void fn_1_388C(OMOBJ *obj)
{
    s32 objNo = obj->work[0];

    *obj->mdlId = lbl_1_bss_3E80.unk1C0[objNo];
    *obj->mtnId = lbl_1_bss_3E80.unk1F6[objNo];
    obj->objFunc = fn_1_3718;
}

void fn_1_38E8(HU3D_MODEL *modelP, Mtx *mtx)
{
}

void fn_1_38EC(HuVecF pos)
{
}

void fn_1_38F0(HSF_OBJECT *object, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr)
{
    Mtx mtx;

    PSMTXIdentity(mtx);
    PSMTXScale(mtx, transform->scale.x, transform->scale.y, transform->scale.z);
    mtxRotCat(mtx, transform->rot.x, transform->rot.y, transform->rot.z);
    mtxRotCat(mtx, 0.0f, lbl_1_data_28[lbl_1_bss_3E80.charNo[0]], 0.0f);
    mtxTransCat(mtx, transform->pos.x, transform->pos.y, transform->pos.z);
    PSMTXConcat(*prev, mtx, *curr);
}

void fn_1_39B4(HSF_OBJECT *object, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr)
{
    Mtx mtx;

    PSMTXIdentity(mtx);
    PSMTXScale(mtx, transform->scale.x, transform->scale.y, transform->scale.z);
    mtxRotCat(mtx, transform->rot.x, transform->rot.y, transform->rot.z);
    mtxRotCat(mtx, 0.0f, lbl_1_data_28[lbl_1_bss_3E80.charNo[1]], 0.0f);
    mtxTransCat(mtx, transform->pos.x, transform->pos.y, transform->pos.z);
    PSMTXConcat(*prev, mtx, *curr);
}

void fn_1_3A78(HSF_OBJECT *object, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr)
{
    Mtx mtx;

    PSMTXIdentity(mtx);
    PSMTXScale(mtx, transform->scale.x, transform->scale.y, transform->scale.z);
    mtxRotCat(mtx, transform->rot.x, transform->rot.y, transform->rot.z);
    mtxRotCat(mtx, 0.0f, lbl_1_data_28[lbl_1_bss_3E80.charNo[2]], 0.0f);
    mtxTransCat(mtx, transform->pos.x, transform->pos.y, transform->pos.z);
    PSMTXConcat(*prev, mtx, *curr);
}

void fn_1_3B3C(HSF_OBJECT *object, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr)
{
    Mtx mtx;

    PSMTXIdentity(mtx);
    PSMTXScale(mtx, transform->scale.x, transform->scale.y, transform->scale.z);
    mtxRotCat(mtx, transform->rot.x, transform->rot.y, transform->rot.z);
    mtxRotCat(mtx, 0.0f, lbl_1_data_28[lbl_1_bss_3E80.charNo[3]], 0.0f);
    mtxTransCat(mtx, transform->pos.x, transform->pos.y, transform->pos.z);
    PSMTXConcat(*prev, mtx, *curr);
}

void fn_1_3C00(HSF_OBJECT *object, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr)
{
    Mtx mtx;

    PSMTXIdentity(mtx);
    PSMTXScale(mtx, transform->scale.x, transform->scale.y, transform->scale.z);
    mtxRotCat(mtx, transform->rot.x, transform->rot.y, transform->rot.z);
    mtxTransCat(mtx, transform->pos.x, transform->pos.y, transform->pos.z);
    PSMTXConcat(*prev, mtx, *curr);
}

void fn_1_3C84(HSF_OBJECT *object, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr)
{
    Mtx mtx;

    PSMTXIdentity(mtx);
    PSMTXScale(mtx, transform->scale.x, transform->scale.y, transform->scale.z);
    mtxRotCat(mtx, transform->rot.x, transform->rot.y, transform->rot.z);
    mtxRotCat(mtx, 0.0f, lbl_1_bss_4 + (lbl_1_data_60[lbl_1_bss_3E80.charNo[0]] + lbl_1_data_28[lbl_1_bss_3E80.charNo[0]]), 0.0f);
    mtxTransCat(mtx, transform->pos.x, transform->pos.y, transform->pos.z);
    PSMTXConcat(*prev, mtx, *curr);
}

void fn_1_3D7C(HSF_OBJECT *object, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr)
{
    Mtx mtx;

    PSMTXIdentity(mtx);
    PSMTXScale(mtx, transform->scale.x, transform->scale.y, transform->scale.z);
    mtxRotCat(mtx, transform->rot.x, transform->rot.y, transform->rot.z);
    mtxTransCat(mtx, transform->pos.x, transform->pos.y, transform->pos.z);
    PSMTXConcat(*prev, mtx, *curr);
}

void fn_1_3E00(HSF_OBJECT *object, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr)
{
    Mtx mtx;
    s32 charNo = lbl_1_bss_3E80.charNo[0];

    PSMTXIdentity(mtx);
    PSMTXScale(mtx, transform->scale.x, transform->scale.y, transform->scale.z);
    mtxRotCat(mtx, transform->rot.x, transform->rot.y, transform->rot.z);
    if (charNo == 8) {
        mtxRotCat(mtx, 0.0f, -(lbl_1_bss_4 + (lbl_1_data_60[lbl_1_bss_3E80.charNo[0]] + lbl_1_data_28[lbl_1_bss_3E80.charNo[0]])), 0.0f);
    }
    mtxTransCat(mtx, transform->pos.x, transform->pos.y, transform->pos.z);
    PSMTXConcat(*prev, mtx, *curr);
}

void fn_1_3F18(HSF_OBJECT *object, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr)
{
    Mtx mtx;
    s32 charNo = lbl_1_bss_3E80.charNo[0];

    PSMTXIdentity(mtx);
    PSMTXScale(mtx, transform->scale.x, transform->scale.y, transform->scale.z);
    mtxRotCat(mtx, transform->rot.x, transform->rot.y, transform->rot.z);
    if (charNo == 8) {
        mtxRotCat(mtx, 0.0f, lbl_1_bss_0 + -lbl_1_data_28[lbl_1_bss_3E80.charNo[0]], 0.0f);
    }
    mtxTransCat(mtx, transform->pos.x, transform->pos.y, transform->pos.z);
    PSMTXConcat(*prev, mtx, *curr);
}

void fn_1_400C(HSF_OBJECT *object, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr)
{
    Mtx mtx;
    s32 charNo = lbl_1_bss_3E80.charNo[1];

    PSMTXIdentity(mtx);
    PSMTXScale(mtx, transform->scale.x, transform->scale.y, transform->scale.z);
    mtxRotCat(mtx, transform->rot.x, transform->rot.y, transform->rot.z);
    if (charNo == 8) {
        mtxRotCat(mtx, 0.0f, -lbl_1_data_28[lbl_1_bss_3E80.charNo[1]], 0.0f);
    }
    mtxTransCat(mtx, transform->pos.x, transform->pos.y, transform->pos.z);
    PSMTXConcat(*prev, mtx, *curr);
}

void fn_1_40F0(HSF_OBJECT *object, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr)
{
    Mtx mtx;
    s32 charNo = lbl_1_bss_3E80.charNo[2];

    PSMTXIdentity(mtx);
    PSMTXScale(mtx, transform->scale.x, transform->scale.y, transform->scale.z);
    mtxRotCat(mtx, transform->rot.x, transform->rot.y, transform->rot.z);
    if (charNo == 8) {
        mtxRotCat(mtx, 0.0f, -lbl_1_data_28[lbl_1_bss_3E80.charNo[2]], 0.0f);
    }
    mtxTransCat(mtx, transform->pos.x, transform->pos.y, transform->pos.z);
    PSMTXConcat(*prev, mtx, *curr);
}

void fn_1_41D4(HSF_OBJECT *object, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr)
{
    Mtx mtx;
    s32 charNo = lbl_1_bss_3E80.charNo[3];

    PSMTXIdentity(mtx);
    PSMTXScale(mtx, transform->scale.x, transform->scale.y, transform->scale.z);
    mtxRotCat(mtx, transform->rot.x, transform->rot.y, transform->rot.z);
    if (charNo == 8) {
        mtxRotCat(mtx, 0.0f, -lbl_1_data_28[lbl_1_bss_3E80.charNo[3]], 0.0f);
    }
    mtxTransCat(mtx, transform->pos.x, transform->pos.y, transform->pos.z);
    PSMTXConcat(*prev, mtx, *curr);
}

void fn_1_42B8(void)
{
    HuVecF pos;

    if (lbl_1_bss_C + 1 < 660) {
        Hu3DMotionCalc(lbl_1_bss_3E80.kuruModel);
        Hu3DModelObjPosGet(lbl_1_bss_3E80.kuruModel, "pmov_ptEFatt", &pos);
        lbl_1_bss_10[lbl_1_bss_C] = pos;
        lbl_1_bss_C++;
    } else {
        OSReport("*******max\n");
    }
}

void fn_1_437C(s16 layerNo)
{
    Mtx mtx;

    Hu3DModelObjMtxGet(lbl_1_bss_3E80.kuruModel, "snowBDEFhook", mtx);
    Hu3DModelMtxSet(lbl_1_bss_3E80.unk170, &mtx);
}

/* Defined after fn_1_6B90: .data keeps definition order */
