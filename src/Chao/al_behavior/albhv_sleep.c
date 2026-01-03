#include <Chao/Chao.h>

#include <Chao/al_perception.h>
#include <Chao/alo_growtree.h>

#include <playsound.h>

int ALBHV_Akubi(task*);
int ALBHV_Tron(task*);
int ALBHV_Sleep(task*);
int ALBHV_SleepSelect(task*);
int ALBHV_SitSleep(task*);

int ALBHV_GoToLockOn(task*);
int ALBHV_TurnRandom(task*);

void AL_CalcIntentionScore_Akubi(task* tp, float* pMaxScore) {
    float score = 0.f;
    
    Uint32 trigger1 = GET_GLOBAL()->IntentionAkubiTrigger1;
    Uint32 trigger2 = GET_GLOBAL()->IntentionAkubiTrigger2;
    Uint32 trigger3 = GET_GLOBAL()->IntentionAkubiTrigger3;

    int emotion = AL_EmotionGetValue(tp, EM_ST_SLEEPY);
    int interval = AL_GetIntervalTimer(tp, INT_TIMER_AKUBI);

    if(!interval) {
        if(emotion > trigger1) {
            if(emotion < trigger2) {
                score = (float)(emotion - trigger1) / (float)(trigger2 - trigger1);
            }
            else if (emotion < trigger3) {
                score = 1.f - (float)(emotion - trigger2) / (float)(trigger3 - trigger2);
            }
            else {
                score = 0.f;
            }

            score *= GET_GLOBAL()->IntentionAkubiMaxScore;
            AL_ScoreRandomise(&score);
        }
    }

    AL_SetIntervalTimer(
        tp, 
        INT_TIMER_AKUBI, 
        GET_GLOBAL()->IntentionAkubiMinInterval + 
        GET_GLOBAL()->IntentionAkubiIntervalRange *
            (njRandom() + ((10000 - AL_EmotionGetValue(tp, EM_ST_SLEEPY)) / 10000.f)) * 0.5f
        
    );

    if(score > *pMaxScore) {
        AL_SetBehavior(tp, ALBHV_Akubi);
        
        AL_ClearIntention(tp);
        *pMaxScore = score;
    }
}

void AL_CalcIntentionScore_Tron(task* tp, float* pMaxScore) {
    float score = 0.f;
    Uint32 trigger = 500;
    Uint32 emotion = AL_EmotionGetValue(tp, EM_ST_SLEEP_DEPTH);

    if(emotion > trigger) {
        score = AL_CalcScoreTypeA(emotion, trigger);
        score = 0.5f + score * 0.5f;

        AL_ScoreRandomise(&score);
    }

    if(score > *pMaxScore) {
        AL_SetBehavior(tp, ALBHV_Tron);
        
        AL_ClearIntention(tp);
        *pMaxScore = score;
    }
}

void AL_CalcIntentionScore_Sleep(task* tp, float* pMaxScore) {
    float score = 0.f;
    Uint32 trigger = GET_GLOBAL()->IntentionSleepTrigger;
    Uint32 emotion = AL_EmotionGetValue(tp, EM_ST_SLEEPY);
    
    MOVE_WORK* move = tp->Data2;
    taskwk* twk = tp->twp;

    if(move->WaterY <= twk->pos.y) {
        if(emotion > trigger) {
            score = AL_CalcScoreTypeA(emotion, trigger);
            score *= GET_GLOBAL()->IntentionSleepMaxScore;
            AL_ScoreRandomise(&score);
        }

        if(score > *pMaxScore) {
            AL_SetBehavior(tp, ALBHV_SleepSelect);
            
            AL_ClearIntention(tp);
            *pMaxScore = score;
        }
    }
}

Bool AL_CanIWakeUp(task* tp) {
    if(AL_EmotionGetValue(tp, EM_ST_SLEEPY) < 1000 && AL_EmotionGetValue(tp, EM_ST_SLEEP_DEPTH) < GET_GLOBAL()->ParamWakeUpSleepDepth) {
        return TRUE;
    }

    return FALSE;
}

