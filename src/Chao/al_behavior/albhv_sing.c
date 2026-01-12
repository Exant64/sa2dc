#include <Chao/Chao.h>

#include <Chao/al_misc.h>
#include <Chao/al_field.h>
#include <playsound.h>

extern BHV_FUNC ALBHV_SingFunc[8];

extern int KinderSingVoiceNumber[5];
extern int NormalSingVoiceNumber[3];

void VoiceTaskExecutor(task* tp) {
    taskwk* twk = tp->twp;

    if(!twk->wtimer--) {
        FreeTask(tp);
        return;
    }
    
    sub_8C05B2D4(twk->id, (Uint32)tp, 1, 110, 60, &tp->twp->pos);
}

void VoiceTaskCreate(task* pChaoTask, int id, Sint16 a3, Sint16 timer) {
    task* pVoice = CreateElementalTask(2, 3, VoiceTaskExecutor, "VoiceTask");

    pVoice->twp->pos = pChaoTask->twp->pos;
    pVoice->twp->ang.y = a3;
    pVoice->twp->id = id;
    pVoice->twp->wtimer = timer;
}

int ALBHV_Sing1(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_SetMotionLink(tp, ALM_UTAU_HIDARI);

            bhv->Mode++;
            bhv->Timer = 0;
            bhv->SubTimer = 0;
        case 1:
            if(AL_IsMotionStop(tp)) {
                if(njRandom() < 0.5f) {
                    AL_SetMotionLink(tp, ALM_UTAU_HIDARI);
                }
                else {
                    AL_SetMotionLink(tp, ALM_UTAU_LEFT);
                }
            }

            if(bhv->Timer++ > 240) {
                bhv->Timer = 0;

                sub_8C05B2D4(KinderSingVoiceNumber[0], (Uint32)tp, 1, 110, 230, &tp->twp->pos);
            }

            if(!(bhv->SubTimer++ % 30)) {
                if(njRandom() < 0.5f) {
                    AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);
                }
                else {
                    AL_FaceSetMouth(tp, AL_MOUTH_NUM_HOYO, -1);
                }
            }
            break;
    }

    return !AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_SING);
}

int ALBHV_Sing2(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_SetMotionLink(tp, ALM_UTAU_HIDARI);

            bhv->Mode++;
            bhv->Timer = 0;
            bhv->SubTimer = 0;
        case 1:
            if(AL_IsMotionStop(tp)) {
                if(njRandom() < 0.5f) {
                    AL_SetMotionLink(tp, ALM_UTAU_HIDARI);
                }
                else {
                    AL_SetMotionLink(tp, ALM_UTAU_LEFT);
                }
            }

            if(bhv->Timer++ > 240) {
                bhv->Timer = 0;
                
                sub_8C05B2D4(KinderSingVoiceNumber[(int)(njRandom() * 1.99f)], (Uint32)tp, 1, 110, 230, &tp->twp->pos);
            }

            if(!(bhv->SubTimer++ % 30)) {
                if(njRandom() < 0.5f) {
                    AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);
                }
                else {
                    AL_FaceSetMouth(tp, AL_MOUTH_NUM_HOYO, -1);
                }
            }
            break;
    }

    return !AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_SING);
}

int ALBHV_Sing3(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_SetMotionLink(tp, ALM_UTAU_HIDARI);

            bhv->Mode++;
            bhv->Timer = 0;
            bhv->SubTimer = 0;
        case 1:
            if(AL_IsMotionStop(tp)) {
                if(njRandom() < 0.5f) {
                    AL_SetMotionLink(tp, ALM_UTAU_HIDARI);
                }
                else {
                    AL_SetMotionLink(tp, ALM_UTAU_LEFT);
                }
            }

            if(bhv->Timer++ > 240) {
                bhv->Timer = 0;
                
                sub_8C05B2D4(KinderSingVoiceNumber[(int)(njRandom() * 2.99f)], (Uint32)tp, 1, 110, 230, &tp->twp->pos);
            }

            if(!(bhv->SubTimer++ % 30)) {
                if(njRandom() < 0.5f) {
                    AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);
                }
                else {
                    AL_FaceSetMouth(tp, AL_MOUTH_NUM_HOYO, -1);
                }
            }
            break;
    }

    return !AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_SING);
}

int ALBHV_Sing4(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_SetMotionLink(tp, ALM_UTAU_HIDARI);

            bhv->Mode++;
            bhv->Timer = 0;
            bhv->SubTimer = 0;
        case 1:
            if(AL_IsMotionStop(tp)) {
                if(njRandom() < 0.5f) {
                    AL_SetMotionLink(tp, ALM_UTAU_HIDARI);
                }
                else {
                    AL_SetMotionLink(tp, ALM_UTAU_LEFT);
                }
            }

            if(bhv->Timer++ > 240) {
                bhv->Timer = 0;
                
                sub_8C05B2D4(KinderSingVoiceNumber[(int)(njRandom() * 3.99f)], (Uint32)tp, 1, 110, 230, &tp->twp->pos);
            }

            if(!(bhv->SubTimer++ % 30)) {
                if(njRandom() < 0.5f) {
                    AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);
                }
                else {
                    AL_FaceSetMouth(tp, AL_MOUTH_NUM_HOYO, -1);
                }
            }
            break;
    }

    return !AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_SING);
}

