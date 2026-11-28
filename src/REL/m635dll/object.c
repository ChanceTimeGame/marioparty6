/* Retail's second m635dll file does not carry math.h's sqrtf statics in .rodata:
 * keep include/math.h out (this file uses none of it). */
#define _MATH_H
#define M_PI 3.141592653589793
#include "REL/m635dll/m635.h"

/* .bss is laid out in reverse definition order */
M635_PLAYER lbl_1_bss_BC[4];
s16 lbl_1_bss_B4[2][2];
M635_BTN lbl_1_bss_74[4];
M635_CAR lbl_1_bss_68;

typedef struct M635Step_s {
    float posZ[2];
    s16 time;
} M635_STEP;

typedef struct M635Pos2D_s {
    float x;
    float y;
} M635_POS2D;

typedef struct M635MotData_s {
    u32 dataNum;
    u32 attr;
} M635_MOTDATA;

float lbl_1_data_A0[2] = { 1.1666666f, 1.6666666f };
HuVecF lbl_1_data_A8[2] = { { -200.0f, 0.0f, 0.0f }, { 200.0f, 0.0f, 0.0f } };
HuVecF lbl_1_data_C0[2] = { { -200.0f, 0.0f, 0.0f }, { 200.0f, 0.0f, 0.0f } };
HuVecF lbl_1_data_D8 = { 500.0f, 2000.0f, 400.0f };
HuVecF lbl_1_data_E4 = { -5.0f, -20.0f, -4.0f };
GXColor lbl_1_data_F0[2] = { { 192, 192, 192, 192 }, { 192, 192, 192, 192 } };
HuVecF lbl_1_data_F8[4] = {
    { -280.0f, 0.0f, -150.0f },
    { -120.0f, 0.0f, -150.0f },
    { 120.0f, 0.0f, -150.0f },
    { 280.0f, 0.0f, -150.0f },
};
HuVecF lbl_1_data_128[4] = {
    { -210.0f, 0.0f, -450.0f },
    { -70.0f, 0.0f, -450.0f },
    { 70.0f, 0.0f, -450.0f },
    { 210.0f, 0.0f, -450.0f },
};
M635_MOTDATA lbl_1_data_158[6] = {
    { 0x8E0054, 0 },
    { 0x930000, 1 },
    { 0x930006, 0 },
    { 0x930007, 0 },
    { 0x930010, 0 },
    { 0x8E0015, 0 },
};
M635_POS2D lbl_1_data_188[4] = {
    { 120.0f, 160.0f },
    { 220.0f, 160.0f },
    { 358.0f, 160.0f },
    { 458.0f, 160.0f },
};
s16 lbl_1_data_1A8[20][8] = {
    { 1, 20, 1, 30, 1, 30, 1, 20 },
    { 71, 90, 71, 100, 71, 100, 71, 90 },
    { 111, 130, 111, 140, 111, 140, 111, 130 },
    { 151, 170, 151, 180, 151, 180, 151, 170 },
    { 191, 210, 191, 220, 191, 220, 191, 210 },
    { 231, 250, 231, 260, 231, 260, 231, 250 },
    { 271, 290, 271, 300, 271, 300, 271, 290 },
    { 311, 330, 311, 340, 311, 340, 311, 330 },
    { 351, 370, 351, 380, 351, 380, 351, 370 },
    { 401, 430, 401, 430, 401, 430, 370, 370 },
    { 431, 460, 431, 460, 431, 460, 370, 370 },
    { 461, 490, 461, 490, 461, 490, 370, 370 },
    { 491, 520, 491, 520, 491, 520, 370, 370 },
    { 521, 550, 521, 550, 521, 550, 370, 370 },
    { 551, 580, 551, 580, 551, 580, 370, 370 },
    { 581, 610, 581, 610, 581, 610, 370, 370 },
    { 611, 640, 611, 640, 611, 640, 370, 370 },
    { 641, 670, 641, 670, 641, 670, 370, 370 },
    { 671, 700, 671, 700, 671, 671, 370, 370 },
    { 710, 830, 710, 830, 710, 830, 710, 830 },
};
M635_STEP lbl_1_data_2E8[21] = {
    { { -150.0f, -150.0f }, 20 },
    { { -180.0f, -180.0f }, 20 },
    { { -180.0f, -188.0f }, 10 },
    { { -196.0f, -188.0f }, 10 },
    { { -196.0f, -204.0f }, 10 },
    { { -212.0f, -204.0f }, 10 },
    { { -212.0f, -220.0f }, 10 },
    { { -228.0f, -220.0f }, 10 },
    { { -228.0f, -236.0f }, 10 },
    { { -236.0f, -236.0f }, 10 },
    { { -238.0f, -238.0f }, 15 },
    { { -240.0f, -240.0f }, 15 },
    { { -242.0f, -242.0f }, 15 },
    { { -244.0f, -244.0f }, 15 },
    { { -246.0f, -246.0f }, 15 },
    { { -248.0f, -248.0f }, 15 },
    { { -250.0f, -250.0f }, 15 },
    { { -252.0f, -252.0f }, 15 },
    { { -254.0f, -254.0f }, 15 },
    { { -256.0f, -256.0f }, 15 },
    { { -356.0f, -356.0f }, 15 },
};
s32 lbl_1_data_3E4[2] = { DATANUM(DATA_m635, 0x00), DATANUM(DATA_m635, 0x01) };
s32 lbl_1_data_3EC[2] = { DATANUM(DATA_m635, 0x02), DATANUM(DATA_m635, 0x03) };
s32 lbl_1_data_3F4[2][2] = {
    { DATANUM(DATA_m635, 0x05), DATANUM(DATA_m635, 0x06) },
    { DATANUM(DATA_m635, 0x07), DATANUM(DATA_m635, 0x08) },
};
s32 lbl_1_data_404[2][2] = {
    { DATANUM(DATA_m635, 0x0B), DATANUM(DATA_m635, 0x0C) },
    { DATANUM(DATA_m635, 0x0D), DATANUM(DATA_m635, 0x0E) },
};
s32 lbl_1_data_414[2] = { DATANUM(DATA_m635, 0x0F), DATANUM(DATA_m635, 0x10) };
s32 lbl_1_data_41C[12] = {
    DATANUM(DATA_m635, 0x16),
    DATANUM(DATA_m635, 0x17),
    DATANUM(DATA_m635, 0x18),
    DATANUM(DATA_m635, 0x19),
    DATANUM(DATA_m635, 0x1A),
    DATANUM(DATA_m635, 0x1B),
    DATANUM(DATA_m635, 0x1C),
    DATANUM(DATA_m635, 0x1D),
    DATANUM(DATA_m635, 0x1E),
    DATANUM(DATA_m635, 0x1F),
    DATANUM(DATA_m635, 0x20),
    DATANUM(DATA_m635, 0x21),
};
s32 lbl_1_data_44C[2] = { DATANUM(DATA_m635, 0x14), DATANUM(DATA_m635, 0x15) };

