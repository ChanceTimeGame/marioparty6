#include "game/board/math.h"
#include "game/board/camera.h"
#include "game/board/object.h"
#include "game/memory.h"
#include "game/frand.h"
#include "game/hu3d.h"
#include "game/disp.h"

#define COSTAB_SIZE 2048
#define COSTAB_DEG (COSTAB_SIZE*4/360.0f)
#define COSTAB_RAD ((float)(COSTAB_SIZE*4/(2*M_PI)))
#define COSTAB_GET(ofs) (*(float *)((u8 *)cosTab+((ofs) & ((COSTAB_SIZE-1)*4))))
#define COSTAB_COS(idx) COSTAB_GET((idx)+2)
#define COSTAB_SIN(idx) COSTAB_GET((idx)-(COSTAB_SIZE/4*4)+2)

static float *cosTab;

void mbMathInit(void)
{
    int i;
    cosTab = HuMemDirectMallocNum(HEAP_HEAP, COSTAB_SIZE*sizeof(float), HU_MEMNUM_OVL);
    for(i=0; i<COSTAB_SIZE; i++) {
        cosTab[i] = HuCos(i*(360.0f/COSTAB_SIZE));
    }
}

void mbMathClose(void)
{
    if(cosTab) {
        HuMemDirectFree(cosTab);
        cosTab = NULL;
    }
}

float mbCosDeg(float deg)
{
    float ret; int idx = deg*COSTAB_DEG; ret = COSTAB_COS(idx); return ret;
}

float mbCosRad(float rad)
{
    return COSTAB_COS((int)(rad*COSTAB_RAD));
}

float mbSinDeg(float deg)
{
    float ret; int idx = deg*COSTAB_DEG; ret = COSTAB_SIN(idx); return ret;
}

float mbSinRad(float rad)
{
    return COSTAB_SIN((int)(rad*COSTAB_RAD));
}

asm void mbMtxRotTrigX(register Mtx m, register float sinA, register float cosA)
{
    nofralloc
    fmr f1, f1
    fmr f0, f2
    frsp f1, f1
    frsp f0, f0
    ps_merge00 f1, f1, f1
    ps_merge00 f0, f0, f0
    psq_l f6, 0x20(m), 0, 0
    psq_l f7, 0x28(m), 0, 0
    psq_l f4, 0x10(m), 0, 0
    psq_l f5, 0x18(m), 0, 0
    ps_mul f8, f1, f6
    ps_mul f9, f1, f7
    ps_msub f8, f0, f4, f8
    ps_msub f9, f0, f5, f9
    psq_st f8, 0x10(m), 0, 0
    psq_st f9, 0x18(m), 0, 0
    ps_mul f8, f1, f4
    ps_mul f9, f1, f5
    ps_madd f8, f0, f6, f8
    ps_madd f9, f0, f7, f9
    psq_st f8, 0x20(m), 0, 0
    psq_st f9, 0x28(m), 0, 0
    blr
}

asm void mbMtxRotTrigY(register Mtx m, register float sinA, register float cosA)
{
    nofralloc
    fmr f1, f1
    fmr f0, f2
    frsp f1, f1
    frsp f0, f0
    ps_merge00 f1, f1, f1
    ps_merge00 f0, f0, f0
    psq_l f4, 0x0(m), 0, 0
    psq_l f5, 0x8(m), 0, 0
    psq_l f6, 0x20(m), 0, 0
    psq_l f7, 0x28(m), 0, 0
    ps_mul f8, f0, f4
    ps_mul f9, f0, f5
    ps_madd f8, f1, f6, f8
    ps_madd f9, f1, f7, f9
    psq_st f8, 0x0(m), 0, 0
    psq_st f9, 0x8(m), 0, 0
    ps_mul f8, f1, f4
    ps_mul f9, f1, f5
    ps_msub f8, f0, f6, f8
    ps_msub f9, f0, f7, f9
    psq_st f8, 0x20(m), 0, 0
    psq_st f9, 0x28(m), 0, 0
    blr
}

asm void mbMtxRotTrigZ(register Mtx m, register float sinA, register float cosA)
{
    nofralloc
    fmr f1, f1
    fmr f0, f2
    frsp f1, f1
    frsp f0, f0
    ps_merge00 f1, f1, f1
    ps_merge00 f0, f0, f0
    psq_l f6, 0x10(m), 0, 0
    psq_l f7, 0x18(m), 0, 0
    psq_l f4, 0x0(m), 0, 0
    psq_l f5, 0x8(m), 0, 0
    ps_mul f8, f1, f6
    ps_mul f9, f1, f7
    ps_msub f8, f0, f4, f8
    ps_msub f9, f0, f5, f9
    psq_st f8, 0x0(m), 0, 0
    psq_st f9, 0x8(m), 0, 0
    ps_mul f8, f1, f4
    ps_mul f9, f1, f5
    ps_madd f8, f0, f6, f8
    ps_madd f9, f0, f7, f9
    psq_st f8, 0x10(m), 0, 0
    psq_st f9, 0x18(m), 0, 0
    blr
}

