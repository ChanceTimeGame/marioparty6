#include "REL/m650dll/m650.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* .bss is laid out in reverse definition order */
M650_OBJMAN lbl_1_bss_1C;
M650_WORK lbl_1_bss_0;

MGSEQ_PARAM lbl_1_data_0 = {
    300,
    1,
    fn_1_F0,
    fn_1_168,
    fn_1_188,
    fn_1_1C8,
    fn_1_200,
    fn_1_304,
    fn_1_3AC,
    fn_1_3B0,
    fn_1_3B4,
};

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
    lbl_1_bss_1C.objman = omInitObjMan(256, 0x2000);
    omGameSysInit(lbl_1_bss_1C.objman);
    MgSeqCreate(&lbl_1_data_0);
}

void fn_1_F0(s16 mode, s16 frameNo)
{
    memset(&lbl_1_bss_0, 0, sizeof(M650_WORK));
    lbl_1_bss_0.unkA = GWMgTimeGet();
    lbl_1_bss_0.winner = -1;
    fn_1_14FC();
    fn_1_1830();
    fn_1_1AE4();
    fn_1_62E0();
    fn_1_5258();
    MgSeqModeNext();
}

void fn_1_168(s16 mode, s16 frameNo)
{
    fn_1_550();
}

void fn_1_188(s16 mode, s16 frameNo)
{
    if (frameNo == 0) {
        lbl_1_bss_0.streamNo = HuAudBGMPlay(78);
    }
}

void fn_1_1C8(s16 mode, s16 frameNo)
{
    if (frameNo == 0) {
        fn_1_5308();
        fn_1_54D4();
        fn_1_2558();
    }
}

void fn_1_200(s16 mode, s16 frameNo)
{
    if (frameNo == 0) {
        HuAudSStreamFadeOut(lbl_1_bss_0.streamNo, 100);
        fn_1_25B0();
        if (!_CheckFlag(FLAG_INST_DECA)) {
            if (lbl_1_bss_0.winner == -1) {
                MgSeqDrawSet();
            } else {
                MgSeqWinnerSet(lbl_1_bss_28[lbl_1_bss_0.winner].charNo, -1, -1, -1);
                GWMgCoinBonusSet(lbl_1_bss_0.winner, 10);
            }
            lbl_1_bss_0.mode = 0;
            lbl_1_bss_0.timeCnt = 0;
        }
    }
}

void fn_1_304(s16 mode, s16 frameNo)
{
    s32 i;
    if (_CheckFlag(FLAG_INST_DECA)) {
        MgSeqModeSet(MGSEQ_MODE_FADEOUT);
        for (i = 0; i < 4; i++) {
            GWMgScoreSet(i, lbl_1_bss_28[i].score);
        }
    } else if (lbl_1_bss_0.winner == -1) {
        MgSeqModeNext();
    } else {
        fn_1_8EC();
    }
}

void fn_1_3AC(s16 mode, s16 frameNo) { }

void fn_1_3B0(s16 mode, s16 frameNo) { }

void fn_1_3B4(s16 mode, s16 frameNo) { }

void fn_1_3B8(void)
{
    OM_CAMERA_VIEW view;
    s32 i;
    view.center.x = 0.0f;
    view.center.y = 150.0f;
    view.center.z = 0.0f;
    view.rot.x = -15.0f;
    view.rot.y = -180.0f;
    view.rot.z = 0.0f;
    view.zoom = 1000.0f;
    for (i = 0; i < 4; i++) {
        omCameraViewMoveMulti(lbl_1_data_150[i], &view, 120, 2);
    }
}

void fn_1_484(void)
{
    OM_CAMERA_VIEW view;
    s32 i;
    view.center.x = 0.0f;
    view.center.y = 300.0f;
    view.center.z = 0.0f;
    view.rot.x = -5.0f;
    view.rot.y = -180.0f;
    view.rot.z = 0.0f;
    view.zoom = 1300.0f;
    for (i = 0; i < 4; i++) {
        omCameraViewMoveMulti(lbl_1_data_150[i], &view, 60, 0);
    }
}

