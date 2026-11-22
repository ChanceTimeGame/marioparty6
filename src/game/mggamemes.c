#include "game/gamemes.h"
#include "game/audio.h"
#include "game/sprite.h"
#include "game/gamework.h"
#include "game/flag.h"
#include "game/object.h"

int GameMesStrCopy(GAMEMES *mes, int id);

static float SprPosX[15][2];
static float SprPosY[15][2];
static float SprMainAngle[15][2];
static float SprOrbitAngle[15][2];
static float SprRareZRot[15];
static float SdMesScale[15];
static int SdMesScaleTime[15];
static int MesUnk1[15];
static float SdMesSprPosX[15];
static float SdMesSprPosY[15];
static float SdMesTPLvl[6][15];
static int DrawMesTime[15][6];
static float DrawMesY[15][6];
static s16 WinnerMesSprX[6];
static int WinnerMesSprNum[5];
static int WinnerMesCharNo[GW_PLAYER_MAX];
static float WinnerMesNameX[GW_PLAYER_MAX];
static float WinnerMesNameY[GW_PLAYER_MAX];
// Retail .bss has 0x18 more bytes after WinnerMesNameY that nothing in this unit references
// (mic.c's .bss starts at 0x802870C0). Unnamed in retail; kept as a global so it is emitted.
u8 lbl_802870A8[0x18];

// CodeWarrior emits .sbss statics in reverse definition order: these are retail order reversed.
static int WinnerMesXOfs;
static float GameMesScale;
static float GameMesTPLvl;
static int GameMesTime;
static int GameMesLetterNum;
static int SdMesWork[2];
static int RecordMesValue;
static int RecordMesType;
static int WinnerMesLanguageNo;
static int WinnerMesCharNum;

static float GetGameMesWidth(GAMEMES *mes, int type, int no, float scale);
static void FixWinnerNameOfs(int grpId, int charNo, float scale, int sprNum);

BOOL GameMesMg4PInit(GAMEMES *mes, va_list args)
{
    s16 i;
    float x;
    float y;
    GAMEMESID strMes;
    u32 mesId;
    mes->subMode = va_arg(args, int);
    if(mes->subMode == GAMEMES_MG_TYPE_DRAW) {
        GameMesMgDrawInit(mes, args);
        return TRUE;
    }
    mes->mesMode = 1;
    mes->angle = 0;
    mesId = (mes->subMode == GAMEMES_MG_TYPE_START) ? MG_INST_SYS_START : MG_INST_SYS_FINISH;
    strMes = GameMesStrWinCreate(mes, mesId);
    for(i=0; i<mes->charNum; i++) {
        x = GetGameMesWidth(mes, 0, i, 1.0f);
        y = mes->pos.y;
        SprPosX[i][0] = SprPosX[i][1] = 56.0f;
        SprPosY[i][0] = SprPosY[i][1] = 56.0f+mes->pos.y;
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            x -= SprPosX[i][0];
            y -= SprPosY[i][0];
            HuSprScaleSet(mes->grpId[0], i, mes->scale.x, mes->scale.y);
        } else {
            HuSprScaleSet(mes->grpId[0], i, 0, 0);
        }
        HuSprPosSet(mes->grpId[0], i, x, y);
        HuSprPriSet(mes->grpId[0], i, 5);
        SprMainAngle[i][0] = SprMainAngle[i][1] = 0;
    }
    GameMesLetterNum = 1;
    GameMesTime = 0;
    GameMesPauseEnable(FALSE);
    return TRUE;
}

BOOL GameMesMg4PExec(GAMEMES *mes)
{
    s16 i;
    float x;
    float y;
    mes->time += GameMesVWait;
    if(mes->time >= mes->timeMax && mes->mesMode == 0) {
        mes->stat |= GAMEMES_STAT_TIMEEND;
        mes->mesMode = -1;
        mes->angle = 0;
    }
    switch(mes->mesMode) {
        case 0:
            break;
        
        case 1:
            if(mes->subMode == GAMEMES_MG_TYPE_START) {
                if(mes->time <= 60) {
                    if(mes->time > 0 && (mes->time % 8) == 0) {
                        if(++GameMesLetterNum > mes->charNum) {
                            GameMesLetterNum = mes->charNum;
                        }
                    }
                    for(i=0; i<GameMesLetterNum; i++) {
                        x = GetGameMesWidth(mes, 0, i, 1.0f);
                        y = mes->pos.y;
                        if(mes->time <= 10.0f+(i*8)) {
                            SprMainAngle[i][0] += 9.0f;
                            if(SprMainAngle[i][0] >= 90.0f) {
                                SprMainAngle[i][0] = 90.0f;
                            }
                            x -= SprPosX[i][0];
                            SprPosY[i][0] = (168.0f+SprPosY[i][1])*HuSin(90.0f-SprMainAngle[i][0]);
                            y += 168.0f;
                            y -= SprPosY[i][0];
                        } else {
                            SprMainAngle[i][1] += 9.0f;
                            if(SprMainAngle[i][1] >= 90.0f) {
                                SprMainAngle[i][1] = 90.0f;
                            }
                            SprPosX[i][0] = SprPosX[i][1]*HuSin(90.0f-SprMainAngle[i][1]);
                            x -= SprPosX[i][0];
                            SprPosY[i][0] = 168.0*(1.0-HuSin(SprMainAngle[i][1]));
                            y += SprPosY[i][0];
                        }
                        HuSprPosSet(mes->grpId[0], i, x, y);
                    }
                }
                if(mes->time >= 60) {
                    GameMesScale = 1.0+(0.5*HuSin(9.0f*(mes->time-60)));
                    for(i=0; i<mes->charNum; i++) {
                        x = GetGameMesWidth(mes, 0, i, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], i, x, y);
                        HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 60) {
                    HuAudFXPlay(25);
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time == 80) {
                    mes->stat |= GAMEMES_STAT_TIMEEND;
                    GameMesStartSePlay();
                }
                if(mes->time >= 80) {
                    mes->mesMode = 0;
                    mes->timeMax = 110;
                }
            } else {
                if(mes->time <= 20) {
                    GameMesScale = HuSin(4.5f*mes->time);
                    for(i=0; i<mes->charNum; i++) {
                        x = GetGameMesWidth(mes, 2, i, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], i, x, y);
                        HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 20) {
                    if(mes->subMode == GAMEMES_MG_TYPE_FINISH) {
                        HuAudFXPlay(26);
                    } else {
                        HuAudFXPlay(28);
                    }
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time >= 50) {
                    if(GameMesTime > 0 && (GameMesTime % 8) == 0) {
                        if(++GameMesLetterNum > mes->charNum) {
                            GameMesLetterNum = mes->charNum;
                        }
                    }
                    for(i=0; i<GameMesLetterNum; i++) {
                        x = GetGameMesWidth(mes, 2, i, 1.0f);
                        y = mes->pos.y;
                        if(GameMesTime <= 10.0f+(i*8)) {
                            SprMainAngle[i][0] += 9.0f;
                            if(SprMainAngle[i][0] >= 90.0f) {
                                SprMainAngle[i][0] = 90.0f;
                            }
                            SprPosX[i][0] = SprPosX[i][1]*HuSin(SprMainAngle[i][0]);
                            x -= SprPosX[i][0];
                            SprPosY[i][0] = 168.0*HuSin(90.0f+SprMainAngle[i][0]);
                            y += 168.0f;
                            y -= SprPosY[i][0];
                        } else {
                            x -= SprPosX[i][0];
                            SprMainAngle[i][1] += 9.0f;
                            if(SprMainAngle[i][1] >= 90.0f) {
                                SprMainAngle[i][1] = 90.0f;
                            }
                            SprPosY[i][0] = (168.0f+SprPosY[i][1])*HuSin(SprMainAngle[i][1]);
                            y += 168.0f;
                            y -= SprPosY[i][0];
                        }
                        HuSprPosSet(mes->grpId[0], i, x, y);
                    }
                    GameMesTime++;
                }
                if(mes->time >= 120) {
                    mes->mesMode = 0;
                    mes->stat |= GAMEMES_STAT_KILL;
                }
            }
            break;
        
        case -1:
            mes->angle += 0.1f*GameMesVWait;
            GameMesTPLvl = 1.0f-mes->angle;
            if(GameMesTPLvl <= 0.0f) {
                GameMesTPLvl = 0;
                mes->mesMode = 0;
                mes->stat |= GAMEMES_STAT_KILL;
            }
            for(i=0; i<mes->charNum; i++) {
                HuSprTPLvlSet(mes->grpId[0], i, GameMesTPLvl);
            }
            break;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            GameMesPauseEnable(TRUE);
        }
        return FALSE;
    }
    return TRUE;
}
BOOL GameMesMg1Vs3Init(GAMEMES *mes, va_list args)
{
    s16 i;
    float x;
    float y;
    GAMEMESID strMes;
    u32 mesId;
    mes->subMode = va_arg(args, int);
    if(mes->subMode == GAMEMES_MG_TYPE_DRAW) {
        GameMesMgDrawInit(mes, args);
        return TRUE;
    }
    mes->mesMode = 1;
    mes->angle = 0;
    mesId = (mes->subMode == GAMEMES_MG_TYPE_START) ? MG_INST_SYS_START : MG_INST_SYS_FINISH;
    strMes = GameMesStrWinCreate(mes, mesId);
    for(i=0; i<mes->charNum; i++) {
        x = GetGameMesWidth(mes, 0, i, 1.0f);
        y = mes->pos.y;
        if(i == 0) {
            SprPosX[i][0] = SprPosX[i][1] = 56.0f+GetGameMesWidth(mes, 0, 0, 1.0f);
        } else {
            SprPosX[i][0] = SprPosX[i][1] = 56.0f+((2.0f*mes->pos.x)-GetGameMesWidth(mes, 0, 1, 1.0f));
        }
        SprPosY[i][0] = SprPosY[i][1] = 56.0f+mes->pos.y;
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            if(i == 0) {
                x -= SprPosX[i][0];
                y += SprPosY[i][0];
            } else {
                x += SprPosX[i][0];
                y -= SprPosY[i][0];
            }
            HuSprScaleSet(mes->grpId[0], i, mes->scale.x, mes->scale.y);
        } else {
            HuSprScaleSet(mes->grpId[0], i, 0, 0);
        }
        HuSprPosSet(mes->grpId[0], i, x, y);
        HuSprPriSet(mes->grpId[0], i, 5);
        SprMainAngle[i][0] = SprMainAngle[i][1] = 0;
    }
    GameMesPauseEnable(FALSE);
    return TRUE;
}

