/* @@HEAD@@ */
#include "game/board/capsule.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/branch.h"
#include "game/board/capselect.h"
#include "game/board/exit.h"
/* Retail calls mbMasuCapsuleGet as returning int (no extsh of its result). */
#define mbMasuCapsuleGet mbMasuCapsuleGet_s16
#include "game/board/masu.h"
#undef mbMasuCapsuleGet
int mbMasuCapsuleGet(s16 id);
#include "game/board/math.h"
#include "game/board/object.h"
#include "game/board/pause.h"
#include "game/board/player.h"
#include "game/board/single.h"
#include "game/board/telop.h"
#include "game/board/tutorial.h"
#include "game/board/window.h"
#include "game/board/wipe.h"

#include "game/charman.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hsfformat.h"
#include "game/hsfload.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/printfunc.h"
#include "game/process.h"
#include "game/sprite.h"
#include "math.h"
#include "string.h"

/* capevent.c: declared here (not through capevent.h) with the types retail passes */
extern u32 mbCapEffNum;
extern s16 *mbCapEffData;
int mbev_CapCall(int playerNo, int capsuleNo, int arg2, int arg3);
int mbev_CapMasuMoveCheck(int masuId);
void mbev_CapBonusCoinCall(int playerNo, int capsuleNo, int coinNum, int arg3);
void mbev_CapPlayerMotShiftWait(int playerNo, int motNo, u32 attr, BOOL waitF);
void mbev_CapEffColorSet(GXColor *color, int no);
OMOBJ *mbev_CapEffGlowCreate(void);
void mbev_CapEffGlowKill(OMOBJ *obj);
BOOL mbev_CapEffGlowDispGet(OMOBJ *obj);
int mbev_CapEffGlowAdd(OMOBJ *obj, HuVecF pos, HuVecF vel, int time, float size, float speed, float scale, GXColor color);
OMOBJ *mbev_CapEffRingCreate(void);
void mbev_CapEffRingKill(OMOBJ *obj);
int mbev_CapEffRingAdd(OMOBJ *obj, HuVecF pos, HuVecF rot, HuVecF scale, int arg4, int time, int arg6, GXColor color);
OMOBJ *mbev_CapEffRayCreate(float arg0, float arg1);
void mbev_CapEffRayKill(OMOBJ *obj);
int mbev_CapEffRayAdd(OMOBJ *obj, HuVecF *pos, HuVecF *rot, HuVecF *rotEnd, float scale, int time);
void mbev_CapEffRayAlphaSet(OMOBJ *obj, float alpha);
void mbev_CapEffRayTransformSet(OMOBJ *obj, HuVecF *pos, HuVecF *rot, HuVecF *scale);
OMOBJ *mbev_CapEffMasuHitCreate(void);
void mbev_CapEffMasuHitKill(OMOBJ *obj);
int mbev_CapEffMasuHitAdd(OMOBJ *obj, HuVecF *pos, HuVecF *rot, HuVecF *rotEnd, float scale, float speed, int time);
void mbev_CapEffMasuHitTransformSet(OMOBJ *obj, HuVecF *pos, HuVecF *rot, HuVecF *scale);
OMOBJ *mbev_CapEffCoinCreate(void);
void mbev_CapEffCoinKill(OMOBJ *obj);
int mbev_CapEffCoinNumGet(OMOBJ *obj);
void mbev_CapEffCoinMultiAdd(OMOBJ *obj, HuVecF *pos, int num);

#define CAP_RAND_F() ((1.0f/0x10000000)*mbRandMod(0x10000000))

typedef void (*CAPTHROWHOOK)(BOOL startF);

typedef struct CapUseWork_s {
    int playerNo;
    int capsuleNo;
} CAPUSEWORK;

typedef struct CapPlayerThrowWork_s {
    int playerNo; //0x00
    int masuId; //0x04
    int capsuleNo; //0x08
    int objId; //0x0C
    int lineObjId; //0x10
    int colorObjId; //0x14
    int motId; //0x18
    int time; //0x1C
    int maxTime; //0x20
    float height; //0x24
    HuVecF pos; //0x28
    HuVecF masuPos; //0x34
    u8 unk40[0xC]; //0x40
} CAPPLAYERTHROWWORK; //size=0x4C

typedef struct CapEffThrowWork_s {
    int mdlId; //0x00
    int masuId; //0x04
    int maxTime; //0x08
    int endF; //0x0C
    int unk10; //0x10
    int frameNo; //0x14
    int time; //0x18
    HuVecF start; //0x1C
    HuVecF end; //0x28
    HuVecF pos; //0x34
    float height; //0x40
    u32 tick; //0x44
    u32 delay; //0x48
    float posX[3]; //0x4C
    float posY[3]; //0x58
    float posZ[3]; //0x64
} CAPEFFTHROWWORK; //size=0x70

typedef struct CapThrowFrame_s {
    float rate;
    float y;
    float side;
    int dir;
} CAPTHROWFRAME;

typedef struct CapAutoThrowWork_s {
    int playerNo; //0x00
    int capsuleNo; //0x04
    int masuId; //0x08
    int maxTime; //0x0C
    float speed; //0x10
    HuVecF pos[3]; //0x14
} CAPAUTOTHROWWORK; //size=0x38

typedef struct CapSelMasuWork_s {
    int playerNo; //0x00
    int unk4; //0x04
    int capsuleNo; //0x08
    int capObjId; //0x0C
    int mdlId; //0x10
    int winId; //0x14
    int winId2; //0x18
    int unk1C; //0x1C
} CAPSELMASU_WORK; //size=0x20

typedef struct CapListFile_s {
    int boardNo;
    u32 file;
} CAPLISTFILE;

typedef struct CapListDefine_s {
    int capsuleNo;
    char *name;
} CAPLISTDEFINE;

typedef struct CapObjData_s {
    int objId; //0x00
    ANIMDATA *anim; //0x04
    s16 animId1; //0x08
    s16 animId2; //0x0A
} CAPOBJDATA; //size=0xC

typedef struct CapObjColorData_s {
    s16 useF; //0x00
    s16 objId; //0x02
    s16 unk4; //0x04
    u8 layer; //0x06
    HuVecF pos; //0x08
    HuVecF rot; //0x14
    HuVecF scale; //0x20
} CAPOBJCOLORDATA; //size=0x2C

/* Statics defined in later parts (forward declarations for the calls part 1 makes). */
static int CapUseDelete(int playerNo, int capsuleNo);
static void CapSelectMasuPlayer(CAPSELMASU_WORK *work);
static void CapSelectMasuCom(CAPSELMASU_WORK *work);
static void CapSelectMasuListGet(s16 *list, s16 masuId, s16 frontNum, s16 backNum);
static void CapThrowEndWin(int masuId, int capsuleNo);
static void CapEffRemoveAddDestroy(void);
static void CapEffHiliteKill(void);
static void CapEffCrackCreate(void);
static void CapEffCrackKill(void);
static void CapEffCrackAdd(HuVecF *pos, HuVecF *rot);
static void CapEffTrailCreate(int capsuleNo);
static void CapEffTrailKill(void);
static void CapEffTrailAdd(HuVecF *pos, int capsuleNo);
static void CapEffTrailPosSet(HuVecF *pos);
static void CapComChoiceSet(int choice);
static void CapThrowCameraSet(float *posX, float *posY, float *posZ, int num);
static void CapThrowCameraCalc(float t, float *posX, float *posY, float *posZ, HuVecF *out, int num);
static void CapColMdlIdGet(void);
static int CapColCheck(HuVecF *pos, HuVecF *next, HuVecF *out);
static void CapColKill(void);
static int CapColExec(int masuId, HuVecF *pos, HuVecF *next, HuVecF *out);

static HUPROCESS *capsuleUseEffProc[4];
static int capsuleUseEffMode[4];
static HuVecF capsuleUseEffPos[4];
static CAPSULE_LIST capsuleList[33];
static s32 capsuleNum[33];
static CAPOBJCOLORDATA capsuleObjColorData[128];
static s16 capsuleObjBorderId[6];
static CAPOBJDATA capsuleObjData[8];

static CAPTHROWHOOK capsuleThrowHook;
static s16 *capsuleBorderObjId;
static OMOBJ *capsuleThrowGlowOMObj;
static OMOBJ *capsuleThrowRingOMObj;
static OMOBJ *capsuleThrowRayOMObj;
static OMOBJ *capsuleThrowMasuHitOMObj;
static OMOBJ *capsuleThrowMasuCoinOMObj;
static int capsuleUseRemoveOnF;
static int capsuleMasuSelectEndF;
static int capsuleComChoice;
static OMOBJ *capEffTrailOMObj;
static OMOBJ *capEffCrackOMObj;
static OMOBJ *capEffHiliteOMObj;
static OMOBJ *capEffRemoveAddOMObj;
static OMOBJ *capEffRemoveOMObj;
static int capsuleSelectComBack;
static OMOBJ *capEffMasuOkOMObj;
static OMOBJ *capsuleGuideOMObj;
static int capsuleComSearchF;
static HUPROCESS *capsulePlayerThrowProc;

static int capsuleMasuSelectComF[4] = { 0, 0, 0, 0 };
static int capsuleMasuSelectResult = -1;
static int capsuleObjId = -1;
static int capsuleColObjId = -1;
static int capsuleColMdlId = -1;
static int capEffThrowMdlId = -1;

