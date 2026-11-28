#include "REL/m670dll/m670.h"

void fn_1_3B30(void)
{
    s32 i;
    M670_COM *com;

    for (i = 0; i < 4; i++) {
        if (GwPlayerConf[i].type == 1) {
            com = &lbl_1_bss_10.com[i];
            if (lbl_1_bss_10.teamF[i] == 0) {
                com->mode = 1;
            }
            else {
                com->mode = 4;
            }
            com->playerNo = i;
            com->player = lbl_1_bss_10.player[i];
        }
    }
}

void fn_1_3BDC(void)
{
    HuVecF listPos[4];
    HuVecF pos;
    s32 list[4];
    HuVecF ofs;
    HuVecF playerPos;
    HuVecF curPos;
    HuVecF dir;
    s32 curNo;
    M670_COM *com;
    s32 action;
    s32 j;
    s32 rand;
    s32 no;
    s32 i;
    s8 *pattern;
    s32 targetNo;
    float minDist;
    float dist;
    float targetDist;
    float y;

    for (i = 0; i < 4; i++) {
        if (GwPlayerConf[i].type != 1) {
            continue;
        }
        com = &lbl_1_bss_10.com[i];
        switch (com->mode) {
            case 0:
                break;
            case 1:
                if ((lbl_1_bss_10.stageMode == 0 || lbl_1_bss_10.stageMode == 2) && lbl_1_bss_10.playerMode[lbl_1_bss_10.soloNo] == 1) {
                    switch (GwPlayerConf[com->playerNo].comDif) {
                        case 0:
                            com->timer = frandmod(180) + 60;
                            break;
                        case 1:
                            com->timer = frandmod(120) + 60;
                            break;
                        case 2:
                            com->timer = frandmod(60) + 60;
                            break;
                        case 3:
                            com->timer = 0;
                            break;
                    }
                    com->timer = frandmod(60) + 60;
                    com->mode = 2;
                }
                break;
            case 2:
                if (--com->timer == 0) {
                    if (lbl_1_bss_10.teamF[com->playerNo] == 0) {
                        com->mode = 3;
                    }
                    else {
                        com->mode = 4;
                    }
                }
                if (lbl_1_bss_10.stageMode != 0 && lbl_1_bss_10.stageMode != 2) {
                    com->mode = 4;
                }
                break;
            case 3:
                lbl_1_bss_10.shout = frandmod(6);
                fn_1_2460(lbl_1_bss_10.soloNo, 2);
                com->mode = 1;
                break;
            case 4:
                action = 0;
                if (lbl_1_bss_10.stageMode == 3) {
                    action = 1;
                }
                else {
                    rand = frandmod(100);
                    pos = com->player->actor->pos;
                    pos.y = 0.0f;
                    if (lbl_1_bss_10.stageMode != 0 && lbl_1_bss_10.stageMode != 2) {
                        switch (GwPlayerConf[com->playerNo].comDif) {
                            case 0:
                                action = (rand < 80) ? 0 : 2;
                                break;
                            case 1:
                                action = (rand < 70) ? 0 : 2;
                                break;
                            case 2:
                                action = (rand < 60) ? 0 : 2;
                                break;
                            case 3:
                                action = (rand < 30) ? 0 : 2;
                                break;
                        }
                        if (fn_1_2F44(&pos)) {
                            action = 1;
                        }
                    }
                    else {
                        switch (GwPlayerConf[com->playerNo].comDif) {
                            case 0:
                                action = (rand < 90) ? 1 : 0;
                                break;
                            case 1:
                                action = (rand < 90) ? 1 : 0;
                                break;
                            case 2:
                                action = (rand < 90) ? 1 : 0;
                                break;
                            case 3:
                                action = (rand < 90) ? 1 : 0;
                                break;
                        }
                    }
                }
                if (action == 0 || action == 2) {
                    pattern = lbl_1_data_250[lbl_1_bss_10.pattern];
                    if (action == 0) {
                        targetNo = frandmod(M670_PILLAR_NUM);
                    }
                    else {
                        playerPos = com->player->actor->pos;
                        playerPos.y = 0.0f;
                        for (no = 0, j = 0; j < M670_PILLAR_NUM; pattern++, j++) {
                            if (*pattern == lbl_1_bss_10.shout) {
                                list[no] = j;
                                listPos[no] = lbl_1_bss_10.pillarPos[j];
                                no++;
                            }
                        }
                        minDist = 99999.0f;
                        no = -1;
                        for (j = 0; j < 4; j++) {
                            VECSubtract(&listPos[j], &playerPos, &ofs);
                            dist = VECMag(&ofs);
                            if (dist < minDist) {
                                no = j;
                                minDist = dist;
                            }
                        }
                        targetNo = list[no];
                    }
                    com->targetNo = targetNo;
                    com->targetPos = lbl_1_bss_10.pillarPos[targetNo];
                    com->targetPos.y = 0.0f;
                    com->timer = 0;
                    com->jumpTime = 0;
                    com->mode = 6;
                }
                else {
                    com->timer = frandmod(60) + 30;
                    com->mode = 2;
                }
                break;
            case 5:
                break;
            case 6:
                curPos = com->player->actor->pos;
                curPos.y = dir.y = 0.0f;
                VECSubtract(&com->targetPos, &curPos, &dir);
                targetDist = VECMag(&dir);
                curNo = fn_1_2DE8(&curPos);
                y = fn_1_2EF4(curNo);
                if (100.0f != y) {
                    com->jumpTime = 30;
                }
                com->timer++;
                if (com->timer >= 120) {
                    com->mode = 4;
                }
                else if (targetDist < 40.0f) {
                    com->mode = 4;
                }
                else {
                    fn_1_1460(&dir, &dir);
                    MgPlayerPadSet(com->player, 56.0f * dir.x, -56.0f * dir.z, (com->jumpTime == 30) ? 0x100 : 0, (com->jumpTime != 0) ? 0x100 : 0);
                }
                if (com->jumpTime != 0) {
                    com->jumpTime = 0;
                }
                break;
            case 7:
                break;
        }
    }
}
