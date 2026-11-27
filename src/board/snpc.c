#include "game/board/snpc.h"

/* Partial: the functions the host's board start reaches, decompiled as they are needed. The rest
 * of the unit (wave B6) is still assembly. */

static int snpcMagic;
static void *snpcSaveWork;
static void *snpcWork;

void mbSNpcInit(void)
{
    snpcMagic = 0;
    snpcSaveWork = NULL;
    snpcWork = NULL;
}