void fn_1_1774(OMOBJMAN *objman)
{
    OMOBJ *obj;

    Hu3DCameraCreate(HU3D_CAM0);
    Hu3DCameraViewportSet(HU3D_CAM0, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(HU3D_CAM0, 20.0f, 60.0f, 25000.0f, 1.2f);
    Hu3DCameraPosSet(HU3D_CAM0, 0.0f, 800.0f, 2000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 100.0f, -500.0f);
    obj = omAddObjEx(objman, 32730, 0, 0, -1, omOutView);
    Center.x = Center.y = 0.0f;
    Center.z = -400.0f;
    CRot.x = -30.0f;
    CRot.y = 0.0f;
    CRot.z = 0.0f;
    CZoom = 3000.0f;
}

void fn_1_195C(void)
{
    s32 i;
    HU3D_MODELID modelId;
    HU3D_LIGHTID lightId;
    HuVecF pos;
    HuVecF target;
    HuVecF up;

    lbl_1_bss_4.stage = GWMgTimeGet();
    lightId = Hu3DGLightCreateV(&lbl_1_data_D8, &lbl_1_data_E4, &lbl_1_data_F0[lbl_1_bss_4.stage]);
    Hu3DGLightStaticSet(lightId, TRUE);
    Hu3DGLightInfinitytSet(lightId);
    lbl_1_bss_4.lightId = lightId;
    Hu3DShadowCreate(30.0f, 20.0f, 10000.0f);
    pos.x = 500.0f;
    pos.y = 2000.0f;
    pos.z = 400.0f;
    up.y = 1.0f;
    up.x = up.z = 0.0f;
    target.x = target.y = target.z = 0.0f;
    Hu3DShadowPosSet(&pos, &up, &target);
    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_3E4[lbl_1_bss_4.stage], HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelCameraSet(modelId, HU3D_CAM0);
    Hu3DModelPosSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(modelId, 1.0f, 1.0f, 1.0f);
    Hu3DModelShadowMapSet(modelId);
    Hu3DModelAttrSet(modelId, HU3D_MOTATTR_LOOP);
    if (lbl_1_bss_4.stage == 0) {
        Hu3DModelShadowMapTPLvlSet(modelId, 0.8f);
    }
    else {
        Hu3DModelShadowMapTPLvlSet(modelId, 0.3f);
    }
    lbl_1_bss_4.stageModel = modelId;
    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_3EC[lbl_1_bss_4.stage], HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelCameraSet(modelId, HU3D_CAM0);
    Hu3DModelPosSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(modelId, 1.0f, 1.0f, 1.0f);
    lbl_1_bss_4.stageModel2 = modelId;
    for (i = 0; i < 2; i++) {
        modelId = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_3F4[lbl_1_bss_4.stage][i], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelCameraSet(modelId, HU3D_CAM0);
        Hu3DModelPosSetV(modelId, &lbl_1_data_C0[i]);
        Hu3DMotionSpeedSet(modelId, 0.0f);
        Hu3DModelShadowMapSet(modelId);
        if (lbl_1_bss_4.stage == 0) {
            Hu3DModelShadowMapTPLvlSet(modelId, 0.8f);
        }
        else {
            Hu3DModelShadowMapTPLvlSet(modelId, 0.3f);
        }
        lbl_1_bss_4.team[i].anim[0].model = modelId;
        modelId = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_404[lbl_1_bss_4.stage][i], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelCameraSet(modelId, HU3D_CAM0);
        Hu3DMotionSpeedSet(modelId, 0.0f);
        lbl_1_bss_4.team[i].model20 = modelId;
        modelId = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m635, 0x04), HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelCameraSet(modelId, HU3D_CAM0);
        Hu3DModelPosSetV(modelId, &lbl_1_data_A8[i]);
        Hu3DMotionSpeedSet(modelId, 0.0f);
        lbl_1_bss_4.team[i].anim[1].model = modelId;
        modelId = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m635, 0x09), HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelCameraSet(modelId, HU3D_CAM0);
        Hu3DModelPosSetV(modelId, &lbl_1_data_A8[i]);
        Hu3DMotionSpeedSet(modelId, 0.0f);
        lbl_1_bss_4.team[i].anim[2].model = modelId;
        modelId = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m635, 0x0A), HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelCameraSet(modelId, HU3D_CAM0);
        Hu3DModelPosSetV(modelId, &lbl_1_data_A8[i]);
        Hu3DMotionSpeedSet(modelId, 0.0f);
        lbl_1_bss_4.team[i].anim[3].model = modelId;
        modelId = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_414[lbl_1_bss_4.stage], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelCameraSet(modelId, HU3D_CAM0);
        Hu3DModelPosSetV(modelId, &lbl_1_data_A8[i]);
        Hu3DMotionSpeedSet(modelId, 0.0f);
        lbl_1_bss_4.team[i].model22 = modelId;
        modelId = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m635, 0x11), HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelCameraSet(modelId, HU3D_CAM0);
        Hu3DModelPosSetV(modelId, &lbl_1_data_A8[i]);
        Hu3DMotionSpeedSet(modelId, 0.0f);
        lbl_1_bss_4.team[i].model24 = modelId;
    }
}

