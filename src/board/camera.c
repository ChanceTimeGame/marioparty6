#include "game/board/camera.h"
#include "game/board/exit.h"
#include "game/board/math.h"
#include "game/board/masu.h"
#include "game/board/object.h"
#include "game/board/player.h"
#include "game/audio.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "math.h"
#include "string.h"

typedef struct ListenerParam_s {
    float sndDist;
    float sndSpeed;
    float startDis;
    float frontSurDis;
    float backSurDis;
} LISTENERPARAM;

typedef struct BoardListenerParam_s {
    int boardNo;
    LISTENERPARAM param;
} BOARDLISTENERPARAM;

/* MP5 MBBoardNoGet; belongs in board.h once another unit needs it */
static inline int BoardNoGet(void)
{
    return GwSystem.boardNo;
}

static MBVIEW viewData[] = {
    { -35, 1600, 30 },
    { -35, 2000, 30 },
    { -35, 2400, 30 },
};

static MBCAMERA cameraStack[MB_CAMERA_STACK_MAX];
static MBCAMERA cameraCur;

static OMOBJ *cameraOMObj;
static s16 lightTypeBackup[2];
static s16 cameraStackLevel;

static void CameraOMExec(OMOBJ *obj);
static void CameraLookAt(MBCAMERA *cameraP);
static void Camera0LayerHook(s16 layerNo);
static void Camera1LayerHook(s16 layerNo);
static void Camera2LayerHook(s16 layerNo);

void mbCameraInit(void)
{
    MBCAMERA *cameraP = mbCameraGet();
    BOARDLISTENERPARAM listenerParamTbl[11] = {
        { 0, { 8000, 1000, 0, 4000, 4000 } },
        { 1, { 8000, 1000, 0, 4000, 4000 } },
        { 2, { 8000, 1000, 0, 4000, 4000 } },
        { 3, { 8000, 1000, 0, 4000, 4000 } },
        { 4, { 8000, 1000, 0, 4000, 4000 } },
        { 5, { 8000, 1000, 0, 4000, 4000 } },
        { 6, { 8000, 1000, 0, 4000, 4000 } },
        { 7, { 8000, 1000, 0, 4000, 4000 } },
        { 8, { 8000, 1000, 0, 4000, 4000 } },
        { 9, { 8000, 1000, 0, 4000, 4000 } },
        { 10, { 8000, 1000, 0, 4000, 4000 } },
    };
    LISTENERPARAM *listenerParam;
    HU3D_CAMERA *hu3dCamP;
    int i;
    HuVecF pos;
    HuVecF dir;
    memset(&cameraStack[0], 0, sizeof(cameraStack));
    memset(&cameraCur, 0, sizeof(cameraCur));
    cameraP->attr = 0;
    cameraP->fov = 30;
    cameraP->near = 100;
    cameraP->far = 20000;
    cameraP->aspect = 1.2f;
    cameraP->viewportX = 0;
    cameraP->viewportY = 0;
    cameraP->viewportH = 480;
    cameraP->viewportW = 640;
    cameraP->viewportNear = 0;
    cameraP->viewportFar = 1;
    cameraP->up.x = cameraP->up.z = 0;
    cameraP->up.y = 1;
    cameraP->shakeTime = 0;
    cameraP->pos.x = cameraP->pos.y = cameraP->pos.z = 0;
    cameraP->target.x = cameraP->target.y = cameraP->target.z = 0;
    cameraP->offset.x = cameraP->offset.y = cameraP->offset.z = 0;
    cameraP->lookAtFunc = CameraLookAt;
    cameraP->zoom = viewData[2].zoom;
    cameraP->rot.x = viewData[2].rotX;
    cameraP->rot.y = cameraP->rot.z = 0;
    cameraP->bit = HU3D_CAM0 | HU3D_CAM1;
    cameraP->speed = 0.15f;
    cameraP->focusNum = 0;
    memset(&cameraP->focus[0], 0, MB_CAMERA_FOCUS_MAX * sizeof(MBCAMERAFOCUS));
    cameraP->curveType = MB_CAMERA_CURVE_LINEAR;
    cameraP->time = 0;
    cameraP->maxTime = -1;
    Hu3DNoSyncSet(FALSE);
    Hu3DCameraCreate(cameraP->bit | HU3D_CAM2);
    hu3dCamP = &Hu3DCamera[2];
    hu3dCamP->pos.x = hu3dCamP->pos.y = hu3dCamP->pos.z = 0;
    hu3dCamP->target.z = -100;
    Hu3DCameraScissorSet(cameraP->bit, cameraP->viewportX, cameraP->viewportY, cameraP->viewportW, cameraP->viewportH);
    cameraP->target.x = cameraP->target.y = cameraP->target.z = 0;
    for(i = 0; i < 11; i++) {
        if(listenerParamTbl[i].boardNo == BoardNoGet()) {
            break;
        }
    }
    if(i >= 11) {
        i = 0;
    }
    listenerParam = &listenerParamTbl[i].param;
    pos.x = pos.y = 0;
    pos.z = 100000;
    dir.x = 0;
    dir.y = 0;
    dir.z = -1;
    VECNormalize(&dir, &dir);
    HuAudFXListnerSetEX(&pos, &dir, listenerParam->sndDist, listenerParam->sndSpeed, listenerParam->startDis, listenerParam->frontSurDis, listenerParam->backSurDis);
    cameraOMObj = omAddObjEx(mbObjMan, 32256, 0, 0, -1, CameraOMExec);
    Hu3DCameraLayerHookSet(HU3D_CAM0, 0, Camera0LayerHook);
    Hu3DCameraLayerHookSet(HU3D_CAM1, 0, Camera1LayerHook);
    Hu3DCameraLayerHookSet(HU3D_CAM2, 0, Camera2LayerHook);
    omSetStatBit(cameraOMObj, OM_STAT_NOPAUSE);
    lightTypeBackup[0] = lightTypeBackup[1] = -1;
}

