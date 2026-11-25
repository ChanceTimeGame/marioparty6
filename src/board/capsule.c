/* @@HEAD@@ */
#include "game/board/capsule.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/branch.h"
#include "game/board/capselect.h"
#include "game/board/exit.h"
#define mbMasuFind_TypeStepGet mbMasuFind_TypeStepGet_s16
#define mbMasuCapsuleGet mbMasuCapsuleGet_s16
#include "game/board/masu.h"
#undef mbMasuFind_TypeStepGet
#undef mbMasuCapsuleGet
int mbMasuFind_TypeStepGet(s16 id, s16 type);
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
    u32 capsuleNo;
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
static s16 capsuleNum[33][2];
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

/* @@PART1@@ */
static int CapUse(int playerNo, int capsuleNo);
static int CapUseSelect(CAPUSEWORK *work);
static void CapEffUse(void);
static void CapEffUseKill(void);
static void CapPlayerThrow(void);
static void CapPlayerThrowKill(void);
static void CapEffThrowCreate(int playerNo, float *posX, float *posY, float *posZ, float height, int masuId);
static void CapEffThrowHook(HU3D_MODEL *modelP, Mtx *mtx);
static int CapEffThrowCheck(HuVecF *pos, int *maxTime);
static void CapEffThrowKill(void);
static void CapEffThrowMasuCreate(int masuId, int capsuleNo);
static int CapEffThrowMasu(int masuId, int capsuleNo, int playerNo, BOOL coinF);
static int CapEffThrowMasuWait(int coinNum);
static void CapAutoThrow(CAPAUTOTHROWWORK *work);

int mbCapUse(int playerNo, int capsuleNo)
{
    s32 selObj;
    s32 selArg;
    int masuId;
    int mesId;
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    capsuleUseRemoveOnF = FALSE;
    if(capsuleNo == 47) {
        while(!mbCapSelectShrinkCheck(playerNo)) {
            HuPrcVSleep();
        }
        mbCapSelectResultGet(playerNo, &selObj, &selArg);
        mbCapSelectResultReset(playerNo);
        if(selObj != -1) {
            mbCapObjKill(selObj);
        }
        return TRUE;
    } else if(capsuleNo == 48) {
        mbWinCreateChoice(2, 0x37003A, -1, 1);
        if(GwPlayer[playerNo].comF) {
            CapComChoiceSet(0);
        }
        mbWinTopWait();
        if(mbWinTopChoiceGet() == 0) {
            mbPauseDisableSet(TRUE);
            mesId = GameMesCreate(6, 1);
            while(GameMesStatGet(mesId)) {
                HuPrcVSleep();
            }
            mbWipeFadeOut();
            mbSingleReturn();
        }
        return FALSE;
    } else if(mbPlayerCapsuleFind(playerNo, capsuleNo) == -1) {
        return TRUE;
    } else {
        masuId = mbCapSelectMasu(playerNo, capsuleNo);
        if(masuId != -1) {
            if(masuId == -2) {
                if(CapUseDelete(playerNo, capsuleNo)) {
                    return TRUE;
                } else {
                    return FALSE;
                }
            } else if(masuId == -3) {
                if(CapUse(playerNo, capsuleNo)) {
                    mbev_CapCall(playerNo, capsuleNo, 1, 0);
                    if(GWPartyGet() == FALSE) {
                        mbSingleCall(4, capsuleNo);
                    }
                } else {
                    return FALSE;
                }
            } else {
                mbCapPlayerThrow(playerNo, masuId, capsuleNo);
                if(GWPartyGet() == FALSE) {
                    mbSingleCall(4, capsuleNo);
                }
                return TRUE;
            }
        } else {
            return FALSE;
        }
    }
    return TRUE;
}

void MBCapsuleStub1(void)
{
}

int MBCapsuleStub2(void)
{
    return 0;
}

static int CapUse(int playerNo, int capsuleNo)
{
    CAPUSEWORK *work;
    int result;
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    work = CapMalloc(sizeof(CAPUSEWORK));
    memset(work, 0, sizeof(CAPUSEWORK));
    work->playerNo = playerNo;
    work->capsuleNo = capsuleNo;
    result = CapUseSelect(work);
    HuMemDirectFree(work);
    return result;
}

static int CapUseSelect(CAPUSEWORK *work)
{
    float t;
    float scale;
    int objId;
    int i;
    int capsuleIdx;
    HuVecF playerPos;
    HuVecF objPos;
    HuVecF pos;
    int winId;
    s32 selObj;
    s32 selArg;
    mbCapSelectResultGet(work->playerNo, &selObj, &selArg);
    mbCapSelectResultReset(work->playerNo);
    if(selObj != -1) {
        objId = selObj;
    } else {
        objId = mbCapObjCreate(work->capsuleNo, 0);
    }
    mbPlayerPosGet(work->playerNo, &playerPos);
    objPos = playerPos;
    objPos.y += 250.0f;
    if(selObj != -1) {
        mbObjPosGet(objId, &objPos);
        objPos.x = playerPos.x;
    }
    mbObjPosSet(objId, objPos.x, objPos.y, objPos.z);
    mbObjLayerSet(objId, 4);
    mbObjAttrSet(objId, 0x40000001);
    if(selObj == -1) {
        mbObjDispSet(objId, TRUE);
        i = 0;
        do {
            i++;
            t = i/18.0f;
            if(t > 1.0f) {
                t = 1.0f;
            }
            mbPlayerPosGet(work->playerNo, &pos);
            pos.y += 100.0+(150.0*HuSin(90.0f*t));
            mbObjPosSetV(objId, &pos);
            scale = 1.2f*HuSin(90.0f*t);
            mbObjScaleSet(objId, scale, scale, scale);
            HuPrcVSleep();
        } while(!mbCameraMoveCheck() || t < 1.0f);
    }
    if(!GwPlayer[work->playerNo].comF) {
        winId = mbWinCreateChoice(2, 0x370027, -1, 0);
        mbWinTopInsertMesSet(mbCapUseMesGet(work->capsuleNo), 0);
        if(GwPlayer[work->playerNo].comF) {
            CapComChoiceSet(0);
        }
        mbWinTopWait();
        if(mbWinTopChoiceGet() != 0 || mbWinTopChoiceGet() == -1) {
            if(selObj == -1) {
                for(i=0; i<=18.0f; i++) {
                    t = 1.0f-(i/18.0f);
                    if(t < 0.0f) {
                        t = 0.0f;
                    }
                    mbPlayerPosGet(work->playerNo, &pos);
                    pos.y += 100.0+(150.0*HuSin(90.0f*t));
                    mbObjPosSetV(objId, &pos);
                    scale = 1.2f*HuSin(90.0f*t);
                    mbObjScaleSet(objId, scale, scale, scale);
                    HuPrcVSleep();
                }
            } else {
                mbCapSelectResultSet(work->playerNo, selObj, selArg);
                objId = -1;
            }
            if(objId != -1) {
                mbCapObjKill(objId);
            }
            return FALSE;
        }
    }
    if(!capsuleUseRemoveOnF) {
        mbPlayerCapsuleUseSet(work->capsuleNo);
        capsuleIdx = mbPlayerCapsuleFind(work->playerNo, work->capsuleNo);
        if(capsuleIdx != -1) {
            mbPlayerCapsuleRemove(work->playerNo, capsuleIdx);
        }
    }
    if(selArg == -1) {
        selArg = 0;
    }
    mbCapSelectResultSet(work->playerNo, objId, selArg);
    return TRUE;
}

int mbCapEffUseCreate(int playerNo, int capsuleNo)
{
    CAPUSEWORK *work;
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    capsuleUseEffProc[playerNo] = HuPrcChildCreate(CapEffUse, 0x2004, 0x6000, 0, mbMainProc);
    capsuleUseEffProc[playerNo]->property = work = CapMalloc(sizeof(CAPUSEWORK));
    memset(work, 0, sizeof(CAPUSEWORK));
    work->playerNo = playerNo;
    work->capsuleNo = capsuleNo;
    capsuleUseEffMode[playerNo] = 0;
    capsuleUseEffPos[playerNo].x = capsuleUseEffPos[playerNo].y = capsuleUseEffPos[playerNo].z = 0.0f;
    HuPrcDestructorSet2(capsuleUseEffProc[playerNo], CapEffUseKill);
    return TRUE;
}

int mbCapEffUseModeGet(int playerNo)
{
    return capsuleUseEffMode[playerNo];
}

int mbCapEffUsePosGet(int playerNo, HuVecF *pos)
{
    int mode = capsuleUseEffMode[playerNo];
    if(mode < 1) {
        return FALSE;
    }
    *pos = capsuleUseEffPos[playerNo];
    return TRUE;
}

int mbCapEffUseWanWanCreate(int playerNo, int capsuleNo, HuVecF *pos)
{
    CAPUSEWORK *work;
    int objId;
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    capsuleUseEffProc[playerNo] = HuPrcChildCreate(CapEffUse, 0x2004, 0x6000, 0, mbMainProc);
    work = CapMalloc(sizeof(CAPUSEWORK));
    capsuleUseEffProc[playerNo]->property = work;
    memset(work, 0, sizeof(CAPUSEWORK));
    work->playerNo = playerNo;
    work->capsuleNo = capsuleNo;
    capsuleUseEffMode[playerNo] = 0;
    capsuleUseEffPos[playerNo].x = capsuleUseEffPos[playerNo].y = capsuleUseEffPos[playerNo].z = 0.0f;
    objId = mbCapObjCreate(capsuleNo, 0);
    mbObjPosSetV(objId, pos);
    mbObjDispSet(objId, FALSE);
    mbCapSelectResultSet(work->playerNo, objId, 0);
    HuPrcDestructorSet2(capsuleUseEffProc[playerNo], CapEffUseKill);
    return TRUE;
}

static void CapEffUse(void)
{
    CAPUSEWORK *work;
    int i;
    int objId;
    int j;
    OMOBJ *rayObj;
    OMOBJ *masuHitObj;
    OMOBJ *glowObj;
    int k;
    int time;
    float t;
    float alpha;
    float angle;
    float glowSpeed;
    float glowSize;
    Mtx rotMtx;
    HuVecF playerPos;
    HuVecF objPos;
    HuVecF rot;
    HuVecF rotEnd;
    HuVecF effPos;
    HuVecF vel;
    HuVecF pos;
    GXColor color;
    s32 selObj;
    s32 selArg;
    work = HuPrcCurrentGet()->property;
    rayObj = mbev_CapEffRayCreate(0.0f, 0.02f);
    masuHitObj = mbev_CapEffMasuHitCreate();
    glowObj = mbev_CapEffGlowCreate();
    mbCapSelectResultGet(work->playerNo, &selObj, &selArg);
    mbCapSelectResultReset(work->playerNo);
    if(selObj != -1) {
        objId = selObj;
    } else {
        objId = mbCapObjCreate(work->capsuleNo, 0);
    }
    mbPlayerPosGet(work->playerNo, &playerPos);
    objPos = playerPos;
    objPos.y += 250.0f;
    if(selObj != -1) {
        mbObjPosGet(objId, &objPos);
        objPos.x = playerPos.x;
    }
    mbObjPosSet(objId, objPos.x, objPos.y, objPos.z);
    mbObjLayerSet(objId, 4);
    mbObjAttrSet(objId, 0x40000001);
    mbev_CapEffRayTransformSet(rayObj, &objPos, NULL, NULL);
    mbev_CapEffMasuHitTransformSet(masuHitObj, &objPos, NULL, NULL);
    if(selObj == -1) {
        mbObjDispSet(objId, TRUE);
        k = 0;
        do {
            k++;
            t = k/18.0f;
            if(t > 1.0f) {
                t = 1.0f;
            }
            mbPlayerPosGet(work->playerNo, &pos);
            pos.y += 100.0+(150.0*HuSin(90.0f*t));
            mbObjPosSetV(objId, &pos);
            alpha = 1.2f*HuSin(90.0f*t);
            mbObjScaleSet(objId, alpha, alpha, alpha);
            HuPrcVSleep();
        } while(!mbCameraMoveCheck() || t < 1.0f);
    }
    capsuleUseEffMode[work->playerNo] = 1;
    capsuleUseEffPos[work->playerNo] = objPos;
    mbAudFXPlay(0x40A);
    for(i=0; i<30.0f; i++) {
        t = i/30.0f;
        alpha = HuCos(90.0f*t);
        for(j=0; j<3; j++) {
            effPos.x = effPos.y = effPos.z = 0.0f;
            rot.x = 360.0f*CAP_RAND_F();
            rot.y = 360.0f*CAP_RAND_F();
            rot.z = 360.0f*CAP_RAND_F();
            rotEnd.x = rot.x+(45.0f*(-0.5f+CAP_RAND_F()));
            rotEnd.y = rot.y+(45.0f*(-0.5f+CAP_RAND_F()));
            rotEnd.z = rot.z+(45.0f*(-0.5f+CAP_RAND_F()));
            mbev_CapEffRayAdd(rayObj, &effPos, &rot, &rotEnd, 200.0f*(1.0f+(0.25f*CAP_RAND_F())), 15.0f+(5.0f*CAP_RAND_F()));
        }
        mbev_CapEffRayAlphaSet(rayObj, alpha);
        for(j=0; j<5; j++) {
            effPos.x = effPos.y = effPos.z = 0.0f;
            rot.x = 360.0f*CAP_RAND_F();
            rot.y = 360.0f*CAP_RAND_F();
            rot.z = 360.0f*CAP_RAND_F();
            rotEnd.x = 360.0f*(-0.5f+CAP_RAND_F());
            rotEnd.y = 360.0f*(-0.5f+CAP_RAND_F());
            rotEnd.z = 360.0f*(-0.5f+CAP_RAND_F());
            mbev_CapEffMasuHitAdd(masuHitObj, &effPos, &rot, &rotEnd, (100.0f*alpha)*(1.5f+CAP_RAND_F()), (25.0f*alpha)*(1.0f+(0.5f*CAP_RAND_F())), 20.0f+(10.0f*CAP_RAND_F()));
        }
        HuPrcVSleep();
    }
    mtxRot(rotMtx, 0.0f, 0.0f, 20.0f);
    for(i=0; i<256; i++) {
        effPos.x = objPos.x+(75.0f*(-0.5f+CAP_RAND_F()));
        effPos.y = objPos.y+(75.0f*(-0.5f+CAP_RAND_F()));
        effPos.z = 30.000002f+objPos.z;
        angle = 360.0f*CAP_RAND_F();
        alpha = 5.0f*(0.5f+CAP_RAND_F());
        vel.x = alpha*HuSin(angle);
        vel.y = alpha*(0.3f*(-0.5f+CAP_RAND_F()));
        vel.z = alpha*HuCos(angle);
        PSMTXMultVec(rotMtx, &vel, &vel);
        mbev_CapEffColorSet(&color, mbRandMod(0x8000));
        mbev_CapEffGlowAdd(glowObj, effPos, vel, 60.0f*(0.4f+(0.5f*CAP_RAND_F())), 100.0f*(0.3f+(0.2f*CAP_RAND_F())), 5.0f*(-0.5f+CAP_RAND_F()), 0.0f, color);
        if(i == 128) {
            HuPrcVSleep();
        }
    }
    capsuleUseEffMode[work->playerNo] = 2;
    capsuleUseEffPos[work->playerNo] = objPos;
    mbObjDispSet(objId, FALSE);
    for(i=0; i<20; i++) {
        t = i/20.0f;
        alpha = HuCos(90.0f*t);
        mbObjScaleSet(objId, alpha, alpha, alpha);
        HuPrcVSleep();
    }
    capsuleObjId = -1;
    while(mbev_CapEffGlowDispGet(glowObj)) {
        HuPrcVSleep();
    }
    if(objId != -1) {
        mbCapObjKill(objId);
    }
    mbev_CapEffRayKill(rayObj);
    mbev_CapEffMasuHitKill(masuHitObj);
    mbev_CapEffGlowKill(glowObj);
    HuPrcEnd();
}

