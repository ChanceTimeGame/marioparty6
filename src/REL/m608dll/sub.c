#include "REL/m608dll/m608.h"

/* .bss is laid out in reverse definition order */
M608_WORK lbl_1_bss_3E80;
M608_WORK3E68 lbl_1_bss_3E68;
M608_WORK3DF0 lbl_1_bss_3DF0[5];

void fn_1_5DA4(HuVecF pos)
{
    lbl_1_bss_3E80.unk428.x = pos.x;
    lbl_1_bss_3E80.unk428.y = 3500.0f + pos.y;
    lbl_1_bss_3E80.unk428.z = pos.z;
    lbl_1_bss_3E80.unk434.x = pos.x;
    lbl_1_bss_3E80.unk434.y = -100.0f;
    lbl_1_bss_3E80.unk434.z = pos.z;
    Hu3DShadowPosSet(&lbl_1_bss_3E80.unk428, &lbl_1_bss_3E80.unk440, &lbl_1_bss_3E80.unk434);
}

void fn_1_5E6C(s32 camNo)
{
    HU3D_MOTIONID motId = lbl_1_bss_3E80.unk10[camNo];
    HU3D_MODELID mdlId = lbl_1_bss_3E80.unk24[camNo];

    Hu3DCameraMotionStart(mdlId, HU3D_CAM0);
    lbl_1_bss_3E80.camTime = 0.0f;
    lbl_1_bss_3E80.camMaxTime = Hu3DMotionMaxTimeGet(mdlId);
    if (lbl_1_bss_3E80.camModel != -1) {
        Hu3DCameraMotionOff(lbl_1_bss_3E80.camModel);
    }
    lbl_1_bss_3E80.camModel = mdlId;
    Hu3DCameraMotionOn(lbl_1_bss_3E80.camModel, HU3D_CAM0);
    OSReport("start camera motion ... idx:%d time:%f\n", camNo, lbl_1_bss_3E80.camMaxTime);
}

BOOL fn_1_5F64(void)
{
    return Hu3DMotionEndCheck(lbl_1_bss_3E80.camModel);
}

void fn_1_5F90(s16 mdlNo)
{
    HU3D_MODELID mdlId = lbl_1_bss_3E80.mdlId[mdlNo];

    Hu3DModelAttrSet(mdlId, HU3D_ATTR_DISPOFF);
}