asm void mbMtxRotTrigScaleX(register Mtx m, register HuVecF *scale, register float sinA, register float cosA)
{
    nofralloc
    fmr f1, f1
    fmr f0, f2
    frsp f1, f1
    frsp f0, f0
    lfs f4, 0x0(scale)
    psq_l f5, 0x4(scale), 0, 0
    fneg f6, f1
    ps_sub f7, f5, f5
    ps_merge00 f8, f0, f6
    ps_merge00 f9, f1, f0
    ps_mul f8, f8, f5
    ps_mul f9, f9, f5
    stfs f4, 0x0(m)
    psq_st f7, 0x4(m), 0, 0
    psq_st f7, 0xc(m), 0, 0
    psq_st f8, 0x14(m), 0, 0
    psq_st f7, 0x1c(m), 0, 0
    psq_st f9, 0x24(m), 0, 0
    stfs f7, 0x2c(m)
    blr
}

asm void mbMtxRotTrigScaleY(register Mtx m, register HuVecF *scale, register float sinA, register float cosA)
{
    nofralloc
    frsp f1, f1
    frsp f0, f2
    psq_l f4, 0x0(scale), 0, 0
    lfs f6, 0x8(scale)
    ps_sub f7, f4, f4
    fmuls f8, f0, f4
    fmuls f9, f1, f6
    ps_merge11 f5, f7, f4
    ps_merge00 f8, f8, f7
    ps_merge00 f9, f9, f7
    psq_st f5, 0x10(m), 0, 0
    psq_st f7, 0x18(m), 0, 0
    fneg f5, f1
    psq_st f8, 0x0(m), 0, 0
    psq_st f9, 0x8(m), 0, 0
    fmuls f8, f5, f4
    fmuls f9, f0, f6
    ps_merge00 f8, f8, f7
    ps_merge00 f9, f9, f7
    psq_st f8, 0x20(m), 0, 0
    psq_st f9, 0x28(m), 0, 0
    blr
}

asm void mbMtxRotTrigScaleZ(register Mtx m, register HuVecF *scale, register float sinA, register float cosA)
{
    nofralloc
    fmr f1, f1
    fmr f0, f2
    frsp f1, f1
    frsp f0, f0
    psq_l f4, 0x0(scale), 0, 0
    lfs f5, 0x8(scale)
    fneg f6, f1
    ps_sub f7, f4, f4
    ps_merge00 f8, f0, f6
    ps_merge00 f9, f1, f0
    ps_merge00 f5, f5, f7
    ps_mul f8, f8, f4
    ps_mul f9, f9, f4
    psq_st f8, 0x0(m), 0, 0
    psq_st f7, 0x8(m), 0, 0
    psq_st f9, 0x10(m), 0, 0
    psq_st f7, 0x18(m), 0, 0
    psq_st f7, 0x20(m), 0, 0
    psq_st f5, 0x28(m), 0, 0
    blr
}

void mbMtxRotAxisDeg(Mtx m, s8 axis, float deg)
{
    int idx = deg*COSTAB_DEG;
    PSMTXRotTrig(m, axis, COSTAB_SIN(idx), COSTAB_COS(idx));
}

void mbMtxRotAxisRad(Mtx m, s8 axis, float rad)
{
    int idx = rad*COSTAB_RAD;
    PSMTXRotTrig(m, axis, COSTAB_SIN(idx), COSTAB_COS(idx));
}

#define ASM_ROTX(m, s, c) \
    asm { \
        ps_merge00 s, s, s; \
        ps_merge00 c, c, c; \
        psq_l f6, 0x20(m), 0, 0; \
        psq_l f7, 0x28(m), 0, 0; \
        psq_l f4, 0x10(m), 0, 0; \
        psq_l f5, 0x18(m), 0, 0; \
        ps_mul f8, s, f6; \
        ps_mul f9, s, f7; \
        ps_msub f8, c, f4, f8; \
        ps_msub f9, c, f5, f9; \
        psq_st f8, 0x10(m), 0, 0; \
        psq_st f9, 0x18(m), 0, 0; \
        ps_mul f8, s, f4; \
        ps_mul f9, s, f5; \
        ps_madd f8, c, f6, f8; \
        ps_madd f9, c, f7, f9; \
        psq_st f8, 0x20(m), 0, 0; \
        psq_st f9, 0x28(m), 0, 0; \
    }

void mbMtxRotXDeg(register Mtx m, float deg)
{
    register float s;
    register float c;
    int idx = deg*COSTAB_DEG;
    c = COSTAB_COS(idx);
    s = COSTAB_SIN(idx);
    ASM_ROTX(m, s, c)
}

void mbMtxRotXRad(register Mtx m, float rad)
{
    register float s;
    register float c;
    int idx = rad*COSTAB_RAD;
    c = COSTAB_COS(idx);
    s = COSTAB_SIN(idx);
    ASM_ROTX(m, s, c)
}

