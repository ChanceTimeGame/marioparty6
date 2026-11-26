#include "game/object.h"
#include "game/hu3d.h"
#include "game/disp.h"
#include "game/sprite.h"
#include "game/audio.h"
#include "game/main.h"
#include "game/init.h"
#include "game/data.h"
#include "game/memory.h"
#include "game/process.h"
#include "game/window.h"
#include "game/wipe.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/charman.h"
#include "game/pad.h"
#include "humath.h"
#include "msm.h"

#include "datadir_enum.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void ObjectSetup(void);

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

#define HU_PAD_BTN_ALL (HuPadBtn[0] | HuPadBtn[1] | HuPadBtn[2] | HuPadBtn[3])
#define HU_PAD_BTNDOWN_ALL (HuPadBtnDown[0] | HuPadBtnDown[1] | HuPadBtnDown[2] | HuPadBtnDown[3])
#define HU_PAD_DSTK_ALL (HuPadDStkRep[0] | HuPadDStkRep[1] | HuPadDStkRep[2] | HuPadDStkRep[3])

/* Files of the title directory used here (no datanum/title.h for MP6 yet) */
#define TITLE_ANM_hudson DATANUM(DATA_title, 1)
#define TITLE_ANM_prog DATANUM(DATA_title, 6)
#define TITLE_ANM_progOn DATANUM(DATA_title, 7)
#define TITLE_ANM_progOff DATANUM(DATA_title, 8)
#define TITLE_ANM_dolby DATANUM(DATA_title, 9)
#define TITLE_ANM_progCursor DATANUM(DATA_title, 10)
#define TITLE_ANM_progCursorSel DATANUM(DATA_title, 11)
#define TITLE_HSF_title DATANUM(DATA_title, 19)
#define TITLE_HSF_back DATANUM(DATA_title, 20)
#define TITLE_HSF_pressStart DATANUM(DATA_title, 21)

static OMOBJ *OutViewObj;
OMOBJMAN *BootObjMan;
Vec BootCameraRot[2];
Vec BootCameraCenter[2];
float BootCameraZoom[2];
HU3D_LIGHTID BootLightId[2];
static HU3D_MODELID TitleMdlId[4];
s16 lbl_1_bss_4;

void BootExec(void);
void BootProgExec(void);
void CameraOutView(OMOBJ *obj);
void CameraMove(OMOBJ *obj);
void BootTitleCreate(void);
BOOL BootTitleExec(BOOL fromTitle);
void *NintendoDataDecode(void);
void *WarningDataDecode(u32 *src);
void BootWarningExec(void);

/* opening.c */
void OpeningCreate(void);
void OpeningExec(BOOL fromBoot);
void OpeningKill(void);

void ObjectSetup(void)
{
    int i;
    OMOVLHIS *his;
    OSReport("******* Boot ObjectSetup *********\n");
    BootObjMan = omInitObjMan(50, 8192);
    BootCameraRot[0].x = -5;
    BootCameraRot[0].y = 0;
    BootCameraRot[0].z = 0;
    BootCameraCenter[0].x = 0;
    BootCameraCenter[0].y = 105;
    BootCameraCenter[0].z = -54;
    BootCameraZoom[0] = 636;
    Hu3DCameraCreate(HU3D_CAM0);
    Hu3DCameraPerspectiveSet(HU3D_CAM0, 45, 20, 15000, HU_DISP_ASPECT);
    Hu3DCameraViewportSet(HU3D_CAM0, 0, 0, HU_FB_WIDTH, HU_FB_HEIGHT, 0, 1);
    HuPrcCreate(BootExec, 100, 12288, 0);
    OutViewObj = omAddObjEx(BootObjMan, 32730, 0, 0, -1, CameraOutView);
    omAddObjEx(BootObjMan, 0, 32, 32, -1, CameraMove);
    BootLightId[0] = Hu3DGLightCreate(0, 10, 1000, 0, -0.01f, -1, 64, 64, 96);
    Hu3DGLightInfinitytSet(BootLightId[0]);
    BootLightId[1] = Hu3DGLightCreate(0, 2000, 100, 0, -1, -0.05f, 160, 160, 160);
    Hu3DGLightInfinitytSet(BootLightId[1]);
    Hu3DBGColorSet(0, 0, 0);
    his = omOvlHisGet(0);
    omOvlHisChg(0, his->ovl, 1, his->stat);
    for(i=0; i<GW_PLAYER_MAX; i++) {
        GwPlayerConf[i].charNo = GwPlayerConf[i].padNo = i;
    }
}

