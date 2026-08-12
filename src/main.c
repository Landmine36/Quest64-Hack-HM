#include "../include/Quest64.h"
#include "../include/PR/gbi.h"

#define MAX_LEVEL 98
#define MAX_HP 500
#define MAX_MP 500
#define MAX_AGI 255
#define MAX_DEF 255

#define BRIAN_TURN 1

typedef unsigned char uint8_t;

/* ---- Placeholder addresses / constants ---- */
#define INVENTORY_SECTION1_START   0x8008cf78
#define INVENTORY_SECTION1_SIZE    75
#define INVENTORY_SECTION2_START   0x8008cfc3
#define INVENTORY_SECTION2_SIZE    32
#define GAME_STATE_ADDR       0x8007B2E4
#define GAME_STATE_TITLE      		1
#define MAX_VALID_ITEM_ID			0x20
#define ITEM_LIST_END              0xFF
#define QTY_FLAG_BIT               0x80
#define QTY_VALUE_MASK             0x7F

#define ADDR_LDA_BASE       0x80003AE7  /*  Base ATK set (native 0x10) */
#define ADDR_LDA_ATK_UP_1   0x8001712B  /*  Atk Up 1 set (native 0x18) */
#define ADDR_LDA_ATK_UP_2   0x8001713F  /*  Atk Up 2 set (native 0x20) */
#define ADDR_LDA_DISPEL     0x80016CF7  /*  Dispel set   (native 0x10) */
#define ADDR_LDA_WEAR_OFF   0x80018197  /*  Wear-off set (native 0x10) */
#define ADDR_CURRENT_MAX_HP      0x8007BA86  /*  u16, 0-999 */
#define ADDR_ELEMENTAL_TOTAL     0x8007BBBC  /*sum of all elemental levels */
#define ADDR_ELEMENTAL_FIRE   0x8007baa4  
#define ADDR_ELEMENTAL_EARTH  0x8007baa5  
#define ADDR_ELEMENTAL_WATER  0x8007baa6  
#define ADDR_ELEMENTAL_WIND   0x8007baa7  
#define HP_WEIGHT_X100    2775   /* 27.75 -> 25% of 111 */
#define ELEM_WEIGHT_X100  8325   /* 83.25 -> 75% of 111 */
#define BASE_STRENGTH     0x10
#define STARTING_MAX_HP        50
#define STARTING_ELEMENTAL     4

/* Per-item stack cap. Index = item ID, value = max EXTRA copies
   allowed (not counting the 1 live copy in Section 1). */
static const uint8_t ItemStackCap[INVENTORY_SECTION2_SIZE] = {
    98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98
};

/* Custom sort order. Index = item ID, value = desired sort rank
   (lower rank = appears earlier in the list). Fill this in to
   group new healing/spell/key/wing items next to their existing
   category-mates instead of raw ID order. Ranks don't need to be
   contiguous — just consistently ordered relative to each other. */
static const uint8_t ItemSortRank[INVENTORY_SECTION2_SIZE] = {
    4,1,2,3,8,5,6,7,12,13,14,15,16,17,19,20,21,22,23,24,26,27,28,29,30,31,11,9,10,32,25,18
};

/* ---- Helper accessors ---- */

static inline uint8_t* Section1Base(void) {
    return (uint8_t*)INVENTORY_SECTION1_START;
}

static inline uint8_t* Section2Base(void) {
    return (uint8_t*)INVENTORY_SECTION2_START;
}

void InventoryStackingTick(void) {
}



extern Gfx* gMasterGfxPos;

f32 hpNightMultiplier = 1.5f;
f32 defNightMultiplier = 1.5f;
f32 agiNightMultiplier = 1.5f;
f32 atkNightMultiplier = 1.5f;
f32 powNightMultiplier = 1.5f;

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

