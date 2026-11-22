#include "game/board/scroll.h"
#include "game/board/audio.h"
#include "game/board/branch.h"
#include "game/board/camera.h"
#include "game/board/effect.h"
#include "game/board/malloc.h"
#include "game/board/math.h"
#include "game/board/object.h"
#include "game/board/pause.h"
#include "game/board/player.h"
#include "game/board/status.h"
#include "game/board/telop.h"
#include "game/board/window.h"
#include "game/board/wipe.h"
#include "game/esprite.h"
#include "game/sprite.h"
#include "game/hsfformat.h"
#include "game/gamework.h"
#include "game/pad.h"
#include "game/data.h"
#include "game/memory.h"
#include "math.h"
#include "string.h"

/* masu.h is not included: these calls disagree with its placeholder prototypes. */
int mbMasuNumGet(void);
u16 mbMasuAttrGet(s16 id);
int mbMasuMAttrGet(s16 id);
int mbMasuTypeGet(s16 id);
void mbMasuPosGet(s16 id, HuVecF *pos);
BOOL mbMasuDispGet(void);
u16 mbMasuDispAttrGet(void);
int mbMasuDispMAttrGet(void);
void mbMasuModelDispSet(int dispF);
s16 mbMasuLinkGet(s16 id, int linkNo);
s16 mbMasuLinkNumGet(s16 id);
s16 mbMasuFind_TypeIdGet(s16 id, s16 type, int arg2, int arg3);
s16 mbMasuFind_AttrIdGet(s16 id, u16 attr);
int mbMasuTypeListGet(s16 type, s16 *list);

#define MAPSPR_MAX 32
#define MAPSPR_SUB_MAX 1

typedef struct MapSpr_s {
    BOOL usedF; //0x00
    BOOL dispF; //0x04
    s16 sprId; //0x08
    s16 sprIdBack; //0x0A
    u32 type; //0x0C
    int charNo; //0x10
    int masuId; //0x14
    GXColor color; //0x18
    s16 sprIdSub[MAPSPR_SUB_MAX]; //0x1C
    HuVecF pos; //0x20
    HuVecF dispPos; //0x2C
    HuVecF ofs; //0x38
} MAPSPR; //size=0x44

typedef struct ScrollWork_s {
    int sprNum; //0x00
    int mode; //0x04
    int masuTime; //0x08
    int pathTime; //0x0C
    float pathScroll; //0x10
    MAPSPR spr[MAPSPR_MAX]; //0x14
} SCROLLWORK; //size=0x894

static float mapViewZoom;
static int lbl_802C0DD8;
static HU3D_MODELID scrollColModel;
static MBSCROLLSTARFINDFUNC scrollStarFindFunc;
static int scrollColTriNum;
static HSF_FACE *scrollColTriData;
static MBSCROLLHOOK scrollHook;
static ANIMDATA *masuMapAnim;
static ANIMDATA *pathAnim;
static MBMAPHOOK mapHook;

static HuVecF scrollPos;
static HuVecF mapViewPos;
static HuVecF mapViewRot;
static u8 mapPathBit[256];
static SCROLLWORK scrollWork;

static SCROLLWORK *scrollWorkP = &scrollWork;
static GXColor starCol = { 255, 255, 255, 255 };

static void ScrollCreate(unsigned int dataNum);
static void ScrollKill(void);
static int ScrollMain(int playerNo);
static int ScrollExec(int playerNo, s16 masuId);
static void RotateScrollView(HuVecF *rot, HuVecF *pos, HuVecF *posOut);
static BOOL CheckScrollCol(HuVecF *pos, HuVecF *dir, HuVecF *endPos);
static void InitScrollCol(void);
static void ResolveScrollCol(HuVecF *dir, HuVecF *pos, HuVecF *posOut, HuVecF *endPos);
static s16 StarMasuGet(int playerNo);
static void MapViewCreate(void);
static void MapViewKill(void);
static int MapViewExec(int playerNo);
static void MapSprCreate(int charNo, s16 masuId, u32 type);
static void MapBaseSprCreate(void);
static void MapSprPosCalc(MAPSPR *sprP);
static void MapSprPlayerPosCalc(int mode);
static BOOL MapSprPlayerCol(void);
static void MapSprPlayerColAll(void);
static void MapSprKill(void);
static void MapPathDraw(s16 masuId, Mtx mtx);
static void MapDraw(HU3D_MODEL *modelP, Mtx *mtx);

/* MSL's sqrtf with plain (not static) constants: retail pools 0.5 and 3.0 among this unit's own
   .sdata2 constants, where include/math.h's static-constant sqrtf would put them first. */
static inline float ScrollSqrtf(float x)
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

static inline BOOL ScrollTimeDayCheck(void)
{
    return GwSystem.curTime == 0;
}

void mbScrollInit(int dataNum)
{
    ScrollCreate(mbObjDataNumGet(dataNum));
    MapViewCreate();
}

void mbScrollClose(void)
{
    ScrollKill();
    MapViewKill();
}

void mbev_Scroll(int playerNo, BOOL mapF)
{
    int result;
    int stackNo;
    BOOL pauseDisable;

    pauseDisable = mbPauseDisableGet();
    mbPauseDisableSet(TRUE);
    mbStatusDispBackup();
    mbStatusMasuDispSet(FALSE);
    mbMusParamSet(0, 96, 500);
    stackNo = mbCameraStackPush();
    lbl_802C0DD8 = 0;
    while(1) {
        if(!mapF) {
            result = ScrollMain(playerNo);
            if(!result) {
                break;
            }
            mapF = TRUE;
        } else {
            result = MapViewExec(playerNo);
            if(!result) {
                break;
            }
            mapF = FALSE;
        }
        mbAudFXPlay(1);
    }
    mbAudFXPlay(3);
    mbMusParamSet(0, 127, 100);
    mbCameraStackIdxSet(stackNo, -1);
    mbStatusDispRestoreForce();
    mbStatusMasuDispSet(TRUE);
    mbWipeDissolveFadeIn();
    mbPauseDisableSet(pauseDisable);
}

