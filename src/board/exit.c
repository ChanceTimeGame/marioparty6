#include "game/board/exit.h"
#include "game/board/pause.h"
#include "game/armem.h"
#include "game/wipe.h"
#include "game/sprite.h"
#include "game/hu3d.h"

static BOOL exitFlag;
static BOOL exitReq;
static HUPROCESS *exitWatchProc;

static void ExitWatch(void);
static void ExitWatchDestroy(void);

void mbExitWatchCreate(void)
{
    exitWatchProc = HuPrcCreate(ExitWatch, 0x2013, 0x2000, 0);
    HuPrcSetStat(exitWatchProc, HU_PRC_STAT_PAUSE_ON | HU_PRC_STAT_UPAUSE_ON);
    HuPrcDestructorSet2(exitWatchProc, ExitWatchDestroy);
    exitFlag = FALSE;
    exitReq = FALSE;
}

static void ExitWatch(void)
{
    OMOBJWORK *objWork;
    int i;
    while (!exitReq && !omSysExitReq) {
        int playerNo = mbPauseStartCheck();
        if (playerNo >= 0) {
            mbPauseCreate(playerNo);
        }
        HuPrcVSleep();
    }
    exitFlag = TRUE;
    WipeWait();
    while (HuARDMACheck()) {
        HuPrcVSleep();
    }
    HuPrcResetStat(mbObjMan, HU_PRC_STAT_UPAUSE);
    objWork = mbObjMan->property;
    for (i = 0; i < objWork->objMax; i++) {
        if ((objWork->objData[i].stat & (OM_STAT_DELETED | OM_STAT_NOPAUSE)) == 0) {
            omResetStatBit(&objWork->objData[i], OM_STAT_PAUSED);
        }
    }
    mbPauseEnableReset();
    HuPrcVSleep();
    omSysPauseCtrl(FALSE);
    HuPrcAllPause(FALSE);
    Hu3DPauseSet(FALSE);
    HuSprPauseSet(FALSE);
    HuPrcKill(mbObjMan);
    HuPrcEnd();
}

static void ExitWatchDestroy(void)
{
    exitWatchProc = NULL;
}

void mbExitReq(void)
{
    exitReq = TRUE;
}

BOOL mbExitCheck(void)
{
    return exitFlag;
}

HUPROCESS *mbExitWatchProcGet(void)
{
    return exitWatchProc;
}