static void CapEffUseKill(void)
{
    CAPUSEWORK *work = HuPrcCurrentGet()->property;
    CapEffRemoveAddDestroy();
    CapEffHiliteKill();
    capsuleUseEffProc[work->playerNo] = NULL;
    capsuleUseEffMode[work->playerNo] = -1;
    HuMemDirectFree(work);
}

void mbCapPlayerThrow(int playerNo, int masuId, int capsuleNo)
{
    CAPPLAYERTHROWWORK *work;
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    capsulePlayerThrowProc = HuPrcChildCreate(CapPlayerThrow, 0x2004, 0x6000, 0, mbMainProc);
    work = CapMalloc(sizeof(CAPPLAYERTHROWWORK));
    capsulePlayerThrowProc->property = work;
    memset(work, 0, sizeof(CAPPLAYERTHROWWORK));
    work->playerNo = playerNo;
    work->capsuleNo = capsuleNo;
    work->masuId = masuId;
    work->objId = -1;
    work->lineObjId = -1;
    work->colorObjId = -1;
    work->time = 0;
    HuPrcDestructorSet2(capsulePlayerThrowProc, CapPlayerThrowKill);
    while(!mbCapPlayerThrowCheck()) {
        HuPrcVSleep();
    }
}

int mbCapPlayerThrowCheck(void)
{
    if(capsulePlayerThrowProc) {
        return FALSE;
    } else {
        return TRUE;
    }
}

int mbCapThrowColCreate(int dataNum)
{
    int objId;
    if(dataNum != -1) {
        objId = mbObjCreate(dataNum, NULL, FALSE);
        mbObjDispSet(objId, FALSE);
        capsuleColObjId = objId;
        CapColMdlIdGet();
    } else {
        capsuleColObjId = -1;
        CapColKill();
    }
    return objId;
}

int mbCapThrowColCheck(HuVecF *pos, HuVecF *next, HuVecF *out)
{
    int result = CapColCheck(pos, next, out);
    return result;
}

static void CapPlayerThrow(void)
{
    CAPPLAYERTHROWWORK *work;
    int seNo;
    int capsuleIdx;
    int maxTime;
    int playerObjId;
    int playerMdlId;
    int coinNum;
    int motTime;
    float rate;
    float time;
    float scale;
    float ofs;
    float dist;
    float glowSize;
    Mtx rotMtx;
    Mtx objMtx;
    Mtx mtx;
    HuVecF camPos;
    HuVecF rot;
    HuVecF linePos;
    HuVecF dir;
    HuVecF dirOld;
    HuVecF pos;
    HuVecF vel;
    HuVecF startPos2;
    HuVecF startPos;
    HuVecF objPos;
    HuVecF handPos;
    float posX[3];
    float posY[3];
    float posZ[3];
    HuVecF out;
    GXColor color;
    float mag;
    work = HuPrcCurrentGet()->property;
    if(capsuleThrowHook) {
        capsuleThrowHook(TRUE);
    }
    CapEffThrowMasuCreate(work->masuId, work->capsuleNo);
    work->motId = mbPlayerMotionCreate(work->playerNo, 0x93005E);
    HuPrcVSleep();
    if(capsuleObjId != -1) {
        work->objId = capsuleObjId;
    } else {
        work->objId = mbObjCreate(mbCapFileGet(work->capsuleNo), NULL, FALSE);
    }
    mbPlayerPosGet(work->playerNo, &pos);
    mbObjPosSet(work->objId, pos.x, 250.0f+pos.y, pos.z);
    mbObjScaleSet(work->objId, 1.2f, 1.2f, 1.2f);
    mbObjLayerSet(work->objId, 4);
    work->lineObjId = mbObjCreate(mbCapFileGet(work->capsuleNo), NULL, FALSE);
    mbObjDispSet(work->lineObjId, FALSE);
    while(mbObjMotionShiftIDGet(mbPlayerObjIDGet(work->playerNo)) != -1) {
        HuPrcVSleep();
    }
    playerObjId = mbPlayerObjIDGet(work->playerNo);
    playerMdlId = mbObjModelIDGet(playerObjId);
    motTime = mbObjMotionTimeGet(playerObjId);
    mbPlayerRotGet(work->playerNo, &rot);
    mbPlayerPosGet(work->playerNo, &work->pos);
    mbMasuPosGet(work->masuId, &work->masuPos);
    VECSubtract(&work->masuPos, &work->pos, &dir);
    mbPlayerRotSet(work->playerNo, 0.0f, HuAtan(dir.x, dir.z), 0.0f);
    mbPlayerMotionSet(work->playerNo, work->motId, HU3D_MOTATTR_NONE);
    mbPlayerMotionTimeSet(work->playerNo, 0.35f*mbPlayerMotionMaxTimeGet(work->playerNo));
    Hu3DMotionCalc(playerMdlId);
    Hu3DModelObjMtxGet(mbObjModelIDGet(mbPlayerObjIDGet(work->playerNo)), CapHookNameGet(work->playerNo), mtx);
    work->pos.x = mtx[0][3];
    work->pos.y = mtx[1][3];
    work->pos.z = mtx[2][3];
    mbPlayerMotionSet(work->playerNo, 1, HU3D_MOTATTR_LOOP);
    mbObjMotionTimeSet(playerObjId, motTime);
    mbPlayerRotSetV(work->playerNo, &rot);
    Hu3DMotionCalc(playerMdlId);
    HuPrcVSleep();
    mbMasuPosGet(work->masuId, &work->masuPos);
    work->masuPos.y += 20.0f;
    VECSubtract(&work->masuPos, &work->pos, &dir);
    dist = VECMag(&dir);
    work->height = 50.0f+(0.1f*dist);
    if(work->height > 700.0f) {
        work->height = 700.0f;
    }
    startPos = work->pos;
    startPos2 = startPos;
    posX[0] = work->pos.x;
    posY[0] = work->pos.y;
    posZ[0] = work->pos.z;
    posX[1] = work->pos.x;
    posY[1] = work->pos.y;
    posZ[1] = work->pos.z;
    posX[2] = work->masuPos.x;
    posY[2] = work->masuPos.y;
    posZ[2] = work->masuPos.z;
    CapEffThrowCreate(work->playerNo, posX, posY, posZ, work->height, work->masuId);
    if(!GwPlayer[work->playerNo].comF) {
        mbWipeDissolveFadeInTime(15);
    }
    maxTime = 45;
    mbObjPosGet(work->objId, &objPos);
    Hu3DMotionCalc(mbObjModelIDGet(mbPlayerObjIDGet(work->playerNo)));
    Hu3DModelObjMtxGet(mbObjModelIDGet(mbPlayerObjIDGet(work->playerNo)), CapHookNameGet(work->playerNo), mtx);
    handPos.x = mtx[0][3];
    handPos.y = mtx[1][3];
    handPos.z = mtx[2][3];
    mbAudFXPlay(0x410);
    for(time=0.0f; time<maxTime; time+=1.0f) {
        Hu3DMotionCalc(mbObjModelIDGet(mbPlayerObjIDGet(work->playerNo)));
        Hu3DModelObjMtxGet(mbObjModelIDGet(mbPlayerObjIDGet(work->playerNo)), CapHookNameGet(work->playerNo), mtx);
        handPos.x = mtx[0][3];
        handPos.y = mtx[1][3];
        handPos.z = mtx[2][3];
        rate = HuSin(90.0f*(time/maxTime));
        ofs = 100.0*HuSin(180.0f*rate);
        pos.x = objPos.x+(rate*(handPos.x-objPos.x))+(ofs*HuSin(180.0f-(270.0f*rate)));
        pos.y = objPos.y+(rate*(handPos.y-objPos.y));
        pos.z = objPos.z+(rate*(handPos.z-objPos.z))+(ofs*HuCos(180.0f-(270.0f*rate)));
        scale = 0.35f+(0.85f*HuCos(90.0f*rate));
        mbObjPosSetV(work->objId, &pos);
        mbObjScaleSet(work->objId, scale, scale, scale);
        HuPrcVSleep();
    }
    mbObjHookSet(mbPlayerObjIDGet(work->playerNo), CapHookNameGet(work->playerNo), work->objId);
    mbPlayerRotateStart(work->playerNo, HuAtan(dir.x, dir.z), 15);
    while(!mbPlayerRotateCheck(work->playerNo)) {
        HuPrcVSleep();
    }
    do {
        HuPrcVSleep();
    } while(!CapEffThrowCheck(&pos, &work->maxTime));
    CapEffThrowKill();
    posX[1] = pos.x;
    posY[1] = pos.y;
    posZ[1] = pos.z;
    work->time = 0;
    mbPlayerMotionShiftSet(work->playerNo, work->motId, 0.0f, 8.0f, HU3D_MOTATTR_NONE);
    mbAudFXPlay(0x405);
    while(mbObjMotionShiftIDGet(mbPlayerObjIDGet(work->playerNo)) != -1) {
        HuPrcVSleep();
    }
    time = mbPlayerMotionMaxTimeGet(work->playerNo);
    while(mbPlayerMotionTimeGet(work->playerNo) <= 0.35f*time) {
        HuPrcVSleep();
    }
    Hu3DMotionCalc(mbObjModelIDGet(mbPlayerObjIDGet(work->playerNo)));
    Hu3DModelObjMtxGet(mbObjModelIDGet(mbPlayerObjIDGet(work->playerNo)), CapHookNameGet(work->playerNo), mtx);
    posX[0] = work->pos.x = mtx[0][3];
    posY[0] = work->pos.y = mtx[1][3];
    posZ[0] = work->pos.z = mtx[2][3];
    if(work->objId != -1) {
        mbObjHookObjReset(mbPlayerObjIDGet(work->playerNo), CapHookNameGet(work->playerNo));
        mbObjDispSet(work->objId, FALSE);
    }
    capsuleObjId = -1;
    work->colorObjId = mbCapObjColorCreate(work->capsuleNo, 0);
    mbPlayerPosGet(work->playerNo, &pos);
    mbCapObjColorPosSet(work->colorObjId, pos.x, 100.0f+pos.y, pos.z);
    mbCapObjColorScaleSet(work->colorObjId, 1.0f, 1.0f, 1.0f);
    mbCapObjColorLayerSet(work->colorObjId, 4);
    CapEffTrailAdd(&work->pos, work->capsuleNo);
    seNo = mbAudFXPlay(0x406);
    do {
        work->time++;
        rate = (float)work->time/work->maxTime;
        VECSubtract(&work->masuPos, &work->pos, &dir);
        VECScale(&dir, &dir, rate);
        VECAdd(&work->pos, &dir, &camPos);
        linePos = camPos;
        CapThrowCameraCalc(rate, posX, posY, posZ, &out, 3);
        camPos.x = out.x;
        camPos.y = out.y;
        camPos.z = out.z;
        VECSubtract(&camPos, &linePos, &pos);
        VECScale(&pos, &pos, 0.75f);
        VECAdd(&linePos, &pos, &linePos);
        mbCapObjColorPosSet(work->colorObjId, camPos.x, camPos.y, camPos.z);
        mbCapObjColorScaleSet(work->colorObjId, 0.5f, 0.5f, 0.5f);
        VECSubtract(&work->masuPos, &work->pos, &dir);
        dirOld = dir;
        dir.x = dirOld.z;
        dir.y = 0.0f;
        dir.z = -dirOld.x;
        if((mag = VECMag(&dir)) <= 0.0f) {
            dir.z = 1.0f;
        }
        PSMTXRotAxisRad(rotMtx, &dir, 0.1f);
        mbCapObjColorMtxGet(work->colorObjId, objMtx);
        PSMTXConcat(rotMtx, objMtx, objMtx);
        mbCapObjColorMtxSet(work->colorObjId, objMtx);
        CapEffTrailPosSet(&camPos);
        mbObjPosSet(work->lineObjId, linePos.x, linePos.y, linePos.z);
        mbObjDispSet(work->lineObjId, FALSE);
        mbCameraFocusObjSet(work->lineObjId);
        if(capsuleThrowGlowOMObj != NULL) {
            pos.x = camPos.x+(0.75f*(100.0f*(0.5f-CAP_RAND_F())));
            pos.y = camPos.y+(0.75f*(100.0f*(0.5f-CAP_RAND_F())));
            pos.z = camPos.z+(0.75f*(100.0f*(0.5f-CAP_RAND_F())));
            vel.x = vel.y = vel.z = 0.0f;
            color = capsulePlayerThrowColorTbl[mbCapColorGet(work->capsuleNo)];
            color.a = 192.0f+(63.0f*CAP_RAND_F());
            mbev_CapEffGlowAdd(capsuleThrowGlowOMObj, pos, vel, 60.0f*(1.0f+CAP_RAND_F()), 100.0f*(0.2f+(0.025f*CAP_RAND_F())), 0.0f, 0.025f, color);
        }
        HuPrcVSleep();
    } while(work->time < work->maxTime);
    if(seNo != -1) {
        mbAudFXStop(seNo);
    }
    seNo = -1;
    mbAudFXPlay(0x407);
    camPos = work->masuPos;
    camPos.y -= 10000.0f;
    CapEffTrailPosSet(&camPos);
    mbObjDispSet(work->objId, FALSE);
    mbObjDispSet(work->lineObjId, FALSE);
    mbCapObjColorDispSet(work->colorObjId, FALSE);
    omVibrate(work->playerNo, 20, 4, 4);
    coinNum = CapEffThrowMasu(work->masuId, work->capsuleNo, work->playerNo, TRUE);
    CapEffThrowMasuWait(coinNum);
    if(_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        mbTutorialCall(18);
    }
    CapThrowEndWin(work->masuId, work->capsuleNo);
    mbev_CapPlayerMotShiftWait(work->playerNo, 1, HU3D_MOTATTR_LOOP, TRUE);
    mbPlayerRotateStart(work->playerNo, 0, 15);
    while(!mbPlayerRotateCheck(work->playerNo)) {
        HuPrcVSleep();
    }
    mbCameraFocusPlayerSet(work->playerNo);
    mbCameraMoveOnSet(FALSE);
    if(capsuleThrowHook) {
        capsuleThrowHook(FALSE);
    }
    mbCameraMoveWait();
    mbCameraMoveOnSet(TRUE);
    mbev_CapBonusCoinCall(work->playerNo, work->capsuleNo, coinNum, TRUE);
    if(!capsuleUseRemoveOnF) {
        mbPlayerCapsuleUseSet(work->capsuleNo);
        capsuleIdx = mbPlayerCapsuleFind(work->playerNo, work->capsuleNo);
        if(capsuleIdx != -1) {
            mbPlayerCapsuleRemove(work->playerNo, capsuleIdx);
        }
    }
    if(work->motId != -1 && !mbExitCheck()) {
        mbPlayerMotionKill(work->playerNo, work->motId);
    }
    work->motId = -1;
    HuPrcEnd();
}

