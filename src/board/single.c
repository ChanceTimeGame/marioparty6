/* @@HEAD@@ */
/* Retail calls mbMasuCapsuleGet and mbMasuFind_TypeStepGet as returning int (no extsh on the
   result), as capevent.c does, and passes mbMgCallSingleOnCheck the minigame overlay. */
#define mbMasuCapsuleGet mbMasuCapsuleGet_s16
#define mbMasuFind_TypeStepGet mbMasuFind_TypeStepGet_s16
#define mbMgCallSingleOnCheck mbMgCallSingleOnCheck_void
#include "game/board/single.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/capsule.h"
#include "game/board/coin.h"
#include "game/board/effect.h"
#include "game/board/exit.h"
#include "game/board/guide.h"
#include "game/board/masu.h"
#include "game/board/math.h"
#include "game/board/mgcall.h"
#include "game/board/object.h"
#include "game/board/pause.h"
#include "game/board/player.h"
#include "game/board/status.h"
#include "game/board/telop.h"
#include "game/board/window.h"
#include "game/board/wipe.h"
#include "game/data.h"
#include "game/esprite.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mgdata.h"
#include "game/mic.h"
#include "game/object.h"
#include "game/process.h"
#include "game/sprite.h"
#include "humath.h"
#include "math.h"
#include "string.h"
#undef mbMasuCapsuleGet
#undef mbMasuFind_TypeStepGet
#undef mbMgCallSingleOnCheck
int mbMasuCapsuleGet(s16 id);
int mbMasuFind_TypeStepGet(s16 id, s16 type);
int mbMgCallSingleOnCheck(int ovl);
int memcmp(const void *a, const void *b, size_t n);

/* Not declared by game/mic.h. */
void HuMCListenerCreate(HUMC_CONTEXTID ctxId, HUMC_CONTEXT_CALLBACK cb, u8 flag);
void HuMCListenerKill(void);

#define SINGLE_EFF_MAX 5

/* One minigame-space effect: the spinning space icon, its sparkles (subMdlId[0]) and the burst
   (subMdlId[1]). id is the slot number plus one. */
typedef struct SingleEff_s {
    /* 0x00 */ BOOL useF;
    /* 0x04 */ BOOL dispF;
    /* 0x08 */ BOOL subDispF;
    /* 0x0C */ s16 masuType;
    /* 0x0E */ s16 id;
    /* 0x10 */ s16 mode;
    /* 0x12 */ s16 mdlId;
    /* 0x14 */ s16 subMdlId[2];
    /* 0x18 */ HuVecF pos;
    /* 0x24 */ HuVecF basePos;
    /* 0x30 */ HuVecF rot;
    /* 0x3C */ HuVecF scale;
    /* 0x48 */ float rotSpeed;
    /* 0x4C */ float bobScale;
    /* 0x50 */ OMOBJ *obj;
    /* 0x54 */ BOOL emitF;
    /* 0x58 */ float tevLevel;
    /* 0x5C */ float alpha;
    /* 0x60 */ s16 time;
    /* 0x62 */ s16 maxTime;
    /* 0x64 */ int seNo;
} SINGLEEFF; /* size 0x68 */

typedef struct SingleMasuOrder_s {
    u8 masuId;
    u8 type;
} SINGLEMASUORDER;

/* Story progress kept in the save (layout from part 3's functions). */
typedef struct SingleSaveWork_s {
    u8 koopaBit; /* 0x00 */
    u8 unk01; /* 0x01 */
    u8 micNum; /* 0x02 */
    s8 micChar; /* 0x03 */
    u8 micMissF; /* 0x04 */
    u8 micHitNum; /* 0x05 */
    u8 mgNum; /* 0x06 */
    u8 mgEvenNum; /* 0x07 */
    u8 mgOddNum; /* 0x08 */
    u16 mgRankTotal; /* 0x0A */
    u8 mgHist[3]; /* 0x0C */
    u8 mgHistNo; /* 0x0F */
    u8 unk10; /* 0x10 */
    u8 unk11; /* 0x11 */
    u8 hist12[3]; /* 0x12 */
    u8 hist12No; /* 0x15 */
    u8 unk16; /* 0x16 */
    u8 unk17; /* 0x17 */
    u8 unk18; /* 0x18 */
    u8 masuNum[12]; /* 0x19 */
} SINGLESAVEWORK; /* size 0x26 */

static ANIMDATA *singleEffAnim[4];
static SINGLEMASUORDER singleMasuOrder[256];
static u32 singleMgUnlock[4];
static SINGLEEFF singleEffData[SINGLE_EFF_MAX];
static u32 mgUnlockOld[4];
static SINGLESAVEWORK singleSaveWork;
static u64 singleBoardFlagOld[3];
static u32 singleMgRecordOld[8];
static u32 singleMgRecordPrize[8];

static int singleTeamChar = -1;
static int mgKoopaCapsuleTbl[2] = { 2, 7 };
static s8 guideLast5MotTbl[3] = { 12, 6, -1 };

/* .sbss: CodeWarrior emits it in reverse definition order. */
static int singleBoard;
static BOOL singleCancelF;
static BOOL singleEndF;
static HUMC_CONTEXTID singleMicContext;
static BOOL singleMicF;
static BOOL singleListenerCreateF;
static BOOL singleListenerOnF;
static int singleMasuOrderNum;
static u8 masuType[5];
static u8 masuTypeNum;
static int returnMode;
static int mgRareSeNo;
static int miniKoopaMgType;
static int miniKoopaType;

static void SingleMicCreate(void);
static void SingleMicKill(void);
static void SingleMicListenerCreate(void);
static void SingleMicListenerKill(void);
static void SingleMasuOrderInit(void);
static void SingleMasuOrderSet(void);
static void SingleMasuTypeReset(void);
static void SingleEffInit(void);
static void SingleEffClose(void);
static s16 SingleEffCreate(HuVecF *pos, int masuType);
static void SingleEffKill(s16 id);
static void SingleEffMgMasuHook(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);
static void SingleEffMgHook(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);
static void SingleEffMgExplodeHook(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);
static void SingleEffMgCapsuleHook(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);
static void SingleEffMgFireHook(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);
static void SingleEffMgFire2Hook(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);
static void SingleEffOMExec(OMOBJ *obj);
static void SingleEffMgStop(s16 id, int type);
static void ev_SingleMg(int playerNo, s16 masuId);
static void ev_SingleMgEnd(int playerNo);
static void ev_SingleRareMg(int playerNo, s16 effId);
static void ev_SingleKoopaMg();
static void ev_SingleKoopaMgSkip(s16 guideObj);
static void ev_SingleKoopaMgEnd(int playerNo);
static void ev_SingleMKoopaMg(int playerNo, s16 masuId);
static void ev_SingleMKoopaMgEnd(int playerNo);
static void SingleMgSaveInit(void);
static void SingleMicListener(HUMC_RESPONSE *resp);
static void SingleFlagFlush(void);
static void SingleMgRecordBackup(void);
static void SingleMgRecordRestore(void);
static void SingleMgRecordPrizeInit(void);
static void SingleMgRecordPrizeSet(void);
static void SingleLast5(void);

