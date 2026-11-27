/* @@HEAD@@ */
#include "game/board/snpc.h"
#include "game/board/object.h"
#include "game/board/malloc.h"
#include "game/board/math.h"
#include "game/board/telop.h"
#include "game/data.h"
#include "game/memory.h"
#include "game/sprite.h"


void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);

/* Defined later in the unit (still assembly). */
void SNpcObjDispSet(BOOL dispF);

#define SNPC_MAGIC 'SNPC'

/* Material hook work of mbObjFade*: a texture projected through the model. */
typedef struct MbObjFadeWork_s {
    /* 0x00 */ u32 magic;
    /* 0x04 */ ANIMDATA *anim;
    /* 0x08 */ HuVecF pos;
    /* 0x14 */ HuVecF rot;
    /* 0x20 */ float scale;
    /* 0x24 */ GXColor color;
} MBOBJFADEWORK; /* 0x28 */

#define MBOBJFADE_MAGIC 'MBTV'

/* mbObjMetal*: a metal environment map with a transparency level. */
typedef struct MbObjMetalWork_s {
    /* 0x00 */ u32 magic;
    /* 0x04 */ ANIMDATA *anim;
    /* 0x08 */ ANIMDATA *hiliteAnim;
    /* 0x0C */ float tpLvl;
    /* 0x10 */ GXColor shadowColor;
    /* 0x14 */ GXColor hiliteColor;
} MBOBJMETALWORK; /* 0x18 */

#define MBOBJMETAL_MAGIC 'TV01'

/* mbObjBiriQ*: the electric tint. */
typedef struct MbObjBiriQWork_s {
    /* 0x00 */ u32 magic;
    /* 0x04 */ int mode;
    /* 0x08 */ float alpha;
    /* 0x0C */ GXColor color;
} MBOBJBIRIQWORK; /* 0x10 */

#define MBOBJBIRIQ_MAGIC 'TV02'

/* One TEV colour or alpha setting: op, output register, then the four inputs. */
typedef struct BiriQMat_s {
    u8 op;
    u8 outReg;
    u8 a;
    u8 b;
    u8 c;
    u8 d;
} BIRIQMAT;

static int snpcMagic;
static void *snpcSaveWork;
static void *snpcWork;

static GXTevKColorSel kColorTbl[8] = {
    GX_TEV_KCSEL_8_8,
    GX_TEV_KCSEL_7_8,
    GX_TEV_KCSEL_6_8,
    GX_TEV_KCSEL_5_8,
    GX_TEV_KCSEL_4_8,
    GX_TEV_KCSEL_3_8,
    GX_TEV_KCSEL_2_8,
    GX_TEV_KCSEL_1_8
};

static int biriQMatNumTbl[4] = { 2, 1, 1, 2 };

/* Per mode, up to two stages of { colour, alpha }. */
static BIRIQMAT biriQMatTbl[16] = {
    { 0x00, 0x01, 0x0F, 0x0F, 0x0F, 0x0E }, { 0x00, 0x01, 0x07, 0x07, 0x07, 0x06 },
    { 0x00, 0x00, 0x00, 0x02, 0x03, 0x0F }, { 0x00, 0x00, 0x07, 0x07, 0x07, 0x00 },
    { 0x00, 0x00, 0x0E, 0x0F, 0x0F, 0x00 }, { 0x00, 0x00, 0x07, 0x07, 0x07, 0x00 },
    { 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x00 }, { 0x00, 0x00, 0x07, 0x07, 0x07, 0x00 },
    { 0x01, 0x00, 0x0E, 0x0F, 0x0F, 0x00 }, { 0x00, 0x00, 0x07, 0x07, 0x07, 0x00 },
    { 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x00 }, { 0x00, 0x00, 0x07, 0x07, 0x07, 0x00 },
    { 0x01, 0x01, 0x00, 0x0F, 0x0F, 0x0C }, { 0x00, 0x01, 0x07, 0x07, 0x07, 0x06 },
    { 0x00, 0x00, 0x00, 0x02, 0x03, 0x0F }, { 0x00, 0x00, 0x07, 0x07, 0x07, 0x00 }
};

