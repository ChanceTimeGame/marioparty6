/* @@HEAD@@ */
/* Retail calls HuWinInsertMesSet unprototyped (extsh on its s16 argument) */
#define HuWinInsertMesSet HuWinInsertMesSet_proto
#include "game/window.h"
#undef HuWinInsertMesSet
void HuWinInsertMesSet();
#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mgdata.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/sprite.h"
#include "game/thpmain.h"
#include "game/wipe.h"
#include "humath.h"
#include "math.h"
#include "dolphin.h"

#include "datadir_enum.h"

/* SLSaveModeExec is called unprototyped (fn_1_4158): game/saveload.h stays out of this unit */

/* frand is called unprototyped: game/frand.h stays out of this unit */

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* lbl_1_data_520: model table read by fn_1_4668 (0x30 per entry) */
typedef struct InstModelData_s {
    u32 dataNum; /* 0x00 */
    s16 mdlNo; /* 0x04 -1: a model; else the entry whose model this motion belongs to */
    HuVecF pos; /* 0x08 */
    HuVecF rot; /* 0x14 */
    HuVecF scale; /* 0x20 */
    s16 cameraBit; /* 0x2C */
} INST_MODEL_DATA; /* size 0x30 */

/* lbl_1_data_798: the stand layout per minigame type (0x70 per entry) */
typedef struct InstTypeData_s {
    u8 num; /* 0x00 */
    u32 dataNum[4]; /* 0x04 */
    float rotY; /* 0x14 */
    float scale; /* 0x18 */
    float playerRot[5]; /* 0x1C */
    u8 playerBit[4]; /* 0x30 */
    HuVecF playerPos[5]; /* 0x34 */
} INST_TYPE_DATA; /* size 0x70 */

/* lbl_1_bss_D8: one stand (0x58 each) */
typedef struct InstStand_s {
    HU3D_MODELID mdlId; /* 0x00 */
    u16 time; /* 0x02 */
    float angle; /* 0x04 */
    u8 unk08[12]; /* 0x08 */
    s8 player[5]; /* 0x14 */
    HuVecF playerPos[5]; /* 0x1C */
} INST_STAND; /* size 0x58 */

typedef INST_TYPE_DATA INST_STAGETBL;
typedef INST_STAND INST_STAGE;

/* .bss is laid out in reverse definition order */
s16 lbl_1_bss_6AA[256];
s16 lbl_1_bss_4AA[256];
HU3D_MODELID lbl_1_bss_49E[6];
HU3D_MOTIONID lbl_1_bss_43A[5][10];
s16 lbl_1_bss_438;
s16 lbl_1_bss_238[256];
INST_STAND lbl_1_bss_D8[4];
s16 lbl_1_bss_D4;
s32 lbl_1_bss_84[20];
s16 lbl_1_bss_78[6];
ANIMDATA *lbl_1_bss_70[2];
s16 lbl_1_bss_6E;
HU3D_MODELID lbl_1_bss_6C;
s16 lbl_1_bss_68[2];
HUSPR_GROUPID lbl_1_bss_66;
s16 lbl_1_bss_64;
ANIMDATA *lbl_1_bss_5C[2];
ANIMDATA *lbl_1_bss_58;
ANIMDATA *lbl_1_bss_50[2];
void *lbl_1_bss_4C;
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
s32 lbl_1_bss_0;

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
void fn_1_96F4(s16 nightF);
HUPROCESS *fn_1_A1E8(void (*func)(void), s32 param);

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
    int lightId;

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
            return;
        } else {
            omOvlReturnEx(1, 1);
            return;
        }
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
        int grpNum[GW_PLAYER_MAX];
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
    if (!_CheckFlag(FLAGNUM(FLAG_GROUP_SAVE, 5)) || !GWMgInstDispFGet()) {
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
    WipeCreate(WIPE_MODE_IN, 5, 40);
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
    lbl_1_bss_30 = (_CheckFlag(FLAGNUM(FLAG_GROUP_SYSTEM, 4))) ? FALSE : TRUE;
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

u32 lbl_1_data_A8[5] = { 0x420002, 0x420003, 0x420004, 0x420005, 0x420006 };

void fn_1_15C8(void)
{
    s16 grpChar[GW_PLAYER_MAX][GW_PLAYER_MAX];
    s16 winTbl[2][4];
    HuVec2f size0;
    HuVec2f size1;
    HuVec2f pos0;
    HuVec2f pos1;
    s16 cnt[GW_PLAYER_MAX];
    s16 win0[2];
    s16 win1[2];
    s16 i;
    s16 page;
    s16 j;
    u32 mes2;
    s16 k;
    u32 mes;
    s16 insertNo;
    s16 time;
    s16 frame;
    s16 maxTime;
    float t;

    time = lbl_1_bss_1C;
    if (time == 0) {
        frame = 4;
    } else {
        frame = 3;
    }
    for (j = 0; j < 2; j++) {
        if (j == 0) {
            frame = 4;
        } else {
            frame = 3;
        }
        for (i = 0; i < 2; i++) {
            size0.x = 400;
            size0.y = 128;
            pos0.x = 560 - size0.x;
            pos0.y = 440 - size0.y;
            win0[i] = HuWinCreate(pos0.x, pos0.y, size0.x, size0.y, frame);
            HuWinMesSpeedSet(win0[i], 0);
            HuWinMesSet(win0[i], *lbl_1_bss_18);
            size1.x = 336;
            size1.y = 48;
            pos1.x = 560 - size1.x;
            pos1.y = 440 - size0.y - size1.y;
            win1[i] = HuWinCreate(pos1.x, pos1.y, size1.x, size1.y, frame);
            HuWinAttrSet(win1[i], 0x800);
            HuWinMesSpeedSet(win1[i], 0);
            HuWinMesSet(win1[i], lbl_1_data_A8[0]);
        }
        winTbl[j][0] = win0[0];
        winTbl[j][1] = win1[0];
        winTbl[j][2] = win0[1];
        winTbl[j][3] = win1[1];
        for (i = 0; i < 4; i++) {
            HuWinDispOff(winTbl[j][i]);
        }
    }
    if (time == 0) {
        win0[0] = winTbl[0][0];
        win1[0] = winTbl[0][1];
        win0[1] = winTbl[0][2];
        win1[1] = winTbl[0][3];
        for (i = 0; i < 4; i++) {
            HuWinDispOn(winTbl[0][i]);
        }
    } else {
        win0[0] = winTbl[1][0];
        win1[0] = winTbl[1][1];
        win0[1] = winTbl[1][2];
        win1[1] = winTbl[1][3];
        for (i = 0; i < 4; i++) {
            HuWinDispOn(winTbl[1][i]);
        }
    }
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        cnt[i] = 0;
    }
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        grpChar[GwPlayerConf[i].grpNo][cnt[GwPlayerConf[i].grpNo]] = GwPlayerConf[i].charNo;
        cnt[GwPlayerConf[i].grpNo]++;
    }
    for (i = insertNo = 0; i < GW_PLAYER_MAX; i++) {
        for (j = 0; j < cnt[i]; j++) {
            HuWinInsertMesSet(winTbl[0][0], grpChar[i][j], insertNo);
            HuWinInsertMesSet(winTbl[0][2], grpChar[i][j], insertNo);
            HuWinInsertMesSet(winTbl[1][0], grpChar[i][j], insertNo);
            HuWinInsertMesSet(winTbl[1][2], grpChar[i][j], insertNo);
            insertNo++;
        }
    }
    HuWinDispOff(win0[1]);
    HuWinDispOff(win1[1]);
    lbl_1_bss_C = 1;
    if (lbl_1_bss_6 == 0 || lbl_1_bss_6 == 1) {
        HuWinPosSet(win1[0], 1000, 0);
        HuWinPosSet(win0[0], 1000, 0);
        while (lbl_1_bss_6 != 1) {
            HuPrcVSleep();
        }
        HuPrcSleep(20);
        for (i = 0; i <= 20; i++) {
            HuWinPosSet(win1[0], pos1.x + 400.0 * (1.0 - HuSin(90.0 * (i / 20.0))), pos1.y);
            HuWinPosSet(win0[0], pos0.x, pos0.y + 200.0 * (1.0 - HuSin(90.0 * (i / 20.0))));
            HuPrcVSleep();
        }
    }
    page = 0;
    mes2 = lbl_1_data_A8[0];
    mes = lbl_1_bss_18[0];
    lbl_1_bss_C = 0;
    while (1) {
        if (lbl_1_bss_8 == 0) {
            if (lbl_1_bss_24 & 0x2020) {
                lbl_1_bss_C = 1;
                lbl_1_bss_10 = 1;
                HuWinPriSet(win1[0], 100);
                HuWinPriSet(win0[0], 100);
                HuWinPriSet(win1[1], 50);
                HuWinPriSet(win0[1], 50);
                HuWinMesSet(win1[1], mes2);
                HuWinMesSet(win0[1], mes);
                HuWinDispOn(win0[1]);
                HuWinDispOn(win1[1]);
                HuWinPosSet(win1[1], 1000, 0);
                HuWinPosSet(win0[1], 1000, 0);
                HuPrcVSleep();
                page++;
                if (page > 4) {
                    page = 0;
                }
                if (page == 2 && lbl_1_bss_18[2] == 0) {
                    page++;
                }
                if (page == 4 && lbl_1_bss_18[4] == 0) {
                    page = 0;
                }
                mes2 = lbl_1_data_A8[page];
                if (page == 1 && lbl_1_bss_18[2] == 0) {
                    mes2 = 0x420007;
                }
                if (page == 3 && lbl_1_bss_18[4] == 0) {
                    mes2 = 0x420008;
                }
                HuWinMesSet(win1[0], mes2);
                mes = lbl_1_bss_18[page];
                HuWinMesSet(win0[0], mes);
                HuAudFXPlay(55);
                for (i = 0; i < 15; i++) {
                    t = i / 15.0;
                    HuWinPosSet(win1[1], pos1.x + 30.0 * HuSin(90.0f * t), pos1.y + 200.0 * (1.0 - HuCos(90.0f * t)));
                    HuWinPosSet(win0[1], pos0.x + 30.0 * HuSin(90.0f * t), pos0.y + 200.0 * (1.0 - HuCos(90.0f * t)));
                    HuPrcVSleep();
                }
                HuWinDispOff(win0[1]);
                HuWinDispOff(win1[1]);
                lbl_1_bss_C = 0;
            } else if (lbl_1_bss_24 & 0x4040) {
                lbl_1_bss_C = 1;
                lbl_1_bss_10 = 1;
                HuWinPriSet(win1[0], 50);
                HuWinPriSet(win0[0], 50);
                HuWinPriSet(win1[1], 100);
                HuWinPriSet(win0[1], 100);
                HuWinMesSet(win1[1], mes2);
                HuWinMesSet(win0[1], mes);
                HuWinDispOn(win0[1]);
                HuWinDispOn(win1[1]);
                HuWinPosSet(win1[1], pos1.x, pos1.y);
                HuWinPosSet(win0[1], pos0.x, pos0.y);
                HuPrcVSleep();
                page--;
                if (page < 0) {
                    page = 4;
                }
                if (page == 2 && lbl_1_bss_18[2] == 0) {
                    page--;
                }
                if (page == 4 && lbl_1_bss_18[4] == 0) {
                    page--;
                }
                mes2 = lbl_1_data_A8[page];
                if (page == 1 && lbl_1_bss_18[2] == 0) {
                    mes2 = 0x420007;
                }
                if (page == 3 && lbl_1_bss_18[4] == 0) {
                    mes2 = 0x420008;
                }
                HuWinMesSet(win1[0], mes2);
                mes = lbl_1_bss_18[page];
                HuWinMesSet(win0[0], mes);
                HuAudFXPlay(54);
                for (i = 1; i <= 15; i++) {
                    t = i / 15.0;
                    HuWinPosSet(win1[0], pos1.x + 30.0 * HuCos(90.0f * t), pos1.y + 200.0 * (1.0 - HuSin(90.0f * t)));
                    HuWinPosSet(win0[0], pos0.x + 30.0 * HuCos(90.0f * t), pos0.y + 200.0 * (1.0 - HuSin(90.0f * t)));
                    HuPrcVSleep();
                }
                HuWinDispOff(win0[1]);
                HuWinDispOff(win1[1]);
                lbl_1_bss_C = 0;
            }
        }
        if (lbl_1_bss_8 != 0) {
            maxTime = 9;
            for (k = 1; k < maxTime; k++) {
                t = 1.0f - (float)k / (float)maxTime;
                for (i = 0; i < 2; i++) {
                    HuWinBGTPLvlSet(win0[i], t);
                    HuWinBGTPLvlSet(win1[i], t);
                }
                HuPrcVSleep();
            }
            time = (time == 0) ? TRUE : FALSE;
            if (time == 0) {
                for (i = 0; i < 4; i++) {
                    HuWinDispOff(winTbl[1][i]);
                }
                win0[0] = winTbl[0][0];
                win1[0] = winTbl[0][1];
                win0[1] = winTbl[0][2];
                win1[1] = winTbl[0][3];
                for (i = 0; i < 2; i++) {
                    HuWinBGTPLvlSet(winTbl[0][i], 0);
                    HuWinDispOn(winTbl[0][i]);
                    HuWinMesSet(win1[i], mes2);
                }
                lbl_1_bss_18 = lbl_1_bss_20->instMes[MG_INST_TYPE_DAY];
            } else {
                for (i = 0; i < 4; i++) {
                    HuWinDispOff(winTbl[0][i]);
                }
                win0[0] = winTbl[1][0];
                win1[0] = winTbl[1][1];
                win0[1] = winTbl[1][2];
                win1[1] = winTbl[1][3];
                for (i = 0; i < 2; i++) {
                    HuWinBGTPLvlSet(winTbl[1][i], 0);
                    HuWinDispOn(winTbl[1][i]);
                    HuWinMesSet(win1[i], mes2);
                }
                lbl_1_bss_18 = lbl_1_bss_20->instMes[MG_INST_TYPE_NIGHT];
            }
            mes = lbl_1_bss_18[page];
            HuWinMesSet(win0[0], mes);
            for (k = 1; k < maxTime; k++) {
                t = (float)k / (float)maxTime;
                for (i = 0; i < 2; i++) {
                    HuWinBGTPLvlSet(win0[i], t);
                    HuWinBGTPLvlSet(win1[i], t);
                }
                HuPrcVSleep();
            }
            while (lbl_1_bss_8 != 0) {
                HuPrcVSleep();
            }
        }
        if (lbl_1_bss_6 == 3) {
            break;
        }
        HuPrcVSleep();
    }
    for (i = 0; i <= 10; i++) {
        HuWinPosSet(win1[0], pos1.x + 400.0 * (1.0 - HuCos(90.0 * (i / 10.0))), pos1.y);
        HuWinPosSet(win0[0], pos0.x, pos0.y + 200.0 * (1.0 - HuCos(90.0 * (i / 10.0))));
        HuPrcVSleep();
    }
    while (1) {
        HuPrcVSleep();
    }
}

