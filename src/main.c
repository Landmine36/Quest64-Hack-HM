#include "../include/Quest64.h"
#include "../include/PR/gbi.h"

#define MAX_LEVEL 98
#define MAX_HP 500
#define MAX_MP 500
#define MAX_AGI 255
#define MAX_DEF 255

#define BRIAN_TURN 1

extern Gfx* gMasterGfxPos;

f32 hpNightMultiplier = 1.0f;
f32 defNightMultiplier = 1.0f;
f32 agiNightMultiplier = 1.0f;
f32 atkNightMultiplier = 1.0f;
f32 powNightMultiplier = 1.0f;

s32 itemIdChosen = -1;
s32 itemIdChosenCopy = -1;
u8 itemIdsToNotRemove[] = {0xE, 0xF, 0x10, 0x11, 0x12, 0x13, 0x1E};
u8 gInventoryCopy[150] = {0};
extern u8 gInventory[150];

void MakeInventoryCopy(void) {
    s32 i;
    for (i = 0; i < sizeof(gInventoryCopy); i++) {
        gInventoryCopy[i] = gInventory[i];
    }
}

void GetInventoryCopy(void) {
    s32 i;
    for (i = 0; i < sizeof(gInventoryCopy); i++) {
        gInventory[i] = gInventoryCopy[i];
    }
}

s32 CheckNonRemovableItem(void) {
    s32 i, j;
    if (itemIdChosen != -1) {
        for (i = 0; i < sizeof(itemIdsToNotRemove); i++) {
            if (itemIdsToNotRemove[i] == itemIdChosen) {
                for (j = 0; j < sizeof(gInventoryCopy); j++) {
                    GetInventoryCopy();
                    itemIdChosen = -1;
                    return 1;
                }
            }
        }
    }
    return 0;
}

//0x8007BA74 spawns a speech bubble when set to 0x00000010
//0x8007BA90 exp gain
extern void bossSpellsDpadRight(void);
extern u16 buttonsHeld;

s32 fireElementLevelUpTextXPos[] = {135, 131, 135, 0};
s32 earthElementLevelUpTextXPos[] = {117, 112, 119, 0};
s32 waterElementLevelUpTextXPos[] = {135, 131, 135, 0};
s32 windElementLevelUpTextXPos[] = {153, 150, 153, 0};

s32 elementCapsTable[] = {10, 20, 30, 40, 40, 50, 99, 255, 255};
s32 gTotalBossesBeatenCount = 0;

void ChangeBrianFireSpells(void);
void ChangeBrianEarthSpells(void);
void ChangeBrianWaterSpells(void);
void ChangeBrianWindSpells(void);
void ChangeBrianBossSpells(void);

void DrawSliderBar(s32 xPos, s32 yPos, s32 width) {
    func_80029B58(0x25, xPos, yPos, width, 3); //arg index 0 and 4 are unknown
}

void DrawSliderBarOutline(s32 xPos, s32 yPos, s32 width) {
    func_80029B58(0x24, xPos, yPos, width, 7); //arg index 0 and 4 are unknown
}

void DrawRowHeader(s32 xPos, s32 yPos, s32 width) {
    func_80029B58(0x22, xPos, yPos, width, 0xC); //arg index 0 and 4 are unknown
}

void DrawMin(s32 xPos, s32 yPos, s32 width) {
    func_80029B58(0x2A, xPos, yPos, width, 0xB);
}

void DrawMax(s32 xPos, s32 yPos, s32 width) {
    func_80029B58(0x29, xPos, yPos, width, 0x08);
}

void DrawSlider(s32 xPos, s32 yPos, s32 width) {
    func_80029B58(0x27, xPos, yPos, width, 0xB);
}

void DrawLineDivider(s32 xPos, s32 yPos, s32 width) {
    func_80029B58(0x3D, xPos, yPos, width, 2);
}

//80028180 sets cursor position for index 0
//800282CC sets cursor position for index 1

void environmentEpilogueHook(s32 xRootOffset) {
    DrawSliderBarOutline(0x8D + xRootOffset, 0x9A, 0x54);
    DrawSliderBar(0x8E + xRootOffset, 0x9B, 0x50);
    DrawRowHeader(0x31 + xRootOffset, 0x8F, 0x34);
    DrawMin(0x6C + xRootOffset, 0x95, 0x14);
    DrawMax(0xEA + xRootOffset, 0x97, 0x18);
    DrawSlider(0xB3 + xRootOffset, 0x92, 0x8);
    DrawLineDivider(0x5D + xRootOffset, 0x84, 0xA7);
}

