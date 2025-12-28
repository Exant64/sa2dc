#include <Chao/Chao.h>
#include <Chao/al_landmark.h>

#include <playsound.h>

int ALBHV_Swim(task*);
void SwimControl(task*);
void SwimControlStop(task*);

Bool AL_CheckWater(task *tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    MOVE_WORK* move = tp->Data2;

    AL_BehaviorUnknown(tp);
    
    if(GET_CHAOWK(tp)->flag & TWK_FLAG_HELD) {
        bhv->Flag &= ~1;
        return FALSE;
    }

    switch(AL_GetStageNumber()) {
        case 1:
            move->WaterY = -2.2f;
            break;
        case 2:
            move->WaterY = -1.5f;
            break;
        case 3:
            move->WaterY = -1;
            break;
        default:
            move->WaterY = -10000;
            break;
    }

    if(GET_CHAOWK(tp)->pos.y + 2 < move->WaterY) {
        if(!(bhv->Flag & 1)) {
            bhv->Flag |= 1;

            sub_8C05B020(0x1020, 0, 0, 0, &GET_CHAOWK(tp)->pos);
            AL_SetBehavior(tp, ALBHV_Swim);
        }

        if(move->Velo.y < 0) {
            move->Velo.y *= 0.1f;    
        }

        bhv->Flag |= 4;
        
        return TRUE;
    }

    bhv->Flag &= ~1;
    bhv->Flag &= ~4;
    
    return FALSE;
}

void AL_DecideAimSwimPosition(task* tp) {
    taskwk* work = tp->twp;
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    MOVE_WORK* move = tp->Data2;
    NJS_POINT3* pPos = &work->pos;
    int i = 0;
    NJS_POINT3 AimPos;

    if(AL_EmotionGetValue(tp, EM_ST_THIRSTY) < 100) {
        AL_GetRandomAttrPos(LMA_GROUND1, &move->AimPos);
    }
    else {
        AimPos.y = pPos->y;
    
        while(TRUE) {
            const float rad = 60 * (njRandom() * 0.7f + 0.3f);
            const Angle ang = work->ang.y + NJM_DEG_ANG(200.f * (0.5f - njRandom()));
    
            AimPos.x = pPos->x + rad * njSin(ang);
            AimPos.z = pPos->z + rad * njCos(ang);
    
            if(AL_GetCurrLandAttr(&AimPos) == LMA_WATER) {
                break;
            }

            i++;
            
            if(i == 100) {
                AimPos.x = 0;
                AimPos.z = 0;
    
                break;
            }
        }
    
        move->AimPos = AimPos;
        
        {
            float fa = move->AimPos.x - work->pos.x;
            float fb = move->AimPos.z - work->pos.z;

            move->AimAng.y = njArcTan2(fa, fb);
        }

    }
}

void SwimControlStop(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    taskwk* work = tp->twp;
    MOVE_WORK* move = tp->Data2;
    Uint16 skill = AL_ParameterGetSkill(tp, SKILL_SWIM);

    if(!(bhv->Flag & 1)) {
        if(move->Flag & 0x400) {
            AL_SetBehavior(tp, ALBHV_Move);
        }
    }

    if(!(bhv->SubTimer++ % 10)) {
        if(njRandom() < 0.4f) {
            NJS_POINT3 pos; 
            NJS_VECTOR vec; 
            
            pos.x = work->pos.x;
            pos.y = move->WaterY;
            pos.z = work->pos.z;

            vec.x = 0.03f;
            vec.y = 0.f;
            vec.z = 0.7f;

            CreateObjectWaterripple(&pos, &vec, 0.2f);
        }
    }

    if(work->pos.y <= move->WaterY - 2.1f) {
        float spd = 0.f;

        work->pos.y += 0.02f;
        if(work->pos.y > move->WaterY - 2.1f)
            work->pos.y = move->WaterY - 2.1f;    

        move->Acc.y = -move->Gravity - 0.05f * move->Velo.y;

        move->Acc.x = (njSin(work->ang.y) * spd) - 0.05f * move->Velo.x;
        move->Acc.z = (njCos(work->ang.y) * spd) - 0.05f * move->Velo.z;
    }
    
    if (AL_IsEmotionTimerReset(tp)) {
        AL_EmotionAdd(tp, EM_ST_THIRSTY, -GET_GLOBAL()->ParamSubThirstySwim);
        AL_EmotionAdd(tp, EM_ST_TEDIOUS, -GET_GLOBAL()->ParamSubTediousSwim);
        AL_EmotionAdd(tp, EM_ST_TIRE, -GET_GLOBAL()->ParamSubTireSwimStop);
    }

    sub_8C05B2D4(4129, tp, 1, 0, 120, &work->pos);
}