static CAPSULE_DATA capsuleData[60] = {
    { 0xC0000, 0xC001C, 0x370000, 0x380000, 0, 0, 5, 'A', 0, "KINOKO", 1 },
    { 0xC0001, 0xC001C, 0x370001, 0x380001, 0, 0, 10, 'B', 0, "S KINOKO", 1 },
    { 0xC0002, 0xC001C, 0x370002, 0x380002, 0, 0, 10, 'A', 0, "P KINOKO", 1 },
    { 0xC0003, 0xC001C, 0x370003, 0x380003, 0, 0, 20, 'A', 0, "M KINOKO", 1 },
    { 0xC0004, 0xC001C, 0x370004, 0x380004, 0, 0, 20, 'B', 0, "KILLER", 1 },
    { 0xC0005, 0xC001C, 0x370005, 0x380005, 0, 0, 15, 'C', 0, "DOKAN", 1 },
    { 0xC0006, 0xC001C, 0x370006, 0x380006, 0, 0, 30, 'E', 0, "HANACHAN", 1 },
    { 0xC0007, 0xC001C, 0x370007, 0x380007, 0, 0, 30, 'E', 0, "N KINOKO", 1 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 1, "NULL", 0 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 1, "NASI", 0 },
    { 0xC0008, 0xC001D, 0x370008, 0x380008, 1, 1, 10, 'A', 1, "TOGEZO", 1 },
    { 0xC0009, 0xC001D, 0x370009, 0x380009, 1, 1, 15, 'A', 1, "KURIBO", 1 },
    { 0xC000A, 0xC001D, 0x37000A, 0x38000A, 1, 1, 20, 'B', 1, "PAKKUN", 1 },
    { 0xC000B, 0xC001D, 0x37000B, 0x38000B, 1, 1, 5, 'D', 1, "JANGO", 1 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 0, "HANUKE", 0 },
    { 0xC000C, 0xC001D, 0x37000D, 0x38000D, 1, 1, 10, 'C', 1, "KOKAMEKKU", 1 },
    { 0xC000D, 0xC001D, 0x37000E, 0x38000E, 1, 1, 5, 'B', 1, "KAMEKKU", 1 },
    { 0xC000E, 0xC001D, 0x37000F, 0x38000F, 1, 1, 10, 'A', 1, "THROWMAN", 1 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 0, "SUKA", 0 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 0, "KARA", 0 },
    { 0xC000F, 0xC001E, 0x370010, 0x380010, 2, 2, 10, 'B', 2, "BOBLE", 1 },
    { 0xC0010, 0xC001E, 0x370011, 0x380011, 2, 2, 15, 'C', 2, "BIRIQ", 1 },
    { 0xC0011, 0xC001E, 0x370012, 0x380012, 2, 2, 15, 'B', 2, "TUMUJIKUN", 1 },
    { 0xC0012, 0xC001E, 0x370013, 0x380013, 2, 2, 15, 'C', 2, "DOSSUN", 1 },
    { 0xC0013, 0xC001E, 0x370014, 0x380014, 2, 2, 10, 'C', 2, "BOMUHEI", 1 },
    { 0xC0014, 0xC001E, 0x37000C, 0x38000C, 2, 2, 10, 'E', 2, "PATAPATA", 1 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 1, "NETA", 0 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 1, "GA  ", 0 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 1, "TUKI", 0 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 1, "TA  ", 0 },
    { 0xC0015, 0xC001F, 0x370015, 0x380015, 3, 3, 20, 'D', 3, "HONE", 1 },
    { 0xC0016, 0xC001F, 0x370016, 0x380016, 3, 3, 10, 'D', 3, "LIGHT", 1 },
    { 0xC0021, 0xC001F, 0x370017, 0x380017, 3, 3, 15, 'D', 3, "TARU", 1 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 1, "0000", 0 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 1, "0000", 0 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 1, "0000", 0 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 1, "0000", 0 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 1, "0000", 0 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 1, "0000", 0 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 1, "0000", 0 },
    { 0xC0021, 0xC001F, 0x370018, 0x380018, 10, 3, 1, 'C', 3, "KILLER MOVE", 0 },
    { 0xC0021, 0xC001F, 0x370019, 0x380019, 10, 3, 1, 'C', 3, "KETTOU", 0 },
    { 0xC0021, 0xC0020, 0x37001A, 0x38001A, 10, 3, 1, 'C', 3, "MIRACLE", 0 },
    { 0xC0021, 0xC001F, 0x37001B, 0x38001B, 9, 3, 1, 'E', 3, "KOOPA", 0 },
    { 0xC0021, 0xC001F, 0x37001C, 0x38001C, 10, 3, 1, 'E', 3, "DONKEY", 0 },
    { 0xC0021, 0xC001F, 0x37001D, 0x38001D, 10, 3, 1, 'Z', 3, "VS", 0 },
    { 0xC0021, 0xC001F, 0x37001E, 0x38001E, 10, 3, 1, 'Z', 3, "R_TERESA", 0 },
    { 0x50017, 0xC001F, 0x37001F, 0x380022, 10, 0, 0, 'Z', 0, "DICE", 1 },
    { 0xC0022, 0xC001F, 0x370020, 0x380022, 10, 0, 0, 'Z', 0, "YAMERU", 1 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 1, "0000", 0 },
    { 0xC0021, 0xC001F, 0x370021, 0x38001F, 10, 0, 1, 'Z', 0, "DEBUG CAM TEST", 0 },
    { 0xC0021, 0xC001F, 0x370022, 0x380020, 10, 0, 1, 'Z', 0, "DEBUG WARP TEST", 0 },
    { 0xC0021, 0xC001F, 0x370023, 0x380021, 10, 0, 1, 'Z', 0, "DEBUG SETPOS TEST", 0 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 1, "0000", 0 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 1, "0000", 0 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 1, "0000", 0 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 1, "0000", 0 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 1, "0000", 0 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 1, "0000", 0 },
    { 0, 0, 0, 0x380022, 10, 0, 0, 'Z', 1, "0000", 0 },
};

static CAPSULE_CHANCE_TBL capsuleChanceTbl[23] = {
    { 0, { { 50, " " }, { 50, " " }, { 30, " " }, { 50, " " }, { 10, " " }, { 30, " " }, { 30, " " }, { 30, " " }, { 10, " " }, { 50, " " }, { 50, " " } } },
    { 1, { { 50, "*" }, { 50, "*" }, { 10, "*" }, { 30, "*" }, { 30, " " }, { 30, "*" }, { 10, " " }, { 30, "*" }, { 30, " " }, { 30, " " }, { 30, "*" } } },
    { 2, { { 5, "b" }, { 5, "b" }, { 10, "b" }, { 10, "b" }, { 10, " " }, { 5, "a" }, { 10, " " }, { 5, "a" }, { 30, " " }, { 10, " " }, { 5, "a" } } },
    { 3, { { 0, "*" }, { 0, "*" }, { 0, "*" }, { 0, "*" }, { 0, "*" }, { 0, "*" }, { 30, " " }, { 0, "*" }, { 0, "*" }, { 30, " " }, { 0, "*" } } },
    { 4, { { 10, " " }, { 30, "*" }, { 30, "*" }, { 10, " " }, { 50, " " }, { 10, "*" }, { 50, " " }, { 50, "*" }, { 30, "*" }, { 50, " " }, { 30, "*" } } },
    { 5, { { 30, " " }, { 30, "*" }, { 10, " " }, { 50, " " }, { 10, " " }, { 30, " " }, { 10, " " }, { 10, "*" }, { 50, " " }, { 10, " " }, { 10, "*" } } },
    { 6, { { 30, "*" }, { 50, "*" }, { 30, "*" }, { 50, "*" }, { 30, "*" }, { 30, "*" }, { 10, " " }, { 50, "*" }, { 10, "*" }, { 10, "*" }, { 30, "*" } } },
    { 10, { { 10, " " }, { 30, " " }, { 50, " " }, { 10, " " }, { 30, " " }, { 10, " " }, { 30, " " }, { 50, " " }, { 10, " " }, { 30, " " }, { 30, " " } } },
    { 11, { { 10, " " }, { 30, "*" }, { 30, "*" }, { 30, "*" }, { 10, " " }, { 10, " " }, { 50, " " }, { 30, "*" }, { 50, " " }, { 50, " " }, { 50, "*" } } },
    { 12, { { 10, " " }, { 30, " " }, { 50, " " }, { 10, " " }, { 30, " " }, { 10, " " }, { 30, " " }, { 30, " " }, { 10, " " }, { 50, " " }, { 30, " " } } },
    { 13, { { 30, " " }, { 10, " " }, { 10, " " }, { 50, " " }, { 10, " " }, { 30, " " }, { 50, " " }, { 10, " " }, { 30, " " }, { 10, " " }, { 10, " " } } },
    { 15, { { 10, " " }, { 10, " " }, { 30, " " }, { 10, " " }, { 10, " " }, { 50, " " }, { 50, " " }, { 10, " " }, { 10, " " }, { 30, " " }, { 10, " " } } },
    { 16, { { 10, " " }, { 10, " " }, { 10, " " }, { 10, " " }, { 30, " " }, { 50, " " }, { 30, " " }, { 10, " " }, { 50, " " }, { 10, " " }, { 10, " " } } },
    { 17, { { 30, " " }, { 10, " " }, { 10, " " }, { 30, " " }, { 10, " " }, { 50, " " }, { 30, " " }, { 30, " " }, { 10, " " }, { 10, " " }, { 30, " " } } },
    { 20, { { 50, " " }, { 30, " " }, { 50, " " }, { 10, " " }, { 50, " " }, { 10, " " }, { 10, " " }, { 10, " " }, { 10, " " }, { 50, " " }, { 50, " " } } },
    { 21, { { 10, " " }, { 30, " " }, { 50, " " }, { 10, " " }, { 30, " " }, { 50, " " }, { 30, " " }, { 10, " " }, { 10, " " }, { 50, " " }, { 50, " " } } },
    { 22, { { 50, " " }, { 10, "*" }, { 10, " " }, { 30, " " }, { 50, " " }, { 30, "*" }, { 10, " " }, { 10, "*" }, { 50, " " }, { 30, " " }, { 10, "*" } } },
    { 23, { { 30, " " }, { 50, "*" }, { 10, "*" }, { 30, " " }, { 50, " " }, { 10, " " }, { 10, " " }, { 10, "*" }, { 30, " " }, { 30, " " }, { 10, "*" } } },
    { 24, { { 30, " " }, { 30, " " }, { 10, " " }, { 30, " " }, { 50, " " }, { 10, " " }, { 10, " " }, { 30, " " }, { 30, " " }, { 10, " " }, { 10, " " } } },
    { 25, { { 30, " " }, { 10, " " }, { 10, " " }, { 50, " " }, { 10, " " }, { 10, " " }, { 50, " " }, { 10, " " }, { 30, " " }, { 10, " " }, { 10, " " } } },
    { 30, { { 30, "*" }, { 50, " " }, { 30, "*" }, { 30, " " }, { 30, "*" }, { 50, "*" }, { 10, " " }, { 50, " " }, { 30, " " }, { 10, " " }, { 50, " " } } },
    { 31, { { 10, "*" }, { 50, " " }, { 30, "*" }, { 30, " " }, { 10, "*" }, { 30, "*" }, { 10, " " }, { 50, " " }, { 0, " " }, { 10, " " }, { 50, " " } } },
    { -1, { { 0, " " }, { 0, " " }, { 0, " " }, { 0, " " }, { 0, " " }, { 0, " " }, { 0, " " }, { 0, " " }, { 0, " " }, { 0, " " }, { 0, " " } } },
};

