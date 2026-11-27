/* @@HEAD@@ */
#include "REL/w01Dll.h"
#include "game/audio.h"
#include "game/charman.h"
#include "game/frand.h"
#include "game/gamemes.h"
#include "game/memory.h"
#include "game/sprite.h"
#include "game/board/capevent.h"
#include "game/board/coin.h"
#include "game/board/status.h"
#include "game/data.h"
#include "game/gamework.h"
#include "game/pad.h"
#include "game/wipe.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/capsule.h"
#include "game/board/exit.h"
#include "game/board/guide.h"
#include "game/board/math.h"
#include "game/board/opening.h"
#include "game/board/scroll.h"
#include "game/board/shopevent.h"
#include "game/board/star.h"
#include "game/board/telop.h"
#include "game/board/window.h"
#include "game/board/wipe.h"
#include "string.h"
#include "math.h"

/* sprite.h leaves it out (WipeImage breaks with it) */
void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* .bss is laid out in reverse definition order */
float *lbl_1_bss_1620;
OSTick lbl_1_bss_161C;
W01_STARWORK *lbl_1_bss_1618;
s16 lbl_1_bss_1616;
W01_MAPOBJ lbl_1_bss_15F4;
W01_BSS_15E8 lbl_1_bss_15E8;
W01_BRIDGE lbl_1_bss_155C;
W01_CHAIN lbl_1_bss_1494;
s32 lbl_1_bss_1490;
W01_PLATFORM *lbl_1_bss_148C;
s16 lbl_1_bss_1488;
W01_SWING lbl_1_bss_1438[1];
s16 lbl_1_bss_1436;
s16 lbl_1_bss_1434;
HuVecF lbl_1_bss_1428;
HuVecF lbl_1_bss_141C;
HuVecF lbl_1_bss_1410;
W01_COINWORK lbl_1_bss_13D8;
ANIMDATA *lbl_1_bss_13D0[2];
W01_LIFT lbl_1_bss_137C[3];
ANIMDATA *lbl_1_bss_1378;
W01_FLOWER lbl_1_bss_1338[2];
ANIMDATA *lbl_1_bss_1334;
static u32 lbl_1_bss_1330; /* 4 bytes above lbl_1_bss_1240[3] (its symbol is 0xF4) */
W01_BRANCH lbl_1_bss_1240[3];
MBOBJID lbl_1_bss_1238[4];
s16 lbl_1_bss_1230[4];
HuVecF lbl_1_bss_1224;
HU3D_MODELID lbl_1_bss_1220;
ANIMDATA *lbl_1_bss_121C;
void *lbl_1_bss_1214[2];
s32 lbl_1_bss_120C[2];
float lbl_1_bss_1208;
float lbl_1_bss_1188[32];
W01BAT lbl_1_bss_EFC;
s32 lbl_1_bss_EF8;
MBOBJID lbl_1_bss_EE8[8];
ANIMDATA *lbl_1_bss_EE4;
W01_TERESA lbl_1_bss_EB4;
BOOL lbl_1_bss_EB0;
W01_LEAF lbl_1_bss_4B0[64];
ANIMDATA *lbl_1_bss_4A4[3];
s16 lbl_1_bss_4A0;
void *lbl_1_bss_49C;
u32 lbl_1_bss_498;
HuVecF lbl_1_bss_48C;
HuVecF lbl_1_bss_480;
W01_SPARK lbl_1_bss_0[32];

