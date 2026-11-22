#ifndef _GAME_BOARD_STAR_H
#define _GAME_BOARD_STAR_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbStarReset(void);
void mbStarInit(void);
void mbStarClose(void);
int mbStarObjCreate(void);
void mbStarObjPosSet(int arg0, float f0, float f1, float f2);
void mbStarObjPosSetV(int arg0, HuVecF *arg1);
void mbStarObjRotSet(int arg0, float f0, float f1, float f2);
void mbStarObjRotSetV(int arg0, HuVecF *arg1);
void mbStarObjScaleSet(int arg0, float f0, float f1, float f2);
void mbStarObjScaleSetV(int arg0, HuVecF *arg1);
void mbStarObjPosGet(int arg0, HuVecF *arg1);
void mbStarObjRotGet(int arg0, HuVecF *arg1);
void mbStarObjScaleGet(int arg0, HuVecF *arg1);
void mbStarObjKill(int arg0);
void mbStarObjDispSet(int objNo, u8 dispF);
void mbStarObjDispFlagSet(int arg0, u8 arg1);
void mbStarChestCreate(int arg0, int arg1);
void mbStarMoveHookSet(void *arg0);
void mbStarMasuFuncSet(void *arg0);
void mbStarMasuNextSet(s16 arg0);
void mbStarGetExec(int playerNo, HuVecF *pos);
void mbStarAddNumSet(int arg0);
void mbStarStub(void);
void mbev_StarMasu(int arg0);
void mbev_StarFreeMasu(int arg0);
void mbStarGetMain(int arg0, int arg1, int arg2, int arg3);
void mbStarGetPosExec(int arg0, int arg1);
void mbStarStub2(void);
void mbStarMapViewProcExec(int playerNo, HuVecF *pos, int no);
void mbStarMapViewExec(int playerNo, int no);
void mbStarNumSet(int num);
int mbStarNumGet(void);
void mbStarFlagInit(void);
void mbStarFlagSet(int no);
void mbStarFlagReset(int no);
u8 mbStarFlagGet(void);
void mbStarNextNoSet(s8 nextNo);
u8 mbStarNextNoGet(void);
void mbStarNoSet(int no);
int mbStarNoRandGet(void);
void mbStarNoDispSet(void);
void mbStarMasuDispSet(int masuId, u8 dispF);
void mbStarDispSetAll(u8 dispF);
void mbStarObjDispSetAll(u8 arg0);
int mbStarAddProcExec(int arg0, int arg1, int arg2, int arg3);
int mbStarAddDispExec(int arg0, int arg1, int arg2, int arg3);
int mbStarAddExec(int arg0, int arg1);
void mbStarAddAllProcExecV(void *arg0, void *arg1, int arg2);
void mbStarAddAllProcExec(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5);
void mbStarAddAllExec(int arg0, int arg1, int arg2, int arg3);
int mbStarDispPlayerCreate(int arg0, int arg1);
int mbStarDispCreate(int arg0, HuVecF *arg1, int arg2);
int mbStarDispCheck(int arg0);
void mbZtarObjInit(void);
void mbZtarObjClose(void);
void mbZtarObjDispFlagSet(int arg0, u8 arg1);
void mbZtarMasuNextSet(s16 arg0);
void mbZtarGetExec(int arg0);
void mbZtarGetMain(int arg0, int arg1, int arg2);

#endif