void fn_1_2A1C(void)
{
    HuVecF pos1;
    HuVecF pos2;
    s16 motId[4];
    s16 mdlId;
    s16 i;
    s32 nightF;
    s16 timer;
    HUPROCESS *proc;
    float t;

    proc = HuPrcCurrentGet();
    nightF = (s32)proc->property;
    timer = 0;
    Hu3DModelLayerSet(lbl_1_bss_6AA[0], 3);
    Hu3DMotionSet(lbl_1_bss_6AA[0], lbl_1_bss_4AA[1]);
    Hu3DModelAttrSet(lbl_1_bss_6AA[0], 0x40000001);
    Hu3DModelLayerSet(lbl_1_bss_6AA[4], 3);
    Hu3DMotionSet(lbl_1_bss_6AA[4], lbl_1_bss_4AA[5]);
    Hu3DModelAttrSet(lbl_1_bss_6AA[4], 0x40000001);
    Hu3DModelHookSet(lbl_1_bss_6AA[4], "gN01m1-itemhook_R", lbl_1_bss_6AA[8]);
    if (nightF == 0) {
        Hu3DModelAttrSet(lbl_1_bss_6AA[4], 1);
    } else {
        Hu3DModelAttrSet(lbl_1_bss_6AA[0], 1);
    }
    lbl_1_bss_10 = 0;
    if (lbl_1_bss_6 == 0 || lbl_1_bss_6 == 1) {
        if (nightF == 0) {
            mdlId = lbl_1_bss_6AA[0];
            motId[0] = lbl_1_bss_4AA[1];
            motId[1] = lbl_1_bss_4AA[2];
        } else {
            mdlId = lbl_1_bss_6AA[4];
            motId[0] = lbl_1_bss_4AA[5];
            motId[1] = lbl_1_bss_4AA[6];
        }
        Hu3DMotionSet(mdlId, motId[1]);
        Hu3DMotionTimeSet(mdlId, Hu3DMotionMaxTimeGet(mdlId));
        Hu3DModelPosSet(mdlId, -1000, 0, 0);
        Hu3DModelAttrReset(mdlId, 0x40000001);
        while (lbl_1_bss_6 != 1) {
            HuPrcVSleep();
        }
        for (i = 1; i <= 30; i++) {
            t = i / 30.0f;
            Hu3DModelPosSet(mdlId, lbl_1_data_70C.x - 200.0 * (1.0 - HuSin(90.0f * t)), lbl_1_data_70C.y + 200.0 * (1.0 - t), lbl_1_data_70C.z);
            if (i == 15) {
                Hu3DMotionShiftSet(mdlId, motId[0], 0, 15, 0x40000001);
            }
            if (i == 10) {
                if (nightF == 0) {
                    HuAudFXPlay(950);
                } else {
                    HuAudFXPlay(942);
                }
            }
            Hu3DModelRotSet(mdlId, 0, 15.0 + 30.0 * (1.0 - t), 0);
            HuPrcVSleep();
        }
    }
    while (1) {
        if (lbl_1_bss_8 != 0) {
            t = 1.0 - (lbl_1_bss_8 - 1) / 19.0f;
            Hu3DModelAttrReset(lbl_1_bss_6AA[0], 1);
            Hu3DModelAttrReset(lbl_1_bss_6AA[4], 1);
            pos1 = lbl_1_data_70C;
            pos2 = lbl_1_data_70C;
            if (nightF == 0) {
                pos1.x = pos1.x - 100 + 100.0 * HuCos(180.0f * t);
                pos2.x = pos2.x - 100 - 100.0 * HuCos(180.0f * t);
            } else {
                pos1.x = pos1.x - 100 - 100.0 * HuCos(180.0f * t);
                pos2.x = pos2.x - 100 + 100.0 * HuCos(180.0f * t);
            }
            pos1.z = pos1.z + 100.0 * HuSin(180.0f * t);
            pos2.z = pos2.z - 100.0 * HuSin(180.0f * t);
            Hu3DModelPosSetV(lbl_1_bss_6AA[0], &pos1);
            Hu3DModelPosSetV(lbl_1_bss_6AA[4], &pos2);
            if (t == 1.0) {
                nightF = (nightF == 0) ? 1 : 0;
                if (nightF == 0) {
                    HuAudFXPlay(950);
                } else {
                    HuAudFXPlay(942);
                }
            }
            if (lbl_1_bss_4AA[1] != Hu3DMotionIDGet(lbl_1_bss_6AA[0]) && lbl_1_bss_4AA[1] != Hu3DMotionShiftIDGet(lbl_1_bss_6AA[0])) {
                Hu3DMotionShiftSet(lbl_1_bss_6AA[0], lbl_1_bss_4AA[1], 0, 5, 0x40000001);
                Hu3DMotionShiftSet(lbl_1_bss_6AA[4], lbl_1_bss_4AA[5], 0, 5, 0x40000001);
                timer = 0;
            }
        } else if (lbl_1_bss_6 == 3) {
            break;
        }
        if (lbl_1_bss_10 && timer == 0) {
            timer = 80;
            lbl_1_bss_10 = 0;
            Hu3DMotionShiftSet(lbl_1_bss_6AA[0], lbl_1_bss_4AA[3], 0, 10, 0x40000001);
            Hu3DMotionShiftSet(lbl_1_bss_6AA[4], lbl_1_bss_4AA[7], 0, 10, 0x40000001);
        }
        if (timer != 0) {
            timer--;
            if (timer == 5) {
                Hu3DMotionShiftSet(lbl_1_bss_6AA[0], lbl_1_bss_4AA[1], 0, 5, 0x40000001);
                Hu3DMotionShiftSet(lbl_1_bss_6AA[4], lbl_1_bss_4AA[5], 0, 5, 0x40000001);
            }
            lbl_1_bss_10 = 0;
        }
        HuPrcVSleep();
    }
    if (nightF == 0) {
        mdlId = lbl_1_bss_6AA[0];
        motId[0] = lbl_1_bss_4AA[2];
    } else {
        mdlId = lbl_1_bss_6AA[4];
        motId[0] = lbl_1_bss_4AA[6];
    }
    Hu3DMotionShiftSet(mdlId, motId[0], 0, 15, 0);
    for (i = 1; i <= 30; i++) {
        t = i / 30.0f;
        Hu3DModelPosSet(mdlId, lbl_1_data_70C.x - 200.0 * (1.0 - HuCos(90.0f * t)), lbl_1_data_70C.y + 100.0f * t, lbl_1_data_70C.z + 300.0 * (1.0 - HuCos(90.0f * t)));
        Hu3DModelRotSet(mdlId, 0, 15.0f + 90.0f * -t, 0);
        HuPrcVSleep();
    }
    HuPrcEnd();
}

