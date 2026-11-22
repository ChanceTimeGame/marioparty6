#include "game/board/status.h"
#include "game/board/masu.h"
#include "game/board/pause.h"
#include "game/board/single.h"
#include "game/board/math.h"
#include "game/board/telop.h"

#include "game/gamework.h"
#include "game/sprite.h"
#include "game/process.h"
#include "game/data.h"
#include "game/memory.h"
#include "math.h"
#include "string.h"

/* player.h and capsule.h are not included: mbPlayerTeamCapsuleGet returns s8 (player.h: u8)
   and mbCapColorGet takes int (capsule.h: s16). */
int mbPlayerTeamFindPlayer(int grp, int index);
int mbPlayerCoinGet(int playerNo);
s16 mbPlayerTeamCoinGet(int grp);
int mbPlayerStarGet(int playerNo);
s16 mbPlayerGrpStarGet(int grp);
int mbPlayerRankGet(int playerNo);
s16 mbPlayerTeamRankGet(int grp);
s8 mbPlayerCapsuleGet(int playerNo, int index);
s8 mbPlayerTeamCapsuleGet(int grp, int index);
int mbCapColorGet(int capsuleNo);

#define STATUS_SPRNO_BACK 0
#define STATUS_SPRNO_BORDER 1
#define STATUS_SPRNO_RANK 2
#define STATUS_SPRNO_COIN_NUM 3
#define STATUS_SPRNO_COIN 6
#define STATUS_SPRNO_STAR_NUM 7
#define STATUS_SPRNO_STAR 10
#define STATUS_SPRNO_KAO 11
#define STATUS_SPRNO_KAOFRAME 13
#define STATUS_SPRNO_COM 15
#define STATUS_SPRNO_TEAMRANK 17
#define STATUS_SPRNO_CAPSULE 18
#define STATUS_SPR_MAX 23

#define STATUS_MOVE_LINEAR 0
#define STATUS_MOVE_SIN 1
#define STATUS_MOVE_REVCOS 2

#define STATUS_LAYOUT_TOP 0
#define STATUS_LAYOUT_BOTTOM 1

typedef struct statusWork_s {
    u8 offF : 1;
    u8 dispF : 1;
    u8 moveF : 1;
    u8 capsuleDispF : 1;
    u8 layoutNo : 2;
    u8 layoutBackup : 2;
    s8 capsuleFocus;
    s16 capsuleTime;
    s8 no;
    s8 playerNo;
    HUSPR_GROUPID gid;
    HuVec2f posBegin;
    HuVec2f posEnd;
    int moveType;
    int time;
    int timeMax;
} STATUSWORK; /* size 0x24 */

typedef struct statusMasuWork_s {
    u8 existF : 1;
    u8 dispF : 1;
    s16 numPrev;
    s16 num;
    HUSPR_GROUPID gid;
    u8 unk8[0x10];
    int time;
    int timeMax;
    u8 unk20[8];
} STATUSMASUWORK; /* size 0x28 */

typedef struct statusDisp_s {
    u8 dispF : 1;
    int no;
} STATUSDISP;

static STATUSWORK statusWork[GW_PLAYER_MAX];
static STATUSMASUWORK statusMasuWork;
static STATUSDISP statusDispBackup[GW_PLAYER_MAX];

static HUPROCESS *statusProc;
static u8 statusForceFlag;

static const int rainbowColorTbl[6] = { 3, 1, 3, 2, 3, 0 };

static HuVec2f statusPosOn[GW_PLAYER_MAX] = {
    { 114, 80 },
    { 462, 80 },
    { 114, 400 },
    { 462, 400 },
};

static HuVec2f statusPosOff[GW_PLAYER_MAX] = {
    { -98, 80 },
    { 674, 80 },
    { -98, 400 },
    { 674, 400 },
};

static HuVec2f statusPosOnTeam[GW_PLAYER_MAX / 2] = {
    { 140, 80 },
    { 436, 80 },
};

static HuVec2f statusPosOffTeam[GW_PLAYER_MAX / 2] = {
    { -124, 80 },
    { 700, 80 },
};

static GXColor statusColTbl[5] = {
    { 128, 128, 128, 0 },
    { 26, 132, 255, 0 },
    { 255, 26, 45, 0 },
    { 10, 180, 60, 0 },
    { 255, 64, 255, 0 },
};

static GXColor statusBorderColTbl[5] = {
    { 88, 88, 88, 0 },
    { 0, 0, 255, 0 },
    { 255, 0, 0, 0 },
    { 0, 255, 0, 0 },
    { 255, 0, 255, 0 },
};

static GXColor statusDefaultColTbl[4] = {
    { 192, 65, 76, 0 },
    { 244, 113, 123, 0 },
    { 95, 100, 16, 0 },
    { 105, 164, 234, 0 },
};

static HuVec2f statusSprPosTbl[STATUS_SPR_MAX] = {
    { 0, 0 },
    { 0, 0 },
    { -68, 0 },
    { 80, 14 },
    { 64, 14 },
    { 48, 14 },
    { 26, 14 },
    { 80, -12 },
    { 64, -12 },
    { 48, -12 },
    { 26, -12 },
    { -16, 0 },
};

static HuVec2f statusCapsuleOfsTbl[2] = {
    { 24, 34 },
    { 24, -34 },
};

static HuVec2f statusComOfsTbl[2] = {
    { -16, -32 },
    { -16, 32 },
};

static HuVec2f statusTeamSprPosTbl[STATUS_SPR_MAX] = {
    { 0, 0 },
    { 0, 0 },
    { -100, 0 },
    { 112, 14 },
    { 96, 14 },
    { 80, 14 },
    { 58, 14 },
    { 112, -12 },
    { 96, -12 },
    { 80, -12 },
    { 58, -12 },
    { -48, 0 },
    { 16, 0 },
};

