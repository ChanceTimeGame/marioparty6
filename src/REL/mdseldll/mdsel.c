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
/* Part 2 needs from HEAD: MDSEL_ACTOR, MDSEL_SE, lbl_1_bss_14/18/30/3A/3C/1D4/24C/8AC/9BC/19EC,
   lbl_1_data_15C, rand8 and frandmod and HuAudFXPanning/HuAudFXVolSet unprototyped. */
void MdSelGuideIdleWait(OMOBJ *obj);
void MdSelGuide2IdleWait(OMOBJ *obj);
void MdSelSeMdlPlay(HU3D_MODELID mdlId, s32 seId, s16 range, s16 vol);
void MdSelSeStop(s16 slot);
s16 MdSelSePlay(HU3D_MODELID mdlId, int seId);

void MdSelGuide2IdleSet(void)
{
    OMOBJ *obj = lbl_1_bss_30;
    MdSelSeMdlPlay(lbl_1_bss_18->mdlId[0], 0x3AE, 16, -1);
    Hu3DMotionShiftSet(lbl_1_bss_18->mdlId[0], lbl_1_bss_18->mtnId[4], 0, 15, HU3D_MOTATTR_LOOP);
    obj->work[0] = 0;
    obj->objFunc = MdSelGuide2IdleWait;
}

void MdSelFlockMove(OMOBJ *obj)
{
    s16 i;
    MDSEL_ACTOR *actor;
    HuVecF pos;
    for (i = 0; i < 30; i++) {
        actor = &lbl_1_bss_9BC[i];
        if (actor->active == 0) {
            continue;
        }
        actor->wait -= 1.0f;
        if (0.0f == actor->wait) {
            lbl_1_bss_19EC[i] = MdSelSePlay(obj->mdlId[i], 0x3C3);
        }
        actor->pos.x += actor->end.x;
        pos.x = actor->pos.x;
        pos.y = actor->pos.y + MdSelSinLerp360(0.0f, actor->end.y, actor->time, actor->maxTime);
        pos.z = actor->pos.z;
        Hu3DModelPosSetV(obj->mdlId[i], &pos);
        if ((actor->time += 1.0f) > actor->maxTime) {
            actor->time = 0.0f;
        }
        if (pos.x > 2000.0f) {
            MdSelSeStop(lbl_1_bss_19EC[i]);
            actor->active = 0;
            Hu3DModelAttrSet(obj->mdlId[i], HU3D_ATTR_DISPOFF);
        }
        pos.y = MdSelSinLerp360(0.0f, actor->end.z, actor->rotTime, actor->rotMaxTime);
        Hu3DModelRotSet(obj->mdlId[i], 0.0f, 90.0f + pos.y, 0.0f);
        if ((actor->rotTime += 1.0f) > actor->rotMaxTime) {
            actor->end.z = frandmod(60);
            actor->rotTime = 0.0f;
            actor->rotMaxTime = frandmod(120) + 60;
        }
    }
    for (i = 0; i < 30; i++) {
        actor = &lbl_1_bss_9BC[i];
        if (actor->active != 0) {
            break;
        }
    }
    if (i == 30) {
        obj->objFunc = NULL;
    }
}

void MdSelFlockStart(void)
{
    OMOBJ *obj = lbl_1_bss_30;
    s16 i;
    MDSEL_ACTOR *actor;
    for (i = 0; i < 30; i++) {
        actor = &lbl_1_bss_9BC[i];
        actor->active = (rand8() % 2) + 1;
        actor->wait = rand8() % 10;
        actor->pos.x = -2000.0f - frandmod(500);
        actor->pos.y = 700.0f + frandmod(200);
        actor->pos.z = frandmod(3000) - 500;
        actor->time = frandmod(120);
        actor->maxTime = frandmod(120) + 120;
        actor->end.x = 5.0f + (i % 10);
        actor->end.y = 100.0f + frandmod(200);
        actor->end.z = frandmod(60);
        actor->rotTime = 0.0f;
        actor->rotMaxTime = frandmod(120) + 60;
        Hu3DMotionShiftSet(obj->mdlId[i], obj->mtnId[0], 0.0f, 0.0f, HU3D_MOTATTR_LOOP);
        Hu3DModelAttrReset(obj->mdlId[i], HU3D_ATTR_DISPOFF);
        Hu3DModelPosSetV(obj->mdlId[i], &actor->pos);
        Hu3DModelRotSet(obj->mdlId[i], 0.0f, 90.0f, 0.0f);
    }
    obj->objFunc = MdSelFlockMove;
}

