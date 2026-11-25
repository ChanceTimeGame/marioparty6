/* @@HEAD@@ */
/* Placeholder HEAD written by part 2 (replaced by part 1's on splice). */
#define mbMasuFind_TypeStepGet mbMasuFind_TypeStepGet_s16
#define mbMasuCapsuleGet mbMasuCapsuleGet_s16
#include "game/board/masu.h"
#undef mbMasuFind_TypeStepGet
#undef mbMasuCapsuleGet
int mbMasuFind_TypeStepGet(s16 id, s16 type);
int mbMasuCapsuleGet(s16 id);

#include "game/board/capsule.h"
#include "game/board/audio.h"
#include "game/board/branch.h"
#include "game/board/camera.h"
#define mbev_CapMasuMoveCheck mbev_CapMasuMoveCheck_s16
#include "game/board/capevent.h"
#undef mbev_CapMasuMoveCheck
int mbev_CapMasuMoveCheck(int masuId);
#include "game/board/capselect.h"
#include "game/board/math.h"
#include "game/board/object.h"
#include "game/board/player.h"
#include "game/board/telop.h"
#include "game/board/tutorial.h"
#include "game/board/window.h"
#include "game/board/wipe.h"

#include "game/data.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/memory.h"
#include "game/pad.h"
#include "game/printfunc.h"

#include "string.h"
#include "math.h"

typedef struct CapsuleData_s {
    u32 file;
    u32 mdlFile;
    u32 descMes;
    u32 useMes;
    u32 unk10;
    u32 unk14;
    int cost;
    u8 masuPat;
    u8 unk1D;
    u16 useMode;
    char *debugName;
    u8 listExcludeF;
} CAPSULE_DATA;

typedef struct CapsuleList_s {
    s8 capsuleNo;
    s8 cost[3];
    s8 unk4[8];
    s8 unkC[4];
} CAPSULE_LIST;

typedef struct CapsuleChanceDif_s {
    s16 chance;
    s8 flag;
} CAPSULE_CHANCE_DIF;

typedef struct CapsuleChance_s {
    s16 capsuleNo;
    CAPSULE_CHANCE_DIF dif[11];
} CAPSULE_CHANCE;

typedef struct CapSelMasuWork_s {
    int playerNo;
    int unk4;
    int capsuleNo;
    int capObjId;
    int mdlId;
    int winId;
    int winId2;
    int unk1C;
} CAPSELMASU_WORK;

static int capsuleMasuSelectComF[4] = { 0 };
static u8 capsulePad1[0x9C] = { 1 };
static CAPSULE_DATA capsuleData[60] = { 1 };
static CAPSULE_CHANCE capsuleChanceTbl[23] = { 1 };
static GXColor capsuleMasuSelectColorTbl[8] = { 1 };
static u8 capsulePad2[0x11D8 - 0xE4E] = { 1 };
static HuVecF capsuleMasuSelectRotTbl[2] = { 1 };

static CAPSULE_LIST capsuleList[33];
static s16 capsuleNum[33][2];

static int capsuleMasuSelectResult = -1;
static int capsuleObjId = -1;

static int capsuleComSearchF;
static int capsuleSelectComBack;
static int capsuleMasuSelectEndF;
static int capsuleUseRemoveOnF;

/* @@PART1@@ */
/* @@PART2@@ */
/* Part 2: statics defined in later parts, and items part 1's HEAD must provide. */
static void CapGuideCreate(void);
static void CapGuideRotYSet(int masuId, float rotY);
static void CapGuideGrowSet(void);
static void CapGuideKill(void);
static void CapEffMasuOkCreate(void);
static void CapEffMasuOkAddAll(s16 masuId, s16 *list);
static void CapEffMasuOkDispSet(int dispF);
static void CapEffMasuOkPosSet(HuVecF *pos, int masuId);
static void CapEffMasuOkNext(void);
static void CapEffMasuOkKill(void);
static void CapEffRemoveCreate(void);
static void CapEffRemoveAddAll(HuVecF pos);
static int CapEffRemoveCheck(void);
static void CapEffRemoveKill(void);
static void CapEffHiliteCreate(void);
static void CapEffHiliteAdd(HuVecF pos, HuVecF rot, HuVecF scale, int arg3, int arg4, int arg5, int arg6, GXColor color);
static void CapEffHiliteKill(void);
static void CapComChoiceSet(int choice);
static float CapCameraXZAngleGet(float angle);
static float CapAngleSumWrap(float a, float b);

static void CapSelectMasuKill(CAPSELMASU_WORK *work);
static int CapSelectMasuCheck(int masuId);
static int CapSelectMasuDispCheck(int masuId);
static void CapSelectMasuListGet(s16 *list, s16 masuId, s16 frontNum, s16 backNum);
static void CapSelectMasuAddFront(s16 *list, s16 masuId, s16 num);
static void CapSelectMasuAddBack(s16 *list, s16 masuId, s16 num);
static void CapSelectMasuLinkCheck(s16 *list, s16 masuId);
static int CapSelectMasuComListGet(s16 *out, s16 *list, s16 masuId, s16 target, s16 num);
static int CapSelectMasuComListGetRev(s16 *out, s16 *list, s16 masuId, s16 target, s16 num);
static int CapSelectMasuWinCreate(int type);
static int CapUseDeleteWin(CAPSELMASU_WORK *work);
static void CapUseDeleteKill(CAPSELMASU_WORK *work);

static inline void CapObjAttrLoopSet(MBOBJID objId)
{
    mbObjAttrSet(objId, 0x40000001);
}

static inline double CapAbsF(float x)
{
    return fabs(x);
}

