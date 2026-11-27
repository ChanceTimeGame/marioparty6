/* @@HEAD@@ */
#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/seqman.h"
#include "game/wipe.h"
#include "humath.h"
#include "math.h"
#include "dolphin.h"

#include "datadir_enum.h"

/* rand8 is called unprototyped: game/frand.h stays out of this unit */

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

typedef struct M612Work_s {
    OMOBJMAN *objman; /* 0x00 */
    s32 mode; /* 0x04 */
    s16 unk8; /* 0x08 */
    HU3D_MODELID panelMdlId[4][7][7]; /* 0x0A */
    HU3D_MODELID unk194; /* 0x194 */
    HU3D_MODELID unk196; /* 0x196 */
    OMOBJ *unk198; /* 0x198 */
    OMOBJ *unk19C; /* 0x19C */
} M612_WORK; /* size 0x1A0 */

/* .bss is laid out in reverse definition order */
M612_WORK lbl_1_bss_4AC;
s32 lbl_1_bss_49C[4];
s32 lbl_1_bss_48C[4];
HU3D_MOTIONID lbl_1_bss_44C[4][8];
s32 lbl_1_bss_43C[4];
s32 lbl_1_bss_42C[4];
s32 lbl_1_bss_41C[4];
s32 lbl_1_bss_40C[4];
s32 lbl_1_bss_3FC[4];
s32 lbl_1_bss_3EC[4];
float lbl_1_bss_3DC[4];
s32 lbl_1_bss_3CC[4];
s32 lbl_1_bss_3BC[4];
s32 lbl_1_bss_3AC[4];
s32 lbl_1_bss_39C[4];
s32 lbl_1_bss_38C[4];
s32 lbl_1_bss_37C[4];
s32 lbl_1_bss_36C[4];
s32 lbl_1_bss_30C[24];
s32 lbl_1_bss_308;
s32 lbl_1_bss_304;
s32 lbl_1_bss_300;
s32 lbl_1_bss_2FC;
s32 lbl_1_bss_2F8;
s32 lbl_1_bss_2F4;
s32 lbl_1_bss_2F0;
float lbl_1_bss_2EC;
float lbl_1_bss_2E8;
s32 lbl_1_bss_2C0[10];
s32 lbl_1_bss_48[158];
s32 lbl_1_bss_44;
s32 lbl_1_bss_40;
s32 lbl_1_bss_3C;
s32 lbl_1_bss_38;
s32 lbl_1_bss_34;
s32 lbl_1_bss_30;
s32 lbl_1_bss_2C;
s32 lbl_1_bss_28;
s32 lbl_1_bss_24;
s32 lbl_1_bss_20;
s32 lbl_1_bss_1C;
s32 lbl_1_bss_18;
s32 lbl_1_bss_14;
s32 lbl_1_bss_10;
s32 lbl_1_bss_C;
s32 lbl_1_bss_8;
s32 lbl_1_bss_4;
s32 lbl_1_bss_0;

void ObjectSetup(void);
void fn_1_8F0(void);
void fn_1_F0C(void);
s32 fn_1_13A4(s32 arg0, s32 arg1);
void fn_1_1430(void);
void fn_1_1694(s16 mode, s16 frameNo);
void fn_1_16D4(s16 mode, s16 frameNo);
void fn_1_1DBC(s16 mode, s16 frameNo);
void fn_1_1E18(s16 mode, s16 frameNo);
void fn_1_1FB4(s16 mode, s16 frameNo);
void fn_1_242C(s16 mode, s16 frameNo);
void fn_1_2BC8(s16 mode, s16 frameNo);
void fn_1_2C8C(s16 mode, s16 frameNo);
void fn_1_2C90(s16 mode, s16 frameNo);
void fn_1_2C94(OMOBJ *obj);
void fn_1_2CEC(s32 playerNo);
s32 fn_1_4038(s32 playerNo);
s32 fn_1_4150(s32 playerNo);
s32 fn_1_42C4(s32 playerNo);
s32 fn_1_4480(s32 playerNo);
void fn_1_48D4(void);
void fn_1_4B94(s32 playerNo);
void fn_1_51D8(void);
void fn_1_54D4(OMOBJ *obj);
void fn_1_5924(OMOBJ *obj);
float fn_1_5E00(s32 dir);

/* Defined by part 2, after fn_1_5924: .data keeps definition order */
extern s8 lbl_1_data_198[][60];
extern s8 lbl_1_data_16B0[][7];

s32 lbl_1_data_0[4] = { 45, 30, 30, 15 };
s32 lbl_1_data_10[4] = { 45, 30, 15, 15 };
s32 lbl_1_data_20[4] = { 1, 1, 2, 0 };
s32 lbl_1_data_30[4] = { 1, 2, 2, 0 };
s32 lbl_1_data_40[4] = { 0, 0, 0, 0 };
unsigned int lbl_1_data_50[9] = {
    DATANUM(DATA_mariomot, 0x00),
    DATANUM(DATA_mariomot, 0x01),
    DATANUM(DATA_mariomot, 0x06),
    DATANUM(DATA_mariomot, 0x07),
    DATANUM(DATA_mariomot, 0x0F),
    DATANUM(DATA_mariomot, 0x22),
    DATANUM(DATA_mariomot, 0x3F),
    DATANUM(DATA_mariomot, 0x17),
    0,
};
s32 lbl_1_data_74[4] = { 1, 2, 4, 8 };
GXColor lbl_1_data_84[2] = { { 0xFF, 0xFF, 0xFF, 0x00 }, { 0xD0, 0xD0, 0xD0, 0x00 } };
s32 lbl_1_data_8C = -1;
MGSEQ_PARAM lbl_1_data_90 = {
    300,
    0,
    fn_1_1694,
    fn_1_16D4,
    fn_1_1DBC,
    fn_1_1E18,
    fn_1_1FB4,
    fn_1_242C,
    fn_1_2BC8,
    fn_1_2C8C,
    fn_1_2C90,
};

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

/* @@PART2@@ */
/* @@END@@ */
