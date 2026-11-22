#ifndef _GAME_BOARD_CAPTHROW_H
#define _GAME_BOARD_CAPTHROW_H

#include "game/board/board.h"

/* Prototypes from the MP6 asm: argument/return classes from register use, names after MP5 where
   the function exists there; argN/fN are placeholder names. */
void mbev_CapTogezo(void);
void mbev_CapTogezoKill(void);
void mbev_CapKuribo(void);
void mbev_CapKuriboKill(void);
void mbev_CapPakkun(void);
void mbev_CapPakkunKill(void);
void mbev_CapJango(void);
void mbev_CapJangoKill(void);
void mbev_CapPatapata(void);
void mbev_CapPatapataKill(void);
void mbev_CapKokamekku(void);
void mbev_CapKokamekkuKill(void);
void mbev_CapKamekku(void);
void mbev_CapKamekkuKill(void);
void mbev_CapThrowman(void);
void mbev_CapThrowmanKill(void);

#endif