static void CapPlayerThrowKill(void)
{
    CAPPLAYERTHROWWORK *work = HuPrcCurrentGet()->property;
    CapEffThrowKill();
    mbCapObjKill(work->objId);
    mbCapObjKill(work->lineObjId);
    mbCapObjColorKill(work->colorObjId);
    HuMemDirectFree(work);
    capsulePlayerThrowProc = NULL;
}

static void CapEffThrowCreate(int playerNo, float *posX, float *posY, float *posZ, float height, int masuId)
{
    CAPEFFTHROWWORK *work;
    HU3D_MODEL *modelP;
    int boardNo;
    capEffThrowMdlId = Hu3DHookFuncCreate(CapEffThrowHook);
    Hu3DModelCameraSet(capEffThrowMdlId, HU3D_CAM0);
    Hu3DModelLayerSet(capEffThrowMdlId, 7);
    modelP = &Hu3DData[capEffThrowMdlId];
    work = HuMemDirectMallocNum(HEAP_MODEL, sizeof(CAPEFFTHROWWORK), modelP->mallocNo);
    modelP->hookData = work;
    memset(work, 0, sizeof(CAPEFFTHROWWORK));
    work->mdlId = capEffThrowMdlId;
    work->masuId = -2;
    work->maxTime = 0;
    work->endF = FALSE;
    work->unk10 = 0;
    work->frameNo = work->unk10;
    work->time = 0;
    work->start.x = posX[0];
    work->start.y = posY[0];
    work->start.z = posZ[0];
    work->end.x = posX[2];
    work->end.y = posY[2];
    work->end.z = posZ[2];
    work->pos.x = work->pos.y = work->pos.z = 0.0f;
    work->height = height;
    work->tick = 0;
    boardNo = mbBoardNoGet();
    if(boardNo < 0) {
        boardNo = 0;
    } else if(boardNo >= 11) {
        boardNo = 10;
    }
    work->delay = capsulePlayerThrowDelayTbl[boardNo];
    work->posX[0] = posX[0];
    work->posY[0] = posY[0];
    work->posZ[0] = posZ[0];
    work->posX[1] = posX[1];
    work->posY[1] = posY[1];
    work->posZ[1] = posZ[1];
    work->posX[2] = posX[2];
    work->posY[2] = posY[2];
    work->posZ[2] = posZ[2];
}

static void CapEffThrowHook(HU3D_MODEL *modelP, Mtx *mtx)
{
    CAPEFFTHROWWORK *work;
    int i;
    u32 tick;
    float dist;
    float rate;
    float angle;
    HuVecF pos;
    HuVecF dir;
    HuVecF out;
    work = modelP->hookData;
    if(work->endF) {
        return;
    }
    if(work->time == 0) {
        VECSubtract(&work->end, &work->start, &pos);
        VECScale(&pos, &pos, capsulePlayerThrowFrameTbl[work->frameNo].rate);
        VECAdd(&pos, &work->start, &pos);
        if(pos.y < work->start.y) {
            pos.y = work->start.y;
        } else if(pos.y < work->end.y) {
            pos.y = work->end.y;
        }
        pos.y += work->height+capsulePlayerThrowFrameTbl[work->frameNo].y;
        if(0.0f != capsulePlayerThrowFrameTbl[work->frameNo].side) {
            VECSubtract(&work->end, &work->start, &dir);
            if(VECMag(&dir) > 0.0f) {
                VECNormalize(&dir, &dir);
            }
            angle = 90.0+HuAtan(dir.x, dir.z);
            dir.x = capsulePlayerThrowFrameTbl[work->frameNo].side*HuSin(angle);
            dir.z = capsulePlayerThrowFrameTbl[work->frameNo].side*HuCos(angle);
            pos.x += dir.x;
            pos.z += dir.z;
        } else if(0.0f != capsulePlayerThrowFrameTbl[work->frameNo].dir) {
            VECSubtract(&work->end, &work->start, &dir);
            dist = VECMag(&dir);
            if(VECMag(&dir) > 0.0f) {
                VECNormalize(&dir, &dir);
            }
            rate = dir.z;
            dir.z = (dir.x*dist)*capsulePlayerThrowFrameTbl[work->frameNo].dir;
            dir.x = (rate*dist)*capsulePlayerThrowFrameTbl[work->frameNo].dir;
            pos.x += dir.x;
            pos.z += dir.z;
        }
        work->posX[1] = pos.x;
        work->posY[1] = pos.y;
        work->posZ[1] = pos.z;
        CapThrowCameraSet(work->posX, work->posY, work->posZ, 3);
        VECSubtract(&pos, &work->start, &dir);
        dist = VECMag(&dir);
        VECSubtract(&work->end, &pos, &dir);
        dist += VECMag(&dir);
        work->maxTime = dist/25.0f;
        if(work->maxTime < 24.0f) {
            work->maxTime = 24;
        }
        work->pos = work->start;
        work->time = 1;
    }
    work->tick = OSGetTick();
    for(i=0; work->time < 0.5f*work->maxTime; i++) {
        rate = work->time/(0.5f*work->maxTime);
        CapThrowCameraCalc(rate, work->posX, work->posY, work->posZ, &out, 3);
        pos.x = out.x;
        pos.y = out.y;
        pos.z = out.z;
        if(CapColCheck(&work->pos, &pos, &dir)) {
            break;
        }
        if(work->masuId != -2 && CapColExec(work->masuId, &work->pos, &pos, &dir)) {
            break;
        }
        work->pos = pos;
        work->time++;
        tick = OSGetTick();
        if(tick-work->tick > work->delay) {
            return;
        }
    }
    if(work->time >= 0.5f*work->maxTime) {
        work->endF = TRUE;
        return;
    }
    work->time = 0;
    work->frameNo++;
    if(work->frameNo < 48) {
        return;
    }
    if(work->masuId != -2) {
        work->time = 0;
        work->frameNo = 0;
        work->masuId = -2;
        return;
    }
    work->frameNo = 0;
    VECSubtract(&work->end, &work->start, &pos);
    VECScale(&pos, &pos, capsulePlayerThrowFrameTbl[work->frameNo].rate);
    VECAdd(&pos, &work->start, &pos);
    if(pos.y < work->start.y) {
        pos.y = work->start.y;
    } else if(pos.y < work->end.y) {
        pos.y = work->end.y;
    }
    pos.y += work->height+capsulePlayerThrowFrameTbl[work->frameNo].y;
    work->posX[1] = pos.x;
    work->posY[1] = pos.y;
    work->posZ[1] = pos.z;
    CapThrowCameraSet(work->posX, work->posY, work->posZ, 3);
    VECSubtract(&pos, &work->start, &dir);
    dist = VECMag(&dir);
    VECSubtract(&work->end, &pos, &dir);
    dist += VECMag(&dir);
    work->maxTime = dist/25.0f;
    if(work->maxTime < 24.0f) {
        work->maxTime = 24;
    }
    work->endF = TRUE;
}

static int CapEffThrowCheck(HuVecF *pos, int *maxTime)
{
    HU3D_MODEL *modelP;
    CAPEFFTHROWWORK *work;
    if(capEffThrowMdlId == -1) {
        return FALSE;
    }
    modelP = &Hu3DData[capEffThrowMdlId];
    work = modelP->hookData;
    if(!work->endF) {
        return FALSE;
    }
    pos->x = work->posX[1];
    pos->y = work->posY[1];
    pos->z = work->posZ[1];
    *maxTime = work->maxTime;
    return TRUE;
}

static void CapEffThrowKill(void)
{
    if(capEffThrowMdlId != -1) {
        Hu3DModelKill(capEffThrowMdlId);
    }
    capEffThrowMdlId = -1;
}

static void CapEffThrowMasuCreate(int masuId, int capsuleNo)
{
    HuVecF pos;
    HuVecF rot;
    if(masuId > 0) {
        mbMasuPosGet(masuId, &pos);
        mbMasuRotGet(masuId, &rot);
    }
    capsuleThrowGlowOMObj = mbev_CapEffGlowCreate();
    capsuleThrowRingOMObj = mbev_CapEffRingCreate();
    capsuleThrowRayOMObj = mbev_CapEffRayCreate(0.0f, 0.02f);
    if(masuId > 0) {
        mbev_CapEffRayTransformSet(capsuleThrowRayOMObj, &pos, &rot, NULL);
    }
    capsuleThrowMasuHitOMObj = mbev_CapEffMasuHitCreate();
    if(masuId > 0) {
        mbev_CapEffMasuHitTransformSet(capsuleThrowMasuHitOMObj, &pos, &rot, NULL);
    }
    capsuleThrowMasuCoinOMObj = mbev_CapEffCoinCreate();
    CapEffTrailCreate(capsuleNo);
    CapEffCrackCreate();
}

static int CapEffThrowMasu(int masuId, int capsuleNo, int playerNo, BOOL coinF)
{
    int i;
    int j;
    int coinLeft;
    int coinNum;
    int coinWait;
    float alpha;
    HuVecF pos;
    HuVecF rot;
    HuVecF rotV;
    HuVecF rotEnd;
    HuVecF ofs;
    HuVecF scale;
    GXColor color;
    int unused;
    unused = 0;
    coinNum = 0;
    if(GWPartyGet() == FALSE || _CheckFlag(FLAG_BOARD_TUTORIAL) || !coinF) {
        coinNum = -1;
    } else {
        coinNum = mbCapBonusCoinNumGet(playerNo, capsuleNo);
    }
    coinLeft = coinNum;
    coinWait = 30;
    ofs.x = ofs.y = ofs.z = 0.0f;
    mbMasuPosGet(masuId, &pos);
    mbMasuRotGet(masuId, &rot);
    CapEffCrackAdd(&pos, &rot);
    mbMasuPosGet(masuId, &pos);
    rot.x = -90.0f;
    rot.y = 0.0f;
    rot.z = 0.0f;
    scale.x = 2.0f;
    scale.y = 4.0f;
    scale.z = 100.0f*(1.0f+(0.25f*CAP_RAND_F()));
    mbev_CapEffColorSet(&color, mbRandMod(0x8000));
    mbev_CapEffRingAdd(capsuleThrowRingOMObj, pos, rot, scale, 3, 15, 1, color);
    for(i=0; i<=60.0f || coinLeft > 0; i++) {
        if(i > 60.0f) {
            i = 60;
        }
        if(i == 20) {
            mbCapMasuCapsuleSet(masuId, capsuleNo, playerNo);
            mbAudFXPlay(0x408);
        }
        if(i == 10 || i == 20) {
            mbMasuPosGet(masuId, &pos);
            rot.x = -90.0f;
            rot.y = 0.0f;
            rot.z = 0.0f;
            scale.x = 2.0f;
            scale.y = 6.0f;
            scale.z = 100.0f*(1.0f+(0.25f*CapEffRandF()));
            mbev_CapEffColorSet(&color, CapEffRandGet());
            mbev_CapEffRingAdd(capsuleThrowRingOMObj, pos, rot, scale, 3, 10, 2, color);
        }
        if(--coinWait <= 0 && coinLeft > 0) {
            mbMasuPosGet(masuId, &pos);
            mbev_CapEffCoinMultiAdd(capsuleThrowMasuCoinOMObj, &pos, 1);
            coinLeft--;
            coinWait = 3.0f*CapEffRandF();
        }
        if(masuId > 0) {
            mbMasuPosGet(masuId, &pos);
            mbMasuRotGet(masuId, &rot);
            mbev_CapEffRayTransformSet(capsuleThrowRayOMObj, &pos, &rot, NULL);
            mbev_CapEffMasuHitTransformSet(capsuleThrowMasuHitOMObj, &pos, &rot, NULL);
        }
        alpha = HuCos(90.0f*(i/60.0f));
        for(j=0; j<2.0f*alpha; j++) {
            pos = ofs;
            rotV.x = 180.0f*CapEffRandSF();
            rotV.y = 360.0f*CapEffRandSF();
            rotV.z = 180.0f*CapEffRandSF();
            rotEnd.x = rotV.x+(45.0f*CapEffRandSF());
            rotEnd.y = rotV.y+(45.0f*CapEffRandSF());
            rotEnd.z = rotV.z+(45.0f*CapEffRandSF());
            mbev_CapEffRayAdd(capsuleThrowRayOMObj, &pos, &rotV, &rotEnd, 200.0f*(1.0f+CapEffRandF()), 20.0f+(5.0f*CapEffRandF()));
        }
        mbev_CapEffRayAlphaSet(capsuleThrowRayOMObj, alpha);
        for(j=0; j<5.0f*alpha; j++) {
            pos = ofs;
            rotV.x = 360.0f*CapEffRandSF();
            rotV.y = 360.0f*CapEffRandSF();
            rotV.z = 360.0f*CapEffRandSF();
            rotEnd.x = 360.0f*CapEffRandSF();
            rotEnd.y = 360.0f*CapEffRandSF();
            rotEnd.z = 360.0f*CapEffRandSF();
            mbev_CapEffMasuHitAdd(capsuleThrowMasuHitOMObj, &pos, &rotV, &rotEnd, (100.0f*alpha)*(1.5f+CapEffRandF()), (25.0f*alpha)*(1.0f+(0.5f*CapEffRandF())), 20.0f+(10.0f*CapEffRandF()));
        }
        HuPrcVSleep();
    }
    return coinNum;
}

