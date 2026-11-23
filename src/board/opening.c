#include "game/board/opening.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/coin.h"
#include "game/board/comchoice.h"
#include "game/board/dice.h"
#include "game/board/effect.h"
#include "game/board/telop.h"
#include "game/board/guide.h"
#include "game/board/masu.h"
#include "game/board/math.h"
#include "game/board/object.h"
#include "game/board/player.h"
#include "game/board/status.h"
#include "game/board/window.h"
#include "game/board/wipe.h"

#include "game/audio.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/memory.h"
#include "game/pad.h"
#include "game/sprite.h"
#include "game/wipe.h"

#include "math.h"
#include "string.h"

/* star.h declares mbStarMapViewProcExec(int, HuVecF *, int); retail calls it with no arguments. */
void mbStarMapViewProcExec();

typedef void (*OPENINGHOOK)(void);

typedef struct OpeningCoin_s {
    s16 id; /* 0x00 */
    HuVecF pos; /* 0x04 */
    HuVecF rot; /* 0x10 */
    HuVecF vel; /* 0x1C */
    BOOL dispF; /* 0x28 */
    int playerNo; /* 0x2C */
    u8 unk30[0xC];
} OPENINGCOIN; /* size 0x3C */

typedef struct OpeningParticleData_s {
    s16 time; /* 0x00 */
    s16 maxTime; /* 0x02 */
    u8 unk04[4];
    HuVecF vel; /* 0x08 */
    float scaleBase; /* 0x14 */
    u8 unk18[0x14];
    float scale; /* 0x2C */
    u8 unk30[0xC];
    HuVecF pos; /* 0x3C */
    GXColor color; /* 0x48 */
    u8 unk4C[0x10];
} OPENINGPARTICLEDATA; /* size 0x5C */

typedef struct OpeningParticle_s {
    s16 initF; /* 0x00 */
    u8 unk02[0x1E];
    u8 unk20; /* 0x20 */
    u8 unk21[5];
    s16 num; /* 0x26 */
    u8 unk28[0x40];
    OPENINGPARTICLEDATA *data; /* 0x68 */
} OPENINGPARTICLE;

static HuVecF openingRot;
static HuVecF openingPos;
static int openingPadDelay[4];
static OPENINGCOIN openingCoinWork[40];
static HuVecF guideCurPos;

static float openingZoom;
static BOOL singleGuideEffOnF;
static int singleGuideWork[2];
static OMOBJ *singleGuideObj;
static HuVecF *singleOpeningPosTbl2;
static HuVecF *singleOpeningPosTbl;
static float *singleOpeningZoomTbl;
static s16 singleWinId;
static s16 singleEff1MdlId;
static int singleFXNo;
static OPENINGHOOK openingInstHook;
static OPENINGHOOK openingStarInstHook;
static HUPROCESS *openingSingleProc;
static HUPROCESS *openingProc;

static s16 openingGuideObjId = -1;

static int guideMotFileTbl[] = {
    0x110001, 0x110002, 0x110003, 0x110004, 0x110005, 0x110006,
    0x110007, 0x110008, 0x110009, 0x11000A, 0x11000B, 0x11000C,
    0x11000D, 0x11000E, 0x11000F, 0x110010, 0x110011, 0x110012,
    0x110013, 0x110014, 0x110015, 0x110016, 0x110017, -1
};

static u32 welcomeMesTbl[] = {
    0x2C0000, 0x2C0001, 0x2C0002, 0x2C0003, 0x2C0004, 0x2C0005
};

static HuVecF openingUnusedVec[2] = {
    { -28, 0, 0 },
    { 0, 0, 500 }
};

static float playerCenterDist[4] = { -3, -1, 1, 3 };

static void OpeningCoinExec(void);
static void PlayerDropExec(HuVecF *pos);
static void PlayerOrderSet(int *order);
static void ev_OpeningSingle(void);
static void ev_OpeningSingleKill(void);
static void OpeningSingleEffHook(HU3D_MODEL *modelP, OPENINGPARTICLE *particle, Mtx mtx);

static inline BOOL mbTeamFGet(void)
{
    return GwSystem.tagF;
}

static inline int GWPlayerTeamGet(int playerNo)
{
    return GwPlayer[playerNo].team;
}

static inline GW_PLAYER *GWPlayerGet(int playerNo)
{
    return &GwPlayer[playerNo];
}

static inline void *OpeningMalloc(s32 size)
{
    return HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
}

static inline void OpeningFree(void *ptr)
{
    HuMemDirectFree(ptr);
}

static float OpeningCurveEval(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float t)
{
    HuVecF vec;
    vec.x = mbHermiteCalcSlope(a->x, b->x, c->x, d->x, t);
    vec.y = mbHermiteCalcSlope(a->y, b->y, c->y, d->y, t);
    vec.z = mbHermiteCalcSlope(a->z, b->z, c->z, d->z, t);
    return VECMag(&vec);
}

