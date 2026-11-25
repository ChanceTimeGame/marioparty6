/* @@HEAD@@ */
/* Part 4's provisional HEAD (wt-b404): only what part 4's functions need to compile and score
   on their own. A splice replaces it with part 1's HEAD. */
#include "game/board/capsule.h"
#include "game/board/camera.h"
#include "game/board/effect.h"
#include "game/board/exit.h"
#include "game/board/math.h"
#include "game/board/object.h"
#include "game/board/player.h"
#include "game/board/window.h"
#include "game/data.h"
#include "game/gamework.h"
#include "game/hsfformat.h"
#include "game/hu3d.h"
#include "game/main.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/sprite.h"
#include "game/window.h"
#include "humath.h"
#include "math.h"
#include "string.h"

typedef struct CapsuleData_s {
    int fileNum; /* 0x00 */
    int objFileNum; /* 0x04 */
    int unk08[3];
    int colorNo; /* 0x14 */
    int unk18[4];
} CAPSULE_DATA; /* size 0x28 */

typedef struct CapsuleObjColor_s {
    s16 useF; /* 0x00 */
    s16 objId; /* 0x02 */
    s16 unk04; /* 0x04 */
    u8 layer; /* 0x06 */
    HuVecF pos; /* 0x08 */
    HuVecF rot; /* 0x14 */
    HuVecF scale; /* 0x20 */
} CAPSULE_OBJCOLOR; /* size 0x2C */

#define CAPSULE_OBJCOLOR_MAX 128

typedef struct CapsuleEffect_s CAPSULE_EFFECT;
typedef void (*CAPSULE_EFFHOOK)(HU3D_MODEL *modelP, CAPSULE_EFFECT *effP, Mtx *matrix);

#define CAPSULE_EFF_ATTR_NONE 0
#define CAPSULE_EFF_ATTR_COUNTER_RESET (1 << 0)
#define CAPSULE_EFF_ATTR_COUNTER_UPDATE (1 << 1)

#define CAPSULE_EFF_DISPATTR_NONE 0
#define CAPSULE_EFF_DISPATTR_ZBUF_OFF (1 << 0)
#define CAPSULE_EFF_DISPATTR_NOANIM (1 << 1)
#define CAPSULE_EFF_DISPATTR_CAMERA_ROT (1 << 2)
#define CAPSULE_EFF_DISPATTR_ROT3D (1 << 3)

typedef struct CapsuleEffData_s {
    s16 time; /* 0x00 */
    s16 work; /* 0x02 */
    s16 mode; /* 0x04 */
    s16 cameraBit; /* 0x06 */
    HuVecF vel; /* 0x08 */
    float baseAlpha; /* 0x14 */
    float tpLvl; /* 0x18 */
    float speed; /* 0x1C */
    float unk20; /* 0x20 */
    float gravity; /* 0x24 */
    float rotSpeed; /* 0x28 */
    float animTime; /* 0x2C */
    float animSpeed; /* 0x30 */
    float scale; /* 0x34 */
    HuVecF rot; /* 0x38 */
    HuVecF pos; /* 0x44 */
    GXColor color; /* 0x50 */
    int no; /* 0x54 */
} CAPSULE_EFFDATA; /* size 0x58 */

struct CapsuleEffect_s {
    s16 mode; /* 0x00 */
    s16 time; /* 0x02 */
    HuVecF vel; /* 0x04 */
    s16 work[8]; /* 0x10 */
    u8 blendMode; /* 0x20 */
    u8 attr; /* 0x21 */
    u8 dispAttr; /* 0x22 */
    u8 unk23; /* 0x23 */
    HU3D_MODELID modelId; /* 0x24 */
    s16 num; /* 0x26 */
    u32 count; /* 0x28 */
    u32 prevCounter; /* 0x2C */
    u32 prevCount; /* 0x30 */
    u32 dlSize; /* 0x34 */
    ANIMDATA *anim; /* 0x38 */
    CAPSULE_EFFDATA *data; /* 0x3C */
    HuVecF *vertex; /* 0x40 */
    HuVec2f *st; /* 0x44 */
    void *dl; /* 0x48 */
    CAPSULE_EFFHOOK hook; /* 0x4C */
    HU3D_MODEL *hookMdlP; /* 0x50 */
}; /* size 0x54 */

typedef struct CapEffHiliteWork_s {
    int mdlId[3]; /* 0x00 */
    int num; /* 0x0C */
    ANIMDATA *anim[3]; /* 0x10 */
} CAPEFF_HILITE_WORK; /* size 0x1C */

typedef struct CapEffCrackData_s {
    int dispF; /* 0x00 */
    float scale; /* 0x04 */
    float rot; /* 0x08 */
    float rotSpeed; /* 0x0C */
    float fadeSpeed; /* 0x10 */
    int delay; /* 0x14 */
    HuVecF basePos; /* 0x18 */
    HuVecF baseVtx[3]; /* 0x24 */
    HuVecF vel; /* 0x48 */
    HuVecF pos; /* 0x54 */
    HuVecF vtx[3]; /* 0x60 */
    HuVec2f st[3]; /* 0x84 */
    GXColor color; /* 0x9C */
} CAPEFF_CRACK_DATA; /* size 0xA0 */

typedef struct CapEffCrackWork_s {
    int mdlId; /* 0x00 */
    int mode; /* 0x04 */
    int time; /* 0x08 */
    int num; /* 0x0C */
    int vtxNum; /* 0x10 */
    int unk14; /* 0x14 */
    HuVecF *vtx; /* 0x18 */
    HuVec2f *st; /* 0x1C */
    CAPEFF_CRACK_DATA *data; /* 0x20 */
    ANIMDATA *anim; /* 0x24 */
    GXColor color; /* 0x28 */
    u32 dlSize; /* 0x2C */
    void *dl; /* 0x30 */
} CAPEFF_CRACK_WORK; /* size 0x34 */

#define CAPEFF_TRAIL_MAX 12

typedef struct CapEffTrailSeg_s {
    HuVecF start; /* 0x00 */
    HuVecF end; /* 0x0C */
    float len; /* 0x18 */
    float ofs; /* 0x1C */
} CAPEFF_TRAIL_SEG; /* size 0x20 */

static CAPSULE_DATA capsuleData[60] = { { 1 } }; /* placeholder: part 1 owns the table */
static int capsuleColObjId = -1;
static int capsuleColMdlId = -1;
static int capsuleComChoice;
static OMOBJ *capEffHiliteOMObj;
static OMOBJ *capEffCrackOMObj;
static OMOBJ *capEffTrailOMObj;
static CAPSULE_OBJCOLOR capsuleObjColorData[CAPSULE_OBJCOLOR_MAX];
static float capsuleTime[8];
static float capsuleBezierX[8];
static float capsuleBezierY[8];
static float capsuleBezierZ[8];

static HuVecF capsuleCrackScaleTbl[2][3] = {
    { { 0, 0, 0 }, { 1, 0, 0 }, { 0, 0, 1 } },
    { { 0, 0, 1 }, { 1, 0, 0 }, { 1, 0, 1 } }
};
static GXColor capsuleTrailColorTbl[4][2] = {
    { { 0x80, 0xFF, 0x80, 0xFF }, { 0xC0, 0xFF, 0xC0, 0xFF } },
    { { 0xFF, 0xFF, 0x80, 0xFF }, { 0xFF, 0xFF, 0xC0, 0xFF } },
    { { 0xFF, 0x80, 0x80, 0xFF }, { 0xFF, 0xC0, 0xC0, 0xFF } },
    { { 0x80, 0xFF, 0xFF, 0xFF }, { 0xC0, 0xFF, 0xFF, 0xFF } }
};
static GXColor capsuleCrackEffColor = { 0xFF, 0xFF, 0x80, 0x40 };
static GXColor capsuleCrackEffAmbColor = { 0xFF, 0xFF, 0xFF, 0xFF };
static GXColor capsuleCrackEffMatColor = { 0xFF, 0xFF, 0xFF, 0xFF };

/* Stand-ins for earlier parts' globals that part 4's functions inline (bodies follow the asm). */
static inline int CapNoGet(s16 capsuleNo)
{
    return (s16)(u8)capsuleNo;
}

int mbCapColorGet(int capsuleNo)
{
    capsuleNo = CapNoGet(capsuleNo);
    return capsuleData[capsuleNo].colorNo;
}

