#include "game/board/audio.h"
#include "game/board/player.h"
#include "game/board/exit.h"
#include "game/gamework.h"
#include "game/flag.h"
#include "game/memory.h"
#include "game/hsfex.h"
#include "game/charman.h"
#include "game/audio.h"
#include "msm.h"

/* MP5's MBBoardNoGet; MP6 retail reads the board number through it here. */
static inline int mbBoardNoGet(void)
{
    return GwSystem.boardNo;
}

#define MB_MUS_CHAN_MAX 2
#define MB_AUD_FXOBJ_MAX 16
#define MB_AUD_FX_MAX 32
#define MBAUD_FXNO_OFF -99

typedef struct MbAudFxObj_s {
    int seId;
    int seNo;
    int vol;
    BOOL emiterF;
    BOOL pauseF;
    BOOL lockF;
    HuVecF pos;
    int *fxRef;
} MBAUDFXOBJ;

typedef struct MbAudFx_s {
    int seId;
    int delay;
    int type;
    s8 pan;
    HuVecF pos;
} MBAUDFX;

typedef struct MbMus_s {
    int streamNo;
    s16 id;
    s16 vol;
    s16 closeTime;
    s16 stopTime;
    s16 pauseTime;
    s16 pauseF;
    s16 stopF;
    HUPROCESS *proc;
} MBMUS;

typedef struct MbBoardMus_s {
    u32 boardNo;
    int musId;
    int musIdAlt;
    BOOL altF;
} MBBOARDMUS;

typedef struct MusBoardFade_s {
    int chan;
    int playChan;
    int speed;
    int fadeSpeed;
    int musId;
    BOOL pauseF;
} MUSBOARDFADE;

static MBAUDFXOBJ audFXObjData[MB_AUD_FXOBJ_MAX];
static MBAUDFX audFXData[MB_AUD_FX_MAX];
static MBMUS musData[MB_MUS_CHAN_MAX];

static int dummyId = -1;

static int musChanId[MB_MUS_CHAN_MAX];
static int audFXDelay;
static int audFXObjOnTimer;
static BOOL audFXObjOnF;
static HUPROCESS *audFXProc;
static HUPROCESS *musBoardFadeProc;

static MBBOARDMUS boardMusData[] = {
    { 0, 13, 14, TRUE },
    { 1, 15, 16, TRUE },
    { 2, 17, 18, TRUE },
    { 3, 19, 20, TRUE },
    { 4, 21, 22, TRUE },
    { 5, 23, 24, TRUE },
    { 6, 45, 45, FALSE },
    { 7, 46, 46, FALSE },
    { 8, 47, 47, FALSE },
    { 9, 37, 38, TRUE },
    { 10, 44, 44, FALSE },
    { -1, 0, 0, FALSE }
};

static u32 guideFxTbl[][2] = {
    { 0x3B4, 0x3AC },
    { 0x3B5, 0x3AD },
    { 0x3B6, 0x3AE },
    { 0x3B7, 0x3AF },
    { 0x3B8, 0x3B0 },
    { 0x3B9, 0x3B1 },
    { 0x3BA, 0x3B2 },
    { 0x3BB, 0x3B3 },
    { -1, -1 }
};

static void MusPlay(void);
static void MusPlayKill(void);
static void MusBoardFade(void);
static void AudFXMain(void);
static void AudFXMainDestroy(void);

