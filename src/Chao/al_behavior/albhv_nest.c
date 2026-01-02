#include <Chao/Chao.h>

#include <Chao/al_egg.h>
#include <Chao/al_misc.h>
#include <Chao/al_field.h>

int ALBHV_Nest(task* tp) {
    taskwk* twk = tp->twp;
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    MOVE_WORK* move = tp->Data2;

    switch(bhv->Mode) {
        case 0:
            AL_FixPosition(tp);
            bhv->Timer = 0;

            ALO_CreateNest(tp);

            GET_CHAOWK(tp)->pAnyTask = AL_ChildFieldCreateT(
                tp, 
                CI_KIND_AL_NEST,
                &GET_CHAOWK(tp)->pos,
                30,
                GET_GLOBAL()->NestWaitTime - 120
            );

            bhv->Mode++;
            break;
        
        case 1:
            AL_SetMotionLinkStep(tp, ALM_HIMA_SIT, 15);
            
            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_IconSet(tp, MD_ICON_HEART, GET_GLOBAL()->NestWaitTime - 300);

            bhv->Mode++;
            break;
        
        case 2:
            bhv->Timer++;
            
            if(bhv->Timer == GET_GLOBAL()->NestWaitTime >> 1) {
                AL_EmotionSetValue(tp, EM_ST_BREED, 0);
            }

            if(bhv->Timer > GET_GLOBAL()->NestWaitTime) {
                return BHV_RET_FINISH;
            }

            if(ALW_RecieveCommand(tp) == ALW_CMD_GO) {
                al_entry_work* pEntry = ALW_IsCommunicationEx(tp, ALW_CATEGORY_CHAO);
                if(pEntry && pEntry->tp && pEntry->tp->twp) {
                    MOV_SetAimPos(tp, &pEntry->tp->twp->pos);
                }

                AL_SetMotionLinkStep(tp, ALM_GENKI_TATI, 15);

                bhv->Mode++;
                bhv->Timer = 0;
                bhv->SubTimer = 0;

                if(GET_CHAOWK(tp)->pAnyTask) {
                    FreeTask(GET_CHAOWK(tp)->pAnyTask);
                    GET_CHAOWK(tp)->pAnyTask = NULL;
                }
            }
            
            break;

        case 3:
            if(bhv->Timer++ > 30) {
                MOV_TurnToAim2(tp, 1024);
            }

            if(AL_IsMotionEnd(tp)) {
                if(ALW_IsCommunicationEx(tp, ALW_CATEGORY_CHAO)) {
                    AL_SetMotionLinkStep(tp, ALM_NADE_KIMOCHII, 15);
                    
                    bhv->Mode++;
                    bhv->Timer = 0;
                    bhv->SubTimer = 0;
                }
                else {
                    return BHV_RET_FINISH;
                }
            }
            break;

        case 4:
            if(ALW_RecieveCommand(tp) == ALW_CMD_GO) {
                AL_SetMotionLinkStep(tp, ALM_HASYAGU_JUMP, 15);
                
                AL_EmotionSetValue(tp, EM_ST_BREED, 0);
                
                bhv->Mode++;
                bhv->Timer = 0;
                bhv->SubTimer = 0;
            }
            
            AL_SetMotionSpd(tp, 1.5f);
            
            break;

        case 5:
            if(bhv->Timer++ == 20) {
                move->Velo.x -= 0.25f * njSin(twk->ang.y);
                move->Velo.y += 0.8f;
                move->Velo.z -= 0.25f * njCos(twk->ang.y);
            }

            if(AL_IsMotionEnd(tp)) {
                ALW_CommunicationOff(tp);

                AL_SetMotionLinkStep(tp, ALM_BANZAI_JUMP, 15);

                bhv->Mode++;
                
                bhv->Timer = 0;
                bhv->SubTimer = 0;
                bhv->SubTimer = (Uint16)(3 + (int)(njRandom() * 3.999999f));
            }
            
            break;

        case 6:
            if(AL_IsMotionEnd(tp)) {
                if(++bhv->Timer >= bhv->SubTimer) {
                    return BHV_RET_FINISH;
                }
            }
            
            break;
    }

    GET_CHAOWK(tp)->NestFlag = TRUE;
    
    return BHV_RET_CONTINUE;
}