static inline int CapObjColorNoGet(int id)
{
    CAPSULE_OBJCOLOR *objColorP = capsuleObjColorData;
    int i;
    for(i=0; i<CAPSULE_OBJCOLOR_MAX; i++, objColorP++) {
        if(objColorP->useF != 0 && objColorP->objId == id) {
            break;
        }
    }
    if(i < CAPSULE_OBJCOLOR_MAX) {
        return i;
    } else {
        return -1;
    }
}

void mbCapObjColorKill(int id)
{
    CAPSULE_OBJCOLOR *objColorP;
    int no;
    if((no = CapObjColorNoGet(id)) != -1) {
        objColorP = &capsuleObjColorData[no];
        if(objColorP->useF != 0) {
            mbObjKill(objColorP->objId);
            objColorP->objId = -1;
            objColorP->unk04 = -1;
            objColorP->useF = 0;
        }
    }
}

void mbCapObjColorPosSet(int id, float x, float y, float z)
{
    CAPSULE_OBJCOLOR *objColorP;
    int no;
    if((no = CapObjColorNoGet(id)) != -1) {
        objColorP = &capsuleObjColorData[no];
        objColorP->pos.x = x;
        objColorP->pos.y = y;
        objColorP->pos.z = z;
        mbObjPosSetV(objColorP->objId, &objColorP->pos);
    }
}

void mbCapObjColorPosSetV(int id, HuVecF *pos)
{
    CAPSULE_OBJCOLOR *objColorP;
    int no;
    if((no = CapObjColorNoGet(id)) != -1) {
        objColorP = &capsuleObjColorData[no];
        mbCapObjColorPosSet(id, pos->x, pos->y, pos->z);
    }
}

/* @@PART1@@ */
/* @@PART2@@ */
/* @@PART3@@ */
/* @@PART4@@ */
/* Part 4 (wt-b404): CapEffHiliteOMExec .. CapColExec. */

int abs(int x);

static void CapEffCrackOMExec(OMOBJ *obj);
static void CapEffCrackDraw(HU3D_MODEL *modelP, Mtx *mtx);
static void CapEffTrailOMExec(OMOBJ *obj);
static void CapEffDraw(HU3D_MODEL *modelP, Mtx *mtx);
static void CapComKeyHook(void);

static inline void *CapEffHeapMalloc(u32 size)
{
    return HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
}

static inline void *CapEffModelMalloc(u32 size, u32 num)
{
    return HuMemDirectMallocNum(HEAP_MODEL, size, num);
}

static inline float CapEffSqrtf(float x)
{
    const double _half = .5;
    const double _three = 3.0;
    volatile float y;
    if(x > 0.0f) {
        double guess = __frsqrte((double)x);
        guess = _half*guess*(_three-guess*guess*x);
        guess = _half*guess*(_three-guess*guess*x);
        guess = _half*guess*(_three-guess*guess*x);
        y = (float)(x*guess);
        return y;
    }
    return x;
}

static inline float CapEffSinDeg(float deg)
{
    float result = mbSinDeg(deg);
    return result;
}

static inline float CapEffCosDeg(float deg)
{
    float result = mbCosDeg(deg);
    return result;
}

#define CAPEFF_RANDF() ((1.0f/268435456.0f)*mbRandMod(0x10000000))

static void CapEffHiliteOMExec(OMOBJ *obj)
{
    CAPEFF_HILITE_WORK *work;
    HU3D_MODEL *modelP;
    CAPSULE_EFFECT *effP;
    CAPSULE_EFFDATA *effDataP;
    int i;
    int j;
    float t;

    work = obj->data;
    if(mbExitCheck() || capEffHiliteOMObj == NULL) {
        for(i=0; i<3; i++) {
            Hu3DModelKill(work->mdlId[i]);
            work->mdlId[i] = HU3D_MODELID_NONE;
        }
        for(i=0; i<3; i++) {
            HuSprAnimKill(work->anim[i]);
            work->anim[i] = NULL;
        }
        omDelObjEx(mbObjMan, obj);
        capEffHiliteOMObj = NULL;
        return;
    }
    for(i=0; i<3; i++) {
        if(work->num <= 0) {
            Hu3DModelAttrSet(work->mdlId[i], HU3D_ATTR_DISPOFF);
        } else {
            Hu3DModelAttrReset(work->mdlId[i], HU3D_ATTR_DISPOFF);
            modelP = &Hu3DData[work->mdlId[i]];
            effP = modelP->hookData;
            effDataP = effP->data;
            effP->unk23 = 0;
            for(j=0; j<effP->num; j++, effDataP++) {
                if(effDataP->scale <= 0.0f) {
                    continue;
                }
                switch(effDataP->time) {
                    case 0:
                        if(effDataP->baseAlpha > 0.0f) {
                            t = (++effDataP->work)/effDataP->baseAlpha;
                        } else {
                            t = 1.0f;
                        }
                        switch(effDataP->mode) {
                            case 0:
                                t = t;
                                break;
                            case 1:
                                t = HuSin(90.0f*t);
                                break;
                            case 2:
                                t = HuCos(90.0f*(1.0f-t));
                                break;
                        }
                        effDataP->scale = effDataP->vel.z*(effDataP->vel.x+(t*(1.0f-effDataP->vel.x)));
                        effDataP->color.a = effDataP->speed*t;
                        if(t >= 1.0f) {
                            effDataP->scale = effDataP->vel.z;
                            effDataP->color.a = effDataP->speed;
                            effDataP->time++;
                            effDataP->work = 0;
                        }
                        break;

                    case 1:
                        if(effDataP->tpLvl > 0.0f) {
                            t = (++effDataP->work)/effDataP->tpLvl;
                        } else {
                            t = 1.0f;
                        }
                        switch(effDataP->mode) {
                            case 0:
                                t = t;
                                break;
                            case 1:
                                t = HuSin(90.0f*t);
                                break;
                            case 2:
                                t = HuCos(90.0f*(1.0f-t));
                                break;
                        }
                        effDataP->scale = effDataP->vel.z*(1.0f+(t*(effDataP->vel.y-1.0f)));
                        effDataP->color.a = effDataP->speed*(1.0f-t);
                        if(t >= 1.0f) {
                            effDataP->scale = 0.0f;
                            work->num--;
                        }
                        break;
                }
            }
        }
    }
}

static void CapEffHiliteKill(void)
{
    capEffHiliteOMObj = NULL;
}

static int CapEffHiliteAdd(HuVecF *pos, HuVecF *rot, HuVecF *scale, int inTime, int outTime, int no, int mode, GXColor *color)
{
    OMOBJ *obj;
    CAPEFF_HILITE_WORK *work;
    HU3D_MODEL *modelP;
    CAPSULE_EFFECT *effP;
    CAPSULE_EFFDATA *effDataP;
    int i;
    if(capEffHiliteOMObj == NULL) {
        return -1;
    }
    obj = capEffHiliteOMObj;
    work = obj->data;
    modelP = &Hu3DData[work->mdlId[no%3]];
    effP = modelP->hookData;
    effDataP = effP->data;
    for(i=0; i<effP->num; i++, effDataP++) {
        if(effDataP->scale <= 0.0f) {
            break;
        }
    }
    if(i >= effP->num) {
        return -1;
    }
    effDataP->time = effDataP->work = 0;
    effDataP->pos.x = pos->x;
    effDataP->pos.y = pos->y;
    effDataP->pos.z = pos->z;
    effDataP->vel.x = scale->x;
    effDataP->vel.y = scale->y;
    effDataP->vel.z = scale->z;
    effDataP->baseAlpha = inTime;
    effDataP->tpLvl = outTime;
    effDataP->speed = color->a;
    effDataP->scale = scale->z;
    effDataP->color.r = color->r;
    effDataP->color.g = color->g;
    effDataP->color.b = color->b;
    effDataP->color.a = color->a;
    effDataP->rot.x = rot->x;
    effDataP->rot.y = rot->y;
    effDataP->rot.z = rot->z;
    effDataP->no = 0;
    effDataP->time = 0;
    effDataP->mode = (s16)mode;
    work->num++;
    return i;
}

