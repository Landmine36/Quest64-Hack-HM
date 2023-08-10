//Payload related information
.definelabel PAYLOAD_START_ROM, 0x1000000
.definelabel PAYLOAD_START_RAM, 0x80400000
.definelabel PAYLOAD_SIZE, 0x3F0000

PAYLOAD_START:
.headersize 0x7FFFF400 //ran once on boot
.org 0x80024C1C
LUI a0, hi(PAYLOAD_START_ROM)
ADDIU a0, a0, lo(PAYLOAD_START_ROM)
LUI a1, hi(PAYLOAD_START_RAM)
ADDIU a1, a1, lo(PAYLOAD_START_RAM)
LUI a2, hi(PAYLOAD_SIZE)
JAL dma_write
ADDIU a2, a2, lo(PAYLOAD_SIZE)
J originalCode
NOP

.org 0x8001F684 //fire element level up text
J fireElementLevelUpText
NOP

.org 0x8001F69C //earth element level up text
J earthElementLevelUpText
NOP

.org 0x8001F6B4 //water element level up text
J waterElementLevelUpText
NOP

.org 0x8001F6CC //wind element level up text
J windElementLevelUpText
NOP


.org 0x80012244
LI v0, NewSpiritTable
LI a0, 0x80084EE8

.org 0x8002684C
J func_8002684C_Hook
NOP

.org 0x80012220
J func_80012220_Hook
NOP

.org 0x8001EBDC
J func_8001EBDC_Hook
NOP

.org 0x80020D4C
J func_80020D4C_Hook
NOP

//ROM 0x3ED4
.org 0x800032D4 //cant pick up spirit if all stats are 99
J newSpiritCap
NOP

//ROM 0x42B0
.org 0x800036B0 //item drop lock
B 0x800036C0

//ROM 0x4CCC
.org 0x800040CC //change death jingle sound
ADDIU a0, r0, 0x001D

//ROM 0x6018 
.org 0x80005418 //movement speed patch 2.0 -> 2.75
LUI at, 0x4030

//ROM 0x8220
.org 0x80007620 //Gain HP Exp until 999
SLTI at, t7, 0x03E7

//ROM 0x8264
.org 0x80007664 //??
ADDIU t5, r0, 0x03E7

//ROM 0x8274
.org 0x80007674 //Gain HP Levels until 999
SLTI at, t4, 0x03E7

//ROM 0x82B0
.org 0x800076B0 //HP Scaling Until
SLTI at, a3, 0x0026

//ROM 0x82F0
.org 0x800076F0 //Gain MP Levels Until
SLTI at, t6, 0x03E7

//ROM 0x832C
.org 0x8000772C //?
ADDIU t5, r0, 0x03E7

//ROM 0x833C
.org 0x8000773C //Gain MP Levels Until
SLTI at, t4, 0x03E8

//ROM 0x8378
.org 0x80007778 //MP Scaling Until
SLTI at, a3, 0x0012

//ROM 0x83AC
.org 0x800077AC //Gain AGI Exp Until
SLTI at, t5, 0x03E7

//ROM 0x83F4
.org 0x800077F4 //Gain AGI Levels Until
SLTI at, t2, 0x03E8

//ROM 0x841C
.org 0x8000781C //AGI Scaling Until
SLTI at, a2, 0x0018

//ROM 0x8450
.org 0x80007850 //Gain DEF Exp Until
SLTI at, t9, 0x03E7

//ROM 0x849C
.org 0x8000789C //Gain DEF Levels Until
SLTI at, t7, 0x03E8

//ROM 0x84C4
.org 0x800078C4 //DEF Scaling Until
SLTI at, a2, 0x0001

//ROM 0x8508
.org 0x80007908 //Element Cap 1
SLTI at, a0, 0x018C

//ROM 0x8540
.org 0x80007940 //Element Cap 2
J elementLevelUpCap2
NOP

//ROM 0x855C
.org 0x8000795C //Element Growth Lock
J elementGrowthLockCap
NOP

//ROM 0xC8D8
.org 0x8000BCD8 //player Max HP >= 1000 set to 1000
SLTI at, v0, 0x03E8

//ROM 0xC8E4
.org 0x8000BCD8 //set player max hp to 999
ADDIU t8, r0, 0x03E7

//ROM 0x1F0AC
.org 0x8001E4AC //? HUD related
ADDIU t6, t5, 0x0000

//ROM 0x202F8
.org 0x8001F6F8 //Fire cap
J newFireCap
NOP

//ROM 0x20338
.org 0x8001F738 //water cap
J newWaterCap
NOP

//ROM 0x20378
.org 0x8001F778 //wind cap
J newWindCap
NOP

//ROM 0x203C0
.org 0x8001F7C0 //earth cap
J newEarthCap
NOP

.org 0x8001EC68
ADDIU a1, r0, 0x001C //move earth element icon

.org 0x8001ECA4
ADDIU a1, r0, 0x0038 //move water element icon

.org 0x8001ECE0
ADDIU a1, r0, 0x0054 //move wind element icon

.org 0x8001ED3C
ADDIU s0, s0, 0x001C //set element text spacing

.org 0x80024FC0
J mainCFunctionWrapper
ADDIU a0, sp, 0x1A0 //restore from hook

.org 0x80024FF8
J mainCFunctionWrapper
SH t6, 0x005C (sp) //restore from hook

.headersize 0x7F374C40
.org 0x800C0680
//.word 0x00000000 //(dont actually patch this, just an example)