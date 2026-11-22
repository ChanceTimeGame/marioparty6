#include "game/board/masu.h"
#include "game/board/object.h"
#include "game/board/camera.h"
#include "game/board/player.h"
#include "game/board/capsule.h"
#include "game/board/capselect.h"
#include "game/board/capevent.h"
#include "game/board/shopevent.h"
#include "game/board/tutorial.h"
#include "game/board/status.h"
#include "game/board/branch.h"
#include "game/board/audio.h"
#include "game/board/gate.h"
#include "game/board/malloc.h"
#include "game/board/math.h"

#include "game/data.h"
#include "game/memory.h"
#include "game/sprite.h"
#include "game/hsfex.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "string.h"

/* star.h and single.h mistype these; the MP6 functions take these arguments. */
void mbev_StarMasu(int playerNo, int masuId);
int mbSingleCall(int no, int masuId);
void mbev_SingleMg(int playerNo, s16 masuId);
void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);

#define MASU_TYPE_NUM 12
#define MASU_NEXT_MAX 4

typedef int (*MASUCHECKFUNC)(int masuId, int arg1, int arg2);

typedef struct MasuNextWork_s {
    int playerNo;
    int mode;
    s16 masuId;
    float rot;
    float scale;
    u8 alpha;
    s16 delay;
    s16 time;
    s16 maxTime;
} MASUNEXTWORK;

typedef struct MasuDispTbl_s {
    int type;
    int dispF;
} MASUDISPTBL;

typedef struct MasuFindWork_s {
    s16 id;
    s16 no;
} MASUFINDWORK;

static u8 masuFindVisit[MASU_MAX];
static s16 masuFindResult[MASU_MAX];
static MASUFINDWORK masuFindWork[MASU_MAX];

static int masuLayer;
static s16 masuNum[MASU_LAYER_MAX];
static MASU *masuData[MASU_LAYER_MAX];
static BOOL masuDispF;
static u16 masuDispAttrMask[MASU_LAYER_MAX];
static u32 masuDispMAttrMask[MASU_LAYER_MAX];
static BOOL masuCapsuleDispF;
static BOOL masuCapsuleFadeOnF;
static MASUEVHOOK masuev_MasuStart;
static MASUEVHOOK masuev_MasuEnd;
static MASUEVHOOK masuev_HatenaHook;
static MASULINKTBLHOOK masuev_LinkTblHook;
static s16 masuMdlId;
static s16 masuDispCnt;
static ANIMDATA *masuAnimTbl[2];
static void *masuDisplayList;
static u32 masuDisplayListLen;
static void *masuDisplayListKao;
static u32 masuDisplayListKaoLen;
static MASUNEXTWORK *masuNextWork;
static ANIMDATA *masuNextAnim;
static s16 masuNextId;
static HUPROCESS *masuNextProc;
static s16 masuNextMdlId;
static BOOL masuNextDispF;
static s16 masuFindNo;
static s16 masuFindStep;
static s16 masuFindId;
static s16 masuFindResultNum;

static int masuFileTbl[2] = { 0x40000, 0x40003 };
static int masuSingleFileTbl[2] = { 0x40001, 0x40003 };

static s16 masuPatTbl[11] = { -1, 0, 1, 2, 6, 7, -2, 5, 8, -1, 9 };
static s16 masuSinglePatTbl[12] = { -1, 0, 1, 2, 6, 7, 3, 5, 8, 9, 10, 11 };

static inline void MasuFree(void *ptr)
{
    HuMemDirectFree(ptr);
}

static inline void *MasuMalloc(u32 size)
{
    return HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
}

static inline BOOL MasuPartyFGet(void)
{
    BOOL partyF = GwSystem.partyF;
    return partyF;
}

static inline BOOL MasuTeamFGet(void)
{
    return GwSystem.tagF;
}

static inline BOOL MasuTimeDayCheck(void)
{
    return GwSystem.curTime == 0;
}

static void MasuDispInit(void);
static void MasuDispClose(void);
static u32 MasuDisplayListMake(void **dl);
static u32 MasuDisplayListKaoMake(void **dl);
static void MasuDraw(HU3D_MODEL *modelP, Mtx *mtx);
static void MasuNextCreate(void);
static void MasuNextMain(void);
static void MasuNextKill(void);
static void MasuNextDraw(HU3D_MODEL *modelP, Mtx *mtx);
static int ev_MasuHatena(int playerNo, s16 masuId);

void mbMasuInit(int dataNum)
{
    int i;
    for(i=0; i<MASU_LAYER_MAX; i++) {
        masuData[i] = mbMalloc(MASU_MAX*sizeof(MASU));
    }
    mbMasuLayerSet(0);
    for(i=0; i<MASU_LAYER_MAX; i++) {
        masuDispAttrMask[i] = 0x10;
        masuDispMAttrMask[i] = 0;
    }
    masuev_MasuStart = NULL;
    masuev_MasuEnd = NULL;
    masuev_HatenaHook = NULL;
    masuev_LinkTblHook = NULL;
    masuDispF = TRUE;
    masuCapsuleDispF = TRUE;
    masuCapsuleFadeOnF = FALSE;
    mbMasuDataRead(mbObjDataNumGet(dataNum));
    MasuDispInit();
    mbGateCreate();
    MasuNextCreate();
    HuDataDirClose(0x40000);
}

void mbMasuClose(void)
{
    int i;
    MasuDispClose();
    MasuNextKill();
    for(i=0; i<MASU_LAYER_MAX; i++) {
        MasuFree(masuData[i]);
        masuData[i] = NULL;
    }
}

BOOL mbMasuDataRead(int dataNum)
{
    MASU *masuP;
    u8 *data;
    int i;
    int j;
    s32 *dataP;
    int starNum = 0;

    dataP = HuDataSelHeapReadNum(dataNum, HU_MEMNUM_OVL, HEAP_MODEL);
    masuNum[masuLayer] = *dataP;
    data = (u8 *)(dataP+1);
    masuP = masuData[masuLayer];
    masuP->pos.x = masuP->pos.y = masuP->pos.z = 0;
    masuP->pos.z = 100000;
    masuP->rot.x = masuP->rot.y = masuP->rot.z = 0;
    masuP->scale.x = masuP->scale.y = masuP->scale.z = 1;
    MTXIdentity(masuP->matrix);
    masuP->linkNum = 0;
    masuP->capsuleNo = -1;
    masuP++;
    for(i=0; i<masuNum[masuLayer]; i++, masuP++) {
        masuP->id = i+1;
        memcpy(&masuP->pos, data, sizeof(HuVecF));
        data += sizeof(HuVecF);
        memcpy(&masuP->rot, data, sizeof(HuVecF));
        data += sizeof(HuVecF);
        memcpy(&masuP->scale, data, sizeof(HuVecF));
        data += sizeof(HuVecF);
        masuP->attr = *(u16 *)data;
        data += sizeof(u16);
        masuP->mAttr = *(u32 *)data;
        data += sizeof(u32);
        masuP->type = *(u16 *)data;
        data += sizeof(u16);
        masuP->capsuleNo = GwSystem.masuCapsule[masuP->id];
        masuP->linkNum = *(u16 *)data;
        data += sizeof(u16);
        for(j=0; j<masuP->linkNum; j++) {
            masuP->linkTbl[j] = (*(u16 *)data)+1;
            data += sizeof(u16);
        }
    }
    HuDataClose(dataP);
    return TRUE;
}

