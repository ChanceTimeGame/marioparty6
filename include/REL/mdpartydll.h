#ifndef _REL_MDPARTYDLL_H
#define _REL_MDPARTYDLL_H

#include "game/hu3d.h"
#include "game/window.h"
#include "game/sprite.h"
#include "game/object.h"
#include "game/process.h"
#include "game/animdata.h"

/* lbl_1_data_C0: sprite creation table (layout as MDSEL_SPRDATA) */
typedef struct MdPartySprData_s {
    s16 grpNo; //0x00
    s16 memberNo; //0x02
    s16 animNo; //0x04
    s16 prio; //0x06
    s16 bank; //0x08
    float posX; //0x0C
    float posY; //0x10
    float scaleX; //0x14
    float scaleY; //0x18
    float zRot; //0x1C
} MDPARTY_SPRDATA; //size 0x20

/* lbl_1_bss_9BC: per-player setup */
typedef struct MdPartyPlayer_s {
    s16 state; //0x00
    s16 unk2; //0x02
    s16 com; //0x04
    s16 comDif; //0x06
    s16 charNo; //0x08
    s16 padNo; //0x0A
    s16 handicap; //0x0C
} MDPARTY_PLAYER; //size 0xE

/* lbl_1_bss_9F4: game rules */
typedef struct MdPartyRule_s {
    s16 tagF; //0x00
    s16 playerNum; //0x02 human players
    s16 boardNo; //0x04
    s16 turnNo; //0x06 turnMax = turnNo * 5 + 10
    s16 mgPack; //0x08
    s16 bonusStarF; //0x0A
    s16 handicapCursor; //0x0C (part 8)
    s16 teamNo; //0x0E row of lbl_1_data_4
    u32 mesNo[2]; //0x10
    s16 unk18; //0x18
    s16 unk1A; //0x1A
} MDPARTY_RULE; //size 0x1C

typedef struct MdPartyCamera_s MDPARTY_CAMERA;
typedef void (*MDPARTY_CAMERA_FUNC)(OMOBJ *obj, MDPARTY_CAMERA *camera);

/* lbl_1_bss_A10 */
struct MdPartyCamera_s {
    OMOBJ *obj; //0x00
    HuVecF center; //0x04
    HuVecF centerEnd; //0x10
    HuVecF rot; //0x1C
    HuVecF rotEnd; //0x28
    float zoom; //0x34
    float zoomEnd; //0x38
    MDPARTY_CAMERA_FUNC func; //0x3C
    float time; //0x40 (part 14)
    u8 unk44[0xC]; //0x44
}; //size 0x50

/* layouts not known yet: owners of the functions that use them report fields */
/* lbl_1_bss_108, _1C8, _228: quadratic Bezier paths (parts 2 and 3) */
typedef struct MdPartyPath_s {
    s16 mode; //0x00
    HuVecF pos[3]; //0x04 start, mid, end
    float time; //0x28
    float maxTime; //0x2C
} MDPARTY_PATH; //size 0x30

/* lbl_1_data_E12 (part 6) */
typedef struct MdPartyCharSlot_s {
    s16 link[8]; //0x00
    s16 owner; //0x10
} MDPARTY_CHARSLOT; //size 0x12
/* lbl_1_bss_288: models (parts 4, 6, 7, 9, 10) */
typedef struct MdPartyModel_s {
    HU3D_MODELID mdlId; //0x00
    HU3D_ANIMID animId[4]; //0x02
    s16 unkA; //0x0A
    HuVecF pos; //0x0C
    HuVecF rot; //0x18
    HuVecF scale; //0x24
    s16 time; //0x30
    s16 unk32[3]; //0x32
} MDPARTY_MODEL; //size 0x38
typedef struct MdPartyWorkA96_s {
    s16 unk0[5]; //0x00
} MDPARTY_WORKA96; //size 0xA

/* lbl_1_bss_AF0 (parts 18, 19) */
typedef struct MdPartyWave_s {
    float indMtx[2][3]; //0x00
    GXColor color; //0x18
    float scroll; //0x1C
    float scrollSpeed; //0x20
    float time; //0x24
} MDPARTY_WAVE; //size 0x28

/* .bss (definition order is the reverse of memory order) */
extern ANIMDATA *lbl_1_bss_B40[9];
extern MDPARTY_WAVE lbl_1_bss_AF0[2];
extern s16 lbl_1_bss_AEC;
extern s16 lbl_1_bss_AE4[4];
extern s16 lbl_1_bss_AE2;
extern s16 lbl_1_bss_ADA[4];
extern s16 lbl_1_bss_AD6[2];
extern s16 lbl_1_bss_AD2[2];
extern MDPARTY_WORKA96 lbl_1_bss_A96[6];
extern s16 lbl_1_bss_A94;
extern void *lbl_1_bss_A90;
extern ANIMDATA *lbl_1_bss_A8C;
extern ANIMDATA *lbl_1_bss_A88;
extern OMOBJ *lbl_1_bss_A84;
extern OMOBJMAN *lbl_1_bss_A80;
extern s32 lbl_1_bss_A6C[5];
extern HU3D_LIGHTID lbl_1_bss_A68[2];
extern HUWINID lbl_1_bss_A60[4];
extern MDPARTY_CAMERA lbl_1_bss_A10;
extern MDPARTY_RULE lbl_1_bss_9F4;
extern MDPARTY_PLAYER lbl_1_bss_9BC[4];
extern ANIMDATA *lbl_1_bss_93C[32];
extern HUSPR_GROUPID lbl_1_bss_918[18];
extern HUSPRID lbl_1_bss_8A4[58];
extern ANIMDATA *lbl_1_bss_800[41];
extern MDPARTY_MODEL lbl_1_bss_288[25];
extern MDPARTY_PATH lbl_1_bss_228[2];
extern MDPARTY_PATH lbl_1_bss_1C8[2];
extern MDPARTY_PATH lbl_1_bss_108[4];
extern HUWINID lbl_1_bss_104[2];
extern HuVecF lbl_1_bss_D4[4];
extern HuVecF lbl_1_bss_A4[4];
extern HuVecF lbl_1_bss_8C[2];
extern float lbl_1_bss_80[3];
extern HuVecF lbl_1_bss_50[4];
extern float lbl_1_bss_38[6];
extern s16 lbl_1_bss_34;
extern OMOBJ *lbl_1_bss_30;
extern s16 lbl_1_bss_2E;
extern s16 lbl_1_bss_2C;
extern s16 lbl_1_bss_2A;
extern s16 lbl_1_bss_28;
extern OMOBJ *lbl_1_bss_24;
extern OMOBJ *lbl_1_bss_1C[2];
extern OMOBJ *lbl_1_bss_18;
extern OMOBJ *lbl_1_bss_14;
extern OMOBJ *lbl_1_bss_10;
extern OMOBJ *lbl_1_bss_C;
extern OMOBJ *lbl_1_bss_8;
extern OMOBJ *lbl_1_bss_4;
extern OMOBJMAN *lbl_1_bss_0;