void mbev_Opening(void)
{
    if(GWPartyGet() == FALSE) {
        mbev_OpeningSingle();
    } else {
        mbev_OpeningParty();
    }
}

static void ev_OpeningParty(void);
static void ev_OpeningPartyKill(void);
static u16 OpeningPadBtn(int playerNo);

void mbev_OpeningParty(void)
{
    if(!_CheckFlag(0x1000E)) {
        openingProc = HuPrcChildCreate(ev_OpeningParty, 0x200C, 0x4000, 0, mbMainProc);
    }
    HuPrcDestructorSet2(openingProc, ev_OpeningPartyKill);
    while(openingProc != NULL) {
        HuPrcVSleep();
    }
}

static void ev_OpeningParty(void)
{
    int orderTbl[10];
    HuVecF playerPos;
    HuVecF cameraOfs = { 0, 250, 0 };
    HuVecF masuPos;
    HuVecF guidePos;
    HuVecF cameraPos;
    HuVecF dir;
    HuVecF cameraRot = { -20, 0, 0 };
    int playerOrder[4];
    int humanNo;
    float zoom = 20000;
    int i;
    int comNum;
    int order1;
    int order2;
    int temp;
    BOOL allComF;
    s16 angle;
    s16 masuId;
    GW_PLAYER *playerP;
    MBPLAYERWORK *playerWorkP;

    mbCameraNearFarSet(10, 30000);
    HuDataDirRead(0x110000);
    openingGuideObjId = mbObjCreate(0x110000, guideMotFileTbl, FALSE);
    HuDataDirClose(0x110000);
    for(i=0; i<4; i++) {
        mbObjDispSet(mbPlayerObjIDGet(i), FALSE);
    }
    for(i=3, comNum=0; i>-1; i--) {
        if(GwPlayerConf[i].type != 0) {
            comNum++;
        } else {
            humanNo = i;
        }
    }
    if(comNum >= 4) {
        allComF = TRUE;
    } else {
        allComF = FALSE;
    }
    mbCameraZoomSet(openingZoom);
    mbCameraRotSetV(&openingRot);
    mbCameraCenterSetV(&openingPos);
    masuId = mbMasuFind_AttrIdGet(-1, 0x8000);
    mbMasuPosGet(masuId, &masuPos);
    guidePos = masuPos;
    guidePos.z -= 200;
    mbObjPosSetV(openingGuideObjId, &guidePos);
    mbObjMotionShiftSet(openingGuideObjId, 1, 0, 8, 0x40000001);
    mbWipeFadeIn();
    mbMusPlay(0, 11, 127, 0);
    mbTelopCreate(-1, mbBoardNoGet()+16, FALSE);
    HuPrcSleep(72);
    cameraPos = masuPos;
    cameraPos.y -= 50;
    mbCameraMovePos(&cameraPos, &cameraRot, &cameraOfs, 1800, -1, 180);
    mbCameraMoveWait();
    playerPos.x = masuPos.x;
    playerPos.y = masuPos.y;
    playerPos.z = masuPos.z;
    PlayerDropExec(&playerPos);
    for(i=0; i<4; i++) {
        mbPlayerPosGet(i, &playerPos);
        VECSubtract(&guidePos, &playerPos, &dir);
        angle = 90-HuAtan(dir.z, dir.x);
        mbPlayerRotateStart(i, angle, 30);
    }
    while(!mbPlayerRotateCheckAll()) {
        HuPrcVSleep();
    }
    mbObjMotionShiftSet(openingGuideObjId, 12, 0, 8, 0x40000001);
    mbAudGuidePlay(0x3B6);
    mbWinCreate(2, welcomeMesTbl[mbBoardNoGet()], 6);
    mbWinTopWait();
    mbAudGuidePlay(0x3B8);
    mbWinCreateChoice(2, 0x2C0009, 6, 0);
    if(allComF) {
        GwSystem.turnPlayerNo = 0;
        mbComChoiceDownSet();
    }
    mbWinTopWait();
    if(mbWinTopChoiceGet() == 0 && openingInstHook != NULL) {
        openingInstHook();
    }
    mbObjMotionShiftSet(openingGuideObjId, 12, 0, 8, 0x40000001);
    mbAudGuidePlay(0x3B8);
    mbWinCreate(2, 0x2C000A, 6);
    mbWinTopWait();
    mbObjMotionShiftSet(openingGuideObjId, 1, 0, 8, 0x40000001);
    for(i=0; i<4; i++) {
        mbPlayerRotateStart(i, 0, 30);
    }
    while(!mbPlayerRotateCheckAll()) {
        HuPrcVSleep();
    }
    for(i=0; i<10; i++) {
        orderTbl[i] = i;
    }
    for(i=0; i<100; i++) {
        order1 = mbRandMod(10);
        order2 = mbRandMod(10);
        temp = orderTbl[order1];
        orderTbl[order1] = orderTbl[order2];
        orderTbl[order2] = temp;
    }
    for(i=0; i<4; i++) {
        mbDiceExec(i, 0, NULL, orderTbl[i], FALSE, FALSE, NULL, 0);
        if(GwPlayer[i].comF) {
            openingPadDelay[i] = mbRandMod(30)+30;
            mbDicePadBtnHookSet(i, OpeningPadBtn);
        }
    }
    mbWinCreateHelp(0x260002);
    mbWinTopPosSet(228, 392);
    while(!mbDiceKillCheckAll()) {
        HuPrcVSleep();
    }
    mbWinTopKill();
    memcpy(playerOrder, orderTbl, 4*sizeof(int));
    PlayerOrderSet(playerOrder);
    mbStatusReset();
    for(i=0; i<4; i++) {
        mbAudGuidePlay(0x3B8);
        mbWinCreateTime(2, 0x2C000B+i, -1);
        mbWinTopInsertMesSet(mbPlayerNameMesGet(i), 0);
        if(GwPlayer[i].comF || HuPadStatGet(GwPlayer[i].padNo) != 0) {
            mbWinTopPlayerDisable(-1);
        } else {
            mbWinTopPlayerDisable(i);
        }
        mbWinTopWait();
        playerP = GWPlayerGet(i);
        playerWorkP = mbPlayerWorkGet(i);
        mbDiceNumShrinkSet(playerP->circuitOrder);
        playerP->circuitOrder = playerWorkP->playerNo = i;
        if(!mbTeamFGet()) {
            mbStatusDispSet(i, TRUE);
        }
        mbPlayerWinLoseVoicePlay(i, 12, 0x243);
        mbPlayerMotionShiftSet(i, 12, 0, 8, 0);
        omVibrate(i, 20, 20, 0);
        HuPrcSleep(8);
        while(!mbPlayerMotionEndCheck(i)) {
            HuPrcVSleep();
        }
        mbPlayerMotionShiftSet(i, 1, 0, 8, 0x40000001);
    }
    HuPrcSleep(30);
    for(i=0; i<4; i++) {
        mbPlayerMotIdleSet(i);
    }
    if(mbTeamFGet()) {
        mbStatusDispSetAll(TRUE);
    }
    HuPrcSleep(30);
    mbObjMotionShiftSet(openingGuideObjId, 12, 0, 8, 0x40000001);
    mbAudGuidePlay(0x3B8);
    mbWinCreate(2, 0x2C000F, 6);
    mbWinTopWait();
    mbObjMotionShiftSet(openingGuideObjId, 6, 0, 8, 0);
    HuPrcSleep(40);
    OpeningCoinExec();
    HuPrcSleep(30);
    mbObjMotionShiftSet(openingGuideObjId, 1, 0, 8, 0x40000001);
    for(i=0; i<4; i++) {
        mbPlayerWinLoseVoicePlay(i, 12, 0x243);
        mbPlayerMotionShiftSet(i, 7, 0, 8, 0);
    }
    while(!mbPlayerMotionEndCheckAll()) {
        HuPrcVSleep();
    }
    openingZoom = mbCameraZoomGet();
    mbCameraRotGet(&openingRot);
    mbCameraCenterGet(&openingPos);
    if(openingStarInstHook != NULL) {
        openingStarInstHook();
    } else {
        mbStarMapViewProcExec();
    }
    mbObjMotionShiftSet(openingGuideObjId, 12, 0, 8, 0x40000001);
    mbAudGuidePlay(0x3B6);
    mbWinCreate(2, 0x2C0010, 6);
    mbWinTopWait();
    mbObjMotionShiftSet(openingGuideObjId, 1, 0, 8, 0x40000001);
    for(i=0; i<4; i++) {
        mbPlayerWinLoseVoicePlay(i, 12, 0x243);
        mbPlayerMotionShiftSet(i, 12, 0, 8, 0);
    }
    while(!mbPlayerMotionEndCheckAll()) {
        HuPrcVSleep();
    }
    HuPrcSleep(30);
    mbMusFadeOutSpeed(0, 1000);
    WipeColorSet(0, 0, 0);
    WipeCreate(2, 0, 21);
    for(i=0; i<60 || WipeCheck(); i++) {
        HuPrcVSleep();
    }
    for(i=0; i<4; i++) {
        mbPlayerMotionSet(i, 1, 0x40000001);
    }
    mbCameraNearFarSet(100, 20000);
    mbObjKill(openingGuideObjId);
    openingGuideObjId = -1;
    mbMusBoardPlay();
    HuPrcEnd();
}