static void Camera0LayerHook(s16 layerNo)
{
    HU3D_LIGHT *lightP = &Hu3DGlobalLight[0];
    if(lightTypeBackup[0] != -1) {
        lightP->type = lightTypeBackup[0];
    }
    lightP = &Hu3DGlobalLight[1];
    if(lightP->type != -1) {
        lightTypeBackup[1] = lightP->type;
        lightP->type = -1;
    }
}

static void Camera1LayerHook(s16 layerNo)
{
    HU3D_LIGHT *lightP = &Hu3DGlobalLight[0];
    if(lightP->type != -1) {
        lightTypeBackup[0] = lightP->type;
        lightP->type = -1;
    }
    lightP = &Hu3DGlobalLight[1];
    if(lightTypeBackup[1] != -1) {
        lightP->type = lightTypeBackup[1];
    }
    Hu3DZClear();
}

static void Camera2LayerHook(s16 layerNo)
{
    Hu3DZClear();
}

static void CameraMotionMain(MBCAMERA *cameraP);

static void CameraOMExec(OMOBJ *obj)
{
    MBCAMERA *cameraP = mbCameraGet();
    HuVecF dir;
    if(mbExitCheck()) {
        omDelObjEx((OMOBJMAN *)HuPrcCurrentGet(), obj);
        return;
    }
    if(mbPauseEnableCheck()) {
        return;
    }
    Hu3DCameraPerspectiveSet(cameraP->bit, cameraP->fov, cameraP->near, cameraP->far, cameraP->aspect);
    Hu3DCameraViewportSet(cameraP->bit, cameraP->viewportX, cameraP->viewportY, cameraP->viewportW, cameraP->viewportH, cameraP->viewportNear, cameraP->viewportFar);
    CameraMotionMain(cameraP);
    cameraP->lookAtFunc(cameraP);
    cameraP->dispOn = TRUE;
    Hu3DCameraPosSetV(cameraP->bit, &cameraP->pos, &cameraP->up, &cameraP->target);
    VECSubtract(&cameraP->target, &cameraP->pos, &dir);
    VECNormalize(&dir, &dir);
    HuAudFXListnerUpdate(&cameraP->pos, &dir);
}

static float CameraCanterCalc(HuVecF *rot, float fov, HuVecF *target);
static void CameraMotionSet(MBCAMERA *cameraP);

