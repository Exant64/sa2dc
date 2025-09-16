#include <task.h>
#include <Chao/Chao.h>

extern Uint32 Clock;
extern Uint32 ChaoGlobalTimer;

void AL_ParameterAddGrowth(task* tp, float add);
void AL_ParameterAddAPos(task* tp, float add);
void AL_ParameterAddSkill(task* pChaoTask, Uint16 SkillKind, int SkillVal);

Bool AL_IsHero(Uint8 type) {
    if (type < TYPE_H_NORMAL)
        return FALSE;
    else if (((type - TYPE_H_NORMAL) % 3) == 0)
        return TRUE;
    return FALSE;
}

Bool AL_IsDark(Uint8 type) {
    if (type < TYPE_D_NORMAL)
        return FALSE;
    else if (((type - TYPE_D_NORMAL) % 3) == 0)
        return TRUE;
    return FALSE;
}

Bool AL_IsHero2(task* pChaoTask) {
    int type = GET_CHAOPARAM(pChaoTask)->type;

    if (type < TYPE_H_NORMAL)
        return FALSE;
    else if (((type - TYPE_H_NORMAL) % 3) == 0)
        return TRUE;

    return FALSE;
}

Bool AL_IsDark2(task* pChaoTask) {
    int type = GET_CHAOPARAM(pChaoTask)->type;

    if (type < TYPE_D_NORMAL)
        return FALSE;
    else if (((type - TYPE_D_NORMAL) % 3) == 0)
        return TRUE;

    return FALSE;
}

