#include "REL/m635dll/m635.h"
#include "math.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* .bss is laid out in reverse definition order */
OMOBJMAN *lbl_1_bss_64;
s32 lbl_1_bss_60; /* unreferenced */
M635_WORK lbl_1_bss_4;
s32 lbl_1_bss_0;

typedef struct M635BtnTbl_s {
    u16 btn[6];
    char name[6];
} M635_BTNTBL;

M635_BTNTBL lbl_1_data_0 = {
    { 0x100, 0x200, 0x400, 0x800, 0x4000, 0x2000 },
    { 'A', 'B', 'X', 'Y', 'L', 'R' }
};

s16 lbl_1_data_12[4][2] = {
    { 60, 30 },
    { 55, 25 },
    { 40, 15 },
    { 32, 6 },
};

OM_CAMERA_VIEW lbl_1_data_24[3] = {
    { { 0.0f, 180.0f, -500.0f }, { -13.0f, 0.0f, 0.0f }, 1500.0f },
    { { -200.0f, 180.0f, -320.0f }, { -13.0f, 0.0f, 0.0f }, 1500.0f },
    { { 200.0f, 180.0f, -320.0f }, { -13.0f, 0.0f, 0.0f }, 1500.0f },
};

MGSEQ_PARAM lbl_1_data_78 = {
    300,
    0,
    fn_1_F0,
    fn_1_134,
    fn_1_164,
    fn_1_1AC,
    fn_1_224,
    fn_1_2D0,
    fn_1_3D8,
    fn_1_3DC,
    fn_1_3E0,
};

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

void ObjectSetup(void)
{
    lbl_1_bss_64 = omInitObjMan(256, 0x2000);
    omGameSysInit(lbl_1_bss_64);
    MgSeqCreate(&lbl_1_data_78);
}

void fn_1_F0(s16 mode, s16 frameNo)
{
    fn_1_10A0();
    fn_1_1774(lbl_1_bss_64);
    fn_1_195C();
    fn_1_2954();
    fn_1_4114();
    fn_1_2018();
    MgSeqModeNext();
}

void fn_1_134(s16 mode, s16 frameNo)
{
    fn_1_408(frameNo);
    fn_1_2CE8();
    fn_1_3C34();
}

void fn_1_164(s16 mode, s16 frameNo)
{
    if (frameNo == 0) {
        lbl_1_bss_4.streamNo = HuAudBGMPlay(78);
    }
    fn_1_2CE8();
    fn_1_3C34();
}

void fn_1_1AC(s16 mode, s16 frameNo)
{
    s32 i;

    if (frameNo == 0) {
        for (i = 0; i < 2; i++) {
            fn_1_25EC(i, 1, 1);
        }
    }
    fn_1_954(frameNo);
    fn_1_2CE8();
    fn_1_3C34();
    fn_1_42A0();
    fn_1_2330();
}

void fn_1_224(s16 mode, s16 frameNo)
{
    s32 i;
    s32 j;

    if (frameNo == 0) {
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 2; j++) {
                fn_1_25EC(i, j, 5);
            }
        }
        if (lbl_1_bss_4.winner == -1) {
            MgSeqDrawSet();
        }
        HuAudSStreamFadeOut(lbl_1_bss_4.streamNo, 100);
    }
    fn_1_2CE8();
    fn_1_3C34();
    fn_1_42A0();
}

void fn_1_2D0(s16 mode, s16 frameNo)
{
    switch (lbl_1_bss_4.mode) {
        case 0:
            MgSeqModeChangeOff();
            WipeCreate(2, 0, 60);
            lbl_1_bss_4.mode++;
            break;
        case 1:
            if (!WipeCheck()) {
                fn_1_638();
                fn_1_448C();
                lbl_1_bss_4.mode++;
            }
            break;
        case 2:
            WipeCreate(1, 5, 60);
            lbl_1_bss_4.mode++;
            break;
        case 3:
            if (!WipeCheck()) {
                lbl_1_bss_4.mode = 0;
                fn_1_8B0();
                MgSeqModeNext();
            }
            break;
    }
    fn_1_2CE8();
    fn_1_42A0();
    fn_1_3C34();
}

