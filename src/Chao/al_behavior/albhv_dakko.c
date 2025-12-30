#include <task.h>

INLINE_ASM(_DakkoControl, 0x276, "asm/nonmatching/Chao/al_behavior/albhv_dakko/_DakkoControl.src");

// MERGE_LIST([['_StopHoldingTaskP', '_lbl_0C52B840'], ['_ALBHV_DakkoNormal', '_lbl_0C52B844'], ['_ALBHV_DakkoDislike', '_lbl_0C52B848'], ['_ALBHV_DakkoLike', '_lbl_0C52B84C']]);
INLINE_ASM(_ALBHV_DakkoLike, 0x1c4, "asm/nonmatching/Chao/al_behavior/albhv_dakko/_ALBHV_DakkoLike.src");

// MERGE_LIST([["h'3F59999A", '_lbl_0C52B9E0']]);
INLINE_ASM(_ALBHV_DakkoNormal, 0x4a, "asm/nonmatching/Chao/al_behavior/albhv_dakko/_ALBHV_DakkoNormal.src");

// MERGE_LIST([['_AL_SetMotionLink', '_lbl_0C52B9E4']]);
INLINE_ASM(_ALBHV_DakkoDislike, 0x1e6, "asm/nonmatching/Chao/al_behavior/albhv_dakko/_ALBHV_DakkoDislike.src");

// MERGE_LIST([["h'3F333333", '_lbl_0C52BB60'], ["h'3F4CCCCD", '_lbl_0C52BB64'], ["h'3F666666", '_lbl_0C52BB68'], ["h'3B8D0000", '_lbl_0C52BB6C'], ["h'3ECCCCCD", '_lbl_0C52BB70'], ["h'3EA8F5C3", '_lbl_0C52BB78'], ['_sub_8C05B020', '_lbl_0C52BB74'], ["h'3F28F5C3", '_lbl_0C52BB7C']]);
INLINE_ASM(_ALBHV_DakkoTron, 0xd4, "asm/nonmatching/Chao/al_behavior/albhv_dakko/_ALBHV_DakkoTron.src");

// MERGE_LIST([['_AL_SetMotionLink', '_lbl_0C52BCA0'], ['_AL_FaceSetEye', '_lbl_0C52BCA4'], ['_AL_IconSet', '_lbl_0C52BCA8'], ['_AL_EmotionAdd', '_lbl_0C52BCAC'], ['_AL_EmotionGetValue', '_lbl_0C52BCB0']]);
INLINE_ASM(_ALBHV_Dakko, 0xe2, "asm/nonmatching/Chao/al_behavior/albhv_dakko/_ALBHV_Dakko.src");

INLINE_ASM(_AL_CheckDakko, 0xe0, "asm/nonmatching/Chao/al_behavior/albhv_dakko/_AL_CheckDakko.src");

