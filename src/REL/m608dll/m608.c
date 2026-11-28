/* @@HEAD@@ */
#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/seqman.h"
#include "game/wipe.h"
#include "humath.h"
#include "math.h"
#include "dolphin.h"

#include "datadir_enum.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

typedef struct M608Work_s {
    OMOBJMAN *objman; /* 0x000 */
    s32 mode; /* 0x004 */
    s32 time; /* 0x008 */
    s32 playerNo; /* 0x00C: the player whose turn it is */
    s16 unk10[10]; /* 0x010 */
    s16 unk24[10]; /* 0x024 */
    HU3D_MODELID camModel; /* 0x038 */
    s16 unk3A; /* 0x03A */
    OMOBJ *unk3C; /* 0x03C */
    float camTime; /* 0x040 */
    float camMaxTime; /* 0x044 */
    s16 unk48; /* 0x048 */
    s16 unk4A; /* 0x04A */
    s16 unk4C; /* 0x04C */
    s16 unk4E; /* 0x04E */
    s16 unk50; /* 0x050 */
    s16 unk52; /* 0x052 */
    HU3D_MODELID stModel[4]; /* 0x054: "p1st" */
    HU3D_MODELID lsModel[4]; /* 0x05C: "p1ls" */
    HU3D_MODELID lsBModel[4]; /* 0x064: "p1lsB" */
    HU3D_MODELID kuruModel; /* 0x06C: "608kurukuru" */
    HU3D_MOTIONID kuruMot; /* 0x06E */
    u8 unk70[0x90]; /* 0x070 */
    s16 charNo[4]; /* 0x100 */
    s16 padNo[4]; /* 0x108 */
    HU3D_MODELID charModel[4]; /* 0x110 */
    HU3D_MOTIONID charMot[4][8]; /* 0x118 */
    OMOBJ *charObj[4]; /* 0x158 */
    HU3D_MODELID boardModel[4]; /* 0x168 */
    HU3D_MODELID unk170; /* 0x170 */
    HU3D_MODELID mdlId[15]; /* 0x172 */
    HU3D_MOTIONID mtnId[12]; /* 0x190 */
    HU3D_MODELID unk1A8; /* 0x1A8 */
    HU3D_MODELID unk1AA; /* 0x1AA */
    s32 jumpState; /* 0x1AC */
    float rotY; /* 0x1B0 */
    float rotYStart; /* 0x1B4 */
    float rotYEnd; /* 0x1B8 */
    s32 rotTime; /* 0x1BC */
    HU3D_MODELID unk1C0[27]; /* 0x1C0 */
    HU3D_MODELID unk1F6[27]; /* 0x1F6 */
    HuVecF unk22C[27]; /* 0x22C */
    OMOBJ *unk370[27]; /* 0x370 */
    s32 unk3DC; /* 0x3DC */
    s32 score[4]; /* 0x3E0 */
    s32 record; /* 0x3F0 */
    s32 winF[4]; /* 0x3F4 */
    s32 newRecordF; /* 0x404 */
    s16 unk408[4]; /* 0x408 */
    s16 unk410[4]; /* 0x410 */
    s16 unk418[4]; /* 0x418 */
    s16 unk420[4]; /* 0x420 */
    HuVecF unk428; /* 0x428 */
    HuVecF unk434; /* 0x434 */
    HuVecF unk440; /* 0x440 */
    s32 unk44C; /* 0x44C */
    s32 seNo; /* 0x450 */
    s32 streamNo; /* 0x454 */
} M608_WORK; /* size 0x458 */

typedef struct M608Work3DF0_s {
    s16 unk0;
    s16 unk2;
    s32 unk4;
    s32 unk8;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    s32 unk14;
} M608_WORK3DF0; /* size 0x18 */

typedef struct M608Work3E68_s {
    s16 unk0;
    s16 unk2;
    s32 unk4[5];
} M608_WORK3E68; /* size 0x18 */

/* .bss is laid out in reverse definition order */
M608_WORK lbl_1_bss_3E80;
M608_WORK3E68 lbl_1_bss_3E68;
M608_WORK3DF0 lbl_1_bss_3DF0[5];
HuVecF lbl_1_bss_10[1320];
s32 lbl_1_bss_C;
HU3D_MODELID lbl_1_bss_8;
float lbl_1_bss_4;
float lbl_1_bss_0;

