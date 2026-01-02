#include <Chao/Chao.h>

int ALBHV_Think(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0: {
            int posture = AL_GetMotionPosture(tp);
            switch(posture) {
                case 2:
                case 1:
                case 3:
                    AL_SetMotionLinkStep(tp, ALM_HIMA_SIT, 35);
                    break;
                case 0:
                default: {
                    float randval = njRandom();
                    if(randval < 0.15f) {
                        AL_SetMotionLink(tp, ALM_HATENA_HATE2);
                        AL_IconSet(tp, MD_ICON_HATENA, 180);

                        sub_8C05B020(24604, 0, 0, 110, &tp->twp->pos);
                    }
                    else if (randval < 0.5f) {
                        AL_SetMotionLink(tp, ALM_STAND);
                    }
                    else if (randval < 0.6f) {
                        AL_SetMotionLink(tp, ALM_KYOROKYORO_UP);
                    }
                    else if (randval < 0.7f) {
                        AL_SetMotionLink(tp, ALM_KYOROKYORO_DOWN);
                    }
                    else {
                        AL_SetMotionLink(tp, ALM_OSYABURI);
                    }
                    
                    break;
                }
            }

            AL_FaceChangeEye(tp, AL_EYE_NUM_NORMAL);
            AL_FaceChangeMouth(tp, AL_MOUTH_NUM_NONE);

            bhv->Timer = RAND_RANGE(60, 180);
            bhv->Mode++;
        }

        case 1:
            if(bhv->Timer-- <= 0) {
                return BHV_RET_THINK;
            }
            
            break;
    }

    AL_Brake(tp, 0.8f);
    
    return BHV_RET_CONTINUE;
}

// dunno what these do yet
// best guess I have is that its somehow kindergarten related

int ALBHV_Think2(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            if(njRandom() < 0.5f) {
                AL_SetMotionLink(tp, ALM_KYOROKYORO_UP);
            }
            else {
                AL_SetMotionLink(tp, ALM_KYOROKYORO_DOWN);
            }

            bhv->Mode++;
            bhv->Timer = 5 + (Uint16)(njRandom() * 10);
        case 1:
            if(AL_IsMotionEnd(tp)) {
                if(!--bhv->Timer) {
                    return BHV_RET_FINISH;
                }
            }
            
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Think3(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_STAND);

            bhv->Mode++;
            bhv->Timer = 5 + (Uint16)(njRandom() * 10);
        case 1:
            if(AL_IsMotionEnd(tp)) {
                if(!--bhv->Timer) {
                    return BHV_RET_FINISH;
                }
            }
            
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Think4(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_OSYABURI);

            bhv->Mode++;
            bhv->Timer = 10 + (Uint16)(njRandom() * 10);
        case 1:
            if(AL_IsMotionEnd(tp)) {
                if(!--bhv->Timer) {
                    return BHV_RET_FINISH;
                }
            }
            
            break;
    }

    return BHV_RET_CONTINUE;
}