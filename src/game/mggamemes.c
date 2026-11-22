#include "game/gamemes.h"
#include "game/audio.h"
#include "game/sprite.h"
#include "game/gamework.h"

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

static int WinnerMesCharNum;
static int WinnerMesLanguageNo;
static int RecordMesType;
static int RecordMesValue;
static int SdMesWork[2];
static int GameMesLetterNum;
static int GameMesTime;
static float GameMesTPLvl;
static float GameMesScale;
static int WinnerMesXOfs;

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
    return TRUE;
}

BOOL GameMesMgKoopaExec(GAMEMES *mes)
{
    return TRUE;
}

BOOL GameMesMgLastInit(GAMEMES *mes, va_list args)
{
    return TRUE;
}

BOOL GameMesMgLastExec(GAMEMES *mes)
{
    return TRUE;
}

BOOL GameMesMgKettouInit(GAMEMES *mes, va_list args)
{
    return TRUE;
}

BOOL GameMesMgKettouExec(GAMEMES *mes)
{
    return TRUE;
}

BOOL GameMesMgDonkeyInit(GAMEMES *mes, va_list args)
{
    return TRUE;
}

BOOL GameMesMgDonkeyExec(GAMEMES *mes)
{
    return TRUE;
}

BOOL GameMesMg678Init(GAMEMES *mes, va_list args)
{
    return TRUE;
}

BOOL GameMesMg678Exec(GAMEMES *mes)
{
    return TRUE;
}

BOOL GameMesMgDrawInit(GAMEMES *mes, va_list args)
{
    return TRUE;
}

BOOL GameMesMgDrawExec(GAMEMES *mes)
{
    return TRUE;
}

BOOL GameMesMgRecordInit(GAMEMES *mes, va_list args)
{
    return TRUE;
}

BOOL GameMesMgRecordExec(GAMEMES *mes)
{
    return TRUE;
}

static char *WinnerMesNameTbl[16*6] = {
    "ﾏﾘｵ",
    "MARIO",
    "MARIO",
    "MARIO",
    "MARIO",
    "MARIO",

    "ﾙｲｰｼﾞ",
    "LUIGI",
    "LUIGI",
    "LUIGI",
    "LUIGI",
    "LUIGI",

    "ﾋﾟｰﾁ",
    "PEACH",
    "PEACH",
    "PEACH",
    "PEACH",
    "PEACH",

    "ﾖｯｼｰ",
    "YOSHI",
    "YOSHI",
    "YOSHI",
    "YOSHI",
    "YOSHI",

    "ﾜﾘｵ",
    "WARIO",
    "WARIO",
    "WARIO",
    "WARIO",
    "WARIO",

    "ﾃﾞｲｼﾞｰ",
    "DAISY",
    "DAISY",
    "DAISY",
    "DAISY",
    "DAISY",

    "ﾜﾙｲｰｼﾞ",
    "WALUIGI",
    "WALUIGI",
    "WALUIGI",
    "WALUIGI",
    "WALUIGI",

    "ｷﾉﾋﾟｵ",
    "TOAD",
    "TOAD",
    "TOAD",
    "TOAD",
    "TOAD",

    "ﾃﾚｻ",
    "BOO",
    "BUU HUU",
    "BOO",
    "BOO",
    "BOO",

    "ﾐﾆｸｯﾊﾟ",
    "KOOPA KID",
    "MINI BOWSER",
    "MINI BOWSER",
    "MINI BOWSER",
    "MINI BOWSER",

    "ｷﾉﾋﾟｺ",
    "TOADETTE",
    "TOADETTE",
    "TOADETTE",
    "TOADETTE",
    "TOADETTE",

    "ﾐﾆｸｯﾊﾟR",
    "RED K.KID",
    "M.BOWSER R",
    "MINI B.ROUGE",
    "MINI B.ROSSO",
    "MINI B.ROJO",

    "ﾐﾆｸｯﾊﾟG",
    "GREEN K.KID",
    "M.BOWSER G",
    "MINI B.VERT",
    "MINI B.VERDE",
    "MINI B.VERDE",

    "ﾐﾆｸｯﾊﾟB",
    "BLUE K.KID",
    "M.BOWSER B",
    "MINI B.BLEU",
    "MINI B.BLU",
    "MINI B.AZUL",

    "ﾄﾞﾝｷｰ",
    "DK",
    "DONKEY KONG",
    "DK",
    "DK",
    "DK",

    "ｸｯﾊﾟ",
    "BOWSER",
    "BOWSER",
    "BOWSER",
    "BOWSER",
    "BOWSER",

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
        { { 0, -35 }, { 0, 35 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
        { { 0, -35 }, { -144, 35 }, { 144, 35 }, { 0, 0 }, { 0, 0 } },
        { { 0, -105 }, { 0, 105 }, { -144, 0 }, { 144, 0 }, { 0, 0 } },
        { { 0, -105 }, { -144, 0 }, { 144, 0 }, { -144, 105 }, { 144, 105 } },
    },
    {
        { { 144, 0 }, { -144, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
        { { 0, 35 }, { -144, -35 }, { 144, -35 }, { 0, 0 }, { 0, 0 } },
        { { 0, 105 }, { 0, -105 }, { -144, 0 }, { 144, 0 }, { 0, 0 } },
        { { 0, 105 }, { -144, -105 }, { 144, -105 }, { -144, 0 }, { 144, 0 } },
    },
};

BOOL GameMesMgWinInit(GAMEMES *mes, va_list args)
{
    return TRUE;
}

BOOL GameMesMgWinExec(GAMEMES *mes)
{
    return TRUE;
}

static void FixWinnerNameOfs(int grpId, int charNo, float scale, int sprNum)
{
}

static float GetGameMesWidth(GAMEMES *mes, int type, int no, float scale)
{
    int ofs = 16;
    float x = mes->pos.x+(mes->scale.x*((((-mes->charNum*56)/2)+((no*56)+28))*scale));
    if(GameMesLanguageNo == 1) {
        switch(type) {
            case 0:
                if(no < 2) {
                    x += (ofs/2)*mes->scale.x*scale;
                } else {
                    x -= (ofs/2)*mes->scale.x*scale;
                }
                break;
            case 1:
                if(no < 2) {
                    x += ofs*mes->scale.x*scale;
                }
                break;
            case 2:
                if(no == 0) {
                    x += mes->scale.x*((ofs/2)+(ofs*2))*scale;
                } else if(no == 1) {
                    x += mes->scale.x*((ofs/2)+(ofs*2))*scale;
                } else if(no == 2) {
                    x += mes->scale.x*(ofs+(ofs/2))*scale;
                } else if(no == 3) {
                    x += (ofs/2)*mes->scale.x*scale;
                } else if(no == 4) {
                    x -= (ofs/2)*mes->scale.x*scale;
                } else if(no == 5) {
                    x -= mes->scale.x*(ofs+(ofs/2))*scale;
                } else if(no == 6) {
                    x -= mes->scale.x*((ofs/2)+(ofs*2))*scale;
                }
                break;
        }
    }
    return x;
}
