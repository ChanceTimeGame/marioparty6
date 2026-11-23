#include "game/board/guide.h"
#include "game/board/coin.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/memory.h"
#include "game/pad.h"
#include "game/board/mgcall.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/capevent.h"
#include "game/board/exit.h"
#include "game/board/masu.h"
#include "game/board/math.h"
#include "game/board/object.h"
#include "game/board/pause.h"
#include "game/board/player.h"
#include "game/board/single.h"
#include "game/board/status.h"
#include "game/board/telop.h"
#include "game/board/window.h"
#include "game/board/wipe.h"

#include "game/esprite.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/mgdata.h"
#include "game/mic.h"
#include "game/object.h"
#include "game/process.h"
#include "game/window.h"

#include "math.h"
#include "string.h"
#include "stdio.h"

typedef struct MgCallWork_s {
    int type; /* 0x00 */
    s16 sprId[10]; /* 0x04 */
    s16 unk18[4]; /* 0x18 */
    MBOBJID guideObjId; /* 0x20 */
    MBOBJID hookObjId; /* 0x22 */
    s16 picSprId[3]; /* 0x24 */
    s16 frameSprId[3]; /* 0x2A */
    s16 winId[3]; /* 0x30 */
} MGCALL_WORK; /* size 0x38 */

typedef struct MgListWork_s {
    unsigned killF : 1;
    unsigned slideF : 1;
    unsigned mode : 3;
    unsigned no : 3;
    unsigned dispF : 1;
    s16 winNo;
    s16 sprId;
    s16 time;
    s16 maxTime;
    int hiddenMes;
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

typedef struct MgListPack_s {
    int no;
    u8 flag;
} MGLISTPACK;

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
    { 424, 240 }
};

static s16 mgCallHisOfsTbl[MG_TYPE_MAX];
static s16 mgCallHis4P[16];
static s16 mgCallHis1Vs3[6];
static s16 mgCallHis2Vs2[6];
static s16 mgCallHisKettou[14];
static HuVec2f mgStatusPos4P[GW_PLAYER_MAX];
static HuVec2f mgStatusPos1Vs3[GW_PLAYER_MAX];
static HuVec2f mgStatusPos2Vs2[GW_PLAYER_MAX];
static OMOBJ *mgListObj[4];

static s16 mgCallHisBattle[2];

static s16 *mgCallHisPtr[MG_TYPE_MAX] = {
    mgCallHis4P,
    mgCallHis1Vs3,
    mgCallHis2Vs2,
    mgCallHisBattle,
    NULL,
    NULL,
    mgCallHisKettou,
    NULL,
    NULL
};

static int mgCallHisSize[MG_TYPE_MAX] = {
    16,
    6,
    6,
    2,
    0,
    0,
    14,
    0,
    0
};

static unsigned int mgCallDataDir = -1;

static int mgCallTypeFileTbl[4] = {
    0x50084,
    0x50084,
    0x50084,
    0x50084
};

static int guideMotFileTbl[2][16] = {
    {
        0x110001,
        0x110004,
        0x110005,
        0x110008,
        0x110010,
        0x110011,
        0x110012,
        0x110013,
        0x110014,
        -1
    },
    {
        0x11001C,
        0x11001F,
        0x110020,
        0x110023,
        0x11002A,
        0x11002B,
        0x11002C,
        0x11002D,
        0x11002E,
        -1
    }
};

static s16 mgCallFocus;
static OMOBJ *mgCallVsEffOMObj;
static BOOL mgCallSingleKoopaF;

static inline int GWPlayerTeamGet(int playerNo)
{
    return GwPlayer[playerNo].team;
}

static inline BOOL GWTeamGet(void)
{
    return GwSystem.tagF;
}

static inline BOOL mbPartyFGet(void)
{
    BOOL partyF = GwSystem.partyF;
    return partyF;
}

static inline int GWMgPackGet(void)
{
    if(GwSystem.mgPack >= 5) {
        GwSystem.mgPack = 0;
    }
    return GwSystem.mgPack;
}

static int SetupTeam(int *colorIn);
static int SetupMgType(int *color);
static int MgRouletteExec(int type, MGCALL_WORK *workP);
static s16 MgNameColorGet(u32 nameMes);
static int MgRouletteNumGet(int type, int height, s16 *noTbl);
static int MgRouletteNumSingleGet(int type, int height, s16 *noTbl);
static int MgRouletteNumMicGet(int type, int height, s16 *noTbl);
static int MgRouletteNumMicSingleGet(int type, int height, s16 *noTbl);
static void MgRouletteOMExec(OMOBJ *obj);
static void MgRouletteSlide(OMOBJ *obj);
static void MgRouletteFocus(OMOBJ *obj);
static BOOL MgRouletteSlideCheck(OMOBJ *obj);
static int MgCallBattleExec(MGCALL_WORK *workP);
static int MgCallHisCheck(int type, s16 no);
static void MgCallCallMg(int type, int no);
static void MgCallVsEffCreate(void);
static void MgCallVsEffPosSet(float posX, float posY);
static void MgCallVsEffKill(void);
static int MgCallVsEffNumGet(void);
static void MgRouletteCreate(MGCALL_WORK *workP);
static void MgRouletteKill(MGCALL_WORK *workP);

void mbMgCallInit(void)
{
    int i;
    for(i=0; i<MG_TYPE_MAX; i++) {
        mgCallHisOfsTbl[i] = 0;
    }
    memset(&mgCallHis4P[0], 0, sizeof(mgCallHis4P));
    memset(&mgCallHis1Vs3[0], 0, sizeof(mgCallHis1Vs3));
    memset(&mgCallHis2Vs2[0], 0, sizeof(mgCallHis2Vs2));
    memset(&mgCallHisBattle[0], 0, sizeof(mgCallHisBattle));
    memset(&mgCallHisKettou[0], 0, sizeof(mgCallHisKettou));
    memcpy(&mgStatusPos4P[0], &statusPos4PBase[0], GW_PLAYER_MAX*sizeof(HuVec2f));
    memcpy(&mgStatusPos1Vs3[0], &statusPos2Vs2Base[0], GW_PLAYER_MAX*sizeof(HuVec2f));
    memcpy(&mgStatusPos2Vs2[0], &statusPos1Vs3Base[0], GW_PLAYER_MAX*sizeof(HuVec2f));
}