void BootExec(void)
{
    HUPROCESS *parent = HuPrcCurrentGet();
    HUSPRID hudsonSprId;
    HUSPRID dolbySprId;
    HUSPRID nintendoSprId;
    BOOL fromTitle;
    HUSPR_GROUPID gid;
    ANIMDATA *anim;
    OSTick tickPrev;
    void *sampBuf;

    s16 i;
    s16 statId;
    fromTitle = FALSE;
    statId = -1;
    if(omovlevtno == 0 && VIGetTvFormat() != VI_MPAL) {
        BootProgExec();
    }
    if(omovlevtno == 0) {
        BootWarningExec();
    }
    gid = HuSprGrpCreate(3);
    anim = HuSprAnimRead(NintendoDataDecode());
    nintendoSprId = HuSprCreate(anim, 0, 0);
    HuSprGrpMemberSet(gid, 0, nintendoSprId);
    HuSprPosSet(gid, 0, HU_DISP_CENTERX, HU_DISP_CENTERY);
    HuSprDispOff(gid, 0);
    if(omovlevtno != 0) {
        HuAudSndGrpSetSet(MSM_GRP_MENU);
        anim = HuSprAnimDataRead(TITLE_ANM_hudson);
        hudsonSprId = HuSprCreate(anim, 0, 0);
        HuSprGrpMemberSet(gid, 1, hudsonSprId);
        HuSprPosSet(gid, 1, HU_DISP_CENTERX, HU_DISP_CENTERY);
        HuSprDispOff(gid, 1);
        dolbySprId = HuSprCreate(HuSprAnimDataRead(TITLE_ANM_dolby), 0, 0);
        HuSprGrpMemberSet(gid, 2, dolbySprId);
        HuSprPosSet(gid, 2, HU_DISP_CENTERX, HU_DISP_CENTERY-34);
        HuSprAttrSet(gid, 2, HUSPR_ATTR_DISPOFF|HUSPR_ATTR_LINEAR);
        HuWinInit(1);
        BootTitleCreate();
        OpeningCreate();
        NintendoDispF = TRUE;
        fromTitle = TRUE;
    } else {
        for(i=0; i<GW_PLAYER_MAX; i++) {
            GwPlayerConf[i].padNo = i;
        }
        HuSprDispOn(gid, 0);
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
        WipeWait();
        WipeColorSet(255, 255, 255);
        if(!NintendoDispF) {
            tickPrev = OSGetTick();
            CharInit();
            HuWindowInit();
            GameMesInit();
            HuWinInit(1);
            statId = HuDataDirReadAsync(DATA_title);
            while(OSTicksToMilliseconds(OSGetTick()-tickPrev) < 1500) {
                HuPrcVSleep();
            }
            OSReport("%dms\n", OSTicksToMilliseconds(OSGetTick()-tickPrev));
        } else {
            for(i=0; i<90; i++) {
                if(HuPadBtnDown[0] & (PAD_BUTTON_A|PAD_BUTTON_START)) {
                    break;
                }
                HuPrcVSleep();
            }
        }
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
        WipeWait();
        if(!NintendoDispF) {
            tickPrev = OSGetTick();
            while(!HuDataGetAsyncStat(statId)) {
                HuPrcVSleep();
            }
            BootTitleCreate();
            anim = HuSprAnimDataRead(TITLE_ANM_hudson);
            hudsonSprId = HuSprCreate(anim, 0, 0);
            HuSprGrpMemberSet(gid, 1, hudsonSprId);
            HuSprPosSet(gid, 1, HU_DISP_CENTERX, HU_DISP_CENTERY);
            HuSprDispOff(gid, 1);
            dolbySprId = HuSprCreate(HuSprAnimDataRead(TITLE_ANM_dolby), 0, 0);
            HuSprGrpMemberSet(gid, 2, dolbySprId);
            HuSprPosSet(gid, 2, HU_DISP_CENTERX, HU_DISP_CENTERY-34);
            HuSprDispOff(gid, 2);
            while(OSTicksToMilliseconds(OSGetTick()-tickPrev) < 1000) {
                HuPrcVSleep();
            }
            OSReport("%dms\n", OSTicksToMilliseconds(OSGetTick()-tickPrev));
        } else {
            HuPrcSleep(60);
        }
        HuSprDispOff(gid, 0);
        HuSprDispOn(gid, 1);
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
        WipeWait();
        if(!NintendoDispF) {
            tickPrev = OSGetTick();
            OpeningCreate();
            while(OSTicksToMilliseconds(OSGetTick()-tickPrev) < 1500) {
                HuPrcVSleep();
            }
            OSReport("%dms\n", OSTicksToMilliseconds(OSGetTick()-tickPrev));
        } else {
            for(i=0; i<90; i++) {
                if(HuPadBtnDown[0] & (PAD_BUTTON_A|PAD_BUTTON_START)) {
                    break;
                }
                HuPrcVSleep();
            }
        }
        WipeColorSet(255, 255, 255);
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
        WipeWait();
        HuPrcSleep(60);
        HuSprDispOff(gid, 0);
        HuSprDispOff(gid, 1);
        HuSprDispOn(gid, 2);
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
        WipeWait();
        if(!NintendoDispF) {
            HuPrcSleep(60);
        } else {
            for(i=0; i<60; i++) {
                if(HuPadBtnDown[0] & (PAD_BUTTON_A|PAD_BUTTON_START)) {
                    break;
                }
                HuPrcVSleep();
            }
        }
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
        WipeWait();
        HuSprDispOff(gid, 2);
        if(!NintendoDispF) {
            HuDataDirClose(DATA_title);
            sampBuf = HuMemDirectMallocNum(HEAP_DVD, msmSysGetSampSize(MSM_GRP_INIT), HU_MEMNUM_OVL);
            msmSysLoadGroup(MSM_GRP_INIT, sampBuf, FALSE);
            HuMemDirectFree(sampBuf);
            msmSysSetGroupLoadMode(1);
            HuAudSndGrpSetSet(MSM_GRP_MENU);
        } else {
            HuPrcSleep(90);
        }
    repeatOpening:
        OpeningExec(TRUE);
        NintendoDispF = TRUE;
        HuSprDispOff(gid, 0);
        HuSprDispOff(gid, 1);
        HuSprDispOff(gid, 2);
    }
    if(!BootTitleExec(fromTitle)) {
        fromTitle = FALSE;
        HuPrcSleep(30);
        goto repeatOpening;
    }
    HuPrcSleep(60);
    OpeningKill();
    HuDataDirClose(DATA_title);
    HuAudSStreamAllFadeOut(1000);
    statId = HuDataDirReadAsync(DATA_filesel);
    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
    WipeWait();
    HuPrcSleep(30);
    while(!HuDataGetAsyncStat(statId)) {
        HuPrcVSleep();
    }
    if(omSysExitReq != 0) {
        omOvlReturn(1);
    } else {
        omOvlCall(DLL_fileseldll, 0, 0);
    }
    for(i=0; i<GW_PLAYER_MAX; i++) {
        GwPlayerConf[i].charNo = -1;
    }
    HuPrcEnd();
    while(1) {
        HuPrcVSleep();
    }
}

