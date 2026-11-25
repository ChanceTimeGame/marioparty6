/* @@HEAD@@ */
#include "game/board/capsule.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/capselect.h"
#include "game/board/exit.h"
#include "game/board/masu.h"
#include "game/board/math.h"
#include "game/board/object.h"
#include "game/board/pause.h"
#include "game/board/player.h"
#include "game/board/single.h"
#include "game/board/tutorial.h"
#include "game/board/window.h"
#include "game/board/wipe.h"

#include "game/charman.h"
#include "game/flag.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/process.h"
#include "math.h"
#include "string.h"

/* capevent.c: declared here (not through capevent.h) with the types retail passes */
extern u32 mbCapEffNum;
extern s16 *mbCapEffData;
int mbev_CapCall(int playerNo, int capsuleNo, int arg2, int arg3);
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

typedef struct CapSelectMasuWork_s {
    int playerNo; //0x00
    int masuId; //0x04
    int capsuleNo; //0x08
    u8 unk0C[0x14]; //0x0C
} CAPSELECTMASUWORK; //size=0x20

typedef struct CapListFile_s {
    int boardNo;
    u32 file;
} CAPLISTFILE;

typedef struct CapListDefine_s {
    int capsuleNo;
    char *name;
} CAPLISTDEFINE;

typedef struct CapObjData_s {
    int capsuleNo; //0x00
    int unk4; //0x04
    s16 objId; //0x08
    s16 unkA; //0x0A
} CAPOBJDATA; //size=0xC

typedef struct CapObjColorData_s {
    s16 objId; //0x00
    s16 mdlId; //0x02
    u8 unk4[2]; //0x04
    u8 layer; //0x06
    HuVecF pos; //0x08
    HuVecF rot; //0x14
    HuVecF scale; //0x20
} CAPOBJCOLORDATA; //size=0x2C

/* Statics defined in later parts (forward declarations for the calls part 1 makes). */
static int CapUseDelete(int playerNo, int capsuleNo);
static void CapSelectMasuPlayer(CAPSELECTMASUWORK *work);
static void CapSelectMasuCom(CAPSELECTMASUWORK *work);
static void CapSelectMasuListGet(s16 *list, s16 masuId, int frontNum, int backNum);
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
static int capsuleBorderObjId;
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
    work = CapMalloc(sizeof(CAPUSEWORK));
    capsuleUseEffProc[playerNo]->property = work;
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
    CAPSELECTMASUWORK *work;
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
    work = CapMalloc(sizeof(CAPSELECTMASUWORK));
    work->playerNo = playerNo;
    work->masuId = -1;
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
/* @@PART3@@ */
/* @@PART4@@ */
/* @@END@@ */
