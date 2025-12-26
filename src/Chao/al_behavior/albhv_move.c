#include <Chao/Chao.h>
#include <Chao/al_landmark.h>

#include <player.h>
#include <playsound.h>
#include <Chao/al_toy/alo_ball.h>

void lbl_0C530F20(task* tp) {
    chaowk* work = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    MOVE_WORK* move = tp->Data2;
    NJS_POINT3* pPos = &work->pos;

    int something = 0;
    NJS_POINT3 AimPos;
    
    AL_GetRandomAttrPos(LMA_GROUND1, &move->AimPos);
}

int ALBHV_Move(task *tp) {
    lbl_0C530F20(tp);
    AL_WalkSelect(tp);

    return BHV_RET_CONTINUE;
}

int ALBHV_Koke(task *tp) {
    chaowk* work = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    MOVE_WORK* move = tp->Data2;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_KOKE, 15);
            bhv->Mode++;
            bhv->Timer = 0;
        case 1:
            if(bhv->Timer == 40) {
                sub_8C05B020(0x100A, 0, 0, 0, &GET_CHAOWK(tp)->pos);
            }
            
            if(bhv->Timer++ > 60) {
                AL_IconSet(tp, MD_ICON_MOJYA, 240);
                AL_FaceChangeEye(tp, AL_EYE_NUM_KYA);
                bhv->Mode++;
            }

            if(move->Flag & 0x400) {
                move->Acc.x = njSin(work->ang.y) * GET_GLOBAL()->WalkAcc;
                move->Acc.z = njCos(work->ang.y) * GET_GLOBAL()->WalkAcc;
            }
            
            break;
        case 2:
            if(bhv->Timer++ > 300) {
                AL_SetMotionLinkStep(tp, ALM_KOKEOKI, 15);
                bhv->Mode++;
            }
            break;
        case 3:
            if(AL_IsMotionEnd(tp)) AL_ReturnBehavior(tp);
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Climb(task *tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    MOVE_WORK* move = tp->Data2;
    NJS_POINT3* pPos = &tp->twp->pos;
    Uint16 skill = AL_ParameterGetSkill(tp, SKILL_POWER);
    chaowk* work = GET_CHAOWK(tp);

    AL_BehaviorUnknown(tp);

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_NOBORU);
            MOV_ClearVelo(tp);
            bhv->Mode++;
        case 1:
            if(move->Flag & 0x4800) {
                Angle ang = njArcTan2(move->FrontWall.vx, move->FrontWall.vz);
                GET_CHAOWK(tp)->ang.y = AdjustAngle(GET_CHAOWK(tp)->ang.y, ang + NJM_DEG_ANG(180), 0x800);
                move->Flag &= ~0x4000;
                bhv->Timer = 0;
            }
            else {
                if(bhv->Timer++ > 2) {
                    bhv->Timer = 0;
                    work->ClimbFirstPos = GET_CHAOWK(tp)->pos.y;
                    bhv->Mode++;
                }
            }
            
            break;
        case 2:
            bhv->Timer++;
            if(bhv->Timer > 3 && (move->Flag & 0x400)) {
                const NJS_VECTOR up = {0, 1, 0};
        
                if(njInnerProduct(&move->Shadow.hit[2].normal, &up) > 0.7f) {
                    AL_SetBehavior(tp, ALBHV_Move);
                }
            }
            else if(move->Flag & 0x4800) {
                bhv->Mode--;
            }

            if(bhv->Timer > 30 || (GET_CHAOWK(tp)->pos.y - work->ClimbFirstPos) > 2.5f) {
                AL_SetBehavior(tp, ALBHV_Move);
            }
            
            break;
    }

    if(bhv->SubTimer++ > 240) return BHV_RET_FINISH;

    {
        Angle ang = GET_CHAOWK(tp)->ang.y;

        pPos->x += 0.1f * njSin(ang);
        pPos->y += 0.08f;
        pPos->z += 0.1f * njCos(ang);

        move->Acc.y = -move->Gravity;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Glide(task *tp) {
    chaowk* work = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    MOVE_WORK* move = tp->Data2;

    switch(bhv->Mode) {
        case 0:
            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_SetMotionLinkStep(tp, ALM_TOBU, 15);
            
            {
                Angle ang = GET_CHAOWK(tp)->ang.y;
        
                move->Velo.x = njSin(ang) * 0.4f;
                move->Velo.y = 0.35f;
                move->Velo.z = njCos(ang) * 0.4f;
            }

            bhv->Timer = (Uint16)(300 + (int)(njRandom() * 301.0f));
            bhv->Mode++;
            break;
        case 1:
            if(move->Flag & 0x400) {
                AL_SetBehavior(tp, ALBHV_Move);
            }
            break;
    }

    MOV_TurnToAim2(tp, 256);

    // fake imo but i have no idea, won't let this one thing spoil the party
    // (float constants usage is out of order)
    if(0) { 
        move->Acc.x = 0.1f;
        move->Acc.z = 0.05f;
    }
    
    move->Acc.x = njSin(work->ang.y) * 0.05f - (0.1f * move->Velo.x);
    move->Acc.z = njCos(work->ang.y) * 0.05f - (0.1f * move->Velo.z);

    if(bhv->Timer > 0) {
        bhv->Timer--;
        
        move->Acc.y = -move->Gravity * 0.95f;
    }
    else {
        move->Acc.y = -move->Gravity * 0.92f;
    }
    
    if(work->pos.y > 100) {
        work->pos.y = 100;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_ChasePlayer(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    chaowk* work = GET_CHAOWK(tp);
    MOVE_WORK* move = tp->Data2;

    AL_BehaviorUnknown(tp);
    
    switch(bhv->Mode) {
        case 0: {
            float randval = njRandom();
            if(randval < 0.2f) {
                AL_SetMotionLinkStep(tp, ALM_OIKAKE, 10);
            }
            else if(randval < 0.6f) {
                AL_SetMotionLinkStep(tp, ALM_RUN, 10);
            }
            else {
                AL_SetMotionLinkStep(tp, ALM_RUN_BANZAI, 10);
            }

            AL_FaceChangeEye(tp, AL_EYE_NUM_NIKO);

            if(njRandom() < 0.4f) {
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_SHARK);
            }
            else {
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_NIKO);
            }

            AL_IconSet(tp, MD_ICON_HEART, 120);

            bhv->Mode++;
            bhv->Timer = 0;
        }
        case 1:
            MOV_SetAimPos(tp, &playertwp[0]->pos);
            break;
    }

    bhv->Timer++;
    if(bhv->Timer > 3000) {
        
    }
    
    if(!(bhv->Timer % 100)) {
        if(njRandom() < 0.1f) {
            AL_SetBehavior(tp, ALBHV_Koke);
        }
    }

    if (MOV_Dist2FromAim(tp) < 64.0) {
        return BHV_RET_FINISH;
    }

    MOV_TurnToAim2(tp, 256);

    if(move->Flag & 0x400) {
        // they fixed this to use ParameterGetSkill in sa2b onwards
        // no clue why they didn't on DC
        CHAO_PARAM* pParam = GET_CHAOPARAM(tp);
        move->Acc.x = (njSin(work->ang.y) * 0.05f) * (pParam->Skill[SKILL_RUN] / 1000.f + 1.f);
        move->Acc.z = (njCos(work->ang.y) * 0.05f) * (pParam->Skill[SKILL_RUN] / 1000.f + 1.f);
    }
    
    return BHV_RET_CONTINUE;
}

int ALBHV_NoticePlayer(task* tp) {
    enum {
        MD_NOTICE_START,
        MD_NOTICE_WAIT,
        MD_NOTICE_KYORO,
        MD_NOTICE_TURN,
        MD_NOTICE_BANZAI,
        MD_NOTICE_WAIT2
    };
    
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    chaowk* work = GET_CHAOWK(tp);
    MOVE_WORK* move = tp->Data2;

    switch(bhv->Mode) {
        case MD_NOTICE_START:
            bhv->Timer = 20 + (Uint16)(njRandom() * 40.0f);
            
            AL_SetMotionLinkStep(tp, ALM_STAND, 10);
            bhv->Mode = MD_NOTICE_WAIT;
            break;
        case MD_NOTICE_WAIT:
            if(bhv->Timer-- <= 0) {
                if(njRandom() < 0.5f) {
                    AL_SetMotionLinkStep(tp, ALM_KYOROKYORO_UP, 10);
                }
                else {
                    AL_SetMotionLinkStep(tp, ALM_HATENA_HATE2, 10);
                }
                
                AL_IconSet(tp, MD_ICON_HATENA, 180);
                
                bhv->Timer = 20 + (Uint16)(njRandom() * 60.0f);
                bhv->Mode = MD_NOTICE_KYORO;
            }
            break;
        case MD_NOTICE_KYORO:
            if(bhv->Timer-- <= 0) {
                AL_SetMotionLinkStep(tp, ALM_ARUKU, 10);
                bhv->Mode = MD_NOTICE_TURN;
            }
            break;
        case MD_NOTICE_TURN:
            AL_SetMotionSpd(tp, 1.5f);
            
            if (MOV_TurnToPlayer2(tp, 768, 0) < 256) {
                AL_IconSet(tp, MD_ICON_BIKKURI, 60);
                
                bhv->Timer = 20 + (Uint16)((njRandom() * 20.0f));

                if(njRandom() < 0.7f) {
                    AL_SetMotionLinkStep(tp, ALM_BANZAI_JUMP, 10);
                    AL_FaceChangeEye(tp, AL_EYE_NUM_NIKO);
                    
                    bhv->Mode = MD_NOTICE_BANZAI;

                    sub_8C05B020(0x6019, 0, 0, 110, &GET_CHAOWK(tp)->pos);
                }
                else {
                    bhv->Mode = MD_NOTICE_WAIT2;
                }
            }
            
            break;
        case MD_NOTICE_BANZAI:
            if(AL_IsMotionEnd(tp)) {
                bhv->Mode = MD_NOTICE_WAIT2;
            }
            break;
        case MD_NOTICE_WAIT2:
            if(bhv->Timer-- <= 0) {
                AL_SetBehavior(tp, ALBHV_ChasePlayer);
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

Bool AL_CheckWhistle(task *tp) {
    if(playertwp[0]->mode != 42) {
        return FALSE;
    }

    if(MOV_Dist2FromPlayer(tp, 0) < 10000) {
        if (AL_KW_GetPlayerLike(tp) > 50) {
            AL_SetBehavior(tp, ALBHV_NoticePlayer);
        }
        
        return TRUE;
    }
    
    return FALSE;
}

// madeup name
int ALBHV_ChaseBall(task *tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    chaowk* work = GET_CHAOWK(tp);
    MOVE_WORK* move = tp->Data2;

    AL_BehaviorUnknown(tp);
    
    switch(bhv->Mode) {
        case 0: {
            float randval = njRandom();
            if(randval < 0.15f) {
                AL_SetMotionLinkStep(tp, ALM_OIKAKE, 10);
            }
            else if(randval < 0.6f) {
                AL_SetMotionLinkStep(tp, ALM_RUN, 10);
            }
            else {
                AL_SetMotionLinkStep(tp, ALM_RUN_BANZAI, 10);
            }

            if(njRandom() < 0.6f) {
                AL_FaceChangeEye(tp, AL_EYE_NUM_NIKO);
            }
            else {
                AL_FaceChangeEye(tp, AL_EYE_NUM_NIKO);
            }

            if(njRandom() < 0.4f) {
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_SHARK);
            }
            else {
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_NIKO);
            }

            AL_IconSet(tp, MD_ICON_BIKKURI, 120);

            bhv->Mode++;
            bhv->Timer = 0;
        }
        case 1:
            if(pBallTask) {
                NJS_POINT3 pos;
                pos.x = pBallTask->twp->pos.x * 1.02f;
                pos.y = pBallTask->twp->pos.y;
                pos.z = pBallTask->twp->pos.z * 1.02f;
                
                MOV_SetAimPos(tp, &pos);

                if(pBallTask->twp->flag & 0x8000) {
                    ALW_AttentionOff(tp);
                    return BHV_RET_FINISH;
                }
            }
            break;
    }

    bhv->Timer++;

    if(!(bhv->Timer % 100)) {
        if(njRandom() < 0.15f) {
            AL_SetBehavior(tp, ALBHV_Koke);
        }
    }

    if(BallWaterFlag) {
        AL_IconSet(tp, MD_ICON_MOJYA, 240);
        
        ALW_AttentionOff(tp);
        return BHV_RET_FINISH;
    }
    
    if (MOV_Dist2FromAim(tp) < 64.0) {
        
    }

    MOV_TurnToAim2(tp, 384);

    if(move->Flag & 0x400) {
        const Uint16 skill = AL_ParameterGetSkill(tp, SKILL_RUN);
        const float acc = GET_GLOBAL()->SkillRunAccBase + skill * GET_GLOBAL()->SkillRunAccRatio;
        
        move->Acc.x = njSin(work->ang.y) * acc;
        move->Acc.z = njCos(work->ang.y) * acc;
    }

    if (AL_IsEmotionTimerReset(tp)) {
        AL_EmotionAdd(tp, EM_ST_TIRE, GET_GLOBAL()->ParamAddTireWalk);
        AL_EmotionAdd(tp, EM_ST_TEDIOUS, -GET_GLOBAL()->ParamSubTediousWalk);
    }

    if(bhv->Timer > 3000) {
        ALW_AttentionOff(tp);
        return BHV_RET_FINISH;
    }

    if(AL_EmotionGetValue(tp, EM_ST_TEDIOUS) < 1000 || AL_EmotionGetValue(tp, EM_ST_TIRE) > 7000) {
        ALW_AttentionOff(tp);
        return BHV_RET_FINISH;
    }
    
    return BHV_RET_CONTINUE;
}

Bool AL_CheckBall(task* tp) {
    Uint32 tedious;
    
    if(!BallFlag) {
        return FALSE;
    }

    tedious = AL_EmotionGetValue(tp, EM_ST_TEDIOUS);
    
    if(tedious > 4000) {
        if(njDistanceP2P(&pBallTask->twp->pos, &tp->twp->pos) < 50.f) {
            AL_EmotionAdd(tp, EM_ST_TEDIOUS, -2000);
            AL_SetBehavior(tp, ALBHV_ChaseBall);
        
            return TRUE;
        }
    }

    return FALSE;
}