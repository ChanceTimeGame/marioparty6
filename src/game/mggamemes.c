#include "game/gamemes.h"
#include "game/audio.h"
#include "game/sprite.h"

static float SprPosX[30];
static float SprPosY[30];
static float SprMainAngle[30];
static HuVec2f SprOrbitAngle[15];
static float SprRareZRot[15];
static float SdMesScale[15];
static int SdMesScaleTime[15];
static int MesUnk1[15];
static float SdMesSprPosX[15];
static float SdMesSprPosY[15];
static float SdMesTPLvl[6][15];
static int DrawMesTime[6][15];
static float DrawMesY[6][15];
static float WinnerMesSprX[3];
static int WinnerMesSprNum[5];
static int WinnerMesCharNo[4];
static float WinnerMesNameX[4];
static float WinnerMesNameY[4];

static int WinnerMesCharNum;
static int RecordMesValue;
static int SdMesWork[2];
static int GameMesLetterNum;
static int GameMesTime;
static float GameMesTPLvl;
static float GameMesScale;
static int WinnerMesXOfs;

static float GetGameMesWidth(GAMEMES *mes, int mode, int no, float scale);

/* first part (g2c1): placeholders */
BOOL GameMesMg4PInit(GAMEMES *mes, va_list args) { SprPosX[0] = SprPosY[0] = SprMainAngle[0] = 0; return TRUE; }
BOOL GameMesMg4PExec(GAMEMES *mes) { return TRUE; }
BOOL GameMesMg1Vs3Init(GAMEMES *mes, va_list args) { return TRUE; }
BOOL GameMesMg1Vs3Exec(GAMEMES *mes) { return TRUE; }
BOOL GameMesMg2Vs2Init(GAMEMES *mes, va_list args) { return TRUE; }
BOOL GameMesMg2Vs2Exec(GAMEMES *mes) { return TRUE; }
BOOL GameMesMgBattleInit(GAMEMES *mes, va_list args) { return TRUE; }
BOOL GameMesMgBattleExec(GAMEMES *mes) { return TRUE; }