void fn_1_35F8(OMOBJ *obj)
{
    HuVecF pos;
    HuVecF offset;
    HuVecF dir;
    HuVecF yOffset;
    float rotZ;
    s8 stickPos;

    if (lbl_1_bss_0 != 0) {
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
        yOffset.x = dir.x * (offset.x * offset.x + (1.0f - offset.x * offset.x) * HuCos(rotZ))
            + dir.y * (offset.x * offset.y * (1.0f - HuCos(rotZ)) - offset.z * HuSin(rotZ))
            + dir.z * (offset.x * offset.z * (1.0f - HuCos(rotZ)) + offset.y * HuSin(rotZ));

        yOffset.y = dir.y * (offset.y * offset.y + (1.0f - offset.y * offset.y) * HuCos(rotZ))
            + dir.x * (offset.x * offset.y * (1.0f - HuCos(rotZ)) + offset.z * HuSin(rotZ))
            + dir.z * (offset.y * offset.z * (1.0f - HuCos(rotZ)) - offset.x * HuSin(rotZ));

        yOffset.z = dir.z * (offset.z * offset.z + (1.0f - offset.z * offset.z) * HuCos(rotZ))
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
        VECNormalize(&yOffset, &offset);
        stickPos = -(HuPadSubStkY[0] & 0xF8);
        if (stickPos != 0) {
            CenterM[1].x += 0.05f * (offset.x * stickPos);
            CenterM[1].y += 0.05f * (offset.y * stickPos);
            CenterM[1].z += 0.05f * (offset.z * stickPos);
        }
    }
}

/* @@PART2@@ */
/* Part 2 needs from HEAD: game/saveload.h left out, INST_TYPE_DATA and INST_STAND, the bss types of
   lbl_1_bss_43A/D8/78, and fn_1_A1E8's prototype (see the report). */
void SLSaveModeExec();
void fn_1_4B48(void);
void fn_1_62A8(void);
void fn_1_750C(void);
void fn_1_8B44(void);

/* .data 0xF8 to 0x798+ is defined here, between the functions, in address order */
void fn_1_4158(void)
{
    HuWinInit(1);
    SLSaveModeExec(0);
    omOvlReturnEx(1, 1);
    while(1) {
        HuPrcVSleep();
    }
}

void fn_1_4188(void)
{
    OSTick tick;
    lbl_1_data_4 = FALSE;
    tick = OSGetTick();
    lbl_1_bss_A = HuDataDirReadAsync(MgDataTbl[lbl_1_bss_36].dataDir);
    while(!HuDataGetAsyncStat(lbl_1_bss_A)) {
        HuPrcVSleep();
    }
    lbl_1_data_4 = TRUE;
    OSReport("Load Time:%d\n", OSTicksToMilliseconds(OSGetTick()-tick));
    OSReport("Finished!\n");
    HuPrcEnd();
    while(1) {
        HuPrcVSleep();
    }
}

void fn_1_4268(void)
{
    s16 no = lbl_1_bss_6C;
    HuVecF pos = { 0, 0, 0 };
    while(1) {
        pos.x += HuPadStkX[1]/4;
        pos.y += HuPadStkY[1]/4;
        pos.z += HuPadTrigL[1]/2;
        pos.z -= HuPadTrigR[1]/2;
        if(HuPadBtnDown[1] & PAD_BUTTON_B) {
            OSReport("%f,%f,%f\n", pos.x, pos.y, pos.z);
        }
        HuPrcVSleep();
    }
}

unsigned int lbl_1_data_F8[198] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0026002A, 0x0026002B, 0x0026002C, 0x0026002D, 0x0026002E, 0x0026002F, 0x00260030, 0x00260031,
    0x00260032, 0x00260033, 0x0026002A, 0x0026002A, 0x0026002A, 0x00260034, 0x0026002A, 0x0026002A,
    0x0026002A, 0x0026003A, 0x0026003B, 0x0026003C, 0x0026003D, 0x0026003E, 0x0026003F, 0x00260040,
    0x00260041, 0x00260042, 0x00260043, 0x00260044, 0x00260045, 0x00260046, 0x00260047, 0x00260048,
    0x00260049, 0x0026004A, 0x0026004B, 0x0026004C, 0x0026004D, 0x0026004E, 0x0026004F, 0x00260050,
    0x00260051, 0x00260052, 0x00260053, 0x00260048, 0x00260037, 0x00260048, 0x00260048, 0x00260048,
    0x00260048, 0x80260054, 0x80260055, 0x80260056, 0x80260057, 0x80260058, 0x80260059, 0x8026005A,
    0x8026005B, 0x8026005C, 0x8026005D, 0x8026005E, 0x8026005F, 0x80260060, 0x80260061, 0x80260062,
    0x80260063, 0x80260064, 0x80260065, 0x80260066, 0x80260067, 0x80260068, 0x80260069, 0x8026006A,
    0x8026006B, 0x8026006C, 0x8026006D, 0x0026002A, 0x0026002A, 0x0026002A, 0x00260038, 0x0026002A,
    0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x00260034, 0x80260039, 0x0026002A, 0x0026002A,
    0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A,
    0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A,
    0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A,
    0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A,
    0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A,
    0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A,
    0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A, 0x0026002A,
    0x0026002A, 0x0026002A, 0x80260035, 0x80260036, 0x0026002A, 0x0026002A,
};
u8 lbl_1_data_410[272] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    12, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 22, 30, 30,
    30, 30, 30, 30, 30, 28, 28, 30, 30, 20, 30, 30, 30, 32, 30, 32,
    30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 16, 30, 30, 30,
    12, 24, 22, 24, 22, 24, 24, 22, 24, 16, 20, 22, 16, 24, 24, 24,
    24, 24, 22, 24, 22, 24, 24, 24, 24, 24, 24, 30, 30, 30, 30, 30,
    30, 30, 16, 30, 30, 16, 30, 32, 32, 32, 32, 32, 32, 32, 32, 32,
    30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
    30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
    30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
    30, 30, 18, 24, 30, 30, 30, 32, 32, 32, 32, 32, 32, 32, 32, 32,
    30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
    30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
    30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 24, 30,
    30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
};

HUSPR_GROUPID fn_1_4400(u32 nameMes)
{
    char *mesP;
    s16 width;
    s16 i;
    s16 charNum;
    s16 *posY;
    s16 *posX;
    ANIMDATA **animP;
    HUSPR_GROUPID grpId;
    HUSPR_GROUP *grpP;
    unsigned int *fileTbl;
    unsigned int file;
    HUSPRID sprId;
    s16 c;

    fileTbl = lbl_1_data_F8;
    animP = HuMemDirectMalloc(HEAP_HEAP, sizeof(ANIMDATA *)*100);
    posX = HuMemDirectMalloc(HEAP_HEAP, sizeof(s16)*100);
    posY = HuMemDirectMalloc(HEAP_HEAP, sizeof(s16)*100);
    mesP = HuWinMesPtrGet(nameMes);
    for(width=0, charNum=0; *mesP; mesP++) {
        c = *mesP;
        if(*mesP == 0x20 || *mesP == 0x10) {
            width += lbl_1_data_410[*mesP];
        } else if(*mesP >= 0x30) {
            file = fileTbl[*mesP];
            animP[charNum] = HuSprAnimRead(HuDataSelHeapReadNum(file & 0x7FFFFFFF, HU_MEMNUM_OVL, HEAP_MODEL));
            posX[charNum] = width;
            posY[charNum] = 0;
            width += lbl_1_data_410[c];
            if(file & 0x80000000) {
                posY[charNum] = 4;
            }
            charNum++;
        }
    }
    grpId = HuSprGrpCreate(charNum);
    grpP = &HuSprGrpData[grpId];
    grpP->work[0] = width;
    width = (width/2)-14;
    for(i=0; i<charNum; i++) {
        sprId = HuSprCreate(animP[i], 0, 0);
        HuSprGrpMemberSet(grpId, i, sprId);
        HuSprPosSet(grpId, i, posX[i]-width, posY[i]);
    }
    HuMemDirectFree(animP);
    HuMemDirectFree(posX);
    HuMemDirectFree(posY);
    return grpId;
}