static HuVec2f statusTeamCapsuleOfsTbl[2] = {
    { -24, 34 },
    { -24, 34 },
};

static HuVec2f statusTeamComOfsTbl[2] = {
    { -48, -32 },
    { -48, -32 },
};

static HuVec2f statusCircuitCapsuleOfsTbl[3] = {
    { 38, 12 },
    { 54, -12 },
    { 70, 12 },
};

static int statusKaoFileTbl[14] = {
    0x50036,
    0x50037,
    0x50038,
    0x50039,
    0x5003A,
    0x5003B,
    0x5003C,
    0x5003D,
    0x5003E,
    0x5003F,
    0x50040,
    0x50041,
    0x50042,
    0x50043,
};

static inline BOOL StatusPartyFGet(void)
{
    BOOL partyF = GwSystem.partyF;
    return partyF;
}

static inline BOOL StatusTeamFGet(void)
{
    return GwSystem.tagF;
}

static inline int StatusGrpGet(int playerNo)
{
    return GwPlayer[playerNo].team;
}

static void StatusMain(void);
static void StatusKill(void);
static BOOL StatusUpdate(void);
static BOOL StatusTeamUpdate(void);
static void StatusPosUpdate(STATUSWORK *status);
static void StatusPauseHook(BOOL dispF);
static void StatusSprCreate(int playerNo, STATUSWORK *status);
static void StatusTeamSprCreate(int grp, STATUSWORK *status);
static void StatusMasuSprCreate(STATUSMASUWORK *work);
static STATUSWORK *StatusPlayerGet(int playerNo);

void mbStatusInit(void)
{
    int i;
    GXColor *colorP;
    memset(&statusWork[0], 0, sizeof(statusWork));
    memset(&statusMasuWork, 0, sizeof(statusMasuWork));
    if(!StatusTeamFGet()) {
        for(i=0; i<GW_PLAYER_MAX; i++) {
            StatusSprCreate(i, &statusWork[i]);
            statusWork[i].offF = TRUE;
            statusWork[i].moveF = FALSE;
            statusWork[i].dispF = FALSE;
            statusWork[i].capsuleDispF = TRUE;
            statusWork[i].no = i;
            statusWork[i].capsuleFocus = -1;
            statusWork[i].capsuleTime = 0;
            statusWork[i].playerNo = i;
            if(i < GW_PLAYER_MAX/2) {
                statusWork[i].layoutNo = STATUS_LAYOUT_TOP;
            } else {
                statusWork[i].layoutNo = STATUS_LAYOUT_BOTTOM;
            }
            statusWork[i].layoutBackup = statusWork[i].layoutNo;
            mbStatusColorSet(i, GwPlayer[i].statusColor);
            HuSprGrpDrawNoSet(statusWork[i].gid, 32);
        }
    } else {
        for(i=0; i<GW_PLAYER_MAX/2; i++) {
            StatusTeamSprCreate(i, &statusWork[i]);
            statusWork[i].offF = TRUE;
            statusWork[i].moveF = FALSE;
            statusWork[i].dispF = FALSE;
            statusWork[i].capsuleDispF = TRUE;
            statusWork[i].capsuleFocus = -1;
            statusWork[i].capsuleTime = 0;
            statusWork[i].no = i;
            statusWork[i].playerNo = i;
            statusWork[i].layoutNo = STATUS_LAYOUT_TOP;
            statusWork[i].layoutBackup = statusWork[i].layoutNo;
            colorP = (i == 0) ? &statusDefaultColTbl[0] : &statusDefaultColTbl[2];
            colorP++;
            HuSprColorSet(statusWork[i].gid, STATUS_SPRNO_BACK, colorP->r, colorP->g, colorP->b);
            HuSprColorSet(statusWork[i].gid, STATUS_SPRNO_BORDER, colorP->r, colorP->g, colorP->b);
            HuSprColorSet(statusWork[i].gid, STATUS_SPRNO_KAOFRAME, colorP->r, colorP->g, colorP->b);
            HuSprColorSet(statusWork[i].gid, STATUS_SPRNO_KAOFRAME+1, colorP->r, colorP->g, colorP->b);
            HuSprGrpDrawNoSet(statusWork[i].gid, 32);
        }
    }
    if(!StatusPartyFGet()) {
        statusMasuWork.existF = TRUE;
        StatusMasuSprCreate(&statusMasuWork);
        HuSprGrpPosSet(statusMasuWork.gid, 472, 72);
        HuSprGrpDrawNoSet(statusMasuWork.gid, 32);
    }
    statusProc = HuPrcChildCreate(StatusMain, 8193, 16384, 0, mbMainProc);
    HuPrcDestructorSet2(statusProc, StatusKill);
    mbStatusDispForceSetAll(FALSE);
    mbStatusMasuDispSet(FALSE);
}

void mbStatusClose(void)
{
    STATUSWORK *status;
    int i;
    for(status=&statusWork[0], i=0; i<GW_PLAYER_MAX; i++, status++) {
        if(status->offF && status->gid != -1) {
            HuSprGrpKill(status->gid);
            status->gid = -1;
        }
    }
    if(statusMasuWork.existF) {
        HuSprGrpKill(statusMasuWork.gid);
        statusMasuWork.existF = FALSE;
    }
}

void mbStatusReset(void)
{
    mbStatusClose();
    HuPrcKill(statusProc);
    mbStatusInit();
}

static void StatusMain(void)
{
    STATUSWORK *status = &statusWork[0];
    mbPauseHookPush(StatusPauseHook);
    while(1) {
        if(!StatusTeamFGet()) {
            if(StatusUpdate()) {
                break;
            }
        } else {
            if(StatusTeamUpdate()) {
                break;
            }
        }
        HuPrcVSleep();
    }
    HuPrcEnd();
}