static void CapEffCrackCreate(void)
{
    OMOBJ *obj;
    CAPEFF_CRACK_WORK *work;
    HU3D_MODEL *modelP;
    CAPEFF_CRACK_DATA *dataP;
    int i;
    int j;
    int k;
    int l;
    float stepX;
    float stepZ;
    float x;
    float z;
    float scale;
    float radius;
    float height;
    float dist;
    void *dlBuf;
    void *dlBegin;
    float vtxScale[3];
    HuVecF center;

    obj = capEffCrackOMObj = omAddObjEx(mbObjMan, 32768, 0, 0, -1, CapEffCrackOMExec);
    obj->data = work = CapEffHeapMalloc(sizeof(CAPEFF_CRACK_WORK));
    memset(work, 0, sizeof(CAPEFF_CRACK_WORK));
    work->mdlId = Hu3DHookFuncCreate(CapEffCrackDraw);
    Hu3DModelCameraSet(work->mdlId, HU3D_CAM0);
    Hu3DModelLayerSet(work->mdlId, 3);
    modelP = &Hu3DData[work->mdlId];
    modelP->hookData = work;
    work->mode = 0;
    work->time = 0;
    work->num = 24*24*2;
    work->vtxNum = work->num*3;
    work->data = dataP = CapEffModelMalloc(work->num*sizeof(CAPEFF_CRACK_DATA), modelP->mallocNo);
    memset(dataP, 0, work->num*sizeof(CAPEFF_CRACK_DATA));
    work->vtx = CapEffModelMalloc(work->vtxNum*sizeof(HuVecF), modelP->mallocNo);
    memset(work->vtx, 0, work->vtxNum*sizeof(HuVecF));
    work->st = CapEffModelMalloc(work->vtxNum*sizeof(HuVec2f), modelP->mallocNo);
    memset(work->st, 0, work->vtxNum*sizeof(HuVec2f));
    work->anim = HuSprAnimRead(HuDataReadNum(0xC002B, HU_MEMNUM_OVL));
    work->color.r = 0;
    work->color.g = 0;
    work->color.b = 0;
    work->color.a = 0;
    stepX = 8.333334f;
    stepZ = 8.333334f;
    x = -100.0f;
    z = -100.0f;
    for(i=0; i<24; i++) {
        for(j=0; j<24; j++) {
            for(k=0; k<2; k++) {
                dataP->dispF = TRUE;
                dataP->scale = 1.0f;
                dataP->rot = 0.0f;
                if(mbRandMod(0x8000) & 1) {
                    dataP->rotSpeed = 5.0f+(2.0f*CAPEFF_RANDF());
                } else {
                    dataP->rotSpeed = -(5.0f+(2.0f*CAPEFF_RANDF()));
                }
                dataP->fadeSpeed = 0.035f+(0.035f*CAPEFF_RANDF());
                dataP->delay = mbRandMod(0x8000) & 0xF;
                for(l=0; l<3; l++) {
                    dataP->vtx[l].x = x+(stepX*capsuleCrackScaleTbl[k][l].x);
                    dataP->vtx[l].z = z+(stepZ*capsuleCrackScaleTbl[k][l].z);
                    dataP->vtx[l].y = 0.0f;
                    scale = CapEffSqrtf((dataP->vtx[l].x*dataP->vtx[l].x)+(dataP->vtx[l].z*dataP->vtx[l].z))/100.0f;
                    vtxScale[l] = scale;
                    if(vtxScale[l] > 1.0f) {
                        scale = 1.0f;
                        if(VECMag(&dataP->vtx[l]) > 0.0f) {
                            VECNormalize(&dataP->vtx[l], &dataP->vtx[l]);
                        }
                        VECScale(&dataP->vtx[l], &dataP->vtx[l], 100.0f);
                    }
                    radius = 100.0f;
                    height = radius*scale;
                    dataP->vtx[l].y = CapEffSqrtf((radius*radius)-(height*height));
                    dataP->st[l].x = (dataP->vtx[l].x+100.0f)/200.0f;
                    dataP->st[l].y = (dataP->vtx[l].z+100.0f)/200.0f;
                }
                center = dataP->vtx[0];
                VECAdd(&center, &dataP->vtx[1], &center);
                VECAdd(&center, &dataP->vtx[2], &center);
                VECScale(&center, &dataP->pos, 0.333333f);
                dist = CapEffSqrtf((dataP->pos.x*dataP->pos.x)+(dataP->pos.z*dataP->pos.z))/100.0f;
                dataP->delay = mbRandMod(0x8000) & 0x7;
                if(VECMag(&dataP->pos) > 0.0f) {
                    VECNormalize(&dataP->pos, &dataP->vel);
                }
                VECScale(&dataP->vel, &dataP->vel, 10.0f*(0.5f+(0.5f*CAPEFF_RANDF())));
                dataP->basePos = dataP->pos;
                for(l=0; l<3; l++) {
                    VECSubtract(&dataP->vtx[l], &dataP->pos, &dataP->vtx[l]);
                    dataP->baseVtx[l] = dataP->vtx[l];
                }
                dataP->color.r = capsuleCrackEffColor.r;
                dataP->color.g = capsuleCrackEffColor.g;
                dataP->color.b = capsuleCrackEffColor.b;
                dataP->color.a = capsuleCrackEffColor.a;
                if(vtxScale[0] > 1.0f && vtxScale[1] > 1.0f && vtxScale[2] > 1.0f) {
                    dataP->dispF = FALSE;
                    dataP->scale = 0.0f;
                }
                dataP++;
            }
            x += stepX;
        }
        x = -100.0f;
        z += stepZ;
    }
    dlBegin = dlBuf = CapEffModelMalloc(0x10000, modelP->mallocNo);
    DCFlushRange(dlBuf, 0x10000);
    GXBeginDisplayList(dlBegin, 0x10000);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, work->vtxNum);
    for(i=0; i<work->vtxNum/3; i++) {
        GXPosition1x16(i*3);
        GXColor1x16(i);
        GXTexCoord1x16(i*3);

        GXPosition1x16((i*3)+1);
        GXColor1x16(i);
        GXTexCoord1x16((i*3)+1);

        GXPosition1x16((i*3)+2);
        GXColor1x16(i);
        GXTexCoord1x16((i*3)+2);
    }
    GXEnd();
    work->dlSize = GXEndDisplayList();
    work->dlSize > 0x10000;
    work->dl = CapEffModelMalloc(work->dlSize, modelP->mallocNo);
    memcpy(work->dl, dlBuf, work->dlSize);
    DCFlushRange(work->dl, work->dlSize);
    HuMemDirectFree(dlBuf);
}

static void CapEffCrackOMExec(OMOBJ *obj)
{
    CAPEFF_CRACK_WORK *work;
    CAPEFF_CRACK_DATA *dataP;
    int i;
    float t;
    int unused = -1;

    work = obj->data;
    if(mbExitCheck() || capEffCrackOMObj == NULL) {
        HuSprAnimKill(work->anim);
        Hu3DModelKill(work->mdlId);
        omDelObjEx(mbObjMan, obj);
        capEffCrackOMObj = NULL;
        return;
    }
    dataP = work->data;
    switch(work->mode) {
        case 0:
            break;

        case 1:
            for(i=0, dataP=work->data; i<work->num; i++, dataP++) {
                dataP->color.a = 64;
                dataP->pos.y = 0.0f;
                dataP->vtx[0].y = 0.0f;
                dataP->vtx[1].y = 0.0f;
                dataP->vtx[2].y = 0.0f;
            }
            Hu3DModelScaleSet(work->mdlId, 0.0f, 1.0f, 0.0f);
            work->time = 0;
            work->mode++;
            break;

        case 2:
            t = (++work->time)/6.0f;
            if(t >= 1.0f) {
                t = 1.0f;
            }
            Hu3DModelScaleSet(work->mdlId, t, 1.0f, t);
            if(work->time >= 6.0f) {
                work->time = 0;
                work->mode++;
            }
            break;

        case 3:
            t = (1.0f/18.0f)*(++work->time);
            if(t >= 1.0f) {
                t = 1.0f;
            }
            for(i=0, dataP=work->data; i<work->num; i++, dataP++) {
                dataP->color.a = 64.0f+(191.0f*t);
                dataP->pos.y = dataP->basePos.y*t;
                dataP->vtx[0].y = dataP->baseVtx[0].y*t;
                dataP->vtx[1].y = dataP->baseVtx[1].y*t;
                dataP->vtx[2].y = dataP->baseVtx[2].y*t;
            }
            work->color.r = work->color.g = work->color.b = 64.0f*t;
            work->color.a = 255.0f*t;
            if(work->time >= 18.0f) {
                work->time = 0;
                work->mode++;
            }
            break;

        case 4:
            if(work->color.a > 20) {
                work->color.a -= 20;
            } else {
                work->color.a = 0;
            }
            for(i=0, dataP=work->data; i<work->num; i++, dataP++) {
                if(dataP->dispF == FALSE) {
                    continue;
                }
                if(dataP->delay != 0) {
                    dataP->delay--;
                    continue;
                }
                VECAdd(&dataP->pos, &dataP->vel, &dataP->pos);
                dataP->rot += dataP->rotSpeed;
                if((dataP->scale -= dataP->fadeSpeed) <= 0.0f) {
                    dataP->dispF = FALSE;
                } else if(dataP->scale > 1.0f) {
                    dataP->color.a = 255;
                } else {
                    dataP->color.a = 255.0f*dataP->scale;
                }
            }
            break;

        case 5:
            break;
    }
}