static void MasuDispInit(void)
{
    int i;
    if(GWPartyGet() != FALSE) {
        for(i=0; i<2; i++) {
            masuAnimTbl[i] = HuSprAnimDataRead(masuFileTbl[i]);
            HuSprAnimLock(masuAnimTbl[i]);
        }
    } else {
        for(i=0; i<2; i++) {
            masuAnimTbl[i] = HuSprAnimDataRead(masuSingleFileTbl[i]);
            HuSprAnimLock(masuAnimTbl[i]);
        }
    }
    masuDisplayListLen = MasuDisplayListMake(&masuDisplayList);
    masuDisplayListKaoLen = MasuDisplayListKaoMake(&masuDisplayListKao);
    masuMdlId = Hu3DHookFuncCreate(MasuDraw);
    Hu3DModelCameraSet(masuMdlId, HU3D_CAM0);
    Hu3DModelLayerSet(masuMdlId, 2);
}

static void MasuDispClose(void)
{
    int i;
    for(i=0; i<2; i++) {
        if(masuAnimTbl[i] != NULL) {
            HuSprAnimKill(masuAnimTbl[i]);
            masuAnimTbl[i] = NULL;
        }
    }
    if(masuDisplayList != NULL) {
        MasuFree(masuDisplayList);
        masuDisplayList = NULL;
    }
    if(masuMdlId >= 0) {
        Hu3DModelKill(masuMdlId);
        masuMdlId = HU3D_MODELID_NONE;
    }
}

#define DLBUF_SIZE 0x1000

static u32 MasuDisplayListMake(void **dl)
{
    void *buf;
    u32 len;
    buf = MasuMalloc(DLBUF_SIZE);
    DCInvalidateRange(buf, DLBUF_SIZE);
    GXBeginDisplayList(buf, DLBUF_SIZE);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-100, 0, -100);
    GXTexCoord2f32(0, 0);
    GXPosition3f32(100, 0, -100);
    GXTexCoord2f32(1, 0);
    GXPosition3f32(100, 0, 100);
    GXTexCoord2f32(1, 1);
    GXPosition3f32(-100, 0, 100);
    GXTexCoord2f32(0, 1);
    GXEnd();
    len = GXEndDisplayList();
    *dl = MasuMalloc(len);
    memcpy(*dl, buf, len);
    DCFlushRange(*dl, len);
    MasuFree(buf);
    return len;
}

static u32 MasuDisplayListKaoMake(void **dl)
{
    void *buf;
    u32 len;
    buf = MasuMalloc(DLBUF_SIZE);
    DCInvalidateRange(buf, DLBUF_SIZE);
    GXBeginDisplayList(buf, DLBUF_SIZE);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-100, 200, 0);
    GXTexCoord2f32(0, 0);
    GXPosition3f32(100, 200, 0);
    GXTexCoord2f32(1, 0);
    GXPosition3f32(100, 0, 0);
    GXTexCoord2f32(1, 1);
    GXPosition3f32(-100, 0, 0);
    GXTexCoord2f32(0, 1);
    GXEnd();
    len = GXEndDisplayList();
    *dl = MasuMalloc(len);
    memcpy(*dl, buf, len);
    DCFlushRange(*dl, len);
    MasuFree(buf);
    return len;
}

#undef DLBUF_SIZE

static inline int MasuPlayerTeamGet(int playerNo)
{
    return GwPlayer[playerNo].team;
}

