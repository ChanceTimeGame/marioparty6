#include "game/hu3d.h"
/* Retail calls GWPlayerStarGet as returning int (no extsh of its result): hide the s16
   prototype from game/gamework.h and declare it without one. */
#define GWPlayerStarGet GWPlayerStarGet_s16
#include "game/gamework.h"
#undef GWPlayerStarGet
int GWPlayerStarGet();

#include "game/board/player.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/coin.h"
#include "game/board/masu.h"
#include "game/board/math.h"
#include "game/board/object.h"
#include "game/audio.h"
#include "game/charman.h"
#include "game/gamework.h"
#include "game/object.h"
#include "game/process.h"
#include "math.h"

/* Work kept in the player's collision object (playerWork[].unk24->work). */
typedef struct PlayerColWork_s {
    u8 unk0_7 : 1;
    u8 unk0_6 : 1;
    u8 activeF : 1;
    u8 unk0_2 : 3;
    u8 unk0_0 : 2;
    u8 unk1;
    u8 masuId;
    u8 unk3[5];
    float unk8;
} PLAYERCOLWORK;

static MBPLAYERWORK playerWork[GW_PLAYER_MAX];
static BOOL blackoutF;

static inline GW_PLAYER *GWPlayerGet(int playerNo)
{
    return &GwPlayer[playerNo];
}

static inline BOOL GWTeamGet(void)
{
    return GwSystem.tagF;
}

static inline int GWPlayerTeamGet(int playerNo)
{
    return GwPlayer[playerNo].team;
}

static s8 tagIdTbl[10*11] = {
     -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
     -1, -1, 10, 11, 12, 13, 14, 15, 16, 17, 18,
     -1, -1, -1, 19, 20, 21, 22, 23, 24, 25, 26,
     -1, -1, -1, -1, 27, 28, 29, 30, 31, 32, 33,
     -1, -1, -1, -1, -1, 34, 35, 36, 37, 38, 39,
     -1, -1, -1, -1, -1, -1, 40, 41, 42, 43, 44,
     -1, -1, -1, -1, -1, -1, -1, 45, 46, 47, 48,
     -1, -1, -1, -1, -1, -1, -1, -1, 49, 50, 51,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, 52, 53,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 54,
};

/* ---- part 1/2 (other forks): stubs so the unit compiles ---- */

void mbPlayerInit(BOOL noEventF) {}
void mbPlayerClose(void) {}

MBPLAYERWORK *mbPlayerWorkGet(int playerNo)
{
    return &playerWork[playerNo];
}

void mbPlayerTurnInitHookSet(void *arg0) {}
void mbPlayerTurnCloseHookSet(void *arg0) {}
void mbPlayerStartTurnHookSet(int playerNo, MBPLAYERTURNHOOK hook) {}
void mbPlayerEndTurnHookSet(int playerNo, MBPLAYERTURNHOOK hook) {}
void mbPlayerMoveHookSet(int playerNo, MBPLAYERTURNHOOK hook) {}
void mbTurnExec(int arg0) {}
void mbSingleTurnExec(int arg0) {}
int mbPlayerDiceTypeGet(int arg0) { return 0; }
void mbPlayerMasuMoveTo(int arg0, s16 arg1, int arg2) {}
void mbPlayerMasuMove(int arg0, int arg1) {}
void mbPlayerMasuMovePos(int arg0, HuVecF *arg1, int arg2) {}
void mbPlayerMasuMoveSpeed(int arg0, s16 arg1, int arg2, int arg3) {}
void mbPlayerMoveExec(int arg0, HuVecF *arg1, HuVecF *arg2, int arg3, HuVecF *arg4, int arg5) {}
void mbPlayerMoveMain(int arg0, HuVecF *arg1, HuVecF *arg2, int arg3, int arg4, int arg5, HuVecF *arg6, int arg7, float f0) {}
#pragma dont_inline on
void mbPlayerRotateStart(int playerNo, s16 endAngle, s16 maxTime) {} /* stub: keep it a call */
#pragma dont_inline reset
BOOL mbPlayerRotateCheck(int playerNo) { return 0; }
BOOL mbPlayerRotateCheckAll(void) { return 0; }
void mbPlayerDiceMotExec(int arg0) {}
void mbMoveNumCreateColor(int arg0, u8 arg1, int arg2) {}
void mbMoveNumCreate(int arg0, int arg1) {}
void mbMoveNumKill(int arg0) {}
void mbMoveNumDispSet(int arg0, u8 arg1) {}
void mbev_PlayerColMasuAllSet(void *arg0, int arg1) {}
void mbev_PlayerColMasu(int arg0, int arg1, int arg2) {}
void mbev_PlayerColCircleAdd(int arg0, int arg1, int arg2, float f0) {}
void mbev_PlayerColMasuAdd(int arg0, int arg1, int arg2) {}
void mbev_PlayerColBall(int arg0, void *arg1, void *arg2) {}
void mbev_PlayerColMasuSet(int arg0, int arg1, int arg2) {}

