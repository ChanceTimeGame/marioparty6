#include "REL/m670dll/m670.h"

/* .bss is laid out in reverse definition order */
float lbl_1_bss_480;
float lbl_1_bss_47C;
s32 lbl_1_bss_418[M670_PILLAR_NUM];
float lbl_1_bss_3B8[M670_PILLAR_NUM];

void fn_1_2A24(s32 mode)
{
    s32 i;
    s8 *pattern;
    s8 *pattern2;
    s8 *pattern3;
    s32 no;
    HU3D_MODELID modelId;

    switch (mode) {
        case 1:
            lbl_1_bss_10.stageObj->work[0] = 0;
            lbl_1_bss_480 = -1500.0f;
            lbl_1_bss_47C = 16.0f;
            no = lbl_1_bss_10.stageObj->work[0];
            pattern = lbl_1_data_250[lbl_1_bss_10.pattern];
            for (i = 0; i < M670_PILLAR_NUM; pattern++, i++) {
                if (*pattern == no) {
                    modelId = lbl_1_bss_10.pillarModel[i];
                    Hu3DMotionSet(modelId, lbl_1_bss_10.pillarMot[i][1]);
                    Hu3DModelAttrReset(modelId, HU3D_MOTATTR_LOOP);
                }
            }
            break;
        case 2:
            pattern2 = lbl_1_data_250[lbl_1_bss_10.pattern];
            OSReport("shout ... %d\n", lbl_1_bss_10.shout);
            for (i = 0; i < M670_PILLAR_NUM; pattern2++, i++) {
                if (*pattern2 != lbl_1_bss_10.shout) {
                    Hu3DMotionSet(lbl_1_bss_10.pillarModel[i], lbl_1_bss_10.pillarMot[i][1]);
                }
            }
            lbl_1_bss_10.shout = -1;
            break;
        case 4:
            pattern3 = lbl_1_data_250[lbl_1_bss_10.pattern];
            OSReport("PILLARF_LAMP\n");
            for (i = 0; i < M670_PILLAR_NUM; pattern3++, i++) {
                if (*pattern3 != lbl_1_bss_10.shout) {
                    Hu3DMotionSet(lbl_1_bss_10.pillarModel[i], lbl_1_bss_10.pillarMot[i][3]);
                }
            }
            HuAudFXPlay(0x897);
            break;
        case 5:
            if (lbl_1_bss_10.round < 10) {
                lbl_1_bss_10.round++;
            }
            lbl_1_bss_10.stageObj->work[0] = 0;
            lbl_1_bss_480 = 100.0f;
            break;
        case 6:
            lbl_1_bss_10.stageObj->work[0] = 0;
            for (i = 0; i < M670_PILLAR_NUM; i++) {
                lbl_1_bss_3B8[i] = lbl_1_bss_480;
                lbl_1_bss_418[i] = -frandmod(60);
            }
            break;
        case 7:
            lbl_1_bss_10.stageObj->work[0] = 120;
            break;
        case 3:
            lbl_1_bss_10.stageObj->work[0] = 0;
            for (i = 0; i < M670_PILLAR_NUM; i++) {
                lbl_1_bss_3B8[i] = -1500.0f;
                lbl_1_bss_418[i] = 0;
            }
            break;
    }
    lbl_1_bss_10.stageMode = mode;
}

s32 fn_1_2DE8(HuVecF *pos)
{
    HuVecF basePos = *pos;
    s32 i;
    s32 no = -1;
    float minDist = 99999.0f;
    float dist;
    HuVecF pillarPos;
    HuVecF ofs;

    basePos.y = 0.0f;
    for (i = 0; i < M670_PILLAR_NUM; i++) {
        pillarPos = lbl_1_bss_10.pillarPos[i];
        pillarPos.y = 0.0f;
        VECSubtract(&pillarPos, &basePos, &ofs);
        dist = VECMag(&ofs);
        if (dist < minDist) {
            no = i;
            minDist = dist;
        }
    }
    return no;
}

