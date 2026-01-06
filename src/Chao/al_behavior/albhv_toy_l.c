#include <Chao/Chao.h>

#include <Chao/al_landmark.h>
#include <Chao/al_field.h>
#include <Chao/al_intention.h>
#include <Chao/al_misc.h>

#include <Chao/al_toy/alo_tv.h>
#include <Chao/al_toy/alo_radicase.h>

int ALBHV_Think(task*);

int ALBHV_PostureChangeStand(task*);
int ALBHV_Notice(task*);
int ALBHV_GoToLockOn(task*);
int ALBHV_Touch(task*);
int ALBHV_GoToAim(task*);
int ALBHV_TurnToLockOn(task*);
int ALBHV_TurnToAim(task*);

int ALBHV_WatchTV(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0: {
            float randval = njRandom();

            if(randval < 0.3f) {
                AL_SetMotionLink(tp, ALM_HIMA_SIT);
            }
            else if(randval < 0.7f) {
                AL_SetMotionLink(tp, ALM_HIMA_SIT_GUDE);
            }
            else {
                AL_SetMotionLink(tp, ALM_LAUGH_SIT);
            }

            if(AL_PartsIsMinimalFlagOn(tp, eMinimalType_Kuja)) {
                if(njRandom() < 0.3f && AL_GetMotionNum(tp) != ALM_OTAKAI_L && AL_GetMotionNum(tp) != ALM_OTAKAI_R) {
                    if(njRandom() < 0.5f) {
                        AL_SetMotionLink(tp, ALM_OTAKAI_L);
                    }
                    else {
                        AL_SetMotionLink(tp, ALM_OTAKAI_R);
                    }
                }
            }

            if(njRandom() < 0.5f) {
                AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            }
            else {
                AL_FaceReturnDefaultEye(tp);
            }

            if(njRandom() < 0.5f) {
                AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);
            }
            else {
                AL_FaceReturnDefaultMouth(tp);
            }

            bhv->Timer = RAND_RANGE(300, 600);
            bhv->Mode++;
        }
        case 1:
            if(bhv->Timer-- <= 0) {
                bhv->Mode = 0;
            }

            if (!AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_TV)) {
                AL_SetMotionLink(tp, ALM_HAKUSYU_SIT);

                bhv->Timer = RAND_RANGE(120, 210);
                bhv->Mode = 2;
            }
            
            break;

        case 2:
            if(bhv->Timer-- <= 0) {
                AL_SetIntervalTimer(tp, INT_TIMER_LTOY, RAND_RANGE(1800, 3600));

                return BHV_RET_FINISH;
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_ListenRadicase(task *tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0: {
            float randval = njRandom();

            if(randval < 0.3f) {
                AL_SetMotionLink(tp, ALM_HIMA_SIT);
            }
            else if(randval < 0.7f) {
                AL_SetMotionLink(tp, ALM_HIMA_SIT_GUDE);
            }
            else {
                AL_SetMotionLink(tp, ALM_LAUGH_SIT);
            }

            if(AL_PartsIsMinimalFlagOn(tp, eMinimalType_Kuja)) {
                if(njRandom() < 0.3f && AL_GetMotionNum(tp) != ALM_OTAKAI_L && AL_GetMotionNum(tp) != ALM_OTAKAI_R) {
                    if(njRandom() < 0.5f) {
                        AL_SetMotionLink(tp, ALM_OTAKAI_L);
                    }
                    else {
                        AL_SetMotionLink(tp, ALM_OTAKAI_R);
                    }
                }
            }

            if(njRandom() < 0.5f) {
                AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            }
            else {
                AL_FaceReturnDefaultEye(tp);
            }

            if(njRandom() < 0.5f) {
                AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);
            }
            else {
                AL_FaceReturnDefaultMouth(tp);
            }

            bhv->Timer = RAND_RANGE(300, 600);
            bhv->Mode++;
        }
        case 1:
            if(bhv->Timer-- <= 0) {
                bhv->Mode = 0;
            }

            if (!AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_RADICASE)) {
                AL_SetMotionLink(tp, ALM_HAKUSYU_SIT);

                bhv->Timer = RAND_RANGE(120, 210);
                bhv->Mode = 2;
            }
            
            break;

        case 2:
            if(bhv->Timer-- <= 0) {
                AL_SetIntervalTimer(tp, INT_TIMER_LTOY, RAND_RANGE(1800, 3600));

                return BHV_RET_FINISH;
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_JumpFromHorse(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    MOVE_WORK* move = (MOVE_WORK*)tp->Data2;
    taskwk* twk = tp->twp;
    
    switch(bhv->Mode) {
        case 0: {
            NJS_VECTOR velo;
            
            AL_SetMotionSkip(tp, ALM_LOOKUP_JUMP);

            velo.x = njSin(twk->ang.y + NJM_DEG_ANG(90)) * 0.3f;
            velo.y = 0.8f;
            velo.z = njCos(twk->ang.y + NJM_DEG_ANG(90)) * 0.3f;
            MOV_SetVelo(tp, &velo);

            AL_FixPosition(tp);
        } break;
    }

    if(move->Flag & 0x400) {
        return BHV_RET_FINISH;
    }
    
    return BHV_RET_CONTINUE;
}

int ALBHV_RideHorse(task*tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    MOVE_WORK* move = (MOVE_WORK*)tp->Data2;
    taskwk* twk = tp->twp;
    
    switch(bhv->Mode) {
        case 0: {
            NJS_VECTOR velo;
            
            AL_SetMotionLink(tp, ALM_LOOKUP_JUMP);

            if(njRandom() < 0.5f) {
                AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            }
            else {
                AL_FaceReturnDefaultEye(tp);
            }

            if(njRandom() < 0.5f) {
                AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);
            }
            else {
                AL_FaceReturnDefaultMouth(tp);
            }
            
            AL_FixPosition(tp);
            
            velo.x = njSin(twk->ang.y) * 0.08f;
            velo.y = 0.65f;
            velo.z = njCos(twk->ang.y) * 0.08f;
            MOV_SetVelo(tp, &velo);

            bhv->Mode++;
        }
        case 1:
            if(move->Velo.y < 0) {
                AL_SetMotionLinkStep(tp, ALM_HORSE_START, 30);

                GET_CHAOWK(tp)->ChaoFlag &= ~CHAO_FLAG_MOVE_ENABLE;

                bhv->Timer = 29;
                bhv->Mode++;
            }
            break;

        case 2:
            if(--bhv->Timer <= 0) {
                AL_SetMotionSkip(tp, ALM_HORSE_YURAYURA);

                AL_ChildFieldCreateT(tp, CI_KIND_AL_TOUCH, &twk->pos, 3.5f, 10);

                bhv->Timer = 1800;
                bhv->Mode++;
            }
            break;

        case 3:
            if(--bhv->Timer <= 0) {
                AL_SetIntervalTimer(tp, INT_TIMER_LTOY, RAND_RANGE(1800, 3600));

                GET_CHAOWK(tp)->ChaoFlag |= CHAO_FLAG_MOVE_ENABLE;

                AL_SetBehavior(tp, ALBHV_JumpFromHorse);
                return BHV_RET_FINISH;
            }
            break;
    }

    {
        al_entry_work* pHorse = ALW_IsCommunication(tp);
        
        if(pHorse) {
            NJS_POINT3* pPos = &twk->pos;
            NJS_POINT3* pHorsePos = &pHorse->tp->twp->pos;
            float t = 0.1f;

            switch(bhv->Mode) {
                case 3:
                case 2:
                    pPos->x += t * (pHorsePos->x - pPos->x);
                    pPos->y += t * (pHorsePos->y - pPos->y);
                    pPos->z += t * (pHorsePos->z - pPos->z);
                case 1:
                    twk->ang.y = AdjustAngle(twk->ang.y, pHorse->tp->twp->ang.y, 1024);
                    break;
            }
        }
        else {
            AL_SetBehavior(tp, ALBHV_Think);
            return BHV_RET_FINISH;
        }
    }
    
    return BHV_RET_CONTINUE;
}

