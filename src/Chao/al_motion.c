#include "Chao/Chao.h"

extern MOTION_TABLE al_motion_table[];

void AL_MotionInit(task* tp) {
    chaowk* cwk = GET_CHAOWK(tp);
    MotionInit(&cwk->MotionCtrl, al_motion_table);
}

void AL_MotionControl(task* tp) {
    MOTION_CTRL* Ctrl = &GET_CHAOWK(tp)->MotionCtrl;
    if (GET_CHAOWK(tp)->ChaoFlag & CHAO_FLAG_MOTION_ENABLE)
        MotionControl(Ctrl);
}

void AL_SetMotionLink(task* tp, int MtnNum) {
    MOTION_CTRL* Ctrl = &GET_CHAOWK(tp)->MotionCtrl;
    SetMotionLink(Ctrl, MtnNum);
}

void AL_SetMotionLinkStep(task* tp, int MtnNum, Uint16 step) {
    MOTION_CTRL* Ctrl = &GET_CHAOWK(tp)->MotionCtrl;
    SetMotionLinkStep(Ctrl, MtnNum, step);
}

void AL_SetMotionLinkSync(task* tp, int MtnNum) {
    MOTION_CTRL* Ctrl = &GET_CHAOWK(tp)->MotionCtrl;
    SetMotionLinkSync(Ctrl, MtnNum);
}

void AL_SetMotionSkip(task* tp, int MtnNum) {
    MOTION_CTRL* Ctrl = &GET_CHAOWK(tp)->MotionCtrl;
    SetMotionSkip(Ctrl, MtnNum);
}

void AL_SetMotionSpd(task* tp, float Spd) {
    MOTION_CTRL* Ctrl = &GET_CHAOWK(tp)->MotionCtrl;
    SetMotionSpd(Ctrl, Spd);
}

void AL_SetMotionFrame(task* tp, float Frame) {
    MOTION_CTRL* Ctrl = &GET_CHAOWK(tp)->MotionCtrl;
    SetMotionFrame(Ctrl, Frame);
}

void sub_C51231C(task* tp, float Frame) {
    MOTION_CTRL* Ctrl = &GET_CHAOWK(tp)->MotionCtrl;
    sub_8C033960(Ctrl, Frame);
}

float AL_GetMotionFrame(task* tp) {
    MOTION_CTRL* Ctrl = &GET_CHAOWK(tp)->MotionCtrl;
    return GetMotionFrame(Ctrl);
}

float sub_C512340(task* tp) {
    MOTION_CTRL* Ctrl = &GET_CHAOWK(tp)->MotionCtrl;
    return sub_8C033990(Ctrl);
}

int AL_GetMotionNum(task* tp) {
    MOTION_CTRL* Ctrl = &GET_CHAOWK(tp)->MotionCtrl;
    return GetMotionNum(Ctrl);
}

int AL_GetMotionPosture(task* tp) {
    MOTION_CTRL* Ctrl = &GET_CHAOWK(tp)->MotionCtrl;
    return GetMotionPosture(Ctrl);
}

int AL_IsMotionEnd(task* tp) {
    MOTION_CTRL* Ctrl = &GET_CHAOWK(tp)->MotionCtrl;
    return IsMotionEnd(Ctrl);
}

int AL_IsMotionStop(task* tp) {
    MOTION_CTRL* Ctrl = &GET_CHAOWK(tp)->MotionCtrl;
    return IsMotionStop(Ctrl);
}

void AL_SetMotionTable(task* tp, MOTION_TABLE* pTable) {
    MOTION_CTRL* Ctrl = &GET_CHAOWK(tp)->MotionCtrl;
    SetMotionTable(Ctrl, pTable);
}