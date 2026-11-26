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
    HU3D_MODELID mdlId; //0x08 bookmark
    HU3D_MODELID fbMdlId; //0x0A
    HU3D_MODELID mdlIdC; //0x0C
    HU3D_MODELID mdlIdE; //0x0E
    HU3D_MODELID mdlId10; //0x10
    s16 state; //0x12
    s16 unk14; //0x14
    s16 unk16; //0x16
    HU3D_ANIMID wakuAnimId; //0x18
    HU3D_ANIMID moyouAnimId; //0x1A
    OSCalendarTime time; //0x1C
    char name[20]; //0x44
    ANIMDATA *fbAnim; //0x58
    void *fbBuf; //0x5C
    char noStr[2]; //0x60
    u8 dispMode; //0x62
    HuVecF pos; //0x64
    HuVecF rot; //0x70
    HUPROCESS *proc; //0x7C
} FILESEL_BOX; //size 0x80

typedef struct FileselWork18_s {
    float unk0[6]; //0x00
    GXColor color; //0x18
    float unk1C; //0x1C
    float unk20; //0x20
    float unk24; //0x24
} FILESEL_WORK18; //size 0x28

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
extern FILESEL_WORK18 lbl_1_bss_18[2];
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

/* filesel.c parts 2 and 3 */
void fn_1_2C6C(void);
void fn_1_35AC(s16 layerNo);
void fn_1_36C4(s16 boxNo, s16 no);
void fn_1_3C80(s16 boxNo, HuVecF *pos);
void fn_1_3CC0(s16 boxNo, float x, float y, float z);
void fn_1_3CF4(s16 boxNo, u8 dispMode);
void fn_1_3D14(void);
void fn_1_50DC(void (*func)(void), s32 arg);
s32 fn_1_5244(s16 boxNo);
void fn_1_63A0(s16 boxNo, s32 flag);
s32 fn_1_642C(s16 boxNo);
void fn_1_69B0(void);
void fn_1_6A8C(void);
void fn_1_6E54(s16 boxNo, s32 flag);
void fn_1_8688(OMOBJ *obj);

/* filename.c */
void fn_1_8B94(void);
s32 fn_1_916C(s16 boxNo);

/* saveload.c */
s32 fn_1_B63C(HUWINID srcWinId);
BOOL fn_1_C278(void);
s32 fn_1_D348(HUWINID srcWinId);
s32 fn_1_D90C(HUWINID srcWinId, u32 mesId);

#endif
