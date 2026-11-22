#ifndef _GAME_BOARD_CAPTRAP_H
#define _GAME_BOARD_CAPTRAP_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbev_CapBoble(void);
void mbev_CapBobleKill(void);
void mbev_CapBobleTrap(void *arg0);
void mbev_CapBobleMove(int arg0);
void mbev_CapBiriQ(void);
void mbev_CapBiriQKill(void);
void mbev_CapBiriQTrap(void *arg0);
void mbev_CapBiriQMetalShock(void *arg0);
void mbev_CapTumujikun(void);
void mbev_CapTumujikunKill(void);
void mbev_CapTumujikunTrap(void *arg0);
void mbev_CapDossun(void);
void mbev_CapDossunKill(void);
void mbev_CapDossunTrap(void *arg0);
void mbev_CapBomhei(void);
void mbev_CapBomheiKill(void);
void mbev_CapBomheiTrap(void *arg0);
void mbev_CapBomheiMove(int arg0);

#endif
