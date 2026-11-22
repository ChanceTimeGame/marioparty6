#include "game/board/guide.h"
#include "game/board/object.h"
#include "game/board/effect.h"
#include "game/board/audio.h"
#include "game/board/pause.h"
#include "game/board/exit.h"
#include "game/board/math.h"
#include "game/board/snpc.h"
#include "game/board/telop.h"
#include "game/gamework.h"
#include "game/frand.h"
#include "game/data.h"
#include "game/sprite.h"

#define GUIDE_MAGIC 'MBGD'

#define GUIDE_STATE_IDLE 0
#define GUIDE_STATE_FADEIN 1
#define GUIDE_STATE_FADEIN_END 2
#define GUIDE_STATE_FADEOUT 3
#define GUIDE_STATE_FADEOUT_END 4

typedef struct GuideWork_s {
    u32 magic;
    u16 activeF : 1;
    u16 killF : 1;
    u16 screenF : 1;
    u16 unkF : 1;
    u16 motNextF : 1;
    u16 state : 4;
    s16 time;
    s16 maxTime;
    s16 seq;
    s16 motNext;
} GUIDEWORK;

typedef struct GuideParticleData_s {
    s16 time;
    s16 colorNo;
    u8 unk4[4];
    HuVecF vel;
    float scaleBase;
    float alpha;
    u8 unk1C[4];
    HuVecF accel;
    float scale;
    u8 unk30[8];
    float zRot;
    HuVecF pos;
    GXColor color;
    s16 animNo;
    u8 unk4E[0x5C - 0x4E];
} GUIDEPARTICLEDATA;

typedef struct GuideParticle_s {
    s16 endF;
    u8 unk2[0x14 - 0x2];
    float unk14;
    u8 unk18[0x26 - 0x18];
    s16 num;
    u32 initF;
    u8 unk2C[0x68 - 0x2C];
    GUIDEPARTICLEDATA *data;
} GUIDEPARTICLE;

static void GuideOMExec(OMOBJ *obj);
static BOOL GuideFadeInUpdate(OMOBJ *obj);
static BOOL GuideFadeOutUpdate(OMOBJ *obj);
static HU3D_MODELID GuideFadeInEffectCreate(OMOBJ *obj);
static HU3D_MODELID GuideFadeOutEffectCreate(OMOBJ *obj);
static void GuideFadeInEffectHook(HU3D_MODEL *modelP, GUIDEPARTICLE *particleP, Mtx mtx);
static void GuideFadeOutEffectHook(HU3D_MODEL *modelP, GUIDEPARTICLE *particleP, Mtx mtx);

static int GuideSunMotCreate[24] = {
    0x110001, 0x110001, 0x110002, 0x110003, 0x110004, 0x110005, 0x110006, 0x110007,
    0x110008, 0x110009, 0x11000A, 0x11000B, 0x11000C, 0x11000D, 0x11000E, 0x11000F,
    0x110010, 0x110011, 0x110012, 0x110013, 0x110014, 0x110015, 0x110016, 0x110017,
};

static int GuideMoonMotCreate[24] = {
    0x11001C, 0x11001C, 0x11001D, 0x11001E, 0x11001F, 0x110020, 0x110021, 0x110022,
    0x110023, 0x110023, 0x110024, 0x110025, 0x110026, 0x110027, 0x110028, 0x110029,
    0x11002A, 0x11002B, 0x11002C, 0x11002D, 0x11002E, 0x11002F, 0x110030, 0x110031,
};

static int guideMdlFileTbl[2] = { 0x110000, 0x11001B };
static int *guideMotFileTbl[2] = { GuideSunMotCreate, GuideMoonMotCreate };
static s8 guideDefaultMotTbl[] = { 1, 7, 4, 17, -1 };

void mbGuideInit(void)
{
}

