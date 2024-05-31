#include <Chao/Chao.h>
#include <Chao/al_misc.h>

float AL_CalcScoreTypeA(int value, int trigger) {
    float fValue = value;
    float fTrigger = trigger;

    if (value > trigger) {
        return (fValue - fTrigger) / (GET_GLOBAL()->EmotionStateLimit - fTrigger);
    }

    return 0;
}

float AL_CalcScoreTypeB(int value, int trigger) {
    float fValue = value;
    float fTrigger = trigger;

    if (value < trigger) {
        return (fTrigger - fValue) / fTrigger;
    }

    return 0;
}

float AL_CalcMoodScoreTypeA(int value, int trigger) {
    float fValue = value;
    float fTrigger = trigger;

    if (value > trigger) {
        return (fValue - fTrigger) / (200 - fTrigger);
    }

    return 0;
}

// this is unused and probably because it's the same thing as the other score type B lol
float AL_CalcMoodScoreTypeB(int value, int trigger) {
    float fValue = value;
    float fTrigger = trigger;

    if (value < trigger) {
        return (fTrigger - fValue) / fTrigger;
    }

    return 0;
}

void AL_ScoreRandomise(float* pScore) {
    float ratio = GET_GLOBAL()->IntentionScoreRandomRatio;

    float a = *pScore * ratio;
    float b = *pScore * (1.0 - ratio) * njRandom();

    *pScore = a + b;
}

Uint32 AL_GetIntervalTimer(task* tp, Uint16 TimerKind) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    return bhv->IntentionTimer[TimerKind];
}

Uint32 AL_SetIntervalTimer(task* tp, Uint16 TimerKind, Uint32 timer) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    bhv->IntentionTimer[TimerKind] = timer;
}

void AL_ClearIntention(task* tp) {
    AL_SetIntention(tp, 0);
}

Uint16 AL_GetIntention(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    return bhv->Intention;
}

void AL_SetIntention(task* tp, Uint16 intention) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    bhv->Intention = intention;
    bhv->IntentionMode = 0;
}

int ALBHV_GoToWater(task* tp);
void AL_CalcIntentionScore_Thirsty(task* tp, float* pMaxScore) {
    float score = 0.0f;
    CHAO_GLOBAL* Global = GET_GLOBAL();

    Uint32 trigger = GET_GLOBAL()->IntentionThirstyTrigger;
    Uint32 value = AL_EmotionGetValue(tp, EM_ST_THIRSTY);
    if (*pMaxScore < 1) {
        if (value > trigger) {
            score = AL_CalcScoreTypeA(value, trigger);
            score *= GET_GLOBAL()->IntentionThirstyMaxScore;
            AL_ScoreRandomise(&score);
        }

        if (score > *pMaxScore) {
            AL_SetBehavior(tp, ALBHV_GoToWater);
            AL_ClearIntention(tp);
            *pMaxScore = score;
        }
    }
}

int ALBHV_ApproachToJyouro(task* tp);
void AL_CalcIntentionScore_Jyouro(task* tp, float* pMaxScore) {
    float score = 0.0f;
    CHAO_GLOBAL* Global = GET_GLOBAL();

    if (*pMaxScore < 1) {
        task* pTree;

        if (pTree = AL_IsHitKindWithNum(tp, 1, CI_KIND_AL_THIRSTY)) {
            Uint32 value = AL_EmotionGetValue(tp, EM_ST_THIRSTY);
            Uint32 trigger = GET_GLOBAL()->IntentionJyouroThirstyTrigger;

            score = AL_CalcScoreTypeB(value, trigger);
            score *= GET_GLOBAL()->IntentionJyouroMaxScore;
            AL_ScoreRandomise(&score);
        }

        if (score > *pMaxScore) {
            AL_SetBehavior(tp, ALBHV_ApproachToJyouro);
            ALW_LockOn(tp, pTree);
            AL_ClearIntention(tp);
            *pMaxScore = score;
        }
    }
}

