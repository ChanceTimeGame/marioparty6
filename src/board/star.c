#include "game/board/star.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/coin.h"
#include "game/board/comchoice.h"
#include "game/board/exit.h"
#include "game/board/guide.h"
#include "game/board/math.h"
#include "game/board/object.h"
#include "game/board/opening.h"
#include "game/board/pause.h"
#include "game/board/player.h"
#include "game/board/scroll.h"
#include "game/board/snpc.h"
#include "game/board/status.h"
#include "game/board/telop.h"
#include "game/board/window.h"
#include "game/board/wipe.h"

#include "game/data.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/memory.h"
#include "game/sprite.h"

/* masu.h's layer arguments are gone in MP6 (see the masu asm); declared here as retail calls them. */
int mbMasuTypeGet(s16 id);
void mbMasuTypeSet(s16 id, u16 type);
void mbMasuCapsuleSet(s16 id, s16 capsuleNo);
void mbMasuPosGet(s16 id, HuVecF *pos);
int mbMasuTypeListGet(s16 type, s16 *list);
s16 mbMasuFind_AttrIdGet(s16 id, u16 attr);

/* effect.h declares the hook setter with an int model id; retail passes the s16 unextended. */
s16 mbParticleCreate(void *anim, int num);
void mbParticleKill(s16 mdlId);
void mbParticleHookSet(s16 mdlId, void *hook);

#define GWTeamGet() ((int)GwSystem.tagF)

#define STAROBJ_MAX 999

#define STAR_MODE_GROW 0
#define STAR_MODE_IDLE 1
#define STAR_MODE_SHRINKIDLE 2
#define STAR_MODE_SHRINK 3
#define STAR_MODE_SHRINKIDLE_END 4

typedef struct StarWork_s {
    u8 killF : 1;
    u8 rotF : 1;
    u8 dispF : 1;
    u8 masuF : 1;
    u8 effDispF : 1;
    u8 effF : 1;
    int objNo;
    int no;
    int masuId;
    int playerNo;
    s8 mode;
    s8 rotSpeed;
    s16 angle;
    s16 effMdlId;
    float baseY;
    s16 time;
    u8 unk22[6];
    int chestMdlId;
    OMOBJ *obj;
    HuVecF pos;
    HuVecF ofs;
    HuVecF rot;
    HuVecF scale;
} STARWORK;

typedef struct StarParticleData_s {
    s16 time;
    s16 maxTime;
    u8 unk4[4];
    HuVecF vel;
    u8 unk14[0x2C - 0x14];
    float scale;
    u8 unk30[0x3C - 0x30];
    HuVecF pos;
    GXColor color;
    u8 unk4C[0x5C - 0x4C];
} STARPARTICLEDATA;

typedef struct StarParticle_s {
    s16 initF;
    u8 unk2[0x1C - 0x2];
    STARWORK *work;
    u8 unk20;
    u8 unk21[0x26 - 0x21];
    s16 num;
    u8 unk28[0x68 - 0x28];
    STARPARTICLEDATA *data;
} STARPARTICLE;

typedef struct StarDispWork_s {
    int mdlId[4];
    int playerNo;
    int num;
    int mode;
    int unk1C;
    u16 time;
    u16 delay;
    HuVecF pos;
    HuVecF mdlPos[4];
} STARDISPWORK;

static const HuVecF starViewOfs = { 0, 100, 0 };

static HuVecF starGuidePos = { -0.73f, -0.77f, -750.0f };
static int numberFileTbl[10] = {
    0x5000B, 0x5000C, 0x5000D, 0x5000E, 0x5000F,
    0x50010, 0x50011, 0x50012, 0x50013, 0x50014
};
static HuVecF starMasuGuidePos = { -0.73f, -0.77f, -750.0f };

static OMOBJ *starOMObj[STAROBJ_MAX];
static OMOBJ *starDispObj[4];
static OMOBJ *ztarOMObj[STAROBJ_MAX];

static int starAddNum = 1;
static s8 starGuideMotTbl[] = { 3, 12, 11, 23, -1 };
static s8 starMasuGuideMotTbl[] = { 3, 12, 11, 23, -1 };

static HUPROCESS *starMasuProc;
static HUPROCESS *starFreeProc;
static int starNum;
static int starMasuPrevType;
static int starResetF;
static int starMin;
static int starMasuNext;
static int ztarMasuNext;
static OMOBJ *starGuideObj;
static ANIMDATA *starEffAnim1;
static ANIMDATA *starEffAnim2;
static void (*starMoveHook)(void);
static void (*starMasuFunc)(void);

static int StarObjCreate(HuVecF *pos);
static void ev_StarMasu(void);
static void ev_StarFreeMasu(void);
static void ev_StarMasuKill(void);
static void ev_StarFreeMasuKill(void);
static void StarObjOMExec(OMOBJ *obj);
static void StarObjKill(OMOBJ *obj);
static void StarObjRotate(STARWORK *work, OMOBJ *obj);
static void StarObjShrinkIdleSet(OMOBJ *obj);
static void StarObjGrowSet(OMOBJ *obj);
static void StarObjShrinkSet(OMOBJ *obj);
static void StarObjGrowWait(OMOBJ *obj);
static void StarObjShrinkIdleWait(OMOBJ *obj);
static s16 StarObjEffCreate(ANIMDATA *anim);
static void StarObjEffKill(s16 mdlId);
static void StarObjEffHook(HU3D_MODEL *modelP, STARPARTICLE *particle, Mtx mtx);
static void StarDispUpdate(OMOBJ *obj);
static void StarDispObjUpdate(STARDISPWORK *work, int no, float posX, float posY, float posZ, float scale);
static void ev_StarMasuRun(BOOL freeF);
static void StarPauseHook(BOOL endF);
static void StarMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *material);
static void ZtarObjOMExec(OMOBJ *obj);
static void ZtarObjKill(OMOBJ *obj);
static void ZtarObjRotate(STARWORK *work, OMOBJ *obj);
static void ZtarObjShrinkIdleSet(OMOBJ *obj);
static void ZtarObjGrowSet(OMOBJ *obj);
static void ZtarObjShrinkSet(OMOBJ *obj);
static void ZtarObjGrowWait(OMOBJ *obj);
static void ZtarObjShrinkIdleWait(OMOBJ *obj);

static inline void StarModelInit(int modelId)
{
    HSF_MATERIAL *material;
    HSF_DATA *hsf;
    HU3D_MODEL *modelP;
    int i;
    modelP = &Hu3DData[modelId];
    hsf = modelP->hsf;
    material = hsf->material;
    modelP->hiliteIdx = 0;
    Hu3DModelMatHookSet(modelId, StarMatHook);
    for(i = 0; i < hsf->materialNum; i++, material++) {
        material->flags |= 0x2000;
    }
}

static inline STARPARTICLE *StarParticleGet(s16 mdlId)
{
    return Hu3DData[mdlId].hookData;
}


static inline int StarObjNoMasuGet(int masuId)
{
    int i;
    for(i = 0; i < STAROBJ_MAX; i++) {
        if(starOMObj[i]) {
            STARWORK *work = starOMObj[i]->data;
            if(work->masuId == masuId) {
                return i;
            }
        }
    }
    return -1;
}

static inline OMOBJ *StarObjMasuGet(int masuId)
{
    int no = StarObjNoMasuGet(masuId);
    if(no >= 0) {
        return starOMObj[no];
    } else {
        return NULL;
    }
}

static inline int StarObjNoPlayerGet(int playerNo)
{
    int i;
    for(i = 0; i < STAROBJ_MAX; i++) {
        if(starOMObj[i]) {
            STARWORK *work = starOMObj[i]->data;
            if(work->playerNo == playerNo) {
                return i;
            }
        }
    }
    return -1;
}

static inline OMOBJ *StarObjPlayerGet(int playerNo)
{
    int no = StarObjNoPlayerGet(playerNo);
    if(no >= 0) {
        return starOMObj[no];
    } else {
        return NULL;
    }
}

static inline void StarObjMasuKill(int masuId)
{
    int i;
    for(i = 0; i < STAROBJ_MAX; i++) {
        if(starOMObj[i]) {
            STARWORK *work = starOMObj[i]->data;
            if(masuId == work->masuId) {
                StarObjKill(starOMObj[i]);
                return;
            }
        }
    }
}

static inline void StarObjPlayerKill(int playerNo)
{
    int i;
    for(i = 0; i < STAROBJ_MAX; i++) {
        if(starOMObj[i]) {
            STARWORK *work = starOMObj[i]->data;
            if(playerNo == work->playerNo) {
                StarObjKill(starOMObj[i]);
                return;
            }
        }
    }
}

static inline int StarObjPlayerCreate(int playerNo, HuVecF *pos)
{
    int no = StarObjCreate(pos);
    OMOBJ *obj = starOMObj[no];
    STARWORK *work = obj->data;
    work->playerNo = playerNo;
    work->masuId = -1;
    work->masuF = 0;
    work->baseY = pos->y - 300.0f;
    work->pos = *pos;
    return no;
}

void mbStarReset(void)
{
    starResetF = 0;
}