static GXColor capsuleMasuSelectColorTbl[8] = {
    { 192, 255, 192, 255 }, { 255, 255, 192, 255 }, { 255, 192, 192, 255 }, { 192, 255, 255, 255 },
    { 192, 255, 192, 255 }, { 255, 255, 192, 255 }, { 255, 192, 192, 255 }, { 192, 255, 255, 255 }
};

static HuVecF capsulePlayerThrowRot[3] = {
    { -90.0f, 0.0f, 20.0f },
    { -90.0f, 0.0f, 0.0f },
    { 0.0f, 10.0f, 60.0f }
};

static GXColor capsulePlayerThrowColorTbl[4] = {
    { 192, 255, 192, 255 }, { 255, 255, 192, 255 }, { 255, 192, 192, 255 }, { 192, 255, 255, 255 }
};

static u32 capsulePlayerThrowDelayTbl[11] = {
    100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 100000
};

static CAPTHROWFRAME capsulePlayerThrowFrameTbl[48] = {
    { 0.5f, 0.0f, 0.0f, 0 }, { 0.2f, 0.0f, 0.0f, 0 }, { 0.8f, 0.0f, 0.0f, 0 }, { 0.5f, 500.0f, 0.0f, 0 },
    { 0.2f, 500.0f, 0.0f, 0 }, { 0.8f, 500.0f, 0.0f, 0 }, { 1.0f, 500.0f, 0.0f, 0 }, { 1.2f, 500.0f, 0.0f, 0 },
    { 0.2f, 0.0f, 300.0f, 0 }, { 0.2f, 0.0f, -300.0f, 0 }, { 0.0f, 0.0f, 300.0f, 0 }, { 0.0f, 0.0f, -300.0f, 0 },
    { 0.8f, 0.0f, 300.0f, 0 }, { 0.8f, 0.0f, -300.0f, 0 }, { 1.0f, 0.0f, 300.0f, 0 }, { 1.0f, 0.0f, -300.0f, 0 },
    { 0.2f, 0.0f, 800.0f, 0 }, { 0.2f, 0.0f, -800.0f, 0 }, { 0.0f, 0.0f, 800.0f, 0 }, { 0.0f, 0.0f, -800.0f, 0 },
    { 0.8f, 0.0f, 800.0f, 0 }, { 0.8f, 0.0f, -800.0f, 0 }, { 1.0f, 0.0f, 800.0f, 0 }, { 1.0f, 0.0f, -800.0f, 0 },
    { 0.2f, 500.0f, 300.0f, 0 }, { 0.2f, 500.0f, -300.0f, 0 }, { 0.0f, 500.0f, 300.0f, 0 }, { 0.0f, 500.0f, -300.0f, 0 },
    { 0.8f, 500.0f, 300.0f, 0 }, { 0.8f, 500.0f, -300.0f, 0 }, { 1.0f, 500.0f, 300.0f, 0 }, { 1.0f, 500.0f, -300.0f, 0 },
    { 0.2f, 500.0f, 800.0f, 0 }, { 0.2f, 500.0f, -800.0f, 0 }, { 0.0f, 500.0f, 800.0f, 0 }, { 0.0f, 500.0f, -800.0f, 0 },
    { 0.8f, 500.0f, 800.0f, 0 }, { 0.8f, 500.0f, -800.0f, 0 }, { 1.0f, 500.0f, 800.0f, 0 }, { 1.0f, 500.0f, -800.0f, 0 },
    { 0.5f, 500.0f, 0.0f, 1 }, { 0.5f, 500.0f, 0.0f, -1 }, { 0.7f, 500.0f, 0.0f, 1 }, { 0.7f, 500.0f, 0.0f, -1 },
    { 0.3f, 500.0f, 0.0f, 1 }, { 0.3f, 500.0f, 0.0f, -1 }, { 0.7f, 500.0f, 0.0f, 1 }, { 0.7f, 500.0f, 0.0f, -1 },
};

static HuVecF capsuleAutoThrowRot[2] = {
    { -90.0f, 0.0f, 0.0f },
    { 0.0f, 10.0f, 60.0f }
};

static GXColor capsuleAutoThrowColorTbl[4] = {
    { 192, 255, 192, 255 }, { 255, 255, 192, 255 }, { 255, 192, 192, 255 }, { 192, 255, 255, 255 }
};

static HuVecF capsuleMasuSelectRotTbl[2] = {
    { -90.0f, 0.0f, 0.0f },
    { 0.0f, 10.0f, 50.0f }
};

static CAPLISTFILE capsuleListFileTbl[12] = {
    { 0, 0xC004A }, { 1, 0xC004B }, { 2, 0xC004C }, { 3, 0xC004D },
    { 4, 0xC004E }, { 5, 0xC004F }, { 6, 0xC0050 }, { 7, 0xC0051 },
    { 8, 0xC0052 }, { 9, 0xC0053 }, { 10, 0xC0054 }, { -1, 0xFFFFFFFF }
};

static CAPLISTDEFINE capsuleListDefineTbl[24] = {
    { 0, "CAPSULE_KINOKO" },
    { 1, "CAPSULE_SKINOKO" },
    { 2, "CAPSULE_PKINOKO" },
    { 3, "CAPSULE_MKINOKO" },
    { 4, "CAPSULE_KILLER" },
    { 5, "CAPSULE_DOKAN" },
    { 6, "CAPSULE_HANACHAN" },
    { 10, "CAPSULE_TOGEZO" },
    { 11, "CAPSULE_KURIBO" },
    { 12, "CAPSULE_PAKKUN" },
    { 13, "CAPSULE_JANGO" },
    { 25, "CAPSULE_PATAPATA" },
    { 15, "CAPSULE_KOKAMEKKU" },
    { 16, "CAPSULE_KAMEKKU" },
    { 17, "CAPSULE_THROWMAN" },
    { 20, "CAPSULE_BOBLE" },
    { 21, "CAPSULE_BIRIQ" },
    { 22, "CAPSULE_TUMUJIKUN" },
    { 23, "CAPSULE_DOSSUN" },
    { 24, "CAPSULE_BOMHEI" },
    { 30, "CAPSULE_HONE" },
    { 31, "CAPSULE_LIGHT" },
    { 32, "CAPSULE_TARU" },
    { -1, "CAPSULE_NULL" },
};

static int capsuleListColW[16] = {
    108, 32, 32, 48, 32, 32, 32, 32, 32, 32, 32, 48, 32, 32, 32, 48
};

/* CapEffThrowMasu's random numbers come from capevent.c's table. */
static inline s16 CapEffRandGet(void)
{
    if(++mbCapEffNum >= 1024) {
        mbCapEffNum = 0;
    }
    return mbCapEffData[mbCapEffNum];
}

static inline float CapEffRandF(void)
{
    return (1.0f/32767.0f)*CapEffRandGet();
}

static inline float CapEffRandSF(void)
{
    return CapEffRandF()-0.5f;
}

#define CapHookNameGet(playerNo) CharModelItemHookGet(GwPlayer[playerNo].charNo, 4, 0)

static inline void *CapMalloc(int size)
{
    return HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
}
/* Placeholder copies of part 2 functions that retail inlines into part 3 (own-file scoring only). */
s16 mbCapValueTypeGet(s16 value)
{
    return value & 0xFF;
}

s16 mbCapMasuTypeGet(s16 masuId)
{
    return mbCapValueTypeGet(mbMasuCapsuleGet(masuId));
}

