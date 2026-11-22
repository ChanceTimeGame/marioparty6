#include "game/gamemes.h"
#include "game/audio.h"
#include "game/frand.h"
#include "game/sprite.h"

static float SprPosX[30];
static float SprPosY[30];
static float SprMainAngle[30];
static float SprOrbitAngle[15][2];
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
BOOL GameMesMg4PInit(GAMEMES *mes, va_list args) { return TRUE; }
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

BOOL GameMesMg678Init(GAMEMES *mes, va_list args) { return TRUE; }
BOOL GameMesMg678Exec(GAMEMES *mes) { return TRUE; }
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
