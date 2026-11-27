#include "game/board/shopevent.h"

/* Partial: the functions the host's board start reaches, decompiled as they are needed. The rest
 * of the unit (wave B6) is still assembly. */

static BOOL ev_ShopEnableF;

void mbev_ShopEnableSet(BOOL enableF)
{
    ev_ShopEnableF = enableF;
}