void fn_1_2014(void) { }

void fn_1_2018(void)
{
    s32 i;
    s32 j;
    HUSPR_GROUPID grpId;
    ANIMDATA *btnAnim[12];
    ANIMDATA *effAnim[2];

    for (i = 0; i < 12; i++) {
        btnAnim[i] = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_data_41C[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    for (i = 0; i < 2; i++) {
        effAnim[i] = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_data_44C[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    for (i = 0; i < 4; i++) {
        grpId = lbl_1_bss_74[i].grpId = HuSprGrpCreate(14);
        HuSprGrpPosSet(grpId, lbl_1_data_188[i].x, lbl_1_data_188[i].y);
        for (j = 0; j < 12; j++) {
            HuSprGrpMemberSet(grpId, j, HuSprCreate(btnAnim[j], 10, 0));
            HuSprPosSet(grpId, j, 0.0f, -5.0f);
            HuSprAttrSet(grpId, j, HUSPR_ATTR_DISPOFF);
        }
        for (j = 0; j < 2; j++) {
            HuSprGrpMemberSet(grpId, j + 12, HuSprCreate(effAnim[j], 15, 0));
            HuSprAttrSet(grpId, j + 12, HUSPR_ATTR_DISPOFF);
            HuSprTPLvlSet(grpId, j + 12, 0.75f);
        }
        lbl_1_bss_74[i].no = -1;
        lbl_1_bss_74[i].mode = 5;
        lbl_1_bss_74[i].scale = 0.0f;
    }
}

void fn_1_2268(s16 team, s16 member)
{
    M635_BTN *btn;
    s16 no;

    btn = &lbl_1_bss_74[team * 2 + member];
    if (btn->mode == 4) {
        no = lbl_1_bss_4.team[team].btnNo[member] * 2;
        HuSprAttrSet(btn->grpId, no, HUSPR_ATTR_DISPOFF);
        HuSprAttrReset(btn->grpId, no + 1, HUSPR_ATTR_DISPOFF);
        btn->time = 0;
    }
}

void fn_1_2330(void)
{
    M635_BTN *btn;
    s32 i;
    s16 no;

    for (i = 0; i < 4; i++) {
        btn = &lbl_1_bss_74[i];
        no = lbl_1_bss_4.team[i / 2].btnNo[i % 2] * 2;
        switch (btn->mode) {
            case 0:
                break;
            case 1:
                btn->scale += 0.2;
                if (btn->scale >= 1.0f) {
                    btn->scale = 1.0f;
                    btn->mode = 0;
                }
                HuSprScaleSet(btn->grpId, no, btn->scale, btn->scale);
                HuSprScaleSet(btn->grpId, 12, btn->scale, btn->scale);
                break;
            case 3:
                btn->time--;
                if (btn->time <= 0) {
                    HuSprAttrSet(btn->grpId, btn->no + 1, HUSPR_ATTR_DISPOFF);
                    HuSprAttrSet(btn->grpId, 13, HUSPR_ATTR_DISPOFF);
                    btn->mode = 5;
                    btn->scale = 0.0f;
                }
                break;
            case 4:
                if (btn->scale < 1.0f) {
                    btn->scale += 0.2;
                }
                if (btn->time % 8 == 0) {
                    HuSprAttrSet(btn->grpId, btn->no, HUSPR_ATTR_DISPOFF);
                    HuSprAttrReset(btn->grpId, btn->no + 1, HUSPR_ATTR_DISPOFF);
                }
                else if (btn->time % 8 == 4) {
                    HuSprAttrSet(btn->grpId, btn->no + 1, HUSPR_ATTR_DISPOFF);
                    HuSprAttrReset(btn->grpId, btn->no, HUSPR_ATTR_DISPOFF);
                }
                HuSprScaleSet(btn->grpId, btn->no, btn->scale, btn->scale);
                HuSprScaleSet(btn->grpId, btn->no + 1, btn->scale, btn->scale);
                HuSprScaleSet(btn->grpId, 13, btn->scale, btn->scale);
                btn->time++;
                break;
            case 5:
                break;
        }
    }
}

void fn_1_25EC(s16 team, s16 member, s16 mode)
{
    M635_BTN *btn;
    s16 no;
    s32 i;

    btn = &lbl_1_bss_74[team * 2 + member];
    btn->mode = mode;
    no = lbl_1_bss_4.team[team].btnNo[member] * 2;
    for (i = 0; i < 12; i++) {
        HuSprAttrSet(btn->grpId, i, HUSPR_ATTR_DISPOFF);
    }
    for (i = 0; i < 2; i++) {
        HuSprAttrSet(btn->grpId, i + 12, HUSPR_ATTR_DISPOFF);
    }
    switch (mode) {
        case 1:
            btn->scale = 0.0f;
            HuSprAttrReset(btn->grpId, no, HUSPR_ATTR_DISPOFF);
            HuSprAttrReset(btn->grpId, 12, HUSPR_ATTR_DISPOFF);
            HuSprScaleSet(btn->grpId, no, 0.0f, 0.0f);
            HuSprScaleSet(btn->grpId, 12, 0.0f, 0.0f);
            btn->no = no;
            break;
        case 3:
            btn->time = 8;
            btn->scale = 1.0f;
            HuSprAttrReset(btn->grpId, no + 1, HUSPR_ATTR_DISPOFF);
            HuSprAttrReset(btn->grpId, 13, HUSPR_ATTR_DISPOFF);
            break;
        case 4:
            HuSprAttrReset(btn->grpId, 13, HUSPR_ATTR_DISPOFF);
            HuSprAttrReset(btn->grpId, no, HUSPR_ATTR_DISPOFF);
            btn->no = no;
            btn->time = 0;
            btn->scale = 0.0f;
            break;
    }
}

s16 fn_1_27E4(s16 team, s16 member)
{
    return lbl_1_bss_74[team * 2 + member].mode;
}

void fn_1_280C(s16 team, s16 member, s16 no)
{
    M635_BTN *btn;
    s32 i;

    btn = &lbl_1_bss_74[team * 2 + member];
    for (i = 0; i < 12; i++) {
        HuSprAttrSet(btn->grpId, i, HUSPR_ATTR_DISPOFF);
    }
    HuSprAttrReset(btn->grpId, no, HUSPR_ATTR_DISPOFF);
    HuSprAttrReset(btn->grpId, 13, HUSPR_ATTR_DISPOFF);
}

void fn_1_28A8(s16 team, s16 member)
{
    M635_BTN *btn;

    btn = &lbl_1_bss_74[team * 2 + member];
    HuSprAttrSet(btn->grpId, 13, HUSPR_ATTR_DISPOFF);
}

void fn_1_2904(void)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        HuSprGrpKill(lbl_1_bss_74[i].grpId);
    }
}

void fn_1_2954(void)
{
    s32 i;
    s16 charNo;
    s32 j;
    s32 k;
    s16 player;
    s16 member0;
    s16 member1;
    HU3D_MODELID modelId;
    s16 num0;
    s16 num1;
    s16 grpNo;
    HU3D_MOTIONID motId;
    s16 maxTime;

    num0 = 0;
    num1 = 0;
    member0 = 0;
    member1 = 0;
    for (i = 0; i < 4; i++) {
        lbl_1_bss_BC[i].playerNo = i;
        charNo = GwPlayerConf[i].charNo;
        lbl_1_bss_BC[i].charNo = charNo;
        lbl_1_bss_BC[i].padNo = GwPlayerConf[i].padNo;
        lbl_1_bss_BC[i].grpNo = grpNo = GwPlayerConf[i].grpNo;
        if (grpNo == 0) {
            lbl_1_bss_BC[i].memberNo = member0;
            member0++;
        }
        else {
            lbl_1_bss_BC[i].memberNo = member1;
            member1++;
        }
        lbl_1_bss_BC[i].comDif = GwPlayerConf[i].comDif;
        lbl_1_bss_BC[i].type = GwPlayerConf[i].type;
        lbl_1_bss_BC[i].comTimer = fn_1_1308(GwPlayerConf[i].comDif);
        if (grpNo == 0) {
            lbl_1_bss_B4[0][num0++] = i;
        }
        else {
            lbl_1_bss_B4[1][num1++] = i;
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            player = lbl_1_bss_B4[i][j];
            charNo = lbl_1_bss_BC[player].charNo;
            modelId = CharModelCreate(charNo, 2);
            Hu3DModelPosSetV(modelId, &lbl_1_data_F8[i * 2 + j]);
            lbl_1_bss_BC[player].posZ = lbl_1_data_F8[i * 2 + j].z;
            lbl_1_bss_BC[player].model = modelId;
            for (k = 0; k < 6; k++) {
                motId = CharMotionCreate(charNo, lbl_1_data_158[k].dataNum);
                lbl_1_bss_BC[player].mot[k] = motId;
            }
            Hu3DModelShadowSet(modelId);
            CharMotionDataClose(charNo);
            CharMotionSet(charNo, lbl_1_bss_BC[player].mot[0]);
            maxTime = CharMotionMaxTimeGet(charNo);
            CharMotionTimeSet(charNo, maxTime);
        }
    }
}

void fn_1_2CE8(void)
{
    s32 i;
    s32 j;
    s16 player;
    s16 step;
    HU3D_MODELID modelId;
    float posZ;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            player = lbl_1_bss_B4[i][j];
            step = lbl_1_bss_BC[player].step;
            if (step == 0) {
                continue;
            }
            modelId = lbl_1_bss_BC[player].model;
            posZ = lbl_1_bss_BC[player].posZ;
            posZ += (lbl_1_data_2E8[step].posZ[j] - lbl_1_data_2E8[step - 1].posZ[j]) / lbl_1_data_2E8[step].time;
            if (posZ <= lbl_1_data_2E8[step].posZ[j]) {
                posZ = lbl_1_data_2E8[step].posZ[j];
            }
            else {
                lbl_1_bss_BC[player].posZ = posZ;
                Hu3DModelPosSet(modelId, lbl_1_data_F8[i * 2 + j].x, lbl_1_data_F8[i * 2 + j].y, lbl_1_bss_BC[player].posZ);
            }
        }
    }
}

