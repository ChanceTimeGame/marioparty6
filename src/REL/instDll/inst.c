/* @@HEAD@@ */
#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mgdata.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/saveload.h"
#include "game/sprite.h"
#include "game/thpmain.h"
#include "game/window.h"
#include "game/wipe.h"
#include "humath.h"
#include "math.h"
#include "dolphin.h"

#include "datadir_enum.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* lbl_1_data_520: model table read by fn_1_4668 (0x30 per entry) */
typedef struct InstModelData_s {
    u32 dataNum; /* 0x00 */
    s16 motNo; /* 0x04 */
    HuVecF pos; /* 0x08 */
    HuVecF rot; /* 0x14 */
    HuVecF scale; /* 0x20 */
    s16 layer; /* 0x2C */
} INST_MODEL_DATA; /* size 0x30 */

/* .bss is laid out in reverse definition order */
s16 lbl_1_bss_6AA[256];
s16 lbl_1_bss_4AA[256];
s16 lbl_1_bss_49E[6];
u8 lbl_1_bss_43A[0x64];
s16 lbl_1_bss_438;
s16 lbl_1_bss_238[256];
s16 lbl_1_bss_D8[176];
s16 lbl_1_bss_D4;
u8 lbl_1_bss_84[0x50];
u8 lbl_1_bss_78[0xC];
s32 lbl_1_bss_70[2];
s16 lbl_1_bss_6E;
s16 lbl_1_bss_6C;
s16 lbl_1_bss_66[3];
u8 lbl_1_bss_5C[0xA];
s32 lbl_1_bss_58;
s32 lbl_1_bss_50[2];
s32 lbl_1_bss_4C;
float lbl_1_bss_48;
OMOBJ *lbl_1_bss_40[2];
OMOBJMAN *lbl_1_bss_3C;
s16 lbl_1_bss_38;
u16 lbl_1_bss_36;
u8 lbl_1_bss_34;
s32 lbl_1_bss_30;
s16 lbl_1_bss_28[4];
s16 lbl_1_bss_26;
s16 lbl_1_bss_24;
MG_DATA *lbl_1_bss_20;
s16 lbl_1_bss_1C;
u32 *lbl_1_bss_18;
s16 lbl_1_bss_14;
s32 lbl_1_bss_10;
s32 lbl_1_bss_C;
s16 lbl_1_bss_A;
s16 lbl_1_bss_8;
s16 lbl_1_bss_6;
s16 lbl_1_bss_4;
void *lbl_1_bss_0;

GXColor lbl_1_data_0 = { 20, 0, 0, 0 };
s32 lbl_1_data_4 = 1;
OM_CAMERA_VIEW lbl_1_data_8 = { { 0, 0, 0 }, { 0, 0, 0 }, 7000 };
OM_CAMERA_VIEW lbl_1_data_24 = { { 0, 0, 0 }, { 0, 0, 0 }, 3000 };
OM_CAMERA_VIEW lbl_1_data_40 = { { 170, 105, 0 }, { 0, 0, 0 }, 100 };

/* Defined after fn_1_4268 (.data 0xF8 onward) */
extern INST_MODEL_DATA lbl_1_data_520[];
extern HuVecF lbl_1_data_700;
extern HuVecF lbl_1_data_70C;
extern HuVecF lbl_1_data_718;
extern HuVecF lbl_1_data_724;

void ObjectSetup(void);
void fn_1_1024(void);
void fn_1_15B4(void);
void fn_1_15C8(void);
void fn_1_2A1C(void);
void fn_1_35F8(OMOBJ *obj);
void fn_1_4158(void);
void fn_1_4188(void);
void fn_1_4268(void);
void fn_1_4668(INST_MODEL_DATA *data);
void fn_1_47E4(s16 arg0);
void fn_1_539C(void);
void fn_1_5DE0(void);
void fn_1_8DB0(void);
void fn_1_8FA0(s16 arg0);
void fn_1_96F4(s16 arg0);
void fn_1_A1E8(void (*func)(void), s16 arg1);