/* .data, HEAD (before PART1) */
extern s32 lbl_1_data_0;
extern s16 lbl_1_data_4[3][4];
extern s32 lbl_1_data_1C[32];
extern s16 lbl_1_data_9C[18];
extern MDPARTY_SPRDATA lbl_1_data_C0[58];
extern s32 lbl_1_data_800[41];
extern HuVecF lbl_1_data_8A4[67];
extern s32 lbl_1_data_BC8;
/* .data, end of PART1 (after the strings of MdPartyWinCallback) */
extern s16 lbl_1_data_BEE[7];
extern s32 lbl_1_data_BFC[14];
/* .data, end of PART1 (after the strings of MdPartyCameraDebug) */
extern s16 lbl_1_data_DA2[3];
extern s32 lbl_1_data_DA8[3];
/* .data defined by later parts (see below) */
extern s16 lbl_1_data_DE6[6];
extern MDPARTY_CHARSLOT lbl_1_data_E12[11];
extern s32 lbl_1_data_F10[9];
/* .data defined by later parts, in this order between their string literals:
   lbl_1_data_DE6 (s16[6], after the strings of fn_1_9218, before fn_1_A970),
   lbl_1_data_E12 (MDPARTY_CHARSLOT[11], after the string table of fn_1_D748, before fn_1_12364),
   lbl_1_data_F10 (s32[9], after the strings of fn_1_2DD58, before fn_1_44DCC) */

