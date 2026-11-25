/* @@HEAD@@ */
/* Placeholder HEAD from part 3 (wt-b403); the splice replaces it with part 1's. */
#include "game/board/capsule.h"
#include "game/board/masu.h"
#include "game/board/math.h"
#include "game/board/window.h"
#include "game/board/object.h"
#include "game/board/player.h"
#include "game/board/exit.h"
#include "game/hsfload.h"
#include "game/hsfformat.h"
#include "game/memory.h"
#include "game/data.h"
#include "game/sprite.h"
#include "game/hu3d.h"
#include "string.h"

typedef struct CapsuleData_s {
    u32 file;
    u32 colorFile;
    u32 descMes;
    u32 useMes;
    s32 masuPat;
    s32 color;
    s32 cost;
    s8 chanceType;
    u16 useMode;
    char *debugName;
    u8 unk24;
} CAPSULE_DATA;

typedef struct CapsuleList_s {
    s8 capsuleNo;
    s8 unk1[3];
    s8 masuChance[8];
    s8 shopChance[4];
} CAPSULE_LIST;

typedef struct CapsuleObjColor_s {
    s16 useF;
    s16 objId;
    s16 unk4;
    u8 layer;
    HuVecF pos;
    HuVecF rot;
    HuVecF scale;
} CAPSULE_OBJCOLOR;

typedef struct CapsuleObjData_s {
    int objId;
    ANIMDATA *anim;
    s16 animId1;
    s16 animId2;
} CAPSULE_OBJDATA;

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
} CAPSULE_EFFECT;

#define CapRandF() ((1.0f/268435456.0f)*mbRandMod(0x10000000))

