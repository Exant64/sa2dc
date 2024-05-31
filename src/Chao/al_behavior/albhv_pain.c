#include "Chao/Chao.h"
#include "playsound.h"

int ALBHV_Pain(task* tp);

void AL_CalcIntentionScore_Pain(task* tp, float* pMaxScore) {
    float score = 0.0f;
    CHAO_GLOBAL* Global = GET_GLOBAL();
    int value = AL_EmotionGetValue(tp, EM_MD_PAIN);
    if (value > Global->IntentionPainTrigger) {
        score =
            1.0 - ((100 - AL_EmotionGetValue(tp, EM_PER_AGRESSIVE)) * (1.0 - Global->IntentionPainMinScore) * 0.005f);
        AL_ScoreRandomise(&score);
        if (score > *pMaxScore) {
            AL_SetBehavior(tp, ALBHV_Pain);
            AL_ClearIntention(tp);
            *pMaxScore = score;
        }
    }
}

int ALBHV_Pain(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    float randval;
    switch (bhv->Mode) {
        case 0:
            if (AL_EmotionGetValue(tp, EM_MD_PAIN) > 100) {
                AL_SetMotionLinkStep(tp, ALM_ITAI_PIYO, 35);

                if (njRandom() < 0.5) {
                    AL_FaceChangeEye(tp, AL_EYE_NUM_GURUGURU);
                }

                randval = njRandom();
                if (randval < 0.5) {
                    AL_FaceChangeMouth(tp, AL_MOUTH_NUM_KOIKE);
                } else {
                    AL_FaceChangeMouth(tp, AL_MOUTH_NUM_MUSU);
                }

                bhv->Timer = RAND_RANGE(240, 420);
                ++bhv->Mode;
            } else {
                bhv->Mode = 2;
            }
        case 1:
            if (bhv->Timer-- <= 0) {
                AL_EmotionAdd(tp, EM_MD_PAIN, -10);
                bhv->Mode = 2;
            }
            break;
        case 2:
            randval = njRandom();
            if (randval < 0.33f)
                AL_SetMotionLinkStep(tp, ALM_ITAI_BURUBURU, 35);
            else if (randval < 0.66f)
                AL_SetMotionLinkStep(tp, ALM_ITAI_ATAMAPONPON, 35);
            else
                AL_SetMotionLinkStep(tp, ALM_ITAI_KOSHIPONPON, 35);

            if (njRandom() < 0.5f)
                AL_FaceChangeEye(tp, AL_EYE_NUM_TOHOHO);
            else
                AL_FaceChangeEye(tp, AL_EYE_NUM_KYA);

            AL_FaceChangeMouth(tp, AL_MOUTH_NUM_MUSU);

            bhv->Timer = RAND_RANGE(120, 320);
            ++bhv->Mode;
            break;
        case 3:
            if (bhv->Timer-- <= 0) {
                if (bhv->SubTimer++ > 4)
                    return BHV_RET_FINISH;

                AL_EmotionAdd(tp, EM_MD_PAIN, -20);
                if (AL_EmotionGetValue(tp, EM_MD_PAIN) < 10)
                    return BHV_RET_FINISH;

                bhv->Mode = 2;
            }
            break;
    }
    return BHV_RET_CONTINUE;
}