static void CapSelectMasuPlayer(CAPSELMASU_WORK *work)
{
    int i;
    int masuId;
    s16 *list;
    int time;
    int frontLinkNum;
    s16 angle;
    int linkNum;
    int padNo;
    int winType;
    int okF;
    int frontNum;
    int autoF;
    float t;
    float scale;
    float diff;
    float minDiff;
    float zoom;
    float angleF;
    float stkAngle;
    float angleFTbl[10];
    float angleCamTbl[10];
    s16 angleTbl[10];
    s16 linkTbl[10];
    s16 linkTmp[10];
    HuVecF pos;
    HuVecF newPos;
    HuVecF delta;
    HuVecF linkPos;
    HuVecF camOfs;
    HuVecF rot;
    HuVecF stick;
    HuVecF dirVec;
    HuVecF masuPos;
    HuVec2f winPos;
    int prevMasu;
    int maxTime;
    int hiliteTime;
    int prevWinType;
    int firstF;
    GXColor color;
    int moveF;
    int nextMasu;
    int objId;
    int mdlId;
    s16 dir;
    void *buf;

    moveF = -1;
    memset(angleTbl, 0, sizeof(angleTbl));
    memset(linkTbl, 0, sizeof(linkTbl));
    padNo = GwPlayer[work->playerNo].padNo;
    masuId = prevMasu = GwPlayer[work->playerNo].masuId;
    frontLinkNum = 0;
    frontNum = 0;
    dir = 1;
    hiliteTime = 0;
    CapEffHiliteCreate();
    buf = HuMemDirectMallocNum(HEAP_HEAP, 0x200, HU_MEMNUM_OVL);
    list = buf;
    memset(list, 0, 0x200);
    capsuleMasuSelectEndF = TRUE;
    CapSelectMasuListGet(list, masuId, 5, 5);
    list[masuId] = -0x8000;
    mbCapSelectResultGet(work->playerNo, &objId, &mdlId);
    mbCapSelectResultReset(work->playerNo);
    if(objId != -1) {
        work->capObjId = capsuleObjId = objId;
    } else {
        work->capObjId = capsuleObjId = mbCapObjCreate(work->capsuleNo, 1);
        mbObjDispSet(work->capObjId, FALSE);
        mbPlayerPosGet(work->playerNo, &pos);
        mbObjPosSet(work->capObjId, pos.x, 250.0f + pos.y, pos.z);
        mbObjScaleSet(work->capObjId, 1.2f, 1.2f, 1.2f);
    }
    mbObjLayerSet(work->capObjId, 4);
    CapObjAttrLoopSet(work->capObjId);
    zoom = mbCameraZoomGet();
    mbCameraOffsetGet(&camOfs);
    work->mdlId = mbObjCreate(mbCapFileGet(work->capsuleNo), NULL, TRUE);
    mbObjDispSet(work->mdlId, FALSE);
    mbPlayerPosGet(work->playerNo, &pos);
    mbObjPosSetV(work->mdlId, &pos);
    mbCameraMoveObj(work->mdlId, NULL, NULL, 3200.0f, -1.0f, 15);
    firstF = TRUE;
    work->winId = mbWinCreateHelp(0x370024);
    mbWinTopPosGet(&winPos);
    mbWinTopPosSet(winPos.x, 284);
    mbWinAttrSet(work->winId, 0x800);
    work->winId2 = -1;
    prevWinType = winType = -1;
    CapGuideCreate();
    CapEffMasuOkCreate();
    CapEffMasuOkAddAll(masuId, list);
    CapEffMasuOkDispSet(TRUE);
    while(1) {
        mbMasuPosGet(masuId, &masuPos);
        if(list[masuId] & 1) {
            okF = TRUE;
            CapEffMasuOkPosSet(&masuPos, masuId);
        } else {
            okF = FALSE;
        }
        linkNum = mbMasuLinkTblGet(masuId, linkTmp);
        for(i = 0, frontLinkNum = 0; i < linkNum; i++) {
            if(list[linkTmp[i]] != 0) {
                linkTbl[frontLinkNum] = linkTmp[i];
                frontLinkNum++;
            }
        }
        linkNum = frontLinkNum;
        linkNum += mbMasuLinkParentGet(masuId, &linkTbl[linkNum]);
        for(i = 0, frontNum = 0; i < linkNum; i++) {
            if(i >= frontLinkNum) {
                if(list[linkTbl[i]] == 0) {
                    linkTbl[i] = -1;
                } else {
                    frontNum++;
                }
            }
        }
        if(!okF && frontLinkNum == 1 && frontNum == 1 && masuId != GwPlayer[work->playerNo].masuId) {
            autoF = TRUE;
        } else {
            autoF = FALSE;
        }
        if(!autoF) {
            if(masuId == GwPlayer[work->playerNo].masuId) {
                winType = 0;
            } else {
                winType = 0;
            }
            if(winType != prevWinType) {
                if(work->winId2 != -1) {
                    mbWinKill(work->winId2);
                }
                work->winId2 = -1;
                work->winId2 = CapSelectMasuWinCreate(winType);
                prevWinType = winType;
            }
        }
        for(i = 0; i < linkNum; i++) {
            if(linkTbl[i] != -1) {
                mbMasuPosGet(linkTbl[i], &linkPos);
                PSVECSubtract(&linkPos, &masuPos, &dirVec);
                angleF = 90.0 - HuAtan(dirVec.z, dirVec.x);
                OSf32tos16(&angleF, &angle);
                if(angle < 0) {
                    angle += 360;
                }
                if(angle > 360) {
                    angle -= 360;
                }
                angleFTbl[i] = angle;
                angleCamTbl[i] = CapCameraXZAngleGet(angle);
                angle = ((angle + 22) / 45) * 45;
                angleTbl[i] = angle;
            }
        }
        if(!autoF) {
            for(i = 0; i < linkNum; i++) {
                if(linkTbl[i] != -1 && list[linkTbl[i]] != 0) {
                    CapGuideRotYSet(masuId, angleFTbl[i]);
                }
            }
        }
        if(okF) {
            mbAudFXPlay(0x403);
        }
        do {
            stick.x = mbPadStkXGet(padNo);
            stick.y = 0.0f;
            stick.z = mbPadStkYGet(padNo);
            if(firstF) {
                mbObjDispSet(work->capObjId, TRUE);
                time = 0;
                do {
                    if(objId == -1) {
                        time++;
                        t = time / 18.0f;
                        if(t > 1.0f) {
                            t = 1.0f;
                        }
                        mbPlayerPosGet(work->playerNo, &pos);
                        pos.y += 100.0 + 150.0 * HuSin(90.0f * t);
                        mbObjPosSetV(work->capObjId, &pos);
                        scale = 1.2f * HuSin(90.0f * t);
                        mbObjScaleSet(work->capObjId, scale, scale, scale);
                    } else {
                        t = 1.0f;
                    }
                    HuPrcVSleep();
                } while(!mbCameraMoveCheck() || t < 1.0f);
                firstF = FALSE;
            }
            if(GwPlayer[work->playerNo].comF) {
                capsuleMasuSelectResult = -1;
                goto end;
            }
            if(okF) {
                if(--hiliteTime <= 0) {
                    mbMasuPosGet(masuId, &linkPos);
                    mbMasuRotGet(masuId, &rot);
                    linkPos.y += 3.0f;
                    rot.x += capsuleMasuSelectRotTbl[0].x;
                    rot.y += capsuleMasuSelectRotTbl[0].y;
                    rot.z += capsuleMasuSelectRotTbl[0].z;
                    color = capsuleMasuSelectColorTbl[mbCapColorGet(work->capsuleNo)];
                    CapEffHiliteAdd(linkPos, rot, capsuleMasuSelectRotTbl[1], 1, 18, 1, 2, color);
                    hiliteTime = 19;
                }
            }
            if(autoF) {
                if(dir > 0) {
                    for(i = 0; i < frontLinkNum; i++) {
                        if(linkTbl[i] != -1) {
                            masuId = linkTbl[i];
                            break;
                        }
                    }
                    moveF = 0;
                } else {
                    for(i = frontLinkNum; i < linkNum; i++) {
                        if(linkTbl[i] != -1) {
                            masuId = linkTbl[i];
                            break;
                        }
                    }
                }
            } else if(HuPadBtnDown[padNo] & PAD_BUTTON_A) {
                if(okF) {
                    mbAudFXPlay(0x404);
                    capsuleMasuSelectResult = masuId;
                    goto end;
                }
            } else if(HuPadBtnDown[padNo] & PAD_BUTTON_B) {
                capsuleMasuSelectResult = -1;
                goto end;
            } else if(fabs(stick.x) >= 8.0 || CapAbsF(stick.z) >= 8.0) {
                stkAngle = HuAtan(mbPadStkXGet(padNo), -mbPadStkYGet(padNo));
                diff = 180.0f;
                minDiff = 180.0f;
                nextMasu = -1;
                for(i = 0; i < linkNum; i++) {
                    if(linkTbl[i] != -1 && list[linkTbl[i]] != 0) {
                        diff = CapAngleSumWrap(angleCamTbl[i], stkAngle);
                        if(fabs(diff) < 45.0 && fabs(diff) < minDiff) {
                            nextMasu = linkTbl[i];
                        }
                    }
                }
                if(nextMasu != -1) {
                    masuId = nextMasu;
                    moveF = 0;
                }
            }
            HuPrcVSleep();
        } while(masuId == prevMasu);
        CapGuideGrowSet();
        CapEffMasuOkNext();
        pos = masuPos;
        mbMasuPosGet(masuId, &newPos);
        PSVECSubtract(&newPos, &pos, &delta);
        t = PSVECMag(&delta);
        maxTime = t / 30.000002f;
        for(time = 1; time < maxTime; time++) {
            t = (float)time / (float)maxTime;
            PSVECSubtract(&newPos, &pos, &delta);
            PSVECScale(&delta, &delta, t);
            PSVECAdd(&pos, &delta, &delta);
            mbObjPosSet(work->mdlId, delta.x, delta.y, delta.z);
            HuPrcVSleep();
        }
        for(time = 0; time < frontLinkNum; time++) {
            if(masuId == linkTbl[time]) {
                break;
            }
        }
        if(time < frontLinkNum) {
            dir = 1;
        } else {
            dir = -1;
        }
        prevMasu = masuId;
    }
end:
    HuMemDirectFree(list);
    if(work->winId != -1) {
        mbWinKill(work->winId);
    }
    work->winId = -1;
    if(work->winId2 != -1) {
        mbWinKill(work->winId2);
    }
    work->winId2 = -1;
    if(capsuleMasuSelectResult != -1) {
        mbWipeDissolveFadeOut();
    } else {
        if(objId != -1) {
            mbCapSelectResultSet(work->playerNo, objId, mdlId);
            work->capObjId = -1;
        }
        if(work->capObjId != -1) {
            mbCapObjKill(work->capObjId);
        }
        work->capObjId = -1;
        capsuleObjId = -1;
    }
    mbCameraMovePlayer(work->playerNo, NULL, &camOfs, zoom, -1.0f, -1);
    mbCameraMoveWait();
    CapSelectMasuKill(work);
}