static GXColor texCol[16];

/* @@PART1@@ */
void mbSNpcInit(void)
{
    snpcMagic = 0;
    snpcSaveWork = NULL;
    snpcWork = NULL;
}

void mbSNpcDispSet(BOOL dispF)
{
    if(snpcMagic == SNPC_MAGIC) {
        SNpcObjDispSet(dispF);
    }
}

/* @@PART2@@ */
/* @@PART3@@ */
/* Counts the TEV stages and texture coordinates Hu3DTevStageTexSet used, so a material hook can
 * append its own stages after them. */
static void GetStarTexTevStage(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *matP, u32 *tevStageP, u32 *texCoordP)
{
    s32 bumpTexMap;
    BOOL texBlendF;
    u32 flags;
    u32 shineF;
    u16 lightOnF;
    s16 texMapId;
    u16 tevStage;
    u16 texCoordId;
    u16 i;
    HSF_ATTRIBUTE *attrP;
    HU3D_MODEL *modelP;
    HSF_OBJECT *objPtr;
    HU3D_ATTR_ANIM *animWorkP;
    u16 j;
    u16 matHiliteF;
    HSF_BITMAP *bitMapPtr;
    HSF_CONSTDATA *constData;

    texMapId = -1;
    objPtr = drawObj->object;
    modelP = drawObj->model;
    for(i = 0; i < matP->attrNum; i++) {
        attrP = &objPtr->mesh.attribute[matP->attr[i]];
        bitMapPtr = attrP->bitmap;
        texCol[i].a = 0;
        if(attrP->animWorkP) {
            animWorkP = attrP->animWorkP;
            if((animWorkP->attr & HU3D_ATTRANIM_ATTR_ANIM2D) && !(Hu3DTexAnimData[animWorkP->animId].attr & HU3D_ANIM_ATTR_NOUSE)) {
            } else if(animWorkP->attr & HU3D_ATTRANIM_ATTR_BMPANIM) {
                bitMapPtr = animWorkP->bitMapPtr;
                switch(bitMapPtr->dataFmt) {
                    case HSF_BMPFMT_I4:
                    case HSF_BMPFMT_I8:
                    case HSF_BMPFMT_IA4:
                    case HSF_BMPFMT_IA8:
                        texCol[i].a = 1;
                        break;

                    case HSF_BMPFMT_CI_IA8:
                        texCol[i].a = 2;
                        break;
                }
            }
        } else {
            switch(bitMapPtr->dataFmt) {
                case HSF_BMPFMT_I4:
                case HSF_BMPFMT_I8:
                case HSF_BMPFMT_IA4:
                case HSF_BMPFMT_IA8:
                    texCol[i].a = 1;
                    break;

                case HSF_BMPFMT_CI_IA8:
                    texCol[i].a = 2;
                    break;
            }
        }
    }
    flags = objPtr->flags | matP->flags;
    if(matP->vtxMode == 2 || matP->vtxMode == 3) {
        matHiliteF = TRUE;
    } else {
        matHiliteF = FALSE;
        if(matP->vtxMode == 0 || matP->vtxMode == 5) {
            lightOnF = GX_FALSE;
        } else {
            lightOnF = GX_TRUE;
        }
    }
    if(Hu3DShineF && lightOnF) {
        shineF = TRUE;
    } else {
        shineF = FALSE;
    }
    if(matP->attrNum == 1) {
        texCoordId = tevStage = 1;
        attrP = &objPtr->mesh.attribute[matP->attr[0]];
        if(attrP->unk20 == 1.0f) {
            if(attrP->unk8[2] == 0) {
                tevStage++;
            } else if(!(modelP->attr & HU3D_ATTR_TOON_MAP)) {
                if(texCol[0].a == 1) {
                    tevStage++;
                } else if(texCol[0].a == 2) {
                    tevStage++;
                }
            }
        }
        if(modelP->attr & HU3D_ATTR_TOON_MAP) {
            texCoordId++;
            tevStage++;
        }
        if(matP->refAlpha != 0.0f) {
            texCoordId++;
            tevStage++;
        }
        if(shineF) {
            tevStage++;
        }
        constData = drawObj->object->constData;
        if(Hu3DShadowF != 0 && shadowNum != 0 && (constData->attr & HU3D_CONST_SHADOW_MAP)) {
            HSF_CONSTDATA *shadowData = objPtr->constData;
            if(shadowData->attr & HU3D_CONST_SHADOW_MAP_TPLVL) {
                tevStage++;
            }
            tevStage++;
            texCoordId++;
        }
        if(matHiliteF) {
            if((modelP->attr & HU3D_ATTR_HILITE) || (flags & HSF_MATERIAL_HILITE)) {
                tevStage++;
                texCoordId++;
                matHiliteF = FALSE;
            } else {
                if(attrP->unk20 != 1.0f) {
                    texCoordId++;
                }
                tevStage++;
            }
        } else if(matP->invAlpha != 0.0f) {
            tevStage++;
        }
        if(modelP->projBit != 0) {
            for(i = 0, j = 1; i < HU3D_PROJ_MAX; i++, j <<= 1) {
                if(j & modelP->projBit) {
                    texCoordId++;
                    tevStage += 2;
                }
            }
        }
    } else {
        texBlendF = FALSE;
        texCoordId = 0;
        bumpTexMap = -1;
        for(i = tevStage = 0; i < matP->attrNum; i++) {
            attrP = &objPtr->mesh.attribute[matP->attr[i]];
            if(attrP->nbtTpLvl != 0.0f) {
                tevStage++;
                bumpTexMap = i;
                texCoordId++;
                texBlendF = TRUE;
            } else if(attrP->unk20 != 1.0f) {
                texMapId = i;
                continue;
            } else {
                texCoordId++;
                if(i == 0) {
                    if(texCol[i].a == 1 || texCol[i].a == 2) {
                        tevStage++;
                    }
                } else if(texBlendF) {
                    texBlendF = FALSE;
                } else if(attrP->unk8[2] == 0) {
                    tevStage++;
                } else if(texCol[i].a == 1 || texCol[i].a == 2) {
                    tevStage++;
                }
            }
            tevStage++;
        }
        if(modelP->attr & HU3D_ATTR_TOON_MAP) {
            texCoordId++;
            tevStage++;
        }
        if(matP->refAlpha != 0.0f) {
            if(texMapId != -1) {
                tevStage++;
                texCoordId++;
            }
            tevStage++;
            texCoordId++;
        }
        if(shineF) {
            tevStage++;
        }
        constData = drawObj->object->constData;
        if(Hu3DShadowF != 0 && shadowNum != 0 && (constData->attr & HU3D_CONST_SHADOW_MAP)) {
            HSF_CONSTDATA *shadowData = objPtr->constData;
            if(shadowData->attr & HU3D_CONST_SHADOW_MAP_TPLVL) {
                tevStage++;
            }
            tevStage++;
            texCoordId++;
        }
        if(matHiliteF) {
            if((modelP->attr & HU3D_ATTR_HILITE) || (flags & HSF_MATERIAL_HILITE)) {
                if(texMapId != -1) {
                    tevStage++;
                    texCoordId++;
                }
                tevStage++;
                texCoordId++;
                matHiliteF = FALSE;
            } else {
                if(texMapId != -1) {
                    texCoordId++;
                }
                tevStage++;
            }
        } else if(matP->invAlpha != 0.0f) {
            tevStage++;
        }
        if(modelP->projBit != 0) {
            for(i = 0, j = 1; i < HU3D_PROJ_MAX; i++, j <<= 1) {
                if(j & modelP->projBit) {
                    texCoordId++;
                    tevStage += 2;
                }
            }
        }
        if(bumpTexMap != -1) {
            texCoordId++;
        }
    }
    *tevStageP = tevStage;
    *texCoordP = texCoordId;
}

