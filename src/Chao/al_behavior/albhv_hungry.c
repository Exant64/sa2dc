#include <Chao/Chao.h>

int ALBHV_GoToPlayerPleasure(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    taskwk* twk = tp->twp;
    MOVE_WORK* move = tp->Data2;
    Uint16 skill = AL_ParameterGetSkill(tp, SKILL_SWIM);
    
    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_OIKAKE, 10);
            AL_FaceChangeEye(tp, AL_EYE_NUM_NIKO);

            if(AL_FaceGetMouthDefaultNum(tp) != AL_MOUTH_NUM_SHARK) {
                if(njRandom() < 0.5f) {
                    AL_FaceChangeMouth(tp, AL_MOUTH_NUM_NIKO);
                }
                else {
                    AL_FaceChangeMouth(tp, AL_MOUTH_NUM_HOYO);
                }
            }

            bhv->Mode++;
            
            bhv->Timer = 0;
            bhv->SubTimer = 0;
        case 1:
            break;
    }

    if(bhv->Timer++ > 1000) {
        return BHV_RET_FINISH;
    }
    
    if (MOV_Dist2FromPlayer(tp, 0) < 100) {
        return BHV_RET_FINISH;
    }

    MOV_TurnToPlayer2(tp, 288, 0);

    if (move->Flag & 0x400) {
        move->Acc.x = njSin(twk->ang.y) * GET_GLOBAL()->WalkAcc;
        move->Acc.z = njCos(twk->ang.y) * GET_GLOBAL()->WalkAcc;
    }
    
    return BHV_RET_CONTINUE;
}

int ALBHV_NeedFruit(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    
    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_GIVEME_P, 25);
            AL_FaceChangeEye(tp, AL_EYE_NUM_NIKO);

            if(AL_FaceGetMouthDefaultNum(tp) != AL_MOUTH_NUM_SHARK) {
                if(njRandom() < 0.5f) {
                    AL_FaceChangeMouth(tp, AL_MOUTH_NUM_NIKO);
                }
                else {
                    AL_FaceChangeMouth(tp, AL_MOUTH_NUM_HOYO);
                }
            }

            bhv->Mode++;
            bhv->Timer = RAND_RANGE(180, 300);
        case 1:
            if(bhv->Timer-- <= 0) {
                return BHV_RET_FINISH;
            }
            
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_HungryDada(task*);
int ALBHV_Goo(task*);
int ALBHV_GoToEat(task*);

void AL_CalcIntentionScore_Hunger(task* tp, float* pMaxScore) {
    float score = 0;
    Uint32 emotion = AL_EmotionGetValue(tp, EM_ST_HUNGER);
    int interval = AL_GetIntervalTimer(tp, INT_TIMER_GOO);

    if(!interval && njRandom() < 0.7f /* 0.6 on sa2b and sadx */) {
        Uint32 trigger = GET_GLOBAL()->IntentionHungerGooTrigger;
        
        score = AL_CalcScoreTypeA(emotion, trigger);
        score *= GET_GLOBAL()->IntentionHungerGooMaxScore;    

        AL_ScoreRandomise(&score);

        if(score > *pMaxScore) {
            AL_SetBehavior(tp, ALBHV_Goo);

            AL_ClearIntention(tp);
            *pMaxScore = score;
        }

        if(emotion > 9900) {
            score = 1;
            AL_ScoreRandomise(&score);

            if(score > *pMaxScore) {
                AL_SetBehavior(tp, ALBHV_HungryDada);

                AL_ClearIntention(tp);
                *pMaxScore = score;
            }
        }
    }
    else {
        task* pFruitTask = AL_GetFoundFruitTask(tp);

        if(pFruitTask) {
            if(!ALW_IsSheAttentionOtherOne(tp, pFruitTask)) {
                if(pFruitTask->twp->flag & 0x8000) {
                    Uint32 trigger = GET_GLOBAL()->IntentionHungerFruitSearchTrigger;
            
                    score = AL_CalcScoreTypeA(emotion, trigger);
                    score *= GET_GLOBAL()->IntentionHungerFruitSearchMaxScore;    
            
                    AL_ScoreRandomise(&score);
            
                    if(score > *pMaxScore) {
                        AL_SetBehavior(tp, ALBHV_GoToPlayerPleasure);
                        AL_SetNextBehavior(tp, ALBHV_NeedFruit);
            
                        AL_ClearIntention(tp);
                        *pMaxScore = score;
                    }
                }
                else {
                    Uint32 trigger = GET_GLOBAL()->IntentionHungerFruitSearchTrigger;
            
                    score = AL_CalcScoreTypeA(emotion, trigger);
                    score *= GET_GLOBAL()->IntentionHungerFruitSearchMaxScore;    
            
                    AL_ScoreRandomise(&score);
            
                    if(score > *pMaxScore) {
                        AL_SetBehavior(tp, ALBHV_GoToEat);
            
                        AL_ClearIntention(tp);
                        *pMaxScore = score;
                    }
                }
            }
            else {
                
            }
        }
        else {
            al_perception_link* pTreeEntry = AL_GetFoundTree(tp);

            if(pTreeEntry) {
                Uint32 trigger = GET_GLOBAL()->IntentionHungerFruitSearchTrigger;
            
                score = AL_CalcScoreTypeA(emotion, trigger);
                score *= GET_GLOBAL()->IntentionHungerFruitSearchMaxScore;    
        
                AL_ScoreRandomise(&score);
                
                if(score > *pMaxScore) {}
            }
        }
    }
}

