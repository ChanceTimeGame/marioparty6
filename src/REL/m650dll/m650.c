/* @@HEAD@@ */
#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mgtimer.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/printfunc.h"
#include "game/process.h"
#include "game/seqman.h"
#include "game/wipe.h"
#include "humath.h"
#include "math.h"
#include "string.h"
#include "dolphin.h"

#include "datadir_enum.h"

/* frandmod is called unprototyped: game/frand.h stays out of this unit */

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

typedef struct M650Work_s {
    MGTIMER *timer; /* 0x00 */
    MGTIMER *timer2; /* 0x04 */
    s16 winner; /* 0x08 */
    s16 unkA; /* 0x0A */
    s16 mode; /* 0x0C */
    s16 timeCnt; /* 0x0E */
    s16 unk10; /* 0x10 */
    s16 unk12; /* 0x12 */
    s16 unk14; /* 0x14 */
    s16 unk16; /* 0x16 */
    s32 streamNo; /* 0x18 */
} M650_WORK; /* size 0x1C */

typedef struct M650ObjMan_s {
    OMOBJMAN *objman; /* 0x00 */
    s32 unk4; /* 0x04 */
    s32 unk8; /* 0x08 */
} M650_OBJMAN; /* size 0xC */

typedef struct M650Player_s {
    s16 unk0; /* 0x00 */
    HU3D_MODELID charMdlId; /* 0x02 */
    HU3D_MOTIONID charMotId[5]; /* 0x04 */
    s16 unkE; /* 0x0E */
    s16 charNo; /* 0x10 */
    s16 padNo; /* 0x12 */
    s16 comDif; /* 0x14, -1 for a human player */
    s16 unk16; /* 0x16 */
    s16 unk18; /* 0x18 */
    s16 unk1A; /* 0x1A */
    s16 mode; /* 0x1C */
    HU3D_MODELID unk1E; /* 0x1E */
    HU3D_MODELID mdlId; /* 0x20 */
    HU3D_MOTIONID motId[4]; /* 0x22 */
    s16 unk2A; /* 0x2A */
    HuVecF unk2C; /* 0x2C */
    HuVecF unk38; /* 0x38 */
    float unk44; /* 0x44 */
    float unk48; /* 0x48 */
    float unk4C; /* 0x4C */
    u8 unk50; /* 0x50 */
    HU3D_MODELID unk52; /* 0x52 */
    HU3D_MODELID unk54; /* 0x54 */
    s16 unk56; /* 0x56 */
    HuVecF unk58; /* 0x58 */
    HuVecF unk64; /* 0x64 */
    MGTIMER *timer; /* 0x70 */
    s16 unk74; /* 0x74 */
    s16 score; /* 0x76 */
    HU3D_MODELID unk78; /* 0x78 */
    HU3D_MODELID unk7A; /* 0x7A */
    HU3D_MODELID unk7C; /* 0x7C */
    s16 obstNo[12]; /* 0x7E */
    s16 obstNum; /* 0x96 */
    s16 unk98; /* 0x98 */
    s16 unk9A; /* 0x9A */
    s16 unk9C; /* 0x9C */
    s16 unk9E; /* 0x9E */
} M650_PLAYER; /* size 0xA0 */

typedef struct M650MotData_s {
    u32 dataNum;
    u32 attr;
} M650_MOTDATA;

typedef struct M650Obst_s {
    HuVecF pos; /* 0x00 */
    s16 unkC; /* 0x0C */
    s16 unkE; /* 0x0E */
} M650_OBST; /* size 0x10 */

typedef struct M650ObstFlag_s {
    s16 unk0;
    s16 unk2;
} M650_OBSTFLAG;

/* .bss is laid out in reverse definition order */
M650_OBJMAN lbl_1_bss_1C;
M650_WORK lbl_1_bss_0;
M650_PLAYER lbl_1_bss_28[4];
M650_OBSTFLAG lbl_1_bss_2AC[32][4];

/* Defined after this file's strings (.data keeps definition order) */
extern M650_OBST lbl_1_data_248[32];