static int CapEffThrowMasuWait(int coinNum)
{
    HuPrcSleep(18);
    while(mbev_CapEffCoinNumGet(capsuleThrowMasuCoinOMObj) > 0) {
        HuPrcVSleep();
    }
    if(coinNum <= 0) {
        while(mbev_CapEffGlowDispGet(capsuleThrowGlowOMObj)) {
            HuPrcVSleep();
        }
    }
    if(capsuleThrowGlowOMObj != NULL) {
        mbev_CapEffGlowKill(capsuleThrowGlowOMObj);
    }
    if(capsuleThrowRingOMObj != NULL) {
        mbev_CapEffRingKill(capsuleThrowRingOMObj);
    }
    if(capsuleThrowRayOMObj != NULL) {
        mbev_CapEffRayKill(capsuleThrowRayOMObj);
    }
    if(capsuleThrowMasuHitOMObj != NULL) {
        mbev_CapEffMasuHitKill(capsuleThrowMasuHitOMObj);
    }
    if(capsuleThrowMasuCoinOMObj != NULL) {
        mbev_CapEffCoinKill(capsuleThrowMasuCoinOMObj);
    }
    CapEffCrackKill();
    CapEffTrailKill();
    return TRUE;
}

void mbCapThrowHookSet(CAPTHROWHOOK hook)
{
    capsuleThrowHook = hook;
}

void mbCapEffThrowCreate(HuVecF *start, HuVecF *end)
{
    HuVecF dir;
    float posX[3];
    float posY[3];
    float posZ[3];
    float dist;
    float height;
    posX[0] = start->x;
    posX[1] = start->x;
    posX[2] = end->x;
    posY[0] = start->y;
    posY[1] = start->y;
    posY[2] = end->y;
    posZ[0] = start->z;
    posZ[1] = start->z;
    posZ[2] = end->z;
    VECSubtract(end, start, &dir);
    dist = VECMag(&dir);
    height = 50.0f+(0.1f*dist);
    if(height > 700.0f) {
        height = 700.0f;
    }
    CapEffThrowCreate(-1, posX, posY, posZ, height, -1);
}

int mbCapEffThrowCheck(HuVecF *pos, int *maxTime)
{
    int result;
    result = CapEffThrowCheck(pos, maxTime);
    if(result != FALSE) {
        CapEffThrowKill();
    }
    return result;
}

void mbCapAutoThrow(HuVecF *start, HuVecF *mid, HuVecF *end, int playerNo, int masuId, int capsuleNo, int maxTime, float speed)
{
    CAPAUTOTHROWWORK *work = CapMalloc(sizeof(CAPAUTOTHROWWORK));
    memset(work, 0, sizeof(CAPAUTOTHROWWORK));
    work->playerNo = playerNo;
    work->capsuleNo = capsuleNo;
    work->masuId = masuId;
    work->maxTime = maxTime;
    work->speed = speed;
    work->pos[0] = *start;
    work->pos[1] = *mid;
    work->pos[2] = *end;
    CapAutoThrow(work);
    mbCapAutoThrowEnd(work);
    HuMemDirectFree(work);
}

static void CapAutoThrow(CAPAUTOTHROWWORK *work)
{
    int objId;
    int time;
    int seNo;
    float rate;
    HuVecF pos;
    HuVecF glowPos;
    HuVecF vel;
    float posX[3];
    float posY[3];
    float posZ[3];
    HuVecF out;
    GXColor color;
    if(capsuleThrowHook) {
        capsuleThrowHook(TRUE);
    }
    CapEffThrowMasuCreate(work->masuId, work->capsuleNo);
    posX[0] = work->pos[0].x;
    posX[1] = work->pos[1].x;
    posX[2] = work->pos[2].x;
    posY[0] = work->pos[0].y;
    posY[1] = work->pos[1].y;
    posY[2] = work->pos[2].y;
    posZ[0] = work->pos[0].z;
    posZ[1] = work->pos[1].z;
    posZ[2] = work->pos[2].z;
    objId = mbCapObjColorCreate(work->capsuleNo, 0);
    mbCapObjColorPosSetV(objId, &work->pos[0]);
    mbCapObjColorScaleSet(objId, 1.0f, 1.0f, 1.0f);
    mbCapObjColorLayerSet(objId, 4);
    if(work->speed < 1.0f) {
        mbCameraMoveMasu(work->masuId, NULL, NULL, 1600.0f, -1.0f, -1);
        mbCameraMoveWait();
        mbWipeSpecialFadeOutCreate(1, 60);
    }
    if(work->speed < 1.0f) {
        CapEffTrailAdd(&work->pos[0], work->capsuleNo);
    }
    if(work->speed < 1.0f) {
        seNo = mbAudFXPlay(0x406);
    } else {
        seNo = -1;
    }
    time = work->speed*work->maxTime;
    do {
        if(work->speed >= 1.0f) {
            break;
        }
        time++;
        rate = (float)time/work->maxTime;
        CapThrowCameraCalc(rate, posX, posY, posZ, &out, 3);
        pos.x = out.x;
        pos.y = out.y;
        pos.z = out.z;
        mbCapObjColorPosSet(objId, pos.x, pos.y, pos.z);
        mbCapObjColorScaleSet(objId, 0.5f, 0.5f, 0.5f);
        if(work->speed < 1.0f) {
            CapEffTrailPosSet(&pos);
        }
        if(capsuleThrowGlowOMObj != NULL) {
            glowPos.x = pos.x+(0.75f*(100.0f*(0.5f-CAP_RAND_F())));
            glowPos.y = pos.y+(0.75f*(100.0f*(0.5f-CAP_RAND_F())));
            glowPos.z = pos.z+(0.75f*(100.0f*(0.5f-CAP_RAND_F())));
            vel.x = vel.y = vel.z = 0.0f;
            color = capsuleAutoThrowColorTbl[mbCapColorGet(work->capsuleNo)];
            color.a = 192.0f+(63.0f*CAP_RAND_F());
            mbev_CapEffGlowAdd(capsuleThrowGlowOMObj, glowPos, vel, 60.0f*(1.0f+CAP_RAND_F()), 100.0f*(0.2f+(0.025f*CAP_RAND_F())), 0.0f, 0.025f, color);
        }
        HuPrcVSleep();
    } while(time < work->maxTime);
    if(seNo != -1) {
        mbAudFXStop(seNo);
    }
    seNo = -1;
    mbAudFXPlay(0x407);
    pos = work->pos[1];
    pos.y -= 10000.0f;
    CapEffTrailPosSet(&pos);
    mbCapObjColorKill(objId);
    if(work->speed < 1.0f) {
        omVibrate(work->playerNo, 20, 7, 3);
    }
    CapEffThrowMasu(work->masuId, work->capsuleNo, work->playerNo, FALSE);
    CapEffThrowMasuWait(0);
    if(work->masuId > 0) {
        CapThrowEndWin(work->masuId, work->capsuleNo);
    }
    if(work->speed < 1.0f) {
        mbWipeSpecialFadeInCreate(1, 1);
    }
    if(capsuleThrowHook) {
        capsuleThrowHook(FALSE);
    }
}

void mbCapAutoThrowEnd(void *work)
{
}

int mbCapSelectMasu(int playerNo, int capsuleNo)
{
    CAPSELMASU_WORK *work;
    capsuleNo = (u8)capsuleNo;
    switch(mbCapUseModeGet(capsuleNo)) {
        case 0:
            capsuleMasuSelectResult = -3;
            return capsuleMasuSelectResult;
        case 3:
        case 4:
            capsuleMasuSelectResult = -2;
            return capsuleMasuSelectResult;
    }
    capsuleMasuSelectResult = -1;
    capsuleMasuSelectComF[playerNo] = FALSE;
    work = CapMalloc(sizeof(CAPSELMASU_WORK));
    work->playerNo = playerNo;
    work->unk4 = -1;
    work->capsuleNo = capsuleNo;
    if(!GwPlayer[playerNo].comF) {
        CapSelectMasuPlayer(work);
    } else {
        CapSelectMasuCom(work);
    }
    HuMemDirectFree(work);
    return capsuleMasuSelectResult;
}

void mbCapSelectMasuInit(void)
{
    int max;
    int masuId;
    int i;
    int num;
    s16 *list;
    max = 0;
    list = CapMalloc(256*sizeof(s16));
    for(masuId=1; masuId<=mbMasuRawNumGet(); masuId++) {
        memset(list, 0, 256*sizeof(s16));
        capsuleMasuSelectEndF = TRUE;
        CapSelectMasuListGet(list, masuId, 5, 5);
        for(i=0, num=1; i<256; i++) {
            if(list[i] & 1) {
                num++;
            }
        }
        if(num > max) {
            max = num;
        }
    }
    HuMemDirectFree(list);
}

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
static int CapEffHiliteAdd(HuVecF pos, HuVecF rot, HuVecF scale, int arg3, int arg4, int arg5, int arg6, GXColor color);
static float CapCameraXZAngleGet(float angle);
static float CapAngleSumWrap(float a, float b);