static void StatusKill(void)
{
    statusProc = NULL;
}

static BOOL StatusUpdate(void)
{
    int i, j;
    int digit;
    float scale;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        STATUSWORK *status = &statusWork[i];
        int capsuleNo;
        if(!StatusPartyFGet() && i > 0) {
            for(j=0; j<STATUS_SPR_MAX; j++) {
                HuSprDispOff(status->gid, j);
            }
            statusWork[i].moveF = FALSE;
            continue;
        }
        GwPlayer[i].rank = mbPlayerRankGet(i);
        HuSprBankSet(status->gid, STATUS_SPRNO_RANK, GwPlayer[i].rank);
        mbSprNumSet(status->gid, STATUS_SPRNO_COIN_NUM, mbPlayerCoinGet(i));
        if(StatusPartyFGet()) {
            mbSprNumSet(status->gid, STATUS_SPRNO_STAR_NUM, mbPlayerStarGet(i));
        } else {
            mbSprNumSet(status->gid, STATUS_SPRNO_STAR_NUM, mbSingleMgUnlockNumGet());
        }
        for(j=0; j<3; j++) {
            capsuleNo = mbPlayerCapsuleGet(i, j);
            if(capsuleNo != -1 && status->capsuleDispF) {
                HuSprBankSet(status->gid, STATUS_SPRNO_CAPSULE+j, mbCapColorGet(capsuleNo));
                HuSprPosSet(status->gid, STATUS_SPRNO_CAPSULE+j, statusCapsuleOfsTbl[status->layoutNo].x+(j*24), statusCapsuleOfsTbl[status->layoutNo].y);
                HuSprDispOn(status->gid, STATUS_SPRNO_CAPSULE+j);
                if(status->capsuleFocus == j) {
                    scale = 1.0+fabs(HuCos(8.0f*status->capsuleTime++));
                    HuSprScaleSet(status->gid, STATUS_SPRNO_CAPSULE+j, scale, scale);
                } else {
                    status->capsuleTime = 0;
                    HuSprScaleSet(status->gid, STATUS_SPRNO_CAPSULE+j, 1.0f, 1.0f);
                }
            } else {
                HuSprDispOff(status->gid, STATUS_SPRNO_CAPSULE+j);
            }
        }
        if(GwPlayer[i].comF) {
            HuSprDispOn(status->gid, STATUS_SPRNO_COM);
            HuSprPosSet(status->gid, STATUS_SPRNO_COM, statusComOfsTbl[status->layoutNo].x, statusComOfsTbl[status->layoutNo].y);
        } else {
            HuSprDispOff(status->gid, STATUS_SPRNO_COM);
        }
        StatusPosUpdate(status);
    }
    if(statusMasuWork.existF) {
        if(statusMasuWork.dispF) {
            HuSprGrpPosSet(statusMasuWork.gid, 472, 72);
            if(mbSingleStepGet() <= 1) {
                HuSprDispOff(statusMasuWork.gid, 1);
                HuSprDispOff(statusMasuWork.gid, 2);
                HuSprDispOn(statusMasuWork.gid, 3);
                HuSprDispOn(statusMasuWork.gid, 4);
            } else {
                HuSprDispOn(statusMasuWork.gid, 1);
                HuSprDispOn(statusMasuWork.gid, 2);
                HuSprDispOff(statusMasuWork.gid, 3);
                HuSprDispOff(statusMasuWork.gid, 4);
            }
            HuSprDispOff(statusMasuWork.gid, 5);
            i = mbSingleStepGet();
            if(i < 0) {
                i = 0;
            }
            if(i >= mbMasuNumGet()) {
                i = 0;
            }
            digit = statusMasuWork.num = i;
            j = digit/10;
            if(j > 0) {
                HuSprDispOn(statusMasuWork.gid, 5);
                HuSprBankSet(statusMasuWork.gid, 5, j);
            }
            digit -= j*10;
            if(i > 0 && i <= 5) {
                digit += 9;
            }
            HuSprBankSet(statusMasuWork.gid, 6, digit);
            if(statusMasuWork.num != statusMasuWork.numPrev) {
                statusMasuWork.numPrev = statusMasuWork.num;
                statusMasuWork.time = statusMasuWork.timeMax = 8;
            }
            if(statusMasuWork.time != 0) {
                statusMasuWork.time--;
                scale = (float)statusMasuWork.time/statusMasuWork.timeMax;
                scale = 1.0f+(0.5f*mbSinDeg(90.0f*scale));
                HuSprScaleSet(statusMasuWork.gid, 5, scale, scale);
                HuSprScaleSet(statusMasuWork.gid, 6, scale, scale);
            }
        } else {
            HuSprGrpPosSet(statusMasuWork.gid, 700, 72);
        }
    }
    return FALSE;
}

