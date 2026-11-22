#include "game/board/branch.h"
#include "game/board/masu.h"
#include "game/board/object.h"
#include "game/board/player.h"
#include "game/board/window.h"
#include "game/board/exit.h"
#include "game/board/config.h"
#include "game/board/tutorial.h"
#include "game/board/telop.h"
#include "game/board/audio.h"
#include "game/board/scroll.h"
#include "game/board/math.h"
#include "game/gamework.h"
#include "game/flag.h"
#include "game/pad.h"

/* Shared headers (masu.h, object.h) mistype these; the MP6 functions take/return these types. */
#define MasuGet(id) (((MASU *(*)(s16))mbMasuGet)(id))
#define MasuFind_IdStepGet(a, b) (((int (*)(int, int))mbMasuFind_IdStepGet)((a), (b)))
#define MasuFind_TypeStepGet2(a, b, c, d) (((int (*)(int, int, int, int))mbMasuFind_TypeStepGet2)((a), (b), (c), (d)))
#define ObjCreate(a, b, c) (((s16 (*)(int, void *, int))mbObjCreate)((a), (b), (c)))

static inline BOOL PartyFGet(void)
{
    BOOL partyF = GwSystem.partyF;
    return partyF;
}

typedef struct BranchGuideWork_s {
    unsigned killF : 1;
    unsigned playerNo : 2;
    unsigned linkNum : 2;
    unsigned flag : 1;
    s8 choice;
    s16 angle;
    s16 unk4;
    s16 modelId[4];
} BRANCHGUIDEWORK;

static OMOBJ *guideOMObj[GW_PLAYER_MAX];

static MBBRANCHCOMHOOK branchComStarHook;
static u32 branchMAttr;
static u16 branchAttr;

static void BranchGuideOMExec(OMOBJ *obj);
static int BranchComChoiceGet(int playerNo, int linkNum, s16 *linkTbl, BOOL partyF);

void mbBranchInit(void)
{
    int i;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        guideOMObj[i] = NULL;
    }
    branchComStarHook = NULL;
    mbBranchAttrInit();
    mbBranchMAttrInit();
}

static void BranchGuideCreate(int playerNo, int masuId, s16 *linkTbl, int linkNum, s16 choice)
{
    BRANCHGUIDEWORK *work;
    OMOBJ *obj;
    HuVecF pos;
    HuVecF posLink;
    HuVecF dirLink;
    float angle;
    int i;
    if(guideOMObj[playerNo] || linkNum <= 1) {
        return;
    }
    guideOMObj[playerNo] = obj = omAddObjEx(mbObjMan, 256, 0, 0, -1, BranchGuideOMExec);
    omSetStatBit(obj, OM_STAT_MODELPAUSE);
    obj->scale.x = obj->scale.y = obj->scale.z = 3.0f;
    work = omObjGetWork(obj, BRANCHGUIDEWORK);
    work->linkNum = linkNum;
    work->killF = FALSE;
    work->flag = FALSE;
    work->playerNo = playerNo;
    work->angle = 0;
    work->choice = choice;
    if(!_CheckFlag(FLAG_BOARD_TUTORIAL) && PartyFGet()) {
        mbWinCreateHelp(0x260007);
    }
    mbMasuPosGet(masuId, &pos);
    pos.y += 350.0f;
    for(i=0; i<linkNum; i++) {
        work->modelId[i] = ObjCreate(mbBoardDataNumGet(0x50000), NULL, TRUE);
        mbObjLayerSet(work->modelId[i], 4);
        mbMasuPosGet(linkTbl[i], &posLink);
        VECSubtract(&posLink, &pos, &dirLink);
        angle = fmod(HuAtan(dirLink.x, dirLink.z), 360);
        mbObjRotSet(work->modelId[i], 0.0f, angle, 0.0f);
        mbObjLayerSet(work->modelId[i], 4);
        posLink.x = pos.x+(100*HuSin(angle));
        posLink.y = pos.y;
        posLink.z = pos.z+(100*HuCos(angle));
        mbObjPosSetV(work->modelId[i], &posLink);
        mbObjScaleSet(work->modelId[i], obj->scale.x, obj->scale.y, obj->scale.z);
    }
}

