#include <Chao/Chao.h>

#include <player.h>
#include <playsound.h>

int ALBHV_OsanpoThrown(task* tp);
int ALBHV_Thrown(task* tp);

int ALBHV_DakkoDislike(task* tp);
int ALBHV_DakkoNormal(task* tp);
int ALBHV_DakkoLike(task* tp);

void DakkoControl(task* tp) {
    taskwk* twk = tp->twp;
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    Angle3* pAimAng = &((MOVE_WORK*)tp->Data2)->AimAng;
    
    MOVE_WORK* move = (MOVE_WORK*)tp->Data2;
    move->Acc.y += -move->Gravity;

    AL_EmotionAdd(tp, EM_MD_RELAX, 200);

    if(!(twk->flag & TWK_FLAG_HELD)) {
        if(!CheckPadReadMode()) {
            MOVE_WORK* move = (MOVE_WORK*)tp->Data2;

            NJS_VECTOR velo;
            velo.x = move->Velo.x;
            velo.y = 0;
            velo.z = move->Velo.z;

            if(njScalor(&velo) > 0) {
                njUnitVector(&velo);
            }

            twk->pos.y += 2;

            velo.x *= 0.2f;
            velo.y = 0.87f;
            velo.z *= 0.2f;

            MOV_SetVelo(tp, &velo);
            AL_SetBehavior(tp, ALBHV_OsanpoThrown);
        }
        else {
            int i;
            Bool flag = FALSE;
            playerwk* player = playerpwp[0];

            for(i = 0; i < player->action_num; i++) {
                if(player->action_list[i] == 20) {
                    MOV_ClearVelo(tp);
                    MOV_ClearAcc(tp);

                    pAimAng->x = 0;

                    AL_SetBehavior(tp, ALBHV_Think);
                    GET_CHAOWK(tp)->ChaoFlag |= CHAO_FLAG_CCL_ENABLE;
                    
                    flag = TRUE;
                    break;
                }
            }

            if(!flag) {
                pAimAng->x = 0;

                AL_SetBehavior(tp, ALBHV_Thrown);
                GET_CHAOWK(tp)->ChaoFlag |= CHAO_FLAG_CCL_ENABLE;
            }
        }
    }
    else {
        tp->twp->ang.y = -playertwp[0]->ang.y + pAimAng->y;
        pAimAng->y = AdjustAngle(pAimAng->y, pAimAng->x, 2048);
        
        MOV_ClearVelo(tp);
        MOV_ClearAcc(tp);
        
        if(bhv->SubTimer++ > 600) {
            Uint32 hunger = AL_EmotionGetValue(tp, EM_ST_HUNGER);

            if(hunger >= 10000) {
                if(AL_GetBehavior(tp) == ALBHV_DakkoDislike) {
                    switch (AL_GetStageNumber()) {
                        case 1:
                        case 2:
                        case 3:
                            StopHoldingTaskP(0);
                            AL_SetBehavior(tp, ALBHV_Think);
                    }
                }
            }

            if(hunger > 8000) {
                if(AL_GetBehavior(tp) == ALBHV_DakkoNormal) {
                    AL_SetBehavior(tp, ALBHV_DakkoDislike);
                }
            }

            if(hunger > 5000) {
                if(AL_GetBehavior(tp) == ALBHV_DakkoLike) {
                    AL_SetBehavior(tp, ALBHV_DakkoNormal);
                }
            }
            
            bhv->SubTimer = 0;
        }
    }
}

int ALBHV_DakkoLike(task* tp){
    taskwk* twk = tp->twp;
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    Angle3* pAimAng = &((MOVE_WORK*)tp->Data2)->AimAng;

    switch(bhv->Mode) {
        case 0: {
            float randval;
            
            AL_FaceChangeEye(tp, AL_EYE_NUM_NIKO);

            if(njRandom() < 0.5f) {
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_WAAI);
            }
            else {
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_NIKO);
            }

            randval = njRandom();

            bhv->Timer = (Uint16)(140 + (int)(njRandom() * 181.0f));

            if(randval < 0.25f) {
                AL_SetMotionLink(tp, ALM_DAKKO);
                
                AL_FaceReturnDefaultEye(tp);
                AL_FaceReturnDefaultMouth(tp);

                bhv->Timer = (Uint16)(200 + (int)(njRandom() * 181.0f));
            }
            else if(randval < 0.4f) {
                AL_SetMotionLink(tp, ALM_DAKKO_KIMO_LR);
            }
            else if(randval < 0.55f) {
                AL_SetMotionLink(tp, ALM_DAKKO_KIMO_FB);
            }
            else if(randval < 0.7f) {
                // on sa2b this is 261 and the 0.85f one doesnt exist
                AL_SetMotionLink(tp, ALM_DAKKO_KIMO_JITABATA);
            }
            else if(randval < 0.85f) {
                AL_SetMotionLink(tp, ALM_DAKKO_GLAD);
            }
            else {
                AL_SetMotionLink(tp, ALM_DAKKO_GLAD_CLAP);
            }

            GET_CHAOWK(tp)->ChaoFlag &= ~CHAO_FLAG_CCL_ENABLE;
            bhv->Mode++;
            
            break;
        }
        case 1:
            if(bhv->Timer-- <= 0) {
                bhv->Mode = 0;
            }
            break;
    }

    DakkoControl(tp);
    
    return BHV_RET_CONTINUE;
}

int ALBHV_DakkoNormal(task* tp){
    taskwk* twk = tp->twp;
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    Angle3* pAimAng = &((MOVE_WORK*)tp->Data2)->AimAng;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_DAKKO);

            GET_CHAOWK(tp)->ChaoFlag &= ~CHAO_FLAG_CCL_ENABLE;
            bhv->Mode++;
            
            break;
    }
    
    DakkoControl(tp);
    
    return BHV_RET_CONTINUE;
}

