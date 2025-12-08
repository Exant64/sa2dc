#include <Chao/Chao.h>

task* CreateChaoFromEgg(task* tp) {
    taskwk* twp = tp->twp;
    chaowk* work = GET_CHAOWK(tp);
    CHAO_PARAM* pParam = GET_CHAOPARAM(tp);
    task* pChao;
    
    if(work->pInfo) {
        AL_ClearChaoParam(work->pInfo);
    }

    ALW_CancelEntry(tp);   

    pChao = CreateChao(&work->gene, NULL, NULL, &twp->pos, twp->ang.y, work->pInfo);
    
    if(pParam->unk_2) {
        CHAO_PARAM* pChaoParam = GET_CHAOPARAM(pChao);

        pChaoParam->like = pParam->like;
        if(pParam->name[0]) {
            njMemCopy(pChaoParam->name, pParam->name, sizeof(pParam->name));
        }

        pChaoParam->nbSucceed = pParam->nbSucceed;

        if(pParam->unk_arr[0]) {
             njMemCopy(pChaoParam->unk_arr, pParam->unk_arr, sizeof(pParam->unk_arr));
        }

        pChaoParam->body.HPos = pParam->body.HPos * 0.2;
        pChaoParam->body.VPos = pParam->body.VPos * 0.2;
        pChaoParam->body.APos = pParam->body.APos * 0.2;

        pChaoParam->body.growth = pParam->body.growth * 0.1;

        njMemCopy(&pChaoParam->body.unk, &pParam->body.unk, sizeof(pParam->body.unk));

        if(njRandom() < 0.3f) {
            pChaoParam->body.ObakeHead = pParam->body.ObakeHead;
        }

        if(njRandom() < 0.3f) {
            pChaoParam->body.ObakeBody = pParam->body.ObakeBody;
        }

        pChaoParam->body.MedalNum = pParam->body.MedalNum;
        pChaoParam->body.ColorNum = pParam->body.ColorNum;
        pChaoParam->body.JewelNum = pParam->body.JewelNum;
        pChaoParam->body.MultiNum = pParam->body.MultiNum;
        pChaoParam->body.BodyType = pParam->body.BodyType;
        pChaoParam->body.FormSubNum = pParam->body.FormSubNum;

        AL_Deform(pChao);
    }

    GET_CHAOWK(pChao)->ang.y = twp->ang.y;
    work->pInfo = NULL;
    
    return pChao;
}
MERGE_LIST([['_rand', '_lbl_0C507940'], ['_AL_ClearChaoParam', '_lbl_0C507920'], ['_ALW_CancelEntry', '_lbl_0C507924'], ['_AL_Deform', '_lbl_0C507944'], ['_CreateChao', '_lbl_0C507928'], ['_njMemCopy', '_lbl_0C50792C'], ["h'3E4CCCCD", '_lbl_0C507930'], ["h'3DCCCCCD", '_lbl_0C507934'], ["h'3E99999A", '_lbl_0C507938'], ["h'38000000", '_lbl_0C50793C']]);

INLINE_ASM(_func_0C507894, 0xcc, "asm/nonmatching/Chao/al_egg/_func_0C507894.src");

INLINE_ASM(_func_0C507960, 0xf8, "asm/nonmatching/Chao/al_egg/_func_0C507960.src");