void BootProgExec(void)
{
    static u16 posTbl[] = {
        221, 311,
        357, 311
    };
    s16 choice = 0;
    s16 time;
    HUSPR_GROUPID gid;
    ANIMDATA *anim;
    HUSPRID sprid;
    if(OSGetResetCode() != 0) {
        return;
    }
    if(VIGetDTVStatus() != VI_NON_INTERLACE) {
        RenderMode = &GXNtsc480IntDf;
        VIConfigure(&GXNtsc480IntDf);
        VIFlush();
        return;
    }
    if(OSGetProgressiveMode() == OS_PROGRESSIVE_MODE_OFF && !(HU_PAD_BTN_ALL & PAD_BUTTON_B)) {
        return;
    }
    if(OSGetProgressiveMode() == OS_PROGRESSIVE_MODE_ON) {
        RenderMode = &GXNtsc480IntDf;
        VIConfigure(&GXNtsc480IntDf);
        VIFlush();
    }
    gid = HuSprGrpCreate(3);
    anim = HuSprAnimDataRead(TITLE_ANM_prog);
    sprid = HuSprCreate(anim, 0, 0);
    HuSprGrpMemberSet(gid, 0, sprid);
    HuSprPosSet(gid, 0, HU_DISP_CENTERX, HU_DISP_CENTERY);
    anim = HuSprAnimDataRead(TITLE_ANM_progCursor);
    sprid = HuSprCreate(anim, 0, 0);
    HuSprGrpMemberSet(gid, 1, sprid);
    HuSprPosSet(gid, 1, posTbl[(choice*2)+0], posTbl[(choice*2)+1]);
    anim = HuSprAnimDataRead(TITLE_ANM_progCursorSel);
    sprid = HuSprCreate(anim, 0, 0);
    HuSprGrpMemberSet(gid, 2, sprid);
    HuSprPosSet(gid, 2, posTbl[(choice*2)+0], posTbl[(choice*2)+1]);
    HuSprDispOff(gid, 2);
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
    WipeWait();
    for(time=0; time<600; time++) {
        if(HU_PAD_DSTK_ALL & (PAD_BUTTON_LEFT | PAD_BUTTON_RIGHT)) {
            choice ^= 1;
            HuSprPosSet(gid, 1, posTbl[(choice*2)+0], posTbl[(choice*2)+1]);
            time = 0;
        }
        if(HU_PAD_BTNDOWN_ALL & PAD_BUTTON_A) {
            HuSprDispOff(gid, 1);
            HuSprPosSet(gid, 2, posTbl[(choice*2)+0], posTbl[(choice*2)+1]);
            HuSprDispOn(gid, 2);
            break;
        }
        HuPrcVSleep();
    }
    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 20);
    WipeWait();
    if(choice == 0) {
        OSSetProgressiveMode(OS_PROGRESSIVE_MODE_ON);
        RenderMode = &GXNtsc480Prog;
        VIConfigure(&GXNtsc480Prog);
        VIFlush();
        HuPrcSleep(100);
    } else {
        OSSetProgressiveMode(OS_PROGRESSIVE_MODE_OFF);
        GXSetCopyFilter(RenderMode->aa, RenderMode->sample_pattern, GX_TRUE, RenderMode->vfilter);
    }
    HuSprGrpKill(gid);
    gid = HuSprGrpCreate(1);
    if(choice == 0) {
        anim = HuSprAnimDataRead(TITLE_ANM_progOn);
    } else {
        anim = HuSprAnimDataRead(TITLE_ANM_progOff);
    }
    sprid = HuSprCreate(anim, 0, 0);
    HuSprGrpMemberSet(gid, 0, sprid);
    HuSprPosSet(gid, 0, HU_DISP_CENTERX, HU_DISP_CENTERY);
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 20);
    WipeWait();
    HuPrcSleep(60);
    for(time=0; time<180; time++) {
        if(HU_PAD_BTNDOWN_ALL & PAD_BUTTON_A) {
            break;
        }
        HuPrcVSleep();
    }
    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
    WipeWait();
    HuSprGrpKill(gid);
    HuPrcSleep(30);
}

