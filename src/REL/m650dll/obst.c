/* Retail's file carries no math.h sqrtf statics: its 0.5/3.0 doubles are pooled at their use */
#define _MATH_H
#define M_PI 3.141592653589793
double sin(double x);
double cos(double x);
double atan2(double y, double x);

static inline float sqrtf(float x)
{
    const double _half = .5;
    const double _three = 3.0;
    volatile float y;
    if (x > 0.0f) {
        double guess = __frsqrte((double)x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        y = (float)(x * guess);
        return y;
    }
    return x;
}

#include "REL/m650dll/m650.h"

/* .bss is laid out in reverse definition order */
HU3D_MODELID lbl_1_bss_4B6;
HU3D_MODELID lbl_1_bss_4B4;
HU3D_MODELID lbl_1_bss_4AC[4];
M650_OBSTFLAG lbl_1_bss_2AC[32][4];
s32 lbl_1_bss_2A8;

M650_OBST lbl_1_data_1F8[5] = {
    { { -550.0f, 0.0f, 1400.0f }, 0 },
    { { -350.0f, 0.0f, 1800.0f }, 1 },
    { { 0.0f, 0.0f, 2000.0f }, 0 },
    { { 350.0f, 0.0f, 1800.0f }, 1 },
    { { 550.0f, 0.0f, 1400.0f }, 0 },
};
M650_OBST lbl_1_data_248[32] = {
    { { -700.0f, 0.0f, 1000.0f }, 0 },
    { { 0.0f, 0.0f, 1000.0f }, 0 },
    { { 700.0f, 0.0f, 1000.0f }, 0 },
    { { -1050.0f, 0.0f, 1500.0f }, 0 },
    { { -350.0f, 0.0f, 1500.0f }, 0 },
    { { 350.0f, 0.0f, 1500.0f }, 0 },
    { { 1500.0f, 0.0f, 1500.0f }, 0 },
    { { -700.0f, 0.0f, 1950.0f }, 0 },
    { { 700.0f, 0.0f, 1950.0f }, 0 },
    { { -1000.0f, 0.0f, 2450.0f }, 0 },
    { { -600.0f, 0.0f, 2450.0f }, 0 },
    { { -200.0f, 0.0f, 2450.0f }, 0 },
    { { 200.0f, 0.0f, 2450.0f }, 0 },
    { { 600.0f, 0.0f, 2450.0f }, 0 },
    { { 1000.0f, 0.0f, 2450.0f }, 0 },
    { { -700.0f, 0.0f, 2900.0f }, 1 },
    { { 0.0f, 0.0f, 2900.0f }, 1 },
    { { 700.0f, 0.0f, 2900.0f }, 1 },
    { { -1050.0f, 0.0f, 3400.0f }, 1 },
    { { -350.0f, 0.0f, 3400.0f }, 1 },
    { { 350.0f, 0.0f, 3400.0f }, 1 },
    { { 1050.0f, 0.0f, 3400.0f }, 1 },
    { { -700.0f, 0.0f, 3850.0f }, 1 },
    { { 0.0f, 0.0f, 3850.0f }, 1 },
    { { 700.0f, 0.0f, 3850.0f }, 1 },
    { { -1050.0f, 0.0f, 4300.0f }, 1 },
    { { -350.0f, 0.0f, 4300.0f }, 1 },
    { { 350.0f, 0.0f, 4300.0f }, 1 },
    { { 1050.0f, 0.0f, 4300.0f }, 1 },
    { { -700.0f, 0.0f, 4750.0f }, 1 },
    { { 0.0f, 0.0f, 4750.0f }, 1 },
    { { 700.0f, 0.0f, 4750.0f }, 1 },
};

u32 lbl_1_data_448[2] = { DATANUM(DATA_m650, 2), DATANUM(DATA_m650, 8) };
u32 lbl_1_data_450[2] = { DATANUM(DATA_m650, 3), DATANUM(DATA_m650, 9) };

void fn_1_62E0(void)
{
    u16 camBit[4] = { HU3D_CAM0, HU3D_CAM1, HU3D_CAM2, HU3D_CAM3 };
    int i;

    lbl_1_bss_2A8 = 0;
    memset(lbl_1_bss_2AC, 0, sizeof(lbl_1_bss_2AC));
    lbl_1_bss_4B6 = Hu3DModelCreateData(lbl_1_data_448[lbl_1_bss_0.unkA]);
    lbl_1_bss_4B4 = Hu3DModelCreateData(lbl_1_data_450[lbl_1_bss_0.unkA]);
    Hu3DModelCameraSet(lbl_1_bss_4B6, HU3D_CAM_ALL);
    Hu3DModelCameraSet(lbl_1_bss_4B4, HU3D_CAM_ALL);
    Hu3DModelAttrSet(lbl_1_bss_4B6, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(lbl_1_bss_4B4, HU3D_ATTR_DISPOFF);
    for (i = 0; i < 4; i++) {
        lbl_1_bss_4AC[i] = Hu3DHookFuncCreate(fn_1_6478);
        Hu3DModelCameraSet(lbl_1_bss_4AC[i], camBit[i]);
    }
}

void fn_1_6478(HU3D_MODEL *modelP, Mtx *mtx)
{
    HSF_DATA *hsf;
    Mtx drawMtx;
    HU3D_MODEL *srcModelP;
    HSF_OBJECT *objA;
    HSF_OBJECT *objB;
    HSF_OBJECT *shadowObj;
    M650_OBSTFLAG *hit;
    M650_PLAYER *work;
    HU3D_MODELID mdlId;
    float ofsZ;
    float motTime;
    float maxTime;
    int i;
    int j;
    int k;

    Hu3DModelObjDrawInit();
    srcModelP = &Hu3DData[lbl_1_bss_4B6];
    hsf = srcModelP->hsf;
    objA = Hu3DModelObjPtrGet(lbl_1_bss_4B6, "m650_03N");
    objB = Hu3DModelObjPtrGet(lbl_1_bss_4B4, "m650_04N");
    shadowObj = Hu3DModelObjPtrGet(lbl_1_bss_4B6, "shadow");
    for (i = 0; i < 32; i++) {
        hit = &lbl_1_bss_2AC[i][lbl_1_bss_2A8];
        if (hit->unk0 == 2) {
            continue;
        }
        switch (hit->unk0) {
            case 3:
                /* Retail fills four rows of a 3x4 Mtx: the fourth row overruns drawMtx on the stack. */
                for (j = 0; j < 4; j++) {
                    for (k = 0; k < 4; k++) {
                        if (j == k) {
                            drawMtx[j][k] = 1.0f;
                        } else {
                            drawMtx[j][k] = 0.0f;
                        }
                    }
                }
                if (lbl_1_bss_0.unk14) {
                    ofsZ = 350.0f;
                } else {
                    ofsZ = 0.0f;
                }
                mtxTransCat(drawMtx, lbl_1_data_1F8[i].pos.x, 0.5f + lbl_1_data_1F8[i].pos.y, ofsZ + lbl_1_data_1F8[i].pos.z);
                PSMTXConcat(*mtx, drawMtx, drawMtx);
                if (lbl_1_data_1F8[i].unkC == 0) {
                    Hu3DModelObjPtrDraw(lbl_1_bss_4B6, objA, drawMtx);
                } else {
                    Hu3DModelObjPtrDraw(lbl_1_bss_4B4, objB, drawMtx);
                }
                Hu3DModelObjPtrDraw(lbl_1_bss_4B6, shadowObj, drawMtx);
                break;
            case 0:
                for (j = 0; j < 4; j++) {
                    for (k = 0; k < 4; k++) {
                        if (j == k) {
                            drawMtx[j][k] = 1.0f;
                        } else {
                            drawMtx[j][k] = 0.0f;
                        }
                    }
                }
                mtxTransCat(drawMtx, lbl_1_data_248[i].pos.x, 0.5f + lbl_1_data_248[i].pos.y, lbl_1_data_248[i].pos.z);
                PSMTXConcat(*mtx, drawMtx, drawMtx);
                if (lbl_1_data_248[i].unkC == 0) {
                    Hu3DModelObjPtrDraw(lbl_1_bss_4B6, objA, drawMtx);
                } else {
                    Hu3DModelObjPtrDraw(lbl_1_bss_4B4, objB, drawMtx);
                }
                Hu3DModelObjPtrDraw(lbl_1_bss_4B6, shadowObj, drawMtx);
                break;
            case 1:
                work = &lbl_1_bss_28[lbl_1_bss_2A8];
                if (lbl_1_data_248[i].unkC == 0) {
                    mdlId = work->unk7A;
                } else {
                    mdlId = work->unk7C;
                }
                motTime = Hu3DMotionTimeGet(mdlId);
                maxTime = Hu3DMotionMaxTimeGet(mdlId);
                if (motTime >= maxTime) {
                    if (hit->unk2 % 4 == 0) {
                        Hu3DModelAttrSet(mdlId, HU3D_ATTR_DISPOFF);
                    } else if (hit->unk2 % 4 == 2) {
                        Hu3DModelAttrReset(mdlId, HU3D_ATTR_DISPOFF);
                        Hu3DModelAttrReset(mdlId, HU3D_ATTR_DISPOFF);
                    }
                    hit->unk2++;
                    if (hit->unk2 >= 30) {
                        Hu3DModelAttrSet(mdlId, HU3D_ATTR_DISPOFF);
                        hit->unk0 = 2;
                    }
                }
                break;
        }
    }
    lbl_1_bss_2A8 = (lbl_1_bss_2A8 + 1) % 4;
}

s16 fn_1_69B0(s16 player)
{
    M650_PLAYER *work = &lbl_1_bss_28[player];
    HuVecF pos;
    HuVecF reflect;
    HuVecF normal;
    s32 wallSe[4] = { 0x7F8, 0x7F9, 0x7FA, 0x7FB };
    s32 seTblA[4] = { 0x7FC, 0x7FD, 0x7FE, 0x7FF };
    s32 seTblB[4] = { 0x800, 0x801, 0x802, 0x803 };
    int i;
    HU3D_MODELID mdlId;

    Hu3DModelPosGet(work->mdlId, &pos);
    for (i = 0; i < 32; i++) {
        if (lbl_1_bss_2AC[i][player].unk0 == 0 && fn_1_6EE4(&pos, &lbl_1_data_248[i].pos, 150.0f)) {
            if (lbl_1_data_248[i].unkC == 0) {
                mdlId = work->unk7A;
            } else {
                mdlId = work->unk7C;
            }
            Hu3DModelPosSetV(mdlId, &lbl_1_data_248[i].pos);
            Hu3DMotionTimeSet(mdlId, 0.0f);
            Hu3DMotionSpeedSet(mdlId, 1.0f);
            Hu3DModelAttrReset(mdlId, HU3D_ATTR_DISPOFF);
            Hu3DModelPosSetV(work->unk78, &lbl_1_data_248[i].pos);
            Hu3DMotionTimeSet(work->unk78, 0.0f);
            Hu3DMotionSpeedSet(work->unk78, 1.0f);
            Hu3DModelAttrReset(work->unk78, HU3D_ATTR_DISPOFF);
            lbl_1_bss_2AC[i][player].unk0 = 1;
            normal.x = pos.x - lbl_1_data_248[i].pos.x;
            normal.y = 0.0f;
            normal.z = pos.z - lbl_1_data_248[i].pos.z;
            VECReflect(&work->unk2C, &normal, &reflect);
            VECNormalize(&reflect, &work->unk38);
            if (lbl_1_data_248[i].unkC == 0) {
                HuAudFXPlay(seTblA[player]);
            } else {
                HuAudFXPlay(seTblB[player]);
            }
            return TRUE;
        }
    }
    if (pos.x <= -1150.0f) {
        normal.x = 1.0f;
        normal.y = normal.z = 0.0f;
        VECReflect(&work->unk2C, &normal, &reflect);
        VECNormalize(&reflect, &work->unk38);
        HuAudFXPlay(wallSe[player]);
        return TRUE;
    } else if (pos.x >= 1150.0f) {
        normal.x = -1.0f;
        normal.y = normal.z = 0.0f;
        VECReflect(&work->unk2C, &normal, &reflect);
        VECNormalize(&reflect, &work->unk38);
        HuAudFXPlay(wallSe[player]);
        return TRUE;
    }
    return FALSE;
}

s16 fn_1_6D88(s16 player, HuVecF *pos)
{
    M650_PLAYER *work = &lbl_1_bss_28[player];
    HuVecF posXZ;
    int i;

    posXZ.x = pos->x;
    posXZ.y = 0.0f;
    posXZ.z = pos->z;
    for (i = 0; i < 32; i++) {
        if (lbl_1_bss_2AC[i][player].unk0 == 0 && fn_1_6EE4(&posXZ, &lbl_1_data_248[i].pos, 90.0f)) {
            if (lbl_1_data_248[i].unkC == 1 && pos->y < 160.0f) {
                return i;
            }
            if (lbl_1_data_248[i].unkC == 0 && pos->y < 180.0f) {
                return i;
            }
        }
    }
    return -1;
}

s16 fn_1_6EE4(HuVecF *a, HuVecF *b, float radius)
{
    float dist = VECSquareDistance(a, b);

    if (dist < radius * radius) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void fn_1_6F54(void)
{
    int i;
    int j;

    for (i = 0; i < 32; i++) {
        for (j = 0; j < 4; j++) {
            lbl_1_bss_2AC[i][j].unk0 = 2;
        }
    }
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 4; j++) {
            lbl_1_bss_2AC[i][j].unk0 = 3;
        }
    }
}

s16 fn_1_7000(s16 player, HuVecF *pos, float angle, s16 no)
{
    HuVecF dir;
    HuVecF ofs;
    HuVecF proj;
    HuVecF near;
    float dist;
    float t;
    float projLen;
    float width;

    if (no >= 32) {
        return FALSE;
    }
    dir.x = 800.0 * HuSin(angle);
    dir.y = 0.0f;
    dir.z = 800.0 * HuCos(angle);
    ofs.x = lbl_1_data_248[no].pos.x - pos->x;
    ofs.y = 0.0f;
    ofs.z = lbl_1_data_248[no].pos.z - pos->z;
    t = VECDotProduct(&dir, &ofs) / VECSquareMag(&dir);
    proj.x = t * dir.x;
    proj.y = 0.0f;
    proj.z = t * dir.z;
    projLen = sqrtf(VECSquareMag(&proj));
    near.x = pos->x + proj.x;
    near.y = 0.0f;
    near.z = pos->z + proj.z;
    dir.x = lbl_1_data_248[no].pos.x - near.x;
    dir.y = 0.0f;
    dir.z = lbl_1_data_248[no].pos.z - near.z;
    dist = sqrtf(VECSquareMag(&dir));
    projLen = (800.0f - projLen) / 800.0f;
    if (projLen < 0.0f) {
        projLen = 0.0f;
    }
    if (dir.x < 0.0f) {
        width = 150.0f + 45.0f * projLen;
    } else {
        width = 150.0f - 45.0f * projLen;
    }
    if (dist <= width) {
        return TRUE;
    } else {
        return FALSE;
    }
}
