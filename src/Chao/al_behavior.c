#include "Chao/Chao.h"

int ALBHV_OdekakeEject(task* tp) {
    chaowk* cwk = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &cwk->Behavior;

    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_STAND);
            bhv->Mode++;
            GET_CHAOWK(tp)->ChaoFlag &= ~CHAO_FLAG_CCL_ENABLE;
            break;
    }

    if (bhv->Timer++ > 60) {
        GET_CHAOWK(tp)->ChaoFlag |= CHAO_FLAG_CCL_ENABLE;
        AL_SetBehavior(tp, ALBHV_Think);
        return BHV_RET_FINISH;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_OsanpoThrown(task* tp) {
    chaowk* cwk = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &cwk->Behavior;

    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_STAND);
            bhv->Mode++;
            GET_CHAOWK(tp)->ChaoFlag &= ~CHAO_FLAG_CCL_ENABLE;
            break;
    }

    return BHV_RET_CONTINUE;
}

void AL_BehaviorResetParameter(task* tp) {
    chaowk* cwk = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &cwk->Behavior;

    cwk->pitch = 0;

    AL_RemoveItemAll(tp);

    if (bhv->BhvFuncList[0] != bhv->PrevFunc) {
        AL_FaceReturnDefaultEye(tp);
        AL_FaceReturnDefaultMouth(tp);
    }
    AL_IconReset(tp);

    if (cwk->pAnyTask) {
        FreeTask(cwk->pAnyTask);
        cwk->pAnyTask = 0;
    }

    if (cwk->pBookTask) {
        FreeTask(cwk->pBookTask);
        cwk->pBookTask = 0;
    }

    GET_CHAOWK(tp)->ChaoFlag |= CHAO_FLAG_CCL_ENABLE;
    AL_EnablePushout(tp);
    AL_ReleasePosition(tp);
    AL_EnableTouch(tp);
    GET_CHAOWK(tp)->ChaoFlag |= CHAO_FLAG_MOVE_ENABLE;
}

BHV_FUNC AL_GetBehavior(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    return bhv->BhvFuncList[bhv->CurrBhvFuncNum];
}

void AL_ResetBehavior(AL_BEHAVIOR* bhv) {
    bhv->Mode = 0;
    bhv->SubMode = 0;
    bhv->Timer = 0;
    bhv->SubTimer = 0;
    bhv->LimitTimer = 3600;
}

void AL_SetBehavior(task* tp, BHV_FUNC Func) {
    AL_SetBehaviorWithTimer(tp, Func, -1);
}
void AL_SetBehaviorWithTimer(task* tp, BHV_FUNC Func, int ReserveTimer) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    int i;

    AL_ResetBehavior(bhv);

    bhv->PrevFunc = bhv->BhvFuncList[bhv->CurrBhvFuncNum];

    for (i = 0; i < 16; i++)
        bhv->BhvFuncList[i] = 0;

    bhv->nbBhvFuncEntry = 1;
    bhv->CurrBhvFuncNum = 0;
    bhv->BhvFuncList[0] = Func;
    bhv->ReserveTimerList[0] = ReserveTimer;
    bhv->FreeWork = 0;
    ALW_SendCommand(tp, 3);
    AL_BehaviorResetParameter(tp);
}

void AL_SetBehaviorWithFreeWork(task* tp, BHV_FUNC Func, int info) {
    AL_SetBehavior(tp, Func);
    AL_BehaviorSetFreeWork(tp, info);
}

void AL_SetNextBehavior(task* tp, BHV_FUNC Func) {
    AL_SetNextBehaviorWithTimer(tp, Func, -1);
}

void AL_SetNextBehaviorWithTimer(task* tp, BHV_FUNC Func, int ReserveTimer) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    if (bhv->nbBhvFuncEntry < 15) {
        bhv->BhvFuncList[bhv->nbBhvFuncEntry] = Func;
        bhv->ReserveTimerList[bhv->nbBhvFuncEntry] = ReserveTimer;
        bhv->nbBhvFuncEntry++;
    }
}

Bool sub_C500F50(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    AL_ResetBehavior(bhv);

    bhv->PrevFunc = bhv->BhvFuncList[bhv->CurrBhvFuncNum];
    bhv->CurrBhvFuncNum++;
    --bhv->nbBhvFuncEntry;

    if (bhv->BhvFuncList[bhv->CurrBhvFuncNum])
        return TRUE;

    return FALSE;
}

void AL_SetBehaviorInterrupt(task* tp, BHV_FUNC Func) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    AL_SetBehavior(tp, Func);
    ALW_CommunicationOff(tp);

    bhv->InterruptFlag = TRUE;
}