int mbev_MgCall(void)
{
    MBCAMERA *cameraP;
    HuVec2f statusPos;
    HuVecF masuPos;
    MGCALL_WORK work;
    MGCALL_WORK *workP = &work;
    int i;
    int masuId;
    int no;
    BOOL battleF = FALSE;
    float t;
    float scale;
    float scaleX;
    float scaleY;

    cameraP = mbCameraGet();
    mbPauseDisableSet(TRUE);
    for(i=0; i<GW_PLAYER_MAX; i++) {
        mbPlayerMotionShiftSet(i, 1, 0, 8, 0x40000001);
    }
    MgRouletteCreate(&work);
    for(i=0; i<GW_PLAYER_MAX; i++) {
        mbStatusCapsuleDispSet(i, FALSE);
    }
    mbStatusDispForceSetAll(TRUE);
    for(i=0; i<GW_PLAYER_MAX; i++) {
        mbPlayerDispSet(i, FALSE);
    }
    if((u8)GwSystem.curTime == 0) {
        workP->guideObjId = mbObjCreate(0x110000, guideMotFileTbl[0], FALSE);
        workP->hookObjId = MB_OBJ_NONE;
    } else {
        workP->guideObjId = mbObjCreate(0x11001B, guideMotFileTbl[1], FALSE);
        workP->hookObjId = mbObjCreate(0x110035, NULL, FALSE);
        mbObjHookSet(workP->guideObjId, "itemhook_R", workP->hookObjId);
    }
    mbObjMotionSet(workP->guideObjId, 2, 0x40000001);
    for(i=1; i<mbMasuNumGet(); i++) {
        if(mbMasuAttrGet(i) & 0x8000) {
            break;
        }
    }
    if(i < mbMasuNumGet()) {
        masuId = i;
    } else {
        masuId = 1;
    }
    mbMasuPosGet(masuId, &masuPos);
    masuPos.y += 250;
    mbObjPosSet(workP->guideObjId, masuPos.x, masuPos.y, masuPos.z);
    mbCameraMoveOnSet(FALSE);
    mbCameraMoveObj(workP->guideObjId, NULL, NULL, 1800, -1, 21);
    mbCameraMoveWait();
    mbCameraFocusObjSet(MB_OBJ_NONE);
    mbWipeFadeIn();
    mbAudFXPlay(0x457);
    if(!GWTeamGet()) {
        for(i=0; i<GW_PLAYER_MAX; i++) {
            mbStatusPosOnGet(i, (HuVecF *)&statusPos);
            mbStatusMoveSet(i, (HuVecF *)&statusPos, (HuVecF *)&statusPos4PBase[i], 1, 15);
        }
    } else {
        for(i=0; i<2; i++) {
            mbStatusPosOnGet(i, (HuVecF *)&statusPos);
            mbStatusNoMoveSet(i, (HuVecF *)&statusPos, (HuVecF *)&statusPosKettouBase[i], 1, 15);
        }
    }
    while(!mbStatusOffCheckAll()) {
        HuPrcVSleep();
    }
    workP->type = SetupTeam(NULL);
    mbObjMotionShiftSet(workP->guideObjId, 9, 0, 8, 0);
    workP->sprId[2] = espEntry(mbBoardDataNumGet(0x5008A), 90, 0);
    espPosSet(workP->sprId[2], 288, 240);
    espScaleSet(workP->sprId[2], 0.5f, 0.5f);
    mbAudFXPlay(0x458);
    for(i=1; i<=30; i++) {
        t = i/30.0f;
        scaleX = (0.5*HuSin(90*t))+(1.5*HuSin(180*t));
        scaleY = (0.5*HuSin(90*t))+(1.5*fabs(HuSin(360*t)));
        espPosSet(workP->sprId[2], 288-(32*(1-t)), 240.0+(64.0*HuSin(180*t)));
        espScaleSet(workP->sprId[2], scaleX, scaleY);
        espZRotSet(workP->sprId[2], 360*-t);
        HuPrcVSleep();
    }
    HuPrcSleep(15);
    workP->sprId[0] = espEntry(mbBoardDataNumGet(mgCallTypeFileTbl[workP->type]), 95, 0);
    espPosSet(workP->sprId[0], 288, -32);
    espDrawNoSet(workP->sprId[0], 32);
    espBankSet(workP->sprId[0], workP->type);
    for(i=0; i<30u; i++) {
        t = i/30.0f;
        espPosSet(workP->sprId[0], 288, 88.0-(120.0*HuSin(90*(1-t))));
        HuPrcVSleep();
    }
    if(workP->type == MG_TYPE_4P && mbRandMod(100) < 10 && mbPlayerMaxCoinGet() >= 10) {
        battleF = TRUE;
        for(i=1; i<=12u; i++) {
            t = i/12.0f;
            scale = 0.5*HuCos(90*t);
            espScaleSet(workP->sprId[2], scale, scale);
            HuPrcVSleep();
        }
        mbAudFXDelaySet(10);
        mbAudGuidePlay(0x3B4);
        mbev_CapPlayerMotShiftSet(workP->guideObjId, 7, 0, 1);
        mbObjMotionShiftSet(workP->guideObjId, 8, 0, 8, 0);
        no = MgCallBattleExec(workP);
    } else {
        for(i=1; i<=12u; i++) {
            t = i/12.0f;
            scale = 0.1f+(0.4f*HuCos(90*t));
            espScaleSet(workP->sprId[2], scale, scale);
            HuPrcVSleep();
        }
        mbObjMotionShiftSet(workP->guideObjId, 4, 0, 8, 0);
        for(i=0; i<GW_PLAYER_MAX; i++) {
            mbStatusPosGet(i, (HuVecF *)&statusPos);
            if(statusPos.x >= 288) {
                statusPos.x = 704;
            } else {
                statusPos.x = -128;
            }
            if(statusPos.y > 272) {
                statusPos.y = 608;
            } else if(statusPos.y < 208) {
                statusPos.y = -128;
            }
            mbStatusMoveSet(i, NULL, (HuVecF *)&statusPos, 1, 15);
        }
        mbAudFXPlay(0x459);
        MgCallVsEffCreate();
        MgCallVsEffPosSet(288, 240);
        for(i=1; i<=30u; i++) {
            t = i/30.0f;
            scale = 0.1f+(3.0*HuSin(90*t));
            espScaleSet(workP->sprId[2], scale, scale);
            espTPLvlSet(workP->sprId[2], 1-t);
            espZRotSet(workP->sprId[2], 360.0*HuSin(90*t));
            HuPrcVSleep();
        }
        espDispOff(workP->sprId[2]);
        mbObjMotionShiftSet(workP->guideObjId, 2, 0, 8, 0x40000001);
        while(MgCallVsEffNumGet()) {
            HuPrcVSleep();
        }
        MgCallVsEffKill();
        while(!mbStatusOffCheckAll()) {
            HuPrcVSleep();
        }
        no = MgRouletteExec(workP->type, workP);
        _SetFlag(0x10002);
    }
    GwSystem.subGameNo = -1;
    MgCallCallMg(workP->type, no);
    MgRouletteKill(workP);
    return 0;
}

