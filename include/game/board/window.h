#ifndef _GAME_BOARD_WINDOW_H
#define _GAME_BOARD_WINDOW_H

#include "game/board/board.h"

#define MBWIN_MAX 16

#define MBWIN_MES_NONE -1
#define MBWIN_NONE -1
#define MBWIN_SPEAKER_NONE -1

typedef void (*MBWINCOMKEYHOOK)(void);

typedef struct MbWin_s {
    /* 0x00 */ HUPROCESS *proc;
    /* 0x04 */ int winId;
    /* 0x08 */ u32 mess;
    /* 0x0C */ u8 no;
    /* 0x0D */ s8 mesSpeed;
    /* 0x10 */ u32 attr;
    /* 0x14 */ int type;
    /* 0x18 */ s8 speakerNo;
    /* 0x1A */ s16 prio;
    /* 0x1C */ int playerNo;
    /* 0x20 */ HuVec2f pos;
    /* 0x28 */ HuVec2f size;
    /* 0x30 */ HuVec2f scale;
    /* 0x38 */ HuVec2f ofs;
    /* 0x40 */ BOOL pauseF;
    /* 0x44 */ BOOL choiceF;
    /* 0x48 */ int choiceNo;
    /* 0x4C */ BOOL choiceDisable[HUWIN_CHOICE_MAX];
    /* 0x8C */ u32 insertMes[HUWIN_INSERTMES_MAX];
    /* 0xCC */ MBWINCOMKEYHOOK comKeyHook;
    /* 0xD0 */ int frame;
} MBWIN; /* size 0xD4 */

void mbWinInit(void);
void mbWinClose(void);
int mbWinCreate(int type, u32 mess, int speakerNo);
int mbWinCreateChoice(int type, u32 mess, int speakerNo, int choiceNo);
int mbWinCreateHelp(u32 mess);
int mbWinCreateFrame(int type, u32 mess, int speakerNo, int frame);
int mbWinCreateTime(int type, u32 mess, int speakerNo);
int mbWinCreateBlank(void);
int mbWinCreateBlankFrame(int frame);
MBWIN *mbWinGet(int winNo);
void mbWinKill(s16 winNo);
void mbWinTopKill(void);
void mbWinKillAll(void);
void mbWinPosSet(s16 winNo, s16 posX, s16 posY);
void mbWinTopPosSet(s16 posX, s16 posY);
void mbWinPosGet(s16 winNo, HuVec2f *pos);
void mbWinTopPosGet(HuVec2f *pos);
void mbWinSizeSet(s16 winNo, s16 sizeX, s16 sizeY);
void mbWinTopSizeSet(s16 sizeX, s16 sizeY);
void mbWinMesMaxSizeGet(s16 winNo, HuVec2f *size);
void mbWinTopMesMaxSizeGet(HuVec2f *size);
void mbWinScaleSet(s16 winNo, float scaleX, float scaleY);
void mbWinTopScaleSet(float scaleX, float scaleY);
void mbWinScaleGet(s16 winNo, HuVec2f *scale);
void mbWinTopScaleGet(HuVec2f *scale);
int mbWinTopChoiceGet(void);
int mbWinChoiceGet(s16 winNo);
void mbWinPause(s16 winNo);
void mbWinTopPause(void);
void mbWinInsertMesSet(s16 winNo, u32 insertMes, int insertMesNo);
void mbWinTopInsertMesSet(u32 insertMes, int insertMesNo);
BOOL mbWinDoneCheck(s16 winNo);
BOOL mbWinTopDoneCheck(void);
void mbWinWait(s16 winNo);
void mbWinTopWait(void);
void mbWinAttrSet(s16 winNo, u32 attr);
void mbWinTopAttrSet(u32 attr);
void mbWinAttrReset(s16 winNo, u32 attr);
void mbWinTopAttrReset(u32 attr);
void mbWinChoiceDisable(s16 winNo, int choiceNo);
void mbWinTopChoiceDisable(int choiceNo);
void mbWinMesSpeedSet(s16 winNo, int speed);
void mbWinTopMesSpeedSet(int speed);
void mbWinMesColSet(s16 winNo, int mesCol);
void mbWinTopMesColSet(int mesCol);
s16 mbWinChoiceNowGet(s16 winNo);
s16 mbWinTopChoiceNowGet(void);
void mbWinPriSet(s16 winNo, s16 prio);
void mbWinTopPriSet(s16 prio);
s16 mbWinChoiceNowGet2(s16 winNo);
s16 mbWinTopChoiceNowGet2(void);
void mbWinPlayerDisable(s16 winNo, int playerNo);
void mbWinTopPlayerDisable(int playerNo);
void mbWinTopComKeyHookSet(MBWINCOMKEYHOOK comKeyHook);
void mbWinDispSet(s16 winNo, BOOL dispF);
void mbWinTopDispSet(BOOL dispF);
void mbWinPauseHook(BOOL dispF);
HUWINID mbWinIDGet(s16 winNo);
HUWINID mbWinTopIDGet(void);
void mbWinCenterGet(s16 winNo, HuVec2f *pos);
void mbWinCenterInsertGet(s16 winNo, u32 mess);
void mbWinTopCenterInsertGet(u32 mess);

#endif
