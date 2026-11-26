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
MDPARTY_WAVE lbl_1_bss_AF0[2];
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
OMOBJ *lbl_1_bss_A84;
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
/* later part's function: not yet in REL/mdpartydll.h */

void MdPartyWinMesSet(s16 winNo, s32 mess, s16 speed)
{
    HuWinAttrSet(lbl_1_bss_A60[winNo], HUWIN_ATTR_ALIGN_CENTER);
    HuWinMesSet(lbl_1_bss_A60[winNo], mess);
    HuWinMesSpeedSet(lbl_1_bss_A60[winNo], speed);
    if (lbl_1_data_BC8 != mess) {
        lbl_1_data_BC8 = -1;
    }
}

void MdPartyWinInsertMesSet(s16 winNo, s32 mess, s16 insertNo)
{
    HuWinHomeClear(lbl_1_bss_A60[winNo]);
    HuWinInsertMesSet(lbl_1_bss_A60[winNo], mess, insertNo);
}

void MdPartyWinInit(void)
{
    s16 i;
    HuWinInit(1);
    lbl_1_bss_A60[0] = HuWinExCreateFrame(16.0f, 337.0f, 544, 42, -1, 0);
    HuWinDispOff(lbl_1_bss_A60[0]);
    HuWinBGTPLvlSet(lbl_1_bss_A60[0], 0.0f);
    lbl_1_bss_A60[1] = HuWinExCreateFrame(16.0f, 372.0f, 544, 68, -1, 5);
    HuWinDispOff(lbl_1_bss_A60[1]);
    HuWinBGTPLvlSet(lbl_1_bss_A60[1], 0.9f);
    lbl_1_bss_A60[2] = HuWinExCreateFrame(16.0f, 372.0f, 544, 68, -1, 3);
    HuWinDispOff(lbl_1_bss_A60[2]);
    HuWinBGTPLvlSet(lbl_1_bss_A60[2], 0.9f);
    lbl_1_bss_A60[3] = HuWinExCreateFrame(16.0f, 372.0f, 544, 68, -1, 4);
    HuWinDispOff(lbl_1_bss_A60[3]);
    HuWinBGTPLvlSet(lbl_1_bss_A60[3], 0.9f);
    for (i = 0; i < 4; i++) {
        winData[lbl_1_bss_A60[i]].padMask = HUWIN_PLAYER_1;
        HuWinCallbackSet(lbl_1_bss_A60[i], (HUWIN_CALLBACK)MdPartyWinCallback);
    }
}

void MdPartyWinKill(void)
{
    s16 i;
    for (i = 0; i < 4; i++) {
        HuWinExKill(lbl_1_bss_A60[i]);
    }
    HuWinAllKill();
}

void MdPartyMainWinOpen(s16 winNo)
{
    if (lbl_1_data_DA2[0] != -1 && lbl_1_data_DA2[0] != winNo) {
        MdPartyWinClose(lbl_1_data_DA2[0]);
    }
    if (lbl_1_data_DA2[0] == -1 || lbl_1_data_DA2[0] != winNo) {
        lbl_1_data_DA2[0] = winNo;
        lbl_1_data_DA8[0] = -1;
        lbl_1_data_DA8[1] = -1;
        MdPartyWinOpen(lbl_1_data_DA2[0]);
    }
}

void MdPartyMainWinClose(void)
{
    if (lbl_1_data_DA2[0] != -1) {
        MdPartyWinClose(lbl_1_data_DA2[0]);
    }
    lbl_1_data_DA2[0] = -1;
    lbl_1_data_DA8[0] = -1;
    lbl_1_data_DA8[1] = -1;
}

void MdPartyMainWinWait(void)
{
    if (lbl_1_data_DA2[0] != -1) {
        MdPartyWinWait(lbl_1_data_DA2[0]);
    }
}

s16 MdPartyMainWinChoiceGet(s16 mode)
{
    if (lbl_1_data_DA2[0] != -1) {
        return MdPartyWinChoiceGet(lbl_1_data_DA2[0], mode);
    }
    return 0;
}

void MdPartyMainWinMesSet(s16 winNo, s32 mess, s16 speed)
{
    MdPartyMainWinOpen(winNo);
    if (lbl_1_data_DA8[0] != mess) {
        lbl_1_data_DA8[0] = mess;
        lbl_1_data_DA8[1] = -1;
        MdPartyWinMesSet(lbl_1_data_DA2[0], lbl_1_data_DA8[0], speed);
    }
}

void MdPartyMainWinInsertMesSet(s16 winNo, s32 mess, s16 insertNo)
{
    MdPartyMainWinOpen(winNo);
    if (lbl_1_data_DA8[1] != mess) {
        lbl_1_data_DA8[0] = -1;
        lbl_1_data_DA8[1] = mess;
        MdPartyWinInsertMesSet(lbl_1_data_DA2[0], lbl_1_data_DA8[1], insertNo);
    }
}

void MdPartyHelpWinMesSet(s32 mess)
{
    if (lbl_1_data_DA2[1] == -1) {
        lbl_1_data_DA2[1] = 0;
        lbl_1_data_DA8[2] = -1;
        MdPartyWinOpen(lbl_1_data_DA2[1]);
    }
    if (lbl_1_data_DA8[2] != mess) {
        lbl_1_data_DA8[2] = mess;
        MdPartyWinMesSet(lbl_1_data_DA2[1], lbl_1_data_DA8[2], 0);
    }
}

void MdPartyHelpWinClose(void)
{
    if (lbl_1_data_DA2[1] != -1) {
        MdPartyWinClose(lbl_1_data_DA2[1]);
    }
    lbl_1_data_DA2[1] = -1;
    lbl_1_data_DA8[2] = -1;
}

void MdPartyShadowInit(void)
{
    HuVecF pos = { 0.0f, 3000.0f, 600.0f };
    HuVecF up = { 0.0f, 1.0f, 0.0f };
    HuVecF target = { 0.0f, 0.0f, 0.0f };
    Hu3DShadowCreate(30.0f, 10.0f, 10000.0f);
    Hu3DShadowPosSet(&pos, &up, &target);
}

void MdPartyShadowKill(void)
{
}

void MdPartySprGrpAttrSet(HUSPR_GROUPID grpId, u32 attr)
{
    HUSPR_GROUP *grpP = &HuSprGrpData[grpId];
    s16 i;
    for (i = 0; i < grpP->sprNum; i++) {
        HuSprAttrSet(grpId, i, (u16)attr);
    }
}

void MdPartySprGrpAttrReset(HUSPR_GROUPID grpId, u32 attr)
{
    HUSPR_GROUP *grpP = &HuSprGrpData[grpId];
    s16 i;
    for (i = 0; i < grpP->sprNum; i++) {
        HuSprAttrReset(grpId, i, (u16)attr);
    }
}

void MdPartySprInit(void)
{
    MDPARTY_SPRDATA *sprData;
    s16 i;
    for (i = 0; i < 32; i++) {
        lbl_1_bss_93C[i] = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_data_1C[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    for (i = 0; i < 17; i++) {
        lbl_1_bss_918[i] = HuSprGrpCreate(lbl_1_data_9C[i]);
    }
    for (i = 0, sprData = lbl_1_data_C0; i < 58; i++, sprData++) {
        lbl_1_bss_8A4[i] = HuSprCreate(lbl_1_bss_93C[sprData->animNo], sprData->prio, sprData->bank);
        HuSprGrpMemberSet(lbl_1_bss_918[sprData->grpNo], sprData->memberNo, lbl_1_bss_8A4[i]);
        HuSprPosSet(lbl_1_bss_918[sprData->grpNo], sprData->memberNo, sprData->posX, sprData->posY);
        HuSprScaleSet(lbl_1_bss_918[sprData->grpNo], sprData->memberNo, sprData->scaleX, sprData->scaleY);
        HuSprZRotSet(lbl_1_bss_918[sprData->grpNo], sprData->memberNo, sprData->zRot);
    }
    for (i = 0; i < 17; i++) {
        MdPartySprGrpAttrSet(lbl_1_bss_918[i], HUSPR_ATTR_DISPOFF);
    }
}

void MdPartySprKill(void)
{
}

void MdPartyHostItemThrowUpdate(OMOBJ *obj)
{
    s16 doneNum = 0;
    float maxTime = 60.0f;
    OMOBJ *obj1 = lbl_1_bss_C;
    OMOBJ *obj2 = lbl_1_bss_10;
    MDPARTY_PATH *path1 = &lbl_1_bss_228[0];
    MDPARTY_PATH *path2 = &lbl_1_bss_228[1];
    HuVecF pos;
    if (obj1 != NULL) {
        pos.x = MdPartySinLerp90(path1->pos[0].x, 500.0f, path1->time, maxTime);
        pos.y = MdPartySinLerp180(path1->pos[0].y, 400.0f, path1->time, maxTime);
        pos.z = path1->pos[0].z;
        if ((path1->time += 1.0f) > maxTime) {
            doneNum++;
            fn_1_43F3C(0, NULL, 0);
        } else {
            fn_1_43F3C(0, &pos, 1);
        }
    }
    if (obj2 != NULL) {
        pos.x = MdPartySinLerp90(path2->pos[0].x, -500.0f, path2->time, maxTime);
        pos.y = MdPartySinLerp180(path2->pos[0].y, 400.0f, path2->time, maxTime);
        pos.z = path2->pos[0].z;
        if ((path2->time += 1.0f) > maxTime) {
            doneNum++;
            fn_1_43F3C(1, NULL, 0);
        } else {
            fn_1_43F3C(1, &pos, 1);
        }
    }
    if (doneNum >= 2) {
        obj->objFunc = NULL;
    }
}

void MdPartyHostItemHoldUpdate(OMOBJ *obj)
{
    OMOBJ *obj1 = lbl_1_bss_C;
    OMOBJ *obj2 = lbl_1_bss_10;
    MDPARTY_PATH *path1 = &lbl_1_bss_228[0];
    MDPARTY_PATH *path2 = &lbl_1_bss_228[1];
    if (obj1 != NULL) {
        Hu3DModelObjPosGet(obj1->mdlId[0], "gN00m1-itemhook_R", &path1->pos[0]);
        fn_1_43F3C(0, &path1->pos[0], 1);
        path1->time = 0.0f;
    }
    if (obj2 != NULL) {
        Hu3DModelObjPosGet(obj2->mdlId[0], "gN00m1-itemhook_R", &path2->pos[0]);
        fn_1_43F3C(1, &path2->pos[0], 1);
        path2->time = 0.0f;
    }
    if (++obj->work[0] > 30) {
        obj->work[0] = 0;
        obj->objFunc = MdPartyHostItemThrowUpdate;
    }
}

void MdPartyHostItemThrowSet(void)
{
    Hu3DMotionShiftSet(lbl_1_bss_C->mdlId[0], lbl_1_bss_C->mtnId[4], 0.0f, 5.0f, 0);
    Hu3DMotionShiftSet(lbl_1_bss_10->mdlId[0], lbl_1_bss_10->mtnId[4], 0.0f, 5.0f, 0);
    lbl_1_bss_30 = omAddObjEx(lbl_1_bss_0, 0x1000, 16, 16, -1, MdPartyHostItemHoldUpdate);
    lbl_1_bss_30->work[0] = 0;
    HuAudFXPlay(0x49F);
    HuAudFXPlay(0x4A0);
}

void MdPartyHostAReactUpdate(OMOBJ *obj)
{
    Hu3DMotionSpeedSet(obj->mdlId[0], 2.0f);
    if (obj->work[3]++ > 30) {
        obj->objFunc = NULL;
        Hu3DMotionSpeedSet(obj->mdlId[0], 1.0f);
        Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[0], 0.0f, 15.0f, HU3D_MOTATTR_LOOP);
    }
}

void MdPartyHostAReactSet(void)
{
    OMOBJ *obj = lbl_1_bss_C;
    Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[3], 0.0f, 10.0f, 0);
    obj->work[3] = 0;
    obj->objFunc = MdPartyHostAReactUpdate;
}

void MdPartyHostBReactUpdate(OMOBJ *obj)
{
    Hu3DMotionSpeedSet(obj->mdlId[0], 2.0f);
    if (obj->work[3]++ > 30) {
        obj->objFunc = NULL;
        Hu3DMotionSpeedSet(obj->mdlId[0], 1.0f);
        Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[0], 0.0f, 15.0f, HU3D_MOTATTR_LOOP);
    }
}

void MdPartyHostBReactSet(void)
{
    OMOBJ *obj = lbl_1_bss_10;
    Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[3], 0.0f, 10.0f, 0);
    obj->work[3] = 0;
    obj->objFunc = MdPartyHostBReactUpdate;
}

void MdPartyHostAFlyUpdate(OMOBJ *obj)
{
    MDPARTY_PATH *path = &lbl_1_bss_1C8[0];
    HuVecF pos;
    if (path->mode == 0) {
        Hu3DModelPosGet(obj->mdlId[0], &pos);
        pos.y += 5.0f;
        Hu3DModelPosSetV(obj->mdlId[0], &pos);
        if ((path->time += 1.0f) > path->maxTime) {
            Hu3DModelPosGet(obj->mdlId[0], &path->pos[0]);
            path->mode = 1;
            path->time = 0.0f;
            path->maxTime = 90.0f;
        }
    }
    if (path->mode == 1) {
        MdPartyModelBezierMove(obj->mdlId[0], &path->pos[0], &path->pos[1], &path->pos[2], path->time, path->maxTime);
        if ((path->time += 1.0f) > path->maxTime) {
            HuAudFXStop(lbl_1_bss_A6C[0]);
            path->mode = 2;
        }
    }
    Hu3DModelPosGet(obj->mdlId[0], &pos);
    fn_1_464A0(4, &pos, 1, 5);
    if (path->mode == 2) {
        obj->objFunc = NULL;
    }
}

void MdPartyHostAFlySet(void)
{
    MDPARTY_PATH *path = &lbl_1_bss_1C8[0];
    OMOBJ *obj = lbl_1_bss_C;
    path->mode = 0;
    path->time = 0.0f;
    path->maxTime = 20.0f;
    Hu3DModelPosGet(obj->mdlId[0], &path->pos[0]);
    path->pos[0].x = -325.0f;
    path->pos[0].y = 0.0f;
    path->pos[0].z = 100.0f;
    Hu3DModelPosGet(obj->mdlId[0], &path->pos[1]);
    path->pos[1].x = 100.0f;
    path->pos[1].y = 600.0f;
    path->pos[1].z = 200.0f;
    Hu3DModelPosGet(obj->mdlId[0], &path->pos[2]);
    path->pos[2].x = 100.0f;
    path->pos[2].y = 225.0f;
    path->pos[2].z = -1000.0f;
    Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[2], 0.0f, 0.0f, 0);
    lbl_1_bss_A6C[0] = HuAudFXPlay(0x47E);
    obj->objFunc = MdPartyHostAFlyUpdate;
}

void MdPartyHostAWalkUpdate(OMOBJ *obj)
{
    HuVecF pos[2] = { { -100.0f, 0.0f, 0.0f }, { -325.0f, 0.0f, 100.0f } };
    float time = obj->work[0];
    MdPartyModelMoveTurn(obj->mdlId[0], &pos[0], &pos[1], time, 30.0f, 15.0f);
    Hu3DMotionSpeedSet(obj->mdlId[0], 1.5f);
    if (++obj->work[0] > 60) {
        Hu3DMotionSpeedSet(obj->mdlId[0], 1.0f);
        Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[0], 0.0f, 15.0f, HU3D_MOTATTR_LOOP);
        obj->objFunc = NULL;
    }
}

s32 MdPartyHostAWalkSet(void)
{
    OMOBJ *obj = lbl_1_bss_C;
    HuVecF pos;
    Hu3DModelPosGet(obj->mdlId[0], &pos);
    if (pos.x < -100.0f) {
        return 0;
    }
    obj->work[0] = 0;
    obj->objFunc = MdPartyHostAWalkUpdate;
    Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[1], 0.0f, 0.0f, HU3D_MOTATTR_LOOP);
    return 1;
}

void MdPartyHostACreate(OMOBJ *obj)
{
    omSetStatBit(obj, 0x100);
    obj->mdlId[0] = Hu3DModelCreate(HuDataReadNum(DATANUM(DATA_mdparty, 0x4F), HU_MEMNUM_OVL));
    obj->mtnId[0] = Hu3DJointMotion(obj->mdlId[0], HuDataReadNum(DATANUM(DATA_mdparty, 0x50), HU_MEMNUM_OVL));
    obj->mtnId[1] = Hu3DJointMotion(obj->mdlId[0], HuDataReadNum(DATANUM(DATA_mdparty, 0x51), HU_MEMNUM_OVL));
    obj->mtnId[2] = Hu3DJointMotion(obj->mdlId[0], HuDataReadNum(DATANUM(DATA_mdparty, 0x52), HU_MEMNUM_OVL));
    obj->mtnId[3] = Hu3DJointMotion(obj->mdlId[0], HuDataReadNum(DATANUM(DATA_mdparty, 0x53), HU_MEMNUM_OVL));
    obj->mtnId[4] = Hu3DJointMotion(obj->mdlId[0], HuDataReadNum(DATANUM(DATA_mdparty, 0x54), HU_MEMNUM_OVL));
    obj->mtnId[5] = Hu3DJointMotion(obj->mdlId[0], HuDataReadNum(DATANUM(DATA_mdparty, 0x55), HU_MEMNUM_OVL));
    Hu3DModelPosSet(obj->mdlId[0], -100.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(obj->mdlId[0], 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(obj->mdlId[0], 1.25f, 1.25f, 1.25f);
    Hu3DModelLayerSet(obj->mdlId[0], 1);
    Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[0], 0.0f, 0.0f, HU3D_MOTATTR_LOOP);
    Hu3DModelShadowSet(obj->mdlId[0]);
    obj->objFunc = NULL;
}

void MdPartyHostAKill(OMOBJ *obj)
{
    if (obj) {
        Hu3DMotionKill(obj->mtnId[0]);
        Hu3DMotionKill(obj->mtnId[1]);
        Hu3DMotionKill(obj->mtnId[2]);
        Hu3DMotionKill(obj->mtnId[3]);
        Hu3DMotionKill(obj->mtnId[4]);
        Hu3DMotionKill(obj->mtnId[5]);
        Hu3DModelKill(obj->mdlId[0]);
        omDelObjEx(lbl_1_bss_0, obj);
    }
    obj->objFunc = NULL;
}

void MdPartyHostBFlyUpdate(OMOBJ *obj)
{
    MDPARTY_PATH *path = &lbl_1_bss_1C8[1];
    HuVecF pos;
    if (path->mode == 0) {
        Hu3DModelPosGet(obj->mdlId[0], &pos);
        pos.y += 5.0f;
        Hu3DModelPosSetV(obj->mdlId[0], &pos);
        if ((path->time += 1.0f) > path->maxTime) {
            Hu3DModelPosGet(obj->mdlId[0], &path->pos[0]);
            path->mode = 1;
            path->time = 0.0f;
            path->maxTime = 90.0f;
        }
    }
    if (path->mode == 1) {
        MdPartyModelBezierMove(obj->mdlId[0], &path->pos[0], &path->pos[1], &path->pos[2], path->time, path->maxTime);
        if ((path->time += 1.0f) > path->maxTime) {
            HuAudFXStop(lbl_1_bss_A6C[1]);
            path->mode = 2;
        }
    }
    Hu3DModelPosGet(obj->mdlId[0], &pos);
    fn_1_464A0(5, &pos, 1, 6);
    if (path->mode == 2) {
        obj->objFunc = NULL;
    }
}

void MdPartyHostBFlySet(void)
{
    MDPARTY_PATH *path = &lbl_1_bss_1C8[1];
    OMOBJ *obj = lbl_1_bss_10;
    path->mode = 0;
    path->time = 0.0f;
    path->maxTime = 20.0f;
    Hu3DModelPosGet(obj->mdlId[0], &path->pos[0]);
    path->pos[0].x = 325.0f;
    path->pos[0].y = 0.0f;
    path->pos[0].z = 100.0f;
    Hu3DModelPosGet(obj->mdlId[0], &path->pos[1]);
    path->pos[1].x = -100.0f;
    path->pos[1].y = 400.0f;
    path->pos[1].z = 200.0f;
    Hu3DModelPosGet(obj->mdlId[0], &path->pos[2]);
    path->pos[2].x = -100.0f;
    path->pos[2].y = 125.0f;
    path->pos[2].z = -1000.0f;
    Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[2], 0.0f, 0.0f, 0);
    lbl_1_bss_A6C[1] = HuAudFXPlay(0x47C);
    obj->objFunc = MdPartyHostBFlyUpdate;
}

void MdPartyHostBWalkUpdate(OMOBJ *obj)
{
    HuVecF pos[2] = { { 100.0f, 0.0f, 0.0f }, { 325.0f, 0.0f, 100.0f } };
    float time = obj->work[0];
    MdPartyModelMoveTurn(obj->mdlId[0], &pos[0], &pos[1], time, 30.0f, -15.0f);
    Hu3DMotionSpeedSet(obj->mdlId[0], 1.5f);
    if (++obj->work[0] > 60) {
        Hu3DMotionSpeedSet(obj->mdlId[0], 1.0f);
        Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[0], 0.0f, 15.0f, HU3D_MOTATTR_LOOP);
        obj->objFunc = NULL;
    }
}

s32 MdPartyHostBWalkSet(void)
{
    OMOBJ *obj = lbl_1_bss_10;
    HuVecF pos;
    Hu3DModelPosGet(obj->mdlId[0], &pos);
    if (pos.x > 100.0f) {
        return 0;
    }
    obj->work[0] = 0;
    obj->objFunc = MdPartyHostBWalkUpdate;
    Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[1], 0.0f, 0.0f, HU3D_MOTATTR_LOOP);
    return 1;
}

