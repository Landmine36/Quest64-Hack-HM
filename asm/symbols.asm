.definelabel dma_write, 0x80024260 //a0 rom, a1 ram, a2 size
.definelabel gCurrentBGM, 0x8008FCC1
.definelabel gPlayBGM, 0x8008FCC2
.definelabel gBGMDelay, 0x8008FCC4
.definelabel gCurrentMap, 0x80084EE4
.definelabel gNextSubmap, 0x80084EE8
.definelabel D_80053B00, 0x80053B00
.definelabel bossFlags, 0x8007D19C
.definelabel gTex_HUD_and_Menu, 0x803A6F40
.definelabel func_80020D4C, 0x80020D4C
.definelabel func_800210FC, 0x800210FC
.definelabel func_80020E2C, 0x80020E2C
.definelabel func_80020B4C, 0x80020B4C
.definelabel buttonsHeld, 0x80092874
.definelabel func_80020F8C, 0x80020F8C
.definelabel int_to_str_with_flags, 0x80024330
.definelabel gBattleState, 0x8008C592
.definelabel gAllowBattles, 0x80084F10
.definelabel D_8007BC10, 0x8007BC10
.definelabel D_8007BC14, 0x8007BC14
.definelabel D_8007BC18, 0x8007BC18
.definelabel D_8007BC1C, 0x8007BC1C
.definelabel D_8007BA5C, 0x8007BA5C
.definelabel expRequiredPerElementLevel, 0x80053D3C
.definelabel D_80053ECC, 0x80053ECC
.definelabel itemReceived, 0x8007BA70
.definelabel AddItemToInventory, 0x800212A0
.definelabel func_800120C0, 0x800120C0
.definelabel func_8002E768, 0x8002E768
.definelabel func_800268d4, 0x800268d4
.definelabel gPlayerData, 0x8007BA80
.definelabel ExpTable, 0x80053D3C
.definelabel D_8008FD0C, 0x8008FD0C
.definelabel D_80092871, 0x80092871
.definelabel D_80092876, 0x80092876
.definelabel D_8008FD04, 0x8008FD04
.definelabel D_8008FD08, 0x8008FD08
.definelabel func_80029B58, 0x80029B58
.definelabel func_8002AB64, 0x8002AB64
.definelabel func_8002A0B8, 0x8002A0B8
.definelabel FramesInMenu, 0x8008FD10
.definelabel curActorTurn, 0x8008C592
.definelabel rng_seed, 0x8004D748
.definelabel _sprintf, 0x80036384
.definelabel drawBoxBehindEnemyName, 0x800210FC
.definelabel D_80086A00, 0x80086A00
.definelabel D_80086A08, 0x80086A08
.definelabel D_80086AC8, 0x80086AC8
.definelabel func_8000EE60, 0x8000EE60
.definelabel func_80012700, 0x80012700

.definelabel AnimationXOffsets, 0x8005F0C0
.definelabel D_8005F8F0, 0x8005F8F0
.definelabel D_8005F8F4, 0x8005F8F4
.definelabel D_8005F914, 0x8005F914
.definelabel D_8005FA00, 0x8005FA00
.definelabel D_8008FD14, 0x8008FD14
.definelabel gBGMVolume, 0x8005F010
.definelabel D_80092872, 0x80092872
.definelabel gGameButton, 0x80092874
.definelabel gGameMode, 0x8007B2E0
.definelabel gGameState, 0x8007B2E4
.definelabel gMasterGfxPos, 0x8007B2FC
.definelabel D_8005FAA0, 0x8005FAA0
.definelabel func_8002A668, 0x8002a668
.definelabel D_8008FD10, 0x8008FD10
.definelabel func_80026F34, 0x80026f34
.definelabel func_80026A40, 0x80026a40
.definelabel func_80026D1C, 0x80026d1c
.definelabel func_8002b57c, 0x8002b57c
.definelabel D_8008FD20, 0x8008fd20
.definelabel D_8008FD0E, 0x8008fd0e
.definelabel func_800278B0, 0x800278b0
.definelabel func_80029448, 0x80029448
.definelabel func_80028624, 0x80028624
.definelabel D_8007BD30, 0x8007BD30
.definelabel D_8007C570, 0x8007C570
.definelabel D_8007C970, 0x8007C970
.definelabel osPiRawStartDma, 0x8003d910
.definelabel _memcpy, 0x8003fb20