static int capsuleMasuSelectComF[4] = { 0, 0, 0, 0 };
static CAPSULE_DATA capsuleData[60] = {
    { 0xC0000, 0xC001C, 0x370000, 0x380000, 0, 0, 5, 0x41, 0, "KINOKO", 1 },
    { 0xC0001, 0xC001C, 0x370001, 0x380001, 0, 0, 10, 0x42, 0, "S KINOKO", 1 },
    { 0xC0002, 0xC001C, 0x370002, 0x380002, 0, 0, 10, 0x41, 0, "P KINOKO", 1 },
    { 0xC0003, 0xC001C, 0x370003, 0x380003, 0, 0, 20, 0x41, 0, "M KINOKO", 1 },
    { 0xC0004, 0xC001C, 0x370004, 0x380004, 0, 0, 20, 0x42, 0, "KILLER", 1 },
    { 0xC0005, 0xC001C, 0x370005, 0x380005, 0, 0, 15, 0x43, 0, "DOKAN", 1 },
    { 0xC0006, 0xC001C, 0x370006, 0x380006, 0, 0, 30, 0x45, 0, "HANACHAN", 1 },
    { 0xC0007, 0xC001C, 0x370007, 0x380007, 0, 0, 30, 0x45, 0, "N KINOKO", 1 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 1, "NULL", 0 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 1, "NASI", 0 },
    { 0xC0008, 0xC001D, 0x370008, 0x380008, 1, 1, 10, 0x41, 1, "TOGEZO", 1 },
    { 0xC0009, 0xC001D, 0x370009, 0x380009, 1, 1, 15, 0x41, 1, "KURIBO", 1 },
    { 0xC000A, 0xC001D, 0x37000A, 0x38000A, 1, 1, 20, 0x42, 1, "PAKKUN", 1 },
    { 0xC000B, 0xC001D, 0x37000B, 0x38000B, 1, 1, 5, 0x44, 1, "JANGO", 1 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 0, "HANUKE", 0 },
    { 0xC000C, 0xC001D, 0x37000D, 0x38000D, 1, 1, 10, 0x43, 1, "KOKAMEKKU", 1 },
    { 0xC000D, 0xC001D, 0x37000E, 0x38000E, 1, 1, 5, 0x42, 1, "KAMEKKU", 1 },
    { 0xC000E, 0xC001D, 0x37000F, 0x38000F, 1, 1, 10, 0x41, 1, "THROWMAN", 1 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 0, "SUKA", 0 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 0, "KARA", 0 },
    { 0xC000F, 0xC001E, 0x370010, 0x380010, 2, 2, 10, 0x42, 2, "BOBLE", 1 },
    { 0xC0010, 0xC001E, 0x370011, 0x380011, 2, 2, 15, 0x43, 2, "BIRIQ", 1 },
    { 0xC0011, 0xC001E, 0x370012, 0x380012, 2, 2, 15, 0x42, 2, "TUMUJIKUN", 1 },
    { 0xC0012, 0xC001E, 0x370013, 0x380013, 2, 2, 15, 0x43, 2, "DOSSUN", 1 },
    { 0xC0013, 0xC001E, 0x370014, 0x380014, 2, 2, 10, 0x43, 2, "BOMUHEI", 1 },
    { 0xC0014, 0xC001E, 0x37000C, 0x38000C, 2, 2, 10, 0x45, 2, "PATAPATA", 1 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 1, "NETA", 0 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 1, "GA  ", 0 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 1, "TUKI", 0 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 1, "TA  ", 0 },
    { 0xC0015, 0xC001F, 0x370015, 0x380015, 3, 3, 20, 0x44, 3, "HONE", 1 },
    { 0xC0016, 0xC001F, 0x370016, 0x380016, 3, 3, 10, 0x44, 3, "LIGHT", 1 },
    { 0xC0021, 0xC001F, 0x370017, 0x380017, 3, 3, 15, 0x44, 3, "TARU", 1 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 1, "0000", 0 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 1, "0000", 0 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 1, "0000", 0 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 1, "0000", 0 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 1, "0000", 0 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 1, "0000", 0 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 1, "0000", 0 },
    { 0xC0021, 0xC001F, 0x370018, 0x380018, 10, 3, 1, 0x43, 3, "KILLER MOVE", 0 },
    { 0xC0021, 0xC001F, 0x370019, 0x380019, 10, 3, 1, 0x43, 3, "KETTOU", 0 },
    { 0xC0021, 0xC0020, 0x37001A, 0x38001A, 10, 3, 1, 0x43, 3, "MIRACLE", 0 },
    { 0xC0021, 0xC001F, 0x37001B, 0x38001B, 9, 3, 1, 0x45, 3, "KOOPA", 0 },
    { 0xC0021, 0xC001F, 0x37001C, 0x38001C, 10, 3, 1, 0x45, 3, "DONKEY", 0 },
    { 0xC0021, 0xC001F, 0x37001D, 0x38001D, 10, 3, 1, 0x5A, 3, "VS", 0 },
    { 0xC0021, 0xC001F, 0x37001E, 0x38001E, 10, 3, 1, 0x5A, 3, "R_TERESA", 0 },
    { 0x50017, 0xC001F, 0x37001F, 0x380022, 10, 0, 0, 0x5A, 0, "DICE", 1 },
    { 0xC0022, 0xC001F, 0x370020, 0x380022, 10, 0, 0, 0x5A, 0, "YAMERU", 1 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 1, "0000", 0 },
    { 0xC0021, 0xC001F, 0x370021, 0x38001F, 10, 0, 1, 0x5A, 0, "DEBUG CAM TEST", 0 },
    { 0xC0021, 0xC001F, 0x370022, 0x380020, 10, 0, 1, 0x5A, 0, "DEBUG WARP TEST", 0 },
    { 0xC0021, 0xC001F, 0x370023, 0x380021, 10, 0, 1, 0x5A, 0, "DEBUG SETPOS TEST", 0 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 1, "0000", 0 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 1, "0000", 0 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 1, "0000", 0 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 1, "0000", 0 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 1, "0000", 0 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 1, "0000", 0 },
    { 0x0, 0x0, 0x0, 0x380022, 10, 0, 0, 0x5A, 1, "0000", 0 },
};
static u8 capsulePadData[0x1600-0xA0C] = { 1 };

static void *capsuleThrowHook;
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

static HUPROCESS *capsuleUseEffProc[4];
static int capsuleUseEffMode[4];
static HuVecF capsuleUseEffPos[4];
static CAPSULE_LIST capsuleList[33];
static s32 capsuleNum[33];

s16 mbCapValueTypeGet(s16 value)
{
    return value & 0xFF;
}

s16 mbCapMasuTypeGet(s16 masuId)
{
    return mbCapValueTypeGet(mbMasuCapsuleGet(masuId));
}

s16 mbCapUseModeGet(s16 capsuleNo)
{
    capsuleNo = mbCapValueTypeGet(capsuleNo);
    return capsuleData[capsuleNo].useMode;
}