void MdPartyHostBCreate(OMOBJ *obj)
{
    omSetStatBit(obj, 0x100);
    obj->mdlId[0] = Hu3DModelCreate(HuDataReadNum(DATANUM(DATA_mdparty, 0x56), HU_MEMNUM_OVL));
    obj->mdlId[1] = Hu3DModelCreate(HuDataReadNum(DATANUM(DATA_mdparty, 0x57), HU_MEMNUM_OVL));
    obj->mtnId[0] = Hu3DJointMotion(obj->mdlId[0], HuDataReadNum(DATANUM(DATA_mdparty, 0x58), HU_MEMNUM_OVL));
    obj->mtnId[1] = Hu3DJointMotion(obj->mdlId[0], HuDataReadNum(DATANUM(DATA_mdparty, 0x59), HU_MEMNUM_OVL));
    obj->mtnId[2] = Hu3DJointMotion(obj->mdlId[0], HuDataReadNum(DATANUM(DATA_mdparty, 0x5A), HU_MEMNUM_OVL));
    obj->mtnId[3] = Hu3DJointMotion(obj->mdlId[0], HuDataReadNum(DATANUM(DATA_mdparty, 0x5B), HU_MEMNUM_OVL));
    obj->mtnId[4] = Hu3DJointMotion(obj->mdlId[0], HuDataReadNum(DATANUM(DATA_mdparty, 0x5C), HU_MEMNUM_OVL));
    obj->mtnId[5] = Hu3DJointMotion(obj->mdlId[0], HuDataReadNum(DATANUM(DATA_mdparty, 0x5D), HU_MEMNUM_OVL));
    Hu3DModelHookSet(obj->mdlId[0], "gN01m1-itemhook_R", obj->mdlId[1]);
    Hu3DModelPosSet(obj->mdlId[0], 100.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(obj->mdlId[0], 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(obj->mdlId[0], 1.25f, 1.25f, 1.25f);
    Hu3DModelLayerSet(obj->mdlId[0], 1);
    Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[0], 0.0f, 0.0f, HU3D_MOTATTR_LOOP);
    Hu3DModelShadowSet(obj->mdlId[0]);
    obj->objFunc = NULL;
}

/* @@PART3@@ */
void MdPartyHostKill(OMOBJ *obj)
{
    if (obj) {
        Hu3DModelHookReset(obj->mdlId[1]);
        Hu3DMotionKill(obj->mtnId[0]);
        Hu3DMotionKill(obj->mtnId[1]);
        Hu3DMotionKill(obj->mtnId[2]);
        Hu3DMotionKill(obj->mtnId[3]);
        Hu3DMotionKill(obj->mtnId[4]);
        Hu3DMotionKill(obj->mtnId[5]);
        Hu3DModelKill(obj->mdlId[0]);
        Hu3DModelKill(obj->mdlId[1]);
        omDelObjEx(lbl_1_bss_0, obj);
    }
    obj->objFunc = NULL;
}

void MdPartyPlayerJumpUpdate(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;
    MDPARTY_PATH *jump;
    HU3D_MODELID mdlId;
    float t;
    HuVecF sePos;
    HuVecF rot;
    HuVecF mdlPos;
    HuVecF pos;
    for (i = 0, player = &lbl_1_bss_9BC[i]; i < 4; i++, player++) {
        jump = &lbl_1_bss_108[i];
        if (jump->mode != 1) {
            continue;
        }
        mdlId = obj->mdlId[player->charNo];
        t = fn_1_1868(0.0f, 1.0f, jump->time, jump->maxTime);
        fn_1_1404(&pos, t, &jump->pos[0], &jump->pos[1], &jump->pos[2]);
        Hu3DModelPosGet(mdlId, &mdlPos);
        Hu3DModelRotGet(mdlId, &rot);
        mdlPos.x -= pos.x;
        mdlPos.z -= pos.z;
        mdlPos.y = -HuAtan(mdlPos.x, -mdlPos.z);
        if (rot.y - mdlPos.y > 180.0f) {
            rot.y -= 360.0f;
        } else if (rot.y - mdlPos.y < -180.0f) {
            rot.y += 360.0f;
        }
        rot.y = fn_1_160C(rot.y, mdlPos.y, 10.0f);
        Hu3DModelPosSet(mdlId, pos.x, pos.y, pos.z);
        Hu3DModelRotSet(mdlId, 0.0f, rot.y, 0.0f);
        if ((jump->time += 1.0f) > jump->maxTime) {
            jump->mode = 2;
            Hu3DModelAttrSet(obj->mdlId[player->charNo], HU3D_ATTR_DISPOFF);
        }
        Hu3DModelPosGet(obj->mdlId[player->charNo], &sePos);
        if (player->com == 0) {
            fn_1_464A0(i, &sePos, 1, player->padNo);
        } else {
            fn_1_464A0(i, &sePos, 1, 4);
        }
    }
    for (i = 0; i < 4; i++) {
        jump = &lbl_1_bss_108[i];
        if (jump->mode != 2) {
            break;
        }
    }
    if (i == 4) {
        obj->objFunc = NULL;
    }
}

void MdPartyPlayerJumpStart(void)
{
    MDPARTY_PATH *jump;
    MDPARTY_PLAYER *player;
    OMOBJ *obj = lbl_1_bss_14;
    s16 i;
    for (i = 0, player = &lbl_1_bss_9BC[i]; i < 4; i++, player++) {
        jump = &lbl_1_bss_108[i];
        jump->mode = 1;
        jump->time = 0.0f;
        jump->maxTime = 90.0f;
        Hu3DModelPosGet(obj->mdlId[player->charNo], &jump->pos[0]);
        Hu3DModelPosGet(obj->mdlId[player->charNo], &jump->pos[1]);
        jump->pos[1].x += 0.5f * jump->pos[1].x;
        jump->pos[1].y = 550.0f;
        jump->pos[1].z = 100.0f;
        Hu3DModelPosGet(obj->mdlId[player->charNo], &jump->pos[2]);
        jump->pos[2].y = 225.0f;
        jump->pos[2].z = -800.0f;
        Hu3DMotionShiftSet(obj->mdlId[player->charNo], obj->mtnId[(player->charNo * 2) + 1], 0.0f, 10.0f, HU3D_MOTATTR_LOOP);
    }
    HuAudFXPlay(0x4A2);
    obj->objFunc = MdPartyPlayerJumpUpdate;
}

void MdPartyPlayerScaleUpdate(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;
    float scale;
    for (i = 0, player = &lbl_1_bss_9BC[i]; i < 4; i++, player++) {
        scale = fn_1_1868(0.0f, 1.0f, obj->work[0], obj->work[1]);
        Hu3DModelScaleSet(obj->mdlId[player->charNo], scale, scale, scale);
    }
    if ((obj->work[0] += 1.0f) > obj->work[1]) {
        obj->objFunc = NULL;
    }
}

void MdPartyPlayerAppear(void)
{
    OMOBJ *obj = lbl_1_bss_14;
    s16 i;
    MDPARTY_PLAYER *player;
    HuVecF pos;
    for (i = 0, player = &lbl_1_bss_9BC[i]; i < 4; i++, player++) {
        Hu3DModelPosGet(lbl_1_bss_288[player->charNo].mdlId, &pos);
        if (player->com) {
            fn_1_45F3C(i, &pos, 4, 0);
        } else {
            fn_1_45F3C(i, &pos, player->padNo, 0);
        }
    }
    for (i = 0, player = &lbl_1_bss_9BC[i]; i < 4; i++, player++) {
        Hu3DModelPosGet(lbl_1_bss_288[player->charNo].mdlId, &pos);
        pos.y -= 70.0f;
        Hu3DModelPosSetV(obj->mdlId[player->charNo], &pos);
        Hu3DModelScaleSet(obj->mdlId[player->charNo], 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrReset(obj->mdlId[player->charNo], HU3D_ATTR_DISPOFF);
    }
    obj->work[0] = 0;
    obj->work[1] = 15;
    obj->objFunc = MdPartyPlayerScaleUpdate;
    HuPrcSleep(15);
    HuAudFXPlay(0x4A1);
    for (i = 0, player = &lbl_1_bss_9BC[i]; i < 4; i++, player++) {
        Hu3DModelPosGet(lbl_1_bss_288[player->charNo].mdlId, &pos);
        if (player->com) {
            fn_1_45F3C(i, &pos, 4, 1);
        } else {
            fn_1_45F3C(i, &pos, player->padNo, 1);
        }
    }
}

void MdPartyCharMdlCreate(OMOBJ *obj)
{
    s16 i;
    omSetStatBit(obj, 0x100);
    for (i = 0; i < 11; i++) {
        obj->mdlId[i] = Hu3DModelCreate(HuDataSelHeapReadNum(DATA_mdparty + i + 0x5E, HU_MEMNUM_OVL, HEAP_MODEL));
        obj->mtnId[i * 2] = Hu3DJointMotion(obj->mdlId[i], HuDataSelHeapReadNum(DATA_mdparty + i + 0x69, HU_MEMNUM_OVL, HEAP_MODEL));
        obj->mtnId[(i * 2) + 1] = Hu3DJointMotion(obj->mdlId[i], HuDataSelHeapReadNum(DATA_mdparty + i + 0x74, HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(obj->mdlId[i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(obj->mdlId[i], 1);
        Hu3DMotionShiftSet(obj->mdlId[i], obj->mtnId[i * 2], 0.0f, 0.0f, HU3D_MOTATTR_LOOP);
        Hu3DModelShadowSet(obj->mdlId[i]);
    }
    obj->objFunc = NULL;
}

void MdPartyCharMdlKill(OMOBJ *obj)
{
    s16 i;
    if (obj) {
        for (i = 0; i < 11; i++) {
            Hu3DMotionKill(obj->mtnId[i * 2]);
            Hu3DMotionKill(obj->mtnId[(i * 2) + 1]);
            Hu3DModelKill(obj->mdlId[i]);
        }
        omDelObjEx(lbl_1_bss_0, obj);
    }
    obj = NULL;
}

void MdPartyTurnNumSet(s16 turn, s16 turnMax)
{
    s16 digit = 0;
    if (turn <= turnMax) {
        HuSprGrpPosSet(lbl_1_bss_918[12], 288.0f, 280.0f);
        MdPartySprGrpAttrReset(lbl_1_bss_918[12], HUSPR_ATTR_DISPOFF);
        digit = turn / 10;
        HuSprBankSet(lbl_1_bss_918[12], 0, digit);
        if (digit == 0) {
            HuSprAttrSet(lbl_1_bss_918[12], 0, HUSPR_ATTR_DISPOFF);
        }
        digit = turn % 10;
        HuSprBankSet(lbl_1_bss_918[12], 1, digit);
        digit = turnMax / 10;
        HuSprBankSet(lbl_1_bss_918[12], 2, digit);
        if (digit == 0) {
            HuSprAttrSet(lbl_1_bss_918[12], 2, HUSPR_ATTR_DISPOFF);
        }
        digit = turnMax % 10;
        HuSprBankSet(lbl_1_bss_918[12], 3, digit);
    }
}

void MdPartyTurnNumHide(void)
{
    MdPartySprGrpAttrSet(lbl_1_bss_918[12], HUSPR_ATTR_DISPOFF);
}

void MdPartyTitleFadeOut(OMOBJ *obj)
{
    float tpLvl = fn_1_1868(1.0f, 0.0f, obj->work[0], 10.0f);
    HuSprGrpTPLvlSet(lbl_1_bss_918[0], tpLvl);
    if (++obj->work[0] > 10) {
        MdPartySprGrpAttrSet(lbl_1_bss_918[0], HUSPR_ATTR_DISPOFF);
        obj->objFunc = NULL;
    }
}

void MdPartyTitleShow(void)
{
    if (lbl_1_bss_28 == 0) {
        HuSprGrpPosSet(lbl_1_bss_918[0], 288.0f, 120.0f);
        MdPartySprGrpAttrReset(lbl_1_bss_918[0], HUSPR_ATTR_DISPOFF);
    }
}

void MdPartyTitleFadeOutStart(void)
{
    OMOBJ *obj = lbl_1_bss_4;
    obj->work[0] = 0;
    obj->objFunc = MdPartyTitleFadeOut;
}

void MdPartyStageCreate(OMOBJ *obj)
{
    s16 i;
    omSetStatBit(obj, 0x100);
    for (i = 0; i < 3; i++) {
        obj->mdlId[i] = Hu3DModelCreate(HuDataSelHeapReadNum(DATA_mdparty + i, HU_MEMNUM_OVL, HEAP_MODEL));
        obj->mtnId[i] = Hu3DMotionIDGet(obj->mdlId[i]);
        Hu3DModelLayerSet(obj->mdlId[i], 1);
        Hu3DMotionShiftSet(obj->mdlId[i], obj->mtnId[i], 0.0f, 0.0f, HU3D_MOTATTR_LOOP);
        Hu3DModelShadowMapSet(obj->mdlId[i]);
    }
    MdPartyTitleShow();
    obj->objFunc = NULL;
}

void MdPartyStageKill(OMOBJ *obj)
{
    s16 i;
    if (obj) {
        for (i = 0; i < 3; i++) {
            Hu3DMotionKill(obj->mtnId[i]);
            Hu3DModelKill(obj->mdlId[i]);
        }
        omDelObjEx(lbl_1_bss_0, obj);
    }
    obj = NULL;
}

void MdPartyDescFadeIn(s16 memberNo)
{
    OMOBJ *obj = lbl_1_bss_8;
    HuSprGrpPosSet(lbl_1_bss_918[1], 288.0f, 60.0f);
    HuSprGrpTPLvlSet(lbl_1_bss_918[1], 0.0f);
    MdPartySprGrpAttrSet(lbl_1_bss_918[1], HUSPR_ATTR_DISPOFF);
    HuSprAttrReset(lbl_1_bss_918[1], memberNo, HUSPR_ATTR_DISPOFF);
    obj->work[2] = 1;
    obj->work[3] = 0;
}

void MdPartyDescFadeOutStart(void)
{
    OMOBJ *obj = lbl_1_bss_8;
    obj->work[2] = 2;
    obj->work[3] = 0;
}

void MdPartyDescInit(void)
{
    HuSprGrpPosSet(lbl_1_bss_918[1], 288.0f, 60.0f);
    HuSprGrpTPLvlSet(lbl_1_bss_918[1], 0.0f);
}

void MdPartyDescUpdate(OMOBJ *obj)
{
    float tpLvl;
    switch (obj->work[2]) {
        case 1:
            tpLvl = fn_1_1868(0.0f, 1.0f, obj->work[3], 10.0f);
            HuSprGrpTPLvlSet(lbl_1_bss_918[1], tpLvl);
            if (++obj->work[3] > 10) {
                obj->work[2] = 0;
            }
            break;
        case 2:
            tpLvl = fn_1_1868(1.0f, 0.0f, obj->work[3], 10.0f);
            HuSprGrpTPLvlSet(lbl_1_bss_918[1], tpLvl);
            if (++obj->work[3] > 10) {
                obj->work[2] = 0;
            }
            break;
    }
}

void MdPartyPicSet(s16 picNo)
{
    OMOBJ *obj = lbl_1_bss_8;
    Hu3DAnimAnimSet(obj->mtnId[2], lbl_1_bss_93C[(picNo * 2) + 3]);
    Hu3DAnimAnimSet(obj->mtnId[3], lbl_1_bss_93C[(picNo * 2) + 4]);
    obj->work[0] = 1;
    obj->work[1] = 0;
}

void MdPartyPicFadeOut(void)
{
    OMOBJ *obj = lbl_1_bss_8;
    obj->work[0] = 2;
    obj->work[1] = 0;
    MdPartyDescFadeOutStart();
}

void MdPartyPicUpdate(OMOBJ *obj)
{
    s16 i;
    HuVecF rot;
    float tpLvl;
    for (i = 0; i < 2; i++) {
        Hu3DModelRotGet(obj->mdlId[i], &rot);
        rot.z -= 0.1f;
        if (rot.z < 0.0f) {
            rot.z += 360.0f;
        }
        Hu3DModelRotSetV(obj->mdlId[i], &rot);
    }
    switch (obj->work[0]) {
        case 1:
            tpLvl = fn_1_1868(1.0f, 0.0f, obj->work[1], 30.0f);
            Hu3DModelTPLvlSet(obj->mdlId[0], tpLvl);
            if (++obj->work[1] > 30) {
                obj->work[0] = 0;
            }
            break;
        case 2:
            tpLvl = fn_1_1868(0.0f, 1.0f, obj->work[1], 30.0f);
            Hu3DModelTPLvlSet(obj->mdlId[0], tpLvl);
            if (++obj->work[1] > 30) {
                obj->work[0] = 0;
            }
            break;
    }
    MdPartyDescUpdate(obj);
}

void MdPartyPicCreate(OMOBJ *obj)
{
    s16 i;
    omSetStatBit(obj, 0x100);
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            obj->mdlId[i] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_mdparty, 3), HU_MEMNUM_OVL, HEAP_MODEL));
        } else {
            obj->mdlId[i] = Hu3DModelLink(obj->mdlId[0]);
        }
        Hu3DModelPosSet(obj->mdlId[i], 0.0f, -100.0f, -850.0f - i);
        Hu3DModelRotSet(obj->mdlId[i], 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrSet(obj->mdlId[i], HU3D_MOTATTR_PAUSE);
        Hu3DModelLayerSet(obj->mdlId[i], 0);
        obj->mtnId[i * 2] = Hu3DAnimCreate(lbl_1_bss_93C[1], obj->mdlId[i], "dpic00");
        obj->mtnId[(i * 2) + 1] = Hu3DAnimCreate(lbl_1_bss_93C[2], obj->mdlId[i], "dpic01");
    }
    MdPartyDescInit();
    obj->objFunc = MdPartyPicUpdate;
}

void MdPartyPicKill(OMOBJ *obj)
{
    s16 i;
    if (obj) {
        for (i = 0; i < 2; i++) {
            Hu3DAnimKill(obj->mtnId[i * 2]);
            Hu3DAnimKill(obj->mtnId[(i * 2) + 1]);
            obj->mtnId[i * 2] = -1;
            obj->mtnId[(i * 2) + 1] = -1;
        }
        for (i = 1; i >= 0; i--) {
            Hu3DModelKill(obj->mdlId[i]);
        }
        omDelObjEx(lbl_1_bss_0, obj);
    }
    obj = NULL;
}

u32 MdPartyPairMesGet(s16 a, s16 b)
{
    s8 pairTbl[55][2] = {
        { 0, 1 }, { 0, 2 }, { 0, 3 }, { 0, 4 }, { 0, 5 }, { 0, 6 }, { 0, 7 }, { 0, 8 }, { 0, 9 }, { 0, 10 },
        { 1, 2 }, { 1, 3 }, { 1, 4 }, { 1, 5 }, { 1, 6 }, { 1, 7 }, { 1, 8 }, { 1, 9 }, { 1, 10 },
        { 2, 3 }, { 2, 4 }, { 2, 5 }, { 2, 6 }, { 2, 7 }, { 2, 8 }, { 2, 9 }, { 2, 10 },
        { 3, 4 }, { 3, 5 }, { 3, 6 }, { 3, 7 }, { 3, 8 }, { 3, 9 }, { 3, 10 },
        { 4, 5 }, { 4, 6 }, { 4, 7 }, { 4, 8 }, { 4, 9 }, { 4, 10 },
        { 5, 6 }, { 5, 7 }, { 5, 8 }, { 5, 9 }, { 5, 10 },
        { 6, 7 }, { 6, 8 }, { 6, 9 }, { 6, 10 },
        { 7, 8 }, { 7, 9 }, { 7, 10 },
        { 8, 9 }, { 8, 10 },
        { 9, 10 }
    };
    s8 i;
    for (i = 0; i < 55; i++) {
        if (a == pairTbl[i][0] && b == pairTbl[i][1]) {
            break;
        }
        if (a == pairTbl[i][1] && b == pairTbl[i][0]) {
            break;
        }
    }
    if (i == 55) {
        return 0x30037;
    }
    return i + 0x30000;
}

void MdPartyInfoWinMesSet(s16 winNo, u32 mess)
{
    HuWinAttrSet(lbl_1_bss_104[winNo], HUWIN_ATTR_ALIGN_CENTER);
    HuWinMesSet(lbl_1_bss_104[winNo], mess);
    HuWinMesSpeedSet(lbl_1_bss_104[winNo], 0);
}

void MdPartyInfoWinOpen(u32 mess0, u32 mess1)
{
    HuWinDispOn(lbl_1_bss_104[0]);
    HuWinDispOn(lbl_1_bss_104[1]);
    MdPartyInfoWinMesSet(0, mess0);
    MdPartyInfoWinMesSet(1, mess1);
    MdPartySprGrpAttrReset(lbl_1_bss_918[13], HUSPR_ATTR_DISPOFF);
}

void MdPartyInfoWinClose(void)
{
    HuWinDispOff(lbl_1_bss_104[0]);
    HuWinDispOff(lbl_1_bss_104[1]);
    MdPartySprGrpAttrSet(lbl_1_bss_918[13], HUSPR_ATTR_DISPOFF);
}

void MdPartyInfoWinCreate(void)
{
    HuVecF winPos[2] = {
        { 150.0f, 120.0f, 0.0f },
        { 426.0f, 120.0f, 0.0f }
    };
    s16 i;
    for (i = 0; i < 2; i++) {
        lbl_1_bss_104[i] = HuWinExCreateFrame(winPos[i].x - 124.0f, 6.0f + winPos[i].y, 240, 42, -1, 0);
        HuWinDispOff(lbl_1_bss_104[i]);
        HuWinBGTPLvlSet(lbl_1_bss_104[i], 0.0f);
        HuWinPriSet(lbl_1_bss_104[i], 0);
        HuSprPosSet(lbl_1_bss_918[13], i, winPos[i].x, winPos[i].y);
    }
}

void MdPartyInfoWinKill(void)
{
    s16 i;
    for (i = 0; i < 2; i++) {
        HuWinExKill(lbl_1_bss_104[i]);
    }
}

void MdPartyIconAOn(s16 playerNo, s16 bank)
{
    lbl_1_bss_D4[playerNo].x = 0.85f;
    MdPartySprGrpAttrReset(lbl_1_bss_918[playerNo + 4], HUSPR_ATTR_DISPOFF);
    HuSprBankSet(lbl_1_bss_918[playerNo + 4], 0, bank);
}

void MdPartyIconAOff(s16 playerNo)
{
    lbl_1_bss_D4[playerNo].x = 0.0f;
}

void MdPartyIconABankSet(s16 playerNo, s16 bank)
{
    HuSprBankSet(lbl_1_bss_918[playerNo + 4], 0, bank);
}

void MdPartyIconBOn(s16 playerNo, s16 bank)
{
    lbl_1_bss_A4[playerNo].x = 0.85f;
    MdPartySprGrpAttrReset(lbl_1_bss_918[playerNo + 8], HUSPR_ATTR_DISPOFF);
    HuSprBankSet(lbl_1_bss_918[playerNo + 8], 2, bank);
}

void MdPartyIconBOff(s16 playerNo)
{
    lbl_1_bss_A4[playerNo].x = 0.0f;
}

void MdPartyIconBBankSet(s16 playerNo, s16 bank)
{
    HuSprBankSet(lbl_1_bss_918[playerNo + 8], 2, bank);
}

void MdPartyIconCOn(s16 teamNo, s16 bank)
{
    lbl_1_bss_8C[teamNo].x = 0.85f;
    MdPartySprGrpAttrReset(lbl_1_bss_918[teamNo + 14], HUSPR_ATTR_DISPOFF);
    HuSprBankSet(lbl_1_bss_918[teamNo + 14], 2, bank);
}

void MdPartyIconCOff(s16 teamNo)
{
    lbl_1_bss_8C[teamNo].x = 0.0f;
}

void MdPartyIconCBankSet(s16 teamNo, s16 bank)
{
    HuSprBankSet(lbl_1_bss_918[teamNo + 14], 2, bank);
}

void MdPartyIconDOn(void)
{
    lbl_1_bss_80[0] = 0.5f;
    MdPartySprGrpAttrReset(lbl_1_bss_918[16], HUSPR_ATTR_DISPOFF);
}

void MdPartyIconDOff(void)
{
    lbl_1_bss_80[0] = 0.0f;
}

void MdPartyIconAllUpdate(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;
    HuVecF pos;
    HuVecF pos2D;
    for (i = 0, player = lbl_1_bss_9BC; i < 4; i++, player++) {
        Hu3DModelPosGet(lbl_1_bss_288[lbl_1_bss_9BC[i].charNo].mdlId, &pos);
        Hu3D3Dto2D(&pos, 1, &pos2D);
        HuSprGrpPosSet(lbl_1_bss_918[i + 4], pos2D.x - 18.0f, (s16)(pos2D.y - 23.0f));
        HuSprAttrReset(lbl_1_bss_918[i + 4], 0, 8);
        lbl_1_bss_D4[i].y = fn_1_160C(lbl_1_bss_D4[i].y, lbl_1_bss_D4[i].x, 3.0f);
        HuSprGrpScaleSet(lbl_1_bss_918[i + 4], lbl_1_bss_D4[i].y, lbl_1_bss_D4[i].y);
        Hu3DModelPosGet(lbl_1_bss_288[lbl_1_bss_9BC[i].charNo].mdlId, &pos);
        Hu3D3Dto2D(&pos, 1, &pos2D);
        HuSprGrpPosSet(lbl_1_bss_918[i + 8], 18.0f + pos2D.x, 23.0f + pos2D.y);
        lbl_1_bss_A4[i].y = fn_1_160C(lbl_1_bss_A4[i].y, lbl_1_bss_A4[i].x, 3.0f);
        HuSprGrpScaleSet(lbl_1_bss_918[i + 8], lbl_1_bss_A4[i].y, lbl_1_bss_A4[i].y);
    }
    for (i = 0; i < 2; i++) {
        Hu3DModelPosGet(lbl_1_bss_288[lbl_1_bss_9BC[i].charNo].mdlId, &pos);
        pos.x = lbl_1_data_8A4[i + 61].x;
        Hu3D3Dto2D(&pos, 1, &pos2D);
        HuSprGrpPosSet(lbl_1_bss_918[i + 14], pos2D.x, 23.0f + pos2D.y);
        lbl_1_bss_8C[i].y = fn_1_160C(lbl_1_bss_8C[i].y, lbl_1_bss_8C[i].x, 3.0f);
        HuSprGrpScaleSet(lbl_1_bss_918[i + 14], lbl_1_bss_8C[i].y, lbl_1_bss_8C[i].y);
    }
    Hu3DModelPosGet(lbl_1_bss_288[lbl_1_bss_9BC[i].charNo].mdlId, &pos);
    pos.x = 0.0f;
    Hu3D3Dto2D(&pos, 1, &pos2D);
    HuSprGrpPosSet(lbl_1_bss_918[16], pos2D.x, pos2D.y);
    lbl_1_bss_80[1] = fn_1_160C(lbl_1_bss_80[1], lbl_1_bss_80[0], 3.0f);
    HuSprGrpScaleSet(lbl_1_bss_918[16], lbl_1_bss_80[1], lbl_1_bss_80[1]);
}

void MdPartyIconInit(OMOBJ *obj)
{
    s16 i;
    for (i = 0; i < 4; i++) {
        lbl_1_bss_D4[i].x = lbl_1_bss_D4[i].y = 0.0f;
        lbl_1_bss_A4[i].x = lbl_1_bss_A4[i].y = 0.0f;
    }
    for (i = 0; i < 2; i++) {
        lbl_1_bss_8C[i].x = lbl_1_bss_8C[i].y = 0.0f;
    }
    lbl_1_bss_80[0] = lbl_1_bss_80[1] = 0.0f;
    obj->objFunc = MdPartyIconAllUpdate;
    MdPartyInfoWinCreate();
}

void MdPartyInfoWinKill2(void)
{
    s16 i;
    for (i = 0; i < 2; i++) {
        HuWinExKill(lbl_1_bss_104[i]);
    }
}

void MdPartyMarkPosSet(s16 memberNo, HuVecF *pos, float ofsX, float ofsY)
{
    HuVecF pos2D = { 0.0f, 0.0f, 0.0f };
    if (pos) {
        Hu3D3Dto2D(pos, 1, &pos2D);
    }
    HuSprPosSet(lbl_1_bss_918[2], memberNo, pos2D.x + ofsX, pos2D.y + ofsY);
}

/* @@PART4@@ */
/* .data: lbl_1_data_DE6 sits after part 3's strings and before this part's strings */
s16 lbl_1_data_DE6[6] = { 0 };

void MdPartyIconOpen(s16 no, HuVecF *pos, float ofsX, float ofsY)
{
    HuVecF pos2D = { 0.0f, 0.0f, 0.0f };
    if (pos) {
        Hu3D3Dto2D(pos, 1, &pos2D);
    }
    HuSprPosSet(lbl_1_bss_918[2], no, pos2D.x + ofsX, pos2D.y + ofsY);
    HuSprScaleSet(lbl_1_bss_918[2], no, 2.0f, 2.0f);
    HuSprAttrReset(lbl_1_bss_918[2], no, HUSPR_ATTR_DISPOFF);
    lbl_1_bss_38[no] = 2.0f;
    lbl_1_data_DE6[no] = 1;
}

void MdPartyIconClose(s16 no)
{
    HuSprAttrSet(lbl_1_bss_918[2], no, HUSPR_ATTR_DISPOFF);
    lbl_1_data_DE6[no] = 0;
}

void MdPartyIconPush(s16 no)
{
    lbl_1_bss_38[no] = 2.0f;
    if (lbl_1_data_DE6[no] == 1) {
        HuAudFXPlay(0);
    }
}

void MdPartyIconUpdate(void)
{
    s16 i;
    for (i = 0; i < 6; i++) {
        lbl_1_bss_38[i] = MdPartySmooth(lbl_1_bss_38[i], 1.0f, 5.0f);
        HuSprScaleSet(lbl_1_bss_918[2], i, lbl_1_bss_38[i], lbl_1_bss_38[i]);
    }
}

void MdPartyIconCloseAll(void)
{
    s16 i;
    for (i = 0; i < 6; i++) {
        MdPartyIconClose(i);
        lbl_1_data_DE6[i] = 0;
    }
}

void MdPartyMarkerPosSet(s16 no, HuVecF *pos)
{
    lbl_1_bss_50[no].x = pos->x - 50.0f;
    lbl_1_bss_50[no].y = 50.0f + pos->y;
    lbl_1_bss_50[no].z = 50.0f + pos->z;
}

/* Show and Hide take an int member number: retail extends it to s16 for the model calls only */
void MdPartyMarkerShow(int no, HuVecF *pos, s16 bank)
{
    OMOBJ *obj = lbl_1_bss_1C[0];
    Hu3DModelPosSet(obj->mdlId[(s16)no], pos->x, pos->y, pos->z);
    Hu3DModelRotSet(obj->mdlId[(s16)no], 0.0f, 0.0f, 180.0f);
    Hu3DModelScaleSet(obj->mdlId[(s16)no], 1.0f, 1.0f, 1.0f);
    MdPartyMarkerPosSet((s16)no, pos);
    Hu3DModelAttrReset(obj->mdlId[(s16)no], HU3D_ATTR_DISPOFF);
    HuSprScaleSet(lbl_1_bss_918[3], no, 0.8f, 0.8f);
    HuSprAttrReset(lbl_1_bss_918[3], no, HUSPR_ATTR_DISPOFF);
    HuSprBankSet(lbl_1_bss_918[3], no, bank);
}

void MdPartyMarkerHide(int no)
{
    OMOBJ *obj = lbl_1_bss_1C[0];
    Hu3DModelAttrSet(obj->mdlId[(s16)no], HU3D_ATTR_DISPOFF);
    HuSprScaleSet(lbl_1_bss_918[3], no, 0.8f, 0.8f);
    HuSprAttrSet(lbl_1_bss_918[3], no, HUSPR_ATTR_DISPOFF);
}

void MdPartyMarkerUpdate(OMOBJ *obj)
{
    HuVecF pos;
    HuVecF pos2D;
    s16 i;
    for (i = 0; i < 4; i++) {
        Hu3DModelPosGet(obj->mdlId[i], &pos);
        MdPartySmoothV(&pos, &lbl_1_bss_50[i], 3.0f);
        Hu3DModelPosSetV(obj->mdlId[i], &pos);
        Hu3D3Dto2D(&pos, 1, &pos2D);
        HuSprPosSet(lbl_1_bss_918[3], (s16)i, pos2D.x, pos2D.y);
        HuSprScaleSet(lbl_1_bss_918[3], (s16)i, 0.8f, 0.8f);
        Hu3DModelRotGet(obj->mdlId[i], &pos);
        pos.z = MdPartySmooth(pos.z, 0.0f, 3.0f);
        Hu3DModelRotSetV(obj->mdlId[i], &pos);
        Hu3DModelScaleGet(obj->mdlId[i], &pos);
        pos.x = pos.y = pos.z = MdPartySmooth(pos.x, 0.4f, 3.0f);
        Hu3DModelScaleSetV(obj->mdlId[i], &pos);
    }
    MdPartyIconUpdate();
}

void MdPartyMarkerObjInit(OMOBJ *obj)
{
    s16 i;
    omSetStatBit(obj, OM_STAT_MODELPAUSE);
    for (i = 0; i < 4; i++) {
        if (i == 0) {
            obj->mdlId[i] = Hu3DModelCreateData(DATANUM(DATA_mdparty, 0x1A));
            obj->mtnId[i] = Hu3DMotionIDGet(obj->mdlId[0]);
        } else {
            obj->mdlId[i] = Hu3DModelLink(obj->mdlId[0]);
        }
        Hu3DModelLayerSet(obj->mdlId[i], 1);
        Hu3DMotionShiftSet(obj->mdlId[i], obj->mtnId[0], 0.0f, 0.0f, HU3D_MOTATTR_LOOP);
        MdPartyMarkerHide(i);
    }
    MdPartyIconCloseAll();
    obj->objFunc = MdPartyMarkerUpdate;
}

void MdPartyMarkerObjKill(OMOBJ *obj)
{
    s16 i;
    if (obj) {
        Hu3DMotionKill(obj->mtnId[0]);
        for (i = 3; i >= 0; i--) {
            Hu3DModelKill(obj->mdlId[i]);
        }
        omDelObjEx(lbl_1_bss_0, obj);
    }
    obj = NULL;
}

s16 MdPartyCursorFind(s16 cur, s16 dir1, s16 dir2, s16 dir3, s16 (*link)[9], s16 num)
{
    s16 next;
    s16 i;
    if (dir1 == -1) {
        return -1;
    }
    next = link[cur][dir1];
    if (next == -1) {
        next = MdPartyCursorFind(cur, dir2, dir3, -1, link, num);
    }
    for (i = 0; i < num; i++) {
        if (cur != i && link[i][8] != -1 && next == i) {
            next = MdPartyCursorFind(next, dir1, dir2, dir3, link, num);
            if (next == -1) {
                next = MdPartyCursorFind(cur, dir2, dir3, -1, link, num);
            }
        }
    }
    return next;
}

s16 MdPartyCursorDirFind(s16 cur, s16 (*link)[9], s16 dir, s16 num)
{
    s16 dirTbl[12][3] = {
        { 2, 1, 3 }, { 6, 7, 5 }, { 0, 7, 1 }, { 4, 5, 3 },
        { 1, 2, 0 }, { 3, 2, 4 }, { 7, 6, 0 }, { 5, 6, 4 },
        { 1, 0, 2 }, { 7, 0, 6 }, { 3, 4, 2 }, { 5, 4, 6 }
    };
    return MdPartyCursorFind(cur, dirTbl[dir][0], dirTbl[dir][1], dirTbl[dir][2], link, num);
}

#define MDPARTY_ABS(x) (((x) < 0.0f) ? -(x) : (x))

s16 MdPartyCursorMove(s16 padNo, s16 *cur, s16 (*link)[9], s16 num)
{
    s16 next = 0;
    s16 dir = 0;
    HuVecF stick = { 0.0f, 0.0f, 0.0f };
    if (HuPadDStkRep[padNo] & PAD_BUTTON_UP) {
        stick.y = 1.0f;
    } else if (HuPadDStkRep[padNo] & PAD_BUTTON_DOWN) {
        stick.y = -1.0f;
    }
    if (HuPadDStkRep[padNo] & PAD_BUTTON_LEFT) {
        stick.x = -1.0f;
    } else if (HuPadDStkRep[padNo] & PAD_BUTTON_RIGHT) {
        stick.x = 1.0f;
    }
    if (MDPARTY_ABS(stick.x) < 0.25f && MDPARTY_ABS(stick.y) < 0.25f) {
        return FALSE;
    }
    stick.z = sqrtf((stick.x * stick.x) + (stick.y * stick.y));
    stick.x /= stick.z;
    stick.y /= stick.z;
    if (MDPARTY_ABS(stick.y) < 0.25f) {
        if (stick.x > 0.0f) {
            dir = 0;
        } else {
            dir = 1;
        }
    } else if (MDPARTY_ABS(stick.x) < 0.25f) {
        if (stick.y > 0.0f) {
            dir = 2;
        } else {
            dir = 3;
        }
    } else if (MDPARTY_ABS(stick.x) > MDPARTY_ABS(stick.y)) {
        if (stick.x > 0.0f) {
            if (stick.y > 0.0f) {
                dir = 4;
            } else {
                dir = 5;
            }
        } else {
            if (stick.y > 0.0f) {
                dir = 6;
            } else {
                dir = 7;
            }
        }
    } else {
        if (stick.y > 0.0f) {
            if (stick.x > 0.0f) {
                dir = 8;
            } else {
                dir = 9;
            }
        } else {
            if (stick.x > 0.0f) {
                dir = 10;
            } else {
                dir = 11;
            }
        }
    }
    next = MdPartyCursorDirFind(*cur, link, dir, num);
    if (next != -1) {
        link[*cur][8] = -1;
        *cur = next;
        link[*cur][8] = 1;
        HuAudFXPlay(0);
        return TRUE;
    }
    return FALSE;
}

void MdPartyPanelAnimSet(s16 mdlNo, s16 animSlot, s16 animDataNo, s16 bank)
{
    MDPARTY_MODEL *panel = &lbl_1_bss_288[mdlNo];
    Hu3DAnimAnimSet(panel->animId[animSlot], lbl_1_bss_800[animDataNo]);
    if (bank != -1) {
        Hu3DAnimBankSet(panel->animId[animSlot], bank);
    }
}

void MdPartyPanelAnimSetAll(s16 mdlNo, s16 animDataNo)
{
    s16 i;
    for (i = 0; i < 4; i++) {
        MdPartyPanelAnimSet(mdlNo, i, animDataNo, -1);
    }
}

void MdPartyPanelSpinIn(s16 mdlNo, s16 time, s16 maxTime, HuVecF *pos, float scale)
{
    MDPARTY_MODEL *panel = &lbl_1_bss_288[mdlNo];
    float value;
    if (time == 0) {
        if (lbl_1_bss_34 == 0) {
            HuAudFXPlay(52);
            lbl_1_bss_34 = 1;
        }
        Hu3DModelPosSet(panel->mdlId, pos->x, pos->y, pos->z);
        Hu3DModelRotSet(panel->mdlId, 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(panel->mdlId, 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrReset(panel->mdlId, HU3D_ATTR_DISPOFF);
    } else {
        lbl_1_bss_34 = 0;
    }
    value = MdPartyLerp(-540.0f, 0.0f, time, maxTime);
    Hu3DModelRotSet(panel->mdlId, 0.0f, value, 0.0f);
    value = MdPartyLerp(0.0f, scale, time, maxTime);
    Hu3DModelScaleSet(panel->mdlId, value, value, 1.0f);
}

void MdPartyPanelSpinOut(s16 mdlNo, s16 time, s16 maxTime)
{
    MDPARTY_MODEL *panel = &lbl_1_bss_288[mdlNo];
    float value;
    if (time == 0) {
        if (lbl_1_bss_34 == 0) {
            HuAudFXPlay(53);
            lbl_1_bss_34 = 1;
        }
        Hu3DModelPosGet(panel->mdlId, &panel->pos);
        Hu3DModelRotGet(panel->mdlId, &panel->rot);
        Hu3DModelScaleGet(panel->mdlId, &panel->scale);
    } else {
        lbl_1_bss_34 = 0;
    }
    value = MdPartyLerp(panel->rot.y, panel->rot.y - 540.0f, time, maxTime);
    Hu3DModelRotSet(panel->mdlId, 0.0f, value, 0.0f);
    value = MdPartyLerp(panel->scale.x, 0.0f, time, maxTime);
    Hu3DModelScaleSet(panel->mdlId, value, value, 1.0f);
    if (time <= maxTime) {
        Hu3DModelAttrReset(panel->mdlId, HU3D_ATTR_DISPOFF);
    }
}

void MdPartyPanelMove(s16 mdlNo, s16 time, s16 maxTime, HuVecF *end, float scaleEnd)
{
    MDPARTY_MODEL *panel = &lbl_1_bss_288[mdlNo];
    HuVecF newPos;
    float value;
    if (time == 0) {
        Hu3DModelPosGet(panel->mdlId, &panel->pos);
        Hu3DModelRotGet(panel->mdlId, &panel->rot);
        Hu3DModelScaleGet(panel->mdlId, &panel->scale);
    }
    newPos.x = MdPartyLerp(panel->pos.x, end->x, time, maxTime);
    newPos.y = MdPartyLerp(panel->pos.y, end->y, time, maxTime);
    newPos.z = MdPartyLerp(panel->pos.z, end->z, time, maxTime);
    Hu3DModelPosSet(panel->mdlId, newPos.x, newPos.y, newPos.z);
    value = MdPartyLerp(panel->scale.x, scaleEnd, time, maxTime);
    Hu3DModelScaleSet(panel->mdlId, value, value, 1.0f);
}

void MdPartyPanelFlip(s16 mdlNo, s16 animBase, s16 animNo, s16 time, s16 maxTime, s16 turnNum)
{
    MDPARTY_MODEL *panel = &lbl_1_bss_288[mdlNo];
    HuVecF rot;
    if (time == 0 || time >= maxTime) {
        Hu3DModelRotSet(panel->mdlId, 0.0f, 0.0f, 0.0f);
    }
    if (time >= maxTime) {
        MdPartyPanelAnimSetAll(mdlNo, animNo + animBase);
        return;
    }
    Hu3DModelRotGet(panel->mdlId, &rot);
    rot.y = MdPartyLerp(0.0f, 180.0f * turnNum, time, maxTime);
    if (MDPARTY_ABS(rot.y) >= 90.0f) {
        MdPartyPanelAnimSetAll(mdlNo, animNo + animBase);
    }
    Hu3DModelRotSetV(panel->mdlId, &rot);
}

void MdPartyPanelRotReset(s16 mdlNo, float rotY, s16 time, s16 maxTime)
{
    MDPARTY_MODEL *panel = &lbl_1_bss_288[mdlNo];
    float value;
    if (time == 0) {
        Hu3DModelRotGet(panel->mdlId, &panel->rot);
        if (panel->rot.y - rotY > 180.0f) {
            panel->rot.y -= 360.0f;
        } else if (panel->rot.y - rotY < -180.0f) {
            panel->rot.y += 360.0f;
        }
    }
    value = MdPartyLerp(panel->rot.y, 0.0f, time, maxTime);
    Hu3DModelRotSet(panel->mdlId, 0.0f, value, 0.0f);
}

void MdPartyPanelStatSet(s16 stat)
{
    lbl_1_bss_24->mtnId[0] = stat;
}

s16 MdPartyPanelStatGet(void)
{
    return lbl_1_bss_24->mtnId[0];
}

s16 MdPartyPanelTimeCheck(void)
{
    OMOBJ *obj = lbl_1_bss_24;
    if (obj->work[0] >= 10) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void MdPartyPanelFuncStart(OMOBJ_FUNC func)
{
    OMOBJ *obj = lbl_1_bss_24;
    MdPartyPanelStatSet(0);
    obj->work[0] = 0;
    obj->work[1] = 0;
    obj->work[2] = 0;
    obj->work[3] = 0;
    obj->objFunc = func;
    while (lbl_1_bss_24->mtnId[0] == 0) {
        HuPrcVSleep();
    }
}

void MdPartyPanelInit(void)
{
    char *texName[4] = { "ys77120", "ys77121", "ys77060", "ys77061" };
    s16 i;
    s16 j;
    for (i = 0; i < 41; i++) {
        lbl_1_bss_800[i] = HuSprAnimDataRead(lbl_1_data_800[i]);
    }
    for (i = 0; i < 25; i++) {
        memset(&lbl_1_bss_288[i], 0, sizeof(MDPARTY_MODEL));
        if (i == 0) {
            lbl_1_bss_288[i].mdlId = Hu3DModelCreateData(DATANUM(DATA_mdparty, 0x24));
        } else {
            lbl_1_bss_288[i].mdlId = Hu3DModelLink(lbl_1_bss_288[0].mdlId);
        }
        for (j = 0; j < 4; j++) {
            lbl_1_bss_288[i].animId[j] = Hu3DAnimCreate(lbl_1_bss_800[0], lbl_1_bss_288[i].mdlId, texName[j]);
        }
        Hu3DModelLayerSet(lbl_1_bss_288[i].mdlId, 1);
        Hu3DModelAttrSet(lbl_1_bss_288[i].mdlId, HU3D_ATTR_DISPOFF);
    }
    lbl_1_bss_24 = omAddObjEx(lbl_1_bss_0, 0x1000, 16, 16, -1, NULL);
}

void MdPartyPanelKill(void)
{
    s16 i;
    s16 j;
    if (lbl_1_bss_24) {
        MdPartyPanelStatSet(-1);
        for (i = 24; i >= 0; i--) {
            for (j = 0; j < 4; j++) {
                Hu3DAnimKill(lbl_1_bss_288[i].animId[j]);
            }
            Hu3DModelKill(lbl_1_bss_288[i].mdlId);
        }
    }
    lbl_1_bss_24 = NULL;
}

void MdPartyPanelIntro(OMOBJ *obj)
{
    if (obj->work[0] == 0) {
        fn_1_884C(lbl_1_bss_9F4.boardNo);
        MdPartyPanelAnimSetAll(21, lbl_1_bss_9F4.tagF + 20);
        MdPartyPanelAnimSetAll(22, lbl_1_bss_9F4.turnNo + 22);
        MdPartyPanelAnimSetAll(23, lbl_1_bss_9F4.mgPack + 31);
        MdPartyPanelAnimSetAll(24, lbl_1_bss_9F4.bonusStarF + 36);
    }
    MdPartyPanelSpinIn(21, obj->work[0], 10, &lbl_1_data_8A4[41], 1.5f);
    MdPartyPanelSpinIn(22, obj->work[0], 10, &lbl_1_data_8A4[42], 1.5f);
    MdPartyPanelSpinIn(23, obj->work[0], 10, &lbl_1_data_8A4[43], 1.5f);
    MdPartyPanelSpinIn(24, obj->work[0], 10, &lbl_1_data_8A4[44], 1.5f);
    if (obj->work[0]++ > 10) {
        fn_1_8C28(lbl_1_bss_9F4.boardNo);
        obj->objFunc = NULL;
        MdPartyPanelStatSet(1);
    }
}

/* @@PART5@@ */
void MdPartyGroupBCloseExec(OMOBJ *obj)
{
    if (obj->work[0] == 0) {
        MdPartyDescFadeOutStart();
    }
    MdPartyPanelSpinOut(21, obj->work[0], 10);
    MdPartyPanelSpinOut(22, obj->work[0], 10);
    MdPartyPanelSpinOut(23, obj->work[0], 10);
    MdPartyPanelSpinOut(24, obj->work[0], 10);
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void MdPartyGroupBCloseResetExec(OMOBJ *obj)
{
    if (obj->work[0] == 0) {
        MdPartyPicFadeOut();
    }
    MdPartyPanelSpinOut(21, obj->work[0], 10);
    MdPartyPanelSpinOut(22, obj->work[0], 10);
    MdPartyPanelSpinOut(23, obj->work[0], 10);
    MdPartyPanelSpinOut(24, obj->work[0], 10);
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void MdPartyPlayerNumSelExec(OMOBJ *obj)
{
    s16 numMin = 1;
    s16 num = 0;
    s16 i;
    for (i = 0; i < 4; i++) {
        if (HuPadStatGet(i) == 0) {
            num++;
        }
    }
    if (num < numMin) {
        num = numMin;
    }
    if (num >= lbl_1_bss_9F4.playerNum) {
        if (obj->work[0]++ > 10) {
            if (HuPadDStkRep[0] & PAD_BUTTON_LEFT) {
                lbl_1_bss_9F4.playerNum--;
                if (lbl_1_bss_9F4.playerNum < numMin) {
                    lbl_1_bss_9F4.playerNum = num;
                }
                obj->work[0] = 0;
                obj->work[1] = 0;
                MdPartyIconPush(4);
            } else if (HuPadDStkRep[0] & PAD_BUTTON_RIGHT) {
                lbl_1_bss_9F4.playerNum++;
                if (lbl_1_bss_9F4.playerNum >= num + 1) {
                    lbl_1_bss_9F4.playerNum = numMin;
                }
                obj->work[0] = 0;
                obj->work[1] = 1;
                MdPartyIconPush(5);
            }
            if (num == numMin) {
                obj->work[0] = 12;
                if (obj->work[3] == 1) {
                    obj->work[3] = 0;
                    MdPartyIconClose(4);
                    MdPartyIconClose(5);
                }
            } else if (obj->work[3] == 0) {
                obj->work[3] = 1;
                MdPartyIconOpen(4, lbl_1_data_8A4, -180.0f, 0.0f);
                MdPartyIconOpen(5, lbl_1_data_8A4, 180.0f, 0.0f);
            }
        }
    } else {
        lbl_1_bss_9F4.playerNum = num;
        obj->work[0] = 0;
        obj->work[1] = 0;
    }
    if (obj->work[0] <= 10) {
        for (i = 0; i < 4; i++) {
            if (obj->work[1] != 0) {
                if (i < lbl_1_bss_9F4.playerNum) {
                    MdPartyPanelFlip(i + 17, 0, 19, obj->work[0], 10, 1);
                } else {
                    MdPartyPanelFlip(i + 17, 0, 18, obj->work[0], 10, 1);
                }
            } else {
                if (i < lbl_1_bss_9F4.playerNum) {
                    MdPartyPanelFlip(i + 17, 0, 19, obj->work[0], 10, -1);
                } else {
                    MdPartyPanelFlip(i + 17, 0, 18, obj->work[0], 10, -1);
                }
            }
        }
    }
}

void MdPartyPlayerNumSelStart(OMOBJ *obj)
{
    obj->work[0] = 10;
    obj->objFunc = MdPartyPlayerNumSelExec;
    lbl_1_bss_24->mtnId[0] = 1;
}

void MdPartyPlayerNumSelEnd(OMOBJ *obj)
{
    MdPartyIconClose(4);
    MdPartyIconClose(5);
    obj->objFunc = NULL;
    lbl_1_bss_24->mtnId[0] = 1;
}

void MdPartyPlayerNumOpenExec(OMOBJ *obj)
{
    s16 numMin = 1;
    s16 i;
    for (i = 0; i < 4; i++) {
        MdPartyPanelSpinIn(i + 17, obj->work[0], 10, &lbl_1_data_8A4[i + 33], 1.25f);
    }
    if (obj->work[0] == 0) {
        for (i = 0, lbl_1_bss_9F4.playerNum = 0; i < 4; i++) {
            if (HuPadStatGet(i) == 0) {
                lbl_1_bss_9F4.playerNum++;
            }
        }
        if (lbl_1_bss_9F4.playerNum < numMin) {
            lbl_1_bss_9F4.playerNum = numMin;
        }
        for (i = 0; i < 4; i++) {
            if (i < lbl_1_bss_9F4.playerNum) {
                MdPartyPanelAnimSetAll(i + 17, 19);
            } else {
                MdPartyPanelAnimSetAll(i + 17, 18);
            }
        }
    }
    for (i = 0; i < 4; i++) {
        MdPartyPanelSpinIn(i + 17, obj->work[0], 10, &lbl_1_data_8A4[i + 33], 1.25f);
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

/* @@PART6@@ */

void MdPartyComModelOut(OMOBJ *obj)
{
    s16 i;
    s16 time;
    MDPARTY_MODEL *charP;
    float value;

    for (i = 0; i < 4; i++) {
        time = obj->work[0];
        charP = &lbl_1_bss_288[(s16)(i + 17)];
        if (time == 0) {
            if (lbl_1_bss_34 == 0) {
                HuAudFXPlay(53);
                lbl_1_bss_34 = 1;
            }
            Hu3DModelPosGet(charP->mdlId, &charP->pos);
            Hu3DModelRotGet(charP->mdlId, &charP->rot);
            Hu3DModelScaleGet(charP->mdlId, &charP->scale);
        } else {
            lbl_1_bss_34 = 0;
        }
        value = MdPartyLerp(charP->rot.y, charP->rot.y - 540.0f, time, 10.0f);
        Hu3DModelRotSet(charP->mdlId, 0.0f, value, 0.0f);
        value = MdPartyLerp(charP->scale.x, 0.0f, time, 10.0f);
        Hu3DModelScaleSet(charP->mdlId, value, value, 1.0f);
        if (time <= 10) {
            Hu3DModelAttrReset(charP->mdlId, 1);
        }
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void MdPartyCharModelSpin(s16 charNo)
{
    MDPARTY_MODEL *charP = &lbl_1_bss_288[charNo];
    HuVecF rot;

    Hu3DModelRotGet(charP->mdlId, &rot);
    rot.y += 6.0f;
    if (rot.y > 360.0f) {
        rot.y -= 360.0f;
    }
    Hu3DModelRotSetV(charP->mdlId, &rot);
}

void MdPartyCharModelRotReset(s16 charNo)
{
    MDPARTY_MODEL *charP = &lbl_1_bss_288[charNo];

    Hu3DModelRotSet(charP->mdlId, 0.0f, 0.0f, 0.0f);
}

MDPARTY_CHARSLOT lbl_1_data_E12[11] = {
    { { -1, -1, 1, 5, 4, -1, -1, -1 }, -1 },
    { { -1, -1, 2, 6, 5, 4, 0, -1 }, -1 },
    { { -1, -1, 3, -1, 6, 5, 1, -1 }, -1 },
    { { -1, -1, -1, -1, -1, 6, 2, -1 }, -1 },
    { { 0, 1, 5, 9, 8, 7, -1, -1 }, -1 },
    { { 1, 2, 6, 10, 9, 8, 4, 0 }, -1 },
    { { 2, 3, 10, 10, 10, 9, 5, 1 }, -1 },
    { { -1, 4, 8, -1, -1, -1, -1, -1 }, -1 },
    { { 4, 5, 9, -1, -1, -1, 7, -1 }, -1 },
    { { 5, 6, 10, -1, -1, -1, 8, 4 }, -1 },
    { { 6, -1, -1, -1, -1, -1, 9, 5 }, -1 },
};

void MdPartyCharSelMain(OMOBJ *obj)
{
    s16 i;
    s16 j;
    MDPARTY_PLAYER *player;

    for (i = 0, player = lbl_1_bss_9BC; i < 4; i++, player++) {
        if (player->com != 0) {
            continue;
        }
        if (player->state == 0) {
            if (obj->work[i]++ > 10) {
                if (MdPartyCursorMove(player->padNo, &player->charNo, (s16 (*)[9])lbl_1_data_E12, 11) != 0) {
                    MdPartyMarkerPosSet(i, &lbl_1_data_8A4[player->charNo + 14]);
                    obj->work[i] = 0;
                } else if (HuPadBtnDown[player->padNo] & PAD_BUTTON_A) {
                    if (lbl_1_bss_2A == 0 && player->charNo == 10) {
                        HuAudFXPlay(4);
                    } else {
                        CharFXPlay(player->charNo, 0x24B);
                        player->state = 1;
                        MdPartyMarkerHide(i);
                        MdPartyIconAOn(i, player->padNo);
                        MdPartyPanelAnimSet((s16)player->charNo, 0, (s16)player->charNo, 1);
                        MdPartyPanelAnimSet((s16)player->charNo, 2, (s16)player->charNo, 1);
                        fn_1_45C40(i, &lbl_1_data_8A4[player->charNo + 14], player->padNo);
                    }
                }
            }
        } else if (HuPadBtnDown[player->padNo] & PAD_BUTTON_B) {
            HuAudFXPlay(3);
            player->state = 0;
            MdPartyMarkerShow(i, &lbl_1_data_8A4[player->charNo + 14], player->padNo);
            MdPartyIconAOff(i);
            MdPartyPanelAnimSet((s16)player->charNo, 0, (s16)player->charNo, 0);
            MdPartyPanelAnimSet((s16)player->charNo, 2, (s16)player->charNo, 0);
        }
    }
    for (i = 0; i < 11; i++) {
        for (j = 0, player = lbl_1_bss_9BC; j < 4; j++, player++) {
            if (player->com == 0 && player->charNo == i && player->state == 0) {
                break;
            }
        }
        if (j == 4) {
            MdPartyCharModelRotReset(i);
        } else {
            MdPartyCharModelSpin(i);
        }
    }
}

void MdPartyCharSelInit(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;

    for (i = 0, player = lbl_1_bss_9BC; i < 4; i++, player++) {
        if (player->com == 0 && player->state == 0) {
            lbl_1_data_E12[player->charNo].owner = 1;
            MdPartyMarkerShow(i, &lbl_1_data_8A4[player->charNo + 14], player->padNo);
        }
    }
    obj->work[0] = 10;
    obj->objFunc = MdPartyCharSelMain;
    lbl_1_bss_24->mtnId[0] = 1;
}

void MdPartyCharSelClose(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;

    for (i = 0, player = lbl_1_bss_9BC; i < 4; i++, player++) {
        if (player->com == 0 && player->state == 0) {
            MdPartyMarkerHide(i);
        }
    }
    for (i = 0; i < 11; i++) {
        MdPartyCharModelRotReset(i);
    }
    obj->objFunc = NULL;
    lbl_1_bss_24->mtnId[0] = 1;
}

void MdPartyCharSelReset(OMOBJ *obj)
{
    MDPARTY_PLAYER *player = lbl_1_bss_9BC;

    player->state = 0;
    MdPartyIconAOff(0);
    MdPartyPanelAnimSet((s16)player->charNo, 0, (s16)player->charNo, 0);
    MdPartyPanelAnimSet((s16)player->charNo, 2, (s16)player->charNo, 0);
    obj->objFunc = NULL;
    lbl_1_bss_24->mtnId[0] = 1;
}

void MdPartyComSelMain(OMOBJ *obj)
{
    s16 i;
    s16 j;
    MDPARTY_PLAYER *player;
    HuVecF pos;

    for (i = 0, player = lbl_1_bss_9BC; i < 4; i++, player++) {
        if (player->com != 1) {
            continue;
        }
        if (player->state == 2) {
            continue;
        }
        if (player->state == 0) {
            if (obj->work[0]++ > 10) {
                if (MdPartyCursorMove(0, &player->charNo, (s16 (*)[9])lbl_1_data_E12, 11) != 0) {
                    MdPartyMarkerPosSet(i, &lbl_1_data_8A4[player->charNo + 14]);
                    obj->work[0] = 0;
                } else if (HuPadBtnDown[0] & PAD_BUTTON_A) {
                    if (lbl_1_bss_2A == 0 && player->charNo == 10) {
                        HuAudFXPlay(4);
                    } else {
                        CharFXPlay(player->charNo, 0x24B);
                        player->state = 1;
                        player->comDif = 1;
                        MdPartyMarkerHide(i);
                        MdPartyIconAOn(i, player->comDif + 5);
                        MdPartyPanelAnimSet((s16)player->charNo, 0, (s16)player->charNo, 1);
                        MdPartyPanelAnimSet((s16)player->charNo, 2, (s16)player->charNo, 1);
                        Hu3DModelPosGet(lbl_1_bss_288[player->charNo].mdlId, &pos);
                        MdPartyIconOpen(4, &pos, -40.0f, -45.0f);
                        MdPartyIconOpen(5, &pos, 40.0f, -45.0f);
                        fn_1_45C40(i, &lbl_1_data_8A4[player->charNo + 14], 4);
                    }
                } else if (HuPadBtnDown[0] & PAD_BUTTON_B) {
                    HuAudFXPlay(3);
                    lbl_1_data_E12[player->charNo].owner = -1;
                    MdPartyMarkerHide(i);
                    for (j = 3; j >= 0; j--) {
                        if (lbl_1_bss_9BC[j].com == 1 && lbl_1_bss_9BC[j].state == 2) {
                            lbl_1_bss_9BC[j].state = 0;
                            lbl_1_data_E12[lbl_1_bss_9BC[j].charNo].owner = -1;
                            MdPartyIconAOff(j);
                            MdPartyPanelAnimSet((s16)lbl_1_bss_9BC[j].charNo, 0, (s16)lbl_1_bss_9BC[j].charNo, 0);
                            MdPartyPanelAnimSet((s16)lbl_1_bss_9BC[j].charNo, 2, (s16)lbl_1_bss_9BC[j].charNo, 0);
                            break;
                        }
                    }
                    if (j != -1) {
                        obj->objFunc = MdPartyComSelInit;
                    } else {
                        obj->objFunc = NULL;
                    }
                }
            }
        } else if (player->state == 1) {
            if (obj->work[0]++ > 10) {
                if (HuPadDStkRep[0] & PAD_BUTTON_LEFT) {
                    player->comDif--;
                    if (lbl_1_bss_2C == 0) {
                        if (player->comDif < 0) {
                            player->comDif += 3;
                        }
                    } else if (player->comDif < 0) {
                        player->comDif += 4;
                    }
                    obj->work[0] = 0;
                    MdPartyIconPush(4);
                } else if (HuPadDStkRep[0] & PAD_BUTTON_RIGHT) {
                    player->comDif++;
                    if (lbl_1_bss_2C == 0) {
                        if (player->comDif > 2) {
                            player->comDif -= 3;
                        }
                    } else if (player->comDif > 3) {
                        player->comDif -= 4;
                    }
                    obj->work[0] = 0;
                    MdPartyIconPush(5);
                } else if (HuPadBtnDown[0] & PAD_BUTTON_A) {
                    HuAudFXPlay(1);
                    player->state = 2;
                    MdPartyIconClose(4);
                    MdPartyIconClose(5);
                    for (j = 0; j < 4; j++) {
                        if (lbl_1_bss_9BC[j].com == 1 && lbl_1_bss_9BC[j].state == 0) {
                            break;
                        }
                    }
                    if (j != 4) {
                        obj->objFunc = MdPartyComSelInit;
                    } else {
                        obj->objFunc = NULL;
                    }
                } else if (HuPadBtnDown[0] & PAD_BUTTON_B) {
                    HuAudFXPlay(3);
                    player->state = 0;
                    MdPartyIconClose(4);
                    MdPartyIconClose(5);
                    MdPartyPanelAnimSet((s16)player->charNo, 0, (s16)player->charNo, 0);
                    MdPartyPanelAnimSet((s16)player->charNo, 2, (s16)player->charNo, 0);
                    MdPartyMarkerShow(i, &lbl_1_data_8A4[player->charNo + 14], 4);
                    MdPartyIconAOff(i);
                }
            }
            MdPartyIconABankSet(i, player->comDif + 5);
        }
        break;
    }
    for (i = 0; i < 11; i++) {
        if (player->charNo == i && player->state == 0) {
            MdPartyCharModelSpin(i);
        } else {
            MdPartyCharModelRotReset(i);
        }
    }
}

void MdPartyComSelInit(OMOBJ *obj)
{
    s16 i;
    s16 j;
    MDPARTY_PLAYER *player;
    HuVecF pos;

    for (i = 0, player = lbl_1_bss_9BC; i < 4; i++, player++) {
        if (player->com != 1) {
            continue;
        }
        if (player->state == 0) {
            for (j = 0; j < 11; j++) {
                if (lbl_1_data_E12[j].owner == -1) {
                    player->charNo = j;
                    lbl_1_data_E12[j].owner = 1;
                    break;
                }
            }
            MdPartyMarkerShow(i, &lbl_1_data_8A4[player->charNo + 14], 4);
            break;
        }
        if (player->state == 1) {
            Hu3DModelPosGet(lbl_1_bss_288[player->charNo].mdlId, &pos);
            MdPartyIconOpen(4, &pos, -40.0f, -45.0f);
            MdPartyIconOpen(5, &pos, 40.0f, -45.0f);
            break;
        }
    }
    obj->work[0] = 10;
    obj->objFunc = MdPartyComSelMain;
    lbl_1_bss_24->mtnId[0] = 1;
}

void MdPartyComSelClose(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;

    for (i = 0, player = lbl_1_bss_9BC; i < 4; i++, player++) {
        if (player->com != 1) {
            continue;
        }
        if (player->state == 0) {
            lbl_1_data_E12[player->charNo].owner = -1;
            MdPartyMarkerHide(i);
            break;
        }
        if (player->state == 1) {
            MdPartyIconClose(4);
            MdPartyIconClose(5);
            break;
        }
    }
    for (i = 0; i < 11; i++) {
        MdPartyCharModelRotReset(i);
    }
    obj->objFunc = NULL;
    lbl_1_bss_24->mtnId[0] = 1;
}

void MdPartyComCharSelMain(OMOBJ *obj)
{
    s16 i;
    s16 j;
    MDPARTY_PLAYER *player;

    for (i = 0, player = lbl_1_bss_9BC; i < 4; i++, player++) {
        if (player->com != 1 || player->state == 1) {
            continue;
        }
        if (player->state == 0) {
            if (obj->work[0]++ <= 10) {
                break;
            }
            if (MdPartyCursorMove(0, &player->charNo, (s16 (*)[9])lbl_1_data_E12, 11) != 0) {
                MdPartyMarkerPosSet(i, &lbl_1_data_8A4[player->charNo + 14]);
                obj->work[0] = 0;
                break;
            }
            if (HuPadBtnDown[0] & PAD_BUTTON_A) {
                if (lbl_1_bss_2A == 0 && player->charNo == 10) {
                    HuAudFXPlay(4);
                    break;
                }
                CharFXPlay(player->charNo, 0x24B);
                player->state = 1;
                player->comDif = 1;
                MdPartyMarkerHide(i);
                MdPartyIconAOn(i, 4);
                MdPartyPanelAnimSet((s16)player->charNo, 0, (s16)player->charNo, 1);
                MdPartyPanelAnimSet((s16)player->charNo, 2, (s16)player->charNo, 1);
                fn_1_45C40(i, &lbl_1_data_8A4[player->charNo + 14], 4);
                for (j = 0; j < 4; j++) {
                    if (lbl_1_bss_9BC[j].com == 1 && lbl_1_bss_9BC[j].state == 0) {
                        break;
                    }
                }
                if (j != 4) {
                    obj->objFunc = MdPartyComCharSelInit;
                } else {
                    obj->objFunc = NULL;
                }
                break;
            }
            if (HuPadBtnDown[0] & PAD_BUTTON_B) {
                HuAudFXPlay(3);
                lbl_1_data_E12[player->charNo].owner = -1;
                MdPartyMarkerHide(i);
                for (j = 3; j >= 0; j--) {
                    if (lbl_1_bss_9BC[j].com == 1 && lbl_1_bss_9BC[j].state == 1) {
                        lbl_1_bss_9BC[j].state = 0;
                        lbl_1_data_E12[lbl_1_bss_9BC[j].charNo].owner = -1;
                        MdPartyIconAOff(j);
                        MdPartyPanelAnimSet((s16)lbl_1_bss_9BC[j].charNo, 0, (s16)lbl_1_bss_9BC[j].charNo, 0);
                        MdPartyPanelAnimSet((s16)lbl_1_bss_9BC[j].charNo, 2, (s16)lbl_1_bss_9BC[j].charNo, 0);
                        break;
                    }
                }
                if (j != -1) {
                    obj->objFunc = MdPartyComCharSelInit;
                } else {
                    obj->objFunc = NULL;
                }
            }
        }
        break;
    }
    for (i = 0; i < 11; i++) {
        if (player->charNo == i && player->state == 0) {
            MdPartyCharModelSpin(i);
        } else {
            MdPartyCharModelRotReset(i);
        }
    }
}

void MdPartyComCharSelInit(OMOBJ *obj)
{
    s16 i;
    s16 j;
    MDPARTY_PLAYER *player;

    for (i = 0, player = lbl_1_bss_9BC; i < 4; i++, player++) {
        if (player->com == 1 && player->state == 0) {
            for (j = 0; j < 11; j++) {
                if (lbl_1_data_E12[j].owner == -1) {
                    player->charNo = j;
                    lbl_1_data_E12[j].owner = 1;
                    break;
                }
            }
            MdPartyMarkerShow(i, &lbl_1_data_8A4[player->charNo + 14], 4);
            break;
        }
    }
    obj->work[0] = 10;
    obj->objFunc = MdPartyComCharSelMain;
    lbl_1_bss_24->mtnId[0] = 1;
}

void MdPartyComCharSelClose(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;

    for (i = 0, player = lbl_1_bss_9BC; i < 4; i++, player++) {
        if (player->com == 1 && player->state == 0) {
            lbl_1_data_E12[player->charNo].owner = -1;
            MdPartyMarkerHide(i);
            break;
        }
    }
    for (i = 0; i < 11; i++) {
        MdPartyCharModelRotReset(i);
    }
    obj->objFunc = NULL;
    lbl_1_bss_24->mtnId[0] = 1;
}

void MdPartyCharSelOpen(OMOBJ *obj)
{
    s16 i;
    s16 j;
    s16 time;
    MDPARTY_PLAYER *player;
    MDPARTY_MODEL *charP;
    OMOBJ *mainObj;
    float value;

    if (obj->work[0] == 0) {
        mainObj = lbl_1_bss_8;
        mainObj->work[2] = 2;
        mainObj->work[3] = 0;
        for (i = 0; i < 11; i++) {
            MdPartyPanelAnimSetAll(i, i);
            MdPartyPanelAnimSet(i, 1, i, 2);
            MdPartyPanelAnimSet(i, 3, i, 2);
            lbl_1_data_E12[i].owner = -1;
        }
        if (lbl_1_bss_2A == 0) {
            MdPartyPanelAnimSetAll(10, 11);
        }
        for (j = 0, player = lbl_1_bss_9BC; j < 4; j++, player++) {
            player->state = 0;
            player->unk2 = 0;
            player->comDif = 0;
            player->charNo = j;
            player->padNo = j;
        }
    }
    for (i = 0; i < 11; i++) {
        time = obj->work[0];
        charP = &lbl_1_bss_288[i];
        if (time == 0) {
            if (lbl_1_bss_34 == 0) {
                HuAudFXPlay(52);
                lbl_1_bss_34 = 1;
            }
            Hu3DModelPosSet(charP->mdlId, lbl_1_data_8A4[i + 14].x, lbl_1_data_8A4[i + 14].y, lbl_1_data_8A4[i + 14].z);
            Hu3DModelRotSet(charP->mdlId, 0.0f, 0.0f, 0.0f);
            Hu3DModelScaleSet(charP->mdlId, 0.0f, 0.0f, 0.0f);
            Hu3DModelAttrReset(charP->mdlId, 1);
        } else {
            lbl_1_bss_34 = 0;
        }
        value = MdPartyLerp(-540.0f, 0.0f, time, 10.0f);
        Hu3DModelRotSet(charP->mdlId, 0.0f, value, 0.0f);
        value = MdPartyLerp(0.0f, 1.25f, time, 10.0f);
        Hu3DModelScaleSet(charP->mdlId, value, value, 1.0f);
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

/* @@PART7@@ */
/* Defined later in the unit (part 20) */

void MdPartyCharSelOutA(OMOBJ *obj)
{
    s16 i;
    s16 j;
    for (i = 0; i < 11; i++) {
        for (j = 0; j < 4; j++) {
            if (i == lbl_1_bss_9BC[j].charNo) {
                break;
            }
        }
        if (j == 4) {
            fn_1_C698(i, obj->work[0], 10);
        }
    }
    if (obj->work[0] >= 10) {
        for (i = 0; i < 4; i++) {
            fn_1_CA68((s16)lbl_1_bss_9BC[i].charNo, obj->work[0] - 10, 10, &lbl_1_data_8A4[i + 45], 1.25f);
        }
    }
    if (obj->work[0]++ > 20) {
        fn_1_884C(lbl_1_bss_9F4.boardNo);
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void MdPartyCharSelOutB(OMOBJ *obj)
{
    s16 i;
    s16 j;
    for (i = 0; i < 11; i++) {
        for (j = 0; j < 4; j++) {
            if (i == lbl_1_bss_9BC[j].charNo) {
                break;
            }
        }
        if (j == 4) {
            fn_1_C698(i, obj->work[0], 10);
        }
    }
    if (obj->work[0] >= 10) {
        fn_1_884C(lbl_1_bss_9F4.boardNo);
        for (i = 0; i < 4; i++) {
            fn_1_CA68((s16)lbl_1_bss_9BC[i].charNo, obj->work[0] - 10, 10, &lbl_1_data_8A4[i + 49], 1.25f);
        }
    }
    if (obj->work[0]++ > 20) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void MdPartyPlayerHide(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;
    if (obj->work[0] == 0) {
        for (i = 0; i < 4; i++) {
            lbl_1_bss_D4[i].x = 0.0f;
        }
    }
    for (i = 0, player = lbl_1_bss_9BC; i < 4; i++, player++) {
        fn_1_C698((s16)player->charNo, obj->work[0], 10);
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void MdPartyCharAllHide(OMOBJ *obj)
{
    s16 i;
    if (obj->work[0] == 0) {
        fn_1_884C(lbl_1_bss_9F4.boardNo);
        for (i = 0; i < 4; i++) {
            lbl_1_bss_D4[i].x = 0.0f;
        }
    }
    for (i = 0; i < 11; i++) {
        fn_1_C698(i, obj->work[0], 10);
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void MdPartyBoardFocus(s16 no)
{
    MDPARTY_MODEL *mdl = &lbl_1_bss_288[no + 11];
    HuVecF vec;
    Hu3DModelPosGet(mdl->mdlId, &vec);
    MdPartySmoothV(&vec, &lbl_1_data_8A4[no + 7], 5.0f);
    Hu3DModelPosSetV(mdl->mdlId, &vec);
    if (mdl->time++ > 30) {
        mdl->time = 35;
        Hu3DModelRotGet(mdl->mdlId, &vec);
        vec.y += 3.0f;
        if (vec.y >= 360.0f) {
            vec.y -= 360.0f;
        }
        Hu3DModelRotSetV(mdl->mdlId, &vec);
    }
    Hu3DModelScaleGet(mdl->mdlId, &vec);
    vec.y = vec.x = MdPartySmooth(vec.x, 2.25f, 5.0f);
    Hu3DModelScaleSetV(mdl->mdlId, &vec);
    Hu3DModelLayerSet(mdl->mdlId, 2);
}

void MdPartyBoardUnfocus(s16 no)
{
    MDPARTY_MODEL *mdl = &lbl_1_bss_288[no + 11];
    Hu3DModelPosSetV(mdl->mdlId, &lbl_1_data_8A4[no + 1]);
    Hu3DModelRotSet(mdl->mdlId, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(mdl->mdlId, 1.25f, 1.25f, 1.0f);
    mdl->time = 0;
    Hu3DModelLayerSet(mdl->mdlId, 1);
}

void MdPartyBoardSelUpdate(OMOBJ *obj)
{
    s16 i;
    if (obj->work[0]++ >= 10) {
        if (HuPadDStkRep[0] & PAD_BUTTON_LEFT) {
            lbl_1_bss_9F4.boardNo--;
            if (lbl_1_bss_9F4.boardNo < 0) {
                lbl_1_bss_9F4.boardNo += 6;
            }
            obj->work[0] = 0;
            obj->work[1] = 0;
            fn_1_884C(lbl_1_bss_9F4.boardNo);
            lbl_1_bss_38[0] = 2.0f;
            if (lbl_1_data_DE6[0] == 1) {
                HuAudFXPlay(0);
            }
        }
        else if (HuPadDStkRep[0] & PAD_BUTTON_RIGHT) {
            lbl_1_bss_9F4.boardNo++;
            if (lbl_1_bss_9F4.boardNo >= 6) {
                lbl_1_bss_9F4.boardNo -= 6;
            }
            obj->work[0] = 0;
            obj->work[1] = 1;
            fn_1_884C(lbl_1_bss_9F4.boardNo);
            lbl_1_bss_38[1] = 2.0f;
            if (lbl_1_data_DE6[1] == 1) {
                HuAudFXPlay(0);
            }
        }
    }
    for (i = 0; i < 6; i++) {
        if (i == lbl_1_bss_9F4.boardNo) {
            MdPartyBoardFocus(i);
        }
        else {
            MdPartyBoardUnfocus(i);
        }
    }
}

void MdPartyBoardSelStart(OMOBJ *obj)
{
    s16 i;
    for (i = 0; i < 6; i++) {
        lbl_1_bss_288[i + 11].time = 0;
    }
    fn_1_A970(0, lbl_1_data_8A4, -240.0f, 0.0f);
    fn_1_A970(1, lbl_1_data_8A4, 240.0f, 0.0f);
    obj->work[0] = 10;
    obj->objFunc = MdPartyBoardSelUpdate;
    lbl_1_bss_24->mtnId[0] = 1;
}

void MdPartyBoardRotReset(OMOBJ *obj)
{
    s16 i;
    if (obj->work[0] == 0) {
        HuSprAttrSet(lbl_1_bss_918[2], 0, 4);
        lbl_1_data_DE6[0] = 0;
        HuSprAttrSet(lbl_1_bss_918[2], 1, 4);
        lbl_1_data_DE6[1] = 0;
    }
    for (i = 0; i < 6; i++) {
        lbl_1_bss_288[i + 11].time = 0;
        fn_1_D3E0(i + 11, 0.0f, obj->work[0], 10);
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void MdPartyBoardAppear(OMOBJ *obj)
{
    s16 i;
    lbl_1_bss_9F4.boardNo = 0;
    if (obj->work[0] == 0) {
        for (i = 0; i < 6; i++) {
            fn_1_C200(i + 11, i + 12);
            fn_1_C158(i + 11, 1, i + 12, 1);
            fn_1_C158(i + 11, 3, i + 12, 1);
            if (lbl_1_data_BEE[i] == 0) {
                fn_1_C158(i + 11, 2, 40, -1);
                fn_1_C158(i + 11, 3, 40, -1);
            }
        }
    }
    for (i = 0; i < 6; i++) {
        fn_1_C28C(i + 11, obj->work[0], 10, &lbl_1_data_8A4[i + 1], 1.25f);
    }
    if (obj->work[0]++ > 10) {
        Hu3DZClearLayerSet(2);
        fn_1_884C(lbl_1_bss_9F4.boardNo);
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void MdPartyBoardSelWait(OMOBJ *obj)
{
    if (obj->work[0] == 0) {
        fn_1_8CCC();
    }
    if (obj->work[0]++ > 30) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void MdPartyBoardDecide(OMOBJ *obj)
{
    s16 i;
    HuVecF rot;
    for (i = 0; i < 6; i++) {
        if (i != lbl_1_bss_9F4.boardNo) {
            fn_1_C698(i + 11, obj->work[0], 10);
        }
    }
    if (obj->work[0] >= 10) {
        fn_1_CA68(lbl_1_bss_9F4.boardNo + 11, obj->work[0] - 10, 10, &lbl_1_data_8A4[13], 1.5f);
        Hu3DModelRotGet(lbl_1_bss_288[lbl_1_bss_9F4.boardNo + 11].mdlId, &rot);
        rot.y += 54.0f;
        Hu3DModelRotSetV(lbl_1_bss_288[lbl_1_bss_9F4.boardNo + 11].mdlId, &rot);
    }
    if (obj->work[0] == 20) {
        Hu3DLayerHookReset(2);
        fn_1_45A48(&lbl_1_data_8A4[13]);
        HuAudFXPlay(0x49E);
        fn_1_8C28(lbl_1_bss_9F4.boardNo);
        Hu3DModelAttrSet(lbl_1_bss_288[lbl_1_bss_9F4.boardNo + 11].mdlId, HU3D_ATTR_DISPOFF);
    }
    if (obj->work[0]++ > 90) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

/* @@PART8@@ */
/* never emitted on its own: every call is inlined */
static inline void MdPartyCharMoveScale(s16 no, s16 time, HuVecF *end, float scale)
{
    HuVecF pos;
    MDPARTY_MODEL *model;
    float value;

    model = &lbl_1_bss_288[no];
    if (time == 0) {
        Hu3DModelPosGet(model->mdlId, &model->pos);
        Hu3DModelRotGet(model->mdlId, &model->rot);
        Hu3DModelScaleGet(model->mdlId, &model->scale);
    }
    pos.x = MdPartyLerp(model->pos.x, end->x, time, 10.0f);
    pos.y = MdPartyLerp(model->pos.y, end->y, time, 10.0f);
    pos.z = MdPartyLerp(model->pos.z, end->z, time, 10.0f);
    Hu3DModelPosSet(model->mdlId, pos.x, pos.y, pos.z);
    value = MdPartyLerp(model->scale.x, scale, time, 10.0f);
    Hu3DModelScaleSet(model->mdlId, value, value, 1.0f);
}

void MdPartyMdlSpinHide(OMOBJ *obj)
{
    s16 i;
    s16 time;
    MDPARTY_MODEL *model;
    float value;

    if (obj->work[0] == 0) {
        OMOBJ *mainObj = lbl_1_bss_8;
        mainObj->work[2] = 2;
        mainObj->work[3] = 0;
    }
    for (i = 0; i < 6; i++) {
        time = obj->work[0];
        model = &lbl_1_bss_288[(s16)(i + 11)];
        if (time == 0) {
            if (!lbl_1_bss_34) {
                HuAudFXPlay(53);
                lbl_1_bss_34 = 1;
            }
            Hu3DModelPosGet(model->mdlId, &model->pos);
            Hu3DModelRotGet(model->mdlId, &model->rot);
            Hu3DModelScaleGet(model->mdlId, &model->scale);
        } else {
            lbl_1_bss_34 = 0;
        }
        value = MdPartyLerp(model->rot.y, model->rot.y - 540.0f, time, 10.0f);
        Hu3DModelRotSet(model->mdlId, 0.0f, value, 0.0f);
        value = MdPartyLerp(model->scale.x, 0.0f, time, 10.0f);
        Hu3DModelScaleSet(model->mdlId, value, value, 1.0f);
        if (time <= 10) {
            Hu3DModelAttrReset(model->mdlId, HU3D_ATTR_DISPOFF);
        }
    }
    if (obj->work[0]++ > 10) {
        Hu3DLayerHookReset(2);
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void MdPartyHandicapSelect(OMOBJ *obj)
{
    HuVecF pos;

    if (obj->work[0]++ >= 10) {
        if (HuPadDStkRep[0] & PAD_BUTTON_LEFT) {
            lbl_1_bss_9F4.handicapCursor--;
            if (lbl_1_bss_9F4.handicapCursor < 0) {
                lbl_1_bss_9F4.handicapCursor += 4;
            }
            obj->work[0] = 0;
            MdPartyIconPush(4);
        } else if (HuPadDStkRep[0] & PAD_BUTTON_RIGHT) {
            lbl_1_bss_9F4.handicapCursor++;
            if (lbl_1_bss_9F4.handicapCursor > 3) {
                lbl_1_bss_9F4.handicapCursor -= 4;
            }
            obj->work[0] = 0;
            MdPartyIconPush(5);
        } else if (HuPadDStkRep[0] & PAD_BUTTON_DOWN) {
            lbl_1_bss_9BC[lbl_1_bss_9F4.handicapCursor].handicap--;
            if (lbl_1_bss_9BC[lbl_1_bss_9F4.handicapCursor].handicap < 0) {
                lbl_1_bss_9BC[lbl_1_bss_9F4.handicapCursor].handicap += 10;
            }
            obj->work[0] = 0;
            MdPartyIconPush(3);
            MdPartyIconBBankSet(lbl_1_bss_9F4.handicapCursor, lbl_1_bss_9BC[lbl_1_bss_9F4.handicapCursor].handicap);
        } else if (HuPadDStkRep[0] & PAD_BUTTON_UP) {
            lbl_1_bss_9BC[lbl_1_bss_9F4.handicapCursor].handicap++;
            if (lbl_1_bss_9BC[lbl_1_bss_9F4.handicapCursor].handicap > 9) {
                lbl_1_bss_9BC[lbl_1_bss_9F4.handicapCursor].handicap -= 10;
            }
            obj->work[0] = 0;
            MdPartyIconPush(2);
            MdPartyIconBBankSet(lbl_1_bss_9F4.handicapCursor, lbl_1_bss_9BC[lbl_1_bss_9F4.handicapCursor].handicap);
        }
    }
    Hu3DModelPosGet(lbl_1_bss_288[lbl_1_bss_9BC[lbl_1_bss_9F4.handicapCursor].charNo].mdlId, &pos);
    MdPartyMarkPosSet(2, &pos, 0.0f, -80.0f);
    MdPartyMarkPosSet(3, &pos, 0.0f, 80.0f);
}

void MdPartyHandicapSelectStart(OMOBJ *obj)
{
    HuVecF pos;

    Hu3DModelPosGet(lbl_1_bss_288[lbl_1_bss_9BC[lbl_1_bss_9F4.handicapCursor].charNo].mdlId, &pos);
    MdPartyIconOpen(2, &pos, 0.0f, -80.0f);
    MdPartyIconOpen(3, &pos, 0.0f, 80.0f);
    pos.x = 0.0f;
    MdPartyIconOpen(4, &pos, -180.0f, 0.0f);
    MdPartyIconOpen(5, &pos, 180.0f, 0.0f);
    obj->work[0] = 10;
    obj->objFunc = MdPartyHandicapSelect;
    lbl_1_bss_24->mtnId[0] = 1;
}

void MdPartyHandicapSelectEnd(OMOBJ *obj)
{
    MdPartyIconClose(2);
    MdPartyIconClose(3);
    MdPartyIconClose(4);
    MdPartyIconClose(5);
    obj->objFunc = NULL;
    lbl_1_bss_24->mtnId[0] = 1;
}

void MdPartyHandicapMoveIn(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;

    lbl_1_bss_9F4.handicapCursor = 0;
    for (i = 0, player = lbl_1_bss_9BC; i < 4; i++, player++) {
        MdPartyCharMoveScale((s16)player->charNo, obj->work[0], &lbl_1_data_8A4[i + 45], 1.25f);
    }
    if (obj->work[0]++ > 10) {
        for (i = 0; i < 4; i++) {
            lbl_1_bss_9BC[i].handicap = 0;
            MdPartyIconBOn(i, lbl_1_bss_9BC[i].handicap);
        }
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void MdPartyPlayerMoveIn(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;

    lbl_1_bss_9F4.handicapCursor = 0;
    for (i = 0, player = lbl_1_bss_9BC; i < 4; i++, player++) {
        MdPartyCharMoveScale((s16)player->charNo, obj->work[0], &lbl_1_data_8A4[i + 45], 1.25f);
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void MdPartyPlayerMoveBack(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;

    for (i = 0, player = lbl_1_bss_9BC; i < 4; i++, player++) {
        MdPartyCharMoveScale((s16)player->charNo, obj->work[0], &lbl_1_data_8A4[i + 29], 1.25f);
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void MdPartyHandicapMoveOut(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;

    if (obj->work[0] == 0) {
        for (i = 0; i < 4; i++) {
            MdPartyIconBOff(i);
        }
    }
    for (i = 0, player = lbl_1_bss_9BC; i < 4; i++, player++) {
        MdPartyCharMoveScale((s16)player->charNo, obj->work[0], &lbl_1_data_8A4[i + 45], 1.25f);
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void MdPartyTeamArrange(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;

    MdPartyCharMoveScale(21, obj->work[0], &lbl_1_data_8A4[41], 1.5f);
    MdPartyCharMoveScale(22, obj->work[0], &lbl_1_data_8A4[42], 1.5f);
    MdPartyCharMoveScale(23, obj->work[0], &lbl_1_data_8A4[43], 1.5f);
    MdPartyCharMoveScale(24, obj->work[0], &lbl_1_data_8A4[44], 1.5f);
    if (lbl_1_bss_9F4.tagF == 0) {
        for (i = 0, player = lbl_1_bss_9BC; i < 4; i++, player++) {
            MdPartyCharMoveScale((s16)player->charNo, obj->work[0], &lbl_1_data_8A4[i + 29], 1.25f);
        }
    } else {
        for (i = 0; i < 4; i++) {
            player = &lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][i]];
            MdPartyCharMoveScale((s16)player->charNo, obj->work[0], &lbl_1_data_8A4[i + 57], 1.25f);
        }
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

/* @@PART9@@ */

void MdPartyMdlSpinOut(OMOBJ *obj)
{
    MDPARTY_PLAYER *player;
    s16 i;
    if (obj->work[0] == 0) {
        fn_1_8950();
        for (i = 0; i < 4; i++) {
            fn_1_9B68(i);
            fn_1_9CC8(i);
        }
        fn_1_9E28(0);
        fn_1_9E28(1);
        fn_1_9F34();
    }
    fn_1_C698(21, obj->work[0], 10);
    fn_1_C698(22, obj->work[0], 10);
    fn_1_C698(23, obj->work[0], 10);
    fn_1_C698(24, obj->work[0], 10);
    for (i = 0, player = lbl_1_bss_9BC; i < 4; i++, player++) {
        fn_1_C698((s16)player->charNo, obj->work[0], 10);
    }
    if (obj->work[0]++ > 10) {
        fn_1_7C88();
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void MdPartyMdlSpinOutAll(OMOBJ *obj)
{
    MDPARTY_PLAYER *player;
    s16 i;
    if (obj->work[0] == 0) {
        fn_1_8CCC();
        for (i = 0; i < 4; i++) {
            fn_1_9B68(i);
            fn_1_9CC8(i);
        }
        fn_1_9E28(0);
        fn_1_9E28(1);
        fn_1_9F34();
    }
    fn_1_C698(21, obj->work[0], 10);
    fn_1_C698(22, obj->work[0], 10);
    fn_1_C698(23, obj->work[0], 10);
    fn_1_C698(24, obj->work[0], 10);
    for (i = 0, player = lbl_1_bss_9BC; i < 4; i++, player++) {
        fn_1_C698((s16)player->charNo, obj->work[0], 10);
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

/* Same code as part 8's MdPartyTeamArrange (fn_1_1B9E8) with other targets and scales */
void MdPartyTeamArrangeBack(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;

    MdPartyCharMoveScale(21, obj->work[0], &lbl_1_data_8A4[37], 1.0f);
    MdPartyCharMoveScale(22, obj->work[0], &lbl_1_data_8A4[38], 1.0f);
    MdPartyCharMoveScale(23, obj->work[0], &lbl_1_data_8A4[39], 1.0f);
    MdPartyCharMoveScale(24, obj->work[0], &lbl_1_data_8A4[40], 1.0f);
    if (lbl_1_bss_9F4.tagF == 0) {
        for (i = 0, player = lbl_1_bss_9BC; i < 4; i++, player++) {
            MdPartyCharMoveScale((s16)player->charNo, obj->work[0], &lbl_1_data_8A4[i + 45], 1.25f);
        }
    } else {
        for (i = 0; i < 4; i++) {
            player = &lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][i]];
            MdPartyCharMoveScale((s16)player->charNo, obj->work[0], &lbl_1_data_8A4[i + 57], 1.25f);
        }
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

/* @@PART10@@ */
void MdPartyTeamSelect(OMOBJ *obj)
{
    HuVecF rot;
    s16 i;
    MDPARTY_PLAYER *player;

    if (obj->work[0]++ > 10) {
        if (HuPadDStkRep[0] & PAD_BUTTON_LEFT) {
            lbl_1_bss_9F4.teamNo--;
            if (lbl_1_bss_9F4.teamNo < 0) {
                lbl_1_bss_9F4.teamNo += 3;
            }
            obj->work[0] = 0;
            obj->work[1] = 0;
            MdPartyIconPush(4);
        } else if (HuPadDStkRep[0] & PAD_BUTTON_RIGHT) {
            lbl_1_bss_9F4.teamNo++;
            if (lbl_1_bss_9F4.teamNo > 2) {
                lbl_1_bss_9F4.teamNo -= 3;
            }
            obj->work[0] = 0;
            obj->work[1] = 1;
            MdPartyIconPush(5);
        }
    }
    if (obj->work[0] < 10) {
        for (i = 0; i < 4; i++) {
            player = &lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][i]];
            Hu3DModelRotGet(lbl_1_bss_288[player->charNo].mdlId, &rot);
            if (obj->work[1]) {
                rot.y += 18.0f;
                if (rot.y > 90.0f) {
                    Hu3DModelPosSetV(lbl_1_bss_288[player->charNo].mdlId, &lbl_1_data_8A4[i + 49]);
                }
            } else {
                rot.y -= 18.0f;
                if (rot.y < -90.0f) {
                    Hu3DModelPosSetV(lbl_1_bss_288[player->charNo].mdlId, &lbl_1_data_8A4[i + 49]);
                }
            }
            Hu3DModelRotSetV(lbl_1_bss_288[player->charNo].mdlId, &rot);
        }
    } else {
        for (i = 0; i < 4; i++) {
            player = &lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][i]];
            Hu3DModelRotGet(lbl_1_bss_288[player->charNo].mdlId, &rot);
            rot.y = 0.0f;
            Hu3DModelRotSetV(lbl_1_bss_288[player->charNo].mdlId, &rot);
        }
    }
}

void MdPartyTeamSelectStart(OMOBJ *obj)
{
    HuVecF pos;

    Hu3DModelPosGet(lbl_1_bss_288[lbl_1_bss_9BC[0].charNo].mdlId, &pos);
    pos.x = 0.0f;
    MdPartyIconOpen(4, &pos, -210.0f, 0.0f);
    MdPartyIconOpen(5, &pos, 210.0f, 0.0f);
    obj->work[0] = 10;
    obj->objFunc = MdPartyTeamSelect;
    MdPartyPanelStatSet(1);
}

void MdPartyTeamSelectEnd(OMOBJ *obj)
{
    MdPartyIconClose(4);
    MdPartyIconClose(5);
    obj->objFunc = NULL;
    MdPartyPanelStatSet(1);
}

void MdPartyTeamWinOpen(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;

    lbl_1_bss_9F4.teamNo = 0;
    lbl_1_bss_9F4.mesNo[0] = 0x30037;
    lbl_1_bss_9F4.mesNo[1] = 0x30037;
    if (obj->work[0] == 0) {
        for (i = 0; i < 4; i++) {
            player = &lbl_1_bss_9BC[i];
            MdPartyPanelAnimSet(player->charNo, 0, player->charNo, 1);
            MdPartyPanelAnimSet(player->charNo, 1, player->charNo, 1);
            MdPartyPanelAnimSet(player->charNo, 2, player->charNo, 1);
            MdPartyPanelAnimSet(player->charNo, 3, player->charNo, 1);
        }
        MdPartyInfoWinOpen(lbl_1_bss_9F4.mesNo[0], lbl_1_bss_9F4.mesNo[1]);
        lbl_1_bss_80[0] = 0.5f;
        MdPartySprGrpAttrReset(lbl_1_bss_918[16], HUSPR_ATTR_DISPOFF);
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        MdPartyPanelStatSet(1);
    }
}

void MdPartyTeamWinReset(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;

    lbl_1_bss_9F4.mesNo[0] = 0x30037;
    lbl_1_bss_9F4.mesNo[1] = 0x30037;
    if (obj->work[0] == 0) {
        for (i = 0; i < 4; i++) {
            player = &lbl_1_bss_9BC[i];
            MdPartyPanelAnimSet(player->charNo, 0, player->charNo, 1);
            MdPartyPanelAnimSet(player->charNo, 2, player->charNo, 1);
        }
        MdPartyInfoWinMesSet(0, lbl_1_bss_9F4.mesNo[0]);
        MdPartyInfoWinMesSet(1, lbl_1_bss_9F4.mesNo[1]);
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        MdPartyPanelStatSet(1);
    }
}

void MdPartyTeamNameSet(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;

    if (obj->work[0] == 0) {
        lbl_1_bss_9F4.mesNo[0] = MdPartyPairMesGet(lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][0]].charNo,
            lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][1]].charNo);
        lbl_1_bss_9F4.mesNo[1] = MdPartyPairMesGet(lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][2]].charNo,
            lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][3]].charNo);
        MdPartyInfoWinMesSet(0, lbl_1_bss_9F4.mesNo[0]);
        MdPartyInfoWinMesSet(1, lbl_1_bss_9F4.mesNo[1]);
        for (i = 0; i < 4; i++) {
            player = &lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][i]];
            MdPartyPanelAnimSet(player->charNo, 0, player->charNo, 3);
        }
        MdPartyPanelAnimSet(lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][0]].charNo, 2, 38, -1);
        MdPartyPanelAnimSet(lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][1]].charNo, 2, 38, -1);
        MdPartyPanelAnimSet(lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][2]].charNo, 2, 39, -1);
        MdPartyPanelAnimSet(lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][3]].charNo, 2, 39, -1);
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        MdPartyPanelStatSet(1);
    }
}

void MdPartyTeamWinClose(OMOBJ *obj)
{
    if (obj->work[0] == 0) {
        MdPartyInfoWinClose();
        MdPartyIconDOff();
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        MdPartyPanelStatSet(1);
    }
}


void MdPartyTeamPlayerSelect(OMOBJ *obj)
{
    s16 i;
    s16 j;
    MDPARTY_PLAYER *player;

    for (i = 0; i < 4; i++) {
        player = &lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][i]];
        if (player->com == 1 && player->state != 2) {
            if (player->state == 1 && obj->work[0]++ > 10) {
                if (HuPadDStkRep[0] & PAD_BUTTON_LEFT) {
                    player->comDif--;
                    if (lbl_1_bss_2C == 0) {
                        if (player->comDif < 0) {
                            player->comDif += 3;
                        }
                    } else {
                        if (player->comDif < 0) {
                            player->comDif += 4;
                        }
                    }
                    obj->work[0] = 0;
                    MdPartyIconPush(4);
                    MdPartyIconABankSet(player->padNo, player->comDif + 5);
                } else if (HuPadDStkRep[0] & PAD_BUTTON_RIGHT) {
                    player->comDif++;
                    if (lbl_1_bss_2C == 0) {
                        if (player->comDif > 2) {
                            player->comDif -= 3;
                        }
                    } else {
                        if (player->comDif > 3) {
                            player->comDif -= 4;
                        }
                    }
                    obj->work[0] = 0;
                    MdPartyIconPush(5);
                    MdPartyIconABankSet(player->padNo, player->comDif + 5);
                } else if (HuPadBtnDown[0] & PAD_BUTTON_A) {
                    HuAudFXPlay(1);
                    player->state = 2;
                    MdPartyIconClose(4);
                    MdPartyIconClose(5);
                    for (j = 0; j < 4; j++) {
                        if (lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][j]].com == 1
                            && lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][j]].state == 1) {
                            lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][j]].comDif = 1;
                            break;
                        }
                    }
                    if (j != 4) {
                        obj->objFunc = MdPartyTeamPlayerSelectStart;
                    } else {
                        obj->objFunc = NULL;
                    }
                } else if (HuPadBtnDown[0] & PAD_BUTTON_B) {
                    HuAudFXPlay(3);
                    player->state = 1;
                    MdPartyIconClose(4);
                    MdPartyIconClose(5);
                    MdPartyIconABankSet(player->padNo, 4);
                    for (j = 3; j >= 0; j--) {
                        if (lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][j]].com == 1
                            && lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][j]].state == 2) {
                            lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][j]].state = 1;
                            break;
                        }
                    }
                    if (j != -1) {
                        obj->objFunc = MdPartyTeamPlayerSelectStart;
                    } else {
                        obj->objFunc = NULL;
                    }
                }
            }
            break;
        }
    }
}

void MdPartyTeamPlayerSelectStart(OMOBJ *obj)
{
    HuVecF pos;
    s16 i;
    MDPARTY_PLAYER *player;

    for (i = 0; i < 4; i++) {
        player = &lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][i]];
        if (player->com == 1 && player->state == 1) {
            Hu3DModelPosGet(lbl_1_bss_288[player->charNo].mdlId, &pos);
            MdPartyIconOpen(4, &pos, -40.0f, -45.0f);
            MdPartyIconOpen(5, &pos, 40.0f, -45.0f);
            MdPartyIconABankSet(player->padNo, player->comDif + 5);
            break;
        }
    }
    obj->work[0] = 10;
    obj->objFunc = MdPartyTeamPlayerSelect;
    MdPartyPanelStatSet(1);
}

void MdPartyTeamPlayerSelectEnd(OMOBJ *obj)
{
    MdPartyIconClose(4);
    MdPartyIconClose(5);
    obj->objFunc = NULL;
    MdPartyPanelStatSet(1);
}

void MdPartyTeamPlayerReset(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;

    for (i = 0; i < 4; i++) {
        player = &lbl_1_bss_9BC[i];
        if (player->com == 1) {
            player->comDif = 1;
        }
    }
    obj->objFunc = NULL;
    MdPartyPanelStatSet(1);
}

void MdPartyTeamMove(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;

    for (i = 0; i < 4; i++) {
        player = &lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][i]];
        MdPartyPanelMove((s16)player->charNo, obj->work[0], 10, &lbl_1_data_8A4[i + 49], 1.25f);
    }
    if (obj->work[0]++ >= 10) {
        MdPartyInfoWinOpen(lbl_1_bss_9F4.mesNo[0], lbl_1_bss_9F4.mesNo[1]);
        for (i = 0; i < 4; i++) {
            player = &lbl_1_bss_9BC[i];
            if (player->com == 1) {
                player->state = 1;
                player->comDif = 1;
                MdPartyIconABankSet(i, 4);
            }
        }
        obj->objFunc = NULL;
        MdPartyPanelStatSet(1);
    }
}

/* @@PART11@@ */
void MdPartyStepEnd(OMOBJ *obj)
{
    obj->objFunc = NULL;
    MdPartyPanelStatSet(1);
}

void MdPartyTeamComReset(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;

    for (i = 0; i < 4; i++) {
        player = &lbl_1_bss_9BC[i];
        if (player->com == 1) {
            player->comDif = 1;
            MdPartyIconABankSet(i, 4);
        }
    }
    obj->objFunc = NULL;
    MdPartyPanelStatSet(1);
}

void MdPartyTeamMoveBack(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;

    for (i = 0; i < 4; i++) {
        player = &lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][i]];
        MdPartyPanelMove((s16)player->charNo, obj->work[0], 10, &lbl_1_data_8A4[i + 49], 1.25f);
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        MdPartyPanelStatSet(1);
    }
}