/* The same count for Hu3DTevStageNoTexSet. */
static void GetStarNoTexTevStage(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *matP, u32 *tevStageP, u32 *texCoordP)
{
    s16 tevStage;
    s16 texMap;
    HU3D_MODEL *modelP;
    HSF_OBJECT *objPtr;
    s16 i;
    s16 j;
    s16 matHiliteF;
    HSF_CONSTDATA *constData;
    HSF_CONSTDATA *shadowData;
    u32 flags;

    tevStage = 1;
    texMap = 0;
    objPtr = drawObj->object;
    modelP = drawObj->model;
    flags = objPtr->flags | matP->flags;
    if(matP->vtxMode == 2 || matP->vtxMode == 3) {
        matHiliteF = TRUE;
    } else {
        matHiliteF = FALSE;
    }
    if(modelP->attr & HU3D_ATTR_TOON_MAP) {
        texMap++;
    }
    if(matP->refAlpha != 0.0f) {
        tevStage++;
        texMap++;
    }
    constData = drawObj->object->constData;
    if(Hu3DShadowF != 0 && shadowNum != 0 && (constData->attr & HU3D_CONST_SHADOW_MAP)) {
        shadowData = objPtr->constData;
        if(shadowData->attr & HU3D_CONST_SHADOW_MAP_TPLVL) {
            tevStage++;
        }
        tevStage++;
        texMap++;
    }
    if(matHiliteF) {
        if((modelP->attr & HU3D_ATTR_HILITE) || (flags & HSF_MATERIAL_HILITE)) {
            texMap++;
        }
        tevStage++;
    } else if(matP->invAlpha != 0.0f) {
        tevStage++;
    }
    if(modelP->projBit != 0) {
        for(i = 0, j = 1; i < HU3D_PROJ_MAX; i++, j <<= 1) {
            if(j & modelP->projBit) {
                texMap++;
                tevStage += 2;
            }
        }
    }
    *tevStageP = tevStage;
    *texCoordP = texMap;
}