/* part 2's static; inlined into mbPlayerPosReset and mbPlayerPosResetAll */
static void PlayerColCornerSnap(int playerNo, int masuId, int cornerNo)
{
    MBPLAYERWORK *workP = mbPlayerWorkGet(playerNo);
    PLAYERCOLWORK *colWork = (PLAYERCOLWORK *)workP->unk24->work;
    Mtx mtx;
    Mtx invMtx;
    HuVecF pos;
    if(colWork->activeF) {
        colWork->unk0_7 = FALSE;
        colWork->unk0_0 = 0;
        colWork->unk0_6 = FALSE;
        colWork->masuId = masuId;
        workP->masuCorner = (s8)cornerNo;
        mbMasuMtxGet(colWork->masuId, mtx);
        PSMTXInverse(mtx, invMtx);
        mbPlayerPosGet(playerNo, &pos);
        PSMTXMultVec(invMtx, &pos, &playerWork[playerNo].unk3C);
        PSMTXMultVec(invMtx, &pos, &playerWork[playerNo].unk24->trans);
        mbPlayerRotYSet(playerNo, 0);
        colWork->unk8 = 0;
        GwPlayer[playerNo].moveF = FALSE;
    }
}

int mbPlayerColCheck(void) { return 0; }
void mbev_PlayerColReserve(int arg0, int arg1, int arg2) {}
void mbev_PlayerColSet(int arg0, int arg1) {}
void mbPlayerColSnapSet(int arg0) {}
void mbPlayerColSnapPlayerSet(int arg0, int arg1) {}
int mbPlayerColSnapGet(int arg0) { return 0; }
void mbPlayerColRestSet(int arg0, int arg1) {}
void mbPlayerColFirstSet(int arg0) {}
void mbPlayerColOrderReset(void) {}
void mbPlayerEyeMatDarkSet(int arg0, int arg1) {}
void mbPlayerMatClone(int playerNo) {}
void mbPlayerMetalSet(int arg0, int arg1) {}
void mbPlayerEffectSet(int arg0, u8 arg1) {}
void mbPlayerMetalColorSet(void *arg0, void *arg1) {}
void mbPlayerBiriQSet(int arg0, int arg1) {}
void mbPlayerSwap(int arg0, int arg1) {}

/* ---- part 3 ---- */

u32 mbPlayerNameMesGet(int playerNo)
{
    u32 nameTbl[14] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13
    };
    return nameTbl[GwPlayer[playerNo].charNo];
}

char *mbPlayerNameGet(int playerNo)
{
    char *nameTbl[14] = {
        "Mario",
        "Luigi",
        "Peach",
        "Yoshi",
        "Wario",
        "Daisy",
        "Waluigi",
        "Kinopio",
        "Teresa",
        "Mini Koopa",
        "Kinopiko",
        NULL,
        NULL,
        NULL
    };
    return nameTbl[GwPlayer[playerNo].charNo];
}

