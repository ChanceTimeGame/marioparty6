#ifndef _GAME_BOARD_PLAYER_H
#define _GAME_BOARD_PLAYER_H

#include "game/board/board.h"

typedef int (*MBPLAYERTURNHOOK)(void); /* startTurnHook/endTurnHook: TRUE clears the hook */
typedef void (*MBPLAYERMOVEHOOK)(int playerNo); /* moveHook: replaces one step of PlayerMove */
typedef void (*MBTURNHOOK)(int playerNo); /* mbPlayerTurnInitHookSet / mbPlayerTurnCloseHookSet */

typedef struct MbPlayerWork_s {
    /* 0x00 */ s16 objId; /* mbPlayerObjIDGet */
    /* 0x02 */ s16 motNo;
    /* 0x04 */ s8 unk4;
    /* 0x05 */ s8 masuCorner;
    /* 0x06 */ s16 unk6; /* PlayerMove: the space being walked to */
    /* 0x08 */ s16 masuNext; /* -1 at each step; frames left while unkA is set */
    /* 0x0A */ s16 unkA; /* set while mbPlayerMasuMove/MoveSpeed walk */
    /* 0x0C */ s16 unkC; /* walk state: 0 none, 1 walk, 2 jump, 3 slide, 4 move hook */
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8 stopF : 1; /* PlayerMove sleeps; then turns to face the camera */
    /* 0x10 */ u8 playerNo : 2;
    /* 0x10 */ u8 stopDoneF : 1;
    /* 0x10 */ u8 unk10_3 : 1; /* cleared during a jump, else set */
    /* 0x10 */ u8 unk10_0 : 3;
    /* 0x11 */ u8 unk11[3];
    /* 0x14 */ OMOBJ *rotateObj; /* mbPlayerRotateStart */
    /* 0x18 */ OMOBJ *unk18; /* mbPlayerMoveMain's walk object */
    /* 0x1C */ void *unk1C;
    /* 0x20 */ OMOBJ *unk20; /* mbMoveNumCreateColor's number object */
    /* 0x24 */ OMOBJ *unk24; /* collision object (PlayerColOMExec) */
    /* 0x28 */ OMOBJ *unk28;
    /* 0x2C */ OMOBJ *unk2C;
    /* 0x30 */ MBPLAYERTURNHOOK startTurnHook; /* mbPlayerStartTurnHookSet */
    /* 0x34 */ MBPLAYERTURNHOOK endTurnHook; /* mbPlayerEndTurnHookSet */
    /* 0x38 */ MBPLAYERMOVEHOOK moveHook; /* mbPlayerMoveHookSet */
    /* 0x3C */ HuVecF unk3C;
    /* 0x48 */ HUPROCESS *moveProc; /* HuPrcChildCreate result */
    /* 0x4C */ void *unk4C; /* freed with HuMemDirectFree in mbPlayerClose */
} MBPLAYERWORK; /* size 0x50 (playerWork is 0x140 = 4 players) */

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbPlayerInit(BOOL noEventF);
void mbPlayerClose(void);
MBPLAYERWORK *mbPlayerWorkGet(int playerNo);
void mbPlayerTurnInitHookSet(MBTURNHOOK hook);
void mbPlayerTurnCloseHookSet(MBTURNHOOK hook);
void mbPlayerStartTurnHookSet(int playerNo, MBPLAYERTURNHOOK hook);
void mbPlayerEndTurnHookSet(int playerNo, MBPLAYERTURNHOOK hook);
void mbPlayerMoveHookSet(int playerNo, MBPLAYERMOVEHOOK hook);
void mbTurnExec(BOOL intrF);
void mbSingleTurnExec(BOOL intrF);
int mbPlayerDiceTypeGet(int diceMode);
void mbPlayerMasuMoveTo(int playerNo, int masuId, BOOL waitF);
void mbPlayerMasuMove(int playerNo, BOOL waitF);
void mbPlayerMasuMovePos(int playerNo, HuVecF *pos, BOOL waitF);
void mbPlayerMasuMoveSpeed(int playerNo, int masuId, s16 maxTime, BOOL waitF);
void mbPlayerMoveExec(int playerNo, HuVecF *srcPos, HuVecF *dstPos, s16 maxTime, HuVecF *rot, BOOL waitF);
void mbPlayerMoveMain(int playerNo, HuVecF *srcPos, HuVecF *dstPos, u32 motNo, float motSpeed, u32 motAttr, s16 maxTime, HuVecF *rot, BOOL waitF);
void mbPlayerRotateStart(int playerNo, s16 endAngle, s16 maxTime);
BOOL mbPlayerRotateCheck(int playerNo);
BOOL mbPlayerRotateCheckAll(void);
void mbPlayerDiceMotExec(int playerNo);
void mbMoveNumCreateColor(int playerNo, BOOL dispF, int color);
void mbMoveNumCreate(int playerNo, BOOL dispF);
void mbMoveNumKill(int playerNo);
void mbMoveNumDispSet(int playerNo, BOOL dispF);
void mbev_PlayerColMasuAllSet(int *masuTbl, BOOL snapF);
void mbev_PlayerColMasu(int playerNo, int masuId, BOOL snapF);
void mbev_PlayerColCircleAdd(int playerNo, int masuId, BOOL snapF, float radius);
void mbev_PlayerColMasuAdd(int playerNo, int masuId, BOOL snapF);
void mbev_PlayerColBall(int masuId, int *playerMasuTbl, HuVecF *posOut);
void mbev_PlayerColMasuSet(int playerNo, int masuId, BOOL snapF);
int mbPlayerColCheck(void);
void mbev_PlayerColReserve(int playerNo, int masuId, BOOL snapF);
void mbev_PlayerColSet(int playerNo, int masuId);
void mbPlayerColSnapSet(BOOL snapF);
void mbPlayerColSnapPlayerSet(int playerNo, BOOL snapF);
int mbPlayerColSnapGet(int playerNo);
void mbPlayerColRestSet(int playerNo, BOOL restF);
void mbPlayerColFirstSet(int playerNo);
void mbPlayerColOrderReset(void);
void mbPlayerEyeMatDarkSet(int playerNo, BOOL darkF);
void mbPlayerMatClone(int playerNo);
void mbPlayerMetalSet(int playerNo, BOOL metalF);
void mbPlayerEffectSet(int playerNo, BOOL effectF);
void mbPlayerMetalColorSet(GXColor *shadow, GXColor *hilite);
void mbPlayerBiriQSet(int playerNo, BOOL biriQF);
void mbPlayerSwap(int playerNo1, int playerNo2);
u32 mbPlayerNameMesGet(int playerNo);
char *mbPlayerNameGet(int playerNo);
int mbPlayerTagNameMesGet(int arg0);
void mbPlayerAmbSet(int playerNo, float ambR, float ambG, float ambB);
s16 mbPlayerObjIDGet(int arg0);
s16 mbPlayerModelIDGet(int arg0);
BOOL mbPlayerAllComCheck(void);
int mbPlayerTeamFind(int arg0);
int mbPlayerTeamFindOther(int arg0);
int mbPlayerTeamFindPlayer(int arg0, int arg1);
int mbPlayerTeamFindOpp(int arg0);
int mbPlayerTeamCheckSame(int arg0, int arg1);
int mbPlayerTeamGet(int arg0);
float mbPlayerWalkSpeedGet(void);
void mbPlayerLayerSet(int playerNo, int layer);
void mbPlayerCameraSet(int playerNo, u16 bit);
void mbPlayerCullRadiusSet(int arg0, float f0);
void mbPlayerStubValSet(int playerNo, BOOL value);
void mbPlayerPosReset(int playerNo);
void mbPlayerPosResetAll(void);
void mbPlayerMtxSet(int playerNo, Mtx *matrix);
void mbPlayerMtxGet(int arg0, void *arg1);
void mbPlayerPosSetV(int playerNo, HuVecF *pos);
void mbPlayerPosSet(int playerNo, float posX, float posY, float posZ);
void mbPlayerPosGet(int playerNo, HuVecF *pos);
void mbPlayerRotSetV(int playerNo, HuVecF *rot);
void mbPlayerRotSet(int playerNo, float rotX, float rotY, float rotZ);
void mbPlayerRotGet(int playerNo, HuVecF *rot);
void mbPlayerRotYSet(int playerNo, float rotY);
float mbPlayerRotYGet(int playerNo);
void mbPlayerScaleSetV(int playerNo, HuVecF *scale);
void mbPlayerScaleSet(int playerNo, float scaleX, float scaleY, float scaleZ);
void mbPlayerScaleGet(int playerNo, HuVecF *scale);
void mbPlayerMotionSet(int playerNo, int motNo, int attr);
s16 mbPlayerMotionGet(int arg0);
void mbPlayerMotionShiftSet(int playerNo, int motNo, float start, float end, int attr);
int mbPlayerMotionCreate(int playerNo, int dataNum);
int mbPlayerMotionKill(int playerNo, int motNo);
void mbPlayerMotionSpeedSet(int playerNo, float speed);
void mbPlayerMotionTimeSet(int playerNo, float time);
float mbPlayerMotionTimeGet(int playerNo);
float mbPlayerMotionMaxTimeGet(int playerNo);
void mbPlayerMotionStartEndSet(int playerNo, float start, float end);
void mbPlayerAttrSet(int arg0, int arg1);
void mbPlayerAttrReset(int arg0, int arg1);
void mbPlayerMotionVoiceOnSet(int playerNo, int motNo, BOOL voiceOn);
BOOL mbPlayerMotionEndCheck(int playerNo);
BOOL mbPlayerMotionEndCheckAll(void);
void mbPlayerMotionEndWait(int playerNo);
void mbPlayerMotIdleSet(int playerNo);
void mbPlayerCoinSet(int playerNo, int coinNum);
int mbPlayerCoinGet(int playerNo);
void mbPlayerCoinAdd(int playerNo, int coinNum);
void mbPlayerTeamCoinSet(int arg0, int arg1);
s16 mbPlayerTeamCoinGet(int arg0);
int mbPlayerMaxCoinGet(void);
void mbPlayerStarSet(int playerNo, int starNum);
int mbPlayerStarGet(int playerNo);
void mbPlayerStarAdd(int playerNo, int starNum);
void mbPlayerGrpStarSet(int grp, int starNum);
s16 mbPlayerGrpStarGet(int grp);
int mbPlayerBestPathGet(void);
int mbPlayerRankGet(int playerNo);
s16 mbPlayerTeamRankGet(int arg0);
void mbPlayerCapsuleUseSet(int capsuleNo);
int mbPlayerCapsuleUseGet(void);
int mbPlayerCapsuleMaxGet(void);
int mbPlayerCapsuleAdd(int playerNo, int capsuleNo);
int mbPlayerCapsuleRemove(int playerNo, int index);
int mbPlayerCapsuleFind(int playerNo, int capsuleNo);
s8 mbPlayerCapsuleGet(int playerNo, int index);
s8 mbPlayerTeamCapsuleGet(int arg0, int arg1);
int mbPlayerCapsuleNumGet(int playerNo);
int mbPlayerTeamCapsuleNumGet(int arg0);
BOOL mbPlayerKettouCheck(int playerNo, s16 masuId);
void mbPlayerWinLoseVoicePlay(int playerNo, int motNo, int seId);
int mbPlayerVoicePanPlay(int playerNo, s16 seId);
int mbPlayerVoicePlay(int playerNo, s16 seId);
void mbPlayerDispSet(int playerNo, BOOL dispF);
int mbPlayerDispGet(int arg0);
GXColor mbPlayerColorGet(int playerNo);
void mbPlayerBlackoutSet(BOOL value);
BOOL mbPlayerBlackoutGet(void);
void mbPlayerMasuCornerSet(int playerNo, s8 cornerNo);
s8 mbPlayerMasuCornerGet(int playerNo);
void mbPlayerPlusMasuExec(int arg0);
void mbPlayerCapCoinMasuExec(int arg0);
void mbPlayerMinusMasuExec(int arg0);

#endif