void AL_SleepControl(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    if(AL_IsEmotionTimerReset(tp)) {
        switch(bhv->SubMode) {
            case 0:
                AL_EmotionAdd(tp, EM_ST_SLEEP_DEPTH, GET_GLOBAL()->ParamAddSleepDepth);
                
                if (AL_EmotionGetValue(tp, EM_ST_SLEEP_DEPTH) >= 10000) {
                    bhv->SubMode++;
                }
                
                break;
            
            case 1:
                AL_EmotionAdd(tp, EM_ST_SLEEPY, -GET_GLOBAL()->ParamSubSleepy);
                AL_EmotionAdd(tp, EM_ST_TIRE, -GET_GLOBAL()->ParamSubTireSleep * (AL_EmotionGetValue(tp, EM_PER_REGAIN) + 300) / 300);
                
                if (AL_EmotionGetValue(tp, EM_ST_SLEEPY) <= 0) {
                    bhv->SubMode++;
                }
                
                break;
            
            case 2:
                AL_EmotionAdd(tp, EM_ST_SLEEPY, -GET_GLOBAL()->ParamSubSleepy);
                AL_EmotionAdd(tp, EM_ST_SLEEP_DEPTH, -GET_GLOBAL()->ParamSubSleepDepth);
                break;
        }
    }
}

int ALBHV_SleepSelect(task* tp) {
    al_perception_link* pTreeEntry;
    
    AL_SetBehavior(tp, ALBHV_Sleep);

    pTreeEntry = AL_GetFoundTree(tp);
    if(pTreeEntry) {
        float t = AL_EmotionGetValue(tp, EM_PER_AGRESSIVE) * (3.f / 1000.f) + 0.5f;
        if(t < njRandom()) {
            if(pTreeEntry->dist < 30.f) {
                if(pTreeEntry->pEntry) {
                    task* pTree = pTreeEntry->pEntry->tp;
                    if(pTree) {
                        TREE_WORK* pTreeWork = ((TREE_WORK*)pTree->twp);
                        int state = pTreeWork->state;
                        
                        switch(state) {
                            case TREE_ST_ADULT:
                            case TREE_ST_LOCAL:
                                ALW_LockOn(tp, pTreeEntry->pEntry->tp);

                                AL_SetBehavior(tp, ALBHV_GoToLockOn);
                                AL_SetNextBehavior(tp, ALBHV_TurnRandom);
                                AL_SetNextBehavior(tp, ALBHV_Sleep);
                                
                                break;
                        }
                    }
                }
            }
        }
    }
}

enum {
    MD_SLEEP_START = 0x0,
    MD_SLEEP_BATAN = 0x1,
    MD_SLEEP_DECIDE_PAUSE = 0x2,
    MD_SLEEP_SLEEP = 0x3,
    MD_SLEEP_UTUBUSE = 0x4,
    MD_SLEEP_UTUBUSE_END = 0x5,
    MD_SLEEP_NEGAERI_L = 0x6,
    MD_SLEEP_NEGAERI_L_END = 0x7,
    MD_SLEEP_NEGAERI_R = 0x8,
    MD_SLEEP_NEGAERI_R_END = 0x9,
    MD_SLEEP_PORIPORI_L = 0xA,
    MD_SLEEP_PORIPORI_L_END = 0xB,
    MD_SLEEP_PORIPORI_R = 0xC,
    MD_SLEEP_PORIPORI_R_END = 0xD,
    MD_SLEEP_SHAKE_HEAD = 0xE,
    MD_SLEEP_WAKE_UP = 0xF,
    MD_SLEEP_END = 0x10,
};

enum {
    FREE_SLEEP_NONE = 0x0,
    FREE_SLEEP_DAINOJI = 0x1,
    FREE_SLEEP_NORMAL = 0x2,
};

