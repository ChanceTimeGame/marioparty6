/* @@HEAD@@ */
#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/seqman.h"
#include "game/wipe.h"
#include "humath.h"
#include "math.h"
#include "dolphin.h"

#include "datadir_enum.h"

/* rand8 is called unprototyped: game/frand.h stays out of this unit */

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

typedef struct M612Work_s {
    OMOBJMAN *objman; /* 0x00 */
    s32 mode; /* 0x04 */
    s16 unk8; /* 0x08 */
    HU3D_MODELID panelMdlId[4][49]; /* 0x0A, [x + z * 7] */
    s16 unk192; /* 0x192 */
    HU3D_MODELID unk194; /* 0x194 */
    HU3D_MODELID unk196; /* 0x196 */
    OMOBJ *unk198; /* 0x198 */
    OMOBJ *unk19C; /* 0x19C */
} M612_WORK; /* size 0x1A0 */

/* .bss is laid out in reverse definition order */
M612_WORK lbl_1_bss_4AC;
s32 lbl_1_bss_49C[4];
s32 lbl_1_bss_48C[4];
HU3D_MOTIONID lbl_1_bss_44C[4][8];
s32 lbl_1_bss_43C[4];
s32 lbl_1_bss_42C[4];
s32 lbl_1_bss_41C[4];
s32 lbl_1_bss_40C[4];
s32 lbl_1_bss_3FC[4];
s32 lbl_1_bss_3EC[4];
float lbl_1_bss_3DC[4];
s32 lbl_1_bss_3CC[4];
s32 lbl_1_bss_3BC[4];
s32 lbl_1_bss_3AC[4];
s32 lbl_1_bss_39C[4];
s32 lbl_1_bss_38C[4];
s32 lbl_1_bss_37C[4];
s32 lbl_1_bss_36C[4];
s32 lbl_1_bss_30C[24];
s32 lbl_1_bss_308;
s32 lbl_1_bss_304;
s32 lbl_1_bss_300;
s32 lbl_1_bss_2FC;
s32 lbl_1_bss_2F8;
s32 lbl_1_bss_2F4;
s32 lbl_1_bss_2F0;
float lbl_1_bss_2EC;
float lbl_1_bss_2E8;
s32 lbl_1_bss_2C0[10];
s32 lbl_1_bss_48[158];
s32 lbl_1_bss_44;
s32 lbl_1_bss_40;
s32 lbl_1_bss_3C;
s32 lbl_1_bss_38;
s32 lbl_1_bss_34;
s32 lbl_1_bss_30;
s32 lbl_1_bss_2C;
s32 lbl_1_bss_28;
s32 lbl_1_bss_24;
s32 lbl_1_bss_20;
s32 lbl_1_bss_1C;
s32 lbl_1_bss_18;
s32 lbl_1_bss_14;
s32 lbl_1_bss_10;
s32 lbl_1_bss_C;
s32 lbl_1_bss_8;
s32 lbl_1_bss_4;
s32 lbl_1_bss_0;

void ObjectSetup(void);
void fn_1_8F0(void);
void fn_1_F0C(void);
s32 fn_1_13A4(s32 arg0, s32 arg1);
void fn_1_1430(void);
void fn_1_1694(s16 mode, s16 frameNo);
void fn_1_16D4(s16 mode, s16 frameNo);
void fn_1_1DBC(s16 mode, s16 frameNo);
void fn_1_1E18(s16 mode, s16 frameNo);
void fn_1_1FB4(s16 mode, s16 frameNo);
void fn_1_242C(s16 mode, s16 frameNo);
void fn_1_2BC8(s16 mode, s16 frameNo);
void fn_1_2C8C(s16 mode, s16 frameNo);
void fn_1_2C90(s16 mode, s16 frameNo);
void fn_1_2C94(OMOBJ *obj);
void fn_1_2CEC(s32 playerNo);
s32 fn_1_4038(s32 playerNo);
s32 fn_1_4150(s32 playerNo);
s32 fn_1_42C4(s32 playerNo);
s32 fn_1_4480(s32 playerNo);
void fn_1_48D4(void);
void fn_1_4B94(s32 playerNo);
void fn_1_51D8(void);
void fn_1_54D4(OMOBJ *obj);
void fn_1_5924(OMOBJ *obj);
float fn_1_5E00(s32 dir);

/* Defined by part 2, after fn_1_5924: .data keeps definition order */
extern s8 lbl_1_data_198[][60];
extern s8 lbl_1_data_16B0[][7];

s32 lbl_1_data_0[4] = { 45, 30, 30, 15 };
s32 lbl_1_data_10[4] = { 45, 30, 15, 15 };
s32 lbl_1_data_20[4] = { 1, 1, 2, 0 };
s32 lbl_1_data_30[4] = { 1, 2, 2, 0 };
s32 lbl_1_data_40[4] = { 0, 0, 0, 0 };
unsigned int lbl_1_data_50[9] = {
    DATANUM(DATA_mariomot, 0x00),
    DATANUM(DATA_mariomot, 0x01),
    DATANUM(DATA_mariomot, 0x06),
    DATANUM(DATA_mariomot, 0x07),
    DATANUM(DATA_mariomot, 0x0F),
    DATANUM(DATA_mariomot, 0x22),
    DATANUM(DATA_mariomot, 0x3F),
    DATANUM(DATA_mariomot, 0x17),
    0,
};
s32 lbl_1_data_74[4] = { 1, 2, 4, 8 };
GXColor lbl_1_data_84[2] = { { 0xFF, 0xFF, 0xFF, 0x00 }, { 0xD0, 0xD0, 0xD0, 0x00 } };
s32 lbl_1_data_8C = -1;
MGSEQ_PARAM lbl_1_data_90 = {
    300,
    0,
    fn_1_1694,
    fn_1_16D4,
    fn_1_1DBC,
    fn_1_1E18,
    fn_1_1FB4,
    fn_1_242C,
    fn_1_2BC8,
    fn_1_2C8C,
    fn_1_2C90,
};

