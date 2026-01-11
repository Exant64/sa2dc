#include <Chao/Chao.h>

#include <Chao/al_misc.h>
#include <Chao/al_field.h>

int ALBHV_FuriFuriDance(task*);
int ALBHV_KuruKuruDance(task*);
int ALBHV_BoxDance(task*);
int ALBHV_GoGoDance(task*);
int ALBHV_RadioDance(task*);
int ALBHV_WatchDance(task*);
int ALBHV_WatchDanceStand(task*);

extern BHV_FUNC ALBHV_DanceFunc[];

void AL_CalcIntentionScore_JoinDance(task* tp, float* pMaxScore) {
    float score = 0.f;
    Uint32 trigger = GET_GLOBAL()->IntentionHimaTrigger;
    Uint32 emotion = AL_EmotionGetValue(tp, EM_ST_TEDIOUS);
    task* pField = AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_DANCE);

    // leftover from copypasting existing func im guessing
    int InstList[8];
    int count = 0;

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
            int kind = -1;

            if(pField->ptp) {
                BHV_FUNC func = AL_GetBehavior(pField->ptp);

                if(func == ALBHV_FuriFuriDance) {
                    if(AL_KW_IsDanceFlagOn(tp, AL_DANCE_FURIRFURI)) {
                        kind = AL_DANCE_FURIRFURI;
                    }
                }
                else if(func == ALBHV_KuruKuruDance) {
                    if(AL_KW_IsDanceFlagOn(tp, AL_DANCE_KURUKURU)) {
                        kind = AL_DANCE_KURUKURU;
                    }
                }
                else if(func == ALBHV_BoxDance) {
                    if(AL_KW_IsDanceFlagOn(tp, AL_DANCE_BOX)) {
                        kind = AL_DANCE_BOX;
                    }
                }
                else if(func == ALBHV_GoGoDance) {
                    if(AL_KW_IsDanceFlagOn(tp, AL_DANCE_GOGO)) {
                        kind = AL_DANCE_GOGO;
                    }
                }
                else if(func == ALBHV_RadioDance) {
                    if(AL_KW_IsDanceFlagOn(tp, AL_DANCE_RADIO)) {
                        kind = AL_DANCE_RADIO;
                    }
                }
            }

            // should've been >=, bug (chao can't join furifuri properly this way)
            if (kind > 0) {
                MOV_SetAimPos(tp, &pField->twp->pos);

                AL_SetBehavior(tp, ALBHV_TurnToAim);
                AL_SetNextBehavior(tp, ALBHV_DanceFunc[kind]);

                AL_EmotionAdd(tp, EM_ST_TEDIOUS, -4000);
                AL_EmotionAdd(tp, EM_ST_LONELY, -4000);

                AL_ClearIntention(tp);

                if(pField->ptp) {
                    AL_EmotionAdd(pField->ptp, EM_ST_LONELY, -3000);
                }
            }
            else {
                if(pField->ptp) {
                    MOV_SetAimPos(tp, &pField->ptp->twp->pos);
                }

                AL_SetBehavior(tp, ALBHV_TurnToAim);
                AL_SetNextBehavior(tp, ALBHV_PostureChangeSit);

                if(njRandom() < 0.5f) {
                    AL_SetNextBehavior(tp, ALBHV_WatchDance);
                }
                else {
                    AL_SetNextBehavior(tp, ALBHV_WatchDanceStand);
                }
            }

            *pMaxScore = 0.99f;
        }
    }
}

Bool AL_DecideBehaviorDance(task* tp) {
    float score = 0;
    taskwk* twk = tp->twp;

    int InstList[8];
    int count = 0;

    int kind;

    CCL_ClearSearch(tp);

    if(AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_DANCE)) {
        return FALSE;
    }
    else {
        int i;

        for(i = 0; i < NB_AL_DANCE; ++i) {
            if(AL_KW_IsDanceFlagOn(tp, i)) {
                InstList[count] = i;
                count++;
            }
        }

        if(count > 0) {
            int kind = InstList[(int)(njRandom() * (count - 0.00001f))];
            NJS_POINT3 center;
            int ang = twk->ang.y;

            center.x = twk->pos.x + 8 * njSin(ang);
            center.y = twk->pos.y;
            center.z = twk->pos.z + 8 * njCos(ang);

            AL_SetBehavior(tp, ALBHV_DanceFunc[kind]);

            AL_ChildFieldCreateT(
                tp,
                CI_KIND_AL_DANCE,
                &center,
                20,
                RAND_RANGE(1800, 4200)
            );

            return TRUE;
        }
    }

    return FALSE;
}

int ALBHV_FuriFuriDance(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0: {
            float randval;

            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);

            randval = njRandom();
            if(randval < 0.25f) {
                AL_SetMotionLink(tp, ALM_HAND_DANCE_UP);
            }
            else if(randval < 0.5f) {
                AL_SetMotionLink(tp, ALM_HAND_DANCE_DOWN);
            }
            else if(randval < 0.75f) {
                AL_SetMotionLink(tp, ALM_HAND_DANCE_RIGHT);
            }
            else {
                AL_SetMotionLink(tp, ALM_HAND_DANCE_LEFT);
            }

            bhv->Mode++;
        }
        case 1:
            if(AL_IsMotionStop(tp)) {
                bhv->Mode = 0;
            }
            break;
    }

    return !AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_DANCE);
}

