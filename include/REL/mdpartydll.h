#ifndef _REL_MDPARTYDLL_H
#define _REL_MDPARTYDLL_H

#include "game/hu3d.h"
#include "game/window.h"
#include "game/sprite.h"
#include "game/object.h"
#include "game/process.h"
#include "game/animdata.h"

/* lbl_1_data_C0: sprite creation table (layout as MDSEL_SPRDATA) */
typedef struct MdPartySprData_s {
    s16 grpNo; //0x00
    s16 memberNo; //0x02
    s16 animNo; //0x04
    s16 prio; //0x06
    s16 bank; //0x08
    float posX; //0x0C
    float posY; //0x10
    float scaleX; //0x14
    float scaleY; //0x18
    float zRot; //0x1C
} MDPARTY_SPRDATA; //size 0x20

/* lbl_1_bss_9BC: per-player setup */
typedef struct MdPartyPlayer_s {
    s16 state; //0x00
    s16 unk2; //0x02
    s16 com; //0x04
    s16 comDif; //0x06
    s16 charNo; //0x08
    s16 padNo; //0x0A
    s16 handicap; //0x0C
} MDPARTY_PLAYER; //size 0xE

/* lbl_1_bss_9F4: game rules */
typedef struct MdPartyRule_s {
    s16 tagF; //0x00
    s16 playerNum; //0x02 human players
    s16 boardNo; //0x04
    s16 turnNo; //0x06 turnMax = turnNo * 5 + 10
    s16 mgPack; //0x08
    s16 bonusStarF; //0x0A
    s16 unkC; //0x0C
    s16 teamNo; //0x0E row of lbl_1_data_4
    u32 mesNo[2]; //0x10
    s16 unk18; //0x18
    s16 unk1A; //0x1A
} MDPARTY_RULE; //size 0x1C

typedef struct MdPartyCamera_s MDPARTY_CAMERA;
typedef void (*MDPARTY_CAMERA_FUNC)(OMOBJ *obj, MDPARTY_CAMERA *camera);

/* lbl_1_bss_A10 */
struct MdPartyCamera_s {
    OMOBJ *obj; //0x00
    HuVecF center; //0x04
    HuVecF centerEnd; //0x10
    HuVecF rot; //0x1C
    HuVecF rotEnd; //0x28
    float zoom; //0x34
    float zoomEnd; //0x38
    MDPARTY_CAMERA_FUNC func; //0x3C
    u8 unk40[0x10]; //0x40
}; //size 0x50

/* layouts not known yet: owners of the functions that use them report fields */
/* lbl_1_bss_108 (part 3) */
typedef struct MdPartyJump_s {
    s16 mode; //0x00
    HuVecF start; //0x04
    HuVecF mid; //0x10
    HuVecF end; //0x1C
    float time; //0x28
    float maxTime; //0x2C
} MDPARTY_JUMP; //size 0x30

/* lbl_1_data_E12 (part 6) */
typedef struct MdPartyCharSlot_s {
    s16 link[8]; //0x00
    s16 owner; //0x10
} MDPARTY_CHARSLOT; //size 0x12
/* lbl_1_bss_288: models (parts 4, 6, 7, 9, 10) */
typedef struct MdPartyModel_s {
    HU3D_MODELID mdlId; //0x00
    HU3D_ANIMID animId[4]; //0x02
    s16 unkA; //0x0A
    HuVecF pos; //0x0C
    HuVecF rot; //0x18
    HuVecF scale; //0x24
    s16 time; //0x30
    s16 unk32[3]; //0x32
} MDPARTY_MODEL; //size 0x38
typedef struct MdPartyWorkA96_s {
    s16 unk0[5]; //0x00
} MDPARTY_WORKA96; //size 0xA