static u16 OpeningPadBtn(int playerNo)
{
    if(openingPadDelay[playerNo]) {
        if(--openingPadDelay[playerNo] == 0) {
            return PAD_BUTTON_A;
        }
    }
    return 0;
}

static void ev_OpeningPartyKill(void)
{
    openingProc = NULL;
}

void mbOpeningViewSet(HuVecF *rot, HuVecF *pos, float zoom)
{
    if(rot) {
        openingRot = *rot;
    }
    if(pos) {
        openingPos = *pos;
    }
    if(zoom >= 0) {
        openingZoom = zoom;
    }
}

static void OpeningCoinExec(void)
{
    HuVecF pos;
    OPENINGCOIN *coin;
    int num;
    int i;
    int j;

    coin = openingCoinWork;
    num = 0;
    for(i=0; i<4; i++) {
        mbPlayerPosGet(i, &pos);
        for(j=0; j<10; j++, coin++) {
            coin->id = mbCoinCreate();
            coin->pos.x = (pos.x+(100.0f*(0.2f*frandf())))-10.0f;
            coin->pos.y = 800.0f+pos.y;
            coin->pos.z = pos.z;
            pos.y += 120.00001f;
            coin->vel.y = -10.0f;
            coin->vel.x = coin->vel.z = 0;
            coin->rot.y = mbRandMod(360);
            coin->dispF = TRUE;
            mbCoinObjPosSetV(coin->id, &coin->pos);
            mbCoinObjRotSetV(coin->id, &coin->rot);
            mbCoinObjDispSet(coin->id, coin->dispF);
            coin->playerNo = i;
            num++;
        }
    }
    while(1) {
        for(coin=openingCoinWork, i=0; i<40; i++, coin++) {
            if(coin->dispF) {
                mbPlayerPosGet(coin->playerNo, &pos);
                VECAdd(&coin->pos, &coin->vel, &coin->pos);
                coin->vel.y -= 0.4f;
                coin->rot.y += 5.0f;
                mbCoinObjPosSetV(coin->id, &coin->pos);
                mbCoinObjRotSetV(coin->id, &coin->rot);
                if(coin->pos.y <= 150.0f+pos.y) {
                    mbCoinObjKill(coin->id);
                    HuAudFXPlay(7);
                    mbPlayerCoinAdd(coin->playerNo, 1);
                    coin->dispF = FALSE;
                    num--;
                }
            }
        }
        if(num <= 0) {
            break;
        }
        HuPrcVSleep();
    }
    mbAudFXPlay(15);
}

