#include <task.h>
#include <Chao/Chao.h>

int ALBHV_Capture(task* tp);
int ALBHV_TurnToMinimal(task* tp);

Bool AL_CheckMinimal(task *tp, float *pMaxScore) {
    BHV_FUNC func = AL_GetBehavior(tp);
    task* v2;
    
    if ( func == ALBHV_Capture || func == ALBHV_TurnToMinimal ) {
        return FALSE;
    }
    
    if ( (v2 = CCL_IsHitKind2(tp, CI_KIND_AL_MINIMAL)) && !ALW_IsHeld(v2) ) {
        MOV_SetAimPos(tp, &v2->twp->pos);
        AL_SetBehavior(tp, ALBHV_TurnToMinimal);
        return TRUE;
    }

    return FALSE;
}

MERGE_LIST([['_AL_GetBehavior', '_lbl_0C52A2A0'], ['_ALBHV_Capture', '_lbl_0C52A2A4'], ['_ALBHV_TurnToMinimal', '_lbl_0C52A2A8'], ['_CCL_IsHitKind2', '_lbl_0C52A2AC'], ['_ALW_IsHeld', '_lbl_0C52A2B0'], ['_MOV_SetAimPos', '_lbl_0C52A2B4'], ['_AL_SetBehavior', '_lbl_0C52A2B8']]);

int ALBHV_TurnToMinimal(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    task* pObake;

    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_CAPTURE_MORAU, 15);
            bhv->Mode++;
            break;
    }

    if (MOV_TurnToAim2(tp, 0x600) < 0x71C) {
        task* pMinimal = CCL_IsHitKind2(tp, CI_KIND_AL_MINIMAL);
        if (pMinimal && !ALW_IsHeld(pMinimal)) {
            StopHoldingTaskP(0);
            AL_GrabObjectBothHands(tp, pMinimal);
            AL_SetBehavior(tp, ALBHV_Capture);
        }
        else {
            return BHV_RET_FINISH;
        }
    }

    return BHV_RET_CONTINUE;
}

MERGE_LIST([['_MOV_TurnToAim2', '_lbl_0C52A2C0'], ['_StopHoldingTaskP', '_lbl_0C52A2C4'], ['_ALBHV_Capture', '_lbl_0C52A2A4'], ['_AL_GrabObjectBothHands', '_lbl_0C52A2C8'], ['_CCL_IsHitKind2', '_lbl_0C52A2AC'], ['_ALW_IsHeld', '_lbl_0C52A2B0'], ['_AL_SetBehavior', '_lbl_0C52A2B8'], ['_AL_SetMotionLinkStep', '_lbl_0C52A2BC']]);
INLINE_ASM(_func_0C52A256, 0x98, "asm/nonmatching/Chao/al_behavior/albhv_capture/_func_0C52A256.src");

INLINE_ASM(_func_0C52A2EE, 0x102, "asm/nonmatching/Chao/al_behavior/albhv_capture/_func_0C52A2EE.src");

// MERGE_LIST([['_lbl_0C567250', '_lbl_0C52A440'], ["h'40133333", '_lbl_0C52A444'], ['_njSin', '_lbl_0C52A448'], ['_njCos', '_lbl_0C52A44C'], ['_lbl_0C541720', '_lbl_0C52A450'], ['_FreeTask', '_lbl_0C52A454'], ["h'3D75C28F", '_lbl_0C52A458']]);
INLINE_ASM(_func_0C52A3F0, 0xe, "asm/nonmatching/Chao/al_behavior/albhv_capture/_func_0C52A3F0.src");

// MERGE_LIST([['_CreateChildTask', '_lbl_0C52A460'], ['_lbl_0C52A2EE', '_lbl_0C52A45C']]);
INLINE_ASM(_ALBHV_Capture, 0xa42, "asm/nonmatching/Chao/al_behavior/albhv_capture/_ALBHV_Capture.src");

