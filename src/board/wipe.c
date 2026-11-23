#include "game/board/wipe.h"
#include "game/board/exit.h"
#include "game/board/malloc.h"
#include "game/board/math.h"
#include "game/board/tutorial.h"
#include "game/wipe.h"
#include "game/flag.h"
#include "game/data.h"
#include "game/memory.h"
#include "game/hu3d.h"
#include "game/sprite.h"
#include "game/init.h"
#include "string.h"

typedef struct WipeSpecial_s WIPESPECIAL;
typedef void (*WIPESPECIAL_FUNC)(HU3D_MODEL *modelP, Mtx *mtx, WIPESPECIAL *work);

struct WipeSpecial_s {
    int activeF; //0x00
    int mode; //0x04
    int stat; //0x08
    s16 type; //0x0C
    s16 time; //0x0E
    s16 maxTime; //0x10
    HU3D_MODELID masuMdlId[3]; //0x12
    HU3D_MODELID hookMdlId; //0x18
    u32 texSize; //0x1C
    void *texBuf; //0x20
    void *work; //0x24
    int drawType; //0x28
};

typedef struct WipeGridCell_s {
    HuVecF pos;
    Mtx mtx;
} WIPEGRID_CELL;

typedef struct WipeGrid_s {
    int w;
    int h;
    int num;
    WIPEGRID_CELL cell[88];
} WIPEGRID;

typedef struct WipePaperStrip_s {
    float dist; //0x00
    float rot; //0x04
    float angle; //0x08
    HuVecF p0; //0x0C
    HuVecF p1; //0x18
    HuVec2f st0; //0x24
    HuVec2f st1; //0x2C
} WIPEPAPER_STRIP;

typedef struct WipePaper_s {
    HuVecF start; //0x00
    HuVecF end; //0x0C
    HuVecF dir; //0x18
    HuVecF normal; //0x24
    float len; //0x30
    WIPEPAPER_STRIP strip[64]; //0x34
} WIPEPAPER;

typedef struct WipeImage_s {
    int no; //0x00
    float x; //0x04
    float y; //0x08
    float unk0C; //0x0C
    float size; //0x10
    float alpha; //0x14
} WIPEIMAGE;

static void WipeShapeUpdate(HU3D_MODEL *modelP, Mtx *mtx, WIPESPECIAL *work);
static void WipeGridUpdate(HU3D_MODEL *modelP, Mtx *mtx, WIPESPECIAL *work);
static void WipePaperUpdate(HU3D_MODEL *modelP, Mtx *mtx, WIPESPECIAL *work);
static void WipeImageUpdate(HU3D_MODEL *modelP, Mtx *mtx, WIPESPECIAL *work);

static int wipeMasuFileTbl[3] = { 0x40007, 0x40008, 0x40009 };
static int wipeImageFileTbl[3] = { 0x4000A, 0x4000C, 0x4000B };
static WIPESPECIAL_FUNC fadeFunc[8] = {
    WipeShapeUpdate,
    WipeShapeUpdate,
    WipeShapeUpdate,
    WipeGridUpdate,
    WipePaperUpdate,
    WipeImageUpdate,
    WipeImageUpdate,
    WipeImageUpdate
};
static HuVec2f paperST[4] = {
    { 0.6f, -1.0f },
    { -0.6f, 1.0f },
    { 0.6f, 1.0f },
    { -0.6f, -1.0f }
};
static HuVecF paperPos[4] = {
    { 0.0f, 0.0f, 0.0f },
    { 640.0f, 0.0f, 0.0f },
    { 640.0f, 480.0f, 0.0f },
    { 0.0f, 480.0f, 0.0f }
};
static HuVecF paperLightPos = { 0.0f, 0.0f, 1000.0f };
static HuVecF paperLightDir = { 0.0f, 0.0f, -1.0f };
static GXColor paperLightColor = { 255, 255, 255, 255 };
static GXColor paperMatColor = { 255, 255, 255, 255 };
static GXColor paperAmbColor = { 192, 192, 192, 255 };

static WIPESPECIAL wipeSpecialData;
static ANIMDATA *wipeImageAnim[3];