void ObjectSetup(void);
void fn_1_F0(s16 mode, s16 frameNo);
void fn_1_168(s16 mode, s16 frameNo);
void fn_1_188(s16 mode, s16 frameNo);
void fn_1_1C8(s16 mode, s16 frameNo);
void fn_1_200(s16 mode, s16 frameNo);
void fn_1_304(s16 mode, s16 frameNo);
void fn_1_3AC(s16 mode, s16 frameNo);
void fn_1_3B0(s16 mode, s16 frameNo);
void fn_1_3B4(s16 mode, s16 frameNo);
void fn_1_3B8(void);
void fn_1_484(void);
void fn_1_550(void);
void fn_1_8EC(void);
void fn_1_F90(OMOBJ *obj);
void fn_1_1228(void);
void fn_1_14FC(void);
void fn_1_1830(void);
void fn_1_1AE4(void);
void fn_1_2010(s16 playerNo);
void fn_1_21CC(OMOBJ *obj);
void fn_1_22A8(s16 playerNo, s16 motNo);
void fn_1_2320(s16 playerNo, s16 motNo);
void fn_1_23C4(s16 playerNo, s16 motNo, float end);
void fn_1_2464(s16 playerNo);
void fn_1_2558(void);
void fn_1_25B0(void);
void fn_1_26D0(void);
void fn_1_273C(s16 playerNo);
void fn_1_289C(s16 playerNo);
s16 fn_1_2CAC(s16 playerNo);
void fn_1_2F1C(s16 playerNo);
void fn_1_33F8(s16 playerNo);
void fn_1_3F28(s16 playerNo);
void fn_1_4430(s16 playerNo);
void fn_1_473C(s16 playerNo);
void fn_1_48C0(s16 playerNo);
void fn_1_49C8(s16 playerNo);
void fn_1_4BD4(s16 playerNo);
s16 fn_1_4D58(void);
void fn_1_5258(void);
void fn_1_5308(void);
void fn_1_5354(void);
void fn_1_5394(void);
void fn_1_54D4(void);
void fn_1_5544(void);
void fn_1_5600(s16 playerNo, float y, float *speed, float *angle);
s16 fn_1_5728(void);
void fn_1_5A24(s16 playerNo);
void fn_1_62E0(void);
s16 fn_1_6D88(s16 playerNo, HuVecF *pos);
void fn_1_6F54(void);
s16 fn_1_7000(s16 playerNo, HuVecF *pos, float angle, s16 obstNo);

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

