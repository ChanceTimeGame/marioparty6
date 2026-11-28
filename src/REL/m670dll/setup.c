#include "REL/m670dll/m670.h"

M670_WORK lbl_1_bss_10;

s32 lbl_1_data_78[6] = {
    DATANUM(DATA_m670, 0x0A),
    DATANUM(DATA_m670, 0x0F),
    DATANUM(DATA_m670, 0x14),
    DATANUM(DATA_m670, 0x19),
    DATANUM(DATA_m670, 0x1E),
    DATANUM(DATA_m670, 0x23)
};

s32 lbl_1_data_90[6][4] = {
    { DATANUM(DATA_m670, 0x0B), DATANUM(DATA_m670, 0x0C), DATANUM(DATA_m670, 0x0D), DATANUM(DATA_m670, 0x0E) },
    { DATANUM(DATA_m670, 0x10), DATANUM(DATA_m670, 0x11), DATANUM(DATA_m670, 0x12), DATANUM(DATA_m670, 0x13) },
    { DATANUM(DATA_m670, 0x15), DATANUM(DATA_m670, 0x16), DATANUM(DATA_m670, 0x17), DATANUM(DATA_m670, 0x18) },
    { DATANUM(DATA_m670, 0x1A), DATANUM(DATA_m670, 0x1B), DATANUM(DATA_m670, 0x1C), DATANUM(DATA_m670, 0x1D) },
    { DATANUM(DATA_m670, 0x1F), DATANUM(DATA_m670, 0x20), DATANUM(DATA_m670, 0x21), DATANUM(DATA_m670, 0x22) },
    { DATANUM(DATA_m670, 0x24), DATANUM(DATA_m670, 0x25), DATANUM(DATA_m670, 0x26), DATANUM(DATA_m670, 0x27) }
};

s32 lbl_1_data_F0[5] = {
    DATANUM(DATA_m670, 0x03),
    DATANUM(DATA_m670, 0x04),
    DATANUM(DATA_m670, 0x06),
    DATANUM(DATA_m670, 0x05),
    DATANUM(DATA_m670, 0x09)
};

s32 lbl_1_data_104[3] = {
    DATANUM(DATA_m670, 0x00),
    DATANUM(DATA_m670, 0x01),
    DATANUM(DATA_m670, 0x02)
};

unsigned int lbl_1_data_110[12] = {
    DATANUM(DATA_mariomot, 0x00), DATANUM(DATA_mariomot, 0x01), DATANUM(DATA_mariomot, 0x02),
    DATANUM(DATA_mariomot, 0x03), DATANUM(DATA_mariomot, 0x04), DATANUM(DATA_mariomot, 0x06),
    DATANUM(DATA_mariomot, 0x07), DATANUM(DATA_mariomot, 0x5E), DATANUM(DATA_mariomot, 0x17),
    DATANUM(DATA_mariomot, 0x22), 0
};

BOOL fn_1_1460(HuVecF *src, HuVecF *dst)
{
    BOOL result;

    if (VECSquareMag(src) < 0.000001) {
        dst->x = 0.01f * ((float)frandmod(20) - 10.0f);
        dst->z = 0.01f * ((float)frandmod(20) - 10.0f);
        dst->y = frandmod(1) ? 0.01f : -0.01f;
        VECNormalize(dst, dst);
        result = FALSE;
    }
    else {
        VECNormalize(src, dst);
        result = TRUE;
    }
    return result;
}

void fn_1_15B8(s32 seId, HuVecF *pos)
{
    s32 pan;
    s32 seNo;
    HuVecF pos2D;

    Hu3D3Dto2D(pos, 1, &pos2D);
    pan = pos2D.x;
    pan /= 5;
    if (pan < 48) {
        pan = 48;
    }
    else if (pan > 127) {
        pan = 127;
    }
    seNo = HuAudFXPlay(seId);
    HuAudFXPanning(seNo, pan);
}

