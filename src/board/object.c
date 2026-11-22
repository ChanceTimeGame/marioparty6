#include "game/board/object.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/exit.h"
#include "game/board/malloc.h"
#include "game/board/math.h"

#include "game/charman.h"
#include "game/data.h"
#include "game/gamework.h"
#include "game/memory.h"

static OMOBJ *objManOMObj;
static MBOBJ *objManData;
static s16 objManNum;
static int objManDispNum;
static int dataDirDay;
static int dataDirNight;

static inline int ObjManDataNumConv(int dataNum)
{
    if(!GwSystem.curTime) {
        if((dataNum & 0xFFFF0000) == dataDirNight) {
            return dataDirDay | (dataNum & 0xFFFF);
        }
    } else {
        if((dataNum & 0xFFFF0000) == dataDirDay) {
            return dataDirNight | (dataNum & 0xFFFF);
        }
    }
    return dataNum;
}

int mbObjDataNumGet(int dataNum)
{
    return ObjManDataNumConv(dataNum);
}

static void ObjManOMExec(OMOBJ *obj);

void mbObjInit(void)
{
    objManNum = 0;
    dataDirDay = dataDirNight = 0xFFFF0000;
    objManData = (MBOBJ *)mbMalloc(MB_OBJ_MAX*sizeof(MBOBJ));
    objManOMObj = omAddObjEx(mbObjMan, 32259, 0, 0, -1, ObjManOMExec);
    omSetStatBit(objManOMObj, OM_STAT_NOPAUSE|OM_STAT_SPRPAUSE);
}

void mbObjClose(void)
{
    int i;
    for(i=1; i<MB_OBJ_MAX; i++) {
        if(objManData[i].objId != 0) {
            mbObjKill(i);
        }
    }
    if(objManData) {
        void *data = objManData;
        HuMemDirectFree(data);
        objManData = NULL;
    }
}

static void ObjManOMExec(OMOBJ *obj)
{
    MBOBJ *objP;
    int i;
    HuVecF pos;
    HuVecF pos2D;
    u32 attr;
    float tpLvl;
    float t;
    if(mbExitCheck()) {
        objManOMObj = NULL;
        omDelObjEx((OMOBJMAN *)HuPrcCurrentGet(), obj);
        return;
    }
    objP = &objManData[1];
    objManDispNum = 0;
    for(i=1; i<MB_OBJ_MAX; i++, objP++) {
        if(objP->objId == 0) {
            continue;
        }
        if(objP->cullRadius >= 0) {
            pos = objP->pos;
            PSVECAdd(&pos, &objP->offset, &pos);
            if(!mbCameraCullCheck(&pos, objP->cullRadius)) {
                continue;
            }
        }
        if(objP->fadeF) {
            mbPos3Dto2D(&pos, &pos2D);
            if(pos2D.z < 1000) {
                t = pos2D.z/1000;
                if(t < 0) {
                    t = 0;
                }
                tpLvl = 1.0-HuCos(90*t);
            } else {
                tpLvl = 1;
            }
            Hu3DModelTPLvlSet(objP->modelId, tpLvl*(objP->alpha/255.0f));
        }
        attr = Hu3DModelAttrGet(objP->modelId);
        if(!objP->disp) {
            Hu3DModelAttrSet(objP->modelId, HU3D_ATTR_DISPOFF);
        } else {
            Hu3DModelAttrReset(objP->modelId, HU3D_ATTR_DISPOFF);
            objManDispNum++;
        }
        Hu3DModelPosSet(objP->modelId, objP->pos.x+objP->offset.x, objP->pos.y+objP->offset.y, objP->pos.z+objP->offset.z);
        Hu3DModelRotSetV(objP->modelId, &objP->rot);
        Hu3DModelScaleSetV(objP->modelId, &objP->scale);
    }
}

static BOOL ObjManModelCreate(MBOBJ *objP, int dataNum, BOOL linkF);
static BOOL ObjManMotionCreate(MBOBJ *objP, int motNum, int *motDataNum);

