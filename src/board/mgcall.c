#include "game/board/mgcall.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/capevent.h"
#include "game/board/exit.h"
#include "game/board/guide.h"
#include "game/board/math.h"
#include "game/board/object.h"
#include "game/board/pause.h"
#include "game/board/player.h"
#include "game/board/coin.h"
#include "game/board/single.h"
#include "game/board/status.h"
#include "game/board/telop.h"
#include "game/board/window.h"
#include "game/board/wipe.h"

#include "game/charman.h"
#include "game/data.h"
#include "game/esprite.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/memory.h"
#include "game/mgdata.h"
#include "game/pad.h"

#include "math.h"
#include "stdio.h"
#include "string.h"

typedef struct MgCallWork_s {
    int type; /* 0x00 */
    s16 sprId[10]; /* 0x04 */
    s16 unk18[4]; /* 0x18 */
    MBOBJID guideObjId; /* 0x20 */
    MBOBJID objId; /* 0x22 */
    s16 picSprId[3]; /* 0x24 */
    s16 frameSprId[3]; /* 0x2A */
    s16 winId[3]; /* 0x30 */
} MGCALL_WORK; /* size 0x38 */

typedef struct MgListWork_s {
    u8 killF : 1;
} MGLISTWORK;

typedef struct MgCallBattlePlayer_s {
    u8 killF : 1;
    s16 cursorSprId;
    s16 charSprId;
    s16 cursorPos;
    s16 cursorPosPrev;
    s16 time;
    s16 maxTime;
    s16 comSelectPos;
    s16 comBtnTime;
} MGCALL_BATTLEPLAYER;

typedef struct MgCallSingleType_s {
    int type;
    int rouletteType;
    unsigned int dataNum;
    int bank;
} MGCALL_SINGLETYPE;

typedef struct MgCallVsEff_s {
    BOOL dispF; /* 0x00 */
    int sprId; /* 0x04 */
    int time; /* 0x08 */
    int maxTime; /* 0x0C */
    float scaleRange; /* 0x10 */
    float scaleBase; /* 0x14 */
    float rotSpeed; /* 0x18 */
    HuVecF pos; /* 0x1C */
    HuVecF vel; /* 0x28 */
    HuVecF scale; /* 0x34 */
    float unk40[2]; /* 0x40 */
    float rot; /* 0x48 */
} MGCALL_VSEFF; /* size 0x4C */

static const HuVec2f statusPos4PBase[GW_PLAYER_MAX] = {
    { 176, 184 },
    { 400, 184 },
    { 176, 296 },
    { 400, 296 },
};

static const HuVec2f statusPos1Vs3Base[GW_PLAYER_MAX] = {
    { 176, 240 },
    { 400, 160 },
    { 400, 240 },
    { 400, 320 },
};

static const HuVec2f statusPos2Vs2Base[GW_PLAYER_MAX] = {
    { 176, 200 },
    { 176, 280 },
    { 400, 200 },
    { 400, 280 },
};

static const HuVec2f statusPosKettouBase[2] = {
    { 152, 240 },
    { 424, 240 },
};

static const u32 battleCharFileTbl[11] = {
    0x00050093,
    0x00050094,
    0x00050095,
    0x00050096,
    0x00050097,
    0x00050098,
    0x00050099,
    0x0005009A,
    0x0005009B,
    0x0005009C,
    0x0005009D,
};

static const u32 battleMgDataTbl[6][3] = {
    { 0x0005002D, 0x0005009E, 0x0005009E },
    { 0x0005002E, 0x0005009F, 0x000500A0 },
    { 0x00050030, 0x000500A1, 0x000500A1 },
    { 0x0005003F, 0x000500A2, 0x000500A3 },
    { 0x0005004F, 0x000500A4, 0x000500A4 },
    { 0x00050050, 0x000500A5, 0x000500A5 },
};

static s16 mgCallHisOfsTbl[9];
static s16 mgCallHis4P[16];
static s16 mgCallHis1Vs3[6];
static s16 mgCallHis2Vs2[6];
static s16 mgCallHisKettou[14];
static HuVec2f mgStatusPos4P[GW_PLAYER_MAX];
static HuVec2f mgStatusPos1Vs3[GW_PLAYER_MAX];
static HuVec2f mgStatusPos2Vs2[GW_PLAYER_MAX];
static OMOBJ *mgListObj[4];

static unsigned int mgCallDataDir = -1;

static s16 mgCallHisBattle[2];
static s16 mgCallFocus;
static OMOBJ *mgCallVsEffOMObj;
static int mgCallSingleKoopaF;

static s16 *mgCallHisPtr[9] = {
    mgCallHis4P,
    mgCallHis1Vs3,
    mgCallHis2Vs2,
    mgCallHisBattle,
    NULL,
    NULL,
    mgCallHisKettou,
    NULL,
    NULL,
};

static int mgCallHisSize[9] = {
    16,
    6,
    6,
    2,
    0,
    0,
    14,
    0,
    0,
};

static unsigned int mgCallTypeFileTbl[4] = {
    0x00050084,
    0x00050084,
    0x00050084,
    0x00050084,
};

static unsigned int guideMotFileTbl[2][16] = {
    { 0x00110001, 0x00110004, 0x00110005, 0x00110008, 0x00110010, 0x00110011, 0x00110012, 0x00110013, 0x00110014, 0xFFFFFFFF },
    { 0x0011001C, 0x0011001F, 0x00110020, 0x00110023, 0x0011002A, 0x0011002B, 0x0011002C, 0x0011002D, 0x0011002E, 0xFFFFFFFF },
};

/* ---- Part 1 (rough, not tuned) ---- */
/* Stubs only: dont_inline keeps part 2 scoring against real calls until part 1 lands. */
#pragma dont_inline on

void mbMgCallInit(void)
{
    int i;
    for(i=0; i<9; i++) {
        mgCallHisOfsTbl[i] = 0;
    }
    memset(mgCallHis4P, 0, sizeof(mgCallHis4P));
    memset(mgCallHis1Vs3, 0, sizeof(mgCallHis1Vs3));
    memset(mgCallHis2Vs2, 0, sizeof(mgCallHis2Vs2));
    memset(mgCallHisBattle, 0, sizeof(mgCallHisBattle));
    memset(mgCallHisKettou, 0, sizeof(mgCallHisKettou));
    memcpy(mgStatusPos4P, statusPos4PBase, sizeof(mgStatusPos4P));
    memcpy(mgStatusPos1Vs3, statusPos1Vs3Base, sizeof(mgStatusPos1Vs3));
    memcpy(mgStatusPos2Vs2, statusPos2Vs2Base, sizeof(mgStatusPos2Vs2));
}

static int MgRouletteExec(int type, MGCALL_WORK *work);
static int MgCallBattleExec(MGCALL_WORK *work);
static int MgRouletteNumGet(int type, int height, s16 *list);

int mbev_MgCall(void)
{
    MGCALL_WORK work;
    int i;
    int mgNo;
    for(i=0; i<10; i++) {
        work.sprId[i] = -1;
    }
    mgNo = MgRouletteExec(0, &work);
    mbObjMotionShiftSet(guideMotFileTbl[0][0], 0, 0, 0, (u32)"itemhook_R");
    if(mgNo < 0) {
        mgNo = MgCallBattleExec(&work);
    }
    for(i=0; i<10; i++) {
        if(work.sprId[i] >= 0) {
            espKill(work.sprId[i]);
        }
    }
    HuPrcSleep(-1);
    return mgNo;
}

