#include "game/board/config.h"
#include "game/board/audio.h"
#include "game/board/malloc.h"
#include "game/board/math.h"
#include "game/board/object.h"
#include "game/board/player.h"

#include "game/gamework.h"
#include "game/esprite.h"
#include "game/sprite.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/memory.h"
#include "game/data.h"
#include "game/hu3d.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"

typedef struct PauseCursor_s {
    HuVecF pos; /* 0x00 */
    HuVecF startPos; /* 0x0C */
    HuVecF delta; /* 0x18 */
    BOOL dispF; /* 0x24 */
    int type; /* 0x28 */
    int hiliteMask; /* 0x2C */
    int mask; /* 0x30 */
    int time; /* 0x34 */
    int maxTime; /* 0x38 */
    float tpLvl[4]; /* 0x3C */
    s16 sprId[4]; /* 0x4C */
    s16 hiliteSprId[4]; /* 0x54 */
} PAUSE_CURSOR; /* size 0x5C */

typedef struct PausePlayer_s {
    int padNo; /* 0x00 */
    int playerNo; /* 0x04 */
    int padStat; /* 0x08 */
    int statTime; /* 0x0C */
} PAUSE_PLAYER; /* size 0x10 */

typedef struct PauseWork_s {
    u8 unk00[0x3C];
    int talkTime; /* 0x3C */
    int talkTimePrev; /* 0x40 */
    u8 unk44[0x194 - 0x44];
    PAUSE_CURSOR cursor; /* 0x194 */
    PAUSE_PLAYER player[4]; /* 0x1F0 */
} PAUSE_WORK; /* size 0x230 */

typedef struct PausePanel_s {
    int objId; /* 0x00 */
    int labelObjId; /* 0x04 */
    int sprId; /* 0x08 */
    ANIMDATA *anim; /* 0x0C */
    s16 animId[2]; /* 0x10 */
    HuVecF pos; /* 0x14 */
    HuVecF startPos; /* 0x20 */
    HuVecF endPos; /* 0x2C */
    BOOL batsuF; /* 0x38 */
    float scale; /* 0x3C */
    float scaleStart; /* 0x40 */
    float scaleEnd; /* 0x44 */
    float baseScale; /* 0x48 */
    s16 bank; /* 0x4C */
    s16 mode; /* 0x4E */
    s16 time; /* 0x50 */
    s16 maxTime; /* 0x52 */
    s16 delay; /* 0x54 */
    s16 anmTime; /* 0x56 */
    s16 anmMaxTime; /* 0x58 */
} PAUSE_PANEL; /* size 0x5C */

#define PAUSE_PANEL_MAX 20

static HuVecF playerPos;
static PAUSE_WORK pauseWork;
static BOOL playerDispF[4];

static HUPROCESS *configProc;
static BOOL configPadDisable;
static HUPROCESS *pauseGuideProc;
static PAUSE_PANEL *pausePanelWork;
static BOOL pauseGuideKillF;
static int pauseDispCopyModelId;
static int pauseDispCopyCounter;
static void *pauseDispCopyFb;
static s16 pausePlayer;
static int configResult;
static BOOL configDoneF;

static s16 pauseCursorSprOfsTbl[2][4][2] = {
    { { 0, -62 }, { 0, 68 }, { 62, 0 }, { -62, 0 } },
    { { 0, -20 }, { 0, 20 }, { 43, -60 }, { -45, -60 } },
};

static int pauseCursorMaskTbl[4] = { 8, 4, 2, 1 };

static inline BOOL GWStorySingleCheck(void)
{
    return GWPartyGet() == FALSE;
}

/* ---- Part 1 (other fork): placeholders so the unit compiles ---- */

int mbConfigExec(int playerNo, s16 guideMdlId)
{
    return 0;
}

/* ---- Part 2 ---- */