static MBOBJID ObjManObjCreate(int charNo, int dataNum, int *motDataNum, BOOL linkF)
{
    MBOBJ *objP;
    int i;
    int motNum;
    BOOL result;
    HU3D_MODEL *modelP;
    objP = &objManData[1];
    for(i=1; i<MB_OBJ_MAX; i++, objP++) {
        if(objP->objId == 0) {
            break;
        }
    }
    if(i >= MB_OBJ_MAX) {
        return MB_OBJ_NONE;
    }
    memset(objP, 0, sizeof(MBOBJ));
    objP->objId = i;
    objP->charNo = charNo;
    result = ObjManModelCreate(objP, dataNum, linkF);
    motNum = 0;
    if(motDataNum != NULL) {
        while(motDataNum[motNum] >= 0) {
            motNum++;
        }
    }
    for(i=0; i<MB_OBJ_MOT_MAX; i++) {
        objP->motId[i] = HU3D_MOTIONID_NONE;
    }
    result = ObjManMotionCreate(objP, motNum, motDataNum);
    objP->disp = TRUE;
    objP->dispOld = objP->disp;
    objP->cameraBit = HU3D_CAM0;
    objP->layer = 0;
    objP->cullRadius = -1;
    objP->alpha = 255;
    Hu3DModelCameraSet(objP->modelId, objP->cameraBit);
    Hu3DModelLayerSet(objP->modelId, objP->layer);
    mbObjPosSet(objP->objId, 0, 0, 0);
    mbObjRotSet(objP->objId, 0, 0, 0);
    mbObjScaleSet(objP->objId, 1, 1, 1);
    mbObjOffsetSet(objP->objId, 0, 0, 0);
    mbObjAmbSet(objP->objId, 1, 1, 1);
    objManNum++;
    modelP = &Hu3DData[objP->modelId];
    if(modelP->linkMdlId < 0) {
        mbObjCullInit(objP->objId);
    }
    return objP->objId;
}

static MBOBJ *ObjManLinkSearch(int dataNum);

MBOBJID mbObjCreate(int dataNum, int *motDataNum, BOOL linkF)
{
    MBOBJID objId;
    if(linkF && !ObjManLinkSearch(dataNum)) {
        objId = ObjManObjCreate(-1, dataNum, motDataNum, linkF);
        mbObjDispSet(objId, FALSE);
    }
    return ObjManObjCreate(-1, dataNum, motDataNum, linkF);
}

MBOBJID mbObjCharCreate(int charNo, int dataNum, int *motDataNum, BOOL linkF)
{
    MBOBJID objId = ObjManObjCreate(charNo, dataNum, motDataNum, linkF);
    mbObjLayerSet(objId, 3);
    return objId;
}

static BOOL ObjManModelCreate(MBOBJ *objP, int dataNum, BOOL linkF)
{
    MBOBJ *linkP = NULL;
    void *data;
    objP->linkCnt = 0;
    if(linkF && objP->charNo == -1) {
        linkP = ObjManLinkSearch(dataNum);
        if(linkP) {
            objP->modelId = Hu3DModelLink(linkP->modelId);
            objP->dataNum = 0;
            objP->linkObj = linkP;
            linkP->linkCnt++;
            return TRUE;
        }
    }
    objP->linkObj = NULL;
    if(objP->charNo == -1) {
        data = HuDataSelHeapReadNum(ObjManDataNumConv(dataNum), HU_MEMNUM_OVL, HEAP_MODEL);
        if(!data) {
            u32 restModel = HuRestMemGet(HEAP_MODEL);
            u32 restData = HuRestMemGet(HEAP_DVD);
        }
    } else {
        data = NULL;
    }
    if(objP->charNo == -1) {
        objP->modelId = Hu3DModelCreate(data);
    } else {
        objP->modelId = CharModelCreate(objP->charNo, 4);
    }
    if(!linkF) {
        objP->dataNum = 0;
    } else {
        objP->dataNum = dataNum;
    }
    return TRUE;
}

static BOOL ObjManMotionCreate(MBOBJ *objP, int motNum, int *motDataNum)
{
    void *data;
    HU3D_MOTIONID motId;
    int i;
    objP->motId[0] = Hu3DData[objP->modelId].motId;
    if(motDataNum == NULL) {
        objP->motNum = 1;
        return TRUE;
    }
    data = NULL;
    for(i=0; i<motNum; i++) {
        if(objP->charNo == -1) {
            int dataNum = ObjManDataNumConv(motDataNum[i]);
            data = HuDataSelHeapReadNum(dataNum, HU_MEMNUM_OVL, HEAP_MODEL);
            if(data == NULL) {
                return -1;
            }
            motId = Hu3DJointMotion(objP->modelId, data);
        } else {
            motId = CharMotionCreate(objP->charNo, motDataNum[i]);
        }
        if(motId < 0) {
            return -1;
        }
        objP->motId[i+1] = motId;
        objP->motData[i+1] = data;
        objP->motDataNum[i+1] = motDataNum[i];
    }
    objP->motNum = motNum+1;
    return TRUE;
}

