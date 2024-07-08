#include <task.h>

INLINE_ASM(_MOV_Init, 0x74, "asm/nonmatching/./move/_MOV_Init.src");

// MERGE_LIST([['_syCalloc', '_lbl_8C033C80'], ["h'40400000", '_lbl_8C033C84'], ["h'400CCCCD", '_lbl_8C033C88'], ["h'C0400000", '_lbl_8C033C8C'], ["h'42200000", '_lbl_8C033C90'], ["h'3F4CCCCD", '_lbl_8C033C94'], ["h'3F666666", '_lbl_8C033C98'], ["h'3E4CCCCD", '_lbl_8C033C9C']]);
INLINE_ASM(_MOV_Control, 0x110, "asm/nonmatching/./move/_MOV_Control.src");

// MERGE_LIST([["h'3C23D70A", '_lbl_8C033CA8'], ["h'0000FFEF", '_lbl_8C033CAC'], ['_njScalor', '_lbl_8C033CA4'], ['_CCL_IsPushed', '_lbl_8C033CA0']]);
INLINE_ASM(_MOV_SetVelo, 0x10, "asm/nonmatching/./move/_MOV_SetVelo.src");

INLINE_ASM(_MOV_ClearVelo, 0x6c, "asm/nonmatching/./move/_MOV_ClearVelo.src");

INLINE_ASM(_func_8C033CC0, 0x10, "asm/nonmatching/./move/_func_8C033CC0.src");

INLINE_ASM(_MOV_ClearAcc, 0x12, "asm/nonmatching/./move/_MOV_ClearAcc.src");

INLINE_ASM(_MOV_SetGravity, 0x8, "asm/nonmatching/./move/_MOV_SetGravity.src");

INLINE_ASM(_sub_8C033CEA, 0x6, "asm/nonmatching/./move/_sub_8C033CEA.src");

INLINE_ASM(_func_8C033CF0, 0x16, "asm/nonmatching/./move/_func_8C033CF0.src");

INLINE_ASM(_MOV_PreservePreviousPosition, 0x16, "asm/nonmatching/./move/_MOV_PreservePreviousPosition.src");

INLINE_ASM(_func_8C033D1C, 0x16, "asm/nonmatching/./move/_func_8C033D1C.src");

INLINE_ASM(_func_8C033D32, 0x16, "asm/nonmatching/./move/_func_8C033D32.src");

INLINE_ASM(_func_8C033D48, 0x6, "asm/nonmatching/./move/_func_8C033D48.src");

INLINE_ASM(_MOV_SetAimPos, 0x12, "asm/nonmatching/./move/_MOV_SetAimPos.src");

INLINE_ASM(_MOV_DistFromAim, 0x60, "asm/nonmatching/./move/_MOV_DistFromAim.src");

INLINE_ASM(_func_8C033DC0, 0x2e, "asm/nonmatching/./move/_func_8C033DC0.src");

INLINE_ASM(_func_8C033DEE, 0x38, "asm/nonmatching/./move/_func_8C033DEE.src");

// MERGE_LIST([['_atan2f', '_lbl_8C033F40'], ["h'4622F986", '_lbl_8C033F44']]);
INLINE_ASM(_func_8C033E26, 0x28, "asm/nonmatching/./move/_func_8C033E26.src");

// MERGE_LIST([['_AdjustAngle', '_lbl_8C033F48'], ['_DiffAngle', '_lbl_8C033F4C']]);
INLINE_ASM(_func_8C033E4E, 0x2a, "asm/nonmatching/./move/_func_8C033E4E.src");

// MERGE_LIST([['_AdjustAngle', '_lbl_8C033F48'], ['_DiffAngle', '_lbl_8C033F4C']]);
INLINE_ASM(_MOV_TurnToAim2, 0x2e, "asm/nonmatching/./move/_MOV_TurnToAim2.src");

// MERGE_LIST([['_AdjustAngle', '_lbl_8C033F48'], ['_DiffAngle', '_lbl_8C033F4C']]);
INLINE_ASM(_func_8C033EA6, 0x30, "asm/nonmatching/./move/_func_8C033EA6.src");

// MERGE_LIST([['_njSqrt', '_lbl_8C033F50']]);
INLINE_ASM(_func_8C033ED6, 0x2e, "asm/nonmatching/./move/_func_8C033ED6.src");

INLINE_ASM(_func_8C033F04, 0x5c, "asm/nonmatching/./move/_func_8C033F04.src");

INLINE_ASM(_func_8C033F60, 0x28, "asm/nonmatching/./move/_func_8C033F60.src");

// MERGE_LIST([['_AdjustAngle', '_lbl_8C0340E0'], ['_DiffAngle', '_lbl_8C0340E4']]);
INLINE_ASM(_func_8C033F88, 0x2e, "asm/nonmatching/./move/_func_8C033F88.src");

// MERGE_LIST([['_AdjustAngle', '_lbl_8C0340E0'], ['_DiffAngle', '_lbl_8C0340E4']]);
INLINE_ASM(_func_8C033FB6, 0x6c, "asm/nonmatching/./move/_func_8C033FB6.src");

// MERGE_LIST([['_playertwp', '_lbl_8C0340E8'], ['_DiffAngle', '_lbl_8C0340E4']]);
INLINE_ASM(_func_8C034022, 0x34, "asm/nonmatching/./move/_func_8C034022.src");

// MERGE_LIST([['_playertwp', '_lbl_8C0340E8'], ['_njSqrt', '_lbl_8C0340EC']]);
INLINE_ASM(_MOV_Dist2FromPlayer, 0x32, "asm/nonmatching/./move/_MOV_Dist2FromPlayer.src");

// MERGE_LIST([['_playertwp', '_lbl_8C0340E8']]);
INLINE_ASM(_func_8C034088, 0x78, "asm/nonmatching/./move/_func_8C034088.src");

INLINE_ASM(_func_8C034100, 0x28, "asm/nonmatching/./move/_func_8C034100.src");

// MERGE_LIST([['_AdjustAngle', '_lbl_8C034280'], ['_DiffAngle', '_lbl_8C034284']]);
INLINE_ASM(_MOV_TurnToPlayer2, 0x32, "asm/nonmatching/./move/_MOV_TurnToPlayer2.src");

// MERGE_LIST([['_AdjustAngle', '_lbl_8C034280'], ['_DiffAngle', '_lbl_8C034284']]);
INLINE_ASM(_func_8C03415A, 0x146, "asm/nonmatching/./move/_func_8C03415A.src");

INLINE_ASM(_func_8C0342A0, 0xca0, "asm/nonmatching/./move/_func_8C0342A0.src");

// MERGE_LIST([["h'3E4CCCCD", '_lbl_8C0350C0'], ['_lbl_8C11D5F8', '_lbl_8C0350C4'], ["h'41000000", '_lbl_8C0350C8'], ['_njUnitVector', '_lbl_8C0350CC']]);
INLINE_ASM(_func_8C034F40, 0x504, "asm/nonmatching/./move/_func_8C034F40.src");

// MERGE_LIST([["h'8C24F1CC", '_lbl_8C035480']]);
INLINE_ASM(_MOV_DetectCollision, 0x5c, "asm/nonmatching/./move/_MOV_DetectCollision.src");