static void PlayerDropExec(HuVecF *pos)
{
    int delay[4] = { 0, 30, 60, 90 };
    HuVecF playerPos[4];
    HuVecF vel[4];
    BOOL landF[4];
    int i;
    int num;

    for(i=0; i<4; i++) {
        mbObjDispSet(mbPlayerObjIDGet(i), TRUE);
        mbPlayerRotYSet(i, 0);
        playerPos[i].x = pos->x+(100.0f*playerCenterDist[i]);
        playerPos[i].y = 800.0f+pos->y;
        playerPos[i].z = pos->z;
        vel[i].y = -20.0f;
        vel[i].x = vel[i].z = 0;
        landF[i] = FALSE;
        mbPlayerPosSetV(i, &playerPos[i]);
        mbPlayerMotionShiftSet(i, 4, 0, 8, 0);
        mbPlayerMotionVoiceOnSet(i, 4, FALSE);
    }
    num = 0;
    while(1) {
        for(i=0; i<4; i++) {
            if(landF[i] && mbPlayerMotionEndCheck(i)) {
                mbPlayerMotionShiftSet(i, 1, 0, 8, 0x40000001);
                num++;
            }
            if(delay[i]-- <= 0 && !landF[i]) {
                VECAdd(&playerPos[i], &vel[i], &playerPos[i]);
                vel[i].y -= 0.2f;
                if(playerPos[i].y <= pos->y) {
                    playerPos[i].y = pos->y;
                    landF[i] = TRUE;
                    mbPlayerMotionShiftSet(i, 5, 0, 8, 0);
                }
                mbPlayerPosSetV(i, &playerPos[i]);
            }
        }
        if(num >= 4) {
            break;
        }
        HuPrcVSleep();
    }
    for(i=0; i<4; i++) {
        mbPlayerMotionVoiceOnSet(i, 4, TRUE);
    }
}

static void PlayerOrderSet(int *order)
{
    int teamCoin[2];
    int teamStar[2];
    int i;
    int j;
    int temp;
    int grp;

    mbPlayerColSnapSet(FALSE);
    for(i=0; i<3; i++) {
        for(j=i+1; j<4; j++) {
            if(order[i] < order[j]) {
                temp = order[i];
                order[i] = order[j];
                order[j] = temp;
                mbPlayerSwap(i, j);
            }
        }
    }
    if(mbTeamFGet()) {
        for(i=0; i<2; i++) {
            teamCoin[i] = 0;
            teamStar[i] = 0;
        }
        for(i=0; i<4; i++) {
            grp = GWPlayerTeamGet(i);
            teamCoin[grp] += GwPlayer[i].coin;
            teamStar[grp] += GwPlayer[i].star;
            GwPlayer[i].coin = 0;
            GwPlayer[i].star = 0;
        }
        for(i=0; i<2; i++) {
            mbPlayerTeamCoinSet(i, teamCoin[i]);
            mbPlayerGrpStarSet(i, teamStar[i]);
        }
    }
    HuPrcVSleep();
}