int mbPlayerTagNameMesGet(int teamNo)
{
    int charNo1 = GwPlayer[mbPlayerTeamFindPlayer(teamNo, 0)].charNo;
    int charNo2 = GwPlayer[mbPlayerTeamFindPlayer(teamNo, 1)].charNo;
    int temp;
    int tagId;
    if(charNo1 > charNo2) {
        temp = charNo1;
        charNo1 = charNo2;
        charNo2 = temp;
    }
    tagId = tagIdTbl[charNo2 + charNo1 * 11];
    OSReport("%d:%d->%d\n", charNo1, charNo2, tagId);
    if(tagId == -1) {
        return 0x30037;
    } else {
        return tagId + 0x30000;
    }
}

void mbPlayerAmbSet(int playerNo, float ambR, float ambG, float ambB)
{
    mbObjAmbSet(mbPlayerObjIDGet(playerNo), ambR, ambG, ambB);
}

s16 mbPlayerObjIDGet(int playerNo)
{
    return playerWork[playerNo].objId;
}

s16 mbPlayerModelIDGet(int playerNo)
{
    return mbObjModelIDGet(playerWork[playerNo].objId);
}

BOOL mbPlayerAllComCheck(void)
{
    int i;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(!GwPlayer[i].comF) {
            return FALSE;
        }
    }
    return TRUE;
}

int mbPlayerTeamFind(int playerNo)
{
    int i;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(i == playerNo) {
            continue;
        }
        if(GWPlayerTeamGet(playerNo) == GWPlayerTeamGet(i)) {
            break;
        }
    }
    return i;
}

int mbPlayerTeamFindOther(int playerNo)
{
    int i;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(i == playerNo) {
            continue;
        }
        if(GWPlayerTeamGet(playerNo) != GWPlayerTeamGet(i)) {
            break;
        }
    }
    return i;
}

int mbPlayerTeamFindPlayer(int teamNo, int memberNo)
{
    int i;
    int no;
    no = 0;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(teamNo != GWPlayerTeamGet(i)) {
            continue;
        }
        if(no == memberNo) {
            return i;
        }
        no++;
    }
    return -1;
}

int mbPlayerTeamFindOpp(int playerNo)
{
    if(!GWTeamGet()) {
        return playerNo;
    }
    return mbPlayerTeamFind(playerNo);
}

int mbPlayerTeamCheckSame(int playerNo1, int playerNo2)
{
    BOOL result = FALSE;
    if(GWTeamGet()) {
        if(GWPlayerTeamGet(playerNo1) == GWPlayerTeamGet(playerNo2)) {
            result = TRUE;
        }
    } else if(playerNo1 == playerNo2) {
        result = TRUE;
    }
    return result;
}

int mbPlayerTeamGet(int playerNo)
{
    if(!GWTeamGet()) {
        return playerNo;
    }
    return GwPlayer[playerNo].team;
}

float mbPlayerWalkSpeedGet(void)
{
    return 20;
}

void mbPlayerLayerSet(int playerNo, int layer)
{
    mbObjLayerSet(mbPlayerObjIDGet(playerNo), layer);
}

void mbPlayerCameraSet(int playerNo, u16 bit)
{
    mbObjCameraSet(mbPlayerObjIDGet(playerNo), bit);
}

void mbPlayerCullRadiusSet(int playerNo, float radius)
{
    mbObjCullRadiusSet(mbPlayerObjIDGet(playerNo), radius);
}

void mbPlayerStubValSet(int playerNo, BOOL value)
{
}

void mbPlayerPosReset(int playerNo)
{
    HuVecF pos;
    mbMasuPosGet(GwPlayer[playerNo].masuId, &pos);
    mbPlayerPosSetV(playerNo, &pos);
    PlayerColCornerSnap(playerNo, GwPlayer[playerNo].masuId, 0);
}