static void CapSelectMasuKill(CAPSELMASU_WORK *work);
static int CapSelectMasuCheck(int masuId);
static int CapSelectMasuDispCheck(int masuId);
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
    int okF;
    int winType;
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
    list = CapMalloc(0x200);
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
    int unk22;
    int prevWinType;
    int winType;
    int firstF;
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
    s16 dir;

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
    list = CapMalloc(0x200);
    memset(list, 0, 0x200);
    candList = CapMalloc(0x200);
    memset(candList, 0, 0x200);
    pathList = CapMalloc(0x200);
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
            candList[num] = i;
            num++;
        }
    }
    if(num <= 0) {
        for(i = 1, num = 0; i < 256; i++) {
            if(i != masuId && (list[i] & 1)) {
                candList[num] = i;
            num++;
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

    capsuleNo = mbCapValueTypeGet(capsuleNo);
    work = CapMalloc(sizeof(CAPSELMASU_WORK));
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
    CAPLISTFILE *file;
    int i;
    int num;
    CAPSULE_LIST *buf;
    CAPSULE_LIST *data;

    for(file = capsuleListFileTbl, i = 0; file->boardNo >= 0; i++, file++) {
        if(file->boardNo == mbBoardNoGet()) {
            break;
        }
    }
    data = HuDataSelHeapReadNum(file->file, HU_MEMNUM_OVL, HEAP_MODEL);
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
    CAPLISTDEFINE *def;
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
    s16 sum;

    def = capsuleListDefineTbl;
    list = CapMalloc(0x210);
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

static int capsuleNumColW[16] = { 108, 32, 32, 48, 32, 32, 32, 32, 32, 32, 32, 48, 32, 32, 32, 48 };

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
            if(capsuleList[i].rate[type] <= 0) {
                break;
            }
            cost = capsuleList[i].rate[type];
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
            if(capsuleList[i].rate[type] <= 0) {
                break;
            }
            cost = capsuleList[i].rate[type];
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
    return capsuleData[capsuleNo].listF;
}

BOOL mbCapThrowMasuCheck(int masuId)
{
    return CapSelectMasuCheck(masuId);
}

static BOOL CapCheckComPath(int playerNo, int maxStep, int mode);

int mbCapComChanceGet(int capsuleNo, int playerNo, int mode)
{
    CAPSULE_CHANCE_TBL *chance = capsuleChanceTbl;
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
    rate = chance->chance[dif].chance;
    flag = chance->chance[dif].mark[0];
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
            return chance->chance[dif].chance;
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

/* Data retail places after mbCapComChanceGet's jump table (used by parts 3 and 4). The second
   chance table shares retail's local name capsuleChanceTbl with part 1's; renamed here. */
static int capsuleThrowTbl[8] = { 10, 11, 12, 13, 25, 15, 16, 17 };
static int capsuleTrapTbl[5] = { 20, 21, 22, 23, 24 };
static int capsuleChanceTbl2[3][4][5] = {
    { { 70, 15, 15, 0, 0 }, { 70, 20, 10, 0, 0 }, { 70, 20, 0, 10, 0 }, { 50, 30, 0, 20, 0 } },
    { { 40, 30, 30, 0, 0 }, { 30, 30, 20, 20, 0 }, { 10, 40, 10, 30, 10 }, { 10, 40, 0, 30, 20 } },
    { { 30, 30, 30, 10, 0 }, { 20, 20, 20, 30, 10 }, { 10, 30, 0, 40, 20 }, { 10, 20, 0, 40, 30 } },
};
static int capsuleMaxTurnTbl[9] = { 10, 15, 20, 25, 30, 35, 40, 45, 50 };
static int capsuleTurnTbl[9][2] = { { 3, 6 }, { 5, 10 }, { 5, 15 }, { 8, 16 }, { 10, 20 }, { 10, 20 }, { 13, 26 }, { 15, 30 }, { 15, 35 } };
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
static HuVecF capsuleCrackScaleTbl[2][3] = {
    { { 0, 0, 0 }, { 1, 0, 0 }, { 0, 0, 1 } },
    { { 0, 0, 1 }, { 1, 0, 0 }, { 1, 0, 1 } }
};

static GXColor capsuleTrailColorTbl[4][2] = {
    { { 0x80, 0xFF, 0x80, 0xFF }, { 0xC0, 0xFF, 0xC0, 0xFF } },
    { { 0xFF, 0xFF, 0x80, 0xFF }, { 0xFF, 0xFF, 0xC0, 0xFF } },
    { { 0xFF, 0x80, 0x80, 0xFF }, { 0xFF, 0xC0, 0xC0, 0xFF } },
    { { 0x80, 0xFF, 0xFF, 0xFF }, { 0xC0, 0xFF, 0xFF, 0xFF } }
};

static HuVecF colScaleTbl[6*3] = {
    { -0.5f, 0.0f, 0.5f }, { -0.5f, 1.0f, 0.5f }, { 0.5f, 1.0f, 0.5f },
    { -0.5f, 0.0f, 0.5f }, { 0.5f, 1.0f, 0.5f }, { 0.5f, 0.0f, 0.5f },
    { 0.5f, 0.0f, 0.5f }, { 0.5f, 1.0f, 0.5f }, { 0.0f, 1.0f, -0.5f },
    { 0.5f, 0.0f, 0.5f }, { 0.0f, 1.0f, -0.5f }, { 0.0f, 0.0f, -0.5f },
    { 0.0f, 0.0f, -0.5f }, { 0.0f, 1.0f, -0.5f }, { -0.5f, 1.0f, 0.5f },
    { 0.0f, 0.0f, -0.5f }, { -0.5f, 1.0f, 0.5f }, { -0.5f, 0.0f, 0.5f },
};

static HuVec2f charSizeTbl[14] = {
    { 100.0f, 175.0f },
    { 100.0f, 200.0f },
    { 110.0f, 220.0f },
    { 100.0f, 175.0f },
    { 175.0f, 175.0f },
    { 110.0f, 220.0f },
    { 200.0f, 260.0f },
    { 100.0f, 150.0f },
    { 175.0f, 175.0f },
    { 150.0f, 125.0f },
    { 100.0f, 150.0f },
    { 150.0f, 125.0f },
    { 150.0f, 125.0f },
    { 150.0f, 125.0f }
};

/* Part 4 (wt-b404): CapEffHiliteOMExec .. CapColExec.
   Uses part 1's HEAD and part 3's CAPSULE_EFFECT / CAPSULE_EFFDATA / CAPEFFHILITE_WORK. */
#include "game/board/effect.h"
#include "game/main.h"
#include "game/window.h"
#include "humath.h"

#define CAPSULE_EFF_ATTR_NONE 0
#define CAPSULE_EFF_ATTR_COUNTER_RESET (1 << 0)
#define CAPSULE_EFF_ATTR_COUNTER_UPDATE (1 << 1)

#define CAPSULE_EFF_DISPATTR_NONE 0
#define CAPSULE_EFF_DISPATTR_ZBUF_OFF (1 << 0)
#define CAPSULE_EFF_DISPATTR_NOANIM (1 << 1)
#define CAPSULE_EFF_DISPATTR_CAMERA_ROT (1 << 2)
#define CAPSULE_EFF_DISPATTR_ROT3D (1 << 3)

typedef void (*CAPSULE_EFFHOOK)(HU3D_MODEL *modelP, CAPSULE_EFFECT *effP, Mtx *matrix);

typedef struct CapEffCrackData_s {
    int dispF; /* 0x00 */
    float scale; /* 0x04 */
    float rot; /* 0x08 */
    float rotSpeed; /* 0x0C */
    float fadeSpeed; /* 0x10 */
    int delay; /* 0x14 */
    HuVecF basePos; /* 0x18 */
    HuVecF baseVtx[3]; /* 0x24 */
    HuVecF vel; /* 0x48 */
    HuVecF pos; /* 0x54 */
    HuVecF vtx[3]; /* 0x60 */
    HuVec2f st[3]; /* 0x84 */
    GXColor color; /* 0x9C */
} CAPEFF_CRACK_DATA; /* size 0xA0 */

typedef struct CapEffCrackWork_s {
    int mdlId; /* 0x00 */
    int mode; /* 0x04 */
    int time; /* 0x08 */
    int num; /* 0x0C */
    int vtxNum; /* 0x10 */
    int unk14; /* 0x14 */
    HuVecF *vtx; /* 0x18 */
    HuVec2f *st; /* 0x1C */
    CAPEFF_CRACK_DATA *data; /* 0x20 */
    ANIMDATA *anim; /* 0x24 */
    GXColor color; /* 0x28 */
    u32 dlSize; /* 0x2C */
    void *dl; /* 0x30 */
} CAPEFF_CRACK_WORK; /* size 0x34 */

#define CAPEFF_TRAIL_MAX 12

typedef struct CapEffTrailSeg_s {
    HuVecF start; /* 0x00 */
    HuVecF end; /* 0x0C */
    float len; /* 0x18 */
    float ofs; /* 0x1C */
} CAPEFF_TRAIL_SEG; /* size 0x20 */

static GXColor capsuleCrackEffColor = { 0xFF, 0xFF, 0x80, 0x40 };
static GXColor capsuleCrackEffAmbColor = { 0xFF, 0xFF, 0xFF, 0xFF };
static GXColor capsuleCrackEffMatColor = { 0xFF, 0xFF, 0xFF, 0xFF };

static float capsuleTime[8];
static float capsuleBezierX[8];
static float capsuleBezierY[8];
static float capsuleBezierZ[8];


int abs(int x);

static void CapEffCrackOMExec(OMOBJ *obj);
static void CapEffCrackDraw(HU3D_MODEL *modelP, Mtx *mtx);
static void CapEffTrailOMExec(OMOBJ *obj);
static void CapEffDraw(HU3D_MODEL *modelP, Mtx *mtx);
static void CapComKeyHook(void);

static inline void *CapEffModelMalloc(u32 size, u32 num)
{
    return HuMemDirectMallocNum(HEAP_MODEL, size, num);
}

static inline float CapEffSqrtf(float x)
{
    const double _half = .5;
    const double _three = 3.0;
    volatile float y;
    if(x > 0.0f) {
        double guess = __frsqrte((double)x);
        guess = _half*guess*(_three-guess*guess*x);
        guess = _half*guess*(_three-guess*guess*x);
        guess = _half*guess*(_three-guess*guess*x);
        y = (float)(x*guess);
        return y;
    }
    return x;
}

static inline float CapEffSinDeg(float deg)
{
    return mbSinDeg(deg);
}

static inline float CapEffCosDeg(float deg)
{
    return mbCosDeg(deg);
}


static void CapEffHiliteOMExec(OMOBJ *obj)
{
    CAPEFFHILITE_WORK *work;
    HU3D_MODEL *modelP;
    CAPSULE_EFFECT *effP;
    CAPSULE_EFFDATA *effDataP;
    int i;
    int j;
    float t;

    work = obj->data;
    if(mbExitCheck() || capEffHiliteOMObj == NULL) {
        for(i=0; i<3; i++) {
            Hu3DModelKill(work->mdlId[i]);
            work->mdlId[i] = HU3D_MODELID_NONE;
        }
        for(i=0; i<3; i++) {
            HuSprAnimKill(work->anim[i]);
            work->anim[i] = NULL;
        }
        omDelObjEx(mbObjMan, obj);
        capEffHiliteOMObj = NULL;
        return;
    }
    for(i=0; i<3; i++) {
        if(work->effNum <= 0) {
            Hu3DModelAttrSet(work->mdlId[i], HU3D_ATTR_DISPOFF);
        } else {
            Hu3DModelAttrReset(work->mdlId[i], HU3D_ATTR_DISPOFF);
            modelP = &Hu3DData[work->mdlId[i]];
            effP = modelP->hookData;
            effDataP = effP->data;
            effP->unk23 = 0;
            for(j=0; j<effP->num; j++, effDataP++) {
                if(effDataP->scale <= 0.0f) {
                    continue;
                }
                switch(effDataP->time) {
                    case 0:
                        if(effDataP->baseAlpha > 0.0f) {
                            t = (++effDataP->work)/effDataP->baseAlpha;
                        } else {
                            t = 1.0f;
                        }
                        switch(effDataP->mode) {
                            case 0:
                                t = t;
                                break;
                            case 1:
                                t = HuSin(90.0f*t);
                                break;
                            case 2:
                                t = HuCos(90.0f*(1.0f-t));
                                break;
                        }
                        effDataP->scale = effDataP->vel.z*(effDataP->vel.x+(t*(1.0f-effDataP->vel.x)));
                        effDataP->color.a = effDataP->speed*t;
                        if(t >= 1.0f) {
                            effDataP->scale = effDataP->vel.z;
                            effDataP->color.a = effDataP->speed;
                            effDataP->time++;
                            effDataP->work = 0;
                        }
                        break;

                    case 1:
                        if(effDataP->tpLvl > 0.0f) {
                            t = (++effDataP->work)/effDataP->tpLvl;
                        } else {
                            t = 1.0f;
                        }
                        switch(effDataP->mode) {
                            case 0:
                                t = t;
                                break;
                            case 1:
                                t = HuSin(90.0f*t);
                                break;
                            case 2:
                                t = HuCos(90.0f*(1.0f-t));
                                break;
                        }
                        effDataP->scale = effDataP->vel.z*(1.0f+(t*(effDataP->vel.y-1.0f)));
                        effDataP->color.a = effDataP->speed*(1.0f-t);
                        if(t >= 1.0f) {
                            effDataP->scale = 0.0f;
                            work->effNum--;
                        }
                        break;
                }
            }
        }
    }
}

static void CapEffHiliteKill(void)
{
    capEffHiliteOMObj = NULL;
}

static int CapEffHiliteAdd(HuVecF posV, HuVecF rotV, HuVecF scaleV, int inTime, int outTime, int no, int mode, GXColor colorV)
{
    HuVecF *pos = &posV; HuVecF *rot = &rotV; HuVecF *scale = &scaleV; GXColor *color = &colorV;
    OMOBJ *obj;
    CAPEFFHILITE_WORK *work;
    HU3D_MODEL *modelP;
    CAPSULE_EFFECT *effP;
    CAPSULE_EFFDATA *effDataP;
    int i;
    if(capEffHiliteOMObj == NULL) {
        return -1;
    }
    obj = capEffHiliteOMObj;
    work = obj->data;
    modelP = &Hu3DData[work->mdlId[no%3]];
    effP = modelP->hookData;
    effDataP = effP->data;
    for(i=0; i<effP->num; i++, effDataP++) {
        if(effDataP->scale <= 0.0f) {
            break;
        }
    }
    if(i >= effP->num) {
        return -1;
    }
    effDataP->time = effDataP->work = 0;
    effDataP->pos.x = pos->x;
    effDataP->pos.y = pos->y;
    effDataP->pos.z = pos->z;
    effDataP->vel.x = scale->x;
    effDataP->vel.y = scale->y;
    effDataP->vel.z = scale->z;
    effDataP->baseAlpha = inTime;
    effDataP->tpLvl = outTime;
    effDataP->speed = color->a;
    effDataP->scale = scale->z;
    effDataP->color.r = color->r;
    effDataP->color.g = color->g;
    effDataP->color.b = color->b;
    effDataP->color.a = color->a;
    effDataP->rot.x = rot->x;
    effDataP->rot.y = rot->y;
    effDataP->rot.z = rot->z;
    effDataP->no = 0;
    effDataP->time = 0;
    effDataP->mode = (s16)mode;
    work->effNum++;
    return i;
}

static void CapEffCrackCreate(void)
{
    OMOBJ *obj;
    CAPEFF_CRACK_WORK *work;
    HU3D_MODEL *modelP;
    CAPEFF_CRACK_DATA *dataP;
    int i;
    int j;
    int k;
    int l;
    float stepX;
    float stepZ;
    float x;
    float z;
    float scale;
    float radius;
    float height;
    void *dlBuf;
    void *dlBegin;
    HuVecF center;
    float vtxScale[3];

    obj = capEffCrackOMObj = omAddObjEx(mbObjMan, 32768, 0, 0, -1, CapEffCrackOMExec);
    obj->data = work = CapMalloc(sizeof(CAPEFF_CRACK_WORK));
    memset(work, 0, sizeof(CAPEFF_CRACK_WORK));
    work->mdlId = Hu3DHookFuncCreate(CapEffCrackDraw);
    Hu3DModelCameraSet(work->mdlId, HU3D_CAM0);
    Hu3DModelLayerSet(work->mdlId, 3);
    modelP = &Hu3DData[work->mdlId];
    modelP->hookData = work;
    work->mode = 0;
    work->time = 0;
    work->num = 24*24*2;
    work->vtxNum = work->num*3;
    work->data = dataP = CapEffModelMalloc(work->num*sizeof(CAPEFF_CRACK_DATA), modelP->mallocNo);
    memset(dataP, 0, work->num*sizeof(CAPEFF_CRACK_DATA));
    work->vtx = CapEffModelMalloc(work->vtxNum*sizeof(HuVecF), modelP->mallocNo);
    memset(work->vtx, 0, work->vtxNum*sizeof(HuVecF));
    work->st = CapEffModelMalloc(work->vtxNum*sizeof(HuVec2f), modelP->mallocNo);
    memset(work->st, 0, work->vtxNum*sizeof(HuVec2f));
    work->anim = HuSprAnimRead(HuDataReadNum(0xC002B, HU_MEMNUM_OVL));
    work->color.r = 0;
    work->color.g = 0;
    work->color.b = 0;
    work->color.a = 0;
    stepX = 8.333334f;
    stepZ = 8.333334f;
    x = -100.0f;
    z = -100.0f;
    for(i=0; i<24; i++) {
        for(j=0; j<24; j++) {
            for(k=0; k<2; k++) {
                dataP->dispF = TRUE;
                dataP->scale = 1.0f;
                dataP->rot = 0.0f;
                if(mbRandMod(0x8000) & 1) {
                    dataP->rotSpeed = 5.0f+(2.0f*CAP_RAND_F());
                } else {
                    dataP->rotSpeed = -(5.0f+(2.0f*CAP_RAND_F()));
                }
                dataP->fadeSpeed = 0.035f+(0.035f*CAP_RAND_F());
                dataP->delay = mbRandMod(0x8000) & 0xF;
                for(l=0; l<3; l++) {
                    dataP->vtx[l].x = x+(stepX*capsuleCrackScaleTbl[k][l].x);
                    dataP->vtx[l].z = z+(stepZ*capsuleCrackScaleTbl[k][l].z);
                    dataP->vtx[l].y = 0.0f;
                    scale = CapEffSqrtf((dataP->vtx[l].x*dataP->vtx[l].x)+(dataP->vtx[l].z*dataP->vtx[l].z))/100.0f;
                    vtxScale[l] = scale;
                    if(vtxScale[l] > 1.0f) {
                        scale = 1.0f;
                        if(VECMag(&dataP->vtx[l]) > 0.0f) {
                            VECNormalize(&dataP->vtx[l], &dataP->vtx[l]);
                        }
                        VECScale(&dataP->vtx[l], &dataP->vtx[l], 100.0f);
                    }
                    radius = 100.0f;
                    height = 100.0f*scale;
                    dataP->vtx[l].y = CapEffSqrtf((radius*radius)-(height*height));
                    radius = 100.0f;
                    dataP->st[l].x = (dataP->vtx[l].x+radius)/200.0f;
                    dataP->st[l].y = (dataP->vtx[l].z+radius)/200.0f;
                }
                center = dataP->vtx[0];
                VECAdd(&center, &dataP->vtx[1], &center);
                VECAdd(&center, &dataP->vtx[2], &center);
                VECScale(&center, &dataP->pos, 0.333333f);
                radius = CapEffSqrtf((dataP->pos.x*dataP->pos.x)+(dataP->pos.z*dataP->pos.z))/100.0f;
                dataP->delay = mbRandMod(0x8000) & 0x7;
                if(VECMag(&dataP->pos) > 0.0f) {
                    VECNormalize(&dataP->pos, &dataP->vel);
                }
                VECScale(&dataP->vel, &dataP->vel, 10.0f*(0.5f+(0.5f*CAP_RAND_F())));
                dataP->basePos = dataP->pos;
                for(l=0; l<3; l++) {
                    VECSubtract(&dataP->vtx[l], &dataP->pos, &dataP->vtx[l]);
                    dataP->baseVtx[l] = dataP->vtx[l];
                }
                dataP->color = capsuleCrackEffColor;
                if(vtxScale[0] > 1.0f && vtxScale[1] > 1.0f && vtxScale[2] > 1.0f) {
                    dataP->dispF = FALSE;
                    dataP->scale = 0.0f;
                }
                dataP++;
            }
            x += stepX;
        }
        x = -100.0f;
        z += stepZ;
    }
    dlBegin = dlBuf = CapEffModelMalloc(0x10000, modelP->mallocNo);
    DCFlushRange(dlBuf, 0x10000);
    GXBeginDisplayList(dlBegin, 0x10000);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, work->vtxNum);
    for(i=0; i<work->vtxNum/3; i++) {
        GXPosition1x16(i*3);
        GXColor1x16(i);
        GXTexCoord1x16(i*3);

        GXPosition1x16((i*3)+1);
        GXColor1x16(i);
        GXTexCoord1x16((i*3)+1);

        GXPosition1x16((i*3)+2);
        GXColor1x16(i);
        GXTexCoord1x16((i*3)+2);
    }
    GXEnd();
    work->dlSize = GXEndDisplayList();
    work->dlSize > 0x10000;
    work->dl = CapEffModelMalloc(work->dlSize, modelP->mallocNo);
    memcpy(work->dl, dlBuf, work->dlSize);
    DCFlushRange(work->dl, work->dlSize);
    HuMemDirectFree(dlBuf);
}

static void CapEffCrackOMExec(OMOBJ *obj)
{
    CAPEFF_CRACK_WORK *work;
    CAPEFF_CRACK_DATA *dataP;
    int i;
    float t;
    int unused = -1;

    work = obj->data;
    if(mbExitCheck() || capEffCrackOMObj == NULL) {
        HuSprAnimKill(work->anim);
        Hu3DModelKill(work->mdlId);
        omDelObjEx(mbObjMan, obj);
        capEffCrackOMObj = NULL;
        return;
    }
    dataP = work->data;
    switch(work->mode) {
        case 0:
            break;

        case 1:
            for(i=0, dataP=work->data; i<work->num; i++, dataP++) {
                dataP->color.a = 64;
                dataP->pos.y = 0.0f;
                dataP->vtx[0].y = 0.0f;
                dataP->vtx[1].y = 0.0f;
                dataP->vtx[2].y = 0.0f;
            }
            Hu3DModelScaleSet(work->mdlId, 0.0f, 1.0f, 0.0f);
            work->time = 0;
            work->mode++;
            break;

        case 2:
            t = (++work->time)/6.0f;
            if(t >= 1.0f) {
                t = 1.0f;
            }
            Hu3DModelScaleSet(work->mdlId, t, 1.0f, t);
            if(work->time >= 6.0f) {
                work->time = 0;
                work->mode++;
            }
            break;

        case 3:
            t = (1.0f/18.0f)*(++work->time);
            if(t >= 1.0f) {
                t = 1.0f;
            }
            for(i=0, dataP=work->data; i<work->num; i++, dataP++) {
                dataP->color.a = 64.0f+(191.0f*t);
                dataP->pos.y = dataP->basePos.y*t;
                dataP->vtx[0].y = dataP->baseVtx[0].y*t;
                dataP->vtx[1].y = dataP->baseVtx[1].y*t;
                dataP->vtx[2].y = dataP->baseVtx[2].y*t;
            }
            work->color.r = work->color.g = work->color.b = 64.0f*t;
            work->color.a = 255.0f*t;
            if(work->time >= 18.0f) {
                work->time = 0;
                work->mode++;
            }
            break;

        case 4:
            if(work->color.a > 20) {
                work->color.a -= 20;
            } else {
                work->color.a = 0;
            }
            for(i=0, dataP=work->data; i<work->num; i++, dataP++) {
                if(dataP->dispF == FALSE) {
                    continue;
                }
                if(dataP->delay != 0) {
                    dataP->delay--;
                    continue;
                }
                VECAdd(&dataP->pos, &dataP->vel, &dataP->pos);
                dataP->rot += dataP->rotSpeed;
                if((dataP->scale -= dataP->fadeSpeed) <= 0.0f) {
                    dataP->dispF = FALSE;
                } else if(dataP->scale > 1.0f) {
                    dataP->color.a = 255;
                } else {
                    dataP->color.a = 255.0f*dataP->scale;
                }
            }
            break;

        case 5:
            break;
    }
}

static void CapEffCrackKill(void)
{
    capEffCrackOMObj = NULL;
}

static void CapEffCrackAdd(HuVecF *pos, HuVecF *rot)
{
    OMOBJ *obj = capEffCrackOMObj;
    CAPEFF_CRACK_WORK *work;
    if(capEffCrackOMObj != NULL) {
        work = obj->data;
        work->mode = 1;
        if(pos != NULL) {
            obj->trans.x = pos->x;
            obj->trans.y = pos->y;
            obj->trans.z = pos->z;
        } else {
            obj->trans.x = obj->trans.y = obj->trans.z = 0.0f;
        }
        if(rot != NULL) {
            obj->rot.x = rot->x;
            obj->rot.y = rot->y;
            obj->rot.z = rot->z;
        } else {
            obj->rot.x = obj->rot.y = obj->rot.z = 0.0f;
        }
        Hu3DModelPosSet(work->mdlId, obj->trans.x, obj->trans.y, obj->trans.z);
        Hu3DModelRotSet(work->mdlId, obj->rot.x, obj->rot.y, obj->rot.z);
    }
}

static void CapEffCrackDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    CAPEFF_CRACK_WORK *work;
    CAPEFF_CRACK_DATA *dataP;
    HuVecF *vtx;
    HuVec2f *st;
    int i;
    int j;
    float sinRot;
    float cosRot;
    HuVecF scaleVtx;

    work = modelP->hookData;
    dataP = work->data;
    if(work->mode == 0) {
        return;
    }
    GXLoadPosMtxImm(*mtx, GX_PNMTX0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    if(shadowModelDrawF) {
        return;
    }
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetTevColor(GX_TEVREG2, work->color);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_RASC, GX_CC_C2);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_A2);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetChanAmbColor(GX_COLOR0A0, capsuleCrackEffMatColor);
    GXSetChanMatColor(GX_COLOR0A0, capsuleCrackEffAmbColor);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    HuSprTexLoad(work->anim, 0, GX_TEXMAP0, GX_REPEAT, GX_REPEAT, GX_LINEAR);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetZCompLoc(GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
    GXSetCullMode(GX_CULL_BACK);
    vtx = work->vtx;
    st = work->st;
    for(i=0; i<work->num; i++, dataP++) {
        if(dataP->dispF != FALSE) {
            if(!dataP->rot) {
                for(j=0; j<3; j++) {
                    VECScale(&dataP->vtx[j], &scaleVtx, dataP->scale);
                    VECAdd(&scaleVtx, &dataP->pos, vtx);
                    vtx++;
                    *st = dataP->st[j];
                    st++;
                }
            } else {
                sinRot = CapEffSinDeg(dataP->rot);
                cosRot = CapEffCosDeg(dataP->rot);
                for(j=0; j<3; j++) {
                    VECScale(&dataP->vtx[j], &scaleVtx, dataP->scale);
                    vtx->x = dataP->pos.x+(dataP->vtx[j].x*cosRot)-(dataP->vtx[j].y*sinRot);
                    vtx->y = dataP->pos.y+(dataP->vtx[j].x*sinRot)+(dataP->vtx[j].y*cosRot);
                    vtx->z = dataP->pos.z+dataP->vtx[j].z;
                    vtx++;
                    *st = dataP->st[j];
                    st++;
                }
            }
        } else {
            vtx->x = vtx->y = vtx->z = 0.0f;
            vtx++;
            vtx->x = vtx->y = vtx->z = 0.0f;
            vtx++;
            vtx->x = vtx->y = vtx->z = 0.0f;
            vtx++;
            st += 3;
        }
    }
    dataP = work->data;
    DCFlushRangeNoSync(work->vtx, work->vtxNum*sizeof(HuVecF));
    DCFlushRangeNoSync(work->st, work->vtxNum*sizeof(HuVec2f));
    DCFlushRangeNoSync(work->data, work->num*sizeof(CAPEFF_CRACK_DATA));
    PPCSync();
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetArray(GX_VA_POS, work->vtx, sizeof(HuVecF));
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetArray(GX_VA_CLR0, &work->data->color, sizeof(CAPEFF_CRACK_DATA));
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetArray(GX_VA_TEX0, work->st, sizeof(HuVec2f));
    GXCallDisplayList(work->dl, work->dlSize);
}

static void CapEffTrailCreate(int capsuleNo)
{
    OMOBJ *obj;
    HuVecF *posBuf;
    HU3D_MODEL *modelP;
    MBPARTICLE *particleP;
    MBPARTICLEDATA *particleDataP;
    int i;

    capsuleNo = mbCapValueTypeGet(capsuleNo);
    obj = capEffTrailOMObj = omAddObjEx(mbObjMan, 32768, CAPEFF_TRAIL_MAX+1, 0, -1, CapEffTrailOMExec);
    posBuf = obj->data = CapMalloc(CAPEFF_TRAIL_MAX*sizeof(HuVecF));
    memset(posBuf, 0, CAPEFF_TRAIL_MAX*sizeof(HuVecF));
    for(i=0; i<CAPEFF_TRAIL_MAX; i++) {
        obj->mdlId[i] = HU3D_MODELID_NONE;
        posBuf[i].x = posBuf[i].y = posBuf[i].z = 0.0f;
    }
    obj->mdlId[CAPEFF_TRAIL_MAX] = mbParticleCreate(HuSprAnimRead(HuDataReadNum(0xC002A, HU_MEMNUM_OVL)), 24);
    Hu3DModelCameraSet(obj->mdlId[CAPEFF_TRAIL_MAX], HU3D_CAM0);
    Hu3DModelLayerSet(obj->mdlId[CAPEFF_TRAIL_MAX], 5);
    modelP = &Hu3DData[obj->mdlId[CAPEFF_TRAIL_MAX]];
    particleP = modelP->hookData;
    particleDataP = particleP->data;
    particleP->blendMode = MB_PARTICLE_BLEND_ADDCOL;
    for(i=0; i<particleP->num; i++, particleDataP++) {
        particleDataP->scale = 0.0f;
        particleDataP->pos.x = particleDataP->pos.y = particleDataP->pos.z = 0.0f;
    }
    obj->stat |= OM_STAT_MODELPAUSE;
    obj->work[0] = FALSE;
    obj->work[1] = 0;
}

static void CapEffTrailOMExec(OMOBJ *obj)
{
    HuVecF *posBuf;
    HU3D_MODEL *modelP;
    MBPARTICLE *particleP;
    MBPARTICLEDATA *particleDataP;
    CAPEFF_TRAIL_SEG *segP;
    int i;
    int j;
    int head;
    int no;
    int next;
    float total;
    float dist;
    float t;
    HuVecF dir;
    HuVecF trailPos[CAPEFF_TRAIL_MAX];
    CAPEFF_TRAIL_SEG seg[CAPEFF_TRAIL_MAX];

    posBuf = obj->data;
    if(mbExitCheck() || capEffTrailOMObj == NULL) {
        for(i=0; i<CAPEFF_TRAIL_MAX; i++) {
            if(obj->mdlId[i] != HU3D_MODELID_NONE) {
                mbCapObjColorKill(obj->mdlId[i]);
            }
            obj->mdlId[i] = HU3D_MODELID_NONE;
        }
        mbParticleKill(obj->mdlId[CAPEFF_TRAIL_MAX]);
        obj->mdlId[CAPEFF_TRAIL_MAX] = HU3D_MODELID_NONE;
        omDelObjEx(mbObjMan, obj);
        capEffTrailOMObj = NULL;
        return;
    }
    if(!obj->work[0]) {
        return;
    }
    head = obj->work[1];
    posBuf[head].x = obj->trans.x;
    posBuf[head].y = obj->trans.y;
    posBuf[head].z = obj->trans.z;
    segP = seg;
    total = 0.0f;
    for(i=0; i<CAPEFF_TRAIL_MAX-1; i++, segP++) {
        no = i+obj->work[1];
        if(no >= CAPEFF_TRAIL_MAX) {
            no -= CAPEFF_TRAIL_MAX;
        }
        next = no+1;
        if(next >= CAPEFF_TRAIL_MAX) {
            next -= CAPEFF_TRAIL_MAX;
        }
        VECSubtract(&posBuf[no], &posBuf[next], &dir);
        segP->start = posBuf[no];
        segP->end = posBuf[next];
        segP->len = VECMag(&dir);
        segP->ofs = total;
        total += segP->len;
    }
    for(i=0; i<CAPEFF_TRAIL_MAX; i++) {
        if(total <= 0.0f) {
            dist = 0.0f;
        } else if(total <= 120.0f) {
            dist = (total/12.0f)*i;
        } else {
            dist = 10.0f*i;
        }
        for(j=0, segP=seg; j<CAPEFF_TRAIL_MAX-1; j++, segP++) {
            if(dist < segP->ofs+segP->len) {
                break;
            }
        }
        if(j < CAPEFF_TRAIL_MAX-1) {
            dist -= segP->ofs;
            if(segP->len > 0.0f) {
                t = dist/segP->len;
                VECSubtract(&segP->end, &segP->start, &dir);
                VECScale(&dir, &dir, t);
                VECAdd(&segP->start, &dir, &trailPos[i]);
            } else {
                trailPos[i] = segP->start;
            }
        } else {
            trailPos[i] = seg[CAPEFF_TRAIL_MAX-2].end;
        }
    }
    for(i=0; i<CAPEFF_TRAIL_MAX; i++) {
        if(obj->mdlId[i] != HU3D_MODELID_NONE) {
            mbCapObjColorPosSetV(obj->mdlId[i], &trailPos[i]);
        }
    }
    modelP = &Hu3DData[obj->mdlId[CAPEFF_TRAIL_MAX]];
    particleP = modelP->hookData;
    particleDataP = particleP->data;
    posBuf = obj->data;
    for(i=0; i<particleP->num/2; i++, particleDataP++) {
        particleDataP->pos.x = trailPos[i].x;
        particleDataP->pos.y = trailPos[i].y;
        particleDataP->pos.z = trailPos[i].z;
    }
    for(i=0; i<particleP->num/2; i++, particleDataP++) {
        particleDataP->pos.x = trailPos[i].x;
        particleDataP->pos.y = trailPos[i].y;
        particleDataP->pos.z = trailPos[i].z;
    }
    head--;
    if(head < 0) {
        head = CAPEFF_TRAIL_MAX-1;
    }
    obj->work[1] = head;
}

static void CapEffTrailKill(void)
{
    capEffTrailOMObj = NULL;
}

static void CapEffTrailAdd(HuVecF *pos, int capsuleNo)
{
    OMOBJ *obj = capEffTrailOMObj;
    HuVecF *posBuf;
    HU3D_MODEL *modelP;
    MBPARTICLE *particleP;
    MBPARTICLEDATA *particleDataP;
    int i;
    float t;
    GXColor color;
    if(capEffTrailOMObj != NULL) {
        posBuf = obj->data;
        for(i=0; i<CAPEFF_TRAIL_MAX; i++) {
            posBuf[i] = *pos;
        }
        modelP = &Hu3DData[obj->mdlId[CAPEFF_TRAIL_MAX]];
        particleP = modelP->hookData;
        particleDataP = particleP->data;
        posBuf = obj->data;
        color = capsuleTrailColorTbl[mbCapColorGet(capsuleNo)][0];
        for(i=0; i<particleP->num/2; i++, particleDataP++) {
            t = i/12.0f;
            particleDataP->scale = 100.0f*(0.9f+(-0.49999997f*t));
            particleDataP->pos.x = pos->x;
            particleDataP->pos.y = pos->y;
            particleDataP->pos.z = pos->z;
            particleDataP->color.r = color.r;
            particleDataP->color.g = color.g;
            particleDataP->color.b = color.b;
            particleDataP->color.a = 32.0f*(1.0f-t);
        }
        color = capsuleTrailColorTbl[mbCapColorGet(capsuleNo)][1];
        for(i=0; i<particleP->num/2; i++, particleDataP++) {
            t = i/12.0f;
            particleDataP->scale = 0.75f*(100.0f*(0.9f+(-0.49999997f*t)));
            particleDataP->pos.x = pos->x;
            particleDataP->pos.y = pos->y;
            particleDataP->pos.z = pos->z;
            particleDataP->color.r = color.r;
            particleDataP->color.g = color.g;
            particleDataP->color.b = color.b;
            particleDataP->color.a = 64.0f*(1.0f-t);
        }
        obj->work[0] = TRUE;
    }
}

static void CapEffTrailPosSet(HuVecF *pos)
{
    OMOBJ *obj = capEffTrailOMObj;
    if(capEffTrailOMObj != NULL) {
        obj->trans.x = pos->x;
        obj->trans.y = pos->y;
        obj->trans.z = pos->z;
    }
}

static HU3D_MODELID CapEffCreate(ANIMDATA *anim, s16 num)
{
    CAPSULE_EFFECT *effP;
    CAPSULE_EFFDATA *effDataP;
    s16 i;
    HuVec2f *st;
    HU3D_MODEL *modelP;
    HuVecF *vtx;
    HU3D_MODELID modelId;
    void *dlBuf;
    void *dlBegin;
    modelId = Hu3DHookFuncCreate(CapEffDraw);
    Hu3DModelCameraSet(modelId, HU3D_CAM0);
    modelP = &Hu3DData[modelId];
    modelP->hookData = effP = CapEffModelMalloc(sizeof(CAPSULE_EFFECT), modelP->mallocNo);
    effP->anim = anim;
    effP->num = num;
    effP->blendMode = HU3D_PARTICLE_BLEND_NORMAL;
    effP->dispAttr = CAPSULE_EFF_DISPATTR_NONE;
    effP->hook = NULL;
    effP->hookMdlP = NULL;
    effP->count = 0;
    effP->attr = CAPSULE_EFF_ATTR_NONE;
    effP->unk23 = 0;
    effP->prevCount = 0;
    effP->mode = effP->time = 0;
    effP->data = effDataP = CapEffModelMalloc(num*sizeof(CAPSULE_EFFDATA), modelP->mallocNo);
    memset(effDataP, 0, num*sizeof(CAPSULE_EFFDATA));
    for(i=0; i<num; i++, effDataP++) {
        effDataP->scale = 0.0f;
        effDataP->rot.x = effDataP->rot.y = effDataP->rot.z = 0.0f;
        effDataP->animTime = 0.0f;
        effDataP->animSpeed = 1.0f;
        effDataP->pos.x = 0.0f;
        effDataP->pos.y = 0.0f;
        effDataP->pos.z = 0.0f;
        effDataP->color.r = effDataP->color.g = effDataP->color.b = effDataP->color.a = 255;
        effDataP->no = 0;
    }
    effP->vertex = vtx = CapEffModelMalloc(num*sizeof(HuVecF)*4, modelP->mallocNo);
    for(i=0; i<num*4; i++, vtx++) {
        vtx->x = vtx->y = vtx->z = 0.0f;
    }
    effP->st = st = CapEffModelMalloc(num*sizeof(HuVec2f)*4, modelP->mallocNo);
    for(i=0; i<num; i++) {
        st->x = 0.0f;
        st->y = 0.0f;
        st++;

        st->x = 1.0f;
        st->y = 0.0f;
        st++;

        st->x = 1.0f;
        st->y = 1.0f;
        st++;

        st->x = 0.0f;
        st->y = 1.0f;
        st++;
    }
    dlBegin = dlBuf = CapEffModelMalloc(0x10000, modelP->mallocNo);
    DCFlushRange(dlBuf, 0x10000);
    GXBeginDisplayList(dlBegin, 0x10000);
    GXBegin(GX_QUADS, GX_VTXFMT0, num*4);
    for(i=0; i<num; i++) {
        GXPosition1x16(i*4);
        GXColor1x16(i);
        GXTexCoord1x16(i*4);

        GXPosition1x16((i*4)+1);
        GXColor1x16(i);
        GXTexCoord1x16((i*4)+1);

        GXPosition1x16((i*4)+2);
        GXColor1x16(i);
        GXTexCoord1x16((i*4)+2);

        GXPosition1x16((i*4)+3);
        GXColor1x16(i);
        GXTexCoord1x16((i*4)+3);
    }
    GXEnd();
    effP->dlSize = GXEndDisplayList();
    effP->dl = CapEffModelMalloc(effP->dlSize, modelP->mallocNo);
    memcpy(effP->dl, dlBuf, effP->dlSize);
    DCFlushRange(effP->dl, effP->dlSize);
    HuMemDirectFree(dlBuf);
    return modelId;
}

static void CapEffDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    static HuVecF posTbl[] = {
        { -0.5f,  0.5f, 0.0f },
        {  0.5f,  0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f },
        { -0.5f, -0.5f, 0.0f }
    };
    static HuVec2f uvTbl[] = {
        { 0.0f, 0.0f },
        { 0.25f, 0.0f },
        { 0.25f, 0.25f },
        { 0.0f, 0.25f },
    };
    CAPSULE_EFFECT *effP;
    CAPSULE_EFFDATA *effDataP;
    HuVecF *vtx;
    HuVec2f *st;
    HuVecF *scaleVtxP;
    s16 i;
    s16 j;
    HuVecF *initVtxP;
    s16 bmpFmt;
    s16 row;
    s16 col;
    CAPSULE_EFFHOOK hook;

    Mtx mtxInv;
    Mtx mtxPos;
    Mtx mtxRotZ;
    HuVecF scaleVtx[4];
    HuVecF finalVtx[4];
    HuVecF initVtx[4];
    ROMtx basePosMtx;

    effP = modelP->hookData;
    if(effP->prevCounter != GlobalCounter || shadowModelDrawF) {
        if(effP->hookMdlP && effP->hookMdlP != modelP){
            CapEffDraw(effP->hookMdlP, mtx);
        }
        GXLoadPosMtxImm(*mtx, GX_PNMTX0);
        GXSetNumTevStages(1);
        GXSetNumTexGens(1);
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        if(shadowModelDrawF) {
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ONE, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
            GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        } else {
            bmpFmt = (effP->anim->bmp->dataFmt & 0xF);
            if(bmpFmt == ANIM_BMP_I8 || bmpFmt == ANIM_BMP_I4) {
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_RASC, GX_CC_ZERO);
            } else {
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
            }
            if(effP->dispAttr & CAPSULE_EFF_DISPATTR_ZBUF_OFF) {
                GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
            } else if(modelP->attr & HU3D_ATTR_ZWRITE_OFF) {
                GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
            } else {
                GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
            }
        }
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        HuSprTexLoad(effP->anim, 0, GX_TEXMAP0, GX_REPEAT, GX_REPEAT, GX_LINEAR);
        GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
        GXSetZCompLoc(GX_FALSE);
        switch (effP->blendMode) {
            case HU3D_PARTICLE_BLEND_NORMAL:
                GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
                break;
            case HU3D_PARTICLE_BLEND_ADDCOL:
                GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
                break;
            case HU3D_PARTICLE_BLEND_INVCOL:
                GXSetBlendMode(GX_BM_BLEND, GX_BL_ZERO, GX_BL_INVDSTCLR, GX_LO_NOOP);
                break;
        }
        if(HmfInverseMtxF3X3(*mtx, mtxInv) == FALSE) {
            PSMTXIdentity(mtxInv);
        }
        PSMTXReorder(mtxInv, basePosMtx);
        if(effP->hook) {
            hook = effP->hook;
            hook(modelP, effP, mtx);
        }
        effDataP = effP->data;
        vtx = effP->vertex;
        st = effP->st;
        if(effP->dispAttr & CAPSULE_EFF_DISPATTR_CAMERA_ROT) {
            MTXIdentity(mtxInv);
            MTXIdentity(*(Mtx *)(&basePosMtx));
            initVtx[0] = posTbl[0];
            initVtx[1] = posTbl[1];
            initVtx[2] = posTbl[2];
            initVtx[3] = posTbl[3];
        } else {
            PSMTXROMultVecArray(basePosMtx, &posTbl[0], initVtx, 4);
        }
        for(i=0; i<effP->num; i++, effDataP++) {
            if(!effDataP->scale) {
                vtx->x = vtx->y = vtx->z = 0;
                vtx++;
                vtx->x = vtx->y = vtx->z = 0;
                vtx++;
                vtx->x = vtx->y = vtx->z = 0;
                vtx++;
                vtx->x = vtx->y = vtx->z = 0;
                vtx++;
            } else if(effP->dispAttr & CAPSULE_EFF_DISPATTR_ROT3D) {
                VECScale(&posTbl[0], &scaleVtx[0], effDataP->scale);
                VECScale(&posTbl[1], &scaleVtx[1], effDataP->scale);
                VECScale(&posTbl[2], &scaleVtx[2], effDataP->scale);
                VECScale(&posTbl[3], &scaleVtx[3], effDataP->scale);
                mtxRot(mtxPos, effDataP->rot.x, effDataP->rot.y, effDataP->rot.z);
                PSMTXMultVecArray(mtxPos, scaleVtx, finalVtx, 4);
                VECAdd(&finalVtx[0], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[1], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[2], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[3], &effDataP->pos, vtx++);
            } else if(!effDataP->rot.z) {
                scaleVtxP = scaleVtx;
                initVtxP = initVtx;
                VECScale(initVtxP++, scaleVtxP, effDataP->scale);
                VECAdd(scaleVtxP++, &effDataP->pos, vtx++);
                VECScale(initVtxP++, scaleVtxP, effDataP->scale);
                VECAdd(scaleVtxP++, &effDataP->pos, vtx++);
                VECScale(initVtxP++, scaleVtxP, effDataP->scale);
                VECAdd(scaleVtxP++, &effDataP->pos, vtx++);
                VECScale(initVtxP++, scaleVtxP, effDataP->scale);
                VECAdd(scaleVtxP++, &effDataP->pos, vtx++);
            } else {
                VECScale(&posTbl[0], &scaleVtx[0], effDataP->scale);
                VECScale(&posTbl[1], &scaleVtx[1], effDataP->scale);
                VECScale(&posTbl[2], &scaleVtx[2], effDataP->scale);
                VECScale(&posTbl[3], &scaleVtx[3], effDataP->scale);
                MTXRotRad(mtxRotZ, 'Z', effDataP->rot.z);
                PSMTXConcat(mtxInv, mtxRotZ, mtxPos);
                PSMTXMultVecArray(mtxPos, scaleVtx, finalVtx, 4);
                VECAdd(&finalVtx[0], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[1], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[2], &effDataP->pos, vtx++);
                VECAdd(&finalVtx[3], &effDataP->pos, vtx++);
            }
        }
        effDataP = effP->data;
        st = effP->st;
        if(!(effP->dispAttr & CAPSULE_EFF_DISPATTR_NOANIM)) {
            for(i=0; i<effP->num; i++, effDataP++) {
                row = effDataP->no & 0x3;
                col = (effDataP->no >> 2) & 0x3;
                for(j=0; j<4; j++, st++) {
                    st->x = (0.25f*row)+uvTbl[j].x;
                    st->y = (0.25f*col)+uvTbl[j].y;
                }
            }
        } else {
            for(i=0; i<effP->num; i++, effDataP++) {
                for(j=0; j<4; j++, st++) {
                    st->x = 4*uvTbl[j].x;
                    st->y = 4*uvTbl[j].y;
                }
            }
        }
        DCFlushRangeNoSync(effP->vertex, effP->num*sizeof(HuVecF)*4);
        DCFlushRangeNoSync(effP->st, effP->num*sizeof(HuVec2f)*4);
        DCFlushRangeNoSync(effP->data, effP->num*sizeof(CAPSULE_EFFDATA));
        PPCSync();
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetArray(GX_VA_POS, effP->vertex, sizeof(HuVecF));
        GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        GXSetArray(GX_VA_CLR0, &effP->data->color, sizeof(CAPSULE_EFFDATA));
        GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetArray(GX_VA_TEX0, effP->st, sizeof(HuVec2f));
        GXCallDisplayList(effP->dl, effP->dlSize);
        if(shadowModelDrawF == FALSE) {
            if(!(effP->attr & CAPSULE_EFF_ATTR_COUNTER_UPDATE)) {
                effP->count++;
            }
            if(effP->prevCount != 0 && effP->prevCount <= effP->count) {
                if(effP->attr & CAPSULE_EFF_ATTR_COUNTER_RESET) {
                    effP->count = 0;
                }
                effP->count = effP->prevCount;
            }
            effP->prevCounter = GlobalCounter;
        }
    }
}