#define ASM_ROTY(m, s, c) \
    asm { \
        ps_merge00 s, s, s; \
        ps_merge00 c, c, c; \
        psq_l f4, 0x0(m), 0, 0; \
        psq_l f5, 0x8(m), 0, 0; \
        psq_l f6, 0x20(m), 0, 0; \
        psq_l f7, 0x28(m), 0, 0; \
        ps_mul f8, c, f4; \
        ps_mul f9, c, f5; \
        ps_madd f8, s, f6, f8; \
        ps_madd f9, s, f7, f9; \
        psq_st f8, 0x0(m), 0, 0; \
        psq_st f9, 0x8(m), 0, 0; \
        ps_mul f8, s, f4; \
        ps_mul f9, s, f5; \
        ps_msub f8, c, f6, f8; \
        ps_msub f9, c, f7, f9; \
        psq_st f8, 0x20(m), 0, 0; \
        psq_st f9, 0x28(m), 0, 0; \
    }

void mbMtxRotYDeg(register Mtx m, float deg)
{
    register float s;
    register float c;
    int idx = deg*COSTAB_DEG;
    c = COSTAB_COS(idx);
    s = COSTAB_SIN(idx);
    ASM_ROTY(m, s, c)
}

void mbMtxRotYRad(register Mtx m, float rad)
{
    register float s;
    register float c;
    int idx = rad*COSTAB_RAD;
    c = COSTAB_COS(idx);
    s = COSTAB_SIN(idx);
    ASM_ROTY(m, s, c)
}

#define ASM_ROTZ(m, s, c) \
    asm { \
        ps_merge00 s, s, s; \
        ps_merge00 c, c, c; \
        psq_l f6, 0x10(m), 0, 0; \
        psq_l f7, 0x18(m), 0, 0; \
        psq_l f4, 0x0(m), 0, 0; \
        psq_l f5, 0x8(m), 0, 0; \
        ps_mul f8, s, f6; \
        ps_mul f9, s, f7; \
        ps_msub f8, c, f4, f8; \
        ps_msub f9, c, f5, f9; \
        psq_st f8, 0x0(m), 0, 0; \
        psq_st f9, 0x8(m), 0, 0; \
        ps_mul f8, s, f4; \
        ps_mul f9, s, f5; \
        ps_madd f8, c, f6, f8; \
        ps_madd f9, c, f7, f9; \
        psq_st f8, 0x10(m), 0, 0; \
        psq_st f9, 0x18(m), 0, 0; \
    }

void mbMtxRotZDeg(register Mtx m, float deg)
{
    register float s;
    register float c;
    int idx = deg*COSTAB_DEG;
    c = COSTAB_COS(idx);
    s = COSTAB_SIN(idx);
    ASM_ROTZ(m, s, c)
}

void mbMtxRotZRad(register Mtx m, float rad)
{
    register float s;
    register float c;
    int idx = rad*COSTAB_RAD;
    c = COSTAB_COS(idx);
    s = COSTAB_SIN(idx);
    ASM_ROTZ(m, s, c)
}

void mbMtxScaleRotXDeg(register Mtx m, register HuVecF *scale, float deg)
{
    register float s;
    register float c;
    int idx = deg*COSTAB_DEG;
    c = COSTAB_COS(idx);
    s = COSTAB_SIN(idx);
    asm {
        lfs f4, 0x0(scale)
        psq_l f5, 0x4(scale), 0, 0
        fneg f6, s
        ps_sub f7, f5, f5
        ps_merge00 f8, c, f6
        ps_merge00 f9, s, c
        ps_mul f8, f8, f5
        ps_mul f9, f9, f5
        stfs f4, 0x0(m)
        psq_st f7, 0x4(m), 0, 0
        psq_st f7, 0xc(m), 0, 0
        psq_st f8, 0x14(m), 0, 0
        psq_st f7, 0x1c(m), 0, 0
        psq_st f9, 0x24(m), 0, 0
        stfs f7, 0x2c(m)
    }
}

void mbMtxScaleRotYDeg(register Mtx m, register HuVecF *scale, float deg)
{
    register float s;
    register float c;
    int idx = deg*COSTAB_DEG;
    c = COSTAB_COS(idx);
    s = COSTAB_SIN(idx);
    asm {
        psq_l f4, 0x0(scale), 0, 0
        lfs f6, 0x8(scale)
        ps_sub f7, f4, f4
        fmuls f8, c, f4
        fmuls f9, s, f6
        ps_merge11 f5, f7, f4
        ps_merge00 f8, f8, f7
        ps_merge00 f9, f9, f7
        psq_st f5, 0x10(m), 0, 0
        psq_st f7, 0x18(m), 0, 0
        fneg f5, s
        psq_st f8, 0x0(m), 0, 0
        psq_st f9, 0x8(m), 0, 0
        fmuls f8, f5, f4
        fmuls f9, c, f6
        ps_merge00 f8, f8, f7
        ps_merge00 f9, f9, f7
        psq_st f8, 0x20(m), 0, 0
        psq_st f9, 0x28(m), 0, 0
    }
}