void fn_1_550(void)
{
    HuVecF pos;
    HuVecF target;
    HuVecF up;
    switch (lbl_1_bss_0.mode) {
        case 0:
            MgSeqModeChangeOff();
            lbl_1_bss_0.mode++;
            break;
        case 1:
            if (lbl_1_bss_0.timeCnt++ >= 60) {
                lbl_1_bss_0.timeCnt = 0;
                lbl_1_bss_0.mode++;
                fn_1_3B8();
            } else if (lbl_1_bss_0.timeCnt == 30) {
                HuAudFXPlay(1001);
            }
            break;
        case 2:
            if (lbl_1_bss_0.timeCnt++ >= 60) {
                lbl_1_bss_0.timeCnt = 0;
                lbl_1_bss_0.mode++;
            }
            break;
        case 3:
            if (fn_1_4D58(lbl_1_bss_0.timeCnt++)) {
                fn_1_484();
                lbl_1_bss_0.timeCnt = 0;
                lbl_1_bss_0.mode++;
                pos.x = -1500.0f;
                pos.y = 9000.0f;
                pos.z = -2000.0f;
                up.x = 0.0f;
                up.y = 1.0f;
                up.z = 0.0f;
                target.x = target.y = 0.0f;
                target.z = 2500.0f;
                Hu3DShadowMultiPosSet(&pos, &up, &target, 15);
            }
            break;
        case 4:
            if (lbl_1_bss_0.timeCnt++ >= 60) {
                fn_1_5394();
                lbl_1_bss_0.timeCnt = 0;
                MgSeqModeNext();
            }
            break;
    }
}