INST_MODEL_DATA lbl_1_data_520[] = {
    { 0x26001A, -1, { -206, -210, 302 }, { 0, 15, 0 }, { 0.9f, 0.9f, 0.9f }, 2 },
    { 0x26001B, 0, { 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 }, 2 },
    { 0x26001C, 0, { 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 }, 2 },
    { 0x26001D, 0, { 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 }, 2 },
    { 0x26001E, -1, { -206, -210, 302 }, { 0, 15, 0 }, { 0.9f, 0.9f, 0.9f }, 2 },
    { 0x26001F, 4, { 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 }, 2 },
    { 0x260020, 4, { 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 }, 2 },
    { 0x260021, 4, { 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 }, 2 },
    { 0x260022, -1, { 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 }, 2 },
    { -1, -1, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, 2 },
};
HuVecF lbl_1_data_700 = { -252, 110, -1400 };
HuVecF lbl_1_data_70C = { -206, -210, 302 };
HuVecF lbl_1_data_718 = { 170, 104, -1400 };
HuVecF lbl_1_data_724 = { 1, 1, 1 };

void fn_1_4668(INST_MODEL_DATA *tbl)
{
    INST_MODEL_DATA *mdlP;
    s16 i;
    for(mdlP=tbl, i=0; mdlP->dataNum != -1; mdlP++, i++) {
        if(mdlP->mdlNo == -1) {
            lbl_1_bss_6AA[i] = Hu3DModelCreate(HuDataSelHeapReadNum(mdlP->dataNum, HU_MEMNUM_OVL, HEAP_MODEL));
            Hu3DModelPosSetV(lbl_1_bss_6AA[i], &mdlP->pos);
            Hu3DModelRotSetV(lbl_1_bss_6AA[i], &mdlP->rot);
            Hu3DModelScaleSetV(lbl_1_bss_6AA[i], &mdlP->scale);
            Hu3DModelCameraSet(lbl_1_bss_6AA[i], mdlP->cameraBit);
            Hu3DModelLayerSet(lbl_1_bss_6AA[i], 1);
        } else {
            lbl_1_bss_4AA[i] = Hu3DJointMotion(lbl_1_bss_6AA[mdlP->mdlNo], HuDataSelHeapReadNum(mdlP->dataNum, HU_MEMNUM_OVL, HEAP_MODEL));
        }
    }
}

void fn_1_47E4(s16 flag)
{
    HUSPR_GROUPID grpId;
    HUSPRID sprId;
    ANIMDATA *animP[2];

    grpId = HuSprGrpCreate(2);
    lbl_1_bss_238[0] = grpId;
    sprId = HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_inst, 0x12), HU_MEMNUM_OVL, HEAP_MODEL)), 100, 0);
    HuSprGrpMemberSet(grpId, 0, sprId);
    HuSprPosSet(grpId, 0, 288, 64);
    sprId = HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_inst, 0x17), HU_MEMNUM_OVL, HEAP_MODEL)), 100, 0);
    HuSprGrpMemberSet(grpId, 1, sprId);
    HuSprPosSet(grpId, 1, 288, 70);
    if(flag == 0) {
        HuSprAttrSet(grpId, 1, HUSPR_ATTR_DISPOFF);
    } else {
        HuSprAttrSet(grpId, 0, HUSPR_ATTR_DISPOFF);
    }
    lbl_1_bss_238[1] = fn_1_4400(lbl_1_bss_20->nameMes);
    HuSprGrpPosSet(lbl_1_bss_238[1], 288, 62);
    if(MgInstExitF) {
        if(lbl_1_bss_20->flag & MG_FLAG_HAS_NIGHT) {
            animP[0] = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_inst, 0x0F), HU_MEMNUM_OVL, HEAP_MODEL));
            animP[1] = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_inst, 0x14), HU_MEMNUM_OVL, HEAP_MODEL));
        } else {
            animP[0] = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_inst, 0x10), HU_MEMNUM_OVL, HEAP_MODEL));
            animP[1] = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_inst, 0x15), HU_MEMNUM_OVL, HEAP_MODEL));
        }
    } else {
        animP[0] = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_inst, 0x11), HU_MEMNUM_OVL, HEAP_MODEL));
        animP[1] = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_inst, 0x16), HU_MEMNUM_OVL, HEAP_MODEL));
    }
    grpId = HuSprGrpCreate(2);
    lbl_1_bss_238[2] = grpId;
    sprId = HuSprCreate(animP[0], 100, 0);
    HuSprGrpMemberSet(grpId, 0, sprId);
    HuSprPosSet(grpId, 0, 108, 270);
    sprId = HuSprCreate(animP[1], 100, 0);
    HuSprGrpMemberSet(grpId, 1, sprId);
    HuSprPosSet(grpId, 1, 108, 270);
    HuSprGrpDrawNoSet(grpId, 64);
    if(flag == 0) {
        HuSprAttrSet(grpId, 1, HUSPR_ATTR_DISPOFF);
    } else {
        HuSprAttrSet(grpId, 0, HUSPR_ATTR_DISPOFF);
    }
    HuSprExecLayerCameraSet(64, 2, 10);
    fn_1_A1E8(fn_1_4B48, flag);
}

void fn_1_4B48(void)
{
    float t;
    float angle;
    float ofsY;
    float posX;
    float posY;
    s16 i;
    s32 flag;
    HUPROCESS *proc;

    proc = HuPrcCurrentGet();
    flag = (s32)proc->property;
    angle = 0.0f;
    if(lbl_1_bss_6 == 0 || lbl_1_bss_6 == 1) {
        HuSprScaleSet(lbl_1_bss_238[2], 0, 0.0f, 0.0f);
        HuSprScaleSet(lbl_1_bss_238[2], 1, 0.0f, 0.0f);
        HuSprPosSet(lbl_1_bss_238[0], 0, 288, -100);
        HuSprPosSet(lbl_1_bss_238[0], 1, 288, -100);
        HuSprGrpPosSet(lbl_1_bss_238[1], 288, -100);
        while(lbl_1_bss_6 != 1) {
            HuPrcVSleep();
        }
        for(i=1; i<=20; i++) {
            t = i/20.0f;
            ofsY = -100.0*(1.0-HuSin(90.0f*t));
            HuSprPosSet(lbl_1_bss_238[0], 0, 288, 64+ofsY);
            HuSprPosSet(lbl_1_bss_238[0], 1, 288, 70+ofsY);
            HuSprGrpPosSet(lbl_1_bss_238[1], 288, 62+ofsY);
            t = HuSin(100.0f*t)*(1.0/HuSin(100));
            HuSprScaleSet(lbl_1_bss_238[2], 0, t, t);
            HuSprScaleSet(lbl_1_bss_238[2], 1, t, t);
            HuPrcVSleep();
        }
    }
    while(1) {
        if(lbl_1_bss_8) {
            t = 1.0-((lbl_1_bss_8-1)/19.0f);
            HuSprAttrReset(lbl_1_bss_238[0], 0, HUSPR_ATTR_DISPOFF);
            HuSprAttrReset(lbl_1_bss_238[0], 1, HUSPR_ATTR_DISPOFF);
            HuSprAttrReset(lbl_1_bss_238[2], 0, HUSPR_ATTR_DISPOFF);
            HuSprAttrReset(lbl_1_bss_238[2], 1, HUSPR_ATTR_DISPOFF);
            if(flag == 0) {
                HuSprTPLvlSet(lbl_1_bss_238[0], 0, 1.0-t);
                HuSprTPLvlSet(lbl_1_bss_238[0], 1, t);
                HuSprTPLvlSet(lbl_1_bss_238[2], 0, 1.0-t);
                HuSprTPLvlSet(lbl_1_bss_238[2], 1, t);
            } else {
                HuSprTPLvlSet(lbl_1_bss_238[0], 0, t);
                HuSprTPLvlSet(lbl_1_bss_238[0], 1, 1.0-t);
                HuSprTPLvlSet(lbl_1_bss_238[2], 0, t);
                HuSprTPLvlSet(lbl_1_bss_238[2], 1, 1.0-t);
            }
            if(1.0 == t) {
                flag = (flag == 0) ? 1 : 0;
            }
        } else if(lbl_1_bss_6 == 3) {
            break;
        }
        posX = 108.0+(2.0*HuSin(angle/2.0f));
        posY = 270.0+(3.0*HuCos(angle));
        HuSprPosSet(lbl_1_bss_238[2], 0, posX, posY);
        HuSprPosSet(lbl_1_bss_238[2], 1, posX, posY);
        angle += 2.0f;
        if(angle > 720.0f) {
            angle -= 720.0f;
        }
        HuPrcVSleep();
    }
    for(i=1; i<=10; i++) {
        t = 1.0-(i/10.0f);
        ofsY = -100.0*(1.0-HuSin(90.0f*t));
        HuSprPosSet(lbl_1_bss_238[0], 0, 288, 64+ofsY);
        HuSprPosSet(lbl_1_bss_238[0], 1, 288, 70+ofsY);
        HuSprGrpPosSet(lbl_1_bss_238[1], 288, 62+ofsY);
        HuSprScaleSet(lbl_1_bss_238[2], 0, t, t);
        HuSprScaleSet(lbl_1_bss_238[2], 1, t, t);
        HuPrcVSleep();
    }
    HuPrcEnd();
}