static BOOL StatusTeamUpdate(void)
{
    int i, j;
    for(i=0; i<GW_PLAYER_MAX/2; i++) {
        STATUSWORK *status = &statusWork[i];
        int grpNum;
        int capsuleNo;
        HuSprBankSet(status->gid, STATUS_SPRNO_RANK, (i*2)+mbPlayerTeamRankGet(i));
        grpNum = mbPlayerTeamCoinGet(i);
        mbSprNumSet(status->gid, STATUS_SPRNO_COIN_NUM, grpNum);
        grpNum = mbPlayerGrpStarGet(i);
        mbSprNumSet(status->gid, STATUS_SPRNO_STAR_NUM, grpNum);
        for(j=0; j<5; j++) {
            capsuleNo = mbPlayerTeamCapsuleGet(i, j);
            if(capsuleNo != -1 && status->capsuleDispF) {
                HuSprBankSet(status->gid, STATUS_SPRNO_CAPSULE+j, mbCapColorGet(capsuleNo));
                HuSprPosSet(status->gid, STATUS_SPRNO_CAPSULE+j, statusTeamCapsuleOfsTbl[status->layoutNo].x+(j*24), statusTeamCapsuleOfsTbl[status->layoutNo].y);
                HuSprDispOn(status->gid, STATUS_SPRNO_CAPSULE+j);
            } else {
                HuSprDispOff(status->gid, STATUS_SPRNO_CAPSULE+j);
            }
        }
        for(j=0; j<2; j++) {
            if(GwPlayer[mbPlayerTeamFindPlayer(i, j)].comF) {
                HuSprDispOn(status->gid, STATUS_SPRNO_COM+j);
                HuSprPosSet(status->gid, STATUS_SPRNO_COM+j, statusTeamComOfsTbl[status->layoutNo].x+(j*64), statusTeamComOfsTbl[status->layoutNo].y);
            } else {
                HuSprDispOff(status->gid, STATUS_SPRNO_COM+j);
            }
        }
        StatusPosUpdate(status);
    }
    return FALSE;
}

static void StatusPosUpdate(STATUSWORK *status)
{
    float weight;
    float time;
    HuVec2f pos;
    if(status->time >= status->timeMax) {
        status->moveF = FALSE;
        return;
    }
    time = (float)(++status->time)/status->timeMax;
    switch(status->moveType) {
        case STATUS_MOVE_LINEAR:
            weight = time;
            break;

        case STATUS_MOVE_SIN:
            weight = HuSin(90.0f*time);
            break;

        case STATUS_MOVE_REVCOS:
            weight = 1.0-HuCos(90.0f*time);
            break;
    }
    pos.x = status->posBegin.x+(weight*(status->posEnd.x-status->posBegin.x));
    pos.y = status->posBegin.y+(weight*(status->posEnd.y-status->posBegin.y));
    HuSprGrpPosSet(status->gid, pos.x, pos.y);
}

void mbStatusMoveTo(int playerNo, HuVecF *posBegin, HuVecF *posEnd)
{
    mbStatusMoveSet(playerNo, posBegin, posEnd, STATUS_MOVE_REVCOS, 16);
}

void mbStatusMoveFrom(int playerNo, HuVecF *posBegin, HuVecF *posEnd)
{
    mbStatusMoveSet(playerNo, posBegin, posEnd, STATUS_MOVE_SIN, 16);
}

static void StatusMoveSet(STATUSWORK *status, HuVecF *posBegin, HuVecF *posEnd, int type, int time)
{
    status->posBegin.x = posBegin->x;
    status->posBegin.y = posBegin->y;
    status->posEnd.x = posEnd->x;
    status->posEnd.y = posEnd->y;
    status->moveType = type;
    status->time = 0;
    status->timeMax = time;
    status->moveF = TRUE;
}

void mbStatusMoveSet(int playerNo, HuVecF *posBegin, HuVecF *posEnd, int type, int time)
{
    STATUSWORK *status = StatusPlayerGet(playerNo);
    if(posBegin == NULL) {
        HuVecF pos;
        mbStatusPosGet(playerNo, &pos);
        StatusMoveSet(status, &pos, posEnd, type, time);
    } else {
        StatusMoveSet(status, posBegin, posEnd, type, time);
    }
}

void mbStatusNoMoveSet(int statusNo, HuVecF *posBegin, HuVecF *posEnd, int type, int time)
{
    STATUSWORK *status = &statusWork[statusNo];
    if(posBegin == NULL) {
        HuVecF pos;
        mbStatusNoPosGet(statusNo, &pos);
        StatusMoveSet(status, &pos, posEnd, type, time);
    } else {
        StatusMoveSet(status, posBegin, posEnd, type, time);
    }
}

void mbStatusPosOnGet(int statusNo, HuVecF *pos)
{
    if(!StatusTeamFGet()) {
        pos->x = statusPosOn[statusNo].x;
        pos->y = statusPosOn[statusNo].y;
    } else {
        pos->x = statusPosOnTeam[statusNo].x;
        pos->y = statusPosOnTeam[statusNo].y;
    }
}

void mbStatusPosOffGet(int statusNo, HuVecF *pos)
{
    if(!StatusTeamFGet()) {
        pos->x = statusPosOff[statusNo].x;
        pos->y = statusPosOff[statusNo].y;
    } else {
        pos->x = statusPosOffTeam[statusNo].x;
        pos->y = statusPosOffTeam[statusNo].y;
    }
}

BOOL mbStatusMoveCheck(int playerNo)
{
    STATUSWORK *status = StatusPlayerGet(playerNo);
    return status->moveF == FALSE;
}

BOOL mbStatusOffCheckAll(void)
{
    int i;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        if(statusWork[i].offF && statusWork[i].moveF) {
            return FALSE;
        }
    }
    return TRUE;
}

static void StatusDispSet(int playerNo, int statusNo, BOOL dispF)
{
    STATUSWORK *status = StatusPlayerGet(playerNo);
    HuVec2f posBegin;
    HuVec2f posEnd;
    if(!StatusPartyFGet() && playerNo > 0) {
        dispF = FALSE;
    }
    if(!dispF) {
        if(!status->dispF) {
            return;
        }
        posBegin = status->posEnd;
        posEnd = status->posBegin;
        mbStatusMoveTo(playerNo, (HuVecF *)&posBegin, (HuVecF *)&posEnd);
        status->dispF = FALSE;
        status->moveF = TRUE;
    } else {
        if(status->dispF) {
            return;
        }
        mbStatusPosOffGet(statusNo, (HuVecF *)&posBegin);
        mbStatusPosOnGet(statusNo, (HuVecF *)&posEnd);
        mbStatusMoveFrom(playerNo, (HuVecF *)&posBegin, (HuVecF *)&posEnd);
        status->dispF = TRUE;
        status->moveF = TRUE;
    }
    if(dispF) {
        status->no = statusNo;
        if(status->no < 2) {
            mbStatusLayoutSet(playerNo, STATUS_LAYOUT_TOP);
        } else {
            mbStatusLayoutSet(playerNo, STATUS_LAYOUT_BOTTOM);
        }
    }
}

