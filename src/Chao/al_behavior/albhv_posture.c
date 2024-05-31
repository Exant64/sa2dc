#include "Chao/Chao.h"

// where's the default return? undefined behavior?

enum { MD_PST_CHANGE_INIT, MD_PST_CHANGE_WAIT };

int ALBHV_PostureChangeStand(task* tp) {
    chaowk* cwk = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &cwk->Behavior;
    int posture;

    switch (bhv->Mode) {
        case MD_PST_CHANGE_INIT:
            posture = AL_GetMotionPosture(tp);
            switch (posture) {
                case 1:
                    AL_SetMotionLinkStep(tp, ALM_GENKI_TATI, 40);
                    break;
                case 2:
                    AL_SetMotionLinkStep(tp, ALM_JUMP_UP, 40);
                    break;
                case 3:
                    AL_SetMotionLinkStep(tp, ALM_JUMP_UP, 35);
                    break;
                case 0:
                default:
                    return BHV_RET_FINISH;
            }
            bhv->Mode = MD_PST_CHANGE_WAIT;
            break;

        case MD_PST_CHANGE_WAIT:
            if (AL_IsMotionEnd(tp))
                return BHV_RET_FINISH;
    }
}

int ALBHV_PostureChangeSit(task* tp) {
    chaowk* cwk = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &cwk->Behavior;
    int posture;

    switch (bhv->Mode) {
        case MD_PST_CHANGE_INIT:
            posture = AL_GetMotionPosture(tp);
            switch (posture) {
                case 2:
                    AL_SetMotionLinkStep(tp, ALM_OKIRU, 40);
                    break;
                case 3:
                    AL_SetMotionLinkStep(tp, ALM_OKIRU, 35);
                    break;
                case 0:
                    AL_SetMotionLinkStep(tp, ALM_GENKI_SUWARI, 40);
                    break;
                default:
                    return BHV_RET_FINISH;
            }
            bhv->Mode = MD_PST_CHANGE_WAIT;
            break;

        case MD_PST_CHANGE_WAIT:
            if (AL_IsMotionEnd(tp))
                return BHV_RET_FINISH;
    }
}

int ALBHV_PostureChangeUnknown(task* tp) {
    chaowk* cwk = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &cwk->Behavior;
    int posture;

    switch (bhv->Mode) {
        case MD_PST_CHANGE_INIT:
            posture = AL_GetMotionPosture(tp);
            switch (posture) {
                case 1:
                    AL_SetMotionLinkStep(tp, ALM_SLEEP, 40);
                    break;
                case 0:
                    AL_SetMotionLinkStep(tp, ALM_BATAN, 40);
                    break;
                case 3:
                    AL_SetMotionLinkStep(tp, ALM_SLEEP, 40);
                    break;
                default:
                    return BHV_RET_FINISH;
            }
            bhv->Mode = MD_PST_CHANGE_WAIT;
            break;

        case MD_PST_CHANGE_WAIT:
            if (AL_IsMotionEnd(tp))
                return BHV_RET_FINISH;
    }
}