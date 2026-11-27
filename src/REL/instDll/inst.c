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
            HuWinMesSet(win0[i], lbl_1_bss_18[0]);
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
            time = (time == 0) ? 1 : 0;
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

/* @@PART2@@ */
/* @@PART3@@ */
/* @@END@@ */
