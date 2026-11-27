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
#include "game/board/comchoice.h"
#include "game/board/malloc.h"
#include "game/board/snpc.h"
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
static inline void *W01Malloc(s32 size)
{
    return HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
}

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
HuVecF lbl_1_data_48C[7] = {
    { -4414, 2900, 1070 },
    { -4244, 2900, 670 },
    { -3804, 2900, 600 },
    { 3526, 3900, 720 },
    { 3946, 3550, 560 },
    { 4346, 3200, 720 },
    { 4566, 2850, 1110 },
};
HuVecF lbl_1_data_4E0[1] = { { -1314, 3120, 1251 } };
int lbl_1_data_4EC[] = { DATANUM(DATA_w01, 33), -1 };
int lbl_1_data_4F4[] = { DATANUM(DATA_w01n, 33), -1 };
int *lbl_1_data_4FC[] = { lbl_1_data_4EC, lbl_1_data_4F4 };
void fn_1_4004(void)
{
    W01_CHAIN *chain = &lbl_1_bss_1494;
    W01_CHAINLINK *link = &chain->link[0];
    HuVecF pos;
    HuVecF prevPos;
    HuVecF dir;
    HuVecF rot;
    float mag;
    float force;
    int i;

    mbObjPosGet(chain->objId, &prevPos);
    for(i = 0; i < 4; i++, link++) {
        PSVECSubtract(&prevPos, &link->pos, &dir);
        mag = PSVECMag(&dir);
        force = 20.0f*(mag-50.0f);
        PSVECNormalize(&dir, &dir);
        link->vel.x += (1.0f/60.0f)*(dir.x*force);
        link->vel.y += -32.666668f+((1.0f/60.0f)*(dir.y*force));
        link->vel.z += (1.0f/60.0f)*(dir.z*force);
        link->pos.x += (1.0f/60.0f)*link->vel.x;
        link->pos.y += (1.0f/60.0f)*link->vel.y;
        link->pos.z += (1.0f/60.0f)*link->vel.z;
        PSVECScale(&link->vel, &link->vel, 0.98f);
        PSVECSubtract(&link->pos, &prevPos, &dir);
        PSVECNormalize(&dir, &dir);
        link->pos.x = prevPos.x+(50.0f*dir.x);
        link->pos.y = prevPos.y+(50.0f*dir.y);
        link->pos.z = prevPos.z+(50.0f*dir.z);
        PSVECSubtract(&prevPos, &link->pos, &dir);
        pos.x = link->pos.x+(0.5f*dir.x);
        pos.y = link->pos.y+(0.5f*dir.y);
        pos.z = link->pos.z+(0.5f*dir.z);
        mbObjPosSetV(link->objId, &pos);
        rot.x = HuAtan(dir.y, W01Sqrtf((dir.z*dir.z)+(dir.x*dir.x)));
        rot.y = HuAtan(-dir.x, -dir.z);
        rot.z = 0.0f;
        mbObjRotSetV(link->objId, &rot);
        prevPos = link->pos;
    }
    if(chain->playerNo >= 0) {
        mbObjPosGet(chain->link[3].objId, &pos);
        mbPlayerPosSet(chain->playerNo, pos.x, pos.y+lbl_1_rodata_F8[GwPlayer[chain->playerNo].charNo], pos.z);
    }
}

void fn_1_44C0(void)
{
    W01_CHAIN *chain = &lbl_1_bss_1494;
    W01_CHAINLINK *link = &chain->link[0];
    HuVecF pos;
    int i;

    fn_1_3214(0.05f);
    mbObjPosGet(chain->objId, &pos);
    for(i = 0; i < 4; i++, link++) {
        link->pos.x = pos.x;
        link->pos.y = pos.y-(50.0f*(i+1));
        link->pos.z = pos.z;
        link->vel.x = link->vel.y = link->vel.z = 0.0f;
    }
}

void fn_1_45A0(void)
{
    W01_PLATFORM *lift;
    int i;
    u32 attr;

    lbl_1_bss_1490 = 7;
    lift = mbMalloc(lbl_1_bss_1490*sizeof(W01_PLATFORM));
    lbl_1_bss_148C = lift;
    for(i = 0; i < lbl_1_bss_1490; i++, lift++) {
        lift->objId = mbObjCreate(W01_DATANUM(14), NULL, TRUE);
        mbObjAttrSet(lift->objId, HU3D_MOTATTR_LOOP);
        lift->pos.x = lbl_1_data_48C[i].x;
        lift->pos.y = 1000.0f+lbl_1_data_48C[i].y;
        lift->pos.z = lbl_1_data_48C[i].z;
        mbObjPosSet(lift->objId, lift->pos.x, lift->pos.y-1000.0f, lift->pos.z);
        attr = mbev_MasuAttrGet(i+1, 0x1C00);
        lift->masuId = mbMasuFind_MAttrMatchIdGet(-1, attr, 0x1C00);
        lift->maxTime = 240;
        lift->time = mbRandMod(lift->maxTime);
        lift->speed = 1.0f;
    }
    lbl_1_bss_1488 = -1;
}

void fn_1_4750(void)
{
    if(lbl_1_bss_148C != NULL) {
        W01Free(lbl_1_bss_148C);
        lbl_1_bss_148C = NULL;
    }
}

void fn_1_47B0(void)
{
    W01_PLATFORM *lift = lbl_1_bss_148C;
    int playerNo = GwSystem.turnPlayerNo;
    int i;
    HuVecF pos;
    HuVecF playerPos;
    HuVecF diff;
    float t;
    float amp;
    float accel;
    float dy;
    float speed;

    if(playerNo >= 0) {
        mbPlayerPosGet(playerNo, &playerPos);
    }
    for(i = 0; i < lbl_1_bss_1490; i++, lift++) {
        t = (float)lift->time++/lift->maxTime;
        amp = 5.0f+(5.0f*(1.0f+mbSinDeg(360.0f*t)));
        mbObjPosGet(lift->objId, &pos);
        accel = -980.0f;
        if(playerNo >= 0) {
            PSVECSubtract(&pos, &playerPos, &diff);
            if(lift->onF) {
                amp += 50.0f;
                if(lbl_1_bss_1488 != lift->masuId) {
                    lift->onF = FALSE;
                }
            } else {
                if(W01Sqrtf((diff.x*diff.x)+(diff.z*diff.z)) < 170.0f && (diff.y > 0.0f || mbPlayerWorkGet(playerNo)->unk10_3 == 1) && lift->masuId == lbl_1_bss_1488) {
                    lift->onF = TRUE;
                    accel -= 50000.0f;
                } else {
                    lift->onF = FALSE;
                }
            }
        }
        dy = lift->pos.y-pos.y;
        accel += (500.0f*(dy-1000.0f))/amp;
        lift->vel += (1.0f/60.0f)*accel;
        pos.y += (1.0f/60.0f)*lift->vel;
        lift->vel *= 0.9f;
        mbObjPosSetV(lift->objId, &pos);
        speed = fabs(accel)/1000.0;
        if(speed < 1.0f) {
            speed = 1.0f;
        } else if(speed > 3.0f) {
            speed = 3.0f;
        }
        lift->speed += 0.1f*(speed-lift->speed);
        mbObjMotionSpeedSet(lift->objId, lift->speed);
        mbMasuPosSet(lift->masuId, pos.x, pos.y, pos.z);
    }
}

void fn_1_4C6C(int playerNo)
{
    int i;

    lbl_1_bss_1488 = GwPlayer[playerNo].masuId;
    for(i = 0; i < 100; i++) {
        fn_1_47B0();
    }
}

void fn_1_4CD0(void)
{
    W01_PLATFORM *lift = lbl_1_bss_148C;
    int i;

    for(i = 0; i < lbl_1_bss_1490; i++, lift++) {
        lift->onF = FALSE;
        mbObjPosSet(lift->objId, lift->pos.x, lift->pos.y-1000.0f, lift->pos.z);
        lift->speed = 1.0f;
    }
    lbl_1_bss_1488 = -1;
}

void fn_1_4D78(s16 masuId)
{
    lbl_1_bss_1488 = masuId;
}

void fn_1_4D88(void)
{
    W01_SWING *swing = lbl_1_bss_1438;
    int i;
    int j;

    lbl_1_bss_1436 = mbMasuFind_MAttrIdGet(-1, 0x4000);
    mbMasuPosGet(lbl_1_bss_1436, &lbl_1_bss_1428);
    lbl_1_bss_1434 = mbMasuFind_MAttrIdGet(-1, 0x2000);
    mbMasuPosGet(lbl_1_bss_1434, &lbl_1_bss_141C);
    PSVECSubtract(&lbl_1_bss_141C, &lbl_1_bss_1428, &lbl_1_bss_1410);
    for(i = 0; i < 1; i++, swing++) {
        swing->len[0] = 550.0f;
        swing->len[1] = 150.0f;
        for(j = 0; j < 2; j++) {
            swing->objId[j] = mbObjCreate(W01_DATANUM(12), NULL, TRUE);
        }
        swing->baseObjId = mbObjCreate(W01_DATANUM(13), NULL, TRUE);
        swing->mass[0] = 1.0f;
        swing->mass[1] = 20.0f;
        swing->angle[0] = swing->angle[1] = 0.0f;
        swing->angVel[0] = swing->angVel[1] = 0.0f;
        swing->pos = lbl_1_data_4E0[i];
        swing->playerNo = -1;
        fn_1_5888(i);
    }
}

void fn_1_4F94(int playerNo)
{
    W01_SWING *swing = lbl_1_bss_1438;
    int i;
    int j;
    s16 motId;
    int left;
    float rotY;
    float t;
    float t2;
    float rotStart;
    float weight;
    HuVecF pos;
    HuVecF playerPos;
    HuVecF target;
    HuVecF diff;
    HuVecF diff2;
    HuVecF pos2;
    HuVecF startPos;
    char *hookName;

    GwPlayer[playerNo].moveF = TRUE;
    mbPlayerColSnapPlayerSet(playerNo, FALSE);
    mbPlayerWorkGet(playerNo)->masuNext = 100;
    GwPlayer[playerNo].masuIdNext = lbl_1_bss_1434;
    motId = mbPlayerMotionCreate(playerNo, 0x930063);
    hookName = CharModelItemHookGet(GwPlayer[playerNo].charNo, 4, 0);
    mbPlayerPosGet(playerNo, &playerPos);
    PSVECSubtract(&target, &playerPos, &diff);
    rotY = HuAtan(lbl_1_bss_1410.x, lbl_1_bss_1410.z);
    mbPlayerRotYSet(playerNo, rotY);
    mbPlayerMotionShiftSet(playerNo, 4, 0.0f, 8.0f, HU3D_MOTATTR_NONE);
    for(i = 0; i < 18U; i++) {
        t = i/18.0f;
        if(i == 12U) {
            mbPlayerMotionShiftSet(playerNo, motId, 0.0f, 8.0f, HU3D_MOTATTR_NONE);
        }
        target = swing->point[0];
        target.y -= 100.0f;
        PSVECSubtract(&target, &playerPos, &diff);
        pos.x = playerPos.x+(t*diff.x);
        pos.y = playerPos.y+(t*diff.y)+(100.0f*mbSinDeg(180.0f*t));
        pos.z = playerPos.z+(t*diff.z);
        mbPlayerPosSetV(playerNo, &pos);
        HuPrcVSleep();
    }
    swing->mass[0] = 20.0f;
    swing->angVel[0] = 2.0f;
    swing->playerNo = playerNo;
    mbAudFXPlay(0x594);
    for(i = 0; i < 120U; i++) {
        HuPrcVSleep();
    }
    swing->mass[0] = 1.0f;
    swing->playerNo = -1;
    mbAudFXPlay(0x594);
    mbMasuPosGet(lbl_1_bss_1434, &target);
    rotY = HuAtan(diff.x, diff.z);
    mbPlayerMotionShiftSet(playerNo, 4, 0.0f, 8.0f, HU3D_MOTATTR_NONE);
    mbPlayerPosGet(playerNo, &startPos);
    rotStart = mbPlayerRotYGet(playerNo);
    PSVECSubtract(&target, &startPos, &diff2);
    for(j = 0; j <= 18; j++) {
        t2 = j/18.0f;
        if(j == 12U) {
            mbPlayerMotionShiftSet(playerNo, 5, 0.0f, 2.0f, HU3D_MOTATTR_NONE);
        }
        pos2.x = startPos.x+(t2*diff2.x);
        pos2.y = startPos.y+(t2*diff2.y)+(200.0*HuSin(180.0f*t2));
        pos2.z = startPos.z+(t2*diff2.z);
        mbPlayerPosSetV(playerNo, &pos2);
        weight = j/6.0f;
        if(weight > 1.0f) {
            weight = 1.0f;
        }
        mbPlayerRotYSet(playerNo, mbAngleLerp(rotStart, rotY, weight));
        left = 18-j;
        mbPlayerWorkGet(playerNo)->masuNext = left;
        HuPrcVSleep();
    }
    mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 2.0f, HU3D_MOTATTR_LOOP);
    mbPlayerMotionKill(playerNo, motId);
    GwPlayer[playerNo].moveF = FALSE;
    mbPlayerColSnapPlayerSet(playerNo, TRUE);
}

void fn_1_5560(int no)
{
    W01_SWING *swing = &lbl_1_bss_1438[no];
    float delta = swing->angle[0]-swing->angle[1];
    float massRatio = swing->mass[1]/(swing->mass[0]+swing->mass[1]);
    float lenRatio = swing->len[1]/swing->len[0];
    float grav = -1960.0f/swing->len[0];
    float denom;
    float acc0;
    float acc1;

    denom = lenRatio-(massRatio*lenRatio*mbCosRad(delta)*mbCosRad(delta));
    if(0.0f != denom) {
        acc0 = ((lenRatio*(grav*grav)*(-mbSinRad(swing->angle[0])+(massRatio*mbCosRad(delta)*mbSinRad(swing->angle[1]))))
            -(massRatio*lenRatio*((swing->angVel[1]*(lenRatio*swing->angVel[1]))+(swing->angVel[0]*swing->angVel[0]*mbCosRad(delta)))*mbSinRad(delta)))/denom;
        acc1 = (((grav*grav)*mbCosRad(delta)*mbSinRad(swing->angle[0]))-((grav*grav)*mbSinRad(swing->angle[1]))
            +(((swing->angVel[0]*swing->angVel[0])+(swing->angVel[1]*(swing->angVel[1]*(massRatio*lenRatio))*mbCosRad(delta)))*mbSinRad(delta)))/denom;
    } else {
        acc0 = acc1 = 0.0f;
    }
    swing->angVel[0] += (1.0f/60.0f)*acc0;
    swing->angVel[1] += (1.0f/60.0f)*acc1;
    swing->angle[0] += (1.0f/60.0f)*swing->angVel[0];
    swing->angle[1] += (1.0f/60.0f)*swing->angVel[1];
    if(swing->playerNo < 0) {
        swing->angVel[0] *= 0.99f;
        swing->angVel[1] *= 0.99f;
    }
    fn_1_5888(no);
}

void fn_1_5888(int no)
{
    W01_SWING *swing = &lbl_1_bss_1438[no];
    float rotY;
    int i;
    Mtx rotMtx;
    Mtx scaleMtx;
    Mtx mtx;
    HuVecF point[2];
    HuVecF pos;
    HuVecF prevPos;
    HuVecF diff;
    HuVecF axis;
    Quaternion baseQuat;
    Quaternion rotQuat;
    Quaternion quat;

    point[0].x = swing->len[0]*mbSinRad(swing->angle[0]);
    point[0].y = -swing->len[0]*mbCosRad(swing->angle[0]);
    point[0].z = 0.0f;
    point[1].x = point[0].x+(swing->len[1]*mbSinRad(swing->angle[1]));
    point[1].y = point[0].y-(swing->len[1]*mbCosRad(swing->angle[1]));
    point[1].z = 0.0f;
    rotY = -HuAtan(lbl_1_bss_1410.z, lbl_1_bss_1410.x);
    mbMtxRotAxisDeg(rotMtx, 'y', rotY);
    mtxTransCat(rotMtx, swing->pos.x, swing->pos.y, swing->pos.z);
    baseQuat.x = sin(M_PI/4);
    baseQuat.y = baseQuat.z = 0.0f;
    baseQuat.w = cos(M_PI/4);
    axis.x = -lbl_1_bss_1410.z;
    axis.y = 0.0f;
    axis.z = lbl_1_bss_1410.x;
    PSVECNormalize(&axis, &axis);
    prevPos = swing->pos;
    for(i = 0; i < 2; i++) {
        PSMTXMultVec(rotMtx, &point[i], &pos);
        PSVECSubtract(&pos, &prevPos, &diff);
        swing->point[i] = pos;
        mbObjPosSet(swing->objId[i], pos.x-(0.5f*diff.x), pos.y-(0.5f*diff.y), pos.z-(0.5f*diff.z));
        C_QUATRotAxisRad(&rotQuat, &axis, swing->angle[i]);
        PSQUATMultiply(&rotQuat, &baseQuat, &quat);
        PSMTXQuat(mtx, &quat);
        PSMTXScale(scaleMtx, 1.0f, 1.0f, swing->len[i]/100.0f);
        PSMTXConcat(mtx, scaleMtx, mtx);
        mbObjMtxSet(swing->objId[i], mtx);
        if(swing->playerNo >= 0 && i == 0) {
            mbPlayerPosSet(swing->playerNo, pos.x, pos.y-100.0f, pos.z);
        }
        prevPos = pos;
    }
    mbObjPosSetV(swing->baseObjId, &pos);
}

void fn_1_5C18(void)
{
    W01_SWING *swing = lbl_1_bss_1438;

    swing->mass[0] = 1.0f;
    swing->mass[1] = 20.0f;
    swing->angle[0] = swing->angle[1] = 0.0f;
    swing->angVel[0] = swing->angVel[1] = 0.0f;
}

void fn_1_5C7C(void)
{
    W01_COINWORK *work = &lbl_1_bss_13D8;
    W01_COIN *coin;
    int i;

    work->objId = mbObjCreate(W01_DATANUM(32), lbl_1_data_4FC[mbTimeDayCheck() ? 0 : 1], FALSE);
    mbObjMotionSet(work->objId, 0, HU3D_MOTATTR_LOOP);
    mbObjPosSet(work->objId, 0.0f, 4300.0f, -1350.0f);
    if(GwSystem.curTime == 0) {
        Hu3DModelObjPosGet(mbObjModelIDGet(work->objId), "b01_m210", &work->pos);
    } else {
        Hu3DModelObjPosGet(mbObjModelIDGet(work->objId), "b01_m210n", &work->pos);
    }
    work->num = mbTimeDayCheck() ? 50 : 30;
    coin = mbMalloc(work->num*sizeof(W01_COIN));
    work->coin = coin;
    for(i = 0; i < work->num; i++, coin++) {
        if(GwSystem.curTime == 0) {
            coin->objId = mbCoinCreate2();
            mbCoinObjDispSet(coin->objId, FALSE);
        } else {
            coin->objId = mbObjCreate(0xD80022, NULL, TRUE);
            mbObjDispSet(coin->objId, FALSE);
        }
    }
    lbl_1_bss_13D0[0] = HuSprAnimDataRead(mbBoardDataNumGet(0x50063));
    HuSprAnimLock(lbl_1_bss_13D0[0]);
    lbl_1_bss_13D0[1] = HuSprAnimDataRead(mbBoardDataNumGet(0x5005E));
    HuSprAnimLock(lbl_1_bss_13D0[1]);
}

void fn_1_5EF0(void)
{
    int i;

    if(lbl_1_bss_13D8.coin) {
        W01Free(lbl_1_bss_13D8.coin);
        lbl_1_bss_13D8.coin = NULL;
    }
    for(i = 0; i < 2; i++) {
        if(lbl_1_bss_13D0[i]) {
            HuSprAnimKill(lbl_1_bss_13D0[i]);
            lbl_1_bss_13D0[i] = NULL;
        }
    }
}

HuVecF lbl_1_data_518 = { -19.0f, 0.0f, 0.0f };
HuVecF lbl_1_data_524 = { 0.0f, 400.0f, 0.0f };
s32 lbl_1_data_530[4] = { 6, 8, 10, 12 };
s32 lbl_1_data_540[4] = { 5, 4, 3, 2 };