void mbMtxScaleRotZDeg(register Mtx m, register HuVecF *scale, float deg)
{
    register float s;
    register float c;
    int idx = deg*COSTAB_DEG;
    c = COSTAB_COS(idx);
    s = COSTAB_SIN(idx);
    asm {
        psq_l f4, 0x0(scale), 0, 0
        lfs f5, 0x8(scale)
        fneg f6, s
        ps_sub f7, f4, f4
        ps_merge00 f8, c, f6
        ps_merge00 f9, s, c
        ps_merge00 f5, f5, f7
        ps_mul f8, f8, f4
        ps_mul f9, f9, f4
        psq_st f8, 0x0(m), 0, 0
        psq_st f7, 0x8(m), 0, 0
        psq_st f9, 0x10(m), 0, 0
        psq_st f7, 0x18(m), 0, 0
        psq_st f7, 0x20(m), 0, 0
        psq_st f5, 0x28(m), 0, 0
    }
}

/* The copies of the Y/Z rotations inlined into mbMtxRot allocate cos before sin. */
static inline void MtxRotYDeg(register Mtx m, float deg)
{
    int idx = deg*COSTAB_DEG;
    register float c = COSTAB_COS(idx);
    register float s = COSTAB_SIN(idx);
    ASM_ROTY(m, s, c)
}

static inline void MtxRotZDeg(register Mtx m, float deg)
{
    int idx = deg*COSTAB_DEG;
    register float c = COSTAB_COS(idx);
    register float s = COSTAB_SIN(idx);
    ASM_ROTZ(m, s, c)
}

void mbMtxRot(Mtx m, float x, float y, float z)
{
    if(x != 0.0f) {
        mbMtxRotAxisDeg(m, 'x', x);
    } else {
        PSMTXIdentity(m);
    }
    if(y != 0.0f) {
        MtxRotYDeg(m, y);
    }
    if(z != 0.0f) {
        MtxRotZDeg(m, z);
    }
}

asm void mbMtxTransCat(register Mtx m, register float x, register float y, register float z)
{
    nofralloc
    lfs f4, 0xc(m)
    lfs f5, 0x1c(m)
    lfs f6, 0x2c(m)
    fadd f4, f4, x
    fadd f5, f5, y
    fadd f6, f6, z
    stfs f4, 0xc(m)
    stfs f5, 0x1c(m)
    stfs f6, 0x2c(m)
    blr
}

u32 mbRandMod(u32 max)
{
    u32 value = frand() & 0x7FFFFFFF;
    if(value & 1) {
        value |= 0x80000000;
    }
    return ((u64)value*max) >> 32;
}

float mbVecMagXZ(HuVecF *a, HuVecF *b)
{
    float dx = a->x-b->x;
    float dz = a->z-b->z;
    return HuMagPoint2D(dx, dz);
}

BOOL mbVecMagXZCheck(HuVecF *a, HuVecF *b, float maxDist)
{
    float dist = mbVecMagXZ(a, b);
    if(dist <= maxDist) {
        return TRUE;
    } else {
        return FALSE;
    }
}

float mbAngleWrap(float angle)
{
    angle = fmod(angle, 360);
    if(angle < -180.0f) {
        angle += 360.0f;
    } else if(angle > 180.0f) {
        angle -= 360.0f;
    }
    return angle;
}

void mbAngleWrapV(HuVecF *angle)
{
    int i;
    float *dest = (float *)angle;
    for(i=0; i<3; i++) {
        *dest = mbAngleWrap(*dest);
        dest++;
    }
}

BOOL mbAngleAdd(float *dest, float angle, float speed)
{
    float wrapAngle = fmod(angle-(*dest), 360);
    float diff;
    if(fabs(wrapAngle) < speed) {
        *dest = angle;
        return TRUE;
    }
    if(wrapAngle < 0.0f) {
        wrapAngle += 360.0f;
    }
    if(wrapAngle > 180.0f) {
        diff = -speed;
    } else {
        diff = speed;
    }
    *dest += diff;
    *dest = mbAngleWrap(*dest);
    return FALSE;
}

BOOL mbAngleMoveTo(float *dest, float angle, float speed)
{
    float wrapAngle = fmod(angle-(*dest), 360);
    if(fabsf2(wrapAngle) < 1.0f) {
        *dest = angle;
        return TRUE;
    }
    if(wrapAngle < 0.0f) {
        wrapAngle += 360.0f;
    }
    if(wrapAngle > 180.0f) {
        wrapAngle -= 360.0f;
    }
    *dest = fmod(*dest+(speed*wrapAngle), 360.0);
    if(*dest < 0.0f) {
        *dest += 360.0f;
    }
    return FALSE;
}

float mbAngleWrap2(float a, float b)
{
    float diff = fmod(a-b, 360);
    if(diff < 0.0f) {
        diff += 360.0f;
    }
    if(diff >= 180.0f) {
        diff -= 360.0f;
    }
    return diff;
}

