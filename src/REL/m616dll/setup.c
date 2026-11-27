#include "REL/m616dll/m616.h"

M616WORK lbl_1_bss_10;

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
