/* @@HEAD@@ */
#include "REL/mdseldll.h"
#include "game/armem.h"
/* Retail calls these unprototyped (extsh on s16 arguments; parts 2 and 5) */
#define HuAudFXPanning HuAudFXPanning_proto
#define HuAudFXVolSet HuAudFXVolSet_proto
#define HuAudFXPlayPan HuAudFXPlayPan_proto
#define HuAudFXPlayVolPan HuAudFXPlayVolPan_proto
#include "game/audio.h"
#undef HuAudFXPanning
#undef HuAudFXVolSet
#undef HuAudFXPlayPan
#undef HuAudFXPlayVolPan
void HuAudFXPanning();
s32 HuAudFXVolSet();
int HuAudFXPlayPan();
int HuAudFXPlayVolPan();
#include "game/charman.h"
#include "game/data.h"
#include "game/gamework.h"
#include "game/mgdata.h"
#include "game/memory.h"
#include "game/mic.h"
#include "game/pad.h"
#include "game/wipe.h"
#include "humath.h"
#include "string.h"
#include "stdio.h"
#include "dolphin.h"

#include "datadir_enum.h"

/* frandmod and rand8 are called unprototyped: game/frand.h stays out of this unit */

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* .bss is laid out in reverse definition order */
HUMC_CONTEXTID lbl_1_bss_1A4C;
HU3D_LIGHTID lbl_1_bss_1A48[2];
HUWINID lbl_1_bss_1A40[4];
ANIMDATA *lbl_1_bss_1A3C[1];
HUSPR_GROUPID lbl_1_bss_1A38[1];
HUSPRID lbl_1_bss_1A36[1];
s16 lbl_1_bss_1A30[3];
s32 lbl_1_bss_1A2C;
s32 lbl_1_bss_1A28;
s16 lbl_1_bss_19EC[30];
s32 lbl_1_bss_19DC[4];
MDSEL_CAMERA lbl_1_bss_19AC;
MDSEL_ACTOR lbl_1_bss_9BC[30];
MDSEL_ACTOR lbl_1_bss_8AC[2];
MDSEL_ACTOR lbl_1_bss_24C[12];
MDSEL_SE lbl_1_bss_1D4[15];
s16 lbl_1_bss_1B0[6][3];
MDSEL_PATH lbl_1_bss_128;
MDSEL_PATH lbl_1_bss_A0;
HuVecF lbl_1_bss_94;
ANIMDATA *lbl_1_bss_80[5];
MDSEL_WORKA lbl_1_bss_44[6];
float lbl_1_bss_40;
s16 lbl_1_bss_3E;
s16 lbl_1_bss_3C;
s16 lbl_1_bss_3A;
s16 lbl_1_bss_38;
OMOBJ *lbl_1_bss_34;
OMOBJ *lbl_1_bss_30;
float lbl_1_bss_2C;
void *lbl_1_bss_28;
u32 lbl_1_bss_24; /* unreferenced; retail .bss has 4 bytes here */
OMOBJ *lbl_1_bss_20;
OMOBJ *lbl_1_bss_1C;
OMOBJ *lbl_1_bss_18;
OMOBJ *lbl_1_bss_14;
OMOBJ *lbl_1_bss_10;
OMOBJ *lbl_1_bss_C;
OMOBJMAN *lbl_1_bss_8;
s32 lbl_1_bss_4;
s32 lbl_1_bss_0;

s32 lbl_1_data_0[1] = { DATANUM(DATA_mdsel, 10) };
s16 lbl_1_data_4[1] = { 1 };
MDSEL_SPRDATA lbl_1_data_8[1] = {
    { 0, 0, 0, 0, 0, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f }
};
HuVecF lbl_1_data_28[6] = {
    { -670.0f, 0.0f, -800.0f },
    { 0.0f, 200.0f, -1100.0f },
    { 650.0f, 0.0f, -780.0f },
    { -1156.0f, -200.0f, -27.0f },
    { 0.0f, 0.0f, 550.0f },
    { 1200.0f, 0.0f, 0.0f }
};
s16 lbl_1_data_70[6] = { 3, 5, 4, 1, 0, 2 };
s32 lbl_1_data_7C = -1;