void MdSelWalkerUpdate(void)
{
    OMOBJ *obj = lbl_1_bss_30;
    s16 i;
    MDSEL_ACTOR *actor;
    HuVecF rot2;
    for (i = 0; i < 2; i++) {
        actor = &lbl_1_bss_8AC[i];
        actor->unk44++;
        if (actor->unk44 > 180) {
            actor->unk44 = 0;
            actor->unk46--;
            if (actor->unk46 < 0) {
                actor->unk46 = 0;
            }
        }
        if (actor->moveMaxTime <= 100.0f && actor->mode == 100 && rand8() % 6 == 0) {
            MdSelSeMdlPlay(obj->mdlId[i + 30], 0x3D4, 16, 100);
        }
        switch (actor->mode) {
            case 0:
                actor->mode = 100;
                actor->moveTime = 0.0f;
                if (actor->unk42 == 0) {
                    actor->moveMaxTime = (rand8() % 100) + 120;
                }
                else {
                    actor->moveMaxTime = (rand8() % 30) + 60;
                }
                if (actor->unk42 == 2) {
                    if (rand8() % 2 == 0) {
                        actor->unk42 = 0;
                    }
                    else {
                        actor->unk42 = 1;
                    }
                }
                Hu3DModelPosGet(obj->mdlId[i + 30], &actor->moveStart);
                Hu3DModelPosGet(obj->mdlId[i + 30], &actor->moveEnd);
                if (actor->moveStart.x > 0.0f) {
                    if (i == 0) {
                        actor->moveEnd.x = (frandmod(200) + 400) * -1;
                    }
                    else {
                        actor->moveEnd.x = (frandmod(100) + 400) * -1;
                    }
                }
                else {
                    if (i == 0) {
                        actor->moveEnd.x = frandmod(200) + 400;
                    }
                    else {
                        actor->moveEnd.x = frandmod(100) + 400;
                    }
                }
                Hu3DMotionShiftSet(obj->mdlId[i + 30], obj->mtnId[31], 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
                break;

            case 100:
                MdSelModelMoveLook(obj->mdlId[i + 30], &actor->moveStart, &actor->moveEnd, actor->moveTime, actor->moveMaxTime);
                if ((actor->moveTime += 1.0f) > actor->moveMaxTime) {
                    actor->mode = 200;
                    actor->moveTime = 0.0f;
                    actor->rotTarget = frandmod(360) - 180;
                    if (actor->unk42 == 1) {
                        actor->mode = 0;
                        actor->unk42 = 2;
                    }
                }
                break;

            case 200:
                Hu3DModelRotGet(obj->mdlId[i + 30], &rot2);
                rot2.y = MdSelSmoothNoCheck(rot2.y, actor->rotTarget, 30.0f);
                Hu3DModelRotSetV(obj->mdlId[i + 30], &rot2);
                actor->unk42 = 0;
                if ((actor->moveTime += 1.0f) > 30.0f) {
                    Hu3DMotionShiftSet(obj->mdlId[i + 30], obj->mtnId[30], 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
                    actor->mode = 300;
                    actor->moveTime = 0.0f;
                    actor->moveMaxTime = rand8() + 120;
                }
                break;

            case 300:
                if ((actor->moveTime += 1.0f) > actor->moveMaxTime) {
                    if (rand8() % 2 == 0) {
                        Hu3DMotionShiftSet(obj->mdlId[i + 30], obj->mtnId[31], 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
                        actor->mode = 200;
                        actor->moveTime = 0.0f;
                        actor->rotTarget = frandmod(360) - 180;
                    }
                    else {
                        actor->mode = 0;
                    }
                }
                break;

            case 400:
                if ((actor->moveTime += 1.0f) > actor->moveMaxTime) {
                    actor->mode = 0;
                    actor->unk42 = 1;
                }
                break;
        }
    }
}

void MdSelWalkerScare(void)
{
    OMOBJ *obj = lbl_1_bss_30;
    s16 i;
    MDSEL_ACTOR *actor;
    for (i = 0; i < 2; i++) {
        actor = &lbl_1_bss_8AC[i];
        actor->mode = 400;
        actor->moveTime = 0.0f;
        actor->moveMaxTime = 20.0f;
        actor->unk46++;
        if (actor->unk46 > 30) {
            actor->unk46 = 30;
        }
        if (actor->unk46 > 10) {
            MdSelSeMdlPlay(obj->mdlId[i + 30], 0x3D7, 16, 100);
        }
        else if (actor->unk46 > 5) {
            MdSelSeMdlPlay(obj->mdlId[i + 30], 0x3D6, 16, 100);
        }
        else {
            MdSelSeMdlPlay(obj->mdlId[i + 30], 0x3D5, 16, 100);
        }
        Hu3DMotionShiftSet(obj->mdlId[i + 30], obj->mtnId[32], 0.0f, 10.0f, HU3D_MOTATTR_LOOP);
    }
}

void MdSelJumperIdle(void)
{
    s16 no = 9;
    MDSEL_ACTOR *actor = &lbl_1_bss_24C[9];
    OMOBJ *obj = lbl_1_bss_30;
    HuVecF pos;
    Hu3DModelAttrReset(obj->mdlId[no + 35], HU3D_ATTR_DISPOFF);
    Hu3DModelPosGet(obj->mdlId[no + 35], &pos);
    if (lbl_1_bss_3C == 1) {
        pos.y = MdSelSmoothNoCheck(pos.y, -400.0f, 10.0f);
        actor->unk34 = 0.0f;
    }
    else {
        pos.y = MdSelSmoothNoCheck(pos.y, -200.0f, 10.0f);
    }
    if (1.0f == actor->unk34) {
        pos.y = MdSelSmoothNoCheck(pos.y, -400.0f, 10.0f);
    }
    Hu3DModelPosSet(obj->mdlId[no + 35], pos.x, pos.y, pos.z);
    if (lbl_1_bss_3C == 0) {
        if (lbl_1_bss_24C[0].active == 1) {
            Hu3DModelPosSet(obj->mdlId[no + 35], -800.0f, pos.y, -100.0f);
        }
        else {
            Hu3DModelPosSet(obj->mdlId[no + 35], 820.0f, pos.y, -120.0f);
        }
    }
    Hu3DModelRotGet(obj->mdlId[no + 35], &pos);
    pos.y = MdSelSmoothNoCheck(pos.y, actor->wait, 15.0f);
    Hu3DModelRotSet(obj->mdlId[no + 35], -40.0f, pos.y, 0.0f);
    Hu3DModelScaleSet(obj->mdlId[no + 35], 1.5f, 1.5f, 1.5f);
    if ((actor->time += 1.0f) > actor->maxTime) {
        actor->time = 0.0f;
        actor->maxTime = rand8() + 90;
        actor->wait = frandmod(90) - 45;
        actor->unk34 = 0.0f;
        if (rand8() % 2 == 0) {
            actor->unk34 = 1.0f;
        }
    }
}

void MdSelJumpMove(OMOBJ *obj)
{
    MDSEL_ACTOR *base = lbl_1_bss_24C;
    s16 i;
    MDSEL_ACTOR *actor;
    HuVecF pos;
    for (i = 0; i < lbl_1_bss_3A; i++) {
        actor = &lbl_1_bss_24C[i];
        if (0.0f == actor->time) {
            MdSelSeMdlPlay(obj->mdlId[i + 35], 0x4AB, 16, -1);
        }
        pos.x = MdSelLerp(actor->pos.x, actor->end.x, actor->time, actor->maxTime);
        pos.z = MdSelLerp(actor->pos.z, actor->end.z, actor->time, actor->maxTime);
        pos.y = MdSelSinLerp180(-300.0f, 1000.0f, actor->time, actor->maxTime);
        Hu3DModelPosSet(obj->mdlId[i + 35], pos.x, pos.y, pos.z);
        if (actor->active == 0) {
            pos.x = MdSelLerp(-90.0f, 90.0f, actor->time, actor->maxTime);
            Hu3DModelRotSet(obj->mdlId[i + 35], pos.x, 90.0f, 0.0f);
        }
        else {
            pos.x = MdSelLerp(-90.0f, 90.0f, actor->time, actor->maxTime);
            Hu3DModelRotSet(obj->mdlId[i + 35], pos.x, -90.0f, 0.0f);
        }
        if (actor->time == actor->maxTime - 5.0f) {
            MdSelSeMdlPlay(obj->mdlId[i + 35], 0x4AC, 16, -1);
        }
        if ((actor->time += 1.0f) > actor->maxTime) {
            Hu3DModelAttrSet(obj->mdlId[i + 35], HU3D_ATTR_DISPOFF);
        }
    }
    if (lbl_1_bss_24C[lbl_1_bss_3A - 1].time > lbl_1_bss_24C[lbl_1_bss_3A - 1].maxTime) {
        lbl_1_bss_3C = 0;
        obj->objFunc = NULL;
    }
}

void MdSelJumpStart(void)
{
    OMOBJ *obj = lbl_1_bss_30;
    MDSEL_ACTOR *base = lbl_1_bss_24C;
    s16 side;
    s16 num;
    s16 i;
    MDSEL_ACTOR *actor;
    side = lbl_1_bss_24C[0].active;
    side += 1;
    side %= 2;
    num = (rand8() % 8) + 1;
    lbl_1_bss_3A = num;
    lbl_1_bss_3C = 1;
    for (i = 0; i < num; i++) {
        actor = &lbl_1_bss_24C[i];
        actor->time = ((i * -5) - 20) - (rand8() % 3);
        actor->maxTime = 120.0f;
        actor->active = side;
        if (actor->active == 0) {
            actor->pos.x = -800.0f;
            actor->pos.y = 0.0f;
            actor->pos.z = -100.0f;
            actor->end.x = (800.0f + frandmod(100)) - 50.0f;
            actor->end.y = 0.0f;
            actor->end.z = (-150.0f + frandmod(100)) - 50.0f;
        }
        else {
            actor->end.x = -800.0f;
            actor->end.y = 0.0f;
            actor->end.z = -100.0f;
            actor->pos.x = (800.0f + frandmod(100)) - 50.0f;
            actor->pos.y = 0.0f;
            actor->pos.z = (-150.0f + frandmod(100)) - 50.0f;
        }
        Hu3DModelAttrReset(obj->mdlId[i + 35], HU3D_ATTR_DISPOFF);
        if (i > 0) {
            Hu3DModelScaleSet(obj->mdlId[i + 35], 1.0f, 1.0f, 1.0f);
        }
    }
    obj->objFunc = MdSelJumpMove;
}

void MdSelSceneryInit(OMOBJ *obj)
{
    s16 i;
    s16 base;
    omSetStatBit(obj, 0x100);
    for (i = 0; i < 30; i++) {
        if (i == 0) {
            obj->mdlId[i] = Hu3DModelCreateData(DATANUM(DATA_mdsel, 0x11));
            obj->mtnId[i] = Hu3DJointMotionData(obj->mdlId[0], DATANUM(DATA_mdsel, 0x12));
        }
        else {
            obj->mdlId[i] = Hu3DModelLink(obj->mdlId[0]);
        }
        Hu3DModelAttrSet(obj->mdlId[i], HU3D_ATTR_DISPOFF);
        Hu3DModelPosSet(obj->mdlId[i], frandmod(1000) - 500, frandmod(1000), -frandmod(1000));
        Hu3DModelLayerSet(obj->mdlId[i], 1);
        Hu3DMotionShiftSet(obj->mdlId[i], obj->mtnId[0], 0.0f, 0.0f, HU3D_MOTATTR_LOOP);
    }
    base = 30;
    for (i = 0; i < 2; i++) {
        obj->mdlId[i + base] = Hu3DModelCreateData(DATANUM(DATA_mdsel, 0x0B));
        if (i == 0) {
            obj->mtnId[30] = Hu3DJointMotionData(obj->mdlId[30], DATANUM(DATA_mdsel, 0x0C));
            obj->mtnId[31] = Hu3DJointMotionData(obj->mdlId[30], DATANUM(DATA_mdsel, 0x0D));
            obj->mtnId[32] = Hu3DJointMotionData(obj->mdlId[30], DATANUM(DATA_mdsel, 0x0E));
        }
        Hu3DModelPosSet(obj->mdlId[i + base], frandmod(1000) - 500, 0.0f, -350.0f + (300.0f * i));
        Hu3DModelScaleSet(obj->mdlId[i + base], 1.5f, 1.5f, 1.5f);
        Hu3DModelLayerSet(obj->mdlId[i + base], 1);
        Hu3DMotionShiftSet(obj->mdlId[i + base], obj->mtnId[30], 0.0f, 0.0f, HU3D_MOTATTR_LOOP);
    }
    base = 35;
    for (i = 0; i < 10; i++) {
        obj->mdlId[i + 35] = Hu3DModelCreateData(DATANUM(DATA_mdsel, 0x0F));
        obj->mtnId[i + 35] = Hu3DJointMotionData(obj->mdlId[i + 35], DATANUM(DATA_mdsel, 0x10));
        Hu3DModelPosSet(obj->mdlId[i + 35], 0.0f, 500.0f, 0.0f);
        Hu3DModelScaleSet(obj->mdlId[i + 35], 1.5f, 1.5f, 1.5f);
        Hu3DModelAttrSet(obj->mdlId[i + 35], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(obj->mdlId[i + 35], 1);
        Hu3DMotionShiftSet(obj->mdlId[i + 35], obj->mtnId[i + 35], 0.0f, 0.0f, HU3D_MOTATTR_LOOP);
    }
    lbl_1_bss_24C[0].active = 1;
    obj->objFunc = NULL;
}

void MdSelSceneryNull(void)
{
}

void MdSelSeUpdate(void)
{
    s16 pan;
    s16 range = 32;
    s16 i;
    s16 vol;
    HuVecF pos;
    HuVecF pos2D;
    for (i = 0; i < 15; i++) {
        if (lbl_1_bss_1D4[i].mdlId == -1) {
            continue;
        }
        Hu3DModelPosGet(lbl_1_bss_1D4[i].mdlId, &pos);
        Hu3D3Dto2D(&pos, 1, &pos2D);
        pos2D.x -= 288.0f;
        pos2D.x /= 288.0f;
        pan = 64.0f + (pos2D.x * range);
        if (pan < 64 - range) {
            pan = 64 - range;
        }
        if (pan > 64 + range) {
            pan = 64 + range;
        }
        Hu3DModelPosGet(lbl_1_bss_1D4[i].mdlId, &pos);
        pos.x = HuAbs(pos.x);
        if (pos.x < 1500.0f) {
            vol = 127;
        }
        else {
            vol = 127.0f - (127.0f * ((pos.x - 1500.0f) / 500.0f));
        }
        if (vol > 127) {
            vol = 127;
        }
        if (vol < 0) {
            vol = 0;
        }
        if (lbl_1_bss_1D4[i].seNo > 0) {
            HuAudFXPanning(lbl_1_bss_1D4[i].seNo, pan);
            HuAudFXVolSet(lbl_1_bss_1D4[i].seNo, vol);
            i == 0;
        }
    }
}

void MdSelSeStop(s16 slot)
{
    if (lbl_1_bss_1D4[slot].mdlId != -1) {
        if (lbl_1_bss_1D4[slot].seNo > 0) {
            HuAudFXStop(lbl_1_bss_1D4[slot].seNo);
        }
        lbl_1_bss_1D4[slot].mdlId = -1;
        lbl_1_bss_1D4[slot].seNo = -1;
    }
}

s16 MdSelSePlay(HU3D_MODELID mdlId, int seId)
{
    s16 i;
    for (i = 0; i < 15; i++) {
        if (lbl_1_bss_1D4[i].mdlId == -1) {
            break;
        }
    }
    if (i == 15) {
        return -1;
    }
    lbl_1_bss_1D4[i].mdlId = mdlId;
    lbl_1_bss_1D4[i].seNo = HuAudFXPlay(seId);
    return i;
}

void MdSelSeInit(void)
{
    s16 i;
    for (i = 0; i < 15; i++) {
        lbl_1_bss_1D4[i].mdlId = -1;
        lbl_1_bss_1D4[i].seNo = -1;
    }
}

void MdSelSceneryUpdate(OMOBJ *obj)
{
    MdSelSeUpdate();
    MdSelWalkerUpdate();
    MdSelJumperIdle();
}

void MdSelMicResult(HUMC_RESPONSE *resp)
{
    s16 i;
    if (resp->result != 0 || resp->nbrWords == 0) {
        return;
    }
    if (lbl_1_bss_30->objFunc == NULL && lbl_1_data_15C != 0) {
        switch (resp->itemP[0]) {
            case 4:
                MdSelFlockStart();
                break;
            case 2:
                MdSelWalkerScare();
                break;
            case 3:
                MdSelJumpStart();
                break;
            case 0:
                MdSelGuideIdleSet();
                break;
            case 1:
                MdSelGuide2IdleSet();
                break;
        }
        for (i = 0; i < resp->nbrWords; i++) {
            OSReport("Result %d\n", resp->itemP[i]);
        }
    }
}
/* @@PART3@@ */
/* Part 3: mode models, guide fly-in, icon and cursor, mic setup, scene reset */
/* Retail calls HuMCListenerCreate/Kill (no header declares them); rand8 is called unprototyped */
void HuMCListenerCreate(HUMC_CONTEXTID ctxId, HUMC_CONTEXT_CALLBACK cb, u8 flag);
void HuMCListenerKill(void);
/* part 5 */
void MdSelEffClose(void);
void MdSelEffSet(s16 grpNo, HuVecF *pos, s16 mode, s16 colorNo);
void MdSelSePosPlay(HuVecF *pos, s32 seId);
/* transition aliases: part 3 functions under their old fn_1_ names (parts 4 and 5 still use them) */
#define fn_1_6C04 MdSelMicInit
#define fn_1_6DFC MdSelMicKill
#define fn_1_6E54 MdSelMicCheck
#define fn_1_6F40 MdSelStageInit
#define fn_1_702C MdSelStageKill
#define fn_1_70BC MdSelModeMdlUpdate
#define fn_1_75A4 MdSelModeMdlSelect
#define fn_1_76DC MdSelModeMdlInit
#define fn_1_77D8 MdSelModeMdlKill
#define fn_1_7868 MdSelGuideFlyUpdate
#define fn_1_7EC4 MdSelGuideFlySet
#define fn_1_809C MdSelGuide2FlyUpdate
#define fn_1_86F8 MdSelGuide2FlySet
#define fn_1_88D0 MdSelGuideInit
#define fn_1_8A58 MdSelGuideKill
#define fn_1_8AE0 MdSelGuide2Init
#define fn_1_8CA4 MdSelGuide2Kill
#define fn_1_8D44 MdSelIconDispOn
#define fn_1_8E38 MdSelIconDispOff
#define fn_1_8EC8 MdSelIconUpdate
#define fn_1_90FC MdSelCursorTargetSet
#define fn_1_9160 MdSelCursorSet
#define fn_1_927C MdSelCursorDispOff
#define fn_1_92BC MdSelCursorUpdate
#define fn_1_97D4 MdSelCursorInit
#define fn_1_98B0 MdSelCursorKill
#define fn_1_9910 MdSelSceneReset

void MdSelMicInit(void)
{
    s16 unused = 0;
    s16 i;
    lbl_1_bss_38 = 0;
    if (GwCommon.mic == 1) {
        HuMCInit(0);
        if (HuMCMount(1) != 0) {
            OSReport("MIC SETTING >>> err!! <<< :: HuMCMount(B) != MIC_RESULT_READY\n");
            HuMCClose();
        } else {
            OSReport("MIC SETTING >>> check <<< :: HuMCMount(B) == MIC_RESULT_READY\n");
            if (GwCommon.mic != 1) {
                OSReport("MIC SETTING >>> err!! <<< :: GwCommon.mic != HUMC_FLAG_USE\n");
                HuMCClose();
            } else {
                OSReport("MIC SETTING >>> ok!! <<< :: GwCommon.mic == HUMC_FLAG_USE\n");
                lbl_1_bss_1A4C = HuMCContextCreate("/mic/ctx/mselect_words");
                HuMCListenerCreate(lbl_1_bss_1A4C, MdSelMicResult, 1);
                lbl_1_bss_38 = 1;
                OSReport("mic ok!!\n");
            }
        }
    }
    lbl_1_data_15C = 1;
    for (i = 0; i < 15; i++) {
        lbl_1_bss_1D4[i].mdlId = -1;
        lbl_1_bss_1D4[i].seNo = -1;
    }
    lbl_1_bss_30 = omAddObjEx(lbl_1_bss_8, 0x1000, 0x40, 0x40, -1, MdSelSceneryInit);
    lbl_1_bss_34 = omAddObjEx(lbl_1_bss_8, 0x1000, 0, 0, -1, MdSelSceneryUpdate);
}

void MdSelMicKill(void)
{
    lbl_1_data_15C = 0;
    if (lbl_1_bss_38 == 1) {
        HuMCListenerKill();
        HuMCContextKill(lbl_1_bss_1A4C);
        HuMCClose();
    }
}

s32 MdSelMicCheck(void)
{
    s32 result = 1;
    if (GwCommon.mic != 1) {
        return 0;
    }
    if (lbl_1_bss_38 == 0) {
        HuMCInit(0);
        if (HuMCMount(1) != 0) {
            OSReport("MIC SETTING >>> err!! <<< :: HuMCMount(B) != MIC_RESULT_READY\n");
            result = 0;
        } else {
            OSReport("MIC SETTING >>> ok!! <<< :: HuMCMount(B) == MIC_RESULT_READY\n");
            result = 1;
        }
        HuMCClose();
    } else {
        if (HuMCMount(1) != 0) {
            OSReport("MIC SETTING >>> err!! <<< :: HuMCMount(B) != MIC_RESULT_READY\n");
            result = 0;
        } else {
            OSReport("MIC SETTING >>> ok!! <<< :: HuMCMount(B) == MIC_RESULT_READY\n");
            result = 1;
        }
    }
    return result;
}

void MdSelStageInit(OMOBJ *obj)
{
    s16 i;
    omSetStatBit(obj, 0x100);
    for (i = 0; i < 3; i++) {
        obj->mdlId[i] = Hu3DModelCreateData(DATA_mdsel + i);
        obj->mtnId[i] = Hu3DMotionIDGet(obj->mdlId[i]);
        Hu3DMotionShiftSet(obj->mdlId[i], obj->mtnId[i], 0.0f, 0.0f, HU3D_MOTATTR_LOOP);
    }
    obj->objFunc = NULL;
}

void MdSelStageKill(OMOBJ *obj)
{
    s16 i;
    if (obj) {
        for (i = 0; i < 3; i++) {
            Hu3DMotionKill(obj->mtnId[i]);
            Hu3DModelKill(obj->mdlId[i]);
        }
        omDelObjEx(lbl_1_bss_8, obj);
    }
    obj = NULL;
}

void MdSelModeMdlUpdate(OMOBJ *obj)
{
    s16 i;
    float time;
    for (i = 0; i < 6; i++) {
        time = Hu3DMotionTimeGet(obj->mdlId[i]);
        if (time < 5.0f) {
            lbl_1_bss_1B0[i][0] = 0;
            lbl_1_bss_1B0[i][1] = 0;
            lbl_1_bss_1B0[i][2] = 0;
        }
        switch (i) {
            case 0:
                if (lbl_1_bss_1B0[0][0] == 0 && time > 80.0f) {
                    lbl_1_bss_1B0[0][0] = 1;
                    HuAudFXPlay(0x4A4);
                }
                break;
            case 1:
                if (lbl_1_bss_1B0[1][0] == 0 && time > 60.0f) {
                    lbl_1_bss_1B0[1][0] = 1;
                    MdSelSePosPlay(&lbl_1_data_28[3], 0x4A5);
                }
                break;
            case 2:
                if (lbl_1_bss_1B0[2][0] == 0 && time > 10.0f) {
                    lbl_1_bss_1B0[2][0] = 1;
                    MdSelSePosPlay(&lbl_1_data_28[5], 0x4A6);
                }
                break;
            case 3:
                if (lbl_1_bss_1B0[3][0] == 0 && time > 10.0f) {
                    lbl_1_bss_1B0[3][0] = 1;
                    MdSelSePosPlay(&lbl_1_data_28[0], 0x4A9);
                }
                if (lbl_1_bss_1B0[3][1] == 0 && time > 50.0f) {
                    lbl_1_bss_1B0[3][1] = 1;
                    MdSelSePosPlay(&lbl_1_data_28[0], 0x4A9);
                }
                if (lbl_1_bss_1B0[3][2] == 0 && time > 130.0f) {
                    lbl_1_bss_1B0[3][2] = 1;
                    MdSelSePosPlay(&lbl_1_data_28[0], 0x4A9);
                }
                break;
            case 4:
                if (lbl_1_bss_1B0[4][0] == 0 && time > 10.0f) {
                    lbl_1_bss_1B0[4][0] = 1;
                    MdSelSePosPlay(&lbl_1_data_28[2], 0x4A7);
                }
                if (lbl_1_bss_1B0[4][1] == 0 && time > 90.0f) {
                    lbl_1_bss_1B0[4][1] = 1;
                    MdSelSePosPlay(&lbl_1_data_28[2], 0x4A8);
                }
                break;
            case 5:
                if (lbl_1_bss_1B0[5][0] == 0 && time > 5.0f && time < 195.0f) {
                    lbl_1_bss_1B0[5][0] = 1;
                    lbl_1_bss_1A28 = HuAudFXPlay(0x4AA);
                }
                if (lbl_1_bss_1B0[5][0] == 1 && (time < 5.0f || time > 195.0f)) {
                    lbl_1_bss_1B0[5][0] = 0;
                    HuAudFXStop(lbl_1_bss_1A28);
                }
                break;
        }
    }
    for (i = 0; i < 6; i++) {
        if (i != lbl_1_bss_1A30[0] || lbl_1_bss_3E != 1) {
            Hu3DModelAttrReset(obj->mdlId[i], HU3D_MOTATTR_LOOP);
        }
    }
    for (i = 0; i < 6; i++) {
        if (rand8() == 0x20) {
            Hu3DModelAttrSet(obj->mdlId[i], HU3D_MOTATTR_LOOP);
            Hu3DModelAttrReset(obj->mdlId[i], HU3D_MOTATTR_PAUSE);
            break;
        }
    }
}

void MdSelModeMdlSelect(s16 sel)
{
    OMOBJ *obj = lbl_1_bss_10;
    s16 i;
    if (sel == -1) {
        obj->work[0] = 99;
        for (i = 0; i < 6; i++) {
            Hu3DModelAttrReset(obj->mdlId[i], HU3D_MOTATTR_LOOP);
        }
    } else if (obj->work[0] != lbl_1_bss_1A30[0]) {
        obj->work[0] = lbl_1_bss_1A30[0];
        for (i = 0; i < 6; i++) {
            if (i == lbl_1_bss_1A30[0]) {
                Hu3DModelAttrSet(obj->mdlId[i], HU3D_MOTATTR_LOOP);
                Hu3DModelAttrReset(obj->mdlId[i], HU3D_MOTATTR_PAUSE);
            } else {
                Hu3DModelAttrReset(obj->mdlId[i], HU3D_MOTATTR_LOOP);
            }
        }
    }
}

void MdSelModeMdlInit(OMOBJ *obj)
{
    s16 i;
    omSetStatBit(obj, 0x100);
    for (i = 0; i < 6; i++) {
        obj->mdlId[i] = Hu3DModelCreateData(DATA_mdsel + 3 + i);
        obj->mtnId[i] = Hu3DMotionIDGet(obj->mdlId[i]);
        Hu3DMotionShiftSet(obj->mdlId[i], obj->mtnId[i], 0.0f, 0.0f, HU3D_MOTATTR_PAUSE);
    }
    obj->work[0] = 99;
    obj->objFunc = MdSelModeMdlUpdate;
}

void MdSelModeMdlKill(OMOBJ *obj)
{
    s16 i;
    if (obj) {
        for (i = 0; i < 6; i++) {
            Hu3DMotionKill(obj->mtnId[i]);
            Hu3DModelKill(obj->mdlId[i]);
        }
        omDelObjEx(lbl_1_bss_8, obj);
    }
    obj = NULL;
}

void MdSelGuideFlyUpdate(OMOBJ *obj)
{
    MDSEL_PATH *path = &lbl_1_bss_128;
    HuVecF pos;
    HuVecF mdlPos;
    HuVecF rot;
    HuVecF scale;
    float scaleVal;
    Hu3DModelPosGet(obj->mdlId[0], &mdlPos);
    Hu3DModelRotGet(obj->mdlId[0], &rot);
    scaleVal = MdSelLerp(5.0f, 2.0f, path->time - path->maxTime / 2.0f, path->maxTime / 2.0f);
    scaleVal = MdSelLerp(5.0f, 0.25f, path->time, path->maxTime);
    Hu3DModelScaleSet(obj->mdlId[0], scaleVal, scaleVal, scaleVal);
    MdSelBezierV(&pos, MdSelLerp(0.0f, 1.0f, path->time, path->maxTime), &path->pos[0], &path->pos[1], &path->pos[2]);
    Hu3DModelPosSetV(obj->mdlId[0], &pos);
    mdlPos.x -= pos.x;
    mdlPos.z -= pos.z;
    mdlPos.y = -HuAtan(mdlPos.x, -mdlPos.z);
    if (rot.y - mdlPos.y > 180.0f) {
        rot.y -= 360.0f;
    } else if (rot.y - mdlPos.y < -180.0f) {
        rot.y += 360.0f;
    }
    rot.y = MdSelSmooth(rot.y, mdlPos.y, 15.0f);
    Hu3DModelRotSetV(obj->mdlId[0], &rot);
    Hu3DModelPosGet(obj->mdlId[0], &pos);
    Hu3DModelScaleGet(obj->mdlId[0], &scale);
    MdSelEffSet(4, &pos, 1, 5);
    if (45.0f == path->time) {
        HuAudFXStop(lbl_1_bss_19DC[0]);
        lbl_1_bss_19DC[2] = HuAudFXPlay(0x47D);
    }
    if ((path->time += 1.0f) > path->maxTime) {
        HuAudFXStop(lbl_1_bss_19DC[2]);
        MdSelEffSet(4, &pos, 0, 5);
        obj->objFunc = NULL;
    }
}

void MdSelGuideFlySet(void)
{
    OMOBJ *obj = lbl_1_bss_14;
    MDSEL_PATH *path = &lbl_1_bss_128;
    float posY;
    s16 idx;
    if (lbl_1_bss_1A30[1] != 2) {
        posY = 200.0f;
        if (lbl_1_bss_1A30[0] == 5) {
            posY = 400.0f;
        }
        idx = lbl_1_bss_1A30[1] + lbl_1_bss_1A30[2] * 3;
        path->pos[0].x = -1150.0f;
        path->pos[0].y = 350.0f;
        path->pos[0].z = -1000.0f;
        path->pos[1].x = -lbl_1_data_28[idx].x;
        path->pos[1].y = 2250.0f;
        path->pos[1].z = 6000.0f;
        path->pos[2].x = lbl_1_data_28[idx].x;
        path->pos[2].y = posY;
        path->pos[2].z = lbl_1_data_28[idx].z;
        path->time = 0.0f;
        path->maxTime = 90.0f;
        Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[1], 0.0f, 5.0f, 0);
        lbl_1_bss_19DC[0] = HuAudFXPlay(0x47E);
        obj->objFunc = MdSelGuideFlyUpdate;
    }
}

void MdSelGuide2FlyUpdate(OMOBJ *obj)
{
    MDSEL_PATH *path = &lbl_1_bss_A0;
    HuVecF pos;
    HuVecF mdlPos;
    HuVecF rot;
    HuVecF scale;
    float scaleVal;
    Hu3DModelPosGet(obj->mdlId[0], &mdlPos);
    Hu3DModelRotGet(obj->mdlId[0], &rot);
    scaleVal = MdSelLerp(5.0f, 2.0f, path->time - path->maxTime / 2.0f, path->maxTime / 2.0f);
    scaleVal = MdSelLerp(5.0f, 0.25f, path->time, path->maxTime);
    Hu3DModelScaleSet(obj->mdlId[0], scaleVal, scaleVal, scaleVal);
    MdSelBezierV(&pos, MdSelLerp(0.0f, 1.0f, path->time, path->maxTime), &path->pos[0], &path->pos[1], &path->pos[2]);
    Hu3DModelPosSetV(obj->mdlId[0], &pos);
    mdlPos.x -= pos.x;
    mdlPos.z -= pos.z;
    mdlPos.y = -HuAtan(mdlPos.x, -mdlPos.z);
    if (rot.y - mdlPos.y > 180.0f) {
        rot.y -= 360.0f;
    } else if (rot.y - mdlPos.y < -180.0f) {
        rot.y += 360.0f;
    }
    rot.y = MdSelSmooth(rot.y, mdlPos.y, 15.0f);
    Hu3DModelRotSetV(obj->mdlId[0], &rot);
    Hu3DModelPosGet(obj->mdlId[0], &pos);
    Hu3DModelScaleGet(obj->mdlId[0], &scale);
    MdSelEffSet(5, &pos, 1, 6);
    if (45.0f == path->time) {
        HuAudFXStop(lbl_1_bss_19DC[1]);
        lbl_1_bss_19DC[3] = HuAudFXPlay(0x47B);
    }
    if ((path->time += 1.0f) > path->maxTime) {
        HuAudFXStop(lbl_1_bss_19DC[3]);
        MdSelEffSet(5, &pos, 0, 6);
        obj->objFunc = NULL;
    }
}

void MdSelGuide2FlySet(void)
{
    OMOBJ *obj = lbl_1_bss_18;
    MDSEL_PATH *path = &lbl_1_bss_A0;
    float posY;
    s16 idx;
    if (lbl_1_bss_1A30[1] != 0) {
        posY = 100.0f;
        if (lbl_1_bss_1A30[0] == 5) {
            posY = 400.0f;
        }
        idx = lbl_1_bss_1A30[1] + lbl_1_bss_1A30[2] * 3;
        path->pos[0].x = 1150.0f;
        path->pos[0].y = 350.0f;
        path->pos[0].z = -1000.0f;
        path->pos[1].x = -lbl_1_data_28[idx].x;
        path->pos[1].y = 2250.0f;
        path->pos[1].z = 6000.0f;
        path->pos[2].x = lbl_1_data_28[idx].x;
        path->pos[2].y = posY;
        path->pos[2].z = lbl_1_data_28[idx].z;
        path->time = 0.0f;
        path->maxTime = 90.0f;
        Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[1], 0.0f, 5.0f, 0);
        lbl_1_bss_19DC[1] = HuAudFXPlay(0x47C);
        obj->objFunc = MdSelGuide2FlyUpdate;
    }
}

void MdSelGuideInit(OMOBJ *obj)
{
    s16 i;
    omSetStatBit(obj, 0x100);
    obj->mdlId[0] = Hu3DModelCreateData(DATA_mdsel + 0x13);
    for (i = 0; i < 5; i++) {
        obj->mtnId[i] = Hu3DJointMotionData(obj->mdlId[0], DATA_mdsel + 0x14 + i);
    }
    Hu3DModelLayerSet(obj->mdlId[0], 1);
    Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[0], 0.0f, 0.0f, HU3D_MOTATTR_LOOP);
    Hu3DModelPosSet(obj->mdlId[0], -1150.0f, 350.0f, -1000.0f);
    Hu3DModelRotSet(obj->mdlId[0], 0.0f, 15.0f, 0.0f);
    Hu3DModelScaleSet(obj->mdlId[0], 5.0f, 5.0f, 5.0f);
    obj->objFunc = NULL;
}

void MdSelGuideKill(OMOBJ *obj)
{
    s16 i;
    if (obj) {
        for (i = 0; i < 2; i++) {
            Hu3DMotionKill(obj->mtnId[i]);
        }
        Hu3DModelKill(obj->mdlId[0]);
        omDelObjEx(lbl_1_bss_8, obj);
    }
    obj = NULL;
}

void MdSelGuide2Init(OMOBJ *obj)
{
    s16 i;
    omSetStatBit(obj, 0x100);
    obj->mdlId[0] = Hu3DModelCreateData(DATA_mdsel + 0x19);
    obj->mdlId[1] = Hu3DModelCreateData(DATA_mdsel + 0x1A);
    for (i = 0; i < 5; i++) {
        obj->mtnId[i] = Hu3DJointMotionData(obj->mdlId[0], DATA_mdsel + 0x1B + i);
    }
    Hu3DModelHookSet(obj->mdlId[0], "gN01m1-itemhook_R", obj->mdlId[1]);
    Hu3DModelLayerSet(obj->mdlId[0], 1);
    Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[0], 0.0f, 0.0f, HU3D_MOTATTR_LOOP);
    Hu3DModelPosSet(obj->mdlId[0], 1150.0f, 350.0f, -1000.0f);
    Hu3DModelRotSet(obj->mdlId[0], 0.0f, -15.0f, 0.0f);
    Hu3DModelScaleSet(obj->mdlId[0], 5.0f, 5.0f, 5.0f);
    obj->objFunc = NULL;
}