static inline GW_PLAYER *CapPlayerGet(int playerNo)
{
    return &GwPlayer[playerNo];
}

static void CapSelectMasuCom(CAPSELMASU_WORK *work)
{
    s16 num;
    s16 i;
    int masuId;
    s16 *list;
    s16 target;
    s16 *pathList;
    s16 *candList;
    int unk23;
    int unk22;
    int prevWinType;
    int unk20;
    int unk19;
    int winType;
    int firstF;
    float unkF31;
    float unkF30;
    s16 angleTbl[10];
    s16 linkTbl[10];
    HuVecF pos;
    HuVecF camOfs;
    int prevMasu;
    int unk44;
    int hiliteTime;
    float zoom;
    int moveF;
    GW_PLAYER *player;
    int masuNum;
    int padNo;
    int objId;
    int mdlId;
    void *listBuf;
    void *candBuf;
    void *pathBuf;
    s16 dir;

    if(0) {
        unkF31 = unkF30;
        unkF30 = unkF31;
        unk19 = unk20;
        unk20 = unk23;
        unk23 = unk19;
    }
    moveF = -1;
    memset(angleTbl, 0, sizeof(angleTbl));
    memset(linkTbl, 0, sizeof(linkTbl));
    player = CapPlayerGet(work->playerNo);
    masuNum = mbMasuRawNumGet();
    padNo = GwPlayer[work->playerNo].padNo;
    prevMasu = masuId = player->masuId;
    unk44 = 0;
    hiliteTime = 0;
    dir = 1;
    listBuf = HuMemDirectMallocNum(HEAP_HEAP, 0x200, HU_MEMNUM_OVL);
    list = listBuf;
    memset(list, 0, 0x200);
    candBuf = HuMemDirectMallocNum(HEAP_HEAP, 0x200, HU_MEMNUM_OVL);
    candList = candBuf;
    memset(candList, 0, 0x200);
    pathBuf = HuMemDirectMallocNum(HEAP_HEAP, 0x200, HU_MEMNUM_OVL);
    pathList = pathBuf;
    memset(pathList, 0, 0x200);
    if(capsuleSelectComBack) {
        capsuleMasuSelectEndF = TRUE;
        CapSelectMasuAddBack(list, masuId, 5);
        if(masuId != mbMasuFind_AttrIdGet(-1, 0x8000)) {
            CapSelectMasuLinkCheck(list, mbMasuTypeFindLink(mbMasuFind_AttrIdGet(-1, 0x8000), 0));
        }
    } else {
        capsuleMasuSelectEndF = TRUE;
        CapSelectMasuListGet(list, masuId, 5, 5);
    }
    list[masuId] = -0x8000;
    for(i = 1, num = 0; i < 256; i++) {
        if(i != masuId && (list[i] & 1) && mbMasuCapsuleGet(i) == -1) {
            candList[num++] = i;
        }
    }
    if(num <= 0) {
        for(i = 1, num = 0; i < 256; i++) {
            if(i != masuId && (list[i] & 1)) {
                candList[num++] = i;
            }
        }
    }
    if(num <= 0) {
        target = -1;
    } else if(num <= 1) {
        target = candList[0];
    } else {
        target = candList[mbRandMod(num)];
    }
    mbCapSelectResultGet(work->playerNo, &objId, &mdlId);
    mbCapSelectResultReset(work->playerNo);
    if(objId != -1) {
        work->capObjId = capsuleObjId = objId;
    } else {
        work->capObjId = capsuleObjId = mbCapObjCreate(work->capsuleNo, 1);
        mbObjDispSet(work->capObjId, TRUE);
        mbPlayerPosGet(work->playerNo, &pos);
        mbObjPosSet(work->capObjId, pos.x, 250.0f + pos.y, pos.z);
        mbObjScaleSet(work->capObjId, 1.2f, 1.2f, 1.2f);
    }
    mbObjLayerSet(work->capObjId, 4);
    CapObjAttrLoopSet(work->capObjId);
    zoom = mbCameraZoomGet();
    mbCameraOffsetGet(&camOfs);
    work->mdlId = mbObjCreate(mbCapFileGet(work->capsuleNo), NULL, TRUE);
    mbObjDispSet(work->mdlId, FALSE);
    mbPlayerPosGet(work->playerNo, &pos);
    mbObjPosSetV(work->mdlId, &pos);
    firstF = TRUE;
    work->winId = -1;
    work->winId2 = -1;
    winType = -1;
    prevWinType = winType;
    CapGuideCreate();
    CapEffMasuOkCreate();
    capsuleComSearchF = FALSE;
    CapSelectMasuComListGet(pathList, list, masuId, target, 0);
    if(!capsuleComSearchF) {
        capsuleComSearchF = FALSE;
        CapSelectMasuComListGetRev(pathList, list, target, masuId, 0);
    }
    for(i = 0; i < 256; i++) {
        if(target == pathList[i]) {
            break;
        }
    }
    if(i < 256 || !capsuleComSearchF) {
        pathList[i + 1] = target;
        pathList[i + 2] = target;
        i += 2;
    } else {
        capsuleMasuSelectComF[work->playerNo] = TRUE;
        capsuleMasuSelectResult = masuId = -1;
        goto end;
    }
    if(_CheckFlag(0x1000E)) {
        target = mbTutorialCall(17);
    }
    unk22 = -1;
    mbAudFXPlay(0x404);
    capsuleMasuSelectResult = target;
end:
    if(work->winId != -1) {
        mbWinKill(work->winId);
    }
    work->winId = -1;
    if(work->winId2 != -1) {
        mbWinKill(work->winId2);
    }
    work->winId2 = -1;
    HuMemDirectFree(list);
    HuMemDirectFree(pathList);
    HuMemDirectFree(candList);
    if(capsuleMasuSelectResult == -1) {
        if(objId != -1) {
            mbCapSelectResultSet(work->playerNo, objId, mdlId);
            work->capObjId = -1;
        }
        if(work->capObjId != -1) {
            mbCapObjKill(work->capObjId);
        }
        work->capObjId = -1;
        capsuleObjId = -1;
    }
    CapSelectMasuKill(work);
}

static void CapSelectMasuKill(CAPSELMASU_WORK *work)
{
    CapGuideKill();
    CapEffMasuOkKill();
    if(work->mdlId != -1) {
        mbObjKill(work->mdlId);
    }
    work->mdlId = -1;
    if(work->winId != -1) {
        mbWinKill(work->winId);
    }
    work->winId = -1;
    if(work->winId2 != -1) {
        mbWinKill(work->winId2);
    }
    work->winId2 = -1;
    CapEffHiliteKill();
}

