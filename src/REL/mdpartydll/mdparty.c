/* @@HEAD@@ */
#include "REL/mdpartydll.h"
#include "game/armem.h"
/* Retail calls these unprototyped (extsh on s16 arguments) */
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
#include "game/board/board.h"
#define CharMotionAMemPGet CharMotionAMemPGet_proto
#include "game/charman.h"
#undef CharMotionAMemPGet
/* compared with li 0 + cmplw: retail sees a pointer result */
void *CharMotionAMemPGet(s16 charNo);
#include "game/data.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/memory.h"
#include "game/pad.h"
#include "game/printfunc.h"
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
ANIMDATA *lbl_1_bss_B40[9];
float lbl_1_bss_AF0[20];
s16 lbl_1_bss_AEC;
s16 lbl_1_bss_AE4[4];
s16 lbl_1_bss_AE2;
s16 lbl_1_bss_ADA[4];
s16 lbl_1_bss_AD6[2];
s16 lbl_1_bss_AD2[2];
MDPARTY_WORKA96 lbl_1_bss_A96[6];
s16 lbl_1_bss_A94;
void *lbl_1_bss_A90;
ANIMDATA *lbl_1_bss_A8C;
ANIMDATA *lbl_1_bss_A88;
s32 lbl_1_bss_A84;
OMOBJMAN *lbl_1_bss_A80;
s32 lbl_1_bss_A6C[5];
HU3D_LIGHTID lbl_1_bss_A68[2];
HUWINID lbl_1_bss_A60[4];
MDPARTY_CAMERA lbl_1_bss_A10;
MDPARTY_RULE lbl_1_bss_9F4;
MDPARTY_PLAYER lbl_1_bss_9BC[4];
ANIMDATA *lbl_1_bss_93C[32];
HUSPR_GROUPID lbl_1_bss_918[18];
HUSPRID lbl_1_bss_8A4[58];
ANIMDATA *lbl_1_bss_800[41];
MDPARTY_MODEL lbl_1_bss_288[25];
MDPARTY_PATH lbl_1_bss_228[2];
MDPARTY_PATH lbl_1_bss_1C8[2];
MDPARTY_PATH lbl_1_bss_108[4];
HUWINID lbl_1_bss_104[2];
HuVecF lbl_1_bss_D4[4];
HuVecF lbl_1_bss_A4[4];
HuVecF lbl_1_bss_8C[2];
float lbl_1_bss_80[3];
HuVecF lbl_1_bss_50[4];
float lbl_1_bss_38[6];
s16 lbl_1_bss_34;
OMOBJ *lbl_1_bss_30;
s16 lbl_1_bss_2E;
s16 lbl_1_bss_2C;
s16 lbl_1_bss_2A;
s16 lbl_1_bss_28;
OMOBJ *lbl_1_bss_24;
OMOBJ *lbl_1_bss_1C[2];
OMOBJ *lbl_1_bss_18;
OMOBJ *lbl_1_bss_14;
OMOBJ *lbl_1_bss_10;
OMOBJ *lbl_1_bss_C;
OMOBJ *lbl_1_bss_8;
OMOBJ *lbl_1_bss_4;
OMOBJMAN *lbl_1_bss_0;