void fn_1_539C(void)
{
    s16 i;
    s16 num;
    s16 charNo;
    s16 playerNo[GW_PLAYER_MAX];

    lbl_1_bss_438 = 4;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        playerNo[i] = -1;
    }
    switch(lbl_1_bss_20->type) {
        case MG_TYPE_2VS2:
            if(lbl_1_bss_20->flag & MG_FLAG_TEAM_ORDER) {
                for(i=num=0; i<GW_PLAYER_MAX; i++) {
                    playerNo[GwPlayerConf[i].grpNo] = lbl_1_bss_28[i];
                }
            } else {
                for(i=num=0; i<GW_PLAYER_MAX; i++) {
                    if(GwPlayerConf[i].grpNo == 0) {
                        playerNo[num++] = lbl_1_bss_28[i];
                    }
                }
                for(i=0; i<GW_PLAYER_MAX; i++) {
                    if(GwPlayerConf[i].grpNo == 1) {
                        playerNo[num++] = lbl_1_bss_28[i];
                    }
                }
            }
            for(i=0; i<GW_PLAYER_MAX; i++) {
                if(playerNo[i] == -1) {
                    break;
                }
            }
            if(i != GW_PLAYER_MAX) {
                OSReport("Error: 2x2P Group Error!\n");
                for(i=0; i<GW_PLAYER_MAX; i++) {
                    playerNo[i] = lbl_1_bss_28[i];
                }
            }
            break;

        case MG_TYPE_1VS3:
            for(i=num=0; i<GW_PLAYER_MAX; i++) {
                if(GwPlayerConf[i].grpNo == 0) {
                    playerNo[0] = lbl_1_bss_28[i];
                } else {
                    num++;
                    playerNo[num] = lbl_1_bss_28[i];
                }
            }
            if(num != 3) {
                OSReport("Error: 1x3P Group Error!\n");
                for(i=0; i<GW_PLAYER_MAX; i++) {
                    playerNo[i] = lbl_1_bss_28[i];
                }
            }
            break;

        case MG_TYPE_KETTOU:
            for(i=num=0; i<GW_PLAYER_MAX; i++) {
                if(GwPlayerConf[i].grpNo < 2) {
                    playerNo[GwPlayerConf[i].grpNo] = lbl_1_bss_28[i];
                    num++;
                }
            }
            if(num != 2) {
                OSReport("Error: DUEL Group Error!\n");
                playerNo[0] = lbl_1_bss_28[0];
                playerNo[1] = lbl_1_bss_28[1];
            }
            lbl_1_bss_438 = 2;
            break;

        case MG_TYPE_LAST:
            if(lbl_1_bss_20->ovl != 0x57) {
                for(i=num=0; i<GW_PLAYER_MAX; i++) {
                    if(GwPlayerConf[i].grpNo == 0) {
                        playerNo[num] = lbl_1_bss_28[i];
                        num++;
                    }
                }
                if(num == 0) {
                    OSReport("Error: RARE Group Error!\n");
                    playerNo[0] = lbl_1_bss_28[0];
                    playerNo[1] = lbl_1_bss_28[1];
                }
            } else {
                for(i=num=0; i<GW_PLAYER_MAX; i++) {
                    if(GwPlayerConf[i].grpNo == 0) {
                        playerNo[num++] = lbl_1_bss_28[i];
                    }
                }
                for(i=0; i<GW_PLAYER_MAX; i++) {
                    if(GwPlayerConf[i].grpNo == 1) {
                        playerNo[num++] = lbl_1_bss_28[i];
                    }
                }
            }
            lbl_1_bss_438 = num;
            break;

        default:
            for(i=0; i<GW_PLAYER_MAX; i++) {
                playerNo[i] = lbl_1_bss_28[i];
            }
            break;
    }
    for(i=0; i<lbl_1_bss_438; i++) {
        charNo = playerNo[i];
        lbl_1_bss_49E[i] = CharModelCreate(charNo, 8);
        lbl_1_bss_43A[i][0] = CharMotionCreate(charNo, DATANUM(DATA_mariomot, 0));
        lbl_1_bss_43A[i][1] = CharMotionCreate(charNo, DATANUM(DATA_mariomot, 3));
        lbl_1_bss_43A[i][2] = CharMotionCreate(charNo, DATANUM(DATA_mariomot, 4));
        Hu3DModelShadowSet(lbl_1_bss_49E[i]);
        CharMotionSet(charNo, lbl_1_bss_43A[i][0]);
        Hu3DModelAttrSet(lbl_1_bss_49E[i], HU3D_MOTATTR_LOOP);
        CharModelVoiceFlagSet(charNo, FALSE);
    }
    if(lbl_1_bss_20->type == MG_TYPE_KOOPA) {
        lbl_1_bss_49E[i] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_inst, 0x27), HU_MEMNUM_OVL, HEAP_MODEL));
        lbl_1_bss_43A[i][0] = Hu3DJointMotion(lbl_1_bss_49E[i], HuDataSelHeapReadNum(DATANUM(DATA_inst, 0x28), HU_MEMNUM_OVL, HEAP_MODEL));
        lbl_1_bss_43A[i][1] = Hu3DJointMotion(lbl_1_bss_49E[i], HuDataSelHeapReadNum(DATANUM(DATA_inst, 0x29), HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelShadowSet(lbl_1_bss_49E[i]);
        Hu3DMotionSet(lbl_1_bss_49E[i], lbl_1_bss_43A[i][0]);
        Hu3DModelAttrSet(lbl_1_bss_49E[i], HU3D_MOTATTR_LOOP);
        lbl_1_bss_438++;
    } else if(lbl_1_bss_20->type == MG_TYPE_DONKEY) {
        lbl_1_bss_49E[i] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_inst, 0x23), HU_MEMNUM_OVL, HEAP_MODEL));
        lbl_1_bss_43A[i][0] = Hu3DJointMotion(lbl_1_bss_49E[i], HuDataSelHeapReadNum(DATANUM(DATA_inst, 0x24), HU_MEMNUM_OVL, HEAP_MODEL));
        lbl_1_bss_43A[i][1] = Hu3DJointMotion(lbl_1_bss_49E[i], HuDataSelHeapReadNum(DATANUM(DATA_inst, 0x25), HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelShadowSet(lbl_1_bss_49E[i]);
        Hu3DMotionSet(lbl_1_bss_49E[i], lbl_1_bss_43A[i][0]);
        Hu3DModelAttrSet(lbl_1_bss_49E[i], HU3D_MOTATTR_LOOP);
        lbl_1_bss_438++;
    }
}

INST_TYPE_DATA lbl_1_data_798[9] = {
    { 4, { 0x260003, 0x260002, 0x260000, 0x260001 }, 45, 0.6f, { 0 }, { 1, 2, 4, 8 } },
    { 2, { 0x260006, 0x260007, 0x0, 0x0 }, 90, 0.6f, { 0 }, { 1, 14, 0, 0 } },
    { 2, { 0x260004, 0x260005, 0x0, 0x0 }, -90, 0.6f, { 0 }, { 3, 12, 0, 0 } },
    { 4, { 0x260003, 0x260002, 0x260000, 0x260001 }, 45, 0.6f, { 0 }, { 1, 2, 4, 8 } },
    { 2, { 0x26000A, 0x26000C, 0x0, 0x0 }, -90, 0.5f, { 0 }, { 15, 16, 0, 0 } },
    { 1, { 0x26000D, 0x0, 0x0, 0x0 }, 0, 0.8f, { 0 }, { 3, 0, 0, 0 } },
    { 2, { 0x260008, 0x260009, 0x0, 0x0 }, -90, 0.7f, { 0 }, { 1, 2, 0, 0 } },
    { 2, { 0x26000A, 0x26000B, 0x0, 0x0 }, -90, 0.5f, { 0 }, { 15, 16, 0, 0 } },
    { 4, { 0x260003, 0x260002, 0x260000, 0x260001 }, 45, 0.6f, { 0 }, { 1, 2, 4, 8 } },
};