void MdPartyTeamHandicapSelect(OMOBJ *obj)
{
    HuVecF pos;

    if (obj->work[0]++ >= 10) {
        if (HuPadDStkRep[0] & PAD_BUTTON_LEFT) {
            lbl_1_bss_9F4.handicapCursor--;
            if (lbl_1_bss_9F4.handicapCursor < 0) {
                lbl_1_bss_9F4.handicapCursor += 2;
            }
            obj->work[0] = 0;
            MdPartyIconPush(4);
        } else if (HuPadDStkRep[0] & PAD_BUTTON_RIGHT) {
            lbl_1_bss_9F4.handicapCursor++;
            if (lbl_1_bss_9F4.handicapCursor > 1) {
                lbl_1_bss_9F4.handicapCursor -= 2;
            }
            obj->work[0] = 0;
            MdPartyIconPush(5);
        } else if (HuPadDStkRep[0] & PAD_BUTTON_DOWN) {
            lbl_1_bss_9BC[lbl_1_bss_9F4.handicapCursor].handicap--;
            if (lbl_1_bss_9BC[lbl_1_bss_9F4.handicapCursor].handicap < 0) {
                lbl_1_bss_9BC[lbl_1_bss_9F4.handicapCursor].handicap += 10;
            }
            obj->work[0] = 0;
            MdPartyIconPush(3);
            MdPartyIconCBankSet(lbl_1_bss_9F4.handicapCursor, lbl_1_bss_9BC[lbl_1_bss_9F4.handicapCursor].handicap);
        } else if (HuPadDStkRep[0] & PAD_BUTTON_UP) {
            lbl_1_bss_9BC[lbl_1_bss_9F4.handicapCursor].handicap++;
            if (lbl_1_bss_9BC[lbl_1_bss_9F4.handicapCursor].handicap > 9) {
                lbl_1_bss_9BC[lbl_1_bss_9F4.handicapCursor].handicap -= 10;
            }
            obj->work[0] = 0;
            MdPartyIconPush(2);
            MdPartyIconCBankSet(lbl_1_bss_9F4.handicapCursor, lbl_1_bss_9BC[lbl_1_bss_9F4.handicapCursor].handicap);
        }
    }
    Hu3DModelPosGet(lbl_1_bss_288[lbl_1_bss_9BC[lbl_1_bss_9F4.handicapCursor].charNo].mdlId, &pos);
    pos.x = lbl_1_data_8A4[lbl_1_bss_9F4.handicapCursor + 61].x;
    MdPartyMarkPosSet(2, &pos, 0.0f, -80.0f);
    MdPartyMarkPosSet(3, &pos, 0.0f, 80.0f);
}