static int mgCallColorChanceTbl[3] = { 60, 10, 30 };
static int mgCallColorChanceTbl2[3] = { 30, 40, 30 };

static void SetupTeam(int *teamTbl)
{
    int i;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        if(teamTbl[i] == 1) {
            mbStatusPosOnGet(i, (HuVecF *)&mgStatusPos4P[i]);
            mbStatusMoveSet(i, NULL, (HuVecF *)&mgStatusPos4P[i], 1, 15);
        } else {
            mbStatusPosOnGet(i, (HuVecF *)&mgStatusPos2Vs2[i]);
            mbStatusMoveSet(i, NULL, (HuVecF *)&mgStatusPos2Vs2[i], 1, 15);
        }
        mbStatusColorSet(i, teamTbl[i]);
    }
    for(i=0; i<GW_PLAYER_MAX; i++) {
        espPosSet((s16)teamTbl[i], 1, 2);
        espScaleSet((s16)teamTbl[i], 1, 2);
        espZRotSet((s16)teamTbl[i], 3);
        espTPLvlSet((s16)teamTbl[i], 4);
        espBankSet((s16)teamTbl[i], 5);
        espDispOn((s16)teamTbl[i]);
        espDispOff((s16)teamTbl[i]);
        espDrawNoSet((s16)teamTbl[i], 6);
        mbStatusColorSet(i, 7);
        mbAudFXPlay(i);
        mbAudGuidePlay(i);
        mbWinTopWait();
    }
    while(!mbStatusOffCheckAll()) {
        HuPrcVSleep();
    }
    HuPrcSleep(10);
}

static int SetupMgType(int *color)
{
    int i;
    int type = 0;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        if(color[i] == 1) {
            type++;
        }
        mbStatusColorSet(i, color[i]);
    }
    while(!mbStatusOffCheckAll()) {
        HuPrcVSleep();
    }
    return type;
}

static int MgRouletteExec(int type, MGCALL_WORK *work)
{
    int i;
    int num;
    s16 list[128];
    num = MgRouletteNumGet(type, 3, list);
    for(i=0; i<GW_PLAYER_MAX; i++) {
        espPosSet(work->sprId[i], 1, 2);
        espScaleSet(work->sprId[i], 1, 2);
        espZRotSet(work->sprId[i], 3);
        espTPLvlSet(work->sprId[i], 4);
        espBankSet(work->sprId[i], 5);
        espDispOn(work->sprId[i]);
        espDispOff(work->sprId[i]);
        espDrawNoSet(work->sprId[i], 6);
        mbStatusColorSet(i, 7);
        mbAudFXPlay(i);
        mbAudGuidePlay(i);
        mbWinTopWait();
    }
    for(i=0; i<num; i++) {
        mbAudFXPlay(0);
        HuPrcVSleep();
        espBankSet(work->sprId[0], i);
    }
    while(!mbStatusOffCheckAll()) {
        HuPrcVSleep();
    }
    mbWinTopWait();
    return list[mbRandMod(num)];
}

static s16 MgNameColorGet(u32 nameMes)
{
    if(nameMes == 0) {
        return 0;
    }
    return 1;
}

static BOOL MgCallHisCheck(int type, s16 mgNo);

static int MgRouletteNumGet(int type, int height, s16 *list)
{
    static int posYTbl[5][4] = {
        { 0, 0, 0, 0 },
        { 248, 0, 0, 0 },
        { 224, 272, 0, 0 },
        { 208, 248, 288, 0 },
        { 203, 233, 263, 293 },
    };
    static int defaultHeightTbl[9] = { 4, 3, 3, 3, 3, 3, 3, 3, 3 };
    int i;
    int num;
    MG_DATA *mgData;
    for(mgData=MgDataTbl, i=0, num=0; mgData->ovl != 0xFFFF; mgData++, i++) {
        if(mgData->type != type) {
            continue;
        }
        if(MgCallHisCheck(type, i+601)) {
            continue;
        }
        list[num++] = i;
    }
    if(num > height) {
        num = height;
    }
    list[0] = posYTbl[num][type]+defaultHeightTbl[type]+mgCallColorChanceTbl[type]+mgCallColorChanceTbl2[type];
    for(i=0; i<GW_PLAYER_MAX; i++) {
        espPosSet(list[i], 1, 2);
        espScaleSet(list[i], 1, 2);
        espZRotSet(list[i], 3);
        espTPLvlSet(list[i], 4);
        espBankSet(list[i], 5);
        espDispOn(list[i]);
        espDispOff(list[i]);
        espDrawNoSet(list[i], 6);
        mbStatusColorSet(i, 7);
        mbAudFXPlay(i);
        mbAudGuidePlay(i);
        mbWinTopWait();
    }
    HuPrcVSleep();
    mbWinTopWait();
    return num;
}

static int MgRouletteNumSingleGet(int type, int height, s16 *list)
{
    return MgRouletteNumGet(type, height, list);
}

static int MgRouletteNumMicGet(int type, int height, s16 *list)
{
    return MgRouletteNumGet(type, height, list);
}

static int MgRouletteNumMicSingleGet(int type, int height, s16 *list)
{
    return MgRouletteNumGet(type, height, list);
}

static void MgRouletteOMExec(OMOBJ *obj)
{
    if(mbExitCheck()) {
        omDelObjEx(mbObjMan, obj);
    }
}

static void MgRouletteSlide(OMOBJ *obj)
{
    obj->work[1] = 1;
}

static void MgRouletteFocus(OMOBJ *obj)
{
    obj->work[2] = 1;
}

static BOOL MgRouletteSlideCheck(OMOBJ *obj)
{
    return obj->work[1];
}

void mbMgRouletteFocusKill(u8 arg0)
{
    mgCallFocus = arg0;
}

#pragma dont_inline reset

/* ---- Part 2 ---- */

static inline BOOL mbTeamFGet(void)
{
    return GwSystem.tagF;
}

static int MgCallBattleCoinGet(void);
static int MgCallBattleSelectExec(MGCALL_WORK *work, int listNum, s16 *list);
static u32 MgCallBattleMesGet(u32 mes);