void mbObjStarTevStageSet(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *matP, u32 *tevStage, u32 *texCoord)
{
    if(matP->attrNum == 0) {
        Hu3DTevStageNoTexSet(drawObj, matP);
        GetStarNoTexTevStage(drawObj, matP, tevStage, texCoord);
    } else {
        Hu3DTevStageTexSet(drawObj, matP);
        GetStarTexTevStage(drawObj, matP, tevStage, texCoord);
    }
}

static void FadeMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *matP);

void mbObjFadeCreate(s16 objId, HuVecF *pos)
{
    int mdlId;
    MBOBJFADEWORK *work;
    HU3D_MODEL *modelP;
    HSF_DATA *hsf;
    int i;

    mdlId = mbObjModelIDGet((MBOBJID)objId);
    modelP = &Hu3DData[mdlId];
    if(modelP->attr & HU3D_ATTR_LINK) {
        modelP->hookData = work = mbMallocNum(sizeof(MBOBJFADEWORK), modelP->mallocNoLink);
    } else {
        modelP->hookData = work = mbMallocNum(sizeof(MBOBJFADEWORK), modelP->mallocNo);
    }
    work->magic = MBOBJFADE_MAGIC;
    mbVecCopy(pos, &work->pos);
    work->scale = 1.0f;
    work->color.r = work->color.g = work->color.b = 255;
    Hu3DModelMatHookSet(mdlId, FadeMatHook);
    hsf = modelP->hsf;
    for(i = 0; i < hsf->materialNum; i++) {
        hsf->material[i].flags |= HSF_MATERIAL_MATHOOK;
    }
    work->anim = HuSprAnimRead(HuDataSelHeapReadNum(mbBoardDataNumGet(0x50067), HU_MEMNUM_OVL, HEAP_MODEL));
}