static float CapAngleSumWrap(float start, float end)
{
    float result;
    if(start >= 360) {
        start -= 360;
    } else if(start < 0) {
        start += 360;
    }
    if(end >= 360) {
        end -= 360;
    } else if(end < 0) {
        end += 360;
    }
    result = start-end;
    if(result <= -180.0f) {
        result += 360.0f;
    } else if(result >= 180.0f){
        result -= 360.0f;
    }
    return result;
}

static float CapCameraXZAngleGet(float angle)
{
    MBCAMERA *cameraP = mbCameraGet();
    Mtx rot;
    HuVecF dir;
    mtxRot(rot, cameraP->rot.x, cameraP->rot.y, cameraP->rot.z);
    dir.x = HuSin(angle);
    dir.y = 0;
    dir.z = HuCos(angle);
    MTXMultVec(rot, &dir, &dir);
    return HuAtan(dir.x, dir.z);
}

static void CapComChoiceSet(int choice)
{
    capsuleComChoice = choice;
    mbWinTopComKeyHookSet(CapComKeyHook);
}

static void CapComKeyHook(void)
{
    int comKey[GW_PLAYER_MAX];
    int i;
    int padNo;
    int key;
    s16 delay;
    int playerNo;
    comKey[0] = comKey[1] = comKey[2] = comKey[3] = 0;
    playerNo = GwSystem.turnPlayerNo;
    padNo = GwPlayer[playerNo].padNo;
    delay = GWComKeyDelayGet();
    if(capsuleComChoice >= 0) {
        key = PAD_BUTTON_RIGHT;
    } else {
        key = PAD_BUTTON_LEFT;
    }
    comKey[padNo] = key;
    for(i=0; i<abs(capsuleComChoice); i++) {
        comKey[padNo] = key;
        HuWinComKeyWait(comKey[0], comKey[1], comKey[2], comKey[3], delay);
    }
    comKey[padNo] = PAD_BUTTON_A;
    HuWinComKeyWait(comKey[0], comKey[1], comKey[2], comKey[3], delay);
}