int ALBHV_GoToTV(task* tp) {
    task* pToy = AL_GetFoundToyTask(tp);
    NJS_POINT3 pos;

    if(pToy && ALO_GetTVWatchPos(&pos)) {
        ALW_LockOn(tp, pToy);
        MOV_SetAimPos(tp, &pos);

        AL_SetBehavior(tp, ALBHV_PostureChangeStand);
        AL_SetNextBehavior(tp, ALBHV_Notice);
        AL_SetNextBehavior(tp, ALBHV_GoToLockOn);
        AL_SetNextBehavior(tp, ALBHV_Touch);
        AL_SetNextBehavior(tp, ALBHV_GoToAim);
        AL_SetNextBehavior(tp, ALBHV_TurnToLockOn);
        AL_SetNextBehavior(tp, ALBHV_WatchTV);

        return BHV_RET_CONTINUE;
    }

    return BHV_RET_FINISH;
}

int ALBHV_GoToRadicase(task* tp) {
    task* pToy = AL_GetFoundToyTask(tp);
    NJS_POINT3 pos;

    if(pToy && ALO_GetRadicaseListenPos(&pos)) {
        ALW_LockOn(tp, pToy);
        MOV_SetAimPos(tp, &pos);

        AL_SetBehavior(tp, ALBHV_PostureChangeStand);
        AL_SetNextBehavior(tp, ALBHV_Notice);
        AL_SetNextBehavior(tp, ALBHV_GoToLockOn);
        AL_SetNextBehavior(tp, ALBHV_Touch);
        AL_SetNextBehavior(tp, ALBHV_GoToAim);
        AL_SetNextBehavior(tp, ALBHV_TurnToLockOn);
        AL_SetNextBehavior(tp, ALBHV_ListenRadicase);

        return BHV_RET_CONTINUE;
    }

    return BHV_RET_FINISH;
}