int ALBHV_DakkoDislike(task* tp) {
    taskwk* twk = tp->twp;
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    Angle3* pAimAng = &((MOVE_WORK*)tp->Data2)->AimAng;

    switch(bhv->Mode) {
         case 0: {
            float randval;
            
            AL_FaceChangeEye(tp, AL_EYE_NUM_KYA);
            AL_FaceChangeMouth(tp, AL_MOUTH_NUM_MUSU);

            randval = njRandom();

            if(randval < 0.3f) {
                AL_SetMotionLink(tp, ALM_DAKKO);
                
                AL_FaceReturnDefaultEye(tp);
                AL_FaceReturnDefaultMouth(tp);
            }
            else if(randval < 0.5f) {
                AL_SetMotionLink(tp, ALM_DAKKO_IYA);
            }
            else if(randval < 0.6f) {
                AL_SetMotionLink(tp, ALM_DAKKO_IYA_LR);
            }
            else if(randval < 0.7f) {
                AL_SetMotionLink(tp, ALM_DAKKO_IYA_UD);
            }
            else if(randval < 0.8f) {    
                AL_SetMotionLink(tp, ALM_DAKKO_IYA_HAND);
            }
            else if(randval < 0.9f) {
                AL_SetMotionLink(tp, ALM_DAKKO_IYA_L);
            }
            else {
                AL_SetMotionLink(tp, ALM_DAKKO_IYA_R);
            }

            bhv->Timer = (Uint16)(100 + (int)(njRandom() * 141.0f));

            if(njRandom() < 0.4f) {
                float randval = njRandom();

                if(randval < 0.33f) {
                    sub_8C05B020(0x6047, 0, 0, 110, &GET_CHAOWK(tp)->pos);
                }
                else if(randval < 0.66f) {
                    sub_8C05B020(0x6048, 0, 0, 110, &GET_CHAOWK(tp)->pos);
                }
                else {
                    sub_8C05B020(0x6049, 0, 0, 110, &GET_CHAOWK(tp)->pos);
                }
            }
             
            bhv->Mode++;
            GET_CHAOWK(tp)->ChaoFlag &= ~CHAO_FLAG_CCL_ENABLE;
            
            break;
        }
        case 1:
            if(bhv->Timer-- <= 0) {
                bhv->Mode = 0;
            }
            break;
    }
    
    DakkoControl(tp);
    
    return BHV_RET_CONTINUE;
}

int ALBHV_DakkoTron(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_DAKKO);
            AL_FaceSetEye(tp, AL_EYE_NUM_TRON, -1);
            AL_IconSet(tp, MD_ICON_MOJYA, 360);

            GET_CHAOWK(tp)->ChaoFlag &= ~CHAO_FLAG_CCL_ENABLE;
            bhv->Mode++;
        case 1:
            if(bhv->Timer++ > 400) {
                AL_IconSet(tp, MD_ICON_MOJYA, 360);
                bhv->Timer = 0;
            }

            AL_EmotionAdd(tp, EM_ST_SLEEP_DEPTH, -2);
            if(AL_EmotionGetValue(tp, EM_ST_SLEEP_DEPTH) == 0) {
                return BHV_RET_FINISH;
            }
            
            break;
    }   
    
    DakkoControl(tp);
    
    return BHV_RET_CONTINUE;
}

int ALBHV_Dakko(task* tp) {
    if(AL_EmotionGetValue(tp, EM_ST_SLEEP_DEPTH) > 500) {
        AL_SetBehavior(tp, ALBHV_DakkoTron);
    }
    else {
        int like = AL_KW_GetPlayerLike(tp);

        if(like > 50) { 
            AL_SetBehavior(tp, ALBHV_DakkoLike);
        }
        else if (like < -50) {
            AL_SetBehavior(tp, ALBHV_DakkoDislike);
        }
        else {
            AL_SetBehavior(tp, ALBHV_DakkoNormal);
        }

        if(AL_EmotionGetValue(tp, EM_MD_RELAX) < 10) {
            AL_ParameterAddUserLike(tp, 1, GET_GLOBAL()->DakkoAddAttr);
        }
    }
    
    GET_CHAOWK(tp)->ChaoFlag &= ~CHAO_FLAG_CCL_ENABLE;
    DakkoControl(tp);
    
    return BHV_RET_CONTINUE;
}

Bool AL_CheckDakko(task* tp) {
    BHV_FUNC func = AL_GetBehavior(tp);

    if(func == ALBHV_Dakko || func == ALBHV_DakkoLike || 
      func == ALBHV_DakkoNormal || func == ALBHV_DakkoDislike || 
      func == ALBHV_DakkoTron) {
        return FALSE;
    }

    if(tp->twp->flag & TWK_FLAG_HELD) {
        Angle3* pAimAng = &((MOVE_WORK*)tp->Data2)->AimAng;

        pAimAng->y = tp->twp->ang.y + playertwp[0]->ang.y - 4000;

        if(DiffAngle(MOV_CalcPlayerAngle(tp, 0), tp->twp->ang.y) < NJM_DEG_ANG(90)) {
            pAimAng->x = NJM_DEG_ANG(270);
        }
        else {
            pAimAng->x = NJM_DEG_ANG(90);
        }

        MOV_ClearVelo(tp);
        MOV_ClearAcc(tp);
        
        AL_SetBehavior(tp, ALBHV_Dakko);

        // name display
        lbl_0C512D1E(tp);
        
        return TRUE;
    }
    
    return FALSE;
}