/* .bss (definition order is the reverse of memory order) */
extern ANIMDATA *lbl_1_bss_B40[9];
extern float lbl_1_bss_AF0[20];
extern s16 lbl_1_bss_AEC;
extern s16 lbl_1_bss_AE4[4];
extern s16 lbl_1_bss_AE2;
extern s16 lbl_1_bss_ADA[4];
extern s16 lbl_1_bss_AD6[2];
extern s16 lbl_1_bss_AD2[2];
extern MDPARTY_WORKA96 lbl_1_bss_A96[6];
extern s16 lbl_1_bss_A94;
extern void *lbl_1_bss_A90;
extern ANIMDATA *lbl_1_bss_A8C;
extern ANIMDATA *lbl_1_bss_A88;
extern s32 lbl_1_bss_A84;
extern OMOBJMAN *lbl_1_bss_A80;
extern s32 lbl_1_bss_A6C[5];
extern HU3D_LIGHTID lbl_1_bss_A68[2];
extern HUWINID lbl_1_bss_A60[4];
extern MDPARTY_CAMERA lbl_1_bss_A10;
extern MDPARTY_RULE lbl_1_bss_9F4;
extern MDPARTY_PLAYER lbl_1_bss_9BC[4];
extern ANIMDATA *lbl_1_bss_93C[32];
extern HUSPR_GROUPID lbl_1_bss_918[18];
extern HUSPRID lbl_1_bss_8A4[58];
extern ANIMDATA *lbl_1_bss_800[41];
extern MDPARTY_MODEL lbl_1_bss_288[25];
extern float lbl_1_bss_228[24];
extern float lbl_1_bss_1C8[24];
extern MDPARTY_JUMP lbl_1_bss_108[4];
extern HUWINID lbl_1_bss_104[2];
extern HuVecF lbl_1_bss_D4[4];
extern HuVecF lbl_1_bss_A4[4];
extern HuVecF lbl_1_bss_8C[2];
extern float lbl_1_bss_80[3];
extern HuVecF lbl_1_bss_50[4];
extern float lbl_1_bss_38[6];
extern s16 lbl_1_bss_34;
extern OMOBJ *lbl_1_bss_30;
extern s16 lbl_1_bss_2E;
extern s16 lbl_1_bss_2C;
extern s16 lbl_1_bss_2A;
extern s16 lbl_1_bss_28;
extern OMOBJ *lbl_1_bss_24;
extern OMOBJ *lbl_1_bss_1C[2];
extern OMOBJ *lbl_1_bss_18;
extern OMOBJ *lbl_1_bss_14;
extern OMOBJ *lbl_1_bss_10;
extern OMOBJ *lbl_1_bss_C;
extern OMOBJ *lbl_1_bss_8;
extern OMOBJ *lbl_1_bss_4;
extern OMOBJMAN *lbl_1_bss_0;

/* .data, HEAD (before PART1) */
extern s32 lbl_1_data_0;
extern s16 lbl_1_data_4[3][4];
extern s32 lbl_1_data_1C[32];
extern s16 lbl_1_data_9C[18];
extern MDPARTY_SPRDATA lbl_1_data_C0[58];
extern s32 lbl_1_data_800[41];
extern HuVecF lbl_1_data_8A4[67];
extern s32 lbl_1_data_BC8;
/* .data, end of PART1 (after the strings of MdPartyWinCallback) */
extern s16 lbl_1_data_BEE[7];
extern s32 lbl_1_data_BFC[14];
/* .data, end of PART1 (after the strings of MdPartyCameraDebug) */
extern s16 lbl_1_data_DA2[3];
extern s32 lbl_1_data_DA8[3];
/* .data defined by later parts, in this order between their string literals:
   lbl_1_data_DE6 (s16[6], after the strings of fn_1_9218, before fn_1_A970),
   lbl_1_data_E12 (MDPARTY_CHARSLOT[11], after the string table of fn_1_D748, before fn_1_12364),
   lbl_1_data_F10 (s32[9], after the strings of fn_1_2DD58, before fn_1_44DCC) */