void fn_1_2F08(s16 player, s16 step)
{
    if (step < 0 || step > 11) {
        return;
    }
    lbl_1_bss_BC[player].step = step;
}

void fn_1_2F40(s16 player)
{
    if (lbl_1_bss_BC[player].step > 8) {
        lbl_1_bss_BC[player].step++;
    }
    else if (lbl_1_bss_BC[player].step == 8) {
        lbl_1_bss_BC[player].step = 10;
    }
    else if (lbl_1_bss_BC[player].step == 0) {
        lbl_1_bss_BC[player].step = 1;
    }
    else if (lbl_1_bss_BC[player].step == 1) {
        if (lbl_1_bss_BC[player].memberNo == 0) {
            lbl_1_bss_BC[player].step = 3;
        }
        else {
            lbl_1_bss_BC[player].step = 2;
        }
    }
    else if (lbl_1_bss_BC[player].step < 8) {
        lbl_1_bss_BC[player].step += 2;
    }
}

s32 fn_1_30C8(s16 player, s16 charNo)
{
    float maxTime;
    M635_PLAYER *work;

    work = &lbl_1_bss_BC[player];
    maxTime = CharMotionMaxTimeGet(charNo);
    fn_1_31BC(charNo);
    CharMotionShiftSet(charNo, lbl_1_bss_BC[player].mot[0], 0.0f, 8.0f, lbl_1_data_158[0].attr);
    omVibrate(player, 20, 4, 4);
    fn_1_3B7C(lbl_1_bss_BC[player].grpNo, lbl_1_bss_BC[player].memberNo, player);
    return 1;
}