void MdSelGuide2Kill(OMOBJ *obj)
{
    s16 i;
    if (obj) {
        Hu3DModelHookReset(obj->mdlId[0]);
        for (i = 0; i < 2; i++) {
            Hu3DMotionKill(obj->mtnId[i]);
        }
        Hu3DModelKill(obj->mdlId[0]);
        Hu3DModelKill(obj->mdlId[1]);
        omDelObjEx(lbl_1_bss_8, obj);
    }
    obj = NULL;
}

s16 lbl_1_data_2D0 = -1;

void MdSelIconDispOn(void)
{
    if (lbl_1_data_2D0 != lbl_1_bss_1A30[0]) {
        lbl_1_bss_40 = 0.0f;
        lbl_1_data_2D0 = lbl_1_bss_1A30[0];
    }
    HuSprBankSet(lbl_1_bss_1A38[0], 0, lbl_1_bss_1A30[0]);
    MdSelSprGrpAttrReset(lbl_1_bss_1A38[0], HUSPR_ATTR_DISPOFF);
}

void MdSelIconDispOff(void)
{
    lbl_1_data_2D0 = -1;
    MdSelSprGrpAttrSet(lbl_1_bss_1A38[0], HUSPR_ATTR_DISPOFF);
}

void MdSelIconUpdate(void)
{
    HuVecF pos3D;
    HuVecF pos2D;
    s16 idx;
    lbl_1_bss_40 = MdSelSmooth(lbl_1_bss_40, 1.0f, 5.0f);
    idx = lbl_1_bss_1A30[1] + lbl_1_bss_1A30[2] * 3;
    if (lbl_1_bss_1A30[2] == 1) {
        pos3D.x = lbl_1_data_28[idx].x / 1.5f;
    } else {
        pos3D.x = lbl_1_data_28[idx].x / 2.0f;
    }
    pos3D.y = lbl_1_data_28[idx].y;
    if (idx == 1) {
        pos3D.y = lbl_1_data_28[idx].y - 200.0f;
    }
    pos3D.z = lbl_1_data_28[idx].z;
    Hu3D3Dto2D(&pos3D, 1, &pos2D);
    if (lbl_1_bss_1A30[2] == 1) {
        pos2D.y -= 100.0f;
    }
    HuSprGrpPosSet(lbl_1_bss_1A38[0], pos2D.x, pos2D.y);
    HuSprGrpScaleSet(lbl_1_bss_1A38[0], lbl_1_bss_40, lbl_1_bss_40);
}