void AL_ReturnBehavior(task* tp) {
    int a[3]; // stack padding, not sure what it would've been used for
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    BHV_FUNC func;

    AL_ResetBehavior(bhv);
    func = bhv->BhvFuncList[bhv->CurrBhvFuncNum];
    bhv->BhvFuncList[bhv->CurrBhvFuncNum] = bhv->PrevFunc;
    bhv->PrevFunc = func;
    bhv->FreeWork = 0;
    ALW_SendCommand(tp, ALW_CMD_CHANGE);

    AL_BehaviorResetParameter(tp);

    bhv->InterruptFlag = FALSE;
}

void AL_BehaviorSetFreeWork(task* tp, int info) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    bhv->FreeWork = info;
}

int AL_BehaviorGetFreeWork(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    return bhv->FreeWork;
}

void AL_BehaviorInit(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    int i;

    bhv->Mode = 0;
    bhv->Timer = 0;
    bhv->SubTimer = 0;
    bhv->MoveRadius = 80;
    bhv->BasePos = GET_CHAOWK(tp)->pos;

    AL_SetBehavior(tp, ALBHV_Think);

    for (i = 0; i < 18; i++)
        bhv->IntentionTimer[i] = 0;
}

void AL_BehaviorExit(task* tp) {
    int a[3];
}

int ALBHV_Mayu(task* tp);
int ALBHV_Dead(task* tp);
int ALBHV_Sleep(task* tp);
int ALBHV_SitSleep(task* tp);
void AL_BehaviorCheckInterrupt(task* tp) {
    bool found_interrupt = FALSE;
    BHV_FUNC func = AL_GetBehavior(tp);
    if (func == ALBHV_Mayu || func == ALBHV_Dead) {
        return;
    }

    if (AL_CheckWater(tp)) {
        found_interrupt = TRUE;
    } else if (func != ALBHV_Sleep && func != ALBHV_SitSleep) {
        if (AL_CheckWhistle(tp)) {
            found_interrupt = TRUE;
        }
        if (AL_CheckBall(tp)) {
            found_interrupt = TRUE;
        }
        if (AL_CheckObakeHead(tp)) {
            found_interrupt = TRUE;
        }
        if (AL_CheckSeed(tp)) {
            found_interrupt = TRUE;
        }
        if (AL_CheckFruit(tp)) {
            found_interrupt = TRUE;
        }
        if (AL_CheckMinimal(tp)) {
            found_interrupt = TRUE;
        }
        if (AL_CheckBikkuri(tp)) {
            found_interrupt = TRUE;
        }
    }

    if (AL_CheckDakko(tp)) {
        found_interrupt = TRUE;
    } else if (AL_CheckNade(tp)) {
        found_interrupt = TRUE;
    }
    if (AL_CheckDamage(tp)) {
        found_interrupt = TRUE;
    }

    if (found_interrupt) {
        ALW_AttentionOff(tp);
        ALW_CommunicationOff(tp);
    }
}

void AL_BehaviorUnknown(task* tp) {
}

void AL_BehaviorControl(task* tp) {
    chaowk* work = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &work->Behavior;

    if (work->ChaoFlag & CHAO_FLAG_BHV_ENABLE) {
        int bhvret;

        if (work->ChaoFlag & CHAO_FLAG_RACE_BHV_ENABLE) {
            AL_BehaviorCheckInterrupt(tp);
            if (bhv->BhvFuncList[bhv->CurrBhvFuncNum] != 0) {
                bhvret = bhv->BhvFuncList[bhv->CurrBhvFuncNum](tp);
            } else {
                bhvret = BHV_RET_FINISH;
            }

            if (bhvret == BHV_RET_FINISH) {
                if (!sub_C500F50(tp)) {
                    AL_SetBehavior(tp, ALBHV_Think);
                }
            } else if (bhvret == BHV_RET_BREAK) {
                AL_SetBehavior(tp, ALBHV_Think);
            }
            return;
        }

        AL_IntentionCheckInterrupt(tp);
        AL_IntervalTimerControl(tp);
        AL_BehaviorCheckInterrupt(tp);

        if (bhv->BhvFuncList[bhv->CurrBhvFuncNum] != NULL) {
            bhvret = bhv->BhvFuncList[bhv->CurrBhvFuncNum](tp);
        } else {
            bhvret = BHV_RET_FINISH;
        }

        if (bhv->ReserveTimerList[bhv->CurrBhvFuncNum] > 0) {
            bhv->ReserveTimerList[bhv->CurrBhvFuncNum]--;
            if (!bhv->ReserveTimerList[bhv->CurrBhvFuncNum]) {
                bhvret = BHV_RET_FINISH;
            }
        }

        if (bhvret == BHV_RET_THINK) {
            if (!AL_GetIntention(tp)) {
                AL_DecideIntention(tp);
            } else {
                if (bhv->InterruptFlag) {}
            }
            AL_DecideBehavior(tp, AL_GetIntention(tp));
        } else if (bhvret == BHV_RET_FINISH) {
            if (!sub_C500F50(tp)) {
                AL_SetBehavior(tp, ALBHV_Think);
            }
        } else if (bhvret == BHV_RET_BREAK) {
            AL_SetBehavior(tp, ALBHV_Think);
        }
    }
}