static int MgCallBattleExec(MGCALL_WORK *work)
{
    int i;
    int coinType;
    int coinTotal;
    int delay;
    int coinNum;
    int speed;
    int nextMaxTime;
    int nextTime;
    BOOL coinHighF;
    int result;
    int battleCoin;
    BOOL fxPlayF;
    int nextDelay;
    int listNum;
    HuVecF pos;
    s16 mgTbl[128];
    char str[32];
    HuVecF guidePos;
    HuVecF guideEnd;
    HuVecF guideCur;
    BOOL fastFocusF;
    int battleCoinOfs;
    s16 playerMaxSteal;
    s16 teamMaxSteal;
    float weight;
    float scale;
    float posY;
    float zRot;
    float velY;
    float t;
    float rotY;

    static int battleCoinTbl[5] = {
        5,
        10,
        20,
        30,
        50
    };
    static int battleCoinChanceTbl[5] = {
        6,
        6,
        3,
        3,
        1
    };
    static unsigned int battleMgSprTbl[6] = {
        0x00010000,
        0x00010000,
        0x00010001,
        0x00010002,
        0x00010003,
        0x00010004
    };

    fxPlayF = FALSE;
    espKill(work->sprId[2]);
    work->sprId[2] = -1;
    mbMusFadeOutSpeed(0, 1000);
    while(mbMusCheck(0)) {
        HuPrcVSleep();
    }
    mbMusPlay(0, 25, 127, 0);
    work->type = 3;
    work->sprId[1] = espEntry(mbBoardDataNumGet(0x50084), 95, 3);
    mbAudFXPlay(0x45B);
    posY = 88;
    velY = 6;
    zRot = 0;
    i = 0;
    do {
        weight = i/30.0f;
        if(weight > 1) {
            weight = 1;
        }
        espPosSet(work->sprId[1], 288, 88-(120*HuSin(90*(1-weight))));
        if(weight >= 0.6f) {
            if(!fxPlayF) {
                mbAudFXPlay(0x45C);
                fxPlayF = TRUE;
            }
            if(zRot > -40) {
                zRot -= 1;
            }
            posY += velY;
            velY += 0.2f;
            espPosSet(work->sprId[0], 288, posY);
            espZRotSet(work->sprId[0], zRot);
        }
        HuPrcVSleep();
        i++;
    } while(posY < 576);
    pos.x = 288;
    pos.y = 240;
    pos.z = 800;
    if(!mbTeamFGet()) {
        for(i=0; i<GW_PLAYER_MAX; i++) {
            mbStatusColorSet(i, 1);
        }
        for(i=0; i<GW_PLAYER_MAX; i++) {
            mbStatusPosOnGet(i, &pos);
            mbStatusMoveSet(i, NULL, &pos, 1, 12);
        }
    } else {
        for(i=0; i<GW_PLAYER_MAX/2; i++) {
            mbStatusPosOnGet(i, &pos);
            mbStatusNoMoveSet(i, NULL, &pos, 1, 12);
        }
    }
    while(!mbStatusOffCheckAll()) {
        HuPrcVSleep();
    }
    HuPrcSleep(12);
    mbObjMotionShiftSet(work->guideObjId, 2, 0, 8, 0x40000001);
    mbAudGuidePlay(0x3B6);
    mbAudFXPlay(0);
    mbWinCreate(2, MgCallBattleMesGet(0x400000), mbGuideSpeakerNoGet());
    mbWinTopWait();
    work->sprId[7] = espEntry(mbBoardDataNumGet(0x5008F), 100, 0);
    work->sprId[8] = espEntry(mbBoardDataNumGet(0x50090), 99, 0);
    for(i=0; i<30; i++) {
        weight = i/30.0f;
        pos.x = 288;
        pos.y = 516-(224*weight);
        espPosSet(work->sprId[7], pos.x, pos.y);
        espPosSet(work->sprId[8], 16+pos.x, pos.y);
        espPosSet(work->sprId[1], 288, 88-(120*HuSin(90*weight)));
        HuPrcVSleep();
    }
    fastFocusF = frand() < 0.1f;
    coinType = 0;
    battleCoin = MgCallBattleCoinGet();
    delay = mbRandMod(30)+60;
    battleCoinOfs = (int)((-5+sqrtf(25+(8.0f*delay)))*0.5f);
    speed = (battleCoin-battleCoinOfs)%5;
    if(fastFocusF) {
        speed--;
    }
    if(speed < 0) {
        speed += 5;
    }
    nextDelay = (speed*3)+45;
    nextTime = nextMaxTime = 3;
    for(i=0; i<nextDelay+delay; i++) {
        if(--nextTime == 0) {
            coinType = (coinType+1)%5;
            if(i > nextDelay) {
                nextMaxTime++;
                nextTime = nextMaxTime;
            } else {
                nextTime = nextMaxTime;
            }
            mbAudFXPlay(0x3F1);
        }
        espBankSet(work->sprId[8], coinType);
        HuPrcVSleep();
    }
    coinType = battleCoin;
    espBankSet(work->sprId[8], coinType);
    mbAudFXPlay(0x46E);
    for(i=0; i<90; i++) {
        weight = i/90.0f;
        scale = 1+(0.2f*HuSin(1080*(1-weight)));
        espScaleSet(work->sprId[8], scale, scale);
        espPosSet(work->sprId[8], pos.x+(16*scale), pos.y);
        HuPrcVSleep();
    }
    coinNum = battleCoinTbl[coinType];
    coinTotal = 0;
    coinHighF = TRUE;
    if(!mbTeamFGet()) {
        for(i=0; i<GW_PLAYER_MAX; i++) {
            if(mbPlayerCoinGet(i) >= coinNum) {
                coinTotal += coinNum;
                GwPlayer[i].coinBattle = coinNum;
            } else {
                coinTotal += mbPlayerCoinGet(i);
                coinHighF = FALSE;
                playerMaxSteal = mbPlayerCoinGet(i);
                GwPlayer[i].coinBattle = playerMaxSteal;
            }
        }
    } else {
        for(i=0; i<GW_PLAYER_MAX/2; i++) {
            if(mbPlayerTeamCoinGet(i) >= coinNum*2) {
                coinTotal += coinNum*2;
                GwPlayer[i].coinBattle = coinNum*2;
            } else {
                coinTotal += mbPlayerTeamCoinGet(i);
                coinHighF = FALSE;
                teamMaxSteal = mbPlayerTeamCoinGet(i);
                GwPlayer[i].coinBattle = teamMaxSteal;
            }
        }
    }
    mbCoinAddAllExec(-battleCoinTbl[coinType], -battleCoinTbl[coinType], -battleCoinTbl[coinType], -battleCoinTbl[coinType]);
    for(i=1; i<12U; i++) {
        scale = HuCos(90*(i/12.0f));
        espScaleSet(work->sprId[7], scale, scale);
        espScaleSet(work->sprId[8], scale, scale);
        espPosSet(work->sprId[8], pos.x+(16*scale), pos.y);
        HuPrcVSleep();
    }
    espDispOff(work->sprId[7]);
    espDispOff(work->sprId[8]);
    if(coinHighF) {
        mbAudGuidePlay(0x3B5);
        mbWinCreate(2, MgCallBattleMesGet(0x400001), mbGuideSpeakerNoGet());
    } else {
        mbAudGuidePlay(0x3BA);
        mbWinCreate(2, MgCallBattleMesGet(0x400002), mbGuideSpeakerNoGet());
    }
    sprintf(str, "%d", coinTotal);
    mbWinTopInsertMesSet(MESSNUM_PTR(str), 0);
    mbWinTopWait();
    listNum = MgRouletteNumGet(3, 3, mgTbl);
    espDispOff(work->sprId[7]);
    espDispOff(work->sprId[8]);
    result = MgCallBattleSelectExec(work, listNum, mgTbl);
    if(work->guideObjId >= 0) {
        mbObjPosGet(work->guideObjId, &guidePos);
        guideEnd.x = guidePos.x-200;
        guideEnd.y = guidePos.y;
        guideEnd.z = guidePos.z;
        mbObjMotionShiftSet(work->guideObjId, 3, 0, 8, 0);
        for(i=1; i<=45U; i++) {
            t = i/45.0f;
            if(t <= 0.25f) {
                rotY = -90*HuSin(90*(4*t));
            } else if(t < 0.75) {
                rotY = -90;
            } else {
                rotY = -90*HuCos(90*(4*(t-0.75f)));
            }
            mbev_CapVecChase(&guidePos, &guideEnd, &guideCur, HuSin(90*t));
            mbObjPosSetV(work->guideObjId, &guideCur);
            mbObjRotSet(work->guideObjId, 0, rotY, 0);
            HuPrcVSleep();
        }
        mbObjMotionShiftSet(work->guideObjId, 2, 0, 8, 0x40000001);
    }
    mbAudGuidePlay(0x3B8);
    mbWinCreate(2, MgCallBattleMesGet(0x400003), mbGuideSpeakerNoGet());
    mbWinTopWait();
    _SetFlag(0x10002);
    mbCameraEyeGet(&pos);
    mbCameraMovePos(&pos, NULL, NULL, 80, -1, 60);
    return mgTbl[result];
}