static void CapSelectMasuAddFront(s16 *list, s16 masuId, int depth);
static void CapSelectMasuAddBack(s16 *list, s16 masuId, int depth);
/* @@PART3@@ */
static int capsuleThrowTbl[8] = { 10, 11, 12, 13, 25, 15, 16, 17 };
static int capsuleTrapTbl[5] = { 20, 21, 22, 23, 24 };
static int capsuleChanceTbl[3][4][5] = {
    { { 70, 15, 15, 0, 0 }, { 70, 20, 10, 0, 0 }, { 70, 20, 0, 10, 0 }, { 50, 30, 0, 20, 0 } },
    { { 40, 30, 30, 0, 0 }, { 30, 30, 20, 20, 0 }, { 10, 40, 10, 30, 10 }, { 10, 40, 0, 30, 20 } },
    { { 30, 30, 30, 10, 0 }, { 20, 20, 20, 30, 10 }, { 10, 30, 0, 40, 20 }, { 10, 20, 0, 40, 30 } }
};
static int capsuleMaxTurnTbl[9] = { 10, 15, 20, 25, 30, 35, 40, 45, 50 };
static int capsuleTurnTbl[9][2] = {
    { 3, 6 }, { 5, 10 }, { 5, 15 }, { 8, 16 }, { 10, 20 }, { 10, 20 }, { 13, 26 }, { 15, 30 }, { 15, 35 }
};

static CAPSULE_OBJCOLOR capsuleObjColorData[128];
static s16 capsuleObjBorderId[6];
static CAPSULE_OBJDATA capsuleObjData[8];

static s16 CapEffCreate(ANIMDATA *anim, s16 num);
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

void mbCapMasuCapsuleSet(s16 masuId, s16 capsuleNo, s16 playerNo)
{
    s16 value;
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
    mbMasuCapsuleSet(masuId, capsuleNo);
    value = mbMasuCapsuleGet(masuId);
    value = value | ((u8)playerNo << 8);
    mbMasuCapsuleSet(masuId, value);
    switch(capsuleNo) {
        case 0x18:
            break;
    }
}

static void CapThrowEndWin(int playerNo, s16 capsuleNo)
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

int mbCapMasuDispTypeGet(s16 masuId)
{
    s16 capsuleNo = mbCapMasuTypeGet(masuId);
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

int mbCapSelectMasuNum(s16 masuId)
{
    int backNum = mbCapSelectMasuBackNum(masuId);
    return mbCapSelectMasuFrontNum(masuId)+backNum;
}

s16 mbCapSelectMasuFrontNum(s16 masuId)
{
    s16 *list;
    s16 i;
    s16 num;
    list = HuMemDirectMallocNum(HEAP_HEAP, 0x200, HU_MEMNUM_OVL);
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

s16 mbCapSelectMasuBackNum(s16 masuId)
{
    s16 *list;
    s16 i;
    s16 num;
    list = HuMemDirectMallocNum(HEAP_HEAP, 0x200, HU_MEMNUM_OVL);
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
            list[num++] = i;
        }
    }
    return num;
}

typedef struct CapsuleChance_s {
    s8 type;
    int sum;
} CAPSULE_CHANCE;