static void ScrollCreate(unsigned int dataNum)
{
    if(dataNum == 0) {
        scrollColModel = HU3D_MODELID_NONE;
        scrollColTriNum = 0;
        scrollColTriData = NULL;
    } else {
        scrollColModel = Hu3DModelCreate(HuDataSelHeapReadNum(dataNum, HU_MEMNUM_OVL, HEAP_MODEL));
        InitScrollCol();
        Hu3DModelAttrSet(scrollColModel, HU3D_ATTR_DISPOFF);
    }
    scrollStarFindFunc = StarMasuGet;
    scrollHook = NULL;
}

static inline void ScrollFree(void *ptr)
{
    HuMemDirectFree(ptr);
}

static void ScrollKill(void)
{
    if(scrollColModel >= 0) {
        Hu3DModelKill(scrollColModel);
        scrollColModel = HU3D_MODELID_NONE;
    }
    if(scrollColTriData) {
        ScrollFree(scrollColTriData);
        scrollColTriData = NULL;
    }
}

static int ScrollMain(int playerNo)
{
    s16 winId;
    int result;
    s16 masuId;

    if(!mbWipeSpecialStatGet()) {
        mbWipeDissolveFadeOut();
        mbStatusDispForceSetAll(FALSE);
    }
    if(scrollHook) {
        scrollHook(TRUE);
    }
    if(scrollStarFindFunc) {
        masuId = scrollStarFindFunc(playerNo);
    } else {
        masuId = 0;
    }
    if(GWPartyGet() != FALSE) {
        if(masuId > 0) {
            winId = mbWinCreateHelp(0x26000B);
        } else {
            winId = mbWinCreateHelp(0x260005);
        }
    } else {
        winId = mbWinCreateHelp(0x26000F);
    }
    result = ScrollExec(playerNo, masuId);
    if(!result) {
        mbWipeDissolveFadeOut();
    } else {
        mbWipeSpecialFadeInCreate(4, 1);
    }
    mbWinKill(winId);
    if(scrollHook) {
        scrollHook(FALSE);
    }
    return result;
}

static int ScrollExec(int playerNo, s16 masuId)
{
    HuVecF focus;
    HuVecF masuPos;
    HuVecF savePos;
    HuVecF diff;
    HuVecF rot;
    HuVecF dir;
    float stkX;
    float stkY;
    float mag;
    float speed;
    int result;
    u16 btn;
    int mode;
    s8 padNo;

    result = 0;
    padNo = GwPlayer[playerNo].padNo;
    mode = 0;
    rot.x = -45;
    rot.y = 0;
    rot.z = 0;
    mbPlayerPosGet(playerNo, &scrollPos);
    RotateScrollView(&rot, &scrollPos, &scrollPos);
    dir.x = HuSin(rot.y)*HuCos(rot.x);
    dir.y = -HuSin(rot.x);
    dir.z = HuCos(rot.y)*HuCos(rot.x);
    VECScale(&dir, &dir, 100);
    if(!CheckScrollCol(&scrollPos, &dir, &focus)) {
        ResolveScrollCol(&dir, &scrollPos, &scrollPos, &focus);
    }
    if(masuId > 0) {
        mbMasuPosGet(masuId, &masuPos);
        RotateScrollView(&rot, &masuPos, &masuPos);
    }
    mbCameraMovePos(&focus, &rot, NULL, 1500, -1, -1);
    mbCameraMoveWait();
    mbWipeDissolveFadeIn();
    while(1) {
        btn = HuPadBtnDown[padNo];
        if(btn & PAD_BUTTON_B) {
            break;
        }
        if((btn & PAD_BUTTON_Y) && GWPartyGet() != FALSE) {
            result = 1;
            break;
        }
        switch(mode) {
            case 0:
                speed = 30.000002f;
                if(HuPadBtn[padNo] & PAD_BUTTON_A) {
                    speed *= 2;
                }
                stkX = mbPadStkXGet(padNo);
                stkY = -(float)mbPadStkYGet(padNo);
                mag = ScrollSqrtf(HuMag2Point2D(stkX, stkY));
                if(mag > 0) {
                    stkX /= mag;
                    stkY /= mag;
                    scrollPos.x += stkX*speed;
                    scrollPos.z += stkY*speed;
                }
                if((HuPadBtn[padNo] & PAD_TRIGGER_R) && masuId > 0) {
                    mode = 1;
                    savePos = scrollPos;
                }
                break;

            case 1:
                if(HuPadBtn[padNo] & PAD_TRIGGER_R) {
                    VECSubtract(&masuPos, &scrollPos, &diff);
                    if(VECMag(&diff) < 50) {
                        focus = masuPos;
                    } else {
                        VECNormalize(&diff, &diff);
                        scrollPos.x += 50*diff.x;
                        scrollPos.z += 50*diff.z;
                    }
                } else {
                    VECSubtract(&savePos, &scrollPos, &diff);
                    if(VECMag(&diff) < 200) {
                        scrollPos = savePos;
                        mode = 0;
                    } else {
                        VECNormalize(&diff, &diff);
                        scrollPos.x += 4.0f*(50*diff.x);
                        scrollPos.z += 4.0f*(50*diff.z);
                    }
                }
                break;
        }
        if(!CheckScrollCol(&scrollPos, &dir, &focus)) {
            ResolveScrollCol(&dir, &scrollPos, &scrollPos, &focus);
        }
        mbCameraFocusPosSet(&focus);
        HuPrcVSleep();
    }
    return result;
}

static void RotateScrollView(HuVecF *rot, HuVecF *pos, HuVecF *posOut)
{
    posOut->x = pos->x+(HuSin(rot->y)*(pos->y/(HuSin(rot->x)/HuCos(rot->x))));
    posOut->z = pos->z+(HuCos(rot->y)*(pos->y/(HuSin(rot->x)/HuCos(rot->x))));
    posOut->y = 0;
}