static void CameraMotionMain(MBCAMERA *cameraP)
{
    HuVecF pos;
    float zoom;
    float speed;
    if(_CheckFlag(FLAG_BOARD_CAMERA_MOVE)) {
        CameraMotionSet(cameraP);
        return;
    }
    if(cameraP->focusNum == 0) {
        return;
    }
    zoom = CameraCanterCalc(&cameraP->rot, cameraP->fov, &pos);
    VECAdd(&pos, &cameraP->offset, &pos);
    speed = (cameraP->moveOn) ? cameraP->speed : 1.0f;
    cameraP->target.x += speed * (pos.x - cameraP->target.x);
    cameraP->target.y += speed * (pos.y - cameraP->target.y);
    cameraP->target.z += speed * (pos.z - cameraP->target.z);
    if(zoom < 0.0f) {
        return;
    }
    if(cameraP->moveOn) {
        cameraP->zoom += 0.2f * (zoom - cameraP->zoom);
    } else {
        cameraP->zoom = zoom;
    }
}

static BOOL CameraFocusCenterCalc(HuVecF *center, HuVecF *focusPos)
{
    MBCAMERA *cameraP = mbCameraGet();
    HuVecF min;
    HuVecF max;
    HuVecF pos;
    int i;
    if(cameraP->focusNum == 0) {
        return FALSE;
    }
    for(i = 0; i < cameraP->focusNum; i++) {
        switch(cameraP->focus[i].type) {
            case MB_CAMERA_FOCUS_POS:
                pos = cameraP->focus[i].pos;
                break;

            case MB_CAMERA_FOCUS_OBJ:
                mbObjPosGet(cameraP->focus[i].id, &pos);
                break;

            case MB_CAMERA_FOCUS_MASU:
                mbMasuPosGet(cameraP->focus[i].id, &pos);
                break;
        }
        if(i == 0) {
            min = max = pos;
        } else {
            if(pos.x < min.x) {
                min.x = pos.x;
            } else if(pos.x > max.x) {
                max.x = pos.x;
            }
            if(pos.y < min.y) {
                min.y = pos.y;
            } else if(pos.y > max.y) {
                max.y = pos.y;
            }
            if(pos.z < min.z) {
                min.z = pos.z;
            } else if(pos.z > max.z) {
                max.z = pos.z;
            }
        }
        if(focusPos) {
            focusPos[i] = pos;
        }
    }
    center->x = min.x + (0.5f * (max.x - min.x));
    center->y = min.y + (0.5f * (max.y - min.y));
    center->z = min.z + (0.5f * (max.z - min.z));
    return TRUE;
}

static float CameraCanterCalc(HuVecF *rot, float fov, HuVecF *target)
{
    MBCAMERA *cameraP = mbCameraGet();
    int i, j;
    HuVecF focusPos[MB_CAMERA_FOCUS_MAX];
    HuVecF dir[2];
    HuVecF center;
    HuVecF ofs2;
    HuVecF ofs1;
    HuVecF rot2;
    HuVecF rot1;
    HuVecF aim;
    float targetDist[2];
    float len;
    float zoomMax;
    float zoom;
    float weight;
    if(rot == NULL) {
        rot = &cameraP->rot;
    }
    if(fov < 0.0f) {
        fov = cameraP->fov;
    }
    if(CameraFocusCenterCalc(&center, focusPos)) {
        *target = center;
    }
    if(cameraP->focusNum < 2) {
        return -1;
    }
    rot2.x = HuSin(rot->y) * HuSin(rot->x);
    rot2.y = HuCos(rot->x);
    rot2.z = HuCos(rot->y) * HuSin(rot->x);
    rot1.x = -HuSin(rot->y) * HuCos(rot->x);
    rot1.y = HuSin(rot->x);
    rot1.z = -HuCos(rot->y) * HuCos(rot->x);
    VECCrossProduct(&rot2, &rot1, &dir[0]);
    VECNormalize(&dir[0], &dir[0]);
    VECCrossProduct(&dir[0], &rot1, &dir[1]);
    VECNormalize(&dir[1], &dir[1]);
    for(i = 0; i < 2; i++) {
        targetDist[i] = -((target->x * dir[i].x) + (target->y * dir[i].y) + (target->z * dir[i].z));
    }
    zoomMax = 0;
    for(i = 0; i < cameraP->focusNum; i++) {
        for(j = 0; j < 2; j++) {
            len = ((dir[j].x * focusPos[i].x) + (dir[j].y * focusPos[i].y) + (dir[j].z * focusPos[i].z)) + targetDist[j];
            ofs2.x = focusPos[i].x - (dir[j].x * len);
            ofs2.y = focusPos[i].y - (dir[j].y * len);
            ofs2.z = focusPos[i].z - (dir[j].z * len);
            weight = ((ofs2.z * rot1.z) + (((ofs2.y * rot1.y) + ((ofs2.x * rot1.x) - (target->x * rot1.x))) - (target->y * rot1.y))) - (target->z * rot1.z);
            ofs1.x = target->x + (weight * rot1.x);
            ofs1.y = target->y + (weight * rot1.y);
            ofs1.z = target->z + (weight * rot1.z);
            VECSubtract(&ofs2, &ofs1, &aim);
            len = VECMag(&aim);
            if(j != 0) {
                len /= cameraP->aspect;
            }
            zoom = len / (HuSin(fov * 0.5f) / HuCos(fov * 0.5f));
            zoom += -weight;
            if(zoom > zoomMax) {
                zoomMax = zoom;
            }
        }
    }
    zoomMax *= 1.3f;
    if(zoomMax < viewData[2].zoom) {
        zoomMax = viewData[2].zoom;
    }
    return zoomMax;
}