static void CapEffCrackKill(void)
{
    capEffCrackOMObj = NULL;
}

static void CapEffCrackAdd(HuVecF *pos, HuVecF *rot)
{
    OMOBJ *obj = capEffCrackOMObj;
    CAPEFF_CRACK_WORK *work;
    if(capEffCrackOMObj != NULL) {
        work = obj->data;
        work->mode = 1;
        if(pos != NULL) {
            obj->trans.x = pos->x;
            obj->trans.y = pos->y;
            obj->trans.z = pos->z;
        } else {
            obj->trans.x = obj->trans.y = obj->trans.z = 0.0f;
        }
        if(rot != NULL) {
            obj->rot.x = rot->x;
            obj->rot.y = rot->y;
            obj->rot.z = rot->z;
        } else {
            obj->rot.x = obj->rot.y = obj->rot.z = 0.0f;
        }
        Hu3DModelPosSet(work->mdlId, obj->trans.x, obj->trans.y, obj->trans.z);
        Hu3DModelRotSet(work->mdlId, obj->rot.x, obj->rot.y, obj->rot.z);
    }
}

static void CapEffCrackDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    CAPEFF_CRACK_WORK *work;
    CAPEFF_CRACK_DATA *dataP;
    HuVecF *vtx;
    HuVec2f *st;
    int i;
    int j;
    float sinRot;
    float cosRot;
    GXColor tevColor;
    GXColor ambColor;
    GXColor matColor;
    HuVecF scaleVtx;

    work = modelP->hookData;
    dataP = work->data;
    if(work->mode == 0) {
        return;
    }
    GXLoadPosMtxImm(*mtx, GX_PNMTX0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    if(shadowModelDrawF) {
        return;
    }
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    tevColor = work->color;
    GXSetTevColor(GX_TEVREG2, tevColor);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_RASC, GX_CC_C2);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_A2);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    ambColor = capsuleCrackEffMatColor;
    GXSetChanAmbColor(GX_COLOR0A0, ambColor);
    matColor = capsuleCrackEffAmbColor;
    GXSetChanMatColor(GX_COLOR0A0, matColor);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    HuSprTexLoad(work->anim, 0, GX_TEXMAP0, GX_REPEAT, GX_REPEAT, GX_LINEAR);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetZCompLoc(GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
    GXSetCullMode(GX_CULL_BACK);
    vtx = work->vtx;
    st = work->st;
    for(i=0; i<work->num; i++, dataP++) {
        if(dataP->dispF != FALSE) {
            if(dataP->rot == 0.0f) {
                for(j=0; j<3; j++) {
                    VECScale(&dataP->vtx[j], &scaleVtx, dataP->scale);
                    VECAdd(&scaleVtx, &dataP->pos, vtx);
                    vtx++;
                    *st++ = dataP->st[j];
                }
            } else {
                sinRot = CapEffSinDeg(dataP->rot);
                cosRot = CapEffCosDeg(dataP->rot);
                for(j=0; j<3; j++) {
                    VECScale(&dataP->vtx[j], &scaleVtx, dataP->scale);
                    vtx->x = dataP->pos.x+(dataP->vtx[j].x*cosRot)-(dataP->vtx[j].y*sinRot);
                    vtx->y = dataP->pos.y+(dataP->vtx[j].x*sinRot)+(dataP->vtx[j].y*cosRot);
                    vtx->z = dataP->pos.z+dataP->vtx[j].z;
                    vtx++;
                    *st++ = dataP->st[j];
                }
            }
        } else {
            vtx->x = vtx->y = vtx->z = 0.0f;
            vtx++;
            vtx->x = vtx->y = vtx->z = 0.0f;
            vtx++;
            vtx->x = vtx->y = vtx->z = 0.0f;
            vtx++;
            st += 3;
        }
    }
    dataP = work->data;
    DCFlushRangeNoSync(work->vtx, work->vtxNum*sizeof(HuVecF));
    DCFlushRangeNoSync(work->st, work->vtxNum*sizeof(HuVec2f));
    DCFlushRangeNoSync(work->data, work->num*sizeof(CAPEFF_CRACK_DATA));
    PPCSync();
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetArray(GX_VA_POS, work->vtx, sizeof(HuVecF));
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetArray(GX_VA_CLR0, &work->data->color, sizeof(CAPEFF_CRACK_DATA));
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetArray(GX_VA_TEX0, work->st, sizeof(HuVec2f));
    GXCallDisplayList(work->dl, work->dlSize);
}

static void CapEffTrailCreate(int capsuleNo)
{
    OMOBJ *obj;
    HuVecF *posBuf;
    HU3D_MODEL *modelP;
    MBPARTICLE *particleP;
    MBPARTICLEDATA *particleDataP;
    int i;

    capsuleNo = CapNoGet(capsuleNo);
    obj = capEffTrailOMObj = omAddObjEx(mbObjMan, 32768, CAPEFF_TRAIL_MAX+1, 0, -1, CapEffTrailOMExec);
    posBuf = obj->data = CapEffHeapMalloc(CAPEFF_TRAIL_MAX*sizeof(HuVecF));
    memset(posBuf, 0, CAPEFF_TRAIL_MAX*sizeof(HuVecF));
    for(i=0; i<CAPEFF_TRAIL_MAX; i++) {
        obj->mdlId[i] = HU3D_MODELID_NONE;
        posBuf[i].x = posBuf[i].y = posBuf[i].z = 0.0f;
    }
    obj->mdlId[CAPEFF_TRAIL_MAX] = mbParticleCreate(HuSprAnimRead(HuDataReadNum(0xC002A, HU_MEMNUM_OVL)), 24);
    Hu3DModelCameraSet(obj->mdlId[CAPEFF_TRAIL_MAX], HU3D_CAM0);
    Hu3DModelLayerSet(obj->mdlId[CAPEFF_TRAIL_MAX], 5);
    modelP = &Hu3DData[obj->mdlId[CAPEFF_TRAIL_MAX]];
    particleP = modelP->hookData;
    particleDataP = particleP->data;
    particleP->blendMode = MB_PARTICLE_BLEND_ADDCOL;
    for(i=0; i<particleP->num; i++, particleDataP++) {
        particleDataP->scale = 0.0f;
        particleDataP->pos.x = particleDataP->pos.y = particleDataP->pos.z = 0.0f;
    }
    obj->stat |= OM_STAT_MODELPAUSE;
    obj->work[0] = FALSE;
    obj->work[1] = 0;
}