void MdSelCursorTargetSet(HuVecF *pos)
{
    lbl_1_bss_94.x = pos->x - 200.0f;
    lbl_1_bss_94.y = 500.0f + pos->y;
    lbl_1_bss_94.z = 350.0f + pos->z;
}

void MdSelCursorSet(HuVecF *pos)
{
    OMOBJ *obj = lbl_1_bss_1C;
    Hu3DModelPosSetV(obj->mdlId[0], pos);
    Hu3DModelRotSet(obj->mdlId[0], 0.0f, 0.0f, 180.0f);
    Hu3DModelScaleSet(obj->mdlId[0], 5.0f, 5.0f, 5.0f);
    MdSelCursorTargetSet(pos);
    Hu3DModelAttrReset(obj->mdlId[0], HU3D_ATTR_DISPOFF);
}

void MdSelCursorDispOff(void)
{
    OMOBJ *obj = lbl_1_bss_1C;
    Hu3DModelAttrSet(obj->mdlId[0], HU3D_ATTR_DISPOFF);
}

void MdSelCursorUpdate(OMOBJ *obj)
{
    HuVecF vec;
    Hu3DModelPosGet(obj->mdlId[0], &vec);
    MdSelSmoothV(&vec, &lbl_1_bss_94, 3.0f);
    Hu3DModelPosSetV(obj->mdlId[0], &vec);
    Hu3DModelRotGet(obj->mdlId[0], &vec);
    vec.z = MdSelSmooth(vec.z, 0.0f, 5.0f);
    Hu3DModelRotSetV(obj->mdlId[0], &vec);
    Hu3DModelScaleGet(obj->mdlId[0], &vec);
    vec.x = vec.y = vec.z = MdSelSmooth(vec.x, 1.5f, 3.0f);
    Hu3DModelScaleSetV(obj->mdlId[0], &vec);
    MdSelIconUpdate();
}