void mbCapMasuPlayerTypeSet(s16 masuId, s16 capsuleNo, s16 playerNo)
{
    s16 value;
    mbMasuCapsuleSet(masuId, capsuleNo);
    value = mbMasuCapsuleGet(masuId);
    value = value | ((u8)playerNo << 8);
    mbMasuCapsuleSet(masuId, value);
}

s16 mbCapUseModeGet(s16 capsuleNo)
{
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    return capsuleData[capsuleNo].useMode;
}

void CapsulePlaceholderBssUse(void)
{
    capsuleUseEffProc[0] = 0;
    capsuleUseEffMode[0] = 0;
    capsuleUseEffPos[0].x = 0;
    capsuleNum[0] = 0;
}

static void CapSelectMasuAddFront(s16 *list, s16 masuId, s16 num);
static void CapSelectMasuAddBack(s16 *list, s16 masuId, s16 num);
/* @@PART1@@ */
/* @@PART2@@ */
/* Own-file only (the splice drops this section): pad for the .data part 2 emits before 0x1600.
   Stub bodies for part 2 statics are in PART4 so they do not inline. */
static u8 capsulePart2DataPad[0x1600-0x14C0] = { 1 };
/* Tables part 2 places after mbCapComChanceGet (copies for own-file scoring). */
static int capsuleThrowTbl[8] = { 10, 11, 12, 13, 25, 15, 16, 17 };
static int capsuleTrapTbl[5] = { 20, 21, 22, 23, 24 };
static int capsuleChanceTbl2[3][4][5] = {
    { { 70, 15, 15, 0, 0 }, { 70, 20, 10, 0, 0 }, { 70, 20, 0, 10, 0 }, { 50, 30, 0, 20, 0 } },
    { { 40, 30, 30, 0, 0 }, { 30, 30, 20, 20, 0 }, { 10, 40, 10, 30, 10 }, { 10, 40, 0, 30, 20 } },
    { { 30, 30, 30, 10, 0 }, { 20, 20, 20, 30, 10 }, { 10, 30, 0, 40, 20 }, { 10, 20, 0, 40, 30 } }
};
static int capsuleMaxTurnTbl[9] = { 10, 15, 20, 25, 30, 35, 40, 45, 50 };
static int capsuleTurnTbl[9][2] = {
    { 3, 6 }, { 5, 10 }, { 5, 15 }, { 8, 16 }, { 10, 20 }, { 10, 20 }, { 13, 26 }, { 15, 30 }, { 15, 35 }
};

/* @@PART3@@ */
/* Part 3: includes, types and helpers part 1's HEAD does not have yet (see report). */
#include "game/hsfload.h"
#include "game/hsfformat.h"
#include "game/data.h"
#include "game/sprite.h"

typedef struct CapsuleEffData_s {
    s16 time;
    s16 work;
    s16 mode;
    s16 cameraBit;
    HuVecF vel;
    float baseAlpha;
    float tpLvl;
    float speed;
    float unk20;
    float gravity;
    float rotSpeed;
    float animTime;
    float animSpeed;
    float scale;
    HuVecF rot;
    HuVecF pos;
    GXColor color;
    int no;
} CAPSULE_EFFDATA;

typedef struct CapsuleEffect_s {
    s16 mode;
    s16 time;
    HuVecF vel;
    s16 work[8];
    u8 blendMode;
    u8 attr;
    u8 dispAttr;
    u8 unk23;
    HU3D_MODELID modelId;
    s16 num;
    u32 count;
    u32 prevCounter;
    u32 prevCount;
    u32 dlSize;
    ANIMDATA *anim;
    CAPSULE_EFFDATA *data;
    HuVecF *vertex;
    HuVec2f *st;
    void *dl;
    void *hook;
    HU3D_MODEL *hookMdlP;
} CAPSULE_EFFECT;

static HU3D_MODELID CapEffCreate(ANIMDATA *anim, s16 num);
static void CapEffHiliteOMExec(OMOBJ *obj);

void mbCapRandomThrowAdd(int capsuleNo, int playerNo, BOOL overwriteF)
{
    int i;
    int type;
    int no;
    int player;
    for(i=1; i<mbMasuNumGet(); i++) {
        type = mbMasuTypeGet(i);
        if(!overwriteF && mbMasuCapsuleGet(i) != -1) {
            continue;
        }
        if(type == 1 || type == 2) {
            if(capsuleNo >= 0) {
                no = capsuleThrowTbl[(capsuleNo-10)%8];
            } else {
                no = capsuleThrowTbl[mbRandMod(8)];
            }
            if(playerNo >= 0) {
                player = playerNo;
            } else {
                player = mbRandMod(4);
            }
            mbCapMasuCapsuleSet(i, no, player);
        }
    }
}

void mbCapRandomTrapAdd(int capsuleNo, int playerNo, BOOL overwriteF)
{
    int i;
    int type;
    int no;
    int player;
    for(i=1; i<mbMasuNumGet(); i++) {
        type = mbMasuTypeGet(i);
        if(!overwriteF && mbMasuCapsuleGet(i) != -1) {
            continue;
        }
        if(type == 1 || type == 2) {
            if(capsuleNo >= 0) {
                no = capsuleTrapTbl[(capsuleNo-20)%5];
            } else {
                no = capsuleTrapTbl[mbRandMod(5)];
            }
            if(playerNo >= 0) {
                player = playerNo;
            } else {
                player = mbRandMod(4);
            }
            mbCapMasuCapsuleSet(i, no, player);
        }
    }
}

void mbCapMasuCapsuleSet(int masuId, int capsuleNo, int playerNo)
{
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    if(capsuleNo > 0x29) {
        return;
    }
    switch(capsuleNo) {
        case 0x1E:
            return;
        case 0x2B:
            return;
    }
    mbCapMasuPlayerTypeSet(masuId, capsuleNo, playerNo);
    switch(capsuleNo) {
        case 0x18:
            break;
    }
}

static void CapThrowEndWin(int playerNo, int capsuleNo)
{
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    switch(capsuleNo) {
        case 0x1E:
            mbWinCreate(2, 0x370025, -1);
            mbWinTopWait();
            break;
        case 0x2B:
            mbWinCreate(2, 0x370025, -1);
            mbWinTopWait();
            break;
    }
}

int mbCapFileGet(int capsuleNo)
{
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    return mbBoardDataNumGet(capsuleData[capsuleNo].file);
}

u32 mbCapDescMesGet(int capsuleNo)
{
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    return capsuleData[capsuleNo].descMes;
}

u32 mbCapUseMesGet(int capsuleNo)
{
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    return capsuleData[capsuleNo].useMes;
}

char *mbCapDebugNameGet(int capsuleNo)
{
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    return capsuleData[capsuleNo].debugName;
}

int mbCapMasuPatGet(int capsuleNo)
{
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    return capsuleData[capsuleNo].masuPat;
}

int mbCapColorGet(int capsuleNo)
{
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    return capsuleData[capsuleNo].color;
}

int mbCapUseCostGet(void)
{
    return 0;
}

