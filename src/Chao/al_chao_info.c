#include <task.h>

INLINE_ASM(_AL_InitChaoID, 0xd2, "asm/nonmatching/Chao/al_chao_info/_AL_InitChaoID.src");

// MERGE_LIST([['_syRtcGetDate', '_lbl_0C5025A0'], ['_rand', '_lbl_0C5025A4'], ["h'3B480000", '_lbl_0C5025A8']]);
INLINE_ASM(_AL_SetChaoID, 0x2a, "asm/nonmatching/Chao/al_chao_info/_AL_SetChaoID.src");

// MERGE_LIST([['_AL_GetGardenID', '_lbl_0C5025AC']]);
INLINE_ASM(_AL_InitChaoSaveInfo, 0xba, "asm/nonmatching/Chao/al_chao_info/_AL_InitChaoSaveInfo.src");

// MERGE_LIST([['_lbl_0C50BD40', '_lbl_0C5025C0'], ['_ChaoInfo', '_lbl_0C5025B0'], ['_memset', '_lbl_0C5025B4'], ['_AL_GeneCreate', '_lbl_0C5025B8'], ['_lbl_0C50BD26', '_lbl_0C5025BC']]);
INLINE_ASM(_AL_ClearChaoParam, 0x6a, "asm/nonmatching/Chao/al_chao_info/_AL_ClearChaoParam.src");

INLINE_ASM(_sub_C5025E0, 0x52, "asm/nonmatching/Chao/al_chao_info/_sub_C5025E0.src");

// MERGE_LIST([['_memset', '_lbl_0C502760'], ['__quick_mvn', '_lbl_0C502764']]);
INLINE_ASM(_sub_C502632, 0x52, "asm/nonmatching/Chao/al_chao_info/_sub_C502632.src");

// MERGE_LIST([['_memset', '_lbl_0C502760'], ['__quick_mvn', '_lbl_0C502764']]);
INLINE_ASM(_AL_GetLocalChaoCount, 0x4a, "asm/nonmatching/Chao/al_chao_info/_AL_GetLocalChaoCount.src");

// MERGE_LIST([['_AL_GetMaxChao', '_lbl_0C502768'], ['_ChaoInfo', '_lbl_0C50276C']]);
INLINE_ASM(_AL_GetCurrGardenChaoCount, 0xe, "asm/nonmatching/Chao/al_chao_info/_AL_GetCurrGardenChaoCount.src");

// MERGE_LIST([['_AL_GetStageNumber', '_lbl_0C502770']]);
INLINE_ASM(_AL_GetAllChaoCount, 0x34, "asm/nonmatching/Chao/al_chao_info/_AL_GetAllChaoCount.src");

// MERGE_LIST([['_AL_GetMaxChao', '_lbl_0C502768'], ['_ChaoInfo', '_lbl_0C50276C']]);
INLINE_ASM(_AL_GetNewChaoSaveInfo, 0x70, "asm/nonmatching/Chao/al_chao_info/_AL_GetNewChaoSaveInfo.src");

INLINE_ASM(_AL_SetChaoOnTheGarden, 0x270, "asm/nonmatching/Chao/al_chao_info/_AL_SetChaoOnTheGarden.src");

// MERGE_LIST([['_lbl_0C569350', '_lbl_0C502A40'], ['_AL_EmotionGetValue', '_lbl_0C502A44'], ['_ALBHV_Sleep', '_lbl_0C502A48'], ['_AL_SetBehavior', '_lbl_0C502A4C']]);
INLINE_ASM(_func_0C5029F0, 0x8e, "asm/nonmatching/Chao/al_chao_info/_func_0C5029F0.src");

// MERGE_LIST([['_CreateChao', '_lbl_0C502BC0']]);
INLINE_ASM(_AL_CreateHoldingChao, 0xe2, "asm/nonmatching/Chao/al_chao_info/_AL_CreateHoldingChao.src");

// MERGE_LIST([['_lbl_0C56934C', '_lbl_0C502BE0'], ['_playertwp', '_lbl_0C502BC4'], ["h'00008000", '_lbl_0C502BE4'], ['_StopHoldingTaskP', '_lbl_0C502BC8'], ['_AL_GetStageNumber', '_lbl_0C502BE8'], ["h'8C2512B8", '_lbl_0C502BCC'], ["h'FFFDFFFF", '_lbl_0C502BEC'], ['_lbl_0C569348', '_lbl_0C502BD0'], ["h'8C04B820", '_lbl_0C502BF0'], ["h'40400000", '_lbl_0C502BD4'], ['_njSin', '_lbl_0C502BD8'], ['_njCos', '_lbl_0C502BDC']]);
INLINE_ASM(_AL_SetHoldingChaoInfo, 0xaa, "asm/nonmatching/Chao/al_chao_info/_AL_SetHoldingChaoInfo.src");

INLINE_ASM(_AL_SetHoldingEggInfo, 0x36, "asm/nonmatching/Chao/al_chao_info/_AL_SetHoldingEggInfo.src");