void mbev_OpeningSingle(void)
{
    int *work;
    int i;
    s16 winId;

    work = singleGuideWork;
    mbDirClose();
    work[1] = FALSE;
    work[0] = FALSE;
    singleGuideObj = NULL;
    singleOpeningPosTbl2 = NULL;
    singleOpeningPosTbl = NULL;
    singleOpeningZoomTbl = NULL;
    singleWinId = -1;
    singleEff1MdlId = -1;
    singleFXNo = -1;
    winId = -1;
    if(!_CheckFlag(0x1000E)) {
        openingSingleProc = HuPrcChildCreate(ev_OpeningSingle, 0x200C, 0x4000, 0, mbMainProc);
    }
    HuPrcDestructorSet2(openingSingleProc, ev_OpeningSingleKill);
    do {
        if(work[0] && mbTelopCheck()) {
            if(winId < 0) {
                winId = mbWinCreateHelp(0x26000C);
                mbWinPosSet(winId, 228, 408);
            }
            for(i=0; i<4; i++) {
                if(!GwPlayer[i].comF && (HuPadBtnDown[GwPlayer[i].padNo] & PAD_BUTTON_START)) {
                    if(singleWinId != -1) {
                        mbWinKill(singleWinId);
                        singleWinId = -1;
                    }
                    HuPrcKill(openingSingleProc);
                    work[1] = TRUE;
                    break;
                }
            }
        }
        HuPrcVSleep();
    } while(!work[1]);
    mbWipeWait();
    if(!WipeCheckIn()) {
        mbWipeFadeOut();
    }
    mbWinKill(winId);
    OSReport("/-------------------------SINGLE_DATA_KILL-------------------------/\n");
    if(singleGuideObj != NULL) {
        mbGuideKill(singleGuideObj);
    }
    if(singleOpeningPosTbl2 != NULL) {
        OpeningFree(singleOpeningPosTbl2);
    }
    if(singleOpeningPosTbl != NULL) {
        OpeningFree(singleOpeningPosTbl);
    }
    if(singleOpeningZoomTbl != NULL) {
        OpeningFree(singleOpeningZoomTbl);
    }
    if(singleEff1MdlId != -1) {
        mbParticleKill(singleEff1MdlId);
        HuDataDirClose(0x210000);
    }
    if(singleFXNo != -1) {
        mbAudFXStop(singleFXNo);
    }
}

static int openingUnusedTbl[3] = { 0x2D, 0x2E, 0x2F };

static HuVecF singleGuidePosTbl[10] = {
    { 0, 0, 0 },
    { -28, 0, 0 },
    { 0, 0, 500 },
    { 0, 250, 0 },
    { -20, 0, 0 },
    { 800, -100, 2000 },
    { -700, -100, 0 },
    { -700, 600, 0 },
    { -400, 600, 0 },
    { -200, 0, 0 }
};

static int singleGuideMotFileTbl[] = {
    0x110001, 0x110004, 0x110005, 0x110006, 0x110007, 0x11000A,
    0x11000C, 0x11000F, 0x110011, 0x110013, 0x110014, -1
};

static u32 singleWelcomeMesTbl[] = {
    0x2C0006, 0x2C0007, 0x2C0008
};

static inline void OpeningCurveSetup(HuVecF *posTbl, int no, int num, HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d)
{
    HuVecF *p;
    HuVecF tanA;
    HuVecF tanB;

    if(no > num-1) {
        no = num-1;
    }
    p = &posTbl[no];
    if(no == 0) {
        VECSubtract(&p[1], &p[0], &tanA);
    } else {
        VECSubtract(&p[1], &p[-1], &tanA);
    }
    if(no == num-2) {
        VECSubtract(&p[1], &p[0], &tanB);
    } else {
        VECSubtract(&p[2], &p[0], &tanB);
    }
    VECScale(&tanA, &tanA, 0.5f);
    VECScale(&tanB, &tanB, 0.5f);
    *a = p[0];
    *b = p[1];
    *c = tanA;
    *d = tanB;
}

static inline float OpeningPathLenGet(void *func, HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float end)
{
    int div;
    int i;
    int j;
    float start;
    float len;
    float delta;
    float dLen;
    float dAccel;

    div = 10;
    start = 0;
    len = 0;
    delta = end-start;
    dLen = 0.5f*(delta*(((float (*)())func)(a, b, c, d, start)+((float (*)())func)(a, b, c, d, end)));
    for(i=1; i<=div; i*=2) {
        for(dAccel=0, j=1; j<=i; j++) {
            dAccel += ((float (*)())func)(a, b, c, d, start+(delta*(j-0.5f)));
        }
        dAccel *= delta;
        len = 0.33333334f*(dLen+(2.0f*dAccel));
        delta *= 0.5f;
        dLen = 0.5f*(dLen+dAccel);
    }
    return len;
}