static void CameraMotionSet(MBCAMERA *cameraP)
{
    MBCAMERAVIEWKEY *key0 = &cameraP->moveKey[0];
    MBCAMERAVIEWKEY *key1 = &cameraP->moveKey[1];
    HuVecF pos;
    float t;
    float weight;
    if(++cameraP->time >= cameraP->maxTime) {
        _ClearFlag(FLAG_BOARD_CAMERA_MOVE);
    }
    t = (float)cameraP->time / (float)cameraP->maxTime;
    switch(cameraP->curveType) {
        case MB_CAMERA_CURVE_LINEAR:
            weight = t;
            break;

        case MB_CAMERA_CURVE_SIN:
            weight = HuSin(90.0f * t);
            break;

        case MB_CAMERA_CURVE_SIN2:
            weight = HuSin(90.0f * t);
            weight *= weight;
            break;

        case MB_CAMERA_CURVE_COS:
            weight = 1.0 - HuCos(90.0f * t);
            break;

        case MB_CAMERA_CURVE_COS2:
            weight = 1.0 - HuCos(90.0f * t);
            weight *= weight;
            break;
    }
    if(!CameraFocusCenterCalc(&pos, NULL)) {
        pos = key1->pos;
    }
    cameraP->offset.x = key0->ofs.x + (weight * (key1->ofs.x - key0->ofs.x));
    cameraP->offset.y = key0->ofs.y + (weight * (key1->ofs.y - key0->ofs.y));
    cameraP->offset.z = key0->ofs.z + (weight * (key1->ofs.z - key0->ofs.z));
    VECAdd(&pos, &cameraP->offset, &pos);
    cameraP->fov = key0->fov + (weight * (key1->fov - key0->fov));
    cameraP->zoom = key0->zoom + (weight * (key1->zoom - key0->zoom));
    cameraP->rot.x = mbAngleLerp(key0->rot.x, key1->rot.x, weight);
    cameraP->rot.y = mbAngleLerp(key0->rot.y, key1->rot.y, weight);
    cameraP->rot.z = mbAngleLerp(key0->rot.z, key1->rot.z, weight);
    cameraP->target.x = key0->pos.x + (weight * (pos.x - key0->pos.x));
    cameraP->target.y = key0->pos.y + (weight * (pos.y - key0->pos.y));
    cameraP->target.z = key0->pos.z + (weight * (pos.z - key0->pos.z));
}

static void CameraLookAt(MBCAMERA *cameraP)
{
    cameraP->pos.x = cameraP->target.x + (cameraP->zoom * (HuSin(cameraP->rot.y) * HuCos(cameraP->rot.x)));
    cameraP->pos.y = cameraP->target.y + (cameraP->zoom * -HuSin(cameraP->rot.x));
    cameraP->pos.z = cameraP->target.z + (cameraP->zoom * (HuCos(cameraP->rot.y) * HuCos(cameraP->rot.x)));
    cameraP->up.x = HuSin(cameraP->rot.y) * HuSin(cameraP->rot.x);
    cameraP->up.y = HuCos(cameraP->rot.x);
    cameraP->up.z = HuCos(cameraP->rot.y) * HuSin(cameraP->rot.x);
    if(cameraP->shake) {
        cameraP->pos.x += cameraP->shakePower * (frandf() - 0.5f);
        cameraP->pos.y += cameraP->shakePower * (frandf() - 0.5f);
        cameraP->pos.z += cameraP->shakePower * (frandf() - 0.5f);
        if(--cameraP->shakeTime == 0) {
            cameraP->shake = FALSE;
            cameraP->shakeTime = 0;
            cameraP->shakePower = 0;
        }
    }
}