static void PauseCursorCreate(void)
{
    int i;
    PAUSE_CURSOR *cursor = &pauseWork.cursor;
    int bankTbl[4] = { 1, 3, 2, 0 };
    int sprId;

    cursor->dispF = FALSE;
    cursor->hiliteMask = 0;
    cursor->mask = 0;
    cursor->type = -1;
    cursor->time = -1;
    cursor->maxTime = -1;
    cursor->pos.x = cursor->pos.y = cursor->pos.z = 0.0f;
    for (i = 0; i < 4; i++) {
        cursor->tpLvl[i] = 0.0f;
        cursor->sprId[i] = sprId = espEntry(mbBoardDataNumGet(0x80020), 90, bankTbl[i] + 4);
        espAttrSet(sprId, 8);
        espPosSet(sprId, cursor->pos.x, cursor->pos.y);
        espDispOff(sprId);
        cursor->hiliteSprId[i] = sprId = espEntry(mbBoardDataNumGet(0x80020), 89, bankTbl[i]);
        espAttrSet(sprId, 8);
        espPosSet(sprId, cursor->pos.x, cursor->pos.y);
        espDispOff(sprId);
    }
}

static void PauseCursorKill(void)
{
    PAUSE_CURSOR *cursor = &pauseWork.cursor;
    int i;

    for (i = 0; i < 4; i++) {
        if (cursor->sprId[i] >= 0) {
            espKill(cursor->sprId[i]);
        }
        if (cursor->hiliteSprId[i] >= 0) {
            espKill(cursor->hiliteSprId[i]);
        }
        cursor->sprId[i] = cursor->hiliteSprId[i] = -1;
    }
}

static void PauseCursorHiliteSet(int type, int mask, int hiliteMask)
{
    PAUSE_CURSOR *cursor = &pauseWork.cursor;
    int i;

    cursor->dispF = FALSE;
    if (type >= 0) {
        cursor->dispF = TRUE;
    }
    if (cursor->type < 0 && type >= 0) {
        cursor->time = -1;
        cursor->maxTime = -1;
    }
    cursor->hiliteMask = hiliteMask;
    cursor->mask = mask;
    cursor->type = type;
    for (i = 0; i < 4; i++) {
        espDispOff(cursor->hiliteSprId[i]);
        if (cursor->dispF) {
            espDispOn(cursor->sprId[i]);
            if (hiliteMask & pauseCursorMaskTbl[i]) {
                espDispOn(cursor->hiliteSprId[i]);
            }
        } else {
            espDispOff(cursor->sprId[i]);
        }
    }
}

static void PauseCursorPosNextSet(int time, float posX, float posY)
{
    PAUSE_CURSOR *cursor = &pauseWork.cursor;
    int i;
    HuVecF pos;

    if (cursor->time < 0 || time <= 0) {
        cursor->time = 0;
        cursor->maxTime = 0;
        cursor->pos.x = posX;
        cursor->pos.y = posY;
        for (i = 0; i < 4; i++) {
            cursor->tpLvl[i] = 0.0f;
            if (cursor->mask & pauseCursorMaskTbl[i]) {
                cursor->tpLvl[i] = 1.0f;
            }
        }
    } else {
        cursor->time = cursor->maxTime = time;
        mbVecCopy(&cursor->pos, &cursor->startPos);
        cursor->delta.x = posX - cursor->startPos.x;
        cursor->delta.y = posY - cursor->startPos.y;
        cursor->delta.z = 0.0f;
    }
    mbNormPosto2D(&cursor->pos, &pos);
    for (i = 0; i < 4; i++) {
        espPosSet(cursor->sprId[i], pos.x + pauseCursorSprOfsTbl[cursor->type][i][0], pos.y + pauseCursorSprOfsTbl[cursor->type][i][1]);
        espTPLvlSet(cursor->sprId[i], cursor->tpLvl[i]);
        espPosSet(cursor->hiliteSprId[i], pos.x + pauseCursorSprOfsTbl[cursor->type][i][0], pos.y + pauseCursorSprOfsTbl[cursor->type][i][1]);
        espTPLvlSet(cursor->hiliteSprId[i], cursor->tpLvl[i]);
    }
}