static inline float OpeningPathLenGet2(void *func, HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float end)
{
    int i;
    int div;
    float start;
    float delta;
    float t;
    float sum;

    div = 10;
    start = 0;
    delta = (end-start)/div;
    t = start;
    sum = 0;
    for(i=0; i<div-1; i++) {
        t += delta;
        sum += ((float (*)())func)(a, b, c, d, t);
    }
    return 0.5*delta*(2.0*sum+(((float (*)())func)(a, b, c, d, start)+((float (*)())func)(a, b, c, d, end)));
}

static inline float OpeningPathTimeGet(void *func, HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float t, float len)
{
    int step;
    float speed;
    float pathLen;
    float oldT;
    float minLen;

    minLen = 0.1f;
    step = 0;
    do {
        pathLen = OpeningPathLenGet2(func, a, b, c, d, t)-len;
        if(fabs(speed = ((float (*)())func)(a, b, c, d, t)) < minLen) {
            speed = 1.0f;
        }
        oldT = t;
        t -= pathLen/speed;
        step++;
    } while(t != oldT && step < 10);
    return t;
}

static inline void OpeningBezierCalc(HuVecF *pts, int num, float t, HuVecF *out)
{
    HuVecF *buf;
    int j;
    int i;

    buf = OpeningMalloc(num*sizeof(HuVecF));
    memcpy(buf, pts, num*sizeof(HuVecF));
    for(i=1; i<num; i++) {
        for(j=0; j<num-i; j++) {
            buf[j].x = buf[j].x+(t*(buf[j+1].x-buf[j].x));
            buf[j].y = buf[j].y+(t*(buf[j+1].y-buf[j].y));
            buf[j].z = buf[j].z+(t*(buf[j+1].z-buf[j].z));
        }
    }
    *out = buf[0];
    OpeningFree(buf);
}