// MERGE_LIST([['_SaveControl3D', '_lbl_0C507B00'], ['_SaveConstantAttr', '_lbl_0C507B04'], ['_OnControl3D', '_lbl_0C507B08'], ['_AddConstantAttr', '_lbl_0C507B0C'], ['_RemoveConstantAttr', '_lbl_0C507B10'], ['_njColorBlendingMode', '_lbl_0C507B14'], ['_SetMaterial', '_lbl_0C507B18'], ['_AL_OBJECT_TEXLIST', '_lbl_0C507B1C'], ['_njSetTexture', '_lbl_0C507B20'], ['_njPushMatrix', '_lbl_0C507B24'], ['_njTranslateV', '_lbl_0C507B28'], ['_njRotateY', '_lbl_0C507B2C'], ['_object_alm_egg_eggtop', '_lbl_0C507B30'], ['_njCnkEasyDrawModel', '_lbl_0C507B34'], ['_OffControl3D', '_lbl_0C507B38'], ["h'3F000000", '_lbl_0C507B3C'], ['_njTranslate', '_lbl_0C507B40'], ["h'3E4CCCCD", '_lbl_0C507B44'], ["h'3FE66666", '_lbl_0C507B48'], ["h'3F333333", '_lbl_0C507B4C'], ['_njScale', '_lbl_0C507B50'], ['_model_kage_marukage_marukage', '_lbl_0C507B54'], ['_njCnkModDrawModel', '_lbl_0C507B58'], ['_njPopMatrix', '_lbl_0C507B5C'], ['_LoadConstantAttr', '_lbl_0C507B60'], ['_LoadControl3D', '_lbl_0C507B64']]);
INLINE_ASM(_func_0C507A58, 0x4, "asm/nonmatching/Chao/al_egg/_func_0C507A58.src");

INLINE_ASM(_func_0C507A5C, 0x144, "asm/nonmatching/Chao/al_egg/_func_0C507A5C.src");

INLINE_ASM(_func_0C507BA0, 0x98, "asm/nonmatching/Chao/al_egg/_func_0C507BA0.src");

// MERGE_LIST([["h'BF000000", '_lbl_0C507D48'], ["h'40800000", '_lbl_0C507D40'], ["h'3F59999A", '_lbl_0C507D4C'], ['_GetShadowPos', '_lbl_0C507D44']]);
INLINE_ASM(_func_0C507C38, 0x148, "asm/nonmatching/Chao/al_egg/_func_0C507C38.src");

INLINE_ASM(_func_0C507D80, 0xe8, "asm/nonmatching/Chao/al_egg/_func_0C507D80.src");

// MERGE_LIST([['_AL_IsMovable', '_lbl_0C507F40'], ['_njScalor', '_lbl_0C507F60'], ["h'3F4CCCCD", '_lbl_0C507F64'], ['_CCL_Enable', '_lbl_0C507F44'], ["h'3D4CCCCD", '_lbl_0C507F68'], ['_CCL_Disable', '_lbl_0C507F48'], ["h'40200000", '_lbl_0C507F4C'], ['_MOV_ClearVelo', '_lbl_0C507F6C'], ['_AL_CalcWaterAction', '_lbl_0C507F50'], ['_MOV_ClearAcc', '_lbl_0C507F70'], ["h'00008000", '_lbl_0C507F74'], ['_MOV_Control', '_lbl_0C507F54'], ['_MOV_PreservePreviousPosition', '_lbl_0C507F58'], ['_lbl_0C5033F4', '_lbl_0C507F5C']]);
INLINE_ASM(_func_0C507E68, 0x50, "asm/nonmatching/Chao/al_egg/_func_0C507E68.src");

// MERGE_LIST([['_njScalor', '_lbl_0C507F60'], ["h'3E4CCCCD", '_lbl_0C507F7C'], ["h'3DCCCCCD", '_lbl_0C507F78']]);
INLINE_ASM(_func_0C507EB8, 0xe8, "asm/nonmatching/Chao/al_egg/_func_0C507EB8.src");

INLINE_ASM(_func_0C507FA0, 0x180, "asm/nonmatching/Chao/al_egg/_func_0C507FA0.src");

INLINE_ASM(_func_0C508120, 0x10a, "asm/nonmatching/Chao/al_egg/_func_0C508120.src");