//element start screen drawing function
void func_80029448_Hook(s32 arg0) {
    s32 var_s0;
    s32 temp_s0;

    func_80029B58(0, arg0 + 0x28, 0x1E, 0xF0, 0x9F);
    func_80029B58(0x37, arg0 + 0x2B, 0xBE, 0xF0, 3);
    func_80029B58(0x37, arg0 + 0x119, 0x24, 2, 0x9A);
    temp_s0 = arg0 + 0x27;
    func_80029B58(0x37, temp_s0, 0x1D, 0xF2, 1);
    func_80029B58(0x37, temp_s0, 0xBD, 0xF2, 1);
    func_80029B58(0x37, temp_s0, 0x1E, 1, 0x9F);
    func_80029B58(0x37, arg0 + 0x118, 0x1E, 1, 0x9F);
    temp_s0 = arg0 + 0x29;
    func_80029B58(0x38, temp_s0, 0x1F, 0xED, 1);
    func_80029B58(0x38, temp_s0, 0x20, 1, 0x9B);
    func_80029B58(0x39, temp_s0, 0xBB, 0xEE, 1);
    func_80029B58(0x39, arg0 + 0x116, 0x1F, 1, 0x9C);
    func_80029B58(5, arg0 + 0x45, 0x1C, 7, 7);
    func_80029B58(5, arg0 + 0xF3, 0x1C, 7, 7);
    temp_s0 = AnimationXOffsets[(FramesInMenu / 4) % 16];
    func_80029B58(6, arg0 + temp_s0 + 0x36, 0x1A, 0xE, 0xB);
    func_80029B58(7, (arg0 - temp_s0) + 0xFB, 0x1A, 0xE, 0xB);
    func_80029B58(0x2D, arg0 + 0x81, 0x19, 0x3A, 0x10);
    temp_s0 = arg0 + 0xA5;
    func_80029B58(0x32, temp_s0, 0x35, 0x35, 6);
    func_80029B58(0x32, arg0 + 0x31, 0x78, 0x35, 6);
    func_80029B58(0x32, arg0 + 0xD9, 0x78, 0x35, 6);
    func_80029B58(0x32, temp_s0, 0xAE, 0x35, 6);
    temp_s0 = arg0 + 0x86;
    func_80029B58(0x14, temp_s0, 0x34, 8, 0xA);
    func_80029B58(0x14, arg0 + 0x47, 0x6B, 8, 0xA);
    func_80029B58(0x14, arg0 + 0xF0, 0x6B, 8, 0xA);
    func_80029B58(0x14, temp_s0, 0xAD, 8, 0xA);
    temp_s0 = arg0 + 0x9A;
    func_80029B58(0x2E, temp_s0, 0x3D, 0xC, 0xC);
    func_80029B58(0x2F, arg0 + 0x67, 0x6F, 0xC, 0xC);
    func_80029B58(0x30, arg0 + 0xCC, 0x6F, 0xC, 0xC);
    func_80029B58(0x31, temp_s0, 0xA1, 0xC, 0xC);
    func_80029B58(0x20, arg0 + 0xA7, 0x92, 0x14, 0xB);
    temp_s0 = arg0 + 0x8D;
    func_8002AB64(2, temp_s0, 0x34, 99, 0xA);
    func_8002AB64(2, arg0 + 0x4E, 0x6B, 99, 0xA);
    func_8002AB64(2, arg0 + 0xF7, 0x6B, 99, 0xA);
    func_8002AB64(2, temp_s0, 0xAD, 99, 0xA);
    func_8002AB64(2, arg0 + 0x78, 0x34, gPlayerData.elements.fire, 0xA);
    func_8002AB64(2, arg0 + 0x39, 0x6B, gPlayerData.elements.earth, 0xA);
    func_8002AB64(2, arg0 + 0xE2, 0x6B, gPlayerData.elements.wind, 0xA);
    func_8002AB64(2, arg0 + 0x78, 0xAD, gPlayerData.elements.water, 0xA);
    //var_s0 = arg0 + 0xA6;
    {
        s32 fireLevel = (gPlayerData.elements.fire / 100.0f + 0.01f) * 50.0f;
        s32 earthLevel = (gPlayerData.elements.earth / 100.0f + 0.01f) * 50.0f;
        s32 windLevel = (gPlayerData.elements.wind / 100.0f + 0.01f) * 50.0f;
        s32 waterLevel = (gPlayerData.elements.water / 100.0f + 0.01f) * 50.0f;


        func_80029B58(0x33, arg0 + 0xA6, 0x36, fireLevel, 3);
        func_80029B58(0x33, arg0 + 0x32, 0x79, earthLevel, 3);
        func_80029B58(0x33, arg0 + 0xDA, 0x79, windLevel, 3);
        func_80029B58(0x33, arg0 + 0xA6, 0xAF, waterLevel, 3);
    }

    

    {
        s32 expToNextLevel = ExpTable[gPlayerData.levels];
        s32 playerExpToNextLevel = expToNextLevel - gPlayerData.curExp;

        var_s0 = (gPlayerData.curExp * 100) / (u32) ExpTable[gPlayerData.levels];
        if (var_s0 > 100) {
            var_s0 = 100;
        }

        func_8002A0B8(var_s0, arg0); //draw exp sphere
    
        if (gPlayerData.curExp > ExpTable[gPlayerData.levels]) {
            func_8002AB64(6, arg0 + 0x7B, 0x92, 0, 0xA); //draw 0 exp
        } else {
            func_8002AB64(6, arg0 + 0x7B, 0x92, playerExpToNextLevel, 0xA);          
        }
    }

    if (!(D_8008FD0C & 0x2000)) {
        if ((D_80092871 >= 0x15) || (D_80092876 & 0x10)) {
            D_8008FD04 = 0x140;
            D_8008FD08 = 0;
            D_8008FD0C = D_8008FD0C | 0x2000;
            func_800268D4(0, 0, 0xFF);
            return;
        }
        if ((D_80092871 < -0x14) || (D_80092876 & 0x2000)) {
            D_8008FD04 = 0;
            D_8008FD08 = -0x140;
            D_8008FD0C = D_8008FD0C | 0x3000;
            func_800268D4(0, 0, 0xFF);
        }
    }
}