void SwimControl(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    taskwk* work = tp->twp;
    MOVE_WORK* move = tp->Data2;
    Uint16 skill = AL_ParameterGetSkill(tp, SKILL_SWIM);
    
    if(!(bhv->Flag & 4) && (move->Flag & 0x400)) {
        AL_SetBehavior(tp, ALBHV_Move);
    }

    if(!(bhv->SubTimer++ % 10) && njRandom() < 0.4f) {
        NJS_POINT3 pos; 
        NJS_VECTOR vec; 
        
        pos.x = work->pos.x;
        pos.y = move->WaterY;
        pos.z = work->pos.z;

        vec.x = 0.03f;
        vec.y = 0.f;
        vec.z = 0.7f;

        CreateObjectWaterripple(&pos, &vec, 0.2f);
    }

    if(move->Flag & 0x4000) {
        if(AL_EmotionGetValue(tp, EM_ST_THIRSTY) < 1000) {
            if(CCL_IsHitKind2(tp, CI_KIND_AL_CLIMB)) {
                AL_SetBehavior(tp, ALBHV_Climb);
            }
        }
        else {
            if(njRandom() < 0.002f) {
                AL_SetBehavior(tp, ALBHV_Swim);
            }
        }
    }

    if(AL_Dist2FromAim(tp) < 36.f) {
        AL_SetBehavior(tp, ALBHV_Swim);
    }

    if(bhv->SubTimer++ > 600) {
        AL_SetBehavior(tp, ALBHV_Swim);
    }
    
    MOV_TurnToAim2(tp, 288);
    
    if(work->pos.y <= move->WaterY - 2.1f) {
        float spd = GET_GLOBAL()->SkillSwimAccBase + skill * GET_GLOBAL()->SkillSwimAccRatio;

        work->pos.y += 0.1f;
        if(work->pos.y > move->WaterY - 2.1f)
            work->pos.y = move->WaterY - 2.1f;    

        move->Acc.y = -move->Gravity - 0.1f * move->Velo.y;

        move->Acc.x = njSin(work->ang.y) * spd - move->Velo.x * 0.05f;
        move->Acc.z = njCos(work->ang.y) * spd - move->Velo.z * 0.05f;
    }
    
    if (AL_IsEmotionTimerReset(tp)) {
        AL_EmotionAdd(tp, EM_ST_THIRSTY, -GET_GLOBAL()->ParamSubThirstySwim);
        AL_EmotionAdd(tp, EM_ST_TEDIOUS, -GET_GLOBAL()->ParamSubTediousSwim);
        AL_EmotionAdd(tp, EM_ST_TIRE, GET_GLOBAL()->ParamAddTireSwim);
    }

    sub_8C05B2D4(0x101F, tp, 1, -25, 90, &work->pos);
}

int ALBHV_SwimObore(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_FaceSetEye(tp, AL_EYE_NUM_KYA, -1);

            if(AL_FaceGetMouthDefaultNum(tp) == AL_EYE_NUM_KYA) {
                float randval = njRandom();
                
                if(randval < 0.25f) {
                    AL_FaceSetMouth(tp, AL_MOUTH_NUM_GEE, -1);
                }
            }
            else {
                float randval = njRandom();

                if(randval < 0.25f) {
                    AL_FaceSetMouth(tp, AL_MOUTH_NUM_UEEN, -1);
                }
                else if(randval < 0.5f) {
                    AL_FaceSetMouth(tp, AL_MOUTH_NUM_KOIKE, -1);
                }
                else if(randval < 0.75f) {
                    AL_FaceSetMouth(tp, AL_MOUTH_NUM_UWAAN, -1);
                }
                else {
                    AL_FaceSetMouth(tp, AL_MOUTH_NUM_MUSU, -1);
                }
            }

            AL_SetMotionLink(tp, ALM_OBORERU);

            bhv->Mode++;
            break;
        case 1:
            break;
    }

    if(!(GET_CHAOWK(tp)->Timer % 120) && njRandom() < 0.2f) {
        sub_8C05B020(24612, 0, 0, 110, &GET_CHAOWK(tp)->pos);
    }

    if (AL_IsEmotionTimerReset(tp)) {
        AL_EmotionAdd(tp, EM_ST_THIRSTY, -10 * GET_GLOBAL()->ParamSubThirstySwim);
    }

    SwimControl(tp);

    return BHV_RET_CONTINUE;
}