static void CapEffTrailOMExec(OMOBJ *obj)
{
    HuVecF *posBuf;
    HU3D_MODEL *modelP;
    MBPARTICLE *particleP;
    MBPARTICLEDATA *particleDataP;
    CAPEFF_TRAIL_SEG *segP;
    int i;
    int j;
    int head;
    int no;
    int next;
    float total;
    float dist;
    float t;
    HuVecF dir;
    HuVecF trailPos[CAPEFF_TRAIL_MAX];
    CAPEFF_TRAIL_SEG seg[CAPEFF_TRAIL_MAX];

    posBuf = obj->data;
    if(mbExitCheck() || capEffTrailOMObj == NULL) {
        for(i=0; i<CAPEFF_TRAIL_MAX; i++) {
            if(obj->mdlId[i] != HU3D_MODELID_NONE) {
                mbCapObjColorKill(obj->mdlId[i]);
            }
            obj->mdlId[i] = HU3D_MODELID_NONE;
        }
        mbParticleKill(obj->mdlId[CAPEFF_TRAIL_MAX]);
        obj->mdlId[CAPEFF_TRAIL_MAX] = HU3D_MODELID_NONE;
        omDelObjEx(mbObjMan, obj);
        capEffTrailOMObj = NULL;
        return;
    }
    if(!obj->work[0]) {
        return;
    }
    head = obj->work[1];
    posBuf[head].x = obj->trans.x;
    posBuf[head].y = obj->trans.y;
    posBuf[head].z = obj->trans.z;
    segP = seg;
    total = 0.0f;
    for(i=0; i<CAPEFF_TRAIL_MAX-1; i++, segP++) {
        no = i+obj->work[1];
        if(no >= CAPEFF_TRAIL_MAX) {
            no -= CAPEFF_TRAIL_MAX;
        }
        next = no+1;
        if(next >= CAPEFF_TRAIL_MAX) {
            next -= CAPEFF_TRAIL_MAX;
        }
        VECSubtract(&posBuf[no], &posBuf[next], &dir);
        segP->start = posBuf[no];
        segP->end = posBuf[next];
        segP->len = VECMag(&dir);
        segP->ofs = total;
        total += segP->len;
    }
    for(i=0; i<CAPEFF_TRAIL_MAX; i++) {
        if(total <= 0.0f) {
            dist = 0.0f;
        } else if(total <= 120.0f) {
            dist = (total/12.0f)*i;
        } else {
            dist = 10.0f*i;
        }
        for(j=0, segP=seg; j<CAPEFF_TRAIL_MAX-1; j++, segP++) {
            if(dist < segP->ofs+segP->len) {
                break;
            }
        }
        if(j < CAPEFF_TRAIL_MAX-1) {
            dist -= segP->ofs;
            if(segP->len > 0.0f) {
                t = dist/segP->len;
                VECSubtract(&segP->end, &segP->start, &dir);
                VECScale(&dir, &dir, t);
                VECAdd(&segP->start, &dir, &trailPos[i]);
            } else {
                trailPos[i] = segP->start;
            }
        } else {
            trailPos[i] = seg[CAPEFF_TRAIL_MAX-2].end;
        }
    }
    for(i=0; i<CAPEFF_TRAIL_MAX; i++) {
        if(obj->mdlId[i] != HU3D_MODELID_NONE) {
            mbCapObjColorPosSetV(obj->mdlId[i], &trailPos[i]);
        }
    }
    modelP = &Hu3DData[obj->mdlId[CAPEFF_TRAIL_MAX]];
    particleP = modelP->hookData;
    particleDataP = particleP->data;
    posBuf = obj->data;
    for(i=0; i<particleP->num/2; i++, particleDataP++) {
        particleDataP->pos.x = trailPos[i].x;
        particleDataP->pos.y = trailPos[i].y;
        particleDataP->pos.z = trailPos[i].z;
    }
    for(i=0; i<particleP->num/2; i++, particleDataP++) {
        particleDataP->pos.x = trailPos[i].x;
        particleDataP->pos.y = trailPos[i].y;
        particleDataP->pos.z = trailPos[i].z;
    }
    head--;
    if(head < 0) {
        head = CAPEFF_TRAIL_MAX-1;
    }
    obj->work[1] = head;
}

static void CapEffTrailKill(void)
{
    capEffTrailOMObj = NULL;
}

static void CapEffTrailAdd(HuVecF *pos, int capsuleNo)
{
    OMOBJ *obj = capEffTrailOMObj;
    HuVecF *posBuf;
    HU3D_MODEL *modelP;
    MBPARTICLE *particleP;
    MBPARTICLEDATA *particleDataP;
    int i;
    float t;
    GXColor color;
    if(capEffTrailOMObj != NULL) {
        posBuf = obj->data;
        for(i=0; i<CAPEFF_TRAIL_MAX; i++) {
            posBuf[i] = *pos;
        }
        modelP = &Hu3DData[obj->mdlId[CAPEFF_TRAIL_MAX]];
        particleP = modelP->hookData;
        particleDataP = particleP->data;
        posBuf = obj->data;
        color = capsuleTrailColorTbl[mbCapColorGet(capsuleNo)][0];
        for(i=0; i<particleP->num/2; i++, particleDataP++) {
            t = i/12.0f;
            particleDataP->scale = 100.0f*(0.9f+(-0.49999997f*t));
            particleDataP->pos.x = pos->x;
            particleDataP->pos.y = pos->y;
            particleDataP->pos.z = pos->z;
            particleDataP->color.r = color.r;
            particleDataP->color.g = color.g;
            particleDataP->color.b = color.b;
            particleDataP->color.a = 32.0f*(1.0f-t);
        }
        color = capsuleTrailColorTbl[mbCapColorGet(capsuleNo)][1];
        for(i=0; i<particleP->num/2; i++, particleDataP++) {
            t = i/12.0f;
            particleDataP->scale = 0.75f*(100.0f*(0.9f+(-0.49999997f*t)));
            particleDataP->pos.x = pos->x;
            particleDataP->pos.y = pos->y;
            particleDataP->pos.z = pos->z;
            particleDataP->color.r = color.r;
            particleDataP->color.g = color.g;
            particleDataP->color.b = color.b;
            particleDataP->color.a = 64.0f*(1.0f-t);
        }
        obj->work[0] = TRUE;
    }
}

static void CapEffTrailPosSet(HuVecF *pos)
{
    OMOBJ *obj = capEffTrailOMObj;
    if(capEffTrailOMObj != NULL) {
        obj->trans.x = pos->x;
        obj->trans.y = pos->y;
        obj->trans.z = pos->z;
    }
}

static HU3D_MODELID CapEffCreate(ANIMDATA *anim, s16 num)
{
    CAPSULE_EFFECT *effP;
    CAPSULE_EFFDATA *effDataP;
    s16 i;
    HuVec2f *st;
    HU3D_MODEL *modelP;
    HuVecF *vtx;
    HU3D_MODELID modelId;
    void *dlBuf;
    void *dlBegin;
    modelId = Hu3DHookFuncCreate(CapEffDraw);
    Hu3DModelCameraSet(modelId, HU3D_CAM0);
    modelP = &Hu3DData[modelId];
    modelP->hookData = effP = CapEffModelMalloc(sizeof(CAPSULE_EFFECT), modelP->mallocNo);
    effP->anim = anim;
    effP->num = num;
    effP->blendMode = HU3D_PARTICLE_BLEND_NORMAL;
    effP->dispAttr = CAPSULE_EFF_DISPATTR_NONE;
    effP->hook = NULL;
    effP->hookMdlP = NULL;
    effP->count = 0;
    effP->attr = CAPSULE_EFF_ATTR_NONE;
    effP->unk23 = 0;
    effP->prevCount = 0;
    effP->mode = effP->time = 0;
    effP->data = effDataP = CapEffModelMalloc(num*sizeof(CAPSULE_EFFDATA), modelP->mallocNo);
    memset(effDataP, 0, num*sizeof(CAPSULE_EFFDATA));
    for(i=0; i<num; i++, effDataP++) {
        effDataP->scale = 0.0f;
        effDataP->rot.x = effDataP->rot.y = effDataP->rot.z = 0.0f;
        effDataP->animTime = 0.0f;
        effDataP->animSpeed = 1.0f;
        effDataP->pos.x = 0.0f;
        effDataP->pos.y = 0.0f;
        effDataP->pos.z = 0.0f;
        effDataP->color.r = effDataP->color.g = effDataP->color.b = effDataP->color.a = 255;
        effDataP->no = 0;
    }
    effP->vertex = vtx = CapEffModelMalloc(num*sizeof(HuVecF)*4, modelP->mallocNo);
    for(i=0; i<num*4; i++, vtx++) {
        vtx->x = vtx->y = vtx->z = 0.0f;
    }
    effP->st = st = CapEffModelMalloc(num*sizeof(HuVec2f)*4, modelP->mallocNo);
    for(i=0; i<num; i++) {
        st->x = 0.0f;
        st->y = 0.0f;
        st++;

        st->x = 1.0f;
        st->y = 0.0f;
        st++;

        st->x = 1.0f;
        st->y = 1.0f;
        st++;

        st->x = 0.0f;
        st->y = 1.0f;
        st++;
    }
    dlBegin = dlBuf = CapEffModelMalloc(0x10000, modelP->mallocNo);
    DCFlushRange(dlBuf, 0x10000);
    GXBeginDisplayList(dlBegin, 0x10000);
    GXBegin(GX_QUADS, GX_VTXFMT0, num*4);
    for(i=0; i<num; i++) {
        GXPosition1x16(i*4);
        GXColor1x16(i);
        GXTexCoord1x16(i*4);

        GXPosition1x16((i*4)+1);
        GXColor1x16(i);
        GXTexCoord1x16((i*4)+1);

        GXPosition1x16((i*4)+2);
        GXColor1x16(i);
        GXTexCoord1x16((i*4)+2);

        GXPosition1x16((i*4)+3);
        GXColor1x16(i);
        GXTexCoord1x16((i*4)+3);
    }
    GXEnd();
    effP->dlSize = GXEndDisplayList();
    effP->dl = CapEffModelMalloc(effP->dlSize, modelP->mallocNo);
    memcpy(effP->dl, dlBuf, effP->dlSize);
    DCFlushRange(effP->dl, effP->dlSize);
    HuMemDirectFree(dlBuf);
    return modelId;
}