void ObjectSetup(void);
s32 fn_1_A0(s32 streamNo, s32 bgmNo);
void fn_1_104(s32 streamNo);
void fn_1_140(s16 mode, s16 frameNo);
void fn_1_190(s16 mode, s16 frameNo);
void fn_1_57C(s16 mode, s16 frameNo);
void fn_1_580(s16 mode, s16 frameNo);
void fn_1_1A2C(s16 mode, s16 frameNo);
void fn_1_1DD8(s16 mode, s16 frameNo);
void fn_1_2374(s16 mode, s16 frameNo);
void fn_1_2538(s16 mode, s16 frameNo);
void fn_1_253C(s16 mode, s16 frameNo);
void fn_1_2540(void);
void fn_1_2FD8(HU3D_MODELID modelId, HU3D_MOTIONID motId, BOOL lagF);
void fn_1_33A4(OMOBJ *obj);
void fn_1_34D0(OMOBJ *obj);
void fn_1_3718(void);
void fn_1_388C(OMOBJ *obj);
void fn_1_38E8(HU3D_MODEL *modelP, Mtx *mtx);
void fn_1_38EC(HuVecF pos);
void fn_1_38F0(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr);
void fn_1_39B4(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr);
void fn_1_3A78(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr);
void fn_1_3B3C(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr);
void fn_1_3C00(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr);
void fn_1_3C84(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr);
void fn_1_3D7C(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr);
void fn_1_3E00(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr);
void fn_1_3F18(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr);
void fn_1_400C(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr);
void fn_1_40F0(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr);
void fn_1_41D4(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr);
void fn_1_42B8(void);
void fn_1_437C(s16 layerNo);
void fn_1_5DA4(HuVecF pos);
void fn_1_5E6C(s32 no);
BOOL fn_1_5F64(void);
void fn_1_5F90(s16 no);
void fn_1_5FDC(s16 no, s16 motNo);
void fn_1_605C(s16 no);
void fn_1_6148(void);
void fn_1_614C(void);
void fn_1_6534(s16 playerNo, s16 score);
void fn_1_6578(void);
void fn_1_66C0(OMOBJ *obj);
void fn_1_6958(s32 no);
s32 fn_1_6B90(s32 playerNo);

/* Defined by part 2 at the end of PART2, after fn_1_6B90's data (.data 0x58C-0x694, a later
   file in retail), together with their strings */
extern char *lbl_1_data_5D0[4];
extern char *lbl_1_data_624[4];
extern char *lbl_1_data_64C[4];
extern char *lbl_1_data_684[4];

MGSEQ_PARAM lbl_1_data_0 = {
    0,
    1,
    fn_1_140,
    fn_1_190,
    fn_1_57C,
    fn_1_580,
    fn_1_1A2C,
    fn_1_1DD8,
    fn_1_2374,
    fn_1_2538,
    fn_1_253C,
};
float lbl_1_data_28[14] = { -35.0f, -35.0f, -35.0f, -25.0f, -35.0f, -35.0f, -35.0f, -5.0f, -35.0f, -10.0f, -5.0f, -10.0f, -10.0f, -10.0f };
float lbl_1_data_60[14] = { -10.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
float lbl_1_data_98[14] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.4f, 1.0f, 1.0f, 0.4f, 1.0f, 1.0f, 1.0f };

