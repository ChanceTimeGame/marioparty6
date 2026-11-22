#ifndef _GAME_BOARD_STAR_H
#define _GAME_BOARD_STAR_H

#include "game/board/board.h"

void mbStarReset(void);
void mbStarInit(void);
void mbStarClose(void);
int mbStarObjCreate(void);
void mbStarObjPosSet(int no, float posX, float posY, float posZ);
void mbStarObjPosSetV(int no, HuVecF *pos);
void mbStarObjRotSet(int no, float rotX, float rotY, float rotZ);
void mbStarObjRotSetV(int no, HuVecF *rot);
void mbStarObjScaleSet(int no, float scaleX, float scaleY, float scaleZ);
void mbStarObjScaleSetV(int no, HuVecF *scale);
void mbStarObjPosGet(int no, HuVecF *pos);
void mbStarObjRotGet(int no, HuVecF *rot);
void mbStarObjScaleGet(int no, HuVecF *scale);
void mbStarObjKill(int no);
void mbStarObjDispSet(int no, BOOL dispF);
void mbStarObjDispFlagSet(int no, BOOL dispF);
void mbStarChestCreate(int no, int playerNo);
void mbStarMoveHookSet(void *hook);
void mbStarMasuFuncSet(void *func);
void mbStarMasuNextSet(int masuId);
void mbStarGetExec(int playerNo, HuVecF *pos);
void mbStarAddNumSet(int num);
void mbStarStub(void);
void mbev_StarMasu(int playerNo, int masuId);
void mbev_StarFreeMasu(int playerNo);
void mbStarGetMain(int playerNo, HuVecF *pos, int starNum, BOOL focusF);
void mbStarGetPosExec(int playerNo, HuVecF *pos);
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
void mbStarMasuDispSet(int masuId, BOOL dispF);
void mbStarDispSetAll(BOOL dispF);
void mbStarObjDispSetAll(BOOL effF);
int mbStarAddProcExec(int playerNo, int num, BOOL dispF, BOOL instF);
int mbStarAddDispExec(int playerNo, int num, BOOL dispF, BOOL instF);
int mbStarAddExec(int playerNo, int num);
void mbStarAddAllProcExecV(int *numTbl, int *dispTbl, BOOL instF);
void mbStarAddAllProcExec(int numP1, int numP2, int numP3, int numP4, BOOL dispF, BOOL instF);
void mbStarAddAllExec(int numP1, int numP2, int numP3, int numP4);
int mbStarDispPlayerCreate(int playerNo, int num);
int mbStarDispCreate(int playerNo, HuVecF *pos, int num);
int mbStarDispCheck(int playerNo);
void mbZtarObjInit(void);
void mbZtarObjClose(void);
void mbZtarObjDispFlagSet(int no, BOOL dispF);
void mbZtarMasuNextSet(int masuId);
void mbZtarGetExec(int playerNo);
void mbZtarGetMain(int playerNo, HuVecF *pos, int num, BOOL focusF);

#endif