static HuVec2f mgBattlePosTbl[4][3] = {
    { { 0, 0 }, { 0, 0 }, { 0, 0 } },
    { { 288, 240 }, { 0, 0 }, { 0, 0 } },
    { { 168, 240 }, { 408, 240 }, { 0, 0 } },
    { { 118, 240 }, { 288, 240 }, { 458, 240 } },
};

static HuVec2f mgBattleCursorOfsTbl[GW_PLAYER_MAX] = {
    { -56, -16 },
    { -38, -48 },
    { 6, -48 },
    { 24, -16 },
};

static int MgCallBattleSelectExec(MGCALL_WORK *work, int listNum, s16 *list)
{
    MGCALL_BATTLEPLAYER *battlePlayer;
    int i;
    u16 btn;
    int j;
    int cursorDupeNum;
    int ignoreNum;
    int selectNum;
    int result;
    int helpWinId;
    s16 playerDoneNum;
    u16 padNo;
    MBCAMERA *cameraP;
    MGCALL_BATTLEPLAYER playerAll[GW_PLAYER_MAX];
    HuVec2f winScale[3];
    HuVec2f winOfs[3];
    int ignoreTbl[3];
    int selectTbl[3];
    s16 cursorNumTbl[3];
    HuVec2f size;
    HuVec2f scale;
    HuVec2f pos;
    HuVec2f framePos;
    HuVec2f winPos;
    HuVec2f posNew;
    float weight;
    float t;
    float posX;
    float posY;
    float scaleX;
    float moveWeight;

    cameraP = mbCameraGet();
    for(i=0; i<listNum; i++) {
        for(j=0; j<6; j++) {
            if(battleMgDataTbl[j][0] == MgDataTbl[list[i]].nameMes) {
                break;
            }
        }
        if(!GwSystem.curTime) {
            work->picSprId[i] = espEntry(mbBoardDataNumGet(battleMgDataTbl[j][1]), 200, 0);
        } else {
            work->picSprId[i] = espEntry(mbBoardDataNumGet(battleMgDataTbl[j][2]), 200, 0);
        }
        pos.x = mgBattlePosTbl[listNum][i].x;
        pos.y = mgBattlePosTbl[listNum][i].y;
        espPosSet(work->picSprId[i], pos.x, pos.y);
        work->frameSprId[i] = espEntry(mbBoardDataNumGet(0x50091), 150, 0);
        framePos.x = mgBattlePosTbl[listNum][i].x;
        framePos.y = 80+mgBattlePosTbl[listNum][i].y;
        espPosSet(work->frameSprId[i], framePos.x, framePos.y);
        work->winId[i] = mbWinCreateHelp(battleMgDataTbl[j][0]);
        mbWinPriSet(work->winId[i], 120);
        mbWinMesMaxSizeGet(work->winId[i], &size);
        winScale[i].x = winScale[i].y = 1;
        if(size.x > 160) {
            scale.x = 160/size.x;
            scale.y = scale.x;
            mbWinScaleSet(work->winId[i], scale.x, scale.y);
            size.x *= scale.x;
            size.y *= scale.y;
            winScale[i] = scale;
        }
        winOfs[i].x = 2+(-(size.x/2));
        winOfs[i].y = 80-(size.y/2);
        winPos.x = winOfs[i].x+mgBattlePosTbl[listNum][i].x;
        winPos.y = winOfs[i].y+mgBattlePosTbl[listNum][i].y;
        mbWinPosSet(work->winId[i], winPos.x, winPos.y);
    }
    for(j=1; j<=30U; j++) {
        weight = j/30.0f;
        weight = 480*HuCos(90*weight);
        for(i=0; i<listNum; i++) {
            pos.x = mgBattlePosTbl[listNum][i].x;
            pos.y = weight+mgBattlePosTbl[listNum][i].y;
            espPosSet(work->picSprId[i], pos.x, pos.y);
            framePos.x = mgBattlePosTbl[listNum][i].x;
            framePos.y = weight+(80+mgBattlePosTbl[listNum][i].y);
            espPosSet(work->frameSprId[i], framePos.x, framePos.y);
            winPos.x = winOfs[i].x+mgBattlePosTbl[listNum][i].x;
            winPos.y = weight+(winOfs[i].y+mgBattlePosTbl[listNum][i].y);
            mbWinPosSet(work->winId[i], winPos.x, winPos.y);
            mbWinScaleSet(work->winId[i], winScale[i].x, winScale[i].y);
        }
        HuPrcVSleep();
    }
    helpWinId = mbWinCreateHelp(0x400008);
    mbWinMesMaxSizeGet(helpWinId, &size);
    mbWinPosSet(helpWinId, 288-(size.x/2), 340);
    for(battlePlayer=&playerAll[0], i=0; i<GW_PLAYER_MAX; i++, battlePlayer++) {
        battlePlayer->cursorSprId = espEntry(mbBoardDataNumGet(0x50092), (i*2)+81, 0);
        battlePlayer->charSprId = espEntry(mbBoardDataNumGet(battleCharFileTbl[GwPlayer[i].charNo]), (i*2)+80, 0);
        battlePlayer->killF = FALSE;
        battlePlayer->cursorPos = battlePlayer->cursorPosPrev = listNum/2;
        battlePlayer->time = 0;
        battlePlayer->maxTime = 18;
        battlePlayer->comSelectPos = mbRandMod(listNum);
        battlePlayer->comBtnTime = 120+(60*frandf());
        espPosSet(battlePlayer->cursorSprId, mgBattleCursorOfsTbl[i].x+mgBattlePosTbl[listNum][battlePlayer->cursorPos].x, mgBattleCursorOfsTbl[i].y+mgBattlePosTbl[listNum][battlePlayer->cursorPos].y);
        espPosSet(battlePlayer->charSprId, mgBattleCursorOfsTbl[i].x+mgBattlePosTbl[listNum][battlePlayer->cursorPos].x, (mgBattleCursorOfsTbl[i].y+mgBattlePosTbl[listNum][battlePlayer->cursorPos].y)-6);
    }
    for(i=0; i<listNum; i++) {
        cursorNumTbl[i] = 0;
    }
    HuPrcVSleep(10);
    for(playerDoneNum=0; playerDoneNum<4; ) {
        for(i=0; i<listNum; i++) {
            mbWinScaleSet(work->winId[i], winScale[i].x, winScale[i].y);
        }
        for(battlePlayer=&playerAll[0], i=0; i<GW_PLAYER_MAX; i++, battlePlayer++) {
            if(battlePlayer->killF) {
                continue;
            }
            if(battlePlayer->cursorPos != battlePlayer->cursorPosPrev) {
                moveWeight = (float)(++battlePlayer->time)/battlePlayer->maxTime;
                t = HuSin(90*moveWeight);
                posX = mgBattleCursorOfsTbl[i].x+(mgBattlePosTbl[listNum][battlePlayer->cursorPosPrev].x+(t*(mgBattlePosTbl[listNum][battlePlayer->cursorPos].x-mgBattlePosTbl[listNum][battlePlayer->cursorPosPrev].x)));
                posY = mgBattleCursorOfsTbl[i].y+mgBattlePosTbl[listNum][battlePlayer->cursorPosPrev].y;
                espPosSet(battlePlayer->cursorSprId, posX, posY);
                espPosSet(battlePlayer->charSprId, posX, posY-6);
                if(battlePlayer->time >= battlePlayer->maxTime) {
                    battlePlayer->cursorPosPrev = battlePlayer->cursorPos;
                    battlePlayer->time = 0;
                }
            } else {
                if(GwPlayerConf[i].type == 0) {
                    padNo = GwPlayer[i].padNo;
                    btn = HuPadBtnDown[padNo];
                    if(mbPadStkXGet(padNo) < -20) {
                        btn |= PAD_BUTTON_LEFT;
                    } else if(mbPadStkXGet(padNo) > 20) {
                        btn |= PAD_BUTTON_RIGHT;
                    }
                } else {
                    btn = 0;
                    if(battlePlayer->comBtnTime == 0) {
                        btn = PAD_BUTTON_A;
                    } else {
                        if(frand()%64 == 0) {
                            battlePlayer->comSelectPos = mbRandMod(listNum);
                        }
                        if(battlePlayer->cursorPos != battlePlayer->comSelectPos) {
                            if(battlePlayer->cursorPos > battlePlayer->comSelectPos) {
                                btn |= PAD_BUTTON_LEFT;
                            } else {
                                btn |= PAD_BUTTON_RIGHT;
                            }
                        }
                    }
                }
                if(btn & PAD_BUTTON_LEFT) {
                    if(battlePlayer->cursorPos > 0) {
                        mbAudFXPlay(0);
                        battlePlayer->cursorPos--;
                    }
                }
                if(btn & PAD_BUTTON_RIGHT) {
                    if(battlePlayer->cursorPos < listNum-1) {
                        mbAudFXPlay(0);
                        battlePlayer->cursorPos++;
                    }
                }
                if(btn & PAD_BUTTON_A) {
                    battlePlayer->killF = TRUE;
                    cursorNumTbl[battlePlayer->cursorPos]++;
                    playerDoneNum++;
                    espDispOff(battlePlayer->cursorSprId);
                    espDispOff(battlePlayer->charSprId);
                    mbAudFXPlay(1);
                }
            }
            if(battlePlayer->comBtnTime) {
                battlePlayer->comBtnTime--;
            }
        }
        HuPrcVSleep();
    }
    mbWinKill(helpWinId);
    for(battlePlayer=&playerAll[0], i=0; i<GW_PLAYER_MAX; i++, battlePlayer++) {
        espKill(battlePlayer->cursorSprId);
        espKill(battlePlayer->charSprId);
    }
    ignoreNum = 0;
    selectNum = 0;
    for(i=0; i<listNum; i++) {
        cursorDupeNum = 0;
        for(j=0; j<listNum; j++) {
            if(i != j && cursorNumTbl[i] < cursorNumTbl[j]) {
                cursorDupeNum++;
            }
        }
        if(cursorDupeNum == 0) {
            ignoreTbl[ignoreNum++] = i;
        } else {
            selectTbl[selectNum++] = i;
        }
    }
    if(ignoreNum == 1) {
        result = ignoreTbl[0];
    } else {
        result = selectTbl[mbRandMod(selectNum)];
    }
    for(i=0; i<listNum; i++) {
        if(i != result) {
            mbWinPosSet(work->winId[i], 0, 480);
        }
    }
    for(j=1; j<=60U; j++) {
        weight = j/60.0f;
        t = HuCos(90*weight);
        scaleX = HuCos(720*weight);
        for(i=0; i<listNum; i++) {
            if(i != result) {
                espScaleSet(work->picSprId[i], t*scaleX, t);
                espScaleSet(work->frameSprId[i], scaleX*t, t);
            }
        }
        HuPrcVSleep();
    }
    for(i=0; i<listNum; i++) {
        if(i != result) {
            espDispOff(work->picSprId[i]);
            espDispOff(work->frameSprId[i]);
        }
    }
    for(j=1; j<=30U; j++) {
        weight = j/30.0f;
        t = HuSin(90*weight);
        i = result;
        posNew.x = mgBattlePosTbl[listNum][i].x+(t*(mgBattlePosTbl[1][0].x-mgBattlePosTbl[listNum][i].x));
        posNew.y = mgBattlePosTbl[listNum][i].y+(t*(mgBattlePosTbl[1][0].y-mgBattlePosTbl[listNum][i].y));
        pos.x = posNew.x;
        pos.y = posNew.y;
        espPosSet(work->picSprId[i], pos.x, pos.y);
        framePos.x = posNew.x;
        framePos.y = 80+posNew.y;
        espPosSet(work->frameSprId[i], framePos.x, framePos.y);
        winPos.x = posNew.x+winOfs[i].x;
        winPos.y = posNew.y+winOfs[i].y;
        mbWinPosSet(work->winId[i], winPos.x, winPos.y);
        HuPrcVSleep();
    }
    return result;
}