void fn_1_31BC(s16 charNo)
{
    s32 i;

    for (i = 0; i < 2; i++) {
        CharModelCryCreate(charNo, 30.0f, 0.8f);
    }
}

void fn_1_3218(s16 player, s16 motNo, float shift)
{
    s16 charNo;

    if (motNo < 0 || motNo >= 6) {
        return;
    }
    charNo = lbl_1_bss_BC[player].charNo;
    CharMotionShiftSet(charNo, lbl_1_bss_BC[player].mot[motNo], 0.0f, shift, lbl_1_data_158[motNo].attr);
}

s16 fn_1_32E0(s16 team, s16 member)
{
    if (team < 0 || team >= 2 || member < 0 || member >= 2) {
        return 0;
    }
    return lbl_1_bss_B4[team][member];
}

void fn_1_3340(void)
{
    s32 i;
    s32 j;
    s16 player;
    HU3D_MODELID modelId;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            player = lbl_1_bss_B4[i][j];
            modelId = lbl_1_bss_BC[player].model;
            Hu3DModelPosSetV(modelId, &lbl_1_data_128[i * 2 + j]);
        }
    }
}

void fn_1_33F8(void) { }

void fn_1_33FC(s16 team)
{
    M635_TEAM *work;
    s32 i;
    s16 player;
    s16 charNo;

    work = &lbl_1_bss_4.team[team];
    work->level++;
    Hu3DMotionSpeedSet(lbl_1_bss_4.team[team].model22, 1.0f);
    if (work->level >= 20) {
        Hu3DMotionSpeedSet(lbl_1_bss_4.team[team].model24, 1.0f);
        for (i = 0; i < 2; i++) {
            player = fn_1_32E0(team, i);
            charNo = lbl_1_bss_BC[player].charNo;
            CharMotionShiftSet(charNo, lbl_1_bss_BC[player].mot[4], 0.0f, 8.0f, lbl_1_data_158[4].attr);
            fn_1_2F40(player);
            OSReport("%d\n", lbl_1_bss_BC[player].step);
            omVibrate(player, 30, 20, 0);
        }
    }
}

