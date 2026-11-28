#include "REL/m670dll/m670.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* .bss is laid out in reverse definition order */
s32 lbl_1_bss_8;
s32 lbl_1_bss_4;
s32 lbl_1_bss_0;

MGSEQ_PARAM lbl_1_data_0 = {
    0,
    0,
    fn_1_1F8,
    fn_1_218,
    fn_1_42C,
    fn_1_5AC,
    fn_1_B4C,
    fn_1_C68,
    fn_1_12C4,
    fn_1_1428,
    fn_1_142C
};

char *lbl_1_data_28[8] = { 0 };

int _prolog(void)
{
    const VoidFunc *ctors = _ctors;
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtors = _dtors;
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void fn_1_A0(HUMC_RESPONSE *resp)
{
    if (resp->result != HUMC_RESPONSE_READY || resp->nbrWords == 0) {
        return;
    }
    if (resp->conf < 3000) {
        return;
    }
    lbl_1_data_28[1] = "a";
    if (lbl_1_bss_10.playerMode[lbl_1_bss_10.soloNo] == 1) {
        switch (resp->itemP[0]) {
            case 0:
                lbl_1_bss_10.shout = 0;
                break;
            case 1:
                lbl_1_bss_10.shout = 1;
                break;
            case 2:
                lbl_1_bss_10.shout = 2;
                break;
            case 3:
                lbl_1_bss_10.shout = 3;
                break;
            case 4:
                lbl_1_bss_10.shout = 4;
                break;
            case 5:
                lbl_1_bss_10.shout = 5;
                break;
        }
        fn_1_2460(lbl_1_bss_10.soloNo, 2);
        HuAudFXPlay(0x89A);
    }
}

void fn_1_1F8(s16 mode, s16 frameNo)
{
    MgSeqModeNext();
}

void fn_1_218(s16 mode, s16 frameNo)
{
    if (frameNo == 0) {
        lbl_1_bss_8 = 0;
        Hu3DCameraMotionStart(lbl_1_bss_10.camMot[0], 1);
        HuMCSelWinItemSet(0x450017, 6, lbl_1_bss_10.soloPadNo);
        fn_1_2A24(1);
    }
    if (frameNo == 50) {
        HuAudFXPlay(0x896);
    }
    MgActorExec();
}

s32 fn_1_2AC(s32 streamNo, s32 bgmNo)
{
    s32 result = streamNo;
    if (result == -1 && (GameMesStatGet(MgSeqGameMesIdGet()) & GAMEMES_STAT_FXPLAY)) {
        result = HuAudBGMPlay(bgmNo);
    }
    return result;
}

void fn_1_310(s32 streamNo)
{
    if (streamNo != -1) {
        HuAudSStreamFadeOut(streamNo, 100);
    }
}

void fn_1_34C(void)
{
    s32 pan;
    s32 i;
    HuVecF pos;
    HuVecF pos2D;

    for (i = 0; i < 4; i++) {
        pos = lbl_1_bss_10.player[i]->actor->pos;
        Hu3D3Dto2D(&pos, 1, &pos2D);
        pan = pos2D.x;
        pan /= 5;
        if (pan < 32) {
            pan = 32;
        }
        else if (pan > 96) {
            pan = 96;
        }
        CharModelVoicePanSet(lbl_1_bss_10.charNo[i], pan);
    }
}

void fn_1_42C(s16 mode, s16 frameNo)
{
    s32 i;

    if (frameNo == 0) {
        lbl_1_bss_10.shout = -1;
        fn_1_2A24(2);
        for (i = 0; i < M670_PILLAR_NUM; i++) {
            fn_1_22F0(i, 100.0f);
        }
    }
    lbl_1_bss_10.streamNo = fn_1_2AC(lbl_1_bss_10.streamNo, 73);
    MgActorExec();
    fn_1_34C();
}

void fn_1_5AC(s16 mode, s16 frameNo)
{
    s32 i;
    MGPLAYER *player;
    s32 stageMode;
    s32 unused = 1;
    HuVecF pos;
    HuVecF pos2;
    MGACTOR_COLMAP_POLY poly;

    if (frameNo == 0) {
        MgTimerParamSet(lbl_1_bss_10.timer, 3600, 0, 0);
        MgTimerModeOnSet(lbl_1_bss_10.timer, 1);
        MgTimerRecordDispOn(lbl_1_bss_10.timer);
        if (GwPlayerConf[lbl_1_bss_10.soloNo].type != 1) {
            HuMCListenerCreate(lbl_1_bss_10.micCtx, fn_1_A0, lbl_1_bss_10.soloPadNo);
        }
        lbl_1_bss_10.aliveMask = 0;
        for (i = 0; i < 4; i++) {
            if (lbl_1_bss_10.soloNo == i) {
                fn_1_2460(i, 1);
            }
            else {
                fn_1_2460(i, 5);
                lbl_1_bss_10.aliveMask |= 1 << i;
                OSReport("away\n");
            }
            MgPlayerAttrReset(lbl_1_bss_10.player[i], 1);
        }
        lbl_1_bss_4 = 0;
        lbl_1_bss_0 = 0;
        lbl_1_bss_10.round = 1;
    }
    fn_1_3BDC();
    stageMode = lbl_1_bss_10.stageMode;
    for (i = 0; i < 4; i++) {
        if (i == lbl_1_bss_10.soloNo) {
            continue;
        }
        player = lbl_1_bss_10.player[i];
        pos = lbl_1_bss_10.player[i]->actor->pos;
        pos2 = pos;
        pos2.y -= 350.0f;
        if ((stageMode == 0 || stageMode == 2 || MgActorColMapPolyGet(&pos, &pos2, 0xFF, &poly)) && pos.z >= -400.0f) {
            Hu3DModelShadowSet(player->actor->mdlId);
        }
        else {
            Hu3DModelShadowReset(player->actor->mdlId);
        }
    }
    if (MgTimerDoneCheck(lbl_1_bss_10.timer)) {
        lbl_1_bss_8++;
    }
    if (!lbl_1_bss_4 && MgTimerDoneCheck(lbl_1_bss_10.timer)) {
        if (GwPlayerConf[lbl_1_bss_10.soloNo].type != 1) {
            HuMCListenerKill();
        }
        for (i = 0; i < 4; i++) {
            if (lbl_1_bss_10.teamF[i] == 0) {
                MgPlayerAttrSet(lbl_1_bss_10.player[i], 1);
            }
        }
        if (lbl_1_bss_10.shout == -1) {
            fn_1_2460(lbl_1_bss_10.soloNo, 1);
        }
        lbl_1_bss_4 = 1;
    }
    if (lbl_1_bss_8 >= 60) {
        for (i = 0; i < 4; i++) {
            MgPlayerAttrSet(lbl_1_bss_10.player[i], 1);
        }
    }
    if (lbl_1_bss_8 >= 60 || lbl_1_bss_10.aliveMask == 0) {
        if (!lbl_1_bss_4) {
            if (!MgTimerDoneCheck(lbl_1_bss_10.timer)) {
                MgTimerRecordDispOff(lbl_1_bss_10.timer);
            }
            for (i = 0; i < 4; i++) {
                MgPlayerAttrSet(lbl_1_bss_10.player[i], 1);
            }
            if (!lbl_1_bss_4 && GwPlayerConf[lbl_1_bss_10.soloNo].type != 1) {
                HuMCListenerKill();
            }
        }
        MgSeqModeNext();
    }
    MgActorExec();
    fn_1_34C();
}

void fn_1_B4C(s16 mode, s16 frameNo)
{
    if (frameNo == 0) {
        fn_1_310(lbl_1_bss_10.streamNo);
    }
    MgActorExec();
    fn_1_34C();
}

void fn_1_C68(s16 mode, s16 frameNo)
{
    s32 i;
    MGPLAYER *player;

    if (frameNo == 0) {
        char **objName;
        s32 num = 0;
        s32 objNo;
        MGPLAYER *player2;
        s32 j;
        MGPLAYER *player3;
        s32 soloNo;
        s16 winner[4] = { -1, -1, -1, -1 };

        if (lbl_1_bss_10.aliveMask == 0) {
            winner[0] = lbl_1_bss_10.soloCharNo;
            soloNo = lbl_1_bss_10.soloNo;
            if (!_CheckFlag(FLAG_MG_PRACTICE)) {
                GwPlayer[soloNo].mgCoinBonus = 10;
            }
        }
        else {
            for (j = 0, i = 0; i < 4; i++) {
                if (i == lbl_1_bss_10.soloNo) {
                    continue;
                }
                winner[j++] = lbl_1_bss_10.charNo[i];
                if (!_CheckFlag(FLAG_MG_PRACTICE)) {
                    GwPlayer[i].mgCoinBonus = 10;
                }
                if (lbl_1_bss_10.aliveMask & (1 << i)) {
                    num++;
                }
            }
        }
        MgSeqWinnerSet(winner[0], winner[1], winner[2], winner[3]);
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
        for (i = 0; i < 60; i++) {
            MgActorExec();
            fn_1_34C();
            HuPrcVSleep();
        }
        fn_1_2A24(0);
        for (i = 0; i < M670_PILLAR_NUM; i++) {
            Hu3DMotionSet(lbl_1_bss_10.pillarModel[i], lbl_1_bss_10.pillarMot[i][1]);
            fn_1_22F0(i, 100.0f);
        }
        for (i = 0; i < 4; i++) {
            MgPlayerDespawn(lbl_1_bss_10.player[i]);
        }
        for (i = 0; i < 4; i++) {
            if (i == lbl_1_bss_10.soloNo) {
                continue;
            }
            player2 = lbl_1_bss_10.player[i];
            Hu3DModelShadowSet(player2->actor->mdlId);
        }
        if (lbl_1_bss_10.aliveMask != 0) {
            char *objName1[1] = { "T02" };
            char *objName2[2] = { "T04", "T05" };
            char *objName3[3] = { "T01", "T02", "T03" };
            HuVecF pos;
            switch (num) {
                case 1:
                    objName = objName1;
                    break;
                case 2:
                    objName = objName2;
                    break;
                case 3:
                    objName = objName3;
                    break;
            }
            for (objNo = 0, i = 0; i < 4; i++) {
                if (lbl_1_bss_10.aliveMask & (1 << i)) {
                    player = lbl_1_bss_10.player[i];
                    Hu3DModelObjPosGet(lbl_1_bss_10.resultPosModel, objName[objNo], &pos);
                    OSReport("pos ... %f, %f, %f\n", pos.x, pos.y, pos.z);
                    Hu3DModelAttrSet(player->actor->mdlId, HU3D_MOTATTR_LOOP);
                    CharMotionSet(player->charNo, player->omObj->mtnId[0]);
                    Hu3DModelPosSetV(player->actor->mdlId, &pos);
                    Hu3DModelRotSet(player->actor->mdlId, 0.0f, 0.0f, 0.0f);
                    objNo++;
                }
            }
        }
        for (i = 0; i < 4; i++) {
            player3 = lbl_1_bss_10.player[i];
            CharMotionSet(lbl_1_bss_10.charNo[i], player3->omObj->mtnId[0]);
        }
        if (lbl_1_bss_10.aliveMask == 0) {
            Hu3DCameraMotionStart(lbl_1_bss_10.camMot[1], 1);
        }
        else {
            Hu3DCameraMotionStart(lbl_1_bss_10.camMot[2], 1);
        }
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_PREV, 60);
        for (i = 0; i < 60; i++) {
            MgActorExec();
            fn_1_34C();
            HuPrcVSleep();
        }
        if (lbl_1_bss_10.aliveMask != 0) {
            fn_1_2460(lbl_1_bss_10.soloNo, 4);
        }
        MgSeqModeNext();
    }
}

void fn_1_12C4(s16 mode, s16 frameNo)
{
    s32 i;
    MGPLAYER *player;
    s32 motNo;

    if (frameNo == 0) {
        for (i = 0; i < 4; i++) {
            player = lbl_1_bss_10.player[i];
            if (lbl_1_bss_10.teamF[i] == 0) {
                if (lbl_1_bss_10.aliveMask == 0) {
                    CharMotionShiftSet(lbl_1_bss_10.charNo[i], player->omObj->mtnId[5], 0.0f, 6.0f, HU3D_MOTATTR_NONE);
                }
            }
            else if (lbl_1_bss_10.aliveMask & (1 << i)) {
                if (lbl_1_bss_10.aliveMask == 0) {
                    motNo = 6;
                }
                else {
                    motNo = 5;
                }
                CharMotionShiftSet(lbl_1_bss_10.charNo[i], player->omObj->mtnId[motNo], 0.0f, 6.0f, HU3D_MOTATTR_NONE);
            }
        }
    }
}

void fn_1_1428(s16 mode, s16 frameNo)
{
}

void fn_1_142C(s16 mode, s16 frameNo)
{
    if (frameNo == 0) {
        HuMCResponseResultGet();
        HuMCClose();
    }
}