void fn_1_5FB4(int playerNo, s16 masuId)
{
    W01_COIN *coin;
    int i;
    int k;
    W01_COINWORK *work;
    int *order;
    int l;
    int otherNo;
    int j;
    s16 winId;
    int masuNum;
    int playerNum;
    int doneBit;
    int count;
    float angle;
    float radius;
    float rotStart;
    float t;
    float weight;
    s16 masuList[256];
    HuVecF tmpPos;
    HuVecF masuPos;
    HuVecF playerPos;
    int coinCnt[4];
    int coinAdd[4];
    HuVecF diff;
    HuVecF pos;
    HuVecF startPos;
    s16 playerList[4];
    int a;
    int b;
    int left;
    s16 motId;
    s16 linkMasuId;

    work = &lbl_1_bss_13D8;
    order = mbMalloc(work->num*sizeof(int));
    if(GwSystem.curTime == 0) {
        for(i = 0; i < 4; i++) {
            coinCnt[i] = lbl_1_data_530[GwPlayer[i].rank]+mbRandMod(5)-2;
            if(coinCnt[i] < 0) {
                coinCnt[i] = 0;
            }
        }
    } else {
        for(i = 0; i < 4; i++) {
            coinCnt[i] = lbl_1_data_540[GwPlayer[i].rank]+mbRandMod(5)-2;
            if(coinCnt[i] < 0) {
                coinCnt[i] = 0;
            }
        }
    }
    if(!GWTeamGet()) {
        for(i = 0; i < 4; i++) {
            work->unk30[i] = mbPlayerCoinGet(i);
        }
    } else {
        for(i = 0; i < 2; i++) {
            work->unk30[i] = mbPlayerTeamCoinGet(i);
        }
    }
    coin = work->coin;
    for(i = 0; i < work->num; i++, coin++) {
        coin->activeF = 1;
        angle = 360.0f*frandf();
        radius = 1000.0f+(100.0f*(10.0f*frandf()));
        coin->pos.x = work->pos.x;
        coin->pos.y = 650.0f+work->pos.y;
        coin->pos.z = work->pos.z;
        coin->vel.x = radius*mbCosDeg(angle);
        coin->vel.y = 2000.0f;
        coin->vel.z = radius*mbSinDeg(angle);
        coin->delay = mbRandMod(30)+1;
        if(GwSystem.curTime == 0) {
            coin->rot.y = 360.0f*frandf();
            mbCoinObjPosSetV(coin->objId, &coin->pos);
            mbCoinObjDispSet(coin->objId, FALSE);
        } else {
            coin->rot.x = 360.0f*frandf();
            coin->rot.y = 360.0f*frandf();
            mbObjPosSetV(coin->objId, &coin->pos);
            mbObjDispSet(coin->objId, FALSE);
        }
        coin->mode = FALSE;
    }
    motId = mbPlayerMotionCreate(playerNo, 0x93001E);
    for(i = 0; i < 4; i++) {
        work->motNo[i] = mbPlayerMotionCreate(i, 0x930019);
    }
    mbPlayerPosGet(playerNo, &playerPos);
    linkMasuId = mbMasuAttrFindLink(masuId, 0x2000);
    mbMasuPosGet(linkMasuId, &masuPos);
    mbPlayerMotionShiftSet(playerNo, 4, 0.0f, 8.0f, HU3D_MOTATTR_NONE);
    mbPlayerPosGet(playerNo, &startPos);
    rotStart = mbPlayerRotYGet(playerNo);
    PSVECSubtract(&masuPos, &startPos, &diff);
    for(j = 0; j <= 18; j++) {
        t = j/18.0f;
        if(j == 12U) {
            mbPlayerMotionShiftSet(playerNo, 5, 0.0f, 2.0f, HU3D_MOTATTR_NONE);
        }
        pos.x = startPos.x+(t*diff.x);
        pos.y = startPos.y+(t*diff.y)+(100.0*HuSin(180.0f*t));
        pos.z = startPos.z+(t*diff.z);
        mbPlayerPosSetV(playerNo, &pos);
        weight = j/6.0f;
        if(weight > 1.0f) {
            weight = 1.0f;
        }
        mbPlayerRotYSet(playerNo, mbAngleLerp(rotStart, 180.0f, weight));
        left = 18-j;
        mbPlayerWorkGet(playerNo)->masuNext = left;
        HuPrcVSleep();
    }
    mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 2.0f, HU3D_MOTATTR_LOOP);
    mbCameraMoveMasu(linkMasuId, &lbl_1_data_518, &lbl_1_data_524, 2875.0f, -1.0f, 30);
    mbCameraMoveWait();
    if(GwSystem.curTime == 0) {
        winId = mbWinCreate(2, 0x2F0000, 17);
    } else {
        winId = mbWinCreate(2, 0x2F0002, 18);
    }
    mbWinWait(winId);
    HuPrcSleep(30);
    mbObjMotionShiftSet(work->objId, 1, 0.0f, 4.0f, HU3D_MOTATTR_NONE);
    HuPrcSleep(30);
    mbPlayerMotionShiftSet(playerNo, motId, 0.0f, 2.0f, HU3D_MOTATTR_NONE);
    mbCameraShakeSet(30, 200.0f);
    mbAudFXPlay(0x596);
    for(i = 0; i < 4; i++) {
        omVibrate(i, 20, 20, 0);
    }
    for(i = 0; i < 60u; i++) {
        fn_1_6D98();
        HuPrcVSleep();
    }
    mbWipeFadeOut();
    mbObjMotionSet(work->objId, 0, HU3D_MOTATTR_LOOP);
    mbPlayerMotIdleSet(playerNo);
    mbPlayerRotYSet(playerNo, 0.0f);
    for(i = 0; i < work->num; i++) {
        order[i] = i;
    }
    for(i = 0; i < 100; i++) {
        a = mbRandMod(work->num);
        b = mbRandMod(work->num);
        k = order[a];
        order[a] = order[b];
        order[b] = k;
    }
    mbev_PlayerColMasuAdd(playerNo, masuId, TRUE);
    mbPlayerPosReset(playerNo);
    for(i = 0; i < 4; i++) {
        CharModelVoiceFlagSet(GwPlayer[i].charNo, FALSE);
    }
    doneBit = 0;
    for(i = 0; i < 4; i++) {
        if(doneBit & (1 << i)) {
            continue;
        }
        mbCameraPlayerViewSetFast(i, 2);
        mbCameraFocusMasuSet(GwPlayer[i].masuId);
        HuPrcVSleep();
        masuId = GwPlayer[i].masuId;
        masuNum = 0;
        for(k = 1; k < mbMasuNumGet(); k++) {
            for(l = 0; l < 4; l++) {
                if(k == GwPlayer[l].masuId) {
                    break;
                }
            }
            if(l >= 4 && mbMasuDispCheck(k)) {
                mbMasuPosGet(k, &tmpPos);
                if(mbCameraCullCheck(&tmpPos, 200.0f)) {
                    masuList[masuNum++] = k;
                }
            }
        }
        playerNum = 0;
        for(k = 0; k < 4; k++) {
            if(!(doneBit & (1 << k)) && masuId == GwPlayer[k].masuId) {
                playerList[playerNum++] = k;
            }
        }
        coin = work->coin;
        for(k = 0; k < work->num; k++, coin++) {
            coin->activeF = 0;
            coin->mode = TRUE;
            coin->vel.x = coin->vel.y = coin->vel.z = 0.0f;
            coin->delay = (order[k]*2)+1;
            if(GwSystem.curTime == 0) {
                mbCoinObjDispSet(coin->objId, FALSE);
            } else {
                mbObjDispSet(coin->objId, FALSE);
            }
        }
        count = 0;
        coin = work->coin;
        for(k = 0; k < playerNum; k++) {
            otherNo = playerList[k];
            mbPlayerPosGet(otherNo, &tmpPos);
            for(l = 0; l < coinCnt[otherNo]; l++, coin++) {
                coin->activeF = 1;
                coin->pos.x = tmpPos.x;
                coin->pos.y = 2000.0f+tmpPos.y;
                coin->pos.z = tmpPos.z;
                coin->groundY = 150.0f+tmpPos.y;
                coin->playerNo = otherNo;
                count++;
            }
        }
        if(masuNum > 0) {
            for(k = 0; k < work->num-count; k++, coin++) {
                masuId = masuList[mbRandMod(masuNum)];
                mbMasuPosGet(masuId, &tmpPos);
                coin->activeF = 1;
                coin->pos.x = tmpPos.x;
                coin->pos.y = 2000.0f+tmpPos.y;
                coin->pos.z = tmpPos.z;
                coin->groundY = 50.0f+tmpPos.y;
                coin->playerNo = -1;
                tmpPos.y += 50.0f;
                coin->modelId = fn_1_7388(&tmpPos);
                Hu3DModelAttrSet(coin->modelId, HU3D_ATTR_DISPOFF);
            }
        }
        mbWipeFadeIn();
        while(fn_1_6D98()) {
            HuPrcVSleep();
        }
        for(k = 0; k < 4; k++) {
            coinAdd[k] = 0;
        }
        for(k = 0; k < playerNum; k++) {
            otherNo = playerList[k];
            doneBit |= 1 << otherNo;
            if(GwSystem.curTime == 0) {
                coinAdd[otherNo] = coinCnt[otherNo];
            } else {
                coinAdd[otherNo] = -coinCnt[otherNo];
            }
        }
        mbCoinAddAllProcExecV(coinAdd, coinAdd, TRUE);
        mbWipeFadeOut();
    }
    mbPlayerMotionKill(playerNo, motId);
    for(i = 0; i < 4; i++) {
        mbPlayerMotionKill(i, work->motNo[i]);
        mbPlayerMotionSet(i, 1, HU3D_MOTATTR_LOOP);
        CharModelVoiceFlagSet(GwPlayer[i].charNo, TRUE);
    }
    mbCameraMoveMasu(linkMasuId, &lbl_1_data_518, &lbl_1_data_524, 2600.0f, -1.0f, -1);
    mbWipeFadeIn();
    HuMemDirectFree(order);
    if(GwSystem.curTime == 0) {
        winId = mbWinCreate(2, 0x2F0001, 17);
    } else {
        winId = mbWinCreate(2, 0x2F0003, 18);
    }
    mbWinWait(winId);
}

/* @@PART3@@ */
/* .data 0x550-0x640, in definition order (lbl_1_data_5B4 and 5C4 are used by part 4's
   fn_1_B01C but lie between part 3's tables) */
int lbl_1_data_550[3] = { DATANUM(DATA_w01, 15), DATANUM(DATA_w01, 16), DATANUM(DATA_w01, 17) };
char *lbl_1_data_56C[3] = { "k_h1", "k_h2", "k_h3" };
int lbl_1_data_578[2] = { DATANUM(DATA_w01, 35), DATANUM(DATA_w01, 38) };
int lbl_1_data_580[2] = { DATANUM(DATA_w01, 36), DATANUM(DATA_w01, 39) };
u32 lbl_1_data_588[2][3] = { { 0x400000, 0x800000, 0x4000000 }, { 0x100000, 0x200000, 0x2000000 } };
char *lbl_1_data_5AC[2] = { "sf_h", "mf_h" };
HuVecF *lbl_1_data_5B4[4] = { lbl_1_data_0, lbl_1_data_90, lbl_1_data_120, lbl_1_data_1B0 };
int lbl_1_data_5C4[4] = { 12, 12, 12, 12 };
int lbl_1_data_5D4[4] = { DATANUM(DATA_capsuleshop, 6), DATANUM(DATA_capsuleshop, 6), DATANUM(DATA_capsuleshop, 7), -1 };
int lbl_1_data_5E4[4] = { DATANUM(DATA_capsuleshop, 1), DATANUM(DATA_capsuleshop, 1), DATANUM(DATA_capsuleshop, 4), -1 };
int lbl_1_data_5F4[3] = { DATANUM(DATA_w01, 28), DATANUM(DATA_w01, 29), DATANUM(DATA_w01, 30) };
char *lbl_1_data_610[3] = { "w_h1", "w_h2", "w_h3" };
HuVecF lbl_1_data_61C = { -10000.0f, 10000.0f, -10000.0f };
HuVecF lbl_1_data_628 = { 1.0f, -1.0f, -1.0f };
GXColor lbl_1_data_634 = { 0xFF, 0xFF, 0xFF, 0xFF };

int fn_1_6D98(void)
{
    int i;
    int num = 0;
    W01_COINWORK *work = &lbl_1_bss_13D8;
    W01_COIN *coin = work->coin;
    float angle;
    int teamNo;
    HuVecF pos;

    for (i = 0; i < work->num; i++, coin++) {
        if (!coin->activeF) {
            continue;
        }
        num++;
        if (coin->delay != 0) {
            if (--coin->delay != 0) {
                continue;
            }
            if (GwSystem.curTime == 0) {
                mbCoinObjDispSet(coin->objId, TRUE);
            } else {
                mbObjDispSet(coin->objId, TRUE);
            }
        }
        coin->vel.y += -32.666668f;
        coin->pos.x += 0.016666668f * coin->vel.x;
        coin->pos.y += 0.016666668f * coin->vel.y;
        coin->pos.z += 0.016666668f * coin->vel.z;
        if (GwSystem.curTime == 0) {
            mbCoinObjPosSetV(coin->objId, &coin->pos);
            coin->rot.y += 4.0f;
            mbCoinObjRotSet(coin->objId, 0.0f, coin->rot.y, 0.0f);
        } else {
            mbObjPosSetV(coin->objId, &coin->pos);
            coin->rot.y += 10.0f;
            coin->rot.x += 20.0f;
            mbObjRotSetV(coin->objId, &coin->rot);
        }
        switch (coin->mode) {
            case 1:
                if (coin->playerNo >= 0 && GwSystem.curTime != 0 && coin->pos.y < 500.0f + coin->groundY) {
                    if (work->motNo[coin->playerNo] != mbObjMotionGet(mbPlayerObjIDGet(coin->playerNo))) {
                        mbPlayerMotionShiftSet(coin->playerNo, work->motNo[coin->playerNo], 0.0f, 4.0f, 0);
                    }
                }
                if (coin->pos.y < coin->groundY) {
                    if (coin->playerNo < 0) {
                        Hu3DModelAttrReset(coin->modelId, HU3D_ATTR_DISPOFF);
                        if (GwSystem.curTime == 0) {
                            mbCoinObjDispSet(coin->objId, FALSE);
                        } else {
                            mbObjDispSet(coin->objId, FALSE);
                        }
                        coin->activeF = FALSE;
                    } else if (GwSystem.curTime == 0) {
                        mbCoinEffCreate(&coin->pos);
                        mbCoinObjDispSet(coin->objId, FALSE);
                        coin->activeF = FALSE;
                    } else {
                        angle = 360.0f * frandf();
                        coin->vel.x = 100.0f * (6.0f * mbCosDeg(angle));
                        coin->vel.y = 1000.0f;
                        coin->vel.z = 100.0f * (6.0f * mbSinDeg(angle));
                        coin->mode = 2;
                        coin->groundY -= 150.0f;
                        coin->time = 0;
                        omVibrate(coin->playerNo, 20, 7, 3);
                        mbPlayerMotionShiftSet(coin->playerNo, 6, 0.0f, 4.0f, 0x40000001);
                        work->unk20[coin->playerNo] = 0.5f;
                        if (!GWTeamGet()) {
                            if (work->unk30[coin->playerNo] > 0) {
                                mbPlayerPosGet(coin->playerNo, &pos);
                                pos.y += 100.0f;
                                fn_1_13D8C(&pos, 1);
                                work->unk30[coin->playerNo]--;
                            }
                        } else {
                            teamNo = GWPlayerTeamGet(coin->playerNo);
                            if (work->unk30[teamNo] > 0) {
                                mbPlayerPosGet(coin->playerNo, &pos);
                                pos.y += 100.0f;
                                fn_1_13D8C(&pos, 1);
                                work->unk30[teamNo]--;
                            }
                        }
                        mbAudFXPlay(0x597);
                        CharFXPlay(GwPlayer[coin->playerNo].charNo, 0x248);
                    }
                }
                break;
            case 2:
                if (++coin->time & 1) {
                    mbObjDispSet(coin->objId, TRUE);
                } else {
                    mbObjDispSet(coin->objId, FALSE);
                }
                if (coin->pos.y < coin->groundY) {
                    mbObjDispSet(coin->objId, FALSE);
                    coin->activeF = FALSE;
                }
                break;
        }
    }
    return num;
}

HU3D_MODELID fn_1_7388(HuVecF *pos)
{
    HU3D_MODELID modelId;

    if (GwSystem.curTime == 0) {
        modelId = mbParticleCreate(lbl_1_bss_13D0[0], 10);
        mbParticleHookSet(modelId, fn_1_7448);
    } else {
        modelId = mbParticleCreate(lbl_1_bss_13D0[1], 1);
        mbParticleHookSet(modelId, fn_1_77AC);
        mbParticleAttrSet(modelId, MB_PARTICLE_ATTR_ANIM);
    }
    Hu3DModelLayerSet(modelId, 5);
    Hu3DModelPosSetV(modelId, pos);
    return modelId;
}

void fn_1_7448(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx)
{
    GXColor color = { 0xFF, 0xFF, 0xC0, 0xC0 };
    MBPARTICLEDATA *data;
    int i;
    int num;
    float angle;
    float speed;
    float t;

    if (particleP->unk00 == 0) {
        data = particleP->data;
        for (i = 0; i < particleP->num; i++, data++) {
            angle = 360.0f * frandf();
            speed = 200.0f + 100.0f * (2.0f * frandf());
            data->pos.x = data->pos.y = data->pos.z = 0.0f;
            data->vel.x = speed * mbCosDeg(angle);
            data->vel.y = 500.0f + 100.0f * (5.0f * frandf());
            data->vel.z = speed * mbSinDeg(angle);
            data->scale = 40.0f + 20.0f * frandf();
            data->rot.z = mbRandMod(360);
            data->color = color;
            data->time = 0;
            data->aliveF = 30;
        }
        particleP->unk00 = 1;
    }
    num = 0;
    data = particleP->data;
    for (i = 0; i < particleP->num; i++, data++) {
        if (data->time > data->aliveF) {
            continue;
        }
        data->vel.y += -65.333336f;
        data->pos.x += 0.016666668f * data->vel.x;
        data->pos.y += 0.016666668f * data->vel.y;
        data->pos.z += 0.016666668f * data->vel.z;
        data->rot.z += 10.0f;
        if (data->aliveF - data->time < 20) {
            t = (data->aliveF - data->time) / 20.0f;
            data->color.a = 255.0f * t;
        }
        data->time++;
        num++;
    }
    if (num == 0) {
        mbParticleKill(particleP->modelId);
    }
}

void fn_1_77AC(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx)
{
    GXColor color = { 0xFF, 0xFF, 0xFF, 0xC0 };
    MBPARTICLEDATA *data;
    int i;
    int num;

    if (particleP->unk00 == 0) {
        data = particleP->data;
        for (i = 0; i < particleP->num; i++, data++) {
            data->pos.x = 50.0f - 100.0f * frandf();
            data->pos.y = 0.0f;
            data->pos.z = 50.0f - 100.0f * frandf();
            data->scale = 200.0f + 20.0f * frandf();
            data->color = color;
            data->animSpeed = 0.5f;
        }
        particleP->unk00 = 1;
    }
    num = 0;
    data = particleP->data;
    for (i = 0; i < particleP->num; i++, data++) {
        if (data->pauseF == 0) {
            num++;
        }
    }
    if (num == 0) {
        mbParticleKill(particleP->modelId);
    }
}

