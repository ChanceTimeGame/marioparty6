#include "game/board/player.h"
#include "game/board/masu.h"
#include "game/board/exit.h"
#include "game/board/math.h"
#include "game/board/object.h"
#include "game/board/telop.h"
#include "game/frand.h"
#include "game/data.h"
#include "game/sprite.h"
#include "game/charman.h"
#include "game/memory.h"
#include "game/hsfformat.h"
#include "string.h"

/* Part 1 (fork b201) owns the unit's head, data and the functions before mbev_PlayerColMasuAdd.
   This worktree only carries what part 2's functions need. */

typedef struct PlayerColWork_s {
    u8 motStartF : 1;
    u8 resetF : 1;
    u8 snapF : 1;
    u8 noRestF : 1;
    u8 playerNo : 2;
    u8 mode : 2;
    u8 circleF;
    u8 masuId;
    u8 masuIdNext;
    s8 time;
    s8 maxTime;
    float rotYStart;
    float radius;
} PLAYERCOLWORK;

#define PLAYER_COLWORK(playerNo) omObjGetWork(playerWork[playerNo].unk24, PLAYERCOLWORK)

typedef struct PlayerMetalWork_s {
    u8 killF : 1;
    u8 fadeInDoneF : 1;
    u8 fadeOutF : 1;
    u8 effectF : 1;
    u8 playerNo : 2;
    s16 time;
    s16 maxTime;
    s16 meshNo;
    s16 cellNum;
} PLAYERMETALWORK;

typedef struct PlayerBiriQWork_s {
    u8 killF : 1;
    u8 flashDoneF : 1;
    u8 flashF : 1;
    u8 fadeOutF : 1;
    u8 effectF : 1;
    u8 playerNo : 2;
    s16 time;
    s16 maxTime;
    s16 meshNo;
    s16 cellNum;
} PLAYERBIRIQWORK;

typedef struct PlayerParticleData_s {
    s16 time;
    s16 maxTime;
    u8 unk4[4];
    float scaleMax;
    float rotSpeed;
    u8 unk10[0x20 - 0x10];
    int vtxNo;
    u8 unk24[0x2C - 0x24];
    float scale;
    u8 unk30[0x38 - 0x30];
    float zRot;
    HuVecF pos;
    GXColor color;
    s16 animNo;
    u8 unk4E[0x58 - 0x4E];
    u8 dispF;
    u8 unk59[0x5C - 0x59];
} PLAYERPARTICLEDATA;

typedef struct PlayerParticle_s {
    s16 endF;
    u8 unk2[0x1C - 0x2];
    void *work;
    u8 unk20;
    u8 unk21[0x26 - 0x21];
    s16 num;
    u32 initF;
    u8 unk2C[0x34 - 0x2C];
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    u8 unk44[0x54 - 0x44];
    GXColor unk54;
    u8 unk58[0x68 - 0x58];
    PLAYERPARTICLEDATA *data;
} PLAYERPARTICLE;

/* Local prototypes: these differ from effect.h/snpc.h (see the report). */
s16 mbParticleCreate(ANIMDATA *anim, s16 num);
void mbParticleKill(s16 modelId);
void mbParticleHookSet(s16 modelId, void *hook);
void mbObjMetalCreate(s16 objId);
int mbObjMetalKill(s16 objId);
void mbObjMetalTPLvlSet(s16 objId, float tpLvl);
void mbObjMetalColorSet(s16 objId, GXColor shadow, GXColor hilite);
void mbObjBiriQCreate(s16 objId);
int mbObjBiriQKill(s16 objId);
void mbObjBiriQColorSet(s16 objId, int mode, float alpha, GXColor color);

static MBPLAYERWORK playerWork[GW_PLAYER_MAX];
static BOOL playerColSnapF;
static GXColor metalHiliteColor;
static GXColor metalShadowColor;
static GXColor metalDefaultColor[2] = { { 0x80, 0xBE, 0x8C, 0xFF }, { 0x64, 0x32, 0x82, 0xFF } };

static void PlayerColCornerSnap(int playerNo, int masuId, int cornerNo);
static void PlayerColInit(int playerNo, int masuId, int cornerNo);

MBPLAYERWORK *mbPlayerWorkGet(int playerNo)
{
    return &playerWork[playerNo];
}

/* Rough port for compilation (part 1 owns it). */
void mbev_PlayerColMasuAllSet(void *arg0, int arg1)
{
    int *masuIdTbl = arg0;
    int i, j;
    int cornerNo;
    int masuId;
    s8 orderNo;
    u8 circleF;
    HuVecF pos;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(playerWork[i].unk24) {
            if(GwPlayer[i].masuId == 0) {
                continue;
            }
            PLAYER_COLWORK(i)->masuIdNext = GwPlayer[i].masuIdNext;
            if(PLAYER_COLWORK(i)->noRestF) {
                continue;
            }
        }
        if(masuIdTbl[i] >= 0) {
            mbPlayerMasuCornerSet(i, 0);
            PlayerColCornerSnap(i, masuIdTbl[i], 0);
            continue;
        }
        if(playerWork[i].unk24 && !PLAYER_COLWORK(i)->snapF) {
            continue;
        }
        masuId = GwPlayer[i].masuId;
        orderNo = GwPlayer[i].orderNo;
        cornerNo = 0;
        for(j = 0; j < GW_PLAYER_MAX; j++) {
            if(i != j && masuId == masuIdTbl[j]) {
                cornerNo++;
            }
        }
        for(j = 0; j < GW_PLAYER_MAX; j++) {
            if(i != j && masuIdTbl[j] < 0 && masuId == GwPlayer[j].masuId && orderNo > GwPlayer[j].orderNo) {
                cornerNo++;
            }
        }
        if(cornerNo == 0) {
            mbMasuPosGet(masuId, &pos);
        } else {
            mbMasuCornerRotPosGet(masuId, cornerNo - 1, &pos);
        }
        circleF = PLAYER_COLWORK(i)->circleF;
        PLAYER_COLWORK(i)->circleF = FALSE;
        if(arg1) {
            mbPlayerPosSetV(i, &pos);
            PlayerColCornerSnap(i, masuId, cornerNo);
        } else if(cornerNo != mbPlayerMasuCornerGet(i) || circleF) {
            PlayerColInit(i, masuId, cornerNo);
        }
        mbPlayerMasuCornerSet(i, cornerNo);
    }
}

/* ---- part 2 ---- */