static int CapSelectMasuCheck(int masuId)
{
    int type = mbMasuTypeGet(masuId);
    u32 mattr = mbMasuMAttrGet(masuId);
    int i;
    if((type == 1 || type == 2) && (mbCapMasuDispTypeGet(masuId) == 0 || mbCapMasuDispTypeGet(masuId) == 1)) {
        for(i = 0; i < 4; i++) {
            if(masuId == GwPlayer[i].masuId) {
                return FALSE;
            }
        }
        return TRUE;
    } else {
        return FALSE;
    }
}

static int CapSelectMasuDispCheck(int masuId)
{
    return mbMasuDispCheck(masuId);
}

static void CapSelectMasuListGet(s16 *list, s16 masuId, s16 frontNum, s16 backNum)
{
    capsuleMasuSelectEndF = TRUE;
    CapSelectMasuAddFront(list, masuId, frontNum);
    capsuleMasuSelectEndF = TRUE;
    CapSelectMasuAddBack(list, masuId, backNum);
    if(masuId != mbMasuFind_AttrIdGet(-1, 0x8000)) {
        CapSelectMasuLinkCheck(list, mbMasuTypeFindLink(mbMasuFind_AttrIdGet(-1, 0x8000), 0));
    }
}

static void CapSelectMasuAddFront(s16 *list, s16 masuId, s16 num)
{
    s16 linkTbl[10];
    s16 linkNum;
    int i;

    linkNum = mbMasuLinkTblGet(masuId, linkTbl);
    if(CapSelectMasuCheck(masuId)) {
        list[masuId] = 1;
    } else {
        list[masuId] = -0x8000;
    }
    if(CapSelectMasuDispCheck(masuId) && !capsuleMasuSelectEndF) {
        num--;
    }
    capsuleMasuSelectEndF = FALSE;
    if(num > 0) {
        for(i = 0; i < linkNum; i++) {
            if(mbev_CapMasuMoveCheck(linkTbl[i])) {
                continue;
            }
            if(i == 0 && (mbMasuAttrGet(masuId) & 0x4000)) {
                continue;
            }
            CapSelectMasuAddFront(list, linkTbl[i], num);
        }
    }
}

static void CapSelectMasuAddBack(s16 *list, s16 masuId, s16 num)
{
    s16 linkTbl[10];
    s16 linkNum;
    s16 endF;
    int i;
    int addNum;

    if(CapSelectMasuCheck(masuId)) {
        list[masuId] = 1;
    } else {
        list[masuId] = -0x8000;
    }
    if(mbMasuDispCheck(masuId) && mbev_CapMasuMoveCheck(masuId)) {
        num = 0;
    }
    if(CapSelectMasuDispCheck(masuId) && !capsuleMasuSelectEndF) {
        num--;
    }
    endF = capsuleMasuSelectEndF;
    capsuleMasuSelectEndF = FALSE;
    addNum = 0;
    if(num > 0) {
        linkNum = mbMasuLinkParentGet(masuId, linkTbl);
        for(i = 0; i < linkNum; i++) {
            if(mbev_CapMasuMoveCheck(linkTbl[i]) && !mbMasuDispCheck(linkTbl[i])) {
                continue;
            }
            CapSelectMasuAddBack(list, linkTbl[i], num);
            addNum++;
        }
    }
    if(endF == FALSE && addNum <= 0 && mbMasuTypeGet(masuId) == 0) {
        CapSelectMasuLinkCheck(list, masuId);
    }
}

static void CapSelectMasuLinkCheck(s16 *list, s16 masuId)
{
    s16 linkTbl[10];
    s16 linkNum;
    int i;

    if(masuId < 1) {
        return;
    }
    linkNum = mbMasuLinkTblGet(masuId, linkTbl);
    if(list[masuId] == 0) {
        return;
    }
    if(mbMasuTypeGet(masuId) == 0) {
        list[masuId] = 0;
    } else {
        return;
    }
    for(i = 0; i < linkNum; i++) {
        if(!mbev_CapMasuMoveCheck(linkTbl[i])) {
            CapSelectMasuLinkCheck(list, linkTbl[i]);
        }
    }
}

static int CapSelectMasuComListGet(s16 *out, s16 *list, s16 masuId, s16 target, s16 num)
{
    s16 linkTbl[10];
    s16 linkNum;
    int i;

    out[num] = masuId;
    num++;
    if(masuId == target) {
        capsuleComSearchF = TRUE;
        return num;
    }
    linkNum = mbMasuLinkTblGet(masuId, linkTbl);
    for(i = 0; i < linkNum; i++) {
        if(list[linkTbl[i]] != 0) {
            CapSelectMasuComListGet(out, list, linkTbl[i], target, num);
            if(capsuleComSearchF) {
                return num;
            }
        }
    }
    return num;
}

static int CapSelectMasuComListGetRev(s16 *out, s16 *list, s16 masuId, s16 target, s16 num)
{
    s16 linkTbl[10];
    s16 linkNum;
    int i;

    out[num] = masuId;
    num++;
    if(masuId == target) {
        capsuleComSearchF = TRUE;
        return num;
    }
    linkNum = mbMasuLinkParentGet(masuId, linkTbl);
    for(i = 0; i < linkNum; i++) {
        if(list[linkTbl[i]] != 0) {
            CapSelectMasuComListGetRev(out, list, linkTbl[i], target, num);
            if(capsuleComSearchF) {
                return num;
            }
        }
    }
    return num;
}

static int CapSelectMasuWinCreate(int type)
{
    int winId;
    HuVec2f winPos;
    switch(type) {
        case 0:
            winId = mbWinCreate(6, 0x370034, -1);
            break;
    }
    mbWinMesSpeedSet(winId, 0);
    mbWinPause(winId);
    mbWinTopPosGet(&winPos);
    mbWinTopPosSet(winPos.x, winPos.y - 8.0f);
    return winId;
}

static int CapUseDelete(int playerNo, int capsuleNo)
{
    int result;
    CAPSELMASU_WORK *work;
    void *buf;

    capsuleNo = mbCapValueTypeGet(capsuleNo);
    buf = HuMemDirectMallocNum(HEAP_HEAP, sizeof(CAPSELMASU_WORK), HU_MEMNUM_OVL);
    work = buf;
    memset(work, 0, sizeof(CAPSELMASU_WORK));
    work->playerNo = playerNo;
    work->unk4 = -1;
    work->capsuleNo = capsuleNo;
    capsuleMasuSelectComF[playerNo] = FALSE;
    result = CapUseDeleteWin(work);
    HuMemDirectFree(work);
    return result;
}