void mbObjFadeKill(s16 objId)
{
    int mdlId;
    MBOBJFADEWORK *work;
    HU3D_MODEL *modelP;
    HSF_DATA *hsf;
    int i;

    mdlId = mbObjModelIDGet((MBOBJID)objId);
    modelP = &Hu3DData[mdlId];
    work = modelP->hookData;
    Hu3DModelMatHookSet(mdlId, NULL);
    hsf = modelP->hsf;
    for(i = 0; i < hsf->materialNum; i++) {
        hsf->material[i].flags &= ~HSF_MATERIAL_MATHOOK;
    }
    HuSprAnimKill(work->anim);
    HuMemDirectFree(work);
    modelP->hookData = NULL;
}

static void FadeMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *matP)
{
    Mtx mtx;
    Mtx mtxTemp;
    u32 tevStage;
    u32 texCoord;
    MBOBJFADEWORK *work;
    float scale;

    work = drawObj->model->hookData;
    mbObjStarTevStageSet(drawObj, matP, &tevStage, &texCoord);
    HuSprTexLoad(work->anim, 0, GX_TEXMAP4, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    PSMTXInverse(Hu3DCameraMtx, mtx);
    PSMTXConcat(mtx, drawObj->matrix, mtx);
    PSMTXTrans(mtxTemp, -work->pos.x, -work->pos.y, -work->pos.z);
    PSMTXConcat(mtxTemp, mtx, mtx);
    mbMtxRot(mtxTemp, work->rot.x, work->rot.y, work->rot.z);
    PSMTXInverse(mtxTemp, mtxTemp);
    PSMTXConcat(mtxTemp, mtx, mtx);
    scale = work->scale;
    if(scale < 0.000001f) {
        scale = 0.000001f;
    }
    PSMTXScale(mtxTemp, 0.001f, -0.01f / scale, 1.0f);
    PSMTXConcat(mtxTemp, mtx, mtx);
    mtx[1][3] += 0.96875f;
    GXLoadTexMtxImm(mtx, GX_TEXMTX4, GX_MTX2x4);
    GXSetNumTexGens(texCoord + 1);
    GXSetNumTevStages(tevStage + 1);
    GXSetTexCoordGen(texCoord, GX_TG_MTX2x4, GX_TG_POS, GX_TEXMTX4);
    GXSetTevOrder(tevStage, texCoord, GX_TEXMAP4, GX_COLOR_NULL);
    GXSetTevKColor(GX_KCOLOR3, work->color);
    GXSetTevKColorSel(tevStage, GX_TEV_KCSEL_K3);
    GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, GX_CC_CPREV);
    GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_TEXA, GX_CA_APREV, GX_CA_ZERO);
    GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    if((drawObj->object->flags | matP->flags) & 0x1200) {
        GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_GEQUAL, 0x80);
    } else {
        GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    }
}

void mbObjFadeTexRotSet(s16 objId, HuVecF *pos, HuVecF *rot)
{
    int mdlId;
    MBOBJFADEWORK *work;
    HU3D_MODEL *modelP;

    mdlId = mbObjModelIDGet((MBOBJID)objId);
    modelP = &Hu3DData[mdlId];
    work = modelP->hookData;
    mbVecCopy(pos, &work->pos);
    mbVecCopy(rot, &work->rot);
}

void mbObjFadeTexColorSet(s16 objId, u8 r, u8 g, u8 b, float scale)
{
    int mdlId;
    MBOBJFADEWORK *work;
    HU3D_MODEL *modelP;

    mdlId = mbObjModelIDGet((MBOBJID)objId);
    modelP = &Hu3DData[mdlId];
    work = modelP->hookData;
    work->color.r = (u8)r;
    work->color.g = (u8)g;
    work->color.b = (u8)b;
    work->scale = scale;
}

static void MetalMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *matP);