int ALBHV_KuruKuruDance(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0: {
            float randval;

            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);

            randval = njRandom();
            if(randval < 0.35f) {
                AL_SetMotionLink(tp, ALM_ROUND_DANCE_LEFT);
            }
            else if(randval < 0.7f) {
                AL_SetMotionLink(tp, ALM_ROUND_DANCE_RIHGT);
            }
            else if(randval < 0.85f) {
                AL_SetMotionLink(tp, ALM_ROUND_DANCE_LEFT_TURN);
            }
            else {
                AL_SetMotionLink(tp, ALM_ROUND_DANCE_RIHGT_TURN);
            }

            bhv->Mode++;
        }
        case 1:
            if(AL_IsMotionStop(tp)) {
                bhv->Mode = 0;
            }
            break;
    }

    return !AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_DANCE);
}

int ALBHV_BoxDance(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0: {
            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_SetMotionLink(tp, ALM_BOX_DANCE);

            bhv->Mode++;
        }
        case 1:
            break;
    }

    return !AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_DANCE);
}

int ALBHV_GoGoDance(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0: {
            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_SetMotionLink(tp, ALM_GOGO_DANCE);

            bhv->Mode++;
        }
        case 1:
            if(AL_IsMotionEnd(tp)) {
                if(njRandom() < 0.1f) {
                    AL_SetMotionLink(tp, ALM_GOGO_DANCE2);
                }
            }
            break;
    }

    return !AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_DANCE);
}

int ALBHV_RadioDance(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0: {
            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_SetMotionLink(tp, ALM_TAISOU);

            bhv->Mode++;
            bhv->Timer = 0;
            bhv->SubTimer = 0;
        }
        case 1:
            if(AL_IsMotionEnd(tp)) {
                if(njRandom() < 0.3f) {
                    AL_SetMotionLink(tp, ALM_TAISOU_YOKO2KOKI);

                    bhv->Mode++;
                    bhv->Timer = 0;
                    bhv->SubTimer = 0;
                }
            }
            break;
        case 2:
            if(AL_IsMotionEnd(tp)) {
                bhv->SubTimer++;
                
                if(bhv->SubTimer >= 2) {
                    AL_SetMotionLink(tp, ALM_TAISOU_KOKI_L);

                    bhv->Mode++;
                    bhv->Timer = 0;
                    bhv->SubTimer = 0;
                }
                else {
                    AL_SetMotionLink(tp, ALM_TAISOU_KOKI_R);
                }
            }
            break;
        case 3:
            if(AL_IsMotionEnd(tp)) {
                bhv->SubTimer++;

                if(bhv->SubTimer >= 1) {
                    AL_SetMotionLink(tp, ALM_TAISOU_MAE);

                    bhv->Mode++;
                    bhv->Timer = 0;
                    bhv->SubTimer = 0;
                }
                else {
                    AL_SetMotionLink(tp, ALM_TAISOU_KOKI_L);
                }
            }
            break;
        case 4:
            if(AL_IsMotionEnd(tp)) {
                bhv->SubTimer++;

                if(bhv->SubTimer >= 1) {
                    AL_SetMotionLink(tp, ALM_TAISOU);

                    bhv->Mode = 1;
                    bhv->Timer = 0;
                    bhv->SubTimer = 0;
                }
                else {
                    AL_SetMotionLink(tp, ALM_TAISOU_MAE);
                }
            }
            break;
    }

    return !AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_DANCE);
}

int ALBHV_WatchDance(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0: {
            if(njRandom() < 0.5f) {
                AL_SetMotionLink(tp, ALM_MUSIC_SIT);
            }
            else {
                AL_SetMotionLink(tp, ALM_HIMA_SIT_GUDE);
            }

            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);

            bhv->Timer = RAND_RANGE(300, 600);
            bhv->Mode++;
        }
        case 1:
            if(bhv->Timer-- <= 0) {
                bhv->Mode = 0;
            }
            
            if(!AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_DANCE)) {
                AL_SetMotionLink(tp, ALM_HAKUSYU_SIT);

                bhv->Timer = RAND_RANGE(180, 300);
                bhv->Mode = 2;
            }
            break;

        case 2:
            if(bhv->Timer-- <= 0) {
                return BHV_RET_FINISH;
            }

            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_WatchDanceStand(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0: {
            AL_SetMotionLink(tp, ALM_MUSIC_STAND);

            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);

            bhv->Timer = RAND_RANGE(300, 600);
            bhv->Mode++;
        }
        case 1:
            if(!AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_DANCE)) {
                AL_SetMotionLink(tp, ALM_HAKUSYU_STAND);

                bhv->Timer = RAND_RANGE(180, 300);
                bhv->Mode = 2;
            }
            break;

        case 2:
            if(bhv->Timer-- <= 0) {
                return BHV_RET_FINISH;
            }
            
            break;
    }

    return BHV_RET_CONTINUE;
}