void fn_1_7944(void)
{
    W01_LIFT *work = lbl_1_bss_137C;
    int i;
    u32 mAttr;

    lbl_1_bss_1378 = HuSprAnimRead(HuDataSelHeapReadNum(W01_DATANUM(0x32), HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(lbl_1_bss_1378);
    for (i = 0; i < 3; i++, work++) {
        work->objId = mbObjCreate(W01_DATANUM(lbl_1_data_550[i] & 0xFFFF), NULL, FALSE);
        mbObjMotionSpeedSet(work->objId, 0.0f);
        mAttr = mbev_MasuAttrGet(i + 1, 0x30000);
        work->masuId = mbMasuFind_MAttrMatchIdGet(-1, mAttr, 0x30000);
        strcpy(work->objName, lbl_1_data_56C[i]);
        work->particleId[0] = mbParticleCreate(lbl_1_bss_1378, 20);
        Hu3DModelLayerSet(work->particleId[0], 5);
        mbParticleHookSet(work->particleId[0], fn_1_7D84);
        Hu3DModelAttrSet(work->particleId[0], HU3D_ATTR_DISPOFF);
        work->particleId[1] = mbParticleCreate(lbl_1_bss_1378, 20);
        Hu3DModelLayerSet(work->particleId[1], 5);
        mbParticleHookSet(work->particleId[1], fn_1_80B4);
        Hu3DModelAttrSet(work->particleId[1], HU3D_ATTR_DISPOFF);
    }
}

void fn_1_7B20(void)
{
    if (lbl_1_bss_1378) {
        HuSprAnimKill(lbl_1_bss_1378);
        lbl_1_bss_1378 = NULL;
    }
}

void fn_1_7B70(int no)
{
    W01_LIFT *work = &lbl_1_bss_137C[no];
    HuVecF pos;

    mbObjMotionTimeSet(work->objId, 0.0f);
    mbObjMotionSpeedSet(work->objId, 1.0f);
    mbMasuPosGet(work->masuId, &pos);
    pos.y -= 100.0f;
    fn_1_7D14(work, &pos);
    mbAudFXPlay(0x590);
    work->activeF = TRUE;
}

void fn_1_7C14(void)
{
    W01_LIFT *work = lbl_1_bss_137C;
    int i;
    HU3D_MODELID modelId;
    HU3D_MODEL *modelP;
    HuVecF pos;

    for (i = 0; i < 3; i++, work++) {
        if (!work->activeF) {
            continue;
        }
        if (mbObjMotionEndCheck(work->objId)) {
            work->activeF = FALSE;
        }
        modelId = mbObjModelIDGet(work->objId);
        modelP = &Hu3DData[modelId];
        Hu3DMotionExec(modelId, modelP->motId, 1.0f + mbObjMotionTimeGet(work->objId), FALSE);
        Hu3DModelObjPosGet(modelId, work->objName, &pos);
        pos.y += 7.5f;
        mbMasuPosSetV(work->masuId, &pos);
    }
}

void fn_1_7D14(W01_LIFT *work, HuVecF *pos)
{
    int i;

    for (i = 0; i < 2; i++) {
        Hu3DModelPosSetV(work->particleId[i], pos);
        Hu3DModelAttrReset(work->particleId[i], HU3D_ATTR_DISPOFF);
    }
}

void fn_1_7D84(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx)
{
    GXColor color = { 0xFF, 0xFF, 0xC0, 0xC0 };
    MBPARTICLEDATA *data;
    int i;
    int num;
    float angle;
    float radius;
    float t;

    if (particleP->unk00 == 0) {
        data = particleP->data;
        for (i = 0; i < particleP->num; i++, data++) {
            angle = 360.0f * frandf();
            radius = 100.0f + 100.0f * frandf();
            data->pos.x = radius * mbCosDeg(angle);
            data->pos.y = 0.0f;
            data->pos.z = radius * mbSinDeg(angle);
            data->vel.y = -500.0f - 500.0f * frandf();
            data->accel.y = -500.0f - 500.0f * frandf();
            data->scale = 100.0f + 20.0f * frandf();
            data->color = color;
            data->time = 0;
            data->aliveF = 60.0f + 30.0f * frandf();
        }
        particleP->unk00 = 1;
    }
    num = 0;
    data = particleP->data;
    for (i = 0; i < particleP->num; i++, data++) {
        if (data->time > data->aliveF) {
            continue;
        }
        t = data->time++ / (float)data->aliveF;
        data->pos.y += 0.016666668f * data->vel.y;
        data->vel.y += 0.016666668f * data->accel.y;
        data->color.a = 192.0f * (1.0f - t);
        num++;
    }
    if (num == 0) {
        particleP->unk00 = 0;
        Hu3DModelAttrSet(particleP->modelId, HU3D_ATTR_DISPOFF);
    }
}

void fn_1_80B4(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx)
{
    GXColor color = { 0xFF, 0xFF, 0xC0, 0xC0 };
    MBPARTICLEDATA *data;
    int i;
    int num;
    float angle;
    float radius;
    float t;

    if (particleP->unk00 == 0) {
        data = particleP->data;
        for (i = 0; i < particleP->num; i++, data++) {
            angle = 360.0f * frandf();
            radius = 200.0f + 100.0f * (0.5f * frandf());
            data->pos.x = radius * mbCosDeg(angle);
            data->pos.y = 0.0f;
            data->pos.z = radius * mbSinDeg(angle);
            data->vel.x = 100.0f * (10.0f * mbCosDeg(angle));
            data->vel.y = -1000.0f;
            data->vel.z = 100.0f * (10.0f * mbSinDeg(angle));
            data->scale = 100.0f + 20.0f * frandf();
            data->color = color;
            data->time = 0;
            data->aliveF = 60.0f + 30.0f * frandf();
        }
        particleP->unk00 = 1;
    }
    num = 0;
    data = particleP->data;
    for (i = 0; i < particleP->num; i++, data++) {
        if (data->time > data->aliveF) {
            continue;
        }
        t = data->time++ / (float)data->aliveF;
        data->pos.x += 0.016666668f * data->vel.x;
        data->pos.y += 0.016666668f * data->vel.y;
        data->pos.z += 0.016666668f * data->vel.z;
        data->vel.x *= 0.9f;
        data->vel.y *= 0.9f;
        data->vel.z *= 0.9f;
        data->color.a = 192.0f * (1.0f - t);
        num++;
    }
    if (num == 0) {
        particleP->unk00 = 0;
        Hu3DModelAttrSet(particleP->modelId, HU3D_ATTR_DISPOFF);
    }
}

void fn_1_8474(void)
{
    W01_FLOWER *work = lbl_1_bss_1338;
    MASU *masuP;
    int j;
    int i;
    int k;
    int dataNum;

    lbl_1_bss_1338[0].dayF = mbTimeDayCheck() ? TRUE : FALSE;
    lbl_1_bss_1338[1].dayF = lbl_1_bss_1338[0].dayF ^ 1;
    for (work = lbl_1_bss_1338, i = 0; i < 2; i++, work++) {
        if (work->dayF) {
            dataNum = lbl_1_data_578[i];
        } else {
            dataNum = lbl_1_data_580[i];
        }
        work->objId = mbObjCreate(W01_DATANUM(dataNum & 0xFFFF), NULL, FALSE);
        mbObjMotionSet(work->objId, 0, 0);
        mbObjMotionSpeedSet(work->objId, 0.0f);
        mbObjMotionShapeSet(work->objId, 0, 0);
        mbObjMotionShapeSpeedSet(work->objId, 0.0f);
        work->masuId[0] = mbMasuFind_MAttrIdGet(-1, lbl_1_data_588[i][0]);
        work->masuId[1] = mbMasuFind_MAttrIdGet(-1, lbl_1_data_588[i][1]);
        work->masuId[2] = mbMasuFind_MAttrIdGet(-1, lbl_1_data_588[i][2]);
        masuP = mbMasuGet(work->masuId[0]);
        if (work->dayF) {
            for (j = 0; j < masuP->linkNum; j++) {
                if (masuP->linkTbl[j] == work->masuId[2]) {
                    for (k = j + 1; k < masuP->linkNum; k++) {
                        masuP->linkTbl[j] = masuP->linkTbl[k];
                    }
                    masuP->linkNum--;
                }
            }
        } else {
            for (j = 0; j < masuP->linkNum; j++) {
                if (masuP->linkTbl[j] == work->masuId[1]) {
                    for (k = j + 1; k < masuP->linkNum; k++) {
                        masuP->linkTbl[j] = masuP->linkTbl[k];
                    }
                    masuP->linkNum--;
                }
            }
        }
        strcpy(work->objName, lbl_1_data_5AC[i]);
        work->activeF = TRUE;
    }
    lbl_1_bss_1334 = HuSprAnimRead(HuDataSelHeapReadNum(mbBoardDataNumGet(0x50063), HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(lbl_1_bss_1334);
}

void fn_1_8798(void)
{
    if (lbl_1_bss_1334) {
        HuSprAnimKill(lbl_1_bss_1334);
        lbl_1_bss_1334 = NULL;
    }
}

void fn_1_87E8(void)
{
    W01_FLOWER *work = lbl_1_bss_1338;
    int i;
    HuVecF pos;

    for (i = 0; i < 2; i++, work++) {
        if (work->activeF) {
            Hu3DModelObjPosGet(mbObjModelIDGet(work->objId), work->objName, &pos);
            mbMasuPosSetV(work->masuId[0], &pos);
        }
    }
}

void fn_1_8860(int playerNo)
{
    W01_FLOWER *work;
    u32 i;
    int j;
    int no;
    s16 masuId;
    HU3D_MODELID particleId;
    HU3D_MODELID modelId;
    int parNo;
    int rest;
    float t;
    float rotY;
    float t2;
    float weight;
    float rotYStart;
    HuVecF start;
    HuVecF end;
    HuVecF pos;
    HuVecF dir;
    HuVecF rot;
    HuVecF dir2;
    HuVecF pos3;
    HuVecF pos2;

    masuId = GwPlayer[playerNo].masuId;
    no = (mbMasuMAttrGet(masuId) & 0x400000) ? 0 : 1;
    work = &lbl_1_bss_1338[no];
    GwPlayer[playerNo].moveF = TRUE;
    mbPlayerColSnapPlayerSet(playerNo, FALSE);
    mbPlayerWorkGet(playerNo)->unk10_3 = FALSE;
    if (work->dayF) {
        particleId = mbParticleCreate(lbl_1_bss_1334, 200);
        Hu3DModelLayerSet(particleId, 5);
        mbParticleHookSet(particleId, fn_1_9134);
        Hu3DParticleBlendModeSet(particleId, 1);
        mbPlayerMotIdleSet(playerNo);
        mbObjMotionTimeSet(work->objId, 0.0f);
        mbObjMotionSpeedSet(work->objId, 1.0f);
        mbObjMotionShapeTimeSet(work->objId, 0.0f);
        mbObjMotionShapeSpeedSet(work->objId, 1.0f);
        modelId = mbObjModelIDGet(work->objId);
        mbAudFXPlay(0x591);
        for (i = 0; i < 60; i++) {
            if (i == 30) {
                mbPlayerScaleSet(playerNo, 1.0f, 0.5f, 1.0f);
            }
            Hu3DModelObjPosGet(modelId, work->objName, &pos);
            mbPlayerPosSetV(playerNo, &pos);
            HuPrcVSleep();
        }
        mbAudFXPlay(0x592);
        mbPlayerPosGet(playerNo, &start);
        mbMasuPosGet(work->masuId[1], &end);
        PSVECSubtract(&end, &start, &dir);
        rotY = HuAtan(dir.x, dir.z);
        mbPlayerRotYSet(playerNo, 180.0f + rotY);
        mbPlayerMotionShiftSet(playerNo, 4, 0.0f, 2.0f, 0);
        mbPlayerRotGet(playerNo, &rot);
        mbPlayerScaleSet(playerNo, 1.0f, 1.0f, 1.0f);
        GwPlayer[playerNo].masuIdNext = work->masuId[1];
        parNo = 0;
        omVibrate(playerNo, 20, 7, 3);
        for (i = 0; i <= 48; i++) {
            t = (s32)i / 48.0f;
            if (i == 24) {
                CharModelVoiceFlagSet(GwPlayer[playerNo].charNo, FALSE);
                mbPlayerMotionShiftSet(playerNo, 5, 0.0f, 2.0f, 0);
            } else if (i == 30) {
                mbObjMotionSpeedSet(work->objId, -1.0f);
                mbObjMotionShapeSpeedSet(work->objId, -1.0f);
            }
            pos.x = start.x + t * dir.x;
            pos.y = start.y + t * dir.y + 100.0f * (10.0f * mbSinDeg(180.0f * t));
            pos.z = start.z + t * dir.z;
            mbPlayerPosSetV(playerNo, &pos);
            mbPlayerRotSet(playerNo, 360.0f * mbSinDeg(90.0f * (1.0f - t)), rot.y, rot.z);
            fn_1_939C(particleId, parNo++, &pos);
            mbPlayerWorkGet(playerNo)->masuNext = 48 - i;
            HuPrcVSleep();
        }
        CharModelVoiceFlagSet(GwPlayer[playerNo].charNo, TRUE);
        mbObjMotionTimeSet(work->objId, 0.0f);
        mbObjMotionSpeedSet(work->objId, 0.0f);
        mbObjMotionShapeTimeSet(work->objId, 0.0f);
        mbObjMotionShapeSpeedSet(work->objId, 0.0f);
        GwPlayer[playerNo].masuId = work->masuId[1];
    } else {
        mbAudFXPlay(0x593);
        mbObjMotionTimeSet(work->objId, 5.0f);
        mbObjMotionSpeedSet(work->objId, 1.0f);
        mbObjMotionShapeTimeSet(work->objId, 5.0f);
        mbObjMotionShapeSpeedSet(work->objId, 1.0f);
        mbPlayerPosGet(playerNo, &start);
        mbMasuPosGet(work->masuId[2], &end);
        PSVECSubtract(&end, &start, &dir);
        rotY = HuAtan(dir.x, dir.z);
        mbPlayerRotYSet(playerNo, rotY);
        GwPlayer[playerNo].masuIdNext = work->masuId[2];
        omVibrate(playerNo, 20, 4, 4);
        mbPlayerMotionShiftSet(playerNo, 4, 0.0f, 8.0f, 0);
        mbPlayerPosGet(playerNo, &pos2);
        rotYStart = mbPlayerRotYGet(playerNo);
        PSVECSubtract(&end, &pos2, &dir2);
        for (j = 0; j <= 30; j++) {
            t2 = j / 30.0f;
            if ((u32)j == 24) {
                mbPlayerMotionShiftSet(playerNo, 5, 0.0f, 2.0f, 0);
            }
            pos3.x = pos2.x + t2 * dir2.x;
            pos3.y = pos2.y + t2 * dir2.y + 300.0 * HuSin(180.0f * t2);
            pos3.z = pos2.z + t2 * dir2.z;
            mbPlayerPosSetV(playerNo, &pos3);
            weight = j / 6.0f;
            if (weight > 1.0f) {
                weight = 1.0f;
            }
            mbPlayerRotYSet(playerNo, mbAngleLerp(rotYStart, rotY, weight));
            rest = 30 - j;
            mbPlayerWorkGet(playerNo)->masuNext = rest;
            HuPrcVSleep();
        }
        mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 2.0f, 0x40000001);
        GwPlayer[playerNo].masuId = work->masuId[2];
    }
    GwPlayer[playerNo].moveF = FALSE;
    mbPlayerColSnapPlayerSet(playerNo, TRUE);
    mbPlayerWorkGet(playerNo)->unk10_3 = TRUE;
}

void fn_1_9134(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx)
{
    MBPARTICLEDATA *data;
    int i;
    int num;
    float t;

    if (particleP->unk00 == 0) {
        data = particleP->data;
        for (i = 0; i < particleP->num; i++, data++) {
            data->pos.x = data->pos.y = data->pos.z = 0.0f;
            data->scale = 0.0f;
            data->color.r = 0xFF;
            data->color.g = 0xFF;
            data->color.b = mbRandMod(128) + 128;
            data->rot.z = 360.0f * frandf();
            data->time = -1;
            data->aliveF = 30.0f + 30.0f * frandf();
        }
        particleP->unk00 = 1;
    }
    num = 0;
    data = particleP->data;
    for (i = 0; i < particleP->num; i++, data++) {
        if (data->time < 0) {
            continue;
        }
        if (data->time > data->aliveF) {
            continue;
        }
        t = data->time++ / (float)data->aliveF;
        data->vel.y += -16.333334f;
        data->pos.y += 0.016666668f * data->vel.y;
        data->color.a = 255.0f * (1.0f - t);
        data->rot.z += 8.0f;
        num++;
    }
    if (particleP->unk00 == 2 && num == 0) {
        mbParticleKill(particleP->modelId);
    }
}

void fn_1_939C(HU3D_MODELID modelId, int no, HuVecF *pos)
{
    void *hookData = Hu3DData[modelId].hookData;
    MBPARTICLE *particleP = hookData;
    MBPARTICLEDATA *data = &particleP->data[no];

    data->pos.x = pos->x + 100.0f * (1.2f * frandf()) - 1.2f * 100.0f / 2;
    data->pos.y = pos->y + 100.0f * (1.2f * frandf()) - 1.2f * 100.0f / 2;
    data->pos.z = pos->z + 100.0f * (1.2f * frandf()) - 1.2f * 100.0f / 2;
    data->scale = 30.0f + 30.0f * frandf();
    data->time = 0;
    particleP->unk00 = 2;
}

void fn_1_950C(BOOL dispF)
{
    W01_FLOWER *work = lbl_1_bss_1338;
    int i;

    for (i = 0; i < 2; i++, work++) {
        mbObjDispSet(work->objId, dispF);
        work->activeF = dispF;
    }
}

void fn_1_9574(void)
{
    W01_BRANCH *work = lbl_1_bss_1240;
    s16 *objId = lbl_1_bss_1238;
    int i;
    u32 mAttr;
    HuVecF pos;

    objId[0] = mbObjCreate(W01_DATANUM(0x1B), NULL, FALSE);
    mbObjAttrSet(objId[0], 0x40000002);
    mbObjPosSet(objId[0], 410.0f, 1900.0f, 1137.0f);
    objId[1] = mbObjCreate(W01_DATANUM(0x1F), NULL, FALSE);
    mbObjAttrSet(objId[1], 0x40000001);
    mbObjHookSet(objId[0], "b01_m200", objId[1]);
    if (GwSystem.curTime == 0) {
        objId[2] = mbObjCreate(DATANUM(DATA_capsuleshop, 5), lbl_1_data_5D4, FALSE);
    } else {
        objId[2] = mbObjCreate(DATANUM(DATA_capsuleshop, 0), lbl_1_data_5E4, FALSE);
    }
    if (GwSystem.curTime != 0) {
        objId[3] = Hu3DLLightCreateV(mbObjModelIDGet(objId[2]), &lbl_1_data_61C, &lbl_1_data_628, &lbl_1_data_634);
        Hu3DLLightStaticSet(mbObjModelIDGet(objId[2]), objId[3], TRUE);
        Hu3DLLightInfinitytSet(mbObjModelIDGet(objId[2]), objId[3]);
    }
    mbObjMotionSet(objId[2], 1, 0x40000001);
    mbObjDispSet(objId[2], FALSE);
    mbObjPosGet(objId[0], &pos);
    mbObjPosSetV(objId[2], &pos);
    for (i = 0; i < 3; i++, work++) {
        work->objId = mbObjCreate(W01_DATANUM(lbl_1_data_5F4[i] & 0xFFFF), NULL, FALSE);
        strcpy(work->objName, lbl_1_data_610[i]);
        mbObjDispSet(work->objId, FALSE);
        work->leafNum = fn_1_B434(&work->leaf);
        work->attachF = 1;
        work->obj = omAddObjEx(mbObjMan, 0x200C, 0, 0, -1, fn_1_ACDC);
        work->obj->work[0] = i;
        work->playerNo = -1;
    }
    for (i = 0; i < 4; i++) {
        mAttr = mbev_MasuAttrGet(i + 2, 0xE0000000);
        lbl_1_bss_1230[i] = mbMasuFind_MAttrMatchIdGet(-1, mAttr, 0xE0000000);
    }
    lbl_1_bss_121C = HuSprAnimRead(HuDataSelHeapReadNum(W01_DATANUM(0x33), HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(lbl_1_bss_121C);
    lbl_1_bss_120C[0] = fn_1_C7DC(&lbl_1_bss_1214[0]);
    lbl_1_bss_120C[1] = fn_1_CAA4(&lbl_1_bss_1214[1]);
    lbl_1_bss_1220 = Hu3DHookFuncCreate(fn_1_BFA0);
    Hu3DModelCameraSet(lbl_1_bss_1220, 1);
    Hu3DModelLayerSet(lbl_1_bss_1220, 5);
    lbl_1_bss_1224.x = 12000.0f;
    lbl_1_bss_1224.y = 3000.0f;
    lbl_1_bss_1224.z = 2400.0f;
    HuDataDirClose(DATANUM(DATA_capsuleshop, 0));
}
/* @@PART4@@ */
HuVecF lbl_1_data_644 = { -20.0f, 0.0f, 0.0f };
HuVecF lbl_1_data_650 = { 0.0f, 300.0f, 0.0f };
GXColor lbl_1_data_65C[3] = { { 0xFF, 0xD3, 0xE5, 0xFF }, { 0xBF, 0xE5, 0xFF, 0xFF }, { 0xFF, 0xF3, 0x9B, 0xFF } };
GXColor lbl_1_data_668[3] = { { 0x80, 0x20, 0x20, 0xFF }, { 0x20, 0x20, 0x80, 0xFF }, { 0x80, 0x80, 0x20, 0xFF } };


void fn_1_99E4(void)
{
    W01_BRANCH *tree = lbl_1_bss_1240;
    int i;

    if (lbl_1_bss_121C) {
        HuSprAnimKill(lbl_1_bss_121C);
        lbl_1_bss_121C = NULL;
    }
    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_1214[i]) {
            W01Free(lbl_1_bss_1214[i]);
            lbl_1_bss_1214[i] = NULL;
        }
    }
    for (i = 0; i < 3; i++, tree++) {
        W01Free(tree->leaf);
        tree->leaf = NULL;
    }
}

void fn_1_9AEC(int playerNo, s16 masuId)
{
    float t;
    float t2;
    float t3;
    float rotY;
    s32 i;
    W01_BRANCH *tree;
    MBOBJID *objs;
    s16 winId;
    s32 choice;
    s32 j;
    s32 k;
    s16 masuNext;
    s32 rand1;
    s32 rand2;
    s32 slot;
    s16 masuLink;
    s16 helpWin;
    s16 motId;
    s32 stepNum;
    s32 swap;
    s32 time;
    HuVecF masuPos;
    HuVecF treePos;
    HuVecF playerPos;
    HuVecF newPos;
    HuVecF dir;
    HuVecF jumpDir;
    HuVecF jumpPos;
    HuVecF jumpStart;
    s16 order[4];

    tree = lbl_1_bss_1240;
    objs = &lbl_1_bss_1238[0];
    mbObjMotionSpeedSet(objs[0], 1.0f);
    {
        MBOBJID id = objs[0];
        mbObjAttrReset(id, 0x40000002);
    }
    mbObjDispSet(objs[2], TRUE);
    mbObjMotionSet(objs[2], 1, 0x40000001);
    mbPlayerRotateStart(playerNo, 180, 15);
    masuLink = mbMasuAttrFindLink(masuId, 0x2000);
    mbMasuPosGet(masuLink, &masuPos);
    if (!GwSystem.curTime) {
        winId = mbWinCreate(2, 0x2F0004, 8);
        mbAudFXDelaySet(30);
        mbAudFXPlay(0x3DB);
    } else {
        winId = mbWinCreate(2, 0x2F0009, 9);
        mbAudFXDelaySet(30);
        mbAudFXPlay(0x3C1);
    }
    mbWinWait(winId);
    if (!GwSystem.curTime) {
        winId = mbWinCreateChoice(2, 0x2F0005, 8, 0);
    } else {
        winId = mbWinCreateChoice(2, 0x2F000A, 9, 0);
    }
    mbWinChoiceGet(winId);
    if (GwPlayer[playerNo].comF) {
        stepNum = mbMasuFind_IdStepGet(masuId, lbl_1_bss_1616);
        if (stepNum > 13 && frandf() < 0.8f) {
            mbComChoiceLeftSet();
        } else {
            mbComChoiceRightSet();
        }
    }
    mbWinWait(winId);
    choice = mbWinChoiceGet(winId);
    if (choice < 0 || choice == 1) {
        mbObjMotionSpeedSet(objs[0], -1.0f);
        while (mbObjMotionTimeGet(objs[0]) != 0.0f) {
            HuPrcVSleep();
        }
        mbObjDispSet(objs[2], FALSE);
        return;
    }
    mbCameraMoveMasu(masuLink, &lbl_1_data_644, &lbl_1_data_650, 2638.0f, -1.0f, 30);
    mbPlayerPosGet(playerNo, &playerPos);
    VECSubtract(&masuPos, &playerPos, &dir);
    dir.z += 100.0f;
    mbPlayerMotionShiftSet(playerNo, 3, 0.0f, 2.0f, 0);
    for (i = 0; i <= 30u; i++) {
        t = i / 30.0f;
        newPos.x = playerPos.x + dir.x * t;
        newPos.y = playerPos.y + dir.y * t;
        newPos.z = playerPos.z + dir.z * t;
        mbPlayerPosSetV(playerNo, &newPos);
        HuPrcVSleep();
    }
    mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 2.0f, 0x40000001);
    mbCameraMoveWait();
    mbObjMotionShiftSet(objs[2], 2, 0.0f, 4.0f, 0);
    if (!GwSystem.curTime) {
        winId = mbWinCreate(2, 0x2F0006, 8);
    } else {
        winId = mbWinCreate(2, 0x2F000B, 9);
    }
    for (i = 0; i < 3; i++, tree++) {
        fn_1_B7D8(i);
        mbObjDispSet(tree->objId, TRUE);
        Hu3DModelAttrReset(lbl_1_bss_1220, 1);
    }
    for (i = 0; i < 60u; i++) {
        t = i / 60.0f;
        for (tree = lbl_1_bss_1240, k = 0; k < 3; k++, tree++) {
            tree->pos.x = masuPos.x + 100.0f * (5.0f * (k - 1));
            tree->pos.y = 1000.0f + (1000.0f + masuPos.y - 100.0f * (7.0f * mbSinDeg(90.0f * t)));
            tree->pos.z = masuPos.z;
            mbObjPosSet(tree->objId, tree->pos.x, tree->pos.y - 1000.0f, tree->pos.z);
        }
        HuPrcVSleep();
    }
    mbWinWait(winId);
    motId = mbPlayerMotionCreate(playerNo, 0x930063);
    helpWin = mbWinCreateHelp(0x2F0008);
    mbWinPosSet(helpWin, 216, 384);
    for (i = 0; i < 4; i++) {
        order[i] = i;
    }
    for (i = 0; i < 100; i++) {
        rand1 = mbRandMod(4);
        rand2 = mbRandMod(4);
        swap = order[rand1];
        order[rand1] = order[rand2];
        order[rand2] = swap;
    }
    choice = fn_1_A82C(playerNo);
    tree = &lbl_1_bss_1240[choice];
    mbPlayerPosGet(playerNo, &playerPos);
    mbObjPosGet(tree->objId, &treePos);
    VECSubtract(&treePos, &playerPos, &dir);
    mbPlayerMotionShiftSet(playerNo, 4, 0.0f, 2.0f, 0);
    for (i = 0; i < 30u; i++) {
        t = i / 30.0f;
        if (i == 24u) {
            mbPlayerMotionShiftSet(playerNo, motId, 0.0f, 2.0f, 0x40000001);
        }
        newPos.x = playerPos.x + dir.x * t;
        newPos.y = playerPos.y + dir.y * t + 100.0f * (1.5f * mbSinDeg(180.0f * t));
        newPos.z = playerPos.z + dir.z * t;
        mbPlayerPosSetV(playerNo, &newPos);
        HuPrcVSleep();
    }
    omVibrate(playerNo, 20, 4, 4);
    fn_1_AFD4(choice, playerNo);
    mbWinKill(helpWin);
    if (!GwSystem.curTime) {
        winId = mbWinCreate(2, 0x2F0007, 8);
    } else {
        winId = mbWinCreate(2, 0x2F0007, 9);
    }
    mbWinWait(winId);
    mbObjMotionShiftSet(objs[2], 3, 0.0f, 4.0f, 0x40000001);
    for (i = 0; i < 3; i++) {
        if (i != choice) {
            fn_1_B97C(i);
            fn_1_B3F8(i);
        } else {
            fn_1_B9A8(i, 480);
        }
    }
    HuPrcSleep(30);
    slot = order[choice];
    masuNext = lbl_1_bss_1230[slot];
    GwPlayer[playerNo].masuIdNext = masuNext;
    lbl_1_data_644.x = -35.0f;
    lbl_1_data_644.y = lbl_1_data_644.z = 0.0f;
    mbCameraMovePlayer(playerNo, &lbl_1_data_644, NULL, 2400.0f, -1.0f, 60);
    fn_1_B01C(choice, slot);
    fn_1_B3F8(choice);
    mbPlayerPosGet(playerNo, &playerPos);
    mbMasuPosGet(masuNext, &newPos);
    fn_1_AFD4(choice, -1);
    mbPlayerMotionShiftSet(playerNo, 4, 0.0f, 8.0f, 0);
    mbPlayerPosGet(playerNo, &jumpStart);
    rotY = mbPlayerRotYGet(playerNo);
    VECSubtract(&newPos, &jumpStart, &jumpDir);
    for (j = 0; j <= 30; j++) {
        t2 = j / 30.0f;
        if (j == 24u) {
            mbPlayerMotionShiftSet(playerNo, 5, 0.0f, 2.0f, 0);
        }
        jumpPos.x = jumpStart.x + t2 * jumpDir.x;
        jumpPos.y = jumpStart.y + t2 * jumpDir.y + 100.0 * HuSin(180.0f * t2);
        jumpPos.z = jumpStart.z + t2 * jumpDir.z;
        mbPlayerPosSetV(playerNo, &jumpPos);
        t3 = j / 6.0f;
        if (t3 > 1.0f) {
            t3 = 1.0f;
        }
        mbPlayerRotYSet(playerNo, mbAngleLerp(rotY, 0.0f, t3));
        time = 30 - j;
        mbPlayerWorkGet(playerNo)->masuNext = time;
        HuPrcVSleep();
    }
    mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 2.0f, 0x40000001);
    omVibrate(playerNo, 20, 4, 4);
    HuPrcSleep(30);
    mbPlayerMotionShiftSet(playerNo, 12, 0.0f, 5.0f, 0);
    mbPlayerWinLoseVoicePlay(playerNo, 12, 0x243);
    mbPlayerMotionEndWait(playerNo);
    mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 5.0f, 0x40000001);
    mbPlayerMotionKill(playerNo, motId);
    tree = &lbl_1_bss_1240[0];
    for (i = 0; i < 3; i++, tree++) {
        mbObjDispSet(tree->objId, FALSE);
        Hu3DModelAttrSet(lbl_1_bss_1220, 1);
    }
    GwPlayer[playerNo].masuId = masuNext;
    mbObjAttrSet(objs[0], 0x40000002);
    mbObjMotionTimeSet(objs[0], 0.0f);
    mbObjDispSet(objs[2], FALSE);
}