static MBOBJ *ObjManLinkSearch(int dataNum)
{
    MBOBJ *objP = &objManData[1];
    int i;
    for(i=1; i<MB_OBJ_MAX; i++, objP++) {
        if(objP->objId != 0 && objP->dataNum == dataNum) {
            return objP;
        }
    }
    return NULL;
}

void mbObjKill(MBOBJID objId)
{
    MBOBJ *objP = &objManData[objId];
    int i;
    if(objP->charNo == -1) {
        if(objP->linkCnt != 0) {
            MBOBJ *linkP = &objManData[1];
            for(i=1; i<MB_OBJ_MAX; i++, linkP++) {
                if(linkP->objId != 0 && linkP->linkObj == objP) {
                    mbObjKill(linkP->objId);
                }
            }
            return;
        }
        objP->motId[0] = HU3D_MOTIONID_NONE;
        if(objP->modelId >= 0) {
            Hu3DModelKill(objP->modelId);
            objP->modelId = HU3D_MODELID_NONE;
        }
        for(i=1; i<MB_OBJ_MOT_MAX; i++) {
            if(objP->motId[i] >= 0) {
                Hu3DMotionKill(objP->motId[i]);
                objP->motId[i] = HU3D_MOTIONID_NONE;
            }
            objP->motData[i] = NULL;
        }
        objP->dataNum = 0;
        if(objP->linkObj) {
            if(--objP->linkObj->linkCnt == 0) {
                mbObjKill(objP->linkObj->objId);
            }
        }
    } else {
        for(i=0; i<MB_OBJ_MOT_MAX; i++) {
            objP->motId[i] = HU3D_MOTIONID_NONE;
            objP->motData[i] = NULL;
        }
        if(objP->modelId >= 0) {
            objP->modelId = HU3D_MODELID_NONE;
        }
        CharModelKill(objP->charNo);
        objP->charNo = -1;
    }
    objManNum--;
    objP->objId = 0;
    objP->disp = FALSE;
}

void mbObjDirSet(int dirDay, int dirNight)
{
    dataDirDay = dirDay;
    dataDirNight = dirNight;
}

MBOBJ *mbObjGet(MBOBJID objId)
{
    return &objManData[objId];
}

BOOL mbObjDispGet(MBOBJID objId)
{
    return objManData[objId].disp;
}

void mbObjDispSet(MBOBJID objId, BOOL disp)
{
    objManData[objId].disp = disp;
}

void mbObjCameraSet(MBOBJID objId, u16 cameraBit)
{
    MBOBJ *objP = &objManData[objId];
    objP->cameraBit = cameraBit;
    Hu3DModelCameraSet(objP->modelId, cameraBit);
}

u8 mbObjLayerGet(MBOBJID objId)
{
    return objManData[objId].layer;
}

void mbObjLayerSet(MBOBJID objId, u8 layer)
{
    MBOBJ *objP = &objManData[objId];
    objP->layer = layer;
    Hu3DModelLayerSet(objP->modelId, layer);
}

void mbObjLayerSetAll(u8 layer)
{
    MBOBJ *objP = &objManData[1];
    int i;
    for(i=1; i<MB_OBJ_MAX; i++, objP++) {
        if(objP->objId != 0) {
            Hu3DModelLayerSet(objP->modelId, layer);
        }
    }
}

HU3D_MODELID mbObjModelIDGet(MBOBJID objId)
{
    return objManData[objId].modelId;
}

void mbObjPosGet(MBOBJID objId, HuVecF *pos)
{
    *pos = objManData[objId].pos;
}

void mbObjPosSetV(MBOBJID objId, HuVecF *pos)
{
    mbObjPosSet(objId, pos->x, pos->y, pos->z);
}