// MERGE_LIST([["h'374CCCCD", '_lbl_0C508360'], ['_CCL_Disable', '_lbl_0C508340'], ["h'3E4CCCCD", '_lbl_0C508364'], ['_ALBHV_BornPon', '_lbl_0C508344'], ['_rand', '_lbl_0C508368'], ['_AL_SetBehavior', '_lbl_0C508348'], ["h'3E23D70A", '_lbl_0C50834C'], ['_ALO_ObakeHeadCreate', '_lbl_0C50836C'], ["h'41700000", '_lbl_0C508350'], ['_lbl_0C55DA04', '_lbl_0C508354'], ["h'3FC00000", '_lbl_0C508358'], ["h'3FA66666", '_lbl_0C50835C']]);
INLINE_ASM(_func_0C50822A, 0x156, "asm/nonmatching/Chao/al_egg/_func_0C50822A.src");

INLINE_ASM(_func_0C508380, 0x1ae, "asm/nonmatching/Chao/al_egg/_func_0C508380.src");

// MERGE_LIST([['_CCL_Disable', '_lbl_0C5085A0'], ['_ALO_ObakeHeadCreate', '_lbl_0C5085C0'], ['_lbl_0C52BDC0', '_lbl_0C5085A4'], ['_sub_8C05B020', '_lbl_0C5085C4'], ['_njScalor', '_lbl_0C5085D8'], ['_AL_SetBehavior', '_lbl_0C5085A8'], ["h'3F333333", '_lbl_0C5085C8'], ["h'3FC00000", '_lbl_0C5085AC'], ['_MOV_Control', '_lbl_0C5085CC'], ["h'3FA66666", '_lbl_0C5085B0'], ['_MOV_PreservePreviousPosition', '_lbl_0C5085D0'], ["h'3E4CCCCD", '_lbl_0C5085B4'], ['_lbl_0C5033F4', '_lbl_0C5085D4'], ["h'374CCCCD", '_lbl_0C5085B8'], ["h'3E99999A", '_lbl_0C5085DC'], ['_rand', '_lbl_0C5085BC']]);
INLINE_ASM(_func_0C50852E, 0x36, "asm/nonmatching/Chao/al_egg/_func_0C50852E.src");

// MERGE_LIST([['_CCL_Disable', '_lbl_0C5085A0']]);
INLINE_ASM(_func_0C508564, 0x15c, "asm/nonmatching/Chao/al_egg/_func_0C508564.src");

INLINE_ASM(_func_0C5086C0, 0x214, "asm/nonmatching/Chao/al_egg/_func_0C5086C0.src");

// MERGE_LIST([["h'3F000000", '_lbl_0C508A20'], ["h'3FE66666", '_lbl_0C508A24'], ["h'3F333333", '_lbl_0C508A28'], ['_njScale', '_lbl_0C508A2C'], ['_model_kage_marukage_marukage', '_lbl_0C508A30'], ['_njCnkModDrawModel', '_lbl_0C508A34'], ['_njPopMatrix', '_lbl_0C508A38']]);
INLINE_ASM(_func_0C5088D4, 0x6, "asm/nonmatching/Chao/al_egg/_func_0C5088D4.src");

// MERGE_LIST([['_ALW_CancelEntry', '_lbl_0C508A3C']]);
INLINE_ASM(_CreateChaoEgg, 0x29c, "asm/nonmatching/Chao/al_egg/_CreateChaoEgg.src");

// MERGE_LIST([["h'3E4CCCCD", '_lbl_0C508BA0'], ["h'3DCCCCCD", '_lbl_0C508BA4'], ['_ChaoGlobal', '_lbl_0C508BA8'], ['__quick_mvn', '_lbl_0C508BAC'], ['_AL_GeneCreate', '_lbl_0C508BB0'], ['_AL_GetStageNumber', '_lbl_0C508BB4'], ['_sub_C502632', '_lbl_0C508BB8']]);
INLINE_ASM(_func_0C508B76, 0x46, "asm/nonmatching/Chao/al_egg/_func_0C508B76.src");

INLINE_ASM(_func_0C508BBC, 0x3a, "asm/nonmatching/Chao/al_egg/_func_0C508BBC.src");