int ALBHV_Sleep(task* tp) {
    chaowk* work = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &work->Behavior;

    switch(bhv->Mode) {
        case MD_SLEEP_START: {
            int posture = AL_GetMotionPosture(tp);
            switch(posture) {
                case 0:
                    AL_SetMotionLink(tp, ALM_BATAN);
                    bhv->Mode = MD_SLEEP_BATAN;
                    break;
                
                case 1:
                    if(AL_EmotionGetValue(tp, EM_ST_TIRE) > 7000 && njRandom() < 0.5f) {
                        AL_SetBehavior(tp, ALBHV_SitSleep);
                    }
                    else {
                        bhv->Mode = MD_SLEEP_DECIDE_PAUSE;
                    }
                    break;
                
                case 2:
                case 3:
                default:
                    bhv->Mode = MD_SLEEP_DECIDE_PAUSE;
            }

            AL_BehaviorSetFreeWork(tp, FREE_SLEEP_NONE);
            
        } break;

        case MD_SLEEP_BATAN:
            if(AL_IsMotionEnd(tp)) {
                bhv->Mode = MD_SLEEP_DECIDE_PAUSE;
            }
            
            break;

        case MD_SLEEP_DECIDE_PAUSE: {
            int emotion = AL_EmotionGetValue(tp, EM_PER_AGRESSIVE);
            if(emotion < 0) emotion = 0;

            switch(AL_BehaviorGetFreeWork(tp)) {
                case FREE_SLEEP_NONE: {
                    if(AL_EmotionGetValue(tp, EM_PER_AGRESSIVE) > njRandom() * 100) {
                        AL_SetMotionLinkStep(tp, ALM_DAINOJI, 40);
                        AL_BehaviorSetFreeWork(tp, FREE_SLEEP_DAINOJI);
                    }
                    else {
                        AL_SetMotionLinkStep(tp, ALM_SLEEP, 40);
                        AL_BehaviorSetFreeWork(tp, FREE_SLEEP_NORMAL);
                    }
                } break;

                case FREE_SLEEP_DAINOJI:
                    AL_SetMotionLink(tp, ALM_DAINOJI_NOBI);
                    break;
                
                case FREE_SLEEP_NORMAL:
                    AL_SetMotionLink(tp, ALM_SLEEP_LOOP);
                    break;
            }

            bhv->Timer = 1000;
            AL_FaceChangeEye(tp, AL_EYE_NUM_SUYASUYA);
            bhv->Mode = MD_SLEEP_SLEEP;
        }
        case MD_SLEEP_SLEEP:
            switch(AL_BehaviorGetFreeWork(tp)) {
                case FREE_SLEEP_DAINOJI:
                    if(!(bhv->Timer % 120) && njRandom() < 0.5f) {
                        if(njRandom() < 0.8f) {
                            sub_8C05B020(24584, 0, 0, 110, &tp->twp->pos);
                        }
                        else {
                            sub_8C05B020(24585, 0, 0, 110, &tp->twp->pos);
                        }
                    }
                    break;
                
                case FREE_SLEEP_NORMAL:
                    if(!(bhv->Timer % 120) && njRandom() < 0.4f) {
                        if(njRandom() < 0.8f) {
                            sub_8C05B020(24587, 0, 0, 110, &tp->twp->pos);
                        }
                        else {
                            sub_8C05B020(24586, 0, 0, 110, &tp->twp->pos);
                        }
                    }
                    break;
            }

            if(bhv->Timer-- <= 0) {
                float randval = njRandom();

                if(randval < 0.3f) {
                    bhv->Timer = 1000;
                    AL_SetMotionLink(tp, ALM_UTUBUSE_NEGAERI);
                    bhv->Mode = MD_SLEEP_UTUBUSE;
                }
                else if(randval < 0.6f) {
                    if(njRandom() < 0.5f) {
                        bhv->Timer = 1000;
                        AL_SetMotionLink(tp, ALM_NEGAERI_L);
                        bhv->Mode = MD_SLEEP_NEGAERI_L;
                    }
                    else {
                        bhv->Timer = 1000;
                        AL_SetMotionLink(tp, ALM_NEGAERI_R);
                        bhv->Mode = MD_SLEEP_NEGAERI_R;
                    }
                }
                else if(randval < 0.9f) {
                    if(njRandom() < 0.5f) {
                        bhv->Timer = 2 + (Uint32)(njRandom() * 4);
                        AL_SetMotionLink(tp, ALM_NERU_PORI_L);
                        bhv->Mode = MD_SLEEP_PORIPORI_L;
                    }
                    else {
                        bhv->Timer = 2 + (Uint32)(njRandom() * 4);
                        AL_SetMotionLink(tp, ALM_NERU_PORI_R);
                        bhv->Mode = MD_SLEEP_PORIPORI_R;
                    }
                }
                else {
                    bhv->Timer = 1000;
                }

                if(njRandom() < 0.25f) {
                    if(njRandom() < 0.5f) {
                        sub_8C05B020(24591, 0, 0, 110, &tp->twp->pos);
                    }
                    else {
                        sub_8C05B020(24593, 0, 0, 110, &tp->twp->pos);
                    }
                }
                
            }

            if(AL_CanIWakeUp(tp)) {
                bhv->Mode = MD_SLEEP_WAKE_UP;
            }
            
            break;

        case MD_SLEEP_UTUBUSE:
            if(bhv->Timer-- <= 0 || AL_CanIWakeUp(tp)) {
                AL_SetMotionLink(tp, ALM_UTUBUSE_END);
                bhv->Mode = MD_SLEEP_UTUBUSE_END;
            }
            break;

        case MD_SLEEP_UTUBUSE_END:
            if(AL_IsMotionEnd(tp)) {
                bhv->Mode = MD_SLEEP_DECIDE_PAUSE;
            }
            
            break;

        case MD_SLEEP_NEGAERI_L:
            if(bhv->Timer-- <= 0 || AL_CanIWakeUp(tp)) {
                AL_SetMotionLink(tp, ALM_NEGAERI_L_RETURN);
                bhv->Mode = MD_SLEEP_NEGAERI_L_END;
            }
            break;

        case MD_SLEEP_NEGAERI_L_END:
            if(AL_IsMotionEnd(tp)) {
                bhv->Mode = MD_SLEEP_DECIDE_PAUSE;
            }
            
            break;

         case MD_SLEEP_NEGAERI_R:
            if(bhv->Timer-- <= 0 || AL_CanIWakeUp(tp)) {
                AL_SetMotionLink(tp, ALM_NEGAERI_R_RETURN);
                bhv->Mode = MD_SLEEP_NEGAERI_R_END;
            }
            break;

        case MD_SLEEP_NEGAERI_R_END:
            if(AL_IsMotionEnd(tp)) {
                bhv->Mode = MD_SLEEP_DECIDE_PAUSE;
            }
            
            break;

        case MD_SLEEP_PORIPORI_L:
            if (AL_IsMotionEnd(tp)) {
                if(bhv->Timer-- <= 0) {
                    AL_SetMotionLink(tp, ALM_NERU_PORI_L_END);
                    bhv->Mode = MD_SLEEP_PORIPORI_L_END;
                }
            }

            if(AL_CanIWakeUp(tp)) {
                AL_SetMotionLink(tp, ALM_NERU_PORI_L_END);
                bhv->Mode = MD_SLEEP_PORIPORI_L_END;
            }
            break;

        case MD_SLEEP_PORIPORI_L_END:
            if(AL_IsMotionEnd(tp)) {
                bhv->Mode = MD_SLEEP_DECIDE_PAUSE;
            }
            break;

        case MD_SLEEP_PORIPORI_R:
            if (AL_IsMotionEnd(tp)) {
                if(bhv->Timer-- <= 0) {
                    AL_SetMotionLink(tp, ALM_NERU_PORI_R_END);
                    bhv->Mode = MD_SLEEP_PORIPORI_R_END;
                }
            }

            if(AL_CanIWakeUp(tp)) {
                AL_SetMotionLink(tp, ALM_NERU_PORI_R_END);
                bhv->Mode = MD_SLEEP_PORIPORI_R_END;
            }
            break;

        case MD_SLEEP_PORIPORI_R_END:
            if(AL_IsMotionEnd(tp)) {
                bhv->Mode = MD_SLEEP_DECIDE_PAUSE;
            }
            break;

        case MD_SLEEP_WAKE_UP:
            AL_SetMotionLinkStep(tp, ALM_NOBI_SIT, 40);
            
            bhv->Mode = MD_SLEEP_END;
            
            AL_FaceChangeEye(tp, AL_EYE_NUM_KYA);
            AL_FaceChangeMouth(tp, AL_MOUTH_NUM_NIKO);
            break;

        case MD_SLEEP_END:
            if(AL_IsMotionStop(tp)) {
                AL_ClearIntention(tp);

                return BHV_RET_FINISH;
            }
            break;

        // ?
        case MD_SLEEP_SHAKE_HEAD:
            break;
    }

    AL_SleepControl(tp);
    
    return BHV_RET_CONTINUE;
}