void mbPlayerPosResetAll(void)
{
    int i, j;
    int cornerNo;
    s16 masuId;
    s8 orderNo;
    HuVecF pos;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        orderNo = GwPlayer[i].orderNo;
        masuId = GwPlayer[i].masuId;
        cornerNo = 0;
        for(j = 0; j < GW_PLAYER_MAX; j++) {
            if(i != j && masuId == GwPlayer[j].masuId && orderNo > GwPlayer[j].orderNo) {
                cornerNo++;
            }
        }
        mbPlayerMasuCornerSet(i, cornerNo);
        if(cornerNo == 0) {
            mbMasuPosGet(masuId, &pos);
        } else {
            mbMasuCornerRotPosGet(masuId, cornerNo - 1, &pos);
        }
        mbPlayerPosSetV(i, &pos);
        PlayerColCornerSnap(i, masuId, cornerNo);
    }
}

void mbPlayerMtxSet(int playerNo, Mtx *matrix)
{
    mbObjMtxSet(mbPlayerObjIDGet(playerNo), *matrix);
}

void mbPlayerMtxGet(int playerNo, void *matrix)
{
    mbObjMtxGet(mbPlayerObjIDGet(playerNo), matrix);
}

void mbPlayerPosSetV(int playerNo, HuVecF *pos)
{
    mbObjPosSetV(mbPlayerObjIDGet(playerNo), pos);
}

void mbPlayerPosSet(int playerNo, float posX, float posY, float posZ)
{
    mbObjPosSet(mbPlayerObjIDGet(playerNo), posX, posY, posZ);
}

void mbPlayerPosGet(int playerNo, HuVecF *pos)
{
    mbObjPosGet(mbPlayerObjIDGet(playerNo), pos);
}

void mbPlayerRotSetV(int playerNo, HuVecF *rot)
{
    mbObjRotSetV(mbPlayerObjIDGet(playerNo), rot);
}

void mbPlayerRotSet(int playerNo, float rotX, float rotY, float rotZ)
{
    mbObjRotSet(mbPlayerObjIDGet(playerNo), rotX, rotY, rotZ);
}

void mbPlayerRotGet(int playerNo, HuVecF *rot)
{
    mbObjRotGet(mbPlayerObjIDGet(playerNo), rot);
}

void mbPlayerRotYSet(int playerNo, float rotY)
{
    rotY = fmod(rotY, 360);
    if(rotY < 0) {
        rotY += 360;
    }
    mbObjRotYSet(mbPlayerObjIDGet(playerNo), rotY);
}

float mbPlayerRotYGet(int playerNo)
{
    return mbObjRotYGet(mbPlayerObjIDGet(playerNo));
}

void mbPlayerScaleSetV(int playerNo, HuVecF *scale)
{
    mbObjScaleSetV(mbPlayerObjIDGet(playerNo), scale);
}

void mbPlayerScaleSet(int playerNo, float scaleX, float scaleY, float scaleZ)
{
    mbObjScaleSet(mbPlayerObjIDGet(playerNo), scaleX, scaleY, scaleZ);
}

void mbPlayerScaleGet(int playerNo, HuVecF *scale)
{
    mbObjScaleGet(mbPlayerObjIDGet(playerNo), scale);
}

void mbPlayerMotionSet(int playerNo, int motNo, int attr)
{
    GW_PLAYER *playerP = GWPlayerGet(playerNo);
    if(motNo == playerWork[playerNo].motNo) {
        return;
    }
    mbObjMotionSet(mbPlayerObjIDGet(playerNo), motNo, attr);
    playerWork[playerNo].motNo = motNo;
    if(motNo == 10) {
        mbObjOffsetSet(mbPlayerObjIDGet(playerNo), 0, 4, 0);
    } else {
        mbObjOffsetSet(mbPlayerObjIDGet(playerNo), 0, 0, 0);
    }
}

s16 mbPlayerMotionGet(int playerNo)
{
    return playerWork[playerNo].motNo;
}

