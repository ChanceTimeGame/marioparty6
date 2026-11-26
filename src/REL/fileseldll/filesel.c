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
/* Retail calls SLCurBoxNoSet unprototyped (extsh on its argument) */
#define SLCurBoxNoSet SLCurBoxNoSet_proto
#include "game/saveload.h"
#undef SLCurBoxNoSet
int SLCurBoxNoSet();
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
u32 lbl_1_bss_4; /* unreferenced; retail .bss has 4 bytes here */

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
                lbl_1_bss_D8[boxNo].lastBoard = -1;
                lbl_1_bss_D8[boxNo].bankStar = 0;
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
void FileselSprUpdate(s16 boxNo, s16 no);
void FileselBoxUpdate(void);
void FileselFbCopyHook(s16 layerNo);

void FileselBoxInit(void)
{
    HUSPR_GROUPID grpId;
    s16 i;
    s16 j;
    HUSPRID sprId;
    HUWINID winId;
    ANIMDATA *slashAnim;
    ANIMDATA *anim;
    ANIMDATA *numAnim;
    HSF_DATA *hsf;
    HSF_OBJECT *obj;
    FILESEL_BOX *box;

    HuSprExecLayerCameraSet(64, 2, 1);
    HuSprExecLayerCameraSet(120, 1, 8);
    anim = HuSprAnimDataRead(0x230008);
    grpId = HuSprGrpCreate(1);
    sprId = HuSprCreate(anim, 0, 0);
    HuSprGrpMemberSet(grpId, 0, sprId);
    HuSprGrpDrawNoSet(grpId, 64);
    HuSprGrpPosSet(grpId, 288.0f, 240.0f);
    HuSprGrpScaleSet(grpId, 20.0f, 20.0f);
    numAnim = HuSprAnimDataRead(0x230007);
    slashAnim = HuSprAnimDataRead(0x230006);
    lbl_1_bss_CC[0] = HuSprAnimDataRead(0x230009);
    lbl_1_bss_CC[1] = HuSprAnimDataRead(0x23000B);
    lbl_1_bss_CC[2] = HuSprAnimDataRead(0x23000A);
    for (i = 0; i < 8; i++) {
        lbl_1_bss_A8[i] = HuSprAnimDataRead(lbl_1_data_98[i]);
    }
    for (i = 0; i < 4; i++) {
        box = &lbl_1_bss_D8[i];
        winId = HuWinCreate(288.0 * (i & 1), 168.0 * (i / 2), 192, 112, 0);
        box->winId = winId;
        box->swayOfs.x = box->swayOfs.y = box->swayOfs.z = 0.0f;
        HuWinScaleSet(winId, 1.5f, 1.5f);
        HuWinBGTPLvlSet(winId, 0.0f);
        HuWinAttrSet(winId, 0x801);
        box->textAnim = HuSprAnimMake(320, 168, 2);
        box->textBuf = box->textAnim->bmp->data = HuMemDirectMallocNum(HEAP_HEAP, GXGetTexBufferSize(320, 168, GX_TF_RGB5A3, GX_FALSE, 0), HU_MEMNUM_OVL);
        HuWinDrawNoSet(winId, 120);
        HuWinMesSpeedSet(winId, 0);
        HuWinMesSet(winId, 0x4A0009);
        grpId = HuSprGrpCreate(17);
        box->grpId = grpId;
        HuSprGrpPosSet(grpId, 288.0 * (i & 1), 168.0 * (i / 2));
        HuSprGrpScaleSet(grpId, 1.5f, 1.5f);
        for (j = 0; j < 10; j++) {
            sprId = HuSprCreate(numAnim, 95, 0);
            HuSprGrpMemberSet(grpId, j, sprId);
            HuSprPosSet(grpId, j, (j * 12) + 42, 70.0f);
            HuSprAttrReset(grpId, j, HUSPR_ATTR_LINEAR);
        }
        sprId = HuSprCreate(slashAnim, 95, 0);
        HuSprGrpMemberSet(grpId, 10, sprId);
        HuSprPosSet(grpId, 10, 70.0f, 96.0f);
        HuSprAttrReset(grpId, 10, HUSPR_ATTR_LINEAR);
        for (j = 0; j < 5; j++) {
            sprId = HuSprCreate(numAnim, 95, 0);
            HuSprGrpMemberSet(grpId, j + 12, sprId);
            HuSprPosSet(grpId, j + 12, (j * 12) + 88, 96.0f);
            HuSprAttrReset(grpId, j + 12, HUSPR_ATTR_LINEAR);
        }
        HuSprGrpDrawNoSet(grpId, 120);
        box->mdlId = Hu3DModelCreateData(0x230002);
        Hu3DModelCameraSet(box->mdlId, HU3D_CAM1);
        Hu3DModelLayerSet(box->mdlId, 1);
        Hu3DModelAttrSet(box->mdlId, 0x40000001);
        j = Hu3DMotionMaxTimeGet(box->mdlId);
        Hu3DMotionTimeSet(box->mdlId, frandmod(j));
        box->frameAnimId = Hu3DAnimCreate(lbl_1_bss_CC[0], box->mdlId, "siori_waku01");
        box->charAnimId = Hu3DAnimCreate(lbl_1_bss_A8[0], box->mdlId, "siori_moyou01");
        Hu3DModelShadowSet(box->mdlId);
        box->textMdlId = Hu3DModelCreateData(0x230003);
        Hu3DModelCameraSet(box->textMdlId, HU3D_CAM1);
        Hu3DModelLayerSet(box->textMdlId, 2);
        Hu3DAnimCreate(box->textAnim, box->textMdlId, "siori_moji01");
        box->proc = HuPrcChildCreate(FileselBoxUpdate, 16, 0x3000, 0, HuPrcCurrentGet());
        obj = Hu3DModelObjPtrGet(box->mdlId, "siori_fix-itemhook_a");
        strcpy(obj->name, lbl_1_data_190[i]);
        box->glowMdlId[0] = Hu3DModelCreateData(0x23001A);
        Hu3DModelCameraSet(box->glowMdlId[0], HU3D_CAM1);
        Hu3DModelLayerSet(box->glowMdlId[0], 2);
        Hu3DModelAttrSet(box->glowMdlId[0], HU3D_ATTR_DISPOFF);
        box->glowMdlId[1] = Hu3DModelCreateData(0x23001B);
        Hu3DModelCameraSet(box->glowMdlId[1], HU3D_CAM1);
        Hu3DModelLayerSet(box->glowMdlId[1], 2);
        Hu3DModelAttrSet(box->glowMdlId[1], HU3D_ATTR_DISPOFF);
        box->glowMdlId[2] = Hu3DModelCreateData(0x23001B);
        Hu3DModelCameraSet(box->glowMdlId[2], HU3D_CAM1);
        Hu3DModelLayerSet(box->glowMdlId[2], 2);
        Hu3DModelAttrSet(box->glowMdlId[2], HU3D_ATTR_DISPOFF);
        hsf = Hu3DData[box->glowMdlId[2]].hsf;
        for (j = 0; j < hsf->materialNum; j++) {
            hsf->material[j].litColor[0] = hsf->material[j].color[0] = 241;
            hsf->material[j].litColor[1] = hsf->material[j].color[1] = 123;
            hsf->material[j].litColor[2] = hsf->material[j].color[2] = 18;
        }
        box->glowNo = 0;
        Hu3DModelAttrSet(box->mdlId, HU3D_ATTR_DISPOFF);
        box->validF = TRUE;
        strcpy(box->name, "ABCDEFGH");
        OSTicksToCalendarTime(OSGetTime(), &box->time);
        box->bankStar = 10;
        FileselSprUpdate(i, i % 3);
    }
    Hu3DCameraLayerHookSet(1, 1, FileselFbCopyHook);
}

