#ifndef _REL_M616DLL_M616_H
#define _REL_M616DLL_M616_H

#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mgtimer.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/seqman.h"
#include "string.h"
#include "dolphin.h"

#include "datadir_enum.h"


void Hu3DMotionTimingHookReset(HU3D_MODELID modelId);

typedef struct M616Work_s {
    OMOBJMAN *objman; /* 0x000 */
    s16 state; /* 0x004 */
    u32 time; /* 0x008 */
    HU3D_MODELID camModel; /* 0x00C */
    HU3D_MOTIONID camMot[17]; /* 0x00E */
    OMOBJ *obj; /* 0x030 */
    float camTime; /* 0x034 */
    float camMaxTime; /* 0x038 */
    HU3D_MODELID stageModel[2]; /* 0x03C */
    HU3D_MODELID daiModel[4]; /* 0x040 */
    HU3D_MOTIONID daiMot[4][13]; /* 0x048 */
    HU3D_MODELID gearModel; /* 0x0B0 */
    HU3D_MOTIONID gearMot[3]; /* 0x0B2 */
    HU3D_MODELID nejiModel[4]; /* 0x0B8 */
    HU3D_MOTIONID nejiMot[6]; /* 0x0C0 */
    HU3D_MODELID baseModel; /* 0x0CC */
    HU3D_MODELID p1stModel; /* 0x0CE */
    s32 charNo[4]; /* 0x0D0 */
    s32 padNo[4]; /* 0x0E0 */
    HU3D_MODELID charModel[4]; /* 0x0F0 */
    HU3D_MOTIONID charMot[4][11]; /* 0x0F8 */
    u16 comChoice[4]; /* 0x150 */
    u32 comTime[4]; /* 0x158 */
    u16 btnOld[4]; /* 0x168 */
    s32 choice[4]; /* 0x170 */
    s32 score[4]; /* 0x180 */
    s32 round; /* 0x190 */
    MGTIMER *timer; /* 0x194 */
    s32 introStep; /* 0x198 */
    HU3D_MODELID effModel[4]; /* 0x19C */
    HuVecF shadowPos; /* 0x1A4 */
    HuVecF shadowTarget; /* 0x1B0 */
    HuVecF shadowUp; /* 0x1BC */
    s32 streamNo; /* 0x1C8 */
} M616WORK; /* size 0x1CC */

extern M616WORK lbl_1_bss_10;
extern HuVecF lbl_1_bss_0;

void fn_1_140(s16 mode, s16 frameNo);
void fn_1_160(s16 mode, s16 frameNo);
void fn_1_4BC(s16 mode, s16 frameNo);
void fn_1_4C0(s16 mode, s16 frameNo);
void fn_1_F14(s16 mode, s16 frameNo);
void fn_1_F78(s16 mode, s16 frameNo);
void fn_1_1284(s16 mode, s16 frameNo);
void fn_1_135C(s16 mode, s16 frameNo);
void fn_1_1360(s16 mode, s16 frameNo);
void fn_1_1364(OMOBJ *obj);
void fn_1_139C(HU3D_MODELID modelId, HU3D_MOTIONID motId, BOOL lagF);
void fn_1_1590(void);
void fn_1_1FF4(s32 no);
BOOL fn_1_20D8(void);
void fn_1_2104(s32 playerNo, s32 motNo);
void fn_1_215C(s32 playerNo, s32 motNo);
void fn_1_2254(s32 motNo);
void fn_1_22BC(s32 playerNo, s32 motNo, u32 attr);
void fn_1_23B0(void);
s32 fn_1_2580(s32 playerNo);

extern u32 lbl_1_data_1F0[17];
extern u32 lbl_1_data_234[6];
extern u32 lbl_1_data_24C[13];
extern unsigned int lbl_1_data_280[12];
extern s32 lbl_1_data_2B0[4][2];
extern u32 lbl_1_data_2D0[3];

extern MGSEQ_PARAM lbl_1_data_0;

#endif