void mbPlayerMotionShiftSet(int playerNo, int motNo, float start, float end, int attr)
{
    GW_PLAYER *playerP = GWPlayerGet(playerNo);
    if(motNo == playerWork[playerNo].motNo) {
        return;
    }
    mbObjMotionShiftSet(mbPlayerObjIDGet(playerNo), motNo, start, end, attr);
    playerWork[playerNo].motNo = motNo;
    if(motNo == 10) {
        mbObjOffsetSet(mbPlayerObjIDGet(playerNo), 0, 4, 0);
    } else {
        mbObjOffsetSet(mbPlayerObjIDGet(playerNo), 0, 0, 0);
    }
}

int mbPlayerMotionCreate(int playerNo, int dataNum)
{
    return mbObjMotionCreate(mbPlayerObjIDGet(playerNo), dataNum);
}

int mbPlayerMotionKill(int playerNo, int motNo)
{
    mbObjMotionKill(mbPlayerObjIDGet(playerNo), motNo);
    return TRUE;
}

void mbPlayerMotionSpeedSet(int playerNo, float speed)
{
    mbObjMotionSpeedSet(mbPlayerObjIDGet(playerNo), speed);
}

void mbPlayerMotionTimeSet(int playerNo, float time)
{
    mbObjMotionTimeSet(mbPlayerObjIDGet(playerNo), time);
}

float mbPlayerMotionTimeGet(int playerNo)
{
    return mbObjMotionTimeGet(mbPlayerObjIDGet(playerNo));
}

float mbPlayerMotionMaxTimeGet(int playerNo)
{
    return mbObjMotionMaxTimeGet(mbPlayerObjIDGet(playerNo));
}

void mbPlayerMotionStartEndSet(int playerNo, float start, float end)
{
    mbObjMotionStartEndSet(mbPlayerObjIDGet(playerNo), start, end);
}

void mbPlayerAttrSet(int playerNo, int attr)
{
    MBOBJID objId = mbPlayerObjIDGet(playerNo);
    mbObjAttrSet(objId, attr);
}

void mbPlayerAttrReset(int playerNo, int attr)
{
    MBOBJID objId = mbPlayerObjIDGet(playerNo);
    mbObjAttrReset(objId, attr);
}

void mbPlayerMotionVoiceOnSet(int playerNo, int motNo, BOOL voiceOn)
{
    mbObjMotionVoiceOnSet(mbPlayerObjIDGet(playerNo), motNo, voiceOn);
}

BOOL mbPlayerMotionEndCheck(int playerNo)
{
    int objId = mbPlayerObjIDGet(playerNo);
    return mbObjMotionShiftIDGet(objId) < 0 && mbObjMotionEndCheck(objId);
}

BOOL mbPlayerMotionEndCheckAll(void)
{
    int i;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(!mbPlayerMotionEndCheck(i)) {
            return FALSE;
        }
    }
    return TRUE;
}

void mbPlayerMotionEndWait(int playerNo)
{
    while(!mbPlayerMotionEndCheck(playerNo)) {
        HuPrcVSleep();
    }
}

void mbPlayerMotIdleSet(int playerNo)
{
    mbPlayerMotionShiftSet(playerNo, 1, 0, 8, HU3D_MOTATTR_LOOP);
}

void mbPlayerCoinSet(int playerNo, int coinNum)
{
    if(!GWTeamGet()) {
        GWPlayerCoinSet(playerNo, coinNum);
    } else {
        GWPlayerCoinSet(mbPlayerTeamFindPlayer(GWPlayerTeamGet(playerNo), 0), coinNum);
    }
}

int mbPlayerCoinGet(int playerNo)
{
    if(!GWTeamGet()) {
        return GWPlayerCoinGet(playerNo);
    } else {
        return GWPlayerCoinGet(mbPlayerTeamFindPlayer(GWPlayerTeamGet(playerNo), 0));
    }
}

void mbPlayerCoinAdd(int playerNo, int coinNum)
{
    GW_PLAYER *playerP;
    if(GWTeamGet()) {
        playerNo = mbPlayerTeamFindPlayer(GWPlayerTeamGet(playerNo), 0);
    }
    playerP = GWPlayerGet(playerNo);
    if(coinNum > 0 && playerP->coinTotal < 999) {
        playerP->coinTotal += coinNum;
        if(playerP->coinTotal > 999) {
            playerP->coinTotal = 999;
        }
    }
    mbPlayerCoinSet(playerNo, coinNum + mbPlayerCoinGet(playerNo));
}

