/* @@HEAD@@ */
#include "REL/w01Dll.h"
#include "game/audio.h"
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

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* .bss is laid out in reverse definition order */
s32 lbl_1_bss_1620;
s32 lbl_1_bss_161C;
u32 *lbl_1_bss_1618;
s16 lbl_1_bss_1616;
MBOBJID lbl_1_bss_15F4[17];
HuVecF lbl_1_bss_15E8;
W01_BSS_155C lbl_1_bss_155C;
W01_BSS_1494 lbl_1_bss_1494;
s32 lbl_1_bss_1490;
s32 lbl_1_bss_148C;
s16 lbl_1_bss_1488;
W01_BSS_1438 lbl_1_bss_1438;
s16 lbl_1_bss_1436;
s16 lbl_1_bss_1434;
HuVecF lbl_1_bss_1428;
HuVecF lbl_1_bss_141C;
HuVecF lbl_1_bss_1410;
W01_BSS_13D8 lbl_1_bss_13D8;
s32 lbl_1_bss_13D0[2];
W01_BSS_137C lbl_1_bss_137C;
s32 lbl_1_bss_1378;
W01_BSS_1338 lbl_1_bss_1338;
s32 lbl_1_bss_1334;
W01_BSS_1240 lbl_1_bss_1240;
s32 lbl_1_bss_1238[2];
s32 lbl_1_bss_1230[2];
HuVecF lbl_1_bss_1224;
s16 lbl_1_bss_1220;
s32 lbl_1_bss_121C;
s32 lbl_1_bss_1214[2];
s32 lbl_1_bss_120C[2];
s32 lbl_1_bss_1208;
W01_BSS_1188 lbl_1_bss_1188;
W01_BSS_EFC lbl_1_bss_EFC;
s32 lbl_1_bss_EF8;
W01_BSS_EE8 lbl_1_bss_EE8;
s32 lbl_1_bss_EE4;
W01_BSS_EB4 lbl_1_bss_EB4;
s32 lbl_1_bss_EB0;
W01_BSS_4B0 lbl_1_bss_4B0;
HuVecF lbl_1_bss_4A4;
s16 lbl_1_bss_4A0;
s32 lbl_1_bss_49C;
s32 lbl_1_bss_498;
HuVecF lbl_1_bss_48C;
HuVecF lbl_1_bss_480;
W01_BSS_0 lbl_1_bss_0;

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
s32 lbl_1_data_3CC[] = { DATANUM(DATA_w01, 6), DATANUM(DATA_w01, 7), DATANUM(DATA_w01, 18), DATANUM(DATA_w01, 19), DATANUM(DATA_w01, 20), 0 };
s32 lbl_1_data_3E4[] = { DATANUM(DATA_w01, 6), DATANUM(DATA_w01, 7), DATANUM(DATA_w01, 21), 0 };
s32 *lbl_1_data_3F4[] = { lbl_1_data_3CC, lbl_1_data_3E4 };
HuVecF lbl_1_data_3FC = { 0.0f, 0.0f, 1200.0f };
HuVecF lbl_1_data_408 = { -28.0f, 0.0f, 0.0f };
HuVecF lbl_1_data_414 = { 0.0f, 0.0f, 0.0f };

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

/* @@PART2@@ */
/* @@PART3@@ */
/* @@PART4@@ */
/* @@PART5@@ */
/* @@PART6@@ */
/* @@PART7@@ */
/* @@END@@ */