/* .data placement: lbl_1_data_150/_154/_15C are defined at the end of PART1 (after the
   strings of MdSelARamDump). Part 3 defines s16 lbl_1_data_2D0 = -1 between fn_1_8AE0 and
   fn_1_8D44; part 5 defines s32 lbl_1_data_35C[5] (DATANUM(DATA_mdsel, 37) x5) before
   fn_1_F790. Their extern declarations are in REL/mdseldll.h. */

/* @@PART1@@ */
void MdSelWinCallback(HUWINID winId, u32 mess, s16 no)
{
    s32 messTbl[3] = { 0xA0000, 0xA0001, -1 };
    s32 seTbl[16] = {
        0x3B5, 0x3B6, 0x3B7, 0x3B8, 0x3B9, 0x3BA, 0x3BB, -1,
        0x3AD, 0x3AE, 0x3AF, 0x3B0, 0x3B1, 0x3B2, 0x3B3, -1
    };
    s16 i;
    no--;
    OSReport("# ========== win callback :: %d\n", no);
    if (lbl_1_data_7C != mess) {
        lbl_1_data_7C = mess;
        for (i = 0;; i++) {
            if (messTbl[i] == -1) {
                HuAudFXPlay(seTbl[no]);
                break;
            }
            if (mess == messTbl[i]) {
                if (no >= 8) {
                    HuAudFXPlayPan(seTbl[no], 80);
                } else {
                    HuAudFXPlayPan(seTbl[no], 48);
                }
                break;
            }
        }
    }
}

void MdSelOvlCall(void)
{
    OMOVLHIS *his = omOvlHisGet(0);
    omOvlHisChg(0, his->ovl, 1, lbl_1_bss_1A30[0]);
    switch (lbl_1_bss_1A30[0]) {
        case 0:
            omOvlCall(DLL_mdpartydll, 0, 0);
            break;
        case 1:
            omOvlCall(DLL_mdsingdll, 0, 0);
            break;
        case 2:
            omOvlCall(DLL_mdminidll, 0, 0);
            break;
        case 3:
            omOvlCall(DLL_mdmicdll, 0, 0);
            break;
        case 4:
            omOvlCall(DLL_optiondll, 0, 0);
            break;
        case 5:
            omOvlCall(DLL_mdbankdll, 0, 0);
            break;
    }
}

void MdSelARamFree(void)
{
    CharDataClose(-1);
    HuARDirFree(DATA_board);
    HuARDirFree(DATA_board_us);
    HuARDirFree(DATA_capsule);
    OSReport(">>>>>>>>>> mdseldll :: objsetup!! <<<<<<<<<<\n");
    OSReport("0x%x :: _effect\n", _effect);
    OSReport("0x%x :: _gamemes\n", _gamemes);
    OSReport("0x%x :: _mgconst\n", _mgconst);
    OSReport("0x%x :: _win\n", _win);
    HuAMemDump();
    OSReport("\n");
}

void MdSelARamDump(void)
{
    OSReport("\n>>>>>>>>>> mdseldll :: dllreturn or dllmove!! <<<<<<<<<<\n");
    OSReport("0x%x :: _effect\n", _effect);
    OSReport("0x%x :: _gamemes\n", _gamemes);
    OSReport("0x%x :: _mgconst\n", _mgconst);
    OSReport("0x%x :: _win\n", _win);
    HuAMemDump();
    OSReport("\n");
}

void MdSelSprGrpAttrSet(HUSPR_GROUPID grpId, u32 attr)
{
    HUSPR_GROUP *grpP = &HuSprGrpData[grpId];
    s16 i;
    for (i = 0; i < grpP->sprNum; i++) {
        HuSprAttrSet(grpId, i, (u16)attr);
    }
}

void MdSelSprGrpAttrReset(HUSPR_GROUPID grpId, u32 attr)
{
    HUSPR_GROUP *grpP = &HuSprGrpData[grpId];
    s16 i;
    for (i = 0; i < grpP->sprNum; i++) {
        HuSprAttrReset(grpId, i, (u16)attr);
    }
}