int ALBHV_Sing5(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_SetMotionLink(tp, ALM_UTAU_HIDARI);

            bhv->Mode++;
            bhv->Timer = 0;
            bhv->SubTimer = 0;
        case 1:
            if(AL_IsMotionStop(tp)) {
                if(njRandom() < 0.5f) {
                    AL_SetMotionLink(tp, ALM_UTAU_HIDARI);
                }
                else {
                    AL_SetMotionLink(tp, ALM_UTAU_LEFT);
                }
            }

            if(bhv->Timer++ > 240) {
                bhv->Timer = 0;
                
                if(njRandom() < 0.7f) {
                    sub_8C05B2D4(KinderSingVoiceNumber[(int)(njRandom() * 3.99f)], (Uint32)tp, 1, 110, 230, &tp->twp->pos);
                }
                else {
                    sub_8C05B2D4(KinderSingVoiceNumber[4], (Uint32)tp, 1, 110, 440, &tp->twp->pos);

                    bhv->Mode = 2;
                }

                if(!AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_SING))  {
                    return BHV_RET_FINISH;
                }
            }

            if(!(bhv->SubTimer++ % 30)) {
                if(njRandom() < 0.5f) {
                    AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);
                }
                else {
                    AL_FaceSetMouth(tp, AL_MOUTH_NUM_HOYO, -1);
                }
            }
            break;

        case 2:
            if(AL_IsMotionStop(tp)) {
                if(njRandom() < 0.5f) {
                    AL_SetMotionLink(tp, ALM_UTAU_HIDARI);
                }
                else {
                    AL_SetMotionLink(tp, ALM_UTAU_LEFT);
                }
            }

            if(bhv->Timer++ > 450) {
                bhv->Timer = 0;
                
                if(njRandom() < 0.3f) {
                    sub_8C05B2D4(KinderSingVoiceNumber[4], (Uint32)tp, 1, 110, 440, &tp->twp->pos);
                }
                else {
                    sub_8C05B2D4(KinderSingVoiceNumber[(int)(njRandom() * 3.99f)], (Uint32)tp, 1, 110, 230, &tp->twp->pos);

                    bhv->Mode = 1;
                }

                if(!AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_SING))  {
                    return BHV_RET_FINISH;
                }
            }

            if(!(bhv->SubTimer++ % 30)) {
                if(njRandom() < 0.5f) {
                    AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);
                }
                else {
                    AL_FaceSetMouth(tp, AL_MOUTH_NUM_HOYO, -1);
                }
            }
            break;
    }

    if(!AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_SING))  {
        return BHV_RET_FINISH;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_SingNormal(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_SetMotionLink(tp, ALM_UTAU_HIDARI);

            bhv->Mode++;
            bhv->Timer = 0;
            bhv->SubTimer = 0;
        case 1:
            if(AL_IsMotionStop(tp)) {
                if(njRandom() < 0.5f) {
                    AL_SetMotionLink(tp, ALM_UTAU_HIDARI);
                }
                else {
                    AL_SetMotionLink(tp, ALM_UTAU_LEFT);
                }
            }

            if(bhv->Timer++ > 180) {
                bhv->Timer = 0;
                
                sub_8C05B2D4(NormalSingVoiceNumber[(int)(njRandom() * 3.99f)], (Uint32)tp, 1, 110, 230, &tp->twp->pos);
            }

            if(!(bhv->SubTimer++ % 30)) {
                if(njRandom() < 0.5f) {
                    AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);
                }
                else {
                    AL_FaceSetMouth(tp, AL_MOUTH_NUM_HOYO, -1);
                }
            }
            break;
    }

    return !AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_SING);
}

int ALBHV_SingOnchi(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_SetMotionLink(tp, ALM_UTAU_HIDARI);

            bhv->Mode++;
            bhv->Timer = 0;
            bhv->SubTimer = 0;
        case 1:
            if(AL_IsMotionStop(tp)) {
                if(njRandom() < 0.5f) {
                    AL_SetMotionLink(tp, ALM_UTAU_HIDARI);
                }
                else {
                    AL_SetMotionLink(tp, ALM_UTAU_LEFT);
                }
            }

            if(bhv->Timer++ > 220) {
                bhv->Timer = 0;
                
                sub_8C05B020(24700, 0, 0, 110, &tp->twp->pos);
            }

            if(!(bhv->SubTimer++ % 30)) {
                if(njRandom() < 0.5f) {
                    AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);
                }
                else {
                    AL_FaceSetMouth(tp, AL_MOUTH_NUM_HOYO, -1);
                }
            }
            break;
    }

    return !AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_SING_ONCHI);
}

