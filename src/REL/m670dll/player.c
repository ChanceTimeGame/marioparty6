#include "REL/m670dll/m670.h"

s8 lbl_1_data_1D8[M670_PILLAR_NUM] = { 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5 };
s8 lbl_1_data_1F0[M670_PILLAR_NUM] = { 1, 4, 5, 3, 2, 1, 0, 0, 5, 4, 1, 4, 3, 2, 2, 0, 5, 3, 5, 2, 4, 0, 1, 3 };
s8 lbl_1_data_208[M670_PILLAR_NUM] = { 2, 5, 4, 4, 1, 2, 0, 0, 3, 1, 3, 1, 4, 2, 5, 0, 3, 0, 4, 5, 1, 5, 3, 2 };
s8 lbl_1_data_220[M670_PILLAR_NUM] = { 5, 1, 0, 2, 0, 4, 3, 4, 2, 1, 1, 3, 4, 5, 2, 0, 3, 0, 1, 5, 4, 5, 2, 3 };
s8 lbl_1_data_238[M670_PILLAR_NUM] = { 4, 2, 5, 5, 3, 0, 2, 0, 4, 1, 1, 2, 5, 3, 3, 4, 0, 2, 1, 3, 4, 5, 0, 1 };

s8 *lbl_1_data_250[6] = { lbl_1_data_1D8, lbl_1_data_1F0, lbl_1_data_208, lbl_1_data_220, lbl_1_data_238, NULL };

void fn_1_2384(MGACTOR *actorP, int param)
{
    s32 playerNo = param;
    HuVecF nbt;
    HuVecF pos;

    if ((actorP->colGroundAttr & 0x80) && MgSeqModeGet() == 5) {
        nbt = *(HuVecF *)((HSF_FACE *)actorP->colObj->mesh.face->data)[actorP->colFace].nbt;
        MgActorPosGet(actorP, &pos);
        pos.x += 8.0f * nbt.x;
        pos.z += 8.0f * nbt.z;
        MgActorPosSetRaw(actorP, &pos);
    }
}

void fn_1_2460(s32 playerNo, s32 mode)
{
    MGPLAYER *player = lbl_1_bss_10.player[playerNo];
    OMOBJ *obj = lbl_1_bss_10.playerObj[playerNo];

    switch (mode) {
        case 1:
            CharMotionShiftSet(lbl_1_bss_10.charNo[playerNo], player->omObj->mtnId[0], 0.0f, 6.0f, HU3D_MOTATTR_LOOP);
            break;
        case 2:
            CharMotionShiftSet(lbl_1_bss_10.charNo[playerNo], player->omObj->mtnId[7], 0.0f, 6.0f, HU3D_MOTATTR_NONE);
            break;
        case 3:
            CharMotionShiftSet(lbl_1_bss_10.charNo[playerNo], player->omObj->mtnId[8], 0.0f, 6.0f, HU3D_MOTATTR_LOOP);
            break;
        case 4:
            CharMotionShiftSet(lbl_1_bss_10.charNo[playerNo], player->omObj->mtnId[9], 0.0f, 6.0f, HU3D_MOTATTR_LOOP);
            break;
        case 5:
            CharMotionShiftSet(lbl_1_bss_10.charNo[playerNo], player->omObj->mtnId[0], 0.0f, 6.0f, HU3D_MOTATTR_LOOP);
            break;
        case 6:
            MgPlayerDespawn(player);
            Hu3DModelAttrSet(player->actor->mdlId, HU3D_ATTR_DISPOFF);
            break;
    }
    lbl_1_bss_10.playerMode[playerNo] = mode;
}

void fn_1_2690(OMOBJ *obj)
{
    s32 playerNo = obj->work[0];
    MGPLAYER *player = lbl_1_bss_10.player[playerNo];
    HuVecF pos;

    switch (lbl_1_bss_10.playerMode[playerNo]) {
        case 2:
            /* retail tests Hu3DMotionShiftIDGet(mdlId < 0), not Hu3DMotionShiftIDGet(mdlId) < 0 */
            if (Hu3DMotionShiftIDGet(player->actor->mdlId < 0) && Hu3DMotionEndCheck(player->actor->mdlId)
                && player->omObj->mtnId[7] == Hu3DMotionIDGet(player->actor->mdlId)) {
                fn_1_2460(playerNo, 3);
                fn_1_2A24(4);
            }
            break;
        case 3:
            if (Hu3DMotionShiftIDGet(player->actor->mdlId < 0)) {
                Hu3DMotionEndCheck(player->actor->mdlId);
            }
            break;
        case 4:
            Hu3DModelPosGet(lbl_1_bss_10.stageModel[2], &pos);
            pos.y -= 4.0f;
            Hu3DModelPosSetV(lbl_1_bss_10.stageModel[2], &pos);
            Hu3DMotionCalc(lbl_1_bss_10.stageModel[2]);
            Hu3DModelObjPosGet(lbl_1_bss_10.stageModel[2], "P00", &pos);
            Hu3DModelPosSetV(lbl_1_bss_10.player[lbl_1_bss_10.soloNo]->actor->mdlId, &pos);
            break;
    }
}

void fn_1_28E8(OMOBJ *obj)
{
    s32 playerNo = obj->work[0];
    MGPLAYER *player = lbl_1_bss_10.player[playerNo];
    HuVecF pos;

    switch (lbl_1_bss_10.playerMode[playerNo]) {
        case 5:
            pos = player->actor->pos;
            if (pos.y < -1000.0f) {
                fn_1_2460(playerNo, 6);
                lbl_1_bss_10.aliveMask &= ~(s32)(1 << playerNo);
                OSReport("pid ... %d : retire\n", playerNo);
            }
            break;
    }
}