int mbCapNextGet(int chanceNo)
{
    int i;
    int num;
    int a;
    int b;
    int temp;
    int turnNo;
    int phase;
    int *tbl;
    int sum;
    int chanceNum;
    int chance;
    int type;
    int listNum;
    CAPSULE_CHANCE chanceList[5];
    int list[33];
    int validList[34];
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
    tbl = capsuleChanceTbl[phase][chanceNo];
    for(i=0, sum=0, chanceNum=0; i<5; i++) {
        if(tbl[i] > 0) {
            chanceList[chanceNum].type = i+'A';
            sum += tbl[i];
            chanceList[chanceNum].sum = sum;
            chanceNum++;
        }
    }
    chance = CapRandF()*sum;
    for(i=0; i<chanceNum; i++) {
        if(chance < chanceList[i].sum) {
            break;
        }
    }
    if(i >= chanceNum) {
        i = 0;
    }
    type = chanceList[i].type;
    for(i=0, listNum=0; i<num; i++) {
        if(type == capsuleData[validList[i]].chanceType) {
            list[listNum++] = validList[i];
        }
    }
    if(listNum <= 0) {
        return mbCapNextGet(-1);
    }
    for(i=0; i<listNum*5; i++) {
        a = i%listNum;
        b = mbRandMod(listNum);
        if(a != b) {
            temp = list[a];
            list[a] = list[b];
            list[b] = temp;
        }
    }
    i = mbRandMod(listNum);
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
    int chance;
    int result;
    int idx;
    if(GwSystem.turnNo <= GwSystem.turnMax/2) {
        idx = GwPlayer[playerNo].rank;
    } else {
        idx = GwPlayer[playerNo].rank+4;
    }
    list = HuMemDirectMallocNum(HEAP_HEAP, 0x210, HU_MEMNUM_OVL);
    listP = list;
    for(i=0, num=0, sum=0; i<0x21; i++) {
        if(capsuleList[i].capsuleNo == -1) {
            break;
        }
        if(capsuleList[i].masuChance[idx] > 0) {
            *listP = capsuleList[i];
            for(j=0; j<mbPlayerCapsuleNumGet(playerNo); j++) {
                if(listP->capsuleNo == mbPlayerCapsuleGet(playerNo, j)) {
                    listP->masuChance[idx] /= 2;
                }
            }
            sum += listP->masuChance[idx];
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
            if(list[i].masuChance[idx] > list[j].masuChance[idx]) {
                temp = list[i];
                list[i] = list[j];
                list[j] = temp;
            }
        }
    }
    chance = CapRandF()*sum;
    for(i=0; i<num; i++) {
        if((chance -= list[i].masuChance[idx]) <= 0) {
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
    int chance;
    int result;
    int idx;
    idx = GwPlayer[playerNo].rank;
    list = HuMemDirectMallocNum(HEAP_HEAP, 0x210, HU_MEMNUM_OVL);
    listP = list;
    for(i=0, num=0, sum=0; i<0x21; i++) {
        if(capsuleList[i].capsuleNo == -1) {
            break;
        }
        if(exclude1 != capsuleList[i].capsuleNo && exclude2 != capsuleList[i].capsuleNo && capsuleList[i].shopChance[idx] > 0) {
            *listP = capsuleList[i];
            for(j=0; j<mbPlayerCapsuleNumGet(playerNo); j++) {
                if(listP->capsuleNo == mbPlayerCapsuleGet(playerNo, j)) {
                    listP->masuChance[idx] /= 2;
                }
            }
            sum += listP->masuChance[idx];
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
            if(list[i].shopChance[idx] > list[j].shopChance[idx]) {
                temp = list[i];
                list[i] = list[j];
                list[j] = temp;
            }
        }
    }
    chance = CapRandF()*sum;
    for(i=0; i<num; i++) {
        if((chance -= list[i].shopChance[idx]) <= 0) {
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
    int shop[3];
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
                *out++ = capsuleList[j];
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
    list = HuMemDirectMallocNum(HEAP_HEAP, 0x210, HU_MEMNUM_OVL);
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

int mbCapBonusCoinNumGet(int playerNo, s16 capsuleNo)
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
            if(CapRandF()*100 < 20+(rank*10)) {
                coin = mbRandMod(rank+1)+1;
            }
            break;
        case 1:
            if(CapRandF()*100 < 40+(rank*10)) {
                coin = mbRandMod(rank+3)+5;
            }
            break;
        case 2:
            if(CapRandF()*100 < 40+(rank*10)) {
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
    MBOBJID objId;
    memset(capsuleObjColorData, 0, sizeof(capsuleObjColorData));
    for(i=0; i<6; i++) {
        capsuleObjBorderId[i] = -1;
    }
    idP = capsuleBorderObjId = HuMemDirectMallocNum(HEAP_HEAP, 0x300, HU_MEMNUM_OVL);
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

int mbCapObjCreate(int capsuleNo)
{
    int i;
    MBOBJID objId;
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

s16 mbCapObjBorderCreate(s16 objId, int capsuleNo)
{
    HU3D_MODELID mdlId;
    HSF_DATA *hsf;
    HSF_OBJECT *object;
    HSF_OBJECT *objectP;
    s16 *idP;
    int i;
    int borderNo;
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
        for(i=0; i<64; i++, idP++) {
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
    return capsuleData[capsuleNo].colorFile;
}

s16 mbCapObjColorCreate(int capsuleNo, BOOL linkF)
{
    CAPSULE_OBJCOLOR *objP = capsuleObjColorData;
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
    CAPSULE_OBJCOLOR *objP = capsuleObjColorData;
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
    CAPSULE_OBJCOLOR *objP;
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
    CAPSULE_OBJCOLOR *objP;
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
    CAPSULE_OBJCOLOR *objP;
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
    CAPSULE_OBJCOLOR *objP;
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
    CAPSULE_OBJCOLOR *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        mbCapObjColorPosSet(objId, pos->x, pos->y, pos->z);
    }
}

void mbCapObjColorRotSetV(int objId, HuVecF *rot)
{
    CAPSULE_OBJCOLOR *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        mbCapObjColorRotSet(objId, rot->x, rot->y, rot->z);
    }
}

void mbCapObjColorScaleSetV(int objId, HuVecF *scale)
{
    CAPSULE_OBJCOLOR *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        mbCapObjColorScaleSet(objId, scale->x, scale->y, scale->z);
    }
}

void mbCapObjColorPosGet(int objId, HuVecF *pos)
{
    CAPSULE_OBJCOLOR *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        *pos = objP->pos;
    }
}

void mbCapObjColorRotGet(int objId, HuVecF *rot)
{
    CAPSULE_OBJCOLOR *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        *rot = objP->rot;
    }
}

void mbCapObjColorScaleGet(int objId, HuVecF *scale)
{
    CAPSULE_OBJCOLOR *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        *scale = objP->scale;
    }
}

void mbCapObjColorLayerSet(int objId, u8 layer)
{
    CAPSULE_OBJCOLOR *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        objP->layer = layer;
        mbObjLayerSet(objP->objId, objP->layer);
    }
}

u8 mbCapObjColorLayerGet(int objId)
{
    CAPSULE_OBJCOLOR *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) == -1) {
        return 0;
    }
    objP = &capsuleObjColorData[no];
    return objP->layer;
}

