#ifndef _GAME_BOARD_CAPSPECIAL_H
#define _GAME_BOARD_CAPSPECIAL_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbev_CapTeresa(void);
void mbev_CapTeresaKill(void);
void mbev_CapTeresaStealSet(int mesId, int coinNum, void *beginHook, void *hook);
void mbev_CapTeresaFadeCreate(int objId);
void mbev_CapTeresaFadeKill(int objId);
void mbev_CapTeresaFadeSet(float alpha);
void mbev_CapMiracle(void);
void mbev_CapMiracleKill(void);
void mbev_CapKettou(void);
void mbev_CapKettouKill(void);
void mbev_CapDonkey(void);
void mbev_CapDonkeyKill(void);
void mbev_CapKoopa(void);
void mbev_CapKoopaKill(void);

#endif
