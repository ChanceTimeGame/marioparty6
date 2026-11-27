/* @@HEAD@@ */
#include "game/board/snpc.h"
#include "game/board/audio.h"
#include "game/board/branch.h"
#include "game/board/camera.h"
#include "game/board/capselect.h"
#include "game/board/coin.h"
#include "game/board/comchoice.h"
#include "game/board/dice.h"
#include "game/board/exit.h"
#include "game/board/guide.h"
#include "game/board/object.h"
#include "game/board/malloc.h"
#include "game/board/masu.h"
#include "game/board/math.h"
#include "game/board/player.h"
#include "game/board/star.h"
#include "game/board/telop.h"
#include "game/board/window.h"
#include "game/board/wipe.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/memory.h"
#include "game/sprite.h"


void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);


#define SNPC_MAGIC 'SNPC'

/* Material hook work of mbObjFade*: a texture projected through the model. */
typedef struct MbObjFadeWork_s {
    /* 0x00 */ u32 magic;
    /* 0x04 */ ANIMDATA *anim;
    /* 0x08 */ HuVecF pos;
    /* 0x14 */ HuVecF rot;
    /* 0x20 */ float scale;
    /* 0x24 */ GXColor color;
} MBOBJFADEWORK; /* 0x28 */

#define MBOBJFADE_MAGIC 'MBTV'

/* mbObjMetal*: a metal environment map with a transparency level. */
typedef struct MbObjMetalWork_s {
    /* 0x00 */ u32 magic;
    /* 0x04 */ ANIMDATA *anim;
    /* 0x08 */ ANIMDATA *hiliteAnim;
    /* 0x0C */ float tpLvl;
    /* 0x10 */ GXColor shadowColor;
    /* 0x14 */ GXColor hiliteColor;
} MBOBJMETALWORK; /* 0x18 */

#define MBOBJMETAL_MAGIC 'TV01'

/* mbObjBiriQ*: the electric tint. */
typedef struct MbObjBiriQWork_s {
    /* 0x00 */ u32 magic;
    /* 0x04 */ int mode;
    /* 0x08 */ float alpha;
    /* 0x0C */ GXColor color;
} MBOBJBIRIQWORK; /* 0x10 */

#define MBOBJBIRIQ_MAGIC 'TV02'

/* One TEV colour or alpha setting: op, output register, then the four inputs. */
typedef struct BiriQMat_s {
    u8 op;
    u8 outReg;
    u8 a;
    u8 b;
    u8 c;
    u8 d;
} BIRIQMAT;

/* One motion of the NPC model: data number, speed, loop flag, start/end frames (0: none). */
typedef struct SNpcMot_s {
    /* 0x00 */ u32 dataNum;
    /* 0x04 */ float speed;
    /* 0x08 */ u16 loopF;
    /* 0x0A */ u8 start;
    /* 0x0B */ u8 end;
} SNPCMOT;

/* What follows a motion: sound slot (0: none) and up to three frame thresholds (-1 ends). */
typedef struct SNpcNextMot_s {
    /* 0x00 */ s16 seNo;
    /* 0x02 */ s16 time[3];
} SNPCNEXTMOT;

typedef struct SNpcWork_s {
    /* 0x000 */ int level;
    /* 0x004 */ int masuType; /* the type of the space the NPC stands on, restored when it leaves */
    /* 0x008 */ int motNo;
    /* 0x00C */ BOOL jumpF;
    /* 0x010 */ int motTime;
    /* 0x014 */ HuVecF pos;
    /* 0x020 */ HuVecF rot;
    /* 0x02C */ OMOBJ *moveObj;
    /* 0x030 */ OMOBJ *diceNumObj;
    /* 0x034 */ OMOBJ *zoomObj;
    /* 0x038 */ OMOBJ *starObj;
    /* 0x03C */ OMOBJ *playerMoveObj;
    /* 0x040 */ s16 objId;
    /* 0x042 */ s16 unk42;
    /* 0x044 */ s16 masuEffObjId;
    /* 0x046 */ u8 unk46[0x50 - 0x46];
    /* 0x050 */ s16 motId[11];
    /* 0x066 */ u8 unk66[0x76 - 0x66];
    /* 0x076 */ s16 path[(0x298 - 0x76) / 2]; /* the spaces to walk, ending with 0 */
} SNPCWORK; /* 0x298 */

/* OMOBJ work of SNpcMoveOMExec: one step to the next space. */
typedef struct SNpcMoveWork_s {
    /* 0x00 */ u8 initF : 1;
    /* 0x00 */ u8 killF : 1;
    /* 0x00 */ u8 jumpF : 2;
    /* 0x02 */ s16 time;
    /* 0x04 */ s16 maxTime;
    /* 0x06 */ s16 rotTime;
    /* 0x08 */ s16 mode;
    /* 0x0A */ s16 masuId;
} SNPCMOVEWORK;

/* OMOBJ work of the rotate, position-fix and zoom objects. */
typedef struct SNpcTimeWork_s {
    /* 0x00 */ u8 initF : 1;
    /* 0x00 */ u8 killF : 1;
    /* 0x02 */ s16 time;
    /* 0x04 */ s16 maxTime;
    /* 0x06 */ s16 mode;
    /* 0x08 */ float start; /* the target angle for a rotation */
    /* 0x0C */ float end;
} SNPCTIMEWORK;

static int snpcUnusedSbss;
static SNPCWORK *snpcWork;
static SNPCSAVEWORK *snpcSaveWork;
static int snpcMagic;

static HuVecF masuViewOfs = { 0.0f, 100.0f, 0.0f };
static u32 snpcMesTbl[6][2] = {
    { 0x2D0000, 0x2D0005 },
    { 0x2D0001, 0x2D0006 },
    { 0x2D0004, 0 },
    { 0x2D0009, 0x2D000B },
    { 0x2D000A, 0x2D000C },
    { 0x2D0002, 0x2D0007 }
};
static u32 snpcStarMesTbl[5][2] = {
    { 0x2D000D, 0x2D0012 },
    { 0x2D000E, 0x2D0013 },
    { 0x2D0010, 0x2D0015 },
    { 0x2D0011, 0x2D0016 },
    { 0x2D000F, 0x2D0014 }
};
static u32 snpcEffectChanceTbl[4] = { 30, 55, 80, 95 };
static int snpcDiceTypeTbl[2][2] = { { 16, 17 }, { 18, 18 } };
static HuVecF snpcDiceOfsTbl[2] = { { 0.0f, 90.0f, 0.0f }, { 0.0f, 90.0f, 0.0f } };
static int snpcSeTbl[8][2] = {
    { 0, 0 }, { 935, 971 }, { 937, 972 }, { 1135, 974 },
    { 933, 969 }, { 934, 970 }, { 993, 993 }, { 995, 1119 }
};
static SNPCNEXTMOT snpcDonkeyNextMotTbl[11] = {
    { 0, { -1, -1, -1 } }, { 4, { 2, 17, -1 } }, { 5, { 2, 17, -1 } }, { 0, { -1, -1, -1 } },
    { 6, { 0, -1, -1 } }, { 7, { 2, -1, -1 } }, { 0, { -1, -1, -1 } }, { 0, { -1, -1, -1 } },
    { 1, { 0, -1, -1 } }, { 2, { 0, -1, -1 } }, { 0, { -1, -1, -1 } }
};
static SNPCNEXTMOT snpcKoopaNextMotTbl[11] = {
    { 0, { -1, -1, -1 } }, { 4, { 6, 30, -1 } }, { 5, { 6, 30, -1 } }, { 0, { -1, -1, -1 } },
    { 6, { 0, -1, -1 } }, { 7, { 2, -1, -1 } }, { 0, { -1, -1, -1 } }, { 0, { -1, -1, -1 } },
    { 1, { 0, -1, -1 } }, { 2, { 0, -1, -1 } }, { 0, { -1, -1, -1 } }
};
static SNPCMOT snpcDonkeyMotTbl[11] = {
    { 0xE000F, 1.0f, 1, 0, 0 },
    { 0xE0010, 1.0f, 1, 0, 0 },
    { 0xE0010, 1.0f, 1, 0, 0 },
    { 0xE0011, 1.0f, 0, 0, 5 },
    { 0xE0011, 1.0f, 0, 6, 0 },
    { 0xE0012, 1.0f, 0, 0, 0 },
    { 0xE0017, 1.0f, 0, 0, 0 },
    { 0xE001A, 1.0f, 0, 0, 0 },
    { 0xE0013, 1.0f, 0, 0, 0 },
    { 0xE0019, 1.0f, 0, 0, 0 },
    { 0xE001C, 1.0f, 0, 0, 0 }
};
static SNPCMOT snpcKoopaMotTbl[11] = {
    { 0xE0001, 1.0f, 1, 0, 0 },
    { 0xE0006, 1.2f, 1, 0, 0 },
    { 0xE0006, 1.2f, 1, 0, 0 },
    { 0xE0005, 1.0f, 0, 0, 20 },
    { 0xE0005, 1.0f, 0, 21, 40 },
    { 0xE0005, 1.0f, 0, 50, 0 },
    { 0xE0007, 1.0f, 0, 0, 0 },
    { 0xE0004, 1.0f, 0, 0, 0 },
    { 0xE0003, 1.0f, 0, 0, 0 },
    { 0xE000A, 1.0f, 0, 0, 0 },
    { 0xE000B, 1.0f, 0, 0, 0 }
};
static HU3D_PARMAN_PARAM snpcStarEffParam[2] = {
    {
        30, 0, 3.3f, 70.0f, 7.0f, { 0.0f, -0.05f, 0.0f }, 2.0f, 1.0f, 30.0f, 0.98f, 2,
        { { 0xFF, 0xFF, 0xFF, 0xFF }, { 0xFF, 0xFF, 0x40, 0xFF }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 } },
        { { 0xFF, 0x80, 0x80, 0x00 }, { 0xFF, 0x40, 0x20, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 } }
    },
    {
        30, 0, 3.3f, 70.0f, 7.0f, { 0.0f, -0.05f, 0.0f }, 2.0f, 1.0f, 30.0f, 0.98f, 2,
        { { 0x90, 0x90, 0x90, 0xFF }, { 0xA0, 0x90, 0xB0, 0xFF }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 } },
        { { 0x10, 0x10, 0x10, 0x00 }, { 0x20, 0x00, 0x30, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 } }
    }
};

