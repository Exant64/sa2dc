#include <task.h>
#include <Chao/Chao.h>

// used in that unused lbl function
enum {
    ATTR_HERO,
    ATTR_DARK,
    ATTR_NEUT
};

typedef struct GROW_PARAM
{
    Sint16 addexp[4];
} GROW_PARAM;

extern GROW_PARAM MinimalGrowParam[24];

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

int lbl_0C52A256() {
    switch (GetCharID(0)){
        case 0:
        case 2:
        case 4:
        case 8:
            return ATTR_HERO;
        case 1:
        case 3:
        case 5:
            return ATTR_DARK;
        default:
            return ATTR_NEUT;
    }
}

void CaptureEffectManager(task *tp){
    taskwk* work = tp->twp;
    task* parent = tp->ptp;
    taskwk* parentWork = parent->twp;

    if(work->mode == 0){
        work->pos.y = 0;
        work->mode++;
    }
    
    work->wtimer++;

    {
        NJS_POINT3 pos;    
        const NJS_VECTOR velo = {0,0,0};

        pos.x = njSin(work->ang.y) * 2.3f + parentWork->pos.x;
        pos.y = parentWork->pos.y + work->pos.y;
        pos.z = njCos(work->ang.y) * 2.3f + parentWork->pos.z;
        lbl_0C541720(&pos, &velo, 1.0f);
        
        pos.x = njSin(-work->ang.y) * 2.3f + parentWork->pos.x;
        pos.y = parentWork->pos.y + work->pos.y;
        pos.z = njCos(-work->ang.y) * 2.3f + parentWork->pos.z;
        lbl_0C541720(&pos, &velo, 1.0f);

        if (work->wtimer > 80) {
            FreeTask(tp);
        }
    
        work->ang.y += 0x1555;
        work->pos.y += 0.06f;
    }
}

void CreateCaptureEffectManager(task* ptp){
    CreateChildTask(2, CaptureEffectManager, ptp);
}