static int SetupTeam(int *colorIn)
{
    int i, j;
    int statusNo;
    int type;
    int colorP1;
    int playerNum;
    BOOL colorChange;

    int origColor[GW_PLAYER_MAX];
    int colorTbl[GW_PLAYER_MAX];

    for(i=0; i<GW_PLAYER_MAX; i++) {
        GwPlayer[i].teamBackup = GWPlayerTeamGet(i);
    }
    if(!GWTeamGet()) {
        for(i=0; i<GW_PLAYER_MAX; i++) {
            origColor[i] = mbStatusColorGet(i);
        }
    } else {
        for(i=0; i<GW_PLAYER_MAX/2; i++) {
            origColor[i] = (i == 0) ? 2 : 1;
        }
    }
    if(colorIn) {
        for(i=0; i<GW_PLAYER_MAX; i++) {
            mbStatusColorSet(i, colorIn[i]);
            colorTbl[i] = colorIn[i];
        }
    }
    type = SetupMgType(colorTbl);
    if(!GWTeamGet()) {
        playerNum = 4;
    } else {
        if(type == MG_TYPE_1VS3) {
            type = (frandf() < 0.5f) ? MG_TYPE_2VS2 : MG_TYPE_4P;
        }
        if(type == MG_TYPE_4P) {
            colorTbl[0] = 2;
            colorTbl[1] = 2;
        } else {
            colorTbl[0] = 2;
            colorTbl[1] = 1;
        }
        playerNum = 2;
    }
    colorChange = FALSE;
    for(i=0; i<playerNum; i++) {
        if(origColor[i] != colorTbl[i]) {
            colorChange = TRUE;
        }
    }
    if(colorChange) {
        mbAudFXPlay(0x46B);
        for(i=0; i<=60u; i++) {
            for(j=0; j<playerNum; j++) {
                if(origColor[j] != colorTbl[j]) {
                    mbStatusRainbowSet(j, i/60.0f, colorTbl[j]);
                }
            }
            HuPrcVSleep();
        }
    }
    HuPrcSleep(30);
    if(GWTeamGet()) {
        switch(type) {
            case MG_TYPE_2VS2:
                for(i=0; i<GW_PLAYER_MAX; i++) {
                    GwPlayerConf[i].grpNo = GWPlayerTeamGet(i);
                }
                break;

            case MG_TYPE_4P:
                for(i=0; i<GW_PLAYER_MAX; i++) {
                    GwPlayerConf[i].grpNo = 0;
                }
                break;
        }
        return type;
    } else {
        switch(type) {
            case MG_TYPE_1VS3:
                j = 0;
                colorP1 = colorTbl[0];
                for(i=1; i<GW_PLAYER_MAX; i++) {
                    if(colorP1 == colorTbl[i]) {
                        j |= (1 << i);
                    }
                }
                if(j == 0) {
                    i = 0;
                } else {
                    for(i=1; i<GW_PLAYER_MAX; i++) {
                        if((j & (1 << i)) == 0) {
                            break;
                        }
                    }
                }
                colorP1 = colorTbl[i];
                statusNo = 0;
                mbStatusMoveSet(i, NULL, (HuVecF *)&statusPos1Vs3Base[statusNo++], 1, 15);
                GwPlayerConf[i].grpNo = 0;
                for(i=0; i<GW_PLAYER_MAX; i++) {
                    if(colorP1 != colorTbl[i]) {
                        mbStatusMoveSet(i, NULL, (HuVecF *)&statusPos1Vs3Base[statusNo++], 1, 15);
                        GwPlayerConf[i].grpNo = 1;
                    }
                }
                break;

            case MG_TYPE_2VS2:
                statusNo = 0;
                for(i=0; i<GW_PLAYER_MAX; i++) {
                    if(colorTbl[i] == 2) {
                        mbStatusMoveSet(i, NULL, (HuVecF *)&statusPos2Vs2Base[statusNo++], 1, 15);
                        GwPlayerConf[i].grpNo = 0;
                    }
                }
                for(i=0; i<GW_PLAYER_MAX; i++) {
                    if(colorTbl[i] == 1) {
                        mbStatusMoveSet(i, NULL, (HuVecF *)&statusPos2Vs2Base[statusNo++], 1, 15);
                        GwPlayerConf[i].grpNo = 1;
                    }
                }
                break;
        }
        while(!mbStatusOffCheckAll()) {
            HuPrcVSleep();
        }
        return type;
    }
}

static int mgCallColorChanceTbl[3] = { 60, 10, 30 };
static int mgCallColorChanceTbl2[3] = { 30, 40, 30 };