void fn_1_3D8(s16 mode, s16 frameNo) { }

void fn_1_3DC(s16 mode, s16 frameNo) { }

void fn_1_3E0(s16 mode, s16 frameNo)
{
    fn_1_2014();
    fn_1_33F8();
    fn_1_2904();
}

void fn_1_408(s16 frameNo)
{
    OM_CAMERA_VIEW view;
    s32 i;
    s16 charNo;

    if (frameNo == 30) {
        if (lbl_1_bss_4.stage == 0) {
            lbl_1_bss_0 = HuAudFXPlay(1865);
        }
        else {
            lbl_1_bss_0 = HuAudFXPlay(1866);
        }
    }
    else if (frameNo == 90) {
        HuAudFXStop(lbl_1_bss_0);
    }
    switch (lbl_1_bss_4.mode) {
        case 0:
            MgSeqModeChangeOff();
            lbl_1_bss_4.mode++;
            break;
        case 1:
            if (frameNo == 5) {
                view.center.x = 0.0f;
                view.center.y = 20.0f;
                view.center.z = -400.0f;
                view.rot.x = -25.0f;
                view.rot.y = view.rot.z = 0.0f;
                view.zoom = 2500.0f;
                omCameraViewMoveSimple(&view, 120);
                lbl_1_bss_4.mode++;
            }
            break;
        case 2:
            if (omCameraViewCheck(HU3D_CAM0)) {
                for (i = 0; i < 4; i++) {
                    charNo = lbl_1_bss_BC[i].charNo;
                    fn_1_30C8(i, charNo);
                }
                for (i = 0; i < 2; i++) {
                    fn_1_33FC(i);
                }
                for (i = 0; i < 4; i++) {
                    fn_1_2F40(i);
                }
                lbl_1_bss_4.mode++;
            }
            break;
        case 3:
            MgSeqModeNext();
            lbl_1_bss_4.mode = 0;
            break;
    }
}

void fn_1_638(void)
{
    s32 i;
    M635_TEAM *team;

    for (i = 0; i < 4; i++) {
        fn_1_3218(i, 1, 0.0f);
    }
    Center.x = lbl_1_data_24[lbl_1_bss_4.winner + 1].center.x;
    Center.y = lbl_1_data_24[lbl_1_bss_4.winner + 1].center.y;
    Center.z = lbl_1_data_24[lbl_1_bss_4.winner + 1].center.z;
    CRot.x = lbl_1_data_24[lbl_1_bss_4.winner + 1].rot.x;
    CRot.y = lbl_1_data_24[lbl_1_bss_4.winner + 1].rot.y;
    CRot.z = lbl_1_data_24[lbl_1_bss_4.winner + 1].rot.z;
    CZoom = lbl_1_data_24[lbl_1_bss_4.winner + 1].zoom;
    if (lbl_1_bss_4.winner == -1) {
        team = lbl_1_bss_4.team;
        fn_1_3340();
        for (i = 0; i < 2; i++) {
            Hu3DModelAttrSet(team[i].anim[1].model, HU3D_ATTR_DISPOFF);
            Hu3DModelAttrSet(team[i].anim[2].model, HU3D_ATTR_DISPOFF);
            Hu3DModelAttrSet(team[i].anim[3].model, HU3D_ATTR_DISPOFF);
        }
    }
    else {
        for (i = 0; i < 4; i++) {
            if (lbl_1_bss_4.winner != lbl_1_bss_BC[i].grpNo) {
                Hu3DModelAttrSet(lbl_1_bss_BC[i].model, HU3D_ATTR_DISPOFF);
            }
        }
    }
}