void mbStatusDispSet(int playerNo, BOOL dispF)
{
    if(!StatusPartyFGet() && playerNo > 0) {
        dispF = FALSE;
    }
    if(!StatusTeamFGet()) {
        StatusDispSet(playerNo, playerNo, dispF);
    } else {
        StatusDispSet(playerNo, StatusGrpGet(playerNo), dispF);
    }
}

void mbStatusDispFocusSet(int playerNo, BOOL dispF)
{
    StatusDispSet(playerNo, 0, dispF);
}

void mbStatusDispBackup(void)
{
    int i;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        statusDispBackup[i].dispF = mbStatusDispGet(i);
        statusDispBackup[i].no = mbStatusNoGet(i);
    }
}

void mbStatusDispRestoreForce(void)
{
    int i;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        STATUSWORK *status = StatusPlayerGet(i);
        status->dispF = statusDispBackup[i].dispF;
        status->no = statusDispBackup[i].no;
        if(status->dispF) {
            mbStatusPosOffGet(status->no, (HuVecF *)&status->posBegin);
            mbStatusPosOnGet(status->no, (HuVecF *)&status->posEnd);
        } else {
            mbStatusPosOnGet(status->no, (HuVecF *)&status->posBegin);
            mbStatusPosOffGet(status->no, (HuVecF *)&status->posEnd);
        }
        HuSprGrpPosSet(status->gid, status->posEnd.x, status->posEnd.y);
    }
}

void mbStatusDispRestore(void)
{
    int i;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        StatusDispSet(i, statusDispBackup[i].no, statusDispBackup[i].dispF);
    }
}

BOOL mbStatusDispGet(int playerNo)
{
    STATUSWORK *status = StatusPlayerGet(playerNo);
    return status->dispF;
}

int mbStatusNoGet(int playerNo)
{
    STATUSWORK *status = StatusPlayerGet(playerNo);
    return status->no;
}

void mbStatusDispSetAll(BOOL dispF)
{
    int i;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        mbStatusDispSet(i, dispF);
    }
}

void mbStatusDispForceSet(int playerNo, BOOL dispF)
{
    STATUSWORK *status = StatusPlayerGet(playerNo);
    if(!StatusPartyFGet() && playerNo > 0) {
        dispF = FALSE;
    }
    status->dispF = dispF;
    if(dispF) {
        mbStatusPosOffGet(status->no, (HuVecF *)&status->posBegin);
        mbStatusPosOnGet(status->no, (HuVecF *)&status->posEnd);
    } else {
        mbStatusPosOnGet(status->no, (HuVecF *)&status->posBegin);
        mbStatusPosOffGet(status->no, (HuVecF *)&status->posEnd);
    }
    HuSprGrpPosSet(status->gid, status->posEnd.x, status->posEnd.y);
    status->time = status->timeMax = 0;
    if(!StatusTeamFGet()) {
        status->no = playerNo;
    } else {
        status->no = StatusGrpGet(playerNo);
    }
    if(status->no < 2) {
        mbStatusLayoutSet(playerNo, STATUS_LAYOUT_TOP);
    } else {
        mbStatusLayoutSet(playerNo, STATUS_LAYOUT_BOTTOM);
    }
    status->moveF = FALSE;
}

void mbStatusDispForceSetAll(BOOL dispF)
{
    int i;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        mbStatusDispForceSet(i, dispF);
    }
}

void mbStatusMasuDispSet(BOOL dispF)
{
    if(statusMasuWork.existF) {
        if(dispF) {
            if(!mbPauseProcCheck()) {
                statusMasuWork.dispF = TRUE;
            }
            HuSprGrpPosSet(statusMasuWork.gid, 472, 72);
        } else {
            HuSprGrpPosSet(statusMasuWork.gid, 700, 72);
        }
    }
}

void mbStatusCapsuleDispSet(int statusNo, BOOL dispF)
{
    STATUSWORK *status = &statusWork[statusNo];
    if(dispF) {
        status->capsuleDispF = TRUE;
    } else {
        status->capsuleDispF = FALSE;
    }
}

void mbStatusLayoutSet(int statusNo, int layoutNo)
{
    STATUSWORK *status = &statusWork[statusNo];
    status->layoutNo = layoutNo;
}

void mbStatusCapsuleFocusSet(int statusNo, int index)
{
    STATUSWORK *status = &statusWork[statusNo];
    status->capsuleFocus = index;
}

static STATUSWORK *StatusPlayerGet(int playerNo)
{
    if(!StatusTeamFGet()) {
        return &statusWork[playerNo];
    } else {
        return &statusWork[StatusGrpGet(playerNo)];
    }
}

void mbStatusPosGet(int playerNo, HuVecF *pos)
{
    STATUSWORK *status = StatusPlayerGet(playerNo);
    HUSPR_GROUP *gp = &HuSprGrpData[status->gid];
    pos->x = gp->pos.x;
    pos->y = gp->pos.y;
    pos->z = 0;
}

void mbStatusNoPosGet(int statusNo, HuVecF *pos)
{
    STATUSWORK *status = &statusWork[statusNo];
    HUSPR_GROUP *gp = &HuSprGrpData[status->gid];
    pos->x = gp->pos.x;
    pos->y = gp->pos.y;
    pos->z = 0;
}