void mbCameraZoomSet(float zoom)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->zoom = zoom;
}

float mbCameraZoomGet(void)
{
    MBCAMERA *cameraP = mbCameraGet();
    return cameraP->zoom;
}

void mbCameraRotSetV(HuVecF *rot)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->rot.x = rot->x;
    cameraP->rot.y = rot->y;
}

void mbCameraRotSet(float rotX, float rotY, float rotZ)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->rot.x = rotX;
    cameraP->rot.y = rotY;
}

void mbCameraRotGet(HuVecF *rot)
{
    MBCAMERA *cameraP = mbCameraGet();
    *rot = cameraP->rot;
}

void mbCameraCenterSetV(HuVecF *center)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->focusNum = 0;
    cameraP->target.x = center->x;
    cameraP->target.y = center->y;
    cameraP->target.z = center->z;
}

void mbCameraCenterSet(float centerX, float centerY, float centerZ)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->focusNum = 0;
    cameraP->target.x = centerX;
    cameraP->target.y = centerY;
    cameraP->target.z = centerZ;
}

void mbCameraCenterGet(HuVecF *center)
{
    MBCAMERA *cameraP = mbCameraGet();
    *center = cameraP->target;
}

void mbCameraEyeSetV(HuVecF *eye)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->pos.x = eye->x;
    cameraP->pos.y = eye->y;
    cameraP->pos.z = eye->z;
}

void mbCameraEyeSet(float eyeX, float eyeY, float eyeZ)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->pos.x = eyeX;
    cameraP->pos.y = eyeY;
    cameraP->pos.z = eyeZ;
}

void mbCameraEyeGet(HuVecF *eye)
{
    MBCAMERA *cameraP = mbCameraGet();
    *eye = cameraP->pos;
}

void mbCameraOffsetSetV(HuVecF *offset)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->offset = *offset;
}

void mbCameraOffsetSet(float offsetX, float offsetY, float offsetZ)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->offset.x = offsetX;
    cameraP->offset.y = offsetY;
    cameraP->offset.z = offsetZ;
}

void mbCameraOffsetGet(HuVecF *offset)
{
    MBCAMERA *cameraP = mbCameraGet();
    *offset = cameraP->offset;
}

void mbCameraCurveTypeSet(int curveType)
{
    MBCAMERA *cameraP = mbCameraGet();
    if(curveType < 0) {
        cameraP->curveType = MB_CAMERA_CURVE_LINEAR;
    } else {
        cameraP->curveType = curveType;
    }
}

s16 mbCameraCurveTypeGet(void)
{
    MBCAMERA *cameraP = mbCameraGet();
    return cameraP->curveType;
}

void mbCameraDirGet(HuVecF *dir)
{
    MBCAMERA *cameraP = mbCameraGet();
    VECSubtract(&cameraP->target, &cameraP->pos, dir);
    if(dir->x != 0.0f || dir->y != 0.0f || dir->z != 0.0f) {
        VECNormalize(dir, dir);
    }
}

void mbCameraPosDirGet(HuVecF *pos, HuVecF *dir)
{
    MBCAMERA *cameraP = mbCameraGet();
    VECSubtract(pos, &cameraP->pos, dir);
    if(dir->x != 0.0f || dir->y != 0.0f || dir->z != 0.0f) {
        VECNormalize(dir, dir);
    }
}

void mbCameraLookAtFuncSet(MBCAMERALOOKATFUNC func)
{
    MBCAMERA *cameraP = mbCameraGet();
    if(func != NULL) {
        cameraP->lookAtFunc = func;
    } else {
        cameraP->lookAtFunc = CameraLookAt;
    }
}

void mbCameraBitSet(u16 bit)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->bit = bit;
}

void mbCameraFovSet(float fov)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->fov = fov;
}

void mbCameraScissorSet(int x, int y, int w, int h)
{
    MBCAMERA *cameraP = mbCameraGet();
    Hu3DCameraScissorSet(cameraP->bit, x, y, w, h);
}

void mbCameraNearFarSet(float near, float far)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->near = near;
    cameraP->far = far;
}

