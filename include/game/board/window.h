#ifndef _GAME_BOARD_WINDOW_H
#define _GAME_BOARD_WINDOW_H

#include "game/board/board.h"

typedef struct MbWin_s {
    /* 0x00 */ HUPROCESS *proc;
    /* 0x04 */ int winId;
    /* 0x08 */ u32 mess;
    /* 0x0C */ u8 no;
    /* 0x0D */ s8 mesSpeed;
    /* 0x0E */ u8 unkE[2];
    /* 0x10 */ u32 attr;
    /* 0x14 */ int type;
    /* 0x18 */ s8 speakerNo;
    /* 0x1A */ s16 prio;
    /* 0x1C */ int playerNo;
    /* 0x20 */ HuVec2f pos;
    /* 0x28 */ HuVec2f size;
    /* 0x30 */ HuVec2f scale;
    /* 0x38 */ HuVec2f unk38;
    /* 0x40 */ int unk40;
    /* 0x44 */ int unk44;
    /* 0x48 */ int unk48;
    /* 0x4C */ u8 unk4C[0xCC - 0x4C]; /* MP5 choiceDisable[]/insertMes[]; layout unproven */
    /* 0xCC */ int unkCC;
    /* 0xD0 */ void *unkD0;
} MBWIN; /* size 0xD4 (stride in mbWinData) */

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbWinInit(void);
void mbWinClose(void);
int mbWinCreate(int type, u32 mess, s8 speakerNo);
int mbWinCreateChoice(int type, u32 mess, int speakerNo, int choiceNo);
int mbWinCreateHelp(void *arg0);
int mbWinCreateFrame(int arg0, int arg1, int arg2, int arg3);
int mbWinCreateTime(int arg0, int arg1, int arg2);
int mbWinCreateBlank(void);
int mbWinCreateBlankFrame(int arg0);
MBWIN *mbWinGet(int winNo);
void mbWinKill(s16 winNo);
void mbWinTopKill(void);
void mbWinKillAll(void);
void mbWinPosSet(s16 winNo, s16 posX, s16 posY);
void mbWinTopPosSet(s16 arg0, s16 arg1);
void mbWinPosGet(s16 winNo, HuVec2f *pos);
void mbWinTopPosGet(HuVec2f *arg0);
void mbWinSizeSet(s16 winNo, s16 sizeX, s16 sizeY);
void mbWinTopSizeSet(int arg0, int arg1);
void mbWinMesMaxSizeGet(s16 winNo, HuVec2f *size);
void mbWinTopMesMaxSizeGet(void *arg0);
void mbWinScaleSet(s16 winNo, float scaleX, float scaleY);
void mbWinTopScaleSet(float f0, float f1);
void mbWinScaleGet(s16 winNo, HuVec2f *scale);
void mbWinTopScaleGet(HuVec2f *arg0);
int mbWinTopChoiceGet(void);
int mbWinChoiceGet(s16 winNo);
void mbWinPause(s16 winNo);
void mbWinTopPause(void);
void mbWinInsertMesSet(int arg0, void *arg1, int arg2);
void mbWinTopInsertMesSet(void *arg0, int arg1);
BOOL mbWinDoneCheck(s16 winNo);
int mbWinTopDoneCheck(void);
void mbWinWait(s16 winNo);
void mbWinTopWait(void);
void mbWinAttrSet(s16 winNo, u32 attr);
void mbWinTopAttrSet(int arg0);
void mbWinAttrReset(s16 winNo, u32 attr);
void mbWinTopAttrReset(int arg0);
void mbWinChoiceDisable(s16 winNo, int choiceNo);
void mbWinTopChoiceDisable(int arg0);
void mbWinMesSpeedSet(s16 winNo, int speed);
void mbWinTopMesSpeedSet(int arg0);
void mbWinMesColSet(s16 winNo, u8 mesCol);
void mbWinTopMesColSet(u8 arg0);
s16 mbWinChoiceNowGet(s16 winNo);
s16 mbWinTopChoiceNowGet(void);
void mbWinPriSet(s16 winNo, s16 prio);
void mbWinTopPriSet(int arg0);
s16 mbWinChoiceNowGet2(s16 winNo);
int mbWinTopChoiceNowGet2(void);
void mbWinPlayerDisable(s16 winNo, int playerNo);
void mbWinTopPlayerDisable(int arg0);
void mbWinTopComKeyHookSet(void *arg0);
void mbWinDispSet(s16 winNo, BOOL dispF);
void mbWinTopDispSet(int arg0);
void mbWinPauseHook(BOOL dispF);
HUWINID mbWinIDGet(s16 winNo);
s16 mbWinTopIDGet(void);
void mbWinCenterGet(s16 winNo, HuVec2f *pos);
void mbWinCenterInsertGet(s16 arg0, int arg1);
void mbWinTopCenterInsertGet(int arg0);

#endif
