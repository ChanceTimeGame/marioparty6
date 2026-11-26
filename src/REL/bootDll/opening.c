/* @@HEAD@@ */
/* Retail's opening.c was built without math.h: its sqrtf statics (0.5, 3.0) are absent from this
 * unit's .rodata. Block it and declare the two functions the unit calls. */
#define _MATH_H
#define M_PI 3.141592653589793
double sin(double x);
double cos(double x);

#include "game/hu3d.h"
#include "game/sprite.h"
#include "game/window.h"
#include "game/process.h"
#include "game/memory.h"
#include "game/data.h"
#include "game/wipe.h"
#include "game/audio.h"
#include "game/pad.h"
#include "game/object.h"
#include "humath.h"

#include "REL/bootDll/opening.h"

typedef void (*EVENT_FUNC)();

typedef struct OpeningEvent_s {
    EVENT_FUNC func;
    int arg1;
    int arg2;
} OPENING_EVENT;

extern HU3D_LIGHTID BootLightId[2];

static void PreDrawHook(s16 layerNo);
static void PostDrawHook(s16 layerNo);
static void OpeningBgUpdate(void);
static void MoveGuide(s32 no, s32 dist);
static void BlinkGuide(u32 time);
static void GuideCharFocus(u32 no, s32 time);
static void RotateGuide(u32 time);
static void CharObjLightStart(u32 time);
static void CharObjLightEnd(u32 time);
static void CharObjStarStart(u32 time);
static void CharObjStarEnd(u32 time);
static void ObjStarExec(void);
static void ObjStarStart(u32 time);
static void ObjStarEnd(void);
static void CharWaitEvent(u32 time);
static void CharFocusLeft(void);
static void CharFocusRight(void);
static void CharFocusRandom(u32 time);
static void CharHappyEvent(u32 time);
static void SetStarDist(s16 no, float dist);
static void SetCharAnimFrame(s16 no, s16 frame);
static void SwapAnimFrame(s16 no, s16 frame);
static void SwapAnimFrameEvent(int charNo, int frame);
static void MessStart(s16 winNo, u32 mess, s16 time);
static void BookOpen(void);
static void BookOpenEvent(void);
static void BookExec(void);
static void EventExec(void);
static void EventDestroy(void);
static void EventCreate(EVENT_FUNC func, int arg1, int arg2);
static void OpeningCameraSet(HuVecF *pos, HuVecF *target, float zoom);
static void FilterDraw(float tpLvl);

/* CodeWarrior emits .bss in reverse definition order */
static HU3D_MODELID OpeningMdlId[8];
s16 lbl_1_bss_224;
static void *BookFrameBuf[2];
static void *FilterFrameBuf;
static ANIMDATA *BookFrameBufAnim[2];
static ANIMDATA *OpeningCharAnim[79];
static HU3D_ANIMID BookFrameBufAnimId[2];
static HU3D_ANIMID OpeningCharAnimId[12];
static HuVecF BookPos;
static HuVecF BookOfs;
static HuVecF BookRot;
static float BookRadius;
static HUWINID OpeningWinId[4];
static s16 OpeningBgState;
static float GuideDist[2];
static float OpeningBgRotZ;
static HuVecF StarPos;
static float FilterTPLvl;
static BOOL CharLightFocusF;
static BOOL CharFocusCancelF;
static BOOL OpeningEndF;
static s16 FbCopyMode;

static s16 WinFrameTypeTbl[4] = { 0, 4, 3, 5 };

