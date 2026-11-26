/* math.h is kept out: this unit defines its own sqrtf (above NameEnterKeySearch). */
#define _MATH_H
#define M_PI 3.141592653589793
#include "game/sprite.h"
#include "game/window.h"
#include "game/data.h"
#include "game/memory.h"
#include "game/process.h"
#include "game/audio.h"
#include "game/pad.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "string.h"

#include "datadir_enum.h"

/* filesel.c (fork 1 moves these into include/REL/fileseldll.h) */
typedef struct FileselBox_s {
    u8 unk00[0x44];
    char name[0x3C];
} FILESEL_BOX;

extern FILESEL_BOX lbl_1_bss_D8[];
extern HUWINID lbl_1_bss_2DE;
extern HUWINID lbl_1_bss_2E0;

#define NAME_SPR_MAX 18
#define NAME_WIN_MAX 3
#define NAME_DEFAULT_NUM 7
#define NAME_LEN_MAX 8

#define NAME_CHAR_CURSOR 0xBF

typedef struct NameKeyboard_s {
    s16 x;
    s16 y;
    u16 val;
} NAMEKEYBOARD;

typedef struct NameSprData_s {
    u32 dataNum;
    u8 bank;
    s16 prio;
    s16 x;
    s16 y;
} NAMESPRDATA;

typedef struct NameKeyBounds_s {
    float minX;
    float minY;
    float maxX;
    float maxY;
} NAMEKEYBOUNDS;

static NAMEKEYBOARD keyboardMain[] = {
    { 80, 140, 'A' },
    { 110, 140, 'B' },
    { 140, 140, 'C' },
    { 170, 140, 'D' },
    { 200, 140, 'E' },
    { 230, 140, 'F' },
    { 260, 140, 'G' },
    { 290, 140, 'H' },
    { 320, 140, 'I' },
    { 350, 140, 'J' },
    { 380, 140, 'K' },
    { 410, 140, 'L' },
    { 440, 140, 'M' },
    { 470, 140, 'N' },
    { 80, 176, 'O' },
    { 110, 176, 'P' },
    { 140, 176, 'Q' },
    { 170, 176, 'R' },
    { 200, 176, 'S' },
    { 230, 176, 'T' },
    { 260, 176, 'U' },
    { 290, 176, 'V' },
    { 320, 176, 'W' },
    { 350, 176, 'X' },
    { 380, 176, 'Y' },
    { 410, 176, 'Z' },
    { 440, 176, ' ' },
    { 470, 176, ' ' },
    { 80, 212, 'a' },
    { 110, 212, 'b' },
    { 140, 212, 'c' },
    { 170, 212, 'd' },
    { 200, 212, 'e' },
    { 230, 212, 'f' },
    { 260, 212, 'g' },
    { 290, 212, 'h' },
    { 320, 212, 'i' },
    { 350, 212, 'j' },
    { 380, 212, 'k' },
    { 410, 212, 'l' },
    { 440, 212, 'm' },
    { 470, 212, 'n' },
    { 80, 248, 'o' },
    { 110, 248, 'p' },
    { 140, 248, 'q' },
    { 170, 248, 'r' },
    { 200, 248, 's' },
    { 230, 248, 't' },
    { 260, 248, 'u' },
    { 290, 248, 'v' },
    { 320, 248, 'w' },
    { 350, 248, 'x' },
    { 380, 248, 'y' },
    { 410, 248, 'z' },
    { 440, 248, 194 },
    { 470, 248, 195 },
    { 120, 293, 0x100 },
    { 200, 293, 0x200 },
    { 348, 293, 0x400 },
    { 468, 293, 0x500 },
    { 0, 0, 0 },
};