//data
.definelabel gInventory, 0x8008cf78
.definelabel gToggleOn,0x800869f9
.definelabel gDamageNumbers, 0x800873e4
.definelabel gDamageColor, 0x8004cfa4
.definelabel gEncounterFloat, 0x8001c653
.definelabel gEncounterStep, 0x8001c68f
.definelabel gEncounterMax, 0x8001c68a
.definelabel gEncounterFlag, 0x800869f8
.definelabel gAutoRevive, 0x800869f9
.definelabel gSoftSteps, 0x800c2ea8
.definelabel gSpellTimer, 0x8007bbd8
.definelabel gSpellCast, 0x8007bbd4
.definelabel gEventflag1, 0x800869E4
.definelabel gEventflag2, 0x800869E5
.definelabel gEventflag3, 0x800869E6
.definelabel gEventflag4, 0x800869E7
.definelabel gEventflag5, 0x800869E8
.definelabel gEventflag6, 0x800869E9
.definelabel gEventflag7, 0x800869EA
.definelabel gEventflag8, 0x800869EB
.definelabel gEventflag9, 0x800869EC
.definelabel gEventflag10, 0x800869ED
.definelabel gEventflag11, 0x800869EE
.definelabel gEventflag12, 0x800869EF
.definelabel gEventflag13, 0x800869F0
.definelabel gEventflag14, 0x800869F1
.definelabel gEventflag15, 0x800869F2
.definelabel gEventflag16, 0x800869F3
.definelabel gEventflag17, 0x800869F4
.definelabel gEventflag18, 0x800869F5
.definelabel gEventflag19, 0x800869F6
.definelabel gEventflag20, 0x800869F7
.definelabel gElementReset, 0x800869FE
.definelabel gSpiritflagforest, 0x80086AF0
.definelabel gBossFlags, 0x8007D19C
.definelabel gametime, 0x800859E0
.definelabel gameday, 0x800859E2
.definelabel cbuttonpressed, 0x80092876
.definelabel eKellytext, 0x8013DA1A
.definelabel eFloratext, 0x801A3D5A
.definelabel eKiliactext, 0x801718AE
.definelabel eWilliamtext, 0x801A6CE6
.definelabel eAbbotttext, 0x801992B6
.definelabel eClairetext, 0x80183336
.definelabel eDonovantext, 0x8016ab2a
.definelabel eZonnetext, 0x8018ceba
.definelabel eClaytext, 0x801832b2
.definelabel eBaggutext, 0x8016433e
.definelabel eErrortext, 0x8016436a
.definelabel eArkarttext, 0x80183468
.definelabel eHershtext, 0x8018CDB0
.definelabel eLeonardotext, 0x8016E016
.definelabel eEponatext, 0x8016db4a
.definelabel eMaggietext, 0x80183412
.definelabel eGalitatext, 0x8016ab82
.definelabel eHectortext, 0x8016dfea
.definelabel eRoachtext, 0x8018343c
.definelabel eBronzetext, 0x8018cf3E
.definelabel eMariontext, 0x80183202
.definelabel eTextlookup, 0x8009067C
.definelabel eTextopen, 0x800905C0
.definelabel eTextID, 0x800905E0
.definelabel eClairespirit, 0x8040564C
.definelabel eClairespirit2, 0x80086A10
.definelabel eBGMTimer, 0x8008FCC2
.definelabel eBGM, 0x8008FCC0
.definelabel gTimeOfDay, 0x800859D0
.definelabel gBattleState2, 0x8008c590
.definelabel gBattleState3, 0x8008c593
.definelabel gBattleState4, 0x8008c592
.definelabel bossonmap, 0x8007d1a0
.definelabel gEventBossRead, 0x8000B9F6
.definelabel gEventBossWrite, 0x8000BA1E
.definelabel gEventBossRRead, 0xB000C5F6
.definelabel gEventBossRWrite, 0xB000C61E
.definelabel sBrianMAXHP, 0x8007BA86
.definelabel sBrianHP, 0x8007BA84
.definelabel sBrianMAXMP, 0x8007BA8A
.definelabel sBrianMP, 0x8007BA88
.definelabel sBrianHPEXP, 0x8007BAA8
.definelabel sBrianMPEXP, 0x8007BAAA
.definelabel sBrianDEFEXP, 0x8007BAAE
.definelabel sBrianAGIEXP, 0x8007BAAC
.definelabel sBrianELEEXP, 0x8007BA90
.definelabel sBrianStone, 0x8007Ba94
.definelabel sBrianSize, 0x8007BAF0
.definelabel sBrianComp, 0x8007BB4B
.definelabel sElementLvl, 0x8007BAB4
.definelabel sFireLvl, 0x8007BAA7
.definelabel sEarthLvl, 0x8007BAA6
.definelabel sWaterLvl, 0x8007BAA5
.definelabel sWindLvl, 0x8007BAA4
.definelabel sBrianB, 0x8007bb4b
.definelabel sBrianBA, 0x8007bb39
.definelabel sBrianBAT, 0x8007bb3a
.definelabel sCombinedEle, 0x8007BAA4
.definelabel sMaxEleLvl, 0x8007BAB4
.definelabel sHPEXPM, 0x80006E9E
.definelabel sSHPEXPM, 0x80004576
.definelabel sMPEXPM, 0x800165B6
.definelabel sHMPEXPM, 0x80016772
.definelabel sDEFEXPM, 0x80006EAE
.definelabel sBAGIEXPM, 0x80007472
.definelabel sAGIEXPM, 0x8041475A
.definelabel sLv1MPCost, 0x800c1b28
.definelabel sLv2MPCost, 0x800c1b2A
.definelabel sLv3MPCost, 0x800c1b2C
.definelabel sATKTimer, 0x8007bb3c
.definelabel sATKStat, 0x8007bbc3
.definelabel sBrianItemPickup, 0x8007BA73
.definelabel sSolvaMHP, 0x803A9F24
.definelabel sSolvaCHP, 0x803A9F26
.definelabel sSolvaDEF, 0x803A9F2E
.definelabel sSolvaAGI, 0x803A9F2C
.definelabel sSolvaATK, 0x803A9F4A
.definelabel sSolvaELE, 0x803A9F44
.definelabel sSolvaEXP, 0x803A9F30
.definelabel sSolvaSTN, 0x803A9F34
.definelabel sSolvaDRP, 0x803A9F54
.definelabel sZelseMHP, 0x803A9F5C
.definelabel sZelseCHP, 0x803A9F5E
.definelabel sZelseDEF, 0x803A9F66
.definelabel sZelseAGI, 0x803A9F64
.definelabel sZelseATK, 0x803A9F82
.definelabel sZelseELE, 0x803A9F7C
.definelabel sZelseEXP, 0x803A9F68
.definelabel sZelseSTN, 0x803A9F6C
.definelabel sZelseDRP, 0x803A9F8C
.definelabel sNeptyMHP, 0x803A9F94
.definelabel sNeptyCHP, 0x803A9F96
.definelabel sNeptyDEF, 0x803A9F9E
.definelabel sNeptyAGI, 0x803A9F9C
.definelabel sNeptyATK, 0x803A9FBa
.definelabel sNeptyELE, 0x803A9FB4
.definelabel sNeptyEXP, 0x803A9FA0
.definelabel sNeptySTN, 0x803A9FA4
.definelabel sNeptyDRP, 0x803A9FC4
.definelabel sShilfMHP, 0x803A9FCC
.definelabel sShilfCHP, 0x803A9FCE
.definelabel sShilfDEF, 0x803A9FD6
.definelabel sShilfAGI, 0x803A9FD4
.definelabel sShilfATK, 0x803A9FE2
.definelabel sShilfELE, 0x803A9FDC
.definelabel sShilfEXP, 0x803A9FD8
.definelabel sShilfSTN, 0x803A9FDC
.definelabel sShilfDRP, 0x803A9FEC
.definelabel sFargoMHP, 0x803AA004
.definelabel sFargoCHP, 0x803AA006
.definelabel sFargoDEF, 0x803AA00E
.definelabel sFargoAGI, 0x803AA00C
.definelabel sFargoATK, 0x803AA02a
.definelabel sFargoELE, 0x803AA024
.definelabel sGuiltyMHP, 0x803AA03C
.definelabel sGuiltyCHP, 0x803AA03E
.definelabel sGuiltyDEF, 0x803AA046
.definelabel sGuiltyAGI, 0x803AA044
.definelabel sGuiltyATK, 0x803AA062
.definelabel sGuiltyELE, 0x803AA05C
.definelabel sBeigisMHP, 0x803AA074
.definelabel sBeigisCHP, 0x803AA076
.definelabel sBeigisDEF, 0x803AA07E
.definelabel sBeigisAGI, 0x803AA07C
.definelabel sBeigisATK, 0x803AA09a
.definelabel sBeigisELE, 0x803AA094
.definelabel sMammonMHP, 0x803AA0AC
.definelabel sMammonCHP, 0x803AA0AE
.definelabel sMammonDEF, 0x803AA0B4
.definelabel sMammonAGI, 0x803AA0B6
.definelabel sMammonATK, 0x803AA0D2
.definelabel sMammonELE, 0x803AA0CC
.definelabel sCurrentenemy, 0x8007CA0D
.definelabel sEnemyHP, 0x8007C9A2
.definelabel sEnemyATK, 0x8007CAAC
.definelabel sEnemyDEF, 0x8007CAB0
.definelabel sEnemyAGI, 0x8007CAAE
.definelabel sEnemyPOW, 0x8007CAB2
.definelabel sEnemyHP2, 0x8007CACA
.definelabel sEnemyATK2, 0x8007Cbd4
.definelabel sEnemyDEF2, 0x8007Cbd8
.definelabel sEnemyAGI2, 0x8007Cbd6
.definelabel sEnemyHP3, 0x8007Cbf2
.definelabel sEnemyATK3, 0x8007CCFC
.definelabel sEnemyDEF3, 0x8007Ccd00
.definelabel sEnemyAGI3, 0x8007Ccfe
.definelabel sEnemyHP4, 0x8007Cd1a
.definelabel sEnemyATK4, 0x8007CE24
.definelabel sEnemyDEF4, 0x8007Ce28
.definelabel sEnemyAGI4, 0x8007Ce26
.definelabel sEnemyHP5, 0x8007Ce42
.definelabel sEnemyATK5, 0x8007CF4C
.definelabel sEnemyDEF5, 0x8007Cf50
.definelabel sEnemyAGI5, 0x8007Cf4e
.definelabel sEnemyHP6, 0x8007Cf6a
.definelabel sEnemyATK6, 0x8007D074
.definelabel sEnemyDEF6, 0x8007d078
.definelabel sEnemyAGI6, 0x8007d076
.definelabel sEnemy1R, 0x8007ca6c
.definelabel sEnemy2R, 0x8007cb94
.definelabel sEnemy3R, 0x8007ccbc
.definelabel sEnemy4R, 0x8007cde4
.definelabel sEnemy5R, 0x8007cf0c
.definelabel sEnemy6R, 0x8007d034
.definelabel sAnimR1, 0x800875e0
.definelabel sAnimR2, 0x80087638
.definelabel sAnimR3, 0x80087690
.definelabel sAnimR4, 0x800876e8
.definelabel sAnimR5, 0x80087740
.definelabel sAnimR6, 0x80087798
.definelabel sAnimR7, 0x800877f0
.definelabel sAnimR8, 0x80087848
.definelabel sAnimR9, 0x800878a0
.definelabel sAnimR10, 0x800878f8
.definelabel sAnimR11, 0x80087950
.definelabel sAnimR12, 0x800879a8
.definelabel sAnimR13, 0x80087a00
.definelabel sAnimR14, 0x80087a58
.definelabel sAnimR15, 0x80087ab0
.definelabel sAnimR16, 0x80087b08
.definelabel sAnimR17, 0x80087b60
.definelabel sAnimR18, 0x80087bb8
.definelabel sAnimR19, 0x80087c10
.definelabel sAnimR20, 0x80087c68
.definelabel sAnimR21, 0x80087cc0
.definelabel sAnim2R1, 0x800875e4
.definelabel sAnim2R2, 0x8008763c
.definelabel sAnim2R3, 0x80087694
.definelabel sAnim2R4, 0x800876ec
.definelabel sAnim2R5, 0x80087744
.definelabel sAnim2R6, 0x8008779c
.definelabel sAnim2R7, 0x800877f4
.definelabel sAnim2R8, 0x8008784c
.definelabel sAnim2R9, 0x800878a4
.definelabel sAnim2R10, 0x800878fc
.definelabel sAnim2R11, 0x80087954
.definelabel sAnim2R12, 0x800879ac
.definelabel sAnim2R13, 0x80087a04
.definelabel sAnim2R14, 0x80087a5c
.definelabel sAnim2R15, 0x80087ab4
.definelabel sAnim2R16, 0x80087b0c
.definelabel sAnim2R17, 0x80087b64
.definelabel sAnim2R18, 0x80087bbc
.definelabel sAnim2R19, 0x80087c14
.definelabel sAnim2R20, 0x80087c6c
.definelabel sAnim2R21, 0x80087cc4
.definelabel sEnemy1RTime, 0x8007ca2d
.definelabel sEnemy2RTime, 0x8007cb55
.definelabel sEnemy3RTime, 0x8007cc7d
.definelabel sEnemy4RTime, 0x8007cda5
.definelabel sEnemy5RTime, 0x8007cecd
.definelabel sEnemy6RTime, 0x8007cff5
.definelabel sSolvaringLocation, 0x803A9AD0
.definelabel sZelseLocation, 0x803a9ae8
.definelabel sNeptyLocation, 0x803a9b00
.definelabel sShilfLocation, 0x803a9b18
.definelabel sFargoLocation, 0x803a9b30
.definelabel sGuiltyLocation, 0x803a9b48
.definelabel sBeigisLocation, 0x803a9b60
.definelabel sSolvaringFlag, 0x803A9AD4
.definelabel sZelseFlag, 0x803a9aec
.definelabel sNeptyFlag, 0x803a9b04
.definelabel sShilfFlag, 0x803a9b1c
.definelabel sFargoFlag, 0x803a9b34
.definelabel sGuiltyFlag, 0x803a9b4c
.definelabel sBeigisFlag, 0x803a9b64
.definelabel gFireball,	0x804136AC
.definelabel gWindcutter, 0x80412aB8


.definelabel customText, 0x80400000