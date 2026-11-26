#ifndef _REL_FILESELDLL_H
#define _REL_FILESELDLL_H

#include "game/hu3d.h"
#include "game/window.h"
#include "game/sprite.h"
#include "game/object.h"
#include "game/process.h"

#define FILESEL_RESULT_CANCEL -1234

typedef struct FileselBox_s {
    BOOL validF; //0x00
    HUSPR_GROUPID grpId; //0x04
    HUWINID winId; //0x06
    HU3D_MODELID mdlId; //0x08
    HU3D_MODELID textMdlId; //0x0A
    HU3D_MODELID glowMdlId[3]; //0x0C
    s16 glowNo; //0x12
    s16 lastBoard; //0x14
    s16 bankStar; //0x16
    HU3D_ANIMID frameAnimId; //0x18
    HU3D_ANIMID charAnimId; //0x1A
    OSCalendarTime time; //0x1C
    char name[20]; //0x44
    ANIMDATA *textAnim; //0x58
    void *textBuf; //0x5C
    char boxNoStr[2]; //0x60
    u8 swayMode; //0x62
    HuVecF pos; //0x64
    HuVecF swayOfs; //0x70
    HUPROCESS *proc; //0x7C
} FILESEL_BOX; //size 0x80

typedef struct FileselWave_s {
    float indMtx[2][3]; //0x00
    GXColor color; //0x18
    float scroll; //0x1C
    float scrollSpeed; //0x20
    float time; //0x24
} FILESEL_WAVE; //size 0x28

extern OMOBJ *lbl_1_bss_344;
extern OMOBJMAN *lbl_1_bss_340;
extern HuVecF lbl_1_bss_2E4[3];
extern HUWINID lbl_1_bss_2E0;
extern HUWINID lbl_1_bss_2DE;
extern HU3D_MODELID lbl_1_bss_2D8[3];
extern FILESEL_BOX lbl_1_bss_D8[4];
extern ANIMDATA *lbl_1_bss_CC[3];
extern ANIMDATA *lbl_1_bss_C8;
extern ANIMDATA *lbl_1_bss_A8[8];
extern HU3D_ANIMID lbl_1_bss_A2[3];
extern HU3D_PARMANID lbl_1_bss_98[5];
extern HuVecF lbl_1_bss_68[4];
extern FILESEL_WAVE lbl_1_bss_18[2];
extern ANIMDATA *lbl_1_bss_14;
extern ANIMDATA *lbl_1_bss_10;
extern void *lbl_1_bss_C;
extern OMOBJ *lbl_1_bss_8;

/* filesel.c part 1 */
void ObjectSetup(void);
void FileselMain(void);
void FileselMicInit(void);
void FileselInit(void);
void FileselToueiHook(s16 layerNo);
void FileselPixelFmtHook(s16 layerNo);
void FileselBookOpen(BOOL boxF);
void FileselBookMoveExec(void);
void FileselInfoWinOpen(void);
s32 FileselSelect(void);
void FileselBoxHide(s16 boxNo);
void FileselBoxShow(void);

/* filesel.c part 2 */
void FileselBoxInit(void);
void FileselFbCopyHook(s16 layerNo);
void FileselSprUpdate(s16 boxNo, s16 no);
void FileselBoxPosSetV(s16 boxNo, HuVecF *pos);
void FileselBoxPosSet(s16 boxNo, float x, float y, float z);
void FileselBoxSwayModeSet(s16 boxNo, s32 mode);
void FileselBoxUpdate(void);
void CameraMove(void);
void FileselPrcCreate(void (*func)(void), s32 param);
void FileselPageTurn(void);
s32 FileselCopy(s16 srcBoxNo);

/* filesel.c part 3 */
void FileselBoxFocusSet(s16 boxNo, BOOL focusF);
void FileselBoxCopyFocusSet(s16 boxNo, BOOL focusF);
s32 FileselEraseConfirm(s16 boxNo);
void FileselCommonReset(void);
void FileselZClear(void);
void FileselDecideExec(s16 boxNo, s32 mode);
void FileselWaveCreate(OMOBJ *obj);

/* filename.c */
void NameEnterInit(void);
s32 NameEnterMain(s16 boxNo);

/* saveload.c */
s32 FileBoxInit(HUWINID srcWinId);
BOOL FileTestOpen(void);
s32 FileClear(HUWINID srcWinId);
s32 FileSaveMesOpen(HUWINID srcWinId, u32 mesId);

#endif
