#include "Chao/Chao.h"
#include "playsound.h"

int ALBHV_Notice(task* tp) {
    enum { MD_NOTICE_START, MD_NOTICE_WAIT };

    chaowk* work = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &work->Behavior;

    switch (bhv->Mode) {
        case MD_NOTICE_START:
            bhv->LimitTimer = 1800;
            AL_IconSet(tp, MD_ICON_BIKKURI, 180);
            ++bhv->Mode;
            bhv->Timer = 0;
            bhv->SubTimer = 0;
            break;

        case MD_NOTICE_WAIT:
            if (bhv->Timer++ > 120)
                return BHV_RET_FINISH;
            break;
    }

    if (--bhv->LimitTimer <= 0) {
        AL_SetBehavior(tp, ALBHV_Think);
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_TurnToAim(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_ARUKU, 15);
            ++bhv->Mode;
            bhv->Timer = 60;
            break;
    }

    if (MOV_TurnToAim2(tp, 512) < 182) {
        return BHV_RET_FINISH;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_GoToAim(task* tp) {
    enum { MD_GTAM_WALK, MD_GTAM_WALK2 };

    chaowk* work = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &work->Behavior;
    float dist;

    switch (bhv->Mode) {
        case MD_GTAM_WALK:
            AL_FaceChangeEye(tp, AL_EYE_NUM_NIKO);

            bhv->LimitTimer = 1800;

            if (bhv->Timer++ > 30) {
                if (AL_ParameterGetSkill(tp, SKILL_RUN) < 150) {
                    AL_SetMotionLink(tp, ALM_HAIHAI);
                } else {
                    AL_SetMotionLink(tp, ALM_ARUKU);
                }
                ++bhv->Mode;
            }
            break;
        case MD_GTAM_WALK2:
            MOV_TurnToAim2(tp, 384);
            AL_ForwardAcc(tp, GET_GLOBAL()->WalkAcc * 0.8f);
            dist = MOV_DistFromAim(tp);
            if (dist < 0.0) {
                AL_SetBehavior(tp, ALBHV_Think);

            } else if (dist < 4.0) {
                return BHV_RET_FINISH;
            }
    }
    AL_SetMotionSpd(tp, 1.5);
    if (--bhv->LimitTimer <= 0) {
        AL_SetBehavior(tp, ALBHV_Think);
    }
    return BHV_RET_CONTINUE;
}

int ALBHV_TurnToLockOn(task* tp) {
    chaowk* work = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &work->Behavior;
    float dist;

    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_ARUKU, 15);
            bhv->Timer = 90;
            bhv->Mode = 1;
            break;
        case 1:
            if (bhv->Timer-- > 0) {
                ALW_TurnToLockOn(tp, 512);
            } else {
                return BHV_RET_FINISH;
            }
            break;
    }
    AL_SetMotionSpd(tp, 1.6f);
    return BHV_RET_CONTINUE;
}

enum { MD_TURN_INIT, MD_TURN_LEFT, MD_TURN_RIGHT };

int ALBHV_Turn(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    chaowk* work = GET_CHAOWK(tp);

    switch (bhv->Mode) {
        case MD_TURN_INIT:
            AL_SetMotionLinkStep(tp, ALM_ARUKU, 15);

            if (njRandom() < 0.5f)
                bhv->Mode = MD_TURN_LEFT;
            else
                bhv->Mode = MD_TURN_RIGHT;

            bhv->Timer = RAND_RANGE(45, 90);
            break;
        case MD_TURN_LEFT:
            if (bhv->Timer-- > 0) {
                work->ang.y += 364;
            } else {
                return BHV_RET_FINISH;
            }
            break;
        case MD_TURN_RIGHT:
            if (bhv->Timer-- > 0) {
                work->ang.y -= 364;
            } else {
                return BHV_RET_FINISH;
            }
            break;
    }
    AL_SetMotionSpd(tp, 1.6f);
    return BHV_RET_CONTINUE;
}