void MdPartyTeamHandicapSelectStart(OMOBJ *obj)
{
    HuVecF pos;

    Hu3DModelPosGet(lbl_1_bss_288[lbl_1_bss_9BC[lbl_1_bss_9F4.handicapCursor].charNo].mdlId, &pos);
    pos.x = lbl_1_data_8A4[lbl_1_bss_9F4.handicapCursor + 61].x;
    MdPartyIconOpen(2, &pos, 0.0f, -80.0f);
    MdPartyIconOpen(3, &pos, 0.0f, 80.0f);
    pos.x = 0.0f;
    MdPartyIconOpen(4, &pos, -210.0f, 0.0f);
    MdPartyIconOpen(5, &pos, 210.0f, 0.0f);
    obj->work[0] = 10;
    obj->objFunc = MdPartyTeamHandicapSelect;
    MdPartyPanelStatSet(1);
}

void MdPartyTeamHandicapSelectEnd(OMOBJ *obj)
{
    MdPartyIconClose(2);
    MdPartyIconClose(3);
    MdPartyIconClose(4);
    MdPartyIconClose(5);
    obj->objFunc = NULL;
    MdPartyPanelStatSet(1);
}

void MdPartyTeamHandicapOpen(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;

    lbl_1_bss_9F4.handicapCursor = 0;
    for (i = 0; i < 4; i++) {
        player = &lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][i]];
        MdPartyPanelMove((s16)player->charNo, obj->work[0], 10, &lbl_1_data_8A4[i + 49], 1.25f);
    }
    if (obj->work[0]++ > 10) {
        for (i = 0; i < 2; i++) {
            lbl_1_bss_9BC[i].handicap = 0;
            MdPartyIconCOn(i, lbl_1_bss_9BC[i].handicap);
        }
        obj->objFunc = NULL;
        MdPartyPanelStatSet(1);
    }
}

