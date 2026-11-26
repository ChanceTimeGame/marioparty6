#ifndef _REL_MDSELDLL_H
#define _REL_MDSELDLL_H

#include "game/hu3d.h"
#include "game/window.h"
#include "game/sprite.h"
#include "game/object.h"
#include "game/process.h"
#include "game/mic.h"

/* mode select camera: pos/target ease toward posEnd/targetEnd */
typedef struct MdSelCamera_s {
    HuVecF pos; //0x00
    HuVecF posEnd; //0x0C
    HuVecF target; //0x18
    HuVecF targetEnd; //0x24
} MDSEL_CAMERA; //size 0x30

typedef struct MdSelSprData_s {
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
} MDSEL_SPRDATA; //size 0x20

/* lbl_1_bss_24C, _8AC, _9BC (layout from part 2) */
typedef struct MdSelActor_s {
    s16 active; //0x00
    float time; //0x04
    float maxTime; //0x08
    HuVecF pos; //0x0C
    HuVecF end; //0x18
    float rotTime; //0x24
    float rotMaxTime; //0x28
    float unk2C; //0x2C
    float wait; //0x30
    float unk34; //0x34
    u8 unk38[8]; //0x38
    s16 mode; //0x40
    s16 unk42; //0x42
    s16 unk44; //0x44
    s16 unk46; //0x46
    float moveTime; //0x48
    float moveMaxTime; //0x4C
    float rotTarget; //0x50
    float unk54; //0x54
    HuVecF moveStart; //0x58
    HuVecF moveEnd; //0x64
    u8 unk70[0x18]; //0x70
} MDSEL_ACTOR; //size 0x88

/* lbl_1_bss_A0, _128 (layout from part 3) */
typedef struct MdSelPath_s {
    u32 unk0; //0x00
    float time; //0x04
    float maxTime; //0x08
    HuVecF pos[3]; //0x0C
    u8 unk30[0x58]; //0x30
} MDSEL_PATH; //size 0x88

/* lbl_1_bss_1D4: per-model sound (layout from part 2) */
typedef struct MdSelSe_s {
    s16 mdlId; //0x00
    int seNo; //0x04
} MDSEL_SE; //size 0x8

typedef struct MdSelWorkA_s {
    s16 unk00[5]; //0x00
} MDSEL_WORKA; //size 0xA

/* .bss (definition order is the reverse of memory order) */
extern HUMC_CONTEXTID lbl_1_bss_1A4C;
extern HU3D_LIGHTID lbl_1_bss_1A48[2];
extern HUWINID lbl_1_bss_1A40[4];
extern ANIMDATA *lbl_1_bss_1A3C[1];
extern HUSPR_GROUPID lbl_1_bss_1A38[1];
extern HUSPRID lbl_1_bss_1A36[1];
extern s16 lbl_1_bss_1A30[3];
extern s32 lbl_1_bss_1A2C;
extern s32 lbl_1_bss_1A28;
extern s16 lbl_1_bss_19EC[30];
extern s32 lbl_1_bss_19DC[4];
extern MDSEL_CAMERA lbl_1_bss_19AC;
extern MDSEL_ACTOR lbl_1_bss_9BC[30];
extern MDSEL_ACTOR lbl_1_bss_8AC[2];
extern MDSEL_ACTOR lbl_1_bss_24C[12];
extern MDSEL_SE lbl_1_bss_1D4[15];
extern s16 lbl_1_bss_1B0[6][3];
extern MDSEL_PATH lbl_1_bss_128;
extern MDSEL_PATH lbl_1_bss_A0;
extern HuVecF lbl_1_bss_94;
extern ANIMDATA *lbl_1_bss_80[5];
extern MDSEL_WORKA lbl_1_bss_44[6];
extern float lbl_1_bss_40;
extern s16 lbl_1_bss_3E;
extern s16 lbl_1_bss_3C;
extern s16 lbl_1_bss_3A;
extern s16 lbl_1_bss_38;
extern OMOBJ *lbl_1_bss_34;
extern OMOBJ *lbl_1_bss_30;
extern float lbl_1_bss_2C;
extern void *lbl_1_bss_28;
extern OMOBJ *lbl_1_bss_20;
extern OMOBJ *lbl_1_bss_1C;
extern OMOBJ *lbl_1_bss_18;
extern OMOBJ *lbl_1_bss_14;
extern OMOBJ *lbl_1_bss_10;
extern OMOBJ *lbl_1_bss_C;
extern OMOBJMAN *lbl_1_bss_8;
extern s32 lbl_1_bss_4;
extern s32 lbl_1_bss_0;

