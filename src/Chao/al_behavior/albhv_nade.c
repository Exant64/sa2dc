#include <Chao/Chao.h>
#include <Chao/al_misc.h>

int ALBHV_Nade(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    int pose;

    switch (bhv->Mode) {
        case 0:
            AL_FixPosition(tp);
            pose = AL_GetMotionPosture(tp);
            switch (pose) {
                case 1:
                    AL_SetMotionLinkStep(tp, ALM_HIMA_SIT, 20);
                    break;
                case 2:
                    AL_SetMotionLinkStep(tp, ALM_OKIRU, 20);
                    break;
                case 3:
                    AL_SetMotionLinkStep(tp, ALM_HIMA_SIT, 20);
                    break;
                case 0:
                default:
                    AL_SetMotionLinkStep(tp, ALM_STAND, 20);
                    break;
            }

            if (AL_EmotionGetValue(tp, EM_MD_RELAX) < 10) {
                AL_ParameterAddUserLike(tp, 1, GET_GLOBAL()->NadeAddAttr);
            }

            bhv->Mode++;
            break;
        case 1:
            if (AL_IsMotionEnd(tp)) {
                if (AL_EmotionGetValue(tp, EM_ST_SLEEP_DEPTH) > 500) {
                    AL_SetBehavior(tp, ALBHV_Tron);
                } else {
                    pose = AL_GetMotionPosture(tp);
                    switch (pose) {
                        case 1:
                        case 2:
                            if (njRandom() < 0.5f) {
                                AL_SetMotionLinkStep(tp, ALM_HIMA_SIT_PATAPATA, 20);
                            } else {
                                AL_SetMotionLinkStep(tp, ALM_HIMA_SIT_SWING, 20);
                            }
                            break;

                        case 0:
                        default:
                            if (njRandom() < 0.5f) {
                                AL_SetMotionLinkStep(tp, ALM_NADE_GLAD, 20);
                            } else {
                                AL_SetMotionLinkStep(tp, ALM_NADE_KIMOCHII, 20);
                            }
                            break;
                    }

                    bhv->Mode++;
                    bhv->Timer = 10;

                    if (njRandom() < 0.3) {
                        if (njRandom() < 0.5f) {
                            sub_8C05B020(0x6074, 0, 0, 110, &GET_CHAOWK(tp)->pos);
                        } else {
                            sub_8C05B020(0x6075, 0, 0, 110, &GET_CHAOWK(tp)->pos);
                        }
                    }
                }
            }
            break;
        case 2:
            if (AL_IsMotionEnd(tp)) {
                if (--bhv->Timer <= 0) {
                    AL_DisableTouch(tp);
                    bhv->Mode++;
                }
            }

            if (AL_EmotionGetValue(tp, EM_MD_RELAX) > 100) {
                AL_FaceChangeEye(tp, AL_EYE_NUM_TOHOHO);
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_NIKO);
            }

            if (bhv->SubTimer++ >= 3) {
                AL_EmotionAdd(tp, EM_MD_RELAX, 1);
            }

            if (CCL_IsHitKind2(tp, CI_KIND_PATTING_HAND)) {
                break;
            }

            switch (AL_GetMotionPosture(tp)) {
                case 1:
                    AL_SetMotionLinkStep(tp, ALM_OSYABURI_L, 20);
                    break;
                case 0:
                    AL_SetMotionLinkStep(tp, ALM_OSYABURI, 20);
                    break;
            }

            bhv->Mode = 4;
            bhv->Timer = 0;
            bhv->SubTimer = 0;
            break;

        case 3:
            if (bhv->Timer++ > 10) {
                return BHV_RET_FINISH;
            }
            break;

        case 4:
            if (bhv->SubTimer++ >= 15) {
                AL_EmotionAdd(tp, EM_MD_RELAX, -1);
            }

            if (AL_EmotionGetValue(tp, EM_MD_RELAX) < 10) {
                return BHV_RET_FINISH;
            }

            break;
    }

    return BHV_RET_CONTINUE;
}

Bool AL_CheckNade(task* tp) {
    BHV_FUNC func = AL_GetBehavior(tp);
    task* hit;
    if (func == ALBHV_Nade || func == ALBHV_Tron || func == ALBHV_Cry || func == ALBHV_Tameiki ||
        func == ALBHV_MesoMeso) {
        return 0;
    }

    if ((GET_CHAOWK(tp)->flag & 1) && AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_PATTING_HAND)) {
        AL_SetBehavior(tp, ALBHV_Nade);

        return TRUE;
    }
    return FALSE;
}