static void BranchGuideOMExec(OMOBJ *obj)
{
    BRANCHGUIDEWORK *work = omObjGetWork(obj, BRANCHGUIDEWORK);
    if(work->killF || mbExitCheck()) {
        int i;
        for(i=0; i<work->linkNum; i++) {
            mbObjKill(work->modelId[i]);
        }
        if(!_CheckFlag(FLAG_BOARD_TUTORIAL) && PartyFGet()) {
            mbWinTopKill();
        }
        guideOMObj[work->playerNo] = NULL;
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    work->angle += 8;
    if(work->angle > 180) {
        work->angle -= 180;
    }
    obj->scale.x = work->angle;
    obj->scale.x = obj->scale.y = obj->scale.z = HuSin((float)work->angle)+3.0;
    mbObjScaleSet(work->modelId[work->choice], obj->scale.x, obj->scale.y, obj->scale.z);
}

static inline int MasuBranchLinkGet(s16 masuId, s16 *linkTbl)
{
    int linkNum = 0;
    MASU *masuP = MasuGet(masuId);
    MASU *masuLinkP;
    int i;
    for(i=0; i<masuP->linkNum; i++) {
        masuLinkP = MasuGet(masuP->linkTbl[i]);
        if((masuLinkP->attr & mbBranchAttrGet()) == 0 && (masuLinkP->mAttr & mbBranchMAttrGet()) == 0) {
            linkTbl[linkNum++] = masuP->linkTbl[i];
        }
    }
    return linkNum;
}

static inline void BranchGuideKill(int playerNo)
{
    if(guideOMObj[playerNo]) {
        BRANCHGUIDEWORK *work = omObjGetWork(guideOMObj[playerNo], BRANCHGUIDEWORK);
        work->killF = TRUE;
        HuPrcSleep(10);
    }
}

static inline void BranchGuideChoiceSet(int playerNo, int choice)
{
    if(guideOMObj[playerNo]) {
        BRANCHGUIDEWORK *work = omObjGetWork(guideOMObj[playerNo], BRANCHGUIDEWORK);
        work->choice = choice;
    }
}

static BOOL ev_Branch(int playerNo, s16 *masuId, BOOL debugF)
{
    float anglePath[10];
    s16 linkTbl[10];
    HuVecF dir;
    HuVecF dir2D;
    HuVecF pos;
    HuVecF pos2D;
    HuVecF posLink;
    HuVecF posLink2D;
    int i;
    int linkNum;
    int choice;
    int choiceTime;
    int padNo;
    int choiceAuto;
    s16 masuPlayer;
    static s16 angleTbl[9] = {
        135, 90, 45,
        180, 0, 0,
        225, 270, 315
    };
    static const s8 chanceTbl[] = {
        30, 20, 10, 0
    };

    choiceTime = -1;
    masuPlayer = GwPlayer[playerNo].masuId;
    linkNum = MasuBranchLinkGet(masuPlayer, linkTbl);
    if(!debugF && linkNum <= 1) {
        *masuId = linkTbl[0];
        return FALSE;
    }
    choice = 0;
    if(PartyFGet()) {
        choice = BranchComChoiceGet(playerNo, linkNum, linkTbl, TRUE);
    }
    BranchGuideCreate(playerNo, masuPlayer, linkTbl, linkNum, choice);
    if(debugF) {
        linkNum += mbMasuLinkParentGet(masuPlayer, &linkTbl[linkNum]);
    }
    mbMasuPosGet(masuPlayer, &pos);
    mbPos3Dto2D(&pos, &pos2D);
    for(i=0; i<linkNum; i++) {
        mbMasuPosGet(linkTbl[i], &posLink);
        mbPos3Dto2D(&posLink, &posLink2D);
        VECSubtract(&posLink2D, &pos2D, &dir2D);
        anglePath[i] = fmod(HuAtan(-dir2D.y, dir2D.x), 360);
    }
    if(!debugF) {
        mbPlayerMotIdleSet(playerNo);
        HuPrcSleep(10);
    }
    mbConfigPadDisableSet(FALSE);
    padNo = GwPlayer[playerNo].padNo;
    while(1) {
        u16 btnDown;
        if(_CheckFlag(FLAG_BOARD_TUTORIAL)) {
            if(choiceTime < 0) {
                i = mbTutorialCall(13);
                if(i < 0) {
                    i = 0;
                }
                choiceAuto = 4;
                if(i & 8) {
                    choiceAuto -= 3;
                }
                if(i & 4) {
                    choiceAuto += 3;
                }
                if(i & 1) {
                    choiceAuto -= 1;
                }
                if(i & 2) {
                    choiceAuto += 1;
                }
                dir.x = 64.0f*mbCosDeg(angleTbl[choiceAuto]);
                dir.y = 64.0f*mbSinDeg(angleTbl[choiceAuto]);
                choiceTime = 30;
            } else {
                choiceTime--;
                if(choiceTime == 0) {
                    btnDown = PAD_BUTTON_A;
                }
            }
        } else if(GwPlayer[playerNo].comF && !debugF) {
            if(choiceTime < 0) {
                if(PartyFGet()) {
                    choiceAuto = BranchComChoiceGet(playerNo, linkNum, linkTbl, FALSE);
                } else if(mbRandMod(100) < chanceTbl[GwPlayer[playerNo].comDif]) {
                    choiceAuto = mbRandMod(linkNum);
                } else {
                    choiceAuto = choice;
                }
                dir.x = 64*HuCos(anglePath[choiceAuto]);
                dir.y = 64*HuSin(anglePath[choiceAuto]);
                btnDown = 0;
                choiceTime = 6;
            } else {
                choiceTime--;
                if(choiceTime == 0) {
                    btnDown = PAD_BUTTON_A;
                }
            }
        } else {
            dir.x = mbPadStkXGet(padNo);
            dir.y = mbPadStkYGet(padNo);
            btnDown = HuPadBtnDown[padNo];
            if(debugF) {
                btnDown = HuPadBtn[padNo];
            }
        }
        if(btnDown == PAD_BUTTON_A) {
            *masuId = linkTbl[choice];
            mbAudFXPlay(2);
            BranchGuideKill(playerNo);
            break;
        }
        if(btnDown == PAD_BUTTON_Y || btnDown == PAD_BUTTON_X) {
            mbAudFXPlay(1);
            mbMoveNumDispSet(playerNo, FALSE);
            BranchGuideKill(playerNo);
            mbPlayerMotIdleSet(playerNo);
            mbev_Scroll(playerNo, (btnDown == PAD_BUTTON_Y) ? TRUE : FALSE);
            mbMoveNumDispSet(playerNo, TRUE);
            BranchGuideCreate(playerNo, masuPlayer, linkTbl, linkNum, choice);
            continue;
        }
        if(debugF && HuPadBtnDown[padNo] == PAD_BUTTON_B) {
            BranchGuideKill(playerNo);
            mbConfigPadDisableSet(TRUE);
            return TRUE;
        }
        if(dir.x != 0.0f || dir.y != 0.0f) {
            float angle = HuAtan(dir.y, dir.x);
            int bestDir = -1;
            float bestDiff = 9999.0f;
            for(i=0; i<linkNum; i++) {
                float angleDiff = fmod(anglePath[i]-angle, 360);
                if(angleDiff < -180.0f) {
                    angleDiff += 360.0f;
                } else if(angleDiff > 180.0f) {
                    angleDiff -= 360.0f;
                }
                if(fabs(angleDiff) < bestDiff) {
                    bestDir = i;
                    bestDiff = fabs(angleDiff);
                }
            }
            if(bestDiff < 45.0f) {
                if(choice != bestDir) {
                    mbAudFXPlay(0);
                    choice = bestDir;
                    BranchGuideChoiceSet(playerNo, choice);
                }
                if(debugF) {
                    *masuId = linkTbl[choice];
                    BranchGuideKill(playerNo);
                    break;
                }
            }
        } else {
            mbPlayerMotIdleSet(playerNo);
        }
        HuPrcVSleep();
    }
    mbConfigPadDisableSet(TRUE);
    return FALSE;
}

BOOL mbev_Branch(int playerNo, s16 *masuId)
{
    return ev_Branch(playerNo, masuId, FALSE);
}

BOOL mbev_BranchDebug(int playerNo, s16 *masuId)
{
    return ev_Branch(playerNo, masuId, TRUE);
}

void mbBranchAttrSet(u16 attr)
{
    branchAttr |= attr;
}

void mbBranchMAttrSet(u32 mAttr)
{
    branchMAttr |= mAttr;
}

void mbBranchAttrReset(u16 attr)
{
    branchAttr &= ~attr;
}

void mbBranchMAttrReset(u32 mAttr)
{
    branchMAttr &= ~mAttr;
}

void mbBranchAttrInit(void)
{
    branchAttr = 0xA020;
}

void mbBranchMAttrInit(void)
{
    branchMAttr = 0;
}

u16 mbBranchAttrGet(void)
{
    return branchAttr;
}

u32 mbBranchMAttrGet(void)
{
    return branchMAttr;
}

BOOL mbBranchAttrCheck(int masuId)
{
    MASU *masuP = MasuGet(masuId);
    if(masuP->attr & mbBranchAttrGet()) {
        return FALSE;
    }
    if(masuP->mAttr & mbBranchMAttrGet()) {
        return FALSE;
    }
    return TRUE;
}

static int BranchComChoiceGet(int playerNo, int linkNum, s16 *linkTbl, BOOL partyF)
{
    int choice = -1;
    int len;
    int bestLen = 9999;
    int i;
    static const s8 chanceTbl[] = {
        30, 20, 10, 0
    };
    if(!partyF && mbPlayerWorkGet(playerNo)->unkE != 0) {
        for(i=0; i<linkNum; i++) {
            s16 target = mbPlayerWorkGet(playerNo)->unkE;
            len = MasuFind_IdStepGet(linkTbl[i], target);
            if(len < bestLen) {
                choice = i;
                bestLen = len;
            }
        }
        return choice;
    }
    if(branchComStarHook != NULL) {
        choice = branchComStarHook(playerNo, linkNum, linkTbl, partyF);
    } else {
        for(i=0; i<linkNum; i++) {
            len = MasuFind_TypeStepGet2(linkTbl[i], 7, 1, 1);
            if(len < bestLen) {
                choice = i;
                bestLen = len;
            }
        }
        if(choice < 0 || (bestLen > 20 && mbRandMod(100) < chanceTbl[GwPlayer[playerNo].comDif] && !partyF)) {
            i = 0;
            while(1) {
                if(MasuGet(linkTbl[i])->linkNum != 0 && mbRandMod(1000) < 500) {
                    break;
                }
                i++;
                if(i >= linkNum) {
                    i = 0;
                }
            }
            choice = i;
        }
    }
    return choice;
}

void mbBranchComStarHookSet(MBBRANCHCOMHOOK hook)
{
    branchComStarHook = hook;
}
