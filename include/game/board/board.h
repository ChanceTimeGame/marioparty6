#ifndef _GAME_BOARD_BOARD_H
#define _GAME_BOARD_BOARD_H

#include "dolphin.h"
#include "humath.h"
#include "game/process.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/window.h"

typedef void (*MBCREATEHOOK)(void);
typedef void (*MBKILLHOOK)(void);
typedef void (*MBLIGHTHOOK)(void);
typedef void (*MBEVENTHOOK)(void);

extern OMOBJMAN *mbObjMan;
extern HUPROCESS *mbMainProc;
extern int mbSaveNewF;

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbObjectSetup(int boardNo, MBCREATEHOOK createHook, MBKILLHOOK killHook);
void mbChangeTimeSet(void);
BOOL mbNextTimeSet(void);
void mbChangeTime(void);
void mbNextTime(void);
void mbOvlCall(int ovl);
BOOL mbReturnMgCheck(void);
void mbDirClose(void);
void mbLightFuncSet(MBLIGHTHOOK setFunc, MBLIGHTHOOK resetFunc);
void mbLightSet(void);
void mbLightReset(void);
void mbPauseEnableSet(void);
void mbPauseEnableReset(void);
BOOL mbPauseEnableCheck(void);
s32 mbBGRead(int dataNum);
void mbBGReadWait(s32 statId);
void mbev_NextTimeSet(MBEVENTHOOK func);
void mbev_LoadTimeSet(MBEVENTHOOK func);
void mbev_TurnStartSet(MBEVENTHOOK func);
void mbev_TurnEndSet(MBEVENTHOOK func);
void fn_8014C3B4(int value);
void fn_8014C3BC(int value);
int fn_8014C3C4(void);
int fn_8014C3CC(void);
void mbSaveInit(int boardNo);
void mbSaveStoryInit(int arg0, int arg1, int arg2);
void mbSavePartyInit(int tagF, int bonusStarF, int mgPack, int turnMax, int handicapP1, int handicapP2, int handicapP3, int handicapP4);
void fn_8014C818(void);
void fn_8014C81C(void);

#endif
