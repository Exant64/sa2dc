#include "Chao/Chao.h"
#include "playsound.h"

int ALBHV_Anger(task* tp);
int ALBHV_AttackPlayer(task* tp);
int ALBHV_Akanbe(task* tp);
void AL_CalcIntentionScore_Anger(task* tp, float* pMaxScore) {
    float score = 0.0f;
    Uint32 angerTrigger = GET_GLOBAL()->IntentionAngerTrigger;
    Uint32 value = AL_EmotionGetValue(tp, EM_MD_ANGER);

    if (AL_EmotionGetValue(tp, EM_PER_KINDNESS) > 30)
        return;

    if (AL_IsDark2(tp)) {
        if (value > GET_GLOBAL()->IntentionAngerTrigger) {
            bool trigger = FALSE;
            task* pFoundPlayer = AL_GetFoundPlayerTask(tp);
            if (pFoundPlayer) {
                int playerLike = AL_KW_GetPlayerLike(tp);
                if (playerLike < 0) {
                    if (AL_EmotionGetValue(tp, EM_PER_AGRESSIVE) > 30 && njRandom() < 0.7f) {
                        score = 0.0f;

                        if (value > angerTrigger) {
                            score = AL_CalcMoodScoreTypeA(value, angerTrigger);
                            score *= GET_GLOBAL()->IntentionAngerMaxScore;
                            AL_ScoreRandomise(&score);
                        }

                        trigger = TRUE;

                        if (score > *pMaxScore) {
                            AL_SetBehavior(tp, ALBHV_Anger);
                            AL_SetNextBehavior(tp, ALBHV_AttackPlayer);
                            AL_ClearIntention(tp);
                            *pMaxScore = score;
                        }
                    } else if (AL_EmotionGetValue(tp, EM_PER_AGRESSIVE) > -50) {
                        score = 0.0f;

                        if (value > angerTrigger) {
                            score = AL_CalcMoodScoreTypeA(value, angerTrigger);
                            score *= GET_GLOBAL()->IntentionAngerMaxScore;
                            AL_ScoreRandomise(&score);
                        }

                        trigger = TRUE;

                        if (score > *pMaxScore) {
                            AL_SetBehavior(tp, ALBHV_Akanbe);
                            AL_ClearIntention(tp);
                            *pMaxScore = score;
                        }
                    }
                }
            }
            if (!trigger) {
                score = 1.0 - ((100 - AL_EmotionGetValue(tp, EM_PER_AGRESSIVE)) *
                               (1.0 - GET_GLOBAL()->IntentionAngerMinScore) * 0.005f);
                if (score > *pMaxScore) {
                    AL_SetBehavior(tp, ALBHV_Anger);
                    AL_ClearIntention(tp);
                    *pMaxScore = score;
                }
            }
        }
    }
}