void MdSelCursorInit(OMOBJ *obj)
{
    omSetStatBit(obj, 0x100);
    obj->mdlId[0] = Hu3DModelCreateData(DATA_mdsel + 9);
    obj->mtnId[0] = Hu3DMotionIDGet(obj->mdlId[0]);
    Hu3DModelLayerSet(obj->mdlId[0], 1);
    Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[0], 0.0f, 0.0f, HU3D_MOTATTR_LOOP);
    MdSelCursorDispOff();
    obj->objFunc = MdSelCursorUpdate;
}

void MdSelCursorKill(OMOBJ *obj)
{
    if (obj) {
        Hu3DMotionKill(obj->mtnId[0]);
        Hu3DModelKill(obj->mdlId[0]);
        omDelObjEx(lbl_1_bss_8, obj);
    }
    obj = NULL;
}

void MdSelSceneReset(void)
{
    MdSelStageKill(lbl_1_bss_C);
    MdSelModeMdlKill(lbl_1_bss_10);
    MdSelGuideKill(lbl_1_bss_14);
    MdSelGuide2Kill(lbl_1_bss_18);
    MdSelCursorKill(lbl_1_bss_1C);
    MdSelEffClose();
    MdSelMicKill();
    MdSelSprInit();
    MdSelWinInit();
    MdSelLightInit();
    MdSelCameraInit();
}

