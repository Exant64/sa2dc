#include "Chao/Chao.h"
#include "playsound.h"

#if 0
int ALBHV_Capture(task *tp);
int ALBHV_TurnToMinimal(task *tp);

int AL_CheckObakeHead(task *tp, float *pMaxScore) {
    BHV_FUNC func = AL_GetBehavior(tp);
    task* v2;
    if ( func == ALBHV_Capture || func == ALBHV_TurnToMinimal ) {
        return 0;
    }
    
    if ( (v2 = CCL_IsHitKind2(tp, CI_KIND_AL_MINIMAL)) && !ALW_IsHeld(v2) ) {
        MOV_SetAimPos(tp, &v2->twp->pos);
        AL_SetBehavior(tp, ALBHV_TurnToMinimal);
        return 1;
    }
    return 0;
}

int ALBHV_TurnToMinimal(task *tp){
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    task* pObake;

    switch(bhv->Mode)
    {
        case 0:
            AL_SetMotionLinkStep(tp, 202, 15);
            bhv->Mode++;
            break;
    }

    if ( MOV_TurnToAim2(tp, 1536) < 182 )
    {   
        task* pMinimal = CCL_IsHitKind2(tp, CI_KIND_AL_MINIMAL);
        if(pMinimal) {
            if(!ALW_IsHeld(pMinimal)) {
                StopHoldingTaskP(0);
                AL_GrabObjectBothHands(tp, pMinimal);
                AL_SetBehavior(tp, ALBHV_Capture);
            }
        }
        else {
            return 1;
        }

    }
    else{
        return 1;
    }

    return 0;
}

int sub_C52A256(void){
    switch (GetCharID()){
        case 0:
        case 2:
        case 4:
        case 8:
            return 0;
        case 1:
        case 3:
        case 5:
            return 1;
        default:
            return 2;
    }
}

static void CaptureEffectManager(task *tp){
    taskwk* work = tp->twp;
    taskwk* parentWork = tp->ptp->twp;

    if(work->mode == 0){
        work->pos.y = 0;
        work->mode++;
    }
    work->field_6++;

    {
        const NJS_VECTOR velo = {0,0,0};
        NJS_POINT3 pos;    

        pos.x = njSin(work->ang.y) * 2.3f + parentWork->pos.x;
        pos.y = parentWork->pos.y + work->pos.y;
        pos.z = njCos(work->ang.y) * 2.3f + parentWork->pos.z;
        sub_6EFF10(&pos, &velo, 1.0f);
        
        pos.x = njSin(-work->ang.y) * 2.3f + parentWork->pos.x;
        pos.y = parentWork->pos.y + work->pos.y;
        pos.z = njCos(-work->ang.y) * 2.3f + parentWork->pos.z;
        sub_6EFF10(&pos, &velo, 1.0f);

        if ( work->field_6 > 80 ) {
            FreeTask(tp);
        }
    
        work->ang.y += 0x1555;
        work->pos.y += 0.06f;
    }

}

static void CreateCaptureEffectManager(task* ptp){
    CreateChildTask(2, CaptureEffectManager, ptp);
}

void ALBHV_Capture (task *tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;  

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, 203);
            work->Behavior.Mode = 1;
            work->Behavior.SubTimer = (Uint16)(njRandom() * 200.0f) + 90;
            sub_8C05B020 (0x1004, 0,0,0, &GET_CHAOWK(tp)->pos);
            CreateCaptureEffectManager(tp);
        case 1:
            if(!AL_MoveHoldingObject(tp)) return 1;

            if(AL_IsMotionEnd(tp)){
                
            }
            break;
        case 2:
            if (AL_IsMotionStop(tp)) {
                return 1;
            }
            break;
    }

    return 0;
}
#endif