void AL_CalcParameter(task* tp) {
    CHAO_PARAM* pParam = GET_CHAOPARAM(tp);
    AL_EMOTION* pEmotion = &pParam->Emotion;

    if (!(GET_CHAOWK(tp)->ChaoFlag & 0x20000))
        return;

    if (pParam->LifeTimer++ >= GET_GLOBAL()->ChaoTimerLifeInterval) {
        pParam->LifeTimer = 0;
        pParam->age++;

        if (pParam->type == 2) {
            int i;

            AL_ParameterAddGrowth(tp, GET_GLOBAL()->TimeAddGrowthChild);

            switch (AL_GetStageNumber()) {
                case 2:
                    AL_ParameterAddAPos(tp, GET_GLOBAL()->TimeAddAttrChild);
                    break;
                case 3:
                    AL_ParameterAddAPos(tp, -GET_GLOBAL()->TimeAddAttrChild);
                    break;
            }

            // only in sa2dc !!!!
            if (AL_ParameterGetSkill(tp, SKILL_SWIM) < 150)
                AL_ParameterAddSkill(tp, SKILL_SWIM, 4);
            if (AL_ParameterGetSkill(tp, SKILL_FLY) < 150)
                AL_ParameterAddSkill(tp, SKILL_FLY, 4);
            if (AL_ParameterGetSkill(tp, SKILL_RUN) < 150)
                AL_ParameterAddSkill(tp, SKILL_RUN, 4);
            if (AL_ParameterGetSkill(tp, SKILL_POWER) < 150)
                AL_ParameterAddSkill(tp, SKILL_POWER, 4);
            if (AL_ParameterGetSkill(tp, SKILL_INTELLECT) < 150)
                AL_ParameterAddSkill(tp, SKILL_INTELLECT, 4);
            if (AL_ParameterGetSkill(tp, SKILL_STAMINA) < 150)
                AL_ParameterAddSkill(tp, SKILL_STAMINA, 4);
        } else {
            int lifemax;

            lifemax = pParam->LifeMax;
            pParam->old++;

            if (pParam->type < 0x14) {
                lifemax -= (pParam->old + 100) / 15;
                if (lifemax < 0) {
                    lifemax = 0;
                }
                pParam->LifeMax = lifemax;
            }

            if (pParam->life > pParam->LifeMax) {
                pParam->life = pParam->LifeMax;
            }

            AL_ParameterAddGrowth(tp, GET_GLOBAL()->TimeAddGrowthAdult);

            switch (AL_GetStageNumber()) {
                case 2:
                    AL_ParameterAddGrowth(tp, GET_GLOBAL()->TimeAddAttrAdult);
                    break;
                case 3:
                    AL_ParameterAddGrowth(tp, -GET_GLOBAL()->TimeAddAttrAdult);
                    break;
            }
        }
        if (njRandom() < (1 / 6000.f)) {
            int illness = (Uint32)(njRandom() * 5.9999f);
            pEmotion = &GET_CHAOPARAM(tp)->Emotion;
            if (illness != 3) {
                if (pEmotion->IllState[illness] == 0) {
                    pEmotion->IllState[illness] = -100;
                }
            }
        }
    }

    if (pEmotion->MoodTimer++ >= GET_GLOBAL()->ChaoTimerMoodInterval) {
        pEmotion->MoodTimer = 0;

        AL_EmotionAdd(tp, EM_MD_PLEASURE, -GET_GLOBAL()->ParamSubPleasure); // Joy
        AL_EmotionAdd(tp, EM_MD_ANGER,
                      -GET_GLOBAL()->ParamSubAnger * (300 - AL_EmotionGetValue(tp, EM_PER_AGRESSIVE)) / 300);
        AL_EmotionAdd(tp, EM_MD_ANGER,
                      -GET_GLOBAL()->ParamSubAnger * (AL_EmotionGetValue(tp, EM_PER_KINDNESS) + 300) / 300);
        AL_EmotionAdd(tp, EM_MD_ANGER,
                      -GET_GLOBAL()->ParamSubAnger * (AL_EmotionGetValue(tp, EM_PER_CALM) + 300) / 300);
        AL_EmotionAdd(tp, EM_MD_SORROW,
                      -GET_GLOBAL()->ParamSubSorrow * (AL_EmotionGetValue(tp, EM_PER_AGRESSIVE) + 300) / 300);
        AL_EmotionAdd(tp, EM_MD_SORROW,
                      -GET_GLOBAL()->ParamSubSorrow * (AL_EmotionGetValue(tp, EM_PER_CALM) + 300) / 300);
        AL_EmotionAdd(tp, EM_MD_FEAR,
                      -GET_GLOBAL()->ParamSubFear * (AL_EmotionGetValue(tp, EM_PER_AGRESSIVE) + 300) / 300);
        AL_EmotionAdd(tp, EM_MD_FEAR, -GET_GLOBAL()->ParamSubFear * (AL_EmotionGetValue(tp, EM_PER_CALM) + 300) / 300);
        AL_EmotionAdd(tp, EM_MD_SURPRISE,
                      -GET_GLOBAL()->ParamSubSurprise * (AL_EmotionGetValue(tp, EM_PER_CALM) + 300) / 300);
        AL_EmotionAdd(tp, EM_MD_PAIN,
                      -GET_GLOBAL()->ParamSubPain * (AL_EmotionGetValue(tp, EM_PER_REGAIN) + 300) / 300);
        AL_EmotionAdd(tp, EM_MD_RELAX,
                      -GET_GLOBAL()->ParamSubRelax * (AL_EmotionGetValue(tp, EM_PER_CURIOSITY) + 300) / 300);
        AL_EmotionAdd(tp, EM_MD_RELAX,
                      -GET_GLOBAL()->ParamSubRelax * (AL_EmotionGetValue(tp, EM_PER_FICKLE) + 300) / 300);
    }

    if (pEmotion->Timer++ >= GET_GLOBAL()->ChaoTimerEmotionInterval) {
        pEmotion->Timer = 0;

        AL_EmotionAdd(tp, EM_ST_SLEEPY,
                      (GET_GLOBAL()->ParamAddSleepy * (AL_EmotionGetValue(tp, EM_PER_SLEEPY_HEAD) + 300) / 300) *
                          (AL_EmotionGetValue(tp, EM_ST_TIRE) * (3 / 50000.f) + 1.0f));

        AL_EmotionAdd(tp, EM_ST_HUNGER,
                      GET_GLOBAL()->ParamAddHunger * (AL_EmotionGetValue(tp, EM_PER_GLUTTON) + 300) / 300); // hunger
        if (pParam->type > 2 && pParam->type < 0x14) {                                                      // adult
            AL_EmotionAdd(tp, EM_ST_BREED,
                          GET_GLOBAL()->ParamAddBreed * (AL_EmotionGetValue(tp, EM_PER_VITALITY) + 300) /
                              300); // desiretomate
        }
        AL_EmotionAdd(tp, EM_ST_TEDIOUS,
                      GET_GLOBAL()->ParamAddTediousCuriosity * (AL_EmotionGetValue(tp, EM_PER_CURIOSITY) + 300) / 300);
        AL_EmotionAdd(tp, EM_ST_TEDIOUS,
                      GET_GLOBAL()->ParamAddTediousAggressive * (AL_EmotionGetValue(tp, EM_PER_AGRESSIVE) + 300) / 300);
        AL_EmotionAdd(tp, EM_ST_TEDIOUS,
                      GET_GLOBAL()->ParamAddTediousCalm * (300 - AL_EmotionGetValue(tp, EM_PER_CALM)) / 300);
        if (AL_EmotionGetValue(tp, EM_ST_TEDIOUS) > 9999) {
            if (njRandom() < 0.1)
                AL_EmotionSetValue(tp, EM_ST_TEDIOUS, 6000);
        }
        AL_EmotionAdd(tp, EM_ST_LONELY,
                      GET_GLOBAL()->ParamAddLonely * (300 - AL_EmotionGetValue(tp, EM_PER_SOLITUDE)) / 300); // Lonely
        AL_EmotionAdd(tp, EM_ST_TIRE,
                      GET_GLOBAL()->ParamAddTireNormal * (300 - AL_EmotionGetValue(tp, EM_PER_VITALITY)) / 300); // Tire
        AL_EmotionAdd(tp, EM_ST_NOURISHMENT, -GET_GLOBAL()->ParamSubNourishment); // Nourish

        // THIS SEPARATED PORTION IS NOT IN SADX!!!!
        if (AL_EmotionGetValue(tp, EM_ST_NOURISHMENT) > GET_GLOBAL()->ParamConditionTriggerAddN)
            AL_EmotionAdd(tp, EM_ST_CONDITION, GET_GLOBAL()->ParamAddConditionN);
        if (AL_EmotionGetValue(tp, EM_ST_NOURISHMENT) < GET_GLOBAL()->ParamConditionTriggerSubN)
            AL_EmotionAdd(tp, EM_ST_CONDITION, -GET_GLOBAL()->ParamSubConditionN);
        if (AL_EmotionGetValue(tp, EM_ST_STRESS) > GET_GLOBAL()->ParamConditionTriggerS)
            AL_EmotionAdd(tp, EM_ST_CONDITION, -GET_GLOBAL()->ParamSubConditionS);
        if (AL_EmotionGetValue(tp, EM_MD_PAIN) > 100)
            AL_EmotionAdd(tp, EM_ST_CONDITION, -GET_GLOBAL()->ParamSubConditionP);

        AL_EmotionAdd(tp, EM_ST_THIRSTY, GET_GLOBAL()->ParamAddThirsty); // Conditn
    }

    if (pEmotion->IllTimer++ >= GET_GLOBAL()->ChaoTimerIllInterval) {
        int i;
        pEmotion->IllTimer = 0;
        for (i = 0; i < 6; i++) {
            if (pEmotion->IllState[i] < 0) {
                pEmotion->IllState[i]++;
            } else if (pEmotion->IllState[i] > 0) {

                pEmotion->IllState[i]--;
            } else {
            }
        }
    }

    AL_KW_Control(tp);
}

