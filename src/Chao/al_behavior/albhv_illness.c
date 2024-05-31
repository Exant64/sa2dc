#include <Chao/Chao.h>

extern BHV_FUNC ALBHV_IllnessFunc[];

void AL_CalcIntentionScore_Illness(task* tp, float* pMaxScore) {
    AL_EMOTION* pEmotion = &GET_CHAOPARAM(tp)->Emotion;
    CHAO_GLOBAL* Global = GET_GLOBAL();
    int i;

    if (*pMaxScore >= 1) {
        return;
    }

    for (i = 0; i < 6; i++) {
        if (pEmotion->IllState[i] < 0) {
            if (njRandom() * 30000 > AL_EmotionGetValue(tp, EM_ST_CONDITION) + 20000) {
                AL_SetBehavior(tp, ALBHV_IllnessFunc[i]);
                *pMaxScore = 0.99f;
            }
        }
    }
}

int ALBHV_Seki(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    int pose;

    switch (bhv->Mode) {
        case 0:
            pose = AL_GetMotionPosture(tp);
            switch (pose) {
                case 1:
                case 2:
                case 3:
                    if (njRandom() < 0.5f) {
                        AL_SetMotionLinkStep(tp, ALM_SEKI_SIT_ALL, 40);
                    } else {
                        AL_SetMotionLinkStep(tp, ALM_SEKI_SIT_KEHON, 40);
                    }
                    break;
                case 0:
                default:
                    if (njRandom() < 0.5f) {
                        AL_SetMotionLinkStep(tp, ALM_SEKI_STAND_ALL, 40);
                    } else {
                        AL_SetMotionLinkStep(tp, ALM_SEKI_STAND_KEHON, 40);
                    }
                    break;
            }

            if (njRandom() < 0.7f) {
                AL_FaceSetEye(tp, AL_EYE_NUM_KYA, -1);
            } else {
                AL_FaceSetEye(tp, AL_EYE_NUM_TOHOHO, -1);
            }

            if (njRandom() < 0.7f) {
                AL_FaceSetMouth(tp, AL_MOUTH_NUM_HOYO, -1);
            } else {
                AL_FaceSetMouth(tp, AL_MOUTH_NUM_MUSU, -1);
            }

            bhv->Mode++;
            bhv->Timer = 0;
            bhv->SubTimer = 0;
        case 1:
            if (AL_IsMotionStop(tp)) {
                if (njRandom() < 0.1f || bhv->SubTimer++ > 10) {
                    return BHV_RET_FINISH;
                }

                bhv->Mode = 0;
            }

            if (bhv->Timer++ % 60 == 0 && njRandom() < 0.4f) {
                sub_8C05B020(0x606A, 0, 0, 110, &GET_CHAOWK(tp)->pos);
            }

        default:
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Kusyami(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    int pose;

    switch (bhv->Mode) {
        case 0:
            pose = AL_GetMotionPosture(tp);
            switch (pose) {
                case 1:
                case 2:
                case 3:
                    AL_SetMotionLinkStep(tp, ALM_KUSYAMI_SIT, 40);
                    break;
                case 0:
                default:
                    AL_SetMotionLinkStep(tp, ALM_KUSYAMI_STAND, 40);
                    break;
            }

            if (njRandom() < 0.7f) {
                AL_FaceSetEye(tp, AL_EYE_NUM_KYA, -1);
            } else {
                AL_FaceSetEye(tp, AL_EYE_NUM_TOHOHO, -1);
            }

            AL_FaceSetMouth(tp, AL_MOUTH_NUM_KOIKE, -1);

            bhv->Mode++;
            bhv->Timer = 0;
            bhv->SubTimer = 0;
        case 1:
            if (AL_IsMotionStop(tp)) {
                if (bhv->Timer % 60 == 0 && njRandom() < 0.3f) {
                    if (njRandom() < 0.4f || bhv->SubTimer++ > 10)
                        return BHV_RET_FINISH;

                    bhv->Mode = 0;
                }
            }

            if (bhv->Timer++ == 180) {
                sub_8C05B020(0x606B, 0, 0, 110, &GET_CHAOWK(tp)->pos);
                AL_FaceSetMouth(tp, AL_MOUTH_NUM_MUSU, -1);
            }

        default:
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Kayui(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    int pose;

    switch (bhv->Mode) {
        case 0:
            pose = AL_GetMotionPosture(tp);
            switch (pose) {
                case 1:
                case 2:
                case 3:
                case 0:
                default:
                    if (njRandom() < 0.2f) {
                        AL_SetMotionLinkStep(tp, ALM_STAND, 40);
                    } else {
                        AL_SetMotionLinkStep(tp, ALM_PORIPORI_TATI, 40);
                        sub_8C05B020(0x1015, 0, 0, 110, &GET_CHAOWK(tp)->pos);
                        bhv->Timer = RAND_RANGE(60, 120);
                    }
                    break;
            }

            AL_FaceSetEye(tp, AL_EYE_NUM_TOHOHO, -1);
            AL_FaceSetMouth(tp, AL_MOUTH_NUM_KOIKE, -1);

            if (njRandom() < 0.1f) {
                sub_8C05B020(0x602C, 0, 0, 110, &GET_CHAOWK(tp)->pos);
            }

            bhv->Mode++;
        case 1:
            if (bhv->Timer-- <= 0 || AL_IsMotionStop(tp)) {
                if (njRandom() < 0.2f)
                    return BHV_RET_FINISH;

                if (bhv->SubTimer++ > 13) {
                    return BHV_RET_FINISH;
                }

                bhv->Mode = 0;
            }

        default:
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Hanamizu(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    int pose;

    switch (bhv->Mode) {
        case 0:
            pose = AL_GetMotionPosture(tp);
            switch (pose) {
                case 1:
                case 2:
                case 3:
                    AL_SetMotionLinkStep(tp, ALM_SITDOWN_SLOW, 40);
                    break;
                case 0:
                default:
                    AL_SetMotionLinkStep(tp, ALM_STAND, 40);
                    break;
            }

            AL_FaceSetEye(tp, AL_EYE_NUM_TRON, -1);

            if (njRandom() < 0.4f) {
                AL_FaceSetMouth(tp, AL_MOUTH_NUM_KOIKE, -1);
            } else {
                AL_FaceSetMouth(tp, 0, -1);
            }

            bhv->Mode++;
            bhv->Timer = 0;
            bhv->SubTimer = 0;
        case 1:
            if (bhv->Timer++ % 60 == 0 && njRandom() < 0.3f) {
                if (njRandom() < 0.4f || bhv->SubTimer++ > 2) {
                    return BHV_RET_FINISH;
                }

                bhv->Mode = 2;
                bhv->Timer = 0;
                bhv->SubTimer = 0;

                sub_8C05B020(0x1010, 0, 0, 70, &GET_CHAOWK(tp)->pos);
            }
            break;

        case 2:
            if (bhv->Timer++ > 60) {
                bhv->Mode = 0;
                bhv->Timer = 0;
                bhv->SubTimer = 0;
            }
            break;

        default:
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Syakkuri(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    int pose;

    switch (bhv->Mode) {
        case 0:
            pose = AL_GetMotionPosture(tp);
            switch (pose) {
                case 1:
                case 2:
                case 3:
                    AL_SetMotionLinkStep(tp, ALM_SYAKKURI_SIT, 40);
                    break;
                case 0:
                default:
                    AL_SetMotionLinkStep(tp, ALM_SYAKKURI_STAND, 40);
                    break;
            }

            if (njRandom() < 0.7f) {
                AL_FaceSetEye(tp, AL_EYE_NUM_KYA, -1);
            } else {
                AL_FaceSetEye(tp, AL_EYE_NUM_TOHOHO, -1);
            }

            if (njRandom() < 0.5f) {
                AL_FaceSetMouth(tp, AL_MOUTH_NUM_MUSU, -1);
            } else {
                AL_FaceSetMouth(tp, AL_MOUTH_NUM_KOIKE, -1);
            }

            bhv->Mode++;
            bhv->Timer = 0;
            bhv->SubTimer = 0;
        case 1:
            if (bhv->Timer++ % 60 == 0 && AL_IsMotionStop(tp) && njRandom() < 0.5f) {
                if (njRandom() < 0.3f || bhv->SubTimer++ > 10) {
                    return BHV_RET_FINISH;
                }

                bhv->Mode = 0;
            }

            if (bhv->Timer == 20) {
                sub_8C05B020(0x606E, 0, 0, 110, &GET_CHAOWK(tp)->pos);
            }
            break;

        default:
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Haraita(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    int pose;

    switch (bhv->Mode) {
        case 0:
            pose = AL_GetMotionPosture(tp);
            switch (pose) {
                case 1:
                case 2:
                case 3:
                    AL_SetMotionLinkStep(tp, ALM_GUTA_A, 40);
                    break;
                case 0:
                default:
                    AL_SetMotionLinkStep(tp, ALM_GOO, 40);
                    break;
            }

            if (njRandom() < 0.7f) {
                AL_FaceSetEye(tp, AL_EYE_NUM_KYA, -1);
            } else {
                AL_FaceSetEye(tp, AL_EYE_NUM_TOHOHO, -1);
            }

            AL_FaceSetMouth(tp, AL_MOUTH_NUM_KOIKE, -1);

            if (njRandom() < 0.3f) {
                sub_8C05B020(0x6022, 0, 0, 110, &GET_CHAOWK(tp)->pos);
            } else {
                sub_8C05B020(0x6023, 0, 0, 110, &GET_CHAOWK(tp)->pos);
            }

            bhv->Mode++;
            bhv->Timer = 0;
            bhv->SubTimer = 0;
        case 1:
            if (bhv->Timer++ % 60 == 0 && njRandom() < 0.3f) {
                if (njRandom() < 0.4f || bhv->SubTimer++ > 10) {
                    return BHV_RET_FINISH;
                }

                bhv->Mode = 0;
            }
            break;

        default:
            break;
    }

    return BHV_RET_CONTINUE;
}
