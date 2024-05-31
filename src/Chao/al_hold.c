#include <Chao/Chao.h>
#include <Chao/al_hold.h>

void AL_GrabObjectLeftHand(task* pChaoTask, task* pObjTask) {
    chaowk* cwk = GET_CHAOWK(pChaoTask);

    ALW_CommunicationOn(pChaoTask, pObjTask);
    ALW_HeldOn(ALW_IsCommunication(pChaoTask));
    AL_MoveHoldingObject(pChaoTask);

    cwk->ChaoFlag |= CHAO_FLAG_HOLD_LEFT;
    cwk->ChaoFlag &= ~CHAO_FLAG_HOLD_RIGHT;
}

void AL_GrabObjectRightHand(task* pChaoTask, task* pObjTask) {
    chaowk* cwk = GET_CHAOWK(pChaoTask);

    ALW_CommunicationOn(pChaoTask, pObjTask);
    ALW_HeldOn(ALW_IsCommunication(pChaoTask));
    AL_MoveHoldingObject(pChaoTask);

    cwk->ChaoFlag &= ~CHAO_FLAG_HOLD_LEFT;
    cwk->ChaoFlag |= CHAO_FLAG_HOLD_RIGHT;
}

void AL_GrabObjectBothHands(task* pChaoTask, task* pObjTask) {
    chaowk* cwk = GET_CHAOWK(pChaoTask);

    ALW_CommunicationOn(pChaoTask, pObjTask);
    ALW_HeldOn(ALW_IsCommunication(pChaoTask));
    AL_MoveHoldingObject(pChaoTask);

    cwk->ChaoFlag |= CHAO_FLAG_HOLD_LEFT;
    cwk->ChaoFlag |= CHAO_FLAG_HOLD_RIGHT;
}

void AL_SetHeldFlagLeft(task* pChaoTask) {
    chaowk* cwk = GET_CHAOWK(pChaoTask);
    cwk->ChaoFlag |= CHAO_FLAG_HOLD_LEFT;
    cwk->ChaoFlag &= ~CHAO_FLAG_HOLD_RIGHT;
}

void AL_SetHeldFlagRight(task* pChaoTask) {
    chaowk* cwk = GET_CHAOWK(pChaoTask);
    cwk->ChaoFlag &= ~CHAO_FLAG_HOLD_LEFT;
    cwk->ChaoFlag |= CHAO_FLAG_HOLD_RIGHT;
}

void AL_SetHeldFlag(task* pChaoTask) {
    chaowk* cwk = GET_CHAOWK(pChaoTask);
    cwk->ChaoFlag |= CHAO_FLAG_HOLD_LEFT;
    cwk->ChaoFlag |= CHAO_FLAG_HOLD_RIGHT;
}

Bool AL_MoveHoldingObject(task* pChaoTask) {
    chaowk* cwk = GET_CHAOWK(pChaoTask);
    al_entry_work* pEntry;

    if (pEntry = ALW_IsCommunication(pChaoTask)) {
        AL_SHAPE* pShape = &GET_CHAOWK(pChaoTask)->Shape;
        NJS_POINT3* Left = &pShape->LeftHandPos;
        NJS_POINT3* Right = &pShape->RightHandPos;
        float rad = pEntry->radius;
        int ang = GET_CHAOWK(pChaoTask)->ang.y;
        float dist = njDistanceP2P(Left, Right);
        float sq;

        dist *= 0.5f;
        sq = njSqrt(rad * rad + dist * dist);

        ALW_HeldOn(pEntry);
        if (cwk->ChaoFlag & CHAO_FLAG_HOLD_LEFT) {
            if (cwk->ChaoFlag & CHAO_FLAG_HOLD_RIGHT) {
                pEntry->pos.x = (Left->x + Right->x) * 0.5 + sq * njSin(ang);
                pEntry->pos.y = (Left->y + Right->y) * 0.5;
                pEntry->pos.z = (Left->z + Right->z) * 0.5 + sq * njCos(ang);
            } else {
                pEntry->pos.x = Left->x + sq * njSin(ang);
                pEntry->pos.y = Left->y;
                pEntry->pos.z = Left->z + sq * njCos(ang);
            }
        } else if (cwk->ChaoFlag & CHAO_FLAG_HOLD_RIGHT) {
            pEntry->pos.x = Right->x + sq * njSin(ang);
            pEntry->pos.y = Right->y;
            pEntry->pos.z = Right->z + sq * njCos(ang);
        }

        pEntry->pos.y -= pEntry->offset;
        pEntry->ang.y = ang;
        return TRUE;
    }

    return FALSE;
}

Bool AL_TraceHoldingPosition(task* pObjTask) {
    chaowk* cwk = GET_CHAOWK(pObjTask);
    al_entry_work* pEntry = ALW_ENTRY_WORK(pObjTask);

    if (!ALW_IsHeld(pObjTask))
        return FALSE;

    if (ALW_IsCommunicationEx(pObjTask, 0)) {
        cwk->pos.x += (pEntry->pos.x - cwk->pos.x) * 0.3f;
        cwk->pos.y += (pEntry->pos.y - cwk->pos.y) * 0.3f;
        cwk->pos.z += (pEntry->pos.z - cwk->pos.z) * 0.3f;
        ALW_HeldOff(pEntry);
        return TRUE;
    }

    return FALSE;
}