void mbev_PlayerColMasuAdd(int playerNo, int masuId, BOOL snapF)
{
    int i;
    int cornerNo;
    int j;
    BOOL circleF;
    s8 orderNo;
    HuVecF pos;

    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(masuId != GwPlayer[i].masuId) {
            continue;
        }
        if(i == playerNo) {
            continue;
        }
        if(playerWork[i].unk24) {
            PLAYER_COLWORK(i)->masuIdNext = GwPlayer[i].masuIdNext;
            if(PLAYER_COLWORK(i)->noRestF) {
                continue;
            }
            if(!PLAYER_COLWORK(i)->snapF) {
                continue;
            }
        }
        orderNo = GwPlayer[i].orderNo;
        cornerNo = 1;
        if(playerNo < 0) {
            cornerNo = 0;
        }
        for(j = 0; j < GW_PLAYER_MAX; j++) {
            if(i != j && masuId == GwPlayer[j].masuId && orderNo > GwPlayer[j].orderNo) {
                cornerNo++;
            }
        }
        if(cornerNo != 0) {
            mbMasuCornerRotPosGet(masuId, cornerNo - 1, &pos);
        } else {
            mbMasuPosGet(masuId, &pos);
        }
        circleF = PLAYER_COLWORK(i)->circleF;
        PLAYER_COLWORK(i)->circleF = FALSE;
        if(snapF) {
            mbPlayerPosSetV(i, &pos);
            PlayerColCornerSnap(i, masuId, cornerNo);
        } else if(cornerNo != mbPlayerMasuCornerGet(i) || circleF) {
            PlayerColInit(i, masuId, cornerNo);
        }
        mbPlayerMasuCornerSet(i, cornerNo);
    }
    if(playerNo >= 0) {
        mbPlayerMasuCornerSet(playerNo, 0);
    }
}

void mbev_PlayerColBall(int masuId, int *playerMasuTbl, HuVecF *posOut)
{
    HuVecF posTbl[GW_PLAYER_MAX];
    int orderTbl[GW_PLAYER_MAX];
    int playerNoTbl[GW_PLAYER_MAX];
    int flagTbl[GW_PLAYER_MAX] = { 0 };
    int i;
    int j;
    int num;
    int temp;

    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(playerMasuTbl[i] >= 0) {
            flagTbl[playerMasuTbl[i]] = TRUE;
        }
    }
    num = 0;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(masuId == GwPlayer[i].masuId || flagTbl[i]) {
            orderTbl[num] = GwPlayer[i].orderNo;
            if(orderTbl[num] && !mbPlayerColSnapGet(i)) {
                orderTbl[num] += 4;
            }
            playerNoTbl[num] = i;
            num++;
        }
    }
    for(i = 0; i < num - 1; i++) {
        for(j = i + 1; j < num; j++) {
            if(orderTbl[i] > orderTbl[j]) {
                temp = orderTbl[i];
                orderTbl[i] = orderTbl[j];
                orderTbl[j] = temp;
                temp = playerNoTbl[i];
                playerNoTbl[i] = playerNoTbl[j];
                playerNoTbl[j] = temp;
            }
        }
    }
    for(j = 0; j < num; j++) {
        i = playerNoTbl[j];
        temp = j;
        if(temp != 0) {
            mbMasuCornerRotPosGet(masuId, temp - 1, &posTbl[i]);
        } else {
            mbMasuPosGet(masuId, &posTbl[i]);
        }
    }
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(playerMasuTbl[i] >= 0) {
            mbVecCopy(&posTbl[playerMasuTbl[i]], &posOut[i]);
        }
    }
}

void mbev_PlayerColMasuSet(int playerNo, int masuId, BOOL snapF)
{
    int masuIdTbl[GW_PLAYER_MAX];
    int i;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        masuIdTbl[i] = -1;
    }
    masuIdTbl[playerNo] = masuId;
    mbev_PlayerColMasuAllSet(masuIdTbl, snapF);
}

static void PlayerColCornerSet(int playerNo, int masuIdNext)
{
    int i;
    int masuIdTbl[GW_PLAYER_MAX];
    int j;
    int cornerNo;
    int masuId;
    BOOL circleF;
    s8 orderNo;
    HuVecF pos;

    for(i = 0; i < GW_PLAYER_MAX; i++) {
        masuIdTbl[i] = -1;
    }
    masuIdTbl[playerNo] = masuIdNext;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(playerWork[i].unk24 == NULL) {
            continue;
        }
        if(GwPlayer[i].masuId == 0) {
            continue;
        }
        PLAYER_COLWORK(i)->masuIdNext = GwPlayer[i].masuIdNext;
        if(i == playerNo) {
            continue;
        }
        if(PLAYER_COLWORK(i)->noRestF) {
            continue;
        }
        if(!PLAYER_COLWORK(i)->snapF) {
            continue;
        }
        masuId = GwPlayer[i].masuId;
        orderNo = GwPlayer[i].orderNo;
        cornerNo = 0;
        for(j = 0; j < GW_PLAYER_MAX; j++) {
            if(i != j && masuId == masuIdTbl[j]) {
                cornerNo++;
            }
        }
        for(j = 0; j < GW_PLAYER_MAX; j++) {
            if(i != j && masuIdTbl[j] < 0 && masuId == GwPlayer[j].masuId && orderNo > GwPlayer[j].orderNo) {
                cornerNo++;
            }
        }
        if(cornerNo == 0) {
            mbMasuPosGet(masuId, &pos);
        } else {
            mbMasuCornerRotPosGet(masuId, cornerNo - 1, &pos);
        }
        circleF = PLAYER_COLWORK(i)->circleF;
        PLAYER_COLWORK(i)->circleF = FALSE;
        if(cornerNo != mbPlayerMasuCornerGet(i) || circleF) {
            PlayerColInit(i, masuId, cornerNo);
        }
        mbPlayerMasuCornerSet(i, cornerNo);
    }
}

static void PlayerColCornerSnap(int playerNo, int masuId, int cornerNo)
{
    MBPLAYERWORK *player = mbPlayerWorkGet(playerNo);
    PLAYERCOLWORK *work = omObjGetWork(player->unk24, PLAYERCOLWORK);
    Mtx masuMtx;
    Mtx masuMtxInv;
    HuVecF posPlayer;
    if(work->snapF) {
        work->motStartF = FALSE;
        work->mode = 0;
        work->resetF = FALSE;
        work->masuId = masuId;
        player->masuCorner = cornerNo;
        mbMasuMtxGet(work->masuId, masuMtx);
        MTXInverse(masuMtx, masuMtxInv);
        mbPlayerPosGet(playerNo, &posPlayer);
        MTXMultVec(masuMtxInv, &posPlayer, &playerWork[playerNo].unk3C);
        MTXMultVec(masuMtxInv, &posPlayer, &playerWork[playerNo].unk24->trans);
        mbPlayerRotYSet(playerNo, 0.0f);
        work->rotYStart = 0.0f;
        GwPlayer[playerNo].moveF = FALSE;
    }
}

static void PlayerColInit(int playerNo, int masuId, int cornerNo)
{
    MBPLAYERWORK *player = mbPlayerWorkGet(playerNo);
    PLAYERCOLWORK *work = omObjGetWork(player->unk24, PLAYERCOLWORK);
    Mtx masuMtx;
    Mtx masuMtxInv;
    HuVecF posPlayer;
    HuVecF posMasu;
    HuVecF dir;
    float scale;
    float rotY;
    if(work->snapF) {
        work->motStartF = FALSE;
        work->mode = 1;
        work->resetF = FALSE;
        work->masuId = masuId;
        player->masuCorner = cornerNo;
        mbMasuMtxGet(work->masuId, masuMtx);
        MTXInverse(masuMtx, masuMtxInv);
        mbPlayerPosGet(playerNo, &posPlayer);
        MTXMultVec(masuMtxInv, &posPlayer, &playerWork[playerNo].unk24->trans);
        GwPlayer[playerNo].moveF = TRUE;
        if(player->masuCorner == 0) {
            mbMasuPosGet(work->masuId, &posMasu);
        } else {
            mbMasuCornerRotPosGet(work->masuId, player->masuCorner - 1, &posMasu);
            if(work->circleF) {
                mbMasuPosGet(work->masuId, &dir);
                VECSubtract(&posMasu, &dir, &posMasu);
                scale = work->radius / VECMag(&posMasu);
                VECScale(&posMasu, &posMasu, scale);
                VECAdd(&posMasu, &dir, &posMasu);
            }
        }
        VECSubtract(&posMasu, &posPlayer, &dir);
        rotY = 90 - HuAtan(dir.z, dir.x);
        mbPlayerRotYSet(playerNo, rotY);
        work->rotYStart = rotY;
    }
}