static int snpcMesSpeakerTbl[2] = { 6, 13 };
static int playerStarMotNoTbl[2] = { 7, 8 };
static int snpcStarStrmTbl[2] = { 30, 28 };
static int playerStarSeTbl[2] = { 573, 585 };
static int snpcStarMesSpeakerTbl[2] = { 6, 13 };
static int snpcMoveStrmTbl[2] = { 30, 28 };
static int playerStarChgMotNoTbl[2] = { 7, 8 };
static int playerStarChgSeTbl[2] = { 573, 585 };
static float snpcRotSpeedTbl[2] = { 1.3f, 1.8f };
static float snpcPosFixSpeedTbl[2] = { 1.3f, 1.8f };
static int snpcMoveNumColor[2] = { 1, 2 };
static int snpcDiceMotTimeTbl[2] = { 25, 25 };
static SNPCNEXTMOT *snpcNextMotTbl[2] = { snpcDonkeyNextMotTbl, snpcKoopaNextMotTbl };
static SNPCMOT *snpcMotTbl[2] = { snpcDonkeyMotTbl, snpcKoopaMotTbl };
static u32 starMdlTbl[2] = { 0x50005, 0xE0020 };

/* Part 2's functions, defined later in the unit. */
static void SNpcEffectExec(void);
static void SNpcPlayerMoveFunc(int playerNo);
static void SNpcSePlay(int seNo, int arg1);
static void SNpcMotSetNext(void);
static void SNpcObjCreate(void);
static void SNpcObjKill(void);
static void SNpcObjDispSet(BOOL dispF);
static void SNpcObjPosSet(float x, float y, float z);
static void SNpcObjPosSetV(HuVecF *pos);
static void SNpcObjPosGet(HuVecF *pos);
static void SNpcObjRotSet(float x, float y, float z);
static void SNpcObjRotSetV(HuVecF *rot);
static void SNpcObjRotGet(HuVecF *rot);
static void SNpcObjMotSet(int motNo);
static void SNpcObjMotShiftSet(int motNo);
static BOOL SNpcObjMotEndCheck(void);
static void SNpcObjMotEndWait(void);
static void SNpcObjMasuSet(int masuId);
static void SNpcMasuEffDispSet(void);
static int SNpcDiceValueGet(int masuId, int num, int *tbl, int level, s16 *path, float speed);
static void SNpcStarCreate(int koopaF, BOOL coinF, HuVecF *pos);
static void SNpcStarWait(void);

static GXTevKColorSel kColorTbl[8] = {
    GX_TEV_KCSEL_8_8,
    GX_TEV_KCSEL_7_8,
    GX_TEV_KCSEL_6_8,
    GX_TEV_KCSEL_5_8,
    GX_TEV_KCSEL_4_8,
    GX_TEV_KCSEL_3_8,
    GX_TEV_KCSEL_2_8,
    GX_TEV_KCSEL_1_8
};

static int biriQMatNumTbl[4] = { 2, 1, 1, 2 };

/* Per mode, up to two stages of { colour, alpha }. */
static BIRIQMAT biriQMatTbl[16] = {
    { 0x00, 0x01, 0x0F, 0x0F, 0x0F, 0x0E }, { 0x00, 0x01, 0x07, 0x07, 0x07, 0x06 },
    { 0x00, 0x00, 0x00, 0x02, 0x03, 0x0F }, { 0x00, 0x00, 0x07, 0x07, 0x07, 0x00 },
    { 0x00, 0x00, 0x0E, 0x0F, 0x0F, 0x00 }, { 0x00, 0x00, 0x07, 0x07, 0x07, 0x00 },
    { 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x00 }, { 0x00, 0x00, 0x07, 0x07, 0x07, 0x00 },
    { 0x01, 0x00, 0x0E, 0x0F, 0x0F, 0x00 }, { 0x00, 0x00, 0x07, 0x07, 0x07, 0x00 },
    { 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x00 }, { 0x00, 0x00, 0x07, 0x07, 0x07, 0x00 },
    { 0x01, 0x01, 0x00, 0x0F, 0x0F, 0x0C }, { 0x00, 0x01, 0x07, 0x07, 0x07, 0x06 },
    { 0x00, 0x00, 0x00, 0x02, 0x03, 0x0F }, { 0x00, 0x00, 0x07, 0x07, 0x07, 0x00 }
};

static GXColor texCol[16];

/* @@PART1@@ */
static void SNpcStarFunc(void);
static int SNpcMoveExec(void);
static void SNpcMoveOMExec(OMOBJ *obj);
static void SNpcRotateUpdate(OMOBJ *obj);
static void SNpcPosFixUpdate(OMOBJ *obj);
static void SNpcZoomUpdate(OMOBJ *obj);
static u16 SNpcDiceBtnHook(int playerNo);
static void SNpcDiceMotHook(int playerNo);
static void SNpcMasuSet(int masuId, BOOL seF);
static int SNpcMasuStarNextGet(BOOL nearF);
static void SNpcPosFixSnap(void);
static void StarChangeExec(s8 *list, BOOL coinF, int num);
static int MasuNextGet(int masuId, int pathNo);
static void SNpcTargetAngleSet(float angle);
static void SNpcRotateWait(void);
static void SNpcPosFixCreate(void);
static void SNpcZoomSet(float zoom);
static BOOL SNpcZoomCheck(void);
static void SNpcZoomWait(void);
static OMOBJ *SNpcDiceExec(int value, int diceNum);
static void SNpcMasuReset(BOOL seF);

void mbSNpcInit(void)
{
    snpcMagic = 0;
    snpcSaveWork = NULL;
    snpcWork = NULL;
}

void mbSNpcCreate(SNPCSAVEWORK *saveWork, int level)
{
    BOOL dayF;
    BOOL koopaF;

    snpcSaveWork = saveWork;
    snpcWork = mbMalloc(sizeof(SNPCWORK));
    snpcWork->level = level;
    if(mbSaveNewF) {
        snpcSaveWork->masuId = mbMasuFind_AttrIdGet(-1, 0x8000);
        snpcSaveWork->masuId = SNpcMasuStarNextGet(TRUE);
        snpcSaveWork->effectNum = 0;
    }
    dayF = GwSystem.curTime == FALSE;
    if(dayF) {
        koopaF = FALSE;
    } else {
        koopaF = TRUE;
    }
    snpcSaveWork->koopaF = koopaF;
    SNpcMasuSet(snpcSaveWork->masuId, FALSE);
    SNpcObjCreate();
    mbPlayerEndTurnHookSet(0, SNpcMoveExec);
    mbPlayerEndTurnHookSet(1, SNpcMoveExec);
    mbPlayerEndTurnHookSet(2, SNpcMoveExec);
    mbPlayerEndTurnHookSet(3, SNpcMoveExec);
    mbStarMasuFuncSet(SNpcStarFunc);
    snpcMagic = SNPC_MAGIC;
}

void mbSNpcKill(void)
{
    SNPCWORK *work;

    if(snpcMagic == SNPC_MAGIC) {
        mbPlayerEndTurnHookSet(3, NULL);
        SNpcObjKill();
        work = snpcWork;
        HuMemDirectFree(work);
        snpcWork = NULL;
        snpcSaveWork = NULL;
        snpcMagic = 0;
    }
}

u8 mbSNpcMasuGet(void)
{
    if(snpcMagic != SNPC_MAGIC) {
        return 0;
    }
    return snpcSaveWork->masuId;
}

void mbSNpcMotWinSet(void)
{
    if(snpcMagic == SNPC_MAGIC) {
        SNpcSePlay(1, 0);
        SNpcObjMotShiftSet(8);
    }
}

void mbSNpcMotIdleSet(void)
{
    if(snpcMagic == SNPC_MAGIC) {
        SNpcObjMotShiftSet(0);
    }
}

void mbSNpcDispSet(BOOL dispF)
{
    if(snpcMagic == SNPC_MAGIC) {
        SNpcObjDispSet(dispF);
    }
}

static void SNpcStarFunc(void)
{
}

void mbSNpcMotReset(void)
{
    if(snpcMagic == SNPC_MAGIC) {
        SNpcPosFixSnap();
        SNpcObjMotSet(0);
    }
}

void mbSNpcPlayerWalkSet(int playerNo, int masuId)
{
    if(snpcMagic == SNPC_MAGIC && masuId == snpcSaveWork->masuId) {
        mbPlayerMoveHookSet(playerNo, SNpcPlayerMoveFunc);
    }
}