void SetCurrentBossesBeaten(void) {
    int totalBossesBeaten = 0;
    int i;

    for (i = 0; i < ARRAY_COUNT(elementCapsTable); i++) {
        if ((bossFlags >> i) & 1) {
            totalBossesBeaten++;
        }
    }
    gTotalBossesBeatenCount = totalBossesBeaten;
}

typedef struct Vec2Int {
    s32 x;
    s32 y;
} Vec2Int;

Vec2Int iconPositionsTriangle[] = {
    {16, -16},
    {0, 0},
    {16, 16},
    {32, 0},
};

Vec2Int iconPositionsVertical[] = {
    {-4, -42},
    {-4, -24},
    {-4, -6},
    {-4, 12},
};

Vec2Int iconPositionsOriginal[] = {
    {6, 6},
    {28, 6},
    {50, 6},
    {72, 6},
};

// void graphics_func(s32 s, s32 t, s32 lrx, s32 lry) {
//     gDPSetTextureImage(gMasterGfxPos++, G_IM_FMT_CI, G_IM_SIZ_8b, 0xB0, (pointerToImgDataHere));
//     gDPSetTile(gMasterGfxPos++, G_IM_FMT_CI, G_IM_SIZ_8b, ((((s + lrx) - s) + 8)) >> 3, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
//     gDPLoadSync(gMasterGfxPos++);
//     gDPLoadTile(gMasterGfxPos++, G_TX_LOADTILE, s * 4, t * 4, (s + lrx) * 4, (t + lry) * 4);
//     gDPPipeSync(gMasterGfxPos++);
//     gDPSetTile(gMasterGfxPos++, G_IM_FMT_CI, G_IM_SIZ_8b, ((((s + lrx) - s) + 8)) >> 3, 0x0000, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
//     gDPSetTileSize(gMasterGfxPos++, G_TX_RENDERTILE, s * 4, t * 4, (s + lrx) * 4, (t + lry) * 4);
// }

void func_8001EBDC_Hook(unk1ebdcs* arg0) {
    s32 i;
    void* HUDTex;

    HUDTex = &gTex_HUD_and_Menu;
    func_800210FC(HUDTex, iconPositionsTriangle[0].x - 6 , iconPositionsTriangle[0].y - 6, 0xC, 0xC, 0x20, 0x10, 0x400, 0x400);
    func_800210FC(HUDTex, iconPositionsTriangle[1].x - 6 , iconPositionsTriangle[1].y - 6, 0xC, 0xC, 0x2C, 0x10, 0x400, 0x400);
    func_800210FC(HUDTex, iconPositionsTriangle[2].x - 6 , iconPositionsTriangle[2].y - 6, 0xC, 0xC, 0x38, 0x10, 0x400, 0x400);
    func_800210FC(HUDTex, iconPositionsTriangle[3].x - 6 , iconPositionsTriangle[3].y - 6, 0xC, 0xC, 0x44, 0x10, 0x400, 0x400);

    func_80020E2C(HUDTex, 0x20, 0x1D, 0x80, 0xA); //something for setting up font
    //graphics_func(0x20, 0x1D, 0x80, 0xA);


    
    for (i = 0; i < 4; i++) {
        func_80020D4C(1, iconPositionsTriangle[i].x, iconPositionsTriangle[i].y, arg0->unk24[i]);
    }
}

s32 strLength(char* str) {
    s32 i = 0;
    while (*str != 0) {
        i++;
    }
    return i;
}

void func_80020D4C_Hook(u16 arg0, s32 xPos, s32 yPos, s32 arg3) {
    u8 *var_s1;
    u8 sp48[0xC];
    s32 var_s2;

    var_s1 = sp48;
    var_s2 = int_to_str_with_flags(arg3, sp48, arg0);
    for (;var_s2 != 0; var_s2--, var_s1++) {
        if (*var_s1 != ' ') {
            func_80020F8C(xPos, yPos, 8, 0xA, (*var_s1 * 8) + ' ', 0x1D, 0x400, 0x400);
            xPos += 7;
        } else {
            if (arg0 & 6) {
                xPos += 7;
            }
        }
    }
}