float MdSelLerp(float start, float end, float time, float maxTime)
{
    if (time <= 0.0f) {
        return start;
    }
    if (time >= maxTime) {
        return end;
    }
    return start + (time / maxTime) * (end - start);
}

float MdSelSmooth(float cur, float target, float speed)
{
    if (cur == target || speed <= 1.0f) {
        return target;
    }
    return (target + cur * (speed - 1.0f)) / speed;
}

void MdSelSmoothV(HuVecF *cur, HuVecF *target, float speed)
{
    cur->x = MdSelSmooth(cur->x, target->x, speed);
    cur->y = MdSelSmooth(cur->y, target->y, speed);
    cur->z = MdSelSmooth(cur->z, target->z, speed);
}

float MdSelSinLerp90(float start, float end, float time, float maxTime)
{
    if (time <= 0.0f) {
        return start;
    }
    if (time >= maxTime) {
        return end;
    }
    return start + (end - start) * HuSin((90.0f / maxTime) * time);
}

float MdSelSinLerp360(float start, float end, float time, float maxTime)
{
    if (time <= 0.0f) {
        return start;
    }
    if (time >= maxTime) {
        return start;
    }
    return start + (end - start) * HuSin((360.0f / maxTime) * time);
}

float MdSelSinLerp180(float start, float end, float time, float maxTime)
{
    if (time <= 0.0f) {
        return start;
    }
    if (time >= maxTime) {
        return start;
    }
    return start + (end - start) * HuSin((180.0f / maxTime) * time);
}

float MdSelBezier(float a, float b, float c, float t)
{
    float u = 1.0f - t;
    return c * (t * t) + (a * (u * u) + 2.0f * (b * (u * t)));
}

void MdSelBezierV(HuVecF *out, float t, HuVecF *a, HuVecF *b, HuVecF *c)
{
    out->x = MdSelBezier(a->x, b->x, c->x, t);
    out->y = MdSelBezier(a->y, b->y, c->y, t);
    out->z = MdSelBezier(a->z, b->z, c->z, t);
}

float MdSelSmoothNoCheck(float cur, float target, float speed)
{
    return (target + cur * (speed - 1.0f)) / speed;
}

float MdSelLerp2(float start, float end, float time, float maxTime)
{
    if (time <= 0.0f) {
        return start;
    }
    if (time >= maxTime) {
        return end;
    }
    return start + (time / maxTime) * (end - start);
}

void MdSelModelMoveLook(HU3D_MODELID mdlId, HuVecF *start, HuVecF *end, float time, float maxTime)
{
    HuVecF pos;
    HuVecF rot;
    HuVecF newPos;
    HuVecF newRot;
    Hu3DModelPosGet(mdlId, &pos);
    Hu3DModelRotGet(mdlId, &rot);
    newPos.x = MdSelLerp2(start->x, end->x, time, maxTime);
    newPos.y = MdSelLerp2(start->y, end->y, time, maxTime);
    newPos.z = MdSelLerp2(start->z, end->z, time, maxTime);
    pos.x -= newPos.x;
    pos.z -= newPos.z;
    newRot.y = -HuAtan(pos.x, -pos.z);
    if (rot.y - newRot.y > 180.0f) {
        rot.y -= 360.0f;
    } else if (rot.y - newRot.y < -180.0f) {
        rot.y += 360.0f;
    }
    newRot.x = rot.x;
    newRot.y = MdSelSmoothNoCheck(rot.y, newRot.y, 10.0f);
    newRot.z = rot.z;
    Hu3DModelPosSetV(mdlId, &newPos);
    Hu3DModelRotSetV(mdlId, &newRot);
}