void CameraOutView(OMOBJ *obj)
{
    static u16 cameraMask[] = { HU3D_CAM0, HU3D_CAM1 };
    s16 i;
    for(i=0; i<1; i++) {
        Vec pos, target, up;
        float x, y, z;

        x = BootCameraRot[i].x;
        y = BootCameraRot[i].y;
        z = BootCameraRot[i].z;

        pos.x = (((HuSin(y) * HuCos(x)) * BootCameraZoom[i]) + BootCameraCenter[i].x);
        pos.y = (-HuSin(x) * BootCameraZoom[i]) + BootCameraCenter[i].y;
        pos.z = ((HuCos(y) * HuCos(x)) * BootCameraZoom[i]) + BootCameraCenter[i].z;
        target.x = BootCameraCenter[i].x;
        target.y = BootCameraCenter[i].y;
        target.z = BootCameraCenter[i].z;
        up.x = HuSin(y) * HuSin(x);
        up.y = HuCos(x);
        up.z = HuCos(y) * HuSin(x);
        Hu3DCameraPosSet(cameraMask[i], pos.x, pos.y, pos.z, up.x, up.y, up.z, target.x, target.y, target.z);
    }
}

void CameraMove(OMOBJ *obj)
{
    Vec pos;
    Vec offset;
    Vec dir;
    Vec yOfs;

    f32 rotZ;
    s8 stickPos;
    static BOOL moveF = FALSE;

    if(moveF) {
        BootCameraRot[0].y += 0.1f * HuPadStkX[0];
        BootCameraRot[0].x += 0.1f * HuPadStkY[0];
        BootCameraZoom[0] += HuPadTrigL[0] / 2;
        BootCameraZoom[0] -= HuPadTrigR[0] / 2;
        if (BootCameraZoom[0] < 100.0f) {
            BootCameraZoom[0] = 100.0f;
        }
        pos.x = BootCameraCenter[0].x + (BootCameraZoom[0] * (HuSin(BootCameraRot[0].y) * HuCos(BootCameraRot[0].x)));
        pos.y = (BootCameraCenter[0].y + (BootCameraZoom[0] * -HuSin(BootCameraRot[0].x)));
        pos.z = (BootCameraCenter[0].z + (BootCameraZoom[0] * (HuCos(BootCameraRot[0].y) * HuCos(BootCameraRot[0].x))));
        offset.x = BootCameraCenter[0].x - pos.x;
        offset.y = BootCameraCenter[0].y - pos.y;
        offset.z = BootCameraCenter[0].z - pos.z;
        dir.x = (HuSin(BootCameraRot[0].y) * HuSin(BootCameraRot[0].x));
        dir.y = HuCos(BootCameraRot[0].x);
        dir.z = (HuCos(BootCameraRot[0].y) * HuSin(BootCameraRot[0].x));
        rotZ = BootCameraRot[0].z;
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
            BootCameraCenter[0].x += 0.05f * (offset.x * stickPos);
            BootCameraCenter[0].y += 0.05f * (offset.y * stickPos);
            BootCameraCenter[0].z += 0.05f * (offset.z * stickPos);
        }
        VECNormalize(&yOfs, &offset);
        stickPos = -(HuPadSubStkY[0] & 0xF8);
        if (stickPos != 0) {
            BootCameraCenter[0].x += 0.05f * (offset.x * stickPos);
            BootCameraCenter[0].y += 0.05f * (offset.y * stickPos);
            BootCameraCenter[0].z += 0.05f * (offset.z * stickPos);
        }
    }
}