int ALBHV_Capture(task* tp) {
    enum {
        MD_INIT,
        MD_SURISURI,
        MD_KURUN
    };

    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;  

    switch(bhv->Mode) {
        case MD_INIT:
            AL_SetMotionLink(tp, ALM_CAPTURE_SURISURI);
            
            bhv->Mode = MD_SURISURI;
            bhv->SubTimer = (Uint16)(njRandom() * 200.0f) + 90;
            
            sub_8C05B020 (0x1004, 0,0,0, &GET_CHAOWK(tp)->pos);
            
            CreateCaptureEffectManager(tp);
            // ParamFukidasiExecutor create here on SA2B onwards
        case MD_SURISURI:
            if(!AL_MoveHoldingObject(tp)) return BHV_RET_FINISH;

            if(AL_IsMotionEnd(tp)){
                al_entry_work* pMinimalEntry = ALW_IsCommunication(tp);
                
                AL_SetMotionLink(tp, ALM_CAPTURE_KURUN);
                
                bhv->Mode = MD_KURUN;
                
                sub_8C05B020(4101, 0, 0, 0, &GET_CHAOWK(tp)->pos);

                {
                    taskwk* twk = tp->twp;
                    int i;

                    // this was really weird
                    // using Uint32 for int mostly fixed it
                    // but the comparison instruction was wrong
                    // whether its 65535 or NJM_DEG_ANG(360) also affects compilation
                    // and so does using Angle or int, prob because Angle is long
                    if(0);
                    
                    for(i = 0; i < 40; i ++) {
                        NJS_POINT3 pos;
                        NJS_VECTOR velo;
                        
                        Angle ang = i * NJM_DEG_ANG(360 / 40);
                        
                        pos.x = twk->pos.x;
                        pos.y = twk->pos.y + 0.5f + njRandom() * 0.3f;
                        pos.z = twk->pos.z;

                        velo.x = njSin(ang) * (0.5f * 1.0f);
                        velo.y = (njRandom() * 0.5f) + 0.2f;
                        velo.z = njCos(ang) * 0.5f;

                        lbl_0C541720(&pos, &velo, 1.0f);
                    }
                }

                if(pMinimalEntry)  {
                    NJS_VECTOR vec;
                    
                    task* pMinimal = pMinimalEntry->tp;
                    NJS_POINT3* pPos = &tp->twp->pos;
                    NJS_POINT3* pMiniPos = &pMinimal->twp->pos;
                    int type = pMinimal->twp->id;

                    int i;

                    pMinimal->twp->ang.z = 1;

                    vec.x = pMiniPos->x - pPos->x;
                    vec.y = 0;
                    vec.z = pMiniPos->z - pPos->z;
                    njUnitVector(&vec);

                    vec.x *= 0.4f;
                    vec.y = 0.85f;
                    vec.z *= 0.4f;
                    MOV_SetVelo(pMinimal, &vec);

                    if(type < eMinimalType__end) {
                        for(i = 0; i < NB_PARTS_KIND; ++i) {
                            AL_PartsMinimalFlagOn(tp, type);
                            
                            if(GET_CHAOPARAM(tp)->type == TYPE_CHILD)  {
                                if(AL_IsExistPartsChild(type, i)) {
                                    if(njRandom() < GET_GLOBAL()->PartsSetProb) {
                                        AL_SetMiminalParts(tp, i, type);
                                    }
                                }
                                else {
                                    if(njRandom() < GET_GLOBAL()->PartsRemoveProb) {
                                        AL_RemoveMinimalParts(tp, i);
    
                                        if(i == PARTS_KIND_REG && type != eMinimalType_Batman) {
                                            GET_CHAOPARAM(tp)->body.ObakeBody = OBAKE_BODY_PARTS_NONE;
                                        }
                                    }
                                }
                            }
                            else {
                                if(AL_IsExistPartsAdult(type, i)) {
                                    if(njRandom() < GET_GLOBAL()->PartsSetProb) {
                                        AL_PartsMinimalFlagOn(tp, type);
                                        AL_SetMiminalParts(tp, i, type);
                                    }
                                }
                                else {
                                    if(njRandom() < GET_GLOBAL()->PartsRemoveProb) {
                                        AL_RemoveMinimalParts(tp, i);
    
                                        if(i == PARTS_KIND_REG && type != eMinimalType_Batman) {
                                            GET_CHAOPARAM(tp)->body.ObakeBody = OBAKE_BODY_PARTS_NONE;
                                        }
                                    }
                                }
                            }
                        }

                        {
                            int part = AL_GetMinimalPart(tp, PARTS_KIND_HORN);
                            
                            if(part == eMinimalType_Dra) {
                                int part = AL_GetMinimalPart(tp, PARTS_KIND_EAR);
                                if(part != -1 && part != eMinimalType_Dra) {
                                    AL_RemoveMinimalParts(tp, PARTS_KIND_EAR);
                                }
                            }
                            
                            if(part == eMinimalType_Shep) {
                                int part = AL_GetMinimalPart(tp, PARTS_KIND_EAR);
                                if(part != -1 && part != eMinimalType_Shep) {
                                    AL_RemoveMinimalParts(tp, PARTS_KIND_EAR);
                                }
                            }
                        }
                    }
                    
                    if(njRandom() < GET_GLOBAL()->PartsRemoveProb && type != eMinimalType_Han) {
                        GET_CHAOPARAM(tp)->body.HonbuNum = HONBU_NORMAL;
                    }

                    switch(type) {
                        case eMinimalType_Han:
                            if(njRandom() < GET_GLOBAL()->PartsSetProb) {
                                AL_PartsMinimalFlagOn(tp, type);

                                if(GET_CHAOPARAM(tp)->type < TYPE_N_CHAOS) {
                                    GET_CHAOPARAM(tp)->body.HonbuNum = HONBU_FIRE_OBAKE;
                                }
                            }
                            break;
                        case eMinimalType_Bone:
                            AL_PartsMinimalFlagOn(tp, type);
                            break;
                        case eMinimalType_Batman:
                            if(njRandom() < GET_GLOBAL()->PartsSetProb) {
                                AL_PartsMinimalFlagOn(tp, type);

                                GET_CHAOPARAM(tp)->body.ObakeBody = OBAKE_BODY_PARTS_SPECTOR;
                            }
                            break;
                    }

                    {
                        // unused var on SA2B onwards
                        GROW_PARAM* param = &MinimalGrowParam[type];
                        Uint8 chaoType = GET_CHAOPARAM(tp)->type;
                        float form;
                        float attr;
                        
                        if(chaoType == TYPE_CHILD) {
                            form = GET_GLOBAL()->MinimalAddFormChild;
                            attr = GET_GLOBAL()->MinimalAddAttrChild;
                        }
                        else {
                            form = GET_GLOBAL()->MinimalAddFormAdult;
                            attr = GET_GLOBAL()->MinimalAddAttrAdult;
                        }

                        AL_ParameterAddUserLike(tp, GET_GLOBAL()->MinimalAddUserLike, attr);

                        switch(type) {
                            case eMinimalType_Pen:
                            case eMinimalType_Goma:
                            case eMinimalType_Rako:
                                AL_ParameterAddVPos(tp, -form);
                                break;
                            case eMinimalType_Usa:
                            case eMinimalType_Chit:
                            case eMinimalType_Ino:
                                AL_ParameterAddHPos(tp, -form);
                                break;
                            case eMinimalType_Kuma:
                            case eMinimalType_Tora:
                            case eMinimalType_Gori:
                                AL_ParameterAddHPos(tp, form);
                                break;
                            case eMinimalType_Kuja:
                            case eMinimalType_Oum:
                            case eMinimalType_Kon:
                                AL_ParameterAddVPos(tp, form);
                                break;
                            case eMinimalType_Ska:
                            case eMinimalType_Shep:
                            case eMinimalType_Ara:
                            case eMinimalType_Han:
                            case eMinimalType_Bone:
                            case eMinimalType_Batman:
                            case eMinimalType_Dra:
                            case eMinimalType_Uni:
                            case eMinimalType_Phen: {
                                float randval = njRandom();
                                
                                if(randval < 0.25f) {
                                    AL_ParameterAddHPos(tp, form);
                                }
                                else if(randval < 0.5f) {
                                    AL_ParameterAddHPos(tp, -form);
                                }
                                else if(randval < 0.75f) {
                                    AL_ParameterAddVPos(tp, form);
                                }
                                else {
                                    AL_ParameterAddVPos(tp, -form);
                                }
                                break;
                            }

                            default: {
                                Uint8 driveType = type - eMinimalType__end;
                                
                                switch(driveType) {
                                    case 0:
                                        AL_ParameterAddVPos(tp, -form * 0.5f);
                                        break;
                                    case 1:
                                        AL_ParameterAddHPos(tp, -form * 0.5f);
                                        break;
                                    case 2:
                                        AL_ParameterAddHPos(tp, form * 0.5f);
                                        break;
                                    case 3:
                                        AL_ParameterAddVPos(tp, form * 0.5f);
                                        break;
                                }
                                break;
                            }
                        }

                        // from this point on, this is completely different on sa2b onwards
                        // it just uses ParameterGrow
                        AL_ParameterAddSkill(tp, SKILL_SWIM, param->addexp[SKILL_SWIM]);
                        AL_ParameterAddSkill(tp, SKILL_FLY, param->addexp[SKILL_FLY]);
                        AL_ParameterAddSkill(tp, SKILL_RUN, param->addexp[SKILL_RUN]);
                        AL_ParameterAddSkill(tp, SKILL_POWER, param->addexp[SKILL_POWER]);

                        switch(chaoType) {
                            case TYPE_N_NORMAL:
                            case TYPE_H_NORMAL:
                            case TYPE_D_NORMAL: {
                                float randval = njRandom() * 3.999f;

                                if(randval < 1) {
                                    AL_ParameterAddSkill(tp, SKILL_SWIM, param->addexp[SKILL_SWIM]);
                                }
                                else if(randval < 2) {
                                    AL_ParameterAddSkill(tp, SKILL_FLY, param->addexp[SKILL_FLY]);
                                }
                                else if(randval < 3) {
                                    AL_ParameterAddSkill(tp, SKILL_RUN, param->addexp[SKILL_RUN]);
                                }
                                else if(randval < 4) {
                                    AL_ParameterAddSkill(tp, SKILL_POWER, param->addexp[SKILL_POWER]);
                                }
                                
                                break;
                            }
                            case TYPE_N_SWIM:
                            case TYPE_H_SWIM:
                            case TYPE_D_SWIM:
                                AL_ParameterAddSkill(tp, SKILL_SWIM, param->addexp[SKILL_SWIM]);
                                break;
                            case TYPE_N_FLY:
                            case TYPE_H_FLY:
                            case TYPE_D_FLY:
                                AL_ParameterAddSkill(tp, SKILL_FLY, param->addexp[SKILL_FLY]);
                                break;
                            case TYPE_N_RUN:
                            case TYPE_H_RUN:
                            case TYPE_D_RUN:
                                AL_ParameterAddSkill(tp, SKILL_RUN, param->addexp[SKILL_RUN]);
                                break;
                            case TYPE_N_POWER:
                            case TYPE_H_POWER:
                            case TYPE_D_POWER: 
                                AL_ParameterAddSkill(tp, SKILL_POWER, param->addexp[SKILL_POWER]);
                                break;
                            default:
                                break;
                        }
                    }
                }
                
                ALW_CommunicationOff(tp);
            }
            break;
        case MD_KURUN:
            if (AL_IsMotionStop(tp)) {
                return BHV_RET_FINISH;
            }
            break;
    }

    return BHV_RET_CONTINUE;
}