#include "../include/Quest64.h"
#include "../include/PR/gbi.h"

#define CURSOR_MOVE 0x200

#define MESSAGE_INDEX 0x0000
#define BGM_INDEX 0x0100

#define _SHIFTL(v, s, w)	\
    ((unsigned int) (((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
#define _SHIFTR(v, s, w)	\
    ((unsigned int)(((unsigned int)(v) >> (s)) & ((0x01 << (w)) - 1)))


typedef struct unkSliderStruct {
    s16 unk_00;
    s16 unk_02;
} unkSliderStruct;

typedef struct unksp38 {
    u8 unk_00[3];
} unksp38; //at a complete random guess, is this rgb?
void func_80029B58(s32, s32, s32, s32, s32);
extern s8 AnimationXOffsets[16];
extern unksp38 D_8005F8F0;
extern unkSliderStruct D_8005F8F4[];
extern unkSliderStruct D_8005F914[];
extern s8 D_8005FA00;
extern s32 D_8008FD04;
extern s32 D_8008FD08;
extern u16 D_8008FD0C;
extern s32 FramesInMenu;
extern s32 D_8008FD14;
extern s8 gBGMVolume;
extern s8 D_80092872;

//primary cursor is the cursor in the middle that doesn't move
//secondary cursor is the cursor that moves

//draw_environment_screen
void func_800278B0_Hook(s32 arg0) {
    s32 sp40;
    s32 sp3C;
    s32 var_a0;
    s32 var_a1;
    s32 var_t2;
    s32 temp_t1;
    s32 var_t1;

    unksp38 sp38;
    unkSliderStruct* temp_v1;
    
    
    sp38 = D_8005F8F0;
    D_8005FA00 = 4 - D_8005FA00;
    func_80029B58(0, arg0 + 0x28, 0x1E, 0xF0, 0x6C); //draw background
    func_80029B58(0x37, arg0 + 0x2B, 0x8B, 0xF0, 3);
    func_80029B58(0x37, arg0 + 0x119, 0x24, 2, 0x68);
    func_80029B58(0x37, arg0 + 0x27, 0x1D, 0xF2, 1);
    func_80029B58(0x37, arg0 + 0x27, 0x8A, 0xF2, 1);
    func_80029B58(0x37, arg0 + 0x27, 0x1E, 1, 0x6D);
    func_80029B58(0x37, arg0 + 0x118, 0x1E, 1, 0x6D);
    func_80029B58(0x38, arg0 + 0x29, 0x1F, 0xED, 1);
    func_80029B58(0x38, arg0 + 0x29, 0x20, 1, 0x68);
    func_80029B58(0x39, arg0 + 0x29, 0x88, 0xEE, 1);
    func_80029B58(0x39, arg0 + 0x116, 0x1F, 1, 0x69);
    func_80029B58(0x3C, arg0 + 0x3A, 0x1E, 0x15, 0x6C);
    func_80029B58(0x3D, arg0 + 0x5D, 0x56, 0xA7, 2);
    func_80029B58(0x24, arg0 + 0x8D, 0x3E, 0x54, 7);
    func_80029B58(0x24, arg0 + 0x8D, 0x6C, 0x54, 7);
    func_80029B58(0x29, arg0 + 0xEA, 0x3A, 0x18, 8);
    func_80029B58(0x2A, arg0 + 0x6C, 0x3D, 0x14, 0xB);
    func_80029B58(0x29, arg0 + 0xEA, 0x6B, 0x18, 8);
    func_80029B58(0x2A, arg0 + 0x6C, 0x68, 0x14, 0xB);
    func_80029B58(0x22, arg0 + 0x31, 0x3B, 0x34, 0xC);
    func_80029B58(0x23, arg0 + 0x35, 0x65, 0x2C, 0x10);
    func_80029B58(5, arg0 + 0x45, 0x1C, 7, 7);
    func_80029B58(5, arg0 + 0xF3, 0x1C, 7, 7);
    temp_t1 = AnimationXOffsets[(FramesInMenu / 4) % 16];
    func_80029B58(6, arg0 + temp_t1 + 0x36, 0x1A, 0xE, 0xB);\
    func_80029B58(7, (arg0 - temp_t1) + 0xFB, 0x1A, 0xE, 0xB);
    func_80029B58(0x21, arg0 + 0x77, 0x1A, 0x50, 0x10);
    func_80029B58(0x25, arg0 + 0x8E, 0x3F, 0x50, 3);
    func_80029B58(0x25, arg0 + 0x8E, 0x6D, 0x50, 3);
    func_80029B58(0x27, arg0 + 0xB3, 0x39, 8, 0xB);
    func_80029B58(0x27, arg0 + 0xB3, 0x67, 8, 0xB);
    if (D_8008FD0C & 0x200) {
        switch (D_8008FD0C & 0x100) {                          /* irregular */
        case MESSAGE_INDEX: //draw message secondary cursor while stationary
            temp_v1 = &D_8005F914[gBGMVolume];
            func_80029B58(sp38.unk_00[temp_v1->unk_02], temp_v1->unk_00 + arg0, 0x67, 8, 0xB);
            break;
        case BGM_INDEX: //draw bgm secondary cursor while stationary
            temp_v1 = &D_8005F8F4[D_8005FA00];
            func_80029B58(sp38.unk_00[temp_v1->unk_02], temp_v1->unk_00 + arg0, 0x39, 8, 0xB);
            break;
        }
    } else {
        //stationary secondary cursor
        temp_v1 = &D_8005F914[gBGMVolume];
        func_80029B58(sp38.unk_00[temp_v1->unk_02], temp_v1->unk_00 + arg0, 0x67, 8, 0xB);
        temp_v1 = &D_8005F8F4[D_8005FA00];
        func_80029B58(sp38.unk_00[temp_v1->unk_02], temp_v1->unk_00 + arg0, 0x39, 8, 0xB);
    }
    if (D_8008FD0C & 0x2000) {
        D_8005FA00 = 4 - D_8005FA00;
        return;
    }
    if (!(D_8008FD0C & 0x800)) {
        if ((D_80092871 >= 0x15) || (D_80092876 & 0x10)) {
            D_8008FD04 = 0x140;
            D_8008FD08 = 0;
            D_8008FD0C = D_8008FD0C | 0x2000;
            func_800268D4(0, 0, 0xFF);
        } else if ((D_80092871 < -0x14) || (D_80092876 & 0x2000)) {
            D_8008FD04 = 0;
            D_8008FD08 = -0x140;
            D_8008FD0C = D_8008FD0C | 0x3000;
            func_800268D4(0, 0, 0xFF);
        }
        
        if (D_80092876 & 0x8000) {
            D_8008FD0C |= 0x800;
            func_800268D4(0, 1, 0xFF);
        }
        goto block_94;
    }
    var_a0 = D_8008FD0C & 0x200;
    if (D_80092876 & 0x8000) {
        if (var_a0 == 0) {
            D_8008FD0C = D_8008FD0C & ~0x100;
            D_8008FD0C &= ~0x800;
            func_800268D4(0, 2, 0xFF);
            D_8005FA00 = 4 - D_8005FA00;
            return;
        }
    }
    sp40 = D_8008FD0C & 0x100;
    var_t1 = AnimationXOffsets[(FramesInMenu / 2) % 16];
    if (var_a0 != 0) {
        switch (sp40) {                          /* switch 1; irregular */
        case 0x0:                                   /* switch 1 */
            temp_v1 = &D_8005F8F4[D_8005FA00];
            if (D_8008FD14 >= temp_v1->unk_00) {
                if (temp_v1->unk_00 >= D_8008FD14) {
                    D_8008FD0C &= ~0x200;
                    D_8008FD0C &= ~0x400;
                    sp40 = sp38.unk_00[temp_v1->unk_02];
                    if ((D_80092871 < 0x14) && (D_80092871 >= -0x13)) {
                        func_80029B58(sp40, D_8008FD14 + arg0, 0x39, 8, 0xB);
                    }
                    D_8008FD0C = D_8008FD0C;
                    sp3C = 0x3A;
                    sp40 = 0;
                }
            }
            break;
        case 0x100:
            temp_v1 = &D_8005F914[gBGMVolume];
            if (D_8008FD14 >= temp_v1->unk_00) {
                if (temp_v1->unk_00 >= D_8008FD14) {
                    D_8008FD0C = D_8008FD0C & ~0x200;
                    D_8008FD0C &= ~0x400;
                    sp40 = sp38.unk_00[temp_v1->unk_02];
                    if ((D_80092871 < 0x14) && (D_80092871 >= -0x13)) {
                        func_80029B58(sp40, D_8008FD14 + arg0, 0x67, 8, 0xB);
                    }
                    D_8008FD0C = D_8008FD0C;
                    sp3C = 0x65;
                    sp40 = 0x100;
                }
            }
            break;
        }
        var_a0 = D_8008FD0C & 0x200;
    }
    var_t2 = D_8008FD14;
    if (var_a0 == 0) {
        switch (sp40) {                             /* switch 2; irregular */
        case 0x0:                                   /* switch 2 */
            sp3C = 0x3A;
            if ((D_80092872 < -0x1E) || (D_80092872 >= 0x1F)) {
                if (!(D_8008FD0C & 0x400)) {
                    D_8008FD0C = D_8008FD0C & ~0x100;
                    D_8008FD0C = D_8008FD0C | 0x100;
                    D_8008FD0C = D_8008FD0C | 0x400;
                    func_800268D4(0, 0, 0xFF);
                    D_8008FD0C = D_8008FD0C;
                    var_t2 = D_8008FD14;
                    var_a0 = D_8008FD0C & 0x200;
                }
            } else if (D_80092871 < -0x14) {
                if (!(D_8008FD0C & 0x400)) {
                    var_t2 = D_8005F8F4[D_8005FA00--].unk_00;
                    if (D_8005FA00 < 0) {
                        D_8005FA00 = 0;
                    }
                    D_8008FD0C = D_8008FD0C | 0x600;
                    //TODO: missing cod here
                    var_a0 = D_8008FD0C & 0x200;
                }
            } else if (D_80092871 >= 0x15) {
                if (!(D_8008FD0C & 0x400)) {
                    D_8008FD0C = D_8008FD0C | 0x600;
                    var_t2 = D_8005F8F4[D_8005FA00++].unk_00;
                    D_8008FD0C = D_8008FD0C;
                    var_a0 = D_8008FD0C & 0x200;
                    if (D_8005FA00 >= 5) {
                        D_8005FA00 = 4;
                    }
                    D_8008FD0C = D_8008FD0C;
                }
            } else {
                D_8008FD0C = D_8008FD0C & ~0x400;
                //TODO: missing cod here
                var_a0 = D_8008FD0C & 0x200;
            }
            break;
        case 0x100:                                 /* switch 2 */
            sp3C = 0x65;
            if ((D_80092872 < -0x1E) || (D_80092872 >= 0x1F)) {
                if (!(D_8008FD0C & 0x400)) {
                    D_8008FD0C &= ~0x100;
                    D_8008FD0C = D_8008FD0C | 0x400;
                    func_800268D4(0, 0, 0xFF);
                    D_8008FD0C = D_8008FD0C;
                    var_t2 = D_8008FD14;
                    var_a0 = D_8008FD0C & 0x200;
                }
            } else if (D_80092871 < -0x14) {
                if (!(D_8008FD0C & 0x400)) {
                    var_t2 = D_8005F914[gBGMVolume--].unk_00;
                    if (gBGMVolume < 0) {
                        gBGMVolume = 0;
                    }
                    D_8008FD0C = (D_8008FD0C | 0x400);
                    D_8008FD0C = D_8008FD0C | 0x200;
                    //TODO missing cod
                    var_a0 = D_8008FD0C & 0x200;
                }
            } else if (D_80092871 >= 0x15) {
                if (!(D_8008FD0C & 0x400)) {
                    D_8008FD0C = D_8008FD0C | 0x400;
                    var_t2 = D_8005F914[gBGMVolume++].unk_00;
                    D_8008FD0C = D_8008FD0C | 0x200;
                    var_a0 = D_8008FD0C & 0x200;
                    if (gBGMVolume >= 5) {
                        gBGMVolume = 4;
                    }
                    D_8008FD0C = D_8008FD0C;
                }
            } else {
                D_8008FD0C = D_8008FD0C & ~0x400;
                D_8008FD0C = D_8008FD0C & 0xFFFF;
                var_a0 = D_8008FD0C & 0x200;
                D_8008FD0C = D_8008FD0C;
            }
            break;
        }
    }
    D_8008FD14 = var_t2;
    if (var_a0 != 0) {
        if (sp40 != 0) {
            if (sp40 != 0x100) {
                D_8008FD14 = var_t2;
            } else {
                temp_v1 = &D_8005F914[gBGMVolume];
                sp3C = 0x65;
                var_a1 = var_t2 + arg0;
                if (var_t2 < temp_v1->unk_00) {
                    var_a1 += 1;
                    D_8008FD14 = var_t2 + 1;
                    sp40 = sp38.unk_00[temp_v1->unk_02];
                } else if (temp_v1->unk_00 < var_t2) {
                    var_a1 -= 1;
                    D_8008FD14 = var_t2 - 1;
                    sp40 = sp38.unk_00[temp_v1->unk_02];
                } else {
                    D_8008FD14 = var_t2;
                    D_8008FD0C = D_8008FD0C & ~0x0400;
                    sp40 = sp38.unk_00[temp_v1->unk_02];
                }
                func_80029B58(sp40, var_a1, 0x67, 8, 0xB);
            }
        } else {
            temp_v1 = &D_8005F8F4[D_8005FA00];
            sp3C = 0x3A;
            var_a1 = var_t2 + arg0;
            if (var_t2 < temp_v1->unk_00) {
                var_a1 += 1;
                D_8008FD14 = var_t2 + 1;
                sp40 = sp38.unk_00[temp_v1->unk_02];
            } else if (temp_v1->unk_00 < var_t2) {
                var_a1 -= 1;
                D_8008FD14 = var_t2 - 1;
                sp40 = sp38.unk_00[temp_v1->unk_02];
            } else {
                D_8008FD14 = var_t2;
                D_8008FD0C = D_8008FD0C & ~0x0400;
                sp40 = sp38.unk_00[temp_v1->unk_02];
            }
            func_80029B58(sp40, var_a1, 0x39, 8, 0xB);
        }
    }
    func_80029B58(1, (arg0 - var_t1) + 0x1E, sp3C, 0x10, 0xD);
block_94:
    D_8005FA00 = 4 - D_8005FA00;
}


void func_800268D4(s32, s32, s32);                      /* extern */
void func_80026A40(void);                                  /* extern */
void func_80026D1C(void);                                  /* extern */
void func_80026F34(s32);                                 /* extern */
void func_800278B0(s32);                                 /* extern */
void func_80028624(s32);                                 /* extern */
void func_80029448(s32);                                 /* extern */
void func_8002A668(Gfx**);
void func_8002B57C(void);
extern Gfx D_8005FAA0[];
extern u16 D_8008FD0C;
extern s8 D_8008FD0E;
extern s32 D_8008FD10;
extern u16 D_8008FD20;
extern Gfx* gMasterGfxPos;
//extern s16 gGameMode;

void func_80026A7C_Hook(s32 arg0) { //draw_pause_screen_main
    gSPDisplayList(gMasterGfxPos++, D_8005FAA0);
    gDPSetTextureLUT(gMasterGfxPos++, G_TT_RGBA16);
    func_8002A668(&gMasterGfxPos);
    D_8008FD10 += 1;
    if (D_8008FD0C & 0x4000) {
        func_8002B57C();
    }
    else if (D_8008FD0C & 0x2000) {
        func_80026D1C();
    } else {
        switch (D_8008FD0C & 7) {                              /* irregular */
        case 0:
            func_80026F34(0);
            break;
        case 1:
            func_800278B0_Hook(0);
            break;
        case 2:
            func_80028624(0);
            break;
        case 3:
            func_80029448_Hook(0);
            break;
        //added case, not accessible currently
        case 4:
            func_80029448_Hook(0);
            break;
        }    
    }

    D_8008FD0C = D_8008FD0C & ~0x0010;
    gDPSetTextureLUT(gMasterGfxPos++, G_TT_NONE);
    gDPSetBlendColor(gMasterGfxPos++, 0, 0, 0, 1);
    if (((D_80092876 & 0x4000) || (D_80092876 & 0x1000)) && !(D_8008FD0C & 0x4000)) {
        D_8008FD0C &= ~0x8000;
        func_800268D4(0, 1, 0xFF);
    }
    if (!(D_8008FD0C & 0x8000)) {
        if (D_8008FD0C & 0x4000) {
            switch (D_8008FD0C & 0xC) {                      /* switch 1; irregular */
            case 0:                                 /* switch 1 */
                if (D_8008FD20 & 0x80) {
                    gGameMode = 1;
                    D_8008FD0E = 1;
                } else {
                    gGameMode = 3;
                    D_8008FD0E = 0;
                }
                break;
            case 8:                                 /* switch 1 */
                gGameMode = 3;
                break;
            case 4:                                 /* switch 1 */
                gGameMode = 1;
            }
        } else {
            gGameMode = 1;
        }
        func_80026A40();
    }
}