int ALBHV_TurnUnused(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    chaowk* work = GET_CHAOWK(tp);

    switch (bhv->Mode) {
        case MD_TURN_INIT:
            AL_SetMotionLinkStep(tp, ALM_ARUKU, 15);

            if (njRandom() < 0.5f)
                bhv->Mode = MD_TURN_LEFT;
            else
                bhv->Mode = MD_TURN_RIGHT;

            bhv->Timer = 90;
            break;
        case MD_TURN_LEFT:
            if (bhv->Timer-- > 0) {
                work->ang.y += 364;
            } else {
                return BHV_RET_FINISH;
            }
            break;
        case MD_TURN_RIGHT:
            if (bhv->Timer-- > 0) {
                work->ang.y -= 364;
            } else {
                return BHV_RET_FINISH;
            }
            break;
    }
    AL_SetMotionSpd(tp, 1.6f);
    return BHV_RET_CONTINUE;
}

int ALBHV_TurnRandom(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    chaowk* work = GET_CHAOWK(tp);

    switch (bhv->Mode) {
        case MD_TURN_INIT:
            AL_SetMotionLinkStep(tp, ALM_ARUKU, 15);

            if (njRandom() < 0.5f)
                bhv->Mode = MD_TURN_LEFT;
            else
                bhv->Mode = MD_TURN_RIGHT;

            bhv->Timer = RAND_RANGE(0, 90);
            break;
        case MD_TURN_LEFT:
            if (bhv->Timer-- > 0) {
                work->ang.y += 364;
            } else {
                return BHV_RET_FINISH;
            }
            break;
        case MD_TURN_RIGHT:
            if (bhv->Timer-- > 0) {
                work->ang.y -= 364;
            } else {
                return BHV_RET_FINISH;
            }
            break;
    }
    AL_SetMotionSpd(tp, 1.6f);
    return BHV_RET_CONTINUE;
}

int ALBHV_GoToLockOn(task* tp) {
    enum { MD_GTLO_WALK, MD_GTLO_WALK2, MD_GTLO_ARRIVE };

    chaowk* work = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &work->Behavior;
    float dist;

    switch (bhv->Mode) {
        case MD_GTLO_WALK:
            AL_FaceChangeEye(tp, AL_EYE_NUM_NIKO);

            bhv->LimitTimer = 1800;

            if (bhv->Timer++ > 30) {
                AL_SetMotionLink(tp, ALM_ARUKU);
                ++bhv->Mode;
            }
            break;
        case MD_GTLO_WALK2:
            ALW_TurnToLockOn(tp, 384);
            AL_ForwardAcc(tp, GET_GLOBAL()->WalkAcc * 0.8f);

            dist = ALW_CalcDistFromLockOn(tp);
            if (dist < 0.0) {
                AL_SetBehavior(tp, ALBHV_Think);

            } else if (dist < 4.0) {
                return BHV_RET_FINISH;
            }
    }

    AL_SetMotionSpd(tp, 1.5);

    if (--bhv->LimitTimer <= 0) {
        AL_SetBehavior(tp, ALBHV_Think);
    }
    return BHV_RET_CONTINUE;
}

extern task* AL_ChildFieldCreateT(task* pParentTask, unsigned int kind, NJS_POINT3* pPos, float radius, int time);
int ALBHV_Touch(task* tp) {
    chaowk* work = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &work->Behavior;
    float dist;

    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_MOTU);
            ++bhv->Mode;
            bhv->Timer = 0;
        case 1:
            if (bhv->Timer++ > 60) {
                NJS_VECTOR position;

                position.x = njSin(work->ang.y) * 3.0f + work->pos.x;
                position.y = work->pos.y + 1.0f;
                position.z = njCos(work->ang.y) * 3.0f + work->pos.z;

                AL_ChildFieldCreateT(tp, CI_KIND_AL_TOUCH, &position, 3.5f, 10);
                return BHV_RET_FINISH;
            }
            break;
    }

    AL_Brake(tp, 0.9f);
    return BHV_RET_CONTINUE;
}