static int CapUseDeleteWin(CAPSELMASU_WORK *work)
{
    int choice;
    int time;
    int index;
    float t;
    float scale;
    int objId;
    int mdlId;
    HuVecF pos;

    choice = -1;
    mbCapSelectResultGet(work->playerNo, &objId, &mdlId);
    mbCapSelectResultReset(work->playerNo);
    if(objId != -1) {
        work->capObjId = capsuleObjId = objId;
    } else {
        work->capObjId = capsuleObjId = mbCapObjCreate(work->capsuleNo, 1);
        mbObjDispSet(work->capObjId, FALSE);
        mbPlayerPosGet(work->playerNo, &pos);
        mbObjPosSet(work->capObjId, pos.x, 250.0f + pos.y, pos.z);
        mbObjScaleSet(work->capObjId, 1.2f, 1.2f, 1.2f);
    }
    mbObjLayerSet(work->capObjId, 4);
    CapObjAttrLoopSet(work->capObjId);
    work->mdlId = -1;
    work->winId = -1;
    work->winId2 = -1;
    mbObjDispSet(work->capObjId, TRUE);
    time = 0;
    do {
        if(objId == -1) {
            time++;
            t = time / 18.0f;
            if(t > 1.0f) {
                t = 1.0f;
            }
            mbPlayerPosGet(work->playerNo, &pos);
            pos.y += 100.0 + 150.0 * HuSin(90.0f * t);
            mbObjPosSetV(work->capObjId, &pos);
            scale = 1.2f * HuSin(90.0f * t);
            mbObjScaleSet(work->capObjId, scale, scale, scale);
        } else {
            t = 1.0f;
        }
        HuPrcVSleep();
    } while(!mbCameraMoveCheck() || t < 1.0f);
    mbWinCreateChoice(2, 0x370026, -1, 1);
    mbWinTopInsertMesSet(mbCapUseMesGet(work->capsuleNo), 0);
    if(GwPlayer[work->playerNo].comF) {
        CapComChoiceSet(-1);
    }
    mbWinTopWait();
    choice = mbWinTopChoiceGet();
    if(choice == 0) {
        CapEffRemoveCreate();
        HuPrcVSleep();
        mbAudFXPlay(0x3FD);
        mbObjDispSet(work->capObjId, FALSE);
        mbObjPosGet(work->capObjId, &pos);
        CapEffRemoveAddAll(pos);
        omVibrate(work->playerNo, 20, 4, 4);
        if(!capsuleUseRemoveOnF) {
            mbPlayerCapsuleUseSet(work->capsuleNo);
            index = mbPlayerCapsuleFind(work->playerNo, work->capsuleNo);
            if(index != -1) {
                mbPlayerCapsuleRemove(work->playerNo, index);
            }
        }
        do {
            HuPrcVSleep();
        } while(CapEffRemoveCheck());
        CapEffRemoveKill();
        HuPrcVSleep();
    } else {
        if(objId != -1) {
            mbCapSelectResultSet(work->playerNo, objId, mdlId);
            work->capObjId = -1;
        }
    }
    if(work->winId != -1) {
        mbWinKill(work->winId);
    }
    work->winId = -1;
    if(work->winId2 != -1) {
        mbWinKill(work->winId2);
    }
    work->winId2 = -1;
    if(work->capObjId != -1) {
        mbCapObjKill(work->capObjId);
    }
    work->capObjId = -1;
    capsuleObjId = -1;
    CapUseDeleteKill(work);
    if(choice == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static void CapUseDeleteKill(CAPSELMASU_WORK *work)
{
    if(work->capObjId != -1) {
        mbCapObjKill(work->capObjId);
    }
    work->capObjId = -1;
    if(work->mdlId != -1) {
        mbObjKill(work->mdlId);
    }
    work->mdlId = -1;
    if(work->winId != -1) {
        mbWinKill(work->winId);
    }
    work->winId = -1;
    if(work->winId2 != -1) {
        mbWinKill(work->winId2);
    }
    work->winId2 = -1;
}

typedef struct CapsuleListFile_s {
    int boardNo;
    int dataNum;
} CAPSULE_LIST_FILE;

typedef struct CapsuleListDefine_s {
    u32 capsuleNo;
    char *name;
} CAPSULE_LIST_DEFINE;

static CAPSULE_LIST_FILE capsuleListFileTbl[12] = { 1 };
static CAPSULE_LIST_DEFINE capsuleListDefineTbl[24] = {
    { 0, "CAPSULE_KINOKO" },
    { 1, "CAPSULE_SKINOKO" },
    { 2, "CAPSULE_PKINOKO" },
    { 3, "CAPSULE_MKINOKO" },
    { 4, "CAPSULE_KILLER" },
    { 5, "CAPSULE_DOKAN" },
    { 6, "CAPSULE_HANACHAN" },
    { 7, "CAPSULE_TOGEZO" },
    { 8, "CAPSULE_KURIBO" },
    { 9, "CAPSULE_PAKKUN" },
    { 10, "CAPSULE_JANGO" },
    { 11, "CAPSULE_PATAPATA" },
    { 12, "CAPSULE_KOKAMEKKU" },
    { 13, "CAPSULE_KAMEKKU" },
    { 14, "CAPSULE_THROWMAN" },
    { 15, "CAPSULE_BOBLE" },
    { 16, "CAPSULE_BIRIQ" },
    { 17, "CAPSULE_TUMUJIKUN" },
    { 18, "CAPSULE_DOSSUN" },
    { 19, "CAPSULE_BOMHEI" },
    { 20, "CAPSULE_HONE" },
    { 21, "CAPSULE_LIGHT" },
    { 22, "CAPSULE_TARU" },
    { -1, "CAPSULE_NULL" },
};
static int capsuleListColW[16] = { 1 };

void mbCapListInit(CAPSULE_LIST *list)
{
    int i;
    int num;
    for(i = 0, num = 0; i < 33; i++) {
        if(list[i].capsuleNo == -1) {
            break;
        }
        if(mbCapListExcludeCheck(list[i].capsuleNo)) {
            capsuleList[num] = list[i];
            num++;
        }
    }
    capsuleList[num].capsuleNo = -1;
    if(!mbSaveNewF) {
        memset(capsuleNum, 0, sizeof(capsuleNum));
    }
}

void mbCapListRead(void)
{
    CAPSULE_LIST_FILE *file;
    int i;
    int num;
    CAPSULE_LIST *buf;
    CAPSULE_LIST *data;

    for(file = capsuleListFileTbl, i = 0; file->boardNo >= 0; i++, file++) {
        if(file->boardNo == mbBoardNoGet()) {
            break;
        }
    }
    data = HuDataSelHeapReadNum(file->dataNum, HU_MEMNUM_OVL, HEAP_MODEL);
    buf = data;
    for(i = 0, num = 0; i < 33; i++, data++) {
        if(data->capsuleNo == -1) {
            break;
        }
        if(mbCapListExcludeCheck(data->capsuleNo)) {
            capsuleList[num] = *data;
            num++;
        }
    }
    capsuleList[num].capsuleNo = -1;
    HuMemDirectFree(buf);
    if(!mbSaveNewF) {
        memset(capsuleNum, 0, sizeof(capsuleNum));
    }
}

int mbCapListCopy(CAPSULE_LIST *list)
{
    int i;
    for(i = 0; i < 32; i++) {
        if(capsuleList[i].capsuleNo == -1) {
            break;
        }
        list[i] = capsuleList[i];
    }
    capsuleList[i].capsuleNo = -1;
    return i;
}

void mbCapListDebug(void)
{
    static GXColor winColor = { 0, 0, 0x90, 0xC0 };
    s8 *q;
    int col;
    CAPSULE_LIST *p;
    int i;
    int k;
    int row;
    int color;
    CAPSULE_LIST *list;
    int x;
    CAPSULE_LIST_DEFINE *def;
    int mode;
    int dy;
    int y;
    int j;
    s8 unkB0[120];
    s8 unk38[120];
    CAPSULE_LIST backup;
    int num;
    int unk20;
    int prevColor;
    int dx;
    void *buf;
    s16 sum;

    def = capsuleListDefineTbl;
    buf = HuMemDirectMallocNum(HEAP_HEAP, 0x210, HU_MEMNUM_OVL);
    list = buf;
    p = list;
    memset(list, 0, 0x210);
    for(i = 0, p = list; i < 33; i++, p++) {
        p->capsuleNo = -1;
    }
    for(j = 0; j < 32; j++) {
        if(capsuleList[j].capsuleNo == -1) {
            break;
        }
        list[j] = capsuleList[j];
    }
    capsuleList[j].capsuleNo = -1;
    col = row = unk20 = mode = 0;
    memset(unkB0, 0, sizeof(unkB0));
    memset(unk38, 0, sizeof(unk38));
    do {
        printWin(4, 96, 630, 270, &winColor);
        x = 8;
        y = 100;
        prevColor = color = -1;
        for(i = 0, p = list; i < 15; i++, p++) {
            q = (s8 *)p;
            x = 8;
            for(k = 0; k < 16; k++) {
                switch(mode) {
                    case 0:
                        if(row == i) {
                            color = 14;
                        } else {
                            color = 15;
                        }
                        break;
                    case 1:
                        if(row == i) {
                            if(col == k) {
                                color = 14;
                            } else {
                                color = 10;
                            }
                        } else {
                            color = 15;
                        }
                        break;
                }
                if(color != prevColor) {
                    fontcolor = color;
                }
                prevColor = color;
                if(k == 0) {
                    if(p->capsuleNo != -1) {
                        print8(x, y, 1.5f, "%s", mbCapDebugNameGet(p->capsuleNo));
                    } else {
                        print8(x, y, 1.5f, "NULL");
                    }
                } else {
                    print8(x, y, 1.5f, "%02d", q[k]);
                }
                x += capsuleListColW[k];
            }
            y += 16;
        }
        x = 8;
        for(k = 0; k < 16; k++) {
            if(mode == 1 && k == col) {
                color = 14;
            } else {
                color = 13;
            }
            if(color != prevColor) {
                fontcolor = color;
            }
            prevColor = color;
            sum = 0;
            for(i = 0, p = list; i < 15; i++, p++) {
                if(p->capsuleNo != -1) {
                    q = (s8 *)p;
                    sum += q[k];
                }
            }
            if(k >= 4) {
                if(sum < 100) {
                    print8(x, y, 1.5f, "%02d", sum);
                } else {
                    print8(x - 8, y, 1.5f, "%02d", sum);
                }
            }
            x += capsuleListColW[k];
        }
        dx = dy = 0;
        if(HuPadDStkRep[0] & PAD_BUTTON_RIGHT) {
            dx = 1;
        } else if(HuPadDStkRep[0] & PAD_BUTTON_LEFT) {
            dx = -1;
        }
        if(HuPadDStkRep[0] & PAD_BUTTON_UP) {
            dy = -1;
        } else if(HuPadDStkRep[0] & PAD_BUTTON_DOWN) {
            dy = 1;
        }
        switch(mode) {
            case 0:
                if(dy > 0) {
                    row++;
                } else if(dy < 0) {
                    row--;
                }
                if(row >= 15) {
                    row = 0;
                } else if(row < 0) {
                    row = 14;
                }
                break;
            case 1:
                if(dx > 0) {
                    col++;
                } else if(dx < 0) {
                    col--;
                }
                if(col >= 16) {
                    col = 0;
                } else if(col < 0) {
                    col = 15;
                }
                q = (s8 *)&list[row];
                if(dy > 0) {
                    q[col]--;
                } else if(dy < 0) {
                    q[col]++;
                }
                if(col == 0) {
                    if(dy < 0) {
                        while(!mbCapValidCheck(q[col]) && q[col] < 32) {
                            q[col]++;
                        }
                    } else if(dy > 0) {
                        while(!mbCapValidCheck(q[col]) && q[col] > -1) {
                            q[col]--;
                        }
                    }
                    if(q[col] > 32) {
                        q[col] = 32;
                    } else if(q[col] < -1) {
                        q[col] = -1;
                    }
                } else {
                    if(q[col] > 99) {
                        q[col] = 99;
                    } else if(q[col] < 0) {
                        q[col] = 0;
                    }
                }
                break;
        }
        switch(mode) {
            case 0:
                if(HuPadBtnDown[0] & PAD_BUTTON_A) {
                    memcpy(&backup, &list[row], sizeof(CAPSULE_LIST));
                    mode++;
                }
                break;
            case 1:
                if(HuPadBtnDown[0] & PAD_BUTTON_A) {
                    mode = 0;
                } else if(HuPadBtnDown[0] & PAD_BUTTON_B) {
                    memcpy(&list[row], &backup, sizeof(CAPSULE_LIST));
                    mode = 0;
                }
                break;
        }
        if(HuPadBtnDown[0] & PAD_BUTTON_Y) {
            OSReport("static CAPSULE_LIST capsuleList[] = { \n");
            for(i = 0, p = list; i < 15; i++, p++) {
                if(p->capsuleNo != -1) {
                    q = (s8 *)p;
                    for(k = 0, def = capsuleListDefineTbl; def->capsuleNo != -1; k++, def++) {
                        if(def->capsuleNo == p->capsuleNo) {
                            break;
                        }
                    }
                    OSReport("  { %s, ", def->name);
                    OSReport("{%d,%d,%d}, ", q[1], q[2], q[3]);
                    OSReport("{%d,%d,%d,%d,%d,%d,%d,%d}, ", q[4], q[5], q[6], q[7], q[8], q[9], q[10], q[11]);
                    OSReport("{%d,%d,%d,%d} }, \n", q[12], q[13], q[14], q[15]);
                }
            }
            OSReport("\t{ CAPSULE_NULL, ");
            OSReport("{%d,%d,%d}, ", 0, 0, 0);
            OSReport("{%d,%d,%d,%d,%d,%d,%d,%d}, ", 0, 0, 0, 0, 0, 0, 0, 0);
            OSReport("{%d,%d,%d,%d} }, \n", 0, 0, 0, 0);
            OSReport("};\n");
        }
        HuPrcVSleep();
    } while(!(HuPadBtn[0] & PAD_TRIGGER_R) || !(HuPadBtn[0] & PAD_TRIGGER_L));
    for(i = 0, num = 0, p = list; i < 33; i++, p++) {
        if(p->capsuleNo != -1) {
            capsuleList[num] = *p;
            num++;
        }
    }
    capsuleList[num].capsuleNo = -1;
    HuMemDirectFree(list);
}

static int capsuleNumColW[16] = { 1 };

void mbCapNumInc(int capsuleNo, int type)
{
    capsuleNum[capsuleNo][type]++;
}

void mbCapNumDebug(void)
{
    static GXColor winColor = { 0, 0, 0x90, 0xC0 };
    CAPSULE_LIST *p;
    int k;
    int color;
    int i;
    int prevColor;
    float y;
    float x;

    do {
        printWin(4, 96, 630, 270, &winColor);
        x = 8.0f;
        y = 100.0f;
        color = prevColor = -1;
        for(i = 0, p = capsuleList; i < 15; i++, p++) {
            x = 80.0f;
            for(k = 0; k < 4; k++) {
                if(color != prevColor) {
                    fontcolor = color;
                }
                prevColor = color;
                if(k == 0) {
                    if(p->capsuleNo != -1) {
                        print8(x, y, 1.5f, "%s", mbCapDebugNameGet(p->capsuleNo));
                    } else {
                        print8(x, y, 1.5f, "NULL");
                    }
                } else if(k == 3) {
                    print8(x, y, 1.5f, "%02d", capsuleNum[p->capsuleNo][0] + capsuleNum[p->capsuleNo][1]);
                } else {
                    print8(x, y, 1.5f, "%02d", capsuleNum[p->capsuleNo][k - 1]);
                }
                x += capsuleNumColW[k];
            }
            y += 16.0f;
        }
        HuPrcVSleep();
    } while(!(HuPadBtn[0] & PAD_TRIGGER_R) || !(HuPadBtn[0] & PAD_TRIGGER_L));
}

s16 mbCapValueTypeGet(s16 value)
{
    return value & 0xFF;
}

s16 mbCapMasuTypeGet(s16 masuId)
{
    return mbCapValueTypeGet(mbMasuCapsuleGet(masuId));
}

s16 mbCapValuePlayerGet(s16 value)
{
    return (value >> 8) & 0xFF;
}

s16 mbCapMasuPlayerGet2(s16 masuId)
{
    return mbMasuCapsuleGet(masuId) >> 8;
}

void mbCapMasuPlayerSet(s16 masuId, s16 playerNo)
{
    s16 value = mbMasuCapsuleGet(masuId);
    value |= (playerNo & 0xFF) << 8;
    mbMasuCapsuleSet(masuId, value);
}

void mbCapMasuPlayerTypeSet(s16 masuId, s16 type, s16 playerNo)
{
    mbMasuCapsuleSet(masuId, type);
    mbCapMasuPlayerSet(masuId, playerNo);
}

s16 mbCapMasuPlayerGet(s16 masuId)
{
    return mbMasuCapsuleGet(masuId) >> 8;
}

s16 mbCapUseModeGet(s16 capsuleNo)
{
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    return capsuleData[capsuleNo].useMode;
}

BOOL mbCapUseTrapCheck(s16 capsuleNo)
{
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    if(mbCapUseModeGet(capsuleNo) == 2) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int mbCapBuyCostGet(s16 capsuleNo, s16 playerNo)
{
    int cost = -1;
    int i;
    int type;
    if(!GwSystem.curTime) {
        type = 0;
    } else {
        switch(GwPlayer[playerNo].rank) {
            case 0:
                type = 1;
                break;
            case 1:
                type = 0;
                break;
            default:
                type = 2;
                break;
        }
    }
    for(i = 0; i < 32; i++) {
        if(capsuleList[i].capsuleNo == -1) {
            break;
        }
        if(capsuleNo == capsuleList[i].capsuleNo) {
            if(capsuleList[i].cost[type] <= 0) {
                break;
            }
            cost = capsuleList[i].cost[type];
            break;
        }
    }
    if(cost > 0) {
        return cost;
    } else {
        return mbCapCostGet(capsuleNo);
    }
}

int mbCapSellCostGet(s16 capsuleNo, s16 playerNo)
{
    int cost = -1;
    int i;
    int type;
    switch(GwPlayer[playerNo].rank) {
        case 0:
            type = 2;
            break;
        case 1:
            type = 0;
            break;
        default:
            type = 1;
            break;
    }
    for(i = 0; i < 32; i++) {
        if(capsuleList[i].capsuleNo == -1) {
            break;
        }
        if(capsuleNo == capsuleList[i].capsuleNo) {
            if(capsuleList[i].cost[type] <= 0) {
                break;
            }
            cost = capsuleList[i].cost[type];
            break;
        }
    }
    if(cost > 0) {
        return cost;
    } else {
        return mbCapCostGet(capsuleNo);
    }
}

int mbCapCostGet(s16 capsuleNo)
{
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    return capsuleData[capsuleNo].cost;
}

int mbCapListExcludeCheck(s16 capsuleNo)
{
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    return capsuleData[capsuleNo].listExcludeF;
}

BOOL mbCapThrowMasuCheck(int masuId)
{
    return CapSelectMasuCheck(masuId);
}

static BOOL CapCheckComPath(int playerNo, int maxStep, int mode);

int mbCapComChanceGet(int capsuleNo, int playerNo, int mode)
{
    CAPSULE_CHANCE *chance = capsuleChanceTbl;
    int i;
    int dif;
    int rate;
    s8 flag;
    int step;
    int step1;
    int step2;
    int step3;
    capsuleSelectComBack = FALSE;
    for(i = 0; chance->capsuleNo != -1; i++, chance++) {
        if(chance->capsuleNo == capsuleNo) {
            break;
        }
    }
    if(chance->capsuleNo < 0) {
        return -1;
    }
    dif = GwPlayer[playerNo].charNo;
    if(dif > 10) {
        dif = 10;
    }
    rate = chance->dif[dif].chance;
    flag = chance->dif[dif].flag;
    step = mbMasuFind_TypeStepGet(GwPlayer[playerNo].masuId, 7);
    switch(capsuleNo) {
        case 1:
            if(flag == '*') {
                if(CapCheckComPath(playerNo, 15, 0)) {
                    rate = 0;
                }
            }
            break;
        case 2:
            step1 = mbMasuFind_TypeStepGet(GwPlayer[playerNo].masuId, 3);
            step2 = mbMasuFind_TypeStepGet(GwPlayer[playerNo].masuId, 5);
            step3 = mbMasuFind_TypeStepGet(GwPlayer[playerNo].masuId, 4);
            if((step1 >= 1 && step1 <= 10) || (step2 >= 1 && step2 <= 10) || (step3 >= 1 && step3 <= 10)) {
                if(flag == 'a') {
                    rate = 80;
                } else if(flag == 'b') {
                    rate = 50;
                }
            }
            break;
        case 3:
            if(flag == '*') {
                if(CapCheckComPath(playerNo, 10, 1)) {
                    rate = 80;
                }
            }
            break;
        case 4:
            if(flag == '*') {
                for(i = 0; i < 4; i++) {
                    if(i != playerNo && GwPlayer[i].masuId == GwPlayer[playerNo].masuId) {
                        break;
                    }
                }
                if(i >= 4) {
                    if(!CapCheckComPath(playerNo, 10, 2)) {
                        rate = 0;
                    }
                }
            }
            if(step >= 1 && step <= 10) {
                rate = 0;
            }
            break;
        case 5:
            if(flag == '*') {
                for(i = 0; i < 4; i++) {
                    if(i != playerNo) {
                        step1 = mbMasuFind_TypeStepGet(GwPlayer[i].masuId, 7);
                        if(step1 >= 1 && step1 <= 10) {
                            break;
                        }
                    }
                }
                if(i < 4) {
                    rate = 80;
                }
            }
            break;
        case 6:
            if(flag == '*') {
                if(mbPlayerCoinGet(playerNo) <= 20) {
                    rate = 0;
                }
            }
            break;
        case 11:
            if(flag == '*') {
                if(mbPlayerCoinGet(playerNo) < 10) {
                    rate = 0;
                }
            }
            break;
        case 22:
            if(flag == '*' && step >= 0 && step <= 5 && mbCapSelectMasuBackNum(GwPlayer[playerNo].masuId) > 0) {
                rate = 80;
                capsuleSelectComBack = TRUE;
            }
            break;
        case 23:
            if(flag == '*' && step >= 0 && step <= 5 && mbCapSelectMasuBackNum(GwPlayer[playerNo].masuId) > 0) {
                rate = 80;
                capsuleSelectComBack = TRUE;
            }
            break;
        case 30:
            if(flag == '*') {
                if(mbPlayerCapsuleNumGet(playerNo) >= mbPlayerCapsuleMaxGet()) {
                    rate = 30;
                }
            }
            if(mode == 0) {
                if(mbPlayerCapsuleNumGet(playerNo) < mbPlayerCapsuleMaxGet()) {
                    rate = 0;
                }
            }
            if(mode == 2) {
                if(mbPlayerCapsuleNumGet(playerNo) < mbPlayerCapsuleMaxGet()) {
                    rate = 100;
                }
            }
            break;
        case 31:
            if(flag == '*') {
                if(mbPlayerCapsuleNumGet(playerNo) >= mbPlayerCapsuleMaxGet()) {
                    rate = 30;
                }
            }
            if(mode == 0) {
                if(mbPlayerCapsuleNumGet(playerNo) < mbPlayerCapsuleMaxGet()) {
                    rate = 0;
                }
            }
            if(mode == 2) {
                if(mbPlayerCapsuleNumGet(playerNo) < mbPlayerCapsuleMaxGet()) {
                    rate = 100;
                }
            }
            break;
    }
    switch(mode) {
        case 0:
            return rate;
        case 1:
            return chance->dif[dif].chance;
        case 2:
            return 100 - rate;
        default:
            return 0;
    }
}

static BOOL CapCheckComPath(int playerNo, int maxStep, int mode)
{
    int masuId;
    int i;
    int step;
    s16 stackNum;
    int linkMasu;
    int linkNum;
    int endF;
    int unk10;
    int startMasu;
    s16 stackMasu[32];
    s16 stackStep[32];
    s8 visit[256];

    startMasu = GwPlayer[playerNo].masuId;
    masuId = startMasu;
    step = maxStep;
    unk10 = 0;
    endF = FALSE;
    stackNum = 0;
    memset(visit, 0, sizeof(visit));
    do {
        do {
            visit[masuId] = TRUE;
            if(masuId != startMasu && ((mbMasuAttrGet(masuId) & mbBranchAttrGet()) || (mbMasuMAttrGet(masuId) & mbBranchMAttrGet()))) {
                step = 0;
                break;
            }
            if(mbMasuDispCheck(masuId)) {
                step--;
            }
            if(startMasu != masuId) {
                switch(mode) {
                    case 0:
                        if(mbCapMasuDispTypeGet(masuId) == 2 && mbCapValueTypeGet(mbMasuCapsuleGet(masuId)) == 23 && playerNo != mbCapMasuPlayerGet(masuId)) {
                            return TRUE;
                        }
                        break;
                    case 1:
                        if(mbCapMasuDispTypeGet(masuId) == 2 && playerNo != mbCapMasuPlayerGet(masuId)) {
                            return TRUE;
                        }
                        break;
                    case 2:
                        for(i = 0; i < 4; i++) {
                            if(i != playerNo && masuId == GwPlayer[i].masuId) {
                                break;
                            }
                        }
                        if(i < 4) {
                            return TRUE;
                        }
                        break;
                }
            }
            if(mbMasuLinkNumGet(masuId) <= 0 || step <= 0) {
                step = 0;
                break;
            }
            linkNum = mbMasuLinkNumGet(masuId);
            for(i = 0; i < linkNum; i++) {
                linkMasu = mbMasuLinkGet(masuId, i);
                if(visit[linkMasu]) {
                    continue;
                }
                if(i < mbMasuLinkNumGet(masuId) - 1) {
                    stackMasu[stackNum] = masuId;
                    stackStep[stackNum] = step;
                    stackNum++;
                }
                masuId = linkMasu;
                break;
            }
            if(i >= linkNum) {
                step = 0;
            }
            if(mbMasuLinkNumGet(masuId) <= 0) {
                visit[masuId] = TRUE;
            }
        } while(step > 0);
        if(stackNum <= 0) {
            endF = TRUE;
        } else {
            stackNum--;
            masuId = stackMasu[stackNum];
            step = stackStep[stackNum];
        }
    } while(!endF);
    return FALSE;
}

typedef struct CapComSel_s {
    int idx;
    int capsuleNo;
    int chance;
    int backF;
} CAP_COMSEL;

int mbCapSelectComGet(int playerNo, int *capsuleTbl, int num)
{
    int i;
    CAP_COMSEL *e;
    int j;
    int total;
    float rnd;
    CAP_COMSEL tmp;
    CAP_COMSEL list[10];

    if(_CheckFlag(0x1000E)) {
        i = mbTutorialCall(15);
        return i;
    }
    if(num <= 0) {
        return -1;
    }
    if(GwSystem.turnNo >= GwSystem.turnMax && mbPlayerCoinGet(playerNo) >= 20) {
        for(i = 0; i < num; i++) {
            if(capsuleTbl[i] == 6) {
                break;
            }
        }
        if(i < num) {
            return i;
        }
    }
    if(mbPlayerCapsuleNumGet(playerNo) < mbPlayerCapsuleMaxGet()) {
        if(100.0f * ((1.0f / 268435456.0f) * mbRandMod(0x10000000)) < 30.0f) {
            return -1;
        }
    }
    for(i = 0, e = list; i < num; i++, e++) {
        e->idx = i;
        e->capsuleNo = capsuleTbl[i];
        e->chance = mbCapComChanceGet(e->capsuleNo, playerNo, 0);
        if(capsuleSelectComBack) {
            e->backF = TRUE;
        } else {
            e->backF = FALSE;
        }
        if(mbCapUseModeGet(e->capsuleNo) == 1 || mbCapUseModeGet(e->capsuleNo) == 2) {
            if(mbCapSelectMasuNum(GwPlayer[playerNo].masuId) <= 0) {
                e->chance = 0;
            }
        }
    }
    for(i = 0; i < num; i++) {
        for(j = i + 1; j < num; j++) {
            if(list[i].chance < list[j].chance) {
                tmp = list[i];
                list[i] = list[j];
                list[j] = tmp;
            }
        }
    }
    j = num;
    num = 0;
    total = 0;
    for(i = 0; i < j; i++) {
        if(list[i].chance > 0) {
            total += list[i].chance;
            num++;
        }
    }
    if(num <= 0) {
        return -1;
    }
    rnd = total * ((1.0f / 268435456.0f) * mbRandMod(0x10000000));
    for(i = 0, e = list; i < num; i++, e++) {
        rnd -= e->chance;
        if(rnd <= 0.0f) {
            break;
        }
    }
    if(i >= num) {
        return -1;
    }
    if(e->backF) {
        capsuleSelectComBack = TRUE;
    } else {
        capsuleSelectComBack = FALSE;
    }
    return e->idx;
}

typedef struct CapComDel_s {
    int idx;
    int capsuleNo;
    int chance;
} CAP_COMDEL;

int mbCapSelectDeleteComGet(int playerNo, int *capsuleTbl, int num)
{
    int i;
    CAP_COMDEL *e;
    int j;
    CAP_COMDEL tmp;
    CAP_COMDEL list[10];

    if(num <= 0) {
        return -1;
    }
    for(i = 0, e = list; i < num; i++, e++) {
        e->idx = i;
        e->capsuleNo = capsuleTbl[i];
        e->chance = mbCapComChanceGet(e->capsuleNo, playerNo, 2);
    }
    for(i = 0; i < num; i++) {
        for(j = i + 1; j < num; j++) {
            if(list[i].chance < list[j].chance) {
                tmp = list[i];
                list[i] = list[j];
                list[j] = tmp;
            }
        }
    }
    return list[0].idx;
}
/* @@PART3@@ */
/* Part 2 local stubs for later parts' statics (dropped by the splice). */
static void CapGuideCreate(void) {}
static void CapGuideRotYSet(int masuId, float rotY) {}
static void CapGuideGrowSet(void) {}
static void CapGuideKill(void) {}
static void CapEffMasuOkCreate(void) {}
static void CapEffMasuOkAddAll(s16 masuId, s16 *list) {}
static void CapEffMasuOkDispSet(int dispF) {}
static void CapEffMasuOkPosSet(HuVecF *pos, int masuId) {}
static void CapEffMasuOkNext(void) {}
static void CapEffMasuOkKill(void) {}
static void CapEffRemoveCreate(void) {}
static void CapEffRemoveAddAll(HuVecF pos) {}
static int CapEffRemoveCheck(void) { return 0; }
static void CapEffRemoveKill(void) {}
static void CapEffHiliteCreate(void) {}
static void CapEffHiliteAdd(HuVecF pos, HuVecF rot, HuVecF scale, int arg3, int arg4, int arg5, int arg6, GXColor color) {}
static void CapEffHiliteKill(void) {}
static void CapComChoiceSet(int choice) {}
static float CapCameraXZAngleGet(float angle) { return angle; }
static float CapAngleSumWrap(float a, float b) { return a; }
/* @@PART4@@ */
/* @@END@@ */