void mbObjPosSet(MBOBJID objId, float x, float y, float z)
{
    MBOBJ *objP = &objManData[objId];
    objP->pos.x = x;
    objP->pos.y = y;
    objP->pos.z = z;
    Hu3DModelPosSet(objP->modelId, objP->pos.x+objP->offset.x, objP->pos.y+objP->offset.y, objP->pos.z+objP->offset.z);
}

void mbObjOffsetGet(MBOBJID objId, HuVecF *offset)
{
    *offset = objManData[objId].offset;
}

void mbObjOffsetSetV(MBOBJID objId, HuVecF *offset)
{
    mbObjOffsetSet(objId, offset->x, offset->y, offset->z);
}

void mbObjOffsetSet(MBOBJID objId, float x, float y, float z)
{
    MBOBJ *objP = &objManData[objId];
    HuVecF pos;
    objP->offset.x = x;
    objP->offset.y = y;
    objP->offset.z = z;
    mbObjPosGet(objId, &pos);
    mbObjPosSet(objId, pos.x, pos.y, pos.z);
}

void mbObjRotGet(MBOBJID objId, HuVecF *rot)
{
    *rot = objManData[objId].rot;
}

void mbObjRotSetV(MBOBJID objId, HuVecF *rot)
{
    mbObjRotSet(objId, rot->x, rot->y, rot->z);
}

void mbObjRotSet(MBOBJID objId, float x, float y, float z)
{
    MBOBJ *objP = &objManData[objId];
    objP->rot.x = x;
    objP->rot.y = y;
    objP->rot.z = z;
    Hu3DModelRotSetV(objP->modelId, &objP->rot);
}

float mbObjRotYGet(MBOBJID objId)
{
    return objManData[objId].rot.y;
}

void mbObjRotYSet(MBOBJID objId, float rotY)
{
    HuVecF rot;
    mbObjRotGet(objId, &rot);
    mbObjRotSet(objId, rot.x, rotY, rot.z);
}

void mbObjScaleGet(MBOBJID objId, HuVecF *scale)
{
    *scale = objManData[objId].scale;
}

void mbObjScaleSetV(MBOBJID objId, HuVecF *scale)
{
    mbObjScaleSet(objId, scale->x, scale->y, scale->z);
}

void mbObjScaleSet(MBOBJID objId, float x, float y, float z)
{
    MBOBJ *objP = &objManData[objId];
    objP->scale.x = x;
    objP->scale.y = y;
    objP->scale.z = z;
    Hu3DModelScaleSetV(objP->modelId, &objP->scale);
}

void mbObjMtxGet(MBOBJID objId, Mtx mtx)
{
    MBOBJ *objP = &objManData[objId];
    HU3D_MODEL *modelP = &Hu3DData[objP->modelId];
    PSMTXCopy(modelP->mtx, mtx);
}

void mbObjMtxSet(MBOBJID objId, Mtx mtx)
{
    MBOBJ *objP = &objManData[objId];
    HU3D_MODEL *modelP = &Hu3DData[objP->modelId];
    PSMTXCopy(mtx, modelP->mtx);
}

void mbObjAttrSet(MBOBJID objId, u32 attr)
{
    MBOBJ *objP = &objManData[objId];
    Hu3DModelAttrSet(objP->modelId, attr);
}

void mbObjAttrReset(MBOBJID objId, u32 attr)
{
    MBOBJ *objP = &objManData[objId];
    Hu3DModelAttrReset(objP->modelId, attr);
}

void mbObjAmbSet(MBOBJID objId, float r, float g, float b)
{
    Hu3DModelAmbSet(objManData[objId].modelId, r, g, b);
}

void mbObjAmbSetAll(float r, float g, float b)
{
    MBOBJ *objP = &objManData[1];
    int i;
    for(i=1; i<MB_OBJ_MAX; i++, objP++) {
        if(objP->objId != 0) {
            mbObjAmbSet(objP->objId, r, g, b);
        }
    }
}

u8 mbObjAlphaGet(MBOBJID objId)
{
    return objManData[objId].alpha;
}

void mbObjAlphaSet(MBOBJID objId, u8 alpha)
{
    MBOBJ *objP = &objManData[objId];
    objP->alpha = alpha;
    Hu3DModelTPLvlSet(objP->modelId, alpha/255.0f);
}

void mbObjFadeSet(MBOBJID objId, BOOL fadeF)
{
    objManData[objId].fadeF = fadeF;
}

