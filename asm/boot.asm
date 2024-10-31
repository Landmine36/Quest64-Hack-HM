.headersize (PAYLOAD_START_RAM - PAYLOAD_START_ROM)

//
ClearBSS:
LUI t0, 0x8007
LUI t1, 0x0002
ADDIU t0, t0, 0x2E90
ORI t1, t1, 0x39C0
bss_clear_loop:
ADDI t1, t1, 0xFFF8
SD r0, 0x0000 (t0)
BNEZ t1, bss_clear_loop
ADDI t0, t0, 0x0008
JR RA
NOP

mainCFunctionWrapper:
    JAL mainCFunction
    NOP
    J 0x80024CE4
    ADDIU a0, sp, 0x1A0

customVar:
.word 0x00000000

customMemCpy: //requires 0x08 alignment
    BLEZ a2, exitMemCpy
    LD t0, 0x0000 (a1)
    SD t0, 0x0000 (a0)
    ADDIU a1, a1, 8
    ADDIU a0, a0, 8
    BEQ r0, r0, customMemCpy
    ADDIU a2, a2, -8
    exitMemCpy:
    JR RA
    NOP

set_gp:
    LUI gp, 0x8040
    JR RA
    ORI gp, gp, 0x0000


executeASM:
    //a0 holds instruction
    //a1 address to store register value to
    ADDIU sp, sp, -0x20
    SW ra, 0x0018 (sp)
    JAL getPC
    NOP
    getPC:
    SW a0, 0x0010 (ra)
    NOP //buffer just in case
    NOP //buffer just in case
    NOP //buffer just in case
    NOP //(custom instruction goes here)
    LW ra, 0x0018 (sp)
    JR RA
    ADDIU sp, sp, 0x20


copyRAM:
    JR RA
    NOP

osViRepeatLine:
    ADDIU          SP, SP, -0x18
    SW             S0, 0x0010 (SP)
    SW             RA, 0x0014 (SP)
    JAL            0x800E2BE0 //__osDisableInt
    ADDU           S0, A0, R0
    ANDI           S0, S0, 0x00FF
    BEQZ           S0, label1
    ADDU           A0, V0, R0
    LUI            V1, 0x8011
    LW             V1, 0x9E94 (V1)
    LHU            V0, 0x0000 (V1)
    J              label0
    ORI            V0, V0, 0x0040
    label1:
    LUI            V1, 0x8011
    LW             V1, 0x9E94(V1)
    LHU            V0, 0x0000 (V1)
    ANDI           V0, V0, 0xFFBF
    label0:
    JAL            0x800E2C00 //__osRestoreInt
    SH             V0, 0x0000 (V1)
    LW             RA, 0x0014 (SP)
    LW             S0, 0x0010 (SP)
    JR             RA
    ADDIU          SP, SP, 0x18

// osSetTime:
// LUI t0, 0x8025
// SW r0, 0xC260 (t0)
// JR RA
// SW r0, 0xC264 (t0)

crash_screen_sleep: //takes arg a0, ms to sleep
    ADDIU sp, sp, -0x20
    ORI v0, r0, 0xB71B
    MULT a0, v0
    DADDU a0, r0, r0
    DADDU a1, r0, r0
    SW ra, 0x0018 (sp)
    SW s1, 0x0014 (sp)
    SW s0, 0x0010 (sp)
    MFHI s0
    MFLO s1
    //JAL osSetTime
    NOP
    crashScreenLoop:
    //JAL osGetTime
    NOP
    DADDU a0, v0, r0
    DADDU a1, v1, r0
    SLTU v0, a0, s0
    BNEZ v0, crashScreenLoop
    NOP
    BNE s0, a0, exitCrashFunc
    SLTU v0, a1, s1
    BNEZ v0, crashScreenLoop
    NOP
    exitCrashFunc:
    LW ra, 0x0018 (sp)
    LW s1, 0x0014 (sp)
    LW s0, 0x0010 (sp)
    JR RA
    ADDIU sp, sp, 0x20

//so emulator wont close the game upon infinite loop
infiniteLoop:
    NOP
    J infiniteLoop
    NOP

__osDpDeviceBusy:
    LUI t6, 0xA410
    LW a0, 0x000C (t6)
    ADDIU sp, sp, -0x08
    ANDI t7, a0, 0x0100
    BEQZ t7, dpRet
    NOP
    B dpExit
    ADDIU v0, r0, 1
    dpRet:
    OR v0, r0, r0
    dpExit:
    JR RA
    ADDIU sp, sp, 0x08

asmCrashGame:
    LUI t0, 0x8000
    ORI t0, t0, 0x0001
    LW t1, 0x0000 (t0)
    JR RA
    NOP

getStatusRegister:
    MFC0 v0, $12
    NOP
    NOP
    JR RA
    NOP