s32 lbl_1_data_0 = -1;
s16 lbl_1_data_4[3][4] = { { 0, 1, 2, 3 }, { 0, 2, 1, 3 }, { 0, 3, 1, 2 } };
s32 lbl_1_data_1C[32] = {
    DATANUM(DATA_mdparty, 28), DATANUM(DATA_mdparty, 4), DATANUM(DATA_mdparty, 5), DATANUM(DATA_mdparty, 6),
    DATANUM(DATA_mdparty, 7), DATANUM(DATA_mdparty, 8), DATANUM(DATA_mdparty, 9), DATANUM(DATA_mdparty, 10),
    DATANUM(DATA_mdparty, 11), DATANUM(DATA_mdparty, 12), DATANUM(DATA_mdparty, 13), DATANUM(DATA_mdparty, 14),
    DATANUM(DATA_mdparty, 15), DATANUM(DATA_mdparty, 16), DATANUM(DATA_mdparty, 17), DATANUM(DATA_mdparty, 30),
    DATANUM(DATA_mdparty, 31), DATANUM(DATA_mdparty, 32), DATANUM(DATA_mdparty, 33), DATANUM(DATA_mdparty, 34),
    DATANUM(DATA_mdparty, 35), DATANUM(DATA_mdparty, 27), DATANUM(DATA_mdparty, 21), DATANUM(DATA_mdparty, 20),
    DATANUM(DATA_mdparty, 22), DATANUM(DATA_mdparty, 23), DATANUM(DATA_mdparty, 18), DATANUM(DATA_mdparty, 19),
    DATANUM(DATA_mdparty, 24), DATANUM(DATA_mdparty, 25), DATANUM(DATA_mdparty, 78), DATANUM(DATA_mdparty, 29),
};
s16 lbl_1_data_9C[18] = { 2, 6, 6, 4, 2, 2, 2, 2, 4, 4, 4, 4, 5, 2, 4, 4, 1, 0 };
MDPARTY_SPRDATA lbl_1_data_C0[58] = {
    { 0, 0, 0, 10, 0, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 1, 0, 15, 0, 0, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 1, 1, 16, 0, 0, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 1, 2, 17, 0, 0, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 1, 3, 18, 0, 0, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 1, 4, 19, 0, 0, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 1, 5, 20, 0, 0, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 2, 0, 21, 0, 0, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 2, 1, 21, 0, 2, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 2, 2, 21, 0, 5, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 2, 3, 21, 0, 7, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 2, 4, 21, 0, 4, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 2, 5, 21, 0, 6, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 3, 0, 22, 0, 0, 0.0f, 0.0f, 0.8f, 0.8f, 0.0f },
    { 3, 1, 22, 0, 0, 0.0f, 0.0f, 0.8f, 0.8f, 0.0f },
    { 3, 2, 22, 0, 0, 0.0f, 0.0f, 0.8f, 0.8f, 0.0f },
    { 3, 3, 22, 0, 0, 0.0f, 0.0f, 0.8f, 0.8f, 0.0f },
    { 4, 0, 22, 9, 0, 22.0f, -24.0f, 1.0f, 1.0f, 0.0f },
    { 4, 1, 23, 10, 0, 22.0f, -24.0f, 1.0f, 1.0f, 0.0f },
    { 5, 0, 22, 9, 0, 22.0f, -24.0f, 1.0f, 1.0f, 0.0f },
    { 5, 1, 23, 10, 0, 22.0f, -24.0f, 1.0f, 1.0f, 0.0f },
    { 6, 0, 22, 9, 0, 22.0f, -24.0f, 1.0f, 1.0f, 0.0f },
    { 6, 1, 23, 10, 0, 22.0f, -24.0f, 1.0f, 1.0f, 0.0f },
    { 7, 0, 22, 9, 0, 22.0f, -24.0f, 1.0f, 1.0f, 0.0f },
    { 7, 1, 23, 10, 0, 22.0f, -24.0f, 1.0f, 1.0f, 0.0f },
    { 8, 0, 25, 9, 0, -44.0f, 24.0f, 1.0f, 1.0f, 0.0f },
    { 8, 1, 24, 9, 10, -25.0f, 25.0f, 0.8f, 0.8f, 0.0f },
    { 8, 2, 24, 9, 0, -4.0f, 24.0f, 0.9f, 0.9f, 0.0f },
    { 8, 3, 23, 10, 1, -22.0f, 24.0f, 1.0f, 1.0f, 0.0f },
    { 9, 0, 25, 9, 0, -44.0f, 24.0f, 1.0f, 1.0f, 0.0f },
    { 9, 1, 24, 9, 10, -25.0f, 25.0f, 0.8f, 0.8f, 0.0f },
    { 9, 2, 24, 9, 0, -4.0f, 24.0f, 0.9f, 0.9f, 0.0f },
    { 9, 3, 23, 10, 1, -22.0f, 24.0f, 1.0f, 1.0f, 0.0f },
    { 10, 0, 25, 9, 0, -44.0f, 24.0f, 1.0f, 1.0f, 0.0f },
    { 10, 1, 24, 9, 10, -25.0f, 25.0f, 0.8f, 0.8f, 0.0f },
    { 10, 2, 24, 9, 0, -4.0f, 24.0f, 0.9f, 0.9f, 0.0f },
    { 10, 3, 23, 10, 1, -22.0f, 24.0f, 1.0f, 1.0f, 0.0f },
    { 11, 0, 25, 9, 0, -44.0f, 24.0f, 1.0f, 1.0f, 0.0f },
    { 11, 1, 24, 9, 10, -25.0f, 25.0f, 0.8f, 0.8f, 0.0f },
    { 11, 2, 24, 9, 0, -4.0f, 24.0f, 0.9f, 0.9f, 0.0f },
    { 11, 3, 23, 10, 1, -22.0f, 24.0f, 1.0f, 1.0f, 0.0f },
    { 12, 0, 27, 9, 0, -35.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 12, 1, 27, 9, 0, 5.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 12, 2, 27, 9, 0, 85.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 12, 3, 27, 9, 0, 125.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 12, 4, 26, 10, 0, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 13, 0, 28, 10, 0, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 13, 1, 29, 10, 0, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 14, 0, 25, 9, 0, -22.0f, 24.0f, 1.0f, 1.0f, 0.0f },
    { 14, 1, 24, 9, 10, -3.0f, 25.0f, 0.8f, 0.8f, 0.0f },
    { 14, 2, 24, 9, 0, 18.0f, 24.0f, 0.9f, 0.9f, 0.0f },
    { 14, 3, 23, 10, 2, 0.0f, 24.0f, 1.0f, 1.0f, 0.0f },
    { 15, 0, 25, 9, 0, -22.0f, 24.0f, 1.0f, 1.0f, 0.0f },
    { 15, 1, 24, 9, 10, -3.0f, 25.0f, 0.8f, 0.8f, 0.0f },
    { 15, 2, 24, 9, 0, 18.0f, 24.0f, 0.9f, 0.9f, 0.0f },
    { 15, 3, 23, 10, 2, 0.0f, 24.0f, 1.0f, 1.0f, 0.0f },
    { 16, 0, 30, 0, 0, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    { 0, 1, 31, 0, 0, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
};
s32 lbl_1_data_800[41] = {
    DATANUM(DATA_mdparty, 39), DATANUM(DATA_mdparty, 40), DATANUM(DATA_mdparty, 41), DATANUM(DATA_mdparty, 42),
    DATANUM(DATA_mdparty, 43), DATANUM(DATA_mdparty, 44), DATANUM(DATA_mdparty, 45), DATANUM(DATA_mdparty, 46),
    DATANUM(DATA_mdparty, 47), DATANUM(DATA_mdparty, 48), DATANUM(DATA_mdparty, 49), DATANUM(DATA_mdparty, 50),
    DATANUM(DATA_mdparty, 51), DATANUM(DATA_mdparty, 52), DATANUM(DATA_mdparty, 53), DATANUM(DATA_mdparty, 54),
    DATANUM(DATA_mdparty, 55), DATANUM(DATA_mdparty, 56), DATANUM(DATA_mdparty, 37), DATANUM(DATA_mdparty, 38),
    DATANUM(DATA_mdparty, 57), DATANUM(DATA_mdparty, 58), DATANUM(DATA_mdparty, 59), DATANUM(DATA_mdparty, 60),
    DATANUM(DATA_mdparty, 61), DATANUM(DATA_mdparty, 62), DATANUM(DATA_mdparty, 63), DATANUM(DATA_mdparty, 64),
    DATANUM(DATA_mdparty, 65), DATANUM(DATA_mdparty, 66), DATANUM(DATA_mdparty, 67), DATANUM(DATA_mdparty, 68),
    DATANUM(DATA_mdparty, 69), DATANUM(DATA_mdparty, 70), DATANUM(DATA_mdparty, 71), DATANUM(DATA_mdparty, 72),
    DATANUM(DATA_mdparty, 73), DATANUM(DATA_mdparty, 74), DATANUM(DATA_mdparty, 75), DATANUM(DATA_mdparty, 76),
    DATANUM(DATA_mdparty, 77),
};
HuVecF lbl_1_data_8A4[67] = {
    { 0.0f, 275.0f, 0.0f },
    { -200.0f, 325.0f, 0.0f },
    { -120.0f, 225.0f, 0.0f },
    { -40.0f, 325.0f, 0.0f },
    { 40.0f, 225.0f, 0.0f },
    { 120.0f, 325.0f, 0.0f },
    { 200.0f, 225.0f, 0.0f },
    { -200.0f, 275.0f, 0.0f },
    { -120.0f, 275.0f, 0.0f },
    { -40.0f, 275.0f, 0.0f },
    { 40.0f, 275.0f, 0.0f },
    { 120.0f, 275.0f, 0.0f },
    { 200.0f, 275.0f, 0.0f },
    { 0.0f, 275.0f, -800.0f },
    { -195.0f, 380.0f, 0.0f },
    { -65.0f, 380.0f, 0.0f },
    { 65.0f, 380.0f, 0.0f },
    { 195.0f, 380.0f, 0.0f },
    { -130.0f, 240.0f, 0.0f },
    { 0.0f, 240.0f, 0.0f },
    { 130.0f, 240.0f, 0.0f },
    { -195.0f, 100.0f, 0.0f },
    { -65.0f, 100.0f, 0.0f },
    { 65.0f, 100.0f, 0.0f },
    { 195.0f, 100.0f, 0.0f },
    { -180.0f, 65.0f, 0.0f },
    { -60.0f, 65.0f, 0.0f },
    { 60.0f, 65.0f, 0.0f },
    { 180.0f, 65.0f, 0.0f },
    { -180.0f, 125.0f, 0.0f },
    { -60.0f, 125.0f, 0.0f },
    { 60.0f, 125.0f, 0.0f },
    { 180.0f, 125.0f, 0.0f },
    { -180.0f, 275.0f, 0.0f },
    { -60.0f, 275.0f, 0.0f },
    { 60.0f, 275.0f, 0.0f },
    { 180.0f, 275.0f, 0.0f },
    { -340.0f, 375.0f, 0.0f },
    { -340.0f, 240.0f, 0.0f },
    { 340.0f, 375.0f, 0.0f },
    { 340.0f, 240.0f, 0.0f },
    { -225.0f, 300.0f, 0.0f },
    { -75.0f, 300.0f, 0.0f },
    { 75.0f, 300.0f, 0.0f },
    { 225.0f, 300.0f, 0.0f },
    { -180.0f, 175.0f, 0.0f },
    { -60.0f, 175.0f, 0.0f },
    { 60.0f, 175.0f, 0.0f },
    { 180.0f, 175.0f, 0.0f },
    { -220.0f, 175.0f, 0.0f },
    { -100.0f, 175.0f, 0.0f },
    { 100.0f, 175.0f, 0.0f },
    { 220.0f, 175.0f, 0.0f },
    { -220.0f, 65.0f, 0.0f },
    { -100.0f, 65.0f, 0.0f },
    { 100.0f, 65.0f, 0.0f },
    { 220.0f, 65.0f, 0.0f },
    { -220.0f, 125.0f, 0.0f },
    { -100.0f, 125.0f, 0.0f },
    { 100.0f, 125.0f, 0.0f },
    { 220.0f, 125.0f, 0.0f },
    { -160.0f, 0.0f, 0.0f },
    { 160.0f, 0.0f, 0.0f },
    { -225.0f, 275.0f, 0.0f },
    { -75.0f, 275.0f, 0.0f },
    { 75.0f, 275.0f, 0.0f },
    { 225.0f, 275.0f, 0.0f },
};
s32 lbl_1_data_BC8 = -1;

/* .data placement: lbl_1_data_BEE/_BFC and _DA2/_DA8 are defined in PART1 (after the strings
   before them); later parts define lbl_1_data_DE6, _E12 and _F10 (see REL/mdpartydll.h). */

/* @@PART1@@ */
void MdPartyWinCallback(HUWINID winId, u32 mess, s16 no)
{
    s32 messTbl[3] = { 0xD0000, 0xD0025, -1 };
    s32 seTbl[16] = {
        0x3B5, 0x3B6, 0x3B7, 0x3B8, 0x3B9, 0x3BA, 0x3BB, -1,
        0x3AD, 0x3AE, 0x3AF, 0x3B0, 0x3B1, 0x3B2, 0x3B3, -1
    };
    s16 i;
    no--;
    OSReport("# ========== win callback :: %d\n", no);
    if (lbl_1_data_BC8 != mess) {
        lbl_1_data_BC8 = mess;
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

s16 lbl_1_data_BEE[7] = { 0 };

void MdPartyUnlockInit(void)
{
    lbl_1_bss_2A = 0;
    lbl_1_bss_2C = 0;
    if (GWBankFlagGet(2)) {
        lbl_1_bss_2A = 1;
    }
    if (GWBankFlagGet(3)) {
        lbl_1_bss_2C = 1;
    }
    lbl_1_data_BEE[0] = 1;
    lbl_1_data_BEE[1] = 1;
    lbl_1_data_BEE[2] = 1;
    lbl_1_data_BEE[3] = 1;
    lbl_1_data_BEE[4] = 1;
    lbl_1_data_BEE[5] = 0;
    if (GWBankFlagGet(0x33)) {
        lbl_1_data_BEE[5] = 1;
    }
}

void MdPartyCharDataLoad(void)
{
    s16 i;
    s32 statId;
    for (i = 0; i < 4; i++) {
        if (CharMotionAMemPGet(GwPlayer[i].charNo) == NULL) {
            break;
        }
    }
    if (i != 4) {
        CharDataClose(-1);
        for (i = 0; i < 4; i++) {
            statId = HuDataDirReadAsync(CharDataDirTbl[GwPlayer[i].charNo][4]);
            if (statId != -1) {
                while (HuDataGetAsyncStat(statId) == 0) {
                    HuPrcVSleep();
                }
            }
            CharMotionInit(GwPlayer[i].charNo);
            HuDataDirClose(CharDataDirTbl[GwPlayer[i].charNo][4]);
        }
    }
}

s32 lbl_1_data_BFC[14] = {
    DATANUM(DATA_w01, 0), DATANUM(DATA_w02, 0), DATANUM(DATA_w03, 0), DATANUM(DATA_w04, 0),
    DATANUM(DATA_w05, 0), DATANUM(DATA_w06, 0), DATANUM(DATA_w10, 0),
    DATANUM(DATA_w01n, 0), DATANUM(DATA_w02n, 0), DATANUM(DATA_w03n, 0), DATANUM(DATA_w04n, 0),
    DATANUM(DATA_w05n, 0), DATANUM(DATA_w06n, 0), DATANUM(DATA_w10n, 0)
};

void MdPartyReadProc(void)
{
    s32 dirTbl[2] = { DATA_board_us, DATA_capsule };
    s16 i;
    s32 statId;
    MdPartyCharDataLoad();
    for (i = 0; i < 2; i++) {
        statId = HuDataDirReadAsync(dirTbl[i]);
        if (statId != -1) {
            while (HuDataGetAsyncStat(statId) == 0) {
                HuPrcVSleep();
            }
        }
        HuAR_MRAMtoARAM(dirTbl[i]);
        while (HuARDMACheck()) {
            HuPrcVSleep();
        }
        HuDataDirClose(dirTbl[i]);
    }
    statId = HuDataDirReadAsync(lbl_1_data_BFC[GwSystem.boardNo]);
    if (statId != -1) {
        while (HuDataGetAsyncStat(statId) == 0) {
            HuPrcVSleep();
        }
    }
    lbl_1_bss_2E = 1;
    HuPrcEnd();
    while (1) {
        HuPrcVSleep();
    }
}

void MdPartyReadInit(void)
{
    lbl_1_bss_2E = 0;
    OSReport("\n>>>>>>>>>> mdpartydll :: READ INIT!! <<<<<<<<<<\n");
    OSReport("0x%x :: _effect\n", _effect);
    OSReport("0x%x :: _gamemes\n", _gamemes);
    OSReport("0x%x :: _mgconst\n", _mgconst);
    OSReport("0x%x :: _win\n", _win);
    HuAMemDump();
    OSReport("\n");
    HuDataDirClose(DATA_mdparty);
    HuPrcChildCreate(MdPartyReadProc, 0x100, 0x4000, 0, lbl_1_bss_0);
}

void MdPartyGameWorkSet(void)
{
    s16 i;
    lbl_1_bss_9F4.bonusStarF++;
    lbl_1_bss_9F4.bonusStarF %= 2;
    GwSystem.tagF = lbl_1_bss_9F4.tagF;
    GwSystem.bonusStarF = lbl_1_bss_9F4.bonusStarF;
    GwSystem.mgPack = lbl_1_bss_9F4.mgPack;
    GwSystem.turnNo = 1;
    GwSystem.turnMax = lbl_1_bss_9F4.turnNo * 5 + 10;
    GwSystem.boardNo = lbl_1_bss_9F4.boardNo;
    GwCommon.partyMgPack = GwSystem.mgPack;
    GwCommon.confTag = lbl_1_bss_9F4.tagF;
    GwCommon.confTurnNum = lbl_1_bss_9F4.turnNo * 5 + 10;
    GwCommon.partyMgPack = lbl_1_bss_9F4.mgPack;
    GwCommon.confBonusStar = lbl_1_bss_9F4.bonusStarF;
    GwCommon.lastBoard = lbl_1_bss_9F4.boardNo;
    for (i = 0; i < 4; i++) {
        GwPlayer[i].comF = lbl_1_bss_9BC[i].com;
        GwPlayer[i].comDif = lbl_1_bss_9BC[i].comDif;
        GwPlayer[i].charNo = lbl_1_bss_9BC[i].charNo;
        GwPlayer[i].padNo = lbl_1_bss_9BC[i].padNo;
        GwPlayer[i].handicap = lbl_1_bss_9BC[i].handicap;
        GwPlayer[i].team = 0;
    }
    if (GwSystem.tagF) {
        for (i = 0; i < 4; i++) {
            GwPlayer[i].handicap = 0;
        }
        GwPlayer[lbl_1_data_4[lbl_1_bss_9F4.teamNo][0]].handicap = lbl_1_bss_9BC[0].handicap;
        GwPlayer[lbl_1_data_4[lbl_1_bss_9F4.teamNo][2]].handicap = lbl_1_bss_9BC[1].handicap;
        GwPlayer[lbl_1_data_4[lbl_1_bss_9F4.teamNo][0]].team = 0;
        GwPlayer[lbl_1_data_4[lbl_1_bss_9F4.teamNo][1]].team = 0;
        GwPlayer[lbl_1_data_4[lbl_1_bss_9F4.teamNo][2]].team = 1;
        GwPlayer[lbl_1_data_4[lbl_1_bss_9F4.teamNo][3]].team = 1;
    }
    for (i = 0; i < 4; i++) {
        GwPlayerConf[i].grpNo = 0;
        GwPlayerConf[i].type = GwPlayer[i].comF;
        GwPlayerConf[i].comDif = GwPlayer[i].comDif;
        GwPlayerConf[i].charNo = GwPlayer[i].charNo;
        GwPlayerConf[i].padNo = GwPlayer[i].padNo;
        GwPlayerConf[i].grpNo = GwPlayer[i].team;
    }
    _ClearFlag(0x1000E);
    mbSavePartyInit(GwSystem.tagF, GwSystem.bonusStarF, GwSystem.mgPack, GwSystem.turnMax, GwPlayer[0].handicap, GwPlayer[1].handicap, GwPlayer[2].handicap, GwPlayer[3].handicap);
    mbSaveInit(GwSystem.boardNo);
}

void MdPartyGameWorkTest(void)
{
    s16 i;
    for (i = 0; i < 4; i++) {
        GwPlayer[i].comF = 1;
        GwPlayer[i].comDif = 0;
        GwPlayer[i].charNo = i;
        GwPlayer[i].padNo = i;
        GwPlayer[i].handicap = 0;
        GwPlayer[i].team = 0;
    }
    for (i = 0; i < 4; i++) {
        GwPlayerConf[i].grpNo = 0;
        GwPlayerConf[i].type = GwPlayer[i].comF;
        GwPlayerConf[i].comDif = GwPlayer[i].comDif;
        GwPlayerConf[i].charNo = GwPlayer[i].charNo;
        GwPlayerConf[i].padNo = GwPlayer[i].padNo;
        GwPlayerConf[i].grpNo = GwPlayer[i].team;
    }
    mbSaveInit(9);
    GwSystem.boardNo = 6;
}

void MdPartyOvlGoto(s16 mode)
{
    s32 ovlTbl[6] = { DLL_w01dll, DLL_w02dll, DLL_w03dll, DLL_w04dll, DLL_w05dll, DLL_w06dll };
    OMOVLHIS *his;
    OMOVLHIS *his2;
    do {
        HuPrcVSleep();
    } while (lbl_1_bss_2E == 0);
    OSReport("\n>>>>>>>>>> mdpartydll :: READ END!! <<<<<<<<<<\n");
    OSReport("0x%x :: _effect\n", _effect);
    OSReport("0x%x :: _gamemes\n", _gamemes);
    OSReport("0x%x :: _mgconst\n", _mgconst);
    OSReport("0x%x :: _win\n", _win);
    HuAMemDump();
    OSReport("\n");
    switch (mode) {
        case 0:
            his = omOvlHisGet(0);
            omOvlHisChg(0, ovlTbl[GwSystem.boardNo], his->evtno, his->stat);
            omOvlGoto(ovlTbl[GwSystem.boardNo], 0, 0);
            break;
        case 1:
            his2 = omOvlHisGet(0);
            omOvlHisChg(0, his2->ovl, 1, 0);
            omOvlCall(DLL_w10dll, 0, 0);
            break;
    }
}

void MdPartyARamFree(void)
{
    CharDataClose(-1);
    HuARDirFree(DATA_board);
    HuARDirFree(DATA_board_us);
    HuARDirFree(DATA_capsule);
    OSReport(">>>>>>>>>> mdpartydll :: objsetup!! <<<<<<<<<<\n");
    OSReport("0x%x :: _effect\n", _effect);
    OSReport("0x%x :: _gamemes\n", _gamemes);
    OSReport("0x%x :: _mgconst\n", _mgconst);
    OSReport("0x%x :: _win\n", _win);
    HuAMemDump();
    OSReport("\n");
}

void MdPartyARamFreeReturn(void)
{
    CharDataClose(-1);
    HuARDirFree(DATA_board);
    HuARDirFree(DATA_board_us);
    HuARDirFree(DATA_capsule);
    OSReport("\n>>>>>>>>>> mdpartydll :: ovlreturn!! <<<<<<<<<<\n");
    OSReport("0x%x :: _effect\n", _effect);
    OSReport("0x%x :: _gamemes\n", _gamemes);
    OSReport("0x%x :: _mgconst\n", _mgconst);
    OSReport("0x%x :: _win\n", _win);
    HuAMemDump();
    OSReport("\n");
}

float MdPartyBezier(float a, float b, float c, float t)
{
    float u = 1.0f - t;
    return c * (t * t) + (a * (u * u) + 2.0f * (b * (u * t)));
}

void MdPartyBezierV(HuVecF *out, float t, HuVecF *a, HuVecF *b, HuVecF *c)
{
    out->x = MdPartyBezier(a->x, b->x, c->x, t);
    out->y = MdPartyBezier(a->y, b->y, c->y, t);
    out->z = MdPartyBezier(a->z, b->z, c->z, t);
}

float MdPartySmooth(float cur, float target, float speed)
{
    if (cur == target) {
        return target;
    }
    return (target + cur * (speed - 1.0f)) / speed;
}

void MdPartySmoothV(HuVecF *cur, HuVecF *target, float speed)
{
    cur->x = MdPartySmooth(cur->x, target->x, speed);
    cur->y = MdPartySmooth(cur->y, target->y, speed);
    cur->z = MdPartySmooth(cur->z, target->z, speed);
}

float MdPartySinLerp90(float start, float end, float time, float maxTime)
{
    if (time <= 0.0f) {
        return start;
    }
    if (time >= maxTime) {
        return end;
    }
    return start + (end - start) * HuSin((90.0f / maxTime) * time);
}

float MdPartyLerp(float start, float end, float time, float maxTime)
{
    if (time <= 0.0f) {
        return start;
    }
    if (time >= maxTime) {
        return end;
    }
    return start + (time / maxTime) * (end - start);
}

void MdPartyModelMoveLook(HU3D_MODELID mdlId, HuVecF *start, HuVecF *end, float time, float maxTime)
{
    HuVecF pos;
    HuVecF rot;
    HuVecF newPos;
    HuVecF newRot;
    Hu3DModelPosGet(mdlId, &pos);
    Hu3DModelRotGet(mdlId, &rot);
    newPos.x = MdPartyLerp(start->x, end->x, time, maxTime);
    newPos.y = MdPartyLerp(start->y, end->y, time, maxTime);
    newPos.z = MdPartyLerp(start->z, end->z, time, maxTime);
    pos.x -= newPos.x;
    pos.z -= newPos.z;
    newRot.y = -HuAtan(pos.x, -pos.z);
    if (rot.y - newRot.y > 180.0f) {
        rot.y -= 360.0f;
    } else if (rot.y - newRot.y < -180.0f) {
        rot.y += 360.0f;
    }
    newRot.x = rot.x;
    newRot.y = MdPartySmooth(rot.y, newRot.y, 10.0f);
    newRot.z = rot.z;
    Hu3DModelPosSetV(mdlId, &newPos);
    Hu3DModelRotSetV(mdlId, &newRot);
}

void MdPartyModelRotTurn(HU3D_MODELID mdlId, float rotY)
{
    HuVecF rot;
    HuVecF newRot;
    Hu3DModelRotGet(mdlId, &rot);
    newRot.y = rotY;
    if (rot.y - newRot.y > 180.0f) {
        rot.y -= 360.0f;
    } else if (rot.y - newRot.y < -180.0f) {
        rot.y += 360.0f;
    }
    newRot.x = rot.x;
    newRot.y = MdPartySmooth(rot.y, newRot.y, 10.0f);
    newRot.z = rot.z;
    Hu3DModelRotSetV(mdlId, &newRot);
}

void MdPartyModelMoveTurn(HU3D_MODELID mdlId, HuVecF *start, HuVecF *end, float time, float maxTime, float rotY)
{
    if (time <= maxTime) {
        MdPartyModelMoveLook(mdlId, start, end, time, maxTime);
    } else {
        MdPartyModelRotTurn(mdlId, rotY);
    }
}

float MdPartySinLerp180(float start, float end, float time, float maxTime)
{
    if (time <= 0.0f || time >= maxTime) {
        return start;
    }
    return start + (end - start) * HuSin((180.0f / maxTime) * time);
}

void MdPartyModelBezierMove(HU3D_MODELID mdlId, HuVecF *a, HuVecF *b, HuVecF *c, float time, float maxTime)
{
    float t;
    HuVecF pos;
    HuVecF ofs;
    HuVecF rot;
    t = MdPartyLerp(0.0f, 1.0f, time, maxTime);
    MdPartyBezierV(&pos, t, a, b, c);
    Hu3DModelPosGet(mdlId, &ofs);
    Hu3DModelRotGet(mdlId, &rot);
    ofs.x -= pos.x;
    ofs.z -= pos.z;
    ofs.y = -HuAtan(ofs.x, -ofs.z);
    if (rot.y - ofs.y > 180.0f) {
        rot.y -= 360.0f;
    } else if (rot.y - ofs.y < -180.0f) {
        rot.y += 360.0f;
    }
    rot.y = MdPartySmooth(rot.y, ofs.y, 10.0f);
    Hu3DModelPosSet(mdlId, pos.x, pos.y, pos.z);
    Hu3DModelRotSet(mdlId, 0.0f, rot.y, 0.0f);
}

void MdPartyPlayerInit(void)
{
    s16 i;
    MDPARTY_PLAYER *player;
    for (i = 0, player = lbl_1_bss_9BC; i < 4; i++, player++) {
        player->state = 0;
        player->unk2 = 0;
        player->comDif = 0;
        player->charNo = i;
        player->padNo = i;
    }
}

void MdPartyPlayerComSet(void)
{
    s16 num = lbl_1_bss_9F4.playerNum;
    s16 i;
    MDPARTY_PLAYER *player;
    lbl_1_bss_9BC[0].com = 0;
    num--;
    for (i = 1, player = &lbl_1_bss_9BC[1]; i < 4; i++, player++) {
        if (num > 0 && HuPadStatGet(i) == 0) {
            player->com = 0;
            num--;
        } else {
            player->com = 1;
        }
    }
}

void MdPartyCameraJump(MDPARTY_CAMERA *camera)
{
    memcpy(&camera->center, &camera->centerEnd, sizeof(HuVecF));
    memcpy(&camera->rot, &camera->rotEnd, sizeof(HuVecF));
    camera->zoom = camera->zoomEnd;
}

void MdPartyCameraStop(MDPARTY_CAMERA *camera)
{
    memcpy(&camera->centerEnd, &camera->center, sizeof(HuVecF));
    memcpy(&camera->rotEnd, &camera->rot, sizeof(HuVecF));
    camera->zoomEnd = camera->zoom;
}

void MdPartyCameraSmooth(MDPARTY_CAMERA *camera, float speed)
{
    MdPartySmoothV(&camera->center, &camera->centerEnd, speed);
    MdPartySmoothV(&camera->rot, &camera->rotEnd, speed);
    camera->zoom = MdPartySmooth(camera->zoom, camera->zoomEnd, speed);
}

void MdPartyCameraFuncSet(MDPARTY_CAMERA_FUNC func)
{
    lbl_1_bss_A10.func = func;
}

void MdPartyCameraDebug(OMOBJ *obj, MDPARTY_CAMERA *camera)
{
    float stkX = HuPadStkX[0];
    float stkY = -HuPadStkY[0];
    float subY = -HuPadSubStkY[0];
    float subX = HuPadSubStkX[0];
    float dx;
    float dz;
    camera->rotEnd.x += 0.02f * subY;
    if (camera->rotEnd.x > 85.0f) {
        camera->rotEnd.x = 85.0f;
    }
    if (camera->rotEnd.x < -85.0f) {
        camera->rotEnd.x = -85.0f;
    }
    camera->rotEnd.y += 0.02f * subX;
    if (HuPadBtn[0] & PAD_TRIGGER_L) {
        camera->centerEnd.y -= 0.25f * stkY;
    } else if (HuPadBtn[0] & PAD_TRIGGER_R) {
        camera->zoomEnd += 0.25f * stkY;
        if (camera->zoomEnd < 5.0f) {
            camera->zoomEnd = 5.0f;
        }
    } else {
        dx = stkX * HuSin(90.0f + camera->rot.y) + stkY * HuSin(camera->rot.y);
        dz = stkX * HuCos(90.0f + camera->rot.y) + stkY * HuCos(camera->rot.y);
        camera->centerEnd.x += 0.25f * dx;
        camera->centerEnd.z += 0.25f * dz;
    }
    MdPartyCameraSmooth(camera, 15.0f);
    print8(16, 110, 1.0f, "-----===== CAMERA TEST =====-----");
    print8(16, 120, 1.0f, "CENTER : %.2f, %.2f, %.2f", camera->center.x, camera->center.y, camera->center.z);
    print8(16, 130, 1.0f, "ROT    : %.2f, %.2f, %.2f", camera->rot.x, camera->rot.y, camera->rot.z);
    print8(16, 140, 1.0f, "ZOOM   : %.2f", camera->zoom);
}

s16 lbl_1_data_DA2[3] = { -1, -1, 0 };
s32 lbl_1_data_DA8[3] = { -1, -1, -1 };

void MdPartyCameraFuncCall(OMOBJ *obj, MDPARTY_CAMERA *camera)
{
    if (camera->func) {
        camera->func(obj, camera);
    }
}

void MdPartyCameraUpdate(OMOBJ *obj)
{
    MDPARTY_CAMERA *camera = &lbl_1_bss_A10;
    MdPartyCameraFuncCall(obj, camera);
    Center.x = camera->center.x;
    Center.y = camera->center.y;
    Center.z = camera->center.z;
    CRot.x = camera->rot.x;
    CRot.y = camera->rot.y;
    CRot.z = camera->rot.z;
    CZoom = camera->zoom;
    omOutView(obj);
}

void MdPartyCameraInit(MDPARTY_CAMERA_FUNC func)
{
    MDPARTY_CAMERA *camera = &lbl_1_bss_A10;
    Hu3DCameraCreate(HU3D_CAM0);
    Hu3DCameraPerspectiveSet(HU3D_CAM0, 30.0f, 10.0f, 10000.0f, 1.2f);
    Hu3DCameraViewportSet(HU3D_CAM0, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    memset(camera, 0, sizeof(MDPARTY_CAMERA));
    camera->func = func;
    camera->center.x = 0.0f;
    camera->center.y = 65.0f;
    camera->center.z = -800.0f;
    camera->rot.x = -7.25f;
    camera->rot.y = 0.0f;
    camera->rot.z = 0.0f;
    if (lbl_1_bss_28 == 0) {
        camera->zoom = 2650.0f;
    } else {
        camera->zoom = 2150.0f;
    }
    camera->obj = omAddObjEx(lbl_1_bss_0, 0x7FDA, 0, 0, -1, MdPartyCameraUpdate);
}

void MdPartyCameraKill(void)
{
    MDPARTY_CAMERA *camera = &lbl_1_bss_A10;
    Hu3DCameraKill(HU3D_CAM0);
    if (camera->obj) {
        omDelObjEx(lbl_1_bss_0, camera->obj);
    }
    camera->obj = NULL;
}

void MdPartyLightInit(void)
{
    lbl_1_bss_A68[0] = Hu3DGLightCreate(0.0f, 1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 255, 255, 255);
    Hu3DGLightInfinitytSet(lbl_1_bss_A68[0]);
    Hu3DGLightStaticSet(lbl_1_bss_A68[0], TRUE);
    lbl_1_bss_A68[1] = Hu3DGLightCreate(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 255, 255, 255);
    Hu3DGLightInfinitytSet(lbl_1_bss_A68[1]);
    Hu3DGLightStaticSet(lbl_1_bss_A68[1], TRUE);
}

void MdPartyLightKill(void)
{
    Hu3DGLightKill(lbl_1_bss_A68[0]);
    Hu3DGLightKill(lbl_1_bss_A68[1]);
}

void MdPartyWinOpen(s16 winNo)
{
    if (winNo == 0) {
        HuWinDispOn(lbl_1_bss_A60[winNo]);
    } else {
        HuWinExOpen(lbl_1_bss_A60[winNo]);
    }
}

void MdPartyWinClose(s16 winNo)
{
    if (winNo == 0) {
        HuWinDispOff(lbl_1_bss_A60[winNo]);
    } else {
        HuWinExClose(lbl_1_bss_A60[winNo]);
    }
}

void MdPartyWinWait(s16 winNo)
{
    HuWinMesWait(lbl_1_bss_A60[winNo]);
}

s16 MdPartyWinChoiceGet(s16 winNo, s16 mode)
{
    s16 choice = 0;
    if (mode == 1) {
        HuWinAttrSet(lbl_1_bss_A60[winNo], HUWIN_ATTR_NOCANCEL);
    } else {
        HuWinAttrReset(lbl_1_bss_A60[winNo], HUWIN_ATTR_NOCANCEL);
    }
    choice = HuWinChoiceGet(lbl_1_bss_A60[winNo], -1);
    if (mode == 2 && choice == -1) {
        choice = 1;
    }
    return choice;
}

/* @@PART2@@ */
/* @@END@@ */