void mbStarInit(void)
{
    memset(starOMObj, 0, sizeof(starOMObj));
    starMin = starNum - 1;
    if(starMin < 1) {
        starMin = 1;
    }
    starEffAnim1 = HuSprAnimRead(HuDataReadNum(0x210001, HU_MEMNUM_OVL));
    HuSprAnimLock(starEffAnim1);
    starEffAnim2 = HuSprAnimRead(HuDataReadNum(0x210001, HU_MEMNUM_OVL));
    HuSprAnimLock(starEffAnim2);
    starMasuProc = NULL;
    starFreeProc = NULL;
    starMoveHook = NULL;
    starMasuFunc = NULL;
    mbZtarObjInit();
}

void mbStarClose(void)
{
    int i;
    for(i = 0; i < STAROBJ_MAX; i++) {
        if(starOMObj[i]) {
            StarObjKill(starOMObj[i]);
            starOMObj[i] = NULL;
        }
    }
    if(starEffAnim1) {
        HuSprAnimKill(starEffAnim1);
        starEffAnim1 = NULL;
    }
    if(starEffAnim1) {
        HuSprAnimKill(starEffAnim1);
        starEffAnim1 = NULL;
    }
    mbZtarObjClose();
}

int mbStarObjCreate(void)
{
    int no = StarObjCreate(NULL);
    OMOBJ *obj = starOMObj[no];
    STARWORK *work = obj->data;
    work->masuId = work->playerNo = -1;
    work->masuF = 2;
    return no;
}

void mbStarObjPosSet(int no, float posX, float posY, float posZ)
{
    OMOBJ *obj = starOMObj[no];
    STARWORK *work = obj->data;
    work->pos.x = posX;
    work->pos.y = posY;
    work->pos.z = posZ;
}

void mbStarObjPosSetV(int no, HuVecF *pos)
{
    mbStarObjPosSet(no, pos->x, pos->y, pos->z);
}

void mbStarObjRotSet(int no, float rotX, float rotY, float rotZ)
{
    OMOBJ *obj = starOMObj[no];
    STARWORK *work = obj->data;
    work->rot.x = rotX;
    work->rot.y = rotY;
    work->rot.z = rotZ;
}

void mbStarObjRotSetV(int no, HuVecF *rot)
{
    mbStarObjRotSet(no, rot->x, rot->y, rot->z);
}

void mbStarObjScaleSet(int no, float scaleX, float scaleY, float scaleZ)
{
    OMOBJ *obj = starOMObj[no];
    STARWORK *work = obj->data;
    work->scale.x = scaleX;
    work->scale.y = scaleY;
    work->scale.z = scaleZ;
}

void mbStarObjScaleSetV(int no, HuVecF *scale)
{
    mbStarObjScaleSet(no, scale->x, scale->y, scale->z);
}

void mbStarObjPosGet(int no, HuVecF *pos)
{
    OMOBJ *obj = starOMObj[no];
    STARWORK *work = obj->data;
    *pos = work->pos;
}

void mbStarObjRotGet(int no, HuVecF *rot)
{
    OMOBJ *obj = starOMObj[no];
    STARWORK *work = obj->data;
    *rot = work->rot;
}

void mbStarObjScaleGet(int no, HuVecF *scale)
{
    OMOBJ *obj = starOMObj[no];
    STARWORK *work = obj->data;
    *scale = work->scale;
}

void mbStarObjKill(int no)
{
    StarObjKill(starOMObj[no]);
}

void mbStarObjDispSet(int no, BOOL dispF)
{
    OMOBJ *obj = starOMObj[no];
    STARWORK *work;
    mbObjDispSet(obj->mdlId[0], dispF);
    work = obj->data;
    work->effDispF = dispF;
    work->dispF = dispF;
}

void mbStarObjDispFlagSet(int no, BOOL dispF)
{
    OMOBJ *obj = starOMObj[no];
    STARWORK *work = obj->data;
    work->effDispF = dispF;
}

void mbStarChestCreate(int no, int playerNo)
{
    OMOBJ *obj = starOMObj[no];
    STARWORK *work = obj->data;
    HuVecF pos;
    work->masuF = 0;
    work->playerNo = playerNo;
    mbPlayerPosGet(playerNo, &pos);
    work->baseY = pos.y;
    mbStarGetMain(playerNo, NULL, starAddNum, TRUE);
}

static int StarObjCreate(HuVecF *pos)
{
    int i;
    for(i = 0; i < STAROBJ_MAX; i++) {
        if(!starOMObj[i]) {
            OMOBJ *obj;
            STARWORK *work;
            int modelId;
            STARPARTICLE *particle;
            obj = omAddObjEx(mbObjMan, 257, 1, 0, -1, StarObjOMExec);
            starOMObj[i] = obj;
            work = HuMemDirectMallocNum(HEAP_HEAP, sizeof(STARWORK), HU_MEMNUM_OVL);
            obj->data = work;
            memset(work, 0, sizeof(STARWORK));
            work->obj = obj;
            obj->mdlId[0] = mbObjCreate(mbBoardDataNumGet(0x50005), NULL, TRUE);
            modelId = mbObjModelIDGet(obj->mdlId[0]);
            StarModelInit(modelId);
            mbObjRotSet(obj->mdlId[0], -90, 0, 0);
            mbObjZWriteOffSet(obj->mdlId[0], FALSE);
            mbObjLayerSet(obj->mdlId[0], 3);
            work->objNo = i;
            work->mode = STAR_MODE_IDLE;
            work->effMdlId = StarObjEffCreate(starEffAnim1);
            particle = StarParticleGet(work->effMdlId);
            particle->work = work;
            work->effDispF = TRUE;
            work->time = 0;
            work->dispF = TRUE;
            work->effF = TRUE;
            work->ofs.x = work->ofs.y = work->ofs.z = 0.0f;
            work->rot.x = work->rot.y = work->rot.z = 0.0f;
            work->scale.x = work->scale.y = work->scale.z = 1.0f;
            omSetStatBit(obj, 0x100);
            return i;
        }
    }
    return -1;
}

void mbStarMoveHookSet(void *hook)
{
    starMoveHook = hook;
}

void mbStarMasuFuncSet(void *func)
{
    starMasuFunc = func;
}

static inline void StarChestModelLoopSet(s16 mdlId)
{
    mbObjAttrSet(mdlId, 0x40000001);
}

static inline int StarObjMasuCreate(int masuId, HuVecF *pos)
{
    int no = StarObjCreate(pos);
    OMOBJ *obj = starOMObj[no];
    STARWORK *work = obj->data;
    work->masuId = masuId;
    work->playerNo = -1;
    work->masuF = 1;
    work->chestMdlId = mbObjCreate(mbBoardDataNumGet(0x50006), NULL, FALSE);
    mbObjMotionTimeSet(work->chestMdlId, 0.0f);
    mbObjMotionSpeedSet(work->chestMdlId, 1.0f);
    StarChestModelLoopSet(work->chestMdlId);
    mbObjLayerSet(work->chestMdlId, 3);
    work->baseY = pos->y - 300.0f;
    work->pos = *pos;
    return no;
}

void mbStarMasuNextSet(int masuId)
{
    HuVecF pos;
    int objNo;
    OMOBJ *obj;
    starMasuPrevType = mbMasuTypeGet(masuId);
    mbMasuTypeSet(masuId, 7);
    mbMasuPosGet(masuId, &pos);
    pos.y += 300.0f;
    objNo = StarObjMasuCreate(masuId, &pos);
    obj = starOMObj[objNo];
    mbMasuCapsuleSet(masuId, -1);
    starMasuNext = masuId;
}

void mbStarGetExec(int playerNo, HuVecF *pos)
{
    HuVecF playerPos;
    int objNo;
    OMOBJ *obj;
    STARWORK *work;
    int seNo;
    mbPlayerPosGet(playerNo, &playerPos);
    playerPos.y += 300.0f;
    objNo = StarObjPlayerCreate(playerNo, &playerPos);
    obj = starOMObj[objNo];
    work = obj->data;
    seNo = mbAudFXPlay(0x447);
    mbAudFXPlay(0x448);
    StarObjGrowSet(obj);
    StarObjGrowWait(obj);
    mbAudFXStop(seNo);
    mbStarGetMain(playerNo, NULL, starAddNum, TRUE);
}

void mbStarAddNumSet(int num)
{
    starAddNum = num;
}

void mbStarStub(void)
{
}

void mbev_StarMasu(int playerNo, int masuId)
{
    mbMoveNumDispSet(playerNo, FALSE);
    starMasuProc = HuPrcChildCreate(ev_StarMasu, 8195, 8192, 0, mbMainProc);
    HuPrcDestructorSet2(starMasuProc, ev_StarMasuKill);
    while(starMasuProc != NULL) {
        HuPrcVSleep();
    }
    mbMoveNumDispSet(playerNo, TRUE);
}

void mbev_StarFreeMasu(int playerNo)
{
    mbMoveNumDispSet(playerNo, FALSE);
    starFreeProc = HuPrcChildCreate(ev_StarFreeMasu, 8195, 8192, 0, mbMainProc);
    HuPrcDestructorSet2(starFreeProc, ev_StarFreeMasuKill);
    while(starFreeProc != NULL) {
        HuPrcVSleep();
    }
    mbMoveNumDispSet(playerNo, TRUE);
}

static void ev_StarMasu(void)
{
    if(starMasuFunc != NULL) {
        starMasuFunc();
    } else {
        ev_StarMasuRun(FALSE);
    }
    HuPrcEnd();
}