setStatusRegister:
    MTC0 a0, $12
    NOP
    NOP
    JR RA
    NOP

bossSpellsDpadRight:
    LUI t0, 0x8009
    LBU t1, 0x2874 (t0)
    BEQZ t1, noDpadRight
    ORI t2, r0, 4

    LUI t3, 0x8007
    ORI t3, t3, 0xB2E7
    SB t2, 0x0000 (t3)

    LUI t3, 0x8007
    ORI t3, t3, 0xBBD1
    SB t2, 0x0000 (t3) //write to byte 8007BBD1
    SB t2, 0x0004 (t3) //write to byte 8007BBD5

    noDpadRight:
    JR RA
    NOP


newFireCap:
    LBU a0, 0x0024 (v0)
    LI at, gTotalBossesBeatenCount
    LW at, 0x0000 (at)
    LI t0, elementCapsTable
    SLL at, at, 2 //multiply by sizeof s32
    ADDU at, at, t0
    LW a1, 0x0000 (at)
    J 0x8001F700
    SLT at, a0, a1

newWaterCap:
    LBU a0, 0x0026 (v0)
    LI at, gTotalBossesBeatenCount
    LW at, 0x0000 (at)
    LI t0, elementCapsTable
    SLL at, at, 2 //multiply by sizeof s32
    ADDU at, at, t0
    LW a1, 0x0000 (at)
    J 0x8001F740
    SLT at, a0, a1

newWindCap:
    LBU a0, 0x0027 (v0)
    LI at, gTotalBossesBeatenCount
    LW at, 0x0000 (at)
    LI t0, elementCapsTable
    SLL at, at, 2 //multiply by sizeof s32
    ADDU at, at, t0
    LW a1, 0x0000 (at)
    J 0x8001F780
    SLT at, a0, a1

newEarthCap:
    LBU a0, 0x0025 (v0)
    LI at, gTotalBossesBeatenCount
    LW at, 0x0000 (at)
    LI t0, elementCapsTable
    SLL at, at, 2 //multiply by sizeof s32
    ADDU at, at, t0
    LW a1, 0x0000 (at)
    J 0x8001F7C8
    SLT at, a0, a1

newSpiritCap:
    LI at, gTotalBossesBeatenCount
    LW at, 0x0000 (at)
    LI t4, elementCapsTable
    SLL at, at, 2 //multiply by sizeof s32
    ADDU at, at, t4
    LW a0, 0x0000 (at)
    J 0x800032DC
    ORI t8, v1, 0x0008

elementLevelUpCap2:
    LBU a0, 0x0034 (v0)
    LI at, gTotalBossesBeatenCount
    LW at, 0x0000 (at)
    LI t4, elementCapsTable
    SLL at, at, 2 //multiply by sizeof s32
    ADDU at, at, t4
    LW at, 0x0000 (at)
    J 0x80007948
    SLT at, a0, at

elementGrowthLockCap:
    LBU t1, 0x0000 (v1)
    LI at, gTotalBossesBeatenCount
    LW at, 0x0000 (at)
    LI t4, elementCapsTable
    SLL at, at, 2 //multiply by sizeof s32
    ADDU at, at, t4
    J 0x80007964
    LW a0, 0x0000 (at)

fireElementLevelUpText:
    LBU	A3, 0xBAA4 (A3)
    LI a2, fireElementLevelUpTextXPos
    SLTI a1, a3, 10
    BNEZ a1, isLessThan10fire
    NOP
    SLTI a1, a3, 100 //if less than 0
    BNEZ a1, isLessThan100fire
    NOP
    //else, is 3 digits
    LI a0, 2
    BEQ r0, r0, shiftValueFire
    NOP

    isLessThan10fire:
    LI a0, 0 //shift amount
    BEQ r0, r0, shiftValueFire
    NOP

    isLessThan100fire:
    LI a0, 1
    BEQ r0, r0, shiftValueFire
    NOP

    shiftValueFire:
    SLL a0, a0, 2 //multiply by sizeof s32
    ADDU a2, a2, a0
    LW a1, 0x0000 (a2)
    ADDIU a0, r0, 0x0003
    J 0x8001F68C
    NOP


earthElementLevelUpText:
    LI a2, earthElementLevelUpTextXPos
    SLTI a1, a3, 10
    BNEZ a1, isLessThan10Earth
    NOP
    SLTI a1, a3, 100 //if less than 0
    BNEZ a1, isLessThan100Earth
    NOP
    //else, is 3 digits
    LI a0, 2
    BEQ r0, r0, shiftValueEarth
    NOP

    isLessThan10Earth:
    LI a0, 0 //shift amount
    BEQ r0, r0, shiftValueEarth
    NOP

    isLessThan100Earth:
    LI a0, 1
    BEQ r0, r0, shiftValueEarth
    NOP

    shiftValueEarth:
    SLL a0, a0, 2 //multiply by sizeof s32
    ADDU a2, a2, a0
    LW a1, 0x0000 (a2)
    ADDIU a0, r0, 0x0003
    J 0x8001F6A4
    NOP

