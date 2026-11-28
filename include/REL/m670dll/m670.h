#ifndef _REL_M670DLL_M670_H
#define _REL_M670DLL_M670_H

#include "game/actman.h"
#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mgtimer.h"
#include "game/mic.h"
#include "game/object.h"
#include "game/process.h"
#include "game/seqman.h"
#include "game/wipe.h"
#include "string.h"
#include "dolphin.h"

#include "datadir_enum.h"

#define M670_PILLAR_NUM 24

/* no header declares these */
void HuMCListenerCreate(HUMC_CONTEXTID ctxId, HUMC_CONTEXT_CALLBACK cb, u8 flag);
void HuMCListenerKill(void);

typedef struct M670Com_s {
    s32 playerNo; /* 0x00 */
    MGPLAYER *player; /* 0x04 */
    s32 mode; /* 0x08 */
    s32 timer; /* 0x0C */
    s32 targetNo; /* 0x10 */
    HuVecF targetPos; /* 0x14 */
    s32 jumpTime; /* 0x20 */
} M670_COM; /* size 0x24 */

typedef struct M670Work_s {
    OMOBJMAN *objman; /* 0x000 */
    u8 unk4[8]; /* 0x004 */
    HU3D_MODELID camModel[3]; /* 0x00C */
    HU3D_MOTIONID camMot[3]; /* 0x012 */
    HU3D_MODELID stageModel[3]; /* 0x018 */
    HU3D_MODELID pillarModel[M670_PILLAR_NUM]; /* 0x01E */
    HU3D_MOTIONID pillarMot[M670_PILLAR_NUM][4]; /* 0x04E */
    HU3D_MODELID colModel[M670_PILLAR_NUM]; /* 0x10E */
    HuVecF pillarPos[M670_PILLAR_NUM]; /* 0x140 */
    s32 colNum; /* 0x260 */
    HU3D_MODELID colMapModel[M670_PILLAR_NUM + 1]; /* 0x264 */
    HU3D_MODELID posModel; /* 0x296 */
    HU3D_MODELID resultPosModel; /* 0x298 */
    s16 charNo[4]; /* 0x29A */
    s16 padNo[4]; /* 0x2A2 */
    MGPLAYER *player[4]; /* 0x2AC */
    s32 teamF[4]; /* 0x2BC */
    s32 shout; /* 0x2CC */
    s32 round; /* 0x2D0 */
    s16 soloNo; /* 0x2D4 */
    s16 soloPadNo; /* 0x2D6 */
    s16 soloCharNo; /* 0x2D8 */
    s32 stageMode; /* 0x2DC */
    s32 playerMode[4]; /* 0x2E0 */
    OMOBJ *playerObj[4]; /* 0x2F0 */
    OMOBJ *stageObj; /* 0x300 */
    s32 aliveMask; /* 0x304 */
    M670_COM com[4]; /* 0x308 */
    u16 micCtx; /* 0x398 */
    MGTIMER *timer; /* 0x39C */
    s32 pattern; /* 0x3A0 */
    s32 streamNo; /* 0x3A4 */
} M670_WORK; /* size 0x3A8 */

extern M670_WORK lbl_1_bss_10;
extern MGSEQ_PARAM lbl_1_data_0;
extern s8 *lbl_1_data_250[6];

void fn_1_A0(HUMC_RESPONSE *resp);
void fn_1_1F8(s16 mode, s16 frameNo);
void fn_1_218(s16 mode, s16 frameNo);
s32 fn_1_2AC(s32 streamNo, s32 bgmNo);
void fn_1_310(s32 streamNo);
void fn_1_34C(void);
void fn_1_42C(s16 mode, s16 frameNo);
void fn_1_5AC(s16 mode, s16 frameNo);
void fn_1_B4C(s16 mode, s16 frameNo);
void fn_1_C68(s16 mode, s16 frameNo);
void fn_1_12C4(s16 mode, s16 frameNo);
void fn_1_1428(s16 mode, s16 frameNo);
void fn_1_142C(s16 mode, s16 frameNo);
BOOL fn_1_1460(HuVecF *src, HuVecF *dst);
void fn_1_15B8(s32 seId, HuVecF *pos);
void ObjectSetup(void);
void fn_1_22F0(s32 no, float y);
void fn_1_2384(MGACTOR *actorP, int param);
void fn_1_2460(s32 playerNo, s32 mode);
void fn_1_2690(OMOBJ *obj);
void fn_1_28E8(OMOBJ *obj);
void fn_1_2A24(s32 mode);
s32 fn_1_2DE8(HuVecF *pos);
float fn_1_2EF4(s32 no);
BOOL fn_1_2F44(HuVecF *pos);
void fn_1_3098(OMOBJ *obj);
void fn_1_3B30(void);
void fn_1_3BDC(void);

#endif