void fn_1_8EC(void)
{
    s32 i;
    s32 j;
    M650_PLAYER *player;
    HuVecF pos;
    HuVecF target;
    HuVecF up;
    switch (lbl_1_bss_0.mode) {
        case 0:
            MgSeqModeChangeOff();
            WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
            lbl_1_bss_0.timeCnt = 0;
            lbl_1_bss_0.mode++;
            break;
        case 1:
            if (WipeCheck()) {
                break;
            }
            lbl_1_bss_0.unk12 = 1;
            fn_1_6F54();
            fn_1_26D0();
            if (lbl_1_bss_0.unk14 != 0) {
                fn_1_1228();
            }
            pos.x = -1000.0f;
            pos.y = 5000.0f;
            pos.z = -1000.0f;
            up.x = 0.0f;
            up.y = 1.0f;
            up.z = 0.0f;
            target.x = target.y = 0.0f;
            target.z = 2500.0f;
            Hu3DShadowMultiPosSet(&pos, &up, &target, 15);
            for (i = 0; i < 4; i++) {
                player = &lbl_1_bss_28[i];
                if (i == lbl_1_bss_0.winner) {
                    CharModelKill(player->charNo);
                    player->charMdlId = CharModelCreate(player->charNo, 2);
                    for (j = 0; j < 5; j++) {
                        player->charMotId[j] = CharMotionCreate(player->charNo, lbl_1_data_58[j].dataNum);
                    }
                }
                Hu3DModelShadowSet(player->charMdlId);
                if (lbl_1_bss_0.unk14 == 0) {
                    Hu3DModelPosSet(player->mdlId, -200.0f, 0.0f, 1200.0f);
                    Hu3DModelRotSet(player->mdlId, 0.0f, 170.0f, 0.0f);
                    Hu3DModelAttrReset(player->unk52, HU3D_ATTR_DISPOFF);
                    Hu3DModelPosSet(player->unk52, -180.0f, 5.0f, 1100.0f);
                    Hu3DModelAttrReset(player->unk54, HU3D_ATTR_DISPOFF);
                    Hu3DModelPosSet(player->unk54, -180.0f, 0.2f, 1100.0f);
                    Hu3DMotionSet(player->mdlId, player->motId[3]);
                }
                fn_1_23C4(i, 3, 1.0f);
                CenterM[i].x = 0.0f;
                CenterM[i].y = 150.0f;
                CenterM[i].z = 900.0f;
                CRotM[i].x = -5.0f;
                CRotM[i].y = 180.0f;
                CRotM[i].z = 0.0f;
                CZoomM[i] = 800.0f;
                if (lbl_1_bss_0.winner == i) {
                    Hu3DCameraScissorSet(lbl_1_data_150[i], 0, 0, 640, 480);
                    Hu3DCameraViewportSet(lbl_1_data_150[i], 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
                } else {
                    Hu3DCameraScissorSet(lbl_1_data_150[i], 0, 0, 0, 0);
                    Hu3DCameraViewportSet(lbl_1_data_150[i], 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
                }
            }
            lbl_1_bss_0.mode++;
            break;
        case 2:
            WipeCreate(WIPE_MODE_IN, 5, 60);
            lbl_1_bss_0.mode++;
            break;
        case 3:
            if (WipeCheck()) {
                break;
            }
            if (lbl_1_bss_0.unk14 == 0) {
                fn_1_2320(lbl_1_bss_0.winner, 4);
                MgSeqModeNext();
            } else {
                MgTimerRecordSet(lbl_1_bss_0.timer, -1);
                lbl_1_bss_0.mode++;
            }
            break;
        case 4:
            if (lbl_1_bss_0.timeCnt++ > 150.0f) {
                fn_1_2320(lbl_1_bss_0.winner, 4);
                MgSeqModeNext();
                lbl_1_bss_0.timeCnt = 0;
                lbl_1_bss_0.mode = 11;
            }
            if (lbl_1_bss_0.timeCnt == 110) {
                lbl_1_bss_0.unk16 = 1;
                if (lbl_1_bss_0.unkA != 0) {
                    HuAudFXPlayPan(2036, 96);
                } else {
                    HuAudFXPlayPan(2036, 48);
                }
            }
            break;
    }
}

void fn_1_F90(OMOBJ *obj)
{
    s16 winner = lbl_1_bss_0.winner;
    M650_PLAYER *player = &lbl_1_bss_28[winner];
    HuVecF pos;
    switch (lbl_1_bss_0.unk16) {
        case 1:
            Hu3DModelPosGet(player->unk52, &pos);
            pos.x += player->unk64.x;
            pos.y += player->unk64.y;
            pos.z += player->unk64.z;
            if (pos.y <= 0.5f) {
                HuAudFXPlayPan(2036, 64.0f + pos.x / 50.0f);
                player->unk64.x *= 0.5;
                player->unk64.y *= -0.5;
                player->unk64.z *= 0.5;
                pos.y = 0.5f;
                if (player->unk64.y < 1.0f) {
                    lbl_1_bss_0.unk16++;
                    if (lbl_1_bss_0.unkA == 0) {
                        HuAudFXPlayPan(1001, 96);
                    } else {
                        HuAudFXPlayPan(1001, 32);
                    }
                }
            }
            player->unk64.y -= 1.6333333f;
            Hu3DModelPosSetV(player->unk52, &pos);
            Hu3DModelPosSet(player->unk54, pos.x, 0.2f, pos.z);
            break;
        case 2:
            Hu3DModelPosGet(player->mdlId, &pos);
            if (player->unk48 < 10.0f) {
                player->unk48 += 0.2f;
            }
            pos.x += player->unk48 * (1 - lbl_1_bss_0.unkA * 2);
            Hu3DModelPosSetV(player->mdlId, &pos);
            break;
    }
}

void fn_1_1228(void)
{
    s16 winner = lbl_1_bss_0.winner;
    M650_PLAYER *player = &lbl_1_bss_28[winner];
    OMOBJ *obj;
    obj = omAddObjEx(lbl_1_bss_1C.objman, 100, 0, 0, -1, fn_1_F90);
    Hu3DModelPosSet(player->mdlId, -700.0f + lbl_1_bss_0.unkA * 1400, 0.0f, 1300.0f);
    Hu3DModelRotSet(player->mdlId, 0.0f, 90.0f + 180.0f * lbl_1_bss_0.unkA, 0.0f);
    Hu3DMotionSet(player->mdlId, player->motId[1]);
    Hu3DModelAttrReset(player->unk52, HU3D_ATTR_DISPOFF);
    Hu3DModelPosSet(player->unk52, -550.0f + 1100.0f * lbl_1_bss_0.unkA, 200.0f, 1300.0f);
    Hu3DModelAttrReset(player->unk54, HU3D_ATTR_DISPOFF);
    Hu3DModelPosSet(player->unk54, -550.0f + 1100.0f * lbl_1_bss_0.unkA, 0.2f, 1300.0f);
    player->unk64.x = 30.0f - 60.0f * lbl_1_bss_0.unkA;
    player->unk64.y = 15.0f;
    player->unk64.z = 0.0f;
    player->unk48 = 0.0f;
}