int ALBHV_ListenSing(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch (bhv->Mode) {
        case 0:
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
        case 1:
            if(bhv->Timer-- <= 0) {
                bhv->Mode = 0;
            }

            if(!AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_SING)) {
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

int ALBHV_Urusai(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch (bhv->Mode) {
        case 0: {
            int posture = AL_GetMotionPosture(tp);
            switch(posture) {
                case 1:
                case 2:
                case 3:
                    AL_SetMotionLinkStep(tp, ALM_ITAI_BURUBURU, 35);
                    break;
                default:
                case 0:
                    AL_SetMotionLink(tp, ALM_HIMA_SIT_GUDE);
                    break;
            }

            if(njRandom() < 0.5f) {
                AL_FaceSetEye(tp, AL_EYE_NUM_KYA, -1);
            }
            else {
                AL_FaceSetEye(tp, AL_EYE_NUM_TOHOHO, -1);
            }

            if(njRandom() < 0.5f) {
                AL_FaceSetMouth(tp, AL_MOUTH_NUM_KOIKE, -1);
            }
            else {
                AL_FaceSetMouth(tp, AL_MOUTH_NUM_MUSU, -1);
            }

            bhv->Timer = RAND_RANGE(300, 600);
            bhv->Mode++;
        }
        case 1:
            if(bhv->Timer-- <= 0) {
                if(njRandom() < 0.5f) {
                    AL_EmotionAdd(tp, EM_MD_ANGER, 150);
                }

                return BHV_RET_FINISH;
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

void AL_CalcIntentionScore_JoinSing(task* tp, float* pMaxScore) {
    float score = 0.f;
    Uint32 trigger = GET_GLOBAL()->IntentionHimaTrigger;
    Uint32 emotion = AL_EmotionGetValue(tp, EM_ST_TEDIOUS);
    task* pField = AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_SING);

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
            if(pField->ptp) {
                MOV_SetAimPos(tp, &pField->ptp->twp->pos);
            }

            AL_SetBehavior(tp, ALBHV_TurnToAim);
            AL_SetNextBehavior(tp, ALBHV_PostureChangeSit);
            AL_SetNextBehavior(tp, ALBHV_ListenSing);
            
            *pMaxScore = 0.99f;
        }
    }
}

void AL_CalcIntentionScore_JoinOnchi(task* tp, float* pMaxScore) {
    float score = 0.f;
    Uint32 trigger = GET_GLOBAL()->IntentionHimaTrigger;
    Uint32 emotion = AL_EmotionGetValue(tp, EM_ST_TEDIOUS);
    task* pField = AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_SING_ONCHI);

    if(*pMaxScore < 1 && pField) {
        if(emotion > trigger) {
            if(njRandom() < 0.7f) {
                score = 0.99f;
            }
        }

        if(score > *pMaxScore) {
            AL_SetBehavior(tp, ALBHV_Urusai);
            
            *pMaxScore = score;
        }
    }
}

Bool AL_DecideBehaviorSing(task* tp) {
    float score = 0;
    taskwk* twk = tp->twp;

    int kind = -1;

    if(AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_SING)) {
        return FALSE;
    }
    else {
        int i;

        for(i = 0; i < NB_AL_SONG; ++i) {
            if(AL_KW_IsSongFlagOn(tp, i)) {
                kind++;
            }
        }

        if(kind >= 0) {
            NJS_POINT3 center;
            int ang = twk->ang.y;

            center.x = twk->pos.x + 8 * njSin(ang);
            center.y = twk->pos.y;
            center.z = twk->pos.z + 8 * njCos(ang);

            AL_SetBehavior(tp, ALBHV_SingFunc[kind]);

            AL_ChildFieldCreateT(
                tp,
                CI_KIND_AL_SING,
                &center,
                20,
                RAND_RANGE(1500, 3300)
            );

            return TRUE;
        }
        else if(AL_PartsIsMinimalFlagOn(tp, eMinimalType_Oum)) {
            NJS_POINT3 center;
            int ang = twk->ang.y;

            center.x = twk->pos.x + 8 * njSin(ang);
            center.y = twk->pos.y;
            center.z = twk->pos.z + 8 * njCos(ang);

            if(GET_CHAOPARAM(tp)->body.APos < -0.5f) {
                AL_SetBehavior(tp, ALBHV_SingOnchi);

                AL_ChildFieldCreateT(
                    tp,
                    CI_KIND_AL_SING_ONCHI,
                    &center,
                    20,
                    RAND_RANGE(1800, 3600)
                );
            }
            else {
                AL_SetBehavior(tp, ALBHV_SingNormal);
                
                AL_ChildFieldCreateT(
                    tp,
                    CI_KIND_AL_SING,
                    &center,
                    20,
                    RAND_RANGE(1200, 2400)
                );
            }

            return TRUE;
        }
    }

    return FALSE;
}