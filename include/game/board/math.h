#ifndef _GAME_BOARD_MATH_H
#define _GAME_BOARD_MATH_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbMathInit(void);
void mbMathClose(void);
float mbCosDeg(float deg);
float mbCosRad(float rad);
float mbSinDeg(float deg);
float mbSinRad(float rad);
void mbMtxRotTrigX(Mtx m, float sinA, float cosA);
void mbMtxRotTrigY(Mtx m, float sinA, float cosA);
void mbMtxRotTrigZ(Mtx m, float sinA, float cosA);
void mbMtxRotTrigScaleX(Mtx m, HuVecF *scale, float sinA, float cosA);
void mbMtxRotTrigScaleY(Mtx m, HuVecF *scale, float sinA, float cosA);
void mbMtxRotTrigScaleZ(Mtx m, HuVecF *scale, float sinA, float cosA);
void mbMtxRotAxisDeg(Mtx m, s8 axis, float deg);
void mbMtxRotAxisRad(Mtx m, s8 axis, float rad);
void mbMtxRotXDeg(Mtx m, float deg);
void mbMtxRotXRad(Mtx m, float rad);
void mbMtxRotYDeg(Mtx m, float deg);
void mbMtxRotYRad(Mtx m, float rad);
void mbMtxRotZDeg(Mtx m, float deg);
void mbMtxRotZRad(Mtx m, float rad);
void mbMtxScaleRotXDeg(Mtx m, HuVecF *scale, float deg);
void mbMtxScaleRotYDeg(Mtx m, HuVecF *scale, float deg);
void mbMtxScaleRotZDeg(Mtx m, HuVecF *scale, float deg);
void mbMtxRot(Mtx m, float x, float y, float z);
void mbMtxTransCat(Mtx m, float x, float y, float z);
u32 mbRandMod(u32 max);
float mbVecMagXZ(HuVecF *a, HuVecF *b);
BOOL mbVecMagXZCheck(HuVecF *a, HuVecF *b, float maxDist);
float mbAngleWrap(float angle);
void mbAngleWrapV(HuVecF *angle);
BOOL mbAngleAdd(float *dest, float angle, float speed);
BOOL mbAngleMoveTo(float *dest, float angle, float speed);
float mbAngleWrap2(float a, float b);
BOOL mbVecMagCheck(HuVecF *a, HuVecF *b, float dist);
void mbMtxLookAtCalc(Mtx dest, HuVecF *eye, HuVecF *up, HuVecF *target);
void mbPos3Dto2D(HuVecF *src, HuVecF *dst);
void mbPos3DtoNorm(HuVecF *src, s16 camBit, HuVecF *dst);
void mbPos2Dto3D(HuVecF *src, HuVecF *dst);
void mbNormPosto3D(HuVecF *src, s16 camBit, HuVecF *dst);
void mbNormPosto2D(HuVecF *src, HuVecF *dst);
float mbBezierCalc(float a, float b, float c, float t);
void mbBezierCalcV(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *dst, float t);
void mbBezierCalcVList(HuVecF *src, HuVecF *dst, float t);
float mbBezierCalcSlope(float a, float b, float c, float t);
void mbBezierCalcSlopeV(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *dst, float t);
float mbHermiteCalc(float a, float b, float c, float d, float t);
void mbHermiteCalcV(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, HuVecF *dst, float t);
float mbHermiteCalcSlope(float a, float b, float c, float d, float t);
float mbAngleLerp(float a, float b, float t);
float mbAngleEaseOut(float a, float b, float t);
float mbAngleEaseIn(float a, float b, float t);
float mbMathDistScale(HuVecF *src, float scale, HuVecF *dst);
void mbObjCullInit(s16 mdlId);

#endif