void mbSNpcStarExec(int playerNo, int masuId)
{
    HuVecF pos;
    HuVecF dir;
    HuVecF rot;
    int unused = -1;
    int koopaF;
    int mode;
    int winId;
    int maxTime;
    int i;
    int strm;
    BOOL contF;
    int jingle;
    int nextMasu;
    s16 moveTime;
    float s;

    if(snpcMagic == SNPC_MAGIC && masuId == snpcSaveWork->masuId) {
        mbCameraStackPush();
        mbMoveNumDispSet(playerNo, FALSE);
        mbCameraMoveMasu(masuId, NULL, &masuViewOfs, mbCameraPlayerViewZoomGet(0), -1.0f, 24);
        koopaF = snpcSaveWork->koopaF;
        mode = 0;
        if(koopaF == FALSE) {
            if(mbPlayerCoinGet(playerNo) >= 20) {
                winId = mbWinCreateChoice(2, snpcMesTbl[0][koopaF], snpcMesSpeakerTbl[koopaF], 0);
                if(GwPlayer[playerNo].comF) {
                    mbComChoiceUpSet();
                }
                mode = 1;
            } else {
                winId = mbWinCreate(2, snpcMesTbl[1][koopaF], snpcMesSpeakerTbl[koopaF]);
            }
        } else {
            if(mbPlayerStarGet(playerNo) > 0) {
                winId = mbWinCreate(2, snpcMesTbl[0][koopaF], snpcMesSpeakerTbl[koopaF]);
                mode = 2;
            } else {
                winId = mbWinCreate(2, snpcMesTbl[1][koopaF], snpcMesSpeakerTbl[koopaF]);
                mode = 3;
            }
        }
        SNpcSePlay(1, 0);
        mbPlayerPosGet(playerNo, &pos);
        SNpcObjPosGet(&dir);
        PSVECSubtract(&dir, &pos, &dir);
        PSVECNormalize(&dir, &dir);
        SNpcTargetAngleSet(HuAtan(-dir.x, -dir.z));
        maxTime = 7;
        PSVECScale(&dir, &dir, -100.0f / maxTime);
        mbPlayerPosGet(playerNo, &pos);
        mbPlayerRotGet(playerNo, &rot);
        rot.z = mbAngleWrap2(HuAtan(-dir.x, -dir.z), rot.y);
        mbPlayerMotionSet(playerNo, 9, 0);
        for(i = 0; i < maxTime; i++) {
            s = HuSin(90.0f * (i + 1) / maxTime);
            mbPlayerRotSet(playerNo, 0.0f, rot.y + s * rot.z, 0.0f);
            HuPrcVSleep();
        }
        strm = snpcStarStrmTbl[koopaF];
        mbMusPlay(0, strm, 127, 0);
        while(!mbPlayerMotionEndCheck(playerNo)) {
            HuPrcVSleep();
        }
        mbPlayerMotIdleSet(playerNo);
        SNpcRotateWait();
        contF = TRUE;
        switch(mode) {
            case 0:
                mbWinWait(winId);
                winId = mbWinCreate(2, snpcMesTbl[5][koopaF], snpcMesSpeakerTbl[koopaF]);
                SNpcSePlay(2, 0);
                SNpcObjMotShiftSet(9);
                mbWinWait(winId);
                mbPlayerWinLoseVoicePlay(playerNo, 13, 585);
                mbPlayerMotionShiftSet(playerNo, 13, 0.0f, 8.0f, 0);
                mbPlayerMotionEndWait(playerNo);
                contF = FALSE;
                break;

            case 1:
                mbWinWait(winId);
                if(mbWinChoiceGet(winId) == 0) {
                    jingle = -1;
                    mbCoinAddDispExec(playerNo, -20, FALSE, TRUE);
                    mbPlayerPosGet(playerNo, &pos);
                    SNpcStarCreate(koopaF, TRUE, &pos);
                    mbPlayerRotateStart(playerNo, 0, 15);
                    SNpcObjMotShiftSet(7);
                    HuPrcSleep(150);
                    SNpcTargetAngleSet(0.0f);
                    mbPlayerMotionShiftSet(playerNo, 11, 0.0f, 8.0f, 0);
                    mbMusPauseFadeOut(0, TRUE, 1000);
                    mbPlayerMotionEndWait(playerNo);
                    mbPlayerMotIdleSet(playerNo);
                    SNpcStarWait();
                    omVibrate(playerNo, 20, 7, 3);
                    mbPlayerStarAdd(playerNo, 1);
                    mbPlayerWinLoseVoicePlay(playerNo, playerStarMotNoTbl[koopaF], playerStarSeTbl[koopaF]);
                    mbPlayerMotionShiftSet(playerNo, playerStarMotNoTbl[koopaF], 0.0f, 8.0f, 0);
                    SNpcSePlay(1, 0);
                    SNpcObjMotShiftSet(8);
                    jingle = mbMusJinglePlay(39);
                    SNpcObjMotEndWait();
                    mbPlayerMotionEndWait(playerNo);
                    HuPrcSleep(60);
                    mbMusJingleWait(jingle);
                    mbMusPauseFadeOut(0, FALSE, 1000);
                    mbPlayerMotIdleSet(playerNo);
                } else {
                    SNpcSePlay(2, 0);
                    SNpcObjMotShiftSet(9);
                    winId = mbWinCreate(2, snpcMesTbl[2][koopaF], snpcMesSpeakerTbl[koopaF]);
                    mbWinWait(winId);
                }
                break;

            case 2:
            case 3:
                mbWinWait(winId);
                if(mode == 3) {
                    winId = mbWinCreate(2, snpcMesTbl[5][koopaF], snpcMesSpeakerTbl[koopaF]);
                    mbWinWait(winId);
                }
                mbPlayerPosGet(playerNo, &pos);
                SNpcStarCreate(koopaF, TRUE, &pos);
                SNpcObjMotShiftSet(7);
                SNpcStarWait();
                omVibrate(playerNo, 20, 7, 3);
                SNpcTargetAngleSet(0.0f);
                if(mode == 2) {
                    mbStarAddProcExec(playerNo, -1, -1, TRUE);
                } else {
                    mbCoinAddProcExec(playerNo, -20, -1, TRUE);
                }
                mbPlayerWinLoseVoicePlay(playerNo, playerStarMotNoTbl[koopaF], playerStarSeTbl[koopaF]);
                mbPlayerMotionShiftSet(playerNo, playerStarMotNoTbl[koopaF], 0.0f, 8.0f, 0);
                SNpcSePlay(1, 0);
                SNpcObjMotShiftSet(8);
                SNpcObjMotEndWait();
                mbPlayerMotionEndWait(playerNo);
                break;
        }
        if(contF) {
            nextMasu = SNpcMasuStarNextGet(FALSE);
            SNpcObjMotShiftSet(3);
            SNpcObjMotEndWait();
            if(snpcSaveWork->koopaF == TRUE) {
                mbCameraShakeSet(12, 50.0f);
            }
            SNpcSePlay(3, 0);
            SNpcObjMotShiftSet(4);
            SNpcObjPosGet(&pos);
            maxTime = 30;
            for(i = 0; i < maxTime; i++) {
                s = (float)i / maxTime;
                s = HuSin(80.0f * s);
                SNpcObjPosSet(pos.x, pos.y + 100.0f * (8.0f * s), pos.z);
                HuPrcVSleep();
            }
            SNpcObjDispSet(FALSE);
            SNpcMasuReset(TRUE);
            mbCameraMovePlayer(-1, NULL, NULL, mbCameraPlayerViewZoomGet(2), -1.0f, 24);
            mbCameraMoveWait();
            mbMasuPosGet(nextMasu, &pos);
            mbMasuPosGet(masuId, &dir);
            PSVECSubtract(&dir, &pos, &dir);
            s = 0.00066666666f * PSVECMag(&dir);
            moveTime = (u32)(60.0f * s);
            mbCameraMoveMasu(nextMasu, NULL, NULL, mbCameraPlayerViewZoomGet(2), -1.0f, moveTime);
            winId = mbWinCreate(2, snpcMesTbl[3][koopaF], mbGuideSpeakerNoGet());
            mbWinPause(winId);
            mbCameraMoveWait();
            mbWinKill(winId);
            mbCameraMoveMasu(-1, NULL, NULL, mbCameraPlayerViewZoomGet(0), -1.0f, 21);
            SNpcObjDispSet(TRUE);
            mbMasuPosGet(nextMasu, &pos);
            SNpcObjRotSet(0.0f, 0.0f, 0.0f);
            maxTime = 30;
            for(i = 0; i < maxTime; i++) {
                s = (float)(maxTime - i - 1) / maxTime;
                s = HuSin(80.0f * s);
                SNpcObjPosSet(pos.x, pos.y + 100.0f * (8.0f * s), pos.z);
                if(i + 5 == maxTime) {
                    SNpcSePlay(7, 0);
                    SNpcObjMotShiftSet(5);
                }
                HuPrcVSleep();
            }
            if(snpcSaveWork->koopaF == TRUE) {
                mbCameraShakeSet(18, 200.0f);
            }
            SNpcObjPosSetV(&pos);
            SNpcMasuSet(nextMasu, TRUE);
            SNpcObjMotEndWait();
            winId = mbWinCreate(2, snpcMesTbl[4][koopaF], snpcMesSpeakerTbl[koopaF]);
            mbWinPlayerDisable(winId, -1);
            SNpcSePlay(1, 0);
            SNpcObjMotShiftSet(8);
            mbWinWait(winId);
            mbCameraMoveWait();
            SNpcObjMotEndWait();
            mbWipeDissolveFadeOut();
            mbCameraStackPop(0);
            mbMasuPosGet(masuId, &pos);
            mbPlayerPosSetV(playerNo, &pos);
            mbPlayerRotSet(playerNo, 0.0f, 0.0f, 0.0f);
            SNpcPosFixSnap();
        } else {
            mbWipeDissolveFadeOut();
            mbCameraStackPop(0);
            SNpcObjPosGet(&dir);
            mbPlayerPosGet(playerNo, &pos);
            SNpcObjPosSetV(&pos);
            mbPlayerPosSetV(playerNo, &dir);
            mbPlayerRotGet(playerNo, &rot);
            SNpcObjRotSet(0.0f, rot.y, 0.0f);
        }
        SNpcObjMotSet(0);
        mbPlayerMotionSet(playerNo, 1, 0x40000001);
        SNpcMasuEffDispSet();
        mbMusBoardFadeOut(0, 0, 1000, 1000, -1, FALSE);
        mbWipeDissolveFadeIn();
        mbMoveNumDispSet(playerNo, TRUE);
        if(!contF) {
            SNpcPosFixCreate();
        }
    }
}

/* Every player in a -1 terminated list plays a motion, with a voice when seNo is set. */
#define SNPC_LIST_MOT(list, motNo, setup) \
    { \
        u32 attr = 0; \
        int seNo = -1; \
        int k; \
        setup; \
        for(k = 0; list[k] >= 0; k++) { \
            if(seNo >= 0) { \
                mbPlayerWinLoseVoicePlay(list[k], motNo, seNo); \
            } \
            mbPlayerMotionShiftSet(list[k], motNo, 0.0f, 8.0f, attr); \
        } \
    }

#define SNPC_LIST_MOT_WAIT(list) \
    { \
        int k; \
        for(k = 0; list[k] >= 0; k++) { \
            mbPlayerMotionEndWait(list[k]); \
        } \
    }

