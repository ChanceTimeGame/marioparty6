#ifndef _GAME_BOARD_MATH_H
#define _GAME_BOARD_MATH_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbMathInit(void);
void mbMathClose(void);
float mbCosDeg(float f0);
float mbCosRad(float f0);
float mbSinDeg(float f0);
float mbSinRad(float f0);
void mbMtxRotTrigX(void *arg0, float f0, float f1);
void mbMtxRotTrigY(void *arg0, float f0, float f1);
void mbMtxRotTrigZ(void *arg0, float f0, float f1);
void mbMtxRotTrigScaleX(void *arg0, HuVec2f *arg1, float f0, float f1);
void mbMtxRotTrigScaleY(void *arg0, HuVecF *arg1, float f0, float f1);
void mbMtxRotTrigScaleZ(void *arg0, HuVecF *arg1, float f0, float f1);
void mbMtxRotAxisDeg(void *arg0, s8 arg1, float f0);
void mbMtxRotAxisRad(void *arg0, s8 arg1, float f0);
void mbMtxRotXDeg(void *arg0, float f0);
void mbMtxRotXRad(void *arg0, float f0);
void mbMtxRotYDeg(void *arg0, float f0);
void mbMtxRotYRad(void *arg0, float f0);
void mbMtxRotZDeg(void *arg0, float f0);
void mbMtxRotZRad(void *arg0, float f0);
void mbMtxScaleRotXDeg(void *arg0, HuVec2f *arg1, float f0);
void mbMtxScaleRotYDeg(void *arg0, HuVecF *arg1, float f0);
void mbMtxScaleRotZDeg(void *arg0, HuVecF *arg1, float f0);
void mbMtxRot(void *arg0, float f0, float f1, float f2);
void mbMtxTransCat(void *arg0, float f0, float f1, float f2);
int mbRandMod(int arg0);
float mbVecMagXZ(HuVecF *arg0, HuVecF *arg1);
int mbVecMagXZCheck(HuVecF *arg0, HuVecF *arg1, float f0);
float mbAngleWrap(float angle);
void mbAngleWrapV(HuVecF *angle);
BOOL mbAngleAdd(float *dest, float angle, float speed);
BOOL mbAngleMoveTo(float *dest, float angle, float speed);
float mbAngleWrap2(float f0, float f1);
BOOL mbVecMagCheck(HuVecF *a, HuVecF *b, float dist);
void mbMtxLookAtCalc(Mtx dest, HuVecF *eye, HuVecF *up, HuVecF *target);
void mbPos3Dto2D(void *arg0, HuVecF *arg1);
void mbPos3DtoNorm(void *arg0, s16 arg1, HuVecF *arg2);
void mbPos2Dto3D(HuVecF *arg0, HuVecF *arg1);
void mbNormPosto3D(HuVecF *arg0, s16 arg1, HuVecF *arg2);
void mbNormPosto2D(HuVecF *arg0, HuVecF *arg1);
float mbBezierCalc(float a, float b, float c, float t);
void mbBezierCalcV(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *dst, float t);
void mbBezierCalcVList(HuVecF *src, HuVecF *dst, float t);
float mbBezierCalcSlope(float a, float b, float c, float t);
void mbBezierCalcSlopeV(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *dst, float t);
float mbHermiteCalc(float a, float b, float c, float d, float t);
void mbHermiteCalcV(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, HuVecF *dst, float t);
float mbHermiteCalcSlope(float a, float b, float c, float d, float t);
float mbAngleLerp(float a, float b, float t);
float mbAngleEaseOut(float f0, float f1, float f2);
float mbAngleEaseIn(float f0, float f1, float f2);
float mbMathDistScale(void *arg0, HuVecF *arg1, float f0);
void mbObjCullInit(int arg0);

#endif