static NAMEKEYBOARD keyboardNumSpc[] = {
    { 114, 136, '1' },
    { 144, 136, '2' },
    { 174, 136, '3' },
    { 204, 136, '4' },
    { 234, 136, '5' },
    { 264, 136, '6' },
    { 294, 136, '7' },
    { 324, 136, '8' },
    { 354, 136, '9' },
    { 384, 136, '0' },
    { 414, 136, 134 },
    { 444, 136, ' ' },
    { 114, 172, '[' },
    { 144, 172, '\\' },
    { 174, 172, '~' },
    { 204, 172, 127 },
    { 234, 172, '<' },
    { 264, 172, '=' },
    { 294, 172, '_' },
    { 324, 172, ']' },
    { 354, 172, '^' },
    { 384, 172, 130 },
    { 414, 172, 133 },
    { 444, 172, '}' },
    { 114, 208, 0xe03 },
    { 144, 208, 0xe04 },
    { 174, 208, 0xe05 },
    { 204, 208, 0xe06 },
    { 234, 208, 0xe09 },
    { 264, 208, 0xe07 },
    { 294, 208, 0xe01 },
    { 324, 208, 0xe0b },
    { 354, 208, ';' },
    { 384, 208, ':' },
    { 414, 208, 0xe15 },
    { 444, 208, 0xe16 },
    { 120, 293, 0x100 },
    { 200, 293, 0x200 },
    { 348, 293, 0x400 },
    { 468, 293, 0x500 },
    { 0, 0, 0 },
};

static NAMEKEYBOARD *keyboardTbl[] = {
    keyboardMain,
    keyboardNumSpc,
    NULL,
    NULL,
};

static NAMESPRDATA nameSprData[NAME_SPR_MAX] = {
    { DATANUM(DATA_filesel, 0x16), 0, 100, 0, 0 },
    { DATANUM(DATA_filesel, 0x1C), 1, 120, 0, 0 },
    { DATANUM(DATA_filesel, 0x1C), 6, 120, 4, -110 },
    { DATANUM(DATA_filesel, 0x1C), 8, 120, -156, 107 },
    { DATANUM(DATA_filesel, 0x1C), 8, 120, -76, 107 },
    { DATANUM(DATA_filesel, 0x1C), 8, 120, -1000, -1000 },
    { DATANUM(DATA_filesel, 0x1C), 2, 120, 60, 105 },
    { DATANUM(DATA_filesel, 0x1C), 4, 120, 180, 105 },
    { DATANUM(DATA_filesel, 0x14), 0, 100, -190, -120 },
    { DATANUM(DATA_filesel, 0x14), 1, 100, 190, -120 },
    { DATANUM(DATA_filesel, 0x15), 0, 100, -230, -120 },
    { DATANUM(DATA_filesel, 0x15), 2, 100, 230, -120 },
    { DATANUM(DATA_filesel, 0x1E), 0, 100, 60, 105 },
    { DATANUM(DATA_filesel, 0x1E), 2, 100, 180, 105 },
    { DATANUM(DATA_filesel, 0x1E), 4, 100, -156, 105 },
    { DATANUM(DATA_filesel, 0x1E), 6, 100, -76, 105 },
    { DATANUM(DATA_filesel, 0x1E), 6, 100, -1000, -1000 },
    { DATANUM(DATA_filesel, 0x17), 0, 125, 0, -4 },
};

static HUSPR_GROUPID nameGrpId;
static s16 nameKeyboardNum;
static s16 lbl_1_bss_42A;
static char nameDefault[NAME_DEFAULT_NUM][17];
static HUWINID nameKeyboardWinId[NAME_WIN_MAX];
static u16 nameMesTemp[22];
static s16 nameCharNo;
static char nameMes[50];
static NAMEKEYBOARD *nameKeyboard;

void NameEnterInit(void)
{
    s16 i;
    s16 j;
    char *mes;
    HUSPR_GROUPID grpId;
    ANIMDATA *animP;
    HUSPRID sprId;
    NAMESPRDATA *sprData;
    HUWIN *winP;

    nameGrpId = grpId = HuSprGrpCreate(NAME_SPR_MAX);
    for(j=0; j<NAME_SPR_MAX; j++) {
        sprData = &nameSprData[j];
        animP = HuSprAnimRead(HuDataSelHeapReadNum(sprData->dataNum, HU_MEMNUM_OVL, HEAP_MODEL));
        sprId = HuSprCreate(animP, sprData->prio, sprData->bank);
        HuSprGrpMemberSet(grpId, j, sprId);
        HuSprPosSet(grpId, j, sprData->x, sprData->y);
        HuSprDispOff(grpId, j);
    }
    HuSprGrpPosSet(grpId, 288, 200);
    for(j=0; j<2; j++) {
        nameKeyboardWinId[j] = HuWinCreate(40, 110, 500, 250, 0);
        winP = &winData[nameKeyboardWinId[j]];
        HuWinBGTPLvlSet(nameKeyboardWinId[j], 0);
        animP = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_filesel, 0x1F)+j, HU_MEMNUM_OVL, HEAP_MODEL));
        if(j == 0) {
            HuWinAnimSet(nameKeyboardWinId[j], animP, 0, 250, 90);
        } else {
            HuWinAnimSet(nameKeyboardWinId[j], animP, 0, 250, 72);
        }
        HuWinDispOff(nameKeyboardWinId[j]);
        HuWinPriSet(nameKeyboardWinId[j], 110);
    }
    lbl_1_bss_42A = 0;
    for(i=0; i<NAME_DEFAULT_NUM; i++) {
        for(j=0; j<17; j++) {
            nameDefault[i][j] = 0;
        }
    }
    for(i=0; i<NAME_DEFAULT_NUM; i++) {
        mes = HuWinMesPtrGet(i);
        for(j=0; *mes; mes++) {
            if(*mes >= '0') {
                nameDefault[i][j++] = *mes;
            }
        }
    }
}