void ObjectSetup(void)
{
    s32 i;
    s32 j;
    s8 *pattern;
    HU3D_MODELID modelId;
    OMOBJ *obj;
    s32 teamNo;
    s8 type;
    HU3D_LIGHTID lightId;
    HuVecF lightPos;
    HuVecF lightTarget;
    HuVecF lightDir;
    HuVecF shadowPos;
    HuVecF shadowUp;
    HuVecF shadowTarget;
    HuVecF pos;

    lbl_1_bss_10.objman = MgActorObjectSetup();
    lbl_1_bss_10.pattern = frandmod(5);
    CRot.x = -35.0f;
    CRot.y = 0.0f;
    CRot.z = 0.0f;
    Center.x = 0.0f;
    Center.y = 700.0f;
    Center.z = 400.0f;
    CZoom = 1000.0f;
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 45.0f, 20.0f, 8000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    for (i = 0; i < 3; i++) {
        lbl_1_bss_10.camMot[i] = Hu3DMotionCreate(HuDataSelHeapReadNum(lbl_1_data_104[i], HU_MEMNUM_OVL, HEAP_MODEL));
        lbl_1_bss_10.camModel[i] = Hu3DModelCameraCreate(lbl_1_bss_10.camMot[i], 1);
        Hu3DCameraMotionOff(lbl_1_bss_10.camModel[i]);
    }
    lightPos.x = 0.0f;
    lightPos.y = 600.0f;
    lightPos.z = 1000.0f;
    lightTarget.x = 0.0f;
    lightTarget.y = 0.0f;
    lightTarget.z = 0.0f;
    VECSubtract(&lightTarget, &lightPos, &lightDir);
    fn_1_1460(&lightDir, &lightDir);
    lightId = Hu3DGLightCreate(lightPos.x, lightPos.y, lightPos.z, lightDir.x, lightDir.y, lightDir.z, 255, 255, 255);
    Hu3DGLightInfinitytSet(lightId);
    Hu3DGLightStaticSet(0, TRUE);
    shadowPos.x = 0.0f;
    shadowPos.y = 10000.0f;
    shadowPos.z = 0.0f;
    shadowUp.x = 0.0f;
    shadowUp.y = 1.0f;
    shadowUp.z = 0.0f;
    shadowTarget.x = 0.0f;
    shadowTarget.y = 0.0f;
    shadowTarget.z = -0.1f;
    Hu3DShadowCreate(8.5f, 5000.0f, 11000.0f);
    Hu3DShadowPosSet(&shadowPos, &shadowUp, &shadowTarget);
    for (i = 0; i < 3; i++) {
        lbl_1_bss_10.stageModel[i] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_F0[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    Hu3DModelShadowMapObjSet(lbl_1_bss_10.stageModel[2], "Cylinder12");
    Hu3DModelLayerSet(lbl_1_bss_10.stageModel[2], 2);
    lbl_1_bss_10.colNum = 0;
    lbl_1_bss_10.posModel = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m670, 0x07), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_10.posModel, HU3D_ATTR_DISPOFF);
    lbl_1_bss_10.resultPosModel = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m670, 0x08), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_10.resultPosModel, HU3D_ATTR_DISPOFF);
    {
        char *pillarName[M670_PILLAR_NUM] = {
            "D01", "D02", "D03", "D04", "D05", "D06", "D07", "D08",
            "D09", "D10", "D11", "D12", "D13", "D14", "D15", "D16",
            "D17", "D18", "D19", "D20", "D21", "D22", "D23", "D24"
        };
        for (i = 0; i < M670_PILLAR_NUM; i++) {
            Hu3DModelObjPosGet(lbl_1_bss_10.posModel, pillarName[i], &lbl_1_bss_10.pillarPos[i]);
        }
    }
    pattern = lbl_1_data_250[lbl_1_bss_10.pattern];
    for (i = 0; i < M670_PILLAR_NUM; i++) {
        type = *pattern;
        modelId = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_78[type], HU_MEMNUM_OVL, HEAP_MODEL));
        lbl_1_bss_10.pillarModel[i] = modelId;
        Hu3DModelShadowMapObjSet(modelId, "dai");
        Hu3DModelLayerSet(modelId, 5);
        for (j = 0; j < 4; j++) {
            lbl_1_bss_10.pillarMot[i][j] = Hu3DJointMotion(modelId, HuDataSelHeapReadNum(lbl_1_data_90[type][j], HU_MEMNUM_OVL, HEAP_MODEL));
        }
        lbl_1_bss_10.colModel[i] = lbl_1_bss_10.colMapModel[lbl_1_bss_10.colNum] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_m670, 0x09), HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(lbl_1_bss_10.colModel[i], HU3D_ATTR_DISPOFF);
        fn_1_22F0(i, -1500.0f);
        pattern++;
        lbl_1_bss_10.colNum++;
    }
    lbl_1_bss_10.stageMode = 0;
    lbl_1_bss_10.stageObj = omAddObjEx(lbl_1_bss_10.objman, 0x2000, 0, 0, -1, fn_1_3098);
    for (i = 0; i < M670_PILLAR_NUM; i++) {
        Hu3DMotionSet(lbl_1_bss_10.pillarModel[i], lbl_1_bss_10.pillarMot[i][0]);
        Hu3DModelAttrReset(lbl_1_bss_10.pillarModel[i], HU3D_MOTATTR_LOOP);
    }
    MgActorColMapInit(lbl_1_bss_10.colMapModel, lbl_1_bss_10.colNum, 50);
    {
        char *playerName[3] = { "P01", "P02", "P03" };
        MGACTOR_PARAM param;
        MGPLAYER *player;
        param.height = 150.0f;
        param.radius = 60.0f;
        param.param = 0;
        param.type = 0;
        param.attr = 0;
        param.narrowHook = NULL;
        param.correctHook = fn_1_2384;
        teamNo = 0;
        for (i = 0; i < 4; i++) {
            lbl_1_bss_10.charNo[i] = GwPlayerConf[i].charNo;
            lbl_1_bss_10.padNo[i] = GwPlayerConf[i].padNo;
            lbl_1_bss_10.playerMode[i] = 0;
            param.correctHookParam = i;
            player = MgPlayerCreate(i, &param, 4, 1, 0xFFFFFFE3, lbl_1_data_110);
            lbl_1_bss_10.player[i] = player;
            MgPlayerVibrateCreate(player);
            if (GwPlayerConf[i].grpNo == 0) {
                lbl_1_bss_10.teamF[i] = 0;
                lbl_1_bss_10.soloNo = i;
                lbl_1_bss_10.soloPadNo = lbl_1_bss_10.padNo[i];
                lbl_1_bss_10.soloCharNo = GwPlayerConf[i].charNo;
                Hu3DModelObjPosGet(lbl_1_bss_10.stageModel[2], "P00", &pos);
                MgPlayerPosSet(player, &pos);
                Hu3DModelPosSetV(lbl_1_bss_10.player[i]->actor->mdlId, &pos);
                MgPlayerDespawn(player);
                lbl_1_bss_10.playerObj[i] = omAddObjEx(lbl_1_bss_10.objman, 0x4000, 0, 0, -1, fn_1_2690);
                Hu3DModelLayerSet(lbl_1_bss_10.player[i]->actor->mdlId, 2);
            }
            else {
                lbl_1_bss_10.teamF[i] = 1;
                Hu3DModelObjPosGet(lbl_1_bss_10.posModel, playerName[teamNo], &pos);
                pos.y = -1500.0f;
                MgPlayerPosSet(player, &pos);
                lbl_1_bss_10.playerObj[i] = omAddObjEx(lbl_1_bss_10.objman, 0x4000, 0, 0, -1, fn_1_28E8);
                Hu3DModelLayerSet(lbl_1_bss_10.player[i]->actor->mdlId, 5);
                teamNo++;
            }
            obj = lbl_1_bss_10.playerObj[i];
            obj->work[0] = i;
            obj->work[1] = (u32)player;
            Hu3DModelShadowSet(lbl_1_bss_10.player[i]->actor->mdlId);
            MgPlayerAttrSet(lbl_1_bss_10.player[i], 1);
            param.param++;
        }
    }
    CharEffectLayerSet(5);
    lbl_1_bss_10.timer = MgTimerCreate(0);
    Hu3DZClearLayerSet(5);
    HuMCInit(0);
    HuMCSelWinCreate(HUMC_SEL_POS_LEFT, HUMC_SEL_POS_TOP);
    lbl_1_bss_10.micCtx = HuMCContextCreate("/mic/ctx/m670_words");
    fn_1_3B30();
    lbl_1_bss_10.streamNo = -1;
    MgSeqCreate(&lbl_1_data_0);
}

void fn_1_22F0(s32 no, float y)
{
    HuVecF pos = lbl_1_bss_10.pillarPos[no];
    pos.y = y;
    Hu3DModelPosSetV(lbl_1_bss_10.pillarModel[no], &pos);
    Hu3DModelPosSetV(lbl_1_bss_10.colModel[no], &pos);
}
