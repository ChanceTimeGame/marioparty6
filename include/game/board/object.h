#ifndef _GAME_BOARD_OBJECT_H
#define _GAME_BOARD_OBJECT_H

#include "game/board/board.h"

#define MB_OBJ_MOT_MAX 32

typedef s16 MBOBJID;

typedef struct MbObj_s {
    /* 0x000 */ u8 attr; /* bitfield; 0x80 disp (mbObjDispGet) */
    /* 0x002 */ u16 cameraBit;
    /* 0x004 */ u8 layer; /* mbObjLayerGet */
    /* 0x005 */ u8 unk5;
    /* 0x006 */ MBOBJID objId;
    /* 0x008 */ HU3D_MODELID modelId;
    /* 0x00A */ s16 unkA;
    /* 0x00C */ s16 motId[MB_OBJ_MOT_MAX]; /* mbObjMotionIDGet */
    /* 0x04C */ s16 motNo;
    /* 0x04E */ u8 unk4E[2];
    /* 0x050 */ void *data;
    /* 0x054 */ u8 unk54[0x15C - 0x54]; /* MP5 motData[] etc.; layout unproven */
    /* 0x15C */ float unk15C;
    /* 0x160 */ float unk160;
    /* 0x164 */ HuVecF pos; /* mbObjPosGet */
    /* 0x170 */ HuVecF rot;
    /* 0x17C */ HuVecF scale;
    /* 0x188 */ HuVecF offset;
    /* 0x194 */ float unk194;
    /* 0x198 */ u8 alpha; /* mbObjAlphaGet */
    /* 0x199 */ u8 unk199[3];
} MBOBJ; /* size 0x19C (stride in objManData) */

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
int mbObjDataNumGet(int arg0);
void mbObjInit(void);
void mbObjClose(void);
s16 mbObjCreate(int arg0, void *arg1, int arg2);
int mbObjCharCreate(int arg0, int arg1, void *arg2, int arg3);
void mbObjKill(int arg0);
void mbObjDirSet(int arg0, int arg1);
MBOBJ *mbObjGet(s16 arg0);
int mbObjDispGet(s16 arg0);
void mbObjDispSet(s16 arg0, u8 arg1);
void mbObjCameraSet(s16 arg0, int arg1);
u8 mbObjLayerGet(s16 arg0);
void mbObjLayerSet(s16 arg0, int arg1);
void mbObjLayerSetAll(int arg0);
s16 mbObjModelIDGet(s16 arg0);
void mbObjPosGet(s16 arg0, HuVecF *arg1);
void mbObjPosSetV(s16 arg0, HuVecF *arg1);
void mbObjPosSet(int arg0, float f0, float f1, float f2);
void mbObjOffsetGet(s16 arg0, HuVecF *arg1);
void mbObjOffsetSetV(int arg0, HuVecF *arg1);
void mbObjOffsetSet(s16 arg0, float f0, float f1, float f2);
void mbObjRotGet(s16 arg0, HuVecF *arg1);
void mbObjRotSetV(s16 arg0, HuVecF *arg1);
void mbObjRotSet(int arg0, float f0, float f1, float f2);
float mbObjRotYGet(s16 arg0);
void mbObjRotYSet(s16 arg0, float f0);
void mbObjScaleGet(s16 arg0, HuVecF *arg1);
void mbObjScaleSetV(int arg0, HuVecF *arg1);
void mbObjScaleSet(s16 arg0, float f0, float f1, float f2);
void mbObjMtxGet(int arg0, void *arg1);
void mbObjMtxSet(s16 arg0, void *arg1);
void mbObjAttrSet(s16 arg0, int arg1);
void mbObjAttrReset(int arg0, int arg1);
void mbObjAmbSet(int arg0, float f0, float f1, float f2);
void mbObjAmbSetAll(float f0, float f1, float f2);
u8 mbObjAlphaGet(s16 arg0);
void mbObjAlphaSet(int arg0, int arg1);
void mbObjFadeSet(s16 arg0, u8 arg1);
void mbObjZWriteOffSet(int arg0, int arg1);
void mbObjCullRadiusSet(s16 arg0, float f0);
void mbObjHookSet(s16 arg0, char *arg1, int arg2);
void mbObjHookReset(int arg0);
void mbObjHookObjReset(int arg0, void *arg1);
void mbObjSePlay(int arg0, int arg1);
void mbObjMotionVoiceOnSet(int arg0, int arg1, int arg2);
s16 mbObjMotionGet(s16 arg0);
void mbObjMotionSet(s16 arg0, int arg1, int arg2);
s16 mbObjMotionIDCurGet(s16 arg0);
int mbObjMotionIDGet(s16 arg0, int arg1);
void mbObjMotionShiftSet(s16 arg0, int arg1, float f0, float f1, int arg2);
int mbObjMotionShiftIDGet(int arg0);
void mbObjMotionNoCreate(s16 arg0, int arg1, int arg2);
int mbObjMotionCreate(s16 arg0, int arg1);
void mbObjMotionKill(s16 arg0, int arg1);
float mbObjMotionTimeGet(int arg0);
void mbObjMotionTimeSet(int arg0, float f0);
float mbObjMotionMaxTimeGet(int arg0);
float mbObjMotionSpeedGet(s16 arg0);
void mbObjMotionSpeedSet(int arg0, float f0);
int mbObjMotionEndCheck(int arg0);
void mbObjMotionStartEndSet(int arg0, s16 arg1, s16 arg2);
void mbObjMotionLoopSet(int arg0, int arg1);
float mbObjMotionShapeTimeGet(s16 arg0);
void mbObjMotionShapeSet(int arg0, int arg1, int arg2);
void mbObjMotionShapeTimeSet(s16 arg0, float f0);
float mbObjMotionShapeMaxTimeGet(s16 arg0);
void mbObjMotionShapeSpeedSet(s16 arg0, float f0);
float mbObjMotionShapeSpeedGet(s16 arg0);
int mbObjMotionShapeEndCheck(s16 arg0);
void mbObjMotionShapeStartEndSet(int arg0, s16 arg1, s16 arg2);
void mbObjShadowSet(int arg0);
void mbObjShadowReset(int arg0);
void mbObjShadowMapSet(int arg0);
void mbObjShadowMapReset(int arg0);
int mbObjStub0(void);

#endif