BOOL GameMesMg1Vs3Exec(GAMEMES *mes)
{
    s16 i;
    float x;
    float y;
    mes->time += GameMesVWait;
    if(mes->time >= mes->timeMax && mes->mesMode == 0) {
        mes->stat |= GAMEMES_STAT_TIMEEND;
        mes->mesMode = -1;
        mes->angle = 0;
    }
    switch(mes->mesMode) {
        case 0:
            break;
        
        case 1:
            if(mes->subMode == GAMEMES_MG_TYPE_START) {
                if(mes->time <= 60) {
                    for(i=0; i<mes->charNum; i++) {
                        SprMainAngle[i][0] += 2.25f;
                        if(SprMainAngle[i][0] >= 90.0f) {
                            SprMainAngle[i][0] = 90.0f;
                        }
                        x = GetGameMesWidth(mes, 0, i, 1.0f);
                        y = mes->pos.y;
                        if(i == 0) {
                            SprPosX[i][0] = SprPosX[i][1]*(1.0-HuSin(SprMainAngle[i][0]));
                            x -= SprPosX[i][0];
                            SprPosY[i][0] = SprPosY[i][1]*HuSin(90.0f-SprMainAngle[i][0]);
                            y += SprPosY[i][0];
                        } else {
                            SprPosX[i][0] = SprPosX[i][1]*(1.0-HuSin(SprMainAngle[i][0]));
                            x += SprPosX[i][0];
                            SprPosY[i][0] = SprPosY[i][1]*HuSin(90.0f-SprMainAngle[i][0]);
                            y -= SprPosY[i][0];
                        }
                        HuSprPosSet(mes->grpId[0], i, x, y);
                    }
                }
                if(mes->time >= 60) {
                    GameMesScale = 1.0+(0.5*HuSin(9.0f*(mes->time-60)));
                    for(i=0; i<mes->charNum; i++) {
                        x = GetGameMesWidth(mes, 0, i, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], i, x, y);
                        HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 60) {
                    HuAudFXPlay(25);
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time == 80) {
                    mes->stat |= GAMEMES_STAT_TIMEEND;
                    GameMesStartSePlay();
                }
                if(mes->time >= 80) {
                    mes->mesMode = 0;
                    mes->timeMax = 110;
                }
            } else {
                if(mes->time <= 20) {
                    GameMesScale = HuSin(4.5f*mes->time);
                    for(i=0; i<mes->charNum; i++) {
                        x = GetGameMesWidth(mes, 2, i, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], i, x, y);
                        HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 20) {
                    if(mes->subMode == GAMEMES_MG_TYPE_FINISH) {
                        HuAudFXPlay(26);
                    } else {
                        HuAudFXPlay(28);
                    }
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time >= 50) {
                    for(i=0; i<mes->charNum; i++) {
                        SprMainAngle[i][0] += 2.25f;
                        if(SprMainAngle[i][0] >= 90.0f) {
                            SprMainAngle[i][0] = 90.0f;
                        }
                        x = GetGameMesWidth(mes, 2, i, 1.0f);
                        y = mes->pos.y;
                        if(i == 0) {
                            SprPosX[i][0] = SprPosX[i][1]*HuSin(90.0f-SprMainAngle[i][0]);
                            x -= SprPosX[i][1];
                            x += SprPosX[i][0];
                            SprPosY[i][0] = SprPosY[i][1]*(1.0-HuSin(SprMainAngle[i][0]));
                            y += SprPosY[i][1];
                            y -= SprPosY[i][0];
                        } else {
                            SprPosX[i][0] = SprPosX[i][1]*HuSin(90.0f-SprMainAngle[i][0]);
                            x += SprPosX[i][1];
                            x -= SprPosX[i][0];
                            SprPosY[i][0] = SprPosY[i][1]*(1.0-HuSin(SprMainAngle[i][0]));
                            y -= SprPosY[i][1];
                            y += SprPosY[i][0];
                        }
                        HuSprPosSet(mes->grpId[0], i, x, y);
                    }
                }
                if(mes->time >= 120) {
                    mes->mesMode = 0;
                    mes->stat |= GAMEMES_STAT_KILL;
                }
            }
            break;
        
        case -1:
            mes->angle += 0.1f*GameMesVWait;
            GameMesTPLvl = 1.0f-mes->angle;
            if(GameMesTPLvl <= 0.0f) {
                GameMesTPLvl = 0;
                mes->mesMode = 0;
                mes->stat |= GAMEMES_STAT_KILL;
            }
            for(i=0; i<mes->charNum; i++) {
                HuSprTPLvlSet(mes->grpId[0], i, GameMesTPLvl);
            }
            break;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            GameMesPauseEnable(TRUE);
        }
        return FALSE;
    }
    return TRUE;
}

BOOL GameMesMg2Vs2Init(GAMEMES *mes, va_list args)
{
    s16 i;
    float x;
    float y;
    s16 half;
    GAMEMESID strMes;
    u32 mesId;
    mes->subMode = va_arg(args, int);
    if(mes->subMode == GAMEMES_MG_TYPE_DRAW) {
        GameMesMgDrawInit(mes, args);
        return TRUE;
    }
    mes->mesMode = 1;
    mes->angle = 0;
    mesId = (mes->subMode == GAMEMES_MG_TYPE_START) ? MG_INST_SYS_START : MG_INST_SYS_FINISH;
    strMes = GameMesStrWinCreate(mes, mesId);
    for(i=0; i<mes->charNum; i++) {
        x = GetGameMesWidth(mes, 0, i, 1.0f);
        y = mes->pos.y;
        half = mes->charNum/2;
        if(i < half) {
            SprPosX[i][0] = SprPosX[i][1] = 56.0f+GetGameMesWidth(mes, 0, half-1, 1.0f);
        } else if(i > half) {
            SprPosX[i][0] = SprPosX[i][1] = 56.0f+((2.0f*mes->pos.x)-GetGameMesWidth(mes, 0, half+1, 1.0f));
        }
        SprPosY[i][0] = SprPosY[i][1] = 56.0f+mes->pos.y;
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            if(i == half) {
                GameMesScale = 0;
            } else {
                if(i < half) {
                    x += SprPosX[i][0];
                    y -= SprPosY[i][0];
                } else {
                    x -= SprPosX[i][0];
                    y += SprPosY[i][0];
                }
                GameMesScale = 1;
            }
            HuSprScaleSet(mes->grpId[0], i, mes->scale.x, mes->scale.y);
        } else {
            GameMesScale = 0;
            HuSprScaleSet(mes->grpId[0], i, GameMesScale, GameMesScale);
        }
        HuSprPosSet(mes->grpId[0], i, x, y);
        HuSprPriSet(mes->grpId[0], i, 5);
        SprMainAngle[i][0] = SprMainAngle[i][1] = 0;
    }
    GameMesTime = 0;
    GameMesPauseEnable(FALSE);
    return TRUE;
}

BOOL GameMesMg2Vs2Exec(GAMEMES *mes)
{
    s16 i;
    float x;
    float y;
    s16 half;
    mes->time += GameMesVWait;
    if(mes->time >= mes->timeMax && mes->mesMode == 0) {
        mes->stat |= GAMEMES_STAT_TIMEEND;
        mes->mesMode = -1;
        mes->angle = 0;
    }
    switch(mes->mesMode) {
        case 0:
            break;
        
        case 1:
            if(mes->subMode == GAMEMES_MG_TYPE_START) {
                if(mes->time <= 60) {
                    for(i=0; i<mes->charNum; i++) {
                        SprMainAngle[i][0] += 2.25f;
                        if(SprMainAngle[i][0] >= 90.0f) {
                            SprMainAngle[i][0] = 90.0f;
                        }
                        x = GetGameMesWidth(mes, 0, i, 1.0f);
                        y = mes->pos.y;
                        half = mes->charNum/2;
                        if(i == half) {
                            if(mes->time < 20) {
                                GameMesScale = 0;
                            } else {
                                GameMesScale = HuSin(4.5f*GameMesTime);
                                if(GameMesTime > 20) {
                                    GameMesScale = 1;
                                }
                            }
                            HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                        } else if(i < half) {
                            SprPosX[i][0] = SprPosX[i][1]*(1.0-HuSin(SprMainAngle[i][0]));
                            x -= SprPosX[i][0];
                            SprPosY[i][0] = SprPosY[i][1]*HuSin(90.0f-SprMainAngle[i][0]);
                            y -= SprPosY[i][0];
                        } else {
                            SprPosX[i][0] = SprPosX[i][1]*(1.0-HuSin(SprMainAngle[i][0]));
                            x += SprPosX[i][0];
                            SprPosY[i][0] = SprPosY[i][1]*HuSin(90.0f-SprMainAngle[i][0]);
                            y += SprPosY[i][0];
                        }
                        HuSprPosSet(mes->grpId[0], i, x, y);
                    }
                    if(mes->time >= 20) {
                        GameMesTime++;
                    }
                }
                if(mes->time >= 60) {
                    GameMesScale = 1.0+(0.5*HuSin(9.0f*(mes->time-60)));
                    for(i=0; i<mes->charNum; i++) {
                        x = GetGameMesWidth(mes, 0, i, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], i, x, y);
                        HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 60) {
                    HuAudFXPlay(25);
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time == 80) {
                    mes->stat |= GAMEMES_STAT_TIMEEND;
                    GameMesStartSePlay();
                }
                if(mes->time >= 80) {
                    mes->mesMode = 0;
                    mes->timeMax = 110;
                }
            } else {
                if(mes->time <= 20) {
                    GameMesScale = HuSin(4.5f*mes->time);
                    for(i=0; i<mes->charNum; i++) {
                        x = GetGameMesWidth(mes, 2, i, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], i, x, y);
                        HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 20) {
                    if(mes->subMode == GAMEMES_MG_TYPE_FINISH) {
                        HuAudFXPlay(26);
                    } else {
                        HuAudFXPlay(28);
                    }
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time >= 50) {
                    for(i=0; i<mes->charNum; i++) {
                        SprMainAngle[i][0] += 2.25f;
                        if(SprMainAngle[i][0] >= 90.0f) {
                            SprMainAngle[i][0] = 90.0f;
                        }
                        x = GetGameMesWidth(mes, 2, i, 1.0f);
                        y = mes->pos.y;
                        half = mes->charNum/2;
                        if(i == half) {
                            GameMesScale = 1.0+(3.0*HuSin(4.5f*GameMesTime));
                            if(GameMesTime > 20) {
                                GameMesScale = 1;
                            }
                            HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                            GameMesTPLvl = HuSin(4.5f*GameMesTime);
                            if(GameMesTime > 20) {
                                GameMesTPLvl = 1;
                            }
                            HuSprTPLvlSet(mes->grpId[0], i, 1.0f-GameMesTPLvl);
                        } else if(i < half) {
                            SprPosX[i][0] = SprPosX[i][1]*HuSin(90.0f-SprMainAngle[i][0]);
                            x -= SprPosX[i][1];
                            x += SprPosX[i][0];
                            SprPosY[i][0] = SprPosY[i][1]*(1.0-HuSin(SprMainAngle[i][0]));
                            y -= SprPosY[i][1];
                            y += SprPosY[i][0];
                        } else {
                            SprPosX[i][0] = SprPosX[i][1]*HuSin(90.0f-SprMainAngle[i][0]);
                            x += SprPosX[i][1];
                            x -= SprPosX[i][0];
                            SprPosY[i][0] = SprPosY[i][1]*(1.0-HuSin(SprMainAngle[i][0]));
                            y += SprPosY[i][1];
                            y -= SprPosY[i][0];
                        }
                        HuSprPosSet(mes->grpId[0], i, x, y);
                    }
                    GameMesTime++;
                }
                if(mes->time >= 120) {
                    mes->mesMode = 0;
                    mes->stat |= GAMEMES_STAT_KILL;
                }
            }
            break;
        
        case -1:
            mes->angle += 0.1f*GameMesVWait;
            GameMesTPLvl = 1.0f-mes->angle;
            if(GameMesTPLvl <= 0.0f) {
                GameMesTPLvl = 0;
                mes->mesMode = 0;
                mes->stat |= GAMEMES_STAT_KILL;
            }
            for(i=0; i<mes->charNum; i++) {
                HuSprTPLvlSet(mes->grpId[0], i, GameMesTPLvl);
            }
            break;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            GameMesPauseEnable(TRUE);
        }
        return FALSE;
    }
    return TRUE;
}

BOOL GameMesMgBattleInit(GAMEMES *mes, va_list args)
{
    s16 i;
    float x;
    float y;
    GAMEMESID strMes;
    u32 mesId;
    mes->subMode = va_arg(args, int);
    if(mes->subMode == GAMEMES_MG_TYPE_DRAW) {
        GameMesMgDrawInit(mes, args);
        return TRUE;
    }
    mes->mesMode = 1;
    mes->angle = 0;
    mesId = (mes->subMode == GAMEMES_MG_TYPE_START) ? MG_INST_SYS_START : MG_INST_SYS_FINISH;
    strMes = GameMesStrWinCreate(mes, mesId);
    for(i=0; i<mes->charNum; i++) {
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            x = mes->pos.x;
            y = -112.0f;
            HuSprPosSet(mes->grpId[0], i, x, y);
            HuSprPriSet(mes->grpId[0], i, 5);
            HuSprScaleSet(mes->grpId[0], i, mes->scale.x, mes->scale.y);
        } else {
            x = GetGameMesWidth(mes, 0, i, 1.0f);
            y = mes->pos.y;
            GameMesScale = 0;
            HuSprScaleSet(mes->grpId[0], i, GameMesScale, GameMesScale);
        }
        SprOrbitAngle[i][0] = SprOrbitAngle[i][1] = 0;
        SprMainAngle[i][0] = SprMainAngle[i][1] = 0;
    }
    GameMesTime = 0;
    GameMesPauseEnable(FALSE);
    return TRUE;
}