static void ev_OpeningSingle(void)
{
    s16 masuList[256];
    HuVecF guidePath[5];
    HuVecF guideVel;
    HuVecF startPos;
    HuVecF center;
    HuVecF rot;
    HuVecF pos;
    HuVecF guidePos;
    HuVecF masuPos;
    HuVecF c;
    HuVecF d;
    HuVecF a;
    HuVecF b;
    HuVecF dir;
    HuVecF prevDir;
    HuVec2f winPos;
    int j;
    int playerNo;
    int boardNo;
    int size;
    float startY;
    ANIMDATA *anim;
    int *work;
    int i;
    int k;
    int num;
    int seg;
    s16 masuId;
    s16 guideMdlId;
    float t;
    float dist;
    float angle;

    work = singleGuideWork;
    playerNo = 0;
    singleGuideObj = mbGuideCreateFlag(&singleGuidePosTbl[0], (s8 *)singleGuideMotFileTbl, FALSE, FALSE, FALSE);
    mbGuideMotionNextSet(singleGuideObj, 1);
    guideMdlId = mbGuideModelGet(singleGuideObj);
    mbObjDispSet(guideMdlId, FALSE);
    mbObjMotionSet(guideMdlId, 5, 0);
    masuId = mbMasuFind_AttrIdGet(-1, 0x8000);
    num = mbMasuFind_TypeListGet2(masuId, 7, FALSE, FALSE, masuList);
    size = num*sizeof(HuVecF);
    singleOpeningPosTbl2 = OpeningMalloc(size);
    singleOpeningPosTbl = OpeningMalloc(size);
    for(i=num-1, j=0; i>-1; i--, j++) {
        mbMasuPosGet(masuList[i], &singleOpeningPosTbl[j]);
    }
    k = 0;
    singleOpeningPosTbl2[k++] = singleOpeningPosTbl[0];
    for(i=0; i<num-1; i++) {
        VECSubtract(&singleOpeningPosTbl[i+1], &singleOpeningPosTbl[i], &dir);
        if(i != 0) {
            VECNormalize(&prevDir, &prevDir);
            VECNormalize(&dir, &dir);
            if(VECDotProduct(&prevDir, &dir) < 0.7f) {
                singleOpeningPosTbl2[k++] = singleOpeningPosTbl[i];
            }
        }
        prevDir = dir;
    }
    singleOpeningPosTbl2[k++] = singleOpeningPosTbl[num-1];
    mbCameraNearFarSet(10, 30000);
    mbCameraZoomSet(mbOpeningZoomGet());
    mbOpeningRotGet(&rot);
    mbCameraRotSetV(&rot);
    mbOpeningPosGet(&pos);
    mbCameraCenterSetV(&pos);
    mbWipeFadeIn();
    mbMusBoardPlay();
    boardNo = mbBoardNoGet()-5;
    mbTelopCreate(-1, mbBoardNoGet()+16, FALSE);
    HuPrcSleep(90);
    work[0] = TRUE;
    startPos = singleOpeningPosTbl2[0];
    if(boardNo < 3) {
        mbCameraMovePos(&startPos, &singleGuidePosTbl[4], NULL, 2400, -1, 180);
        mbCameraMoveWait();
        HuPrcSleep(12);
        startY = startPos.y;
        singleOpeningZoomTbl = OpeningMalloc(k*sizeof(float));
        for(i=0; i<k-1; i++) {
            OpeningCurveSetup(singleOpeningPosTbl2, i, k, &a, &b, &c, &d);
            singleOpeningZoomTbl[i] = OpeningPathLenGet(OpeningCurveEval, &a, &b, &c, &d, 1);
        }
        t = 0;
        dist = 0;
        seg = 0;
        do {
            OpeningCurveSetup(singleOpeningPosTbl2, seg, k, &a, &b, &c, &d);
            t = OpeningPathTimeGet(OpeningCurveEval, &a, &b, &c, &d, t, dist);
            mbHermiteCalcV(&a, &b, &c, &d, &center, t);
            mbCameraCenterSetV(&center);
            dist += 20.0f;
            if(dist >= singleOpeningZoomTbl[seg]) {
                dist -= singleOpeningZoomTbl[seg];
                seg++;
                t -= 1.0f;
            }
            HuPrcVSleep();
        } while(seg < k-1);
    } else {
        mbCameraMovePos(&startPos, &singleGuidePosTbl[4], NULL, 3700, -1, 180);
        mbCameraMoveWait();
        HuPrcSleep(12);
        singleOpeningZoomTbl = NULL;
        mbCameraMoveMasu(masuId, &singleGuidePosTbl[4], NULL, 2400, -1, 300);
        mbCameraMoveWait();
    }
    HuPrcSleep(12);
    mbMasuPosGet(masuId, &masuPos);
    for(i=0; i<5; i++) {
        guidePath[i] = masuPos;
        guidePath[i].x += singleGuidePosTbl[i+5].x;
        guidePath[i].y += singleGuidePosTbl[i+5].y;
        guidePath[i].z += singleGuidePosTbl[i+5].z;
    }
    prevDir = guidePath[0];
    anim = HuSprAnimRead(HuDataReadNum(0x210001, HU_MEMNUM_OVL));
    singleEff1MdlId = mbParticleCreate(anim, 128);
    mbParticleHookSet(singleEff1MdlId, OpeningSingleEffHook);
    Hu3DModelLayerSet(singleEff1MdlId, 5);
    singleGuideEffOnF = TRUE;
    mbCameraMovePlayer(playerNo, NULL, NULL, 1800, -1, 60);
    mbObjDispSet(guideMdlId, TRUE);
    singleFXNo = mbAudFXPlay(0x445);
    for(i=0; i<=180u; i++) {
        t = i/180.0f;
        OpeningBezierCalc(guidePath, 5, t, &guidePos);
        mbObjPosSetV(guideMdlId, &guidePos);
        guideCurPos = guidePos;
        VECSubtract(&guidePos, &prevDir, &dir);
        angle = HuAtan(dir.x, dir.z);
        if(i == 150u) {
            mbObjMotionShiftSet(guideMdlId, 4, 0, 16, 0x40000001);
        } else if(i > 150u) {
            t = (i-150u)/30.0f;
            angle = (1.0f-t)*angle;
        }
        mbObjRotYSet(guideMdlId, angle);
        prevDir = guidePos;
        HuPrcVSleep();
    }
    singleGuideEffOnF = FALSE;
    mbCameraMoveWait();
    if(singleFXNo != -1) {
        mbAudFXStop(singleFXNo);
    }
    HuPrcSleep(60);
    mbGuideMotionSet(singleGuideObj, 12, TRUE);
    mbAudGuidePlay(0x3B6);
    singleWinId = mbWinCreate(2, singleWelcomeMesTbl[mbBoardNoGet()-6], 6);
    mbWinTopPosGet(&winPos);
    winPos.y -= 35.0f;
    mbWinTopPosSet(winPos.x, winPos.y);
    mbWinTopWait();
    singleWinId = -1;
    mbAudGuidePlay(0x3B8);
    singleWinId = mbWinCreate(2, 0x2C0011, 6);
    mbWinTopPosGet(&winPos);
    winPos.y -= 35.0f;
    mbWinTopPosSet(winPos.x, winPos.y);
    mbWinTopWait();
    singleWinId = -1;
    mbAudGuidePlay(0x3B6);
    singleWinId = mbWinCreate(2, 0x2C0012, 6);
    mbWinTopPosGet(&winPos);
    winPos.y -= 35.0f;
    mbWinTopPosSet(winPos.x, winPos.y);
    mbWinTopWait();
    singleWinId = -1;
    mbObjMotionShiftSet(guideMdlId, 5, 0, 8, 0);
    guideVel.x = guideVel.y = guideVel.z = 0;
    mbObjPosGet(guideMdlId, &guidePos);
    singleFXNo = mbAudFXPlay(0x445);
    for(i=0; i<120u; i++) {
        guideVel.y += -16.333334f;
        guidePos.y -= 0.016666668f*guideVel.y;
        mbObjPosSetV(guideMdlId, &guidePos);
        guideCurPos = guidePos;
        if(i == 12u) {
            singleGuideEffOnF = TRUE;
        }
        HuPrcVSleep();
    }
    if(singleFXNo != -1) {
        mbAudFXStop(singleFXNo);
    }
    HuPrcSleep(30);
    mbWipeFadeOut();
    work[0] = FALSE;
    work[1] = TRUE;
    HuPrcEnd();
}