static inline void CapThrowCameraParamCalc(float *pos, float *paramOut, float *paramIn, int num)
{
    int i;
    float delta[8];
    float speed[8];

    paramOut[0] = 0;
    paramOut[num-1] = 0;
    for(i=0; i<num-1; i++) {
        delta[i] = paramIn[i+1]-paramIn[i];
        speed[i+1] = (pos[i+1]-pos[i])/delta[i];
    }
    paramOut[1] = (speed[2]-speed[1])-(delta[0]*paramOut[0]);
    speed[1] = 2*(paramIn[2]-paramIn[0]);
    for(i=1; i<num-2; i++) {
        double t = delta[i]/speed[i];
        paramOut[i+1] = (speed[i+2]-speed[i+1])-(paramOut[i]*t);
        speed[i+1] = (2*(paramIn[i+2]-paramIn[i]))-(delta[i]*t);
    }
    paramOut[num-2] -= delta[num-2]*paramOut[num-1];
    for(i=num-2; i>0; i--) {
        paramOut[i] = (paramOut[i]-(delta[i]*paramOut[i+1]))/speed[i];
    }
}

static void CapThrowCameraSet(float *x, float *y, float *z, int num)
{
    int i;
    float dx;
    float dy;
    capsuleTime[0] = 0;
    for(i=1; i<num; i++) {
        dx = x[i]-x[i-1];
        dy = y[i]-y[i-1];
        capsuleTime[i] = capsuleTime[i-1]+sqrt((dx*dx)+(dy*dy));
    }
    for(i=1; i<num; i++) {
        capsuleTime[i] /= capsuleTime[num-1];
    }
    CapThrowCameraParamCalc(x, capsuleBezierX, capsuleTime, num);
    CapThrowCameraParamCalc(y, capsuleBezierY, capsuleTime, num);
    CapThrowCameraParamCalc(z, capsuleBezierZ, capsuleTime, num);
}