static void CapEffDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    static HuVecF posTbl[] = {
        { -0.5f,  0.5f, 0.0f },
        {  0.5f,  0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f },
        { -0.5f, -0.5f, 0.0f }
    };
    static HuVec2f uvTbl[] = {
        { 0.0f, 0.0f },
        { 0.25f, 0.0f },
        { 0.25f, 0.25f },
        { 0.0f, 0.25f },
    };
    CAPSULE_EFFECT *effP;
    CAPSULE_EFFDATA *effDataP;
    HuVecF *vtx;
    HuVec2f *st;
    HuVecF *scaleVtxP;
    s16 i;
    s16 j;
    HuVecF *initVtxP;
    s16 bmpFmt;
    s16 row;
    s16 col;
    CAPSULE_EFFHOOK hook;

    Mtx mtxInv;
    Mtx mtxPos;
    Mtx mtxRotZ;
    HuVecF scaleVtx[4];
    HuVecF finalVtx[4];
    HuVecF initVtx[4];
    ROMtx basePosMtx;

    effP = modelP->hookData;
    if(effP->prevCounter != GlobalCounter || shadowModelDrawF) {
        if(effP->hookMdlP && effP->hookMdlP != modelP){
            CapEffDraw(effP->hookMdlP, mtx);
        }
        GXLoadPosMtxImm(*mtx, GX_PNMTX0);
        GXSetNumTevStages(1);
        GXSetNumTexGens(1);
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        if(shadowModelDrawF) {
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ONE, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
            GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        } else {
            bmpFmt = (effP->anim->bmp->dataFmt & 0xF);
            if(bmpFmt == ANIM_BMP_I8 || bmpFmt == ANIM_BMP_I4) {
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_RASC, GX_CC_ZERO);
            } else {
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
            }
            if(effP->dispAttr & CAPSULE_EFF_DISPATTR_ZBUF_OFF) {
                GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
            } else if(modelP->attr & HU3D_ATTR_ZWRITE_OFF) {
                GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
            } else {
                GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
            }
        }
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        HuSprTexLoad(effP->anim, 0, GX_TEXMAP0, GX_REPEAT, GX_REPEAT, GX_LINEAR);
        GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
        GXSetZCompLoc(GX_FALSE);
        switch (effP->blendMode) {
            case HU3D_PARTICLE_BLEND_NORMAL:
                GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
                break;
            case HU3D_PARTICLE_BLEND_ADDCOL:
                GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
                break;
            case HU3D_PARTICLE_BLEND_INVCOL:
                GXSetBlendMode(GX_BM_BLEND, GX_BL_ZERO, GX_BL_INVDSTCLR, GX_LO_NOOP);
                break;
        }
        if(HmfInverseMtxF3X3(*mtx, mtxInv) == FALSE) {
            PSMTXIdentity(mtxInv);
        }
        PSMTXReorder(mtxInv, basePosMtx);
        if(effP->hook) {
            hook = effP->hook;
            hook(modelP, effP, mtx);
        }
        effDataP = effP->data;
        vtx = effP->vertex;
        st = effP->st;
        if(effP->dispAttr & CAPSULE_EFF_DISPATTR_CAMERA_ROT) {
            MTXIdentity(mtxInv);
            MTXIdentity(*(Mtx *)(&basePosMtx));
            initVtx[0] = posTbl[0];
            initVtx[1] = posTbl[1];
            initVtx[2] = posTbl[2];
            initVtx[3] = posTbl[3];
        } else {
            PSMTXROMultVecArray(basePosMtx, &posTbl[0], initVtx, 4);
        }
        for(i=0; i<effP->num; i++, effDataP++) {
            if(!effDataP->scale) {
                vtx->x = vtx->y = vtx->z = 0;
                vtx++;
                vtx->x = vtx->y = vtx->z = 0;
                vtx++;
                vtx->x = vtx->y = vtx->z = 0;
                vtx++;
                vtx->x = vtx->y = vtx->z = 0;
                vtx++;
            } else if(effP->dispAttr & CAPSULE_EFF_DISPATTR_ROT3D) {
                VECScale(&posTbl[0], &scaleVtx[0], effDataP->scale);
                VECScale(&posTbl[1], &scaleVtx[1], effDataP->scale);
                VECScale(&posTbl[2], &scaleVtx[2], effDataP->scale);
                VECScale(&posTbl[3], &scaleVtx[3], effDataP->scale);
                mtxRot(mtxPos, effDataP->rot.x, effDataP->rot.y, effDataP->rot.z);
                PSMTXMultVecArray(mtxPos, scaleVtx, finalVtx, 4);
                VECAdd(&finalVtx[0], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[1], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[2], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[3], &effDataP->pos, vtx++);
            } else if(!effDataP->rot.z) {
                scaleVtxP = scaleVtx;
                initVtxP = initVtx;
                VECScale(initVtxP++, scaleVtxP, effDataP->scale);
                VECAdd(scaleVtxP++, &effDataP->pos, vtx++);
                VECScale(initVtxP++, scaleVtxP, effDataP->scale);
                VECAdd(scaleVtxP++, &effDataP->pos, vtx++);
                VECScale(initVtxP++, scaleVtxP, effDataP->scale);
                VECAdd(scaleVtxP++, &effDataP->pos, vtx++);
                VECScale(initVtxP++, scaleVtxP, effDataP->scale);
                VECAdd(scaleVtxP++, &effDataP->pos, vtx++);
            } else {
                VECScale(&posTbl[0], &scaleVtx[0], effDataP->scale);
                VECScale(&posTbl[1], &scaleVtx[1], effDataP->scale);
                VECScale(&posTbl[2], &scaleVtx[2], effDataP->scale);
                VECScale(&posTbl[3], &scaleVtx[3], effDataP->scale);
                MTXRotRad(mtxRotZ, 'Z', effDataP->rot.z);
                PSMTXConcat(mtxInv, mtxRotZ, mtxPos);
                PSMTXMultVecArray(mtxPos, scaleVtx, finalVtx, 4);
                VECAdd(&finalVtx[0], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[1], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[2], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[3], &effDataP->pos, vtx++);
            }
        }
        effDataP = effP->data;
        st = effP->st;
        if(!(effP->dispAttr & CAPSULE_EFF_DISPATTR_NOANIM)) {
            for(i=0; i<effP->num; i++, effDataP++) {
                row = effDataP->no & 0x3;
                col = (effDataP->no >> 2) & 0x3;
                for(j=0; j<4; j++, st++) {
                    st->x = (0.25f*row)+uvTbl[j].x;
                    st->y = (0.25f*col)+uvTbl[j].y;
                }
            }
        } else {
            for(i=0; i<effP->num; i++, effDataP++) {
                for(j=0; j<4; j++, st++) {
                    st->x = 4*uvTbl[j].x;
                    st->y = 4*uvTbl[j].y;
                }
            }
        }
        DCFlushRangeNoSync(effP->vertex, effP->num*sizeof(HuVecF)*4);
        DCFlushRangeNoSync(effP->st, effP->num*sizeof(HuVec2f)*4);
        DCFlushRangeNoSync(effP->data, effP->num*sizeof(CAPSULE_EFFDATA));
        PPCSync();
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetArray(GX_VA_POS, effP->vertex, sizeof(HuVecF));
        GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        GXSetArray(GX_VA_CLR0, &effP->data->color, sizeof(CAPSULE_EFFDATA));
        GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetArray(GX_VA_TEX0, effP->st, sizeof(HuVec2f));
        GXCallDisplayList(effP->dl, effP->dlSize);
        if(shadowModelDrawF == FALSE) {
            if(!(effP->attr & CAPSULE_EFF_ATTR_COUNTER_UPDATE)) {
                effP->count++;
            }
            if(effP->prevCount != 0 && effP->prevCount <= effP->count) {
                if(effP->attr & CAPSULE_EFF_ATTR_COUNTER_RESET) {
                    effP->count = 0;
                }
                effP->count = effP->prevCount;
            }
            effP->prevCounter = GlobalCounter;
        }
    }
}