int mbPlayerColCheck(void)
{
    int i;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        PLAYERCOLWORK *work = PLAYER_COLWORK(i);
        if(work->mode != 0) {
            return FALSE;
        }
    }
    return TRUE;
}

static void PlayerColKill(int playerNo)
{
}

void mbev_PlayerColReserve(int playerNo, int masuId, BOOL snapF)
{
    int masuIdTbl[GW_PLAYER_MAX];
    int i;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        masuIdTbl[i] = -1;
    }
    masuIdTbl[playerNo] = masuId;
    mbev_PlayerColMasuAllSet(masuIdTbl, snapF);
}

static void PlayerColOMExec(OMOBJ *obj)
{
    PLAYERCOLWORK *work = omObjGetWork(obj, PLAYERCOLWORK);
    int playerNo = work->playerNo;
    MBPLAYERWORK *player = mbPlayerWorkGet(playerNo);
    Mtx masuMtx;
    Mtx masuMtxInv;
    HuVecF pos;
    HuVecF dir;
    float weight;
    float rotY;

    if(mbExitCheck()) {
        omDelObjEx(HuPrcCurrentGet(), obj);
        player->unk24 = NULL;
        return;
    }
    if(GwPlayer[playerNo].masuId == 0) {
        return;
    }
    if(work->masuIdNext != GwPlayer[playerNo].masuIdNext) {
        work->masuIdNext = GwPlayer[playerNo].masuIdNext;
        PlayerColCornerSet(playerNo, work->masuIdNext);
    }
    if(!work->snapF || player->unk18 || player->unk1C) {
        work->resetF = TRUE;
        work->mode = 0;
        return;
    }
    if(work->resetF) {
        work->resetF = FALSE;
        work->masuId = GwPlayer[playerNo].masuId;
        work->masuIdNext = GwPlayer[playerNo].masuIdNext;
        mbMasuMtxGet(work->masuId, masuMtx);
        MTXInverse(masuMtx, masuMtxInv);
        mbPlayerPosGet(playerNo, &pos);
        MTXMultVec(masuMtxInv, &pos, &player->unk3C);
        MTXMultVec(masuMtxInv, &pos, &obj->trans);
        player->unk3C.y = obj->trans.y = 0.0f;
    }
    switch(work->mode) {
        case 0:
            obj->trans.x = player->unk3C.x;
            obj->trans.y = player->unk3C.y;
            obj->trans.z = player->unk3C.z;
            break;

        case 1:
            if(!work->motStartF) {
                mbPlayerMotionSet(playerNo, 3, HU3D_MOTATTR_LOOP);
                work->motStartF = TRUE;
                work->time = 0;
                work->maxTime = 12;
                obj->rot.x = obj->trans.x;
                obj->rot.y = obj->trans.y;
                obj->rot.z = obj->trans.z;
            }
            if(player->masuCorner == 0) {
                player->unk3C.x = player->unk3C.y = player->unk3C.z = 0.0f;
            } else {
                mbMasuCornerPosGet(work->masuId, player->masuCorner - 1, &player->unk3C);
                if(work->circleF) {
                    weight = work->radius / VECMag(&player->unk3C);
                    VECScale(&player->unk3C, &player->unk3C, weight);
                }
            }
            if(work->time > work->maxTime) {
                work->mode = 2;
                work->time = 0;
                work->maxTime = 8;
                obj->trans.x = player->unk3C.x;
                obj->trans.y = player->unk3C.y;
                obj->trans.z = player->unk3C.z;
                mbPlayerMotionShiftSet(playerNo, 1, 0, 8, HU3D_MOTATTR_LOOP);
            } else {
                weight = (float)(work->time++) / work->maxTime;
                VECSubtract(&player->unk3C, &obj->rot, &dir);
                obj->trans.x = obj->rot.x + (weight * (player->unk3C.x - obj->rot.x));
                obj->trans.y = obj->rot.y + (weight * (player->unk3C.y - obj->rot.y));
                obj->trans.z = obj->rot.z + (weight * (player->unk3C.z - obj->rot.z));
            }
            break;

        case 2:
            if(work->time > work->maxTime) {
                mbPlayerRotYSet(playerNo, 0);
                GwPlayer[playerNo].moveF = FALSE;
                work->mode = 0;
            }
            weight = (float)(work->time++) / work->maxTime;
            rotY = mbAngleEaseOut(work->rotYStart, 0, weight);
            mbPlayerRotYSet(playerNo, rotY);
            break;
    }
    mbMasuMtxGet(work->masuId, masuMtx);
    MTXMultVec(masuMtx, &obj->trans, &pos);
    mbPlayerPosSetV(playerNo, &pos);
}

void mbev_PlayerColSet(int playerNo, int masuId)
{
    int i;
    int cornerNo;
    HuVecF pos;
    if(playerColSnapF) {
        for(i = 0; i < GW_PLAYER_MAX; i++) {
            if(GwPlayer[i].moveF) {
                continue;
            }
            if(masuId != GwPlayer[i].masuId) {
                continue;
            }
            cornerNo = mbPlayerMasuCornerGet(i);
            if(cornerNo == 0) {
                mbMasuPosGet(masuId, &pos);
            } else {
                mbMasuCornerRotPosGet(masuId, cornerNo - 1, &pos);
            }
            mbPlayerPosSetV(i, &pos);
            PlayerColCornerSnap(i, masuId, cornerNo);
        }
    }
}

void mbPlayerColSnapSet(BOOL snapF)
{
    int i;
    BOOL flag = (snapF) ? TRUE : FALSE;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(playerWork[i].unk24) {
            PLAYERCOLWORK *work = PLAYER_COLWORK(i);
            work->snapF = flag;
        }
    }
    playerColSnapF = snapF;
}

void mbPlayerColSnapPlayerSet(int playerNo, BOOL snapF)
{
    BOOL flag = (snapF) ? TRUE : FALSE;
    if(playerWork[playerNo].unk24) {
        PLAYERCOLWORK *work = PLAYER_COLWORK(playerNo);
        work->snapF = flag;
    }
}

int mbPlayerColSnapGet(int playerNo)
{
    PLAYERCOLWORK *work = PLAYER_COLWORK(playerNo);
    return work->snapF;
}

void mbPlayerColRestSet(int playerNo, BOOL restF)
{
    BOOL flag = (restF) ? FALSE : TRUE;
    if(playerWork[playerNo].unk24) {
        PLAYERCOLWORK *work = PLAYER_COLWORK(playerNo);
        work->noRestF = flag;
    }
}

