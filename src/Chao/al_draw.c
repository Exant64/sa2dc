#include <task.h>

INLINE_ASM(_AL_DrawSub, "asm/nonmatching/Chao/al_draw/_AL_DrawSub.src");

// MERGE_LIST([['_njCnkSimpleDrawModel', '_lbl_0C503AC0'], ['_njPopMatrixEx', '_lbl_0C503AC4']]);
INLINE_ASM(_AL_ShadowDraw, "asm/nonmatching/Chao/al_draw/_AL_ShadowDraw.src");

// MERGE_LIST([['_lbl_0C56B098', '_lbl_0C503D40'], ['_njPushMatrixEx', '_lbl_0C503D44'], ['_njRotateY', '_lbl_0C503D48'], ["h'3FCCCCCD", '_lbl_0C503D4C'], ['_lbl_0C56B090', '_lbl_0C503D50'], ["h'3C23D70A", '_lbl_0C503D54'], ["h'3F19999A", '_lbl_0C503D58'], ["h'3F266666", '_lbl_0C503D5C'], ['_lbl_0C56B08C', '_lbl_0C503D60'], ["h'3F666666", '_lbl_0C503D64'], ["h'3A83126F", '_lbl_0C503D68'], ['_njTranslate', '_lbl_0C503D6C'], ["h'3F000000", '_lbl_0C503D70']]);
INLINE_ASM(_AL_Draw, "asm/nonmatching/Chao/al_draw/_AL_Draw.src");