static float CapAngleSumWrap(float start, float end)
{
    float result;
    if(start >= 360) {
        start -= 360;
    } else if(start < 0) {
        start += 360;
    }
    if(end >= 360) {
        end -= 360;
    } else if(end < 0) {
        end += 360;
    }
    result = start-end;
    if(result <= -180.0f) {
        result += 360.0f;
    } else if(result >= 180.0f){
        result -= 360.0f;
    }
    return result;
}

static float CapCameraXZAngleGet(float angle)
{
    MBCAMERA *cameraP = mbCameraGet();
    Mtx rot;
    HuVecF dir;
    mtxRot(rot, cameraP->rot.x, cameraP->rot.y, cameraP->rot.z);
    dir.x = HuSin(angle);
    dir.y = 0;
    dir.z = HuCos(angle);
    MTXMultVec(rot, &dir, &dir);
    return HuAtan(dir.x, dir.z);
}

static void CapComChoiceSet(int choice)
{
    capsuleComChoice = choice;
    mbWinTopComKeyHookSet(CapComKeyHook);
}

static void CapComKeyHook(void)
{
    int comKey[GW_PLAYER_MAX];
    int i;
    int padNo;
    int key;
    s16 delay;
    int playerNo;
    comKey[0] = comKey[1] = comKey[2] = comKey[3] = 0;
    playerNo = GwSystem.turnPlayerNo;
    padNo = GwPlayer[playerNo].padNo;
    delay = GWComKeyDelayGet();
    if(capsuleComChoice >= 0) {
        key = PAD_BUTTON_RIGHT;
    } else {
        key = PAD_BUTTON_LEFT;
    }
    comKey[padNo] = key;
    for(i=0; i<abs(capsuleComChoice); i++) {
        comKey[padNo] = key;
        HuWinComKeyWait(comKey[0], comKey[1], comKey[2], comKey[3], delay);
    }
    comKey[padNo] = PAD_BUTTON_A;
    HuWinComKeyWait(comKey[0], comKey[1], comKey[2], comKey[3], delay);
}

static inline void CapThrowCameraParamCalc(float *pos, float *paramOut, float *paramIn, int num)
{
    int i;
    float delta[8];
    float speed[8];

    paramOut[0] = 0;
    paramOut[num-1] = 0;
    for(i=0; i<num-1; i++) {
        delta[i] = paramIn[i+1]-paramIn[i];
        speed[i+1] = (pos[i+1]-pos[i])/delta[i];
    }
    paramOut[1] = (speed[2]-speed[1])-(delta[0]*paramOut[0]);
    speed[1] = 2*(paramIn[2]-paramIn[0]);
    for(i=1; i<num-2; i++) {
        double t = delta[i]/speed[i];
        paramOut[i+1] = (speed[i+2]-speed[i+1])-(paramOut[i]*t);
        speed[i+1] = (2*(paramIn[i+2]-paramIn[i]))-(delta[i]*t);
    }
    paramOut[num-2] -= delta[num-2]*paramOut[num-1];
    for(i=num-2; i>0; i--) {
        paramOut[i] = (paramOut[i]-(delta[i]*paramOut[i+1]))/speed[i];
    }
}

static void CapThrowCameraSet(float *x, float *y, float *z, int num)
{
    int i;
    float dx;
    float dy;
    capsuleTime[0] = 0;
    for(i=1; i<num; i++) {
        dx = x[i]-x[i-1];
        dy = y[i]-y[i-1];
        capsuleTime[i] = capsuleTime[i-1]+sqrt((dx*dx)+(dy*dy));
    }
    for(i=1; i<num; i++) {
        capsuleTime[i] /= capsuleTime[num-1];
    }
    CapThrowCameraParamCalc(x, capsuleBezierX, capsuleTime, num);
    CapThrowCameraParamCalc(y, capsuleBezierY, capsuleTime, num);
    CapThrowCameraParamCalc(z, capsuleBezierZ, capsuleTime, num);
}

static inline float CapThrowCameraBezierGet(float t, float *pos, float *param, float *time, int num)
{
    int i;
    int max;
    int idx;
    float timeFrac;
    float dt;

    for(i=0, max=num-1; i<max;) {
        idx = (i+max)/2;
        if(time[idx] < t) {
            i = idx+1;
        } else{
            max = idx;
        }
    }
    if(i > 0) {
        i--;
    }
    dt = time[i+1]-time[i];
    timeFrac = t-time[i];
    return pos[i] + (timeFrac * ((timeFrac * ((3.0f * param[i]) + ((timeFrac * (param[i+1] - param[i])) / dt))) + (((pos[i+1] - pos[i]) / dt) - (dt * ((2 * param[i]) + param[i+1])))));
}

static void CapThrowCameraCalc(float t, float *x, float *y, float *z, HuVecF *out, int num)
{
    out->x = CapThrowCameraBezierGet(t, x, capsuleBezierX, capsuleTime, num);
    out->y = CapThrowCameraBezierGet(t, y, capsuleBezierY, capsuleTime, num);
    out->z = CapThrowCameraBezierGet(t, z, capsuleBezierZ, capsuleTime, num);
}

static void CapColMdlIdGet(void)
{
    int boardNo = mbBoardNoGet();
    if(capsuleColObjId != -1) {
        capsuleColMdlId = mbObjModelIDGet(capsuleColObjId);
    }
}

static BOOL CapColCheck(HuVecF *posA, HuVecF *posB, HuVecF *out)
{
    HSF_FACE *faceP;
    HSF_DATA *hsfP;
    HSF_BUFFER *faceBufP;
    HSF_BUFFER *vtxBufP;
    BOOL quadF;
    int i;
    int j;
    HU3D_MODEL *modelP;

    float dot;
    float dotEdge;
    float dotA;
    float dotB;
    float invDotA;
    float scale;

    HuVecF faceVtx[4];
    HuVecF faceNorm;
    HuVecF faceCA;
    HuVecF faceBA;
    HuVecF faceBAQuad;
    HuVecF faceCAQuad;
    HuVecF faceNormQuad;
    HuVecF dir;
    HuVecF outPos;
    HuVecF dotVec;

    float mag;
    int temp;

    quadF = FALSE;
    temp = 0;
    if(capsuleColMdlId == HU3D_MODELID_NONE) {
        return FALSE;
    }
    modelP = &Hu3DData[capsuleColMdlId];
    hsfP = modelP->hsf;
    faceBufP = hsfP->face;
    vtxBufP = hsfP->vertex;
    VECSubtract(posA, posB, &dir);
    if(VECMag(&dir) <= 0.0f) {
        return FALSE;
    }
    VECNormalize(&dir, &dir);
    mag = VECMag(&dir);
    for(i=0; i<hsfP->faceNum; i++, faceBufP++) {
        faceP = faceBufP->data;
        for(j=0; j<faceBufP->count; j++, faceP++) {
            switch(faceP->type & HSF_FACE_MASK) {
                case 0:
                    quadF = TRUE;
                    break;

                case 1:
                    quadF = TRUE;
                    break;

                case 2:
                    quadF = FALSE;
                    faceVtx[0] = *(((HuVecF *)vtxBufP->data)+faceP->index[0].vertex);
                    faceVtx[1] = *(((HuVecF *)vtxBufP->data)+faceP->index[1].vertex);
                    faceVtx[2] = *(((HuVecF *)vtxBufP->data)+faceP->index[2].vertex);
                    break;

                case 3:
                    quadF = TRUE;
                    break;

                default:
                    break;
            }
            if(!quadF) {
                VECSubtract(&faceVtx[1], &faceVtx[0], &faceBAQuad);
                VECSubtract(&faceVtx[2], &faceVtx[0], &faceCAQuad);
                VECCrossProduct(&faceBAQuad, &faceCAQuad, &faceNormQuad);
                if(VECMag(&faceNormQuad) > 0.0f) {
                    VECNormalize(&faceNormQuad, &faceNormQuad);
                }
                dot = -VECDotProduct(&faceNormQuad, &faceVtx[0]);
                dotA = (faceNormQuad.x*posA->x)+(faceNormQuad.y*posA->y)+(faceNormQuad.z*posA->z)+dot;
                dotB = (faceNormQuad.x*posB->x)+(faceNormQuad.y*posB->y)+(faceNormQuad.z*posB->z)+dot;
                if(dotA*dotB > 0) {
                    continue;
                }
                invDotA = -((faceNormQuad.x*posA->x)+(faceNormQuad.y*posA->y)+(faceNormQuad.z*posA->z)+dot);
                dotEdge = (faceNormQuad.x*dir.x)+(faceNormQuad.y*dir.y)+(faceNormQuad.z*dir.z);
                if(dotEdge != 0.0f) {
                    scale = invDotA/dotEdge;
                    VECScale(&dir, &outPos, scale);
                    VECAdd(&outPos, posA, &outPos);
                    *out = outPos;
                    VECSubtract(&faceVtx[1], &faceVtx[0], &faceBA);
                    VECSubtract(&outPos, &faceVtx[0], &faceCA);
                    VECCrossProduct(&faceBA, &faceCA, &faceNorm);
                    dotVec.x = VECDotProduct(&faceNormQuad, &faceNorm);
                    VECSubtract(&faceVtx[2], &faceVtx[1], &faceBA);
                    VECSubtract(&outPos, &faceVtx[1], &faceCA);
                    VECCrossProduct(&faceBA, &faceCA, &faceNorm);
                    dotVec.y = VECDotProduct(&faceNormQuad, &faceNorm);
                    VECSubtract(&faceVtx[0], &faceVtx[2], &faceBA);
                    VECSubtract(&outPos, &faceVtx[2], &faceCA);
                    VECCrossProduct(&faceBA, &faceCA, &faceNorm);
                    dotVec.z = VECDotProduct(&faceNormQuad, &faceNorm);
                    if(dotVec.x > 0 && dotVec.y > 0 && dotVec.z > 0) {
                        return TRUE;
                    }
                    if(dotVec.x < 0 && dotVec.y < 0 && dotVec.z < 0) {
                        return TRUE;
                    }
                }
            }
        }
    }
    return FALSE;
}