void fn_1_3738(s16 team, s16 level)
{
    M635_TEAM *work;
    s32 i;
    s16 player;
    s16 playerNo[2];
    s16 charNo[2];

    work = &lbl_1_bss_4.team[team];
    if (work->level != level) {
        for (i = 0; i < 2; i++) {
            player = lbl_1_bss_B4[team][i];
            playerNo[i] = lbl_1_bss_BC[player].playerNo;
            charNo[i] = lbl_1_bss_BC[player].charNo;
            fn_1_30C8(playerNo[i], charNo[i]);
            fn_1_2F40(player);
        }
        if (level > 9) {
            if (team == 0) {
                HuAudFXPlay(1863);
            }
            else {
                HuAudFXPlay(1864);
            }
        }
    }
    work->level = level;
}

void fn_1_3AC4(s16 team)
{
    M635_TEAM *work;
    M635_ANIM *anim;

    work = &lbl_1_bss_4.team[team];
    if (work->level < 10) {
        work->level++;
    }
    if (work->anim[1].time >= 430) {
        anim = &work->anim[1];
        anim->time = 401;
        Hu3DMotionTimeSet(anim->model, anim->time);
        Hu3DMotionSpeedSet(anim->model, 1.0f);
    }
}

void fn_1_3B7C(s16 team, s16 member, s16 player)
{
    M635_TEAM *work;
    M635_ANIM *anim;

    work = &lbl_1_bss_4.team[team];
    if (member != 0) {
        anim = &work->anim[2];
    }
    else {
        anim = &work->anim[3];
    }
    if (anim->time >= 430) {
        anim->time = 401;
        Hu3DMotionTimeSet(anim->model, anim->time);
        Hu3DMotionSpeedSet(anim->model, 1.0f);
    }
}