/* @@PART1@@ */
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

void ObjectSetup(void)
{
    int i;
    OMOVLHIS *his;
    int num;
    HU3D_LIGHTID lightId;

    OSReport("******* INST ObjectSetup *********\n");
    lbl_1_bss_3C = omInitObjMan(50, 8192);
    lbl_1_bss_34 = GWLanguageGet();
    lbl_1_bss_36 = GwSystem.mgNo;
    if (lbl_1_bss_36 >= 82) {
        lbl_1_bss_36 = 0;
    }
    GWMgUnlockSet(lbl_1_bss_36 + 601);
    lbl_1_bss_20 = &MgDataTbl[lbl_1_bss_36];
    if (lbl_1_bss_20->flag & MG_FLAG_RARE) {
        lbl_1_bss_20->type = MG_TYPE_LAST;
    }
    if (_CheckFlag(FLAG_INST_DECA)) {
        lbl_1_bss_18 = lbl_1_bss_20->instMes[MG_INST_TYPE_DECATHLON];
    } else if (GWMgTimeGet() == MG_TIME_DAY) {
        lbl_1_bss_18 = lbl_1_bss_20->instMes[MG_INST_TYPE_DAY];
    } else {
        lbl_1_bss_18 = lbl_1_bss_20->instMes[MG_INST_TYPE_NIGHT];
    }
    lbl_1_bss_38 = 0;
    lbl_1_bss_4 = 0;
    _ClearFlag(FLAG_MG_PAUSE_OFF);
    for (i = 0; sndGrpTable[i].ovl != -1; i++) {
        if (sndGrpTable[i].ovl == MgDataTbl[lbl_1_bss_36].ovl) {
            break;
        }
    }
    if (sndGrpTable[i].ovl != -1) {
        HuAudSndGrpSetSet(sndGrpTable[i].grpSet);
    }
    if (!_CheckFlag(FLAG_INST_MG_MODE) && !_CheckFlag(FLAG_MG_PRACTICE)) {
        GWSubGameNoSet(-1);
        MgSubMode = 0;
    }
    lbl_1_bss_14 = GWSubGameNoGet();
    if (lbl_1_bss_14 < 0 || !lbl_1_bss_20->instPic[0][lbl_1_bss_14]) {
        lbl_1_bss_14 = 0;
    }
    if (lbl_1_bss_20->ovl == 0x53) {
        for (i = num = 0; i < GW_PLAYER_MAX; i++) {
            if (GwPlayerConf[i].grpNo == 0) {
                num++;
            }
        }
        if (num == 0 || num >= 3) {
            GwPlayerConf[0].grpNo = 0;
            GwPlayerConf[1].grpNo = GwPlayerConf[2].grpNo = GwPlayerConf[3].grpNo = 1;
            GWSubGameNoSet(0);
            lbl_1_bss_14 = 0;
        } else if (num == 1) {
            GWSubGameNoSet(0);
            lbl_1_bss_14 = 0;
        } else {
            GWSubGameNoSet(1);
            lbl_1_bss_14 = 1;
            lbl_1_bss_18 = lbl_1_bss_20->instMes[MG_INST_TYPE_NIGHT];
        }
    }
    if ((omprevovl >= 6 && omprevovl <= 0x57 && !_CheckFlag(FLAG_MG_PRACTICE) && _CheckFlag(FLAG_INST_MG_MODE)) || omovlevtno == 2) {
        if (lbl_1_bss_20->flag & MG_FLAG_TEAM_ORDER) {
            for (i = 0; i < GW_PLAYER_MAX; i++) {
                GwPlayerConf[i].grpNo = GwPlayerConf[i].grpNo / 2;
            }
        }
        HuDataDirClose(MgDataTbl[lbl_1_bss_36].dataDir);
        if (_CheckFlag(FLAGNUM(FLAG_GROUP_SYSTEM, 4))) {
            HuPrcChildCreate(fn_1_4158, 100, 0x3000, 0, HuPrcCurrentGet());
        } else {
            omOvlReturnEx(1, 1);
        }
        return;
    }
    _ClearFlag(FLAG_MG_PRACTICE);
    if (!_CheckFlag(FLAG_INST_MG_MODE) && !_CheckFlag(FLAG_INST_NO_HISCHG)) {
        his = omOvlHisGet(0);
        if (lbl_1_bss_20->type != MG_TYPE_4P && lbl_1_bss_20->type != MG_TYPE_1VS3 && lbl_1_bss_20->type != MG_TYPE_2VS2 && lbl_1_bss_20->type != MG_TYPE_BATTLE) {
            omOvlHisChg(0, 4, 2, his->stat);
        } else {
            omOvlHisChg(0, 0x71, his->evtno, his->stat);
        }
    }
    if (lbl_1_bss_20->type == MG_TYPE_4P) {
        for (i = 0; i < GW_PLAYER_MAX; i++) {
            GwPlayerConf[i].grpNo = i;
        }
    }
    if (omovlevtno == 0) {
        int grpMap[GW_PLAYER_MAX][GW_PLAYER_MAX];
        int grpNum[2];
        if (lbl_1_bss_20->flag & MG_FLAG_TEAM_ORDER) {
            grpNum[0] = grpNum[1] = 0;
            for (i = 0; i < GW_PLAYER_MAX; i++) {
                if (GwPlayerConf[i].grpNo >= 2) {
                    break;
                }
            }
            if (i == GW_PLAYER_MAX) {
                for (i = 0; i < GW_PLAYER_MAX; i++) {
                    grpMap[GwPlayerConf[i].grpNo][grpNum[GwPlayerConf[i].grpNo]++] = i;
                }
                if (frand() & 1) {
                    GwPlayerConf[grpMap[0][0]].grpNo = 0;
                    GwPlayerConf[grpMap[0][1]].grpNo = 1;
                } else {
                    GwPlayerConf[grpMap[0][0]].grpNo = 1;
                    GwPlayerConf[grpMap[0][1]].grpNo = 0;
                }
                if (frand() & 1) {
                    GwPlayerConf[grpMap[1][0]].grpNo = 2;
                    GwPlayerConf[grpMap[1][1]].grpNo = 3;
                } else {
                    GwPlayerConf[grpMap[1][0]].grpNo = 3;
                    GwPlayerConf[grpMap[1][1]].grpNo = 2;
                }
            }
        } else if (MgDataTbl[lbl_1_bss_36].type == MG_TYPE_1VS3) {
            grpNum[0] = grpNum[1] = 0;
            for (i = 0; i < GW_PLAYER_MAX; i++) {
                if (GwPlayerConf[i].grpNo > 1) {
                    break;
                }
                if (GwPlayerConf[i].grpNo < 0) {
                    break;
                }
                grpNum[GwPlayerConf[i].grpNo]++;
            }
            if (i != GW_PLAYER_MAX || grpNum[0] != 1 || grpNum[1] != 3) {
                OSReport("Error: Illegal Group pattern.Adjusted.\n");
                GwPlayerConf[0].grpNo = 0;
                GwPlayerConf[1].grpNo = GwPlayerConf[2].grpNo = GwPlayerConf[3].grpNo = 1;
            }
        } else if (MgDataTbl[lbl_1_bss_36].type == MG_TYPE_2VS2) {
            grpNum[0] = grpNum[1] = 0;
            for (i = 0; i < GW_PLAYER_MAX; i++) {
                if (GwPlayerConf[i].grpNo > 1) {
                    break;
                }
                if (GwPlayerConf[i].grpNo < 0) {
                    break;
                }
                grpNum[GwPlayerConf[i].grpNo]++;
            }
            if (i != GW_PLAYER_MAX || grpNum[0] != 2 || grpNum[1] != 2) {
                OSReport("Error: Illegal Group pattern.Adjusted.\n");
                GwPlayerConf[0].grpNo = GwPlayerConf[1].grpNo = 0;
                GwPlayerConf[2].grpNo = GwPlayerConf[3].grpNo = 1;
            }
        }
    }
    if (!_CheckFlag(FLAGNUM(FLAG_GROUP_SAVE, 5)) || !GwSystem.mgInstDispF) {
        GWMgPlayNumAdd(1);
        HuDataDirClose(DATA_inst);
        omSysPauseEnable(TRUE);
        omOvlCallEx(MgDataTbl[lbl_1_bss_36].ovl, 1, 0, 0);
        return;
    }
    HuDataDirClose(MgDataTbl[lbl_1_bss_36].dataDir);
    omSysPauseEnable(FALSE);
    Hu3DCameraCreate(HU3D_CAM0 | HU3D_CAM1);
    Hu3DCameraPerspectiveSet(HU3D_CAM0, 5, 20, 15000, 1.2f);
    Hu3DCameraViewportSet(HU3D_CAM0, 0, 0, 640, 480, 0, 1);
    Hu3DCameraPerspectiveSet(HU3D_CAM1, 10, 20, 15000, 1.2f);
    Hu3DCameraViewportSet(HU3D_CAM1, 0, 0, 640, 480, 0, 1);
    omCameraViewSetMulti(HU3D_CAM0, &lbl_1_data_8);
    omCameraViewSetMulti(HU3D_CAM1, &lbl_1_data_24);
    lightId = Hu3DGLightCreate(0, 100, 1000, 0, -0.5f, -1, 255, 255, 255);
    Hu3DGLightInfinitytSet(lightId);
    lbl_1_bss_40[0] = omAddObjEx(lbl_1_bss_3C, 32730, 0, 0, -1, omOutViewMulti);
    lbl_1_bss_40[0]->work[0] = 2;
    omAddObjEx(lbl_1_bss_3C, 0, 32, 32, -1, fn_1_35F8);
    HuAudSStreamStop(0);
    HuAudSStreamPlay(64);
    Hu3DBGColorSet(0, 0, 0);
    HuWinInit(1);
    HuPrcChildCreate(fn_1_1024, 1000, 0x3000, 0, lbl_1_bss_3C);
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        lbl_1_bss_28[i] = GwPlayerConf[i].charNo;
    }
    Hu3DShadowCreate(15, 20, 5000);
    {
        HuVecF pos;
        HuVecF up;
        HuVecF target;
        pos = lbl_1_data_700;
        pos.y += 1000;
        pos.z += 1000;
        up.y = 1;
        up.x = up.z = 0;
        target = lbl_1_data_700;
        Hu3DShadowPosSet(&pos, &up, &target);
    }
    Hu3DShadowTPLvlSet(0.3f);
    if (omprevovl >= 6 && omprevovl <= 0x57) {
        lbl_1_bss_6 = 2;
    } else {
        lbl_1_bss_6 = 0;
    }
    lbl_1_bss_1C = GWMgTimeGet();
    if ((lbl_1_bss_20->flag & MG_FLAG_DISABLE_DAY || lbl_1_bss_20->type == MG_TYPE_KOOPA) && MgInstExitF) {
        lbl_1_bss_1C = 1;
    }
}

