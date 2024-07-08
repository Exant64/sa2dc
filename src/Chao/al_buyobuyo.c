#include <task.h>

INLINE_ASM(_AL_InitCalcMotionMatrix, 0x52, "asm/nonmatching/Chao/al_buyobuyo/_AL_InitCalcMotionMatrix.src");

// MERGE_LIST([['_MtnInfo', '_lbl_0C501580']]);
INLINE_ASM(_AL_CalcMotionMatrixTranslate, 0x2ee, "asm/nonmatching/Chao/al_buyobuyo/_AL_CalcMotionMatrixTranslate.src");

INLINE_ASM(_AL_CalcMotionMatrixRotation, 0x220, "asm/nonmatching/Chao/al_buyobuyo/_AL_CalcMotionMatrixRotation.src");

// MERGE_LIST([["h'4F800000", '_lbl_0C501A80'], ['_njRotateXYZ', '_lbl_0C501A84']]);
INLINE_ASM(_AL_CalcMotionMartix, 0x5a, "asm/nonmatching/Chao/al_buyobuyo/_AL_CalcMotionMartix.src");

// MERGE_LIST([['_mdata', '_lbl_0C501A88'], ['_MtnInfo', '_lbl_0C501A8C'], ['_njScale', '_lbl_0C501A90']]);
INLINE_ASM(_AL_SetCurrMatrixSub, 0xa6, "asm/nonmatching/Chao/al_buyobuyo/_AL_SetCurrMatrixSub.src");

INLINE_ASM(_AL_SetCurrMatrix, 0x34, "asm/nonmatching/Chao/al_buyobuyo/_AL_SetCurrMatrix.src");

// MERGE_LIST([['_njPushMatrixEx', '_lbl_0C501C80'], ['_njUnitMatrix', '_lbl_0C501C84'], ['_njTranslateV', '_lbl_0C501C88'], ['_njRotateY', '_lbl_0C501C8C'], ['_njPopMatrixEx', '_lbl_0C501C90']]);
INLINE_ASM(_AL_CalcShadowPos, 0x3b2, "asm/nonmatching/Chao/al_buyobuyo/_AL_CalcShadowPos.src");

INLINE_ASM(_AL_BuyoBuyoObject, 0x49a, "asm/nonmatching/Chao/al_buyobuyo/_AL_BuyoBuyoObject.src");

INLINE_ASM(_AL_BuyoBuyoControl, 0x80, "asm/nonmatching/Chao/al_buyobuyo/_AL_BuyoBuyoControl.src");