void mbCameraNearFarGet(float *near, float *far)
{
    MBCAMERA *cameraP = mbCameraGet();
    if(near) {
        *near = cameraP->near;
    }
    if(far) {
        *far = cameraP->far;
    }
}

void mbCameraLookAtGet(Mtx lookAt)
{
    MBCAMERA *cameraP = mbCameraGet();
    MTXLookAt(lookAt, &cameraP->pos, &cameraP->up, &cameraP->target);
}

void mbCameraLookAtInvGet(Mtx lookAtInv)
{
    MBCAMERA *cameraP = mbCameraGet();
    Mtx lookAt;
    MTXLookAt(lookAt, &cameraP->pos, &cameraP->up, &cameraP->target);
    MTXInverse(lookAt, lookAtInv);
}

MBCAMERA *mbCameraGet(void)
{
    return &cameraCur;
}

int mbCameraStackPush(void)
{
    int i;
    for(i = 0; i < MB_CAMERA_STACK_MAX; i++) {
        if(!cameraStack[i].dispOn) {
            break;
        }
    }
    memcpy(&cameraStack[i], &cameraCur, sizeof(MBCAMERA));
    cameraStackLevel = i + 1;
    return i + 1;
}

static void CameraMoveApply(const HuVecF *pos, const HuVecF *rot, const HuVecF *ofs, float zoom, float fov, s16 maxTime);

void mbCameraStackIdxSet(s16 stackNo, int maxTime)
{
    MBCAMERA *stackP = &cameraStack[stackNo - 1];
    int i;
    stackP->dispOn = FALSE;
    if(stackP->focusNum == 0) {
        CameraMoveApply(&stackP->target, &stackP->rot, &stackP->offset, stackP->zoom, stackP->fov, maxTime);
    } else {
        CameraMoveApply(NULL, &stackP->rot, &stackP->offset, stackP->zoom, stackP->fov, maxTime);
    }
    cameraCur.focusNum = stackP->focusNum;
    for(i = 0; i < cameraCur.focusNum; i++) {
        cameraCur.focus[i] = stackP->focus[i];
    }
}

void mbCameraStackPop(int maxTime)
{
    mbCameraStackIdxSet(cameraStackLevel, maxTime);
}

BOOL mbCameraCullCheck(HuVecF *pos, float radius)
{
    MBCAMERA *cameraP = mbCameraGet();
    Mtx lookAt;
    HuVecF viewPos;
    float tanHalf;
    float w;
    float h;
    float r;
    if(!cameraP->dispOn) {
        return FALSE;
    }
    MTXLookAt(lookAt, &cameraP->pos, &cameraP->up, &cameraP->target);
    MTXMultVec(lookAt, pos, &viewPos);
    tanHalf = HuSin(0.5f * cameraP->fov) / HuCos(0.5f * cameraP->fov);
    w = 1.2f * (tanHalf * -viewPos.z);
    h = tanHalf * -viewPos.z;
    r = radius / HuCos(0.5f * cameraP->fov);
    if(fabs(viewPos.x) - r < w && fabs(viewPos.y) - r < h && viewPos.z - radius <= 0.0f) {
        return TRUE;
    }
    return FALSE;
}

void mbCameraMoveOnSet(BOOL moveOn)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->moveOn = moveOn;
}

void mbCameraSpeedSet(float speed)
{
    MBCAMERA *cameraP = mbCameraGet();
    if(speed < 0.0f) {
        cameraP->speed = 0.15f;
    } else {
        cameraP->speed = speed;
    }
}

void mbCameraFocusReset(void)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->focusNum = 0;
}

void mbCameraFocusPosSet(const HuVecF *pos)
{
    MBCAMERA *cameraP = mbCameraGet();
    mbCameraFocusReset();
    mbCameraFocusPosAdd(pos);
}

void mbCameraFocusPosAdd(const HuVecF *pos)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->focus[cameraP->focusNum].type = MB_CAMERA_FOCUS_POS;
    cameraP->focus[cameraP->focusNum].pos = *pos;
    cameraP->focusNum++;
    if(!_CheckFlag(FLAG_BOARD_CAMERA_MOVE)) {
        mbCameraOffsetSet(0, 0, 0);
    }
    mbCameraMoveOnSet(TRUE);
    mbCameraSpeedSet(-1);
}