int ALBHV_GoToBall(task* tp) {
    task* pToy = AL_GetFoundToyTask(tp);
    return BHV_RET_FINISH;
}

int ALBHV_GoToPiano(task* tp) {
    task* pToy = AL_GetFoundToyTask(tp);
    return BHV_RET_FINISH;
}

int ALBHV_GoToOrgan(task* tp) {
    task* pToy = AL_GetFoundToyTask(tp);
    return BHV_RET_FINISH;
}

int ALBHV_GoToHorse(task* tp) {
    task* pToy = AL_GetFoundToyTask(tp);
    NJS_POINT3 pos;

    if(pToy && !ALW_IsSheAttentionOtherOne(tp, pToy)) {
        MOV_SetAimPos(tp, &pToy->twp->pos);
        ALW_CommunicationOn(tp, pToy);

        AL_SetBehavior(tp, ALBHV_PostureChangeStand);
        AL_SetNextBehavior(tp, ALBHV_Notice);
        AL_SetNextBehavior(tp, ALBHV_GoToAim);
        AL_SetNextBehavior(tp, ALBHV_TurnToAim);
        AL_SetNextBehavior(tp, ALBHV_RideHorse);

        return BHV_RET_CONTINUE;
    }

    return BHV_RET_FINISH;
}

int ALBHV_GoToBox(task* tp) {
    task* pToy = AL_GetFoundToyTask(tp);
    NJS_POINT3 pos;

    if(pToy) {
        ALW_LockOn(tp, pToy);

        AL_SetBehavior(tp, ALBHV_PostureChangeStand);
        AL_SetNextBehavior(tp, ALBHV_Notice);
        AL_SetNextBehavior(tp, ALBHV_GoToLockOn);
        AL_SetNextBehavior(tp, ALBHV_Touch);
        // box spawns a Bikkuri field for the behavior,
        // which gets checked in AL_CheckBikkuri
        AL_SetNextBehavior(tp, ALBHV_Think);

        return BHV_RET_CONTINUE;
    }

    return BHV_RET_FINISH;
}

void AL_CalcIntentionScore_ListenRadicase(task* tp, float* pMaxScore) {
    float score = 0;
    
    Uint32 trigger = GET_GLOBAL()->IntentionHimaTrigger;
    Uint32 emotion = AL_EmotionGetValue(tp, EM_ST_TEDIOUS);
    task* pField = AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_RADICASE);

    // no clue
    int a = 0;
    int b = 0;

    if(*pMaxScore < 1 && pField) {
        if(emotion > trigger) {
            if(emotion < 7000) {
                emotion += 3000;
            }

            score = AL_CalcScoreTypeA(emotion, trigger);
            score *= GET_GLOBAL()->IntentionHimaMaxScore;

            AL_ScoreRandomise(&score);
        }

        if(score > *pMaxScore) {
            if(pRadicaseTask) {
                NJS_POINT3 pos;
                
                if(ALO_GetRadicaseListenPos(&pos)) {
                    MOV_SetAimPos(tp, &pos);
                    ALW_LockOn(tp, pRadicaseTask);
                    
                    AL_SetBehavior(tp, ALBHV_PostureChangeStand);
                    AL_SetNextBehavior(tp, ALBHV_Notice);
                    AL_SetNextBehavior(tp, ALBHV_GoToAim);
                    AL_SetNextBehavior(tp, ALBHV_TurnToLockOn);
                    AL_SetNextBehavior(tp, ALBHV_ListenRadicase);
                }
            }

            *pMaxScore = 0.99f;
        }
    }
}