void fn_1_5DE0(void)
{
    INST_STAND *stageP;
    INST_TYPE_DATA *tblP;
    s16 j;
    s16 i;
    s16 num;
    Mtx mtx;
    Mtx mtx2;
    Mtx rotMtx;
    HuVecF pos;

    fn_1_62A8();
    stageP = lbl_1_bss_D8;
    PSMTXRotRad(mtx, 'x', 0.7853982f);
    PSMTXRotRad(mtx2, 'y', 0.7853982f);
    PSMTXConcat(mtx, mtx2, mtx);
    tblP = &lbl_1_data_798[lbl_1_bss_20->type];
    lbl_1_bss_D4 = tblP->num;
    for(i=0; i<lbl_1_bss_D4; i++, stageP++) {
        stageP->mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(tblP->dataNum[i], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelCameraSet(stageP->mdlId, 2);
        Hu3DModelLayerSet(stageP->mdlId, 1);
        Hu3DModelShadowMapSet(stageP->mdlId);
        PSMTXTrans(mtx2, 30.0*HuSin(tblP->rotY+((360.0f/lbl_1_bss_D4)*i)), 0.0f, 30.0*HuCos(tblP->rotY+((360.0f/lbl_1_bss_D4)*i)));
        PSMTXConcat(mtx, mtx2, mtx2);
        Hu3DMtxTransGet(mtx2, &pos);
        PSVECAdd(&pos, &lbl_1_data_700, &pos);
        Hu3DModelPosSetV(stageP->mdlId, &pos);
        Hu3DModelScaleSet(stageP->mdlId, tblP->scale, tblP->scale, tblP->scale);
        Hu3DModelMtxSet(stageP->mdlId, &mtx);
        for(j=0; j<5; j++) {
            stageP->player[j] = -1;
        }
        for(j=num=0; j<5; j++) {
            if(tblP->playerBit[i] & (1 << j)) {
                stageP->player[num] = j;
                pos = tblP->playerPos[j];
                PSMTXMultVec(mtx, &pos, &pos);
                PSMTXRotRad(mtx2, 'x', 0.7853982f);
                PSMTXRotRad(rotMtx, 'y', 0.017453292f*(45.0f+tblP->playerRot[j]));
                PSMTXConcat(mtx2, rotMtx, mtx2);
                Hu3DModelMtxSet(lbl_1_bss_49E[j], &mtx2);
                PSVECAdd(&pos, &lbl_1_data_700, &pos);
                stageP->playerPos[num] = pos;
                lbl_1_bss_78[j] = i;
                num++;
            }
        }
        stageP->time = 0;
        stageP->angle = frandmod(360);
    }
    fn_1_8B44();
    fn_1_A1E8(fn_1_750C, 0);
}

void fn_1_62A8(void)
{
    INST_TYPE_DATA *tblP;
    s16 i;

    tblP = lbl_1_data_798;
    for(i=0; i<4; i++) {
        tblP->playerPos[i].x = 70.0*HuSin(45.0+(90.0*i));
        tblP->playerPos[i].y = 0.0f;
        tblP->playerPos[i].z = 70.0*HuCos(45.0+(90.0*i));
        tblP->playerRot[i] = HuAtan(-tblP->playerPos[i].x, -tblP->playerPos[i].z);
    }
    tblP++;
    tblP->playerPos[0].x = (70.0*HuSin(90))-20.0;
    tblP->playerPos[0].y = 0.0f;
    tblP->playerPos[0].z = 70.0*HuCos(90);
    tblP->playerRot[0] = -90.0f;
    for(i=0; i<3; i++) {
        tblP->playerPos[i+1].x = 20.0+(70.0*HuSin(180.0+(30.0+(60.0*i))));
        tblP->playerPos[i+1].y = 0.0f;
        tblP->playerPos[i+1].z = 70.0*HuCos(180.0+(30.0+(60.0*i)));
        tblP->playerRot[i+1] = 90.0f;
    }
    tblP++;
    for(i=0; i<4; i++) {
        tblP->playerPos[i].x = (((i < 2) ? 1 : -1)*10)+(70.0*HuSin(180.0+(45.0+(90.0*i))));
        tblP->playerPos[i].y = 0.0f;
        tblP->playerPos[i].z = 70.0*HuCos(180.0+(45.0+(90.0*i)));
        tblP->playerRot[i] = 90.0f*((i < 2) ? 1 : -1);
    }
    tblP++;
    for(i=0; i<4; i++) {
        tblP->playerPos[i].x = 70.0*HuSin(45.0+(90.0*i));
        tblP->playerPos[i].y = 0.0f;
        tblP->playerPos[i].z = 70.0*HuCos(45.0+(90.0*i));
        tblP->playerRot[i] = HuAtan(-tblP->playerPos[i].x, -tblP->playerPos[i].z);
    }
    tblP++;
    for(i=0; i<4; i++) {
        tblP->playerPos[i].x = 70.0*HuSin(180.0+(22.5+(45.0*i)));
        tblP->playerPos[i].y = 0.0f;
        tblP->playerPos[i].z = 70.0*HuCos(180.0+(22.5+(45.0*i)));
        tblP->playerRot[i] = 90.0f;
    }
    tblP->playerPos[i].x = (70.0*HuSin(90))-20.0;
    tblP->playerPos[i].y = 0.0f;
    tblP->playerPos[i].z = 70.0*HuCos(90);
    tblP->playerRot[i] = -90.0f;
    tblP++;
    if(lbl_1_bss_438 == 1) {
        tblP->playerPos[0].x = 0.0f;
        tblP->playerPos[0].y = 0.0f;
        tblP->playerPos[0].z = 0.0f;
        tblP->playerRot[0] = 0.0f;
        lbl_1_data_798[5].playerBit[0] = 1;
    } else if(lbl_1_bss_438 == 2) {
        for(i=0; i<lbl_1_bss_438; i++) {
            tblP->playerPos[i].x = -56.0f+(2.0f*(56.0f*i));
            tblP->playerPos[i].y = 0.0f;
            tblP->playerPos[i].z = 0.0f;
            tblP->playerRot[i] = HuAtan(-tblP->playerPos[i].x, -tblP->playerPos[i].z);
        }
        lbl_1_data_798[5].playerBit[0] = 3;
    } else {
        for(i=0; i<lbl_1_bss_438; i++) {
            tblP->playerPos[i].x = (70.0*HuSin(180.0+(45.0+(90.0*i))))-(((i < 2) ? 1 : -1)*20);
            tblP->playerPos[i].y = 0.0f;
            tblP->playerPos[i].z = 70.0*HuCos(180.0+(45.0+(90.0*i)));
            tblP->playerRot[i] = 90.0f*((i < 2) ? 1 : -1);
        }
        lbl_1_data_798[5].playerBit[0] = 15;
    }
    tblP++;
    for(i=0; i<2; i++) {
        tblP->playerPos[i].x = (((i < 1) ? 1 : -1)*30)+(70.0*HuSin((180.0*i)-90.0));
        tblP->playerPos[i].y = 0.0f;
        tblP->playerPos[i].z = 70.0*HuCos((180.0*i)-90.0);
        tblP->playerRot[i] = HuAtan(-tblP->playerPos[i].x, -tblP->playerPos[i].z);
    }
    tblP++;
    for(i=0; i<4; i++) {
        tblP->playerPos[i].x = 70.0*HuSin(180.0+(22.5+(45.0*i)));
        tblP->playerPos[i].y = 0.0f;
        tblP->playerPos[i].z = 70.0*HuCos(180.0+(22.5+(45.0*i)));
        tblP->playerRot[i] = 90.0f;
    }
    tblP->playerPos[i].x = (70.0*HuSin(90))-20.0;
    tblP->playerPos[i].y = 0.0f;
    tblP->playerPos[i].z = 70.0*HuCos(90);
    tblP->playerRot[i] = -90.0f;
}
/* @@PART3@@ */
/* Part 3 needs these from HEAD (types as part 3 uses them) */
extern MG_DATA *lbl_1_bss_20;
extern s16 lbl_1_bss_6;
extern s16 lbl_1_bss_8;
extern s16 lbl_1_bss_14;
extern s16 lbl_1_bss_24;
extern s16 lbl_1_bss_26;
extern s32 lbl_1_bss_30;
extern float lbl_1_bss_48;
extern void *lbl_1_bss_4C;
extern ANIMDATA *lbl_1_bss_50[2];
extern ANIMDATA *lbl_1_bss_58;
extern ANIMDATA *lbl_1_bss_5C[2];
extern HUSPR_GROUPID lbl_1_bss_66;
extern HU3D_MODELID lbl_1_bss_6C;
extern s16 lbl_1_bss_6E;
extern ANIMDATA *lbl_1_bss_70[2];
extern s16 lbl_1_bss_78[6];
extern s32 lbl_1_bss_84[20];
extern s16 lbl_1_bss_D4;
extern INST_STAGE lbl_1_bss_D8[4];
extern s16 lbl_1_bss_438;
extern HU3D_MOTIONID lbl_1_bss_43A[][10];
extern HU3D_MODELID lbl_1_bss_49E[6];
extern HuVecF lbl_1_data_700;
extern HuVecF lbl_1_data_718;
extern HuVecF lbl_1_data_724;
extern INST_STAGETBL lbl_1_data_798[];
extern GXRenderModeObj *RenderMode;

void fn_1_8504(s16 player, HuVecF *pos, HuVecF *rot);
void fn_1_8680(void);
void fn_1_902C(s16 layerNo);
void fn_1_99CC(void);
void fn_1_9CB0(s16 layerNo);
void fn_1_9CF4(HU3D_MODEL *modelP, Mtx *mtx);
void fn_1_96F4(s16 nightF);
void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);
HUPROCESS *fn_1_A1E8(void (*func)(void), s32 param);