static inline BOOL WipeCreateCheck(s16 mode, s16 type, s16 maxTime)
{
    if(_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        return FALSE;
    }
    WipeCreate(mode, type, maxTime);
    return TRUE;
}

void mbWipeCreate(s16 mode, s16 type, s16 maxTime)
{
    if(_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        return;
    }
    WipeCreate(mode, type, maxTime);
}

void mbWipeWait(void)
{
    WipeWait();
}

void mbWipeFadeOut(void)
{
    if(WipeCreateCheck(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 21)) {
        WipeColorSet(0, 0, 0);
        WipeWait();
    }
}

void mbWipeFadeIn(void)
{
    if(WipeCreateCheck(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 21)) {
        WipeWait();
    }
}

void mbWipeWhiteFadeOut(void)
{
    if(WipeCreateCheck(WIPE_MODE_OUT, WIPE_TYPE_WHITE, 21)) {
        WipeWait();
    }
}

void mbWipeWhiteFadeIn(void)
{
    if(WipeCreateCheck(WIPE_MODE_IN, WIPE_TYPE_WHITE, 21)) {
        WipeWait();
    }
}

void mbWipeDissolveFadeOut(void)
{
    if(WipeCreateCheck(WIPE_MODE_OUT, WIPE_TYPE_CROSS_COPY, 1)) {
        WipeWait();
    }
}

void mbWipeDissolveFadeIn(void)
{
    if(WipeCreateCheck(WIPE_MODE_IN, WIPE_TYPE_CROSS_COPY, 30)) {
        WipeWait();
    }
}

void mbWipeFadeOutTime(int maxTime)
{
    if(WipeCreateCheck(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, maxTime)) {
        WipeColorSet(0, 0, 0);
        WipeWait();
    }
}

void mbWipeFadeInTime(int maxTime)
{
    if(WipeCreateCheck(WIPE_MODE_IN, WIPE_TYPE_NORMAL, maxTime)) {
        WipeWait();
    }
}

void mbWipeWhiteFadeOutTime(int maxTime)
{
    if(WipeCreateCheck(WIPE_MODE_OUT, WIPE_TYPE_WHITE, maxTime)) {
        WipeWait();
    }
}

void mbWipeWhiteFadeInTime(int maxTime)
{
    if(WipeCreateCheck(WIPE_MODE_IN, WIPE_TYPE_WHITE, maxTime)) {
        WipeWait();
    }
}

void mbWipeDissolveFadeOutTime(int maxTime)
{
    if(WipeCreateCheck(WIPE_MODE_OUT, WIPE_TYPE_CROSS_COPY, maxTime)) {
        WipeWait();
    }
}

void mbWipeDissolveFadeInTime(int maxTime)
{
    if(WipeCreateCheck(WIPE_MODE_IN, WIPE_TYPE_CROSS_COPY, maxTime)) {
        WipeWait();
    }
}

static inline void *WipeMalloc(s32 size)
{
    return HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
}

static inline void WipeFree(void *ptr)
{
    HuMemDirectFree(ptr);
}

static void WipeMasuMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *material);
static void WipeSpecialDraw(HU3D_MODEL *modelP, Mtx *mtx);

