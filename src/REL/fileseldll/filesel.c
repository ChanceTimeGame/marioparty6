/* @@HEAD@@ */
#include "REL/fileseldll.h"
#include "game/audio.h"
#include "game/card.h"
#include "game/data.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/memory.h"
#include "game/mic.h"
#include "game/pad.h"
#include "game/saveload.h"
#include "game/wipe.h"
#include "humath.h"
#include "string.h"
#include "stdio.h"
#include "msm.h"

#include "datadir_enum.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

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

/* .bss is laid out in reverse definition order */
OMOBJ *lbl_1_bss_344;
OMOBJMAN *lbl_1_bss_340;
u8 lbl_1_bss_308[0x38];
HuVecF lbl_1_bss_2E4[3];
HUWINID lbl_1_bss_2E0;
HUWINID lbl_1_bss_2DE;
HU3D_MODELID lbl_1_bss_2D8[3];
FILESEL_BOX lbl_1_bss_D8[4];
ANIMDATA *lbl_1_bss_CC[3];
ANIMDATA *lbl_1_bss_C8;
ANIMDATA *lbl_1_bss_A8[8];
HU3D_ANIMID lbl_1_bss_A2[3];
HU3D_PARMANID lbl_1_bss_98[5];
HuVecF lbl_1_bss_68[4];
FILESEL_WAVE lbl_1_bss_18[2];
ANIMDATA *lbl_1_bss_14;
ANIMDATA *lbl_1_bss_10;
void *lbl_1_bss_C;
OMOBJ *lbl_1_bss_8;

OM_CAMERA_VIEW lbl_1_data_0 = {
    { 0, 250, 0 },
    { 0, 0, 0 },
    7000
};

OM_CAMERA_VIEW lbl_1_data_1C = {
    { 0, -4, 0 },
    { 0, 0, 0 },
    2800
};

char *lbl_1_data_88[4] = {
    "book_012-itemhook01",
    "book_013-itemhook02",
    "book_014-itemhook03",
    "book_015-itemhook04"
};

s32 lbl_1_data_98[8] = {
    DATANUM(DATA_filesel, 12),
    DATANUM(DATA_filesel, 13),
    DATANUM(DATA_filesel, 14),
    DATANUM(DATA_filesel, 15),
    DATANUM(DATA_filesel, 16),
    DATANUM(DATA_filesel, 17),
    DATANUM(DATA_filesel, 18),
    DATANUM(DATA_filesel, 19)
};

/* @@PART1@@ */
void ObjectSetup(void)
{
    int lightId;
    OSReport("******* FileSelect ObjectSetup *********\n");
    lbl_1_bss_340 = omInitObjMan(50, 512);
    omGameSysInit(lbl_1_bss_340);
    GWLanguageSet(GwLanguage);
    lightId = Hu3DGLightCreate(-1000, 1500, 1000, 1, -1.5f, -1, 255, 255, 255);
    Hu3DGLightInfinitytSet(lightId);
    lbl_1_bss_344 = omAddObjEx(lbl_1_bss_340, 32730, 0, 0, -1, omOutViewMulti);
    lbl_1_bss_344->work[0] = 2;
    Hu3DCameraCreate(HU3D_CAM0|HU3D_CAM1);
    Hu3DCameraPerspectiveSet(HU3D_CAM0, 5, 20, 15000, 1.2f);
    Hu3DCameraViewportSet(HU3D_CAM0, 0, 0, 640, 480, 0, 1);
    Hu3DCameraPerspectiveSet(HU3D_CAM1, 10, 20, 15000, 1.2f);
    Hu3DCameraViewportSet(HU3D_CAM1, 0, 0, 640, 480, 0, 1);
    omCameraViewSetMulti(HU3D_CAM0, &lbl_1_data_0);
    omCameraViewSetMulti(HU3D_CAM1, &lbl_1_data_1C);
    HuPrcChildCreate(FileselMain, 256, 12288, 0, HuPrcCurrentGet());
    Hu3DBGColorSet(0, 0, 0);
    HuWinInit(1);
    HuWinMesLanguageSet(GwLanguage);
    HuWinMesRead();
    HuAudSStreamPlay(2);
    OSReport("%d\n", 3250);
}

