#include "game/board/comchoice.h"
#include "game/board/window.h"
#include "game/gamework.h"

static int listDownNum;
static int listRightNum;

static void ComChoiceKeySet(int playerNo, BOOL moveF, BOOL horiF);
static void ComChoiceListKeySet(int playerNo, int rightNum, int downNum);

static void ComChoiceLeft(void)
{
    ComChoiceKeySet(GwSystem.turnPlayerNo, FALSE, TRUE);
}

void mbComChoiceLeftSet(void)
{
    mbWinTopComKeyHookSet(ComChoiceLeft);
}

static void ComChoiceRight(void)
{
    ComChoiceKeySet(GwSystem.turnPlayerNo, TRUE, TRUE);
}

void mbComChoiceRightSet(void)
{
    mbWinTopComKeyHookSet(ComChoiceRight);
}

static void ComChoiceUp(void)
{
    ComChoiceKeySet(GwSystem.turnPlayerNo, FALSE, FALSE);
}

void mbComChoiceUpSet(void)
{
    mbWinTopComKeyHookSet(ComChoiceUp);
}

static void ComChoiceDown(void)
{
    ComChoiceKeySet(GwSystem.turnPlayerNo, TRUE, FALSE);
}

void mbComChoiceDownSet(void)
{
    mbWinTopComKeyHookSet(ComChoiceDown);
}

static void ComChoiceKeySet(int playerNo, BOOL moveF, BOOL horiF)
{
    s32 key[GW_PLAYER_MAX];
    int padNo;
    s16 delay;
    s32 keyDir;
    key[0] = key[1] = key[2] = key[3] = 0;
    padNo = GwPlayer[playerNo].padNo;
    delay = GWComKeyDelayGet();
    if (horiF == FALSE) {
        keyDir = PAD_BUTTON_DOWN;
    } else {
        keyDir = PAD_BUTTON_RIGHT;
    }
    if (moveF == TRUE) {
        key[padNo] = keyDir;
        HuWinComKeyWait(key[0], key[1], key[2], key[3], delay);
    }
    key[padNo] = PAD_BUTTON_A;
    HuWinComKeyWait(key[0], key[1], key[2], key[3], delay);
}

static void ComChoiceList(void)
{
    ComChoiceListKeySet(GwSystem.turnPlayerNo, listRightNum, listDownNum);
}

static void ComChoiceListKeySet(int playerNo, int rightNum, int downNum)
{
    s32 key[GW_PLAYER_MAX];
    int padNo;
    s16 delay;
    int i;
    key[0] = key[1] = key[2] = key[3] = 0;
    padNo = GwPlayer[playerNo].padNo;
    delay = GWComKeyDelayGet();
    for (i = 0; i < downNum; i++) {
        key[padNo] = PAD_BUTTON_DOWN;
        HuWinComKeyWait(key[0], key[1], key[2], key[3], delay);
    }
    for (i = 0; i < rightNum; i++) {
        key[padNo] = PAD_BUTTON_RIGHT;
        HuWinComKeyWait(key[0], key[1], key[2], key[3], delay);
    }
    key[padNo] = PAD_BUTTON_A;
    HuWinComKeyWait(key[0], key[1], key[2], key[3], delay);
}

void mbComChoiceListDownSet(int downNum)
{
    listRightNum = 0;
    listDownNum = downNum;
    mbWinTopComKeyHookSet(ComChoiceList);
}

void mbComChoiceListRightSet(int rightNum)
{
    listRightNum = rightNum;
    listDownNum = 0;
    mbWinTopComKeyHookSet(ComChoiceList);
}

void mbComChoiceListSet(int rightNum, int downNum)
{
    listRightNum = rightNum;
    listDownNum = downNum;
    mbWinTopComKeyHookSet(ComChoiceList);
}