/* @@PART1@@ */
int _prolog(void) {
    const VoidFunc* ctors = _ctors;
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void) {
    const VoidFunc* dtors = _dtors;
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    s16 i;
    OMOBJ *obj = NULL;
    HU3D_LLIGHTID lightId;

    lbl_1_bss_4AC.unk8 = 0;
    lbl_1_bss_4AC.objman = omInitObjMan(80, 0x2000);
    omGameSysInit(lbl_1_bss_4AC.objman);
    lbl_1_bss_10 = GWMgTimeGet();
    fn_1_1430();
    fn_1_8F0();
    fn_1_F0C();
    for (i = 0; i < 4; i++) {
        lbl_1_bss_42C[i] = 0;
    }
    fn_1_48D4();
    obj = omAddObjEx(lbl_1_bss_4AC.objman, 0x7FDA, 0, 0, -1, omOutViewMulti);
    omOutViewMultiCntSet(obj, 4);
    MgSeqCreate(&lbl_1_data_90);
    for (i = 0; i < 4; i++) {
        lbl_1_bss_49C[i] = GwPlayerConf[i].charNo;
        lbl_1_bss_43C[i] = GwPlayerConf[i].padNo;
        lbl_1_bss_48C[i] = CharModelMotListCreate(lbl_1_bss_49C[i], 4, lbl_1_data_50, lbl_1_bss_44C[i]);
        Hu3DModelPosSet(lbl_1_bss_48C[i], 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrSet(lbl_1_bss_48C[i], HU3D_MOTATTR_LOOP);
        CharMotionSet(lbl_1_bss_49C[i], lbl_1_bss_44C[i][0]);
        Hu3DModelCameraSet(lbl_1_bss_48C[i], lbl_1_data_74[i]);
        lbl_1_bss_3FC[i] = -1;
        lbl_1_bss_30C[i] = 0;
        Hu3DModelAttrSet(lbl_1_bss_48C[i], HU3D_ATTR_DISPOFF);
    }
    {
        HuVecF pos = { 0.0f, 1000.0f, 5000.0f };
        HuVecF dir = { 0.0f, 0.0f, -1.0f };
        lightId = CharLightCreateV(&pos, &dir, &lbl_1_data_84[lbl_1_bss_10]);
    }
    CharLightStaticSet(TRUE);
    for (i = 0; i < 4; i++) {
        lbl_1_bss_36C[i] = 0;
        if (i == 0) {
            lbl_1_bss_39C[i] = Hu3DModelCreateData(DATANUM(DATA_m612, 0x15));
            lbl_1_bss_38C[i] = Hu3DJointMotionData(lbl_1_bss_39C[0], DATANUM(DATA_m612, 0x16));
        } else {
            lbl_1_bss_39C[i] = Hu3DModelLink(lbl_1_bss_39C[0]);
        }
        {
            HuVecF lightPos = { 0.0f, 1000.0f, 5000.0f };
            HuVecF lightDir = { 0.0f, 0.0f, -1.0f };
            Hu3DLLightCreateV(lbl_1_bss_39C[i], &lightPos, &lightDir, &lbl_1_data_84[lbl_1_bss_10]);
        }
        Hu3DMotionSet(lbl_1_bss_39C[i], lbl_1_bss_38C[0]);
        Hu3DModelCameraSet(lbl_1_bss_39C[i], lbl_1_data_74[i]);
        Hu3DModelAttrSet(lbl_1_bss_39C[i], HU3D_ATTR_DISPOFF);
    }
    lbl_1_bss_300 = Hu3DModelCreateData(DATANUM(DATA_m612, 0x17));
    lbl_1_bss_2FC = Hu3DJointMotionData(lbl_1_bss_300, DATANUM(DATA_m612, 0x18));
    lbl_1_bss_2F8 = Hu3DJointMotionData(lbl_1_bss_300, DATANUM(DATA_m612, 0x19));
    lbl_1_bss_2F4 = Hu3DJointMotionData(lbl_1_bss_300, DATANUM(DATA_m612, 0x1A));
    {
        HuVecF stagePos = { 0.0f, 1000.0f, 5000.0f };
        HuVecF stageDir = { 0.0f, 0.0f, -1.0f };
        Hu3DLLightCreateV(lbl_1_bss_300, &stagePos, &stageDir, &lbl_1_data_84[lbl_1_bss_10]);
    }
    Hu3DModelPosSet(lbl_1_bss_300, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrSet(lbl_1_bss_300, HU3D_MOTATTR_LOOP);
    Hu3DMotionSet(lbl_1_bss_300, lbl_1_bss_2FC);
    lbl_1_bss_308 = 0;
    lbl_1_bss_2F0 = 0;
    lbl_1_bss_304 = 0;
    CenterM[0].y = 1000.0f;
    CenterM[0].z = 400.0f;
    CRotM[0].x = 0.0f;
    lbl_1_bss_4AC.mode = 0;
    lbl_1_bss_4AC.unk19C = omAddObjEx(lbl_1_bss_4AC.objman, 0x7FDA, 1, 0, -1, fn_1_5924);
    omAddObjEx(lbl_1_bss_4AC.objman, 0x7FDA, 1, 0, -1, fn_1_54D4);
}

void fn_1_8F0(void)
{
    if (lbl_1_bss_10 == 0) {
        lbl_1_bss_44 = Hu3DModelCreateData(DATANUM(DATA_m612, 0x02));
        lbl_1_bss_40 = Hu3DModelCreateData(DATANUM(DATA_m612, 0x03));
        lbl_1_bss_3C = Hu3DModelCreateData(DATANUM(DATA_m612, 0x08));
        lbl_1_bss_38 = Hu3DModelCreateData(DATANUM(DATA_m612, 0x04));
        lbl_1_bss_34 = Hu3DModelCreateData(DATANUM(DATA_m612, 0x05));
        lbl_1_bss_30 = Hu3DModelCreateData(DATANUM(DATA_m612, 0x06));
        lbl_1_bss_2C = Hu3DModelCreateData(DATANUM(DATA_m612, 0x07));
        lbl_1_bss_28 = -1;
        lbl_1_bss_20 = -1;
        lbl_1_bss_24 = -1;
    } else {
        lbl_1_bss_44 = Hu3DModelCreateData(DATANUM(DATA_m612, 0x0B));
        lbl_1_bss_40 = Hu3DModelCreateData(DATANUM(DATA_m612, 0x0C));
        lbl_1_bss_3C = Hu3DModelCreateData(DATANUM(DATA_m612, 0x11));
        lbl_1_bss_38 = Hu3DModelCreateData(DATANUM(DATA_m612, 0x0D));
        lbl_1_bss_34 = Hu3DModelCreateData(DATANUM(DATA_m612, 0x0E));
        lbl_1_bss_30 = Hu3DModelCreateData(DATANUM(DATA_m612, 0x0F));
        lbl_1_bss_2C = Hu3DModelCreateData(DATANUM(DATA_m612, 0x10));
        lbl_1_bss_20 = Hu3DModelCreateData(DATANUM(DATA_m612, 0x12));
        Hu3DModelLayerSet(lbl_1_bss_20, 7);
        Hu3DModelAttrSet(lbl_1_bss_20, HU3D_MOTATTR_LOOP);
        Hu3DModelCameraSet(lbl_1_bss_20, 15);
        lbl_1_bss_28 = Hu3DModelCreateData(DATANUM(DATA_m612, 0x13));
        Hu3DModelLayerSet(lbl_1_bss_28, 7);
        Hu3DModelAttrSet(lbl_1_bss_28, HU3D_MOTATTR_LOOP);
        Hu3DModelCameraSet(lbl_1_bss_28, 15);
        Hu3DModelAttrSet(lbl_1_bss_28, HU3D_ATTR_DISPOFF);
        lbl_1_bss_24 = Hu3DModelCreateData(DATANUM(DATA_m612, 0x14));
        Hu3DModelLayerSet(lbl_1_bss_24, 7);
        Hu3DModelAttrSet(lbl_1_bss_24, HU3D_MOTATTR_LOOP);
        Hu3DModelCameraSet(lbl_1_bss_24, 15);
        Hu3DModelAttrSet(lbl_1_bss_24, HU3D_ATTR_DISPOFF);
    }
    Hu3DModelPosSet(lbl_1_bss_44, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(lbl_1_bss_44, 15);
    Hu3DModelLayerSet(lbl_1_bss_44, 1);
    Hu3DModelAttrSet(lbl_1_bss_44, HU3D_MOTATTR_LOOP);
    Hu3DModelCameraSet(lbl_1_bss_40, 15);
    Hu3DModelLayerSet(lbl_1_bss_40, 1);
    Hu3DModelAttrSet(lbl_1_bss_40, HU3D_MOTATTR_LOOP);
    Hu3DModelCameraSet(lbl_1_bss_3C, 15);
    Hu3DModelLayerSet(lbl_1_bss_3C, 0);
    Hu3DModelAttrSet(lbl_1_bss_3C, HU3D_MOTATTR_PAUSE);
    Hu3DModelAttrSet(lbl_1_bss_3C, HU3D_MOTATTR_LOOP);
    Hu3DModelCameraSet(lbl_1_bss_38, 15);
    Hu3DModelLayerSet(lbl_1_bss_38, 2);
    Hu3DModelAttrSet(lbl_1_bss_38, HU3D_MOTATTR_LOOP);
    Hu3DModelLayerSet(lbl_1_bss_34, 6);
    Hu3DModelAttrSet(lbl_1_bss_30, HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(lbl_1_bss_30, 2);
    Hu3DModelAttrSet(lbl_1_bss_2C, HU3D_ATTR_DISPOFF);
    Hu3DModelLayerSet(lbl_1_bss_2C, 3);
}

void fn_1_F0C(void)
{
    s32 i;
    s32 j;
    s32 k;
    s32 unused;

    if (lbl_1_bss_10 == 0) {
        lbl_1_bss_4AC.panelMdlId[0][0] = Hu3DModelCreateData(DATANUM(DATA_m612, 0x00));
    } else {
        lbl_1_bss_4AC.panelMdlId[0][0] = Hu3DModelCreateData(DATANUM(DATA_m612, 0x09));
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 7; j++) {
            for (k = 0; k < 7; k++) {
                if (!((k == 0) & (j == 0) & (i == 0))) {
                    lbl_1_bss_4AC.panelMdlId[i][k + (j * 7)] = Hu3DModelLink(lbl_1_bss_4AC.panelMdlId[0][0]);
                }
                Hu3DModelAttrSet(lbl_1_bss_4AC.panelMdlId[i][k + (j * 7)], HU3D_MOTATTR_PAUSE);
                Hu3DModelPosSet(lbl_1_bss_4AC.panelMdlId[i][k + (j * 7)], (k * 200) - 600, 0.0f, (j * 200) - 1400);
                Hu3DModelCameraSet(lbl_1_bss_4AC.panelMdlId[i][k + (j * 7)], lbl_1_data_74[i]);
                Hu3DModelLayerSet(lbl_1_bss_4AC.panelMdlId[i][k + (j * 7)], 5);
            }
        }
    }
    unused = 0;
    lbl_1_bss_4AC.unk194 = Hu3DModelLink(lbl_1_bss_4AC.panelMdlId[0][0]);
    Hu3DModelPosSet(lbl_1_bss_4AC.unk194, 0.0f, 0.0f, 0.0f);
    Hu3DModelLayerSet(lbl_1_bss_4AC.unk194, 5);
    Hu3DModelAttrReset(lbl_1_bss_4AC.unk194, HU3D_MOTATTR_PAUSE);
    lbl_1_bss_4AC.unk196 = Hu3DModelLink(lbl_1_bss_4AC.panelMdlId[0][0]);
    Hu3DModelPosSet(lbl_1_bss_4AC.unk196, 0.0f, 0.0f, -1600.0f);
    Hu3DModelLayerSet(lbl_1_bss_4AC.unk196, 5);
    Hu3DModelAttrReset(lbl_1_bss_4AC.unk196, HU3D_MOTATTR_PAUSE);
    if (lbl_1_bss_10 == 0) {
        lbl_1_bss_2C0[0] = Hu3DModelCreateData(DATANUM(DATA_m612, 0x01));
    } else {
        lbl_1_bss_2C0[0] = Hu3DModelCreateData(DATANUM(DATA_m612, 0x0A));
    }
    for (i = 0; i < 4; i++) {
        if (i != 0) {
            lbl_1_bss_2C0[i] = Hu3DModelLink(lbl_1_bss_2C0[0]);
        }
        Hu3DModelAttrSet(lbl_1_bss_2C0[i], HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(lbl_1_bss_2C0[i], HU3D_MOTATTR_PAUSE);
        Hu3DModelCameraSet(lbl_1_bss_2C0[i], lbl_1_data_74[i]);
        Hu3DModelLayerSet(lbl_1_bss_2C0[i], 4);
    }
}

s32 fn_1_13A4(s32 x, s32 z)
{
    s32 bit;
    s8 row;
    s32 mask;

    row = lbl_1_data_16B0[lbl_1_bss_8][z];
    if (lbl_1_bss_C) {
        bit = x;
    } else {
        bit = 6 - x;
    }
    mask = 1 << bit;
    if (mask & row) {
        return 1;
    } else {
        return 0;
    }
}

void fn_1_1430(void)
{
    s32 i;

    lbl_1_bss_8 = rand8() % 8;
    lbl_1_bss_4 = (lbl_1_bss_8 * 9) + (rand8() % 4);
    lbl_1_bss_C = rand8() & 1;
    for (i = 0; lbl_1_data_198[lbl_1_bss_4][i] != 0; i++) {
        if (lbl_1_bss_C) {
            switch (lbl_1_data_198[lbl_1_bss_4][i]) {
                case 1:
                    lbl_1_bss_48[i] = 2;
                    break;
                case 2:
                    lbl_1_bss_48[i] = 1;
                    break;
                default:
                    lbl_1_bss_48[i] = lbl_1_data_198[lbl_1_bss_4][i];
                    break;
            }
        } else {
            lbl_1_bss_48[i] = lbl_1_data_198[lbl_1_bss_4][i];
        }
    }
    lbl_1_bss_48[i] = 0;
    for (i = 0; i < 4; i++) {
        lbl_1_bss_3BC[i] = (lbl_1_bss_3BC[i] & 0xF) | 0x30;
        lbl_1_bss_3BC[i] = (lbl_1_bss_3BC[i] & 0xF0) | 7;
    }
}

void fn_1_1694(s16 mode, s16 frameNo)
{
    lbl_1_bss_14 = 0;
    lbl_1_bss_18 = 0;
    MgSeqModeNext();
}

void fn_1_16D4(s16 mode, s16 frameNo)
{
    s32 i;
    s32 k;
    s32 j;
    s32 time;
    s32 time2;

    switch (lbl_1_bss_4AC.mode) {
        case 0:
            MgSeqModeChangeOff();
            lbl_1_bss_2EC = 0.0f;
            lbl_1_bss_2E8 = 0.0f;
            lbl_1_bss_14 = 0;
            lbl_1_bss_4AC.mode = 1;
            HuAudFXPlay(0x6A5);
            break;
        case 1:
            lbl_1_bss_2EC = (1.0f / 60.0f) * lbl_1_bss_14;
            CenterM[0].z = (1000.0f * lbl_1_bss_2EC) - 600.0f;
            lbl_1_bss_14++;
            if (lbl_1_bss_14 == 60) {
                lbl_1_bss_14 = 0;
                lbl_1_bss_4AC.mode = 2;
            }
            break;
        case 2:
            lbl_1_bss_2E8 = (1.0f / 60.0f) * lbl_1_bss_14;
            CRotM[0].x = -40.0f * lbl_1_bss_2E8;
            lbl_1_bss_14++;
            if (lbl_1_bss_14 == 60) {
                lbl_1_bss_14 = 0;
                lbl_1_bss_308 = 2;
                lbl_1_bss_4AC.mode = 3;
            }
            break;
        case 3:
            if (lbl_1_bss_14 % 120 == 0) {
                HuAudFXPlay(0x6A5);
            }
            lbl_1_bss_14++;
            if (lbl_1_bss_308 == 6) {
                lbl_1_bss_4AC.mode = 4;
                lbl_1_bss_14 = 0;
            }
            break;
        case 4:
            lbl_1_bss_14++;
            if (lbl_1_bss_14 > 60) {
                lbl_1_bss_14 = 0;
                lbl_1_bss_4AC.mode = 5;
            }
            break;
        case 5:
            WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
            for (time = 0; time < 60; time++) {
                HuPrcVSleep();
            }
            for (j = 0; j < 7; j++) {
                for (k = 0; k < 7; k++) {
                    Hu3DModelAttrSet(lbl_1_bss_4AC.panelMdlId[0][k + (j * 7)], HU3D_MOTATTR_PAUSE);
                    Hu3DMotionTimeSet(lbl_1_bss_4AC.panelMdlId[0][k + (j * 7)], 0.0f);
                }
            }
            Hu3DModelAttrSet(lbl_1_bss_300, HU3D_ATTR_DISPOFF);
            Hu3DModelAttrSet(lbl_1_bss_38, HU3D_ATTR_DISPOFF);
            Hu3DModelAttrReset(lbl_1_bss_30, HU3D_ATTR_DISPOFF);
            Hu3DModelAttrReset(lbl_1_bss_2C, HU3D_ATTR_DISPOFF);
            if (lbl_1_bss_10 == 1) {
                Hu3DModelAttrSet(lbl_1_bss_20, HU3D_ATTR_DISPOFF);
                Hu3DModelAttrReset(lbl_1_bss_28, HU3D_ATTR_DISPOFF);
                Hu3DModelAttrReset(lbl_1_bss_24, HU3D_ATTR_DISPOFF);
            }
            lbl_1_bss_4AC.unk198 = omAddObjEx(lbl_1_bss_4AC.objman, 0x7FDA, 1, 0, -1, fn_1_2C94);
            lbl_1_bss_4AC.mode = 6;
            break;
        case 6:
            lbl_1_bss_14 = 60;
            Hu3DModelAttrSet(lbl_1_bss_40, HU3D_ATTR_DISPOFF);
            fn_1_51D8();
            lbl_1_bss_4AC.mode = 8;
            for (i = 0; i < 4; i++) {
                Hu3DModelAttrReset(lbl_1_bss_48C[i], HU3D_ATTR_DISPOFF);
            }
            for (i = 0; i < 4; i++) {
                CenterM[i].x = 0.0f;
                CenterM[i].y = 150.0f;
                CenterM[i].z = 0.0f;
                CRotM[i].x = -40.0f;
                CRotM[i].y = 0.0f;
                CRotM[i].z = 0.0f;
                CZoomM[i] = 800.0f;
            }
            WipeCreate(WIPE_MODE_IN, WIPE_TYPE_PREV, 60);
            for (time2 = 0; time2 < 60; time2++) {
                HuPrcVSleep();
            }
            break;
        case 7:
            lbl_1_bss_14--;
            fn_1_4B94(0);
            if (lbl_1_bss_14 == 0) {
                lbl_1_bss_4AC.mode = 8;
            }
            break;
        case 8:
            MgSeqModeNext();
            lbl_1_bss_4AC.mode = 9;
            break;
    }
}

void fn_1_1DBC(s16 mode, s16 frameNo)
{
    if (lbl_1_data_8C == -1 && GameMesFXPlayCheck(MgSeqGameMesIdGet())) {
        lbl_1_data_8C = HuAudBGMPlay(0x53);
    }
}

void fn_1_1E18(s16 mode, s16 frameNo)
{
    s32 i;
    s32 num;
    s32 list[4];

    switch (lbl_1_bss_4AC.mode) {
        case 9:
            lbl_1_bss_0 = -1;
            lbl_1_bss_4AC.mode = 10;
        case 10:
            for (i = 0, num = 0; i < 4; i++) {
                if (lbl_1_bss_42C[i] == 11) {
                    list[num] = i;
                    num++;
                }
            }
            if (num == 0) {
                break;
            }
            if (num != 1) {
                lbl_1_bss_0 = list[rand8() % num];
            } else {
                lbl_1_bss_0 = list[0];
            }
            lbl_1_bss_14 = 0;
            for (i = 0; i < 4; i++) {
                if (lbl_1_bss_42C[i] == 2) {
                    lbl_1_bss_42C[i] = 1;
                    CharMotionSet(lbl_1_bss_49C[i], lbl_1_bss_44C[i][0]);
                }
            }
            lbl_1_bss_4AC.mode = 12;
            MgSeqModeNext();
            break;
    }
}

void fn_1_1FB4(s16 mode, s16 frameNo)
{
    s16 i;
    s16 num;
    s16 winner[4];

    if (MgSeqFrameNoGet() == 0 && lbl_1_data_8C != -1) {
        HuAudSStreamFadeOut(lbl_1_data_8C, 100);
        lbl_1_data_8C = -1;
    }
    switch (lbl_1_bss_4AC.mode) {
        case 10:
            for (i = 0; i < 4; i++) {
                winner[i] = -1;
            }
            MgSeqWinnerSet(winner[0], winner[1], winner[2], winner[3]);
            lbl_1_bss_14 = 0;
            for (i = 0; i < 4; i++) {
                if (lbl_1_bss_42C[i] == 2 || lbl_1_bss_42C[i] == 3) {
                    CharMotionShiftSet(lbl_1_bss_49C[i], lbl_1_bss_44C[i][0], 0.0f, 5.0f, HU3D_MOTATTR_NONE);
                    lbl_1_bss_42C[i] = 1;
                }
            }
            lbl_1_bss_4AC.mode = 18;
            break;
        case 11:
            for (i = 0, num = 0; i < 4; i++) {
                if (lbl_1_bss_42C[i] == 1) {
                    num++;
                }
            }
            if (num == 4) {
                for (i = 0; i < 4; i++) {
                    CharMotionSet(lbl_1_bss_49C[i], lbl_1_bss_44C[i][3]);
                    Hu3DModelRotSet(lbl_1_bss_48C[i], 0.0f, 0.0f, 0.0f);
                    Hu3DModelAttrReset(lbl_1_bss_48C[i], HU3D_MOTATTR_LOOP);
                }
                lbl_1_bss_4AC.mode = 19;
            }
            break;
        case 12:
            lbl_1_bss_14 = 0;
            for (i = 0; i < 4; i++) {
                winner[i] = -1;
                if (lbl_1_bss_42C[i] == 2 || lbl_1_bss_42C[i] == 3) {
                    CharMotionShiftSet(lbl_1_bss_49C[i], lbl_1_bss_44C[i][0], 0.0f, 10.0f, HU3D_MOTATTR_LOOP);
                    lbl_1_bss_42C[i] = 1;
                }
            }
            winner[lbl_1_bss_0] = lbl_1_bss_49C[lbl_1_bss_0];
            MgSeqWinnerSet(winner[0], winner[1], winner[2], winner[3]);
            lbl_1_bss_4AC.mode = 13;
            GWMgCoinBonusSet(lbl_1_bss_0, 10);
            break;
    }
}

void fn_1_242C(s16 mode, s16 frameNo)
{
    s32 i;
    s32 j;

    switch (lbl_1_bss_4AC.mode) {
        case 13:
            lbl_1_bss_14 = 0;
            lbl_1_bss_4AC.mode = 14;
        case 14:
            if (lbl_1_bss_14 >= 60) {
                lbl_1_bss_4AC.mode = 15;
            }
            lbl_1_bss_14++;
            break;
        case 15:
            lbl_1_bss_14 = 0;
            Hu3DModelAttrReset(lbl_1_bss_40, HU3D_ATTR_DISPOFF);
            if (lbl_1_bss_10 == 1) {
                Hu3DModelAttrSet(lbl_1_bss_28, HU3D_ATTR_DISPOFF);
                Hu3DModelAttrSet(lbl_1_bss_24, HU3D_ATTR_DISPOFF);
                Hu3DModelAttrReset(lbl_1_bss_20, HU3D_ATTR_DISPOFF);
            }
            WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
            for (i = 0; i < 60; i++) {
                lbl_1_bss_14++;
                if (lbl_1_bss_14 <= 60) {
                    fn_1_4B94(lbl_1_bss_0);
                }
                HuPrcVSleep();
            }
            lbl_1_bss_14 = 0;
            Hu3DModelAttrSet(lbl_1_bss_30, HU3D_ATTR_DISPOFF);
            Hu3DModelAttrSet(lbl_1_bss_2C, HU3D_ATTR_DISPOFF);
            Hu3DModelAttrReset(lbl_1_bss_38, HU3D_ATTR_DISPOFF);
            {
                HuVecF lightPos = { 0.0f, 3000.0f, 3000.0f };
                HuVecF lightDir = { 0.0f, 0.0f, -1.0f };
                HuVecF pos;
                CharLightCreateV(&lightPos, &lightDir, &lbl_1_data_84[lbl_1_bss_10]);
            CharLightStaticSet(TRUE);
            Hu3DCameraKill(-2);
            pos.x = -130.0f;
            pos.y = 1519.668f;
            pos.z = -2800.8f;
            Hu3DModelCameraSet(lbl_1_bss_48C[lbl_1_bss_0], 1);
            Hu3DModelPosSetV(lbl_1_bss_48C[lbl_1_bss_0], &pos);
            CenterM[0].x = 0.0f;
            CenterM[0].y = 1800.0f;
            CenterM[0].z = -2500.0f;
            CRotM[0].x = -20.0f;
            Hu3DCameraPerspectiveSet(1, 45.0f, 20.0f, 15000.0f, 1.2f);
            Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
            Hu3DCameraScissorSet(1, 0, 0, 640, 480);
            Hu3DModelAttrReset(lbl_1_bss_48C[lbl_1_bss_0], HU3D_MOTATTR_LOOP);
            pos.x = 150.0f;
            Hu3DModelPosSetV(lbl_1_bss_300, &pos);
            Hu3DModelAttrReset(lbl_1_bss_300, HU3D_ATTR_DISPOFF);
            Hu3DModelRotSet(lbl_1_bss_300, 0.0f, 0.0f, 0.0f);
            Hu3DMotionSet(lbl_1_bss_300, lbl_1_bss_2F4);
            Hu3DModelAttrReset(lbl_1_bss_3C, HU3D_MOTATTR_PAUSE);
            Hu3DModelAttrSet(lbl_1_bss_3C, HU3D_MOTATTR_LOOP);
            {
                HuVecF shadowPos = { 0.0f, 5000.0f, -2000.0f };
                HuVecF shadowUp = { 0.0f, 1.0f, 0.0f };
                HuVecF shadowTarget = { 0.0f, 0.0f, -2600.0f };
                Hu3DShadowCreate(30.0f, 20.0f, 5000.0f);
                Hu3DShadowPosSet(&shadowPos, &shadowUp, &shadowTarget);
            }
            if (lbl_1_bss_10 == 0) {
                Hu3DShadowTPLvlSet(0.7f);
                Hu3DShadowColSet(0x80, 0x80, 0x80);
            } else {
                Hu3DShadowTPLvlSet(0.3f);
                Hu3DShadowColSet(0x10, 0x10, 0x10);
            }
            Hu3DModelShadowMapSet(lbl_1_bss_40);
            Hu3DModelShadowSet(lbl_1_bss_300);
            Hu3DModelShadowSet(lbl_1_bss_48C[lbl_1_bss_0]);
            }
            lbl_1_bss_4AC.mode = 16;
            break;
        case 16:
            WipeCreate(WIPE_MODE_IN, WIPE_TYPE_PREV, 60);
            for (i = 0; i < 60; i++) {
                HuPrcVSleep();
            }
            lbl_1_bss_14 = 0;
            lbl_1_bss_4AC.mode = 17;
            break;
        case 17:
            lbl_1_bss_14 = 0;
            MgSeqModeNext();
            break;
        case 18:
            if (lbl_1_bss_14 >= 60) {
                for (j = 0; j < 4; j++) {
                    CharMotionSet(lbl_1_bss_49C[j], lbl_1_bss_44C[j][3]);
                    Hu3DModelRotSet(lbl_1_bss_48C[j], 0.0f, 0.0f, 0.0f);
                    Hu3DModelAttrReset(lbl_1_bss_48C[j], HU3D_MOTATTR_LOOP);
                }
                MgSeqModeNext();
            }
            lbl_1_bss_14++;
            break;
        case 19:
            break;
    }
}

void fn_1_2BC8(s16 mode, s16 frameNo)
{
    if (lbl_1_bss_0 != -1) {
        if (lbl_1_bss_14 == 0) {
            CharMotionShiftSet(lbl_1_bss_49C[lbl_1_bss_0], lbl_1_bss_44C[lbl_1_bss_0][2], 0.0f, 10.0f, HU3D_MOTATTR_NONE);
        }
        lbl_1_bss_14++;
    }
}

void fn_1_2C8C(s16 mode, s16 frameNo)
{
}

void fn_1_2C90(s16 mode, s16 frameNo)
{
}

void fn_1_2C94(OMOBJ *obj)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        switch (lbl_1_bss_4AC.mode) {
            case 9:
                break;
            default:
                fn_1_2CEC(i);
                break;
        }
    }
}

void fn_1_2CEC(s32 playerNo)
{
    s32 x;
    s32 z;
    s32 nextX;
    s32 nextZ;
    s32 curX;
    s32 curZ;
    s32 result;
    float rotY;
    float t;
    HuVecF pos;
    HuVecF panelPos;
    HuVecF fallPos;
    HuVecF risePos;
    HuVecF bouncePos;

    switch (lbl_1_bss_42C[playerNo]) {
        case 0:
            lbl_1_bss_3BC[playerNo] = 0x37;
            lbl_1_bss_42C[playerNo] = 1;
            lbl_1_bss_3AC[playerNo] = 0;
            lbl_1_bss_3CC[playerNo] = 0;
            break;
        case 1:
        case 3:
            if (lbl_1_bss_4AC.mode != 10) {
                break;
            }
            if (GwPlayerConf[playerNo].type == 0) {
                lbl_1_bss_41C[playerNo] = fn_1_4038(playerNo);
            } else {
                lbl_1_bss_41C[playerNo] = fn_1_4480(playerNo);
            }
            if (lbl_1_bss_41C[playerNo] != 0) {
                curX = (lbl_1_bss_3BC[playerNo] & 0xF0) >> 4;
                curZ = lbl_1_bss_3BC[playerNo] & 0xF;
                rotY = fn_1_5E00(lbl_1_bss_41C[playerNo]);
                switch (lbl_1_bss_41C[playerNo]) {
                    case 2:
                        curX++;
                        break;
                    case 1:
                        curX--;
                        break;
                    case 4:
                        curZ++;
                        break;
                    case 3:
                        curZ--;
                        break;
                }
                if (fn_1_4150(playerNo)) {
                    if (lbl_1_bss_42C[playerNo] == 1) {
                        CharMotionShiftSet(lbl_1_bss_49C[playerNo], lbl_1_bss_44C[playerNo][1], 0.0f, 10.0f, HU3D_MOTATTR_LOOP);
                    }
                    lbl_1_bss_40C[playerNo] = 20;
                    lbl_1_bss_42C[playerNo] = 2;
                    Hu3DModelRotSet(lbl_1_bss_48C[playerNo], 0.0f, rotY, 0.0f);
                } else {
                    Hu3DModelRotSet(lbl_1_bss_48C[playerNo], 0.0f, rotY, 0.0f);
                    if (lbl_1_bss_44C[playerNo][0] != CharMotionShiftIDGet(lbl_1_bss_49C[playerNo])) {
                        CharMotionShiftSet(lbl_1_bss_49C[playerNo], lbl_1_bss_44C[playerNo][0], 0.0f, 5.0f, HU3D_MOTATTR_NONE);
                        lbl_1_bss_42C[playerNo] = 1;
                    }
                    return;
                }
            } else {
                if (lbl_1_bss_42C[playerNo] == 3) {
                    CharMotionSet(lbl_1_bss_49C[playerNo], lbl_1_bss_44C[playerNo][0]);
                    lbl_1_bss_42C[playerNo] = 1;
                }
                return;
            }
        case 2:
            lbl_1_bss_40C[playerNo]--;
            if (lbl_1_bss_40C[playerNo] >= 0) {
                Hu3DModelPosGet(lbl_1_bss_48C[playerNo], &pos);
                switch (lbl_1_bss_41C[playerNo]) {
                    case 1:
                        pos.x -= 10.0f;
                        CenterM[playerNo].x -= 10.0f;
                        break;
                    case 2:
                        pos.x += 10.0f;
                        CenterM[playerNo].x += 10.0f;
                        break;
                    case 3:
                        pos.z -= 10.0f;
                        CenterM[playerNo].z -= 10.0f;
                        break;
                    case 4:
                        pos.z += 10.0f;
                        CenterM[playerNo].z += 10.0f;
                        break;
                }
                Hu3DModelPosSetV(lbl_1_bss_48C[playerNo], &pos);
            }
            if (lbl_1_bss_40C[playerNo] != 0) {
                break;
            }
            result = fn_1_42C4(playerNo);
            if (result == 2) {
                x = (lbl_1_bss_3BC[playerNo] & 0xF0) >> 4;
                z = lbl_1_bss_3BC[playerNo] & 0xF;
                switch (lbl_1_bss_41C[playerNo]) {
                    case 2:
                        x++;
                        break;
                    case 1:
                        x--;
                        break;
                    case 4:
                        z++;
                        break;
                    case 3:
                        z--;
                        break;
                }
                if (x >= 0 && x < 7 && z >= 0 && z < 7 && !(Hu3DModelAttrGet(lbl_1_bss_4AC.panelMdlId[playerNo][x + (z * 7)]) & HU3D_ATTR_DISPOFF)) {
                    nextX = (lbl_1_bss_3BC[playerNo] & 0xF0) >> 4;
                    nextZ = lbl_1_bss_3BC[playerNo] & 0xF;
                    switch (lbl_1_bss_41C[playerNo]) {
                        case 2:
                            nextX++;
                            break;
                        case 1:
                            nextX--;
                            break;
                        case 3:
                            nextZ--;
                            break;
                        case 4:
                            nextZ++;
                            break;
                    }
                    panelPos.x = (nextX * 200) - 600;
                    panelPos.y = 0.0f;
                    panelPos.z = (nextZ * 200) - 1400;
                    Hu3DModelPosSetV(lbl_1_bss_2C0[playerNo], &panelPos);
                    Hu3DModelAttrReset(lbl_1_bss_2C0[playerNo], HU3D_ATTR_DISPOFF);
                    Hu3DModelAttrReset(lbl_1_bss_2C0[playerNo], HU3D_MOTATTR_PAUSE);
                    Hu3DModelAttrSet(lbl_1_bss_4AC.panelMdlId[playerNo][nextX + (nextZ * 7)], HU3D_ATTR_DISPOFF);
                    lbl_1_bss_30C[playerNo] = 0;
                    CharMotionShiftSet(lbl_1_bss_49C[playerNo], lbl_1_bss_44C[playerNo][4], 0.0f, 10.0f, HU3D_MOTATTR_NONE);
                    lbl_1_bss_42C[playerNo] = 4;
                    HuAudFXPlay(0x6A2);
                    lbl_1_bss_1C = 1;
                } else {
                    CharMotionShiftSet(lbl_1_bss_49C[playerNo], lbl_1_bss_44C[playerNo][5], 0.0f, 10.0f, HU3D_MOTATTR_NONE);
                    omVibrate(playerNo, 20, 7, 3);
                    lbl_1_bss_42C[playerNo] = 5;
                    lbl_1_bss_1C = 0;
                }
                lbl_1_bss_3EC[playerNo] = 0;
                break;
            }
            x = (lbl_1_bss_3BC[playerNo] & 0xF0) >> 4;
            z = lbl_1_bss_3BC[playerNo] & 0xF;
            switch (lbl_1_bss_41C[playerNo]) {
                case 2:
                    x++;
                    break;
                case 1:
                    x--;
                    break;
                case 4:
                    z++;
                    break;
                case 3:
                    z--;
                    break;
            }
            lbl_1_bss_3BC[playerNo] = (x << 4) | (z & 0xF);
            if (result == 3) {
                Hu3DModelRotSet(lbl_1_bss_48C[playerNo], 0.0f, 0.0f, 0.0f);
                CharMotionSet(lbl_1_bss_49C[playerNo], lbl_1_bss_44C[playerNo][0]);
                lbl_1_bss_42C[playerNo] = 11;
                break;
            }
            if (x >= 0 && x < 7 && z >= 0 && z < 7) {
                Hu3DModelAttrReset(lbl_1_bss_4AC.panelMdlId[playerNo][x + (z * 7)], HU3D_MOTATTR_PAUSE);
            }
            if (result == 0) {
                lbl_1_bss_3AC[playerNo]++;
            } else {
                lbl_1_bss_3AC[playerNo]--;
            }
            lbl_1_bss_42C[playerNo] = 3;
            lbl_1_bss_3CC[playerNo]++;
            break;
        case 4:
            if (lbl_1_bss_30C[playerNo] > 40) {
                omVibrate(playerNo, 20, 7, 3);
                CharMotionShiftSet(lbl_1_bss_49C[playerNo], lbl_1_bss_44C[playerNo][5], 0.0f, 10.0f, HU3D_MOTATTR_NONE);
                lbl_1_bss_42C[playerNo] = 5;
            }
            lbl_1_bss_30C[playerNo]++;
            break;
        case 5:
            t = 0.0f;
            Hu3DModelPosGet(lbl_1_bss_48C[playerNo], &fallPos);
            t = (1.0f / 30.0f) * lbl_1_bss_3EC[playerNo];
            fallPos.y = -(800.0f * t);
            Hu3DModelPosSetV(lbl_1_bss_48C[playerNo], &fallPos);
            if (lbl_1_bss_3EC[playerNo] == 0 && lbl_1_bss_1C == 1) {
                HuAudFXPlay(0x6A4);
            }
            lbl_1_bss_3EC[playerNo]++;
            if (lbl_1_bss_3EC[playerNo] == 30) {
                lbl_1_bss_36C[playerNo] = 2;
                lbl_1_bss_3EC[playerNo] = 0;
                lbl_1_bss_42C[playerNo] = 6;
                Hu3DModelAttrSet(lbl_1_bss_48C[playerNo], HU3D_ATTR_DISPOFF);
            }
            lbl_1_bss_3CC[playerNo] = 0;
            break;
        case 6:
            if (lbl_1_bss_36C[playerNo] == 5) {
                Hu3DModelAttrSet(lbl_1_bss_2C0[playerNo], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrSet(lbl_1_bss_2C0[playerNo], HU3D_MOTATTR_PAUSE);
                Hu3DMotionTimeSet(lbl_1_bss_2C0[playerNo], 0.0f);
                CharMotionSet(lbl_1_bss_49C[playerNo], lbl_1_bss_44C[playerNo][6]);
                lbl_1_bss_3EC[playerNo] = 0;
                lbl_1_bss_42C[playerNo] = 7;
                Hu3DModelAttrReset(lbl_1_bss_48C[playerNo], HU3D_ATTR_DISPOFF);
            }
            break;
        case 7:
            Hu3DModelPosGet(lbl_1_bss_39C[playerNo], &risePos);
            risePos.y -= 300.0f;
            if (risePos.y > 0.0f) {
                risePos.y = 0.0f;
            }
            Hu3DModelPosSetV(lbl_1_bss_48C[playerNo], &risePos);
            if (risePos.y >= 0.0f) {
                lbl_1_bss_40C[playerNo] = 20;
                lbl_1_bss_42C[playerNo] = 8;
                lbl_1_bss_3DC[playerNo] = 30.0f;
                lbl_1_bss_3EC[playerNo] = 0;
            }
            break;
        case 8:
            lbl_1_bss_40C[playerNo]--;
            if (lbl_1_bss_40C[playerNo] >= 0) {
                Hu3DModelPosGet(lbl_1_bss_48C[playerNo], &bouncePos);
                lbl_1_bss_3DC[playerNo] -= 3.0f;
                bouncePos.y += lbl_1_bss_3DC[playerNo];
                if (bouncePos.y < 0.0f) {
                    bouncePos.y = 0.0f;
                }
                switch (lbl_1_bss_41C[playerNo]) {
                    case 1:
                        bouncePos.x += 10.0f;
                        CenterM[playerNo].x += 10.0f;
                        break;
                    case 2:
                        bouncePos.x -= 10.0f;
                        CenterM[playerNo].x -= 10.0f;
                        break;
                    case 3:
                        bouncePos.z += 10.0f;
                        CenterM[playerNo].z += 10.0f;
                        break;
                    case 4:
                        bouncePos.z -= 10.0f;
                        CenterM[playerNo].z -= 10.0f;
                        break;
                }
                Hu3DModelPosSetV(lbl_1_bss_48C[playerNo], &bouncePos);
            } else {
                CharMotionShiftSet(lbl_1_bss_49C[playerNo], lbl_1_bss_44C[playerNo][0], 0.0f, 10.0f, HU3D_MOTATTR_NONE);
                lbl_1_bss_42C[playerNo] = 1;
            }
            break;
        case 9:
            break;
        case 10:
            lbl_1_bss_40C[playerNo]--;
            if (lbl_1_bss_40C[playerNo] == 0) {
                lbl_1_bss_42C[playerNo] = 1;
            }
            break;
        case 11:
            break;
    }
}

/* @@PART2@@ */
/* placeholder until part 2 splices in */
s32 fn_1_4038(s32 playerNo) { return 0; }
s32 fn_1_4150(s32 playerNo) { return 0; }
s32 fn_1_42C4(s32 playerNo) { return 0; }
s32 fn_1_4480(s32 playerNo) { return 0; }
void fn_1_48D4(void) { }
void fn_1_4B94(s32 playerNo) { }
void fn_1_51D8(void) { }
void fn_1_54D4(OMOBJ *obj) { }
void fn_1_5924(OMOBJ *obj) { }
float fn_1_5E00(s32 dir) { return 0.0f; }
s8 lbl_1_data_198[90][60] = {
    { 3, 1, 3, 3, 2, 2, 3, 3, 1, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 3, 3, 2, 2, 3, 3, 3, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 3, 3, 2, 2, 3, 3, 3, 3, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 3, 3, 2, 2, 2, 3, 3, 1, 1, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 3, 3, 2, 2, 2, 3, 3, 3, 1, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 3, 3, 2, 2, 2, 3, 3, 3, 3, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 3, 3, 2, 2, 2, 2, 3, 3, 1, 1, 1, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 3, 3, 2, 2, 2, 2, 3, 3, 3, 1, 1, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 3, 3, 2, 2, 2, 2, 3, 3, 3, 3, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 3, 3, 3, 2, 2, 3, 3, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 3, 3, 3, 2, 2, 3, 3, 3, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 3, 3, 3, 2, 2, 2, 3, 3, 1, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 3, 3, 3, 2, 2, 2, 3, 3, 3, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 3, 3, 3, 2, 2, 2, 2, 3, 3, 1, 1, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 3, 3, 3, 2, 2, 2, 2, 3, 3, 3, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 3, 3, 3, 3, 2, 2, 3, 3, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 3, 3, 3, 3, 2, 2, 2, 3, 3, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 3, 3, 3, 3, 2, 2, 2, 2, 3, 3, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 3, 3, 2, 2, 2, 3, 3, 1, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 3, 3, 2, 2, 2, 3, 3, 3, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 3, 3, 2, 2, 2, 3, 3, 3, 3, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 3, 3, 2, 2, 2, 2, 3, 3, 1, 1, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 3, 3, 2, 2, 2, 2, 3, 3, 3, 1, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 3, 3, 2, 2, 2, 2, 3, 3, 3, 3, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 3, 3, 2, 2, 2, 2, 2, 3, 3, 1, 1, 1, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 3, 3, 2, 2, 2, 2, 2, 3, 3, 3, 1, 1, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 3, 3, 2, 2, 2, 2, 2, 3, 3, 3, 3, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 3, 3, 3, 2, 2, 2, 3, 3, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 3, 3, 3, 2, 2, 2, 3, 3, 3, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 3, 3, 3, 2, 2, 2, 2, 3, 3, 1, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 3, 3, 3, 2, 2, 2, 2, 3, 3, 3, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 3, 3, 3, 2, 2, 2, 2, 2, 3, 3, 1, 1, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 3, 3, 3, 2, 2, 2, 2, 2, 3, 3, 3, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 3, 3, 3, 3, 2, 2, 2, 3, 3, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 3, 3, 3, 3, 2, 2, 2, 2, 3, 3, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 3, 3, 3, 3, 2, 2, 2, 2, 2, 3, 3, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 1, 3, 3, 2, 2, 2, 2, 3, 3, 1, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 1, 3, 3, 2, 2, 2, 2, 3, 3, 3, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 1, 3, 3, 2, 2, 2, 2, 3, 3, 3, 3, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 1, 3, 3, 2, 2, 2, 2, 2, 3, 3, 1, 1, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 1, 3, 3, 2, 2, 2, 2, 2, 3, 3, 3, 1, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 1, 3, 3, 2, 2, 2, 2, 2, 3, 3, 3, 3, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 1, 3, 3, 2, 2, 2, 2, 2, 2, 3, 3, 1, 1, 1, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 1, 3, 3, 2, 2, 2, 2, 2, 2, 3, 3, 3, 1, 1, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 1, 3, 3, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 1, 3, 3, 3, 2, 2, 2, 2, 3, 3, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 1, 3, 3, 3, 2, 2, 2, 2, 3, 3, 3, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 1, 3, 3, 3, 2, 2, 2, 2, 2, 3, 3, 1, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 1, 3, 3, 3, 2, 2, 2, 2, 2, 3, 3, 3, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 1, 3, 3, 3, 2, 2, 2, 2, 2, 2, 3, 3, 1, 1, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 1, 3, 3, 3, 2, 2, 2, 2, 2, 2, 3, 3, 3, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 1, 3, 3, 3, 3, 2, 2, 2, 2, 3, 3, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 1, 3, 3, 3, 3, 2, 2, 2, 2, 2, 3, 3, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 1, 1, 1, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 3, 3, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 3, 3, 2, 2, 3, 3, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 3, 3, 2, 2, 3, 3, 3, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 3, 3, 2, 2, 2, 3, 3, 1, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 3, 3, 2, 2, 2, 3, 3, 3, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 3, 3, 2, 2, 2, 2, 3, 3, 1, 1, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 3, 3, 2, 2, 2, 2, 3, 3, 3, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 3, 3, 3, 2, 2, 3, 3, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 3, 3, 3, 2, 2, 2, 3, 3, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 3, 3, 3, 2, 2, 2, 2, 3, 3, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 1, 3, 3, 2, 2, 2, 3, 3, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 1, 3, 3, 2, 2, 2, 3, 3, 3, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 1, 3, 3, 2, 2, 2, 2, 3, 3, 1, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 1, 3, 3, 2, 2, 2, 2, 3, 3, 3, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 1, 3, 3, 2, 2, 2, 2, 2, 3, 3, 1, 1, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 1, 3, 3, 2, 2, 2, 2, 2, 3, 3, 3, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 1, 3, 3, 3, 2, 2, 2, 3, 3, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 1, 3, 3, 3, 2, 2, 2, 2, 3, 3, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 1, 3, 3, 3, 2, 2, 2, 2, 2, 3, 3, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 1, 1, 3, 3, 2, 2, 2, 2, 3, 3, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 1, 1, 3, 3, 2, 2, 2, 2, 3, 3, 3, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 1, 1, 3, 3, 2, 2, 2, 2, 2, 3, 3, 1, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 1, 1, 3, 3, 2, 2, 2, 2, 2, 3, 3, 3, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 1, 1, 3, 3, 2, 2, 2, 2, 2, 2, 3, 3, 1, 1, 1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 1, 1, 3, 3, 2, 2, 2, 2, 2, 2, 3, 3, 3, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 1, 1, 3, 3, 3, 2, 2, 2, 2, 3, 3, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 1, 1, 3, 3, 3, 2, 2, 2, 2, 2, 3, 3, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 1, 1, 1, 3, 3, 3, 2, 2, 2, 2, 2, 2, 3, 3, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 3, 1, 3, 3, 2, 2, 3, 3, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 3, 1, 3, 3, 2, 2, 2, 3, 3, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 3, 1, 3, 3, 2, 2, 2, 2, 3, 3, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 3, 1, 1, 3, 3, 2, 2, 2, 3, 3, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 3, 1, 1, 3, 3, 2, 2, 2, 2, 3, 3, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 3, 1, 1, 3, 3, 2, 2, 2, 2, 2, 3, 3, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 3, 1, 1, 1, 3, 3, 2, 2, 2, 2, 3, 3, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 3, 1, 1, 1, 3, 3, 2, 2, 2, 2, 2, 3, 3, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 3, 1, 1, 1, 3, 3, 2, 2, 2, 2, 2, 2, 3, 3, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
s8 lbl_1_data_16B0[10][7] = {
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
};
/* @@END@@ */