void mbPlayerColFirstSet(int playerNo)
{
    int i;
    int orderNo = 1;
    GwPlayer[playerNo].orderNo = 0;
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        if(playerNo != i) {
            GwPlayer[i].orderNo = orderNo++;
        }
    }
}

void mbPlayerColOrderReset(void)
{
    s8 playerNo[GW_PLAYER_MAX];
    s8 orderNo[GW_PLAYER_MAX];
    s8 fixF[GW_PLAYER_MAX];
    int i, j, k;
    int num;
    s16 masuId;

    memset(fixF, 0, GW_PLAYER_MAX);
    for(i = 0; i < GW_PLAYER_MAX; i++) {
        mbPlayerColRestSet(i, TRUE);
        if(GwPlayer[i].masuId == 0) {
            continue;
        }
        if(fixF[i]) {
            continue;
        }
        masuId = GwPlayer[i].masuId;
        playerNo[0] = i;
        orderNo[0] = GwPlayer[i].orderNo;
        num = 1;
        for(j = 0; j < GW_PLAYER_MAX; j++) {
            if(i != j && masuId == GwPlayer[j].masuId) {
                playerNo[num] = j;
                orderNo[num] = GwPlayer[j].orderNo;
                num++;
            }
        }
        if(num > 1) {
            for(j = 0; j < num - 1; j++) {
                for(k = j + 1; k < num; k++) {
                    if(orderNo[j] > orderNo[k]) {
                        s8 temp = orderNo[j];
                        orderNo[j] = orderNo[k];
                        orderNo[k] = temp;
                    }
                }
            }
        }
        for(j = 0; j < num; j++) {
            GwPlayer[playerNo[j]].orderNo = orderNo[mbPlayerMasuCornerGet(playerNo[j])];
            fixF[playerNo[j]] = TRUE;
        }
    }
}

static char *eyeMatNameTbl[14][2] = {
    "eye1", "eye2",
    "eye1", "eye2",
    "mat14", "mat16",
    "eye1", "eye2",
    "Clswario_eye_l1_AUTO14", "Clswario_eye_l1_AUTO15",
    "m_donkey_eye4", "m_donkey_eye5",
    "mat65", "mat66",
    "Clswaluigi_eye_l1_AUTO1", "Clswaluigi_eye_l1_AUTO2"
};

void mbPlayerEyeMatDarkSet(int playerNo, BOOL darkF)
{
    BOOL validF;
    HU3D_MODELID modelId = mbObjModelIDGet(mbPlayerObjIDGet(playerNo));
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    HSF_DATA *hsf = modelP->hsf;
    HSF_MATERIAL *matP = hsf->material;
    HSF_MATERIAL *matCopy = playerWork[playerNo].unk4C;
    if(darkF) {
        char **name = &eyeMatNameTbl[GwPlayer[playerNo].charNo][0];
        int i, j;
        for(i = 0; i < hsf->materialNum; i++, matP++, matCopy++) {
            validF = TRUE;
            for(j = 0; j < matP->attrNum; j++) {
                HSF_ATTRIBUTE *attrP = &hsf->attribute[matP->attr[j]];
                if(strcmp(name[0], attrP->bitmap->name) == 0 || strcmp(name[1], attrP->bitmap->name) == 0) {
                    validF = FALSE;
                }
            }
            if(validF) {
                if(darkF) {
                    matP->color[0] *= 0.0f;
                    matP->color[1] *= 0.0f;
                    matP->color[2] *= 0.0f;
                } else {
                    matP->color[0] = matCopy->color[0];
                    matP->color[1] = matCopy->color[1];
                    matP->color[2] = matCopy->color[2];
                }
            }
        }
    } else {
        memcpy(hsf->material, matCopy, hsf->materialNum * sizeof(HSF_MATERIAL));
    }
    DCStoreRange(hsf->material, hsf->materialNum * sizeof(HSF_MATERIAL));
}

static inline void *PlayerMalloc(s32 size)
{
    return HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
}

void mbPlayerMatClone(int playerNo)
{
    HU3D_MODELID modelId = mbObjModelIDGet(mbPlayerObjIDGet(playerNo));
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    HSF_DATA *hsf = modelP->hsf;
    HSF_MATERIAL *matP = PlayerMalloc(hsf->materialNum * sizeof(HSF_MATERIAL));
    memcpy(matP, hsf->material, hsf->materialNum * sizeof(HSF_MATERIAL));
    playerWork[playerNo].unk4C = matP;
}

static void PlayerMetalKill(int playerNo);
static void MetalEffectCreate(OMOBJ *obj);
static void MetalEffectHook(HU3D_MODEL *modelP, PLAYERPARTICLE *particleP, Mtx mtx);
static void PlayerBiriQEffectSet(int playerNo, BOOL effectF);
static void PlayerBiriQFlashSet(int playerNo);
static void PlayerBiriQKill(int playerNo);
static void BiriQEffectCreate(OMOBJ *obj);
static void BiriQEffect1Hook(HU3D_MODEL *modelP, PLAYERPARTICLE *particleP, Mtx mtx);
static void BiriQEffect2Hook(HU3D_MODEL *modelP, PLAYERPARTICLE *particleP, Mtx mtx);

static inline void PlayerFree(void *ptr)
{
    HuMemDirectFree(ptr);
}