void fn_1_1024(void)
{
    s16 i;
    BOOL exitF;
    BOOL comInputF;

    exitF = FALSE;
    fn_1_8FA0(lbl_1_bss_1C);
    fn_1_96F4(lbl_1_bss_1C);
    fn_1_4668(lbl_1_data_520);
    fn_1_539C();
    fn_1_5DE0();
    fn_1_47E4(lbl_1_bss_1C);
    fn_1_A1E8(fn_1_2A1C, lbl_1_bss_1C);
    HuPrcChildCreate(fn_1_15C8, 100, 0x3000, 0, HuPrcCurrentGet());
    HuPrcVSleep();
    HuPrcChildCreate(fn_1_4188, 2000, 0x3000, 0, HuPrcCurrentGet());
    HuDataDirClose(DATA_inst);
    CharModelDataClose(-1);
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 40);
    WipeWait();
    if (lbl_1_bss_6 != 2) {
        lbl_1_bss_6 = 1;
        HuPrcSleep(30);
    }
    lbl_1_bss_6 = 2;
    comInputF = FALSE;
    if (lbl_1_bss_20->type != MG_TYPE_KETTOU) {
        for (i = 0; i < GW_PLAYER_MAX; i++) {
            if (GwPlayerConf[i].type == GW_PLAYER_TYPE_MAN) {
                break;
            }
        }
        if (i == GW_PLAYER_MAX) {
            comInputF = TRUE;
        }
    } else {
        for (i = 0; i < GW_PLAYER_MAX; i++) {
            if ((GwPlayerConf[i].grpNo == 0 || GwPlayerConf[i].grpNo == 1) && GwPlayerConf[i].type == GW_PLAYER_TYPE_MAN) {
                break;
            }
        }
        if (i == GW_PLAYER_MAX) {
            comInputF = TRUE;
        }
    }
    lbl_1_bss_30 = (_CheckFlag(FLAG_INST_DECA)) ? FALSE : TRUE;
    while (1) {
        fn_1_8DB0();
        if (lbl_1_bss_8 == 0) {
            if (lbl_1_bss_24 & PAD_BUTTON_START) {
                HuAudFXPlay(2);
                GWMgPlayNumAdd(1);
                break;
            }
            if (lbl_1_bss_24 & PAD_TRIGGER_Z) {
                OMOVLHIS *his;
                HuAudFXPlay(2);
                his = omOvlHisGet(0);
                omOvlHisChg(0, 4, 1, his->stat);
                _SetFlag(FLAG_MG_PRACTICE);
                break;
            }
        }
        if (MgInstExitF) {
            if (lbl_1_bss_8 == 0 && (lbl_1_bss_24 & PAD_BUTTON_B)) {
                HuAudFXPlay(3);
                exitF = TRUE;
                break;
            }
            if (!lbl_1_bss_30 && (lbl_1_bss_20->flag & MG_FLAG_HAS_NIGHT) && (lbl_1_bss_24 & PAD_BUTTON_X) && lbl_1_bss_8 == 0 && lbl_1_bss_C == 0) {
                fn_1_15B4();
                HuAudFXPlay(1);
            }
            if (lbl_1_bss_8 != 0) {
                lbl_1_bss_8--;
                if (lbl_1_bss_8 == 0) {
                    lbl_1_bss_1C = (lbl_1_bss_1C == 0) ? 1 : 0;
                    GWMgTimeSet(lbl_1_bss_1C);
                }
            }
        }
        if (comInputF) {
            HuPrcSleep(60);
            break;
        }
        HuPrcVSleep();
    }
    if (!exitF) {
        lbl_1_bss_6 = 3;
        omCameraViewMoveMulti(HU3D_CAM1, &lbl_1_data_40, 60, 1);
        HuPrcSleep(20);
        HuAudFXPlay(56);
    }
    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 40);
    HuAudSStreamAllFadeOut(1000);
    WipeWait();
    HuTHPClose();
    while (HuTHPProcCheck()) {
        HuTHPClose();
        HuPrcVSleep();
    }
    if (!exitF) {
        while (!lbl_1_data_4) {
            HuPrcVSleep();
        }
        omOvlCallEx(lbl_1_bss_20->ovl, 1, 0, 0);
    } else {
        if (!HuDataGetAsyncStat(lbl_1_bss_A)) {
            HuDataDirCancel(lbl_1_bss_A);
        } else {
            HuDataDirClose(MgDataTbl[lbl_1_bss_36].dataDir);
        }
        omOvlReturnEx(1, 1);
    }
    HuPrcEnd();
    while (1) {
        HuPrcVSleep();
    }
}

void fn_1_15B4(void)
{
    lbl_1_bss_8 = 20;
}

/* @@PART2@@ */
/* @@PART3@@ */
/* @@END@@ */