void BootTitleCreate(void)
{
    TitleMdlId[0] = Hu3DModelCreateData(TITLE_HSF_title);
    TitleMdlId[1] = Hu3DModelCreateData(TITLE_HSF_pressStart);
    TitleMdlId[2] = Hu3DModelCreateData(TITLE_HSF_back);
    Hu3DModelDispOff(TitleMdlId[0]);
    Hu3DModelDispOff(TitleMdlId[1]);
    Hu3DModelDispOff(TitleMdlId[2]);
    Hu3DModelLayerSet(TitleMdlId[0], 1);
}

BOOL BootTitleExec(BOOL fromTitle)
{
    s16 time;
    Hu3DCameraPerspectiveSet(HU3D_CAM0, 20, 20, 5000, HU_DISP_ASPECT);
    Hu3DCameraViewportSet(HU3D_CAM0, 0, 0, HU_FB_WIDTH, HU_FB_HEIGHT, 0, 1);
    BootCameraRot[0].x = 0;
    BootCameraRot[0].y = 0;
    BootCameraRot[0].z = 0;
    BootCameraCenter[0].x = 0;
    BootCameraCenter[0].y = 0;
    BootCameraCenter[0].z = 0;
    BootCameraZoom[0] = 1000;
    Hu3DModelDispOn(TitleMdlId[2]);
    Hu3DMotionTimeSet(TitleMdlId[2], 0);
    Hu3DMotionStartEndSet(TitleMdlId[2], 0, 71);
    Hu3DModelAttrReset(TitleMdlId[2], HU3D_MOTATTR_LOOP);
    Hu3DModelDispOn(TitleMdlId[1]);
    Hu3DMotionTimeSet(TitleMdlId[1], 0);
    Hu3DModelAttrSet(TitleMdlId[0], HU3D_MOTATTR_LOOP);
    Hu3DMotionStartEndSet(TitleMdlId[0], 0, 59);
    if(!fromTitle) {
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
        HuPrcSleep(10);
        HuAudSStreamPlay(1);
        WipeWait();
        while(Hu3DMotionTimeGet(TitleMdlId[2]) < 70) {
            HuPrcVSleep();
        }
        Hu3DMotionStartEndSet(TitleMdlId[2], 70, 500);
    } else {
        Hu3DMotionTimeSet(TitleMdlId[2], 71);
        Hu3DMotionStartEndSet(TitleMdlId[2], 70, 500);
        Hu3DMotionTimeSet(TitleMdlId[1], 70);
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
        HuPrcSleep(10);
        HuAudSStreamPlay(1);
        WipeWait();
    }
    Hu3DModelAttrSet(TitleMdlId[2], HU3D_MOTATTR_LOOP);
    Hu3DModelDispOn(TitleMdlId[0]);
    for(time=0; time<2100; time++) {
        if(HuPadBtnDown[0] & PAD_BUTTON_START) {
            HuAudFXPlay(1180);
            Hu3DMotionStartEndSet(TitleMdlId[0], 0, 90);
            Hu3DModelAttrReset(TitleMdlId[0], HU3D_MOTATTR_LOOP);
            Hu3DMotionTimeSet(TitleMdlId[0], 60);
            HuPrcSleep(20);
            return TRUE;
        }
        HuPrcVSleep();
    }
    WipeColorSet(255, 255, 255);
    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
    WipeWait();
    Hu3DModelDispOff(TitleMdlId[0]);
    Hu3DModelDispOff(TitleMdlId[2]);
    return FALSE;
}

