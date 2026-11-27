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

/* .bss types (from every part) */
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
    /* 0x00 */ HuVecF pos;
    /* 0x0C */ MBOBJID objId;
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

typedef struct W01BranchLeaf_s {
    HuVecF pos; /* 0x00 */
    HuVecF rot; /* 0x0C */
    HuVecF rotBase; /* 0x18 */
    HuVecF vel; /* 0x24 */
    HuVecF windScale; /* 0x30 */
    s16 time; /* 0x3C */
    float angle; /* 0x40 */
    float angVel; /* 0x44 */
} W01_BRANCHLEAF; /* size 0x48 */

typedef struct W01Branch_s {
    MBOBJID objId; /* 0x00 */
    char objName[18]; /* 0x02 */
    W01_BRANCHLEAF *leaf; /* 0x14 */
    s32 leafNum; /* 0x18 */
    s32 attachF; /* 0x1C */
    s16 fadeF; /* 0x20 */
    s32 playerNo; /* 0x24 */
    HuVecF pos; /* 0x28 */
    HuVecF vel; /* 0x34 */
    OMOBJ *obj; /* 0x40 */
    s16 fadeTime; /* 0x44 */
    s16 fadeMax; /* 0x46 */
    u32 unk48; /* 0x48 */
    s32 moveF; /* 0x4C */
} W01_BRANCH; /* size 0x50 */

typedef struct W01ZSort_s {
    HuVecF pos; /* 0x00 */
    s32 colNo; /* 0x0C */
    float depth; /* 0x10: a float (fn_1_BFA0 stores m[2][1] and takes fabs of it) */
} W01ZSORT; /* size 0x14 */

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

typedef float (*W01_CURVEFUNC)(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float t);

/* lbl_1_bss_0 (part 7) */
typedef struct W01Spark_s {
    /* 0x00 */ BOOL activeF;
    /* 0x04 */ MBOBJID objId;
    /* 0x08 */ HuVecF pos;
    /* 0x14 */ HuVecF vel;
    /* 0x20 */ s16 time;
    /* 0x22 */ s16 maxTime;
} W01_SPARK; /* size 0x24 */

/* lbl_1_bss_1618: GwSystem.boardWork (part 7) */
typedef struct W01StarWork_s {
    /* 0x00 */ u8 unk0[4];
    /* 0x04 */ u8 starNum;
    /* 0x05 */ u8 starOrder[8];
    /* 0x0D */ u8 starIdx;
} W01_STARWORK;

/* lbl_1_bss_1438: double-pendulum swing (part 2) */
typedef struct W01Swing_s {
    /* 0x00 */ int playerNo;
    /* 0x04 */ MBOBJID objId[2];
    /* 0x08 */ MBOBJID baseObjId;
    /* 0x0C */ float angle[2];
    /* 0x14 */ float angVel[2];
    /* 0x1C */ float mass[2];
    /* 0x24 */ float len[2];
    /* 0x2C */ HuVecF pos;
    /* 0x38 */ HuVecF point[2];
} W01_SWING; /* size 0x50; lbl_1_bss_1438 is W01_SWING[1] */

/* lbl_1_bss_148C: bobbing platforms, lbl_1_bss_1490 of them (part 2) */
typedef struct W01Platform_s {
    /* 0x00 */ int onF;
    /* 0x04 */ MBOBJID objId;
    /* 0x08 */ HuVecF pos;
    /* 0x14 */ float unk14;
    /* 0x18 */ float vel;
    /* 0x1C */ float unk1C;
    /* 0x20 */ s16 masuId;
    /* 0x22 */ s16 time;
    /* 0x24 */ s16 maxTime;
    /* 0x28 */ float speed;
} W01_PLATFORM; /* size 0x2C */

/* lbl_1_bss_1494: hanging chain the player rides (parts 1, 2) */
typedef struct W01ChainLink_s {
    /* 0x00 */ MBOBJID objId;
    /* 0x04 */ HuVecF pos;
    /* 0x10 */ HuVecF vel;
} W01_CHAINLINK; /* size 0x1C */

typedef struct W01Chain_s {
    /* 0x00 */ s16 masuA;
    /* 0x02 */ s16 masuB;
    /* 0x04 */ HuVecF posA;
    /* 0x10 */ HuVecF posB;
    /* 0x1C */ HuVecF dir;
    /* 0x28 */ float rotY;
    /* 0x2C */ float len;
    /* 0x30 */ int playerNo;
    /* 0x34 */ MBOBJID ropeId[16];
    /* 0x54 */ MBOBJID objId;
    /* 0x58 */ W01_CHAINLINK link[4];
} W01_CHAIN; /* size 0xC8 */

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

/* lbl_1_bss_155C: the rope bridge (fn_1_10A0, fn_1_13CC, fn_1_2A08) */
typedef struct W01BridgePlank_s {
    MBOBJID mdlId; //0x00
    s16 masuId; //0x02
} W01_BRIDGEPLANK; //size 0x4

typedef struct W01Bridge_s {
    s16 masuA; //0x00
    s16 masuB; //0x02
    HuVecF posA; //0x04
    HuVecF posB; //0x10
    HuVecF dir; //0x1C
    float curLen; //0x28 where the bridge sags most, eased toward the player
    float len; //0x2C
    float rotY; //0x30
    BOOL resetF; //0x34
    W01_BRIDGEPLANK plank[6]; //0x38
    MBOBJID ropeId[7]; //0x50
    u8 unk5E[0x12]; //0x5E
    HuVecF ropeEndA; //0x70
    HuVecF ropeEndB; //0x7C
    s16 lastMasu; //0x88
} W01_BRIDGE; //size 0x8C

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