BOOL mbCapUseCheck(int capsuleNo)
{
    if(mbCapUseModeGet(capsuleNo) == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL mbCapValidCheck(int capsuleNo)
{
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    if(capsuleNo < 0 || capsuleNo >= 0x35) {
        return FALSE;
    }
    if(capsuleData[capsuleNo].file == 0) {
        return FALSE;
    } else {
        return TRUE;
    }
}

s16 mbCapMasuDispTypeGet(s16 masuId)
{
    s16 capsuleNo = mbMasuCapsuleGet(masuId);
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    if(capsuleNo == 0xFF) {
        return 0;
    }
    if(mbMasuTypeGet(masuId) != 1 && mbMasuTypeGet(masuId) != 2) {
        return 0;
    }
    if(mbCapUseModeGet(capsuleNo) == 1) {
        return 1;
    } else {
        return 2;
    }
}

int mbCapSelectMasuNum(int masuId)
{
    return mbCapSelectMasuBackNum(masuId)+mbCapSelectMasuFrontNum(masuId);
}

int mbCapSelectMasuFrontNum(int masuId)
{
    s16 *list;
    s16 i;
    s16 num;
    list = CapMalloc(0x200);
    memset(list, 0, 0x200);
    capsuleMasuSelectEndF = TRUE;
    CapSelectMasuAddFront(list, masuId, 5);
    for(i=0, num=0; i<0x100; i++) {
        if(list[i] & 1) {
            num++;
        }
    }
    HuMemDirectFree(list);
    return num;
}

int mbCapSelectMasuBackNum(int masuId)
{
    s16 *list;
    s16 i;
    s16 num;
    list = CapMalloc(0x200);
    memset(list, 0, 0x200);
    capsuleMasuSelectEndF = TRUE;
    CapSelectMasuAddBack(list, masuId, 5);
    for(i=0, num=0; i<0x100; i++) {
        if(list[i] & 1) {
            num++;
        }
    }
    HuMemDirectFree(list);
    return num;
}

int mbCapValidListGet(int *list)
{
    int i;
    int num;
    for(i=0, num=0; i<0x21; i++) {
        if(mbCapValidCheck(i)) {
            list[num] = i;
            num++;
        }
    }
    return num;
}

typedef struct CapChanceEntry_s {
    s8 type;
    int sum;
} CAPCHANCE_ENTRY;

int mbCapNextGet(int chanceNo)
{
    int i;
    int n;
    int a;
    int b;
    int num;
    int *tbl;
    int phase;
    int sum;
    int temp;
    int turnNo;
    int type;
    int chance;
    int validList[34];
    int list[33];
    CAPCHANCE_ENTRY chanceList[5];
    num = mbCapValidListGet(validList);
    if(chanceNo < 0) {
        for(i=0; i<num*5; i++) {
            a = i%num;
            b = mbRandMod(num);
            if(a != b) {
                temp = validList[a];
                validList[a] = validList[b];
                validList[b] = temp;
            }
        }
        return validList[mbRandMod(num)];
    }
    for(i=0; i<8U; i++) {
        if(GwSystem.turnMax <= capsuleMaxTurnTbl[i]) {
            break;
        }
    }
    if(i < 9U) {
        turnNo = i;
        for(i=0; i<2; i++) {
            if(GwSystem.turnNo <= capsuleTurnTbl[turnNo][i]) {
                break;
            }
        }
        phase = i;
    } else {
        if(GwSystem.turnNo <= GwSystem.turnMax/3) {
            phase = 0;
        } else if(GwSystem.turnNo <= (GwSystem.turnMax/3)*2) {
            phase = 1;
        } else {
            phase = 2;
        }
    }
    if(chanceNo > 3) {
        chanceNo = 3;
    }
    tbl = capsuleChanceTbl2[phase][chanceNo];
    for(i=0, sum=0, n=0; i<5; i++) {
        if(tbl[i] > 0) {
            chanceList[n].type = i+'A';
            sum += tbl[i];
            chanceList[n].sum = sum;
            n++;
        }
    }
    chance = CAP_RAND_F()*sum;
    for(i=0; i<n; i++) {
        if(chance < chanceList[i].sum) {
            break;
        }
    }
    if(i >= n) {
        i = 0;
    }
    type = chanceList[i].type;
    for(i=0, n=0; i<num; i++) {
        if(type == capsuleData[validList[i]].rank) {
            list[n] = validList[i];
            n++;
        }
    }
    if(n <= 0) {
        return mbCapNextGet(-1);
    }
    for(i=0; i<n*5; i++) {
        a = i%n;
        b = mbRandMod(n);
        if(a != b) {
            temp = list[a];
            list[a] = list[b];
            list[b] = temp;
        }
    }
    i = mbRandMod(n);
    return list[i];
}

int mbCapMasuNextGet(int playerNo)
{
    CAPSULE_LIST *list;
    CAPSULE_LIST *listP;
    CAPSULE_LIST temp;
    int i;
    int j;
    int a;
    int b;
    int num;
    int sum;
    int result;
    int idx;
    if(GwSystem.turnNo <= GwSystem.turnMax/2) {
        idx = GwPlayer[playerNo].rank;
    } else {
        idx = GwPlayer[playerNo].rank+4;
    }
    list = CapMalloc(0x210);
    listP = list;
    for(i=0, num=0, sum=0; i<0x21; i++) {
        if(capsuleList[i].capsuleNo == -1) {
            break;
        }
        if(capsuleList[i].boardRate[idx] > 0) {
            *listP = capsuleList[i];
            for(j=0; j<mbPlayerCapsuleNumGet(playerNo); j++) {
                if(listP->capsuleNo == mbPlayerCapsuleGet(playerNo, j)) {
                    listP->boardRate[idx] /= 2;
                }
            }
            sum += listP->boardRate[idx];
            listP++;
            num++;
        }
    }
    if(num >= 2) {
        for(i=0; i<0x100; i++) {
            a = mbRandMod(num);
            b = mbRandMod(num);
            if(a != b) {
                temp = list[a];
                list[a] = list[b];
                list[b] = temp;
            }
        }
    } else {
        if(num == 1) {
            result = list[0].capsuleNo;
        } else {
            result = -1;
        }
        goto end;
    }
    for(i=0; i<num-1; i++) {
        for(j=i+1; j<num; j++) {
            if(list[i].boardRate[idx] > list[j].boardRate[idx]) {
                temp = list[i];
                list[i] = list[j];
                list[j] = temp;
            }
        }
    }
    result = CAP_RAND_F()*sum;
    for(i=0; i<num; i++) {
        if((result -= list[i].boardRate[idx]) <= 0) {
            break;
        }
    }
    if(i < num) {
        result = list[i].capsuleNo;
    } else {
        result = list[0].capsuleNo;
    }
end:
    HuMemDirectFree(list);
    return result;
}

static int CapShopNextGet(int playerNo, int exclude1, int exclude2)
{
    CAPSULE_LIST *list;
    CAPSULE_LIST *listP;
    CAPSULE_LIST temp;
    int i;
    int j;
    int a;
    int b;
    int num;
    int sum;
    int result;
    int idx;
    idx = GwPlayer[playerNo].rank;
    list = CapMalloc(0x210);
    listP = list;
    for(i=0, num=0, sum=0; i<0x21; i++) {
        if(capsuleList[i].capsuleNo == -1) {
            break;
        }
        if(exclude1 != capsuleList[i].capsuleNo && exclude2 != capsuleList[i].capsuleNo && capsuleList[i].turnRate[idx] > 0) {
            *listP = capsuleList[i];
            for(j=0; j<mbPlayerCapsuleNumGet(playerNo); j++) {
                if(listP->capsuleNo == mbPlayerCapsuleGet(playerNo, j)) {
                    listP->boardRate[idx] /= 2;
                }
            }
            sum += listP->boardRate[idx];
            listP++;
            num++;
        }
    }
    if(num >= 2) {
        for(i=0; i<0x100; i++) {
            a = mbRandMod(num);
            b = mbRandMod(num);
            if(a != b) {
                temp = list[a];
                list[a] = list[b];
                list[b] = temp;
            }
        }
    } else {
        if(num == 1) {
            result = list[0].capsuleNo;
        } else {
            result = -1;
        }
        goto end;
    }
    for(i=0; i<num-1; i++) {
        for(j=i+1; j<num; j++) {
            if(list[i].turnRate[idx] > list[j].turnRate[idx]) {
                temp = list[i];
                list[i] = list[j];
                list[j] = temp;
            }
        }
    }
    result = CAP_RAND_F()*sum;
    for(i=0; i<num; i++) {
        if((result -= list[i].turnRate[idx]) <= 0) {
            break;
        }
    }
    if(i < num) {
        result = list[i].capsuleNo;
    } else {
        result = list[0].capsuleNo;
    }
end:
    HuMemDirectFree(list);
    return result;
}

int mbCapShopListGet(int playerNo, CAPSULE_LIST *out)
{
    int i;
    int j;
    int shop[33];
    shop[0] = CapShopNextGet(playerNo, -1, -1);
    shop[1] = CapShopNextGet(playerNo, shop[0], -1);
    shop[2] = CapShopNextGet(playerNo, shop[0], shop[1]);
    for(i=0; i<3; i++) {
        if(shop[i] == -1) {
            break;
        }
        for(j=0; j<0x21; j++) {
            if(capsuleList[j].capsuleNo == -1) {
                break;
            }
            if(capsuleList[j].capsuleNo == shop[i]) {
                *out = capsuleList[j];
                out++;
                break;
            }
        }
        if(j >= 0x21) {
            break;
        }
    }
    return i;
}

int mbCapRandomListGet(int *out, int max)
{
    CAPSULE_LIST *list;
    CAPSULE_LIST *listP;
    CAPSULE_LIST temp;
    int i;
    int a;
    int b;
    int num;
    int outNum;
    list = CapMalloc(0x210);
    listP = list;
    memset(list, 0, 0x210);
    for(i=0, num=0; i<0x21; i++, listP++) {
        if(capsuleList[i].capsuleNo == -1) {
            break;
        }
        *listP = capsuleList[i];
        num++;
    }
    if(num >= 2) {
        for(i=0; i<0x100; i++) {
            a = mbRandMod(num);
            b = mbRandMod(num);
            if(a != b) {
                temp = list[a];
                list[a] = list[b];
                list[b] = temp;
            }
        }
    }
    for(i=0, outNum=0; i<max; i++) {
        if(list[i].capsuleNo == -1) {
            break;
        }
        out[i] = list[i].capsuleNo;
        outNum++;
    }
    HuMemDirectFree(list);
    return outNum;
}

int mbCapBonusCoinNumGet(int playerNo, int capsuleNo)
{
    int rank = 0;
    int coin = 0;
    switch(GwPlayer[playerNo].rank) {
        case 0:
            rank = 0;
            break;
        case 1:
            rank = 1;
            break;
        case 2:
            rank = 2;
            break;
        case 3:
            rank = 3;
            break;
    }
    switch(mbCapUseModeGet(capsuleNo)) {
        case 0:
            if(CAP_RAND_F()*100 < 20.0f+(rank*10)) {
                coin = mbRandMod(rank+1)+1;
            }
            break;
        case 1:
            if(CAP_RAND_F()*100 < 40.0f+(rank*10)) {
                coin = mbRandMod(rank+3)+5;
            }
            break;
        case 2:
            if(CAP_RAND_F()*100 < 40.0f+(rank*10)) {
                coin = mbRandMod(rank+2)+3;
            }
            break;
    }
    return coin;
}

int mbCapDescWinCreate(int capsuleNo)
{
    int winId;
    HuVec2f pos;
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    winId = mbWinCreate(6, mbCapDescMesGet(capsuleNo), -1);
    mbWinMesSpeedSet(winId, 0);
    mbWinPause(winId);
    mbWinPosGet(winId, &pos);
    pos.y -= 8;
    mbWinPosSet(winId, pos.x, pos.y);
    return winId;
}

void mbCapInit(void)
{
    int i;
    int j;
    s16 *idP;
    int objId;
    CAPOBJCOLORDATA *objP;
    objP = capsuleObjColorData;
    memset(objP, 0, sizeof(capsuleObjColorData));
    for(i=0; i<6; i++) {
        capsuleObjBorderId[i] = -1;
    }
    idP = capsuleBorderObjId = CapMalloc(0x300);
    memset(capsuleBorderObjId, 0, 0x300);
    for(i=0; i<6; i++) {
        for(j=0; j<64; j++, idP++) {
            *idP = -1;
        }
    }
    for(i=0; i<8; i++) {
        capsuleObjData[i].objId = -1;
    }
    objId = mbObjCreate(0xC003D, NULL, TRUE);
    mbObjDispSet(objId, FALSE);
    objId = mbObjCreate(0xC0043, NULL, TRUE);
    mbObjDispSet(objId, FALSE);
}

int mbCapObjCreate(int capsuleNo, int arg1)
{
    int i;
    int objId;
    ANIMDATA *anim;
    Mtx mtx;
    if(capsuleNo == 0x30) {
        for(i=0; i<8; i++) {
            if(capsuleObjData[i].objId == -1) {
                break;
            }
        }
        objId = mbObjCreate(mbCapFileGet(capsuleNo), NULL, FALSE);
        capsuleObjData[i].objId = objId;
        anim = capsuleObjData[i].anim = HuSprAnimRead(HuDataSelHeapReadNum(0xC0027, HU_MEMNUM_OVL, HEAP_MODEL));
        capsuleObjData[i].animId1 = Hu3DAnimCreate(anim, mbObjModelIDGet(objId), "S3TCys77120");
        Hu3DAnmNoSet(capsuleObjData[i].animId1, 0);
        capsuleObjData[i].animId2 = Hu3DAnimLink(capsuleObjData[i].animId1, mbObjModelIDGet(objId), "S3TCys77121");
        Hu3DAnmNoSet(capsuleObjData[i].animId2, 0);
        PSMTXScale(mtx, 1.5f, 1.5f, 1.5f);
        mbObjMtxSet(objId, mtx);
        return objId;
    } else {
        objId = mbObjCreate(mbCapFileGet(capsuleNo), NULL, FALSE);
        mbCapObjBorderCreate(objId, capsuleNo);
        return objId;
    }
}

static int capsuleBorderFileTbl[6] = { 0xC0018, 0xC0019, 0xC001A, 0xC001B, 0, 0 };

s16 mbCapObjBorderCreate(int objId, int capsuleNo)
{
    int borderNo;
    HSF_DATA *hsf;
    s16 *idP;
    HSF_OBJECT *objectP;
    int i;
    HSF_OBJECT *object;
    int j;
    int mdlId;
    char name[256];
    mdlId = mbObjModelIDGet(objId);
    hsf = Hu3DData[mdlId].hsf;
    object = hsf->object;
    strcpy(name, MakeObjectName((s8 *)"center"));
    for(i=0; i<hsf->objectNum; i++, object++) {
        objectP = object;
        if(objectP->constData && strcmp(name, objectP->name) == 0) {
            break;
        }
    }
    if(i >= hsf->objectNum) {
        return -1;
    }
    borderNo = capsuleNo/10;
    if(capsuleObjBorderId[borderNo] < 0 && capsuleBorderFileTbl[borderNo] != 0) {
        capsuleObjBorderId[borderNo] = mbObjCreate(capsuleBorderFileTbl[borderNo], NULL, TRUE);
    }
    if(capsuleObjBorderId[borderNo] >= 0) {
        mbObjHookSet(objId, "center", capsuleObjBorderId[borderNo]);
        idP = &capsuleBorderObjId[borderNo*64];
        for(j=0; j<64; j++, idP++) {
            if(*idP == -1) {
                *idP = objId;
                break;
            }
        }
    }
    return capsuleObjBorderId[borderNo];
}

void mbCapObjKill(int objId)
{
    int i;
    for(i=0; i<8; i++) {
        if(objId == capsuleObjData[i].objId) {
            break;
        }
    }
    if(i < 8) {
        Hu3DAnimKill(capsuleObjData[i].animId1);
        Hu3DAnimKill(capsuleObjData[i].animId2);
        HuSprAnimKill(capsuleObjData[i].anim);
        mbObjKill(capsuleObjData[i].objId);
        capsuleObjData[i].objId = -1;
    } else {
        mbCapObjBorderKill(objId);
        mbObjKill(objId);
    }
}

void mbCapObjBorderKill(int objId)
{
    s16 *idP;
    int i;
    int j;
    idP = capsuleBorderObjId;
    for(i=0; i<6; i++) {
        for(j=0; j<64; j++, idP++) {
            if(*idP == objId) {
                *idP = -1;
            }
        }
    }
    for(i=0; i<6; i++) {
        idP = &capsuleBorderObjId[i*64];
        for(j=0; j<64; j++, idP++) {
            if(*idP >= 0) {
                break;
            }
        }
        if(j >= 64 && capsuleObjBorderId[i] >= 0) {
            mbObjKill(capsuleObjBorderId[i]);
            capsuleObjBorderId[i] = -1;
        }
    }
}

static inline u32 CapColorFileGet(int capsuleNo)
{
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    return capsuleData[capsuleNo].iconFile;
}

int mbCapObjColorCreate(int capsuleNo, BOOL linkF)
{
    CAPOBJCOLORDATA *objP = capsuleObjColorData;
    int i;
    u32 dataNum;
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    for(i=0; i<128; i++, objP++) {
        if(objP->useF == 0) {
            break;
        }
    }
    if(i >= 128) {
        return -1;
    }
    dataNum = CapColorFileGet(capsuleNo);
    objP->useF = 1;
    objP->objId = mbObjCreate(dataNum, NULL, linkF);
    objP->layer = 4;
    objP->pos.x = objP->pos.y = objP->pos.z = 0;
    objP->rot.x = objP->rot.y = objP->rot.z = 0;
    objP->scale.x = objP->scale.y = objP->scale.z = 1;
    mbCapObjColorLayerSet(objP->objId, objP->layer);
    return objP->objId;
}

static inline int CapObjColorNoGet(int objId)
{
    CAPOBJCOLORDATA *objP = capsuleObjColorData;
    int i;
    for(i=0; i<128; i++, objP++) {
        if(objP->useF != 0 && objP->objId == objId) {
            break;
        }
    }
    if(i < 128) {
        return i;
    } else {
        return -1;
    }
}

static inline void CapObjAttrSet(s16 objId, u32 attr)
{
    mbObjAttrSet(objId, attr);
}

static inline void CapObjAttrReset(s16 objId, u32 attr)
{
    mbObjAttrReset(objId, attr);
}

void mbCapObjColorKill(int objId)
{
    CAPOBJCOLORDATA *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        if(objP->useF != 0) {
            mbObjKill(objP->objId);
            objP->objId = -1;
            objP->unk4 = -1;
            objP->useF = 0;
        }
    }
}

void mbCapObjColorPosSet(int objId, float x, float y, float z)
{
    CAPOBJCOLORDATA *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        objP->pos.x = x;
        objP->pos.y = y;
        objP->pos.z = z;
        mbObjPosSetV(objP->objId, &objP->pos);
    }
}