/* every global function of mdparty.c, in reference order (signatures from the definitions) */
void MdPartyWinCallback(HUWINID winId, u32 mess, s16 no);
void MdPartyUnlockInit(void);
void MdPartyCharDataLoad(void);
void MdPartyReadProc(void);
void MdPartyReadInit(void);
void MdPartyGameWorkSet(void);
void MdPartyGameWorkTest(void);
void MdPartyOvlGoto(s16 mode);
void MdPartyARamFree(void);
void MdPartyARamFreeReturn(void);
float MdPartyBezier(float a, float b, float c, float t);
void MdPartyBezierV(HuVecF *out, float t, HuVecF *a, HuVecF *b, HuVecF *c);
float MdPartySmooth(float cur, float target, float speed);
void MdPartySmoothV(HuVecF *cur, HuVecF *target, float speed);
float MdPartySinLerp90(float start, float end, float time, float maxTime);
float MdPartyLerp(float start, float end, float time, float maxTime);
void MdPartyModelMoveLook(HU3D_MODELID mdlId, HuVecF *start, HuVecF *end, float time, float maxTime);
void MdPartyModelRotTurn(HU3D_MODELID mdlId, float rotY);
void MdPartyModelMoveTurn(HU3D_MODELID mdlId, HuVecF *start, HuVecF *end, float time, float maxTime, float rotY);
float MdPartySinLerp180(float start, float end, float time, float maxTime);
void MdPartyModelBezierMove(HU3D_MODELID mdlId, HuVecF *a, HuVecF *b, HuVecF *c, float time, float maxTime);
void MdPartyPlayerInit(void);
void MdPartyPlayerComSet(void);
void MdPartyCameraJump(MDPARTY_CAMERA *camera);
void MdPartyCameraStop(MDPARTY_CAMERA *camera);
void MdPartyCameraSmooth(MDPARTY_CAMERA *camera, float speed);
void MdPartyCameraFuncSet(MDPARTY_CAMERA_FUNC func);
void MdPartyCameraDebug(OMOBJ *obj, MDPARTY_CAMERA *camera);
void MdPartyCameraFuncCall(OMOBJ *obj, MDPARTY_CAMERA *camera);
void MdPartyCameraUpdate(OMOBJ *obj);
void MdPartyCameraInit(MDPARTY_CAMERA_FUNC func);
void MdPartyCameraKill(void);
void MdPartyLightInit(void);
void MdPartyLightKill(void);
void MdPartyWinOpen(s16 winNo);
void MdPartyWinClose(s16 winNo);
void MdPartyWinWait(s16 winNo);
s16 MdPartyWinChoiceGet(s16 winNo, s16 mode);
void MdPartyWinMesSet(s16 winNo, s32 mess, s16 speed);
void MdPartyWinInsertMesSet(s16 winNo, s32 mess, s16 insertNo);
void MdPartyWinInit(void);
void MdPartyWinKill(void);
void MdPartyMainWinOpen(s16 winNo);
void MdPartyMainWinClose(void);
void MdPartyMainWinWait(void);
s16 MdPartyMainWinChoiceGet(s16 mode);
void MdPartyMainWinMesSet(s16 winNo, s32 mess, s16 speed);
void MdPartyMainWinInsertMesSet(s16 winNo, s32 mess, s16 insertNo);
void MdPartyHelpWinMesSet(s32 mess);
void MdPartyHelpWinClose(void);
void MdPartyShadowInit(void);
void MdPartyShadowKill(void);
void MdPartySprGrpAttrSet(HUSPR_GROUPID grpId, u32 attr);
void MdPartySprGrpAttrReset(HUSPR_GROUPID grpId, u32 attr);
void MdPartySprInit(void);
void MdPartySprKill(void);
void MdPartyHostItemThrowUpdate(OMOBJ *obj);
void MdPartyHostItemHoldUpdate(OMOBJ *obj);
void MdPartyHostItemThrowSet(void);
void MdPartyHostAReactUpdate(OMOBJ *obj);
void MdPartyHostAReactSet(void);
void MdPartyHostBReactUpdate(OMOBJ *obj);
void MdPartyHostBReactSet(void);
void MdPartyHostAFlyUpdate(OMOBJ *obj);
void MdPartyHostAFlySet(void);
void MdPartyHostAWalkUpdate(OMOBJ *obj);
s32 MdPartyHostAWalkSet(void);
void MdPartyHostACreate(OMOBJ *obj);
void MdPartyHostAKill(OMOBJ *obj);
void MdPartyHostBFlyUpdate(OMOBJ *obj);
void MdPartyHostBFlySet(void);
void MdPartyHostBWalkUpdate(OMOBJ *obj);
s32 MdPartyHostBWalkSet(void);
void MdPartyHostBCreate(OMOBJ *obj);
void MdPartyHostKill(OMOBJ *obj);
void MdPartyPlayerJumpUpdate(OMOBJ *obj);
void MdPartyPlayerJumpStart(void);
void MdPartyPlayerScaleUpdate(OMOBJ *obj);
void MdPartyPlayerAppear(void);
void MdPartyCharMdlCreate(OMOBJ *obj);
void MdPartyCharMdlKill(OMOBJ *obj);
void MdPartyTurnNumSet(s16 turn, s16 turnMax);
void MdPartyTurnNumHide(void);
void MdPartyTitleFadeOut(OMOBJ *obj);
void MdPartyTitleShow(void);
void MdPartyTitleFadeOutStart(void);
void MdPartyStageCreate(OMOBJ *obj);
void MdPartyStageKill(OMOBJ *obj);
void MdPartyDescFadeIn(s16 memberNo);
void MdPartyDescFadeOutStart(void);
void MdPartyDescInit(void);
void MdPartyDescUpdate(OMOBJ *obj);
void MdPartyPicSet(s16 picNo);
void MdPartyPicFadeOut(void);
void MdPartyPicUpdate(OMOBJ *obj);
void MdPartyPicCreate(OMOBJ *obj);
void MdPartyPicKill(OMOBJ *obj);
u32 MdPartyPairMesGet(s16 a, s16 b);
void MdPartyInfoWinMesSet(s16 winNo, u32 mess);
void MdPartyInfoWinOpen(u32 mess0, u32 mess1);
void MdPartyInfoWinClose(void);
void MdPartyInfoWinCreate(void);
void MdPartyInfoWinKill(void);
void MdPartyIconAOn(s16 playerNo, s16 bank);
void MdPartyIconAOff(s16 playerNo);
void MdPartyIconABankSet(s16 playerNo, s16 bank);
void MdPartyIconBOn(s16 playerNo, s16 bank);
void MdPartyIconBOff(s16 playerNo);
void MdPartyIconBBankSet(s16 playerNo, s16 bank);
void MdPartyIconCOn(s16 teamNo, s16 bank);
void MdPartyIconCOff(s16 teamNo);
void MdPartyIconCBankSet(s16 teamNo, s16 bank);
void MdPartyIconDOn(void);
void MdPartyIconDOff(void);
void MdPartyIconAllUpdate(OMOBJ *obj);
void MdPartyIconInit(OMOBJ *obj);
void MdPartyInfoWinKill2(void);
void MdPartyMarkPosSet(s16 memberNo, HuVecF *pos, float ofsX, float ofsY);
void MdPartyIconOpen(s16 no, HuVecF *pos, float ofsX, float ofsY);
void MdPartyIconClose(s16 no);
void MdPartyIconPush(s16 no);
void MdPartyIconUpdate(void);
void MdPartyIconCloseAll(void);
void MdPartyMarkerPosSet(s16 no, HuVecF *pos);
void MdPartyMarkerShow(int no, HuVecF *pos, s16 bank);
void MdPartyMarkerHide(int no);
void MdPartyMarkerUpdate(OMOBJ *obj);
void MdPartyMarkerObjInit(OMOBJ *obj);
void MdPartyMarkerObjKill(OMOBJ *obj);
s16 MdPartyCursorFind(s16 cur, s16 dir1, s16 dir2, s16 dir3, s16 (*link)[9], s16 num);
s16 MdPartyCursorDirFind(s16 cur, s16 (*link)[9], s16 dir, s16 num);
s16 MdPartyCursorMove(s16 padNo, s16 *cur, s16 (*link)[9], s16 num);
void MdPartyPanelAnimSet(s16 mdlNo, s16 animSlot, s16 animDataNo, s16 bank);
void MdPartyPanelAnimSetAll(s16 mdlNo, s16 animDataNo);
void MdPartyPanelSpinIn(s16 mdlNo, s16 time, s16 maxTime, HuVecF *pos, float scale);
void MdPartyPanelSpinOut(s16 mdlNo, s16 time, s16 maxTime);
void MdPartyPanelMove(s16 mdlNo, s16 time, s16 maxTime, HuVecF *end, float scaleEnd);
void MdPartyPanelFlip(s16 mdlNo, s16 animBase, s16 animNo, s16 time, s16 maxTime, s16 turnNum);
void MdPartyPanelRotReset(s16 mdlNo, float rotY, s16 time, s16 maxTime);
void MdPartyPanelStatSet(s16 stat);
s16 MdPartyPanelStatGet(void);
s16 MdPartyPanelTimeCheck(void);
void MdPartyPanelFuncStart(OMOBJ_FUNC func);
void MdPartyPanelInit(void);
void MdPartyPanelKill(void);
void MdPartyPanelIntro(OMOBJ *obj);
void MdPartyGroupBCloseExec(OMOBJ *obj);
void MdPartyGroupBCloseResetExec(OMOBJ *obj);
void MdPartyPlayerNumSelExec(OMOBJ *obj);
void MdPartyPlayerNumSelStart(OMOBJ *obj);
void MdPartyPlayerNumSelEnd(OMOBJ *obj);
void MdPartyPlayerNumOpenExec(OMOBJ *obj);
void MdPartyComModelOut(OMOBJ *obj);
void MdPartyCharModelSpin(s16 charNo);
void MdPartyCharModelRotReset(s16 charNo);
void MdPartyCharSelMain(OMOBJ *obj);
void MdPartyCharSelInit(OMOBJ *obj);
void MdPartyCharSelClose(OMOBJ *obj);
void MdPartyCharSelReset(OMOBJ *obj);
void MdPartyComSelMain(OMOBJ *obj);
void MdPartyComSelInit(OMOBJ *obj);
void MdPartyComSelClose(OMOBJ *obj);
void MdPartyComCharSelMain(OMOBJ *obj);
void MdPartyComCharSelInit(OMOBJ *obj);
void MdPartyComCharSelClose(OMOBJ *obj);
void MdPartyCharSelOpen(OMOBJ *obj);
void MdPartyCharSelOutA(OMOBJ *obj);
void MdPartyCharSelOutB(OMOBJ *obj);
void MdPartyPlayerHide(OMOBJ *obj);
void MdPartyCharAllHide(OMOBJ *obj);
void MdPartyBoardFocus(s16 no);
void MdPartyBoardUnfocus(s16 no);
void MdPartyBoardSelUpdate(OMOBJ *obj);
void MdPartyBoardSelStart(OMOBJ *obj);
void MdPartyBoardRotReset(OMOBJ *obj);
void MdPartyBoardAppear(OMOBJ *obj);
void MdPartyBoardSelWait(OMOBJ *obj);
void MdPartyBoardDecide(OMOBJ *obj);
void MdPartyMdlSpinHide(OMOBJ *obj);
void MdPartyHandicapSelect(OMOBJ *obj);
void MdPartyHandicapSelectStart(OMOBJ *obj);
void MdPartyHandicapSelectEnd(OMOBJ *obj);
void MdPartyHandicapMoveIn(OMOBJ *obj);
void MdPartyPlayerMoveIn(OMOBJ *obj);
void MdPartyPlayerMoveBack(OMOBJ *obj);
void MdPartyHandicapMoveOut(OMOBJ *obj);
void MdPartyTeamArrange(OMOBJ *obj);
void MdPartyMdlSpinOut(OMOBJ *obj);
void MdPartyMdlSpinOutAll(OMOBJ *obj);
void MdPartyTeamArrangeBack(OMOBJ *obj);
void MdPartyTeamSelect(OMOBJ *obj);
void MdPartyTeamSelectStart(OMOBJ *obj);
void MdPartyTeamSelectEnd(OMOBJ *obj);
void MdPartyTeamWinOpen(OMOBJ *obj);
void MdPartyTeamWinReset(OMOBJ *obj);
void MdPartyTeamNameSet(OMOBJ *obj);
void MdPartyTeamWinClose(OMOBJ *obj);
void MdPartyTeamPlayerSelect(OMOBJ *obj);
void MdPartyTeamPlayerSelectStart(OMOBJ *obj);
void MdPartyTeamPlayerSelectEnd(OMOBJ *obj);
void MdPartyTeamPlayerReset(OMOBJ *obj);
void MdPartyTeamMove(OMOBJ *obj);
void MdPartyStepEnd(OMOBJ *obj);
void MdPartyTeamComReset(OMOBJ *obj);
void MdPartyTeamMoveBack(OMOBJ *obj);
void MdPartyTeamHandicapSelect(OMOBJ *obj);
void MdPartyTeamHandicapSelectStart(OMOBJ *obj);
void MdPartyTeamHandicapSelectEnd(OMOBJ *obj);
void MdPartyTeamHandicapOpen(OMOBJ *obj);
void MdPartyTeamHandicapReset(OMOBJ *obj);
void MdPartyTeamMoveRule(OMOBJ *obj);
void MdPartyTeamHandicapClose(OMOBJ *obj);
void MdPartyRuleSelect(OMOBJ *obj);
void MdPartyRuleSelectStart(OMOBJ *obj);
void MdPartyRuleSelectEnd(OMOBJ *obj);
void MdPartyRulePanelSpinIn(OMOBJ *obj);
void MdPartyRulePanelMoveIn(OMOBJ *obj);
void MdPartyRulePanelMoveOut(OMOBJ *obj);
void MdPartyRulePanelSpinOut(OMOBJ *obj);
void MdPartyAllKill(void);
void MdPartyExitWait(OMOBJ *obj);
void MdPartyExitCheck(OMOBJ *obj);
void MdPartyMainProc(void);
void MdPartyInit(void);
void MdPartyDataClose(void);
void ObjectSetup(void);
void MdPartyCameraIntroBoard(OMOBJ *obj, MDPARTY_CAMERA *camera);
void MdPartyCameraBoardOut(OMOBJ *obj, MDPARTY_CAMERA *camera);
void MdPartyCameraBoardOutStart(OMOBJ *obj, MDPARTY_CAMERA *camera);
s32 MdPartyStartIntro(void);
s32 MdPartyStartMes(void);
void MdPartyHostWalkIn(void);
s16 MdPartyQuitAsk(void);
s32 MdPartyResume(void);
s16 MdPartyTestStart(void);
s32 MdPartyHostOut(void);
s16 MdPartyPlayerNumSelect(); /* retail callers pass an argument */
s16 MdPartyCharSelStatGet(void);
s16 MdPartyCharSelect(s16 resetF);
s16 MdPartyComSelStatGet(s16 *comNo, s16 *firstComNo, s16 *lastNo);
s16 MdPartyComSelect();
s16 MdPartyPlayerSelect(s16 hideF);
s16 MdPartyComSelStatGetB(s16 *comNo, s16 *firstNo, s16 *lastNo);
s16 MdPartyComSelectB(s16 mode);
s16 MdPartyPlayerSelectB(s16 mode);
s16 MdPartyTeamModeSelect(s16 mode);
void MdPartyTeamComStatGet(s16 *comNo, s16 *firstNo, s16 *lastNo);
s16 MdPartyTeamComSelect(s16 mode);
s16 MdPartyTeamRuleMesShow(void);
s16 MdPartyBoardConfirm(s16 flag);
s16 MdPartyHandicapConfirm(s16 flag);
s16 MdPartyTeamHandicapConfirm(s16 flag);
s16 MdPartyTeamConfirm(void);
void MdPartyBoardGo(void);
s16 MdPartyRuleConfirm(s16 flag);
s16 MdPartyMainSeq(); /* retail callers pass 0 */
float MdPartyWaveSinLerp90(float start, float end, float time, float maxTime);
float MdPartyWaveLerp(float start, float end, float time, float maxTime);
float MdPartyWaveSmooth(float cur, float target, float speed);
void MdPartyWaveCopyHook(s16 layerNo);
void MdPartyWaveMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *material);
void MdPartyWaveUpdate(OMOBJ *obj);
void MdPartyWaveStart(s16 no, HuVecF *pos);
void MdPartyWaveCreate(OMOBJ *obj);
void MdPartyWaveObjKill(OMOBJ *obj);
void MdPartyWaveInit(void);
void MdPartyWaveKill(void);
void MdPartyWaveStartFirst(HuVecF *pos);
void MdPartyBlurHook(s16 layerNo);
void MdPartyBlurOn(void);
void MdPartyBlurOff(void);
void MdPartyGlowSet(s16 no, HuVecF *pos, GXColor color, float maxTime, float scale);
void MdPartyGlowHook(HU3D_MODEL *modelP, HU3D_PARTICLE *particle, Mtx mtx);
void MdPartyGlowCreate(void);
void MdPartyGlowKill(void);
void MdPartyBurstSet(s16 no, float maxTime, HuVecF *pos, GXColor color);
void MdPartyBurstHook(HU3D_MODEL *modelP, HU3D_PARTICLE *particle, Mtx mtx);
void MdPartyBurstCreate(void);
void MdPartyBurstKill(void);
void MdPartyWarpStart(void);
void MdPartyWarpHook(HU3D_MODEL *modelP, HU3D_PARTICLE *particle, Mtx mtx);
void MdPartyWarpCreate(void);
void MdPartyWarpKill(void);
void MdPartySparkSet(s16 no, float maxTime, HuVecF *pos);
void MdPartySparkHook(HU3D_MODEL *modelP, HU3D_PARTICLE *particle, Mtx mtx);
void MdPartySparkCreate(void);
void MdPartySparkKill(void);
void MdPartyGlitterDispSet(s16 no, s16 dispF);
void MdPartyGlitterStart(s16 no, HuVecF *pos, GXColor *color);
void MdPartyGlitterEnd(s16 no);
void MdPartyGlitterHook(HU3D_MODEL *modelP, HU3D_PARTICLE *particle, Mtx mtx);
void MdPartyGlitterCreate(void);
void MdPartyGlitterKill(void);
void MdPartyFlareDispSet(s16 no, s16 dispF);
void MdPartyFlareOn(s16 no, HuVecF *pos, GXColor *color);
void MdPartyFlareOff(s16 no);
void MdPartyFlareHook(HU3D_MODEL *modelP, HU3D_PARTICLE *particle, Mtx mtx);
void MdPartyFlareCreate(void);
void MdPartyFlareKill(void);
void MdPartyGlitterSet(s16 no, HuVecF *pos, s16 mode);
void MdPartyEffDispSet(s16 grpNo, s16 dispF);
void MdPartyEffOn(s16 grpNo, HuVecF *pos, GXColor *color);
void MdPartyEffOff(s16 grpNo);
void MdPartyEffHook(HU3D_MODEL *modelP, HU3D_PARTICLE *particle, Mtx mtx);
void MdPartyEffCreate(void);
void MdPartyEffKill(void);
void MdPartyParticleInit(OMOBJMAN *objman);
void MdPartyParticleKill(void);
void MdPartyFlashSet(HuVecF *pos);
void MdPartyCursorEffSet(s16 no, HuVecF *pos, s16 dif);
void MdPartyHitEffSet(s16 playerNo, HuVecF *pos, s16 dif, s16 mode);
void MdPartyConfettiStart(void);
void MdPartyEffSet(s16 playerNo, HuVecF *pos, s16 mode, s16 dif);

