/* @@HEAD@@ */
#include "game/actman.h"
#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mgscore.h"
#include "game/object.h"
#include "game/process.h"
#include "game/seqman.h"
#include "dolphin.h"
#include "string.h"
#include "math.h"

#include "datadir_enum.h"

#pragma dont_inline on

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* The work of the sequence object (fn_1_5F0 and on). */
typedef struct M621Main_s {
    s16 unk0;
    s16 timer;
    int seNo;
} M621_MAIN;

/* The camera object (fn_1_B4C). */
typedef struct M621Camera_s {
    int timer;
    HU3D_MOTIONID motA;
    HU3D_MOTIONID motB;
    HU3D_MODELID camMdlId;
} M621_CAMERA;

/* The stage object (fn_1_D28). */
typedef struct M621Stage_s {
    int unk0;
    OMOBJ *obj;
    HU3D_MODELID mdlId;
    HU3D_MOTIONID motId;
    HU3D_MODELID hookMdlId;
} M621_STAGE;

/* One piece of a target: piece 0 is the head, the rest follow it. */
typedef struct M621Item_s {
    s16 state;
    s16 timer;
    u32 unk4;
    HuVecF vel;
    HU3D_MODELID itemMdlId;
    s16 unk16;
    MGACTOR *actor;
    s16 unk1C;
    s16 mdlId;
    s16 playerNo;
} M621_ITEM; /* size 0x24 */

typedef struct M621Target_s {
    s16 no;
    s16 state;
    int timer;
    s16 unk8;
    s16 itemNum;
    s16 unkC;
    s16 unkE;
    float rotY;
    float unk14;
    float unk18;
    HuVecF pos;
    float height;
    s16 unk2C;
    HU3D_MODELID mdlId;
    MGACTOR *actor;
    M621_ITEM item[7];
} M621_TARGET; /* size 0x130 */

typedef struct M621Player_s {
    int timer;
    MGPLAYER *player;
    float rotY;
    HuVecF pos;
    s16 score;
    s16 winF;
    s16 playerNo;
    s16 charNo;
    int motNo;
    s16 unk24;
    s16 unk26;
    s16 vibTimer;
    HUSPR_GROUPID scoreBoxId;
    MGSCORE *scoreP;
    s16 comF;
    s16 comDif;
    s16 comMode;
    s16 comTimer;
    s16 comWait;
    M621_TARGET *target;
} M621_PLAYER; /* size 0x40 */

void *fn_1_A0(int prio, u32 size, OMOBJ_FUNC func);
void fn_1_140(OMOBJ *obj, OMOBJ_FUNC func);
void ObjectSetup(void);
void fn_1_2BC(void);
void fn_1_2E0(s16 mode, s16 frameNo);
void fn_1_358(s16 mode, s16 frameNo);
void fn_1_35C(s16 mode, s16 frameNo);
void fn_1_3B8(s16 mode, s16 frameNo);
void fn_1_3BC(s16 mode, s16 frameNo);
void fn_1_410(s16 mode, s16 frameNo);
void fn_1_414(s16 mode, s16 frameNo);
void fn_1_418(s16 mode, s16 frameNo);
void fn_1_41C(s16 mode, s16 frameNo);
void fn_1_420(HuVecF *pos, s16 seNo);
int fn_1_594(s16 pan, s16 seNo);
void fn_1_5C8(int seNo);
void fn_1_5F0(OMOBJ *obj);
void fn_1_70C(OMOBJ *obj);
void fn_1_778(OMOBJ *obj);
void fn_1_A5C(OMOBJ *obj);
void fn_1_B4C(OMOBJ *obj);
void fn_1_C90(OMOBJ *obj);
void fn_1_CEC(OMOBJ *obj);
void fn_1_D28(OMOBJ *obj);
void fn_1_106C(M621_PLAYER *work, int motNo, float shift, u32 attr);
int fn_1_10E8(HuVecF *in, HuVecF *out);
int fn_1_1240(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b);
void fn_1_13EC(OMOBJ *obj);
void fn_1_175C(OMOBJ *obj);
void fn_1_1844(OMOBJ *obj);
void fn_1_19C4(OMOBJ *obj);
void fn_1_1A3C(OMOBJ *obj);
void fn_1_1B5C(OMOBJ *obj);
void fn_1_1BF4(OMOBJ *obj);
M621_TARGET *fn_1_1C7C(void);
M621_TARGET *fn_1_1D08(M621_PLAYER *work);
s16 fn_1_1F9C(M621_PLAYER *work);
s16 fn_1_2074(M621_PLAYER *work);
void fn_1_215C(OMOBJ *obj);
u32 fn_1_26A4(void);
void fn_1_270C(u32 bit);
s16 fn_1_272C(void);
void fn_1_27A0(void);
void fn_1_29E8(OMOBJ *obj);
void fn_1_2A54(M621_PLAYER *player, M621_TARGET *target);
void fn_1_2C6C(M621_PLAYER *player, M621_TARGET *target, s16 itemNo);
void fn_1_3678(OMOBJ *obj);
void fn_1_5054(OMOBJ *obj);
void fn_1_51F4(s16 playerNo, HuVecF *pos);
void fn_1_52D4(s32 playerNo, s16 mgCoin);
void fn_1_5328(s32 playerNo, u32 mgScore);
s16 fn_1_5340(void);
float fn_1_5350(float x);
double fn_1_537C(double x);

MGSEQ_PARAM lbl_1_data_0 = {
    30, MGSEQ_TIMER_BOTTOM,
    fn_1_2E0, fn_1_358, fn_1_35C, fn_1_3B8, fn_1_3BC, fn_1_410, fn_1_414, fn_1_418, fn_1_41C
};
s32 lbl_1_data_28[2] = { -1, 0 };
HuVecF lbl_1_data_30 = { 0.0f, 0.0f, 0.0f };
s32 lbl_1_data_3C[2][5] = {
    { DATANUM(DATA_m621, 2), -1, DATANUM(DATA_m621, 3), DATANUM(DATA_m621, 1), DATANUM(DATA_m621, 0) },
    { DATANUM(DATA_m621, 5), DATANUM(DATA_m621, 6), DATANUM(DATA_m621, 7), DATANUM(DATA_m621, 4), DATANUM(DATA_m621, 0) },
};
s32 lbl_1_data_64[2][2] = {
    { DATANUM(DATA_m621, 0x21), -1 },
    { DATANUM(DATA_m621, 0x23), DATANUM(DATA_m621, 0x22) },
};
s32 lbl_1_data_74[2][3] = {
    { DATANUM(DATA_m621, 0x0F), DATANUM(DATA_m621, 0x11), DATANUM(DATA_m621, 0x1B) },
    { DATANUM(DATA_m621, 0x10), DATANUM(DATA_m621, 0x12), DATANUM(DATA_m621, 0x1C) },
};
s32 lbl_1_data_8C[4] = { DATANUM(DATA_m621, 8), DATANUM(DATA_m621, 9), DATANUM(DATA_m621, 10), DATANUM(DATA_m621, 11) };
s32 lbl_1_data_9C[4][2] = {
    { DATANUM(DATA_m621, 0x15), DATANUM(DATA_m621, 0x1D) },
    { DATANUM(DATA_m621, 0x16), DATANUM(DATA_m621, 0x1E) },
    { DATANUM(DATA_m621, 0x17), DATANUM(DATA_m621, 0x1F) },
    { DATANUM(DATA_m621, 0x18), DATANUM(DATA_m621, 0x20) },
};
HuVecF lbl_1_data_BC = { 0.0f, 1.0f, 0.0f };
char *lbl_1_data_258[21] = {
    "621sambo-samboitiA", "621sambo-samboitiB", "621sambo-samboitiC", "621sambo-samboitiD",
    "621sambo-samboitiE", "621sambo-samboitiF", "621sambo-samboitiG", "621sambo-samboitiI",
    "621sambo-samboitiJ", "621sambo-samboitiK", "621sambo-samboitiL", "621sambo-samboitiM",
    "621sambo-samboitiN", "621sambo-samboitiO", "621sambo-samboitiP", "621sambo-samboitiQ",
    "621sambo-samboitiR", "621sambo-samboitiS", "621sambo-samboitiT", "621sambo-samboitiU",
    "621sambo-samboitiV"
};
u32 lbl_1_data_2AC = 0xFFFFFFFE;
unsigned int lbl_1_data_2B0[] = {
    DATANUM(DATA_mariomot, 0x00), DATANUM(DATA_mariomot, 0x01), DATANUM(DATA_mariomot, 0x02),
    DATANUM(DATA_mariomot, 0x03), DATANUM(DATA_mariomot, 0x04), DATANUM(DATA_mariomot, 0x05),
    DATANUM(DATA_mariomot, 0x38), DATANUM(DATA_mariomot, 0x18), DATANUM(DATA_mariomot, 0x06),
    DATANUM(DATA_mariomot, 0x28), DATANUM(DATA_mariomot, 0x0F), DATANUM(DATA_mariomot, 0x10),
    DATANUM(DATA_mariomot, 0x11), DATANUM(DATA_mariomot, 0x12), DATANUM(DATA_mariomot, 0x16),
    DATANUM(DATA_mariomot, 0x16), 0
};
char *lbl_1_data_334[4] = { "621sambo-P1null", "621sambo-P2null", "621sambo-P3null", "621sambo-P4null" };
HuVecF lbl_1_data_344[4] = {
    { 55.0f, 420.0f, 0.0f },
    { 175.0f, 420.0f, 0.0f },
    { 401.0f, 420.0f, 0.0f },
    { 521.0f, 420.0f, 0.0f },
};