void mbPlayerTeamCoinSet(int teamNo, int coinNum)
{
    GWPlayerCoinSet(mbPlayerTeamFindPlayer(teamNo, 0), coinNum);
}

s16 mbPlayerTeamCoinGet(int teamNo)
{
    return GWPlayerCoinGet(mbPlayerTeamFindPlayer(teamNo, 0));
}

int mbPlayerMaxCoinGet(void)
{
    int maxCoin = 0;
    int i;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(mbPlayerCoinGet(i) >= maxCoin) {
            maxCoin = mbPlayerCoinGet(i);
        }
    }
    return maxCoin;
}

void mbPlayerStarSet(int playerNo, int starNum)
{
    if(!GWTeamGet()) {
        GWPlayerStarSet(playerNo, starNum);
    } else {
        GWPlayerStarSet(mbPlayerTeamFindPlayer(GWPlayerTeamGet(playerNo), 0), starNum);
    }
}

int mbPlayerStarGet(int playerNo)
{
    if(!GWTeamGet()) {
        return GWPlayerStarGet(playerNo);
    } else {
        return GWPlayerStarGet(mbPlayerTeamFindPlayer(GWPlayerTeamGet(playerNo), 0));
    }
}

void mbPlayerStarAdd(int playerNo, int starNum)
{
    int num;
    mbAudFXPlay(8);
    num = starNum + mbPlayerStarGet(playerNo);
    if(num < 0) {
        num = 0;
    }
    mbPlayerStarSet(playerNo, num);
}

void mbPlayerGrpStarSet(int grp, int starNum)
{
    GWPlayerStarSet(mbPlayerTeamFindPlayer(grp, 0), starNum);
}

s16 mbPlayerGrpStarGet(int grp)
{
    return GWPlayerStarGet(mbPlayerTeamFindPlayer(grp, 0));
}

int mbPlayerBestPathGet(void)
{
    int i;
    int bestPlayer;
    int len;
    int minLen;
    minLen = 9999;
    bestPlayer = -1;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        len = mbMasuFind_TypeStepGet2(GwPlayer[i].masuId, 7, 1, 1);
        if(len < minLen) {
            minLen = len;
            bestPlayer = i;
        }
    }
    if(bestPlayer < 0) {
        return mbRandMod(4);
    } else {
        return bestPlayer;
    }
}

int mbPlayerRankGet(int playerNo)
{
    int score[GW_PLAYER_MAX];
    int i;
    int rank;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        score[i] = mbPlayerCoinGet(i) | (mbPlayerStarGet(i) * 1024);
    }
    rank = 0;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(i != playerNo && score[playerNo] < score[i]) {
            rank++;
        }
    }
    return rank;
}

s16 mbPlayerTeamRankGet(int teamNo)
{
    int score[2];
    int i;
    int rank;
    for(i = 0; i < 2; i++) {
        score[i] = mbPlayerTeamCoinGet(i) | (mbPlayerGrpStarGet(i) * 2048);
    }
    rank = 0;
    for(i = 0; i < 2; i++) {
        if(i != teamNo && score[teamNo] < score[i]) {
            rank++;
        }
    }
    return rank;
}

void mbPlayerCapsuleUseSet(int capsuleNo)
{
    GwPlayer[GwSystem.turnPlayerNo].capsuleUse = capsuleNo;
}

int mbPlayerCapsuleUseGet(void)
{
    return GwPlayer[GwSystem.turnPlayerNo].capsuleUse;
}

int mbPlayerCapsuleMaxGet(void)
{
    return (GWTeamGet() == FALSE) ? 3 : 5;
}