static void ev_StarFreeMasu(void)
{
    ev_StarMasuRun(TRUE);
    HuPrcEnd();
}

static void ev_StarMasuKill(void)
{
    starMasuProc = NULL;
}

static void ev_StarFreeMasuKill(void)
{
    starFreeProc = NULL;
}

void mbStarGetMain(int playerNo, HuVecF *pos, int starNum, BOOL focusF)
{
    OMOBJ *obj;
    STARWORK *work;
    int starNo;
    int objNo;
    int seNo;
    int masuId;
    int time;
    int streamNo;
    HuVecF playerPos;
    masuId = GwPlayer[playerNo].masuId;
    seNo = -1;
    if(pos == NULL) {
        obj = StarObjMasuGet(masuId);
        if(obj == NULL) {
            obj = StarObjPlayerGet(playerNo);
        }
        if(obj == NULL) {
            mbPlayerPosGet(playerNo, &playerPos);
            playerPos.y += 300.0f;
            objNo = StarObjPlayerCreate(playerNo, &playerPos);
            obj = starOMObj[objNo];
        }
        work = obj->data;
        starNo = work->no;
    } else {
        objNo = StarObjPlayerCreate(playerNo, (HuVecF *)&pos);
        obj = starOMObj[objNo];
        work = obj->data;
        mbPlayerPosGet(playerNo, &playerPos);
        work->baseY = playerPos.y;
        starNo = -1;
        seNo = mbAudFXPlay(0x447);
        mbAudFXPlay(0x448);
        HuPrcVSleep();
    }
    if(work->masuF == 1 && work->dispF == 0) {
        mbObjDispSet(obj->mdlId[0], TRUE);
        work->effDispF = TRUE;
        work->dispF = TRUE;
        streamNo = mbAudFXPlay(0x447);
        mbAudFXPlay(0x448);
        StarObjGrowSet(obj);
        StarObjGrowWait(obj);
        mbAudFXStop(streamNo);
    }
    _SetFlag(0x10022);
    mbMusFadeOutSpeed(0, 1000);
    HuPrcSleep(60);
    StarObjShrinkIdleSet(obj);
    StarObjShrinkIdleWait(obj);
    mbPlayerMotionSet(playerNo, 11, 0);
    time = 0;
    do {
        if(time++ == 27) {
            StarObjShrinkSet(obj);
        }
        if(time == 30) {
            if(seNo >= 0) {
                mbAudFXStop(seNo);
            }
            mbAudFXPlay(0x449);
            mbStarObjDispFlagSet(work->objNo, FALSE);
        }
        HuPrcVSleep();
    } while(!mbPlayerMotionEndCheck(playerNo));
    mbPlayerMotIdleSet(playerNo);
    omVibrate(playerNo, 20, 7, 3);
    streamNo = mbMusJinglePlay(39);
    mbPlayerWinLoseVoicePlay(playerNo, 7, 573);
    mbPlayerMotionShiftSet(playerNo, 7, 0.0f, 8.0f, 0);
    if(focusF) {
        MBCAMERAVIEWKEY view;
        float weight;
        float deltaZoom;
        float zoomNew;
        float zoomBase;
        mbCameraMoveOnSet(FALSE);
        view.zoom = 1600.0f;
        mbCameraRotGet(&view.rot);
        view.rot.x -= 15.0f;
        mbObjPosGet(obj->mdlId[0], &view.pos);
        mbCameraMovePlayer(playerNo, &view.rot, &starViewOfs, view.zoom, -1.0f, 102);
        mbCameraMoveWait();
        zoomBase = mbCameraZoomGet();
        mbCameraFocusObjSet(-1);
        for(time = 0; time < 30u; time++) {
            weight = time / 30.0f;
            deltaZoom = (1.0 + HuSin(720.0f * weight)) * 0.5;
            zoomNew = zoomBase + (-500.0f * deltaZoom);
            mbCameraZoomSet(zoomNew);
            HuPrcVSleep();
        }
    }
    mbPlayerStarAdd(playerNo, starNum);
    GwSystem.starTotal = GwSystem.starTotal + starNum;
    if(starNo >= 0) {
        mbStarNextNoSet(starNo);
        mbStarFlagReset(starNo);
    }
    mbMusJingleWait(streamNo);
    mbPlayerMotionEndWait(playerNo);
    if(focusF) {
        mbCameraMoveWait();
    }
    HuPrcSleep(30);
    _ClearFlag(0x10022);
    if(work->masuF == 0) {
        StarObjPlayerKill(playerNo);
    } else if(work->masuF == 1) {
        StarObjMasuKill(GwPlayer[playerNo].masuId);
    }
    HuPrcSleep(12);
    starAddNum = 1;
}

void mbStarGetPosExec(int playerNo, HuVecF *pos)
{
    mbStarGetMain(playerNo, pos, 1, TRUE);
}

static void StarObjOMExec(OMOBJ *obj)
{
    STARWORK *work = obj->data;
    float t;
    if(work->killF || mbExitCheck()) {
        if(work->masuF == 1) {
            mbObjKill(work->chestMdlId);
        }
        StarObjEffKill(work->effMdlId);
        work->effMdlId = -1;
        work->masuF = -1;
        mbObjKill(obj->mdlId[0]);
        obj->mdlId[0] = -1;
        omDelObjEx(HuPrcCurrentGet(), obj);
        starOMObj[work->objNo] = NULL;
        return;
    }
    if(!_CheckFlag(0x1000E) && work->masuF == 1 && work->dispF == 0) {
        BOOL dispF = TRUE;
        int masuId = GwPlayer[GwSystem.turnPlayerNo].masuId;
        if(work->masuId == masuId) {
            dispF = FALSE;
        }
        if(work->effF == 0) {
            dispF = FALSE;
        }
        if(dispF) {
            mbObjDispSet(obj->mdlId[0], TRUE);
            work->effDispF = TRUE;
            work->dispF = TRUE;
            StarObjGrowSet(obj);
        }
    }
    switch(work->mode) {
        case STAR_MODE_GROW:
            t = (work->time++) / 90.0f;
            work->scale.x = mbSinDeg(90.0f * t);
            work->scale.y = work->scale.z = work->scale.x;
            if(work->time > 90) {
                work->rot.y = 0.0f;
                work->rotF = FALSE;
                work->mode = STAR_MODE_IDLE;
                work->time = 0;
                return;
            }
            break;

        case STAR_MODE_IDLE:
            if(work->masuF == 1) {
                HuVecF pos;
                mbMasuPosGet(work->masuId, &pos);
                work->pos.x = pos.x;
                work->pos.z = pos.z;
            }
            work->ofs.y = 100 * (0.2f * HuSin(4.0f * work->time));
            work->time++;
            break;

        case STAR_MODE_SHRINKIDLE:
            work->mode = STAR_MODE_SHRINKIDLE_END;
            break;

        case STAR_MODE_SHRINK:
            work->pos.y += -4.0f;
            if(work->pos.y > 80.0f + work->baseY) {
                if(work->angle < 90) {
                    work->angle += 2;
                    if(work->angle > 90) {
                        work->angle = 90;
                    }
                }
                t = work->angle;
                work->scale.x = HuSin(90.0f + t);
                if(work->scale.x <= 0.0f) {
                    work->scale.x = 0.001f;
                }
                work->scale.y = work->scale.z = work->scale.x;
            } else {
                work->killF = TRUE;
                mbObjDispSet(obj->mdlId[0], FALSE);
                if(work->masuF == 1) {
                    mbObjDispSet(work->chestMdlId, FALSE);
                }
                return;
            }
            break;

        case STAR_MODE_SHRINKIDLE_END:
            break;
    }
    StarObjRotate(work, obj);
    mbObjPosSet(obj->mdlId[0], work->pos.x + work->ofs.x, work->pos.y + work->ofs.y, work->pos.z + work->ofs.z);
    mbObjRotSetV(obj->mdlId[0], &work->rot);
    mbObjScaleSetV(obj->mdlId[0], &work->scale);
    if(work->masuF == 1) {
        mbObjPosSet(work->chestMdlId, work->pos.x + work->ofs.x, work->pos.y + work->ofs.y, (work->pos.z + work->ofs.z) - 10.0f);
        mbObjRotSetV(work->chestMdlId, &work->rot);
        mbObjScaleSet(work->chestMdlId, 2.0f * work->scale.x, 2.0f * work->scale.y, 2.0f * work->scale.z);
    }
}

static void StarObjKill(OMOBJ *obj)
{
    STARWORK *work = obj->data;
    work->killF = TRUE;
}

static void StarObjRotate(STARWORK *work, OMOBJ *obj)
{
    float rotSpeed;
    if(work->rotF) {
        if(work->rotSpeed < 16.0f) {
            work->rotSpeed += 2.0f;
            if(work->rotSpeed > 16.0f) {
                work->rotSpeed = 16;
            }
        }
        OSs8tof32(&work->rotSpeed, &rotSpeed);
        if(work->mode == STAR_MODE_GROW) {
            work->rot.y = 720.0f * mbSinDeg(work->time);
        } else {
            work->rot.y = mbAngleWrap(work->rot.y + rotSpeed);
        }
    }
}