OMOBJMAN *lbl_1_bss_0;
s32 lbl_1_bss_4;
s16 lbl_1_bss_8;
int lbl_1_bss_C;
M621_PLAYER *lbl_1_bss_10[4];
void *lbl_1_bss_20;
HU3D_MOTIONID lbl_1_bss_24[4];
HU3D_MODELID lbl_1_bss_2C[4];
HU3D_MOTIONID lbl_1_bss_34;
HU3D_MODELID lbl_1_bss_36;
HU3D_MODELID lbl_1_bss_38;
HU3D_MODELID lbl_1_bss_3A;
HU3D_MODELID lbl_1_bss_3C;
HU3D_MODELID lbl_1_bss_3E;
M621_TARGET *lbl_1_bss_40[3];
void *lbl_1_bss_4C;

/* @@PART1@@ */
int _prolog(void)
{
    const VoidFunc *ctors = _ctors;
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtors = _dtors;
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void *fn_1_A0(int prio, u32 size, OMOBJ_FUNC func)
{
    OMOBJ *obj = omAddObjEx(lbl_1_bss_0, prio, 0, 0, 0, func);
    if (size != 0) {
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
        memset(obj->data, 0, size);
    } else {
        obj->data = NULL;
    }
    return obj->data;
}

void fn_1_140(OMOBJ *obj, OMOBJ_FUNC func)
{
    obj->objFunc = func;
}

void ObjectSetup(void)
{
    HU3D_LIGHTID lightId;
    lbl_1_bss_0 = MgActorObjectSetup();
    lightId = Hu3DGLightCreate(0, 1000, 1000, 1, -1, -1, 255, 255, 255);
    Hu3DGLightInfinitytSet(lightId);
    {
        HuVecF pos = { -100.0f, 8000.0f, 0.0f };
        HuVecF up = { 0.0f, 1.0f, 0.0f };
        HuVecF target = { 0.0f, 0.0f, 0.0f };
        Hu3DShadowCreate(15, 10, 10000);
        Hu3DShadowColSet(16, 16, 16);
        Hu3DShadowPosSet(&pos, &up, &target);
        Hu3DShadowTPLvlSet(0.3f);
    }
    HuPrcChildCreate(fn_1_2BC, 100, 0x3000, 0, lbl_1_bss_0);
}

void fn_1_2BC(void)
{
    MgSeqCreate(&lbl_1_data_0);
    while (TRUE) {
        MgActorExec();
        HuPrcVSleep();
    }
}

void fn_1_2E0(s16 mode, s16 frameNo)
{
    OMOBJ *obj = omAddObjEx(lbl_1_bss_0, 100, 0, 0, 0, fn_1_5F0);
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M621_MAIN), HU_MEMNUM_OVL);
    memset(obj->data, 0, sizeof(M621_MAIN));
    MgSeqModeNext();
}

void fn_1_358(s16 mode, s16 frameNo)
{
}

void fn_1_35C(s16 mode, s16 frameNo)
{
    if (lbl_1_data_28[0] == -1 && GameMesFXPlayCheck(MgSeqGameMesIdGet())) {
        lbl_1_data_28[0] = HuAudSStreamPlay(0x48);
    }
}

void fn_1_3B8(s16 mode, s16 frameNo)
{
}

void fn_1_3BC(s16 mode, s16 frameNo)
{
    if (lbl_1_data_28[0] != -1) {
        HuAudSStreamFadeOut(lbl_1_data_28[0], 100);
        lbl_1_data_28[0] = -1;
    }
}

void fn_1_410(s16 mode, s16 frameNo)
{
}

void fn_1_414(s16 mode, s16 frameNo)
{
}

void fn_1_418(s16 mode, s16 frameNo)
{
}

void fn_1_41C(s16 mode, s16 frameNo)
{
}

void fn_1_420(HuVecF *pos, s16 seNo)
{
    HuVecF pos2D;
    s16 pan;
    s16 vol;
    Hu3D3Dto2D(pos, 1, &pos2D);
    if (pos2D.x > 608.0f) {
        pan = 80;
    } else if (pos2D.x < 32.0f) {
        pan = 48;
    } else {
        pan = 48.0f + (32.0f * (pos2D.x - 32.0f)) / 576.0f;
    }
    if (pos2D.y > 400.0f) {
        vol = 127;
    } else if (pos2D.y < 40.0f) {
        vol = 96;
    } else {
        vol = 96.0f + (31.0f * (pos2D.y - 40.0f)) / 360.0f;
    }
    HuAudFXPlayVolPan(seNo, vol, pan);
}

int fn_1_594(s16 pan, s16 seNo)
{
    return HuAudFXPlayVolPan(seNo, 127, pan);
}

void fn_1_5C8(int seNo)
{
    HuAudFXStop(seNo);
}

void fn_1_5F0(OMOBJ *obj)
{
    int i;
    frandom(OSGetTick());
    lbl_1_bss_8 = _CheckFlag(0x30002);
    fn_1_A0(10, sizeof(M621_CAMERA), fn_1_B4C);
    lbl_1_bss_4C = fn_1_A0(10, sizeof(M621_STAGE), fn_1_D28);
    for (i = 0; i < 4; i++) {
        lbl_1_bss_10[i] = fn_1_A0(20, sizeof(M621_PLAYER), fn_1_13EC);
        lbl_1_bss_10[i]->playerNo = i;
    }
    lbl_1_bss_C = 0;
    fn_1_27A0();
    lbl_1_bss_20 = fn_1_A0(10, 0x54, fn_1_5054);
    fn_1_140(obj, fn_1_70C);
}

void fn_1_70C(OMOBJ *obj)
{
    M621_MAIN *work = obj->data;
    if (MgSeqModeGet() == MGSEQ_MODE_FADEIN) {
        work->seNo = fn_1_594(64, 0x6F2);
        work->timer = 0;
        fn_1_140(obj, fn_1_778);
        return;
    }
}

void fn_1_778(OMOBJ *obj)
{
    M621_MAIN *work = obj->data;
    M621_PLAYER *winner[4];
    s16 winNum;
    s16 maxScore;
    s16 i;
    if (MgSeqModeGet() == MGSEQ_MODE_FADEIN) {
        work->timer++;
        if (work->timer == 220) {
            HuAudFXStop(work->seNo);
        }
    }
    if (MgSeqModeGet() == MGSEQ_MODE_PREWIN) {
        if (lbl_1_bss_8 == 0) {
            winNum = 0;
            maxScore = 0;
            for (i = 0; i < 4; i++) {
                if (lbl_1_bss_10[i]->score > 0) {
                    if (lbl_1_bss_10[i]->score > maxScore) {
                        winner[0] = lbl_1_bss_10[i];
                        winNum = 1;
                        maxScore = lbl_1_bss_10[i]->score;
                    } else if (maxScore == lbl_1_bss_10[i]->score) {
                        winner[winNum] = lbl_1_bss_10[i];
                        winNum++;
                    }
                }
            }
            for (i = 0; i < winNum; i++) {
                winner[i]->winF = 1;
                fn_1_52D4(winner[i]->playerNo, 10);
            }
            switch (winNum) {
                case 0:
                    MgSeqWinnerSet(-1, -1, -1, -1);
                    break;
                case 1:
                    MgSeqWinnerSet(winner[0]->player->charNo, -1, -1, -1);
                    break;
                case 2:
                    MgSeqWinnerSet(winner[0]->player->charNo, winner[1]->player->charNo, -1, -1);
                    break;
                case 3:
                    MgSeqWinnerSet(winner[0]->player->charNo, winner[1]->player->charNo, winner[2]->player->charNo, -1);
                    break;
                case 4:
                    MgSeqWinnerSet(winner[0]->player->charNo, winner[1]->player->charNo, winner[2]->player->charNo, winner[3]->player->charNo);
                    break;
            }
        }
        work->timer = 0;
        fn_1_140(obj, fn_1_A5C);
        return;
    }
}