/* old fn_1_ names of renamed functions */
#define fn_1_0 MdPartyWinCallback
#define fn_1_1B4 MdPartyUnlockInit
#define fn_1_2B0 MdPartyCharDataLoad
#define fn_1_400 MdPartyReadProc
#define fn_1_64C MdPartyReadInit
#define fn_1_718 MdPartyGameWorkSet
#define fn_1_E30 MdPartyGameWorkTest
#define fn_1_109C MdPartyOvlGoto
#define fn_1_1248 MdPartyARamFree
#define fn_1_12F8 MdPartyARamFreeReturn
#define fn_1_13A8 MdPartyBezier
#define fn_1_1404 MdPartyBezierV
#define fn_1_160C MdPartySmooth
#define fn_1_163C MdPartySmoothV
#define fn_1_1780 MdPartySinLerp90
#define fn_1_1868 MdPartyLerp
#define fn_1_18AC MdPartyModelMoveLook
#define fn_1_1C14 MdPartyModelRotTurn
#define fn_1_1D54 MdPartyModelMoveTurn
#define fn_1_21D4 MdPartySinLerp180
#define fn_1_22A8 MdPartyModelBezierMove
#define fn_1_26FC MdPartyPlayerInit
#define fn_1_275C MdPartyPlayerComSet
#define fn_1_2810 MdPartyCameraJump
#define fn_1_2860 MdPartyCameraStop
#define fn_1_28B0 MdPartyCameraSmooth
#define fn_1_2B64 MdPartyCameraFuncSet
#define fn_1_2B74 MdPartyCameraDebug
#define fn_1_3284 MdPartyCameraFuncCall
#define fn_1_32D0 MdPartyCameraUpdate
#define fn_1_33A0 MdPartyCameraInit
#define fn_1_353C MdPartyCameraKill
#define fn_1_3598 MdPartyLightInit
#define fn_1_36C4 MdPartyLightKill
#define fn_1_3700 MdPartyWinOpen
#define fn_1_3770 MdPartyWinClose
#define fn_1_37E0 MdPartyWinWait
#define fn_1_381C MdPartyWinChoiceGet
#define fn_1_38F0 MdPartyWinMesSet
#define fn_1_39AC MdPartyWinInsertMesSet
#define fn_1_3A1C MdPartyWinInit
#define fn_1_3C44 MdPartyWinKill
#define fn_1_3CA0 MdPartyMainWinOpen
#define fn_1_3E0C MdPartyMainWinClose
#define fn_1_3EC8 MdPartyMainWinWait
#define fn_1_3F28 MdPartyMainWinChoiceGet
#define fn_1_4020 MdPartyMainWinMesSet
#define fn_1_4258 MdPartyMainWinInsertMesSet
#define fn_1_444C MdPartyHelpWinMesSet
#define fn_1_45D0 MdPartyHelpWinClose
#define fn_1_467C MdPartyShadowInit
#define fn_1_4730 MdPartyShadowKill
#define fn_1_4734 MdPartySprGrpAttrSet
#define fn_1_47B4 MdPartySprGrpAttrReset
#define fn_1_4834 MdPartySprInit
#define fn_1_4A80 MdPartySprKill
#define fn_1_4A84 MdPartyHostItemThrowUpdate
#define fn_1_4EDC MdPartyHostItemHoldUpdate
#define fn_1_4FE8 MdPartyHostItemThrowSet
#define fn_1_50EC MdPartyHostAReactUpdate
#define fn_1_5194 MdPartyHostAReactSet
#define fn_1_5208 MdPartyHostBReactUpdate
#define fn_1_52B0 MdPartyHostBReactSet
#define fn_1_5324 MdPartyHostAFlyUpdate
#define fn_1_58A0 MdPartyHostAFlySet
#define fn_1_5A18 MdPartyHostAWalkUpdate
#define fn_1_5FB0 MdPartyHostAWalkSet
#define fn_1_605C MdPartyHostACreate
#define fn_1_6284 MdPartyHostAKill
#define fn_1_6324 MdPartyHostBFlyUpdate
#define fn_1_68A4 MdPartyHostBFlySet
#define fn_1_6A20 MdPartyHostBWalkUpdate
#define fn_1_6FB8 MdPartyHostBWalkSet
#define fn_1_7064 MdPartyHostBCreate
#define fn_1_72C4 MdPartyHostKill
#define fn_1_737C MdPartyPlayerJumpUpdate
#define fn_1_7900 MdPartyPlayerJumpStart
#define fn_1_7ABC MdPartyPlayerScaleUpdate
#define fn_1_7C88 MdPartyPlayerAppear
#define fn_1_7ED4 MdPartyCharMdlCreate
#define fn_1_8074 MdPartyCharMdlKill
#define fn_1_8124 MdPartyTurnNumSet
#define fn_1_8318 MdPartyTurnNumHide
#define fn_1_8398 MdPartyTitleFadeOut
#define fn_1_852C MdPartyTitleShow
#define fn_1_85E8 MdPartyTitleFadeOutStart
#define fn_1_861C MdPartyStageCreate
#define fn_1_87BC MdPartyStageKill
#define fn_1_884C MdPartyDescFadeIn
#define fn_1_8950 MdPartyDescFadeOutStart
#define fn_1_8980 MdPartyDescInit
#define fn_1_89E0 MdPartyDescUpdate
#define fn_1_8C28 MdPartyPicSet
#define fn_1_8CCC MdPartyPicFadeOut
#define fn_1_8D20 MdPartyPicUpdate
#define fn_1_9218 MdPartyPicCreate
#define fn_1_945C MdPartyPicKill
#define fn_1_9558 MdPartyPairMesGet
#define fn_1_9658 MdPartyInfoWinMesSet
#define fn_1_96E4 MdPartyInfoWinOpen
#define fn_1_9804 MdPartyInfoWinClose
#define fn_1_98A4 MdPartyInfoWinCreate
#define fn_1_9A24 MdPartyInfoWinKill
#define fn_1_9A7C MdPartyIconAOn
#define fn_1_9B68 MdPartyIconAOff
#define fn_1_9B90 MdPartyIconABankSet
#define fn_1_9BDC MdPartyIconBOn
#define fn_1_9CC8 MdPartyIconBOff
#define fn_1_9CF0 MdPartyIconBBankSet
#define fn_1_9D3C MdPartyIconCOn
#define fn_1_9E28 MdPartyIconCOff
#define fn_1_9E50 MdPartyIconCBankSet
#define fn_1_9E9C MdPartyIconDOn
#define fn_1_9F34 MdPartyIconDOff
#define fn_1_9F50 MdPartyIconAllUpdate
#define fn_1_A5D8 MdPartyIconInit
#define fn_1_A880 MdPartyInfoWinKill2
#define fn_1_A8D8 MdPartyMarkPosSet
#define fn_1_A970 MdPartyIconOpen
#define fn_1_AA94 MdPartyIconClose
#define fn_1_AAF0 MdPartyIconPush
#define fn_1_AB64 MdPartyIconUpdate
#define fn_1_AC88 MdPartyIconCloseAll
#define fn_1_AD14 MdPartyMarkerPosSet
#define fn_1_AD9C MdPartyMarkerShow
#define fn_1_AF70 MdPartyMarkerHide
#define fn_1_B008 MdPartyMarkerUpdate
#define fn_1_B4C8 MdPartyMarkerObjInit
#define fn_1_B6C0 MdPartyMarkerObjKill
#define fn_1_B748 MdPartyCursorFind
#define fn_1_BB30 MdPartyCursorDirFind
#define fn_1_BBD8 MdPartyCursorMove
#define fn_1_C158 MdPartyPanelAnimSet
#define fn_1_C200 MdPartyPanelAnimSetAll
#define fn_1_C28C MdPartyPanelSpinIn
#define fn_1_C698 MdPartyPanelSpinOut
#define fn_1_CA68 MdPartyPanelMove
#define fn_1_D058 MdPartyPanelFlip
#define fn_1_D3E0 MdPartyPanelRotReset
#define fn_1_D648 MdPartyPanelStatSet
#define fn_1_D660 MdPartyPanelStatGet
#define fn_1_D678 MdPartyPanelTimeCheck
#define fn_1_D6B0 MdPartyPanelFuncStart
#define fn_1_D748 MdPartyPanelInit
#define fn_1_D978 MdPartyPanelKill
#define fn_1_DA54 MdPartyPanelIntro
#define fn_1_EB74 MdPartyGroupBCloseExec
#define fn_1_F838 MdPartyGroupBCloseResetExec
#define fn_1_10518 MdPartyPlayerNumSelExec
#define fn_1_114C4 MdPartyPlayerNumSelStart
#define fn_1_114F4 MdPartyPlayerNumSelEnd
#define fn_1_1158C MdPartyPlayerNumOpenExec
#define fn_1_11EAC MdPartyComModelOut
#define fn_1_12260 MdPartyCharModelSpin
#define fn_1_122FC MdPartyCharModelRotReset
#define fn_1_12364 MdPartyCharSelMain
#define fn_1_12B18 MdPartyCharSelInit
#define fn_1_12D7C MdPartyCharSelClose
#define fn_1_12EC8 MdPartyCharSelReset
#define fn_1_12FD8 MdPartyComSelMain
#define fn_1_13EC0 MdPartyComSelInit
#define fn_1_14344 MdPartyComSelClose
#define fn_1_14510 MdPartyComCharSelMain
#define fn_1_14CC0 MdPartyComCharSelInit
#define fn_1_14F64 MdPartyComCharSelClose
#define fn_1_150D0 MdPartyCharSelOpen
#define fn_1_156B4 MdPartyCharSelOutA
#define fn_1_160A0 MdPartyCharSelOutB
#define fn_1_16A8C MdPartyPlayerHide
#define fn_1_16E84 MdPartyCharAllHide
#define fn_1_1733C MdPartyBoardFocus
#define fn_1_1765C MdPartyBoardUnfocus
#define fn_1_17730 MdPartyBoardSelUpdate
#define fn_1_17E10 MdPartyBoardSelStart
#define fn_1_1807C MdPartyBoardRotReset
#define fn_1_18384 MdPartyBoardAppear
#define fn_1_18A10 MdPartyBoardSelWait
#define fn_1_18AA4 MdPartyBoardDecide
#define fn_1_1948C MdPartyMdlSpinHide
#define fn_1_19860 MdPartyHandicapSelect
#define fn_1_19D24 MdPartyHandicapSelectStart
#define fn_1_1A124 MdPartyHandicapSelectEnd
#define fn_1_1A20C MdPartyHandicapMoveIn
#define fn_1_1A8B0 MdPartyPlayerMoveIn
#define fn_1_1AE60 MdPartyPlayerMoveBack
#define fn_1_1B400 MdPartyHandicapMoveOut
#define fn_1_1B9E8 MdPartyTeamArrange
#define fn_1_1D758 MdPartyMdlSpinOut
#define fn_1_1E7F8 MdPartyMdlSpinOutAll
#define fn_1_1F8B8 MdPartyTeamArrangeBack
#define fn_1_21628 MdPartyTeamSelect
#define fn_1_219C8 MdPartyTeamSelectStart
#define fn_1_21C04 MdPartyTeamSelectEnd
#define fn_1_21C9C MdPartyTeamWinOpen
#define fn_1_22010 MdPartyTeamWinReset
#define fn_1_221F4 MdPartyTeamNameSet
#define fn_1_227E4 MdPartyTeamWinClose
#define fn_1_228E8 MdPartyTeamPlayerSelect
#define fn_1_22EB8 MdPartyTeamPlayerSelectStart
#define fn_1_2318C MdPartyTeamPlayerSelectEnd
#define fn_1_23224 MdPartyTeamPlayerReset
#define fn_1_232A0 MdPartyTeamMove
#define fn_1_239DC MdPartyStepEnd
#define fn_1_23A00 MdPartyTeamComReset
#define fn_1_23AC0 MdPartyTeamMoveBack
#define fn_1_24090 MdPartyTeamHandicapSelect
#define fn_1_2457C MdPartyTeamHandicapSelectStart
#define fn_1_249A4 MdPartyTeamHandicapSelectEnd
#define fn_1_24A8C MdPartyTeamHandicapOpen
#define fn_1_25160 MdPartyTeamHandicapReset
#define fn_1_25740 MdPartyTeamMoveRule
#define fn_1_25D10 MdPartyTeamHandicapClose
#define fn_1_26328 MdPartyRuleSelect
#define fn_1_28044 MdPartyRuleSelectStart
#define fn_1_28434 MdPartyRuleSelectEnd
#define fn_1_2851C MdPartyRulePanelSpinIn
#define fn_1_29598 MdPartyRulePanelMoveIn
#define fn_1_2A8C4 MdPartyRulePanelMoveOut
#define fn_1_2BBD8 MdPartyRulePanelSpinOut
#define fn_1_2C874 MdPartyAllKill
#define fn_1_2CD6C MdPartyExitWait
#define fn_1_2CDA8 MdPartyExitCheck
#define fn_1_2CE00 MdPartyMainProc
#define fn_1_2D1D4 MdPartyInit
#define fn_1_2DD38 MdPartyDataClose
#define fn_1_2DD58 ObjectSetup
#define fn_1_2DFD4 MdPartyCameraIntroBoard
#define fn_1_2E338 MdPartyCameraBoardOut
#define fn_1_2E560 MdPartyCameraBoardOutStart
#define fn_1_2E5D4 MdPartyStartIntro
#define fn_1_2E638 MdPartyStartMes
#define fn_1_2EBEC MdPartyHostWalkIn
#define fn_1_2EF24 MdPartyQuitAsk
#define fn_1_2F22C MdPartyResume
#define fn_1_30B4C MdPartyTestStart
#define fn_1_31508 MdPartyHostOut
#define fn_1_31AC0 MdPartyPlayerNumSelect
#define fn_1_32904 MdPartyCharSelStatGet
#define fn_1_329A4 MdPartyCharSelect
#define fn_1_335F0 MdPartyComSelStatGet
#define fn_1_33724 MdPartyComSelect
#define fn_1_34D10 MdPartyPlayerSelect
#define fn_1_34FC4 MdPartyComSelStatGetB
#define fn_1_350F8 MdPartyComSelectB
#define fn_1_36264 MdPartyPlayerSelectB
#define fn_1_36518 MdPartyTeamModeSelect
#define fn_1_37260 MdPartyTeamComStatGet
#define fn_1_3736C MdPartyTeamComSelect
#define fn_1_38314 MdPartyTeamRuleMesShow
#define fn_1_38A3C MdPartyBoardConfirm
#define fn_1_39C60 MdPartyHandicapConfirm
#define fn_1_3AD84 MdPartyTeamHandicapConfirm
#define fn_1_3BEA8 MdPartyTeamConfirm
#define fn_1_3CA70 MdPartyBoardGo
#define fn_1_3CFE4 MdPartyRuleConfirm
#define fn_1_3EAC8 MdPartyMainSeq
#define fn_1_3F424 MdPartyWaveSinLerp90
#define fn_1_3F50C MdPartyWaveLerp
#define fn_1_3F550 MdPartyWaveSmooth
#define fn_1_3F580 MdPartyWaveCopyHook
#define fn_1_3F5EC MdPartyWaveMatHook
#define fn_1_3FA6C MdPartyWaveUpdate
#define fn_1_3FB6C MdPartyWaveStart
#define fn_1_3FC60 MdPartyWaveCreate
#define fn_1_3FE44 MdPartyWaveObjKill
#define fn_1_3FEF4 MdPartyWaveInit
#define fn_1_3FF44 MdPartyWaveKill
#define fn_1_40020 MdPartyWaveStartFirst
#define fn_1_400E0 MdPartyBlurHook
#define fn_1_40A9C MdPartyBlurOn
#define fn_1_40AC8 MdPartyBlurOff
#define fn_1_40AEC MdPartyGlowSet
#define fn_1_40BEC MdPartyGlowHook
#define fn_1_40EAC MdPartyGlowCreate
#define fn_1_40FC0 MdPartyGlowKill
#define fn_1_40FEC MdPartyBurstSet
#define fn_1_410D4 MdPartyBurstHook
#define fn_1_4161C MdPartyBurstCreate
#define fn_1_417DC MdPartyBurstKill
#define fn_1_41834 MdPartyWarpStart
#define fn_1_418D4 MdPartyWarpHook
#define fn_1_42060 MdPartyWarpCreate
#define fn_1_42160 MdPartyWarpKill
#define fn_1_4218C MdPartySparkSet
#define fn_1_42258 MdPartySparkHook
#define fn_1_42B40 MdPartySparkCreate
#define fn_1_42CD4 MdPartySparkKill
#define fn_1_42D2C MdPartyGlitterDispSet
#define fn_1_42DA8 MdPartyGlitterStart
#define fn_1_42EAC MdPartyGlitterEnd
#define fn_1_42F34 MdPartyGlitterHook
#define fn_1_433AC MdPartyGlitterCreate
#define fn_1_43518 MdPartyGlitterKill
#define fn_1_43570 MdPartyFlareDispSet
#define fn_1_435EC MdPartyFlareOn
#define fn_1_43724 MdPartyFlareOff
#define fn_1_43778 MdPartyFlareHook
#define fn_1_43D78 MdPartyFlareCreate
#define fn_1_43EE4 MdPartyFlareKill
#define fn_1_43F3C MdPartyGlitterSet
#define fn_1_44300 MdPartyEffDispSet
#define fn_1_443B4 MdPartyEffOn
#define fn_1_4451C MdPartyEffOff
#define fn_1_4459C MdPartyEffHook
#define fn_1_44B1C MdPartyEffCreate
#define fn_1_44D48 MdPartyEffKill
#define fn_1_44DCC MdPartyParticleInit
#define fn_1_4581C MdPartyParticleKill
#define fn_1_45A48 MdPartyFlashSet
#define fn_1_45C40 MdPartyCursorEffSet
#define fn_1_45F3C MdPartyHitEffSet
#define fn_1_463E0 MdPartyConfettiStart
#define fn_1_464A0 MdPartyEffSet

#endif