void mbStatusKaoNoSet(int statusNo, int kaoNo)
{
    STATUSWORK *status = &statusWork[statusNo];
    HuSprBankSet(status->gid, STATUS_SPRNO_KAO, kaoNo);
}

#define STATUS_SPR_INITNUM 11

static void StatusSprCreate(int playerNo, STATUSWORK *status)
{
    static int fileTbl[STATUS_SPR_INITNUM] = {
        0x50028,
        0x50029,
        0x50027,
        0x50023,
        0x50023,
        0x50023,
        0x50026,
        0x50023,
        0x50023,
        0x50023,
        0x50025,
    };
    static int fileTblSingle[STATUS_SPR_INITNUM] = {
        0x50028,
        0x50029,
        0x50027,
        0x50023,
        0x50023,
        0x50023,
        0x50026,
        0x50023,
        0x50023,
        0x50023,
        0x50030,
    };
    static s16 prioTbl[STATUS_SPR_INITNUM] = {
        1520,
        1510,
        1500,
        1500,
        1500,
        1500,
        1500,
        1500,
        1500,
        1500,
        1500,
    };
    int charNo = GwPlayerConf[playerNo].charNo;
    int i;
    int dataNum;
    s16 prio;
    HUSPRID sprId;
    float posX, posY;
    status->gid = HuSprGrpCreate(STATUS_SPR_MAX);
    for(i=0; i<STATUS_SPR_INITNUM; i++) {
        if(StatusPartyFGet()) {
            dataNum = fileTbl[i];
        } else {
            dataNum = fileTblSingle[i];
        }
        prio = prioTbl[i];
        mbSprCreate(mbBoardDataNumGet(dataNum), prio, NULL, &sprId);
        HuSprGrpMemberSet(status->gid, i, sprId);
        HuSprAttrSet(status->gid, i, HUSPR_ATTR_NOANIM);
        HuSprAttrSet(status->gid, i, HUSPR_ATTR_LINEAR);
        HuSprPosSet(status->gid, i, statusSprPosTbl[i].x, statusSprPosTbl[i].y);
    }
    mbSprCreate(mbBoardDataNumGet(statusKaoFileTbl[charNo]), 1500, NULL, &sprId);
    HuSprGrpMemberSet(status->gid, STATUS_SPRNO_KAO, sprId);
    HuSprAttrSet(status->gid, STATUS_SPRNO_KAO, HUSPR_ATTR_LINEAR);
    posX = statusSprPosTbl[STATUS_SPRNO_KAO].x;
    posY = statusSprPosTbl[STATUS_SPRNO_KAO].y;
    HuSprPosSet(status->gid, STATUS_SPRNO_KAO, posX, posY);
    mbSprCreate(mbBoardDataNumGet(0x50035), 1490, NULL, &sprId);
    HuSprGrpMemberSet(status->gid, STATUS_SPRNO_KAOFRAME, sprId);
    HuSprAttrSet(status->gid, STATUS_SPRNO_KAOFRAME, HUSPR_ATTR_LINEAR);
    posX = statusSprPosTbl[STATUS_SPRNO_KAO].x;
    posY = statusSprPosTbl[STATUS_SPRNO_KAO].y;
    HuSprPosSet(status->gid, STATUS_SPRNO_KAOFRAME, posX, posY);
    mbSprCreate(mbBoardDataNumGet(0x50024), 1490, NULL, &sprId);
    HuSprGrpMemberSet(status->gid, STATUS_SPRNO_COM, sprId);
    HuSprAttrSet(status->gid, STATUS_SPRNO_COM, HUSPR_ATTR_LINEAR);
    for(i=0; i<3; i++) {
        mbSprCreate(mbBoardDataNumGet(0x5002E), 1500, NULL, &sprId);
        HuSprGrpMemberSet(status->gid, STATUS_SPRNO_CAPSULE+i, sprId);
        HuSprAttrSet(status->gid, STATUS_SPRNO_CAPSULE+i, HUSPR_ATTR_NOANIM);
        HuSprAttrSet(status->gid, STATUS_SPRNO_CAPSULE+i, HUSPR_ATTR_LINEAR);
    }
    HuSprTPLvlSet(status->gid, STATUS_SPRNO_BACK, 0.7f);
    if(!StatusPartyFGet()) {
        i = STATUS_SPRNO_RANK;
        HuSprDispOff(status->gid, i);
        for(i=STATUS_SPRNO_STAR_NUM; i<=STATUS_SPRNO_STAR; i++) {
            HuSprPosSet(status->gid, i, statusSprPosTbl[i].x-24, statusSprPosTbl[i].y);
        }
        for(i=STATUS_SPRNO_COIN_NUM; i<=STATUS_SPRNO_COIN; i++) {
            HuSprPosSet(status->gid, i, statusSprPosTbl[i].x-24, statusSprPosTbl[i].y);
        }
        i = STATUS_SPRNO_KAO;
        HuSprPosSet(status->gid, i, statusSprPosTbl[i].x-24, statusSprPosTbl[i].y);
        HuSprPosSet(status->gid, STATUS_SPRNO_KAOFRAME, statusSprPosTbl[i].x-24, statusSprPosTbl[i].y);
    }
}

