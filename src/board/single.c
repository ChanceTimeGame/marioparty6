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
static void ev_SingleKoopaMg(int playerNo, s16 masuId);
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

static inline BOOL SingleMgUnlockCheck(int mgNo)
{
    if (GWMgUnlockGet(mgNo) || mbSingleMgUnlockGet(mgNo)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/* Minigames still locked in story mode, of one type (type < 0: any). */


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
static inline u32 SingleMgUnlockBitGet(int mgNo)
{
    mgNo -= 0x259;
    return singleMgUnlock[mgNo >> 5] & (1 << (mgNo % 32));
}

static inline int SingleMgFlagListGet(u8 *list)
{
    int num = 0;
    int i;
    for(i=0; MgDataTbl[i].ovl != 0xFFFF; i++) {
        if((MgDataTbl[i].flag & 0x80) && MgDataTbl[i].nameMes != MESSNUM(5, 0x4C)) {
            list[num++] = i;
        }
    }
    return num;
}

static inline int SingleMgLockNumGet(int type, u8 *list)
{
    int num = 0;
    int i;
    for(i=0; MgDataTbl[i].ovl != 0xFFFF; i++) {
        if((type < 0 || type == MgDataTbl[i].type)
            && MgDataTbl[i].type != 4 && MgDataTbl[i].type != 7
            && ((MgDataTbl[i].flag & 0x80) || mbMgCallSingleOnCheck(MgDataTbl[i].ovl))
            && MgDataTbl[i].nameMes != MESSNUM(5, 0x4C)
            && !SingleMgUnlockCheck(i+0x259)) {
            if(list) {
                list[num] = i;
            }
            num++;
        }
    }
    return num;
}

static inline int SingleMgUnlockListGet(u8 *list)
{
    int i, j;
    int num = 0;
    for(i=0; i<4; i++) {
        for(j=0; j<32; j++) {
            if(singleMgUnlock[i] & (1 << j)) {
                if(list) {
                    list[num] = (i*32)+j;
                }
                num++;
            }
        }
    }
    return num;
}

static inline float SingleFAbs(register float x)
{
    asm {
        fabs x, x
    }
    return x;
}

static inline void SingleMasuTypePush(s16 type)
{
    masuType[masuTypeNum++] = type;
    masuTypeNum = masuTypeNum % 5;
}

static void ev_SingleRareMg(int playerNo, s16 effNo)
{
    static const HuVecF cameraOfs = { 0, 100, 0 };
    static const u32 nameMes[] = { MESSNUM(5, 0x51), MESSNUM(5, 0x4D), MESSNUM(5, 0x4E) };
    u8 mgList[132];
    HuVecF effPos;
    HuVecF pos;
    HuVecF rot;
    int jingleNo;
    int i;
    int mgNo;
    int num;
    s16 winId;
    SINGLEEFF *eff;
    float t;
    float weight;
    float zoomNew;
    float zoom;

    mbPauseDisableSet(TRUE);
    for(i=0; MgDataTbl[i].ovl != 0xFFFF; i++) {
        if(MgDataTbl[i].nameMes == nameMes[singleBoard]) {
            break;
        }
    }
    mgNo = i;
    _SetFlag(0x10022);
    mbMusFadeOutSpeed(0, 1000);
    mbPlayerMotionSet(playerNo, 11, 0);
    mbPlayerPosGet(playerNo, &pos);
    i = 0;
    do {
        if(i++ == 12U) {
            effPos = pos;
            effPos.y += 100.0f;
            eff = &singleEffData[effNo-1];
            eff->mode = 2;
            eff->pos = effPos;
            eff->basePos = eff->pos;
            eff->emitF = 0;
            eff->time = 0;
            eff->maxTime = 60;
        }
        if(i == 30) {
            if(mgRareSeNo >= 0) {
                mbAudFXStop(mgRareSeNo);
            }
            mbAudFXPlay(0x449);
        }
        HuPrcVSleep();
    } while(!mbPlayerMotionEndCheck(playerNo));
    mbPlayerMotIdleSet(playerNo);
    omVibrate(playerNo, 20, 7, 3);
    jingleNo = mbMusJinglePlay(0x27);
    mbPlayerMotionShiftSet(playerNo, 7, 0, 8, 0);
    mbPlayerWinLoseVoicePlay(playerNo, 7, 0x23D);
    mbCameraRotGet(&rot);
    rot.x -= 15.0f;
    mbCameraMovePlayer(playerNo, &rot, &cameraOfs, 1600, -1, 102);
    mbCameraMoveWait();
    zoom = mbCameraZoomGet();
    mbCameraFocusObjSet(-1);
    for(i=0; i<30U; i++) {
        t = i/30.0f;
        weight = 0.5*(1.0+HuSin(720.0f*t));
        zoomNew = zoom+(-500.0f*weight);
        mbCameraZoomSet(zoomNew);
        HuPrcVSleep();
    }
    if(!GWMgUnlockGet(mgNo+0x259)) {
        winId = mbWinCreate(2, MESSNUM(0x2B, 0), -1);
        mbWinInsertMesSet(winId, mbPlayerNameMesGet(playerNo), 0);
        if(mgNo != 0xFFFF) {
            mbWinInsertMesSet(winId, MgDataTbl[mgNo].nameMes, 1);
        } else {
            mbWinInsertMesSet(winId, MESSNUM(0x2B, 0x27), 1);
        }
        mbSingleMgUnlockSet(mgNo+0x259);
        GWSingleMgFlagSet(mgNo+0x259);
        SingleMasuTypePush(7);
        GWSinglePrizeFlagSet(8);
        num = SingleMgFlagListGet(mgList);
        for(i=0; i<num; i++) {
            if(!SingleMgUnlockCheck(mgList[i]+0x259)) {
                break;
            }
        }
        if(i >= num) {
            GWSinglePrizeFlagSet(0x2E);
        }
        num = SingleMgLockNumGet(-1, NULL);
        if(num == 0) {
            GWSinglePrizeFlagSet(0x2F);
        }
    } else {
        winId = mbWinCreate(2, MESSNUM(0x2B, 1), -1);
        mbWinInsertMesSet(winId, mbPlayerNameMesGet(playerNo), 0);
        mbCoinAddProcExec(playerNo, 50, 1, TRUE);
    }
    mbMusJingleWait(jingleNo);
    mbWinWait(winId);
    {
        GAMEMESID gameMesId = GameMesCreate(6, 1);
        while(GameMesStatGet(gameMesId)) {
            HuPrcVSleep();
        }
    }
    mbWipeSpecialCreate(1, 6, 90);
    mbWipeSpecialWait();
    mbWipeFadeOutTime(1);
    mbWipeSpecialKill();
    GwSystem.turnNo++;
    mbSingleReturn();
}

static void ev_SingleKoopaMg(int playerNo, s16 spaceId)
{
    static int guideMot[] = { 0xE0001, 0xE0005, 0xE0003, 0xE0007, 0xE0004, 0xE000A, HU_DATANUM_NONE };
    static HuVecF defCameraRot = { -15, 0, 0 };
    static HuVecF cameraPos = { 0, 100, 100 };
    static HuVecF cameraRot = { -35, 0, 0 };
    static int mgTypeTbl[3][2] = { { 1, 0 }, { 2, 1 }, { 4, 2 } };
    HuVecF pos;
    HuVecF objPos;
    HuVecF pos3D;
    HuVecF masuPos;
    int j;
    BOOL haveF;
    int capNo;
    s16 motId[2];
    s16 espId[2];
    MBOBJID capObj = 0;
    s16 particleId[2] = { -1, -1 };
    float scale;
    float t;
    float t2;
    s16 masuId;
    int i;
    s16 winId;
    MBOBJID guideObj;

    guideObj = mbObjCreate(0xE0000, guideMot, FALSE);
    mbObjDispSet(guideObj, FALSE);
    mbPlayerRotateStart(playerNo, 0, 15);
    motId[0] = mbPlayerMotionCreate(playerNo, 0x930017);
    motId[1] = mbPlayerMotionCreate(playerNo, 0x930019);
    espId[0] = espEntry(0xE0024, 100, 0);
    espPosSet(espId[0], 288, 240);
    espScaleSet(espId[0], 4, 4);
    espTPLvlSet(espId[0], 0);
    espColorSet(espId[0], 255, 0, 0);
    espDispOff(espId[0]);
    espAttrSet(espId[0], 8);
    espDrawNoSet(espId[0], 0);
    espDispOff(espId[0]);
    mbMusFadeOutSpeed(0, 1000);
    while(mbMusCheck(0)) {
        HuPrcVSleep();
    }
    while(!mbPlayerRotateCheck(playerNo)) {
        HuPrcVSleep();
    }
    mbPlayerMotionShiftSet(playerNo, motId[0], 0, 8, 0);
    mbCameraPlayerViewSet(playerNo, 0);
    mbEffFadeCreate(30, 160);
    espDispOn(espId[0]);
    for(i=0; i<=180U; i++) {
        t = i/180.0f;
        if(i == 30) {
            mbAudFXPlay(0x453);
        }
        if(i == 18 || i == 90 || i == 162) {
            omVibrate(playerNo, 20, 7, 3);
        }
        espTPLvlSet(espId[0], SingleFAbs(mbSinDeg(450.0f*t)));
        HuPrcVSleep();
    }
    mbWipeCreate(2, 0x83, 60);
    mbWipeSpecialWait();
    espDispOff(espId[0]);
    mbEffFadeOutSet(30);
    masuId = mbMasuFind_AttrIdGet(-1, 0x8000);
    mbMasuPosGet(masuId, &masuPos);
    pos = masuPos;
    pos.y += 150.0f;
    mbCameraMovePos(&pos, &defCameraRot, NULL, 500, -1, -1);
    mbCameraMoveWait();
    mbObjPosSetV(guideObj, &masuPos);
    mbObjDispSet(guideObj, TRUE);
    mbObjMotionSet(guideObj, 1, 0x40000001);
    mbPlayerColSnapPlayerSet(playerNo, FALSE);
    mbPlayerPosSet(playerNo, masuPos.x, masuPos.y, 300.0f+masuPos.z);
    mbPlayerMotionSet(playerNo, 1, 0x40000001);
    mbPlayerRotYSet(playerNo, 180);
    mbWipeFadeIn();
    HuPrcSleep(30);
    mbCameraMoveMasu(masuId, &cameraRot, &cameraPos, 1600, -1, 60);
    mbMusPlay(0, 0x1B, 127, 0);
    mbAudFXDelaySet(30);
    mbAudFXPlay(0x3CB);
    mbObjMotionShiftSet(guideObj, 3, 0, 8, 0x40000001);
    mbCameraMoveWait();
    winId = mbWinCreate(2, MESSNUM(0x2B, 2), 13);
    mbWinWait(winId);
    mbObjMotionShiftSet(guideObj, 1, 0, 8, 0x40000001);
    winId = mbWinCreate(2, MESSNUM(0x2B, 3), 13);
    mbWinWait(winId);
    mbAudFXDelaySet(30);
    mbAudFXPlay(0x3CD);
    mbObjMotionShiftSet(guideObj, 5, 0, 8, 0);
    while(!mbObjMotionEndCheck(guideObj)) {
        HuPrcVSleep();
    }
    if(!SingleMgUnlockListGet(NULL) && mbPlayerCoinGet(playerNo) == 0 && mbPlayerCapsuleNumGet(playerNo) == 0) {
        haveF = FALSE;
    } else {
        haveF = TRUE;
    }
    espId[1] = espEntry(mbBoardDataNumGet(0x5008E), 100, haveF^1);
    espDrawNoSet(espId[1], 32);
    for(i=1; i<60U; i++) {
        t = i/60.0f;
        if(i == 27U) {
            mbAudFXPlay(0x45D);
        }
        scale = mbSinDeg(90.0f*t);
        espPosSet(espId[1], 288, (240.0f-(250.0f*mbSinDeg(180.0f*t)))+(50.0f*mbCosDeg(180.0f*t)));
        espScaleSet(espId[1], scale, scale);
        espZRotSet(espId[1], 3.0f*(360.0f*t));
        HuPrcVSleep();
    }
    omVibrate(playerNo, 20, 7, 3);
    for(i=1; i<60U; i++) {
        t = i/60.0f;
        scale = 1.0f+(0.2f*mbSinDeg(720.0f*t));
        espPosSet(espId[1], 288, 240);
        espScaleSet(espId[1], scale, scale);
        espZRotSet(espId[1], 0);
        HuPrcVSleep();
    }
    for(i=1; i<18U; i++) {
        t = i/18.0f;
        scale = 1.0f+(5.0f*mbSinDeg(90.0f*t));
        espPosSet(espId[1], 288, 240);
        espScaleSet(espId[1], scale, scale);
        espTPLvlSet(espId[1], 1.0f-mbSinDeg(90.0f*t));
        HuPrcVSleep();
    }
    espDispOff(espId[1]);
    mbAudFXDelaySet(30);
    mbAudFXPlay(0x3CB);
    mbObjMotionShiftSet(guideObj, 3, 0, 8, 0x40000001);
    if(!haveF) {
        winId = mbWinCreate(2, MESSNUM(0x2B, 4), 13);
        mbWinInsertMesSet(winId, mbPlayerNameMesGet(playerNo), 0);
        mbWinWait(winId);
        capNo = (mbRandMod(100) < 50) ? 0 : 1;
        winId = mbWinCreate(2, MESSNUM(0x2B, 5), 13);
        mbWinInsertMesSet(winId, capNo+MESSNUM(0x2B, 0x27), 0);
        mbWinWait(winId);
        mbAudFXDelaySet(30);
        mbAudFXPlay(0x3CD);
        mbObjMotionShiftSet(guideObj, 5, 0, 8, 0);
        while(!mbObjMotionEndCheck(guideObj)) {
            HuPrcVSleep();
        }
        capObj = mbCapObjCreate(mgKoopaCapsuleTbl[capNo], 0);
        mbObjDispSet(capObj, FALSE);
        mbObjCameraSet(capObj, HU3D_CAM1);
        mbObjLayerSet(capObj, 7);
        mbObjAttrSet(capObj, 0x40000001);
        mbObjMotionSpeedSet(capObj, 0);
        mbObjPosGet(guideObj, &objPos);
        objPos.y += 200.0f;
        objPos.z += 200.0f;
        particleId[0] = SingleParticleCreate(singleEffAnim[2], 100, SingleEffMgCapsuleHook);
        pos = objPos;
        pos.y += 80.0f;
        pos.z += 100.0f;
        Hu3DModelPosSetV(particleId[0], &pos);
        Hu3DModelCameraSet(particleId[0], HU3D_CAM1);
        Hu3DModelLayerSet(particleId[0], 7);
        Hu3DModelAttrSet(particleId[0], HU3D_ATTR_DISPOFF);
        particleId[1] = SingleParticleCreate(singleEffAnim[1], 100, SingleEffMgExplodeHook);
        Hu3DModelPosSetV(particleId[1], &objPos);
        mbParticleBlendModeSet((int)particleId[1], 1);
        Hu3DModelCameraSet(particleId[1], HU3D_CAM1);
        Hu3DModelLayerSet(particleId[1], 7);
        Hu3DModelAttrSet(particleId[1], HU3D_ATTR_DISPOFF);
        mbAudFXPlay(0x427);
        for(i=0; i<3; i++) {
            Hu3DModelAttrReset(particleId[0], HU3D_ATTR_DISPOFF);
            mbAudFXPlay(0x60B);
            HuPrcSleep(12);
            mbObjPosSetV(capObj, &objPos);
            mbObjScaleSet(capObj, 1, 1, 1);
            mbObjDispSet(capObj, TRUE);
            HuPrcSleep(30);
            for(j=0; j<=60U; j++) {
                t = j/60.0f;
                t2 = 1.0f-t;
                scale = 1080.0f*t;
                mbPos3Dto2D(&objPos, &pos);
                pos.x = 114.0f+(128.0f*t2);
                pos.y = 80.0f;
                pos.x += t2*(128.0f*-mbCosDeg(-scale));
                pos.y += t2*(128.0f*mbSinDeg(-scale));
                mbPos2Dto3D(&pos, &pos3D);
                scale = mbSinDeg(90.0f*t);
                pos.x = objPos.x+(scale*(pos3D.x-objPos.x));
                pos.y = objPos.y+(scale*(pos3D.y-objPos.y));
                pos.z = objPos.z+(scale*(pos3D.z-objPos.z));
                mbObjPosSetV(capObj, &pos);
                scale = 0.2f+(0.8f*(1.0f-t));
                mbObjScaleSet(capObj, scale, scale, scale);
                HuPrcVSleep();
            }
            Hu3DModelPosSetV(particleId[1], &pos);
            Hu3DModelAttrReset(particleId[1], HU3D_ATTR_DISPOFF);
            mbObjDispSet(capObj, FALSE);
            mbPlayerCapsuleAdd(playerNo, mgKoopaCapsuleTbl[capNo]);
            omVibrate(playerNo, 20, 7, 3);
            HuPrcSleep(30);
        }
        mbObjDispSet(capObj, FALSE);
        mbAudFXDelaySet(30);
        mbAudFXPlay(0x3CB);
        mbObjMotionShiftSet(guideObj, 3, 0, 8, 0x40000001);
        HuPrcSleep(60);
        winId = mbWinCreate(2, MESSNUM(0x2B, 6), 13);
        mbWinWait(winId);
        ev_SingleKoopaMgSkip(guideObj);
        mbPlayerRotYSet(playerNo, 0);
        mbPlayerPosReset(playerNo);
        mbCameraPlayerViewSetFast(playerNo, 0);
    } else {
        winId = mbWinCreate(2, MESSNUM(0x2B, 8), 13);
        mbWinWait(winId);
        mbObjMotionShiftSet(guideObj, 1, 0, 8, 0x40000001);
        winId = mbWinCreate(2, MESSNUM(0x2B, 9), 13);
        mbWinInsertMesSet(winId, mbPlayerNameMesGet(playerNo), 0);
        mbWinWait(winId);
        mbAudFXDelaySet(30);
        mbAudFXPlay(0x3CD);
        mbObjMotionShiftSet(guideObj, 5, 0, 8, 0);
        if(mbPlayerCoinGet(playerNo) == 0) {
            if(SingleMgUnlockListGet(NULL)) {
                winId = mbWinCreate(2, MESSNUM(0x2B, 10), 13);
                mbWinWait(winId);
                returnMode = 0;
            } else {
                winId = mbWinCreate(2, MESSNUM(0x2B, 12), 13);
                mbWinInsertMesSet(winId, mbPlayerNameMesGet(playerNo), 0);
                mbWinWait(winId);
                returnMode = 1;
            }
        } else {
            if(SingleMgUnlockListGet(NULL) && mbRandMod(100) < 50) {
                winId = mbWinCreate(2, MESSNUM(0x2B, 10), 13);
                mbWinWait(winId);
                returnMode = 0;
            } else {
                winId = mbWinCreate(2, MESSNUM(0x2B, 11), 13);
                mbWinInsertMesSet(winId, mbPlayerNameMesGet(playerNo), 0);
                mbWinWait(winId);
                returnMode = 2;
            }
        }
        mbObjMotionShiftSet(guideObj, 1, 0, 8, 0x40000001);
        winId = mbWinCreate(2, MESSNUM(0x2B, 13), 13);
        mbWinWait(winId);
        i = mbRandMod(3);
        miniKoopaMgType = mgTypeTbl[i][0];
        _SetFlag(0x10003);
        mbev_MgCallSingleKoopa(mgTypeTbl[i][1], 1);
    }
    mbObjKill(guideObj);
    if(capObj != 0) {
        mbCapObjKill(capObj);
    }
    for(i=0; i<2; i++) {
        espKill(espId[i]);
    }
    for(i=0; i<2; i++) {
        mbPlayerMotionKill(playerNo, motId[i]);
    }
    for(i=0; i<2; i++) {
        if(particleId[i] >= 0) {
            mbParticleKill(particleId[i]);
        }
    }
    HuDataDirClose(0xE0000);
    if(mbWipeSpecialStatGet()) {
        mbWipeFadeIn();
    }
}

static void ev_SingleKoopaMgSkip(s16 guideObj)
{
    s16 winId = mbWinCreate(2, MESSNUM(0x2B, 7), 13);
    mbWinWait(winId);
    mbWipeSpecialFadeInCreate(7, 30);
    mbWipeSpecialWait();
    mbWipeFadeOutTime(1);
    mbWipeSpecialKill();
    mbObjDispSet(guideObj, FALSE);
    mbMusBoardPlay();
}

static inline s16 SinglePlayerMgCoinGet(int playerNo)
{
    return GwPlayer[playerNo].mgCoin;
}

static inline s16 SinglePlayerMgCoinBonusGet(int playerNo)
{
    return GwPlayer[playerNo].mgCoinBonus;
}

static inline void SingleEffDispSet(s16 no, BOOL dispF)
{
    SINGLEEFF *eff = &singleEffData[no-1];
    eff->dispF = dispF;
}

static inline void SingleEffPosGet(s16 no, HuVecF *pos)
{
    SINGLEEFF *eff = &singleEffData[no-1];
    *pos = eff->pos;
}

static inline u8 SingleMasuTypeGet(int no)
{
    return masuType[no];
}

static void ev_SingleKoopaMgEnd(int playerNo)
{
    static int guideMot[] = { 0xE0001, 0xE0005, 0xE0003, 0xE0007, 0xE0004, 0xE000A, HU_DATANUM_NONE };
    static const HuVecF cameraOfs = { 0, 100, 100 };
    s16 particleId[5] = { -1, -1, -1, -1, -1 };
    s16 capObj[3] = { 0, 0, 0 };
    s16 fireId[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    s16 effId[5] = { 0, 0, 0, 0, 0 };
    u8 mgList[132];
    HuVecF pos;
    HuVecF capPos;
    HuVecF masuPos;
    int mode;
    int seNo;
    BOOL unlockF;
    s16 playerMasuId;
    s16 masuId;
    int i;
    int fireNum;
    int num;
    int mgNo;
    s16 winId;
    MBOBJID guideObj;

    if(!mbWipeSpecialStatGet()) {
        mbWipeFadeOut();
    }
    mbStatusDispForceSet(playerNo, TRUE);
    mgNo = GwSystem.mgNo;
    playerMasuId = GwPlayer[playerNo].masuId;
    mbPlayerRotYSet(playerNo, 0);
    guideObj = mbObjCreate(0xE0000, guideMot, FALSE);
    masuId = mbMasuFind_AttrIdGet(-1, 0x8000);
    mbMasuPosGet(masuId, &masuPos);
    mbObjPosSetV(guideObj, &masuPos);
    mbObjDispSet(guideObj, TRUE);
    mbObjMotionSet(guideObj, 1, 0x40000001);
    mbPlayerColSnapPlayerSet(playerNo, FALSE);
    mbPlayerPosSet(playerNo, masuPos.x, masuPos.y, 300.0f+masuPos.z);
    mbPlayerMotionSet(playerNo, 1, 0x40000001);
    mbPlayerRotYSet(playerNo, 180);
    mbCameraMoveMasu(masuId, NULL, &cameraOfs, 1600, -1, -1);
    mbCameraMoveWait();
    mbMusPlay(0, 0x1C, 127, 0);
    if(SinglePlayerMgCoinBonusGet(playerNo)+SinglePlayerMgCoinGet(playerNo) > 0) {
        mode = 0;
    } else {
        for(i=1; i<4; i++) {
            if(SinglePlayerMgCoinBonusGet(i)+SinglePlayerMgCoinGet(i) > 0) {
                break;
            }
        }
        mode = (i < 4) ? 1 : 2;
    }
    switch(mode) {
        case 0:
            mbSingleCall(10, MgDataTbl[mgNo].type);
            mbAudFXDelaySet(30);
            mbAudFXPlay(0x3CC);
            mbObjMotionSet(guideObj, 6, 0x40000001);
            mbWipeFadeIn();
            mbPauseDisableSet(FALSE);
            winId = mbWinCreate(2, MESSNUM(0x2B, 0xE), 13);
            mbWinWait(winId);
            mbAudFXDelaySet(30);
            mbAudFXPlay(0x3CC);
            mbObjMotionShiftSet(guideObj, 6, 0, 8, 0x40000001);
            if(!(unlockF = SingleMgUnlockCheck(mgNo+0x259))) {
                winId = mbWinCreate(2, MESSNUM(0x2B, 0xF), 13);
                mbWinWait(winId);
            } else {
                winId = mbWinCreate(2, MESSNUM(0x2B, 0x2A), 13);
                mbWinWait(winId);
            }
            mbAudFXDelaySet(30);
            mbAudFXPlay(0x3CD);
            mbObjMotionShiftSet(guideObj, 5, 0, 8, 0);
            while(!mbObjMotionEndCheck(guideObj)) {
                HuPrcVSleep();
            }
            capPos.x = masuPos.x;
            capPos.y = 200.0f+masuPos.y;
            capPos.z = 200.0f+masuPos.z;
            particleId[0] = SingleParticleCreate(singleEffAnim[2], 100, SingleEffMgCapsuleHook);
            pos.x = capPos.x;
            pos.y = 80.0f+capPos.y;
            pos.z = 100.0f+capPos.z;
            Hu3DModelPosSetV(particleId[0], &pos);
            Hu3DModelCameraSet(particleId[0], HU3D_CAM1);
            Hu3DModelLayerSet(particleId[0], 7);
            mbAudFXPlay(0x60B);
            HuPrcSleep(12);
            mbAudFXPlay(0x427);
            effId[0] = SingleEffCreate(&pos, miniKoopaMgType);
            HuPrcSleep(30);
            SingleEffMgStart(effId[0]);
            mbAudFXPlay(0x607);
            HuPrcSleep(60);
            if(!unlockF) {
                mbSingleMgUnlockSet(mgNo+0x259);
                GWSingleMgFlagSet(mgNo+0x259);
                SingleMasuTypePush(miniKoopaMgType);
                mbSingleCall(9, mgNo);
            }
            SingleMgRecordPrizeSet();
            while(SingleEffGet(effId[0])->mode) {
                HuPrcVSleep();
            }
            HuPrcSleep(30);
            if(!(MgDataTbl[mgNo].flag & 0x40)) {
                mbCoinAddExec(playerNo, 10);
            } else {
                mbCoinAddExec(playerNo, SinglePlayerMgCoinGet(playerNo)+SinglePlayerMgCoinBonusGet(playerNo));
            }
            mbPlayerMotionShiftSet(playerNo, 7, 0, 8, 0);
            mbPlayerWinLoseVoicePlay(playerNo, 7, 0x243);
            if(!unlockF) {
                winId = mbWinCreate(2, MESSNUM(0x2B, 0), -1);
                mbWinInsertMesSet(winId, mbPlayerNameMesGet(playerNo), 0);
                if(mgNo != 0xFFFF) {
                    mbWinInsertMesSet(winId, MgDataTbl[mgNo].nameMes, 1);
                } else {
                    mbWinInsertMesSet(winId, MESSNUM(0x2B, 0x27), 1);
                }
                mbWinWait(winId);
            }
            while(!mbPlayerMotionEndCheck(playerNo)) {
                HuPrcVSleep();
            }
            HuPrcSleep(30);
            mbPlayerMotionShiftSet(playerNo, 1, 0, 8, 0x40000001);
            mbAudFXDelaySet(30);
            mbAudFXPlay(0x3CB);
            mbObjMotionShiftSet(guideObj, 3, 0, 8, 0x40000001);
            HuPrcSleep(30);
            break;

        case 1:
            mbAudFXDelaySet(30);
            mbAudFXPlay(0x3CB);
            mbObjMotionSet(guideObj, 3, 0x40000001);
            mbWipeFadeIn();
            mbPauseDisableSet(FALSE);
            winId = mbWinCreate(2, MESSNUM(0x2B, 0x10), 13);
            mbWinInsertMesSet(winId, mbPlayerNameMesGet(playerNo), 0);
            mbWinWait(winId);
            switch(returnMode) {
                case 0:
                    num = SingleMgUnlockListGet(mgList);
                    if(num == 0) {
                        break;
                    }
                    winId = mbWinCreate(2, MESSNUM(0x2B, 0x11), 13);
                    mbWinWait(winId);
                    if(num > 5) {
                        num = 5;
                    }
                    pos.x = masuPos.x-(100.0f*(0.75f*(num-1)));
                    pos.y = 200.0f+masuPos.y;
                    pos.z = 200.0f+masuPos.z;
                    for(i=0; i<num; i++) {
                        effId[i] = SingleEffCreate(&pos, SingleMasuTypeGet(i));
                        SingleEffDispSet(effId[i], FALSE);
                        particleId[i] = SingleParticleCreate(singleEffAnim[2], 100, SingleEffMgCapsuleHook);
                        Hu3DModelPosSet(particleId[i], pos.x, 80.0f+pos.y, 100.0f+pos.z);
                        Hu3DModelCameraSet(particleId[i], HU3D_CAM1);
                        Hu3DModelLayerSet(particleId[i], 7);
                        pos.x += 150.0f;
                    }
                    mbAudFXPlay(0x60B);
                    HuPrcSleep(12);
                    for(i=0; i<num; i++) {
                        SingleEffDispSet(effId[i], TRUE);
                    }
                    seNo = mbAudFXPlay(0x606);
                    HuPrcSleep(12);
                    mbObjAttrReset(guideObj, 0x40000001);
                    while(!mbObjMotionEndCheck(guideObj)) {
                        HuPrcVSleep();
                    }
                    mbAudFXDelaySet(30);
                    mbAudFXPlay(0x3CD);
                    mbObjMotionShiftSet(guideObj, 5, 0, 8, 0);
                    HuPrcVSleep();
                    while(mbObjMotionShiftIDGet(guideObj) != -1 || !mbObjMotionEndCheck(guideObj)) {
                        HuPrcVSleep();
                    }
                    mbAudFXPlay(0x427);
                    mbAudFXStop(seNo);
                    mbAudFXPlay(0x60C);
                    omVibrate(playerNo, 20, 20, 0);
                    fireNum = 0;
                    for(i=0; i<num; i++) {
                        SingleEffPosGet(effId[i], &pos);
                        fireId[fireNum] = SingleParticleCreate(singleEffAnim[3], 256, SingleEffMgFireHook);
                        Hu3DModelPosSetV(fireId[fireNum], &pos);
                        fireNum++;
                        fireId[fireNum] = SingleParticleCreate(singleEffAnim[3], 64, SingleEffMgFire2Hook);
                        Hu3DModelPosSet(fireId[fireNum], 0, 0.05f, 0.05f);
                        Hu3DModelPosSetV(fireId[fireNum], &pos);
                        fireNum++;
                    }
                    HuPrcSleep(12);
                    mbSingleMgUnlockInit();
                    SingleMasuTypeReset();
                    GwSingleMgFlag[0] = GwSingleMgFlag[1] = GwSingleMgFlag[2] = 0;
                    mbSinglePrizeFlagReset(6);
                    GWSingleMgWinNumSet(0);
                    mbSinglePrizeFlagReset(5);
                    GWSingleMgRecordNumSet(0);
                    SingleMgRecordRestore();
                    for(i=0; i<num; i++) {
                        SingleEffDispSet(effId[i], FALSE);
                    }
                    HuPrcSleep(60);
                    mbPlayerMotionShiftSet(playerNo, 8, 0, 8, 0);
                    mbAudFXDelaySet(30);
                    mbAudFXPlay(0x3CB);
                    mbObjMotionShiftSet(guideObj, 3, 0, 8, 0x40000001);
                    break;

                case 1:
                    winId = mbWinCreate(2, MESSNUM(0x2B, 0x13), 13);
                    mbWinWait(winId);
                    num = mbPlayerCapsuleNumGet(playerNo);
                    pos.x = masuPos.x-(100.0f*(0.75f*(num-1)));
                    pos.y = 200.0f+masuPos.y;
                    pos.z = 200.0f+masuPos.z;
                    for(i=0; i<num; i++) {
                        capObj[i] = mbCapObjCreate(mbPlayerCapsuleGet(playerNo, i), 0);
                        mbObjDispSet(capObj[i], FALSE);
                        mbObjPosSetV(capObj[i], &pos);
                        mbObjCameraSet(capObj[i], HU3D_CAM0);
                        mbObjLayerSet(capObj[i], 4);
                        mbObjMotionSpeedSet(capObj[i], 0);
                        particleId[i] = SingleParticleCreate(singleEffAnim[2], 100, SingleEffMgCapsuleHook);
                        Hu3DModelPosSet(particleId[i], pos.x, 80.0f+pos.y, 100.0f+pos.z);
                        Hu3DModelCameraSet(particleId[i], HU3D_CAM1);
                        Hu3DModelLayerSet(particleId[i], 7);
                        pos.x += 150.0f;
                    }
                    mbAudFXPlay(0x60B);
                    for(i=0; i<num; i++) {
                        mbPlayerCapsuleRemove(playerNo, 0);
                    }
                    HuPrcSleep(12);
                    for(i=0; i<num; i++) {
                        mbObjDispSet(capObj[i], TRUE);
                    }
                    HuPrcSleep(12);
                    mbObjAttrReset(guideObj, 0x40000001);
                    while(!mbObjMotionEndCheck(guideObj)) {
                        HuPrcVSleep();
                    }
                    mbAudFXDelaySet(30);
                    mbAudFXPlay(0x3CD);
                    mbObjMotionShiftSet(guideObj, 5, 0, 8, 0);
                    HuPrcVSleep();
                    while(mbObjMotionShiftIDGet(guideObj) != -1 || !mbObjMotionEndCheck(guideObj)) {
                        HuPrcVSleep();
                    }
                    mbAudFXPlay(0x427);
                    mbAudFXPlay(0x60C);
                    omVibrate(playerNo, 20, 20, 0);
                    fireNum = 0;
                    for(i=0; i<num; i++) {
                        mbObjPosGet(capObj[i], &pos);
                        fireId[fireNum] = SingleParticleCreate(singleEffAnim[3], 256, SingleEffMgFireHook);
                        Hu3DModelPosSetV(fireId[fireNum], &pos);
                        fireNum++;
                        fireId[fireNum] = SingleParticleCreate(singleEffAnim[3], 64, SingleEffMgFire2Hook);
                        Hu3DModelPosSet(fireId[fireNum], 0, 0.05f, 0.05f);
                        Hu3DModelPosSetV(fireId[fireNum], &pos);
                        fireNum++;
                    }
                    HuPrcSleep(12);
                    for(i=0; i<num; i++) {
                        mbObjDispSet(capObj[i], FALSE);
                    }
                    HuPrcSleep(60);
                    mbPlayerMotionShiftSet(playerNo, 8, 0, 8, 0);
                    mbAudFXDelaySet(30);
                    mbAudFXPlay(0x3CB);
                    mbObjMotionShiftSet(guideObj, 3, 0, 8, 0x40000001);
                    break;

                case 2:
                    winId = mbWinCreate(2, MESSNUM(0x2B, 0x12), 13);
                    mbWinWait(winId);
                    mbAudFXDelaySet(30);
                    mbAudFXPlay(0x3CD);
                    mbObjMotionShiftSet(guideObj, 5, 0, 8, 0);
                    while(!mbObjMotionEndCheck(guideObj)) {
                        HuPrcVSleep();
                    }
                    mbWipeWhiteFadeOutTime(1);
                    mbAudFXPlay(0x427);
                    mbWipeWhiteFadeInTime(90);
                    omVibrate(playerNo, 20, 20, 0);
                    mbPlayerMotionShiftSet(playerNo, 8, 0, 8, 0);
                    mbCoinAddProcExec(playerNo, -mbPlayerCoinGet(playerNo), -1, TRUE);
                    mbAudFXDelaySet(30);
                    mbAudFXPlay(0x3CB);
                    mbObjMotionShiftSet(guideObj, 3, 0, 8, 0x40000001);
                    HuPrcSleep(30);
                    break;
            }
            while(!mbPlayerMotionEndCheck(playerNo)) {
                HuPrcVSleep();
            }
            HuPrcSleep(30);
            mbPlayerMotionShiftSet(playerNo, 1, 0, 8, 0x40000001);
            break;

        case 2:
            mbAudFXDelaySet(30);
            mbAudFXPlay(0x3CB);
            mbObjMotionSet(guideObj, 3, 0x40000001);
            mbWipeFadeIn();
            mbPauseDisableSet(FALSE);
            winId = mbWinCreate(2, MESSNUM(0x2B, 0x14), 13);
            mbWinInsertMesSet(winId, mbPlayerNameMesGet(playerNo), 0);
            mbWinWait(winId);
            winId = mbWinCreate(2, MESSNUM(0x2B, 0x15), 13);
            mbWinWait(winId);
            break;
    }
    ev_SingleKoopaMgSkip(guideObj);
    mbPlayerPosReset(playerNo);
    mbCameraPlayerViewSetFast(playerNo, 0);
    mbObjKill(guideObj);
    for(i=0; i<5; i++) {
        if(particleId[i] >= 0) {
            mbParticleKill(particleId[i]);
        }
    }
    for(i=0; i<10; i++) {
        if(fireId[i] >= 0) {
            mbParticleKill(fireId[i]);
        }
    }
    for(i=0; i<3; i++) {
        if(capObj[i] > 0) {
            mbCapObjKill(capObj[i]);
        }
    }
    for(i=0; i<5; i++) {
        if(effId[i] > 0) {
            SingleEffKill(effId[i]);
        }
    }
    HuDataDirClose(0xE0000);
}

/* @@PART3@@ */



static inline void SingleKoopaSePlay(int seNo)
{
    static s32 seLoseTbl[][3] = {
        { 0x2A3, 0x273, 0x25B },
        { 0x2A4, 0x274, 0x25C },
        { 0x2A5, 0x275, 0x25D },
        { 0x2A9, 0x279, 0x261 },
        { -1, -1, -1 }
    };
    int i, j;
    for(i = 0; seLoseTbl[i][0] >= 0; i++) {
        for(j = 0; j < 3; j++) {
            if(seLoseTbl[i][j] == seNo) {
                break;
            }
        }
        if(j < 3) {
            break;
        }
    }
    if(seLoseTbl[i][0] >= 0) {
        mbAudFXPlay(seLoseTbl[i][miniKoopaType]);
    } else {
        mbAudFXPlay(seNo);
    }
}

static inline BOOL SingleKettouMgUnlockAllCheck(void)
{
    int i;
    for(i = 0; MgDataTbl[i].ovl != 0xFFFF; i++) {
        if(MgDataTbl[i].type == MG_TYPE_KETTOU && !GWMgUnlockGet(i + 0x259) && !mbSingleMgUnlockGet(i + 0x259)) {
            return FALSE;
        }
    }
    return TRUE;
}

static void ev_SingleMKoopaMg(int playerNo, s16 masuId)
{
    static HuVecF viewOfs = { 0, 100, 0 };
    int koopaNo;
    int speakerNo;
    s16 winNo;
    s16 motId;
    s16 koopaMotId;
    float angle;
    HuVecF masuPos;
    HuVecF playerPos;
    HuVecF koopaPos;
    HuVecF dir;

    miniKoopaType = mbMasuCapsuleGet(masuId);
    if(miniKoopaType < 0) {
        miniKoopaType = 0;
    }
    koopaNo = miniKoopaType + 1;
    speakerNo = miniKoopaType + 19;
    mbWipeSpecialFadeInCreate(8, 30);
    mbWipeSpecialWait();
    GwPlayer[koopaNo].masuId = masuId;
    GwPlayer[koopaNo].masuIdNext = masuId;
    mbPlayerDispSet(koopaNo, TRUE);
    mbPlayerMotionSet(playerNo, 1, 0x40000001);
    mbPlayerMotionSet(koopaNo, 1, 0x40000001);
    motId = mbPlayerMotionCreate(playerNo, 0x8E0078);
    koopaMotId = mbPlayerMotionCreate(koopaNo, 0x8E0078);
    mbMasuPosGet(masuId, &masuPos);
    mbPlayerPosSet(playerNo, 70.0f + masuPos.x, masuPos.y, masuPos.z - 70.0f);
    mbPlayerPosSet(koopaNo, masuPos.x - 70.0f, masuPos.y, 70.0f + masuPos.z);
    mbPlayerColSnapPlayerSet(playerNo, FALSE);
    mbPlayerColSnapPlayerSet(koopaNo, FALSE);
    mbPlayerPosGet(playerNo, &playerPos);
    mbPlayerPosGet(koopaNo, &koopaPos);
    PSVECSubtract(&koopaPos, &playerPos, &dir);
    angle = 180.0 * (atan2(dir.x, dir.z) / M_PI);
    mbPlayerRotYSet(playerNo, angle);
    mbPlayerRotYSet(koopaNo, 180.0f + angle);
    mbCameraMoveMasu(masuId, NULL, &viewOfs, 1600.0f, -1.0f, -1);
    mbCameraMoveWait();
    mbMusPlay(0, 26, 127, 0);
    mbWipeSpecialFadeOutCreate(8, 30);
    mbWipeSpecialWait();
    winNo = mbWinCreate(2, 0x2B0016, speakerNo);
    SingleKoopaSePlay(0x2A5);
    mbWinInsertMesSet(winNo, mbPlayerNameMesGet(playerNo), 0);
    mbWinWait(winNo);
    if(!SingleKettouMgUnlockAllCheck()) {
        if(mbPlayerCoinGet(playerNo) == 0) {
            winNo = mbWinCreate(2, 0x2B0017, speakerNo);
            mbWinWait(winNo);
            winNo = mbWinCreate(2, 0x2B0018, speakerNo);
            SingleKoopaSePlay(0x2A5);
            mbWinWait(winNo);
            returnMode = 0;
        } else {
            winNo = mbWinCreate(2, 0x2B0017, speakerNo);
            mbWinWait(winNo);
            winNo = mbWinCreate(2, 0x2B001A, speakerNo);
            SingleKoopaSePlay(0x2A5);
            mbWinWait(winNo);
            returnMode = 1;
        }
    } else {
        if(mbPlayerCoinGet(playerNo) != 0) {
            winNo = mbWinCreate(2, 0x2B001C, speakerNo);
            mbWinWait(winNo);
            winNo = mbWinCreate(2, 0x2B001A, speakerNo);
            SingleKoopaSePlay(0x2A5);
            mbWinWait(winNo);
            returnMode = 2;
        } else {
            winNo = mbWinCreate(2, 0x2B001C, speakerNo);
            mbWinWait(winNo);
            winNo = mbWinCreate(2, 0x2B0018, speakerNo);
            SingleKoopaSePlay(0x2A5);
            mbWinWait(winNo);
            returnMode = 3;
        }
    }
    mbPlayerMotionShiftSet(playerNo, motId, 0.0f, 8.0f, 0x40000001);
    Hu3DMotionAttrSet(mbObjMotionIDGet(mbPlayerObjIDGet(playerNo), motId), HU3D_MOTATTR_LOOP);
    mbPlayerMotionShiftSet(koopaNo, koopaMotId, 0.0f, 8.0f, 0x40000001);
    Hu3DMotionAttrSet(mbObjMotionIDGet(mbPlayerObjIDGet(koopaNo), koopaMotId), HU3D_MOTATTR_LOOP);
    winNo = mbWinCreate(2, 0x2B001D, speakerNo);
    SingleKoopaSePlay(0x2A3);
    mbWinWait(winNo);
    _SetFlag(FLAG_BOARD_MG_KETTOU);
    mbev_MgCallSingle(6);
    mbPlayerMotionKill(playerNo, motId);
    mbPlayerMotionKill(koopaNo, koopaMotId);
    mbPlayerDispSet(koopaNo, FALSE);
    GwPlayer[koopaNo].masuId = 0;
}

static void ev_SingleMKoopaMgEnd(int playerNo)
{
    static HuVecF viewOfs = { 0, 100, 0 };
    s16 effNo = 0;
    s16 particleId = -1;
    int mgNo;
    s16 masuId;
    int masuType;
    int koopaNo;
    int speakerNo;
    s16 winNo;
    int seNo;
    BOOL unlockF;
    s16 coin;
    s16 bonus;
    s16 koopaCoin;
    s16 koopaBonus;
    float angle;
    HuVecF effPos;
    HuVecF playerPos;
    HuVecF koopaPos;
    HuVecF masuPos;
    HuVecF dir;

    if(!mbWipeSpecialStatGet()) {
        mbWipeFadeOut();
    }
    mgNo = GwSystem.mgNo;
    masuId = GwPlayer[playerNo].masuId;
    masuType = mbMasuTypeGet(masuId);
    mbStatusDispForceSet(playerNo, TRUE);
    mbCameraMoveMasu(masuId, NULL, &viewOfs, 1600.0f, -1.0f, -1);
    mbCameraMoveWait();
    miniKoopaType = mbMasuCapsuleGet(masuId);
    if(miniKoopaType < 0) {
        miniKoopaType = 0;
    }
    koopaNo = miniKoopaType + 1;
    speakerNo = miniKoopaType + 19;
    GwPlayer[koopaNo].masuId = masuId;
    GwPlayer[koopaNo].masuIdNext = masuId;
    mbPlayerDispSet(koopaNo, TRUE);
    mbPlayerColSnapPlayerSet(playerNo, FALSE);
    mbPlayerColSnapPlayerSet(koopaNo, FALSE);
    mbMasuPosGet(masuId, &masuPos);
    mbPlayerPosSet(playerNo, 70.0f + masuPos.x, masuPos.y, masuPos.z - 70.0f);
    mbPlayerPosSet(koopaNo, masuPos.x - 70.0f, masuPos.y, 70.0f + masuPos.z);
    mbMusPlay(0, 26, 127, 0);
    coin = GwPlayer[playerNo].mgCoin;
    bonus = GwPlayer[playerNo].mgCoinBonus;
    if(bonus + coin > 0) {
        mbSingleCall(10, 6);
        mbPlayerMotionSet(koopaNo, 6, 0x40000001);
        mbWipeFadeIn();
        mbPauseDisableSet(FALSE);
        winNo = mbWinCreate(2, 0x2B001F, speakerNo);
        SingleKoopaSePlay(0x2A9);
        mbWinWait(winNo);
        switch(returnMode) {
            case 0:
            case 1:
                winNo = mbWinCreate(2, 0x2B0021, speakerNo);
                mbWinWait(winNo);
                mbMasuPosGet(masuId, &effPos);
                effPos.y += 200.0f;
                effPos.z += 200.0f;
                particleId = SingleParticleCreate(singleEffAnim[2], 100, SingleEffMgCapsuleHook);
                Hu3DModelPosSet(particleId, effPos.x, 80.0f + effPos.y, 100.0f + effPos.z);
                Hu3DModelCameraSet(particleId, 2);
                Hu3DModelLayerSet(particleId, 7);
                HuPrcSleep(12);
                effNo = SingleEffCreate(&effPos, masuType);
                seNo = mbAudFXPlay(0x606);
                HuPrcSleep(30);
                mbAudFXStop(seNo);
                SingleEffMgStart(effNo);
                mbAudFXPlay(0x607);
                HuPrcSleep(60);
                if(!(unlockF = SingleMgUnlockCheck(mgNo + 0x259))) {
                    mbSingleMgUnlockSet(mgNo + 0x259);
                    GWSingleMgFlagSet(mgNo + 0x259);
                    SingleMasuTypePush(miniKoopaType + 9);
                    mbSingleCall(9, mgNo);
                }
                while(SingleEffGet(effNo)->mode != 0) {
                    HuPrcVSleep();
                }
                HuPrcSleep(30);
                mbCoinAddExec(playerNo, 10);
                mbPlayerMotionShiftSet(playerNo, 7, 0.0f, 8.0f, 0);
                mbPlayerWinLoseVoicePlay(playerNo, 7, 0x243);
                if(!unlockF) {
                    winNo = mbWinCreate(2, 0x2B0000, -1);
                    mbWinInsertMesSet(winNo, mbPlayerNameMesGet(playerNo), 0);
                    if(mgNo != 0xFFFF) {
                        mbWinInsertMesSet(winNo, MgDataTbl[mgNo].nameMes, 1);
                    } else {
                        mbWinInsertMesSet(winNo, 0x2B0027, 1);
                    }
                    mbWinWait(winNo);
                }
                break;
            case 2:
            case 3:
                winNo = mbWinCreate(2, 0x2B0024, speakerNo);
                mbWinWait(winNo);
                mbPlayerMotionShiftSet(playerNo, 7, 0.0f, 8.0f, 0);
                mbPlayerWinLoseVoicePlay(playerNo, 7, 0x243);
                mbCoinAddProcExec(playerNo, 10, 1, TRUE);
                break;
        }
        SingleMgRecordPrizeSet();
        while(!mbPlayerMotionEndCheck(playerNo)) {
            HuPrcVSleep();
        }
        HuPrcSleep(30);
        mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 8.0f, 0x40000001);
    } else if((koopaCoin = GwPlayer[1].mgCoin, koopaBonus = GwPlayer[1].mgCoinBonus, koopaBonus + koopaCoin > 0)) {
        mbPlayerMotionSet(playerNo, 6, 0x40000001);
        mbPlayerMotionSet(koopaNo, 7, 0);
        mbWipeFadeIn();
        mbPauseDisableSet(FALSE);
        winNo = mbWinCreate(2, 0x2B001E, speakerNo);
        mbWinWait(winNo);
        switch(returnMode) {
            case 0:
            case 3:
                winNo = mbWinCreate(2, 0x2B0025, speakerNo);
                mbWinWait(winNo);
                break;
            case 1:
            case 2:
                winNo = mbWinCreate(2, 0x2B0026, speakerNo);
                mbWinWait(winNo);
                mbCoinAddProcExec(playerNo, -(mbPlayerCoinGet(playerNo) + 1) / 2, -1, TRUE);
                break;
        }
    } else {
        mbPlayerMotionSet(playerNo, 1, 0x40000001);
        mbPlayerMotionSet(koopaNo, 1, 0x40000001);
        mbPlayerPosGet(playerNo, &playerPos);
        mbPlayerPosGet(koopaNo, &koopaPos);
        PSVECSubtract(&koopaPos, &playerPos, &dir);
        angle = 180.0 * (atan2(dir.x, dir.z) / M_PI);
        mbPlayerRotYSet(playerNo, angle);
        mbPlayerRotYSet(koopaNo, 180.0f + angle);
        mbWipeFadeIn();
        mbPauseDisableSet(FALSE);
        winNo = mbWinCreate(2, 0x2B002B, speakerNo);
        mbWinWait(winNo);
        goto end;
    }
    mbPlayerPosGet(playerNo, &playerPos);
    mbPlayerPosGet(koopaNo, &koopaPos);
    PSVECSubtract(&koopaPos, &playerPos, &dir);
    angle = 180.0 * (atan2(dir.x, dir.z) / M_PI);
    mbPlayerRotateStart(playerNo, angle, 15);
    mbPlayerRotateStart(koopaNo, 180.0f + angle, 15);
    HuPrcSleep(30);
    winNo = mbWinCreate(2, 0x2B0020, speakerNo);
    SingleKoopaSePlay(0x2A4);
    mbWinWait(winNo);
end:
    mbWipeSpecialFadeInCreate(8, 30);
    mbWipeSpecialWait();
    mbWipeFadeOutTime(1);
    mbWipeSpecialKill();
    mbPlayerDispSet(koopaNo, FALSE);
    GwPlayer[koopaNo].masuId = 0;
    if(effNo != 0) {
        SingleEffKill(effNo);
    }
    if(particleId >= 0) {
        mbParticleKill(particleId);
    }
}

static void SingleMgSaveInit(void)
{
    SINGLESAVEWORK *work = &singleSaveWork;
    if(mbSaveNewF && !_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        GWSingleDataInit();
        GWSingleMgWinNumSet(0);
        GWSingleMgRecordNumSet(0);
        memset(singleBoardFlagOld, 0, sizeof(singleBoardFlagOld));
        memset(work, 0, sizeof(SINGLESAVEWORK));
    }
}

void mbSinglePrizeFlagReset(int flag)
{
    if(flag <= GW_SINGLE_PRIZE_FLAG_MAX) {
        GwSinglePrizeFlag[flag >> 5] &= ~(1 << (flag & 31));
    }
}

int mbSingleCall(int cmd, int value)
{
    int playerNo = GwSystem.turnPlayerNo;
    SINGLESAVEWORK *work = &singleSaveWork;
    int i;
    int j;
    int masuType;
    int num;
    u32 *flagP;

    if(GWPartyGet() != FALSE || _CheckFlag(FLAG_BOARD_TUTORIAL)) {
        return 0;
    }
    switch(cmd) {
        case 0:
            work->micChar = -1;
            singleListenerOnF = TRUE;
            SingleMicListenerCreate();
            return -1;

        case 1:
            SingleMicListenerKill();
            work->micChar = -1;
            singleListenerOnF = FALSE;
            return -1;

        case 2: {
            int charNo;
            int charNum;
            u8 charList[10];
            SingleMicListenerKill();
            singleListenerOnF = FALSE;
            if(work->micChar >= 0 && GwPlayer[playerNo].diceMode == 0) {
                work->micNum++;
                charNo = work->micChar;
                if(mbRandMod(100) < 50) {
                    charNum = 0;
                    for(i = 0; i < 6; i++) {
                        if(i != work->micChar) {
                            charList[charNum++] = i;
                        }
                    }
                    charNo = charList[mbRandMod(charNum)];
                }
                return charNo;
            }
            return -1;
        }

        case 3:
            if(work->mgNum < 99) {
                work->mgNum++;
            }
            work->mgHist[work->mgHistNo] = value;
            if(!GWSinglePrizeFlagGet(12)) {
                j = work->mgHistNo - 1;
                for(i = 0; i < 2; i++, j--) {
                    if(j < 0) {
                        j = 2;
                    }
                    if(work->mgHist[j] != value) {
                        break;
                    }
                }
                if(i >= 2) {
                    GWSinglePrizeFlagSet(12);
                    mbSinglePrizeFlagReset(11);
                } else if(i == 1) {
                    GWSinglePrizeFlagSet(11);
                }
            }
            if((value & 1) == 0) {
                work->mgEvenNum++;
            } else {
                work->mgOddNum++;
            }
            work->mgRankTotal += value;
            if(++work->mgHistNo >= 3) {
                work->mgHistNo = 0;
            }
            if(work->micChar < 0) {
                work->micMissF = TRUE;
            } else if(work->micChar + 1 == value) {
                work->micHitNum++;
            }
            break;

        case 4:
            if(work->unk10 < 99) {
                work->unk10++;
            }
            if(value == 2) {
                work->unk17 = TRUE;
            } else {
                work->unk16 = TRUE;
            }
            break;

        case 5:
            if(work->unk11 < 99) {
                work->unk11++;
            }
            work->hist12[work->hist12No] = value;
            if(!GWSinglePrizeFlagGet(26)) {
                j = work->hist12No - 1;
                for(i = 0; i < 2; i++, j--) {
                    if(j < 0) {
                        j = 2;
                    }
                    if(work->hist12[j] != value) {
                        break;
                    }
                }
                if(i >= 2) {
                    GWSinglePrizeFlagSet(26);
                    mbSinglePrizeFlagReset(25);
                } else if(i == 1) {
                    GWSinglePrizeFlagSet(25);
                }
            }
            if(++work->hist12No >= 3) {
                work->hist12No = 0;
            }
            break;

        case 6:
            if(work->unk18 < 99) {
                work->unk18++;
            }
            break;

        case 7:
            if(mbMasuDispCheck(value)) {
                flagP = &((u32 *)singleBoardFlagOld)[singleBoard * 2] + 1;
                i = value - 1;
                if(i >= 32) {
                    flagP--;
                    i -= 32;
                }
                *flagP |= 1 << i;
            }
            masuType = mbMasuTypeGet(value);
            if(masuType == 7) {
                GWSinglePrizeFlagSet(39);
            }
            if(work->masuNum[masuType] < 99) {
                work->masuNum[masuType]++;
            }
            break;

        case 8:
            if(work->unk01 < 99) {
                work->unk01++;
            }
            break;

        case 9: {
            int mgType;
            u8 mgList[128];
            GWSinglePrizeFlagSet(6);
            GWSingleMgWinNumSet(GWSingleMgWinNumGet() + 1);
            mgType = MgDataTbl[value].type;
            num = SingleMgLockNumGet(mgType, mgList);
            if(num == 0) {
                switch(mgType) {
                    case 0:
                        GWSinglePrizeFlagSet(41);
                        break;
                    case 1:
                        GWSinglePrizeFlagSet(42);
                        break;
                    case 2:
                        GWSinglePrizeFlagSet(43);
                        break;
                    case 3:
                        GWSinglePrizeFlagSet(44);
                        break;
                    case 6:
                        GWSinglePrizeFlagSet(45);
                        break;
                }
            }
            num = SingleMgLockNumGet(-1, NULL);
            if(num == 0) {
                GWSinglePrizeFlagSet(47);
            }
            break;
        }

        case 10:
            GWSingleMgWinInc(GWStoryComDifGet());
            if(value == 6) {
                work->koopaBit |= 1 << miniKoopaType;
            }
            break;

        case 11:
            if(mbSingleStepGet() <= 5 && mbMasuDispCheck(value)) {
                omVibrate(playerNo, 20, 4, 4);
            }
            break;

        case 12:
            SingleLast5();
            break;
    }
    return 0;
}

static void SingleMicListener(HUMC_RESPONSE *resp)
{
    SINGLESAVEWORK *work = &singleSaveWork;
    if(resp->result != 0 || resp->nbrWords == 0) {
        work->micChar = -1;
    } else {
        work->micChar = resp->itemP[0];
        if(work->micChar > 5) {
            work->micChar = -1;
        }
    }
}

static void SingleFlagFlush(void)
{
    int playerNo = GwSystem.turnPlayerNo;
    SINGLESAVEWORK *work = &singleSaveWork;
    int i;
    int j;
    int num;
    int total;
    u32 *flagP;
    u32 *oldP;
    u32 flag[6];

    if(_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        return;
    }
    if(work->mgNum >= 3) {
        if(work->unk01 == 0) {
            GWSinglePrizeFlagSet(10);
        }
        if(work->mgEvenNum * 100 / work->mgNum >= 75) {
            GWSinglePrizeFlagSet(13);
        }
        if(work->mgOddNum * 100 / work->mgNum >= 75) {
            GWSinglePrizeFlagSet(14);
        }
        if((float)work->mgRankTotal / (float)work->mgNum >= 5.0f) {
            GWSinglePrizeFlagSet(15);
        }
        if((float)work->mgRankTotal / (float)work->mgNum <= 2.0f) {
            GWSinglePrizeFlagSet(16);
        }
        if(work->micNum != 0 && (u8)work->micHitNum == work->mgNum) {
            GWSinglePrizeFlagSet(19);
        }
        if(work->hist12[0] == 0 && work->hist12[1] == 0 && work->hist12[2] == 0) {
            GWSinglePrizeFlagSet(21);
        }
        if(mbPlayerCapsuleNumGet(playerNo) == 3) {
            GWSinglePrizeFlagSet(22);
        }
        if(work->unk10 == 0 && work->unk11 != 0) {
            GWSinglePrizeFlagSet(27);
        }
        if(work->masuNum[1] * 100 / work->mgNum >= 50) {
            GWSinglePrizeFlagSet(33);
        }
        if(work->masuNum[2] * 100 / work->mgNum >= 50) {
            GWSinglePrizeFlagSet(34);
        }
        if(work->masuNum[4] * 100 / work->mgNum >= 50) {
            GWSinglePrizeFlagSet(35);
        }
        num = 0;
        for(i = 0; i < 3; i++) {
            num += work->masuNum[i + 9];
        }
        if(num * 100 / work->mgNum >= 50) {
            GWSinglePrizeFlagSet(36);
        }
        if(work->masuNum[3] * 100 / work->mgNum >= 50) {
            GWSinglePrizeFlagSet(37);
        }
        if(work->masuNum[6] * 100 / work->mgNum >= 50) {
            GWSinglePrizeFlagSet(38);
        }
    }
    if(work->koopaBit == 7) {
        GWSinglePrizeFlagSet(7);
    }
    if(work->unk01 >= 10) {
        GWSinglePrizeFlagSet(9);
    }
    if(work->micHitNum != 0) {
        GWSinglePrizeFlagSet(17);
    }
    if(work->micNum != 0 && work->micMissF == 0) {
        GWSinglePrizeFlagSet(18);
    }
    if(work->mgNum >= 10) {
        GWSinglePrizeFlagSet(20);
    }
    if(work->unk18 != 0) {
        GWSinglePrizeFlagSet(23);
    }
    if(work->unk10 == 0 && work->unk18 >= 3) {
        GWSinglePrizeFlagSet(24);
    }
    if(work->unk10 >= 5) {
        GWSinglePrizeFlagSet(28);
    }
    if(work->unk10 >= 3) {
        if(work->unk17 == 0) {
            GWSinglePrizeFlagSet(29);
        } else if(work->unk16 == 0) {
            GWSinglePrizeFlagSet(30);
        }
    }
    for(i = 0; i < 3; i++) {
        if(GwCommon.singleBoardPlayNum[i] == 0) {
            break;
        }
    }
    if(GwCommon.singleBoardPlayNum[singleBoard] < 100) {
        GwCommon.singleBoardPlayNum[singleBoard]++;
        if(i < 3) {
            for(i = 0; i < 3; i++) {
                if(GwCommon.singleBoardPlayNum[i] == 0) {
                    break;
                }
            }
            if(i >= 3) {
                GWSinglePrizeFlagSet(48);
            }
        }
        total = 0;
        for(i = 0; i < 3; i++) {
            total += GwCommon.singleBoardPlayNum[i];
        }
        if(total == 10) {
            GWSinglePrizeFlagSet(49);
        } else if(total == 100) {
            GWSinglePrizeFlagSet(50);
        }
    }
    flagP = (u32 *)GwCommon.singleBoardFlag;
    oldP = (u32 *)singleBoardFlagOld;
    for(i = 0; i < 6; i++, flagP++, oldP++) {
        flag[i] = *flagP | *oldP;
    }
    if(memcmp(flag, GwCommon.singleBoardFlag, sizeof(flag)) != 0) {
        num = 0;
        for(i = 0; i < 6; i++) {
            for(j = 0; j < 32; j++) {
                if(flag[i] & (1 << j)) {
                    num++;
                }
            }
        }
        if(num >= 71) {
            GWSinglePrizeFlagSet(40);
        }
    }
    memcpy(GwCommon.singleBoardFlag, flag, sizeof(flag));
}

static void SingleMgRecordBackup(void)
{
    int i;
    for(i = 0; i < 8; i++) {
        singleMgRecordOld[i] = GwCommon.record[i];
    }
}

static void SingleMgRecordRestore(void)
{
    int i;
    for(i = 0; i < 8; i++) {
        GwCommon.record[i] = singleMgRecordOld[i];
    }
}

static void SingleMgRecordPrizeInit(void)
{
    int i;
    for(i = 0; i < 8; i++) {
        singleMgRecordPrize[i] = GwCommon.record[i];
    }
}

static void SingleMgRecordPrizeSet(void)
{
    int i;
    for(i = 0; i < 8; i++) {
        if(GwCommon.record[i] != singleMgRecordPrize[i]) {
            break;
        }
    }
    if(i < 8) {
        GWSinglePrizeFlagSet(5);
        GWSingleMgRecordNumSet(GWSingleMgRecordNumGet() + 1);
    }
}

static void SingleLast5(void)
{
    static u32 mesTbl[] = { 0x20006, 0x20007, 0x20008, 0x20008 };
    int playerNo = GwSystem.turnPlayerNo;
    OMOBJ *guide;
    s16 winNo;
    HuVecF pos;

    mbPlayerMotionSet(playerNo, 1, 0x40000001);
    mbCameraPlayerViewSetFast(playerNo, 0);
    mbPlayerPosReset(playerNo);
    if(mbWipeSpecialStatGet()) {
        mbWipeFadeIn();
    }
    mbPlayerPosGet(playerNo, &pos);
    pos.y += 100.0f;
    pos.z -= 100.0f;
    guide = mbGuideCreateFlag(&pos, (s8 *)guideLast5MotTbl, FALSE, TRUE, FALSE);
    mbGuideMotionNextSet(guide, 1);
    winNo = mbWinCreate(2, 0x2B002C, mbGuideSpeakerNoGet());
    mbWinTopInsertMesSet(mesTbl[singleBoard], 0);
    mbGuideMotionShiftSet(guide, 12, TRUE);
    mbWinWait(winNo);
    winNo = mbWinCreate(2, 0x2B002D, mbGuideSpeakerNoGet());
    mbGuideMotionShiftSet(guide, 6, TRUE);
    mbGuideMotionStop(guide);
    mbWinWait(winNo);
    mbGuideMotionSet(guide, 7, TRUE);
    HuPrcSleep(30);
    mbGuideEnd(guide, TRUE);
}

void mbSingleReturn(void)
{
    singleEndF = TRUE;
    mbExitReq();
    HuPrcSleep(-1);
}

void mbSingleReturnWrite(void)
{
    singleCancelF = TRUE;
    singleEndF = TRUE;
    mbExitReq();
    HuPrcSleep(-1);
}

void mbSingleGameEnd(void)
{
    int playerNo = GwSystem.turnPlayerNo;
    GAMEMESID mesId;

    mbPauseDisableSet(TRUE);
    mbPlayerMotionSet(playerNo, 1, 0x40000001);
    mbCameraPlayerViewSetFast(playerNo, 0);
    mbPlayerPosReset(playerNo);
    if(mbWipeSpecialStatGet()) {
        mbWipeFadeIn();
    }
    mesId = GameMesCreate(6, 1);
    while(GameMesStatGet(mesId)) {
        HuPrcVSleep();
    }
    mbWipeSpecialCreate(1, 6, 90);
    mbMusFadeOutSpeed(0, 1000);
    mbWipeSpecialWait();
    mbWipeFadeOutTime(1);
    mbWipeSpecialKill();
    singleEndF = TRUE;
    mbExitReq();
    HuPrcSleep(-1);
}

void mbSingleSaveFlush(int mode)
{
    int playerNo = GwSystem.turnPlayerNo;
    int i;
    int j;
    int k;

    switch(mode) {
        case -1:
            for(i = 0; i < 8; i++) {
                GwCommon.record[i] = singleMgRecordOld[i];
            }
            GWMgCoinBonusSet(playerNo, -1);
            break;

        case 0:
            GWMgCoinBonusSet(playerNo, 0);
            break;

        case 1:
            for(k = 0; k < 4; k++) {
                for(j = 0; j < 32; j++) {
                    if(singleMgUnlock[k] & (1 << j)) {
                        GWMgUnlockSet((k * 32) + 0x259 + j);
                    }
                }
            }
            SingleFlagFlush();
            GWMgCoinBonusSet(playerNo, 1);
            break;
    }
}

int mbSingleStepGet(void)
{
    s16 masuId = GwPlayer[GwSystem.turnPlayerNo].masuId;
    return mbMasuFind_TypeStepGet(masuId, 7);
}

int mbSingleOppCharGet(void)
{
    return miniKoopaType + 11;
}

void mbSingleTeamCharSet(int charNo)
{
    singleTeamChar = charNo;
}

int mbSingleTeamCharGet(void)
{
    return singleTeamChar;
}

int mbSingleMgUnlockCheck(void)
{
    return SingleMgLockNumGet(-1, NULL) == 0;
}
/* @@END@@ */