static BOOL MgCallHisCheck(int type, s16 mgNo)
{
    s16 i;
    if(mgCallHisPtr[type] == NULL) {
        return FALSE;
    }
    for(i=0; i<mgCallHisSize[type]; i++) {
        if(mgNo == mgCallHisPtr[type][i]) {
            return TRUE;
        }
    }
    return FALSE;
}

static int MgCallBattleCoinGet(void)
{
    static int maxTurnTbl[9] = {
        10,
        15,
        20,
        25,
        30,
        35,
        40,
        45,
        50
    };
    static int turnTbl[9][2] = {
        { 3, 6 },
        { 5, 10 },
        { 5, 15 },
        { 8, 16 },
        { 10, 20 },
        { 10, 20 },
        { 13, 26 },
        { 15, 30 },
        { 15, 35 }
    };
    static int totalCoinTbl[3] = {
        20,
        30,
        50
    };
    static int battleCoinTbl[4][3][5] = {
        {
            { 10, 85, 5, 0, 0 },
            { 10, 75, 15, 0, 0 },
            { 10, 65, 25, 0, 0 },
        },
        {
            { 5, 70, 20, 5, 0 },
            { 5, 60, 25, 10, 0 },
            { 5, 50, 30, 15, 0 },
        },
        {
            { 5, 60, 20, 10, 5 },
            { 5, 45, 30, 15, 5 },
            { 5, 35, 30, 20, 10 },
        },
        {
            { 5, 45, 30, 15, 5 },
            { 5, 30, 35, 20, 10 },
            { 5, 20, 35, 25, 15 },
        },
    };
    int i;
    int maxTurnNo;
    int turnNo;
    int coinNo;
    int maxCoin;
    int total;
    int chance;
    int j;
    int chanceTbl[5];

    for(i=0; i<8U; i++) {
        if(GwSystem.turnMax <= maxTurnTbl[i]) {
            break;
        }
    }
    maxTurnNo = i;
    for(i=0; i<2; i++) {
        if(GwSystem.turnNo <= turnTbl[maxTurnNo][i]) {
            break;
        }
    }
    turnNo = i;
    maxCoin = mbPlayerMaxCoinGet();
    for(j=0; j<3; j++) {
        if(maxCoin < totalCoinTbl[j]) {
            break;
        }
    }
    coinNo = j;
    total = 0;
    for(j=0; j<5; j++) {
        total += battleCoinTbl[coinNo][turnNo][j];
        chanceTbl[j] = total;
    }
    chance = mbRandMod(total);
    for(j=0; j<4; j++) {
        if(chance < chanceTbl[j]) {
            break;
        }
    }
    return j;
}