int fn_1_A82C(int playerNo)
{
    float startX;
    float rot;
    float t;
    float rotY;
    float x;
    float endX;
    u16 btn;
    s32 next;
    s32 cur;
    GAMEMESID mesId;
    s32 timer;
    s32 comWait;
    s16 moveTime;
    s32 moveF;
    s8 padNo;
    s16 moveMax;
    HuVecF pos;

    mbPlayerPosGet(playerNo, &pos);
    comWait = 60.0f + 60.0f * frandf();
    moveF = 0;
    cur = next = 1;
    padNo = GwPlayer[playerNo].padNo;
    mesId = GameMesCreate(1, 10, -1, -1);
    HuSprGrpDrawNoSet(GameMesGet(mesId)->grpId[0], 32);
    timer = 600;
    while (1) {
        if (!moveF) {
            if (GwPlayer[playerNo].comF) {
                if (comWait == 0) {
                    btn = PAD_BUTTON_A;
                } else if (frandf() < 0.05f) {
                    if (frandf() < 0.5f) {
                        btn = PAD_BUTTON_LEFT;
                    } else {
                        btn = PAD_BUTTON_RIGHT;
                    }
                }
            } else {
                btn = HuPadBtnDown[padNo];
                if (mbPadStkXGet(padNo) < -20) {
                    btn |= PAD_BUTTON_LEFT;
                } else if (mbPadStkXGet(padNo) > 20) {
                    btn |= PAD_BUTTON_RIGHT;
                }
            }
            if (timer < 0 || (btn & PAD_BUTTON_A)) {
                break;
            }
            if (cur > 0 && (btn & PAD_BUTTON_LEFT)) {
                next--;
            }
            if (cur < 2 && (btn & PAD_BUTTON_RIGHT)) {
                next++;
            }
            if (cur != next) {
                moveF = 1;
                moveTime = 0;
                moveMax = 30;
                rotY = (cur < next) ? 90.0f : -90.0f;
                mbPlayerRotYSet(playerNo, rotY);
                mbPlayerMotionShiftSet(playerNo, 3, 0.0f, 4.0f, 0x40000001);
                startX = pos.x + 100.0f * (5.0f * (cur - 1));
                endX = pos.x + 100.0f * (5.0f * (next - 1));
            }
        } else {
            t = (float)moveTime++ / moveMax;
            x = startX + t * (endX - startX);
            mbPlayerPosSet(playerNo, x, pos.y, pos.z);
            if (moveTime > moveMax) {
                cur = next;
                mbPlayerRotateStart(playerNo, 180, 15);
                while (!mbPlayerRotateCheck(playerNo)) {
                    HuPrcVSleep();
                }
                moveF = 0;
            }
        }
        if (comWait != 0) {
            comWait--;
        }
        if (timer >= 0) {
            timer--;
            if (timer >= 0) {
                GameMesDispSet(mesId, 1, (timer + 59) / 60);
            } else {
                GameMesDispSet(mesId, 2, -1);
                mesId = -1;
            }
        }
        HuPrcVSleep();
    }
    mbAudFXPlay(1);
    if (mesId >= 0) {
        GameMesDispSet(mesId, 2, -1);
        mesId = -1;
    }
    return cur;
}

void fn_1_ACDC(OMOBJ *obj)
{
    float accel;
    float speed;
    float dy;
    float t;
    W01_BRANCH *tree;
    HuVecF pos;

    tree = &lbl_1_bss_1240[obj->work[0]];
    switch (tree->fadeF) {
        case 1:
            t = (float)tree->fadeTime++ / tree->fadeMax;
            mbObjAlphaSet(tree->objId, (s32)(255.0f * (1.0f - t)));
            if (tree->fadeTime > tree->fadeMax) {
                tree->fadeF = 0;
            }
            break;
    }
    if (tree->moveF) {
        pos.x = tree->pos.x;
        pos.y = tree->pos.y - 1000.0f;
        pos.z = tree->pos.z;
    } else {
        speed = 5.0f + 5.0f * (1.0f + mbSinDeg(obj->work[1]));
        obj->work[1] += 2;
        if (tree->playerNo >= 0) {
            speed += 50.0f;
        }
        mbObjPosGet(tree->objId, &pos);
        pos.x = tree->pos.x;
        pos.z = tree->pos.z;
        accel = -980.0f;
        dy = tree->pos.y - pos.y;
        accel += (500.0f * (dy - 1000.0f)) / speed;
        tree->vel.y += (1.0f / 60.0f) * accel;
        pos.y += (1.0f / 60.0f) * tree->vel.y;
        tree->vel.y *= 0.9f;
    }
    if (tree->playerNo >= 0) {
        mbPlayerPosSetV(tree->playerNo, &pos);
    }
    mbObjPosSetV(tree->objId, &pos);
    pos.y += 356.0f;
    fn_1_BB44(tree, &pos);
}

void fn_1_AFD4(int no, int playerNo)
{
    W01_BRANCH *tree = &lbl_1_bss_1240[no];

    if (playerNo >= 0) {
        tree->vel.y += -833.3334f;
    }
    tree->playerNo = playerNo;
}

void fn_1_B01C(int no, int pathNo)
{
    float t;
    float accel;
    float speed;
    W01_BRANCH *tree;
    HuVecF *work;
    s32 k;
    s32 i;
    W01_BRANCHLEAF *leaf;
    HuVecF *path;
    s32 j;
    s32 num;
    s32 size;
    HuVecF *freeP;
    HuVecF out;
    HuVecF pos;

    tree = &lbl_1_bss_1240[no];
    mbObjPosGet(tree->objId, &pos);
    size = lbl_1_data_5C4[pathNo] * sizeof(HuVecF);
    path = W01Malloc(size);
    memcpy(path, lbl_1_data_5B4[pathNo], size);
    path[0] = pos;
    tree->moveF = 1;
    tree->pos.x = pos.x;
    tree->pos.y = 1000.0f + pos.y;
    tree->pos.z = pos.z;
    accel = 0.000019841269f;
    speed = 120.0f * accel;
    for (i = 0; i < 480u; i++) {
        if (i < 120u) {
            t = 0.5f * accel * (i * i);
        } else {
            t = 120.0f * (120.0f * (0.5f * accel)) + speed * (i - 120.0f);
        }
        num = lbl_1_data_5C4[pathNo];
        work = W01Malloc(num * sizeof(HuVecF));
        memcpy(work, path, num * sizeof(HuVecF));
        for (j = 1; j < num; j++) {
            for (k = 0; k < num - j; k++) {
                work[k].x = work[k].x + t * (work[k + 1].x - work[k].x);
                work[k].y = work[k].y + t * (work[k + 1].y - work[k].y);
                work[k].z = work[k].z + t * (work[k + 1].z - work[k].z);
            }
        }
        out = work[0];
        freeP = work;
        HuMemDirectFree(freeP);
        tree->pos.x = out.x;
        tree->pos.y = 1000.0f + out.y;
        tree->pos.z = out.z;
        HuPrcVSleep();
    }
    tree->vel.x = tree->vel.y = tree->vel.z = 0.0f;
    tree->pos.y += 100.0f;
    tree->moveF = 0;
    leaf = tree->leaf;
    for (i = 0; i < tree->leafNum; i++, leaf++) {
        if (leaf->time < 0) {
            leaf->time = 1;
        }
    }
    HuMemDirectFree(path);
}

void fn_1_B3F8(int no)
{
    W01_BRANCH *tree = &lbl_1_bss_1240[no];

    tree->fadeF = 1;
    tree->fadeTime = 0;
    tree->fadeMax = 30;
}

int fn_1_B434(W01_BRANCHLEAF **leafP)
{
    W01_BRANCHLEAF *leaf;
    s32 i;
    s32 num;
    s32 count;
    HuVecF vtx[1024];
    Mtx mtx;
    HuVecF dir;

    num = fn_1_D1D0(vtx, 1.0f);
    for (i = 0; i < 2; i++) {
        num = fn_1_CCEC(vtx, num, 1.0f, vtx);
    }
    count = fn_1_D320(vtx, num * 3, vtx);
    leaf = mbMalloc(count * sizeof(W01_BRANCHLEAF));
    *leafP = leaf;
    mbMtxRot(mtx, 45.0f, 45.0f, 0.0f);
    for (i = 0; i < count; i++, leaf++) {
        leaf->pos.x = leaf->pos.y = leaf->pos.z = 0.0f;
        leaf->vel.x = leaf->vel.y = leaf->vel.z = 0.0f;
        MTXMultVec(mtx, &vtx[i], &dir);
        leaf->rot.x = HuAtan(dir.y, sqrtf(dir.z * dir.z + dir.x * dir.x)) - 90.0;
        leaf->rot.y = HuAtan(-dir.x, -dir.z);
        leaf->rot.z = 0.0f;
        leaf->rotBase = leaf->rot;
        leaf->windScale.x = 0.5f + 0.5f * frandf();
        leaf->windScale.y = 0.5f + 0.5f * frandf();
        leaf->windScale.z = 0.5f + 0.5f * frandf();
        leaf->time = mbRandMod(60) + 1;
        leaf->angle = 0.017453292f * leaf->rot.x;
        leaf->angVel = 0.0f;
    }
    return count;
}

void fn_1_B7D8(int no)
{
    W01_BRANCHLEAF *leaf;
    W01_BRANCH *tree;
    s32 i;

    tree = &lbl_1_bss_1240[no];
    leaf = tree->leaf;
    for (i = 0; i < tree->leafNum; i++, leaf++) {
        leaf->pos.x = leaf->pos.y = leaf->pos.z = 0.0f;
        leaf->vel.x = leaf->vel.y = leaf->vel.z = 0.0f;
        leaf->rot = leaf->rotBase;
        leaf->windScale.x = 0.8f + 0.2f * frandf();
        leaf->windScale.y = 0.8f + 0.2f * frandf();
        leaf->windScale.z = 0.8f + 0.2f * frandf();
        leaf->time = mbRandMod(60) + 1;
        leaf->angle = 0.017453292f * leaf->rot.x;
        leaf->angVel = 0.0f;
    }
    tree->attachF = 1;
    tree->playerNo = -1;
    tree->vel.x = tree->vel.y = tree->vel.z = 0.0f;
    tree->moveF = 0;
    mbObjAlphaSet(tree->objId, 255);
}

void fn_1_B97C(int no)
{
    W01_BRANCH *tree = &lbl_1_bss_1240[no];

    tree->attachF = 0;
}