int ALBHV_Rest(task*);
void AL_CalcIntentionScore_Rest(task* tp, float* pMaxScore) {
    float score = 0.0f;
    CHAO_GLOBAL* Global = GET_GLOBAL();

    Uint32 value = AL_EmotionGetValue(tp, EM_ST_TIRE);
    if (*pMaxScore < 1) {
        Uint32 trigger1 = GET_GLOBAL()->IntentionRestTrigger1;
        Uint32 trigger2 = GET_GLOBAL()->IntentionRestTrigger2;
        int emotion = -AL_EmotionGetValue(tp, EM_PER_CALM) + AL_EmotionGetValue(tp, EM_PER_CURIOSITY);
        float f = (200 + emotion) / 400.f;

        Uint32 trigger = trigger1 + (Uint32)(f * (trigger2 - trigger1));
        if (value > trigger) {
            score = AL_CalcScoreTypeA(value, trigger);
            score *= GET_GLOBAL()->IntentionRestMaxScore;
            AL_ScoreRandomise(&score);
        }

        if (score > *pMaxScore) {
            AL_SetBehavior(tp, ALBHV_PostureChangeSit);
            AL_SetNextBehavior(tp, ALBHV_Rest);

            AL_ClearIntention(tp);
            *pMaxScore = score;
        }
    }
}

void AL_CalcIntentionScore_Mayu(task* tp, float* pMaxScore) {
    CHAO_PARAM* pParam = GET_CHAOPARAM(tp);
    CHAO_GLOBAL* Global = GET_GLOBAL();

    if (*pMaxScore < 1) {
        if (pParam->type != TYPE_CHILD) {
            return;
        }

        if (pParam->body.growth < GET_GLOBAL()->GrowthMayuPoint) {
            return;
        }

        pParam->body.growth = GET_GLOBAL()->GrowthMayuPoint;

        if (AL_IsGroundStop(tp)) {
            AL_SetBehavior(tp, ALBHV_Mayu);

            AL_ClearIntention(tp);
            *pMaxScore = 1;
        }
    }
}

void AL_CalcIntentionScore_MakeNest(task* tp, float* pMaxScore) {
    CHAO_PARAM* pParam = GET_CHAOPARAM(tp);
    CHAO_GLOBAL* Global = GET_GLOBAL();

    if (*pMaxScore < 1) {
        switch (pParam->type) {
            case TYPE_CHILD:
            case TYPE_N_CHAOS:
            case TYPE_H_CHAOS:
            case TYPE_D_CHAOS:
                return;
        }

        if (AL_IsHitKindWithNum(tp, 1, CI_KIND_AL_NEST)) {
            return;
        }

        if (AL_EmotionGetValue(tp, EM_ST_BREED) < 10000) {
            return;
        }

        if (AL_GetCurrGardenChaoCount() < 8) {
            if (AL_IsGroundStop(tp)) {
                AL_SetBehavior(tp, ALBHV_PostureChangeSit);
                AL_SetNextBehavior(tp, ALBHV_Nest);

                AL_ClearIntention(tp);
                *pMaxScore = 1;
            }
        } else {
            AL_EmotionSetValue(tp, 11, 0);
        }
    }
}

void AL_CalcIntentionScore_ApproachNest(task* tp, float* pMaxScore) {
    CHAO_PARAM* pParam = GET_CHAOPARAM(tp);
    float score = 0.0f;

    CHAO_GLOBAL* Global = GET_GLOBAL();

    Uint32 trigger = GET_GLOBAL()->IntentionApproachNestTrigger;
    Uint32 value = AL_EmotionGetValue(tp, EM_ST_BREED);

    switch (pParam->type) {
        case TYPE_CHILD:
        case TYPE_N_CHAOS:
        case TYPE_H_CHAOS:
        case TYPE_D_CHAOS:
            return;
    }

    if (AL_IsHitKindWithNum(tp, 1, CI_KIND_AL_NEST)) {
        if (value > trigger) {
            score = 0.88f;
            score *= GET_GLOBAL()->IntentionApproachNestMaxScore;
            AL_ScoreRandomise(&score);
        }

        if (score > *pMaxScore) {
            AL_SetBehavior(tp, ALBHV_ApproachNest);
            AL_ClearIntention(tp);
            *pMaxScore = score;
        }
    }
}