static int MgRouletteExec(int type, MGCALL_WORK *work);
static void MgCallCallMg(int type, int mgNo);

void mbev_MgCallKettou(void)
{
    int i;
    int sprId;
    int mgNo;
    float weight;
    _SetFlag(0x10010);
    mbPauseDisableSet(TRUE);
    sprId = espEntry(mbBoardDataNumGet(mgCallTypeFileTbl[3]), 100, 0);
    espPosSet(sprId, 288, -32);
    espBankSet(sprId, 4);
    for(i=0; i<30U; i++) {
        weight = i/30.0f;
        espPosSet(sprId, 288, 88-(120*HuSin(90*(1-weight))));
        HuPrcVSleep();
    }
    _SetFlag(0x10005);
    mgNo = MgRouletteExec(6, NULL);
    MgCallCallMg(6, mgNo);
    espKill(sprId);
    HuPrcSleep(-1);
}

void mbev_MgCallDonkey(void)
{
    int i;
    int sprId;
    int mgNo;
    float weight;
    mbPauseDisableSet(TRUE);
    sprId = espEntry(mbBoardDataNumGet(0x50086), 95, 0);
    espPosSet(sprId, 288, -32);
    espDrawNoSet(sprId, 32);
    for(i=0; i<30U; i++) {
        weight = i/30.0f;
        espPosSet(sprId, 288, 88-(120*HuSin(90*(1-weight))));
        HuPrcVSleep();
    }
    _SetFlag(0x10006);
    mgNo = MgRouletteExec(7, NULL);
    MgCallCallMg(7, mgNo);
    HuPrcSleep(-1);
}

void mbev_MgCallKoopa(void)
{
    int i;
    int sprId;
    int mgNo;
    float weight;
    mbPauseDisableSet(TRUE);
    sprId = espEntry(mbBoardDataNumGet(0x50085), 95, 0);
    espPosSet(sprId, 288, -32);
    espDrawNoSet(sprId, 32);
    for(i=0; i<30U; i++) {
        weight = i/30.0f;
        espPosSet(sprId, 288, 88-(120*HuSin(90*(1-weight))));
        HuPrcVSleep();
    }
    _SetFlag(0x10003);
    mgNo = MgRouletteExec(4, NULL);
    MgCallCallMg(4, mgNo);
    HuPrcSleep(-1);
}

void mbev_MgCallSingle(int type)
{
    mbev_MgCallSingleKoopa(type, FALSE);
}

static void MgRouletteCreate(MGCALL_WORK *work);

static MGCALL_SINGLETYPE mgCallSingleTypeTbl[7] = {
    { 0, 0, 0x00050084, 0 },
    { 1, 1, 0x00050084, 1 },
    { 2, 2, 0x00050084, 2 },
    { 3, 3, 0x00050084, 3 },
    { 6, 6, 0x00050084, 4 },
    { 4, 4, 0x00050085, 0 },
    { -1, -1, -1, -1 },
};

void mbev_MgCallSingleKoopa(int type, int koopaF)
{
    int i;
    MGCALL_SINGLETYPE *typeP;
    int sprId;
    MG_DATA *mgData;
    int no;
    MGCALL_WORK *workP;
    int teamChar;
    int num;
    int mgNo;
    int a;
    int b;
    int mgType;
    int temp;
    float weight;
    MGCALL_WORK work;
    int list[16];
    int charTbl[3];

    mgData = MgDataTbl;
    typeP = mgCallSingleTypeTbl;
    workP = &work;
    sprId = -1;
    teamChar = -1;
    MgRouletteCreate(workP);
    _SetFlag(0x10010);
    mgCallSingleKoopaF = koopaF;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        GwPlayerConf[i].comDif = GWStoryComDifGet();
    }
    if(type == 2) {
        teamChar = mbSingleTeamCharGet();
        GwPlayerConf[1].charNo = teamChar;
        GwPlayerConf[1].comDif = 2;
        charTbl[0] = 11;
        charTbl[1] = 12;
        charTbl[2] = 13;
        for(i=0; i<64; i++) {
            a = i%3;
            b = mbRandMod(3);
            if(a != b) {
                temp = charTbl[a];
                charTbl[a] = charTbl[b];
                charTbl[b] = temp;
            }
        }
        GwPlayerConf[2].charNo = charTbl[0];
        GwPlayerConf[3].charNo = charTbl[1];
    } else {
        GwPlayerConf[1].charNo = 11;
        GwPlayerConf[2].charNo = 12;
        GwPlayerConf[3].charNo = 13;
    }
    switch(type) {
        case 0:
            for(i=0; i<GW_PLAYER_MAX; i++) {
                GwPlayerConf[i].grpNo = i;
            }
            break;

        case 1:
            GwPlayerConf[0].grpNo = 0;
            GwPlayerConf[1].grpNo = 1;
            GwPlayerConf[2].grpNo = 1;
            GwPlayerConf[3].grpNo = 1;
            break;

        case 2:
            GwPlayerConf[0].grpNo = 0;
            GwPlayerConf[1].grpNo = 0;
            GwPlayerConf[2].grpNo = 1;
            GwPlayerConf[3].grpNo = 1;
            break;

        case 3:
            for(i=0; i<GW_PLAYER_MAX; i++) {
                GwPlayerConf[i].grpNo = i;
            }
            break;

        case 4:
            for(i=0; i<GW_PLAYER_MAX; i++) {
                GwPlayerConf[i].grpNo = i;
            }
            break;

        case 6:
            for(i=0; i<GW_PLAYER_MAX; i++) {
                GwPlayerConf[i].grpNo = 2;
            }
            GwPlayerConf[0].grpNo = 0;
            GwPlayerConf[1].grpNo = 1;
            GwPlayerConf[1].charNo = mbSingleOppCharGet();
            break;

        case -1:
            for(i=0; i<GW_PLAYER_MAX; i++) {
                GwPlayerConf[i].grpNo = i;
            }
            break;
    }
    if(type == -1) {
        switch(mbBoardNoGet()) {
            case 6:
                no = 1;
                break;

            case 7:
                no = 2;
                break;

            default:
                no = 3;
                break;
        }
        for(i=0, num=0; mgData->ovl != 0xFFFF; i++, mgData++) {
            if(mgData->flag & 0x80) {
                list[num] = i;
                num++;
            }
        }
        if(num < no) {
            mgNo = 0;
        } else {
            mgNo = list[no-1];
        }
        mgType = MgDataTbl[no].type;
    } else {
        for(i=0; i<5; i++, typeP++) {
            if(typeP->type == type) {
                break;
            }
        }
        mbPauseDisableSet(TRUE);
        sprId = espEntry(mbBoardDataNumGet(typeP->dataNum), 100, 0);
        workP->sprId[0] = sprId;
        espPosSet(sprId, 288, -32);
        espBankSet(sprId, typeP->bank);
        for(i=0; i<30U; i++) {
            weight = i/30.0f;
            espPosSet(sprId, 288, 88-(120*HuSin(90*(1-weight))));
            HuPrcVSleep();
        }
        mgNo = MgRouletteExec(typeP->rouletteType, workP);
        mgType = typeP->rouletteType;
    }
    GwSystem.curTime = 0;
    GwMgTime = 0;
    MgCallCallMg(mgType, mgNo);
    mbDirClose();
    if(teamChar != -1) {
        CharMotionInit(teamChar);
    }
    if(sprId != -1) {
        espKill(sprId);
    }
    HuPrcSleep(-1);
}