void mbCapObjColorAttrSet(int objId, u32 attr)
{
    CAPSULE_OBJCOLOR *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        CapObjAttrSet(objP->objId, attr);
    }
}

void mbCapObjColorAttrReset(int objId, u32 attr)
{
    CAPSULE_OBJCOLOR *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        CapObjAttrReset(objP->objId, attr);
    }
}

void mbCapObjColorDispSet(int objId, BOOL dispF)
{
    CAPSULE_OBJCOLOR *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        mbObjDispSet(objP->objId, dispF);
    }
}

void mbCapObjColorAlphaSet(int objId, u8 alpha)
{
    CAPSULE_OBJCOLOR *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        mbObjAlphaSet(objP->objId, alpha);
    }
}

void mbCapObjColorMtxSet(int objId, Mtx mtx)
{
    CAPSULE_OBJCOLOR *objP;
    int no;
    if((no = CapObjColorNoGet(objId)) != -1) {
        objP = &capsuleObjColorData[no];
        mbObjMtxSet(objP->objId, mtx);
    }
}

void mbCapObjColorMtxGet(int objId, Mtx mtx)
{
    CAPSULE_OBJCOLOR *objP;
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
    work = HuMemDirectMallocNum(HEAP_HEAP, 10*sizeof(CAPGUIDE_WORK), HU_MEMNUM_OVL);
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
                    work->scale = 1.0+(0.2f*HuSin(work->angle));
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

static void CapEffMasuOkAddAll(int playerNo, s16 *masuList)
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
    work = obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(CAPEFFREMOVE_WORK), HU_MEMNUM_OVL);
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

static void CapEffRemoveAddAll(HuVecF *pos)
{
    int i;
    for(i=0; i<32; i++) {
        HuVecF effPos;
        HuVecF effVel;
        GXColor effColor;
        float angle;
        float dist;
        angle = i*11.25f;
        dist = (CapRandF()*100)*0.33f;
        effPos.x = pos->x+(dist*HuCos(angle));
        effPos.y = pos->y+(dist*HuSin(angle))+100;
        effPos.z = pos->z+50;
        dist = ((CapRandF()*0.04f)+0.005f)*100;
        effVel.x = dist*HuCos(angle);
        effVel.y = dist*HuSin(angle);
        effVel.z = 0;
        dist = CapRandF();
        effColor.r = 192+(32*dist);
        effColor.g = 192+(32*dist);
        effColor.b = 192+(32*dist);
        effColor.a = (CapRandF()*63)+192;
        CapEffRemoveAdd(effPos, effVel, ((CapRandF()*0.5f)+1)*100, (CapRandF()*0.25f)+0.12f, ((CapRandF()*0.5f)+0.5f)*100, (CapRandF()*0.66f)+0.33f, effColor);
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
    s16 mdlId;
    int i;
    obj = capEffHiliteOMObj = omAddObjEx(mbObjMan, 32768, 0, 0, -1, CapEffHiliteOMExec);
    work = obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(CAPEFFHILITE_WORK), HU_MEMNUM_OVL);
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
/* @@END@@ */