void mbCapObjColorRotSet(int objId, float x, float y, float z)
{
    CAPOBJCOLORDATA *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        objP->rot.x = x;
        objP->rot.y = y;
        objP->rot.z = z;
        mbObjRotSetV(objP->objId, &objP->rot);
    }
}

void mbCapObjColorScaleSet(int objId, float x, float y, float z)
{
    CAPOBJCOLORDATA *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        objP->scale.x = x;
        objP->scale.y = y;
        objP->scale.z = z;
        mbObjScaleSetV(objP->objId, &objP->scale);
    }
}

void mbCapObjColorPosSetV(int objId, HuVecF *pos)
{
    CAPOBJCOLORDATA *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        mbCapObjColorPosSet(objId, pos->x, pos->y, pos->z);
    }
}

void mbCapObjColorRotSetV(int objId, HuVecF *rot)
{
    CAPOBJCOLORDATA *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        mbCapObjColorRotSet(objId, rot->x, rot->y, rot->z);
    }
}

void mbCapObjColorScaleSetV(int objId, HuVecF *scale)
{
    CAPOBJCOLORDATA *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        mbCapObjColorScaleSet(objId, scale->x, scale->y, scale->z);
    }
}

void mbCapObjColorPosGet(int objId, HuVecF *pos)
{
    CAPOBJCOLORDATA *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        *pos = objP->pos;
    }
}

void mbCapObjColorRotGet(int objId, HuVecF *rot)
{
    CAPOBJCOLORDATA *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        *rot = objP->rot;
    }
}

void mbCapObjColorScaleGet(int objId, HuVecF *scale)
{
    CAPOBJCOLORDATA *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        *scale = objP->scale;
    }
}

void mbCapObjColorLayerSet(int objId, u8 layer)
{
    CAPOBJCOLORDATA *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        objP->layer = layer;
        mbObjLayerSet(objP->objId, objP->layer);
    }
}

u8 mbCapObjColorLayerGet(int objId)
{
    CAPOBJCOLORDATA *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) == -1) {
        return 0;
    }
    objP = &capsuleObjColorData[no];
    return objP->layer;
}

void mbCapObjColorAttrSet(int objId, u32 attr)
{
    CAPOBJCOLORDATA *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        CapObjAttrSet(objP->objId, attr);
    }
}

