#include <Chao/Chao.h>

int ALBHV_Sorrow(task*);
int ALBHV_Tameiki(task*);
int ALBHV_Cry(task*);
int ALBHV_MesoMeso(task*);

void AL_CalcIntentionScore_Sorrow(task* tp, float* pMaxScore) {
    // this is the one "score" var in sa2b debug build that doesn't end up somehow
    // in the struct/array debug variable section thing
    float score = 0.f;
    
    Uint32 trigger = GET_GLOBAL()->IntentionSorrowTrigger;
    int emotion = AL_EmotionGetValue(tp, EM_MD_SORROW);

    if(emotion > GET_GLOBAL()->IntentionSorrowTrigger) {
        score = 1 - (((AL_EmotionGetValue(tp, EM_PER_AGRESSIVE) + 100) / 200.f) * (1 - GET_GLOBAL()->IntentionSorrowMinScore));

        if(score > *pMaxScore) {
            AL_SetBehavior(tp, ALBHV_Sorrow);
            
            AL_ClearIntention(tp);
            *pMaxScore = score;
        }
    }
}

int ALBHV_Sorrow(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    int emotion = AL_EmotionGetValue(tp, EM_MD_SORROW);
    
    if(emotion > 120) {
        if(njRandom() < 0.5f) {
            AL_SetBehavior(tp, ALBHV_MesoMeso);
        }
        else {
            AL_SetBehavior(tp, ALBHV_Cry);
        }
    }
    else {
        if(njRandom() < 0.5f) {
            AL_SetBehavior(tp, ALBHV_MesoMeso);
        }
        else {
            AL_SetBehavior(tp, ALBHV_Tameiki);
        }
    }
    
    return BHV_RET_CONTINUE;
}

int ALBHV_Tameiki(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0: {
            int posture = AL_GetMotionPosture(tp);
            switch(posture) {
                case 1:
                case 2:
                case 3:
                    AL_SetMotionLinkStep(tp, 505, 35);
                    break;
                case 0:
                default:
                    AL_SetMotionLinkStep(tp, 502, 30);
                    break;
            }

            AL_FaceChangeEye(tp, 3);
            AL_FaceChangeMouth(tp, 4);

            bhv->Timer = RAND_RANGE(180, 300);
            bhv->Mode++;
        }
        case 1:
            if(bhv->Timer-- <= 0) {
                return BHV_RET_FINISH;
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_MesoMeso(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0: {
            int posture = AL_GetMotionPosture(tp);
            switch(posture) {
                case 1:
                case 2:
                case 3: {
                    float randval = njRandom();
                    if(randval < 0.3f) {
                        AL_SetMotionLinkStep(tp, 549, 35);
                    }
                    else if(randval < 0.6f) {
                        AL_SetMotionLinkStep(tp, 551, 35);
                    }
                    else if(randval < 0.8f) {
                        AL_SetMotionLinkStep(tp, 552, 35);
                    }
                    else {
                        AL_SetMotionLinkStep(tp, 553, 35);
                    }
                } break;
                    
                case 0:
                default: {
                    float randval = njRandom();
                    if(randval < 0.3f) {
                        AL_SetMotionLinkStep(tp, 544, 35);
                    }
                    else if(randval < 0.6f) {
                        AL_SetMotionLinkStep(tp, 546, 35);
                    }
                    else if(randval < 0.8f) {
                        AL_SetMotionLinkStep(tp, 547, 35);
                    }
                    else {
                        AL_SetMotionLinkStep(tp, 548, 35);
                    }
                } break;
            }

            if(njRandom() < 0.5f) {
                AL_FaceChangeEye(tp, 3);
            }
            else {
                AL_FaceChangeEye(tp, 1);
            }

            {
                float randval = njRandom();
                
                if(randval < 0.5f) {
                    AL_FaceChangeMouth(tp, 4);
                }
                else {
                    AL_FaceChangeMouth(tp, 6);
                }
            }

            bhv->Timer = RAND_RANGE(100, 200);
            bhv->Mode++;
        }
        case 1:
            if(bhv->Timer-- <= 0) {
                AL_EmotionAdd(tp, EM_MD_SORROW, -10);

                if(AL_EmotionGetValue(tp, EM_MD_SORROW) < 10) {
                    return BHV_RET_FINISH;
                }
                
                bhv->Mode = 0;
            }
            break;
    }
    
    return BHV_RET_CONTINUE;
}

int ALBHV_Cry(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0: {
            int posture = AL_GetMotionPosture(tp);
            switch(posture) {
                case 1:
                case 2:
                case 3: {
                    float randval = njRandom();
                    if(randval < 0.4f) {
                        AL_SetMotionLinkStep(tp, 528, 35);
                    }
                    else if(randval < 0.8f) {
                        AL_SetMotionLinkStep(tp, 531, 35);
                    }
                    else {
                        AL_SetMotionLinkStep(tp, 533, 35);
                    }
                } break;
                    
                case 0:
                default: {
                    float randval = njRandom();
                    if(randval < 0.5f) {
                        AL_SetMotionLinkStep(tp, 523, 35);
                    }
                    else {
                        AL_SetMotionLinkStep(tp, 525, 35);
                    }
                } break;
            }

            if(njRandom() < 0.5f) {
                AL_FaceChangeEye(tp, 3);
            }
            else {
                AL_FaceChangeEye(tp, 1);
            }

            {
                float randval = njRandom();
                
                if(randval < 0.4f) {
                    AL_FaceChangeMouth(tp, 4);
                }
                else {
                    AL_FaceChangeMouth(tp, 6);
                }
            }

            bhv->Timer = RAND_RANGE(100, 200);
            bhv->Mode++;
        }
        case 1:
            if(AL_IsMotionStop(tp) || bhv->Timer-- <= 0) {
                AL_EmotionAdd(tp, EM_MD_SORROW, -10);

                if(AL_EmotionGetValue(tp, EM_MD_SORROW) < 10) {
                    return BHV_RET_FINISH;
                }
                
                bhv->Mode = 0;
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

    if(!(GET_CHAOWK(tp)->Timer % 180) && njRandom() < 0.3f) {
        sub_8C05B020(24577, 0, 0, 110, &tp->twp->pos);
    }
    
    return BHV_RET_CONTINUE;
}