static inline SINGLEEFF *SingleEffGet(s16 id)
{
    return &singleEffData[id-1];
}

static inline s16 SingleEffParticleCreate(ANIMDATA *anim)
{
    s16 modelId = mbParticleCreate(anim, 100);
    mbParticleHookSet(modelId, SingleEffMgCapsuleHook);
    Hu3DModelCameraSet(modelId, HU3D_CAM0);
    Hu3DModelLayerSet(modelId, 5);
    return modelId;
}

static inline s16 SingleEffCapsuleCreate(void)
{
    return SingleEffParticleCreate(singleEffAnim[2]);
}

/* @@PART1@@ */
void mbSingleInit(void)
{
    static int effFile[4] = { 0xA0000, 0x50063, 0x5005E, 0x50066 };
    static int boardNo[4] = { 6, 7, 8, 10 };
    int i;
    int masuNum;
    s16 masuList[8];

    singleMicF = FALSE;
    singleListenerCreateF = FALSE;
    singleListenerOnF = FALSE;
    singleMicContext = -1;
    SingleMicCreate();
    for (i = 0; i < 4; i++) {
        if (boardNo[i] == mbBoardNoGet()) {
            break;
        }
    }
    singleBoard = i;
    if (singleTeamChar < 0) {
        mbSingleTeamCharSet(7);
    }
    if (GwPlayer[GwSystem.turnPlayerNo].charNo == mbSingleTeamCharGet()) {
        if ((u8)GwPlayer[GwSystem.turnPlayerNo].charNo != 7) {
            mbSingleTeamCharSet(7);
        } else {
            mbSingleTeamCharSet(10);
        }
    }
    masuNum = mbMasuTypeListGet(9, masuList);
    if (mbSaveNewF) {
        for (i = 0; i < 4; i++) {
            mbPlayerCoinSet(i, 0);
        }
        mbSingleMgUnlockInit();
        SingleMasuTypeReset();
        SingleMgRecordBackup();
        for (i = 0; i < masuNum; i++) {
            mbMasuCapsuleSet(masuList[i], i);
        }
    }
    for (i = 0; i < masuNum; i++) {
        mbMasuTypeSet(masuList[i], mbMasuCapsuleGet(masuList[i]) + 9);
    }
    if (!_CheckFlag(FLAGNUM(0x10000, 0xE))) {
        if (mbSaveNewF) {
            SingleMasuOrderInit();
        }
        SingleMasuOrderSet();
    }
    for (i = 0; i < 4; i++) {
        singleEffAnim[i] = HuSprAnimRead(HuDataSelHeapReadNum(mbBoardDataNumGet(effFile[i]), HU_MEMNUM_OVL, HEAP_MODEL));
        HuSprAnimLock(singleEffAnim[i]);
    }
    SingleEffInit();
    SingleMgSaveInit();
    singleEndF = FALSE;
    singleCancelF = FALSE;
    HuDataDirClose(0xA0000);
}

void mbSingleClose(void)
{
    int playerNo = GwSystem.turnPlayerNo;
    int i;

    SingleEffClose();
    for (i = 0; i < 4; i++) {
        HuSprAnimKill(singleEffAnim[i]);
        singleEffAnim[i] = NULL;
    }
    if (GwSystem.turnNo > GwSystem.turnMax) {
        singleEndF = TRUE;
    }
    if (singleEndF) {
        if (!singleCancelF) {
            mbSingleSaveFlush(1);
        } else {
            mbSingleSaveFlush(-1);
        }
    }
    SingleMicKill();
}

void mbSingleSaveInit(int teamChar, int mgPack, int comDif)
{
    int i;

    GwSystem.partyF = FALSE;
    GwSystem.tagF = FALSE;
    GwSystem.storyComDif = comDif;
    GwSystem.bonusStarF = FALSE;
    GwSystem.mgPack = mgPack;
    for (i = 0; i < 4; i++) {
        GwPlayer[i].handicap = 0;
    }
    GwSystem.turnMax = 50;
    memset(GwPlayer, 0, sizeof(GwPlayer));
    singleTeamChar = teamChar;
    _ClearFlag(0);
    _ClearFlag(1);
    _ClearFlag(2);
    _SetFlag(FLAGNUM(0x10000, 0x14));
    _ClearFlag(FLAGNUM(0x10000, 0xE));
    _SetFlag(5);
    _ClearFlag(FLAGNUM(0x30000, 0x2));
    _SetFlag(FLAGNUM(0x10000, 0xD));
}

static void SingleMicCreate(void)
{
    if (HuMCMicGet() == 1 && HuMCProbe(1) == 0 && !singleMicF) {
        HuMCInit(0);
        singleMicF = TRUE;
        singleMicContext = HuMCContextCreate("/mic/ctx/030_single_voice");
        if (singleListenerOnF) {
            SingleMicListenerCreate();
        }
    }
}

static void SingleMicKill(void)
{
    if (singleMicF) {
        SingleMicListenerKill();
        if (singleMicContext >= 0) {
            HuMCContextKill(singleMicContext);
            singleMicContext = -1;
        }
        HuMCClose();
        singleMicF = FALSE;
    }
}

static void SingleMicListenerCreate(void)
{
    if (!singleMicF || singleListenerCreateF) {
        return;
    }
    HuMCListenerCreate(singleMicContext, SingleMicListener, 0);
    singleListenerCreateF = TRUE;
}

static void SingleMicListenerKill(void)
{
    if (!singleMicF || !singleListenerCreateF) {
        return;
    }
    HuMCListenerKill();
    singleListenerCreateF = FALSE;
}