static int PauseCursorMove(void)
{
    PAUSE_CURSOR *cursor = &pauseWork.cursor;
    int i;
    float t;
    HuVecF pos;

    if (cursor->time <= 0) {
        return 0;
    }
    cursor->time--;
    t = (float)(cursor->maxTime - cursor->time) / cursor->maxTime;
    t = mbSinDeg(90.0f * t);
    PSVECScale(&cursor->delta, &pos, t);
    PSVECAdd(&cursor->startPos, &pos, &cursor->pos);
    for (i = 0; i < 4; i++) {
        if (cursor->mask & pauseCursorMaskTbl[i]) {
            if (cursor->tpLvl[i] < 1.0f) {
                cursor->tpLvl[i] += 0.1f;
            }
            if (cursor->tpLvl[i] > 1.0f) {
                cursor->tpLvl[i] = 1.0f;
            }
        } else {
            if (cursor->tpLvl[i] > 0.0f) {
                cursor->tpLvl[i] -= 0.1f;
            }
            if (cursor->tpLvl[i] < 0.0f) {
                cursor->tpLvl[i] = 0.0f;
            }
        }
    }
    mbNormPosto2D(&cursor->pos, &pos);
    for (i = 0; i < 4; i++) {
        espPosSet(cursor->sprId[i], pos.x + pauseCursorSprOfsTbl[cursor->type][i][0], pos.y + pauseCursorSprOfsTbl[cursor->type][i][1]);
        espTPLvlSet(cursor->sprId[i], cursor->tpLvl[i]);
        espPosSet(cursor->hiliteSprId[i], pos.x + pauseCursorSprOfsTbl[cursor->type][i][0], pos.y + pauseCursorSprOfsTbl[cursor->type][i][1]);
        espTPLvlSet(cursor->hiliteSprId[i], cursor->tpLvl[i]);
    }
    return cursor->time;
}

static BOOL PausePlayerComRead(BOOL initF)
{
    PAUSE_PLAYER *player = pauseWork.player;
    BOOL result = FALSE;
    int i;
    int j;
    int temp;

    if (initF) {
        for (i = 0; i < 4; i++) {
            player[i].playerNo = i;
            player[i].padNo = GwPlayer[i].padNo;
            player[i].padStat = 0;
            player[i].statTime = 0;
        }
        for (i = 0; i < 3; i++) {
            for (j = i + 1; j < 4; j++) {
                if (player[i].padNo > player[j].padNo || (player[i].padNo == player[j].padNo && player[i].playerNo > player[j].playerNo)) {
                    temp = player[i].padNo;
                    player[i].padNo = player[j].padNo;
                    player[j].padNo = temp;
                    temp = player[i].playerNo;
                    player[i].playerNo = player[j].playerNo;
                    player[j].playerNo = temp;
                }
            }
        }
        if (GWPartyGet() != FALSE) {
            for (i = 0; i < 4; i++) {
                player[i].padStat = HuPadStatGet(player[i].padNo);
                if (player[i].padStat != 0 && !GwPlayer[player[i].playerNo].comF) {
                    GwPlayer[player[i].playerNo].comF = TRUE;
                    GwPlayerConf[player[i].playerNo].type = 1;
                }
            }
        }
    }
    if (GWPartyGet() == FALSE) {
        return FALSE;
    }
    for (i = 0; i < 4; i++) {
        temp = HuPadStatGet(player[i].padNo);
        if (temp != player[i].padStat) {
            player[i].statTime++;
        } else {
            player[i].statTime = 0;
        }
        if (player[i].statTime >= 4) {
            if (temp == 0) {
                GwPlayer[player[i].playerNo].comF = FALSE;
                GwPlayerConf[player[i].playerNo].type = 0;
                result = TRUE;
                player[i].padStat = temp;
            } else if (temp == -1) {
                GwPlayer[player[i].playerNo].comF = TRUE;
                GwPlayerConf[player[i].playerNo].type = 1;
                result = TRUE;
                player[i].padStat = temp;
            }
        }
    }
    return result;
}

static BOOL PausePadCheck(int no)
{
    PAUSE_PLAYER *player = pauseWork.player;
    return player[no].padStat != -1;
}

static void PauseDispCopyDraw(HU3D_MODEL *modelP, Mtx *mtx);