void mbCapObjColorAttrReset(int objId, u32 attr)
{
    CAPOBJCOLORDATA *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        CapObjAttrReset(objP->objId, attr);
    }
}

void mbCapObjColorDispSet(int objId, BOOL dispF)
{
    CAPOBJCOLORDATA *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        mbObjDispSet(objP->objId, dispF);
    }
}

void mbCapObjColorAlphaSet(int objId, u8 alpha)
{
    CAPOBJCOLORDATA *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        mbObjAlphaSet(objP->objId, alpha);
    }
}

void mbCapObjColorMtxSet(int objId, Mtx mtx)
{
    CAPOBJCOLORDATA *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        mbObjMtxSet(objP->objId, mtx);
    }
}

void mbCapObjColorMtxGet(int objId, Mtx mtx)
{
    CAPOBJCOLORDATA *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        mbObjMtxGet(objP->objId, mtx);
    }
}

typedef struct CapGuideWork_s {
    int objId;
    int mode;
    int angle;
    float scale;
} CAPGUIDE_WORK;

static void CapGuideOMExec(OMOBJ *obj);

static void CapGuideCreate(void)
{
    CAPGUIDE_WORK *work;
    int i;
    work = CapMalloc(10*sizeof(CAPGUIDE_WORK));
    memset(work, 0, 10*sizeof(CAPGUIDE_WORK));
    capsuleGuideOMObj = omAddObjEx(mbObjMan, 32768, 0, 0, -1, CapGuideOMExec);
    capsuleGuideOMObj->work[0] = 0;
    capsuleGuideOMObj->data = work;
    for(i=0; i<10; i++, work++) {
        work->objId = mbObjCreate(0xC0023, NULL, TRUE);
        mbObjLayerSet(work->objId, 5);
        mbObjDispSet(work->objId, FALSE);
        work->mode = 0;
        work->angle = 0;
        work->scale = 0;
    }
}

static void CapGuideRotYSet(int masuId, float rotY)
{
    CAPGUIDE_WORK *work = capsuleGuideOMObj->data;
    int i;
    HuVecF masuPos;
    HuVecF pos;
    if(capsuleGuideOMObj == NULL) {
        return;
    }
    mbMasuPosGet(masuId, &masuPos);
    for(i=0; i<10; i++, work++) {
        if(mbObjDispGet(work->objId) != TRUE) {
            int zOfs = 0;
            mbObjRotSet(work->objId, 0, rotY, 0);
            pos.x = masuPos.x+(150*HuSin(rotY));
            pos.y = masuPos.y+10;
            pos.z = (masuPos.z+(150*HuCos(rotY)))+zOfs;
            mbObjPosSetV(work->objId, &pos);
            mbObjScaleSet(work->objId, 0, 0, 0);
            mbObjDispSet(work->objId, TRUE);
            mbObjLayerSet(work->objId, 5);
            CapObjAttrSet(work->objId, HU3D_ATTR_ZCMP_OFF);
            work->mode = 0;
            work->angle = 0;
            work->scale = 0;
            break;
        }
    }
    capsuleGuideOMObj->work[0] = 0;
    capsuleGuideOMObj->work[1] = 1;
}

static void CapGuideOMExec(OMOBJ *obj)
{
    CAPGUIDE_WORK *work = obj->data;
    int i;
    int angle;
    if(mbExitCheck() || capsuleGuideOMObj == NULL) {
        for(i=0; i<10; i++, work++) {
            mbObjKill(work->objId);
        }
        omDelObjEx(mbObjMan, obj);
        capsuleGuideOMObj = NULL;
        return;
    }
    if(capsuleGuideOMObj->work[1]) {
        capsuleGuideOMObj->work[1]--;
        return;
    }
    for(i=0; i<10; i++, work++) {
        if(mbObjDispGet(work->objId) == FALSE) {
            continue;
        }
        if(capsuleGuideOMObj->work[0] == 0) {
            switch(work->mode) {
                case 0:
                    if(work->scale < 2.0f) {
                        work->scale += 0.2;
                    } else {
                        work->scale = 2.0f;
                        work->mode = 1;
                    }
                    mbObjScaleSet(work->objId, work->scale, work->scale, work->scale);
                case 1:
                    if((work->angle += 20) > 360) {
                        work->angle -= 360;
                    }
                    angle = work->angle;
                    work->scale = 1.0+(0.2f*HuSin(angle));
                    break;
            }
        } else {
            if((work->scale -= 0.2f) < 0) {
                work->mode = 0;
                mbObjDispSet(work->objId, FALSE);
            }
            mbObjScaleSet(work->objId, work->scale, work->scale, work->scale);
        }
    }
}

static void CapGuideGrowSet(void)
{
    CAPGUIDE_WORK *work = capsuleGuideOMObj->data;
    if(capsuleGuideOMObj != NULL) {
        capsuleGuideOMObj->work[0] = 1;
    }
}

static void CapGuideKill(void)
{
    capsuleGuideOMObj = NULL;
}

typedef struct CapEffMasuOkWork_s {
    int objId;
    int mode;
    int masuId;
    int unkC;
    float scale;
} CAPEFFMASUOK_WORK;

static void CapEffMasuOkOMExec(OMOBJ *obj);

static void CapEffMasuOkCreate(void)
{
    OMOBJ *obj;
    CAPEFFMASUOK_WORK *work;
    int i;
    obj = capEffMasuOkOMObj = omAddObjEx(mbObjMan, 32768, 0, 0, -1, CapEffMasuOkOMExec);
    work = obj->data = HuMemDirectMallocNum(HEAP_HEAP, 33*sizeof(CAPEFFMASUOK_WORK), HU_MEMNUM_OVL);
    memset(work, 0, 33*sizeof(CAPEFFMASUOK_WORK));
    for(i=0; i<32; i++, work++) {
        work->objId = mbObjCreate(0xC0024, NULL, TRUE);
        mbObjDispSet(work->objId, FALSE);
        CapObjAttrSet(work->objId, 0x40000001);
        mbObjLayerSet(work->objId, 5);
        work->masuId = 0;
        work->mode = 0;
        work->unkC = 0;
        work->scale = 0;
    }
    work->objId = mbObjCreate(0xC0025, NULL, TRUE);
    mbObjDispSet(work->objId, FALSE);
    CapObjAttrSet(work->objId, 0x40000001);
    mbObjLayerSet(work->objId, 5);
    work->masuId = 0;
    work->mode = 0;
    work->unkC = 0;
    work->scale = 0;
}

static void CapEffMasuOkAddAll(s16 masuId, s16 *masuList)
{
    OMOBJ *obj = capEffMasuOkOMObj;
    CAPEFFMASUOK_WORK *work = obj->data;
    int i;
    int num;
    HuVecF pos;
    HuVecF rot;
    for(i=1, num=0; i<=mbMasuNumGet(); i++) {
        if(masuList[i] & 1) {
            mbMasuPosGet(i, &pos);
            mbMasuRotGet(i, &rot);
            pos.y += 10;
            mbObjPosSetV(work->objId, &pos);
            mbObjRotSetV(work->objId, &rot);
            work->masuId = i;
            work->mode = 1;
            num++;
            work++;
        }
    }
}

static void CapEffMasuOkOMExec(OMOBJ *obj)
{
    CAPEFFMASUOK_WORK *work = obj->data;
    int i;
    HuVecF pos;
    HuVecF rot;
    if(mbExitCheck() || capEffMasuOkOMObj == NULL) {
        for(i=0; i<32; i++, work++) {
            mbObjKill(work->objId);
        }
        mbObjKill(work->objId);
        omDelObjEx(mbObjMan, obj);
        capEffMasuOkOMObj = NULL;
        return;
    }
    for(i=0; i<32; i++, work++) {
        if(work->masuId > 0) {
            mbMasuPosGet(work->masuId, &pos);
            mbMasuRotGet(work->masuId, &rot);
            pos.y += 10;
            mbObjPosSetV(work->objId, &pos);
            mbObjRotSetV(work->objId, &rot);
        }
    }
    if(work->masuId > 0) {
        mbMasuPosGet(work->masuId, &pos);
        mbMasuRotGet(work->masuId, &rot);
        pos.y += 10;
        mbObjPosSetV(work->objId, &pos);
        mbObjRotSetV(work->objId, &rot);
    }
    switch(work->mode) {
        case 1:
            work->mode++;
            break;
        case 2:
            work->scale += 0.2f;
            if(work->scale >= 1.0f) {
                work->scale = 1.0f;
                work->mode++;
            }
            mbObjScaleSet(work->objId, work->scale, work->scale, work->scale);
            break;
        case 10:
            work->scale -= 0.2f;
            if(work->scale <= 0.001f) {
                mbObjDispSet(work->objId, FALSE);
                work->scale = 0.001f;
                work->mode++;
            }
            mbObjScaleSet(work->objId, work->scale, work->scale, work->scale);
            break;
    }
}

static void CapEffMasuOkDispSet(BOOL dispF)
{
    OMOBJ *obj = capEffMasuOkOMObj;
    CAPEFFMASUOK_WORK *work = obj->data;
    int i;
    for(i=0; i<32; i++, work++) {
        if(work->mode == 1 && dispF) {
            mbObjDispSet(work->objId, TRUE);
        } else {
            mbObjDispSet(work->objId, FALSE);
        }
    }
}

static void CapEffMasuOkKill(void)
{
    capEffMasuOkOMObj = NULL;
}

