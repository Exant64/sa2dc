#include <Chao/Chao.h>
#include <Chao/adv1_al_status.h>

static Uint8 eye_default_num[3][3][3] = {
    {
        {9,0,0},{9,0,0},{9,0,0},
    },
    {
        {9,0,0},{9,0,4},{9,0,4},
    },
    {
        {0xA, 0xA, 0xA}, {9,0,4}, {0,4,4},
    },
};

static Uint8 mouse_default_num[3][3][3] = {
    {
        {8,0,4},{8,8,4},{8,0,3},
    },
    {
        {0,1,1},{0,0,0},{0,3,3},
    },
    {
        {0, 1, 1}, {0,4,4}, {3,3,3},
    },
};

#define GENE_ANALYZE_RAND ((Uint32)(njRandom() * 1.9999f))
#define GENE_CREATE_RAND(num) (Uint32)(njRandom() * (num - 0.0001f))
#define BLEND_GENE(field)                                             \
    pDstGene->##field[0] = pSrcGene1->##field[(int)(njRandom() * 1.99f)]; \
    pDstGene->##field[1] = pSrcGene2->##field[(int)(njRandom() * 1.99f)];

void AL_BlendGene(AL_GENE* pSrcGene1, AL_GENE* pSrcGene2, AL_GENE* pDstGene) ;

void AL_GeneCreate(AL_GENE* pGene) {
    int i;

    for (i = 0; i < 7; i++) {
        pGene->Abl[i][0] = pGene->Abl[i][1] = GENE_CREATE_RAND(5);
    }

    pGene->LifeTime[0] = GENE_CREATE_RAND(10);
    pGene->LifeTime[1] = GENE_CREATE_RAND(10);

    pGene->HPos[0] = 0;
    pGene->HPos[1] = 0;
    pGene->VPos[0] = 0;
    pGene->VPos[1] = 0;
    pGene->APos[0] = 0;
    pGene->APos[1] = 0;

    for (i = 0; i < 13; i++) {
        pGene->Personality[i][0] = GENE_CREATE_RAND(5);
        pGene->Personality[i][1] = GENE_CREATE_RAND(5);
    }

    pGene->Taste[0] = GENE_CREATE_RAND(6);
    pGene->Taste[1] = GENE_CREATE_RAND(6);
    pGene->Tv[0] = GENE_CREATE_RAND(6);
    pGene->Tv[1] = GENE_CREATE_RAND(6);
    pGene->Music[0] = GENE_CREATE_RAND(6);
    pGene->Music[1] = GENE_CREATE_RAND(6);

    pGene->Color[0] = 0;
    pGene->Color[1] = 0;
    pGene->Jewel[0] = 0;
    pGene->Jewel[1] = 0;
    pGene->Multi[0] = 0;
    pGene->Multi[1] = 0;
}

void lbl_0C50BD26(AL_GENE* pGene) {
    AL_GeneCreate(pGene);
    pGene->APos[0] = 5;
    pGene->APos[1] = 5;
}

void lbl_0C50BD40(AL_GENE* pGene) {
    AL_GeneCreate(pGene);
    pGene->APos[0] = -5;
    pGene->APos[1] = -5;
}

void AL_SucceedGeneParam(AL_GENE* pGene, CHAO_PARAM* pParam) {
    int i;

    for (i = 0; i < 7; i++) {
        pGene->Abl[i][0] = pGene->Abl[i][1] = (pParam->Skill[i]) / (GET_GLOBAL()->InitMulPotentialSkill + 1);
    }

    for (i = 0; i < 13; i++) {
        pGene->Personality[i][(int)(njRandom() * 1.99f)] = (pParam->Emotion.Personality[i] / 40) + 2;
    }
}

// permuted function, hence the odd code
void lbl_0C50BE14(chao_info_* pChaoInfo, chao_info_* pChaoInfo2) {
    AL_GENE *pGene1;
    AL_GENE *new_var2;
    chao_info_ *pInfa;
    AL_GENE *pGene2;
    CHAO_PARAM *pParam2;
    chao_info *pGeneInf;
    chao_info_ *new_var;
    AL_GENE *new_var4;
    CHAO_PARAM *pParam2_;
    chao_info *pGeneInf_;
    chao_info_ *new_var_;
    AL_GENE copy;

    new_var2 = &(&((chao_info_ *) pChaoInfo)->gene_pad)->gene;
    new_var4 = new_var2;
    pGene1 = new_var4;

    pGene2 = &(&pChaoInfo2->gene_pad)->gene;

    pGeneInf = pChaoInfo2;
    new_var = pGeneInf;
    pParam2 = &new_var->param;

    AL_BlendGene(pGene1, pGene2, &copy);
    *pGene2 = copy;
    pParam2->type = 1;
}   