/* mdparty.c part 1 */
void MdPartyWinCallback(HUWINID winId, u32 mess, s16 no);
void MdPartyUnlockInit(void);
void MdPartyCharDataLoad(void);
void MdPartyReadProc(void);
void MdPartyReadInit(void);
void MdPartyGameWorkSet(void);
void MdPartyGameWorkTest(void);
void MdPartyOvlGoto(s16 mode);
void MdPartyARamFree(void);
void MdPartyARamFreeReturn(void);
float MdPartyBezier(float a, float b, float c, float t);
void MdPartyBezierV(HuVecF *out, float t, HuVecF *a, HuVecF *b, HuVecF *c);
float MdPartySmooth(float cur, float target, float speed);
void MdPartySmoothV(HuVecF *cur, HuVecF *target, float speed);
float MdPartySinLerp90(float start, float end, float time, float maxTime);
float MdPartyLerp(float start, float end, float time, float maxTime);
void MdPartyModelMoveLook(HU3D_MODELID mdlId, HuVecF *start, HuVecF *end, float time, float maxTime);
void MdPartyModelRotTurn(HU3D_MODELID mdlId, float rotY);
void MdPartyModelMoveTurn(HU3D_MODELID mdlId, HuVecF *start, HuVecF *end, float time, float maxTime, float rotY);
float MdPartySinLerp180(float start, float end, float time, float maxTime);
void MdPartyModelBezierMove(HU3D_MODELID mdlId, HuVecF *a, HuVecF *b, HuVecF *c, float time, float maxTime);
void MdPartyPlayerInit(void);
void MdPartyPlayerComSet(void);
void MdPartyCameraJump(MDPARTY_CAMERA *camera);
void MdPartyCameraStop(MDPARTY_CAMERA *camera);
void MdPartyCameraSmooth(MDPARTY_CAMERA *camera, float speed);
void MdPartyCameraFuncSet(MDPARTY_CAMERA_FUNC func);
void MdPartyCameraDebug(OMOBJ *obj, MDPARTY_CAMERA *camera);
void MdPartyCameraFuncCall(OMOBJ *obj, MDPARTY_CAMERA *camera);
void MdPartyCameraUpdate(OMOBJ *obj);
void MdPartyCameraInit(MDPARTY_CAMERA_FUNC func);
void MdPartyCameraKill(void);
void MdPartyLightInit(void);
void MdPartyLightKill(void);
void MdPartyWinOpen(s16 winNo);
void MdPartyWinClose(s16 winNo);
void MdPartyWinWait(s16 winNo);
s16 MdPartyWinChoiceGet(s16 winNo, s16 mode);

/* old fn_1_ names of renamed functions (so parts calling the old names still inline) */
#define fn_1_0 MdPartyWinCallback
#define fn_1_1B4 MdPartyUnlockInit
#define fn_1_2B0 MdPartyCharDataLoad
#define fn_1_400 MdPartyReadProc
#define fn_1_64C MdPartyReadInit
#define fn_1_718 MdPartyGameWorkSet
#define fn_1_E30 MdPartyGameWorkTest
#define fn_1_109C MdPartyOvlGoto
#define fn_1_1248 MdPartyARamFree
#define fn_1_12F8 MdPartyARamFreeReturn
#define fn_1_13A8 MdPartyBezier
#define fn_1_1404 MdPartyBezierV
#define fn_1_160C MdPartySmooth
#define fn_1_163C MdPartySmoothV
#define fn_1_1780 MdPartySinLerp90
#define fn_1_1868 MdPartyLerp
#define fn_1_18AC MdPartyModelMoveLook
#define fn_1_1C14 MdPartyModelRotTurn
#define fn_1_1D54 MdPartyModelMoveTurn
#define fn_1_21D4 MdPartySinLerp180
#define fn_1_22A8 MdPartyModelBezierMove
#define fn_1_26FC MdPartyPlayerInit
#define fn_1_275C MdPartyPlayerComSet
#define fn_1_2810 MdPartyCameraJump
#define fn_1_2860 MdPartyCameraStop
#define fn_1_28B0 MdPartyCameraSmooth
#define fn_1_2B64 MdPartyCameraFuncSet
#define fn_1_2B74 MdPartyCameraDebug
#define fn_1_3284 MdPartyCameraFuncCall
#define fn_1_32D0 MdPartyCameraUpdate
#define fn_1_33A0 MdPartyCameraInit
#define fn_1_353C MdPartyCameraKill
#define fn_1_3598 MdPartyLightInit
#define fn_1_36C4 MdPartyLightKill
#define fn_1_3700 MdPartyWinOpen
#define fn_1_3770 MdPartyWinClose
#define fn_1_37E0 MdPartyWinWait
#define fn_1_381C MdPartyWinChoiceGet

#endif