void fn_1_5FDC(s16 mdlNo, s16 motNo)
{
    HU3D_MODELID mdlId = lbl_1_bss_3E80.mdlId[mdlNo];

    Hu3DMotionSet(mdlId, lbl_1_bss_3E80.mtnId[motNo]);
    Hu3DModelAttrReset(mdlId, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(mdlId, HU3D_MOTATTR_LOOP);
}

void fn_1_605C(s16 mdlNo)
{
    HU3D_MODELID mdlId = lbl_1_bss_3E80.mdlId[mdlNo];

    Hu3DMotionTimeSet(mdlId, 0.0f);
    Hu3DModelAttrReset(mdlId, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(mdlId, HU3D_MOTATTR_LOOP);
}

HU3D_PARMAN_PARAM lbl_1_data_320 = {
    1800, 0, 0.5f, 1000.0f, 180.0f, { 0.0f, -0.3f, 0.0f }, 1.0f, 1.0f, 100.0f, 1.0f, 2,
    { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
};
HU3D_PARMAN_PARAM lbl_1_data_370 = {
    1800, 0, 1.0f, 1000.0f, 180.0f, { 0.0f, -0.3f, 0.0f }, 1.0f, 1.0f, 100.0f, 1.0f, 2,
    { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
};
HU3D_PARMAN_PARAM lbl_1_data_3C0 = {
    600, 0, 1.0f, 1000.0f, 180.0f, { 0.0f, -0.03f, 0.0f }, 1.0f, 1.0f, 30.0f, 1.0f, 2,
    { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
};

void fn_1_60CC(void)
{
    lbl_1_data_320.scaleRange = 5000.0f * frandf();
    lbl_1_data_370.scaleRange = 5000.0f * frandf();
    lbl_1_data_3C0.scaleRange = 2000.0f * frandf();
}

void fn_1_6148(void)
{
}

void fn_1_614C(void)
{
}

typedef struct M608ScoreInfo_s {
    s16 x; /* 0x00 */
    s16 y; /* 0x02 */
    s16 sizeX; /* 0x04 */
    s16 sizeY; /* 0x06 */
    GXColor boxColor; /* 0x08 */
    float unkC; /* 0x0C */
    GXColor scoreColor; /* 0x10 */
} M608_SCOREINFO; /* size 0x14 */

unsigned int lbl_1_data_410[15] = {
    DATANUM(DATA_mgconst, 0x00), DATANUM(DATA_mgconst, 0x01), DATANUM(DATA_mgconst, 0x02),
    DATANUM(DATA_mgconst, 0x03), DATANUM(DATA_mgconst, 0x04), DATANUM(DATA_mgconst, 0x05),
    DATANUM(DATA_mgconst, 0x06), DATANUM(DATA_mgconst, 0x07), DATANUM(DATA_mgconst, 0x08),
    DATANUM(DATA_mgconst, 0x09), DATANUM(DATA_mgconst, 0x0A), DATANUM(DATA_mgconst, 0x0B),
    DATANUM(DATA_mgconst, 0x0C), DATANUM(DATA_mgconst, 0x0D), DATANUM(DATA_mgconst, 0x45),
};
M608_SCOREINFO lbl_1_data_44C[5] = {
    { 90, 420, 128, 40, { 64, 64, 64, 0 }, 0.7f, { 255, 255, 255, 0 } },
    { 220, 420, 128, 40, { 64, 64, 64, 0 }, 0.7f, { 255, 255, 255, 0 } },
    { 350, 420, 128, 40, { 64, 64, 64, 0 }, 0.7f, { 255, 255, 255, 0 } },
    { 480, 420, 128, 40, { 64, 64, 64, 0 }, 0.7f, { 255, 255, 255, 0 } },
    { 280, 58, 128, 40, { 64, 64, 64, 0 }, 0.7f, { 66, 255, 122, 0 } },
};

void fn_1_6150(void)
{
    s32 i;
    MGSCORE *score;
    HUSPR_GROUPID boxId;
    HUSPR_GROUPID grpId;
    s32 animNo;

    for (i = 0; i < 5; i++) {
        if (i < 4) {
            boxId = MgScoreBoxCreateChar(lbl_1_data_44C[i].sizeX, lbl_1_data_44C[i].sizeY, lbl_1_bss_3E80.charNo[i]);
            lbl_1_bss_3DF0[i].unk0 = boxId;
        } else {
            boxId = MgScoreBoxCreate(lbl_1_data_44C[i].sizeX, lbl_1_data_44C[i].sizeY);
            lbl_1_bss_3DF0[i].unk0 = boxId;
        }
        MgScoreBoxPosSet(boxId, lbl_1_data_44C[i].x, lbl_1_data_44C[i].y);
        MgScoreBoxColorSet(boxId, lbl_1_data_44C[i].boxColor.r, lbl_1_data_44C[i].boxColor.g, lbl_1_data_44C[i].boxColor.b);
        score = MgScoreCreate(DATANUM(DATA_mgconst, 0x30), DATANUM(DATA_mgconst, 0x30), FALSE);
        lbl_1_bss_3DF0[i].unk4 = score;
        MgScoreUnitBankSet(score, 17);
        MgScoreMaxDigitSet(score, 4);
        MgScorePosSet(score, lbl_1_data_44C[i].x - 12, lbl_1_data_44C[i].y);
        MgScoreColorSet(score, lbl_1_data_44C[i].scoreColor.r, lbl_1_data_44C[i].scoreColor.g, lbl_1_data_44C[i].scoreColor.b);
        if (i >= 4) {
            animNo = 14;
            lbl_1_bss_3DF0[i].unk8 = HuSprAnimDataRead(lbl_1_data_410[animNo]);
            grpId = HuSprGrpCreate(2);
            lbl_1_bss_3DF0[i].unkC = grpId;
            lbl_1_bss_3DF0[i].unkE = HuSprCreate(lbl_1_bss_3DF0[i].unk8, 10, 0);
            HuSprGrpMemberSet(grpId, 0, lbl_1_bss_3DF0[i].unkE);
            HuSprGrpPosSet(grpId, lbl_1_data_44C[i].x - 40, lbl_1_data_44C[i].y);
        }
    }
    fn_1_6604();
}

void fn_1_6534(s16 scoreNo, s16 value)
{
    MgScoreValueSet(lbl_1_bss_3DF0[scoreNo].unk4, value);
}

void fn_1_6578(void)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        MgScoreBoxDispSet(lbl_1_bss_3DF0[i].unk0, TRUE);
        MgScoreDispOn(lbl_1_bss_3DF0[i].unk4);
        HuSprAttrReset(lbl_1_bss_3DF0[i].unkC, 0, HUSPR_ATTR_DISPOFF);
    }
}

void fn_1_6604(void)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        MgScoreBoxDispSet(lbl_1_bss_3DF0[i].unk0, FALSE);
        MgScoreDispOff(lbl_1_bss_3DF0[i].unk4);
    }
    if (_CheckFlag(FLAG_INST_DECA)) {
        MgScoreBoxDispSet(lbl_1_bss_3DF0[4].unk0, FALSE);
        MgScoreDispOff(lbl_1_bss_3DF0[4].unk4);
        HuSprAttrSet(lbl_1_bss_3DF0[4].unkC, 0, HUSPR_ATTR_DISPOFF);
    }
}

void fn_1_66C0(OMOBJ *obj)
{
    float scale;
    s32 time;
    s16 value;

    scale = *(float *)&obj->work[0];
    time = obj->work[2];
    if (time >= 150) {
        if (obj->work[1] != 0) {
            scale += 0.02f;
            if (scale >= 1.2f) {
                scale = 1.2f;
                obj->work[1] = 0;
            }
        } else {
            scale -= 0.04f;
            if (scale <= 1.0f) {
                scale = 1.0f;
                obj->work[1] = 1;
            }
        }
        MgScoreDigitScaleSet(lbl_1_bss_3DF0[4].unk4, scale, scale);
        HuSprScaleSet(lbl_1_bss_3DF0[4].unkC, 0, scale, scale);
        if (time == 150) {
            GWRecordSet(GW_RECORD_M608, lbl_1_bss_3E80.record * 90);
            value = lbl_1_bss_3E80.record * 90;
            MgScoreValueSet(lbl_1_bss_3DF0[4].unk4, value);
        }
    }
    *(float *)&obj->work[0] = scale;
    obj->work[2] = time + 1;
}

unsigned int lbl_1_data_4B0[6] = {
    DATANUM(DATA_m608, 0x65), DATANUM(DATA_m608, 0x61), DATANUM(DATA_m608, 0x62),
    DATANUM(DATA_m608, 0x64), DATANUM(DATA_m608, 0x63), 0,
};

void fn_1_6828(void)
{
    s32 i;
    void *data;

    lbl_1_bss_3E68.unk0 = -1;
    lbl_1_bss_3E68.unk2 = HuSprGrpCreate(5);
    for (i = 0; i < 5; i++) {
        data = HuDataSelHeapReadNum(lbl_1_data_4B0[i], HU_MEMNUM_OVL, HEAP_MODEL);
        if (data == NULL) {
            i++;
            i--;
        }
        lbl_1_bss_3E68.unk4[i] = HuSprAnimRead(data);
        HuSprGrpMemberSet(lbl_1_bss_3E68.unk2, i, HuSprCreate(lbl_1_bss_3E68.unk4[i], 10, 0));
        HuSprAttrSet(lbl_1_bss_3E68.unk2, i, HUSPR_ATTR_DISPOFF);
    }
    HuSprGrpPosSet(lbl_1_bss_3E68.unk2, 480.0f, 304.0f);
}

void fn_1_6958(s32 no)
{
    if (lbl_1_bss_3E68.unk0 >= 0) {
        HuSprAttrSet(lbl_1_bss_3E68.unk2, lbl_1_bss_3E68.unk0, HUSPR_ATTR_DISPOFF);
    }
    lbl_1_bss_3E68.unk0 = no;
    if (lbl_1_bss_3E68.unk0 >= 0) {
        HuSprAttrReset(lbl_1_bss_3E68.unk2, lbl_1_bss_3E68.unk0, HUSPR_ATTR_DISPOFF);
    }
}

void fn_1_69F4(void)
{
    s32 i;
    u32 rand;
    u32 interval;

    for (i = 0; i < 4; i++) {
        if (GwPlayerConf[i].type == 1) {
            lbl_1_bss_3E80.unk408[i] = GwPlayerConf[i].comDif;
            interval = lbl_1_data_56C[lbl_1_bss_3E80.unk408[i] * 2 + 1];
            rand = frand() % interval;
            lbl_1_bss_3E80.unk410[i] = lbl_1_data_56C[lbl_1_bss_3E80.unk408[i] * 2];
            lbl_1_bss_3E80.unk418[i] = 300 / (lbl_1_bss_3E80.unk410[i] + rand);
            lbl_1_bss_3E80.unk420[i] = 0;
            OSReport("com(%d) ... level : %d, interval : %d\n", i, lbl_1_bss_3E80.unk408[i], lbl_1_bss_3E80.unk418[i]);
        }
    }
}

s32 fn_1_6B90(s32 playerNo)
{
    s16 interval;
    s16 cnt;
    s32 btn;
    s32 pressF;

    interval = lbl_1_bss_3E80.unk418[playerNo];
    cnt = lbl_1_bss_3E80.unk420[playerNo];
    btn = 0;
    pressF = FALSE;
    cnt++;
    if (interval <= cnt) {
        pressF = TRUE;
        cnt = 0;
    } else if (frandmod(1000) < 10) {
        pressF = TRUE;
        cnt = 0;
    }
    if (pressF) {
        switch (lbl_1_bss_3E80.score[playerNo] % 4) {
            case 0:
                btn |= PAD_BUTTON_A;
                break;
            case 1:
                btn |= PAD_BUTTON_B;
                break;
            case 2:
                btn |= PAD_BUTTON_Y;
                break;
            case 3:
                btn |= PAD_BUTTON_X;
                break;
        }
    }
    lbl_1_bss_3E80.unk420[playerNo] = cnt;
    return btn;
}

unsigned int lbl_1_data_4F0[10] = {
    DATANUM(DATA_m608, 0x07), DATANUM(DATA_m608, 0x08), DATANUM(DATA_m608, 0x09), DATANUM(DATA_m608, 0x0A),
    DATANUM(DATA_m608, 0x0B), DATANUM(DATA_m608, 0x0C), DATANUM(DATA_m608, 0x0D), DATANUM(DATA_m608, 0x0E),
    DATANUM(DATA_m608, 0x0F), DATANUM(DATA_m608, 0x10),
};
unsigned int lbl_1_data_518[4] = {
    DATANUM(DATA_m608, 0x2E), DATANUM(DATA_m608, 0x2F), DATANUM(DATA_m608, 0x30), DATANUM(DATA_m608, 0x31),
};
unsigned int lbl_1_data_528[4] = {
    DATANUM(DATA_m608, 0x32), DATANUM(DATA_m608, 0x33), DATANUM(DATA_m608, 0x34), DATANUM(DATA_m608, 0x35),
};
unsigned int lbl_1_data_538[4] = {
    DATANUM(DATA_m608, 0x36), DATANUM(DATA_m608, 0x37), DATANUM(DATA_m608, 0x38), DATANUM(DATA_m608, 0x39),
};
unsigned int lbl_1_data_548[9] = {
    DATANUM(DATA_mario, 0x89), DATANUM(DATA_mario, 0x8A), DATANUM(DATA_mario, 0x8B), DATANUM(DATA_mario, 0x8C),
    DATANUM(DATA_mario, 0x8D), DATANUM(DATA_mariomot, 0x00), DATANUM(DATA_mariomot, 0x06), DATANUM(DATA_mariomot, 0x28), 0,
};
s32 lbl_1_data_56C[8] = { 4, 8, 16, 6, 22, 8, 34, 6 };
char *lbl_1_data_5D0[4] = { "608kurukuru-p1st", "608kurukuru-p2st", "608kurukuru-p3st", "608kurukuru-p4st" };
char *lbl_1_data_624[4] = { "608kurukuru-p1ls", "608kurukuru-p2ls", "608kurukuru-p3ls", "608kurukuru-p4ls" };
char *lbl_1_data_64C[4] = { "p1lsB", "p2lsB", "p3lsB", "p4lsB" };
char *lbl_1_data_684[4] = { "p1lsboard", "p2lsboard", "p3lsboard", "p4lsboard" };
unsigned int lbl_1_data_694[27] = {
    DATANUM(DATA_m608, 0x11), DATANUM(DATA_m608, 0x12), DATANUM(DATA_m608, 0x13), DATANUM(DATA_m608, 0x14),
    DATANUM(DATA_m608, 0x15), DATANUM(DATA_m608, 0x16), DATANUM(DATA_m608, 0x17), DATANUM(DATA_m608, 0x18),
    DATANUM(DATA_m608, 0x19), DATANUM(DATA_m608, 0x1A), DATANUM(DATA_m608, 0x1B), DATANUM(DATA_m608, 0x1C),
    DATANUM(DATA_m608, 0x1D), DATANUM(DATA_m608, 0x1E), DATANUM(DATA_m608, 0x1F), DATANUM(DATA_m608, 0x20),
    DATANUM(DATA_m608, 0x21), DATANUM(DATA_m608, 0x22), DATANUM(DATA_m608, 0x23), DATANUM(DATA_m608, 0x24),
    DATANUM(DATA_m608, 0x25), DATANUM(DATA_m608, 0x26), DATANUM(DATA_m608, 0x27), DATANUM(DATA_m608, 0x28),
    DATANUM(DATA_m608, 0x29), DATANUM(DATA_m608, 0x2A), DATANUM(DATA_m608, 0x2B),
};
