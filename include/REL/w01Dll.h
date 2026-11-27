#ifndef _REL_W01DLL_H
#define _REL_W01DLL_H

#include "dolphin.h"
#include "humath.h"
#include "game/object.h"
#include "game/process.h"
#include "game/hu3d.h"
#include "game/gamework.h"
#include "game/board/board.h"
#include "game/board/object.h"
#include "game/board/effect.h"
#include "game/board/masu.h"
#include "game/board/player.h"
#include "datadir_enum.h"

/* Every model/motion/data number of this board comes from the day (w01) or night (w01n) directory. */
static inline BOOL mbTimeDayCheck(void)
{
    return GwSystem.curTime == 0;
}

#define W01_DIR (mbTimeDayCheck() ? DATA_w01 : DATA_w01n)
#define W01_DATANUM(file) (W01_DIR | (file))

/* GWTeamGet must be an inline function (a macro adds a clrlwi) */
static inline BOOL GWTeamGet(void)
{
    return GwSystem.tagF;
}

static inline int GWPlayerTeamGet(int playerNo)
{
    return GwPlayer[playerNo].team;
}

/* .bss types (parts 3, 5, 6, 7) */
typedef struct W01Coin_s {
    /* 0x00 */ int activeF;
    /* 0x04 */ MBOBJID objId;
    /* 0x08 */ HuVecF vel;
    /* 0x14 */ s16 mode;
    /* 0x16 */ s16 delay;
    /* 0x18 */ s16 time;
    /* 0x1C */ int playerNo;
    /* 0x20 */ float groundY;
    /* 0x24 */ HuVecF pos;
    /* 0x30 */ HuVecF rot;
    /* 0x3C */ HU3D_MODELID modelId;
} W01_COIN; /* size 0x40 */

typedef struct W01CoinWork_s {
    /* 0x00 */ u8 unk00[0x10];
    /* 0x10 */ int num;
    /* 0x14 */ W01_COIN *coin;
    /* 0x18 */ s16 motNo[4];
    /* 0x20 */ float unk20[4];
    /* 0x30 */ s16 unk30[4];
} W01_COINWORK; /* size 0x38 */

typedef struct W01Lift_s {
    /* 0x00 */ int activeF;
    /* 0x04 */ MBOBJID objId;
    /* 0x06 */ s16 masuId;
    /* 0x08 */ HU3D_MODELID particleId[2];
    /* 0x0C */ char objName[16];
} W01_LIFT; /* size 0x1C */

typedef struct W01Flower_s {
    /* 0x00 */ int activeF;
    /* 0x04 */ int dayF;
    /* 0x08 */ MBOBJID objId;
    /* 0x0A */ s16 masuId[3];
    /* 0x10 */ char objName[16];
} W01_FLOWER; /* size 0x20 */

typedef struct W01Branch_s {
    /* 0x00 */ MBOBJID objId;
    /* 0x02 */ char objName[18];
    /* 0x14 */ u32 unk14;
    /* 0x18 */ u32 unk18;
    /* 0x1C */ int unk1C;
    /* 0x20 */ u32 unk20;
    /* 0x24 */ int unk24;
    /* 0x28 */ u8 unk28[0x18];
    /* 0x40 */ OMOBJ *obj;
    /* 0x44 */ u8 unk44[0xC];
} W01_BRANCH; /* size 0x50 */

/* lbl_1_bss_EFC (part 5) */
struct W01Bat_s;

typedef struct W01BatSub_s {
    /* 0x00 */ s16 mdlId;
    /* 0x04 */ HuVecF pos;
    /* 0x10 */ HuVecF homePos;
    /* 0x1C */ HuVecF rot;
    /* 0x28 */ s16 mode;
    /* 0x2A */ s16 no;
    /* 0x2C */ float swayAmp;
    /* 0x30 */ float swayAngle;
    /* 0x34 */ float swaySpeed;
    /* 0x38 */ float angle;
    /* 0x3C */ float posY;
    /* 0x40 */ float orbitAngle;
    /* 0x44 */ float orbitDir;
    /* 0x48 */ float radius;
    /* 0x4C */ float radiusBase;
    /* 0x50 */ float radiusVel;
    /* 0x54 */ s16 wait;
    /* 0x56 */ s16 time;
    /* 0x58 */ s16 maxTime;
    /* 0x5A */ s16 maxTimeInit;
    /* 0x5C */ HuVecF start;
    /* 0x68 */ HuVecF end;
    /* 0x74 */ struct W01Bat_s *parent;
} W01BATSUB; /* size 0x78 */