static void ev_OpeningSingleKill(void)
{
    openingSingleProc = NULL;
}

float mbOpeningZoomGet(void)
{
    return openingZoom;
}

void mbOpeningRotGet(HuVecF *rot)
{
    *rot = openingRot;
}

void mbOpeningPosGet(HuVecF *pos)
{
    *pos = openingPos;
}

void mbOpeningInstHookSet(void *hook)
{
    openingInstHook = hook;
}

void mbOpeningStarInstHookSet(void *hook)
{
    openingStarInstHook = hook;
}

s16 mbOpeningGuideObjIdGet(void)
{
    return openingGuideObjId;
}

void mbOpeningGuidePosRestore(void)
{
    s16 masuId;
    HuVecF masuPos;
    HuVecF pos;

    masuId = mbMasuFind_AttrIdGet(-1, 0x8000);
    mbMasuPosGet(masuId, &masuPos);
    pos = masuPos;
    pos.z -= 200;
    mbObjPosSetV(openingGuideObjId, &pos);
    mbObjMotionShiftSet(openingGuideObjId, 1, 0, 8, 0x40000001);
}

static HuVecF openingCameraRestoreRot = { -20, 0, 0 };
static HuVecF openingCameraRestorePos = { 0, 250, 0 };

void mbOpeningCameraPosRestore(void)
{
    s16 masuId;
    HuVecF masuPos;
    HuVecF pos;

    masuId = mbMasuFind_AttrIdGet(-1, 0x8000);
    mbMasuPosGet(masuId, &masuPos);
    pos = masuPos;
    pos.y -= 50;
    mbCameraMovePos(&pos, &openingCameraRestoreRot, &openingCameraRestorePos, 1800, -1, 0);
}

static void OpeningSingleEffHook(HU3D_MODEL *modelP, OPENINGPARTICLE *particle, Mtx mtx)
{
    HuVecF dir;
    GXColor colorTbl[4] = {
        { 255, 255, 255, 128 },
        { 128, 255, 0, 128 },
        { 128, 255, 255, 128 },
        { 255, 128, 255, 128 }
    };
    OPENINGPARTICLEDATA *data;
    int i;

    if(particle->initF == 0) {
        data = particle->data;
        particle->unk20 = 1;
        for(i=0; i<particle->num; i++, data++) {
            data->vel.y = 0;
            data->scale = 0;
            data->time = 0;
            data->maxTime = -1;
        }
        particle->initF = 1;
    }
    data = particle->data;
    for(i=0; i<particle->num; i++, data++) {
        if(data->maxTime < 0 && mbRandMod(100) < 5) {
            data->maxTime = 60.0f*(0.4f+(0.4f*(0.000015258789f*(frand() & 0xFFFF))));
            dir.x = frandf()-frandf();
            dir.y = frandf()-frandf();
            dir.z = frandf()-frandf();
            VECNormalize(&dir, &dir);
            VECScale(&dir, &dir, 40.0f);
            data->pos.x = dir.x+guideCurPos.x;
            data->pos.y = dir.y+guideCurPos.y;
            data->pos.z = dir.z+guideCurPos.z;
            data->vel.x = 0;
            data->vel.y = 0;
            data->vel.z = 0;
            data->scaleBase = 20.0f+(20.0f*frandf());
            data->color = colorTbl[mbRandMod(4)];
            if(!singleGuideEffOnF) {
                data->color.a = 0;
                data->scale = 0;
            }
            data->time = 0;
        }
        if(data->maxTime >= 0) {
            data->pos.y += 0.016666668f*data->vel.y;
            data->vel.y += -16.333334f;
            data->scale = data->scaleBase*(1.0f-((float)data->time/data->maxTime));
            if(data->time++ >= data->maxTime) {
                data->maxTime = -1;
            }
        }
    }
}