static void StarObjShrinkIdleSet(OMOBJ *obj)
{
    STARWORK *work = obj->data;
    work->mode = STAR_MODE_SHRINKIDLE;
    if(work->dispF == 0) {
        mbObjDispSet(obj->mdlId[0], TRUE);
        work->pos.y = 800.0f + work->baseY;
    }
}

static void StarObjGrowSet(OMOBJ *obj)
{
    STARWORK *work = obj->data;
    work->rotF = TRUE;
    work->angle = 0;
    work->time = 0;
    work->mode = STAR_MODE_GROW;
    work->effDispF = TRUE;
    work->ofs.y = 0.0f;
    work->dispF = TRUE;
    work->effF = TRUE;
    if(work->masuF == 1) {
        mbObjDispSet(work->chestMdlId, TRUE);
    }
}

static void StarObjShrinkSet(OMOBJ *obj)
{
    STARWORK *work = obj->data;
    work->rotF = TRUE;
    work->angle = 0;
    work->rotSpeed = 0;
    work->mode = STAR_MODE_SHRINK;
}

static void StarObjGrowWait(OMOBJ *obj)
{
    STARWORK *work = obj->data;
    while(work->mode != STAR_MODE_IDLE) {
        HuPrcVSleep();
    }
    HuPrcSleep(20);
}

static void StarObjShrinkIdleWait(OMOBJ *obj)
{
    STARWORK *work = obj->data;
    while(work->mode != STAR_MODE_SHRINKIDLE_END) {
        HuPrcVSleep();
    }
}

static s16 StarObjEffCreate(ANIMDATA *anim)
{
    s16 mdlId = mbParticleCreate(anim, 64);
    mbParticleHookSet(mdlId, StarObjEffHook);
    Hu3DModelLayerSet(mdlId, 5);
    return mdlId;
}

static void StarObjEffKill(s16 mdlId)
{
    if(mdlId >= 0) {
        mbParticleKill(mdlId);
    }
}

static void StarObjEffHook(HU3D_MODEL *modelP, STARPARTICLE *particle, Mtx mtx)
{
    STARWORK *work = particle->work;
    OMOBJ *obj = work->obj;
    GXColor color = { 255, 255, 192, 192 };
    STARPARTICLEDATA *data;
    int i;
    int num;
    HuVecF dir;
    float t;
    if(particle->initF == 0) {
        data = particle->data;
        particle->unk20 = 1;
        for(i = 0; i < particle->num; i++, data++) {
            data->vel.y = 0.0f;
            data->scale = 0.0f;
            data->color = color;
            data->time = 0;
            data->maxTime = -1;
        }
        particle->initF = 1;
    }
    num = 0;
    data = particle->data;
    for(i = 0; i < particle->num; i++, data++) {
        if(!work->effDispF && data->maxTime < 0) {
            continue;
        }
        if(!work->dispF || !work->effF) {
            data->vel.y = 0.0f;
            data->scale = 0.0f;
        } else if(data->maxTime < 0) {
            dir.x = frandf() - frandf();
            dir.y = frandf() - frandf();
            dir.z = frandf() - frandf();
            PSVECNormalize(&dir, &dir);
            PSVECScale(&dir, &dir, 80.0f * work->scale.x);
            data->pos.x = dir.x + work->pos.x;
            data->pos.y = dir.y + work->pos.y;
            data->pos.z = dir.z + work->pos.z;
            data->vel.y = 0.0f;
            data->scale = 20.0f + (work->scale.x * mbRandMod(20));
            data->maxTime = mbRandMod(12) + 18;
            data->time = 0;
        }
        t = (float)(data->time++) / data->maxTime;
        data->pos.y += 0.016666668f * data->vel.y;
        data->vel.y += -16.333334f;
        data->color.a = 255.0f * (1.0f - t);
        if(data->time > data->maxTime) {
            data->scale = 0.0f;
            data->maxTime = -1;
        }
        num++;
    }
}

void mbStarStub2(void)
{
}

void mbStarMapViewProcExec(int playerNo, HuVecF *pos, int no)
{
    OMOBJ *guideObj = NULL;
    s16 startMasuId;
    int i;
    int masuNum;
    int winId;
    float zoom;
    s16 masuList[8];
    HuVecF startPos;
    HuVecF endPos;
    HuVecF center;
    HuVecF rot;
    HuVecF guidePos;
    masuNum = mbMasuTypeListGet(7, masuList);
    if(masuNum > 0) {
        mbWipeFadeOut();
        mbMusPauseFadeOut(0, TRUE, 1000);
        HuPrcSleep(120);
        for(i = 0; i < 4; i++) {
            mbPlayerMotionShiftSet(i, 1, 0.0f, 1.0f, 0x40000001);
        }
        mbCameraZoomSet(2400.0f);
        mbCameraRotSet(325.0f, 0.0f, 0.0f);
        startMasuId = mbMasuFind_AttrIdGet(-1, 0x8000);
        mbMasuPosGet(startMasuId, &startPos);
        mbMasuPosGet(masuList[0], &endPos);
        mbNormPosto3D(&starGuidePos, 4, &guidePos);
        guideObj = mbGuideCreateFlag(&guidePos, starGuideMotTbl, TRUE, FALSE, TRUE);
        mbGuideMotionNextSet(guideObj, 1);
        mbStatusDispForceSetAll(FALSE);
        mbMusPlay(1, 12, 127, 0);
        mbWipeFadeIn();
        mbAudGuidePlay(952);
        mbGuideMotionShiftSet(guideObj, 12, TRUE);
        winId = mbWinCreateTime(4, 0x270010, -1);
        mbWinPause(winId);
        mbev_StarScroll(&startPos, &endPos, 150);
        mbWinKill(winId);
        mbAudGuidePlay(952);
        mbGuideMotionShiftSet(guideObj, 12, TRUE);
        mbWinCreateTime(4, 0x27000E, -1);
        mbWinTopWait();
        mbWipeFadeOut();
        mbMusFadeOutSpeed(1, 1000);
        HuPrcSleep(60);
        mbStatusDispForceSetAll(TRUE);
        mbGuideKill(guideObj);
        guideObj = NULL;
        zoom = mbOpeningZoomGet();
        mbOpeningRotGet(&rot);
        mbOpeningPosGet(&center);
        mbCameraZoomSet(zoom);
        mbCameraRotSetV(&rot);
        mbCameraCenterSetV(&center);
        mbMusPauseFadeOut(0, FALSE, 1000);
        mbWipeFadeIn();
    }
}

void mbStarMapViewExec(int playerNo, int no)
{
}

void mbStarNumSet(int num)
{
    starNum = num;
}

int mbStarNumGet(void)
{
    return starNum;
}

void mbStarFlagInit(void)
{
    GwSystem.starFlag = 0;
}

void mbStarFlagSet(int no)
{
    GwSystem.starFlag |= (1 << no);
}

void mbStarFlagReset(int no)
{
    GwSystem.starFlag &= ~(1 << no);
}

u8 mbStarFlagGet(void)
{
    return GwSystem.starFlag;
}

void mbStarNextNoSet(s8 nextNo)
{
    GwSystem.starPos = nextNo;
}

u8 mbStarNextNoGet(void)
{
    return GwSystem.starPos;
}

void mbStarNoSet(int no)
{
}

int mbStarNoRandGet(void)
{
    return -1;
}

void mbStarNoDispSet(void)
{
}

static inline void StarObjDispSet(int no, BOOL dispF)
{
    OMOBJ *obj = starOMObj[no];
    if(obj) {
        STARWORK *work;
        mbObjDispSet(obj->mdlId[0], dispF);
        work = obj->data;
        work->effDispF = dispF;
        work->dispF = dispF;
        if(work->masuF == 1) {
            mbObjDispSet(work->chestMdlId, dispF);
        }
    }
}

void mbStarMasuDispSet(int masuId, BOOL dispF)
{
    int no = StarObjNoMasuGet(masuId);
    if(no >= 0) {
        StarObjDispSet(no, dispF);
    }
}

void mbStarDispSetAll(BOOL dispF)
{
    int i;
    for(i = 0; i < STAROBJ_MAX; i++) {
        StarObjDispSet(i, dispF);
    }
}

void mbStarObjDispSetAll(BOOL effF)
{
    int i;
    for(i = 0; i < STAROBJ_MAX; i++) {
        if(starOMObj[i]) {
            STARWORK *work = starOMObj[i]->data;
            work->effF = effF;
        }
        StarObjDispSet(i, effF);
    }
}

static inline int StarAdd(int playerNo, int num, BOOL instF)
{
    int i;
    int delay;
    int sign;
    int result;
    int total;
    int addNum;
    s16 seId;
    if(abs(num) >= 50) {
        delay = 1;
    } else if(abs(num) >= 20) {
        delay = 3;
    } else {
        delay = 6;
    }
    total = num + mbPlayerStarGet(playerNo);
    addNum = num;
    if(total > 999) {
        addNum = 999 - mbPlayerStarGet(playerNo);
    } else if(total < 0) {
        addNum = -mbPlayerStarGet(playerNo);
    }
    result = addNum;
    if(!instF) {
        sign = (addNum >= 0) ? 1 : -1;
        seId = (sign > 0) ? 8 : 50;
        for(i = 0; i < abs(addNum); i++) {
            mbPlayerStarAdd(playerNo, sign);
            mbAudFXPlay(seId);
            HuPrcSleep(delay);
        }
    } else {
        mbPlayerStarAdd(playerNo, result);
    }
    return result;
}