/* .data */
extern s32 lbl_1_data_0[1];
extern s16 lbl_1_data_4[1];
extern MDSEL_SPRDATA lbl_1_data_8[1];
extern HuVecF lbl_1_data_28[6];
extern s16 lbl_1_data_70[6];
extern s32 lbl_1_data_7C;
extern s16 lbl_1_data_150[2];
extern s32 lbl_1_data_154[2];
extern s16 lbl_1_data_15C;
/* defined by part 3 (after fn_1_8AE0, before fn_1_8D44) */
extern s16 lbl_1_data_2D0;
/* defined by part 5 (before fn_1_F790) */
extern s32 lbl_1_data_35C[5];

/* mdsel.c part 1 */
void MdSelWinCallback(HUWINID winId, u32 mess, s16 no);
void MdSelOvlCall(void);
void MdSelARamFree(void);
void MdSelARamDump(void);
void MdSelSprGrpAttrSet(HUSPR_GROUPID grpId, u32 attr);
void MdSelSprGrpAttrReset(HUSPR_GROUPID grpId, u32 attr);
float MdSelLerp(float start, float end, float time, float maxTime);
float MdSelSmooth(float cur, float target, float speed);
void MdSelSmoothV(HuVecF *cur, HuVecF *target, float speed);
float MdSelSinLerp90(float start, float end, float time, float maxTime);
float MdSelSinLerp360(float start, float end, float time, float maxTime);
float MdSelSinLerp180(float start, float end, float time, float maxTime);
float MdSelBezier(float a, float b, float c, float t);
void MdSelBezierV(HuVecF *out, float t, HuVecF *a, HuVecF *b, HuVecF *c);
float MdSelSmoothNoCheck(float cur, float target, float speed);
float MdSelLerp2(float start, float end, float time, float maxTime);
void MdSelModelMoveLook(HU3D_MODELID mdlId, HuVecF *start, HuVecF *end, float time, float maxTime);
void MdSelCameraTargetSet(s16 no);
void MdSelCameraZoomSet(void);
void MdSelCameraUpdate(OMOBJ *obj);
void MdSelCameraInit(void);
void MdSelCameraKill(void);
void MdSelLightInit(void);
void MdSelLightKill(void);
void MdSelWinOpen(s16 winNo);
void MdSelWinClose(s16 winNo);
void MdSelWinWait(s16 winNo);
s16 MdSelWinChoiceGet(s16 winNo, s16 mode);
void MdSelWinMesSet(s16 winNo, s32 mess, s16 speed);
void MdSelWinInit(void);
void MdSelWinKill(void);
void MdSelMainWinOpen(s16 winNo);
void MdSelMainWinClose(void);
void MdSelMainWinWait(void);
s16 MdSelMainWinChoiceGet(s16 mode);
void MdSelMainWinMesSet(s16 winNo, s32 mess, s16 speed);
void MdSelHelpWinMesSet(s32 mess);
void MdSelHelpWinClose(void);
void MdSelSprInit(void);
void MdSelSprKill(void);
void MdSelFbHook(s16 layerNo);
void MdSelFbHookSet(void);
void MdSelFbInit(void);
void MdSelFbKill(void);
void MdSelGuideIdleWait(OMOBJ *obj);
void MdSelGuideIdleSet(void);
void MdSelGuide2IdleWait(OMOBJ *obj);


/* transition aliases: part 1 functions under their old fn_1_ names (drop once parts use the names) */
#define fn_1_0 MdSelWinCallback
#define fn_1_1B4 MdSelOvlCall
#define fn_1_2CC MdSelARamFree
#define fn_1_37C MdSelARamDump
#define fn_1_40C MdSelSprGrpAttrSet
#define fn_1_48C MdSelSprGrpAttrReset
#define fn_1_50C MdSelLerp
#define fn_1_550 MdSelSmooth
#define fn_1_598 MdSelSmoothV
#define fn_1_724 MdSelSinLerp90
#define fn_1_80C MdSelSinLerp360
#define fn_1_8E8 MdSelSinLerp180
#define fn_1_9C4 MdSelBezier
#define fn_1_A20 MdSelBezierV
#define fn_1_C28 MdSelSmoothNoCheck
#define fn_1_C48 MdSelLerp2
#define fn_1_C8C MdSelModelMoveLook
#define fn_1_FEC MdSelCameraTargetSet
#define fn_1_11D4 MdSelCameraZoomSet
#define fn_1_1370 MdSelCameraUpdate
#define fn_1_1734 MdSelCameraInit
#define fn_1_1964 MdSelCameraKill
#define fn_1_1988 MdSelLightInit
#define fn_1_1AD8 MdSelLightKill
#define fn_1_1B30 MdSelWinOpen
#define fn_1_1BA0 MdSelWinClose
#define fn_1_1C10 MdSelWinWait
#define fn_1_1C4C MdSelWinChoiceGet
#define fn_1_1D20 MdSelWinMesSet
#define fn_1_1DDC MdSelWinInit
#define fn_1_2024 MdSelWinKill
#define fn_1_2080 MdSelMainWinOpen
#define fn_1_21DC MdSelMainWinClose
#define fn_1_2288 MdSelMainWinWait
#define fn_1_22E8 MdSelMainWinChoiceGet
#define fn_1_23E0 MdSelMainWinMesSet
#define fn_1_25F8 MdSelHelpWinMesSet
#define fn_1_277C MdSelHelpWinClose
#define fn_1_2828 MdSelSprInit
#define fn_1_2A74 MdSelSprKill
#define fn_1_2A78 MdSelFbHook
#define fn_1_30FC MdSelFbHookSet
#define fn_1_313C MdSelFbInit
#define fn_1_318C MdSelFbKill
#define fn_1_31E4 MdSelGuideIdleWait
#define fn_1_3274 MdSelGuideIdleSet
#define fn_1_3328 MdSelGuide2IdleWait

