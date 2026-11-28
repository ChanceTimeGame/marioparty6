#include "REL/m608dll/m608.h"

void ObjectSetup(void)
{
    HuVecF mdlPos;
    HuVecF lightAim;
    HuVecF lightPos;
    s32 i;
    HU3D_MODELID mdlId;
    HU3D_MODELID itemMdlId;
    s32 charNo;
    HU3D_LLIGHTID llightId;
    HU3D_LIGHTID lightIdA;
    HU3D_MODELID npcMdlId;
    HU3D_LIGHTID lightIdB;
    s16 motNo;
    s16 mgTime;
    float maxTime;

    Hu3DParManInit();
    memset(&lbl_1_bss_3E80, 0, sizeof(lbl_1_bss_3E80));
    lbl_1_bss_3E80.camModel = -1;
    lbl_1_bss_3E80.unk428.x = 0.0f;
    lbl_1_bss_3E80.unk428.y = 10000.0f;
    lbl_1_bss_3E80.unk428.z = 0.0f;
    lbl_1_bss_3E80.unk440.x = 0.0f;
    lbl_1_bss_3E80.unk440.y = 0.0f;
    lbl_1_bss_3E80.unk440.z = 1.0f;
    lbl_1_bss_3E80.unk434.x = 0.0f;
    lbl_1_bss_3E80.unk434.y = -100.0f;
    lbl_1_bss_3E80.unk434.z = 0.0f;
    lbl_1_bss_3E80.newRecordF = 0;
    lbl_1_bss_3E80.unk3DC = 0;
    lbl_1_bss_3E80.objman = omInitObjMan(100, 0x2000);
    omGameSysInit(lbl_1_bss_3E80.objman);
    fn_1_6828();
    CRot.x = -20.0f;
    CRot.y = 0.0f;
    CRot.z = 0.0f;
    Center.x = 0.0f;
    Center.y = 100.0f;
    Center.z = 0.0f;
    CZoom = 400.0f;
    Hu3DCameraCreate(HU3D_CAM0);
    Hu3DCameraPerspectiveSet(HU3D_CAM0, 45.0f, 20.0f, 800.0f, 1.2f);
    Hu3DCameraViewportSet(HU3D_CAM0, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    lbl_1_bss_3E80.unk3C = omAddObjEx(lbl_1_bss_3E80.objman, 16, 0, 0, -1, fn_1_2540);
    for (i = 0; i < 10; i++) {
        lbl_1_bss_3E80.unk10[i] = Hu3DMotionCreateData(lbl_1_data_4F0[i]);
        lbl_1_bss_3E80.unk24[i] = Hu3DModelCameraCreate(lbl_1_bss_3E80.unk10[i], HU3D_CAM0);
        Hu3DCameraMotionOff(lbl_1_bss_3E80.unk24[i]);
    }
    Hu3DShadowCreate(30.0f, 1.0f, 13000.0f);
    Hu3DShadowPosSet(&lbl_1_bss_3E80.unk428, &lbl_1_bss_3E80.unk440, &lbl_1_bss_3E80.unk434);
    Hu3DShadowColSet(50, 50, 50);
    Hu3DShadowSizeSet(192);
    Hu3DShadowTPLvlSet(0.7f);
    Hu3DShadowColSet(50, 50, 50);
    Hu3DShadowSizeSet(240);
    for (i = 0; i < 4; i++) {
        lbl_1_bss_3E80.stModel[i] = Hu3DModelCreateData(lbl_1_data_518[i]);
        lbl_1_bss_3E80.lsModel[i] = Hu3DModelCreateData(lbl_1_data_528[i]);
        lbl_1_bss_3E80.lsBModel[i] = Hu3DModelCreateData(lbl_1_data_538[i]);
        Hu3DModelLayerSet(lbl_1_bss_3E80.stModel[i], 1);
        Hu3DModelLayerSet(lbl_1_bss_3E80.lsModel[i], 1);
        Hu3DModelLayerSet(lbl_1_bss_3E80.lsBModel[i], 1);
        mdlPos.x = 0.0f;
        mdlPos.y = -2.0f;
        mdlPos.z = 0.0f;
        Hu3DModelPosSetV(lbl_1_bss_3E80.lsModel[i], &mdlPos);
        Hu3DModelPosSetV(lbl_1_bss_3E80.lsBModel[i], &mdlPos);
    }
    for (i = 0; i < 4; i++) {
        Hu3DModelObjPosGet(lbl_1_bss_3E80.lsModel[i], lbl_1_data_684[i], &lbl_1_bss_3E80.unk70[i]);
        Hu3DModelObjPosGet(lbl_1_bss_3E80.lsModel[i], lbl_1_data_624[i], &lbl_1_bss_3E80.unkD0[i]);
        Hu3DModelObjPosGet(lbl_1_bss_3E80.lsBModel[i], lbl_1_data_64C[i], &lbl_1_bss_3E80.unkD0[i]);
    }
    lbl_1_bss_3E80.kuruModel = Hu3DModelCreateData(DATANUM(DATA_m608, 0x2C));
    lbl_1_bss_3E80.kuruMot = Hu3DJointMotionData(lbl_1_bss_3E80.kuruModel, DATANUM(DATA_m608, 0x2D));
    Hu3DModelLayerSet(lbl_1_bss_3E80.kuruModel, 2);
    Hu3DModelShadowSet(lbl_1_bss_3E80.kuruModel);
    lbl_1_bss_3E80.unk1A8 = Hu3DModelCreateData(DATANUM(DATA_m608, 0x3A));
    lbl_1_bss_3E80.unk1AA = Hu3DModelCreateData(DATANUM(DATA_m608, 0x3B));
    mgTime = GwMgTime;
    if (mgTime == 0) {
        lbl_1_bss_3E80.unk4A = Hu3DModelCreateData(DATANUM(DATA_m608, 0x00));
        lbl_1_bss_3E80.unk4C = Hu3DModelCreateData(DATANUM(DATA_m608, 0x01));
        Hu3DModelAttrSet(lbl_1_bss_3E80.unk4A, HU3D_MOTATTR_LOOP);
        lbl_1_bss_3E80.mdlId[7] = Hu3DModelCreateData(DATANUM(DATA_m608, 0x40));
        lbl_1_bss_3E80.mdlId[3] = Hu3DModelCreateData(DATANUM(DATA_m608, 0x46));
        lbl_1_bss_3E80.mtnId[0] = Hu3DJointMotionData(lbl_1_bss_3E80.mdlId[3], DATANUM(DATA_m608, 0x47));
        lbl_1_bss_3E80.mtnId[1] = Hu3DJointMotionData(lbl_1_bss_3E80.mdlId[3], DATANUM(DATA_m608, 0x48));
        Hu3DModelAttrSet(lbl_1_bss_3E80.mdlId[3], HU3D_MOTATTR_LOOP);
        lbl_1_bss_3E80.mdlId[5] = Hu3DModelCreateData(DATANUM(DATA_m608, 0x53));
        lbl_1_bss_3E80.mtnId[4] = Hu3DJointMotionData(lbl_1_bss_3E80.mdlId[5], DATANUM(DATA_m608, 0x54));
        lbl_1_bss_3E80.mtnId[5] = Hu3DJointMotionData(lbl_1_bss_3E80.mdlId[5], DATANUM(DATA_m608, 0x55));
        fn_1_5FDC(5, 4);
        lbl_1_bss_3E80.mdlId[4] = Hu3DModelCreateData(DATANUM(DATA_m608, 0x4D));
        lbl_1_bss_3E80.mtnId[2] = Hu3DJointMotionData(lbl_1_bss_3E80.mdlId[4], DATANUM(DATA_m608, 0x4E));
        lbl_1_bss_3E80.mtnId[3] = Hu3DJointMotionData(lbl_1_bss_3E80.mdlId[4], DATANUM(DATA_m608, 0x4F));
        fn_1_5FDC(4, 2);
        Hu3DModelAttrSet(lbl_1_bss_3E80.mdlId[4], HU3D_MOTATTR_LOOP);
        lbl_1_bss_3E80.mdlId[8] = Hu3DModelCreateData(DATANUM(DATA_m608, 0x42));
        lbl_1_bss_3E80.mdlId[9] = Hu3DModelCreateData(DATANUM(DATA_m608, 0x44));
        {
        HuVecF lightPosA = { 100.0f, 800.0f, -100.0f };
        HuVecF lightDirA = { 0.3f, -0.8f, 0.3f };
        HuVecF lightUnkA = { 20.0f, 45.0f, 1000.0f };
        GXColor lightColA = { 255, 255, 255, 255 };

        lightIdA = Hu3DGLightCreateV(&lightPosA, &lightDirA, &lightColA);
        Hu3DGLightInfinitytSet(lightIdA);
        Hu3DGLightStaticSet(lightIdA, TRUE);
        lightAim.x = 0.0f;
        lightAim.y = 0.0f;
        lightAim.z = 0.0f;
        lightPos.x = 1.0f;
        lightPos.y = 7825.0f;
        lightPos.z = 9582.0f;
        Hu3DGLightPosAimSetV(lightIdA, &lightPos, &lightAim);
        Hu3DModelShadowMapObjSet(lbl_1_bss_3E80.unk4A, "kurukuru14");
        Hu3DModelShadowMapObjSet(lbl_1_bss_3E80.unk4A, "kurukuru14-hsha");
        }
    } else {
        lbl_1_bss_3E80.unk4A = Hu3DModelCreateData(DATANUM(DATA_m608, 0x03));
        lbl_1_bss_3E80.unk4E = Hu3DModelCreateData(DATANUM(DATA_m608, 0x05));
        lbl_1_bss_3E80.unk50 = Hu3DModelCreateData(DATANUM(DATA_m608, 0x02));
        lbl_1_bss_3E80.unk52 = Hu3DModelCreateData(DATANUM(DATA_m608, 0x06));
        Hu3DModelClusterAttrSet(lbl_1_bss_3E80.unk4A, 0, HU3D_CLUSTER_ATTR_LOOP);
        Hu3DModelAttrSet(lbl_1_bss_3E80.unk4E, HU3D_MOTATTR_LOOP);
        Hu3DModelAttrSet(lbl_1_bss_3E80.unk50, HU3D_MOTATTR_LOOP);
        Hu3DModelAttrSet(lbl_1_bss_3E80.unk52, HU3D_MOTATTR_LOOP);
        lbl_1_bss_3E80.mdlId[7] = Hu3DModelCreateData(DATANUM(DATA_m608, 0x41));
        lbl_1_bss_3E80.mdlId[3] = Hu3DModelCreateData(DATANUM(DATA_m608, 0x49));
        lbl_1_bss_3E80.mtnId[0] = Hu3DJointMotionData(lbl_1_bss_3E80.mdlId[3], DATANUM(DATA_m608, 0x4A));
        lbl_1_bss_3E80.mtnId[1] = Hu3DJointMotionData(lbl_1_bss_3E80.mdlId[3], DATANUM(DATA_m608, 0x4B));
        Hu3DModelAttrSet(lbl_1_bss_3E80.mdlId[3], HU3D_MOTATTR_LOOP);
        lbl_1_bss_3E80.mdlId[5] = Hu3DModelCreateData(DATANUM(DATA_m608, 0x53));
        lbl_1_bss_3E80.mtnId[4] = Hu3DJointMotionData(lbl_1_bss_3E80.mdlId[5], DATANUM(DATA_m608, 0x54));
        lbl_1_bss_3E80.mtnId[5] = Hu3DJointMotionData(lbl_1_bss_3E80.mdlId[5], DATANUM(DATA_m608, 0x55));
        fn_1_5FDC(5, 4);
        lbl_1_bss_3E80.mdlId[4] = Hu3DModelCreateData(DATANUM(DATA_m608, 0x50));
        lbl_1_bss_3E80.mtnId[2] = Hu3DJointMotionData(lbl_1_bss_3E80.mdlId[4], DATANUM(DATA_m608, 0x51));
        lbl_1_bss_3E80.mtnId[3] = Hu3DJointMotionData(lbl_1_bss_3E80.mdlId[4], DATANUM(DATA_m608, 0x52));
        fn_1_5FDC(4, 2);
        Hu3DModelAttrSet(lbl_1_bss_3E80.mdlId[4], HU3D_MOTATTR_LOOP);
        lbl_1_bss_3E80.mdlId[8] = Hu3DModelCreateData(DATANUM(DATA_m608, 0x43));
        lbl_1_bss_3E80.mdlId[9] = Hu3DModelCreateData(DATANUM(DATA_m608, 0x45));
        {
        HuVecF lightPosB = { 6379.0f, 4448.0f, -3.2f };
        HuVecF lightDirB = { 0.3f, -0.8f, 0.3f };
        HuVecF lightUnkB = { 20.0f, 45.0f, 1000.0f };
        GXColor lightColB = { 255, 255, 255, 255 };

        lightIdB = Hu3DGLightCreateV(&lightPosB, &lightDirB, &lightColB);
        Hu3DGLightPointSet(lightIdB, 1000.0f, 1.0f, GX_DA_OFF);
        Hu3DGLightStaticSet(lightIdB, TRUE);
        }
    }
    for (i = 0; i < 4; i++) {
        Hu3DModelShadowSet(lbl_1_bss_3E80.stModel[i]);
        Hu3DModelShadowSet(lbl_1_bss_3E80.lsModel[i]);
        Hu3DModelShadowSet(lbl_1_bss_3E80.lsBModel[i]);
    }
    lbl_1_bss_3E80.mdlId[0] = Hu3DModelCreateData(DATANUM(DATA_m608, 0x3C));
    lbl_1_bss_3E80.mdlId[1] = Hu3DModelCreateData(DATANUM(DATA_m608, 0x3D));
    fn_1_5F90(0);
    fn_1_5F90(1);
    fn_1_5F90(7);
    fn_1_5F90(8);
    fn_1_5F90(9);
    fn_1_5F90(5);
    lbl_1_bss_3E80.unk44C = frand() & 3;
    lbl_1_bss_3E80.mdlId[2] = Hu3DModelCreateData(DATANUM(DATA_m608, 0x4C));
    Hu3DModelAttrSet(lbl_1_bss_3E80.mdlId[2], HU3D_MOTATTR_LOOP);
    lbl_1_bss_3E80.mdlId[6] = Hu3DModelCreateData(DATANUM(DATA_m608, 0x5A));
    lbl_1_bss_3E80.mtnId[6] = Hu3DJointMotionData(lbl_1_bss_3E80.mdlId[6], DATANUM(DATA_m608, 0x5B));
    lbl_1_bss_3E80.mtnId[7] = Hu3DJointMotionData(lbl_1_bss_3E80.mdlId[6], DATANUM(DATA_m608, 0x5C));
    {
    char *hookName[10] = { "NPC3", "NPC4", "NPC5", "NPC6", "NPC7", "NPC8", "NPC9", "NPC10", "NPC11", "NPC12" };
    s32 hookMot[10] = { 1, 1, 1, 0, 1, 1, 0, 1, 1, 1 };

    for (i = 0; i < 10; i++) {
        mdlId = Hu3DModelLink(lbl_1_bss_3E80.mdlId[6]);
        motNo = hookMot[i] ? 7 : 6;
        Hu3DModelHookSet(lbl_1_bss_3E80.mdlId[2], hookName[i], mdlId);
        Hu3DMotionSet(mdlId, lbl_1_bss_3E80.mtnId[motNo]);
        maxTime = Hu3DMotionMaxTimeGet(mdlId);
        Hu3DMotionTimeSet(mdlId, maxTime * frandf());
        Hu3DModelAttrSet(mdlId, HU3D_MOTATTR_LOOP);
        llightId = Hu3DLLightCreate(mdlId, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 128, 128, 128);
        Hu3DLLightInfinitytSet(mdlId, llightId);
    }
    }
    npcMdlId = lbl_1_bss_3E80.mdlId[6];
    Hu3DModelHookSet(lbl_1_bss_3E80.mdlId[4], "npc1", npcMdlId);
    Hu3DMotionSet(npcMdlId, lbl_1_bss_3E80.mtnId[6]);
    Hu3DModelAttrSet(npcMdlId, HU3D_MOTATTR_LOOP);
    lbl_1_bss_3E80.mdlId[10] = Hu3DModelCreateData(DATANUM(DATA_m608, 0x5D));
    lbl_1_bss_3E80.mtnId[8] = Hu3DJointMotionData(lbl_1_bss_3E80.mdlId[10], DATANUM(DATA_m608, 0x5E));
    lbl_1_bss_3E80.mdlId[11] = Hu3DModelCreateData(DATANUM(DATA_m608, 0x56));
    lbl_1_bss_3E80.mdlId[12] = Hu3DModelCreateData(DATANUM(DATA_m608, 0x57));
    lbl_1_bss_3E80.mdlId[13] = Hu3DModelCreateData(DATANUM(DATA_m608, 0x58));
    lbl_1_bss_3E80.mdlId[14] = Hu3DModelCreateData(DATANUM(DATA_m608, 0x59));
    itemMdlId = lbl_1_bss_3E80.mdlId[10];
    Hu3DModelHookSet(lbl_1_bss_3E80.mdlId[5], "npc2", itemMdlId);
    Hu3DModelHookSet(itemMdlId, "itemhook_L", lbl_1_bss_3E80.mdlId[14]);
    Hu3DModelHookSet(itemMdlId, "itemhook_R", lbl_1_bss_3E80.mdlId[14]);
    Hu3DModelHookSet(itemMdlId, "itemhook_C", lbl_1_bss_3E80.mdlId[11]);
    Hu3DModelAttrSet(lbl_1_bss_3E80.mdlId[10], HU3D_MOTATTR_LOOP);
    for (i = 0; i < 4; i++) {
        charNo = lbl_1_bss_3E80.charNo[i] = GwPlayerConf[i].charNo;
        lbl_1_bss_3E80.padNo[i] = GwPlayerConf[i].padNo;
        lbl_1_bss_3E80.charModel[i] = CharModelMotListCreate(charNo, 4, lbl_1_data_548, lbl_1_bss_3E80.charMot[i]);
        lbl_1_bss_3E80.charObj[i] = omAddObjEx(lbl_1_bss_3E80.objman, 0x2200, 1, 0, -1, NULL);
        *lbl_1_bss_3E80.charObj[i]->mdlId = lbl_1_bss_3E80.charModel[i];
        Hu3DModelShadowSet(lbl_1_bss_3E80.charModel[i]);
        CharMotionDataClose(charNo);
        OSReport("*** %dP ... char %d  pad %d\n", i, charNo, lbl_1_bss_3E80.padNo[i]);
    }
    for (i = 0; i < 4; i++) {
        lbl_1_bss_3E80.boardModel[i] = Hu3DModelCreateData(DATANUM(DATA_m608, 0x3F));
        Hu3DModelShadowSet(lbl_1_bss_3E80.boardModel[i]);
        Hu3DModelLayerSet(lbl_1_bss_3E80.boardModel[i], 3);
    }
    lbl_1_bss_3E80.unk170 = Hu3DModelCreateData(DATANUM(DATA_m608, 0x3E));
    Hu3DModelLayerSet(lbl_1_bss_3E80.unk170, 3);
    for (i = 0; i < 27; i++) {
        lbl_1_bss_3E80.unk1C0[i] = Hu3DModelCreateData(lbl_1_data_694[i]);
        lbl_1_bss_3E80.unk1F6[i] = Hu3DJointMotionData(lbl_1_bss_3E80.unk1C0[i], lbl_1_data_694[i]);
        Hu3DModelAttrSet(lbl_1_bss_3E80.unk1C0[i], HU3D_ATTR_DISPOFF);
        lbl_1_bss_3E80.unk370[i] = omAddObjEx(lbl_1_bss_3E80.objman, 0x7FDA, 1, 1, -1, fn_1_388C);
        lbl_1_bss_3E80.unk370[i]->work[0] = i;
    }
    for (i = 0; i < 4; i++) {
        llightId = Hu3DLLightCreate(lbl_1_bss_3E80.charModel[i], 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, -1.0f, 180, 180, 180);
        Hu3DLLightInfinitytSet(lbl_1_bss_3E80.charModel[i], llightId);
    }
    fn_1_6150();
    fn_1_6534(4, GWRecordGet(GW_RECORD_M608));
    lbl_1_bss_3E80.record = GWRecordGet(GW_RECORD_M608);
    fn_1_69F4();
    fn_1_6148();
    lbl_1_bss_3E80.streamNo = -1;
    MgSeqCreate(&lbl_1_data_0);
}
