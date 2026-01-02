#include <Chao/Chao.h>

int ALBHV_Akanbe(task*);
int ALBHV_SuriSuriPlayer(task*);
int ALBHV_AttackPlayer(task*);

int ALBHV_Greet(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            if(GET_CHAOPARAM(tp)->body.APos > 0.5f) {
                AL_SetMotionLinkStep(tp, ALM_OJIGI, 20);    
                sub_8C05B020(24721, 0, 0, 0, &tp->twp->pos);
            }
            else {
                AL_SetMotionLinkStep(tp, ALM_HELLO, 20);    
            }

            AL_FaceChangeEye(tp, AL_EYE_NUM_NIKO);
            AL_FaceChangeMouth(tp, AL_MOUTH_NUM_NIKO);

            bhv->Timer = 200;
            bhv->Mode++;
        case 1:
            if(bhv->Timer-- <= 0) {
                return BHV_RET_FINISH;
            }
            
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_CalcMeetPlayer(task* tp) {
    AL_KW_MeetPlayer(tp);
    
    return BHV_RET_FINISH;
}

void AL_CalcIntentionScore_Player(task* tp, float* pMaxScore) {
    task* pPlayer = AL_GetFoundPlayerTask(tp);
    int timerPlayer = AL_GetIntervalTimer(tp, INT_TIMER_PLAYER);
    int timerGreet = AL_GetIntervalTimer(tp, INT_TIMER_GREET);

    if(pPlayer) {
        int like = AL_KW_GetPlayerLike(tp);
        int fear = AL_KW_GetPlayerFear(tp);

        if(!timerGreet) {
            if(like >= 0) {
                if(AL_KW_GetPlayerMeet(tp) < 2 || AL_KW_GetPlayerDistance(tp) > 300) {
                    float score = 0.f;
                    int lonely = AL_EmotionGetValue(tp, EM_ST_LONELY);
                    Uint32 trigger = GET_GLOBAL()->IntentionPlayerGreetLonelyTrigger;
    
                    if(lonely > trigger) {
                        score = AL_CalcScoreTypeA(lonely, trigger);
                        score *= GET_GLOBAL()->IntentionPlayerGreetMaxScore;
    
                        AL_ScoreRandomise(&score);
                    }
    
                    if(score > *pMaxScore) {
                        AL_SetBehavior(tp, ALBHV_Greet);
                        AL_SetNextBehavior(tp, ALBHV_CalcMeetPlayer);
    
                        AL_ClearIntention(tp);
                        *pMaxScore = score;
                    }
                }
            }

            if(like < -20 && AL_KW_GetPlayerDistance(tp) > 300) {
                float score = 0.f;
                int lonely = AL_EmotionGetValue(tp, EM_ST_LONELY);
                Uint32 trigger = GET_GLOBAL()->IntentionPlayerGreetLonelyTrigger;

                if(lonely > trigger) {
                    score = AL_CalcScoreTypeA(lonely, trigger);
                    score *= GET_GLOBAL()->IntentionPlayerGreetMaxScore;

                    AL_ScoreRandomise(&score);
                }

                if(score > *pMaxScore) {
                    AL_SetBehavior(tp, ALBHV_Akanbe);
                    AL_SetNextBehavior(tp, ALBHV_CalcMeetPlayer);

                    AL_ClearIntention(tp);
                    *pMaxScore = score;
                }
            }
        }

        if(!timerPlayer) {
            if(like > 60){
                if (AL_KW_GetPlayerDistance(tp) > 100) {
                    float score = 0.f;
                    int lonely = AL_EmotionGetValue(tp, EM_ST_LONELY);
                    Uint32 trigger = GET_GLOBAL()->IntentionPlayerSuriSuriLonelyTrigger;
    
                    if(lonely > trigger) {
                        score = AL_CalcScoreTypeA(lonely, trigger);
                        score *= GET_GLOBAL()->IntentionPlayerSuriSuriMaxScore;
    
                        AL_ScoreRandomise(&score);
                    }
    
                    if(score > *pMaxScore) {
                        AL_SetBehavior(tp, ALBHV_SuriSuriPlayer);
    
                        AL_ClearIntention(tp);
                        *pMaxScore = score;
                    }
                }
            }

            if(like < -60 && AL_IsDark2(tp)) {
                if(AL_KW_GetPlayerDistance(tp) > 100 && AL_EmotionGetValue(tp, EM_PER_AGRESSIVE) > 30) {
                    float score = 0.f;
                    int lonely = AL_EmotionGetValue(tp, EM_ST_LONELY);
                    Uint32 trigger = GET_GLOBAL()->IntentionPlayerSuriSuriLonelyTrigger;
    
                    if(lonely > trigger) {
                        score = AL_CalcScoreTypeA(lonely, trigger);
                        score *= GET_GLOBAL()->IntentionPlayerSuriSuriMaxScore;
    
                        AL_ScoreRandomise(&score);
                    }
    
                    if(score > *pMaxScore) {
                        AL_EmotionAdd(tp, EM_MD_ANGER, 200);
                        
                        AL_SetNextBehavior(tp, ALBHV_AttackPlayer);
    
                        AL_ClearIntention(tp);
                        *pMaxScore = score;
                    }
                }
            }
        }
    }
}

int ALBHV_SuriSuri(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_SURISURI, 20);

            AL_FaceChangeEye(tp, AL_EYE_NUM_TOHOHO);
            AL_FaceChangeMouth(tp, AL_MOUTH_NUM_NIKO);

            bhv->Mode++;
        case 1:
            if (AL_IsMotionEnd(tp)) {
                if (MOV_Dist2FromPlayer(tp, 0) < 25) {
                    AL_EmotionAdd(tp, EM_MD_PLEASURE, 10);
                    AL_SetBehavior(tp, ALBHV_SuriSuri);
                    
                    if (AL_EmotionGetValue(tp, EM_MD_PLEASURE) > 140) {
                        AL_EmotionAdd(tp, EM_ST_LONELY, -6000);
                        
                        return BHV_RET_FINISH;
                    }
                }
                else {
                    AL_SetBehavior(tp, ALBHV_SuriSuriPlayer);
                }
            }
            
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_SuriSuriPlayer(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    taskwk* twk = tp->twp;
    MOVE_WORK* move = tp->Data2;
    Uint16 skill = AL_ParameterGetSkill(tp, 0);
    
    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_SKIP, 10);
            
            AL_FaceChangeEye(tp, AL_EYE_NUM_NIKO);
            AL_FaceChangeMouth(tp, AL_MOUTH_NUM_NIKO);

            bhv->Mode++;
            
            bhv->Timer = 0;
            bhv->SubTimer = 0;

            if(AL_EmotionGetValue(tp, EM_MD_PLEASURE) > 180) {
                AL_EmotionAdd(tp, EM_ST_LONELY, -6000);
                        
                return BHV_RET_FINISH;
            }
        case 1:
            break;
    }

    if(bhv->Timer++ > 1000) {
        return BHV_RET_FINISH;
    }
    
    if (MOV_Dist2FromPlayer(tp, 0) < 25) {
        AL_EmotionAdd(tp, EM_MD_PLEASURE, 10);

        AL_SetBehavior(tp, ALBHV_SuriSuri);
        AL_SetNextBehavior(tp, ALBHV_SuriSuriPlayer);
    }

    MOV_TurnToPlayer2(tp, 288, 0);

    if (move->Flag & 0x400) {
        move->Acc.x = njSin(twk->ang.y) * GET_GLOBAL()->WalkAcc;
        move->Acc.z = njCos(twk->ang.y) * GET_GLOBAL()->WalkAcc;
    }
    
    return BHV_RET_CONTINUE;
}