static inline s8 *PlayerCapsulePtrGet(int playerNo, int index)
{
    if(!GWTeamGet()) {
        return &GwPlayer[playerNo].capsule[index];
    } else {
        int memberNo = (index < 3) ? 0 : 1;
        int grp = GWPlayerTeamGet(playerNo);
        int no = -1;
        int j;
        for(j = 0; j < GW_PLAYER_MAX; j++) {
            if(grp == GWPlayerTeamGet(j)) {
                no++;
                if(no == memberNo) {
                    break;
                }
            }
        }
        if(j >= GW_PLAYER_MAX) {
            return NULL;
        }
        return &GwPlayer[j].capsule[index - (memberNo * 3)];
    }
}

int mbPlayerCapsuleAdd(int playerNo, int capsuleNo)
{
    GW_PLAYER *playerP = GWPlayerGet(playerNo);
    int max = mbPlayerCapsuleMaxGet();
    int i;
    for(i = 0; i < max; i++) {
        if(mbPlayerCapsuleGet(playerNo, i) != -1) {
            continue;
        }
        *PlayerCapsulePtrGet(playerNo, i) = (s8)capsuleNo;
        return i;
    }
    return -1;
}

int mbPlayerCapsuleRemove(int playerNo, int index)
{
    int capsuleNo = mbPlayerCapsuleGet(playerNo, index);
    GW_PLAYER *playerP = GWPlayerGet(playerNo);
    int max;
    int i;
    if(capsuleNo == -1) {
        return capsuleNo;
    }
    max = mbPlayerCapsuleMaxGet();
    for(i = index; i < max - 1; i++) {
        *PlayerCapsulePtrGet(playerNo, i) = *PlayerCapsulePtrGet(playerNo, i + 1);
    }
    for(; i < max; i++) {
        *PlayerCapsulePtrGet(playerNo, i) = -1;
    }
    return capsuleNo;
}

int mbPlayerCapsuleFind(int playerNo, int capsuleNo)
{
    int max = mbPlayerCapsuleMaxGet();
    int i;
    for(i = 0; i < max; i++) {
        if(capsuleNo == mbPlayerCapsuleGet(playerNo, i)) {
            return i;
        }
    }
    return -1;
}

s8 mbPlayerCapsuleGet(int playerNo, int index)
{
    return *PlayerCapsulePtrGet(playerNo, index);
}

s8 mbPlayerTeamCapsuleGet(int teamNo, int index)
{
    return mbPlayerCapsuleGet(mbPlayerTeamFindPlayer(teamNo, 0), index);
}

static inline int PlayerCountCapsules(int playerNo)
{
    int i;
    for(i = 0; i < 3; i++) {
        if(GwPlayer[playerNo].capsule[i] == -1) {
            break;
        }
    }
    return i;
}

int mbPlayerCapsuleNumGet(int playerNo)
{
    int num = PlayerCountCapsules(playerNo);
    if(GWTeamGet()) {
        int otherPlayer = mbPlayerTeamFind(playerNo);
        num += PlayerCountCapsules(otherPlayer);
        if(num > 5) {
            num = 5;
        }
    }
    return num;
}

int mbPlayerTeamCapsuleNumGet(int teamNo)
{
    return mbPlayerCapsuleNumGet(mbPlayerTeamFindPlayer(teamNo, 0));
}

BOOL mbPlayerKettouCheck(int playerNo, s16 masuId)
{
    int i;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(playerNo != i && masuId == GwPlayer[i].masuId) {
            return TRUE;
        }
    }
    return FALSE;
}

void mbPlayerWinLoseVoicePlay(int playerNo, int motNo, int seId)
{
    MBOBJ *objP = mbObjGet(mbPlayerObjIDGet(playerNo));
    u8 charNo = GwPlayer[playerNo].charNo;
    CharWinLoseVoicePlay(charNo, objP->motId[motNo], seId);
}

int mbPlayerVoicePanPlay(int playerNo, s16 seId)
{
    mbObjSePlay(mbPlayerObjIDGet(playerNo), seId);
}

int mbPlayerVoicePlay(int playerNo, s16 seId)
{
    HuVecF pos;
    u8 pan;
    mbPlayerPosGet(playerNo, &pos);
    pan = mbAudFXPosPanGet(&pos);
    return CharFXPlayVolPan(GwPlayer[playerNo].charNo, seId, 127, pan);
}