int mbStarAddProcExec(int playerNo, int num, BOOL dispF, BOOL instF)
{
    int result = 0;
    if(num != 0) {
        result = StarAdd(playerNo, num, instF);
    }
    if(result != 0 || dispF) {
        mbAudFXPlay(15);
        mbStarDispPlayerCreate(playerNo, result);
        while(!mbStarDispCheck(playerNo)) {
            HuPrcVSleep();
        }
    }
    return result;
}

int mbStarAddDispExec(int playerNo, int num, BOOL dispF, BOOL instF)
{
    int result = StarAdd(playerNo, num, instF);
    if(result != 0) {
        mbAudFXPlay(15);
    }
    if(dispF && result != 0) {
        mbStarDispPlayerCreate(playerNo, result);
        while(!mbStarDispCheck(playerNo)) {
            HuPrcVSleep();
        }
    }
    return result;
}

int mbStarAddExec(int playerNo, int num)
{
    return mbStarAddDispExec(playerNo, num, FALSE, FALSE);
}

static void StarAddAllProc(int *numTbl, BOOL instF, int *outTbl)
{
    int total;
    int playerNo;
    int i;
    int num[4];
    int delay[4];
    int time[4];
    int signTbl[4];
    BOOL addF[4];
    for(i = 0; i < 4; i++) {
        num[i] = numTbl[i];
        addF[i] = FALSE;
    }
    if(GWTeamGet() == FALSE) {
        for(i = 0; i < 4; i++) {
            total = num[i] + mbPlayerStarGet(i);
            if(total > 999) {
                num[i] = 999 - mbPlayerStarGet(i);
            } else if(total < 0) {
                num[i] = -mbPlayerStarGet(i);
            }
            outTbl[i] = num[i];
            addF[i] = TRUE;
        }
    } else {
        for(i = 0; i < 2; i++) {
            playerNo = mbPlayerTeamFindPlayer(i, 0);
            addF[playerNo] = TRUE;
            num[playerNo] = mbStatTeamMinValGet(i, mbPlayerGrpStarGet(i), 999, numTbl, outTbl);
        }
    }
    if(!instF) {
        for(i = 0; i < 4; i++) {
            if(addF[i]) {
                if(abs(num[i]) >= 50) {
                    delay[i] = 1;
                } else if(abs(num[i]) >= 20) {
                    delay[i] = 3;
                } else {
                    delay[i] = 6;
                }
                time[i] = delay[i];
                signTbl[i] = (num[i] >= 0) ? 1 : -1;
            }
        }
        do {
            playerNo = 0;
            for(i = 0; i < 4; i++) {
                if(addF[i] && num[i] != 0) {
                    if(--time[i] == 0) {
                        mbPlayerStarAdd(i, signTbl[i]);
                        num[i] -= signTbl[i];
                        mbAudFXPlay(8);
                        time[i] = delay[i];
                    }
                    playerNo++;
                }
            }
            HuPrcVSleep();
        } while(playerNo != 0);
    } else {
        for(i = 0; i < 4; i++) {
            if(addF[i]) {
                mbPlayerStarAdd(i, num[i]);
            }
        }
    }
    mbAudFXPlay(15);
}

void mbStarAddAllProcExecV(int *numTbl, int *dispTbl, BOOL instF)
{
    int i;
    BOOL waitF;
    int out[4];
    BOOL dispF[4];
    StarAddAllProc(numTbl, instF, out);
    for(i = 0; i < 4; i++) {
        dispF[i] = FALSE;
        if(out[i] != 0 || dispTbl[i] != 0) {
            mbStarDispPlayerCreate(i, out[i]);
            dispF[i] = TRUE;
        }
    }
    do {
        waitF = FALSE;
        for(i = 0; i < 4; i++) {
            if(dispF[i] && !mbStarDispCheck(i)) {
                waitF = TRUE;
            }
        }
        HuPrcVSleep();
    } while(waitF);
}

void mbStarAddAllProcExec(int numP1, int numP2, int numP3, int numP4, BOOL dispF, BOOL instF)
{
    int i;
    BOOL waitF;
    int num[4];
    int out[4];
    BOOL dispFlag[4];
    num[0] = numP1;
    num[1] = numP2;
    num[2] = numP3;
    num[3] = numP4;
    StarAddAllProc(num, instF, out);
    if(dispF) {
        for(i = 0; i < 4; i++) {
            if(out[i] != 0) {
                mbStarDispPlayerCreate(i, out[i]);
                dispFlag[i] = TRUE;
            }
        }
        do {
            waitF = FALSE;
            for(i = 0; i < 4; i++) {
                if(dispFlag[i] && !mbStarDispCheck(i)) {
                    waitF = TRUE;
                }
            }
            HuPrcVSleep();
        } while(waitF);
    }
}

void mbStarAddAllExec(int numP1, int numP2, int numP3, int numP4)
{
    int num[4];
    int out[4];
    num[0] = numP1;
    num[1] = numP2;
    num[2] = numP3;
    num[3] = numP4;
    StarAddAllProc(num, FALSE, out);
}

int mbStarDispPlayerCreate(int playerNo, int num)
{
    HuVecF pos;
    mbPlayerPosGet(playerNo, &pos);
    pos.y += 250.0f;
    mbStarDispCreate(playerNo, &pos, num);
    return playerNo;
}

static inline void *StarDispWorkAlloc(void)
{
    return HuMemDirectMallocNum(HEAP_HEAP, sizeof(STARDISPWORK), HU_MEMNUM_OVL);
}

static const int SignMdlTbl[2] = { 0x50015, 0x50016 };

int mbStarDispCreate(int playerNo, HuVecF *posP, int num)
{
    HuVecF pos;
    int signNo;
    int modelId;
    int ones;
    HU3D_MODEL *modelP;
    HSF_DATA *hsf;
    HSF_MATERIAL *material;
    int tens;
    int i;
    OMOBJ *obj;
    STARDISPWORK *work;
    float motTime;
    if(posP == NULL) {
        mbPlayerPosGet(playerNo, &pos);
        pos.y += 250.0f;
    } else {
        pos = *posP;
    }
    signNo = (num < 0) ? 1 : 0;
    tens = abs(num) / 10;
    ones = abs(num) - (tens * 10);
    if(tens > 9) {
        tens = 9;
    }
    if(ones > 9) {
        ones = 9;
    }
    if(signNo) {
        motTime = 2.5f;
    } else {
        motTime = 1.5f;
    }
    obj = starDispObj[playerNo] = omAddObjEx(mbObjMan, 0x8000, 0, 0, -1, StarDispUpdate);
    work = obj->data = StarDispWorkAlloc();
    memset(work, 0, sizeof(STARDISPWORK));
    work->mdlId[0] = mbObjCreate(mbBoardDataNumGet(SignMdlTbl[signNo]), NULL, FALSE);
    work->mdlId[1] = mbObjCreate(mbBoardDataNumGet(numberFileTbl[ones]), NULL, FALSE);
    work->mdlId[2] = mbObjCreate(mbBoardDataNumGet(numberFileTbl[tens]), NULL, FALSE);
    work->mdlId[3] = mbObjCreate(mbBoardDataNumGet(0x50005), NULL, FALSE);
    modelId = mbObjModelIDGet(work->mdlId[3]);
    modelP = &Hu3DData[modelId];
    hsf = modelP->hsf;
    material = hsf->material;
    modelP->hiliteIdx = 0;
    Hu3DModelMatHookSet(modelId, StarMatHook);
    for(i = 0; i < hsf->materialNum; i++, material++) {
        material->flags |= 0x2000;
    }
    for(i = 0; i < 3; i++) {
        mbObjPosSetV(work->mdlId[i], &pos);
        mbObjScaleSet(work->mdlId[i], 0.001f, 0.001f, 0.001f);
        mbObjMotionTimeSet(work->mdlId[i], motTime);
        mbObjMotionSpeedSet(work->mdlId[i], 0.0f);
        work->mdlPos[i] = pos;
        mbObjLayerSet(work->mdlId[i], 4);
    }
    mbObjPosSetV(work->mdlId[3], &pos);
    work->mdlPos[3] = pos;
    mbObjScaleSet(work->mdlId[3], 0.001f, 0.001f, 0.001f);
    mbObjLayerSet(work->mdlId[3], 4);
    if(tens == 0) {
        mbObjDispSet(work->mdlId[2], FALSE);
    }
    if(num != 0) {
        if(num >= 0) {
            mbAudFXPlay(8);
        } else {
            mbAudFXPlay(50);
        }
    }
    work->playerNo = playerNo;
    work->num = num;
    work->mode = 0;
    work->unk1C = 0;
    work->time = 0;
    work->delay = 0;
    work->pos = pos;
    obj->trans.x = work->pos.x;
    obj->trans.y = work->pos.y;
    obj->trans.z = work->pos.z;
    obj->rot.x = 0.0f;
    obj->rot.y = 0.0f;
    obj->rot.z = 0.0f;
    obj->scale.x = 0.01f;
    obj->scale.y = 0.01f;
    obj->scale.z = 0.01f;
    return playerNo;
}

int mbStarDispCheck(int playerNo)
{
    if(starDispObj[playerNo] != NULL) {
        return FALSE;
    } else {
        return TRUE;
    }
}