int AL_ParameterGetSkill(task* pChaoTask, Uint16 SkillKind) {
    chaowk* work = GET_CHAOWK(pChaoTask);
    CHAO_PARAM* pParam = &work->param;

    if (SkillKind >= 0 && SkillKind < 7) {
        return pParam->Skill[SkillKind];
    }

    return -1;
}

void AL_ParameterAddSkill(task* pChaoTask, Uint16 SkillKind, int SkillVal) {
    CHAO_PARAM* pParam = GET_CHAOPARAM(pChaoTask);
    chaowk* work = GET_CHAOWK(pChaoTask);

    if (SkillKind >= 0 && SkillKind < 7) {
        int skill = pParam->Skill[SkillKind];

        if (pParam->Abl[SkillKind] > pParam->Skill[SkillKind])
            SkillVal *= 2;

        skill += SkillVal;

        if (skill > GET_GLOBAL()->SkillLimit)
            skill = GET_GLOBAL()->SkillLimit;
        if (skill < 0)
            skill = 0;

        pParam->Skill[SkillKind] = skill;

        if (pParam->Abl[SkillKind] < skill) {
            pParam->Abl[SkillKind] = skill;
        }
    }
}

// not setskill, but i dont actually know what it is
void AL_ParameterSetSkill(task* pChaoTask, Sint16 Value) {
    CHAO_PARAM* pParam = GET_CHAOPARAM(pChaoTask);
    chaowk* work = GET_CHAOWK(pChaoTask);

    int x = pParam->thing;
    x += Value;
    if (x < 0)
        x = 0;
    pParam->thing = x;
}