void mbPauseDispCopyCreate(void)
{
    u32 size;
    int i;

    pauseDispCopyFb = NULL;
    pauseDispCopyCounter = 0;
    size = GXGetTexBufferSize(320, 240, GX_TF_RGB565, GX_FALSE, 0);
    if (!GWStorySingleCheck) {
        pauseDispCopyFb = mbMallocFlush(size);
    } else {
        pauseDispCopyFb = mbMallocFlushModel(size);
    }
    pauseDispCopyModelId = Hu3DHookFuncCreate(PauseDispCopyDraw);
    Hu3DModelCameraSet(pauseDispCopyModelId, 4);
    Hu3DModelLayerSet(pauseDispCopyModelId, 2);
    HuPrcVSleep();
    for (i = 0; i < 4; i++) {
        playerDispF[i] = mbObjDispGet(mbPlayerObjIDGet(i));
        mbObjDispSet(mbPlayerObjIDGet(i), FALSE);
    }
}

void mbPauseDispCopyKill(void)
{
    int i;

    pauseDispCopyCounter = 0;
    HuMemDirectFree(pauseDispCopyFb);
    pauseDispCopyFb = NULL;
    Hu3DModelKill(pauseDispCopyModelId);
    for (i = 0; i < 4; i++) {
        if (playerDispF[i]) {
            mbObjDispSet(mbPlayerObjIDGet(i), TRUE);
        }
    }
}

static void PauseDispCopyDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    if (pauseDispCopyCounter == 0) {
        GXSetTexCopySrc(0, 0, 640, 480);
        GXSetTexCopyDst(320, 240, GX_TF_RGB565, GX_TRUE);
        GXCopyTex(pauseDispCopyFb, GX_FALSE);
        GXPixModeSync();
        pauseDispCopyCounter++;
    } else {
        Mtx modelview;
        Mtx44 proj;
        GXTexObj texObj;

        MTXOrtho(proj, 0, 480, 0, 640, 0, 100);
        GXSetProjection(proj, GX_ORTHOGRAPHIC);
        GXSetViewport(0, 0, 640, 480, 0, 1);
        GXSetScissor(0, 0, 640, 480);
        MTXIdentity(modelview);
        GXLoadPosMtxImm(modelview, GX_PNMTX0);
        GXSetCullMode(GX_CULL_NONE);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
        GXSetNumTexGens(1);
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
        GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
        GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        GXInitTexObj(&texObj, pauseDispCopyFb, 320, 240, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_TRUE);
        GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 8);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3s16(0, 0, -50);
        GXTexCoord2s16(0, 0);
        GXPosition3s16(640, 0, -50);
        GXTexCoord2s16(256, 0);
        GXPosition3s16(640, 480, -50);
        GXTexCoord2s16(256, 256);
        GXPosition3s16(0, 480, -50);
        GXTexCoord2s16(0, 256);
        GXEnd();
    }
}

static void PauseGuideMain(void);
static void PauseGuideDestroy(void);

void mbPauseGuideCreate(void)
{
    pausePanelWork = mbMalloc(PAUSE_PANEL_MAX * sizeof(PAUSE_PANEL));
    pauseGuideKillF = FALSE;
    pauseGuideProc = HuPrcChildCreate(PauseGuideMain, 0x2011, 0x2000, 0, mbMainProc);
    HuPrcDestructorSet2(pauseGuideProc, PauseGuideDestroy);
    HuPrcSetStat(pauseGuideProc, 12);
}

void mbPauseGuideKill(void)
{
    pauseGuideKillF = TRUE;
}

static inline float PauseFabs(register float x)
{
    asm {
        fabs x, x
    }
    return x;
}