BOOL mbVecMagCheck(HuVecF *a, HuVecF *b, float dist)
{
    HuVecF diff;
    VECSubtract(a, b, &diff);
    if(VECSquareMag(&diff) >= dist*dist) {
        return FALSE;
    } else {
        return TRUE;
    }
}

void mbMtxLookAtCalc(Mtx dest, HuVecF *eye, HuVecF *up, HuVecF *target)
{
    HuVecF f, u, s;
    f.x = eye->x-target->x;
    f.y = eye->y-target->y;
    f.z = eye->z-target->z;
    VECNormalize(&f, &f);
    VECCrossProduct(up, &f, &u);
    VECNormalize(&u, &u);
    VECCrossProduct(&f, &u, &s);
    dest[0][0] = u.x;
    dest[0][1] = u.y;
    dest[0][2] = u.z;
    dest[0][3] = 0;
    dest[1][0] = s.x;
    dest[1][1] = s.y;
    dest[1][2] = s.z;
    dest[1][3] = 0;
    dest[2][0] = f.x;
    dest[2][1] = f.y;
    dest[2][2] = f.z;
    dest[2][3] = 0;
}

void mbPos3Dto2D(HuVecF *src, HuVecF *dst)
{
    MBCAMERA *cameraP = mbCameraGet();
    float tanFov;
    float width;
    float height;
    Mtx lookAt;
    HuVecF pos;

    MTXLookAt(lookAt, &cameraP->pos, &cameraP->up, &cameraP->target);
    MTXMultVec(lookAt, src, &pos);
    tanFov = mbSinDeg(cameraP->fov*0.5f)/mbCosDeg(cameraP->fov*0.5f);
    width = HU_DISP_ASPECT*(tanFov*pos.z);
    height = tanFov*pos.z;
    dst->x = HU_DISP_CENTERX+(pos.x*(HU_DISP_CENTERX/(-width)));
    dst->y = HU_DISP_CENTERY+(pos.y*(HU_DISP_CENTERY/height));
    dst->z = -pos.z;
}

static inline HU3D_CAMERA *CameraBitGet(s16 bit)
{
    int i;
    for(i=0; i<HU3D_CAM_MAX; i++) {
        if(bit & (1 << i)) {
            break;
        }
    }
    return &Hu3DCamera[i];
}

void mbPos3DtoNorm(HuVecF *src, s16 camBit, HuVecF *dst)
{
    HU3D_CAMERA *cameraP = CameraBitGet(camBit);
    float tanFov;
    float height;
    Mtx lookAt;
    HuVecF pos;

    MTXLookAt(lookAt, &cameraP->pos, &cameraP->up, &cameraP->target);
    MTXMultVec(lookAt, src, &pos);
    tanFov = mbSinDeg(cameraP->fov*0.5f)/mbCosDeg(cameraP->fov*0.5f);
    height = tanFov*(-pos.z);
    dst->x = pos.x/(HU_DISP_ASPECT*height);
    dst->y = pos.y/height;
    dst->z = pos.z;
}

void mbPos2Dto3D(HuVecF *src, HuVecF *dst)
{
    MBCAMERA *cameraP = mbCameraGet();
    float tanFov = mbSinDeg(cameraP->fov*0.5f)/mbCosDeg(cameraP->fov*0.5f);
    float height = 2.0f*(tanFov*src->z);
    float normY = src->y/HU_DISP_HEIGHT;
    float normX = src->x/HU_DISP_WIDTH;
    Mtx lookAt;

    dst->x = (normX-0.5)*(HU_DISP_ASPECT*height);
    dst->y = -(normY-0.5)*height;
    dst->z = -src->z;
    mbCameraLookAtInvGet(lookAt);
    MTXMultVec(lookAt, dst, dst);
}

void mbNormPosto3D(HuVecF *src, s16 camBit, HuVecF *dst)
{
    HU3D_CAMERA *cameraP = CameraBitGet(camBit);
    float height = (mbSinDeg(cameraP->fov*0.5f)/mbCosDeg(cameraP->fov*0.5f))*fabs(src->z);
    Mtx lookAt;
    Mtx inv;

    dst->x = src->x*(HU_DISP_ASPECT*height);
    dst->y = src->y*height;
    dst->z = src->z;
    MTXLookAt(lookAt, &cameraP->pos, &cameraP->up, &cameraP->target);
    MTXInverse(lookAt, inv);
    MTXMultVec(inv, dst, dst);
}

void mbNormPosto2D(HuVecF *src, HuVecF *dst)
{
    dst->x = HU_DISP_CENTERX*(1.0f+src->x);
    dst->y = -HU_DISP_CENTERY*(src->y-1.0f);
    dst->z = src->z;
}

float mbBezierCalc(float a, float b, float c, float t)
{
    float invTime = 1.0f-t;
    return (c*(t*t))+((a*(invTime*invTime))+(b*((2.0f*invTime)*t)));
}