void AL_CalcIntentionScore_WatchTV(task* tp, float* pMaxScore) {
    float score = 0;
    
    Uint32 trigger = GET_GLOBAL()->IntentionHimaTrigger;
    Uint32 emotion = AL_EmotionGetValue(tp, EM_ST_TEDIOUS);
    task* pField = AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_TV);

    // no clue
    int a = 0;
    int b = 0;

    if(*pMaxScore < 1 && pField) {
        if(emotion > trigger) {
            if(emotion < 7000) {
                emotion += 3000;
            }

            score = AL_CalcScoreTypeA(emotion, trigger);
            score *= GET_GLOBAL()->IntentionHimaMaxScore;

            AL_ScoreRandomise(&score);
        }

        if(score > *pMaxScore) {
            if(pTVTask) {
                NJS_POINT3 pos;
                
                if(ALO_GetTVWatchPos(&pos)) {
                    MOV_SetAimPos(tp, &pos);
                    ALW_LockOn(tp, pTVTask);
                    
                    AL_SetBehavior(tp, ALBHV_PostureChangeStand);
                    AL_SetNextBehavior(tp, ALBHV_Notice);
                    AL_SetNextBehavior(tp, ALBHV_GoToAim);
                    AL_SetNextBehavior(tp, ALBHV_TurnToLockOn);
                    AL_SetNextBehavior(tp, ALBHV_WatchTV);
                }
            }

            *pMaxScore = 0.99f;
        }
    }
}

void AL_CalcIntentionScore_LToy(task* tp, float* pMaxScore) {
    float score = 0;
    Uint32 emotion = AL_EmotionGetValue(tp, EM_ST_TEDIOUS);
    Uint32 interval = AL_GetIntervalTimer(tp, INT_TIMER_LTOY);

    if(interval <= 0) {
        task* pToy = AL_GetFoundToyTask(tp);

        if(pToy) {
            Uint32 trigger = GET_GLOBAL()->IntentionHimaTrigger;
            
            score = AL_CalcScoreTypeA(emotion, trigger);
            score *= GET_GLOBAL()->IntentionHimaMaxScore;
            AL_ScoreRandomise(&score);

            if(score > *pMaxScore) {
                int kind = ((al_entry_work*)pToy->EntityData2)->kind;
                
                switch(kind) {
                    case ALW_KIND_TV:
                        AL_SetBehavior(tp, ALBHV_GoToTV);
                        break;
                    case ALW_KIND_RADICASE:
                        AL_SetBehavior(tp, ALBHV_GoToRadicase);
                        break;
                    case ALW_KIND_BALL:
                        AL_SetBehavior(tp, ALBHV_GoToBall);
                        break;
                    case ALW_KIND_PIANO:
                        AL_SetBehavior(tp, ALBHV_GoToPiano);
                        break;
                    case ALW_KIND_ORGAN:
                        AL_SetBehavior(tp, ALBHV_GoToOrgan);
                        break;
                    case ALW_KIND_HORSE: {
                        int landattr = AL_GetCurrLandAttr(&pToy->twp->pos);

                        if (landattr != LMA_GROUND1 && landattr != LMA_GROUND2 && landattr == LMA_GROUND3) {
                            return;
                        }
                        
                        AL_SetBehavior(tp, ALBHV_GoToHorse);
                    } break;
                    case ALW_KIND_BOX:
                        AL_SetBehavior(tp, ALBHV_GoToBox);
                        break;
                }

                AL_EmotionAdd(tp, EM_ST_TEDIOUS, -5000);
                AL_ClearIntention(tp);

                *pMaxScore = score;
            }
        }
    }
}