static void StatusTeamSprCreate(int grp, STATUSWORK *status)
{
    static int fileTbl[STATUS_SPR_INITNUM] = {
        0x5002A,
        0x5002B,
        0x5002D,
        0x50023,
        0x50023,
        0x50023,
        0x50026,
        0x50023,
        0x50023,
        0x50023,
        0x50025,
    };
    static s16 prioTbl[STATUS_SPR_INITNUM] = {
        1520,
        1510,
        1500,
        1500,
        1500,
        1500,
        1500,
        1500,
        1500,
        1500,
        1500,
    };
    int i;
    int kaoNo;
    int dataNum;
    s16 prio;
    HUSPRID sprId;
    float posX, posY;
    status->gid = HuSprGrpCreate(STATUS_SPR_MAX);
    for(i=0; i<STATUS_SPR_INITNUM; i++) {
        dataNum = fileTbl[i];
        prio = prioTbl[i];
        mbSprCreate(mbBoardDataNumGet(dataNum), prio, NULL, &sprId);
        HuSprGrpMemberSet(status->gid, i, sprId);
        HuSprAttrSet(status->gid, i, HUSPR_ATTR_NOANIM);
        HuSprAttrSet(status->gid, i, HUSPR_ATTR_LINEAR);
        HuSprPosSet(status->gid, i, statusTeamSprPosTbl[i].x, statusTeamSprPosTbl[i].y);
    }
    mbSprCreate(mbBoardDataNumGet(0x5002C), 1510, NULL, &sprId);
    HuSprGrpMemberSet(status->gid, STATUS_SPRNO_TEAMRANK, sprId);
    HuSprAttrSet(status->gid, STATUS_SPRNO_TEAMRANK, HUSPR_ATTR_NOANIM);
    HuSprAttrSet(status->gid, STATUS_SPRNO_TEAMRANK, HUSPR_ATTR_LINEAR);
    HuSprPosSet(status->gid, STATUS_SPRNO_TEAMRANK, statusTeamSprPosTbl[STATUS_SPRNO_RANK].x, statusTeamSprPosTbl[STATUS_SPRNO_RANK].y);
    HuSprBankSet(status->gid, STATUS_SPRNO_TEAMRANK, grp);
    kaoNo = 0;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        if(grp == StatusGrpGet(i)) {
            mbSprCreate(mbBoardDataNumGet(statusKaoFileTbl[GwPlayerConf[i].charNo]), 1500, NULL, &sprId);
            HuSprGrpMemberSet(status->gid, STATUS_SPRNO_KAO+kaoNo, sprId);
            HuSprAttrSet(status->gid, STATUS_SPRNO_KAO+kaoNo, HUSPR_ATTR_LINEAR);
            posX = statusTeamSprPosTbl[STATUS_SPRNO_KAO+kaoNo].x;
            posY = statusTeamSprPosTbl[STATUS_SPRNO_KAO+kaoNo].y;
            HuSprPosSet(status->gid, STATUS_SPRNO_KAO+kaoNo, posX, posY);
            mbSprCreate(mbBoardDataNumGet(0x50035), 1490, NULL, &sprId);
            HuSprGrpMemberSet(status->gid, STATUS_SPRNO_KAOFRAME+kaoNo, sprId);
            HuSprAttrSet(status->gid, STATUS_SPRNO_KAOFRAME+kaoNo, HUSPR_ATTR_LINEAR);
            HuSprPosSet(status->gid, STATUS_SPRNO_KAOFRAME+kaoNo, posX, posY);
            kaoNo++;
        }
    }
    for(i=0; i<2; i++) {
        mbSprCreate(mbBoardDataNumGet(0x50024), 1490, NULL, &sprId);
        HuSprGrpMemberSet(status->gid, STATUS_SPRNO_COM+i, sprId);
        HuSprAttrSet(status->gid, STATUS_SPRNO_COM+i, HUSPR_ATTR_LINEAR);
    }
    for(i=0; i<5; i++) {
        mbSprCreate(mbBoardDataNumGet(0x5002E), 1500, NULL, &sprId);
        HuSprGrpMemberSet(status->gid, STATUS_SPRNO_CAPSULE+i, sprId);
        HuSprAttrSet(status->gid, STATUS_SPRNO_CAPSULE+i, HUSPR_ATTR_NOANIM);
        HuSprAttrSet(status->gid, STATUS_SPRNO_CAPSULE+i, HUSPR_ATTR_LINEAR);
    }
    HuSprTPLvlSet(status->gid, STATUS_SPRNO_BACK, 0.7f);
}

#undef STATUS_SPR_INITNUM

static void StatusMasuSprCreate(STATUSMASUWORK *work)
{
    static int fileTbl[7] = {
        0x50031,
        0x50032,
        0x50032,
        0x50034,
        0x50034,
        0x50033,
        0x50033,
    };
    static HuVec2f ofsTbl[7] = {
        { 0, 0 },
        { -48, 0 },
        { 48, 0 },
        { -48, 0 },
        { 48, 0 },
        { -12, 0 },
        { 12, 0 },
    };
    static s16 sprOfsTbl[6][7] = {
        { 0, -48, 48, -48, 48, -12, 12 },
        { 0, -28, -28, -28, -28, -64, -40 },
        { 0, -28, -28, -28, -28, -64, -40 },
        { 0, -28, -28, -28, -28, -64, -40 },
        { 0, -28, -28, -28, -28, -64, -40 },
        { 0, -28, -28, -28, -28, -64, -40 },
    };
    int languageNo;
    int i;
    int dataNum;
    s16 prio;
    HUSPRID sprId;
    work->gid = HuSprGrpCreate(7);
    languageNo = mbLanguageGet();
    for(i=0; i<7; i++) {
        dataNum = fileTbl[i];
        prio = 1520;
        mbSprCreate(mbBoardDataNumGet(dataNum), prio, NULL, &sprId);
        HuSprGrpMemberSet(work->gid, i, sprId);
        HuSprAttrSet(work->gid, i, HUSPR_ATTR_NOANIM);
        HuSprAttrSet(work->gid, i, HUSPR_ATTR_LINEAR);
        HuSprPosSet(work->gid, i, sprOfsTbl[languageNo][i], 0.0f);
    }
    HuSprBankSet(work->gid, 2, 1);
    HuSprBankSet(work->gid, 4, 1);
    HuSprTPLvlSet(work->gid, 0, 0.7f);
}