static void PauseGuideMain(void)
{
    Mtx lookAt;
    Mtx rotMtx;
    Mtx invRot;
    HuVecF pos;
    HuVecF rot;
    PAUSE_PANEL *panel;
    int i;
    BOOL endF;
    BOOL priF;
    HU3D_CAMERA *cameraP;
    float t;
    float anmT;
    float weight;
    float scale;
    float sprScale;

    for (i = 0; i < 16; i++) {
        if ((1 << i) & 4) {
            break;
        }
    }
    cameraP = &Hu3DCamera[i];
    while (!pauseGuideKillF) {
        panel = pausePanelWork;
        for (i = 0; i < PAUSE_PANEL_MAX; i++, panel++) {
            if (!panel->objId) {
                continue;
            }
            endF = FALSE;
            panel->time++;
            if (panel->delay) {
                t = 0.0f;
                if (panel->time >= panel->delay) {
                    panel->delay = panel->time = 0;
                    if (panel->mode == 4) {
                        mbAudFXPlay(52);
                    } else if (panel->mode == 5) {
                        mbAudFXPlay(53);
                    }
                }
            } else {
                t = 1.0f;
                if (panel->time < panel->maxTime && panel->maxTime > 0) {
                    t = (float)panel->time / panel->maxTime;
                } else {
                    panel->time = panel->maxTime = 0;
                    endF = TRUE;
                }
                switch (panel->mode) {
                    case 0:
                    case 1:
                    case 2:
                        break;
                    case 4:
                        mbObjRotGet(panel->objId, &pos);
                        pos.y = -500.0f * (1.0f - t);
                        if (endF) {
                            pos.y = 0.0f;
                            if (panel->labelObjId && panel->batsuF) {
                                mbObjDispSet(panel->labelObjId, TRUE);
                            }
                        }
                        mbObjRotSetV(panel->objId, &pos);
                        mbObjDispSet(panel->objId, TRUE);
                        if (panel->sprId >= 0) {
                            espDispOn(panel->sprId);
                        }
                        break;
                    case 5:
                        mbObjRotGet(panel->objId, &pos);
                        pos.y = 500.0f * t;
                        if (panel->labelObjId) {
                            mbObjDispSet(panel->labelObjId, FALSE);
                        }
                        if (endF) {
                            pos.y = 0.0f;
                            mbObjDispSet(panel->objId, FALSE);
                            if (panel->sprId >= 0) {
                                espDispOff(panel->sprId);
                            }
                        }
                        panel->scale = mbCosDeg(90.0f * t);
                        panel->scaleEnd = panel->scaleStart = panel->scale;
                        mbObjRotSetV(panel->objId, &pos);
                        break;
                }
            }
            if (panel->anmMaxTime) {
                if (panel->labelObjId) {
                    mbObjDispSet(panel->labelObjId, FALSE);
                }
                mbObjRotGet(panel->objId, &pos);
                panel->anmTime++;
                if (panel->anmTime < panel->anmMaxTime && panel->anmMaxTime > 0) {
                    anmT = (float)panel->anmTime / panel->anmMaxTime;
                    pos.y = 180.0f * anmT;
                } else {
                    panel->anmTime = panel->anmMaxTime = 0;
                    Hu3DAnmNoSet(panel->animId[0], panel->bank);
                    pos.y = 0.0f;
                    if (panel->labelObjId && panel->batsuF) {
                        mbObjDispSet(panel->labelObjId, TRUE);
                    }
                }
                mbObjRotSetV(panel->objId, &pos);
            }
            if (endF) {
                panel->mode = 0;
                mbVecCopy(&panel->endPos, &panel->pos);
                mbVecCopy(&panel->endPos, &panel->startPos);
                panel->scaleStart = panel->scale = panel->scaleEnd;
            } else {
                weight = mbSinDeg(90.0f * t);
                PSVECSubtract(&panel->endPos, &panel->startPos, &pos);
                PSVECScale(&pos, &pos, weight);
                PSVECAdd(&panel->startPos, &pos, &panel->pos);
                panel->scale = panel->scaleStart + weight * (panel->scaleEnd - panel->scaleStart);
            }
            priF = FALSE;
            if (panel->scaleEnd >= 1.2f) {
                priF = TRUE;
            }
            mbNormPosto3D(&panel->pos, 4, &pos);
            mbObjPosSetV(panel->objId, &pos);
            if (panel->labelObjId) {
                mbObjPosSetV(panel->labelObjId, &pos);
            }
            if (panel->sprId >= 0) {
                pos.y -= 100.0f * (0.3f * (panel->scale * panel->baseScale));
                mbPos3DtoNorm(&pos, 4, &pos);
                mbNormPosto2D(&pos, &pos);
                espPosSet(panel->sprId, pos.x, pos.y);
                if (priF) {
                    espPriSet(panel->sprId, 99);
                } else {
                    espPriSet(panel->sprId, 100);
                }
            }
            scale = panel->scale * panel->baseScale;
            mbObjScaleSet(panel->objId, 0.75f * scale, 0.75f * scale, 0.75f * scale);
            if (panel->labelObjId) {
                mbObjScaleSet(panel->labelObjId, 0.8f * (0.75f * scale), 0.8f * (0.75f * scale), 0.75f * scale);
            }
            if (panel->sprId >= 0) {
                sprScale = 0.85f * scale;
                if (panel->anmMaxTime) {
                    t = (float)panel->anmTime / panel->anmMaxTime;
                    weight = PauseFabs(mbCosDeg(180.0f * t));
                    espScaleSet(panel->sprId, sprScale, sprScale * weight);
                    if (t >= 0.5f) {
                        espBankSet(panel->sprId, panel->bank);
                    }
                    weight = 255.0f * weight;
                    espColorSet(panel->sprId, weight, weight, weight);
                } else {
                    espScaleSet(panel->sprId, sprScale, sprScale);
                    espColorSet(panel->sprId, 255, 255, 255);
                }
            }
            mbObjRotGet(panel->objId, &rot);
            mtxRot(rotMtx, rot.x, rot.y, rot.z);
            PSMTXInverse(rotMtx, invRot);
            mbObjPosGet(panel->objId, &pos);
            MTXLookAt(lookAt, &cameraP->pos, &cameraP->up, &pos);
            lookAt[0][3] = lookAt[1][3] = lookAt[2][3] = 0.0f;
            PSMTXInverse(lookAt, lookAt);
            mbObjMtxSet(panel->labelObjId, lookAt);
            PSMTXConcat(invRot, lookAt, lookAt);
            PSMTXConcat(lookAt, rotMtx, lookAt);
            mbObjMtxSet(panel->objId, lookAt);
        }
        HuPrcVSleep();
    }
    HuPrcEnd();
}

