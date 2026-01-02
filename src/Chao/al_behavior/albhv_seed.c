#include <Chao/Chao.h>

#include <Chao/al_misc.h>
#include <Chao/al_field.h>
#include <playsound.h>

extern NJS_CNK_OBJECT object_alo_johro_pos_jyohro[];
extern NJS_CNK_OBJECT object_alo_scop_pos_scop[];

extern AL_ITEM_INFO ScopItemInfo;
extern AL_ITEM_INFO JyouroItemInfo;

int ALBHV_Sleep(task*);
int ALBHV_Eat(task*);

int ALBHV_Seed1(task*);
int ALBHV_Seed2(task*);
int ALBHV_Seed3(task*);
int ALBHV_Seed4(task*);
int ALBHV_Jyouro(task*);
int ALBHV_NyokiNyoki(task*);

Bool AL_CheckSeed(task* tp) {
    BHV_FUNC func = AL_GetBehavior(tp);
    task* pSeed;

    if(func == ALBHV_Sleep || func == ALBHV_Eat) {
        return FALSE;
    }

    if(AL_ParameterGetSkill(tp, SKILL_RUN) < GET_GLOBAL()->SkillWalk) {
        return FALSE;
    }

    if(ALW_IsCommunication(tp)) {
        return FALSE;
    }

    if(func == ALBHV_Seed1) {
        return FALSE;
    }

    if(pSeed = CCL_IsHitKind2(tp, CI_KIND_AL_SEED)) {
        if(!ALW_IsHeld(pSeed)) {
            MOV_SetAimPos(tp, &pSeed->twp->pos);
            
            AL_SetBehavior(tp, ALBHV_Seed1);
            
            return TRUE;
        }
    }

    return FALSE;
}

int ALBHV_Seed1(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_CAPTURE_MORAU, 15);

            bhv->Mode++;
            bhv->Timer = 60;
            break;
    }

    if (MOV_TurnToAim2(tp, 1536) < 182) {
        task* pSeed = CCL_IsHitKind2(tp, CI_KIND_AL_SEED);
        
        if(pSeed) {
            if(!ALW_IsHeld(pSeed)) {
                StopHoldingTaskP(0);

                AL_GrabObjectBothHands(tp, pSeed);
                
                AL_SetBehavior(tp, ALBHV_Seed2);
            }
        }
        else {
            if (bhv->Timer-- <= 0) {
                return BHV_RET_FINISH;
            }
        }
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Seed2(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_HATENA_HATE2, 15);

            AL_FaceSetEye(tp, AL_EYE_NUM_NORMAL, -1);
            AL_IconSet(tp, MD_ICON_HATENA, 300);

            sub_8C05B020(24604, 0, 0, 110, &tp->twp->pos);

            AL_SetHeldFlagLeft(tp);
            
            bhv->Timer = RAND_RANGE(300, 420);
            bhv->Mode++;
            
            break;
    }

    if(--bhv->Timer <= 0) {
        AL_SetBehavior(tp, ALBHV_Seed3);
    }

    return !AL_MoveHoldingObject(tp);
}