void mbObjMetalCreate(s16 objId)
{
    int mdlId;
    MBOBJMETALWORK *work;
    HU3D_MODEL *modelP;
    HSF_DATA *hsf;
    int i;

    mdlId = mbObjModelIDGet((MBOBJID)objId);
    modelP = &Hu3DData[mdlId];
    if(modelP->attr & HU3D_ATTR_LINK) {
        modelP->hookData = work = mbMallocNum(sizeof(MBOBJMETALWORK), modelP->mallocNoLink);
    } else {
        modelP->hookData = work = mbMallocNum(sizeof(MBOBJMETALWORK), modelP->mallocNo);
    }
    work->magic = MBOBJMETAL_MAGIC;
    work->tpLvl = 1.0f;
    work->shadowColor.r = work->shadowColor.g = work->shadowColor.b = 255;
    work->hiliteColor.r = work->hiliteColor.g = work->hiliteColor.b = 255;
    work->shadowColor.r = 0x81;
    work->shadowColor.g = 0xFF;
    work->shadowColor.b = 0xAE;
    work->hiliteColor.r = 0xCA;
    work->hiliteColor.g = 0x57;
    work->hiliteColor.b = 0xFF;
    Hu3DModelMatHookSet(mdlId, MetalMatHook);
    hsf = modelP->hsf;
    for(i = 0; i < hsf->materialNum; i++) {
        hsf->material[i].flags |= HSF_MATERIAL_MATHOOK;
    }
    work->anim = HuSprAnimRead(HuDataSelHeapReadNum(mbBoardDataNumGet(0x50069), HU_MEMNUM_OVL, HEAP_MODEL));
    work->hiliteAnim = HuSprAnimRead(HuDataSelHeapReadNum(mbBoardDataNumGet(0x50068), HU_MEMNUM_OVL, HEAP_MODEL));
}

BOOL mbObjMetalKill(s16 objId)
{
    int mdlId;
    MBOBJMETALWORK *work;
    HU3D_MODEL *modelP;
    HSF_DATA *hsf;
    int i;

    mdlId = mbObjModelIDGet((MBOBJID)objId);
    modelP = &Hu3DData[mdlId];
    if(modelP->hookData == NULL) {
        return FALSE;
    }
    work = modelP->hookData;
    if(work->magic != MBOBJMETAL_MAGIC) {
        return FALSE;
    }
    Hu3DModelMatHookSet(mdlId, NULL);
    hsf = modelP->hsf;
    for(i = 0; i < hsf->materialNum; i++) {
        hsf->material[i].flags &= ~HSF_MATERIAL_MATHOOK;
    }
    HuSprAnimKill(work->anim);
    HuSprAnimKill(work->hiliteAnim);
    HuMemDirectFree(work);
    modelP->hookData = NULL;
    return TRUE;
}

void mbObjMetalTPLvlSet(s16 objId, float tpLvl)
{
    int mdlId;
    MBOBJMETALWORK *work;
    HU3D_MODEL *modelP;

    mdlId = mbObjModelIDGet((MBOBJID)objId);
    modelP = &Hu3DData[mdlId];
    work = modelP->hookData;
    work->tpLvl = tpLvl;
}

void mbObjMetalColorSet(s16 objId, GXColor shadowColor, GXColor hiliteColor)
{
    int mdlId;
    MBOBJMETALWORK *work;
    HU3D_MODEL *modelP;

    mdlId = mbObjModelIDGet((MBOBJID)objId);
    modelP = &Hu3DData[mdlId];
    work = modelP->hookData;
    work->shadowColor = shadowColor;
    work->hiliteColor = hiliteColor;
}

static inline float SNpcFabs(register float x)
{
    asm {
        fabs x, x
    }
    return x;
}