static inline BOOL mbTimeDayCheck(void)
{
    return GwSystem.curTime == 0;
}

static inline void mbMgTimeSet(int time)
{
    GwMgTime = time;
}

static void MgCallCallMg(int type, int mgNo)
{
    int mgId;
    s32 statId;
    int dataNum;

    mgId = mgNo+601;
    GWMgUnlockSet(mgId);
    GwSystem.mgNo = mgId-601;
    if(mgCallHisPtr[type] != NULL) {
        mgCallHisPtr[type][mgCallHisOfsTbl[type]] = mgId;
        if(++mgCallHisOfsTbl[type] >= mgCallHisSize[type]) {
            mgCallHisOfsTbl[type] = 0;
        }
    }
    dataNum = 0x260000;
    mgCallDataDir = MgDataTbl[mgNo].dataDir;
    mbDirClose();
    statId = mbBGRead(dataNum);
    if(!GwSystem.curTime || GWPartyGet() == FALSE) {
        mbWipeCreate(2, 0x8B, 60);
    } else {
        mbWipeCreate(2, 0x8A, 60);
    }
    mbMusFadeOutSpeed(0, 1000);
    mbMusFadeOutSpeed(1, 1000);
    mbAudFXStopAll(1000);
    mbBGReadWait(statId);
    mbWipeSpecialWait();
    mbMgTimeSet(mbTimeDayCheck() ? 0 : 1);
    mbOvlCall(4);
}

void mbMgCallDataClose(void)
{
    if(mgCallDataDir != -1) {
        HuDataDirClose(mgCallDataDir);
    }
}

void mbev_MgCallTutorial(void)
{
    MGCALL_WORK *workP;
    MBCAMERA *cameraP;
    int i;
    s16 winId;
    MGLISTWORK *listWork;
    OMOBJ *obj;
    float weight;
    float scale;
    MGCALL_WORK work;
    Mtx matrix;
    HuVecF pos;
    HuVecF posPrev;
    HuVecF pos2D;
    HuVecF pos3D;
    int teamTbl[4];

    workP = &work;
    cameraP = mbCameraGet();
    mbWipeDissolveFadeIn();
    mbAudFXPlay(0);
    mbWinCreate(5, 0x2A002D, -1);
    mbWinTopWait();
    mbWinCreate(5, 0x2A002E, -1);
    mbWinTopWait();
    pos.x = pos.y = pos.z = 0;
    posPrev = pos;
    for(i=0; i<=12U; i++) {
        weight = i/12.0f;
        posPrev.y = pos.y+(weight*(119-pos.y));
        HuPrcVSleep();
    }
    for(i=0; i<GW_PLAYER_MAX; i++) {
        mgStatusPos4P[i].y += 64;
        mgStatusPos1Vs3[i].y += 64;
        mgStatusPos2Vs2[i].y += 64;
    }
    for(i=0; i<GW_PLAYER_MAX; i++) {
        mbStatusPosOnGet(i, &pos);
        mbStatusMoveSet(i, &pos, (HuVecF *)&mgStatusPos4P[i], 1, 15);
    }
    while(!mbStatusOffCheckAll()) {
        HuPrcVSleep();
    }
    mbWinCreate(2, 0x2A002F, -1);
    mbWinTopWait();
    for(i=0; i<GW_PLAYER_MAX; i++) {
        teamTbl[i] = 1;
    }
    SetupTeam(teamTbl);
    workP->sprId[2] = espEntry(mbBoardDataNumGet(0x5008A), 100, 0);
    espPosSet(workP->sprId[2], 288, 304);
    for(i=0; i<30U; i++) {
        weight = i/30.0f;
        scale = 1.5+HuSin(360*weight);
        if(weight < 0.2f) {
            scale *= weight/0.2f;
        }
        espScaleSet(workP->sprId[2], scale, scale);
        HuPrcVSleep();
    }
    mbWinCreate(2, 0x2A0031, -1);
    mbWinTopWait();
    teamTbl[1] = 2;
    teamTbl[3] = 2;
    SetupTeam(teamTbl);
    mbWinCreate(2, 0x2A0033, -1);
    mbWinTopWait();
    mbWinCreate(2, 0x2A0034, -1);
    mbWinTopWait();
    teamTbl[0] = 2;
    SetupTeam(teamTbl);
    mbWinCreate(2, 0x2A0036, -1);
    mbWinTopWait();
    teamTbl[0] = 1;
    teamTbl[1] = 1;
    SetupTeam(teamTbl);
    mbWinCreate(2, 0x2A0037, -1);
    mbWinTopWait();
    mbWinCreate(2, 0x2A0039, -1);
    mbWinTopWait();
    mbWinCreate(2, 0x2A003A, -1);
    mbWinTopWait();
    workP->sprId[0] = espEntry(mbBoardDataNumGet(mgCallTypeFileTbl[1]), 1500, 0);
    espPosSet(workP->sprId[0], 288, -32);
    espDrawNoSet(workP->sprId[0], 32);
    for(i=0; i<30U; i++) {
        weight = i/30.0f;
        espPosSet(workP->sprId[0], 288, 88-(120*HuSin(90*(1-weight))));
        HuPrcVSleep();
    }
    for(i=0; i<60U; i++) {
        weight = i/60.0f;
        scale = 1.5+(0.8f*HuSin(weight*(720+(720*weight))));
        espScaleSet(workP->sprId[2], scale, scale);
        HuPrcVSleep();
    }
    for(i=0; i<GW_PLAYER_MAX; i++) {
        mbStatusPosGet(i, &pos);
        if(pos.x >= 288) {
            pos.x = 704;
        } else {
            pos.x = -128;
        }
        mbStatusMoveSet(i, NULL, &pos, 1, 15);
    }
    workP->sprId[3] = mbObjCreate(mbBoardDataNumGet(0x5005B), NULL, 0);
    mbObjLayerSet(workP->sprId[3], 6);
    pos2D.x = 288;
    pos2D.y = 304;
    pos2D.z = 500;
    mbPos2Dto3D(&pos2D, &pos3D);
    mbObjPosSetV(workP->sprId[3], &pos3D);
    MTXLookAt(matrix, &cameraP->pos, &cameraP->up, &pos3D);
    MTXInverse(matrix, matrix);
    matrix[0][3] = matrix[1][3] = matrix[2][3] = 0;
    mbObjMtxSet(workP->sprId[3], matrix);
    espKill(workP->sprId[2]);
    workP->sprId[2] = -1;
    mbAudFXPlay(0x459);
    for(i=0; i<30U; i++) {
        weight = i/30.0f;
        scale = 0.4f*((1+(5*weight))+(0.5*HuSin(16*(360*weight))));
        mbObjScaleSet(workP->sprId[3], scale, scale, scale);
        if(i >= 25) {
            mbObjAlphaSet(workP->sprId[3], 255*HuCos(90*((i-25)/5.0f)));
        }
        HuPrcVSleep();
    }
    mbObjKill(workP->sprId[3]);
    workP->sprId[3] = -1;
    while(!mbStatusOffCheckAll()) {
        HuPrcVSleep();
    }
    MgRouletteExec(1, workP);
    winId = mbWinCreate(2, 0x2A003B, -1);
    mbWinWait(winId);
    winId = mbWinCreate(2, 0x2A003C, -1);
    mbWinWait(winId);
    mbWipeDissolveFadeOut();
    for(i=0; i<4; i++) {
        if(mgListObj[i]) {
            obj = mgListObj[i];
            listWork = omObjGetWork(obj, MGLISTWORK);
            listWork->killF = TRUE;
        }
    }
    espKill(workP->sprId[0]);
}