void MdPartyTeamHandicapReset(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;

    lbl_1_bss_9F4.handicapCursor = 0;
    for (i = 0; i < 4; i++) {
        player = &lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][i]];
        MdPartyPanelMove((s16)player->charNo, obj->work[0], 10, &lbl_1_data_8A4[i + 49], 1.25f);
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        MdPartyPanelStatSet(1);
    }
}

void MdPartyTeamMoveRule(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;

    for (i = 0; i < 4; i++) {
        player = &lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][i]];
        MdPartyPanelMove((s16)player->charNo, obj->work[0], 10, &lbl_1_data_8A4[i + 57], 1.25f);
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        MdPartyPanelStatSet(1);
    }
}

void MdPartyTeamHandicapClose(OMOBJ *obj)
{
    s16 i;
    MDPARTY_PLAYER *player;

    if (obj->work[0] == 0) {
        for (i = 0; i < 2; i++) {
            MdPartyIconCOff(i);
        }
    }
    for (i = 0; i < 4; i++) {
        player = &lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][i]];
        MdPartyPanelMove((s16)player->charNo, obj->work[0], 10, &lbl_1_data_8A4[i + 49], 1.25f);
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        MdPartyPanelStatSet(1);
    }
}

void MdPartyRuleSelect(OMOBJ *obj)
{
    HuVecF pos;

    if (obj->work[0]++ >= 10) {
        if (HuPadDStkRep[0] & PAD_BUTTON_LEFT) {
            lbl_1_bss_9F4.unk18--;
            if (lbl_1_bss_9F4.unk18 < 0) {
                lbl_1_bss_9F4.unk18 += 4;
            }
            obj->work[0] = 0;
            obj->work[1] = 2;
            MdPartyIconPush(4);
        } else if (HuPadDStkRep[0] & PAD_BUTTON_RIGHT) {
            lbl_1_bss_9F4.unk18++;
            if (lbl_1_bss_9F4.unk18 > 3) {
                lbl_1_bss_9F4.unk18 -= 4;
            }
            obj->work[0] = 0;
            obj->work[1] = 2;
            MdPartyIconPush(5);
        } else if (HuPadDStkRep[0] & PAD_BUTTON_DOWN) {
            switch (lbl_1_bss_9F4.unk18) {
                case 0:
                    lbl_1_bss_9F4.tagF--;
                    if (lbl_1_bss_9F4.tagF < 0) {
                        lbl_1_bss_9F4.tagF += 2;
                    }
                    break;
                case 1:
                    lbl_1_bss_9F4.turnNo--;
                    if (lbl_1_bss_9F4.turnNo < 0) {
                        lbl_1_bss_9F4.turnNo += 9;
                    }
                    break;
                case 2:
                    lbl_1_bss_9F4.mgPack--;
                    if (lbl_1_bss_9F4.mgPack < 0) {
                        lbl_1_bss_9F4.mgPack += 5;
                    }
                    break;
                case 3:
                    lbl_1_bss_9F4.bonusStarF--;
                    if (lbl_1_bss_9F4.bonusStarF < 0) {
                        lbl_1_bss_9F4.bonusStarF += 2;
                    }
                    break;
            }
            obj->work[0] = 0;
            obj->work[1] = 0;
            MdPartyIconPush(3);
        } else if (HuPadDStkRep[0] & PAD_BUTTON_UP) {
            switch (lbl_1_bss_9F4.unk18) {
                case 0:
                    lbl_1_bss_9F4.tagF++;
                    if (lbl_1_bss_9F4.tagF >= 2) {
                        lbl_1_bss_9F4.tagF -= 2;
                    }
                    break;
                case 1:
                    lbl_1_bss_9F4.turnNo++;
                    if (lbl_1_bss_9F4.turnNo >= 9) {
                        lbl_1_bss_9F4.turnNo -= 9;
                    }
                    break;
                case 2:
                    lbl_1_bss_9F4.mgPack++;
                    if (lbl_1_bss_9F4.mgPack >= 5) {
                        lbl_1_bss_9F4.mgPack -= 5;
                    }
                    break;
                case 3:
                    lbl_1_bss_9F4.bonusStarF++;
                    if (lbl_1_bss_9F4.bonusStarF >= 2) {
                        lbl_1_bss_9F4.bonusStarF -= 2;
                    }
                    break;
            }
            obj->work[0] = 0;
            obj->work[1] = 1;
            MdPartyIconPush(2);
        }
    } else if (obj->work[1] != 2) {
        switch (lbl_1_bss_9F4.unk18) {
            case 0:
                if (obj->work[1] != 0) {
                    MdPartyPanelFlip(21, 20, lbl_1_bss_9F4.tagF, obj->work[0], 10, 1);
                } else {
                    MdPartyPanelFlip(21, 20, lbl_1_bss_9F4.tagF, obj->work[0], 10, -1);
                }
                break;
            case 1:
                if (obj->work[1] != 0) {
                    MdPartyPanelFlip(22, 22, lbl_1_bss_9F4.turnNo, obj->work[0], 10, 1);
                } else {
                    MdPartyPanelFlip(22, 22, lbl_1_bss_9F4.turnNo, obj->work[0], 10, -1);
                }
                break;
            case 2:
                if (obj->work[1] != 0) {
                    MdPartyPanelFlip(23, 31, lbl_1_bss_9F4.mgPack, obj->work[0], 10, 1);
                } else {
                    MdPartyPanelFlip(23, 31, lbl_1_bss_9F4.mgPack, obj->work[0], 10, -1);
                }
                break;
            case 3:
                if (obj->work[1] != 0) {
                    MdPartyPanelFlip(24, 36, lbl_1_bss_9F4.bonusStarF, obj->work[0], 10, 1);
                } else {
                    MdPartyPanelFlip(24, 36, lbl_1_bss_9F4.bonusStarF, obj->work[0], 10, -1);
                }
                break;
        }
    }
    Hu3DModelPosGet(lbl_1_bss_288[lbl_1_bss_9F4.unk18 + 21].mdlId, &pos);
    MdPartyMarkPosSet(2, &pos, 0.0f, -65.0f);
    MdPartyMarkPosSet(3, &pos, 0.0f, 65.0f);
}

/* @@PART12@@ */
void MdPartyRuleSelectStart(OMOBJ *obj)
{
    HuVecF pos;

    Hu3DModelPosGet(lbl_1_bss_288[lbl_1_bss_9F4.unk18 + 21].mdlId, &pos);
    MdPartyIconOpen(2, &pos, 0.0f, -65.0f);
    MdPartyIconOpen(3, &pos, 0.0f, 65.0f);
    pos.x = 0.0f;
    MdPartyIconOpen(4, &pos, -220.0f, 0.0f);
    MdPartyIconOpen(5, &pos, 220.0f, 0.0f);
    obj->work[0] = 10;
    obj->objFunc = fn_1_26328;
    lbl_1_bss_24->mtnId[0] = 1;
}

void MdPartyRuleSelectEnd(OMOBJ *obj)
{
    MdPartyIconClose(2);
    MdPartyIconClose(3);
    MdPartyIconClose(4);
    MdPartyIconClose(5);
    obj->objFunc = NULL;
    lbl_1_bss_24->mtnId[0] = 1;
}