void mbObjZWriteOffSet(MBOBJID objId, BOOL zWriteOff)
{
    MBOBJ *objP = &objManData[objId];
    if(zWriteOff) {
        Hu3DModelAttrSet(objP->modelId, HU3D_ATTR_ZWRITE_OFF);
    } else {
        Hu3DModelAttrReset(objP->modelId, HU3D_ATTR_ZWRITE_OFF);
    }
}

void mbObjCullRadiusSet(MBOBJID objId, float radius)
{
    if(radius < 0) {
        radius = -1;
    }
    objManData[objId].cullRadius = radius;
}

void mbObjHookSet(MBOBJID objId, char *objName, MBOBJID hookObjId)
{
    MBOBJ *objP = &objManData[objId];
    MBOBJ *hookObjP = &objManData[hookObjId];
    Hu3DModelHookSet(objP->modelId, objName, hookObjP->modelId);
    mbObjPosSet(hookObjId, 0, 0, 0);
}

void mbObjHookReset(MBOBJID objId)
{
    MBOBJ *objP = &objManData[objId];
    Hu3DModelHookReset(objP->modelId);
}

void mbObjHookObjReset(MBOBJID objId, char *objName)
{
    MBOBJ *objP = &objManData[objId];
    Hu3DModelHookObjReset(objP->modelId, objName);
}

void mbObjSePlay(MBOBJID objId, s16 seId)
{
    MBOBJ *objP = &objManData[objId];
    mbAudFXPosPlay(seId, &objP->pos);
}

void mbObjMotionVoiceOnSet(MBOBJID objId, int motNo, BOOL voiceOn)
{
    MBOBJ *objP = &objManData[objId];
    if(objP->charNo != -1) {
        CharMotionVoiceOnSet(objP->charNo, objP->motDataNum[motNo], voiceOn);
    }
}

int mbObjMotionGet(MBOBJID objId)
{
    MBOBJ *objP = &objManData[objId];
    return objP->motNo;
}

void mbObjMotionSet(MBOBJID objId, int motNo, u32 attr)
{
    MBOBJ *objP = &objManData[objId];
    if(objP->charNo == -1) {
        Hu3DMotionSet(objP->modelId, objP->motId[motNo]);
    } else {
        CharMotionSet(objP->charNo, objP->motId[motNo]);
    }
    Hu3DMotionSpeedSet(objP->modelId, 1);
    objP->motStart = 0;
    if(objP->charNo == -1) {
        objP->motEnd = Hu3DMotionMaxTimeGet(objP->modelId);
    } else {
        objP->motEnd = CharMotionMaxTimeGet(objP->charNo);
    }
    if(attr & (HU3D_MOTATTR_REV|HU3D_MOTATTR_SHIFT_REV)) {
        if(objP->charNo == -1) {
            Hu3DMotionTimeSet(objP->modelId, objP->motEnd);
        } else {
            CharMotionTimeSet(objP->charNo, objP->motEnd);
        }
    }
    Hu3DModelAttrReset(objP->modelId, HU3D_MOTATTR_LOOP|HU3D_MOTATTR_PAUSE);
    Hu3DModelAttrSet(objP->modelId, attr);
    objP->motNo = motNo;
}

int mbObjMotionIDCurGet(MBOBJID objId)
{
    return mbObjMotionIDGet(objId, mbObjMotionGet(objId));
}

int mbObjMotionIDGet(MBOBJID objId, int motNo)
{
    MBOBJ *objP = &objManData[objId];
    return objP->motId[motNo];
}

void mbObjMotionShiftSet(MBOBJID objId, int motNo, float start, float end, u32 attr)
{
    MBOBJ *objP = &objManData[objId];
    objP->motStart = 0;
    if(objP->charNo == -1) {
        objP->motEnd = Hu3DMotionMaxTimeGet(objP->modelId);
    } else {
        objP->motEnd = CharMotionMaxTimeGet(objP->charNo);
    }
    if(objP->charNo == -1) {
        Hu3DMotionShiftSet(objP->modelId, objP->motId[motNo], start, end, attr);
    } else {
        CharMotionShiftSet(objP->charNo, objP->motId[motNo], start, end, attr);
    }
    objP->motNo = motNo;
}

