/* @@HEAD@@ */
/* capevent.h declares mbev_CapCall with an s16 capsule number for its callers (masu.c passes
   mbMasuCapsuleGet's s16 unconverted); the definition takes an int. */
#define mbev_CapCall mbev_CapCall_s16
#include "game/board/capevent.h"
#undef mbev_CapCall
int mbev_CapCall(int playerNo, int capsuleNo, BOOL trapF, BOOL moveF);
#include "game/board/capsule.h"
#include "game/board/capmove.h"
#include "game/board/capthrow.h"
#include "game/board/captrap.h"
#include "game/board/capspecial.h"
#include "game/board/shopevent.h"
#include "game/board/audio.h"
#define mbCameraMultiFocusSet mbCameraMultiFocusSet_void
#include "game/board/camera.h"
#undef mbCameraMultiFocusSet
void mbCameraMultiFocusSet();
#include "game/board/branch.h"
#include "game/board/coin.h"
#include "game/board/exit.h"
#define mbMasuCapsuleGet mbMasuCapsuleGet_s16
#include "game/board/masu.h"
#include "game/board/math.h"
#undef mbMasuCapsuleGet
int mbMasuCapsuleGet(s16 id);
#include "game/board/object.h"
#include "game/board/player.h"
#include "game/board/star.h"
#include "game/board/status.h"
#include "game/board/telop.h"
#include "game/board/window.h"
#include "game/gamework.h"
#include "game/memory.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/data.h"
#include "game/sprite.h"
#include "game/esprite.h"
#include "game/object.h"
#include "game/process.h"
#include "game/hu3d.h"
#include "game/hsfex.h"
#include "game/charman.h"
#include "game/main.h"
#include "game/window.h"
#include "humath.h"
#include "math.h"
#include "game/pad.h"
#include "game/printfunc.h"

#include "string.h"

/* Later-defined statics the early parts call. */
static void ev_CapCoinAdd(OMOBJ *eff, int playerNo, int coinNum, int arg3, void (*winFunc)(void));
static void ev_CapCall(CAPWORK *work, BOOL waitF);
static void ev_CapWorkInit(CAPOBJWORK *objWork, int capsuleNo);
static void ev_CapWorkClose(CAPOBJWORK *objWork);

static float ev_CapRotCamera(float rotY);
static HU3D_MODELID ev_CapEffCreate(ANIMDATA *anim, s16 num);
static void ev_CapEffGridSet(HU3D_MODELID modelId, int w, int h, int arg3);
static void ev_CapEffDraw(HU3D_MODEL *modelP, Mtx *mtx);
static void ev_CapComChoiceHook(void);

#define CAP_RANDF() (0.0000000037252903f*mbRandMod(0x10000000))

static inline float CapSinDeg(float deg)
{
    return mbSinDeg(deg);
}

static inline float CapCosDeg(float deg)
{
    return mbCosDeg(deg);
}

/* One particle of a CAPEV_EFFECT (ev_CapEffCreate's model). The unions carry the names the
   different effects use for the same per-particle words. Size 0x6C. */
typedef struct CapEvEffData_s {
    /* 0x00 */ union {
        s16 work[4];
        struct {
            s16 time;
            s16 maxTime;
            s16 count;
            s16 unk06;
        };
    };
    /* 0x08 */ union {
        struct {
            HuVecF vec1;
            HuVecF vec2;
            HuVecF vec3;
            float val1;
            float val2;
            float val3;
        };
        struct {
            HuVecF vel;
            float alpha;
            float alphaSpeed;
            float speed;
            float unk20;
            float gravity;
            float rotSpeed;
            u8 unk2C[0xC];
        };
    };
    /* 0x38 */ union {
        float animTime;
        float unk38;
    };
    /* 0x3C */ union {
        float animSpeed;
        float unk3C;
    };
    /* 0x40 */ float scale;
    /* 0x44 */ float scaleX;
    /* 0x48 */ float scaleY;
    /* 0x4C */ HuVecF rot;
    /* 0x58 */ HuVecF pos;
    /* 0x64 */ GXColor color;
    /* 0x68 */ union {
        int no;
        int unk68;
    };
} CAPEV_EFFDATA;

typedef struct CapEvEffect_s CAPEV_EFFECT;
typedef void (*CAPEV_EFFHOOK)(HU3D_MODEL *modelP, CAPEV_EFFECT *effP, Mtx *mtx);

/* ev_CapEffCreate's particle model (Hu3DData[].hookData). Size 0x60. */
struct CapEvEffect_s {
    /* 0x00 */ union {
        s16 mode;
        s16 unk00;
    };
    /* 0x02 */ union {
        s16 time;
        s16 unk02;
    };
    /* 0x04 */ union {
        u8 unk04[0x1C];
        struct {
            HuVecF vel;
            s16 work[8];
        };
    };
    /* 0x20 */ u8 blendMode;
    /* 0x21 */ union {
        u8 attr;
        u8 pat;
    };
    /* 0x22 */ union {
        u8 dispAttr;
        u8 unk22;
    };
    /* 0x23 */ u8 unk23;
    /* 0x24 */ union {
        s16 unk24;
        HU3D_MODELID modelId;
    };
    /* 0x26 */ s16 num;
    /* 0x28 */ union {
        u32 count;
        int unk28;
    };
    /* 0x2C */ union {
        u32 prevCounter;
        int unk2C;
    };
    /* 0x30 */ union {
        u32 prevCount;
        int unk30;
    };
    /* 0x34 */ u32 dlSize;
    /* 0x38 */ ANIMDATA *anim;
    /* 0x3C */ CAPEV_EFFDATA *data;
    /* 0x40 */ union {
        HuVecF *vertex;
        HuVecF *vtxBuf;
    };
    /* 0x44 */ union {
        HuVec2f *st;
        HuVec2f *stBuf;
    };
    /* 0x48 */ union {
        void *dl;
        void *dlBuf;
    };
    /* 0x4C */ union {
        CAPEV_EFFHOOK hook;
        int unk4C;
    };
    /* 0x50 */ union {
        HuVec2f *gridSt;
        int unk50;
    };
    /* 0x54 */ u32 unk54;
    /* 0x58 */ union {
        int gridNum;
        int unk58;
    };
    /* 0x5C */ union {
        HU3D_MODEL *hookMdlP;
        int unk5C;
    };
};

#define CAPEV_EFF_ATTR_COUNTER_RESET (1 << 0)
#define CAPEV_EFF_ATTR_COUNTER_UPDATE (1 << 1)
#define CAPEV_EFF_DISPATTR_ZBUF_OFF (1 << 0)
#define CAPEV_EFF_DISPATTR_NOANIM (1 << 1)
#define CAPEV_EFF_DISPATTR_CAMERA_ROT (1 << 2)
#define CAPEV_EFF_DISPATTR_ROT3D (1 << 3)
#define CAPEV_EFF_DISPATTR_SCALEXY (1 << 4)
#define CAPEV_EFF_DISPATTR_ST_DONE (1 << 5)
#define CAPEV_EFF_DISPATTR_CULL_OFF (1 << 6)

/* The OMOBJ work of the single-model effects (explode, boost, snow, glow, ...). */
typedef struct CapEffWork_s {
    /* 0x00 */ int mdlId;
    /* 0x04 */ int num;
    /* 0x08 */ int no;
    /* 0x0C */ ANIMDATA *anim;
} CAPEFFWORK;

typedef struct CapEffRingWork_s {
    /* 0x00 */ int mdlId[3];
    /* 0x0C */ union {
        int num;
        int dispNum;
    };
    /* 0x10 */ int no;
    /* 0x14 */ ANIMDATA *anim[3];
} CAPEFFRINGWORK;

typedef struct CapEffCoinData_s {
    /* 0x00 */ int coinId;
    /* 0x04 */ int no;
    /* 0x08 */ union {
        int activeF;
        BOOL dispF;
    };
    /* 0x0C */ int type;
    /* 0x10 */ int time;
    /* 0x14 */ int count;
    /* 0x18 */ float rotSpeed;
    /* 0x1C */ float gravity;
    /* 0x20 */ float maxY;
    /* 0x24 */ float alpha;
    /* 0x28 */ float alphaSpeed;
    /* 0x2C */ HuVecF pos;
    /* 0x38 */ HuVecF rot;
    /* 0x44 */ HuVecF scale;
    /* 0x50 */ HuVecF vel;
    /* 0x5C */ OMOBJ *glowEff;
} CAPEFFCOINDATA;

/* mbev_CapObjMotionSet / mbev_CapPlayerMotionSet process work. */
typedef struct CapMotWork_s {
    /* 0x00 */ int playerNo;
    /* 0x04 */ int objId;
    /* 0x08 */ int delay;
    /* 0x0C */ int motNo;
    /* 0x10 */ int nextMotNo;
    /* 0x14 */ u32 attr;
    /* 0x18 */ u32 nextAttr;
    /* 0x1C */ BOOL shiftF;
    /* 0x20 */ BOOL nextShiftF;
} CAPMOTWORK;

static inline void *CapMalloc(s32 size)
{
    return HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
}

static inline void CapFree(void *ptr)
{
    HuMemDirectFree(ptr);
}

