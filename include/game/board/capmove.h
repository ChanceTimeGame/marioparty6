#ifndef _GAME_BOARD_CAPMOVE_H
#define _GAME_BOARD_CAPMOVE_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbev_CapKinoko(void);
void mbev_CapKinokoKill(void);
void mbev_CapSKinoko(void);
void mbev_CapSKinokoKill(void);
void mbev_CapPKinoko(void);
void mbev_CapPKinokoKill(void);
void mbev_CapMKinoko(void);
void mbev_CapMKinokoKill(void);
void mbev_CapKiller(void);
void mbev_CapKillerKill(void);
void mbev_CapDokan(void);
void mbev_CapDokanKill(void);
void mbev_CapHanachan(void);
void mbev_CapHanachanKill(void);
void mbev_CapNKinoko(void);
void mbev_CapNKinokoKill(void);
void mbev_CapKillerMove(void);
void mbev_CapKillerMoveKill(void);

#endif
