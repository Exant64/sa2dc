#include <task.h>

INLINE_ASM(_ALBHV_BornDamage, 0xe0, "asm/nonmatching/Chao/al_behavior/albhv_damage/_ALBHV_BornDamage.src");

INLINE_ASM(_AL_CheckDamage, 0xe8, "asm/nonmatching/Chao/al_behavior/albhv_damage/_AL_CheckDamage.src");

// MERGE_LIST([['_AL_GetBehavior', '_lbl_0C52C040'], ['_AL_SetBehavior', '_lbl_0C52C060'], ['_ALBHV_Damage', '_lbl_0C52C044'], ['_rand', '_lbl_0C52C064'], ['_ALBHV_Mayu', '_lbl_0C52C048'], ['_ALO_ObakeHeadCreate', '_lbl_0C52C068'], ['_ALBHV_Dead', '_lbl_0C52C04C'], ["h'8C01F7A2", '_lbl_0C52C050'], ["h'3F4CCCCD", '_lbl_0C52C054'], ['_njUnitVector', '_lbl_0C52C058'], ["h'3FC00000", '_lbl_0C52C05C']]);
INLINE_ASM(_ALBHV_Damage, 0x32e, "asm/nonmatching/Chao/al_behavior/albhv_damage/_ALBHV_Damage.src");

// MERGE_LIST([['_AL_IconSet', '_lbl_0C52C3A0']]);
INLINE_ASM(_ALBHV_Thrown, 0x2aa, "asm/nonmatching/Chao/al_behavior/albhv_damage/_ALBHV_Thrown.src");

INLINE_ASM(_AL_CheckBikkuri, 0x3c, "asm/nonmatching/Chao/al_behavior/albhv_damage/_AL_CheckBikkuri.src");

// MERGE_LIST([['_AL_GetBehavior', '_lbl_0C52C660'], ['_AL_SetBehaviorWithTimer', '_lbl_0C52C66C'], ['_ALBHV_Bikkuri', '_lbl_0C52C664'], ['_CCL_IsHitKind2', '_lbl_0C52C668']]);
INLINE_ASM(_ALBHV_Bikkuri, 0x104, "asm/nonmatching/Chao/al_behavior/albhv_damage/_ALBHV_Bikkuri.src");