void fn_1_8B0(void)
{
    s32 i;

    if (lbl_1_bss_4.winner == -1) {
        for (i = 0; i < 4; i++) {
            fn_1_3218(i, 3, 8.0f);
        }
    }
    else {
        for (i = 0; i < 2; i++) {
            fn_1_3218(fn_1_32E0(lbl_1_bss_4.winner, i), 2, 8.0f);
        }
    }
}

void fn_1_954(s16 frameNo)
{
    s32 i;
    s16 winner;

    for (i = 0; i < 2; i++) {
        fn_1_9B8(i);
    }
    winner = fn_1_D84();
    if (winner != -1) {
        fn_1_F44(winner);
    }
}

void fn_1_9B8(s16 team)
{
    M635_TEAM *work;
    s32 i;
    s16 player;
    s16 sum;
    s16 padNo[2];
    s16 playerNo[2];
    s16 charNo[2];
    u16 btn[2];

    work = &lbl_1_bss_4.team[team];
    for (i = 0; i < 2; i++) {
        player = lbl_1_bss_B4[team][i];
        playerNo[i] = lbl_1_bss_BC[player].playerNo;
        padNo[i] = lbl_1_bss_BC[player].padNo;
        charNo[i] = lbl_1_bss_BC[player].charNo;
        if (lbl_1_bss_BC[lbl_1_bss_B4[team][i]].type == 0) {
            btn[i] = HuPadBtnDown[padNo[i]];
            btn[i] &= (u16)~0x60;
        }
        else {
            btn[i] = fn_1_1168(team, i);
        }
    }
    switch (work->state) {
        case 0:
            if (fn_1_27E4(team, work->member) == 3) {
                break;
            }
            if (btn[work->member] != lbl_1_data_0.btn[work->btnNo[work->member]]) {
                break;
            }
            fn_1_25EC(team, work->member, 3);
            fn_1_30C8(playerNo[work->member], charNo[work->member]);
            fn_1_2F40(playerNo[work->member]);
            work->progress++;
            work->member = 1 - work->member;
            fn_1_33FC(team);
            if (work->progress >= 8) {
                work->state = 1;
                for (i = 0; i < 2; i++) {
                    work->btnNo[i] = fn_1_EC0(work->btnNo[i], work->state);
                }
            }
            else {
                work->btnNo[work->member] = fn_1_EC0(work->btnNo[work->member], work->state);
                fn_1_25EC(team, work->member, 1);
            }
            break;
        case 1:
            if (fn_1_27E4(team, 0) != 4 && fn_1_27E4(team, 0) != 3) {
                fn_1_25EC(team, 0, 4);
                fn_1_25EC(team, 1, 4);
            }
            sum = 0;
            for (i = 0; i < 2; i++) {
                if (btn[i] == lbl_1_data_0.btn[work->btnNo[i]]) {
                    fn_1_31BC(charNo[i]);
                    work->count[i]++;
                }
                sum += work->count[i];
            }
            fn_1_3738(team, sum / 10 + 9);
            break;
    }
}

s16 fn_1_D84(void)
{
    s16 i;
    s16 j;
    s16 total[2];

    if (lbl_1_bss_4.winner != -1) {
        return -1;
    }
    for (i = 0; i < 2; i++) {
        total[i] = 0;
        if (lbl_1_bss_4.team[i].state != 0) {
            for (j = 0; j < 2; j++) {
                total[i] += lbl_1_bss_4.team[i].count[j];
            }
        }
    }
    if (total[0] >= 100 && total[1] >= 100) {
        return frandmod(2);
    }
    if (total[0] >= 100) {
        return 0;
    }
    if (total[1] >= 100) {
        return 1;
    }
    return -1;
}