void fn_1_3C34(void)
{
    s32 i;
    M635_TEAM *work;
    s16 level;
    M635_ANIM *anim;

    for (i = 0; i < 2; i++) {
        work = &lbl_1_bss_4.team[i];
        level = work->level;
        if (level == 0) {
            continue;
        }
        anim = &work->anim[1];
        anim->time = Hu3DMotionTimeGet(anim->model);
        if (anim->time < lbl_1_data_1A8[level - 1][0]) {
            if (lbl_1_data_1A8[level - 1][0] < 400 || lbl_1_data_1A8[level - 1][0] > 700) {
                anim->time = lbl_1_data_1A8[level - 1][0];
                Hu3DMotionTimeSet(anim->model, anim->time);
                if (i == 0) {
                    HuAudFXPlay(1859);
                }
                else {
                    HuAudFXPlay(1860);
                }
            }
            Hu3DMotionSpeedSet(anim->model, 1.0f);
        }
        if (anim->time >= lbl_1_data_1A8[level - 1][1]) {
            Hu3DMotionSpeedSet(anim->model, 0.0f);
        }
        if (anim->time == 769) {
            Hu3DMotionSpeedSet(lbl_1_bss_4.team[i].model20, 1.0f);
        }
        else if (anim->time == 710) {
            if (i == 0) {
                HuAudFXPlay(1861);
            }
            else {
                HuAudFXPlay(1862);
            }
        }
        anim = &work->anim[2];
        anim->time = Hu3DMotionTimeGet(anim->model);
        if (anim->time < lbl_1_data_1A8[level - 1][2]) {
            anim->time = lbl_1_data_1A8[level - 1][2];
            Hu3DMotionTimeSet(anim->model, anim->time);
            Hu3DMotionSpeedSet(anim->model, 1.0f);
        }
        if (anim->time >= lbl_1_data_1A8[level - 1][3]) {
            Hu3DMotionSpeedSet(anim->model, 0.0f);
        }
        anim = &work->anim[3];
        anim->time = Hu3DMotionTimeGet(anim->model);
        if (anim->time < lbl_1_data_1A8[level - 1][4]) {
            anim->time = lbl_1_data_1A8[level - 1][4];
            Hu3DMotionTimeSet(anim->model, anim->time);
            Hu3DMotionSpeedSet(anim->model, 1.0f);
        }
        if (anim->time >= lbl_1_data_1A8[level - 1][5]) {
            Hu3DMotionSpeedSet(anim->model, 0.0f);
        }
        anim = &work->anim[0];
        anim->time = Hu3DMotionTimeGet(anim->model);
        if (anim->time < lbl_1_data_1A8[level - 1][6]) {
            anim->time = lbl_1_data_1A8[level - 1][6];
            Hu3DMotionTimeSet(anim->model, anim->time);
            Hu3DMotionSpeedSet(anim->model, 1.0f);
        }
        if (anim->time >= lbl_1_data_1A8[level - 1][7]) {
            Hu3DMotionSpeedSet(anim->model, 0.0f);
        }
    }
}