static void MgCallVsEffOMExec(OMOBJ *obj);

static void MgCallVsEffCreate(void)
{
    OMOBJ *obj;
    MGCALL_VSEFF *effP;
    MGCALL_VSEFF *eff;
    int i;
    obj = mgCallVsEffOMObj = omAddObjEx(mbObjMan, 32768, 0, 0, -1, MgCallVsEffOMExec);
    effP = HuMemDirectMallocNum(HEAP_HEAP, 64*sizeof(MGCALL_VSEFF), HU_MEMNUM_OVL);
    obj->data = effP;
    eff = effP;
    memset(eff, 0, 64*sizeof(MGCALL_VSEFF));
    obj->work[0] = obj->work[1] = obj->work[2] = obj->work[3] = 0;
    for(i=0; i<64; i++, eff++) {
        eff->dispF = FALSE;
        eff->sprId = espEntry(mbBoardDataNumGet(0x5008B), 99, i%4);
        espDispOff(eff->sprId);
        eff->time = 0;
        eff->maxTime = 0;
        eff->scaleRange = 1;
        eff->scaleBase = 1;
        eff->rotSpeed = 0;
    }
}

static int MgCallVsEffNumGet(void);

static void MgCallVsEffOMExec(OMOBJ *obj)
{
    MGCALL_VSEFF *eff;
    int i;
    float weight;
    float scale;
    eff = obj->data;
    if(obj->work[3] && MgCallVsEffNumGet() <= 0) {
        mgCallVsEffOMObj = NULL;
    }
    if(mbExitCheck() || mgCallVsEffOMObj == NULL) {
        for(i=0; i<64; i++, eff++) {
            espKill(eff->sprId);
        }
        omDelObjEx(mbObjMan, obj);
        mgCallVsEffOMObj = NULL;
        return;
    }
    for(i=0; i<64; i++, eff++) {
        if(!eff->dispF) {
            continue;
        }
        weight = (float)(++eff->time)/eff->maxTime;
        PSVECAdd(&eff->pos, &eff->vel, &eff->pos);
        eff->rot += eff->rotSpeed;
        scale = eff->scaleBase+(weight*eff->scaleRange);
        espPosSet(eff->sprId, eff->pos.x, eff->pos.y);
        espScaleSet(eff->sprId, eff->scale.x*scale, eff->scale.y*scale);
        espZRotSet(eff->sprId, eff->rot);
        espTPLvlSet(eff->sprId, HuCos(90*weight));
        if(weight >= 1) {
            eff->dispF = FALSE;
            espDispOff(eff->sprId);
        }
    }
}

static void MgCallVsEffPosSet(float x, float y)
{
    OMOBJ *obj;
    MGCALL_VSEFF *eff;
    int i;
    float angle;
    float speed;
    float scale;
    GXColor color;
    obj = mgCallVsEffOMObj;
    eff = obj->data;
    for(i=0; i<64; i++, eff++) {
        if(eff->dispF) {
            continue;
        }
        angle = 360*frandf();
        speed = 15*(0.3f+(0.7f*frandf()));
        eff->dispF = TRUE;
        eff->time = 0;
        eff->maxTime = 60*(0.5f+(0.5f*frandf()));
        eff->scaleRange = 0.5f+(0.5f*frandf());
        eff->scaleBase = 0.2f+(0.2f*frandf());
        eff->rotSpeed = 3*(frandf()-0.5f);
        eff->pos.x = x;
        eff->pos.y = y;
        eff->vel.x = speed*HuSin(angle);
        eff->vel.y = speed*HuCos(angle);
        eff->scale.x = 1+frandf();
        eff->scale.y = 1+frandf();
        eff->rot = frandf();
        mbev_CapEffColorSet(&color, frand()%7);
        scale = eff->scaleBase;
        espPosSet(eff->sprId, eff->pos.x, eff->pos.y);
        espScaleSet(eff->sprId, eff->scale.x*scale, eff->scale.y*scale);
        espZRotSet(eff->sprId, eff->rot);
        espColorSet(eff->sprId, color.r, color.g, color.b);
        espTPLvlSet(eff->sprId, 1);
        espDispOn(eff->sprId);
    }
}

static void MgCallVsEffKill(void)
{
    mgCallVsEffOMObj = NULL;
}

static int MgCallVsEffNumGet(void)
{
    OMOBJ *obj;
    MGCALL_VSEFF *eff;
    int i;
    int num;
    obj = mgCallVsEffOMObj;
    if(mgCallVsEffOMObj == NULL) {
        return 0;
    }
    eff = obj->data;
    for(i=0, num=0; i<64; i++, eff++) {
        if(eff->dispF) {
            num++;
        }
    }
    return num;
}

int mbMgCallVsEffCreate(void)
{
    MgCallVsEffCreate();
    MgCallVsEffPosSet(288, 240);
    mgCallVsEffOMObj->work[3] = 1;
    return 60;
}

int mbMgCallSingleOnCheck(void)
{
    return TRUE;
}

int mbMgRouletteNumGet(int type)
{
    static int defaultHeightTbl[9] = {
        4,
        3,
        3,
        3,
        3,
        3,
        3,
        3,
        3
    };
    int height;
    s16 list[128];
    height = defaultHeightTbl[type];
    height = MgRouletteNumGet(type, height, list);
    return height;
}

static u32 MgCallBattleMesGet(u32 mes)
{
    if(!GwSystem.curTime) {
        return mes;
    }
    return mes+4;
}

static void MgRouletteCreate(MGCALL_WORK *work)
{
    int i;
    memset(work, 0, sizeof(MGCALL_WORK));
    for(i=0; i<10; i++) {
        work->sprId[i] = -1;
    }
    for(i=0; i<3; i++) {
        work->picSprId[i] = -1;
        work->frameSprId[i] = -1;
        work->winId[i] = -1;
    }
    work->guideObjId = -1;
    work->objId = -1;
}

static void MgRouletteKill(MGCALL_WORK *work)
{
    int i;
    for(i=0; i<10; i++) {
        if(work->sprId[i] >= 0) {
            espKill(work->sprId[i]);
        }
    }
    if(work->guideObjId >= 0) {
        mbObjKill(work->guideObjId);
    }
    if(work->objId >= 0) {
        mbObjKill(work->objId);
    }
    for(i=0; i<3; i++) {
        if(work->picSprId[i] >= 0) {
            espKill(work->picSprId[i]);
        }
        if(work->frameSprId[i] >= 0) {
            espKill(work->frameSprId[i]);
        }
        if(work->winId[i] >= 0) {
            mbWinKill(work->winId[i]);
        }
    }
    MgRouletteCreate(work);
}