void mbAudInit(void)
{
    int i;
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    MBAUDFX *fx = &audFXData[0];
    for(i=0; i<MB_MUS_CHAN_MAX; i++) {
        musData[i].streamNo = -1;
        musData[i].id = -1;
        musData[i].vol = 0;
        musData[i].closeTime = 0;
        musData[i].stopTime = 0;
        musData[i].stopF = FALSE;
        musData[i].pauseTime = 0;
        musData[i].pauseF = FALSE;
        musData[i].proc = NULL;
    }
    musBoardFadeProc = NULL;
    dummyId = -1;
    memset(&audFXObjData[0], 0, sizeof(audFXObjData));
    for(i=0, audFx = &audFXObjData[0]; i<MB_AUD_FXOBJ_MAX; i++, audFx++) {
        audFx->seId = -1;
        audFx->seNo = -1;
    }
    audFXProc = HuPrcChildCreate(AudFXMain, 8193, 16384, 0, mbMainProc);
    HuPrcDestructorSet2(audFXProc, AudFXMainDestroy);
    if(_CheckFlag(0x10022)) {
        audFXObjOnF = TRUE;
        audFXObjOnTimer = 1;
    } else {
        audFXObjOnF = FALSE;
        audFXObjOnTimer = 0;
    }
    audFXDelay = 0;
    memset(&audFXData[0], 0, sizeof(audFXData));
    for(i=0; i<MB_AUD_FX_MAX; i++, fx++) {
        fx->seId = -1;
    }
    mbAudFXPlay(0x452);
}

void mbMusBoardPlay(void)
{
    int i;
    int musId = boardMusData[0].musId;
    for(i=0; boardMusData[i].boardNo != -1; i++) {
        if(boardMusData[i].boardNo == mbBoardNoGet()) {
            if(boardMusData[i].altF) {
                if(!GwSystem.curTime) {
                    musId = boardMusData[i].musId;
                } else {
                    musId = boardMusData[i].musIdAlt;
                }
            } else {
                musId = boardMusData[i].musId;
            }
            break;
        }
    }
    mbMusPlay(0, musId, 127, 0);
}

void mbMusPlay(int chan, int id, s8 vol, u16 fadeSpeed)
{
    MBMUS *musP = &musData[chan];
    int streamNo;
    int chanOther;
    MSM_STREAMPARAM streamParam;
    if(musicOffF || omSysExitReq) {
        return;
    }
    streamParam.flag = MSM_STREAMPARAM_CHAN|MSM_STREAMPARAM_VOL;
    chanOther = chan ^ 1;
    if(musData[chan].streamNo != -1 && id == musData[chan].id) {
        if(mbMusStatGet(chan) == 2 || mbMusStatGet(chan) == 4) {
            return;
        }
    }
    if(musData[chan].streamNo != -1 && id == musData[chan].id && mbMusStopCheck(chan)) {
        mbMusPauseFadeOut(chan, FALSE, 0);
        return;
    }
    if(mbMusCheck(chan)) {
        if(mbMusEndCheck(chan) == FALSE) {
            mbMusFadeOut(chan);
        }
        while(mbMusCheck(chan)) {
            HuPrcVSleep();
        }
    }
    mbMusStop(chan, 0);
    if(fadeSpeed) {
        streamParam.flag |= MSM_STREAMPARAM_FADESPEED;
        streamParam.fadeSpeed = fadeSpeed;
    }
    streamParam.chan = chan*2;
    streamParam.vol = vol;
    streamNo = HuAudSStreamChanPlay(id, chan*2);
    if(streamNo >= 0) {
        int *chanP;
        musP->streamNo = streamNo;
        musP->id = id;
        musP->vol = vol;
        musP->closeTime = 3;
        musP->stopTime = 60;
        musP->stopF = FALSE;
        musP->proc = HuPrcChildCreate(MusPlay, 8193, 8192, 0, mbMainProc);
        HuPrcSetStat(musP->proc, 12);
        musP->proc->property = chanP = &musChanId[chan];
        *chanP = chan;
        HuPrcDestructorSet2(musP->proc, MusPlayKill);
    }
}

void mbMusFadeOut(int chan)
{
    MBMUS *musP = &musData[chan];
    mbMusFadeOutSpeed(chan, 1000);
}