waterElementLevelUpText:
    LI a2, waterElementLevelUpTextXPos
    SLTI a1, a3, 10
    BNEZ a1, isLessThan10water
    NOP
    SLTI a1, a3, 100 //if less than 0
    BNEZ a1, isLessThan100water
    NOP
    //else, is 3 digits
    LI a0, 2
    BEQ r0, r0, shiftValuewater
    NOP

    isLessThan10water:
    LI a0, 0 //shift amount
    BEQ r0, r0, shiftValuewater
    NOP

    isLessThan100water:
    LI a0, 1
    BEQ r0, r0, shiftValuewater
    NOP

    shiftValuewater:
    SLL a0, a0, 2 //multiply by sizeof s32
    ADDU a2, a2, a0
    LW a1, 0x0000 (a2)
    ADDIU a0, r0, 0x0003
    J 0x8001F6BC
    NOP


windElementLevelUpText:
    LI a2, windElementLevelUpTextXPos
    SLTI a1, a3, 10
    BNEZ a1, isLessThan10wind
    NOP
    SLTI a1, a3, 100 //if less than 0
    BNEZ a1, isLessThan100wind
    NOP
    //else, is 3 digits
    LI a0, 2
    BEQ r0, r0, shiftValuewind
    NOP

    isLessThan10wind:
    LI a0, 0 //shift amount
    BEQ r0, r0, shiftValuewind
    NOP

    isLessThan100wind:
    LI a0, 1
    BEQ r0, r0, shiftValuewind
    NOP

    shiftValuewind:
    SLL a0, a0, 2 //multiply by sizeof s32
    ADDU a2, a2, a0
    LW a1, 0x0000 (a2)
    ADDIU a0, r0, 0x0003
    J 0x8001F6D4
    NOP

elementAttackHook: //just push all cpu regs to the stack
OR a3, t9, r0
ADDU a2, t8, t6
ADDIU sp, sp, -0x90
sw $at, 0x0014($sp)
sw $v0, 0x0018($sp)
sw $v1, 0x001C($sp)
sw $a0, 0x0020($sp)
sw $a1, 0x0024($sp)
sw $a2, 0x0028($sp)
sw $a3, 0x002C($sp)
sw $t0, 0x0030($sp)
sw $t1, 0x0034($sp)
sw $t2, 0x0038($sp)
sw $t3, 0x003C($sp)
sw $t4, 0x0040($sp)
sw $t5, 0x0044($sp)
sw $t6, 0x0048($sp)
sw $t7, 0x004C($sp)
sw $s0, 0x0050($sp)
sw $s1, 0x0054($sp)
sw $s2, 0x0058($sp)
sw $s3, 0x005C($sp)
sw $s4, 0x0060($sp)
sw $s5, 0x0064($sp)
sw $s6, 0x0068($sp)
sw $s7, 0x006C($sp)
sw $t8, 0x0070($sp)
sw $t9, 0x0074($sp)
sw $ra, 0x0080($sp)

JAL ElementAttackHookC
NOP

BEQZ v0, exitElementAttackHook
lw $a2, 0x0028($sp)

//is brian's turn, use different table
//s0 currently holds spell attack as u16
SRL $t0, $s0, 8 //move spell upper byte to t0
ANDI $t1, $s0, 0xFF //get spell lower byte
LI $t2, SpellTablePointersBrian
SLL $t3, $t0, 2 //multiply first by sizeof pointer (4 bytes)
ADDU $t3, $t3, t2 //now points to pointer for spell data
LW $t4, 0x000 ($t3) //now load pointer to specific spell type
LI $t5, SpellDataSize
LW $t5, 0x0000 ($t5)
MULT t5, t1 //multiply second value by sizeof SpellData array
NOP
NOP
MFLO t6
NOP
NOP
ADDU a2, t6, t4 //now t7 points to correct spell data to read from

exitElementAttackHook:
lw $ra, 0x0080($sp)
lw $t9, 0x0074($sp)
lw $t8, 0x0070($sp)
lw $s7, 0x006C($sp)
lw $s6, 0x0068($sp)
lw $s5, 0x0064($sp)
lw $s4, 0x0060($sp)
lw $s3, 0x005C($sp)
lw $s2, 0x0058($sp)
lw $s1, 0x0054($sp)
lw $s0, 0x0050($sp)
lw $t7, 0x004C($sp)
lw $t6, 0x0048($sp)
lw $t5, 0x0044($sp)
lw $t4, 0x0040($sp)
lw $t3, 0x003C($sp)
lw $t2, 0x0038($sp)
lw $t1, 0x0034($sp)
lw $t0, 0x0030($sp)
lw $a3, 0x002C($sp)
//lw $a2, 0x0028($sp)
lw $a1, 0x0024($sp)
lw $v1, 0x001C($sp)
lw $v0, 0x0018($sp)
lw $at, 0x0014($sp)
ADDIU sp, sp, 0x90
J 0x80014B78
NOP