void MdPartyRulePanelSpinIn(OMOBJ *obj)
{
    if (obj->work[0] == 0) {
        lbl_1_bss_9F4.unk18 = 0;
        lbl_1_bss_9F4.tagF = GwCommon.confTag;
        lbl_1_bss_9F4.turnNo = (GwCommon.confTurnNum / 5) - 2;
        lbl_1_bss_9F4.mgPack = GwCommon.partyMgPack;
        lbl_1_bss_9F4.bonusStarF = GwCommon.confBonusStar;
        lbl_1_bss_9F4.bonusStarF++;
        lbl_1_bss_9F4.bonusStarF %= 2;
        MdPartyPanelAnimSetAll(21, lbl_1_bss_9F4.tagF + 20);
        MdPartyPanelAnimSetAll(22, lbl_1_bss_9F4.turnNo + 22);
        MdPartyPanelAnimSetAll(23, lbl_1_bss_9F4.mgPack + 31);
        MdPartyPanelAnimSetAll(24, lbl_1_bss_9F4.bonusStarF + 36);
    }
    MdPartyPanelSpinIn(21, obj->work[0], 10, &lbl_1_data_8A4[63], 1.5f);
    MdPartyPanelSpinIn(22, obj->work[0], 10, &lbl_1_data_8A4[64], 1.5f);
    MdPartyPanelSpinIn(23, obj->work[0], 10, &lbl_1_data_8A4[65], 1.5f);
    MdPartyPanelSpinIn(24, obj->work[0], 10, &lbl_1_data_8A4[66], 1.5f);
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void MdPartyRulePanelMoveIn(OMOBJ *obj)
{
    if (obj->work[0] == 0) {
        lbl_1_bss_9F4.unk18 = 0;
    }
    MdPartyCharMoveScale(21, obj->work[0], &lbl_1_data_8A4[63], 1.5f);
    MdPartyCharMoveScale(22, obj->work[0], &lbl_1_data_8A4[64], 1.5f);
    MdPartyCharMoveScale(23, obj->work[0], &lbl_1_data_8A4[65], 1.5f);
    MdPartyCharMoveScale(24, obj->work[0], &lbl_1_data_8A4[66], 1.5f);
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

/* @@PART13@@ */
/* part 13: declarations of later parts' functions this part references */

void MdPartyRulePanelMoveOut(OMOBJ *obj)
{
    MdPartyCharMoveScale(21, obj->work[0], &lbl_1_data_8A4[37], 1.0f);
    MdPartyCharMoveScale(22, obj->work[0], &lbl_1_data_8A4[38], 1.0f);
    MdPartyCharMoveScale(23, obj->work[0], &lbl_1_data_8A4[39], 1.0f);
    MdPartyCharMoveScale(24, obj->work[0], &lbl_1_data_8A4[40], 1.0f);
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void MdPartyRulePanelSpinOut(OMOBJ *obj)
{
    MdPartyPanelSpinOut(21, obj->work[0], 10);
    MdPartyPanelSpinOut(22, obj->work[0], 10);
    MdPartyPanelSpinOut(23, obj->work[0], 10);
    MdPartyPanelSpinOut(24, obj->work[0], 10);
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void MdPartyAllKill(void)
{
    MdPartyStageKill(lbl_1_bss_4);
    MdPartyPicKill(lbl_1_bss_8);
    MdPartyHostAKill(lbl_1_bss_C);
    MdPartyHostKill(lbl_1_bss_10);
    MdPartyCharMdlKill(lbl_1_bss_14);
    MdPartyInfoWinKill();
    MdPartyMarkerObjKill(lbl_1_bss_1C[0]);
    fn_1_4581C();
    MdPartyPanelKill();
    MdPartyWinKill();
    MdPartyLightKill();
    MdPartyCameraKill();
}

void MdPartyExitWait(OMOBJ *obj)
{
    if (!WipeCheck()) {
        MdPartyAllKill();
        omOvlReturnEx(1, 1);
    }
}

void MdPartyExitCheck(OMOBJ *obj)
{
    if (omSysExitReq) {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
        obj->objFunc = MdPartyExitWait;
    }
}

void MdPartyMainProc(void)
{
    s16 mode = 0;
    mode = fn_1_3EAC8(0);
    if (lbl_1_data_0 != -1) {
        HuAudSStreamFadeOut(lbl_1_data_0, 1000);
        lbl_1_data_0 = -1;
    }
    switch (mode) {
        case 0:
        case 1:
            WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
            while (WipeCheck()) {
                HuPrcVSleep();
            }
            break;
        case 2:
        case 3:
            WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
            while (WipeCheck()) {
                HuPrcVSleep();
            }
            break;
    }
    MdPartyAllKill();
    switch (mode) {
        case 0:
            MdPartyARamFreeReturn();
            omOvlReturnEx(1, 1);
            break;
        case 1:
            MdPartyOvlGoto(1);
            break;
        case 2:
        case 3:
            MdPartyOvlGoto(0);
            break;
    }
    HuPrcEnd();
    while (1) {
        HuPrcVSleep();
    }
}

void MdPartyInit(void)
{
    lbl_1_bss_0 = omInitObjMan(27, 0x2000);
    omGameSysInit(lbl_1_bss_0);
    MdPartyCameraInit(fn_1_2DFD4);
    MdPartyLightInit();
    MdPartyWinInit();
    MdPartyShadowInit();
    MdPartySprInit();
    MdPartyPanelInit();
    fn_1_44DCC(lbl_1_bss_0);
    lbl_1_bss_4 = omAddObjEx(lbl_1_bss_0, 0x1000, 16, 16, -1, fn_1_861C);
    lbl_1_bss_8 = omAddObjEx(lbl_1_bss_0, 0x1000, 16, 16, -1, fn_1_9218);
    lbl_1_bss_C = omAddObjEx(lbl_1_bss_0, 0x1000, 16, 16, -1, fn_1_605C);
    lbl_1_bss_10 = omAddObjEx(lbl_1_bss_0, 0x1000, 16, 16, -1, fn_1_7064);
    lbl_1_bss_14 = omAddObjEx(lbl_1_bss_0, 0x1000, 16, 32, -1, fn_1_7ED4);
    lbl_1_bss_18 = omAddObjEx(lbl_1_bss_0, 0x1000, 16, 16, -1, fn_1_A5D8);
    lbl_1_bss_1C[0] = omAddObjEx(lbl_1_bss_0, 0x1000, 16, 16, -1, fn_1_B4C8);
    HuPrcChildCreate(MdPartyMainProc, 0x3000, 0x3000, 0, lbl_1_bss_0);
}

/* @@PART14@@ */
/* part 14 needs from HEAD: MDPARTY_CAMERA 0x40 float time (camera intro timer) */
/* other parts' functions this part calls (prototypes for part 1's header) */

void MdPartyDataClose(void)
{
    HuDataDirCloseAll();
}

void ObjectSetup(void)
{
    OSReport("\n-----===== MARIO PARTY 6 :: PARTY MODE =====-----\n\n");
    HuDataDirCloseAll();
    MdPartyARamFree();
    _ClearFlag(0x10014);
    _ClearFlag(0x10023);
    MdPartyUnlockInit();
    lbl_1_bss_28 = omovlevtno;
    MdPartyInit();
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

void MdPartyCameraIntroBoard(OMOBJ *obj, MDPARTY_CAMERA *camera)
{
    camera->centerEnd.x = 0.0f;
    camera->centerEnd.y = 65.0f;
    camera->centerEnd.z = -800.0f;
    camera->rotEnd.x = -7.25f;
    camera->rotEnd.y = 0.0f;
    camera->rotEnd.z = 0.0f;
    camera->zoomEnd = 2150.0f;
    MdPartyCameraSmooth(camera, 15.0f);
}

void MdPartyCameraBoardOut(OMOBJ *obj, MDPARTY_CAMERA *camera)
{
    camera->center.y = MdPartyLerp(camera->centerEnd.y, 275.0f, camera->time, 60.0f);
    camera->center.z = MdPartyLerp(camera->centerEnd.z, -2900.0f, camera->time, 60.0f);
    camera->rot.x = MdPartyLerp(camera->rotEnd.x, 0.0f, camera->time, 60.0f);
    camera->time += 1.0f;
}

void MdPartyCameraBoardOutStart(OMOBJ *obj, MDPARTY_CAMERA *camera)
{
    camera->time = 0.0f;
    MdPartyCameraJump(camera);
    MdPartyCameraFuncSet(MdPartyCameraBoardOut);
}

s32 MdPartyStartIntro(void)
{
    HuPrcSleep(5);
    lbl_1_data_0 = HuAudSStreamPlay(4);
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 60);
    while (WipeCheck()) {
        HuPrcVSleep();
    }
    return 1;
}

s32 MdPartyStartMes(void)
{
    MdPartyHostAReactSet();
    MdPartyHostBReactSet();
    MdPartyMainWinMesSet(1, 0xD0000, 1);
    MdPartyMainWinWait();
    MdPartyHostAReactSet();
    MdPartyMainWinMesSet(3, 0xD0001, 1);
    MdPartyMainWinWait();
    MdPartyTitleFadeOutStart();
    return 1;
}

void MdPartyHostWalkIn(void)
{
    HuVecF pos;
    Hu3DModelPosGet(lbl_1_bss_C->mdlId[0], &pos);
    if (pos.x > -200.0f) {
        MdPartyMainWinClose();
        Hu3DMotionShiftSet(lbl_1_bss_C->mdlId[0], lbl_1_bss_C->mtnId[5], 0.0f, 30.0f, 0);
        Hu3DMotionShiftSet(lbl_1_bss_10->mdlId[0], lbl_1_bss_10->mtnId[5], 0.0f, 30.0f, 0);
        HuPrcSleep(60);
        MdPartyHostAWalkSet();
        MdPartyHostBWalkSet();
        MdPartyMainWinClose();
        HuPrcSleep(90);
    }
}

s16 MdPartyQuitAsk(void)
{
    MdPartyHostBReactSet();
    MdPartyMainWinMesSet(2, 0xD0008, 1);
    return MdPartyMainWinChoiceGet(2);
}

s32 MdPartyResume(void)
{
    s16 choice = 0;
    s16 turn = 0;
    if (GwCommon.saveEnableF == 1) {
        SLBoardLoad();
        lbl_1_bss_9F4.tagF = GwSystem.tagF;
        lbl_1_bss_9F4.boardNo = GwSystem.boardNo;
        lbl_1_bss_9F4.turnNo = GwSystem.turnMax / 5 - 2;
        lbl_1_bss_9F4.mgPack = GwSystem.mgPack;
        lbl_1_bss_9F4.bonusStarF = GwSystem.bonusStarF;
        lbl_1_bss_9F4.bonusStarF++;
        lbl_1_bss_9F4.bonusStarF %= 2;
        turn = GwSystem.turnNo;
        MdPartyHostWalkIn();
        MdPartyPanelFuncStart(MdPartyPanelIntro);
        MdPartyTurnNumSet(turn, (lbl_1_bss_9F4.turnNo + 2) * 5);
    loop:
        HuPrcVSleep();
        MdPartyHostBReactSet();
        MdPartyMainWinMesSet(2, 0xD0002, 1);
        choice = MdPartyMainWinChoiceGet(0);
        if (choice == 0) {
            _SetFlag(0x10023);
            MdPartyTurnNumHide();
            MdPartyPanelFuncStart(MdPartyGroupBCloseExec);
            MdPartyMainWinClose();
            MdPartyReadInit();
            HuAudFXPlay(0x4A2);
            MdPartyHostAFlySet();
            MdPartyHostBFlySet();
            HuPrcSleep(10);
            MdPartyConfettiStart();
            if (lbl_1_data_0 != -1) {
                HuAudSStreamFadeOut(lbl_1_data_0, 3000);
                lbl_1_data_0 = -1;
            }
            HuAudFXPlay(0x4A3);
            HuPrcSleep(90);
            MdPartyCameraFuncSet(MdPartyCameraBoardOut);
            HuPrcSleep(60);
            return 0;
        }
        if (choice == -1) {
            if (MdPartyQuitAsk()) {
                goto loop;
            }
            return -1;
        }
    loop2:
        HuPrcVSleep();
        MdPartyMainWinMesSet(2, 0xD0026, 1);
        choice = MdPartyMainWinChoiceGet(0);
        if (choice == -1) {
            if (MdPartyQuitAsk()) {
                goto loop2;
            }
            return -1;
        }
        if (choice != 0) {
            goto loop;
        }
        MdPartyTurnNumHide();
        MdPartyPanelFuncStart(MdPartyGroupBCloseResetExec);
    }
    return 1;
}

s16 MdPartyTestStart(void)
{
    s16 choice = 0;
    while (1) {
        HuPrcVSleep();
        MdPartyHostBReactSet();
        MdPartyMainWinMesSet(2, 0xD0004, 1);
        choice = MdPartyMainWinChoiceGet(0);
        if (choice != -1) {
            if (choice == 0) {
                MdPartyMainWinClose();
                MdPartyGameWorkTest();
                MdPartyReadInit();
            }
            break;
        }
        if (MdPartyQuitAsk()) {
            continue;
        }
        choice = -1;
        break;
    }
    return choice;
}

s32 MdPartyHostOut(void)
{
    MdPartyHostWalkIn();
    MdPartyHostAReactSet();
    MdPartyMainWinMesSet(3, 0xD0005, 1);
    MdPartyMainWinWait();
    return 1;
}
/* @@PART15@@ */
s16 MdPartyPlayerNumSelect() /* callers pass an argument it never reads */
{
    MDPARTY_PLAYER *player;
    s16 i;
    s16 num;
    s16 result = 0;
    MdPartyPanelFuncStart(MdPartyPlayerNumOpenExec);
    while (1) {
        HuPrcVSleep();
        MdPartyPanelFuncStart(MdPartyPlayerNumSelStart);
        MdPartyHelpWinMesSet(0x1000A);
        MdPartyHostBReactSet();
        MdPartyMainWinMesSet(2, lbl_1_bss_9F4.playerNum + 0xD0009, 0);
        while (1) {
            HuPrcVSleep();
            MdPartyHelpWinMesSet(0x1000A);
            MdPartyMainWinMesSet(2, lbl_1_bss_9F4.playerNum + 0xD0009, 0);
            if (MdPartyPanelTimeCheck()) {
                if (HuPadBtnDown[0] & PAD_BUTTON_A) {
                    HuAudFXPlay(2);
                    num = lbl_1_bss_9F4.playerNum;
                    lbl_1_bss_9BC[0].com = FALSE;
                    num--;
                    for (i = 1, player = &lbl_1_bss_9BC[1]; i < 4; i++, player++) {
                        if (num > 0 && HuPadStatGet(i) == 0) {
                            player->com = FALSE;
                            num--;
                        } else {
                            player->com = TRUE;
                        }
                    }
                    result = 0;
                    break;
                } else if (HuPadBtnDown[0] & PAD_BUTTON_B) {
                    HuAudFXPlay(3);
                    result = 1;
                    break;
                } else if (HuPadBtnDown[0] & PAD_BUTTON_X) {
                    HuAudFXPlay(3);
                    result = -1;
                    break;
                }
            }
        }
        MdPartyHelpWinClose();
        MdPartyPanelFuncStart(MdPartyPlayerNumSelEnd);
        if (result == -1) {
            MdPartyHostBReactSet();
            MdPartyMainWinMesSet(2, 0xD0008, 1);
            if (MdPartyMainWinChoiceGet(2) != 0) {
                continue;
            }
            result = -1;
        }
        break;
    }
    if (result != -1) {
        MdPartyPanelFuncStart(MdPartyComModelOut);
    }
    return result;
}

s16 MdPartyCharSelStatGet(void)
{
    s16 i;
    s16 num;
    for (i = 0, num = 0; i < 4; i++) {
        if (lbl_1_bss_9BC[i].state == 1) {
            num++;
        }
    }
    if (lbl_1_bss_9F4.playerNum <= num) {
        return 0;
    } else if (lbl_1_bss_9BC[0].state == 0) {
        return 1;
    } else {
        return -1;
    }
}

s16 MdPartyCharSelect(s16 resetF)
{
    s16 result = 0;
    if (resetF) {
        MdPartyPanelFuncStart(MdPartyCharSelReset);
    }
    while (1) {
        HuPrcVSleep();
        MdPartyHelpWinMesSet(0x1000A);
        MdPartyHostAReactSet();
        MdPartyMainWinMesSet(3, 0xD000E, 0);
        MdPartyPanelFuncStart(MdPartyCharSelInit);
        while (1) {
            HuPrcVSleep();
            MdPartyMainWinMesSet(3, 0xD000E, 0);
            result = MdPartyCharSelStatGet();
            if (MdPartyPanelTimeCheck()) {
                if (result == 0) {
                    result = 0;
                    break;
                } else if (result == 1 && (HuPadBtnDown[0] & PAD_BUTTON_B)) {
                    HuAudFXPlay(3);
                    result = 1;
                    break;
                } else if (HuPadBtnDown[0] & PAD_BUTTON_X) {
                    HuAudFXPlay(3);
                    result = -1;
                    break;
                }
            }
        }
        MdPartyHelpWinClose();
        MdPartyPanelFuncStart(MdPartyCharSelClose);
        if (result == -1) {
            MdPartyHostBReactSet();
            MdPartyMainWinMesSet(2, 0xD0008, 1);
            if (MdPartyMainWinChoiceGet(2) != 0) {
                continue;
            }
            result = -1;
        }
        break;
    }
    return result;
}

s16 MdPartyComSelStatGet(s16 *comNo, s16 *firstComNo, s16 *lastNo)
{
    s16 i;
    s16 num = 0;
    *comNo = 0;
    *firstComNo = 0;
    *lastNo = 3;
    for (i = 0; i < 4; i++) {
        if (lbl_1_bss_9BC[i].com == TRUE) {
            *firstComNo = i;
            break;
        }
    }
    for (i = 0; i < 4; i++) {
        if (lbl_1_bss_9BC[i].com == TRUE && lbl_1_bss_9BC[i].state != 2) {
            break;
        }
    }
    *comNo = i;
    for (i = 0; i < 4; i++) {
        if (lbl_1_bss_9BC[i].com == TRUE && lbl_1_bss_9BC[i].state != 2) {
            num++;
        }
    }
    return num;
}

s16 MdPartyComSelect()
{
    s16 result = 0;
    s16 comNo = 0;
    s16 firstComNo = 0;
    s16 lastNo = 0;
    s16 num = 0;
    num = MdPartyComSelStatGet(&comNo, &firstComNo, &lastNo);
    while (1) {
        HuPrcVSleep();
        MdPartyHelpWinMesSet(0x1000A);
        MdPartyHostAReactSet();
        if (lbl_1_bss_9BC[comNo].state == 0) {
            MdPartyMainWinInsertMesSet(3, 0xD0013 - (num - 1), 0);
            MdPartyMainWinMesSet(3, 0xD000F, 0);
        } else {
            MdPartyMainWinMesSet(3, 0xD0014, 0);
        }
        MdPartyPanelFuncStart(MdPartyComSelInit);
        while (1) {
            HuPrcVSleep();
            if (lbl_1_bss_9BC[comNo].state == 0) {
                MdPartyMainWinInsertMesSet(3, 0xD0013 - (num - 1), 0);
                MdPartyMainWinMesSet(3, 0xD000F, 0);
            } else {
                MdPartyMainWinMesSet(3, 0xD0014, 0);
            }
            if (MdPartyPanelTimeCheck()) {
                num = MdPartyComSelStatGet(&comNo, &firstComNo, &lastNo);
                if (comNo > lastNo) {
                    result = 0;
                    break;
                } else if (firstComNo == comNo && lbl_1_bss_9BC[comNo].state == 0 && (HuPadBtnDown[0] & PAD_BUTTON_B)) {
                    HuAudFXPlay(3);
                    result = 1;
                    break;
                } else if (HuPadBtnDown[0] & PAD_BUTTON_X) {
                    HuAudFXPlay(3);
                    result = -1;
                    break;
                }
            }
        }
        MdPartyHelpWinClose();
        MdPartyPanelFuncStart(MdPartyComSelClose);
        if (result == -1) {
            MdPartyHostBReactSet();
            MdPartyMainWinMesSet(2, 0xD0008, 1);
            if (MdPartyMainWinChoiceGet(2) != 0) {
                continue;
            }
            result = -1;
        }
        break;
    }
    return result;
}

s16 MdPartyPlayerSelect(s16 hideF)
{
    s16 result = 0;
    s16 resetF = 0;
    if (hideF) {
        MdPartyPanelFuncStart(MdPartyPlayerHide);
    }
    MdPartyPanelFuncStart(MdPartyCharSelOpen);
    switch (lbl_1_bss_9F4.playerNum) {
        case 0:
            result = MdPartyComSelect(0);
            break;
        case 4:
            result = MdPartyCharSelect(0);
            break;
        default:
            while (1) {
                HuPrcVSleep();
                result = MdPartyCharSelect(resetF);
                if (result != 0) {
                    break;
                }
                result = MdPartyComSelect(0);
                if (result != 1) {
                    break;
                }
                resetF = 1;
            }
            break;
    }
    if (result == 0) {
        MdPartyPanelFuncStart(MdPartyCharSelOutA);
    } else if (result == 1) {
        MdPartyPanelFuncStart(MdPartyCharAllHide);
    }
    return result;
}

/* @@PART16@@ */
/* part 16: functions of other parts called or referenced here */

s16 MdPartyComSelStatGetB(s16 *comNo, s16 *firstNo, s16 *lastNo)
{
    s16 num = 0;
    s16 i;
    *comNo = 0;
    *firstNo = 0;
    *lastNo = 3;
    for (i = 0; i < 4; i++) {
        if (lbl_1_bss_9BC[i].com == 1) {
            *firstNo = i;
            break;
        }
    }
    for (i = 0; i < 4; i++) {
        if (lbl_1_bss_9BC[i].com == 1 && lbl_1_bss_9BC[i].state != 1) {
            break;
        }
    }
    *comNo = i;
    for (i = 0; i < 4; i++) {
        if (lbl_1_bss_9BC[i].com == 1 && lbl_1_bss_9BC[i].state != 1) {
            num++;
        }
    }
    return num;
}

s16 MdPartyComSelectB(s16 mode)
{
    s16 result = 0;
    s16 comNo = 0;
    s16 firstNo = 0;
    s16 lastNo = 0;
    s16 num = 0;
    num = MdPartyComSelStatGet(&comNo, &firstNo, &lastNo);
    while (1) {
        HuPrcVSleep();
        MdPartyHelpWinMesSet(0x1000A);
        MdPartyHostAReactSet();
        MdPartyMainWinInsertMesSet(3, 0xD0013 - (num - 1), 0);
        MdPartyMainWinMesSet(3, 0xD000F, 0);
        MdPartyPanelFuncStart(MdPartyComCharSelInit);
        while (1) {
            HuPrcVSleep();
            MdPartyMainWinInsertMesSet(3, 0xD0013 - (num - 1), 0);
            MdPartyMainWinMesSet(3, 0xD000F, 0);
            if (MdPartyPanelTimeCheck()) {
                num = MdPartyComSelStatGetB(&comNo, &firstNo, &lastNo);
                if (comNo > lastNo) {
                    result = 0;
                    break;
                }
                if (firstNo == comNo && lbl_1_bss_9BC[comNo].state == 0 && (HuPadBtnDown[0] & PAD_BUTTON_B)) {
                    HuAudFXPlay(3);
                    result = 1;
                    break;
                }
                if (HuPadBtnDown[0] & PAD_BUTTON_X) {
                    HuAudFXPlay(3);
                    result = -1;
                    break;
                }
            }
        }
        MdPartyHelpWinClose();
        MdPartyPanelFuncStart(MdPartyComCharSelClose);
        if (result != -1) {
            break;
        }
        if (!MdPartyQuitAsk()) {
            result = -1;
            break;
        }
    }
    return result;
}

s16 MdPartyPlayerSelectB(s16 mode)
{
    s16 result = 0;
    s16 retry = 0;
    if (mode != 0) {
        MdPartyPanelFuncStart(MdPartyPlayerHide);
    }
    MdPartyPanelFuncStart(MdPartyCharSelOpen);
    switch (lbl_1_bss_9F4.playerNum) {
    case 0:
        result = MdPartyComSelectB(0);
        break;
    case 4:
        result = MdPartyCharSelect(0);
        break;
    default:
        while (1) {
            HuPrcVSleep();
            result = MdPartyCharSelect(retry);
            if (result != 0) {
                break;
            }
            result = MdPartyComSelectB(0);
            if (result != 1) {
                break;
            }
            retry = 1;
        }
        break;
    }
    if (result == 0) {
        MdPartyPanelFuncStart(MdPartyCharSelOutB);
    } else if (result == 1) {
        MdPartyPanelFuncStart(MdPartyCharAllHide);
    }
    return result;
}

s16 MdPartyTeamModeSelect(s16 mode)
{
    s16 result = 0;
    if (mode == 0) {
        MdPartyPanelFuncStart(MdPartyTeamWinOpen);
    } else {
        MdPartyPanelFuncStart(MdPartyTeamWinReset);
    }
    while (1) {
        HuPrcVSleep();
        MdPartyPanelFuncStart(MdPartyTeamSelectStart);
        MdPartyHelpWinMesSet(0x1000A);
        MdPartyHostAReactSet();
        MdPartyMainWinMesSet(3, 0xD0015, 0);
        while (1) {
            HuPrcVSleep();
            MdPartyMainWinMesSet(3, 0xD0015, 0);
            if (MdPartyPanelTimeCheck()) {
                if (HuPadBtnDown[0] & PAD_BUTTON_A) {
                    HuAudFXPlay(2);
                    result = 0;
                    break;
                }
                if (HuPadBtnDown[0] & PAD_BUTTON_B) {
                    HuAudFXPlay(3);
                    result = 1;
                    break;
                }
                if (HuPadBtnDown[0] & PAD_BUTTON_X) {
                    HuAudFXPlay(3);
                    result = -1;
                    break;
                }
            }
        }
        MdPartyHelpWinClose();
        MdPartyPanelFuncStart(MdPartyTeamSelectEnd);
        if (result != -1) {
            break;
        }
        if (!MdPartyQuitAsk()) {
            result = -1;
            break;
        }
    }
    if (result == 0) {
        MdPartyPanelFuncStart(MdPartyTeamNameSet);
    } else if (result == 1) {
        MdPartyPanelFuncStart(MdPartyTeamWinClose);
    }
    return result;
}

void MdPartyTeamComStatGet(s16 *comNo, s16 *firstNo, s16 *lastNo)
{
    MDPARTY_PLAYER *player;
    s16 i;
    *comNo = 0;
    *firstNo = 0;
    *lastNo = 3;
    for (i = 0; i < 4; i++) {
        player = &lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][i]];
        if (player->com == 1) {
            *firstNo = i;
            break;
        }
    }
    for (i = 0; i < 4; i++) {
        player = &lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.teamNo][i]];
        if (player->com == 1 && player->state != 2) {
            break;
        }
    }
    *comNo = i;
}

s16 MdPartyTeamComSelect(s16 mode)
{
    s16 result = 0;
    s16 comNo;
    s16 firstNo;
    s16 lastNo;
    if (lbl_1_bss_9F4.playerNum == 4) {
        if (mode == 0) {
            return 0;
        }
        MdPartyInfoWinOpen(lbl_1_bss_9F4.mesNo[0], lbl_1_bss_9F4.mesNo[1]);
        return 1;
    }
    if (mode == 0) {
        MdPartyPanelFuncStart(MdPartyTeamPlayerReset);
    } else {
        MdPartyPanelFuncStart(MdPartyTeamMove);
    }
    while (1) {
        HuPrcVSleep();
        MdPartyHelpWinMesSet(0x1000A);
        MdPartyHostAReactSet();
        MdPartyMainWinMesSet(3, 0xD0014, 0);
        MdPartyPanelFuncStart(MdPartyTeamPlayerSelectStart);
        while (1) {
            HuPrcVSleep();
            MdPartyMainWinMesSet(3, 0xD0014, 0);
            if (MdPartyPanelTimeCheck()) {
                MdPartyTeamComStatGet(&comNo, &firstNo, &lastNo);
                if (comNo > lastNo) {
                    result = 0;
                    break;
                }
                if (comNo == firstNo && lbl_1_bss_9BC[comNo].state == 1 && (HuPadBtnDown[0] & PAD_BUTTON_B)) {
                    HuAudFXPlay(3);
                    result = 1;
                    break;
                }
                if (HuPadBtnDown[0] & PAD_BUTTON_X) {
                    HuAudFXPlay(3);
                    result = -1;
                    break;
                }
            }
        }
        MdPartyHelpWinClose();
        MdPartyPanelFuncStart(MdPartyTeamPlayerSelectEnd);
        if (result != -1) {
            break;
        }
        if (!MdPartyQuitAsk()) {
            result = -1;
            break;
        }
    }
    if (result == 0) {
        MdPartyPanelFuncStart(MdPartyStepEnd);
    } else if (result == 1) {
        MdPartyPanelFuncStart(MdPartyTeamComReset);
    }
    return result;
}

s16 MdPartyTeamRuleMesShow(void)
{
    MdPartyHostAReactSet();
    MdPartyMainWinInsertMesSet(3, lbl_1_bss_9F4.mesNo[0], 0);
    MdPartyMainWinInsertMesSet(3, lbl_1_bss_9F4.mesNo[1], 1);
    MdPartyMainWinMesSet(3, 0xD0016, 1);
    MdPartyMainWinWait();
    MdPartyInfoWinClose();
    MdPartyPanelFuncStart(MdPartyTeamMoveBack);
}
/* @@PART17@@ */
/* other parts' functions this part calls or inlines (bodies come from the splice) */

/* Board confirm: 0 = board chosen, -1 = back */
s16 MdPartyBoardConfirm(s16 flag)
{
    s16 result = 0;
    if (flag != 0) {
        MdPartyPanelFuncStart(MdPartyBoardSelWait);
    }
    MdPartyPanelFuncStart(MdPartyBoardAppear);
    while (TRUE) {
        HuPrcVSleep();
        MdPartyPanelFuncStart(MdPartyBoardSelStart);
        MdPartyHelpWinMesSet(0x1000A);
        MdPartyHostBReactSet();
        if (lbl_1_data_BEE[lbl_1_bss_9F4.boardNo] == 1) {
            MdPartyMainWinMesSet(2, lbl_1_bss_9F4.boardNo + 0x20009, 0);
        } else {
            MdPartyMainWinMesSet(2, 0x110017, 0);
        }
        while (TRUE) {
            HuPrcVSleep();
            if (lbl_1_data_BEE[lbl_1_bss_9F4.boardNo] == 1) {
                MdPartyMainWinMesSet(2, lbl_1_bss_9F4.boardNo + 0x20009, 0);
            } else {
                MdPartyMainWinMesSet(2, 0x110017, 0);
            }
            if (MdPartyPanelTimeCheck()) {
                if (HuPadBtnDown[0] & PAD_BUTTON_A) {
                    if (lbl_1_data_BEE[lbl_1_bss_9F4.boardNo] == 0) {
                        HuAudFXPlay(4);
                        continue;
                    }
                    HuAudFXPlay(2);
                    result = 0;
                    break;
                } else if (HuPadBtnDown[0] & PAD_BUTTON_B) {
                    HuAudFXPlay(3);
                    result = -1;
                    break;
                } else if (HuPadBtnDown[0] & PAD_BUTTON_X) {
                    HuAudFXPlay(3);
                    result = -1;
                    break;
                }
            }
        }
        MdPartyHelpWinClose();
        MdPartyPanelFuncStart(MdPartyBoardRotReset);
        if (result == -1) {
            MdPartyHostBReactSet();
            MdPartyMainWinMesSet(2, 0xD0008, 1);
            if (MdPartyMainWinChoiceGet(2) != 0) {
                continue;
            }
            result = -1;
        }
        break;
    }
    if (result == 0) {
        MdPartyPanelFuncStart(MdPartyBoardDecide);
    } else if (result == 1) {
        MdPartyPanelFuncStart(MdPartyMdlSpinHide);
    }
    return result;
}