int ALBHV_ApproachNest(task* tp) {
    taskwk* twk = tp->twp;
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    MOVE_WORK* move = tp->Data2;

    switch(bhv->Mode) {
        case 0:
            if(!AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_NEST)) {
                return BHV_RET_FINISH;
            }
            
            switch(AL_GetMotionPosture(tp)) {
                case 2:
                    AL_SetMotionLinkStep(tp, ALM_JUMP_UP, 15);
                    break;
                case 1:
                    AL_SetMotionLinkStep(tp, ALM_GENKI_TATI, 15);
                    break;
                case 0:
                default:
                    AL_SetMotionLinkStep(tp, ALM_KYOROKYORO_UP, 15);
                    break;
            }

            bhv->Timer = 0;
            bhv->Mode++;
            
            break;

        case 1:
            if(AL_IsMotionEnd(tp)) {
                task* pField;
                
                AL_SetMotionLinkStep(tp, ALM_RHYTHM_WALK_A, 15);
                
                AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
                AL_IconSet(tp, MD_ICON_HEART, 400);

                pField = AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_NEST);
                if(!pField) {
                    return BHV_RET_FINISH;
                }
                
                move->AimPos.x = pField->twp->pos.x;
                move->AimPos.z = pField->twp->pos.z;

                if(pField->ptp) {
                    ALW_AttentionOn(tp, pField->ptp);
                }

                bhv->Mode++;
            }
            break;

        case 2:
            if(ALW_RecieveCommand(tp) == ALW_CMD_CHANGE) {
                return BHV_RET_FINISH;
            }
            else {
                MOV_TurnToAim2(tp, 512);
    
                if(move->Flag & 0x400) {
                    move->Acc.x += njSin(twk->ang.y) * GET_GLOBAL()->WalkSlowAcc;
                    move->Acc.z += njCos(twk->ang.y) * GET_GLOBAL()->WalkSlowAcc;
                }
    
                {
                    task* pChao = AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_CHAO);
                    task* pAttention = ALW_IsAttention(tp)->tp;
    
                    if(pAttention == pChao) {
                        if (ALW_IsSheAttentionOtherOne(tp, pAttention)) {
                            return BHV_RET_FINISH;
                        }
    
                        ALW_CommunicationOn(tp, pAttention);
                        ALW_SendCommand(tp, ALW_CMD_GO);
                        PlayJingle("chao_g_dance.adx");
                        AL_SetMotionLinkStep(tp, ALM_NADE_KIMOCHII, 15);
    
                        bhv->Mode++;
                        bhv->Timer = 0;
                        bhv->SubTimer = 0;
                    }
                }
    
                AL_SetMotionSpd(tp, 1.4f);
    
                if(!AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_NEST)) {
                    return BHV_RET_FINISH;
                }
    
                if(bhv->LimitTimer-- <= 0) {
                    return BHV_RET_FINISH;
                }
            }
            break;

        case 3:
            if(!ALW_IsCommunicationEx(tp, ALW_CATEGORY_CHAO)) {
                return BHV_RET_FINISH;
            }
            
            if(bhv->Timer++ > 300) {
                ALW_SendCommand(tp, ALW_CMD_GO);

                AL_SetMotionLinkStep(tp, ALM_HASYAGU_JUMP, 15);
                AL_EmotionSetValue(tp, EM_ST_BREED, 0);

                bhv->Mode++;
                bhv->Timer = 0;
                bhv->SubTimer = 0;
            }

            AL_SetMotionSpd(tp, 1.5f);
            break;

        case 4:
            if(bhv->Timer++ == 20) {
                al_entry_work* pEntry;
                
                move->Velo.x -= 0.25f * njSin(twk->ang.y);
                move->Velo.y += 0.8f;
                move->Velo.z -= 0.25f * njCos(twk->ang.y);

                pEntry = ALW_IsCommunicationEx(tp, ALW_CATEGORY_CHAO);
                if(pEntry) {
                    task* pOtherChao = pEntry->tp;
                    
                    if(pOtherChao) {
                        AL_GENE ChildGene;
                        NJS_POINT3 pos;
                        chao_info* pInfo;

                        pos = twk->pos;
                        
                        pos.x += 2 * njSin(twk->ang.y);
                        pos.z += 2 * njCos(twk->ang.y);

                        AL_CreateChildGene(pOtherChao, tp, &ChildGene);

                        pInfo = AL_GetNewChaoSaveInfo();
                        if(pInfo) {
                            CreateEgg(&ChildGene, &pos, CREATE_EGG_BORN, pInfo);
                        }
                    }
                }
            }

            if(AL_IsMotionEnd(tp)) {
                ALW_CommunicationOff(tp);

                AL_SetMotionLinkStep(tp, ALM_BANZAI_JUMP, 15);

                bhv->Mode++;
                bhv->Timer = 0;
                bhv->SubTimer = 0;
                bhv->SubTimer = (Uint16)(4 + (int)(njRandom() * 3.999999f));
            }
            break;

        case 5:
            if(AL_IsMotionEnd(tp)) {
                if(++bhv->Timer >= bhv->SubTimer) {
                    return BHV_RET_FINISH;
                }
            }
            
            break;
    }

    return BHV_RET_CONTINUE;
}