static void StarDispUpdate(OMOBJ *obj)
{
    STARDISPWORK *work = obj->data;
    u16 i;
    float t;
    float scale;
    float ofsY;
    float dist;
    float posX3;
    float posX1;
    float posX2;
    float posX0;
    float angle;
    float weight;
    float baseAngle;
    if(work->delay != 0) {
        work->delay--;
        return;
    }
    if(mbExitCheck()) {
        work->mode = 4;
    }
    switch(work->mode) {
        case 0:
        {
            OSu16tof32(&work->time, &t);
            t = HuSin(t);
            scale = t;
            obj->rot.y = 405.0f * t;
            StarDispObjUpdate(work, 3, obj->trans.x, obj->trans.y, obj->trans.z, scale);
            mbObjRotYSet(work->mdlId[3], obj->rot.y);
            if(work->time < 90) {
                work->time += 6;
                break;
            }
            work->mode++;
            work->time = 0;
            for(i = 0; i < 4; i++) {
                StarDispObjUpdate(work, i, obj->trans.x, obj->trans.y, obj->trans.z, scale);
            }
        }
            break;

        case 1:
        {
            OSu16tof32(&work->time, &t);
            if(work->num >= 10) {
                dist = 140.0f;
            } else {
                dist = 105.0f;
            }
            ofsY = 200.0 * HuSin(2.0f * t);
            t = HuSin(t);
            obj->rot.y = 45.0f + (315.0f * t);
            if(work->num >= 10) {
                posX3 = obj->trans.x + (t * -dist);
                posX0 = obj->trans.x + ((t * -dist) / 3.0f);
                posX1 = obj->trans.x + (t * dist);
                posX2 = obj->trans.x + ((t * dist) / 3.0f);
            } else {
                posX0 = obj->trans.x;
                posX2 = obj->trans.x;
                posX1 = obj->trans.x + (t * dist);
                posX3 = obj->trans.x + (t * -dist);
            }
            StarDispObjUpdate(work, 3, posX3, obj->trans.y + ofsY, obj->trans.z, 1.0f);
            StarDispObjUpdate(work, 0, posX0, obj->trans.y + ofsY, obj->trans.z, 1.0f);
            StarDispObjUpdate(work, 1, posX1, obj->trans.y + ofsY, obj->trans.z, 1.0f);
            StarDispObjUpdate(work, 2, posX2, obj->trans.y + ofsY, obj->trans.z, 1.0f);
            mbObjRotYSet(work->mdlId[3], obj->rot.y);
            mbObjRotYSet(work->mdlId[0], obj->rot.y);
            mbObjRotYSet(work->mdlId[1], obj->rot.y);
            mbObjRotYSet(work->mdlId[2], obj->rot.y);
            if(work->time < 90) {
                work->time += 6;
                break;
            }
            obj->trans.y += ofsY;
            work->mode++;
            work->time = 0;
        }
            break;

        case 2:
        {
            OSu16tof32(&work->time, &t);
            t = HuSin(t);
            if(work->num >= 10) {
                ofsY = (-50.0f * t) + obj->trans.y;
            } else {
                ofsY = (50.0f * t) + obj->trans.y;
            }
            for(i = 0; i < 4; i++) {
                HuVecF pos = work->mdlPos[i];
                StarDispObjUpdate(work, i, pos.x, ofsY, pos.z, 1.0f);
            }
            if(work->time < 90) {
                work->time += 6;
                break;
            }
            work->mode++;
            work->time = 0;
            work->delay = 18;
            obj->scale.x = 1.0f;
            obj->scale.y = 1.0f;
            obj->trans.y = ofsY;
        }
            break;

        case 3:
            weight = work->time / 24.0f;
            obj->rot.y = 90.0 + (270.0 * HuSin(90.0f * weight));
            baseAngle = 180.0f * weight;
            for(i = 0; i < 4; i++) {
                angle = baseAngle - (30.0f * i);
                if(angle < 0.0f) {
                    angle = 0.0f;
                } else if(angle > 90.0f) {
                    angle = 90.0f;
                }
                if(i != 3) {
                    scale = 1.0f;
                } else {
                    scale = HuCos(angle);
                }
                t = HuSin(angle);
                if(work->num >= 0) {
                    StarDispObjUpdate(work, i, work->mdlPos[i].x, obj->trans.y + (100.0f * t), work->mdlPos[i].z, scale);
                } else {
                    StarDispObjUpdate(work, i, work->mdlPos[i].x, obj->trans.y + (-100.0f * t), work->mdlPos[i].z, scale);
                }
                if(i != 3) {
                    mbObjAlphaSet(work->mdlId[i], 255.0 * HuCos(angle));
                }
                mbObjRotYSet(work->mdlId[i], obj->rot.y);
            }
            if(++work->time > 24.0f) {
                for(i = 0; i < 4; i++) {
                    mbObjDispSet(work->mdlId[i], FALSE);
                }
                work->mode++;
                work->time = 0;
            }
            break;

        case 4:
            starDispObj[work->playerNo] = NULL;
            for(i = 0; i < 4; i++) {
                mbObjKill(work->mdlId[i]);
            }
            HuMemDirectFree(obj->data);
            obj->data = NULL;
            omDelObjEx(mbObjMan, obj);
            break;
    }
}

static inline void StarPos3Dto2D(HuVecF pos, HuVecF *out)
{
    HuVecF *posP = &pos;
    Hu3D3Dto2D(posP, HU3D_CAM0, out);
}

static inline void StarPos2Dto3D(HuVecF pos, HuVecF *out)
{
    HuVecF *posP = &pos;
    Hu3D2Dto3D(posP, HU3D_CAM0, out);
}

static void StarDispObjUpdate(STARDISPWORK *work, int no, float posX, float posY, float posZ, float scale)
{
    HuVecF pos;
    HuVecF pos2D;
    HuVecF ofs;
    Mtx cameraMtx;
    Mtx mtx;
    float dist;
    float zoom;
    float scaleZ;
    work->mdlPos[no].x = posX;
    work->mdlPos[no].y = posY;
    work->mdlPos[no].z = posZ;
    pos = work->mdlPos[no];
    StarPos3Dto2D(pos, &pos2D);
    Hu3DCameraSet(0, cameraMtx);
    PSMTXIdentity(mtx);
    mtx[0][3] = pos.x;
    mtx[1][3] = pos.y;
    mtx[2][3] = pos.z;
    PSMTXConcat(cameraMtx, mtx, mtx);
    ofs.x = mtx[0][3];
    ofs.y = mtx[1][3];
    ofs.z = mtx[2][3];
    dist = PSVECMag(&ofs);
    zoom = 800.0f;
    scaleZ = zoom / dist;
    pos2D.z = zoom;
    StarPos2Dto3D(pos2D, &pos);
    mbObjScaleSet(work->mdlId[no], scale * scaleZ, scale * scaleZ, scale * scaleZ);
    mbObjPosSet(work->mdlId[no], pos.x, pos.y, pos.z);
}