BOOL GameMesMgBattleExec(GAMEMES *mes)
{
    s16 i;
    float x;
    float y;
    float radius;
    float angle;
    float baseX;
    s16 half;
    mes->time += GameMesVWait;
    if(mes->time >= mes->timeMax && mes->mesMode == 0) {
        mes->stat |= GAMEMES_STAT_TIMEEND;
        mes->mesMode = -1;
        mes->angle = 0;
    }
    switch(mes->mesMode) {
        case 0:
            break;
        
        case 1:
            if(mes->subMode == GAMEMES_MG_TYPE_START) {
                if(mes->time <= 60) {
                    for(i=0; i<mes->charNum; i++) {
                        if(mes->time < 30) {
                            x = mes->pos.x;
                            y = mes->pos.y;
                            SprMainAngle[i][0] += 3.6f;
                            if(SprMainAngle[i][0] >= 90.0f) {
                                SprMainAngle[i][0] = 90.0f;
                            }
                            radius = (112.0f+mes->pos.x)*(1.0-HuSin(SprMainAngle[i][0]));
                            angle = (360.0f/mes->charNum)*(i-2);
                            SprOrbitAngle[i][0] += 5.0f;
                            angle += SprOrbitAngle[i][0];
                            x += radius*HuCos(angle);
                            y += radius*HuSin(angle);
                        } else if(mes->time < 55) {
                            x = mes->pos.x;
                            y = mes->pos.y;
                            half = mes->charNum/2;
                            baseX = GetGameMesWidth(mes, 0, 0, 1.0f);
                            SprOrbitAngle[i][1] += (GetGameMesWidth(mes, 0, half, 1.0f)-baseX)/15.0f;
                            if(i < half) {
                                x -= SprOrbitAngle[i][1];
                                if(x < GetGameMesWidth(mes, 0, i, 1.0f)) {
                                    x = GetGameMesWidth(mes, 0, i, 1.0f);
                                }
                            } else {
                                x += SprOrbitAngle[i][1];
                                if(x > GetGameMesWidth(mes, 0, i, 1.0f)) {
                                    x = GetGameMesWidth(mes, 0, i, 1.0f);
                                }
                            }
                        } else {
                            x = GetGameMesWidth(mes, 0, i, 1.0f);
                            y = mes->pos.y;
                        }
                        HuSprPosSet(mes->grpId[0], i, x, y);
                    }
                }
                if(mes->time >= 60) {
                    GameMesScale = 1.0+(0.5*HuSin(9.0f*(mes->time-60)));
                    for(i=0; i<mes->charNum; i++) {
                        x = GetGameMesWidth(mes, 0, i, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], i, x, y);
                        HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 60) {
                    HuAudFXPlay(25);
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time == 80) {
                    mes->stat |= GAMEMES_STAT_TIMEEND;
                    GameMesStartSePlay();
                }
                if(mes->time >= 80) {
                    mes->mesMode = 0;
                    mes->timeMax = 110;
                }
            } else {
                if(mes->time <= 20) {
                    GameMesScale = HuSin(4.5f*mes->time);
                    for(i=0; i<mes->charNum; i++) {
                        x = GetGameMesWidth(mes, 2, i, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], i, x, y);
                        HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 20) {
                    if(mes->subMode == GAMEMES_MG_TYPE_FINISH) {
                        HuAudFXPlay(26);
                    } else {
                        HuAudFXPlay(28);
                    }
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time >= 50) {
                    for(i=0; i<mes->charNum; i++) {
                        if(GameMesTime < 20) {
                            x = GetGameMesWidth(mes, 2, i, 1.0f);
                            y = mes->pos.y;
                            half = mes->charNum/2;
                            baseX = GetGameMesWidth(mes, 2, 0, 1.0f);
                            SprOrbitAngle[i][1] += (GetGameMesWidth(mes, 2, half, 1.0f)-baseX)/15.0f;
                            if(i < half) {
                                x += SprOrbitAngle[i][1];
                                if(x > mes->pos.x) {
                                    x = mes->pos.x;
                                }
                            } else {
                                x -= SprOrbitAngle[i][1];
                                if(x < mes->pos.x) {
                                    x = mes->pos.x;
                                }
                            }
                        } else {
                            x = mes->pos.x;
                            y = mes->pos.y;
                            SprMainAngle[i][0] += 4.5f;
                            if(SprMainAngle[i][0] >= 90.0f) {
                                SprMainAngle[i][0] = 90.0f;
                            }
                            radius = (112.0f+mes->pos.x)*(1.0-HuSin(90.0f-SprMainAngle[i][0]));
                            angle = (360.0f/mes->charNum)*(i-2);
                            SprOrbitAngle[i][0] -= 5.0f;
                            angle += SprOrbitAngle[i][0];
                            x += radius*HuCos(angle);
                            y += radius*HuSin(angle);
                        }
                        HuSprPosSet(mes->grpId[0], i, x, y);
                    }
                    GameMesTime++;
                }
                if(mes->time >= 120) {
                    mes->mesMode = 0;
                    mes->stat |= GAMEMES_STAT_KILL;
                }
            }
            break;
        
        case -1:
            mes->angle += 0.1f*GameMesVWait;
            GameMesTPLvl = 1.0f-mes->angle;
            if(GameMesTPLvl <= 0.0f) {
                GameMesTPLvl = 0;
                mes->mesMode = 0;
                mes->stat |= GAMEMES_STAT_KILL;
            }
            for(i=0; i<mes->charNum; i++) {
                HuSprTPLvlSet(mes->grpId[0], i, GameMesTPLvl);
            }
            break;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            GameMesPauseEnable(TRUE);
        }
        return FALSE;
    }
    return TRUE;
}

BOOL GameMesMgKoopaInit(GAMEMES *mes, va_list args)
{
    s16 i;
    GAMEMESID strMes;
    u32 mesId;
    float x;
    float y;

    mes->subMode = va_arg(args, int);
    if(mes->subMode == GAMEMES_MG_TYPE_DRAW) {
        GameMesMgDrawInit(mes, args);
        return TRUE;
    }
    mes->mesMode = 1;
    mes->angle = 0;
    mesId = (mes->subMode == GAMEMES_MG_TYPE_START) ? MG_INST_SYS_START : MG_INST_SYS_FINISH;
    strMes = GameMesStrWinCreate(mes, mesId);
    for(i=0; i<mes->charNum; i++) {
        x = GetGameMesWidth(mes, 0, i, 1.0f);
        y = mes->pos.y;
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            SprOrbitAngle[i][0] = SprOrbitAngle[i][1] = (56+(2*mes->pos.x))-GetGameMesWidth(mes, 0, 0, 1.0f);
            x += SprOrbitAngle[i][0];
            HuSprScaleSet(mes->grpId[0], i, mes->scale.x, mes->scale.y);
        } else {
            SprOrbitAngle[i][0] = 0;
            SprOrbitAngle[i][1] = (2*mes->pos.x)-GetGameMesWidth(mes, 0, 0, 1.0f);
            GameMesScale = 0;
            HuSprScaleSet(mes->grpId[0], i, GameMesScale, GameMesScale);
        }
        HuSprPosSet(mes->grpId[0], i, x, y);
        HuSprPriSet(mes->grpId[0], i, 5);
    }
    GameMesPauseEnable(FALSE);
    return TRUE;
}

BOOL GameMesMgKoopaExec(GAMEMES *mes)
{
    s16 i;
    float x;
    float y;

    mes->time += GameMesVWait;
    if(mes->time >= mes->timeMax && mes->mesMode == 0) {
        mes->stat |= GAMEMES_STAT_TIMEEND;
        mes->mesMode = -1;
        mes->angle = 0;
    }
    switch(mes->mesMode) {
        case 1:
            if(mes->subMode == GAMEMES_MG_TYPE_START) {
                if(mes->time <= 60) {
                    for(i=0; i<mes->charNum; i++) {
                        if(i == mes->charNum-1) {
                            if(mes->time >= 35) {
                                SprOrbitAngle[i][0] -= SprOrbitAngle[i][1]/20.0f;
                            }
                            x = SprOrbitAngle[i][0]+GetGameMesWidth(mes, 0, i, 1.0f);
                            if(x < GetGameMesWidth(mes, 0, i, 1.0f)) {
                                x = GetGameMesWidth(mes, 0, i, 1.0f);
                            }
                        } else {
                            if(mes->time >= 15) {
                                SprOrbitAngle[i][0] -= SprOrbitAngle[i][1]/20.0f;
                            }
                            x = SprOrbitAngle[i][0]+GetGameMesWidth(mes, 0, i, 1.0f);
                            if(x < GetGameMesWidth(mes, 0, i, 1.0f)) {
                                x = GetGameMesWidth(mes, 0, i, 1.0f);
                            }
                        }
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], i, x, y);
                    }
                }
                if(mes->time >= 60) {
                    GameMesScale = 1.0+(0.5*HuSin(9.0f*(mes->time-60)));
                    for(i=0; i<mes->charNum; i++) {
                        x = GetGameMesWidth(mes, 0, i, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], i, x, y);
                        HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 60) {
                    HuAudFXPlay(25);
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time == 80) {
                    mes->stat |= GAMEMES_STAT_TIMEEND;
                    GameMesStartSePlay();
                }
                if(mes->time >= 80) {
                    mes->mesMode = 0;
                    mes->timeMax = 110;
                }
            } else {
                if(mes->time <= 20) {
                    GameMesScale = HuSin(4.5f*mes->time);
                    for(i=0; i<mes->charNum; i++) {
                        x = GetGameMesWidth(mes, 2, i, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], i, x, y);
                        HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 20) {
                    if(mes->subMode == GAMEMES_MG_TYPE_FINISH) {
                        HuAudFXPlay(26);
                    } else {
                        HuAudFXPlay(28);
                    }
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time >= 50) {
                    for(i=0; i<mes->charNum; i++) {
                        if(i == mes->charNum-1) {
                            if(mes->time >= 70) {
                                SprOrbitAngle[i][0] += SprOrbitAngle[i][1]/20.0f;
                            }
                        } else {
                            SprOrbitAngle[i][0] += SprOrbitAngle[i][1]/20.0f;
                        }
                        x = GetGameMesWidth(mes, 2, i, 1.0f)-SprOrbitAngle[i][0];
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], i, x, y);
                    }
                }
                if(mes->time >= 120) {
                    mes->mesMode = 0;
                    mes->stat |= GAMEMES_STAT_KILL;
                }
            }
            break;

        case -1:
            mes->angle += 0.1f*GameMesVWait;
            GameMesTPLvl = 1.0f-mes->angle;
            if(GameMesTPLvl <= 0) {
                GameMesTPLvl = 0;
                mes->mesMode = 0;
                mes->stat |= GAMEMES_STAT_KILL;
            }
            for(i=0; i<mes->charNum; i++) {
                HuSprTPLvlSet(mes->grpId[0], i, GameMesTPLvl);
            }
            break;

        default:
            break;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            GameMesPauseEnable(TRUE);
        }
        return FALSE;
    }
    return TRUE;
}
BOOL GameMesMgLastInit(GAMEMES *mes, va_list args)
{
    s16 i;
    GAMEMESID strMes;
    u32 mesId;
    float x;
    float y;

    mes->subMode = va_arg(args, int);
    if(mes->subMode == GAMEMES_MG_TYPE_DRAW) {
        GameMesMgDrawInit(mes, args);
        return TRUE;
    }
    mes->mesMode = 1;
    mes->angle = 0;
    mesId = (mes->subMode == GAMEMES_MG_TYPE_START) ? MG_INST_SYS_START : MG_INST_SYS_FINISH;
    strMes = GameMesStrWinCreate(mes, mesId);
    for(i=0; i<mes->charNum; i++) {
        x = GetGameMesWidth(mes, 0, i, 1.0f);
        y = mes->pos.y;
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            SprOrbitAngle[i][0] = SprOrbitAngle[i][1] = (56+(2*mes->pos.x))-GetGameMesWidth(mes, 0, 0, 1.0f);
            x += SprOrbitAngle[i][0];
            HuSprScaleSet(mes->grpId[0], i, mes->scale.x, mes->scale.y);
            SprRareZRot[i] = 600;
            HuSprZRotSet(mes->grpId[0], i, SprRareZRot[i]);
        } else {
            SprOrbitAngle[i][0] = 0;
            SprOrbitAngle[i][1] = (2*mes->pos.x)-GetGameMesWidth(mes, 0, 0, 1.0f);
            GameMesScale = 0;
            HuSprScaleSet(mes->grpId[0], i, GameMesScale, GameMesScale);
            SprRareZRot[i] = 0;
        }
        HuSprPosSet(mes->grpId[0], i, x, y);
        HuSprPriSet(mes->grpId[0], i, 5);
    }
    GameMesPauseEnable(FALSE);
    return TRUE;
}

