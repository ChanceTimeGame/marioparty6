/* Retail's file carries no math.h sqrtf statics: its 0.5/3.0 doubles are pooled at their use */
#define _MATH_H
#define M_PI 3.141592653589793
double sin(double x);
double cos(double x);
double atan2(double y, double x);

static inline float sqrtf(float x)
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

#include "REL/m650dll/m650.h"

/* frandmod is called unprototyped: game/frand.h stays out of this unit */

M650_PLAYER lbl_1_bss_28[4];

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




void fn_1_3F28(s16 player)
{
    M650_PLAYER *work = &lbl_1_bss_28[player];
    HuVecF pos;
    HuVecF target;
    HuVecF rot;
    HuVecF delta;
    s32 seTbl[4] = { 0x7F8, 0x7F9, 0x7FA, 0x7FB };
    float dist;
    float angle;

    if (work->unk48 < 12.0f) {
        work->unk48 += 1.0f;
    }
    Hu3DModelPosGet(work->unk52, &target);
    Hu3DModelPosGet(work->mdlId, &pos);
    target.y = 0.0f;
    delta.x = target.x - pos.x;
    delta.z = target.z - pos.z;
    dist = HuMagXZVecF(&delta);
    if (dist > work->unk48) {
        work->unk2C.x = work->unk48 * (delta.x / dist);
        work->unk2C.z = work->unk48 * (delta.z / dist);
    } else {
        work->unk2C.x = delta.x;
        work->unk2C.z = delta.z;
    }
    Hu3DModelRotGet(work->mdlId, &rot);
    if (0.0f == work->unk2C.x) {
        angle = 0.0f;
    } else {
        angle = HuAtan(work->unk2C.x, work->unk2C.z);
    }
    if (rot.y < angle) {
        rot.y += 2.0f;
        if (rot.y > angle) {
            rot.y = angle;
        }
    } else if (rot.y > angle) {
        rot.y -= 2.0f;
        if (rot.y < angle) {
            rot.y = angle;
        }
    }
    Hu3DModelRotSetV(work->mdlId, &rot);
    if (work->unk74 && pos.z >= 5500.0f) {
        Hu3DModelAttrSet(work->unk52, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(work->unk54, HU3D_ATTR_DISPOFF);
        work->mode = 10;
        fn_1_22A8(player, 0);
    } else if (dist > work->unk48) {
        pos.x += work->unk2C.x;
        pos.z += work->unk2C.z;
        Hu3DModelPosSetV(work->mdlId, &pos);
    } else {
        Hu3DModelPosSetV(work->mdlId, &target);
        Hu3DModelAttrSet(work->unk52, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(work->unk54, HU3D_ATTR_DISPOFF);
        if (lbl_1_bss_0.unk10 == 0) {
            if (work->unk74 == 0) {
                work->mode = 7;
            } else {
                work->mode = 10;
                fn_1_22A8(player, 0);
            }
        } else {
            fn_1_22A8(player, 0);
            work->mode = 9;
        }
    }
    if (fn_1_69B0(player)) {
        work->mode = 5;
        omVibrate(player, 20, 7, 3);
    }
}

void fn_1_4430(s16 player)
{
    M650_PLAYER *work = &lbl_1_bss_28[player];
    HuVecF pos;
    s16 time = work->unk48;

    if (work->unk18 % 4 == 0) {
        Hu3DModelAttrSet(work->unk52, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(work->unk54, HU3D_ATTR_DISPOFF);
    } else if (work->unk18 % 4 == 2) {
        Hu3DModelAttrReset(work->unk52, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrReset(work->unk54, HU3D_ATTR_DISPOFF);
    }
    if (work->unk18++ > time) {
        Hu3DModelAttrSet(work->unk52, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(work->unk54, HU3D_ATTR_DISPOFF);
        if (lbl_1_bss_0.unk10 == 0) {
            work->mode = 6;
            fn_1_2464(player);
        } else {
            work->mode = 9;
            fn_1_22A8(player, 0);
        }
        Hu3DMotionSpeedSet(work->mdlId, 1.0f);
        work->unk18 = 0;
    } else {
        Hu3DModelPosGet(work->mdlId, &pos);
        pos.x += work->unk38.x * work->unk48 * 0.5f;
        pos.z += work->unk38.z * work->unk48 * 0.5f;
        if (pos.x < -1150.0f) {
            pos.x = -1150.0f;
        }
        if (pos.x > 1150.0f) {
            pos.x = 1150.0f;
        }
        Hu3DModelPosSetV(work->mdlId, &pos);
        Hu3DMotionSpeedSet(work->mdlId, 0.3f);
    }
}

void fn_1_473C(s16 player)
{
    M650_PLAYER *work = &lbl_1_bss_28[player];
    HuVecF rot;

    if (--work->unk16 != 0) {
        Hu3DModelRotGet(work->mdlId, &rot);
        rot.y += 3.0f;
        if (rot.y >= 180.0f) {
            rot.y -= 360.0f;
        }
        Hu3DModelRotSetV(work->mdlId, &rot);
    } else {
        fn_1_22A8(player, 0);
        fn_1_2320(player, 0);
        work->mode = 8;
        work->unk44 = 0.0f;
        work->unk50 = 0;
    }
}

void fn_1_48C0(s16 player)
{
    M650_PLAYER *work = &lbl_1_bss_28[player];

    if (work->unk18++ == 0) {
        fn_1_22A8(player, 3);
    }
    if (work->unk18 >= 30) {
        fn_1_22A8(player, 0);
        work->mode = 8;
        work->unk18 = 0;
    }
}

void fn_1_49C8(s16 player)
{
    M650_PLAYER *work = &lbl_1_bss_28[player];
    s16 seqMode = MgSeqModeGet();
    HuVecF rot;

    Hu3DModelRotGet(work->mdlId, &rot);
    if (rot.y < 0.0f) {
        rot.y += 2.0f;
        if (rot.y > 0.0f) {
            rot.y = 0.0f;
        }
    } else if (rot.y > 0.0f) {
        rot.y -= 2.0f;
        if (rot.y < 0.0f) {
            rot.y = 0.0f;
        }
    }
    Hu3DModelRotSetV(work->mdlId, &rot);
    if (0.0f == rot.y) {
        work->unk48 = 0.0f;
        if (seqMode == 5) {
            if (lbl_1_bss_28[player].unk74 == 0) {
                work->mode = 1;
                work->unk44 = 0.0f;
                fn_1_289C(player);
            } else {
                work->mode = 10;
            }
        } else {
            work->mode = 9;
            fn_1_22A8(player, 0);
        }
        work->unk64.x = work->unk64.y = work->unk64.z = 0.0f;
    }
}

void fn_1_4BD4(s16 player)
{
    M650_PLAYER *work = &lbl_1_bss_28[player];
    s16 seqMode = MgSeqModeGet();
    HuVecF rot;

    Hu3DModelRotGet(work->mdlId, &rot);
    if (rot.y < 180.0f) {
        rot.y += 4.0f;
        if (rot.y > 180.0f) {
            rot.y = 180.0f;
        }
    } else if (rot.y > -180.0f) {
        rot.y -= 4.0f;
        if (rot.y < -180.0f) {
            rot.y = 180.0f;
        }
    }
    Hu3DModelRotSetV(work->mdlId, &rot);
    if (180.0f == rot.y) {
        work->mode = 9;
        fn_1_22A8(player, 3);
    }
}

s16 fn_1_4D58(s16 time)
{
    u16 camBit[4] = { HU3D_CAM0, HU3D_CAM1, HU3D_CAM2, HU3D_CAM3 };
    HuVec2f vpX;
    HuVec2f vpY;
    float centerX;
    float centerY;
    float ofsX;
    float ofsY;
    int i;

    centerX = 640.0f - 5.3333335f * time;
    centerY = 480.0f - 4.0f * time;
    ofsX = (16.0f * time) / 60.0f;
    ofsY = (40.0f * time) / 60.0f;
    for (i = 0; i < 4; i++) {
        switch (i) {
            case 0:
                vpX.x = 0.0f;
                vpY.x = 0.0f;
                vpX.y = centerX + ofsX;
                vpY.y = centerY + ofsY;
                Hu3DCameraScissorSet(camBit[i], 0, 0, centerX - 2.0f, centerY - 2.0f);
                break;
            case 1:
                vpX.x = centerX - ofsX;
                vpY.x = 0.0f;
                vpX.y = 640.0f - centerX + ofsX;
                vpY.y = centerY + ofsY;
                Hu3DCameraScissorSet(camBit[i], 2.0f + centerX, 0, 640.0f - centerX - 2.0f, centerY - 2.0f);
                break;
            case 2:
                vpX.x = 0.0f;
                vpY.x = centerY - ofsY;
                vpX.y = centerX + ofsX;
                vpY.y = 480.0f - centerY + ofsY;
                Hu3DCameraScissorSet(camBit[i], 0, 2.0f + centerY, centerX - 2.0f, 480.0f - centerY - 2.0f);
                break;
            case 3:
                vpX.x = centerX - ofsX;
                vpY.x = centerY - ofsY;
                vpX.y = 640.0f - centerX + ofsX;
                vpY.y = 480.0f - centerY + ofsY;
                Hu3DCameraScissorSet(camBit[i], 2.0f + centerX, 2.0f + centerY, 640.0f - centerX - 2.0f, 480.0f - centerY - 2.0f);
                break;
        }
        Hu3DCameraViewportSet(camBit[i], vpX.x, vpY.x, vpX.y, vpY.y, 0.0f, 1.0f);
    }
    if (time >= 60) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void fn_1_5258(void)
{
    if (!_CheckFlag(FLAG_INST_DECA)) {
        lbl_1_bss_0.recordTime = GWRecordGet(5);
        if (lbl_1_bss_0.recordTime == 0) {
            lbl_1_bss_0.recordTime = 3600;
        }
        lbl_1_bss_0.timer = MgTimerCreate(MGTIMER_TYPE_RECORD);
        MgTimerParamSet(lbl_1_bss_0.timer, 0, 18000, lbl_1_bss_0.recordTime);
        MgTimerRecordDispOn(lbl_1_bss_0.timer);
    }
}

void fn_1_5308(void)
{
    if (!_CheckFlag(FLAG_INST_DECA)) {
        MgTimerModeOnSet(lbl_1_bss_0.timer, MGTIMER_OFFTYPE_NORMAL);
        HuAudFXPlay(13);
    }
}

void fn_1_5354(void)
{
    if (!_CheckFlag(FLAG_INST_DECA)) {
        MgTimerModeOffSet(lbl_1_bss_0.timer);
    }
}

void fn_1_5394(void)
{
    HuVecF timerPos[4] = {
        { 76.0f, 224.0f, 0.0f },
        { 516.0f, 224.0f, 0.0f },
        { 76.0f, 424.0f, 0.0f },
        { 516.0f, 424.0f, 0.0f },
    };
    int i;

    if (_CheckFlag(FLAG_INST_DECA)) {
        for (i = 0; i < 4; i++) {
            lbl_1_bss_28[i].timer = MgTimerCreate(MGTIMER_TYPE_SCORE);
            MgTimerParamSet(lbl_1_bss_28[i].timer, 0, 5400, 0);
            MgTimerPosSet(lbl_1_bss_28[i].timer, timerPos[i].x, timerPos[i].y);
            MgTimerRecordDispOn(lbl_1_bss_28[i].timer);
        }
    }
}

void fn_1_54D4(void)
{
    int i;

    if (_CheckFlag(FLAG_INST_DECA)) {
        for (i = 0; i < 4; i++) {
            MgTimerModeOnSet(lbl_1_bss_28[i].timer, MGTIMER_OFFTYPE_FLASH);
        }
        HuAudFXPlay(13);
    }
}

void fn_1_5544(s16 player)
{
    if (_CheckFlag(FLAG_INST_DECA)) {
        MgTimerModeOffSet(lbl_1_bss_28[player].timer);
        if (lbl_1_bss_28[player].timer->stopF == FALSE) {
            lbl_1_bss_28[player].timer->stopF = TRUE;
            lbl_1_bss_28[player].timer->mode = MGTIMER_MODE_ON;
        }
    }
}

void fn_1_5600(s16 player, float dist, float *speed, float *angle)
{
    float t = 5.444444768958626 - dist / 100.0f;

    *angle = HuAtan(t, 8.300000190734863);
    if (*angle < 90.0f) {
        *speed = 8.300000190734863 / (0.3333333432674408 * HuCos(*angle));
    } else {
        *speed = 0.0f;
    }
}

s16 fn_1_5728(void)
{
    s16 seqMode = MgSeqModeGet();
    HuVecF pos;
    int i;
    s32 time;

    if (seqMode != 5) {
        return FALSE;
    }
    if (_CheckFlag(FLAG_INST_DECA)) {
        for (i = 0; i < 4; i++) {
            Hu3DModelPosGet(lbl_1_bss_28[i].mdlId, &pos);
            if (pos.z >= 5000.0f) {
                lbl_1_bss_28[i].score = MgTimerValueGet(lbl_1_bss_28[i].timer);
                fn_1_5544(i);
                lbl_1_bss_28[i].unk74 = 1;
            }
        }
        for (i = 0; i < 4; i++) {
            if (lbl_1_bss_28[i].unk74 == 0) {
                break;
            }
        }
        if (i >= 4) {
            return TRUE;
        }
    } else {
        for (i = 0; i < 4; i++) {
            Hu3DModelPosGet(lbl_1_bss_28[i].mdlId, &pos);
            if (pos.z >= 5000.0f) {
                lbl_1_bss_0.winner = i;
                if (!_CheckFlag(FLAG_MG_PRACTICE)) {
                    GwPlayer[i].mgCoinBonus = 10;
                }
                time = MgTimerValueGet(lbl_1_bss_0.timer);
                if (lbl_1_bss_28[i].comDif == -1 && time < lbl_1_bss_0.recordTime && !_CheckFlag(FLAG_MG_PRACTICE)) {
                    MgSeqRecordSet(time);
                    lbl_1_bss_0.unk14 = 1;
                    GWRecordSet(5, time);
                }
                fn_1_5354();
                return TRUE;
            }
        }
    }
    return FALSE;
}

void fn_1_5A24(s16 player)
{
    M650_PLAYER *work = &lbl_1_bss_28[player];
    s16 seqMode = MgSeqModeGet();
    HuVecF pos;
    OM_CAMERA_VIEW view;
    s16 time;

    if (seqMode == 5) {
        Hu3DModelPosGet(work->mdlId, &pos);
        switch (work->mode) {
            case 1:
                view.center.x = pos.x;
                view.center.y = 230.0f;
                view.center.z = pos.z;
                view.rot.x = -27.0f;
                view.rot.y = -180.0f;
                view.rot.z = 0.0f;
                view.zoom = 1300.0f;
                time = 30;
                break;
            case 2:
                view.center.x = pos.x;
                view.center.y = 230.0f;
                view.center.z = pos.z;
                view.rot.x = -8.0f;
                view.rot.y = -180.0f;
                view.rot.z = 0.0f;
                view.zoom = 500.0f;
                time = 30;
                break;
            case 3:
                view.center.x = pos.x;
                view.center.y = 230.0f;
                view.center.z = pos.z;
                view.rot.x = -8.0f;
                view.rot.y = -180.0f;
                view.rot.z = 0.0f;
                view.zoom = 1000.0f;
                time = 30;
                break;
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
                view.center.x = pos.x;
                view.center.y = 230.0f;
                view.center.z = pos.z;
                view.rot.x = -15.0f;
                view.rot.y = -180.0f;
                view.rot.z = 0.0f;
                view.zoom = 1000.0f;
                time = 30;
                break;
            case 10:
                view.center.x = pos.x;
                view.center.y = 180.0f;
                view.center.z = pos.z;
                view.rot.x = -11.0f;
                view.rot.y = -180.0f;
                view.rot.z = 0.0f;
                view.zoom = 2000.0f;
                time = 30;
                break;
            case 9:
                if (_CheckFlag(FLAG_INST_DECA)) {
                    view.center.x = pos.x;
                    view.center.y = 180.0f;
                    view.center.z = pos.z;
                    view.rot.x = -11.0f;
                    view.rot.y = -180.0f;
                    view.rot.z = 0.0f;
                    view.zoom = 2000.0f;
                    time = 30;
                } else {
                    view.center.x = pos.x;
                    view.center.y = 280.0f;
                    view.center.z = pos.z;
                    view.rot.x = -15.0f;
                    view.rot.y = -180.0f;
                    view.rot.z = 0.0f;
                    view.zoom = 1000.0f;
                    time = 30;
                }
                break;
            default:
                view.center.x = pos.x;
                view.center.y = 230.0f;
                view.center.z = pos.z;
                view.rot.x = -15.0f;
                view.rot.y = -180.0f;
                view.rot.z = 0.0f;
                view.zoom = 1000.0f;
                time = 30;
                break;
        }
        omCameraViewMoveSimpleMulti(lbl_1_data_150[player], &view, time);
    }
}

void fn_1_5E40(void)
{
    int i;

    if (HuPadBtn[0] & PAD_BUTTON_TRIGGER_R) {
        for (i = 0; i < 4; i++) {
            if (HuPadBtn[0] & PAD_BUTTON_X) {
                CZoomM[i] += 5.0f;
            }
            if (HuPadBtn[0] & PAD_BUTTON_Y) {
                CZoomM[i] -= 5.0f;
            }
            if (HuPadSubStkY[0] > 32) {
                CRotM[i].x += 1.0f;
            }
            if (HuPadSubStkY[0] < -32) {
                CRotM[i].x -= 1.0f;
            }
            if (HuPadSubStkX[0] > 32) {
                CRotM[i].y += 1.0f;
            }
            if (HuPadSubStkX[0] < -32) {
                CRotM[i].y -= 1.0f;
            }
            if (HuPadStkX[0] > 5 || HuPadStkX[0] < -5) {
                CenterM[i].x += HuPadStkX[0];
            }
            if (HuPadStkY[0] > 5 || HuPadStkY[0] < -5) {
                CenterM[i].z -= HuPadStkY[0];
            }
            if (HuPadBtn[0] & PAD_BUTTON_UP) {
                CenterM[i].y += 5.0f;
            }
            if (HuPadBtn[0] & PAD_BUTTON_DOWN) {
                CenterM[i].y -= 5.0f;
            }
        }
        print8(100, 300, 1.8f, "CnrX:%f", CenterM[0].x);
        print8(100, 316, 1.8f, "CnrY:%f", CenterM[0].y);
        print8(100, 332, 1.8f, "CnrZ:%f", CenterM[0].z);
        print8(100, 348, 1.8f, "RotZ:%f", CRotM[0].x);
        print8(100, 364, 1.8f, "RotY:%f", CRotM[0].y);
        print8(100, 380, 1.8f, "RotZ:%f", CRotM[0].z);
        print8(100, 396, 1.8f, "Zoom:%f", CZoomM[0]);
    }
}
