#ifndef _QUEST64_H_
#define _QUEST64_H_

#include "types.h"
#include "functions.h"
#include "enums.h"
#include "macros.h"

typedef struct SpellData {
/* 0x00 */ u16 levelRequirement;
/* 0x02 */ u16 ifAttackDisappearOnHit;
/* 0x04 */ u16 spellMenuInputs; //determines how unk_06 and unk_08 are read
/* 0x06 */ u16 firstAndSecondSpellInputDirections;
/* 0x08 */ u16 thirdAndFourthSpellInputDirections;
/* 0x0A */ u16 range;
/* 0x0C */ u16 damage_base;
/* 0x0E */ u16 accuracy;
/* 0x10 */ u16 type;
/* 0x12 */ u16 projectileType;
/* 0x14 */ u16 projectileTravelHeight;
/* 0x16 */ u16 autoHitBool;
/* 0x18 */ u16 target; 
/* 0x1A */ u16 unk_1A; //0x1A is padding probably
/* 0x1C */ f32 size;
/* 0x20 */ u16 attackCount; //0x20
/* 0x22 */ u16 frameDelayBetweenHits; //this value / 2 is actual frame delay
/* 0x24 */ u16 frameDelaySpellEffect; // this value / 2 is actual frame delay
/* 0x26 */ u16 startingPoint; //?
/* 0x28 */ u16 homingType;
/* 0x2A */ u16 angle; //?
/* 0x2C */ u16 attackDirection; //?
/* 0x2E */ u16 speed1; //?
/* 0x30 */ u16 speed2; //?
/* 0x32 */ u8 ifCompression; //0 if not used, 1 if so
/* 0x33 */ u8 ifVampsTouch; //0 if not used, 1 if so
/* 0x34 */ u8 ifPowerStaff; //0 not used, 1 is lv1, 2 is lvl 2
/* 0x35 */ u8 ifRestriction; //0 if not used, 1 if so
/* 0x36 */ u8 movementType; //0 not used, 1 slow enemy, 2 wind walk
/* 0x37 */ u8 agilityType; //0 not used, 1 evade lvl1, 2 evade lvl2, 3 slow enemy
/* 0x38 */ u8 ifSilence; //0 if not used, 1 if so
/* 0x39 */ u8 ifSoulSearch; //0 if not used, 1 if so
/* 0x3A */ u8 ifMagicBarrier; //0 if not used, 1 if so
/* 0x3B */ u8 ifConfusion; //0 if not used, 1 if so
/* 0x3C */ u8 defenseType; //0 not used, 1 is weak lvl2, 2 is weak lv1, 3 is armor lvl1, 4 is armor lvl2
/* 0x3D */ u8 unk_3D;
/* 0x3E */ u16 unk_3E;
/* 0x40 */ u16 unk_40;
/* 0x42 */ u16 unkInvalidity;
} SpellData;

typedef struct NpcData {
/* 0x00 */ u16 unk_00;
/* 0x02 */ u16 unk_02;
/* 0x04 */ u8 unk_04;
/* 0x05 */ u8 unk_05;
/* 0x06 */ u8 unk06; ///
/* 0x06 */ u8 unk07; ///
/* 0x08 */ u16 textOffset1;
/* 0x0A */ u16 textOffset2;
/* 0x0C */ s32 unkC; //unknown type (unused?)
/* 0x10 */ u8* modelData;
/* 0x14 */ u8* npcName;
/* 0x18 */ s32 unk18; //unknown type (unused?)
/* 0x1C */ Vec2f pos; //x,y position
/* 0x24 */ f32 rotation;
/* 0x28 */ f32 sizeScaler;
} NpcData; //sizeof 0x2C

typedef struct Unk_D_8007BD30 {
    char unk0[0x80];
    NpcData* npcData;
} Unk_D_8007BD30;

extern Unk_D_8007BD30 D_8007BD30;