static void CapColKill(void)
{
    capsuleColMdlId = HU3D_MODELID_NONE;
}

static HuVecF colScaleTbl[6][3] = {
    { { -0.5f, 0.0f, 0.5f }, { -0.5f, 1.0f, 0.5f }, { 0.5f, 1.0f, 0.5f } },
    { { -0.5f, 0.0f, 0.5f }, { 0.5f, 1.0f, 0.5f }, { 0.5f, 0.0f, 0.5f } },
    { { 0.5f, 0.0f, 0.5f }, { 0.5f, 1.0f, 0.5f }, { 0.0f, 1.0f, -0.5f } },
    { { 0.5f, 0.0f, 0.5f }, { 0.0f, 1.0f, -0.5f }, { 0.0f, 0.0f, -0.5f } },
    { { 0.0f, 0.0f, -0.5f }, { 0.0f, 1.0f, -0.5f }, { -0.5f, 1.0f, 0.5f } },
    { { 0.0f, 0.0f, -0.5f }, { -0.5f, 1.0f, 0.5f }, { -0.5f, 0.0f, 0.5f } }
};

static HuVec2f charSizeTbl[14] = {
    { 100.0f, 175.0f },
    { 100.0f, 200.0f },
    { 110.0f, 220.0f },
    { 100.0f, 175.0f },
    { 175.0f, 175.0f },
    { 110.0f, 220.0f },
    { 200.0f, 260.0f },
    { 100.0f, 150.0f },
    { 175.0f, 175.0f },
    { 150.0f, 125.0f },
    { 100.0f, 150.0f },
    { 150.0f, 125.0f },
    { 150.0f, 125.0f },
    { 150.0f, 125.0f }
};

static BOOL CapColExec(int playerNo, HuVecF *posA, HuVecF *posB, HuVecF *out)
{
    BOOL quadF;
    int i;
    int j;

    float dot;
    float dotEdge;
    float dotA;
    float dotB;
    float invDotA;
    float scale;

    HuVecF faceVtx[3];
    HuVecF faceNorm;
    HuVecF faceCA;
    HuVecF faceBA;
    HuVecF faceBAQuad;
    HuVecF faceCAQuad;
    HuVecF faceNormQuad;
    HuVecF dir;
    HuVecF outPos;
    HuVecF playerPos;
    HuVecF dotVec;

    float mag;
    int temp2;
    int temp;

    quadF = FALSE;
    temp = 0;
    VECSubtract(posA, posB, &dir);
    if(VECMag(&dir) <= 0.0f) {
        return FALSE;
    }
    VECNormalize(&dir, &dir);
    mag = VECMag(&dir);
    for(i=0; i<GW_PLAYER_MAX; i++) {
        if(!GWPartyGet() && i != 0) {
            continue;
        }
        if(i == playerNo) {
            continue;
        }
        if(!mbPlayerDispGet(i)) {
            continue;
        }
        mbPlayerPosGet(i, &playerPos);
        for(j=0; j<6; j++) {
            faceVtx[0].x = colScaleTbl[j][0].x*charSizeTbl[GwPlayer[i].charNo].x;
            faceVtx[0].y = colScaleTbl[j][0].y*charSizeTbl[GwPlayer[i].charNo].y;
            faceVtx[0].z = colScaleTbl[j][0].z*charSizeTbl[GwPlayer[i].charNo].x;
            faceVtx[1].x = colScaleTbl[j][1].x*charSizeTbl[GwPlayer[i].charNo].x;
            faceVtx[1].y = colScaleTbl[j][1].y*charSizeTbl[GwPlayer[i].charNo].y;
            faceVtx[1].z = colScaleTbl[j][1].z*charSizeTbl[GwPlayer[i].charNo].x;
            faceVtx[2].x = colScaleTbl[j][2].x*charSizeTbl[GwPlayer[i].charNo].x;
            faceVtx[2].y = colScaleTbl[j][2].y*charSizeTbl[GwPlayer[i].charNo].y;
            faceVtx[2].z = colScaleTbl[j][2].z*charSizeTbl[GwPlayer[i].charNo].x;
            VECAdd(&faceVtx[0], &playerPos, &faceVtx[0]);
            VECAdd(&faceVtx[1], &playerPos, &faceVtx[1]);
            VECAdd(&faceVtx[2], &playerPos, &faceVtx[2]);
            VECSubtract(&faceVtx[1], &faceVtx[0], &faceBAQuad);
            VECSubtract(&faceVtx[2], &faceVtx[0], &faceCAQuad);
            VECCrossProduct(&faceBAQuad, &faceCAQuad, &faceNormQuad);
            if(VECMag(&faceNormQuad) > 0.0f) {
                VECNormalize(&faceNormQuad, &faceNormQuad);
            }
            dot = -VECDotProduct(&faceNormQuad, &faceVtx[0]);
            dotA = (faceNormQuad.x*posA->x)+(faceNormQuad.y*posA->y)+(faceNormQuad.z*posA->z)+dot;
            dotB = (faceNormQuad.x*posB->x)+(faceNormQuad.y*posB->y)+(faceNormQuad.z*posB->z)+dot;
            if(dotA*dotB > 0) {
                continue;
            }
            invDotA = -((faceNormQuad.x*posA->x)+(faceNormQuad.y*posA->y)+(faceNormQuad.z*posA->z)+dot);
            dotEdge = (faceNormQuad.x*dir.x)+(faceNormQuad.y*dir.y)+(faceNormQuad.z*dir.z);
            if(dotEdge != 0.0f) {
                scale = invDotA/dotEdge;
                VECScale(&dir, &outPos, scale);
                VECAdd(&outPos, posA, &outPos);
                *out = outPos;
                VECSubtract(&faceVtx[1], &faceVtx[0], &faceBA);
                VECSubtract(&outPos, &faceVtx[0], &faceCA);
                VECCrossProduct(&faceBA, &faceCA, &faceNorm);
                dotVec.x = VECDotProduct(&faceNormQuad, &faceNorm);
                VECSubtract(&faceVtx[2], &faceVtx[1], &faceBA);
                VECSubtract(&outPos, &faceVtx[1], &faceCA);
                VECCrossProduct(&faceBA, &faceCA, &faceNorm);
                dotVec.y = VECDotProduct(&faceNormQuad, &faceNorm);
                VECSubtract(&faceVtx[0], &faceVtx[2], &faceBA);
                VECSubtract(&outPos, &faceVtx[2], &faceCA);
                VECCrossProduct(&faceBA, &faceCA, &faceNorm);
                dotVec.z = VECDotProduct(&faceNormQuad, &faceNorm);
                if(dotVec.x > 0 && dotVec.y > 0 && dotVec.z > 0) {
                    return TRUE;
                }
                if(dotVec.x < 0 && dotVec.y < 0 && dotVec.z < 0) {
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

/* @@END@@ */