void mbCameraFocusPlayerSet(int playerNo)
{
    MBCAMERA *cameraP = mbCameraGet();
    mbCameraFocusReset();
    if(playerNo >= 0) {
        mbCameraFocusPlayerAdd(playerNo);
    }
}

void mbCameraFocusPlayerAdd(int playerNo)
{
    mbCameraFocusObjAdd(mbPlayerObjIDGet(playerNo));
    if(!_CheckFlag(FLAG_BOARD_CAMERA_MOVE)) {
        mbCameraOffsetSet(0, 100, 0);
    }
    mbCameraMoveOnSet(TRUE);
    mbCameraSpeedSet(-1);
}

void mbCameraFocusObjSet(s16 objId)
{
    MBCAMERA *cameraP = mbCameraGet();
    mbCameraFocusReset();
    if(objId >= 0) {
        mbCameraFocusObjAdd(objId);
    }
}

void mbCameraFocusObjAdd(s16 objId)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->focus[cameraP->focusNum].type = MB_CAMERA_FOCUS_OBJ;
    cameraP->focus[cameraP->focusNum].id = objId;
    cameraP->focusNum++;
    if(!_CheckFlag(FLAG_BOARD_CAMERA_MOVE)) {
        mbCameraOffsetSet(0, 0, 0);
    }
    mbCameraMoveOnSet(TRUE);
    mbCameraSpeedSet(-1);
}

void mbCameraFocusMasuSet(int masuId)
{
    MBCAMERA *cameraP = mbCameraGet();
    mbCameraFocusReset();
    if(masuId >= 0) {
        mbCameraFocusMasuAdd(masuId);
    }
}

void mbCameraFocusMasuAdd(int masuId)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->focus[cameraP->focusNum].type = MB_CAMERA_FOCUS_MASU;
    cameraP->focus[cameraP->focusNum].id = masuId;
    cameraP->focusNum++;
    if(!_CheckFlag(FLAG_BOARD_CAMERA_MOVE)) {
        mbCameraOffsetSet(0, 0, 0);
    }
    mbCameraMoveOnSet(TRUE);
    mbCameraSpeedSet(-1);
}

static void CameraMoveApply(const HuVecF *pos, const HuVecF *rot, const HuVecF *ofs, float zoom, float fov, s16 maxTime)
{
    MBCAMERA *cameraP = mbCameraGet();
    MBCAMERAVIEWKEY *key0 = &cameraP->moveKey[0];
    MBCAMERAVIEWKEY *key1 = &cameraP->moveKey[1];
    mbCameraCurveTypeSet(MB_CAMERA_CURVE_LINEAR);
    key0->fov = cameraP->fov;
    key0->zoom = cameraP->zoom;
    key0->rot = cameraP->rot;
    key0->pos = cameraP->target;
    key0->ofs = cameraP->offset;
    key1->fov = (fov < 0.0f) ? key0->fov : fov;
    key1->zoom = (zoom < 0.0f) ? key0->zoom : zoom;
    key1->rot = (!rot) ? key0->rot : *rot;
    key1->pos = (!pos) ? key0->pos : *pos;
    key1->ofs = (!ofs) ? key0->ofs : *ofs;
    _SetFlag(FLAG_BOARD_CAMERA_MOVE);
    cameraP->time = 0;
    if(maxTime < 1) {
        maxTime = 1;
    }
    cameraP->maxTime = maxTime;
}

void mbCameraMovePos(const HuVecF *pos, const HuVecF *rot, const HuVecF *ofs, float zoom, float fov, s16 maxTime)
{
    HuVecF ofsTemp;
    if(!ofs) {
        ofsTemp.x = ofsTemp.y = ofsTemp.z = 0;
    } else {
        ofsTemp = *ofs;
    }
    CameraMoveApply(pos, rot, &ofsTemp, zoom, fov, maxTime);
    if(pos) {
        mbCameraFocusPosSet(pos);
    }
}

void mbCameraMovePlayer(s16 playerNo, const HuVecF *rot, const HuVecF *ofs, float zoom, float fov, s16 maxTime)
{
    if(playerNo == -1) {
        CameraMoveApply(NULL, rot, ofs, zoom, fov, maxTime);
    } else {
        HuVecF pos;
        HuVecF ofsTemp;
        mbPlayerPosGet(playerNo, &pos);
        if(!ofs) {
            ofsTemp.x = ofsTemp.z = 0;
            ofsTemp.y = 100;
        } else {
            ofsTemp = *ofs;
        }
        CameraMoveApply(&pos, rot, &ofsTemp, zoom, fov, maxTime);
        mbCameraFocusPlayerSet(playerNo);
    }
}

