#include "Chao/Chao.h"
#include "playsound.h"

void AL_CalcIntentionScore_Hima(task* tp, float* pMaxScore) {
    float score = 0.0f;
    CHAO_GLOBAL* Global = GET_GLOBAL();
    // int ;

    Uint32 himatrigger = GET_GLOBAL()->IntentionHimaTrigger;
    Uint32 value = AL_EmotionGetValue(tp, EM_ST_TEDIOUS);
    if (*pMaxScore < 1) {

        if (value > himatrigger) {
            score = AL_CalcScoreTypeA(value, himatrigger);
            score *= GET_GLOBAL()->IntentionHimaMaxScore;
            AL_ScoreRandomise(&score);
        }

        if (score > *pMaxScore) {
            bool trigger = FALSE;

            if (njRandom() < 0.4) {
                float randval2 = njRandom();
                if (randval2 < 0.25) {
                    if (AL_DecideBehaviorMusic(tp)) {
                        AL_EmotionAdd(tp, EM_ST_TEDIOUS, -5000);
                        AL_ClearIntention(tp);
                        *pMaxScore = 0.99f;
                        trigger = TRUE;
                    }
                } else if (randval2 < 0.5) {
                    if (AL_DecideBehaviorDance(tp)) {
                        AL_EmotionAdd(tp, EM_ST_TEDIOUS, -5000);
                        AL_ClearIntention(tp);
                        *pMaxScore = 0.99f;
                        trigger = TRUE;
                    }
                } else if (randval2 < 0.75) {
                    if (AL_DecideBehaviorSing(tp)) {
                        AL_EmotionAdd(tp, EM_ST_TEDIOUS, -5000);
                        AL_ClearIntention(tp);
                        *pMaxScore = 0.99f;
                        trigger = TRUE;
                    }
                } else {
                    if (AL_DecideBehaviorArt(tp)) {
                        AL_EmotionAdd(tp, EM_ST_TEDIOUS, -5000);
                        AL_ClearIntention(tp);
                        *pMaxScore = 0.99f;
                        trigger = TRUE;
                    }
                }
            } else {
                if (njRandom() < 0.5) {
                    if (AL_DecideBehaviorSToy(tp)) {
                        AL_EmotionAdd(tp, EM_ST_TEDIOUS, -5000);
                        AL_ClearIntention(tp);
                        *pMaxScore = 0.99f;
                        trigger = TRUE;
                    }
                } else {
                    if (AL_DecideBehaviorMinimal(tp)) {
                        AL_EmotionAdd(tp, EM_ST_TEDIOUS, -5000);
                        AL_ClearIntention(tp);
                        *pMaxScore = 0.99f;
                        trigger = TRUE;
                    }
                }
            }

            if (trigger) {
                // ALBHV_Himaunused was probably meant to be used here
            }
        }
    }
}

int ALBHV_HimaUnused(task *tp) {
    return BHV_RET_CONTINUE;
}