static int SetupMgType(int *color)
{
    int i;
    int no;
    int blueNum;
    int redNum;
    int randomNum;
    BOOL preferRedF;
    int playerNum;
    int chanceTotal;
    int type;
    int hatenaNum;
    BOOL sameGrpF;
    int index1, index2;

    int randomTypeTbl[MG_TYPE_MAX];

    int totalChanceTbl[MG_TYPE_MAX];
    int redPlayerTbl[GW_PLAYER_MAX];
    int bluePlayerTbl[GW_PLAYER_MAX];
    int hatenaPlayerTbl[GW_PLAYER_MAX];

    int chance;

    playerNum = 0;
    hatenaNum = 0;
    blueNum = redNum = 0;
    sameGrpF = TRUE;

    for(i=0; i<GW_PLAYER_MAX; i++) {
        switch(mbStatusColorGet(i)) {
            case 1:
                bluePlayerTbl[blueNum++] = i;
                if(redNum) {
                    sameGrpF = FALSE;
                }
                playerNum++;
                break;

            case 2:
                redPlayerTbl[redNum++] = i;
                if(blueNum) {
                    sameGrpF = FALSE;
                }
                playerNum++;
                break;

            default:
                hatenaPlayerTbl[hatenaNum++] = i;
                break;
        }
    }
    if(playerNum >= GW_PLAYER_MAX) {
        if(sameGrpF) {
            type = MG_TYPE_4P;
        } else if(blueNum == redNum) {
            type = MG_TYPE_2VS2;
        } else {
            type = MG_TYPE_1VS3;
        }
    } else {
        randomNum = 0;
        if(sameGrpF) {
            randomTypeTbl[randomNum++] = MG_TYPE_4P;
            if(playerNum < GW_PLAYER_MAX/2) {
                randomTypeTbl[randomNum++] = MG_TYPE_2VS2;
            }
        } else {
            randomTypeTbl[randomNum++] = MG_TYPE_2VS2;
        }
        randomTypeTbl[randomNum++] = MG_TYPE_1VS3;
        chanceTotal = 0;
        for(i=0; i<randomNum; i++) {
            chanceTotal += mgCallColorChanceTbl[randomTypeTbl[i]];
            totalChanceTbl[i] = chanceTotal;
        }
        chance = frandmod(chanceTotal);
        for(i=0; i<randomNum; i++) {
            if(chance < totalChanceTbl[i]) {
                break;
            }
        }
        type = randomTypeTbl[i];
        for(i=0; i<100; i++) {
            index1 = frandmod(hatenaNum);
            index2 = frandmod(hatenaNum);
            no = hatenaPlayerTbl[index1];
            hatenaPlayerTbl[index1] = hatenaPlayerTbl[index2];
            hatenaPlayerTbl[index2] = no;
        }
        switch(type) {
            case MG_TYPE_4P:
                if(playerNum == 0) {
                    preferRedF = (frandf() < 0.5f) ? FALSE : TRUE;
                } else {
                    if(blueNum != 0) {
                        preferRedF = FALSE;
                    } else {
                        preferRedF = TRUE;
                    }
                }
                no = 0;
                if(!preferRedF) {
                    for(i=blueNum; i<GW_PLAYER_MAX; i++) {
                        bluePlayerTbl[blueNum++] = hatenaPlayerTbl[no++];
                    }
                } else {
                    for(i=redNum; i<GW_PLAYER_MAX; i++) {
                        redPlayerTbl[redNum++] = hatenaPlayerTbl[no++];
                    }
                }
                break;

            case MG_TYPE_1VS3:
                if(playerNum == 0) {
                    preferRedF = (frandf() < 0.5f) ? FALSE : TRUE;
                } else {
                    if(blueNum >= 2) {
                        preferRedF = FALSE;
                    } else if(redNum >= 2) {
                        preferRedF = TRUE;
                    } else {
                        preferRedF = (frandf() < 0.5f) ? FALSE : TRUE;
                    }
                }
                no = 0;
                if(!preferRedF) {
                    for(i=blueNum; i<GW_PLAYER_MAX-1; i++) {
                        bluePlayerTbl[blueNum++] = hatenaPlayerTbl[no++];
                    }
                    for(i=redNum; i<1; i++) {
                        redPlayerTbl[redNum++] = hatenaPlayerTbl[no++];
                    }
                } else {
                    for(i=blueNum; i<1; i++) {
                        bluePlayerTbl[blueNum++] = hatenaPlayerTbl[no++];
                    }
                    for(i=redNum; i<GW_PLAYER_MAX-1; i++) {
                        redPlayerTbl[redNum++] = hatenaPlayerTbl[no++];
                    }
                }
                break;

            case MG_TYPE_2VS2:
                no = 0;
                for(i=blueNum; i<GW_PLAYER_MAX/2; i++) {
                    bluePlayerTbl[blueNum++] = hatenaPlayerTbl[no++];
                }
                for(i=redNum; i<GW_PLAYER_MAX/2; i++) {
                    redPlayerTbl[redNum++] = hatenaPlayerTbl[no++];
                }
                break;
        }
    }
    for(i=0; i<blueNum; i++) {
        color[bluePlayerTbl[i]] = 1;
    }
    for(i=0; i<redNum; i++) {
        color[redPlayerTbl[i]] = 2;
    }
    return type;
}

static inline float MgCallSqrtf(float x)
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