void ObjectSetup(void);

/* Every global function of w01.c, in reference order (signatures from the definitions). */
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
void fn_1_9F4(int objId);
void fn_1_AC4(BOOL startF);
void fn_1_B68(void);
void fn_1_DD0(void);
void fn_1_10A0(void);
void fn_1_13CC(void);
void fn_1_2A08(void);
void fn_1_2A1C(void);
void fn_1_2D08(int playerNo);
void fn_1_3214(float t);
void fn_1_4004(void);
void fn_1_44C0(void);
void fn_1_45A0(void);
void fn_1_4750(void);
void fn_1_47B0(void);
void fn_1_4C6C(int playerNo);
void fn_1_4CD0(void);
void fn_1_4D78(s16 masuId);
void fn_1_4D88(void);
void fn_1_4F94(int playerNo);
void fn_1_5560(int no);
void fn_1_5888(int no);
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
void fn_1_7B70(int no);
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
void fn_1_950C(BOOL dispF);
void fn_1_9574(void);
void fn_1_99E4(void);
void fn_1_9AEC(int playerNo, s16 masuId);
int fn_1_A82C(int playerNo);
void fn_1_ACDC(OMOBJ *obj);
void fn_1_AFD4(int no, int playerNo);
void fn_1_B01C(int no, int pathNo);
void fn_1_B3F8(int no);
int fn_1_B434(W01_BRANCHLEAF **leafP);
void fn_1_B7D8(int no);
void fn_1_B97C(int no);
void fn_1_B9A8(int no, int time);
void fn_1_BB44(W01_BRANCH *tree, HuVecF *pos);
void fn_1_BFA0(HU3D_MODEL *modelP, Mtx *mtx);
void fn_1_C694(const s32 *data, u32 color);
s32 fn_1_C7DC(void **dlP);
s32 fn_1_CAA4(void **dlP);
s32 fn_1_CCEC(HuVecF *src, s32 triNum, float radius, HuVecF *dst);
s32 fn_1_D080(HuVecF *dst, float radius);
s32 fn_1_D1D0(HuVecF *dst, float radius);
s32 fn_1_D320(HuVecF *src, s32 num, HuVecF *dst);
void fn_1_D50C(W01ZSORT *list, s32 left, s32 right);
void fn_1_DEA4(void);
void fn_1_E234(int playerNo);
void fn_1_ED00(void);
void fn_1_F06C(void);
void fn_1_F0D0(void);
void fn_1_F1F4(int playerNo, s16 masuId);
void fn_1_F66C(W01BATSUB *sub);
BOOL fn_1_1089C(void);
void fn_1_10900(void);
void fn_1_10998(int num);
void fn_1_10AA8(void);
void fn_1_10C68(void);
void fn_1_10CB8(int playerNo, s16 masuId);
void fn_1_11C14(HuVecF *pos);
void fn_1_11C84(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);
void fn_1_12090(void);
void fn_1_12454(void);
void fn_1_124C8(void);
void fn_1_126EC(int playerNo, s16 masuId);
void fn_1_12A4C(void);
void fn_1_12CF0(void);
void fn_1_12D58(HU3D_MODEL *modelP, Mtx *mtx);
u32 fn_1_130C4(void **dlBuf);
void fn_1_1330C(void);
void fn_1_139E8(BOOL flag);
void fn_1_139F8(void);
void fn_1_13B38(void);
void fn_1_13BC4(void);
s16 fn_1_13CBC(int playerNo);
void fn_1_13D04(void);
void fn_1_13D8C(HuVecF *pos, int num);
void fn_1_13F50(void);
float fn_1_14108(W01_CURVEFUNC func, HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float t);
float fn_1_142B0(W01_CURVEFUNC func, HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float t);
float fn_1_144C0(W01_CURVEFUNC func, HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float t, float len, int maxIter);
float fn_1_147DC(W01_CURVEFUNC func, HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float t, float len, int maxIter);
float fn_1_14A90(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float t);
float fn_1_14B34(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float t);
float fn_1_14BF0(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float t);
float fn_1_14CAC(float a, float b, float c, float d, float t);
float fn_1_14D34(float a, float b, float c, float d, float t);
void fn_1_14E0C(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, HuVecF *dst, float t);
void fn_1_150CC(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, HuVecF *dst, float t);
void fn_1_1547C(HuVecF *src, int num, HuVecF *dst, float t);
float fn_1_155EC(int i, int k, float t);
void fn_1_15B60(HuVecF *ctrl, int num, HuVecF *dst, float t);
void fn_1_15E1C(HuVecF *list, int idx, int num, HuVecF *p0, HuVecF *p1, HuVecF *p2, HuVecF *p3);
void fn_1_15FF8(HuVecF *list, int idx, int num, HuVecF *p0, HuVecF *p1, HuVecF *t0, HuVecF *t1);
void fn_1_16148(HuVecF *a, HuVecF *b, HuVecF *dst, float t);
void fn_1_161AC(int playerNo, HuVecF *target, float rotY, float height, int time);
void fn_1_1644C(void);
OSTick fn_1_16478(void);

#endif