static int SNpcMoveExec(void)
{
    int tbl[3] = { 1, 2, -1 };
    HuVecF pos;
    HuVecF starPos;
    s8 payList[5];
    s8 freeList[5];
    s16 coinTbl[4];
    s16 starTbl[4];
    int pathNo;
    int value;
    int masuId;
    int nextMasu;
    int diceNum;
    int jingle = -1;
    int hitNum;
    int humanNum;
    int comPlayer;
    s8 hitTbl[4];
    int team0;
    int team1;
    int strm;
    int i;
    int koopaF;
    int winId;
    int j;
    int payNum;
    int freeNum;
    float speed;

    if(GwSystem.turnPlayerNo >= 3) {
        koopaF = snpcSaveWork->koopaF;
        mbWipeSpecialFadeInCreate(5, 1);
        masuId = snpcSaveWork->masuId;
        mbMasuPosGet(masuId, &pos);
        SNpcObjPosSetV(&pos);
        SNpcObjRotSet(0.0f, 0.0f, 0.0f);
        mbCapMasuDispSet(FALSE);
        mbMasuPlayerDispSet(FALSE);
        mbCameraFocusObjSet(snpcWork->objId);
        mbCameraOffsetSet(0.0f, 150.0f, 0.0f);
        mbCameraZoomSet(mbCameraPlayerViewZoomGet(0));
        mbCameraMoveOnSet(FALSE);
        mbWipeSpecialFadeOutCreate(5, 42);
        mbCameraMoveOnSet(TRUE);
        strm = snpcMoveStrmTbl[koopaF];
        mbMusPlay(0, strm, 127, 0);
        mbTelopCreate(-1, koopaF + 14, TRUE);
        diceNum = 1;
        if(mbRandMod(100) < snpcEffectChanceTbl[snpcSaveWork->effectNum]) {
            diceNum++;
            snpcSaveWork->effectNum = 0;
        } else {
            snpcSaveWork->effectNum++;
            if(snpcSaveWork->effectNum >= 4) {
                snpcSaveWork->effectNum = 3;
            }
        }
        if(GwSystem.turnNo < 2) {
            diceNum = 1;
        }
        if(diceNum > 1) {
            SNpcEffectExec();
        }
        if(koopaF) {
            speed = 0.4f + 0.6f * frandf();
        } else {
            speed = 0.2f + 0.6 * frandf();
            if(diceNum > 1) {
                speed = 0.4f + 0.5f * frandf();
            }
            if(mbRandMod(100) < 10) {
                speed = 0.6f + 0.4f * frandf();
            }
        }
        value = SNpcDiceValueGet(snpcSaveWork->masuId, diceNum * 10, tbl, snpcWork->level, snpcWork->path, speed);
        snpcWork->diceNumObj = SNpcDiceExec(value, diceNum);
        SNpcZoomSet(mbCameraPlayerViewZoomGet(2));
        SNpcMasuReset(TRUE);
        for(pathNo = -1; snpcWork->path[pathNo + 1] > 0; masuId = nextMasu) {
            pathNo = MasuNextGet(masuId, pathNo);
            nextMasu = snpcWork->path[pathNo];
            mbev_PlayerColCircleAdd(-1, nextMasu, FALSE, 175.0f);
            for(i = 0, j = 0; i < 4; i++) {
                coinTbl[i] = mbPlayerCoinGet(i);
                starTbl[i] = mbPlayerStarGet(i);
                hitTbl[i] = FALSE;
                if(nextMasu == GwPlayer[i].masuId) {
                    hitTbl[i] = TRUE;
                }
            }
            if(GwSystem.tagF) {
                for(j = 0; j < 2; j++) {
                    team0 = mbPlayerTeamFindPlayer(j, 0);
                    team1 = mbPlayerTeamFindPlayer(j, 1);
                    if(hitTbl[team0] && hitTbl[team1]) {
                        starTbl[team0] = coinTbl[team1] = 0;
                        if(coinTbl[team0] > 20) {
                            coinTbl[team1] = coinTbl[team0] - 20;
                            coinTbl[team0] = 20;
                        }
                        if(starTbl[team1] > 1) {
                            starTbl[team0] = starTbl[team1] - 1;
                            starTbl[team1] = 1;
                        }
                    }
                }
            }
            payNum = humanNum = hitNum = freeNum = 0;
            comPlayer = 0;
            for(i = 0; i < 4; i++) {
                if(hitTbl[i]) {
                    if(GwPlayer[i].comF) {
                        comPlayer = i;
                    } else {
                        humanNum++;
                    }
                    if(!koopaF) {
                        if(coinTbl[i] >= 20) {
                            payList[payNum++] = i;
                            hitNum++;
                        } else {
                            freeList[freeNum++] = i;
                        }
                    } else {
                        if(starTbl[i] > 0) {
                            payList[payNum++] = i;
                        } else {
                            freeList[freeNum++] = i;
                        }
                        hitNum++;
                    }
                }
            }
            payList[payNum] = -1;
            freeList[freeNum] = -1;
            snpcWork->moveObj = omAddObjEx(mbObjMan, 256, 0, 0, -1, SNpcMoveOMExec);
            ((SNPCMOVEWORK *)snpcWork->moveObj->work)->masuId = nextMasu;
            while(snpcWork->moveObj) {
                HuPrcVSleep();
            }
            mbev_PlayerColMasuAdd(-1, masuId, FALSE);
            if(mbMasuDispCheck(nextMasu)) {
                value--;
            }
            mbDiceSNpcNumSet(snpcWork->diceNumObj, value);
            if(humanNum) {
                comPlayer = -1;
            }
            if(payNum + freeNum > 0) {
                mbDiceSNpcNumDispSet(snpcWork->diceNumObj, FALSE);
                SNpcZoomSet(mbCameraPlayerViewZoomGet(0));
                SNpcTargetAngleSet(0.0f);
                SNpcRotateWait();
                while(!mbPlayerColCheck()) {
                    HuPrcVSleep();
                }
                winId = mbWinCreate(2, snpcStarMesTbl[1][koopaF], snpcStarMesSpeakerTbl[koopaF]);
                mbWinPlayerDisable(winId, comPlayer);
                if(koopaF == FALSE) {
                    SNPC_LIST_MOT(freeList, 12, seNo = 579);
                    SNPC_LIST_MOT(payList, 12, seNo = 579);
                } else {
                    SNPC_LIST_MOT(freeList, 13, seNo = 585);
                    SNPC_LIST_MOT(payList, 13, seNo = 585);
                    SNpcObjMotShiftSet(8);
                }
                SNpcSePlay(1, 0);
                SNPC_LIST_MOT_WAIT(freeList);
                SNPC_LIST_MOT_WAIT(payList);
                SNPC_LIST_MOT(freeList, 1, attr = 0x40000001);
                SNPC_LIST_MOT(payList, 1, attr = 0x40000001);
                if(koopaF == TRUE) {
                    SNpcObjMotEndWait();
                    SNpcObjMotShiftSet(0);
                }
                mbWinWait(winId);
                if(hitNum) {
                    if(koopaF == TRUE && payNum == 0) {
                        winId = mbWinCreate(2, snpcStarMesTbl[4][koopaF], snpcStarMesSpeakerTbl[koopaF]);
                    } else {
                        winId = mbWinCreate(2, snpcStarMesTbl[0][koopaF], snpcStarMesSpeakerTbl[koopaF]);
                    }
                    mbWinPlayerDisable(winId, comPlayer);
                    mbWinWait(winId);
                    SNpcObjMotShiftSet(7);
                    mbMasuPosGet(nextMasu, &starPos);
                    SNpcStarCreate(koopaF, FALSE, &starPos);
                    mbMusPauseFadeOut(0, TRUE, 1000);
                    SNpcStarWait();
                    SNpcObjMotEndWait();
                    SNpcObjMotShiftSet(0);
                    if(koopaF == FALSE) {
                        for(j = 0; payList[j] >= 0; j++) {
                            mbCoinAddDispExec(payList[j], -20, FALSE, TRUE);
                        }
                        jingle = mbMusJinglePlay(39);
                        StarChangeExec(payList, FALSE, 1);
                        mbMusJingleWait(jingle);
                    } else if(payNum) {
                        StarChangeExec(payList, FALSE, -1);
                        SNPC_LIST_MOT(freeList, 12, seNo = 579);
                        SNPC_LIST_MOT_WAIT(freeList);
                    } else {
                        StarChangeExec(freeList, TRUE, -20);
                    }
                    mbMusPauseFadeOut(0, FALSE, 1000);
                    HuPrcSleep(12);
                    if(payNum && freeNum) {
                        winId = mbWinCreate(2, snpcStarMesTbl[2][koopaF], snpcStarMesSpeakerTbl[koopaF]);
                        mbWinPlayerDisable(winId, comPlayer);
                        if(koopaF == FALSE) {
                            SNPC_LIST_MOT(payList, 1, attr = 0x40000001);
                            SNpcSePlay(2, 0);
                            SNpcObjMotShiftSet(9);
                            mbWinWait(winId);
                            SNPC_LIST_MOT(freeList, 13, seNo = 585);
                            SNPC_LIST_MOT_WAIT(freeList);
                            SNpcObjMotEndWait();
                        } else {
                            mbWinWait(winId);
                            StarChangeExec(freeList, TRUE, -20);
                        }
                    }
                } else {
                    winId = mbWinCreate(2, snpcStarMesTbl[4][koopaF], snpcStarMesSpeakerTbl[koopaF]);
                    mbWinPlayerDisable(winId, comPlayer);
                    SNpcSePlay(2, 0);
                    SNpcObjMotShiftSet(9);
                    mbWinWait(winId);
                    SNPC_LIST_MOT(freeList, 13, seNo = 585);
                    SNPC_LIST_MOT_WAIT(freeList);
                    SNpcObjMotEndWait();
                }
                mbDiceSNpcNumDispSet(snpcWork->diceNumObj, TRUE);
                SNpcZoomSet(mbCameraPlayerViewZoomGet(2));
            }
        }
        mbDiceSNpcNumKill(snpcWork->diceNumObj);
        snpcWork->diceNumObj = NULL;
        SNpcMasuSet(snpcWork->path[pathNo], TRUE);
        SNpcZoomSet(mbCameraPlayerViewZoomGet(0));
        SNpcTargetAngleSet(0.0f);
        SNpcRotateWait();
        winId = mbWinCreate(2, snpcStarMesTbl[3][koopaF], snpcStarMesSpeakerTbl[koopaF]);
        mbWinPlayerDisable(winId, -1);
        SNpcSePlay(1, 0);
        SNpcObjMotShiftSet(8);
        mbWinWait(winId);
        SNpcObjMotEndWait();
        SNpcZoomWait();
        if(GwSystem.turnPlayerNo < 3 || _CheckFlag(0x20000)) {
            mbCapMasuDispSet(TRUE);
            mbMasuPlayerDispSet(TRUE);
        }
        SNpcMasuEffDispSet();
        mbMusBoardFadeOut(0, 0, 1000, 1000, -1, FALSE);
        return FALSE;
    }
}

static void StarChangeExec(s8 *list, BOOL coinF, int num)
{
    int numTbl[4];
    int dispTbl[4];
    int playerNo;
    int i;
    int motIdx;

    motIdx = 0;
    if(num < 0) {
        motIdx++;
    }
    for(i = 0; list[i] >= 0; i++) {
        mbPlayerWinLoseVoicePlay(list[i], playerStarChgMotNoTbl[motIdx], playerStarChgSeTbl[motIdx]);
        mbPlayerMotionShiftSet(list[i], playerStarChgMotNoTbl[motIdx], 0.0f, 8.0f, 0);
        omVibrate(list[i], 20, 7, 3);
    }
    for(i = 0; i < 4; i++) {
        numTbl[i] = 0;
        dispTbl[i] = 0;
    }
    for(i = 0; list[i] >= 0; i++) {
        playerNo = list[i];
        numTbl[playerNo] = num;
        dispTbl[playerNo] = num;
    }
    if(!coinF) {
        mbStarAddAllProcExecV(numTbl, dispTbl, TRUE);
    } else {
        mbCoinAddAllProcExecV(numTbl, dispTbl, TRUE);
    }
    for(i = 0; list[i] >= 0; i++) {
        mbPlayerMotionEndWait(list[i]);
    }
    HuPrcSleep(20);
}

static int MasuNextGet(int masuId, int pathNo)
{
    int tbl[3] = { 1, 2, -1 };
    int nextNo = pathNo + 1;
    BOOL jumpF = FALSE;

    if((mbMasuAttrGet(masuId) & 0x2) && (mbMasuAttrGet(snpcWork->path[pathNo + 1]) & 0x1)) {
        jumpF = TRUE;
        nextNo = pathNo + 1;
    } else if((mbMasuAttrGet(masuId) & 0x8) && (mbMasuAttrGet(snpcWork->path[pathNo + 1]) & 0x4)) {
        jumpF = TRUE;
        nextNo = pathNo + 2;
    } else if((mbMasuAttrGet(snpcWork->path[pathNo + 1]) & 0x8) && (mbMasuAttrGet(snpcWork->path[pathNo + 2]) & 0x4)) {
        jumpF = TRUE;
        nextNo = pathNo + 3;
    }
    snpcWork->jumpF = FALSE;
    if(jumpF) {
        snpcWork->jumpF = TRUE;
    }
    return nextNo;
}