/* @@PART4@@ */
/* Part 4 relies on: part 5's MdSelMain (s16) and MdSelEffInit; 1D20/23E0 take s32 mess;
   lbl_1_bss_19AC is HuVecF[4]; HuAudFXPlayPan unprototyped. */
s16 MdSelMain(void);
void MdSelEffInit(void);

void MdSelProc(void)
{
    s16 result = 0;
    result = MdSelMain();
    HuAudSStreamFadeOut(lbl_1_bss_1A2C, 1000);
    switch(result) {
        case 0:
            break;
        case -1:
            WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
            WipeWait();
            HuAudFadeOut(1000);
            fn_1_9910();
            fn_1_37C();
            omOvlReturnEx(1, 1);
            break;
        case 1:
            HuAudFXPlay(1199);
            WipeCreate(WIPE_MODE_OUT, 0x82, 60);
            WipeWait();
            HuAudFadeOut(1000);
            fn_1_9910();
            fn_1_37C();
            fn_1_1B4();
            break;
    }
    HuPrcEnd();
    while(1) {
        HuPrcVSleep();
    }
}

void MdSelInit(void)
{
    lbl_1_bss_8 = omInitObjMan(27, 0x2000);
    omGameSysInit(lbl_1_bss_8);
    fn_1_1734();
    fn_1_1988();
    fn_1_1DDC();
    fn_1_2828();
    lbl_1_bss_C = omAddObjEx(lbl_1_bss_8, 0x1000, 16, 16, -1, fn_1_6F40);
    lbl_1_bss_10 = omAddObjEx(lbl_1_bss_8, 0x1000, 16, 16, -1, fn_1_76DC);
    lbl_1_bss_14 = omAddObjEx(lbl_1_bss_8, 0x1000, 16, 16, -1, fn_1_88D0);
    lbl_1_bss_18 = omAddObjEx(lbl_1_bss_8, 0x1000, 16, 16, -1, fn_1_8AE0);
    lbl_1_bss_1C = omAddObjEx(lbl_1_bss_8, 0x1000, 16, 16, -1, fn_1_97D4);
    fn_1_6C04();
    MdSelEffInit();
    HuPrcChildCreate(MdSelProc, 0x3000, 0x3000, 0, (HUPROCESS *)lbl_1_bss_8);
}

void MdSelDataClose(void)
{
    HuDataDirCloseAll();
}

void ObjectSetup(void)
{
    OSReport("\n-----===== MARIO PARTY 6 :: MODE SELECT =====-----\n\n");
    HuDataDirCloseAll();
    MgPauseExitF = FALSE;
    fn_1_2CC();
    _SetFlag(5);
    MgInstExitF = FALSE;
    MgPauseExitF = FALSE;
    MgExitReq = FALSE;
    _ClearFlag(0x10014);
    _ClearFlag(0x30001);
    _ClearFlag(0x30002);
    _ClearFlag(0x30003);
    _ClearFlag(0x30004);
    lbl_1_bss_0 = omovlevtno;
    lbl_1_bss_4 = omovlstat;
    MdSelInit();
}

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

s32 MdSelIntroStart(void)
{
    HuPrcSleep(5);
    fn_1_FEC(-1);
    lbl_1_bss_1A2C = HuAudSStreamPlay(3);
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 60);
    WipeWait();
    return 1;
}

s32 MdSelIntroMes(void)
{
    s16 i;
    if(lbl_1_bss_0 == 0) {
        HuAudFXPlayPan(950, 48);
        HuAudFXPlayPan(942, 80);
        fn_1_23E0(1, 0xA0000, 1);
        fn_1_2288();
        fn_1_21DC();
        lbl_1_bss_1A30[0] = 0;
        lbl_1_bss_1A30[1] = 1;
        lbl_1_bss_1A30[2] = 1;
    } else {
        HuPrcSleep(10);
        lbl_1_bss_1A30[0] = lbl_1_bss_4;
        for(i=0; i<6; i++) {
            if(lbl_1_bss_4 == lbl_1_data_70[i]) {
                break;
            }
        }
        lbl_1_bss_1A30[1] = i%3;
        lbl_1_bss_1A30[2] = i/3;
    }
    return 1;
}