static inline float CapThrowCameraBezierGet(float t, float *pos, float *param, float *time, int num)
{
    int i;
    int max;
    int idx;
    float timeFrac;
    float dt;

    for(i=0, max=num-1; i<max;) {
        idx = (i+max)/2;
        if(time[idx] < t) {
            i = idx+1;
        } else{
            max = idx;
        }
    }
    if(i > 0) {
        i--;
    }
    dt = time[i+1]-time[i];
    timeFrac = t-time[i];
    return pos[i] + (timeFrac * ((timeFrac * ((3.0f * param[i]) + ((timeFrac * (param[i+1] - param[i])) / dt))) + (((pos[i+1] - pos[i]) / dt) - (dt * ((2 * param[i]) + param[i+1])))));
}

static void CapThrowCameraCalc(float t, float *x, float *y, float *z, HuVecF *out, int num)
{
    out->x = CapThrowCameraBezierGet(t, x, capsuleBezierX, capsuleTime, num);
    out->y = CapThrowCameraBezierGet(t, y, capsuleBezierY, capsuleTime, num);
    out->z = CapThrowCameraBezierGet(t, z, capsuleBezierZ, capsuleTime, num);
}

static void CapColMdlIdGet(void)
{
    int boardNo = mbBoardNoGet();
    if(capsuleColObjId != -1) {
        capsuleColMdlId = mbObjModelIDGet(capsuleColObjId);
    }
}

static BOOL CapColCheck(HuVecF *posA, HuVecF *posB, HuVecF *out)
{
    HSF_FACE *faceP;
    HSF_DATA *hsfP;
    HSF_BUFFER *faceBufP;
    HSF_BUFFER *vtxBufP;
    BOOL quadF;
    int i;
    int j;
    HU3D_MODEL *modelP;

    float dot;
    float dotEdge;
    float dotA;
    float dotB;
    float invDotA;
    float scale;

    HuVecF faceVtx[4];
    HuVecF faceNorm;
    HuVecF faceCA;
    HuVecF faceBA;
    HuVecF faceBAQuad;
    HuVecF faceCAQuad;
    HuVecF faceNormQuad;
    HuVecF dir;
    HuVecF outPos;
    HuVecF dotVec;

    float mag;
    int temp;

    quadF = FALSE;
    temp = 0;
    if(capsuleColMdlId == HU3D_MODELID_NONE) {
        return FALSE;
    }
    modelP = &Hu3DData[capsuleColMdlId];
    hsfP = modelP->hsf;
    faceBufP = hsfP->face;
    vtxBufP = hsfP->vertex;
    VECSubtract(posA, posB, &dir);
    if(VECMag(&dir) <= 0.0f) {
        return FALSE;
    }
    VECNormalize(&dir, &dir);
    mag = VECMag(&dir);
    for(i=0; i<hsfP->faceNum; i++, faceBufP++) {
        faceP = faceBufP->data;
        for(j=0; j<faceBufP->count; j++, faceP++) {
            switch(faceP->type & HSF_FACE_MASK) {
                case 0:
                    quadF = TRUE;
                    break;

                case 1:
                    quadF = TRUE;
                    break;

                case 2:
                    quadF = FALSE;
                    faceVtx[0] = *(((HuVecF *)vtxBufP->data)+faceP->index[0].vertex);
                    faceVtx[1] = *(((HuVecF *)vtxBufP->data)+faceP->index[1].vertex);
                    faceVtx[2] = *(((HuVecF *)vtxBufP->data)+faceP->index[2].vertex);
                    break;

                case 3:
                    quadF = TRUE;
                    break;

                default:
                    break;
            }
            if(!quadF) {
                VECSubtract(&faceVtx[1], &faceVtx[0], &faceBAQuad);
                VECSubtract(&faceVtx[2], &faceVtx[0], &faceCAQuad);
                VECCrossProduct(&faceBAQuad, &faceCAQuad, &faceNormQuad);
                if(VECMag(&faceNormQuad) > 0.0f) {
                    VECNormalize(&faceNormQuad, &faceNormQuad);
                }
                dot = -VECDotProduct(&faceNormQuad, &faceVtx[0]);
                dotA = (faceNormQuad.x*posA->x)+(faceNormQuad.y*posA->y)+(faceNormQuad.z*posA->z)+dot;
                dotB = (faceNormQuad.x*posB->x)+(faceNormQuad.y*posB->y)+(faceNormQuad.z*posB->z)+dot;
                if(dotA*dotB > 0) {
                    continue;
                }
                invDotA = -((faceNormQuad.x*posA->x)+(faceNormQuad.y*posA->y)+(faceNormQuad.z*posA->z)+dot);
                dotEdge = (faceNormQuad.x*dir.x)+(faceNormQuad.y*dir.y)+(faceNormQuad.z*dir.z);
                if(dotEdge != 0.0f) {
                    scale = invDotA/dotEdge;
                    VECScale(&dir, &outPos, scale);
                    VECAdd(&outPos, posA, &outPos);
                    *out = outPos;
                    VECSubtract(&faceVtx[1], &faceVtx[0], &faceBA);
                    VECSubtract(&outPos, &faceVtx[0], &faceCA);
                    VECCrossProduct(&faceBA, &faceCA, &faceNorm);
                    dotVec.x = VECDotProduct(&faceNormQuad, &faceNorm);
                    VECSubtract(&faceVtx[2], &faceVtx[1], &faceBA);
                    VECSubtract(&outPos, &faceVtx[1], &faceCA);
                    VECCrossProduct(&faceBA, &faceCA, &faceNorm);
                    dotVec.y = VECDotProduct(&faceNormQuad, &faceNorm);
                    VECSubtract(&faceVtx[0], &faceVtx[2], &faceBA);
                    VECSubtract(&outPos, &faceVtx[2], &faceCA);
                    VECCrossProduct(&faceBA, &faceCA, &faceNorm);
                    dotVec.z = VECDotProduct(&faceNormQuad, &faceNorm);
                    if(dotVec.x > 0 && dotVec.y > 0 && dotVec.z > 0) {
                        return TRUE;
                    }
                    if(dotVec.x < 0 && dotVec.y < 0 && dotVec.z < 0) {
                        return TRUE;
                    }
                }
            }
        }
    }
    return FALSE;
}

static void CapColKill(void)
{
    capsuleColMdlId = HU3D_MODELID_NONE;
}



static BOOL CapColExec(int playerNo, HuVecF *posA, HuVecF *posB, HuVecF *out)
{
    int i;
    int j;

    float dot;
    float dotEdge;
    float dotA;
    float dotB;
    float invDotA;
    float scale;

    HuVecF faceVtx[4];
    HuVecF faceNorm;
    HuVecF faceCA;
    HuVecF faceBA;
    HuVecF faceBAQuad;
    HuVecF faceCAQuad;
    HuVecF faceNormQuad;
    HuVecF dir;
    HuVecF outPos;
    HuVecF playerPos;
    HuVecF dotVec;

    float mag;
    BOOL quadF;
    int temp;

    quadF = FALSE;
    temp = 0;
    VECSubtract(posA, posB, &dir);
    if(VECMag(&dir) <= 0.0f) {
        return FALSE;
    }
    VECNormalize(&dir, &dir);
    mag = VECMag(&dir);
    for(i=0; i<GW_PLAYER_MAX; i++) {
        if(GWPartyGet() == FALSE && i != 0) {
            continue;
        }
        if(i == playerNo) {
            continue;
        }
        if(!mbPlayerDispGet(i)) {
            continue;
        }
        mbPlayerPosGet(i, &playerPos);
        for(j=0; j<6; j++) {
            faceVtx[0].x = colScaleTbl[j*3].x*charSizeTbl[GwPlayer[i].charNo].x;
            faceVtx[0].y = colScaleTbl[j*3].y*charSizeTbl[GwPlayer[i].charNo].y;
            faceVtx[0].z = colScaleTbl[j*3].z*charSizeTbl[GwPlayer[i].charNo].x;
            faceVtx[1].x = colScaleTbl[(j*3)+1].x*charSizeTbl[GwPlayer[i].charNo].x;
            faceVtx[1].y = colScaleTbl[(j*3)+1].y*charSizeTbl[GwPlayer[i].charNo].y;
            faceVtx[1].z = colScaleTbl[(j*3)+1].z*charSizeTbl[GwPlayer[i].charNo].x;
            faceVtx[2].x = colScaleTbl[(j*3)+2].x*charSizeTbl[GwPlayer[i].charNo].x;
            faceVtx[2].y = colScaleTbl[(j*3)+2].y*charSizeTbl[GwPlayer[i].charNo].y;
            faceVtx[2].z = colScaleTbl[(j*3)+2].z*charSizeTbl[GwPlayer[i].charNo].x;
            VECAdd(&faceVtx[0], &playerPos, &faceVtx[0]);
            VECAdd(&faceVtx[1], &playerPos, &faceVtx[1]);
            VECAdd(&faceVtx[2], &playerPos, &faceVtx[2]);
            VECSubtract(&faceVtx[1], &faceVtx[0], &faceBAQuad);
            VECSubtract(&faceVtx[2], &faceVtx[0], &faceCAQuad);
            VECCrossProduct(&faceBAQuad, &faceCAQuad, &faceNormQuad);
            if(VECMag(&faceNormQuad) > 0.0f) {
                VECNormalize(&faceNormQuad, &faceNormQuad);
            }
            dot = -VECDotProduct(&faceNormQuad, &faceVtx[0]);
            dotA = (faceNormQuad.x*posA->x)+(faceNormQuad.y*posA->y)+(faceNormQuad.z*posA->z)+dot;
            dotB = (faceNormQuad.x*posB->x)+(faceNormQuad.y*posB->y)+(faceNormQuad.z*posB->z)+dot;
            if(dotA*dotB > 0) {
                continue;
            }
            invDotA = -((faceNormQuad.x*posA->x)+(faceNormQuad.y*posA->y)+(faceNormQuad.z*posA->z)+dot);
            dotEdge = (faceNormQuad.x*dir.x)+(faceNormQuad.y*dir.y)+(faceNormQuad.z*dir.z);
            if(dotEdge != 0.0f) {
                scale = invDotA/dotEdge;
                VECScale(&dir, &outPos, scale);
                VECAdd(&outPos, posA, &outPos);
                *out = outPos;
                VECSubtract(&faceVtx[1], &faceVtx[0], &faceBA);
                VECSubtract(&outPos, &faceVtx[0], &faceCA);
                VECCrossProduct(&faceBA, &faceCA, &faceNorm);
                dotVec.x = VECDotProduct(&faceNormQuad, &faceNorm);
                VECSubtract(&faceVtx[2], &faceVtx[1], &faceBA);
                VECSubtract(&outPos, &faceVtx[1], &faceCA);
                VECCrossProduct(&faceBA, &faceCA, &faceNorm);
                dotVec.y = VECDotProduct(&faceNormQuad, &faceNorm);
                VECSubtract(&faceVtx[0], &faceVtx[2], &faceBA);
                VECSubtract(&outPos, &faceVtx[2], &faceCA);
                VECCrossProduct(&faceBA, &faceCA, &faceNorm);
                dotVec.z = VECDotProduct(&faceNormQuad, &faceNorm);
                if(dotVec.x > 0 && dotVec.y > 0 && dotVec.z > 0) {
                    return TRUE;
                }
                if(dotVec.x < 0 && dotVec.y < 0 && dotVec.z < 0) {
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

/* @@END@@ */