static int MgRouletteExec(int type, MGCALL_WORK *workP)
{
    static int posYTbl[5][4] = {
        { 0, 0, 0, 0 },
        { 248, 0, 0, 0 },
        { 224, 272, 0, 0 },
        { 208, 248, 288, 0 },
        { 203, 233, 263, 293 },
    };
    static int defaultHeightTbl[MG_TYPE_MAX] = {
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
    MGCALL_WORK work;
    s16 noTbl[128];
    int hiddenTbl[16];
    HuVecF objPos;
    HuVecF targetPos;
    HuVecF outPos;
    int focusOfs;
    int nameColor;
    int nextDelay;
    int grp1Num;
    int soloGrp;
    BOOL seF;
    int i;
    MGLISTWORK *listWork;
    int height;
    int delay;
    MG_DATA *data;
    int nextMaxTime;
    int nextTime;
    int speed;
    int focusNo;
    BOOL micF;
    int hiddenNum;
    BOOL unlockF;
    int grp0Num;
    float chance;
    float t;
    float weight;
    float posY;
    float speedY;
    float rotZ;
    float rotY;

    micF = FALSE;
    hiddenNum = 0;
    seF = FALSE;
    if(workP == NULL) {
        workP = &work;
        MgRouletteCreate(workP);
    }
    height = defaultHeightTbl[type];
    if(type == MG_TYPE_1VS3) {
        grp0Num = grp1Num = 0;
        for(i=0; i<GW_PLAYER_MAX; i++) {
            if(GwPlayerConf[i].grpNo == 0) {
                grp0Num++;
            } else {
                grp1Num++;
            }
        }
        if(grp0Num == 1) {
            soloGrp = 0;
        } else {
            soloGrp = 1;
        }
        for(i=0; i<GW_PLAYER_MAX; i++) {
            if(soloGrp == GwPlayerConf[i].grpNo && (u8)GwPlayer[i].comF == FALSE) {
                micF = TRUE;
            }
        }
    }
    if(HuMCMicGet() == 1 && HuMCProbe(1)) {
        micF = FALSE;
    }
    if(HuMCMicGet() == 0) {
        micF = FALSE;
    }
    if(mbPartyFGet()) {
        chance = 0.5f;
    } else {
        chance = 0.1f;
    }
    if(micF && frandf() < chance) {
        type = MG_TYPE_1VS3;
        workP->sprId[1] = espEntry(mbBoardDataNumGet(0x50084), 95, 5);
        mbAudFXPlay(0x45B);
        posY = 88;
        speedY = 6;
        rotZ = 0;
        i = 0;
        do {
            t = i/30.0f;
            if(t > 1) {
                t = 1;
            }
            espPosSet(workP->sprId[1], 288, 88.0-(120.0*HuSin(90*(1-t))));
            if(t >= 0.6f) {
                if(!seF) {
                    mbAudFXPlay(0x45C);
                    seF = TRUE;
                }
                if(rotZ > -40) {
                    rotZ -= 1;
                }
                posY += speedY;
                speedY += 0.2f;
                if(workP->sprId[0] >= 0) {
                    espPosSet(workP->sprId[0], 288, posY);
                    espZRotSet(workP->sprId[0], rotZ);
                }
            }
            HuPrcVSleep();
            i++;
        } while(posY < 576);
        if(mbPartyFGet()) {
            height = MgRouletteNumMicGet(type, height, noTbl);
        } else {
            height = MgRouletteNumMicSingleGet(type, height, noTbl);
        }
    } else {
        if(mbPartyFGet()) {
            height = MgRouletteNumGet(type, height, noTbl);
        } else {
            height = MgRouletteNumSingleGet(type, height, noTbl);
        }
    }
    workP->sprId[4] = espEntry(mbBoardDataNumGet(0x50087), 100, 0);
    espPosSet(workP->sprId[4], 288, 240);
    espDrawNoSet(workP->sprId[4], 32);
    workP->sprId[5] = espEntry(mbBoardDataNumGet(0x50088), 102, 0);
    espPosSet(workP->sprId[5], 288, 248);
    espDrawNoSet(workP->sprId[5], 32);
    espTPLvlSet(workP->sprId[5], 0.5f);
    workP->sprId[6] = espEntry(mbBoardDataNumGet(0x50089), 101, 0);
    espPosSet(workP->sprId[6], 288, 256);
    espDrawNoSet(workP->sprId[6], 32);
    espTPLvlSet(workP->sprId[6], 0.5f);
    espDispOff(workP->sprId[6]);
    for(i=1; i<=30; i++) {
        t = 1-(i/30.0f);
        espPosSet(workP->sprId[4], 288, 32+(240+(480*t)));
        espPosSet(workP->sprId[5], 288, 32+(248+(480*t)));
        HuPrcVSleep();
    }
    for(i=0; i<4; i++) {
        mgListObj[i] = NULL;
    }
    hiddenNum = 0;
    for(i=0; i<height; i++) {
        data = &MgDataTbl[noTbl[i]];
        mgListObj[i] = omAddObjEx(mbObjMan, 257, 0, 0, OM_GRP_NONE, MgRouletteOMExec);
        listWork = omObjGetWork(mgListObj[i], MGLISTWORK);
        listWork->dispF = TRUE;
        listWork->no = i;
        mgListObj[i]->trans.y = posYTbl[height][i]+32;
        unlockF = FALSE;
        if(mbPartyFGet()) {
            if(GWMgUnlockGet(noTbl[i]+0x259)) {
                unlockF = TRUE;
            }
        } else {
            if(GWMgUnlockGet(noTbl[i]+0x259) || mbSingleMgUnlockGet(noTbl[i]+0x259)) {
                unlockF = TRUE;
            }
        }
        if(unlockF) {
            listWork->winNo = mbWinCreateHelp(data->nameMes);
            listWork->hiddenMes = -1;
        } else {
            if(mbLanguageGet() != 5) {
                listWork->winNo = mbWinCreateHelp(MESSNUM_PTR("\xC3\xC3\xC3\xC3\xC3\xC3\xC3"));
            } else {
                listWork->winNo = mbWinCreateHelp(MESSNUM_PTR("\x5D\xC3\x5E"));
            }
            listWork->hiddenMes = data->nameMes;
            hiddenTbl[hiddenNum] = i;
            hiddenNum++;
        }
        mbWinPriSet(listWork->winNo, 90);
        nameColor = MgNameColorGet(data->nameMes);
        MgRouletteSlide(mgListObj[i]);
    }
    mgCallFocus = -1;
    while(!MgRouletteSlideCheck(mgListObj[0])) {
        HuPrcVSleep();
    }
    mgCallFocus = 0;
    focusNo = mbRandMod(height);
    if(!mbPartyFGet() && hiddenNum != 0) {
        focusNo = hiddenTbl[mbRandMod(hiddenNum)];
    }
    delay = mbRandMod(30)+90;
    focusOfs = (MgCallSqrtf(25+(8.0f*delay))+-5)/2;
    speed = (focusNo-focusOfs)%height;
    if(speed < 0) {
        speed += height;
    }
    nextDelay = (height*3)+(speed*3);
    nextTime = nextMaxTime = 3;
    for(i=0; i<nextDelay+delay; i++) {
        if(--nextTime == 0) {
            mgCallFocus = (mgCallFocus+1)%height;
            if(i > nextDelay) {
                nextMaxTime++;
                nextTime = nextMaxTime;
            } else {
                nextTime = nextMaxTime;
            }
            espPosSet(workP->sprId[6], 288, mgListObj[mgCallFocus]->trans.y);
            espDispOn(workP->sprId[6]);
            mbAudFXPlay(0x3F1);
        }
        HuPrcVSleep();
    }
    mgCallFocus = focusNo;
    espPosSet(workP->sprId[6], 288, mgListObj[mgCallFocus]->trans.y);
    MgRouletteFocus(mgListObj[mgCallFocus]);
    mbAudFXPlay(0x46E);
    if(!mbPartyFGet() && !mbSingleMgUnlockGet(noTbl[mgCallFocus]+0x259) && !GWMgUnlockGet(noTbl[mgCallFocus]+0x259)) {
        omVibrate(GwSystem.turnPlayerNo, 20, 7, 3);
    }
    if(workP->guideObjId >= 0) {
        mbObjPosGet(workP->guideObjId, &objPos);
        targetPos.x = objPos.x-300;
        targetPos.y = objPos.y-150;
        targetPos.z = objPos.z;
        mbObjMotionShiftSet(workP->guideObjId, 3, 0, 8, 0);
        for(i=1; i<=60u; i++) {
            weight = i/60.0f;
            if(weight <= 0.25f) {
                rotY = -90.0*HuSin(90*(4*weight));
            } else if(weight < 0.75) {
                rotY = -90;
            } else {
                rotY = -90.0*HuCos(90*(4*(weight-0.75f)));
            }
            mbev_CapVecChase(&objPos, &targetPos, &outPos, HuSin(90*weight));
            mbObjPosSetV(workP->guideObjId, &outPos);
            mbObjRotSet(workP->guideObjId, 0, rotY, 0);
            HuPrcVSleep();
        }
        mbObjMotionShiftSet(workP->guideObjId, 2, 0, 8, 0x40000001);
        HuPrcSleep(120);
    } else {
        HuPrcSleep(120);
    }
    return noTbl[mgCallFocus];
}

static s16 MgNameColorGet(u32 nameMes)
{
    char *mesPtr;
    s16 i;
    if(nameMes > 0x80000000) {
        mesPtr = (char *)nameMes;
    } else {
        mesPtr = HuWinMesPtrGet(nameMes);
    }
    for(i=0; *mesPtr; mesPtr++) {
        if(*mesPtr == 0x1E) {
            return mesPtr[1]-1;
        }
    }
    return 7;
}

static int MgRouletteNumGet(int type, int height, s16 *noTbl)
{
    int i;
    int num;
    int pack;
    MG_DATA *data;
    int no;
    int callNo;
    int j;
    int order1;
    int order2;
    int temp;
    BOOL skipF;
    BOOL storyF = FALSE;

    s16 validNo[128];

    MGLISTPACK packData[4] = {
        { 1, 0x08 },
        { 2, 0x01 },
        { 3, 0x02 },
        { 4, 0x04 },
    };
    for(pack=0; pack<4; pack++) {
        if(packData[pack].no == GWMgPackGet()) {
            break;
        }
    }
    if(pack >= 4) {
        pack = -1;
    }
    for(no=0, num=0, data=&MgDataTbl[0]; data->ovl != 0xFFFF; data++, no++) {
        if(data->type != type) {
            continue;
        }
        if(pack >= 0 && !(data->flag & packData[pack].flag)) {
            continue;
        }
        if(!storyF && (data->flag & 0x200)) {
            continue;
        }
        if((u8)GwSystem.curTime != 0 && (data->flag & 0x400)) {
            continue;
        }
        if((u8)GwSystem.curTime == 0 && (data->flag & 0x800)) {
            continue;
        }
        if(!MgCallHisCheck(type, no+0x259)) {
            validNo[num++] = no;
        }
    }
    if(num < height) {
        no = mgCallHisOfsTbl[type];
        for(i=0; i<mgCallHisSize[type]; i++) {
            callNo = mgCallHisPtr[type][no];
            if(callNo) {
                callNo -= 0x259;
                skipF = FALSE;
                if(pack >= 0 && !(MgDataTbl[callNo].flag & packData[pack].flag)) {
                    skipF = TRUE;
                }
                if(!storyF && (MgDataTbl[callNo].flag & 0x200)) {
                    skipF = TRUE;
                }
                if((u8)GwSystem.curTime != 0 && (MgDataTbl[callNo].flag & 0x400)) {
                    skipF = TRUE;
                }
                if((u8)GwSystem.curTime == 0 && (MgDataTbl[callNo].flag & 0x800)) {
                    skipF = TRUE;
                }
                for(j=0; j<num; j++) {
                    if(callNo == validNo[j]) {
                        break;
                    }
                }
                if(j >= num && !skipF) {
                    validNo[num++] = callNo;
                    if(num >= height) {
                        break;
                    }
                }
            }
            no = (no+1)%mgCallHisSize[type];
        }
    }
    for(i=0; i<100; i++) {
        order1 = mbRandMod(num);
        order2 = mbRandMod(num);
        temp = validNo[order1];
        validNo[order1] = validNo[order2];
        validNo[order2] = temp;
    }
    if(num > height) {
        num = height;
    }
    for(i=0; i<num; i++) {
        noTbl[i] = validNo[i];
    }
    for(; i<height; i++) {
        noTbl[i] = -1;
    }
    return num;
}

static int MgRouletteNumSingleGet(int type, int height, s16 *noTbl)
{
    int i;
    int num;
    int pack;
    MG_DATA *data;
    int no;
    int order1;
    int order2;
    int temp;
    int rest;
    int singleNum;
    int unlockNum;
    BOOL storyF = FALSE;

    s16 validNo[128];
    s16 singleNo[128];
    s16 unlockNo[128];

    MGLISTPACK packData[4] = {
        { 1, 0x08 },
        { 2, 0x01 },
        { 3, 0x02 },
        { 4, 0x04 },
    };
    for(pack=0; pack<4; pack++) {
        if(packData[pack].no == GWMgPackGet()) {
            break;
        }
    }
    if(pack >= 4) {
        pack = -1;
    }
    for(no=0, num=0, singleNum=0, unlockNum=0, data=&MgDataTbl[0]; data->ovl != 0xFFFF; data++, no++) {
        if(data->type != type) {
            continue;
        }
        if(pack >= 0 && !(data->flag & packData[pack].flag)) {
            continue;
        }
        if(!storyF && (data->flag & 0x200)) {
            continue;
        }
        if(mgCallSingleKoopaF && (data->flag & 0x40)) {
            continue;
        }
        if(mbSingleMgUnlockGet(no+0x259)) {
            singleNo[singleNum] = no;
            singleNum++;
        } else if(GWMgUnlockGet(no+0x259)) {
            unlockNo[unlockNum] = no;
            unlockNum++;
        } else {
            validNo[num++] = no;
        }
    }
    if(num < height && unlockNum > 0) {
        if(unlockNum >= 2) {
            for(i=0; i<100; i++) {
                order1 = i%unlockNum;
                order2 = mbRandMod(unlockNum);
                if(order1 != order2) {
                    temp = unlockNo[order1];
                    unlockNo[order1] = unlockNo[order2];
                    unlockNo[order2] = temp;
                }
            }
        }
        rest = height-num;
        for(i=0; i<rest && i<unlockNum; i++) {
            validNo[num++] = unlockNo[i];
        }
    }
    if(num < height && singleNum > 0) {
        if(singleNum >= 2) {
            for(i=0; i<100; i++) {
                order1 = i%singleNum;
                order2 = mbRandMod(singleNum);
                if(order1 != order2) {
                    temp = singleNo[order1];
                    singleNo[order1] = singleNo[order2];
                    singleNo[order2] = temp;
                }
            }
        }
        rest = height-num;
        for(i=0; i<rest && i<singleNum; i++) {
            validNo[num++] = singleNo[i];
        }
    }
    for(i=0; i<100; i++) {
        order1 = mbRandMod(num);
        order2 = mbRandMod(num);
        temp = validNo[order1];
        validNo[order1] = validNo[order2];
        validNo[order2] = temp;
    }
    if(num > height) {
        num = height;
    }
    for(i=0; i<num; i++) {
        noTbl[i] = validNo[i];
    }
    for(; i<height; i++) {
        noTbl[i] = -1;
    }
    return num;
}

static int MgRouletteNumMicGet(int type, int height, s16 *noTbl)
{
    int i;
    int num;
    int pack;
    MG_DATA *data;
    int no;
    int order1;
    int order2;
    int temp;
    BOOL storyF = FALSE;

    s16 validNo[128];

    MGLISTPACK packData[4] = {
        { 1, 0x08 },
        { 2, 0x01 },
        { 3, 0x02 },
        { 4, 0x04 },
    };
    for(pack=0; pack<4; pack++) {
        if(packData[pack].no == GWMgPackGet()) {
            break;
        }
    }
    if(pack >= 4) {
        pack = -1;
    }
    for(no=0, num=0, data=&MgDataTbl[0]; data->ovl != 0xFFFF; data++, no++) {
        if(data->type != type) {
            continue;
        }
        if(pack >= 0 && !(data->flag & packData[pack].flag)) {
            continue;
        }
        if(!(data->flag & 0x200)) {
            continue;
        }
        if((u8)GwSystem.curTime != 0 && (data->flag & 0x400)) {
            continue;
        }
        if((u8)GwSystem.curTime == 0 && (data->flag & 0x800)) {
            continue;
        }
        validNo[num++] = no;
    }
    for(i=0; i<100; i++) {
        order1 = mbRandMod(num);
        order2 = mbRandMod(num);
        temp = validNo[order1];
        validNo[order1] = validNo[order2];
        validNo[order2] = temp;
    }
    if(num > height) {
        num = height;
    }
    for(i=0; i<num; i++) {
        noTbl[i] = validNo[i];
    }
    for(; i<height; i++) {
        noTbl[i] = -1;
    }
    return num;
}

static int MgRouletteNumMicSingleGet(int type, int height, s16 *noTbl)
{
    int i;
    int num;
    int pack;
    MG_DATA *data;
    int no;
    int order1;
    int order2;
    int temp;
    int rest;
    int singleNum;
    int unlockNum;
    BOOL storyF = FALSE;

    s16 validNo[128];
    s16 singleNo[128];
    s16 unlockNo[128];

    MGLISTPACK packData[4] = {
        { 1, 0x08 },
        { 2, 0x01 },
        { 3, 0x02 },
        { 4, 0x04 },
    };
    for(pack=0; pack<4; pack++) {
        if(packData[pack].no == GWMgPackGet()) {
            break;
        }
    }
    if(pack >= 4) {
        pack = -1;
    }
    for(no=0, num=0, singleNum=0, unlockNum=0, data=&MgDataTbl[0]; data->ovl != 0xFFFF; data++, no++) {
        if(data->type != type) {
            continue;
        }
        if(pack >= 0 && !(data->flag & packData[pack].flag)) {
            continue;
        }
        if(!(data->flag & 0x200)) {
            continue;
        }
        if((u8)GwSystem.curTime != 0 && (data->flag & 0x400)) {
            continue;
        }
        if((u8)GwSystem.curTime == 0 && (data->flag & 0x800)) {
            continue;
        }
        if(mbSingleMgUnlockGet(no+0x259)) {
            singleNo[singleNum] = no;
            singleNum++;
        } else if(GWMgUnlockGet(no+0x259)) {
            unlockNo[unlockNum] = no;
            unlockNum++;
        } else {
            validNo[num++] = no;
        }
    }
    if(num < height && unlockNum > 0) {
        if(unlockNum >= 2) {
            for(i=0; i<100; i++) {
                order1 = i%unlockNum;
                order2 = mbRandMod(unlockNum);
                if(order1 != order2) {
                    temp = unlockNo[order1];
                    unlockNo[order1] = unlockNo[order2];
                    unlockNo[order2] = temp;
                }
            }
        }
        rest = height-num;
        for(i=0; i<rest && i<unlockNum; i++) {
            validNo[num++] = unlockNo[i];
        }
    }
    if(num < height && singleNum > 0) {
        if(singleNum >= 2) {
            for(i=0; i<100; i++) {
                order1 = i%singleNum;
                order2 = mbRandMod(singleNum);
                if(order1 != order2) {
                    temp = singleNo[order1];
                    singleNo[order1] = singleNo[order2];
                    singleNo[order2] = temp;
                }
            }
        }
        rest = height-num;
        for(i=0; i<rest && i<singleNum; i++) {
            validNo[num++] = singleNo[i];
        }
    }
    for(i=0; i<100; i++) {
        order1 = mbRandMod(num);
        order2 = mbRandMod(num);
        temp = validNo[order1];
        validNo[order1] = validNo[order2];
        validNo[order2] = temp;
    }
    if(num > height) {
        num = height;
    }
    for(i=0; i<num; i++) {
        noTbl[i] = validNo[i];
    }
    for(; i<height; i++) {
        noTbl[i] = -1;
    }
    return num;
}

static void MgRouletteOMExec(OMOBJ *obj)
{
    MGLISTWORK *work = omObjGetWork(obj, MGLISTWORK);
    float weight;
    float time;
    HuVec2f winSize;

    if(work->killF || mbExitCheck()) {
        mbWinKill(work->winNo);
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    if(work->dispF) {
        mbWinDispSet(work->winNo, TRUE);
    } else {
        mbWinDispSet(work->winNo, FALSE);
    }
    switch(work->mode) {
        case 0:
            if(work->time > work->maxTime) {
                work->slideF = FALSE;
                work->mode = 1;
            } else {
                weight = (float)(work->time++)/work->maxTime;
                time = HuSin(weight*90);
                if((work->no & 0x1) == 0) {
                    obj->trans.x = -160+(448*time);
                } else {
                    obj->trans.x = 736+(-448*time);
                }
            }
            break;

        case 1:
            if(work->no == mgCallFocus) {
                work->mode = 2;
            }
            break;

        case 2:
            if(work->no != mgCallFocus) {
                work->mode = 3;
                work->time = 0;
                work->maxTime = 8;
            }
            break;

        case 3:
            if(work->time > work->maxTime) {
                work->mode = 1;
            } else {
                weight = (float)(work->time++)/work->maxTime;
            }
            break;

        case 4:
            if(work->time <= work->maxTime) {
                weight = (float)(work->time++)/work->maxTime;
                obj->scale.x = obj->scale.y = (0.2f*HuSin((1-weight)*720))+1;
            }
            break;

        case 5:
            if(work->time <= work->maxTime) {
                weight = (float)(work->time++)/work->maxTime;
                obj->trans.y = obj->rot.y+(weight*(96-obj->rot.y));
                obj->scale.x = obj->scale.y = (0.5f*HuCos(weight*90))+0.5f;
            }
            break;
    }
    mbWinScaleSet(work->winNo, obj->scale.x, obj->scale.y);
    mbWinMesMaxSizeGet(work->winNo, &winSize);
    mbWinPosSet(work->winNo, obj->trans.x-((winSize.x/2)*obj->scale.x), obj->trans.y-((winSize.y/2)*obj->scale.y));
}

static void MgRouletteSlide(OMOBJ *obj)
{
    MGLISTWORK *work = omObjGetWork(obj, MGLISTWORK);
    work->slideF = TRUE;
    work->mode = 0;
    work->time = 0;
    work->maxTime = 30;
}

static void MgRouletteFocus(OMOBJ *obj)
{
    MGLISTWORK *work = omObjGetWork(obj, MGLISTWORK);
    work->mode = 4;
    work->time = 0;
    work->maxTime = 90;
    if(work->hiddenMes >= 0) {
        mbWinKill(work->winNo);
        work->winNo = mbWinCreateHelp(work->hiddenMes);
    }
}

static BOOL MgRouletteSlideCheck(OMOBJ *obj)
{
    MGLISTWORK *work = omObjGetWork(obj, MGLISTWORK);
    if(work->slideF) {
        return FALSE;
    } else {
        return TRUE;
    }
}

void mbMgRouletteFocusKill(BOOL dispF)
{
    MGLISTWORK *work = omObjGetWork(mgListObj[mgCallFocus], MGLISTWORK);
    work->dispF = dispF;
}

/* ---- part 2 (fork 2): rough stubs so the unit compiles; data placement only ---- */

static const int battleCharFileTbl[11] = {
    0x50093, 0x50094, 0x50095, 0x50096, 0x50097, 0x50098, 0x50099, 0x5009A, 0x5009B, 0x5009C, 0x5009D
};

static const int battleMgDataTbl[6][3] = {
    { 0x5002D, 0x5009E, 0x5009E },
    { 0x5002E, 0x5009F, 0x500A0 },
    { 0x50030, 0x500A1, 0x500A1 },
    { 0x5003F, 0x500A2, 0x500A3 },
    { 0x5004F, 0x500A4, 0x500A4 },
    { 0x50050, 0x500A5, 0x500A5 },
};

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
    work->hookObjId = -1;
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
    if(work->hookObjId >= 0) {
        mbObjKill(work->hookObjId);
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
