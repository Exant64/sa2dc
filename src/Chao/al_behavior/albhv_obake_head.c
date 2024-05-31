#include "Chao/Chao.h"
#include "playsound.h"
#include <Chao/al_hold.h>

// forward decl
int ALBHV_Throw(task* tp);
int ALBHV_HoldThink(task* tp);
int ALBHV_TurnToObakeHead(task* tp);
int ALBHV_WearObakeHead(task* tp);

Bool AL_CheckObakeHead(task* tp, float* pMaxScore) {
    BHV_FUNC func = AL_GetBehavior(tp);
    task* obake;

    if (func == ALBHV_Eat || func == ALBHV_Sleep || func == ALBHV_TurnToObakeHead) {
        return 0;
    }

    if ((obake = CCL_IsHitKind2(tp, CI_KIND_AL_OBAKE_HEAD)) && !ALW_IsHeld(obake)) {
        MOV_SetAimPos(tp, &obake->twp->pos);
        AL_SetBehavior(tp, ALBHV_TurnToObakeHead);
        return TRUE;
    }

    return FALSE;
}

int ALBHV_TurnToObakeHead(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    task* pObake;

    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_CAPTURE_MORAU, 15);
            bhv->Mode++;
            bhv->Timer = 60;
            break;
    }

    if (MOV_TurnToAim2(tp, 0x600) < 0xB6) {
        task* pObake = CCL_IsHitKind2(tp, CI_KIND_AL_OBAKE_HEAD);
        if (pObake) {
            if (!ALW_IsHeld(pObake)) {
                StopHoldingTaskP(0);
                AL_GrabObjectBothHands(tp, pObake);
                AL_SetBehavior(tp, ALBHV_HoldThink);
                if (AL_PartsIsMinimalFlagOn(tp, 0x10) && !GET_CHAOPARAM(tp)->body.ObakeHead) {
                    AL_SetNextBehavior(tp, ALBHV_WearObakeHead);
                } else {
                    AL_SetNextBehavior(tp, ALBHV_Throw);
                }
            }
        } else if (bhv->Timer-- <= 0) {
            return BHV_RET_FINISH;
        }
    }
    return BHV_RET_CONTINUE;
}

int ALBHV_HoldThink(task* tp) {
    chaowk* work = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &work->Behavior;
    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_MOTU, 35);
            ++bhv->Mode;
            AL_IconSet(tp, MD_ICON_HATENA, 120);
            sub_8C05B020(24604, 0, 0, 110, &GET_CHAOWK(tp)->pos);
            bhv->Timer = RAND_RANGE(120, 180);
            break;
    }

    if (bhv->Timer-- > 0) {
        if (!AL_MoveHoldingObject(tp)) {
            return BHV_RET_FINISH;
        }
    } else {
        return BHV_RET_FINISH;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_WearObakeHead(task* tp) {
    chaowk* work = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &work->Behavior;
    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_KABURU, 35);
            ++bhv->Mode;
            AL_IconSet(tp, MD_ICON_BIKKURI, 120);
            break;
        case 1:
            if (AL_IsMotionStop(tp)) {
                al_entry_work* pEntry = ALW_IsCommunication(tp);
                if (pEntry) {
                    if (pEntry->tp) {
                        taskwk* work = pEntry->tp->twp;
                        AL_SetObakeHead(tp, work->ang.x);
                        FreeTask(pEntry->tp);
                    }
                }
                AL_SetMotionLinkStep(tp, ALM_KABURU_END, 35);
            }

            break;
        case 2:
            if (AL_IsMotionStop(tp)) {
                return BHV_RET_FINISH;
            }
            break;
    }

    if (!AL_MoveHoldingObject(tp)) {
        return BHV_RET_FINISH;
    }

    return BHV_RET_CONTINUE;
}
extern void test(task*);
int ALBHV_Throw(task* tp) {
    chaowk* work = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &work->Behavior;

    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_THROW, 35);
            ++bhv->Mode;
            break;
        case 1:
            if (AL_IsMotionEnd(tp)) {                            // 168
                al_entry_work* pEntry = ALW_IsCommunication(tp); // 169
                if (pEntry) {                                    // 170
                    task* ptp = pEntry->tp;
                    int what;
                    if (0)
                        test(ptp);
                    what = *(int*)&GET_CHAOWK(tp)->pos.x;
                    {
                        int ang = GET_CHAOWK(tp)->ang.y + 0x71C7; // 175
                        // 176
                        NJS_POINT3 velo;            // 173
                        velo.x = njSin(ang) * 0.3f; // 177
                        velo.y = 0.7f;              // 176
                        velo.z = njCos(ang) * 0.3f; // 179
                        // 180
                        MOV_SetVelo(ptp, &velo); // 181
                    }
                }
                ALW_CommunicationOff(tp);
                return BHV_RET_FINISH;
            }
            break;
    }

    if (!AL_MoveHoldingObject(tp)) {
        return BHV_RET_FINISH;
    }

    return BHV_RET_CONTINUE;
}