// void func_8001EBDC_Hook_DpadToggle(unk1ebdcs* arg0) {
//     s32 i;
//     void* HUDTex;

//     if (buttonsHeld & 0x0800) {
//         elementPositions = 1;
//     } else if (buttonsHeld & 0x0400) {
//         elementPositions = 2;
//     } else {
//         elementPositions = 0;
//     }

//     HUDTex = &gTex_HUD_and_Menu;
//     if (elementPositions == 1) {
//         func_800210FC(HUDTex, iconPositionsVertical[0].x - 6 , iconPositionsVertical[0].y - 6, 0xC, 0xC, 0x20, 0x10, 0x400, 0x400);
//         func_800210FC(HUDTex, iconPositionsVertical[1].x - 6 , iconPositionsVertical[1].y - 6, 0xC, 0xC, 0x2C, 0x10, 0x400, 0x400);
//         func_800210FC(HUDTex, iconPositionsVertical[2].x - 6 , iconPositionsVertical[2].y - 6, 0xC, 0xC, 0x38, 0x10, 0x400, 0x400);
//         func_800210FC(HUDTex, iconPositionsVertical[3].x - 6 , iconPositionsVertical[3].y - 6, 0xC, 0xC, 0x44, 0x10, 0x400, 0x400);
//     } else if (elementPositions == 2) {
//         func_800210FC(HUDTex, iconPositionsTriangle[0].x - 6 , iconPositionsTriangle[0].y - 6, 0xC, 0xC, 0x20, 0x10, 0x400, 0x400);
//         func_800210FC(HUDTex, iconPositionsTriangle[1].x - 6 , iconPositionsTriangle[1].y - 6, 0xC, 0xC, 0x2C, 0x10, 0x400, 0x400);
//         func_800210FC(HUDTex, iconPositionsTriangle[2].x - 6 , iconPositionsTriangle[2].y - 6, 0xC, 0xC, 0x38, 0x10, 0x400, 0x400);
//         func_800210FC(HUDTex, iconPositionsTriangle[3].x - 6 , iconPositionsTriangle[3].y - 6, 0xC, 0xC, 0x44, 0x10, 0x400, 0x400);
//     } else {
//         func_800210FC(HUDTex, iconPositionsOriginal[0].x - 6, iconPositionsOriginal[0].y - 6, 0xC, 0xC, 0x20, 0x10, 0x400, 0x400);
//         func_800210FC(HUDTex, iconPositionsOriginal[1].x - 6, iconPositionsOriginal[1].y - 6, 0xC, 0xC, 0x2C, 0x10, 0x400, 0x400);
//         func_800210FC(HUDTex, iconPositionsOriginal[2].x - 6, iconPositionsOriginal[2].y - 6, 0xC, 0xC, 0x38, 0x10, 0x400, 0x400);
//         func_800210FC(HUDTex, iconPositionsOriginal[3].x - 6, iconPositionsOriginal[3].y - 6, 0xC, 0xC, 0x44, 0x10, 0x400, 0x400);
//     }

//     func_80020E2C(HUDTex, 0x20, 0x1D, 0x80, 0xA);
    
//     for (i = 0; i < 4; i++) {
//         if (elementPositions == 1) {
//             func_80020D4C(1, iconPositionsVertical[i].x, iconPositionsVertical[i].y, arg0->unk24[i]);
//         } else if (elementPositions == 2) {
//             func_80020D4C(1, iconPositionsTriangle[i].x, iconPositionsTriangle[i].y, arg0->unk24[i]);
//         } else {
//             func_80020D4C(1, iconPositionsOriginal[i].x, iconPositionsOriginal[i].y, arg0->unk24[i]);
//         }
//     }
// }

//gPlayerData, 0x8007BA80
//unkStruct* arg0 is 0x8007BAB8
//unkStruct3* arg1 is 0x8007BACC

