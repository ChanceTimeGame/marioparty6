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

/* .bss placeholders (sizes from the asm). The part whose functions use one replaces it with the
   real layout in its report; part 1 merges it into the unit's HEAD. */
typedef struct W01Bss0_s { u8 unk[0x480]; } W01_BSS_0; /* placeholder: owner replaces */
typedef struct W01Bss4B0_s { u8 unk[0xA00]; } W01_BSS_4B0; /* placeholder: owner replaces */
typedef struct W01BssEB4_s { u8 unk[0x30]; } W01_BSS_EB4; /* placeholder: owner replaces */
typedef struct W01BssEE8_s { u8 unk[0x10]; } W01_BSS_EE8; /* placeholder: owner replaces */
typedef struct W01BssEFC_s { u8 unk[0x28C]; } W01_BSS_EFC; /* placeholder: owner replaces */
typedef struct W01Bss1188_s { u8 unk[0x80]; } W01_BSS_1188; /* placeholder: owner replaces */
typedef struct W01Bss1240_s { u8 unk[0xF4]; } W01_BSS_1240; /* placeholder: owner replaces */
typedef struct W01Bss1338_s { u8 unk[0x40]; } W01_BSS_1338; /* placeholder: owner replaces */
typedef struct W01Bss137C_s { u8 unk[0x54]; } W01_BSS_137C; /* placeholder: owner replaces */
typedef struct W01Bss13D8_s { u8 unk[0x38]; } W01_BSS_13D8; /* placeholder: owner replaces */
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
void fn_1_3214(void);
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
void fn_1_5FB4(void);
void fn_1_6D98(void);
int fn_1_7388(int arg0);
void fn_1_7448(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);
void fn_1_77AC(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);
void fn_1_7944(void);
void fn_1_7B20(void);
void fn_1_7B70(int arg0);
void fn_1_7C14(void);
void fn_1_7D14(int arg0, int arg1);
void fn_1_7D84(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);
void fn_1_80B4(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);
void fn_1_8474(void);
void fn_1_8798(void);
void fn_1_87E8(void);
void fn_1_8860(int playerNo);
void fn_1_9134(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);
void fn_1_939C(int arg0, int arg1, int arg2);
void fn_1_950C(int arg0);
void fn_1_9574(void);
void fn_1_99E4(void);
void fn_1_9AEC(void);
void fn_1_A82C(void);
void fn_1_ACDC(OMOBJ *obj);
void fn_1_AFD4(int arg0, int arg1);
void fn_1_B01C(void);
void fn_1_B3F8(int arg0);
void fn_1_B434(void);
void fn_1_B7D8(int arg0);
void fn_1_B97C(int arg0);
void fn_1_B9A8(void);
void fn_1_BB44(int arg0, int arg1);
void fn_1_BFA0(HU3D_MODEL *modelP, Mtx *mtx);
void fn_1_C694(int arg0, int arg1);
void fn_1_C7DC(void);
int fn_1_CAA4(void);
int fn_1_CCEC(void);
int fn_1_D080(int arg0, float f0);
int fn_1_D1D0(int arg0, float f0);
void fn_1_D320(void);
void fn_1_D50C(void);
void fn_1_DEA4(void);
void fn_1_E234(int playerNo);
void fn_1_ED00(void);
void fn_1_F06C(void);
void fn_1_F0D0(void);
void fn_1_F1F4(void);
void fn_1_F66C(int arg0);
int fn_1_1089C(void);
void fn_1_10900(void);
void fn_1_10998(int arg0);
void fn_1_10AA8(void);
void fn_1_10C68(void);
void fn_1_10CB8(void);
void fn_1_11C14(int arg0);
void fn_1_11C84(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);
void fn_1_12090(void);
void fn_1_12454(void);
void fn_1_124C8(void);
void fn_1_126EC(void);
void fn_1_12A4C(void);
void fn_1_12CF0(void);
void fn_1_12D58(HU3D_MODEL *modelP, Mtx *mtx);
int fn_1_130C4(void);
void fn_1_1330C(void);
void fn_1_139E8(int arg0);
void fn_1_139F8(void);
void fn_1_13B38(void);
void fn_1_13BC4(void);
void fn_1_13CBC(int arg0);
void fn_1_13D04(void);
void fn_1_13D8C(int arg0, int arg1);
void fn_1_13F50(void);
void fn_1_14108(void);
void fn_1_142B0(void);
void fn_1_144C0(void);
void fn_1_147DC(void);
void fn_1_14A90(int arg0, int arg1, int arg2, float f0);
void fn_1_14B34(int arg0, int arg1, int arg2, int arg3, float f0);
void fn_1_14BF0(int arg0, int arg1, int arg2, int arg3, float f0);
void fn_1_14CAC(float f0, float f1, float f2, float f3, float f4);
void fn_1_14D34(float f0, float f1, float f2, float f3, float f4);
void fn_1_14E0C(int arg0, int arg1, int arg2, int arg3, int arg4, float f0);
void fn_1_150CC(int arg0, int arg1, int arg2, int arg3, int arg4, float f0);
void fn_1_1547C(void);
void fn_1_155EC(int arg0, int arg1, float f0);
void fn_1_15B60(void);
void fn_1_15E1C(int arg0, int arg1, int arg2);
void fn_1_15FF8(void);
void fn_1_16148(int arg0, int arg1, int arg2, float f0);
void fn_1_161AC(int arg0, int arg1, int arg2, float f0, float f1);
void fn_1_1644C(void);
int fn_1_16478(void);

#endif