void FileselFbCopyHook(s16 layerNo)
{
    s16 i;
    FILESEL_BOX *box;
    for (i = 0; i < 4; i++) {
        box = &lbl_1_bss_D8[i];
        Hu3DFbCopyExec((1.0f / 0.9f) * (288.0 * (i & 1)), 168.0 * (i / 2), 320, 168, GX_TF_RGB5A3, 0, box->textBuf);
    }
}

void FileselSprUpdate(s16 boxNo, s16 no)
{
    FILESEL_BOX *box = &lbl_1_bss_D8[boxNo];
    HUSPR_GROUPID grpId = box->grpId;
    OSCalendarTime *time = &box->time;
    u8 i;
    u8 len;
    char str[8];

    box->boxNoStr[0] = no + '1';
    box->boxNoStr[1] = 0;
    HuWinInsertMesSet(box->winId, MESSNUM_PTR(box->boxNoStr), 0);
    if (!box->validF) {
        box->lastBoard = -1;
        box->name[0] = 0;
        HuWinInsertMesSet(box->winId, 0x90033, 1);
        HuWinMesSet(box->winId, 0x90042);
        for (i = 0; i < 10; i++) {
            HuSprAttrReset(grpId, i, HUSPR_ATTR_DISPOFF);
            HuSprBankSet(grpId, i, 12);
        }
        HuSprBankSet(grpId, 2, 11);
        HuSprBankSet(grpId, 5, 11);
        HuSprBankSet(grpId, 12, 10);
        HuSprBankSet(grpId, 12, 14);
        HuSprBankSet(grpId, 13, 0);
        Hu3DAnimAnimSet(box->charAnimId, lbl_1_bss_A8[0]);
        HuSprBankSet(grpId, 13, 0);
        for (i = 0; i < 3; i++) {
            HuSprAttrSet(grpId, i + 14, HUSPR_ATTR_DISPOFF);
        }
        HuSprPosSet(grpId, 10, 84.0f, 96.0f);
        HuSprPosSet(grpId, 12, 102.0f, 96.0f);
        HuSprPosSet(grpId, 13, 114.0f, 96.0f);
    } else {
        for (i = 0; i < 15; i++) {
            HuSprAttrReset(grpId, i, HUSPR_ATTR_DISPOFF);
        }
        HuSprBankSet(grpId, 2, 11);
        HuSprBankSet(grpId, 5, 11);
        sprintf(str, "%02d", time->mon + 1);
        for (i = 0; i < 2; i++) {
            HuSprBankSet(grpId, i, str[i] & 0xF);
        }
        sprintf(str, "%02d", time->mday);
        for (i = 0; i < 2; i++) {
            HuSprBankSet(grpId, i + 3, str[i] & 0xF);
        }
        sprintf(str, "%04d", time->year);
        for (i = 0; i < 4; i++) {
            HuSprBankSet(grpId, i + 6, str[i] & 0xF);
        }
        HuWinInsertMesSet(box->winId, MESSNUM_PTR(box->name), 1);
        HuWinMesSet(box->winId, 0x90042);
        HuSprBankSet(grpId, 12, 14);
        for (i = 0; i < 5; i++) {
            HuSprAttrSet(grpId, i + 13, HUSPR_ATTR_DISPOFF);
        }
        if (box->bankStar > 9999) {
            box->bankStar = 9999;
        }
        sprintf(str, "%d", box->bankStar);
        len = strlen(str);
        HuSprPosSet(grpId, 10, 108 - ((len * 12) + 36) / 2, 96.0f);
        HuSprPosSet(grpId, 12, 126 - ((len * 12) + 36) / 2, 96.0f);
        for (i = 0; i < 4; i++) {
            if (str[i] == 0) {
                break;
            }
            HuSprBankSet(grpId, i + 13, str[i] & 0xF);
            HuSprAttrReset(grpId, i + 13, HUSPR_ATTR_DISPOFF);
            HuSprPosSet(grpId, i + 13, (126 - ((len * 12) + 36) / 2) + ((i + 1) * 12), 96.0f);
        }
        if (box->lastBoard == -1) {
            Hu3DAnimAnimSet(box->charAnimId, lbl_1_bss_A8[1]);
        } else {
            Hu3DAnimAnimSet(box->charAnimId, lbl_1_bss_A8[box->lastBoard + 2]);
        }
    }
}

void FileselBoxPosSetV(s16 boxNo, HuVecF *pos)
{
    FILESEL_BOX *box = &lbl_1_bss_D8[boxNo];
    box->pos = *pos;
}

void FileselBoxPosSet(s16 boxNo, float x, float y, float z)
{
    FILESEL_BOX *box = &lbl_1_bss_D8[boxNo];
    box->pos.x = x;
    box->pos.y = y;
    box->pos.z = z;
}

void FileselBoxSwayModeSet(s16 boxNo, s32 mode)
{
    lbl_1_bss_D8[boxNo].swayMode = mode;
}