void AL_BlendGene(AL_GENE* pSrcGene1, AL_GENE* pSrcGene2, AL_GENE* pDstGene) {
    int i;

    memset(pDstGene, 0, sizeof(*pDstGene));

    for (i = 0; i < 7; i++) {
        BLEND_GENE(Abl[i]);
    }
    BLEND_GENE(LifeTime);
    BLEND_GENE(HPos);
    BLEND_GENE(VPos);
    BLEND_GENE(APos);

    for (i = 0; i < 13; i++) {
        BLEND_GENE(Personality[i]);
    }

    BLEND_GENE(Taste);
    BLEND_GENE(Tv);
    BLEND_GENE(Music);

    BLEND_GENE(Color);
    BLEND_GENE(Jewel);
    BLEND_GENE(Multi);
}

void AL_CreateChildGene(task* pMotherTask, task* pFatherTask, AL_GENE* pChildGene) {
    CHAO_PARAM* pMotherParam = &GET_CHAOWK(pMotherTask)->param;
    CHAO_PARAM* pFatherParam = &GET_CHAOWK(pFatherTask)->param;
    AL_GENE* pMotherGene = &GET_CHAOWK(pMotherTask)->gene;
    AL_GENE* pFatherGene = &GET_CHAOWK(pFatherTask)->gene;
    CHAO_PARAM MotherParam, FatherParam;
    AL_GENE MotherGene, FatherGene;

    MotherParam = *pMotherParam;
    FatherParam = *pFatherParam;

    MotherGene = *pMotherGene;
    FatherGene = *pFatherGene;

    AL_SucceedGeneParam(&MotherGene, &MotherParam);
    AL_SucceedGeneParam(&FatherGene, &FatherParam);

    AL_BlendGene(&MotherGene, &FatherGene, pChildGene);
}


void AL_GeneAnalyzeCommon(task* tp) {
    chaowk* work = GET_CHAOWK(tp);
    AL_GENE* pGene = &work->gene;
    CHAO_PARAM* pParam = &work->param;
    AL_EMOTION* pEmotion = &pParam->Emotion;

    int i;

    AL_ClearParts(tp);

    pParam->type = 2;
    pParam->place = AL_GetStageNumber();
    pParam->ClassNum = -1;

    AL_SetChaoID(tp);

    pParam->Skill[6] = 0;
    pParam->thing = pParam->Skill[6];

    for (i = 0; i < 7; i++) {
        Uint8 abl;

        // higher chance to pick the higher Abl?
        if (pGene->Abl[i][0] >= pGene->Abl[i][1]) {
            if (njRandom() < 0.7f) {
                abl = pGene->Abl[i][0];
            } else {
                abl = pGene->Abl[i][1];
            }
        } else {
            if (njRandom() < 0.7f) {
                abl = pGene->Abl[i][1];
            } else {
                abl = pGene->Abl[i][0];
            }
        }

        pParam->Abl[i] = abl * GET_GLOBAL()->InitMulPotentialSkill;
    }

    if (pGene->LifeTime[0] >= pGene->LifeTime[1]) {
        pParam->life = pParam->LifeMax = GET_GLOBAL()->InitBaseLife + pGene->LifeTime[0] * GET_GLOBAL()->InitMulLife;
    } else {
        pParam->life = pParam->LifeMax = GET_GLOBAL()->InitBaseLife + pGene->LifeTime[1] * GET_GLOBAL()->InitMulLife;
    }

    {
        Sint8 a = pGene->HPos[GENE_ANALYZE_RAND];
        pParam->body.HPos = a * GET_GLOBAL()->InitMulForm;
        a = pGene->VPos[GENE_ANALYZE_RAND];
        pParam->body.VPos = a * GET_GLOBAL()->InitMulForm;
        a = pGene->APos[GENE_ANALYZE_RAND];
        pParam->body.APos = a * GET_GLOBAL()->InitMulAttr;
    }

    pEmotion->State[EM_ST_NOURISHMENT] = GET_GLOBAL()->EmotionStateLimit;
    pEmotion->State[EM_ST_CONDITION] = GET_GLOBAL()->EmotionStateLimit;

    for (i = 0; i < 13; i++) {
        pEmotion->Personality[i] = 40 * (pGene->Personality[i][(int)(njRandom() * 1.99f)] - 2);
    }

    {
        pEmotion->Taste = pGene->Taste[GENE_ANALYZE_RAND];
        pEmotion->Tv = pGene->Tv[GENE_ANALYZE_RAND];
        pEmotion->Music = pGene->Music[GENE_ANALYZE_RAND];
    }

    {
        int a, b, c;

        if (pEmotion->Personality[1] < -35) {
            b = 0;
        } else if (pEmotion->Personality[1] < 35) {
            b = 1;
        } else {
            b = 2;
        }

        if (pEmotion->Personality[2] < -35) {
            a = 0;
        } else if (pEmotion->Personality[2] < 35) {
            a = 1;
        } else {
            a = 2;
        }

        if (pEmotion->Personality[0] < -35) {
            c = 0;
        } else if (pEmotion->Personality[0] < 35) {
            c = 1;
        } else {
            c = 2;
        }

        pParam->body.DefaultEyeNum = eye_default_num[a][b][c];
        pParam->body.DefaultMouthNum = mouse_default_num[a][b][c];
    }

    if (pGene->Color[0]) {
        if (pGene->Color[1]) {
            pParam->body.ColorNum = pGene->Color[GENE_ANALYZE_RAND];
        } else {
            pParam->body.ColorNum = pGene->Color[0];
        }
    } else {
        if (pGene->Color[1]) {
            pParam->body.ColorNum = pGene->Color[1];
        } else {
            pParam->body.ColorNum = 0;
        }
    }

    if (pGene->Jewel[0]) {
        if (pGene->Jewel[1]) {
            pParam->body.JewelNum = pGene->Jewel[GENE_ANALYZE_RAND];
        } else {
            pParam->body.JewelNum = pGene->Jewel[0];
        }
    } else {
        if (pGene->Jewel[1]) {
            pParam->body.JewelNum = pGene->Jewel[1];
        } else {
            pParam->body.JewelNum = 0;
        }
    }

    if (pGene->Multi[0] || pGene->Multi[1]) {
        pParam->body.MultiNum = 1;
    } else {
        pParam->body.MultiNum = 0;
    }
}