static CAPEVENT_DATA ev_CapsuleData[60] = {
    { mbev_CapKinoko, mbev_CapKinokoKill, NULL, 2, 0, -1, 0 },
    { mbev_CapSKinoko, mbev_CapSKinokoKill, NULL, 2, 0, -1, 0 },
    { mbev_CapPKinoko, mbev_CapPKinokoKill, NULL, 2, 0, -1, 0 },
    { mbev_CapMKinoko, mbev_CapMKinokoKill, NULL, 2, 0, -1, 0 },
    { mbev_CapKiller, mbev_CapKillerKill, NULL, 2, 0, -1, 0 },
    { mbev_CapDokan, mbev_CapDokanKill, NULL, 2, 0, 0xD0000, 0 },
    { mbev_CapHanachan, mbev_CapHanachanKill, NULL, 2, 0, 0xD0000, 0 },
    { mbev_CapNKinoko, mbev_CapNKinokoKill, NULL, 2, 0, -1, 0 },
    { NULL, NULL, NULL, 0, 0, -1, 0 },
    { NULL, NULL, NULL, 0, 0, -1, 0 },
    { mbev_CapTogezo, mbev_CapTogezoKill, NULL, 1, 1, 0xF0000, 0 },
    { mbev_CapKuribo, mbev_CapKuriboKill, NULL, 1, 1, 0xF0000, 0 },
    { mbev_CapPakkun, mbev_CapPakkunKill, NULL, 1, 2, 0xF0000, 0 },
    { mbev_CapJango, mbev_CapJangoKill, NULL, 1, 1, 0xF0000, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { mbev_CapKokamekku, mbev_CapKokamekkuKill, NULL, 1, 1, 0xF0000, 0 },
    { mbev_CapKamekku, mbev_CapKamekkuKill, NULL, 1, 1, 0xF0000, 0 },
    { mbev_CapThrowman, mbev_CapThrowmanKill, NULL, 1, 1, 0xF0000, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { mbev_CapBoble, mbev_CapBobleKill, (void (*)(CAPWORK *))mbev_CapBobleTrap, 1, 1, -1, 1 },
    { mbev_CapBiriQ, mbev_CapBiriQKill, (void (*)(CAPWORK *))mbev_CapBiriQTrap, 0, 1, -1, 1 },
    { mbev_CapTumujikun, mbev_CapTumujikunKill, (void (*)(CAPWORK *))mbev_CapTumujikunTrap, 0, 1, -1, 1 },
    { mbev_CapDossun, mbev_CapDossunKill, (void (*)(CAPWORK *))mbev_CapDossunTrap, 1, 1, -1, 1 },
    { mbev_CapBomhei, mbev_CapBomheiKill, (void (*)(CAPWORK *))mbev_CapBomheiTrap, 1, 1, -1, 1 },
    { mbev_CapPatapata, mbev_CapPatapataKill, NULL, 1, 1, 0xF0000, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { mbev_CapNull, mbev_CapNullKill, NULL, 0, 1, -1, 0 },
    { mbev_CapNull, mbev_CapNullKill, NULL, 1, 1, -1, 0 },
    { mbev_CapNull, mbev_CapNullKill, NULL, 1, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { mbev_CapKillerMove, mbev_CapKillerMoveKill, NULL, 2, 1, -1, 0 },
    { mbev_CapKettou, mbev_CapKettouKill, NULL, 0, 1, 0x110000, 0 },
    { mbev_CapMiracle, mbev_CapMiracleKill, NULL, 0, 1, 0x110000, 0 },
    { mbev_CapKoopa, mbev_CapKoopaKill, NULL, 2, 0, 0xE0000, 0 },
    { mbev_CapDonkey, mbev_CapDonkeyKill, NULL, 0, 1, 0xE0000, 0 },
    { mbev_CapNull, mbev_CapNullKill, NULL, 0, 1, -1, 0 },
    { mbev_CapTeresa, mbev_CapTeresaKill, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { mbev_CapDebugCam, mbev_CapDebugCamKlll, NULL, 0, 1, -1, 0 },
    { mbev_CapDebugWarp, mbev_CapDebugWarpKill, NULL, 0, 1, -1, 0 },
    { mbev_CapDebugPosSelect, mbev_CapDebugPosSelectKill, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
    { NULL, NULL, NULL, 0, 1, -1, 0 },
};

/* Unreferenced in this unit. */
static HuVecF ev_CapsuleUnusedOfs[10] = {
    { 0.0f, -10.0f, -20.0f },
    { 0.0f, -10.0f, -20.0f },
    { 0.0f, -30.000002f, -20.0f },
    { 0.0f, -10.0f, -20.0f },
    { 0.0f, -10.0f, -20.0f },
    { 0.0f, 10.0f, -10.0f },
    { 0.0f, -30.000002f, -20.0f },
    { 0.0f, -10.0f, -20.0f },
    { 0.0f, -10.0f, -20.0f },
    { 0.0f, -10.0f, -20.0f },
};

static GXColor ev_CapsuleRandomColorTbl[7] = {
    { 255, 127, 127, 255 },
    { 255, 127, 64, 255 },
    { 255, 255, 127, 255 },
    { 127, 255, 127, 255 },
    { 127, 127, 255, 255 },
    { 64, 64, 255, 255 },
    { 255, 127, 255, 255 },
};

static HuVecF ev_CapsuleViewOfs = { 0.0f, 100.0f, 0.0f };

/* Defined at the end of PART1 (after mbev_CapDebugCam's strings in .data, after "%d" in .sdata). */
static s32 ev_CapEffRingFile[6];
static GXColor ev_CapEffElectricColor[4];
static HuVecF basePos[4];
static HuVec2f baseST1[4];
static HuVec2f baseST2[4];
static GXColor capsuleRingColor;

static int kettouCoinLose = 10;
static int kettouOppCoinLose = 5;
static int capsuleMasuType = -1;
static int capsuleMasuId = -1;
static int capsulePlayer = -1;
static int capsuleMasuPlayer = -1;
static int capsuleEventMasu = -1;
static int capsuleEventPlayer = -1;
static int capsuleEventPrevMasu = -1;
static int capsuleEventPrevPlayer = -1;

static HUPROCESS *ev_CapBonusCoinProc[4];
static char ev_CapBonusCoinMes[16];
static HUPROCESS *ev_CapMainProc[8];
static void *ev_CapEffExplodeOMObj[8];
static void *ev_CapEffBoostOMObj[8];
static void *ev_CapEffSnowOMObj[8];
static void *ev_CapEffGlowOMObj[8];
static void *ev_CapEffRingOMObj[8];
static void *ev_CapEffElectricOMObj[8];
static void *ev_CapEffRayOMObj[8];
static void *ev_CapEffMasuHitOMObj[8];
static void *ev_CapEffCoinOMObj[8];
static void *ev_CapEffCoinManOMObj[8];
static void *ev_CapEffStarManOMObj[8];
static void *ev_CapEffCapLoseOMObj[8];
static void *ev_CapEffMoveOMObj[8];

/* .sbss comes out in reverse definition order. */
static int capsuleChoice;
static int ev_CapKillFlag; /* lbl_802C0FD8: cleared by ev_CapKill, read nowhere */
static int bonusCoinNum;
static int bonusCoinWinId;
static CAPHOOK capsuleHook;
s16 *mbCapEffData;
u32 mbCapEffNum;
static ANIMDATA *electricEffAnim;
static ANIMDATA *ringHitEffAnim2;
static ANIMDATA *ringHitEffAnim1;
static ANIMDATA *boostEffAnim;
static int biriQMasuNum;

/* @@PART1@@ */
int mbev_CapCall(int playerNo, int capsuleNo, BOOL trapF, BOOL moveF)
{
    CAPWORK work;
    int viewNo;
    int masuPlayer;
    int masuId;
    BOOL moveNumF = FALSE;

    masuId = GwPlayer[playerNo].masuId;
    if(trapF) {
        masuPlayer = mbCapValuePlayerGet(capsuleNo);
        capsuleNo = mbCapValueTypeGet(capsuleNo);
        if(!mbCapValidCheck(capsuleNo)) {
            return FALSE;
        }
    } else if(moveF) {
        if(playerNo == capsuleEventPlayer && capsuleEventMasu == GwPlayer[playerNo].masuId) {
            capsuleEventMasu = -1;
            capsuleEventPlayer = -1;
            return FALSE;
        }
        capsuleEventMasu = -1;
        capsuleEventPlayer = -1;
        if(playerNo == capsulePlayer && capsuleMasuId == GwPlayer[playerNo].masuId) {
            capsuleNo = (capsuleMasuPlayer << 8) | capsuleMasuType;
        }
        capsuleMasuType = -1;
        capsuleMasuId = -1;
        capsulePlayer = -1;
        capsuleMasuPlayer = -1;
        if(mbMasuTypeGet(masuId) != 1 && mbMasuTypeGet(masuId) != 2) {
            return FALSE;
        }
        masuPlayer = mbCapValuePlayerGet(capsuleNo);
        capsuleNo = mbCapValueTypeGet(capsuleNo);
        if(playerNo < 0 || playerNo >= 4 || !mbCapValidCheck(capsuleNo)) {
            return FALSE;
        }
        if(mbCapUseModeGet(capsuleNo) != 2) {
            return FALSE;
        }
        if(mbev_CapPlayerCheck(masuPlayer, playerNo)) {
            return TRUE;
        }
    } else {
        if(playerNo == capsuleEventPrevPlayer && capsuleEventPrevMasu == GwPlayer[playerNo].masuId) {
            capsuleEventPrevMasu = -1;
            capsuleEventPrevPlayer = -1;
            return FALSE;
        }
        capsuleEventPrevMasu = -1;
        capsuleEventPrevPlayer = -1;
        if(mbMasuTypeGet(masuId) != 1 && mbMasuTypeGet(masuId) != 2) {
            return FALSE;
        }
        masuPlayer = mbCapValuePlayerGet(capsuleNo);
        capsuleNo = mbCapValueTypeGet(capsuleNo);
        if(playerNo < 0 || playerNo >= 4 || !mbCapValidCheck(capsuleNo)) {
            return FALSE;
        }
        if(mbCapUseModeGet(capsuleNo) == 2) {
            return FALSE;
        }
        if(!trapF && !moveF && mbCapValidCheck(capsuleNo)) {
            GwPlayer[playerNo].capsuleMasuNum++;
            if(GwPlayer[playerNo].capsuleMasuNum > 99) {
                GwPlayer[playerNo].capsuleMasuNum = 99;
            }
        }
        if(mbev_CapPlayerCheck(masuPlayer, playerNo)) {
            return TRUE;
        }
    }
    omVibrate(playerNo, 20, 4, 4);
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = capsuleNo;
    work.trapF = trapF;
    work.unk1C = 0;
    work.moveF = moveF;
    work.masuId = GwPlayer[work.playerNo].masuId;
    work.trapMasuId = -1;
    if(!trapF) {
        work.masuPlayer = masuPlayer;
    }
    memset(&work.flag, 0, 4);
    masuId = GwPlayer[work.playerNo].masuId;
    viewNo = mbCameraPlayerViewNoGet();
    mbAudFXPlay(1035);
    if(!trapF && !moveF) {
        mbev_CapEffOpenCreate(work.playerNo, work.masuId, TRUE, TRUE, TRUE);
    }
    if(ev_CapsuleData[capsuleNo].trapFunc != NULL) {
        ev_CapWorkInit(&work.objWork, -1);
        ev_CapsuleData[capsuleNo].trapFunc(&work);
        ev_CapWorkClose(&work.objWork);
    }
    if(moveF && ev_CapsuleData[work.capsuleNo].trapF && mbCapUseModeGet(capsuleNo) == 2) {
        if((capsuleNo == 22 || capsuleNo == 25) && GwPlayer[playerNo].moveNum <= 1) {
            mbev_CapMoveMasuSet(playerNo, GwPlayer[playerNo].masuId);
        }
        return TRUE;
    }
    switch(ev_CapsuleData[work.capsuleNo].cameraMode) {
        case 1:
            mbCameraPlayerViewSet(work.playerNo, 0);
            break;

        case 2:
            mbCameraPlayerViewSet(work.playerNo, 1);
            break;
    }
    switch(ev_CapsuleData[work.capsuleNo].statusMode) {
        case 0:
            if(!mbev_CapStatusDispCheck(work.playerNo)) {
                mbev_CapStatusDispSetAll(0, 1);
                mbStatusDispFocusSet(work.playerNo, TRUE);
                while(!mbStatusMoveCheck(work.playerNo)) {
                    HuPrcVSleep();
                }
            }
            break;

        case 1:
            mbev_CapStatusDispSetAll(1, 0);
            break;

        case 2:
            mbev_CapStatusDispSetAll(0, 0);
            break;
    }
    mbPlayerRotateStart(work.playerNo, 0, 15);
    ev_CapCall(&work, TRUE);
    if(moveNumF) {
        mbMoveNumDispSet(work.playerNo, TRUE);
    }
    mbev_CapStatusDispSetAll(1, 1);
    if(!moveF) {
        mbev_CapCameraViewSet(work.playerNo, viewNo, moveF);
    } else if(GwPlayer[work.playerNo].moveNum > 1) {
        mbCameraPlayerViewSet(work.playerNo, 2);
    } else {
        mbCameraPlayerViewSet(work.playerNo, 0);
    }
    if(masuId != GwPlayer[work.playerNo].masuId || work.flag.unk80F) {
        return FALSE;
    }
    return FALSE;
}

int mbev_CapCallTrap(int playerNo, int masuId, int trapMasuId)
{
    CAPWORK work;
    int masuPlayer = mbCapValuePlayerGet(mbMasuCapsuleGet(trapMasuId));
    if(mbCapMasuDispTypeGet(trapMasuId) != 2) {
        return FALSE;
    }
    if(mbev_CapPlayerCheck(masuPlayer, playerNo)) {
        return TRUE;
    }
    mbAudFXPlay(1035);
    if(GwPlayer[playerNo].biriQF) {
        biriQMasuNum = 60;
    }
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = mbCapMasuTypeGet(trapMasuId);
    work.trapF = TRUE;
    work.unk1C = TRUE;
    work.moveF = FALSE;
    work.masuId = masuId;
    work.trapMasuId = trapMasuId;
    mbev_CapEffOpenCreate(work.playerNo, work.trapMasuId, TRUE, FALSE, FALSE);
    if(ev_CapsuleData[work.capsuleNo].trapF) {
        ev_CapCall(&work, FALSE);
    }
    return TRUE;
}

void mbev_CapCallKettou(int playerNo, int masuId, int kettouF)
{
    CAPWORK work;
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = 41;
    work.trapF = FALSE;
    work.unk1C = FALSE;
    work.moveF = FALSE;
    work.masuId = masuId;
    work.trapMasuId = -1;
    work.flag.kettouF = kettouF;
    HuPrcSleep(10);
    mbAudFXPlay(1104);
    ev_CapCall(&work, TRUE);
}

void mbev_CapCircuitCallKettou(void)
{
}

void mbev_CapCallDonkey(int playerNo)
{
    CAPWORK work;
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = 44;
    work.trapF = FALSE;
    work.unk1C = FALSE;
    work.moveF = FALSE;
    work.masuId = GwPlayer[playerNo].masuId;
    work.trapMasuId = -1;
    omVibrate(playerNo, 20, 7, 3);
    ev_CapCall(&work, TRUE);
}

void mbev_CapCallKoopa(int playerNo)
{
    CAPWORK work;
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = 43;
    work.trapF = FALSE;
    work.unk1C = FALSE;
    work.moveF = FALSE;
    work.masuId = GwPlayer[playerNo].masuId;
    work.trapMasuId = -1;
    ev_CapCall(&work, TRUE);
}

void MBCapsuleStub5(void)
{
}

void MBCapsuleStub6(void)
{
}

void MBCapsuleStub7(void)
{
}

void mbev_CapCallTeresa(int playerNo, int masuId)
{
    CAPWORK work;
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = 46;
    work.trapF = FALSE;
    work.unk1C = FALSE;
    work.moveF = FALSE;
    work.masuId = masuId;
    work.trapMasuId = -1;
    ev_CapCall(&work, TRUE);
}

void mbev_CapCallMiracle(int playerNo, int masuId)
{
    CAPWORK work;
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = 42;
    work.trapF = FALSE;
    work.unk1C = FALSE;
    work.moveF = FALSE;
    work.masuId = masuId;
    work.trapMasuId = -1;
    ev_CapCall(&work, TRUE);
}

static void ev_CapBiriQShockOMExec(OMOBJ *obj);

void mbev_CapBiriQShockCreate(int playerNo)
{
    OMOBJ *obj;
    CAPWORK work;
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = 21;
    work.trapF = FALSE;
    work.unk1C = FALSE;
    work.moveF = FALSE;
    work.masuId = GwPlayer[playerNo].masuId;
    work.trapMasuId = -1;
    biriQMasuNum = 60;
    obj = omAddObjEx(mbObjMan, 32768, 0, 0, -1, ev_CapBiriQShockOMExec);
    mbev_CapBiriQMetalShock(&work);
    biriQMasuNum = 0;
}

int mbev_CapBiriQShockDelayGet(int playerNo)
{
    if(GwPlayer[playerNo].biriQF) {
        return biriQMasuNum;
    } else {
        return 0;
    }
}

static void ev_CapBiriQShockOMExec(OMOBJ *obj)
{
    if(mbExitCheck() || --biriQMasuNum <= 0) {
        biriQMasuNum = 0;
        omDelObjEx(mbObjMan, obj);
        return;
    }
}

static void ev_CapBiriQMetalShock(void);
static void ev_CapBiriQMetalShockDestroy(void);

void mbev_CapBiriQMetalShockCreate(int playerNo)
{
    HUPROCESS *proc;
    void *mem;
    CAPWORK work;
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = 21;
    work.trapF = FALSE;
    work.unk1C = FALSE;
    work.moveF = FALSE;
    work.masuId = GwPlayer[playerNo].masuId;
    work.trapMasuId = -1;
    work.flag.shockNoWaitF = TRUE;
    biriQMasuNum = 0;
    proc = HuPrcChildCreate(ev_CapBiriQMetalShock, 8196, 24576, 0, mbMainProc);
    mem = HuMemDirectMallocNum(HEAP_HEAP, sizeof(CAPWORK), HU_MEMNUM_OVL);
    proc->property = mem;
    memcpy(proc->property, &work, sizeof(CAPWORK));
    HuPrcDestructorSet2(proc, ev_CapBiriQMetalShockDestroy);
}

static void ev_CapBiriQMetalShock(void)
{
    CAPWORK *work = HuPrcCurrentGet()->property;
    mbev_CapBiriQMetalShock(work);
    HuPrcEnd();
}

static void ev_CapBiriQMetalShockDestroy(void)
{
    CAPWORK *work = HuPrcCurrentGet()->property;
    HuMemDirectFree(work);
}

void mbev_CapRandomBonusCoin(int playerNo, int capsuleNo, BOOL waitF)
{
    mbev_CapBonusCoinCall(playerNo, capsuleNo, -1, waitF);
}

void mbev_CapBonusCoinCall(int playerNo, int capsuleNo, int coinNum, BOOL waitF)
{
    int unused = 0;
    int num = 0;
    BOOL partyF = GwSystem.partyF;
    if(!partyF || _CheckFlag(FLAG_BOARD_TUTORIAL)) {
        return;
    }
    if(coinNum > 0) {
        mbev_CapBonusCoin(playerNo, coinNum, waitF, TRUE);
    } else if(coinNum != 0) {
        num = mbCapBonusCoinNumGet(playerNo, capsuleNo);
        if(num > 0) {
            mbev_CapBonusCoin(playerNo, num, waitF, TRUE);
        }
    }
}

typedef struct CapBonusCoinWork_s {
    int playerNo;
    int coinNum;
    int arg3;
} CAPBONUSCOINWORK;

static void ev_CapBonusCoin(void);
static void ev_CapBonusCoinKill(void);
static void ev_CapBonusCoinWin(void);

void mbev_CapBonusCoin(int playerNo, int coinNum, BOOL waitF, int arg3)
{
    HUPROCESS *proc;
    CAPBONUSCOINWORK *work;
    proc = ev_CapBonusCoinProc[playerNo] = HuPrcChildCreate(ev_CapBonusCoin, 8196, 24576, 0, mbMainProc);
    HuPrcDestructorSet2(proc, ev_CapBonusCoinKill);
    work = proc->property = CapMalloc(sizeof(CAPBONUSCOINWORK));
    memset(work, 0, sizeof(CAPBONUSCOINWORK));
    work->playerNo = playerNo;
    work->coinNum = coinNum;
    work->arg3 = arg3;
    if(waitF) {
        while(!mbev_CapBonusCoinCheck(playerNo)) {
            HuPrcVSleep();
        }
    }
}

BOOL mbev_CapBonusCoinCheck(int playerNo)
{
    if(ev_CapBonusCoinProc[playerNo] != NULL) {
        return FALSE;
    } else {
        return TRUE;
    }
}

static void ev_CapBonusCoin(void)
{
    HUPROCESS *proc = HuPrcCurrentGet();
    CAPBONUSCOINWORK *work = proc->property;
    void *eff;
    bonusCoinNum = work->coinNum;
    bonusCoinWinId = -1;
    eff = mbev_CapEffCoinCreate();
    ev_CapCoinAdd(eff, work->playerNo, work->coinNum, work->arg3, ev_CapBonusCoinWin);
    mbev_CapEffCoinKill(eff);
    if(bonusCoinWinId >= 0) {
        mbWinWait(bonusCoinWinId);
    }
    bonusCoinWinId = -1;
    HuPrcEnd();
}

static void ev_CapBonusCoinKill(void)
{
    HUPROCESS *proc = HuPrcCurrentGet();
    CAPBONUSCOINWORK *work = proc->property;
    ev_CapBonusCoinProc[work->playerNo] = NULL;
    HuMemDirectFree(work);
}

static void ev_CapBonusCoinWin(void)
{
    bonusCoinWinId = mbWinCreate(2, 0x370039, -1);
    sprintf(ev_CapBonusCoinMes, "%d", bonusCoinNum);
    mbWinTopInsertMesSet((u32)ev_CapBonusCoinMes, 0);
}

void mbev_CapMoveMasuSet(int playerNo, int masuId)
{
    capsuleEventPlayer = playerNo;
    capsuleEventMasu = masuId;
}

void mbev_CapStopMasuSet(int playerNo, int masuId)
{
    capsuleEventPrevPlayer = playerNo;
    capsuleEventPrevMasu = masuId;
}

void mbev_CapKettouEndCall(int playerNo)
{
    CAPWORK work;
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = 41;
    work.trapF = FALSE;
    work.unk1C = FALSE;
    work.moveF = FALSE;
    work.masuId = GwPlayer[playerNo].masuId;
    work.trapMasuId = -1;
    work.flag.kettouEndF = TRUE;
    mbPlayerColSnapSet(TRUE);
    ev_CapCall(&work, TRUE);
}

void mbev_CapDonkeyEndCall(int playerNo)
{
    CAPWORK work;
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = 44;
    work.trapF = FALSE;
    work.unk1C = FALSE;
    work.moveF = FALSE;
    work.masuId = GwPlayer[playerNo].masuId;
    work.trapMasuId = -1;
    work.flag.donkeyEndF = TRUE;
    mbPlayerColSnapSet(TRUE);
    ev_CapCall(&work, TRUE);
}

void mbev_CapKoopaEndCall(int playerNo)
{
    CAPWORK work;
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = 43;
    work.trapF = FALSE;
    work.unk1C = FALSE;
    work.moveF = FALSE;
    work.masuId = GwPlayer[playerNo].masuId;
    work.trapMasuId = -1;
    work.flag.koopaEndF = TRUE;
    mbPlayerColSnapSet(TRUE);
    ev_CapCall(&work, TRUE);
}

void mbev_CapVsEndCall(void)
{
}

void mbev_CapKillerCall(void)
{
}

void mbev_CapKillerMultiCall(void)
{
}

void mbev_CapKillerMoveCall(int playerNo)
{
    CAPWORK work;
    memset(&work, 0, sizeof(CAPWORK));
    work.playerNo = playerNo;
    work.masuPlayer = -1;
    work.capsuleNo = 40;
    work.trapF = FALSE;
    work.unk1C = FALSE;
    work.moveF = TRUE;
    work.masuId = GwPlayer[playerNo].masuId;
    work.trapMasuId = -1;
    ev_CapCall(&work, TRUE);
}

void MBCapsuleStub11(void)
{
}

void MBCapsuleStub12(void)
{
}

int mbev_CapKillerMoveCheck(void)
{
    return TRUE;
}

int mbev_CapKillerMoveCheckAll(void)
{
    return TRUE;
}

int mbev_CapCapGet(void)
{
    return -1;
}

u16 mbev_CapBankCoinGet(void)
{
    return GwSystem.bankCoin;
}

int mbev_CapKettouCoinLoseGet(void)
{
    return kettouCoinLose;
}

int mbev_CapKettouOppCoinLoseGet(void)
{
    return kettouOppCoinLose;
}

int mbev_CapKettouCoinLoseGet2(void)
{
    return kettouCoinLose;
}

int mbev_CapKettouOppCoinLoseGet2(void)
{
    return kettouOppCoinLose;
}

void mbev_CapOpeningAdd(void)
{
}

void mbev_CapKoopaAdd(void)
{
}

void mbev_CapBubbleHookSet(CAPHOOK hook)
{
    capsuleHook = hook;
}

void mbev_CapBubbleHookCall(int arg1, int arg2, int arg3, int arg4, int arg5)
{
    if(capsuleHook != NULL) {
        capsuleHook(-1, arg1, arg2, arg3, arg4, arg5);
    }
}

void mbev_CapBubbleHookCallStory(int type, int arg1, int arg2)
{
    if(capsuleHook != NULL) {
        switch(type) {
            case 0:
                capsuleHook(43, arg1, arg2, 0, 0, 0);
                break;

            case 1:
                capsuleHook(-99, arg1, arg2, 0, 0, 0);
                break;
        }
    }
}

void mbev_CapBankCoinInit(void)
{
    GwSystem.bankCoin = 0;
}

void mbev_CapInit(void)
{
    int i;
    for(i = 0; i < 8; i++) {
        ev_CapMainProc[i] = NULL;
    }
    for(i = 0; i < 8; i++) {
        ev_CapEffExplodeOMObj[i] = NULL;
        ev_CapEffBoostOMObj[i] = NULL;
        ev_CapEffSnowOMObj[i] = NULL;
        ev_CapEffGlowOMObj[i] = NULL;
        ev_CapEffRingOMObj[i] = NULL;
        ev_CapEffElectricOMObj[i] = NULL;
        ev_CapEffRayOMObj[i] = NULL;
        ev_CapEffMasuHitOMObj[i] = NULL;
        ev_CapEffCoinOMObj[i] = NULL;
        ev_CapEffCoinManOMObj[i] = NULL;
        ev_CapEffStarManOMObj[i] = NULL;
        ev_CapEffCapLoseOMObj[i] = NULL;
    }
    mbCapListRead();
    capsuleHook = NULL;
    mbev_ShopEnableSet(FALSE);
    mbCapThrowColCreate(-1);
    mbCapThrowHookSet(NULL);
    mbev_CapTeresaStealSet(-1, 0, 0, 0);
    boostEffAnim = HuSprAnimRead(HuDataReadNum(0xC0035, HU_MEMNUM_OVL));
    HuSprAnimLock(boostEffAnim);
    ringHitEffAnim1 = HuSprAnimRead(HuDataReadNum(0xC0038, HU_MEMNUM_OVL));
    HuSprAnimLock(ringHitEffAnim1);
    ringHitEffAnim2 = HuSprAnimRead(HuDataReadNum(0xC0031, HU_MEMNUM_OVL));
    HuSprAnimLock(ringHitEffAnim2);
    electricEffAnim = HuSprAnimRead(HuDataReadNum(0xC0037, HU_MEMNUM_OVL));
    HuSprAnimLock(electricEffAnim);
    mbCapEffNum = 0;
    mbCapEffData = CapMalloc(1024 * sizeof(s16));
    for(i = 0; i < 1024; i++) {
        mbCapEffData[i] = frand() & 0x7FFF;
    }
    capsuleMasuType = -1;
    capsuleMasuId = -1;
    capsulePlayer = -1;
    capsuleMasuPlayer = -1;
    capsuleEventMasu = -1;
    capsuleEventPlayer = -1;
    capsuleEventPrevMasu = -1;
    capsuleEventPrevPlayer = -1;
    for(i = 0; i < 4; i++) {
        ev_CapBonusCoinProc[i] = NULL;
    }
}

static void ev_CapKill(void)
{
    HUPROCESS *proc = HuPrcCurrentGet();
    CAPWORK *work = proc->property;
    void (*killFunc)(void);
    if(work->capsuleNo != -1 && ev_CapsuleData[work->capsuleNo].killFunc != NULL) {
        killFunc = ev_CapsuleData[work->capsuleNo].killFunc;
        killFunc();
    }
    HuDataDirClose(0x130000);
    HuDataDirClose(0xD0000);
    HuDataDirClose(0xE0000);
    HuDataDirClose(0xF0000);
    HuDataDirClose(0x100000);
    HuDataDirClose(0x110000);
    ev_CapKillFlag = 0;
    ev_CapWorkClose(&work->objWork);
    if(!mbExitCheck()) {
        if(work->effExplode != NULL) {
            mbev_CapEffExplodeKill(work->effExplode);
        }
        if(work->effBoost != NULL) {
            mbev_CapEffBoostKill(work->effBoost);
        }
        if(work->effSnow != NULL) {
            mbev_CapEffSnowKill(work->effSnow);
        }
        if(work->effGlow != NULL) {
            mbev_CapEffGlowKill(work->effGlow);
        }
        if(work->effRing != NULL) {
            mbev_CapEffRingKill(work->effRing);
        }
        if(work->effCoin != NULL) {
            mbev_CapEffCoinKill(work->effCoin);
        }
        if(work->effCoinMan != NULL) {
            mbev_CapCoinManKill(work->effCoinMan);
        }
        if(work->effStarMan != NULL) {
            mbev_CapStarManKill(work->effStarMan);
        }
        if(work->effCapLose != NULL) {
            mbev_CapEffCapLoseKill(work->effCapLose);
        }
    }
    ev_CapMainProc[work->procNo] = NULL;
    _ClearFlag(FLAG_BOARD_STAR_RESET);
    HuMemDirectFree(work);
}

static inline BOOL ev_CapMainProcEndCheck(int procNo)
{
    if(ev_CapMainProc[procNo] != NULL) {
        return FALSE;
    } else {
        return TRUE;
    }
}

static void ev_CapCall(CAPWORK *work, BOOL waitF)
{
    int i;
    HUPROCESS *proc;
    CAPWORK *newWork;
    for(i = 0; i < 8; i++) {
        if(ev_CapMainProc[i] == NULL) {
            break;
        }
    }
    newWork = CapMalloc(sizeof(CAPWORK));
    memset(newWork, 0, sizeof(CAPWORK));
    newWork->playerNo = work->playerNo;
    newWork->masuPlayer = work->masuPlayer;
    newWork->capsuleNo = work->capsuleNo;
    newWork->trapF = work->trapF;
    newWork->unk1C = work->unk1C;
    newWork->moveF = work->moveF;
    newWork->masuId = work->masuId;
    newWork->trapMasuId = work->trapMasuId;
    newWork->procNo = i;
    memcpy(&newWork->flag, &work->flag, 4);
    ev_CapWorkInit(&newWork->objWork, newWork->capsuleNo);
    mbev_CapWait(newWork);
    if(ev_CapsuleData[work->capsuleNo].func != NULL) {
        proc = HuPrcChildCreate(ev_CapsuleData[work->capsuleNo].func, 8196, 24576, 0, mbMainProc);
        ev_CapMainProc[i] = proc;
        proc->property = newWork;
        HuPrcDestructorSet2(proc, ev_CapKill);
        if(waitF) {
            do {
                HuPrcVSleep();
            } while(!ev_CapMainProcEndCheck(newWork->procNo));
            while(mbMusBoardFadeCheck()) {
                HuPrcVSleep();
            }
        }
    }
}

void mbev_CapWait(CAPWORK *work)
{
    CAPOBJWORK *objWork = &work->objWork;
    if(objWork != NULL) {
        if(objWork->bgStatId >= 0) {
            mbBGReadWait(objWork->bgStatId);
            objWork->bgStatId = -1;
        }
        mbCameraMoveWait();
    }
}

static void ev_CapWorkOMExec(OMOBJ *obj);

static void ev_CapWorkInit(CAPOBJWORK *objWork, int capsuleNo)
{
    int i;
    int j;
    memset(objWork, 0, sizeof(CAPOBJWORK));
    for(i = 0; i < 64; i++) {
        for(j = 0; j < 4; j++) {
            objWork->playerMot[i][j] = -1;
        }
        objWork->objId[i] = -1;
        objWork->sprId[i] = -1;
        objWork->mem[i] = NULL;
        objWork->objMasuId[i] = -1;
        objWork->objOfs[i].x = objWork->objOfs[i].y = objWork->objOfs[i].z = 0.0f;
    }
    for(i = 0; i < 4; i++) {
        objWork->playerMasuId[i] = -1;
        objWork->playerOfs[i].x = objWork->playerOfs[i].y = objWork->playerOfs[i].z = 0.0f;
    }
    objWork->obj = omAddObjEx(mbObjMan, 32767, 0, 0, -1, ev_CapWorkOMExec);
    objWork->obj->data = objWork;
    if(capsuleNo >= 0) {
        if(capsuleNo < 0) {
            objWork->bgStatId = -1;
        } else if(ev_CapsuleData[capsuleNo].bgDataNum != -1) {
            objWork->bgStatId = mbBGRead(ev_CapsuleData[capsuleNo].bgDataNum);
        } else {
            objWork->bgStatId = -1;
        }
    }
}

static void ev_CapWorkClose(CAPOBJWORK *objWork)
{
    int i;
    int j;
    if(objWork != NULL) {
        if(objWork->obj != NULL) {
            objWork->obj->work[0] = 1;
        }
        for(i = 0; i < 64; i++) {
            if(!mbExitCheck()) {
                for(j = 0; j < 4; j++) {
                    if(objWork->playerMot[i][j] != -1) {
                        mbPlayerMotionKill(j, objWork->playerMot[i][j]);
                    }
                }
            }
            if(objWork->objId[i] != -1) {
                mbObjKill(objWork->objId[i]);
            }
            if(objWork->sprId[i] != -1) {
                espKill(objWork->sprId[i]);
            }
            if(objWork->mem[i] != NULL) {
                CapFree(objWork->mem[i]);
            }
        }
    }
}

static void ev_CapWorkOMExec(OMOBJ *obj)
{
    CAPOBJWORK *objWork = obj->data;
    HuVecF pos;
    int i;
    if(mbExitCheck() || obj->work[0]) {
        obj->data = NULL;
        omDelObjEx(mbObjMan, obj);
        return;
    }
    for(i = 0; i < 64; i++) {
        if(objWork->objId[i] != -1 && objWork->objMasuId[i] != -1) {
            mbMasuPosGet(objWork->objMasuId[i], &pos);
            VECAdd(&pos, &objWork->objOfs[i], &pos);
            mbObjPosSetV(objWork->objId[i], &pos);
        }
    }
    for(i = 0; i < 4; i++) {
        if(objWork->playerMasuId[i] != -1) {
            mbMasuPosGet(objWork->playerMasuId[i], &pos);
            VECAdd(&pos, &objWork->playerOfs[i], &pos);
            mbPlayerPosSetV(i, &pos);
        }
    }
}

s16 mbev_CapPlayerMotionCreate(CAPOBJWORK *objWork, int playerNo, int dataNum)
{
    int i;
    for(i = 0; i < 64; i++) {
        if(objWork->playerMot[i][playerNo] == -1) {
            break;
        }
    }
    if(i >= 64) {
        return -1;
    }
    objWork->playerMot[i][playerNo] = mbPlayerMotionCreate(playerNo, dataNum);
    HuPrcVSleep();
    return objWork->playerMot[i][playerNo];
}

int mbev_CapObjCreate(CAPOBJWORK *objWork, int dataNum, int *motDataNum, BOOL linkF, int sleepTime, BOOL dirCloseF)
{
    int i;
    int j;
    for(i = 0; i < 64; i++) {
        if(objWork->objId[i] == -1) {
            break;
        }
    }
    if(i >= 64) {
        return -1;
    }
    objWork->objId[i] = mbObjCreate(dataNum, NULL, linkF);
    if(dirCloseF) {
        HuDataDirClose(dataNum);
    }
    mbObjDispSet(objWork->objId[i], FALSE);
    if(sleepTime > 0) {
        HuPrcSleep(sleepTime);
    }
    if(objWork->objId[i] != -1 && motDataNum != NULL) {
        for(j = 0; motDataNum[j] != -1; j++) {
            mbObjMotionCreate(objWork->objId[i], motDataNum[j]);
            if(sleepTime > 0) {
                HuPrcVSleep();
            }
        }
    }
    mbObjDispSet(objWork->objId[i], TRUE);
    mbObjLayerSet(objWork->objId[i], 3);
    return objWork->objId[i];
}

void mbev_CapObjPosSet(CAPOBJWORK *objWork, int objId, int masuId, HuVecF *ofs)
{
    int i;
    for(i = 0; i < 64; i++) {
        if(objWork->objId[i] == objId) {
            objWork->objMasuId[i] = masuId;
            if(ofs != NULL) {
                objWork->objOfs[i] = *ofs;
            } else {
                objWork->objOfs[i].x = objWork->objOfs[i].y = objWork->objOfs[i].z = 0.0f;
            }
        }
    }
}

void mbev_CapPlayerPosSet(CAPOBJWORK *objWork, int playerNo, int masuId, HuVecF *ofs)
{
    objWork->playerMasuId[playerNo] = masuId;
    if(ofs != NULL) {
        objWork->playerOfs[playerNo] = *ofs;
    } else {
        objWork->playerOfs[playerNo].x = objWork->playerOfs[playerNo].y = objWork->playerOfs[playerNo].z = 0.0f;
    }
}

void mbev_CapObjClose(CAPOBJWORK *objWork, int objId)
{
    int i;
    if(objId != -1) {
        for(i = 0; i < 64; i++) {
            if(objWork->objId[i] == objId) {
                mbObjKill(objId);
                objWork->objId[i] = -1;
            }
        }
    }
}

s16 mbev_CapSprCreate(CAPOBJWORK *objWork, unsigned int dataNum, s16 prio, s16 bank)
{
    int i;
    for(i = 0; i < 64; i++) {
        if(objWork->sprId[i] == -1) {
            break;
        }
    }
    if(i >= 64) {
        return -1;
    }
    objWork->sprId[i] = espEntry(dataNum, prio, bank);
    espDrawNoSet(objWork->sprId[i], 32);
    return objWork->sprId[i];
}

void mbev_CapSprClose(CAPOBJWORK *objWork, s16 sprId)
{
    int i;
    if(sprId != -1) {
        for(i = 0; i < 64; i++) {
            if(objWork->sprId[i] == sprId) {
                espKill(sprId);
                objWork->sprId[i] = -1;
            }
        }
    }
}

void *mbev_CapMalloc(CAPOBJWORK *objWork, s32 size)
{
    int i;
    for(i = 0; i < 64; i++) {
        if(objWork->mem[i] == NULL) {
            break;
        }
    }
    if(i >= 64) {
        return NULL;
    }
    objWork->mem[i] = CapMalloc(size);
    return objWork->mem[i];
}

void mbev_CapMallocClose(CAPOBJWORK *objWork, void *ptr)
{
    int i;
    if(ptr != NULL) {
        for(i = 0; i < 64; i++) {
            if(objWork->mem[i] == ptr) {
                HuMemDirectFree(ptr);
                objWork->mem[i] = NULL;
            }
        }
    }
}

typedef struct CapEffOpenWork_s {
    /* 0x00 */ int masuId;
    /* 0x04 */ int time;
    /* 0x08 */ int openF;
    /* 0x0C */ HuVecF ofs;
} CAPEFFOPENWORK;

static void ev_CapEffOpen(void);
static void ev_CapEffOpenKill(void);

void mbev_CapEffOpenCreate(int playerNo, int masuId, BOOL effF, BOOL openF, BOOL keepF)
{
    HUPROCESS *proc;
    CAPEFFOPENWORK *work;
    int value;
    if(effF) {
        proc = HuPrcChildCreate(ev_CapEffOpen, 8196, 24576, 0, mbMainProc);
        HuPrcDestructorSet2(proc, ev_CapEffOpenKill);
        work = CapMalloc(sizeof(CAPEFFOPENWORK));
        proc->property = work;
        memset(work, 0, sizeof(CAPEFFOPENWORK));
        work->masuId = masuId;
        work->time = 25;
        work->ofs.x = work->ofs.y = work->ofs.z = 0.0f;
        if(!openF) {
            work->ofs.y += 100.0f;
            work->openF = FALSE;
        } else {
            work->openF = TRUE;
        }
    }
    if(openF) {
        if(!keepF) {
            mbMasuCapsuleSet(masuId, -1);
        }
    } else {
        value = mbMasuCapsuleGet(masuId);
        capsuleMasuType = mbCapValueTypeGet(value);
        capsuleMasuId = masuId;
        capsulePlayer = playerNo;
        capsuleMasuPlayer = mbCapValuePlayerGet(value);
        mbMasuCapsuleSet(masuId, -1);
    }
}

static void ev_CapEffOpen(void)
{
    HUPROCESS *proc = HuPrcCurrentGet();
    CAPEFFOPENWORK *work = proc->property;
    HuVecF pos;
    HuVecF effPos;
    HuVecF rot;
    HuVecF vel;
    HuVecF scale;
    GXColor color;
    void *rayEff;
    void *ringEff;
    void *masuHitEff;
    int i;
    int j;
    float alpha;

    rayEff = mbev_CapEffRayCreate(1.0f, 0.0f);
    ringEff = mbev_CapEffRingCreate();
    masuHitEff = mbev_CapEffMasuHitCreate();
    mbMasuPosGet(work->masuId, &pos);
    VECAdd(&pos, &work->ofs, &pos);
    scale.x = scale.z = 0.0f;
    scale.y = 5.0f;
    VECAdd(&pos, &scale, &pos);
    if(work->openF) {
        rot.x = -90.0f;
        rot.y = rot.z = 0.0f;
        scale.x = 1.0f;
        scale.y = 2.0f;
        scale.z = 100.0f;
        mbev_CapEffColorSet(&color, mbRandMod(32768));
        mbev_CapEffRingAdd(ringEff, pos, rot, scale, 1, 10, 0, color);
        scale.x = 2.0f;
        scale.y = 2.5f;
        scale.z = 200.0f;
        mbev_CapEffColorSet(&color, mbRandMod(32768));
        mbev_CapEffRingAdd(ringEff, pos, rot, scale, 3, 10, 1, color);
        for(i = 0; i <= work->time; i++) {
            alpha = HuCos(90.0f*((float)i/work->time));
            mbMasuPosGet(work->masuId, &pos);
            VECAdd(&pos, &work->ofs, &pos);
            for(j = 0; j < 3; j++) {
                effPos = pos;
                rot.x = 180.0f*(-0.5f+CAP_RANDF());
                rot.y = 360.0f*CAP_RANDF();
                rot.z = 180.0f*(-0.5f+CAP_RANDF());
                vel.x = 180.0f*(-0.5f+CAP_RANDF());
                vel.y = rot.y+(60.0f*CAP_RANDF());
                vel.z = 180.0f*(-0.5f+CAP_RANDF());
                mbev_CapEffRayAdd(rayEff, &effPos, &rot, &vel, 200.0f*(1.0f+(0.25f*CAP_RANDF())), 10.0f+(5.0f*CAP_RANDF()));
            }
            mbev_CapEffRayAlphaSet(rayEff, alpha);
            for(j = 0; j < 5.0f*alpha; j++) {
                effPos = pos;
                rot.x = 180.0f*(-0.5f+CAP_RANDF());
                rot.y = 360.0f*CAP_RANDF();
                rot.z = 180.0f*(-0.5f+CAP_RANDF());
                vel.x = 180.0f*(-0.5f+CAP_RANDF());
                vel.y = 360.0f*CAP_RANDF();
                vel.z = 180.0f*(-0.5f+CAP_RANDF());
                mbev_CapEffMasuHitAdd(masuHitEff, &effPos, &rot, &vel, 100.0f*(2.0f+CAP_RANDF()), alpha*(50.0f*(1.0f+(0.5f*CAP_RANDF()))), 10.0f+(5.0f*CAP_RANDF()));
            }
            HuPrcVSleep();
        }
    } else {
        mbCameraRotGet(&rot);
        scale.x = 1.0f;
        scale.y = 2.0f;
        scale.z = 100.0f;
        mbev_CapEffColorSet(&color, mbRandMod(32768));
        mbev_CapEffRingAdd(ringEff, pos, rot, scale, 1, 10, 0, color);
        scale.x = 2.0f;
        scale.y = 2.5f;
        scale.z = 200.0f;
        mbev_CapEffColorSet(&color, mbRandMod(32768));
        mbev_CapEffRingAdd(ringEff, pos, rot, scale, 3, 10, 1, color);
        for(i = 0; i <= work->time; i++) {
            alpha = HuCos(90.0f*((float)i/work->time));
            mbMasuPosGet(work->masuId, &pos);
            VECAdd(&pos, &work->ofs, &pos);
            for(j = 0; j < 3; j++) {
                effPos = pos;
                rot.x = 360.0f*CAP_RANDF();
                rot.y = 360.0f*CAP_RANDF();
                rot.z = 360.0f*CAP_RANDF();
                vel.x = rot.x+(60.0f*(-0.5f+CAP_RANDF()));
                vel.y = rot.y+(60.0f*(-0.5f+CAP_RANDF()));
                vel.z = rot.z+(60.0f*(-0.5f+CAP_RANDF()));
                mbev_CapEffRayAdd(rayEff, &effPos, &rot, &vel, 200.0f*(1.0f+(0.25f*CAP_RANDF())), 10.0f+(5.0f*CAP_RANDF()));
            }
            mbev_CapEffRayAlphaSet(rayEff, alpha);
            for(j = 0; j < 5.0f*alpha; j++) {
                effPos = pos;
                rot.x = 360.0f*CAP_RANDF();
                rot.y = 360.0f*CAP_RANDF();
                rot.z = 360.0f*CAP_RANDF();
                vel.x = 360.0f*(-0.5f+CAP_RANDF());
                vel.y = 360.0f*(-0.5f+CAP_RANDF());
                vel.z = 360.0f*(-0.5f+CAP_RANDF());
                mbev_CapEffMasuHitAdd(masuHitEff, &effPos, &rot, &vel, 100.0f*(2.0f+CAP_RANDF()), alpha*(50.0f*(1.0f+(0.5f*CAP_RANDF()))), 10.0f+(5.0f*CAP_RANDF()));
            }
            HuPrcVSleep();
        }
    }
    mbev_CapEffRayKill(rayEff);
    mbev_CapEffRingKill(ringEff);
    mbev_CapEffMasuHitKill(masuHitEff);
    HuPrcEnd();
}

static void ev_CapEffOpenKill(void)
{
    HUPROCESS *proc = HuPrcCurrentGet();
    CAPEFFOPENWORK *work = proc->property;
    HuMemDirectFree(work);
}

void mbev_CapNull(void)
{
    CAPWORK *work = HuPrcCurrentGet()->property;
    HuPrcEnd();
}

void mbev_CapNullKill(void)
{
}

void mbev_CapDebugCam(void)
{
    CAPWORK *work = HuPrcCurrentGet()->property;
    MBCAMERA *camera;
    HuVecF eye;
    HuVecF rot;
    HuVecF eyeOrig;
    HuVecF rotOrig;
    float zoomOrig;
    float zoom;
    float x;
    float y;
    float speed;
    int objId;
    int mode;
    int dispF;
    int frame;

    mbCapEffUseCreate(work->playerNo, work->capsuleNo);
    while(mbCapEffUseModeGet(work->playerNo) >= 0) {
        HuPrcVSleep();
    }
    mbCameraMoveWait();
    camera = mbCameraGet();
    eye.x = eyeOrig.x = camera->target.x+camera->offset.x;
    eye.y = eyeOrig.y = camera->target.y+camera->offset.y;
    eye.z = eyeOrig.z = camera->target.z+camera->offset.z;
    rot.x = rotOrig.x = camera->rot.x;
    rot.y = rotOrig.y = camera->rot.y;
    rot.z = rotOrig.z = camera->rot.z;
    zoom = zoomOrig = camera->zoom;
    mode = 0;
    dispF = FALSE;
    x = 32.0f;
    y = 50.0f;
    speed = 1.0f;
    frame = 0;
    objId = mbev_CapObjCreate(&work->objWork, 0xC0044, NULL, FALSE, 5, FALSE);
    mbObjPosSet(objId, eye.x, eye.y, eye.z);
    mbObjDispSet(objId, FALSE);
    mbCameraFocusObjSet(objId);
    do {
        if(HuPadBtnDown[0] & PAD_BUTTON_X) {
            mode++;
            if(mode > 2) {
                mode = 0;
            }
        }
        if(HuPadBtnDown[0] & PAD_BUTTON_Y) {
            dispF ^= TRUE;
        }
        frame++;
        mbObjDispSet(objId, dispF);
        switch(mode) {
            case 0:
                if(HuPadBtn[0] & PAD_TRIGGER_L) {
                    speed = 2.0f;
                } else if(HuPadBtn[0] & PAD_TRIGGER_R) {
                    speed = 0.5f;
                } else {
                    speed = 1.0f;
                }
                if(fabs(mbPadStkXGet(0)) > 8.0) {
                    eye.x += speed*(0.05f*mbPadStkXGet(0));
                }
                if(fabs(mbPadStkYGet(0)) > 8.0) {
                    eye.z -= speed*(0.05f*mbPadStkYGet(0));
                }
                if(HuPadBtn[0] & PAD_BUTTON_UP) {
                    eye.y += 25.0f*speed;
                }
                if(HuPadBtn[0] & PAD_BUTTON_DOWN) {
                    eye.y -= 25.0f*speed;
                }
                if(HuPadBtn[0] & PAD_BUTTON_A) {
                    zoom += 25.0f*speed;
                }
                if(HuPadBtn[0] & PAD_BUTTON_B) {
                    zoom -= 25.0f*speed;
                }
                if(HuPadBtnDown[0] & PAD_BUTTON_START) {
                    eye = eyeOrig;
                    rot = rotOrig;
                    zoom = zoomOrig;
                }
                fontcolor = 15;
                print8(36.0+x, 84.0+y, 1.5f, "\xFD\x01POSITION MODE");
                break;

            case 1:
                if(HuPadBtn[0] & PAD_TRIGGER_L) {
                    speed = 2.0f;
                } else if(HuPadBtn[0] & PAD_TRIGGER_R) {
                    speed = 0.5f;
                } else {
                    speed = 1.0f;
                }
                if(fabs(mbPadStkXGet(0)) > 8.0) {
                    rot.y += speed*(0.05f*mbPadStkXGet(0));
                }
                if(fabs(mbPadStkYGet(0)) > 8.0) {
                    rot.x += speed*(0.05f*mbPadStkYGet(0));
                }
                if(rot.x >= 360.0f) {
                    rot.x -= 360.0f;
                } else if(rot.x < 0.0f) {
                    rot.x += 360.0f;
                }
                if(rot.y >= 360.0f) {
                    rot.y -= 360.0f;
                } else if(rot.y < 0.0f) {
                    rot.y += 360.0f;
                }
                if(HuPadBtn[0] & PAD_BUTTON_A) {
                    zoom += 25.0f*speed;
                }
                if(HuPadBtn[0] & PAD_BUTTON_B) {
                    zoom -= 25.0f*speed;
                }
                if(HuPadBtnDown[0] & PAD_BUTTON_START) {
                    eye = eyeOrig;
                    rot = rotOrig;
                    zoom = zoomOrig;
                }
                fontcolor = 15;
                print8(36.0+x, 84.0+y, 1.5f, "\xFD\x01ROTATE MODE");
                break;

            default:
                if(fabs(mbPadStkXGet(0)) > 8.0) {
                    x += 0.05f*mbPadStkXGet(0);
                }
                if(fabs(mbPadStkYGet(0)) > 8.0) {
                    y += 0.05f*mbPadStkYGet(0);
                }
                fontcolor = 14;
                print8(36.0+x, 84.0+y, 1.5f, "\xFD\x01""DISP MODE");
                break;
        }
        print8(36.0+x, 96.0+y, 1.5f, "\xFD\x01RX:%.4f RY:%.4f RZ:%.4f", rot.x, rot.y, rot.z);
        print8(36.0+x, 108.0+y, 1.5f, "\xFD\x01""CX:%.4f CY:%.4f CZ:%.4f", eye.x, eye.y, eye.z);
        print8(36.0+x, 120.0+y, 1.5f, "\xFD\x01ZM:%.4f", zoom);
        mbObjPosSet(objId, eye.x, eye.y, eye.z);
        mbCameraEyeSet(eye.x, eye.y, eye.z);
        mbCameraRotSet(rot.x, rot.y, rot.z);
        mbCameraZoomSet(zoom);
        HuPrcVSleep();
    } while(!(HuPadBtn[0] & PAD_TRIGGER_L) || !(HuPadBtn[0] & PAD_TRIGGER_R));
    mbCameraMoveOnSet(TRUE);
    mbCameraMovePos(&eyeOrig, &rotOrig, NULL, zoomOrig, -1.0f, 1);
    mbCameraFocusPlayerSet(work->playerNo);
    HuPrcEnd();
}

/* Data the later parts use; defined here so it follows mbev_CapDebugCam's strings. */
static s32 ev_CapEffRingFile[6] = { 0xC0031, 0xC0032, 0xC0033, 0xC0031, 0xC0038, 0xC0038 };

static GXColor ev_CapEffElectricColor[4] = {
    { 192, 192, 255, 255 },
    { 164, 192, 255, 255 },
    { 192, 255, 255, 255 },
    { 192, 164, 255, 255 },
};

static HuVecF basePos[4] = {
    { -0.5f, 0.5f, 0.0f },
    { 0.5f, 0.5f, 0.0f },
    { 0.5f, -0.5f, 0.0f },
    { -0.5f, -0.5f, 0.0f },
};

static HuVec2f baseST1[4] = { { 0.0f, 0.0f }, { 0.25f, 0.0f }, { 0.25f, 0.25f }, { 0.0f, 0.25f } };
static HuVec2f baseST2[4] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

static GXColor capsuleRingColor = { 255, 255, 127, 255 };

/* @@PART2@@ */

typedef struct CapMoveWork_s {
    /* 0x00 */ int playerNo;
    /* 0x04 */ int mode;
    /* 0x08 */ int hitMot;
    /* 0x0C */ int landMot;
    /* 0x10 */ BOOL hitF;
    /* 0x14 */ BOOL landF;
    /* 0x18 */ BOOL masuYF;
    /* 0x1C */ float minY;
    /* 0x20 */ float gravity;
    /* 0x24 */ HuVecF pos;
    /* 0x30 */ HuVecF vel;
    /* 0x3C */ HuVecF p0;
    /* 0x48 */ HuVecF p1;
    /* 0x54 */ HuVecF p2;
    /* 0x60 */ HuVecF rot;
    /* 0x6C */ int wait;
    /* 0x70 */ int time;
    /* 0x74 */ float rotSpeed;
} CAPMOVEWORK; /* size 0x78 */








void mbev_CapDebugCamKlll(void)
{
}

void mbev_CapDebugWarp(void)
{
    CAPWORK *work;
    int i;
    int linkNum;
    int masuId;
    int objId;
    int sprId;
    float stkX;
    float stkY;
    float t;
    float diff;
    float angle[10];
    s16 link[10];
    HuVecF pos;
    HuVecF start;
    HuVecF temp;

    work = HuPrcCurrentGet()->property;
    mbCapEffUseCreate(work->playerNo, work->capsuleNo);
    while(mbCapEffUseModeGet(work->playerNo) >= 0) {
        HuPrcVSleep();
    }
    masuId = GwPlayer[work->playerNo].masuId;
    mbPlayerPosGet(work->playerNo, &pos);
    objId = mbev_CapObjCreate(&work->objWork, 0xC0044, NULL, FALSE, 5, FALSE);
    mbObjPosSet(objId, pos.x, pos.y, pos.z);
    mbObjDispSet(objId, FALSE);
    mbCameraMoveObj(objId, NULL, &ev_CapsuleViewOfs, 3000.0f, -1.0f, 1);
    sprId = mbev_CapSprCreate(&work->objWork, mbBoardDataNumGet(0x50001), 2000, 0);
    espDispOn(sprId);
    espDrawNoSet(sprId, 0);
    espAttrSet(sprId, 1);
    espPosSet(sprId, 0.0f, 0.0f);
    espBankSet(sprId, 0);
    do {
        for(i=0, linkNum=0; i<mbMasuLinkNumGet(masuId); i++, linkNum++) {
            link[linkNum] = mbMasuLinkGet(masuId, i);
        }
        linkNum += mbMasuLinkParentGet(masuId, &link[linkNum]);
        for(i=0; i<linkNum; i++) {
            mbMasuPosGet(link[i], &temp);
            VECSubtract(&temp, &pos, &temp);
            angle[i] = ev_CapRotCamera(HuAtan(temp.x, temp.z));
        }
        stkX = mbPadStkXGet(0);
        stkY = mbPadStkYGet(0);
        if(fabs(stkX) > 8.0 || fabs(stkY) > 8.0) {
            for(i=0; i<linkNum; i++) {
                diff = mbev_CapAngleWrap(angle[i], HuAtan(stkX, -stkY));
                if(fabsf(diff) < 30.0) {
                    masuId = link[i];
                    start = pos;
                    mbMasuPosGet(masuId, &pos);
                    for(i=0; i<18.0f; i++) {
                        t = i/18.0f;
                        VECSubtract(&pos, &start, &temp);
                        VECScale(&temp, &temp, t);
                        VECAdd(&start, &temp, &temp);
                        mbObjPosSetV(objId, &temp);
                        Hu3D3Dto2D(&temp, 1, &temp);
                        temp.x -= 32.0f;
                        temp.y -= 16.0f;
                        espPosSet(sprId, temp.x, temp.y);
                        HuPrcVSleep();
                    }
                    mbObjPosSetV(objId, &pos);
                }
            }
        }
        if(HuPadBtnDown[0] & PAD_BUTTON_A) {
            GwPlayer[0].masuId = masuId;
            mbPlayerPosSetV(0, &pos);
            mbev_PlayerColMasuSet(0, masuId, TRUE);
        }
        if(HuPadBtnDown[0] & PAD_BUTTON_B) {
            GwPlayer[1].masuId = masuId;
            mbPlayerPosSetV(1, &pos);
            mbev_PlayerColMasuSet(1, masuId, TRUE);
        }
        if(HuPadBtnDown[0] & PAD_BUTTON_X) {
            GwPlayer[2].masuId = masuId;
            mbPlayerPosSetV(2, &pos);
            mbev_PlayerColMasuSet(2, masuId, TRUE);
        }
        if(HuPadBtnDown[0] & PAD_BUTTON_Y) {
            GwPlayer[3].masuId = masuId;
            mbPlayerPosSetV(3, &pos);
            mbev_PlayerColMasuSet(3, masuId, TRUE);
        }
        Hu3D3Dto2D(&pos, 1, &temp);
        temp.x -= 32.0f;
        temp.y -= 16.0f;
        espPosSet(sprId, temp.x, temp.y);
        HuPrcVSleep();
    } while(!(HuPadBtn[0] & PAD_TRIGGER_L) || !(HuPadBtn[0] & PAD_TRIGGER_R));
    mbCameraMoveOnSet(TRUE);
    mbCameraFocusPlayerSet(work->playerNo);
    HuPrcEnd();
}

void mbev_CapDebugWarpKill(void)
{
}

void mbev_CapDebugPosSelect(void)
{
    CAPWORK *work = HuPrcCurrentGet()->property;
    mbCapEffUseCreate(work->playerNo, work->capsuleNo);
    while(mbCapEffUseModeGet(work->playerNo) >= 0) {
        HuPrcVSleep();
    }
    mbCapSelectMasuInit();
    HuPrcEnd();
}

void mbev_CapDebugPosSelectKill(void)
{
}

void mbev_CapStatusDispSet(int playerNo1, int playerNo2, BOOL waitF)
{
    HuVecF posBegin;
    HuVecF posEnd;
    if(!mbStatusDispGet(playerNo1)) {
        posBegin.x = -130.0f;
        posBegin.y = 240.0f;
        posBegin.z = 0.0f;
    } else {
        mbStatusPosGet(playerNo1, &posBegin);
    }
    posEnd.x = 130.0f;
    posEnd.y = 240.0f;
    posEnd.z = 0.0f;
    mbStatusMoveSet(playerNo1, &posBegin, &posEnd, 1, 30);
    if(!mbStatusDispGet(playerNo2)) {
        posBegin.x = 706.0f;
        posBegin.y = 240.0f;
        posBegin.z = 0.0f;
    } else {
        mbStatusPosGet(playerNo2, &posBegin);
    }
    posEnd.x = 446.0f;
    posEnd.y = 240.0f;
    posEnd.z = 0.0f;
    mbStatusMoveSet(playerNo2, &posBegin, &posEnd, 1, 30);
    if(waitF) {
        do {
            HuPrcVSleep();
        } while(!mbStatusMoveCheck(playerNo1) || !mbStatusMoveCheck(playerNo2));
    }
}

void mbev_CapDuelStatusOffSet(int playerNo1, int playerNo2, BOOL waitF)
{
    HuVecF posOff;
    HuVecF posOn;
    posOff.x = -130.0f;
    posOff.y = 240.0f;
    posOff.z = 0.0f;
    posOn.x = 130.0f;
    posOn.y = 240.0f;
    posOn.z = 0.0f;
    mbStatusMoveSet(playerNo1, &posOn, &posOff, 2, 30);
    posOff.x = 706.0f;
    posOff.y = 240.0f;
    posOff.z = 0.0f;
    posOn.x = 446.0f;
    posOn.y = 240.0f;
    posOn.z = 0.0f;
    mbStatusMoveSet(playerNo2, &posOn, &posOff, 2, 30);
    if(waitF) {
        do {
            HuPrcVSleep();
        } while(!mbStatusMoveCheck(playerNo1) || !mbStatusMoveCheck(playerNo2));
    }
}

void mbev_CapDuelStatusOnSet(int playerNo1, int playerNo2, BOOL waitF)
{
    HuVecF posOff;
    HuVecF posOn;
    mbStatusPosOffGet(0, &posOff);
    mbStatusPosOnGet(0, &posOn);
    mbStatusLayoutSet(playerNo1, 0);
    mbStatusMoveSet(playerNo1, &posOff, &posOn, 1, 30);
    mbStatusPosOffGet(1, &posOff);
    mbStatusPosOnGet(1, &posOn);
    mbStatusLayoutSet(playerNo2, 0);
    mbStatusMoveSet(playerNo2, &posOff, &posOn, 1, 30);
    if(waitF) {
        do {
            HuPrcVSleep();
        } while(!mbStatusMoveCheck(playerNo1) || !mbStatusMoveCheck(playerNo2));
    }
}

void mbev_CapDuelStatusDispSet(int playerNo1, int playerNo2, BOOL waitF)
{
    HuVecF posOff;
    HuVecF posOn;
    mbStatusPosOffGet(0, &posOff);
    mbStatusPosOnGet(0, &posOn);
    mbStatusMoveSet(playerNo1, &posOn, &posOff, 2, 30);
    mbStatusPosOffGet(1, &posOff);
    mbStatusPosOnGet(1, &posOn);
    mbStatusMoveSet(playerNo2, &posOn, &posOff, 2, 30);
    if(waitF) {
        do {
            HuPrcVSleep();
        } while(!mbStatusMoveCheck(playerNo1) || !mbStatusMoveCheck(playerNo2));
        mbStatusDispForceSet(playerNo1, FALSE);
        mbStatusDispForceSet(playerNo2, FALSE);
    }
}

void mbev_CapStatusDispSetAll(BOOL dispF, BOOL waitF)
{
    int i;
    int num;
    int dispNum;
    BOOL allOnF;
    BOOL allOffF;
    for(i=0, num=0; i<4; i++) {
        num++;
    }
    for(i=0, dispNum=0; i<4; i++) {
        if(mbStatusDispGet(i)) {
            dispNum++;
        }
    }
    if(dispNum == num) {
        allOnF = TRUE;
    } else {
        allOnF = FALSE;
    }
    for(i=0, dispNum=0; i<4; i++) {
        if(!mbStatusDispGet(i)) {
            dispNum++;
        }
    }
    if(dispNum == num) {
        allOffF = TRUE;
    } else {
        allOffF = FALSE;
    }
    if(dispF) {
        if(allOnF) {
            return;
        }
        if(!allOnF && !allOffF) {
            mbev_CapStatusDispSetAll(FALSE, TRUE);
        }
    } else if(allOffF) {
        return;
    }
    mbStatusDispSetAll(dispF);
    if(waitF) {
        do {
            for(i=0; i<4; i++) {
                if(!mbStatusMoveCheck(i)) {
                    break;
                }
            }
            HuPrcVSleep();
        } while(i < 4);
    }
}

BOOL mbev_CapStatusDispCheck(int playerNo)
{
    int i;
    for(i=0; i<4; i++) {
        if(i == playerNo) {
            if(!mbStatusDispGet(i)) {
                break;
            }
        } else {
            if(mbStatusDispGet(i)) {
                break;
            }
        }
    }
    if(i >= 4) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void mbev_CapPlayerMoveObjInit(void)
{
    int i;
    for(i=0; i<4; i++) {
        ev_CapEffMoveOMObj[i] = NULL;
    }
}

void mbev_CapPlayerMoveHitCreate(int playerNo, BOOL hitF, BOOL landF)
{
    OMOBJ *obj;
    CAPMOVEWORK *work;
    obj = ev_CapEffMoveOMObj[playerNo] = omAddObjEx(mbObjMan, 0x8000, 0, 0, -1, mbev_CapPlayerMoveObjExec);
    work = obj->data = CapMalloc(sizeof(CAPMOVEWORK));
    memset(work, 0, sizeof(CAPMOVEWORK));
    if(hitF) {
        work->hitMot = 9;
    } else {
        work->hitMot = -1;
    }
    if(landF) {
        work->landMot = 6;
    } else {
        work->landMot = -1;
    }
    work->playerNo = playerNo;
    work->mode = 0;
    work->hitF = hitF;
    work->landF = landF;
    work->masuYF = TRUE;
    mbPlayerPosGet(playerNo, &work->pos);
    work->minY = work->pos.y;
    work->gravity = 6.5333333f;
    work->vel.x = work->vel.z = 0.0f;
    work->vel.y = 60.000004f;
    if(work->hitF && work->hitMot != -1) {
        work->mode = 0;
        mbPlayerMotionSet(playerNo, work->hitMot, 0);
    } else {
        work->mode = 1;
        if(work->landF && work->landMot != -1) {
            mbPlayerMotionShiftSet(playerNo, work->landMot, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
        }
    }
    mbPlayerColSnapPlayerSet(work->playerNo, FALSE);
}

void mbev_CapPlayerMoveEjectCreate(int playerNo, BOOL landF)
{
    OMOBJ *obj;
    CAPMOVEWORK *work;
    obj = ev_CapEffMoveOMObj[playerNo] = omAddObjEx(mbObjMan, 0x8000, 0, 0, -1, mbev_CapPlayerMoveObjExec);
    work = obj->data = CapMalloc(sizeof(CAPMOVEWORK));
    memset(work, 0, sizeof(CAPMOVEWORK));
    work->hitMot = -1;
    if(landF) {
        work->landMot = 6;
    } else {
        work->landMot = -1;
    }
    work->playerNo = playerNo;
    work->mode = 0;
    work->hitF = TRUE;
    work->landF = landF;
    work->masuYF = TRUE;
    mbPlayerPosGet(playerNo, &work->pos);
    work->minY = work->pos.y;
    work->gravity = 6.5333333f;
    work->vel.x = work->vel.z = 0.0f;
    work->vel.y = 80.0f;
    work->mode = 0;
    mbPlayerColSnapPlayerSet(work->playerNo, FALSE);
}

void mbev_CapPlayerMoveIdleCreate(int playerNo, int wait)
{
    OMOBJ *obj;
    CAPMOVEWORK *work;
    int masuId;
    HuVecF masuPos;
    HuVecF dir;
    obj = ev_CapEffMoveOMObj[playerNo] = omAddObjEx(mbObjMan, 0x8000, 0, 0, -1, mbev_CapPlayerMoveObjExec);
    work = obj->data = CapMalloc(sizeof(CAPMOVEWORK));
    memset(work, 0, sizeof(CAPMOVEWORK));
    work->hitMot = 9;
    work->landMot = 6;
    work->playerNo = playerNo;
    work->mode = 1;
    work->hitF = TRUE;
    work->landF = TRUE;
    work->masuYF = TRUE;
    mbPlayerPosGet(playerNo, &work->pos);
    work->minY = work->pos.y;
    work->gravity = 6.5333333f;
    work->vel.x = work->vel.z = 0.0f;
    work->vel.y = 80.0f;
    work->mode = 1;
    masuId = GwPlayer[playerNo].masuId;
    mbMasuPosGet(masuId, &masuPos);
    mbPlayerPosGet(playerNo, &work->p0);
    work->p1 = work->p0;
    work->p1.y += 1000.0f;
    VECSubtract(&work->p0, &masuPos, &dir);
    VECScale(&dir, &dir, 4.0f);
    VECAdd(&work->p1, &dir, &work->p2);
    work->wait = wait;
    work->time = 0;
    work->rotSpeed = 2.0f*(-0.5f+CAP_RANDF());
    mbPlayerRotGet(playerNo, &work->rot);
    mbPlayerMotionSet(playerNo, work->hitMot, 0);
    mbPlayerColSnapPlayerSet(work->playerNo, FALSE);
}

void mbev_CapPlayerMoveMinYSet(int playerNo, float minY)
{
    OMOBJ *obj = ev_CapEffMoveOMObj[playerNo];
    CAPMOVEWORK *work;
    if(obj != NULL) {
        work = obj->data;
        work->minY = minY;
        work->masuYF = FALSE;
    }
}

void mbev_CapPlayerMoveVelSet(int playerNo, HuVecF *vel, float gravity)
{
    OMOBJ *obj = ev_CapEffMoveOMObj[playerNo];
    CAPMOVEWORK *work;
    if(obj != NULL) {
        work = obj->data;
        work->gravity = gravity;
        work->vel = *vel;
    }
}

void mbev_CapPlayerMoveObjExec(OMOBJ *obj)
{
    CAPMOVEWORK *work = obj->data;
    float t;
    float minY;
    HuVecF pos;
    switch(work->mode) {
        case 0:
            VECAdd(&work->pos, &work->vel, &work->pos);
            work->vel.y -= work->gravity;
            if(work->masuYF) {
                mbMasuPosGet(GwPlayer[work->playerNo].masuId, &pos);
                minY = pos.y;
            } else {
                minY = work->minY;
            }
            if(work->pos.y <= minY) {
                work->pos.y = minY;
                work->mode = 4;
                if(work->landF && work->landMot != -1) {
                    mbPlayerMotionShiftSet(work->playerNo, work->landMot, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
                } else {
                    mbPlayerMotionShiftSet(work->playerNo, 1, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
                }
                mbPlayerColSnapPlayerSet(work->playerNo, TRUE);
            }
            mbPlayerPosSetV(work->playerNo, &work->pos);
            break;

        case 1:
            t = ++work->time/60.0f;
            mbev_CapBezierGetV(t, (float *)&work->p0, (float *)&work->p1, (float *)&work->p2, (float *)&pos);
            mbPlayerPosSetV(work->playerNo, &pos);
            mbPlayerRotSet(work->playerNo, work->rot.x, work->rot.y+((720.0f*t)*work->rotSpeed), work->rot.z);
            if(t >= 1.0f) {
                mbPlayerDispSet(work->playerNo, FALSE);
                work->mode = 2;
                work->time = 0;
            }
            break;

        case 2:
            if(++work->time >= work->wait) {
                work->pos = work->p1;
                work->pos.y = 1000.0f+work->p0.y;
                mbPlayerPosSetV(work->playerNo, &work->pos);
                mbPlayerRotSetV(work->playerNo, &work->rot);
                mbPlayerDispSet(work->playerNo, TRUE);
                work->minY = work->p0.y;
                work->mode = 0;
                work->time = 0;
            }
            break;

        case 4:
            break;
    }
    if(mbExitCheck() || work->mode == 4 || ev_CapEffMoveOMObj[work->playerNo] == NULL) {
        HuMemDirectFree(work);
        obj->data = NULL;
        omDelObjEx(mbObjMan, obj);
        ev_CapEffMoveOMObj[work->playerNo] = NULL;
    }
}

BOOL mbev_CapPlayerMoveObjCheck(int playerNo)
{
    OMOBJ *obj = ev_CapEffMoveOMObj[playerNo];
    CAPMOVEWORK *work;
    if(obj == NULL) {
        return TRUE;
    }
    work = obj->data;
    if(work->mode >= 1) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void mbev_CapPlayerMoveObjClose(int playerNo)
{
    ev_CapEffMoveOMObj[playerNo] = NULL;
}

void mbev_CapPlayerMoveObjKill(void)
{
    int i;
    for(i=0; i<4; i++) {
        ev_CapEffMoveOMObj[i] = NULL;
    }
}

void *mbev_CapEffExplodeCreate(void)
{
    int i;
    OMOBJ *obj;
    CAPEFFWORK *work;
    ANIMDATA *anim;
    int modelId;
    HU3D_MODEL *modelP;
    CAPEV_EFFECT *effP;
    for(i=0; i<8; i++) {
        if(ev_CapEffExplodeOMObj[i] == NULL) {
            break;
        }
    }
    obj = ev_CapEffExplodeOMObj[i] = omAddObjEx(mbObjMan, 0x8000, 0, 0, -1, mbev_CapEffExplodeOMExec);
    work = obj->data = CapMalloc(sizeof(CAPEFFWORK));
    memset(work, 0, sizeof(CAPEFFWORK));
    work->anim = anim = HuSprAnimRead(HuDataReadNum(0xC002F, HU_MEMNUM_OVL));
    work->mdlId = modelId = ev_CapEffCreate(anim, 256);
    Hu3DModelLayerSet(work->mdlId, 5);
    work->num = 0;
    work->no = i;
    modelP = &Hu3DData[work->mdlId];
    effP = modelP->hookData;
    effP->blendMode = 0;
    effP->unk22 = 2;
    return obj;
}

void *mbev_CapEffExhaustCreate(void)
{
    int i;
    OMOBJ *obj;
    CAPEFFWORK *work;
    ANIMDATA *anim;
    int modelId;
    HU3D_MODEL *modelP;
    CAPEV_EFFECT *effP;
    for(i=0; i<8; i++) {
        if(ev_CapEffExplodeOMObj[i] == NULL) {
            break;
        }
    }
    obj = ev_CapEffExplodeOMObj[i] = omAddObjEx(mbObjMan, 0x8000, 0, 0, -1, mbev_CapEffExplodeOMExec);
    work = obj->data = CapMalloc(sizeof(CAPEFFWORK));
    memset(work, 0, sizeof(CAPEFFWORK));
    work->anim = anim = HuSprAnimRead(HuDataReadNum(0xC002E, HU_MEMNUM_OVL));
    work->mdlId = modelId = ev_CapEffCreate(anim, 256);
    Hu3DModelLayerSet(work->mdlId, 5);
    work->num = 0;
    work->no = i;
    modelP = &Hu3DData[work->mdlId];
    effP = modelP->hookData;
    effP->blendMode = 0;
    effP->unk22 = 2;
    return obj;
}

void mbev_CapEffExplodeOMExec(OMOBJ *obj)
{
    CAPEFFWORK *work = obj->data;
    HU3D_MODEL *modelP;
    CAPEV_EFFECT *effP;
    CAPEV_EFFDATA *dataP;
    int i;
    if(mbExitCheck() || ev_CapEffExplodeOMObj[work->no] == (OMOBJ *)-1) {
        Hu3DModelKill(work->mdlId);
        work->mdlId = -1;
        HuSprAnimKill(work->anim);
        work->anim = NULL;
        ev_CapEffExplodeOMObj[work->no] = NULL;
        omDelObjEx(mbObjMan, obj);
        return;
    }
    if(work->num <= 0) {
        Hu3DModelAttrSet(work->mdlId, HU3D_ATTR_DISPOFF);
        return;
    }
    Hu3DModelAttrReset(work->mdlId, HU3D_ATTR_DISPOFF);
    modelP = &Hu3DData[work->mdlId];
    effP = modelP->hookData;
    dataP = effP->data;
    effP->unk23 = 0;
    for(i=0; i<effP->num; i++, dataP++) {
        if(dataP->scale <= 0.0f) {
            continue;
        }
        dataP->pos.x += dataP->vel.x;
        dataP->pos.y += dataP->vel.y;
        dataP->pos.z += dataP->vel.z;
        dataP->rot.z += dataP->speed;
        if(dataP->rot.z >= 360.0f) {
            dataP->rot.z -= 360.0f;
        }
        dataP->unk38 += dataP->unk3C;
        dataP->color.a = 255.0f*(1.0f-(dataP->unk38/16.0f));
        if(dataP->unk38 >= 16.0f) {
            dataP->unk68 = 0;
            dataP->time = 0;
            dataP->scale = 0.0f;
            work->num--;
        }
    }
}

void mbev_CapEffExplodeKill(void *eff)
{
    int i;
    for(i=0; i<8; i++) {
        if(ev_CapEffExplodeOMObj[i] == eff) {
            break;
        }
    }
    ev_CapEffExplodeOMObj[i] = (OMOBJ *)-1;
}

int mbev_CapEffExplodeAnimGet(void *eff)
{
    int i;
    CAPEFFWORK *work;
    for(i=0; i<8; i++) {
        if(ev_CapEffExplodeOMObj[i] == eff) {
            break;
        }
    }
    if(ev_CapEffExplodeOMObj[i] == NULL) {
        return 0;
    }
    work = ((OMOBJ *)eff)->data;
    return work->num;
}

void mbev_CapEffExplodeAnimSet(void *eff, int dataNum)
{
    CAPEFFWORK *work;
    CAPEV_EFFECT *effP;
    int i;
    HU3D_MODEL *modelP;
    CAPEV_EFFDATA *dataP;
    for(i=0; i<8; i++) {
        if(ev_CapEffExplodeOMObj[i] == eff) {
            break;
        }
    }
    work = ((OMOBJ *)eff)->data;
    modelP = &Hu3DData[work->mdlId];
    effP = modelP->hookData;
    dataP = effP->data;
    if(work->anim != NULL) {
        HuSprAnimKill(work->anim);
    }
    work->anim = effP->anim = HuSprAnimRead(HuDataReadNum(dataNum, HU_MEMNUM_OVL));
}

int mbev_CapEffExplodeAdd(void *eff, HuVecF pos, HuVecF vel, float scale, float rotSpeed, float speed, GXColor color)
{
    int i;
    CAPEFFWORK *work;
    HU3D_MODEL *modelP;
    CAPEV_EFFECT *effP;
    CAPEV_EFFDATA *dataP;
    for(i=0; i<8; i++) {
        if(ev_CapEffExplodeOMObj[i] == eff) {
            break;
        }
    }
    work = ((OMOBJ *)eff)->data;
    modelP = &Hu3DData[work->mdlId];
    effP = modelP->hookData;
    dataP = effP->data;
    for(i=0; i<effP->num; i++, dataP++) {
        if(dataP->scale <= 0.0f) {
            break;
        }
    }
    if(i >= effP->num) {
        return -1;
    }
    dataP->time = dataP->maxTime = 0;
    dataP->pos.x = pos.x;
    dataP->pos.y = pos.y;
    dataP->pos.z = pos.z;
    dataP->vel.x = vel.x;
    dataP->vel.y = vel.y;
    dataP->vel.z = vel.z;
    dataP->speed = rotSpeed;
    dataP->scale = scale;
    dataP->color.r = color.r;
    dataP->color.g = color.g;
    dataP->color.b = color.b;
    dataP->color.a = color.a;
    dataP->color.r = dataP->color.g = dataP->color.b = 255;
    dataP->rot.z = 0.0f;
    dataP->unk68 = 0;
    dataP->time = 0;
    dataP->unk38 = 0.0f;
    dataP->unk3C = speed;
    work->num++;
    return i;
}

int mbev_CapEffExplodeKillerAdd(void *eff, HuVecF pos, HuVecF vel, float scale, float rotSpeed, float width, float speed, GXColor color)
{
    HuVecF p;
    HuVecF dir;
    float ofs;
    int id1;
    int id2;
    dir.x = vel.z;
    dir.z = vel.x;
    dir.y = 0.0f;
    if(VECMag(&dir) > 0.0f) {
        VECNormalize(&dir, &dir);
    }
    ofs = 0.5f*width;
    p.x = pos.x+(dir.x*ofs);
    p.y = pos.y+(dir.y*ofs);
    p.z = pos.z+(dir.z*ofs);
    id1 = mbev_CapEffExplodeAdd(eff, p, vel, scale, rotSpeed, speed, color);
    p.x = pos.x-(dir.x*ofs);
    p.y = pos.y-(dir.y*ofs);
    p.z = pos.z-(dir.z*ofs);
    id2 = mbev_CapEffExplodeAdd(eff, p, vel, scale, -rotSpeed, speed, color);
    return (id1 << 16) | id2;
}

void mbev_CapEffExplodeCircleAdd(void *eff, HuVecF *pos, float scale, float radius, float angle)
{
    HuVecF p;
    HuVecF vel;
    GXColor color;
    float speed;
    float t;
    p.x = pos->x+(scale*(radius*CapCosDeg(angle)));
    p.y = 50.0f+pos->y;
    p.z = pos->z+(scale*(radius*CapSinDeg(angle)));
    speed = 100.0f*(0.005f+(0.04f*CAP_RANDF()));
    vel.x = speed*CapCosDeg(angle);
    vel.y = speed*CapSinDeg(angle);
    vel.z = 0.0f;
    t = CAP_RANDF();
    color.r = 64.0f+(32.0f*t);
    color.g = 64.0f+(32.0f*t);
    color.b = 64.0f+(32.0f*t);
    color.a = 64.0f+(63.0f*CAP_RANDF());
    mbev_CapEffExplodeKillerAdd(eff, p, vel, 100.0f*(1.0f+(0.5f*CAP_RANDF())), 2.5f, 100.0f*(0.5f+(0.5f*CAP_RANDF())), 0.33f+(0.2f*CAP_RANDF()), color);
}

void mbev_CapEffDustCloudAdd(void *eff, HuVecF *pos)
{
    int i;
    float angle;
    float r;
    HuVecF p;
    HuVecF vel;
    GXColor color;
    for(i=0; i<32; i++) {
        angle = 11.25f*i;
        r = 0.75f*(100.0f*(0.5f*CAP_RANDF()));
        p.x = pos->x+(r*CapCosDeg(angle));
        p.y = 100.0f+(pos->y+(r*CapSinDeg(angle)));
        p.z = 50.0f+pos->z;
        r = 100.0f*(0.005f+(0.04f*CAP_RANDF()));
        vel.x = r*CapCosDeg(angle);
        vel.y = r*CapSinDeg(angle);
        vel.z = 0.0f;
        r = CAP_RANDF();
        color.r = 32.0f+(32.0f*r);
        color.g = 32.0f+(32.0f*r);
        color.b = 32.0f+(32.0f*r);
        color.a = 128.0f+(63.0f*CAP_RANDF());
        mbev_CapEffExplodeKillerAdd(eff, p, vel, 100.0f*(1.5f+(0.5f*CAP_RANDF())), -0.5f+CAP_RANDF(), 100.0f*(0.5f+(0.5f*CAP_RANDF())), 0.33f+(0.66f*CAP_RANDF()), color);
    }
    for(i=0; i<32; i++) {
        angle = 360.0f*CAP_RANDF();
        r = 0.33f*(100.0f*(0.5f*CAP_RANDF()));
        p.x = pos->x+(1.25f*(100.0f*(-0.5f+CAP_RANDF())));
        p.y = pos->y+(1.25f*(100.0f*(-0.5f+CAP_RANDF())));
        p.z = pos->z+(50.0f*CAP_RANDF());
        r = 100.0f*(0.005f+(0.04f*CAP_RANDF()));
        vel.x = r*CapCosDeg(angle);
        vel.y = r*CapSinDeg(angle);
        vel.z = 0.0f;
        r = CAP_RANDF();
        color.r = 32.0f+(32.0f*r);
        color.g = 32.0f+(32.0f*r);
        color.b = 32.0f+(32.0f*r);
        color.a = 128.0f+(63.0f*CAP_RANDF());
        mbev_CapEffExplodeKillerAdd(eff, p, vel, 100.0f*(1.5f+(0.5f*CAP_RANDF())), -0.5f+CAP_RANDF(), 100.0f*(0.5f+(0.5f*CAP_RANDF())), 0.33f+(0.66f*CAP_RANDF()), color);
    }
}

/* @@PART3@@ */








void mbev_CapEffDustExplodeAdd(OMOBJ *obj, HuVecF pos)
{
    int i;
    float angle;
    float radius;
    HuVecF p;
    HuVecF vel;
    GXColor color;
    for(i=0; i<32; i++) {
        angle = 11.25f*i;
        radius = 0.33f*(100.0f*CAP_RANDF());
        p.x = pos.x+(radius*CapCosDeg(angle));
        p.y = 100.0f+(pos.y+(radius*CapSinDeg(angle)));
        p.z = 50.0f+pos.z;
        radius = 100.0f*(0.005f+(0.04f*CAP_RANDF()));
        vel.x = radius*CapCosDeg(angle);
        vel.y = radius*CapSinDeg(angle);
        vel.z = 0.0f;
        radius = CAP_RANDF();
        color.r = 192.0f+(32.0f*radius);
        color.g = 192.0f+(32.0f*radius);
        color.b = 192.0f+(32.0f*radius);
        color.a = 192.0f+(63.0f*CAP_RANDF());
        mbev_CapEffExplodeKillerAdd(obj, p, vel, 100.0f*(1.0f+(0.5f*CAP_RANDF())), -0.5f+CAP_RANDF(), 100.0f*(0.5f+(0.5f*CAP_RANDF())), 0.33f+(0.66f*CAP_RANDF()), color);
    }
}

void mbev_CapEffDustHeavyAdd(OMOBJ *obj, HuVecF pos)
{
    int i;
    float angle;
    float radius;
    HuVecF p;
    HuVecF vel;
    GXColor color;
    for(i=0; i<32; i++) {
        angle = 22.5f*i;
        angle += 20.0f*(-0.5f+CAP_RANDF());
        radius = 0.33f*(100.0f*CAP_RANDF());
        p.x = pos.x+(radius*CapCosDeg(angle));
        p.y = 50.0f+pos.y;
        p.z = pos.z+(radius*CapSinDeg(angle));
        radius = 100.0f*(0.03f+(0.06f*CAP_RANDF()));
        vel.x = radius*CapCosDeg(angle);
        vel.y = 0.01f*(100.0f*CAP_RANDF());
        vel.z = radius*CapSinDeg(angle);
        radius = CAP_RANDF();
        color.r = 192.0f+(63.0f*radius);
        color.g = 192.0f+(63.0f*radius);
        color.b = 192.0f+(63.0f*radius);
        color.a = 64.0f+(63.0f*CAP_RANDF());
        mbev_CapEffExplodeKillerAdd(obj, p, vel, 100.0f*(1.0f+(0.5f*CAP_RANDF())), -0.5f+CAP_RANDF(), 100.0f*(0.5f+(0.5f*CAP_RANDF())), 0.33f+(0.66f*CAP_RANDF()), color);
    }
    mbCameraShakeSet(30, 50.0f);
}

void mbev_CapEffDustMultiAdd(OMOBJ *obj, HuVecF *pos, HuVecF *rot, int num)
{
    int i;
    int angle;
    int col;
    float s;
    float c;
    HuVecF p;
    HuVecF vel;
    Mtx mtx;
    GXColor color;
    mtxRot(mtx, rot->x, rot->y, rot->z);
    for(i=0; i<num; i++) {
        p.x = pos->x;
        p.y = pos->y;
        p.z = pos->z;
        angle = (360.0f/num)*i;
        s = mbSinDeg(angle);
        vel.x = 0.075f*(100.0f*s)*(0.9f+(0.1f*CAP_RANDF()));
        vel.y = 0.0f;
        c = mbCosDeg(angle);
        vel.z = 0.075f*(100.0f*c)*(0.9f+(0.1f*CAP_RANDF()));
        MTXMultVec(mtx, &vel, &vel);
        col = 63.0f*CAP_RANDF();
        color.r = col+192;
        color.g = col+192;
        color.b = col+192;
        color.a = 192.0f+(63.0f*CAP_RANDF());
        mbev_CapEffExplodeAdd(obj, p, vel, 200.0f, 0.5f*(-0.5f+CAP_RANDF()), 0.33f, color);
    }
}



void mbev_CapEffBoostOMExec(OMOBJ *obj);

OMOBJ *mbev_CapEffBoostCreate(void)
{
    int i;
    OMOBJ *obj;
    CAPEFFWORK *work;
    HU3D_MODEL *modelP;
    ANIMDATA *anim;
    int mdlId;
    CAPEV_EFFECT *eff;
    for(i=0; i<8; i++) {
        if(ev_CapEffBoostOMObj[i] == NULL) {
            break;
        }
    }
    obj = ev_CapEffBoostOMObj[i] = omAddObjEx(mbObjMan, 32768, 0, 0, -1, mbev_CapEffBoostOMExec);
    work = obj->data = CapMalloc(sizeof(CAPEFFWORK));
    memset(work, 0, sizeof(CAPEFFWORK));
    anim = boostEffAnim;
    work->anim = anim;
    work->mdlId = mdlId = ev_CapEffCreate(anim, 256);
    Hu3DModelLayerSet(work->mdlId, 5);
    work->num = 0;
    work->no = i;
    modelP = &Hu3DData[work->mdlId];
    eff = modelP->hookData;
    eff->blendMode = 0;
    eff->unk22 = 2;
    return obj;
}

void mbev_CapEffBoostOMExec(OMOBJ *obj)
{
    CAPEFFWORK *work = obj->data;
    HU3D_MODEL *modelP;
    CAPEV_EFFECT *eff;
    CAPEV_EFFDATA *data;
    int i;
    float t;
    float s;
    if(mbExitCheck() || ev_CapEffBoostOMObj[work->no] == (OMOBJ *)-1) {
        Hu3DModelKill(work->mdlId);
        work->mdlId = -1;
        HuSprAnimKill(work->anim);
        work->anim = NULL;
        ev_CapEffBoostOMObj[work->no] = NULL;
        omDelObjEx(mbObjMan, obj);
        return;
    }
    if(work->num <= 0) {
        Hu3DModelAttrSet(work->mdlId, HU3D_ATTR_DISPOFF);
        return;
    }
    Hu3DModelAttrReset(work->mdlId, HU3D_ATTR_DISPOFF);
    modelP = &Hu3DData[work->mdlId];
    eff = modelP->hookData;
    data = eff->data;
    for(i=0; i<eff->num; i++, data++) {
        if(data->scale <= 0.0f) {
            continue;
        }
        VECAdd(&data->pos, &data->vel, &data->pos);
        data->rot.z += data->speed;
        if(data->rot.z >= 360.0f) {
            data->rot.z -= 360.0f;
        }
        t = (float)data->time/(float)data->maxTime;
        data->time--;
        s = mbSinDeg(90.0f*t);
        data->color.a = data->alpha*s;
        if(data->time < 0) {
            data->scale = 0.0f;
            work->num--;
        }
    }
}

void mbev_CapEffBoostKill(OMOBJ *obj)
{
    int i;
    for(i=0; i<8; i++) {
        if(ev_CapEffBoostOMObj[i] == obj) {
            break;
        }
    }
    ev_CapEffBoostOMObj[i] = (OMOBJ *)-1;
}

int mbev_CapEffBoostTimeGet(OMOBJ *obj)
{
    int i;
    CAPEFFWORK *work;
    for(i=0; i<8; i++) {
        if(ev_CapEffBoostOMObj[i] == obj) {
            break;
        }
    }
    work = obj->data;
    return work->num;
}

void mbev_CapEffBoostBlendModeSet(OMOBJ *obj, int blendMode)
{
    int i;
    CAPEFFWORK *work;
    CAPEV_EFFECT *eff;
    HU3D_MODEL *modelP;
    for(i=0; i<8; i++) {
        if(ev_CapEffBoostOMObj[i] == obj) {
            break;
        }
    }
    work = obj->data;
    modelP = &Hu3DData[work->mdlId];
    eff = modelP->hookData;
    eff->blendMode = (u8)blendMode;
}

int mbev_CapEffBoostAdd(OMOBJ *obj, HuVecF pos, HuVecF vel, float scale, float speed, int time, GXColor color)
{
    int i;
    CAPEFFWORK *work;
    HU3D_MODEL *modelP;
    CAPEV_EFFECT *eff;
    CAPEV_EFFDATA *data;
    for(i=0; i<8; i++) {
        if(ev_CapEffBoostOMObj[i] == obj) {
            break;
        }
    }
    work = obj->data;
    modelP = &Hu3DData[work->mdlId];
    eff = modelP->hookData;
    data = eff->data;
    for(i=0; i<eff->num; i++, data++) {
        if(data->scale <= 0.0) {
            break;
        }
    }
    if(i >= eff->num) {
        return -1;
    }
    data->time = data->maxTime = time;
    data->vel.x = vel.x;
    data->vel.y = vel.y;
    data->vel.z = vel.z;
    data->alpha = color.a;
    data->speed = speed;
    data->scale = scale;
    data->rot.z = 360.0f*CAP_RANDF();
    data->pos.x = pos.x;
    data->pos.y = pos.y;
    data->pos.z = pos.z;
    data->color = color;
    data->unk68 = 0;
    work->num++;
    return i;
}

void mbev_CapEffSnowOMExec(OMOBJ *obj);

OMOBJ *mbev_CapEffSnowCreate(void)
{
    int i;
    OMOBJ *obj;
    CAPEFFWORK *work;
    HU3D_MODEL *modelP;
    ANIMDATA *anim;
    int mdlId;
    CAPEV_EFFECT *eff;
    for(i=0; i<8; i++) {
        if(ev_CapEffSnowOMObj[i] == NULL) {
            break;
        }
    }
    obj = ev_CapEffSnowOMObj[i] = omAddObjEx(mbObjMan, 32768, 0, 0, -1, mbev_CapEffSnowOMExec);
    work = obj->data = CapMalloc(sizeof(CAPEFFWORK));
    memset(work, 0, sizeof(CAPEFFWORK));
    anim = HuSprAnimRead(HuDataReadNum(0xC0035, HU_MEMNUM_OVL));
    work->anim = anim;
    work->mdlId = mdlId = ev_CapEffCreate(anim, 128);
    Hu3DModelLayerSet(work->mdlId, 5);
    work->num = 0;
    work->no = i;
    modelP = &Hu3DData[work->mdlId];
    eff = modelP->hookData;
    eff->blendMode = 0;
    eff->unk22 = 2;
    return obj;
}

void mbev_CapEffSnowOMExec(OMOBJ *obj)
{
    CAPEFFWORK *work = obj->data;
    HU3D_MODEL *modelP;
    CAPEV_EFFECT *eff;
    CAPEV_EFFDATA *data;
    int i;
    float angle;
    float s;
    float sx;
    if(mbExitCheck() || ev_CapEffSnowOMObj[work->no] == (OMOBJ *)-1) {
        Hu3DModelKill(work->mdlId);
        work->mdlId = -1;
        HuSprAnimKill(work->anim);
        work->anim = NULL;
        ev_CapEffSnowOMObj[work->no] = NULL;
        omDelObjEx(mbObjMan, obj);
        return;
    }
    if(work->num <= 0) {
        Hu3DModelAttrSet(work->mdlId, HU3D_ATTR_DISPOFF);
        return;
    }
    Hu3DModelAttrReset(work->mdlId, HU3D_ATTR_DISPOFF);
    modelP = &Hu3DData[work->mdlId];
    eff = modelP->hookData;
    data = eff->data;
    for(i=0; i<eff->num; i++, data++) {
        if(data->scale <= 0.0f) {
            continue;
        }
        if(++data->time > 360) {
            data->time -= 360;
        }
        angle = 2.0f*data->time;
        s = mbSinDeg(angle);
        sx = s;
        data->pos.x += data->vel.x*sx;
        data->pos.y += data->vel.y;
        data->alpha -= data->alphaSpeed;
        data->color.a = 255.0f*data->alpha;
        if(data->alpha < 0.0f) {
            data->scale = 0.0f;
        }
    }
}

void mbev_CapEffSnowKill(OMOBJ *obj)
{
    int i;
    for(i=0; i<8; i++) {
        if(ev_CapEffSnowOMObj[i] == obj) {
            break;
        }
    }
    ev_CapEffSnowOMObj[i] = (OMOBJ *)-1;
}

int mbev_CapEffSnowDispGet(OMOBJ *obj)
{
    int i;
    CAPEFFWORK *work;
    for(i=0; i<8; i++) {
        if(ev_CapEffSnowOMObj[i] == obj) {
            break;
        }
    }
    work = obj->data;
    return work->num;
}

int mbev_CapEffSnowAdd(OMOBJ *obj, HuVecF pos, int time)
{
    int i;
    CAPEFFWORK *work;
    HU3D_MODEL *modelP;
    CAPEV_EFFECT *eff;
    CAPEV_EFFDATA *data;
    for(i=0; i<8; i++) {
        if(ev_CapEffSnowOMObj[i] == obj) {
            break;
        }
    }
    work = obj->data;
    modelP = &Hu3DData[work->mdlId];
    eff = modelP->hookData;
    data = eff->data;
    for(i=0; i<eff->num; i++, data++) {
        if(data->scale <= 0.0) {
            break;
        }
    }
    if(i >= eff->num) {
        return -1;
    }
    data->pos = pos;
    data->vel.x = 0.5f*(1.5f+(0.2f*CAP_RANDF()));
    data->vel.z = 1.5f+(0.2f*CAP_RANDF());
    data->vel.y = 2.0f*-(1.5f+(0.2f*CAP_RANDF()));
    data->alpha = 1.0f;
    data->alphaSpeed = 1.0f/time;
    data->time = mbRandMod(360);
    data->scale = 15.000001f*(1.0f+(0.25f*CAP_RANDF()));
    data->color.r = data->color.g = data->color.b = data->color.a = 255;
    work->num++;
    return i;
}

void mbev_CapEffGlowOMExec(OMOBJ *obj);

OMOBJ *mbev_CapEffGlowCreate(void)
{
    int i;
    OMOBJ *obj;
    CAPEFFWORK *work;
    HU3D_MODEL *modelP;
    ANIMDATA *anim;
    int mdlId;
    CAPEV_EFFECT *eff;
    for(i=0; i<8; i++) {
        if(ev_CapEffGlowOMObj[i] == NULL) {
            break;
        }
    }
    obj = ev_CapEffGlowOMObj[i] = omAddObjEx(mbObjMan, 32768, 0, 0, -1, mbev_CapEffGlowOMExec);
    work = obj->data = CapMalloc(sizeof(CAPEFFWORK));
    memset(work, 0, sizeof(CAPEFFWORK));
    anim = HuSprAnimRead(HuDataReadNum(0xC0029, HU_MEMNUM_OVL));
    work->anim = anim;
    work->mdlId = mdlId = ev_CapEffCreate(anim, 1024);
    Hu3DModelLayerSet(work->mdlId, 5);
    work->num = 0;
    work->no = i;
    modelP = &Hu3DData[work->mdlId];
    eff = modelP->hookData;
    eff->blendMode = 1;
    eff->unk22 = 2;
    return obj;
}

OMOBJ *mbev_CapEffGlowFireCreate(void)
{
    int i;
    OMOBJ *obj;
    CAPEFFWORK *work;
    HU3D_MODEL *modelP;
    ANIMDATA *anim;
    int mdlId;
    CAPEV_EFFECT *eff;
    for(i=0; i<8; i++) {
        if(ev_CapEffGlowOMObj[i] == NULL) {
            break;
        }
    }
    obj = ev_CapEffGlowOMObj[i] = omAddObjEx(mbObjMan, 32768, 0, 0, -1, mbev_CapEffGlowOMExec);
    work = obj->data = CapMalloc(sizeof(CAPEFFWORK));
    memset(work, 0, sizeof(CAPEFFWORK));
    anim = HuSprAnimRead(HuDataReadNum(0xC0035, HU_MEMNUM_OVL));
    work->anim = anim;
    work->mdlId = mdlId = ev_CapEffCreate(anim, 1024);
    Hu3DModelLayerSet(work->mdlId, 5);
    work->num = 0;
    work->no = i;
    modelP = &Hu3DData[work->mdlId];
    eff = modelP->hookData;
    eff->blendMode = 1;
    eff->unk22 = 2;
    return obj;
}

void mbev_CapEffGlowOMExec(OMOBJ *obj)
{
    CAPEFFWORK *work = obj->data;
    HU3D_MODEL *modelP;
    CAPEV_EFFECT *eff;
    CAPEV_EFFDATA *data;
    int i;
    float t;
    float u;
    float v;
    float c;
    float s1;
    float s2;
    float s3;
    if(mbExitCheck() || ev_CapEffGlowOMObj[work->no] == (OMOBJ *)-1) {
        Hu3DModelKill(work->mdlId);
        work->mdlId = -1;
        HuSprAnimKill(work->anim);
        work->anim = NULL;
        ev_CapEffGlowOMObj[work->no] = NULL;
        omDelObjEx(mbObjMan, obj);
        return;
    }
    if(work->num <= 0) {
        Hu3DModelAttrSet(work->mdlId, HU3D_ATTR_DISPOFF);
        return;
    }
    Hu3DModelAttrReset(work->mdlId, HU3D_ATTR_DISPOFF);
    modelP = &Hu3DData[work->mdlId];
    eff = modelP->hookData;
    data = eff->data;
    eff->unk23 = 0;
    for(i=0; i<eff->num; i++, data++) {
        if(data->scale <= 0.0f) {
            continue;
        }
        data->pos.x += data->vel.x;
        data->pos.y += data->vel.y;
        data->pos.z += data->vel.z;
        if(data->gravity) {
            data->vel.y -= data->gravity;
        }
        switch(data->time) {
            case 0:
                data->scale = data->alpha*data->alphaSpeed;
                break;
            case 1:
                t = data->maxTime/100.0f;
                u = 1.0f-t;
                if(data->alphaSpeed > t) {
                    v = (1.0f/u)*(data->alphaSpeed-t);
                    c = mbCosDeg(90.0f*v);
                    data->scale = data->alpha*c;
                } else {
                    v = data->alphaSpeed*(1.0f/t);
                    s1 = mbSinDeg(90.0f*v);
                    data->scale = data->alpha*s1;
                }
                break;
            case 2:
                t = (float)(++data->count)/(float)data->maxTime;
                u = 360.0f*t;
                s2 = mbSinDeg(u);
                data->pos.x += 10.0f*(data->alphaSpeed*s2);
                break;
            case 3:
                t = (float)(++data->count)/(float)data->maxTime;
                u = 180.0f+(360.0f*t);
                s3 = mbSinDeg(u);
                data->pos.x += 10.0f*(data->alphaSpeed*s3);
                break;
        }
        data->alphaSpeed -= data->speed;
        data->rot.z += data->rotSpeed;
        if(data->alphaSpeed <= 0.0f) {
            data->unk68 = 0;
            data->time = 0;
            data->scale = 0.0f;
            work->num--;
        }
    }
}

void mbev_CapEffGlowKill(OMOBJ *obj)
{
    int i;
    for(i=0; i<8; i++) {
        if(ev_CapEffGlowOMObj[i] == obj) {
            break;
        }
    }
    ev_CapEffGlowOMObj[i] = (OMOBJ *)-1;
}

int mbev_CapEffGlowDispGet(OMOBJ *obj)
{
    int i;
    CAPEFFWORK *work;
    for(i=0; i<8; i++) {
        if(ev_CapEffGlowOMObj[i] == obj) {
            break;
        }
    }
    work = obj->data;
    return work->num;
}

void mbev_CapEffGlowPatSet(OMOBJ *obj, int pat)
{
    int i;
    CAPEFFWORK *work;
    CAPEV_EFFECT *eff;
    HU3D_MODEL *modelP;
    for(i=0; i<8; i++) {
        if(ev_CapEffGlowOMObj[i] == obj) {
            break;
        }
    }
    work = obj->data;
    modelP = &Hu3DData[work->mdlId];
    eff = modelP->hookData;
    eff->blendMode = (u8)pat;
}

void mbev_CapEffGlowBlendModeSet(OMOBJ *obj, int blendMode)
{
    int i;
    CAPEFFWORK *work;
    CAPEV_EFFECT *eff;
    HU3D_MODEL *modelP;
    for(i=0; i<8; i++) {
        if(ev_CapEffGlowOMObj[i] == obj) {
            break;
        }
    }
    work = obj->data;
    modelP = &Hu3DData[work->mdlId];
    eff = modelP->hookData;
    eff->pat = (u8)blendMode;
}

void mbev_CapEffGlowAnimSet(OMOBJ *obj, int dataNum)
{
    int i;
    CAPEFFWORK *work;
    CAPEV_EFFECT *eff;
    HU3D_MODEL *modelP;
    for(i=0; i<8; i++) {
        if(ev_CapEffGlowOMObj[i] == obj) {
            break;
        }
    }
    work = obj->data;
    modelP = &Hu3DData[work->mdlId];
    eff = modelP->hookData;
    HuSprAnimKill(work->anim);
    work->anim = eff->anim = HuSprAnimRead(HuDataReadNum(dataNum, HU_MEMNUM_OVL));
}

int mbev_CapEffGlowAdd(OMOBJ *obj, HuVecF pos, HuVecF vel, int time, float scale, float rotSpeed, float gravity, GXColor color)
{
    int i;
    CAPEFFWORK *work;
    HU3D_MODEL *modelP;
    CAPEV_EFFECT *eff;
    CAPEV_EFFDATA *data;
    for(i=0; i<8; i++) {
        if(ev_CapEffGlowOMObj[i] == obj) {
            break;
        }
    }
    work = obj->data;
    modelP = &Hu3DData[work->mdlId];
    eff = modelP->hookData;
    data = eff->data;
    for(i=0; i<eff->num; i++, data++) {
        if(data->scale <= 0.0f) {
            break;
        }
    }
    if(i >= eff->num) {
        return -1;
    }
    data->time = data->maxTime = 0;
    data->pos.x = pos.x;
    data->pos.y = pos.y;
    data->pos.z = pos.z;
    data->vel.x = vel.x;
    data->vel.y = vel.y;
    data->vel.z = vel.z;
    data->alpha = scale;
    data->alphaSpeed = 1.0f;
    if(time > 1) {
        data->speed = 1.0f/time;
    } else {
        data->speed = 1.0f;
    }
    data->gravity = gravity;
    data->rotSpeed = rotSpeed;
    data->scale = scale;
    data->color = color;
    data->rot.z = 360.0f*CAP_RANDF();
    data->unk68 = 0;
    data->time = 0;
    work->num++;
    return i;
}

int mbev_CapEffGlowKinokoAdd(OMOBJ *obj, HuVecF pos, int time, float scale, float rangeX, float rangeY, float rangeZ, int type, GXColor *colorP)
{
    int i;
    float rotSpeed;
    float gravity;
    float t;
    HuVecF p;
    HuVecF vel;
    GXColor color;
    for(i=0; i<8; i++) {
        if(ev_CapEffGlowOMObj[i] == NULL) {
            break;
        }
    }
    p.x = pos.x+(rangeX*(-0.5f+CAP_RANDF()));
    p.y = pos.y+(rangeY*(-0.5f+CAP_RANDF()));
    p.z = pos.z+(rangeZ*CAP_RANDF());
    vel.x = 0.0f;
    vel.z = 0.0f;
    switch(type) {
        case 1:
            vel.y = -1.0f*(1.0f+(0.2f*CAP_RANDF()));
            rotSpeed = 0.0f;
            gravity = 0.0f;
            if(mbRandMod(32768) & 1) {
                rotSpeed *= -1.0f;
            }
            break;
        case 2:
            vel.y = 1.0f+(0.2f*CAP_RANDF());
            rotSpeed = 0.05f+(0.02f*CAP_RANDF());
            gravity = 0.0f;
            if(mbRandMod(32768) & 1) {
                rotSpeed *= -1.0f;
            }
            break;
        case 3:
            vel.y = 0.0f;
            rotSpeed = 0.05f+(0.02f*CAP_RANDF());
            gravity = 0.08166666f;
            if(mbRandMod(32768) & 1) {
                rotSpeed *= -1.0f;
            }
            break;
        default:
            vel.y = 0.0f;
            rotSpeed = 0.0f;
            gravity = 0.0f;
            break;
    }
    if(colorP != NULL) {
        color = *colorP;
    } else {
        t = CAP_RANDF();
        color.r = 192.0f+(63.0f*t);
        color.g = 192.0f+(63.0f*t);
        color.b = 192.0f+(63.0f*t);
        color.a = 192.0f+(63.0f*CAP_RANDF());
    }
    return mbev_CapEffGlowAdd(obj, p, vel, time, scale, rotSpeed, gravity, color);
}

void mbev_CapEffGlowKinokoAddAlt(OMOBJ *obj, HuVecF pos, int time, float scale, float rangeX, float rangeY, float rangeZ, int type)
{
    GXColor color;
    color = ev_CapsuleRandomColorTbl[mbRandMod(7)];
    color.a = 192.0f+(63.0f*CAP_RANDF());
    mbev_CapEffGlowKinokoAdd(obj, pos, time, scale, rangeX, rangeY, rangeZ, type, &color);
}

BOOL mbev_CapEffGlowKinokoTimeSet(OMOBJ *obj, int no, int mode, int maxTime)
{
    int i;
    CAPEFFWORK *work;
    HU3D_MODEL *modelP;
    CAPEV_EFFECT *eff;
    CAPEV_EFFDATA *data;
    for(i=0; i<8; i++) {
        if(ev_CapEffGlowOMObj[i] == obj) {
            break;
        }
    }
    work = obj->data;
    modelP = &Hu3DData[work->mdlId];
    eff = modelP->hookData;
    data = eff->data;
    if(no < 0 || no >= eff->num) {
        return FALSE;
    }
    data = &eff->data[no];
    data->time = (s16)mode;
    data->maxTime = (s16)maxTime;
    data->count = 0;
    return TRUE;
}

void mbev_CapEffGlowCoinAdd(OMOBJ *obj, HuVecF *pos, HuVecF *rot)
{
    int i;
    float a;
    float b;
    float r;
    Mtx mtx;
    HuVecF vel;
    GXColor color;
    if(rot != NULL) {
        mtxRot(mtx, rot->x, rot->y, rot->z);
    } else {
        MTXIdentity(mtx);
    }
    for(i=0; i<16; i++) {
        a = (45.0f*i)+(10.0f*(-0.5f+CAP_RANDF()));
        if(i & 1) {
            b = 10.0f*CAP_RANDF();
        } else {
            b = -10.0f*CAP_RANDF();
        }
        r = 5.0f*(0.5f+CAP_RANDF());
        vel.x = r*(CapCosDeg(b)*CapSinDeg(a));
        vel.y = r*CapSinDeg(b);
        vel.z = r*(CapCosDeg(b)*CapCosDeg(a));
        MTXMultVec(mtx, &vel, &vel);
        color.r = 255;
        color.g = 255;
        color.b = 0;
        color.a = 255;
        mbev_CapEffGlowAdd(obj, *pos, vel, 100.0f*(0.5f+(0.3f*CAP_RANDF())), 60.0f*(0.5f+(0.25f*CAP_RANDF())), 0.0f, 0.16333334f, color);
    }
}

void mbev_CapEffRingOMExec(OMOBJ *obj);

OMOBJ *mbev_CapEffRingCreate(void)
{
    int j;
    int i;
    OMOBJ *obj;
    CAPEFFRINGWORK *work;
    HU3D_MODEL *modelP;
    int mdlId;
    ANIMDATA *anim;
    CAPEV_EFFECT *eff;
    for(i=0; i<8; i++) {
        if(ev_CapEffRingOMObj[i] == NULL) {
            break;
        }
    }
    obj = ev_CapEffRingOMObj[i] = omAddObjEx(mbObjMan, 32768, 0, 0, -1, mbev_CapEffRingOMExec);
    work = obj->data = CapMalloc(sizeof(CAPEFFRINGWORK));
    memset(work, 0, sizeof(CAPEFFRINGWORK));
    work->num = 0;
    work->no = i;
    for(j=0; j<3; j++) {
        anim = HuSprAnimRead(HuDataReadNum(ev_CapEffRingFile[j], HU_MEMNUM_OVL));
        work->anim[j] = anim;
        work->mdlId[j] = mdlId = ev_CapEffCreate(anim, 32);
        Hu3DModelLayerSet(mdlId, 5);
        work->num = 0;
        modelP = &Hu3DData[mdlId];
        eff = modelP->hookData;
        eff->blendMode = 1;
        eff->unk22 = 79;
    }
    return obj;
}

OMOBJ *mbev_CapEffRingHitCreate(void)
{
    int j;
    int i;
    OMOBJ *obj;
    CAPEFFRINGWORK *work;
    HU3D_MODEL *modelP;
    ANIMDATA *anim;
    int mdlId;
    CAPEV_EFFECT *eff;
    for(i=0; i<8; i++) {
        if(ev_CapEffRingOMObj[i] == NULL) {
            break;
        }
    }
    obj = ev_CapEffRingOMObj[i] = omAddObjEx(mbObjMan, 32768, 0, 0, -1, mbev_CapEffRingOMExec);
    work = obj->data = CapMalloc(sizeof(CAPEFFRINGWORK));
    memset(work, 0, sizeof(CAPEFFRINGWORK));
    work->num = 0;
    work->no = i;
    for(j=0; j<3; j++) {
        if(j == 0) {
            work->anim[j] = anim = ringHitEffAnim2;
        } else {
            work->anim[j] = anim = ringHitEffAnim1;
        }
        work->mdlId[j] = mdlId = ev_CapEffCreate(anim, 32);
        Hu3DModelLayerSet(mdlId, 5);
        work->num = 0;
        modelP = &Hu3DData[mdlId];
        eff = modelP->hookData;
        eff->blendMode = 1;
        eff->unk22 = 79;
    }
    return obj;
}

/* @@PART4@@ */
/* Part 4: CapEffRing (from OMExec) .. CapEffCoinMultiAdd. Helpers and types only part 4 uses. */








static inline void *EvMalloc(int size)
{
    return HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
}

static inline void *EvEffModelMalloc(u32 size, u32 num)
{
    return HuMemDirectMallocNum(HEAP_MODEL, size, num);
}

static inline float EvSqrtF(float x)
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

static inline s16 EvEffRand(void)
{
    if(++mbCapEffNum >= 1024) {
        mbCapEffNum = 0;
    }
    return mbCapEffData[mbCapEffNum];
}

static inline float EvEffRandF(void)
{
    return (1.0f/32767.0f)*EvEffRand();
}


typedef struct CapEffElectricData_s {
    /* 0x00 */ int no;
    /* 0x04 */ int count;
    /* 0x08 */ int num;
    /* 0x0C */ int time;
    /* 0x10 */ int interval;
    /* 0x14 */ HuVecF basePos;
    /* 0x20 */ HuVecF curPos;
    /* 0x2C */ HuVecF prevPos;
    /* 0x38 */ HuVecF unk38;
    /* 0x44 */ HuVecF hist[6];
    /* 0x8C */ int objId;
    /* 0x90 */ HuVecF ofs;
} CAPEFFELECTRICDATA;

typedef struct CapEffElectricWork_s {
    /* 0x00 */ int mdlId;
    /* 0x04 */ int dispNum;
    /* 0x08 */ int no;
    /* 0x0C */ ANIMDATA *anim;
    /* 0x10 */ CAPEFFELECTRICDATA data[32];
} CAPEFFELECTRICWORK;

typedef struct CapEffRayData_s {
    /* 0x000 */ int no;
    /* 0x004 */ int activeF;
    /* 0x008 */ int time;
    /* 0x00C */ int maxTime;
    /* 0x010 */ float scale;
    /* 0x014 */ float curScale;
    /* 0x018 */ HuVecF pos;
    /* 0x024 */ HuVecF rotStart;
    /* 0x030 */ HuVecF rotEnd;
    /* 0x03C */ HuVecF rot;
    /* 0x048 */ HuVecF vtx[16];
    /* 0x108 */ GXColor color[16];
    /* 0x148 */ HuVecF vtxDraw[16];
    /* 0x208 */ GXColor colorDraw[8];
} CAPEFFRAYDATA;

typedef struct CapEffRayWork_s {
    /* 0x00 */ int mdlId;
    /* 0x04 */ int no;
    /* 0x08 */ float alpha;
    /* 0x0C */ void *dl;
    /* 0x10 */ u32 dlSize;
    /* 0x14 */ CAPEFFRAYDATA *data;
} CAPEFFRAYWORK;

typedef struct CapEffMasuHitWork_s {
    /* 0x00 */ int mdlId;
    /* 0x04 */ int dispNum;
    /* 0x08 */ int no;
    /* 0x0C */ ANIMDATA *anim;
} CAPEFFMASUHITWORK;

/* obj->data is CAPEFFCOINDATA[128]; the first entry's no is the object's slot */

void mbev_CapEffRingOMExec(OMOBJ *obj)
{
    CAPEFFRINGWORK *work;
    int i;
    int j;
    HU3D_MODEL *modelP;
    CAPEV_EFFECT *effP;
    CAPEV_EFFDATA *effDataP;
    float t;

    work = obj->data;
    if(mbExitCheck() || ev_CapEffRingOMObj[work->no] == (OMOBJ *)-1) {
        for(i=0; i<3; i++) {
            Hu3DModelKill(work->mdlId[i]);
            work->mdlId[i] = -1;
        }
        for(i=0; i<3; i++) {
            HuSprAnimKill(work->anim[i]);
            work->anim[i] = NULL;
        }
        ev_CapEffRingOMObj[work->no] = NULL;
        omDelObjEx(mbObjMan, obj);
        return;
    }
    for(i=0; i<3; i++) {
        if(work->dispNum <= 0) {
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
                switch(effDataP->work[0]) {
                    case 0:
                        t = ++effDataP->work[1]/effDataP->vec2.x;
                        t = CapSinDeg(90.0f*t);
                        effDataP->scale = effDataP->vec1.z*(effDataP->vec1.x+(t*(1.0f-effDataP->vec1.x)));
                        effDataP->color.a = effDataP->vec2.z*t;
                        if(t >= 1.0f) {
                            effDataP->scale = effDataP->vec1.z;
                            effDataP->color.a = effDataP->vec2.z;
                            effDataP->work[0]++;
                            effDataP->work[1] = 0;
                        }
                        break;

                    case 1:
                        t = ++effDataP->work[1]/effDataP->vec2.y;
                        t = CapSinDeg(90.0f*t);
                        effDataP->scale = effDataP->vec1.z*(1.0f+(t*(effDataP->vec1.y-1.0f)));
                        effDataP->color.a = effDataP->vec2.z*(1.0f-t);
                        if(t >= 1.0f) {
                            effDataP->scale = 0.0f;
                            work->dispNum--;
                        }
                        break;
                }
            }
        }
    }
}

void mbev_CapEffRingKill(OMOBJ *obj)
{
    int i;
    for(i=0; i<8; i++) {
        if(ev_CapEffRingOMObj[i] == obj) {
            break;
        }
    }
    ev_CapEffRingOMObj[i] = (OMOBJ *)-1;
}

int mbev_CapEffRingDispGet(OMOBJ *obj)
{
    int i;
    CAPEFFRINGWORK *work;
    for(i=0; i<8; i++) {
        if(ev_CapEffRingOMObj[i] == obj) {
            break;
        }
    }
    work = obj->data;
    return work->dispNum;
}

int mbev_CapEffRingAdd(OMOBJ *obj, HuVecF pos, HuVecF rot, HuVecF scale, int inTime, int outTime, int no, GXColor color)
{
    CAPEFFRINGWORK *work;
    int i;
    HU3D_MODEL *modelP;
    int j;
    CAPEV_EFFECT *effP;
    CAPEV_EFFDATA *effDataP;
    for(i=0; i<8; i++) {
        if(ev_CapEffRingOMObj[i] == obj) {
            break;
        }
    }
    work = obj->data;
    modelP = &Hu3DData[work->mdlId[no%3]];
    effP = modelP->hookData;
    effDataP = effP->data;
    for(j=0; j<effP->num; j++, effDataP++) {
        if(effDataP->scale <= 0.0f) {
            break;
        }
    }
    if(j >= effP->num) {
        return -1;
    }
    effDataP->work[0] = effDataP->work[1] = 0;
    effDataP->pos.x = pos.x;
    effDataP->pos.y = pos.y;
    effDataP->pos.z = pos.z;
    effDataP->vec1.x = scale.x;
    effDataP->vec1.y = scale.y;
    effDataP->vec1.z = scale.z;
    effDataP->vec2.x = inTime;
    effDataP->vec2.y = outTime;
    effDataP->vec2.z = color.a;
    effDataP->scale = scale.z;
    effDataP->color.r = color.r;
    effDataP->color.g = color.g;
    effDataP->color.b = color.b;
    effDataP->color.a = color.a;
    effDataP->rot.x = rot.x;
    effDataP->rot.y = rot.y;
    effDataP->rot.z = rot.z;
    effDataP->no = 0;
    effDataP->work[0] = 0;
    work->dispNum++;
    return j;
}

void mbev_CapEffRingHitAdd(OMOBJ *obj, HuVecF pos, HuVecF rot, HuVecF scale)
{
    mbev_CapEffRingAdd(obj, pos, rot, scale, 1, 12, 2, capsuleRingColor);
}

void mbev_CapEffRingAnimSet(OMOBJ *obj, int no, int dataNum)
{
    CAPEFFRINGWORK *work;
    CAPEV_EFFECT *effP;
    HU3D_MODEL *modelP;
    int i;
    for(i=0; i<8; i++) {
        if(ev_CapEffRingOMObj[i] == obj) {
            break;
        }
    }
    work = obj->data;
    modelP = &Hu3DData[work->mdlId[no%3]];
    effP = modelP->hookData;
    HuSprAnimKill(work->anim[no%3]);
    work->anim[no%3] = effP->anim = HuSprAnimRead(HuDataReadNum(dataNum, HU_MEMNUM_OVL));
}

OMOBJ *mbev_CapEffElectricCreate(void)
{
    CAPEFFELECTRICWORK *work;
    int i;
    CAPEFFELECTRICDATA *elecP;
    CAPEV_EFFECT *effP;
    int j = 0;
    OMOBJ *obj;
    HU3D_MODEL *modelP;
    for(i=0; i<8; i++) {
        if(ev_CapEffElectricOMObj[i] == NULL) {
            break;
        }
    }
    obj = ev_CapEffElectricOMObj[i] = omAddObjEx(mbObjMan, 32768, 0, 0, -1, mbev_CapEffElectricOMExec);
    work = obj->data = EvMalloc(sizeof(CAPEFFELECTRICWORK));
    memset(work, 0, sizeof(CAPEFFELECTRICWORK));
    work->anim = electricEffAnim;
    work->mdlId = ev_CapEffCreate(work->anim, 192);
    ev_CapEffGridSet(work->mdlId, 4, 1, 0);
    Hu3DModelLayerSet(work->mdlId, 5);
    modelP = &Hu3DData[work->mdlId];
    effP = modelP->hookData;
    effP->dispAttr = 0x5D;
    effP->blendMode = 1;
    work->dispNum = 0;
    work->no = i;
    for(elecP=work->data, j=0; j<32; j++, elecP++) {
        elecP->no = -1;
    }
    return obj;
}

void mbev_CapEffElectricOMExec(OMOBJ *obj)
{
    CAPEFFELECTRICWORK *work;
    CAPEFFELECTRICDATA *elecP;
    CAPEV_EFFECT *effP;
    HU3D_MODEL *modelP;
    CAPEV_EFFDATA *effDataP;
    int i;
    float len;
    HuVecF pos;
    HuVecF dir;
    HuVecF ofs;

    work = obj->data;
    elecP = work->data;
    if(mbExitCheck() || ev_CapEffElectricOMObj[work->no] == (OMOBJ *)-1) {
        Hu3DModelKill(work->mdlId);
        work->mdlId = -1;
        HuSprAnimKill(work->anim);
        work->anim = NULL;
        ev_CapEffElectricOMObj[work->no] = NULL;
        omDelObjEx(mbObjMan, obj);
        return;
    }
    modelP = &Hu3DData[work->mdlId];
    effP = modelP->hookData;
    effDataP = effP->data;
    for(i=0; i<32; i++, elecP++, effDataP += 6) {
        if(elecP->no < 0) {
            continue;
        }
        if(++elecP->time < elecP->interval) {
            continue;
        }
        elecP->time = 0;
        if(elecP->objId < 0) {
            pos = elecP->basePos;
        } else {
            mbObjPosGet(elecP->objId, &pos);
            VECAdd(&pos, &elecP->ofs, &pos);
            VECSubtract(&pos, &elecP->prevPos, &ofs);
            for(i=0; i<6; i++) {
                VECAdd(&effDataP[i].pos, &ofs, &effDataP[i].pos);
            }
        }
        pos.x += 2.5f*(100.0f*(-0.5f+CAP_RANDF()));
        pos.y += 2.5f*(100.0f*(-0.5f+CAP_RANDF()));
        pos.z += 2.5f*(100.0f*(-0.5f+CAP_RANDF()));
        elecP->prevPos = elecP->curPos;
        elecP->curPos = pos;
        for(i=0; i<5; i++) {
            elecP->hist[i+1] = elecP->hist[i];
        }
        for(i=0; i<5; i++) {
            effDataP[i+1] = effDataP[i];
        }
        VECSubtract(&elecP->curPos, &elecP->prevPos, &dir);
        VECScale(&dir, &dir, 0.5f);
        VECAdd(&elecP->prevPos, &dir, &effDataP->pos);
        VECSubtract(&elecP->curPos, &elecP->prevPos, &dir);
        len = EvSqrtF((dir.x*dir.x)+(dir.z*dir.z));
        effDataP->rot.x = 180.0*(atan2(-dir.y, len)/M_PI);
        effDataP->rot.y = 90.0+(180.0*(atan2(dir.x, dir.z)/M_PI));
        effDataP->rot.z = 0.0f;
        effDataP->no = mbRandMod(4);
        len = VECMag(&dir);
        if(len <= 0.0f) {
            len = 0.01f;
        }
        effDataP->scale = len;
        if(++elecP->count >= elecP->num) {
            elecP->no = -1;
            for(i=0; i<6; i++) {
                effDataP[i].scale = 0.0f;
            }
            work->dispNum--;
        }
    }
}

void mbev_CapEffElectricKill(OMOBJ *obj)
{
    int i;
    for(i=0; i<8; i++) {
        if(ev_CapEffElectricOMObj[i] == obj) {
            break;
        }
    }
    (void)(i >= 8);
    ev_CapEffElectricOMObj[i] = (OMOBJ *)-1;
}

int mbev_CapEffElectricDispGet(OMOBJ *obj)
{
    int i;
    CAPEFFELECTRICWORK *work;
    for(i=0; i<8; i++) {
        if(ev_CapEffElectricOMObj[i] == obj) {
            break;
        }
    }
    (void)(i >= 8);
    work = obj->data;
    return work->dispNum;
}

int mbev_CapEffElectricAdd(OMOBJ *obj, HuVecF pos, int num, int interval)
{
    CAPEFFELECTRICDATA *elecP;
    CAPEV_EFFDATA *effDataP;
    int j;
    CAPEFFELECTRICWORK *work;
    int i;
    int k;
    CAPEV_EFFECT *effP;
    HU3D_MODEL *modelP;
    for(i=0; i<8; i++) {
        if(ev_CapEffElectricOMObj[i] == obj) {
            break;
        }
    }
    (void)(i >= 8);
    work = obj->data;
    elecP = work->data;
    modelP = &Hu3DData[work->mdlId];
    effP = modelP->hookData;
    effDataP = effP->data;
    for(j=0; j<32; j++, elecP++, effDataP += 6) {
        if(elecP->no < 0) {
            break;
        }
    }
    if(j >= 32) {
        return -1;
    }
    effDataP->pos = pos;
    effDataP->rot.x = effDataP->rot.y = effDataP->rot.z = 0.0f;
    effDataP->no = mbRandMod(4);
    effDataP->scale = 0.0f;
    effDataP->color = ev_CapEffElectricColor[mbRandMod(4)];
    for(k=0; k<6; k++, effDataP++) {
        effDataP->scale = 0.0f;
    }
    elecP->no = j;
    elecP->basePos = elecP->curPos = elecP->prevPos = pos;
    elecP->count = 0;
    elecP->num = num;
    elecP->time = 0;
    elecP->interval = interval;
    elecP->objId = -1;
    work->dispNum++;
    return j;
}

void mbev_CapEffElectricModelSet(OMOBJ *obj, int objId, int no, HuVecF *ofs)
{
    CAPEFFELECTRICDATA *elecP;
    int i;
    int j;
    CAPEFFELECTRICWORK *work;
    CAPEV_EFFECT *effP;
    CAPEV_EFFDATA *effDataP;
    HU3D_MODEL *modelP;
    for(i=0; i<8; i++) {
        if(ev_CapEffElectricOMObj[i] == obj) {
            break;
        }
    }
    (void)(i >= 8);
    work = obj->data;
    elecP = work->data;
    modelP = &Hu3DData[work->mdlId];
    effP = modelP->hookData;
    effDataP = effP->data;
    for(j=0; j<32; j++, elecP++, effDataP += 6) {
        if(elecP->no == no) {
            break;
        }
    }
    if(j < 32) {
        elecP->objId = objId;
        if(ofs != NULL) {
            elecP->ofs = *ofs;
        } else {
            elecP->ofs.x = elecP->ofs.y = elecP->ofs.z = 0.0f;
        }
    }
}

OMOBJ *mbev_CapEffRayCreate(float ofsY, float spread)
{
    int i;
    OMOBJ *obj;
    CAPEFFRAYWORK *work;
    HU3D_MODEL *modelP;
    int j;
    CAPEFFRAYDATA *rayP;
    int k;
    float t;
    void *dlBuf;
    void *dlBegin;
    for(i=0; i<8; i++) {
        if(ev_CapEffRayOMObj[i] == NULL) {
            break;
        }
    }
    obj = omAddObjEx(mbObjMan, 32768, 0, 0, -1, mbev_CapEffRayOMExec);
    ev_CapEffRayOMObj[i] = obj;
    work = obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(CAPEFFRAYWORK), HU_MEMNUM_OVL);
    memset(work, 0, sizeof(CAPEFFRAYWORK));
    work->no = i;
    work->alpha = 1.0f;
    work->mdlId = Hu3DHookFuncCreate(mbev_CapEffRayDraw);
    Hu3DModelCameraSet(work->mdlId, HU3D_CAM0);
    Hu3DModelLayerSet(work->mdlId, 5);
    modelP = &Hu3DData[work->mdlId];
    modelP->hookData = work;
    rayP = work->data = EvEffModelMalloc(128*sizeof(CAPEFFRAYDATA), modelP->mallocNo);
    memset(rayP, 0, 128*sizeof(CAPEFFRAYDATA));
    for(j=0; j<128; j++, rayP++) {
        rayP->no = j;
        rayP->activeF = FALSE;
        rayP->time = 0;
        rayP->maxTime = 0;
        rayP->scale = 1.0f;
        rayP->pos.x = rayP->pos.y = rayP->pos.z = 0.0f;
        rayP->rotStart.x = rayP->rotStart.y = rayP->rotStart.z = 0.0f;
        rayP->rotEnd.x = rayP->rotEnd.y = rayP->rotEnd.z = 0.0f;
        for(k=0; k<16; k+=2) {
            t = (k/2)/7.0f;
            rayP->vtx[k].x = -0.05f-(spread*t);
            rayP->vtx[k].y = ofsY+t;
            rayP->vtx[k].z = 0.0f;
            rayP->vtx[k+1].x = 0.05f+(spread*t);
            rayP->vtx[k+1].y = ofsY+t;
            rayP->vtx[k+1].z = 0.0f;
            rayP->vtxDraw[k] = rayP->vtx[k];
            rayP->vtxDraw[k+1] = rayP->vtx[k+1];
        }
        for(k=0; k<16; k++) {
            t = k/7.0f;
            mbev_CapEffColorSet(&rayP->color[k], mbRandMod(0x8000));
            rayP->color[k].a = 255.0*sin((M_PI*(180.0f*t))/180.0);
        }
    }
    DCFlushRangeNoSync(work->data, 128*sizeof(CAPEFFRAYDATA));
    dlBegin = dlBuf = EvEffModelMalloc(0x10000, modelP->mallocNo);
    DCFlushRange(dlBuf, 0x10000);
    GXBeginDisplayList(dlBegin, 0x10000);
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 16);
    for(j=0; j<8; j++) {
        GXPosition1x16(j*2);
        GXColor1x16(j);
        GXPosition1x16((j*2)+1);
        GXColor1x16(j);
    }
    GXEnd();
    work->dlSize = GXEndDisplayList();
    work->dl = EvEffModelMalloc(work->dlSize, modelP->mallocNo);
    memcpy(work->dl, dlBuf, work->dlSize);
    DCFlushRange(work->dl, work->dlSize);
    HuMemDirectFree(dlBuf);
    return obj;
}

void mbev_CapEffRayOMExec(OMOBJ *obj)
{
    CAPEFFRAYWORK *work;
    CAPEFFRAYDATA *rayP;
    int i;
    int j;
    float t;
    float s;
    work = obj->data;
    if(mbExitCheck() || ev_CapEffRayOMObj[work->no] == (OMOBJ *)-1) {
        Hu3DModelKill(work->mdlId);
        ev_CapEffRayOMObj[work->no] = NULL;
        omDelObjEx(mbObjMan, obj);
        return;
    }
    for(rayP=work->data, i=0; i<128; i++, rayP++) {
        if(!rayP->activeF) {
            continue;
        }
        t = (float)(++rayP->time)/rayP->maxTime;
        if(t >= 1.0f) {
            rayP->activeF = FALSE;
            continue;
        }
        s = mbSinDeg(180.0f*t);
        rayP->curScale = rayP->scale*s;
        rayP->rot.x = mbev_CapAngleSumLerp(t, rayP->rotStart.x, rayP->rotEnd.x);
        rayP->rot.y = mbev_CapAngleSumLerp(t, rayP->rotStart.y, rayP->rotEnd.y);
        rayP->rot.z = mbev_CapAngleSumLerp(t, rayP->rotStart.z, rayP->rotEnd.z);
        for(j=0; j<8; j++) {
            mbev_CapColorLerp(t, &rayP->color[j], &rayP->color[j+8], &rayP->colorDraw[j]);
            rayP->colorDraw[j].a = rayP->color[j].a*work->alpha;
        }
    }
    DCFlushRangeNoSync(work->data, 128*sizeof(CAPEFFRAYDATA));
}

void mbev_CapEffRayKill(OMOBJ *obj)
{
    int i;
    CAPEFFRAYWORK *work;
    for(i=0; i<8; i++) {
        if(ev_CapEffRayOMObj[i] == obj) {
            break;
        }
    }
    work = obj->data;
    ev_CapEffRayOMObj[work->no] = (OMOBJ *)-1;
}

void mbev_CapEffRayDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    CAPEFFRAYWORK *work;
    CAPEFFRAYDATA *rayP;
    int i;
    Mtx modelMtx;
    work = modelP->hookData;
    rayP = work->data;
    GXSetNumTevStages(1);
    GXSetNumTexGens(0);
    GXSetNumChans(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetZCompLoc(GX_FALSE);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    for(i=0; i<128; i++, rayP++) {
        if(!rayP->activeF) {
            continue;
        }
        MTXIdentity(modelMtx);
        modelMtx[0][0] = 1.0f-rayP->curScale;
        modelMtx[1][1] = rayP->curScale;
        modelMtx[2][2] = rayP->curScale;
        mbMtxRotXDeg(modelMtx, rayP->rot.x);
        mbMtxRotYDeg(modelMtx, rayP->rot.y);
        mbMtxRotZDeg(modelMtx, rayP->rot.z);
        mbMtxTransCat(modelMtx, rayP->pos.x, rayP->pos.y, rayP->pos.z);
        MTXConcat(*mtx, modelMtx, modelMtx);
        GXLoadPosMtxImm(modelMtx, GX_PNMTX0);
        GXSetArray(GX_VA_POS, rayP->vtxDraw, sizeof(HuVecF));
        GXSetArray(GX_VA_CLR0, rayP->colorDraw, sizeof(GXColor));
        GXCallDisplayList(work->dl, work->dlSize);
    }
}

int mbev_CapEffRayAdd(OMOBJ *obj, HuVecF *pos, HuVecF *rot, HuVecF *rotEnd, float scale, int time)
{
    int i;
    CAPEFFRAYWORK *work;
    int j;
    CAPEFFRAYDATA *rayP;
    for(i=0; i<8; i++) {
        if(ev_CapEffRayOMObj[i] == obj) {
            break;
        }
    }
    work = obj->data;
    rayP = work->data;
    for(j=0; j<128; j++, rayP++) {
        if(!rayP->activeF) {
            break;
        }
    }
    if(j >= 128) {
        return -1;
    }
    rayP->activeF = TRUE;
    rayP->time = 0;
    rayP->maxTime = time;
    rayP->scale = scale;
    rayP->pos = *pos;
    rayP->rotStart = *rot;
    rayP->rotEnd = *rotEnd;
    return j;
}

void mbev_CapEffRayAlphaSet(OMOBJ *obj, float alpha)
{
    CAPEFFRAYWORK *work;
    int i;
    CAPEFFRAYDATA *rayP;
    for(i=0; i<8; i++) {
        if(ev_CapEffRayOMObj[i] == obj) {
            break;
        }
    }
    work = obj->data;
    rayP = work->data;
    work->alpha = alpha;
}

void mbev_CapEffRayTransformSet(OMOBJ *obj, HuVecF *pos, HuVecF *rot, Mtx *mtx)
{
    int i;
    CAPEFFRAYWORK *work;
    CAPEFFRAYDATA *rayP;
    for(i=0; i<8; i++) {
        if(ev_CapEffRayOMObj[i] == obj) {
            break;
        }
    }
    work = obj->data;
    rayP = work->data;
    if(pos != NULL) {
        Hu3DModelPosSet(work->mdlId, pos->x, pos->y, pos->z);
    }
    if(rot != NULL) {
        Hu3DModelRotSet(work->mdlId, rot->x, rot->y, rot->z);
    }
    if(mtx != NULL) {
        Hu3DModelMtxSet(work->mdlId, mtx);
    }
}

OMOBJ *mbev_CapEffMasuHitCreate(void)
{
    CAPEFFMASUHITWORK *work;
    CAPEV_EFFECT *effP;
    int i;
    CAPEV_EFFDATA *effDataP;
    OMOBJ *obj;
    int j;
    HU3D_MODEL *modelP;
    for(i=0; i<8; i++) {
        if(ev_CapEffMasuHitOMObj[i] == NULL) {
            break;
        }
    }
    obj = omAddObjEx(mbObjMan, 32768, 0, 0, -1, mbev_CapEffMasuHitOMExec);
    ev_CapEffMasuHitOMObj[i] = obj;
    work = obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(CAPEFFMASUHITWORK), HU_MEMNUM_OVL);
    memset(work, 0, sizeof(CAPEFFMASUHITWORK));
    work->anim = HuSprAnimRead(HuDataReadNum(0xC0039, HU_MEMNUM_OVL));
    work->dispNum = 0;
    work->no = i;
    work->mdlId = ev_CapEffCreate(work->anim, 128);
    ev_CapEffGridSet(work->mdlId, 1, 4, 0);
    Hu3DModelLayerSet(work->mdlId, 5);
    modelP = &Hu3DData[work->mdlId];
    effP = modelP->hookData;
    effP->blendMode = 1;
    for(effDataP=effP->data, j=0; j<effP->num; j++, effDataP++) {
        mbev_CapEffColorSet(&effDataP->color, mbRandMod(0x8000));
        effDataP->scale = 0.0f;
    }
    return obj;
}

void mbev_CapEffMasuHitOMExec(OMOBJ *obj)
{
    CAPEFFMASUHITWORK *work;
    HU3D_MODEL *modelP;
    CAPEV_EFFECT *effP;
    CAPEV_EFFDATA *effDataP;
    int i;
    float t;
    float scale;
    float radius;
    HuVecF rot;

    work = obj->data;
    if(mbExitCheck() || ev_CapEffMasuHitOMObj[work->no] == (OMOBJ *)-1) {
        Hu3DModelKill(work->mdlId);
        work->mdlId = -1;
        HuSprAnimKill(work->anim);
        work->anim = NULL;
        ev_CapEffMasuHitOMObj[work->no] = NULL;
        omDelObjEx(mbObjMan, obj);
        return;
    }
    modelP = &Hu3DData[work->mdlId];
    effP = modelP->hookData;
    effDataP = effP->data;
    for(i=0; i<effP->num; i++, effDataP++) {
        if(effDataP->scale <= 0.0f) {
            continue;
        }
        t = (float)(++effDataP->work[0])/effDataP->work[1];
        if(t >= 1.0f) {
            effDataP->scale = 0.0f;
            continue;
        }
        scale = effDataP->val1*CapCosDeg(90.0f*t);
        radius = effDataP->val2;
        rot.x = mbev_CapAngleSumLerp(t, effDataP->vec2.x, effDataP->vec3.x);
        rot.y = mbev_CapAngleSumLerp(t, effDataP->vec2.y, effDataP->vec3.y);
        rot.z = mbev_CapAngleSumLerp(t, effDataP->vec2.z, effDataP->vec3.z);
        effDataP->pos.x = effDataP->vec1.x+(radius*(CapSinDeg(rot.x)*CapSinDeg(rot.y)));
        effDataP->pos.y = effDataP->vec1.y+(radius*CapCosDeg(rot.x));
        effDataP->pos.z = effDataP->vec1.z+(radius*(CapSinDeg(rot.x)*CapCosDeg(rot.y)));
        effDataP->scale = scale;
        effDataP->rot.z = rot.z;
        effDataP->color.a = (u8)(s32)(255.0f*CapSinDeg(180.0f*t));
    }
}

void mbev_CapEffMasuHitKill(OMOBJ *obj)
{
    int i;
    CAPEFFMASUHITWORK *work;
    for(i=0; i<8; i++) {
        if(ev_CapEffMasuHitOMObj[i] == obj) {
            break;
        }
    }
    work = obj->data;
    ev_CapEffMasuHitOMObj[work->no] = (OMOBJ *)-1;
}

int mbev_CapEffMasuHitAdd(OMOBJ *obj, HuVecF *pos, HuVecF *rot, HuVecF *rotEnd, float radius, float scale, int time)
{
    int i;
    CAPEFFMASUHITWORK *work;
    HU3D_MODEL *modelP;
    int j;
    CAPEV_EFFECT *effP;
    CAPEV_EFFDATA *effDataP;
    for(i=0; i<8; i++) {
        if(ev_CapEffMasuHitOMObj[i] == obj) {
            break;
        }
    }
    work = obj->data;
    modelP = &Hu3DData[work->mdlId];
    effP = modelP->hookData;
    effDataP = effP->data;
    for(j=0; j<effP->num; j++, effDataP++) {
        if(0.0f == effDataP->scale) {
            break;
        }
    }
    if(j >= effP->num) {
        return -1;
    }
    effDataP->work[0] = 0;
    effDataP->work[1] = time;
    effDataP->work[2] = 0;
    effDataP->scale = scale;
    effDataP->vec1 = *pos;
    effDataP->vec2 = *rot;
    effDataP->vec3 = *rotEnd;
    effDataP->val1 = scale;
    effDataP->val2 = radius;
    effDataP->no = mbRandMod(4);
    effDataP->color.a = 0;
    return j;
}

void mbev_CapEffMasuHitTransformSet(OMOBJ *obj, HuVecF *pos, HuVecF *rot, Mtx *mtx)
{
    int i;
    CAPEFFMASUHITWORK *work;
    for(i=0; i<8; i++) {
        if(ev_CapEffMasuHitOMObj[i] == obj) {
            break;
        }
    }
    work = obj->data;
    if(pos != NULL) {
        Hu3DModelPosSet(work->mdlId, pos->x, pos->y, pos->z);
    }
    if(rot != NULL) {
        Hu3DModelRotSet(work->mdlId, rot->x, rot->y, rot->z);
    }
    if(mtx != NULL) {
        Hu3DModelMtxSet(work->mdlId, mtx);
    }
}

OMOBJ *mbev_CapEffCoinCreate(void)
{
    int i;
    OMOBJ *obj;
    CAPEFFCOINDATA *work;
    int j;
    CAPEFFCOINDATA *coinP;
    for(i=0; i<8; i++) {
        if(ev_CapEffCoinOMObj[i] == NULL) {
            break;
        }
    }
    obj = ev_CapEffCoinOMObj[i] = omAddObjEx(mbObjMan, 32768, 0, 0, -1, mbev_CapEffCoinOMExec);
    coinP = work = obj->data = EvMalloc(128*sizeof(CAPEFFCOINDATA));
    memset(coinP, 0, 128*sizeof(CAPEFFCOINDATA));
    coinP->no = i;
    for(j=0; j<128; j++, coinP++) {
        coinP->coinId = -1;
        coinP->activeF = FALSE;
        coinP->type = 1;
        coinP->time = 0;
        coinP->count = 0;
        coinP->gravity = 0.0f;
        coinP->maxY = -1000000.0f;
        coinP->pos.x = coinP->pos.y = coinP->pos.z = 0.0f;
        coinP->rot.x = coinP->rot.y = coinP->rot.z = 0.0f;
        coinP->scale.x = coinP->scale.y = coinP->scale.z = 1.0f;
        coinP->vel.x = coinP->vel.y = coinP->vel.z = 0.0f;
    }
    return obj;
}

void mbev_CapEffCoinOMExec(OMOBJ *obj)
{
    CAPEFFCOINDATA *coinP;
    int i;
    int j;
    float speed;
    float angle;
    float t;
    float elev;
    HuVecF pos;
    HuVecF vel;
    GXColor color;

    coinP = obj->data;
    if(mbExitCheck() || ev_CapEffCoinOMObj[coinP->no] == (OMOBJ *)-1) {
        for(i=0; i<128; i++, coinP++) {
            if(coinP->coinId >= 0) {
                mbCoinObjNumDec(coinP->coinId);
            }
            coinP->coinId = -1;
        }
        coinP = obj->data;
        ev_CapEffCoinOMObj[coinP->no] = NULL;
        omDelObjEx(mbObjMan, obj);
        return;
    }
    for(i=0; i<128; i++, coinP++) {
        if(!coinP->activeF) {
            continue;
        }
        VECAdd(&coinP->pos, &coinP->vel, &coinP->pos);
        coinP->vel.y -= coinP->gravity;
        coinP->vel.y *= 0.95f;
        coinP->rot.y += coinP->rotSpeed;
        mbCoinObjPosSetV(coinP->coinId, &coinP->pos);
        mbCoinObjRotSetV(coinP->coinId, &coinP->rot);
        mbCoinObjScaleSetV(coinP->coinId, &coinP->scale);
        if(coinP->type == 0) {
            coinP->alpha -= coinP->alphaSpeed;
            speed = 1.0f-coinP->alpha;
            mbCoinObjAlphaSet(coinP->coinId, CapCosDeg(90.0f*(speed*speed)));
        }
        if(++coinP->count > coinP->time || (coinP->vel.y < 0.0f && coinP->pos.y < coinP->maxY)) {
            coinP->activeF = FALSE;
            if(coinP->coinId >= 0) {
                mbCoinObjNumDec(coinP->coinId);
            }
            coinP->coinId = -1;
            switch(coinP->type) {
                case 1:
                    for(j=0, angle=0.0f; j<16; j++) {
                        angle += 22.5f*(1.0f+CAP_RANDF());
                        pos = coinP->pos;
                        speed = 2.5f*(1.0f+CAP_RANDF());
                        elev = 45.0f-(90.0f*CAP_RANDF());
                        vel.x = speed*(CapSinDeg(angle)*CapCosDeg(elev));
                        vel.y = speed*(CapCosDeg(angle)*CapCosDeg(elev));
                        vel.z = speed*CapSinDeg(elev);
                        t = CAP_RANDF();
                        color.r = 192.0f+(63.0f*t);
                        color.g = 192.0f+(63.0f*t);
                        color.b = 64.0f+(63.0f*t);
                        color.a = 192.0f+(63.0f*CAP_RANDF());
                        if(coinP->glowEff) {
                            mbev_CapEffGlowAdd(coinP->glowEff, pos, vel, 60.0f*(1.0f+(0.5f*CAP_RANDF())), 100.0f*(0.3f+(0.1f*CAP_RANDF())), 0.0f, 0.0f, color);
                        }
                    }
                    break;

                case 2:
                    for(j=0, angle=0.0f; j<32; j++) {
                        angle += 22.5f*(1.0f+CAP_RANDF());
                        pos = coinP->pos;
                        speed = 2.5f*(1.0f+CAP_RANDF());
                        elev = 45.0f-(90.0f*CAP_RANDF());
                        vel.x = speed*(CapSinDeg(angle)*CapCosDeg(elev));
                        vel.y = speed*(CapCosDeg(angle)*CapCosDeg(elev));
                        vel.z = speed*CapSinDeg(elev);
                        t = CAP_RANDF();
                        color.r = 192.0f+(63.0f*t);
                        color.g = 192.0f+(63.0f*t);
                        color.b = 64.0f+(63.0f*t);
                        color.a = 192.0f+(63.0f*CAP_RANDF());
                        if(coinP->glowEff) {
                            mbev_CapEffGlowAdd(coinP->glowEff, pos, vel, 60.0f*(1.0f+(0.5f*CAP_RANDF())), 100.0f*(0.3f+(0.1f*CAP_RANDF())), 0.0f, 0.0f, color);
                        }
                    }
                    break;

                case 3:
                    for(j=0, angle=0.0f; j<16; j++) {
                        angle += 22.5f*(1.0f+CAP_RANDF());
                        pos = coinP->pos;
                        speed = 50.0f*(1.0f+(0.2f*CAP_RANDF()));
                        elev = -(75.0f+(15.0f*CAP_RANDF()));
                        vel.x = speed*(CapSinDeg(angle)*CapCosDeg(elev));
                        vel.y = speed*(CapCosDeg(angle)*CapCosDeg(elev));
                        vel.z = speed*CapSinDeg(elev);
                        t = CAP_RANDF();
                        color.r = 192.0f+(63.0f*t);
                        color.g = 192.0f+(63.0f*t);
                        color.b = 64.0f+(63.0f*t);
                        color.a = 192.0f+(63.0f*CAP_RANDF());
                        if(coinP->glowEff) {
                            mbev_CapEffGlowAdd(coinP->glowEff, pos, vel, 6, 100.0f*(0.3f+(0.1f*CAP_RANDF())), 0.0f, 3.266667f, color);
                        }
                    }
                    break;

                case 4:
                    pos = coinP->pos;
                    pos.y -= 50.0f;
                    mbCoinEffCreate(&pos);
                    break;
            }
        }
    }
}

void mbev_CapEffCoinKill(OMOBJ *obj)
{
    int i;
    for(i=0; i<8; i++) {
        if(ev_CapEffCoinOMObj[i] == obj) {
            break;
        }
    }
    ev_CapEffCoinOMObj[i] = (OMOBJ *)-1;
}

int mbev_CapEffCoinNumGet(OMOBJ *obj)
{
    CAPEFFCOINDATA *coinP;
    int i;
    int j;
    int num = 0;
    for(i=0; i<8; i++) {
        if(ev_CapEffCoinOMObj[i] == obj) {
            break;
        }
    }
    coinP = obj->data;
    for(j=0; j<128; j++, coinP++) {
        if(coinP->activeF) {
            num++;
        }
    }
    return num;
}

int mbev_CapEffCoinAdd(OMOBJ *obj, HuVecF *pos, HuVecF *vel, float scale, float gravity, int time, int type)
{
    int i;
    CAPEFFCOINDATA *coinP;
    int j;
    for(i=0; i<8; i++) {
        if(ev_CapEffCoinOMObj[i] == obj) {
            break;
        }
    }
    coinP = obj->data;
    for(j=0; j<128; j++, coinP++) {
        if(!coinP->activeF) {
            break;
        }
    }
    if(j >= 128) {
        return -1;
    }
    coinP->activeF = TRUE;
    coinP->type = type;
    coinP->time = time;
    coinP->count = 0;
    coinP->gravity = gravity;
    coinP->maxY = -1000000.0f;
    coinP->alpha = 1.0f;
    coinP->alphaSpeed = 1.0f/time;
    coinP->pos = *pos;
    coinP->rot.y = 360.0f*CAP_RANDF();
    coinP->rot.x = 30.0f*(-0.5f+CAP_RANDF());
    coinP->rot.z = 0.0f;
    coinP->scale.x = coinP->scale.y = coinP->scale.z = scale;
    coinP->vel = *vel;
    coinP->rotSpeed = 10.0f*(-0.5f+CAP_RANDF());
    coinP->coinId = mbCoinCreate2();
    mbCoinObjPosSetV(coinP->coinId, &coinP->pos);
    mbCoinObjRotSetV(coinP->coinId, &coinP->rot);
    mbCoinObjScaleSetV(coinP->coinId, &coinP->scale);
    return j;
}

BOOL mbev_CapEffCoinMaxYSet(OMOBJ *obj, int no, float maxY)
{
    CAPEFFCOINDATA *coinP;
    int i;
    for(i=0; i<8; i++) {
        if(ev_CapEffCoinOMObj[i] == obj) {
            break;
        }
    }
    coinP = obj->data;
    coinP += no;
    coinP->maxY = maxY;
    return TRUE;
}

void mbev_CapEffCoinMultiAdd(OMOBJ *obj, HuVecF *pos, int num)
{
    int i;
    float angle;
    float elev;
    float speed;
    HuVecF coinPos;
    HuVecF vel;
    angle = 360.0f*EvEffRandF();
    for(i=0; i<num; i++) {
        angle += 360.0f*(1.0f/num);
        coinPos = *pos;
        coinPos.y += 100.0f;
        elev = 65.0f+(15.0f*EvEffRandF());
        speed = 20.0f*(0.8f+(0.4f*EvEffRandF()));
        vel.x = speed*(CapSinDeg(angle)*CapCosDeg(elev));
        vel.z = speed*(CapCosDeg(angle)*CapCosDeg(elev));
        vel.y = speed*CapSinDeg(elev);
        mbev_CapEffCoinAdd(obj, &coinPos, &vel, 0.75f, 0.81666666f, 30, 0);
    }
}
/* @@PART5@@ */
/* Part 5: mbev_CapEffCoinGlowSet .. ev_CapEffGridSet. Types only part 5 uses. */
typedef struct CapEvCoinManWork_s {
    int no;
    BOOL activeF;
    BOOL is2DF;
    int objId;
    int playerNo;
    int num;
    int time;
    int maxTime;
    float ofsY;
    HuVecF pos;
    HuVecF end;
} CAPEV_COINMANWORK; /* size 0x3C, used by the coin and the star managers */

typedef struct CapEvCapLoseWork_s {
    int no;
    BOOL activeF;
    int objId;
    int capsuleNo;
    int unk10;
    int time;
    int maxTime;
    HuVecF pos;
    HuVecF vel;
} CAPEV_CAPLOSEWORK; /* size 0x34 */


void mbev_CapEffCoinGlowSet(OMOBJ *obj, OMOBJ *glowEff)
{
    CAPEFFCOINDATA *data;
    int i;
    int j;
    for(i=0; i<8; i++) {
        if(ev_CapEffCoinOMObj[i] == obj) {
            break;
        }
    }
    data = obj->data;
    for(j=0; j<128; j++, data++) {
        data->glowEff = glowEff;
    }
}

void mbev_CapCoinAdd(void *eff, int playerNo, int coinNum, BOOL highF)
{
    ev_CapCoinAdd(eff, playerNo, coinNum, highF, NULL);
}

static void ev_CapCoinAdd(OMOBJ *eff, int playerNo, int coinNum, int highF, void (*hook)(void))
{
    int i;
    int no;
    void (*func)(void);
    int delay;
    HuVecF playerPos;
    HuVecF pos;
    HuVecF vel;
    if(coinNum <= 0) {
        return;
    }
    if(coinNum <= 5) {
        delay = 5;
    } else if(coinNum <= 10) {
        delay = 3;
    } else if(coinNum <= 20) {
        delay = 2;
    } else {
        delay = 1;
    }
    for(i=0; i<coinNum; i++) {
        mbPlayerPosGet(playerNo, &playerPos);
        pos = playerPos;
        if(highF) {
            pos.y += 600.0f;
        } else {
            pos.y += 300.0f;
        }
        pos.x += 0.5f*(100.0f*(-0.5f+CAP_RANDF()));
        vel.x = vel.y = vel.z = 0.0f;
        no = mbev_CapEffCoinAdd(eff, &pos, &vel, 0.75f, 4.9f, 30, 4);
        if(no >= 0) {
            mbev_CapEffCoinMaxYSet(eff, no, 150.0f+playerPos.y);
        }
        HuPrcSleep(delay);
    }
    while(mbev_CapEffCoinNumGet(eff) > 0) {
        HuPrcVSleep();
    }
    if(hook != NULL) {
        func = hook;
        func();
    }
    mbCoinAddDispExec(playerNo, coinNum, FALSE, TRUE);
    mbev_CapCoinDisp(playerNo, coinNum, TRUE, TRUE);
}

static inline int ev_CapCoinManNoGet(OMOBJ *obj)
{
    int i;
    for(i=0; i<8; i++) {
        if(ev_CapEffCoinManOMObj[i] == obj) {
            break;
        }
    }
    return i;
}

OMOBJ *mbev_CapCoinManCreate(void)
{
    OMOBJ *obj;
    int i;
    int no;
    CAPEV_COINMANWORK *work;
    CAPEV_COINMANWORK *workTop;
    for(no=0; no<8; no++) {
        if(ev_CapEffCoinManOMObj[no] == NULL) {
            break;
        }
    }
    obj = ev_CapEffCoinManOMObj[no] = omAddObjEx(mbObjMan, 32768, 0, 0, -1, mbev_CapCoinManOMExec);
    work = obj->data = CapMalloc(64*sizeof(CAPEV_COINMANWORK));
    workTop = work;
    memset(work, 0, 64*sizeof(CAPEV_COINMANWORK));
    work->no = no;
    for(i=0; i<64; i++, work++) {
        work->objId = -1;
        work->activeF = FALSE;
        work->is2DF = TRUE;
        work->playerNo = -1;
        work->num = 0;
        work->time = 0;
        work->maxTime = 0;
        work->ofsY = 0.0f;
        work->pos.x = work->pos.y = work->pos.z = 0.0f;
        work->end.x = work->end.y = work->end.z = 0.0f;
    }
    return obj;
}

void mbev_CapCoinManOMExec(OMOBJ *obj)
{
    CAPEV_COINMANWORK *work = obj->data;
    int i;
    float t;
    float s;
    HuVecF pos;
    HuVecF pos3D;
    if(mbExitCheck() || ev_CapEffCoinManOMObj[work->no] == (OMOBJ *)-1) {
        for(i=0; i<64; i++, work++) {
            if(work->objId >= 0) {
                mbCoinObjNumDec(work->objId);
            }
            work->objId = -1;
        }
        work = obj->data;
        ev_CapEffCoinManOMObj[work->no] = NULL;
        omDelObjEx(mbObjMan, obj);
        return;
    }
    for(i=0; i<64; i++, work++) {
        if(!work->activeF) {
            continue;
        }
        t = (float)work->time/work->maxTime;
        mbev_CapVecChase(t, &work->pos, &work->end, &pos);
        if(work->ofsY) {
            s = mbSinDeg(180.0f*t);
            pos.y += work->ofsY*s;
        }
        if(work->is2DF) {
            Hu3D2Dto3D(&pos, HU3D_CAM0, &pos3D);
            mbCoinObjPosSetV(work->objId, &pos3D);
        } else {
            mbCoinObjPosSetV(work->objId, &pos);
        }
        if(++work->time >= work->maxTime) {
            mbCoinObjNumDec(work->objId);
            work->objId = -1;
            work->activeF = FALSE;
            if(work->playerNo != -1 && work->num > 0) {
                mbPlayerCoinAdd(work->playerNo, work->num);
                if(!work->is2DF) {
                    mbPlayerPosGet(work->playerNo, &pos);
                    pos.y += 50.0f;
                    mbCoinEffCreate(&pos);
                }
                mbAudFXPlay(7);
            }
        }
    }
}

void mbev_CapCoinManKill(OMOBJ *obj)
{
    int i;
    for(i=0; i<8; i++) {
        if(ev_CapEffCoinManOMObj[i] == obj) {
            break;
        }
    }
    ev_CapEffCoinManOMObj[i] = (OMOBJ *)-1;
}

int mbev_CapCoinManNumGet(OMOBJ *obj)
{
    CAPEV_COINMANWORK *work;
    int i;
    int num;
    ev_CapCoinManNoGet(obj);
    work = obj->data;
    for(i=0, num=0; i<64; i++, work++) {
        if(work->activeF) {
            num++;
        }
    }
    return num;
}

int mbev_CapCoinManObjAdd(OMOBJ *obj, HuVecF *pos, HuVecF *end, float scale, float ofsY, int time, int playerNo, int coinNum, BOOL is2DF)
{
    CAPEV_COINMANWORK *work;
    int i;
    float rotY;
    ev_CapCoinManNoGet(obj);
    work = obj->data;
    for(i=0; i<64; i++, work++) {
        if(!work->activeF) {
            break;
        }
    }
    if(i >= 64) {
        return -1;
    }
    work->activeF = TRUE;
    work->is2DF = is2DF;
    work->playerNo = playerNo;
    work->num = coinNum;
    work->time = 0;
    work->maxTime = time;
    work->ofsY = ofsY;
    work->pos = *pos;
    work->end = *end;
    work->objId = mbCoinCreate2();
    mbCoinObjPosSetV(work->objId, &work->pos);
    rotY = 360.0f*CAP_RANDF();
    mbCoinObjRotSet(work->objId, 50.0f*(-0.5f+CAP_RANDF()), rotY, 0.0f);
    mbCoinObjScaleSet(work->objId, scale, scale, scale);
    return i;
}

int mbev_CapCoinManAdd2(OMOBJ *obj, BOOL rightF, int playerNo, int coinNum)
{
    HuVecF pos;
    HuVecF end;
    int time;
    float ofsY;
    if(!rightF) {
        pos.x = 80.0f;
        pos.y = 240.0f;
        pos.z = 1000.0f;
        end.x = 496.0f;
        end.y = 240.0f;
        end.z = 1000.0f;
        end.x += 25.0f*(0.5f-CAP_RANDF());
    } else {
        pos.x = 396.0f;
        pos.y = 240.0f;
        pos.z = 1000.0f;
        end.x = 180.0f;
        end.y = 240.0f;
        end.z = 1000.0f;
        end.x += 25.0f*(0.5f-CAP_RANDF());
    }
    time = 30.0f*(1.0f+(0.3f*CAP_RANDF()));
    ofsY = -75.0f+(1.0f+(0.35f*CAP_RANDF()));
    if(mbev_CapCoinManObjAdd(obj, &pos, &end, 0.3f, ofsY, time, playerNo, coinNum, TRUE) != -1) {
        return coinNum;
    } else {
        return 0;
    }
}

int mbev_CapCoinManAdd(OMOBJ *obj, HuVecF *pos, HuVecF *end, int playerNo, int coinNum)
{
    int time;
    float ofsY;
    time = 60.0f*(0.3f+(0.05f*CAP_RANDF()));
    ofsY = 100.0f*(2.0f+(0.5f*CAP_RANDF()));
    if(mbev_CapCoinManObjAdd(obj, pos, end, 1.0f, ofsY, time, playerNo, coinNum, FALSE) != -1) {
        return coinNum;
    } else {
        return 0;
    }
}

static inline int ev_CapStarManNoGet(OMOBJ *obj)
{
    int i;
    for(i=0; i<8; i++) {
        if(ev_CapEffStarManOMObj[i] == obj) {
            break;
        }
    }
    return i;
}

OMOBJ *mbev_CapStarManCreate(void)
{
    OMOBJ *obj;
    int i;
    int no;
    CAPEV_COINMANWORK *work;
    CAPEV_COINMANWORK *workTop;
    for(no=0; no<8; no++) {
        if(ev_CapEffStarManOMObj[no] == NULL) {
            break;
        }
    }
    obj = ev_CapEffStarManOMObj[no] = omAddObjEx(mbObjMan, 32768, 0, 0, -1, mbev_CapStarManOMExec);
    work = obj->data = CapMalloc(8*sizeof(CAPEV_COINMANWORK));
    workTop = work;
    memset(work, 0, 8*sizeof(CAPEV_COINMANWORK));
    work->no = no;
    for(i=0; i<8; i++, work++) {
        work->objId = mbStarObjCreate();
        mbStarObjDispSet(work->objId, FALSE);
        work->activeF = FALSE;
        work->is2DF = FALSE;
        work->playerNo = -1;
        work->num = 0;
        work->time = 0;
        work->maxTime = 0;
        work->ofsY = 0.0f;
        work->pos.x = work->pos.y = work->pos.z = 0.0f;
        work->end.x = work->end.y = work->end.z = 0.0f;
    }
    return obj;
}

void mbev_CapStarManOMExec(OMOBJ *obj)
{
    CAPEV_COINMANWORK *work = obj->data;
    int i;
    float t;
    float s;
    HuVecF pos;
    HuVecF pos3D;
    if(mbExitCheck() || ev_CapEffStarManOMObj[work->no] == (OMOBJ *)-1) {
        for(i=0; i<8; i++, work++) {
            if(work->objId != -1) {
                mbStarObjKill(work->objId);
            }
        }
        work = obj->data;
        ev_CapEffStarManOMObj[work->no] = NULL;
        omDelObjEx(mbObjMan, obj);
        return;
    }
    for(i=0; i<8; i++, work++) {
        if(!work->activeF || work->objId == -1) {
            continue;
        }
        t = (float)work->time/work->maxTime;
        VECSubtract(&work->end, &work->pos, &pos);
        VECScale(&pos, &pos, t);
        VECAdd(&work->pos, &pos, &pos);
        if(work->ofsY) {
            s = mbSinDeg(180.0f*t);
            pos.y += work->ofsY*s;
        }
        if(work->is2DF) {
            Hu3D2Dto3D(&pos, HU3D_CAM0, &pos3D);
            mbStarObjPosSetV(work->objId, &pos3D);
        } else {
            mbStarObjPosSetV(work->objId, &pos);
        }
        if(++work->time >= work->maxTime) {
            mbStarObjDispSet(work->objId, FALSE);
            work->activeF = FALSE;
            if(work->playerNo != -1 && work->num > 0) {
                if(!work->is2DF) {
                    mbPlayerPosGet(work->playerNo, &pos);
                    pos.y += 50.0f;
                    mbCoinEffCreate(&pos);
                }
                mbPlayerStarAdd(work->playerNo, work->num);
            }
        }
    }
}

void mbev_CapStarManKill(OMOBJ *obj)
{
    int i;
    for(i=0; i<8; i++) {
        if(ev_CapEffStarManOMObj[i] == obj) {
            break;
        }
    }
    ev_CapEffStarManOMObj[i] = (OMOBJ *)-1;
}

int mbev_CapStarManNumGet(OMOBJ *obj)
{
    CAPEV_COINMANWORK *work;
    int i;
    int num;
    ev_CapStarManNoGet(obj);
    work = obj->data;
    for(i=0, num=0; i<8; i++, work++) {
        if(work->activeF) {
            num++;
        }
    }
    return num;
}

int mbev_CapStarManObjAdd(OMOBJ *obj, HuVecF *pos, HuVecF *end, float scale, float ofsY, int time, int playerNo, int starNum, BOOL is2DF)
{
    CAPEV_COINMANWORK *work;
    int i;
    ev_CapStarManNoGet(obj);
    work = obj->data;
    for(i=0; i<8; i++, work++) {
        if(!work->activeF) {
            break;
        }
    }
    if(i >= 8) {
        return -1;
    }
    work->activeF = TRUE;
    work->is2DF = is2DF;
    work->playerNo = playerNo;
    work->num = starNum;
    work->time = 0;
    work->maxTime = time;
    work->ofsY = ofsY;
    work->pos = *pos;
    work->end = *end;
    mbStarObjDispSet(work->objId, TRUE);
    mbStarObjPosSetV(work->objId, &work->pos);
    mbStarObjScaleSet(work->objId, scale, scale, scale);
    return i;
}

int mbev_CapStarManAdd2(OMOBJ *obj, BOOL rightF, int playerNo, int starNum)
{
    HuVecF pos;
    HuVecF end;
    int time;
    float ofsY;
    if(!rightF) {
        pos.x = 80.0f;
        pos.y = 240.0f;
        pos.z = 1000.0f;
        end.x = 496.0f;
        end.y = 240.0f;
        end.z = 1000.0f;
        end.x += 25.0f*(0.5f-CAP_RANDF());
    } else {
        pos.x = 396.0f;
        pos.y = 240.0f;
        pos.z = 1000.0f;
        end.x = 180.0f;
        end.y = 240.0f;
        end.z = 1000.0f;
        end.x += 25.0f*(0.5f-CAP_RANDF());
    }
    time = 30.0f*(1.0f+(0.3f*CAP_RANDF()));
    ofsY = -75.0f+(1.0f+(0.35f*CAP_RANDF()));
    if(mbev_CapStarManObjAdd(obj, &pos, &end, 0.15f, ofsY, time, playerNo, starNum, TRUE) != -1) {
        return starNum;
    } else {
        return 0;
    }
}

int mbev_CapStarManAdd(OMOBJ *obj, HuVecF *pos, HuVecF *end, int playerNo, int starNum)
{
    int time;
    float ofsY;
    time = 60.0f*(0.3f+(0.05f*CAP_RANDF()));
    ofsY = 100.0f*(2.0f+(0.5f*CAP_RANDF()));
    if(mbev_CapStarManObjAdd(obj, pos, end, 1.0f, ofsY, time, playerNo, starNum, FALSE) != -1) {
        return starNum;
    } else {
        return 0;
    }
}

static inline void ev_CapObjAttrSet(MBOBJID objId, u32 attr)
{
    mbObjAttrSet(objId, attr);
}

static inline int ev_CapEffCapLoseNoGet(OMOBJ *obj)
{
    int i;
    for(i=0; i<8; i++) {
        if(ev_CapEffCapLoseOMObj[i] == obj) {
            break;
        }
    }
    return i;
}

OMOBJ *mbev_CapEffCapLoseCreate(void)
{
    OMOBJ *obj;
    int i;
    int no;
    CAPEV_CAPLOSEWORK *work;
    for(no=0; no<8; no++) {
        if(ev_CapEffCapLoseOMObj[no] == NULL) {
            break;
        }
    }
    obj = ev_CapEffCapLoseOMObj[no] = omAddObjEx(mbObjMan, 32768, 0, 0, -1, mbev_CapEffCapLoseOMExec);
    work = obj->data = CapMalloc(6*sizeof(CAPEV_CAPLOSEWORK));
    memset(work, 0, 6*sizeof(CAPEV_CAPLOSEWORK));
    work->no = no;
    for(i=0; i<6; i++, work++) {
        work->activeF = FALSE;
        work->objId = -1;
        work->capsuleNo = -1;
        work->unk10 = -1;
        work->time = 0;
        work->maxTime = 0;
        work->pos.x = work->pos.y = work->pos.z = 0.0f;
        work->vel.x = work->vel.x = work->vel.x = 0.0f;
    }
    return obj;
}

void mbev_CapEffCapLoseOMExec(OMOBJ *obj)
{
    CAPEV_CAPLOSEWORK *work = obj->data;
    int i;
    if(mbExitCheck() || ev_CapEffCapLoseOMObj[work->no] == (OMOBJ *)-1) {
        for(i=0; i<6; i++, work++) {
            if(work->objId != -1) {
                mbCapObjColorKill(work->objId);
            }
            work->objId = -1;
        }
        work = obj->data;
        ev_CapEffCapLoseOMObj[work->no] = NULL;
        omDelObjEx(mbObjMan, obj);
        return;
    }
    for(i=0; i<6; i++, work++) {
        if(!work->activeF) {
            continue;
        }
        VECAdd(&work->pos, &work->vel, &work->pos);
        work->vel.y -= 4.9000006f;
        work->vel.y *= 0.98f;
        work->time++;
        mbCapObjColorPosSetV(work->objId, &work->pos);
        mbCapObjColorAlphaSet(work->objId, 255.0f-(255.0f*((float)work->time/work->maxTime)));
        if(work->time >= work->maxTime) {
            if(work->objId != -1) {
                mbCapObjColorKill(work->objId);
            }
            work->objId = -1;
            work->activeF = FALSE;
        }
    }
}

void mbev_CapEffCapLoseKill(OMOBJ *obj)
{
    int i;
    for(i=0; i<8; i++) {
        if(ev_CapEffCapLoseOMObj[i] == obj) {
            break;
        }
    }
    ev_CapEffCapLoseOMObj[i] = (OMOBJ *)-1;
}

int mbev_CapEffCapLoseNumGet(OMOBJ *obj)
{
    CAPEV_CAPLOSEWORK *work;
    int i;
    int num;
    ev_CapEffCapLoseNoGet(obj);
    work = obj->data;
    for(i=0, num=0; i<6; i++, work++) {
        if(work->activeF) {
            num++;
        }
    }
    return num;
}

int mbev_CapEffCapLoseObjAdd(OMOBJ *obj, HuVecF *pos, HuVecF *vel, float scale, int time, int capsuleNo)
{
    CAPEV_CAPLOSEWORK *work;
    int i;
    int no;
    for(no=0; no<8; no++) {
        if(ev_CapEffCapLoseOMObj[no] == obj) {
            break;
        }
    }
    work = obj->data;
    for(i=0; i<6; i++, work++) {
        if(!work->activeF) {
            break;
        }
    }
    if(i >= 6) {
        return -1;
    }
    work->activeF = TRUE;
    work->objId = mbCapObjColorCreate(capsuleNo, TRUE);
    ev_CapObjAttrSet(work->objId, 0x40000001);
    work->capsuleNo = capsuleNo;
    work->unk10 = -1;
    work->time = 0;
    work->maxTime = time;
    work->pos = *pos;
    work->vel = *vel;
    mbCapObjColorPosSet(work->objId, pos->x, pos->y, pos->z);
    mbCapObjColorScaleSet(work->objId, scale, scale, scale);
    mbCapObjColorLayerSet(work->objId, 3);
    return i;
}

void mbev_CapEffCapLoseAdd(OMOBJ *obj, int playerNo, int num, float radius)
{
    int i;
    int capsuleNo;
    float angle;
    float speed;
    float elev;
    float sinAngle;
    float cosAngle;
    float cosElev;
    float sinAngle2;
    float cosElev2;
    float cosAngle2;
    float sinElev;
    HuVecF playerPos;
    HuVecF pos;
    HuVecF vel;
    ev_CapEffCapLoseNoGet(obj);
    mbPlayerPosGet(playerNo, &playerPos);
    if(num > mbPlayerCapsuleMaxGet()) {
        num = mbPlayerCapsuleMaxGet();
    }
    angle = 360.0f*CAP_RANDF();
    for(i=0; i<num; i++) {
        capsuleNo = mbPlayerCapsuleGet(playerNo, i);
        if(capsuleNo == -1) {
            continue;
        }
        speed = 65.0f*(1.0f+(0.1f*CAP_RANDF()));
        elev = 75.0f+(15.0f*CAP_RANDF());
        angle += (360.0f/num)+(10.0f*CAP_RANDF());
        sinAngle = mbSinDeg(angle);
        pos.x = playerPos.x+(radius*sinAngle);
        cosAngle = mbCosDeg(angle);
        pos.z = playerPos.z+(radius*cosAngle);
        pos.y = playerPos.y;
        cosElev = mbCosDeg(elev);
        sinAngle2 = mbSinDeg(angle);
        vel.x = speed*(sinAngle2*cosElev);
        cosElev2 = mbCosDeg(elev);
        cosAngle2 = mbCosDeg(angle);
        vel.z = speed*(cosAngle2*cosElev2);
        sinElev = mbSinDeg(elev);
        vel.y = speed*sinElev;
        mbev_CapEffCapLoseObjAdd(obj, &pos, &vel, 0.5f, 30, capsuleNo);
    }
}

static inline void *ev_CapEffMalloc(u32 size, u32 num)
{
    return HuMemDirectMallocNum(HEAP_MODEL, size, num);
}

static HU3D_MODELID ev_CapEffCreate(ANIMDATA *anim, s16 num)
{
    CAPEV_EFFECT *effP;
    CAPEV_EFFDATA *effDataP;
    s16 i;
    HuVec2f *st;
    HU3D_MODEL *modelP;
    HuVecF *vtx;
    HU3D_MODELID modelId;
    void *dlBuf;
    void *dlBegin;
    modelId = Hu3DHookFuncCreate(ev_CapEffDraw);
    Hu3DModelCameraSet(modelId, HU3D_CAM0);
    modelP = &Hu3DData[modelId];
    modelP->hookData = effP = ev_CapEffMalloc(sizeof(CAPEV_EFFECT), modelP->mallocNo);
    effP->anim = anim;
    HuSprAnimLock(anim);
    effP->num = num;
    effP->blendMode = HU3D_PARTICLE_BLEND_NORMAL;
    effP->dispAttr = 0;
    effP->hook = NULL;
    effP->hookMdlP = NULL;
    effP->count = 0;
    effP->attr = 0;
    effP->unk23 = 0;
    effP->prevCount = 0;
    effP->mode = effP->time = 0;
    effP->gridSt = NULL;
    effP->unk54 = 0;
    effP->gridNum = 16;
    effP->data = effDataP = ev_CapEffMalloc(num*sizeof(CAPEV_EFFDATA), modelP->mallocNo);
    memset(effDataP, 0, num*sizeof(CAPEV_EFFDATA));
    for(i=0; i<num; i++, effDataP++) {
        effDataP->scale = 0.0f;
        effDataP->scaleX = effDataP->scaleY = 1.0f;
        effDataP->rot.x = effDataP->rot.y = effDataP->rot.z = 0.0f;
        effDataP->animTime = 0.0f;
        effDataP->animSpeed = 1.0f;
        effDataP->pos.x = 0.0f;
        effDataP->pos.y = 0.0f;
        effDataP->pos.z = 0.0f;
        effDataP->color.r = effDataP->color.g = effDataP->color.b = effDataP->color.a = 255;
        effDataP->no = 0;
    }
    effP->vertex = vtx = ev_CapEffMalloc(num*sizeof(HuVecF)*4, modelP->mallocNo);
    for(i=0; i<num*4; i++, vtx++) {
        vtx->x = vtx->y = vtx->z = 0.0f;
    }
    effP->st = st = ev_CapEffMalloc(num*sizeof(HuVec2f)*4, modelP->mallocNo);
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
    dlBegin = dlBuf = ev_CapEffMalloc(0x10000, modelP->mallocNo);
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
    effP->dl = ev_CapEffMalloc(effP->dlSize, modelP->mallocNo);
    memcpy(effP->dl, dlBuf, effP->dlSize);
    DCFlushRange(effP->dl, effP->dlSize);
    HuMemDirectFree(dlBuf);
    return modelId;
}

static void ev_CapEffDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    CAPEV_EFFECT *effP;
    CAPEV_EFFDATA *effDataP;
    HuVecF *vtx;
    HuVec2f *st;
    s16 i;
    s16 j;
    s16 bmpFmt;
    s16 row;
    s16 col;
    CAPEV_EFFHOOK hook;

    Mtx mtxInv;
    Mtx mtxPos;
    Mtx mtxRotZ;
    HuVecF scaleVtx[4];
    HuVecF baseVtx[4];
    HuVecF initVtx[4];
    ROMtx basePosMtx;

    effP = modelP->hookData;
    if(effP->prevCounter != GlobalCounter || shadowModelDrawF) {
        if(effP->hookMdlP && effP->hookMdlP != modelP) {
            ev_CapEffDraw(effP->hookMdlP, mtx);
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
            if(effP->dispAttr & CAPEV_EFF_DISPATTR_ZBUF_OFF) {
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
        switch(effP->blendMode) {
            case HU3D_PARTICLE_BLEND_NORMAL:
                GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
                break;
            case HU3D_PARTICLE_BLEND_ADDCOL:
                GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
                break;
            case HU3D_PARTICLE_BLEND_INVCOL:
                GXSetBlendMode(GX_BM_BLEND, GX_BL_ZERO, GX_BL_INVSRCALPHA, GX_LO_NOOP);
                break;
        }
        if(effP->dispAttr & CAPEV_EFF_DISPATTR_CULL_OFF) {
            GXSetCullMode(GX_CULL_NONE);
        } else {
            GXSetCullMode(GX_CULL_BACK);
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
        if(effP->dispAttr & CAPEV_EFF_DISPATTR_CAMERA_ROT) {
            PSMTXIdentity(mtxInv);
            PSMTXIdentity(*(Mtx *)(&basePosMtx));
            initVtx[0] = baseVtx[0] = basePos[0];
            initVtx[1] = baseVtx[1] = basePos[1];
            initVtx[2] = baseVtx[2] = basePos[2];
            initVtx[3] = baseVtx[3] = basePos[3];
        } else {
            initVtx[0] = baseVtx[0] = basePos[0];
            initVtx[1] = baseVtx[1] = basePos[1];
            initVtx[2] = baseVtx[2] = basePos[2];
            initVtx[3] = baseVtx[3] = basePos[3];
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
                continue;
            }
            if(effP->dispAttr & CAPEV_EFF_DISPATTR_SCALEXY) {
                initVtx[0].x = baseVtx[0].x*effDataP->scaleX;
                initVtx[0].y = baseVtx[0].y*effDataP->scaleY;
                initVtx[1].x = baseVtx[1].x*effDataP->scaleX;
                initVtx[1].y = baseVtx[1].y*effDataP->scaleY;
                initVtx[2].x = baseVtx[2].x*effDataP->scaleX;
                initVtx[2].y = baseVtx[2].y*effDataP->scaleY;
                initVtx[3].x = baseVtx[3].x*effDataP->scaleX;
                initVtx[3].y = baseVtx[3].y*effDataP->scaleY;
            }
            VECScale(&initVtx[0], &scaleVtx[0], effDataP->scale);
            VECScale(&initVtx[1], &scaleVtx[1], effDataP->scale);
            VECScale(&initVtx[2], &scaleVtx[2], effDataP->scale);
            VECScale(&initVtx[3], &scaleVtx[3], effDataP->scale);
            if(effP->dispAttr & CAPEV_EFF_DISPATTR_ROT3D) {
                mtxRot(mtxRotZ, effDataP->rot.x, effDataP->rot.y, effDataP->rot.z);
                PSMTXConcat(mtxInv, mtxRotZ, mtxPos);
            } else if(!effDataP->rot.z) {
                PSMTXCopy(mtxInv, mtxPos);
            } else {
                PSMTXRotRad(mtxRotZ, 'Z', effDataP->rot.z);
                PSMTXConcat(mtxInv, mtxRotZ, mtxPos);
            }
            mtxPos[0][3] = effDataP->pos.x;
            mtxPos[1][3] = effDataP->pos.y;
            mtxPos[2][3] = effDataP->pos.z;
            PSMTXMultVecArray(mtxPos, scaleVtx, vtx, 4);
            vtx += 4;
        }
        effDataP = effP->data;
        st = effP->st;
        if(!(effP->dispAttr & CAPEV_EFF_DISPATTR_NOANIM)) {
            if(effP->gridSt == NULL) {
                for(i=0; i<effP->num; i++, effDataP++) {
                    row = effDataP->no & 0x3;
                    col = (effDataP->no >> 2) & 0x3;
                    for(j=0; j<4; j++, st++) {
                        st->x = (0.25f*row)+baseST1[j].x;
                        st->y = (0.25f*col)+baseST1[j].y;
                    }
                }
            } else {
                for(i=0; i<effP->num; i++, effDataP++) {
                    for(j=0; j<4; j++, st++) {
                        st->x = effP->gridSt[j+(effDataP->no*4)].x;
                        st->y = effP->gridSt[j+(effDataP->no*4)].y;
                    }
                }
            }
        } else if(!(effP->dispAttr & CAPEV_EFF_DISPATTR_ST_DONE)) {
            for(i=0; i<effP->num; i++, effDataP++) {
                for(j=0; j<4; j++, st++) {
                    st->x = baseST2[j].x;
                    st->y = baseST2[j].y;
                }
            }
            effP->dispAttr |= CAPEV_EFF_DISPATTR_ST_DONE;
        }
        DCFlushRangeNoSync(effP->vertex, effP->num*sizeof(HuVecF)*4);
        DCFlushRangeNoSync(effP->st, effP->num*sizeof(HuVec2f)*4);
        DCFlushRangeNoSync(effP->data, effP->num*sizeof(CAPEV_EFFDATA));
        PPCSync();
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetArray(GX_VA_POS, effP->vertex, sizeof(HuVecF));
        GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        GXSetArray(GX_VA_CLR0, &effP->data->color, sizeof(CAPEV_EFFDATA));
        GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetArray(GX_VA_TEX0, effP->st, sizeof(HuVec2f));
        GXCallDisplayList(effP->dl, effP->dlSize);
        if(shadowModelDrawF == FALSE) {
            if(!(effP->attr & CAPEV_EFF_ATTR_COUNTER_UPDATE)) {
                effP->count++;
            }
            if(effP->prevCount != 0 && effP->prevCount <= effP->count) {
                if(effP->attr & CAPEV_EFF_ATTR_COUNTER_RESET) {
                    effP->count = 0;
                }
                effP->count = effP->prevCount;
            }
            effP->prevCounter = GlobalCounter;
        }
    }
}

static void ev_CapEffGridSet(HU3D_MODELID modelId, int w, int h, BOOL flipF)
{
    int i;
    int j;
    int num;
    float invW;
    float invH;
    HU3D_MODEL *modelP;
    CAPEV_EFFECT *effP;
    HuVec2f *st;
    if(w < 1) {
        w = 1;
    }
    if(h < 1) {
        h = 1;
    }
    num = w*h;
    invW = 1.0f/w;
    invH = 1.0f/h;
    modelP = &Hu3DData[modelId];
    effP = modelP->hookData;
    effP->gridNum = num;
    effP->gridSt = st = ev_CapEffMalloc(num*sizeof(HuVec2f)*4, modelP->mallocNo);
    memset(st, 0, num*sizeof(HuVec2f)*4);
    for(i=0; i<h; i++) {
        for(j=0; j<w; j++) {
            if(flipF) {
                st->x = i*invW;
                st->y = j*invH;
                st++;
                st->x = (i+1)*invW;
                st->y = j*invH;
                st++;
                st->x = (i+1)*invW;
                st->y = (j+1)*invH;
                st++;
                st->x = i*invW;
                st->y = (j+1)*invH;
                st++;
            } else {
                st->x = j*invW;
                st->y = i*invH;
                st++;
                st->x = (j+1)*invW;
                st->y = i*invH;
                st++;
                st->x = (j+1)*invW;
                st->y = (i+1)*invH;
                st++;
                st->x = j*invW;
                st->y = (i+1)*invH;
                st++;
            }
        }
    }
}

/* @@PART6@@ */


static float ev_CapRotCamera(float angle);

#ifndef GWTeamGet
#define GWTeamGet() ((int)GwSystem.tagF)
#endif

static inline int GWPlayerTeamGet(int playerNo)
{
    return GwPlayer[playerNo].team;
}


void mbev_CapEffColorSet(GXColor *color, int no)
{
    if(no < 0) {
        no *= -1;
    }
    *color = ev_CapsuleRandomColorTbl[no % 7];
}

BOOL mbev_CapPlayerMotShiftCheck(int playerNo)
{
    int objId = mbPlayerObjIDGet(playerNo);
    int modelId = mbObjModelIDGet(objId);
    if(Hu3DMotionShiftIDGet(modelId) == HU3D_MOTIONID_NONE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void mbev_CapPlayerMotShiftSet(int objId, int motNo, u32 attr, BOOL shiftF)
{
    if(shiftF) {
        mbObjMotionShiftSet(objId, motNo, 0, 8, attr);
        if(attr & HU3D_MOTATTR_LOOP) {
            do {
                HuPrcVSleep();
            } while(mbObjMotionShiftIDGet(objId) != HU3D_MOTIONID_NONE);
        } else {
            do {
                HuPrcVSleep();
            } while(mbObjMotionShiftIDGet(objId) != HU3D_MOTIONID_NONE);
            do {
                HuPrcVSleep();
            } while(!mbObjMotionEndCheck(objId));
        }
    } else {
        mbObjMotionSet(objId, motNo, attr);
        if(!(attr & HU3D_MOTATTR_LOOP)) {
            do {
                HuPrcVSleep();
            } while(mbObjMotionEndCheck(objId));
        }
    }
}

void mbev_CapPlayerMotShiftWait(int playerNo, int motNo, u32 attr, BOOL shiftF)
{
    int objId = mbPlayerObjIDGet(playerNo);
    if(shiftF) {
        mbPlayerMotionShiftSet(playerNo, motNo, 0, 8, attr);
        if(attr & HU3D_MOTATTR_LOOP) {
            do {
                HuPrcVSleep();
            } while(mbObjMotionShiftIDGet(objId) != HU3D_MOTIONID_NONE);
        } else {
            do {
                HuPrcVSleep();
            } while(mbObjMotionShiftIDGet(objId) != HU3D_MOTIONID_NONE);
            do {
                HuPrcVSleep();
            } while(!mbObjMotionEndCheck(objId));
        }
    } else {
        mbPlayerMotionSet(playerNo, motNo, attr);
        if(!(attr & HU3D_MOTATTR_LOOP)) {
            do {
                HuPrcVSleep();
            } while(mbObjMotionEndCheck(objId));
        }
    }
}

void mbev_CapObjMotionOMExec(OMOBJ *obj);

void mbev_CapObjMotionSet(int objId, int delay, int motNo, int nextMotNo, u32 attr, u32 nextAttr, BOOL shiftF, BOOL nextShiftF)
{
    OMOBJ *obj;
    CAPMOTWORK *work;
    obj = omAddObjEx(mbObjMan, 32768, 0, 0, -1, mbev_CapObjMotionOMExec);
    work = obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(CAPMOTWORK), HU_MEMNUM_OVL);
    memset(work, 0, sizeof(CAPMOTWORK));
    work->objId = objId;
    work->delay = delay;
    work->motNo = motNo;
    work->nextMotNo = nextMotNo;
    work->attr = attr;
    work->nextAttr = nextAttr;
    work->shiftF = shiftF;
    work->nextShiftF = nextShiftF;
    if(shiftF) {
        mbObjMotionShiftSet(objId, motNo, 0, 8, attr);
    } else {
        mbObjMotionSet(objId, motNo, attr);
    }
}

void mbev_CapObjMotionOMExec(OMOBJ *obj)
{
    CAPMOTWORK *work = obj->data;
    BOOL doneF = FALSE;
    if(mbExitCheck()) {
        omDelObjEx(mbObjMan, obj);
        return;
    }
    if(work->attr & HU3D_MOTATTR_LOOP) {
        if(mbObjMotionShiftIDGet(work->objId) == HU3D_MOTIONID_NONE) {
            float maxTime = mbObjMotionMaxTimeGet(work->objId);
            if(mbObjMotionTimeGet(work->objId)+1.0f >= maxTime) {
                doneF = TRUE;
            }
        }
    } else {
        if(mbObjMotionShiftIDGet(work->objId) == HU3D_MOTIONID_NONE && mbObjMotionEndCheck(work->objId)) {
            doneF = TRUE;
        }
    }
    if(doneF) {
        if(--work->delay <= 0) {
            if(work->nextShiftF) {
                mbObjMotionShiftSet(work->objId, work->nextMotNo, 0, 8, work->nextShiftF);
            } else {
                mbObjMotionSet(work->objId, work->nextMotNo, work->nextShiftF);
            }
            omDelObjEx(mbObjMan, obj);
        }
    }
}

void mbev_CapPlayerMotionOMExec(OMOBJ *obj);

void mbev_CapPlayerMotionSet(int playerNo, int delay, int motNo, int nextMotNo, u32 attr, u32 nextAttr, BOOL shiftF, BOOL nextShiftF)
{
    OMOBJ *obj;
    CAPMOTWORK *work;
    obj = omAddObjEx(mbObjMan, 32768, 0, 0, -1, mbev_CapPlayerMotionOMExec);
    work = obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(CAPMOTWORK), HU_MEMNUM_OVL);
    memset(work, 0, sizeof(CAPMOTWORK));
    work->playerNo = playerNo;
    work->delay = delay;
    work->motNo = motNo;
    work->nextMotNo = nextMotNo;
    work->attr = attr;
    work->nextAttr = nextAttr;
    work->shiftF = shiftF;
    work->nextShiftF = nextShiftF;
    if(shiftF) {
        mbPlayerMotionShiftSet(playerNo, motNo, 0, 8, attr);
    } else {
        mbPlayerMotionSet(playerNo, motNo, attr);
    }
}

void mbev_CapPlayerMotionOMExec(OMOBJ *obj)
{
    CAPMOTWORK *work = obj->data;
    BOOL doneF = FALSE;
    if(mbExitCheck()) {
        omDelObjEx(mbObjMan, obj);
        return;
    }
    if(work->attr & HU3D_MOTATTR_LOOP) {
        if(mbObjMotionShiftIDGet(mbPlayerObjIDGet(work->playerNo)) == HU3D_MOTIONID_NONE) {
            float maxTime = mbPlayerMotionMaxTimeGet(work->playerNo);
            if(mbPlayerMotionTimeGet(work->playerNo)+1.0f >= maxTime) {
                doneF = TRUE;
            }
        }
    } else {
        if(mbObjMotionShiftIDGet(mbPlayerObjIDGet(work->playerNo)) == HU3D_MOTIONID_NONE && mbPlayerMotionEndCheck(work->playerNo)) {
            doneF = TRUE;
        }
    }
    if(doneF) {
        if(--work->delay <= 0) {
            if(work->nextShiftF) {
                mbPlayerMotionShiftSet(work->playerNo, work->nextMotNo, 0, 8, work->nextShiftF);
            } else {
                mbPlayerMotionSet(work->playerNo, work->nextMotNo, work->nextShiftF);
            }
            omDelObjEx(mbObjMan, obj);
        }
    }
}

void mbev_CapPlayerIdleWait(void)
{
    int i;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        mbPlayerMotIdleSet(i);
    }
    do {
        for(i=0; i<GW_PLAYER_MAX; i++) {
            if(!mbev_CapPlayerMotShiftCheck(i)) {
                break;
            }
        }
        HuPrcVSleep();
    } while(i < GW_PLAYER_MAX);
}

void mbev_CapPlayerRotate(int playerNo, float rotY)
{
    mbPlayerRotateStart(playerNo, rotY, 15);
    while(!mbPlayerRotateCheck(playerNo)) {
        HuPrcVSleep();
    }
}

int mbev_CapPlayerSquishVoiceSet(int *playerNo, int masuId, BOOL voiceF);

int mbev_CapPlayerSquishSet(int *playerNo, int masuId)
{
    return mbev_CapPlayerSquishVoiceSet(playerNo, masuId, FALSE);
}

int mbev_CapPlayerSquishVoiceSet(int *playerNo, int masuId, BOOL voiceF)
{
    int i;
    int num;
    int playerNoTemp[GW_PLAYER_MAX];
    num = 0;
    if(playerNo == NULL) {
        playerNo = playerNoTemp;
    }
    for(i=0; i<GW_PLAYER_MAX; i++) {
        if(masuId == GwPlayer[i].masuId) {
            playerNo[num++] = i;
        }
    }
    for(i=0; i<num; i++) {
        if(voiceF) {
            CharMotionVoiceOnSet(GwPlayer[playerNo[i]].charNo, 46, FALSE);
        }
        mbPlayerMotionSet(playerNo[i], 10, HU3D_MOTATTR_NONE);
        mbPlayerMotionTimeSet(playerNo[i], 20);
        mbPlayerMotionSpeedSet(playerNo[i], 0);
    }
    return num;
}

void mbev_CapPlayerStunSet(int *playerNo, int num, int type)
{
    int i;
    for(i=0; i<num; i++) {
        mbPlayerMotionSpeedSet(playerNo[i], 1);
    }
    i = 0;
    do {
        HuPrcVSleep();
        for(i=0; i<num; i++) {
            if(!mbPlayerMotionEndCheck(playerNo[i])) {
                break;
            }
        }
    } while(i < num);
    for(i=0; i<num; i++) {
        switch(type) {
            case 0:
                mbPlayerMotionShiftSet(playerNo[i], 6, 0, 8, HU3D_MOTATTR_LOOP);
                break;
            case 1:
                mbPlayerMotionShiftSet(playerNo[i], 6, 0, 8, HU3D_MOTATTR_LOOP);
                break;
        }
        CharMotionVoiceOnSet(GwPlayer[playerNo[i]].charNo, 46, TRUE);
    }
}

void mbev_CapPlayerShockSet(int playerNo)
{
    int masuId = GwPlayer[playerNo].masuId;
    int i;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        if(i != playerNo && masuId == GwPlayer[i].masuId) {
            mbev_CapPlayerMotionSet(i, 0, 9, 1, HU3D_MOTATTR_NONE, HU3D_MOTATTR_LOOP, TRUE, TRUE);
        }
    }
}

void mbev_CapCameraViewSet(int playerNo, int viewNo, BOOL moveF)
{
    static HuVecF viewRot = { -33, 0, 0 };
    int view;
    if(GwSystem.playerMode < 6) {
        if(viewNo != -1) {
            view = viewNo;
        } else {
            view = mbCameraPlayerViewNoGet();
        }
        mbCameraMoveOnSet(TRUE);
        switch(view) {
            case 0:
                if(!moveF) {
                    mbCameraPlayerViewSet(playerNo, view);
                } else {
                    mbCameraMovePlayer(playerNo, &viewRot, &ev_CapsuleViewOfs, 1800, -1, 21);
                }
                break;
            case 1:
                if(!moveF) {
                    mbCameraPlayerViewSet(playerNo, view);
                } else {
                    mbCameraMovePlayer(playerNo, &viewRot, &ev_CapsuleViewOfs, 2100, -1, 21);
                }
                break;
            case 2:
                if(!moveF) {
                    mbCameraPlayerViewSet(playerNo, view);
                } else {
                    mbCameraMovePlayer(playerNo, &viewRot, &ev_CapsuleViewOfs, 3200, -1, 21);
                }
                break;
            case 3:
                mbCameraMultiFocusSet(0, 0, -1.0, 21);
                break;
        }
        mbCameraMoveWait();
    }
}

void mbev_CapVibrate(int type)
{
    int i;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        switch(type) {
            case 0:
                omVibrate(i, 20, 4, 4);
                break;
            case 1:
                omVibrate(i, 20, 7, 3);
                break;
            case 2:
                omVibrate(i, 20, 20, 0);
                break;
        }
    }
}

BOOL mbev_CapPlayerCheck(int playerNo1, int playerNo2)
{
    if(GWTeamGet() == FALSE) {
        if(playerNo1 == playerNo2) {
            return TRUE;
        } else {
            return FALSE;
        }
    } else {
        if(GWPlayerTeamGet(playerNo1) == GWPlayerTeamGet(playerNo2)) {
            return TRUE;
        } else {
            return FALSE;
        }
    }
}

BOOL mbev_CapCullCheck(int playerNo, int masuId);

BOOL mbev_CapCullPlayerCheck(int playerNo)
{
    return mbev_CapCullCheck(playerNo, 0);
}

BOOL mbev_CapPointCullCheck(HuVecF *pos);

BOOL mbev_CapCullCheck(int playerNo, int masuId)
{
    static float charSize[14][2] = {
        { 100, 175 },
        { 100, 200 },
        { 110, 220 },
        { 100, 175 },
        { 175, 175 },
        { 110, 220 },
        { 200, 260 },
        { 100, 150 },
        { 175, 175 },
        { 150, 125 },
        { 100, 150 },
        { 150, 125 },
        { 150, 125 },
        { 150, 125 },
    };
    static HuVecF pointEdge[5] = {
        { 0, 0, 0 },
        { -1, 2, 0 },
        { 1, 2, 0 },
        { -1, -1, 0 },
        { 1, -1, 0 },
    };
    HuVecF pos;
    HuVecF ofs;
    int charNo;
    int i;
    if(masuId <= 0) {
        mbPlayerPosGet(playerNo, &pos);
    } else {
        mbMasuPosGet(masuId, &pos);
    }
    charNo = GwPlayer[playerNo].charNo;
    for(i=0; i<5; i++) {
        ofs.x = pointEdge[i].x*charSize[charNo][0];
        ofs.y = pointEdge[i].y*charSize[charNo][1];
        ofs.z = pointEdge[i].z*charSize[charNo][0];
        VECAdd(&ofs, &pos, &ofs);
        if(mbev_CapPointCullCheck(&ofs)) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL mbev_CapPointCullCheck(HuVecF *pos)
{
    HuVecF norm;
    mbPos3DtoNorm(pos, 1, &norm);
    if(fabs(norm.x) <= 1.0 && fabs(norm.y) <= 1.0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int mbev_CapPlayerMasuNumGet(int masuId)
{
    int i;
    int num;
    for(i=0, num=0; i<GW_PLAYER_MAX; i++) {
        if(masuId == GwPlayer[i].masuId) {
            num++;
        }
    }
    return num;
}

int mbev_CapPlayerNoSearch(int playerNo)
{
    int i;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        if(i != playerNo) {
            return i;
        }
    }
    return -1;
}

int mbev_CapPlayerOrderGet(int *playerNo, int ignorePlayerNo, int firstPlayerNo, BOOL sortF)
{
    int i;
    int num;
    int j;
    int temp;
    for(i=0, num=0; i<GW_PLAYER_MAX; i++) {
        if(i != ignorePlayerNo) {
            playerNo[num] = i;
            num++;
        }
    }
    if(sortF) {
        for(i=0; i<num-1; i++) {
            for(j=i+1; j<num; j++) {
                if((u8)GwPlayer[playerNo[i]].rank > (u8)GwPlayer[playerNo[j]].rank) {
                    temp = playerNo[i];
                    playerNo[i] = playerNo[j];
                    playerNo[j] = temp;
                }
            }
        }
        for(i=0; i<num; i++) {
            if(firstPlayerNo == playerNo[i] && i != 0) {
                temp = playerNo[i];
                playerNo[i] = playerNo[0];
                playerNo[0] = temp;
            }
        }
    }
    return num;
}

int mbev_CapCoinDisp(int playerNo, int coinNum, BOOL motF, BOOL waitF)
{
    HuVecF pos;
    int dispId;
    mbPlayerPosGet(playerNo, &pos);
    pos.y += 250;
    dispId = mbCoinDispCapsuleCreate(&pos, coinNum);
    if(motF) {
        if(coinNum > 0) {
            mbPlayerWinLoseVoicePlay(playerNo, 12, 579);
            mbev_CapPlayerMotShiftWait(playerNo, 12, HU3D_MOTATTR_NONE, TRUE);
        } else {
            mbPlayerWinLoseVoicePlay(playerNo, 13, 585);
            mbev_CapPlayerMotShiftWait(playerNo, 13, HU3D_MOTATTR_NONE, TRUE);
        }
    }
    if(waitF) {
        do {
            HuPrcVSleep();
        } while(!mbCoinDispKillCheck(dispId));
        if(motF) {
            mbev_CapPlayerMotShiftWait(playerNo, 1, HU3D_MOTATTR_LOOP, TRUE);
        }
    }
    return dispId;
}

BOOL mbev_CapMasuMoveCheck(int masuId)
{
    if((mbMasuAttrGet(masuId) & mbBranchAttrGet()) != 0 || (mbMasuMAttrGet(masuId) & mbBranchMAttrGet()) != 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void mbev_CapChoiceSet(int choice)
{
    capsuleChoice = choice;
    mbWinTopComKeyHookSet(ev_CapComChoiceHook);
}

static void ev_CapComChoiceHook(void)
{
    int comKey[GW_PLAYER_MAX];
    int i;
    int padNo;
    s16 delay;
    int key;
    int playerNo;
    comKey[0] = comKey[1] = comKey[2] = comKey[3] = 0;
    playerNo = GwSystem.turnPlayerNo;
    padNo = GwPlayer[playerNo].padNo;
    delay = GWComKeyDelayGet();
    key = PAD_BUTTON_DOWN;
    comKey[padNo] = key;
    for(i=0; i<capsuleChoice; i++) {
        comKey[padNo] = key;
        HuWinComKeyWait(comKey[0], comKey[1], comKey[2], comKey[3], delay);
    }
    comKey[padNo] = PAD_BUTTON_A;
    HuWinComKeyWait(comKey[0], comKey[1], comKey[2], comKey[3], delay);
}

s16 mbev_CapMasuLinkNextGet(s16 masuId, HuVecF *pos)
{
    s16 linkTbl[10];
    s16 linkNum;
    s16 i;
    s16 nextId;
    s16 forkF;
    if(masuId <= 0) {
        return -1;
    }
    linkNum = mbMasuLinkTblGet(masuId, linkTbl);
    if(mbMasuAttrGet(masuId) & 0x4000) {
        forkF = TRUE;
    } else {
        forkF = FALSE;
    }
    for(i=0; i<linkNum; i++) {
        if(!mbev_CapMasuMoveCheck(linkTbl[i])) {
            if(!forkF || linkNum < 2 || i != 0) {
                nextId = linkTbl[i];
                break;
            }
        }
    }
    if(i >= linkNum) {
        return -1;
    }
    if(pos != NULL) {
        mbMasuPosGet(nextId, pos);
    }
    return nextId;
}

s16 mbev_CapMasuLinkNextRandomGet(s16 masuId, HuVecF *pos)
{
    s16 nextTbl[MASU_LINK_MAX];
    s16 linkTbl[10];
    s16 linkNum;
    s16 i;
    s16 forkF;
    s16 nextNum;
    s16 no;
    if(masuId <= 0) {
        return -1;
    }
    linkNum = mbMasuLinkTblGet(masuId, linkTbl);
    if(mbMasuAttrGet(masuId) & 0x4000) {
        forkF = TRUE;
    } else {
        forkF = FALSE;
    }
    for(i=0, nextNum=0; i<linkNum; i++) {
        if(!mbev_CapMasuMoveCheck(linkTbl[i])) {
            if(!forkF || linkNum < 2 || i != 0) {
                nextTbl[nextNum] = linkTbl[i];
                nextNum++;
            }
        }
    }
    if(nextNum <= 0) {
        return -1;
    }
    if(nextNum > 1) {
        no = mbRandMod(nextNum);
    } else {
        no = 0;
    }
    if(pos != NULL) {
        mbMasuPosGet(nextTbl[no], pos);
    }
    return nextTbl[no];
}

s16 mbev_CapMasuValidPrevGet(s16 masuId, HuVecF *pos)
{
    s16 linkTbl[10];
    s16 i;
    s16 prevId;
    s16 linkNum;
    if(masuId <= 0) {
        return -1;
    }
    linkNum = mbMasuLinkParentGet(masuId, linkTbl);
    for(i=0; i<linkNum; i++) {
        if(!mbev_CapMasuMoveCheck(linkTbl[i])) {
            prevId = linkTbl[i];
            break;
        }
    }
    if(i >= linkNum) {
        return -1;
    }
    if(pos != NULL) {
        mbMasuPosGet(prevId, pos);
    }
    return prevId;
}

s16 mbev_CapMasuPrevGet(s16 masuId, HuVecF *pos)
{
    s16 linkTbl[10];
    s16 i;
    s16 prevId;
    s16 linkNum;
    if(masuId <= 0) {
        return -1;
    }
    linkNum = mbMasuLinkParentGet(masuId, linkTbl);
    for(i=0; i<linkNum; i++) {
        prevId = linkTbl[i];
        break;
    }
    if(i >= linkNum) {
        return -1;
    }
    if(pos != NULL) {
        mbMasuPosGet(prevId, pos);
    }
    return prevId;
}

int mbev_CapPlayerComSelSameGet(int playerNo, int type, BOOL sameMasuF);

int mbev_CapPlayerComSelGet(int playerNo, int type)
{
    return mbev_CapPlayerComSelSameGet(playerNo, type, FALSE);
}

int mbev_CapPlayerComSelRandomGet(int playerNo, int type, int *playerTbl, int num);

int mbev_CapPlayerComSelSameGet(int playerNo, int type, BOOL sameMasuF)
{
    int playerTbl[GW_PLAYER_MAX];
    int i;
    int num;
    for(i=0, num=0; i<GW_PLAYER_MAX; i++) {
        if(GWTeamGet() != FALSE && mbev_CapPlayerCheck(playerNo, i)) {
            continue;
        }
        if(sameMasuF && (s16)GwPlayer[playerNo].masuId != GwPlayer[i].masuId) {
            continue;
        }
        if(i == playerNo) {
            continue;
        }
        playerTbl[num] = i;
        num++;
    }
    return mbev_CapPlayerComSelRandomGet(playerNo, type, playerTbl, num);
}

int mbev_CapPlayerComSelRandomGet(int playerNo, int type, int *playerTbl, int num)
{
    static int chanceTbl[GW_PLAYER_MAX] = { 100, 60, 30, 10 };
    int validTbl[GW_PLAYER_MAX];
    int i;
    int validNum;
    int j;
    int chanceNum;
    int temp;
    int a;
    int b;
    int total;
    int chance;
    for(i=0, validNum=0; i<num; i++) {
        if(playerTbl[i] >= 0) {
            validTbl[validNum] = playerTbl[i];
            validNum++;
        }
    }
    if(validNum <= 0) {
        return -1;
    }
    if(validNum == 1) {
        return validTbl[0];
    }
    for(i=0; i<64; i++) {
        a = mbRandMod(validNum);
        b = mbRandMod(validNum);
        if(a != b) {
            temp = validTbl[a];
            validTbl[a] = validTbl[b];
            validTbl[b] = temp;
        }
    }
    switch(type) {
        case 0:
            for(i=0; i<validNum-1; i++) {
                for(j=i+1; j<validNum; j++) {
                    if(mbPlayerCoinGet(validTbl[i]) < mbPlayerCoinGet(validTbl[j])) {
                        temp = validTbl[i];
                        validTbl[i] = validTbl[j];
                        validTbl[j] = temp;
                    }
                }
            }
            for(i=0, chanceNum=0; i<validNum; i++) {
                if(mbPlayerCoinGet(validTbl[i]) > 0) {
                    chanceNum++;
                }
            }
            if(chanceNum <= 0) {
                chanceNum = validNum;
            }
            break;
        case 1:
            for(i=0; i<validNum-1; i++) {
                for(j=i+1; j<validNum; j++) {
                    if(mbPlayerStarGet(validTbl[i]) < mbPlayerStarGet(validTbl[j])) {
                        temp = validTbl[i];
                        validTbl[i] = validTbl[j];
                        validTbl[j] = temp;
                    }
                }
            }
            for(i=0, chanceNum=0; i<validNum; i++) {
                if(mbPlayerStarGet(validTbl[i]) > 0) {
                    chanceNum++;
                }
            }
            if(chanceNum <= 0) {
                chanceNum = validNum;
            }
            break;
        case 2:
            for(i=0; i<validNum-1; i++) {
                for(j=i+1; j<validNum; j++) {
                    if(mbPlayerCapsuleNumGet(validTbl[i]) < mbPlayerCapsuleNumGet(validTbl[j])) {
                        temp = validTbl[i];
                        validTbl[i] = validTbl[j];
                        validTbl[j] = temp;
                    }
                }
            }
            for(i=0, chanceNum=0; i<validNum; i++) {
                if(mbPlayerCapsuleNumGet(validTbl[i]) > 0) {
                    chanceNum++;
                }
            }
            if(chanceNum <= 0) {
                chanceNum = validNum;
            }
            break;
        default:
            if(validNum > 0) {
                return validTbl[mbRandMod(validNum)];
            } else {
                return -1;
            }
    }
    if(chanceNum == 1) {
        return validTbl[0];
    }
    for(i=0, total=0; i<chanceNum; i++) {
        total += chanceTbl[i];
    }
    chance = total*(mbRandMod(0x10000000)*(1.0f/0x10000000));
    for(i=0; i<chanceNum; i++) {
        if(chance < chanceTbl[i]) {
            break;
        }
    }
    if(i >= chanceNum) {
        return validTbl[0];
    }
    return validTbl[i];
}

int mbev_CapPlayerComSelKettouGet(int playerNo, int type, int *playerTbl, int num)
{
    int selPlayer;
    int i;
    int no;
    selPlayer = mbev_CapPlayerComSelRandomGet(playerNo, type, playerTbl, num);
    for(i=0; i<num; i++) {
        if(selPlayer == playerTbl[i]) {
            no = i;
            break;
        }
    }
    for(i=0; i<no; i++) {
        if(playerTbl[i] < 0) {
            no--;
        }
    }
    return no;
}

float mbev_CapAngleWrap(float start, float end)
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

float mbev_CapAngleLerp(float start, float end, float speed)
{
    float diff;
    float result;
    if(start >= 360.0) {
        start -= 360.0;
    } else if(start < 0.0) {
        start += 360.0;
    }
    if(end >= 360.0) {
        end -= 360.0;
    } else if(end < 0.0) {
        end += 360.0;
    }
    diff = 360.0+(start-end);
    if(fabs(diff) >= 360.0) {
        diff = fmod(diff, 360.0);
    }
    if(diff < 180.0) {
        if(diff <= speed) {
            result = diff;
        } else {
            result = speed;
        }
    } else {
        if(360.0-diff <= speed) {
            result = -(360.0-diff);
        } else {
            result = -speed;
        }
    }
    result += end;
    if(result >= 360.0) {
        result -= 360.0;
    } else if(result < 0.0) {
        result += 360.0;
    }
    return result;
}

float mbev_CapAngleSumLerp(float t, float start, float end)
{
    float diff = mbev_CapAngleWrap(end, start);
    return mbev_CapAngleLerp(end, start, fabs(diff*t));
}

static float ev_CapRotCamera(float angle)
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

void mbev_CapVecChase(float t, HuVecF *start, HuVecF *end, HuVecF *out)
{
    HuVecF dir;
    VECSubtract(end, start, &dir);
    VECScale(&dir, &dir, t);
    VECAdd(start, &dir, out);
}

void mbev_CapVecRotGet(HuVecF *dir, HuVecF *rot)
{
    HuVecF temp;
    temp.x = HuAtan(-dir->y, EvSqrtF(HuMag2Point2D(dir->x, dir->z)));
    temp.y = HuAtan(dir->x, dir->z);
    temp.z = 0;
    *rot = temp;
}

void mbev_CapColorLerp(float t, GXColor *a, GXColor *b, GXColor *out)
{
    out->r = (int)((float)a->r+t*((float)b->r-(float)a->r));
    out->g = (int)((float)a->g+t*((float)b->g-(float)a->g));
    out->b = (int)((float)a->b+t*((float)b->b-(float)a->b));
    out->a = (int)((float)a->a+t*((float)b->a-(float)a->a));
}

void mbev_CapHermiteConstGet(float t, float *h1, float *h2, float *h3, float *h4)
{
    float t2 = t*t;
    float t3 = t*t2;
    float h = 3.0*t2-t3-t3;
    *h1 = 1.0-h;
    *h2 = h;
    *h3 = t+(t3-t2-t2);
    *h4 = t3-t2;
}

float mbev_CapHermiteConstGet2(float t, float p0, float p1, float p2, float p3)
{
    float diff;
    float h1;
    float h2;
    float h3;
    float h4;
    float result;
    float rate1;
    float rate2;
    float tan1;
    float tan2;
    int diffInt;
    diff = p2-p1;
    diffInt = p2-p1;
    if(p1 == p2) {
        tan1 = diff;
    } else {
        rate1 = 0.5f;
        tan1 = rate1*(diff+(p1-p0));
    }
    if(p1 == p3) {
        tan2 = diff;
    } else {
        rate2 = 0.5f;
        tan2 = rate2*(diff+(p3-p2));
    }
    mbev_CapHermiteConstGet(t, &h1, &h2, &h3, &h4);
    return result = h4*tan2+(h3*tan1+(h1*p1+h2*p2));
}

void mbev_CapHermiteGetV(float t, HuVecF *p0, HuVecF *p1, HuVecF *p2, HuVecF *p3, HuVecF *out)
{
    out->x = mbev_CapHermiteConstGet2(t, p0->x, p1->x, p2->x, p3->x);
    out->y = mbev_CapHermiteConstGet2(t, p0->y, p1->y, p2->y, p3->y);
    out->z = mbev_CapHermiteConstGet2(t, p0->z, p1->z, p2->z, p3->z);
}

float mbev_CapBezierGet(float t, float p0, float p1, float p2)
{
    float it = 1.0-t;
    float result = p2*(t*t)+(p0*(it*it)+2.0*(p1*(it*t)));
    return result;
}

void mbev_CapBezierGetV(float t, float *p0, float *p1, float *p2, float *out)
{
    int i;
    for(i=0; i<3; i++) {
        *out++ = mbev_CapBezierGet(t, *p0++, *p1++, *p2++);
    }
}

float mbev_CapBezierSlopeGet(float t, float p0, float p1, float p2)
{
    float result = 2.0*(t*p2+((t-1.0)*p0+(1.0-2.0*t)*p1));
    return result;
}

void mbev_CapBezierNormGetV(float t, float *p0, float *p1, float *p2, float *out)
{
    float slope[3];
    int i;
    float len;
    for(i=0; i<3; i++) {
        slope[i] = mbev_CapBezierSlopeGet(t, *p0++, *p1++, *p2++);
    }
    len = EvSqrtF(slope[2]*slope[2]+(slope[0]*slope[0]+slope[1]*slope[1]));
    if(len) {
        len = 1.0/len;
        for(i=0; i<3; i++) {
            *out++ = len*slope[i];
        }
    } else {
        *out++ = 0;
        *out++ = 0;
        *out++ = 1.0f;
    }
}
/* @@END@@ */