/* Handicap confirm: 0 = decided, 1 = back to the handicap select, -1 = quit */
s16 MdPartyHandicapConfirm(s16 flag)
{
    s16 result = 0;
    if (flag == 0) {
        MdPartyPanelFuncStart(MdPartyHandicapMoveIn);
    } else {
        MdPartyPanelFuncStart(MdPartyPlayerMoveIn);
    }
    while (TRUE) {
        HuPrcVSleep();
        MdPartyPanelFuncStart(MdPartyHandicapSelectStart);
        MdPartyHelpWinMesSet(0x1000A);
        MdPartyHostBReactSet();
        MdPartyMainWinInsertMesSet(2, lbl_1_bss_9BC[lbl_1_bss_9F4.handicapCursor].charNo, 0);
        MdPartyMainWinMesSet(2, 0xD0022, 0);
        while (TRUE) {
            HuPrcVSleep();
            MdPartyMainWinInsertMesSet(2, lbl_1_bss_9BC[lbl_1_bss_9F4.handicapCursor].charNo, 0);
            MdPartyMainWinMesSet(2, 0xD0022, 0);
            if (MdPartyPanelTimeCheck()) {
                if (HuPadBtnDown[0] & PAD_BUTTON_A) {
                    HuAudFXPlay(2);
                    result = 0;
                    break;
                } else if (HuPadBtnDown[0] & PAD_BUTTON_B) {
                    HuAudFXPlay(3);
                    result = 1;
                    break;
                } else if (HuPadBtnDown[0] & PAD_BUTTON_X) {
                    HuAudFXPlay(3);
                    result = -1;
                    break;
                }
            }
        }
        MdPartyHelpWinClose();
        MdPartyPanelFuncStart(MdPartyHandicapSelectEnd);
        if (result == -1) {
            MdPartyHostBReactSet();
            MdPartyMainWinMesSet(2, 0xD0008, 1);
            if (MdPartyMainWinChoiceGet(2) != 0) {
                continue;
            }
            result = -1;
        }
        break;
    }
    if (result == 0) {
        MdPartyPanelFuncStart(MdPartyPlayerMoveBack);
    } else if (result == 1) {
        MdPartyPanelFuncStart(MdPartyHandicapMoveOut);
    }
    return result;
}

/* Team handicap confirm: 0 = decided, 1 = back to the handicap select, -1 = quit */
s16 MdPartyTeamHandicapConfirm(s16 flag)
{
    s16 result = 0;
    if (flag == 0) {
        MdPartyPanelFuncStart(MdPartyTeamHandicapOpen);
    } else {
        MdPartyPanelFuncStart(MdPartyTeamHandicapReset);
    }
    while (TRUE) {
        HuPrcVSleep();
        MdPartyPanelFuncStart(MdPartyTeamHandicapSelectStart);
        MdPartyHelpWinMesSet(0x1000A);
        MdPartyHostBReactSet();
        MdPartyMainWinInsertMesSet(2, lbl_1_bss_9F4.mesNo[lbl_1_bss_9F4.handicapCursor], 0);
        MdPartyMainWinMesSet(2, 0xD0027, 0);
        while (TRUE) {
            HuPrcVSleep();
            MdPartyMainWinInsertMesSet(2, lbl_1_bss_9F4.mesNo[lbl_1_bss_9F4.handicapCursor], 0);
            MdPartyMainWinMesSet(2, 0xD0027, 0);
            if (MdPartyPanelTimeCheck()) {
                if (HuPadBtnDown[0] & PAD_BUTTON_A) {
                    HuAudFXPlay(2);
                    result = 0;
                    break;
                } else if (HuPadBtnDown[0] & PAD_BUTTON_B) {
                    HuAudFXPlay(3);
                    result = 1;
                    break;
                } else if (HuPadBtnDown[0] & PAD_BUTTON_X) {
                    HuAudFXPlay(3);
                    result = -1;
                    break;
                }
            }
        }
        MdPartyHelpWinClose();
        MdPartyPanelFuncStart(MdPartyTeamHandicapSelectEnd);
        if (result == -1) {
            MdPartyHostBReactSet();
            MdPartyMainWinMesSet(2, 0xD0008, 1);
            if (MdPartyMainWinChoiceGet(2) != 0) {
                continue;
            }
            result = -1;
        }
        break;
    }
    if (result == 0) {
        MdPartyPanelFuncStart(MdPartyTeamMoveRule);
    } else if (result == 1) {
        MdPartyPanelFuncStart(MdPartyTeamHandicapClose);
    }
    return result;
}

/* Team confirm: 0 = start, 1 = rearrange, 2 = back to the team select */
s16 MdPartyTeamConfirm(void)
{
    s16 result = 0;
    MdPartyPanelFuncStart(MdPartyTeamArrange);
    HuPrcVSleep();
    MdPartyHostAReactSet();
    MdPartyMainWinMesSet(3, 0xD0023, 1);
    result = MdPartyMainWinChoiceGet(2);
    if (result == 0) {
        MdPartyMainWinMesSet(1, 0xD0025, 1);
        HuPrcSleep(60);
        MdPartyHostItemThrowSet();
        HuPrcSleep(60);
        MdPartyMainWinClose();
        MdPartyPanelFuncStart(MdPartyMdlSpinOut);
        result = 0;
    } else {
        MdPartyHostAReactSet();
        MdPartyMainWinMesSet(3, 0xD0024, 1);
        result = MdPartyMainWinChoiceGet(2);
        if (result == 0) {
            MdPartyPanelFuncStart(MdPartyMdlSpinOutAll);
            result = 1;
        } else {
            MdPartyPanelFuncStart(MdPartyTeamArrangeBack);
            result = 2;
        }
    }
    return result;
}

/* @@PART18@@ */
/* part 18: earlier parts' functions it calls or inlines (declared for the own-file build) */
void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);


void MdPartyBoardGo(void)
{
    MdPartyGameWorkSet();
    MdPartyReadInit();
    HuPrcSleep(10);
    MdPartyPlayerJumpStart();
    MdPartyHostAFlySet();
    MdPartyHostBFlySet();
    HuPrcSleep(10);
    fn_1_463E0();
    if (lbl_1_data_0 != -1) {
        HuAudSStreamFadeOut(lbl_1_data_0, 3000);
        lbl_1_data_0 = -1;
    }
    HuAudFXPlay(0x4A3);
    HuPrcSleep(90);
    MdPartyCameraFuncSet(fn_1_2E338);
    HuPrcSleep(60);
}

/* Rule confirm: 0 = go on, 1 = back, -1 = cancel */
s16 MdPartyRuleConfirm(s16 flag)
{
    s16 result = 0;
    if (flag == 0) {
        MdPartyPanelFuncStart(MdPartyRulePanelSpinIn);
    } else {
        MdPartyPanelFuncStart(MdPartyRulePanelMoveIn);
    }
    do {
        HuPrcVSleep();
        MdPartyPanelFuncStart(MdPartyRuleSelectStart);
        MdPartyHostAReactSet();
        MdPartyHelpWinMesSet(0x1000A);
        switch (lbl_1_bss_9F4.unk18) {
            case 0:
                MdPartyMainWinMesSet(3, lbl_1_bss_9F4.tagF + 0xD0006, 0);
                break;
            case 1:
                MdPartyMainWinMesSet(3, lbl_1_bss_9F4.turnNo + 0xD0017, 0);
                break;
            case 2:
                MdPartyMainWinMesSet(3, lbl_1_bss_9F4.mgPack + 0x40000, 0);
                break;
            case 3:
                MdPartyMainWinMesSet(3, lbl_1_bss_9F4.bonusStarF + 0xD0020, 0);
                break;
        }
        while (TRUE) {
            HuPrcVSleep();
            switch (lbl_1_bss_9F4.unk18) {
                case 0:
                    MdPartyMainWinMesSet(3, lbl_1_bss_9F4.tagF + 0xD0006, 0);
                    break;
                case 1:
                    MdPartyMainWinMesSet(3, lbl_1_bss_9F4.turnNo + 0xD0017, 0);
                    break;
                case 2:
                    MdPartyMainWinMesSet(3, lbl_1_bss_9F4.mgPack + 0x40000, 0);
                    break;
                case 3:
                    MdPartyMainWinMesSet(3, lbl_1_bss_9F4.bonusStarF + 0xD0020, 0);
                    break;
            }
            if (MdPartyPanelTimeCheck()) {
                if (HuPadBtnDown[0] & PAD_BUTTON_A) {
                    HuAudFXPlay(2);
                    result = 0;
                    break;
                } else if (HuPadBtnDown[0] & PAD_BUTTON_B) {
                    HuAudFXPlay(3);
                    result = 1;
                    break;
                } else if (HuPadBtnDown[0] & PAD_BUTTON_X) {
                    HuAudFXPlay(3);
                    result = -1;
                    break;
                }
            }
        }
        MdPartyHelpWinClose();
        MdPartyPanelFuncStart(MdPartyRuleSelectEnd);
        if (result != -1) {
            goto end;
        }
        MdPartyHostBReactSet();
        MdPartyMainWinMesSet(2, 0xD0008, 1);
    } while (MdPartyMainWinChoiceGet(2) != 0);
    result = -1;
end:
    if (result == 0) {
        MdPartyPanelFuncStart(fn_1_2A8C4);
    } else if (result == 1) {
        MdPartyPanelFuncStart(fn_1_2BBD8);
    }
    return result;
}

static s16 MdPartyCharSelectSeq(s16 back)
{
    s16 result = 0;
    s16 flag = 0;
    if (back != 0) {
        MdPartyPanelFuncStart(MdPartyPlayerHide);
    }
    MdPartyPanelFuncStart(MdPartyCharSelOpen);
    switch (lbl_1_bss_9F4.playerNum) {
        case 0:
            result = fn_1_33724(0);
            break;
        case 4:
            result = fn_1_329A4(0);
            break;
        default:
            while (TRUE) {
                HuPrcVSleep();
                result = fn_1_329A4(flag);
                if (result != 0) {
                    break;
                }
                result = fn_1_33724(0);
                if (result != 1) {
                    break;
                }
                flag = 1;
            }
            break;
    }
    if (result == 0) {
        MdPartyPanelFuncStart(MdPartyCharSelOutA);
    } else if (result == 1) {
        MdPartyPanelFuncStart(MdPartyCharAllHide);
    }
    return result;
}

static s16 MdPartyTeamCharSelectSeq(s16 back)
{
    s16 result = 0;
    s16 flag = 0;
    if (back != 0) {
        MdPartyPanelFuncStart(MdPartyPlayerHide);
    }
    MdPartyPanelFuncStart(MdPartyCharSelOpen);
    switch (lbl_1_bss_9F4.playerNum) {
        case 0:
            result = fn_1_350F8(0);
            break;
        case 4:
            result = fn_1_329A4(0);
            break;
        default:
            while (TRUE) {
                HuPrcVSleep();
                result = fn_1_329A4(flag);
                if (result != 0) {
                    break;
                }
                result = fn_1_350F8(0);
                if (result != 1) {
                    break;
                }
                flag = 1;
            }
            break;
    }
    if (result == 0) {
        MdPartyPanelFuncStart(MdPartyCharSelOutB);
    } else if (result == 1) {
        MdPartyPanelFuncStart(MdPartyCharAllHide);
    }
    return result;
}

s16 MdPartyMainSeq() /* called with 0, which it never reads */
{
    s16 mode = 0;
    s16 result = 0;
    s16 back = 0;
    HuPrcSleep(5);
    lbl_1_data_0 = HuAudSStreamPlay(4);
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 60);
    while (WipeCheck()) {
        HuPrcVSleep();
    }
    if (lbl_1_bss_28 == 0) {
        fn_1_2E638();
        result = fn_1_2F22C();
        if (result == 0) {
            return 3;
        }
        if (result == -1) {
            return 0;
        }
    } else {
        mode = 1;
        back = 0;
    }
    do {
        HuPrcVSleep();
        switch (mode) {
            case 0:
                result = fn_1_30B4C();
                if (result == 0) {
                    return 1;
                }
                if (result == 1) {
                    mode = 1;
                    back = 0;
                }
                break;
            case 1:
                fn_1_31508();
                mode = 2;
                back = 0;
                break;
            case 2:
                result = fn_1_38A3C(back);
                if (result == 0) {
                    mode = 3;
                    back = 0;
                } else if (result == 1) {
                    mode = 0;
                    back = 1;
                }
                break;
            case 3:
                result = MdPartyRuleConfirm(back);
                if (result == 0) {
                    mode = 4;
                    back = 0;
                } else if (result == 1) {
                    mode = 2;
                    back = 1;
                }
                break;
            case 4:
                result = fn_1_31AC0(back);
                if (result == 0) {
                    if (lbl_1_bss_9F4.tagF == 0) {
                        mode = 5;
                        back = 0;
                    } else {
                        mode = 100;
                        back = 0;
                    }
                } else if (result == 1) {
                    mode = 3;
                    back = 1;
                }
                break;
            case 5:
                result = MdPartyCharSelectSeq(back);
                if (result == 0) {
                    mode = 6;
                    back = 0;
                } else if (result == 1) {
                    mode = 4;
                    back = 1;
                }
                break;
            case 6:
                result = fn_1_39C60(back);
                if (result == 0) {
                    mode = 7;
                    back = 0;
                } else if (result == 1) {
                    mode = 5;
                    back = 1;
                }
                break;
            case 7:
                result = fn_1_3BEA8();
                if (result == 0) {
                    mode = 8;
                    back = 0;
                } else if (result == 1) {
                    mode = 1;
                    back = 0;
                } else if (result == 2) {
                    if (lbl_1_bss_9F4.tagF == 0) {
                        mode = 6;
                        back = 1;
                    } else {
                        mode = 104;
                        back = 1;
                    }
                }
                break;
            case 8:
                MdPartyBoardGo();
                return 2;
            case 100:
                result = MdPartyTeamCharSelectSeq(back);
                if (result == 0) {
                    mode = 101;
                    back = 0;
                } else if (result == 1) {
                    mode = 4;
                    back = 1;
                }
                break;
            case 101:
                result = fn_1_36518(back);
                if (result == 0) {
                    mode = 102;
                    back = 0;
                } else if (result == 1) {
                    mode = 100;
                    back = 1;
                }
                break;
            case 102:
                result = fn_1_3736C(back);
                if (result == 0) {
                    mode = 103;
                    back = 0;
                } else if (result == 1) {
                    mode = 101;
                    back = 1;
                }
                break;
            case 103:
                fn_1_38314();
                mode = 104;
                back = 0;
                break;
            case 104:
                result = fn_1_3AD84(back);
                if (result == 0) {
                    mode = 7;
                    back = 0;
                } else if (result == 1) {
                    mode = 102;
                    back = 1;
                }
                break;
        }
    } while (result != -1);
    return 0;
}

float MdPartyWaveSinLerp90(float start, float end, float time, float maxTime)
{
    if (time <= 0.0f) {
        return start;
    }
    if (time >= maxTime) {
        return end;
    }
    return start + (end - start) * HuSin((90.0f / maxTime) * time);
}

float MdPartyWaveLerp(float start, float end, float time, float maxTime)
{
    if (time <= 0.0f) {
        return start;
    }
    if (time >= maxTime) {
        return end;
    }
    return start + (time / maxTime) * (end - start);
}

float MdPartyWaveSmooth(float cur, float target, float speed)
{
    if (cur == target) {
        return target;
    }
    return (target + cur * (speed - 1.0f)) / speed;
}

void MdPartyWaveCopyHook(s16 layerNo)
{
    if (lbl_1_bss_A90) {
        GXSetTexCopySrc(0, 0, 640, 480);
        GXSetTexCopyDst(320, 240, GX_TF_RGB565, GX_TRUE);
        GXCopyTex(lbl_1_bss_A90, GX_FALSE);
    }
}

void MdPartyWaveMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *material)
{
    MDPARTY_WAVE *work = drawObj->model->hookData;
    HU3D_CAMERA *cameraP = Hu3DCamera;
    Mtx modelMtx;
    Mtx scaleMtx;
    Mtx texMtx;
    Mtx lightMtx;
    Mtx invCamera;
    GXTexObj texObj;

    GXInitTexObj(&texObj, lbl_1_bss_A90, 320, 240, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0, 0, 0, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    HuSprTexLoad(lbl_1_bss_A88, 0, GX_TEXMAP2, GX_REPEAT, GX_REPEAT, GX_LINEAR);
    HuSprTexLoad(lbl_1_bss_A8C, 0, GX_TEXMAP3, GX_REPEAT, GX_REPEAT, GX_LINEAR);
    GXSetNumTexGens(3);
    GXSetNumTevStages(3);
    MTXLightPerspective(lightMtx, cameraP->fov, 1.2f, 0.5f, -0.5f, 0.5f, 0.5f);
    MTXInverse(Hu3DCameraMtx, invCamera);
    MTXConcat(invCamera, drawObj->matrix, modelMtx);
    MTXConcat(lightMtx, Hu3DCameraMtx, texMtx);
    MTXConcat(texMtx, modelMtx, texMtx);
    GXLoadTexMtxImm(texMtx, GX_TEXMTX1, GX_MTX3x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    MTXTrans(modelMtx, 0, work->scroll += work->scrollSpeed, 0);
    MTXScale(scaleMtx, 0.8f, 0.8f, 1);
    MTXConcat(scaleMtx, modelMtx, texMtx);
    GXLoadTexMtxImm(texMtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTevColor(GX_TEVREG0, work->color);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD2, GX_TEXMAP0, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD1, GX_TEXMAP3, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_TEXC, GX_CC_A0, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumIndStages(1);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP2);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE, GX_ITM_0);
    GXSetIndTexMtx(GX_ITM_0, work->indMtx, -1);
}
/* @@PART19@@ */

/* part 18 */

void MdPartyWaveUpdate(OMOBJ *obj)
{
    MDPARTY_WAVE *work = &lbl_1_bss_AF0[0];
    work->indMtx[0][0] = work->indMtx[1][0] = 0.0f;
    work->indMtx[0][1] = work->indMtx[1][1] = 0.0f;
    work->indMtx[0][2] = work->indMtx[1][2] = (0.09f * work->time) / 90.0f;
    work->scrollSpeed = (-0.02f * work->time) / 90.0f;
    if ((work->time -= 1.0f) < 0.0f) {
        Hu3DModelAttrSet(obj->mdlId[0], HU3D_ATTR_DISPOFF);
        Hu3DLayerHookReset(1);
        obj->objFunc = NULL;
    }
}

void MdPartyWaveStart(s16 no, HuVecF *pos)
{
    OMOBJ *obj = lbl_1_bss_A84;
    if (obj) {
        lbl_1_bss_AF0[no].scroll = 0.0f;
        lbl_1_bss_AF0[no].time = 90.0f;
        Hu3DModelPosSet(obj->mdlId[no], pos->x, pos->y, -840.0f);
        Hu3DModelAttrReset(obj->mdlId[no], HU3D_ATTR_DISPOFF);
        Hu3DLayerHookSet(1, MdPartyWaveCopyHook);
        obj->objFunc = MdPartyWaveUpdate;
    }
}

void MdPartyWaveCreate(OMOBJ *obj)
{
    HU3D_MODEL *modelP = NULL;
    omSetStatBit(obj, OM_STAT_MODELPAUSE);
    lbl_1_bss_A88 = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_mdparty, 128), HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_A8C = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_mdparty, 128), HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_A90 = HuMemDirectMallocNum(HEAP_MODEL, GXGetTexBufferSize(320, 240, GX_TF_RGB565, GX_FALSE, 0), HU_MEMNUM_OVL);
    obj->mdlId[0] = Hu3DModelCreate(HuDataSelHeapReadNum(DATANUM(DATA_mdparty, 127), HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelPosSet(obj->mdlId[0], 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(obj->mdlId[0], 90.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(obj->mdlId[0], 6.0f, 6.0f, 6.0f);
    Hu3DModelLayerSet(obj->mdlId[0], 1);
    Hu3DModelAttrSet(obj->mdlId[0], HU3D_ATTR_DISPOFF);
    Hu3DModelMatHookSet(obj->mdlId[0], MdPartyWaveMatHook);
    memset(&lbl_1_bss_AF0[0], 0, sizeof(MDPARTY_WAVE));
    modelP = &Hu3DData[obj->mdlId[0]];
    modelP->hookData = &lbl_1_bss_AF0[0];
    obj->objFunc = NULL;
}

void MdPartyWaveObjKill(OMOBJ *obj)
{
    HU3D_MODEL *modelP = NULL;
    Hu3DLayerHookReset(1);
    if (obj) {
        modelP = &Hu3DData[obj->mdlId[0]];
        modelP->hookData = &lbl_1_bss_AF0[0];
        Hu3DModelKill(obj->mdlId[0]);
        if (lbl_1_bss_A90) {
            HuMemDirectFree(lbl_1_bss_A90);
        }
        lbl_1_bss_A90 = NULL;
    }
    obj = NULL;
}

void MdPartyWaveInit(void)
{
    lbl_1_bss_A84 = omAddObjEx(lbl_1_bss_A80, 4096, 1, 0, -1, MdPartyWaveCreate);
}

void MdPartyWaveKill(void)
{
    if (lbl_1_bss_A84) {
        MdPartyWaveObjKill(lbl_1_bss_A84);
    }
    lbl_1_bss_A84 = NULL;
}

void MdPartyWaveStartFirst(HuVecF *pos)
{
    MdPartyWaveStart(0, pos);
}

void MdPartyBlurHook(s16 layerNo)
{
    GXTexObj texObj;
    Mtx44 proj;
    Mtx modelview;
    float alpha;
    s16 i;

    C_MTXOrtho(proj, 0.0f, 480.0f, 0.0f, 640.0f, 0.0f, 10.0f);
    GXSetProjection(proj, GX_ORTHOGRAPHIC);
    GXInitTexObj(&texObj, lbl_1_bss_A90, 320, 240, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
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
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    PSMTXIdentity(modelview);
    GXLoadPosMtxImm(modelview, GX_PNMTX0);
    alpha = MdPartyWaveLerp(0.0f, 1.0f, lbl_1_bss_A94, 90.0f);
    lbl_1_bss_A94++;
    if (lbl_1_bss_A94 > 90) {
        lbl_1_bss_A94 = 90;
    }
    GXBegin(GX_QUADS, GX_VTXFMT0, 16);
    for (i = 0; i < 4; i++) {
        GXPosition3f32(i * -1, i * -1, 0.0f);
        GXColor4u8(255.0f - 8.0f * alpha, 255.0f - 8.0f * alpha, 255.0f - 8.0f * alpha, 128.0f * alpha);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(i + 640, i * -1, 0.0f);
        GXColor4u8(255.0f - 8.0f * alpha, 255.0f - 8.0f * alpha, 255.0f - 8.0f * alpha, 128.0f * alpha);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(i + 640, i + 480, 0.0f);
        GXColor4u8(255.0f - 8.0f * alpha, 255.0f - 8.0f * alpha, 255.0f - 8.0f * alpha, 128.0f * alpha);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(i * -1, i + 480, 0.0f);
        GXColor4u8(255.0f - 8.0f * alpha, 255.0f - 8.0f * alpha, 255.0f - 8.0f * alpha, 128.0f * alpha);
        GXTexCoord2f32(0.0f, 1.0f);
    }
    GXEnd();
    GXSetTexCopySrc(0, 0, 640, 480);
    GXSetTexCopyDst(320, 240, GX_TF_RGB565, GX_TRUE);
    GXCopyTex(lbl_1_bss_A90, GX_FALSE);
    Hu3DZClear();
}

void MdPartyBlurOn(void)
{
    Hu3DLayerHookSet(14, MdPartyBlurHook);
}

void MdPartyBlurOff(void)
{
    Hu3DLayerHookReset(14);
}

void MdPartyGlowSet(s16 no, HuVecF *pos, GXColor color, float maxTime, float scale)
{
    HU3D_MODEL *modelP = &Hu3DData[lbl_1_bss_AEC];
    HU3D_PARTICLE *particle = modelP->hookData;
    HU3D_PARTICLE_DATA *data = &particle->data[no];
    data->time = 1;
    data->pos.x = pos->x;
    data->pos.y = pos->y;
    data->pos.z = pos->z;
    data->scale = 0.0f;
    data->color.r = color.r;
    data->color.g = color.g;
    data->color.b = color.b;
    data->color.a = color.a;
    data->vel.x = 0.0f;
    data->vel.y = maxTime;
    data->accel.x = scale;
    data->accel.y = color.a;
    particle->dataCnt++;
    modelP->attr &= ~HU3D_ATTR_DISPOFF;
}

void MdPartyGlowHook(HU3D_MODEL *modelP, HU3D_PARTICLE *particle, Mtx mtx)
{
    HU3D_PARTICLE_DATA *data;
    s16 i;
    if (particle->dataCnt <= 0) {
        modelP->attr |= HU3D_ATTR_DISPOFF;
    } else {
        for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
            if (data->time == 1) {
                data->scale = MdPartyWaveSinLerp90(0.0f, data->accel.x, data->vel.x, data->vel.y);
                data->color.a = MdPartyWaveSinLerp90(data->accel.y, 0.0f, data->vel.x, data->vel.y);
                if ((data->vel.x += 1.0f) > data->vel.y) {
                    data->time = 0;
                    data->scale = 0.0f;
                    particle->dataCnt--;
                }
            }
        }
        DCFlushRangeNoSync(particle->data, particle->maxCnt * sizeof(HU3D_PARTICLE_DATA));
    }
}

void MdPartyGlowCreate(void)
{
    lbl_1_bss_AEC = Hu3DParticleCreate(lbl_1_bss_B40[0], 8);
    Hu3DModelPosSet(lbl_1_bss_AEC, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(lbl_1_bss_AEC, 1.0f, 1.0f, 1.0f);
    Hu3DModelLayerSet(lbl_1_bss_AEC, 7);
    Hu3DModelAttrSet(lbl_1_bss_AEC, HU3D_ATTR_DISPOFF);
    Hu3DParticleScaleSet(lbl_1_bss_AEC, 1.0f);
    Hu3DParticleHookSet(lbl_1_bss_AEC, MdPartyGlowHook);
    Hu3DParticleBlendModeSet(lbl_1_bss_AEC, HU3D_PARTICLE_BLEND_ADDCOL);
}

void MdPartyGlowKill(void)
{
    Hu3DModelKill(lbl_1_bss_AEC);
}

void MdPartyBurstSet(s16 no, float maxTime, HuVecF *pos, GXColor color)
{
    HU3D_MODEL *modelP = &Hu3DData[lbl_1_bss_AE4[no]];
    HU3D_PARTICLE *particle = modelP->hookData;
    HU3D_PARTICLE_DATA *data;
    Hu3DModelPosSetV(lbl_1_bss_AE4[no], pos);
    data = particle->data;
    data->time = 1;
    data->color.r = color.r;
    data->color.g = color.g;
    data->color.b = color.b;
    data->color.a = color.a;
    data->time = 0;
    data->parManId = maxTime;
    particle->dataCnt = 1;
    modelP->attr &= ~HU3D_ATTR_DISPOFF;
}

void MdPartyBurstHook(HU3D_MODEL *modelP, HU3D_PARTICLE *particle, Mtx mtx)
{
    HU3D_PARTICLE_DATA *first;
    s16 i;
    HU3D_PARTICLE_DATA *data;
    if (particle->dataCnt <= 0) {
        modelP->attr |= HU3D_ATTR_DISPOFF;
    } else {
        if (particle->dataCnt == 1) {
            first = particle->data;
            for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
                data->attr |= HU3D_PARTICLE_ATTR_SCALEY;
                data->zRot = 0.017453292f * frandmod(360);
                data->vel.z = frandmod(25) + 50;
                data->vel.x = data->vel.z * -HuSin(57.29578f * data->zRot);
                data->vel.y = data->vel.z * HuCos(57.29578f * data->zRot);
                data->scale = 0.1f * data->vel.z;
                data->scaleY = 3.0f * data->vel.z;
                data->color.r = first->color.r;
                data->color.g = first->color.g;
                data->color.b = first->color.b;
                data->color.a = first->color.a;
                data->pos.x = data->vel.x;
                data->pos.y = data->vel.y;
                data->pos.z = 0.0f;
            }
            particle->dataCnt = 2;
        } else if (particle->dataCnt == 2) {
            first = particle->data;
            for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
                if (i % 2 == 0) {
                    data->zRot += i / 500.0f;
                } else {
                    data->zRot -= i / 500.0f;
                }
                data->vel.x = data->vel.z * -HuSin(57.29578f * data->zRot);
                data->vel.y = data->vel.z * HuCos(57.29578f * data->zRot);
                data->pos.x = data->vel.x;
                data->pos.y = data->vel.y;
                data->pos.z = 0.0f;
                data->color.a = MdPartyWaveSinLerp90(255.0f, 0.0f, first->time, first->parManId);
            }
            if ((first->time += 1.0f) > first->parManId) {
                particle->dataCnt = 0;
            }
        }
        DCFlushRangeNoSync(particle->data, particle->maxCnt * sizeof(HU3D_PARTICLE_DATA));
    }
}

void MdPartyBurstCreate(void)
{
    s16 i;
    for (i = 0; i < 4; i++) {
        lbl_1_bss_AE4[i] = Hu3DParticleCreate(lbl_1_bss_B40[1], 16);
        Hu3DModelPosSet(lbl_1_bss_AE4[i], 200.0f * i, 200.0f, 0.0f);
        Hu3DModelScaleSet(lbl_1_bss_AE4[i], 1.0f, 1.0f, 1.0f);
        Hu3DModelLayerSet(lbl_1_bss_AE4[i], 7);
        Hu3DModelAttrSet(lbl_1_bss_AE4[i], HU3D_ATTR_DISPOFF);
        Hu3DParticleScaleSet(lbl_1_bss_AE4[i], 1.0f);
        Hu3DParticleHookSet(lbl_1_bss_AE4[i], MdPartyBurstHook);
        Hu3DParticleBlendModeSet(lbl_1_bss_AE4[i], HU3D_PARTICLE_BLEND_ADDCOL);
    }
}

void MdPartyBurstKill(void)
{
    s16 i;
    for (i = 0; i < 4; i++) {
        Hu3DModelKill(lbl_1_bss_AE4[i]);
    }
}

void MdPartyWarpStart(void)
{
    HU3D_MODEL *modelP = &Hu3DData[lbl_1_bss_AE2];
    HU3D_PARTICLE *particle = modelP->hookData;
    s16 i;
    HU3D_PARTICLE_DATA *data;
    for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
        data->time = 0;
        data->scale = 0.0f;
    }
    particle->dataCnt = 1;
    modelP->attr &= ~HU3D_ATTR_DISPOFF;
}

void MdPartyWarpHook(HU3D_MODEL *modelP, HU3D_PARTICLE *particle, Mtx mtx)
{
    HU3D_PARTICLE_DATA *data;
    s16 i;
    s16 color;
    s16 num = 0;
    float speed = 125.0f;
    if (particle->dataCnt == 1) {
        particle->emitCnt -= 2;
        if (particle->emitCnt < 0) {
            particle->emitCnt += 360.0f;
        }
        if (particle->count > 100) {
            speed -= particle->count - 100;
            if (speed < 1.0f) {
                speed = 1.0f;
            }
        }
        for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
            if (data->time == 0) {
                if (num < 7) {
                    color = rand8() + 128;
                    color = color & 0xFF;
                    data->color.r = 255;
                    color = rand8() + 128;
                    color = color & 0xFF;
                    data->color.g = 255;
                    color = rand8() % 204;
                    color = color & 0xFF;
                    data->color.b = color;
                    if (i % 4 == 0) {
                        data->color.b = 255;
                    }
                    data->color.a = 255;
                    data->scale = 80.0f;
                    data->vel.x = 45.0f * (i % 8) + particle->emitCnt;
                    data->vel.z = 750.0f - (particle->count * 2);
                    if (data->vel.z < 1.0f) {
                        data->vel.z = 1.0f;
                    }
                    data->accel.x = 0.0f;
                    data->accel.y = frandmod(30) + 90;
                    data->pos.x = data->vel.z * -HuSin(data->vel.x);
                    data->pos.y = data->vel.z * HuCos(data->vel.x);
                    data->pos.z = 800.0f;
                    data->time = 1;
                    num++;
                }
            } else if (data->time == 1) {
                data->pos.x = data->vel.z * -HuSin(data->vel.x);
                data->pos.y = data->vel.z * HuCos(data->vel.x);
                data->pos.z = MdPartyWaveLerp(800.0f, -2000.0f, data->accel.x, data->accel.y);
                data->zRot += 0.1f;
                data->scale = MdPartyWaveLerp(150.0f, 50.0f, data->accel.x, data->accel.y);
                data->vel.x += 2.0f;
                data->vel.z = MdPartyWaveLerp(500.0f, speed, data->accel.x, data->accel.y);
                if (data->accel.x > data->accel.y - 10.0f) {
                    data->color.a = MdPartyWaveLerp(255.0f, 0.0f, data->accel.x - (data->accel.y - 10.0f), 10.0f);
                }
                if ((data->accel.x += 1.0f) > data->accel.y) {
                    data->time = 0;
                    data->scale = 0.0f;
                }
            }
        }
    }
    DCFlushRangeNoSync(particle->data, particle->maxCnt * sizeof(HU3D_PARTICLE_DATA));
}