OMOBJ *mbGuideCreate(int guideNo, HuVecF *pos, HuVecF *rot, s8 *motTbl, float scale, u32 attr)
{
    u8 motFlag[25];
    int i;
    s8 *motP;
    OMOBJ *obj;
    GUIDEWORK *work;
    int modelId;
    int motNo;
    int motId;

    for(i = 0; i < sizeof(motFlag); i++) {
        motFlag[i] = FALSE;
    }
    motP = guideDefaultMotTbl;
    while(*motP >= 0) {
        motFlag[*motP++] = TRUE;
    }
    if(motTbl != NULL) {
        motP = motTbl;
        while(*motP >= 0) {
            motFlag[*motP++] = TRUE;
        }
    }
    obj = omAddObjEx(mbObjMan, 256, 4, 0, -1, GuideOMExec);
    for(i = 0; i < 4; i++) {
        obj->mdlId[i] = -1;
    }
    omSetStatBit(obj, 0x100);
    work = (GUIDEWORK *)obj->work;
    work->magic = GUIDE_MAGIC;
    work->activeF = TRUE;
    work->killF = FALSE;
    work->unkF = FALSE;
    work->motNextF = FALSE;
    work->screenF = FALSE;
    work->state = GUIDE_STATE_IDLE;
    work->motNext = 0;
    modelId = mbObjCreate(guideMdlFileTbl[guideNo], NULL, TRUE);
    obj->mdlId[0] = modelId;
    if(guideNo == 1) {
        obj->mdlId[1] = mbObjCreate(0x110035, NULL, TRUE);
        mbObjHookSet(modelId, "itemhook_R", obj->mdlId[1]);
    }
    for(i = 1; i < sizeof(motFlag); i++) {
        if(motFlag[i]) {
            mbObjMotionNoCreate(modelId, guideMotFileTbl[guideNo][i], i);
            if(i == 11 || i == 16) {
                motId = mbObjMotionIDGet(modelId, i);
                Hu3DMotionAttrSet(motId, HU3D_MOTATTR_LOOP);
            }
        }
    }
    if(mbPauseProcCheck()) {
        omSetStatBit(obj, 0xA0);
        mbObjAttrSet(modelId, 0x200000);
    }
    if(attr & 1) {
        work->screenF = TRUE;
        mbObjCameraSet(modelId, 4);
        mbObjLayerSet(modelId, 3);
    } else {
        mbObjLayerSet(modelId, 3);
    }
    if(pos) {
        mbObjPosSetV(modelId, pos);
    }
    if(rot) {
        mbObjRotSetV(modelId, rot);
    }
    mbObjScaleSet(modelId, scale, scale, scale);
    if(attr & 4) {
        motNo = 1;
    } else {
        motNo = 4;
    }
    mbObjMotionSet(modelId, motNo, 0x40000001);
    HuDataDirClose(0x110000);
    if(attr & 2) {
        work->state = GUIDE_STATE_FADEIN;
        work->seq = 0;
        while(!mbGuideIdleCheck(obj)) {
            HuPrcVSleep();
        }
    }
    return obj;
}

OMOBJ *mbGuideCreateFlag(HuVecF *pos, s8 *motTbl, BOOL screenF, BOOL fadeInF, BOOL motF)
{
    u32 attr = 0;
    if(screenF) {
        attr |= 1;
    }
    if(fadeInF) {
        attr |= 2;
    }
    if(motF) {
        attr |= 4;
    }
    return mbGuideCreate(mbGuideNoGet(), pos, NULL, motTbl, 1.0f, attr);
}

OMOBJ *mbGuideCreateIn(void)
{
    return mbGuideCreate(mbGuideNoGet(), NULL, NULL, NULL, 1.0f, 0);
}

void mbGuideKill(OMOBJ *obj)
{
    GUIDEWORK *work = (GUIDEWORK *)obj->work;
    work->killF = TRUE;
    if(obj->mdlId[0] != 0) {
        mbObjDispSet(obj->mdlId[0], FALSE);
    }
}

void mbGuideEnd(OMOBJ *obj, BOOL unused)
{
    GUIDEWORK *work = (GUIDEWORK *)obj->work;
    work->state = GUIDE_STATE_FADEOUT;
    work->seq = 0;
    while(!mbGuideIdleCheck(obj)) {
        HuPrcVSleep();
    }
    work->killF = TRUE;
    if(obj->mdlId[0] != 0) {
        mbObjDispSet(obj->mdlId[0], FALSE);
    }
}

void mbGuideFadeIn(OMOBJ *obj)
{
    GUIDEWORK *work = (GUIDEWORK *)obj->work;
    work->state = GUIDE_STATE_FADEIN;
    work->seq = 0;
    while(!mbGuideIdleCheck(obj)) {
        HuPrcVSleep();
    }
}