HuVecF lbl_1_data_28 = { -2000.0f, 10000.0f, -400.0f };
HuVecF lbl_1_data_34 = { -0.4f, -0.8f, -0.8f };
GXColor lbl_1_data_40[2] = { { 0x80, 0x80, 0x80, 0x80 }, { 0x90, 0x90, 0x90, 0x90 } };
u32 lbl_1_data_48[4] = {
    DATANUM(DATA_m650, 0x12),
    DATANUM(DATA_m650, 0x13),
    DATANUM(DATA_m650, 0x14),
    DATANUM(DATA_m650, 0x15),
};
M650_MOTDATA lbl_1_data_58[5] = {
    { DATANUM(DATA_mario, 0xA5), HU3D_MOTATTR_LOOP },
    { DATANUM(DATA_mariomot, 0x16), HU3D_MOTATTR_LOOP },
    { DATANUM(DATA_mario, 0xA6), 0 },
    { DATANUM(DATA_mariomot, 0x00), HU3D_MOTATTR_LOOP },
    { DATANUM(DATA_mariomot, 0x06), 0 },
};
u32 lbl_1_data_80[2] = { DATANUM(DATA_m650, 0x00), DATANUM(DATA_m650, 0x06) };
u32 lbl_1_data_88[2] = { DATANUM(DATA_m650, 0x01), DATANUM(DATA_m650, 0x07) };
u32 lbl_1_data_90[2] = { DATANUM(DATA_m650, 0x05), DATANUM(DATA_m650, 0x0A) };
u32 lbl_1_data_98[2] = { DATANUM(DATA_m650, 0x0C), DATANUM(DATA_m650, 0x0D) };
u32 lbl_1_data_A0[2] = { DATANUM(DATA_m650, 0x0E), DATANUM(DATA_m650, 0x0F) };
HuVecF lbl_1_data_A8[14] = {
    { 0.55f, 1.0f, 0.6f },
    { 0.55f, 1.0f, 0.5f },
    { 0.75f, 1.0f, 0.75f },
    { 0.6f, 1.0f, 0.6f },
    { 0.75f, 1.0f, 0.7f },
    { 0.75f, 1.0f, 0.75f },
    { 0.8f, 1.0f, 0.7f },
    { 0.5f, 1.0f, 0.5f },
    { 0.6f, 1.0f, 0.6f },
    { 0.8f, 1.0f, 0.7f },
    { 0.5f, 1.0f, 0.5f },
    { 0.8f, 1.0f, 0.7f },
    { 0.8f, 1.0f, 0.7f },
    { 0.8f, 1.0f, 0.7f },
};
u16 lbl_1_data_150[4] = { HU3D_CAM0, HU3D_CAM1, HU3D_CAM2, HU3D_CAM3 };

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
            lbl_1_bss_0.timeCnt++;
            if (fn_1_4D58()) {
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

void fn_1_14FC(void)
{
    u16 cameraBit[4] = { HU3D_CAM0, HU3D_CAM1, HU3D_CAM2, HU3D_CAM3 };
    s32 i;
    OMOBJ *obj;
    for (i = 0; i < 4; i++) {
        Hu3DCameraCreate(cameraBit[i]);
        Hu3DCameraViewportSet(cameraBit[i], 0.0f, 0.0f, (i != 0) ? 0.0f : 640.0f, (i != 0) ? 0.0f : 480.0f, 0.0f, 1.0f);
        Hu3DCameraPerspectiveSet(cameraBit[i], 40.0f, 60.0f, 25000.0f, 1.2f);
        Hu3DCameraScissorSet(cameraBit[i], 0, 0, (i != 0) ? 0.0f : 640.0f, (i != 0) ? 0.0f : 480.0f);
    }
    obj = omAddObjEx(lbl_1_bss_1C.objman, 0x7FDA, 0, 0, -1, omOutViewMulti);
    omOutViewMultiCntSet(obj, 4);
    for (i = 0; i < 4; i++) {
        CenterM[i].x = 0.0f;
        CenterM[i].y = 200.0f;
        CenterM[i].z = 0.0f;
        CRotM[i].x = -10.0f;
        CRotM[i].y = 0.0f;
        CRotM[i].z = 0.0f;
        CZoomM[i] = 1000.0f;
    }
}

void fn_1_1830(void)
{
    HU3D_MODELID modelId;
    HU3D_LIGHTID lightId;
    HuVecF pos;
    HuVecF target;
    HuVecF up;
    lightId = Hu3DGLightCreateV(&lbl_1_data_28, &lbl_1_data_34, &lbl_1_data_40[lbl_1_bss_0.unkA]);
    Hu3DGLightStaticSet(lightId, FALSE);
    Hu3DGLightInfinitytSet(lightId);
    if (lbl_1_bss_0.unkA == 0) {
        Hu3DShineSet(TRUE);
        Hu3DAmbColorSet(0.5f, 0.5f, 0.5f);
    }
    Hu3DShadowMultiCreate(30.0f, 500.0f, 80000.0f, 15);
    pos.x = -750.0f;
    pos.y = 4500.0f;
    pos.z = -1000.0f;
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    target.x = target.y = 0.0f;
    target.z = 0.0f;
    Hu3DShadowMultiPosSet(&pos, &up, &target, 15);
    if (lbl_1_bss_0.unkA == 0) {
        Hu3DShadowMultiColSet(130, 30, 0, 15);
        Hu3DShadowMultiTPLvlSet(0.8f, 15);
    } else {
        Hu3DShadowMultiColSet(0, 0, 0, 15);
        Hu3DShadowMultiTPLvlSet(0.5f, 15);
    }
    modelId = Hu3DModelCreateData(lbl_1_data_80[lbl_1_bss_0.unkA]);
    Hu3DModelCameraSet(modelId, HU3D_CAM_ALL);
    Hu3DModelShadowMapSet(modelId);
    if (lbl_1_bss_0.unkA == 0) {
        Hu3DModelShadowMapTPLvlSet(modelId, 1.0f);
    } else {
        Hu3DModelShadowMapTPLvlSet(modelId, 0.5f);
    }
    modelId = Hu3DModelCreateData(lbl_1_data_88[lbl_1_bss_0.unkA]);
    Hu3DModelCameraSet(modelId, HU3D_CAM_ALL);
}

void fn_1_1AE4(void)
{
    HU3D_MODELID modelId;
    M650_PLAYER *player;
    s32 i;
    s32 j;
    s16 charNo;
    OMOBJ *obj;
    for (i = 0; i < 4; i++) {
        player = &lbl_1_bss_28[i];
        player->charNo = charNo = GwPlayerConf[i].charNo;
        player->padNo = GwPlayerConf[i].padNo;
        if (GwPlayerConf[i].type == 0) {
            player->comDif = -1;
        } else {
            player->comDif = GwPlayerConf[i].comDif;
        }
        modelId = Hu3DModelCreateData(DATANUM(DATA_m650, 0x04));
        Hu3DModelCameraSet(modelId, lbl_1_data_150[i]);
        Hu3DModelAttrSet(modelId, HU3D_ATTR_DISPOFF);
        player->unk1E = modelId;
        modelId = Hu3DModelCreateData(DATANUM(DATA_m650, 0x11));
        Hu3DModelCameraSet(modelId, lbl_1_data_150[i]);
        for (j = 0; j < 4; j++) {
            player->motId[j] = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(lbl_1_data_48[j], HU_MEMNUM_OVL, HEAP_MODEL));
        }
        Hu3DMotionSet(modelId, player->motId[0]);
        Hu3DModelAttrSet(modelId, HU3D_MOTATTR_LOOP);
        Hu3DModelAmbSet(modelId, 1.0f, 1.0f, 1.0f);
        Hu3DModelShadowSet(modelId);
        player->mdlId = modelId;
        modelId = CharModelCreate(player->charNo, 8);
        Hu3DModelShadowSet(modelId);
        Hu3DModelCameraSet(modelId, lbl_1_data_150[i]);
        player->charMdlId = modelId;
        for (j = 0; j < 5; j++) {
            player->charMotId[j] = CharMotionCreate(player->charNo, lbl_1_data_58[j].dataNum);
        }
        CharMotionShiftSet(player->charNo, player->charMotId[0], 0.0f, 0.0f, lbl_1_data_58[0].attr);
        modelId = Hu3DModelCreateData(DATANUM(DATA_m650, 0x10));
        Hu3DModelCameraSet(modelId, lbl_1_data_150[i]);
        Hu3DModelAttrSet(modelId, HU3D_ATTR_DISPOFF);
        player->unk52 = modelId;
        modelId = Hu3DModelCreateData(lbl_1_data_90[lbl_1_bss_0.unkA]);
        Hu3DModelCameraSet(modelId, lbl_1_data_150[i]);
        Hu3DModelScaleSet(modelId, 0.3f, 1.0f, 0.3f);
        Hu3DModelPosSet(modelId, 0.0f, 0.2f, 0.0f);
        Hu3DModelAttrSet(modelId, HU3D_ATTR_DISPOFF);
        Hu3DModelTPLvlSet(modelId, 0.5f);
        player->unk54 = modelId;
        modelId = Hu3DModelCreateData(lbl_1_data_98[lbl_1_bss_0.unkA]);
        Hu3DMotionSpeedSet(modelId, 0.0f);
        Hu3DModelAttrSet(modelId, HU3D_ATTR_DISPOFF);
        Hu3DModelCameraSet(modelId, lbl_1_data_150[i]);
        player->unk7A = modelId;
        modelId = Hu3DModelCreateData(lbl_1_data_A0[lbl_1_bss_0.unkA]);
        Hu3DMotionSpeedSet(modelId, 0.0f);
        Hu3DModelCameraSet(modelId, lbl_1_data_150[i]);
        Hu3DModelAttrSet(modelId, HU3D_ATTR_DISPOFF);
        player->unk7C = modelId;
        modelId = Hu3DModelCreateData(DATANUM(DATA_m650, 0x0B));
        Hu3DMotionSpeedSet(modelId, 0.0f);
        Hu3DModelCameraSet(modelId, lbl_1_data_150[i]);
        Hu3DModelAttrSet(modelId, HU3D_ATTR_DISPOFF);
        player->unk78 = modelId;
        player->score = -1;
    }
    CharEffectLayerSet(3);
    obj = omAddObjEx(lbl_1_bss_1C.objman, 80, 0, 0, -1, fn_1_21CC);
}