void NameEnterCursorSet(s16 keyboardNo, s16 keyNo)
{
    u16 val = nameKeyboard[keyNo].val;
    HuSprPosSet(nameGrpId, 0, 24.0f+(nameKeyboard[keyNo].x-288.0f), (nameKeyboard[keyNo].y-200.0f)-12.0f);
    if(val >= 0x100 && val <= 0x500) {
        HuSprDispOff(nameGrpId, 1);
    } else {
        HuSprDispOn(nameGrpId, 1);
        HuSprPosSet(nameGrpId, 1, 10.0f+(nameKeyboard[keyNo].x-288.0f), 12.0f+(nameKeyboard[keyNo].y-200.0f));
    }
}

s16 NameEnterClose(HUWINID winId);
s16 NameEnterKeySearch(s16 keyNo, float posX, float posY, float moveX, float moveY);
void NameEnterKeyBoundsGet(NAMEKEYBOUNDS *bounds);
void NameEnterDispOn(void);
void NameEnterDispOff(void);

s32 NameEnterMain(s16 boxNo)
{
    s16 keyNo;
    s16 keyboardNo;
    s16 i;
    s16 tempIdx;
    s16 prevKeyboard;
    s16 newKeyNo;
    s16 funcKeyF;
    HUWINID nameWinId;
    u16 val;
    s32 result;
    s16 prevCharNo;
    float moveX;
    float moveY;
    float posX;
    float posY;
    NAMEKEYBOUNDS bounds;

    prevKeyboard = keyboardNo = 0;
    keyNo = 0;
    funcKeyF = 0;
    nameCharNo = 0;
    nameMes[0] = NAME_CHAR_CURSOR;
    nameMes[1] = '\0';
    nameKeyboard = keyboardTbl[keyboardNo];
    NameEnterCursorSet(keyboardNo, keyNo);
    HuSprBankSet(nameGrpId, 3, 8);
    HuSprBankSet(nameGrpId, 4, 8);
    HuSprBankSet(nameGrpId, 5, 8);
    HuSprBankSet(nameGrpId, 6, 2);
    HuSprBankSet(nameGrpId, 7, 4);
    (void)HuAudFXPlay(0);
    NameEnterDispOn();
    HuWinMesSet(lbl_1_bss_2E0, 0x90009);
    HuWinMesSet(lbl_1_bss_2DE, 0x10002);
    HuWinDispOn(lbl_1_bss_2DE);
    nameWinId = HuWinCreate(-10000, 70, 176, 26, 0);
    HuWinBGTPLvlSet(nameWinId, 0);
    HuWinMesSpeedSet(nameWinId, 0);
    HuWinPriSet(nameWinId, 40);
    HuWinMesSet(nameWinId, MESSNUM_PTR(nameMes));
    for(nameKeyboardNum=0; keyboardTbl[nameKeyboardNum]; nameKeyboardNum++);
    NameEnterKeyBoundsGet(&bounds);
    while(1) {
        moveX = moveY = 0;
        if(HuPadDStkRep[0] & PAD_BUTTON_LEFT) {
            moveX = -1;
        } else if(HuPadDStkRep[0] & PAD_BUTTON_RIGHT) {
            moveX = 1;
        }
        if(HuPadDStkRep[0] & PAD_BUTTON_UP) {
            moveY = -1;
        } else if(HuPadDStkRep[0] & PAD_BUTTON_DOWN) {
            moveY = 1;
        }
        if(moveX || moveY) {
            newKeyNo = NameEnterKeySearch(keyNo, nameKeyboard[keyNo].x, nameKeyboard[keyNo].y, moveX, moveY);
            if(newKeyNo == -1) {
                if(moveX) {
                    moveY = 0;
                    if(moveX < 0) {
                        posX = bounds.maxX;
                    } else {
                        posX = bounds.minX;
                    }
                    posY = nameKeyboard[keyNo].y;
                } else {
                    moveX = 0;
                    if(moveY < 0) {
                        posY = bounds.maxY;
                    } else {
                        posY = bounds.minY;
                    }
                    posX = nameKeyboard[keyNo].x;
                }
                newKeyNo = NameEnterKeySearch(keyNo, posX, posY, moveX, moveY);
            }
            if(newKeyNo != -1) {
                HuAudFXPlay(0);
                keyNo = newKeyNo;
                val = nameKeyboard[keyNo].val;
                HuSprBankSet(nameGrpId, 3, 8);
                HuSprBankSet(nameGrpId, 4, 8);
                HuSprBankSet(nameGrpId, 5, 8);
                HuSprBankSet(nameGrpId, 6, 2);
                HuSprBankSet(nameGrpId, 7, 4);
                if(val >= 0x100 && val <= 0x500) {
                    funcKeyF = 1;
                    switch(val) {
                        case 0x100:
                            HuSprBankSet(nameGrpId, 3, 9);
                            break;
                        case 0x200:
                            HuSprBankSet(nameGrpId, 4, 9);
                            break;
                        case 0x300:
                            HuSprBankSet(nameGrpId, 5, 9);
                            break;
                        case 0x400:
                            HuSprBankSet(nameGrpId, 6, 3);
                            break;
                        case 0x500:
                            HuSprBankSet(nameGrpId, 7, 5);
                            break;
                    }
                } else {
                    funcKeyF = 0;
                }
            }
        }
        if(HuPadBtnDown[0] & PAD_BUTTON_START) {
            for(keyNo=0; nameKeyboard[keyNo].val && nameKeyboard[keyNo].val != 0x500; keyNo++);
            HuSprBankSet(nameGrpId, 3, 8);
            HuSprBankSet(nameGrpId, 4, 8);
            HuSprBankSet(nameGrpId, 5, 8);
            HuSprBankSet(nameGrpId, 6, 2);
            HuSprBankSet(nameGrpId, 7, 5);
            funcKeyF = 1;
            i = NameEnterClose(nameWinId);
            if(i) {
                nameCharNo = i;
                prevCharNo = i;
                for(i=tempIdx=0; i<prevCharNo; tempIdx++, i++) {
                    if(nameMes[i+1] == 0x80 || nameMes[i+1] == 0x81) {
                        nameMesTemp[tempIdx] = (nameMes[i] << 8)|nameMes[i+1];
                        nameCharNo--;
                        i++;
                    } else {
                        nameMesTemp[tempIdx] = nameMes[i];
                    }
                }
            }
        } else if(HuPadBtnDown[0] & PAD_BUTTON_A) {
            if((nameKeyboard[keyNo].val < 0x100 || nameKeyboard[keyNo].val > 0x500) && nameCharNo < NAME_LEN_MAX) {
                nameMesTemp[nameCharNo] = nameKeyboard[keyNo].val;
                nameCharNo++;
                for(i=tempIdx=0; i<nameCharNo; i++) {
                    if(nameMesTemp[i] > 255) {
                        nameMes[tempIdx++] = nameMesTemp[i] >> 8;
                        nameMes[tempIdx++] = nameMesTemp[i];
                    } else {
                        nameMes[tempIdx++] = nameMesTemp[i];
                    }
                }
                if(nameCharNo < NAME_LEN_MAX) {
                    nameMes[tempIdx] = NAME_CHAR_CURSOR;
                    nameMes[tempIdx+1] = '\0';
                } else {
                    nameMes[tempIdx] = '\0';
                }
                HuWinMesSet(nameWinId, MESSNUM_PTR(nameMes));
                if(nameCharNo >= NAME_LEN_MAX) {
                    for(keyNo=0; nameKeyboard[keyNo].val && nameKeyboard[keyNo].val != 0x500; keyNo++);
                    HuSprBankSet(nameGrpId, 7, 5);
                    funcKeyF = 1;
                }
            } else if(nameKeyboard[keyNo].val == 0x100) {
                keyboardNo = 0;
            } else if(nameKeyboard[keyNo].val == 0x200) {
                keyboardNo = 1;
            } else if(nameKeyboard[keyNo].val == 0x300) {
                keyboardNo = 2;
            } else if(nameKeyboard[keyNo].val == 0x400) {
                goto cancel;
            } else if(nameKeyboard[keyNo].val == 0x500) {
                for(i=0; nameMes[i]; i++) {
                    if(nameMes[i] == NAME_CHAR_CURSOR) {
                        nameMes[i] = '\0';
                    }
                }
                (void)HuAudFXPlay(2);
                HuSprBankSet(nameGrpId, 13, 3);
                HuPrcSleep(10);
                HuSprBankSet(nameGrpId, 13, 2);
                if(NameEnterClose(nameWinId)) {
                    HuPrcSleep(30);
                }
                i = 0;
                while(1) {
                    GwCommon.name[i] = lbl_1_bss_D8[boxNo].name[i] = nameMes[i];
                    if(nameMes[i] == '\0') {
                        break;
                    }
                    i++;
                }
                result = 0;
                break;
            }
            (void)HuAudFXPlay(1);
        } else if(HuPadBtnDown[0] & PAD_BUTTON_B) {
            cancel:
            nameCharNo--;
            if(nameCharNo < 0) {
                result = -1234;
                break;
            }
            HuAudFXPlay(3);
            for(i=tempIdx=0; i<nameCharNo; i++) {
                if(nameMesTemp[i] > 255) {
                    nameMes[tempIdx++] = nameMesTemp[i] >> 8;
                    nameMes[tempIdx++] = nameMesTemp[i];
                } else {
                    nameMes[tempIdx++] = nameMesTemp[i];
                }
            }
            nameMes[tempIdx] = NAME_CHAR_CURSOR;
            nameMes[tempIdx+1] = '\0';
            HuWinMesSet(nameWinId, MESSNUM_PTR(nameMes));
            HuSprBankSet(nameGrpId, 12, 1);
            HuPrcSleep(5);
            HuSprBankSet(nameGrpId, 12, 0);
        } else if(HuPadBtnDown[0] & PAD_TRIGGER_R) {
            keyboardNo++;
            if(keyboardNo >= nameKeyboardNum) {
                keyboardNo = 0;
            }
            for(keyNo=0; nameKeyboard[keyNo].val; keyNo++) {
                if(nameKeyboard[keyNo].val == ((keyboardNo+1)*256)) {
                    break;
                }
            }
            funcKeyF = 0;
            HuSprBankSet(nameGrpId, 11, 3);
        } else if(HuPadBtnDown[0] & PAD_TRIGGER_L) {
            keyboardNo--;
            if(keyboardNo < 0) {
                keyboardNo = nameKeyboardNum-1;
            }
            for(keyNo=0; nameKeyboard[keyNo].val; keyNo++) {
                if(nameKeyboard[keyNo].val == ((keyboardNo+1)*256)) {
                    break;
                }
            }
            funcKeyF = 0;
            HuSprBankSet(nameGrpId, 10, 1);
        }
        if(keyboardNo != prevKeyboard) {
            HuAudFXPlay(1162);
            HuSprBankSet(nameGrpId, 3, 8);
            HuSprBankSet(nameGrpId, 4, 8);
            HuSprBankSet(nameGrpId, 5, 8);
            HuSprBankSet(nameGrpId, 6, 2);
            HuSprBankSet(nameGrpId, 7, 4);
            if(keyboardNo == 0) {
                HuSprBankSet(nameGrpId, 14, 5);
                HuSprBankSet(nameGrpId, 3, 9);
            } else if(keyboardNo == 1) {
                HuSprBankSet(nameGrpId, 15, 7);
                HuSprBankSet(nameGrpId, 4, 9);
            } else {
                HuSprBankSet(nameGrpId, 16, 9);
                HuSprBankSet(nameGrpId, 5, 9);
            }
            tempIdx = nameKeyboard[keyNo].val;
            HuWinDispOff(nameKeyboardWinId[prevKeyboard]);
            nameKeyboard = keyboardTbl[keyboardNo];
            NameEnterKeyBoundsGet(&bounds);
            HuWinDispOn(nameKeyboardWinId[keyboardNo]);
            for(keyNo=0; nameKeyboard[keyNo].val; keyNo++) {
                if(tempIdx == nameKeyboard[keyNo].val) {
                    break;
                }
            }
            NameEnterCursorSet(keyboardNo, keyNo);
            prevKeyboard = keyboardNo;
            HuPrcSleep(10);
            HuSprBankSet(nameGrpId, 10, 0);
            HuSprBankSet(nameGrpId, 11, 2);
            HuSprBankSet(nameGrpId, 14, 4);
            HuSprBankSet(nameGrpId, 15, 6);
        }
        NameEnterCursorSet(keyboardNo, keyNo);
        HuPrcVSleep();
    }
    HuWinHomeClear(lbl_1_bss_2DE);
    HuWinHomeClear(lbl_1_bss_2E0);
    HuWinKill(nameWinId);
    HuAudFXPlay(0);
    NameEnterDispOff();
    HuPrcVSleep();
    return result;
}

