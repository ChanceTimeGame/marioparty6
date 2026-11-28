#ifndef _REL_M635DLL_M635_H
#define _REL_M635DLL_M635_H

#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/seqman.h"
#include "game/sprite.h"
#include "game/wipe.h"
#include "string.h"
#include "dolphin.h"

#include "datadir_enum.h"

/* frandmod is called unprototyped: game/frand.h stays out of this REL */

typedef struct M635Anim_s {
    HU3D_MODELID model; /* 0x00 */
    s16 time; /* 0x02 */
} M635_ANIM; /* size 0x04 */

typedef struct M635Team_s {
    u16 btnNo[2]; /* 0x00 */
    s16 count[2]; /* 0x04 */
    s16 progress; /* 0x08 */
    s16 member; /* 0x0A */
    s16 state; /* 0x0C */
    s16 level; /* 0x0E */
    M635_ANIM anim[4]; /* 0x10 */
    HU3D_MODELID model20; /* 0x20 */
    HU3D_MODELID model22; /* 0x22 */
    HU3D_MODELID model24; /* 0x24 */
} M635_TEAM; /* size 0x26 */

typedef struct M635Work_s {
    M635_TEAM team[2]; /* 0x00 */
    HU3D_LIGHTID lightId; /* 0x4C */
    HU3D_MODELID stageModel; /* 0x4E */
    HU3D_MODELID stageModel2; /* 0x50 */
    s16 winner; /* 0x52 */
    s16 stage; /* 0x54 */
    s16 mode; /* 0x56 */
    int streamNo; /* 0x58 */
} M635_WORK; /* size 0x5C */

typedef struct M635Player_s {
    HU3D_MODELID model; /* 0x00 */
    HU3D_MOTIONID mot[6]; /* 0x02 */
    s16 playerNo; /* 0x0E */
    s16 charNo; /* 0x10 */
    s16 padNo; /* 0x12 */
    s16 grpNo; /* 0x14 */
    s16 memberNo; /* 0x16 */
    s16 type; /* 0x18 */
    s16 comDif; /* 0x1A */
    float posZ; /* 0x1C */
    s16 comTimer; /* 0x20 */
    s16 step; /* 0x22 */
} M635_PLAYER; /* size 0x24 */

typedef struct M635Btn_s {
    HUSPR_GROUPID grpId; /* 0x00 */
    s16 mode; /* 0x02 */
    s16 time; /* 0x04 */
    s16 no; /* 0x06 */
    float scale; /* 0x08 */
    u32 unkC; /* 0x0C */
} M635_BTN; /* size 0x10 */

typedef struct M635Car_s {
    HU3D_MODELID model; /* 0x00 */
    s16 wait; /* 0x02 */
    float posX; /* 0x04 */
    s16 dir; /* 0x08 */
} M635_CAR; /* size 0x0C */

extern M635_WORK lbl_1_bss_4;
extern M635_CAR lbl_1_bss_68;
extern M635_BTN lbl_1_bss_74[4];
extern s16 lbl_1_bss_B4[2][2];
extern M635_PLAYER lbl_1_bss_BC[4];

void ObjectSetup(void);
void fn_1_F0(s16 mode, s16 frameNo);
void fn_1_134(s16 mode, s16 frameNo);
void fn_1_164(s16 mode, s16 frameNo);
void fn_1_1AC(s16 mode, s16 frameNo);
void fn_1_224(s16 mode, s16 frameNo);
void fn_1_2D0(s16 mode, s16 frameNo);
void fn_1_3D8(s16 mode, s16 frameNo);
void fn_1_3DC(s16 mode, s16 frameNo);
void fn_1_3E0(s16 mode, s16 frameNo);
void fn_1_408(s16 frameNo);
void fn_1_638(void);
void fn_1_8B0(void);
void fn_1_954(s16 frameNo);
void fn_1_9B8(s16 team);
s16 fn_1_D84(void);
u16 fn_1_EC0(u16 btnNo, s16 state);
void fn_1_F44(s16 winner);
void fn_1_10A0(void);
u16 fn_1_1168(s16 team, s16 member);
s16 fn_1_1308(s16 comDif);
s16 fn_1_13F8(s16 comDif);
void fn_1_14E8(void);

void fn_1_1774(OMOBJMAN *objman);
void fn_1_195C(void);
void fn_1_2014(void);
void fn_1_2018(void);
void fn_1_2268(s16 team, s16 member);
void fn_1_2330(void);
void fn_1_25EC(s16 team, s16 member, s16 mode);
s16 fn_1_27E4(s16 team, s16 member);
void fn_1_280C(s16 team, s16 member, s16 no);
void fn_1_28A8(s16 team, s16 member);
void fn_1_2904(void);
void fn_1_2954(void);
void fn_1_2CE8(void);
void fn_1_2F08(s16 player, s16 step);
void fn_1_2F40(s16 player);
s32 fn_1_30C8(s16 player, s16 charNo);
void fn_1_31BC(s16 charNo);
void fn_1_3218(s16 player, s16 motNo, float shift);
s16 fn_1_32E0(s16 team, s16 member);
void fn_1_3340(void);
void fn_1_33F8(void);
void fn_1_33FC(s16 team);
void fn_1_3738(s16 team, s16 level);
void fn_1_3AC4(s16 team);
void fn_1_3B7C(s16 team, s16 member, s16 player);
void fn_1_3C34(void);
void fn_1_4114(void);
void fn_1_42A0(void);
void fn_1_448C(void);

#endif