void fn_1_B9A8(int no, int time)
{
    s32 i;
    W01_BRANCH *tree;
    s32 *order;
    W01_BRANCHLEAF *leaf;
    s32 num;
    s32 a;
    s32 b;
    s32 swap;

    tree = &lbl_1_bss_1240[no];
    order = W01Malloc(tree->leafNum * sizeof(s32));
    num = 0.6f * tree->leafNum;
    for (i = 0; i < tree->leafNum; i++) {
        order[i] = i;
    }
    for (i = 0; i < 100; i++) {
        a = mbRandMod(tree->leafNum);
        b = mbRandMod(tree->leafNum);
        swap = order[a];
        order[a] = order[b];
        order[b] = swap;
    }
    for (i = 0; i < num; i++) {
        leaf = &tree->leaf[order[i]];
        leaf->time = ((i * time) / num) + 60;
    }
    for (; i < tree->leafNum; i++) {
        leaf = &tree->leaf[order[i]];
        leaf->time = -1;
    }
    HuMemDirectFree(order);
    tree->attachF = 0;
}

void fn_1_BB44(W01_BRANCH *tree, HuVecF *pos)
{
    float accel;
    float scale;
    W01_BRANCHLEAF *leaf;
    s32 i;
    Mtx mtx;
    HuVecF ofs;
    HuVecF leafOfs;
    HuVecF dir;

    leaf = tree->leaf;
    ofs.x = 0.0f;
    ofs.y = 70.0f;
    ofs.z = 0.0f;
    if (tree->attachF) {
        for (i = 0; i < tree->leafNum; i++, leaf++) {
            mbMtxRot(mtx, leaf->rot.x, leaf->rot.y, leaf->rot.z);
            MTXMultVec(mtx, &ofs, &leafOfs);
            VECAdd(pos, &leafOfs, &leaf->pos);
        }
    } else {
        for (i = 0; i < tree->leafNum; i++, leaf++) {
            if (leaf->time != 0) {
                mbMtxRot(mtx, leaf->rot.x, leaf->rot.y, leaf->rot.z);
                MTXMultVec(mtx, &ofs, &leafOfs);
                VECAdd(pos, &leafOfs, &leaf->pos);
                if (leaf->time > 0) {
                    leaf->time--;
                }
            } else {
                accel = (-980.0f * mbSinRad(leaf->angle)) / 70.0f;
                leaf->angVel += (1.0f / 60.0f) * accel;
                leaf->angle += (1.0f / 60.0f) * leaf->angVel;
                leaf->rot.x = 57.29578f * leaf->angle;
                dir.x = 0.0f;
                dir.y = -1.0f;
                dir.z = 0.0f;
                mbMtxRot(mtx, leaf->rot.x, leaf->rot.y, leaf->rot.z);
                MTXMultVec(mtx, &dir, &dir);
                scale = 70.0f * (leaf->angVel * leaf->angVel);
                VECScale(&dir, &dir, scale);
                leaf->vel.x += (1.0f / 60.0f) * lbl_1_bss_1224.x * leaf->windScale.x + (1.0f / 60.0f) * (dir.x - 10.0f * leaf->vel.x);
                leaf->vel.y += (1.0f / 60.0f) * lbl_1_bss_1224.y * leaf->windScale.y + (1.0f / 60.0f) * (-980.0f + dir.y - 10.0f * leaf->vel.y);
                leaf->vel.z += (1.0f / 60.0f) * lbl_1_bss_1224.z * leaf->windScale.z + (1.0f / 60.0f) * (dir.z - 10.0f * leaf->vel.z);
                leaf->pos.x += (1.0f / 60.0f) * leaf->vel.x;
                leaf->pos.y += (1.0f / 60.0f) * leaf->vel.y;
                leaf->pos.z += (1.0f / 60.0f) * leaf->vel.z;
                leaf->angVel *= 0.99f;
                leaf->vel.x *= 0.95f;
                leaf->vel.y *= 0.95f;
                leaf->vel.z *= 0.95f;
            }
        }
    }
}

const HuVecF lbl_1_rodata_288[11] = {
    { 0.0f, -100.0f, 0.0f },
    { 0.0f, -60.0f, 0.0f },
    { 5.0f, -80.0f, -4.33012f },
    { 0.0f, -80.0f, 4.33012f },
    { -5.0f, -80.0f, -4.33012f },
    { 1.0f, -65.0f, -0.86602f },
    { 0.0f, -65.0f, 0.86602f },
    { -1.0f, -65.0f, -0.86602f },
    { 1.0f, 0.0f, -0.86602f },
    { 0.0f, 0.0f, 0.86602f },
    { -1.0f, 0.0f, -0.86602f },
};
const s32 lbl_1_rodata_30C[15] = { GX_TRIANGLEFAN, 5, 0, 2, 4, 3, 2, GX_TRIANGLEFAN, 5, 1, 4, 2, 3, 4, -1 };
const s32 lbl_1_rodata_348[19] = { GX_QUADS, 4, 5, 6, 9, 8, GX_QUADS, 4, 6, 7, 10, 9, GX_QUADS, 4, 7, 5, 8, 10, -1 };

void fn_1_BFA0(HU3D_MODEL *modelP, Mtx *mtx)
{
    float t;
    float depth;
    s32 i;
    W01_BRANCHLEAF *leaf;
    s32 num;
    W01_BRANCH *tree;
    s32 j;
    GXColor *colP;
    GXColor col;
    Mtx m;
    W01ZSORT draw[1024];

    tree = lbl_1_bss_1240;
    if (mbObjDispGet(tree->objId)) {
        HuSprTexLoad(lbl_1_bss_121C, 0, GX_TEXMAP0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_1);
        GXSetColorUpdate(GX_TRUE);
        GXSetAlphaUpdate(GX_FALSE);
        GXSetZCompLoc(GX_FALSE);
        GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
        GXSetCullMode(GX_CULL_BACK);
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        num = 0;
        for (i = 0; i < 3; i++, tree++) {
            leaf = tree->leaf;
            for (j = 0; j < tree->leafNum; j++, leaf++) {
                mbMtxRot(m, leaf->rot.x, leaf->rot.y, leaf->rot.z);
                mtxTransCat(m, leaf->pos.x, leaf->pos.y, leaf->pos.z);
                MTXConcat(*mtx, m, m);
                draw[num].pos.x = m[0][3];
                draw[num].pos.y = m[1][3];
                draw[num].pos.z = m[2][3];
                draw[num].colNo = i;
                draw[num].depth = m[2][1];
                num++;
                GXLoadPosMtxImm(m, GX_PNMTX0);
                GXCallDisplayList(lbl_1_bss_1214[0], lbl_1_bss_120C[0]);
            }
        }
        fn_1_D50C(draw, 0, num - 1);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
        if (!GwSystem.curTime) {
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
        } else {
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
        }
        for (i = 0; i < num; i++) {
            if (!GwSystem.curTime) {
                col = lbl_1_data_65C[draw[i].colNo];
                t = fabs(draw[i].depth);
                col.r = 255.0f * (1.0f - t) + col.r * t;
                col.g = 255.0f * (1.0f - t) + col.g * t;
                col.b = 255.0f * (1.0f - t) + col.b * t;
            } else {
                col = lbl_1_data_668[draw[i].colNo];
            }
            GXSetTevColor(GX_TEVREG0, col);
            MTXTrans(m, draw[i].pos.x, draw[i].pos.y, draw[i].pos.z);
            GXLoadPosMtxImm(m, GX_PNMTX0);
            GXCallDisplayList(lbl_1_bss_1214[1], lbl_1_bss_120C[1]);
        }
    }
}

void fn_1_C694(const s32 *data, u32 color)
{
    float x;
    float y;
    float z;
    s32 i;
    s32 idx;
    s32 num;

    while (*data != -1) {
        num = data[1];
        GXBegin(data[0], GX_VTXFMT0, num);
        data += 2;
        for (i = 0; i < num; i++) {
            idx = *data++;
            GXPosition3f32(0.7f * lbl_1_rodata_288[idx].x, 0.7f * lbl_1_rodata_288[idx].y, 0.7f * lbl_1_rodata_288[idx].z);
            GXColor1u32(color);
        }
    }
}

s32 fn_1_C7DC(void **dlP)
{
    void *buf;
    s32 size;
    u32 color1 = 0x685747FF;
    u32 color2 = 0xBEB095FF;

    buf = W01Malloc(0x1000);
    DCInvalidateRange(buf, 0x1000);
    GXBeginDisplayList(buf, 0x1000);
    fn_1_C694(lbl_1_rodata_30C, color1);
    fn_1_C694(lbl_1_rodata_348, color2);
    size = GXEndDisplayList();
    *dlP = W01Malloc(size);
    memcpy(*dlP, buf, size);
    DCFlushRange(*dlP, size);
    HuMemDirectFree(buf);
    return size;
}