typedef struct W01Bat_s {
    /* 0x000 */ s16 mdlId;
    /* 0x004 */ HuVecF pos;
    /* 0x010 */ HuVecF rot;
    /* 0x01C */ HuVecF rotBase;
    /* 0x028 */ W01BATSUB sub[5];
    /* 0x280 */ float angle;
    /* 0x284 */ float angVel;
    /* 0x288 */ s32 mode;
} W01BAT; /* size 0x28C */

/* lbl_1_bss_4B0 (parts 6, 7) */
typedef struct W01Leaf_s {
    /* 0x00 */ HuVecF pos;
    /* 0x0C */ HuVecF vel;
    /* 0x18 */ float rot;
    /* 0x1C */ float rotVel;
    /* 0x20 */ float scale;
    /* 0x24 */ s32 texNo; /* GX_TEXMAP0-2: one of lbl_1_bss_4A4 */
} W01_LEAF; /* size 0x28 */

/* lbl_1_bss_EB4: the Boo space's Boo (objId[0]), sign (objId[1]) and its hooked part (objId[2]) */
typedef struct W01Teresa_s {
    /* 0x00 */ MBOBJID objId[3];
    /* 0x06 */ s16 masuId;
    /* 0x08 */ HuVecF masuPos;
    /* 0x14 */ HuVecF linkPos;
    /* 0x20 */ HuVecF dir;
    /* 0x2C */ float rotY;
} W01_TERESA; /* size 0x30 */

/* .bss placeholders (sizes from the asm). The part whose functions use one replaces it with the
   real layout in its report; part 1 merges it into the unit's HEAD. */
/* lbl_1_bss_15F4: board map objects (fn_1_F4) */
typedef struct W01MapObj_s {
    MBOBJID mapId; //0x00
    MBOBJID objId[16]; //0x02
} W01_MAPOBJ; //size 0x22

/* lbl_1_bss_15E8: time-change event (fn_1_B68 / fn_1_DD0) */
typedef struct W01Bss15E8_s {
    BOOL endF; //0x00 telop done, skip allowed
    BOOL skipF; //0x04
    MBOBJID mdlId; //0x08
} W01_BSS_15E8; //size 0xC

typedef struct W01BssEE8_s { u8 unk[0x10]; } W01_BSS_EE8; /* placeholder: owner replaces */
typedef struct W01Bss0_s { u8 unk[0x480]; } W01_BSS_0; /* placeholder: owner replaces */
typedef struct W01Bss1188_s { u8 unk[0x80]; } W01_BSS_1188; /* placeholder: owner replaces */
typedef struct W01Bss1438_s { u8 unk[0x50]; } W01_BSS_1438; /* placeholder: owner replaces */
typedef struct W01Bss1494_s { u8 unk[0xC8]; } W01_BSS_1494; /* placeholder: owner replaces */
typedef struct W01Bss155C_s { u8 unk[0x8C]; } W01_BSS_155C; /* placeholder: owner replaces */

void ObjectSetup(void);

/* Every global function of w01.c, in reference order. Hook-typed ones are exact; the rest are
   GUESSED from register use (argN are ints, fN floats): the part that owns a function tells part 1
   its real signature and the list is regenerated from the definitions at the final splice. */