static s32 OpeningCharFileTbl[80] = {
    0xD5001E, 0xD50020, 0xD50022, 0xD50021, 0xD50022, 0xD50023, 0xD50024, 0xD50026,
    0xD50027, 0xD50028, 0xD50029, 0xD5002A, 0xD5002B, 0xD5002C, 0xD5002D, 0xD5002E,
    0xD5002F, 0xD50030, 0xD50031, 0xD50032, 0xD50033, 0xD50034, 0xD50035, 0xD50036,
    0xD50037, 0xD50038, 0xD50039, 0xD5003A, 0xD5003B, 0xD5003C, 0xD5003D, 0xD5003E,
    0xD5003F, 0xD50040, 0xD50041, 0xD50042, 0xD50043, 0xD50044, 0xD50045, 0xD50046,
    0xD50047, 0xD50048, 0xD50049, 0xD5004A, 0xD5004B, 0xD5004C, 0xD5004D, 0xD5004E,
    0xD5004F, 0xD50050, 0xD50051, 0xD50052, 0xD50053, 0xD50054, 0xD50055, 0xD50056,
    0xD50057, 0xD50058, 0xD50059, 0xD5005A, 0xD5005B, 0xD5005C, 0xD5005D, 0xD5005E,
    0xD5005F, 0xD50060, 0xD50061, 0xD50062, 0xD50063, 0xD50064, 0xD50069, 0xD5006A,
    0xD5006B, 0xD5006C, 0xD50065, 0xD50066, 0xD50067, 0xD50068, 0xD50025, -1,
};

static char *OpeningCharTexNameTbl[12] = {
    "Dummy_mario",
    "Dummy_Luigi",
    "Dummy_peach",
    "Dummy_yoshi",
    "Dummy_wario",
    "Dummy_daisy",
    "Dummy_waluigi",
    "Dummy_kinopio",
    "Dummy_teresa",
    "Dummy_minikoopa",
    "guide001",
    "guide002",
};

static char *OpeningCharObjNameTbl[12] = {
    "op_stage011-mario",
    "op_stage011-luigi",
    "op_stage011-peach",
    "op_stage011-yoshi",
    "op_stage011-wario",
    "op_stage011-daisy",
    "op_stage011-waluigi",
    "op_stage011-kinopio",
    "op_stage011-teresa",
    "op_stage011-minikoopa",
    "op_stage011-soruru",
    "op_stage011-luluna",
};

static HuVecF BookDefPos = { 0.0f, 0.0f, -2000.0f };
static HuVecF BookDefRot = { -20.0f, -30.0f, 30.0f };