void mbGuideFadeOut(OMOBJ *obj)
{
    GUIDEWORK *work = (GUIDEWORK *)obj->work;
    work->state = GUIDE_STATE_FADEOUT;
    work->seq = 0;
    while(!mbGuideIdleCheck(obj)) {
        HuPrcVSleep();
    }
    if(obj->mdlId[0] != 0) {
        mbObjDispSet(obj->mdlId[0], FALSE);
    }
}

MBOBJID mbGuideModelGet(OMOBJ *obj)
{
    GUIDEWORK *work = (GUIDEWORK *)obj->work;
    int modelId = 0;
    if(!work->killF) {
        modelId = obj->mdlId[0];
    }
    return modelId;
}

void mbGuideScreenSet(OMOBJ *obj, BOOL screenF)
{
    GUIDEWORK *work = (GUIDEWORK *)obj->work;
    int modelId = 0;
    Mtx mtx;
    if(!work->killF) {
        modelId = obj->mdlId[0];
        if(screenF) {
            work->screenF = TRUE;
            mbObjCameraSet(modelId, 4);
            mbObjLayerSet(modelId, 3);
        } else {
            work->screenF = FALSE;
            mbObjCameraSet(modelId, 1);
            mbObjLayerSet(modelId, 3);
            MTXIdentity(mtx);
            mbObjMtxSet(modelId, mtx);
        }
    }
}

BOOL mbGuideIdleCheck(OMOBJ *obj)
{
    GUIDEWORK *work = (GUIDEWORK *)obj->work;
    return work->state == GUIDE_STATE_IDLE;
}

void mbGuideMotionNextSet(OMOBJ *obj, s16 motNo)
{
    GUIDEWORK *work = (GUIDEWORK *)obj->work;
    work->motNext = motNo;
}

void mbGuideMotionSet(OMOBJ *obj, s16 motNo, BOOL shiftF)
{
    GUIDEWORK *work = (GUIDEWORK *)obj->work;
    int modelId = obj->mdlId[0];
    if(shiftF) {
        mbObjMotionShiftSet(modelId, motNo, 0.0f, 12.0f, TRUE);
    } else {
        mbObjMotionSet(modelId, motNo, TRUE);
    }
    work->motNextF = FALSE;
    work->motNext = motNo;
}

void mbGuideMotionShiftSet(OMOBJ *obj, s16 motNo, BOOL shiftF)
{
    GUIDEWORK *work = (GUIDEWORK *)obj->work;
    int modelId = obj->mdlId[0];
    if(shiftF) {
        mbObjMotionShiftSet(modelId, motNo, 0.0f, 12.0f, FALSE);
    } else {
        mbObjMotionSet(modelId, motNo, FALSE);
    }
    work->motNextF = TRUE;
}

void mbGuideMotionStop(OMOBJ *obj)
{
    GUIDEWORK *work = (GUIDEWORK *)obj->work;
    work->motNextF = FALSE;
}

BOOL mbGuideMotionCheck(OMOBJ *obj)
{
    GUIDEWORK *work = (GUIDEWORK *)obj->work;
    return work->motNextF != TRUE;
}

