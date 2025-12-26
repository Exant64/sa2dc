#include <Chao/Chao.h>
#include <Chao/al_landmark.h>

#include <playsound.h>

int ALBHV_Climb(task*);
int ALBHV_Turn(task*);
int ALBHV_JumpToPond(task*);
int ALBHV_Glide(task*);
int ALBHV_Koke(task*);
int ALBHV_PostureChangeSit(task *tp);
int ALBHV_PostureChangeStand(task *tp);

int WalkControl(task *tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    taskwk* twk = tp->twp;
    MOVE_WORK* move = tp->Data2;

    if(AL_Dist2FromAim(tp) < 36.f) return TRUE;

    if(move->Flag & 0x4000) {
        if (CCL_IsHitKind2(tp, CI_KIND_AL_CLIMB)) {
            AL_SetBehavior(tp, ALBHV_Climb);
            twk->pos.y += 0.1f;
        }
        else {
            AL_SetBehavior(tp, ALBHV_Turn);
        }
    }

    if(twk->wtimer++ > 30 && AL_IsHitKindWithNum(tp, 1, CI_KIND_AL_FURIMUKI)) {
        AL_SetBehavior(tp, ALBHV_Turn);
    }

    MOV_TurnToAim2(tp, 288);

    if(move->Flag & 0x400) {
        const NJS_VECTOR up = {0, 1, 0};
        const float dot = njInnerProduct(&move->Shadow.hit[2].normal, &up);

        if(dot < 0.7f) {
            move->Velo.y += 0.55f;
            AL_SetBehavior(tp, ALBHV_JumpToPond);
        }
    }
    else if (twk->pos.y - move->Shadow.hit[2].onpos > 2) {
        move->Velo.y += 0.55f;
        if(AL_GetCurrLandAttr(&move->AimPos) == LMA_WATER) {
            AL_SetBehavior(tp, ALBHV_JumpToPond);
        }
        else if(AL_ParameterGetSkill(tp, SKILL_FLY) < 400 || njRandom() < 0.6f) {
            AL_SetBehavior(tp, ALBHV_JumpToPond);
        }
        else {
            AL_SetBehavior(tp, ALBHV_Glide);
        }
    }

    if (AL_IsEmotionTimerReset(tp)) {
        AL_EmotionAdd(tp, EM_ST_TEDIOUS, -GET_GLOBAL()->ParamSubTediousWalk);
        AL_EmotionAdd(tp, EM_ST_TIRE, GET_GLOBAL()->ParamAddTireWalk);
    }

    return FALSE;
}

int ALBHV_HaiHai(task *tp ) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    taskwk* twk = tp->twp;
    Uint16 skill = AL_ParameterGetSkill(tp, SKILL_RUN);
    MOVE_WORK* move = tp->Data2;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_HAIHAI);
            bhv->Mode++;
            break;
        case 1:
            break;
    }

    if(!(GET_CHAOWK(tp)->Timer % 120) && njRandom() < 0.3f) {
        if(njRandom() < 0.5f) {
            sub_8C05B020(0x6014, 0, 0, 110, &GET_CHAOWK(tp)->pos);
        }
        else {
            sub_8C05B020(0x6015, 0, 0, 110, &GET_CHAOWK(tp)->pos);
        }
    }

    if(move->Flag & 0x400) {
        AL_ForwardAcc(tp, GET_GLOBAL()->WalkSlowAcc);
    }

    return WalkControl(tp);
}

int ALBHV_WalkNormal(task *tp ) {
    taskwk* twk = tp->twp;
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    Uint16 skill = AL_ParameterGetSkill(tp, SKILL_RUN);
    MOVE_WORK* move = tp->Data2;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_ARUKU);
            bhv->Mode++;
            break;
        case 1:
            break;
    }

    if((bhv->Timer++ % 85) == 1) {
        if(njRandom() < 0.03f) {
            AL_SetBehavior(tp, ALBHV_Koke);
        }
    }

    if(move->Flag & 0x400) {
        AL_ForwardAcc(tp, GET_GLOBAL()->WalkAcc);
    }

    {
        float velo = njScalor(&move->Velo);
        AL_SetMotionSpd(tp, velo * 16);
    }
    
    return WalkControl(tp);
}

int ALBHV_Run(task* tp) {
    taskwk* twk = tp->twp;
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    Uint16 skill = AL_ParameterGetSkill(tp, SKILL_RUN);
    MOVE_WORK* move = tp->Data2;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_HASIRU);
            bhv->Mode++;
            break;
        case 1:
            break;
    }

    if(!(bhv->Timer++ % 100)) {
        if(njRandom() < 0.02f) {
            AL_SetBehavior(tp, ALBHV_Koke);
        }
    }

    if(move->Flag & 0x400) {
        const float acc = GET_GLOBAL()->SkillRunAccBase + skill * GET_GLOBAL()->SkillRunAccRatio;
        
        move->Acc.x = njSin(twk->ang.y) * acc;
        move->Acc.z = njCos(twk->ang.y) * acc;
    }

    {
        float velo = njScalor(&move->Velo);
        AL_SetMotionSpd(tp, velo * 14);
    }
    
    return WalkControl(tp);
}

int ALBHV_Run2(task* tp) {
    taskwk* twk = tp->twp;
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    Uint16 skill = AL_ParameterGetSkill(tp, SKILL_RUN);
    MOVE_WORK* move = tp->Data2;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_HASIRU_HIROGE);
            bhv->Mode++;
            break;
        case 1:;
    }

    if(move->Flag & 0x400) {
        const float acc = GET_GLOBAL()->SkillRunAccBase + skill * GET_GLOBAL()->SkillRunAccRatio;
        
        move->Acc.x = njSin(twk->ang.y) * acc;
        move->Acc.z = njCos(twk->ang.y) * acc;
    }

    {
        float velo = njScalor(&move->Velo);
        AL_SetMotionSpd(tp, velo * 14);
    }
    
    return WalkControl(tp);
}

void AL_WalkSelect(task *tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    taskwk* twk = tp->twp;
    Uint16 skill = AL_ParameterGetSkill(tp, SKILL_RUN);
    MOVE_WORK* move = tp->Data2;

    if(skill < GET_GLOBAL()->SkillWalk) {
        AL_SetBehavior(tp, ALBHV_PostureChangeSit);
        AL_SetNextBehavior(tp, ALBHV_HaiHai);
    }
    else if(skill < GET_GLOBAL()->SkillRun) {
        AL_SetBehavior(tp, ALBHV_PostureChangeStand);
        AL_SetNextBehavior(tp, ALBHV_WalkNormal);
    }
    else if(skill < GET_GLOBAL()->SkillRun2) {
        AL_SetBehavior(tp, ALBHV_PostureChangeStand);
        AL_SetNextBehavior(tp, ALBHV_Run);
    }
    else {
        AL_SetBehavior(tp, ALBHV_PostureChangeStand);
        AL_SetNextBehavior(tp, ALBHV_Run2);
    }
}

int ALBHV_WalkSelect(task *tp) {
    AL_WalkSelect(tp);
    return 0;
}