void mbBezierCalcV(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *dst, float t)
{
    dst->x = mbBezierCalc(a->x, b->x, c->x, t);
    dst->y = mbBezierCalc(a->y, b->y, c->y, t);
    dst->z = mbBezierCalc(a->z, b->z, c->z, t);
}

void mbBezierCalcVList(HuVecF *src, HuVecF *dst, float t)
{
    dst->x = mbBezierCalc(src[0].x, src[1].x, src[2].x, t);
    dst->y = mbBezierCalc(src[0].y, src[1].y, src[2].y, t);
    dst->z = mbBezierCalc(src[0].z, src[1].z, src[2].z, t);
}

float mbBezierCalcSlope(float a, float b, float c, float t)
{
    return 2.0f*((-a+b)+(t*(c+(a-(2.0f*b)))));
}

void mbBezierCalcSlopeV(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *dst, float t)
{
    dst->x = mbBezierCalcSlope(a->x, b->x, c->x, t);
    dst->y = mbBezierCalcSlope(a->y, b->y, c->y, t);
    dst->z = mbBezierCalcSlope(a->z, b->z, c->z, t);
}

float mbHermiteCalc(float a, float b, float c, float d, float t)
{
    float tt = t*t;
    float ttt = t*tt;
    float aCoef = 1.0f+((2.0f*ttt)-(3.0f*tt));
    float bCoef = (-2.0f*ttt)+(3.0f*tt);
    float cCoef = t+(ttt-(2.0f*tt));
    float dCoef = ttt-tt;
    return (aCoef*a)+(bCoef*b)+(cCoef*c)+(dCoef*d);
}

void mbHermiteCalcV(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, HuVecF *dst, float t)
{
    dst->x = mbHermiteCalc(a->x, b->x, c->x, d->x, t);
    dst->y = mbHermiteCalc(a->y, b->y, c->y, d->y, t);
    dst->z = mbHermiteCalc(a->z, b->z, c->z, d->z, t);
}

float mbHermiteCalcSlope(float a, float b, float c, float d, float t)
{
    float tt = t*t;
    float aCoef = (6.0f*tt)-(6.0f*t);
    float bCoef = (-6.0f*tt)+(6.0f*t);
    float cCoef = 1.0f+((3.0f*tt)-(4.0f*t));
    float dCoef = (3.0f*tt)-(2.0f*t);
    return (aCoef*a)+(bCoef*b)+(cCoef*c)+(dCoef*d);
}

float mbAngleLerp(float a, float b, float t)
{
    float diff = fmod(b-a, 360);
    float ret;
    if(diff < 0.0f) {
        diff += 360.0f;
    }
    if(diff > 180.0f) {
        diff -= 360.0f;
    }
    ret = fmod(a+(t*diff), 360);
    if(ret < 0) {
        ret += 360.0f;
    }
    return ret;
}

float mbAngleEaseOut(float a, float b, float t)
{
    return mbAngleLerp(a, b, HuSin(90.0f*t));
}

float mbAngleEaseIn(float a, float b, float t)
{
    return mbAngleLerp(a, b, 1.0-HuCos(90.0f*t));
}

float mbMathDistScale(HuVecF *src, float scale, HuVecF *dst)
{
    MBCAMERA *cameraP = mbCameraGet();
    HuVecF pos;
    float tanFov;
    float depth;
    float z;
    mbPos3Dto2D(src, &pos);
    tanFov = HuSin(cameraP->fov*0.5f)/HuCos(cameraP->fov*0.5f);
    depth = pos.z*tanFov;
    z = (depth/scale)/tanFov;
    pos.z = z;
    mbPos2Dto3D(&pos, dst);
    return 0.0f;
}