int ALBHV_Goo(task* tp) {
    enum {
        MD_GOO_START = 0x0,
        MD_GOO_STAND = 0x1,
        MD_GOO_SIT = 0x2,
        MD_GOO_END = 0x3,
    };

    chaowk* work = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &work->Behavior;

    switch(bhv->Mode) {
        case MD_GOO_START: {
            int posture = AL_GetMotionPosture(tp);
            switch(posture) {
                case 1:
                    AL_SetMotionLink(tp, ALM_GUTA_A);
                    
                    bhv->Timer = 180;
                    bhv->Mode = MD_GOO_SIT;
                    break;
                case 2:
                case 3:
                case 0:
                default:
                    AL_SetMotionLink(tp, ALM_GOO);
                    bhv->Mode = MD_GOO_STAND;
                    break;
            }

            AL_FaceChangeEye(tp, AL_EYE_NUM_TOHOHO);
        } break;

        case MD_GOO_STAND:
            if(AL_IsMotionEnd(tp)) {
                bhv->Mode = MD_GOO_END;
                bhv->Timer = 0;
                bhv->SubTimer = 0;
            }
            break;

        case MD_GOO_SIT:
            if(bhv->Timer <= 0) {
                if(AL_IsMotionEnd(tp)) {
                    bhv->Mode = MD_GOO_END;
                    bhv->Timer = 0;
                    bhv->SubTimer = 0;
                }
            }
            else {
                bhv->Timer--;
            }
            break;

        case MD_GOO_END:
            if(bhv->Timer++ > 60) {
                AL_ClearIntention(tp);

                AL_SetIntervalTimer(
                    tp,
                    INT_TIMER_GOO,
                    GET_GLOBAL()->IntentionHungerGooMinInterval +
                        GET_GLOBAL()->IntentionHungerGooIntervalRange * 
                        0.5f * (njRandom() + (10000 - AL_EmotionGetValue(tp, EM_ST_HUNGER)) / 10000.f)
                );

                return BHV_RET_FINISH;
            }
            
            break;
    }
    
    return BHV_RET_CONTINUE;
}

int ALBHV_HungryDada(task* tp) {
    chaowk* work = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &work->Behavior;

    switch(bhv->Mode) {
        case 0: {
            AL_FaceSetEye(tp, AL_EYE_NUM_KYA, -1);
            AL_FaceSetMouth(tp, AL_MOUTH_NUM_UEEN, -1);
            
            AL_SetMotionLinkStep(tp, ALM_DADA, 20);

            bhv->Timer = RAND_RANGE(240, 440);
            bhv->Mode++;
        } break;

        case 1:
            if(bhv->Timer-- <= 0) {
                AL_SetIntervalTimer(
                    tp,
                    INT_TIMER_GOO,
                    GET_GLOBAL()->IntentionHungerGooMinInterval +
                        GET_GLOBAL()->IntentionHungerGooIntervalRange * 
                        0.5f * (njRandom() + (10000 - AL_EmotionGetValue(tp, EM_ST_HUNGER)) / 10000.f)
                );

                return BHV_RET_FINISH;
            }
            
            break;
    }

    if((bhv->Timer % 8) < 4) {
        NJS_POINT3 pos;
        NJS_VECTOR vec;
        
        AL_SHAPE* pShape = &GET_CHAOWK(tp)->Shape;

        pos = pShape->LeftEyePos;
        pos.y -= 0.2f;

        vec.x = pShape->LeftEyeVec.x * 0.1f;
        vec.y = 0.1f + pShape->LeftEyeVec.y * 0.1f;
        vec.z = pShape->LeftEyeVec.z * 0.1f;
        
        lbl_0C5449A6(&pos, &vec, 0.1f);

        pos = pShape->RightEyePos;
        pos.y -= 0.2f;

        vec.x = pShape->RightEyeVec.x * 0.1f;
        vec.y = 0.1f + pShape->RightEyeVec.y * 0.1f;
        vec.z = pShape->RightEyeVec.z * 0.1f;
        
        lbl_0C5449A6(&pos, &vec, 0.1f);
    }

    if(!(GET_CHAOWK(tp)->Timer % 120 /*60 on sa2b*/) && njRandom() < 0.3f /*0.5 on sa2b*/) {
        sub_8C05B020(24579, 0, 0, 60, &tp->twp->pos);
    }
    
    return BHV_RET_CONTINUE;
}