static void MasuDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    Mtx modelview;
    Mtx trans;
    Mtx rot;
    Mtx model;
    Mtx texMtx;
    HuVecF pos;
    HuVecF minPos;
    MASU *masuP;
    int i;
    int patNo;
    int minPlayer;
    int playerNo;
    float minDist;
    float dist;
    float scale;
    GXColor color = { 255, 255, 255, 255 };
    GXColor kaoColor = { 255, 255, 255, 255 };

    if(masuDispF == FALSE) {
        return;
    }
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXInvalidateTexAll();
    HuSprTexLoad(masuAnimTbl[0], 0, GX_TEXMAP0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    HuSprTexLoad(masuAnimTbl[1], 0, GX_TEXMAP1, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    HuSprTexLoad(masuAnimTbl[1], 0, GX_TEXMAP2, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetCullMode(GX_CULL_BACK);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    masuDispCnt = 0;
    masuP = &masuData[masuLayer][1];
    for(i=0; i<masuNum[masuLayer]; i++, masuP++) {
        if(GWPartyGet() != FALSE) {
            patNo = masuPatTbl[masuP->type];
            if(patNo < 0) {
                if(patNo == -1) {
                    continue;
                }
                patNo = (MasuTimeDayCheck()) ? 4 : 3;
            }
        } else {
            patNo = masuSinglePatTbl[masuP->type];
            if(patNo < 0) {
                continue;
            }
        }
        mbMasuPosGet(i+1, &pos);
        if(mbCameraCullCheck(&pos, 200) == FALSE) {
            continue;
        }
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        if(masuP->useMtxF == FALSE) {
            mtxRot(rot, masuP->rot.x, masuP->rot.y, masuP->rot.z);
            MTXTrans(trans, masuP->pos.x, 3+masuP->pos.y, masuP->pos.z);
            MTXConcat(trans, rot, model);
        } else {
            MTXCopy(masuP->matrix, model);
        }
        MTXConcat(*mtx, model, modelview);
        GXLoadPosMtxImm(modelview, GX_PNMTX0);
        if(GWPartyGet() != FALSE && (playerNo = mbCapMasuPlayerGet(i+1)) >= 0 && mbCapMasuDispTypeGet(i+1) == 1) {
            MTXScale(texMtx, 1, 1.0f/13, 0);
            if(!MasuTeamFGet()) {
                mtxTransCat(texMtx, 0, GwPlayer[playerNo].charNo/13.0f, 0);
            } else {
                mtxTransCat(texMtx, 0, (MasuPlayerTeamGet(playerNo)+11)/13.0f, 0);
            }
            GXLoadTexMtxImm(texMtx, GX_TEXMTX0, GX_MTX2x4);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR0A0);
            GXCallDisplayList(masuDisplayList, masuDisplayListLen);
        } else if((masuP->attr & masuDispAttrMask[masuLayer]) == 0 && (masuP->mAttr & masuDispMAttrMask[masuLayer]) == 0) {
            MTXScale(texMtx, 0.25f, 1.0f/3, 0);
            mtxTransCat(texMtx, (patNo%4)/4.0f, (patNo/4)/3.0f, 0);
            GXLoadTexMtxImm(texMtx, GX_TEXMTX0, GX_MTX2x4);
            GXCallDisplayList(masuDisplayList, masuDisplayListLen);
        }
        masuDispCnt++;
    }
    if(!masuCapsuleDispF || GWPartyGet() == FALSE) {
        return;
    }
    masuP = &masuData[masuLayer][1];
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    for(i=0; i<masuNum[masuLayer]; i++, masuP++) {
        playerNo = mbCapMasuPlayerGet(i+1);
        if(playerNo < 0) {
            continue;
        }
        if(mbCapMasuDispTypeGet(i+1) != 2) {
            continue;
        }
        mbMasuPosGet(i+1, &pos);
        if(mbCameraCullCheck(&pos, 200) == FALSE) {
            continue;
        }
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP2, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A1, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        MTXScale(texMtx, 1, 1.0f/13, 0);
        if(!MasuTeamFGet()) {
            mtxTransCat(texMtx, 0, GwPlayer[playerNo].charNo/13.0f, 0);
        } else {
            mtxTransCat(texMtx, 0, (MasuPlayerTeamGet(playerNo)+11)/13.0f, 0);
        }
        GXLoadTexMtxImm(texMtx, GX_TEXMTX0, GX_MTX2x4);
        if(masuP->useMtxF == FALSE) {
            mtxRot(rot, masuP->rot.x, masuP->rot.y, masuP->rot.z);
            MTXTrans(trans, masuP->pos.x, masuP->pos.y, masuP->pos.z);
            MTXConcat(trans, rot, model);
        } else {
            MTXCopy(masuP->matrix, model);
        }
        if(!masuCapsuleFadeOnF) {
            int j;
            minDist = 100000;
            minPlayer = -1;
            for(j=0; j<4; j++) {
                mbPlayerPosGet(j, &pos);
                pos.x = __abs(pos.x-model[0][3]);
                pos.y = __abs(pos.y-model[1][3]);
                pos.z = __abs(pos.z-model[2][3]);
                if(minDist > pos.z+(pos.x+pos.y)) {
                    minDist = pos.z+(pos.x+pos.y);
                    minPos = pos;
                    minPlayer = j;
                }
            }
            kaoColor.a = 224;
            scale = 1;
            if(minPlayer >= 0) {
                dist = minPos.x;
                if(dist < minPos.y) {
                    dist = minPos.y;
                }
                if(dist < minPos.z) {
                    dist = minPos.z;
                }
                if(dist < 250) {
                    dist -= 150;
                    if(dist < 0) {
                        dist = 0;
                    }
                    dist /= 100;
                    kaoColor.a = 224-(64*(1-dist));
                    scale = 1+(0.5f*(1-dist));
                }
            }
        } else {
            kaoColor.a = 224;
            scale = 1;
        }
        GXSetTevColor(GX_TEVREG1, kaoColor);
        MTXScale(modelview, scale, scale, scale);
        MTXConcat(model, modelview, model);
        MTXConcat(*mtx, model, modelview);
        GXLoadPosMtxImm(modelview, GX_PNMTX0);
        GXCallDisplayList(masuDisplayListKao, masuDisplayListKaoLen);
    }
}

static void MasuNextCreate(void)
{
    masuNextProc = HuPrcChildCreate(MasuNextMain, 0x2003, 0x2000, 0, mbMainProc);
    masuNextWork = mbMalloc(MASU_NEXT_MAX*sizeof(MASUNEXTWORK));
    masuNextMdlId = Hu3DHookFuncCreate(MasuNextDraw);
    Hu3DModelCameraSet(masuNextMdlId, HU3D_CAM0);
    Hu3DModelLayerSet(masuNextMdlId, 2);
    masuNextAnim = HuSprAnimDataRead(0x40005);
    masuNextDispF = TRUE;
}

static void MasuNextMain(void)
{
    MASUNEXTWORK *work;
    int i;
    int playerNo;
    float t;
    while(1) {
        playerNo = GwSystem.turnPlayerNo;
        work = masuNextWork;
        for(i=0; i<MASU_NEXT_MAX; i++, work++) {
            if(work->masuId == 0) {
                continue;
            }
            if(work->delay != 0) {
                work->delay--;
                continue;
            }
            if(work->playerNo != playerNo) {
                work->masuId = 0;
                continue;
            }
            switch(work->mode) {
                case 0:
                    t = (float)work->time/work->maxTime;
                    if(work->time < work->maxTime) {
                        work->time++;
                    }
                    work->scale = 1+mbCosDeg((90.0f*work->time)/work->maxTime);
                    work->alpha = 192*t;
                    if(masuNextId != work->masuId) {
                        work->mode = 1;
                        work->time = 0;
                    }
                    break;
                    
                case 1:
                    t = (float)(work->time++)/work->maxTime;
                    work->alpha = 192*(1-t);
                    if(work->time > work->maxTime) {
                        work->masuId = 0;
                    }
                    break;
            }
        }
        HuPrcVSleep();
    }
}

static void MasuNextKill(void)
{
    if(masuNextWork != NULL) {
        MasuFree(masuNextWork);
        masuNextWork = NULL;
    }
    if(masuNextMdlId >= 0) {
        Hu3DModelKill(masuNextMdlId);
        masuNextMdlId = HU3D_MODELID_NONE;
    }
    if(masuNextAnim != NULL) {
        HuSprAnimKill(masuNextAnim);
        masuNextAnim = NULL;
    }
}

static void MasuNextDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    int drawNum = 0;
    GXColor color = { 255, 255, 255, 255 };
    Mtx masuMtx;
    Mtx modelMtx;
    Mtx rotMtx;
    MASUNEXTWORK *work;
    int i;
    int j;
    float rotY;
    
    if(masuNextDispF == FALSE) {
        return;
    }
    HuSprTexLoad(masuNextAnim, 0, GX_TEXMAP0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetCullMode(GX_CULL_BACK);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    work = masuNextWork;
    for(i=0; i<MASU_NEXT_MAX; i++, work++) {
        if(work->masuId == 0) {
            continue;
        }
        mbMasuMtxGet(work->masuId, masuMtx);
        color.a = work->alpha;
        GXSetTevColor(GX_TEVREG0, color);
        for(j=0; j<1; j++) {
            rotY = (j & 1) ? work->rot : -work->rot;
            MTXRotRad(rotMtx, 'Y', MTXDegToRad(rotY));
            MTXConcat(masuMtx, rotMtx, modelMtx);
            mtxScaleCat(modelMtx, work->scale, 1, work->scale);
            MTXConcat(*mtx, modelMtx, modelMtx);
            GXLoadPosMtxImm(modelMtx, GX_PNMTX0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(-120.00001f, 0.5f, -120.00001f);
            GXTexCoord2f32(0, 0);
            GXPosition3f32(120.00001f, 0.5f, -120.00001f);
            GXTexCoord2f32(1, 0);
            GXPosition3f32(120.00001f, 0.5f, 120.00001f);
            GXTexCoord2f32(1, 1);
            GXPosition3f32(-120.00001f, 0.5f, 120.00001f);
            GXTexCoord2f32(0, 1);
            GXEnd();
        }
        drawNum++;
    }
}

void mbMasuNextSet(s16 masuId)
{
    MASUNEXTWORK *work = masuNextWork;
    int i;
    if(masuNextId != masuId) {
        masuNextId = masuId;
        if(mbMasuDispCheck(masuId)) {
            for(i=0; i<MASU_NEXT_MAX; i++, work++) {
                if(work->masuId == 0) {
                    break;
                }
            }
            work->masuId = masuId;
            work->mode = 0;
            work->time = 0;
            work->maxTime = 12;
            work->rot = 0;
            work->playerNo = GwSystem.turnPlayerNo;
            work->delay = 6;
            work->alpha = 0;
        }
    }
}

void mbMasuNextDispSet(BOOL dispF)
{
    masuNextDispF = dispF;
}

int mbev_MasuMove(int playerNo, s16 masuId)
{
    if(MasuPartyFGet()) {
        switch(mbMasuTypeGet(masuId)) {
            case 6:
                break;
                
            case 7:
                mbev_StarMasu(playerNo, masuId);
                return TRUE;
                
            case 9:
                mbev_Shop(playerNo, masuId);
                break;
                
            case 8:
                mbCapMasuExec(playerNo, masuId);
                break;
        }
    } else {
        mbSingleCall(11, masuId);
        switch(mbMasuTypeGet(masuId)) {
            case 8:
                mbCapMasuExec(playerNo, masuId);
                break;
        }
    }
    mbev_CapCall(playerNo, mbMasuCapsuleGet(masuId), FALSE, TRUE);
    return FALSE;
}

#define MASU_PRIZE_INC(playerNo, field) do { \
    GwPlayer[playerNo].field++; \
    if(GwPlayer[playerNo].field > 99) { \
        GwPlayer[playerNo].field = 99; \
    } \
} while(0)

int mbev_MasuCapStop(int playerNo, s16 masuId)
{
    int result = TRUE;
    if(_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        mbTutorialCall(10);
    }
    if(MasuPartyFGet()) {
        if(mbMasuPlayerCapStopCheck(playerNo, masuId) == 0) {
            switch(mbMasuTypeGet(masuId)) {
                case 0:
                case 1:
                    MASU_PRIZE_INC(playerNo, plusMasuNum);
                    break;
                    
                case 2:
                    MASU_PRIZE_INC(playerNo, minusMasuNum);
                    break;
                    
                case 6:
                    if(GwSystem.curTime == 0) {
                        MASU_PRIZE_INC(playerNo, donkeyMasuNum);
                    } else {
                        MASU_PRIZE_INC(playerNo, koopaMasuNum);
                    }
                    break;
                    
                case 3:
                    MASU_PRIZE_INC(playerNo, hatenaMasuNum);
                    break;
                    
                case 4:
                    MASU_PRIZE_INC(playerNo, miracleMasuNum);
                    break;
                    
                case 5:
                    MASU_PRIZE_INC(playerNo, kettouMasuNum);
                    break;
            }
        } else {
            result = mbev_CapCall(playerNo, mbMasuCapsuleGet(masuId), FALSE, FALSE);
        }
    } else {
        switch(mbMasuTypeGet(masuId)) {
            case 6:
                MASU_PRIZE_INC(playerNo, koopaMasuNum);
                break;
                
            case 3:
                MASU_PRIZE_INC(playerNo, hatenaMasuNum);
                break;
        }
    }
    if(_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        mbTutorialCall(11);
    }
    return result;
}

int mbev_MasuStop(int playerNo, s16 masuId)
{
    int result = TRUE;
    if(_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        mbTutorialCall(12);
    }
    if(MasuPartyFGet()) {
        if(mbMasuPlayerCapStopCheck(playerNo, masuId) > 0) {
            mbPlayerCapCoinMasuExec(playerNo);
            return TRUE;
        }
        switch(mbMasuTypeGet(masuId)) {
            case 1:
                mbPlayerPlusMasuExec(playerNo);
                break;
                
            case 2:
                mbPlayerMinusMasuExec(playerNo);
                break;
                
            case 6:
                if(!_CheckFlag(FLAG_BOARD_TUTORIAL)) {
                    if(GwSystem.curTime == 0) {
                        mbev_CapCallDonkey(playerNo);
                    } else {
                        mbev_CapCallKoopa(playerNo);
                    }
                }
                break;
                
            case 3:
                result = ev_MasuHatena(playerNo, masuId);
                break;
                
            case 7:
                mbev_StarMasu(playerNo, masuId);
                break;
                
            case 5:
                if(!_CheckFlag(FLAG_BOARD_TUTORIAL)) {
                    mbev_CapCallKettou(playerNo, GwPlayer[playerNo].masuId, 1);
                }
                break;
                
            case 4:
                if(!_CheckFlag(FLAG_BOARD_TUTORIAL)) {
                    mbev_CapCallMiracle(playerNo, masuId);
                }
                break;
        }
    } else {
        mbSingleCall(7, masuId);
        switch(mbMasuTypeGet(masuId)) {
            case 1:
            case 2:
            case 4:
            case 5:
            case 6:
            case 7:
            case 9:
            case 10:
            case 11:
                if(!_CheckFlag(FLAG_BOARD_TUTORIAL)) {
                    mbev_SingleMg(playerNo, masuId);
                }
                break;
                
            case 3:
                if(!_CheckFlag(FLAG_BOARD_TUTORIAL)) {
                    result = ev_MasuHatena(playerNo, masuId);
                }
                break;
                
            case 8:
                break;
        }
    }
    return result;
}

static MASUDISPTBL masuDispTbl[11] = {
    { 0, 0 }, { 1, 1 }, { 2, 1 }, { 3, 1 }, { 4, 1 }, { 5, 1 },
    { 6, 1 }, { 7, 0 }, { 8, 0 }, { 9, 0 }, { 10, 0 }
};

static MASUDISPTBL masuSingleDispTbl[12] = {
    { 0, 0 }, { 1, 1 }, { 2, 1 }, { 3, 1 }, { 4, 1 }, { 5, 1 },
    { 6, 1 }, { 7, 1 }, { 8, 0 }, { 9, 1 }, { 10, 1 }, { 11, 1 }
};

int mbev_MasuMasuStart(int playerNo)
{
    int result;
    if(masuev_MasuEnd) {
        mbPlayerColSnapPlayerSet(playerNo, FALSE);
        result = masuev_MasuEnd(playerNo, GwPlayer[playerNo].masuId);
        mbPlayerColSnapPlayerSet(playerNo, TRUE);
    } else {
        result = FALSE;
    }
    mbMasuNextSet(GwPlayer[playerNo].masuId);
    return result;
}

int mbev_MasuMasuEnd(int masuId)
{
    int playerNo = GwSystem.turnPlayerNo;
    int result;
    if(masuev_MasuStart) {
        mbPlayerColSnapPlayerSet(playerNo, FALSE);
        result = masuev_MasuStart(playerNo, masuId);
        mbPlayerColSnapPlayerSet(playerNo, TRUE);
    } else {
        result = FALSE;
    }
    mbMasuNextSet(masuId);
    return result;
}

static int ev_MasuHatena(int playerNo, s16 masuId)
{
    int result = TRUE;
    if(masuev_HatenaHook) {
        mbAudFXPlay(0x450);
        omVibrate(playerNo, 20, 7, 3);
        mbPlayerColSnapPlayerSet(playerNo, FALSE);
        result = masuev_HatenaHook(playerNo, masuId);
    }
    return result;
}

int mbMasuNumGet(void)
{
    return masuNum[masuLayer]+1;
}

int mbMasuRawNumGet(void)
{
    return masuNum[masuLayer];
}

MASU *mbMasuGet(s16 id)
{
    return &masuData[masuLayer][id];
}

void mbMasuLayerSet(int layer)
{
    masuLayer = layer;
}

int mbMasuLayerGet(void)
{
    return masuLayer;
}

u16 mbMasuAttrGet(s16 id)
{
    return masuData[masuLayer][id].attr;
}

void mbMasuAttrSet(s16 id, u16 attr)
{
    masuData[masuLayer][id].attr = attr;
}

u32 mbMasuMAttrGet(s16 id)
{
    return masuData[masuLayer][id].mAttr;
}

void mbMasuMAttrSet(s16 id, u32 mAttr)
{
    masuData[masuLayer][id].mAttr = mAttr;
}

u32 mbev_MasuBitGet(u32 value, u32 mask)
{
    u32 bit;
    int i;
    u32 result;
    u32 outBit;
    value &= mask;
    result = 0;
    outBit = bit = 1;
    for(i=0; i<32; i++) {
        if(mask & bit) {
            if(value & bit) {
                result |= outBit;
            }
            outBit <<= 1;
        }
        bit <<= 1;
    }
    return result;
}

u32 mbev_MasuAttrGet(int value, u32 mask)
{
    u32 bit;
    int i;
    int inBit;
    u32 result;
    bit = inBit = 1;
    result = 0;
    for(i=0; i<32; i++) {
        if(mask & bit) {
            if(value & inBit) {
                result |= bit;
            }
            inBit <<= 1;
        }
        bit <<= 1;
    }
    return result;
}

int mbMasuTypeGet(s16 id)
{
    return masuData[masuLayer][id].type;
}

void mbMasuTypeSet(s16 id, int type)
{
    masuData[masuLayer][id].type = type;
}

void mbMasuTypeChange(u16 oldType, u16 newType)
{
    int i;
    MASU *masuP = &masuData[masuLayer][1];
    for(i=0; i<masuNum[masuLayer]; i++) {
        if(masuP->type == oldType) {
            masuP->type = newType;
            mbMasuCapsuleSet(i+1, -1);
        }
    }
}

int mbMasuDispCheck(s16 id)
{
    MASU *masuP = &masuData[masuLayer][id];
    int i;
    if(GWPartyGet() != FALSE) {
        for(i=0; i<MASU_TYPE_NUM; i++) {
            if(masuP->type == masuDispTbl[i].type) {
                break;
            }
        }
        return masuDispTbl[i].dispF;
    } else {
        for(i=0; i<MASU_TYPE_NUM; i++) {
            if(masuP->type == masuSingleDispTbl[i].type) {
                break;
            }
        }
        return masuSingleDispTbl[i].dispF;
    }
}

void mbMasuCapsuleReset(void)
{
    int i;
    for(i=0; i<MASU_MAX; i++) {
        GwSystem.masuCapsule[i] = -1;
    }
}

s16 mbMasuCapsuleGet(s16 id)
{
    return masuData[masuLayer][id].capsuleNo;
}

void mbMasuCapsuleSet(s16 id, int capsuleNo)
{
    GwSystem.masuCapsule[id] = capsuleNo;
    masuData[masuLayer][id].capsuleNo = capsuleNo;
}

void mbMasuPosGet(s16 id, HuVecF *pos)
{
    MASU *masuP = &masuData[masuLayer][id];
    if(masuP->useMtxF == FALSE) {
        *pos = masuP->pos;
    } else {
        pos->x = masuP->matrix[0][3];
        pos->y = masuP->matrix[1][3];
        pos->z = masuP->matrix[2][3];
    }
}

void mbMasuPosSet(s16 id, float x, float y, float z)
{
    MASU *masuP = &masuData[masuLayer][id];
    masuP->pos.x = x;
    masuP->pos.y = y;
    masuP->pos.z = z;
}

void mbMasuPosSetV(s16 id, HuVecF *pos)
{
    mbMasuPosSet(id, pos->x, pos->y, pos->z);
}

void mbMasuCornerRotPosGet(s16 id, int cornerNo, HuVecF *pos)
{
    HuVecF masuPos;
    HuVecF corner;
    HuVecF rot;
    mbMasuCornerPosGet(id, cornerNo, &corner);
    mbMasuPosGet(id, &masuPos);
    mbMasuRotGet(id, &rot);
    pos->x = masuPos.x+(corner.x*mbCosDeg(rot.z))+(corner.y*mbSinDeg(rot.z));
    pos->y = masuPos.y+(corner.x*mbSinDeg(rot.z))+(corner.y*(mbCosDeg(rot.x)*mbCosDeg(rot.z)))+(corner.z*mbSinDeg(-rot.x));
    pos->z = masuPos.z+(corner.y*mbSinDeg(rot.x))+(corner.z*mbCosDeg(rot.x));
}

typedef struct MasuCornerSort_s {
    int no;
    int num;
} MASUCORNERSORT;

void mbMasuCornerPosGet(s16 id, int cornerNo, HuVecF *pos)
{
    static int cornerNoTbl[8] = { 3, 1, 7, 5, 4, 2, 0, 6 };
    MASUCORNERSORT sortTbl[8];
    s16 linkTbl[10];
    int cornerNum[8];
    HuVecF masuPos;
    HuVecF linkPos;
    HuVecF dir;
    MASUCORNERSORT temp;
    int i;
    int j;
    int linkNum;
    MASU *masuP;
    u16 link;
    int cornerIdx;
    s16 linkId;
    float angle;
    
    masuP = mbMasuGet(id);
    linkNum = 0;
    for(i=0; i<masuP->linkNum; i++) {
        link = masuP->linkTbl[i];
        linkTbl[linkNum++] = link;
    }
    linkNum += mbMasuLinkParentGet(id, &linkTbl[linkNum]);
    for(i=0; i<8; i++) {
        cornerNum[i] = 0;
    }
    mbMasuPosGet(id, &masuPos);
    for(i=0; i<linkNum; i++) {
        linkId = linkTbl[i];
        mbMasuPosGet(linkId, &linkPos);
        VECSubtract(&linkPos, &masuPos, &dir);
        angle = HuAtan(-dir.z, dir.x);
        if(angle < 0) {
            angle += 360;
        }
        cornerIdx = (s16)((22.5f+angle)/45);
        cornerNum[cornerIdx]++;
    }
    for(i=0; i<8; i++) {
        sortTbl[i].no = cornerNoTbl[i];
        sortTbl[i].num = cornerNum[sortTbl[i].no];
    }
    for(i=0; i<7; i++) {
        for(j=i; j<8; j++) {
            if(sortTbl[j].num < sortTbl[i].num) {
                temp = sortTbl[i];
                sortTbl[i] = sortTbl[j];
                sortTbl[j] = temp;
            }
        }
    }
    pos->x = 100*mbCosDeg(360.0f*sortTbl[cornerNo].no/8);
    pos->y = 0;
    pos->z = 100*-mbSinDeg(360.0f*sortTbl[cornerNo].no/8);
}

void mbMasuRotGet(s16 id, HuVecF *rot)
{
    MASU *masuP = &masuData[masuLayer][id];
    if(masuP->useMtxF) {
        Hu3DMtxRotGet(masuP->matrix, rot);
    } else {
        *rot = masuP->rot;
    }
}

void mbMasuRotSet(s16 id, float x, float y, float z)
{
    MASU *masuP = &masuData[masuLayer][id];
    masuP->rot.x = x;
    masuP->rot.y = y;
    masuP->rot.z = z;
}

void mbMasuRotSetV(s16 id, HuVecF *rot)
{
    mbMasuRotSet(id, rot->x, rot->y, rot->z);
}

void mbMasuMtxGet(s16 id, Mtx mtx)
{
    MASU *masuP = &masuData[masuLayer][id];
    Mtx rot;
    Mtx trans;
    if(masuP->useMtxF) {
        MTXCopy(masuP->matrix, mtx);
    } else {
        mtxRot(rot, masuP->rot.x, masuP->rot.y, masuP->rot.z);
        MTXTrans(trans, masuP->pos.x, masuP->pos.y, masuP->pos.z);
        MTXConcat(trans, rot, mtx);
    }
}

void mbMasuMtxSet(s16 id, Mtx mtx)
{
    MASU *masuP = &masuData[masuLayer][id];
    if(mtx != NULL) {
        masuP->useMtxF = TRUE;
        MTXCopy(mtx, masuP->matrix);
    } else {
        masuP->useMtxF = FALSE;
    }
}

BOOL mbMasuDispGet(void)
{
    return masuDispF;
}

void mbMasuDispSet(BOOL dispF)
{
    masuDispF = dispF;
}

void mbMasuDispAttrSet(u16 attr)
{
    masuDispAttrMask[masuLayer] |= attr;
}

void mbMasuDispAttrReset(int attr)
{
    masuDispAttrMask[masuLayer] &= ~attr;
}

u16 mbMasuDispAttrGet(void)
{
    return masuDispAttrMask[masuLayer];
}

void mbMasuDispMAttrSet(u32 mAttr)
{
    masuDispMAttrMask[masuLayer] |= mAttr;
}

void mbMasuDispMAttrReset(u32 mAttr)
{
    masuDispMAttrMask[masuLayer] &= ~mAttr;
}

u32 mbMasuDispMAttrGet(void)
{
    return masuDispMAttrMask[masuLayer];
}

void mbMasuModelDispSet(BOOL dispF)
{
    if(dispF) {
        Hu3DModelDispOn(masuMdlId);
    } else {
        Hu3DModelDispOff(masuMdlId);
    }
}

s16 mbMasuLinkGet(s16 id, int no)
{
    MASU *masuP = &masuData[masuLayer][id];
    return masuP->linkTbl[no];
}

s16 mbMasuLinkNumGet(s16 id)
{
    MASU *masuP = &masuData[masuLayer][id];
    return masuP->linkNum;
}

int mbMasuLinkTblGet(s16 id, s16 *linkTbl)
{
    return mbMasuLinkTblGet2(id, linkTbl, FALSE);
}

int mbMasuLinkTblGet2(s16 id, s16 *linkTbl, BOOL hookF)
{
    MASU *masuP = &masuData[masuLayer][id];
    int num;
    int i;
    MASU *linkP;
    if(masuev_LinkTblHook != NULL && hookF) {
        num = masuev_LinkTblHook(id, masuP->mAttr, linkTbl, hookF);
        if(num > 0) {
            return num;
        }
    }
    num = 0;
    for(i=0; i<masuP->linkNum; i++) {
        linkP = mbMasuGet(masuP->linkTbl[i]);
        if((linkP->attr & mbBranchAttrGet()) == 0 && (linkP->mAttr & mbBranchMAttrGet()) == 0) {
            if(linkTbl) {
                linkTbl[num] = masuP->linkTbl[i];
            }
            num++;
        }
    }
    return num;
}

s16 mbMasuAttrFindLink(s16 id, u16 attr)
{
    MASU *masuP = &masuData[masuLayer][id];
    int i;
    MASU *linkP;
    for(i=0; i<masuP->linkNum; i++) {
        linkP = mbMasuGet(masuP->linkTbl[i]);
        if(linkP->attr & attr) {
            return masuP->linkTbl[i];
        }
    }
    return -1;
}

s16 mbMasuAttrMatchFindLink(s16 id, u16 attr, u16 mask)
{
    MASU *masuP = &masuData[masuLayer][id];
    int i;
    MASU *linkP;
    for(i=0; i<masuP->linkNum; i++) {
        linkP = mbMasuGet(masuP->linkTbl[i]);
        if(attr == (linkP->attr & mask)) {
            return masuP->linkTbl[i];
        }
    }
    return -1;
}

s16 mbMasuMAttrFindLink(s16 id, u32 mAttr)
{
    MASU *masuP = &masuData[masuLayer][id];
    int i;
    MASU *linkP;
    for(i=0; i<masuP->linkNum; i++) {
        linkP = mbMasuGet(masuP->linkTbl[i]);
        if(linkP->mAttr & mAttr) {
            return masuP->linkTbl[i];
        }
    }
    return -1;
}

s16 mbMasuMAttrMatchFindLink(s16 id, u32 mAttr, u32 mask)
{
    MASU *masuP = &masuData[masuLayer][id];
    int i;
    MASU *linkP;
    for(i=0; i<masuP->linkNum; i++) {
        linkP = mbMasuGet(masuP->linkTbl[i]);
        if(mAttr == (linkP->mAttr & mask)) {
            return masuP->linkTbl[i];
        }
    }
    return -1;
}

s16 mbMasuTypeFindLink(s16 id, int type)
{
    MASU *masuP = &masuData[masuLayer][id];
    int i;
    MASU *linkP;
    for(i=0; i<masuP->linkNum; i++) {
        linkP = mbMasuGet(masuP->linkTbl[i]);
        if(linkP->type == type) {
            return masuP->linkTbl[i];
        }
    }
    return -1;
}

s16 mbMasuLinkParentGet(s16 id, s16 *linkTbl)
{
    int i;
    int num = 0;
    MASU *masuP = &masuData[masuLayer][1];
    int j;
    for(i=0; i<masuNum[masuLayer]; i++, masuP++) {
        for(j=0; j<masuP->linkNum; j++) {
            if(masuP->linkTbl[j] == id) {
                linkTbl[num++] = i+1;
            }
        }
    }
    return num;
}

static int MasuTypeCheck(int masuId, int type, int unused)
{
    return mbMasuTypeGet(masuId) == type;
}

static int MasuAttrCheck(int masuId, int attr, int unused)
{
    return attr & mbMasuAttrGet(masuId);
}

static int MasuMAttrCheck(int masuId, int mAttr, int unused)
{
    return mAttr & mbMasuMAttrGet(masuId);
}

static int MasuAttrMatchCheck(int masuId, int attr, int mask)
{
    return (attr & mbMasuAttrGet(masuId)) == mask;
}

static int MasuMAttrMatchCheck(int masuId, int mAttr, int mask)
{
    return (mAttr & mbMasuMAttrGet(masuId)) == mask;
}

static int MasuIdCheck(int masuId, int targetId, int unused)
{
    return masuId == targetId;
}

static void MasuFind(s16 masuId, MASUCHECKFUNC checkFunc, int arg1, int arg2, BOOL hookF, BOOL dispF)
{
    s16 depth = 0;
    s16 startId = masuId;
    int linkNo;
    int i;
    int linkNum;
    s16 linkTbl[MASU_LINK_MAX];
    
    masuFindNo = 0;
    masuFindStep = 9999;
    masuFindId = -1;
    memset(masuFindVisit, 0, sizeof(masuFindVisit));
    masuFindResultNum = 0;
    linkNo = 0;
    while(1) {
        masuFindVisit[masuId] = TRUE;
        if(checkFunc(masuId, arg1, arg2)) {
            if(dispF && masuId != startId && mbMasuDispCheck(masuId) == FALSE) {
                masuFindNo++;
            }
            if(masuFindNo < masuFindStep) {
                masuFindStep = masuFindNo;
                masuFindId = masuId;
                for(i=0; i<depth; i++) {
                    masuFindResult[i] = masuFindWork[i].id;
                }
                masuFindResult[i] = masuId;
                masuFindResultNum = depth+1;
            }
        } else {
            linkNum = mbMasuLinkTblGet2(masuId, linkTbl, hookF);
            for(; linkNo<linkNum; linkNo++) {
                if(masuFindVisit[linkTbl[linkNo]] == FALSE) {
                    break;
                }
            }
            if(linkNo < linkNum) {
                masuFindWork[depth].id = masuId;
                masuFindWork[depth].no = linkNo+1;
                depth++;
                masuId = linkTbl[linkNo];
                linkNo = 0;
                if(!dispF) {
                    masuFindNo++;
                } else if(mbMasuDispCheck(masuId)) {
                    masuFindNo++;
                }
                continue;
            }
        }
        masuFindVisit[masuId] = FALSE;
        depth--;
        if(depth < 0) {
            break;
        }
        if(!dispF) {
            masuFindNo--;
        } else if(mbMasuDispCheck(masuId)) {
            masuFindNo--;
        }
        masuId = masuFindWork[depth].id;
        linkNo = masuFindWork[depth].no;
    }
}

s16 mbMasuFind_TypeStepGet(s16 id, s16 type)
{
    MasuFind(id, MasuTypeCheck, type, 0, FALSE, TRUE);
    return masuFindStep;
}

s16 mbMasuFind_TypeStepGet2(s16 id, s16 type, BOOL hookF, BOOL dispF)
{
    MasuFind(id, MasuTypeCheck, type, 0, hookF, dispF);
    return masuFindStep;
}

s16 mbMasuFind_AttrStepGet(s16 id, u16 attr)
{
    MasuFind(id, MasuAttrCheck, attr, 0, FALSE, TRUE);
    return masuFindStep;
}

s16 mbMasuFind_AttrMatchStepGet(s16 id, u16 attr, u16 mask)
{
    MasuFind(id, MasuAttrMatchCheck, attr, mask, FALSE, TRUE);
    return masuFindStep;
}

s16 mbMasuFind_AttrStepGet2(s16 id, u16 attr, u16 mask, BOOL hookF, BOOL dispF)
{
    MasuFind(id, MasuAttrMatchCheck, attr, mask, hookF, dispF);
    return masuFindStep;
}

s16 mbMasuFind_MAttrStepGet(s16 id, u32 mAttr)
{
    MasuFind(id, MasuMAttrCheck, mAttr, 0, FALSE, TRUE);
    return masuFindStep;
}

s16 mbMasuFind_MAttrMatchStepGet(s16 id, u32 mAttr, u32 mask)
{
    MasuFind(id, MasuMAttrMatchCheck, mAttr, mask, FALSE, TRUE);
    return masuFindStep;
}

s16 mbMasuFind_MAttrStepGet2(s16 id, u32 mAttr, u32 mask, BOOL hookF, BOOL dispF)
{
    MasuFind(id, MasuMAttrMatchCheck, mAttr, mask, hookF, dispF);
    return masuFindStep;
}

s16 mbMasuFind_IdStepGet(s16 id, s16 targetId)
{
    MasuFind(id, MasuIdCheck, targetId, 0, FALSE, TRUE);
    return masuFindStep;
}

s16 mbMasuFind_IdStepGet2(s16 id, s16 targetId, BOOL hookF, BOOL dispF)
{
    MasuFind(id, MasuIdCheck, targetId, 0, hookF, dispF);
    return masuFindStep;
}

s16 mbMasuFind_TypeSearch(s16 id, s16 type)
{
    return mbMasuFind_TypeIdGet(id, type, FALSE, TRUE);
}

s16 mbMasuFind_TypeIdGet(s16 id, s16 type, BOOL hookF, BOOL dispF)
{
    int i;
    MASU *masuP;
    if(id >= 0) {
        MasuFind(id, MasuTypeCheck, type, 0, hookF, dispF);
        return masuFindId;
    }
    masuP = &masuData[masuLayer][1];
    for(i=0; i<masuNum[masuLayer]; i++, masuP++) {
        if(masuP->type == type) {
            return i+1;
        }
    }
    return -1;
}

s16 mbMasuFind_AttrIdGet(s16 id, u16 attr)
{
    int i;
    MASU *masuP;
    if(id >= 0) {
        MasuFind(id, MasuAttrCheck, attr, 0, FALSE, TRUE);
        return masuFindId;
    }
    masuP = &masuData[masuLayer][1];
    for(i=0; i<masuNum[masuLayer]; i++, masuP++) {
        if(masuP->attr & attr) {
            return i+1;
        }
    }
    return -1;
}

s16 mbMasuFind_AttrMatchIdGet(s16 id, u16 attr, u16 mask)
{
    return mbMasuFind_AttrMatchIdGet2(id, attr, mask, FALSE, TRUE);
}

s16 mbMasuFind_AttrMatchIdGet2(s16 id, u16 attr, u16 mask, BOOL hookF, BOOL dispF)
{
    int i;
    MASU *masuP;
    if(id >= 0) {
        MasuFind(id, MasuAttrMatchCheck, attr, mask, hookF, dispF);
        return masuFindId;
    }
    masuP = &masuData[masuLayer][1];
    for(i=0; i<masuNum[masuLayer]; i++, masuP++) {
        if(attr == (masuP->attr & mask)) {
            return i+1;
        }
    }
    return -1;
}

s16 mbMasuFind_MAttrIdGet(s16 id, u32 mAttr)
{
    int i;
    MASU *masuP;
    if(id >= 0) {
        MasuFind(id, MasuMAttrCheck, mAttr, 0, FALSE, TRUE);
        return masuFindId;
    }
    masuP = &masuData[masuLayer][1];
    for(i=0; i<masuNum[masuLayer]; i++, masuP++) {
        if(masuP->mAttr & mAttr) {
            return i+1;
        }
    }
    return -1;
}

s16 mbMasuFind_MAttrMatchIdGet(s16 id, u32 mAttr, u32 mask)
{
    return mbMasuFind_MAttrMatchIdGet2(id, mAttr, mask, FALSE, TRUE);
}

s16 mbMasuFind_MAttrMatchIdGet2(s16 id, u32 mAttr, u32 mask, BOOL hookF, BOOL dispF)
{
    int i;
    MASU *masuP;
    if(id >= 0) {
        MasuFind(id, MasuMAttrMatchCheck, mAttr, mask, hookF, dispF);
        return masuFindId;
    }
    masuP = &masuData[masuLayer][1];
    for(i=0; i<masuNum[masuLayer]; i++, masuP++) {
        if(mAttr == (masuP->mAttr & mask)) {
            return i+1;
        }
    }
    return -1;
}

int mbMasuTypeListGet(s16 type, s16 *list)
{
    int i;
    int num = 0;
    MASU *masuP = &masuData[masuLayer][1];
    for(i=0; i<masuNum[masuLayer]; i++, masuP++) {
        if(masuP->type == type) {
            if(list != NULL) {
                list[num] = i+1;
            }
            num++;
        }
    }
    return num;
}

int mbMasuAttrListGet(u16 attr, s16 *list)
{
    int i;
    int num = 0;
    MASU *masuP = &masuData[masuLayer][1];
    for(i=0; i<masuNum[masuLayer]; i++, masuP++) {
        if(masuP->attr & attr) {
            if(list != NULL) {
                list[num] = i+1;
            }
            num++;
        }
    }
    return num;
}

int mbMasuAttrMatchListGet(u16 attr, u16 mask, s16 *list)
{
    int i;
    int num = 0;
    MASU *masuP = &masuData[masuLayer][1];
    for(i=0; i<masuNum[masuLayer]; i++, masuP++) {
        if(attr == (masuP->attr & mask)) {
            if(list != NULL) {
                list[num] = i+1;
            }
            num++;
        }
    }
    return num;
}

int mbMasuMAttrListGet(u32 mAttr, s16 *list)
{
    int i;
    int num = 0;
    MASU *masuP = &masuData[masuLayer][1];
    for(i=0; i<masuNum[masuLayer]; i++, masuP++) {
        if(masuP->mAttr & mAttr) {
            if(list != NULL) {
                list[num] = i+1;
            }
            num++;
        }
    }
    return num;
}

int mbMasuMAttrMatchTblGet(u32 mAttr, u32 mask, s16 *list)
{
    int i;
    int num = 0;
    MASU *masuP = &masuData[masuLayer][1];
    for(i=0; i<masuNum[masuLayer]; i++, masuP++) {
        if(mAttr == (masuP->mAttr & mask)) {
            if(list != NULL) {
                list[num] = i+1;
            }
            num++;
        }
    }
    return num;
}

s16 mbMasuFind_TypeListGet(s16 id, s16 type, s16 *list)
{
    return mbMasuFind_TypeListGet2(id, type, FALSE, TRUE, list);
}

s16 mbMasuFind_TypeListGet2(s16 id, s16 type, BOOL hookF, BOOL dispF, s16 *list)
{
    MasuFind(id, MasuTypeCheck, type, 0, hookF, dispF);
    memcpy(list, masuFindResult, masuFindResultNum*sizeof(s16));
    return masuFindResultNum;
}

s16 mbMasuFind_AttrNumGet(s16 id, u16 attr, s16 *list)
{
    return mbMasuFind_AttrMatchNumGet2(id, attr, attr, FALSE, TRUE, list);
}

s16 mbMasuFind_AttrMatchNumGet(s16 id, u16 attr, u16 mask, s16 *list)
{
    return mbMasuFind_AttrMatchNumGet2(id, attr, mask, FALSE, TRUE, list);
}

s16 mbMasuFind_AttrMatchNumGet2(s16 id, u16 attr, u16 mask, BOOL hookF, BOOL dispF, s16 *list)
{
    MasuFind(id, MasuAttrMatchCheck, attr, mask, hookF, dispF);
    return masuFindResultNum;
}

s16 mbMasuFind_MAttrNumGet(s16 id, u32 mAttr)
{
    return mbMasuFind_MAttrMatchIdGet2(id, mAttr, mAttr, FALSE, TRUE);
}

s16 mbMasuFind_MAttrMatchListGet(s16 id, u32 mAttr, u32 mask, s16 *list)
{
    return mbMasuFind_MAttrMatchListGet2(id, mAttr, mask, FALSE, TRUE, list);
}

s16 mbMasuFind_MAttrMatchListGet2(s16 id, u32 mAttr, u32 mask, BOOL hookF, BOOL dispF, s16 *list)
{
    MasuFind(id, MasuMAttrMatchCheck, mAttr, mask, hookF, dispF);
    memcpy(list, masuFindResult, masuFindResultNum*sizeof(s16));
    return masuFindResultNum;
}

s16 mbMasuFind_IdListGet(s16 id, s16 targetId, s16 *list)
{
    return mbMasuFind_IdListGet2(id, targetId, FALSE, TRUE, list);
}

s16 mbMasuFind_IdListGet2(s16 id, s16 targetId, BOOL hookF, BOOL dispF, s16 *list)
{
    MasuFind(id, MasuIdCheck, targetId, 0, hookF, dispF);
    memcpy(list, masuFindResult, masuFindResultNum*sizeof(s16));
    return masuFindResultNum;
}

void mbev_MasuMoveEndSet(MASUEVHOOK func)
{
    masuev_MasuEnd = func;
}

void mbev_MasuMoveStartSet(MASUEVHOOK func)
{
    masuev_MasuStart = func;
}

void mbev_MasuHatenaSet(MASUEVHOOK func)
{
    masuev_HatenaHook = func;
}

void mbev_MasuLinkTblHookSet(MASULINKTBLHOOK func)
{
    masuev_LinkTblHook = func;
}

int mbMasuPlayerCapMoveCheck(int playerNo, s16 masuId)
{
    int capPlayer = mbCapMasuPlayerGet(masuId);
    if(capPlayer < 0) {
        return 0;
    }
    if(mbCapMasuDispTypeGet(masuId) == 2) {
        if(capPlayer == playerNo || (MasuTeamFGet() && capPlayer == mbPlayerTeamFind(playerNo))) {
            return 1;
        }
        return -1;
    }
    return 0;
}

int mbMasuPlayerCapStopCheck(int playerNo, s16 masuId)
{
    int capPlayer = mbCapMasuPlayerGet(masuId);
    if(capPlayer < 0) {
        return 0;
    }
    if(mbCapMasuDispTypeGet(masuId) == 1) {
        if(capPlayer == playerNo || (MasuTeamFGet() && capPlayer == mbPlayerTeamFind(playerNo))) {
            return 1;
        }
        return -1;
    }
    return 0;
}

void mbMasuPlayerColorSet(int playerNo)
{
    int colorTbl[8][2] = {
        { 1, 1 }, { 2, 2 }, { 6, 0 }, { 3, 3 },
        { 5, 3 }, { 4, 3 }, { 8, 3 }, { -1, -1 }
    };
    s16 masuId;
    int capResult;
    int type;
    int i;
    int color;
    if(MasuPartyFGet()) {
        masuId = GwPlayer[playerNo].masuId;
        capResult = mbMasuPlayerCapStopCheck(playerNo, masuId);
        if(capResult == 0) {
            type = mbMasuTypeGet(masuId);
            for(i=0; colorTbl[i][0] >= 0; i++) {
                if(type == colorTbl[i][0]) {
                    break;
                }
            }
            if(colorTbl[i][0] < 0) {
                mbStatusColorSet(playerNo, 0);
                return;
            }
            if(colorTbl[i][0] == 6) {
                color = (MasuTimeDayCheck()) ? 1 : 2;
            } else {
                color = colorTbl[i][1];
            }
        } else if(capResult > 0) {
            color = 4;
        } else {
            color = 4;
        }
        mbStatusColorSet(playerNo, color);
    }
}

void mbMasuPlayerDispSet(BOOL dispF)
{
    masuCapsuleDispF = dispF;
}

void mbMasuPlayerFadeSet(BOOL fadeF)
{
    masuCapsuleFadeOnF = fadeF;
}

void mbMasuPlayerPrizeReset(int playerNo)
{
    GwPlayer[playerNo].plusMasuNum = 0;
    GwPlayer[playerNo].minusMasuNum = 0;
    GwPlayer[playerNo].capsuleMasuNum = 0;
    GwPlayer[playerNo].hatenaMasuNum = 0;
    GwPlayer[playerNo].koopaMasuNum = 0;
    GwPlayer[playerNo].miracleMasuNum = 0;
    GwPlayer[playerNo].kettouMasuNum = 0;
    GwPlayer[playerNo].donkeyMasuNum = 0;
}

typedef struct MasuKinokoResult_s {
    u8 id;
    u8 step;
} MASUKINOKORESULT;

static inline void MasuPlayerNextSet(int playerNo, int masuId)
{
    mbPlayerWorkGet(playerNo)->unkE = masuId;
}

int mbMasuPKinokoValueGet(int playerNo, s16 masuId)
{
    int linkNo;
    s16 depth = 0;
    s16 startId = masuId;
    s16 resultNum = 0;
    s16 candNum = 0;
    int i;
    int linkNum;
    int type;
    u32 sel;
    MASUKINOKORESULT result[MASU_MAX];
    u8 visit[MASU_MAX];
    u8 cand[MASU_MAX];
    s16 linkTbl[MASU_LINK_MAX];
    
    if(mbRandMod(100) < 50) {
        return 9;
    }
    masuFindNo = 0;
    masuFindStep = 9999;
    masuFindId = -1;
    memset(masuFindVisit, 0, sizeof(masuFindVisit));
    memset(visit, 0, sizeof(visit));
    visit[masuId] = TRUE;
    masuFindResultNum = 0;
    linkNo = 0;
    while(1) {
        masuFindVisit[masuId] = TRUE;
        if(masuFindNo <= 10) {
            linkNum = mbMasuLinkTblGet2(masuId, linkTbl, FALSE);
            for(; linkNo<linkNum; linkNo++) {
                if(masuFindVisit[linkTbl[linkNo]] == FALSE) {
                    break;
                }
            }
            if(linkNo < linkNum) {
                masuFindWork[depth].id = masuId;
                masuFindWork[depth].no = linkNo+1;
                depth++;
                if(mbMasuDispCheck(masuId)) {
                    if(visit[masuId] == FALSE) {
                        result[resultNum].id = masuId;
                        result[resultNum].step = masuFindNo;
                        resultNum++;
                        visit[masuId] = TRUE;
                    } else if(resultNum != 0) {
                        for(i=0; i<resultNum; i++) {
                            if(masuId == result[i].id) {
                                break;
                            }
                        }
                        if(result[i].step > masuFindNo) {
                            result[i].step = masuFindNo;
                        }
                    }
                }
                masuId = linkTbl[linkNo];
                linkNo = 0;
                if(mbMasuDispCheck(masuId)) {
                    masuFindNo++;
                }
                continue;
            }
        }
        masuFindVisit[masuId] = FALSE;
        depth--;
        if(depth < 0) {
            break;
        }
        if(mbMasuDispCheck(masuId)) {
            masuFindNo--;
        }
        masuId = masuFindWork[depth].id;
        linkNo = masuFindWork[depth].no;
    }
    for(i=0; i<resultNum; i++) {
        type = mbMasuTypeGet(result[i].id);
        if(type == 3 || type == 4 || type == 5 || (GwSystem.curTime == 0 && type == 6)) {
            cand[candNum++] = i;
        }
    }
    if(candNum == 0) {
        for(i=0; i<resultNum; i++) {
            if(playerNo == mbCapMasuPlayerGet(result[i].id) && mbCapMasuDispTypeGet(result[i].id) == 1) {
                cand[candNum++] = i;
            }
        }
    }
    if(candNum == 0) {
        return 9;
    }
    sel = mbRandMod(candNum);
    MasuPlayerNextSet(playerNo, result[cand[sel]].id);
    return result[cand[sel]].step-1;
}

int mbMasuStub(void)
{
    return 0;
}