static void PauseGuideDestroy(void)
{
    int i;
    void *work;

    if (pausePanelWork) {
        for (i = 0; i < PAUSE_PANEL_MAX; i++) {
        }
        work = pausePanelWork;
        HuMemDirectFree(work);
        pausePanelWork = NULL;
    }
    pauseGuideProc = NULL;
}

static inline void PausePanelAttrLoopSet(MBOBJID objId)
{
    mbObjAttrSet(objId, 0x40000001);
}

static inline void PausePanelAttrSet(MBOBJID objId)
{
    mbObjAttrSet(objId, 0x200000);
}

s16 mbPausePanelCreate(int dataNum, u32 sprDataNum)
{
    int j;
    int i;
    PAUSE_PANEL *panel;

    for (i = 1; i < PAUSE_PANEL_MAX; i++) {
        if (pausePanelWork[i].objId <= 0) {
            break;
        }
    }
    panel = &pausePanelWork[i];
    memset(panel, 0, sizeof(PAUSE_PANEL));
    panel->scale = panel->scaleStart = panel->scaleEnd = panel->baseScale = 1.0f;
    panel->pos.z = panel->startPos.z = panel->endPos.z = -500.0f;
    panel->objId = mbObjCreate(mbBoardDataNumGet(0x80025), NULL, FALSE);
    mbObjCameraSet(panel->objId, 4);
    mbObjLayerSet(panel->objId, 4);
    PausePanelAttrLoopSet(panel->objId);
    PausePanelAttrSet(panel->objId);
    mbObjDispSet(panel->objId, FALSE);
    panel->anim = HuSprAnimDataRead(mbBoardDataNumGet(dataNum));
    for (j = 0; j < 2; j++) {
        if (j == 0) {
            panel->animId[0] = Hu3DAnimCreate(panel->anim, mbObjModelIDGet(panel->objId), "S3TCys77120");
        } else {
            panel->animId[j] = Hu3DAnimLink(panel->animId[0], mbObjModelIDGet(panel->objId), "S3TCys77121");
        }
        Hu3DAnmNoSet(panel->animId[j], 0);
    }
    panel->labelObjId = mbObjCreate(mbBoardDataNumGet(0x80024), NULL, TRUE);
    mbObjCameraSet(panel->labelObjId, 4);
    mbObjLayerSet(panel->labelObjId, 4);
    PausePanelAttrSet(panel->labelObjId);
    mbObjDispSet(panel->labelObjId, FALSE);
    panel->sprId = -1;
    if (sprDataNum) {
        panel->sprId = espEntry(mbBoardDataNumGet(sprDataNum), 100, 0);
        espAttrSet(panel->sprId, 8);
        espDispOff(panel->sprId);
    }
    return i;
}