static void ev_StarMasuRun(BOOL freeF)
{
    int guideNo;
    int speakerNo;
    int mesOfs;
    int playerNo;
    int masuId;
    OMOBJ *obj;
    STARWORK *work;
    int winId;
    int seNo;
    HuVecF startPos;
    HuVecF endPos;
    HuVecF guidePos;
    guideNo = mbGuideNoGet();
    starGuideObj = NULL;
    if(GwSystem.curTime == 0) {
        speakerNo = 6;
        mesOfs = 0;
    } else {
        speakerNo = 7;
        mesOfs = 1;
    }
    playerNo = GwSystem.turnPlayerNo;
    mbAudFXPlay(0x44A);
    mbPlayerMotIdleSet(playerNo);
    mbPlayerRotateStart(playerNo, 0, 15);
    while(!mbPlayerRotateCheck(playerNo)) {
        HuPrcVSleep();
    }
    mbCameraMoveWait();
    if(mbPlayerStarGet(playerNo) >= 999) {
        mbAudGuidePlay(951);
        mbWinCreate(2, mesOfs + 0x27000A, speakerNo);
        mbWinTopWait();
        goto end;
    }
    if(!freeF && mbPlayerCoinGet(playerNo) < 20) {
        mbAudGuidePlay(951);
        mbWinCreate(2, mesOfs + 0x270004, speakerNo);
        mbWinTopWait();
        goto end;
    }
    mbStatusDispSetAll(FALSE);
    while(!mbStatusOffCheckAll()) {
        HuPrcVSleep();
    }
    mbAudGuidePlay(950);
    mbWinCreate(2, mesOfs + 0x270000, speakerNo);
    mbWinTopInsertMesSet(mbPlayerNameMesGet(playerNo), 0);
    mbWinTopWait();
    mbStatusDispFocusSet(playerNo, TRUE);
    while(!mbStatusMoveCheck(playerNo)) {
        HuPrcVSleep();
    }
    if(!freeF) {
        mbAudGuidePlay(952);
        mbWinCreateChoice(1, mesOfs + 0x270002, speakerNo, 0);
        if(GwPlayer[playerNo].comF) {
            if(mbPlayerCoinGet(playerNo) >= 20) {
                mbComChoiceUpSet();
            } else {
                mbComChoiceDownSet();
            }
        }
        mbWinTopWait();
        if(mbWinTopChoiceGet() == -1) {
            mbAudGuidePlay(951);
            mbWinCreate(2, mesOfs + 0x270008, speakerNo);
            mbWinTopWait();
            mbStatusDispFocusSet(playerNo, FALSE);
            while(!mbStatusMoveCheck(playerNo)) {
                HuPrcVSleep();
            }
            mbStatusDispSetAll(TRUE);
            goto end;
        }
        switch(mbWinTopChoiceGet()) {
            case 0:
                if(mbPlayerCoinGet(playerNo) < 20) {
                    mbAudGuidePlay(951);
                    mbWinCreate(2, mesOfs + 0x270004, speakerNo);
                    mbWinTopWait();
                    mbStatusDispFocusSet(playerNo, FALSE);
                    while(!mbStatusMoveCheck(playerNo)) {
                        HuPrcVSleep();
                    }
                    mbStatusDispSetAll(TRUE);
                    goto end;
                }
                goto buy;

            case 1:
                mbAudGuidePlay(951);
                mbWinCreate(2, mesOfs + 0x270008, speakerNo);
                mbWinTopWait();
                mbStatusDispFocusSet(playerNo, FALSE);
                while(!mbStatusMoveCheck(playerNo)) {
                    HuPrcVSleep();
                }
                mbStatusDispSetAll(TRUE);
                goto end;

            default:
            buy:
                mbAudGuidePlay(952);
                mbWinCreate(2, mesOfs + 0x270006, speakerNo);
                mbWinTopWait();
                mbCoinAddExec(playerNo, -20);
                break;
        }
    } else {
        mbWinCreate(2, mesOfs + 0x270006, speakerNo);
        mbWinTopWait();
    }
    masuId = GwPlayer[playerNo].masuId;
    mbMasuPosGet(masuId, &startPos);
    _SetFlag(0x1001C);
    mbStarGetMain(playerNo, NULL, 1, TRUE);
    mbWipeFadeOut();
    mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 1.0f, 0x40000001);
    mbCameraPlayerViewSetFast(playerNo, 2);
    if(starMoveHook != NULL) {
        starMoveHook();
    }
    obj = StarObjMasuGet(starMasuNext);
    work = obj->data;
    work->time = 0;
    mbMasuPosGet(starMasuNext, &endPos);
    mbObjDispSet(obj->mdlId[0], FALSE);
    work->effDispF = FALSE;
    mbObjDispSet(work->chestMdlId, FALSE);
    mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 1.0f, 0x40000001);
    mbPauseHookPush(StarPauseHook);
    mbNormPosto3D(&starMasuGuidePos, 4, &guidePos);
    starGuideObj = mbGuideCreateFlag(&guidePos, starMasuGuideMotTbl, TRUE, FALSE, TRUE);
    mbGuideMotionNextSet(starGuideObj, 1);
    mbStatusDispForceSetAll(FALSE);
    mbWipeFadeIn();
    mbMusPlay(1, 12, 127, 0);
    mbAudGuidePlay(952);
    mbGuideMotionShiftSet(starGuideObj, 12, TRUE);
    winId = mbWinCreateTime(4, mesOfs + 0x27000C, -1);
    mbWinPause(winId);
    mbev_StarScroll(&startPos, &endPos, 120);
    mbWinKill(winId);
    mbObjDispSet(obj->mdlId[0], TRUE);
    seNo = mbAudFXPlay(0x447);
    mbAudFXPlay(0x448);
    StarObjGrowSet(obj);
    StarObjGrowWait(obj);
    mbAudFXStop(seNo);
    mbAudGuidePlay(952);
    mbGuideMotionShiftSet(starGuideObj, 12, TRUE);
    mbWinCreateTime(4, mesOfs + 0x27000E, -1);
    mbWinTopWait();
    mbMusFadeOutSpeed(1, 1000);
    mbWipeFadeOut();
    mbCameraPlayerViewSetFast(playerNo, 2);
    mbStatusDispForceSet(playerNo, FALSE);
    mbStatusDispForceSetAll(TRUE);
    mbStatusDispForceSetAll(TRUE);
    mbGuideKill(starGuideObj);
    starGuideObj = NULL;
    HuPrcSleep(60);
    mbMusBoardPlay();
    mbPauseHookPop(StarPauseHook);
    mbWipeFadeIn();
end:
    _ClearFlag(0x1001C);
    mbCameraFocusPlayerSet(GwSystem.turnPlayerNo);
    mbCameraMoveWait();
}

static void StarPauseHook(BOOL endF)
{
    if(starGuideObj != NULL) {
        int mdlId = mbGuideModelGet(starGuideObj);
        if(endF) {
            mbObjDispSet(mdlId, TRUE);
        } else {
            mbObjDispSet(mdlId, FALSE);
        }
    }
}