void MdPartyWarpCreate(void)
{
    lbl_1_bss_AE2 = Hu3DParticleCreate(lbl_1_bss_B40[2], 1000);
    Hu3DModelPosSet(lbl_1_bss_AE2, 0.0f, 275.0f, 0.0f);
    Hu3DModelScaleSet(lbl_1_bss_AE2, 1.0f, 1.0f, 1.0f);
    Hu3DModelLayerSet(lbl_1_bss_AE2, 7);
    Hu3DModelAttrSet(lbl_1_bss_AE2, HU3D_ATTR_DISPOFF);
    Hu3DParticleScaleSet(lbl_1_bss_AE2, 1.0f);
    Hu3DParticleHookSet(lbl_1_bss_AE2, MdPartyWarpHook);
}

void MdPartyWarpKill(void)
{
    Hu3DModelKill(lbl_1_bss_AE2);
}

void MdPartySparkSet(s16 no, float maxTime, HuVecF *pos)
{
    HU3D_MODEL *modelP = &Hu3DData[lbl_1_bss_ADA[no]];
    HU3D_PARTICLE *particle = modelP->hookData;
    HU3D_PARTICLE_DATA *data;
    Hu3DModelPosSetV(lbl_1_bss_ADA[no], pos);
    data = particle->data;
    data->time = 0;
    data->parManId = maxTime;
    particle->dataCnt = 1;
    modelP->attr &= ~HU3D_ATTR_DISPOFF;
}

void MdPartySparkHook(HU3D_MODEL *modelP, HU3D_PARTICLE *particle, Mtx mtx)
{
    HuVecF dir;
    HuVecF vel;
    HU3D_PARTICLE_DATA *first;
    HU3D_PARTICLE_DATA *data;
    s16 i;
    s16 color;
    float speed;
    if (particle->dataCnt == 1) {
        for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
            dir.x = frandmod(100) - 50;
            dir.y = frandmod(100) - 50;
            dir.z = frandmod(100) - 50;
            PSVECNormalize(&dir, &vel);
            speed = 200.0f;
            data->vel.x = vel.x * speed;
            data->vel.y = vel.y * speed;
            data->vel.z = vel.z * speed;
            color = rand8() + 128;
            color = color & 0xFF;
            data->color.r = color;
            color = rand8() + 128;
            color = color & 0xFF;
            data->color.g = color;
            color = rand8() % 128;
            color = color & 0xFF;
            data->color.b = color;
            data->color.a = 204;
            data->pos.x = data->accel.x = frandmod(4) - 2;
            data->pos.y = data->accel.y = frandmod(4) - 2;
            data->pos.z = data->accel.z = frandmod(4) - 2;
            data->scale = 0.0f;
        }
        particle->dataCnt = 2;
    } else if (particle->dataCnt == 2) {
        first = particle->data;
        for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
            data->scale = MdPartyWaveSinLerp90(0.0f, 125.0f, first->time, 5.0f);
        }
        if ((first->time += 1.0f) > 15) {
            first->time = 0;
            particle->dataCnt = 3;
        }
    } else if (particle->dataCnt == 3) {
        first = particle->data;
        for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
            data->pos.x = MdPartyWaveSinLerp90(data->accel.x, data->vel.x, first->time, first->parManId);
            data->pos.y = MdPartyWaveSinLerp90(data->accel.y, data->vel.y, first->time, first->parManId);
            data->pos.z = MdPartyWaveSinLerp90(data->accel.z, data->vel.z, first->time, first->parManId);
            data->scale = MdPartyWaveLerp(50.0f, 0.0f, first->time, first->parManId);
        }
        if ((first->time += 1.0f) > first->parManId) {
            particle->dataCnt = 0;
            modelP->attr |= HU3D_ATTR_DISPOFF;
        }
    }
}

void MdPartySparkCreate(void)
{
    s16 i;
    for (i = 0; i < 4; i++) {
        lbl_1_bss_ADA[i] = Hu3DParticleCreate(lbl_1_bss_B40[2], 64);
        Hu3DModelPosSet(lbl_1_bss_ADA[i], 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(lbl_1_bss_ADA[i], 1.0f, 1.0f, 1.0f);
        Hu3DModelLayerSet(lbl_1_bss_ADA[i], 7);
        Hu3DModelAttrSet(lbl_1_bss_ADA[i], HU3D_ATTR_DISPOFF);
        Hu3DParticleScaleSet(lbl_1_bss_ADA[i], 1.0f);
        Hu3DParticleHookSet(lbl_1_bss_ADA[i], MdPartySparkHook);
        Hu3DParticleBlendModeSet(lbl_1_bss_ADA[i], HU3D_PARTICLE_BLEND_ADDCOL);
    }
}

void MdPartySparkKill(void)
{
    s16 i;
    for (i = 0; i < 4; i++) {
        Hu3DModelKill(lbl_1_bss_ADA[i]);
    }
}

void MdPartyGlitterDispSet(s16 no, s16 dispF)
{
    if (dispF) {
        Hu3DModelAttrReset(lbl_1_bss_AD6[no], HU3D_ATTR_DISPOFF);
    } else {
        Hu3DModelAttrSet(lbl_1_bss_AD6[no], HU3D_ATTR_DISPOFF);
    }
}

void MdPartyGlitterStart(s16 no, HuVecF *pos, GXColor *color)
{
    HU3D_MODEL *modelP = &Hu3DData[lbl_1_bss_AD6[no]];
    HU3D_PARTICLE *particle = modelP->hookData;
    s16 i;
    HU3D_PARTICLE_DATA *data;
    for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
        data->time = 1;
        if (color != NULL) {
            data->color.r = color->r;
            data->color.g = color->g;
            data->color.b = color->b;
        }
    }
    if (pos != NULL) {
        Hu3DModelPosSetV(lbl_1_bss_AD6[no], pos);
    }
    Hu3DModelAttrReset(lbl_1_bss_AD6[no], HU3D_ATTR_DISPOFF);
}

void MdPartyGlitterEnd(s16 no)
{
    HU3D_MODEL *modelP = &Hu3DData[lbl_1_bss_AD6[no]];
    HU3D_PARTICLE *particle = modelP->hookData;
    s16 i;
    HU3D_PARTICLE_DATA *data;
    for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
        data->time = 2;
    }
}

void MdPartyGlitterHook(HU3D_MODEL *modelP, HU3D_PARTICLE *particle, Mtx mtx)
{
    u8 tbl[5][2] = { { 16, 255 }, { 16, 255 }, { 48, 128 }, { 128, 64 }, { 255, 32 } };
    s16 i;
    HU3D_PARTICLE_DATA *data;
    if (particle->count == 0) {
        for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
            data->time = 0;
        }
    }
    for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
        if (data->time == 1) {
            data->pos.x = 0.0f;
            data->pos.y = 0.0f;
            data->pos.z = 0.0f;
            data->vel.x = tbl[i % 5][1];
            data->vel.y = tbl[i % 5][0];
            data->color.a = MdPartyWaveSmooth(data->color.a, data->vel.x, 10.0f);
            data->scale = MdPartyWaveSmooth(data->scale, data->vel.y, 10.0f);
        } else if (data->time == 2) {
            data->pos.x = 0.0f;
            data->pos.y = 0.0f;
            data->pos.z = 0.0f;
            data->vel.x = 0.0f;
            data->vel.y = 0.0f;
            data->color.a = MdPartyWaveSmooth(data->color.a, data->vel.x, 10.0f);
            data->scale = MdPartyWaveSmooth(data->scale, data->vel.y, 10.0f);
        } else {
            data->color.a = 0;
            data->scale = 0.0f;
        }
    }
    DCFlushRangeNoSync(particle->data, particle->maxCnt * sizeof(HU3D_PARTICLE_DATA));
}


/* @@PART20@@ */

/* part 20: particle effects (glitter, flare, select effect, flash and hit effects) */
void MdPartyGlitterCreate(void)
{
    s16 i;
    for (i = 0; i < 2; i++) {
        lbl_1_bss_AD6[i] = Hu3DParticleCreate(lbl_1_bss_B40[0], 10);
        Hu3DModelPosSet(lbl_1_bss_AD6[i], 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(lbl_1_bss_AD6[i], 1.0f, 1.0f, 1.0f);
        Hu3DModelAttrSet(lbl_1_bss_AD6[i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(lbl_1_bss_AD6[i], 2);
        Hu3DParticleHookSet(lbl_1_bss_AD6[i], MdPartyGlitterHook);
        Hu3DParticleBlendModeSet(lbl_1_bss_AD6[i], HU3D_PARTICLE_BLEND_ADDCOL);
    }
}

void MdPartyGlitterKill(void)
{
    s16 i;
    for (i = 0; i < 2; i++) {
        Hu3DModelKill(lbl_1_bss_AD6[i]);
    }
}

void MdPartyFlareDispSet(s16 no, s16 dispF)
{
    if (dispF) {
        Hu3DModelAttrReset(lbl_1_bss_AD2[no], HU3D_ATTR_DISPOFF);
    } else {
        Hu3DModelAttrSet(lbl_1_bss_AD2[no], HU3D_ATTR_DISPOFF);
    }
}

void MdPartyFlareOn(s16 no, HuVecF *pos, GXColor *color)
{
    HU3D_PARTICLE *particle;
    HU3D_MODEL *modelP;
    modelP = &Hu3DData[lbl_1_bss_AD2[no]];
    particle = modelP->hookData;
    particle->dataCnt = 1;
    if (color != NULL) {
        particle->pos.x = color->r;
        particle->pos.y = color->g;
        particle->pos.z = color->b;
    }
    if (pos != NULL) {
        particle->unk_10.x = pos->x;
        particle->unk_10.y = pos->y;
        particle->unk_10.z = pos->z;
    }
    Hu3DModelAttrReset(lbl_1_bss_AD2[no], HU3D_ATTR_DISPOFF);
}

void MdPartyFlareOff(s16 no)
{
    HU3D_MODEL *modelP;
    HU3D_PARTICLE *particle;
    modelP = &Hu3DData[lbl_1_bss_AD2[no]];
    particle = modelP->hookData;
    particle->dataCnt = 0;
}

void MdPartyFlareHook(HU3D_MODEL *modelP, HU3D_PARTICLE *particle, Mtx mtx)
{
    HU3D_PARTICLE_DATA *data;
    s16 i;
    s16 num;
    float ofs;
    float color;
    num = 0;
    if (particle->count == 0) {
        for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
            data->time = 0;
        }
        particle->dataCnt = 1;
        particle->pos.x = 255;
        particle->pos.y = 255;
        particle->pos.z = 255;
    }
    for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
        if (data->time == 0 && particle->dataCnt == 1 && num <= 3) {
            num++;
            data->time = 1;
            data->vel.x = 0;
            data->vel.y = frandmod(30) + 60;
            data->accel.x = frandmod(100) - 50;
            data->accel.y = -frandmod(100) - 50;
            data->accel.z = frandmod(100) - 50;
            PSVECNormalize(&data->accel, &data->accel);
            data->accel.x *= 2;
            data->accel.y *= 5;
            data->accel.z *= 2;
            data->pos.x = particle->unk_10.x + frandmod(100) - 50;
            data->pos.y = particle->unk_10.y + frandmod(100) - 50;
            data->pos.z = particle->unk_10.z + frandmod(100) - 50;
            ofs = frandmod(128);
            color = particle->pos.x + ofs;
            if (color > 255) {
                color = 255;
            }
            data->color.r = color;
            color = particle->pos.y + ofs;
            if (color > 255) {
                color = 255;
            }
            data->color.g = color;
            color = particle->pos.z + ofs;
            if (color > 255) {
                color = 255;
            }
            data->color.b = color;
            data->color.a = 0;
        } else if (data->time == 1) {
            data->pos.x += data->accel.x;
            data->pos.y += data->accel.y;
            data->pos.z += data->accel.z;
            if (rand8() % 5 == 0) {
                data->zRot = 0.017453292f * frandmod(360);
                data->color.a = frandmod(127) + 128;
            }
            ofs = MdPartyLerp(1, 0, data->vel.x, data->vel.y);
            data->scale = ofs * frandmod(90);
            if ((data->vel.x += 1) > data->vel.y) {
                data->time = 0;
                data->scale = 0;
                data->color.a = 0;
            }
        }
    }
    DCFlushRangeNoSync(particle->data, particle->maxCnt * sizeof(HU3D_PARTICLE_DATA));
}

void MdPartyFlareCreate(void)
{
    s16 i;
    for (i = 0; i < 2; i++) {
        lbl_1_bss_AD2[i] = Hu3DParticleCreate(lbl_1_bss_B40[3], 256);
        Hu3DModelPosSet(lbl_1_bss_AD2[i], 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(lbl_1_bss_AD2[i], 1.0f, 1.0f, 1.0f);
        Hu3DModelAttrSet(lbl_1_bss_AD2[i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(lbl_1_bss_AD2[i], 2);
        Hu3DParticleHookSet(lbl_1_bss_AD2[i], MdPartyFlareHook);
        Hu3DParticleBlendModeSet(lbl_1_bss_AD2[i], HU3D_PARTICLE_BLEND_ADDCOL);
    }
}

void MdPartyFlareKill(void)
{
    s16 i;
    for (i = 0; i < 2; i++) {
        Hu3DModelKill(lbl_1_bss_AD2[i]);
    }
}

void MdPartyGlitterSet(s16 no, HuVecF *pos, s16 mode)
{
    GXColor colorTbl[2] = { { 255, 114, 46, 0 }, { 109, 207, 246, 0 } };
    if (mode == 1) {
        MdPartyGlitterStart(no, pos, &colorTbl[no]);
        MdPartyFlareOn(no, pos, &colorTbl[no]);
    } else if (mode == 0) {
        MdPartyGlitterEnd(no);
        MdPartyFlareOff(no);
    } else if (mode == 2) {
        MdPartyGlitterDispSet(no, FALSE);
        MdPartyFlareDispSet(no, FALSE);
    }
}

void MdPartyEffDispSet(s16 grpNo, s16 dispF)
{
    s16 i;
    for (i = 0; i < 5; i++) {
        if (dispF) {
            Hu3DModelAttrReset(lbl_1_bss_A96[grpNo].unk0[i], HU3D_ATTR_DISPOFF);
        } else {
            Hu3DModelAttrSet(lbl_1_bss_A96[grpNo].unk0[i], HU3D_ATTR_DISPOFF);
        }
    }
}

void MdPartyEffOn(s16 grpNo, HuVecF *pos, GXColor *color)
{
    HU3D_PARTICLE *particle;
    HU3D_MODEL *modelP;
    s16 i;
    for (i = 0; i < 5; i++) {
        modelP = &Hu3DData[lbl_1_bss_A96[grpNo].unk0[i]];
        particle = modelP->hookData;
        particle->dataCnt = 1;
        if (color != NULL) {
            particle->pos.x = color->r;
            particle->pos.y = color->g;
            particle->pos.z = color->b;
        }
        if (pos != NULL) {
            particle->unk_10.x = pos->x;
            particle->unk_10.y = pos->y;
            particle->unk_10.z = pos->z;
        }
        Hu3DModelAttrReset(lbl_1_bss_A96[grpNo].unk0[i], HU3D_ATTR_DISPOFF);
    }
}

void MdPartyEffOff(s16 grpNo)
{
    HU3D_MODEL *modelP;
    HU3D_PARTICLE *particle;
    s16 i;
    for (i = 0; i < 5; i++) {
        modelP = &Hu3DData[lbl_1_bss_A96[grpNo].unk0[i]];
        particle = modelP->hookData;
        particle->dataCnt = 0;
    }
}

void MdPartyEffHook(HU3D_MODEL *modelP, HU3D_PARTICLE *particle, Mtx mtx)
{
    HU3D_PARTICLE_DATA *data;
    s16 i;
    s16 num;
    float ofs;
    float color;
    num = 0;
    if (particle->count == 0) {
        for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
            data->time = 0;
        }
        particle->dataCnt = 1;
        particle->pos.x = 255;
        particle->pos.y = 255;
        particle->pos.z = 255;
    }
    for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
        if (data->time == 0 && particle->dataCnt == 1 && num < 1) {
            num++;
            data->time = 1;
            data->vel.x = 0;
            data->vel.y = frandmod(30) + 30;
            data->accel.x = frandmod(100) - 50;
            data->accel.y = -frandmod(100) - 50;
            data->accel.z = frandmod(100) - 50;
            PSVECNormalize(&data->accel, &data->accel);
            data->accel.x *= 2;
            data->accel.y *= 2;
            data->accel.z *= 2;
            data->pos.x = particle->unk_10.x + frandmod(100) - 50;
            data->pos.y = particle->unk_10.y;
            data->pos.z = particle->unk_10.z + frandmod(100) - 50;
            ofs = frandmod(32);
            color = particle->pos.x + ofs;
            if (color > 255) {
                color = 255;
            }
            data->color.r = color;
            color = particle->pos.y + ofs;
            if (color > 255) {
                color = 255;
            }
            data->color.g = color;
            color = particle->pos.z + ofs;
            if (color > 255) {
                color = 255;
            }
            data->color.b = color;
            data->color.a = 0;
        } else if (data->time == 1) {
            data->pos.y += data->accel.y;
            if (rand8() % 5 == 0) {
                data->zRot = 0.017453292f * frandmod(360);
                data->color.a = frandmod(127) + 128;
            }
            ofs = MdPartyLerp(1, 0, data->vel.x, data->vel.y);
            data->scale = 40 * ofs;
            if ((data->vel.x += 1) > data->vel.y) {
                data->time = 0;
                data->scale = 0;
                data->color.a = 0;
            }
        }
    }
    DCFlushRangeNoSync(particle->data, particle->maxCnt * sizeof(HU3D_PARTICLE_DATA));
}

void MdPartyEffCreate(void)
{
    s16 maxCnt[5] = { 10, 10, 10, 10, 256 };
    s16 i;
    s16 j;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 5; j++) {
            lbl_1_bss_A96[i].unk0[j] = Hu3DParticleCreate(lbl_1_bss_B40[j + 4], maxCnt[j]);
            Hu3DModelPosSet(lbl_1_bss_A96[i].unk0[j], 0, 0, 0);
            Hu3DModelScaleSet(lbl_1_bss_A96[i].unk0[j], 1, 1, 1);
            Hu3DModelAttrSet(lbl_1_bss_A96[i].unk0[j], HU3D_ATTR_DISPOFF);
            Hu3DModelLayerSet(lbl_1_bss_A96[i].unk0[j], 2);
            Hu3DParticleHookSet(lbl_1_bss_A96[i].unk0[j], MdPartyEffHook);
            Hu3DParticleBlendModeSet(lbl_1_bss_A96[i].unk0[j], HU3D_PARTICLE_BLEND_ADDCOL);
        }
    }
}

void MdPartyEffKill(void)
{
    s16 i;
    s16 j;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 5; j++) {
            Hu3DModelKill(lbl_1_bss_A96[i].unk0[j]);
        }
    }
}

s32 lbl_1_data_F10[9] = {
    DATANUM(DATA_mdparty, 130), DATANUM(DATA_mdparty, 131), DATANUM(DATA_mdparty, 129),
    DATANUM(DATA_mdparty, 132), DATANUM(DATA_mdparty, 137), DATANUM(DATA_mdparty, 137),
    DATANUM(DATA_mdparty, 137), DATANUM(DATA_mdparty, 137), DATANUM(DATA_mdparty, 137),
};

void MdPartyParticleInit(OMOBJMAN *objman)
{
    s16 i;
    lbl_1_bss_A80 = objman;
    MdPartyWaveInit();
    for (i = 0; i < 9; i++) {
        lbl_1_bss_B40[i] = HuSprAnimDataRead(lbl_1_data_F10[i]);
    }
    MdPartyGlowCreate();
    MdPartyBurstCreate();
    MdPartyWarpCreate();
    MdPartySparkCreate();
    MdPartyGlitterCreate();
    MdPartyFlareCreate();
    MdPartyEffCreate();
}

void MdPartyParticleKill(void)
{
    MdPartyGlowKill();
    MdPartyBurstKill();
    MdPartyWarpKill();
    MdPartySparkKill();
    MdPartyGlitterKill();
    MdPartyFlareKill();
    MdPartyEffKill();
    MdPartyBlurOff();
    MdPartyWaveKill();
}

void MdPartyFlashSet(HuVecF *pos)
{
    MdPartyWaveStartFirst(pos);
    MdPartyGlowSet(5, pos, (GXColor){ 128, 128, 255, 128 }, 120.0f, 3000.0f);
}

void MdPartyCursorEffSet(s16 no, HuVecF *pos, s16 dif)
{
    GXColor colorTbl[5] = {
        { 254, 77, 75, 255 },
        { 50, 127, 200, 255 },
        { 199, 175, 0, 255 },
        { 52, 192, 63, 255 },
        { 159, 93, 200, 255 },
    };
    HuVecF effPos;
    effPos.x = pos->x;
    effPos.y = pos->y;
    effPos.z = 10.0f + pos->z;
    MdPartyGlowSet(no, &effPos, colorTbl[dif], 10.0f, 500.0f);
    MdPartyBurstSet(no, 10.0f, &effPos, colorTbl[dif]);
}

void MdPartyHitEffSet(s16 playerNo, HuVecF *pos, s16 dif, s16 mode)
{
    GXColor colorTbl[5] = {
        { 254, 77, 75, 255 },
        { 50, 127, 200, 255 },
        { 199, 175, 0, 255 },
        { 52, 192, 63, 255 },
        { 159, 93, 200, 255 },
    };
    HuVecF effPos;
    effPos.x = pos->x;
    effPos.y = pos->y;
    effPos.z = pos->z;
    Hu3DZClearLayerSet(7);
    if (mode == 0) {
        MdPartyGlowSet(playerNo, &effPos, colorTbl[dif], 20.0f, 300.0f);
        MdPartySparkSet(playerNo, 30.0f, &effPos);
    } else {
        MdPartyGlowSet(playerNo, &effPos, colorTbl[dif], 60.0f, 300.0f);
        MdPartyBurstSet(playerNo, 30.0f, &effPos, colorTbl[dif]);
    }
}

void MdPartyConfettiStart(void)
{
    MdPartyBlurOn();
    MdPartyWarpStart();
}

void MdPartyEffSet(s16 playerNo, HuVecF *pos, s16 mode, s16 dif)
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
    if (mode == 1) {
        MdPartyEffOn(playerNo, pos, &colorTbl[dif]);
    } else if (mode == 0) {
        MdPartyEffOff(playerNo);
    } else if (mode == 2) {
        MdPartyEffDispSet(playerNo, 0);
    }
}
/* @@END@@ */