static void ObjectCullUpdate(HSF_OBJECT *obj, Mtx mtx)
{
    static HuVecF box[8];
    static HuVecF boxView[8];
    HU3D_CAMERA *cameraP;
    float tanFov;
    float near;
    float aspect;
    float fov;
    float height;
    float invAspect;
    BOOL cullF;
    int i;
    int idx;
    HuVecF *p;
    HuVecF *view;
    HuVecF center;
    HuVecF pos;
    ROMtx roMtx;
    HuVecF *min;
    HuVecF *max;

    obj->flags &= ~0x400;
    if(shadowModelDrawF == FALSE) {
        cameraP = &Hu3DCamera[Hu3DCameraNo];
        fov = cameraP->fov;
        near = cameraP->near;
        aspect = cameraP->aspect;
    } else {
        fov = Hu3DShadow->fov;
        near = Hu3DShadow->near;
        aspect = 1.0f;
    }
    tanFov = -1.0f*(mbSinDeg(fov*0.5f)/mbCosDeg(fov*0.5f));
    VECAdd(&obj->mesh.mesh.min, &obj->mesh.mesh.max, &center);
    VECScale(&center, &center, 0.5f);
    MTXMultVec(mtx, &center, &pos);
    height = fabsf2(pos.z*tanFov);
    if(fabsf2(pos.x) <= height*aspect && fabsf2(pos.y) <= height && pos.y < -near) {
        return;
    }
    MTXReorder(mtx, roMtx);
    invAspect = 1.0f/aspect;
    roMtx[0][0] *= invAspect;
    roMtx[1][0] *= invAspect;
    roMtx[2][0] *= invAspect;
    roMtx[3][0] *= invAspect;
    roMtx[0][2] *= tanFov;
    roMtx[1][2] *= tanFov;
    roMtx[2][2] *= tanFov;
    roMtx[3][2] *= tanFov;
    min = &obj->mesh.mesh.min;
    max = &obj->mesh.mesh.max;
    box[0].x = max->x;
    box[0].y = max->y;
    box[0].z = max->z;
    box[1].x = max->x;
    box[1].y = max->y;
    box[1].z = min->z;
    box[2].x = max->x;
    box[2].y = min->y;
    box[2].z = max->z;
    box[3].x = max->x;
    box[3].y = min->y;
    box[3].z = min->z;
    box[4].x = min->x;
    box[4].y = max->y;
    box[4].z = max->z;
    box[5].x = min->x;
    box[5].y = max->y;
    box[5].z = min->z;
    box[6].x = min->x;
    box[6].y = min->y;
    box[6].z = max->z;
    box[7].x = min->x;
    box[7].y = min->y;
    box[7].z = min->z;
    PSMTXROMultVecArray(roMtx, box, boxView, 8);
    view = boxView;
    for(i=0, p=view; i<8; p++, i++) {
        if(p->z > near) {
            break;
        }
    }
    if(i >= 8) {
        obj->flags |= 0x400;
        return;
    }
    cullF = FALSE;
    if(pos.x >= 0.0f) {
        for(i=0, p=view; i<8; p++, i++) {
            if(p->x < p->z) {
                break;
            }
        }
        if(i >= 8) {
            cullF = TRUE;
        }
    } else {
        for(i=0, p=view; i<8; p++, i++) {
            if(p->x > -p->z) {
                break;
            }
        }
        if(i >= 8) {
            cullF = TRUE;
        }
    }
    if(cullF) {
        obj->flags |= 0x400;
        return;
    }
    cullF = FALSE;
    if(pos.y >= 0.0f) {
        for(i=0, p=view; i<8; p++, i++) {
            if(p->y < p->z) {
                break;
            }
        }
        if(i >= 8) {
            cullF = TRUE;
        }
    } else {
        for(i=0, p=view; i<8; p++, i++) {
            if(p->y > -p->z) {
                break;
            }
        }
        if(i >= 8) {
            cullF = TRUE;
        }
    }
    if(cullF) {
        obj->flags |= 0x400;
    }
}

static void ObjectBBoxUpdate(register HSF_OBJECT *obj)
{
    register float *vtx = obj->mesh.vertex->data;
    register int num = obj->mesh.vertex->count;
    obj->mesh.mesh.max.x = obj->mesh.mesh.max.y = -1000000.0f;
    obj->mesh.mesh.min.x = obj->mesh.mesh.min.y = 1000000.0f;
    asm {
        subi vtx, vtx, 4
        mtctr num
        psq_l f0, 0x70(obj), 0, 0
        psq_l f2, 0x64(obj), 0, 0
        ps_mr f1, f0
        ps_mr f3, f2
    }
    asm {
    loop:
        psq_l f4, 0x4(vtx), 0, 0
        psq_lu f5, 0xc(vtx), 1, 0
        ps_cmpo0 cr0, f0, f4
        bge max0
        ps_cmpo1 cr0, f0, f4
        bge max1
        ps_mr f0, f4
        b maxz
    max1:
        ps_merge01 f0, f4, f0
        b maxz
    max0:
        ps_cmpo1 cr0, f0, f4
        bge maxz
        ps_merge01 f0, f0, f4
    maxz:
        ps_cmpo0 cr0, f1, f5
        bge min
        ps_mr f1, f5
    min:
        ps_cmpo0 cr0, f2, f4
        ble min0
        ps_cmpo1 cr0, f2, f4
        ble min1
        ps_mr f2, f4
        b minz
    min1:
        ps_merge01 f2, f4, f2
        b minz
    min0:
        ps_cmpo1 cr0, f2, f4
        ble minz
        ps_merge01 f2, f2, f4
    minz:
        ps_cmpo0 cr0, f3, f5
        ble next
        ps_mr f3, f5
    next:
        bdnz loop
        psq_st f0, 0x70(obj), 0, 0
        psq_st f1, 0x78(obj), 1, 0
        psq_st f2, 0x64(obj), 0, 0
        psq_st f3, 0x6c(obj), 1, 0
    }
}

