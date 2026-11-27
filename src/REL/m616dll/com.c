#include "REL/m616dll/m616.h"

void fn_1_23B0(void)
{
    s32 i;
    u16 btn;
    s32 *comDif;

    for (i = 0; i < 4; i++) {
        if (GwPlayerConf[i].type == 1) {
            comDif = lbl_1_data_2B0[GwPlayerConf[i].comDif];
            if (lbl_1_bss_10.round == 0) {
                do {
                    btn = frand() % 5;
                } while (btn == 0);
            }
            else if (frand() % 100 < comDif[1]) {
                btn = lbl_1_bss_10.comChoice[i];
            }
            else {
                do {
                    btn = frand() % 5;
                } while (btn == lbl_1_bss_10.comChoice[i] || btn == 0);
            }
            lbl_1_bss_10.comChoice[i] = btn;
            lbl_1_bss_10.comTime[i] = frand() % 270;
            OSReport("com %d : btn...%d time ...%d\n", i, lbl_1_bss_10.comChoice[i], lbl_1_bss_10.comTime[i]);
        }
    }
}

s32 fn_1_2580(s32 playerNo)
{
    return lbl_1_bss_10.comChoice[playerNo];
}

u32 lbl_1_data_1F0[17] = {
    DATANUM(DATA_m616, 0x00), DATANUM(DATA_m616, 0x01), DATANUM(DATA_m616, 0x02), DATANUM(DATA_m616, 0x03),
    DATANUM(DATA_m616, 0x06), DATANUM(DATA_m616, 0x04), DATANUM(DATA_m616, 0x07), DATANUM(DATA_m616, 0x08),
    DATANUM(DATA_m616, 0x07), DATANUM(DATA_m616, 0x05), DATANUM(DATA_m616, 0x0B), DATANUM(DATA_m616, 0x09),
    DATANUM(DATA_m616, 0x0B), DATANUM(DATA_m616, 0x0A), DATANUM(DATA_m616, 0x0B), DATANUM(DATA_m616, 0x09),
    DATANUM(DATA_m616, 0x0B)
};

u32 lbl_1_data_234[6] = {
    DATANUM(DATA_m616, 0x16), DATANUM(DATA_m616, 0x17), DATANUM(DATA_m616, 0x18),
    DATANUM(DATA_m616, 0x19), DATANUM(DATA_m616, 0x1A), DATANUM(DATA_m616, 0x1B)
};

u32 lbl_1_data_24C[13] = {
    DATANUM(DATA_m616, 0x1C), DATANUM(DATA_m616, 0x1D), DATANUM(DATA_m616, 0x1E), DATANUM(DATA_m616, 0x1F),
    DATANUM(DATA_m616, 0x21), DATANUM(DATA_m616, 0x20), DATANUM(DATA_m616, 0x22), DATANUM(DATA_m616, 0x23),
    DATANUM(DATA_m616, 0x24), DATANUM(DATA_m616, 0x25), DATANUM(DATA_m616, 0x26), DATANUM(DATA_m616, 0x27),
    DATANUM(DATA_m616, 0x28)
};

unsigned int lbl_1_data_280[12] = {
    DATANUM(DATA_mariomot, 0x00), DATANUM(DATA_mariomot, 0x03), DATANUM(DATA_mariomot, 0x06),
    DATANUM(DATA_mariomot, 0x28), DATANUM(DATA_mariomot, 0x6F), DATANUM(DATA_mariomot, 0x70),
    DATANUM(DATA_mariomot, 0x04), DATANUM(DATA_mariomot, 0x24), DATANUM(DATA_mariomot, 0x25),
    DATANUM(DATA_mario, 0x9A), DATANUM(DATA_mario, 0x9B), 0
};

s32 lbl_1_data_2B0[4][2] = {
    { 80, 50 },
    { 85, 30 },
    { 90, 10 },
    { 100, 0 }
};

u32 lbl_1_data_2D0[3] = { DATANUM(DATA_m616, 0x10), DATANUM(DATA_m616, 0x11), DATANUM(DATA_m616, 0x12) };