int ALBHV_SitSleep(task* tp) {
    enum {
        MD_SITSLEEP_START = 0x0,
        MD_SITSLEEP_SLEEP = 0x1,
        MD_SITSLEEP_UTOUTO = 0x2, // "nodding off"
        MD_SITSLEEP_END = 0x3,
    };

    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case MD_SITSLEEP_START:
            if(AL_EmotionGetValue(tp, EM_ST_TIRE) / 10000.f > njRandom()) {
                AL_SetMotionLink(tp, ALM_SIT_SLEEP);
                
                bhv->Mode = MD_SITSLEEP_SLEEP;
            }
            else {
                AL_SetMotionLink(tp, ALM_UTOUTO);
                
                bhv->Mode = MD_SITSLEEP_UTOUTO;
                bhv->Timer = 3 + njRandom() * 10;
            }

            AL_FaceChangeEye(tp, AL_EYE_NUM_SUYASUYA);
            
            break;
        
        case MD_SITSLEEP_SLEEP:
            if (AL_CanIWakeUp(tp)) {
                AL_SetMotionLink(tp, ALM_HIMA_SIT);
                
                return BHV_RET_FINISH;
            }
            break;
    
        case MD_SITSLEEP_UTOUTO:
            if (AL_IsMotionEnd(tp)) {
                if(bhv->Timer-- <= 0) {
                    AL_SetMotionLink(tp, ALM_UTOUTO_KOKKURI);
                    bhv->Timer = 3 + njRandom() * 10;
                }        
            }   
            
            if (AL_CanIWakeUp(tp)) {
                AL_SetMotionLink(tp, ALM_HIMA_SIT);
                AL_ClearIntention(tp);
            
                return BHV_RET_FINISH;
            }

            break;
    }

    AL_SleepControl(tp);

    return BHV_RET_CONTINUE;
}