int ALBHV_Seed3(task* tp) {
    taskwk* twk = tp->twp;
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    MOVE_WORK* move = (MOVE_WORK*)tp->Data2;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_HAKOBU, 15);
            AL_FaceSetEye(tp, AL_EYE_NUM_NORMAL, -1);

            AL_SetHeldFlag(tp);

            bhv->LimitTimer = RAND_RANGE(1800, 2200);
            bhv->Mode++;

            lbl_0C530F20(tp);
            
            break;
        
        case 1: {
            Bool flag = FALSE;
            task* pPlantPos;

            if(pPlantPos = AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_PLANT_POS)) {
                if(njDistanceP2P(&pPlantPos->twp->pos, &twk->pos) < AL_ParameterGetSkill(tp, SKILL_INTELLECT) / 1000.f + 20.f ) {
                    ALW_LockOn(tp, pPlantPos);

                    bhv->Mode++;

                    flag = TRUE;

                    sub_8C05B020(24602, 0, 0, 110, &tp->twp->pos);
                }
            }

            if(!flag) {
                MOV_TurnToAim2(tp, 256);

                if(AL_Dist2FromAim(tp) < 64) {
                    if(njRandom() < 0.7f) {
                        AL_SetBehavior(tp, ALBHV_Seed2);
                    }
                }
            }
            
            break;
        }

        case 2:
            ALW_TurnToLockOn(tp, 640);

            if(ALW_CalcDistFromLockOn(tp) < 2.5f) {
                AL_SetBehavior(tp, ALBHV_Seed4);
            }
            
            break;
    }

    if(--bhv->LimitTimer <= 0) {
        return BHV_RET_FINISH;
    }

    if (move->Flag & 0x400) {        
        move->Acc.x = njSin(twk->ang.y) * GET_GLOBAL()->WalkAcc * 0.8f;
        move->Acc.z = njCos(twk->ang.y) * GET_GLOBAL()->WalkAcc * 0.8f;
    }
    
    {
        float velo = njScalor(&move->Velo);
        AL_SetMotionSpd(tp, velo * 18);
    }
    
    if(!AL_MoveHoldingObject(tp)) {
        return BHV_RET_FINISH;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Seed4(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_STAND, 15);
            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_IconSet(tp, MD_ICON_BIKKURI, 180);

            AL_SetHeldFlagLeft(tp);
            AL_FixPosition(tp);

            bhv->Timer = RAND_RANGE(120, 240);
            bhv->Mode++;            
            break;
        
        case 1:
            ALW_TurnToLockOn(tp, 256);

            if(bhv->Timer-- <= 0) {
                if(!AL_KW_IsSToyFlagOn(tp, AL_STOY_SCOP)) {
                    AL_IconSet(tp, MD_ICON_MOJYA, 180);

                    return BHV_RET_FINISH;
                }
                else {
                    AL_SetMotionLinkStep(tp, ALM_SEED_UERU_START, 15);
    
                    ScopItemInfo.pObject = object_alo_scop_pos_scop;
                    AL_SetItemOffset(tp, AL_PART_HAND_R, &ScopItemInfo);
    
                    bhv->Timer = RAND_RANGE(180, 300);
                    bhv->Mode++;
                }
            }
            break;
        
        case 2:
            if(bhv->Timer-- <= 0) {
                AL_SetMotionLinkStep(tp, ALM_SEED_UERU_UERU, 15);
                
                bhv->Mode++;
                bhv->Timer = 0;
                bhv->SubTimer = 0;
            }    
            break;
        
        case 3:
            if(!(bhv->Timer++ % 35)) {
                sub_8C05B020(4131, 0, 0, -10, &tp->twp->pos);
            }

            if(AL_IsMotionEnd(tp)) {
                al_entry_work* pSeedEntry = ALW_IsCommunicationEx(tp, 7);    

                if(pSeedEntry && pSeedEntry->tp) {
                    task* pSeed = pSeedEntry->tp;
                    Sint16 kind = pSeed->twp->ang.x;
                    
                    task* pPlantPos = AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_PLANT_POS);
                    
                    if(pPlantPos) {
                        al_entry_work* pPlantEntry = pPlantPos->EntityData2;

                        pPlantPos->twp->ang.y = pSeed->twp->ang.y;
                        
                        if(pPlantEntry) {
                            sub_C51E3F8(pPlantEntry, ALW_CMD_PLANTED, kind);
                        }

                        ALW_CommunicationOff(tp);
                        ALW_LockOn(tp, pPlantPos);
                    }
                    
                    // clear save logic here on sa2b
                    
                    FreeTask(pSeedEntry->tp);
                }
                
                AL_SetMotionLinkStep(tp, ALM_SEED_UERU_UMERU_START, 15);

                bhv->Timer = RAND_RANGE(180, 360);
                bhv->Mode++;
            }
            
            break;
        
        case 4:
            if(!(bhv->Timer % 35)) {
                sub_8C05B020(4132, 0, 0, -10, &tp->twp->pos);
            }

            if(!(bhv->Timer % 60)) {
                if(njRandom() < 0.5f) {
                    sub_8C05B020(24709, 0, 0, 100, &tp->twp->pos);
                }
            }

            if(bhv->Timer-- <= 0) {
                AL_SetMotionLinkStep(tp, ALM_SEED_UERU_END, 15);

                bhv->Mode++;
            }
            
            break;
        
        case 5:
            if(AL_IsMotionStop(tp)) {
                AL_SetBehavior(tp, ALBHV_Jyouro);
            }
            break;
    }

    AL_MoveHoldingObject(tp);

    return BHV_RET_CONTINUE;
}