void mbSprCreate(int dataNum, int prio, ANIMDATA **animP, HUSPRID *sprIdP)
{
    HUSPRID sprId;
    void *data;
    ANIMDATA *anim;
    data = HuDataSelHeapReadNum(dataNum, HU_MEMNUM_OVL, HEAP_MODEL);
    anim = HuSprAnimRead(data);
    sprId = HuSprCreate(anim, prio, 0);
    if(animP) {
        *animP = anim;
    }
    if(sprIdP) {
        *sprIdP = sprId;
    }
}

void mbSprNumSet(HUSPR_GROUPID gid, s16 memberNo, int num)
{
    int digit100 = num/100;
    int digit10 = (num-(digit100*100))/10;
    int digit1 = (num-((digit100*100)+(digit10*10)))%10;
    if(digit100 == 0) {
        HuSprBankSet(gid, memberNo+2, 10);
    } else {
        HuSprBankSet(gid, memberNo+2, digit100);
    }
    if(digit10 == 0 && digit100 == 0) {
        HuSprBankSet(gid, memberNo+1, digit1);
        HuSprDispOff(gid, memberNo);
    } else {
        HuSprBankSet(gid, memberNo+1, digit10);
        HuSprBankSet(gid, memberNo, digit1);
        HuSprDispOn(gid, memberNo);
    }
}

static void StatusPauseHook(BOOL dispF)
{
    int i;
    STATUSWORK *status;
    if(!dispF) {
        statusForceFlag = 0;
        for(i=0; i<GW_PLAYER_MAX; i++) {
            if(mbStatusDispGet(i)) {
                status = StatusPlayerGet(i);
                HuSprGrpPosSet(status->gid, status->posBegin.x, status->posBegin.y);
                statusForceFlag |= (1 << i);
            }
        }
        if(statusMasuWork.existF) {
            HuSprGrpPosSet(statusMasuWork.gid, 700, 72);
        }
    } else {
        for(i=0; i<GW_PLAYER_MAX; i++) {
            if(statusForceFlag & (1 << i)) {
                status = StatusPlayerGet(i);
                HuSprGrpPosSet(status->gid, status->posEnd.x, status->posEnd.y);
            }
        }
        if(statusMasuWork.existF) {
            if(!mbPauseProcCheck()) {
                statusMasuWork.dispF = TRUE;
            }
            HuSprGrpPosSet(statusMasuWork.gid, 472, 72);
        }
    }
}

void mbStatusRainbowSet(int statusNo, float time, int colorEnd)
{
    STATUSWORK *status = &statusWork[statusNo];
    int colorNo;
    int color;
    int colorNext;
    float weight;
    float weightInv;
    GXColor borderCol;
    GXColor col;
    colorNo = 5.0f*time;
    if(colorNo < 5) {
        color = rainbowColorTbl[colorNo];
        if(colorNo < 4) {
            colorNext = rainbowColorTbl[colorNo+1];
        } else {
            colorNext = colorEnd;
        }
        weight = fmod(5.0f*time, 1.0);
        weightInv = 1.0f-weight;
        col.r = (weightInv*statusColTbl[color].r)+(weight*statusColTbl[colorNext].r);
        col.g = (weightInv*statusColTbl[color].g)+(weight*statusColTbl[colorNext].g);
        col.b = (weightInv*statusColTbl[color].b)+(weight*statusColTbl[colorNext].b);
        borderCol.r = (weightInv*statusBorderColTbl[color].r)+(weight*statusBorderColTbl[colorNext].r);
        borderCol.g = (weightInv*statusBorderColTbl[color].g)+(weight*statusBorderColTbl[colorNext].g);
        borderCol.b = (weightInv*statusBorderColTbl[color].b)+(weight*statusBorderColTbl[colorNext].b);
    } else {
        if(!StatusTeamFGet()) {
            col = statusColTbl[colorEnd];
            borderCol = statusBorderColTbl[colorEnd];
        } else {
            if(colorEnd == 2) {
                colorEnd = 0;
            } else {
                colorEnd = 2;
            }
            col = statusDefaultColTbl[colorEnd];
            borderCol = statusDefaultColTbl[colorEnd+1];
        }
    }
    HuSprColorSet(status->gid, STATUS_SPRNO_BACK, col.r, col.g, col.b);
    HuSprColorSet(status->gid, STATUS_SPRNO_BORDER, borderCol.r, borderCol.g, borderCol.b);
    HuSprColorSet(status->gid, STATUS_SPRNO_KAOFRAME, borderCol.r, borderCol.g, borderCol.b);
    if(StatusTeamFGet()) {
        HuSprColorSet(status->gid, STATUS_SPRNO_KAOFRAME+1, borderCol.r, borderCol.g, borderCol.b);
    }
}

void mbStatusColorSet(int playerNo, int color)
{
    STATUSWORK *status = StatusPlayerGet(playerNo);
    if(!StatusTeamFGet()) {
        HuSprColorSet(status->gid, STATUS_SPRNO_BACK, statusColTbl[color].r, statusColTbl[color].g, statusColTbl[color].b);
        HuSprColorSet(status->gid, STATUS_SPRNO_BORDER, statusBorderColTbl[color].r, statusBorderColTbl[color].g, statusBorderColTbl[color].b);
        HuSprColorSet(status->gid, STATUS_SPRNO_KAOFRAME, statusBorderColTbl[color].r, statusBorderColTbl[color].g, statusBorderColTbl[color].b);
    }
    GwPlayer[playerNo].statusColor = color;
}

int mbStatusColorGet(int playerNo)
{
    return GwPlayer[playerNo].statusColor;
}

void mbStatusColorAllSet(int color)
{
    int i;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        mbStatusColorSet(i, color);
    }
}