void AL_ParameterAddGrowth(task* tp, float add) {
    CHAO_PARAM* pParam = GET_CHAOPARAM(tp);
    AL_SHAPE* pShape = &GET_CHAOWK(tp)->Shape;

    pParam->body.growth += add;

    if (pParam->body.growth > GET_GLOBAL()->GrowthLimit) {
        pParam->body.growth = GET_GLOBAL()->GrowthLimit;
    }

    if (pParam->body.growth < 0) {
        pParam->body.growth = 0;
    }

    pShape->Flag |= AL_SHAPE_FLAG_DEFORM;
}

void AL_ParameterAddHPos(task* tp, float add) {
    CHAO_PARAM* pParam = GET_CHAOPARAM(tp);
    AL_SHAPE* pShape = &GET_CHAOWK(tp)->Shape;

    pParam->body.HPos += add;

    if (pParam->body.HPos > GET_GLOBAL()->DeformLimit) {
        pParam->body.HPos = GET_GLOBAL()->DeformLimit;
    }

    if (pParam->body.HPos < -GET_GLOBAL()->DeformLimit) {
        pParam->body.HPos = -GET_GLOBAL()->DeformLimit;
    }

    pShape->Flag |= AL_SHAPE_FLAG_DEFORM;
}

void AL_ParameterAddVPos(task* tp, float add) {
    CHAO_PARAM* pParam = GET_CHAOPARAM(tp);
    AL_SHAPE* pShape = &GET_CHAOWK(tp)->Shape;

    pParam->body.VPos += add;

    if (pParam->body.VPos > GET_GLOBAL()->DeformLimit) {
        pParam->body.VPos = GET_GLOBAL()->DeformLimit;
    }

    if (pParam->body.VPos < -GET_GLOBAL()->DeformLimit) {
        pParam->body.VPos = -GET_GLOBAL()->DeformLimit;
    }

    pShape->Flag |= AL_SHAPE_FLAG_DEFORM;
}

void AL_ParameterAddAPos(task* tp, float add) {
    CHAO_PARAM* pParam = GET_CHAOPARAM(tp);
    AL_SHAPE* pShape = &GET_CHAOWK(tp)->Shape;

    pParam->body.APos += add;

    if (pParam->body.APos > 1) {
        pParam->body.APos = 1;
    }

    if (pParam->body.APos < -1) {
        pParam->body.APos = -1;
    }

    pShape->Flag |= AL_SHAPE_FLAG_DEFORM;
}

void AL_ParameterAddLife(task* tp, int add) {
    CHAO_PARAM* pParam = GET_CHAOPARAM(tp);

    int life = pParam->life;
    int lifemax = pParam->LifeMax;

    life += add;

    if (life > lifemax) {
        life = lifemax;
    }

    if (life < 0) {
        life = 0;
    }

    pParam->life = life;
}

void AL_ParameterAddLifeMax(task* tp, int add) {
    CHAO_PARAM* pParam = GET_CHAOPARAM(tp);

    int life = pParam->life;
    int lifemax = pParam->LifeMax;

    lifemax += add;

    if (lifemax > GET_GLOBAL()->LifeMaxLimit) {
        lifemax = GET_GLOBAL()->LifeMaxLimit;
    }

    if (lifemax < 0) {
        lifemax = 0;
    }

    if (life > lifemax)
        life = lifemax;

    pParam->life = life;
    pParam->LifeMax = lifemax;
}

void AL_ParameterAddUserLike(task* tp, int add, float AddAttr) {
    CHAO_PARAM* pParam = GET_CHAOPARAM(tp);
    int like = pParam->like;

    like += add;

    if (like > 100)
        like = 100;
    else if (like < -100)
        like = -100;

    pParam->like = like;

    switch (GetCharID(0)) {
        case 0:
        case 2:
        case 4:
        case 8:
            AL_ParameterAddAPos(tp, AddAttr);
            break;
        case 1:
        case 3:
        case 5:
            AL_ParameterAddAPos(tp, -AddAttr);
    }

    AL_KW_AddPlayerLike(tp, add);

    if (add > 0) {
        AL_IconSet(tp, 4, 0xB4);
    } else {
        AL_IconSet(tp, 5, 0xB4);
    }
}