static void CapEffMasuOkPosSet(HuVecF *pos, int masuId)
{
    OMOBJ *obj = capEffMasuOkOMObj;
    CAPEFFMASUOK_WORK *work = obj->data;
    HuVecF effPos;
    work += 32;
    effPos = *pos;
    effPos.y += 10;
    mbObjPosSetV(work->objId, &effPos);
    mbObjDispSet(work->objId, TRUE);
    mbObjScaleSet(work->objId, 0.001f, 0.001f, 0.001f);
    work->masuId = masuId;
    work->mode = 1;
    work->scale = 0;
}

static void CapEffMasuOkNext(void)
{
    OMOBJ *obj = capEffMasuOkOMObj;
    CAPEFFMASUOK_WORK *work = obj->data;
    work += 32;
    work->mode = 10;
}

typedef struct CapEffRemoveWork_s {
    int mdlId;
    int effNum;
    ANIMDATA *anim;
} CAPEFFREMOVE_WORK;

static void CapEffRemoveOMExec(OMOBJ *obj);

static void CapEffRemoveCreate(void)
{
    CAPEFFREMOVE_WORK *work;
    OMOBJ *obj;
    CAPSULE_EFFECT *effP;
    HU3D_MODEL *modelP;
    ANIMDATA *anim;
    int mdlId;
    obj = capEffRemoveOMObj = omAddObjEx(mbObjMan, 32768, 0, 0, -1, CapEffRemoveOMExec);
    work = obj->data = CapMalloc(sizeof(CAPEFFREMOVE_WORK));
    memset(work, 0, sizeof(CAPEFFREMOVE_WORK));
    work->anim = anim = HuSprAnimRead(HuDataReadNum(0xC0028, HU_MEMNUM_OVL));
    work->mdlId = mdlId = CapEffCreate(anim, 64);
    Hu3DModelLayerSet(work->mdlId, 5);
    work->effNum = 0;
    modelP = &Hu3DData[work->mdlId];
    effP = modelP->hookData;
    effP->blendMode = 0;
}

static void CapEffRemoveOMExec(OMOBJ *obj)
{
    CAPSULE_EFFDATA *effDataP;
    CAPEFFREMOVE_WORK *work;
    CAPSULE_EFFECT *effP;
    int i;
    HU3D_MODEL *modelP;
    work = obj->data;
    if(mbExitCheck() || capEffRemoveOMObj == NULL) {
        Hu3DModelKill(work->mdlId);
        work->mdlId = -1;
        HuSprAnimKill(work->anim);
        work->anim = NULL;
        omDelObjEx(mbObjMan, obj);
        capEffRemoveOMObj = NULL;
        return;
    }
    if(work->effNum <= 0) {
        Hu3DModelDispOff(work->mdlId);
    } else {
        Hu3DModelDispOn(work->mdlId);
        modelP = &Hu3DData[work->mdlId];
        effP = modelP->hookData;
        effDataP = effP->data;
        effP->unk23 = 0;
        for(i=0; i<effP->num; i++, effDataP++) {
            if(effDataP->scale <= 0) {
                continue;
            }
            effDataP->pos.x += effDataP->vel.x;
            effDataP->pos.y += effDataP->vel.y;
            effDataP->pos.z += effDataP->vel.z;
            effDataP->rot.z += effDataP->speed;
            if(effDataP->rot.z >= 360) {
                effDataP->rot.z -= 360;
            }
            effDataP->no = effDataP->animTime;
            effDataP->animTime += effDataP->animSpeed;
            if(effDataP->no >= 16) {
                effDataP->no = 0;
                effDataP->time = 0;
                effDataP->scale = 0;
                work->effNum--;
            }
        }
    }
}

static void CapEffRemoveKill(void)
{
    capEffRemoveOMObj = NULL;
}

static int CapEffRemoveCheck(void)
{
    OMOBJ *obj = capEffRemoveOMObj;
    CAPEFFREMOVE_WORK *work;
    if(capEffRemoveOMObj == NULL) {
        return 0;
    } else {
        work = obj->data;
        return work->effNum;
    }
}

static inline int CapEffRemoveAddData(HuVecF pos, HuVecF vel, float size, float speed, float radius, float animSpeed, GXColor color)
{
    CAPSULE_EFFDATA *effDataP;
    CAPSULE_EFFECT *effP;
    int i;
    CAPEFFREMOVE_WORK *work;
    OMOBJ *obj;
    HU3D_MODEL *modelP;
    obj = capEffRemoveOMObj;
    work = obj->data;
    modelP = &Hu3DData[work->mdlId];
    effP = modelP->hookData;
    effDataP = effP->data;
    for(i=0; i<effP->num; i++, effDataP++) {
        if(effDataP->scale <= 0) {
            break;
        }
    }
    if(i >= effP->num) {
        return -1;
    }
    effDataP->time = effDataP->work = 0;
    effDataP->pos.x = pos.x;
    effDataP->pos.y = pos.y;
    effDataP->pos.z = pos.z;
    effDataP->vel.x = vel.x;
    effDataP->vel.y = vel.y;
    effDataP->vel.z = vel.z;
    effDataP->speed = speed;
    effDataP->scale = size;
    effDataP->color = color;
    effDataP->rot.z = 0;
    effDataP->no = 0;
    effDataP->time = 0;
    effDataP->animTime = 0;
    effDataP->animSpeed = animSpeed;
    work->effNum++;
    return i;
}

static int CapEffRemoveAdd(HuVecF pos, HuVecF vel, float size, float speed, float radius, float animSpeed, GXColor color)
{
    HuVecF finalPos;
    HuVecF dir;
    float dist;
    int out1, out2;
    dir.x = vel.z;
    dir.z = vel.x;
    dir.y = 0;
    if(VECMag(&dir) > 0) {
        VECNormalize(&dir, &dir);
    }
    dist = 0.5f*radius;
    finalPos.x = pos.x+(dir.x*dist);
    finalPos.y = pos.y+(dir.y*dist);
    finalPos.z = pos.z+(dir.z*dist);
    out1 = CapEffRemoveAddData(finalPos, vel, size, speed, radius, animSpeed, color);
    finalPos.x = pos.x-(dir.x*dist);
    finalPos.y = pos.y-(dir.y*dist);
    finalPos.z = pos.z-(dir.z*dist);
    out2 = CapEffRemoveAddData(finalPos, vel, size, -speed, radius, animSpeed, color);
    return (out1 << 16)|out2;
}

static void CapEffRemoveAddAll(HuVecF pos)
{
    int i;
    for(i=0; i<32; i++) {
        HuVecF effPos;
        HuVecF effVel;
        GXColor effColor;
        float angle;
        float dist;
        angle = i*11.25f;
        dist = (CAP_RAND_F()*100)*0.33f;
        effPos.x = pos.x+(dist*HuCos(angle));
        effPos.y = pos.y+(dist*HuSin(angle))+100;
        effPos.z = pos.z+50;
        dist = ((CAP_RAND_F()*0.04f)+0.005f)*100;
        effVel.x = dist*HuCos(angle);
        effVel.y = dist*HuSin(angle);
        effVel.z = 0;
        dist = CAP_RAND_F();
        effColor.r = 192+(32*dist);
        effColor.g = 192+(32*dist);
        effColor.b = 192+(32*dist);
        effColor.a = (CAP_RAND_F()*63)+192;
        CapEffRemoveAdd(effPos, effVel, ((CAP_RAND_F()*0.5f)+1)*100, (CAP_RAND_F()*0.25f)+0.12f, ((CAP_RAND_F()*0.5f)+0.5f)*100, (CAP_RAND_F()*0.66f)+0.33f, effColor);
    }
}

static void CapEffRemoveAddDestroy(void)
{
    capEffRemoveAddOMObj = NULL;
}

static int capsuleHiliteFileTbl[3] = { 0xC0031, 0xC0032, 0xC0033 };

typedef struct CapEffHiliteWork_s {
    int mdlId[3];
    int effNum;
    ANIMDATA *anim[3];
} CAPEFFHILITE_WORK;

static void CapEffHiliteCreate(void)
{
    CAPEFFHILITE_WORK *work;
    OMOBJ *obj;
    CAPSULE_EFFECT *effP;
    HU3D_MODEL *modelP;
    ANIMDATA *anim;
    int mdlId;
    int i;
    obj = capEffHiliteOMObj = omAddObjEx(mbObjMan, 32768, 0, 0, -1, CapEffHiliteOMExec);
    work = obj->data = CapMalloc(sizeof(CAPEFFHILITE_WORK));
    memset(work, 0, sizeof(CAPEFFHILITE_WORK));
    for(i=0; i<3; i++) {
        work->anim[i] = anim = HuSprAnimRead(HuDataReadNum(capsuleHiliteFileTbl[i], HU_MEMNUM_OVL));
        work->mdlId[i] = mdlId = CapEffCreate(anim, 32);
        Hu3DModelLayerSet(mdlId, 5);
        work->effNum = 0;
        modelP = &Hu3DData[mdlId];
        effP = modelP->hookData;
        effP->blendMode = 1;
        effP->dispAttr = 14;
    }
}
/* @@PART4@@ */
/* Own-file only: stub bodies for part 2 and part 4 statics so the calls resolve. */
static void CapSelectMasuAddFront(s16 *list, s16 masuId, s16 num)
{
}
static void CapSelectMasuAddBack(s16 *list, s16 masuId, s16 num)
{
}
static void CapEffHiliteOMExec(OMOBJ *obj)
{
}
static HU3D_MODELID CapEffCreate(ANIMDATA *anim, s16 num)
{
    return 0;
}
/* @@END@@ */