HuVecF lbl_1_data_0[12] = {
    { 404.027f, 1906.982f, 1602.27f },
    { 396.405f, 2867.034f, 1602.27f },
    { 18.919f, 3527.538f, 1154.26f },
    { -754.168f, 3896.764f, 773.22f },
    { -1705.322f, 3967.575f, 808.46f },
    { -2371.506f, 3583.616f, 1288.2f },
    { -2375.788f, 3020.537f, 2043.46f },
    { -1626.787f, 2637.14f, 2411.21f },
    { -698.376f, 2401.676f, 2472.98f },
    { 53.351f, 1964.242f, 2809.18f },
    { 181.735f, 1186.632f, 3321.39f },
    { -174.189f, 353.94f, 3609.05f }
};
HuVecF lbl_1_data_90[12] = {
    { 404.802f, 1890.115f, 1602.86f },
    { 439.242f, 2983.518f, 1617.86f },
    { 927.996f, 3885.564f, 1689.91f },
    { 1776.691f, 3710.542f, 2248.3f },
    { 1953.882f, 3086.915f, 3105.08f },
    { 1433.679f, 2476.203f, 3817.67f },
    { 473.901f, 2028.14f, 3948.49f },
    { -401.079f, 2214.438f, 3363.46f },
    { -1225.101f, 2654.676f, 2796.34f },
    { -2118.948f, 3099.055f, 2348.9f },
    { -3120.922f, 3248.053f, 1954.5f },
    { -4150.389f, 3105.958f, 1612.22f }
};
HuVecF lbl_1_data_120[12] = {
    { 408.268f, 1911.207f, 1602.91f },
    { 433.165f, 2978.895f, 1595.73f },
    { 1236.239f, 3596.299f, 1349.63f },
    { 1914.063f, 4131.424f, 785.95f },
    { 1573.443f, 4661.845f, -1.51f },
    { 544.371f, 4590.068f, -155.27f },
    { -261.341f, 4232.139f, 424.76f },
    { -1040.278f, 3881.412f, 1059.76f },
    { -2057.965f, 3734.857f, 1021.5f },
    { -2606.166f, 3961.725f, 195.23f },
    { -2294.736f, 4305.593f, -733.59f },
    { -1306.535f, 4351.29f, -1093.0f }
};
HuVecF lbl_1_data_1B0[12] = {
    { 403.171f, 1884.617f, 1602.97f },
    { 382.664f, 2773.3f, 1602.96f },
    { -54.871f, 3486.786f, 1583.27f },
    { -745.963f, 3927.353f, 1297.28f },
    { -834.243f, 4531.977f, 683.26f },
    { -268.617f, 4994.301f, 244.29f },
    { 581.714f, 4841.613f, 301.37f },
    { 1314.879f, 4417.083f, 566.93f },
    { 2000.25f, 3940.82f, 873.31f },
    { 2682.966f, 3462.441f, 1182.36f },
    { 3422.876f, 3048.471f, 1444.17f },
    { 4277.739f, 2853.796f, 1547.19f }
};
HuVecF lbl_1_data_240[33] = {
    { 2920.08f, 1.71e+03f, 2709.85f },
    { 3089.852f, 1706.496f, 2852.23f },
    { 3166.71f, 1706.472f, 2911.94f },
    { 3250.049f, 1706.695f, 2961.8f },
    { 3341.962f, 1707.155f, 2992.53f },
    { 3438.212f, 1703.546f, 3002.6f },
    { 3533.875f, 1697.705f, 2991.16f },
    { 3622.981f, 1684.937f, 2956.05f },
    { 3702.106f, 1666.285f, 2902.58f },
    { 3770.439f, 1646.999f, 2836.01f },
    { 3822.205f, 1628.028f, 2756.01f },
    { 3849.386f, 1610.384f, 2664.44f },
    { 3855.093f, 1594.141f, 2568.66f },
    { 3844.302f, 1577.956f, 2473.3f },
    { 3813.252f, 1559.652f, 2383.1f },
    { 3757.564f, 1534.157f, 2307.85f },
    { 3687.271f, 1497.8f, 2251.36f },
    { 3612.487f, 1450.343f, 2211.08f },
    { 3538.582f, 1392.559f, 2185.26f },
    { 3467.922f, 1326.412f, 2175.3f },
    { 3401.724f, 1255.214f, 2180.31f },
    { 3339.533f, 1181.964f, 2196.31f },
    { 3279.571f, 1108.567f, 2218.94f },
    { 3220.484f, 1036.039f, 2246.23f },
    { 3160.87f, 965.55f, 2277.41f },
    { 3098.671f, 899.881f, 2313.58f },
    { 3032.371f, 842.26f, 2355.64f },
    { 2962.411f, 793.103f, 2402.31f },
    { 2890.046f, 750.548f, 2451.78f },
    { 2816.504f, 711.819f, 2502.65f },
    { 2742.336f, 675.37f, 2554.3f },
    { 2667.836f, 640.229f, 2606.38f },
    { 2593.185f, 605.703f, 2658.66f }
};
u32 lbl_1_data_3CC[] = { DATANUM(DATA_w01, 6), DATANUM(DATA_w01, 7), DATANUM(DATA_w01, 18), DATANUM(DATA_w01, 19), DATANUM(DATA_w01, 20), 0 };
u32 lbl_1_data_3E4[] = { DATANUM(DATA_w01, 6), DATANUM(DATA_w01, 7), DATANUM(DATA_w01, 21), 0 };
u32 *lbl_1_data_3F4[] = { lbl_1_data_3CC, lbl_1_data_3E4 };
HuVecF lbl_1_data_3FC = { 0.0f, 0.0f, 1200.0f };
HuVecF lbl_1_data_408 = { -28.0f, 0.0f, 0.0f };
HuVecF lbl_1_data_414 = { 0.0f, 0.0f, 0.0f };

/* Part 2 (lift, swing and coin functions) needs these in HEAD */
static inline void W01Free(void *ptr)
{
    HuMemDirectFree(ptr);
}

/* MSL's sqrtf with plain constants (0.5 and 3.0 sit in this unit's .rodata pool) */
static inline float W01Sqrtf(float x)
{
    const double _half = .5;
    const double _three = 3.0;
    volatile float y;
    if(x > 0.0f) {
        double guess = __frsqrte((double)x);
        guess = _half*guess*(_three-guess*guess*x);
        guess = _half*guess*(_three-guess*guess*x);
        guess = _half*guess*(_three-guess*guess*x);
        y = (float)(x*guess);
        return y;
    }
    return x;
}

/* Static helpers: CodeWarrior inlines them into later functions, compiles them here (their
   literals open .rodata) and the linker strips the unreferenced bodies. fn_1_14108..fn_1_147DC,
   fn_1_14CAC, fn_1_14D34 and fn_1_161AC are their emitted twins at the end of .text. */
static float W01PathLenGet2(void *func, HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float end)
{
    int i;
    int div;
    float start;
    float t;
    float delta;
    float sum;

    div = 10;
    start = 0;
    delta = (end - start) / div;
    t = start;
    sum = 0;
    for (i = 0; i < div - 1; i++) {
        t += delta;
        sum += ((float (*)())func)(a, b, c, d, t);
    }
    sum = 0.5 * delta * (2.0 * sum + (((float (*)())func)(a, b, c, d, start) + ((float (*)())func)(a, b, c, d, end)));
    return sum;
}

static float W01PathLenGet(void *func, HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float end)
{
    int div;
    int j;
    int i;
    float start;
    float delta;
    float dLen;
    float dAccel;
    float len;

    div = 10;
    start = 0;
    len = 0;
    delta = end - start;
    dLen = 0.5f * (delta * (((float (*)())func)(a, b, c, d, start) + ((float (*)())func)(a, b, c, d, end)));
    for (i = 1; i <= div; i *= 2) {
        for (dAccel = 0, j = 1; j <= i; j++) {
            dAccel += ((float (*)())func)(a, b, c, d, start + (delta * (j - 0.5f)));
        }
        dAccel *= delta;
        len = 0.33333334f * (dLen + (2.0f * dAccel));
        delta *= 0.5f;
        dLen = 0.5f * (dLen + dAccel);
    }
    return len;
}

static float W01PathTimeGet(void *func, HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float t, float len)
{
    int step;
    float pathLen;
    float speed;
    float oldT;
    float minLen;

    minLen = 0.1f;
    step = 0;
    do {
        pathLen = W01PathLenGet2(func, a, b, c, d, t) - len;
        if (fabs(speed = ((float (*)())func)(a, b, c, d, t)) < minLen) {
            speed = 1.0f;
        }
        oldT = t;
        t -= pathLen / speed;
        step++;
    } while (t != oldT && step < 10);
    return t;
}

static float W01Hermite(float a, float b, float c, float d, float t)
{
    float invTime = 1.0f - t;
    return (d * (t * (t * t))) + ((c * (invTime * ((3.0f * t) * t))) + ((a * (invTime * (invTime * invTime))) + (b * (invTime * ((3.0f * t) * invTime)))));
}

