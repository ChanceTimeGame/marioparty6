#ifndef _GAME_BOARD_SHOPEVENT_H
#define _GAME_BOARD_SHOPEVENT_H

#include "game/board/board.h"

/* Called for each shop model as it is created, with the model's object id and the shop's index
   (unprototyped: w01's hook takes only the object id). */
typedef void (*MBEV_SHOPHOOK)();

void mbev_ShopEnableSet(BOOL enableF);
void mbev_ShopInit(int dataNum);
void mbev_ShopExInit(int dataNum, MBEV_SHOPHOOK hook);
void mbev_ShopCreate(int dataNum, int motDataNum);
void mbev_ShopExObjHookSet(MBEV_SHOPHOOK hook);
void mbev_ShopBackCreate(int dataNum, int motDataNum, int type, BOOL linkF);
void mbev_ShopBackMotCreate(int dataNum, int motDataNum, int type, BOOL linkF, char *hookName);
int mbev_Shop(int playerNo, int masuId);

#endif
