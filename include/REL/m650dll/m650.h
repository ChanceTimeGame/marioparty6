#ifndef _REL_M650DLL_M650_H
#define _REL_M650DLL_M650_H

#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mgtimer.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/printfunc.h"
#include "game/process.h"
#include "game/seqman.h"
#include "game/wipe.h"
#include "humath.h"
#include "math.h"
#include "string.h"
#include "dolphin.h"

#include "datadir_enum.h"

typedef struct M650Work_s {
    MGTIMER *timer; /* 0x00 */
    s32 recordTime; /* 0x04 */
    s16 winner; /* 0x08 */
    s16 unkA; /* 0x0A */
    s16 mode; /* 0x0C */
    s16 timeCnt; /* 0x0E */
    s16 unk10; /* 0x10 */
    s16 unk12; /* 0x12 */
    s16 unk14; /* 0x14 */
    s16 unk16; /* 0x16 */
    s32 streamNo; /* 0x18 */
} M650_WORK; /* size 0x1C */

typedef struct M650ObjMan_s {
    OMOBJMAN *objman; /* 0x00 */
    s32 unk4; /* 0x04 */
    s32 unk8; /* 0x08 */
} M650_OBJMAN; /* size 0xC */

typedef struct M650Player_s {
    s16 unk0; /* 0x00 */
    HU3D_MODELID charMdlId; /* 0x02 */
    HU3D_MOTIONID charMotId[5]; /* 0x04 */
    s16 unkE; /* 0x0E */
    s16 charNo; /* 0x10 */
    s16 padNo; /* 0x12 */
    s16 comDif; /* 0x14, -1 for a human player */
    s16 unk16; /* 0x16 */
    s16 unk18; /* 0x18 */
    s16 unk1A; /* 0x1A */
    s16 mode; /* 0x1C */
    HU3D_MODELID unk1E; /* 0x1E */
    HU3D_MODELID mdlId; /* 0x20 */
    HU3D_MOTIONID motId[4]; /* 0x22 */
    s16 unk2A; /* 0x2A */
    HuVecF unk2C; /* 0x2C */
    HuVecF unk38; /* 0x38 */
    float unk44; /* 0x44 */
    float unk48; /* 0x48 */
    float unk4C; /* 0x4C */
    u8 unk50; /* 0x50 */
    HU3D_MODELID unk52; /* 0x52 */
    HU3D_MODELID unk54; /* 0x54 */
    s16 unk56; /* 0x56 */
    HuVecF unk58; /* 0x58 */
    HuVecF unk64; /* 0x64 */
    MGTIMER *timer; /* 0x70 */
    s16 unk74; /* 0x74 */
    s16 score; /* 0x76 */
    HU3D_MODELID unk78; /* 0x78 */
    HU3D_MODELID unk7A; /* 0x7A */
    HU3D_MODELID unk7C; /* 0x7C */
    s16 obstNo[12]; /* 0x7E */
    s16 obstNum; /* 0x96 */
    s16 unk98; /* 0x98 */
    s16 unk9A; /* 0x9A */
    s16 unk9C; /* 0x9C */
    s16 unk9E; /* 0x9E */
} M650_PLAYER; /* size 0xA0 */

typedef struct M650MotData_s {
    u32 dataNum;
    u32 attr;
} M650_MOTDATA;

typedef struct M650Obst_s {
    HuVecF pos; /* 0x00 */
    s16 unkC; /* 0x0C */
    s16 unkE; /* 0x0E */
} M650_OBST; /* size 0x10 */

typedef struct M650ObstFlag_s {
    s16 unk0;
    s16 unk2;
} M650_OBSTFLAG;

extern M650_WORK lbl_1_bss_0;
extern M650_OBJMAN lbl_1_bss_1C;
extern M650_PLAYER lbl_1_bss_28[4];
extern s32 lbl_1_bss_2A8;
extern M650_OBSTFLAG lbl_1_bss_2AC[32][4];
extern HU3D_MODELID lbl_1_bss_4AC[4];
extern HU3D_MODELID lbl_1_bss_4B4;
extern HU3D_MODELID lbl_1_bss_4B6;

extern MGSEQ_PARAM lbl_1_data_0;
extern HuVecF lbl_1_data_28;
extern HuVecF lbl_1_data_34;
extern GXColor lbl_1_data_40[2];
extern u32 lbl_1_data_48[4];
extern M650_MOTDATA lbl_1_data_58[5];
extern u32 lbl_1_data_80[2];
extern u32 lbl_1_data_88[2];
extern u32 lbl_1_data_90[2];
extern u32 lbl_1_data_98[2];
extern u32 lbl_1_data_A0[2];
extern u16 lbl_1_data_150[4];
extern M650_OBST lbl_1_data_1F8[5];
extern M650_OBST lbl_1_data_248[32];
extern u32 lbl_1_data_448[2];
extern u32 lbl_1_data_450[2];

void ObjectSetup(void);
void fn_1_F0(s16 mode, s16 frameNo);
void fn_1_168(s16 mode, s16 frameNo);
void fn_1_188(s16 mode, s16 frameNo);
void fn_1_1C8(s16 mode, s16 frameNo);
void fn_1_200(s16 mode, s16 frameNo);
void fn_1_304(s16 mode, s16 frameNo);
void fn_1_3AC(s16 mode, s16 frameNo);
void fn_1_3B0(s16 mode, s16 frameNo);
void fn_1_3B4(s16 mode, s16 frameNo);
void fn_1_3B8(void);
void fn_1_484(void);
void fn_1_550(void);
void fn_1_8EC(void);
void fn_1_F90(OMOBJ *obj);
void fn_1_1228(void);
void fn_1_14FC(void);
void fn_1_1830(void);
void fn_1_1AE4(void);
void fn_1_2010(s16 playerNo);
void fn_1_21CC(OMOBJ *obj);
void fn_1_22A8(s16 playerNo, s16 motNo);
void fn_1_2320(s16 playerNo, s16 motNo);
void fn_1_23C4(s16 playerNo, s16 motNo, float end);
void fn_1_2464(s16 playerNo);
void fn_1_2558(void);
void fn_1_25B0(void);
void fn_1_26D0(void);
void fn_1_273C(s16 playerNo);
void fn_1_289C(s16 playerNo);
s16 fn_1_2CAC(s16 playerNo);
void fn_1_2F1C(s16 playerNo);
void fn_1_33F8(s16 playerNo);
void fn_1_3F28(s16 playerNo);
void fn_1_4430(s16 playerNo);
void fn_1_473C(s16 playerNo);
void fn_1_48C0(s16 playerNo);
void fn_1_49C8(s16 playerNo);
void fn_1_4BD4(s16 playerNo);
s16 fn_1_4D58(s16 time);
void fn_1_5258(void);
void fn_1_5308(void);
void fn_1_5354(void);
void fn_1_5394(void);
void fn_1_54D4(void);
void fn_1_5544(s16 playerNo);
void fn_1_5600(s16 playerNo, float y, float *speed, float *angle);
s16 fn_1_5728(void);
void fn_1_5A24(s16 playerNo);
void fn_1_5E40(void);
void fn_1_62E0(void);
void fn_1_6478(HU3D_MODEL *modelP, Mtx *mtx);
s16 fn_1_69B0(s16 playerNo);
s16 fn_1_6D88(s16 playerNo, HuVecF *pos);
s16 fn_1_6EE4(HuVecF *a, HuVecF *b, float radius);
void fn_1_6F54(void);
s16 fn_1_7000(s16 playerNo, HuVecF *pos, float angle, s16 obstNo);

#endif