void fn_1_2010(s16 playerNo)
{
    M650_PLAYER *player = &lbl_1_bss_28[playerNo];
    s16 seqMode;
    HuVecF pos;
    Mtx mtx;
    seqMode = MgSeqModeGet();
    switch (player->mode) {
        case 1:
            fn_1_273C(playerNo);
            break;
        case 2:
            fn_1_2F1C(playerNo);
            fn_1_33F8(playerNo);
            break;
        case 3:
            fn_1_33F8(playerNo);
            break;
        case 4:
            fn_1_3F28(playerNo);
            break;
        case 5:
            fn_1_4430(playerNo);
            break;
        case 6:
            fn_1_473C(playerNo);
            break;
        case 7:
            fn_1_48C0(playerNo);
            break;
        case 8:
            fn_1_49C8(playerNo);
            break;
        case 10:
            fn_1_4BD4(playerNo);
            break;
    }
    Hu3DModelPosGet(player->mdlId, &pos);
    Hu3DModelPosSetV(player->unk1E, &pos);
    pos.y += 0.2;
    if (lbl_1_bss_0.unk12 == 0) {
        Hu3DModelObjMtxGet(player->mdlId, "itemhook_C", mtx);
        Hu3DModelMtxSet(player->charMdlId, &mtx);
    } else {
        mtxRot(mtx, 0.0f, 180.0f, 0.0f);
        mtxTransCat(mtx, 0.0f, 0.0f, 900.0f);
        Hu3DModelMtxSet(player->charMdlId, &mtx);
    }
    fn_1_5A24(playerNo);
}