u16 fn_1_EC0(u16 btnNo, s16 state)
{
    s32 i;
    u16 newBtnNo;
    s16 num;

    if (state == 0) {
        num = 6;
    }
    else {
        num = 4;
    }
    for (i = 0; i < 5; i++) {
        newBtnNo = frandmod(num);
        if (newBtnNo != btnNo) {
            break;
        }
    }
    return newBtnNo;
}

void fn_1_F44(s16 winner)
{
    s32 i;
    s16 charNo[2];

    lbl_1_bss_4.winner = winner;
    fn_1_33FC(winner);
    for (i = 0; i < 2; i++) {
        charNo[i] = lbl_1_bss_BC[lbl_1_bss_B4[winner][i]].charNo;
    }
    if (winner == -1) {
        MgSeqDrawSet();
    }
    else {
        MgSeqWinnerSet(charNo[0], charNo[1], -1, -1);
        GWMgCoinBonusSet(lbl_1_bss_B4[winner][0], 10);
        GWMgCoinBonusSet(lbl_1_bss_B4[winner][1], 10);
    }
    MgSeqModeNext();
}

void fn_1_10A0(void)
{
    s32 i;

    memset(&lbl_1_bss_4, 0, sizeof(lbl_1_bss_4));
    memset(lbl_1_bss_BC, 0, sizeof(lbl_1_bss_BC));
    lbl_1_bss_4.winner = -1;
    lbl_1_bss_4.stage = GWMgTimeGet();
    for (i = 0; i < 2; i++) {
        lbl_1_bss_4.team[i].member = 1;
        lbl_1_bss_4.team[i].btnNo[1] = frandmod(6);
    }
}

u16 fn_1_1168(s16 team, s16 member)
{
    M635_TEAM *work;
    s16 player;
    s16 comDif;

    player = lbl_1_bss_B4[team][member];
    comDif = lbl_1_bss_BC[player].comDif;
    work = &lbl_1_bss_4.team[team];
    switch (work->state) {
        case 0:
            if (work->member != member) {
                return 0;
            }
            if (lbl_1_bss_BC[player].comTimer-- > 0) {
                break;
            }
            lbl_1_bss_BC[player].comTimer = fn_1_1308(comDif);
            return lbl_1_data_0.btn[work->btnNo[member]];
        case 1:
            if (lbl_1_bss_BC[player].comTimer-- > 0) {
                break;
            }
            lbl_1_bss_BC[player].comTimer = fn_1_13F8(comDif);
            return lbl_1_data_0.btn[work->btnNo[member]];
    }
    return 0;
}

s16 fn_1_1308(s16 comDif)
{
    if (comDif < 0 || comDif > 3) {
        return 0;
    }
    return 0.75 * lbl_1_data_12[comDif][0] + frandmod(lbl_1_data_12[comDif][0] / 2);
}

s16 fn_1_13F8(s16 comDif)
{
    if (comDif < 0 || comDif > 3) {
        return 0;
    }
    return 0.75 * lbl_1_data_12[comDif][1] + frandmod(lbl_1_data_12[comDif][1] / 2);
}

void fn_1_14E8(void)
{
    if (HuPadBtn[0] & PAD_BUTTON_UP) {
        CZoom -= 10.0f;
    }
    if (HuPadBtn[0] & PAD_BUTTON_DOWN) {
        CZoom += 10.0f;
    }
    if (HuPadSubStkX[0] > 20) {
        CRot.y += 1.0f;
    }
    if (HuPadSubStkX[0] < -20) {
        CRot.y -= 1.0f;
    }
    if (HuPadSubStkY[0] > 20) {
        CRot.x += 1.0f;
    }
    if (HuPadSubStkY[0] < -20) {
        CRot.x -= 1.0f;
    }
    if (HuPadStkX[0] > 5 || HuPadStkX[0] < -5) {
        Center.x += HuPadStkX[0] / 2;
    }
    if (HuPadStkY[0] > 5 || HuPadStkY[0] < -5) {
        Center.z -= HuPadStkY[0] / 2;
    }
}