s32 elementCapsTable[] = {10, 20, 30, 40, 40, 50, 99, 150, 150};
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
    if (arg1->unk64->unk6 < 999) {
        if (arg0->playerStats->unk28 >= D_80053ECC[arg0->playerStats->unk30]) {
            arg0->playerStats->unk28 = arg0->playerStats->unk28 - D_80053ECC[arg0->playerStats->unk30];
            var_a0 = 1;
            if ((arg1->unk64->unk6 + 1) > 999) {
                var_a0 = 999 - arg1->unk64->unk6;
            }
            arg1->unk64->unk6 += var_a0;
            arg1->unk64->unk4 += var_a0;
            if (arg0->playerStats->unk30 < 41) {
                arg0->playerStats->unk30++;
            }
        }
    } else {
        arg0->playerStats->unk28 = 0;
    }
    if (arg1->unk64->unkA < 999) {
        if (arg0->playerStats->unk2A >= D_80053ECC[arg0->playerStats->unk31] * 4) {
            arg0->playerStats->unk2A -= D_80053ECC[arg0->playerStats->unk31] * 4;
            var_a0 = 1;
            if ((arg1->unk64->unkA + 1) > 999) {
                var_a0 = (999 - arg1->unk64->unkA);
            }
            arg1->unk64->unkA += var_a0;
            arg1->unk64->unk8 += var_a0;
            if (arg0->playerStats->unk31 < 1) {
                arg0->playerStats->unk31++;
            }
        }
    } else {
        arg0->playerStats->unk2A = 0;
    }
    if (arg1->unk64->unkC < 999) {
        if (arg0->playerStats->unk2C >= D_80053ECC[arg0->playerStats->unk32]) {
            arg0->playerStats->unk2C -= D_80053ECC[arg0->playerStats->unk32];
            var_a0 = 1;
            if ((arg1->unk64->unkC + 1) > 999) {
                var_a0 = (999 - arg1->unk64->unkC);
            }
            arg1->unk64->unkC += var_a0;
            if (arg0->playerStats->unk32 < 1) {
                arg0->playerStats->unk32++;
            }
        }
    } else {
        arg0->playerStats->unk2C = 0;
    }
    if (arg1->unk64->unkE < 999) {
        if (arg0->playerStats->unk2E >= D_80053ECC[arg0->playerStats->unk33] * 2) {
            arg0->playerStats->unk2E -= D_80053ECC[arg0->playerStats->unk33] * 2;
            var_a0 = 1;
            if ((arg1->unk64->unkE + 1) > 999) {
                var_a0 = (999 - arg1->unk64->unkE);
            }
            arg1->unk64->unkE += var_a0;
            if (arg0->playerStats->unk33 < 1) {
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
	
//	if (gCurrentMap == 3) {
//		if (gNextSubmap == 0) {
//			if (gEventflag1 & 4) {
//				if ((gEventflag1 & 8) == 0x00) {
//					if (sCurrentenemy == 0x0E) {
//						if ((gBattleState3 & 8) == 0x00) {
//							gBattleState3 |= 8;
//							sEnemyHP = sEnemyHP * 1.3;
//							sEnemyATK = sEnemyATK * 1.3;
//							sEnemyDEF = sEnemyDEF * 1.3;
//							sEnemyAGI = sEnemyAGI * 1.3;
//						}
//					}
//				}
//			}
//		}
//	}
	
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
							sBrianStone = sBrianStone + 300;
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
							sBrianStone = sBrianStone + 30000;
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
		
		
	//Remains of Donovan's Family in Greenoch
	//Zonne in Greenoch
		if (gCurrentMap == 23) {
			if (gNextSubmap == 2) {
				if ((gEventflag2 & 1) == 0x0) {
					if (eTextlookup == 0xa0c0810B) {
						gEventflag2 |= 1;
					}
				}
			}
		}
	//Donovan in Larapool
		if (gCurrentMap == 17) {
			if (gNextSubmap == 0) {
				if (gEventflag2 & 1) {
					if (eDonovantext == 0x5788){
					if ((gEventflag2 & 2) == 0x0) {
						eDonovantext = 0xbc00;
					}
					}
				}
			}
		}
	//Donovan in Larapool
		if (gCurrentMap == 17) {
			if (gNextSubmap == 0) {
				if (gEventflag2 & 1) {
					if ((gEventflag2 & 2) == 0x0) {
						if (eTextlookup == 0xa0c08118) {
							gEventflag2 |= 2;
						}
					}
				}
			}
		}
	//Zonne in Greenoch
		if (gCurrentMap == 23) {
			if (gNextSubmap == 2) {
				if (gEventflag2 & 2) {
					if ((gEventflag2 & 4) == 0x0) {
						eZonnetext = 0xbd00;
					}
				}
			}
		}
	//Zonne in Greenoch
		if (gCurrentMap == 23) {
			if (gNextSubmap == 2) {
				if (gEventflag2 & 2) {
					if ((gEventflag2 & 4) == 0x0) {
						if (eTextlookup == 0xa0c08107) {
							gEventflag2 |= 4;
							sBrianHPEXP = sBrianHPEXP + 60;
							sBrianMPEXP = sBrianMPEXP + 100;
							sBrianDEFEXP = sBrianDEFEXP + 20;
							sBrianAGIEXP = sBrianAGIEXP + 50;
							sBrianELEEXP = sBrianELEEXP + 30000;
							sBrianStone = sBrianStone + 15000;
							sBrianItemPickup = 0x1A;
							
						}
					}
				}
			}
		}
	
	//Zonne in Greenoch
		if (gCurrentMap == 23) {
			if (gNextSubmap == 2) {
				if (gEventflag2 & 2) {
					if (gEventflag2 & 4) {
						eZonnetext = 0xbe00;
					}
				}
			}
		}
		
	//Donovan in Larapool
		if (gCurrentMap == 17) {
			if (gNextSubmap == 0) {
				if (gEventflag2 & 2) {
					if (gEventflag2 & 4) {
					if (eDonovantext == 0x5788 || eDonovantext == 0xBC00){
						eDonovantext = 0xbf00;
					}
					}
				}
			}
		}
		
	//End of Donovans side quest
	
	//Clay the Potter's Side Quest
		if (gCurrentMap == 0x10) {
			if (gNextSubmap == 6) {
				if (gBossFlags & 16) {
						eClaytext = 0xC000;
				}
			}
		}
		
		if (gCurrentMap == 0x10) {
			if (gNextSubmap == 6) {	
				if (eTextlookup == 0xa0c08113) {
					gEventflag2 |= 8;
				}
			}
		}
		
		if (gCurrentMap == 0x12) {
			if (gNextSubmap == 5) {
				if (gEventflag2 & 8) {
					eBaggutext = 0xC200;
				}
			}
		}
		
		if (gCurrentMap == 0x12) {
			if (gNextSubmap == 5) {
				if (gEventflag2 & 8) {
					eErrortext = 0xC100;
				}
			}
		}
					
		if (gCurrentMap == 0x12) {
			if (gNextSubmap == 5) {	
				if (gEventflag2 & 8) {
					if (eTextlookup == 0xa0c08113) {
						gEventflag2 |= 16;
					}
				}
			}
		}
		
		if (gCurrentMap == 0x10) {
			if (gNextSubmap == 6) {
				if (gEventflag2 & 16) {
					eClaytext = 0xC300;
				}
			}
		}
		
		if (gCurrentMap == 0x10) {
			if (gNextSubmap == 6) {	
				if (gEventflag2 & 16) {
					if (eTextlookup == 0xa0c08101) {
						if (!(gEventflag2 & 32)) {
						gEventflag2 |= 32;
							sBrianHPEXP = sBrianHPEXP + 40;
							sBrianMPEXP = sBrianMPEXP + 40;
							sBrianDEFEXP = sBrianDEFEXP + 40;
							sBrianAGIEXP = sBrianAGIEXP + 40;
							sBrianELEEXP = sBrianELEEXP + 15000;
							sBrianStone = sBrianStone + 1000;
							sBrianItemPickup = 0x1A;
						}
					}
				}
			}
		}
		
		if (gCurrentMap == 0x10) {
			if (gNextSubmap == 6) {	
				if (gEventflag2 & 32) {
					eClaytext = 0xC400;
				}
			}
		}
		
		
		
	//End of Clay's Sidequest
	
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
					eEponatext = 0xB860;
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
				if (gEventflag13 & 128) {
					eEponatext = 0xBB60;
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
		
		if (gEventflag13 & 128) {
			if ((gCurrentMap == 0 && (gNextSubmap == 3))|(gCurrentMap == 2)|(gCurrentMap == 3)|(gCurrentMap == 5)|(gCurrentMap == 7)|(gCurrentMap == 8 && (gNextSubmap == 0))|(gCurrentMap == 9)|(gCurrentMap == 10)|(gCurrentMap == 12)|(gCurrentMap == 14 && (gNextSubmap == 1))|(gCurrentMap == 19)|(gCurrentMap == 23)|(gCurrentMap == 26 && (!(gNextSubmap == 3)))|(gCurrentMap == 27)|(gCurrentMap == 28)|(gCurrentMap == 29)|(gCurrentMap == 30)|(gCurrentMap == 31)|(gCurrentMap == 32)|(gCurrentMap == 32 && (!(gNextSubmap == 1)))|(gCurrentMap == 33)|(gCurrentMap == 34)|(gCurrentMap == 35)) {
				gEventBossRead = 0x69F3;
				gEventBossWrite = 0x69F3;
//				gEventBossRRead = 0x69F3;
//				gEventBossRWrite = 0x69F3;
			}
//			else if (!(gCurrentMap == 26|gCurrentMap == 31|gCurrentMap == 35|gCurrentMap == 12)) {
			else if (!(gCurrentMap == 0 && (!(gNextSubmap == 3)))|(gCurrentMap == 2)|(gCurrentMap == 3)|(gCurrentMap == 5)|(gCurrentMap == 7)|(gCurrentMap == 8 && (!(gNextSubmap == 0)))|(gCurrentMap == 9)|(gCurrentMap == 10)|(gCurrentMap == 12)|(gCurrentMap == 14 && (!(gNextSubmap == 1)))|(gCurrentMap == 19)|(gCurrentMap == 23)|(gCurrentMap == 26 && (!(gNextSubmap == 3)))|(gCurrentMap == 27)|(gCurrentMap == 28)|(gCurrentMap == 29)|(gCurrentMap == 30)|(gCurrentMap == 31)|(gCurrentMap == 32)|(gCurrentMap == 32 && (!(gNextSubmap == 1)))|(gCurrentMap == 33)|(gCurrentMap == 34)|(gCurrentMap == 35)) {
				gEventBossRead = 0xD19C;
				gEventBossWrite = 0xD19C;
//				gEventBossRRead = 0xD19C;
//				gEventBossRWrite = 0xD19C;
			}
		}
		
			
		
		//Solvaring Pride
		if (gEventflag13 & 128){
			if (!(sSolvaMHP == 12000)){
			sSolvaMHP = 12000;
			sSolvaCHP = 12000;
			sSolvaDEF = 1000;
			sSolvaAGI = 350;
			sSolvaATK = 120;
			sSolvaEXP = 500000;
			sSolvaSTN = 25000;
			sSolvaDRP = 0x1A000000;
			sSolvaX = 0x00000000;	
			sSolvaZ = 0x00000000;	
			sSolvaF = 0x00000000;	
			sSolvaMap = 0x000E0001;	
			sSolvaName = 0x8040ef0A;	
			sSolvaBName = 0x8040ef60;	
			sSolvaText = 0x8040f000;	
//			sSolvaS1 = 0x;	
//			sSolvaS2 = 0x;	
			}
		}
		//Zelse Sorrow
		if (gEventflag13 & 128){
			if (!(sZelseMHP == 15000)){
			sZelseMHP = 15000;
			sZelseCHP = 15000;
			sZelseDEF = 1020;
			sZelseAGI = 450;
			sZelseATK = 125;
			sZelseEXP = 510000;
			sZelseSTN = 25000;
			sZelseDRP = 0x1A000000;
			sZelseX = 0xC31AC3A7;
			sZelseZ = 0xC349246B;
			sZelseF = 0x3BC58D33;
			sZelseMap = 0x00200000;
			sZelseName= 0x8040ef14;
			sZelseBName= 0x8040ef68;
			sZelseText = 0x8040f100;
			sZelseS1 = 0x830a;
			sZelseS2 = 0x830b;
			sZelseS3 = 0x8307;
		}
		}
		
		//Nepty Sloth
		if (gEventflag13 & 128){
			if (!(sNeptyMHP == 18000)){
			sNeptyMHP = 18000;
			sNeptyCHP = 18000;
			sNeptyDEF = 1040;
			sNeptyAGI = 500;
			sNeptyATK = 130;
			sNeptyEXP = 520000;
			sNeptySTN = 25000;
			sNeptyDRP = 0x1A000000;
			sNeptyX = 0xc3a9ec04;	
			sNeptyZ = 0xc3403c6d;	
			sNeptyF = 0x3fe00000;	
			sNeptyMap = 0x00020000;	
			sNeptyName = 0x8040ef1f;	
			sNeptyBName = 0x8040ef70;	
			sNeptyText = 0x8040f200;	
//			sNeptyS1 = 0x;	
//			sNeptyS2 = 0x;	
		}
		}
		//Shilf Lust
		if (gEventflag13 & 128){
			if (!(sShilfMHP == 21000)){
			sShilfMHP = 21000;
			sShilfCHP = 21000;
			sShilfDEF = 1060;
			sShilfAGI = 550;
			sShilfATK = 135;
			sShilfEXP = 530000;
			sShilfSTN = 25000;
			sShilfDRP = 0x1A000000;
			sShilfX = 0x00000000;	
			sShilfZ = 0xc1800000;	
			sShilfF = 0x3fc00000;	
			sShilfMap = 0x00000003;	
			sShilfName = 0x8040ef29;	
			sShilfBName = 0x8040ef78;	
			sShilfText = 0x8040f300;	
//			sShilfS1 = 0x;	
//			sShilfS2 = 0x;	
		}
		}
		//Fargo Wrath
		if (gEventflag13 & 128){
			if (!(sFargoMHP == 24000)){
			sFargoMHP = 24000;
			sFargoCHP = 24000;
			sFargoDEF = 1080;
			sFargoAGI = 600;
			sFargoATK = 140;
			sFargoEXP = 540000;
			sFargoSTN = 25000;
			sFargoDRP = 0x1A000000;
			sFargoX = 0xc4407a70;	
			sFargoZ = 0xc3d630e3;	
			sFargoF = 0x3f400000;	
			sFargoMap = 0x00080000;	
			sFargoName = 0x8040ef32;	
			sFargoBName = 0x8040ef80;	
			sFargoText = 0x8040f400;	
//			sFargoS1 = 0x;	
//			sFargoS2 = 0x;	
		}
		}
		//Guilty Gluttony
		if (gEventflag13 & 128){
			if (!(sGuiltyMHP == 27000)){
			sGuiltyMHP = 27000;
			sGuiltyCHP = 27000;
			sGuiltyDEF = 1100;
			sGuiltyAGI = 650;
			sGuiltyATK = 128;
			sGuiltyEXP = 530000;
			sGuiltySTN = 25000;
			sGuiltyDRP = 0x1A000000;
			//sGuiltyX = 0xc4b00000;	
			//sGuiltyZ = 0xc3200000;	
			//sGuiltyF = 0x3fc80000;	
			//sGuiltyMap = 0x000B0001;	
			sGuiltyX = 0xc4725c31;	
			sGuiltyZ = 0x4234e49f;	
			sGuiltyF = 0x3fea3752;	
			sGuiltyMap = 0x00090000;	
			sGuiltyName = 0x8040ef3c;	
			sGuiltyBName = 0x8040ef88;	
			sGuiltyText = 0x8040f500;	
//			sGuiltyS1 = 0x;	
//			sGuiltyS2 = 0x;	
		}
		}
		//Beigis Envy
		if (gEventflag13 & 128){
			if (!(sBeigisMHP == 30000)){
			sBeigisMHP = 30000;
			sBeigisCHP = 30000;
			sBeigisDEF = 1120;
			sBeigisAGI = 700;
			sBeigisATK = 150;
			sBeigisEXP = 530000;
			sBeigisSTN = 25000;
			sBeigisDRP = 0x1A000000;
			sBeigisName = 0x8040ef49;	
			sBeigisBName = 0x8040ef91;	
			sBeigisText = 0x8040f600;	
//			sBeigisS1 = 0x;	
//			sBeigisS2 = 0x;	
		}
		}
	
	//Mammon Greed
	if (gEventflag13 & 128){
		if (gEventflag16 == 0x7F) {
			if (!(sMammonMHP == 32000)){
					sMammonMHP = 32000;
					sMammonCHP = 32000;
					sMammonDEF = 999;
					sMammonAGI = 1500;
					sMammonATK = 150;	
					sMammonName = 0x8040ef52;	
					sMammonBName = 0x8040ef98;	
					sMammonText = 0x8040f800;	
//					sMammonS1 = 0x;	
//					sMammonS2 = 0x;		
//					sMammonS3 = 0x;					
				}
			}
	}
	
	

	//Spell Cost Reduce
	
	if (gCurrentMap == 0x20) {
			if (gNextSubmap == 0x01) {
				if (bossFlags >= 0x5F) {
					if (eTextlookup == 0x80c0e00e) {
						if ((gEventflag15 & 16) == 0){
							sBrianHPEXP = sBrianHPEXP + 16;
							sBrianMPEXP = sBrianMPEXP + 16;
							sBrianDEFEXP = sBrianDEFEXP + 16;
							sBrianAGIEXP = sBrianAGIEXP + 16;
							sBrianELEEXP = sBrianELEEXP + 15000;
							gEventflag15 |= 16;
						}
					}
				if ((gEventflag15 & 16) == 0){	
				eKellytext = 0xD800;
				}
				else
					eKellytext = 0x9600;
				}
			}
		}
	
	 	if (gEventflag15 & 16){
			if (sLv1MPCost == 0x0001){
				sLv1MPCost = 0x0000;
				sLv2MPCost = 0x0001;
				sLv3MPCost = 0x0002;
			}
		}
		if (!(gEventflag15 & 16)){
 			if (sLv1MPCost == 0x0000){
				sLv1MPCost = 0x0001;
				sLv2MPCost = 0x0002;
				sLv3MPCost = 0x0003;
		    }
		}
// END
	//EXP Multipliers
	
	
	
	if (gCurrentMap == 0x0E) {
			if (gNextSubmap == 0x0E) {
				if (bossFlags >= 0x07) {
					if (eTextlookup == 0x80c0e07f) {
						if ((gEventflag15 & 8) == 0){
							sBrianHPEXP = sBrianHPEXP + 32;
							sBrianMPEXP = sBrianMPEXP + 15;
							sBrianDEFEXP = sBrianDEFEXP + 16;
							sBrianAGIEXP = sBrianAGIEXP + 32;
							sBrianELEEXP = sBrianELEEXP + 5000;
							gEventflag15 |= 8;
						}
					}
				if ((gEventflag15 & 8) == 0){	
				eFloratext = 0xD000;
				}
				else
					eFloratext = 0x4700;
				}
			}
		}
	
	
	if (gCurrentMap == 0x19) {
			if (gNextSubmap == 0x00) {
				if (bossFlags >= 0x5F) {
					if (eTextlookup == 0x80c0e07f) {
						if ((gEventflag15 & 4) == 0){
							sBrianHPEXP = sBrianHPEXP + 50;
							sBrianMPEXP = sBrianMPEXP + 30;
							sBrianDEFEXP = sBrianDEFEXP + 64;
							sBrianAGIEXP = sBrianAGIEXP + 32;
							sBrianELEEXP = sBrianELEEXP + 40000;
							gEventflag15 |= 4;
						}
					}
				if ((gEventflag15 & 4) == 0){	
				eKiliactext = 0xD200;
				}
				else
					eKiliactext = 0xA078;
				}
			}
		}
		
	if (gCurrentMap == 0x15) {
		if (gNextSubmap == 0x0B) {
			if (bossFlags >= 0x1F) {
				if (eTextlookup == 0x80c0e00e) {
					if ((gEventflag15 & 1) == 0){
						sBrianHPEXP = sBrianHPEXP + 50;
						sBrianMPEXP = sBrianMPEXP + 10;
						sBrianDEFEXP = sBrianDEFEXP + 10;
						sBrianAGIEXP = sBrianAGIEXP + 10;
						sBrianELEEXP = sBrianELEEXP + 10000;
						gEventflag15 |= 1;
					}
				}
			if ((gEventflag15 & 1) == 0){	
			eWilliamtext = 0xD600;
			}
			else
				eWilliamtext = 0x7E40;
			}
		}
	}
	
	if (gCurrentMap == 0x0d) {
		if (gNextSubmap == 0x11) {
			if (bossFlags >= 0x0F) {
				if (eTextlookup == 0x80c0e07f) {
					if ((gEventflag15 & 2) == 0){
						sBrianHPEXP = sBrianHPEXP + 25;
						sBrianMPEXP = sBrianMPEXP + 10;
						sBrianDEFEXP = sBrianDEFEXP + 10;
						sBrianAGIEXP = sBrianAGIEXP + 10;
						sBrianELEEXP = sBrianELEEXP + 2000;
						gEventflag15 |= 2;
					}
				}
			if ((gEventflag15 & 2) == 0){	
			eAbbotttext = 0xD400;
			}
			else
				eAbbotttext = 0x2620;
			}
		}
	}
	
	
	
		if (gEventflag15 & 1){
			if (sHPEXPM == 1){
				sHPEXPM = 2;
				sSHPEXPM = 2;
			}
		}
		if (!(gEventflag15 & 1)){
			if (sHPEXPM == 2){
				sHPEXPM = 1;
				sSHPEXPM = 1;
			}
		}
		if (gEventflag15 & 2){
			if (sMPEXPM == 1){
				sMPEXPM = 2;
				sHMPEXPM = 2;
			}
		}
		if (!(gEventflag15 & 2)){
			if (sMPEXPM == 2){
				sMPEXPM = 1;
				sHMPEXPM = 1;
			}
		}
		if (gEventflag15 & 4){
			if (sDEFEXPM == 1){
				sDEFEXPM = 2;
			}
		}
		if (!(gEventflag15 & 4)){
			if (sDEFEXPM == 2){
				sDEFEXPM = 1;
			}
		}
		if (gEventflag15 & 8){
			if (sAGIEXPM == 1){
				sAGIEXPM = 2;
				sBAGIEXPM = 2;
			}
		}
		if (!(gEventflag15 & 8)){
			if (sAGIEXPM == 2){
				sAGIEXPM = 1;
				sBAGIEXPM = 1;
			}
		}
//// End

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

	//Elemental Level Bugfix
	if (sMaxEleLvl == 0xFF000000){
		if (!(sCombinedEle == 0x96969696)){
			sMaxEleLvl = 0xFE000000;
		}
	}
	if (sMaxEleLvl == 0xFE000000){
		if (sCombinedEle == 0x96969696){
			sMaxEleLvl = 0xFF000000;
		}
	}
	//End
		
	
	
//Drain Attack Fix
	if (gSpellCast == 0x00000006){
			if (gSpellTimer == 0x00020000){
				if (sATKStat <= 0x1D){
				sATKStat = sATKStat * 1.5;
				gSpellTimer = 0x00010000;
				}
			}
	}
//Drain Attack Fix End

//Brian Shrunk Fix	
		if (sBrianSize == 0x3d0f5c29 && !(sBrianComp == 0x04)){
				sBrianSize = 0x3d8f5c29;
		}

//Damage Number Color Change
	if (gDamageNumbers == 0x00000001){
     gDamageColor = 0xFFFFFF00;
	}
	if (gDamageNumbers == 0x00000002){
     gDamageColor = 0xFFFF0000;
	}
	if (gDamageNumbers == 0x00000003){
     gDamageColor = 0xFF404000;
	}
	if (gDamageNumbers == 0x00000004){
     gDamageColor = 0x80008000;
	}
	if (gDamageNumbers == 0x00000005){
		gDamageColor = 0x00000000;
	}
//

//Restriction Lines
		if (sEnemy1R == 0x800875a8){
		if (sAnim2R1 == 0x800eaef0){
			if (sEnemy1RTime >= 0x04){
				sAnimR1 = 0x04;}
			else if (sEnemy1RTime <= 0x03){
				sAnimR1 = sEnemy1RTime;
			}
		}
	}
		if (sEnemy1R == 0x80087600){
					if (sAnim2R2 == 0x800eaef0){
			if (sEnemy1RTime >= 0x04){
			sAnimR2 = 0x04;}
		else if (sEnemy1RTime <= 0x03){
			sAnimR2 = sEnemy1RTime;
		}
	}
		}
		if (sEnemy1R == 0x80087658){
					if (sAnim2R3 == 0x800eaef0){
			if (sEnemy1RTime >= 0x04){
			sAnimR3 = 0x04;}
		else if (sEnemy1RTime <= 0x03){
			sAnimR3 = sEnemy1RTime;
		}
	}
		}
		if (sEnemy1R == 0x800876b0){
					if (sAnim2R4 == 0x800eaef0){
			if (sEnemy1RTime >= 0x04){
			sAnimR4 = 0x04;}
		else if (sEnemy1RTime <= 0x03){
			sAnimR4 = sEnemy1RTime;
		}
	}
		}
		if (sEnemy1R == 0x80087708){
					if (sAnim2R5 == 0x800eaef0){
			if (sEnemy1RTime >= 0x04){
			sAnimR5 = 0x04;}
		else if (sEnemy1RTime <= 0x03){
			sAnimR5 = sEnemy1RTime;
		}
	}
		}
		if (sEnemy1R == 0x80087760){
					if (sAnim2R6 == 0x800eaef0){
			if (sEnemy1RTime >= 0x04){
			sAnimR6 = 0x04;}
		else if (sEnemy1RTime <= 0x03){
			sAnimR6 = sEnemy1RTime;
		}
	}
		}
		if (sEnemy1R == 0x800877b8){
					if (sAnim2R7 == 0x800eaef0){
			if (sEnemy1RTime >= 0x04){
			sAnimR7 = 0x04;}
		else if (sEnemy1RTime <= 0x03){
			sAnimR7 = sEnemy1RTime;
		}
	}
		}
		if (sEnemy1R == 0x80087910){
					if (sAnim2R8 == 0x800eaef0){
			if (sEnemy1RTime >= 0x04){
			sAnimR8 = 0x04;}
		else if (sEnemy1RTime <= 0x03){
			sAnimR8 = sEnemy1RTime;
		}
	}
		}
		if (sEnemy1R == 0x80087868){
					if (sAnim2R9 == 0x800eaef0){
			if (sEnemy1RTime >= 0x04){
			sAnimR9 = 0x04;}
		else if (sEnemy1RTime <= 0x03){
			sAnimR9 = sEnemy1RTime;
		}
	}
		}
		if (sEnemy1R == 0x800878c0){
					if (sAnim2R10 == 0x800eaef0){
			if (sEnemy1RTime >= 0x04){
			sAnimR10 = 0x04;}
		else if (sEnemy1RTime <= 0x03){
			sAnimR10 = sEnemy1RTime;
		}
	}
		}
		if (sEnemy1R == 0x80087918){
					if (sAnim2R11 == 0x800eaef0){
			if (sEnemy1RTime >= 0x04){
			sAnimR11 = 0x04;}
		else if (sEnemy1RTime <= 0x03){
			sAnimR11 = sEnemy1RTime;
		}
	}
		}
		if (sEnemy1R == 0x80087970){
					if (sAnim2R12 == 0x800eaef0){
			if (sEnemy1RTime >= 0x04){
			sAnimR12 = 0x04;}
		else if (sEnemy1RTime <= 0x03){
			sAnimR12 = sEnemy1RTime;
		}
	}	
		}
		if (sEnemy1R == 0x800879c8){
					if (sAnim2R13 == 0x800eaef0){
			if (sEnemy1RTime >= 0x04){
			sAnimR13 = 0x04;}
		else if (sEnemy1RTime <= 0x03){
			sAnimR13 = sEnemy1RTime;
		}
	}	
		}
		if (sEnemy1R == 0x80087a20){
					if (sAnim2R14 == 0x800eaef0){
			if (sEnemy1RTime >= 0x04){
			sAnimR14 = 0x04;}
		else if (sEnemy1RTime <= 0x03){
			sAnimR14 = sEnemy1RTime;
		}
	}	
		}
		if (sEnemy1R == 0x80087a78){
					if (sAnim2R15 == 0x800eaef0){
			if (sEnemy1RTime >= 0x04){
			sAnimR15 = 0x04;}
		else if (sEnemy1RTime <= 0x03){
			sAnimR15 = sEnemy1RTime;
		}
	}	
		}
		if (sEnemy1R == 0x80087ad0){
					if (sAnim2R16 == 0x800eaef0){
			if (sEnemy1RTime >= 0x04){
			sAnimR16 = 0x04;}
		else if (sEnemy1RTime <= 0x03){
			sAnimR16 = sEnemy1RTime;
		}
	}	
		}
		if (sEnemy1R == 0x80087b28){
					if (sAnim2R17 == 0x800eaef0){
			if (sEnemy1RTime >= 0x04){
			sAnimR17 = 0x04;}
		else if (sEnemy1RTime <= 0x03){
			sAnimR17 = sEnemy1RTime;
		}
	}	
		}
		if (sEnemy1R == 0x80087b80){
					if (sAnim2R18 == 0x800eaef0){
			if (sEnemy1RTime >= 0x04){
			sAnimR18 = 0x04;}
		else if (sEnemy1RTime <= 0x03){
			sAnimR18 = sEnemy1RTime;
		}
	}	
		}
		if (sEnemy1R == 0x80087bd8){
					if (sAnim2R19 == 0x800eaef0){
			if (sEnemy1RTime >= 0x04){
			sAnimR19 = 0x04;}
		else if (sEnemy1RTime <= 0x03){
			sAnimR19 = sEnemy1RTime;
		}
	}	 
		}
		if (sEnemy1R == 0x80087c30){
					if (sAnim2R20 == 0x800eaef0){
			if (sEnemy1RTime >= 0x04){
			sAnimR20 = 0x04;}
		else if (sEnemy1RTime <= 0x03){
			sAnimR20 = sEnemy1RTime;
		}
	}	
		}
		if (sEnemy1R == 0x80087c88){
					if (sAnim2R21 == 0x800eaef0){
			if (sEnemy1RTime >= 0x04){
			sAnimR21 = 0x04;}
		else if (sEnemy1RTime <= 0x03){
			sAnimR21 = sEnemy1RTime;
		}
	}
		}
		if (sEnemy2R == 0x800875a8){
		if (sAnim2R1 == 0x800eaef0){
			if (sEnemy2RTime >= 0x04){
				sAnimR1 = 0x04;}
			else if (sEnemy2RTime <= 0x03){
				sAnimR1 = sEnemy2RTime;
			}
		}
	}
		if (sEnemy2R == 0x80087600){
					if (sAnim2R2 == 0x800eaef0){
			if (sEnemy2RTime >= 0x04){
			sAnimR2 = 0x04;}
		else if (sEnemy2RTime <= 0x03){
			sAnimR2 = sEnemy2RTime;
		}
	}
		}
		if (sEnemy2R == 0x80087658){
					if (sAnim2R3 == 0x800eaef0){
			if (sEnemy2RTime >= 0x04){
			sAnimR3 = 0x04;}
		else if (sEnemy2RTime <= 0x03){
			sAnimR3 = sEnemy2RTime;
		}
	}
		}
		if (sEnemy2R == 0x800876b0){
					if (sAnim2R4 == 0x800eaef0){
			if (sEnemy2RTime >= 0x04){
			sAnimR4 = 0x04;}
		else if (sEnemy2RTime <= 0x03){
			sAnimR4 = sEnemy2RTime;
		}
	}
		}
		if (sEnemy2R == 0x80087708){
					if (sAnim2R5 == 0x800eaef0){
			if (sEnemy2RTime >= 0x04){
			sAnimR5 = 0x04;}
		else if (sEnemy2RTime <= 0x03){
			sAnimR5 = sEnemy2RTime;
		}
	}
		}
		if (sEnemy2R == 0x80087760){
					if (sAnim2R6 == 0x800eaef0){
			if (sEnemy2RTime >= 0x04){
			sAnimR6 = 0x04;}
		else if (sEnemy2RTime <= 0x03){
			sAnimR6 = sEnemy2RTime;
		}
	}
		}
		if (sEnemy2R == 0x800877b8){
					if (sAnim2R7 == 0x800eaef0){
			if (sEnemy2RTime >= 0x04){
			sAnimR7 = 0x04;}
		else if (sEnemy2RTime <= 0x03){
			sAnimR7 = sEnemy2RTime;
		}
	}
		}
		if (sEnemy2R == 0x80087910){
					if (sAnim2R8 == 0x800eaef0){
			if (sEnemy2RTime >= 0x04){
			sAnimR8 = 0x04;}
		else if (sEnemy2RTime <= 0x03){
			sAnimR8 = sEnemy2RTime;
		}
	}
		}
		if (sEnemy2R == 0x80087868){
					if (sAnim2R9 == 0x800eaef0){
			if (sEnemy2RTime >= 0x04){
			sAnimR9 = 0x04;}
		else if (sEnemy2RTime <= 0x03){
			sAnimR9 = sEnemy2RTime;
		}
	}
		}
		if (sEnemy2R == 0x800878c0){
					if (sAnim2R10 == 0x800eaef0){
			if (sEnemy2RTime >= 0x04){
			sAnimR10 = 0x04;}
		else if (sEnemy2RTime <= 0x03){
			sAnimR10 = sEnemy2RTime;
		}
	}
		}
		if (sEnemy2R == 0x80087918){
					if (sAnim2R11 == 0x800eaef0){
			if (sEnemy2RTime >= 0x04){
			sAnimR11 = 0x04;}
		else if (sEnemy2RTime <= 0x03){
			sAnimR11 = sEnemy2RTime;
		}
	}
		}
		if (sEnemy2R == 0x80087970){
					if (sAnim2R12 == 0x800eaef0){
			if (sEnemy2RTime >= 0x04){
			sAnimR12 = 0x04;}
		else if (sEnemy2RTime <= 0x03){
			sAnimR12 = sEnemy2RTime;
		}
	}	
		}
		if (sEnemy2R == 0x800879c8){
					if (sAnim2R13 == 0x800eaef0){
			if (sEnemy2RTime >= 0x04){
			sAnimR13 = 0x04;}
		else if (sEnemy2RTime <= 0x03){
			sAnimR13 = sEnemy2RTime;
		}
	}	
		}
		if (sEnemy2R == 0x80087a20){
					if (sAnim2R14 == 0x800eaef0){
			if (sEnemy2RTime >= 0x04){
			sAnimR14 = 0x04;}
		else if (sEnemy2RTime <= 0x03){
			sAnimR14 = sEnemy2RTime;
		}
	}	
		}
		if (sEnemy2R == 0x80087a78){
					if (sAnim2R15 == 0x800eaef0){
			if (sEnemy2RTime >= 0x04){
			sAnimR15 = 0x04;}
		else if (sEnemy2RTime <= 0x03){
			sAnimR15 = sEnemy2RTime;
		}
	}	
		}
		if (sEnemy2R == 0x80087ad0){
					if (sAnim2R16 == 0x800eaef0){
			if (sEnemy2RTime >= 0x04){
			sAnimR16 = 0x04;}
		else if (sEnemy2RTime <= 0x03){
			sAnimR16 = sEnemy2RTime;
		}
	}	
		}
		if (sEnemy2R == 0x80087b28){
					if (sAnim2R17 == 0x800eaef0){
			if (sEnemy2RTime >= 0x04){
			sAnimR17 = 0x04;}
		else if (sEnemy2RTime <= 0x03){
			sAnimR17 = sEnemy2RTime;
		}
	}	
		}
		if (sEnemy2R == 0x80087b80){
					if (sAnim2R18 == 0x800eaef0){
			if (sEnemy2RTime >= 0x04){
			sAnimR18 = 0x04;}
		else if (sEnemy2RTime <= 0x03){
			sAnimR18 = sEnemy2RTime;
		}
	}	
		}
		if (sEnemy2R == 0x80087bd8){
					if (sAnim2R19 == 0x800eaef0){
			if (sEnemy2RTime >= 0x04){
			sAnimR19 = 0x04;}
		else if (sEnemy2RTime <= 0x03){
			sAnimR19 = sEnemy2RTime;
		}
	}	 
		}
		if (sEnemy2R == 0x80087c30){
					if (sAnim2R20 == 0x800eaef0){
			if (sEnemy2RTime >= 0x04){
			sAnimR20 = 0x04;}
		else if (sEnemy2RTime <= 0x03){
			sAnimR20 = sEnemy2RTime;
		}
	}	
		}
		if (sEnemy2R == 0x80087c88){
					if (sAnim2R21 == 0x800eaef0){
			if (sEnemy2RTime >= 0x04){
			sAnimR21 = 0x04;}
		else if (sEnemy2RTime <= 0x03){
			sAnimR21 = sEnemy2RTime;
		}
	}
		}
		if (sEnemy3R == 0x800875a8){
		if (sAnim2R1 == 0x800eaef0){
			if (sEnemy3RTime >= 0x04){
				sAnimR1 = 0x04;}
			else if (sEnemy3RTime <= 0x03){
				sAnimR1 = sEnemy3RTime;
			}
		}
	}
		if (sEnemy3R == 0x80087600){
					if (sAnim2R2 == 0x800eaef0){
			if (sEnemy3RTime >= 0x04){
			sAnimR2 = 0x04;}
		else if (sEnemy3RTime <= 0x03){
			sAnimR2 = sEnemy3RTime;
		}
	}
		}
		if (sEnemy3R == 0x80087658){
					if (sAnim2R3 == 0x800eaef0){
			if (sEnemy3RTime >= 0x04){
			sAnimR3 = 0x04;}
		else if (sEnemy3RTime <= 0x03){
			sAnimR3 = sEnemy3RTime;
		}
	}
		}
		if (sEnemy3R == 0x800876b0){
					if (sAnim2R4 == 0x800eaef0){
			if (sEnemy3RTime >= 0x04){
			sAnimR4 = 0x04;}
		else if (sEnemy3RTime <= 0x03){
			sAnimR4 = sEnemy3RTime;
		}
	}
		}
		if (sEnemy3R == 0x80087708){
					if (sAnim2R5 == 0x800eaef0){
			if (sEnemy3RTime >= 0x04){
			sAnimR5 = 0x04;}
		else if (sEnemy3RTime <= 0x03){
			sAnimR5 = sEnemy3RTime;
		}
	}
		}
		if (sEnemy3R == 0x80087760){
					if (sAnim2R6 == 0x800eaef0){
			if (sEnemy3RTime >= 0x04){
			sAnimR6 = 0x04;}
		else if (sEnemy3RTime <= 0x03){
			sAnimR6 = sEnemy3RTime;
		}
	}
		}
		if (sEnemy3R == 0x800877b8){
					if (sAnim2R7 == 0x800eaef0){
			if (sEnemy3RTime >= 0x04){
			sAnimR7 = 0x04;}
		else if (sEnemy3RTime <= 0x03){
			sAnimR7 = sEnemy3RTime;
		}
	}
		}
		if (sEnemy3R == 0x80087910){
					if (sAnim2R8 == 0x800eaef0){
			if (sEnemy3RTime >= 0x04){
			sAnimR8 = 0x04;}
		else if (sEnemy3RTime <= 0x03){
			sAnimR8 = sEnemy3RTime;
		}
	}
		}
		if (sEnemy3R == 0x80087868){
					if (sAnim2R9 == 0x800eaef0){
			if (sEnemy3RTime >= 0x04){
			sAnimR9 = 0x04;}
		else if (sEnemy3RTime <= 0x03){
			sAnimR9 = sEnemy3RTime;
		}
	}
		}
		if (sEnemy3R == 0x800878c0){
					if (sAnim2R10 == 0x800eaef0){
			if (sEnemy3RTime >= 0x04){
			sAnimR10 = 0x04;}
		else if (sEnemy3RTime <= 0x03){
			sAnimR10 = sEnemy3RTime;
		}
	}
		}
		if (sEnemy3R == 0x80087918){
					if (sAnim2R11 == 0x800eaef0){
			if (sEnemy3RTime >= 0x04){
			sAnimR11 = 0x04;}
		else if (sEnemy3RTime <= 0x03){
			sAnimR11 = sEnemy3RTime;
		}
	}
		}
		if (sEnemy3R == 0x80087970){
					if (sAnim2R12 == 0x800eaef0){
			if (sEnemy3RTime >= 0x04){
			sAnimR12 = 0x04;}
		else if (sEnemy3RTime <= 0x03){
			sAnimR12 = sEnemy3RTime;
		}
	}	
		}
		if (sEnemy3R == 0x800879c8){
					if (sAnim2R13 == 0x800eaef0){
			if (sEnemy3RTime >= 0x04){
			sAnimR13 = 0x04;}
		else if (sEnemy3RTime <= 0x03){
			sAnimR13 = sEnemy3RTime;
		}
	}	
		}
		if (sEnemy3R == 0x80087a20){
					if (sAnim2R14 == 0x800eaef0){
			if (sEnemy3RTime >= 0x04){
			sAnimR14 = 0x04;}
		else if (sEnemy3RTime <= 0x03){
			sAnimR14 = sEnemy3RTime;
		}
	}	
		}
		if (sEnemy3R == 0x80087a78){
					if (sAnim2R15 == 0x800eaef0){
			if (sEnemy3RTime >= 0x04){
			sAnimR15 = 0x04;}
		else if (sEnemy3RTime <= 0x03){
			sAnimR15 = sEnemy3RTime;
		}
	}	
		}
		if (sEnemy3R == 0x80087ad0){
					if (sAnim2R16 == 0x800eaef0){
			if (sEnemy3RTime >= 0x04){
			sAnimR16 = 0x04;}
		else if (sEnemy3RTime <= 0x03){
			sAnimR16 = sEnemy3RTime;
		}
	}	
		}
		if (sEnemy3R == 0x80087b28){
					if (sAnim2R17 == 0x800eaef0){
			if (sEnemy3RTime >= 0x04){
			sAnimR17 = 0x04;}
		else if (sEnemy3RTime <= 0x03){
			sAnimR17 = sEnemy3RTime;
		}
	}	
		}
		if (sEnemy3R == 0x80087b80){
					if (sAnim2R18 == 0x800eaef0){
			if (sEnemy3RTime >= 0x04){
			sAnimR18 = 0x04;}
		else if (sEnemy3RTime <= 0x03){
			sAnimR18 = sEnemy3RTime;
		}
	}	
		}
		if (sEnemy3R == 0x80087bd8){
					if (sAnim2R19 == 0x800eaef0){
			if (sEnemy3RTime >= 0x04){
			sAnimR19 = 0x04;}
		else if (sEnemy3RTime <= 0x03){
			sAnimR19 = sEnemy3RTime;
		}
	}	 
		}
		if (sEnemy3R == 0x80087c30){
					if (sAnim2R20 == 0x800eaef0){
			if (sEnemy3RTime >= 0x04){
			sAnimR20 = 0x04;}
		else if (sEnemy3RTime <= 0x03){
			sAnimR20 = sEnemy3RTime;
		}
	}	
		}
		if (sEnemy3R == 0x80087c88){
					if (sAnim2R21 == 0x800eaef0){
			if (sEnemy3RTime >= 0x04){
			sAnimR21 = 0x04;}
		else if (sEnemy3RTime <= 0x03){
			sAnimR21 = sEnemy3RTime;
		}
	}
		}
		if (sEnemy4R == 0x800875a8){
		if (sAnim2R1 == 0x800eaef0){
			if (sEnemy4RTime >= 0x04){
				sAnimR1 = 0x04;}
			else if (sEnemy4RTime <= 0x03){
				sAnimR1 = sEnemy4RTime;
			}
		}
	}
		if (sEnemy4R == 0x80087600){
					if (sAnim2R2 == 0x800eaef0){
			if (sEnemy4RTime >= 0x04){
			sAnimR2 = 0x04;}
		else if (sEnemy4RTime <= 0x03){
			sAnimR2 = sEnemy4RTime;
		}
	}
		}
		if (sEnemy4R == 0x80087658){
					if (sAnim2R3 == 0x800eaef0){
			if (sEnemy4RTime >= 0x04){
			sAnimR3 = 0x04;}
		else if (sEnemy4RTime <= 0x03){
			sAnimR3 = sEnemy4RTime;
		}
	}
		}
		if (sEnemy4R == 0x800876b0){
					if (sAnim2R4 == 0x800eaef0){
			if (sEnemy4RTime >= 0x04){
			sAnimR4 = 0x04;}
		else if (sEnemy4RTime <= 0x03){
			sAnimR4 = sEnemy4RTime;
		}
	}
		}
		if (sEnemy4R == 0x80087708){
					if (sAnim2R5 == 0x800eaef0){
			if (sEnemy4RTime >= 0x04){
			sAnimR5 = 0x04;}
		else if (sEnemy4RTime <= 0x03){
			sAnimR5 = sEnemy4RTime;
		}
	}
		}
		if (sEnemy4R == 0x80087760){
					if (sAnim2R6 == 0x800eaef0){
			if (sEnemy4RTime >= 0x04){
			sAnimR6 = 0x04;}
		else if (sEnemy4RTime <= 0x03){
			sAnimR6 = sEnemy4RTime;
		}
	}
		}
		if (sEnemy4R == 0x800877b8){
					if (sAnim2R7 == 0x800eaef0){
			if (sEnemy4RTime >= 0x04){
			sAnimR7 = 0x04;}
		else if (sEnemy4RTime <= 0x03){
			sAnimR7 = sEnemy4RTime;
		}
	}
		}
		if (sEnemy4R == 0x80087910){
					if (sAnim2R8 == 0x800eaef0){
			if (sEnemy4RTime >= 0x04){
			sAnimR8 = 0x04;}
		else if (sEnemy4RTime <= 0x03){
			sAnimR8 = sEnemy4RTime;
		}
	}
		}
		if (sEnemy4R == 0x80087868){
					if (sAnim2R9 == 0x800eaef0){
			if (sEnemy4RTime >= 0x04){
			sAnimR9 = 0x04;}
		else if (sEnemy4RTime <= 0x03){
			sAnimR9 = sEnemy4RTime;
		}
	}
		}
		if (sEnemy4R == 0x800878c0){
					if (sAnim2R10 == 0x800eaef0){
			if (sEnemy4RTime >= 0x04){
			sAnimR10 = 0x04;}
		else if (sEnemy4RTime <= 0x03){
			sAnimR10 = sEnemy4RTime;
		}
	}
		}
		if (sEnemy4R == 0x80087918){
					if (sAnim2R11 == 0x800eaef0){
			if (sEnemy4RTime >= 0x04){
			sAnimR11 = 0x04;}
		else if (sEnemy4RTime <= 0x03){
			sAnimR11 = sEnemy4RTime;
		}
	}
		}
		if (sEnemy4R == 0x80087970){
					if (sAnim2R12 == 0x800eaef0){
			if (sEnemy4RTime >= 0x04){
			sAnimR12 = 0x04;}
		else if (sEnemy4RTime <= 0x03){
			sAnimR12 = sEnemy4RTime;
		}
	}	
		}
		if (sEnemy4R == 0x800879c8){
					if (sAnim2R13 == 0x800eaef0){
			if (sEnemy4RTime >= 0x04){
			sAnimR13 = 0x04;}
		else if (sEnemy4RTime <= 0x03){
			sAnimR13 = sEnemy4RTime;
		}
	}	
		}
		if (sEnemy4R == 0x80087a20){
					if (sAnim2R14 == 0x800eaef0){
			if (sEnemy4RTime >= 0x04){
			sAnimR14 = 0x04;}
		else if (sEnemy4RTime <= 0x03){
			sAnimR14 = sEnemy4RTime;
		}
	}	
		}
		if (sEnemy4R == 0x80087a78){
					if (sAnim2R15 == 0x800eaef0){
			if (sEnemy4RTime >= 0x04){
			sAnimR15 = 0x04;}
		else if (sEnemy4RTime <= 0x03){
			sAnimR15 = sEnemy4RTime;
		}
	}	
		}
		if (sEnemy4R == 0x80087ad0){
					if (sAnim2R16 == 0x800eaef0){
			if (sEnemy4RTime >= 0x04){
			sAnimR16 = 0x04;}
		else if (sEnemy4RTime <= 0x03){
			sAnimR16 = sEnemy4RTime;
		}
	}	
		}
		if (sEnemy4R == 0x80087b28){
					if (sAnim2R17 == 0x800eaef0){
			if (sEnemy4RTime >= 0x04){
			sAnimR17 = 0x04;}
		else if (sEnemy4RTime <= 0x03){
			sAnimR17 = sEnemy4RTime;
		}
	}	
		}
		if (sEnemy4R == 0x80087b80){
					if (sAnim2R18 == 0x800eaef0){
			if (sEnemy4RTime >= 0x04){
			sAnimR18 = 0x04;}
		else if (sEnemy4RTime <= 0x03){
			sAnimR18 = sEnemy4RTime;
		}
	}	
		}
		if (sEnemy4R == 0x80087bd8){
					if (sAnim2R19 == 0x800eaef0){
			if (sEnemy4RTime >= 0x04){
			sAnimR19 = 0x04;}
		else if (sEnemy4RTime <= 0x03){
			sAnimR19 = sEnemy4RTime;
		}
	}	 
		}
		if (sEnemy4R == 0x80087c30){
					if (sAnim2R20 == 0x800eaef0){
			if (sEnemy4RTime >= 0x04){
			sAnimR20 = 0x04;}
		else if (sEnemy4RTime <= 0x03){
			sAnimR20 = sEnemy4RTime;
		}
	}	
		}
		if (sEnemy4R == 0x80087c88){
					if (sAnim2R21 == 0x800eaef0){
			if (sEnemy4RTime >= 0x04){
			sAnimR21 = 0x04;}
		else if (sEnemy4RTime <= 0x03){
			sAnimR21 = sEnemy4RTime;
		}
	}
		}
		if (sEnemy5R == 0x800875a8){
		if (sAnim2R1 == 0x800eaef0){
			if (sEnemy5RTime >= 0x04){
				sAnimR1 = 0x04;}
			else if (sEnemy5RTime <= 0x03){
				sAnimR1 = sEnemy5RTime;
			}
		}
	}
		if (sEnemy5R == 0x80087600){
					if (sAnim2R2 == 0x800eaef0){
			if (sEnemy5RTime >= 0x04){
			sAnimR2 = 0x04;}
		else if (sEnemy5RTime <= 0x03){
			sAnimR2 = sEnemy5RTime;
		}
	}
		}
		if (sEnemy5R == 0x80087658){
					if (sAnim2R3 == 0x800eaef0){
			if (sEnemy5RTime >= 0x04){
			sAnimR3 = 0x04;}
		else if (sEnemy5RTime <= 0x03){
			sAnimR3 = sEnemy5RTime;
		}
	}
		}
		if (sEnemy5R == 0x800876b0){
					if (sAnim2R4 == 0x800eaef0){
			if (sEnemy5RTime >= 0x04){
			sAnimR4 = 0x04;}
		else if (sEnemy5RTime <= 0x03){
			sAnimR4 = sEnemy5RTime;
		}
	}
		}
		if (sEnemy5R == 0x80087708){
					if (sAnim2R5 == 0x800eaef0){
			if (sEnemy5RTime >= 0x04){
			sAnimR5 = 0x04;}
		else if (sEnemy5RTime <= 0x03){
			sAnimR5 = sEnemy5RTime;
		}
	}
		}
		if (sEnemy5R == 0x80087760){
					if (sAnim2R6 == 0x800eaef0){
			if (sEnemy5RTime >= 0x04){
			sAnimR6 = 0x04;}
		else if (sEnemy5RTime <= 0x03){
			sAnimR6 = sEnemy5RTime;
		}
	}
		}
		if (sEnemy5R == 0x800877b8){
					if (sAnim2R7 == 0x800eaef0){
			if (sEnemy5RTime >= 0x04){
			sAnimR7 = 0x04;}
		else if (sEnemy5RTime <= 0x03){
			sAnimR7 = sEnemy5RTime;
		}
	}
		}
		if (sEnemy5R == 0x80087910){
					if (sAnim2R8 == 0x800eaef0){
			if (sEnemy5RTime >= 0x04){
			sAnimR8 = 0x04;}
		else if (sEnemy5RTime <= 0x03){
			sAnimR8 = sEnemy5RTime;
		}
	}
		}
		if (sEnemy5R == 0x80087868){
					if (sAnim2R9 == 0x800eaef0){
			if (sEnemy5RTime >= 0x04){
			sAnimR9 = 0x04;}
		else if (sEnemy5RTime <= 0x03){
			sAnimR9 = sEnemy5RTime;
		}
	}
		}
		if (sEnemy5R == 0x800878c0){
					if (sAnim2R10 == 0x800eaef0){
			if (sEnemy5RTime >= 0x04){
			sAnimR10 = 0x04;}
		else if (sEnemy5RTime <= 0x03){
			sAnimR10 = sEnemy5RTime;
		}
	}
		}
		if (sEnemy5R == 0x80087918){
					if (sAnim2R11 == 0x800eaef0){
			if (sEnemy5RTime >= 0x04){
			sAnimR11 = 0x04;}
		else if (sEnemy5RTime <= 0x03){
			sAnimR11 = sEnemy5RTime;
		}
	}
		}
		if (sEnemy5R == 0x80087970){
					if (sAnim2R12 == 0x800eaef0){
			if (sEnemy5RTime >= 0x04){
			sAnimR12 = 0x04;}
		else if (sEnemy5RTime <= 0x03){
			sAnimR12 = sEnemy5RTime;
		}
	}	
		}
		if (sEnemy5R == 0x800879c8){
					if (sAnim2R13 == 0x800eaef0){
			if (sEnemy5RTime >= 0x04){
			sAnimR13 = 0x04;}
		else if (sEnemy5RTime <= 0x03){
			sAnimR13 = sEnemy5RTime;
		}
	}	
		}
		if (sEnemy5R == 0x80087a20){
					if (sAnim2R14 == 0x800eaef0){
			if (sEnemy5RTime >= 0x04){
			sAnimR14 = 0x04;}
		else if (sEnemy5RTime <= 0x03){
			sAnimR14 = sEnemy5RTime;
		}
	}	
		}
		if (sEnemy5R == 0x80087a78){
					if (sAnim2R15 == 0x800eaef0){
			if (sEnemy5RTime >= 0x04){
			sAnimR15 = 0x04;}
		else if (sEnemy5RTime <= 0x03){
			sAnimR15 = sEnemy5RTime;
		}
	}	
		}
		if (sEnemy5R == 0x80087ad0){
					if (sAnim2R16 == 0x800eaef0){
			if (sEnemy5RTime >= 0x04){
			sAnimR16 = 0x04;}
		else if (sEnemy5RTime <= 0x03){
			sAnimR16 = sEnemy5RTime;
		}
	}	
		}
		if (sEnemy5R == 0x80087b28){
					if (sAnim2R17 == 0x800eaef0){
			if (sEnemy5RTime >= 0x04){
			sAnimR17 = 0x04;}
		else if (sEnemy5RTime <= 0x03){
			sAnimR17 = sEnemy5RTime;
		}
	}	
		}
		if (sEnemy5R == 0x80087b80){
					if (sAnim2R18 == 0x800eaef0){
			if (sEnemy5RTime >= 0x04){
			sAnimR18 = 0x04;}
		else if (sEnemy5RTime <= 0x03){
			sAnimR18 = sEnemy5RTime;
		}
	}	
		}
		if (sEnemy5R == 0x80087bd8){
					if (sAnim2R19 == 0x800eaef0){
			if (sEnemy5RTime >= 0x04){
			sAnimR19 = 0x04;}
		else if (sEnemy5RTime <= 0x03){
			sAnimR19 = sEnemy5RTime;
		}
	}	 
		}
		if (sEnemy5R == 0x80087c30){
					if (sAnim2R20 == 0x800eaef0){
			if (sEnemy5RTime >= 0x04){
			sAnimR20 = 0x04;}
		else if (sEnemy5RTime <= 0x03){
			sAnimR20 = sEnemy5RTime;
		}
	}	
		}
		if (sEnemy5R == 0x80087c88){
					if (sAnim2R21 == 0x800eaef0){
			if (sEnemy5RTime >= 0x04){
			sAnimR21 = 0x04;}
		else if (sEnemy5RTime <= 0x03){
			sAnimR21 = sEnemy5RTime;
		}
	}
		}
		if (sEnemy6R == 0x800875a8){
		if (sAnim2R1 == 0x800eaef0){
			if (sEnemy6RTime >= 0x04){
				sAnimR1 = 0x04;}
			else if (sEnemy6RTime <= 0x03){
				sAnimR1 = sEnemy6RTime;
			}
		}
	}
		if (sEnemy6R == 0x80087600){
					if (sAnim2R2 == 0x800eaef0){
			if (sEnemy6RTime >= 0x04){
			sAnimR2 = 0x04;}
		else if (sEnemy6RTime <= 0x03){
			sAnimR2 = sEnemy6RTime;
		}
	}
		}
		if (sEnemy6R == 0x80087658){
					if (sAnim2R3 == 0x800eaef0){
			if (sEnemy6RTime >= 0x04){
			sAnimR3 = 0x04;}
		else if (sEnemy6RTime <= 0x03){
			sAnimR3 = sEnemy6RTime;
		}
	}
		}
		if (sEnemy6R == 0x800876b0){
					if (sAnim2R4 == 0x800eaef0){
			if (sEnemy6RTime >= 0x04){
			sAnimR4 = 0x04;}
		else if (sEnemy6RTime <= 0x03){
			sAnimR4 = sEnemy6RTime;
		}
	}
		}
		if (sEnemy6R == 0x80087708){
					if (sAnim2R5 == 0x800eaef0){
			if (sEnemy6RTime >= 0x04){
			sAnimR5 = 0x04;}
		else if (sEnemy6RTime <= 0x03){
			sAnimR5 = sEnemy6RTime;
		}
	}
		}
		if (sEnemy6R == 0x80087760){
					if (sAnim2R6 == 0x800eaef0){
			if (sEnemy6RTime >= 0x04){
			sAnimR6 = 0x04;}
		else if (sEnemy6RTime <= 0x03){
			sAnimR6 = sEnemy6RTime;
		}
	}
		}
		if (sEnemy6R == 0x800877b8){
					if (sAnim2R7 == 0x800eaef0){
			if (sEnemy6RTime >= 0x04){
			sAnimR7 = 0x04;}
		else if (sEnemy6RTime <= 0x03){
			sAnimR7 = sEnemy6RTime;
		}
	}
		}
		if (sEnemy6R == 0x80087910){
					if (sAnim2R8 == 0x800eaef0){
			if (sEnemy6RTime >= 0x04){
			sAnimR8 = 0x04;}
		else if (sEnemy6RTime <= 0x03){
			sAnimR8 = sEnemy6RTime;
		}
	}
		}
		if (sEnemy6R == 0x80087868){
					if (sAnim2R9 == 0x800eaef0){
			if (sEnemy6RTime >= 0x04){
			sAnimR9 = 0x04;}
		else if (sEnemy6RTime <= 0x03){
			sAnimR9 = sEnemy6RTime;
		}
	}
		}
		if (sEnemy6R == 0x800878c0){
					if (sAnim2R10 == 0x800eaef0){
			if (sEnemy6RTime >= 0x04){
			sAnimR10 = 0x04;}
		else if (sEnemy6RTime <= 0x03){
			sAnimR10 = sEnemy6RTime;
		}
	}
		}
		if (sEnemy6R == 0x80087918){
					if (sAnim2R11 == 0x800eaef0){
			if (sEnemy6RTime >= 0x04){
			sAnimR11 = 0x04;}
		else if (sEnemy6RTime <= 0x03){
			sAnimR11 = sEnemy6RTime;
		}
	}
		}
		if (sEnemy6R == 0x80087970){
					if (sAnim2R12 == 0x800eaef0){
			if (sEnemy6RTime >= 0x04){
			sAnimR12 = 0x04;}
		else if (sEnemy6RTime <= 0x03){
			sAnimR12 = sEnemy6RTime;
		}
	}	
		}
		if (sEnemy6R == 0x800879c8){
					if (sAnim2R13 == 0x800eaef0){
			if (sEnemy6RTime >= 0x04){
			sAnimR13 = 0x04;}
		else if (sEnemy6RTime <= 0x03){
			sAnimR13 = sEnemy6RTime;
		}
	}	
		}
		if (sEnemy6R == 0x80087a20){
					if (sAnim2R14 == 0x800eaef0){
			if (sEnemy6RTime >= 0x04){
			sAnimR14 = 0x04;}
		else if (sEnemy6RTime <= 0x03){
			sAnimR14 = sEnemy6RTime;
		}
	}	
		}
		if (sEnemy6R == 0x80087a78){
					if (sAnim2R15 == 0x800eaef0){
			if (sEnemy6RTime >= 0x04){
			sAnimR15 = 0x04;}
		else if (sEnemy6RTime <= 0x03){
			sAnimR15 = sEnemy6RTime;
		}
	}	
		}
		if (sEnemy6R == 0x80087ad0){
					if (sAnim2R16 == 0x800eaef0){
			if (sEnemy6RTime >= 0x04){
			sAnimR16 = 0x04;}
		else if (sEnemy6RTime <= 0x03){
			sAnimR16 = sEnemy6RTime;
		}
	}	
		}
		if (sEnemy6R == 0x80087b28){
					if (sAnim2R17 == 0x800eaef0){
			if (sEnemy6RTime >= 0x04){
			sAnimR17 = 0x04;}
		else if (sEnemy6RTime <= 0x03){
			sAnimR17 = sEnemy6RTime;
		}
	}	
		}
		if (sEnemy6R == 0x80087b80){
					if (sAnim2R18 == 0x800eaef0){
			if (sEnemy6RTime >= 0x04){
			sAnimR18 = 0x04;}
		else if (sEnemy6RTime <= 0x03){
			sAnimR18 = sEnemy6RTime;
		}
	}	
		}
		if (sEnemy6R == 0x80087bd8){
					if (sAnim2R19 == 0x800eaef0){
			if (sEnemy6RTime >= 0x04){
			sAnimR19 = 0x04;}
		else if (sEnemy6RTime <= 0x03){
			sAnimR19 = sEnemy6RTime;
		}
	}	 
		}
		if (sEnemy6R == 0x80087c30){
					if (sAnim2R20 == 0x800eaef0){
			if (sEnemy6RTime >= 0x04){
			sAnimR20 = 0x04;}
		else if (sEnemy6RTime <= 0x03){
			sAnimR20 = sEnemy6RTime;
		}
	}	
		}
		if (sEnemy6R == 0x80087c88){
					if (sAnim2R21 == 0x800eaef0){
			if (sEnemy6RTime >= 0x04){
			sAnimR21 = 0x04;}
		else if (sEnemy6RTime <= 0x03){
			sAnimR21 = sEnemy6RTime;
		}
	}
		}
//

//Enemy Spell Adjust per Region
	if (gCurrentMap == 0x02 || gCurrentMap == 0x1F){
		gFireball = 0x00010000;
		gWindcutter = 0x00020003;
	}

	if (gCurrentMap == 0x03 || gCurrentMap == 0x20 || gCurrentMap == 0x05 || gCurrentMap == 0x1B){
		gFireball = 0x020000;
		gWindcutter = 0x00030003;
	}

	if (gCurrentMap == 0x21 || gCurrentMap == 0x1A || gCurrentMap == 0x0C || gCurrentMap == 0x07 || gCurrentMap == 0x1C){
		gFireball = 0x00020000;
		gWindcutter = 0x00040003;
	}

	if (gCurrentMap == 0x09 || gCurrentMap == 0x1D || gCurrentMap == 0x0B || gCurrentMap == 0x1E || gCurrentMap == 0x22){
		gFireball = 0x00030000;
		gWindcutter = 0x00060003;
	}
//
	
//Auto Revive
	if (gSpellCast == 0x0000030C){
		if (gSpellTimer == 0x00010000){
			if (gAutoRevive == 0x00){
				gAutoRevive = 0x01;
				sBrianBA |= 1;
				sBrianBAT = 0xFF;
				sBrianB = 0x01;
			}
		}
	}

	if (gAutoRevive == 0x01){
		if (sBrianHP == 0x0000){
			sBrianHP = sBrianMAXHP * 0.25;
			gAutoRevive = 0x00;
			sBrianBA |= 0;
			sBrianBAT = 0x00;
			sBrianB = 0x00;
		}
	}
	
	if (gAutoRevive == 0x01){
		if (gBattleState2 == 0x00000000 || gBattleState2 == 0x00000016 || gBattleState2 == 0x00000406){
			gAutoRevive = 0x00;
		}
	}
//Auto Revive End


//Encounter Rate Toggle On
	if (gSpellCast == 0x00000309) {
			if ((gEncounterFlag & 1) == 0x01){
					if (gSpellTimer == 0x00040000){
						gEncounterFloat = 0x49;
						gEncounterStep = 0x32;
						gEncounterMax = 0x07D0;
						gEncounterFlag = 0x00;
						gSoftSteps = 0x00400002;
						gSpellTimer = gSpellTimer - 0x00030000;
					}
			}
			
			else if ((gEncounterFlag & 1) == 0x00){
					if (gSpellTimer == 0x00030000){
						gEncounterFloat = 0x59;
						gEncounterStep = 0x64;
						gEncounterMax = 0x0FA0;
						gEncounterFlag = 0x01;
						gSoftSteps = 0x00400004;
						gSpellTimer = gSpellTimer - 0x00020000;
					}
				}
			}
	
//Encounter Rate Toggle End

//D-Pad Up to Save
//	if (gGameButton == 0x08000000) {
//if (gCurrentMap == 0x0f || gCurrentMap == 0x10 || gCurrentMap == 0x11 || gCurrentMap == 0x12 || gCurrentMap == //0x14 || gCurrentMap == 0x16 || gCurrentMap == 0x17 || gCurrentMap == 0x19 || gCurrentMap == 0x1a || gCurrentMap == 0x1e) {
//			if (!(gCurrentMap == 0x1a && gNextSubmap == 0x01)) {
//				if (!(gCurrentMap == 0x17 && (gNextSubmap == 0x00 || gNextSubmap == 0x08))) {
//			gGameState = 0x00000480;
//			gGameMode = 0x00020000;
//			gGameState = 0x000005a0;
//			}
//			}
//		}
//	}

//Shop Attempt Dondoran
	if (gCurrentMap == 0x10) {
			if (gNextSubmap == 0x0D) {
				if (eMaggietext == 0x3A68 || eMaggietext == 0xDF00)  {
					if (eTextlookup == 0x80c00E18) {
						if (cbuttonpressed == 0x08){
							if (sBrianStone >= 50){
								sBrianItemPickup = 0x01;
								eTextopen = 0x00000000;
								eTextID = 0x00000000;
								sBrianStone = sBrianStone - 50;
							}
					}
						if (cbuttonpressed == 0x02){
							if (sBrianStone >= 75){
								sBrianItemPickup = 0x05;
								eTextopen = 0x00000000;
								eTextID = 0x00000000;
								sBrianStone = sBrianStone - 75;
							}
					}
						if (cbuttonpressed == 0x04){
							if (sBrianStone >= 100){
								sBrianItemPickup = 0x0C;
								eTextopen = 0x00000000;
								eTextID = 0x00000000;
								sBrianStone = sBrianStone - 100;
							}
					}
						if (cbuttonpressed == 0x01){
							if (sBrianStone >= 200){
								sBrianItemPickup = 0x0A;
								eTextopen = 0x00000000;
								eTextID = 0x00000000;
								sBrianStone = sBrianStone - 200;
							
							}
						}
					}
				if ((sBrianStone) >= 50){	
				eMaggietext = 0xDF00;
				}
				else
					eMaggietext = 0x3A68;
				}
				}
		}
		
	//Shop Attempt Larapool
	if (gCurrentMap == 0x11) {
		if (gNextSubmap == 0x00) {
			if (eGalitatext == 0x5AD0 || eGalitatext == 0xE000)  {
				if (eTextlookup == 0x80c00E18) {
						if (cbuttonpressed == 0x08){
							if (sBrianStone >= 50){
								sBrianItemPickup = 0x01;
								eTextopen = 0x00000000;
								eTextID = 0x00000000;
								sBrianStone = sBrianStone - 50;
							}
					}
						if (cbuttonpressed == 0x02){
							if (sBrianStone >= 75){
								sBrianItemPickup = 0x05;
								eTextopen = 0x00000000;
								eTextID = 0x00000000;
								sBrianStone = sBrianStone - 75;
							}
					}
						if (cbuttonpressed == 0x04){
							if (sBrianStone >= 175){
								sBrianItemPickup = 0x06;
								eTextopen = 0x00000000;
								eTextID = 0x00000000;
								sBrianStone = sBrianStone - 175;
							}
					}
						if (cbuttonpressed == 0x01){
							if (sBrianStone >= 250){
								sBrianItemPickup = 0x0B;
								eTextopen = 0x00000000;
								eTextID = 0x00000000;
								sBrianStone = sBrianStone - 250;
							
							}
						}
					}
				if ((sBrianStone) >= 50){	
				eGalitatext = 0xE000;
				}
				else
					eGalitatext = 0x5AD0;
				}
		}
		}
		
		//Shop Attempt Normoon
	if (gCurrentMap == 0x14) {
			if (gNextSubmap == 0x02) {
				if (eHectortext == 0x61D0 || eHectortext == 0xE100)  {
					if (eTextlookup == 0x80c00E18) {
						if (cbuttonpressed == 0x08){
							if (sBrianStone >= 50){
								sBrianItemPickup = 0x01;
								eTextopen = 0x00000000;
								eTextID = 0x00000000;
								sBrianStone = sBrianStone - 50;
							}
					}
						if (cbuttonpressed == 0x02){
							if (sBrianStone >= 150){
								sBrianItemPickup = 0x02;
								eTextopen = 0x00000000;
								eTextID = 0x00000000;
								sBrianStone = sBrianStone - 150;
							}
					}
						if (cbuttonpressed == 0x04){
							if (sBrianStone >= 75){
								sBrianItemPickup = 0x05;
								eTextopen = 0x00000000;
								eTextID = 0x00000000;
								sBrianStone = sBrianStone - 75;
							}
					}
						if (cbuttonpressed == 0x01){
							if (sBrianStone >= 275){
								sBrianItemPickup = 0x08;
								eTextopen = 0x00000000;
								eTextID = 0x00000000;
								sBrianStone = sBrianStone - 275;
							
							}
						}
					}
				if ((sBrianStone) >= 50){	
				eHectortext = 0xE100;
				}
				else
					eHectortext = 0x61D0;
				}
				}
		}
		
		//Shop Attempt Limelin
	if (gCurrentMap == 0x16) {
			if (gNextSubmap == 0x14) {
				if (eRoachtext == 0x7570 || eRoachtext == 0xE200)  {
					if (eTextlookup == 0x80c00E18) {
						if (cbuttonpressed == 0x08){
							if (sBrianStone >= 150){
								sBrianItemPickup = 0x02;
								eTextopen = 0x00000000;
								eTextID = 0x00000000;
								sBrianStone = sBrianStone - 150;
							}
					}
						if (cbuttonpressed == 0x02){
							if (sBrianStone >= 175){
								sBrianItemPickup = 0x06;
								eTextopen = 0x00000000;
								eTextID = 0x00000000;
								sBrianStone = sBrianStone - 175;
							}
					}
						if (cbuttonpressed == 0x04){
							if (sBrianStone >= 275){
								sBrianItemPickup = 0x08;
								eTextopen = 0x00000000;
								eTextID = 0x00000000;
								sBrianStone = sBrianStone - 275;
							}
					}
						if (cbuttonpressed == 0x01){
							if (sBrianStone >= 200){
								sBrianItemPickup = 0x0A;
								eTextopen = 0x00000000;
								eTextID = 0x00000000;
								sBrianStone = sBrianStone - 200;
							
							}
						}
					}
				if ((sBrianStone) >= 150){	
				eRoachtext = 0xE200;
				}
				else
					eRoachtext = 0x7570;
				}
				}
		}
		
		//Shop Attempt Greenoch
	if (gCurrentMap == 0x17) {
			if (gNextSubmap == 0x05) {
				if (eBronzetext == 0xB490 || eBronzetext == 0xE300)  {
					if (eTextlookup == 0x80c00E18) {
						if (cbuttonpressed == 0x08){
							if (sBrianStone >= 300){
								sBrianItemPickup = 0x03;
								eTextopen = 0x00000000;
								eTextID = 0x00000000;
								sBrianStone = sBrianStone - 300;
							}
					}
						if (cbuttonpressed == 0x02){
							if (sBrianStone >= 350){
								sBrianItemPickup = 0x07;
								eTextopen = 0x00000000;
								eTextID = 0x00000000;
								sBrianStone = sBrianStone - 350;
							}
					}
						if (cbuttonpressed == 0x04){
							if (sBrianStone >= 350){
								sBrianItemPickup = 0x09;
								eTextopen = 0x00000000;
								eTextID = 0x00000000;
								sBrianStone = sBrianStone - 350;
							}
					}
						if (cbuttonpressed == 0x01){
							if (sBrianStone >= 250){
								sBrianItemPickup = 0x0B;
								eTextopen = 0x00000000;
								eTextID = 0x00000000;
								sBrianStone = sBrianStone - 250;
							
							}
						}
					}
				if ((sBrianStone) >= 250){	
				eBronzetext = 0xE300;
				}
				else
					eBronzetext = 0xb490;
				}
				}
		}

	//D-Pad Up to save
		if (gGameButton == 0x08000000) {
			if (gGameState == 0x00000000) {
				if (gBattleState4 == 0x0000 || gBattleState4 == 0x0016 || gBattleState4 == 0x0406){
					if (gCurrentMap == 0x0f || gCurrentMap == 0x10 || gCurrentMap == 0x11 || gCurrentMap == 0x12 || gCurrentMap == 0x14 || gCurrentMap == 0x16 || (gCurrentMap == 0x17 && gNextSubmap == 0x00) || (gCurrentMap == 0x17 && gNextSubmap == 0x08) || (gCurrentMap == 0x17 && gNextSubmap == 0x16) || gCurrentMap == 0x19 || gCurrentMap == 0x1a || gCurrentMap == 0x1e) {
			//Can Save anywhere in Melrode, Dondoran, Larapool, Normoon, Limelin, Brannoch Castle, Only on 1st floor of Greenoch INN, Only on First Floor of Bation's INN, only on First Floor of Brannoch INN.
						if (!(gCurrentMap == 0x1a && gNextSubmap == 0x01)) {
							gGameState = 0x00000480;
							gGameMode = 0x00020000;
							gGameState = 0x000005a0;
						}
					}
				}
			}
		}
		
		//Save Music
		if (gGameState == 0x000005A0){
			if (gGameMode == 0x00020000){
				if (!(eBGM == 0x1313)){
					eBGM = 0x1313;
					eBGMTimer = 0x0001;
				}
			}
		}
// End Save Music

//Elements Reset
if (gCurrentMap == 0x10) {
			if (gNextSubmap == 0x15) {
				if (bossFlags >= 0x5F) {
					if (gElementReset >= 1) {
						if (gGameState == 0){
							gGameState = 8;
							gElementReset = gElementReset - 1;
						}
					}
				}
			}
		}
		
		
		
		if (gCurrentMap == 0x10) {
			if (gNextSubmap == 0x15) {
				if (bossFlags >= 0x5F) {
					if (sBrianStone >= 10000){
						eMariontext = 0xe800;
						if (gElementReset == 0) {
							if (gGameState == 2){
								if (eTextlookup == 0xA0c0800E) {
									if (cbuttonpressed == 0x01){
										gElementReset = sFireLvl - 1;
										sFireLvl = 1;
										sBrianStone = sBrianStone - 10000;
										eTextopen = 0x00000000;
										eTextID = 0x00000000;
									}
									if (cbuttonpressed == 0x04){
										gElementReset = sEarthLvl - 1;
										sEarthLvl = 1;
										sBrianStone = sBrianStone - 10000;
										eTextopen = 0x00000000;
										eTextID = 0x00000000;
									}
									if (cbuttonpressed == 0x02){
										gElementReset = sWaterLvl - 1;
										sWaterLvl = 1;
										sBrianStone = sBrianStone - 10000;
										eTextopen = 0x00000000;
										eTextID = 0x00000000;
									}
									if (cbuttonpressed == 0x08){
										gElementReset = sWindLvl - 1;
										sWindLvl = 1;
										sBrianStone = sBrianStone - 10000;
										eTextopen = 0x00000000;
										eTextID = 0x00000000;
									}
								}
							}
						}
					}
				}
			}
		}

	
	
InventoryStackingTick();

void InventoryStackingTick(void) {
	    if (*(unsigned short*)GAME_STATE_ADDR == GAME_STATE_TITLE) {
        uint8_t* qty = Section2Base();
        for (int i = 0; i < MAX_VALID_ITEM_ID; i++) {
            qty[i] = 0;
        }
        return; /* nothing else to do this frame */
		}
    uint8_t* list = Section1Base();
    uint8_t* qty  = Section2Base();

    
uint8_t seen[INVENTORY_SECTION2_SIZE];
for (int i = 0; i < INVENTORY_SECTION2_SIZE; i++) {
    seen[i] = 0;
}

    int liveCount = 0;
    uint8_t* base = Section1Base();

    /* Detect + undo native block-shift drift. If slot 74 isn't the
       terminator, an item-use event dragged the whole 150-byte block
       (including Section 2) left by 1. Shift the affected range back
       right by 1 to restore alignment, then re-plant the terminator
       so leaked quantity data never reaches live Section 1 entries. */
if (base[74] != ITEM_LIST_END) {
    for (int i = 74 + MAX_VALID_ITEM_ID; i > 74; i--) {
        base[i] = base[i - 1];
    }
    base[74] = ITEM_LIST_END;
}


    /* ...rest of existing function (Pass 1, Pass 2, Pass 3) unchanged below... */
    /* ---- Pass 1: scan, detect + resolve duplicates ---- */
    for (int i = 0; i < INVENTORY_SECTION1_SIZE; i++) {
        uint8_t id = list[i];

        if (id == ITEM_LIST_END) {
            break; /* end of live list */
        }

        if (seen[id]) {
            /* Duplicate found — this happens after a stacking pickup,
               since native insert logic drops the new copy at the end
               of the list. Merge it: delete this entry, bump quantity. */
            uint8_t currentExtra = qty[id] & QTY_VALUE_MASK;
            uint8_t cap = ItemStackCap[id];

            if (currentExtra < cap) {
                qty[id] = QTY_FLAG_BIT | (currentExtra + 1);
            }
            /* If already at cap, the "extra" pickup is simply discarded
               by virtue of being deleted here without incrementing. */

            /* Delete this slot by shifting everything after it left by 1. */
            for (int j = i; j < INVENTORY_SECTION1_SIZE - 1; j++) {
                list[j] = list[j + 1];
            }
            list[INVENTORY_SECTION1_SIZE - 1] = ITEM_LIST_END;

            i--; /* re-check this index since a new value just shifted into it */
            continue;
        }

        seen[id] = 1;
        liveCount++;

        /* First time this item type has been seen live — make sure its
           quantity byte at least has the "ever owned" flag set. */
        if (!(qty[id] & QTY_FLAG_BIT)) {
            qty[id] = QTY_FLAG_BIT;
        }
    }

    /* ---- Pass 2: restock items that were fully used ----
       For any item ID whose quantity byte shows extra copies remaining
       but which is NOT currently present in Section 1, native use-logic
       must have just consumed the live copy. Restock one and decrement. */
	   for (int id = 0; id < MAX_VALID_ITEM_ID; id++) {
    if (qty[id] == 0xFF) {
        qty[id] = 0; /* treat as invalid/uninitialized, not a real value */
        continue;
    }
    uint8_t extra = qty[id] & QTY_VALUE_MASK;
    /* ... rest of Pass 2 unchanged ... */
}
    for (int id = 0; id < INVENTORY_SECTION2_SIZE; id++) {
        uint8_t extra = qty[id] & QTY_VALUE_MASK;

        if (extra > 0 && !seen[id]) {
            /* Find first empty slot and restock. */
            for (int i = 0; i < INVENTORY_SECTION1_SIZE; i++) {
                if (list[i] == ITEM_LIST_END) {
                    list[i] = (uint8_t)id;
                    liveCount++;
                    qty[id] = QTY_FLAG_BIT | (extra - 1);
                    break;
                }
            }
        }
        /* If extra == 0, quantity byte stays at exactly QTY_FLAG_BIT —
           permanent "ever owned" flag, no live copies, nothing to do. */
    }

    /* ---- Pass 3: sort live entries by custom rank ---- */
    /* Simple insertion sort — liveCount is small (~31 max), no need
       for anything fancier. */
    for (int i = 1; i < liveCount; i++) {
        uint8_t key = list[i];
        uint8_t keyRank = ItemSortRank[key];
        int j = i - 1;
        while (j >= 0 && ItemSortRank[list[j]] > keyRank) {
            list[j + 1] = list[j];
            j--;
        }
        list[j + 1] = key;
    }
}

InventoryStackingTick();

/* ---- Dynamic Strength  ---- */
{
    u16 currentMaxHP   = *(volatile u16*)ADDR_CURRENT_MAX_HP;
	u8 fireLevel  = *(volatile u8*)ADDR_ELEMENTAL_FIRE;
	u8 earthLevel = *(volatile u8*)ADDR_ELEMENTAL_EARTH;
	u8 waterLevel = *(volatile u8*)ADDR_ELEMENTAL_WATER;
	u8 windLevel  = *(volatile u8*)ADDR_ELEMENTAL_WIND;
	u16 elementalTotal = (u16)fireLevel + earthLevel + waterLevel + windLevel;

    u16 hpGrowth   = (currentMaxHP   > STARTING_MAX_HP)    ? (currentMaxHP   - STARTING_MAX_HP)    : 0;
	u16 elemGrowth = (elementalTotal > STARTING_ELEMENTAL) ? (elementalTotal - STARTING_ELEMENTAL) : 0;

    u32 hpContrib   = ((u32)hpGrowth   * HP_WEIGHT_X100)   / (999 - STARTING_MAX_HP)    / 100;
    u32 elemContrib = ((u32)elemGrowth * ELEM_WEIGHT_X100) / (600 - STARTING_ELEMENTAL) / 100;

    u32 baseCalc = BASE_STRENGTH + hpContrib + elemContrib;
    if (baseCalc > 0xFF) baseCalc = 0xFF;
    u8 base = (u8)baseCalc;

    u32 up1Calc = ((u32)base * 3) / 2;
    if (up1Calc > 0xFF) up1Calc = 0xFF;
    u8 up1 = (u8)up1Calc;

    u32 up2Calc = (u32)base * 2;
    if (up2Calc > 0xFF) up2Calc = 0xFF;
    u8 up2 = (u8)up2Calc;

    *(volatile u8*)ADDR_LDA_BASE     = base;
    *(volatile u8*)ADDR_LDA_ATK_UP_1 = up1;
    *(volatile u8*)ADDR_LDA_ATK_UP_2 = up2;
    *(volatile u8*)ADDR_LDA_DISPEL   = base;
    *(volatile u8*)ADDR_LDA_WEAR_OFF = base;
}

	
//	void clean_inventory(void)
//{
//    u8 *ptr = &sInventory;
//
//    for (u8 step = 0; step < 150; step++, ptr++) {
//        u8 val = *ptr;
//
//        // Stop scanning early if we hit empty slot
//        if (val == 0xFF) {
//            break;
//        }
//
//        // Remove AP Item Unimportant, AP Item Important
//        if (val == 0x1A || val == 0x1B) {
//            *ptr = 0xFF;
//        }
//    }
//}	
	//	if (gGameState == 0x00000000){
	//	clean_inventory();}
	
		
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