static void PlayerMetalOMExec(OMOBJ *obj)
{
    PLAYERMETALWORK *work = omObjGetWork(obj, PLAYERMETALWORK);
    BOOL killF = FALSE;
    float weight;
    if(mbExitCheck() || work->killF) {
        if(work->killF) {
            killF = TRUE;
        }
        work->killF = TRUE;
    }
    if(!work->killF) {
        if(!GwPlayer[work->playerNo].metalF && !work->fadeOutF) {
            work->fadeOutF = TRUE;
            work->time = 0;
            work->maxTime = 20;
        }
        if(work->fadeOutF) {
            work->time++;
            weight = (float)work->time / work->maxTime;
            if(weight > 1.0f) {
                weight = 1.0f;
            }
            mbObjMetalTPLvlSet(mbPlayerObjIDGet(work->playerNo), 1.0f - weight);
            if(work->time >= work->maxTime) {
                work->killF = TRUE;
            }
        } else if(!work->fadeInDoneF) {
            work->time++;
            weight = (float)work->time / work->maxTime;
            if(weight > 1.0f) {
                weight = 1.0f;
            }
            mbObjMetalTPLvlSet(mbPlayerObjIDGet(work->playerNo), weight);
            if(work->time >= work->maxTime) {
                work->fadeInDoneF = TRUE;
            }
        }
        if(obj->mdlId[0] >= 0) {
            if(!mbObjGet(mbPlayerObjIDGet(work->playerNo))->disp || !work->effectF) {
                Hu3DModelAttrSet(obj->mdlId[0], HU3D_ATTR_DISPOFF);
            } else {
                Hu3DModelAttrReset(obj->mdlId[0], HU3D_ATTR_DISPOFF);
            }
        }
    }
    if(work->killF) {
        if(!killF) {
            PlayerMetalKill(work->playerNo);
        }
        if(obj->data) {
            PlayerFree(obj->data);
        }
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
}

void mbPlayerMetalSet(int playerNo, BOOL metalF)
{
    OMOBJ *obj;
    PLAYERMETALWORK *work;
    if(metalF) {
        if(playerWork[playerNo].unk28 != NULL) {
            OSReport("------------already METAL!!----------");
        }
        GwPlayer[playerNo].metalF = TRUE;
        obj = playerWork[playerNo].unk28;
        if(obj == NULL) {
            obj = playerWork[playerNo].unk28 = omAddObjEx(mbObjMan, 256, 1, 0, -1, PlayerMetalOMExec);
            omSetStatBit(obj, 0x100);
            mbObjMetalCreate(mbPlayerObjIDGet(playerNo));
            mbObjMetalTPLvlSet(mbPlayerObjIDGet(playerNo), 0);
            mbObjMetalColorSet(mbPlayerObjIDGet(playerNo), metalShadowColor, metalHiliteColor);
        }
        work = omObjGetWork(obj, PLAYERMETALWORK);
        work->playerNo = playerNo;
        work->fadeOutF = FALSE;
        work->fadeInDoneF = FALSE;
        work->effectF = TRUE;
        work->time = 0;
        work->maxTime = 20;
        if(obj->mdlId[0] <= 0) {
            MetalEffectCreate(obj);
        }
        CharModelStepSet(GwPlayer[playerNo].charNo, 6);
    } else {
        GwPlayer[playerNo].metalF = FALSE;
        CharModelStepSet(GwPlayer[playerNo].charNo, 0);
    }
}

static void PlayerMetalKill(int playerNo)
{
    OMOBJ *obj = playerWork[playerNo].unk28;
    if(obj != NULL) {
        PLAYERMETALWORK *work = omObjGetWork(obj, PLAYERMETALWORK);
        work->killF = TRUE;
        playerWork[playerNo].unk28 = NULL;
        mbObjMetalKill(mbPlayerObjIDGet(playerNo));
        mbParticleKill(obj->mdlId[0]);
        obj->mdlId[0] = -1;
    }
}

void mbPlayerEffectSet(int playerNo, BOOL effectF)
{
    OMOBJ *obj = playerWork[playerNo].unk28;
    if(obj) {
        PLAYERMETALWORK *work = omObjGetWork(obj, PLAYERMETALWORK);
        work->effectF = effectF;
    }
    PlayerBiriQEffectSet(playerNo, effectF);
}

static void ResetMetalColor(void)
{
    metalShadowColor = metalDefaultColor[0];
    metalHiliteColor = metalDefaultColor[1];
}

void mbPlayerMetalColorSet(GXColor *shadow, GXColor *hilite)
{
    metalShadowColor = *shadow;
    metalHiliteColor = *hilite;
}

#ifdef __MWERKS__
/* Paired-single element-wise HuVecF multiply. */
static inline void PlayerVecMul(register HuVecF *a, register HuVecF *b, register HuVecF *dst)
{
    register float axy;
    register float bxy;
    register float az;
    register float bz;
    asm {
        psq_l axy, 0(a), 0, 0
        psq_l bxy, 0(b), 0, 0
        psq_l az, 8(a), 1, 0
        psq_l bz, 8(b), 1, 0
        ps_mul axy, axy, bxy
        ps_mul az, az, bz
        psq_st axy, 0(dst), 0, 0
        psq_st az, 8(dst), 1, 0
    }
}
#endif

static float GetBiriQEffectRadius(OMOBJ *obj, int playerNo, int *out)
{
    int i;
    HSF_OBJECT *meshP;
    s16 *cntTbl;
    int cnt;
    int x, y, z;
    HSF_DATA *hsf;
    int vtxNum;
    int no;
    s16 *idxTbl;
    int meshNo;
    int unused;
    int vtxMax;
    HSF_OBJECT *objP;
    HuVecF *vtxP;
    int rand;
    s16 *buf;
    HuVecF scale;
    HuVecF ofs;
    HuVecF pos;
    float radius;

    meshNo = meshNo = -1;
    vtxMax = unused = 0;
    hsf = Hu3DData[mbPlayerModelIDGet(playerNo)].hsf;
    objP = hsf->object;
    for(i = 0; i < hsf->objectNum; i++, objP++) {
        meshP = objP;
        if(meshP->type == 2 && meshP->mesh.vertex->count > 0) {
            vtxNum = meshP->mesh.vertex->count;
            if(vtxMax < vtxNum) {
                vtxMax = vtxNum;
                meshNo = i;
            }
        }
    }
    vtxNum = vtxMax;
    out[0] = meshNo;
    obj->data = PlayerMalloc(2500);
    buf = obj->data;
    cntTbl = buf + 125;
    idxTbl = cntTbl + 125;
    memset(cntTbl, 0, 250);
    meshP = &hsf->object[out[0]];
    VECScale(&meshP->mesh.mesh.min, &ofs, -1.0f);
    VECSubtract(&meshP->mesh.mesh.max, &meshP->mesh.mesh.min, &scale);
    scale.x += 1.0f;
    scale.y += 1.0f;
    scale.z += 1.0f;
    if(scale.x > 0.0f) {
        scale.x = 5.0f / scale.x;
    }
    if(scale.y > 0.0f) {
        scale.y = 5.0f / scale.y;
    }
    if(scale.z > 0.0f) {
        scale.z = 5.0f / scale.z;
    }
    vtxNum = meshP->mesh.vertex->count;
    vtxP = meshP->mesh.vertex->data;
    for(i = 0; i < vtxNum; i++, vtxP++) {
        VECAdd(vtxP, &ofs, &pos);
        PlayerVecMul(&pos, &scale, &pos);
        x = pos.x;
        if(x < 0) {
            x += 5;
        }
        if(x >= 5) {
            x -= 5;
        }
        y = pos.y;
        if(y < 0) {
            y += 5;
        }
        if(y >= 5) {
            y -= 5;
        }
        z = pos.z;
        if(z < 0) {
            z += 5;
        }
        if(z >= 5) {
            z -= 5;
        }
        no = x + y * 5 + z * 25; //IDX
        cnt = cntTbl[no];
        if(cnt < 8) {
            idxTbl[(no * 8) + cnt] = i;
        } else {
            rand = mbRandMod(cnt + 1);
            if(rand < 8) {
                idxTbl[(no * 8) + rand] = i;
            }
        }
        cntTbl[no]++;
    }
    for(i = 0, cnt = 0; i < 125; i++) {
        if(cntTbl[i] > 0) {
            buf[cnt++] = i;
            if(cntTbl[i] > 8) {
                cntTbl[i] = 8;
            }
        }
    }
    out[1] = cnt;
    radius = meshP->mesh.mesh.max.y - meshP->mesh.mesh.min.y;
    return radius;
}

static inline PLAYERPARTICLE *PlayerParticleGet(s16 modelId)
{
    return Hu3DData[modelId].hookData;
}

static void MetalEffectCreate(OMOBJ *obj)
{
    int i;
    HSF_OBJECT *meshP;
    PLAYERMETALWORK *work = omObjGetWork(obj, PLAYERMETALWORK);
    s16 *cntTbl;
    int cnt;
    int x, y, z;
    HSF_DATA *hsf;
    int vtxNum;
    PLAYERPARTICLE *particleP;
    int meshNo;
    int unused;
    int vtxMax;
    HSF_OBJECT *objP;
    HuVecF *vtxP;
    int num;
    int rand;
    int no;
    s16 *buf;
    s16 *idxTbl;
    HuVecF scale;
    HuVecF ofs;
    HuVecF pos;

    num = 5;
    obj->mdlId[0] = mbParticleCreate(HuSprAnimRead(HuDataReadNum(mbBoardDataNumGet(0x5006A), HU_MEMNUM_OVL)), num);
    mbParticleHookSet(obj->mdlId[0], MetalEffectHook);
    Hu3DModelLayerSet(obj->mdlId[0], 5);
    meshNo = meshNo = -1;
    vtxMax = unused = 0;
    hsf = Hu3DData[mbPlayerModelIDGet(work->playerNo)].hsf;
    objP = hsf->object;
    for(i = 0; i < hsf->objectNum; i++, objP++) {
        meshP = objP;
        if(meshP->type == 2 && meshP->mesh.vertex->count > 0) {
            vtxNum = meshP->mesh.vertex->count;
            if(vtxMax < vtxNum) {
                vtxMax = vtxNum;
                meshNo = i;
            }
        }
    }
    vtxNum = vtxMax;
    work->meshNo = meshNo;
    particleP = PlayerParticleGet(obj->mdlId[0]);
    particleP->work = obj;
    for(i = 0; i < num; i++) {
        particleP->data[i].vtxNo = 0;
    }
    obj->data = PlayerMalloc(2500);
    buf = obj->data;
    cntTbl = buf + 125;
    idxTbl = cntTbl + 125;
    memset(cntTbl, 0, 250);
    meshP = &hsf->object[work->meshNo];
    VECScale(&meshP->mesh.mesh.min, &ofs, -1.0f);
    VECSubtract(&meshP->mesh.mesh.max, &meshP->mesh.mesh.min, &scale);
    scale.x += 1.0f;
    scale.y += 1.0f;
    scale.z += 1.0f;
    if(scale.x > 0.0f) {
        scale.x = 5.0f / scale.x;
    }
    if(scale.y > 0.0f) {
        scale.y = 5.0f / scale.y;
    }
    if(scale.z > 0.0f) {
        scale.z = 5.0f / scale.z;
    }
    vtxNum = meshP->mesh.vertex->count;
    vtxP = meshP->mesh.vertex->data;
    for(i = 0; i < vtxNum; i++, vtxP++) {
        VECAdd(vtxP, &ofs, &pos);
        PlayerVecMul(&pos, &scale, &pos);
        x = pos.x;
        if(x < 0) {
            x += 5;
        }
        if(x >= 5) {
            x -= 5;
        }
        y = pos.y;
        if(y < 0) {
            y += 5;
        }
        if(y >= 5) {
            y -= 5;
        }
        z = pos.z;
        if(z < 0) {
            z += 5;
        }
        if(z >= 5) {
            z -= 5;
        }
        no = x + y * 5 + z * 25;
        cnt = cntTbl[no];
        if(cnt < 8) {
            idxTbl[(no * 8) + cnt] = i;
        } else {
            rand = mbRandMod(cnt + 1);
            if(rand < 8) {
                idxTbl[(no * 8) + rand] = i;
            }
        }
        cntTbl[no]++;
    }
    for(i = 0, cnt = 0; i < 125; i++) {
        if(cntTbl[i] > 0) {
            buf[cnt++] = i;
            if(cntTbl[i] > 8) {
                cntTbl[i] = 8;
            }
        }
    }
    work->cellNum = cnt;
}

static void MetalEffectHook(HU3D_MODEL *modelP, PLAYERPARTICLE *particleP, Mtx mtx)
{
    PLAYERPARTICLEDATA *dataP;
    int i;
    PLAYERMETALWORK *work;
    HSF_OBJECT *meshP = NULL;
    int no;
    OMOBJ *obj;
    s16 *buf;
    s16 *cntTbl;
    HSF_DATA *hsf;
    s16 *idxTbl;
    BOOL dispF;
    BOOL initF;
    int rand;
    int unused;
    float weight;
    Mtx objMtx;
    HuVecF dir;
    HuVecF pos;
    HuVecF camPos;

    obj = particleP->work;
    work = omObjGetWork(obj, PLAYERMETALWORK);
    unused = -1;
    initF = FALSE;

    if(!particleP->initF) {
        dataP = particleP->data;
        for(i = 0; i < particleP->num; i++, dataP++) {
            dataP->scale = 0;
            dataP->color.a = 0;
            dataP->time = 0;
        }
        particleP->initF = TRUE;
        particleP->unk34 = 10;
        particleP->unk38 = 2;
        particleP->unk3C = 8;
        particleP->unk40 = 15;
        particleP->unk54.r = particleP->unk54.g = particleP->unk54.b = particleP->unk54.a = 255;
        particleP->unk20 = 1;
        initF = TRUE;
    }
    MTXInverse(Hu3DCameraMtx, objMtx);
    camPos.x = objMtx[0][3];
    camPos.y = objMtx[1][3];
    camPos.z = objMtx[2][3];
    hsf = Hu3DData[mbPlayerModelIDGet(work->playerNo)].hsf;
    meshP = &hsf->object[work->meshNo];
    Hu3DModelObjMtxGet(mbPlayerModelIDGet(work->playerNo), meshP->name, objMtx);
    dataP = particleP->data;
    for(i = 0; i < particleP->num; i++, dataP++) {
        if(dataP->time == 0) {
            if(work->fadeOutF) {
                continue;
            }
            dataP->time++;
            buf = obj->data;
            cntTbl = buf + 125;
            idxTbl = cntTbl + 125;
            no = buf[mbRandMod(work->cellNum)];
            rand = mbRandMod(cntTbl[no]);
            no = idxTbl[(no * 8) + rand];
            dataP->vtxNo = no;
            dataP->color.r = mbRandMod(120) + 120;
            dataP->color.g = mbRandMod(120) + 120;
            dataP->color.b = mbRandMod(120) + 120;
            dataP->color.a = mbRandMod(48) + 100;
            dataP->scaleMax = 30.0f + (60.0f * frandf());
            dataP->scale = 0;
            dataP->rotSpeed = (6.0f * frandf()) - 3.0f;
            dataP->zRot = 360.0f * frandf();
            dataP->maxTime = mbRandMod(30) + 30;
            dataP->animNo = mbRandMod(2);
            dataP->dispF = TRUE;
            if(initF) {
                dataP->scaleMax = 60.0f + (80.0f * frandf());
                dataP->maxTime = mbRandMod(40) + 40;
            }
        }
        dispF = TRUE;
        dataP->time++;
        weight = (float)dataP->time / dataP->maxTime;
        dataP->scale = dataP->scaleMax * mbSinDeg(180.0f * weight);
        dataP->zRot += dataP->rotSpeed;
        if(weight > 0.8f) {
            dataP->color.a *= 0.9f;
        }
        if(weight > 0.6f) {
            dispF = FALSE;
        }
        if(dataP->time >= dataP->maxTime) {
            dataP->time = 0;
            dataP->color.a = 0;
            dataP->scale = 0;
            dataP->dispF = FALSE;
        }
        if(dispF) {
            MTXMultVec(objMtx, &((HuVecF *)meshP->mesh.vertex->data)[dataP->vtxNo], &pos);
            VECSubtract(&camPos, &pos, &dir);
            weight = VECMag(&dir);
            if(weight > 0.0f) {
                VECScale(&dir, &dir, 200.0f / weight);
            }
            VECAdd(&pos, &dir, &dataP->pos);
        }
    }
}

static void PlayerBiriQOMExec(OMOBJ *obj)
{
    PLAYERBIRIQWORK *work = omObjGetWork(obj, PLAYERBIRIQWORK);
    BOOL killF = FALSE;
    BOOL dispF = TRUE;
    if(mbExitCheck() || work->killF) {
        if(work->killF) {
            killF = TRUE;
        }
        work->killF = TRUE;
    }
    if(!work->killF) {
        if(!GwPlayer[work->playerNo].biriQF && !work->fadeOutF) {
            work->fadeOutF = TRUE;
            work->time = 0;
            work->maxTime = 20;
        }
        if(work->fadeOutF) {
            work->killF = TRUE;
        } else if(work->flashF) {
            work->time = 0;
            work->maxTime = 24;
            work->flashF = FALSE;
            work->flashDoneF = FALSE;
        } else if(!work->flashDoneF) {
            GXColor color = { 255, 255, 255, 255 };
            int modeTbl[4] = { 1, 3, 2, 3 };
            float alpha = 1.0f;
            int mode = modeTbl[(work->time >> 1) & 3];
            if(mode == 0) {
                alpha = 0.0f;
            }
            mbObjBiriQColorSet(mbPlayerObjIDGet(work->playerNo), mode, alpha, color);
            if(work->time >= work->maxTime) {
                work->flashDoneF = TRUE;
                mbObjBiriQColorSet(mbPlayerObjIDGet(work->playerNo), 0, 0.0f, color);
                if(obj->mdlId[0] < 0) {
                    BiriQEffectCreate(obj);
                }
            }
            work->time++;
            dispF = FALSE;
        } else {
            GXColor color = { 255, 255, 255, 255 };
            int modeTbl[4] = { 1, 3, 2, 3 };
            float alpha = 0.1f;
            int mode = modeTbl[(work->time >> 1) & 3];
            if(mode == 0) {
                alpha = 0.0f;
            }
            mbObjBiriQColorSet(mbPlayerObjIDGet(work->playerNo), mode, alpha, color);
            work->time++;
        }
        if(obj->mdlId[0] >= 0) {
            if(!mbObjGet(mbPlayerObjIDGet(work->playerNo))->disp || !work->effectF || !dispF) {
                Hu3DModelAttrSet(obj->mdlId[0], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrSet(obj->mdlId[1], HU3D_ATTR_DISPOFF);
            } else {
                Hu3DModelAttrReset(obj->mdlId[0], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrReset(obj->mdlId[1], HU3D_ATTR_DISPOFF);
            }
        }
    }
    if(work->killF) {
        if(!killF) {
            PlayerBiriQKill(work->playerNo);
        }
        if(obj->data) {
            PlayerFree(obj->data);
        }
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
}

void mbPlayerBiriQSet(int playerNo, BOOL biriQF)
{
    OMOBJ *obj;
    PLAYERBIRIQWORK *work;
    if(biriQF) {
        if(playerWork[playerNo].unk2C != NULL) {
            OSReport("------------already BiriQ!!----------");
        }
        GwPlayer[playerNo].biriQF = TRUE;
        obj = playerWork[playerNo].unk2C;
        if(obj == NULL) {
            GXColor color = { 255, 255, 255, 255 };
            obj = playerWork[playerNo].unk2C = omAddObjEx(mbObjMan, 256, 2, 0, -1, PlayerBiriQOMExec);
            omSetStatBit(obj, 0x100);
            obj->mdlId[0] = obj->mdlId[1] = -1;
            mbObjBiriQCreate(mbPlayerObjIDGet(playerNo));
            mbObjBiriQColorSet(mbPlayerObjIDGet(playerNo), 1, 0.0f, color);
        }
        work = omObjGetWork(obj, PLAYERBIRIQWORK);
        work->playerNo = playerNo;
        work->fadeOutF = FALSE;
        work->flashDoneF = FALSE;
        work->time = 0;
        work->maxTime = 20;
        work->effectF = TRUE;
        PlayerBiriQFlashSet(playerNo);
    } else {
        GwPlayer[playerNo].biriQF = FALSE;
    }
}

static void PlayerBiriQFlashSet(int playerNo)
{
    OMOBJ *obj = playerWork[playerNo].unk2C;
    if(obj != NULL) {
        PLAYERBIRIQWORK *work = omObjGetWork(obj, PLAYERBIRIQWORK);
        work->flashF = TRUE;
    }
}

static void PlayerBiriQKill(int playerNo)
{
    OMOBJ *obj = playerWork[playerNo].unk2C;
    if(obj != NULL) {
        PLAYERBIRIQWORK *work = omObjGetWork(obj, PLAYERBIRIQWORK);
        work->killF = TRUE;
        playerWork[playerNo].unk2C = NULL;
        mbObjBiriQKill(mbPlayerObjIDGet(playerNo));
        if(obj->mdlId[0] >= 0) {
            mbParticleKill(obj->mdlId[0]);
            obj->mdlId[0] = -1;
        }
        if(obj->mdlId[1] >= 0) {
            mbParticleKill(obj->mdlId[1]);
            obj->mdlId[1] = -1;
        }
    }
}

static void PlayerBiriQEffectSet(int playerNo, BOOL effectF)
{
    OMOBJ *obj = playerWork[playerNo].unk2C;
    if(obj) {
        PLAYERBIRIQWORK *work = omObjGetWork(obj, PLAYERBIRIQWORK);
        work->effectF = effectF;
    }
}

static void BiriQEffectCreate(OMOBJ *obj)
{
    PLAYERBIRIQWORK *work = omObjGetWork(obj, PLAYERBIRIQWORK);
    int num;
    PLAYERPARTICLE *particleP;
    int out[2];
    float radius = GetBiriQEffectRadius(obj, work->playerNo, out);
    work->meshNo = out[0];
    work->cellNum = out[1];
    num = 21.0f * ((1.0f / 150.0f) * radius);
    obj->mdlId[0] = mbParticleCreate(HuSprAnimRead(HuDataReadNum(mbBoardDataNumGet(0x5006B), HU_MEMNUM_OVL)), num);
    obj->mdlId[1] = mbParticleCreate(HuSprAnimRead(HuDataReadNum(mbBoardDataNumGet(0x5006C), HU_MEMNUM_OVL)), num);
    mbParticleHookSet(obj->mdlId[0], BiriQEffect1Hook);
    mbParticleHookSet(obj->mdlId[1], BiriQEffect2Hook);
    Hu3DModelLayerSet(obj->mdlId[0], 3);
    Hu3DModelLayerSet(obj->mdlId[1], 3);
    particleP = PlayerParticleGet(obj->mdlId[0]);
    particleP->work = obj;
    particleP->endF = 0;
    particleP = PlayerParticleGet(obj->mdlId[1]);
    particleP->work = PlayerParticleGet(obj->mdlId[0]);
    particleP->endF = 0;
}

static void BiriQEffect1Hook(HU3D_MODEL *modelP, PLAYERPARTICLE *particleP, Mtx mtx)
{
    PLAYERPARTICLEDATA *dataP;
    int i;
    PLAYERBIRIQWORK *work;
    HSF_OBJECT *meshP = NULL;
    int no;
    OMOBJ *obj;
    s16 *buf;
    s16 *cntTbl;
    HSF_DATA *hsf;
    s16 *idxTbl;
    BOOL dispF;
    int rand;
    BOOL initF;
    int unused;
    float weight;
    Mtx objMtx;
    HuVecF dir;
    HuVecF pos;
    HuVecF camPos;

    obj = particleP->work;
    work = omObjGetWork(obj, PLAYERBIRIQWORK);
    unused = -1;
    initF = FALSE;

    if(particleP->endF == 0) {
        dataP = particleP->data;
        for(i = 0; i < particleP->num; i++, dataP++) {
            dataP->scale = 0;
            dataP->color.a = 0;
            dataP->time = 0;
        }
        particleP->endF = 1;
        initF = TRUE;
    }
    MTXInverse(Hu3DCameraMtx, objMtx);
    camPos.x = objMtx[0][3];
    camPos.y = objMtx[1][3];
    camPos.z = objMtx[2][3];
    hsf = Hu3DData[mbPlayerModelIDGet(work->playerNo)].hsf;
    meshP = &hsf->object[work->meshNo];
    Hu3DModelObjMtxGet(mbPlayerModelIDGet(work->playerNo), meshP->name, objMtx);
    dataP = particleP->data;
    for(i = 0; i < particleP->num; i++, dataP++) {
        if(dataP->time == 0) {
            if(work->fadeOutF) {
                continue;
            }
            dataP->time++;
            buf = obj->data;
            cntTbl = buf + 125;
            idxTbl = cntTbl + 125;
            no = buf[mbRandMod(work->cellNum)];
            rand = mbRandMod(cntTbl[no]);
            no = idxTbl[(no * 8) + rand];
            dataP->vtxNo = no;
            dataP->color.r = mbRandMod(40) + 30;
            dataP->color.g = mbRandMod(40) + 90;
            dataP->color.b = mbRandMod(40) + 180;
            dataP->color.a = mbRandMod(58) + 130;
            dataP->scaleMax = 30.0f + (60.0f * frandf());
            dataP->scale = 0;
            dataP->zRot = 360.0f * frandf();
            dataP->maxTime = mbRandMod(12) + 12;
            dataP->animNo = mbRandMod(4);
            dataP->dispF = TRUE;
        }
        dispF = TRUE;
        dataP->time++;
        weight = (float)dataP->time / dataP->maxTime;
        dataP->scale = dataP->scaleMax * (0.1f + (0.9f * mbSinDeg(180.0f * weight)));
        if(dataP->time & 1) {
            dataP->animNo = mbRandMod(4);
            if(mbRandMod(100) < 25) {
                dataP->zRot += 180.0f;
            }
        }
        if(weight > 0.8f) {
            dataP->color.a *= 0.9f;
        }
        if(weight > 0.6f) {
            dispF = FALSE;
        }
        if(dataP->time >= dataP->maxTime) {
            dataP->time = 0;
            dataP->color.a = 0;
            dataP->scale = 0;
            dataP->dispF = FALSE;
        }
        if(dispF) {
            MTXMultVec(objMtx, &((HuVecF *)meshP->mesh.vertex->data)[dataP->vtxNo], &pos);
            VECSubtract(&camPos, &pos, &dir);
            weight = VECMag(&dir);
            if(weight > 0.0f) {
                VECScale(&dir, &dir, 18.0f / weight);
            }
            VECAdd(&pos, &dir, &dataP->pos);
        }
    }
}

static void BiriQEffect2Hook(HU3D_MODEL *modelP, PLAYERPARTICLE *particleP, Mtx mtx)
{
    PLAYERPARTICLEDATA *dataP;
    int i;
    int alpha;
    PLAYERPARTICLE *srcP;
    if(particleP->endF == 0) {
        dataP = particleP->data;
        for(i = 0; i < particleP->num; i++, dataP++) {
            dataP->scale = 0;
            dataP->color.a = 0;
            dataP->time = 0;
        }
        particleP->endF = 1;
        particleP->unk20 = 1;
    }
    srcP = particleP->work;
    memcpy(particleP->data, srcP->data, particleP->num * sizeof(PLAYERPARTICLEDATA));
    dataP = particleP->data;
    for(i = 0; i < particleP->num; i++, dataP++) {
        dataP->color.r = 255;
        dataP->color.g = 255;
        dataP->color.b = 255;
        alpha = 1.2f * dataP->color.a;
        if(alpha > 255) {
            alpha = 255;
        }
        dataP->color.a = alpha;
    }
}

void mbPlayerSwap(int playerNo1, int playerNo2)
{
    OMOBJ *colObj1 = mbPlayerWorkGet(playerNo1)->unk24;
    OMOBJ *colObj2 = mbPlayerWorkGet(playerNo2)->unk24;
    GW_PLAYER_CONF tempConf;
    MBPLAYERWORK tempWork;
    GW_PLAYER tempPlayer;
    tempConf = GwPlayerConf[playerNo1];
    GwPlayerConf[playerNo1] = GwPlayerConf[playerNo2];
    GwPlayerConf[playerNo2] = tempConf;
    tempPlayer = GwPlayer[playerNo1];
    GwPlayer[playerNo1] = GwPlayer[playerNo2];
    GwPlayer[playerNo2] = tempPlayer;
    memcpy(&tempWork, mbPlayerWorkGet(playerNo1), sizeof(MBPLAYERWORK));
    memcpy(mbPlayerWorkGet(playerNo1), mbPlayerWorkGet(playerNo2), sizeof(MBPLAYERWORK));
    memcpy(mbPlayerWorkGet(playerNo2), &tempWork, sizeof(MBPLAYERWORK));
    mbPlayerWorkGet(playerNo1)->unk24 = colObj1;
    mbPlayerWorkGet(playerNo2)->unk24 = colObj2;
    GwPlayer[playerNo1].padNo = GwPlayerConf[playerNo1].padNo;
    GwPlayerConf[playerNo1].padNo = GwPlayerConf[playerNo1].padNo;
    GwPlayer[playerNo2].padNo = GwPlayerConf[playerNo2].padNo;
    GwPlayerConf[playerNo2].padNo = GwPlayerConf[playerNo2].padNo;
}

/* ---- part 3 placeholders (fork b203 owns these) ---- */

void mbPlayerPosSetV(int playerNo, HuVecF *pos) { }
void mbPlayerPosGet(int playerNo, HuVecF *pos) { }
void mbPlayerRotYSet(int playerNo, float rotY) { }
void mbPlayerMotionSet(int arg0, int arg1, int arg2) { }
void mbPlayerMotionShiftSet(int arg0, int arg1, float f0, float f1, int arg2) { }
s16 mbPlayerObjIDGet(int arg0) { return 0; }
s16 mbPlayerModelIDGet(int arg0) { return 0; }
void mbPlayerMasuCornerSet(int playerNo, s8 cornerNo) { }
s8 mbPlayerMasuCornerGet(int playerNo) { return 0; }