void fn_1_21CC(OMOBJ *obj)
{
    s32 i;
    u32 seqMode = MgSeqModeGet();
    for (i = 0; i < 4; i++) {
        fn_1_2010(i);
    }
    if (fn_1_5728()) {
        MgSeqModeNext();
    }
    if (_CheckFlag(FLAG_INST_DECA)) {
        for (i = 0; i < 4; i++) {
            if (lbl_1_bss_28[i].timer && MgTimerDoneCheck(lbl_1_bss_28[i].timer) == TRUE) {
                break;
            }
        }
        if (i < 4 && seqMode == MGSEQ_MODE_MAIN) {
            MgSeqModeNext();
        }
    }
}

void fn_1_22A8(s16 playerNo, s16 motNo)
{
    M650_PLAYER *player = &lbl_1_bss_28[playerNo];
    Hu3DMotionShiftSet(player->mdlId, player->motId[motNo], 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
}

void fn_1_2320(s16 playerNo, s16 motNo)
{
    CharMotionShiftSet(lbl_1_bss_28[playerNo].charNo, lbl_1_bss_28[playerNo].charMotId[motNo], 0.0f, 8.0f, lbl_1_data_58[motNo].attr);
}

void fn_1_23C4(s16 playerNo, s16 motNo, float end)
{
    CharMotionShiftSet(lbl_1_bss_28[playerNo].charNo, lbl_1_bss_28[playerNo].charMotId[motNo], 0.0f, end, lbl_1_data_58[motNo].attr);
}

void fn_1_2464(s16 playerNo)
{
    M650_PLAYER *player = &lbl_1_bss_28[playerNo];
    player->unk16 = 120;
    player->mode = 6;
    fn_1_22A8(playerNo, 2);
    fn_1_2320(playerNo, 1);
}

void fn_1_2558(void)
{
    s32 i;
    for (i = 0; i < 4; i++) {
        lbl_1_bss_28[i].mode = 1;
        fn_1_289C(i);
    }
}

void fn_1_25B0(void)
{
    s32 i;
    lbl_1_bss_0.unk10 = 1;
    for (i = 0; i < 4; i++) {
        switch (lbl_1_bss_28[i].mode) {
            case 1:
                lbl_1_bss_28[i].mode = 9;
                Hu3DModelAttrSet(lbl_1_bss_28[i].unk1E, HU3D_ATTR_DISPOFF);
                break;
            case 4:
                if (lbl_1_bss_0.winner != i) {
                    lbl_1_bss_28[i].mode = 9;
                    fn_1_22A8(i, 0);
                }
                break;
        }
    }
}

void fn_1_26D0(void)
{
    s32 i;
    for (i = 0; i < 4; i++) {
        lbl_1_bss_28[i].mode = 9;
        Hu3DModelAttrSet(lbl_1_bss_28[i].unk1E, HU3D_ATTR_DISPOFF);
    }
}

void fn_1_273C(s16 playerNo)
{
    M650_PLAYER *player = &lbl_1_bss_28[playerNo];
    Hu3DModelAttrReset(player->unk1E, HU3D_ATTR_DISPOFF);
    Hu3DModelRotSet(player->unk1E, 0.0f, player->unk44, 0.0f);
    if (player->unk50) {
        player->unk44 += 1.7142857f;
        if (player->unk44 >= 30.0f) {
            player->unk44 = 30.0f;
            player->unk50 = FALSE;
        }
    } else {
        player->unk44 -= 1.7142857f;
        if (player->unk44 <= -30.0f) {
            player->unk44 = -30.0f;
            player->unk50 = TRUE;
        }
    }
    if (fn_1_2CAC(playerNo)) {
        player->mode = 2;
        player->unk48 = 0.0f;
        player->unk18 = 0;
        player->unk4C = 0.0f;
    }
}

void fn_1_289C(s16 playerNo)
{
    s16 base[4] = { 210, 140, 50, 0 };
    s16 range[4] = { 105, 70, 35, 70 };
    s16 chance[4] = { 90, 45, 15, 5 };
    M650_PLAYER *player;
    s32 i;
    HuVecF pos;
    HuVecF dir;
    player = &lbl_1_bss_28[playerNo];
    if (player->comDif == -1) {
        return;
    }
    player->unk98 = base[player->comDif] + frandmod(range[player->comDif]);
    if (frandmod(100) < chance[player->comDif]) {
        player->unk9C = 1;
    } else {
        player->unk9C = 0;
    }
    Hu3DModelPosGet(player->mdlId, &pos);
    player->obstNum = 0;
    player->unk9A = 0;
    for (i = 0; i < 32; i++) {
        if (lbl_1_bss_2AC[i][playerNo].unk0 != 0) {
            continue;
        }
        if (lbl_1_data_248[i].pos.z < pos.z - 50.0f) {
            continue;
        }
        if (lbl_1_data_248[i].pos.z > 900.0f + pos.z) {
            continue;
        }
        if (lbl_1_data_248[i].pos.x > 500.0f + pos.x) {
            continue;
        }
        if (lbl_1_data_248[i].pos.x < pos.x - 500.0f) {
            continue;
        }
        dir.x = lbl_1_data_248[i].pos.x - pos.x;
        dir.y = 0.0f;
        dir.z = lbl_1_data_248[i].pos.z - pos.z;
        if (sqrtf(VECSquareMag(&dir)) > 1000.0f) {
            continue;
        }
        player->obstNo[player->obstNum] = i;
        if (++player->obstNum == 12) {
            OSReport("WARNING! obst cnt\n");
            print8(100, 100 + playerNo * 16, 1.8f, "OBST CNT OVER %d", playerNo);
            return;
        }
    }
}

s16 fn_1_2CAC(s16 playerNo)
{
    M650_PLAYER *player = &lbl_1_bss_28[playerNo];
    s32 i;
    s16 hitF;
    s16 outF;
    float ofsX;
    float ofsZ;
    HuVecF pos;
    Hu3DModelPosGet(player->mdlId, &pos);
    if (player->comDif == -1) {
        return HuPadBtnDown[player->padNo] & PAD_BUTTON_A;
    }
    if (player->unk98 != 0) {
        player->unk98--;
        return FALSE;
    }
    ofsX = 800.0 * HuSin(player->unk44);
    ofsZ = 800.0 * HuCos(player->unk44);
    if (pos.x + ofsX >= 1150.0f || pos.x + ofsX <= -1150.0f) {
        outF = TRUE;
    } else {
        outF = FALSE;
    }
    hitF = FALSE;
    for (i = 0; i < player->obstNum; i++) {
        if (fn_1_7000(playerNo, &pos, player->unk44, player->obstNo[i])) {
            hitF = TRUE;
            break;
        }
    }
    if (player->unk9A++ >= 70) {
        if (player->unk9C) {
            return TRUE;
        }
        if (outF) {
            return FALSE;
        }
        return TRUE;
    }
    if (player->unk9C) {
        if (hitF || outF || player->obstNum == 0) {
            return TRUE;
        }
        return FALSE;
    }
    if (!hitF && !outF) {
        return TRUE;
    }
    return FALSE;
}

void fn_1_2F1C(s16 playerNo)
{
    M650_PLAYER *player = &lbl_1_bss_28[playerNo];
    HuVecF rot;
    HuVecF pos;
    HuVecF ofs;
    s32 seNo[4] = { 2028, 2029, 2030, 2031 };
    float angle;
    float speed;
    float time;
    float maxTime;
    Hu3DModelRotGet(player->mdlId, &rot);
    if (rot.y > player->unk44) {
        rot.y -= 2.0f;
        if (rot.y < player->unk44) {
            rot.y = player->unk44;
        }
    } else if (rot.y < player->unk44) {
        rot.y += 2.0f;
        if (rot.y > player->unk44) {
            rot.y = player->unk44;
        }
    }
    Hu3DModelRotSetV(player->mdlId, &rot);
    if (player->unk18 == 30) {
        fn_1_2320(playerNo, 2);
        Hu3DModelPosSet(player->unk52, 0.0f, 0.0f, 0.0f);
        Hu3DModelHookSet(player->charMdlId, "f-itemhook-r", player->unk52);
        Hu3DModelRotSet(player->unk52, 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrReset(player->unk52, HU3D_ATTR_DISPOFF);
    }
    if (player->unk18 > 30) {
        time = Hu3DMotionTimeGet(player->charMdlId);
        maxTime = Hu3DMotionMaxTimeGet(player->charMdlId);
        if (time >= maxTime && player->unk18 > 44) {
            fn_1_2320(playerNo, 0);
            player->mode = 3;
            player->unk18 = 0;
            return;
        }
        if (player->unk18 == 44) {
            HuAudFXPlay(seNo[playerNo]);
            Hu3DModelObjPosGet(player->charMdlId, "f-itemhook-r", &pos);
            ofs.x = ofs.y = ofs.z = 0.0f;
            Hu3DModelHookReset(player->charMdlId);
            Hu3DModelPosSet(player->unk52, pos.x + ofs.x, pos.y + ofs.y, pos.z + ofs.z);
            Hu3DModelAttrReset(player->unk54, HU3D_ATTR_DISPOFF);
            Hu3DModelPosSet(player->unk54, pos.x + ofs.x, 0.2f, pos.z + ofs.z);
            Hu3DModelRotSet(player->unk52, 0.0f, 45.0f, 0.0f);
            fn_1_5600(playerNo, pos.y + ofs.y, &speed, &angle);
            player->unk64.x = speed * HuCos(angle) * HuSin(player->unk44);
            player->unk64.y = speed * HuSin(angle);
            player->unk64.z = speed * HuCos(angle) * HuCos(player->unk44);
            Hu3DModelAttrSet(player->unk1E, HU3D_ATTR_DISPOFF);
        }
    }
    player->unk18++;
}

void fn_1_33F8(s16 playerNo)
{
    M650_PLAYER *player = &lbl_1_bss_28[playerNo];
    HuVecF pos;
    HuVecF dir;
    HuVecF reflect;
    HuVecF normal;
    s32 wallSeNo[4] = { 2032, 2033, 2034, 2035 };
    s32 landSeNo[4] = { 2036, 2037, 2038, 2039 };
    s16 obstNo;
    float speed;
    float push;
    float dist;
    float distSq;
    float radius;
    if (0.0f == player->unk64.x && 0.0f == player->unk64.y && 0.0f == player->unk64.z) {
        return;
    }
    player->unk1A++;
    speed = HuMagVecF(&player->unk64);
    Hu3DModelPosGet(player->unk52, &pos);
    pos.x += player->unk64.x;
    pos.y += player->unk64.y;
    pos.z += player->unk64.z;
    dir.x = player->unk64.x;
    dir.y = 0.0f;
    dir.z = player->unk64.z;
    player->unk4C += sqrtf(VECSquareMag(&dir));
    if (pos.y <= 0.5f) {
        HuAudFXPlay(landSeNo[playerNo]);
        pos.y = 0.5f;
        player->unk64.x *= 0.4;
        player->unk64.y *= -0.4;
        player->unk64.z *= 0.4;
        if (player->unk64.y < 1.0f) {
            player->unk1A = 0;
            if (lbl_1_bss_0.unk10 == 0) {
                player->mode = 4;
                fn_1_22A8(playerNo, 1);
                (s16)playerNo; /* retail evaluates playerNo here and discards it */
                HuAudFXPlayPan(1001, 32);
            } else {
                player->mode = 9;
            }
            player->unk64.x = player->unk64.y = player->unk64.z = 0.0f;
        }
    }
    if (pos.x <= -1210.0f) {
        HuAudFXPlay(wallSeNo[playerNo]);
        pos.x = -1210.0f;
        normal.x = 1.0f;
        normal.y = normal.z = 0.0f;
        C_VECReflect(&player->unk64, &normal, &reflect);
        player->unk64.x = 0.5 * reflect.x * speed;
        player->unk64.y = 0.5 * reflect.y * speed;
        player->unk64.z = 0.5 * reflect.z * speed;
    }
    if (pos.x >= 1210.0f) {
        HuAudFXPlay(wallSeNo[playerNo]);
        pos.x = 1210.0f;
        normal.x = -1.0f;
        normal.y = normal.z = 0.0f;
        C_VECReflect(&player->unk64, &normal, &reflect);
        player->unk64.x = 0.5 * reflect.x * speed;
        player->unk64.y = 0.5 * reflect.y * speed;
        player->unk64.z = 0.5 * reflect.z * speed;
    }
    obstNo = fn_1_6D88(playerNo, &pos);
    if (obstNo >= 0) {
        HuAudFXPlay(wallSeNo[playerNo]);
        dir.x = pos.x;
        dir.y = 0.0f;
        dir.z = pos.z;
        distSq = VECSquareDistance(&lbl_1_data_248[obstNo].pos, &dir);
        radius = 8100.0f;
        if (distSq < radius) {
            push = sqrtf(radius - distSq);
            normal.x = normal.y = normal.z = 0.0f;
            dist = sqrtf(VECSquareDistance(&player->unk64, &normal));
            if (dist > 0.0f) {
                pos.x -= player->unk64.x * push / dist;
                pos.y -= player->unk64.y * push / dist;
                pos.z -= player->unk64.z * push / dist;
            }
        }
        normal.x = pos.x - lbl_1_data_248[obstNo].pos.x;
        normal.y = 0.0f;
        normal.z = pos.z - lbl_1_data_248[obstNo].pos.z;
        VECNormalize(&normal, &normal);
        if (player->unk64.x || player->unk64.y || player->unk64.z) {
            C_VECReflect(&player->unk64, &normal, &reflect);
            player->unk64.x = 0.4 * reflect.x * speed;
            player->unk64.y = 0.4 * reflect.y * speed;
            player->unk64.z = 0.4 * reflect.z * speed;
        }
        pos.x = 90.0f * normal.x + lbl_1_data_248[obstNo].pos.x;
        pos.z = 90.0f * normal.z + lbl_1_data_248[obstNo].pos.z;
    }
    Hu3DModelPosSetV(player->unk52, &pos);
    Hu3DModelPosSet(player->unk54, pos.x, 0.2f, pos.z);
    player->unk64.y -= 1.6333333f;
}

/* @@PART2@@ */
/* @@END@@ */
