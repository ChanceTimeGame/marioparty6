#ifndef _GAME_BOARD_CAMERA_H
#define _GAME_BOARD_CAMERA_H

#include "game/board/board.h"
#include "game/flag.h"

#define MB_CAMERA_STACK_MAX 16
#define MB_CAMERA_FOCUS_MAX 8

#define MB_CAMERA_FOCUS_POS 0
#define MB_CAMERA_FOCUS_OBJ 1
#define MB_CAMERA_FOCUS_MASU 2

#define MB_CAMERA_CURVE_LINEAR 0
#define MB_CAMERA_CURVE_SIN 1
#define MB_CAMERA_CURVE_SIN2 2
#define MB_CAMERA_CURVE_COS 3
#define MB_CAMERA_CURVE_COS2 4

/* Set while a camera move (mbCameraMove*) runs. */
#define FLAG_BOARD_CAMERA_MOVE FLAGNUM(FLAG_GROUP_COMMON, 26)

typedef struct MbView_s {
    s16 rotX;
    s16 zoom;
    s16 fov;
} MBVIEW;

/* One end of a camera move; moveKey[0] is the start, moveKey[1] the goal. */
typedef struct MbCameraViewKey_s {
    /* 0x00 */ HuVecF rot;
    /* 0x0C */ HuVecF pos;
    /* 0x18 */ HuVecF ofs;
    /* 0x24 */ float zoom;
    /* 0x28 */ float fov;
} MBCAMERAVIEWKEY; /* size 0x2C */

typedef struct MbCameraFocus_s {
    /* 0x00 */ int type; /* MB_CAMERA_FOCUS_* */
    /* 0x04 */ int id; /* object or masu id */
    /* 0x08 */ HuVecF pos; /* MB_CAMERA_FOCUS_POS */
} MBCAMERAFOCUS; /* size 0x14 */

typedef struct MbCamera_s MBCAMERA;
typedef void (*MBCAMERALOOKATFUNC)(MBCAMERA *cameraP);

/* One camera's work; cameraCur and each of the 16 cameraStack entries (board/camera.c). */
struct MbCamera_s {
    /* 0x000 */ union {
        struct {
            u8 dispOn : 1; /* 0x80; in a stack entry: slot in use */
            u8 moveOn : 1; /* 0x40; ease toward the focus at speed (mbCameraMoveOnSet) */
            u8 shake : 1; /* 0x20; mbCameraShakeSet */
        };
        u8 attr;
    };
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
    /* 0x07A */ s16 time;
    /* 0x07C */ s16 maxTime;
    /* 0x080 */ MBCAMERALOOKATFUNC lookAtFunc;
    /* 0x084 */ MBCAMERAVIEWKEY moveKey[2];
    /* 0x0DC */ int focusNum;
    /* 0x0E0 */ MBCAMERAFOCUS focus[MB_CAMERA_FOCUS_MAX];
}; /* size 0x180 */

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbCameraInit(void);
void mbCameraZoomSet(float zoom);
float mbCameraZoomGet(void);
void mbCameraRotSetV(HuVecF *rot);
void mbCameraRotSet(float rotX, float rotY, float rotZ);
void mbCameraRotGet(HuVecF *rot);
void mbCameraCenterSetV(HuVecF *center);
void mbCameraCenterSet(float centerX, float centerY, float centerZ);
void mbCameraCenterGet(HuVecF *center);
void mbCameraEyeSetV(HuVecF *eye);
void mbCameraEyeSet(float eyeX, float eyeY, float eyeZ);
void mbCameraEyeGet(HuVecF *eye);
void mbCameraOffsetSetV(HuVecF *offset);
void mbCameraOffsetSet(float offsetX, float offsetY, float offsetZ);
void mbCameraOffsetGet(HuVecF *offset);
void mbCameraCurveTypeSet(int curveType);
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
void mbCameraLookAtInvGet(Mtx lookAtInv);
MBCAMERA *mbCameraGet(void);
int mbCameraStackPush(void);
void mbCameraStackIdxSet(s16 stackNo, int maxTime);
void mbCameraStackPop(int maxTime);
BOOL mbCameraCullCheck(HuVecF *pos, float radius);
void mbCameraMoveOnSet(BOOL moveOn);
void mbCameraSpeedSet(float speed);
void mbCameraFocusReset(void);
void mbCameraFocusPosSet(const HuVecF *pos);
void mbCameraFocusPosAdd(const HuVecF *pos);
void mbCameraFocusPlayerSet(int playerNo);
void mbCameraFocusPlayerAdd(int playerNo);
void mbCameraFocusObjSet(s16 objId);
void mbCameraFocusObjAdd(s16 objId);
void mbCameraFocusMasuSet(int masuId);
void mbCameraFocusMasuAdd(int masuId);
void mbCameraMovePos(const HuVecF *pos, const HuVecF *rot, const HuVecF *ofs, float zoom, float fov, s16 maxTime);
void mbCameraMovePlayer(s16 playerNo, const HuVecF *rot, const HuVecF *ofs, float zoom, float fov, s16 maxTime);
void mbCameraMoveObj(s16 objId, const HuVecF *rot, const HuVecF *ofs, float zoom, float fov, s16 maxTime);
void mbCameraMoveMasu(s16 masuId, const HuVecF *rot, const HuVecF *ofs, float zoom, float fov, s16 maxTime);
BOOL mbCameraMoveCheck(void);
void mbCameraMoveWait(void);
void mbCameraMoveStop(void);
void mbCameraShakeSet(int time, float power);
void mbCameraShakeReset(void);
void mbCameraPlayerViewSetTime(int playerNo, int viewNo, int maxTime);
void mbCameraPlayerViewSet(int playerNo, int viewNo);
void mbCameraPlayerViewSetFast(int playerNo, int viewNo);
void mbCameraPlayerViewGet(int viewNo, MBVIEW *view);
int mbCameraPlayerViewNoGet(void);
float mbCameraPlayerViewZoomGet(int viewNo);
float mbCameraPlayerViewRotXGet(int viewNo);
void mbCameraMultiFocusSet(void);

#endif