static float W01HermiteSlope(float a, float b, float c, float d, float t)
{
    float tt = t * t;
    return ((3.0f * tt) * d) + ((c * ((-9.0f * tt) + (6.0f * t))) + ((a * (((-3.0f * tt) - (6.0f * t)) - 3.0f)) + (b * (3.0f + ((9.0f * tt) - (12.0f * t))))));
}

/* static twin of fn_1_161AC (part 7), for the jumps parts inline before it */
static void W01PlayerJumpTo(int playerNo, HuVecF *target, float rotY, float height, int time)
{
    int i;
    s16 left;
    float t;
    float rate;
    float rotStart;
    HuVecF pos;
    HuVecF newPos;
    HuVecF diff;

    mbPlayerMotionShiftSet(playerNo, 4, 0.0f, 8.0f, 0);
    mbPlayerPosGet(playerNo, &pos);
    rotStart = mbPlayerRotYGet(playerNo);
    PSVECSubtract(target, &pos, &diff);
    for (i = 0; i <= time; i++) {
        t = (float)i / time;
        if (i == time - 6U) {
            mbPlayerMotionShiftSet(playerNo, 5, 0.0f, 2.0f, 0);
        }
        newPos.x = pos.x + (t * diff.x);
        newPos.y = pos.y + (t * diff.y) + (height * HuSin(180.0f * t));
        newPos.z = pos.z + (t * diff.z);
        mbPlayerPosSetV(playerNo, &newPos);
        rate = i / 6.0f;
        if (rate > 1.0f) {
            rate = 1.0f;
        }
        mbPlayerRotYSet(playerNo, mbAngleLerp(rotStart, rotY, rate));
        left = time - i;
        mbPlayerWorkGet(playerNo)->masuNext = left;
        HuPrcVSleep();
    }
    mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 2.0f, 0x40000001);
}

/* Later .data (string literals, local initialisers, tables) is defined with the function that
   uses it first: CodeWarrior emits it in function order. */

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
    GwSystem.partyF = TRUE;
    mbObjectSetup(0, fn_1_F4, fn_1_60C);
}

void fn_1_F4(void)
{
    W01_MAPOBJ *mapObj = &lbl_1_bss_15F4;
    int boardNo = mbBoardNoGet();
    int tblNo;
    int i;

    HuAudSndGrpSetSet(22);
    lbl_1_bss_1618 = (W01_STARWORK *)GwSystem.boardWork;
    mbObjDirSet(DATA_w01, DATA_w01n);
    mbMasuInit(W01_DIR);
    mapObj->mapId = mbObjCreate(W01_DIR | 4, NULL, FALSE);
    mbObjAttrSet(mapObj->mapId, 0x40000001);
    tblNo = mbTimeDayCheck() ? 0 : 1;
    for(i = 0; lbl_1_data_3F4[tblNo][i] != 0; i++) {
        mapObj->objId[i] = mbObjCreate(W01_DIR | FILENUM(lbl_1_data_3F4[tblNo][i]), NULL, FALSE);
        mbObjAttrSet(mapObj->objId[i], 0x40000001);
    }
    mapObj->objId[i] = mbObjCreate(W01_DIR | 5, NULL, FALSE);
    mbObjLayerSet(mapObj->objId[i], 1);
    i++;
    mbScrollInit(W01_DIR | 2);
    mbCapThrowColCreate(W01_DIR | 3);
    mbLightFuncSet(fn_1_970, fn_1_9F0);
    if(mbSaveNewF) {
        memset(lbl_1_bss_1618, 0, 16);
        fn_1_139F8();
    }
    mbPlayerTurnInitHookSet(fn_1_914);
    mbPlayerTurnCloseHookSet(fn_1_944);
    fn_1_13B38();
    mbOpeningInstHookSet(fn_1_644);
    mbOpeningStarInstHookSet(NULL);
    mbOpeningViewSet(&lbl_1_data_408, &lbl_1_data_414, 10000.0f);
    mbev_ShopExInit(W01_DIR | 22, fn_1_9F4);
    mbev_ShopBackCreate(W01_DIR | 26, -1, -1, 1);
    mbev_MasuMoveEndSet(fn_1_760);
    mbev_MasuMoveStartSet(fn_1_734);
    mbev_MasuHatenaSet(fn_1_860);
    mbStarMoveHookSet(fn_1_13BC4);
    mbMapCameraSet(NULL, &lbl_1_data_3FC, 18500.0f);
    mbMapHookSet(fn_1_AC4);
    mbev_NextTimeSet(fn_1_B68);
    omAddObjEx(mbObjMan, 0x200C, 0, 0, -1, fn_1_6D4);
    fn_1_10A0();
    fn_1_2A1C();
    fn_1_45A0();
    fn_1_4D88();
    fn_1_5C7C();
    fn_1_7944();
    fn_1_8474();
    fn_1_9574();
    fn_1_ED00();
    fn_1_DEA4();
    fn_1_124C8();
    fn_1_10AA8();
    fn_1_12A4C();
    fn_1_13D04();
    HuDataDirClose(W01_DIR);
}

void fn_1_60C(void)
{
    fn_1_4750();
    fn_1_7B20();
    fn_1_8798();
    fn_1_99E4();
    fn_1_10C68();
    fn_1_12CF0();
    fn_1_5EF0();
}

void fn_1_644(void)
{
    s16 winId;

    winId = mbWinCreate(2, 0x2F0016, mbGuideSpeakerNoGet());
    mbWinWait(winId);
    winId = mbWinCreate(2, 0x2F0017, mbGuideSpeakerNoGet());
    mbWinWait(winId);
    winId = mbWinCreate(2, 0x2F0018, mbGuideSpeakerNoGet());
    mbWinWait(winId);
}