void fn_1_4114(void)
{
    HU3D_MODELID modelId;
    s16 dir;
    float posX;
    float rotY;

    memset(&lbl_1_bss_68, 0, sizeof(lbl_1_bss_68));
    dir = frandmod(2);
    lbl_1_bss_68.dir = dir;
    if (lbl_1_bss_4.stage == 0) {
        modelId = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m635, 0x12), HU_MEMNUM_OVL, HEAP_MODEL));
    }
    else {
        modelId = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m635, 0x13), HU_MEMNUM_OVL, HEAP_MODEL));
    }
    Hu3DModelAttrSet(modelId, HU3D_MOTATTR_LOOP);
    Hu3DModelCameraSet(modelId, HU3D_CAM0);
    Hu3DModelShadowSet(modelId);
    if (dir) {
        posX = -600.0f;
        rotY = 90.0f;
    }
    else {
        posX = 600.0f;
        rotY = -90.0f;
    }
    Hu3DModelPosSet(modelId, posX, 0.0f, -550.0f);
    Hu3DModelRotSet(modelId, 0.0f, rotY, 0.0f);
    lbl_1_bss_68.posX = posX;
    lbl_1_bss_68.model = modelId;
}

void fn_1_42A0(void)
{
    if (lbl_1_bss_68.wait > 0) {
        lbl_1_bss_68.wait--;
        return;
    }
    if (lbl_1_bss_68.dir != 0) {
        lbl_1_bss_68.posX += lbl_1_data_A0[lbl_1_bss_4.stage];
        if (lbl_1_bss_68.posX >= 600.0f) {
            lbl_1_bss_68.wait = 120;
            lbl_1_bss_68.dir = 0;
            Hu3DModelRotSet(lbl_1_bss_68.model, 0.0f, -90.0f, 0.0f);
        }
    }
    else {
        lbl_1_bss_68.posX -= lbl_1_data_A0[lbl_1_bss_4.stage];
        if (lbl_1_bss_68.posX <= -600.0f) {
            lbl_1_bss_68.wait = 120;
            lbl_1_bss_68.dir = 1;
            Hu3DModelRotSet(lbl_1_bss_68.model, 0.0f, 90.0f, 0.0f);
        }
    }
    Hu3DModelPosSet(lbl_1_bss_68.model, lbl_1_bss_68.posX, 0.0f, -550.0f);
}

void fn_1_448C(void)
{
    Hu3DModelAttrSet(lbl_1_bss_68.model, HU3D_ATTR_DISPOFF);
}