void mbPausePanelKill(s16 panelId)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    int i;

    for (i = 0; i < 2; i++) {
        if (panel->animId[i] >= 0) {
            Hu3DAnimKill(panel->animId[i]);
        }
        panel->animId[i] = -1;
    }
    if (panel->anim) {
        HuSprAnimKill(panel->anim);
    }
    panel->anim = NULL;
    if (panel->objId) {
        mbObjKill(panel->objId);
        panel->objId = 0;
    }
    if (panel->labelObjId) {
        mbObjKill(panel->labelObjId);
        panel->labelObjId = 0;
    }
    if (panel->sprId >= 0) {
        espKill(panel->sprId);
        panel->sprId = -1;
    }
}

void mbPausePanelPosSet(s16 panelId, float posX, float posY)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    panel->pos.x = panel->startPos.x = panel->endPos.x = posX;
    panel->pos.y = panel->startPos.y = panel->endPos.y = posY;
}

void mbPausePanelPosGet(s16 panelId, HuVecF *pos)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    pos->x = panel->pos.x;
    pos->y = panel->pos.y;
    pos->z = 0.0f;
}

void mbPausePanelRotSet(s16 panelId, float rotX, float rotY, float rotZ)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    mbObjRotSet(panel->objId, rotX, rotY, rotZ);
}

void mbPausePanelScaleSet(s16 panelId, float scale)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    panel->scale = panel->scaleStart = panel->scaleEnd = scale;
}

float mbPausePanelScaleGet(s16 panelId)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    return panel->scale;
}

void mbPausePanelBankSet(s16 panelId, int bank)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    panel->bank = bank;
    Hu3DAnmNoSet(panel->animId[0], bank);
    Hu3DAnmNoSet(panel->animId[1], bank);
    if (panel->sprId >= 0) {
        espBankSet(panel->sprId, bank);
    }
}

void mbPausePanelBatsuSet(s16 panelId, BOOL batsuF)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    panel->batsuF = batsuF;
}

BOOL mbPausePanelFreezeGet(s16 panelId)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    BOOL result = FALSE;
    if (panel->mode == 0 && panel->anmMaxTime == 0) {
        result = TRUE;
    }
    return result;
}

void mbPausePanelUnlockSet(s16 panelId)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    panel->mode = 0;
    mbVecCopy(&panel->pos, &panel->startPos);
    mbVecCopy(&panel->pos, &panel->endPos);
    panel->scaleEnd = panel->scaleStart = panel->scale;
    panel->time = panel->maxTime = 0;
}

void mbPausePanelSlideSet(s16 panelId, int time, HuVecF *pos)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    panel->mode = 1;
    panel->maxTime = time;
    panel->time = 0;
    mbVecCopy(pos, &panel->endPos);
    panel->endPos.z = -500.0f;
    mbVecCopy(&panel->pos, &panel->startPos);
}

void mbPausePanelSizeSet(s16 panelId, int time, float scale)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    panel->mode = 2;
    panel->maxTime = time;
    panel->time = 0;
    panel->scaleEnd = scale;
    panel->scaleStart = panel->scale;
}