int ALBHV_Akubi(task* tp) {
    enum {
        MD_AKUBI_START = 0x0,
        MD_AKUBI_STAND = 0x1,
        MD_AKUBI_STAND_END = 0x2,
        MD_NOBI_STAND = 0x3,
        MD_AKUBI_SIT = 0x4,
        MD_AKUBI_SIT_END = 0x5,
        MD_NOBI_SIT = 0x6,
        MD_MEKOSURI_SIT = 0x7,
        MD_AKUBI_END = 0x8,
    };

    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case MD_AKUBI_START: {
            int posture = AL_GetMotionPosture(tp);
            switch(posture) {
                case 0:
                    if(njRandom() < 0.5f) {
                        bhv->Timer = 3 + njRandom() * 6;

                        AL_SetMotionLink(tp, ALM_AKUBI);
                        
                        bhv->Mode = MD_AKUBI_STAND;
                        sub_8C05B020(24581, 0, 0, 110, &tp->twp->pos);

                        AL_FaceChangeEye(tp, AL_EYE_NUM_TOHOHO);
                        AL_FaceChangeMouth(tp, AL_MOUTH_NUM_HOYO);
                    }
                    else {
                        AL_SetMotionLink(tp, ALM_NOBI_STAND);
                        sub_8C05B020(24590, 0, 0, 110, &tp->twp->pos);

                        bhv->Mode = MD_NOBI_STAND;

                        AL_FaceChangeEye(tp, AL_EYE_NUM_KYA);
                        AL_FaceChangeMouth(tp, AL_MOUTH_NUM_HOYO);
                    }
                    
                    break;
                case 1:
                default: {
                    float randval = njRandom();

                    if(randval < 0.3f) {
                        bhv->Timer = 2 + njRandom() * 6;

                        AL_SetMotionLink(tp, ALM_SIT_AKUBI);
                        bhv->Mode = MD_AKUBI_SIT;
                        
                        sub_8C05B020(24581, 0, 0, 110, &tp->twp->pos);
                        
                        AL_FaceChangeEye(tp, AL_EYE_NUM_TOHOHO);
                        AL_FaceChangeMouth(tp, AL_MOUTH_NUM_HOYO);
                    }
                    else if(randval < 0.6f) {
                        AL_SetMotionLink(tp, ALM_NOBI_SIT);
                        bhv->Mode = MD_NOBI_SIT;
                        
                        sub_8C05B020(24590, 0, 0, 110, &tp->twp->pos);
                        
                        AL_FaceChangeEye(tp, AL_EYE_NUM_KYA);
                        AL_FaceChangeMouth(tp, AL_MOUTH_NUM_HOYO);
                    }
                    else {
                        AL_SetMotionLink(tp, ALM_MEKOSURI);
                        bhv->Mode = MD_MEKOSURI_SIT;
                        
                        AL_FaceChangeEye(tp, AL_EYE_NUM_TOHOHO);
                        AL_FaceChangeMouth(tp, AL_MOUTH_NUM_KOIKE);
                    }
                } break;
            }
        } break;

        case MD_AKUBI_STAND:
            if (AL_IsMotionEnd(tp)) {
                if(bhv->Timer-- <= 0) {
                    AL_SetMotionLink(tp, ALM_AKUBI_END);
                    bhv->Mode = MD_AKUBI_STAND_END;
                }    
            }
            break;

        case MD_AKUBI_STAND_END:
            if(AL_IsMotionEnd(tp)) {
                bhv->Mode = MD_AKUBI_END;
            }
            break;

        case MD_NOBI_STAND:
            if(AL_IsMotionEnd(tp)) {
                bhv->Mode = MD_AKUBI_END;
            }
            break;

        case MD_AKUBI_SIT:
            if (AL_IsMotionEnd(tp)) {
                if(bhv->Timer-- <= 0) {
                    AL_SetMotionLink(tp, ALM_SIT_AKUBI_END);
                    bhv->Mode = MD_AKUBI_SIT_END;
                }    
            }
            break;

        case MD_AKUBI_SIT_END:
            if(AL_IsMotionEnd(tp)) {
                bhv->Mode = MD_AKUBI_END;
            }
            break;
        
        case MD_NOBI_SIT:
            if(AL_IsMotionEnd(tp)) {
                bhv->Mode = MD_AKUBI_END;
            }
            break;

        case MD_MEKOSURI_SIT:
            if(AL_IsMotionEnd(tp)) {
                bhv->Mode = MD_AKUBI_END;
            }
            break;

        case MD_AKUBI_END:
            AL_ClearIntention(tp);
            return BHV_RET_FINISH;
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Tron(task* tp) {    
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch (bhv->Mode) {
        case 0: {
            int posture = AL_GetMotionPosture(tp);
            switch(posture) {
                case 1:
                    AL_SetMotionLink(tp, ALM_HIMA_SIT_GUDE);
                    break;
                case 2:
                    AL_SetMotionLink(tp, ALM_SLEEP);
                    break;
                case 0:
                default:
                    AL_SetMotionLink(tp, ALM_STAND);
                    break;
            }

            AL_FaceSetEye(tp, AL_EYE_NUM_TRON, -1);
            
            if(njRandom() < 0.3f) {
                AL_FaceSetMouth(tp, AL_MOUTH_NUM_KOIKE, -1);
            }
            
            AL_IconSet(tp, MD_ICON_MOJYA, 360);
            
            bhv->Mode++;
        }
        case 1:
            if(bhv->Timer++ > 400) {
                AL_IconSet(tp, MD_ICON_MOJYA, 360);
                bhv->Timer = 0;
            }

            AL_EmotionAdd(tp, EM_ST_SLEEP_DEPTH, -2);

            if(!AL_EmotionGetValue(tp, EM_ST_SLEEP_DEPTH)) {
                return BHV_RET_FINISH;
            }
            
            break;
    }
    
    return BHV_RET_CONTINUE;
}