void FileselMain(void)
{
    s16 i;
    s32 result;
    BOOL ovlReturnF;
    ovlReturnF = TRUE;
    FileselInit();
    FileselBoxInit();
    SLSaveFlagSet(TRUE);
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 60);
    WipeWait();
    result = FileBoxInit(-1);
    if(result == -2) {
        goto end;
    }
    if(result == -1) {
        HuAudFXPlay(1167);
        FileselBookOpen(FALSE);
        GwCommon = GwCommonOrig;
        FileselDecideExec(0, 0);
        SLSaveFlagSet(FALSE);
        ovlReturnF = FALSE;
        goto end;
    }
    HuAudFXPlay(1167);
    FileselBookOpen(TRUE);
    while(1) {
        result = FileselSelect();
        if(result == FILESEL_RESULT_CANCEL) {
            goto end;
        }
        if(result != -3) {
            break;
        }
        result = FileBoxInit(-1);
        if(result == -2) {
            goto end;
        }
        if(result == -1) {
            GwCommon = GwCommonOrig;
            FileselDecideExec(0, 0);
            SLSaveFlagSet(FALSE);
            goto noReturn;
        }
        for(i=0; i<3; i++) {
            FileselSprUpdate(i, i);
        }
    }
    if(result == -1) {
        GwCommon = GwCommonOrig;
        FileselDecideExec(0, 0);
        SLSaveFlagSet(FALSE);
        ovlReturnF = FALSE;
        goto end;
    }
    SLSaveFlagSet(TRUE);
    FileselDecideExec(result, TRUE);
    noReturn:
    ovlReturnF = FALSE;
    end:
    HuDataDirClose(DATA_win);
    HuDataDirClose(DATA_effect);
    HuDataDirClose(DATA_filesel);
    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
    HuAudSStreamAllFadeOut(1000);
    WipeWait();
    if(!SLSaveFlagGet()) {
        FileselMicInit();
    }
    GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
    for(i=0; i<4; i++) {
        HuMemDirectFree(lbl_1_bss_D8[i].textAnim);
    }
    if(ovlReturnF) {
        omOvlReturnEx(1, 1);
    } else {
        if(OSGetSoundMode() == OS_SOUND_MODE_MONO) {
            msmSysSetOutputMode(SND_OUTPUTMODE_MONO);
        } else if(GwCommon.outputMode == 2) {
            msmSysSetOutputMode(SND_OUTPUTMODE_SURROUND);
        } else {
            msmSysSetOutputMode(SND_OUTPUTMODE_STEREO);
        }
        omOvlCallEx(93, 1, 0, 0);
    }
    while(1) {
        HuPrcVSleep();
    }
}

void FileselMicInit(void)
{
    if(HuMCProbe(1) != 0) {
        HuMCMicSet(FALSE);
    } else {
        HuMCMicSet(TRUE);
        HuMCInit(0);
        if(HuMCMount(1) != 0) {
            HuMCMicSet(FALSE);
        }
        HuMCClose();
    }
    HuMCMicGet();
}