static BOOL CheckScrollCol(HuVecF *pos, HuVecF *dir, HuVecF *endPos)
{
    float maxArea;
    float area;
    float triArea;

    HuVecF cross;
    HuVecF *vtxP[4];
    HuVecF edge;
    HuVecF up;
    HuVecF out;

    HSF_FACE *faceP;
    HSF_BUFFER *normBufP;
    HSF_BUFFER *vtxBufP;
    HSF_OBJECT *objP;
    HSF_BUFFER *faceBufP;
    int i;
    HU3D_MODEL *modelP;

    maxArea = -1;
    if(scrollColModel < 0) {
        return FALSE;
    }
    modelP = &Hu3DData[scrollColModel];
    objP = modelP->hsf->root;
    if(objP->type != HSF_OBJ_MESH) {
        return FALSE;
    }
    faceBufP = objP->mesh.face;
    vtxBufP = objP->mesh.vertex;
    normBufP = objP->mesh.normal;
    for(faceP = faceBufP->data, i=0; i<faceBufP->count; i++, faceP++) {
        if(faceP->type == HSF_FACE_TRI) {
            vtxP[0] = ((HuVecF *)vtxBufP->data)+faceP->index[0].vertex;
            triArea = (faceP->nbt[0]*vtxP[0]->x)+(faceP->nbt[1]*vtxP[0]->y)+(faceP->nbt[2]*vtxP[0]->z);
            area = ((triArea-(faceP->nbt[0]*pos->x))-(faceP->nbt[1]*pos->y)-(faceP->nbt[2]*pos->z))/((faceP->nbt[0]*dir->x)+(faceP->nbt[1]*dir->y)+(faceP->nbt[2]*dir->z));
            if(area < 0) {
                continue;
            }
            if(maxArea >= 0 && area >= maxArea) {
                continue;
            }
            out.x = pos->x+(area*dir->x);
            out.y = pos->y+(area*dir->y);
            out.z = pos->z+(area*dir->z);
            if(faceP->type == HSF_FACE_TRI) {
                vtxP[1] = ((HuVecF *)vtxBufP->data)+faceP->index[1].vertex;
                vtxP[2] = ((HuVecF *)vtxBufP->data)+faceP->index[2].vertex;
                VECSubtract(vtxP[1], vtxP[0], &edge);
                VECSubtract(&out, vtxP[1], &up);
                VECCrossProduct(&edge, &up, &cross);
                if(VECDotProduct(&cross, (HuVecF *)faceP->nbt) < 0) {
                    continue;
                }
                VECSubtract(vtxP[2], vtxP[1], &edge);
                VECSubtract(&out, vtxP[2], &up);
                VECCrossProduct(&edge, &up, &cross);
                if(VECDotProduct(&cross, (HuVecF *)faceP->nbt) < 0) {
                    continue;
                }
                VECSubtract(vtxP[0], vtxP[2], &edge);
                VECSubtract(&out, vtxP[0], &up);
                VECCrossProduct(&edge, &up, &cross);
                if(VECDotProduct(&cross, (HuVecF *)faceP->nbt) < 0) {
                    continue;
                }
            }
            maxArea = area;
        }
    }
    if(maxArea >= 0) {
        endPos->x = pos->x+(maxArea*dir->x);
        endPos->y = pos->y+(maxArea*dir->y);
        endPos->z = pos->z+(maxArea*dir->z);
        return TRUE;
    } else {
        return FALSE;
    }
}

static void InitScrollCol(void)
{
    HSF_FACE *faceP;
    HSF_FACE *faceP2;
    HSF_BUFFER *faceBufP;
    HSF_BUFFER *vtxBufP;
    HSF_OBJECT *objP;
    HU3D_MODEL *modelP;
    int i;
    int j;
    int k;
    int m;
    int prev;
    int next;
    int edgeNum;

    if(scrollColModel < 0) {
        return;
    }
    modelP = &Hu3DData[scrollColModel];
    objP = modelP->hsf->root;
    if(objP->type != HSF_OBJ_MESH) {
        return;
    }
    faceBufP = objP->mesh.face;
    vtxBufP = objP->mesh.vertex;
    scrollColTriNum = 0;
    scrollColTriData = mbMalloc(faceBufP->count*sizeof(HSF_FACE));
    for(faceP = faceBufP->data, i=0; i<faceBufP->count; i++, faceP++) {
        for(j=0; j<3; j++) {
            edgeNum = 0;
            next = (j+1)%3;
            for(faceP2 = faceBufP->data, k=0; k<faceBufP->count; k++, faceP2++) {
                if(faceP == faceP2) {
                    continue;
                }
                for(m=0; m<3; m++) {
                    prev = m-1;
                    if(prev < 0) {
                        prev = 2;
                    }
                    if(faceP->index[j].vertex == faceP2->index[m].vertex && faceP->index[next].vertex == faceP2->index[prev].vertex) {
                        edgeNum++;
                        goto edgeFound;
                    }
                }
            }
            edgeFound:
            if(edgeNum == 0) {
                scrollColTriData[scrollColTriNum++] = *faceP;
                break;
            }
        }
    }
}