static void SingleMasuOrderInit(void)
{
    static int masuNum[3][3] = {
        { 1, 1, 5 },
        { 3, 9, 3 },
        { 11, 4, 4 }
    };
    static int masuType[3] = { 1, 2, 4 };
    int i;
    int num;
    int j;
    int listNo;
    int a;
    int b;
    s16 temp;
    s16 list[256];

    num = mbMasuTypeListGet(1, list);
    num += mbMasuTypeListGet(2, &list[num]);
    num += mbMasuTypeListGet(4, &list[num]);
    for (i = 0; i < 100; i++) {
        a = mbRandMod(num);
        b = mbRandMod(num);
        temp = list[a];
        list[a] = list[b];
        list[b] = temp;
    }
    singleMasuOrderNum = 0;
    listNo = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < masuNum[singleBoard][i]; j++) {
            singleMasuOrder[singleMasuOrderNum].type = masuType[i];
            singleMasuOrder[singleMasuOrderNum].masuId = list[listNo++];
            singleMasuOrderNum++;
        }
    }
}

static void SingleMasuOrderSet(void)
{
    int i;

    for (i = 0; i < singleMasuOrderNum; i++) {
        mbMasuTypeSet(singleMasuOrder[i].masuId, singleMasuOrder[i].type);
    }
}

void mbSingleMgUnlockInit(void)
{
    memset(singleMgUnlock, 0, sizeof(singleMgUnlock));
}

void mbSingleMgUnlockWrite(void)
{
    int i;
    int j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 32; j++) {
            if ((1 << j) & singleMgUnlock[i]) {
                GWMgUnlockSet((i * 32) + 0x259 + j);
            }
        }
    }
}

void mbSingleMgUnlockSet(int mgNo)
{
    mgNo -= 0x259;
    singleMgUnlock[mgNo >> 5] |= (1 << (mgNo % 32));
}

void mbSingleMgUnlockReset(int mgNo)
{
    mgNo -= 0x259;
    singleMgUnlock[mgNo >> 5] &= ~(1 << (mgNo % 32));
}

BOOL mbSingleMgUnlockGet(int mgNo)
{
    mgNo -= 0x259;
    return ((1 << (mgNo % 32)) & singleMgUnlock[mgNo >> 5]) != 0;
}

BOOL mbSingleMgUnlockCheckAny(void)
{
    int i;

    for (i = 0; i < 4; i++) {
        if (singleMgUnlock[i] != 0) {
            return TRUE;
        }
    }
    return FALSE;
}

int mbSingleMgUnlockNumGet(void)
{
    int i;
    int j;
    int num;

    num = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 32; j++) {
            if ((1 << j) & singleMgUnlock[i]) {
                num++;
            }
        }
    }
    return num;
}

static void SingleMasuTypeReset(void)
{
    masuTypeNum = 0;
    memset(masuType, 0, sizeof(masuType));
}

static inline HU3D_MODELID SingleParticleCreate(ANIMDATA *anim, s16 maxCnt, void *hook)
{
    HU3D_MODELID modelId = mbParticleCreate(anim, maxCnt);
    mbParticleHookSet(modelId, hook);
    Hu3DModelCameraSet(modelId, HU3D_CAM0);
    Hu3DModelLayerSet(modelId, 5);
    return modelId;
}

static inline MBPARTICLE *SingleParticleGet(s16 modelId)
{
    return Hu3DData[modelId].hookData;
}

static void SingleEffInit(void)
{
    SINGLEEFF *effP;
    MBPARTICLE *particleP;
    int i;

    effP = singleEffData;
    memset(effP, 0, sizeof(singleEffData));
    for (i = 0; i < SINGLE_EFF_MAX; i++, effP++) {
        effP->mdlId = SingleParticleCreate(singleEffAnim[0], 1, SingleEffMgMasuHook);
        mbParticleAttrSet(effP->mdlId, MB_PARTICLE_ATTR_3D);
        Hu3DModelAttrSet(effP->mdlId, 0x200001);
        particleP = SingleParticleGet(effP->mdlId);
        particleP->work = effP;
        effP->subMdlId[0] = SingleParticleCreate(singleEffAnim[1], 20, SingleEffMgHook);
        mbParticleAttrSet(effP->subMdlId[0], MB_PARTICLE_ATTR_ANIM);
        Hu3DModelAttrSet(effP->subMdlId[0], HU3D_ATTR_DISPOFF);
        mbParticleBlendModeSet((s16)effP->subMdlId[0], MB_PARTICLE_BLEND_ADDCOL);
        particleP = SingleParticleGet(effP->subMdlId[0]);
        particleP->work = effP;
        effP->subMdlId[1] = SingleParticleCreate(singleEffAnim[1], 100, SingleEffMgExplodeHook);
        Hu3DModelAttrSet(effP->subMdlId[1], HU3D_ATTR_DISPOFF);
        mbParticleBlendModeSet((s16)effP->subMdlId[1], MB_PARTICLE_BLEND_ADDCOL);
        effP->obj = omAddObjEx(mbObjMan, 0x200C, 0, 0, -1, SingleEffOMExec);
        effP->obj->work[0] = i;
        effP->seNo = -1;
    }
}

static void SingleEffClose(void)
{
    int i;
    SINGLEEFF *effP;

    effP = singleEffData;
    for (i = 0; i < SINGLE_EFF_MAX; i++, effP++) {
        if (effP->useF) {
            SingleEffKill(i + 1);
        }
    }
}