static void SNpcMoveOMExec(OMOBJ *obj)
{
    SNPCMOVEWORK *work = (SNPCMOVEWORK *)obj->work;
    BOOL endF = FALSE;
    HuVecF pos;
    HuVecF dir;
    float t;
    float rotT;

    if(mbExitCheck() || work->killF) {
        omDelObjEx(HuPrcCurrentGet(), obj);
        snpcWork->moveObj = NULL;
        return;
    }
    if(!work->initF) {
        work->initF = TRUE;
        work->rotTime = 0;
        work->time = 0;
        work->mode = 0;
        SNpcObjPosGet(&obj->trans);
        SNpcObjRotGet(&obj->rot);
        mbMasuPosGet(work->masuId, &obj->scale);
        PSVECSubtract(&obj->scale, &obj->trans, &dir);
        t = PSVECMag(&dir);
        if(snpcSaveWork->koopaF == TRUE) {
            t *= 1.6666666f;
        }
        work->maxTime = t / 16.666666f;
        obj->rot.z = HuAtan(dir.x, dir.z);
        if(snpcWork->jumpF) {
            work->jumpF = 1;
            SNpcObjMotShiftSet(3);
        } else {
            SNpcObjMotShiftSet(1);
        }
    }
    work->time++;
    work->rotTime++;
    t = (float)work->time / work->maxTime;
    pos.x = obj->trans.x + t * (obj->scale.x - obj->trans.x);
    pos.y = obj->trans.y + t * (obj->scale.y - obj->trans.y);
    pos.z = obj->trans.z + t * (obj->scale.z - obj->trans.z);
    rotT = 0.16666667f * work->rotTime;
    if(rotT > 1.0f) {
        rotT = 1.0f;
    }
    obj->rot.y = mbAngleLerp(obj->rot.y, obj->rot.z, 0.15f * rotT);
    SNpcObjRotSet(0.0f, obj->rot.y, 0.0f);
    if(work->jumpF == 0) {
        SNpcObjPosSetV(&pos);
        if(work->time >= work->maxTime) {
            endF = TRUE;
        }
    } else {
        switch(work->mode) {
            case 0:
                if(SNpcObjMotEndCheck()) {
                    SNpcObjMotShiftSet(4);
                    work->time = 0;
                    work->maxTime = 35;
                    work->mode++;
                }
                break;

            case 1:
                if(work->time >= work->maxTime - 5) {
                    SNpcObjMotShiftSet(5);
                    work->mode++;
                }
            case 2:
                pos.y += 100.0 * (4.0 * HuSin(180.0f * t));
                SNpcObjPosSetV(&pos);
                if(work->time >= work->maxTime) {
                    if(snpcSaveWork->koopaF == TRUE) {
                        mbCameraShakeSet(12, 100.0f);
                    }
                    work->mode++;
                }
                break;

            case 3:
                if(SNpcObjMotEndCheck()) {
                    endF = TRUE;
                }
                break;
        }
    }
    if(snpcWork->diceNumObj) {
        SNpcObjPosGet(&pos);
        mbDiceSNpcNumPosSet(snpcWork->diceNumObj, &pos);
    }
    SNpcMotSetNext();
    if(endF) {
        omDelObjEx(HuPrcCurrentGet(), obj);
        snpcWork->moveObj = NULL;
        return;
    }
}

static void SNpcTargetAngleSet(float angle)
{
    snpcWork->moveObj = omAddObjEx(mbObjMan, 256, 0, 0, -1, SNpcRotateUpdate);
    ((SNPCTIMEWORK *)snpcWork->moveObj->work)->start = angle;
}

static inline BOOL SNpcRotateCheck(void)
{
    if(snpcWork->moveObj) {
        return FALSE;
    }
    if(mbObjMotionShiftIDGet(snpcWork->objId) != -1) {
        return FALSE;
    }
    return TRUE;
}

static void SNpcRotateWait(void)
{
    while(!SNpcRotateCheck()) {
        HuPrcVSleep();
    }
}

static inline float SNpcAbsF(register float x)
{
    // clang-format off
    asm {
        fabs x, x
    }
    // clang-format on
    return x;
}

static void SNpcRotateUpdate(OMOBJ *obj)
{
    SNPCTIMEWORK *work = (SNPCTIMEWORK *)obj->work;
    BOOL endF = FALSE;
    float t;
    float s;
    float angle;

    if(mbExitCheck() || work->killF) {
        omDelObjEx(HuPrcCurrentGet(), obj);
        snpcWork->moveObj = NULL;
        return;
    }
    if(!work->initF) {
        work->initF = TRUE;
        work->time = 0;
        work->mode = 0;
        SNpcObjRotGet(&obj->rot);
        obj->scale.y = mbAngleWrap2(work->start, obj->rot.y);
        t = (1.0f / 180.0f) * SNpcAbsF(obj->scale.y);
        work->maxTime = 15.0f * (0.7f + t * snpcRotSpeedTbl[snpcSaveWork->koopaF]);
        if(__abs((int)obj->scale.y) < 5.0f) {
            SNpcObjMotShiftSet(0);
        } else {
            SNpcObjMotShiftSet(1);
        }
    }
    t = (float)work->time++ / work->maxTime;
    s = 0.5 + 0.5 * HuSin(270.0f + 180.0f * t);
    angle = obj->rot.y + s * obj->scale.y;
    SNpcObjRotSet(obj->rot.x, angle, obj->rot.z);
    if(work->time >= work->maxTime) {
        endF = TRUE;
        SNpcObjMotShiftSet(0);
    }
    SNpcMotSetNext();
    if(endF) {
        omDelObjEx(HuPrcCurrentGet(), obj);
        snpcWork->moveObj = NULL;
        return;
    }
}

static void SNpcPosFixCreate(void)
{
    snpcWork->moveObj = omAddObjEx(mbObjMan, 256, 0, 0, -1, SNpcPosFixUpdate);
}

static void SNpcPosFixUpdate(OMOBJ *obj)
{
    SNPCTIMEWORK *work = (SNPCTIMEWORK *)obj->work;
    BOOL endF = FALSE;
    HuVecF pos;
    HuVecF endPos;
    HuVecF rot;
    float t;
    float s;
    float angle;

    if(mbExitCheck() || work->killF) {
        omDelObjEx(HuPrcCurrentGet(), obj);
        snpcWork->moveObj = NULL;
        return;
    }
    if(!work->initF) {
        work->initF = TRUE;
        work->mode = 0;
        SNpcObjPosGet(&pos);
        SNpcObjRotGet(&rot);
        SNpcPosFixSnap();
        SNpcObjPosGet(&endPos);
        SNpcObjPosSetV(&pos);
        SNpcObjRotSetV(&rot);
        obj->trans.x = pos.x;
        obj->trans.y = pos.y;
        obj->trans.z = pos.z;
        obj->scale.x = endPos.x;
        obj->scale.y = endPos.y;
        obj->scale.z = endPos.z;
        work->maxTime = 30;
        work->time = 0;
        SNpcObjMotShiftSet(1);
    }
    if(work->mode == 0) {
        work->time++;
        t = (float)work->time / work->maxTime;
        pos.x = obj->trans.x + t * (obj->scale.x - obj->trans.x);
        pos.y = obj->trans.y + t * (obj->scale.y - obj->trans.y);
        pos.z = obj->trans.z + t * (obj->scale.z - obj->trans.z);
        SNpcObjPosSetV(&pos);
        if(work->time >= work->maxTime) {
            work->mode++;
            SNpcObjRotGet(&obj->rot);
            work->start = 0.0f;
            obj->scale.y = mbAngleWrap2(work->start, obj->rot.y);
            t = (1.0f / 180.0f) * SNpcAbsF(obj->scale.y);
            work->maxTime = 15.0f * (0.7f + t * snpcPosFixSpeedTbl[snpcSaveWork->koopaF]);
            work->time = 0;
            if(__abs((int)obj->scale.y) < 5.0f) {
                SNpcObjMotShiftSet(0);
            } else {
                SNpcObjMotShiftSet(1);
            }
        }
    } else {
        t = (float)work->time++ / work->maxTime;
        s = 0.5 + 0.5 * HuSin(270.0f + 180.0f * t);
        angle = obj->rot.y + s * obj->scale.y;
        SNpcObjRotSet(obj->rot.x, angle, obj->rot.z);
        if(work->time >= work->maxTime) {
            endF = TRUE;
            SNpcObjMotShiftSet(0);
        }
    }
    SNpcMotSetNext();
    if(endF) {
        work->killF = TRUE;
    }
}

static void SNpcZoomSet(float zoom)
{
    if(!SNpcZoomCheck()) {
        snpcWork->zoomObj = omAddObjEx(mbObjMan, 256, 0, 0, -1, SNpcZoomUpdate);
    }
    ((SNPCTIMEWORK *)snpcWork->zoomObj->work)->initF = FALSE;
    ((SNPCTIMEWORK *)snpcWork->zoomObj->work)->killF = FALSE;
    ((SNPCTIMEWORK *)snpcWork->zoomObj->work)->end = zoom;
}

static BOOL SNpcZoomCheck(void)
{
    return snpcWork->zoomObj != NULL;
}

static void SNpcZoomWait(void)
{
    while(snpcWork->zoomObj != NULL) {
        HuPrcVSleep();
    }
}

static void SNpcZoomUpdate(OMOBJ *obj)
{
    SNPCTIMEWORK *work = (SNPCTIMEWORK *)obj->work;
    BOOL endF = FALSE;
    float t;

    if(mbExitCheck() || work->killF) {
        omDelObjEx(HuPrcCurrentGet(), obj);
        snpcWork->zoomObj = NULL;
        return;
    }
    if(!work->initF) {
        work->initF = TRUE;
        work->start = mbCameraZoomGet();
        work->time = 0;
        work->maxTime = 21;
    }
    work->time++;
    t = (float)work->time / work->maxTime;
    if(t >= 1.0f) {
        t = 1.0f;
        work->killF = TRUE;
    }
    mbCameraZoomSet(work->start + t * (work->end - work->start));
}