/* @@PART1@@ */
void OpeningCreate(void)
{
    void *tempBuf;
    s16 i;

    OpeningMdlId[0] = Hu3DModelCreateData(0xD50016);
    Hu3DModelLayerSet(OpeningMdlId[0], 1);
    Hu3DMotionSpeedSet(OpeningMdlId[0], 0.015f);
    Hu3DModelPosSet(OpeningMdlId[0], 0.0f, 0.0f, -4500.0f);
    OpeningMdlId[1] = Hu3DModelCreateData(0xD50017);
    Hu3DModelLayerSet(OpeningMdlId[1], 1);
    Hu3DMotionSpeedSet(OpeningMdlId[1], 0.5f);
    Hu3DMotionClusterSpeedSet(OpeningMdlId[1], 0, 0.5f);
    Hu3DModelPosSet(OpeningMdlId[1], 0.0f, 0.0f, -2000.0f);
    Hu3DModelRotSet(OpeningMdlId[1], -20.0f, -30.0f, 30.0f);
    OpeningMdlId[2] = Hu3DModelCreateData(0xD50018);
    Hu3DModelLayerSet(OpeningMdlId[2], 0);
    Hu3DModelPosSet(OpeningMdlId[2], 0.0f, -330.0f, 0.0f);
    OpeningMdlId[3] = Hu3DModelCreateData(0xD50019);
    Hu3DModelLayerSet(OpeningMdlId[3], 1);
    OpeningMdlId[4] = Hu3DModelCreateData(0xD5001A);
    Hu3DModelLayerSet(OpeningMdlId[4], 1);
    OpeningMdlId[5] = Hu3DModelCreateData(0xD5001C);
    Hu3DModelLayerSet(OpeningMdlId[5], 1);
    OpeningMdlId[6] = Hu3DModelCreateData(0xD5001D);
    Hu3DModelLayerSet(OpeningMdlId[6], 1);
    Hu3DModelAttrSet(OpeningMdlId[6], HU3D_MOTATTR_LOOP);
    OpeningMdlId[7] = Hu3DModelCreateData(0xD5001B);
    Hu3DModelLayerSet(OpeningMdlId[7], 1);
    Hu3DMotionSpeedSet(OpeningMdlId[7], 2.0f);
    Hu3DModelScaleSet(OpeningMdlId[7], 2.0f, 2.0f, 2.0f);
    tempBuf = HuMemDirectMallocNum(HEAP_DVD, 0x100000, HU_MEMNUM_OVL);
    BookFrameBuf[0] = HuMemDirectMallocNum(HEAP_DVD, GXGetTexBufferSize(640, 320, GX_TF_RGBA8, GX_FALSE, 0), HU_MEMNUM_OVL);
    HuMemDirectFree(tempBuf);
    BookFrameBuf[1] = HuMemDirectMallocNum(HEAP_HEAP, GXGetTexBufferSize(640, 320, GX_TF_RGBA8, GX_FALSE, 0), HU_MEMNUM_OVL);
    BookFrameBufAnim[0] = HuSprAnimMake(640, 320, 0);
    BookFrameBufAnim[0]->bmp->data = BookFrameBuf[0];
    BookFrameBufAnim[1] = HuSprAnimMake(640, 320, 0);
    BookFrameBufAnim[1]->bmp->data = BookFrameBuf[1];
    BookFrameBufAnimId[0] = Hu3DAnimCreate(BookFrameBufAnim[0], OpeningMdlId[1], "op_book_dummy");
    BookFrameBufAnimId[1] = Hu3DAnimCreate(BookFrameBufAnim[1], OpeningMdlId[1], "op_book_dummy2");
    FilterFrameBuf = HuMemDirectMallocNum(HEAP_HEAP, GXGetTexBufferSize(640, 320, GX_TF_I8, GX_FALSE, 0), HU_MEMNUM_OVL);
    for (i = 0; i < 79; i++) {
        OpeningCharAnim[i] = HuSprAnimRead(HuDataSelHeapReadNum(OpeningCharFileTbl[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    for (i = 0; i < 12; i++) {
        OpeningCharAnimId[i] = Hu3DAnimCreate(OpeningCharAnim[0], OpeningMdlId[2], OpeningCharTexNameTbl[i]);
    }
    HuPrcChildCreate(BookExec, 0x100, 0x1000, 0, HuPrcCurrentGet());
    BookOfs.x = BookOfs.y = BookOfs.z = 0.0f;
    BookPos = BookDefPos;
    BookRot = BookDefRot;
    BookRadius = 1.0f;
    OpeningDispOff();
}

void OpeningDispOff(void)
{
    s16 i;
    for (i = 0; i < 8; i++) {
        Hu3DModelAttrSet(OpeningMdlId[i], HU3D_ATTR_DISPOFF);
    }
}

void OpeningExec(BOOL skipF)
{
    Mtx mtx;
    HUPROCESS *eventProc;
    u32 time;
    HUWINID winId;
    s16 i;

    Hu3DModelAttrSet(OpeningMdlId[3], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(OpeningMdlId[4], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(OpeningMdlId[5], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(OpeningMdlId[6], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(OpeningMdlId[7], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(OpeningMdlId[0], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(OpeningMdlId[1], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(OpeningMdlId[2], HU3D_ATTR_DISPOFF);
    Hu3DCameraLayerHookSet(1, 0, PreDrawHook);
    Hu3DCameraLayerHookSet(1, 8, PostDrawHook);
    Hu3DModelRotSet(OpeningMdlId[5], 0.0f, 0.0f, 0.0f);
    FbCopyMode = 0;
    CharFocusCancelF = FALSE;
    CharLightFocusF = FALSE;
    Hu3DAmbColorSet(1.0f, 1.0f, 1.0f);
    Hu3DShineSet(FALSE);
    BookOfs.x = BookOfs.y = BookOfs.z = 0.0f;
    BookPos = BookDefPos;
    BookRot = BookDefRot;
    BookRadius = 1.0f;
    Hu3DMotionTimeSet(OpeningMdlId[1], 0.0f);
    Hu3DModelAttrSet(OpeningMdlId[1], HU3D_MOTATTR_PAUSE);
    Hu3DMotionClusterTimeSet(OpeningMdlId[1], 0, 0.0f);
    Hu3DModelClusterAttrSet(OpeningMdlId[1], 0, HU3D_CLUSTER_ATTR_PAUSE);
    MTXTrans(mtx, 150.0f, 0.0f, 0.0f);
    Hu3DModelMtxSet(OpeningMdlId[1], &mtx);
    SetCharAnimFrame(0, 0);
    SetCharAnimFrame(1, 7);
    SetCharAnimFrame(2, 14);
    SetCharAnimFrame(3, 21);
    SetCharAnimFrame(4, 28);
    SetCharAnimFrame(5, 35);
    SetCharAnimFrame(6, 42);
    SetCharAnimFrame(7, 49);
    SetCharAnimFrame(8, 56);
    SetCharAnimFrame(9, 63);
    SetCharAnimFrame(10, 70);
    SetCharAnimFrame(11, 74);
    for (i = 0; i < 4; i++) {
        OpeningWinId[i] = HuWinExCreateFrame(-10000.0f, 364.0f, 544, 68, -1, WinFrameTypeTbl[i]);
        HuWinAttrSet(OpeningWinId[i], HUWIN_ATTR_ALIGN_CENTER);
    }
    GuideDist[0] = -225.0f;
    GuideDist[1] = -135.0f;
    SetStarDist(10, GuideDist[0]);
    SetStarDist(11, GuideDist[1]);
    winId = HuWinCreate(16.0f, 326.0f, 544, 42, 0);
    HuWinAttrSet(winId, HUWIN_ATTR_ALIGN_CENTER);
    HuWinPriSet(winId, 100);
    HuWinBGTPLvlSet(winId, 0.0f);
    HuWinMesSpeedSet(winId, 0);
    HuWinMesSet(winId, 0x10005);
    HuWinDispOff(winId);
    Hu3DGLightPosSet(BootLightId[0], 300.0f, 1000.0f, 1000.0f, -0.3f, -1.0f, -1.0f);
    Hu3DGLightColorSet(BootLightId[0], 255, 255, 255, 255);
    Hu3DGLightColorSet(BootLightId[1], 0, 0, 0, 255);
    HuAudSStreamPlay(0);
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
    while (WipeCheck()) {
        HuPrcVSleep();
    }
    OpeningEndF = FALSE;
    eventProc = HuPrcChildCreate(OpeningEventExec, 0x100, 0x3000, 0, HuPrcCurrentGet());
    time = 0;
    while (!OpeningEndF) {
        if (skipF) {
            if (time > 250 && omcurovl != DLL_bootdll) {
                HuWinDispOn(winId);
            }
            if ((omcurovl == DLL_bootdll && (HuPadBtnDown[0] & PAD_BUTTON_A)) || (HuPadBtnDown[0] & PAD_BUTTON_START)) {
                break;
            }
        }
        time++;
        HuPrcVSleep();
    }
    HuWinDispOff(winId);
    HuAudSStreamAllFadeOut(1000);
    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
    while (WipeCheck()) {
        HuPrcVSleep();
    }
    HuWinKill(winId);
    HuPrcKill(eventProc);
    Hu3DCameraLayerHookReset(1, 0);
    Hu3DCameraLayerHookReset(1, 8);
    for (i = 0; i < 4; i++) {
        HuWinExKill(OpeningWinId[i]);
    }
    OpeningDispOff();
}

void OpeningEventExec(void)
{
    OpeningBgState = 0;
    EventCreate(OpeningBgUpdate, 0, 0);
    EventCreate(RotateGuide, 1050, 0);
    EventCreate(CharWaitEvent, 990, 0);
    EventCreate(BlinkGuide, 1010, 0);
    EventCreate(ObjStarExec, 0, 0);
    BookOpen();
    HuPrcSleep(60);
    HuWinExOpen(OpeningWinId[0]);
    MessStart(0, 0xB0000, 240);
    MessStart(0, 0xB0001, 240);
    MessStart(0, 0xB0002, 240);
    MessStart(0, 0xB0003, 80);
    EventCreate(GuideCharFocus, 10, 60);
    EventCreate(CharFocusLeft, 0, 0);
    HuPrcSleep(160);
    while (OpeningBgRotZ > -200.0f) {
        HuPrcVSleep();
    }
    OpeningBgState = 2;
    EventCreate(GuideCharFocus, 0x800B, 60);
    EventCreate(CharFocusRight, 0, 0);
    MessStart(0, 0xB0004, 80);
    EventCreate(GuideCharFocus, 0x800A, 40);
    EventCreate(CharFocusLeft, 0, 0);
    OpeningBgState = 3;
    MessStart(0, 0xB0005, 70);
    EventCreate(GuideCharFocus, 0x800B, 40);
    EventCreate(CharFocusRight, 0, 0);
    OpeningBgState = 4;
    MessStart(0, 0xB0006, 60);
    EventCreate(GuideCharFocus, 0x800A, 40);
    EventCreate(CharFocusLeft, 0, 0);
    OpeningBgState = 3;
    MessStart(0, 0xB0007, 60);
    EventCreate(GuideCharFocus, 0x800B, 40);
    EventCreate(CharFocusRight, 0, 0);
    OpeningBgState = 4;
    MessStart(0, 0xB0006, 50);
    EventCreate(GuideCharFocus, 0x800A, 40);
    EventCreate(CharFocusLeft, 0, 0);
    OpeningBgState = 3;
    MessStart(0, 0xB0007, 50);
    OpeningBgState = 5;
    EventCreate(GuideCharFocus, 0x800A, 10000);
    EventCreate(GuideCharFocus, 0x800B, 10000);
    EventCreate(CharFocusRandom, 600, 0);
    MessStart(0, 0xB0008, 240);
    MessStart(0, 0xB0009, 240);
    HuPrcSleep(120);
    HuWinExClose(OpeningWinId[0]);
    CharLightFocusF = TRUE;
    CharFocusCancelF = TRUE;
    EventCreate(CharObjLightStart, 20, 0);
    SwapAnimFrame(0, 4);
    HuWinExOpen(OpeningWinId[0]);
    MessStart(0, 0xB000A, 0);
    HuPrcSleep(20);
    SwapAnimFrame(1, 11);
    SwapAnimFrame(2, 18);
    SwapAnimFrame(3, 25);
    SwapAnimFrame(4, 32);
    SwapAnimFrame(5, 39);
    SwapAnimFrame(6, 46);
    SwapAnimFrame(7, 53);
    HuPrcSleep(30);
    SwapAnimFrame(8, 57);
    SwapAnimFrame(9, 64);
    HuPrcSleep(60);
    EventCreate(CharObjLightEnd, 20, 0);
    HuPrcSleep(30);
    MessStart(0, 0xB000B, 10);
    EventCreate(CharObjStarStart, 30, 0);
    HuPrcSleep(60);
    CharLightFocusF = CharFocusCancelF = FALSE;
    EventCreate(GuideCharFocus, 0x800A, 20);
    EventCreate(GuideCharFocus, 0x800B, 20);
    HuPrcSleep(170);
    MessStart(0, 0xB000C, 240);
    MessStart(0, 0xB000D, 240);
    MessStart(0, 0xB000E, 40);
    EventCreate(CharHappyEvent, 200, 0);
    HuPrcSleep(200);
    EventCreate(CharObjStarEnd, 20, 0);
    HuWinExClose(OpeningWinId[0]);
    FbCopyMode = 1;
    Hu3DModelAttrReset(OpeningMdlId[1], HU3D_MOTATTR_PAUSE);
    Hu3DModelClusterAttrReset(OpeningMdlId[1], 0, HU3D_CLUSTER_ATTR_PAUSE);
    HuPrcSleep(10);
    SetCharAnimFrame(10, 72);
    SetCharAnimFrame(11, 76);
    EventCreate(CharHappyEvent, 390, 0);
    HuPrcSleep(140);
    HuWinExOpen(OpeningWinId[0]);
    EventCreate(ObjStarStart, 30, 0);
    MessStart(0, 0xB000F, 240);
    FbCopyMode = 2;
    HuWinExClose(OpeningWinId[0]);
    HuPrcSleep(130);
    HuWinExOpen(OpeningWinId[0]);
    MessStart(0, 0xB0010, 300);
    HuWinExClose(OpeningWinId[0]);
    EventCreate(ObjStarEnd, 0, 0);
    HuPrcSleep(140);
    OpeningEndF = TRUE;
    while (1) {
        HuPrcVSleep();
    }
}

static void PreDrawHook(s16 layerNo)
{
    HuVecF pos;
    HuVecF target;

    Hu3DCameraPerspectiveSet(1, 45.0f, 20.0f, 8000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    pos.x = pos.y = pos.z = 0.0f;
    target.x = 0.0f;
    target.y = 0.0f;
    target.z = 0.0f;
    OpeningCameraSet(&pos, &target, 450.0f);
}

static void PostDrawHook(s16 layerNo)
{
    HuVecF pos;
    HuVecF target;

    Hu3DCameraPerspectiveSet(1, 30.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    pos.x = pos.y = pos.z = 0.0f;
    target.x = target.y = target.z = 0.0f;
    OpeningCameraSet(&pos, &target, 500.0f);
    switch (FbCopyMode) {
        case 0:
            Hu3DFbCopyExec(0, 80, 640, 320, GX_TF_RGBA8, 0, BookFrameBuf[0]);
            FilterTPLvl = 0.0f;
            break;
        case 1:
            Hu3DFbCopyExec(0, 80, 640, 320, GX_TF_RGBA8, 0, BookFrameBuf[1]);
            break;
        case 2:
            FilterTPLvl += 0.01f;
            if (FilterTPLvl > 0.9f) {
                FilterTPLvl = 0.9f;
            }
            Hu3DFbCopyExec(0, 80, 640, 320, GX_TF_I8, 0, FilterFrameBuf);
            FilterDraw(FilterTPLvl);
            Hu3DFbCopyExec(0, 80, 640, 320, GX_TF_RGBA8, 0, BookFrameBuf[1]);
            break;
    }
    Hu3DZClear();
}

void OpeningKill(void)
{
    Hu3DAnimKill(BookFrameBufAnimId[0]);
    Hu3DAnimKill(BookFrameBufAnimId[1]);
    HuSprAnimKill(BookFrameBufAnim[0]);
    HuSprAnimKill(BookFrameBufAnim[1]);
}

static void OpeningBgUpdate(void)
{
    HSF_OBJECT *obj;
    s32 time;
    s16 prevState;
    s32 dummy;
    float t;
    float start;

    prevState = -1;
    OpeningBgState = 0;
    obj = Hu3DModelObjPtrGet(OpeningMdlId[2], "op_stage004-bg_root");
    time = 0;
    while (1) {
        if (prevState != OpeningBgState) {
            time = 0;
        }
        prevState = OpeningBgState;
        switch (OpeningBgState) {
            case 0:
                time++;
                if (time > 1000) {
                    break;
                }
                t = HuSin(90.0f * (time / 1000.0f));
                obj->mesh.base.rot.z = -200.0f * t;
                dummy = obj->mesh.base.rot.z / 19.0f;
                if (obj->mesh.base.rot.z > -120.0f) {
                    GuideDist[0] = -45.0f;
                } else {
                    GuideDist[0] = -225.0f;
                }
                SetStarDist(10, GuideDist[0]);
                dummy = obj->mesh.base.rot.z / 19.0f;
                GuideDist[1] = -135.0f;
                SetStarDist(11, GuideDist[1]);
                OpeningBgRotZ = obj->mesh.base.rot.z;
                break;
            case 1:
                break;
            case 2:
                time++;
                if (time > 30) {
                    break;
                }
                t = time / 30.0f;
                obj->mesh.base.rot.z = -200.0 + 20.0 * HuSin(90.0f * t);
                if (time == 1) {
                    EventCreate(MoveGuide, 11, -150);
                }
                if (time == 15.0) {
                    EventCreate(MoveGuide, 10, -210);
                }
                break;
            case 3:
                if (time == 0) {
                    start = obj->mesh.base.rot.z;
                }
                time++;
                if (time > 30) {
                    break;
                }
                t = time / 30.0f;
                obj->mesh.base.rot.z = start + (-200.0f - start) * HuSin(90.0f * t);
                break;
            case 4:
                if (time == 0) {
                    start = obj->mesh.base.rot.z;
                }
                time++;
                if (time > 30) {
                    break;
                }
                t = time / 30.0f;
                obj->mesh.base.rot.z = start + (-160.0f - start) * HuSin(90.0f * t);
                break;
            case 5:
                if (time == 0) {
                    start = obj->mesh.base.rot.z;
                }
                time++;
                if (time > 30) {
                    break;
                }
                t = time / 30.0f;
                obj->mesh.base.rot.z = start + (-180.0f - start) * HuSin(90.0f * t);
                break;
        }
        HuPrcVSleep();
    }
}

static void MoveGuide(s32 no, s32 dist)
{
    float start;
    float t;
    s16 i;

    start = GuideDist[no - 10];
    for (i = 1; i <= 20; i++) {
        t = HuSin(90.0f * (i / 20.0f));
        SetStarDist(no, start + t * (dist - start));
        HuPrcVSleep();
    }
}

static void BlinkGuide(u32 time)
{
    s16 frame;
    s16 cnt;
    s16 i;

    SetCharAnimFrame(10, 71);
    SetCharAnimFrame(11, 75);
    frame = 1;
    for (i = 1, cnt = 0; i <= time; i++) {
        if (cnt++ > 10) {
            frame ^= 1;
            SetCharAnimFrame(10, frame + 70);
            SetCharAnimFrame(11, frame + 74);
            cnt = 0;
        }
        HuPrcVSleep();
    }
    SetCharAnimFrame(10, 70);
    SetCharAnimFrame(11, 74);
}

static void GuideCharFocus(u32 no, s32 time)
{
    HSF_OBJECT *obj;
    s16 frame;
    s32 flag;
    s16 i;
    s16 cnt;
    float scale;

    flag = no & 0x8000;
    CharFocusCancelF = FALSE;
    no &= 0xFF;
    obj = Hu3DModelObjPtrGet(OpeningMdlId[2], OpeningCharObjNameTbl[no]);
    if (no == 10) {
        if (!flag) {
            SetCharAnimFrame(no, 71);
        } else {
            SetCharAnimFrame(no, 73);
        }
    } else {
        if (!flag) {
            SetCharAnimFrame(no, 75);
        } else {
            SetCharAnimFrame(no, 77);
        }
    }
    scale = 1.2f;
    for (frame = 1, i = 1, cnt = 0; i <= time; i++) {
        obj->mesh.base.scale.x = obj->mesh.base.scale.y = scale;
        if (cnt++ > 5) {
            frame ^= 1;
            if (CharLightFocusF) {
                scale = 1.0f;
            } else {
                scale = (scale == 1.0f) ? 1.2f : 1.0f;
            }
            if (no == 10) {
                if (!flag) {
                    SetCharAnimFrame(no, frame + 70);
                } else {
                    SetCharAnimFrame(no, frame + 72);
                }
            } else {
                if (!flag) {
                    SetCharAnimFrame(no, frame + 74);
                } else {
                    SetCharAnimFrame(no, frame + 76);
                }
            }
            cnt = 0;
        }
        if (CharFocusCancelF) {
            break;
        }
        HuPrcVSleep();
    }
    if (no == 10) {
        if (!flag) {
            SetCharAnimFrame(no, 71);
        } else {
            SetCharAnimFrame(no, 73);
        }
    } else {
        if (!flag) {
            SetCharAnimFrame(no, 75);
        } else {
            SetCharAnimFrame(no, 77);
        }
    }
    obj->mesh.base.scale.x = obj->mesh.base.scale.y = 1.0f;
}

static void RotateGuide(u32 time)
{
    HSF_OBJECT *obj;
    HSF_OBJECT *bgObj;
    s16 i;
    float angle;
    float amp;

    angle = 0.0f;
    amp = 10.0f;
    bgObj = Hu3DModelObjPtrGet(OpeningMdlId[2], "op_stage004-bg_root");
    for (i = 1;; i++) {
        obj = Hu3DModelObjPtrGet(OpeningMdlId[2], OpeningCharObjNameTbl[10]);
        obj->mesh.base.rot.z = -bgObj->mesh.base.rot.z;
        obj->mesh.base.rot.z += amp * HuSin(angle);
        obj = Hu3DModelObjPtrGet(OpeningMdlId[2], OpeningCharObjNameTbl[11]);
        obj->mesh.base.rot.z = -bgObj->mesh.base.rot.z;
        obj->mesh.base.rot.z += amp * HuCos(angle);
        angle += 5.0f;
        if (i > time - 10) {
            amp -= 1.0f;
            if (amp < 0.0f) {
                amp = 0.0f;
            }
        }
        HuPrcVSleep();
    }
}

static void CharObjLightStart(u32 time)
{
    HuVecF pos;
    s16 i;
    s16 len;
    s16 j;
    float scaleX;
    float scaleY;

    Hu3DModelAttrReset(OpeningMdlId[3], HU3D_ATTR_DISPOFF);
    Hu3DModelPosGet(OpeningMdlId[1], &pos);
    pos.x += 30.0f;
    pos.y -= 45.0f;
    pos.z += 20.0f;
    Hu3DModelPosSetV(OpeningMdlId[3], &pos);
    Hu3DModelScaleSet(OpeningMdlId[3], 0.0f, 0.0f, 0.0f);
    HuAudFXPlay(1200);
    for (i = 1; i <= time; i++) {
        if (i <= 0.7 * time) {
            len = 0.7 * time;
            scaleY = 0.9f * (HuSin(130.0f * ((float)i / len)) * (1.0 / HuSin(130.0f)));
        }
        if (i > 0.3 * time) {
            j = i - 0.3f * time;
            len = 0.7f * time;
            scaleX = 0.9f * (HuSin(130.0f * ((float)j / len)) * (1.0 / HuSin(130.0f)));
        }
        Hu3DModelScaleSet(OpeningMdlId[3], scaleX, scaleY, 1.0f);
        HuPrcVSleep();
    }
}

static void CharObjLightEnd(u32 time)
{
    s16 i;
    float scaleX;
    float scaleY;

    for (i = 1; i <= time; i++) {
        scaleX = 0.9f * (1.0 - HuSin(90.0f * ((float)i / time)));
        scaleY = 0.9f * (1.0 - HuSin(90.0f * ((float)i / time)));
        Hu3DModelScaleSet(OpeningMdlId[3], scaleX, scaleY, 1.0f);
        HuPrcVSleep();
    }
}

/* @@PART2@@ */
#pragma dont_inline on
static void CharObjStarStart(u32 time) { }
static void CharObjStarEnd(void) { }
static void ObjStarExec(void) { }
static void ObjStarStart(u32 time) { }
static void ObjStarEnd(void) { }
static void CharWaitEvent(u32 time) { }
static void CharFocusLeft(void) { }
static void CharFocusRight(void) { }
static void CharFocusRandom(u32 time) { }
static void CharHappyEvent(u32 time) { }
static void SetStarDist(s16 no, float dist) { }
static void SetCharAnimFrame(s16 no, s16 frame) { }
static void SwapAnimFrame(s16 no, s16 frame) { }
static void SwapAnimFrameEvent(int charNo, int frame) { }
static void MessStart(s16 winNo, u32 mess, s16 time) { }
static void BookOpen(void) { }
static void BookOpenEvent(void) { }
static void BookExec(void) { }
static void EventExec(void) { }
static void EventDestroy(void) { }
static void EventCreate(EVENT_FUNC func, int arg1, int arg2) { }
static void OpeningCameraSet(HuVecF *pos, HuVecF *target, float zoom) { }
static void FilterDraw(float tpLvl) { }
#pragma dont_inline reset
/* @@END@@ */