void FileselBoxUpdate(void)
{
    s16 prevGlowNo;
    HUPROCESS *proc = HuPrcCurrentGet();
    float swayRot = 0.0f;
    float swayPos = 0.0f;
    float glowTime = 0.0f;
    float swaySpeed = 0.0f;
    float swayAmp = 0.0f;
    s16 i;
    s16 boxNo;
    FILESEL_BOX *box;
    HU3D_MODEL *modelP;
    HU3D_MODEL *glowModelP;
    Mtx mtx;

    for (i = 0; i < 4; i++) {
        box = &lbl_1_bss_D8[i];
        if (box->proc == proc) {
            break;
        }
    }
    boxNo = i;
    swayRot = frandmod(90);
    swayPos = frandmod(180);
    swaySpeed = 0.01f * frandmod(100);
    while (1) {
        switch (box->swayMode) {
            case 0:
                swayAmp -= 0.05f;
                if (swayAmp < 0.0f) {
                    swayAmp = 0.0f;
                }
                break;
            case 1:
                swayAmp += 0.01f;
                if (swayAmp > 1.0f) {
                    swayAmp = 1.0f;
                }
                break;
        }
        swayRot += 1.0f + swaySpeed;
        if (swayRot > 360.0f) {
            swayRot -= 360.0f;
        }
        swayPos += 2.0f + swaySpeed;
        if (swayPos > 1440.0f) {
            swayPos -= 1440.0f;
        }
        if (box->swayMode != 2) {
            Hu3DModelRotSet(box->mdlId, 0.0f, 0.0f, swayAmp * (5.0 * HuSin(swayRot)));
            box->swayOfs.x = swayAmp * (2.0 * HuSin(swayPos));
            box->swayOfs.y = swayAmp * (2.0 * HuSin(swayPos / 2.0f));
        }
        Hu3DModelPosSet(box->mdlId, box->pos.x + box->swayOfs.x, box->pos.y + box->swayOfs.y, box->pos.z + box->swayOfs.z);
        if (!(Hu3DModelAttrGet(box->mdlId) & HU3D_ATTR_DISPOFF)) {
            Hu3DModelAttrReset(box->textMdlId, HU3D_ATTR_DISPOFF);
            Hu3DModelObjMtxGet(box->mdlId, lbl_1_data_190[i], mtx);
            Hu3DModelMtxSet(box->textMdlId, &mtx);
            modelP = &Hu3DData[box->mdlId];
            glowModelP = &Hu3DData[box->glowMdlId[0]];
            MTXCopy(glowModelP->mtx, modelP->mtx);
            glowModelP->pos = modelP->pos;
            glowModelP->scale = modelP->scale;
            glowModelP->rot = modelP->rot;
            glowModelP = &Hu3DData[box->glowMdlId[1]];
            MTXCopy(glowModelP->mtx, modelP->mtx);
            glowModelP->pos = modelP->pos;
            glowModelP->scale = modelP->scale;
            glowModelP->rot = modelP->rot;
            glowModelP = &Hu3DData[box->glowMdlId[2]];
            MTXCopy(glowModelP->mtx, modelP->mtx);
            glowModelP->pos = modelP->pos;
            glowModelP->scale = modelP->scale;
            glowModelP->rot = modelP->rot;
        } else {
            Hu3DModelAttrSet(box->textMdlId, HU3D_ATTR_DISPOFF);
        }
        switch (box->glowNo) {
            case 0:
                Hu3DModelAttrSet(box->glowMdlId[0], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrSet(box->glowMdlId[1], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrSet(box->glowMdlId[2], HU3D_ATTR_DISPOFF);
                break;
            case 1:
                Hu3DModelAttrReset(box->glowMdlId[0], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrSet(box->glowMdlId[1], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrSet(box->glowMdlId[2], HU3D_ATTR_DISPOFF);
                break;
            case 2:
                Hu3DModelAttrSet(box->glowMdlId[0], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrReset(box->glowMdlId[1], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrSet(box->glowMdlId[2], HU3D_ATTR_DISPOFF);
                break;
            case 3:
                Hu3DModelAttrSet(box->glowMdlId[0], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrSet(box->glowMdlId[1], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrReset(box->glowMdlId[2], HU3D_ATTR_DISPOFF);
                break;
        }
        if (prevGlowNo != box->glowNo) {
            glowTime = 0.0f;
        }
        modelP = &Hu3DData[box->glowMdlId[0]];
        VECScale(&modelP->scale, &modelP->scale, 1.0 + 0.2f * HuSin(glowTime));
        Hu3DModelTPLvlSet(box->glowMdlId[0], HuSin(2.0f * glowTime));
        modelP = &Hu3DData[box->glowMdlId[1]];
        VECScale(&modelP->scale, &modelP->scale, 1.0 + 0.2f * HuSin(glowTime));
        Hu3DModelTPLvlSet(box->glowMdlId[1], 1.0 - HuSin(glowTime));
        modelP = &Hu3DData[box->glowMdlId[2]];
        VECScale(&modelP->scale, &modelP->scale, 1.0 + 0.2f * HuSin(glowTime));
        Hu3DModelTPLvlSet(box->glowMdlId[2], 1.0 - HuSin(glowTime));
        if (box->glowNo == 2) {
            glowTime += 3.0f;
        } else {
            glowTime += 2.0f;
        }
        if (glowTime > 90.0f) {
            glowTime -= 90.0f;
        }
        prevGlowNo = box->glowNo;
        HuPrcVSleep();
    }
}

void CameraMove(void)
{
    Vec pos;
    Vec offset;
    Vec dir;
    Vec yOfs;

    f32 rotZ;
    s8 stickPos;
    static BOOL moveF = FALSE;

    if (moveF) {
        CRotM[1].y += 0.1f * HuPadStkX[0];
        CRotM[1].x += 0.1f * HuPadStkY[0];
        CZoomM[1] += HuPadTrigL[0] / 2;
        CZoomM[1] -= HuPadTrigR[0] / 2;
        if (CZoomM[1] < 100.0f) {
            CZoomM[1] = 100.0f;
        }
        pos.x = CenterM[1].x + (CZoomM[1] * (HuSin(CRotM[1].y) * HuCos(CRotM[1].x)));
        pos.y = (CenterM[1].y + (CZoomM[1] * -HuSin(CRotM[1].x)));
        pos.z = (CenterM[1].z + (CZoomM[1] * (HuCos(CRotM[1].y) * HuCos(CRotM[1].x))));
        offset.x = CenterM[1].x - pos.x;
        offset.y = CenterM[1].y - pos.y;
        offset.z = CenterM[1].z - pos.z;
        dir.x = (HuSin(CRotM[1].y) * HuSin(CRotM[1].x));
        dir.y = HuCos(CRotM[1].x);
        dir.z = (HuCos(CRotM[1].y) * HuSin(CRotM[1].x));
        rotZ = CRotM[1].z;
        yOfs.x = dir.x * (offset.x * offset.x + (1.0f - offset.x * offset.x) * HuCos(rotZ))
            + dir.y * (offset.x * offset.y * (1.0f - HuCos(rotZ)) - offset.z * HuSin(rotZ))
            + dir.z * (offset.x * offset.z * (1.0f - HuCos(rotZ)) + offset.y * HuSin(rotZ));

        yOfs.y = dir.y * (offset.y * offset.y + (1.0f - offset.y * offset.y) * HuCos(rotZ))
            + dir.x * (offset.x * offset.y * (1.0f - HuCos(rotZ)) + offset.z * HuSin(rotZ))
            + dir.z * (offset.y * offset.z * (1.0f - HuCos(rotZ)) - offset.x * HuSin(rotZ));

        yOfs.z = dir.z * (offset.z * offset.z + (1.0f - offset.z * offset.z) * HuCos(rotZ))
            + (dir.x * (offset.x * offset.z * (1.0 - HuCos(rotZ)) - offset.y * HuSin(rotZ))
                + dir.y * (offset.y * offset.z * (1.0 - HuCos(rotZ)) + offset.x * HuSin(rotZ)));

        VECCrossProduct(&dir, &offset, &offset);
        VECNormalize(&offset, &offset);
        stickPos = (HuPadSubStkX[0] & 0xF8);
        if (stickPos != 0) {
            CenterM[1].x += 0.05f * (offset.x * stickPos);
            CenterM[1].y += 0.05f * (offset.y * stickPos);
            CenterM[1].z += 0.05f * (offset.z * stickPos);
        }
        VECNormalize(&yOfs, &offset);
        stickPos = -(HuPadSubStkY[0] & 0xF8);
        if (stickPos != 0) {
            CenterM[1].x += 0.05f * (offset.x * stickPos);
            CenterM[1].y += 0.05f * (offset.y * stickPos);
            CenterM[1].z += 0.05f * (offset.z * stickPos);
        }
    }
}

void FileselPrcCreate(void (*func)(void), s32 param)
{
    HUPROCESS *proc = HuPrcChildCreate(func, 16, 0x3000, 0, HuPrcCurrentGet());
    proc->property = (void *)param;
}

void FileselPageTurn(void)
{
    HUPROCESS *proc = HuPrcCurrentGet();
    s32 bank = (s32)proc->property;
    s16 i;
    float t;
    for (i = 1; i <= 10; i++) {
        t = i / 10.0f;
        if (i == 5) {
            Hu3DAnimBankSet(lbl_1_bss_A2[0], bank);
            Hu3DAnimBankSet(lbl_1_bss_A2[1], bank);
        }
        Hu3DModelRotSet(lbl_1_bss_2D8[2], 0.0f, 180.0f * t, 0.0f);
        HuPrcVSleep();
    }
    HuPrcEnd();
}

s32 FileselCopy(s16 srcBoxNo)
{
    s16 boxNo;
    s16 i;
    s16 prevBox;
    s16 selNum;
    s16 diff;
    s32 result;
    int seNo;
    float t;
    HuVecF pos[3];

    boxNo = 0;
    FileselPrcCreate(FileselPageTurn, 2);
    for (i = 0; i < 3; i++) {
        Hu3DModelPosGet(lbl_1_bss_D8[i].mdlId, &pos[i]);
    }
    HuWinInsertMesSet(lbl_1_bss_2E0, MESSNUM_PTR(lbl_1_bss_D8[srcBoxNo].name), 0);
    HuWinMesSet(lbl_1_bss_2E0, 0x9000E);
    HuWinMesWait(lbl_1_bss_2E0);
    HuWinMesSet(lbl_1_bss_2DE, 0x10002);
    HuWinDispOn(lbl_1_bss_2DE);
    for (boxNo = 0; boxNo < 3; boxNo++) {
        if (!lbl_1_bss_D8[boxNo].validF) {
            break;
        }
    }
    FileselBoxSwayModeSet(boxNo, 0);
    for (i = selNum = 0; i < 3; i++) {
        if (!lbl_1_bss_D8[i].validF) {
            selNum++;
        }
    }
    for (i = 1; i <= 5; i++) {
        t = i / 5.0f;
        Hu3DModelScaleSet(lbl_1_bss_D8[boxNo].mdlId, 1.5 + 0.5 * t, 1.5 + 0.5 * t, 1.5 + 0.5 * t);
        FileselBoxPosSet(boxNo, pos[boxNo].x, pos[boxNo].y, (20.0f * t) + pos[boxNo].z);
        HuPrcVSleep();
    }
    lbl_1_bss_D8[boxNo].glowNo = 1;
    prevBox = boxNo;
    while (1) {
        if (selNum > 1) {
            diff = 0;
            if (HuPadDStkRep[0] & PAD_BUTTON_LEFT) {
                diff = -1;
            } else if (HuPadDStkRep[0] & PAD_BUTTON_RIGHT) {
                diff = 1;
            }
            if (diff) {
                HuAudFXPlay(0x483);
                prevBox = boxNo;
                boxNo += diff;
                if (boxNo < 0) {
                    boxNo = 2;
                } else if (boxNo >= 3) {
                    boxNo = 0;
                }
                while (lbl_1_bss_D8[boxNo].validF) {
                    boxNo += diff;
                    if (boxNo < 0) {
                        boxNo = 2;
                    } else if (boxNo >= 3) {
                        boxNo = 0;
                    }
                }
                for (i = 1; i <= 5; i++) {
                    t = HuSin(130.0 * (i / 5.0)) * (1.0 / HuSin(130.0));
                    t = 1.5 + 0.5 * t;
                    Hu3DModelScaleSet(lbl_1_bss_D8[boxNo].mdlId, t, t, t);
                    t = i / 5.0;
                    FileselBoxPosSet(boxNo, pos[boxNo].x, pos[boxNo].y, (20.0f * t) + pos[boxNo].z);
                    t = 1.5 + 0.5 * (1.0 - (i / 5.0));
                    Hu3DModelScaleSet(lbl_1_bss_D8[prevBox].mdlId, t, t, t);
                    t = 1.0 - (i / 5.0);
                    FileselBoxPosSet(prevBox, pos[prevBox].x, pos[prevBox].y, (20.0f * t) + pos[prevBox].z);
                    HuPrcVSleep();
                }
                lbl_1_bss_D8[boxNo].glowNo = 1;
                lbl_1_bss_D8[prevBox].glowNo = 0;
                FileselBoxSwayModeSet(boxNo, 0);
                FileselBoxSwayModeSet(prevBox, 1);
            }
        }
        if (UnMountCnt) {
            UnMountCnt = 0;
            lbl_1_bss_D8[boxNo].glowNo = 0;
            HuWinHomeClear(lbl_1_bss_2DE);
            FileselPrcCreate(FileselPageTurn, 1);
            return -4;
        }
        if (HuPadBtnDown[0] & PAD_BUTTON_A) {
            HuAudFXPlay(1);
            break;
        }
        if (HuPadBtnDown[0] & PAD_BUTTON_B) {
            HuAudFXPlay(3);
            goto cancel;
        }
        HuPrcVSleep();
    }
    HuWinHomeClear(lbl_1_bss_2DE);
    HuWinHomeClear(lbl_1_bss_2E0);
    for (i = 1; i <= 20; i++) {
        t = 1.5 * (1.0 - HuSin(90.0 * (i / 20.0)));
        Hu3DModelScaleSet(lbl_1_bss_D8[boxNo].mdlId, 1.5f * t, 1.5f * t, 1.5f * t);
        HuPrcVSleep();
    }
    lbl_1_bss_D8[boxNo].glowNo = 0;
    memcpy(&saveBuf[curSlotNo][SLBoxDataOffsetGet(boxNo)], &saveBuf[curSlotNo][SLBoxDataOffsetGet(srcBoxNo)], 0xCB2);
    seNo = HuAudFXPlay(0x484);
    result = FileSaveMesOpen(-1, 0x90028);
    HuAudFXStop(seNo);
    HuAudFXPlay(0x485);
    if (result == -1234) {
        result = -4;
    }
    if (result == 0) {
        lbl_1_bss_D8[boxNo].validF = TRUE;
        lbl_1_bss_D8[boxNo].lastBoard = lbl_1_bss_D8[srcBoxNo].lastBoard;
        lbl_1_bss_D8[boxNo].time = lbl_1_bss_D8[srcBoxNo].time;
        lbl_1_bss_D8[boxNo].bankStar = lbl_1_bss_D8[srcBoxNo].bankStar;
        memcpy(&lbl_1_bss_D8[boxNo].name, &lbl_1_bss_D8[srcBoxNo].name, 17);
        FileselSprUpdate(boxNo, boxNo);
        HuAudFXPlay(0x488);
        FileselBoxPosSet(boxNo, pos[boxNo].x, pos[boxNo].y, pos[boxNo].z);
        for (i = 1; i <= 10; i++) {
            t = 1.5 * (i / 10.0);
            Hu3DModelScaleSet(lbl_1_bss_D8[boxNo].mdlId, t, t, t);
            HuPrcVSleep();
        }
        FileselBoxSwayModeSet(boxNo, 1);
        HuWinMesSet(lbl_1_bss_2E0, 0x90010);
        HuWinMesWait(lbl_1_bss_2E0);
    } else {
        FileselBoxPosSet(boxNo, pos[boxNo].x, pos[boxNo].y, pos[boxNo].z);
        for (i = 1; i <= 5; i++) {
            t = 1.5 * (i / 5.0);
            Hu3DModelScaleSet(lbl_1_bss_D8[boxNo].mdlId, t, t, t);
            HuPrcVSleep();
        }
        FileselBoxSwayModeSet(boxNo, 1);
    }
    FileselPrcCreate(FileselPageTurn, 1);
    return result;
cancel:
    HuWinHomeClear(lbl_1_bss_2DE);
    lbl_1_bss_D8[boxNo].glowNo = 0;
    for (i = 1; i <= 5; i++) {
        t = 1.5 * (1.0 + 0.5 * (1.0 - (i / 5.0)));
        Hu3DModelScaleSet(lbl_1_bss_D8[boxNo].mdlId, t, t, t);
        t = 1.0 - (i / 5.0);
        FileselBoxPosSet(boxNo, pos[boxNo].x, pos[boxNo].y, (20.0f * t) + pos[boxNo].z);
        HuPrcVSleep();
    }
    FileselBoxSwayModeSet(boxNo, 1);
    FileselPrcCreate(FileselPageTurn, 1);
    return -1234;
}

/* @@PART3@@ */
/* Part 3 (0x6314-0x8B94). What part 1's HEAD should absorb is listed in the report. */

void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);

void FileselWaveStart(s16 no, HuVecF *pos);
void FileselZoomProc(void);
void FileselParticleProc(void);
void FileselWaveCopyHook(s16 layerNo);
void FileselWaveMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *material);
void FileselWaveUpdate(OMOBJ *obj);

#define fileselBox lbl_1_bss_D8
#define fileselAnim lbl_1_bss_CC
#define infoWinId lbl_1_bss_2E0
#define helpWinId lbl_1_bss_2DE
#define bookMdlId lbl_1_bss_2D8
#define parManId lbl_1_bss_98
#define waveObj lbl_1_bss_8
#define waveTexBuf lbl_1_bss_C
#define waveAnim1 lbl_1_bss_10
#define waveAnim2 lbl_1_bss_14
#define waveWork lbl_1_bss_18
#define objman lbl_1_bss_340

void FileselBoxFocusSet(s16 boxNo, BOOL focusF)
{
    if (focusF) {
        Hu3DAnimAnimSet(fileselBox[boxNo].frameAnimId, fileselAnim[1]);
    }
    else {
        Hu3DAnimAnimSet(fileselBox[boxNo].frameAnimId, fileselAnim[0]);
    }
}

void FileselBoxCopyFocusSet(s16 boxNo, BOOL focusF)
{
    if (focusF) {
        Hu3DAnimAnimSet(fileselBox[boxNo].frameAnimId, fileselAnim[2]);
    }
    else {
        Hu3DAnimAnimSet(fileselBox[boxNo].frameAnimId, fileselAnim[0]);
    }
}

s32 FileselEraseConfirm(s16 boxNo)
{
    HUWIN *winP = &winData[infoWinId];
    s32 result;
    s32 seNo;
    s32 i;
    float scale;
    FileselPrcCreate(FileselPageTurn, 3);
    FileselBoxFocusSet(boxNo, TRUE);
    HuWinInsertMesSet(infoWinId, (u32)&fileselBox[boxNo].name, 0);
    HuWinMesSet(infoWinId, 0x90013);
    HuWinMesWait(infoWinId);
    HuWinChoiceSet(infoWinId, -1);
    while (winP->stat) {
        if (UnMountCnt) {
            HuWinMesSet(infoWinId, 0x90026);
            HuWinMesWait(infoWinId);
            UnMountCnt = 0;
            FileselPrcCreate(FileselPageTurn, 1);
            FileselBoxFocusSet(boxNo, FALSE);
            return CARD_RESULT_NOFILE;
        }
        HuPrcVSleep();
    }
    result = winP->choice;
    HuWinHomeClear(infoWinId);
    if (UnMountCnt) {
        HuWinMesSet(infoWinId, 0x90026);
        HuWinMesWait(infoWinId);
        UnMountCnt = 0;
        FileselPrcCreate(FileselPageTurn, 1);
        FileselBoxFocusSet(boxNo, FALSE);
        return CARD_RESULT_NOFILE;
    }
    if (result == 0) {
        seNo = HuAudFXPlay(0x486);
        SLSaveEmptySet(curSlotNo, boxNo);
        result = FileSaveMesOpen(-1, 0x90029);
        HuAudFXStop(seNo);
        seNo = HuAudFXPlay(0x487);
        if (result == 0) {
            HuAudFXPlay(0x489);
            for (i = 1; i <= 20; i++) {
                scale = 1.0 - HuSin(90.0 * (i / 20.0));
                Hu3DModelScaleSet(fileselBox[boxNo].mdlId, 1.5f * scale, 1.5f * scale, 1.5f * scale);
                HuPrcVSleep();
            }
            HuPrcSleep(30);
            fileselBox[boxNo].validF = FALSE;
            fileselBox[boxNo].validF = FALSE;
            FileselSprUpdate(boxNo, boxNo);
            fileselBox[boxNo].glowNo = 1;
            FileselBoxFocusSet(boxNo, FALSE);
            for (i = 1; i <= 10; i++) {
                scale = i / 10.0;
                Hu3DModelScaleSet(fileselBox[boxNo].mdlId, 1.5f * scale, 1.5f * scale, 1.5f * scale);
                HuPrcVSleep();
            }
            HuWinMesSet(infoWinId, 0x90016);
            HuWinMesWait(infoWinId);
        }
    }
    else {
        result = 0;
    }
    FileselPrcCreate(FileselPageTurn, 1);
    FileselBoxFocusSet(boxNo, FALSE);
    return result;
}

void FileselCommonReset(void)
{
    GwCommon = GwCommonOrig;
    if (OSGetSoundMode() == OS_SOUND_MODE_MONO) {
        GwCommon.outputMode = 0;
    }
    else {
        GwCommon.outputMode = 1;
    }
    if (HuMCProbe(1)) {
        HuMCMicSet(0);
    }
    else {
        HuMCMicSet(1);
        HuMCInit(0);
        if (HuMCMount(1)) {
            HuMCMicSet(0);
        }
        HuMCClose();
    }
    HuMCMicGet();
}

void FileselZClear(void)
{
    HU3D_CAMERA *cameraP = &Hu3DCamera[Hu3DCameraNo];
    Mtx44 proj;
    Mtx modelview;
    Vec pos;
    Vec target;
    Vec up;
    float z;

    MTXPerspective(proj, cameraP->fov, cameraP->aspect, cameraP->near, cameraP->far);
    GXSetProjection(proj, GX_PERSPECTIVE);
    GXSetViewport(cameraP->viewportX, cameraP->viewportY, cameraP->viewportW, cameraP->viewportH, cameraP->viewportNear, cameraP->viewportFar);
    GXSetScissor(cameraP->scissorX, cameraP->scissorY, cameraP->scissorW, cameraP->scissorH);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_SRCALPHA, GX_LO_NOOP);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ONE);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetCullMode(GX_CULL_NONE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
    GXSetColorUpdate(GX_FALSE);
    pos.x = pos.y = pos.z = 0;
    target.x = target.y = 0;
    target.z = -100;
    up.x = up.z = 0;
    up.y = 1;
    MTXLookAt(modelview, &pos, &up, &target);
    GXLoadPosMtxImm(modelview, GX_PNMTX0);
    z = cameraP->far - 1;
    pos.x = pos.y = z * HuTan(cameraP->fov / 2);
    pos.x *= cameraP->aspect;
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-pos.x, -pos.y, -z);
    GXPosition3f32(pos.x, -pos.y, -z);
    GXPosition3f32(pos.x, pos.y, -z);
    GXPosition3f32(-pos.x, pos.y, -z);
    GXEnd();
    GXSetColorUpdate(GX_TRUE);
}

void FileselDecideExec(s16 boxNo, s32 mode)
{
    HuVecF waveOfs = { 0, 0, 15 };
    HuVecF temp;
    HuVecF boxPos;
    HuVecF pos;
    HuVecF rot;
    FILESEL_BOX *boxP;
    float t;
    float scale;
    float dirF;
    s16 i;
    s16 j;

    Hu3DModelPosGet(bookMdlId[0], &pos);
    Hu3DModelRotGet(bookMdlId[0], &rot);
    rot.x = (s32)rot.x % 360;
    if (rot.x > 180.0f) {
        rot.x -= 360.0f;
    }
    rot.y = (s32)rot.y % 360;
    if (rot.y > 180.0f) {
        rot.y -= 360.0f;
    }
    rot.z = (s32)rot.z % 360;
    if (rot.z > 180.0f) {
        rot.z -= 360.0f;
    }
    if (mode == 0) {
        HuWinDispOff(helpWinId);
        HuWinExClose(infoWinId);
        Hu3DModelScaleGet(bookMdlId[2], &temp);
        if (temp.x > 0.1f) {
            for (i = 1; i <= 10; i++) {
                t = i / 10.0f;
                Hu3DModelRotSet(bookMdlId[2], 0, 520.0f * t, 0);
                scale = 1.0 - t;
                Hu3DModelScaleSet(bookMdlId[2], scale, scale, scale);
                HuPrcVSleep();
            }
        }
        for (i = 1; i <= 20; i++) {
            t = HuSin(90.0 * (i / 20.0f));
            temp.x = (1.0 - t) * pos.x;
            temp.y = (1.0 - t) * pos.y;
            temp.z = (1.0 - t) * pos.z;
            Hu3DModelPosSetV(bookMdlId[0], &temp);
            temp.x = (1.0 - t) * rot.x;
            temp.y = (1.0 - t) * rot.y;
            temp.z = (1.0 - t) * rot.z;
            Hu3DModelRotSetV(bookMdlId[0], &temp);
            HuPrcVSleep();
        }
        FileselPrcCreate(FileselZoomProc, 0);
        HuPrcSleep(40);
        HuAudFXPlay(0x48C);
        FileselWaveStart(0, &waveOfs);
    }
    else {
        for (i = 0; i < 3; i++) {
            FileselBoxSwayModeSet(boxNo, 0);
        }
        HuWinDispOff(helpWinId);
        HuWinExClose(infoWinId);
        for (i = 0; i < 3; i++) {
            FileselBoxSwayModeSet(boxNo, 2);
        }
        for (i = 1; i <= 10; i++) {
            t = i / 10.0f;
            for (j = 0; j < 3; j++) {
                if (j != boxNo) {
                    boxP = &fileselBox[j];
                    Hu3DModelRotSet(boxP->mdlId, 0, 520.0f * t, 0);
                    scale = 1.5 * (1.0 - t);
                    Hu3DModelScaleSet(boxP->mdlId, scale, scale, scale);
                }
            }
            Hu3DModelRotSet(bookMdlId[2], 0, 520.0f * t, 0);
            scale = 1.0 - t;
            Hu3DModelScaleSet(bookMdlId[2], scale, scale, scale);
            HuPrcVSleep();
        }
        boxP = &fileselBox[boxNo];
        Hu3DModelPosGet(boxP->mdlId, &boxPos);
        OSReport("%f,%f,%f\n", boxPos.x, boxPos.y, boxPos.z);
        dirF = (frandmod(2) == 0) ? 1 : -1;
        for (i = 1; i <= 80; i++) {
            if (i <= 20) {
                t = HuSin(90.0 * (i / 20.0f));
                temp.x = (1.0 - t) * pos.x;
                temp.y = (1.0 - t) * pos.y;
                temp.z = (1.0 - t) * pos.z;
                Hu3DModelPosSetV(bookMdlId[0], &temp);
                temp.x = (1.0 - t) * rot.x;
                temp.y = (1.0 - t) * rot.y;
                temp.z = (1.0 - t) * rot.z;
                Hu3DModelRotSetV(bookMdlId[0], &temp);
                Hu3DShadowTPLvlSet(0.5 * (1.0 - t));
            }
            if (i == 10) {
                FileselPrcCreate(FileselParticleProc, boxNo);
                HuAudFXPlay(0x48B);
            }
            if (i > 10) {
                t = (i - 10.0f) / 70.0f;
                temp.x = (1.0 - t) * boxPos.x;
                temp.y = (1.0 - t) * boxPos.y + 100.0 * HuSin(180.0f * t);
                temp.z = boxPos.z + 300.0 * HuSin(180.0f * t) - 150.0f * t;
                FileselBoxPosSetV(boxNo, &temp);
                Hu3DModelRotSet(boxP->mdlId, 0, 480.0f * t, 45.0f * t * dirF);
                scale = 1.125 + 1.125 * (1.0 - t);
                Hu3DModelScaleSet(boxP->mdlId, scale, scale, scale);
            }
            if (i == 40) {
                FileselPrcCreate(FileselZoomProc, 0);
            }
            if (i == 75) {
                HuAudFXPlay(0x48C);
                FileselWaveStart(0, &waveOfs);
            }
            HuPrcVSleep();
        }
    }
}

void FileselZoomProc(void)
{
    HU3D_CAMERA *cameraP = &Hu3DCamera[1];
    float zoom = CZoomM[1];
    float t;
    s16 i;
    for (i = 0; i < 100.0; i++) {
        t = i / 100.0;
        t = 1.0 - HuCos(90.0 * t);
        CZoomM[1] = zoom + (200.0f - zoom) * t;
        HuPrcVSleep();
    }
    HuPrcEnd();
}

void FileselParticleProc(void)
{
    HUPROCESS *proc = HuPrcCurrentGet();
    s32 boxNo = (s32)proc->property;
    Mtx mtx;
    Mtx parMtx;
    HuVecF parOfs[5] = {
        { 0, 0, 0 },
        { 40, 10, 0 },
        { -40, 10, 0 },
        { 20, -40, 0 },
        { -20, -40, 0 },
    };
    HuVecF parPos;
    HuVecF pos;
    s32 i;
    for (i = 0; i < 5; i++) {
        Hu3DParManAttrReset(parManId[i], 1);
    }
    while (1) {
        Hu3DModelObjMtxGet(fileselBox[boxNo].mdlId, "siori_fix02-siori_waku", mtx);
        Hu3DMtxTransGet(mtx, &pos);
        if (pos.z > 0.0f) {
            for (i = 0; i < 5; i++) {
                MTXTrans(parMtx, parOfs[i].x, parOfs[i].y, parOfs[i].z);
                MTXConcat(mtx, parMtx, parMtx);
                Hu3DMtxTransGet(parMtx, &parPos);
                Hu3DParManPosSet(parManId[i], parPos.x, parPos.y, parPos.z);
                VECSubtract(&parPos, &pos, &parPos);
                if (parPos.x * parPos.x + parPos.y * parPos.y + parPos.z * parPos.z <= 0.0) {
                    parPos.x = parPos.y = parPos.z = 0;
                }
                else {
                    VECNormalize(&parPos, &parPos);
                }
                Hu3DParManVecSet(parManId[i], parPos.x, -0.5f, parPos.z);
            }
        }
        else {
            for (i = 0; i < 5; i++) {
                Hu3DParManAttrSet(parManId[i], 1);
            }
        }
        HuPrcVSleep();
    }
}

void FileselWaveCopyHook(s16 layerNo)
{
    if (waveTexBuf) {
        GXSetTexCopySrc(0, 0, 640, 480);
        GXSetTexCopyDst(640, 480, GX_TF_RGB565, GX_FALSE);
        GXCopyTex(waveTexBuf, GX_FALSE);
    }
}

void FileselWaveMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *material)
{
    HU3D_CAMERA *cameraP = &Hu3DCamera[Hu3DCameraNo];
    FILESEL_WAVE *work = drawObj->model->hookData;
    Mtx modelMtx;
    Mtx scaleMtx;
    Mtx texMtx;
    Mtx lightMtx;
    Mtx invCamera;
    GXTexObj texObj;
    float transS;

    work->scroll += work->scrollSpeed;
    GXInitTexObj(&texObj, waveTexBuf, 640, 480, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0, 0, 0, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    HuSprTexLoad(waveAnim1, 0, GX_TEXMAP2, GX_REPEAT, GX_REPEAT, GX_LINEAR);
    HuSprTexLoad(waveAnim2, 0, GX_TEXMAP3, GX_REPEAT, GX_REPEAT, GX_LINEAR);
    GXSetNumTexGens(3);
    GXSetNumTevStages(2);
    transS = (cameraP->scissorX + cameraP->scissorW / 2.0f) / 640.0f;
    MTXLightPerspective(lightMtx, cameraP->fov, 1.2f, 0.5f, -0.5f, transS, 0.5f);
    MTXInverse(Hu3DCameraMtx, invCamera);
    MTXConcat(invCamera, drawObj->matrix, modelMtx);
    MTXConcat(lightMtx, Hu3DCameraMtx, texMtx);
    MTXConcat(texMtx, modelMtx, texMtx);
    GXLoadTexMtxImm(texMtx, GX_TEXMTX1, GX_MTX3x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    MTXTrans(modelMtx, 0, work->scroll, 0);
    MTXScale(scaleMtx, 0.8f, 0.8f, 1);
    MTXConcat(scaleMtx, modelMtx, texMtx);
    GXLoadTexMtxImm(texMtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTevColor(GX_TEVREG0, work->color);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP0, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumIndStages(1);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP2);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE, GX_ITM_0);
    GXSetIndTexMtx(GX_ITM_0, work->indMtx, -1);
}

void FileselWaveUpdate(OMOBJ *obj)
{
    FILESEL_WAVE *work;
    s16 i;
    float t;
    for (i = 0, work = waveWork; i < 2; i++, work++) {
        if (work->time <= 0.0f) {
            continue;
        }
        work->indMtx[0][0] = 0;
        work->indMtx[0][1] = 0;
        work->indMtx[0][2] = (0.09f * work->time) / 120.0f;
        work->indMtx[1][0] = 0;
        work->indMtx[1][1] = 0;
        work->indMtx[1][2] = (0.09f * work->time) / 120.0f;
        work->scrollSpeed = (-0.02f * work->time) / 120.0f;
        t = (work->time - 80.0f) / 40.0f;
        if (t >= 0.0f) {
            work->color.r = 0;
            work->color.g = 16.0f * t;
            work->color.b = 64.0f * t;
        }
        work->time -= 1.0f;
        if (work->time <= 0.0f) {
            Hu3DLayerHookReset(10);
            Hu3DModelAttrSet(obj->mdlId[i], HU3D_ATTR_DISPOFF);
            obj->objFunc = NULL;
        }
    }
}

void FileselWaveStart(s16 no, HuVecF *pos)
{
    OMOBJ *obj = waveObj;
    if (obj) {
        if (no >= 2 || no < 0) {
            return;
        }
        waveWork[no].scroll = 0;
        waveWork[no].time = 120;
        Hu3DModelAttrReset(obj->mdlId[no], HU3D_ATTR_DISPOFF);
        Hu3DModelPosSet(obj->mdlId[no], pos->x, pos->y, pos->z);
        Hu3DModelRotSet(obj->mdlId[no], 90, 0, 0);
        Hu3DModelScaleSet(obj->mdlId[no], 1, 1, 1);
        Hu3DLayerHookSet(10, FileselWaveCopyHook);
        obj->objFunc = FileselWaveUpdate;
    }
}

void FileselWaveCreate(OMOBJ *obj)
{
    s16 i;
    HU3D_MODEL *modelP = NULL;
    omSetStatBit(obj, OM_STAT_MODELPAUSE);
    waveAnim1 = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_filesel, 0x19), HU_MEMNUM_OVL, HEAP_MODEL));
    waveAnim2 = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_filesel, 0x19), HU_MEMNUM_OVL, HEAP_MODEL));
    waveTexBuf = HuMemDirectMallocNum(HEAP_MODEL, GXGetTexBufferSize(640, 480, GX_TF_RGB565, GX_FALSE, 0), HU_MEMNUM_OVL);
    for (i = 0; i < 2; i++) {
        obj->mdlId[i] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_filesel, 0x18), HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelPosSet(obj->mdlId[i], 0, 0, 0);
        Hu3DModelRotSet(obj->mdlId[i], 90, 0, 0);
        Hu3DModelScaleSet(obj->mdlId[i], 0.1f, 0.1f, 0.1f);
        Hu3DModelCameraSet(obj->mdlId[i], HU3D_CAM1);
        Hu3DModelLayerSet(obj->mdlId[i], 3);
        Hu3DModelAttrSet(obj->mdlId[i], HU3D_ATTR_DISPOFF);
        Hu3DModelMatHookSet(obj->mdlId[i], FileselWaveMatHook);
        waveWork[i].indMtx[0][0] = waveWork[i].indMtx[0][1] = waveWork[i].indMtx[0][2] = waveWork[i].indMtx[1][0] = waveWork[i].indMtx[1][1] = waveWork[i].indMtx[1][2] = 0;
        waveWork[i].color = (GXColor) { 0, 16, 64, 16 };
        waveWork[i].scroll = 0;
        waveWork[i].scrollSpeed = 0;
        modelP = &Hu3DData[obj->mdlId[i]];
        modelP->hookData = &waveWork[i];
    }
    obj->objFunc = NULL;
    waveObj = obj;
}

void FileselWaveKill(void)
{
    OMOBJ *obj = waveObj;
    HU3D_MODEL *modelP = NULL;
    s16 i;
    if (obj) {
        Hu3DLayerHookReset(10);
        for (i = 0; i < 2; i++) {
            modelP = &Hu3DData[obj->mdlId[i]];
            modelP->hookData = NULL;
            Hu3DModelKill(obj->mdlId[i]);
        }
        omDelObjEx(objman, obj);
    }
    obj = NULL;
    if (waveAnim1) {
        HuSprAnimKill(waveAnim1);
    }
    waveAnim1 = NULL;
    if (waveAnim2) {
        HuSprAnimKill(waveAnim2);
    }
    waveAnim2 = NULL;
    if (waveTexBuf) {
        HuMemDirectFree(waveTexBuf);
    }
    waveTexBuf = NULL;
}

/* @@END@@ */