s16 MdSelExitAsk(void)
{
    HuAudFXPlayPan(952, 48);
    HuAudFXPlayPan(944, 80);
    fn_1_23E0(1, 0xA0001, 1);
    return fn_1_22E8(2);
}

s16 MdSelModeSelect(void)
{
    s16 no;
    s16 delay;
    s16 result;
    s16 prevCol;
    delay = 10;
    result = 0;
    prevCol = 1;
    while(1) {
        HuPrcVSleep();
        no = lbl_1_bss_1A30[1]+(lbl_1_bss_1A30[2]*3);
        lbl_1_bss_1A30[0] = lbl_1_data_70[no];
        fn_1_FEC(no);
        fn_1_9160(&lbl_1_data_28[no]);
        fn_1_25F8(0x10002);
        fn_1_23E0(1, lbl_1_bss_1A30[0]+0xA0002, 0);
        fn_1_8D44();
        lbl_1_bss_3E = 1;
        while(1) {
            HuPrcVSleep();
            fn_1_75A4(0);
            no = lbl_1_bss_1A30[1]+(lbl_1_bss_1A30[2]*3);
            lbl_1_bss_1A30[0] = lbl_1_data_70[no];
            fn_1_FEC(no);
            fn_1_90FC(&lbl_1_data_28[no]);
            fn_1_8D44();
            if(lbl_1_bss_1A30[1] == 0) {
                if(prevCol != lbl_1_bss_1A30[1]) {
                    prevCol = lbl_1_bss_1A30[1];
                    Hu3DMotionShiftSet(lbl_1_bss_14->mdlId[0], lbl_1_bss_14->mtnId[3], 0, 5, HU3D_MOTATTR_LOOP);
                    Hu3DMotionShiftSet(lbl_1_bss_18->mdlId[0], lbl_1_bss_18->mtnId[2], 0, 15, HU3D_MOTATTR_LOOP);
                    Hu3DMotionShiftStartEndSet(lbl_1_bss_18->mdlId[0], 20, 100);
                }
                fn_1_23E0(1, lbl_1_bss_1A30[0]+0xA0002, 0);
            } else if(lbl_1_bss_1A30[1] == 2) {
                if(prevCol != lbl_1_bss_1A30[1]) {
                    prevCol = lbl_1_bss_1A30[1];
                    Hu3DMotionShiftSet(lbl_1_bss_14->mdlId[0], lbl_1_bss_14->mtnId[2], 0, 15, HU3D_MOTATTR_LOOP);
                    Hu3DMotionShiftSet(lbl_1_bss_18->mdlId[0], lbl_1_bss_18->mtnId[3], 0, 5, HU3D_MOTATTR_LOOP);
                    Hu3DMotionShiftStartEndSet(lbl_1_bss_14->mdlId[0], 20, 100);
                }
                fn_1_23E0(1, lbl_1_bss_1A30[0]+0xA0002, 0);
            } else {
                if(prevCol != lbl_1_bss_1A30[1]) {
                    prevCol = lbl_1_bss_1A30[1];
                    Hu3DMotionShiftSet(lbl_1_bss_14->mdlId[0], lbl_1_bss_14->mtnId[0], 0, 15, HU3D_MOTATTR_LOOP);
                    Hu3DMotionShiftSet(lbl_1_bss_18->mdlId[0], lbl_1_bss_18->mtnId[0], 0, 15, HU3D_MOTATTR_LOOP);
                }
                fn_1_23E0(1, lbl_1_bss_1A30[0]+0xA0002, 0);
            }
            delay++;
            if(delay < 5) {
                continue;
            }
            delay = 11;
            if(HuPadDStkRep[0] & PAD_BUTTON_LEFT) {
                if(lbl_1_bss_1A30[1] > 0) {
                    HuAudFXPlay(0);
                    lbl_1_bss_1A30[1]--;
                    delay = 0;
                } else if(lbl_1_bss_1A30[2] == 0) {
                    HuAudFXPlay(0);
                    lbl_1_bss_1A30[2] = 1;
                    delay = 0;
                }
            } else if(HuPadDStkRep[0] & PAD_BUTTON_RIGHT) {
                if(lbl_1_bss_1A30[1] < 2) {
                    HuAudFXPlay(0);
                    lbl_1_bss_1A30[1]++;
                    delay = 0;
                } else if(lbl_1_bss_1A30[2] == 0) {
                    HuAudFXPlay(0);
                    lbl_1_bss_1A30[2] = 1;
                    delay = 0;
                }
            } else if(HuPadDStkRep[0] & PAD_BUTTON_DOWN) {
                if(lbl_1_bss_1A30[2] < 1) {
                    HuAudFXPlay(0);
                    lbl_1_bss_1A30[2]++;
                    delay = 0;
                }
            } else if(HuPadDStkRep[0] & PAD_BUTTON_UP) {
                if(lbl_1_bss_1A30[2] > 0) {
                    HuAudFXPlay(0);
                    lbl_1_bss_1A30[2]--;
                    delay = 0;
                }
            } else if(HuPadBtnDown[0] & PAD_BUTTON_A) {
                if(lbl_1_bss_1A30[0] == 0 || lbl_1_bss_1A30[0] == 1) {
                    if(GwCommon.mic == 1) {
                        if(!fn_1_6E54()) {
                            OSReport("MIC CHECK!! >> nouse!! << :: device err!!\n");
                            HuMCMicSet(0);
                        }
                    }
                    HuAudFXPlay(2);
                    if(lbl_1_bss_1A30[0] == 0 || lbl_1_bss_1A30[0] == 5) {
                        HuAudFXPlayPan(950, 48);
                        HuAudFXPlayPan(942, 80);
                    } else if(lbl_1_bss_1A30[0] == 1 || lbl_1_bss_1A30[0] == 3) {
                        HuAudFXPlay(950);
                    } else {
                        HuAudFXPlay(942);
                    }
                    result = 1;
                    break;
                } else if(lbl_1_bss_1A30[0] == 3) {
                    if(GwCommon.mic == 2) {
                        HuAudFXPlay(2);
                        HuAudFXPlay(950);
                        result = 1;
                        break;
                    } else if(GwCommon.mic == 0) {
                        HuAudFXPlay(4);
                        fn_1_23E0(1, 0xA0009, 1);
                        fn_1_2288();
                        fn_1_21DC();
                    } else if(!fn_1_6E54()) {
                        OSReport("MIC CHECK!! >> use!! << :: device error\n");
                        HuAudFXPlay(4);
                        fn_1_23E0(1, 0xA0008, 1);
                        fn_1_2288();
                        fn_1_21DC();
                    } else {
                        HuAudFXPlay(2);
                        HuAudFXPlay(950);
                        result = 1;
                        break;
                    }
                } else {
                    HuAudFXPlay(2);
                    if(lbl_1_bss_1A30[0] == 0 || lbl_1_bss_1A30[0] == 5) {
                        HuAudFXPlayPan(950, 48);
                        HuAudFXPlayPan(942, 80);
                    } else if(lbl_1_bss_1A30[0] == 1 || lbl_1_bss_1A30[0] == 3) {
                        HuAudFXPlay(950);
                    } else {
                        HuAudFXPlay(942);
                    }
                    result = 1;
                    break;
                }
            } else if(HuPadBtnDown[0] & PAD_BUTTON_B) {
                HuAudFXPlay(3);
                result = -1;
                break;
            }
            no = lbl_1_bss_1A30[1]+(lbl_1_bss_1A30[2]*3);
            lbl_1_bss_1A30[0] = lbl_1_data_70[no];
            fn_1_8D44();
        }
        lbl_1_bss_3E = 0;
        if(result == -1) {
            prevCol = -1;
            Hu3DMotionShiftSet(lbl_1_bss_14->mdlId[0], lbl_1_bss_14->mtnId[0], 0, 30, HU3D_MOTATTR_LOOP);
            Hu3DMotionShiftSet(lbl_1_bss_18->mdlId[0], lbl_1_bss_18->mtnId[0], 0, 30, HU3D_MOTATTR_LOOP);
            fn_1_75A4(-1);
        }
        fn_1_FEC(-1);
        fn_1_927C();
        fn_1_8E38();
        fn_1_277C();
        if(result != -1) {
            break;
        }
        if(MdSelExitAsk()) {
            continue;
        }
        result = -1;
        break;
    }
    return result;
}

/* @@PART5@@ */
/* Retail calls frandmod and rand8 unprototyped (signed conversions of their results) */