void mbPausePanelAnmNoSet(s16 panelId, int time, int bank)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    panel->anmMaxTime = time;
    panel->anmTime = 0;
    Hu3DAnmNoSet(panel->animId[0], panel->bank);
    panel->bank = bank;
    Hu3DAnmNoSet(panel->animId[1], bank);
    panel->sprId >= 0;
}

void mbPausePanelGrowSet(s16 panelId, int time, int delay, float scale)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    panel->mode = 4;
    panel->maxTime = time;
    panel->delay = delay + 1;
    panel->time = 0;
    panel->scaleEnd = 1.0f;
    panel->scaleStart = panel->scale = 0.00001f;
    panel->baseScale = scale;
    mbObjDispSet(panel->objId, FALSE);
    if (panel->labelObjId) {
        mbObjDispSet(panel->labelObjId, FALSE);
    }
}

void mbPausePanelShrinkSet(s16 panelId, int time, int delay)
{
    PAUSE_PANEL *panel = &pausePanelWork[panelId];
    panel->mode = 5;
    panel->maxTime = time;
    panel->delay = delay + 1;
    panel->time = 0;
}

void mbPauseGuideMoveSet(s16 objId, int time, HuVecF *pos, HuVecF *normPos)
{
    HuVecF end;
    HuVecF start;
    int i;
    float t;
    float dir;

    if (pos == NULL) {
        mbNormPosto3D(normPos, 4, &end);
    } else {
        mbVecCopy(pos, &end);
    }
    mbObjPosGet(objId, &start);
    if (abs((int)(end.x - start.x)) < 200.0f) {
        for (i = 0; i <= time; i++) {
            t = (float)i / time;
            t = mbSinDeg(90.0f * t);
            mbObjPosSet(objId, start.x + t * (end.x - start.x), start.y + t * (end.y - start.y), start.z + t * (end.z - start.z));
            HuPrcVSleep();
        }
    } else {
        time *= 1.5;
        dir = 90.0f;
        if (end.x < start.x) {
            dir = -90.0f;
        }
        mbObjMotionShiftSet(objId, 3, 0.0f, 8.0f, 0x40000001);
        mbObjMotionSpeedSet(objId, 1.2f);
        for (i = 0; i <= time; i++) {
            t = (float)i / time;
            mbObjPosSet(objId, start.x + t * (end.x - start.x), start.y + t * (end.y - start.y), start.z + t * (end.z - start.z));
            t = (3.0f * i) / time;
            if (t > 1.0f) {
                t = 1.0f;
            }
            t = mbSinDeg(90.0f * t);
            mbObjRotSet(objId, 0.0f, dir * t, 0.0f);
            HuPrcVSleep();
        }
        mbObjMotionShiftSet(objId, 1, 0.0f, 12.0f, 0x40000001);
        for (i = 0; i <= 12; i++) {
            t = i / 12.0f;
            if (t > 1.0f) {
                t = 1.0f;
            }
            t = 1.0f - mbSinDeg(90.0f * t);
            mbObjRotSet(objId, 0.0f, dir * t, 0.0f);
            HuPrcVSleep();
        }
        mbObjRotSet(objId, 0.0f, 0.0f, 0.0f);
    }
}

void mbPauseGuideTalkSet(s16 objId)
{
    if (pauseWork.talkTime == 0 && pauseWork.talkTimePrev > 0) {
        pauseWork.talkTime = 1;
    } else if (mbObjMotionShiftIDGet(objId) != -1) {
        return;
    }
    if (pauseWork.talkTimePrev == 0 && pauseWork.talkTime > 0) {
        mbObjMotionShiftSet(objId, 12, 0.0f, 8.0f, 0x40000001);
        mbObjMotionSpeedSet(objId, 2.0f);
    }
    if (pauseWork.talkTime) {
        pauseWork.talkTime--;
        if (pauseWork.talkTime == 0) {
            mbObjMotionShiftSet(objId, 1, 0.0f, 16.0f, 0x40000001);
        }
    }
    pauseWork.talkTimePrev = pauseWork.talkTime;
}

void mbConfigPadDisableSet(BOOL disableF)
{
    configPadDisable = disableF;
}