void fn_1_750C(void)
{
    float rotY;
    float scale;
    float t;
    float t2;
    float y;
    INST_STAGE *standP;
    INST_STAGETBL *dataP;
    s16 i;
    s16 j;
    s16 time;
    HuVecF basePos[4];
    Mtx mtx;
    Mtx mtxRotY;
    Mtx mtxRotX;
    float angle[4];
    HuVecF pos;
    HuVecF rot;
    HuVecF dir;
    HuVecF tmp;

    rotY = 45.0f;
    for(i=0; i<lbl_1_bss_D4; i++) {
        Hu3DModelPosGet(lbl_1_bss_D8[i].mdlId, &basePos[i]);
        angle[i] = (i*45)+frandmod(90);
    }
    for(i=0; i<lbl_1_bss_438; i++) {
        lbl_1_bss_84[i] = 1;
    }
    dataP = &lbl_1_data_798[lbl_1_bss_20->type];
    if(lbl_1_bss_6 == 0 || lbl_1_bss_6 == 1) {
        for(i=0; i<lbl_1_bss_438; i++) {
            Hu3DModelPosSet(lbl_1_bss_49E[i], -5000.0f, 0.0f, 0.0f);
            lbl_1_bss_84[i] = 0;
            fn_1_A1E8(fn_1_8680, i);
        }
        for(standP=lbl_1_bss_D8, i=0; i<lbl_1_bss_D4; i++, standP++) {
            Hu3DModelScaleSet(standP->mdlId, 0.0f, 0.0f, 0.0f);
        }
        HuPrcSleep(30);
        for(j=1; j<=15; j++) {
            t = j/15.0f;
            standP = lbl_1_bss_D8;
            scale = dataP->scale*(HuSin(130.0f*t)*(1.0/HuSin(130.0)));
            for(i=0; i<lbl_1_bss_D4; i++, standP++) {
                Hu3DModelScaleSet(standP->mdlId, scale, scale, scale);
            }
            HuPrcVSleep();
        }
    }
    while(1) {
        for(standP=lbl_1_bss_D8, i=0; i<lbl_1_bss_D4; i++, standP++) {
            y = 3.0*HuSin(angle[i]);
            PSMTXRotRad(mtxRotX, 'x', 0.7853982f);
            PSMTXRotRad(mtx, 'y', MTXDegToRad(rotY));
            PSMTXConcat(mtxRotX, mtx, mtxRotX);
            Hu3DModelMtxSet(standP->mdlId, &mtxRotX);
            PSMTXTrans(mtx, 30.0*HuSin(dataP->rotY+((360.0f/lbl_1_bss_D4)*i)), y, 30.0*HuCos(dataP->rotY+((360.0f/lbl_1_bss_D4)*i)));
            PSMTXConcat(mtxRotX, mtx, mtx);
            Hu3DMtxTransGet(mtx, &pos);
            PSVECAdd(&pos, &lbl_1_data_700, &pos);
            Hu3DModelPosSetV(standP->mdlId, &pos);
            for(j=0; j<5; j++) {
                if(standP->player[j] == -1) {
                    break;
                }
                if(lbl_1_bss_84[standP->player[j]]) {
                    fn_1_8504(standP->player[j], &pos, &rot);
                    Hu3DModelPosSetV(lbl_1_bss_49E[standP->player[j]], &pos);
                    PSMTXRotRad(mtx, 'x', MTXDegToRad(rot.x));
                    PSMTXRotRad(mtxRotY, 'y', MTXDegToRad(rot.y+rotY));
                    PSMTXConcat(mtx, mtxRotY, mtx);
                    Hu3DModelMtxSet(lbl_1_bss_49E[standP->player[j]], &mtx);
                }
            }
            if(standP->time != 0) {
                standP->time--;
                standP->angle += 30.0f;
                if(standP->angle > 360.0f) {
                    standP->angle -= 360.0f;
                }
                if(i & 1) {
                    Hu3DModelRotSet(standP->mdlId, (standP->time/10.0f)*HuSin(standP->angle), 0.0f, 0.0f);
                } else {
                    Hu3DModelRotSet(standP->mdlId, 0.0f, 0.0f, (standP->time/10.0f)*HuSin(standP->angle));
                }
            }
            angle[i] += 3.0f;
            if(angle[i] > 360.0f) {
                angle[i] -= 360.0f;
            }
        }
        rotY += 0.2f;
        if(rotY > 360.0f) {
            rotY -= 360.0f;
        }
        if(lbl_1_bss_6 == 3) {
            break;
        }
        HuPrcVSleep();
    }
    dataP = &lbl_1_data_798[lbl_1_bss_20->type];
    for(time=1; time<=60; time++) {
        t = time/60.0f;
        t2 = 1.0f-(time/40.0f);
        if(t2 < 0.0) {
            t2 = 0.0f;
        }
        PSMTXRotRad(mtxRotX, 'x', MTXDegToRad(45.0*HuSin(90.0f*t2)));
        PSMTXRotRad(mtx, 'y', MTXDegToRad(rotY+(360.0*(1.0-HuCos(90.0f*t)))));
        PSMTXConcat(mtxRotX, mtx, mtxRotX);
        PSVECSubtract(&lbl_1_data_718, &lbl_1_data_700, &dir);
        dir.z -= 100.0f;
        scale = (0.2f*dataP->scale)+(0.8f*(dataP->scale*HuCos(90.0f*t)));
        for(standP=lbl_1_bss_D8, i=0; i<lbl_1_bss_D4; i++, standP++) {
            PSMTXTrans(mtx, 30.0*HuSin(dataP->rotY+((360.0f/lbl_1_bss_D4)*i)), 0.0f, 30.0*HuCos(dataP->rotY+((360.0f/lbl_1_bss_D4)*i)));
            PSMTXConcat(mtxRotX, mtx, mtx);
            Hu3DMtxTransGet(mtx, &pos);
            PSVECScale(&dir, &tmp, t);
            PSVECAdd(&tmp, &lbl_1_data_700, &tmp);
            PSVECAdd(&pos, &tmp, &pos);
            pos.z += 500.0*HuSin(180.0f*t);
            Hu3DModelPosSetV(standP->mdlId, &pos);
            Hu3DModelScaleSet(standP->mdlId, scale, scale, scale);
            Hu3DModelMtxSet(standP->mdlId, &mtxRotX);
            for(j=0; j<5; j++) {
                if(standP->player[j] == -1) {
                    break;
                }
                if(lbl_1_bss_84[standP->player[j]]) {
                    fn_1_8504(standP->player[j], &pos, &rot);
                    Hu3DModelPosSetV(lbl_1_bss_49E[standP->player[j]], &pos);
                    Hu3DModelScaleSet(lbl_1_bss_49E[standP->player[j]], scale, scale, scale);
                    PSMTXRotRad(mtx, 'x', MTXDegToRad(rot.x));
                    PSMTXRotRad(mtxRotY, 'y', MTXDegToRad((rotY+rot.y)+(360.0*(1.0-HuCos(90.0f*t)))));
                    PSMTXConcat(mtx, mtxRotY, mtx);
                    Hu3DModelMtxSet(lbl_1_bss_49E[standP->player[j]], &mtx);
                }
            }
        }
        HuPrcVSleep();
    }
    HuPrcEnd();
}

void fn_1_8504(s16 player, HuVecF *pos, HuVecF *rot)
{
    INST_STAGETBL *dataP;
    INST_STAGE *standP;
    HU3D_MODEL *modelP;
    HuVecF ofs;
    Mtx mtxModel;
    Mtx mtx;

    dataP = &lbl_1_data_798[lbl_1_bss_20->type];
    standP = &lbl_1_bss_D8[lbl_1_bss_78[player]];
    modelP = &Hu3DData[standP->mdlId];
    mtxRot(mtxModel, modelP->rot.x, modelP->rot.y, modelP->rot.z);
    mtxScaleCat(mtxModel, modelP->scale.x, modelP->scale.y, modelP->scale.z);
    mtxTransCat(mtxModel, modelP->pos.x, modelP->pos.y, modelP->pos.z);
    PSMTXConcat(mtxModel, modelP->mtx, mtx);
    ofs = dataP->playerPos[player];
    PSVECScale(&ofs, &ofs, 1.0/dataP->scale);
    PSMTXMultVec(mtx, &ofs, pos);
    rot->x = 45.0f;
    rot->y = dataP->playerRot[player]+modelP->rot.y;
    rot->z = 0.0f;
}

void fn_1_8680(void)
{
    HUPROCESS *proc;
    int player;
    BOOL koopaF;
    s16 time;
    float t;
    float ofsY;
    HuVecF pos;
    HuVecF rot;

    proc = HuPrcCurrentGet();
    player = (int)proc->property;
    koopaF = FALSE;
    if(player == 4 && lbl_1_bss_20->type == MG_TYPE_KOOPA) {
        koopaF = TRUE;
    }
    lbl_1_bss_84[player] = 0;
    while(lbl_1_bss_6 != 1) {
        HuPrcVSleep();
    }
    Hu3DMotionSet(lbl_1_bss_49E[player], lbl_1_bss_43A[player][1]);
    if(!koopaF) {
        Hu3DMotionTimeSet(lbl_1_bss_49E[player], Hu3DMotionMaxTimeGet(lbl_1_bss_49E[player]));
        Hu3DModelAttrReset(lbl_1_bss_49E[player], HU3D_MOTATTR_LOOP);
    } else {
        Hu3DMotionTimeSet(lbl_1_bss_49E[player], 30.0f);
        Hu3DModelAttrReset(lbl_1_bss_49E[player], HU3D_MOTATTR_LOOP);
        Hu3DModelAttrSet(lbl_1_bss_49E[player], HU3D_MOTATTR_PAUSE);
    }
    HuPrcSleep(player*2);
    for(time=1; time<=30; time++) {
        t = time/30.0f;
        ofsY = 500.0*HuCos(90.0f*t);
        fn_1_8504(player, &pos, &rot);
        pos.y += ofsY;
        Hu3DModelPosSetV(lbl_1_bss_49E[player], &pos);
        if(time == 25) {
            if(koopaF) {
                Hu3DModelAttrReset(lbl_1_bss_49E[player], HU3D_MOTATTR_PAUSE);
            } else {
                Hu3DMotionShiftSet(lbl_1_bss_49E[player], lbl_1_bss_43A[player][2], 0.0f, 5.0f, HU3D_MOTATTR_NONE);
            }
        }
        HuPrcVSleep();
    }
    lbl_1_bss_D8[lbl_1_bss_78[player]].time = 30;
    Hu3DMotionShiftSet(lbl_1_bss_49E[player], lbl_1_bss_43A[player][0], 0.0f, 5.0f, HU3D_MOTATTR_LOOP);
    lbl_1_bss_84[player] = 1;
    HuPrcEnd();
}