BOOL GameMesMgLastExec(GAMEMES *mes)
{
    s16 i;
    float x;
    float y;

    mes->time += GameMesVWait;
    if(mes->time >= mes->timeMax && mes->mesMode == 0) {
        mes->stat |= GAMEMES_STAT_TIMEEND;
        mes->mesMode = -1;
        mes->angle = 0;
    }
    switch(mes->mesMode) {
        case 1:
            if(mes->subMode == GAMEMES_MG_TYPE_START) {
                if(mes->time <= 60 && mes->time >= 20) {
                    for(i=0; i<mes->charNum; i++) {
                        SprOrbitAngle[i][0] -= SprOrbitAngle[i][1]/20.0f;
                        x = SprOrbitAngle[i][0]+GetGameMesWidth(mes, 0, i, 1.0f);
                        if(x < GetGameMesWidth(mes, 0, i, 1.0f)) {
                            x = GetGameMesWidth(mes, 0, i, 1.0f);
                        }
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], i, x, y);
                        if(mes->time < 40) {
                            SprRareZRot[i] -= 30;
                            HuSprZRotSet(mes->grpId[0], i, SprRareZRot[i]);
                        }
                    }
                }
                if(mes->time >= 60) {
                    GameMesScale = 1.0+(0.5*HuSin(9.0f*(mes->time-60)));
                    for(i=0; i<mes->charNum; i++) {
                        x = GetGameMesWidth(mes, 0, i, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], i, x, y);
                        HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 60) {
                    HuAudFXPlay(25);
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time == 80) {
                    mes->stat |= GAMEMES_STAT_TIMEEND;
                    GameMesStartSePlay();
                }
                if(mes->time >= 80) {
                    mes->mesMode = 0;
                    mes->timeMax = 110;
                }
            } else {
                if(mes->time <= 20) {
                    GameMesScale = HuSin(4.5f*mes->time);
                    for(i=0; i<mes->charNum; i++) {
                        x = GetGameMesWidth(mes, 2, i, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], i, x, y);
                        HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 20) {
                    if(mes->subMode == GAMEMES_MG_TYPE_FINISH) {
                        HuAudFXPlay(26);
                    } else {
                        HuAudFXPlay(28);
                    }
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time >= 50) {
                    for(i=0; i<mes->charNum; i++) {
                        SprOrbitAngle[i][0] += SprOrbitAngle[i][1]/20.0f;
                        x = GetGameMesWidth(mes, 2, i, 1.0f)-SprOrbitAngle[i][0];
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], i, x, y);
                        SprRareZRot[i] -= 30;
                        HuSprZRotSet(mes->grpId[0], i, SprRareZRot[i]);
                    }
                }
                if(mes->time >= 120) {
                    mes->mesMode = 0;
                    mes->stat |= GAMEMES_STAT_KILL;
                }
            }
            break;

        case -1:
            mes->angle += 0.1f*GameMesVWait;
            GameMesTPLvl = 1.0f-mes->angle;
            if(GameMesTPLvl <= 0) {
                GameMesTPLvl = 0;
                mes->mesMode = 0;
                mes->stat |= GAMEMES_STAT_KILL;
            }
            for(i=0; i<mes->charNum; i++) {
                HuSprTPLvlSet(mes->grpId[0], i, GameMesTPLvl);
            }
            break;

        default:
            break;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            GameMesPauseEnable(TRUE);
        }
        return FALSE;
    }
    return TRUE;
}

BOOL GameMesMgKettouInit(GAMEMES *mes, va_list args)
{
    s16 i;
    s16 center;
    GAMEMESID strMes;
    u32 mesId;
    float y;
    float x;

    mes->subMode = va_arg(args, int);
    if(mes->subMode == GAMEMES_MG_TYPE_DRAW) {
        GameMesMgDrawInit(mes, args);
        return TRUE;
    }
    mes->mesMode = 1;
    mes->angle = 0;
    mesId = (mes->subMode == GAMEMES_MG_TYPE_START) ? MG_INST_SYS_START : MG_INST_SYS_FINISH;
    strMes = GameMesStrWinCreate(mes, mesId);
    for(i=0; i<mes->charNum; i++) {
        x = GetGameMesWidth(mes, 0, i, 1.0f);
        y = mes->pos.y;
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            center = mes->charNum/2;
            if(i == center) {
                GameMesScale = 0;
            } else {
                SprOrbitAngle[i][0] = SprOrbitAngle[i][1] = 56+mes->pos.y;
                if(i < center) {
                    y -= SprOrbitAngle[i][0];
                } else {
                    y += SprOrbitAngle[i][0];
                }
                GameMesScale = 1;
            }
            HuSprScaleSet(mes->grpId[0], i, GameMesScale, GameMesScale);
        } else {
            SprOrbitAngle[i][0] = 0;
            SprOrbitAngle[i][1] = 56+mes->pos.y;
            GameMesScale = 0;
            HuSprScaleSet(mes->grpId[0], i, GameMesScale, GameMesScale);
        }
        HuSprPosSet(mes->grpId[0], i, x, y);
        HuSprPriSet(mes->grpId[0], i, 5);
    }
    GameMesTime = 0;
    GameMesPauseEnable(FALSE);
    return TRUE;
}

BOOL GameMesMgKettouExec(GAMEMES *mes)
{
    s16 i;
    s16 center;
    float y;
    float x;

    mes->time += GameMesVWait;
    if(mes->time >= mes->timeMax && mes->mesMode == 0) {
        mes->stat |= GAMEMES_STAT_TIMEEND;
        mes->mesMode = -1;
        mes->angle = 0;
    }
    switch(mes->mesMode) {
        case 1:
            if(mes->subMode == GAMEMES_MG_TYPE_START) {
                if(mes->time <= 60 && mes->time >= 20) {
                    for(i=0; i<mes->charNum; i++) {
                        x = GetGameMesWidth(mes, 0, i, 1.0f);
                        SprOrbitAngle[i][0] -= SprOrbitAngle[i][1]/20.0f;
                        center = mes->charNum/2;
                        if(i == center) {
                            y = mes->pos.y;
                            GameMesScale = HuSin(4.5f*GameMesTime);
                            if(GameMesTime > 20) {
                                GameMesScale = 1;
                            }
                            HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                        } else if(i < center) {
                            y = mes->pos.y-SprOrbitAngle[i][0];
                            if(y > mes->pos.y) {
                                y = mes->pos.y;
                            }
                        } else {
                            y = mes->pos.y+SprOrbitAngle[i][0];
                            if(y < mes->pos.y) {
                                y = mes->pos.y;
                            }
                        }
                        HuSprPosSet(mes->grpId[0], i, x, y);
                    }
                    GameMesTime++;
                }
                if(mes->time >= 60) {
                    GameMesScale = 1.0+(0.5*HuSin(9.0f*(mes->time-60)));
                    for(i=0; i<mes->charNum; i++) {
                        x = GetGameMesWidth(mes, 0, i, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], i, x, y);
                        HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 60) {
                    HuAudFXPlay(25);
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time == 80) {
                    mes->stat |= GAMEMES_STAT_TIMEEND;
                    GameMesStartSePlay();
                }
                if(mes->time >= 80) {
                    mes->mesMode = 0;
                    mes->timeMax = 110;
                }
            } else {
                if(mes->time <= 20) {
                    GameMesScale = HuSin(4.5f*mes->time);
                    for(i=0; i<mes->charNum; i++) {
                        x = GetGameMesWidth(mes, 2, i, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], i, x, y);
                        HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 20) {
                    if(mes->subMode == GAMEMES_MG_TYPE_FINISH) {
                        HuAudFXPlay(26);
                    } else {
                        HuAudFXPlay(28);
                    }
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time >= 50) {
                    for(i=0; i<mes->charNum; i++) {
                        x = GetGameMesWidth(mes, 2, i, 1.0f);
                        SprOrbitAngle[i][0] += SprOrbitAngle[i][1]/20.0f;
                        center = mes->charNum/2;
                        if(i == center) {
                            y = mes->pos.y;
                            GameMesScale = 1.0+(3.0*HuSin(4.5f*GameMesTime));
                            if(GameMesTime > 20) {
                                GameMesScale = 1;
                            }
                            HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                            GameMesTPLvl = HuSin(4.5f*GameMesTime);
                            if(GameMesTime > 20) {
                                GameMesTPLvl = 1;
                            }
                            HuSprTPLvlSet(mes->grpId[0], i, 1.0f-GameMesTPLvl);
                        } else if(i < center) {
                            y = mes->pos.y-SprOrbitAngle[i][0];
                        } else {
                            y = mes->pos.y+SprOrbitAngle[i][0];
                        }
                        HuSprPosSet(mes->grpId[0], i, x, y);
                    }
                    GameMesTime++;
                }
                if(mes->time >= 120) {
                    mes->mesMode = 0;
                    mes->stat |= GAMEMES_STAT_KILL;
                }
            }
            break;

        case -1:
            mes->angle += 0.1f*GameMesVWait;
            GameMesTPLvl = 1.0f-mes->angle;
            if(GameMesTPLvl <= 0) {
                GameMesTPLvl = 0;
                mes->mesMode = 0;
                mes->stat |= GAMEMES_STAT_KILL;
            }
            for(i=0; i<mes->charNum; i++) {
                HuSprTPLvlSet(mes->grpId[0], i, GameMesTPLvl);
            }
            break;

        default:
            break;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            GameMesPauseEnable(TRUE);
        }
        return FALSE;
    }
    return TRUE;
}

BOOL GameMesMgDonkeyInit(GAMEMES *mes, va_list args)
{
    s16 i;
    s16 center;
    GAMEMESID strMes;
    u32 mesId;
    float x;
    float y;

    mes->subMode = va_arg(args, int);
    if(mes->subMode == GAMEMES_MG_TYPE_DRAW) {
        GameMesMgDrawInit(mes, args);
        return TRUE;
    }
    mes->mesMode = 1;
    mes->angle = 0;
    mesId = (mes->subMode == GAMEMES_MG_TYPE_START) ? MG_INST_SYS_START : MG_INST_SYS_FINISH;
    strMes = GameMesStrWinCreate(mes, mesId);
    for(i=0; i<mes->charNum; i++) {
        x = GetGameMesWidth(mes, 0, i, 1.0f);
        y = mes->pos.y;
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            center = mes->charNum/2;
            if(i == center) {
                GameMesScale = 0;
            } else {
                SprOrbitAngle[i][0] = SprOrbitAngle[i][1] = 56+mes->pos.y;
                if(i < center) {
                    x -= SprOrbitAngle[i][0];
                    y -= SprOrbitAngle[i][0];
                } else {
                    x += SprOrbitAngle[i][0];
                    y -= SprOrbitAngle[i][0];
                }
                GameMesScale = 1;
            }
            HuSprScaleSet(mes->grpId[0], i, GameMesScale, GameMesScale);
        } else {
            SprOrbitAngle[i][0] = 0;
            SprOrbitAngle[i][1] = 56+mes->pos.y;
            GameMesScale = 0;
            HuSprScaleSet(mes->grpId[0], i, GameMesScale, GameMesScale);
        }
        HuSprPosSet(mes->grpId[0], i, x, y);
        HuSprPriSet(mes->grpId[0], i, 5);
    }
    GameMesTime = 0;
    GameMesPauseEnable(FALSE);
    return TRUE;
}