int ALBHV_SwimBataashi(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0: {
            float randval = njRandom();

            if(randval < 0.2f) {
                AL_FaceSetEye(tp, AL_EYE_NUM_KYA, -1);
            }
            else if(randval < 0.4f) {
                AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            }
            
            AL_SetMotionLink(tp, ALM_BATAASHI_B);

            bhv->Mode++;
            break;
        }
        case 1:
            break;
    }

    SwimControl(tp);

    return BHV_RET_CONTINUE;
}

int ALBHV_SwimCrawl(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0: {
            float randval = njRandom();

            if(randval < 0.2f) {
                AL_FaceSetEye(tp, AL_EYE_NUM_KYA, -1);
            }
            else if(randval < 0.4f) {
                AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            }
            
            AL_SetMotionLink(tp, ALM_CRAWL);

            bhv->Mode++;
            break;
        }
        case 1:
            break;
    }

    SwimControl(tp);

    return BHV_RET_CONTINUE;
}

int ALBHV_SwimSeoyogi(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0: {
            float randval = njRandom();

            if(randval < 0.2f) {
                AL_FaceSetEye(tp, AL_EYE_NUM_KYA, -1);
            }
            else if(randval < 0.4f) {
                AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            }
            
            AL_SetMotionLink(tp, ALM_SEOYOGI_KAITEN);

            bhv->Mode++;
            break;
        }
        case 1:
            break;
    }

    SwimControl(tp);

    return BHV_RET_CONTINUE;
}

int ALBHV_SwimPuha(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_PUHA_PUHA, 20);
            bhv->Mode++;
        case 1:
            if(bhv->Timer++ > 30) {
                AL_FaceSetEye(tp, AL_EYE_NUM_KYA, 120);
                AL_FaceSetMouth(tp, AL_MOUTH_NUM_HOYO, 120);

                bhv->Mode++;
                
                sub_8C05B020(0x6025, 0, 0, 0, &GET_CHAOWK(tp)->pos);
            }
        case 2:
            if(AL_IsMotionEnd(tp)) {
                float randval = njRandom();
        
                if(randval < 0.5f) {
                    AL_FaceSetEye(tp, AL_EYE_NUM_TOHOHO, 120);
                }
                else if(randval < 0.8f) {
                    AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, 120);
                }
        
                AL_SetMotionLink(tp, ALM_DRIFT);
        
                bhv->Timer = (Uint16)(600 + (int)(njRandom() * 601.0f));
                bhv->Mode++;
            }
            break;
        case 3:
            if(bhv->Timer-- <= 0) {
                AL_SetBehavior(tp, ALBHV_Swim);
            }
            break;
    }
    
    SwimControlStop(tp);
    
    return BHV_RET_CONTINUE;
}

int ALBHV_Swim(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    taskwk* work = tp->twp;
    MOVE_WORK* move = tp->Data2;
    Uint16 skill = AL_ParameterGetSkill(tp, SKILL_SWIM);

    AL_DecideAimSwimPosition(tp);
    AL_DisableNade(tp);

    if(skill < GET_GLOBAL()->SkillSwimBataashi) {
        AL_SetBehavior(tp, ALBHV_SwimObore);
    }
    else {
        if(skill < GET_GLOBAL()->SkillSwimCrawl) {
            AL_SetBehavior(tp, ALBHV_SwimBataashi);    
        }
        else {
            AL_SetBehavior(tp, ALBHV_SwimCrawl);
        }

        if (AL_PartsIsMinimalFlagOn(tp, eMinimalType_Rako)) {
            if(njRandom() < 0.5f) {
                AL_SetBehavior(tp, ALBHV_SwimSeoyogi);
            }
        }

        if(njRandom() < 0.2f) {
            AL_SetBehavior(tp, ALBHV_SwimPuha);
        }
    }

    SwimControl(tp);
    
    return BHV_RET_CONTINUE;
}