static inline void MtxScaleTrigX(register Mtx m, register HuVecF *scale, register float s, register float c)
{
    asm {
        lfs f4, 0x0(scale)
        psq_l f5, 0x4(scale), 0, 0
        fneg f6, s
        ps_sub f7, f5, f5
        ps_merge00 f8, c, f6
        ps_merge00 f9, s, c
        ps_mul f8, f8, f5
        ps_mul f9, f9, f5
        stfs f4, 0x0(m)
        psq_st f7, 0x4(m), 0, 0
        psq_st f7, 0xc(m), 0, 0
        psq_st f8, 0x14(m), 0, 0
        psq_st f7, 0x1c(m), 0, 0
        psq_st f9, 0x24(m), 0, 0
        stfs f7, 0x2c(m)
    }
}

static inline void MtxScaleTrigY(register Mtx m, register HuVecF *scale, register float s, register float c)
{
    asm {
        psq_l f4, 0x0(scale), 0, 0
        lfs f6, 0x8(scale)
        ps_sub f7, f4, f4
        fmuls f8, c, f4
        fmuls f9, s, f6
        ps_merge11 f5, f7, f4
        ps_merge00 f8, f8, f7
        ps_merge00 f9, f9, f7
        psq_st f5, 0x10(m), 0, 0
        psq_st f7, 0x18(m), 0, 0
        fneg f5, s
        psq_st f8, 0x0(m), 0, 0
        psq_st f9, 0x8(m), 0, 0
        fmuls f8, f5, f4
        fmuls f9, c, f6
        ps_merge00 f8, f8, f7
        ps_merge00 f9, f9, f7
        psq_st f8, 0x20(m), 0, 0
        psq_st f9, 0x28(m), 0, 0
    }
}

static inline void MtxScaleTrigZ(register Mtx m, register HuVecF *scale, register float s, register float c)
{
    asm {
        psq_l f4, 0x0(scale), 0, 0
        lfs f5, 0x8(scale)
        fneg f6, s
        ps_sub f7, f4, f4
        ps_merge00 f8, c, f6
        ps_merge00 f9, s, c
        ps_merge00 f5, f5, f7
        ps_mul f8, f8, f4
        ps_mul f9, f9, f4
        psq_st f8, 0x0(m), 0, 0
        psq_st f7, 0x8(m), 0, 0
        psq_st f9, 0x10(m), 0, 0
        psq_st f7, 0x18(m), 0, 0
        psq_st f7, 0x20(m), 0, 0
        psq_st f5, 0x28(m), 0, 0
    }
}

static inline void MtxTrigY(register Mtx m, register float s, register float c)
{
    ASM_ROTY(m, s, c)
}

static inline void MtxTrigZ(register Mtx m, register float s, register float c)
{
    ASM_ROTZ(m, s, c)
}

static void ObjectCullHook(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr)
{
    Mtx mtx;
    BOOL rotF = FALSE;
    float s;
    float c;
    int idx;
    if(transform->rot.x != 0.0f) {
        rotF = TRUE;
        idx = transform->rot.x*COSTAB_DEG;
        c = COSTAB_COS(idx);
        s = COSTAB_SIN(idx);
        MtxScaleTrigX(mtx, &transform->scale, s, c);
    }
    if(transform->rot.y != 0.0f) {
        idx = transform->rot.y*COSTAB_DEG;
        s = COSTAB_SIN(idx);
        c = COSTAB_COS(idx);
        if(!rotF) {
            rotF = TRUE;
            MtxScaleTrigY(mtx, &transform->scale, s, c);
        } else {
            MtxTrigY(mtx, s, c);
        }
    }
    if(transform->rot.z != 0.0f) {
        idx = transform->rot.z*COSTAB_DEG;
        s = COSTAB_SIN(idx);
        c = COSTAB_COS(idx);
        if(!rotF) {
            rotF = TRUE;
            MtxScaleTrigZ(mtx, &transform->scale, s, c);
        } else {
            MtxTrigZ(mtx, s, c);
        }
    }
    if(!rotF) {
        MTXScale(mtx, transform->scale.x, transform->scale.y, transform->scale.z);
    }
    mtx[0][3] = transform->pos.x;
    mtx[1][3] = transform->pos.y;
    mtx[2][3] = transform->pos.z;
    MTXConcat(*prev, mtx, *curr);
    ObjectCullUpdate(obj, *curr);
}

void mbObjCullInit(s16 mdlId)
{
    HU3D_MODEL *modelP;
    HSF_DATA *hsf;
    s16 i;
    HSF_OBJECT *obj;
    BOOL cullF = FALSE;
    HU3D_MODELID modelId;

    modelP = &Hu3DData[mbObjModelIDGet(mdlId)];
    hsf = modelP->hsf;
    modelId = mbObjModelIDGet(mdlId);
    obj = hsf->object;
    for(i=0; i<hsf->objectNum; i++, obj++) {
        if(obj->mesh.cenvNum == 0 && obj->constData != NULL) {
            cullF = TRUE;
            ObjectBBoxUpdate(obj);
            ((HSF_CONSTDATA *)obj->constData)->hook = ObjectCullHook;
        }
    }
    if(!cullF) {
        Hu3DModelAttrSet(mbObjModelIDGet(mdlId), HU3D_ATTR_NOCULL);
    }
}