s16 NameEnterClose(HUWINID winId)
{
    s16 i;
    s16 len;
    for(i=len=0; nameMes[i]; i++) {
        if(nameMes[i] && nameMes[i] != 0x10 && nameMes[i] != ' ') {
            len++;
        }
    }
    if(len == 0) {
        strcpy(nameMes, nameDefault[frandmod(NAME_DEFAULT_NUM)]);
        HuWinMesSet(winId, MESSNUM_PTR(nameMes));
        return strlen(nameMes);
    } else {
        return 0;
    }
}

/* Retail's sqrtf here uses plain const locals (as board/scroll.c does): its 0.5 and 3.0 are
 * literals emitted at first use, after NameEnterMain's constants, not math.h's statics. */
extern inline float sqrtf(float x)
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

s16 NameEnterKeySearch(s16 keyNo, float posX, float posY, float moveX, float moveY)
{
    s16 i;
    s16 newKeyNo;
    float cursorX;
    float cursorY;
    float normX;
    float normY;
    float scale;
    float cursorMag;
    float prevCursorMag;
    float normMag;

    scale = HuMagPoint2D(moveX, moveY);
    normX = moveX/scale;
    normY = moveY/scale;
    prevCursorMag = 409600;
    newKeyNo = -1;
    for(i=0; nameKeyboard[i].val; i++) {
        if(i != keyNo) {
            cursorX = nameKeyboard[i].x-posX;
            cursorY = nameKeyboard[i].y-posY;
            cursorMag = HuMagPoint2D(cursorX, cursorY);
            cursorX /= cursorMag;
            cursorY /= cursorMag;
            normMag = HuMagPoint2D(cursorX-normX, cursorY-normY);
            if(nameKeyboard[keyNo].val >= 0x100 && nameKeyboard[keyNo].val <= 0x500 &&
                nameKeyboard[i].val >= 0x100 && nameKeyboard[i].val <= 0x500 &&
                normMag < 0.1 && cursorMag < 55+prevCursorMag) {
                prevCursorMag = cursorMag;
                newKeyNo = i;
            } else if(normMag < 0.8 && cursorMag < prevCursorMag) {
                prevCursorMag = cursorMag;
                newKeyNo = i;
            }
        }
    }
    return newKeyNo;
}