void func_800074A0_Hook(PlayerData* arg0, unkStruct3* arg1) {
    u16 var_a0;
    ElementLevels* elementsInstance;
    s32 levelIndex;

    if (!(gBattleState & 1)) {
        if (gAllowBattles & 1) {
            D_8007BC10 += D_8007BA5C;
            if (D_8007BC10 > 200.0) {
                D_8007BC10 -= 200.0;
            }
            D_8007BC18 += D_8007BA5C;
            if (1000.0 < D_8007BC18) { //D_800710F8 is 1000.0
                D_8007BC18 -= 1000.0;
                arg0->playerStats->unk2C++;
            }
        } else {
            D_8007BC14 += D_8007BA5C;
            if (D_8007BC14 > 800.0) {
                D_8007BC14 -= 800.0;
            }
            D_8007BC1C += D_8007BA5C;
            if (2000.0 < D_8007BC1C) { //D_80071100 is 2000.0
                D_8007BC1C -= 2000.0;
                arg0->playerStats->unk2C++;
            }
        }
    }
    if (arg1->unk64->unk6 < 500) {
        if (arg0->playerStats->unk28 >= D_80053ECC[arg0->playerStats->unk30]) {
            arg0->playerStats->unk28 = arg0->playerStats->unk28 - D_80053ECC[arg0->playerStats->unk30];
            var_a0 = 1;
            if ((arg1->unk64->unk6 + 1) > 500) {
                var_a0 = 500 - arg1->unk64->unk6;
            }
            arg1->unk64->unk6 += var_a0;
            arg1->unk64->unk4 += var_a0;
            if (arg0->playerStats->unk30 < 54) {
                arg0->playerStats->unk30++;
            }
        }
    } else {
        arg0->playerStats->unk28 = 0;
    }
    if (arg1->unk64->unkA < 500) {
        if (arg0->playerStats->unk2A >= D_80053ECC[arg0->playerStats->unk31] * 4) {
            arg0->playerStats->unk2A -= D_80053ECC[arg0->playerStats->unk31] * 4;
            var_a0 = 1;
            if ((arg1->unk64->unkA + 1) > 500) {
                var_a0 = (500 - arg1->unk64->unkA);
            }
            arg1->unk64->unkA += var_a0;
            arg1->unk64->unk8 += var_a0;
            if (arg0->playerStats->unk31 < 54) {
                arg0->playerStats->unk31++;
            }
        }
    } else {
        arg0->playerStats->unk2A = 0;
    }
    if (arg1->unk64->unkC < 255) {
        if (arg0->playerStats->unk2C >= D_80053ECC[arg0->playerStats->unk32]) {
            arg0->playerStats->unk2C -= D_80053ECC[arg0->playerStats->unk32];
            var_a0 = 1;
            if ((arg1->unk64->unkC + 1) > 255) {
                var_a0 = (255 - arg1->unk64->unkC);
            }
            arg1->unk64->unkC += var_a0;
            if (arg0->playerStats->unk32 < 54) {
                arg0->playerStats->unk32++;
            }
        }
    } else {
        arg0->playerStats->unk2C = 0;
    }
    if (arg1->unk64->unkE < 255) {
        if (arg0->playerStats->unk2E >= D_80053ECC[arg0->playerStats->unk33] * 2) {
            arg0->playerStats->unk2E -= D_80053ECC[arg0->playerStats->unk33] * 2;
            var_a0 = 1;
            if ((arg1->unk64->unkE + 1) > 255) {
                var_a0 = (255 - arg1->unk64->unkE);
            }
            arg1->unk64->unkE += var_a0;
            if (arg0->playerStats->unk33 < 54) {
                arg0->playerStats->unk33++;
            }
        }
    } else {
        arg0->playerStats->unk2E = 0;
    }
    if (!(gGameState & LEVEL_UP_MENU)) {
        //replace max level capping XP and make it so that it checks element levels instead
        elementsInstance = &arg0->playerStats->elements;
        if ((elementsInstance->fire + elementsInstance->earth + elementsInstance->water + elementsInstance->wind) >= elementCapsTable[gTotalBossesBeatenCount] * 4) {
            arg1->unk64->expGained = 0;
        } else {
            levelIndex = arg0->playerStats->levels;
            if (arg0->playerStats->levels >= 99) {
                levelIndex = 99;
            }
            if (arg1->unk64->expGained >= expRequiredPerElementLevel[levelIndex]) {
                arg1->unk64->expGained -= expRequiredPerElementLevel[levelIndex];
                //remove level up cap check. just level up no matter what
                arg0->playerStats->levels++;

                elementsInstance = &arg0->playerStats->elements;
                if ((elementsInstance->fire != elementCapsTable[gTotalBossesBeatenCount]) ||
                    (elementsInstance->earth != elementCapsTable[gTotalBossesBeatenCount]) ||
                    (elementsInstance->water != elementCapsTable[gTotalBossesBeatenCount]) ||
                    (elementsInstance->wind != elementCapsTable[gTotalBossesBeatenCount])) {
                    gGameState |= LEVEL_UP_MENU;
                    arg0->movementState = 0; //IDLE = 0
                    func_800268D4(0, 1, 0xFF);
                }
            }
        }
    }
    if (!(gGameState & (LEVEL_UP_MENU | TALKING_TO_NPC)) && (itemReceived != 255)) {
        func_800120C0(itemReceived);
        func_8002E768(1);
        AddItemToInventory(itemReceived);
        itemReceived = 255;
        arg0->unk8 |= 2;
        gGameState |= TALKING_TO_NPC;
        func_800268D4(0, 0x3B, 255);
    }
}

// void func_800074A0_Hook(PlayerData* arg0, unkStruct3* arg1) {
//     u16 var_a0;
//     ElementLevels* elementsInstance;