static void MetalMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *matP)
{
    Mtx mtx;
    Mtx mtxTemp;
    HuVecF halfDir;
    HuVecF axis;
    u32 tevStage;
    u32 texCoord;
    GXColor color;
    MBOBJMETALWORK *work;
    float angle;

    work = drawObj->model->hookData;
    mbObjStarTevStageSet(drawObj, matP, &tevStage, &texCoord);
    if(!(work->tpLvl <= 0.05f)) {
        HuSprTexLoad(work->anim, 0, GX_TEXMAP4, GX_REPEAT, GX_REPEAT, GX_LINEAR);
        HuSprTexLoad(work->hiliteAnim, 0, GX_TEXMAP5, GX_REPEAT, GX_REPEAT, GX_LINEAR);
        PSMTXCopy(drawObj->matrix, mtx);
        PSMTXScale(mtxTemp, 0.5f / drawObj->scale.x, -0.5f / drawObj->scale.y, 0.5f / drawObj->scale.z);
        PSMTXConcat(mtxTemp, mtx, mtx);
        mtx[0][3] = mtx[1][3] = 0.5f;
        GXLoadTexMtxImm(mtx, GX_TEXMTX4, GX_MTX2x4);
        PSMTXCopy(drawObj->matrix, mtx);
        {
            HuVecF baseDir = { 0.0f, 0.0f, -1.0f };
            PSMTXMultVecSR(Hu3DCameraMtx, &Hu3DGlobalLight[0].dir, &halfDir);
            C_VECHalfAngle(&baseDir, &halfDir, &halfDir);
            if(SNpcFabs(halfDir.z) < 0.999f) {
                angle = acosf(halfDir.z);
                PSVECCrossProduct(&baseDir, &halfDir, &axis);
                PSMTXRotAxisRad(mtxTemp, &axis, angle);
                PSMTXConcat(mtxTemp, mtx, mtx);
            }
        }
        PSMTXScale(mtxTemp, 0.5f / drawObj->scale.x, -0.5f / drawObj->scale.y, 0.5f / drawObj->scale.z);
        PSMTXConcat(mtxTemp, mtx, mtx);
        mtx[0][3] = 0.5f;
        mtx[1][3] = 0.5f;
        GXLoadTexMtxImm(mtx, GX_TEXMTX5, GX_MTX2x4);
        GXSetNumTexGens(texCoord + 2);
        GXSetNumTevStages(tevStage + 3);
        GXSetTexCoordGen(texCoord, GX_TG_MTX2x4, GX_TG_NRM, GX_TEXMTX4);
        GXSetTexCoordGen(texCoord + 1, GX_TG_MTX2x4, GX_TG_NRM, GX_TEXMTX5);
        GXSetTevOrder(tevStage, texCoord, GX_TEXMAP4, GX_COLOR_NULL);
        GXSetTevKColorSel(tevStage, kColorTbl[(int)(7.9f * (1.0f - work->tpLvl))]);
        GXSetTevColorIn(tevStage, GX_CC_CPREV, GX_CC_TEXC, GX_CC_KONST, GX_CC_ZERO);
        GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        tevStage++;
        GXSetTevOrder(tevStage, texCoord + 1, GX_TEXMAP5, GX_COLOR_NULL);
        color.r = work->shadowColor.r * work->tpLvl;
        color.g = work->shadowColor.g * work->tpLvl;
        color.b = work->shadowColor.b * work->tpLvl;
        GXSetTevKColor(GX_KCOLOR2, color);
        GXSetTevKColorSel(tevStage, GX_TEV_KCSEL_K2);
        GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, GX_CC_CPREV);
        GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
        tevStage++;
        GXSetTevOrder(tevStage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        color.r = work->hiliteColor.r * work->tpLvl;
        color.g = work->hiliteColor.g * work->tpLvl;
        color.b = work->hiliteColor.b * work->tpLvl;
        GXSetTevKColor(GX_KCOLOR3, color);
        GXSetTevKColorSel(tevStage, GX_TEV_KCSEL_K3);
        GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_A0, GX_CC_KONST, GX_CC_CPREV);
        GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    }
}

static void BiriQMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *matP);