void NameEnterKeyBoundsGet(NAMEKEYBOUNDS *bounds)
{
    s16 i;
    bounds->minX = bounds->minY = 10000;
    bounds->maxX = bounds->maxY = -10000;
    for(i=0; nameKeyboard[i].val; i++) {
        if(nameKeyboard[i].x < bounds->minX) {
            bounds->minX = nameKeyboard[i].x;
        }
        if(nameKeyboard[i].y < bounds->minY) {
            bounds->minY = nameKeyboard[i].y;
        }
        if(nameKeyboard[i].x > bounds->maxX) {
            bounds->maxX = nameKeyboard[i].x;
        }
        if(nameKeyboard[i].y > bounds->maxY) {
            bounds->maxY = nameKeyboard[i].y;
        }
    }
    bounds->minX -= 20;
    bounds->minY -= 20;
    bounds->maxX += 20;
    bounds->maxY += 20;
}

void NameEnterDispOn(void)
{
    s16 i;
    for(i=0; i<NAME_SPR_MAX; i++) {
        HuSprDispOn(nameGrpId, i);
        HuSprTPLvlSet(nameGrpId, i, 1);
    }
    HuWinDispOn(nameKeyboardWinId[0]);
}

void NameEnterDispOff(void)
{
    s16 i;
    for(i=0; i<NAME_SPR_MAX; i++) {
        HuSprDispOff(nameGrpId, i);
    }
    for(i=0; i<NAME_WIN_MAX; i++) {
        HuWinDispOff(nameKeyboardWinId[i]);
    }
}