s32 fn_1_CAA4(void **dlP)
{
    void *buf;
    s32 size;

    buf = W01Malloc(0x1000);
    DCInvalidateRange(buf, 0x1000);
    GXBeginDisplayList(buf, 0x1000);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-35.0f, 35.0f, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(35.0f, 35.0f, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(35.0f, -35.0f, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(-35.0f, -35.0f, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    size = GXEndDisplayList();
    *dlP = W01Malloc(size);
    memcpy(*dlP, buf, size);
    DCFlushRange(*dlP, size);
    HuMemDirectFree(buf);
    return size;
}
/* @@PART5@@ */
HuVecF lbl_1_data_674[4] = {
    { 0.0f, 0.0f, 0.61237246f },
    { 0.57735026f, 0.0f, -0.20412414f },
    { -0.28867513f, 0.5f, -0.20412414f },
    { -0.28867513f, -0.5f, -0.20412414f }
};
s32 lbl_1_data_6A4[12] = { 0, 2, 1, 0, 3, 2, 0, 1, 3, 1, 2, 3 };
HuVecF lbl_1_data_6D4[6] = {
    { 0.0f, 1.0f, 0.0f },
    { 0.0f, -1.0f, 0.0f },
    { -1.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, -1.0f },
    { 1.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f }
};
s32 lbl_1_data_71C[25] = { 0, 5, 2, 0, 4, 5, 0, 3, 4, 0, 2, 3, 1, 2, 5, 1, 5, 4, 1, 4, 3, 1, 3, 2, -1 };
HuVecF lbl_1_data_780 = { 0.0f, 0.0f, 20.0f };

s32 fn_1_CCEC(HuVecF *src, s32 triNum, float radius, HuVecF *dst)
{
    HuVecF *buf;
    HuVecF *tri;
    HuVecF *bufStart;
    s32 size;
    s32 i;
    s32 j;
    s32 k;
    s32 num;
    HuVecF dir;
    HuVecF mid[3];

    num = 0;
    size = triNum * 3 * sizeof(HuVecF);
    buf = HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
    tri = buf;
    bufStart = tri;
    memcpy(bufStart, src, size);
    for (i = 0; i < triNum; i++, tri += 3) {
        for (j = 0; j < 3; j++) {
            k = j + 1;
            if (k >= 3) {
                k = 0;
            }
            VECSubtract(&tri[k], &tri[j], &dir);
            mid[j].x = (0.5f * dir.x) + tri[j].x;
            mid[j].y = (0.5f * dir.y) + tri[j].y;
            mid[j].z = (0.5f * dir.z) + tri[j].z;
            VECNormalize(&mid[j], &mid[j]);
            VECScale(&mid[j], &mid[j], radius);
        }
        dst[num++] = tri[0];
        dst[num++] = mid[0];
        dst[num++] = mid[2];
        dst[num++] = mid[0];
        dst[num++] = tri[1];
        dst[num++] = mid[1];
        dst[num++] = mid[2];
        dst[num++] = mid[1];
        dst[num++] = tri[2];
        dst[num++] = mid[0];
        dst[num++] = mid[1];
        dst[num++] = mid[2];
    }
    HuMemDirectFree(bufStart);
    return num / 3;
}

s32 fn_1_D080(HuVecF *dst, float radius)
{
    s32 i;

    for (i = 0; i < 4; i++, dst += 3) {
        dst[0] = lbl_1_data_674[lbl_1_data_6A4[i * 3]];
        dst[1] = lbl_1_data_674[lbl_1_data_6A4[i * 3 + 1]];
        dst[2] = lbl_1_data_674[lbl_1_data_6A4[i * 3 + 2]];
        VECScale(&dst[0], &dst[0], radius);
        VECScale(&dst[1], &dst[1], radius);
        VECScale(&dst[2], &dst[2], radius);
    }
    return 4;
}

s32 fn_1_D1D0(HuVecF *dst, float radius)
{
    s32 i;

    for (i = 0; i < 8; i++, dst += 3) {
        dst[0] = lbl_1_data_6D4[lbl_1_data_71C[i * 3]];
        dst[1] = lbl_1_data_6D4[lbl_1_data_71C[i * 3 + 1]];
        dst[2] = lbl_1_data_6D4[lbl_1_data_71C[i * 3 + 2]];
        VECScale(&dst[0], &dst[0], radius);
        VECScale(&dst[1], &dst[1], radius);
        VECScale(&dst[2], &dst[2], radius);
    }
    return 8;
}

s32 fn_1_D320(HuVecF *src, s32 num, HuVecF *dst)
{
    float eps;
    u8 *dupF;
    s32 i;
    s32 j;
    s32 count;
    HuVecF dir;

    eps = 0.0001f;
    dupF = mbMalloc(num);
    for (i = 0; i < num - 1; i++) {
        if (dupF[i]) {
            continue;
        }
        for (j = i + 1; j < num; j++) {
            if (dupF[j]) {
                continue;
            }
            VECSubtract(&src[j], &src[i], &dir);
            if (fabs(dir.x) < eps && fabs(dir.y) < eps && fabs(dir.z) < eps) {
                dupF[j] = TRUE;
            }
        }
    }
    count = 0;
    for (i = 0; i < num; i++) {
        if (dupF[i]) {
            continue;
        }
        *dst++ = src[i];
        count++;
    }
    HuMemDirectFree(dupF);
    return count;
}

void fn_1_D50C(W01ZSORT *list, s32 left, s32 right)
{
    s32 i;
    s32 j;
    float pivot;
    W01ZSORT temp;

    pivot = list[(left + right) / 2].pos.z;
    i = left;
    j = right;
    while (1) {
        while (list[i].pos.z < pivot) {
            i++;
        }
        while (pivot < list[j].pos.z) {
            j--;
        }
        if (i >= j) {
            break;
        }
        temp = list[i];
        list[i] = list[j];
        list[j] = temp;
        i++;
        j--;
    }
    if (left < i - 1) {
        fn_1_D50C(list, left, i - 1);
    }
    if (j + 1 < right) {
        fn_1_D50C(list, j + 1, right);
    }
}

static inline void W01CurveSetup(HuVecF *posTbl, int no, int num, HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d)
{
    HuVecF *p;
    HuVecF tanA;
    HuVecF tanB;

    if (no > num - 1) {
        no = num - 1;
    }
    p = &posTbl[no];
    if (no == 0) {
        VECSubtract(&p[1], &p[0], &tanA);
    } else {
        VECSubtract(&p[1], &p[-1], &tanA);
    }
    if (no == num - 2) {
        VECSubtract(&p[1], &p[0], &tanB);
    } else {
        VECSubtract(&p[2], &p[0], &tanB);
    }
    VECScale(&tanA, &tanA, 0.5f);
    VECScale(&tanB, &tanB, 0.5f);
    *a = p[0];
    *b = p[1];
    *c = tanA;
    *d = tanB;
}

void fn_1_DEA4(void)
{
    HuVecF a;
    HuVecF b;
    HuVecF c;
    HuVecF d;
    float len;
    u32 i;

    lbl_1_bss_1208 = 0;
    for (i = 0; i < 32; i++) {
        W01CurveSetup(lbl_1_data_240, i, 33, &a, &b, &c, &d);
        len = W01PathLenGet(fn_1_14BF0, &a, &b, &c, &d, 1);
        lbl_1_bss_1188[i] = len;
        lbl_1_bss_1208 += len;
    }
}

static inline void W01PlayerMasuNextSet(int playerNo, s16 num)
{
    mbPlayerWorkGet(playerNo)->masuNext = num;
}

static inline void W01PlayerJump(int playerNo, HuVecF *target, float angle)
{
    HuVecF startPos;
    HuVecF jumpPos;
    HuVecF delta;
    float rotY;
    float jumpT;
    float rotT;
    s32 j;

    mbPlayerPosGet(playerNo, &startPos);
    rotY = mbPlayerRotYGet(playerNo);
    VECSubtract(target, &startPos, &delta);
    for (j = 0; j <= 18; j++) {
        jumpT = j / 18.0f;
        if (j == 12u) {
            mbPlayerMotionShiftSet(playerNo, 5, 0, 2, 0);
        }
        jumpPos.x = startPos.x + (jumpT * delta.x);
        jumpPos.y = startPos.y + (jumpT * delta.y) + (100.0 * HuSin(180.0f * jumpT));
        jumpPos.z = startPos.z + (jumpT * delta.z);
        mbPlayerPosSetV(playerNo, &jumpPos);
        rotT = j / 6.0f;
        if (rotT > 1.0f) {
            rotT = 1.0f;
        }
        mbPlayerRotYSet(playerNo, mbAngleLerp(rotY, angle, rotT));
        W01PlayerMasuNextSet(playerNo, 18 - j);
        HuPrcVSleep();
    }
}

void fn_1_E234(int playerNo)
{
    HuVecF a;
    HuVecF b;
    HuVecF pos;
    HuVecF masuPos;
    HuVecF endPos;
    HuVecF playerPos;
    HuVecF c;
    HuVecF d;
    HuVecF dir;
    HuVecF rot;
    float t;
    float dist;
    float speed;
    float angle;
    s32 seg;
    u32 i;
    s16 masuId;
    s16 nextMasuId;
    s16 motId;

    seg = 0;
    omVibrate(playerNo, 20, 7, 3);
    GwPlayer[playerNo].moveF = TRUE;
    W01PlayerMasuNextSet(playerNo, 100);
    mbPlayerColSnapPlayerSet(playerNo, FALSE);
    motId = mbPlayerMotionCreate(playerNo, 0x93000A);
    mbPlayerMotionShiftSet(playerNo, motId, 0, 8, 0);
    masuId = GwPlayer[playerNo].masuId;
    nextMasuId = mbMasuLinkGet(masuId, 0);
    GwPlayer[playerNo].masuIdNext = nextMasuId;
    W01CurveSetup(lbl_1_data_240, 0, 33, &a, &b, &c, &d);
    t = 0;
    dist = 0;
    speed = lbl_1_bss_1208 / 108.0f;
    for (i = 0; i < 108; i++) {
        W01CurveSetup(lbl_1_data_240, seg, 33, &a, &b, &c, &d);
        t = W01PathTimeGet(fn_1_14BF0, &a, &b, &c, &d, t, dist);
        mbHermiteCalcV(&a, &b, &c, &d, &pos, t);
        mbPlayerPosGet(playerNo, &playerPos);
        VECSubtract(&pos, &playerPos, &dir);
        rot.x = -HuAtan(dir.y, sqrtf((dir.z * dir.z) + (dir.x * dir.x)));
        rot.y = HuAtan(dir.x, dir.z);
        rot.z = 0;
        mbPlayerRotSetV(playerNo, &rot);
        mbPlayerPosSetV(playerNo, &pos);
        dist += speed;
        if (dist >= lbl_1_bss_1188[seg]) {
            dist -= lbl_1_bss_1188[seg];
            seg++;
            t -= 1.0f;
        }
        HuPrcVSleep();
    }
    GwPlayer[playerNo].masuId = nextMasuId;
    mbPlayerPosGet(playerNo, &endPos);
    mbMasuPosGet(nextMasuId, &masuPos);
    VECSubtract(&masuPos, &endPos, &dir);
    angle = HuAtan(dir.x, dir.z);
    mbPlayerRotSet(playerNo, 0, angle, 0);
    mbPlayerMotionShiftSet(playerNo, 4, 0, 8, 0);
    W01PlayerJump(playerNo, &masuPos, angle);
    mbPlayerMotionShiftSet(playerNo, 1, 0, 2, 0x40000001);
    mbPlayerMotionKill(playerNo, motId);
    GwPlayer[playerNo].moveF = FALSE;
    mbPlayerColSnapPlayerSet(playerNo, TRUE);
}

void fn_1_ED00(void)
{
    W01BAT *bat;
    W01BATSUB *sub;
    s32 i;
    float angle;
    float radius;
    Mtx mtx;
    Mtx rot;
    Mtx trans;
    HuVecF ofs;

    bat = &lbl_1_bss_EFC;
    sub = bat->sub;
    bat->mdlId = mbObjCreate(W01_DATANUM(0x29), NULL, FALSE);
    bat->pos.x = 1871;
    bat->pos.y = 2479;
    bat->pos.z = 1078;
    bat->rot.x = -54;
    bat->rot.y = -4;
    bat->rot.z = 0;
    bat->rotBase = bat->rot;
    bat->angle = bat->angVel = 0;
    MTXTrans(trans, bat->pos.x, bat->pos.y, bat->pos.z);
    mbMtxRot(rot, bat->rot.x, bat->rot.y, bat->rot.z);
    MTXConcat(trans, rot, mtx);
    for (i = 0; i < 5; i++, sub++) {
        memset(sub, 0, sizeof(W01BATSUB));
        sub->mdlId = mbObjCreate(W01_DATANUM(0x2A), NULL, TRUE);
        mbObjBiriQCreate((s16)sub->mdlId);
        mbObjAttrSet(sub->mdlId, 0x40000001);
        sub->angle = angle = 360.0f * frandf();
        radius = 100.0f * (1.6f * frandf());
        ofs.x = radius * mbCosDeg(angle);
        ofs.y = -420;
        ofs.z = radius * mbSinDeg(angle);
        MTXMultVec(mtx, &ofs, &sub->homePos);
        sub->no = i;
        sub->swayAmp = 60.000004f + (100.0f * frandf());
        sub->swayAngle = 360.0f * frandf();
        sub->swaySpeed = 8.0f * ((2.0f * frandf()) - 1.0f);
        sub->radius = sub->radiusBase = 80.0f + (100.0f * (0.5f * frandf()));
        sub->maxTime = sub->maxTimeInit = 30.0f + (12.0f * frandf());
        sub->time = mbRandMod(sub->maxTime);
        sub->parent = bat;
    }
}

void fn_1_F06C(void)
{
    s32 i;
    W01BAT *bat;
    W01BATSUB *sub;

    bat = &lbl_1_bss_EFC;
    sub = bat->sub;
    for (i = 0; i < 5; i++, sub++) {
        mbObjBiriQKill((s16)bat->mdlId);
    }
}

void fn_1_F0D0(void)
{
    W01BAT *bat;
    s32 i;
    W01BATSUB *sub;
    float accel;

    bat = &lbl_1_bss_EFC;
    accel = (-100000.0f * mbSinRad(bat->angle)) / 500.0f;
    bat->angVel += 0.016666668f * accel;
    bat->angle += 0.016666668f * bat->angVel;
    bat->rot.y = bat->rotBase.y + (57.29578f * bat->angle);
    bat->angVel *= 0.9f;
    mbObjPosSetV(bat->mdlId, &bat->pos);
    mbObjRotSetV(bat->mdlId, &bat->rot);
    for (sub = bat->sub, i = 0; i < 5; i++, sub++) {
        fn_1_F66C(sub);
    }
}

void fn_1_F1F4(int playerNo, s16 masuId)
{
    W01BAT *bat;
    HuVecF coinPos;
    HuVecF pos;
    s16 motId[2];
    W01BATSUB *sub;
    s8 padNo;
    float rotY;
    float t;
    s32 i;
    s32 seNo;
    s32 coin;
    s32 coinNum;
    s32 coinLeft;

    bat = &lbl_1_bss_EFC;
    sub = bat->sub;
    motId[0] = mbPlayerMotionCreate(playerNo, 0x930017);
    motId[1] = mbPlayerMotionCreate(playerNo, 0x930062);
    mbCameraPlayerViewSet(playerNo, 1);
    mbPlayerMotionShiftSet(playerNo, motId[0], 0, 4, 0);
    bat->mode = 1;
    seNo = mbAudFXPlay(0x595);
    HuPrcSleep(30);
    mbPlayerMotionShiftSet(playerNo, 9, 0, 4, 0);
    rotY = mbPlayerRotYGet(playerNo);
    for (i = 0; i <= 12u; i++) {
        t = i / 12.0f;
        mbPlayerRotYSet(playerNo, mbAngleEaseOut(rotY, 0, t));
        HuPrcVSleep();
    }
    while (!mbPlayerMotionEndCheck(playerNo)) {
        HuPrcVSleep();
    }
    mbPlayerMotionShiftSet(playerNo, motId[1], 0, 8, 0x40000001);
    bat->mode = 2;
    padNo = GwPlayer[playerNo].padNo;
    lbl_1_bss_EF8 = -1;
    coin = mbPlayerCoinGet(playerNo);
    coinLeft = coin;
    mbPlayerPosGet(playerNo, &pos);
    CharModelVoiceFlagSet(GwPlayer[playerNo].charNo, FALSE);
    HuPrcSleep(60);
    coinNum = mbRandMod(4) + 1;
    fn_1_10998(coinNum);
    while (lbl_1_bss_EF8 < 0) {
        HuPrcVSleep();
    }
    fn_1_10900();
    mbPlayerMotionShiftSet(playerNo, 15, 0, 4, 0x40000001);
    omVibrate(playerNo, 20, 7, 3);
    CharFXPlay(GwPlayer[playerNo].charNo, 0x240);
    coinPos.x = pos.x;
    coinPos.y = 100.0f + pos.y;
    coinPos.z = pos.z;
    if (coinLeft > 0) {
        if (coinLeft >= coinNum) {
            fn_1_13D8C(&coinPos, coinNum);
        } else {
            fn_1_13D8C(&coinPos, coinLeft);
        }
        coinLeft -= coinNum;
    }
    for (i = 0; i < 30u; i++) {
        t = i / 30.0f;
        coinPos.y = pos.y + (100.0f * (2.0f * mbSinDeg(180.0f * t)));
        mbPlayerPosSet(playerNo, pos.x, coinPos.y, pos.z);
        HuPrcVSleep();
    }
    bat->mode = 0;
    mbAudFXStop(seNo);
    mbPlayerMotIdleSet(playerNo);
    HuPrcSleep(30);
    CharModelVoiceFlagSet(GwPlayer[playerNo].charNo, TRUE);
    mbPlayerMotionShiftSet(playerNo, 13, 0, 4, 0);
    mbCoinAddProcExec(playerNo, -(coin - coinLeft), -1, TRUE);
    for (i = 0; i < 2; i++) {
        mbPlayerMotionKill(playerNo, motId[i]);
    }
}

static inline float W01Asin(float x)
{
    if (x >= 1.0f) {
        return 1.5707964f;
    } else if (x <= -1.0f) {
        return -1.5707964f;
    } else {
        return asinf(x);
    }
}

void fn_1_F66C(W01BATSUB *sub)
{
    Mtx mtx;
    HuVecF ofs;
    HuVecF playerPos;
    HuVecF dir;
    HuVecF rot;
    HuVecF delta;
    HuVecF delta2;
    float rotY;
    W01BAT *bat;
    int playerNo;
    float angle;
    float dist;
    float t;
    float ang;
    float asinX;
    float x;

    playerNo = GwSystem.turnPlayerNo;
    bat = sub->parent;
    switch (sub->mode) {
        case 0:
            if (bat->mode == 1) {
                sub->mode = 1;
                mbPlayerPosGet(playerNo, &playerPos);
                VECSubtract(&playerPos, &sub->pos, &dir);
                angle = HuAtan(dir.x, dir.z);
                dist = VECMag(&dir);
                if (dist < sub->radius) {
                    x = sub->radius / sqrtf((sub->radius * sub->radius) + (dist * dist));
                    if (x >= 1.0f) {
                        asinX = 1.5707964f;
                    } else if (x <= -1.0f) {
                        asinX = -1.5707964f;
                    } else {
                        asinX = asinf(x);
                    }
                    ang = 1.5707964f - asinX;
                } else if (0.0f != dist) {
                    ang = 1.5707964f - W01Asin(sub->radius / dist);
                } else {
                    ang = 0;
                }
                if (frand() & 1) {
                    sub->orbitAngle = angle + (57.29578f * ang);
                    sub->orbitDir = 1.0f;
                } else {
                    sub->orbitAngle = angle - (57.29578f * ang);
                    sub->orbitDir = -1.0f;
                }
                sub->posY = (150.0f + playerPos.y) - (100.0f * (1.3f * frandf()));
                sub->radiusVel = 0;
            } else if (sub->wait != 0) {
                sub->wait--;
            } else if (sub->time > sub->maxTime) {
                sub->time = 0;
                sub->wait = 30.0f + (30.0f * frandf());
                bat->angVel += 0.5f * mbCosDeg(sub->angle);
            } else {
                t = (float)sub->time++ / sub->maxTime;
                angle = 360.0f * t;
                ofs.x = 0.5f * (sub->swayAmp * mbSinDeg(angle));
                ofs.y = sub->swayAmp * (mbCosDeg(angle) - 1.0f);
                ofs.z = 0;
                mbMtxRotAxisDeg(mtx, 'y', sub->swayAngle);
                MTXMultVec(mtx, &ofs, &ofs);
                sub->swayAngle += sub->swaySpeed;
                mbMtxRot(mtx, bat->rot.x, bat->rot.y, bat->rot.z);
                mtxTransCat(mtx, sub->homePos.x, sub->homePos.y, sub->homePos.z);
                MTXMultVec(mtx, &ofs, &ofs);
                VECSubtract(&ofs, &sub->pos, &dir);
                sub->pos = ofs;
                sub->rot.x = HuAtan(-dir.y, sqrtf((dir.z * dir.z) + (dir.x * dir.x)));
                sub->rot.y = HuAtan(dir.x, dir.z);
            }
            break;
        case 1:
            if (sub->radius != sub->radiusBase || 0.0f != sub->radiusVel) {
                sub->radiusVel -= 20.0f;
                sub->radius += 0.016666668f * sub->radiusVel;
                if (sub->radius < sub->radiusBase && sub->radiusVel < 0.0f) {
                    sub->orbitAngle += sub->orbitDir * ((360.0f * (sub->radiusBase - sub->radius)) / (6.2831855f * sub->radiusBase));
                    sub->radiusVel = 0;
                    sub->radius = sub->radiusBase;
                }
            }
            mbPlayerPosGet(playerNo, &playerPos);
            ofs.x = playerPos.x - (sub->radius * mbSinDeg(sub->orbitAngle));
            ofs.y = sub->posY;
            ofs.z = playerPos.z - (sub->radius * mbCosDeg(sub->orbitAngle));
            VECSubtract(&ofs, &sub->pos, &dir);
            rot.x = HuAtan(-dir.y, sqrtf((dir.z * dir.z) + (dir.x * dir.x)));
            rot.y = HuAtan(dir.x, dir.z);
            dist = VECMag(&dir);
            sub->orbitAngle += (360.0f * sub->orbitDir) * (10.0f / (6.2831855f * dist));
            angle = 360.0f * (100.0f / (6.2831855f * dist));
            if (angle > 20.0f) {
                angle = 20.0f;
            }
            mbAngleAdd(&sub->rot.x, rot.x, angle);
            mbAngleAdd(&sub->rot.y, rot.y, angle);
            mbMtxRot(mtx, sub->rot.x, sub->rot.y, sub->rot.z);
            MTXMultVec(mtx, &lbl_1_data_780, &dir);
            VECAdd(&sub->pos, &dir, &sub->pos);
            if (bat->mode == 0) {
                sub->mode = 6;
                sub->time = 0;
                sub->maxTime = 180;
            }
            break;
        case 2:
            t = (float)sub->time++ / sub->maxTime;
            VECSubtract(&sub->end, &sub->start, &delta);
            VECScale(&delta, &delta, t);
            VECAdd(&sub->start, &delta, &sub->pos);
            mbPlayerPosGet(playerNo, &playerPos);
            VECSubtract(&sub->pos, &playerPos, &dir);
            rotY = HuAtan(-dir.x, -dir.z);
            sub->rot.x = -80.0f * (1.0f - mbCosDeg(90.0f * t));
            sub->rot.y = mbAngleLerp(sub->rot.y, rotY, t);
            if (sub->time > sub->maxTime) {
                sub->mode = 3;
                sub->time = 0;
                sub->maxTime = 30;
            }
            break;
        case 3:
            t = (float)sub->time++ / sub->maxTime;
            if (sub->time > sub->maxTime) {
                sub->mode = 4;
                sub->time = 0;
                sub->maxTime = 9;
                sub->start = sub->pos;
                mbPlayerPosGet(playerNo, &playerPos);
                VECSubtract(&sub->pos, &playerPos, &dir);
                angle = HuAtan(dir.z, dir.x);
                sub->end.x = playerPos.x + (50.0f * mbCosDeg(angle));
                sub->end.y = sub->posY;
                sub->end.z = playerPos.z + (50.0f * mbSinDeg(angle));
            }
            break;
        case 4:
            t = (float)sub->time++ / sub->maxTime;
            VECSubtract(&sub->end, &sub->start, &delta2);
            VECScale(&delta2, &delta2, t);
            VECAdd(&sub->start, &delta2, &sub->pos);
            sub->rot.x = -80.0f + (-40.0f * (1.0f - mbCosDeg(90.0f * t)));
            if (sub->time > sub->maxTime) {
                sub->mode = 5;
                lbl_1_bss_EF8 = sub->no;
                sub->time = 0;
                sub->maxTime = 6;
            }
            break;
        case 5:
            t = (float)sub->time++ / sub->maxTime;
            if (sub->time > sub->maxTime) {
                sub->mode = 1;
                sub->radiusVel = 600.0f;
            }
            break;
        case 6:
            VECSubtract(&sub->homePos, &sub->pos, &dir);
            rot.x = HuAtan(-dir.y, sqrtf((dir.z * dir.z) + (dir.x * dir.x)));
            rot.y = HuAtan(dir.x, dir.z);
            dist = VECMag(&dir);
            if (dist < 50.0f || ++sub->time > sub->maxTime) {
                sub->mode = 0;
                sub->pos = sub->homePos;
                sub->time = 0;
                sub->maxTime = sub->maxTimeInit;
            } else {
                angle = 360.0f * (100.0f / (6.2831855f * dist));
                if (angle > 20.0f) {
                    angle = 20.0f;
                }
                mbAngleAdd(&sub->rot.x, rot.x, angle);
                mbAngleAdd(&sub->rot.y, rot.y, angle);
                mbMtxRot(mtx, sub->rot.x, sub->rot.y, sub->rot.z);
                MTXMultVec(mtx, &lbl_1_data_780, &dir);
                VECAdd(&sub->pos, &dir, &sub->pos);
            }
            break;
    }
    mbObjPosSetV(sub->mdlId, &sub->pos);
    mbObjRotSetV(sub->mdlId, &sub->rot);
}
/* @@PART6@@ */

BOOL fn_1_1089C(void)
{
    W01BATSUB *bat = &lbl_1_bss_EFC.sub[0];
    int i;

    for(i = 0; i < 5; i++, bat++) {
        if(bat->mode != 0 && bat->mode != 6) {
            return FALSE;
        }
    }
    return TRUE;
}

void fn_1_10900(void)
{
    W01BATSUB *bat = &lbl_1_bss_EFC.sub[0];
    GXColor color = { 0, 0, 0, 255 };
    int i;

    for(i = 0; i < 5; i++, bat++) {
        if(bat->mode >= 1 && bat->mode <= 4) {
            bat->mode = 1;
            bat->radiusVel = 600.0f;
        }
    }
}

void fn_1_10998(int num)
{
    W01BATSUB *bat = &lbl_1_bss_EFC.sub[0];
    GXColor color = { 0, 0, 0, 255 };
    int i;

    for(i = 0; i < num; i++, bat++) {
        bat->mode = 2;
        bat->time = 0;
        bat->maxTime = 12;
        bat->start = bat->pos;
        bat->end.x = bat->start.x + (200.0f * mbSinDeg(bat->rot.y));
        bat->end.y = 50.0f + bat->start.y;
        bat->end.z = bat->start.z + (200.0f * mbCosDeg(bat->rot.y));
    }
}

s32 lbl_1_data_7A8[5] = {
    DATANUM(DATA_w01, 0x2B), DATANUM(DATA_w01, 0x2C), DATANUM(DATA_w01, 0x2D), DATANUM(DATA_w01, 0x2E), DATANUM(DATA_w01, 0x2F),
};

void fn_1_10AA8(void)
{
    s16 *objId = lbl_1_bss_EE8;
    int i;
    int layer;

    for(i = 0; i < 5; i++) {
        objId[i] = mbObjCreate(W01_DATANUM(FILENUM(lbl_1_data_7A8[i])), NULL, FALSE);
        if(i != 0) {
            mbObjDispSet(objId[i], FALSE);
        }
    }
    objId[5] = mbObjCreate(W01_DATANUM(0x30), NULL, FALSE);
    mbObjDispSet(objId[5], FALSE);
    layer = mbMasuLayerGet();
    mbMasuLayerSet(1);
    mbMasuDataRead(W01_DATANUM(0x01));
    mbMasuLayerSet(layer);
    lbl_1_bss_EE4 = HuSprAnimRead(HuDataSelHeapReadNum(W01_DATANUM(0x31), HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(lbl_1_bss_EE4);
}

void fn_1_10C68(void)
{
    if(lbl_1_bss_EE4) {
        HuSprAnimKill(lbl_1_bss_EE4);
        lbl_1_bss_EE4 = NULL;
    }
}

HuVecF lbl_1_data_7BC = { 0.0f, 950.0f, 3000.0f };
HuVecF lbl_1_data_7C8 = { -19.0f, 0.0f, 0.0f };
HuVecF lbl_1_data_7D4 = { 0.0f, 250.0f, 0.0f };
HuVecF lbl_1_data_7E0 = { 0.0f, 900.0f, 2500.0f };

void fn_1_10CB8(int playerNo, s16 masuId)
{
    s16 *objId;
    int k;
    s16 winId;
    int i;
    int j;
    int timer;
    int seWait;
    int pressCnt;
    int idleCnt;
    int linkCnt;
    int pressF;
    s16 motId;
    GAMEMESID mesId;
    int seNo;
    float speed;
    float t;
    float t2;
    float angle;
    float rotT;
    float rotT2;
    float t3;
    float rotY;
    float rotY2;
    Mtx hookMtx;
    Mtx mtx;
    HuVecF playerPos;
    HuVecF linkPos;
    HuVecF nextPos;
    HuVecF diff;
    HuVecF dir;
    HuVecF movePos;
    HuVecF startPos;
    HuVecF dir2;
    HuVecF movePos2;
    HuVecF startPos2;
    int pushF;
    int motTime;
    int masuNext;
    int masuNext2;
    s16 linkMasu;
    s16 nextMasu;
    s16 helpWinId;
    u16 btn;
    s8 padNo;

    seWait = 0;
    pressF = 0;
    seNo = -1;
    objId = lbl_1_bss_EE8;
    motId = mbPlayerMotionCreate(playerNo, DATANUM(DATA_mariomot, 0x64));
    mbPlayerPosGet(playerNo, &playerPos);
    linkMasu = mbMasuAttrFindLink(masuId, 0x2000);
    mbMasuPosGet(linkMasu, &linkPos);
    VECSubtract(&linkPos, &playerPos, &diff);
    angle = HuAtan(diff.x, diff.z);
    mbPlayerMotionShiftSet(playerNo, 4, 0.0f, 8.0f, HU3D_MOTATTR_NONE);
    mbPlayerPosGet(playerNo, &startPos);
    rotY = mbPlayerRotYGet(playerNo);
    VECSubtract(&linkPos, &startPos, &dir);
    for(i = 0; i <= 18; i++) {
        t = i / 18.0f;
        if(i == 12u) {
            mbPlayerMotionShiftSet(playerNo, 5, 0.0f, 2.0f, HU3D_MOTATTR_NONE);
        }
        movePos.x = startPos.x + (t * dir.x);
        movePos.y = (startPos.y + (t * dir.y)) + (100.0 * HuSin(180.0f * t));
        movePos.z = startPos.z + (t * dir.z);
        mbPlayerPosSetV(playerNo, &movePos);
        rotT = i / 6.0f;
        if(rotT > 1.0f) {
            rotT = 1.0f;
        }
        mbPlayerRotYSet(playerNo, mbAngleLerp(rotY, angle, rotT));
        masuNext = 18 - i;
        mbPlayerWorkGet(playerNo)->masuNext = masuNext;
        HuPrcVSleep();
    }
    mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 2.0f, HU3D_MOTATTR_LOOP);
    nextMasu = mbMasuLinkGet(linkMasu, 0);
    mbMasuPosGet(nextMasu, &nextPos);
    VECSubtract(&nextPos, &linkPos, &diff);
    angle = HuAtan(diff.x, diff.z);
    HuPrcSleep(3);
    mbPlayerMotionShiftSet(playerNo, 4, 0.0f, 8.0f, HU3D_MOTATTR_NONE);
    mbPlayerPosGet(playerNo, &startPos2);
    rotY2 = mbPlayerRotYGet(playerNo);
    VECSubtract(&nextPos, &startPos2, &dir2);
    for(j = 0; j <= 18; j++) {
        t2 = j / 18.0f;
        if(j == 12u) {
            mbPlayerMotionShiftSet(playerNo, 5, 0.0f, 2.0f, HU3D_MOTATTR_NONE);
        }
        movePos2.x = startPos2.x + (t2 * dir2.x);
        movePos2.y = (startPos2.y + (t2 * dir2.y)) + (100.0 * HuSin(180.0f * t2));
        movePos2.z = startPos2.z + (t2 * dir2.z);
        mbPlayerPosSetV(playerNo, &movePos2);
        rotT2 = j / 6.0f;
        if(rotT2 > 1.0f) {
            rotT2 = 1.0f;
        }
        mbPlayerRotYSet(playerNo, mbAngleLerp(rotY2, angle, rotT2));
        masuNext2 = 18 - j;
        mbPlayerWorkGet(playerNo)->masuNext = masuNext2;
        HuPrcVSleep();
    }
    mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 2.0f, HU3D_MOTATTR_LOOP);
    mbCameraMovePos(&lbl_1_data_7BC, &lbl_1_data_7C8, &lbl_1_data_7D4, 4500.0f, -1.0f, 30);
    mbCameraMoveWait();
    HuPrcSleep(12);
    winId = mbWinCreate(2, 0x2F000F, -1);
    mbWinWait(winId);
    motTime = mbPlayerMotionTimeGet(playerNo);
    mbPlayerMotionSet(playerNo, motId, HU3D_MOTATTR_NONE);
    mbPlayerMotionTimeSet(playerNo, 0.0f);
    Hu3DMotionCalc(mbPlayerModelIDGet(playerNo));
    Hu3DModelObjMtxGet(mbObjModelIDGet(mbPlayerObjIDGet(playerNo)), CharModelItemHookGet(GwPlayer[playerNo].charNo, 4, 0), hookMtx);
    mbPlayerMotionSet(playerNo, 1, HU3D_MOTATTR_LOOP);
    mbPlayerMotionTimeSet(playerNo, motTime);
    Hu3DMotionCalc(mbPlayerModelIDGet(playerNo));
    mbPlayerMotionShiftSet(playerNo, motId, 0.0f, 30.0f, HU3D_MOTATTR_NONE);
    mbObjDispSet(objId[5], TRUE);
    MTXCopy(hookMtx, mtx);
    for(k = 0; k < 30u; k++) {
        t3 = k / 30.0f;
        mtx[0][3] = hookMtx[0][3] + (100.0f * (10.0f * (1.0f - t3)));
        mtx[1][3] = hookMtx[1][3] + (100.0f * (5.0f * mbSinDeg(180.0f * t3)));
        mbObjMtxSet(objId[5], mtx);
        HuPrcVSleep();
    }
    MTXIdentity(mtx);
    mbObjMtxSet(objId[5], mtx);
    mbObjHookSet(mbPlayerObjIDGet(playerNo), CharModelItemHookGet(GwPlayer[playerNo].charNo, 4, 0), objId[5]);
    helpWinId = mbWinCreateHelp(0x2F0014);
    mbWinPosSet(helpWinId, 192, 336);
    padNo = GwPlayer[playerNo].padNo;
    pressCnt = 0;
    idleCnt = 0;
    speed = 0.0f;
    mbObjAttrSet(mbPlayerObjIDGet(playerNo), HU3D_MOTATTR_LOOP);
    pushF = FALSE;
    mesId = GameMesCreate(1, 5, -1, -1);
    HuSprGrpDrawNoSet(GameMesGet(mesId)->grpId[0], 32);
    timer = 300;
    for(k = 0; k <= 300u; k++) {
        if(GwPlayer[playerNo].comF) {
            if(frandf() < 0.2f) {
                btn = PAD_BUTTON_A;
            } else {
                btn = 0;
            }
        } else {
            btn = HuPadBtnDown[padNo];
        }
        if(btn & PAD_BUTTON_A) {
            if(mbObjMotionEndCheck(objId[1])) {
                if(pressCnt == 0) {
                    mbObjDispSet(objId[0], FALSE);
                }
                mbObjDispSet(objId[1], TRUE);
                mbObjMotionTimeSet(objId[1], 0.0f);
                mbObjMotionSpeedSet(objId[1], 1.0f);
                mbObjMotionShapeTimeSet(objId[1], 0.0f);
                mbObjMotionShapeSpeedSet(objId[1], 1.0f);
                pushF = TRUE;
            }
            pressF = TRUE;
            pressCnt++;
            idleCnt = 0;
            speed = 1.0f;
        } else {
            if(++idleCnt > 8) {
                speed -= 0.1f;
                if(speed < 0.0f) {
                    speed = 0.0f;
                }
            }
        }
        mbPlayerMotionSpeedSet(playerNo, speed);
        if(timer >= 0) {
            timer--;
            if(timer >= 0) {
                GameMesDispSet(mesId, 1, (timer + 59) / 60);
            } else {
                GameMesDispSet(mesId, 2, -1);
            }
        }
        if(pressF) {
            if(seWait == 0) {
                mbAudFXPlay(0x598);
                seWait = 12;
            }
            pressF = FALSE;
        }
        if(seWait) {
            seWait--;
        }
        HuPrcVSleep();
    }
    mbWipeDissolveFadeOut();
    mbWinKill(helpWinId);
    mbStatusDispForceSetAll(FALSE);
    mbCameraMovePos(NULL, NULL, NULL, 3200.0f, -1.0f, -1);
    mbCameraMoveWait();
    mbPlayerMotionSet(playerNo, 1, HU3D_MOTATTR_LOOP);
    mbPlayerPosReset(playerNo);
    mbPlayerColSnapPlayerSet(playerNo, TRUE);
    mbObjHookReset(mbPlayerObjIDGet(playerNo));
    mbObjDispSet(objId[5], FALSE);
    for(k = 0; k < 5; k++) {
        mbObjDispSet(objId[k], FALSE);
    }
    mbObjDispSet(objId[2], TRUE);
    mbObjAttrSet(objId[2], HU3D_MOTATTR_LOOP | HU3D_MOTATTR_SHAPE_LOOP);
    mbObjMotionTimeSet(objId[2], 0.0f);
    mbObjMotionSpeedSet(objId[2], 1.0f);
    mbObjMotionShapeTimeSet(objId[2], 0.0f);
    mbObjMotionShapeSpeedSet(objId[2], 1.0f);
    mbWipeDissolveFadeIn();
    seNo = mbAudFXPlay(0x599);
    winId = mbWinCreate(2, 0x2F0010, -1);
    mbWinWait(winId);
    mbMasuLayerSet(1);
    linkCnt = 0;
    for(k = 0; k < 4; k++) {
        masuId = GwPlayer[k].masuId;
        if(mbMasuLinkNumGet(masuId) != 0) {
            linkCnt++;
        }
    }
    mbMasuLayerSet(0);
    if(linkCnt != 0 && pressCnt > 5 && frandf() < 0.8f) {
        mbObjDispSet(objId[2], FALSE);
        mbObjDispSet(objId[3], TRUE);
        mbObjAttrSet(objId[3], HU3D_MOTATTR_NONE);
        mbObjMotionTimeSet(objId[3], 0.0f);
        mbObjMotionSpeedSet(objId[3], 1.0f);
        mbObjMotionShapeTimeSet(objId[3], 0.0f);
        mbObjMotionShapeSpeedSet(objId[3], 1.0f);
        mbCameraShakeSet(30, 500.0f);
        mbAudFXStop(seNo);
        mbAudFXPlay(0x59A);
        for(k = 0; k < 4; k++) {
            omVibrate(k, 20, 20, 0);
        }
        winId = mbWinCreate(2, 0x2F0011, -1);
        HuPrcSleep(17);
        fn_1_11C14(&lbl_1_data_7E0);
        mbWinWait(winId);
        while(objId[6] >= 0) {
            HuPrcVSleep();
        }
        mbWipeDissolveFadeOut();
        mbObjDispSet(objId[3], FALSE);
        mbObjDispSet(objId[0], TRUE);
        fn_1_12090();
        mbCameraMovePos(&lbl_1_data_7BC, &lbl_1_data_7C8, &lbl_1_data_7D4, 4500.0f, -1.0f, -1);
        mbWipeDissolveFadeIn();
        winId = mbWinCreate(2, 0x2F0013, -1);
        mbWinWait(winId);
    } else {
        mbObjAttrReset(objId[2], HU3D_MOTATTR_LOOP | HU3D_MOTATTR_SHAPE_LOOP);
        while(!mbObjMotionEndCheck(objId[2])) {
            HuPrcVSleep();
        }
        mbAudFXStop(seNo);
        mbObjDispSet(objId[2], FALSE);
        mbObjDispSet(objId[4], TRUE);
        mbObjMotionTimeSet(objId[4], 0.0f);
        mbObjMotionSpeedSet(objId[4], 1.0f);
        mbObjMotionShapeTimeSet(objId[4], 0.0f);
        mbObjMotionShapeSpeedSet(objId[4], 1.0f);
        winId = mbWinCreate(2, 0x2F0012, -1);
        mbWinWait(winId);
    }
    mbPlayerMotionKill(playerNo, motId);
}

void fn_1_11C14(HuVecF *pos)
{
    s16 *objId = lbl_1_bss_EE8;

    objId[6] = mbParticleCreate(lbl_1_bss_EE4, 100);
    Hu3DModelLayerSet(objId[6], 5);
    mbParticleHookSet(objId[6], fn_1_11C84);
    Hu3DModelPosSetV(objId[6], pos);
}

void fn_1_11C84(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx)
{
    GXColor color = { 255, 255, 192, 192 };
    s16 *objId;
    MBPARTICLEDATA *dataP;
    int num;
    int i;
    float angleX;
    float angleY;
    float t;
    float speed;
    HuVecF dir;

    objId = lbl_1_bss_EE8;
    if(particleP->unk00 == 0) {
        for(dataP = particleP->data, i = 0; i < particleP->num; i++, dataP++) {
            angleY = (45.0f * frandf()) - 22.5f;
            angleX = (30.0f * frandf()) - 10.0f;
            dir.x = mbSinDeg(angleY) * mbCosDeg(angleX);
            dir.y = mbSinDeg(angleX);
            dir.z = mbCosDeg(angleY) * mbCosDeg(angleX);
            dataP->pos.x = 100.0f * (15.0f * dir.x);
            dataP->pos.y = 100.0f * (2.0f * dir.y);
            dataP->pos.z = (100.0f * (4.0f * frandf())) - 200.0f;
            speed = 4000.0f + (4000.0f * frandf());
            VECScale(&dir, &dataP->vel, speed);
            dataP->scale = 30.0f + (20.0f * frandf());
            dataP->color = color;
            dataP->time = 0;
            dataP->aliveF = 60.0f + (30.0f * frandf());
        }
        particleP->unk00 = 1;
    }
    num = 0;
    for(dataP = particleP->data, i = 0; i < particleP->num; i++, dataP++) {
        if(dataP->time <= dataP->aliveF) {
            t = (float)dataP->time++ / dataP->aliveF;
            dataP->pos.x += 0.016666668f * dataP->vel.x;
            dataP->pos.y += 0.016666668f * dataP->vel.y;
            dataP->pos.z += 0.016666668f * dataP->vel.z;
            dataP->vel.y += -16.333334f;
            dataP->color.a = 192.0f * (1.0f - t);
            num++;
        }
    }
    if(num == 0) {
        mbParticleKill(particleP->modelId);
        objId[6] = -1;
    }
}

void fn_1_12090(void)
{
    int i;
    int j;
    s16 masuId;
    float t;
    HuVecF masuPos;
    HuVecF linkPos;
    HuVecF pos;
    HuVecF dir;
    s16 linkMasu[4];
    s16 motId[2];

    mbMasuLayerSet(1);
    for(i = 0; i < 4; i++) {
        masuId = GwPlayer[i].masuId;
        if(mbMasuLinkNumGet(masuId) != 0) {
            linkMasu[i] = mbMasuLinkGet(masuId, 0);
        } else {
            linkMasu[i] = 0;
        }
    }
    mbMasuLayerSet(0);
    for(i = 0; i < 4; i++) {
        if(linkMasu[i] == 0) {
            continue;
        }
        motId[0] = mbPlayerMotionCreate(i, DATANUM(DATA_mariomot, 0x12));
        motId[1] = mbPlayerMotionCreate(i, DATANUM(DATA_mariomot, 0x13));
        mbCameraPlayerViewSetFast(i, 2);
        mbCameraShakeSet(30, 500.0f);
        mbWipeDissolveFadeIn();
        mbPlayerMotionShiftSet(i, motId[0], 0.0f, 4.0f, HU3D_MOTATTR_NONE);
        mbMasuPosGet(GwPlayer[i].masuId, &masuPos);
        mbMasuPosGet(linkMasu[i], &linkPos);
        VECSubtract(&linkPos, &masuPos, &dir);
        mbPlayerRotYSet(i, HuAtan(dir.x, dir.z));
        GwPlayer[i].masuIdNext = linkMasu[i];
        HuPrcVSleep();
        mbPlayerColSnapPlayerSet(i, FALSE);
        for(j = 0; j <= 48u; j++) {
            t = j / 48.0f;
            pos.x = masuPos.x + (t * (linkPos.x - masuPos.x));
            pos.y = (masuPos.y + (t * (linkPos.y - masuPos.y))) + (100.0f * (7.5f * mbSinDeg(180.0f * t)));
            pos.z = masuPos.z + (t * (linkPos.z - masuPos.z));
            mbPlayerPosSetV(i, &pos);
            HuPrcVSleep();
        }
        CharFXPlay(GwPlayer[i].charNo, 0xC3);
        omVibrate(i, 20, 7, 3);
        mbPlayerMotionShiftSet(i, motId[1], 0.0f, 4.0f, HU3D_MOTATTR_NONE);
        HuPrcSleep(30);
        mbWipeDissolveFadeOut();
        mbPlayerRotYSet(i, 0.0f);
        mbPlayerMotionSet(i, 1, HU3D_MOTATTR_LOOP);
        mbPlayerColSnapPlayerSet(i, TRUE);
        mbPlayerMotionKill(i, motId[0]);
        mbPlayerMotionKill(i, motId[1]);
        GwPlayer[i].masuId = linkMasu[i];
    }
}

void fn_1_12454(void)
{
    int i;
    s16 *objId = lbl_1_bss_EE8;

    for(i = 0; i < 5; i++) {
        if(i == 0) {
            mbObjDispSet(objId[i], TRUE);
        } else {
            mbObjDispSet(objId[i], FALSE);
        }
    }
}

void fn_1_124C8(void)
{
    W01_TERESA *work = &lbl_1_bss_EB4;
    s16 linkMasu;

    work->objId[0] = mbObjCreate(DATANUM(DATA_capsuleshop, 0x08), NULL, FALSE);
    mbObjDispSet(work->objId[0], FALSE);
    mbObjLayerSet(work->objId[0], 3);
    mbObjScaleSet(work->objId[0], 2.0f, 2.0f, 2.0f);
    work->objId[1] = mbObjCreate(W01_DATANUM(0x18), NULL, FALSE);
    mbObjAttrSet(work->objId[1], HU3D_MOTATTR_PAUSE);
    work->objId[2] = mbObjCreate(W01_DATANUM(0x19), NULL, FALSE);
    mbObjAttrSet(work->objId[2], HU3D_MOTATTR_LOOP);
    mbObjHookSet(work->objId[1], "b01_m082", work->objId[2]);
    work->masuId = mbMasuFind_MAttrIdGet(-1, 0x1000000);
    mbMasuPosGet(work->masuId, &work->masuPos);
    linkMasu = mbMasuAttrFindLink(work->masuId, 0x2000);
    mbMasuPosGet(linkMasu, &work->linkPos);
    VECSubtract(&work->masuPos, &work->linkPos, &work->dir);
    work->rotY = HuAtan(work->dir.x, work->dir.z);
    mbObjRotSet(work->objId[0], 0.0f, work->rotY, 0.0f);
    mbObjRotSet(work->objId[1], 0.0f, work->rotY, 0.0f);
    mbObjPosSetV(work->objId[1], &work->linkPos);
    HuDataDirClose(DATANUM(DATA_capsuleshop, 0));
}

void fn_1_126EC(int playerNo, s16 masuId)
{
    W01_TERESA *work = &lbl_1_bss_EB4;
    int i;
    s16 winId;
    MBOBJID objId;
    float t;
    float s;
    float rotY;
    HuVecF pos;
    HuVecF endPos;

    if(GwSystem.curTime == 0) {
        mbPlayerMotIdleSet(playerNo);
        winId = mbWinCreate(2, 0x2F0015, -1);
        mbWinWait(winId);
    } else {
        mbMoveNumDispSet(playerNo, FALSE);
        rotY = mbPlayerRotYGet(playerNo);
        mbObjDispSet(work->objId[0], TRUE);
        mbev_CapTeresaFadeCreate(work->objId[0]);
        objId = work->objId[1];
        mbObjAttrReset(objId, HU3D_MOTATTR_PAUSE);
        omVibrate(playerNo, 20, 7, 3);
        endPos.x = work->linkPos.x + (0.6f * work->dir.x);
        endPos.y = work->linkPos.y + (0.6f * work->dir.y);
        endPos.z = work->linkPos.z + (0.6f * work->dir.z);
        for(i = 0; i <= 60u; i++) {
            t = i / 60.0f;
            s = mbSinDeg(90.0f * t);
            pos.x = work->linkPos.x + (s * (endPos.x - work->linkPos.x));
            pos.y = 150.0f + (work->linkPos.y + (s * (endPos.y - work->linkPos.y)));
            pos.z = work->linkPos.z + (s * (endPos.z - work->linkPos.z));
            mbObjPosSetV(work->objId[0], &pos);
            mbev_CapTeresaFadeSet(255.0f * t);
            if(i == 6u) {
                mbPlayerMotionShiftSet(playerNo, 9, 0.0f, 4.0f, HU3D_MOTATTR_NONE);
            }
            if(i > 6u) {
                t = (i - 6u) / 12.0f;
                if(t > 1.0f) {
                    t = 1.0f;
                }
                mbPlayerRotYSet(playerNo, mbAngleEaseOut(rotY, 180.0f + work->rotY, t));
            }
            HuPrcVSleep();
        }
        HuPrcSleep(30);
        mbWipeDissolveFadeOut();
        mbev_CapTeresaFadeKill(work->objId[0]);
        mbObjDispSet(work->objId[0], FALSE);
        mbev_CapCallTeresa(playerNo, work->masuId);
        mbObjMotionTimeSet(work->objId[1], 0.0f);
        mbObjAttrSet(work->objId[1], HU3D_MOTATTR_PAUSE);
        mbMoveNumDispSet(playerNo, TRUE);
        mbWipeDissolveFadeIn();
    }
}

s32 lbl_1_data_7F8[3] = { DATANUM(DATA_w01, 0x34), DATANUM(DATA_w01, 0x35), DATANUM(DATA_w01, 0x36) };

void fn_1_12A4C(void)
{
    W01_LEAF *leaf = lbl_1_bss_4B0;
    int i;

    for(i = 0; i < 64; i++, leaf++) {
        leaf->pos.x = 3000.0f * frandf();
        leaf->pos.y = 6000.0f * frandf();
        leaf->pos.z = 2000.0f * frandf();
        leaf->rot = 6.2831855f * frandf();
        leaf->rotVel = 0.0f;
        leaf->scale = 0.7f + (0.3f * frandf());
        leaf->texNo = mbRandMod(3);
    }
    for(i = 0; i < 3; i++) {
        lbl_1_bss_4A4[i] = HuSprAnimRead(HuDataSelHeapReadNum(W01_DATANUM(FILENUM(lbl_1_data_7F8[i])), HU_MEMNUM_OVL, HEAP_MODEL));
        HuSprAnimLock(lbl_1_bss_4A4[i]);
    }
    lbl_1_bss_498 = fn_1_130C4(&lbl_1_bss_49C);
    lbl_1_bss_4A0 = Hu3DHookFuncCreate(fn_1_12D58);
    Hu3DModelCameraSet(lbl_1_bss_4A0, 1);
    Hu3DModelLayerSet(lbl_1_bss_4A0, 5);
    lbl_1_bss_48C.x = lbl_1_bss_48C.y = lbl_1_bss_48C.z = 0.0f;
    lbl_1_bss_480.x = 100.0f + (100.0f * (8.0f * frandf()));
    lbl_1_bss_480.y = 0.0f;
    lbl_1_bss_480.z = 0.0f;
    lbl_1_bss_EB0 = TRUE;
}

void fn_1_12CF0(void)
{
    int i;

    for(i = 0; i < 3; i++) {
        HuSprAnimKill(lbl_1_bss_4A4[i]);
        lbl_1_bss_4A4[i] = NULL;
    }
}

void fn_1_12D58(HU3D_MODEL *modelP, Mtx *mtx)
{
    W01_LEAF *leaf = lbl_1_bss_4B0;
    int i;
    GXColor color;
    Mtx modelMtx;
    Mtx rotMtx;

    if(!lbl_1_bss_EB0 || GwSystem.curTime != 0) {
        return;
    }
    for(i = 0; i < 3; i++) {
        HuSprTexLoad(lbl_1_bss_4A4[i], 0, i, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    }
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    for(i = 0; i < 64; i++, leaf++) {
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, leaf->texNo, GX_COLOR0A0);
        MTXScale(modelMtx, leaf->scale, leaf->scale, leaf->scale);
        mbMtxRotAxisRad(rotMtx, 'z', leaf->rot);
        MTXConcat(modelMtx, rotMtx, modelMtx);
        mtxTransCat(modelMtx, leaf->pos.x, leaf->pos.y, leaf->pos.z);
        MTXConcat(*mtx, modelMtx, modelMtx);
        GXLoadPosMtxImm(modelMtx, GX_PNMTX0);
        if(modelMtx[2][3] <= 0.0f && modelMtx[2][3] > -1000.0f) {
            color.a = 255.0f * (modelMtx[2][3] / -1000.0f);
        } else {
            color.a = 255;
        }
        GXSetTevColor(GX_TEVREG0, color);
        GXCallDisplayList(lbl_1_bss_49C, lbl_1_bss_498);
    }
}

static inline void *LeafDLMalloc(u32 size)
{
    return HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
}

u32 fn_1_130C4(void **dlBuf)
{
    void *buf;
    u32 size;

    buf = LeafDLMalloc(0x2000);
    DCInvalidateRange(buf, 0x2000);
    GXBeginDisplayList(buf, 0x2000);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-25.0f, 0.0f, 25.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(25.0f, 0.0f, 25.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(25.0f, 0.0f, -25.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(-25.0f, 0.0f, -25.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXEnd();
    size = GXEndDisplayList();
    *dlBuf = LeafDLMalloc(size);
    memcpy(*dlBuf, buf, size);
    DCFlushRange(*dlBuf, size);
    HuMemDirectFree(buf);
    return size;
}
/* @@PART7@@ */
void fn_1_1330C(void)
{
    W01_LEAF *leaf;
    MBCAMERA *cameraP;
    int i;
    BOOL dayF;
    HuVecF dir;
    HuVecF rel;
    HuVecF cross;
    float dot;
    float mag;
    float base;

    leaf = lbl_1_bss_4B0;
    cameraP = mbCameraGet();
    if (cameraP->dispOn && lbl_1_bss_EB0) {
    for (i = 0; i < 64; i++, leaf++) {
        dir.x = -mbSinRad(leaf->rot);
        dir.y = mbCosRad(leaf->rot);
        dir.z = 0.0f;
        rel.x = -leaf->vel.x;
        rel.y = -leaf->vel.y;
        rel.z = -leaf->vel.z;
        PSVECAdd(&rel, &lbl_1_bss_48C, &rel);
        mag = PSVECMag(&rel);
        if (0.0f != mag) {
            PSVECNormalize(&rel, &rel);
        } else {
            rel.x = rel.y = rel.z = 0.0f;
        }
        dot = PSVECDotProduct(&dir, &rel);
        if (dot < 0.0f) {
            dir.x = -dir.x;
            dir.y = -dir.y;
            dir.z = -dir.z;
            dot = -dot;
        }
        PSVECCrossProduct(&dir, &rel, &cross);
        if (cross.z < 0.0f) {
            leaf->rotVel -= 0.002f * ((1.0f / 60.0f) * mag * dot);
        } else {
            leaf->rotVel += 0.002f * ((1.0f / 60.0f) * mag * dot);
        }
        leaf->rot += leaf->rotVel;
        leaf->rotVel *= 0.95f;
        mag *= 10.0f;
        leaf->vel.x += (1.0f / 60.0f) * (mag * (dir.x * dot));
        leaf->vel.y += (-980.0f * (1.0f / 60.0f)) + ((1.0f / 60.0f) * (mag * (dir.y * dot)));
        leaf->vel.z += (1.0f / 60.0f) * (mag * (dir.z * dot));
        leaf->pos.x += (1.0f / 60.0f) * leaf->vel.x;
        leaf->pos.y += (1.0f / 60.0f) * leaf->vel.y;
        leaf->pos.z += (1.0f / 60.0f) * leaf->vel.z;
        PSVECScale(&leaf->vel, &leaf->vel, 0.95f);
        if (1500.0f + cameraP->pos.x < leaf->pos.x) {
            leaf->pos.x = (-1500.0f + cameraP->pos.x) + fmod(leaf->pos.x - (1500.0f + cameraP->pos.x), 3000.0);
        } else if (-1500.0f + cameraP->pos.x > leaf->pos.x) {
            leaf->pos.x = (1500.0f + cameraP->pos.x) + fmod(leaf->pos.x - (-1500.0f + cameraP->pos.x), 3000.0);
        }
        if (leaf->pos.y < 0.0f) {
            leaf->pos.y = 6000.0f;
        }
        if (-1000.0f + cameraP->pos.z < leaf->pos.z) {
            leaf->pos.z = (-3000.0f + cameraP->pos.z) + fmod(leaf->pos.z - (-1000.0f + cameraP->pos.z), 2000.0);
        } else if (-3000.0f + cameraP->pos.z > leaf->pos.z) {
            leaf->pos.z = (-1000.0f + cameraP->pos.z) + fmod(leaf->pos.z - (-3000.0f + cameraP->pos.z), 2000.0);
        }
    }
    lbl_1_bss_48C.x += 0.1f * (lbl_1_bss_480.x - lbl_1_bss_48C.x);
    lbl_1_bss_48C.y += 0.1f * (lbl_1_bss_480.y - lbl_1_bss_48C.y);
    lbl_1_bss_48C.z += 0.1f * (lbl_1_bss_480.z - lbl_1_bss_48C.z);
    if (frandf() < 0.01f) {
        dayF = GwSystem.curTime == 0;
        lbl_1_bss_480.x = (100.0f * (8.0f * frandf())) + (dayF ? 100.0f : 800.0f);
        lbl_1_bss_480.y = 0.0f;
        lbl_1_bss_480.z = 0.0f;
    }
    }
}

void fn_1_139E8(BOOL flag)
{
    lbl_1_bss_EB0 = flag;
}

void fn_1_139F8(void)
{
    int i;
    int a;
    int b;
    u8 temp;

    lbl_1_bss_1618->starIdx = 0;
    lbl_1_bss_1618->starNum = mbMasuMAttrListGet(0x80, NULL);
    for (i = 0; i < lbl_1_bss_1618->starNum; i++) {
        lbl_1_bss_1618->starOrder[i] = i;
    }
    for (i = 0; i < 100; i++) {
        a = mbRandMod(lbl_1_bss_1618->starNum);
        b = mbRandMod(lbl_1_bss_1618->starNum);
        temp = lbl_1_bss_1618->starOrder[a];
        lbl_1_bss_1618->starOrder[a] = lbl_1_bss_1618->starOrder[b];
        lbl_1_bss_1618->starOrder[b] = temp;
    }
}

void fn_1_13B38(void)
{
    int num;
    s16 masuList[8];

    num = mbMasuMAttrListGet(0x80, masuList);
    lbl_1_bss_1616 = masuList[lbl_1_bss_1618->starOrder[lbl_1_bss_1618->starIdx]];
    mbMasuTypeSet(lbl_1_bss_1616, 7);
    mbStarMasuNextSet(lbl_1_bss_1616);
}

void fn_1_13BC4(void)
{
    HuVecF pos;

    mbMasuPosGet(lbl_1_bss_1616, &pos);
    mbMasuTypeSet(lbl_1_bss_1616, 1);
    lbl_1_bss_1618->starIdx = (lbl_1_bss_1618->starIdx + 1) % lbl_1_bss_1618->starNum;
    fn_1_13B38();
}

s16 fn_1_13CBC(int playerNo)
{
    return mbMasuFind_TypeIdGet(GwPlayer[playerNo].masuId, 7, TRUE, TRUE);
}

void fn_1_13D04(void)
{
    W01_SPARK *spark;
    int i;
    MBOBJID objId;

    spark = lbl_1_bss_0;
    for (i = 0; i < 32; i++, spark++) {
        objId = mbObjCreate(mbBoardDataNumGet(0x50004), NULL, TRUE);
        spark->objId = objId;
        spark->activeF = FALSE;
        mbObjDispSet(spark->objId, FALSE);
    }
}

void fn_1_13D8C(HuVecF *pos, int num)
{
    W01_SPARK *spark;
    int i;
    int j;
    float angle;

    spark = lbl_1_bss_0;
    for (j = 0; j < num; j++) {
        for (i = 0; i < 32; i++, spark++) {
            if (!spark->activeF) {
                break;
            }
        }
        if (i >= 32) {
            break;
        }
        spark->activeF = TRUE;
        spark->pos = *pos;
        angle = mbRandMod(360);
        spark->vel.x = 100.0f * (3.0f * mbCosDeg(angle));
        spark->vel.y = 2800.0f + (100.0f * (2.0f * frandf()));
        spark->vel.z = 100.0f * (3.0f * mbSinDeg(angle));
        mbObjRotYSet(spark->objId, mbRandMod(360));
        mbObjAlphaSet(spark->objId, 255);
        spark->time = 0;
        spark->maxTime = 42;
    }
}

void fn_1_13F50(void)
{
    W01_SPARK *spark;
    int i;
    float alpha;

    spark = lbl_1_bss_0;
    for (i = 0; i < 32; i++, spark++) {
        mbObjDispSet(spark->objId, spark->activeF);
        if (spark->activeF) {
            spark->vel.y += -9800.0f * (1.0f / 60.0f);
            spark->pos.x += (1.0f / 60.0f) * spark->vel.x;
            spark->pos.y += (1.0f / 60.0f) * spark->vel.y;
            spark->pos.z += (1.0f / 60.0f) * spark->vel.z;
            mbObjPosSetV(spark->objId, &spark->pos);
            mbObjRotYSet(spark->objId, 10.0f + mbObjRotYGet(spark->objId));
            if (spark->maxTime - spark->time < 5) {
                alpha = (spark->maxTime - spark->time) / 5.0f;
                mbObjAlphaSet(spark->objId, 255.0f * alpha);
            }
            spark->time++;
            if (spark->time > spark->maxTime) {
                spark->activeF = FALSE;
            }
        }
    }
}

float fn_1_14108(W01_CURVEFUNC func, HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float t)
{
    int n;
    float start;
    float h;
    float x;
    float sum;
    int i;

    n = 10;
    start = 0.0f;
    h = (t - start) / n;
    x = start;
    sum = 0.0f;
    for (i = 0; i < n - 1; i++) {
        x += h;
        sum += func(a, b, c, d, x);
    }
    sum = 0.5 * h * (2.0 * sum + (func(a, b, c, d, start) + func(a, b, c, d, t)));
    return sum;
}

float fn_1_142B0(W01_CURVEFUNC func, HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float t)
{
    int n;
    float start;
    float result;
    float h;
    float trap;
    int i;
    float sum;
    int k;

    n = 10;
    start = 0.0f;
    result = 0.0f;
    h = t - start;
    trap = 0.5f * (h * (func(a, b, c, d, start) + func(a, b, c, d, t)));
    for (k = 1; k <= n; k *= 2) {
        sum = 0.0f;
        for (i = 1; i <= k; i++) {
            sum += func(a, b, c, d, start + h * (i - 0.5f));
        }
        sum *= h;
        result = 0.33333334f * (trap + 2.0f * sum);
        h *= 0.5f;
        trap = 0.5f * (trap + sum);
    }
    return result;
}

float fn_1_144C0(W01_CURVEFUNC func, HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float t, float len, int maxIter)
{
    float eps;
    int i;
    float diff;
    float speed;
    float prev;

    eps = 0.1f;
    i = 0;
    do {
        diff = fn_1_142B0(func, a, b, c, d, t) - len;
        speed = func(a, b, c, d, t);
        if (fabsf2(speed) < eps) {
            speed = 1.0f;
        }
        prev = t;
        t -= diff / speed;
        i++;
    } while (t != prev && i < maxIter);
    return t;
}

float fn_1_147DC(W01_CURVEFUNC func, HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float t, float len, int maxIter)
{
    float eps;
    int i;
    float diff;
    float speed;
    float prev;

    eps = 0.1f;
    i = 0;
    do {
        diff = fn_1_14108(func, a, b, c, d, t) - len;
        speed = func(a, b, c, d, t);
        if (fabsf2(speed) < eps) {
            speed = 1.0f;
        }
        prev = t;
        t -= diff / speed;
        i++;
    } while (t != prev && i < maxIter);
    return t;
}

float fn_1_14A90(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float t)
{
    HuVecF slope;

    slope.x = mbBezierCalcSlope(a->x, b->x, c->x, t);
    slope.y = mbBezierCalcSlope(a->y, b->y, c->y, t);
    slope.z = mbBezierCalcSlope(a->z, b->z, c->z, t);
    return PSVECMag(&slope);
}

float fn_1_14B34(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float t)
{
    HuVecF slope;

    slope.x = fn_1_14D34(a->x, b->x, c->x, d->x, t);
    slope.y = fn_1_14D34(a->y, b->y, c->y, d->y, t);
    slope.z = fn_1_14D34(a->z, b->z, c->z, d->z, t);
    return PSVECMag(&slope);
}

float fn_1_14BF0(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float t)
{
    HuVecF slope;

    slope.x = mbHermiteCalcSlope(a->x, b->x, c->x, d->x, t);
    slope.y = mbHermiteCalcSlope(a->y, b->y, c->y, d->y, t);
    slope.z = mbHermiteCalcSlope(a->z, b->z, c->z, d->z, t);
    return PSVECMag(&slope);
}

float fn_1_14CAC(float a, float b, float c, float d, float t)
{
    float invTime = 1.0f - t;
    return (d * (t * (t * t))) + ((c * (invTime * ((3.0f * t) * t))) + ((a * (invTime * (invTime * invTime))) + (b * (invTime * ((3.0f * t) * invTime)))));
}

float fn_1_14D34(float a, float b, float c, float d, float t)
{
    float tt = t * t;
    return ((3.0f * tt) * d) + ((c * ((-9.0f * tt) + (6.0f * t))) + ((a * (((-3.0f * tt) - (6.0f * t)) - 3.0f)) + (b * (3.0f + ((9.0f * tt) - (12.0f * t))))));
}

void fn_1_14E0C(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, HuVecF *dst, float t)
{
    dst->x = fn_1_14CAC(a->x, b->x, c->x, d->x, t);
    dst->y = fn_1_14CAC(a->y, b->y, c->y, d->y, t);
    dst->z = fn_1_14CAC(a->z, b->z, c->z, d->z, t);
}

void fn_1_150CC(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, HuVecF *dst, float t)
{
    dst->x = fn_1_14D34(a->x, b->x, c->x, d->x, t);
    dst->y = fn_1_14D34(a->y, b->y, c->y, d->y, t);
    dst->z = fn_1_14D34(a->z, b->z, c->z, d->z, t);
}

void fn_1_1547C(HuVecF *src, int num, HuVecF *dst, float t)
{
    HuVecF *buf;
    HuVecF *work;
    int j;
    int i;

    buf = HuMemDirectMallocNum(HEAP_HEAP, num * sizeof(HuVecF), HU_MEMNUM_OVL);
    work = buf;
    memcpy(work, src, num * sizeof(HuVecF));
    for (j = 1; j < num; j++) {
        for (i = 0; i < num - j; i++) {
            work[i].x = work[i].x + (t * (work[i + 1].x - work[i].x));
            work[i].y = work[i].y + (t * (work[i + 1].y - work[i].y));
            work[i].z = work[i].z + (t * (work[i + 1].z - work[i].z));
        }
    }
    *dst = work[0];
    HuMemDirectFree(work);
}

float fn_1_155EC(int i, int k, float t)
{
    float a;
    float b;
    float d;

    if (k == 0) {
        if (t >= lbl_1_bss_1620[i] && t < lbl_1_bss_1620[i + 1]) {
            return 1.0f;
        } else {
            return 0.0f;
        }
    }
    d = lbl_1_bss_1620[i + k] - lbl_1_bss_1620[i];
    if (d > 0.0f) {
        a = ((t - lbl_1_bss_1620[i]) * fn_1_155EC(i, k - 1, t)) / d;
    } else {
        a = 0.0f;
    }
    d = lbl_1_bss_1620[i + k + 1] - lbl_1_bss_1620[i + 1];
    if (d > 0.0f) {
        b = ((lbl_1_bss_1620[i + k + 1] - t) * fn_1_155EC(i + 1, k - 1, t)) / d;
    } else {
        b = 0.0f;
    }
    return a + b;
}

void fn_1_15B60(HuVecF *ctrl, int num, HuVecF *dst, float t)
{
    int n;
    int degree;
    int i;
    float *knotFree;
    float *knot;
    float w;
    HuVecF pos;

    degree = 3;
    if (t < 0.0f) {
        *dst = ctrl[0];
        return;
    }
    if (t >= 1.0f) {
        *dst = ctrl[num - 1];
        return;
    }
    knot = HuMemDirectMallocNum(HEAP_HEAP, (num + degree + 1) * sizeof(float), HU_MEMNUM_OVL);
    lbl_1_bss_1620 = knot;
    n = 0;
    for (i = 0; i <= degree; i++) {
        lbl_1_bss_1620[n++] = 0.0f;
    }
    for (i = 0; i < num - degree - 1; i++) {
        lbl_1_bss_1620[n++] = (float)(i + 1) / (num - degree);
    }
    for (i = 0; i <= degree; i++) {
        lbl_1_bss_1620[n++] = 1.0f;
    }
    pos.x = pos.y = pos.z = 0.0f;
    for (i = 0; i < num; i++) {
        w = fn_1_155EC(i, degree, t);
        pos.x += w * ctrl[i].x;
        pos.y += w * ctrl[i].y;
        pos.z += w * ctrl[i].z;
    }
    knotFree = lbl_1_bss_1620;
    HuMemDirectFree(knotFree);
    *dst = pos;
}

void fn_1_15E1C(HuVecF *list, int idx, int num, HuVecF *p0, HuVecF *p1, HuVecF *p2, HuVecF *p3)
{
    HuVecF *p;
    HuVecF a;
    HuVecF b;
    HuVecF c;

    if (idx > num - 1) {
        idx = num - 1;
    }
    p = &list[idx];
    if (idx == num - 1) {
        a = p[0];
        b = a;
        c = a;
    } else if (idx == num - 2) {
        a = p[1];
        b = a;
        c = a;
    } else if (idx == num - 3) {
        a = p[1];
        b = p[2];
        c = b;
    } else {
        a = p[1];
        b = p[2];
        c = p[3];
    }
    *p0 = p[0];
    *p1 = a;
    *p2 = b;
    *p3 = c;
}

void fn_1_15FF8(HuVecF *list, int idx, int num, HuVecF *p0, HuVecF *p1, HuVecF *t0, HuVecF *t1)
{
    HuVecF *p;
    HuVecF tan0;
    HuVecF tan1;

    if (idx > num - 1) {
        idx = num - 1;
    }
    p = &list[idx];
    if (idx == 0) {
        PSVECSubtract(&p[1], &p[0], &tan0);
    } else {
        PSVECSubtract(&p[1], &p[-1], &tan0);
    }
    if (idx == num - 2) {
        PSVECSubtract(&p[1], &p[0], &tan1);
    } else {
        PSVECSubtract(&p[2], &p[0], &tan1);
    }
    PSVECScale(&tan0, &tan0, 0.5f);
    PSVECScale(&tan1, &tan1, 0.5f);
    *p0 = p[0];
    *p1 = p[1];
    *t0 = tan0;
    *t1 = tan1;
}

void fn_1_16148(HuVecF *a, HuVecF *b, HuVecF *dst, float t)
{
    HuVecF diff;

    PSVECSubtract(b, a, &diff);
    PSVECScale(&diff, &diff, t);
    PSVECAdd(a, &diff, dst);
}

void fn_1_161AC(int playerNo, HuVecF *target, float rotY, float height, int time)
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

void fn_1_1644C(void)
{
    lbl_1_bss_161C = OSGetTick();
}

OSTick fn_1_16478(void)
{
    return OSGetTick() - lbl_1_bss_161C;
}
/* @@END@@ */