static void GuideOMExec(OMOBJ *obj)
{
    GUIDEWORK *work = (GUIDEWORK *)obj->work;
    int i;
    HU3D_CAMERA *cameraP;
    int modelId;
    HuVecF pos;
    HuVecF scale;
    Mtx lookAt;

    if(mbExitCheck() || work->killF) {
        if(obj->mdlId[0] > 0) {
            mbObjKill(obj->mdlId[0]);
        }
        if(obj->mdlId[1] > 0) {
            mbObjKill(obj->mdlId[1]);
        }
        obj->mdlId[0] = 0;
        obj->mdlId[1] = 0;
        if(obj->mdlId[2] >= 0) {
            mbParticleKill(obj->mdlId[2]);
        }
        if(obj->mdlId[3] >= 0) {
            mbParticleKill(obj->mdlId[3]);
        }
        obj->mdlId[2] = -1;
        obj->mdlId[3] = -1;
        work->magic = 0;
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    switch(work->state) {
        case GUIDE_STATE_IDLE:
            if(work->motNextF && mbObjMotionShiftIDGet(obj->mdlId[0]) < 0 && mbObjMotionEndCheck(obj->mdlId[0])) {
                mbObjMotionShiftSet(obj->mdlId[0], work->motNext, 0.0f, 12.0f, TRUE);
                work->motNextF = FALSE;
            }
            break;

        case GUIDE_STATE_FADEIN:
            if(GuideFadeInUpdate(obj)) {
                work->state = GUIDE_STATE_IDLE;
            }
            break;

        case GUIDE_STATE_FADEIN_END:
            break;

        case GUIDE_STATE_FADEOUT:
            if(GuideFadeOutUpdate(obj)) {
                work->state = GUIDE_STATE_IDLE;
            }
            break;

        case GUIDE_STATE_FADEOUT_END:
            break;
    }
    if(work->screenF) {
        for(i = 0; i < HU3D_CAM_MAX; i++) {
            if((1 << i) & 4) {
                break;
            }
        }
        cameraP = &Hu3DCamera[i];
        modelId = obj->mdlId[0];
        mbObjPosGet(modelId, &pos);
        mbObjScaleGet(modelId, &scale);
        pos.y += 150.0f * scale.y;
        MTXLookAt(lookAt, &cameraP->pos, &cameraP->up, &pos);
        MTXInverse(lookAt, lookAt);
        mbObjMtxSet(modelId, lookAt);
    }
}

static inline GUIDEPARTICLE *GuideParticleGet(HU3D_MODELID modelId)
{
    return Hu3DData[modelId].hookData;
}

static BOOL GuideFadeInUpdate(OMOBJ *obj)
{
    GUIDEWORK *work = (GUIDEWORK *)obj->work;
    HuVecF pos;
    HuVecF rot = { 0, 0, 0 };
    BOOL result = FALSE;
    int modelId = obj->mdlId[0];
    float t;

    switch(work->seq) {
        case 0:
            mbObjDispSet(modelId, TRUE);
            mbObjPosGet(modelId, &pos);
            obj->trans.x = pos.x;
            obj->trans.y = pos.y;
            obj->trans.z = pos.z;
            pos.y += 200.0f;
            mbObjFadeCreate(modelId, &pos);
            mbObjFadeTexColorSet(modelId, 128, 128, 128, 0.5f);
            if(obj->mdlId[1] > 0) {
                mbObjFadeCreate((s16)obj->mdlId[1], &pos);
                mbObjFadeTexColorSet((s16)obj->mdlId[1], 128, 128, 128, 0.5f);
            }
            obj->mdlId[2] = GuideFadeInEffectCreate(obj);
            Hu3DModelPosSetV(obj->mdlId[2], &pos);
            work->time = 0;
            work->maxTime = 60;
            work->seq++;
            mbAudFXPlay(1120);
            break;

        case 1:
            work->time = 0;
            work->maxTime = 60;
            work->seq++;
            break;

        case 2:
            work->time++;
            t = (float)work->time / work->maxTime;
            pos.x = obj->trans.x;
            pos.y = (obj->trans.y - 50.0f) + (250.0f * (1.0f - t));
            pos.z = obj->trans.z;
            mbObjFadeTexRotSet(modelId, &pos, &rot);
            if(obj->mdlId[1] > 0) {
                mbObjFadeTexRotSet((s16)obj->mdlId[1], &pos, &rot);
            }
            if(pos.y <= obj->trans.y) {
                GuideParticleGet(obj->mdlId[2])->endF = TRUE;
            } else {
                Hu3DModelPosSetV(obj->mdlId[2], &pos);
            }
            if(work->time >= work->maxTime) {
                work->maxTime = 30;
                work->time = 0;
                work->seq++;
            }
            break;

        case 3:
            work->time++;
            if(work->time < work->maxTime) {
                break;
            }
            work->seq++;

        case 4:
            mbObjFadeKill(modelId);
            if(obj->mdlId[1] > 0) {
                mbObjFadeKill((s16)obj->mdlId[1]);
            }
            mbParticleKill(obj->mdlId[2]);
            obj->mdlId[2] = -1;
            result = TRUE;
            break;
    }
    return result;
}

static BOOL GuideFadeOutUpdate(OMOBJ *obj)
{
    GUIDEWORK *work = (GUIDEWORK *)obj->work;
    HuVecF pos;
    HuVecF rot = { 0, 0, 0 };
    BOOL result = FALSE;
    int modelId = obj->mdlId[0];
    float t;

    switch(work->seq) {
        case 0:
            mbObjPosGet(modelId, &pos);
            obj->trans.x = pos.x;
            obj->trans.y = pos.y;
            obj->trans.z = pos.z;
            pos.y -= 50.0f;
            mbObjFadeCreate(modelId, &pos);
            mbObjFadeTexColorSet(modelId, 128, 128, 128, 0.5f);
            if(obj->mdlId[1] > 0) {
                mbObjFadeCreate((s16)obj->mdlId[1], &pos);
                mbObjFadeTexColorSet((s16)obj->mdlId[1], 128, 128, 128, 0.5f);
            }
            obj->mdlId[2] = GuideFadeOutEffectCreate(obj);
            Hu3DModelPosSetV(obj->mdlId[2], &obj->trans);
            work->time = 0;
            work->maxTime = 60;
            work->seq++;
            mbAudFXPlay(1121);
            break;

        case 1:
            work->time = 0;
            work->maxTime = 60;
            work->seq++;
            break;

        case 2:
            work->time++;
            t = (float)work->time / work->maxTime;
            pos.x = obj->trans.x;
            pos.y = (obj->trans.y - 50.0f) + (250.0f * t);
            pos.z = obj->trans.z;
            mbObjFadeTexRotSet(modelId, &pos, &rot);
            if(obj->mdlId[1] > 0) {
                mbObjFadeTexRotSet((s16)obj->mdlId[1], &pos, &rot);
            }
            if(pos.y >= obj->trans.y) {
                Hu3DModelPosSetV(obj->mdlId[2], &pos);
            }
            if(pos.y >= 160.0f + obj->trans.y) {
                GuideParticleGet(obj->mdlId[2])->endF = TRUE;
            }
            if(work->time >= work->maxTime) {
                work->maxTime = 30;
                work->time = 0;
                work->seq++;
            }
            break;

        case 3:
            work->time++;
            if(work->time < work->maxTime) {
                break;
            }
            work->seq++;

        case 4:
            mbObjDispSet(obj->mdlId[0], FALSE);
            mbObjFadeKill(modelId);
            if(obj->mdlId[1] > 0) {
                mbObjDispSet(obj->mdlId[1], FALSE);
                mbObjFadeKill((s16)obj->mdlId[1]);
            }
            mbParticleKill(obj->mdlId[2]);
            obj->mdlId[2] = -1;
            result = TRUE;
            break;
    }
    return result;
}

int mbGuideNoGet(void)
{
    int guideNo = 0;
    if(GWPartyGet() == FALSE) {
        guideNo = 0;
    } else if(GwSystem.curTime == 0) {
        guideNo = 0;
    } else {
        guideNo = 1;
    }
    return guideNo;
}

int mbGuideSpeakerNoGet(void)
{
    static int speakerTbl[2] = { 6, 7 };
    return speakerTbl[mbGuideNoGet()];
}

static HU3D_MODELID GuideFadeInEffectCreate(OMOBJ *obj)
{
    Mtx mtx;
    int modelId = mbParticleCreate(HuSprAnimRead(HuDataSelHeapReadNum(mbBoardDataNumGet(0x50064), 0x10000000, HEAP_MODEL)), 200);
    mbParticleHookSet(modelId, GuideFadeInEffectHook);
    Hu3DModelCameraSet(modelId, mbObjGet(obj->mdlId[0])->cameraBit);
    Hu3DModelLayerSet(modelId, mbObjGet(obj->mdlId[0])->layer + 1);
    mbObjMtxGet(obj->mdlId[0], mtx);
    Hu3DModelMtxSet(modelId, &mtx);
    if(mbPauseProcCheck()) {
        Hu3DModelAttrSet(modelId, 0x200000);
    }
    return modelId;
}

static HU3D_MODELID GuideFadeOutEffectCreate(OMOBJ *obj)
{
    Mtx mtx;
    int modelId = mbParticleCreate(HuSprAnimRead(HuDataSelHeapReadNum(mbBoardDataNumGet(0x50064), 0x10000000, HEAP_MODEL)), 200);
    mbParticleHookSet(modelId, GuideFadeOutEffectHook);
    Hu3DModelCameraSet(modelId, mbObjGet(obj->mdlId[0])->cameraBit);
    Hu3DModelLayerSet(modelId, mbObjGet(obj->mdlId[0])->layer + 1);
    mbObjMtxGet(obj->mdlId[0], mtx);
    Hu3DModelMtxSet(modelId, &mtx);
    if(mbPauseProcCheck()) {
        Hu3DModelAttrSet(modelId, 0x200000);
    }
    return modelId;
}

static void GuideFadeInEffectHook(HU3D_MODEL *modelP, GUIDEPARTICLE *particleP, Mtx mtx)
{
    static u8 effNo[16] = { 0, 1, 2, 2, 3, 3, 3, 3, 0, 1, 2, 2, 3, 3, 3, 3 };
    static float effSize[4] = { 0.9f, 0.9f, 0.9f, 0.8f };
    static GXColor effColor[8] = {
        { 220, 64, 64, 0 },
        { 64, 220, 64, 0 },
        { 220, 220, 64, 0 },
        { 64, 64, 220, 0 },
        { 220, 64, 220, 0 },
        { 64, 220, 220, 0 },
        { 220, 120, 64, 0 },
        { 64, 120, 220, 0 },
    };
    GUIDEPARTICLEDATA *dataP;
    int i;
    int num;
    int colorNo;
    int no;
    float t;
    float t2;

    if(particleP->initF == 0) {
        particleP->initF = 1;
        particleP->endF = FALSE;
        particleP->unk14 = 0.0f;
    }
    num = 0;
    if(particleP->endF == FALSE) {
        num = 6;
    }
    for(dataP = particleP->data, i = 0; i < particleP->num; i++, dataP++) {
        if(num <= 0) {
            break;
        }
        if(dataP->time != 0) {
            continue;
        }
        no = effNo[mbRandMod(16)];
        dataP->animNo = no;
        dataP->vel.x = 360.0f * frandf();
        dataP->vel.y = 80.0f * (0.3f + (0.7f * frandf()));
        dataP->vel.z = 130.0f * (0.7f + (0.3f * frandf()));
        dataP->scale = effSize[no] * (35.0f * (0.5f + (0.5f * frandf())));
        dataP->scaleBase = dataP->scale;
        colorNo = mbRandMod(8);
        t = 0.3f * frandf();
        dataP->color.r = (t * (255.0f - effColor[colorNo].r)) + effColor[colorNo].r;
        dataP->color.g = (t * (255.0f - effColor[colorNo].g)) + effColor[colorNo].g;
        dataP->color.b = (t * (255.0f - effColor[colorNo].b)) + effColor[colorNo].b;
        dataP->color.a = mbRandMod(70) + 150;
        dataP->alpha = dataP->color.a;
        dataP->color.a = 0;
        dataP->zRot = 0.0f;
        if(dataP->animNo < 3) {
            dataP->zRot = 360.0f * frandf();
        }
        dataP->time = 30;
        num--;
    }
    for(dataP = particleP->data, i = 0; i < particleP->num; i++, dataP++) {
        if(dataP->time == 0) {
            continue;
        }
        t = (1.0f / 30.0f) * dataP->time;
        dataP->vel.x += 5.0f;
        dataP->pos.x = t * (dataP->vel.z * mbSinDeg(dataP->vel.x));
        dataP->pos.z = t * (dataP->vel.z * mbCosDeg(dataP->vel.x));
        t2 = (1.0f / 0.6f) * (t - 0.4f);
        if(t2 < 0.0f) {
            t2 = 0.0f;
        }
        dataP->pos.y = dataP->vel.y * (t * t);
        dataP->scale -= 0.4f;
    }
    for(dataP = particleP->data, i = 0; i < particleP->num; i++, dataP++) {
        if(dataP->time == 0) {
            continue;
        }
        dataP->time--;
        if(dataP->time < 10) {
            dataP->scale *= 0.95f;
            dataP->color.a *= 0.7f;
            if(dataP->time == 0) {
                dataP->color.a = 0;
                dataP->scale = 0.0f;
            }
        } else if(particleP->endF == FALSE) {
            t = dataP->color.a;
            t = 1.0f + (t + (0.3f * (dataP->alpha - t)));
            dataP->color.a = t;
        }
        if(dataP->color.r < 250) {
            dataP->color.r += 5;
        }
        if(dataP->color.g < 250) {
            dataP->color.g += 5;
        }
        if(dataP->color.b < 250) {
            dataP->color.b += 5;
        }
        if(particleP->endF != FALSE) {
            dataP->color.a *= 0.8f;
        }
    }
}

static void GuideFadeOutEffectHook(HU3D_MODEL *modelP, GUIDEPARTICLE *particleP, Mtx mtx)
{
    static u8 effNo[16] = { 0, 1, 2, 2, 3, 3, 3, 3, 0, 1, 2, 2, 3, 3, 3, 3 };
    static float effSize[4] = { 0.9f, 0.9f, 0.9f, 0.8f };
    static GXColor effColor[8] = {
        { 220, 64, 64, 0 },
        { 64, 220, 64, 0 },
        { 220, 220, 64, 0 },
        { 64, 64, 220, 0 },
        { 220, 64, 220, 0 },
        { 64, 220, 220, 0 },
        { 220, 120, 64, 0 },
        { 64, 120, 220, 0 },
    };
    GUIDEPARTICLEDATA *dataP;
    int i;
    int num;
    int colorNo;
    int no;
    float t;
    float t2;
    HuVecF vec;
    Mtx rotMtx;

    if(particleP->initF == 0) {
        particleP->initF = 1;
        particleP->endF = FALSE;
        particleP->unk14 = 0.0f;
    }
    num = 0;
    if(particleP->endF == FALSE) {
        num = 6;
    }
    for(dataP = particleP->data, i = 0; i < particleP->num; i++, dataP++) {
        if(num <= 0) {
            break;
        }
        if(dataP->time != 0) {
            continue;
        }
        no = effNo[mbRandMod(16)];
        dataP->animNo = no;
        t = 360.0f * frandf();
        mbMtxRotAxisDeg(rotMtx, 'Y', t);
        vec.x = vec.y = 0.0f;
        vec.z = (1.0f / 60.0f) * (600.0f * (0.7f + (0.3f * frandf())));
        MTXMultVec(rotMtx, &vec, &dataP->vel);
        t = 360.0f * frandf();
        t2 = 0.7f + (0.3f * frandf());
        vec.x = t2 * mbCosDeg(t);
        vec.y = 0.2f + (t2 * mbSinDeg(t));
        vec.z = -0.8f;
        MTXMultVec(rotMtx, &vec, &dataP->accel);
        VECScale(&dataP->accel, &dataP->accel, 0.55555564f);
        VECScale(&dataP->vel, &dataP->pos, 1.0f);
        dataP->pos.y += 5.0f;
        dataP->scale = effSize[no] * (30.0f * (0.5f + (0.5f * frandf())));
        dataP->scaleBase = dataP->scale;
        colorNo = mbRandMod(8);
        dataP->colorNo = colorNo;
        t = 0.8f + (0.2f * frandf());
        dataP->color.r = (t * (255.0f - effColor[colorNo].r)) + effColor[colorNo].r;
        dataP->color.g = (t * (255.0f - effColor[colorNo].g)) + effColor[colorNo].g;
        dataP->color.b = (t * (255.0f - effColor[colorNo].b)) + effColor[colorNo].b;
        dataP->color.a = mbRandMod(70) + 150;
        dataP->alpha = dataP->color.a;
        dataP->color.a = 0;
        dataP->zRot = 0.0f;
        if(dataP->animNo < 3) {
            dataP->zRot = 360.0f * frandf();
        }
        dataP->time = 20;
        num--;
    }
    for(dataP = particleP->data, i = 0; i < particleP->num; i++, dataP++) {
        if(dataP->time == 0) {
            continue;
        }
        VECAdd(&dataP->pos, &dataP->vel, &dataP->pos);
        VECAdd(&dataP->vel, &dataP->accel, &dataP->vel);
    }
    for(dataP = particleP->data, i = 0; i < particleP->num; i++, dataP++) {
        if(dataP->time == 0) {
            continue;
        }
        dataP->time--;
        if(dataP->time < 10) {
            dataP->scale *= 0.95f;
            dataP->color.a *= 0.7f;
            if(dataP->time == 0) {
                dataP->color.a = 0;
                dataP->scale = 0.0f;
            }
        } else if(particleP->endF == FALSE) {
            t = dataP->color.a;
            t = 1.0f + (t + (0.3f * (dataP->alpha - t)));
            dataP->color.a = t;
        }
        colorNo = dataP->colorNo;
        dataP->color.r = dataP->color.r + (0.1f * ((float)effColor[colorNo].r - dataP->color.r));
        dataP->color.g = dataP->color.g + (0.1f * ((float)effColor[colorNo].g - dataP->color.g));
        dataP->color.b = dataP->color.b + (0.1f * ((float)effColor[colorNo].b - dataP->color.b));
        if(particleP->endF != FALSE) {
            dataP->color.a *= 0.8f;
        }
    }
}