HU3D_PARMAN_PARAM lbl_1_data_E8 = {
    50, 0,
    1.0f, 10.0f, 0.0f,
    { 0.0f, -0.02f, 0.0f },
    2.0f, 1.0f, 20.0f, 0.98f,
    2,
    {
        { 240, 224, 32, 255 },
        { 255, 255, 64, 255 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    },
    {
        { 192, 128, 16, 0 },
        { 192, 64, 16, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    }
};

void FileselInit(void)
{
    HuVecF pos;
    HuVecF target;
    HuVecF up;
    ANIMDATA *anim;
    s16 i;
    Hu3DShadowCreate(15, 20, 5000);
    pos.x = 0;
    pos.y = 1000;
    pos.z = 2000;
    up.y = 1;
    up.x = up.z = 0;
    target.x = 0;
    target.y = 50;
    target.z = 0;
    Hu3DShadowPosSet(&pos, &up, &target);
    Hu3DShadowTPLvlSet(0);
    lbl_1_bss_2D8[1] = Hu3DModelCreateData(DATANUM(DATA_filesel, 0));
    Hu3DModelCameraSet(lbl_1_bss_2D8[1], HU3D_CAM1);
    Hu3DModelLayerSet(lbl_1_bss_2D8[1], 1);
    Hu3DMotionSpeedSet(lbl_1_bss_2D8[1], 0.05f);
    Hu3DModelPosSet(lbl_1_bss_2D8[1], 0, 0, -9000);
    Hu3DModelAttrSet(lbl_1_bss_2D8[1], HU3D_MOTATTR_LOOP);
    lbl_1_bss_2D8[0] = Hu3DModelCreateData(DATANUM(DATA_filesel, 1));
    Hu3DModelCameraSet(lbl_1_bss_2D8[0], HU3D_CAM1);
    Hu3DModelLayerSet(lbl_1_bss_2D8[0], 1);
    Hu3DModelAttrSet(lbl_1_bss_2D8[0], 0x40000002);
    Hu3DModelClusterAttrSet(lbl_1_bss_2D8[0], 0, 0xC0000002);
    Hu3DModelPosSet(lbl_1_bss_2D8[0], 0, 0, -2000);
    Hu3DModelRotSet(lbl_1_bss_2D8[0], -20, -30, 30);
    Hu3DModelShadowMapSet(lbl_1_bss_2D8[0]);
    lbl_1_bss_2D8[2] = Hu3DModelCreateData(DATANUM(DATA_filesel, 4));
    Hu3DModelCameraSet(lbl_1_bss_2D8[2], HU3D_CAM1);
    Hu3DModelLayerSet(lbl_1_bss_2D8[2], 1);
    Hu3DModelAttrSet(lbl_1_bss_2D8[2], HU3D_ATTR_DISPOFF);
    Hu3DModelShadowSet(lbl_1_bss_2D8[2]);
    lbl_1_bss_C8 = HuSprAnimDataRead(DATANUM(DATA_filesel, 29));
    lbl_1_bss_A2[0] = Hu3DAnimCreate(lbl_1_bss_C8, lbl_1_bss_2D8[2], "data_select");
    Hu3DAnimBankSet(lbl_1_bss_A2[0], 1);
    lbl_1_bss_A2[1] = Hu3DAnimCreate(lbl_1_bss_C8, lbl_1_bss_2D8[2], "data_select_ura");
    Hu3DAnimBankSet(lbl_1_bss_A2[1], 0);
    HuPrcChildCreate(FileselBookMoveExec, 256, 4096, 0, HuPrcCurrentGet());
    NameEnterInit();
    Hu3DCameraLayerHookSet(HU3D_CAM0, 0, FileselToueiHook);
    Hu3DCameraLayerHookSet(HU3D_CAM1, 0, FileselPixelFmtHook);
    omAddObjEx(lbl_1_bss_340, 4096, 16, 16, -1, FileselWaveCreate);
    anim = HuSprAnimDataRead(DATANUM(DATA_effect, 0));
    for(i=0; i<5; i++) {
        lbl_1_bss_98[i] = Hu3DParManCreate(anim, 100, &lbl_1_data_E8);
        Hu3DParManAttrSet(lbl_1_bss_98[i], 100);
        Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(lbl_1_bss_98[i]), 1);
        Hu3DParManRotSet(lbl_1_bss_98[i], 90, 0, 0);
        Hu3DModelCameraSet(Hu3DParManModelIDGet(lbl_1_bss_98[i]), HU3D_CAM1);
        Hu3DModelLayerSet(Hu3DParManModelIDGet(lbl_1_bss_98[i]), 4);
        Hu3DParManAttrSet(lbl_1_bss_98[i], 1);
    }
}

void FileselToueiHook(s16 layerNo)
{
    GXSetPixelFmt(GX_PF_RGBA6_Z24, GX_ZC_LINEAR);
    FileselZClear();
}

void FileselPixelFmtHook(s16 layerNo)
{
    GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
}

HuVecF lbl_1_data_154[3] = {
    { -130, 120, 30 },
    { 20, 10, 60 },
    { 170, 90, 30 }
};

void FileselBookOpen(BOOL boxF)
{
    Mtx mtx;
    HuVecF rot;
    HuVecF pos;
    HuVecF trans;
    HuVecF objRot;
    float t;
    float time;
    float maxTime;
    float t2;
    s16 i;
    FILESEL_BOX *box;
    lbl_1_bss_2E0 = HuWinExCreate(16, 360, 544, 80, -1);
    HuWinAttrSet(lbl_1_bss_2E0, 0x800);
    HuWinPadMaskSet(lbl_1_bss_2E0, 1);
    for(i=0; i<3; i++) {
        FileselSprUpdate(i, i);
        FileselBoxSwayModeSet(i, 2);
    }
    maxTime = 10.0f+Hu3DMotionMaxTimeGet(lbl_1_bss_D8[i].mdlId);
    rot.x = -20;
    rot.y = -30;
    rot.z = 30;
    pos.x = 0;
    pos.y = 0;
    pos.z = -2000;
    HuPrcSleep(10);
    for(time=0; time<maxTime; time += 1.0f) {
        HuPrcVSleep();
        if(time == 10.0f) {
            Hu3DModelAttrReset(lbl_1_bss_2D8[0], 0x40000002);
            Hu3DModelClusterAttrReset(lbl_1_bss_2D8[0], 0, 0xC0000002);
        }
        t = HuSin(90.0*((1.0+time)/maxTime));
        if(time < 0.7*maxTime) {
            t2 = HuSin(90.0*((1.0+time)/(0.7*maxTime)));
        } else {
            t2 = 1.0f;
        }
        Hu3DModelPosSet(lbl_1_bss_2D8[0], pos.x+(-pos.x*t), pos.y+((50-pos.y)*t), pos.z+(-pos.z*t));
        Hu3DModelRotSet(lbl_1_bss_2D8[0], rot.x+((340-rot.x)*t), rot.y+((360-rot.y)*t2), rot.z+(-rot.z*t2));
        if(boxF) {
            if(time >= 70) {
                if(time == 70) {
                    Hu3DModelAttrReset(lbl_1_bss_2D8[2], HU3D_ATTR_DISPOFF);
                    for(i=0; i<3; i++) {
                        Hu3DModelAttrReset(lbl_1_bss_D8[i].mdlId, HU3D_ATTR_DISPOFF);
                    }
                }
                t = HuSin(90.0*((time-70)/(maxTime-70)));
                Hu3DModelObjMtxGet(lbl_1_bss_2D8[0], lbl_1_data_88[0], mtx);
                Hu3DMtxTransGet(mtx, &trans);
                Hu3DMtxRotGet(mtx, &objRot);
                trans.x += -260*t;
                trans.y += -50*t;
                trans.z += 500.0*HuSin(180.0*((time-70)/(maxTime-70)));
                Hu3DModelPosSetV(lbl_1_bss_2D8[2], &trans);
                objRot.x *= 1.0-t;
                objRot.y *= 1.0-t;
                objRot.z *= 1.0-t;
                Hu3DModelRotSetV(lbl_1_bss_2D8[2], &objRot);
                Hu3DModelScaleSet(lbl_1_bss_2D8[2], 1.2f, 1.2f, 1.2f);
                for(i=0; i<3; i++) {
                    box = &lbl_1_bss_D8[i];
                    Hu3DModelObjMtxGet(lbl_1_bss_2D8[0], lbl_1_data_88[i+1], mtx);
                    Hu3DMtxTransGet(mtx, &trans);
                    Hu3DMtxRotGet(mtx, &objRot);
                    trans.x += t*(lbl_1_data_154[i].x-trans.x);
                    trans.y += t*(lbl_1_data_154[i].y-trans.y);
                    trans.z += t*(lbl_1_data_154[i].z-trans.z);
                    lbl_1_bss_2E4[i] = trans;
                    FileselBoxPosSetV(i, &trans);
                    objRot.x *= 1.0-t;
                    objRot.y *= 1.0-t;
                    objRot.z *= 1.0-t;
                    Hu3DModelRotSetV(box->mdlId, &objRot);
                    Hu3DModelScaleSet(box->mdlId, 1.5f, 1.5f, 1.5f);
                }
            }
            if(time > maxTime-30) {
                t = (time-(maxTime-30))/30;
                Hu3DShadowTPLvlSet(0.5f*t);
            }
            if(time == maxTime-10) {
                FileselPrcCreate(FileselInfoWinOpen, 0);
            }
        }
    }
    if(boxF) {
        lbl_1_bss_2DE = HuWinCreate(16, 326, 544, 42, 0);
        HuWinAttrSet(lbl_1_bss_2DE, 0x800);
        HuWinPriSet(lbl_1_bss_2DE, 100);
        HuWinBGTPLvlSet(lbl_1_bss_2DE, 0);
        HuWinMesSpeedSet(lbl_1_bss_2DE, 0);
    }
}

void FileselBookMoveExec(void)
{
    Mtx mtx;
    float rotA;
    float rotB;
    rotA = 0;
    rotB = 45;
    while(1) {
        rotA += 2.0f;
        if(rotA > 360) {
            rotA -= 360;
        }
        rotB += 1.1;
        if(rotB > 360) {
            rotB -= 360;
        }
        PSMTXTrans(mtx, 0, 3.0*HuSin(rotA), 0);
        mtxRotCat(mtx, 0, 0, HuSin(rotB));
        Hu3DModelMtxSet(lbl_1_bss_2D8[0], &mtx);
        PSMTXTrans(mtx, HuSin(rotA), 3.0*HuSin(rotB), 0);
        Hu3DModelMtxSet(lbl_1_bss_2D8[2], &mtx);
        HuPrcVSleep();
    }
}

void FileselInfoWinOpen(void)
{
    HuWinExOpen(lbl_1_bss_2E0);
    HuPrcEnd();
}

s32 FileselSelect(void)
{
    OSTime time;
    HuVecF scale;
    HuVecF pos[3];
    float t;
    float sinMax;
    s16 boxNo;
    s16 i;
    s16 j;
    FILESEL_BOX *box;
    s32 choice;
    s16 dir;
    s16 validNum;
    s32 seNo;

    boxNo = 0;
    for(i=0; i<3; i++) {
        pos[i] = lbl_1_bss_2E4[i];
    }
    Hu3DModelScaleGet(lbl_1_bss_D8[0].mdlId, &scale);
    if(scale.x < 0.01f) {
        for(i=1; i<=5; i++) {
            t = i/5.0f;
            for(j=0; j<3; j++) {
                box = &lbl_1_bss_D8[j];
                Hu3DModelScaleSet(box->mdlId, 1.5f*t, 1.5f*t, 1.5f*t);
            }
            HuPrcVSleep();
        }
    }
    repeat:
    for(i=validNum=0; i<3; i++) {
        if(lbl_1_bss_D8[i].validF) {
            validNum++;
        }
        if(boxNo == i) {
            FileselBoxSwayModeSet(i, 0);
        } else {
            FileselBoxSwayModeSet(i, 1);
        }
    }
    Hu3DModelScaleGet(lbl_1_bss_D8[boxNo].mdlId, &scale);
    if(scale.x < 2.0f) {
        for(i=1; i<=10; i++) {
            box = &lbl_1_bss_D8[boxNo];
            t = i/10.0f;
            FileselBoxPosSet(boxNo, pos[boxNo].x, pos[boxNo].y, (20*t)+pos[boxNo].z);
            t = HuSin(130*t)*(1.0/HuSin(130));
            Hu3DModelScaleSet(box->mdlId, 1.5+(0.5*t), 1.5+(0.5*t), 1.5+(0.5*t));
            if(i == 5) {
                box->glowNo = 1;
            }
            HuPrcVSleep();
        }
    }
    HuWinMesSet(lbl_1_bss_2E0, 0x90008);
    HuWinMesWait(lbl_1_bss_2E0);
    HuWinMesSet(lbl_1_bss_2DE, 0x10004);
    HuWinDispOn(lbl_1_bss_2DE);
    while(1) {
        HuPrcVSleep();
        if(UnMountCnt) {
            HuWinMesSet(lbl_1_bss_2E0, 0x90026);
            HuWinMesWait(lbl_1_bss_2E0);
            UnMountCnt = 0;
            goto cancel;
        }
        dir = 0;
        if(HuPadBtnDown[0] & (PAD_BUTTON_A|PAD_BUTTON_START)) {
            HuAudFXPlay(1);
            HuWinDispOff(lbl_1_bss_2DE);
            SLCurBoxNoSet(boxNo);
            if(lbl_1_bss_D8[boxNo].validF) {
                if(validNum >= 3) {
                    HuWinMesSet(lbl_1_bss_2E0, 0x9003C);
                } else {
                    HuWinMesSet(lbl_1_bss_2E0, 0x9003D);
                }
                HuWinMesWait(lbl_1_bss_2E0);
                choice = HuWinChoiceGet(lbl_1_bss_2E0, -1);
                if(UnMountCnt) {
                    HuWinMesSet(lbl_1_bss_2E0, 0x90026);
                    HuWinMesWait(lbl_1_bss_2E0);
                    UnMountCnt = 0;
                    goto cancel;
                }
                if(choice == -1) {
                    goto repeat;
                }
                if(choice == 0) {
                    SLCommonLoad();
                    goto select;
                }
                if((validNum >= 3 && choice == 1) || (validNum < 3 && choice == 2)) {
                    lbl_1_bss_D8[boxNo].glowNo = 2;
                    choice = FileselEraseConfirm(boxNo);
                    lbl_1_bss_D8[boxNo].glowNo = 1;
                    if(choice == FILESEL_RESULT_CANCEL || choice == -4 || choice == -5) {
                        goto cancel;
                    }
                    if(choice == -1) {
                        goto fail;
                    }
                    goto repeat;
                } else {
                    lbl_1_bss_D8[boxNo].glowNo = 3;
                    FileselBoxCopyFocusSet(boxNo, 1);
                    choice = FileselCopy(boxNo);
                    FileselBoxCopyFocusSet(boxNo, 0);
                    lbl_1_bss_D8[boxNo].glowNo = 1;
                    if(choice == -4 || choice == -5) {
                        goto cancel;
                    }
                    if(choice == -1) {
                        goto fail;
                    }
                    goto repeat;
                }
            } else {
                UnMountCnt = 0;
                FileselCommonReset();
                FileselBoxHide(-1);
                choice = NameEnterMain(boxNo);
                FileselBoxShow();
                if(choice == FILESEL_RESULT_CANCEL) {
                    goto repeat;
                }
                if(UnMountCnt) {
                    HuWinMesSet(lbl_1_bss_2E0, 0x90026);
                    HuWinMesWait(lbl_1_bss_2E0);
                    UnMountCnt = 0;
                    goto cancel;
                }
                if(!FileTestOpen()) {
                    time = OSGetTime();
                    SLSaveDataMake(TRUE, &time);
                    SLCommonSet();
                    for(i=0; i<3; i++) {
                        if(i != boxNo) {
                            SLSaveEmptySet(curSlotNo, i);
                        }
                    }
                    seNo = HuAudFXPlay(1165);
                    choice = FileClear(-1);
                    HuAudFXStop(seNo);
                    if(choice == FILESEL_RESULT_CANCEL || choice == -4 || choice == -5) {
                        goto cancel;
                    }
                    if(choice == -1) {
                        goto fail;
                    }
                } else {
                    time = OSGetTime();
                    SLSaveDataMake(FALSE, &time);
                    SLCommonSet();
                    seNo = HuAudFXPlay(1165);
                    choice = FileSaveMesOpen(-1, 0x9002A);
                    HuAudFXStop(seNo);
                    if(choice == FILESEL_RESULT_CANCEL || choice == -4 || choice == -5) {
                        goto cancel;
                    }
                    if(choice == -1) {
                        goto fail;
                    }
                }
                lbl_1_bss_D8[boxNo].validF = TRUE;
                lbl_1_bss_D8[boxNo].charNo = -1;
                lbl_1_bss_D8[boxNo].playNum = 0;
                OSTicksToCalendarTime(GwCommon.time, &lbl_1_bss_D8[boxNo].time);
                FileselSprUpdate(boxNo, boxNo);
                goto select;
            }
        } else if(HuPadBtnDown[0] & PAD_BUTTON_B) {
            HuAudFXPlay(3);
            boxNo = FILESEL_RESULT_CANCEL;
            goto select;
        }
        if(HuPadDStkRep[0] & PAD_BUTTON_LEFT) {
            dir = -1;
        }
        if(HuPadDStkRep[0] & PAD_BUTTON_RIGHT) {
            dir = 1;
        }
        if(dir != 0 && boxNo+dir >= 0 && boxNo+dir < 3) {
            HuAudFXPlay(1155);
            FileselBoxSwayModeSet(boxNo+dir, 0);
            FileselBoxSwayModeSet(boxNo, 1);
            for(i=1; i<=10; i++) {
                t = i/10.0f;
                t = HuSin(130*t)*(1.0/HuSin(130));
                box = &lbl_1_bss_D8[boxNo+dir];
                Hu3DModelScaleSet(box->mdlId, 1.5+(0.5*t), 1.5+(0.5*t), 1.5+(0.5*t));
                FileselBoxPosSet(boxNo+dir, pos[boxNo+dir].x, pos[boxNo+dir].y, (20*t)+pos[boxNo+dir].z);
                t = 1.0-t;
                box = &lbl_1_bss_D8[boxNo];
                Hu3DModelScaleSet(box->mdlId, 1.5+(0.5*t), 1.5+(0.5*t), 1.5+(0.5*t));
                FileselBoxPosSet(boxNo, pos[boxNo].x, pos[boxNo].y, (20*t)+pos[boxNo].z);
                if(i == 5) {
                    lbl_1_bss_D8[boxNo+dir].glowNo = 1;
                    lbl_1_bss_D8[boxNo].glowNo = 0;
                }
                HuPrcVSleep();
            }
            boxNo += dir;
        }
    }
    select:
    return boxNo;
    cancel:
    HuWinHomeClear(lbl_1_bss_2E0);
    HuWinDispOff(lbl_1_bss_2DE);
    for(i=1; i<=10; i++) {
        t = HuCos(90.0*(i/10.0));
        for(j=0; j<3; j++) {
            box = &lbl_1_bss_D8[j];
            Hu3DModelScaleSet(box->mdlId, t, t, t);
        }
        HuPrcVSleep();
    }
    for(i=0; i<3; i++) {
        lbl_1_bss_D8[i].glowNo = 0;
    }
    HuPrcSleep(30);
    return -3;
    fail:
    HuWinHomeClear(lbl_1_bss_2E0);
    for(i=1; i<=10; i++) {
        t = HuCos(90.0*(i/10.0));
        for(j=0; j<3; j++) {
            box = &lbl_1_bss_D8[j];
            Hu3DModelScaleSet(box->mdlId, t, t, t);
        }
        HuPrcVSleep();
    }
    return -1;
}

void FileselBoxHide(s16 boxNo)
{
    float t;
    s16 i;
    for(i=0; i<3; i++) {
        Hu3DModelPosGet(lbl_1_bss_D8[i].mdlId, &lbl_1_bss_68[i]);
    }
    Hu3DModelPosGet(lbl_1_bss_2D8[2], &lbl_1_bss_68[i]);
    if(boxNo == -1) {
        for(i=1; i<=10; i++) {
            t = 1.0-HuCos(90.0*(i/10.0));
            FileselBoxPosSet(0, lbl_1_bss_68[0].x, lbl_1_bss_68[0].y+(350*t), lbl_1_bss_68[0].z);
            FileselBoxPosSet(1, lbl_1_bss_68[1].x, lbl_1_bss_68[1].y+(350*t), lbl_1_bss_68[1].z);
            FileselBoxPosSet(2, lbl_1_bss_68[2].x, lbl_1_bss_68[2].y+(350*t), lbl_1_bss_68[2].z);
            Hu3DModelPosSet(lbl_1_bss_2D8[2], lbl_1_bss_68[3].x-(200*t), lbl_1_bss_68[3].y, lbl_1_bss_68[3].z);
            HuPrcVSleep();
        }
    } else {
        for(i=1; i<=10; i++) {
            t = 1.0-HuCos(90.0*(i/10.0));
            switch(boxNo) {
                case 0:
                    FileselBoxPosSet(0, lbl_1_bss_68[0].x, lbl_1_bss_68[0].y+(300*t), lbl_1_bss_68[0].z);
                    break;
                case 1:
                    FileselBoxPosSet(1, lbl_1_bss_68[1].x, lbl_1_bss_68[1].y-(400*t), lbl_1_bss_68[1].z);
                    break;
                case 2:
                    FileselBoxPosSet(2, lbl_1_bss_68[2].x, lbl_1_bss_68[2].y+(300*t), lbl_1_bss_68[2].z);
                    break;
            }
            HuPrcVSleep();
        }
    }
}

void FileselBoxShow(void)
{
    HuVecF pos;
    HuVecF ofs[4];
    HuVecF newPos;
    float t;
    s16 i;
    s16 j;
    for(i=0; i<3; i++) {
        Hu3DModelPosGet(lbl_1_bss_D8[i].mdlId, &pos);
        PSVECSubtract(&pos, &lbl_1_bss_68[i], &ofs[i]);
    }
    Hu3DModelPosGet(lbl_1_bss_2D8[2], &pos);
    PSVECSubtract(&pos, &lbl_1_bss_68[i], &ofs[i]);
    for(i=1; i<=10; i++) {
        t = 1.0-HuSin(90.0*(i/10.0));
        for(j=0; j<3; j++) {
            PSVECScale(&ofs[j], &pos, t);
            PSVECAdd(&lbl_1_bss_68[j], &pos, &newPos);
            FileselBoxPosSetV(j, &newPos);
        }
        PSVECScale(&ofs[j], &pos, t);
        PSVECAdd(&lbl_1_bss_68[j], &pos, &newPos);
        Hu3DModelPosSetV(lbl_1_bss_2D8[2], &newPos);
        HuPrcVSleep();
    }
}

char *lbl_1_data_190[4] = {
    "card1",
    "card2",
    "card3",
    "card4"
};

/* @@PART2@@ */
/* @@PART3@@ */
/* @@END@@ */
