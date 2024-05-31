#include "Chao/Chao.h"
#include "playsound.h"

int ALBHV_Pleasure(task* tp);

void AL_CalcIntentionScore_Pleasure(task* tp, float* pMaxScore) {
    float score = 0.0f;
    CHAO_GLOBAL* Global = GET_GLOBAL();
    int value = AL_EmotionGetValue(tp, EM_MD_PLEASURE);
    if (value > Global->IntentionPleasureTrigger) {
        score =
            1.0 - ((100 - AL_EmotionGetValue(tp, EM_PER_CALM)) * (1.0 - Global->IntentionPleasureMinScore) * 0.005f);
        AL_ScoreRandomise(&score);
        if (score > *pMaxScore) {
            AL_SetBehavior(tp, ALBHV_Pleasure);
            AL_ClearIntention(tp);
            *pMaxScore = score;
        }
    }
}

int ALBHV_Pleasure(task* tp) {
    enum {
        MD_PLEASURE_INIT,
        MD_PLEASURE_HASYAGU,
        MD_PLEASURE_HASYAGU_END,
        MD_PLEASURE_GLAD,
        MD_PLEASURE_BANZAI,
        MD_PLEASURE_END
    };

    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    float randval;
    switch (bhv->Mode) {
        case MD_PLEASURE_INIT:
            randval = njRandom();
            if (randval < 1 / 5.0f) {
                AL_SetMotionLink(tp, ALM_HASYAGU_JUMP);
                bhv->Mode = MD_PLEASURE_HASYAGU;
            } else {
                if (randval < 6 / 10.0f) {
                    AL_SetMotionLink(tp, ALM_GLAD_STAND);
                    bhv->Mode = MD_PLEASURE_GLAD;
                } else {
                    AL_SetMotionLink(tp, ALM_BANZAI_JUMP);
                    bhv->Mode = MD_PLEASURE_BANZAI;
                }
            }

            randval = njRandom();
            if (randval < 0.5f) {
                AL_FaceChangeEye(tp, AL_EYE_NUM_KYA);
            } else {
                AL_FaceChangeEye(tp, AL_EYE_NUM_NIKO);
            }

            randval = njRandom();
            if (randval < 0.4f) {
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_NIKO);
            } else if (randval < 0.8f) {
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_WAAI);
            } else {
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_HOYO);
            }

            if (njRandom() < 0.7f) {
                // 0.2
                randval = njRandom();
                if (randval < 0.2f) {
                    sub_8C05B020(24623, 0, 0, 110, &GET_CHAOWK(tp)->pos);
                } else if (randval < 0.4f) {
                    sub_8C05B020(24624, 0, 0, 110, &GET_CHAOWK(tp)->pos);
                } else if (randval < 0.6f) {
                    sub_8C05B020(24625, 0, 0, 110, &GET_CHAOWK(tp)->pos);
                } else if (randval < 0.8f) {
                    sub_8C05B020(24626, 0, 0, 110, &GET_CHAOWK(tp)->pos);
                } else {
                    sub_8C05B020(24627, 0, 0, 110, &GET_CHAOWK(tp)->pos);
                }
            }

            bhv->Timer = (Uint16)(120 + (int)(njRandom() * 61.0f));
        case MD_PLEASURE_HASYAGU:
            if (--bhv->Timer <= 0) {
                AL_SetMotionLink(tp, ALM_HASYAGU_LAND);
                bhv->Mode = MD_PLEASURE_HASYAGU_END;
            }
            break;
        case MD_PLEASURE_HASYAGU_END:
            if (AL_IsMotionEnd(tp))
                bhv->Mode = MD_PLEASURE_END;
            break;
        case MD_PLEASURE_GLAD:
            if (--bhv->Timer <= 0)
                bhv->Mode = MD_PLEASURE_END;
            break;
        case MD_PLEASURE_BANZAI:
            if (--bhv->Timer <= 0 && AL_IsMotionEnd(tp))
                bhv->Mode = MD_PLEASURE_END;
            break;
        case MD_PLEASURE_END:
            if (bhv->SubTimer++ > 8)
                return BHV_RET_FINISH;

            AL_EmotionAdd(tp, EM_MD_PLEASURE, -20);
            if (AL_EmotionGetValue(tp, EM_MD_PLEASURE) < 10)
                return BHV_RET_FINISH;

            bhv->Mode = MD_PLEASURE_INIT;

            break;
    }
    return BHV_RET_CONTINUE;
}