extern u32 gToggleOn;
extern u32 gDamageNumbers;
extern u32 gDamageColor;
extern s32 gCurrentMap;
extern s32 gNextSubmap;
extern u8 gEncounterFloat;
extern u8 gEncounterStep;
extern u16 cbuttonpressed;
extern u16 gEncounterMax;
extern u8 gEncounterFlag;
extern u8 gAutoRevive;
extern u32 gSoftSteps;
extern u32 gSpellTimer;
extern u32 gSpellCast;
extern u32 sATKTimer;
extern u8 sATKStat;
extern u32 gGameButton;
extern s32 gGameState;
extern u32 gGameMode;
extern u8 gEventflag1;
extern u8 gEventflag2;
extern u8 gEventflag3;
extern u8 gEventflag4;
extern u8 gEventflag5;
extern u8 gEventflag6;
extern u8 gEventflag7;
extern u8 gEventflag8;
extern u8 gEventflag9;
extern u8 gEventflag10;
extern u8 gEventflag11;
extern u8 gEventflag12;
extern u8 gEventflag13;
extern u8 gEventflag14;
extern u8 gEventflag15;
extern u8 gEventflag16;
extern u8 gEventflag17;
extern u8 gEventflag18;
extern u8 gEventflag19;
extern u8 gEventflag20;
extern u8 gSpiritflagforest;
extern u8 gBossFlags;
extern u32 eTextlookup;
extern u32 eTextopen;
extern u32 eTextID;
extern u16 eKellytext;
extern u16 eFloratext;
extern u16 eKiliactext;
extern u16 eWilliamtext;
extern u16 eAbbotttext;
extern u16 eClairetext;
extern u16 eDonovantext;
extern u16 eZonnetext;
extern u16 eArkarttext;
extern u16 eHershtext;
extern u16 eClaytext;
extern u16 eBaggutext;
extern u16 eErrortext;
extern u16 eLeonardotext;
extern u16 eEponatext;
extern u16 eMaggietext;
extern u16 eGalitatext;
extern u16 eHectortext;
extern u16 eRoachtext;
extern u16 eBronzetext;
extern u32 eClairespirit;
extern u32 eClairespirit2;
extern u32 gTimeOfDay;
extern u16 eBGMTimer;
extern u16 eBGM;
extern u32 gBattleState2;
extern u16 gBattleState3;
extern u16 gBattleState4;
extern u32 bossonmap;
extern u16 gEventBossRead;
extern u16 gEventBossWrite;
extern u16 gEventBossRRead;
extern u16 gEventBossRWrite;
extern u16 sBrianMAXHP;
extern u16 sBrianHP;
extern u16 sBrianMAXMP;
extern u16 sBrianMP;
extern u16 sBrianHPEXP;
extern u16 sBrianMPEXP;
extern u16 sBrianDEFEXP;
extern u16 sBrianAGIEXP;
extern u32 sBrianELEEXP;
extern u32 sBrianStone;
extern u16 sHPEXPM;
extern u16 sSHPEXPM;
extern u16 sMPEXPM;
extern u16 sHMPEXPM;
extern u16 sHMPEXPM;
extern u16 sDEFEXPM;
extern u16 sAGIEXPM;
extern u16 sBAGIEXPM;
extern u8 sElementLvl;
extern u8 sFireLvl;
extern u8 sEarthLvl;
extern u8 sWaterLvl;
extern u8 sWindLvl;
extern u8 sBrianB;
extern u8 sBrianBA;
extern u8 sBrianBAT;
extern u32 sMaxEleLvl;
extern u32 sCombinedEle;
extern u16 sLv1MPCost;
extern u16 sLv2MPCost;
extern u16 sLv3MPCost;
extern u8 sBrianItemPickup;
extern u16 sSolvaMHP;
extern u16 sSolvaCHP;
extern u16 sSolvaDEF;
extern u16 sSolvaAGI;
extern u16 sSolvaATK;
extern u16 sSolvaELE;
extern u32 sSolvaEXP;
extern u32 sSolvaSTN;
extern u32 sSolvaDRP;
extern u16 sZelseMHP;
extern u16 sZelseCHP;
extern u16 sZelseDEF;
extern u16 sZelseAGI;
extern u16 sZelseATK;
extern u16 sZelseELE;
extern u16 sNeptyMHP;
extern u16 sNeptyCHP;
extern u16 sNeptyDEF;
extern u16 sNeptyAGI;
extern u16 sNeptyATK;
extern u16 sNeptyELE;
extern u16 sShilfMHP;
extern u16 sShilfCHP;
extern u16 sShilfDEF;
extern u16 sShilfAGI;
extern u16 sShilfATK;
extern u16 sShilfELE;
extern u16 sFargoMHP;
extern u16 sFargoCHP;
extern u16 sFargoDEF;
extern u16 sFargoAGI;
extern u16 sFargoATK;
extern u16 sFargoELE;
extern u16 sGuiltyMHP;
extern u16 sGuiltyCHP;
extern u16 sGuiltyDEF;
extern u16 sGuiltyAGI;
extern u16 sGuiltyATK;
extern u16 sGuiltyELE;
extern u16 sBeigisMHP;
extern u16 sBeigisCHP;
extern u16 sBeigisDEF;
extern u16 sBeigisAGI;
extern u16 sBeigisATK;
extern u16 sBeigisELE;
extern u16 sMammonMHP;
extern u16 sMammonCHP;
extern u16 sMammonDEF;
extern u16 sMammonAGI;
extern u16 sMammonATK;
extern u16 sMammonELE;
extern u8 sCurrentenemy;
extern u16 sEnemyHP;
extern u16 sEnemyATK;
extern u16 sEnemyDEF;
extern u16 sEnemyAGI;
extern u16 sEnemyPOW;
extern u16 sEnemyHP2;
extern u16 sEnemyATK2;
extern u16 sEnemyDEF2;
extern u16 sEnemyAGI2;
extern u16 sEnemyHP3;
extern u16 sEnemyATK3;
extern u16 sEnemyDEF3;
extern u16 sEnemyAGI3;
extern u16 sEnemyHP4;
extern u16 sEnemyATK4;
extern u16 sEnemyDEF4;
extern u16 sEnemyAGI4;
extern u16 sEnemyHP5;
extern u16 sEnemyATK5;
extern u16 sEnemyDEF5;
extern u16 sEnemyAGI5;
extern u16 sEnemyHP6;
extern u16 sEnemyATK6;
extern u16 sEnemyDEF6;
extern u16 sEnemyAGI6;
extern u32 sEnemy1R;
extern u32 sEnemy2R;
extern u32 sEnemy3R;
extern u32 sEnemy4R;
extern u32 sEnemy5R;
extern u32 sEnemy6R;
extern u16 sAnimR1;
extern u16 sAnimR2;
extern u16 sAnimR3;
extern u16 sAnimR4;
extern u16 sAnimR5;
extern u16 sAnimR6;
extern u16 sAnimR7;
extern u16 sAnimR8;
extern u16 sAnimR9;
extern u16 sAnimR10;
extern u16 sAnimR11;
extern u16 sAnimR12;
extern u16 sAnimR13;
extern u16 sAnimR14;
extern u16 sAnimR15;
extern u16 sAnimR16;
extern u16 sAnimR17;
extern u16 sAnimR18;
extern u16 sAnimR19;
extern u16 sAnimR20;
extern u16 sAnimR21;
extern u32 sAnim2R1;
extern u32 sAnim2R2;
extern u32 sAnim2R3;
extern u32 sAnim2R4;
extern u32 sAnim2R5;
extern u32 sAnim2R6;
extern u32 sAnim2R7;
extern u32 sAnim2R8;
extern u32 sAnim2R9;
extern u32 sAnim2R10;
extern u32 sAnim2R11;
extern u32 sAnim2R12;
extern u32 sAnim2R13;
extern u32 sAnim2R14;
extern u32 sAnim2R15;
extern u32 sAnim2R16;
extern u32 sAnim2R17;
extern u32 sAnim2R18;
extern u32 sAnim2R19;
extern u32 sAnim2R20;
extern u32 sAnim2R21;
extern u8 sEnemy1RTime;
extern u8 sEnemy2RTime;
extern u8 sEnemy3RTime;
extern u8 sEnemy4RTime;
extern u8 sEnemy5RTime;
extern u8 sEnemy6RTime;
extern u32 sSolvaringLocation;
extern u32 sZelseLocation;
extern u32 sNeptyLocation;
extern u32 sShilfLocation;
extern u32 sFargoLocation;
extern u32 sGuiltyLocation;
extern u32 sBeigisLocation;
extern u32 sSolvaringFlag;
extern u32 sZelseFlag;
extern u32 sNeptyFlag;
extern u32 sShilfFlag;
extern u32 sFargoFlag;
extern u32 sGuiltyFlag;
extern u32 sBeigisFlag;
extern u32 gFireball;
extern u32 gWindcutter;
#endif