void AL_ParameterControlInit() {
    Clock = 0;
    ChaoGlobalTimer = 0;
}

void AL_ParameterControl() {
    if (++Clock >= GET_GLOBAL()->ChaoTimerBaseInterval) {
        task* pChao;
        int index;

        Clock = 0;
        index = ChaoGlobalTimer % nbMaxEntry[0];

        pChao = ALW_GetTask(ALW_CATEGORY_CHAO, index);
        if (pChao) {
            AL_CalcParameter(pChao);
        }
        ++ChaoGlobalTimer;
    }
}

Bool AL_IsLifeTimerReset(task* tp) {
    chaowk* work = GET_CHAOWK(tp);
    CHAO_PARAM* pParam = &work->param;

    if (0)
        ;
    else if (pParam->LifeTimer)
        ;
    else {
        pParam->LifeTimer++;
        return TRUE;
    }

    return FALSE;
}

Bool AL_IsEmotionTimerReset(task* tp) {
    AL_EMOTION* pEmotion = &GET_CHAOPARAM(tp)->Emotion;

    if (0)
        ;
    else if (pEmotion->Timer)
        ;
    else {
        pEmotion->Timer++;
        return TRUE;
    }

    return FALSE;
}

void AL_ShapeElementFromParam(AL_SHAPE_ELEMENT* pElement, CHAO_PARAM* pParam) {
    pElement->type = pParam->type;
    pElement->DefaultEyeNum = pParam->body.DefaultEyeNum;
    pElement->DefaultMouthNum = pParam->body.DefaultMouthNum;
    pElement->HonbuNum = pParam->body.HonbuNum;
    pElement->ObakeHead = pParam->body.ObakeHead;
    pElement->ObakeBody = pParam->body.ObakeBody;
    pElement->MedalNum = pParam->body.MedalNum;
    pElement->ColorNum = pParam->body.ColorNum;
    pElement->JewelNum = pParam->body.JewelNum;
    pElement->MultiNum = pParam->body.MultiNum;

    njMemCopy(pElement->MinimalParts, pParam->Parts.MinimalParts, sizeof(pElement->MinimalParts));

    pElement->HPos = 10000.f * pParam->body.HPos;
    pElement->VPos = 10000.f * pParam->body.VPos;
    pElement->APos = 10000.f * pParam->body.APos;
    pElement->Growth = 10000.f * pParam->body.growth;

    njMemCopy(pElement->name, pParam->name, sizeof(pElement->name));
    njMemCopy(pElement->Skill, pParam->Skill, sizeof(pElement->Skill));
}

void AL_ShapeExpandElementToParam(CHAO_PARAM* pParam, AL_SHAPE_ELEMENT* pElement) {
    pParam->type = pElement->type;
    pParam->body.DefaultEyeNum = pElement->DefaultEyeNum;
    pParam->body.DefaultMouthNum = pElement->DefaultMouthNum;
    pParam->body.HonbuNum = pElement->HonbuNum;
    pParam->body.ObakeHead = pElement->ObakeHead;
    pParam->body.ObakeBody = pElement->ObakeBody;
    pParam->body.MedalNum = pElement->MedalNum;
    pParam->body.ColorNum = pElement->ColorNum;
    pParam->body.JewelNum = pElement->JewelNum;
    pParam->body.MultiNum = pElement->MultiNum;

    njMemCopy(pParam->Parts.MinimalParts, pElement->MinimalParts, sizeof(pParam->Parts.MinimalParts));

    pParam->body.HPos = pElement->HPos / 10000.f;
    pParam->body.VPos = pElement->VPos / 10000.f;
    pParam->body.APos = pElement->APos / 10000.f;
    pParam->body.growth = pElement->Growth / 10000.f;

    njMemCopy(pParam->name, pElement->name, sizeof(pParam->name));
    njMemCopy(pParam->Skill, pElement->Skill, sizeof(pParam->Skill));
}