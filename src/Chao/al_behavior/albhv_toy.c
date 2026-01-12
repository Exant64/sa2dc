#include <task.h>

INLINE_ASM(_ALO_ShabonDisplayer, 0x94, "asm/nonmatching/Chao/al_behavior/albhv_toy/_ALO_ShabonDisplayer.src");

// MERGE_LIST([['_njColorBlendingMode', '_lbl_0C5393C0'], ["h'8C11B96C", '_lbl_0C5393E0'], ['_AL_TOY_TEXLIST', '_lbl_0C5393C4'], ['_njPopMatrixEx', '_lbl_0C5393E4'], ['_njSetTexture', '_lbl_0C5393C8'], ['_SetMaterial', '_lbl_0C5393CC'], ['_anim', '_lbl_0C5393D0'], ["h'3E4CCCCD", '_lbl_0C5393D4'], ['_njPushMatrixEx', '_lbl_0C5393D8'], ['_njTranslateEx', '_lbl_0C5393DC']]);
INLINE_ASM(_ALO_ShabonExecutor, 0xa2, "asm/nonmatching/Chao/al_behavior/albhv_toy/_ALO_ShabonExecutor.src");

// MERGE_LIST([["h'3C23D70A", '_lbl_0C539400'], ["h'3CA3D70A", '_lbl_0C539404'], ['_njScalor', '_lbl_0C5393E8'], ['_MOV_Control', '_lbl_0C539408'], ["h'3D4CCCCD", '_lbl_0C5393EC'], ["h'3F666666", '_lbl_0C5393F0'], ["h'3E19999A", '_lbl_0C5393F4'], ['_FreeTask', '_lbl_0C5393F8'], ['_njSin', '_lbl_0C5393FC']]);
INLINE_ASM(_ALO_ShabonCreate, 0xb2, "asm/nonmatching/Chao/al_behavior/albhv_toy/_ALO_ShabonCreate.src");

// MERGE_LIST([['_MOV_SetGravity', '_lbl_0C539420'], ["h'40400000", '_lbl_0C539424'], ["h'3F4CCCCD", '_lbl_0C539428'], ['_lbl_0C5672FC', '_lbl_0C53940C'], ["h'3F333333", '_lbl_0C53942C'], ['_ALO_ShabonExecutor', '_lbl_0C539410'], ['_rand', '_lbl_0C539430'], ['_CreateElementalTask', '_lbl_0C539414'], ["h'3799999A", '_lbl_0C539434'], ['_ALO_ShabonDisplayer', '_lbl_0C539418'], ['_MOV_Init', '_lbl_0C53941C']]);
INLINE_ASM(_ALBHV_Syabon, 0x200, "asm/nonmatching/Chao/al_behavior/albhv_toy/_ALBHV_Syabon.src");

// MERGE_LIST([['_AL_SetMotionLink', '_lbl_0C539620'], ["h'38000000", '_lbl_0C539640'], ["h'3E800000", '_lbl_0C539660'], ['_AL_FaceSetEye', '_lbl_0C539624'], ["h'3ECCCCCD", '_lbl_0C539644'], ['_AL_TOY_TEXLIST', '_lbl_0C539628'], ["h'364CCCCD", '_lbl_0C539648'], ["h'3D4CCCCD", '_lbl_0C539658'], ['_lbl_0C632400', '_lbl_0C53962C'], ["h'3E4CCCCD", '_lbl_0C53964C'], ['_AL_SetItem', '_lbl_0C539630'], ["h'3DCCCCCD", '_lbl_0C539650'], ['_lbl_0C632628', '_lbl_0C539634'], ["h'3F000000", '_lbl_0C539654'], ['_rand', '_lbl_0C539638'], ['_AL_IsMotionStop', '_lbl_0C53965C'], ['__modls', '_lbl_0C53963C']]);
INLINE_ASM(_ALBHV_Houki, 0xf8, "asm/nonmatching/Chao/al_behavior/albhv_toy/_ALBHV_Houki.src");

INLINE_ASM(_ALBHV_Garagara, 0x82, "asm/nonmatching/Chao/al_behavior/albhv_toy/_ALBHV_Garagara.src");

// MERGE_LIST([['_AL_SetMotionLink', '_lbl_0C5397C0'], ['_AL_FaceSetEye', '_lbl_0C5397C4'], ['_AL_TOY_TEXLIST', '_lbl_0C5397C8'], ['_object_alo_gara_n_gara_n', '_lbl_0C5397CC'], ['_AL_SetItem', '_lbl_0C5397D0'], ['_rand', '_lbl_0C5397D4'], ["h'3BC90000", '_lbl_0C5397D8']]);
INLINE_ASM(_ALBHV_Hanabi, 0x82, "asm/nonmatching/Chao/al_behavior/albhv_toy/_ALBHV_Hanabi.src");