/* @@PART1@@ */
int _prolog(void)
{
    const VoidFunc *ctors = _ctors;
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtors = _dtors;
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

s32 fn_1_A0(s32 streamNo, s32 bgmNo)
{
    s32 result = streamNo;
    if (result == -1 && (GameMesStatGet(MgSeqGameMesIdGet()) & GAMEMES_STAT_FXPLAY)) {
        result = HuAudBGMPlay(bgmNo);
    }
    return result;
}

void fn_1_104(s32 streamNo)
{
    if (streamNo != -1) {
        HuAudSStreamFadeOut(streamNo, 100);
    }
}

void fn_1_140(s16 mode, s16 frameNo)
{
    lbl_1_bss_0 = 0.0f;
    lbl_1_bss_4 = 0.0f;
    MgSeqModeNext();
}

void fn_1_190(s16 mode, s16 frameNo)
{
    s32 i;
    s32 charNo;
    char *hookName;
    HuVecF pos;

    if (frameNo == 0) {
        fn_1_5E6C(0);
        for (i = 0; i < 4; i++) {
            charNo = lbl_1_bss_3E80.charNo[i];
            CharMotionSet(charNo, lbl_1_bss_3E80.charMot[i][0]);
            Hu3DModelAttrSet(lbl_1_bss_3E80.charModel[i], HU3D_MOTATTR_LOOP);
            Hu3DModelHookSet(lbl_1_bss_3E80.stModel[i], lbl_1_data_5D0[i], lbl_1_bss_3E80.charModel[i]);
            hookName = CharModelItemHookGet(lbl_1_bss_3E80.charNo[i], 4, 2);
            Hu3DModelHookSet(lbl_1_bss_3E80.charModel[i], hookName, lbl_1_bss_3E80.boardModel[i]);
        }
        Hu3DObjHookSet(lbl_1_bss_3E80.charModel[0], "ske_head", fn_1_38F0);
        Hu3DObjHookSet(lbl_1_bss_3E80.charModel[1], "ske_head", fn_1_39B4);
        Hu3DObjHookSet(lbl_1_bss_3E80.charModel[2], "ske_head", fn_1_3A78);
        Hu3DObjHookSet(lbl_1_bss_3E80.charModel[3], "ske_head", fn_1_3B3C);
        OSReport("1P neck ... %f\n", lbl_1_data_28[lbl_1_bss_3E80.charNo[0]]);
        OSReport("2P neck ... %f\n", lbl_1_data_28[lbl_1_bss_3E80.charNo[1]]);
        OSReport("3P neck ... %f\n", lbl_1_data_28[lbl_1_bss_3E80.charNo[2]]);
        OSReport("4P neck ... %f\n", lbl_1_data_28[lbl_1_bss_3E80.charNo[3]]);
        Hu3DObjHookSet(lbl_1_bss_3E80.boardModel[0], "snowboard_txt", fn_1_3F18);
        Hu3DObjHookSet(lbl_1_bss_3E80.boardModel[1], "snowboard_txt", fn_1_400C);
        Hu3DObjHookSet(lbl_1_bss_3E80.boardModel[2], "snowboard_txt", fn_1_40F0);
        Hu3DObjHookSet(lbl_1_bss_3E80.boardModel[3], "snowboard_txt", fn_1_41D4);
        fn_1_5FDC(3, 0);
        fn_1_5FDC(4, 2);
        if (lbl_1_bss_3E80.unk44C == 0) {
            fn_1_5FDC(5, 4);
        }
        fn_1_5FDC(10, 8);
        Hu3DModelObjPosGet(lbl_1_bss_3E80.stModel[1], "p2st", &pos);
        fn_1_5DA4(pos);
        lbl_1_bss_3E80.streamNo = HuAudSStreamPlay(79);
    } else if (fn_1_5F64()) {
        fn_1_614C();
        lbl_1_bss_3E80.playerNo = 0;
        lbl_1_bss_3E80.mode = 0;
        lbl_1_bss_3E80.time = 0;
        MgSeqModeNext();
    }
}

void fn_1_57C(s16 mode, s16 frameNo)
{
}

/* @@PART2@@ */
/* Part 1's scoring stubs: the splice replaces this section with part 2's functions */
void fn_1_3718(void) {}
void fn_1_388C(OMOBJ *obj) {}
void fn_1_38E8(HU3D_MODEL *modelP, Mtx *mtx) {}
void fn_1_38EC(HuVecF pos) {}
void fn_1_38F0(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr) {}
void fn_1_39B4(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr) {}
void fn_1_3A78(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr) {}
void fn_1_3B3C(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr) {}
void fn_1_3C00(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr) {}
void fn_1_3C84(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr) {}
void fn_1_3D7C(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr) {}
void fn_1_3E00(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr) {}
void fn_1_3F18(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr) {}
void fn_1_400C(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr) {}
void fn_1_40F0(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr) {}
void fn_1_41D4(HSF_OBJECT *obj, HSF_TRANSFORM *transform, Mtx *prev, Mtx *curr) {}
void fn_1_42B8(void) {}
void fn_1_437C(s16 layerNo) {}
void ObjectSetup(void) {}
void fn_1_5DA4(HuVecF pos) {}
void fn_1_5E6C(s32 no) {}
BOOL fn_1_5F64(void) { return 0; }
void fn_1_5F90(s16 no) {}
void fn_1_5FDC(s16 no, s16 motNo) {}
void fn_1_605C(s16 no) {}
void fn_1_6148(void) {}
void fn_1_614C(void) {}
void fn_1_6534(s16 playerNo, s16 score) {}
void fn_1_6578(void) {}
void fn_1_66C0(OMOBJ *obj) {}
void fn_1_6958(s32 no) {}
s32 fn_1_6B90(s32 playerNo) { return 0; }
char *lbl_1_data_5D0[4] = { "608kurukuru-p1st", "608kurukuru-p2st", "608kurukuru-p3st", "608kurukuru-p4st" };
char *lbl_1_data_624[4] = { "608kurukuru-p1ls", "608kurukuru-p2ls", "608kurukuru-p3ls", "608kurukuru-p4ls" };
char *lbl_1_data_64C[4] = { "p1lsB", "p2lsB", "p3lsB", "p4lsB" };
char *lbl_1_data_684[4] = { "p1lsboard", "p2lsboard", "p3lsboard", "p4lsboard" };
/* @@END@@ */