extern u32 nintendoData[];
extern u32 warningData[];
extern u32 warningPressStartData[];

void *NintendoDataDecode(void)
{
    u32 *src = nintendoData;
    u32 size = *src++;
    void *dst = HuMemDirectMalloc(HEAP_MODEL, size);
    u32 type = *src++;
    if(dst) {
        HuDecodeData(src, dst, size, type);
    }
    return dst;
}

void *WarningDataDecode(u32 *src)
{
    u32 size = *src++;
    void *dst = HuMemDirectMalloc(HEAP_MODEL, size);
    u32 type = *src++;
    if(dst) {
        HuDecodeData(src, dst, size, type);
    }
    return dst;
}

void BootWarningExec(void)
{
    float scaleX = 0.94736844f;
    float scaleY = 1.0714285f;
    HUSPR_GROUPID gid;
    HUSPRID sprid;
    s16 dir;
    ANIMDATA *anim;
    float unused;
    float tpLvl;
    s16 time;

    gid = HuSprGrpCreate(2);
    anim = HuSprAnimRead(WarningDataDecode(warningData));
    sprid = HuSprCreate(anim, 0, 0);
    HuSprGrpMemberSet(gid, 0, sprid);
    HuSprPosSet(gid, 0, HU_DISP_CENTERX, 224*scaleY);
    HuSprScaleSet(gid, 0, scaleX, scaleY);
    anim = HuSprAnimRead(WarningDataDecode(warningPressStartData));
    sprid = HuSprCreate(anim, 0, 0);
    HuSprGrpMemberSet(gid, 1, sprid);
    HuSprPosSet(gid, 1, HU_DISP_CENTERX, 383*scaleY);
    HuSprDispOff(gid, 1);
    HuSprScaleSet(gid, 1, scaleX, scaleY);
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
    WipeWait();
    HuPrcSleep(90);
    unused = 0;
    dir = 1;
    HuSprDispOn(gid, 1);
    for(time=0; time<3510 && !HU_PAD_BTNDOWN_ALL; time++) {
        if(time != 0 && time%30 == 0) {
            dir = -dir;
        }
        tpLvl = (time%30)/30.0f;
        if(dir < 0) {
            tpLvl = 1.0f-tpLvl;
        }
        HuSprTPLvlSet(gid, 1, tpLvl);
        HuPrcVSleep();
    }
    HuSprDispOff(gid, 1);
    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
    WipeWait();
    HuSprGrpKill(gid);
    HuPrcSleep(30);
}