float fn_1_2EF4(s32 no)
{
    HU3D_MODELID modelId = lbl_1_bss_10.pillarModel[no];
    HuVecF pos;

    Hu3DModelPosGet(modelId, &pos);
    return pos.y;
}

BOOL fn_1_2F44(HuVecF *pos)
{
    s32 no = fn_1_2DE8(pos);
    HU3D_MODELID modelId = lbl_1_bss_10.pillarModel[no];
    HuVecF pillarPos;

    Hu3DModelPosGet(modelId, &pillarPos);
    return (100.0f - pillarPos.y) < 1.0f;
}

void fn_1_3098(OMOBJ *obj)
{
    s32 i;
    s32 doneF;
    s32 no;
    s8 *pattern;
    HU3D_MODELID modelId;
    s8 *pattern2;
    s32 doneF2;
    s8 *pattern3;
    float y;
    float time;
    s8 *pattern4;
    s32 doneF3;
    s32 oldTime;
    s8 *pattern5;
    s32 doneF4;
    float y2;

    if (MgSeqModeGet() >= 5 && MgTimerDoneCheck(lbl_1_bss_10.timer)) {
        return;
    }
    switch (lbl_1_bss_10.stageMode) {
        case 1:
            lbl_1_bss_480 += lbl_1_bss_47C;
            if (0.0f != lbl_1_bss_47C) {
                if (lbl_1_bss_47C > 1.0f) {
                    lbl_1_bss_47C -= 0.065f;
                }
                else {
                    lbl_1_bss_47C = 1.0f;
                }
            }
            if (lbl_1_bss_480 > 100.0f) {
                lbl_1_bss_480 = 100.0f;
                if (0.0f != lbl_1_bss_47C) {
                    for (i = 0; i < 4; i++) {
                        omVibrate(i, 20, 4, 4);
                    }
                    HuAudFXPlay(0x899);
                }
                lbl_1_bss_47C = 0.0f;
            }
            for (i = 0; i < M670_PILLAR_NUM; i++) {
                fn_1_22F0(i, lbl_1_bss_480);
            }
            doneF = TRUE;
            no = obj->work[0];
            pattern = lbl_1_data_250[lbl_1_bss_10.pattern];
            switch ((s32)obj->work[1]) {
                case 0:
                    for (i = 0; i < M670_PILLAR_NUM; pattern++, i++) {
                        if (*pattern == no) {
                            modelId = lbl_1_bss_10.pillarModel[i];
                            if (!Hu3DMotionEndCheck(modelId)) {
                                doneF = FALSE;
                            }
                        }
                    }
                    if (doneF) {
                        pattern = lbl_1_data_250[lbl_1_bss_10.pattern];
                        for (i = 0; i < M670_PILLAR_NUM; pattern++, i++) {
                            if (*pattern == no) {
                                modelId = lbl_1_bss_10.pillarModel[i];
                                Hu3DMotionSet(modelId, lbl_1_bss_10.pillarMot[i][3]);
                            }
                        }
                        if (no > 2) {
                            obj->work[1] = 2;
                        }
                        else {
                            obj->work[1] = 1;
                        }
                    }
                    break;
                case 1:
                    for (i = 0; i < M670_PILLAR_NUM; pattern++, i++) {
                        if (*pattern == no) {
                            modelId = lbl_1_bss_10.pillarModel[i];
                            if (!Hu3DMotionEndCheck(modelId)) {
                                doneF = FALSE;
                            }
                        }
                    }
                    if (doneF) {
                        no = (no + 1) % 6;
                        pattern = lbl_1_data_250[lbl_1_bss_10.pattern];
                        for (i = 0; i < M670_PILLAR_NUM; pattern++, i++) {
                            if (*pattern == no) {
                                modelId = lbl_1_bss_10.pillarModel[i];
                                Hu3DMotionSet(modelId, lbl_1_bss_10.pillarMot[i][1]);
                            }
                        }
                        obj->work[0] = no;
                        obj->work[1] = 0;
                    }
                    break;
            }
            break;
        case 4:
            pattern2 = lbl_1_data_250[lbl_1_bss_10.pattern];
            doneF2 = TRUE;
            for (i = 0; i < M670_PILLAR_NUM; pattern2++, i++) {
                if (*pattern2 != lbl_1_bss_10.shout && !Hu3DMotionEndCheck(lbl_1_bss_10.pillarModel[i])) {
                    doneF2 = FALSE;
                }
            }
            fn_1_2A24(5);
            break;
        case 5:
            time = obj->work[0];
            pattern3 = lbl_1_data_250[lbl_1_bss_10.pattern];
            y = lbl_1_bss_480;
            y -= 4.0f * lbl_1_bss_10.round;
            if (y < 50.0f) {
                y = 50.0f;
            }
            lbl_1_bss_480 = y;
            for (i = 0; i < M670_PILLAR_NUM; pattern3++, i++) {
                if (*pattern3 != lbl_1_bss_10.shout) {
                    fn_1_22F0(i, lbl_1_bss_480);
                }
            }
            obj->work[0]++;
            if (obj->work[0] == 40 / lbl_1_bss_10.round) {
                fn_1_2A24(6);
            }
            break;
        case 6:
            pattern4 = lbl_1_data_250[lbl_1_bss_10.pattern];
            doneF3 = TRUE;
            if (!MgTimerDoneCheck(lbl_1_bss_10.timer)) {
                for (i = 0; i < M670_PILLAR_NUM; pattern4++, i++) {
                    if (*pattern4 != lbl_1_bss_10.shout) {
                        if (lbl_1_bss_418[i] > 0.0f) {
                            lbl_1_bss_3B8[i] -= 4.0f * lbl_1_bss_10.round * lbl_1_bss_418[i];
                            fn_1_22F0(i, lbl_1_bss_3B8[i]);
                        }
                        if (lbl_1_bss_3B8[i] >= -1500.0f) {
                            doneF3 = FALSE;
                        }
                        oldTime = lbl_1_bss_418[i];
                        lbl_1_bss_418[i] += 1.0f;
                        if (oldTime < 0 && lbl_1_bss_418[i] >= 0) {
                            fn_1_15B8(0x898, &lbl_1_bss_10.pillarPos[i]);
                        }
                    }
                }
            }
            if (doneF3) {
                fn_1_2A24(7);
            }
            break;
        case 7:
            if (obj->work[0] == 0) {
                fn_1_2A24(3);
            }
            else {
                if (obj->work[0] == 30) {
                    HuAudFXPlay(0x896);
                    HuAudFXPlay(0x89B);
                }
                obj->work[0]--;
            }
            break;
        case 3:
            pattern5 = lbl_1_data_250[lbl_1_bss_10.pattern];
            doneF4 = TRUE;
            for (i = 0; i < M670_PILLAR_NUM; pattern5++, i++) {
                if (*pattern5 != lbl_1_bss_10.shout) {
                    if (lbl_1_bss_418[i] > 0.0f) {
                        y2 = lbl_1_bss_3B8[i];
                        y2 += 0.82f * lbl_1_bss_418[i];
                        if (y2 > 100.0f) {
                            y2 = 100.0f;
                        }
                        lbl_1_bss_3B8[i] = y2;
                        fn_1_22F0(i, lbl_1_bss_3B8[i]);
                    }
                    if (100.0f != lbl_1_bss_3B8[i]) {
                        doneF4 = FALSE;
                    }
                }
                lbl_1_bss_418[i]++;
            }
            if (doneF4) {
                fn_1_2460(lbl_1_bss_10.soloNo, 1);
                fn_1_2A24(2);
                HuAudFXPlay(0x899);
            }
            else {
                obj->work[0]++;
            }
            break;
    }
}