int ALBHV_ApproachToJyouro(task* tp) {
    taskwk* twk = tp->twp;
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    MOVE_WORK* move = (MOVE_WORK*)tp->Data2;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_ARUKU, 15);
            
            AL_FaceSetEye(tp, AL_EYE_NUM_NORMAL, -1);
            AL_IconSet(tp, MD_ICON_BIKKURI, 300);

            bhv->LimitTimer = RAND_RANGE(1800, 1920);
            bhv->Mode++;
            break;
    }

    if(--bhv->LimitTimer <= 0) {
        return BHV_RET_FINISH;
    }

    ALW_TurnToLockOn(tp, 640);

    if(ALW_CalcDistFromLockOn(tp) < 4) {
        AL_SetBehavior(tp, ALBHV_Jyouro);
    }

    if (move->Flag & 0x400) {        
        move->Acc.x = njSin(twk->ang.y) * GET_GLOBAL()->WalkAcc * 0.8f;
        move->Acc.z = njCos(twk->ang.y) * GET_GLOBAL()->WalkAcc * 0.8f;
    }
    
    {
        float velo = njScalor(&move->Velo);
        AL_SetMotionSpd(tp, velo * 18);
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Jyouro(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_OSYABURI, 15);

            bhv->Timer = RAND_RANGE(60, 120);
            AL_FixPosition(tp);
            bhv->Mode++;
            
            break;
        
        case 1:
            if(--bhv->Timer <= 0) {
                AL_SetMotionLinkStep(tp, ALM_HIRAMEKI, 15);
                
                AL_IconSet(tp, MD_ICON_BIKKURI, 120);
                AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);

                sub_8C05B020(24602, 0, 0, 110, &tp->twp->pos);

                bhv->Timer = RAND_RANGE(60, 100);

                if(!AL_KW_IsSToyFlagOn(tp, AL_STOY_JYOURO)) {
                    AL_EmotionAdd(tp, EM_ST_THIRSTY, 2000);

                    return BHV_RET_FINISH;
                }
                else {
                    bhv->Mode++;
                }    
            }
            break;
        
        case 2:
            if(--bhv->Timer <= 0) {
                AL_SetMotionLinkStep(tp, ALM_ARUKU, 15);

                bhv->Timer = 150;
                bhv->Mode++;
            }
            break;
        
        case 3:
            AL_ForwardSpd(tp, -0.015);
            ALW_TurnToLockOn(tp, 512);

            if(--bhv->Timer <= 0) {
                AL_SetMotionLinkStep(tp, ALM_JYOURO_START, 15);

                JyouroItemInfo.pObject = object_alo_johro_pos_jyohro;
                AL_SetItemOffset(tp, AL_PART_HAND_R, &JyouroItemInfo);

                bhv->Timer = RAND_RANGE(240, 340);
                bhv->Mode++;
            }
            
            break;
        
        case 4:
            if(--bhv->Timer <= 0) {
                AL_SetMotionLinkStep(tp, ALM_JYOURO_END, 15);
                bhv->Mode++;
            }
            break;
        
        case 5:
            if (AL_IsMotionEnd(tp)) {
                AL_EmotionAdd(tp, EM_ST_THIRSTY, 2000);
                AL_SetBehavior(tp, ALBHV_NyokiNyoki);
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_NyokiNyoki(task *tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_TREE_DANCE_NYOKI, 15);
            AL_FixPosition(tp);

            bhv->Mode++;
            break;
        
        case 1:
            if(AL_IsMotionStop(tp)) {
                AL_SetMotionLinkStep(tp, ALM_TREE_DANCE_NYOKI, 15);
                bhv->Mode++;
            }
            break;

        case 2:
            if(AL_IsMotionStop(tp)) {
                taskwk* twk = tp->twp;
                int ang = twk->ang.y;
                NJS_POINT3 pos;

                pos.x = twk->pos.x + njSin(ang) * 4;
                pos.y = twk->pos.y;
                pos.z = twk->pos.z + njCos(ang) * 4;
                
                AL_AllFieldCreateT(CI_KIND_AL_WATER, &pos, 5, 10);

                return BHV_RET_FINISH;
            }

            break;
    }

    return BHV_RET_CONTINUE;
}