static void ResolveScrollCol(HuVecF *dir, HuVecF *pos, HuVecF *posOut, HuVecF *endPos)
{
    HSF_FACE *faceP;
    int i;
    HSF_BUFFER *vtxBufP;
    HSF_FACE *faceMinP;
    HSF_OBJECT *objP;
    int no;
    HU3D_MODEL *modelP;

    float scale;
    float mag;
    float xzMag;
    float minMag;
    float scaleY;

    HuVecF inVtx[3];
    HuVecF edge;
    HuVecF edge2;
    HuVecF outPos2;

    faceMinP = NULL;
    if(scrollColModel < 0) {
        return;
    }
    modelP = &Hu3DData[scrollColModel];
    objP = modelP->hsf->root;
    if(objP->type != HSF_OBJ_MESH) {
        return;
    }
    vtxBufP = objP->mesh.vertex;
    for(faceP = scrollColTriData, no=0; no<scrollColTriNum; no++, faceP++) {
        if(VECDotProduct(dir, (HuVecF *)faceP->nbt) < 0) {
            continue;
        }
        for(i=0; i<3; i++) {
            xzMag = ScrollSqrtf(HuMag2Point2D(dir->x, dir->z));
            scaleY = (((HuVecF *)vtxBufP->data)+faceP->index[i].vertex)->y/(dir->y/xzMag);
            inVtx[i].x = (((HuVecF *)vtxBufP->data)+faceP->index[i].vertex)->x-(scaleY*(dir->x/xzMag));
            inVtx[i].z = (((HuVecF *)vtxBufP->data)+faceP->index[i].vertex)->z-(scaleY*(dir->z/xzMag));
            inVtx[i].y = 0;
        }
        for(i=0; i<3; i++) {
            int nextVtx = (i+1)%3;
            VECSubtract(&inVtx[nextVtx], &inVtx[i], &edge);
            scale = ((pos->x*edge.x)-(edge.x*inVtx[i].x)+(pos->y*edge.y)-(edge.y*inVtx[i].y)+(pos->z*edge.z)-(edge.z*inVtx[i].z))/VECSquareMag(&edge);
            if(scale >= 0 && scale < 1) {
                edge2.x = inVtx[i].x+(scale*edge.x);
                edge2.y = inVtx[i].y+(scale*edge.y);
                edge2.z = inVtx[i].z+(scale*edge.z);
                VECSubtract(&edge2, pos, &edge);
                mag = VECMag(&edge);
                if(!faceMinP || mag < minMag) {
                    faceMinP = faceP;
                    minMag = mag;
                    outPos2 = edge2;
                }
            }
        }
        for(i=0; i<3; i++) {
            VECSubtract(&inVtx[i], pos, &edge);
            mag = VECMag(&edge);
            if(!faceMinP || mag < minMag) {
                faceMinP = faceP;
                minMag = mag;
                outPos2 = inVtx[i];
            }
        }
    }
    if(faceMinP) {
        HuVecF *vtxP;
        float dot;
        faceP = faceMinP;
        vtxP = ((HuVecF *)vtxBufP->data)+faceP->index[0].vertex;
        dot = (faceP->nbt[0]*vtxP->x)+(faceP->nbt[1]*vtxP->y)+(faceP->nbt[2]*vtxP->z);
        scale = ((dot-(faceP->nbt[0]*outPos2.x))-(faceP->nbt[1]*outPos2.y)-(faceP->nbt[2]*outPos2.z))/((faceP->nbt[0]*dir->x)+(faceP->nbt[1]*dir->y)+(faceP->nbt[2]*dir->z));
        if(posOut) {
            *posOut = outPos2;
        }
        if(endPos) {
            endPos->x = outPos2.x+(scale*dir->x);
            endPos->y = outPos2.y+(scale*dir->y);
            endPos->z = outPos2.z+(scale*dir->z);
        }
    }
}

static s16 StarMasuGet(int playerNo)
{
    return mbMasuFind_TypeIdGet(GwPlayer[playerNo].masuId, 7, 1, 1);
}

void mbev_StarScroll(HuVecF *startPos, HuVecF *endPos, s16 maxTime)
{
    HuVecF rot;
    HuVecF dir;
    HuVecF start;
    HuVecF end;
    HuVecF pos;
    HuVecF focus;
    float t;
    int i;

    mbCameraRotGet(&rot);
    RotateScrollView(&rot, startPos, &start);
    RotateScrollView(&rot, endPos, &end);
    dir.x = HuSin(rot.y)*HuCos(rot.x);
    dir.y = -HuSin(rot.x);
    dir.z = HuCos(rot.y)*HuCos(rot.x);
    VECScale(&dir, &dir, 100);
    if(!CheckScrollCol(&start, &dir, &focus)) {
        ResolveScrollCol(&dir, &start, NULL, &focus);
    }
    mbCameraMovePos(&focus, &rot, NULL, 1500, -1, 24);
    mbCameraMoveWait();
    if(maxTime < 0) {
        maxTime = 120;
    }
    for(i=0; i<=maxTime; i++) {
        t = (float)i/maxTime;
        pos.x = start.x+(t*(end.x-start.x));
        pos.y = start.y;
        pos.z = start.z+(t*(end.z-start.z));
        if(!CheckScrollCol(&pos, &dir, &focus)) {
            ResolveScrollCol(&dir, &pos, NULL, &focus);
        }
        mbCameraFocusPosSet(&focus);
        HuPrcVSleep();
    }
}