void fn_1_8B44(void)
{
    INST_STAGETBL *dataP;
    s16 i;
    HuVecF pos;
    HuVecF rot;

    dataP = &lbl_1_data_798[lbl_1_bss_20->type];
    for(i=0; i<lbl_1_bss_438; i++) {
        Hu3DModelScaleSet(lbl_1_bss_49E[i], dataP->scale, dataP->scale, dataP->scale);
        fn_1_8504(i, &pos, &rot);
        Hu3DModelPosSetV(lbl_1_bss_49E[i], &pos);
    }
    if(lbl_1_bss_20->type == MG_TYPE_DONKEY) {
        Hu3DModelScaleSet(lbl_1_bss_49E[4], 1.5*dataP->scale, 1.5*dataP->scale, 1.5*dataP->scale);
    }
}

void fn_1_8DB0(void)
{
    s16 i;
    for(i=lbl_1_bss_24=lbl_1_bss_26=0; i<4; i++) {
        if(lbl_1_bss_20->type != MG_TYPE_KETTOU) {
            if(GwPlayerConf[i].type == GW_PLAYER_TYPE_MAN) {
                lbl_1_bss_26 |= HuPadBtn[GwPlayerConf[i].padNo];
                lbl_1_bss_24 |= HuPadBtnDown[GwPlayerConf[i].padNo];
            }
        } else {
            if(GwPlayerConf[i].type == GW_PLAYER_TYPE_MAN && GwPlayerConf[i].grpNo < 2) {
                lbl_1_bss_26 |= HuPadBtn[GwPlayerConf[i].padNo];
                lbl_1_bss_24 |= HuPadBtnDown[GwPlayerConf[i].padNo];
            }
        }
    }
}

void fn_1_8FA0(s16 layerNo)
{
    Hu3DCameraLayerHookSet(1, 8, fn_1_902C);
    lbl_1_bss_70[0] = HuSprAnimRead(HuDataSelHeapReadNum(0x26000E, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_70[1] = HuSprAnimRead(HuDataSelHeapReadNum(0x260013, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_6E = layerNo;
}

void fn_1_902C(s16 layerNo)
{
    float t;
    GXColor color;
    Mtx44 proj;
    Mtx mtx;

    GXInvalidateTexAll();
    MTXOrtho(proj, 0.0f, 480.0f, 0.0f, 576.0f, 0.0f, 10.0f);
    GXSetProjection(proj, GX_ORTHOGRAPHIC);
    if(RenderMode->field_rendering) {
        GXSetViewportJitter(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f, VIGetNextField());
    } else {
        GXSetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    }
    GXSetScissor(0, 0, 640, 480);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetNumChans(0);
    PSMTXIdentity(mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    PSMTXTrans(mtx, -lbl_1_bss_48, -lbl_1_bss_48, 0.0f);
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
    HuSprTexLoad(lbl_1_bss_70[lbl_1_bss_6E], 0, GX_TEXMAP0, GX_REPEAT, GX_REPEAT, GX_LINEAR);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumTexGens(1);
    if(lbl_1_bss_8 == 0) {
        GXSetNumTevStages(1);
    } else {
        t = (lbl_1_bss_8-1)/19.0f;
        color.a = 255.0f*t;
        GXSetTevColor(GX_TEVREG0, color);
        HuSprTexLoad(lbl_1_bss_70[(lbl_1_bss_6E+1)&1], 0, GX_TEXMAP1, GX_REPEAT, GX_REPEAT, GX_LINEAR);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_TEXC, GX_CC_CPREV, GX_CC_A0, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetNumTevStages(2);
        if(lbl_1_bss_8 == 1) {
            lbl_1_bss_6E ^= 1;
        }
    }
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition2f32(0.0f, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition2f32(576.0f, 0.0f);
    GXTexCoord2f32(9.0f, 0.0f);
    GXPosition2f32(576.0f, 480.0f);
    GXTexCoord2f32(9.0f, 7.5f);
    GXPosition2f32(0.0f, 480.0f);
    GXTexCoord2f32(0.0f, 7.5f);
    lbl_1_bss_48 += 0.005f;
    if(lbl_1_bss_48 >= 0.999f) {
        lbl_1_bss_48 = 0.0f;
    }
}

void fn_1_96F4(s16 nightF)
{
    s16 time;
    void *data;
    HUSPRID sprId;

    time = (nightF == 0) ? 0 : 1;
    lbl_1_bss_6C = Hu3DHookFuncCreate(fn_1_9CF4);
    Hu3DModelCameraSet(lbl_1_bss_6C, 2);
    Hu3DModelLayerSet(lbl_1_bss_6C, 1);
    Hu3DModelPosSetV(lbl_1_bss_6C, &lbl_1_data_718);
    Hu3DModelScaleSetV(lbl_1_bss_6C, &lbl_1_data_724);
    lbl_1_bss_5C[0] = HuSprAnimRead(HuDataSelHeapReadNum(0x260019, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_5C[1] = HuSprAnimRead(HuDataSelHeapReadNum(0x260018, HU_MEMNUM_OVL, HEAP_MODEL));
    data = HuDataReadNumHeapShortForce(lbl_1_bss_20->instPic[0][lbl_1_bss_14], HU_MEMNUM_OVL, HEAP_MODEL);
    lbl_1_bss_50[0] = HuSprAnimRead(data);
    data = HuDataReadNumHeapShortForce(lbl_1_bss_20->instPic[1][lbl_1_bss_14], HU_MEMNUM_OVL, HEAP_MODEL);
    lbl_1_bss_50[1] = HuSprAnimRead(data);
    lbl_1_bss_58 = lbl_1_bss_50[time];
    sprId = HuTHPSprCreateVol(lbl_1_bss_20->movie[time][lbl_1_bss_14], TRUE, 100, 0.0f);
    lbl_1_bss_66 = HuSprGrpCreate(1);
    HuSprGrpMemberSet(lbl_1_bss_66, 0, sprId);
    HuSprGrpDrawNoSet(lbl_1_bss_66, 127);
    HuSprPosSet(lbl_1_bss_66, 0, 128.0f, 128.0f);
    lbl_1_bss_4C = HuMemDirectMallocNum(HEAP_HEAP, GXGetTexBufferSize(284, 256, GX_TF_RGB565, GX_FALSE, 0), HU_MEMNUM_OVL);
    Hu3DCameraLayerHookSet(1, 0, fn_1_9CB0);
    fn_1_A1E8(fn_1_99CC, time);
}

void fn_1_99CC(void)
{
    HUPROCESS *proc;
    int time;
    s16 i;
    float t;
    HUSPRID sprId;

    proc = HuPrcCurrentGet();
    time = (int)proc->property;
    if(lbl_1_bss_6 == 0 || lbl_1_bss_6 == 1) {
        Hu3DModelPosSet(lbl_1_bss_6C, 5000.0f, 0.0f, 0.0f);
        while(lbl_1_bss_6 != 1) {
            HuPrcVSleep();
        }
        for(i=1; i<=20; i++) {
            t = i/20.0f;
            Hu3DModelPosSet(lbl_1_bss_6C, lbl_1_data_718.x+(500.0*(1.0-HuSin(90.0f*t))), lbl_1_data_718.y, lbl_1_data_718.z);
            HuPrcVSleep();
        }
    }
    while(1) {
        if(lbl_1_bss_8 != 0) {
            lbl_1_bss_30 = 1;
            time ^= 1;
            HuTHPClose();
            lbl_1_bss_58 = lbl_1_bss_50[time];
            while(HuTHPProcCheck()) {
                HuPrcVSleep();
            }
            HuSprGrpKill(lbl_1_bss_66);
            sprId = HuTHPSprCreateVol(lbl_1_bss_20->movie[time][lbl_1_bss_14], TRUE, 100, 0.0f);
            lbl_1_bss_66 = HuSprGrpCreate(1);
            HuSprGrpMemberSet(lbl_1_bss_66, 0, sprId);
            HuSprGrpDrawNoSet(lbl_1_bss_66, 127);
            HuSprPosSet(lbl_1_bss_66, 0, 128.0f, 128.0f);
            while(lbl_1_bss_8 != 0) {
                HuPrcVSleep();
            }
            lbl_1_bss_30 = 0;
        }
        HuPrcVSleep();
    }
}

void fn_1_9CB0(s16 layerNo)
{
    Hu3DFbCopyExec(0, 0, 284, 256, GX_TF_RGB565, FALSE, lbl_1_bss_4C);
}

void fn_1_9CF4(HU3D_MODEL *modelP, Mtx *mtx)
{
    GXInvalidateTexAll();
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetNumChans(0);
    GXLoadPosMtxImm(*mtx, GX_PNMTX0);
    if(HuTHPStartCheck()) {
        Hu3DTexLoad(lbl_1_bss_4C, 284, 256, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, TRUE, GX_TEXMAP0);
    } else {
        HuSprTexLoad(lbl_1_bss_58, 0, GX_TEXMAP0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    }
    HuSprTexLoad(lbl_1_bss_5C[0], 0, GX_TEXMAP1, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    HuSprTexLoad(lbl_1_bss_5C[1], 0, GX_TEXMAP2, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_TEXA, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD1, GX_TEXMAP2, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumTexGens(2);
    GXSetNumTevStages(3);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-200.0f, 150.0f, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(200.0f, 150.0f, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(200.0f, -150.0f, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(-200.0f, -150.0f, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
}

HUPROCESS *fn_1_A1E8(void (*func)(void), s32 param)
{
    HUPROCESS *proc = HuPrcChildCreate(func, 16, 0x3000, 0, HuPrcCurrentGet());
    proc->property = (void *)param;
}

/* @@END@@ */