void mbWipeSpecialInit(void)
{
    WIPESPECIAL *work = &wipeSpecialData;
    int i;
    memset(work, 0, sizeof(WIPESPECIAL));
    for(i=0; i<3; i++) {
        work->masuMdlId[i] = Hu3DModelCreate(HuDataSelHeapReadNum(wipeMasuFileTbl[i], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(work->masuMdlId[i], HU3D_ATTR_DISPOFF);
        Hu3DModelMatHookSet(work->masuMdlId[i], WipeMasuMatHook);
        Hu3DModelPosSet(work->masuMdlId[i], 0.0f, 0.0f, -100.0f);
        Hu3DModelCameraSet(work->masuMdlId[i], HU3D_CAM2);
        Hu3DModelLayerSet(work->masuMdlId[i], 7);
    }
    work->texSize = GXGetTexBufferSize(320, 240, GX_TF_RGB565, GX_FALSE, 0);
    work->texBuf = WipeMalloc(work->texSize);
    DCFlushRange(work->texBuf, work->texSize);
    work->hookMdlId = Hu3DHookFuncCreate(WipeSpecialDraw);
    Hu3DModelCameraSet(work->hookMdlId, HU3D_CAM2);
    Hu3DModelLayerSet(work->hookMdlId, 6);
    for(i=0; i<3; i++) {
        wipeImageAnim[i] = HuSprAnimRead(HuDataSelHeapReadNum(wipeImageFileTbl[i], HU_MEMNUM_OVL, HEAP_MODEL));
        HuSprAnimLock(wipeImageAnim[i]);
    }
}

void mbWipeSpecialClose(void)
{
    WIPESPECIAL *work = &wipeSpecialData;
    int i;
    mbWipeSpecialKill();
    if(work->texBuf) {
        WipeFree(work->texBuf);
        work->texBuf = NULL;
    }
    for(i=0; i<3; i++) {
        HuSprAnimKill(wipeImageAnim[i]);
        wipeImageAnim[i] = NULL;
    }
}

void mbWipeSpecialKill(void)
{
    WIPESPECIAL *work = &wipeSpecialData;
    if(work->work) {
        WipeFree(work->work);
        work->work = NULL;
    }
    work->type = 0;
    work->activeF = FALSE;
    work->drawType = 0;
    work->stat = 0;
}

static void WipeMasuMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *material)
{
    HU3D_CAMERA *cameraP = &Hu3DCamera[Hu3DCameraNo];
    WIPESPECIAL *work = &wipeSpecialData;
    float tanFov;
    Mtx texMtx;
    GXColor color;
    GXSetNumTexGens(1);
    tanFov = HuSin(0.5f*cameraP->fov)/HuCos(0.5f*cameraP->fov);
    MTXIdentity(texMtx);
    texMtx[0][0] = (-0.5f*drawObj->matrix[0][0])/(cameraP->aspect*(tanFov*drawObj->matrix[2][3]));
    texMtx[1][1] = (0.5f*drawObj->matrix[1][1])/(tanFov*drawObj->matrix[2][3]);
    texMtx[0][3] = 0.5f;
    texMtx[1][3] = 0.5f;
    GXLoadTexMtxImm(texMtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_POS, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    Hu3DTexLoad(work->texBuf, 320, 240, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE, GX_TEXMAP0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0);
    color.a = 255.0f*(1.0f-material->invAlpha);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    Hu3DMatLightSet(drawObj->model, 2, material->hiliteScale);
}

static void WipeGridDraw(HU3D_MODEL *modelP, Mtx *mtx, WIPESPECIAL *work);
static void WipePaperDraw(HU3D_MODEL *modelP, Mtx *mtx, WIPESPECIAL *work);
static void WipeImageDraw(HU3D_MODEL *modelP, Mtx *mtx, WIPESPECIAL *work);

static void WipeSpecialDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    WIPESPECIAL *work = &wipeSpecialData;
    if(mbExitCheck()) {
        return;
    }
    if(work->activeF && fadeFunc[work->type-1]) {
        fadeFunc[work->type-1](modelP, mtx, work);
    }
    switch(work->drawType) {
        case 4:
            WipeGridDraw(modelP, mtx, work);
            break;

        case 5:
            WipePaperDraw(modelP, mtx, work);
            break;

        case 6:
        case 7:
        case 8:
            WipeImageDraw(modelP, mtx, work);
            break;
    }
}

static void WipeShapeUpdate(HU3D_MODEL *modelP, Mtx *mtx, WIPESPECIAL *work)
{
    float t;
    float scale;
    if(work->mode == WIPE_MODE_OUT) {
        t = (float)work->time/work->maxTime;
        if(!mbPauseEnableCheck()) {
            work->time++;
        }
        scale = 0.5*(1.0-HuSin(90.0f*t));
        Hu3DModelScaleSet(work->masuMdlId[work->type-1], scale, scale, scale);
        if(work->time > work->maxTime) {
            Hu3DModelAttrSet(work->masuMdlId[work->type-1], HU3D_ATTR_DISPOFF);
            work->type = 0;
            work->activeF = FALSE;
            work->drawType = 0;
            work->stat = 0;
        }
    } else {
        Hu3DModelAttrReset(work->masuMdlId[work->type-1], HU3D_ATTR_DISPOFF);
        Hu3DModelPosSet(work->masuMdlId[work->type-1], 0.0f, 0.0f, -100.0f);
        Hu3DModelScaleSet(work->masuMdlId[work->type-1], 1.0f, 1.0f, 1.0f);
        GXSetCopyFilter(GX_TRUE, RenderMode->sample_pattern, GX_TRUE, RenderMode->vfilter);
        Hu3DFbCopyExec(0, 0, 640, 480, GX_TF_RGB565, TRUE, work->texBuf);
        GXPixModeSync();
        GXSetCopyFilter(RenderMode->aa, RenderMode->sample_pattern, GX_TRUE, RenderMode->vfilter);
        work->type = 0;
        work->activeF = FALSE;
        work->stat = 1;
    }
}

static void WipeGridUpdate(HU3D_MODEL *modelP, Mtx *mtx, WIPESPECIAL *work)
{
    WIPEGRID *grid;
    WIPEGRID_CELL *cell;
    int i;
    int j;
    float t;
    float scale;
    if(mbPauseEnableCheck()) {
        return;
    }
    if(work->mode == WIPE_MODE_OUT) {
        grid = work->work;
        t = (float)work->time/work->maxTime;
        if(!mbPauseEnableCheck()) {
            work->time++;
        }
        scale = 1.0-HuSin(90.0f*t);
        for(cell=grid->cell, i=0; i<grid->num; i++) {
            MTXScale(cell->mtx, scale, scale, 1.0f);
            mtxRotCat(cell->mtx, 0.0f, 0.0f, 180.0f*t);
            cell++;
        }
        if(work->time > work->maxTime) {
            WipeFree(work->work);
            work->work = NULL;
            work->type = 0;
            work->activeF = FALSE;
            work->drawType = 0;
            work->stat = 0;
        }
    } else {
        grid = mbMalloc(sizeof(WIPEGRID));
        work->work = grid;
        grid->w = 11;
        grid->h = 8;
        grid->num = grid->w*grid->h;
        for(cell=grid->cell, i=0; i<8; i++) {
            for(j=0; j<11; j++) {
                cell->pos.x = (320.0f/(grid->w-1))+((640.0f*j)/(grid->w-1));
                cell->pos.y = (240.0f/(grid->h-1))+((480.0f*i)/(grid->h-1));
                cell->pos.z = -100.0f;
                MTXIdentity(cell->mtx);
                cell++;
            }
        }
        GXSetCopyFilter(GX_TRUE, RenderMode->sample_pattern, GX_TRUE, RenderMode->vfilter);
        Hu3DFbCopyExec(0, 0, 640, 480, GX_TF_RGB565, TRUE, work->texBuf);
        GXPixModeSync();
        GXSetCopyFilter(RenderMode->aa, RenderMode->sample_pattern, GX_TRUE, RenderMode->vfilter);
        work->type = 0;
        work->activeF = FALSE;
        work->stat = 1;
    }
}

static void WipePaperVtxUpdate(WIPEPAPER *paper, float t)
{
    WIPEPAPER_STRIP *strip;
    int i;
    float dist;
    float radius;
    float angle;
    float ofs;
    HuVec2f dir;
    HuVec2f pos[64];

    strip = paper->strip;
    radius = 200.0f*(1.0f-t);
    ofs = t*(paper->len+(3.1415927f*radius));
    for(i=0; i<64; i++, strip++) {
        dist = strip->dist+ofs;
        if(dist < paper->len) {
            pos[i].x = dist;
            pos[i].y = 0.0f;
            strip->angle = 0.0f;
        } else if(dist < paper->len+(3.1415927f*radius)) {
            dist -= paper->len;
            angle = 360.0f*(dist/(6.2831855f*radius));
            pos[i].x = paper->len+(radius*mbSinDeg(angle));
            pos[i].y = radius*(1.0f-mbCosDeg(angle));
            if(angle > 90.0f) {
                angle = 180.0f-angle;
            }
            strip->angle = angle;
        } else {
            dist -= paper->len+(3.1415927f*radius);
            pos[i].x = paper->len-dist;
            pos[i].y = 2.0f*radius;
            strip->angle = 0.0f;
        }
    }
    strip = paper->strip;
    for(i=0; i<63; i++, strip++) {
        dir.x = pos[i+1].x-pos[i].x;
        dir.y = pos[i+1].y-pos[i].y;
        strip->rot = HuAtan(dir.y, dir.x);
    }
}

static void WipePaperUpdate(HU3D_MODEL *modelP, Mtx *mtx, WIPESPECIAL *work)
{
    WIPEPAPER *paper;
    WIPEPAPER_STRIP *strip;
    int i;
    int j;
    int k;
    int playerNo;
    float t;
    float min;
    float max;
    float ofs[4];
    HuVec2f start;
    HuVec2f end;
    HuVec2f dir;
    HuVec2f pos;

    if(mbPauseEnableCheck()) {
        return;
    }
    if(work->mode == WIPE_MODE_OUT) {
        paper = work->work;
        t = (float)work->time/work->maxTime;
        if(!mbPauseEnableCheck()) {
            work->time++;
        }
        WipePaperVtxUpdate(paper, t);
        if(work->time > work->maxTime) {
            WipeFree(work->work);
            work->work = NULL;
            work->type = 0;
            work->activeF = FALSE;
            work->drawType = 0;
            work->stat = 0;
        }
    } else {
        paper = mbMalloc(sizeof(WIPEPAPER));
        work->work = paper;
        playerNo = GwSystem.turnPlayerNo;
        if(playerNo < 0) {
            playerNo = 0;
        } else if(playerNo > 3) {
            playerNo = 3;
        }
        dir = paperST[playerNo];
        start.x = 320.0f-(400.0f*dir.x);
        start.y = 240.0f-(400.0f*dir.y);
        end.x = 320.0f+(400.0f*dir.x);
        end.y = 240.0f+(400.0f*dir.y);
        VECSubtract((Vec *)&end, (Vec *)&start, (Vec *)&dir);
        min = 9999.0f;
        max = -9999.0f;
        for(i=0; i<4; i++) {
            t = ((dir.y*paperPos[i].y)+((dir.x*paperPos[i].x)-(start.x*dir.x))-(start.y*dir.y))/((dir.x*dir.x)+(dir.y*dir.y));
            if(t < min) {
                min = t;
            }
            if(t > max) {
                max = t;
            }
        }
        paper->start.x = start.x+(min*dir.x);
        paper->start.y = start.y+(min*dir.y);
        paper->end.x = start.x+(max*dir.x);
        paper->end.y = start.y+(max*dir.y);
        VECSubtract(&paper->end, &paper->start, &paper->dir);
        paper->len = VECMag(&paper->dir);
        paper->normal.x = paper->dir.y;
        paper->normal.y = -paper->dir.x;
        paper->normal.z = 0.0f;
        dir.x = paper->dir.y;
        dir.y = -paper->dir.x;
        for(strip=paper->strip, i=0; i<64; i++, strip++) {
            t = i/63.0f;
            strip->dist = paper->len*t;
            pos.x = paper->start.x+(t*paper->dir.x);
            pos.y = paper->start.y+(t*paper->dir.y);
            ofs[0] = -pos.x/dir.x;
            ofs[1] = (640.0f-pos.x)/dir.x;
            ofs[2] = -pos.y/dir.y;
            ofs[3] = (480.0f-pos.y)/dir.y;
            for(j=0; j<3; j++) {
                for(k=j; k<4; k++) {
                    if(ofs[j] > ofs[k]) {
                        t = ofs[j];
                        ofs[j] = ofs[k];
                        ofs[k] = t;
                    }
                }
            }
            strip->p0.x = pos.x+(ofs[1]*dir.x);
            strip->p0.y = pos.y+(ofs[1]*dir.y);
            strip->p0.z = -500.0f;
            strip->st0.x = strip->p0.x/640.0f;
            strip->st0.y = strip->p0.y/480.0f;
            strip->p1.x = pos.x+(ofs[2]*dir.x);
            strip->p1.y = pos.y+(ofs[2]*dir.y);
            strip->p1.z = -500.0f;
            strip->st1.x = strip->p1.x/640.0f;
            strip->st1.y = strip->p1.y/480.0f;
        }
        GXSetCopyFilter(GX_TRUE, RenderMode->sample_pattern, GX_TRUE, RenderMode->vfilter);
        Hu3DFbCopyExec(0, 0, 640, 480, GX_TF_RGB565, TRUE, work->texBuf);
        GXPixModeSync();
        GXSetCopyFilter(RenderMode->aa, RenderMode->sample_pattern, GX_TRUE, RenderMode->vfilter);
        work->type = 0;
        work->activeF = FALSE;
        work->stat = 1;
    }
}

static void WipeImageUpdate(HU3D_MODEL *modelP, Mtx *mtx, WIPESPECIAL *work)
{
    WIPEIMAGE *image;
    float t;
    if(mbPauseEnableCheck()) {
        return;
    }
    if(work->mode == WIPE_MODE_OUT) {
        image = work->work;
        t = (float)work->time/work->maxTime;
        if(!mbPauseEnableCheck()) {
            work->time++;
        }
        image->size = 1000.0f*t;
        if(work->time > work->maxTime) {
            if(work->work) {
                WipeFree(work->work);
                work->work = NULL;
            }
            work->type = 0;
            work->activeF = FALSE;
            work->drawType = 0;
            work->stat = 0;
        }
    } else {
        if(!work->work) {
            image = mbMalloc(sizeof(WIPEIMAGE));
            work->work = image;
            image->x = 288.0f;
            image->y = 240.0f;
            image->alpha = 1.0f;
            image->no = work->type-6;
        } else {
            image = work->work;
        }
        t = (float)work->time/work->maxTime;
        if(!mbPauseEnableCheck()) {
            work->time++;
        }
        image->size = 1000.0f*(1.0f-t);
        if(work->time > work->maxTime) {
            work->type = 0;
            work->activeF = FALSE;
            work->stat = 1;
            return;
        }
    }
}

static void WipeGridDraw(HU3D_MODEL *modelP, Mtx *mtx, WIPESPECIAL *work)
{
    WIPEGRID *grid = work->work;
    GXColor color = { 255, 255, 255, 255 };
    Mtx modelMtx;
    Mtx44 proj;
    WIPEGRID_CELL *cell;
    int i;
    int j;
    float w;
    float h;

    C_MTXOrtho(proj, 0, 480, 0, 640, 0, 1000);
    GXSetProjection(proj, GX_ORTHOGRAPHIC);
    GXSetViewport(0, 0, 640, 480, 0, 1);
    GXSetScissor(0, 0, 640, 480);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_1);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    Hu3DTexLoad(work->texBuf, 320, 240, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE, GX_TEXMAP0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    w = 640.0f/(grid->w-1);
    h = 480.0f/(grid->h-1);
    for(i=0; i<grid->h-1; i++) {
        for(j=0; j<grid->w-1; j++) {
            cell = &grid->cell[(i*grid->w)+j];
            MTXTrans(modelMtx, cell->pos.x, cell->pos.y, cell->pos.z);
            MTXConcat(modelMtx, cell->mtx, modelMtx);
            GXLoadPosMtxImm(modelMtx, GX_PNMTX0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(0.5f*-w, 0.5f*-h, 0.0f);
            GXTexCoord2f32((float)j/(grid->w-1), (float)i/(grid->h-1));
            GXPosition3f32(0.5f*w, 0.5f*-h, 0.0f);
            GXTexCoord2f32((float)(j+1)/(grid->w-1), (float)i/(grid->h-1));
            GXPosition3f32(0.5f*w, 0.5f*h, 0.0f);
            GXTexCoord2f32((float)(j+1)/(grid->w-1), (float)(i+1)/(grid->h-1));
            GXPosition3f32(0.5f*-w, 0.5f*h, 0.0f);
            GXTexCoord2f32((float)j/(grid->w-1), (float)(i+1)/(grid->h-1));
            GXEnd();
        }
    }
}

static void WipePaperDraw(HU3D_MODEL *modelP, Mtx *mtx, WIPESPECIAL *work)
{
    WIPEPAPER *paper = work->work;
    WIPEPAPER_STRIP *strip = paper->strip;
    GXColor color = { 255, 255, 255, 255 };
    int i;
    Mtx rotMtx;
    Mtx nrmMtx;
    GXLightObj lightObj;
    Mtx44 proj;
    HuVecF prev1;
    HuVecF prev0;
    HuVecF next1;
    HuVecF next0;
    HuVecF d1;
    HuVecF d0;
    HuVecF nrm;

    C_MTXOrtho(proj, 0, 480, 0, 640, 0, 1000);
    GXSetProjection(proj, GX_ORTHOGRAPHIC);
    GXSetViewport(0, 0, 640, 480, 0, 1);
    GXSetScissor(0, 0, 640, 480);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_1);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    memset(&lightObj, 0, sizeof(GXLightObj));
    GXInitLightAttnK(&lightObj, 1.0f, 0.0f, 0.0f);
    GXInitLightDistAttn(&lightObj, 0.0f, 1.0f, GX_DA_OFF);
    GXInitLightPos(&lightObj, paperLightPos.x, paperLightPos.y, paperLightPos.z);
    GXInitLightDir(&lightObj, paperLightDir.x, paperLightDir.y, paperLightDir.z);
    GXInitLightColor(&lightObj, paperLightColor);
    GXLoadLightObjImm(&lightObj, GX_LIGHT0);
    Hu3DTexLoad(work->texBuf, 320, 240, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE, GX_TEXMAP0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_TRUE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetChanMatColor(GX_COLOR0A0, paperMatColor);
    GXSetChanAmbColor(GX_COLOR0A0, paperAmbColor);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_NRM, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    prev0 = strip->p0;
    prev1 = strip->p1;
    MTXIdentity(rotMtx);
    GXLoadPosMtxImm(rotMtx, GX_PNMTX0);
    MTXInvXpose(rotMtx, nrmMtx);
    GXLoadNrmMtxImm(nrmMtx, GX_PNMTX0);
    for(i=0; i<63; i++, strip++) {
        VECSubtract(&strip[1].p0, &strip->p0, &d0);
        VECSubtract(&strip[1].p1, &strip->p1, &d1);
        MTXRotAxisRad(rotMtx, &paper->normal, 0.017453292f*strip->rot);
        MTXMultVec(rotMtx, &d0, &d0);
        MTXMultVec(rotMtx, &d1, &d1);
        VECAdd(&prev0, &d0, &next0);
        VECAdd(&prev1, &d1, &next1);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        MTXRotAxisRad(rotMtx, &paper->normal, 0.017453292f*(90.0f+strip->angle));
        MTXMultVec(rotMtx, &paper->dir, &nrm);
        GXPosition3f32(prev0.x, prev0.y, prev0.z);
        GXNormal3f32(nrm.x, nrm.y, nrm.z);
        GXTexCoord2f32(strip->st0.x, strip->st0.y);
        GXPosition3f32(prev1.x, prev1.y, prev1.z);
        GXNormal3f32(nrm.x, nrm.y, nrm.z);
        GXTexCoord2f32(strip->st1.x, strip->st1.y);
        MTXRotAxisRad(rotMtx, &paper->normal, 0.017453292f*(90.0f+strip[1].angle));
        MTXMultVec(rotMtx, &paper->dir, &nrm);
        GXPosition3f32(next1.x, next1.y, next1.z);
        GXNormal3f32(nrm.x, nrm.y, nrm.z);
        GXTexCoord2f32(strip[1].st1.x, strip[1].st1.y);
        GXPosition3f32(next0.x, next0.y, next0.z);
        GXNormal3f32(nrm.x, nrm.y, nrm.z);
        GXTexCoord2f32(strip[1].st0.x, strip[1].st0.y);
        GXEnd();
        prev0 = next0;
        prev1 = next1;
    }
}

static void WipeImageDraw(HU3D_MODEL *modelP, Mtx *mtx, WIPESPECIAL *work)
{
    GXColor color = { 255, 255, 255, 255 };
    WIPEIMAGE *image = work->work;
    Mtx44 imageProj;
    Mtx modelMtx;
    Mtx44 proj;
    float half;
    float x0;
    float y0;
    float x1;
    float y1;

    C_MTXOrtho(proj, 0, 480, 0, 640, 0, 1000);
    GXSetProjection(proj, GX_ORTHOGRAPHIC);
    GXSetViewport(0, 0, 640, 480, 0, 1);
    GXSetScissor(0, 0, 640, 480);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_1);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    C_MTXOrtho(imageProj, 0, 480, 0, 576, 0, 1000);
    GXSetProjection(imageProj, GX_ORTHOGRAPHIC);
    HuSprTexLoad(wipeImageAnim[image->no], 0, GX_TEXMAP0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    MTXIdentity(modelMtx);
    GXLoadPosMtxImm(modelMtx, GX_PNMTX0);
    half = 0.5f*image->size;
    x0 = image->x-half;
    y0 = image->y-half;
    x1 = image->x+half;
    y1 = image->y+half;
    color.a = 255.0f*image->alpha;
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    if(y0 > 0.0f) {
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition2f32(0.0f, 0.0f);
        GXPosition2f32(576.0f, 0.0f);
        GXPosition2f32(576.0f, y0);
        GXPosition2f32(0.0f, y0);
        GXEnd();
    }
    if(y1 < 480.0f) {
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition2f32(0.0f, y1);
        GXPosition2f32(576.0f, y1);
        GXPosition2f32(576.0f, 480.0f);
        GXPosition2f32(0.0f, 480.0f);
        GXEnd();
    }
    if(x0 > 0.0f) {
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition2f32(0.0f, y0);
        GXPosition2f32(x0, y0);
        GXPosition2f32(x0, y1);
        GXPosition2f32(0.0f, y1);
        GXEnd();
    }
    if(x1 < 576.0f) {
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition2f32(x1, y0);
        GXPosition2f32(576.0f, y0);
        GXPosition2f32(576.0f, y1);
        GXPosition2f32(x1, y1);
        GXEnd();
    }
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition2f32(x0, y0);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition2f32(x1, y0);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition2f32(x1, y1);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition2f32(x0, y1);
    GXTexCoord2f32(0.0f, 1.0f);
    GXEnd();
}

static inline BOOL WipeSpecialCreate(int mode, int type, s16 maxTime)
{
    WIPESPECIAL *work = &wipeSpecialData;
    if(_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        return FALSE;
    }
    work->mode = mode;
    work->type = type;
    work->time = 0;
    work->maxTime = maxTime;
    work->activeF = TRUE;
    work->drawType = type;
    if(mode == WIPE_MODE_IN) {
        if(work->work) {
            WipeFree(work->work);
        }
        work->work = NULL;
    }
    return TRUE;
}

void mbWipeSpecialCreate(int mode, int type, int maxTime)
{
    WIPESPECIAL *work = &wipeSpecialData;
    if(_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        return;
    }
    work->mode = mode;
    work->type = type;
    work->time = 0;
    work->maxTime = maxTime;
    work->activeF = TRUE;
    work->drawType = type;
    if(mode == WIPE_MODE_IN) {
        if(work->work) {
            WipeFree(work->work);
        }
        work->work = NULL;
    }
}

void mbWipeSpecialFadeOutCreate(int type, int maxTime)
{
    if(WipeSpecialCreate(WIPE_MODE_OUT, type, maxTime)) {
        mbWipeSpecialWait();
    }
}

void mbWipeSpecialFadeInCreate(int type, int maxTime)
{
    if(WipeSpecialCreate(WIPE_MODE_IN, type, maxTime)) {
        mbWipeSpecialWait();
    }
}

int mbWipeSpecialCheck(void)
{
    WIPESPECIAL *work = &wipeSpecialData;
    return work->activeF | WipeCheck();
}

void mbWipeSpecialWait(void)
{
    while(mbWipeSpecialCheck()) {
        HuPrcVSleep();
    }
}

int mbWipeSpecialStatGet(void)
{
    return wipeSpecialData.stat | WipeCheckIn();
}