//     if (!(gBattleState & 1)) {
//         if (gAllowBattles & 1) {
//             D_8007BC10 += D_8007BA5C;
//             if (D_8007BC10 > 200.0) {
//                 D_8007BC10 -= 200.0;
//             }
//             D_8007BC18 += D_8007BA5C;
//             if (1000.0 < D_8007BC18) { //D_800710F8 is 1000.0
//                 D_8007BC18 -= 1000.0;
//                 arg0->unk10->unk2C++;
//             }
//         } else {
//             D_8007BC14 += D_8007BA5C;
//             if (D_8007BC14 > 800.0) {
//                 D_8007BC14 -= 800.0;
//             }
//             D_8007BC1C += D_8007BA5C;
//             if (2000.0 < D_8007BC1C) { //D_80071100 is 2000.0
//                 D_8007BC1C -= 2000.0;
//                 arg0->unk10->unk2C++;
//             }
//         }
//     }
//     if (arg1->unk64->unk6 < 500) {
//         if (arg0->unk10->unk28 >= D_80053ECC[arg0->unk10->unk30]) {
//             arg0->unk10->unk28 = arg0->unk10->unk28 - D_80053ECC[arg0->unk10->unk30];
//             var_a0 = 1;
//             if ((arg1->unk64->unk6 + 1) > 500) {
//                 var_a0 = 500 - arg1->unk64->unk6;
//             }
//             arg1->unk64->unk6 += var_a0;
//             arg1->unk64->unk4 += var_a0;
//             if (arg0->unk10->unk30 < 54) {
//                 arg0->unk10->unk30++;
//             }
//         }
//     } else {
//         arg0->unk10->unk28 = 0;
//     }
//     if (arg1->unk64->unkA < 500) {
//         if (arg0->unk10->unk2A >= D_80053ECC[arg0->unk10->unk31] * 4) {
//             arg0->unk10->unk2A -= D_80053ECC[arg0->unk10->unk31] * 4;
//             var_a0 = 1;
//             if ((arg1->unk64->unkA + 1) > 500) {
//                 var_a0 = (500 - arg1->unk64->unkA);
//             }
//             arg1->unk64->unkA += var_a0;
//             arg1->unk64->unk8 += var_a0;
//             if (arg0->unk10->unk31 < 54) {
//                 arg0->unk10->unk31++;
//             }
//         }
//     } else {
//         arg0->unk10->unk2A = 0;
//     }
//     if (arg1->unk64->unkC < 255) {
//         if (arg0->unk10->unk2C >= D_80053ECC[arg0->unk10->unk32]) {
//             arg0->unk10->unk2C -= D_80053ECC[arg0->unk10->unk32];
//             var_a0 = 1;
//             if ((arg1->unk64->unkC + 1) > 255) {
//                 var_a0 = (255 - arg1->unk64->unkC);
//             }
//             arg1->unk64->unkC += var_a0;
//             if (arg0->unk10->unk32 < 54) {
//                 arg0->unk10->unk32++;
//             }
//         }
//     } else {
//         arg0->unk10->unk2C = 0;
//     }
//     if (arg1->unk64->unkE < 255) {
//         if (arg0->unk10->unk2E >= D_80053ECC[arg0->unk10->unk33] * 2) {
//             arg0->unk10->unk2E -= D_80053ECC[arg0->unk10->unk33] * 2;
//             var_a0 = 1;
//             if ((arg1->unk64->unkE + 1) > 255) {
//                 var_a0 = (255 - arg1->unk64->unkE);
//             }
//             arg1->unk64->unkE += var_a0;
//             if (arg0->unk10->unk33 < 54) {
//                 arg0->unk10->unk33++;
//             }
//         }
//     } else {
//         arg0->unk10->unk2E = 0;
//     }
//     if (!(gGameState & LEVEL_UP_MENU)) {
//         if (arg0->unk10->unk34 >= 98) {
//             arg1->unk64->unk10 = 0;
//         } else {
//             if (arg1->unk64->unk10 >= expRequiredPerElementLevel[arg0->unk10->unk34]) {
//                 arg1->unk64->unk10 -= expRequiredPerElementLevel[arg0->unk10->unk34];
//                 if (arg0->unk10->unk34 < 98) {
//                     arg0->unk10->unk34++;
//                 }
//                 elementsInstance = &arg0->unk10->elements;
//                 if ((elementsInstance->fire != 50) || (elementsInstance->earth != 50) ||
//                     (elementsInstance->water != 50) || (elementsInstance->wind != 50)) {
//                     gGameState |= LEVEL_UP_MENU;
//                     arg0->unk0 = 0;
//                     func_800268D4(0, 1, 0xFF);
//                 }
//             }
//         }
//     }
//     if (!(gGameState & 0xA) && (itemReceived != 255)) {
//         func_800120C0(itemReceived);
//         func_8002E768(1);
//         AddItemToInventory(itemReceived);
//         itemReceived = 255;
//         arg0->unk8 |= 2;
//         gGameState |= 2;
//         func_800268D4(0, 0x3B, 255);
//     }
// }