void AL_CalcIntentionScore_Dead(task* tp, float* pMaxScore) {
    CHAO_PARAM* pParam = GET_CHAOPARAM(tp);
    CHAO_GLOBAL* Global = GET_GLOBAL();

    if (*pMaxScore < 1) {
        if (pParam->life > 0) {
            return;
        }

        if (AL_IsGroundStop(tp)) {
            AL_SetBehavior(tp, ALBHV_Dead);

            AL_ClearIntention(tp);
            *pMaxScore = 1;
        }
    }
}

int ALBHV_Move(task* tp);
void AL_DecideIntention(task* tp) {
    CHAO_PARAM* pParam = GET_CHAOPARAM(tp);
    float max_score = GET_GLOBAL()->IntentionMinScore;

    AL_CalcPerceptionAll(tp);
    AL_ClearIntention(tp);

    AL_CalcIntentionScore_Illness(tp, &max_score);
    AL_CalcIntentionScore_Akubi(tp, &max_score);
    AL_CalcIntentionScore_Tron(tp, &max_score);
    AL_CalcIntentionScore_Sleep(tp, &max_score);
    AL_CalcIntentionScore_Hunger(tp, &max_score);
    AL_CalcIntentionScore_Thirsty(tp, &max_score);
    AL_CalcIntentionScore_JoinMusic(tp, &max_score);
    AL_CalcIntentionScore_JoinDance(tp, &max_score);
    AL_CalcIntentionScore_JoinSing(tp, &max_score);
    AL_CalcIntentionScore_JoinOnchi(tp, &max_score);
    AL_CalcIntentionScore_WatchTV(tp, &max_score);
    AL_CalcIntentionScore_ListenRadicase(tp, &max_score);
    AL_CalcIntentionScore_LToy(tp, &max_score);
    AL_CalcIntentionScore_Hima(tp, &max_score);
    AL_CalcIntentionScore_Rest(tp, &max_score);
    AL_CalcIntentionScore_Fear(tp, &max_score);
    AL_CalcIntentionScore_Sorrow(tp, &max_score);
    AL_CalcIntentionScore_Pain(tp, &max_score);
    AL_CalcIntentionScore_Pleasure(tp, &max_score);
    AL_CalcIntentionScore_Mayu(tp, &max_score);

    if (pParam->type != TYPE_CHILD) {
        AL_CalcIntentionScore_Anger(tp, &max_score);
        AL_CalcIntentionScore_Jyouro(tp, &max_score);
        AL_CalcIntentionScore_Player(tp, &max_score);
        AL_CalcIntentionScore_ApproachNest(tp, &max_score);
        AL_CalcIntentionScore_MakeNest(tp, &max_score);
        AL_CalcIntentionScore_Dead(tp, &max_score);
    }

    if (max_score <= GET_GLOBAL()->IntentionMinScore) {
        AL_SetBehavior(tp, ALBHV_Move);
        AL_ClearIntention(tp);
    }
}

void AL_IntervalTimerControl(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    int i;

    for (i = 0; i < 16; i++) {
        if (bhv->IntentionTimer[i] > 0)
            bhv->IntentionTimer[i]--;
    }
}

void AL_IntentionCheckInterrupt(task* tp) {
}

int sub_C50EE3E() {
    return 1;
}

extern Uint8 FuncListNum, FuncNum;
extern BHV_FUNC ALBHV_IllnessFunc[], ALBHV_MusicFunc[], ALBHV_MinimalFunc[], ALBHV_SToyFunc[];
void AL_DecideBehavior(task* tp, eINTENTION intention) {
    switch (intention) {
        case INTENTION_NONE:
            break;
        case INTENTION_JOY:
            rand();

            switch (FuncListNum) {
                case 0:
                    AL_SetBehavior(tp, ALBHV_Think);
                    break;
                case 1:
                    AL_SetBehavior(tp, ALBHV_IllnessFunc[FuncNum]);
                    break;
                case 2:
                    AL_SetBehavior(tp, ALBHV_MusicFunc[FuncNum]);
                    break;
                case 3:
                    AL_SetBehavior(tp, ALBHV_MinimalFunc[FuncNum]);
                    break;
                case 4:
                    AL_SetBehavior(tp, ALBHV_SToyFunc[FuncNum]);
                    break;
            }

            AL_ClearIntention(tp);
            break;
        default:
            AL_SetBehavior(tp, ALBHV_Think);

            AL_ClearIntention(tp);
            break;
    }
}