int mbObjMotionShiftIDGet(MBOBJID objId)
{
    MBOBJ *objP = &objManData[objId];
    return Hu3DMotionShiftIDGet(objP->modelId);
}

void mbObjMotionNoCreate(MBOBJID objId, int dataNum, int motNo)
{
    MBOBJ *objP = &objManData[objId];
    if(objP->charNo == -1) {
        objP->motData[motNo] = HuDataSelHeapReadNum(ObjManDataNumConv(dataNum), HU_MEMNUM_OVL, HEAP_MODEL);
        objId = Hu3DJointMotion(objP->modelId, objP->motData[motNo]);
    } else {
        objId = CharMotionCreate(objP->charNo, dataNum);
        CharMotionDataClose(objP->charNo);
    }
    objP->motId[motNo] = objId;
    objP->motNum++;
}

int mbObjMotionCreate(MBOBJID objId, int dataNum)
{
    MBOBJ *objP = &objManData[objId];
    int motNo;
    for(motNo=1; motNo<MB_OBJ_MOT_MAX; motNo++) {
        if(objP->motId[motNo] == HU3D_MOTIONID_NONE) {
            break;
        }
    }
    if(objP->charNo == -1) {
        objP->motData[motNo] = HuDataSelHeapReadNum(ObjManDataNumConv(dataNum), HU_MEMNUM_OVL, HEAP_MODEL);
        objId = Hu3DJointMotion(objP->modelId, objP->motData[motNo]);
    } else {
        objId = CharMotionCreate(objP->charNo, dataNum);
        CharMotionDataClose(objP->charNo);
    }
    objP->motId[motNo] = objId;
    objP->motNum++;
    return motNo;
}

void mbObjMotionKill(MBOBJID objId, int motNo)
{
    MBOBJ *objP = &objManData[objId];
    if(motNo == objP->motNo && objP->motNo != 0) {
        mbObjMotionSet(objId, 0, HU3D_MOTATTR_NONE);
    }
    if(objP->motId[motNo] >= 0) {
        if(objP->charNo == -1) {
            Hu3DMotionKill(objP->motId[motNo]);
        } else {
            CharMotionKill(objP->charNo, objP->motId[motNo]);
        }
        objP->motId[motNo] = HU3D_MOTIONID_NONE;
    }
    objP->motData[motNo] = NULL;
    objP->motNum--;
}

float mbObjMotionTimeGet(MBOBJID objId)
{
    MBOBJ *objP = &objManData[objId];
    float time;
    if(objP->charNo == -1) {
        time = Hu3DData[objP->modelId].motWork.time;
    } else {
        time = CharMotionTimeGet(objP->charNo);
    }
    return time;
}

void mbObjMotionTimeSet(MBOBJID objId, float time)
{
    MBOBJ *objP = &objManData[objId];
    if(objP->charNo == -1) {
        Hu3DMotionTimeSet(objP->modelId, time);
    } else {
        CharMotionTimeSet(objP->charNo, time);
    }
}

float mbObjMotionMaxTimeGet(MBOBJID objId)
{
    MBOBJ *objP = &objManData[objId];
    float maxTime;
    if(objP->charNo == -1) {
        maxTime = Hu3DMotionMaxTimeGet(objP->modelId);
    } else {
        maxTime = CharMotionMaxTimeGet(objP->charNo);
    }
    return maxTime;
}

float mbObjMotionSpeedGet(MBOBJID objId)
{
    HU3D_MODEL *modelP;
    MBOBJ *objP = &objManData[objId];
    modelP = &Hu3DData[objP->modelId];
    return modelP->motWork.speed;
}

void mbObjMotionSpeedSet(MBOBJID objId, float speed)
{
    MBOBJ *objP = &objManData[objId];
    HU3D_MODEL *modelP;
    if(objP->charNo == -1) {
        Hu3DMotionSpeedSet(objP->modelId, speed);
    } else {
        CharMotionSpeedSet(objP->charNo, speed);
    }
    modelP = &Hu3DData[objP->modelId];
    modelP->motShiftWork.speed = speed;
}

BOOL mbObjMotionEndCheck(MBOBJID objId)
{
    MBOBJ *objP = &objManData[objId];
    BOOL result;
    if(objP->charNo == -1) {
        result = Hu3DMotionEndCheck(objP->modelId);
    } else {
        result = CharMotionEndCheck(objP->charNo);
    }
    return result;
}