static s16 SingleEffCreate(HuVecF *pos, int masuType)
{
    SINGLEEFF *effP;
    int i;
    MBPARTICLE *particleP;

    effP = singleEffData;
    for (i = 0; i < SINGLE_EFF_MAX; i++, effP++) {
        if (!effP->useF) {
            break;
        }
    }
    effP->id = i + 1;
    effP->useF = TRUE;
    effP->dispF = TRUE;
    effP->subDispF = TRUE;
    effP->masuType = masuType;
    effP->pos = *pos;
    effP->rot.x = effP->rot.y = effP->rot.z = 0.0f;
    effP->scale.x = effP->scale.y = effP->scale.z = 1.0f;
    effP->alpha = 1.0f;
    effP->tevLevel = 0.0f;
    effP->bobScale = 1.0f;
    effP->emitF = TRUE;
    Hu3DModelAttrReset(effP->mdlId, HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(effP->subMdlId[0], HU3D_ATTR_DISPOFF);
    particleP = SingleParticleGet(effP->subMdlId[0]);
    particleP->unk00 = 0;
    Hu3DModelCameraSet(effP->mdlId, HU3D_CAM0);
    Hu3DModelLayerSet(effP->mdlId, 5);
    for (i = 0; i < 2; i++) {
        Hu3DModelCameraSet(effP->subMdlId[i], HU3D_CAM0);
        Hu3DModelLayerSet(effP->subMdlId[i], 5);
    }
    effP->seNo = -1;
    return effP->id;
}

static void SingleEffKill(s16 id)
{
    SINGLEEFF *effP;
    int i;

    effP = &singleEffData[id-1];
    effP->useF = FALSE;
    effP->dispF = FALSE;
    Hu3DModelAttrSet(effP->mdlId, HU3D_ATTR_DISPOFF);
    for (i = 0; i < 2; i++) {
        Hu3DModelAttrSet(effP->subMdlId[i], HU3D_ATTR_DISPOFF);
    }
}

static void SingleEffMgMasuHook(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx)
{
    static s16 masuPatNo[12] = { -1, 0, 1, 2, 6, 7, 3, 5, 8, 9, 10, 11 };
    SINGLEEFF *effP;
    MBPARTICLEDATA *dataP;
    HuVecF rot;
    Mtx rotMtx;

    effP = particleP->work;
    if (particleP->unk00 == 0) {
        dataP = particleP->data;
        dataP->pos.x = dataP->pos.y = dataP->pos.z = 0.0f;
        dataP->scale = 120.0f;
        dataP->time = 0;
        particleP->tevColorIn[0] = 8;
        particleP->tevColorIn[1] = 12;
        particleP->tevColorIn[2] = 2;
        particleP->tevColorIn[3] = 15;
        particleP->tevColor1.r = particleP->tevColor1.g = particleP->tevColor1.b = particleP->tevColor1.a = 255;
        particleP->unk00 = 1;
    }
    dataP = particleP->data;
    mbCameraRotGet(&rot);
    mbMtxRot(rotMtx, rot.x, rot.y, rot.z);
    mtxScaleCat(rotMtx, effP->scale.x, effP->scale.y, effP->scale.z);
    mtxTransCat(rotMtx, effP->pos.x, effP->pos.y, effP->pos.z);
    PSMTXConcat(mtx, rotMtx, mtx);
    dataP->rot = effP->rot;
    dataP->animNo = masuPatNo[effP->masuType];
    dataP->color.a = 255.0f * effP->alpha;
    particleP->tevColor1.r = particleP->tevColor1.g = particleP->tevColor1.b = 255.0f * effP->tevLevel;
    if (!Hu3DPauseF) {
        dataP->pos.y = effP->bobScale * (100.0f * (0.1f * mbSinDeg(4.0f * dataP->time)));
        dataP->time++;
    }
}

static void SingleEffMgHook(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx)
{
    float angle;
    float speed;
    GXColor color = { 255, 255, 192, 192 };
    SINGLEEFF *effP;
    MBPARTICLEDATA *dataP;
    int i;
    int num;
    float alpha;

    effP = particleP->work;
    if (particleP->unk00 == 0) {
        dataP = particleP->data;
        for (i = 0; i < particleP->num; i++, dataP++) {
            angle = 360.0f * frandf();
            speed = 200.0f + (100.0f * (2.0f * frandf()));
            dataP->vel.x = dataP->vel.y = dataP->vel.z = 0.0f;
            dataP->scale = 0.0f;
            dataP->rot.z = mbRandMod(360);
            dataP->color = color;
            dataP->time = mbRandMod(30) + 1;
            dataP->aliveF = 30;
        }
        particleP->unk00 = 1;
    }
    dataP = particleP->data;
    num = 0;
    for (i = 0; i < particleP->num; i++, dataP++) {
        if (dataP->time) {
            if (--dataP->time == 0) {
                dataP->pos.x = effP->pos.x + (effP->scale.x * (50.0f - (100.0f * frandf())));
                dataP->pos.y = effP->pos.y + (effP->scale.y * (50.0f - (100.0f * frandf())));
                dataP->pos.z = effP->pos.z + (effP->scale.z * (50.0f - (100.0f * frandf())));
                dataP->scale = 30.0f + (10.0f * frandf());
            }
            num++;
        } else if (dataP->aliveF) {
            dataP->vel.y += -16.333334f;
            dataP->pos.x += 0.016666668f * dataP->vel.x;
            dataP->pos.y += 0.016666668f * dataP->vel.y;
            dataP->pos.z += 0.016666668f * dataP->vel.z;
            dataP->rot.z += 10.0f;
            dataP->aliveF--;
            if (dataP->aliveF == 0) {
                if (effP->emitF) {
                    dataP->pos.x = effP->pos.x + (effP->scale.x * (50.0f - (100.0f * frandf())));
                    dataP->pos.y = effP->pos.y + (effP->scale.y * (50.0f - (100.0f * frandf())));
                    dataP->pos.z = effP->pos.z + (effP->scale.z * (50.0f - (100.0f * frandf())));
                    dataP->vel.x = dataP->vel.y = dataP->vel.z = 0.0f;
                    dataP->aliveF = 30;
                    dataP->color.a = 255;
                }
            } else if (dataP->aliveF < 20) {
                alpha = dataP->aliveF / 20.0f;
                dataP->color.a = 255.0f * alpha;
            }
            num++;
        }
    }
    if (num == 0) {
        particleP->unk00 = 0;
        Hu3DModelAttrSet(particleP->modelId, HU3D_ATTR_DISPOFF);
        effP->subDispF = FALSE;
    }
}

static void SingleEffMgExplodeHook(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx)
{
    GXColor color = { 255, 255, 192, 192 };
    MBPARTICLEDATA *dataP;
    int i;
    int num;
    float angle;
    float speed;
    float alpha;

    if (particleP->unk00 == 0) {
        dataP = particleP->data;
        for (i = 0; i < particleP->num; i++, dataP++) {
            angle = 360.0f * frandf();
            speed = 1000.0f + (100.0f * (20.0f * frandf()));
            dataP->pos.x = dataP->pos.y = dataP->pos.z = 0.0f;
            dataP->vel.x = speed * mbCosDeg(angle);
            dataP->vel.y = speed * mbSinDeg(angle);
            dataP->vel.z = 0.0f;
            dataP->scale = 0.0f;
            dataP->rot.z = mbRandMod(360);
            dataP->color = color;
            dataP->time = mbRandMod(6) + 1;
            dataP->aliveF = 30.0f + (30.0f * frandf());
        }
        particleP->unk00 = 1;
    }
    dataP = particleP->data;
    num = 0;
    for (i = 0; i < particleP->num; i++, dataP++) {
        if (dataP->time) {
            if (--dataP->time == 0) {
                dataP->scale = 100.0f + (10.0f * frandf());
            }
            num++;
        } else if (dataP->aliveF) {
            dataP->pos.x += 0.016666668f * dataP->vel.x;
            dataP->pos.y += 0.016666668f * dataP->vel.y;
            dataP->pos.z += 0.016666668f * dataP->vel.z;
            dataP->aliveF--;
            if (dataP->aliveF < 20) {
                alpha = dataP->aliveF / 20.0f;
                dataP->color.a = 255.0f * alpha;
            }
            num++;
        }
    }
    if (num == 0) {
        particleP->unk00 = 0;
        Hu3DModelAttrSet(particleP->modelId, HU3D_ATTR_DISPOFF);
    }
}

static void SingleEffMgCapsuleHook(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx)
{
    GXColor color = { 255, 255, 255, 255 };
    MBPARTICLEDATA *dataP;
    int i;
    int num;
    float angle;
    float speed;
    float alpha;

    if (particleP->unk00 == 0) {
        particleP->attr |= MB_PARTICLE_ATTR_ANIM;
        dataP = particleP->data;
        for (i = 0; i < particleP->num; i++, dataP++) {
            angle = 360.0f * frandf();
            speed = 100.0f + (100.0f * frandf());
            dataP->pos.x = dataP->pos.y = dataP->pos.z = 0.0f;
            dataP->vel.x = speed * mbCosDeg(angle);
            dataP->vel.y = speed * mbSinDeg(angle);
            dataP->vel.z = 0.0f;
            dataP->scale = 100.0f + (10.0f * frandf());
            dataP->animTime = 0.0f;
            dataP->animSpeed = 0.5f;
            dataP->animNo = 0;
            dataP->dispF = FALSE;
            dataP->pauseF = TRUE;
            dataP->time = mbRandMod(6) + 1;
            dataP->aliveF = 30.0f + (30.0f * frandf());
        }
        particleP->unk00 = 1;
    }
    dataP = particleP->data;
    num = 0;
    for (i = 0; i < particleP->num; i++, dataP++) {
        if (dataP->time) {
            if (--dataP->time == 0) {
                dataP->dispF = TRUE;
                dataP->pauseF = FALSE;
            }
            num++;
        } else if (dataP->pauseF == FALSE) {
            dataP->pos.x += 0.016666668f * dataP->vel.x;
            dataP->pos.y += 0.016666668f * dataP->vel.y;
            dataP->pos.z += 0.016666668f * dataP->vel.z;
            dataP->aliveF--;
            if (dataP->aliveF < 20) {
                alpha = dataP->aliveF / 20.0f;
                dataP->color.a = 255.0f * alpha;
            }
            num++;
        }
    }
    if (num == 0) {
        particleP->unk00 = 0;
        Hu3DModelAttrSet(particleP->modelId, HU3D_ATTR_DISPOFF);
    }
}

static void SingleEffMgFireHook(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx)
{
    MBPARTICLEDATA *dataP;
    int i;
    int num;
    float scale;
    float value;
    float speed;
    HuVecF ofs;
    HuVecF dir;

    if (particleP->unk00 == 0) {
        dataP = particleP->data;
        for (i = 0; i < particleP->num; i++, dataP++) {
            dataP->scale = 0.0f;
            dataP->color.a = 0;
            dataP->time = -(i >> 3);
        }
        particleP->unk00 = 1;
    }
    ofs.x = 0.0f;
    ofs.y = 0.0f;
    ofs.z = 0.0f;
    scale = 1.0f;
    num = 0;
    dataP = particleP->data;
    for (i = 0; i < particleP->num; i++, dataP++) {
        if (!dataP->dispF) {
            continue;
        }
        if (dataP->time < 0) {
            dataP->time++;
            num++;
            continue;
        }
        if (dataP->time == 0) {
            dataP->time++;
            dataP->rot.z = 360.0f * frandf();
            dataP->rot.x = 360.0f * frandf();
            dataP->rot.y = 360.0f * frandf();
            dataP->rot.z = 360.0f * frandf();
            dataP->color.r = dataP->color.g = dataP->color.b = 160.0f + (95.0f * frandf());
            value = 360.0f * frandf();
            dataP->pos.x = mbSinDeg(value);
            dataP->pos.z = mbCosDeg(value);
            value = 1.0f - mbSinDeg(90.0f * frandf());
            if (((0.000030517578f * (u16)frand()) - 1.0f) < 0.0f) {
                value *= -1.0f;
            }
            dataP->pos.y = value;
            value = 1.0f - (value * value);
            if (value <= 0.0f) {
                value = 0.0f;
            } else {
                value = sqrtf(1.0f + (value * value));
            }
            dataP->pos.x *= value;
            dataP->pos.z *= value;
            value = scale * (2.5000002f + (100.0f * (0.016666668f * (1.5f * frandf()))));
            dataP->vel.x = dataP->pos.x * value;
            dataP->vel.z = dataP->pos.z * value;
            dataP->vel.y = dataP->pos.y * value;
            dataP->aliveF = 20;
            PSVECScale(&dataP->pos, &dataP->pos, 30.000002f * scale);
            if (dataP->pos.y > 0.0f) {
                dataP->pos.y *= 1.0f + (3.0f * frandf());
            }
            PSVECAdd(&ofs, &dataP->pos, &dataP->pos);
            dataP->accel.x = 100.0f + (80.0f * frandf());
            dataP->accel.y = scale;
            dataP->color.a = 0;
            dataP->scale = scale * (30.0f + (30.0f * frandf()));
            dataP->animBank = (s32)(1000.0f * frandf()) & 3;
            value = 360.0f * frandf();
            speed = scale * (3.3333335f + (100.0f * (0.016666668f * (2.0f * frandf()))));
            dataP->speedDecay += speed * mbSinDeg(value);
            dataP->scaleBase += speed * mbCosDeg(value);
        }
        PSVECAdd(&dataP->pos, &dataP->vel, &dataP->pos);
        dataP->vel.y += 0.8166668f * dataP->accel.y;
        dataP->vel.x *= 0.95f;
        dataP->vel.z *= 0.95f;
        if (dataP->pos.y > ofs.y + (50.0f * dataP->accel.y)) {
            PSVECSubtract(&ofs, &dataP->pos, &dir);
            dataP->vel.x += 0.7f * (0.016666668f * dir.x);
            dataP->vel.z += 0.7f * (0.016666668f * dir.z);
            if (dataP->pos.y > ofs.y + (90.0f * dataP->accel.y)) {
                dataP->vel.x += 1.3f * (0.016666668f * dataP->speedDecay);
                dataP->vel.z += 1.3f * (0.016666668f * dataP->scaleBase);
            }
        }
        dataP->aliveF--;
        if (dataP->aliveF <= 0) {
            if (particleP->count < 30) {
                dataP->time = 0;
                dataP->scale = 0.0f;
                dataP->color.a = 0;
            } else {
                dataP->dispF = FALSE;
            }
        } else if (dataP->aliveF < 8) {
            if (dataP->color.a >= 30) {
                dataP->color.a -= 30;
            }
            dataP->scale += 5.0f;
        } else {
            dataP->color.a = dataP->color.a + (0.3f * (dataP->accel.x - dataP->color.a));
            dataP->scale += 2.0f;
        }
        num++;
    }
    if (num == 0) {
        particleP->unk00 = 0;
        Hu3DModelAttrSet(particleP->modelId, HU3D_ATTR_DISPOFF);
    }
}

static void SingleEffMgFire2Hook(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx)
{
    MBPARTICLEDATA *dataP;
    int i;
    int num;
    float scale;
    float value;
    HuVecF ofs;
    HuVecF dir;

    if (particleP->count == 0) {
        dataP = particleP->data;
        for (i = 0; i < particleP->num; i++, dataP++) {
            dataP->scale = 0.0f;
            dataP->color.a = 0;
            dataP->time = -(i >> 2);
        }
        particleP->count = 1;
        particleP->blendMode = MB_PARTICLE_BLEND_ADDCOL;
    }
    ofs.x = 0.0f;
    ofs.y = 0.0f;
    ofs.z = 0.0f;
    scale = 1.0f;
    num = 0;
    dataP = particleP->data;
    for (i = 0; i < particleP->num; i++, dataP++) {
        if (!dataP->dispF) {
            continue;
        }
        if (dataP->time < 0) {
            dataP->time++;
            num++;
            continue;
        }
        if (dataP->time == 0) {
            dataP->time++;
            dataP->rot.z = 360.0f * frandf();
            dataP->rot.x = 360.0f * frandf();
            dataP->rot.y = 360.0f * frandf();
            dataP->rot.z = 360.0f * frandf();
            value = frandf();
            dataP->color.r = dataP->color.g = dataP->color.b = 255;
            dataP->color.r = 128.0f + (127.0f * frandf());
            value = 360.0f * frandf();
            dataP->pos.x = mbSinDeg(value);
            dataP->pos.z = mbCosDeg(value);
            value = 1.0f - mbSinDeg(90.0f * frandf());
            if (((0.000030517578f * (u16)frand()) - 1.0f) < 0.0f) {
                value *= -1.0f;
            }
            dataP->pos.y = value;
            value = 1.0f - (value * value);
            if (value <= 0.0f) {
                value = 0.0f;
            } else {
                value = sqrtf(1.0f + (value * value));
            }
            dataP->pos.x *= value;
            dataP->pos.z *= value;
            value = scale * (1.3333335f + (100.0f * (0.016666668f * (1.2f * frandf()))));
            dataP->vel.x = dataP->pos.x * value;
            dataP->vel.z = dataP->pos.z * value;
            dataP->vel.y = dataP->pos.y * value;
            dataP->aliveF = 16;
            PSVECScale(&dataP->pos, &dataP->pos, 30.000002f * scale);
            if (dataP->pos.y > 0.0f) {
                dataP->pos.y *= 1.0f + (2.0f * frandf());
            }
            PSVECAdd(&ofs, &dataP->pos, &dataP->pos);
            dataP->accel.x = scale * (50.0f + (70.0f * frandf()));
            dataP->accel.y = scale;
            dataP->color.a = 0;
            dataP->scale = scale * (40.0f + (60.0f * frandf()));
            dataP->animBank = (s32)(1000.0f * frandf()) & 3;
        }
        PSVECAdd(&dataP->pos, &dataP->vel, &dataP->pos);
        dataP->vel.y += 1.088889f * dataP->accel.y;
        dataP->vel.x *= 0.95f;
        dataP->vel.z *= 0.95f;
        if (dataP->pos.y > ofs.y + (50.0f * dataP->accel.y)) {
            PSVECSubtract(&ofs, &dataP->pos, &dir);
            dataP->vel.x += 0.7f * (0.016666668f * dir.x);
            dataP->vel.z += 0.7f * (0.016666668f * dir.z);
        }
        dataP->aliveF--;
        if (dataP->aliveF <= 0) {
            if (particleP->count < 30) {
                dataP->time = 0;
                dataP->scale = 0.0f;
                dataP->color.a = 0;
            } else {
                dataP->dispF = FALSE;
            }
        } else if (dataP->aliveF < 8) {
            if (dataP->color.a >= 30) {
                dataP->color.a -= 30;
            }
            dataP->scale += 5.0f;
        } else {
            dataP->color.a = dataP->color.a + (0.3f * (dataP->accel.x - dataP->color.a));
            dataP->scale += 2.0f;
        }
        num++;
    }
    if (num == 0) {
        particleP->unk00 = 0;
        Hu3DModelAttrSet(particleP->modelId, HU3D_ATTR_DISPOFF);
    }
}

static void SingleEffOMExec(OMOBJ *obj)
{
    SINGLEEFF *effP;
    float t;
    float tInv;
    HuVecF pos2D;
    HuVecF pos3D;

    effP = &singleEffData[obj->work[0]];
    if (mbExitCheck()) {
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    if (!effP->useF) {
        return;
    }
    if (effP->dispF) {
        Hu3DModelAttrReset(effP->mdlId, HU3D_ATTR_DISPOFF);
        if (effP->subDispF) {
            Hu3DModelAttrReset(effP->subMdlId[0], HU3D_ATTR_DISPOFF);
        }
    } else {
        Hu3DModelAttrSet(effP->mdlId, HU3D_ATTR_DISPOFF);
        Hu3DModelAttrSet(effP->subMdlId[0], HU3D_ATTR_DISPOFF);
    }
    switch (effP->mode) {
        case 0:
            break;
        case 1:
            t = effP->time++ / (float)effP->maxTime;
            effP->scale.x = effP->scale.y = t;
            effP->bobScale = t;
            effP->pos.y = effP->basePos.y + (100.0f * (2.0f * mbSinDeg(90.0f * t)));
            if (effP->time > effP->maxTime) {
                effP->mode = 0;
            }
            break;
        case 2:
            t = effP->time++ / (float)effP->maxTime;
            tInv = 1.0f - t;
            effP->rotSpeed += 0.2f;
            effP->scale.x = effP->scale.y = mbCosDeg(90.0f * t);
            effP->bobScale = tInv;
            effP->pos.y = effP->basePos.y + (100.0f * (2.0f * mbSinDeg(90.0f * tInv)));
            if (effP->time > effP->maxTime) {
                effP->mode = 0;
            }
            break;
        case 3:
            t = effP->time++ / (float)effP->maxTime;
            effP->rotSpeed += 0.2f;
            effP->tevLevel = t;
            effP->bobScale = 1.0f - t;
            mbPos3Dto2D(&effP->basePos, &pos2D);
            pos2D.x = 114.0f;
            pos2D.y = 80.0f;
            mbPos2Dto3D(&pos2D, &pos3D);
            effP->pos.x = effP->basePos.x + (t * (pos3D.x - effP->basePos.x));
            effP->pos.y = effP->basePos.y + (t * (pos3D.y - effP->basePos.y));
            effP->pos.z = effP->basePos.z + (t * (pos3D.z - effP->basePos.z));
            if (effP->time == effP->maxTime - 12U) {
                effP->emitF = FALSE;
            }
            if (effP->time > effP->maxTime) {
                SingleEffMgStop(effP->id, 0);
            }
            break;
        case 4:
            t = effP->time++ / (float)effP->maxTime;
            effP->scale.x = effP->scale.y = 1.0f + (4.0f * mbSinDeg(90.0f * t));
            effP->alpha = 1.0f - t;
            if (effP->time > effP->maxTime) {
                effP->mode = 0;
                Hu3DModelAttrSet(effP->mdlId, HU3D_ATTR_DISPOFF);
                mbParticleBlendModeSet((s16)effP->mdlId, MB_PARTICLE_BLEND_NORMAL);
            }
            break;
        case 5:
            t = effP->time++ / (float)effP->maxTime;
            effP->scale.x = effP->scale.y = (1.0f + (1.5f * mbSinDeg(90.0f * t))) + (0.5f * mbSinDeg(1440.0f * t));
            effP->bobScale = 1.0f - t;
            effP->rotSpeed += 0.4f;
            effP->tevLevel = t;
            if (effP->time == effP->maxTime - 12U) {
                effP->emitF = FALSE;
            }
            if (effP->time > effP->maxTime) {
                SingleEffMgStop(effP->id, 1);
                if (effP->seNo >= 0) {
                    mbAudFXStop(effP->seNo);
                    effP->seNo = -1;
                }
            }
            break;
    }
    effP->rot.y += effP->rotSpeed;
}

static void SingleEffMgStop(s16 id, int type)
{
    SINGLEEFF *effP;
    int i;

    effP = &singleEffData[id-1];
    effP->mode = 4;
    effP->rot.y = 0.0f;
    effP->rotSpeed = 0.0f;
    effP->time = 0;
    effP->maxTime = 30;
    Hu3DModelAttrReset(effP->subMdlId[1], HU3D_ATTR_DISPOFF);
    Hu3DModelPosSetV(effP->subMdlId[1], &effP->pos);
    Hu3DModelCameraSet(effP->mdlId, HU3D_CAM1);
    Hu3DModelLayerSet(effP->mdlId, 7);
    for (i = 0; i < 2; i++) {
        Hu3DModelCameraSet(effP->subMdlId[i], HU3D_CAM1);
        Hu3DModelLayerSet(effP->subMdlId[i], 7);
    }
    if (!type) {
        mbAudFXPlay(0x608);
        omVibrate(GwSystem.turnPlayerNo, 20, 7, 3);
    } else {
        mbAudFXPlay(0x60A);
        omVibrate(GwSystem.turnPlayerNo, 20, 20, 0);
    }
}

void mbev_SingleMg(int playerNo, s16 masuId)
{
    int i;
    int type;

    mbCameraPlayerViewSet(playerNo, 0);
    mbCameraMoveWait();
    type = mbMasuTypeGet(masuId);
    mbPlayerRotateStart(playerNo, 0, 15);
    while (!mbPlayerRotateCheck(playerNo)) {
        HuPrcVSleep();
    }
    for (i = 0; i < 4; i++) {
        mgUnlockOld[i] = GwCommon.mgUnlock[i];
    }
    SingleMgRecordPrizeInit();
    switch (type) {
        case 6:
            ev_SingleKoopaMg(playerNo, masuId);
            break;
        case 9:
        case 10:
        case 11:
            ev_SingleMKoopaMg(playerNo, masuId);
            break;
        default:
            ev_SingleMg(playerNo, masuId);
            break;
    }
}

BOOL mbev_SingleMgEnd(int playerNo)
{
    int mgNo = GwSystem.mgNo;

    if (!((1 << (mgNo % 32)) & mgUnlockOld[mgNo >> 5])) {
        GwCommon.mgUnlock[mgNo >> 5] &= ~(1 << (mgNo % 32));
    }
    mbPlayerColSnapSet(TRUE);
    mbSingleCall(8, 0);
    if (_CheckFlag(FLAGNUM(0x10000, 0x2))) {
        ev_SingleMgEnd(playerNo);
        _ClearFlag(FLAGNUM(0x10000, 0x2));
    } else if (_CheckFlag(FLAGNUM(0x10000, 0x3))) {
        ev_SingleKoopaMgEnd(playerNo);
        _ClearFlag(FLAGNUM(0x10000, 0x3));
    } else if (_CheckFlag(FLAGNUM(0x10000, 0x5))) {
        ev_SingleMKoopaMgEnd(playerNo);
        _ClearFlag(FLAGNUM(0x10000, 0x5));
    }
    return TRUE;
}

static void ev_SingleMg(int playerNo, s16 masuId)
{
    SINGLEEFF *effP;
    int type;
    s16 effId;
    int seNo;
    HuVecF effPos;
    HuVecF pos;

    type = mbMasuTypeGet(masuId);
    mbPlayerPosGet(playerNo, &pos);
    effPos.x = pos.x;
    effPos.y = 100.0f + pos.y;
    effPos.z = pos.z;
    effId = SingleEffCreate(&effPos, type);
    effP = &singleEffData[effId-1];
    effP->mode = 1;
    effP->pos = effPos;
    effP->basePos = effP->pos;
    effP->scale.x = effP->scale.y = 0.0f;
    effP->bobScale = 0.0f;
    effP->rotSpeed = 4.0f;
    effP->time = 0;
    effP->maxTime = 60;
    if (type == 7) {
        mgRareSeNo = mbAudFXPlay(0x447);
        mbAudFXPlay(0x448);
    } else {
        seNo = mbAudFXPlay(0x606);
    }
    while (SingleEffGet(effId)->mode) {
        HuPrcVSleep();
    }
    _SetFlag(FLAGNUM(0x10000, 0x2));
    switch (type) {
        case 1:
            mbev_MgCallSingle(0);
            break;
        case 2:
            mbev_MgCallSingle(1);
            break;
        case 4:
            mbev_MgCallSingle(2);
            break;
        case 5:
            mbev_MgCallSingle(3);
            break;
        case 7:
            ev_SingleRareMg(playerNo, effId);
            break;
    }
    SingleEffKill(effId);
    mbAudFXStop(seNo);
}

static inline BOOL SingleMgUnlockCheck(int mgNo)
{
    if (GWMgUnlockGet(mgNo) || mbSingleMgUnlockGet(mgNo)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static inline void SingleEffMgStart(s16 id)
{
    SINGLEEFF *effP = &singleEffData[id-1];
    int i;

    effP->mode = 3;
    effP->basePos = effP->pos;
    effP->time = 0;
    effP->maxTime = 58;
    Hu3DModelCameraSet(effP->mdlId, HU3D_CAM1);
    Hu3DModelLayerSet(effP->mdlId, 7);
    for (i = 0; i < 2; i++) {
        Hu3DModelCameraSet(effP->subMdlId[i], HU3D_CAM1);
        Hu3DModelLayerSet(effP->subMdlId[i], 7);
    }
}

static void ev_SingleMgEnd(int playerNo)
{
    SINGLEEFF *effP;
    SINGLEEFF *loseEffP;
    int mgNo;
    s16 winId;
    int seNo;
    int type;
    s16 effId;
    u32 i;
    BOOL unlockF;
    s16 masuId;
    s16 coin;
    s16 bonus;
    s16 bonus2;
    s16 coin2;
    HuVecF effPos;
    HuVecF pos;

    mgNo = GwSystem.mgNo;
    seNo = mbAudFXPlay(0x606);
    mbAudFXVolSet(seNo, 0);
    if (!mbWipeSpecialStatGet()) {
        mbWipeFadeOut();
    }
    mbStatusDispForceSet(playerNo, TRUE);
    mbCameraMovePlayer(playerNo, NULL, NULL, 1600.0f, -1.0f, -1);
    mbCameraMoveWait();
    masuId = GwPlayer[playerNo].masuId;
    type = mbMasuTypeGet(masuId);
    mbPlayerRotYSet(playerNo, 0.0f);
    mbPlayerPosGet(playerNo, &pos);
    effPos.x = pos.x;
    effPos.y = 300.0f + pos.y;
    effPos.z = pos.z;
    effId = SingleEffCreate(&effPos, type);
    effP = SingleEffGet(effId);
    effP->scale.x = effP->scale.y = effP->scale.z = 1.0f;
    effP->rotSpeed = 4.0f;
    effP->bobScale = 1.0f;
    mbMusBoardPlay();
    mbWipeFadeIn();
    mbPauseDisableSet(FALSE);
    for (i = 0; i < 60; i++) {
        if (i > 21) {
            mbAudFXVolSet(seNo, ((i - 21) * 127) / 39);
        }
        HuPrcVSleep();
    }
    if ((MgDataTbl[mgNo].type == 3 && GwPlayer[playerNo].mgCoinBonus == 0)
        || (MgDataTbl[mgNo].type != 3 && (coin = GwPlayer[playerNo].mgCoin, bonus = GwPlayer[playerNo].mgCoinBonus, bonus + coin > 0))) {
        mbSingleCall(10, MgDataTbl[mgNo].type);
        mbAudFXStop(seNo);
        SingleEffMgStart(effId);
        mbAudFXPlay(0x607);
        HuPrcSleep(60);
        if (!(unlockF = SingleMgUnlockCheck(mgNo + 0x259))) {
            mbSingleMgUnlockSet(mgNo + 0x259);
            GWSingleMgFlagSet(mgNo + 0x259);
            masuType[masuTypeNum++] = (s16)type;
            masuTypeNum = masuTypeNum % 5;
            mbSingleCall(9, mgNo);
        }
        SingleMgRecordPrizeSet();
        while (SingleEffGet(effId)->mode) {
            HuPrcVSleep();
        }
        HuPrcSleep(30);
        mbCameraMovePlayer(playerNo, NULL, NULL, 1800.0f, -1.0f, 60);
        if (!(MgDataTbl[mgNo].flag & 0x40)) {
            mbCoinAddExec(playerNo, 10);
        } else {
            bonus2 = GwPlayer[playerNo].mgCoinBonus;
            coin2 = GwPlayer[playerNo].mgCoin;
            mbCoinAddExec(playerNo, coin2 + bonus2);
        }
        mbPlayerMotionShiftSet(playerNo, 7, 0.0f, 8.0f, HU3D_MOTATTR_NONE);
        mbPlayerWinLoseVoicePlay(playerNo, 7, 0x243);
        if (!unlockF) {
            winId = mbWinCreate(2, 0x2B0000, -1);
            mbWinInsertMesSet(winId, mbPlayerNameMesGet(playerNo), 0);
            if (mgNo != 0xFFFF) {
                mbWinInsertMesSet(winId, MgDataTbl[mgNo].nameMes, 1);
            } else {
                mbWinInsertMesSet(winId, 0x2B0027, 1);
            }
        }
        while (!mbPlayerMotionEndCheck(playerNo)) {
            HuPrcVSleep();
        }
        mbWinWait(winId);
        HuPrcSleep(60);
    } else {
        mbAudFXStop(seNo);
        loseEffP = &singleEffData[effId-1];
        loseEffP->mode = 5;
        loseEffP->basePos = loseEffP->pos;
        loseEffP->time = 0;
        loseEffP->maxTime = 90;
        loseEffP->seNo = mbAudFXPlay(0x609);
        HuPrcSleep(90);
        mbPlayerMotionShiftSet(playerNo, 9, 0.0f, 8.0f, HU3D_MOTATTR_NONE);
        HuPrcSleep(30);
        mbPlayerMotionShiftSet(playerNo, 6, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
        while (SingleEffGet(effId)->mode) {
            HuPrcVSleep();
        }
        HuPrcSleep(120);
    }
    mbWipeFadeOut();
    SingleEffKill(effId);
}

/* @@PART2@@ */
/* @@PART3@@ */
/* @@END@@ */