void mbObjBiriQCreate(s16 objId)
{
    int mdlId;
    MBOBJBIRIQWORK *work;
    HU3D_MODEL *modelP;
    HSF_DATA *hsf;
    int i;

    mdlId = mbObjModelIDGet((MBOBJID)objId);
    modelP = &Hu3DData[mdlId];
    if(modelP->attr & HU3D_ATTR_LINK) {
        modelP->hookData = work = mbMallocNum(sizeof(MBOBJBIRIQWORK), modelP->mallocNoLink);
    } else {
        modelP->hookData = work = mbMallocNum(sizeof(MBOBJBIRIQWORK), modelP->mallocNo);
    }
    work->magic = MBOBJBIRIQ_MAGIC;
    work->alpha = 0.0f;
    work->color.r = work->color.g = work->color.b = work->color.a = 255;
    Hu3DModelMatHookSet(mdlId, BiriQMatHook);
    hsf = modelP->hsf;
    for(i = 0; i < hsf->materialNum; i++) {
        hsf->material[i].flags |= HSF_MATERIAL_MATHOOK;
    }
}

BOOL mbObjBiriQKill(s16 objId)
{
    int mdlId;
    MBOBJBIRIQWORK *work;
    HU3D_MODEL *modelP;
    HSF_DATA *hsf;
    int i;

    mdlId = mbObjModelIDGet((MBOBJID)objId);
    modelP = &Hu3DData[mdlId];
    if(modelP->hookData == NULL) {
        return FALSE;
    }
    work = modelP->hookData;
    if(work->magic != MBOBJBIRIQ_MAGIC) {
        return FALSE;
    }
    Hu3DModelMatHookSet(mdlId, NULL);
    hsf = modelP->hsf;
    for(i = 0; i < hsf->materialNum; i++) {
        hsf->material[i].flags &= ~HSF_MATERIAL_MATHOOK;
    }
    HuMemDirectFree(work);
    modelP->hookData = NULL;
    return TRUE;
}

void mbObjBiriQColorSet(s16 objId, int mode, float alpha, GXColor color)
{
    int mdlId;
    MBOBJBIRIQWORK *work;
    HU3D_MODEL *modelP;

    mdlId = mbObjModelIDGet((MBOBJID)objId);
    modelP = &Hu3DData[mdlId];
    if(modelP->hookData != NULL) {
        work = modelP->hookData;
        if(work->magic == MBOBJBIRIQ_MAGIC) {
            work->mode = mode;
            work->alpha = alpha;
            work->color = color;
        }
    }
}

static void BiriQMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *matP)
{
    u32 tevStage;
    u32 texCoord;
    GXColor color;
    MBOBJBIRIQWORK *work;
    BIRIQMAT *matTbl;
    int i;

    work = drawObj->model->hookData;
    mbObjStarTevStageSet(drawObj, matP, &tevStage, &texCoord);
    if(!(work->alpha <= 0.01f)) {
        GXSetNumTevStages(tevStage + biriQMatNumTbl[work->mode]);
        switch(work->mode) {
            case 0:
            case 3:
                color.r = work->color.r;
                color.g = work->color.g;
                color.b = work->color.b;
                break;

            case 1:
            case 2:
                color.r = work->color.r * work->alpha;
                color.g = work->color.g * work->alpha;
                color.b = work->color.b * work->alpha;
                break;
        }
        color.a = 255.0f * work->alpha;
        GXSetTevKColor(GX_KCOLOR3, color);
        GXSetTevKColorSel(tevStage, GX_TEV_KCSEL_K3);
        GXSetTevKAlphaSel(tevStage, GX_TEV_KASEL_K3_A);
        matTbl = &biriQMatTbl[work->mode * 4];
        for(i = 0; i < biriQMatNumTbl[work->mode]; i++, tevStage++) {
            GXSetTevOrder(tevStage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
            GXSetTevColorOp(tevStage, matTbl->op, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, matTbl->outReg);
            GXSetTevColorIn(tevStage, matTbl->a, matTbl->b, matTbl->c, matTbl->d);
            matTbl++;
            GXSetTevAlphaOp(tevStage, matTbl->op, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, matTbl->outReg);
            GXSetTevAlphaIn(tevStage, matTbl->a, matTbl->b, matTbl->c, matTbl->d);
            matTbl++;
        }
    }
}
/* @@END@@ */