// MERGE_LIST([['_AL_SetMotionLink', '_lbl_0C5397C0'], ['_AL_FaceSetEye', '_lbl_0C5397C4'], ['_AL_TOY_TEXLIST', '_lbl_0C5397C8'], ['_AL_SetItem', '_lbl_0C5397D0'], ['_rand', '_lbl_0C5397D4'], ["h'3BC90000", '_lbl_0C5397D8'], ['_object_alo_hanabi_h_hanabi_h', '_lbl_0C5397DC']]);
INLINE_ASM(_BookExecutor, 0xd4, "asm/nonmatching/Chao/al_behavior/albhv_toy/_BookExecutor.src");

// MERGE_LIST([["h'41700000", '_lbl_0C539920'], ["h'3E19999A", '_lbl_0C539924'], ["h'41B80000", '_lbl_0C539928'], ['_FreeTask', '_lbl_0C53992C']]);
INLINE_ASM(_BookDisplayer, 0x5e, "asm/nonmatching/Chao/al_behavior/albhv_toy/_BookDisplayer.src");

// MERGE_LIST([['_njTranslateV', '_lbl_0C539940'], ['_njPopMatrixEx', '_lbl_0C539958'], ['_njRotateY', '_lbl_0C539944'], ['_njScale', '_lbl_0C539948'], ['_motion_alo_book_pos', '_lbl_0C53994C'], ["h'3C23D70A", '_lbl_0C539930'], ['_object_alo_book_pos_pos', '_lbl_0C539950'], ['_AL_TOY_TEXLIST', '_lbl_0C539934'], ['_njCnkEasyDrawMotion', '_lbl_0C539954'], ['_njSetTexture', '_lbl_0C539938'], ['_njPushMatrixEx', '_lbl_0C53993C']]);
INLINE_ASM(_BookDestructor, 0x4, "asm/nonmatching/Chao/al_behavior/albhv_toy/_BookDestructor.src");

INLINE_ASM(_BookCreate, 0xc6, "asm/nonmatching/Chao/al_behavior/albhv_toy/_BookCreate.src");

INLINE_ASM(_ALBHV_Book, 0x11a, "asm/nonmatching/Chao/al_behavior/albhv_toy/_ALBHV_Book.src");

// MERGE_LIST([["h'38000000", '_lbl_0C539AE0'], ['_AL_IsMotionStop', '_lbl_0C539B00'], ['_AL_SetMotionLink', '_lbl_0C539AE4'], ["h'3BB50000", '_lbl_0C539B04'], ['_rand', '_lbl_0C539AE8'], ['_AL_FixPosition', '_lbl_0C539AEC'], ["h'3F000000", '_lbl_0C539AF0'], ['_AL_FaceSetEye', '_lbl_0C539AF4'], ["h'3B4A0000", '_lbl_0C539AF8'], ["h'3E99999A", '_lbl_0C539AFC']]);
INLINE_ASM(_ALBHV_Minicar, 0x178, "asm/nonmatching/Chao/al_behavior/albhv_toy/_ALBHV_Minicar.src");

// MERGE_LIST([['_AL_TOY_TEXLIST', '_lbl_0C539CA0'], ['_object_alo_minicar_minicar', '_lbl_0C539CA4'], ['_AL_SetItemParallelRightHand', '_lbl_0C539CA8'], ["h'3B4A0000", '_lbl_0C539CAC'], ["h'3E4CCCCD", '_lbl_0C539CB0'], ['_AL_IsMotionStop', '_lbl_0C539CB4'], ["h'3BB50000", '_lbl_0C539CB8']]);
INLINE_ASM(_ALBHV_Doll, 0xe6, "asm/nonmatching/Chao/al_behavior/albhv_toy/_ALBHV_Doll.src");

INLINE_ASM(_ALBHV_Hopping, 0x80, "asm/nonmatching/Chao/al_behavior/albhv_toy/_ALBHV_Hopping.src");

// MERGE_LIST([['_AL_SetMotionLink', '_lbl_0C539E80'], ['_AL_FaceSetEye', '_lbl_0C539E84'], ['_AL_TOY_TEXLIST', '_lbl_0C539E88'], ['_object_alo_hopping_hopping', '_lbl_0C539E8C'], ['_AL_SetItem', '_lbl_0C539E90'], ['_rand', '_lbl_0C539E94'], ["h'3C168000", '_lbl_0C539E98']]);
INLINE_ASM(_AL_DecideBehaviorSToy, 0x164, "asm/nonmatching/Chao/al_behavior/albhv_toy/_AL_DecideBehaviorSToy.src");

INLINE_ASM(_func_0C539EDC, 0x34, "asm/nonmatching/Chao/al_behavior/albhv_toy/_func_0C539EDC.src");

// MERGE_LIST([['_AL_SetMotionLink', '_lbl_0C539FE0'], ['_rand', '_lbl_0C539FE4']]);
INLINE_ASM(_rodata, "asm/nonmatching/Chao/al_behavior/albhv_toy/rodata.src");