BOOL GameMesMgDonkeyExec(GAMEMES *mes)
{
    s16 i;
    s16 center;
    float x;
    float y;

    mes->time += GameMesVWait;
    if(mes->time >= mes->timeMax && mes->mesMode == 0) {
        mes->stat |= GAMEMES_STAT_TIMEEND;
        mes->mesMode = -1;
        mes->angle = 0;
    }
    switch(mes->mesMode) {
        case 1:
            if(mes->subMode == GAMEMES_MG_TYPE_START) {
                if(mes->time <= 60 && mes->time >= 20) {
                    for(i=0; i<mes->charNum; i++) {
                        x = GetGameMesWidth(mes, 0, i, 1.0f);
                        SprOrbitAngle[i][0] -= SprOrbitAngle[i][1]/20.0f;
                        center = mes->charNum/2;
                        if(i == center) {
                            y = mes->pos.y;
                            GameMesScale = HuSin(4.5f*GameMesTime);
                            if(GameMesTime > 20) {
                                GameMesScale = 1;
                            }
                            HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                        } else if(i < center) {
                            x = GetGameMesWidth(mes, 0, i, 1.0f)-SprOrbitAngle[i][0];
                            if(x > GetGameMesWidth(mes, 0, i, 1.0f)) {
                                x = GetGameMesWidth(mes, 0, i, 1.0f);
                            }
                            y = mes->pos.y-SprOrbitAngle[i][0];
                            if(y > mes->pos.y) {
                                y = mes->pos.y;
                            }
                        } else {
                            x = SprOrbitAngle[i][0]+GetGameMesWidth(mes, 0, i, 1.0f);
                            if(x < GetGameMesWidth(mes, 0, i, 1.0f)) {
                                x = GetGameMesWidth(mes, 0, i, 1.0f);
                            }
                            y = mes->pos.y-SprOrbitAngle[i][0];
                            if(y > mes->pos.y) {
                                y = mes->pos.y;
                            }
                        }
                        HuSprPosSet(mes->grpId[0], i, x, y);
                    }
                    GameMesTime++;
                }
                if(mes->time >= 60) {
                    GameMesScale = 1.0+(0.5*HuSin(9.0f*(mes->time-60)));
                    for(i=0; i<mes->charNum; i++) {
                        x = GetGameMesWidth(mes, 0, i, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], i, x, y);
                        HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 60) {
                    HuAudFXPlay(25);
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time == 80) {
                    mes->stat |= GAMEMES_STAT_TIMEEND;
                    GameMesStartSePlay();
                }
                if(mes->time >= 80) {
                    mes->mesMode = 0;
                    mes->timeMax = 110;
                }
            } else {
                if(mes->time <= 20) {
                    GameMesScale = HuSin(4.5f*mes->time);
                    for(i=0; i<mes->charNum; i++) {
                        x = GetGameMesWidth(mes, 2, i, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], i, x, y);
                        HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 20) {
                    if(mes->subMode == GAMEMES_MG_TYPE_FINISH) {
                        HuAudFXPlay(26);
                    } else {
                        HuAudFXPlay(28);
                    }
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time >= 50) {
                    for(i=0; i<mes->charNum; i++) {
                        x = GetGameMesWidth(mes, 2, i, 1.0f);
                        SprOrbitAngle[i][0] += SprOrbitAngle[i][1]/20.0f;
                        center = mes->charNum/2;
                        if(i == center) {
                            y = mes->pos.y;
                            GameMesScale = 1.0+(3.0*HuSin(4.5f*GameMesTime));
                            if(GameMesTime > 20) {
                                GameMesScale = 1;
                            }
                            HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                            GameMesTPLvl = HuSin(4.5f*GameMesTime);
                            if(GameMesTime > 20) {
                                GameMesTPLvl = 1;
                            }
                            HuSprTPLvlSet(mes->grpId[0], i, 1.0f-GameMesTPLvl);
                        } else if(i < center) {
                            x = GetGameMesWidth(mes, 2, i, 1.0f)-SprOrbitAngle[i][0];
                            y = mes->pos.y-SprOrbitAngle[i][0];
                        } else {
                            x = SprOrbitAngle[i][0]+GetGameMesWidth(mes, 2, i, 1.0f);
                            y = mes->pos.y-SprOrbitAngle[i][0];
                        }
                        HuSprPosSet(mes->grpId[0], i, x, y);
                    }
                    GameMesTime++;
                }
                if(mes->time >= 120) {
                    mes->mesMode = 0;
                    mes->stat |= GAMEMES_STAT_KILL;
                }
            }
            break;

        case -1:
            mes->angle += 0.1f*GameMesVWait;
            GameMesTPLvl = 1.0f-mes->angle;
            if(GameMesTPLvl <= 0) {
                GameMesTPLvl = 0;
                mes->mesMode = 0;
                mes->stat |= GAMEMES_STAT_KILL;
            }
            for(i=0; i<mes->charNum; i++) {
                HuSprTPLvlSet(mes->grpId[0], i, GameMesTPLvl);
            }
            break;

        default:
            break;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            GameMesPauseEnable(TRUE);
        }
        return FALSE;
    }
    return TRUE;
}

BOOL GameMesMg678Init(GAMEMES *mes, va_list args)
{
    s16 i;
    s16 j;
    GAMEMESID strMes;
    float x;
    float y;
    mes->subMode = va_arg(args, int);
    if(mes->subMode == GAMEMES_MG_TYPE_DRAW) {
        GameMesMgDrawInit(mes, args);
        return TRUE;
    }
    mes->mesMode = 1;
    mes->angle = 0;
    if(mes->subMode == GAMEMES_MG_TYPE_START) {
        for(j=0; j<15; j++) {
            SdMesScale[j] = 20;
            SdMesScaleTime[j] = 0;
        }
        GameMesScale = 0;
        SdMesWork[0] = SdMesWork[1] = 0;
    } else {
        for(j=0; j<15; j++) {
            MesUnk1[j] = 0;
            SdMesScale[j] = 1;
        }
        GameMesScale = 0;
        SdMesWork[0] = 1;
        SdMesWork[1] = 0;
    }
    for(i=0; i<6; i++) {
        if(i == 0) {
            u32 mesId = (mes->subMode == GAMEMES_MG_TYPE_START) ? MG_INST_SYS_START : MG_INST_SYS_FINISH;
            strMes = GameMesStrWinCreate(mes, mesId);
        } else {
            strMes = GameMesStrCopy(mes, strMes);
        }
        for(j=0; j<mes->charNum; j++) {
            if(mes->subMode == GAMEMES_MG_TYPE_START) {
                x = mes->pos.x+(mes->scale.x*(((-mes->charNum*56)/2)+(j*56)+28));
                y = mes->pos.y;
                if(i == 0) {
                    HuSprScaleSet(mes->grpId[i], j, mes->scale.x*SdMesScale[j], mes->scale.y*SdMesScale[j]);
                } else {
                    HuSprScaleSet(mes->grpId[i], j, mes->scale.x, mes->scale.y);
                }
            } else {
                x = mes->pos.x+(mes->scale.x*(((-mes->charNum*56)/2)+(j*56)+28));
                y = mes->pos.y;
                if(i == 0) {
                    SdMesSprPosX[j] = x;
                    SdMesSprPosY[j] = y;
                    HuSprScaleSet(mes->grpId[i], j, 0, 0);
                } else {
                    HuSprScaleSet(mes->grpId[i], j, mes->scale.x, mes->scale.y);
                }
            }
            HuSprPosSet(mes->grpId[i], j, x, y);
            HuSprPriSet(mes->grpId[i], j, (mes->subMode == GAMEMES_MG_TYPE_START) ? (i+5) : (j+(i+5)));
            if(i == 0) {
                HuSprTPLvlSet(mes->grpId[i], j, (mes->subMode == GAMEMES_MG_TYPE_START) ? 0.0f : 1.0f);
                SdMesTPLvl[i][j] = (mes->subMode == GAMEMES_MG_TYPE_START) ? 0.0f : 1.0f;
            } else {
                HuSprTPLvlSet(mes->grpId[i], j, 1.0f/(i+1));
                SdMesTPLvl[i][j] = 1.0f/(i+1);
            }
            if(mes->subMode == GAMEMES_MG_TYPE_START) {
                HuSprDispOff(mes->grpId[i], j);
            }
            if(i != 0 && mes->subMode == GAMEMES_MG_TYPE_FINISH) {
                HuSprDispOff(mes->grpId[i], j);
            }
        }
    }
    GameMesPauseEnable(FALSE);
    return TRUE;
}