void fn_1_F4(void);
void fn_1_60C(void);
void fn_1_644(void);
void fn_1_6D4(OMOBJ *obj);
int fn_1_734(int playerNo, s16 masuId);
int fn_1_760(int playerNo, s16 masuId);
int fn_1_860(int playerNo, s16 masuId);
void fn_1_914(int playerNo);
void fn_1_944(int playerNo);
void fn_1_970(void);
void fn_1_9F0(void);
void fn_1_9F4(int arg0);
void fn_1_AC4(BOOL startF);
void fn_1_B68(void);
void fn_1_DD0(void);
void fn_1_10A0(void);
void fn_1_13CC(void);
void fn_1_2A08(void);
void fn_1_2A1C(void);
void fn_1_2D08(int playerNo);
void fn_1_3214(float f0);
void fn_1_4004(void);
void fn_1_44C0(void);
void fn_1_45A0(void);
void fn_1_4750(void);
void fn_1_47B0(void);
void fn_1_4C6C(int arg0);
void fn_1_4CD0(void);
void fn_1_4D78(int arg0);
void fn_1_4D88(void);
void fn_1_4F94(int playerNo);
void fn_1_5560(int arg0);
void fn_1_5888(int arg0);
void fn_1_5C18(void);
void fn_1_5C7C(void);
void fn_1_5EF0(void);
void fn_1_5FB4(int playerNo, s16 masuId);
int fn_1_6D98(void);
HU3D_MODELID fn_1_7388(HuVecF *pos);
void fn_1_7448(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);
void fn_1_77AC(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);
void fn_1_7944(void);
void fn_1_7B20(void);
void fn_1_7B70(int arg0);
void fn_1_7C14(void);
void fn_1_7D14(W01_LIFT *work, HuVecF *pos);
void fn_1_7D84(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);
void fn_1_80B4(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);
void fn_1_8474(void);
void fn_1_8798(void);
void fn_1_87E8(void);
void fn_1_8860(int playerNo);
void fn_1_9134(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);
void fn_1_939C(HU3D_MODELID modelId, int no, HuVecF *pos);
void fn_1_950C(BOOL flag);
void fn_1_9574(void);
void fn_1_99E4(void);
void fn_1_9AEC(int playerNo, s16 masuId);
void fn_1_A82C(int arg0);
void fn_1_ACDC(OMOBJ *obj);
void fn_1_AFD4(int arg0, int arg1);
void fn_1_B01C(int arg0, int arg1);
void fn_1_B3F8(int arg0);
u32 fn_1_B434(u32 *arg0);
void fn_1_B7D8(int arg0);
void fn_1_B97C(int arg0);
void fn_1_B9A8(int arg0, int arg1);
void fn_1_BB44(int arg0, int arg1, float f0);
void fn_1_BFA0(HU3D_MODEL *modelP, Mtx *mtx);
void fn_1_C694(int arg0, int arg1);
u32 fn_1_C7DC(u32 *arg0);
u32 fn_1_CAA4(u32 *arg0);
int fn_1_CCEC(int arg0, int arg1, int arg2, float f0);
int fn_1_D080(int arg0, float f0);
int fn_1_D1D0(int arg0, int arg1, float f0);
void fn_1_D320(int arg0, int arg1, int arg2);
void fn_1_D50C(int arg0, int arg1, int arg2);
void fn_1_DEA4(void);
void fn_1_E234(int playerNo);
void fn_1_ED00(void);
void fn_1_F06C(void);
void fn_1_F0D0(void);
void fn_1_F1F4(int playerNo, s16 masuId);
void fn_1_F66C(int arg0);
BOOL fn_1_1089C(void);
void fn_1_10900(int arg0);
void fn_1_10998(int arg0);
void fn_1_10AA8(void);
void fn_1_10C68(void);
void fn_1_10CB8(int playerNo, s16 masuId);
void fn_1_11C14(HuVecF *pos);
void fn_1_11C84(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);
void fn_1_12090(void);
void fn_1_12454(void);
void fn_1_124C8(void);
void fn_1_126EC(int playerNo, s16 masuId); /* fn_1_760 passes masuId in r4 */
void fn_1_12A4C(void);
void fn_1_12CF0(void);
void fn_1_12D58(HU3D_MODEL *modelP, Mtx *mtx);
u32 fn_1_130C4(void **arg0);
void fn_1_1330C(void);
void fn_1_139E8(int arg0);
void fn_1_139F8(void);
void fn_1_13B38(void);
void fn_1_13BC4(void);
void fn_1_13CBC(int arg0);
void fn_1_13D04(void);
void fn_1_13D8C(HuVecF *pos, int num);
void fn_1_13F50(void);
void fn_1_14108(int arg0, int arg1, int arg2, int arg3, int arg4, float f0);
void fn_1_142B0(int arg0, int arg1, int arg2, int arg3, int arg4, float f0);
void fn_1_144C0(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, float f0, float f1);
void fn_1_147DC(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, float f0, float f1);
float fn_1_14A90(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float t);
void fn_1_14B34(int arg0, int arg1, int arg2, int arg3, float f0);
void fn_1_14BF0(int arg0, int arg1, int arg2, int arg3, float f0);
void fn_1_14CAC(float f0, float f1, float f2, float f3, float f4);
void fn_1_14D34(float f0, float f1, float f2, float f3, float f4);
void fn_1_14E0C(int arg0, int arg1, int arg2, int arg3, int arg4, float f0);
void fn_1_150CC(int arg0, int arg1, int arg2, int arg3, int arg4, float f0);
void fn_1_1547C(int arg0, int arg1, int arg2, float f0);
void fn_1_155EC(int arg0, int arg1, float f0);
void fn_1_15B60(int arg0, int arg1, int arg2, float f0);
void fn_1_15E1C(int arg0, int arg1, int arg2);
void fn_1_15FF8(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6);
void fn_1_16148(int arg0, int arg1, int arg2, float f0);
void fn_1_161AC(int arg0, int arg1, int arg2, float f0, float f1);
void fn_1_1644C(void);
int fn_1_16478(void);

#endif