static OMOBJ *SNpcDiceExec(int value, int diceNum)
{
    HuVecF pos;
    int tbl[3] = { 0, 0, 0 };
    int koopaF;
    int i;

    koopaF = snpcSaveWork->koopaF;
    SNpcObjPosGet(&pos);
    PSVECAdd(&pos, &snpcDiceOfsTbl[koopaF], &pos);
    if(diceNum > 2) {
        diceNum = 2;
    }
    tbl[0] = value;
    if(diceNum == 2 && value > 1) {
        i = value >> 1;
        tbl[0] = i;
        tbl[1] = value - i;
        i = 10 - tbl[1];
        if(i > tbl[0] - 1) {
            i = tbl[0] - 1;
        }
        i = mbRandMod(i + 1);
        tbl[0] -= i;
        tbl[1] += i;
        if(mbRandMod(100) < 80) {
            i = tbl[0];
            tbl[0] = tbl[1];
            tbl[1] = i;
        }
    }
    for(i = 0; i < 3; i++) {
        tbl[i]--;
    }
    mbDiceProcExec(-1, snpcDiceTypeTbl[diceNum - 1][koopaF], NULL, tbl, FALSE, FALSE, &pos, snpcMoveNumColor[koopaF]);
    mbDicePadBtnHookSet(-1, SNpcDiceBtnHook);
    mbDiceMotHookSet(-1, SNpcDiceMotHook);
    while(!mbDiceKillCheck(-1)) {
        HuPrcVSleep();
    }
    SNpcObjPosGet(&pos);
    return mbDiceSNpcNumCreate(-1, &pos);
}

static u16 SNpcDiceBtnHook(int playerNo)
{
    return PAD_BUTTON_A;
}

static void SNpcDiceMotHook(int playerNo)
{
    int time;

    SNpcObjMotSet(6);
    time = 0;
    do {
        if(time++ == snpcDiceMotTimeTbl[snpcSaveWork->koopaF]) {
            mbDiceObjHit(-1);
        }
        HuPrcVSleep();
    } while(!SNpcObjMotEndCheck());
    SNpcObjMotShiftSet(0);
}

static void SNpcMasuSet(int masuId, BOOL seF)
{
    int typeTbl[2] = { 7, 10 };
    int seTbl[2] = { 1527, 1528 };

    snpcSaveWork->masuId = masuId;
    snpcWork->masuType = mbMasuTypeGet(masuId);
    mbMasuTypeSet(masuId, typeTbl[snpcSaveWork->koopaF]);
    mbMasuCapsuleSet(masuId, -1);
    if(seF) {
        mbAudFXPlay(seTbl[snpcSaveWork->koopaF]);
        SNpcObjMasuSet(masuId);
    }
}

static void SNpcMasuReset(BOOL seF)
{
    mbMasuTypeSet(snpcSaveWork->masuId, snpcWork->masuType);
    if(seF) {
        mbAudFXPlay(1529);
        SNpcObjMasuSet(snpcSaveWork->masuId);
    }
    snpcSaveWork->masuId = 0;
}

static int SNpcMasuStarNextGet(BOOL nearF)
{
    int tbl[3] = { 1, 2, -1 };
    int num;
    int masuId;

    num = 10;
    if(nearF) {
        num = 4;
    }
    masuId = mbSNpcMasuStarNextGet(snpcSaveWork->masuId, num, tbl, snpcWork->level);
    return masuId;
}

static void SNpcPosFixSnap(void)
{
    Mtx mtx;
    HuVecF pos;
    HuVecF dir;
    int masuId;
    int i;
    int linkId;

    if(snpcMagic == SNPC_MAGIC) {
        masuId = snpcSaveWork->masuId;
        for(i = 0; i < mbMasuLinkNumGet(masuId); i++) {
            linkId = mbMasuLinkGet(masuId, i);
            if(!(mbMasuMAttrGet(linkId) & mbBranchMAttrGet())) {
                break;
            }
        }
        mbMasuPosGet(masuId, &pos);
        mbMasuPosGet(linkId, &dir);
        PSVECSubtract(&dir, &pos, &dir);
        dir.y = 0.0f;
        PSVECNormalize(&dir, &dir);
        PSVECScale(&dir, &dir, 130.0f);
        mbMasuMtxGet(masuId, mtx);
        PSMTXMultVec(mtx, &dir, &pos);
        SNpcObjPosSetV(&pos);
        SNpcObjRotSet(0.0f, 0.0f, 0.0f);
    }
}

/* @@PART2@@ */
#pragma dont_inline on
static void SNpcEffectExec(void) { }
static void SNpcPlayerMoveFunc(int playerNo) { }
static void SNpcSePlay(int seNo, int arg1) { }
static void SNpcMotSetNext(void) { }
static void SNpcObjCreate(void) { }
static void SNpcObjKill(void) { }
static void SNpcObjDispSet(BOOL dispF) { }
static void SNpcObjPosSet(float x, float y, float z) { }
static void SNpcObjPosSetV(HuVecF *pos) { }
static void SNpcObjPosGet(HuVecF *pos) { }
static void SNpcObjRotSet(float x, float y, float z) { }
static void SNpcObjRotSetV(HuVecF *rot) { }
static void SNpcObjRotGet(HuVecF *rot) { }
static void SNpcObjMotSet(int motNo) { }
static void SNpcObjMotShiftSet(int motNo) { }
static BOOL SNpcObjMotEndCheck(void) { return 0; }
static void SNpcObjMotEndWait(void) { }
static void SNpcObjMasuSet(int masuId) { }
static void SNpcMasuEffDispSet(void) { }
static int SNpcDiceValueGet(int masuId, int num, int *tbl, int level, s16 *path, float speed) { return 0; }
int mbSNpcMasuStarNextGet(int masuId, int num, int *tbl, int level) { return 0; }
static void SNpcStarCreate(int koopaF, BOOL coinF, HuVecF *pos) { }
static void SNpcStarWait(void) { }
#pragma dont_inline reset
/* @@PART3@@ */
/* Counts the TEV stages and texture coordinates Hu3DTevStageTexSet used, so a material hook can
 * append its own stages after them. */
static void GetStarTexTevStage(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *matP, u32 *tevStageP, u32 *texCoordP)
{
    s32 bumpTexMap;
    BOOL texBlendF;
    u32 flags;
    u32 shineF;
    u16 lightOnF;
    s16 texMapId;
    u16 tevStage;
    u16 texCoordId;
    u16 i;
    HSF_ATTRIBUTE *attrP;
    HU3D_MODEL *modelP;
    HSF_OBJECT *objPtr;
    HU3D_ATTR_ANIM *animWorkP;
    u16 j;
    u16 matHiliteF;
    HSF_BITMAP *bitMapPtr;
    HSF_CONSTDATA *constData;

    texMapId = -1;
    objPtr = drawObj->object;
    modelP = drawObj->model;
    for(i = 0; i < matP->attrNum; i++) {
        attrP = &objPtr->mesh.attribute[matP->attr[i]];
        bitMapPtr = attrP->bitmap;
        texCol[i].a = 0;
        if(attrP->animWorkP) {
            animWorkP = attrP->animWorkP;
            if((animWorkP->attr & HU3D_ATTRANIM_ATTR_ANIM2D) && !(Hu3DTexAnimData[animWorkP->animId].attr & HU3D_ANIM_ATTR_NOUSE)) {
            } else if(animWorkP->attr & HU3D_ATTRANIM_ATTR_BMPANIM) {
                bitMapPtr = animWorkP->bitMapPtr;
                switch(bitMapPtr->dataFmt) {
                    case HSF_BMPFMT_I4:
                    case HSF_BMPFMT_I8:
                    case HSF_BMPFMT_IA4:
                    case HSF_BMPFMT_IA8:
                        texCol[i].a = 1;
                        break;

                    case HSF_BMPFMT_CI_IA8:
                        texCol[i].a = 2;
                        break;
                }
            }
        } else {
            switch(bitMapPtr->dataFmt) {
                case HSF_BMPFMT_I4:
                case HSF_BMPFMT_I8:
                case HSF_BMPFMT_IA4:
                case HSF_BMPFMT_IA8:
                    texCol[i].a = 1;
                    break;

                case HSF_BMPFMT_CI_IA8:
                    texCol[i].a = 2;
                    break;
            }
        }
    }
    flags = objPtr->flags | matP->flags;
    if(matP->vtxMode == 2 || matP->vtxMode == 3) {
        matHiliteF = TRUE;
    } else {
        matHiliteF = FALSE;
        if(matP->vtxMode == 0 || matP->vtxMode == 5) {
            lightOnF = GX_FALSE;
        } else {
            lightOnF = GX_TRUE;
        }
    }
    if(Hu3DShineF && lightOnF) {
        shineF = TRUE;
    } else {
        shineF = FALSE;
    }
    if(matP->attrNum == 1) {
        texCoordId = tevStage = 1;
        attrP = &objPtr->mesh.attribute[matP->attr[0]];
        if(attrP->unk20 == 1.0f) {
            if(attrP->unk8[2] == 0) {
                tevStage++;
            } else if(!(modelP->attr & HU3D_ATTR_TOON_MAP)) {
                if(texCol[0].a == 1) {
                    tevStage++;
                } else if(texCol[0].a == 2) {
                    tevStage++;
                }
            }
        }
        if(modelP->attr & HU3D_ATTR_TOON_MAP) {
            texCoordId++;
            tevStage++;
        }
        if(matP->refAlpha != 0.0f) {
            texCoordId++;
            tevStage++;
        }
        if(shineF) {
            tevStage++;
        }
        constData = drawObj->object->constData;
        if(Hu3DShadowF != 0 && shadowNum != 0 && (constData->attr & HU3D_CONST_SHADOW_MAP)) {
            HSF_CONSTDATA *shadowData = objPtr->constData;
            if(shadowData->attr & HU3D_CONST_SHADOW_MAP_TPLVL) {
                tevStage++;
            }
            tevStage++;
            texCoordId++;
        }
        if(matHiliteF) {
            if((modelP->attr & HU3D_ATTR_HILITE) || (flags & HSF_MATERIAL_HILITE)) {
                tevStage++;
                texCoordId++;
                matHiliteF = FALSE;
            } else {
                if(attrP->unk20 != 1.0f) {
                    texCoordId++;
                }
                tevStage++;
            }
        } else if(matP->invAlpha != 0.0f) {
            tevStage++;
        }
        if(modelP->projBit != 0) {
            for(i = 0, j = 1; i < HU3D_PROJ_MAX; i++, j <<= 1) {
                if(j & modelP->projBit) {
                    texCoordId++;
                    tevStage += 2;
                }
            }
        }
    } else {
        texBlendF = FALSE;
        texCoordId = 0;
        bumpTexMap = -1;
        for(i = tevStage = 0; i < matP->attrNum; i++) {
            attrP = &objPtr->mesh.attribute[matP->attr[i]];
            if(attrP->nbtTpLvl != 0.0f) {
                tevStage++;
                bumpTexMap = i;
                texCoordId++;
                texBlendF = TRUE;
            } else if(attrP->unk20 != 1.0f) {
                texMapId = i;
                continue;
            } else {
                texCoordId++;
                if(i == 0) {
                    if(texCol[i].a == 1 || texCol[i].a == 2) {
                        tevStage++;
                    }
                } else if(texBlendF) {
                    texBlendF = FALSE;
                } else if(attrP->unk8[2] == 0) {
                    tevStage++;
                } else if(texCol[i].a == 1 || texCol[i].a == 2) {
                    tevStage++;
                }
            }
            tevStage++;
        }
        if(modelP->attr & HU3D_ATTR_TOON_MAP) {
            texCoordId++;
            tevStage++;
        }
        if(matP->refAlpha != 0.0f) {
            if(texMapId != -1) {
                tevStage++;
                texCoordId++;
            }
            tevStage++;
            texCoordId++;
        }
        if(shineF) {
            tevStage++;
        }
        constData = drawObj->object->constData;
        if(Hu3DShadowF != 0 && shadowNum != 0 && (constData->attr & HU3D_CONST_SHADOW_MAP)) {
            HSF_CONSTDATA *shadowData = objPtr->constData;
            if(shadowData->attr & HU3D_CONST_SHADOW_MAP_TPLVL) {
                tevStage++;
            }
            tevStage++;
            texCoordId++;
        }
        if(matHiliteF) {
            if((modelP->attr & HU3D_ATTR_HILITE) || (flags & HSF_MATERIAL_HILITE)) {
                if(texMapId != -1) {
                    tevStage++;
                    texCoordId++;
                }
                tevStage++;
                texCoordId++;
                matHiliteF = FALSE;
            } else {
                if(texMapId != -1) {
                    texCoordId++;
                }
                tevStage++;
            }
        } else if(matP->invAlpha != 0.0f) {
            tevStage++;
        }
        if(modelP->projBit != 0) {
            for(i = 0, j = 1; i < HU3D_PROJ_MAX; i++, j <<= 1) {
                if(j & modelP->projBit) {
                    texCoordId++;
                    tevStage += 2;
                }
            }
        }
        if(bumpTexMap != -1) {
            texCoordId++;
        }
    }
    *tevStageP = tevStage;
    *texCoordP = texCoordId;
}

