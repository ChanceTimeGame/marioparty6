#ifndef _GAME_BOARD_OBJECT_H
#define _GAME_BOARD_OBJECT_H

#include "game/board/board.h"

#define MB_OBJ_MOT_MAX 32

typedef s16 MBOBJID;

typedef struct MbObj_s {
    /* 0x000 */ u8 disp : 1;
    /* 0x000 */ u8 dispOld : 1;
    /* 0x000 */ u8 fadeF : 1;
    /* 0x002 */ u16 cameraBit;
    /* 0x004 */ u8 layer;
    /* 0x005 */ s8 charNo;
    /* 0x006 */ MBOBJID objId;
    /* 0x008 */ HU3D_MODELID modelId;
    /* 0x00A */ s16 motNum;
    /* 0x00C */ HU3D_MOTIONID motId[MB_OBJ_MOT_MAX];
    /* 0x04C */ s16 motNo;
    /* 0x050 */ int dataNum;
    /* 0x054 */ s16 linkCnt;
    /* 0x058 */ struct MbObj_s *linkObj;
    /* 0x05C */ void *motData[MB_OBJ_MOT_MAX];
    /* 0x0DC */ int motDataNum[MB_OBJ_MOT_MAX];
    /* 0x15C */ float motStart;
    /* 0x160 */ float motEnd;
    /* 0x164 */ HuVecF pos;
    /* 0x170 */ HuVecF rot;
    /* 0x17C */ HuVecF scale;
    /* 0x188 */ HuVecF offset;
    /* 0x194 */ float cullRadius;
    /* 0x198 */ u8 alpha;
} MBOBJ; /* size 0x19C */

#define MB_OBJ_MAX 512
#define MB_OBJ_NONE -1

int mbObjDataNumGet(int dataNum);
void mbObjInit(void);
void mbObjClose(void);
MBOBJID mbObjCreate(int dataNum, int *motDataNum, BOOL linkF);
MBOBJID mbObjCharCreate(int charNo, int dataNum, int *motDataNum, BOOL linkF);
void mbObjKill(MBOBJID objId);
void mbObjDirSet(int dirDay, int dirNight);
MBOBJ *mbObjGet(MBOBJID objId);
BOOL mbObjDispGet(MBOBJID objId);
void mbObjDispSet(MBOBJID objId, BOOL disp);
void mbObjCameraSet(MBOBJID objId, u16 cameraBit);
u8 mbObjLayerGet(MBOBJID objId);
void mbObjLayerSet(MBOBJID objId, u8 layer);
void mbObjLayerSetAll(u8 layer);
HU3D_MODELID mbObjModelIDGet(MBOBJID objId);
void mbObjPosGet(MBOBJID objId, HuVecF *pos);
void mbObjPosSetV(MBOBJID objId, HuVecF *pos);
void mbObjPosSet(MBOBJID objId, float x, float y, float z);
void mbObjOffsetGet(MBOBJID objId, HuVecF *offset);
void mbObjOffsetSetV(MBOBJID objId, HuVecF *offset);
void mbObjOffsetSet(MBOBJID objId, float x, float y, float z);
void mbObjRotGet(MBOBJID objId, HuVecF *rot);
void mbObjRotSetV(MBOBJID objId, HuVecF *rot);
void mbObjRotSet(MBOBJID objId, float x, float y, float z);
float mbObjRotYGet(MBOBJID objId);
void mbObjRotYSet(MBOBJID objId, float rotY);
void mbObjScaleGet(MBOBJID objId, HuVecF *scale);
void mbObjScaleSetV(MBOBJID objId, HuVecF *scale);
void mbObjScaleSet(MBOBJID objId, float x, float y, float z);
void mbObjMtxGet(MBOBJID objId, Mtx mtx);
void mbObjMtxSet(MBOBJID objId, Mtx mtx);
void mbObjAttrSet(MBOBJID objId, u32 attr);
void mbObjAttrReset(MBOBJID objId, u32 attr);
void mbObjAmbSet(MBOBJID objId, float r, float g, float b);
void mbObjAmbSetAll(float r, float g, float b);
u8 mbObjAlphaGet(MBOBJID objId);
void mbObjAlphaSet(MBOBJID objId, u8 alpha);
void mbObjFadeSet(MBOBJID objId, BOOL fadeF);
void mbObjZWriteOffSet(MBOBJID objId, BOOL zWriteOff);
void mbObjCullRadiusSet(MBOBJID objId, float radius);
void mbObjHookSet(MBOBJID objId, char *objName, MBOBJID hookObjId);
void mbObjHookReset(MBOBJID objId);
void mbObjHookObjReset(MBOBJID objId, char *objName);
void mbObjSePlay(MBOBJID objId, s16 seId);
void mbObjMotionVoiceOnSet(MBOBJID objId, int motNo, BOOL voiceOn);
int mbObjMotionGet(MBOBJID objId);
void mbObjMotionSet(MBOBJID objId, int motNo, u32 attr);
HU3D_MOTIONID mbObjMotionIDCurGet(MBOBJID objId);
HU3D_MOTIONID mbObjMotionIDGet(MBOBJID objId, int motNo);
void mbObjMotionShiftSet(MBOBJID objId, int motNo, float start, float end, u32 attr);
int mbObjMotionShiftIDGet(MBOBJID objId);
void mbObjMotionNoCreate(MBOBJID objId, int dataNum, int motNo);
int mbObjMotionCreate(MBOBJID objId, int dataNum);
void mbObjMotionKill(MBOBJID objId, int motNo);
float mbObjMotionTimeGet(MBOBJID objId);
void mbObjMotionTimeSet(MBOBJID objId, float time);
float mbObjMotionMaxTimeGet(MBOBJID objId);
float mbObjMotionSpeedGet(MBOBJID objId);
void mbObjMotionSpeedSet(MBOBJID objId, float speed);
BOOL mbObjMotionEndCheck(MBOBJID objId);
void mbObjMotionStartEndSet(MBOBJID objId, s16 start, s16 end);
void mbObjMotionLoopSet(MBOBJID objId, BOOL loopF);
float mbObjMotionShapeTimeGet(MBOBJID objId);
void mbObjMotionShapeSet(MBOBJID objId, int motNo, u32 attr);
void mbObjMotionShapeTimeSet(MBOBJID objId, float time);
float mbObjMotionShapeMaxTimeGet(MBOBJID objId);
void mbObjMotionShapeSpeedSet(MBOBJID objId, float speed);
float mbObjMotionShapeSpeedGet(MBOBJID objId);
BOOL mbObjMotionShapeEndCheck(MBOBJID objId);
void mbObjMotionShapeStartEndSet(MBOBJID objId, s16 start, s16 end);
void mbObjShadowSet(MBOBJID objId);
void mbObjShadowReset(MBOBJID objId);
void mbObjShadowMapSet(MBOBJID objId);
void mbObjShadowMapReset(MBOBJID objId);
int mbObjStub0(void);

#endif
