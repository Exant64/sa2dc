#include "Chao/Chao.h"

int ALBHV_Rest(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    float randval;
    CHAO_GLOBAL* Global;

    switch (bhv->Mode) {
        case 0:
            bhv->Timer = (Uint16)(120 + (int)(njRandom() * 501.0f));

            randval = njRandom();
            if (randval < 0.2f) {
                AL_SetMotionLinkStep(tp, ALM_HIMA_SIT, 40);
            } else if (randval < 0.3f) {
                AL_SetMotionLinkStep(tp, ALM_HIMA_SIT_PATAPATA, 40);
            } else if (randval < 0.4f) {
                AL_SetMotionLinkStep(tp, ALM_HIMA_SIT_GUDE, 40);
            } else if (randval < 0.5f) {
                AL_SetMotionLinkStep(tp, ALM_HIMA_SIT_PATA, 40);
            } else if (randval < 0.6f) {
                AL_SetMotionLinkStep(tp, ALM_HIMA_SIT_PATAPATA, 40);
            } else if (randval < 0.7f) {
                AL_SetMotionLinkStep(tp, ALM_HIMA_SIT_SWING, 40);
            } else if (randval < 0.8f) {
                bhv->Timer = (Uint16)(120 + (int)(njRandom() * 121.0f));

                if (njRandom() < 0.5f) {
                    AL_SetMotionLinkStep(tp, ALM_HIMA_SIT_LOOK_R, 40);
                } else {
                    AL_SetMotionLinkStep(tp, ALM_HIMA_SIT_LOOK_L, 40);
                }
            } else {
                bhv->Timer = (Uint16)(60 + (int)(njRandom() * 121.0f));
                AL_SetMotionLinkStep(tp, ALM_HIMA_SIT_PORI, 40);
            }

            ++bhv->Mode;
            break;
        case 1:
            if (bhv->Timer-- <= 0) {
                if (AL_EmotionGetValue(tp, EM_ST_HUNGER) > GET_GLOBAL()->IntentionRestEndTrigger)
                    return BHV_RET_FINISH;

                if (AL_EmotionGetValue(tp, EM_ST_SLEEPY) > GET_GLOBAL()->IntentionRestEndTrigger)
                    return BHV_RET_FINISH;

                if (AL_EmotionGetValue(tp, EM_ST_TEDIOUS) > GET_GLOBAL()->IntentionRestEndTrigger)
                    return BHV_RET_FINISH;

                if (AL_EmotionGetValue(tp, EM_ST_LONELY) > GET_GLOBAL()->IntentionRestEndTrigger)
                    return BHV_RET_FINISH;

                if (AL_EmotionGetValue(tp, EM_ST_TIRE) < 100)
                    return BHV_RET_FINISH;

                if (njRandom() < 0.3f)
                    return BHV_RET_FINISH;

                bhv->Mode = 0;
            }

            if (AL_IsEmotionTimerReset(tp)) {
                AL_EmotionAdd(tp, EM_ST_TIRE,
                              -(GET_GLOBAL()->ParamSubTireRest * (AL_EmotionGetValue(tp, EM_PER_REGAIN) + 300)) /
                                  300); // tire
                AL_EmotionAdd(tp, EM_ST_CONDITION,
                              (GET_GLOBAL()->ParamAddConditionRest * (AL_EmotionGetValue(tp, EM_PER_REGAIN) + 300)) /
                                  300);
            }
            break;
    }
    return BHV_RET_CONTINUE;
}