void mbMusStop(int chan, int speed)
{
    MBMUS *musP = &musData[chan];
    HuAudSStreamStop(musP->streamNo);
    musP->streamNo = -1;
    musP->id = -1;
    musP->closeTime = 0;
    musP->stopTime = 0;
    musP->stopF = FALSE;
    musP->pauseTime = 0;
    musP->pauseF = FALSE;
}

void mbMusFadeOutSpeed(int chan, u16 speed)
{
    MBMUS *musP = &musData[chan];
    if(musP->streamNo == -1) {
        return;
    }
    if(mbMusEndCheck(chan)) {
        return;
    }
    HuAudSStreamFadeOut(musP->streamNo, speed);
    musP->stopTime = (s16)((speed*60)/1000)+5;
    musP->stopF = TRUE;
}

void mbMusStub(void)
{

}

void mbMusParamSet(int chan, s8 vol, u16 fadeSpeed)
{
    MBMUS *musP = &musData[chan];
    MSM_STREAMPARAM streamParam;
    if(musP->streamNo == -1) {
        return;
    }
    streamParam.flag = MSM_STREAMPARAM_CHAN|MSM_STREAMPARAM_VOL;
    if(fadeSpeed) {
        streamParam.flag |= MSM_STREAMPARAM_FADESPEED;
        streamParam.fadeSpeed = fadeSpeed;
    }
    if(vol < 0) {
        vol = 127;
    }
    streamParam.vol = vol;
    musP->vol = vol;
    HuAudSStreamParamSet(musP->streamNo, vol, fadeSpeed);
}

void mbMusPauseFadeOut(int chan, BOOL pauseF, int speed)
{
    MBMUS *musP = &musData[chan];
    if(musP->streamNo == -1) {
        return;
    }
    if(pauseF) {
        if(mbMusStopCheck(chan)) {
            return;
        }
    } else {
        if(mbMusStopCheck(chan) == FALSE) {
            return;
        }
    }
    musData[chan].pauseF = pauseF;
    if(speed > 0) {
        HuAudSStreamPauseFadeOut(musP->streamNo, pauseF, speed);
        musData[chan].pauseTime = 2.0f+((speed*60)/1000.0f);
    } else {
        HuAudSStreamPauseFadeOut(musP->streamNo, pauseF, 1000);
        musData[chan].pauseTime = 62;
    }
}

s32 mbMusStatGet(int chan)
{
    MBMUS *musP = &musData[chan];
    if(musP->streamNo == -1) {
        return 0;
    }
    if(musP->stopF) {
        return 1;
    }
    if(musP->pauseF) {
        return 3;
    }
    return 2;
}

void mbAudClose(void)
{
    int i;
    for(i=0; i<MB_MUS_CHAN_MAX; i++) {
        MBMUS *musP = &musData[i];
        mbMusFadeOutSpeed(i, 1000);
    }
    mbAudFXObjClose();
}