/* The same count for Hu3DTevStageNoTexSet. */
static void GetStarNoTexTevStage(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *matP, u32 *tevStageP, u32 *texCoordP)
{
    s16 tevStage;
    s16 texMap;
    HU3D_MODEL *modelP;
    HSF_OBJECT *objPtr;
    s16 i;
    s16 j;
    s16 matHiliteF;
    HSF_CONSTDATA *constData;
    HSF_CONSTDATA *shadowData;
    u32 flags;

    tevStage = 1;
    texMap = 0;
    objPtr = drawObj->object;
    modelP = drawObj->model;
    flags = objPtr->flags | matP->flags;
    if(matP->vtxMode == 2 || matP->vtxMode == 3) {
        matHiliteF = TRUE;
    } else {
        matHiliteF = FALSE;
    }
    if(modelP->attr & HU3D_ATTR_TOON_MAP) {
        texMap++;
    }
    if(matP->refAlpha != 0.0f) {
        tevStage++;
        texMap++;
    }
    constData = drawObj->object->constData;
    if(Hu3DShadowF != 0 && shadowNum != 0 && (constData->attr & HU3D_CONST_SHADOW_MAP)) {
        shadowData = objPtr->constData;
        if(shadowData->attr & HU3D_CONST_SHADOW_MAP_TPLVL) {
            tevStage++;
        }
        tevStage++;
        texMap++;
    }
    if(matHiliteF) {
        if((modelP->attr & HU3D_ATTR_HILITE) || (flags & HSF_MATERIAL_HILITE)) {
            texMap++;
        }
        tevStage++;
    } else if(matP->invAlpha != 0.0f) {
        tevStage++;
    }
    if(modelP->projBit != 0) {
        for(i = 0, j = 1; i < HU3D_PROJ_MAX; i++, j <<= 1) {
            if(j & modelP->projBit) {
                texMap++;
                tevStage += 2;
            }
        }
    }
    *tevStageP = tevStage;
    *texCoordP = texMap;
}

void mbObjStarTevStageSet(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *matP, u32 *tevStage, u32 *texCoord)
{
    if(matP->attrNum == 0) {
        Hu3DTevStageNoTexSet(drawObj, matP);
        GetStarNoTexTevStage(drawObj, matP, tevStage, texCoord);
    } else {
        Hu3DTevStageTexSet(drawObj, matP);
        GetStarTexTevStage(drawObj, matP, tevStage, texCoord);
    }
}

static void FadeMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *matP);

void mbObjFadeCreate(s16 objId, HuVecF *pos)
{
    int mdlId;
    MBOBJFADEWORK *work;
    HU3D_MODEL *modelP;
    HSF_DATA *hsf;
    int i;

    mdlId = mbObjModelIDGet((MBOBJID)objId);
    modelP = &Hu3DData[mdlId];
    if(modelP->attr & HU3D_ATTR_LINK) {
        modelP->hookData = work = mbMallocNum(sizeof(MBOBJFADEWORK), modelP->mallocNoLink);
    } else {
        modelP->hookData = work = mbMallocNum(sizeof(MBOBJFADEWORK), modelP->mallocNo);
    }
    work->magic = MBOBJFADE_MAGIC;
    mbVecCopy(pos, &work->pos);
    work->scale = 1.0f;
    work->color.r = work->color.g = work->color.b = 255;
    Hu3DModelMatHookSet(mdlId, FadeMatHook);
    hsf = modelP->hsf;
    for(i = 0; i < hsf->materialNum; i++) {
        hsf->material[i].flags |= HSF_MATERIAL_MATHOOK;
    }
    work->anim = HuSprAnimRead(HuDataSelHeapReadNum(mbBoardDataNumGet(0x50067), HU_MEMNUM_OVL, HEAP_MODEL));
}

void mbObjFadeKill(s16 objId)
{
    int mdlId;
    MBOBJFADEWORK *work;
    HU3D_MODEL *modelP;
    HSF_DATA *hsf;
    int i;

    mdlId = mbObjModelIDGet((MBOBJID)objId);
    modelP = &Hu3DData[mdlId];
    work = modelP->hookData;
    Hu3DModelMatHookSet(mdlId, NULL);
    hsf = modelP->hsf;
    for(i = 0; i < hsf->materialNum; i++) {
        hsf->material[i].flags &= ~HSF_MATERIAL_MATHOOK;
    }
    HuSprAnimKill(work->anim);
    HuMemDirectFree(work);
    modelP->hookData = NULL;
}

static void FadeMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *matP)
{
    Mtx mtx;
    Mtx mtxTemp;
    u32 tevStage;
    u32 texCoord;
    MBOBJFADEWORK *work;
    float scale;

    work = drawObj->model->hookData;
    mbObjStarTevStageSet(drawObj, matP, &tevStage, &texCoord);
    HuSprTexLoad(work->anim, 0, GX_TEXMAP4, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    PSMTXInverse(Hu3DCameraMtx, mtx);
    PSMTXConcat(mtx, drawObj->matrix, mtx);
    PSMTXTrans(mtxTemp, -work->pos.x, -work->pos.y, -work->pos.z);
    PSMTXConcat(mtxTemp, mtx, mtx);
    mbMtxRot(mtxTemp, work->rot.x, work->rot.y, work->rot.z);
    PSMTXInverse(mtxTemp, mtxTemp);
    PSMTXConcat(mtxTemp, mtx, mtx);
    scale = work->scale;
    if(scale < 0.000001f) {
        scale = 0.000001f;
    }
    PSMTXScale(mtxTemp, 0.001f, -0.01f / scale, 1.0f);
    PSMTXConcat(mtxTemp, mtx, mtx);
    mtx[1][3] += 0.96875f;
    GXLoadTexMtxImm(mtx, GX_TEXMTX4, GX_MTX2x4);
    GXSetNumTexGens(texCoord + 1);
    GXSetNumTevStages(tevStage + 1);
    GXSetTexCoordGen(texCoord, GX_TG_MTX2x4, GX_TG_POS, GX_TEXMTX4);
    GXSetTevOrder(tevStage, texCoord, GX_TEXMAP4, GX_COLOR_NULL);
    GXSetTevKColor(GX_KCOLOR3, work->color);
    GXSetTevKColorSel(tevStage, GX_TEV_KCSEL_K3);
    GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, GX_CC_CPREV);
    GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_TEXA, GX_CA_APREV, GX_CA_ZERO);
    GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    if((drawObj->object->flags | matP->flags) & 0x1200) {
        GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_GEQUAL, 0x80);
    } else {
        GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    }
}

void mbObjFadeTexRotSet(s16 objId, HuVecF *pos, HuVecF *rot)
{
    int mdlId;
    MBOBJFADEWORK *work;
    HU3D_MODEL *modelP;

    mdlId = mbObjModelIDGet((MBOBJID)objId);
    modelP = &Hu3DData[mdlId];
    work = modelP->hookData;
    mbVecCopy(pos, &work->pos);
    mbVecCopy(rot, &work->rot);
}

void mbObjFadeTexColorSet(s16 objId, u8 r, u8 g, u8 b, float scale)
{
    int mdlId;
    MBOBJFADEWORK *work;
    HU3D_MODEL *modelP;

    mdlId = mbObjModelIDGet((MBOBJID)objId);
    modelP = &Hu3DData[mdlId];
    work = modelP->hookData;
    work->color.r = (u8)r;
    work->color.g = (u8)g;
    work->color.b = (u8)b;
    work->scale = scale;
}

static void MetalMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *matP);

void mbObjMetalCreate(s16 objId)
{
    int mdlId;
    MBOBJMETALWORK *work;
    HU3D_MODEL *modelP;
    HSF_DATA *hsf;
    int i;

    mdlId = mbObjModelIDGet((MBOBJID)objId);
    modelP = &Hu3DData[mdlId];
    if(modelP->attr & HU3D_ATTR_LINK) {
        modelP->hookData = work = mbMallocNum(sizeof(MBOBJMETALWORK), modelP->mallocNoLink);
    } else {
        modelP->hookData = work = mbMallocNum(sizeof(MBOBJMETALWORK), modelP->mallocNo);
    }
    work->magic = MBOBJMETAL_MAGIC;
    work->tpLvl = 1.0f;
    work->shadowColor.r = work->shadowColor.g = work->shadowColor.b = 255;
    work->hiliteColor.r = work->hiliteColor.g = work->hiliteColor.b = 255;
    work->shadowColor.r = 0x81;
    work->shadowColor.g = 0xFF;
    work->shadowColor.b = 0xAE;
    work->hiliteColor.r = 0xCA;
    work->hiliteColor.g = 0x57;
    work->hiliteColor.b = 0xFF;
    Hu3DModelMatHookSet(mdlId, MetalMatHook);
    hsf = modelP->hsf;
    for(i = 0; i < hsf->materialNum; i++) {
        hsf->material[i].flags |= HSF_MATERIAL_MATHOOK;
    }
    work->anim = HuSprAnimRead(HuDataSelHeapReadNum(mbBoardDataNumGet(0x50069), HU_MEMNUM_OVL, HEAP_MODEL));
    work->hiliteAnim = HuSprAnimRead(HuDataSelHeapReadNum(mbBoardDataNumGet(0x50068), HU_MEMNUM_OVL, HEAP_MODEL));
}