static void StarMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *material)
{
    GXColor color = { 255, 255, 255, 255 };
    u32 tevStage;
    u32 tevStage2;
    GXColor oldColor;
    HuVecF dir = { 0.5f, -0.5f, -1.0f };
    HuVecF lightPos;
    HuVecF lightDir;
    Hu3DGLightParamGet(0, &lightPos, &lightDir, &oldColor);
    Hu3DGlobalLight[0].color = color;
    Hu3DGlobalLight[0].dir = dir;
    mbObjStarTevStageSet(drawObj, material, &tevStage, &tevStage2);
    Hu3DGLightColorSet(0, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
    Hu3DGLightPosSetV(0, &lightPos, &lightDir);
}

void mbZtarObjInit(void)
{
    memset(ztarOMObj, 0, sizeof(ztarOMObj));
}

void mbZtarObjClose(void)
{
    int i;
    for(i = 0; i < STAROBJ_MAX; i++) {
        if(ztarOMObj[i]) {
            ZtarObjKill(ztarOMObj[i]);
            ztarOMObj[i] = NULL;
        }
    }
}

static int ZtarObjCreate(HuVecF *pos)
{
    int i;
    for(i = 0; i < STAROBJ_MAX; i++) {
        if(!ztarOMObj[i]) {
            OMOBJ *obj;
            STARWORK *work;
            int modelId;
            STARPARTICLE *particle;
            obj = omAddObjEx(mbObjMan, 257, 1, 0, -1, ZtarObjOMExec);
            ztarOMObj[i] = obj;
            work = HuMemDirectMallocNum(HEAP_HEAP, sizeof(STARWORK), HU_MEMNUM_OVL);
            obj->data = work;
            memset(work, 0, sizeof(STARWORK));
            work->obj = obj;
            obj->mdlId[0] = mbObjCreate(0xE0020, NULL, TRUE);
            modelId = mbObjModelIDGet(obj->mdlId[0]);
            StarModelInit(modelId);
            mbObjRotSet(obj->mdlId[0], -90, 0, 0);
            mbObjZWriteOffSet(obj->mdlId[0], FALSE);
            mbObjLayerSet(obj->mdlId[0], 3);
            work->objNo = i;
            work->mode = STAR_MODE_IDLE;
            work->effMdlId = StarObjEffCreate(starEffAnim1);
            particle = StarParticleGet(work->effMdlId);
            particle->work = work;
            work->effDispF = TRUE;
            work->time = 0;
            work->dispF = TRUE;
            work->ofs.x = work->ofs.y = work->ofs.z = 0.0f;
            work->rot.x = work->rot.y = work->rot.z = 0.0f;
            work->scale.x = work->scale.y = work->scale.z = 1.0f;
            omSetStatBit(obj, 0x100);
            return i;
        }
    }
    return -1;
}

void mbZtarObjDispFlagSet(int no, BOOL dispF)
{
    OMOBJ *obj = ztarOMObj[no];
    STARWORK *work = obj->data;
    work->effDispF = dispF;
}

static inline int ZtarObjMasuCreate(int masuId, HuVecF *pos)
{
    int no = ZtarObjCreate(pos);
    OMOBJ *obj = ztarOMObj[no];
    STARWORK *work = obj->data;
    work->masuId = masuId;
    work->playerNo = -1;
    work->masuF = 1;
    work->baseY = pos->y - 300.0f;
    work->pos = *pos;
    return no;
}

void mbZtarMasuNextSet(int masuId)
{
    HuVecF pos;
    int objNo;
    OMOBJ *obj;
    starMasuPrevType = mbMasuTypeGet(masuId);
    mbMasuTypeSet(masuId, 10);
    mbMasuPosGet(masuId, &pos);
    pos.y += 300.0f;
    objNo = ZtarObjMasuCreate(masuId, &pos);
    obj = ztarOMObj[objNo];
    mbMasuCapsuleSet(masuId, -1);
    ztarMasuNext = masuId;
}

static inline int ZtarObjPlayerCreate(int playerNo, HuVecF *pos)
{
    int no = ZtarObjCreate(pos);
    OMOBJ *obj = ztarOMObj[no];
    STARWORK *work = obj->data;
    work->playerNo = playerNo;
    work->masuId = -1;
    work->masuF = 0;
    work->baseY = pos->y - 300.0f;
    work->pos = *pos;
    return no;
}

void mbZtarGetExec(int playerNo)
{
    HuVecF playerPos;
    int objNo;
    OMOBJ *obj;
    STARWORK *work;
    mbPlayerPosGet(playerNo, &playerPos);
    playerPos.y += 300.0f;
    objNo = ZtarObjPlayerCreate(playerNo, &playerPos);
    obj = ztarOMObj[objNo];
    work = obj->data;
    ZtarObjGrowSet(obj);
    ZtarObjGrowWait(obj);
    mbZtarGetMain(playerNo, NULL, -1, TRUE);
}

static inline int ZtarObjNoMasuGet(int masuId)
{
    int i;
    for(i = 0; i < STAROBJ_MAX; i++) {
        if(ztarOMObj[i]) {
            STARWORK *work = ztarOMObj[i]->data;
            if(work->masuId == masuId) {
                return i;
            }
        }
    }
    return -1;
}

static inline OMOBJ *ZtarObjMasuGet(int masuId)
{
    int no = ZtarObjNoMasuGet(masuId);
    if(no >= 0) {
        return ztarOMObj[no];
    } else {
        return NULL;
    }
}

static inline int ZtarObjNoPlayerGet(int playerNo)
{
    int i;
    for(i = 0; i < STAROBJ_MAX; i++) {
        if(ztarOMObj[i]) {
            STARWORK *work = ztarOMObj[i]->data;
            if(work->playerNo == playerNo) {
                return i;
            }
        }
    }
    return -1;
}

static inline OMOBJ *ZtarObjPlayerGet(int playerNo)
{
    int no = ZtarObjNoPlayerGet(playerNo);
    if(no >= 0) {
        return ztarOMObj[no];
    } else {
        return NULL;
    }
}

static inline void ZtarObjMasuKill(int masuId)
{
    int i;
    for(i = 0; i < STAROBJ_MAX; i++) {
        if(ztarOMObj[i]) {
            STARWORK *work = ztarOMObj[i]->data;
            if(masuId == work->masuId) {
                ZtarObjKill(ztarOMObj[i]);
                return;
            }
        }
    }
}

static inline void ZtarObjPlayerKill(int playerNo)
{
    int i;
    for(i = 0; i < STAROBJ_MAX; i++) {
        if(ztarOMObj[i]) {
            STARWORK *work = ztarOMObj[i]->data;
            if(playerNo == work->playerNo) {
                ZtarObjKill(ztarOMObj[i]);
                return;
            }
        }
    }
}

void mbZtarGetMain(int playerNo, HuVecF *pos, int num, BOOL focusF)
{
    OMOBJ *obj;
    STARWORK *work;
    int objNo;
    int masuId;
    int starNo;
    int seNo;
    int seNo2;
    int time;
    int coin;
    HuVecF playerPos;
    masuId = GwPlayer[playerNo].masuId;
    seNo = -1;
    _SetFlag(0x10022);
    mbMusFadeOutSpeed(0, 1000);
    if(pos == NULL) {
        obj = ZtarObjMasuGet(masuId);
        if(obj == NULL) {
            obj = ZtarObjPlayerGet(playerNo);
        }
        work = obj->data;
        starNo = work->no;
    } else {
        objNo = ZtarObjPlayerCreate(playerNo, (HuVecF *)&pos);
        obj = ztarOMObj[objNo];
        work = obj->data;
        mbPlayerPosGet(playerNo, &playerPos);
        work->baseY = playerPos.y;
        starNo = -1;
        HuPrcVSleep();
    }
    ZtarObjShrinkIdleSet(obj);
    seNo2 = mbAudFXPlay(0x462);
    ZtarObjShrinkIdleWait(obj);
    time = 0;
    do {
        if(time++ == 27) {
            ZtarObjShrinkSet(obj);
        }
        if(time == 30) {
            mbZtarObjDispFlagSet(work->objNo, FALSE);
        }
        HuPrcVSleep();
    } while(time < 60);
    mbAudFXStop(seNo2);
    mbPlayerMotIdleSet(playerNo);
    omVibrate(playerNo, 20, 7, 3);
    mbPlayerMotionShiftSet(playerNo, 8, 0.0f, 8.0f, 0);
    mbPlayerWinLoseVoicePlay(playerNo, 13, 585);
    if(mbPlayerStarGet(playerNo) >= 1) {
        mbStarAddDispExec(playerNo, num, TRUE, TRUE);
        GwSystem.starTotal = GwSystem.starTotal + num;
    } else {
        coin = mbPlayerCoinGet(playerNo);
        if(coin >= 20) {
            mbCoinAddDispExec(playerNo, -20, TRUE, TRUE);
        } else if(coin > 0) {
            mbCoinAddDispExec(playerNo, -coin, TRUE, TRUE);
        }
    }
    mbPlayerMotionEndWait(playerNo);
    _ClearFlag(0x10022);
    if(work->masuF == 0) {
        ZtarObjPlayerKill(playerNo);
    } else if(work->masuF == 1) {
        ZtarObjMasuKill(GwPlayer[playerNo].masuId);
    }
    HuPrcSleep(18);
}

static void ZtarObjOMExec(OMOBJ *obj)
{
    STARWORK *work = obj->data;
    float t;
    if(work->killF || mbExitCheck()) {
        StarObjEffKill(work->effMdlId);
        work->effMdlId = -1;
        work->masuF = -1;
        mbObjKill(obj->mdlId[0]);
        obj->mdlId[0] = -1;
        omDelObjEx(HuPrcCurrentGet(), obj);
        ztarOMObj[work->objNo] = NULL;
        return;
    }
    switch(work->mode) {
        case STAR_MODE_GROW:
            t = (work->time++) / 90.0f;
            work->scale.x = mbSinDeg(90.0f * t);
            work->scale.y = work->scale.z = work->scale.x;
            if(work->time > 90) {
                work->rot.y = 0.0f;
                work->rotF = FALSE;
                work->mode = STAR_MODE_IDLE;
                work->time = 0;
                return;
            }
            break;

        case STAR_MODE_IDLE:
            if(work->masuF == 1) {
                HuVecF pos;
                mbMasuPosGet(work->masuId, &pos);
                work->pos.x = pos.x;
                work->pos.z = pos.z;
            }
            work->ofs.y = 100 * (0.2f * HuSin(4.0f * work->time));
            work->time++;
            break;

        case STAR_MODE_SHRINKIDLE:
            work->mode = STAR_MODE_SHRINKIDLE_END;
            break;

        case STAR_MODE_SHRINK:
            work->pos.y += -4.0f;
            if(work->pos.y > 80.0f + work->baseY) {
                if(work->angle < 90) {
                    work->angle += 2;
                    if(work->angle > 90) {
                        work->angle = 90;
                    }
                }
                t = work->angle;
                work->scale.x = HuSin(90.0f + t);
                if(work->scale.x <= 0.0f) {
                    work->scale.x = 0.001f;
                }
                work->scale.y = work->scale.z = work->scale.x;
            } else {
                work->killF = TRUE;
                mbObjDispSet(obj->mdlId[0], FALSE);
                return;
            }
            break;

        case STAR_MODE_SHRINKIDLE_END:
            break;
    }
    ZtarObjRotate(work, obj);
    mbObjPosSet(obj->mdlId[0], work->pos.x + work->ofs.x, work->pos.y + work->ofs.y, work->pos.z + work->ofs.z);
    mbObjRotSetV(obj->mdlId[0], &work->rot);
    mbObjScaleSetV(obj->mdlId[0], &work->scale);
}

static void ZtarObjKill(OMOBJ *obj)
{
    STARWORK *work = obj->data;
    work->killF = TRUE;
}

static void ZtarObjRotate(STARWORK *work, OMOBJ *obj)
{
    float rotSpeed;
    if(work->rotF) {
        if(work->rotSpeed < 16.0f) {
            work->rotSpeed += 2.0f;
            if(work->rotSpeed > 16.0f) {
                work->rotSpeed = 16;
            }
        }
        OSs8tof32(&work->rotSpeed, &rotSpeed);
        if(work->mode == STAR_MODE_GROW) {
            work->rot.y = 720.0f * mbSinDeg(work->time);
        } else {
            work->rot.y = mbAngleWrap(work->rot.y + rotSpeed);
        }
    }
}

static void ZtarObjShrinkIdleSet(OMOBJ *obj)
{
    STARWORK *work = obj->data;
    work->mode = STAR_MODE_SHRINKIDLE;
    if(work->dispF == 0) {
        mbObjDispSet(obj->mdlId[0], TRUE);
        work->pos.y = 800.0f + work->baseY;
    }
}

static void ZtarObjGrowSet(OMOBJ *obj)
{
    STARWORK *work = obj->data;
    work->rotF = TRUE;
    work->angle = 0;
    work->time = 0;
    work->mode = STAR_MODE_GROW;
    work->effDispF = TRUE;
    work->ofs.y = 0.0f;
    mbAudFXPlay(0x45E);
}

static void ZtarObjShrinkSet(OMOBJ *obj)
{
    STARWORK *work = obj->data;
    work->rotF = TRUE;
    work->angle = 0;
    work->rotSpeed = 0;
    work->mode = STAR_MODE_SHRINK;
}

static void ZtarObjGrowWait(OMOBJ *obj)
{
    STARWORK *work = obj->data;
    while(work->mode != STAR_MODE_IDLE) {
        HuPrcVSleep();
    }
    HuPrcSleep(20);
}

static void ZtarObjShrinkIdleWait(OMOBJ *obj)
{
    STARWORK *work = obj->data;
    while(work->mode != STAR_MODE_SHRINKIDLE_END) {
        HuPrcVSleep();
    }
}