// converts sa1dc name character to sa2dc
static Sint8 FUN_0c50c8e0(Uint8 uVar2) {
    Sint8 iVar1;
    if (uVar2 == 0x20 || (uVar2 >= 0x60 && uVar2 <= 99) || (uVar2 >= 0xbE && uVar2 <= 0xbF)) iVar1 = 0x5f;
    else if (uVar2 >= 0x21 && uVar2 <= 0x5f) iVar1 = uVar2 - 0x20;
    else if (uVar2 >= 100 && uVar2 <= 0xb6) iVar1 = uVar2 + 0x3c;
    else if (uVar2 >= 0xb7 && uVar2 <= 0xb8) iVar1 = uVar2 + 0x3f;
    else if (uVar2 == 185) iVar1 = 94;
    else if (uVar2 >= 186 && uVar2 <= 189) iVar1 = uVar2 + 62;
    else if (uVar2 >= 192 && uVar2 <= 194) iVar1 = uVar2 + 51;
    else if (uVar2 >= 195 && uVar2 <= 196) iVar1 = uVar2 + 57;
    else iVar1 = 0;
         
    return iVar1;
}

// converts sa1dc chao to sa2dc
void lbl_0C50C99C(al_status* pStatus, chao_info_* pDst) {
    int i;
    
    CHAO_PARAM* pParam = GET_INFO_PARAM(pDst);
    AL_GENE* pGene = GET_INFO_GENE(pDst);

    memset(pGene, 0, sizeof(AL_GENE));
    memset(pParam, 0, sizeof(CHAO_PARAM));

    AL_GeneCreate(pGene);
    
    pParam->type = 1;
    pParam->like = pStatus->like;

    pParam->unk_2 = 1;

    pParam->body.HPos = pStatus->h_pos;
    pParam->body.VPos = pStatus->v_pos;
    pParam->body.growth = pStatus->growth;

    pParam->nbSucceed = pStatus->nbSucceed + 1;

    for(i = 0; i < 8; i++) {
        pParam->name[i] = FUN_0c50c8e0(pStatus->name[i]);
    }

    pGene->Abl[0][0] = pGene->Abl[0][1] = pStatus->swim / GET_GLOBAL()->InitMulPotentialSkill;
    pGene->Abl[1][0] = pGene->Abl[1][1] = pStatus->fly / GET_GLOBAL()->InitMulPotentialSkill;
    pGene->Abl[2][0] = pGene->Abl[2][1] = pStatus->run / GET_GLOBAL()->InitMulPotentialSkill;
    pGene->Abl[3][0] = pGene->Abl[3][1] = pStatus->power / GET_GLOBAL()->InitMulPotentialSkill;

    if(pStatus->flag & 2) {
        pGene->Color[0] = pGene->Color[1] = 1;
    }
    
    if(pStatus->flag & 8) {
        pGene->Jewel[0] = pGene->Jewel[1] = 2;
    }

    if(pStatus->flag & 4) {
        pGene->Jewel[0] = pGene->Jewel[1] = 1;
    }

    if(pStatus->flag & 0x40) {
        switch(pStatus->extra_num) {
            case 0:
                pGene->Jewel[0] = pGene->Jewel[1] = 8;
                break;
            case 1:
                pGene->Jewel[0] = pGene->Jewel[1] = 9;
                break;
            case 2:
                pGene->Jewel[0] = pGene->Jewel[1] = 10;
                break;
            case 3:
                pGene->Jewel[0] = pGene->Jewel[1] = 11;
                break;
        }
    }

    pGene->Personality[0][0] = pGene->Personality[0][1] = pStatus->curiosity / 40 + 2;
    pGene->Personality[1][0] = pGene->Personality[1][1] = pStatus->kindness / 40 + 2;
    pGene->Personality[2][0] = pGene->Personality[2][1] = pStatus->aggressive / 40 + 2;
}