void mainCFunction(void) { //ran every frame
    SetCurrentBossesBeaten();

    //change attack
    ChangeBrianFireSpells();
    ChangeBrianEarthSpells();
    ChangeBrianWaterSpells();
    ChangeBrianWindSpells();
    ChangeBrianBossSpells();
	
	//Claire Connor Forest Spirit Side Quest
	if (gCurrentMap == 16) {
		if (gNextSubmap == 1) {
			if ((gSpiritflagforest & 0xF) == 0xF) {
				if ((gEventflag1 & 1) == 0x0) {
					gEventflag1 |= 1;
				}
			}
		}
	}
	
	if (gEventflag1 & 1)  {
		if (gCurrentMap == 16) {
			if (gNextSubmap == 1) {
			eClairetext = 0x346E;
			}
		}
	}
	
	
	if (gCurrentMap == 16) {
		if (gNextSubmap == 1) {
			if (eTextlookup == 0xc0811882) {
				if (gEventflag1 & 1) {
					gEventflag1 |= 2;
					eClairespirit = 0xC0AB6890;
					eClairespirit2 = 0x40AB6890;
				}
			}
		}
	}
	
	
	if (gCurrentMap == 16) {
		if (gNextSubmap == 1) {
			if (gEventflag1 & 2) {
				eClairetext = 0x378B;
				eClairespirit = 0xC0AB6890;
				eClairespirit2 = 0x40AB6890;
			}
		}
	}
	//Claire Side Quest End		
	
	//Cockatrice Hunt Side Quest
	if (gCurrentMap == 16) {
		if (gNextSubmap == 19) {
			if (eTextlookup == 0xA0C08108) {
				gEventflag1 |= 4;
			}
		}
	}
	
	if (gCurrentMap == 3) {
		if (gNextSubmap == 0) {
			if (gEventflag1 & 4) {
				if (sCurrentenemy == 0x0E) {
					if (sEnemyHP == 0x0000) {
					gEventflag1 |= 8;
					}
				}
			}
		}
	}
	
		if (gCurrentMap == 16) {
			if (gNextSubmap == 19) {
				if (gEventflag1 & 8) {
					if ((gEventflag1 & 16) == 0x0) {
						eArkarttext = 0x486C;
					}
				}
			}
		}
	
		if (gCurrentMap == 16) {
			if (gNextSubmap == 19) {
				if (gEventflag1 & 8) {
					if (eTextlookup == 0xc0811382) {
						if ((gEventflag1 & 16) == 0x0) {
							gEventflag1 |= 16;
							sBrianHPEXP = sBrianHPEXP + 50;
							sBrianMPEXP = sBrianMPEXP + 30;
							sBrianDEFEXP = sBrianDEFEXP + 16;
							sBrianAGIEXP = sBrianAGIEXP + 16;
							sBrianELEEXP = sBrianELEEXP + 450;
							sBrianItemPickup = 0x1A;
						}
					}
				}
			}
		}
		
		if (gCurrentMap == 16) {
			if (gNextSubmap == 19) {
				if (gEventflag1 & 16) {
					eArkarttext = 0x486C;
				}
			}
		}
	//End of Cockatrice Hunt Side Quest
	
	//Flamed Mane Hunt Side Quest
	if (gCurrentMap == 23) {
		if (gNextSubmap == 15) {
			if (eTextlookup == 0xA0C08116) {
				gEventflag1 |= 32;
			}
		}
	}
	
	if (gCurrentMap == 29) {
		if (gNextSubmap == 0) {
			if (gEventflag1 & 32) {
				if (sCurrentenemy == 0x09) {
					if (sEnemyHP == 0x0000) { 
						gEventflag1 |= 64;
					}
				}
			}
		}
	}
	
		if (gCurrentMap == 23) {
			if (gNextSubmap == 15) {
				if (gEventflag1 & 64) {
					if ((gEventflag1 & 128) == 0x0) {
						eHershtext = 0x8C4C;
					}
				}
			}
		}
	
		if (gCurrentMap == 23) {
			if (gNextSubmap == 15) {
				if (gEventflag1 & 8) {
					if (eTextlookup == 0xa0c08118) {
						if ((gEventflag1 & 128) == 0x0) {
							gEventflag1 |= 128;
							sBrianHPEXP = sBrianHPEXP + 100;
							sBrianMPEXP = sBrianMPEXP + 60;
							sBrianDEFEXP = sBrianDEFEXP + 50;
							sBrianAGIEXP = sBrianAGIEXP + 50;
							sBrianELEEXP = sBrianELEEXP + 45000;
							sBrianItemPickup = 0x1A;
						}
					}
				}
			}
		}
	
	
		if (gCurrentMap == 23) {
			if (gNextSubmap == 15) {
				if (gEventflag1 & 128) {
					eHershtext = 0x8B42;
				}
			}
		}
		
	//Secret of the Philosopher's Stone
		if (gCurrentMap == 20) {
			if (gNextSubmap == 7) {
				if (bossFlags & 0x20) {
					if ((gEventflag13 & 128) == 0x0) {
					eLeonardotext = 0x6798;
					}
				}
			}
		}
		
		if (gCurrentMap == 20) {
			if (gNextSubmap == 7) {
				if (bossFlags & 0x20) {
					if (eTextlookup == 0xa0c08118) {
						gEventflag14 |= 1;
					}
				}
			}
		}
					
					
		if (gCurrentMap == 20) {
			if (gNextSubmap == 7) {
				if (gEventflag14 & 1){
					eLeonardotext = 0x67FA;
				}
			}
		}
		
		if (gCurrentMap == 26) {
			if (gNextSubmap == 3) {
				if (gEventflag14 & 1){
					eEponatext = 0xACF6;
				}
			}
		}
		
		if (gCurrentMap == 26) {
			if (gNextSubmap == 3) {
				if (bossFlags & 0x20) {
					if (gEventflag14 & 1){
						if (eTextlookup == 0xa0c08118) {
						gEventflag13 |= 128;
						}
					}
				}
			}
		}
		
		if (gCurrentMap == 26) {
			if (gNextSubmap == 3) {
				if (gEventflag13 & 128){
					eEponatext = 0xE330;
				}
			}
		}
		
		if (gCurrentMap == 26) {
			if (gNextSubmap == 3) {
				if (bossFlags & 0x20) {
					if (gEventflag14 & 1){
						if (eTextlookup == 0xa0c08112) {
						gEventflag14 |= 2;
						}
					}
				}
			}
		}
		
		
	//Mammon Empowered after Guilty
	if (bossFlags & 0x20) {
		sMammonMHP = 32000;
		sMammonCHP = 32000;
		sMammonDEF = 550;
		sMammonAGI = 666;
		sMammonATK = 150;
	}	
	


	//MaxHP/MP Bugfix
	if (sBrianMAXHP >= 1000) {
		sBrianMAXHP = 999;
	}
	if (sBrianHP >= 1000) {
		sBrianHP = 999;
	}
	if (sBrianMAXMP >= 1000) {
		sBrianMAXMP = 999;
	}
	if (sBrianMP >= 1000) {
		sBrianMP = 999;
	}

}