static void MapViewCreate(void)
{
    mapViewZoom = 20000;
    mapViewPos.x = mapViewPos.y = mapViewPos.z = 0;
    mapViewRot.x = -78;
    mapViewRot.y = 0;
    mapViewRot.z = 0;
    mapHook = NULL;
    masuMapAnim = HuSprAnimRead(HuDataSelHeapReadNum(0x40002, HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(masuMapAnim);
    pathAnim = HuSprAnimRead(HuDataSelHeapReadNum(0x40006, HU_MEMNUM_OVL, HEAP_MODEL));
    HuSprAnimLock(pathAnim);
    HuDataDirClose(0x40000);
}

static void MapViewKill(void)
{
    if(masuMapAnim != NULL) {
        HuSprAnimKill(masuMapAnim);
        masuMapAnim = NULL;
    }
    if(pathAnim != NULL) {
        HuSprAnimKill(pathAnim);
        pathAnim = NULL;
    }
}

static int MapViewExec(int playerNo)
{
    int dispTbl[4];
    float near;
    float far;
    int i;
    HU3D_MODELID modelId;
    u16 btn;
    int result;
    s16 winId;
    s16 padNo;

    if(!mbWipeSpecialStatGet()) {
        mbWipeSpecialFadeInCreate(4, 1);
        mbStatusDispForceSetAll(FALSE);
    }
    mbCameraNearFarGet(&near, &far);
    mbCameraNearFarSet(100, 30000);
    mbCameraMovePos(&mapViewPos, &mapViewRot, NULL, mapViewZoom, -1, -1);
    mbCameraMoveWait();
    memset(scrollWorkP, 0, sizeof(SCROLLWORK));
    scrollWorkP->mode = 0;
    MapBaseSprCreate();
    if(mapHook) {
        mapHook(TRUE);
    }
    MapSprPlayerPosCalc(scrollWorkP->mode);
    mbMasuModelDispSet(FALSE);
    modelId = Hu3DHookFuncCreate(MapDraw);
    Hu3DModelCameraSet(modelId, HU3D_CAM2);
    Hu3DModelLayerSet(modelId, 2);
    for(i=0; i<4; i++) {
        dispTbl[i] = mbPlayerDispGet(i);
        mbPlayerDispSet(i, FALSE);
    }
    mbEffFadeCreate(1, 128);
    scrollWorkP->masuTime = 0;
    winId = mbWinCreateHelp(0x260006);
    mbWinPosSet(winId, 120, 408);
    mbWipeSpecialFadeOutCreate(4, 30);
    padNo = GwPlayer[playerNo].padNo;
    while(1) {
        btn = HuPadBtnDown[padNo];
        if(btn & PAD_BUTTON_A) {
            scrollWorkP->mode = (scrollWorkP->mode+1)%3;
            if(GWPartyGet() == FALSE && scrollWorkP->mode == 1) {
                scrollWorkP->mode = (scrollWorkP->mode+1)%3;
            }
            MapSprPlayerPosCalc(scrollWorkP->mode);
        }
        if(btn & PAD_BUTTON_B) {
            result = 0;
            break;
        } else if(btn & PAD_BUTTON_X) {
            result = 1;
            break;
        }
        HuPrcVSleep();
    }
    mbWipeDissolveFadeOut();
    mbWinKill(winId);
    mbMasuModelDispSet(TRUE);
    if(modelId >= 0) {
        Hu3DModelKill(modelId);
    }
    MapSprKill();
    for(i=0; i<4; i++) {
        mbPlayerDispSet(i, dispTbl[i]);
    }
    mbEffFadeOutSet(1);
    mbCameraNearFarSet(near, far);
    if(mapHook) {
        mapHook(FALSE);
    }
    return result;
}

static const int mapCharFileTbl[16] = {
    0x50036, 0x50037, 0x50038, 0x50039,
    0x5003A, 0x5003B, 0x5003C, 0x5003D,
    0x5003E, 0x5003F, 0x50040, 0x50041,
    0x50042, 0x50043, 0x500A9, 0x500A8
};

static GXColor charColorTbl[16] = {
    { 0xE3, 0x43, 0x43, 0xFF },
    { 0x44, 0x43, 0xE3, 0xFF },
    { 0xF1, 0x9E, 0xDC, 0xFF },
    { 0x43, 0xE4, 0x44, 0xFF },
    { 0x8A, 0x3C, 0xB4, 0xFF },
    { 0xE3, 0xE4, 0x44, 0xFF },
    { 0xC0, 0xC0, 0xC0, 0xFF },
    { 0xE3, 0xE3, 0xE3, 0xFF },
    { 0x28, 0xE3, 0xE3, 0xFF },
    { 0xE3, 0x8B, 0x28, 0xFF },
    { 0xB4, 0x28, 0x28, 0xFF },
    { 0xB4, 0x28, 0x28, 0xFF },
    { 0x28, 0xB4, 0x28, 0xFF },
    { 0x28, 0x28, 0xB4, 0xFF },
    { 0x28, 0xB4, 0x28, 0xFF },
    { 0x28, 0x28, 0xB4, 0xFF }
};

static inline s16 MapEspCreate(int dataNum, s16 prio)
{
    s16 espId = espEntry(mbBoardDataNumGet(dataNum), prio, 0);
    espDrawNoSet(espId, 64);
    return espId;
}

static void MapSprCreate(int charNo, s16 masuId, u32 type)
{
    MAPSPR *sprP;
    int i;
    int j;

    for(sprP = scrollWorkP->spr, i=0; i<MAPSPR_MAX; i++, sprP++) {
        if(!sprP->usedF) {
            break;
        }
    }
    if(i >= MAPSPR_MAX) {
        return;
    }
    if(charNo >= 0) {
        sprP->color = charColorTbl[charNo];
    } else {
        sprP->color.r = sprP->color.g = sprP->color.b = 0;
        sprP->color.a = 255;
    }
    sprP->usedF = TRUE;
    sprP->dispF = TRUE;
    sprP->charNo = charNo;
    sprP->type = type;
    sprP->masuId = masuId;
    sprP->sprIdBack = -1;
    if(sprP->charNo >= 0 && sprP->charNo <= 15) {
        sprP->sprId = MapEspCreate(mapCharFileTbl[sprP->charNo], 2000);
        sprP->sprIdSub[0] = MapEspCreate(0x500AF, 2300);
        espColorSet(sprP->sprIdSub[0], sprP->color.r, sprP->color.g, sprP->color.b);
    } else {
        switch(charNo) {
            case -1:
                sprP->sprId = MapEspCreate(0x500A6, 2500);
                sprP->sprIdBack = MapEspCreate(0x500A6, 2600);
                espScaleSet(sprP->sprId, 0.6f, 0.6f);
                espScaleSet(sprP->sprIdBack, 0.6f, 0.6f);
                espTPLvlSet(sprP->sprIdBack, 0.5f);
                break;

            case -2:
                sprP->sprId = MapEspCreate(0x500A7, 2500);
                sprP->sprIdBack = MapEspCreate(0x500A7, 2600);
                espScaleSet(sprP->sprId, 0.6f, 0.6f);
                espScaleSet(sprP->sprIdBack, 0.6f, 0.6f);
                espTPLvlSet(sprP->sprIdBack, 0.5f);
                break;

            case 19:
                sprP->sprId = MapEspCreate(0x500AD, 2700);
                espScaleSet(sprP->sprId, 0.5f, 0.5f);
                break;

            case 16:
                sprP->sprId = MapEspCreate(0x500AA, 2700);
                espScaleSet(sprP->sprId, 0.5f, 0.5f);
                break;

            case 17:
                sprP->sprId = MapEspCreate(0x500AB, 2700);
                espScaleSet(sprP->sprId, 0.5f, 0.5f);
                break;

            case 18:
                sprP->sprId = MapEspCreate(0x500AC, 2700);
                espScaleSet(sprP->sprId, 0.5f, 0.5f);
                break;

            case 20:
                sprP->sprId = MapEspCreate(0x500AE, 2700);
                espScaleSet(sprP->sprId, 0.5f, 0.5f);
                break;
        }
        for(j=0; j<MAPSPR_SUB_MAX; j++) {
            sprP->sprIdSub[j] = -1;
        }
    }
    MapSprPosCalc(sprP);
    sprP->dispPos = sprP->pos;
    scrollWorkP->sprNum++;
}

static void MapBaseSprCreate(void)
{
    s16 masuTbl[12];
    int i;
    int num;
    int playerNo;

    if(GWPartyGet() != FALSE) {
        for(i=0; i<4; i++) {
            if(i == GwSystem.turnPlayerNo) {
                MapSprCreate(GwPlayer[i].charNo, GwPlayer[i].masuId, 5);
            } else {
                MapSprCreate(GwPlayer[i].charNo, GwPlayer[i].masuId, 4);
            }
        }
    } else {
        playerNo = GwSystem.turnPlayerNo;
        MapSprCreate(GwPlayer[playerNo].charNo, GwPlayer[playerNo].masuId, 1);
    }
    num = mbMasuTypeListGet(7, masuTbl);
    for(i=0; i<num; i++) {
        MapSprCreate(-1, masuTbl[i], 0);
    }
    num = mbMasuTypeListGet(10, masuTbl);
    for(i=0; i<num; i++) {
        MapSprCreate(-2, masuTbl[i], 0);
    }
}

static void MapSprPosCalc(MAPSPR *sprP)
{
    HuVecF pos;
    mbMasuPosGet(sprP->masuId, &pos);
    Hu3D3Dto2D(&pos, HU3D_CAM0, &sprP->pos);
    if(sprP->charNo >= 0 && sprP->charNo <= 15) {
        sprP->dispPos.x = sprP->pos.x;
        sprP->dispPos.y = (sprP->pos.y > 240) ? (sprP->pos.y-64) : (64+sprP->pos.y);
        sprP->dispPos.z = 0;
    } else {
        sprP->dispPos = sprP->pos;
    }
}

static inline void MapSprDispOn(MAPSPR *sprP)
{
    int i;
    sprP->dispF = TRUE;
    if(sprP->sprId >= 0) {
        espDispOn(sprP->sprId);
    }
    if(sprP->sprIdBack >= 0) {
        espDispOn(sprP->sprIdBack);
    }
    for(i=0; i<MAPSPR_SUB_MAX; i++) {
        if(sprP->sprIdSub[i] >= 0) {
            espDispOn(sprP->sprIdSub[i]);
        }
    }
}

static inline void MapSprDispOff(MAPSPR *sprP)
{
    int i;
    sprP->dispF = FALSE;
    if(sprP->sprId >= 0) {
        espDispOff(sprP->sprId);
    }
    if(sprP->sprIdBack >= 0) {
        espDispOff(sprP->sprIdBack);
    }
    for(i=0; i<MAPSPR_SUB_MAX; i++) {
        if(sprP->sprIdSub[i] >= 0) {
            espDispOff(sprP->sprIdSub[i]);
        }
    }
}

static void MapSprPlayerPosCalc(int mode)
{
    MAPSPR *sprP;
    int i;
    HuVecF dir;
    HuVecF mid;
    float mag;
    float scaleY;
    float rotZ;

    for(sprP = scrollWorkP->spr, i=0; i<scrollWorkP->sprNum; i++, sprP++) {
        if(sprP->type != 0) {
            MapSprDispOff(sprP);
            MapSprPosCalc(sprP);
        }
    }
    switch(scrollWorkP->mode) {
        case 0:
            for(sprP = scrollWorkP->spr, i=0; i<scrollWorkP->sprNum; i++, sprP++) {
                if(sprP->type & 1) {
                    MapSprDispOn(sprP);
                }
            }
            break;

        case 1:
            for(sprP = scrollWorkP->spr, i=0; i<scrollWorkP->sprNum; i++, sprP++) {
                if(sprP->type & 4) {
                    MapSprDispOn(sprP);
                }
            }
            break;
    }
    MapSprPlayerColAll();
    for(sprP = scrollWorkP->spr, i=0; i<scrollWorkP->sprNum; i++, sprP++) {
        espPosSet(sprP->sprId, sprP->dispPos.x, sprP->dispPos.y);
        if(sprP->sprIdBack >= 0) {
            espPosSet(sprP->sprIdBack, sprP->dispPos.x, sprP->dispPos.y);
        }
        if(sprP->charNo >= 0 && sprP->charNo <= 15) {
            dir.x = sprP->dispPos.x-sprP->pos.x;
            dir.y = sprP->dispPos.y-sprP->pos.y;
            dir.z = 0;
            mag = VECMag(&dir);
            mid.x = sprP->pos.x+(0.5f*dir.x);
            mid.y = sprP->pos.y+(0.5f*dir.y);
            espPosSet(sprP->sprIdSub[0], mid.x, mid.y);
            rotZ = HuAtan(dir.x, -dir.y);
            espZRotSet(sprP->sprIdSub[0], rotZ);
            scaleY = mag/64.0f;
            espScaleSet(sprP->sprIdSub[0], 1.0f, scaleY);
        }
    }
}

static BOOL MapSprPlayerCol(void)
{
    BOOL hitF;
    MAPSPR *sprP;
    MAPSPR *sprP2;
    int i;
    int j;
    HuVecF dir;
    float ofs;

    hitF = FALSE;
    for(sprP = scrollWorkP->spr, i=0; i<scrollWorkP->sprNum; i++, sprP++) {
        sprP->ofs.x = sprP->ofs.y = sprP->ofs.z = 0;
    }
    for(i=0; i<scrollWorkP->sprNum-1; i++) {
        sprP = &scrollWorkP->spr[i];
        if(sprP->charNo < 0 || sprP->charNo > 15 || !sprP->dispF) {
            continue;
        }
        for(j=i+1; j<scrollWorkP->sprNum; j++) {
            sprP2 = &scrollWorkP->spr[j];
            if(sprP2->charNo < 0 || sprP2->charNo > 15 || !sprP2->dispF) {
                continue;
            }
            VECSubtract(&sprP2->dispPos, &sprP->dispPos, &dir);
            if(fabs(dir.x) < 64.0 && fabs(dir.y) < 64.0) {
                ofs = 0.5*(64.0-fabs(dir.x));
                if(dir.x < 0) {
                    ofs = -ofs;
                }
                sprP->ofs.x += -ofs;
                sprP2->ofs.x += ofs;
                hitF = TRUE;
            }
        }
    }
    for(sprP = scrollWorkP->spr, i=0; i<scrollWorkP->sprNum; i++, sprP++) {
        if(sprP->charNo >= 0 && sprP->charNo <= 15) {
            sprP->dispPos.x += sprP->ofs.x;
            if(sprP->dispPos.x < 48) {
                sprP->dispPos.x = 48;
            } else if(sprP->dispPos.x > 528) {
                sprP->dispPos.x = 528;
            }
        }
    }
    return hitF;
}

static void MapSprPlayerColAll(void)
{
    int i;
    for(i=0; i<50; i++) {
        if(!MapSprPlayerCol()) {
            break;
        }
    }
}

static void MapSprKill(void)
{
    MAPSPR *sprP;
    int i;
    int j;
    for(sprP = scrollWorkP->spr, i=0; i<scrollWorkP->sprNum; i++, sprP++) {
        if(sprP->sprId >= 0) {
            espKill(sprP->sprId);
        }
        if(sprP->sprIdBack >= 0) {
            espKill(sprP->sprIdBack);
        }
        for(j=0; j<MAPSPR_SUB_MAX; j++) {
            if(sprP->sprIdSub[j] >= 0) {
                espKill(sprP->sprIdSub[j]);
            }
        }
    }
}

/* Only MapDraw calls this, inlined; the linker strips the unreferenced copy. Its definition here
   is what places 24.0f, 0.2f and 90.0f before MapPathDraw's constants in .sdata2. */
static void MapMasuSprScaleSet(void)
{
    MAPSPR *sprP;
    int i;
    float time;
    float scale;

    time = (scrollWorkP->masuTime++)/24.0f;
    if(scrollWorkP->masuTime > 24u) {
        scrollWorkP->masuTime = 0;
    }
    scale = 0.6f+(0.2f*mbSinDeg(90.0f*time));
    for(sprP = scrollWorkP->spr, i=0; i<scrollWorkP->sprNum; i++, sprP++) {
        if(sprP->charNo < 0) {
            espScaleSet(sprP->sprIdBack, scale, scale);
        }
    }
}

static inline void MapCameraProject(Mtx mtx, HuVecF *in, HuVecF *out)
{
    HU3D_CAMERA *cameraP = &Hu3DCamera[0];
    HuVecF posCamera;
    float x, y;
    MTXMultVec(mtx, in, &posCamera);
    x = posCamera.z*(HuSin(cameraP->fov/2)/HuCos(cameraP->fov/2))*1.2f;
    y = posCamera.z*(HuSin(cameraP->fov/2)/HuCos(cameraP->fov/2));
    out->x = 288+(posCamera.x*(288/(-x)));
    out->y = 240+(posCamera.y*(240/y));
    out->z = 0;
}

static void MapPathDraw(s16 masuId, Mtx mtx)
{
    Mtx modelview;
    Mtx trans;
    HuVecF pos;
    HuVecF posEnd;
    HuVecF masuPos;
    HuVecF pathDir;
    int i;
    int linkNum;
    s16 linkId;
    u32 attr;
    u32 mattr;
    float pathLen;
    float pathYScale;

    mapPathBit[masuId] = TRUE;
    mbMasuPosGet(masuId, &masuPos);
    MapCameraProject(mtx, &masuPos, &pos);
    MTXTrans(trans, pos.x, pos.y, 0);
    linkNum = mbMasuLinkNumGet(masuId);
    for(i=0; i<linkNum; i++) {
        linkId = mbMasuLinkGet(masuId, i);
        attr = mbMasuAttrGet(linkId);
        mattr = mbMasuMAttrGet(linkId);
        if(attr & mbBranchAttrGet()) {
            continue;
        }
        if(mattr & mbBranchMAttrGet()) {
            continue;
        }
        mbMasuPosGet(linkId, &masuPos);
        MapCameraProject(mtx, &masuPos, &posEnd);
        VECSubtract(&posEnd, &pos, &pathDir);
        pathDir.z = 0;
        pathLen = VECMag(&pathDir);
        pathYScale = pathLen/16;
        masuPos.x = pos.x+(0.5f*pathDir.x);
        masuPos.y = pos.y+(0.5f*pathDir.y);
        mtxRot(modelview, 0, 0, HuAtan(pathDir.x, -pathDir.y));
        mtxScaleCat(modelview, 1, pathYScale, 0);
        mtxTransCat(modelview, masuPos.x, masuPos.y, 0);
        GXLoadPosMtxImm(modelview, GX_PNMTX0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition2f32(-4, -8);
        GXTexCoord2f32(0, scrollWorkP->pathScroll);
        GXPosition2f32(4, -8);
        GXTexCoord2f32(1, scrollWorkP->pathScroll);
        GXPosition2f32(4, 8);
        GXTexCoord2f32(1, scrollWorkP->pathScroll+pathYScale);
        GXPosition2f32(-4, 8);
        GXTexCoord2f32(0, scrollWorkP->pathScroll+pathYScale);
        GXEnd();
        if(!mapPathBit[linkId]) {
            MapPathDraw(linkId, mtx);
        }
    }
}

static s16 masuPatTbl[11] = {
    -1, 0, 1, 2, 6, 7, -2, 5, 8, -1, 9
};

static void MapDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    Mtx uvMtx;
    Mtx trans;
    Mtx lookAt;
    Mtx44 proj;
    HuVecF pos;
    int i;
    int masuNum;
    s16 startId;
    int type;
    u32 attr;
    u32 mattr;
    int patNo;
    float uvX;
    float uvY;

    mbCameraLookAtGet(lookAt);
    MTXOrtho(proj, 0, 480, 0, 576, 0, 100);
    GXSetProjection(proj, GX_ORTHOGRAPHIC);
    HuSprTexLoad(masuMapAnim, 0, GX_TEXMAP0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    HuSprTexLoad(pathAnim, 0, GX_TEXMAP1, GX_CLAMP, GX_REPEAT, GX_LINEAR);
    GXSetTevColor(GX_TEVREG0, starCol);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
    GXSetNumTevStages(1);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetChanAmbColor(GX_COLOR0A0, starCol);
    GXSetChanMatColor(GX_COLOR0A0, starCol);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    scrollWorkP->pathScroll = (scrollWorkP->pathTime++)/30.0f;
    if(scrollWorkP->pathTime > 30u) {
        scrollWorkP->pathTime = 0;
    }
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR0A0);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    startId = mbMasuFind_AttrIdGet(-1, 0x8000);
    memset(mapPathBit, 0, sizeof(mapPathBit));
    MapPathDraw(startId, lookAt);
    if(!mbMasuDispGet()) {
        return;
    }
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
    masuNum = mbMasuNumGet();
    for(i=1; i<masuNum; i++) {
        type = mbMasuTypeGet(i);
        attr = mbMasuAttrGet(i);
        mattr = mbMasuMAttrGet(i);
        if(type == 0) {
            continue;
        }
        if((attr & ~0x10) & mbMasuDispAttrGet()) {
            continue;
        }
        if(mattr & mbMasuDispMAttrGet()) {
            continue;
        }
        patNo = masuPatTbl[type];
        if(patNo < 0) {
            if(patNo == -1) {
                continue;
            }
            patNo = (ScrollTimeDayCheck()) ? 4 : 3;
        }
        uvX = (patNo%4)/4.0f;
        uvY = (patNo/4)/3.0f;
        MTXScale(uvMtx, 0.25f, 0.33333334f, 0);
        mtxTransCat(uvMtx, uvX, uvY, 0);
        GXLoadTexMtxImm(uvMtx, GX_TEXMTX0, GX_MTX2x4);
        mbMasuPosGet(i, &pos);
        MapCameraProject(lookAt, &pos, &pos);
        MTXTrans(trans, pos.x, pos.y, 0);
        GXLoadPosMtxImm(trans, GX_PNMTX0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition2f32(-8, -8);
        GXTexCoord2f32(0, 0);
        GXPosition2f32(8, -8);
        GXTexCoord2f32(1, 0);
        GXPosition2f32(8, 8);
        GXTexCoord2f32(1, 1);
        GXPosition2f32(-8, 8);
        GXTexCoord2f32(0, 1);
        GXEnd();
    }
    MapMasuSprScaleSet();
}

void mbScrollStarFindFuncSet(MBSCROLLSTARFINDFUNC func)
{
    scrollStarFindFunc = func;
}

void mbMapCameraSet(HuVecF *rot, HuVecF *pos, float zoom)
{
    if(rot) {
        mapViewRot = *rot;
    }
    if(pos) {
        mapViewPos = *pos;
    }
    if(zoom >= 0) {
        mapViewZoom = zoom;
    }
}

void mbMapHookSet(MBMAPHOOK hook)
{
    mapHook = hook;
}

void mbScrollHookSet(MBSCROLLHOOK hook)
{
    scrollHook = hook;
}

void mbMapSprAdd(int charNo, s16 masuId)
{
    if(charNo >= 16) {
        MapSprCreate(charNo, masuId, 0);
    } else {
        MapSprCreate(charNo, masuId, 5);
    }
}

void mbev_ScrollCapsule(int playerNo)
{
    BOOL pauseDisable;
    int stackNo;
    int result;
    BOOL mapF;

    mapF = FALSE;
    pauseDisable = mbPauseDisableGet();
    mbPauseDisableSet(TRUE);
    mbStatusDispBackup();
    mbStatusMasuDispSet(FALSE);
    mbMusParamSet(0, 96, 500);
    stackNo = mbCameraStackPush();
    lbl_802C0DD8 = 0;
    while(1) {
        if(!mapF) {
            result = ScrollMain(playerNo);
            if(!result) {
                break;
            }
            mapF = TRUE;
        } else {
            result = MapViewExec(playerNo);
            if(!result) {
                break;
            }
            mapF = FALSE;
        }
        mbAudFXPlay(1);
    }
    mbAudFXPlay(3);
    mbMusParamSet(0, 127, 100);
    mbCameraStackIdxSet(stackNo, -1);
    mbStatusDispRestoreForce();
    mbStatusMasuDispSet(TRUE);
    mbWipeDissolveFadeIn();
    mbPauseDisableSet(pauseDisable);
}
