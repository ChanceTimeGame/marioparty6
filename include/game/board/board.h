#ifndef _GAME_BOARD_BOARD_H
#define _GAME_BOARD_BOARD_H

#include "dolphin.h"
#include "humath.h"
#include "game/process.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/window.h"

extern OMOBJMAN *mbObjMan;
extern HUPROCESS *mbMainProc;
extern int mbSaveNewF;

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbObjectSetup(u8 arg0, void *arg1, void *arg2);
void mbChangeTimeSet(void);
int mbNextTimeSet(void);
void mbChangeTime(void);
void mbNextTime(void);
void mbOvlCall(int arg0);
int mbReturnMgCheck(void);
void mbDirClose(void);
void mbLightFuncSet(void *arg0, void *arg1);
void mbLightSet(void);
void mbLightReset(void);
void mbPauseEnableSet(void);
void mbPauseEnableReset(void);
int mbPauseEnableCheck(void);
int mbBGRead(int arg0);
void mbBGReadWait(int arg0);
void mbev_NextTimeSet(void *arg0);
void mbev_LoadTimeSet(int arg0);
void mbev_TurnStartSet(int arg0);
void mbev_TurnEndSet(int arg0);
void fn_8014C3B4(int arg0);
void fn_8014C3BC(int arg0);
int fn_8014C3C4(void);
int fn_8014C3CC(void);
void mbSaveInit(u8 arg0);
void mbSaveStoryInit(int arg0, int arg1, int arg2);
void mbSavePartyInit(u8 arg0, u8 arg1, u8 arg2, u8 arg3, s8 arg4, s8 arg5, s8 arg6, s8 arg7);
void fn_8014C818(void);
void fn_8014C81C(void);

#endif