BOOL mbMusCheck(int chan)
{
    if(musData[chan].streamNo != -1) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL mbMusStopCheck(int chan)
{
    if(mbMusCheck(chan) == FALSE) {
        return FALSE;
    } else if(mbMusStatGet(chan) == 3) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL mbMusFadeCheck(int chan)
{
    if(mbMusCheck(chan) == FALSE) {
        return FALSE;
    } else if(musData[chan].pauseTime > 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL mbMusEndCheck(int chan)
{
    if(mbMusCheck(chan) == FALSE) {
        return TRUE;
    } else if(mbMusStatGet(chan) == 1 || mbMusStatGet(chan) == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int mbMusJinglePlay(s16 id)
{
    int streamNo = HuAudSStreamChanPlay(id, 2);
    return streamNo;
}

void mbMusJingleWait(int streamNo)
{
    int i;
    if(streamNo < 0) {
        return;
    }
    for(i=0; i<600; i++) {
        if(mbMusJingleStatGet(streamNo) == 0) {
            break;
        }
        HuPrcVSleep();
    }
    if(i >= 600) {
        HuAudSStreamFadeOut(streamNo, 10);
        HuPrcSleep(11);
    }
}

s32 mbMusJingleStatGet(int streamNo)
{
    if(streamNo < 0) {
        return 0;
    }
    return HuAudSStreamStatGet(streamNo);
}

static void MusPlay(void)
{
    MBMUS *musP;
    do {
        HUPROCESS *proc = HuPrcCurrentGet();
        int *chanP = proc->property;
        musP = &musData[*chanP];
        if(musP->pauseTime > 0) {
            musP->pauseTime--;
        }
        if(mbExitCheck()) {
            if(mbMusEndCheck(*chanP) == FALSE) {
                mbMusStop(*chanP, 0);
            }
            musP->streamNo = -1;
            musP->id = -1;
            musP->closeTime = 0;
            musP->stopTime = 0;
            musP->stopF = FALSE;
            musP->pauseTime = 0;
            musP->pauseF = FALSE;
            musP->proc = NULL;
            break;
        } else {
            if(musP->stopF) {
                if(--musP->stopTime < 0) {
                    mbMusStop(*chanP, 0);
                    musP->streamNo = -1;
                    musP->id = -1;
                    musP->closeTime = 0;
                    musP->stopTime = 0;
                    musP->stopF = FALSE;
                    musP->pauseTime = 0;
                    musP->pauseF = FALSE;
                    musP->proc = NULL;
                    break;
                }
            }
            if(musP->streamNo != -1) {
                switch(HuAudSStreamStatGet(musP->streamNo)) {
                    case 5:
                        mbMusStop(*chanP, 0);
                        musP->closeTime = 0;

                    case 0:
                        if(--musP->closeTime <= 0) {
                            musP->streamNo = -1;
                            musP->id = -1;
                            musP->closeTime = 0;
                            musP->stopTime = 0;
                            musP->stopF = FALSE;
                            musP->pauseTime = 0;
                            musP->pauseF = FALSE;
                            musP->proc = NULL;
                        }
                        break;
                }
                HuPrcVSleep();
            }
            if(musP == NULL) {
                break;
            }
        }
    } while(musP->streamNo != -1);
    if(musP) {
        musP->proc = NULL;
    }
    HuPrcEnd();
}

static void MusPlayKill(void)
{
    HUPROCESS *proc = HuPrcCurrentGet();
    if(proc->property) {
        proc->property = NULL;
    }
}

BOOL mbMusBoardFadeOut(int chan, int playChan, int speed, int fadeSpeed, int musId, BOOL pauseF)
{
    MUSBOARDFADE *work;
    if(musBoardFadeProc) {
        return FALSE;
    }
    musBoardFadeProc = HuPrcChildCreate(MusBoardFade, 8193, 8192, 0, mbMainProc);
    musBoardFadeProc->property = work = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MUSBOARDFADE), HU_MEMNUM_OVL);
    memset(work, 0, sizeof(MUSBOARDFADE));
    work->chan = chan;
    work->playChan = playChan;
    work->speed = speed;
    work->fadeSpeed = fadeSpeed;
    work->musId = musId;
    work->pauseF = pauseF;
    if(work->pauseF) {
        mbMusPauseFadeOut(chan, TRUE, work->speed);
    } else {
        mbMusFadeOutSpeed(chan, work->speed);
    }
    return TRUE;
}

BOOL mbMusBoardFadeCheck(void)
{
    if(musBoardFadeProc == NULL) {
        return FALSE;
    } else {
        return TRUE;
    }
}

static void MusBoardFade(void)
{
    MUSBOARDFADE *work = HuPrcCurrentGet()->property;
    int time = ((work->speed*60)/1000)+10;
    int i;
    for(i=0; i<time; i++) {
        HuPrcVSleep();
    }
    HuPrcSleep(5);
    if(work->musId == -1) {
        mbMusBoardPlay();
    } else {
        mbMusPlay(work->playChan, work->musId, 127, work->fadeSpeed);
    }
    HuMemDirectFree(work);
    musBoardFadeProc = NULL;
    HuPrcEnd();
}

void mbMusPauseSet(BOOL pauseF)
{
    if(pauseF) {
        s32 stat;
        if(musData[0].streamNo != -1) {
            HuAudSStreamPauseFadeOut(musData[0].streamNo, TRUE, 5);
        }
        if(musData[1].streamNo != -1) {
            HuAudSStreamPauseFadeOut(musData[1].streamNo, TRUE, 5);
        }
        stat = HuAudSStreamStatGet(2);
        if(stat == 2) {
            HuAudSStreamPauseFadeOut(2, TRUE, 5);
        }
    } else {
        s32 stat;
        if(musData[0].streamNo != -1 && musData[0].pauseF == FALSE) {
            HuAudSStreamPauseFadeOut(musData[0].streamNo, FALSE, 5);
        }
        if(musData[1].streamNo != -1 && musData[0].pauseF == FALSE) {
            HuAudSStreamPauseFadeOut(musData[1].streamNo, FALSE, 5);
        }
        stat = HuAudSStreamStatGet(2);
        if(stat == 3) {
            HuAudSStreamPauseFadeOut(2, FALSE, 5);
        }
    }
}

void mbAudStub1(void)
{

}

void mbAudStub2(void)
{

}

int mbAudFXObjSet(int seId)
{
    return mbAudFXObjCreate(seId, TRUE);
}

int mbAudFXObjCreate(int seId, BOOL multiF)
{
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    int i;
    if(multiF == FALSE) {
        for(i=0, audFx=&audFXObjData[0]; i<MB_AUD_FXOBJ_MAX; i++, audFx++) {
            if(audFx->seId == seId) {
                return -1;
            }
        }
    }
    for(i=0, audFx=&audFXObjData[0]; i<MB_AUD_FXOBJ_MAX; i++, audFx++) {
        if(audFx->seId == -1) {
            break;
        }
    }
    if(i >= MB_AUD_FXOBJ_MAX) {
        return -1;
    }
    if(audFXObjOnF) {
        audFx->seNo = MBAUD_FXNO_OFF;
        audFx->seId = seId;
        audFx->vol = 127;
        audFx->pauseF = FALSE;
        audFx->emiterF = FALSE;
        audFx->lockF = TRUE;
        audFx->fxRef = NULL;
        return MBAUD_FXNO_OFF;
    } else {
        audFx->seNo = HuAudFXPlay(seId);
        if(audFx->seNo < 0) {
            audFx->seNo = audFx->seId = -1;
            return -1;
        } else {
            audFx->seId = seId;
            audFx->vol = 127;
            audFx->pauseF = FALSE;
            audFx->emiterF = FALSE;
            audFx->lockF = FALSE;
            audFx->fxRef = NULL;
            return audFx->seNo;
        }
    }
}

void mbAudFXObjVolSet(int seNo, s16 vol)
{
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    int i;
    for(i=0, audFx=&audFXObjData[0]; i<MB_AUD_FXOBJ_MAX; i++, audFx++) {
        if(audFx->seNo == seNo) {
            break;
        }
    }
    if(i >= MB_AUD_FXOBJ_MAX) {
        return;
    }
    audFx->vol = vol;
    HuAudFXVolSet(audFx->seNo, audFx->vol);
}

void mbAudFXObjKill(int seNo)
{
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    int i;
    for(i=0, audFx=&audFXObjData[0]; i<MB_AUD_FXOBJ_MAX; i++, audFx++) {
        if(audFx->seNo == seNo) {
            break;
        }
    }
    if(i >= MB_AUD_FXOBJ_MAX) {
        return;
    }
    audFx->seId = -1;
    audFx->seNo = -1;
    HuAudFXFadeOut(seNo, 1000);
}

static void AudFXObjPauseSet(int seNo, BOOL pauseF)
{
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    int i;
    for(i=0, audFx=&audFXObjData[0]; i<MB_AUD_FXOBJ_MAX; i++, audFx++) {
        if(audFx->seNo == seNo) {
            break;
        }
    }
    if(i >= MB_AUD_FXOBJ_MAX) {
        return;
    }
    if(pauseF == FALSE) {
        if(audFx->pauseF != FALSE) {
            (void)i;
        } else {
            (void)i;
            return;
        }
        audFx->pauseF = FALSE;
        msmSePause(audFx->seNo, FALSE, 1000);
    } else {
        if(audFx->pauseF != FALSE) {
            (void)audFx;
        } else {
            audFx->pauseF = TRUE;
            msmSePause(audFx->seNo, TRUE, 1000);
            return;
        }
    }
}

void mbAudFXObjClose(void)
{
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    int i;
    for(i=0, audFx=&audFXObjData[0]; i<MB_AUD_FXOBJ_MAX; i++, audFx++) {
        if(audFx->seNo != -1) {
            mbAudFXObjKill(audFx->seNo);
        }
    }
}

int mbAudFXObjEmitterSet(int seId, HuVecF *pos)
{
    return mbAudFXObjEmitterCreate(seId, pos, TRUE);
}

int mbAudFXObjEmitterCreate(int seId, HuVecF *pos, BOOL multiF)
{
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    int i;
    if(multiF == FALSE) {
        for(i=0, audFx=&audFXObjData[0]; i<MB_AUD_FXOBJ_MAX; i++, audFx++) {
            if(audFx->seId == seId) {
                return -1;
            }
        }
    }
    for(i=0, audFx=&audFXObjData[0]; i<MB_AUD_FXOBJ_MAX; i++, audFx++) {
        if(audFx->seId == -1) {
            break;
        }
    }
    if(i >= MB_AUD_FXOBJ_MAX) {
        return -1;
    }
    if(audFXObjOnF) {
        audFx->seNo = MBAUD_FXNO_OFF;
        audFx->seId = seId;
        audFx->vol = 127;
        audFx->pauseF = FALSE;
        audFx->emiterF = TRUE;
        audFx->pos = *pos;
        audFx->lockF = TRUE;
        audFx->fxRef = NULL;
        return MBAUD_FXNO_OFF;
    } else {
        audFx->seNo = HuAudFXEmiterPlay(seId, pos);
        if(audFx->seNo < 0) {
            audFx->seNo = -1;
            return -1;
        } else {
            audFx->seId = seId;
            audFx->vol = 127;
            audFx->pauseF = FALSE;
            audFx->emiterF = TRUE;
            audFx->pos = *pos;
            audFx->lockF = FALSE;
            audFx->fxRef = NULL;
            return audFx->seNo;
        }
    }
}

void mbAudFXObjEmitterUpdate(int seNo, HuVecF *pos)
{
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    int i;
    for(i=0, audFx=&audFXObjData[0]; i<MB_AUD_FXOBJ_MAX; i++, audFx++) {
        if(audFx->seNo == seNo && audFx->emiterF) {
            break;
        }
    }
    if(i >= MB_AUD_FXOBJ_MAX) {
        return;
    }
    audFx->pos = *pos;
    if(!audFXObjOnF) {
        HuAudFXEmiterUpDate(audFx->seNo, &audFx->pos);
    }
}

void mbAudFXObjEmiterPosSet(int seId, HuVecF *pos)
{
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    int i;
    for(i=0, audFx=&audFXObjData[0]; i<MB_AUD_FXOBJ_MAX; i++, audFx++) {
        if(audFx->seId == seId && audFx->emiterF && audFx->lockF) {
            break;
        }
    }
    if(i >= MB_AUD_FXOBJ_MAX) {
        return;
    }
    audFx->pos = *pos;
}

void mbAudFXObjRefSet(int seId, int *fxRef)
{
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    int i;
    for(i=0, audFx=&audFXObjData[0]; i<MB_AUD_FXOBJ_MAX; i++, audFx++) {
        if(audFx->seId == seId && audFx->lockF) {
            break;
        }
    }
    if(i >= MB_AUD_FXOBJ_MAX) {
        return;
    }
    audFx->fxRef = fxRef;
}

static void AudFXMain(void)
{
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    MBAUDFX *fx = &audFXData[0];
    int i;
    do {
        for(fx=&audFXData[0], i=0; i<MB_AUD_FX_MAX; i++, fx++) {
            if(fx->seId < 0) {
                continue;
            }
            if(--fx->delay > 0) {
                continue;
            }
            switch(fx->type) {
                case 0:
                    HuAudFXPlay(fx->seId);
                    break;

                case 1:
                    HuAudFXPlayPan(fx->seId, fx->pan);
                    break;

                case 2:
                    HuAudFXEmiterPlay(fx->seId, &fx->pos);
                    break;
            }
            fx->seId = -1;
            fx->delay = -1;
            fx->type = 0;
        }
        for(i=0, audFx=&audFXObjData[0]; i<MB_AUD_FXOBJ_MAX; i++, audFx++) {
            if(audFx->seId != -1 && audFx->seNo != -1 && audFx->lockF == FALSE && HuAudFXStatusGet(audFx->seNo) == 0) {
                audFx->seId = -1;
                audFx->seNo = -1;
            }
        }
        if(!_CheckFlag(0x10022)) {
            if(audFXObjOnF) {
                if(--audFXObjOnTimer <= 0) {
                    for(i=0, audFx=&audFXObjData[0]; i<MB_AUD_FXOBJ_MAX; i++, audFx++) {
                        if(audFx->lockF && audFx->seNo == MBAUD_FXNO_OFF) {
                            if(audFx->emiterF) {
                                audFx->seNo = HuAudFXEmiterPlay(audFx->seId, &audFx->pos);
                                if(audFx->seNo < 0) {
                                    audFx->seNo = -1;
                                    audFx->seId = -1;
                                }
                            } else {
                                audFx->seNo = HuAudFXPlay(audFx->seId);
                                if(audFx->seNo < 0) {
                                    audFx->seNo = -1;
                                    audFx->seId = -1;
                                }
                            }
                            if(audFx->fxRef != NULL) {
                                *audFx->fxRef = audFx->seNo;
                            }
                            audFx->lockF = FALSE;
                        }
                        if(audFx->seId != -1 && audFx->seNo != -1) {
                            AudFXObjPauseSet(audFx->seNo, FALSE);
                        }
                    }
                }
            }
            if(audFXObjOnTimer <= 0) {
                audFXObjOnF = FALSE;
            }
        } else {
            if(audFXObjOnF == FALSE) {
                for(i=0, audFx=&audFXObjData[0]; i<MB_AUD_FXOBJ_MAX; i++, audFx++) {
                    if(audFx->seId != -1 && audFx->seNo != -1) {
                        AudFXObjPauseSet(audFx->seNo, TRUE);
                    }
                }
            }
            audFXObjOnF = TRUE;
            audFXObjOnTimer = 1;
        }
        for(i=0; i<GW_PLAYER_MAX; i++) {
            HuVecF pos;
            int pan;
            mbPlayerPosGet(i, &pos);
            pan = mbAudFXPosPanGet(&pos);
            CharModelVoicePanSet(GwPlayer[i].charNo, pan);
        }
        HuPrcVSleep();
    } while(!mbExitCheck());
    HuPrcEnd();
}

static void AudFXMainDestroy(void)
{
    mbAudFXObjClose();
    audFXProc = NULL;
}

static void AudFXDelayAdd(s16 seId, int delay, int type, s8 pan, HuVecF *pos)
{
    MBAUDFX *fx = &audFXData[0];
    int i;
    for(i=0; i<MB_AUD_FX_MAX; i++, fx++) {
        if(fx->seId < 0) {
            break;
        }
    }
    if(i >= MB_AUD_FX_MAX) {
        return;
    }
    fx->seId = seId;
    fx->delay = delay;
    fx->type = type;
    fx->pan = pan;
    if(pos != NULL) {
        fx->pos = *pos;
    }
}

int mbAudFXPlay(s16 seId)
{
    int seNo = -1;
    if(audFXDelay > 0) {
        AudFXDelayAdd(seId, audFXDelay, 0, 0, NULL);
    } else {
        seNo = HuAudFXPlay(seId);
    }
    audFXDelay = 0;
    return seNo;
}

void mbAudFXStop(int seNo)
{
    HuAudFXStop(seNo);
}

void mbAudFXStopAll(int speed)
{
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    MBAUDFX *fx = &audFXData[0];
    int i;
    if(speed <= 0) {
        speed = 1000;
    }
    msmSeStopAll(FALSE, speed);
    memset(&audFXObjData[0], 0, sizeof(audFXObjData));
    for(i=0, audFx=&audFXObjData[0]; i<MB_AUD_FXOBJ_MAX; i++, audFx++) {
        audFx->seId = -1;
        audFx->seNo = -1;
    }
    memset(&audFXData[0], 0, sizeof(audFXData));
    for(i=0; i<MB_AUD_FX_MAX; i++, fx++) {
        fx->seId = -1;
    }
}

int mbAudFXPosPlay(s16 seId, HuVecF *pos)
{
    int pan;
    int seNo = -1;
    pan = mbAudFXPosPanGet(pos);
    if(audFXDelay > 0) {
        AudFXDelayAdd(seId, audFXDelay, 1, pan, NULL);
    } else {
        seNo = HuAudFXPlayPan(seId, pan);
    }
    audFXDelay = 0;
    return seNo;
}

u8 mbAudFXPosPanGet(HuVecF *pos)
{
    HuVecF pos2D;
    int pan;
    Hu3D3Dto2D(pos, 1, &pos2D);
    pan = (int)((pos2D.x*(1.0f/576.0f))*64)+32;
    if(pan < 32) {
        pan = 32;
    } else if(pan > 96) {
        pan = 96;
    }
    return pan;
}

int mbAudFXEmitterPlay(int seId, HuVecF *pos)
{
    int seNo = -1;
    if(audFXDelay > 0) {
        AudFXDelayAdd(seId, audFXDelay, 2, 0, pos);
    } else {
        seNo = HuAudFXEmiterPlay(seId, pos);
    }
    audFXDelay = 0;
    return seNo;
}

void mbAudFXVolSet(int seNo, s16 vol)
{
    HuAudFXVolSet(seNo, vol);
}

void mbAudFXPanning(int seNo, s16 pan)
{
    HuAudFXPanning(seNo, pan);
}

void mbAudFXPosPanning(int seNo, HuVecF *pos)
{
    s16 pan = mbAudFXPosPanGet(pos);
    HuAudFXPanning(seNo, pan);
}

void mbAudFXDelaySet(int delay)
{
    audFXDelay = delay;
}

int mbAudGuidePlay(s16 seId)
{
    int guideNo;
    int i;
    if(!GwSystem.curTime || !GWPartyGet()) {
        guideNo = 0;
    } else {
        guideNo = 1;
    }
    for(i=0; guideFxTbl[i][0] != -1; i++) {
        if(seId == guideFxTbl[i][0] || seId == guideFxTbl[i][1]) {
            break;
        }
    }
    if(guideFxTbl[i][guideNo] != -1) {
        return mbAudFXPlay(guideFxTbl[i][guideNo]);
    } else {
        return mbAudFXPlay(seId);
    }
}