void MdSelCameraTargetSet(s16 no)
{
    float div = 4.0f;
    if (no == -1) {
        lbl_1_bss_19AC.posEnd.x = 0.0f;
        lbl_1_bss_19AC.posEnd.y = 1860.0f;
        lbl_1_bss_19AC.posEnd.z = 4180.0f;
        lbl_1_bss_19AC.targetEnd.x = 0.0f;
        lbl_1_bss_19AC.targetEnd.y = 317.0f;
        lbl_1_bss_19AC.targetEnd.z = 100.0f;
    } else {
        lbl_1_bss_19AC.posEnd.x = lbl_1_data_28[no].x / div;
        lbl_1_bss_19AC.posEnd.y = 1860.0f + lbl_1_data_28[no].y / div;
        lbl_1_bss_19AC.posEnd.z = 4080.0f + lbl_1_data_28[no].z / div;
        lbl_1_bss_19AC.targetEnd.x = lbl_1_data_28[no].x / div;
        lbl_1_bss_19AC.targetEnd.y = 317.0f + lbl_1_data_28[no].y / div;
        lbl_1_bss_19AC.targetEnd.z = lbl_1_data_28[no].z / div;
    }
}

void MdSelCameraZoomSet(void)
{
    float div = 1.0f;
    float div2 = 2.0f;
    float ofsY = 250.0f;
    s16 no = lbl_1_bss_1A30[1] + lbl_1_bss_1A30[2] * 3;
    if (lbl_1_bss_1A30[0] == 5) {
        ofsY = 400.0f;
    }
    lbl_1_bss_19AC.posEnd.x = lbl_1_data_28[no].x;
    lbl_1_bss_19AC.posEnd.y = ofsY + lbl_1_data_28[no].y;
    lbl_1_bss_19AC.posEnd.z = 1000.0f + lbl_1_data_28[no].z;
    lbl_1_bss_19AC.targetEnd.x = lbl_1_data_28[no].x / div;
    lbl_1_bss_19AC.targetEnd.y = ofsY + lbl_1_data_28[no].y / div;
    lbl_1_bss_19AC.targetEnd.z = lbl_1_data_28[no].z / div;
}

void MdSelCameraUpdate(OMOBJ *obj)
{
    lbl_1_bss_19AC.pos.x = MdSelSmooth(lbl_1_bss_19AC.pos.x, lbl_1_bss_19AC.posEnd.x, 15.0f);
    lbl_1_bss_19AC.pos.y = MdSelSmooth(lbl_1_bss_19AC.pos.y, lbl_1_bss_19AC.posEnd.y, 15.0f);
    lbl_1_bss_19AC.pos.z = MdSelSmooth(lbl_1_bss_19AC.pos.z, lbl_1_bss_19AC.posEnd.z, 15.0f);
    lbl_1_bss_19AC.target.x = MdSelSmooth(lbl_1_bss_19AC.target.x, lbl_1_bss_19AC.targetEnd.x, 15.0f);
    lbl_1_bss_19AC.target.y = MdSelSmooth(lbl_1_bss_19AC.target.y, lbl_1_bss_19AC.targetEnd.y, 15.0f);
    lbl_1_bss_19AC.target.z = MdSelSmooth(lbl_1_bss_19AC.target.z, lbl_1_bss_19AC.targetEnd.z, 15.0f);
    Hu3DCameraPosSet(HU3D_CAM0, lbl_1_bss_19AC.pos.x, lbl_1_bss_19AC.pos.y, lbl_1_bss_19AC.pos.z, 0.0f, 1.0f, 0.0f, lbl_1_bss_19AC.target.x, lbl_1_bss_19AC.target.y, lbl_1_bss_19AC.target.z);
}