void fn_1_6D4(OMOBJ *obj)
{
    if(mbExitCheck()) {
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    fn_1_13CC();
    fn_1_4004();
    fn_1_47B0();
    fn_1_5560(0);
    fn_1_7C14();
    fn_1_F0D0();
    fn_1_1330C();
    fn_1_13F50();
}

int fn_1_734(int playerNo, s16 masuId)
{
    fn_1_4D78(masuId);
    return 0;
}

int fn_1_760(int playerNo, s16 masuId)
{
    u32 attr = mbMasuMAttrGet(masuId);

    if(attr & 0x30000) {
        fn_1_7B70(mbev_MasuBitGet(attr, 0x30000) - 1);
    }
    if(attr & 0x100) {
        mbPlayerMoveHookSet(playerNo, fn_1_2D08);
    }
    if(attr & 0x4000) {
        mbPlayerMoveHookSet(playerNo, fn_1_4F94);
    }
    if((attr & 0x400000) || (attr & 0x100000)) {
        mbPlayerMoveHookSet(playerNo, fn_1_8860);
    }
    if(attr & 0x40000) {
        mbPlayerMoveHookSet(playerNo, fn_1_E234);
    }
    if(attr & 0x1000000) {
        fn_1_126EC(playerNo, masuId);
    }
    return 0;
}

int fn_1_860(int playerNo, s16 masuId)
{
    u32 attr = mbMasuMAttrGet(masuId);

    if(attr & 0x8) {
        fn_1_5FB4(playerNo, masuId);
        return 0;
    }
    if(attr & 0x20000000) {
        fn_1_9AEC(playerNo, masuId);
    }
    if(attr & 0x40) {
        fn_1_F1F4(playerNo, masuId);
    }
    if(attr & 0x8000) {
        fn_1_10CB8(playerNo, masuId);
    }
    return 1;
}

void fn_1_914(int playerNo)
{
    fn_1_87E8();
    fn_1_12454();
    fn_1_4C6C(playerNo);
}

void fn_1_944(int playerNo)
{
    fn_1_2A08();
    fn_1_4CD0();
    fn_1_5C18();
    fn_1_44C0();
}

void fn_1_970(void)
{
    W01_MAPOBJ *mapObj = &lbl_1_bss_15F4;

    Hu3DModelLightInfoSet(mbObjModelIDGet(mapObj->mapId), 1);
    if(GwSystem.curTime == 0) {
        Hu3DFogSet(6000.0f, 70000.0f, 200, 200, 200);
    }
}

void fn_1_9F0(void)
{
}

void fn_1_9F4(int objId)
{
    MBOBJID hookId = mbObjCreate(W01_DIR | 23, NULL, TRUE);

    mbObjAttrSet(hookId, 0x40000001);
    mbObjHookSet(objId, "b01_m080", hookId);
    mbObjCullRadiusSet(objId, 400.0f);
    mbObjCullRadiusSet(hookId, 400.0f);
}

void fn_1_AC4(BOOL startF)
{
    if(startF) {
        fn_1_139E8(0);
        if(GwSystem.curTime == 0) {
            Hu3DFogClear();
        }
    } else {
        fn_1_139E8(1);
        if(GwSystem.curTime == 0) {
            Hu3DFogSet(6000.0f, 70000.0f, 200, 200, 200);
        }
    }
}

void fn_1_B68(void)
{
    W01_BSS_15E8 *work = &lbl_1_bss_15E8;
    int i;
    HUPROCESS *proc;
    s16 winId;

    for(i = 0; i < 4; i++) {
        mbPlayerDispSet(i, FALSE);
    }
    memset(work, 0, sizeof(W01_BSS_15E8));
    if(GwSystem.curTime == 0) {
        work->mdlId = mbObjCreate(DATANUM(DATA_w01, 37), NULL, FALSE);
    } else {
        work->mdlId = mbObjCreate(DATANUM(DATA_w01n, 40), NULL, FALSE);
    }
    mbObjMotionSpeedSet(work->mdlId, 0.0f);
    mbObjMotionShapeSpeedSet(work->mdlId, 0.0f);
    fn_1_950C(0);
    proc = HuPrcChildCreate(fn_1_DD0, 0x2007, 0x8000, 0, mbMainProc);
    winId = -1;
    do {
        if(work->endF) {
            if(winId < 0) {
                winId = mbWinCreateHelp(0x26000C);
                mbWinPosSet(winId, 228, 408);
            }
            for(i = 0; i < 4; i++) {
                if(!GwPlayer[i].comF && (HuPadBtnDown[GwPlayer[i].padNo] & PAD_BUTTON_START)) {
                    HuPrcKill(proc);
                    work->skipF = TRUE;
                    break;
                }
            }
        }
        HuPrcVSleep();
    } while(!work->skipF);
    mbWipeWait();
    if(!WipeCheckIn()) {
        mbWipeFadeOut();
    }
    mbWinKill(winId);
    mbObjKill(work->mdlId);
    fn_1_950C(1);
    for(i = 0; i < 4; i++) {
        mbPlayerDispSet(i, TRUE);
    }
    HuDataDirClose(W01_DIR);
}

void fn_1_DD0(void)
{
    static HuVecF lbl_1_data_42C[2][2] = {
        { { -50.0f, 15.0f, 0.0f }, { -20.0f, 0.0f, 0.0f } },
        { { -50.0f, -15.0f, 0.0f }, { -20.0f, 0.0f, 0.0f } },
    };
    static HuVecF lbl_1_data_45C[2][2] = {
        { { 0.0f, 0.0f, 0.0f }, { -20.0f, 0.0f, 0.0f } },
        { { 0.0f, 0.0f, 0.0f }, { -20.0f, 0.0f, 0.0f } },
    };
    W01_BSS_15E8 *work = &lbl_1_bss_15E8;
    HuVecF pos;
    s16 masuId;
    s16 no;

    pos.x = pos.y = pos.z = 0.0f;
    mbCameraMovePos(&pos, NULL, NULL, 10000.0f, -1.0f, 1);
    mbCameraMoveWait();
    mbWipeFadeIn();
    mbTelopTimeChangeCreate();
    while(mbTelopTimeChangeCheck()) {
        HuPrcVSleep();
    }
    mbWipeFadeOut();
    work->endF = TRUE;
    if(GwSystem.curTime == 0) {
        masuId = mbMasuFind_MAttrIdGet(-1, 0x400000);
        no = 0;
    } else {
        masuId = mbMasuFind_MAttrIdGet(-1, 0x100000);
        no = 1;
    }
    mbMasuPosGet(masuId, &pos);
    pos.y += 1000.0f;
    mbCameraMovePos(&pos, &lbl_1_data_42C[no][0], NULL, 5000.0f, -1.0f, 1);
    HuPrcVSleep();
    mbCameraMoveMasu(masuId, &lbl_1_data_42C[no][1], NULL, 2000.0f, -1.0f, 240);
    mbCameraCurveTypeSet(1);
    mbWipeFadeIn();
    HuPrcSleep(60);
    mbObjMotionSpeedSet(work->mdlId, 1.0f);
    mbObjMotionShapeSpeedSet(work->mdlId, 1.0f);
    HuPrcSleep(120);
    mbWipeFadeOut();
    masuId = mbMasuFind_MAttrIdGet(-1, 0x8);
    mbMasuPosGet(masuId, &pos);
    pos.y -= 500.0f;
    mbCameraMovePos(&pos, &lbl_1_data_45C[no][0], NULL, 3000.0f, -1.0f, 1);
    HuPrcVSleep();
    pos.y += 1000.0f;
    mbCameraMovePos(&pos, &lbl_1_data_45C[no][1], NULL, 1000.0f, -1.0f, 240);
    mbCameraCurveTypeSet(1);
    mbWipeFadeIn();
    HuPrcSleep(180);
    mbWipeFadeOut();
    work->skipF = TRUE;
    HuPrcEnd();
}

void fn_1_10A0(void)
{
    W01_BRIDGE *work = &lbl_1_bss_155C;
    W01_BRIDGEPLANK *plank = work->plank;
    int i;
    u32 attr;

    work->posA.x = -1123.03f;
    work->posA.y = 3430.0f;
    work->posA.z = -152.14f;
    work->masuA = mbMasuFind_MAttrIdGet(-1, 0x10);
    work->posB.x = 1090.968f;
    work->posB.y = 3430.0f;
    work->posB.z = 68.8458f;
    work->masuB = mbMasuFind_MAttrIdGet(-1, 0x20);
    VECSubtract(&work->posB, &work->posA, &work->dir);
    work->len = VECMag(&work->dir);
    work->rotY = HuAtan(-work->dir.x, -work->dir.z);
    work->curLen = 0.5f * work->len;
    for(i = 0; i < 6; i++, plank++) {
        plank->mdlId = mbObjCreate(W01_DIR | 8, NULL, TRUE);
        mbObjRotYSet(plank->mdlId, work->rotY);
        attr = mbev_MasuAttrGet(i + 1, 7);
        plank->masuId = mbMasuFind_MAttrMatchIdGet(-1, attr, 7);
    }
    work->ropeEndA.x = -1213.0f - 100.0f * (0.5f * mbSinDeg(work->rotY));
    work->ropeEndA.y = 3489.0f;
    work->ropeEndA.z = -161.0f - 100.0f * (0.5f * mbCosDeg(work->rotY));
    work->ropeEndB.x = 1169.0f + 100.0f * (0.5f * mbSinDeg(work->rotY));
    work->ropeEndB.y = 3489.0f;
    work->ropeEndB.z = 76.0f + 100.0f * (0.5f * mbCosDeg(work->rotY));
    for(i = 0; i < 7; i++) {
        work->ropeId[i] = mbObjCreate(W01_DIR | 9, NULL, TRUE);
    }
    work->resetF = TRUE;
}

void fn_1_13CC(void)
{
    Mtx mtx;
    HuVecF midPos;
    HuVecF pos;
    HuVecF ctrl;
    HuVecF prevPos;
    HuVecF dir;
    HuVecF rot;
    W01_BRIDGE *work;
    W01_BRIDGEPLANK *plank;
    float a;
    float b;
    float c;
    float t;
    s32 playerNo;
    int masuId;
    int masuCur;
    int i;
    int ropeNo;
    int prevMasu;
    float sag;
    float totalLen;
    float segLen;
    float curLen;

    work = &lbl_1_bss_155C;
    plank = work->plank;
    playerNo = GwSystem.turnPlayerNo;
    if(playerNo >= 0) {
        masuId = GwPlayer[playerNo].masuId;
        masuCur = work->masuA;
        do {
            if(masuCur == masuId) {
                break;
            }
            prevMasu = masuCur;
            masuCur = mbMasuMAttrFindLink(masuCur, 7);
        } while(masuCur > 0);
        if(masuCur == masuId) {
            mbPlayerPosGet(playerNo, &midPos);
            t = (midPos.z * work->dir.z + (midPos.y * work->dir.y + (midPos.x * work->dir.x - work->posA.x * work->dir.x) - work->posA.y * work->dir.y) - work->posA.z * work->dir.z) / VECSquareMag(&work->dir);
            if(mbPlayerWorkGet(playerNo)->unk10_3 != 1 || t < 0.0f || t >= 1.0f) {
                curLen = 0.5f * work->len;
            } else {
                curLen = t * work->len;
            }
            goto moved;
        }
    }
    curLen = 0.5f * work->len;
moved:
    if(!work->resetF) {
        work->curLen += 0.05f * (curLen - work->curLen);
        if(fabs(curLen - work->curLen) < 1.0) {
            return;
        }
    } else {
        work->curLen = curLen;
        work->resetF = FALSE;
        work->lastMasu = GwPlayer[playerNo].masuId;
    }
    if(masuCur >= 0 && work->lastMasu != masuCur) {
        mbAudFXPlay(0x594);
        work->lastMasu = masuId;
    }
    a = work->curLen;
    b = work->len - a;
    c = 1.2f * work->len;
    sag = sqrtf((a * a * a * a + c * c * c * c + b * b * b * b - 2.0f * (a * a * b * b + a * a * c * c - c * c * b * b + 2.0f * c * c * b * b)) / (4.0f * c * c));
    t = work->curLen / work->len;
    ctrl.x = work->posA.x + t * work->dir.x;
    ctrl.y = (work->posA.y + t * work->dir.y) - sag;
    ctrl.z = work->posA.z + t * work->dir.z;
    totalLen = W01PathLenGet(fn_1_14A90, &work->posA, &ctrl, &work->posB, NULL, 1);
    t = 0.0f;
    segLen = totalLen / 6.0f;
    prevPos = work->posA;
    for(i = 0; i < 6; i++, plank++) {
        if(i < 5) {
            t = W01PathTimeGet(fn_1_14A90, &work->posA, &ctrl, &work->posB, NULL, t, segLen);
            mbBezierCalcV(&work->posA, &ctrl, &work->posB, &pos, t);
        } else {
            pos = work->posB;
        }
        VECSubtract(&pos, &prevPos, &dir);
        midPos.x = prevPos.x + 0.5f * dir.x;
        midPos.y = prevPos.y + 0.5f * dir.y;
        midPos.z = prevPos.z + 0.5f * dir.z;
        mbObjPosSetV(plank->mdlId, &midPos);
        mbObjRotGet(plank->mdlId, &rot);
        rot.x = HuAtan(dir.y, sqrtf(dir.z * dir.z + dir.x * dir.x));
        mbObjRotSetV(plank->mdlId, &rot);
        mbMtxRot(mtx, rot.x, rot.y, rot.z);
        dir.x = 0.0f;
        dir.y = 50.0f;
        dir.z = 0.0f;
        MTXMultVec(mtx, &dir, &dir);
        mbMasuPosSet(plank->masuId, midPos.x + dir.x, midPos.y + dir.y, midPos.z + dir.z);
        mbMasuRotSet(plank->masuId, 0.0f, 0.0f, rot.x);
        prevPos = pos;
        segLen += totalLen / 6.0f;
    }
    ropeNo = 0;
    plank = work->plank;
    for(i = 0; i < 7; i++, plank++) {
        if(i == 0) {
            mbObjPosGet(plank->mdlId, &midPos);
            midPos.y -= 30.000002f;
            prevPos = midPos;
            VECSubtract(&midPos, &work->ropeEndA, &dir);
            VECNormalize(&dir, &dir);
            midPos.x = work->ropeEndA.x + 100.0f * (0.5f * dir.x);
            midPos.y = work->ropeEndA.y + 100.0f * (0.5f * dir.y);
            midPos.z = work->ropeEndA.z + 100.0f * (0.5f * dir.z);
            mbObjPosSetV(work->ropeId[ropeNo], &midPos);
            rot.x = HuAtan(dir.y, sqrtf(dir.z * dir.z + dir.x * dir.x));
            rot.y = work->rotY;
            rot.z = 0.0f;
            mbObjRotSetV(work->ropeId[ropeNo], &rot);
            ropeNo++;
        } else if(i == 6) {
            VECSubtract(&work->ropeEndB, &prevPos, &dir);
            VECNormalize(&dir, &dir);
            midPos.x = work->ropeEndB.x - 100.0f * (0.5f * dir.x);
            midPos.y = work->ropeEndB.y - 100.0f * (0.5f * dir.y);
            midPos.z = work->ropeEndB.z - 100.0f * (0.5f * dir.z);
            mbObjPosSetV(work->ropeId[ropeNo], &midPos);
            rot.x = HuAtan(dir.y, sqrtf(dir.z * dir.z + dir.x * dir.x));
            rot.y = work->rotY;
            rot.z = 0.0f;
            mbObjRotSetV(work->ropeId[ropeNo], &rot);
            ropeNo++;
        } else {
            mbObjPosGet(plank->mdlId, &midPos);
            midPos.y -= 30.000002f;
            VECSubtract(&midPos, &prevPos, &dir);
            prevPos = midPos;
            midPos.x -= 0.5f * dir.x;
            midPos.y -= 0.5f * dir.y;
            midPos.z -= 0.5f * dir.z;
            mbObjPosSetV(work->ropeId[ropeNo], &midPos);
            rot.x = HuAtan(dir.y, sqrtf(dir.z * dir.z + dir.x * dir.x));
            rot.y = work->rotY;
            rot.z = 0.0f;
            mbObjRotSetV(work->ropeId[ropeNo], &rot);
            ropeNo++;
        }
    }
}

void fn_1_2A08(void)
{
    lbl_1_bss_155C.resetF = TRUE;
}

/* player height above the chain by character (fn_1_2D08, fn_1_4004) */
const float lbl_1_rodata_F8[14] = { -80.0f, -90.0f, -125.0f, -80.0f, -100.0f, -125.0f, -110.0f, -80.0f, -90.0f, -80.0f, -80.0f, -80.0f, -80.0f, -80.0f };

void fn_1_2A1C(void)
{
    W01_CHAIN *work = &lbl_1_bss_1494;
    W01_CHAINLINK *link = work->link;
    int i;
    HuVecF pos;

    work->posA.x = 747.0f;
    work->posA.y = 4993.0f;
    work->posA.z = -1309.0f;
    work->masuA = mbMasuFind_MAttrIdGet(-1, 0x100);
    work->posB.x = 2708.0f;
    work->posB.y = 4037.0f;
    work->posB.z = 18.0f;
    work->masuB = mbMasuFind_MAttrIdGet(-1, 0x200);
    VECSubtract(&work->posB, &work->posA, &work->dir);
    work->len = VECMag(&work->dir);
    work->rotY = HuAtan(-work->dir.x, -work->dir.z);
    work->playerNo = -1;
    for(i = 0; i < 16; i++) {
        work->ropeId[i] = mbObjCreate(W01_DIR | 10, NULL, TRUE);
        mbObjRotYSet(work->ropeId[i], work->rotY);
    }
    work->objId = mbObjCreate(W01_DIR | 11, NULL, TRUE);
    mbObjRotYSet(work->objId, work->rotY);
    fn_1_3214(0.05f);
    mbObjPosGet(work->objId, &pos);
    for(i = 0; i < 4; i++, link++) {
        link->objId = mbObjCreate(W01_DIR | 10, NULL, TRUE);
        mbObjScaleSet(link->objId, 0.7f, 0.7f, 0.5f);
        link->pos.x = pos.x;
        link->pos.y = pos.y - 50.0f * (i + 1);
        link->pos.z = pos.z;
        link->vel.x = link->vel.y = link->vel.z = 0.0f;
    }
}

void fn_1_2D08(int playerNo)
{
    W01_CHAIN *work = &lbl_1_bss_1494;
    W01_CHAINLINK *link = work->link;
    float rotY;
    float t;
    int i;
    s16 motId;
    HuVecF movePos;
    HuVecF playerPos;
    HuVecF pos;
    HuVecF dir;

    GwPlayer[playerNo].moveF = TRUE;
    mbPlayerColSnapPlayerSet(playerNo, FALSE);
    mbPlayerWorkGet(playerNo)->masuNext = 100;
    mbObjPosGet(work->link[3].objId, &pos);
    pos.y += lbl_1_rodata_F8[GwPlayer[playerNo].charNo];
    motId = mbPlayerMotionCreate(playerNo, 0x930063);
    mbPlayerPosGet(playerNo, &playerPos);
    VECSubtract(&pos, &playerPos, &dir);
    rotY = HuAtan(work->dir.x, work->dir.z);
    mbPlayerRotYSet(playerNo, rotY);
    mbPlayerMotionShiftSet(playerNo, 4, 0.0f, 8.0f, 0);
    for(i = 0; i < 18U; i++) {
        t = i / 18.0f;
        if(i == 15U) {
            mbPlayerMotionShiftSet(playerNo, motId, 0.0f, 4.0f, 0);
        }
        movePos.x = playerPos.x + t * dir.x;
        movePos.y = playerPos.y + t * dir.y + 100.0f * mbSinDeg(180.0f * t);
        movePos.z = playerPos.z + t * dir.z;
        mbPlayerPosSetV(playerNo, &movePos);
        HuPrcVSleep();
    }
    work->playerNo = playerNo;
    mbAudFXPlay(0x594);
    for(i = 3; i < 54U; i++) {
        t = i / 59.0f;
        if(i == 12U) {
            CharFXPlay(GwPlayer[playerNo].charNo, 0x243);
        }
        fn_1_3214(t);
        if((i - 3) % 30 == 0) {
            omVibrate(playerNo, 20, 4, 4);
        }
        HuPrcVSleep();
    }
    work->playerNo = -1;
    mbPlayerPosGet(playerNo, &playerPos);
    mbMasuPosGet(work->masuB, &pos);
    VECSubtract(&pos, &playerPos, &dir);
    rotY = HuAtan(dir.x, dir.z);
    mbPlayerRotYSet(playerNo, rotY);
    mbPlayerMotionShiftSet(playerNo, 4, 0.0f, 4.0f, 0);
    for(i = 0; i <= 18U; i++) {
        t = i / 18.0f;
        if(i == 12U) {
            mbPlayerMotionShiftSet(playerNo, 5, 0.0f, 8.0f, 0);
        }
        movePos.x = playerPos.x + t * dir.x;
        movePos.y = playerPos.y + t * dir.y + 100.0f * (2.0f * mbSinDeg(180.0f * t));
        movePos.z = playerPos.z + t * dir.z;
        mbPlayerPosSetV(playerNo, &movePos);
        mbPlayerWorkGet(playerNo)->masuNext = 18 - i;
        HuPrcVSleep();
    }
    mbPlayerMotionKill(playerNo, motId);
    GwPlayer[playerNo].moveF = FALSE;
    mbPlayerColSnapPlayerSet(playerNo, TRUE);
}

void fn_1_3214(float t)
{
    HuVecF midPos;
    HuVecF ctrl;
    HuVecF prevPos;
    HuVecF pos;
    HuVecF hookPos;
    HuVecF dir;
    HuVecF rot;
    W01_CHAIN *work;
    float a;
    float b;
    float c;
    float sag;
    float time;
    float segLen;
    int i;
    float totalLen;
    float scale;
    float curLen;

    work = &lbl_1_bss_1494;
    curLen = t * work->len;
    a = curLen;
    b = work->len - a;
    c = 1.1f * work->len;
    sag = sqrtf((a * a * a * a + c * c * c * c + b * b * b * b - 2.0f * (a * a * b * b + a * a * c * c - c * c * b * b + 2.0f * c * c * b * b)) / (4.0f * c * c));
    ctrl.x = work->posA.x + t * work->dir.x;
    ctrl.y = (work->posA.y + t * work->dir.y) - sag;
    ctrl.z = work->posA.z + t * work->dir.z;
    hookPos = ctrl;
    totalLen = W01PathLenGet(fn_1_14A90, &work->posA, &ctrl, &work->posB, NULL, 1);
    time = 0.0f;
    segLen = totalLen / 16.0f;
    prevPos = work->posA;
    for(i = 0; i < 16; i++) {
        if(i < 15) {
            time = W01PathTimeGet(fn_1_14A90, &work->posA, &ctrl, &work->posB, NULL, time, segLen);
            mbBezierCalcV(&work->posA, &ctrl, &work->posB, &pos, time);
        } else {
            pos = work->posB;
        }
        VECSubtract(&pos, &prevPos, &dir);
        midPos.x = prevPos.x + 0.5f * dir.x;
        midPos.y = prevPos.y + 0.5f * dir.y;
        midPos.z = prevPos.z + 0.5f * dir.z;
        mbObjPosSetV(work->ropeId[i], &midPos);
        mbObjRotGet(work->ropeId[i], &rot);
        rot.x = HuAtan(dir.y, sqrtf(dir.z * dir.z + dir.x * dir.x));
        mbObjRotSetV(work->ropeId[i], &rot);
        scale = VECMag(&dir) / 100.0f;
        mbObjScaleSet(work->ropeId[i], 1.0f, 1.0f, scale);
        VECSubtract(&pos, &prevPos, &dir);
        sag = (hookPos.z * dir.z + (hookPos.x * dir.x - prevPos.x * dir.x) - prevPos.z * dir.z) / (dir.x * dir.x + dir.z * dir.z);
        if(sag >= 0.0f && sag < 1.0f) {
            hookPos.y = prevPos.y + sag * dir.y;
            mbObjPosSetV(work->objId, &hookPos);
            mbObjRotSetV(work->objId, &rot);
        }
        prevPos = pos;
        segLen += totalLen / 16.0f;
    }
}

/* @@PART2@@ */
/* @@PART3@@ */
/* @@PART4@@ */
/* @@PART5@@ */
/* @@PART6@@ */
/* @@PART7@@ */
/* scoring stubs for other parts (never spliced) */
void fn_1_4004(void) { }
void fn_1_44C0(void) { }
void fn_1_45A0(void) { }
void fn_1_4750(void) { }
void fn_1_47B0(void) { }
void fn_1_4C6C(int arg0) { }
void fn_1_4CD0(void) { }
void fn_1_4D78(int arg0) { }
void fn_1_4D88(void) { }
void fn_1_4F94(int playerNo) { }
void fn_1_5560(int arg0) { }
void fn_1_5888(int arg0) { }
void fn_1_5C18(void) { }
void fn_1_5C7C(void) { }
void fn_1_5EF0(void) { }
void fn_1_5FB4(int playerNo, s16 masuId) { }
int fn_1_6D98(void) { return 0; }
HU3D_MODELID fn_1_7388(HuVecF *pos) { return 0; }
void fn_1_7448(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx) { }
void fn_1_77AC(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx) { }
void fn_1_7944(void) { }
void fn_1_7B20(void) { }
void fn_1_7B70(int arg0) { }
void fn_1_7C14(void) { }
void fn_1_7D14(W01_LIFT *work, HuVecF *pos) { }
void fn_1_7D84(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx) { }
void fn_1_80B4(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx) { }
void fn_1_8474(void) { }
void fn_1_8798(void) { }
void fn_1_87E8(void) { }
void fn_1_8860(int playerNo) { }
void fn_1_9134(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx) { }
void fn_1_939C(HU3D_MODELID modelId, int no, HuVecF *pos) { }
void fn_1_950C(BOOL flag) { }
void fn_1_9574(void) { }
void fn_1_99E4(void) { }
void fn_1_9AEC(int playerNo, s16 masuId) { }
int fn_1_A82C(int playerNo) { return 0; }
void fn_1_ACDC(OMOBJ *obj) { }
void fn_1_AFD4(int no, int playerNo) { }
void fn_1_B01C(int no, int pathNo) { }
void fn_1_B3F8(int no) { }
int fn_1_B434(W01_BRANCHLEAF **leafP) { return 0; }
void fn_1_B7D8(int no) { }
void fn_1_B97C(int no) { }
void fn_1_B9A8(int no, int time) { }
void fn_1_BB44(W01_BRANCH *tree, HuVecF *pos) { }
void fn_1_BFA0(HU3D_MODEL *modelP, Mtx *mtx) { }
void fn_1_C694(const s32 *data, u32 color) { }
s32 fn_1_C7DC(void **dlP) { return 0; }
s32 fn_1_CAA4(void **dlP) { return 0; }
s32 fn_1_CCEC(HuVecF *src, s32 triNum, float radius, HuVecF *dst) { return 0; }
s32 fn_1_D080(HuVecF *dst, float radius) { return 0; }
s32 fn_1_D1D0(HuVecF *dst, float radius) { return 0; }
s32 fn_1_D320(HuVecF *src, s32 num, HuVecF *dst) { return 0; }
void fn_1_D50C(W01ZSORT *list, s32 left, s32 right) { }
void fn_1_DEA4(void) { }
void fn_1_E234(int playerNo) { }
void fn_1_ED00(void) { }
void fn_1_F06C(void) { }
void fn_1_F0D0(void) { }
void fn_1_F1F4(int playerNo, s16 masuId) { }
void fn_1_F66C(W01BATSUB *sub) { }
BOOL fn_1_1089C(void) { return 0; }
void fn_1_10900(void) { }
void fn_1_10998(int arg0) { }
void fn_1_10AA8(void) { }
void fn_1_10C68(void) { }
void fn_1_10CB8(int playerNo, s16 masuId) { }
void fn_1_11C14(HuVecF *pos) { }
void fn_1_11C84(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx) { }
void fn_1_12090(void) { }
void fn_1_12454(void) { }
void fn_1_124C8(void) { }
void fn_1_126EC(int playerNo, s16 masuId) { }
void fn_1_12A4C(void) { }
void fn_1_12CF0(void) { }
void fn_1_12D58(HU3D_MODEL *modelP, Mtx *mtx) { }
u32 fn_1_130C4(void **arg0) { return 0; }
void fn_1_1330C(void) { }
void fn_1_139E8(int arg0) { }
void fn_1_139F8(void) { }
void fn_1_13B38(void) { }
void fn_1_13BC4(void) { }
void fn_1_13CBC(int arg0) { }
void fn_1_13D04(void) { }
void fn_1_13D8C(HuVecF *pos, int num) { }
void fn_1_13F50(void) { }
void fn_1_14108(int arg0, int arg1, int arg2, int arg3, int arg4, float f0) { }
void fn_1_142B0(int arg0, int arg1, int arg2, int arg3, int arg4, float f0) { }
void fn_1_144C0(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, float f0, float f1) { }
void fn_1_147DC(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, float f0, float f1) { }
float fn_1_14A90(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float t) { return 0; }
void fn_1_14B34(int arg0, int arg1, int arg2, int arg3, float f0) { }
float fn_1_14BF0(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float t) { return 0; }
void fn_1_14CAC(float f0, float f1, float f2, float f3, float f4) { }
void fn_1_14D34(float f0, float f1, float f2, float f3, float f4) { }
void fn_1_14E0C(int arg0, int arg1, int arg2, int arg3, int arg4, float f0) { }
void fn_1_150CC(int arg0, int arg1, int arg2, int arg3, int arg4, float f0) { }
void fn_1_1547C(int arg0, int arg1, int arg2, float f0) { }
void fn_1_155EC(int arg0, int arg1, float f0) { }
void fn_1_15B60(int arg0, int arg1, int arg2, float f0) { }
void fn_1_15E1C(int arg0, int arg1, int arg2) { }
void fn_1_15FF8(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6) { }
void fn_1_16148(int arg0, int arg1, int arg2, float f0) { }
void fn_1_161AC(int arg0, int arg1, int arg2, float f0, float f1) { }
void fn_1_1644C(void) { }
int fn_1_16478(void) { return 0; }
/* @@END@@ */