int ALBHV_Anger(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_ANGRY_STAND, 20);

            ++bhv->Mode;

            AL_FaceChangeEye(tp, AL_EYE_NUM_ANGER);

            if (AL_FaceGetMouthDefaultNum(tp) == AL_MOUTH_NUM_SHARK) {
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_GEE);
            } else {
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_MUSU);
            }

            bhv->Timer = (Uint16)(180 + (int)(njRandom() * 301.0f));
        case 1:
            if (bhv->Timer % 150 == 0) {
                if (njRandom() < 0.5) {
                    float randval = njRandom();
                    if (randval < 0.33f)
                        sub_8C05B020(24677, 0, 0, 110, &GET_CHAOWK(tp)->pos);
                    else if (randval < 0.66f)
                        sub_8C05B020(24678, 0, 0, 110, &GET_CHAOWK(tp)->pos);
                    else
                        sub_8C05B020(24680, 0, 0, 110, &GET_CHAOWK(tp)->pos);
                }
            }
            if (!--bhv->Timer)
                return BHV_RET_FINISH;
            break;
    }
    return BHV_RET_CONTINUE;
}
int ALBHV_Genkotu(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_PUNCH, 20);
            ++bhv->Mode;
            AL_FaceChangeEye(tp, AL_EYE_NUM_ANGER);

            if (AL_FaceGetMouthDefaultNum(tp) == AL_MOUTH_NUM_SHARK) {
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_GEE);
            } else {
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_MUSU);
            }

        case 1:
            if (AL_IsMotionEnd(tp))
                return BHV_RET_FINISH;
            break;
    }
    return BHV_RET_CONTINUE;
}
int ALBHV_Kick(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_POKAPOKA, 20);
            ++bhv->Mode;
            AL_FaceChangeEye(tp, AL_EYE_NUM_ANGER);

            if (AL_FaceGetMouthDefaultNum(tp) == AL_MOUTH_NUM_SHARK) {
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_GEE);
            } else {
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_MUSU);
            }

        case 1:
            if (AL_IsMotionEnd(tp))
                return BHV_RET_FINISH;
            break;
    }
    return BHV_RET_CONTINUE;
}
int ALBHV_Punch(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_GENKOTU, 20);
            ++bhv->Mode;
            AL_FaceChangeEye(tp, AL_EYE_NUM_ANGER);

            if (AL_FaceGetMouthDefaultNum(tp) == AL_MOUTH_NUM_SHARK) {
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_GEE);
            } else {
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_MUSU);
            }

        case 1:
            if (AL_IsMotionEnd(tp))
                return BHV_RET_FINISH;
            break;
    }
    return BHV_RET_CONTINUE;
}
int ALBHV_PokaPoka(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_KICK, 20);
            ++bhv->Mode;
            AL_FaceChangeEye(tp, AL_EYE_NUM_ANGER);

            if (AL_FaceGetMouthDefaultNum(tp) == AL_MOUTH_NUM_SHARK) {
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_GEE);
            } else {
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_MUSU);
            }
        case 1:
            if (AL_IsMotionEnd(tp))
                return BHV_RET_FINISH;
            break;
    }
    return BHV_RET_CONTINUE;
}
int ALBHV_Akanbe(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_AKANBE, 20);
            ++bhv->Mode;
            AL_FaceChangeEye(tp, AL_EYE_NUM_KYA);
            AL_FaceChangeMouth(tp, AL_MOUTH_NUM_MUSU);
            sub_8C05B020(0x6069, 0, 0, 110, &GET_CHAOWK(tp)->pos);
        case 1:
            if (AL_IsMotionEnd(tp))
                return BHV_RET_FINISH;
            break;
    }
    return BHV_RET_CONTINUE;
}
int ALBHV_AttackPlayer(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    chaowk* work = GET_CHAOWK(tp);
    MOVE_WORK* v2 = (MOVE_WORK*)tp->Data2;
    //???
    Uint32 skill = AL_ParameterGetSkill(tp, SKILL_SWIM);
    float dist;

    switch (bhv->Mode) {
        case 0: {
            AL_SetMotionLinkStep(tp, ALM_ANGER_WALK, 10);
            AL_FaceChangeEye(tp, AL_EYE_NUM_ANGER);
            if (AL_FaceGetMouthDefaultNum(tp) == AL_MOUTH_NUM_SHARK) {
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_GEE);
            } else {
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_MUSU);
            }
            bhv->Mode++;
            bhv->Timer = 0;
            bhv->SubTimer = 0;
            if (AL_EmotionGetValue(tp, EM_MD_ANGER) < 10) {
                return BHV_RET_FINISH;
            }
        }
        case 1:
            break;
    }

    if (AL_IsEmotionTimerReset(tp)) {}

    if (bhv->Timer++ > 1000) {
        return BHV_RET_FINISH;
    }

    dist = MOV_Dist2FromPlayer(tp, 0);
    if (dist < 25) {
        float randval;
        AL_EmotionAdd(tp, EM_MD_ANGER, -10);

        randval = njRandom();
        if (randval < 0.35)
            AL_SetBehavior(tp, ALBHV_Genkotu);
        else if (randval < 0.7)
            AL_SetBehavior(tp, ALBHV_PokaPoka);
        else if (randval < 0.9)
            AL_SetBehavior(tp, ALBHV_Kick);
        else
            AL_SetBehavior(tp, ALBHV_Punch);
        AL_SetNextBehavior(tp, ALBHV_AttackPlayer);

        if (njRandom() < 0.6) {
            float randval = njRandom();
            if (randval < 0.25f) {
                sub_8C05B020(24687, 0, 0, 110, &GET_CHAOWK(tp)->pos);
            } else if (randval < 0.5f) {
                sub_8C05B020(24688, 0, 0, 110, &GET_CHAOWK(tp)->pos);
            } else if (randval < 0.75f) {
                sub_8C05B020(24689, 0, 0, 110, &GET_CHAOWK(tp)->pos);
            } else {
                sub_8C05B020(24690, 0, 0, 110, &GET_CHAOWK(tp)->pos);
            }
        }
    }

    if (dist > 100) {
        AL_SetBehavior(tp, ALBHV_Anger);
    }

    MOV_TurnToPlayer2(tp, 288, 0);
    if (v2->Flag & 0x400) {
        v2->Acc.x = njSin(work->ang.y) * GET_GLOBAL()->WalkAcc;
        v2->Acc.z = njCos(work->ang.y) * GET_GLOBAL()->WalkAcc;
    }

    return BHV_RET_CONTINUE;
}