.align 4
itemRemovalHook:
    LBU a0, 0xCF78 (a0) //restore from hook
    JAL 0x800212E4 //restore from hook
    ADDU gp, a0, r0 //copy item removal to gp
    J 0x800220DC
    NOP

itemRemovalHook2:
    JAL MakeInventoryCopy
    NOP
    LI t5, itemIdChosen
    SW gp, 0x0000 (t5) //store chosen item id
    LUI t5, 0x8009 //restore from hook
    LW t5, 0xC750 (t5) //restore from hook
    LUI v0, 0x8009 //restore from hook
    J 0x800220EC
    NOP


	itemRemovalHook3:
    SW ra, 0x0028 (sp)

    LI t0, itemIdChosen
    LW t0, 0x0000 (t0)
    ADDIU t1, r0, 0xFFFF
    BEQ t0, t1, skipInventoryRestore
    NOP
    //give item back by restoring inventory
    JAL CheckNonRemovableItem
    NOP

    LW ra, 0x0028 (sp)
    JR RA
    ADDIU sp, sp, 0x148 //restore from hook

    skipInventoryRestore:
    JR RA
    ADDIU sp, sp, 0x148 //restore from hook

//80022244
//JR RA
//ADDIU sp, sp, 0x148

	
		checkIfNight:
    LI at, gTimeOfDay
    LW at, 0x0000 (at)
    LI v0, 0x005400
    SLTI v0, at, 0x5400
    BEQZL v0, isNight
    ORI v0, r0, 1
    SLTI v0, at, 0x1600
    BNEZL v0, isNight
    ORI v0, r0, 1
    //otherwise, is day
    ORI v0, r0, 0
    isNight:
    JR RA
    NOP


enemyHPSetHook:
    JAL checkIfNight
    NOP
    BEQZ v0, isNotNightHPSet
    NOP
     //set new pow by hpNightMultiplier
    LI t5, hpNightMultiplier
    LWC1 f6, 0x0000 (t5)
    MTC1 t4, f8
    CVT.S.W f8, f8
    MUL.S f8, f8, f6
    CVT.W.S f8, f8
    MFC1 t4, f8
    isNotNightHPSet:
    SH t4, 0x000A (a0)
    J 0x80009094
    LHU t5, 0x0006 (v1)
	
enemySetOtherStatsHook:
    LW t7, 0x0064 (s0)
    LHU t9, 0x000C (t7)
    LW t0, 0x0064 (s0)
    LHU t1, 0x000E (t0)
    SWC1 f4, 0x0120 (a0)
	
    JAL checkIfNight
    NOP
    BEQZ v0, isNotNightOtherStatsSet
    NOP

    //set new pow by defNightMultiplier
    LI t5, defNightMultiplier
    LWC1 f6, 0x0000 (t5)
    MTC1 t1, f8
    CVT.S.W f8, f8
    MUL.S f8, f8, f6
    CVT.W.S f8, f8
    MFC1 t1, f8


    //set new pow by powNightMultiplier
    LI t5, powNightMultiplier
    LWC1 f6, 0x0000 (t5)
    MTC1 t8, f8
    CVT.S.W f8, f8
    MUL.S f8, f8, f6
    CVT.W.S f8, f8
    MFC1 t8, f8


    //set new atk by atkNightMultiplier
    LI t5, atkNightMultiplier
    LWC1 f6, 0x0000 (t5)
    MTC1 t6, f8
    CVT.S.W f8, f8
    MUL.S f8, f8, f6
    CVT.W.S f8, f8
    MFC1 t6, f8

    //multiply agi by agiNightMultiplier at night
    LI t5, agiNightMultiplier
    LWC1 f6, 0x0000 (t5)
    MTC1 t9, f8
    CVT.S.W f8, f8
    MUL.S f8, f8, f6
    CVT.W.S f8, f8
    MFC1 t9, f8

    isNotNightOtherStatsSet:
    SH t1, 0x0118 (a0) //set new DEF
    SH t8, 0x011A (a0) //set new POW
    SH t6, 0x0114 (a0) //set new ATK
    SH t9, 0x0116 (a0) //set new AGI
    J 0x80009248
    NOP




//0x80022168

//lui a1, 0x8009
//lbu a1, 0xCF78 (a1)


//BEQZ t4, 0x80022168
//ADDU a3, t7, t9


textDrawingTest:
    J DrawingTestC
    NOP