void fn_1_A5C(OMOBJ *obj)
{
    M621_MAIN *work = obj->data;
    int i;
    work->timer++;
    if ((lbl_1_bss_C >= 4 && work->timer >= 60) || work->timer >= 180) {
        if (lbl_1_bss_8) {
            for (i = 0; i < 4; i++) {
                fn_1_5328(lbl_1_bss_10[i]->playerNo, lbl_1_bss_10[i]->score);
            }
            MgSeqModeSet(MGSEQ_MODE_FADEOUT);
        } else {
            MgSeqModeNext();
        }
        fn_1_140(obj, NULL);
        return;
    }
}

void fn_1_B4C(OMOBJ *obj)
{
    M621_CAMERA *work = obj->data;
    Hu3DCameraCreate(HU3D_CAM0);
    Hu3DCameraPerspectiveSet(HU3D_CAM0, 45.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(HU3D_CAM0, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    work->motA = Hu3DMotionCreateData(DATANUM(DATA_m621, 0x19));
    work->motB = Hu3DMotionCreateData(DATANUM(DATA_m621, 0x1A));
    work->camMdlId = Hu3DModelCameraCreate(work->motA, HU3D_CAM0);
    Hu3DMotionSpeedSet(work->camMdlId, 1.0f);
    Hu3DCameraMotionStart(work->camMdlId, HU3D_CAM0);
    work->timer = 0;
    fn_1_140(obj, fn_1_C90);
}

void fn_1_C90(OMOBJ *obj)
{
    M621_CAMERA *work = obj->data;
    if (MgSeqModeGet() == MGSEQ_MODE_MAIN) {
        work->timer = 0;
        fn_1_140(obj, fn_1_CEC);
        return;
    }
}

void fn_1_CEC(OMOBJ *obj)
{
    M621_CAMERA *work = obj->data;
    fn_1_140(obj, NULL);
}

void fn_1_D28(OMOBJ *obj)
{
    M621_STAGE *work = obj->data;
    int i;
    COL_ATTRPARAM attrParam;
    work->obj = omAddObjEx(lbl_1_bss_0, 0x7FDA, 5, 2, -1, NULL);
    for (i = 0; i < 5; i++) {
        if (lbl_1_data_3C[fn_1_5340()][i] != -1) {
            work->obj->mdlId[i] = Hu3DModelCreateData(lbl_1_data_3C[fn_1_5340()][i]);
        } else {
            work->obj->mdlId[i] = 0;
        }
    }
    for (i = 0; i < 2; i++) {
        if (lbl_1_data_64[fn_1_5340()][i] != -1) {
            work->obj->mtnId[i] = Hu3DJointMotionData(work->obj->mdlId[i], lbl_1_data_64[fn_1_5340()][i]);
            Hu3DMotionSet(work->obj->mdlId[i], work->obj->mtnId[i]);
        } else {
            work->obj->mtnId[i] = 0;
        }
    }
    for (i = 0; i < 4; i++) {
        Hu3DModelAttrSet(work->obj->mdlId[i], HU3D_MOTATTR_LOOP);
    }
    for (i = 4; i < 5; i++) {
        Hu3DModelAttrSet(work->obj->mdlId[i], HU3D_ATTR_DISPOFF);
    }
    MgActorColMapInit(&work->obj->mdlId[4], 1, 40);
    MgActorColAttrParamGet(&attrParam, 1);
    attrParam.yDeviate = 0.0f;
    MgActorColAttrParamSet(&attrParam, 1);
    work->mdlId = Hu3DModelCreateData(DATANUM(DATA_m621, 0x14));
    Hu3DModelAttrSet(work->mdlId, HU3D_MOTATTR_LOOP);
    Hu3DModelScaleSet(work->mdlId, 0.6f, 0.6f, 0.6f);
    work->motId = Hu3DJointMotionData(work->mdlId, DATANUM(DATA_m621, 0x24));
    Hu3DMotionSet(work->mdlId, work->motId);
    work->hookMdlId = Hu3DModelCreateData(DATANUM(DATA_m621, 0x13));
    Hu3DModelAttrSet(work->hookMdlId, HU3D_MOTATTR_LOOP);
    Hu3DModelHookSet(work->hookMdlId, "jango_mot", work->mdlId);
    fn_1_140(obj, NULL);
}

void fn_1_106C(M621_PLAYER *work, int motNo, float shift, u32 attr)
{
    if (work->motNo != motNo) {
        work->motNo = motNo;
        CharMotionShiftSet(work->charNo, work->player->omObj->mtnId[motNo], 0.0f, shift, attr);
    }
}

int fn_1_10E8(HuVecF *in, HuVecF *out)
{
    int result;
    if (VECSquareMag(in) < 0.000001) {
        out->x = 0.01f * (frandmod(20) - 10.0f);
        out->z = 0.01f * (frandmod(20) - 10.0f);
        out->y = frandmod(1) ? 0.01f : -0.01f;
        VECNormalize(out, out);
        result = FALSE;
    } else {
        VECNormalize(in, out);
        result = TRUE;
    }
    return result;
}

int fn_1_1240(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b)
{
    M621_PLAYER *player;
    HuVecF dir;
    HuVecF up;
    HuVecF ofs;
    float dot;
    if (a->type != 0 || b->type < 5 || b->type >= 8 || b->paramB == 0) {
        return FALSE;
    }
    player = lbl_1_bss_10[a->paramB];
    VECSubtract(&b->point, &a->point, &dir);
    fn_1_10E8(&dir, &dir);
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    dot = VECDotProduct(&up, &dir);
    if (dot < -cos(1.0471975511965976)) {
        ofs = dir;
        ofs.y = 0.0f;
        if (fn_1_5350(VECSquareMag(&ofs)) < 0.001) {
            ofs.x = 1.0f;
        } else {
            fn_1_10E8(&ofs, &ofs);
        }
        VECScale(&ofs, &ofs, -50.0f);
        a->normPos = ofs;
    }
    return TRUE;
}

void fn_1_13EC(OMOBJ *obj)
{
    M621_PLAYER *work = obj->data;
    HSF_OBJECT *hsfObj;
    HU3D_MODELID mdlId;
    HuVecF pos;
    MGACTOR_PARAM param = { 0 };
    work->timer = 0;
    work->charNo = GwPlayerConf[work->playerNo].charNo;
    work->unk24 = 0;
    work->score = 0;
    work->winF = 0;
    param.height = 150.0f;
    param.radius = 40.0f;
    param.param = work->playerNo;
    param.type = 0;
    param.attr = 0;
    param.correctHookParam = 0;
    param.narrowHook = fn_1_1240;
    param.correctHook = NULL;
    work->player = MgPlayerCreate(work->playerNo, &param, 8, 1, 0x2F, lbl_1_data_2B0);
    work->comF = GwPlayerConf[work->playerNo].type;
    work->comDif = GwPlayerConf[work->playerNo].comDif;
    work->comMode = 0;
    work->target = NULL;
    work->comWait = 0;
    work->comTimer = 0;
    work->vibTimer = 0;
    MgPlayerComStkOn(work->player);
    Hu3DModelShadowSet(work->player->actor->mdlId);
    mdlId = Hu3DModelCreateData(lbl_1_data_8C[work->playerNo]);
    hsfObj = Hu3DModelObjPtrGet(mdlId, lbl_1_data_334[work->playerNo]);
    pos.x = hsfObj->mesh.base.pos.x;
    pos.y = hsfObj->mesh.base.pos.y;
    pos.z = hsfObj->mesh.base.pos.z;
    MgActorPosSet(work->player->actor, &pos);
    work->rotY = hsfObj->mesh.base.rot.y;
    MgActorRotYSet(work->player->actor, work->rotY);
    work->pos = pos;
    work->scoreBoxId = MgScoreBoxCreateChar(90, 40, work->charNo);
    MgScoreBoxPosSet(work->scoreBoxId, lbl_1_data_344[work->playerNo].x, lbl_1_data_344[work->playerNo].y);
    MgScoreBoxDispSet(work->scoreBoxId, FALSE);
    work->scoreP = MgScoreCreate(DATANUM(DATA_mgconst, 0x31), -1, TRUE);
    MgScoreMaxDigitSet(work->scoreP, 2);
    MgScoreDispOff(work->scoreP);
    MgScorePosSet(work->scoreP, 10.0f + lbl_1_data_344[work->playerNo].x, lbl_1_data_344[work->playerNo].y);
    MgScoreColorSet(work->scoreP, 255, 216, 21);
    MgScoreValueSet(work->scoreP, 0);
    CharEffectLayerSet(5);
    work->timer = 0;
    fn_1_140(obj, fn_1_175C);
}

void fn_1_175C(OMOBJ *obj)
{
    M621_PLAYER *work = obj->data;
    work->vibTimer--;
    work->timer++;
    if (work->timer == 155) {
        fn_1_106C(work, 7, 1.0f, 0);
    } else if (work->timer == 205) {
        fn_1_106C(work, 0, 5.0f, HU3D_MOTATTR_LOOP);
    }
    if (MgSeqModeGet() == MGSEQ_MODE_MAIN) {
        MgScoreBoxDispSet(work->scoreBoxId, TRUE);
        MgScoreDispOn(work->scoreP);
        MgPlayerComStkOff(work->player);
        work->timer = 0;
        fn_1_140(obj, fn_1_1844);
        return;
    }
}

void fn_1_1844(OMOBJ *obj)
{
    M621_PLAYER *work = obj->data;
    HuVecF pos;
    HuVecF start;
    HuVecF end;
    MGACTOR_COLMAP_POLY poly;
    work->vibTimer--;
    if (work->player->mode == MGPLAYER_MODE_PUNCH || work->player->mode == MGPLAYER_MODE_KICK) {
        if (work->unk24 == 0) {
            work->unk24 = 1;
        }
    } else {
        work->unk24 = 0;
        work->unk26 = 0;
        if (work->comF) {
            fn_1_215C(obj);
        }
    }
    MgActorPosGet(work->player->actor, &pos);
    end = pos;
    start = end;
    start.y = 10000.0f;
    end.y = -10000.0f;
    if (MgActorColMapPolyGet(&start, &end, -1, &poly) && pos.y < poly.pos.y) {
        pos.y = poly.pos.y;
        MgActorPosSet(work->player->actor, &pos);
    }
    if (MgSeqModeGet() == MGSEQ_MODE_FINISH) {
        MgPlayerPadSet(work->player, 0, 0, 0, 0);
        work->timer = 3;
        fn_1_140(obj, fn_1_19C4);
        return;
    }
}

void fn_1_19C4(OMOBJ *obj)
{
    M621_PLAYER *work = obj->data;
    MgPlayerPadSet(work->player, 0, 0, 0, 0);
    if (work->player->motNo == MGPLAYER_MOT_IDLE) {
        MgPlayerComStkOn(work->player);
        fn_1_140(obj, fn_1_1A3C);
        return;
    }
}

void fn_1_1A3C(OMOBJ *obj)
{
    M621_PLAYER *work = obj->data;
    float rotY;
    MgPlayerPadSet(work->player, 0, 0, 0, 0);
    MgActorRotYGet(work->player->actor, &rotY);
    if (rotY == work->rotY) {
        lbl_1_bss_C++;
        fn_1_140(obj, fn_1_1B5C);
    } else {
        if (rotY < work->rotY) {
            rotY += 6.0f;
            if (rotY > work->rotY) {
                rotY = work->rotY;
            }
        } else {
            rotY -= 6.0f;
            if (rotY < work->rotY) {
                rotY = work->rotY;
            }
        }
        MgActorRotYSet(work->player->actor, rotY);
    }
}

void fn_1_1B5C(OMOBJ *obj)
{
    M621_PLAYER *work = obj->data;
    MgPlayerPadSet(work->player, 0, 0, 0, 0);
    if (lbl_1_bss_8) {
        fn_1_140(obj, NULL);
    } else if (MgSeqModeGet() == MGSEQ_MODE_WINNER) {
        work->timer = 0;
        fn_1_140(obj, fn_1_1BF4);
        return;
    }
}

void fn_1_1BF4(OMOBJ *obj)
{
    M621_PLAYER *work = obj->data;
    if (work->winF) {
        fn_1_106C(work, 8, 4.0f, 0);
    } else {
        fn_1_106C(work, 9, 4.0f, 0);
    }
    fn_1_140(obj, NULL);
}

M621_TARGET *fn_1_1C7C(void)
{
    int i = 3.0f * frandf();
    if (i >= 3) {
        i = 0;
    }
    if (lbl_1_bss_40[i]->state != 1) {
        return NULL;
    }
    return lbl_1_bss_40[i];
}

M621_TARGET *fn_1_1D08(M621_PLAYER *work)
{
    M621_TARGET *target = NULL;
    int i;
    float minDist;
    float dist;
    HuVecF pos;
    HuVecF dir;
    s8 used[3];
    if (frandf() < 0.55f) {
        MgActorPosGet(work->player->actor, &pos);
        for (i = 0; i < 3; i++) {
            if (lbl_1_bss_40[i]->state == 1) {
                if (target == NULL) {
                    target = lbl_1_bss_40[i];
                    VECSubtract(&target->pos, &pos, &dir);
                    dir.y = 0.0f;
                    minDist = VECSquareMag(&dir);
                } else {
                    VECSubtract(&lbl_1_bss_40[i]->pos, &pos, &dir);
                    dir.y = 0.0f;
                    dist = VECSquareMag(&dir);
                    if (dist < minDist) {
                        target = lbl_1_bss_40[i];
                        minDist = dist;
                    }
                }
            }
        }
    } else {
        for (i = 0; i < 3; i++) {
            used[i] = 0;
        }
        for (i = 0; i < 4; i++) {
            if (lbl_1_bss_10[i]->comF && lbl_1_bss_10[i]->target != NULL && lbl_1_bss_10[i]->target->state == 1) {
                used[lbl_1_bss_10[i]->target->no] = 1;
            }
        }
        for (i = 0; i < 3; i++) {
            if (used[i] == 0 && lbl_1_bss_40[i]->state == 1) {
                target = lbl_1_bss_40[i];
                break;
            }
        }
    }
    return target;
}

s16 fn_1_1F9C(M621_PLAYER *work)
{
    switch (work->comDif) {
        case 0:
            return (int)(60.0f * frandf()) + 120;
        case 1:
            return (int)(60.0f * frandf()) + 60;
        case 2:
            return (int)(50.0f * frandf()) + 50;
    }
    return 0;
}

s16 fn_1_2074(M621_PLAYER *work)
{
    HuVecF targetPos;
    HuVecF pos;
    HuVecF dir;
    if (work->comDif < 2) {
        return FALSE;
    }
    if (work->target == NULL || work->target->state != 1) {
        return FALSE;
    }
    MgActorPosGet(work->target->item[0].actor, &targetPos);
    MgActorPosGet(work->player->actor, &pos);
    VECSubtract(&targetPos, &pos, &dir);
    if (dir.y < 250.0f && dir.x * dir.x + dir.z * dir.z < 40000.0f) {
        return TRUE;
    }
    return FALSE;
}

void fn_1_215C(OMOBJ *obj)
{
    M621_PLAYER *work = obj->data;
    float dy;
    HuVecF actorPos;
    HuVecF pos;
    HuVecF targetPos;
    HuVecF dir;
    HuVecF stick;
    if (work->target == NULL || work->target->state != 1) {
        work->target = fn_1_1D08(work);
        work->comWait = fn_1_1F9C(work) / 2;
        work->comTimer = 0;
    }
    if (work->target == NULL) {
        return;
    }
    MgActorPosGet(work->player->actor, &pos);
    switch (work->comMode) {
        case 0:
            targetPos = work->target->pos;
            VECSubtract(&targetPos, &pos, &dir);
            dy = dir.y;
            VECNormalize(&dir, &stick);
            VECScale(&stick, &stick, 20.0f);
            stick.z = (int)stick.z;
            stick.x = (int)stick.x;
            VECScale(&stick, &stick, 4.0f);
            if (work->comWait > 0) {
                work->comWait--;
            }
            if (work->comWait <= 0 && fn_1_2074(work)) {
                MgPlayerPadSet(work->player, stick.x, -stick.z, PAD_BUTTON_A, PAD_BUTTON_A);
                work->comMode = 1;
                work->comTimer = 0;
            } else if (VECSquareMag(&dir) < 14400.0f) {
                if (work->comWait <= 0) {
                    MgPlayerPadSet(work->player, stick.x, -stick.z, PAD_BUTTON_B, PAD_BUTTON_B);
                    work->comWait = fn_1_1F9C(work);
                } else {
                    MgPlayerPadSet(work->player, stick.x, -stick.z, 0, 0);
                }
                work->comTimer = 0;
            } else if (work->player->actor->colGroundAttr & 0x100) {
                MgPlayerPadSet(work->player, stick.x, -stick.z, PAD_BUTTON_A, PAD_BUTTON_A);
                work->comMode = 2;
            } else {
                MgPlayerPadSet(work->player, stick.x, -stick.z, 0, 0);
                work->comTimer++;
                if (work->comTimer > 40) {
                    work->target = NULL;
                }
            }
            break;
        case 1:
            MgActorPosGet(work->target->item[0].actor, &actorPos);
            VECSubtract(&actorPos, &pos, &dir);
            dy = dir.y;
            VECNormalize(&dir, &dir);
            VECScale(&dir, &dir, 40.0f);
            if (work->player->actor->velY < 0.0f && dy < 150.0f) {
                MgPlayerPadSet(work->player, dir.x, -dir.z, PAD_BUTTON_B, PAD_BUTTON_B);
                work->comMode = 0;
            } else {
                MgPlayerPadSet(work->player, dir.x, -dir.z, 0, PAD_BUTTON_A);
                if (!MgPlayerModeAttrCheck(work->player, MGPLAYER_MODEATTR_AIR)) {
                    work->comMode = 0;
                }
            }
            break;
        case 2:
            MgActorPosGet(work->target->item[0].actor, &actorPos);
            VECSubtract(&actorPos, &pos, &dir);
            dy = dir.y;
            VECNormalize(&dir, &dir);
            VECScale(&dir, &dir, 80.0f);
            MgPlayerPadSet(work->player, dir.x, -dir.z, 0, PAD_BUTTON_A);
            if (!MgPlayerModeAttrCheck(work->player, MGPLAYER_MODEATTR_AIR)) {
                work->comMode = 0;
            }
            break;
    }
}

u32 fn_1_26A4(void)
{
    u32 bit;
    for (bit = 0x80000000; bit != 1; bit >>= 1) {
        if (bit & lbl_1_data_2AC) {
            lbl_1_data_2AC &= ~bit;
            return bit;
        }
    }
    return 0;
}

void fn_1_270C(u32 bit)
{
    lbl_1_data_2AC |= bit;
}

s16 fn_1_272C(void)
{
    s16 num = 0;
    u32 bit;
    for (bit = 0x80000000; bit != 1; bit >>= 1) {
        if (bit & lbl_1_data_2AC) {
            num++;
        }
    }
    return (num > 23) ? 23 : num;
}

void fn_1_27A0(void)
{
    int i;
    lbl_1_bss_3E = Hu3DModelCreateData(DATANUM(DATA_m621, 0x0C));
    lbl_1_bss_3C = Hu3DModelCreateData(DATANUM(DATA_m621, 0x0D));
    lbl_1_bss_3A = Hu3DModelCreateData(DATANUM(DATA_m621, 0x0E));
    lbl_1_bss_38 = Hu3DModelCreateData(lbl_1_data_74[fn_1_5340()][0]);
    lbl_1_bss_36 = Hu3DModelCreateData(lbl_1_data_74[fn_1_5340()][1]);
    lbl_1_bss_34 = Hu3DJointMotionData(lbl_1_bss_36, lbl_1_data_74[fn_1_5340()][2]);
    Hu3DModelAttrSet(lbl_1_bss_3E, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(lbl_1_bss_3C, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(lbl_1_bss_38, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(lbl_1_bss_36, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(lbl_1_bss_3A, HU3D_ATTR_DISPOFF);
    for (i = 0; i < 3; i++) {
        lbl_1_bss_40[i] = fn_1_A0(30, sizeof(M621_TARGET), fn_1_29E8);
        lbl_1_bss_40[i]->no = i;
        lbl_1_bss_40[i]->unk8 = 1;
        lbl_1_bss_40[i]->mdlId = Hu3DModelLink(lbl_1_bss_38);
    }
}

void fn_1_29E8(OMOBJ *obj)
{
    M621_TARGET *work = obj->data;
    if (MgSeqModeGet() == MGSEQ_MODE_FADEIN) {
        work->state = 0;
        work->timer = 150;
        work->unk2C = -1;
        fn_1_140(obj, fn_1_3678);
        return;
    }
}

void fn_1_2A54(M621_PLAYER *player, M621_TARGET *target)
{
    M621_ITEM *item;
    int delay;
    int num;
    int i;
    HuVecF pos;
    delay = 0;
    if (target->item[0].state == 2) {
        target->timer = 0;
        item = &target->item[0];
        VECSubtract(&item->actor->pos, &player->player->actor->pos, &pos);
        item->vel = pos;
        item->vel.y = 10.0f;
        VECNormalize(&item->vel, &item->vel);
        VECScale(&item->vel, &item->vel, 20.0f);
        MgActorColAttrSet(item->actor, 4);
        MgActorColAttrReset(item->actor, 2);
        item->timer = 0;
        item->state = 3;
        MgActorPosGet(item->actor, &pos);
        fn_1_420(&pos, 0x6F0);
        pos.y += 75.0f;
        pos.z += 80.0f;
        fn_1_51F4(player->playerNo, &pos);
        num = 1;
        for (i = 1; i < target->itemNum; i++) {
            item = &target->item[i];
            if (item->state == 2) {
                MgActorPushSet(item->actor, &lbl_1_data_30);
                MgActorColAttrSet(item->actor, 4);
                item->timer = delay;
                delay += 10;
                item->state = 4;
                item->playerNo = player->playerNo;
                num++;
            }
        }
        MgActorKill(target->actor);
        target->actor = NULL;
        target->state = 2;
        player->score += num;
        if (player->score > 99) {
            player->score = 99;
        }
        MgScoreValueSet(player->scoreP, player->score);
        if (player->vibTimer <= 0) {
            omVibrate(player->player->playerNo, 10, 10, 0);
            player->vibTimer = 0;
        }
    }
}

void fn_1_2C6C(M621_PLAYER *player, M621_TARGET *target, s16 itemNo)
{
    M621_ITEM *item = &target->item[itemNo];
    HuVecF pos;
    if (target->item[0].state == 2 && item->state == 2) {
        VECSubtract(&item->actor->pos, &player->player->actor->pos, &pos);
        item->vel = pos;
        item->vel.y = 20.0f;
        VECNormalize(&item->vel, &item->vel);
        VECScale(&item->vel, &item->vel, 20.0f);
        MgActorColAttrSet(item->actor, 4);
        MgActorColAttrReset(item->actor, 2);
        item->timer = 0;
        item->state = 3;
        MgActorPosGet(item->actor, &pos);
        fn_1_420(&pos, 0x6F0);
        pos.y += 50.0f;
        pos.z += 55.0f;
        fn_1_51F4(player->playerNo, &pos);
        player->score++;
        if (player->score > 99) {
            player->score = 99;
        }
        MgScoreValueSet(player->scoreP, player->score);
        if (player->vibTimer <= 0) {
            omVibrate(player->player->playerNo, 10, 10, 0);
            player->vibTimer = 0;
        }
    }
}

s16 lbl_1_data_37E = 1;
s16 lbl_1_data_380 = 1;

/* @@PART2@@ */
#pragma auto_inline off

#include "humath.h"

typedef struct M621Effect_s {
    s16 state;
    s16 mdlId;
} M621EFFECT;

extern s16 lbl_1_data_37E;
extern s16 lbl_1_data_380;

void fn_1_390C(OMOBJ *obj);
void fn_1_3A78(OMOBJ *obj);
void fn_1_3E18(OMOBJ *obj);
void fn_1_47B4(OMOBJ *obj);
void fn_1_4CB0(OMOBJ *obj);
void fn_1_5168(OMOBJ *obj);
void fn_1_51F4(s16 type, HuVecF *pos);

void fn_1_2E10(M621_ITEM *seg)
{
    HuVecF pos;
    seg->state = 5;
    MgActorPosGet(seg->actor, &pos);
    seg->mdlId = Hu3DModelLink(lbl_1_bss_36);
    seg->timer = 5;
}

int fn_1_2E68(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b)
{
    M621_PLAYER *player;
    M621_TARGET *tower;
    int i;
    int j;
    if (b->type == 2 || b->type == 3) {
        player = NULL;
        tower = lbl_1_bss_40[a->type - 5];
        for (j = 0; j < 4; j++) {
            if (b->paramB - 0x100 == lbl_1_bss_10[j]->player->actor->no) {
                player = lbl_1_bss_10[j];
                break;
            }
        }
        if (player->unk24 == 2) {
            return 0;
        }
        player->unk24 = 2;
        if (player->player->actor->colGroundAttr & 1) {
            for (i = tower->itemNum - 1; i >= 0; i--) {
                if (tower->item[i].state == 2) {
                    if (i == 0) {
                        fn_1_2A54(player, tower);
                    } else {
                        fn_1_2C6C(player, tower, i);
                    }
                    break;
                }
            }
        } else if (a->paramB == 1) {
            fn_1_2A54(player, tower);
        } else {
            fn_1_2C6C(player, tower, a->paramB - 1);
        }
    }
    return 0;
}

int fn_1_2FFC(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b)
{
    if (b->type >= 5 && b->type < 8) {
        return 1;
    }
    return 0;
}

void fn_1_3024(MGACTOR *actor, int param)
{
    M621_ITEM *seg = (M621_ITEM *)param;
    if (seg->state == 3) {
        fn_1_2E10(seg);
    } else if (seg->state == 2) {
        seg->vel.y = 0.0f;
    }
}
void fn_1_3084(M621_TARGET *tower)
{
    HuVecF pos;
    MGACTOR_PARAM param;
    int num;
    int i;

    num = fn_1_272C();
    if (tower->unk8 != 0) {
        tower->itemNum = (int)(5.0f * frandf()) + 3;
        tower->unk14 = 0.0f;
    } else {
        if (num >= 7) {
            tower->itemNum = (int)(5.0f * frandf()) + 3;
            if (tower->itemNum > 7) {
                tower->itemNum = 7;
            }
        } else {
            tower->itemNum = num;
        }
        tower->unk14 = 360.0f * frandf();
    }
    tower->height = 90.0f * (tower->itemNum + 1);
    tower->unkC = 30;
    param.height = 150.0f + (80.0f + 90.0f * (tower->itemNum - 2));
    param.radius = 130.0f;
    param.type = tower->no + 5;
    param.param = 0;
    param.narrowHook = fn_1_2FFC;
    param.correctHook = NULL;
    param.correctHookParam = 0;
    param.attr = 2;
    tower->actor = MgActorCreate(&param, -1);
    MgActorColMaskSet(tower->actor, 1);
    pos = tower->pos;
    pos.y -= tower->height;
    MgActorPosSet(tower->actor, &pos);
    param.height = 90.0f;
    param.radius = 60.0f;
    param.type = tower->no + 5;
    param.narrowHook = fn_1_2E68;
    param.correctHook = fn_1_3024;
    param.attr = 2;
    tower->item[0].itemMdlId = Hu3DModelLink(lbl_1_bss_3E);
    tower->item[0].unk4 = fn_1_26A4();
    tower->item[0].vel.x = tower->item[0].vel.y = tower->item[0].vel.z = 0.0f;
    Hu3DModelAttrReset(tower->item[0].itemMdlId, HU3D_ATTR_DISPOFF);
    Hu3DModelShadowSet(tower->item[0].itemMdlId);
    param.height = 150.0f;
    param.param = 1;
    param.correctHookParam = (int)&tower->item[0];
    tower->item[0].actor = MgActorCreate(&param, tower->item[0].itemMdlId);
    MgActorColMaskSet(tower->item[0].actor, tower->item[0].unk4);
    tower->rotY = tower->unk14;
    if (tower->rotY > 70.0f) {
        tower->rotY = 70.0f;
    } else if (tower->rotY < -70.0f) {
        tower->rotY = -70.0f;
    }
    MgActorRotYSet(tower->item[0].actor, tower->rotY);
    pos = tower->pos;
    pos.y += (80.0f + 90.0f * (tower->itemNum - 2)) - tower->height;
    MgActorPosSet(tower->item[0].actor, &pos);
    tower->item[0].actor->gravity = 0.0f;
    tower->item[0].state = 1;
    for (i = 1; i < tower->itemNum; i++) {
        tower->item[i].itemMdlId = Hu3DModelLink(lbl_1_bss_3C);
        tower->item[i].unk4 = fn_1_26A4();
        tower->item[i].vel.x = tower->item[i].vel.y = tower->item[i].vel.z = 0.0f;
        Hu3DModelAttrReset(tower->item[i].itemMdlId, HU3D_ATTR_DISPOFF);
        Hu3DModelShadowSet(tower->item[i].itemMdlId);
        param.height = (i == 1) ? 80.0f : 90.0f;
        param.param = i + 1;
        param.correctHookParam = (int)&tower->item[i];
        tower->item[i].actor = MgActorCreate(&param, tower->item[i].itemMdlId);
        MgActorColMaskSet(tower->item[i].actor, tower->item[i].unk4);
        pos = tower->pos;
        pos.y += (90.0f * (tower->itemNum - i - 1)) - tower->height;
        MgActorPosSet(tower->item[i].actor, &pos);
        tower->item[i].actor->gravity = 0.0f;
        MgActorRotYSet(tower->item[i].actor, tower->unk14);
        tower->item[i].state = 1;
    }
}

void fn_1_3678(OMOBJ *obj)
{
    M621_TARGET *tower = obj->data;
    HSF_OBJECT *hsfObj;
    int i;
    HuVecF dir;

    if (MgSeqModeGet() == 6 || MgSeqModeGet() == 7 || MgSeqModeGet() == 8) {
        fn_1_140(obj, NULL);
        return;
    }
    tower->timer--;
    if ((u32)MgSeqTimerValueGet() <= tower->timer + 60) {
        fn_1_140(obj, NULL);
        return;
    }
    if (tower->timer < 30) {
        do {
            tower->unk2C = 21.0f * frandf();
            hsfObj = Hu3DModelObjPtrGet(lbl_1_bss_3A, lbl_1_data_258[tower->unk2C]);
            tower->pos.x = hsfObj->mesh.base.pos.x;
            tower->pos.y = hsfObj->mesh.base.pos.y;
            tower->pos.z = hsfObj->mesh.base.pos.z;
            for (i = 0; i < 3; i++) {
                if (tower->no != lbl_1_bss_40[i]->no && lbl_1_bss_40[i]->unk2C >= 0) {
                    PSVECSubtract(&tower->pos, &lbl_1_bss_40[i]->pos, &dir);
                    dir.y = 0.0f;
                    if (PSVECSquareMag(&dir) < 62500.0f) {
                        tower->unk2C = 22;
                        break;
                    }
                }
            }
        } while (tower->unk2C >= 21);
        Hu3DModelPosSetV(tower->mdlId, &tower->pos);
        Hu3DModelAttrReset(tower->mdlId, HU3D_ATTR_DISPOFF);
        Hu3DMotionTimeSet(tower->mdlId, 0.0f);
        Hu3DModelLayerSet(tower->mdlId, 1);
        if (tower->unk8 != 0) {
            for (i = 0; i < 4; i++) {
                lbl_1_bss_10[i]->vibTimer = 30;
                omVibrate(lbl_1_bss_10[i]->player->playerNo, 30, 4, 4);
            }
        }
        fn_1_140(obj, fn_1_390C);
        return;
    }
}

void fn_1_390C(OMOBJ *obj)
{
    M621_TARGET *tower = obj->data;
    int i;

    if (MgSeqModeGet() == 6 || MgSeqModeGet() == 7 || MgSeqModeGet() == 8) {
        fn_1_140(obj, NULL);
        return;
    }
    tower->timer--;
    if (tower->timer <= 0) {
        fn_1_3084(tower);
        tower->timer = 0;
        if (tower->unk8 != 0) {
            for (i = 0; i < 4; i++) {
                lbl_1_bss_10[i]->vibTimer = tower->itemNum * 6;
                omVibrate(lbl_1_bss_10[i]->player->playerNo, tower->itemNum * 6, 4, 4);
            }
            tower->unk8 = 0;
            if (lbl_1_data_37E != 0) {
                fn_1_594(64, 0x6EF);
                lbl_1_data_37E = 0;
            }
        } else {
            fn_1_420(&tower->pos, 0x6EF);
        }
        fn_1_140(obj, fn_1_3A78);
        return;
    }
}

void fn_1_3A78(OMOBJ *obj)
{
    M621_TARGET *tower = obj->data;
    HuVecF pos;
    s16 i;
    float t;
    float ofs;

    if (MgSeqModeGet() == 6 || MgSeqModeGet() == 7 || MgSeqModeGet() == 8) {
        tower->timer = 0;
        fn_1_140(obj, fn_1_4CB0);
        return;
    }
    tower->timer++;
    t = 1.0f - tower->timer / (float)(tower->itemNum * 6);
    if (t < 0.0f) {
        t = 0.0f;
    }
    ofs = 25.0f * ((s16)(tower->height * t) / 25);
    for (i = 0; i < tower->itemNum; i++) {
        pos = tower->pos;
        if (i == 0) {
            pos.y += 1.0f + ((80.0f + 90.0f * (tower->itemNum - 2)) - ofs);
        } else {
            pos.y += 1.0f + (90.0f * (tower->itemNum - i - 1) - ofs);
        }
        MgActorPosSet(tower->item[i].actor, &pos);
    }
    pos = tower->pos;
    pos.y = 1.0f + (pos.y - ofs);
    MgActorPosSet(tower->actor, &pos);
    if (t <= 0.0f) {
        for (i = 0; i < tower->itemNum; i++) {
            tower->item[i].timer = 0;
            tower->item[i].state = 2;
        }
        MgActorColAttrReset(tower->item[tower->itemNum - 1].actor, 2);
        MgActorColAttrReset(tower->actor, 2);
        tower->state = 1;
        fn_1_140(obj, fn_1_3E18);
        return;
    }
}
void fn_1_3E18(OMOBJ *obj)
{
    MGACTOR_COLMAP_POLY poly;
    HuVecF pos;
    HuVecF nextPos;
    HuVecF push;
    HuVecF actorPos;
    HuVecF rayStart;
    HuVecF rayEnd;
    M621_ITEM *segP;
    s16 cnt0;
    M621_TARGET *tower;
    M621_ITEM *nextSegP;
    s16 j;
    s16 cnt2;
    s16 i;
    M621_ITEM *seg;
    float maxTop;
    float top;
    float wave;

    tower = obj->data;
    cnt0 = 0;
    cnt2 = 0;
    maxTop = 0.0f;
    if (MgSeqModeGet() == 6 || MgSeqModeGet() == 7 || MgSeqModeGet() == 8) {
        tower->timer = 0;
        fn_1_140(obj, fn_1_47B4);
        return;
    }
    if (Hu3DMotionEndCheck(tower->mdlId)) {
        Hu3DModelAttrSet(tower->mdlId, HU3D_ATTR_DISPOFF);
    }
    for (i = 0; i < tower->itemNum; i++) {
        segP = &tower->item[i];
        switch (segP->state) {
            case 0:
                cnt0++;
                break;
            case 2:
                cnt2++;
                break;
        }
    }
    if (cnt0 == tower->itemNum) {
        tower->state = 2;
        if ((u32)MgSeqTimerValueGet() / 60 <= 1) {
            fn_1_140(obj, NULL);
            return;
        }
        tower->timer = 30.0f + 90.0f * frandf();
        tower->unk2C = -1;
        fn_1_140(obj, fn_1_3678);
        return;
    }
    if (MgSeqModeGet() == 5) {
        tower->unkC--;
        if (tower->unkC <= 0) {
            tower->unk18 = frandf();
            tower->unkC = 30;
        }
        if (tower->unk18 < 0.4f) {
            tower->unk14 -= 0.3f;
            if (tower->unk14 < -180.0f) {
                tower->unk14 = 360.0f + tower->unk14;
            }
            tower->rotY -= 0.3f;
            if (tower->rotY < -70.0f) {
                tower->rotY = -70.0f;
            }
        } else if (tower->unk18 > 0.6f) {
            tower->unk14 += 0.3f;
            if (tower->unk14 > 180.0f) {
                tower->unk14 -= 360.0f;
            }
            tower->rotY += 0.3f;
            if (tower->rotY > 70.0f) {
                tower->rotY = 70.0f;
            }
        }
        push.x = push.y = 0.0f;
        push.z = 1.0f;
        if (tower->item[0].state == 2) {
            MgActorRotYSet(tower->item[0].actor, tower->rotY);
        }
        for (i = 1; i < tower->itemNum; i++) {
            if (tower->item[i].state == 2) {
                MgActorRotYSet(tower->item[i].actor, tower->unk14);
            }
        }
    } else {
        push.x = push.y = push.z = 0.0f;
    }
    if (tower->actor != NULL) {
        MgActorPosGet(tower->actor, &tower->pos);
        MgActorRotYSet(tower->actor, tower->unk14);
        MgActorPushSet(tower->actor, &push);
    }
    for (i = 0; i < tower->itemNum; i++) {
        seg = &tower->item[i];
        nextSegP = NULL;
        for (j = i + 1; j < tower->itemNum; j++) {
            if (tower->item[j].state == 2) {
                nextSegP = &tower->item[j];
                break;
            }
        }
        switch (seg->state) {
            case 2:
                seg->timer++;
                cnt2--;
                wave = 10.0 * HuSin((360.0f * seg->timer) / 120.0f + 120.0f * cnt2);
                if (nextSegP != NULL) {
                    seg->vel.y -= 0.7f;
                    MgActorPosGet(seg->actor, &pos);
                    PSVECAdd(&pos, &seg->vel, &pos);
                    MgActorPosGet(nextSegP->actor, &nextPos);
                    if (i == 0 && 80.0f + nextPos.y >= pos.y) {
                        pos.y = 80.0f + nextPos.y;
                        seg->vel.y = 0.0f;
                    } else if (i != 0 && 90.0f + nextPos.y >= pos.y) {
                        pos.y = 90.0f + nextPos.y;
                        seg->vel.y = 0.0f;
                    }
                    pos.x = tower->pos.x + wave;
                    pos.z = tower->pos.z;
                    MgActorPosSetRaw(seg->actor, &pos);
                    top = (pos.y - tower->pos.y) + ((i == 0) ? 150.0f : 90.0f);
                } else {
                    MgActorPosGet(seg->actor, &actorPos);
                    rayStart = rayEnd = actorPos;
                    rayStart.y = 10000.0f;
                    rayEnd.y = -10000.0f;
                    if (MgActorColMapPolyGet(&rayStart, &rayEnd, -1, &poly)) {
                        if (actorPos.y < poly.pos.y) {
                            actorPos.y = poly.pos.y;
                            MgActorPosSet(seg->actor, &actorPos);
                        } else if (actorPos.y > poly.pos.y) {
                            MgActorColAttrReset(seg->actor, 2);
                            seg->actor->gravity = 150.0f;
                            MgActorPosGet(seg->actor, &pos);
                            pos.x = tower->pos.x + wave;
                            pos.z = tower->pos.z;
                            MgActorPosSetRaw(seg->actor, &pos);
                        }
                    }
                    top = (i == 0) ? 150.0f : 90.0f;
                }
                if (top > maxTop) {
                    maxTop = top;
                }
                break;
            case 3:
                seg->timer++;
                if (seg->timer % 2 == 0) {
                    Hu3DModelAttrReset(seg->itemMdlId, HU3D_ATTR_DISPOFF);
                } else {
                    Hu3DModelAttrSet(seg->itemMdlId, HU3D_ATTR_DISPOFF);
                }
                seg->vel.y -= 0.7f;
                MgActorPosGet(seg->actor, &pos);
                PSVECAdd(&pos, &seg->vel, &pos);
                MgActorPosSetRaw(seg->actor, &pos);
                break;
            case 4:
                seg->timer--;
                if (seg->timer <= 0) {
                    MgActorPosGet(seg->actor, &pos);
                    fn_1_420(&pos, 0x6F0);
                    pos.y += 50.0f;
                    pos.z += 55.0f;
                    fn_1_51F4(seg->playerNo, &pos);
                    fn_1_2E10(seg);
                }
                break;
            case 5:
                seg->timer--;
                if (seg->timer == 0) {
                    MgActorKill(seg->actor);
                    Hu3DModelKill(seg->itemMdlId);
                    seg->state = 0;
                    fn_1_270C(seg->unk4);
                }
                break;
        }
    }
    if (tower->actor != NULL) {
        ColBodyGetSafe(tower->actor->no)->param.height = maxTop;
    }
}
void fn_1_47B4(OMOBJ *obj)
{
    HuVecF pos;
    HuVecF nextPos;
    M621_ITEM *segP;
    s16 cnt0;
    s16 cnt2;
    M621_ITEM *nextSegP;
    s16 j;
    s16 i;
    M621_TARGET *tower;
    M621_ITEM *seg;
    float wave;

    tower = obj->data;
    cnt0 = 0;
    cnt2 = 0;
    for (i = 0; i < tower->itemNum; i++) {
        segP = &tower->item[i];
        switch (segP->state) {
            case 0:
                cnt0++;
                break;
        }
    }
    if (cnt0 == tower->itemNum) {
        fn_1_140(obj, NULL);
        return;
    }
    if (tower->actor != NULL) {
        MgActorPushSet(tower->actor, &lbl_1_data_30);
    }
    if (tower->timer == 0) {
        Hu3DModelPosSetV(tower->mdlId, &tower->pos);
        Hu3DModelAttrReset(tower->mdlId, HU3D_ATTR_DISPOFF);
        Hu3DMotionTimeSet(tower->mdlId, 0.0f);
        Hu3DModelLayerSet(tower->mdlId, 1);
    }
    tower->timer++;
    if (tower->timer >= 30) {
        tower->timer = 0;
        if (lbl_1_data_380 != 0) {
            fn_1_594(64, 0x6F1);
            lbl_1_data_380 = 0;
        }
        fn_1_140(obj, fn_1_4CB0);
        return;
    }
    for (i = 0; i < tower->itemNum; i++) {
        seg = &tower->item[i];
        nextSegP = NULL;
        for (j = i + 1; j < tower->itemNum; j++) {
            if (tower->item[j].state == 2) {
                nextSegP = &tower->item[j];
                break;
            }
        }
        switch (seg->state) {
            case 2:
                seg->timer++;
                cnt2--;
                wave = 10.0 * HuSin((360.0f * seg->timer) / 120.0f + 120.0f * cnt2);
                if (nextSegP != NULL) {
                    seg->vel.y -= 0.7f;
                    MgActorPosGet(seg->actor, &pos);
                    PSVECAdd(&pos, &seg->vel, &pos);
                    MgActorPosGet(nextSegP->actor, &nextPos);
                    if (i == 0 && 80.0f + nextPos.y >= pos.y) {
                        pos.y = 80.0f + nextPos.y;
                        seg->vel.y = 0.0f;
                    } else if (i != 0 && 90.0f + nextPos.y >= pos.y) {
                        pos.y = 90.0f + nextPos.y;
                        seg->vel.y = 0.0f;
                    }
                    pos.x = tower->pos.x + wave;
                    pos.z = tower->pos.z;
                    MgActorPosSetRaw(seg->actor, &pos);
                } else {
                    MgActorColAttrReset(seg->actor, 2);
                    seg->actor->gravity = 150.0f;
                }
                break;
            case 3:
                seg->timer++;
                if (seg->timer % 2 == 0) {
                    Hu3DModelAttrReset(seg->itemMdlId, HU3D_ATTR_DISPOFF);
                } else {
                    Hu3DModelAttrSet(seg->itemMdlId, HU3D_ATTR_DISPOFF);
                }
                seg->vel.y -= 0.7f;
                MgActorPosGet(seg->actor, &pos);
                PSVECAdd(&pos, &seg->vel, &pos);
                MgActorPosSetRaw(seg->actor, &pos);
                break;
            case 4:
                fn_1_2E10(seg);
                break;
            case 5:
                seg->timer--;
                if (seg->timer == 0) {
                    MgActorKill(seg->actor);
                    Hu3DModelKill(seg->itemMdlId);
                    seg->state = 0;
                    fn_1_270C(seg->unk4);
                }
                break;
        }
    }
}
void fn_1_4CB0(OMOBJ *obj)
{
    HuVecF pos;
    HuVecF nextPos;
    s16 cnt2;
    M621_ITEM *segP;
    s16 cnt0;
    M621_ITEM *nextSegP;
    s16 j;
    s16 i;
    M621_TARGET *tower;
    M621_ITEM *seg;

    tower = obj->data;
    cnt0 = 0;
    cnt2 = 0;
    for (i = 0; i < tower->itemNum; i++) {
        segP = &tower->item[i];
        switch (segP->state) {
            case 0:
                cnt0++;
                break;
        }
    }
    if (cnt0 == tower->itemNum) {
        fn_1_140(obj, NULL);
        return;
    }
    for (i = 0; i < tower->itemNum; i++) {
        seg = &tower->item[i];
        nextSegP = NULL;
        for (j = i + 1; j < tower->itemNum; j++) {
            if (tower->item[j].state == 2) {
                nextSegP = &tower->item[j];
                break;
            }
        }
        switch (seg->state) {
            case 2:
                seg->timer++;
                cnt2--;
                MgActorColAttrSet(seg->actor, 2);
                seg->actor->gravity = 150.0f;
                if (nextSegP != NULL) {
                    seg->vel.y -= 0.7f;
                    MgActorPosGet(seg->actor, &pos);
                    PSVECAdd(&pos, &seg->vel, &pos);
                    MgActorPosGet(nextSegP->actor, &nextPos);
                    if (i == 0 && 80.0f + nextPos.y >= pos.y) {
                        pos.y = 80.0f + nextPos.y;
                        seg->vel.y = 0.0f;
                    } else if (i != 0 && 90.0f + nextPos.y >= pos.y) {
                        pos.y = 90.0f + nextPos.y;
                        seg->vel.y = 0.0f;
                    }
                    pos.x = tower->pos.x;
                    pos.z = tower->pos.z;
                    MgActorPosSetRaw(seg->actor, &pos);
                } else {
                    seg->actor->gravity = 150.0f;
                    MgActorPosGet(seg->actor, &pos);
                }
                if (pos.y < -200.0f) {
                    fn_1_2E10(seg);
                }
                break;
            case 3:
                seg->timer++;
                if (seg->timer % 2 == 0) {
                    Hu3DModelAttrReset(seg->itemMdlId, HU3D_ATTR_DISPOFF);
                } else {
                    Hu3DModelAttrSet(seg->itemMdlId, HU3D_ATTR_DISPOFF);
                }
                seg->vel.y -= 0.7f;
                MgActorPosGet(seg->actor, &pos);
                PSVECAdd(&pos, &seg->vel, &pos);
                MgActorPosSetRaw(seg->actor, &pos);
                break;
            case 4:
                fn_1_2E10(seg);
                break;
            case 5:
                seg->timer--;
                if (seg->timer == 0) {
                    MgActorKill(seg->actor);
                    Hu3DModelKill(seg->itemMdlId);
                    seg->state = 0;
                    fn_1_270C(seg->unk4);
                }
                break;
        }
    }
}
void fn_1_5054(OMOBJ *obj)
{
    M621EFFECT *effect = obj->data;
    int i;

    for (i = 0; i < 4; i++) {
        lbl_1_bss_2C[i] = Hu3DModelCreateData(lbl_1_data_9C[i][0]);
        lbl_1_bss_24[i] = Hu3DJointMotionData(lbl_1_bss_2C[i], lbl_1_data_9C[i][1]);
        Hu3DModelAttrSet(lbl_1_bss_2C[i], HU3D_ATTR_DISPOFF);
    }
    memset(effect, 0, sizeof(M621EFFECT) * 21);
    fn_1_140(obj, fn_1_5168);
}

void fn_1_5168(OMOBJ *obj)
{
    M621EFFECT *work = obj->data;
    int i;
    M621EFFECT *effect;

    for (i = 0; i < 21; i++) {
        effect = &work[i];
        if (effect->state == 1 && Hu3DMotionEndCheck(effect->mdlId)) {
            Hu3DModelKill(effect->mdlId);
            effect->state = 0;
        }
    }
}

void fn_1_51F4(s16 type, HuVecF *pos)
{
    int i;
    M621EFFECT *effect;

    for (i = 0; i < 21; i++) {
        effect = &((M621EFFECT *)lbl_1_bss_20)[i];
        if (effect->state == 0) {
            effect->mdlId = Hu3DModelLink(lbl_1_bss_2C[type]);
            Hu3DMotionSet(effect->mdlId, lbl_1_bss_24[type]);
            Hu3DModelAttrReset(effect->mdlId, HU3D_ATTR_DISPOFF);
            Hu3DModelPosSetV(effect->mdlId, pos);
            Hu3DModelLayerSet(effect->mdlId, 2);
            effect->state = 1;
            break;
        }
    }
}
void fn_1_52D4(s32 playerNo, s16 mgCoin)
{
    if (_CheckFlag(FLAG_MG_PRACTICE)) {
        return;
    }
    GwPlayer[playerNo].mgCoinBonus = mgCoin;
}

void fn_1_5328(s32 playerNo, u32 mgScore)
{
    GwPlayer[playerNo].mgScore = mgScore;
}

s16 fn_1_5340(void)
{
    return GwMgTime;
}

float fn_1_5350(float x)
{
    return fn_1_537C(x);
}

double fn_1_537C(double x)
{
    return __fabs(x);
}

/* @@END@@ */