void mbPlayerDispSet(int playerNo, BOOL dispF)
{
    if(GwPlayer[playerNo].masuId == 0) {
        dispF = FALSE;
    }
    mbObjDispSet(mbPlayerObjIDGet(playerNo), dispF);
}

int mbPlayerDispGet(int playerNo)
{
    return mbObjDispGet(mbPlayerObjIDGet(playerNo));
}

GXColor mbPlayerColorGet(int playerNo)
{
    GXColor color[13] = {
        { 227, 67, 67, 255 },
        { 68, 67, 227, 255 },
        { 241, 158, 220, 255 },
        { 67, 228, 68, 255 },
        { 138, 60, 180, 255 },
        { 227, 228, 68, 255 },
        { 192, 192, 192, 255 },
        { 227, 227, 227, 255 },
        { 40, 227, 227, 255 },
        { 227, 139, 40, 255 },
        { 180, 40, 40, 255 },
        { 40, 180, 40, 255 },
        { 40, 40, 180, 255 }
    };
    return color[GwPlayer[playerNo].charNo];
}

void mbPlayerBlackoutSet(BOOL value)
{
    blackoutF = value;
}

BOOL mbPlayerBlackoutGet(void)
{
    return blackoutF;
}

void mbPlayerMasuCornerSet(int playerNo, s8 cornerNo)
{
    playerWork[playerNo].masuCorner = cornerNo;
}

s8 mbPlayerMasuCornerGet(int playerNo)
{
    return playerWork[playerNo].masuCorner;
}

static void MasuCoinExec(int playerNo, int coinNum)
{
    HuVecF pos;
    s8 dispId;
    BOOL idleF;
    if(coinNum < 0) {
        omVibrate(playerNo, 20, 4, 4);
    }
    mbPlayerRotateStart(playerNo, 0, 15);
    if(GwSystem.last5Effect == 1) {
        coinNum *= 3;
    }
    mbPlayerPosGet(playerNo, &pos);
    pos.y += 250;
    if(coinNum >= 0) {
        mbAudFXPlay(0x44D);
    } else {
        mbAudFXPlay(0x44E);
    }
    dispId = mbCoinDispMasuCreate(&pos, coinNum, 0);
    while(playerWork[playerNo].rotateObj != NULL) {
        HuPrcVSleep();
    }
    if(coinNum >= 0) {
        mbPlayerWinLoseVoicePlay(playerNo, 12, 0x243);
        mbPlayerMotionShiftSet(playerNo, 12, 0, 4, HU3D_MOTATTR_NONE);
    } else {
        mbPlayerWinLoseVoicePlay(playerNo, 13, 0x249);
        mbPlayerMotionShiftSet(playerNo, 13, 0, 4, HU3D_MOTATTR_NONE);
    }
    mbCoinAddExec(playerNo, coinNum);
    mbCameraMoveWait();
    idleF = FALSE;
    while(!mbCoinDispKillCheck(dispId) || !idleF) {
        if(mbPlayerMotionEndCheck(playerNo) && !idleF) {
            mbPlayerMotIdleSet(playerNo);
            idleF = TRUE;
        }
        HuPrcVSleep();
    }
}

void mbPlayerPlusMasuExec(int playerNo)
{
    mbCameraMoveOnSet(TRUE);
    mbCameraPlayerViewSet(playerNo, 0);
    MasuCoinExec(playerNo, 3);
}

void mbPlayerCapCoinMasuExec(int playerNo)
{
    mbCameraMoveOnSet(TRUE);
    mbCameraPlayerViewSet(playerNo, 0);
    MasuCoinExec(playerNo, 5);
}

void mbPlayerMinusMasuExec(int playerNo)
{
    mbCameraMoveOnSet(TRUE);
    mbCameraPlayerViewSet(playerNo, 0);
    MasuCoinExec(playerNo, -3);
}