BOOL mbObjMetalKill(s16 objId)
{
    int mdlId;
    MBOBJMETALWORK *work;
    HU3D_MODEL *modelP;
    HSF_DATA *hsf;
    int i;

    mdlId = mbObjModelIDGet((MBOBJID)objId);
    modelP = &Hu3DData[mdlId];
    if(modelP->hookData == NULL) {
        return FALSE;
    }
    work = modelP->hookData;
    if(work->magic != MBOBJMETAL_MAGIC) {
        return FALSE;
    }
    Hu3DModelMatHookSet(mdlId, NULL);
    hsf = modelP->hsf;
    for(i = 0; i < hsf->materialNum; i++) {
        hsf->material[i].flags &= ~HSF_MATERIAL_MATHOOK;
    }
    HuSprAnimKill(work->anim);
    HuSprAnimKill(work->hiliteAnim);
    HuMemDirectFree(work);
    modelP->hookData = NULL;
    return TRUE;
}

void mbObjMetalTPLvlSet(s16 objId, float tpLvl)
{
    int mdlId;
    MBOBJMETALWORK *work;
    HU3D_MODEL *modelP;

    mdlId = mbObjModelIDGet((MBOBJID)objId);
    modelP = &Hu3DData[mdlId];
    work = modelP->hookData;
    work->tpLvl = tpLvl;
}

void mbObjMetalColorSet(s16 objId, GXColor shadowColor, GXColor hiliteColor)
{
    int mdlId;
    MBOBJMETALWORK *work;
    HU3D_MODEL *modelP;

    mdlId = mbObjModelIDGet((MBOBJID)objId);
    modelP = &Hu3DData[mdlId];
    work = modelP->hookData;
    work->shadowColor = shadowColor;
    work->hiliteColor = hiliteColor;
}

static inline float SNpcFabs(register float x)
{
    asm {
        fabs x, x
    }
    return x;
}

static void MetalMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *matP)
{
    Mtx mtx;
    Mtx mtxTemp;
    HuVecF halfDir;
    HuVecF axis;
    u32 tevStage;
    u32 texCoord;
    GXColor color;
    MBOBJMETALWORK *work;
    float angle;

    work = drawObj->model->hookData;
    mbObjStarTevStageSet(drawObj, matP, &tevStage, &texCoord);
    if(!(work->tpLvl <= 0.05f)) {
        HuSprTexLoad(work->anim, 0, GX_TEXMAP4, GX_REPEAT, GX_REPEAT, GX_LINEAR);
        HuSprTexLoad(work->hiliteAnim, 0, GX_TEXMAP5, GX_REPEAT, GX_REPEAT, GX_LINEAR);
        PSMTXCopy(drawObj->matrix, mtx);
        PSMTXScale(mtxTemp, 0.5f / drawObj->scale.x, -0.5f / drawObj->scale.y, 0.5f / drawObj->scale.z);
        PSMTXConcat(mtxTemp, mtx, mtx);
        mtx[0][3] = mtx[1][3] = 0.5f;
        GXLoadTexMtxImm(mtx, GX_TEXMTX4, GX_MTX2x4);
        PSMTXCopy(drawObj->matrix, mtx);
        {
            HuVecF baseDir = { 0.0f, 0.0f, -1.0f };
            PSMTXMultVecSR(Hu3DCameraMtx, &Hu3DGlobalLight[0].dir, &halfDir);
            C_VECHalfAngle(&baseDir, &halfDir, &halfDir);
            if(SNpcFabs(halfDir.z) < 0.999f) {
                angle = acosf(halfDir.z);
                PSVECCrossProduct(&baseDir, &halfDir, &axis);
                PSMTXRotAxisRad(mtxTemp, &axis, angle);
                PSMTXConcat(mtxTemp, mtx, mtx);
            }
        }
        PSMTXScale(mtxTemp, 0.5f / drawObj->scale.x, -0.5f / drawObj->scale.y, 0.5f / drawObj->scale.z);
        PSMTXConcat(mtxTemp, mtx, mtx);
        mtx[0][3] = 0.5f;
        mtx[1][3] = 0.5f;
        GXLoadTexMtxImm(mtx, GX_TEXMTX5, GX_MTX2x4);
        GXSetNumTexGens(texCoord + 2);
        GXSetNumTevStages(tevStage + 3);
        GXSetTexCoordGen(texCoord, GX_TG_MTX2x4, GX_TG_NRM, GX_TEXMTX4);
        GXSetTexCoordGen(texCoord + 1, GX_TG_MTX2x4, GX_TG_NRM, GX_TEXMTX5);
        GXSetTevOrder(tevStage, texCoord, GX_TEXMAP4, GX_COLOR_NULL);
        GXSetTevKColorSel(tevStage, kColorTbl[(int)(7.9f * (1.0f - work->tpLvl))]);
        GXSetTevColorIn(tevStage, GX_CC_CPREV, GX_CC_TEXC, GX_CC_KONST, GX_CC_ZERO);
        GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        tevStage++;
        GXSetTevOrder(tevStage, texCoord + 1, GX_TEXMAP5, GX_COLOR_NULL);
        color.r = work->shadowColor.r * work->tpLvl;
        color.g = work->shadowColor.g * work->tpLvl;
        color.b = work->shadowColor.b * work->tpLvl;
        GXSetTevKColor(GX_KCOLOR2, color);
        GXSetTevKColorSel(tevStage, GX_TEV_KCSEL_K2);
        GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, GX_CC_CPREV);
        GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
        tevStage++;
        GXSetTevOrder(tevStage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        color.r = work->hiliteColor.r * work->tpLvl;
        color.g = work->hiliteColor.g * work->tpLvl;
        color.b = work->hiliteColor.b * work->tpLvl;
        GXSetTevKColor(GX_KCOLOR3, color);
        GXSetTevKColorSel(tevStage, GX_TEV_KCSEL_K3);
        GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_A0, GX_CC_KONST, GX_CC_CPREV);
        GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    }
}

static void BiriQMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *matP);

void mbObjBiriQCreate(s16 objId)
{
    int mdlId;
    MBOBJBIRIQWORK *work;
    HU3D_MODEL *modelP;
    HSF_DATA *hsf;
    int i;

    mdlId = mbObjModelIDGet((MBOBJID)objId);
    modelP = &Hu3DData[mdlId];
    if(modelP->attr & HU3D_ATTR_LINK) {
        modelP->hookData = work = mbMallocNum(sizeof(MBOBJBIRIQWORK), modelP->mallocNoLink);
    } else {
        modelP->hookData = work = mbMallocNum(sizeof(MBOBJBIRIQWORK), modelP->mallocNo);
    }
    work->magic = MBOBJBIRIQ_MAGIC;
    work->alpha = 0.0f;
    work->color.r = work->color.g = work->color.b = work->color.a = 255;
    Hu3DModelMatHookSet(mdlId, BiriQMatHook);
    hsf = modelP->hsf;
    for(i = 0; i < hsf->materialNum; i++) {
        hsf->material[i].flags |= HSF_MATERIAL_MATHOOK;
    }
}

BOOL mbObjBiriQKill(s16 objId)
{
    int mdlId;
    MBOBJBIRIQWORK *work;
    HU3D_MODEL *modelP;
    HSF_DATA *hsf;
    int i;

    mdlId = mbObjModelIDGet((MBOBJID)objId);
    modelP = &Hu3DData[mdlId];
    if(modelP->hookData == NULL) {
        return FALSE;
    }
    work = modelP->hookData;
    if(work->magic != MBOBJBIRIQ_MAGIC) {
        return FALSE;
    }
    Hu3DModelMatHookSet(mdlId, NULL);
    hsf = modelP->hsf;
    for(i = 0; i < hsf->materialNum; i++) {
        hsf->material[i].flags &= ~HSF_MATERIAL_MATHOOK;
    }
    HuMemDirectFree(work);
    modelP->hookData = NULL;
    return TRUE;
}

void mbObjBiriQColorSet(s16 objId, int mode, float alpha, GXColor color)
{
    int mdlId;
    MBOBJBIRIQWORK *work;
    HU3D_MODEL *modelP;

    mdlId = mbObjModelIDGet((MBOBJID)objId);
    modelP = &Hu3DData[mdlId];
    if(modelP->hookData != NULL) {
        work = modelP->hookData;
        if(work->magic == MBOBJBIRIQ_MAGIC) {
            work->mode = mode;
            work->alpha = alpha;
            work->color = color;
        }
    }
}

static void BiriQMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *matP)
{
    u32 tevStage;
    u32 texCoord;
    GXColor color;
    MBOBJBIRIQWORK *work;
    BIRIQMAT *matTbl;
    int i;

    work = drawObj->model->hookData;
    mbObjStarTevStageSet(drawObj, matP, &tevStage, &texCoord);
    if(!(work->alpha <= 0.01f)) {
        GXSetNumTevStages(tevStage + biriQMatNumTbl[work->mode]);
        switch(work->mode) {
            case 0:
            case 3:
                color.r = work->color.r;
                color.g = work->color.g;
                color.b = work->color.b;
                break;

            case 1:
            case 2:
                color.r = work->color.r * work->alpha;
                color.g = work->color.g * work->alpha;
                color.b = work->color.b * work->alpha;
                break;
        }
        color.a = 255.0f * work->alpha;
        GXSetTevKColor(GX_KCOLOR3, color);
        GXSetTevKColorSel(tevStage, GX_TEV_KCSEL_K3);
        GXSetTevKAlphaSel(tevStage, GX_TEV_KASEL_K3_A);
        matTbl = &biriQMatTbl[work->mode * 4];
        for(i = 0; i < biriQMatNumTbl[work->mode]; i++, tevStage++) {
            GXSetTevOrder(tevStage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
            GXSetTevColorOp(tevStage, matTbl->op, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, matTbl->outReg);
            GXSetTevColorIn(tevStage, matTbl->a, matTbl->b, matTbl->c, matTbl->d);
            matTbl++;
            GXSetTevAlphaOp(tevStage, matTbl->op, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, matTbl->outReg);
            GXSetTevAlphaIn(tevStage, matTbl->a, matTbl->b, matTbl->c, matTbl->d);
            matTbl++;
        }
    }
}
/* @@END@@ */