void mbCameraMoveObj(s16 objId, const HuVecF *rot, const HuVecF *ofs, float zoom, float fov, s16 maxTime)
{
    if(objId == -1) {
        CameraMoveApply(NULL, rot, ofs, zoom, fov, maxTime);
    } else {
        HuVecF pos;
        HuVecF ofsTemp;
        mbObjPosGet(objId, &pos);
        if(!ofs) {
            ofsTemp.x = ofsTemp.y = ofsTemp.z = 0;
        } else {
            ofsTemp = *ofs;
        }
        CameraMoveApply(&pos, rot, &ofsTemp, zoom, fov, maxTime);
        mbCameraFocusObjSet(objId);
    }
}

void mbCameraMoveMasu(s16 masuId, const HuVecF *rot, const HuVecF *ofs, float zoom, float fov, s16 maxTime)
{
    if(masuId == -1) {
        CameraMoveApply(NULL, rot, ofs, zoom, fov, maxTime);
    } else {
        HuVecF pos;
        HuVecF ofsTemp;
        mbMasuPosGet(masuId, &pos);
        if(!ofs) {
            ofsTemp.x = ofsTemp.y = ofsTemp.z = 0;
        } else {
            ofsTemp = *ofs;
        }
        CameraMoveApply(&pos, rot, &ofsTemp, zoom, fov, maxTime);
        mbCameraFocusMasuSet(masuId);
    }
}

BOOL mbCameraMoveCheck(void)
{
    return _CheckFlag(FLAG_BOARD_CAMERA_MOVE) ? FALSE : TRUE;
}

void mbCameraMoveWait(void)
{
    while(!mbCameraMoveCheck()) {
        HuPrcVSleep();
    }
    HuPrcVSleep();
}

void mbCameraMoveStop(void)
{
    MBCAMERA *cameraP = mbCameraGet();
    if(_CheckFlag(FLAG_BOARD_CAMERA_MOVE)) {
        _ClearFlag(FLAG_BOARD_CAMERA_MOVE);
        cameraP->fov = cameraP->moveKey[1].fov;
        cameraP->zoom = cameraP->moveKey[1].zoom;
        cameraP->rot = cameraP->moveKey[1].rot;
        cameraP->target = cameraP->moveKey[1].pos;
        cameraP->offset = cameraP->moveKey[1].ofs;
        cameraP->time = cameraP->maxTime;
    }
}

void mbCameraShakeSet(int time, float power)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->shake = TRUE;
    cameraP->shakePower = power;
    cameraP->shakeTime = time;
}

void mbCameraShakeReset(void)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->shake = FALSE;
    cameraP->shakePower = 0;
    cameraP->shakeTime = 0;
}

void mbCameraPlayerViewSetTime(int playerNo, int viewNo, int maxTime)
{
    MBCAMERA *cameraP = mbCameraGet();
    HuVecF rot;
    rot.x = viewData[viewNo].rotX;
    rot.y = rot.z = 0;
    mbCameraMovePlayer(playerNo, &rot, NULL, viewData[viewNo].zoom, viewData[viewNo].fov, maxTime);
}

void mbCameraPlayerViewSet(int playerNo, int viewNo)
{
    mbCameraPlayerViewSetTime(playerNo, viewNo, 21);
}

void mbCameraPlayerViewSetFast(int playerNo, int viewNo)
{
    mbCameraPlayerViewSetTime(playerNo, viewNo, -1);
}

void mbCameraPlayerViewGet(int viewNo, MBVIEW *view)
{
    memcpy(view, &viewData[viewNo], sizeof(MBVIEW));
}

int mbCameraPlayerViewNoGet(void)
{
    MBCAMERA *cameraP = mbCameraGet();
    int i;
    for(i = 0; i < 3; i++) {
        if(cameraP->zoom <= viewData[i].zoom) {
            break;
        }
    }
    if(i >= 3) {
        return 0;
    }
    return i;
}

float mbCameraPlayerViewZoomGet(int viewNo)
{
    return viewData[viewNo].zoom;
}

float mbCameraPlayerViewRotXGet(int viewNo)
{
    return viewData[viewNo].rotX;
}

void mbCameraMultiFocusSet(void)
{
}