/* g2c2 begin */
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
            SprOrbitAngle[i].x = SprOrbitAngle[i].y = (56+(2*mes->pos.x))-GetGameMesWidth(mes, 0, 0, 1.0f);
            x += SprOrbitAngle[i].x;
            HuSprScaleSet(mes->grpId[0], i, mes->scale.x, mes->scale.y);
        } else {
            SprOrbitAngle[i].x = 0;
            SprOrbitAngle[i].y = (2*mes->pos.x)-GetGameMesWidth(mes, 0, 0, 1.0f);
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
                                SprOrbitAngle[i].x -= SprOrbitAngle[i].y/20.0f;
                            }
                            x = SprOrbitAngle[i].x+GetGameMesWidth(mes, 0, i, 1.0f);
                            if(x < GetGameMesWidth(mes, 0, i, 1.0f)) {
                                x = GetGameMesWidth(mes, 0, i, 1.0f);
                            }
                        } else {
                            if(mes->time >= 15) {
                                SprOrbitAngle[i].x -= SprOrbitAngle[i].y/20.0f;
                            }
                            x = SprOrbitAngle[i].x+GetGameMesWidth(mes, 0, i, 1.0f);
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
                                SprOrbitAngle[i].x += SprOrbitAngle[i].y/20.0f;
                            }
                        } else {
                            SprOrbitAngle[i].x += SprOrbitAngle[i].y/20.0f;
                        }
                        x = GetGameMesWidth(mes, 2, i, 1.0f)-SprOrbitAngle[i].x;
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
            SprOrbitAngle[i].x = SprOrbitAngle[i].y = (56+(2*mes->pos.x))-GetGameMesWidth(mes, 0, 0, 1.0f);
            x += SprOrbitAngle[i].x;
            HuSprScaleSet(mes->grpId[0], i, mes->scale.x, mes->scale.y);
            SprRareZRot[i] = 600;
            HuSprZRotSet(mes->grpId[0], i, SprRareZRot[i]);
        } else {
            SprOrbitAngle[i].x = 0;
            SprOrbitAngle[i].y = (2*mes->pos.x)-GetGameMesWidth(mes, 0, 0, 1.0f);
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
                        SprOrbitAngle[i].x -= SprOrbitAngle[i].y/20.0f;
                        x = SprOrbitAngle[i].x+GetGameMesWidth(mes, 0, i, 1.0f);
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
                        SprOrbitAngle[i].x += SprOrbitAngle[i].y/20.0f;
                        x = GetGameMesWidth(mes, 2, i, 1.0f)-SprOrbitAngle[i].x;
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
                SprOrbitAngle[i].x = SprOrbitAngle[i].y = 56+mes->pos.y;
                if(i < center) {
                    y -= SprOrbitAngle[i].x;
                } else {
                    y += SprOrbitAngle[i].x;
                }
                GameMesScale = 1;
            }
            HuSprScaleSet(mes->grpId[0], i, GameMesScale, GameMesScale);
        } else {
            SprOrbitAngle[i].x = 0;
            SprOrbitAngle[i].y = 56+mes->pos.y;
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
                        SprOrbitAngle[i].x -= SprOrbitAngle[i].y/20.0f;
                        center = mes->charNum/2;
                        if(i == center) {
                            y = mes->pos.y;
                            GameMesScale = HuSin(4.5f*GameMesTime);
                            if(GameMesTime > 20) {
                                GameMesScale = 1;
                            }
                            HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                        } else if(i < center) {
                            y = mes->pos.y-SprOrbitAngle[i].x;
                            if(y > mes->pos.y) {
                                y = mes->pos.y;
                            }
                        } else {
                            y = mes->pos.y+SprOrbitAngle[i].x;
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
                        SprOrbitAngle[i].x += SprOrbitAngle[i].y/20.0f;
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
                            y = mes->pos.y-SprOrbitAngle[i].x;
                        } else {
                            y = mes->pos.y+SprOrbitAngle[i].x;
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
                SprOrbitAngle[i].x = SprOrbitAngle[i].y = 56+mes->pos.y;
                if(i < center) {
                    x -= SprOrbitAngle[i].x;
                    y -= SprOrbitAngle[i].x;
                } else {
                    x += SprOrbitAngle[i].x;
                    y -= SprOrbitAngle[i].x;
                }
                GameMesScale = 1;
            }
            HuSprScaleSet(mes->grpId[0], i, GameMesScale, GameMesScale);
        } else {
            SprOrbitAngle[i].x = 0;
            SprOrbitAngle[i].y = 56+mes->pos.y;
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
                        SprOrbitAngle[i].x -= SprOrbitAngle[i].y/20.0f;
                        center = mes->charNum/2;
                        if(i == center) {
                            y = mes->pos.y;
                            GameMesScale = HuSin(4.5f*GameMesTime);
                            if(GameMesTime > 20) {
                                GameMesScale = 1;
                            }
                            HuSprScaleSet(mes->grpId[0], i, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                        } else if(i < center) {
                            x = GetGameMesWidth(mes, 0, i, 1.0f)-SprOrbitAngle[i].x;
                            if(x > GetGameMesWidth(mes, 0, i, 1.0f)) {
                                x = GetGameMesWidth(mes, 0, i, 1.0f);
                            }
                            y = mes->pos.y-SprOrbitAngle[i].x;
                            if(y > mes->pos.y) {
                                y = mes->pos.y;
                            }
                        } else {
                            x = SprOrbitAngle[i].x+GetGameMesWidth(mes, 0, i, 1.0f);
                            if(x < GetGameMesWidth(mes, 0, i, 1.0f)) {
                                x = GetGameMesWidth(mes, 0, i, 1.0f);
                            }
                            y = mes->pos.y-SprOrbitAngle[i].x;
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
                        SprOrbitAngle[i].x += SprOrbitAngle[i].y/20.0f;
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
                            x = GetGameMesWidth(mes, 2, i, 1.0f)-SprOrbitAngle[i].x;
                            y = mes->pos.y-SprOrbitAngle[i].x;
                        } else {
                            x = SprOrbitAngle[i].x+GetGameMesWidth(mes, 2, i, 1.0f);
                            y = mes->pos.y-SprOrbitAngle[i].x;
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

/* last part (g2c3): placeholders */
BOOL GameMesMgDrawInit(GAMEMES *mes, va_list args) { return TRUE; }
BOOL GameMesMgDrawExec(GAMEMES *mes) { return TRUE; }
BOOL GameMesMgRecordInit(GAMEMES *mes, va_list args) { return TRUE; }
BOOL GameMesMgRecordExec(GAMEMES *mes) { return TRUE; }
BOOL GameMesMgWinInit(GAMEMES *mes, va_list args) { return TRUE; }
BOOL GameMesMgWinExec(GAMEMES *mes) { return TRUE; }

static float GetGameMesWidth(GAMEMES *mes, int mode, int no, float scale)
{
    return mes->pos.x+(scale*(mes->scale.x*(((-mes->charNum*56)/2)+((no*56)+28))));
}
