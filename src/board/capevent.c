#include "game/board/capevent.h"
#include "game/gamework.h"

/* Partial: the functions the host's board start reaches, decompiled as they are needed. The rest
 * of the unit (wave B5) is still assembly. */

void mbev_CapBankCoinInit(void)
{
    GwSystem.bankCoin = 0;
}
