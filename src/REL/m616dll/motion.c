#include "REL/m616dll/m616.h"

void fn_1_1FF4(s32 no)
{
    HU3D_MODELID modelId;
    HU3D_MOTIONID motId;

    motId = lbl_1_bss_10.camMot[no];
    modelId = Hu3DModelCameraCreate(motId, HU3D_CAM0);
    Hu3DCameraMotionStart(modelId, HU3D_CAM0);
    lbl_1_bss_10.camTime = 0.0f;
    lbl_1_bss_10.camMaxTime = Hu3DMotionMaxTimeGet(modelId);
    if (lbl_1_bss_10.camModel != HU3D_MODELID_NONE) {
        Hu3DModelKill(lbl_1_bss_10.camModel);
    }
    lbl_1_bss_10.camModel = modelId;
    OSReport("start camera motion ... idx:%d time:%f\n", no, lbl_1_bss_10.camMaxTime);
}

BOOL fn_1_20D8(void)
{
    return Hu3DMotionEndCheck(lbl_1_bss_10.camModel);
}

void fn_1_2104(s32 playerNo, s32 motNo)
{
    Hu3DMotionSet(lbl_1_bss_10.nejiModel[playerNo], lbl_1_bss_10.nejiMot[motNo]);
}

void fn_1_215C(s32 playerNo, s32 motNo)
{
    if (motNo == 12 || lbl_1_bss_10.daiMot[playerNo][motNo] != Hu3DMotionIDGet(lbl_1_bss_10.daiModel[playerNo])) {
        Hu3DMotionSet(lbl_1_bss_10.daiModel[playerNo], lbl_1_bss_10.daiMot[playerNo][motNo]);
        Hu3DModelAttrSet(lbl_1_bss_10.daiModel[playerNo], HU3D_MOTATTR_LOOP);
        OSReport("base motion time ... %f\n", Hu3DMotionMaxTimeGet(lbl_1_bss_10.daiModel[playerNo]));
    }
}

void fn_1_2254(s32 motNo)
{
    Hu3DMotionSet(lbl_1_bss_10.gearModel, lbl_1_bss_10.gearMot[motNo]);
    OSReport("gear motion time ... %f\n", Hu3DMotionMaxTimeGet(lbl_1_bss_10.gearModel));
}

void fn_1_22BC(s32 playerNo, s32 motNo, u32 attr)
{
    float start = 0.0f;
    HU3D_MOTIONID motId = Hu3DMotionIDGet(lbl_1_bss_10.charModel[playerNo]);
    if (motNo == 10 || motId != lbl_1_bss_10.charMot[playerNo][motNo]) {
        CharMotionShiftSet(lbl_1_bss_10.charNo[playerNo], lbl_1_bss_10.charMot[playerNo][motNo], start, 8.0f, attr);
    }
}