extern u32 rng_seed;
u32 calls = 0;

extern int _sprintf(char *s, const char *fmt, ...);
void drawBoxBehindEnemyName(void*, s32, s32, s32, s32, s32, s32, s32, s32);
//80036384 is sprintf

typedef struct BoxStruct {
    void* unk_00;
    s32 xPos;
    s32 yPos;
    s32 width;
    s32 height;
    s32 unk_14;
    s32 unk_18;
    s32 unk_1C;
    s32 unk_20;
} BoxStruct;

BoxStruct Box1 = {(void*)0x803A6F60, 0, 0x94, 0x48, 0xA, 0x64, 0x2C, 0x400, 0x400};
BoxStruct Box2 = {(void*)0x803A6F60, 0, 0x9F, 0x48, 0xA, 0x64, 0x2C, 0x400, 0x400};

void DrawingTestC(void) {
    // char buffer[20];
    
    // drawBoxBehindEnemyName(Box1.unk_00, Box1.xPos, Box1.yPos,
    //     Box1.width, Box1.height, Box1.unk_14, Box1.unk_18, Box1.unk_1C, Box1.unk_20);
    // _sprintf(buffer, "%08X", rng_seed);
    // func_80020B4C(20, 150, 0, buffer);

    // drawBoxBehindEnemyName(Box2.unk_00, Box2.xPos, Box2.yPos,
    //     Box2.width, Box2.height, Box2.unk_14, Box2.unk_18, Box2.unk_1C, Box2.unk_20);
    // _sprintf(buffer, "%d", calls);
    // func_80020B4C(20, 160, 0, buffer);
    //TestPrint();
}

u32 getRandomNumberHook(u32 arg0) {
    if (arg0 != 0) {
        calls++;
        return ((u32) ((rng_seed = (rng_seed * 0x41C64E6D) + 0x3039) >> 0x10)) % arg0;
    }
    return 0;
}

#define BOSS_TURN 0x100
#define IN_BATTLE 0x1
#define ENEMY_TURN 0x2


s32 ElementAttackHookC(void) {
    if (curActorTurn == (BOSS_TURN | ENEMY_TURN | IN_BATTLE)) {
        return 0;
    }

    if (curActorTurn == (ENEMY_TURN | IN_BATTLE)) {
        return 0;
    }
    //else, is brian's turn or out of battle
    return 1;
}

extern u8 D_8007C570[];
extern u8 D_8007C970[];
extern u8 customText[];

void func_80008A00_Hook(Unk_D_8007BD30* arg0, s32 arg1) {
    s32 i;

    //this requires 8 byte alignment on both addresses or it will crash console!
    _memcpy(D_8007C570, &customText[arg1], 1024);
    
    //dma_write(&D_D305E0[arg1], D_8007C570, 1024);
    
    if (arg0->npcData->npcName != NULL) {
        for (i = 0; i < 32; i++) {
            D_8007C970[i] = arg0->npcData->npcName[i];
        }
        return;
    }
    *D_8007C970 = 0xFF;
}