void MdSelExitAnim(void)
{
    lbl_1_data_15C = 0;
    HuPrcVSleep(60);
    MdSelMainWinClose();
    fn_1_7EC4();
    HuPrcSleep(10);
    fn_1_86F8();
    HuPrcSleep(80);
    MdSelCameraZoomSet();
    HuPrcSleep(50);
}

s16 MdSelMain(void)
{
    s16 result = 0;
    fn_1_B304();
    fn_1_B414();
    result = fn_1_BAB4();
    if(result != -1) {
        MdSelExitAnim();
    }
    return result;
}

void MdSelEffDispSet(s16 grpNo, s16 dispF)
{
    s16 i;
    for(i=0; i<5; i++) {
        if(dispF) {
            Hu3DModelAttrReset(lbl_1_bss_44[grpNo].unk00[i], HU3D_ATTR_DISPOFF);
        } else {
            Hu3DModelAttrSet(lbl_1_bss_44[grpNo].unk00[i], HU3D_ATTR_DISPOFF);
        }
    }
}

void MdSelEffOn(s16 grpNo, HuVecF *pos, GXColor *color)
{
    HU3D_PARTICLE *particle;
    HU3D_MODEL *modelP;
    s16 i;
    for(i=0; i<5; i++) {
        modelP = &Hu3DData[lbl_1_bss_44[grpNo].unk00[i]];
        particle = modelP->hookData;
        particle->dataCnt = 1;
        if(color != NULL) {
            particle->pos.x = color->r;
            particle->pos.y = color->g;
            particle->pos.z = color->b;
        }
        if(pos != NULL) {
            particle->unk_10.x = pos->x;
            particle->unk_10.y = pos->y;
            particle->unk_10.z = pos->z;
        }
        Hu3DModelAttrReset(lbl_1_bss_44[grpNo].unk00[i], HU3D_ATTR_DISPOFF);
    }
}

void MdSelEffOff(s16 grpNo)
{
    HU3D_MODEL *modelP;
    HU3D_PARTICLE *particle;
    s16 i;
    for(i=0; i<5; i++) {
        modelP = &Hu3DData[lbl_1_bss_44[grpNo].unk00[i]];
        particle = modelP->hookData;
        particle->dataCnt = 0;
    }
}

void MdSelEffHook(HU3D_MODEL *modelP, HU3D_PARTICLE *particle, Mtx mtx)
{
    HU3D_PARTICLE_DATA *data;
    s16 i;
    s16 num;
    float ofs;
    float color;
    num = 0;
    if(particle->count == 0) {
        for(i=0, data=particle->data; i<particle->maxCnt; i++, data++) {
            data->time = 0;
        }
        particle->dataCnt = 1;
        particle->pos.x = 255;
        particle->pos.y = 255;
        particle->pos.z = 255;
    }
    for(i=0, data=particle->data; i<particle->maxCnt; i++, data++) {
        if(data->time == 0 && particle->dataCnt == 1 && num < 1) {
            num++;
            data->time = 1;
            data->vel.x = 0;
            data->vel.y = frandmod(30) + 30;
            data->accel.x = frandmod(100) - 50;
            data->accel.y = -frandmod(100) - 50;
            data->accel.z = frandmod(100) - 50;
            PSVECNormalize(&data->accel, &data->accel);
            data->accel.x *= 2;
            data->accel.z *= 2;
            data->colorIdx = frandmod(10) + 5;
            data->pos.x = particle->unk_10.x + ((frandmod(100) - 50) * 3);
            data->pos.y = particle->unk_10.y;
            data->pos.z = particle->unk_10.z + ((frandmod(100) - 50) * 3);
            ofs = frandmod(32);
            color = particle->pos.x + ofs;
            if(color > 255) {
                color = 255;
            }
            data->color.r = color;
            color = particle->pos.y + ofs;
            if(color > 255) {
                color = 255;
            }
            data->color.g = color;
            color = particle->pos.z + ofs;
            if(color > 255) {
                color = 255;
            }
            data->color.b = color;
            data->color.a = 0;
        } else if(data->time == 1) {
            data->pos.y += data->colorIdx;
            data->colorIdx += data->accel.y;
            if(rand8() % 5 == 0) {
                data->zRot = 0.017453292f * frandmod(360);
                data->color.a = frandmod(127) + 128;
            }
            ofs = MdSelLerp(1, 0, data->vel.x, data->vel.y);
            data->scale = 100 * ofs;
            if((data->vel.x += 1) > data->vel.y) {
                data->time = 0;
                data->scale = 0;
                data->color.a = 0;
            }
        }
    }
    DCFlushRangeNoSync(particle->data, particle->maxCnt * sizeof(HU3D_PARTICLE_DATA));
}

void MdSelEffCreate(void)
{
    s16 maxCnt[5] = { 10, 10, 10, 10, 256 };
    s16 i;
    s16 j;
    for(i=0; i<6; i++) {
        for(j=0; j<5; j++) {
            lbl_1_bss_44[i].unk00[j] = Hu3DParticleCreate(lbl_1_bss_80[j], maxCnt[j]);
            Hu3DModelPosSet(lbl_1_bss_44[i].unk00[j], 0, 0, 0);
            Hu3DModelScaleSet(lbl_1_bss_44[i].unk00[j], 1, 1, 1);
            Hu3DModelAttrSet(lbl_1_bss_44[i].unk00[j], HU3D_ATTR_DISPOFF);
            Hu3DModelLayerSet(lbl_1_bss_44[i].unk00[j], 2);
            Hu3DParticleHookSet(lbl_1_bss_44[i].unk00[j], MdSelEffHook);
            Hu3DParticleBlendModeSet(lbl_1_bss_44[i].unk00[j], HU3D_PARTICLE_BLEND_ADDCOL);
        }
    }
}

void MdSelEffKill(void)
{
    s16 i;
    s16 j;
    for(i=0; i<6; i++) {
        for(j=0; j<5; j++) {
            Hu3DModelKill(lbl_1_bss_44[i].unk00[j]);
        }
    }
}

s32 lbl_1_data_35C[5] = {
    DATANUM(DATA_mdsel, 37),
    DATANUM(DATA_mdsel, 37),
    DATANUM(DATA_mdsel, 37),
    DATANUM(DATA_mdsel, 37),
    DATANUM(DATA_mdsel, 37)
};

void MdSelEffInit(void)
{
    s16 i;
    for(i=0; i<5; i++) {
        lbl_1_bss_80[i] = HuSprAnimDataRead(lbl_1_data_35C[i]);
    }
    MdSelEffCreate();
}

void MdSelEffClose(void)
{
    s16 j;
    s16 i;
    for(i=0; i<6; i++) {
        for(j=0; j<5; j++) {
            Hu3DModelKill(lbl_1_bss_44[i].unk00[j]);
        }
    }
}

void MdSelEffSet(s16 grpNo, HuVecF *pos, s16 mode, s16 colorNo)
{
    GXColor colorTbl[7] = {
        { 254, 77, 75, 0 },
        { 50, 127, 200, 0 },
        { 199, 175, 0, 0 },
        { 52, 192, 63, 0 },
        { 159, 93, 200, 0 },
        { 255, 114, 46, 0 },
        { 109, 207, 246, 0 }
    };
    if(mode == 1) {
        MdSelEffOn(grpNo, pos, &colorTbl[colorNo]);
    } else if(mode == 0) {
        MdSelEffOff(grpNo);
    } else if(mode == 2) {
        MdSelEffDispSet(grpNo, 0);
    }
}

void MdSelSePosPlay(HuVecF *pos, s32 seId)
{
    HuVecF pos2D;
    s16 pan;
    Hu3D3Dto2D(pos, 1, &pos2D);
    pos2D.x -= 288;
    pos2D.x /= 288;
    pan = 64 + 16 * pos2D.x;
    if(pan < 48) {
        pan = 48;
    }
    if(pan > 72) {
        pan = 72;
    }
    HuAudFXPlayPan(seId, pan);
}

void MdSelSeMdlPlay(HU3D_MODELID mdlId, s32 seId, s16 range, s16 vol)
{
    HuVecF pos;
    HuVecF pos2D;
    s16 pan;
    if(mdlId != -1) {
        Hu3DModelPosGet(mdlId, &pos);
        Hu3D3Dto2D(&pos, 1, &pos2D);
        pos2D.x -= 288;
        pos2D.x /= 288;
        pan = 64 + pos2D.x * range;
        if(pan < 64 - range) {
            pan = 64 - range;
        }
        if(pan > 64 + range) {
            pan = 64 + range;
        }
        if(vol == -1) {
            HuAudFXPlayPan(seId, pan);
        } else {
            HuAudFXPlayVolPan(seId, vol, pan);
        }
    } else {
        HuAudFXPlay(seId);
    }
}
/* @@END@@ */
