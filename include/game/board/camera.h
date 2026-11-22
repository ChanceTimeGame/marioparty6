#ifndef _GAME_BOARD_CAMERA_H
#define _GAME_BOARD_CAMERA_H

#include "game/board/board.h"

/* One camera's work; cameraCur and each of the 16 cameraStack entries (board/camera.c). */
typedef struct MbCameraViewKey_s {
    /* 0x00 */ HuVecF rot;
    /* 0x0C */ HuVecF pos;
    /* 0x18 */ HuVecF ofs;
    /* 0x24 */ float zoom;
    /* 0x28 */ float fov;
} MBCAMERAVIEWKEY; /* size 0x2C; copied as 9 words + 2 floats; member names after MP5, order unproven */

typedef struct MbCamera_s MBCAMERA;
typedef void (*MBCAMERALOOKATFUNC)(MBCAMERA *cameraP); /* argument unproven */

struct MbCamera_s {
    /* 0x000 */ u8 attr; /* bit 0x40 moveOn (mbCameraMoveOnSet), 0x20 shake (mbCameraShakeSet) */
    /* 0x002 */ u16 bit;
    /* 0x004 */ float fov;
    /* 0x008 */ float near;
    /* 0x00C */ float far;
    /* 0x010 */ float aspect;
    /* 0x014 */ float viewportX;
    /* 0x018 */ float viewportY;
    /* 0x01C */ float viewportW;
    /* 0x020 */ float viewportH;
    /* 0x024 */ float viewportNear;
    /* 0x028 */ float viewportFar;
    /* 0x02C */ HuVecF pos;
    /* 0x038 */ HuVecF up;
    /* 0x044 */ HuVecF target;
    /* 0x050 */ HuVecF offset;
    /* 0x05C */ HuVecF rot;
    /* 0x068 */ float zoom;
    /* 0x06C */ float speed;
    /* 0x070 */ int shakeTime;
    /* 0x074 */ float shakePower;
    /* 0x078 */ s16 curveType;
    /* 0x07A */ s16 unk7A;
    /* 0x07C */ s16 unk7C;
    /* 0x07E */ u8 unk7E[2];
    /* 0x080 */ MBCAMERALOOKATFUNC lookAtFunc;
    /* 0x084 */ MBCAMERAVIEWKEY moveKey[2];
    /* 0x0DC */ int focusNum;
    /* 0x0E0 */ u32 unkE0[(0x180 - 0xE0) / 4]; /* focus list (MP5: {type, targetId}[]); layout unproven */
}; /* size 0x180 (cameraCur; cameraStack is 0x1800 = 16 entries) */

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbCameraInit(void);
void mbCameraZoomSet(float zoom);
float mbCameraZoomGet(void);
void mbCameraRotSetV(HuVecF *rot);
void mbCameraRotSet(float rotX, float rotY, float rotZ);
void mbCameraRotGet(HuVecF *rot);
void mbCameraCenterSetV(HuVecF *arg0);
void mbCameraCenterSet(float f0, float f1, float f2);
void mbCameraCenterGet(HuVecF *arg0);
void mbCameraEyeSetV(HuVecF *arg0);
void mbCameraEyeSet(float f0, float f1, float f2);
void mbCameraEyeGet(HuVecF *arg0);
void mbCameraOffsetSetV(HuVecF *offset);
void mbCameraOffsetSet(float offsetX, float offsetY, float offsetZ);
void mbCameraOffsetGet(HuVecF *offset);
void mbCameraCurveTypeSet(int arg0);
s16 mbCameraCurveTypeGet(void);
void mbCameraDirGet(HuVecF *dir);
void mbCameraPosDirGet(HuVecF *pos, HuVecF *dir);
void mbCameraLookAtFuncSet(MBCAMERALOOKATFUNC func);
void mbCameraBitSet(u16 bit);
void mbCameraFovSet(float fov);
void mbCameraScissorSet(int x, int y, int w, int h);
void mbCameraNearFarSet(float near, float far);
void mbCameraNearFarGet(float *near, float *far);
void mbCameraLookAtGet(Mtx lookAt);
u32 mbCameraLookAtInvGet(void *arg0);
MBCAMERA *mbCameraGet(void);
int mbCameraStackPush(void);
void mbCameraStackIdxSet(int arg0, s16 arg1);
void mbCameraStackPop(s16 arg0);
BOOL mbCameraCullCheck(HuVecF *pos, float radius);
void mbCameraMoveOnSet(u8 arg0);
void mbCameraSpeedSet(float speed);
void mbCameraFocusReset(void);
void mbCameraFocusPosSet(HuVecF *arg0);
void mbCameraFocusPosAdd(HuVecF *arg0);
void mbCameraFocusPlayerSet(int playerNo);
void mbCameraFocusPlayerAdd(int playerNo);
void mbCameraFocusObjSet(s16 arg0);
void mbCameraFocusObjAdd(int arg0);
void mbCameraFocusMasuSet(int masuId);
void mbCameraFocusMasuAdd(int masuId);
void mbCameraMovePos(HuVecF *arg0, HuVecF *arg1, HuVecF *arg2, s16 arg3, float f0, float f1);
void mbCameraMovePlayer(int arg0, HuVecF *arg1, HuVecF *arg2, int arg3, float f0, float f1);
void mbCameraMoveObj(int arg0, HuVecF *arg1, HuVecF *arg2, s16 arg3, float f0, float f1);
void mbCameraMoveMasu(int arg0, HuVecF *arg1, HuVecF *arg2, s16 arg3, float f0, float f1);
int mbCameraMoveCheck(void);
void mbCameraMoveWait(void);
void mbCameraMoveStop(void);
void mbCameraShakeSet(int arg0, float f0);
void mbCameraShakeReset(void);
void mbCameraPlayerViewSetTime(s16 arg0, int arg1, s16 arg2);
void mbCameraPlayerViewSet(int arg0, int arg1);
void mbCameraPlayerViewSetFast(int arg0, int arg1);
void mbCameraPlayerViewGet(int arg0, void *arg1);
int mbCameraPlayerViewNoGet(void);
float mbCameraPlayerViewZoomGet(int arg0);
float mbCameraPlayerViewRotXGet(int arg0);
void mbCameraMultiFocusSet(void);

#endif