void mbObjMotionStartEndSet(MBOBJID objId, s16 start, s16 end)
{
    MBOBJ *objP = &objManData[objId];
    if(start >= 0) {
        objP->motStart = start;
    }
    if(end >= 0) {
        objP->motEnd = end;
    }
    Hu3DMotionStartEndSet(objP->modelId, objP->motStart, objP->motEnd);
}

void mbObjMotionLoopSet(MBOBJID objId, BOOL loopF)
{
    MBOBJ *objP = &objManData[objId];
    if(loopF) {
        Hu3DModelAttrSet(objP->modelId, HU3D_MOTATTR_LOOP);
    } else {
        Hu3DModelAttrReset(objP->modelId, HU3D_MOTATTR_LOOP);
    }
}

float mbObjMotionShapeTimeGet(MBOBJID objId)
{
    MBOBJ *objP = &objManData[objId];
    HU3D_MODEL *modelP = &Hu3DData[objP->modelId];
    return modelP->motShapeWork.time;
}

void mbObjMotionShapeSet(MBOBJID objId, int motNo, u32 attr)
{
    MBOBJ *objP = &objManData[objId];
    HU3D_MODEL *modelP = &Hu3DData[objP->modelId];
    modelP->motShapeWork.speed = 1;
    Hu3DMotionShapeSet(objP->modelId, objP->motId[motNo]);
    if(attr & HU3D_MOTATTR_SHAPE_REV) {
        modelP->motShapeWork.time = Hu3DMotionMaxTimeGet(objP->modelId);
    }
    Hu3DModelAttrSet(objP->modelId, attr);
}

void mbObjMotionShapeTimeSet(MBOBJID objId, float time)
{
    MBOBJ *objP = &objManData[objId];
    HU3D_MODEL *modelP = &Hu3DData[objP->modelId];
    modelP->motShapeWork.time = time;
}

float mbObjMotionShapeMaxTimeGet(MBOBJID objId)
{
    MBOBJ *objP = &objManData[objId];
    HU3D_MODEL *modelP = &Hu3DData[objP->modelId];
    modelP = &Hu3DData[objP->modelId];
    return modelP->motShapeWork.end;
}

void mbObjMotionShapeSpeedSet(MBOBJID objId, float speed)
{
    MBOBJ *objP = &objManData[objId];
    HU3D_MODEL *modelP = &Hu3DData[objP->modelId];
    modelP->motShapeWork.speed = speed;
}

float mbObjMotionShapeSpeedGet(MBOBJID objId)
{
    HU3D_MODEL *modelP;
    MBOBJ *objP = &objManData[objId];
    modelP = &Hu3DData[objP->modelId];
    return modelP->motShapeWork.speed;
}

BOOL mbObjMotionShapeEndCheck(MBOBJID objId)
{
    MBOBJ *objP = &objManData[objId];
    HU3D_MODEL *modelP = &Hu3DData[objP->modelId];
    if(modelP->attr & HU3D_MOTATTR_SHAPE_REV) {
        return modelP->motShapeWork.time <= 0;
    } else {
        return mbObjMotionShapeMaxTimeGet(objId) <= modelP->motShapeWork.time;
    }
}

void mbObjMotionShapeStartEndSet(MBOBJID objId, s16 start, s16 end)
{
    MBOBJ *objP = &objManData[objId];
    if(start >= 0) {
        objP->motStart = start;
    }
    if(end >= 0) {
        objP->motEnd = end;
    }
    Hu3DMotionShapeStartEndSet(objP->modelId, objP->motStart, objP->motEnd);
}

void mbObjShadowSet(MBOBJID objId)
{
    MBOBJ *objP = &objManData[objId];
    Hu3DModelShadowSet(objP->modelId);
}

void mbObjShadowReset(MBOBJID objId)
{
    MBOBJ *objP = &objManData[objId];
    Hu3DModelShadowReset(objP->modelId);
}

void mbObjShadowMapSet(MBOBJID objId)
{
    MBOBJ *objP = &objManData[objId];
    Hu3DModelShadowMapSet(objP->modelId);
}

void mbObjShadowMapReset(MBOBJID objId)
{
    MBOBJ *objP = &objManData[objId];
    Hu3DModelShadowMapReset(objP->modelId);
}

int mbObjStub0(void)
{
    return 0;
}