void MdSelCameraInit(void)
{
    Hu3DCameraCreate(HU3D_CAM0);
    Hu3DCameraPerspectiveSet(HU3D_CAM0, 30.0f, 10.0f, 10000.0f, 1.2f);
    Hu3DCameraViewportSet(HU3D_CAM0, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraPosSet(HU3D_CAM0, 0.0f, 1860.0f, 4080.0f, 0.0f, 1.0f, 0.0f, 0.0f, 317.0f, 0.0f);
    lbl_1_bss_19AC.pos.x = lbl_1_bss_19AC.posEnd.x = 0.0f;
    lbl_1_bss_19AC.pos.y = lbl_1_bss_19AC.posEnd.y = 1860.0f;
    lbl_1_bss_19AC.pos.z = lbl_1_bss_19AC.posEnd.z = 4480.0f;
    lbl_1_bss_19AC.target.x = lbl_1_bss_19AC.targetEnd.x = 0.0f;
    lbl_1_bss_19AC.target.y = lbl_1_bss_19AC.targetEnd.y = 317.0f;
    lbl_1_bss_19AC.target.z = lbl_1_bss_19AC.targetEnd.z = 0.0f;
    lbl_1_bss_20 = omAddObjEx(lbl_1_bss_8, 0x1000, 0, 0, -1, MdSelCameraUpdate);
}

void MdSelCameraKill(void)
{
    Hu3DCameraKill(HU3D_CAM0);
}

void MdSelLightInit(void)
{
    HuVecF pos[2] = { { 0.0f, 1.0f, 1.0f }, { -1.0f, 1.0f, -1.0f } };
    HuVecF dir[2] = { { 0.0f, -1.0f, -1.0f }, { 1.0f, -1.0f, -1.0f } };
    GXColor color = { 255, 255, 255, 255 };
    s16 i;
    for (i = 0; i < 2; i++) {
        lbl_1_bss_1A48[i] = Hu3DGLightCreateV(&pos[i], &dir[i], &color);
        Hu3DGLightInfinitytSet(lbl_1_bss_1A48[i]);
        Hu3DGLightStaticSet(lbl_1_bss_1A48[i], TRUE);
    }
}

void MdSelLightKill(void)
{
    s16 i;
    for (i = 0; i < 2; i++) {
        Hu3DGLightKill(lbl_1_bss_1A48[i]);
    }
}

void MdSelWinOpen(s16 winNo)
{
    if (winNo == 0) {
        HuWinDispOn(lbl_1_bss_1A40[winNo]);
    } else {
        HuWinExOpen(lbl_1_bss_1A40[winNo]);
    }
}

void MdSelWinClose(s16 winNo)
{
    if (winNo == 0) {
        HuWinDispOff(lbl_1_bss_1A40[winNo]);
    } else {
        HuWinExClose(lbl_1_bss_1A40[winNo]);
    }
}

void MdSelWinWait(s16 winNo)
{
    HuWinMesWait(lbl_1_bss_1A40[winNo]);
}

s16 MdSelWinChoiceGet(s16 winNo, s16 mode)
{
    s16 choice = 0;
    if (mode == 1) {
        HuWinAttrSet(lbl_1_bss_1A40[winNo], HUWIN_ATTR_NOCANCEL);
    } else {
        HuWinAttrReset(lbl_1_bss_1A40[winNo], HUWIN_ATTR_NOCANCEL);
    }
    choice = HuWinChoiceGet(lbl_1_bss_1A40[winNo], -1);
    if (mode == 2 && choice == -1) {
        choice = 1;
    }
    return choice;
}

void MdSelWinMesSet(s16 winNo, s32 mess, s16 speed)
{
    HuWinAttrSet(lbl_1_bss_1A40[winNo], HUWIN_ATTR_ALIGN_CENTER);
    HuWinMesSet(lbl_1_bss_1A40[winNo], mess);
    HuWinMesSpeedSet(lbl_1_bss_1A40[winNo], speed);
    if (lbl_1_data_7C != mess) {
        lbl_1_data_7C = -1;
    }
}

void MdSelWinInit(void)
{
    s16 i;
    HuWinInit(1);
    lbl_1_bss_1A40[0] = HuWinExCreateFrame(16.0f, 337.0f, 544, 42, -1, 0);
    HuWinDispOff(lbl_1_bss_1A40[0]);
    HuWinBGTPLvlSet(lbl_1_bss_1A40[0], 0.0f);
    lbl_1_bss_1A40[1] = HuWinExCreateFrame(16.0f, 372.0f, 544, 68, -1, 0);
    HuWinDispOff(lbl_1_bss_1A40[1]);
    HuWinBGTPLvlSet(lbl_1_bss_1A40[1], 0.9f);
    lbl_1_bss_1A40[2] = HuWinExCreateFrame(16.0f, 372.0f, 544, 68, -1, 3);
    HuWinDispOff(lbl_1_bss_1A40[2]);
    HuWinBGTPLvlSet(lbl_1_bss_1A40[2], 0.9f);
    lbl_1_bss_1A40[3] = HuWinExCreateFrame(16.0f, 372.0f, 544, 68, -1, 4);
    HuWinDispOff(lbl_1_bss_1A40[3]);
    HuWinBGTPLvlSet(lbl_1_bss_1A40[3], 0.9f);
    for (i = 0; i < 4; i++) {
        winData[lbl_1_bss_1A40[i]].padMask = HUWIN_PLAYER_1;
        HuWinCallbackSet(lbl_1_bss_1A40[i], (HUWIN_CALLBACK)MdSelWinCallback);
        HuWinAttrSet(lbl_1_bss_1A40[i], HUWIN_ATTR_UPAUSE);
    }
}

void MdSelWinKill(void)
{
    s16 i;
    for (i = 0; i < 4; i++) {
        HuWinExKill(lbl_1_bss_1A40[i]);
    }
    HuWinAllKill();
}

void MdSelMainWinOpen(s16 winNo)
{
    if (lbl_1_data_150[0] != -1 && lbl_1_data_150[0] != winNo) {
        MdSelWinClose(lbl_1_data_150[0]);
    }
    if (lbl_1_data_150[0] == -1 || lbl_1_data_150[0] != winNo) {
        lbl_1_data_150[0] = winNo;
        lbl_1_data_154[0] = -1;
        MdSelWinOpen(lbl_1_data_150[0]);
    }
}

void MdSelMainWinClose(void)
{
    if (lbl_1_data_150[0] != -1) {
        MdSelWinClose(lbl_1_data_150[0]);
    }
    lbl_1_data_150[0] = -1;
    lbl_1_data_154[0] = -1;
}

void MdSelMainWinWait(void)
{
    if (lbl_1_data_150[0] != -1) {
        MdSelWinWait(lbl_1_data_150[0]);
    }
}

s16 MdSelMainWinChoiceGet(s16 mode)
{
    if (lbl_1_data_150[0] != -1) {
        return MdSelWinChoiceGet(lbl_1_data_150[0], mode);
    }
    return 0;
}

void MdSelMainWinMesSet(s16 winNo, s32 mess, s16 speed)
{
    MdSelMainWinOpen(winNo);
    if (lbl_1_data_154[0] != mess) {
        lbl_1_data_154[0] = mess;
        MdSelWinMesSet(lbl_1_data_150[0], lbl_1_data_154[0], speed);
    }
}

void MdSelHelpWinMesSet(s32 mess)
{
    if (lbl_1_data_150[1] == -1) {
        lbl_1_data_150[1] = 0;
        lbl_1_data_154[1] = -1;
        MdSelWinOpen(lbl_1_data_150[1]);
    }
    if (lbl_1_data_154[1] != mess) {
        lbl_1_data_154[1] = mess;
        MdSelWinMesSet(lbl_1_data_150[1], lbl_1_data_154[1], 0);
    }
}

void MdSelHelpWinClose(void)
{
    if (lbl_1_data_150[1] != -1) {
        MdSelWinClose(lbl_1_data_150[1]);
    }
    lbl_1_data_150[1] = -1;
    lbl_1_data_154[1] = -1;
}

void MdSelSprInit(void)
{
    MDSEL_SPRDATA *sprData = lbl_1_data_8;
    s16 i;
    for (i = 0; i < 1; i++) {
        lbl_1_bss_1A3C[i] = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_data_0[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    for (i = 0; i < 1; i++) {
        lbl_1_bss_1A38[i] = HuSprGrpCreate(lbl_1_data_4[i]);
    }
    for (i = 0; i < 1; i++, sprData++) {
        lbl_1_bss_1A36[i] = HuSprCreate(lbl_1_bss_1A3C[sprData->animNo], sprData->prio, sprData->bank);
        HuSprGrpMemberSet(lbl_1_bss_1A38[sprData->grpNo], sprData->memberNo, lbl_1_bss_1A36[i]);
        HuSprPosSet(lbl_1_bss_1A38[sprData->grpNo], sprData->memberNo, sprData->posX, sprData->posY);
        HuSprScaleSet(lbl_1_bss_1A38[sprData->grpNo], sprData->memberNo, sprData->scaleX, sprData->scaleY);
        HuSprZRotSet(lbl_1_bss_1A38[sprData->grpNo], sprData->memberNo, sprData->zRot);
    }
    for (i = 0; i < 1; i++) {
        MdSelSprGrpAttrSet(lbl_1_bss_1A38[i], HUSPR_ATTR_DISPOFF);
    }
}

void MdSelSprKill(void)
{
}

void MdSelFbHook(s16 layerNo)
{
    Mtx44 proj;
    GXTexObj texObj;
    Mtx trans;
    Mtx rot;
    Mtx final;
    float alpha;
    if (lbl_1_bss_28 != NULL) {
        C_MTXOrtho(proj, 0.0f, 480.0f, 0.0f, 640.0f, 0.0f, 10.0f);
        GXSetProjection(proj, GX_ORTHOGRAPHIC);
        GXInitTexObj(&texObj, lbl_1_bss_28, 640, 480, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
        GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        MTXTrans(trans, -320.0f, -240.0f, 0.0f);
        MTXRotDeg(rot, 'Z', lbl_1_bss_2C / 10.0f);
        MTXConcat(rot, trans, final);
        mtxTransCat(final, 320.0f, 240.0f, 0.0f);
        GXLoadPosMtxImm(final, GX_PNMTX0);
        alpha = MdSelSinLerp90(0.0f, 1.0f, lbl_1_bss_2C += 1.0f, 60.0f);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-50.0f, -50.0f, 0.0f);
        GXColor4u8(128, 128, 128, 160);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(690.0f, -50.0f, 0.0f);
        GXColor4u8(128, 128, 128, 160);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(690.0f, 530.0f, 0.0f);
        GXColor4u8(128, 128, 128, 160);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-50.0f, 530.0f, 0.0f);
        GXColor4u8(128, 128, 128, 160);
        GXTexCoord2f32(0.0f, 1.0f);
        GXEnd();
        if (lbl_1_bss_28) {
            Hu3DFbCopyExec(0, 0, 640, 480, GX_TF_RGBA8, 0, lbl_1_bss_28);
        }
    }
}

void MdSelFbHookSet(void)
{
    if (lbl_1_bss_28) {
        Hu3DLayerHookSet(15, MdSelFbHook);
    }
}

void MdSelFbInit(void)
{
    lbl_1_bss_28 = HuMemDirectMallocNum(HEAP_MODEL, GXGetTexBufferSize(640, 480, GX_TF_RGBA8, GX_FALSE, 0), HU_MEMNUM_OVL);
}

void MdSelFbKill(void)
{
    Hu3DLayerHookReset(15);
    if (lbl_1_bss_28) {
        HuMemDirectFree(lbl_1_bss_28);
    }
    lbl_1_bss_28 = NULL;
}

void MdSelGuideIdleWait(OMOBJ *obj)
{
    if (++obj->work[0] > 120) {
        Hu3DMotionShiftSet(lbl_1_bss_14->mdlId[0], lbl_1_bss_14->mtnId[0], 0.0f, 15.0f, HU3D_MOTATTR_LOOP);
        obj->objFunc = NULL;
    }
}

void MdSelGuideIdleSet(void)
{
    OMOBJ *obj = lbl_1_bss_30;
    MdSelSeMdlPlay(lbl_1_bss_14->mdlId[0], 0x3B6, 16, -1);
    Hu3DMotionShiftSet(lbl_1_bss_14->mdlId[0], lbl_1_bss_14->mtnId[4], 0.0f, 15.0f, HU3D_MOTATTR_LOOP);
    obj->work[0] = 0;
    obj->objFunc = MdSelGuideIdleWait;
}

void MdSelGuide2IdleWait(OMOBJ *obj)
{
    if (++obj->work[0] > 120) {
        Hu3DMotionShiftSet(lbl_1_bss_18->mdlId[0], lbl_1_bss_18->mtnId[0], 0.0f, 15.0f, HU3D_MOTATTR_LOOP);
        obj->objFunc = NULL;
    }
}

s16 lbl_1_data_150[2] = { -1, -1 };
s32 lbl_1_data_154[2] = { -1, -1 };
s16 lbl_1_data_15C = 1;

/* @@PART2@@ */
/* @@PART3@@ */
/* @@PART4@@ */
/* @@PART5@@ */
/* @@END@@ */
