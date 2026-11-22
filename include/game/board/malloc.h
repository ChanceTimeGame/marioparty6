#ifndef _GAME_BOARD_MALLOC_H
#define _GAME_BOARD_MALLOC_H

#include "game/board/board.h"

void *mbMalloc(s32 size);
void *mbMallocNum(s32 size, u32 num);
void *mbMallocFlush(s32 size);
void *mbMallocFlushModel(s32 size);
void *mbMallocFlushModelNum(s32 size, u32 num);

#endif