BOOL GameMesMg678Exec(GAMEMES *mes)
{
    s16 j;
    s16 i;
    int timeStart;
    int timeEnd;
    float x;
    float y;

    mes->time += GameMesVWait;
    if(mes->time >= mes->timeMax && mes->mesMode == 0) {
        mes->stat |= GAMEMES_STAT_TIMEEND;
        mes->mesMode = -1;
        mes->angle = 0;
    }
    switch(mes->mesMode) {
        case 1:
            if(mes->subMode == GAMEMES_MG_TYPE_START) {
                if(GameMesLanguageNo == 0) {
                    timeStart = 110;
                    timeEnd = 130;
                } else {
                    timeStart = 123;
                    timeEnd = 143;
                }
                if(mes->time <= timeStart) {
                    if(SdMesWork[0] < mes->charNum) {
                        if(mes->time > 0) {
                            SdMesWork[0] = 1;
                        }
                        if(mes->time > 8) {
                            SdMesWork[0]++;
                        }
                        for(j=2; j<mes->charNum; j++) {
                            if(mes->time > ((j-2)*7)+45) {
                                SdMesWork[0]++;
                            }
                        }
                        if(SdMesWork[0] > mes->charNum) {
                            SdMesWork[0] = mes->charNum;
                        }
                    }
                    for(i=0; i<6; i++) {
                        for(j=0; j<SdMesWork[0]; j++) {
                            if(i == 0) {
                                SdMesScale[j] -= 20.0f/(timeStart/5);
                                if(SdMesScale[j] < 1) {
                                    SdMesScale[j] = 1;
                                }
                                HuSprDispOn(mes->grpId[i], j);
                            }
                            HuSprScaleSet(mes->grpId[i], j, mes->scale.x*SdMesScale[j], mes->scale.y*SdMesScale[j]);
                            if(i == 0) {
                                SdMesTPLvl[0][j] += 0.017f;
                                if(SdMesTPLvl[0][j] > 1) {
                                    SdMesTPLvl[0][j] = 1;
                                }
                                HuSprTPLvlSet(mes->grpId[i], j, SdMesTPLvl[0][j]);
                                x = mes->pos.x+(mes->scale.x*(((-mes->charNum*56)/2)+(j*56)+28));
                            } else {
                                x = mes->pos.x+(mes->scale.x*(((-mes->charNum*56)/2)+(j*56)+28));
                            }
                            y = mes->pos.y;
                            HuSprPosSet(mes->grpId[i], j, x, y);
                            if(i == 0) {
                                if(SdMesScale[j] == 1.0f) {
                                    SdMesScaleTime[j]++;
                                }
                            }
                            if(SdMesScaleTime[j] != 0) {
                                HuSprDispOn(mes->grpId[i], j);
                                HuSprScaleSet(mes->grpId[i], j, mes->scale.x*(1.0f+(0.04f*((SdMesScaleTime[j]/2)*i))), mes->scale.y*(1.0f+(0.04f*((SdMesScaleTime[j]/2)*i))));
                                if(i != 0) {
                                    SdMesTPLvl[i][j] -= 0.02f;
                                    if(SdMesTPLvl[i][j] < 0.01f) {
                                        SdMesTPLvl[i][j] = 0;
                                        HuSprDispOff(mes->grpId[i], j);
                                    } else {
                                        HuSprTPLvlSet(mes->grpId[i], j, SdMesTPLvl[i][j]);
                                        x += frandmod(7)*(0.1f*(frandmod(2) != 0 ? 1 : -1));
                                        y += frandmod(7)*(0.1f*(frandmod(2) != 0 ? 1 : -1));
                                        HuSprPosSet(mes->grpId[i], j, x, y);
                                        if(i == 1) {
                                            HuSprPosSet(mes->grpId[0], j, x, y);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if(mes->time >= timeStart) {
                    GameMesScale = 1.0+(0.5*HuSin(9.0f*(mes->time-timeStart)));
                    for(j=0; j<mes->charNum; j++) {
                        x = mes->pos.x+(GameMesScale*(mes->scale.x*(((-mes->charNum*56)/2)+(j*56)+28)));
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                        HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == timeStart) {
                    HuAudFXPlay(25);
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time == timeEnd) {
                    mes->stat |= GAMEMES_STAT_TIMEEND;
                    GameMesStartSePlay();
                }
                if(mes->time >= timeEnd) {
                    mes->mesMode = 0;
                    mes->timeMax = timeEnd+30;
                }
            } else {
                if(mes->time <= 20) {
                    GameMesScale = HuSin(4.5f*mes->time);
                    for(j=0; j<mes->charNum; j++) {
                        x = mes->pos.x+(GameMesScale*(mes->scale.x*(((-mes->charNum*56)/2)+(j*56)+28)));
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                        HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 20) {
                    if(mes->subMode == GAMEMES_MG_TYPE_FINISH) {
                        HuAudFXPlay(26);
                    } else {
                        HuAudFXPlay(28);
                    }
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time >= 50) {
                    for(i=0; i<1; i++) {
                        for(j=0; j<SdMesWork[0]; j++) {
                            x = mes->pos.x+(mes->scale.x*(((-mes->charNum*56)/2)+(j*56)+28));
                            y = mes->pos.y;
                            HuSprPosSet(mes->grpId[i], j, x, y);
                            if(SdMesScale[j] == 1.0f) {
                                if(SdMesWork[1] < 3) {
                                    HuSprScaleSet(mes->grpId[i], j, mes->scale.x*(1.0f-(0.2f*(SdMesWork[1]+1))), mes->scale.y*(1.0f-(0.2f*(SdMesWork[1]+1))));
                                } else if(SdMesWork[1] < 5) {
                                    HuSprScaleSet(mes->grpId[i], j, mes->scale.x*(1.0f-(0.3f*(4-SdMesWork[1]))), mes->scale.y*(1.0f-(0.3f*(4-SdMesWork[1]))));
                                }
                            }
                            if(SdMesWork[1] >= 5 || SdMesScale[j] > 1.0f) {
                                if(i == 0) {
                                    SdMesScale[j] += 1.5f;
                                    if(SdMesScale[j] < 0) {
                                        SdMesScale[j] = 0;
                                    }
                                }
                                HuSprScaleSet(mes->grpId[i], j, mes->scale.x*SdMesScale[j], mes->scale.y*SdMesScale[j]);
                                SdMesTPLvl[i][j] -= 0.07f;
                                if(SdMesTPLvl[i][j] < 0) {
                                    SdMesTPLvl[i][j] = 0;
                                    HuSprDispOff(mes->grpId[i], j);
                                } else {
                                    HuSprTPLvlSet(mes->grpId[i], j, SdMesTPLvl[i][j]);
                                }
                            }
                        }
                    }
                    if(SdMesWork[0] <= mes->charNum && ++SdMesWork[1] == 8) {
                        SdMesWork[0]++;
                        if(SdMesWork[0] > mes->charNum) {
                            SdMesWork[0] = mes->charNum;
                        }
                        SdMesWork[1] = 0;
                    }
                }
                if(mes->time >= 180) {
                    mes->mesMode = 0;
                    mes->stat |= GAMEMES_STAT_KILL;
                }
            }
            break;

        case -1:
            mes->angle += 0.1f*GameMesVWait;
            GameMesTPLvl = 1.0f-mes->angle;
            if(GameMesTPLvl <= 0) {
                GameMesTPLvl = 0;
                mes->mesMode = 0;
                mes->stat |= GAMEMES_STAT_KILL;
            }
            for(j=0; j<mes->charNum; j++) {
                HuSprTPLvlSet(mes->grpId[0], j, GameMesTPLvl);
            }
            break;

        default:
            break;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            GameMesPauseEnable(TRUE);
        }
        return FALSE;
    }
    return TRUE;
}
/* g2c2 end */

BOOL GameMesMgDrawInit(GAMEMES *mes, va_list args)
{
    s16 i;
    s16 j;
    float x;
    float y;
    float scale;
    GAMEMESID strMes;

    mes->subMode = GAMEMES_MG_TYPE_DRAW;
    mes->mesNo = GAMEMES_TYPE_MG_DRAW;
    mes->mesMode = 1;
    mes->angle = 0;
    for(j=0; j<15; j++) {
        MesUnk1[j] = 0;
        for(i=0; i<6; i++) {
            DrawMesTime[j][i] = 0;
            DrawMesY[j][i] = mes->pos.y;
        }
    }
    GameMesScale = 0;
    SdMesWork[0] = 1;
    SdMesWork[1] = 0;
    for(i=0; i<6; i++) {
        if(i == 0) {
            strMes = GameMesStrWinCreate(mes, MG_INST_SYS_TIE);
        } else {
            strMes = GameMesStrCopy(mes, strMes);
        }
        for(j=0; j<mes->charNum; j++) {
            x = GetGameMesWidth(mes, 1, j, 1.0f);
            y = mes->pos.y;
            if(i == 0) {
                SdMesSprPosX[j] = x;
                SdMesSprPosY[j] = y;
            }
            HuSprPosSet(mes->grpId[i], j, x, y);
            HuSprPriSet(mes->grpId[i], j, 5+i);
            HuSprTPLvlSet(mes->grpId[i], j, 1.0f/(i+1));
            if(mes->charNum <= 6) {
                scale = 1.0f;
            } else {
                scale = 6.0f/mes->charNum;
            }
            mes->scale.x = scale;
            if(i == 0) {
                HuSprScaleSet(mes->grpId[i], j, 0, 0);
            } else {
                HuSprScaleSet(mes->grpId[i], j, mes->scale.x, mes->scale.y);
                HuSprDispOff(mes->grpId[i], j);
            }
        }
    }
    return TRUE;
}

BOOL GameMesMgDrawExec(GAMEMES *mes)
{
    s16 i;
    s16 j;
    s16 k;
    float x;
    float y;
    float t;

    mes->time += GameMesVWait;
    switch(mes->mesMode) {
        case 1:
            if(mes->time <= 20) {
                GameMesScale = HuSin(mes->time*4.5f);
                for(j=0; j<mes->charNum; j++) {
                    x = GetGameMesWidth(mes, 1, j, GameMesScale);
                    y = mes->pos.y;
                    HuSprPosSet(mes->grpId[0], j, x, y);
                    HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                }
            }
            if(mes->time == 20) {
                HuAudFXPlay(28);
                mes->stat |= GAMEMES_STAT_FXPLAY;
            }
            if(mes->time >= 140) {
                for(i=0; i<6; i++) {
                    for(j=0; j<SdMesWork[0]; j++) {
                        DrawMesTime[j][i]++;
                        x = GetGameMesWidth(mes, 1, j, 1.0f);
                        if(i == 0) {
                            t = DrawMesTime[j][i];
                            y = mes->pos.y-((36.0f*t)-(1.8f*t*t));
                            for(k=5; k>0; k--) {
                                DrawMesY[j][k] = DrawMesY[j][k-1];
                            }
                            DrawMesY[j][0] = y;
                        } else {
                            y = DrawMesY[j][i];
                        }
                        HuSprPosSet(mes->grpId[i], j, x, y);
                        HuSprDispOn(mes->grpId[i], j);
                        DrawMesY[j][i] = y;
                    }
                }
                if(SdMesWork[0] < mes->charNum && ++SdMesWork[1] == 10) {
                    SdMesWork[0]++;
                    SdMesWork[1] = 0;
                }
            }
            if(mes->time >= 210) {
                mes->mesMode = 0;
                mes->stat |= GAMEMES_STAT_KILL;
            }
            break;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        return FALSE;
    }
    return TRUE;
}

BOOL GameMesMgRecordInit(GAMEMES *mes, va_list args)
{
    s16 no;
    int num;
    s16 i;
    ANIMDATA *animP;
    HUSPRID sprId;
    s16 x;
    s16 separatorNo[2];

    RecordMesValue = va_arg(args, int);
    mes->mesMode = 1;
    mes->angle = 0;
    switch(omcurovl) {
        case DLL_m678dll:
            RecordMesType = 0;
            num = 0;
            break;

        case DLL_m608dll:
            RecordMesType = 1;
            num = 1;
            break;

        case DLL_m606dll:
        case DLL_m618dll:
        case DLL_m638dll:
        case DLL_m648dll:
        case DLL_m650dll:
        case DLL_m652dll:
            RecordMesType = 2;
            num = 0;
            break;

        case DLL_mikeactdll:
            RecordMesValue = GWMikeActRecordGet(GWSubGameNoGet());
            RecordMesType = 2;
            num = 0;
            break;

        default:
            RecordMesType = -1;
            num = 0;
            break;
    }
    mes->grpId[0] = HuSprGrpCreate(num+10);
    num = (GameMesLanguageNo == 0) ? GAMEMES_ANM_record_new
        : (GameMesLanguageNo == 1) ? GAMEMES_ANM_record_new_us
        : (GameMesLanguageNo == 2) ? GAMEMES_ANM_record_new_ge
        : (GameMesLanguageNo == 3) ? GAMEMES_ANM_record_new_fr
        : (GameMesLanguageNo == 4) ? GAMEMES_ANM_record_new_it
        : (GameMesLanguageNo == 5) ? GAMEMES_ANM_record_new_sp
        : GAMEMES_ANM_record_new_us;
    animP = HuSprAnimRead(GameMesDataRead(num));
    sprId = HuSprCreate(animP, 0, 0);
    HuSprGrpMemberSet(mes->grpId[0], 0, sprId);
    HuSprPosSet(mes->grpId[0], 0, 0, -32);
    HuSprPriSet(mes->grpId[0], 0, 2);
    animP = HuSprAnimRead(GameMesDataRead(GAMEMES_ANM_record_crown));
    sprId = HuSprCreate(animP, 0, 0);
    HuSprGrpMemberSet(mes->grpId[0], 1, sprId);
    HuSprPriSet(mes->grpId[0], 1, 2);
    animP = HuSprAnimRead(GameMesDataRead(GAMEMES_ANM_record_num));
    for(i=0; i<8; i++) {
        sprId = HuSprCreate(animP, 0, 0);
        HuSprGrpMemberSet(mes->grpId[0], i+2, sprId);
        HuSprPriSet(mes->grpId[0], i+2, 2);
    }
    if(RecordMesType == 1) {
        animP = HuSprAnimRead(GameMesDataRead(GAMEMES_ANM_record_dot));
        sprId = HuSprCreate(animP, 0, 0);
        HuSprGrpMemberSet(mes->grpId[0], 10, sprId);
        HuSprPriSet(mes->grpId[0], 10, 2);
    }
    if(RecordMesType < 2) {
        no = 0;
        if(RecordMesValue > 999999) {
            RecordMesValue = 999999;
        }
        num = RecordMesValue/100000;
        if(num != 0) {
            HuSprBankSet(mes->grpId[0], no+2, num);
            RecordMesValue -= num*100000;
            no++;
        }
        num = RecordMesValue/10000;
        if(num != 0 || no != 0) {
            HuSprBankSet(mes->grpId[0], no+2, num);
            RecordMesValue -= num*10000;
            no++;
        }
        num = RecordMesValue/1000;
        if(num != 0 || no != 0) {
            HuSprBankSet(mes->grpId[0], no+2, num);
            RecordMesValue -= num*1000;
            no++;
        }
        num = RecordMesValue/100;
        if(num != 0 || no != 0) {
            HuSprBankSet(mes->grpId[0], no+2, num);
            RecordMesValue -= num*100;
            no++;
        }
        num = RecordMesValue/10;
        if(num != 0 || no != 0) {
            HuSprBankSet(mes->grpId[0], no+2, num);
            RecordMesValue -= num*10;
            no++;
        }
        HuSprBankSet(mes->grpId[0], no+2, RecordMesValue);
        no++;
        for(i=no; i<8; i++) {
            HuSprDispOff(mes->grpId[0], i+2);
        }
        if(RecordMesType == 1) {
            num = 16;
            HuSprPosSet(mes->grpId[0], 10, 48+(-((no*44)+48)/2)+(no*44)+num, 32);
        } else {
            num = 0;
        }
        HuSprPosSet(mes->grpId[0], 1, 24+(-((no*44)+48)/2)-num, 32);
        for(i=0; i<no; i++) {
            HuSprPosSet(mes->grpId[0], i+2, 70+(-((no*44)+48)/2)+(i*44)-num, 32);
            if(RecordMesType == -1) {
                HuSprDispOff(mes->grpId[0], i+2);
            }
        }
    } else if(RecordMesType == 2) {
        no = 0;
        num = RecordMesValue/36000;
        if(num != 0) {
            HuSprBankSet(mes->grpId[0], no+2, num);
            RecordMesValue -= num*36000;
            no++;
        }
        num = RecordMesValue/3600;
        HuSprBankSet(mes->grpId[0], no+2, num);
        RecordMesValue -= num*3600;
        no++;

        HuSprBankSet(mes->grpId[0], no+2, 11);
        separatorNo[0] = no;
        no++;

        num = RecordMesValue/600;
        HuSprBankSet(mes->grpId[0], no+2, num);
        RecordMesValue -= num*600;
        no++;

        num = RecordMesValue/60;
        HuSprBankSet(mes->grpId[0], no+2, num);
        RecordMesValue -= num*60;
        no++;

        HuSprBankSet(mes->grpId[0], no+2, 10);
        separatorNo[1] = no;
        no++;

        RecordMesValue = (100.0/60.0)*(float)RecordMesValue;

        num = RecordMesValue/10;
        HuSprBankSet(mes->grpId[0], no+2, num);
        RecordMesValue -= num*10;
        no++;

        HuSprBankSet(mes->grpId[0], no+2, RecordMesValue);
        no++;
        for(i=no; i<8; i++) {
            HuSprDispOff(mes->grpId[0], i+2);
        }
        HuSprPosSet(mes->grpId[0], 1, 24+(-((no*48)+20)/2), 32);
        for(x=72+(-((no*48)+20)/2), i=0; i<no; i++) {
            HuSprPosSet(mes->grpId[0], i+2, x, 32);
            if(separatorNo[0] == i) {
                x += 28;
            } else if(separatorNo[1] == i) {
                x += 48;
            } else {
                x += 48;
            }
        }
    }
    HuSprGrpScaleSet(mes->grpId[0], 0, 0);
    return TRUE;
}

BOOL GameMesMgRecordExec(GAMEMES *mes)
{
    if(!_CheckFlag(FLAG_MG_PRACTICE)) {
        mes->time += GameMesVWait;
        if(mes->time >= mes->timeMax && mes->mesMode == 0) {
            mes->stat |= GAMEMES_STAT_TIMEEND;
            mes->mesMode = -1;
            mes->angle = 0;
        }
        switch(mes->mesMode) {
            case 1:
                if(mes->time <= 20) {
                    GameMesScale = HuSin(mes->time*4.5f);
                    HuSprGrpPosSet(mes->grpId[0], 288, 240);
                    HuSprGrpScaleSet(mes->grpId[0], mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                }
                if(mes->time == 20) {
                    HuAudFXPlay(27);
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time >= 100) {
                    mes->mesMode = 0;
                    mes->timeMax = 140;
                }
                break;

            case -1:
                mes->angle += 0.1f*GameMesVWait;
                GameMesTPLvl = 1.0f-mes->angle;
                if(GameMesTPLvl <= 0.0f) {
                    GameMesTPLvl = 0;
                }
                HuSprGrpTPLvlSet(mes->grpId[0], GameMesTPLvl);
                GameMesScale = 1.0f-mes->angle;
                if(GameMesScale <= 0.0f) {
                    GameMesScale = 0;
                    mes->stat |= GAMEMES_STAT_KILL;
                }
                HuSprGrpScaleSet(mes->grpId[0], mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                break;
        }
    } else {
        mes->stat |= GAMEMES_STAT_TIMEEND;
        mes->stat |= GAMEMES_STAT_KILL;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        return FALSE;
    }
    return TRUE;
}

static char *WinnerMesNameTbl[16*6] = {
    "ﾏﾘｵ", "MARIO", "MARIO", "MARIO", "MARIO", "MARIO",
    "ﾙｲｰｼﾞ", "LUIGI", "LUIGI", "LUIGI", "LUIGI", "LUIGI",
    "ﾋﾟｰﾁ", "PEACH", "PEACH", "PEACH", "PEACH", "PEACH",
    "ﾖｯｼｰ", "YOSHI", "YOSHI", "YOSHI", "YOSHI", "YOSHI",
    "ﾜﾘｵ", "WARIO", "WARIO", "WARIO", "WARIO", "WARIO",
    "ﾃﾞｲｼﾞｰ", "DAISY", "DAISY", "DAISY", "DAISY", "DAISY",
    "ﾜﾙｲｰｼﾞ", "WALUIGI", "WALUIGI", "WALUIGI", "WALUIGI", "WALUIGI",
    "ｷﾉﾋﾟｵ", "TOAD", "TOAD", "TOAD", "TOAD", "TOAD",
    "ﾃﾚｻ", "BOO", "BUU HUU", "BOO", "BOO", "BOO",
    "ﾐﾆｸｯﾊﾟ", "KOOPA KID", "MINI BOWSER", "MINI BOWSER", "MINI BOWSER", "MINI BOWSER",
    "ｷﾉﾋﾟｺ", "TOADETTE", "TOADETTE", "TOADETTE", "TOADETTE", "TOADETTE",
    "ﾐﾆｸｯﾊﾟR", "RED K.KID", "M.BOWSER R", "MINI B.ROUGE", "MINI B.ROSSO", "MINI B.ROJO",
    "ﾐﾆｸｯﾊﾟG", "GREEN K.KID", "M.BOWSER G", "MINI B.VERT", "MINI B.VERDE", "MINI B.VERDE",
    "ﾐﾆｸｯﾊﾟB", "BLUE K.KID", "M.BOWSER B", "MINI B.BLEU", "MINI B.BLU", "MINI B.AZUL",
    "ﾄﾞﾝｷｰ", "DK", "DONKEY KONG", "DK", "DK", "DK",
    "ｸｯﾊﾟ", "BOWSER", "BOWSER", "BOWSER", "BOWSER", "BOWSER"
};

static int WinnerMesGrpInfo[16*6] = {
    255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255,
    255, 255, 48, 255, 255, 255,
    255, 80, 64, 64, 64, 64,
    255, 255, 255, 255, 255, 255,
    255, 48, 128, 64, 64, 64,
    255, 80, 128, 64, 64, 64,
    255, 64, 128, 64, 64, 64,
    255, 255, 96, 255, 255, 255,
    255, 255, 255, 255, 255, 255,
};

static s16 WinnerMesOfsTbl[2][4][5][2] = {
    {
        { { 0, -35 }, { 0, 35 } },
        { { 0, -35 }, { -144, 35 }, { 144, 35 } },
        { { 0, -105 }, { 0, 105 }, { -144, 0 }, { 144, 0 } },
        { { 0, -105 }, { -144, 0 }, { 144, 0 }, { -144, 105 }, { 144, 105 } },
    },
    {
        { { 144, 0 }, { -144, 0 } },
        { { 0, 35 }, { -144, -35 }, { 144, -35 } },
        { { 0, 105 }, { 0, -105 }, { -144, 0 }, { 144, 0 } },
        { { 0, 105 }, { -144, -105 }, { 144, -105 }, { -144, 0 }, { 144, 0 } },
    }
};

#define WINNER_OFS_LANG ((GameMesLanguageNo == 3 || GameMesLanguageNo == 4) ? 0 : 1)

BOOL GameMesMgWinInit(GAMEMES *mes, va_list args)
{
    int i;
    int charNo;
    int type;
    int nameLen;
    int charTbl[4];
    GAMEMESID strMes;
    char *nameMes;
    s16 nameFlag;
    u32 mesId;
    int temp;
    float x;
    float y;
    float ofs;
    float scale;
    int charNum;

    type = va_arg(args, int);
    if(type < 0 && type >= 6) {
        return FALSE;
    }
    mesId = MG_INST_SYS_WIN;
    strMes = GameMesStrWinCreate(mes, mesId);
    WinnerMesSprX[0] = mes->charNum*56;
    WinnerMesSprNum[0] = mes->charNum;
    if(mes->charNum <= 6) {
        scale = 1.0f;
    } else {
        scale = 6.0f/mes->charNum;
    }
    mes->scale.x = scale;
    charNum = 0;
    charTbl[0] = charTbl[1] = charTbl[2] = charTbl[3] = -1;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        charNo = va_arg(args, int);
        if(charNo >= 0) {
            charTbl[charNum] = charNo;
            charNum++;
        }
    }
    if((GameMesLanguageNo == 3 || GameMesLanguageNo == 4) && charNum == 3) {
        temp = charTbl[0];
        charTbl[0] = charTbl[2];
        charTbl[2] = charTbl[1];
        charTbl[1] = temp;
    }
    WinnerMesCharNum = 0;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        charNo = charTbl[i];
        if(charNo < 0) {
            continue;
        }
        WinnerMesLanguageNo = (GameMesLanguageNo == 0) ? 0
        : (GameMesLanguageNo == 1) ? 1
        : (GameMesLanguageNo == 2) ? 2
        : (GameMesLanguageNo == 3) ? 3
        : (GameMesLanguageNo == 4) ? 4
        : 5;
        nameMes = WinnerMesNameTbl[(charNo*6)+WinnerMesLanguageNo];
        nameFlag = 0;
        strMes = GameMesStrCreate(mes, nameMes, nameFlag);
        if(GameMesLanguageNo == 1) {
            if(charNo == 9 || charNo == 11 || charNo == 12 || charNo == 13) {
                mes->charNum++;
            }
        } else if(GameMesLanguageNo == 2) {
            if(charNo == 8 || charNo == 9 || charNo == 11 || charNo == 12 || charNo == 13 || charNo == 14) {
                mes->charNum++;
            }
        } else if(GameMesLanguageNo == 3 || GameMesLanguageNo == 4 || GameMesLanguageNo == 5) {
            if(charNo == 9 || charNo == 11 || charNo == 12 || charNo == 13) {
                mes->charNum++;
            }
        }
        WinnerMesCharNo[WinnerMesCharNum] = charNo;
        WinnerMesCharNum++;
        WinnerMesSprX[WinnerMesCharNum] = mes->charNum*56;
        WinnerMesSprNum[WinnerMesCharNum] = mes->charNum;
    }
    if(WinnerMesCharNum == 0) {
        return FALSE;
    }
    mes->charNum = WinnerMesCharNum+1;
    mes->mesMode = 1;
    mes->angle = 0;
    WinnerMesXOfs = 600;
    for(y=0, i=0; i<mes->charNum; i++) {
        if(WinnerMesCharNum == 1) {
            nameLen = WinnerMesSprX[1]/56;
            if(nameLen <= 5) {
                mes->winScale = 1;
                ofs = WinnerMesSprX[1];
            } else {
                mes->winScale = 5.0f/nameLen;
                ofs = 280;
            }
            if(i != 0) {
                FixWinnerNameOfs(mes->grpId[i], WinnerMesCharNo[i-1], mes->winScale, WinnerMesSprX[i]/56);
            }
            x = ofs+WinnerMesSprX[0]+32.0f;
            if(i == 0) {
                x = ((576.0f-x)/2)+(x-(WinnerMesSprX[0]/2));
            } else {
                x = ((576.0f-x)/2)+(ofs/2);
            }
        } else if(i == 0) {
            x = 288.0f;
        } else if(WinnerMesCharNum == 2) {
            if(WinnerMesSprX[i]/56 <= 4) {
                mes->winScale = 1;
            } else {
                mes->winScale = 4.0f/(WinnerMesSprX[i]/56);
            }
            if(i != 0) {
                FixWinnerNameOfs(mes->grpId[i], WinnerMesCharNo[i-1], mes->winScale, WinnerMesSprX[i]/56);
            }
            if(i == 1) {
                x = 272.0f-((WinnerMesSprX[1]*mes->winScale)/2);
                if(WinnerMesSprX[i]/56 < 4) {
                    x -= ((4-(WinnerMesSprX[i]/56))*56)/2.0f;
                }
            } else {
                x = 304.0f+((WinnerMesSprX[2]*mes->winScale)/2);
                if(WinnerMesSprX[i]/56 < 4) {
                    x += ((4-(WinnerMesSprX[i]/56))*56)/2.0f;
                }
            }
        } else if(WinnerMesCharNum == 3) {
            if(WinnerMesSprX[i]/56 <= 4) {
                mes->winScale = 1;
            } else {
                mes->winScale = 4.0f/(WinnerMesSprX[i]/56);
            }
            if(i != 0) {
                FixWinnerNameOfs(mes->grpId[i], WinnerMesCharNo[i-1], mes->winScale, WinnerMesSprX[i]/56);
            }
            if(i == 1) {
                x = 288;
            } else if(i == 2) {
                x = 272.0f-((WinnerMesSprX[2]*mes->winScale)/2);
                if(WinnerMesSprX[i]/56 < 4) {
                    x -= ((4-(WinnerMesSprX[i]/56))*56)/2.0f;
                }
            } else {
                x = 304.0f+((WinnerMesSprX[3]*mes->winScale)/2);
                if(WinnerMesSprX[i]/56 < 4) {
                    x += ((4-(WinnerMesSprX[i]/56))*56)/2.0f;
                }
            }
        } else if(WinnerMesCharNum == 4) {
            if(WinnerMesSprX[i]/56 <= 4) {
                mes->winScale = 1;
            } else {
                mes->winScale = 4.0f/(WinnerMesSprX[i]/56);
            }
            if(i != 0) {
                FixWinnerNameOfs(mes->grpId[i], WinnerMesCharNo[i-1], mes->winScale, WinnerMesSprX[i]/56);
            }
            if((i%2) != 0) {
                x = 272.0f-((WinnerMesSprX[i]*mes->winScale)/2);
                if(WinnerMesSprX[i]/56 < 4) {
                    x -= ((4-(WinnerMesSprX[i]/56))*56)/2.0f;
                }
            } else {
                x = 304.0f+((WinnerMesSprX[i]*mes->winScale)/2);
                if(WinnerMesSprX[i]/56 < 4) {
                    x += ((4-(WinnerMesSprX[i]/56))*56)/2.0f;
                }
            }
        }
        WinnerMesNameX[i] = x;
        if(WinnerMesCharNum == 1) {
            switch(omcurovl) {
                case DLL_m603dll:
                case DLL_m606dll:
                case DLL_m614dll:
                case DLL_m615dll:
                case DLL_m617dll:
                case DLL_m619dll:
                case DLL_m621dll:
                case DLL_m624dll:
                case DLL_m626dll:
                case DLL_m629dll:
                case DLL_m631dll:
                case DLL_m632dll:
                case DLL_m648dll:
                case DLL_m651dll:
                case DLL_m652dll:
                case DLL_m660dll:
                case DLL_m661dll:
                case DLL_m663dll:
                case DLL_m665dll:
                case DLL_m667dll:
                case DLL_m670dll:
                    WinnerMesNameY[i] = 123;
                    break;

                case DLL_m605dll:
                case DLL_m607dll:
                case DLL_m608dll:
                case DLL_m609dll:
                case DLL_m610dll:
                case DLL_m612dll:
                case DLL_m616dll:
                case DLL_m618dll:
                case DLL_m625dll:
                case DLL_m627dll:
                case DLL_m633dll:
                case DLL_m634dll:
                case DLL_m650dll:
                case DLL_m653dll:
                case DLL_m654dll:
                case DLL_m655dll:
                case DLL_m656dll:
                case DLL_m657dll:
                case DLL_m658dll:
                case DLL_m659dll:
                case DLL_m662dll:
                    WinnerMesNameY[i] = 357;
                    break;

                case DLL_m601dll:
                case DLL_m613dll:
                    WinnerMesNameY[i] = 357;
                    WinnerMesNameY[i] += 50.0f;
                    break;

                case DLL_m604dll:
                    if(GWMgTimeGet() == 0) {
                        WinnerMesNameY[i] = 357;
                        break;
                    }
                    WinnerMesNameY[i] = 240.0f+WinnerMesOfsTbl[WINNER_OFS_LANG][WinnerMesCharNum-1][i][1];
                    break;

                default:
                    WinnerMesNameY[i] = 240.0f+WinnerMesOfsTbl[WINNER_OFS_LANG][WinnerMesCharNum-1][i][1];
                    break;
            }
        } else if(WinnerMesCharNum == 2) {
            switch(omcurovl) {
                case DLL_m603dll:
                case DLL_m613dll:
                case DLL_m614dll:
                case DLL_m619dll:
                case DLL_m621dll:
                case DLL_m635dll:
                case DLL_m644dll:
                case DLL_m645dll:
                case DLL_m668dll:
                    WinnerMesNameY[i] = 73;
                    if(i == 0) {
                        WinnerMesNameY[0] += 70.0f;
                    }
                    break;

                case DLL_m601dll:
                case DLL_m605dll:
                case DLL_m607dll:
                case DLL_m608dll:
                case DLL_m609dll:
                case DLL_m616dll:
                case DLL_m637dll:
                case DLL_m638dll:
                case DLL_m640dll:
                case DLL_m643dll:
                    WinnerMesNameY[i] = 407;
                    if(i != 0) {
                        WinnerMesNameY[i] -= 70.0f;
                    }
                    break;

                case DLL_m604dll:
                    if(GWMgTimeGet() == 0) {
                        WinnerMesNameY[i] = 407;
                        if(i != 0) {
                            WinnerMesNameY[i] -= 70.0f;
                        }
                        break;
                    }
                    WinnerMesNameY[i] = 240.0f+WinnerMesOfsTbl[WINNER_OFS_LANG][WinnerMesCharNum-1][i][1];
                    break;

                case DLL_m615dll:
                case DLL_m636dll:
                    WinnerMesNameY[i] = 73;
                    if(i == 0) {
                        WinnerMesNameY[0] += 70.0f;
                    }
                    WinnerMesNameY[i] += 50.0f;
                    break;

                default:
                    WinnerMesNameY[i] = 240.0f+WinnerMesOfsTbl[WINNER_OFS_LANG][WinnerMesCharNum-1][i][1];
                    break;
            }
        } else {
            WinnerMesNameY[i] = 240.0f+WinnerMesOfsTbl[WINNER_OFS_LANG][WinnerMesCharNum-1][i][1];
        }
        x = WinnerMesNameX[i];
        y = WinnerMesNameY[i];
        if(WinnerMesCharNum == 1) {
            if(GameMesLanguageNo == 3 || GameMesLanguageNo == 4) {
                if(i == 1) {
                    y += WinnerMesXOfs;
                }
            } else {
                if(i == 1) {
                    x -= WinnerMesXOfs;
                }
            }
        } else if(WinnerMesCharNum == 2) {
            if(i == 1) {
                x -= WinnerMesXOfs;
            } else if(i == 2) {
                x += WinnerMesXOfs;
            }
        } else if(WinnerMesCharNum == 3) {
            if(GameMesLanguageNo == 3 || GameMesLanguageNo == 4) {
                if(i == 1) {
                    y += WinnerMesXOfs;
                } else if(i == 2) {
                    x -= WinnerMesXOfs;
                } else if(i == 3) {
                    x += WinnerMesXOfs;
                }
            } else {
                if(i == 1) {
                    y -= WinnerMesXOfs;
                } else if(i == 2) {
                    x -= WinnerMesXOfs;
                } else if(i == 3) {
                    x += WinnerMesXOfs;
                }
            }
        } else if(WinnerMesCharNum == 4) {
            if(i == 1) {
                x -= WinnerMesXOfs;
            } else if(i == 2) {
                x += WinnerMesXOfs;
            } else if(i == 3) {
                x -= WinnerMesXOfs;
            } else if(i == 4) {
                x += WinnerMesXOfs;
            }
        }
        HuSprGrpPosSet(mes->grpId[i], x, y);
        if(i == 0) {
            HuSprGrpScaleSet(mes->grpId[0], 0, 0);
        } else {
            HuSprGrpScaleSet(mes->grpId[i], mes->winScale, 1.0f);
        }
        for(charNo=0; charNo<WinnerMesSprNum[i]; charNo++) {
            HuSprPriSet(mes->grpId[i], charNo, 2);
        }
    }
    return TRUE;
}

BOOL GameMesMgWinExec(GAMEMES *mes)
{
    int i;
    float x;
    float y;
    float t;
    float scale;
    float zRot;

    mes->time += GameMesVWait;
    if(mes->time >= mes->timeMax && mes->mesMode == 0) {
        mes->stat |= GAMEMES_STAT_TIMEEND;
        mes->mesMode = -1;
        mes->angle = 0;
    }
    switch(mes->mesMode) {
        case 1:
            if(mes->time <= 20) {
                t = mes->time;
                zRot = (t/20.0f)*360.0f;
                HuSprGrpZRotSet(mes->grpId[0], zRot);
                scale = HuSin((t/20.0f)*90.0f);
                HuSprGrpScaleSet(mes->grpId[0], mes->scale.x*scale, mes->scale.y*scale);
                WinnerMesXOfs -= 30;
                for(i=1; i<mes->charNum; i++) {
                    x = WinnerMesNameX[i];
                    y = WinnerMesNameY[i];
                    if(WinnerMesCharNum == 1) {
                        if(GameMesLanguageNo == 3 || GameMesLanguageNo == 4) {
                            if(i == 1) {
                                y += WinnerMesXOfs;
                            }
                        } else {
                            if(i == 1) {
                                x -= WinnerMesXOfs;
                            }
                        }
                    } else if(WinnerMesCharNum == 2) {
                        if(i == 1) {
                            x -= WinnerMesXOfs;
                        } else if(i == 2) {
                            x += WinnerMesXOfs;
                        }
                    } else if(WinnerMesCharNum == 3) {
                        if(GameMesLanguageNo == 3 || GameMesLanguageNo == 4) {
                            if(i == 1) {
                                y += WinnerMesXOfs;
                            } else if(i == 2) {
                                x -= WinnerMesXOfs;
                            } else if(i == 3) {
                                x += WinnerMesXOfs;
                            }
                        } else {
                            if(i == 1) {
                                y -= WinnerMesXOfs;
                            } else if(i == 2) {
                                x -= WinnerMesXOfs;
                            } else if(i == 3) {
                                x += WinnerMesXOfs;
                            }
                        }
                    } else if(WinnerMesCharNum == 4) {
                        if(i == 1) {
                            x -= WinnerMesXOfs;
                        } else if(i == 2) {
                            x += WinnerMesXOfs;
                        } else if(i == 3) {
                            x -= WinnerMesXOfs;
                        } else if(i == 4) {
                            x += WinnerMesXOfs;
                        }
                    }
                    HuSprGrpPosSet(mes->grpId[i], x, y);
                }
            }
            if(mes->time == 140) {
                mes->mesMode = 0;
                mes->timeMax = 170;
            }
            break;

        case -1:
            mes->angle += GameMesVWait*0.1f;
            GameMesTPLvl = 1.0f-mes->angle;
            if(GameMesTPLvl <= 0) {
                GameMesTPLvl = 0;
                mes->stat |= GAMEMES_STAT_KILL;
            }
            for(i=0; i<mes->charNum; i++) {
                HuSprGrpTPLvlSet(mes->grpId[i], GameMesTPLvl);
            }
            break;

        default:
            break;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        return FALSE;
    }
    return TRUE;
}

static void FixWinnerNameOfs(int grpId, int charNo, float scale, int sprNum)
{
    int i;
    HUSPRITE *sp;
    BOOL needFix = FALSE;
    if(GameMesLanguageNo == 1) {
        if(charNo == 9) {
            needFix = TRUE;
        }
    } else if(GameMesLanguageNo == 2) {
        if(charNo == 8 || charNo == 9 || charNo == 14) {
            needFix = TRUE;
        }
    } else if(GameMesLanguageNo == 3 || GameMesLanguageNo == 4 || GameMesLanguageNo == 5) {
        if(charNo == 9) {
            needFix = TRUE;
        }
    }
    if(needFix) {
        int num = (WinnerMesGrpInfo[(charNo*6)+WinnerMesLanguageNo] & 0xF0) >> 4;
        for(i=0; i<num; i++) {
            sp = &HuSprData[HuSprGrpData[grpId].sprId[i]];
            sp->pos.x += (56.0f*scale)/2;
        }
        for(i=num; i<sprNum-1; i++) {
            sp = &HuSprData[HuSprGrpData[grpId].sprId[i]];
            sp->pos.x -= (56.0f*scale)/2;
        }
    }
}

static float GetGameMesWidth(GAMEMES *mes, int type, int no, float scale)
{
    int w = 16;
    float x = mes->pos.x+(scale*(mes->scale.x*(((-mes->charNum*56)/2)+(no*56)+28)));
    if(GameMesLanguageNo == 1) {
        switch(type) {
            case 0:
                if(no < 2) {
                    x += scale*((w/2)*mes->scale.x);
                } else {
                    x -= scale*((w/2)*mes->scale.x);
                }
                break;

            case 1:
                if(no < 2) {
                    x += scale*(w*mes->scale.x);
                }
                break;

            case 2:
                if(no == 0) {
                    x += scale*(mes->scale.x*((w/2)+(w*2)));
                } else if(no == 1) {
                    x += scale*(mes->scale.x*((w/2)+(w*2)));
                } else if(no == 2) {
                    x += scale*(mes->scale.x*(w+(w/2)));
                } else if(no == 3) {
                    x += scale*((w/2)*mes->scale.x);
                } else if(no == 4) {
                    x -= scale*((w/2)*mes->scale.x);
                } else if(no == 5) {
                    x -= scale*(mes->scale.x*(w+(w/2)));
                } else if(no == 6) {
                    x -= scale*(mes->scale.x*((w/2)+(w*2)));
                }
                break;
        }
    }
    return x;
}