/* parts 2-5 functions under their old fn_1_ names (from symbols.txt) */
#define fn_1_33B8 MdSelGuide2IdleSet
#define fn_1_346C MdSelFlockMove
#define fn_1_3910 MdSelFlockStart
#define fn_1_3CC0 MdSelWalkerUpdate
#define fn_1_46DC MdSelWalkerScare
#define fn_1_485C MdSelJumperIdle
#define fn_1_4CF4 MdSelJumpMove
#define fn_1_5290 MdSelJumpStart
#define fn_1_5614 MdSelSceneryInit
#define fn_1_5BEC MdSelSceneryNull
#define fn_1_5BF0 MdSelSeUpdate
#define fn_1_5EA4 MdSelSeStop
#define fn_1_5F60 MdSelSePlay
#define fn_1_6018 MdSelSeInit
#define fn_1_607C MdSelSceneryUpdate
#define fn_1_651C MdSelMicResult
#define fn_1_6C04 MdSelMicInit
#define fn_1_6DFC MdSelMicKill
#define fn_1_6E54 MdSelMicCheck
#define fn_1_6F40 MdSelStageInit
#define fn_1_702C MdSelStageKill
#define fn_1_70BC MdSelModeMdlUpdate
#define fn_1_75A4 MdSelModeMdlSelect
#define fn_1_76DC MdSelModeMdlInit
#define fn_1_77D8 MdSelModeMdlKill
#define fn_1_7868 MdSelGuideFlyUpdate
#define fn_1_7EC4 MdSelGuideFlySet
#define fn_1_809C MdSelGuide2FlyUpdate
#define fn_1_86F8 MdSelGuide2FlySet
#define fn_1_88D0 MdSelGuideInit
#define fn_1_8A58 MdSelGuideKill
#define fn_1_8AE0 MdSelGuide2Init
#define fn_1_8CA4 MdSelGuide2Kill
#define fn_1_8D44 MdSelIconDispOn
#define fn_1_8E38 MdSelIconDispOff
#define fn_1_8EC8 MdSelIconUpdate
#define fn_1_90FC MdSelCursorTargetSet
#define fn_1_9160 MdSelCursorSet
#define fn_1_927C MdSelCursorDispOff
#define fn_1_92BC MdSelCursorUpdate
#define fn_1_97D4 MdSelCursorInit
#define fn_1_98B0 MdSelCursorKill
#define fn_1_9910 MdSelSceneReset
#define fn_1_A310 MdSelProc
#define fn_1_A5D4 MdSelInit
#define fn_1_B0C8 MdSelDataClose
#define fn_1_B0E8 ObjectSetup
#define fn_1_B304 MdSelIntroStart
#define fn_1_B414 MdSelIntroMes
#define fn_1_B804 MdSelExitAsk
#define fn_1_BAB4 MdSelModeSelect
#define fn_1_E1FC MdSelExitAnim
#define fn_1_E7B0 MdSelMain
#define fn_1_ECAC MdSelEffDispSet
#define fn_1_ED60 MdSelEffOn
#define fn_1_EEC8 MdSelEffOff
#define fn_1_EF48 MdSelEffHook
#define fn_1_F4E4 MdSelEffCreate
#define fn_1_F70C MdSelEffKill
#define fn_1_F790 MdSelEffInit
#define fn_1_FA18 MdSelEffClose
#define fn_1_FA9C MdSelEffSet
#define fn_1_FDF8 MdSelSePosPlay
#define fn_1_FEC0 MdSelSeMdlPlay

/* other parts (prototypes as known to part 1) */
void MdSelSePosPlay(HuVecF *pos, s32 seId);
void MdSelSeMdlPlay(HU3D_MODELID mdlId, s32 seId, s16 range, s16 vol);

#endif
