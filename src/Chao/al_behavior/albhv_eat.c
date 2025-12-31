#include <Chao/Chao.h>

#include <playsound.h>

int ALBHV_PostureChangeStand(task*tp);
int ALBHV_Notice(task*tp);
int ALBHV_GoToLockOn(task*tp);

int ALBHV_Sleep(task*);

int ALBHV_Eat(task*);
int ALBHV_TurnToFruit(task*);

Bool AL_CheckFruit(task *tp) {
    BHV_FUNC func = AL_GetBehavior(tp);
    task* pFruit;
    
    if(func == ALBHV_Eat || func == ALBHV_Sleep || func == ALBHV_TurnToFruit) {
        return FALSE;
    }

    if(pFruit = CCL_IsHitKind2(tp, CI_KIND_AL_FRUIT)) {
        if(!ALW_IsHeld(pFruit)) {
            MOV_SetAimPos(tp, &pFruit->twp->pos);
            AL_SetBehavior(tp, ALBHV_TurnToFruit);
            
            return TRUE;
        }
    }

    return FALSE;
}

int ALBHV_TurnToFruit(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_CAPTURE_MORAU, 15);

            bhv->Mode++;
            bhv->Timer = 60;
            
            break;
    }

    if(MOV_TurnToAim2(tp, 1536) < 182) {
        task* pFruit = CCL_IsHitKind2(tp, CI_KIND_AL_FRUIT);
        if(pFruit) {
            if(!ALW_IsHeld(pFruit)) {
                // extra sounds on sadx?
                
                StopHoldingTaskP(0);
                AL_GrabObjectBothHands(tp, pFruit);
                
                AL_SetBehavior(tp, ALBHV_Eat);
            }
        }
        else {
            if(bhv->Timer-- <= 0) {
                sub_8C05B020(24631, 0, 0, 110, &tp->twp->pos);
                return BHV_RET_FINISH;
            }
        }
    }
    
    return BHV_RET_CONTINUE;
}

enum {
    FREE_EAT_NONE = 0x0,
    FREE_EAT_GATUGATU = 0x1,
    FREE_EAT_NORMAL = 0x2,
};

enum {
    MD_INIT = 0,
    MD_SUWARU = 1,
    MD_KUNKUN = 2,
    MD_UUUUN = 3,
    MD_DECIDE_MOTION = 4,
    MD_TUMARU = 5,
    MD_TABERU = 6,
    MD_MIAGERU = 7,
    MD_MODORU = 8,
    MD_SUTERU_START = 9, // throwaway
    MD_SUTERU = 10,
    MD_MANPUKU = 11, // full stomach
    MD_MANPUKU_SURISURI = 12,
    MD_TATSU = 13,
};

int ALBHV_Eat(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case MD_INIT:
            AL_SetMotionLink(tp, ALM_TABERU_SUWARU);
            bhv->Mode = MD_SUWARU;
            bhv->SubTimer = 100 + (Uint16)(njRandom() * 300);
        case MD_SUWARU:
            if(AL_IsMotionEnd(tp)) {
                al_entry_work* pFruitEntry = ALW_IsCommunicationEx(tp, ALW_CATEGORY_FRUIT);
                task* pFruit = NULL;

                if(pFruitEntry) {
                    pFruit = pFruitEntry->tp;
                }

                if(AL_EmotionGetValue(tp, EM_PER_GLUTTON) >= 0 || AL_EmotionGetValue(tp, EM_PER_CURIOSITY) >= 0 || AL_EmotionGetValue(tp, EM_ST_HUNGER) > 5000) {
                    if(njRandom() > 0.5f) {
                        AL_SetMotionLink(tp, ALM_EAT_KUNKUN);
                        
                        bhv->Mode = MD_KUNKUN;
                        bhv->SubTimer = 120;
                    }
                    else {
                        bhv->Mode = MD_DECIDE_MOTION;
                    }
                }
                else {
                    bhv->Mode = MD_DECIDE_MOTION;
                }

                if(pFruit) {
                    int type = ALO_GetFruitKind(pFruit);

                    switch(type) {
                        case 3:
                        case 4:
                        case 5:
                        case 13:
                            if(AL_IsDark2(tp)) {
                                bhv->Mode = MD_SUTERU_START;
                                
                                AL_ParameterAddUserLike(tp, -1, -GET_GLOBAL()->FruitDislikeSubAttr);
                                
                                sub_8C05B020(24634, 0, 0, 110, &tp->twp->pos);
                            }
                            break;
                        case 6:
                        case 7:
                        case 8:
                        case 14:
                            if(AL_IsHero2(tp)) {
                                bhv->Mode = MD_SUTERU_START;
                                
                                AL_ParameterAddUserLike(tp, -1, -GET_GLOBAL()->FruitDislikeSubAttr);
                                
                                sub_8C05B020(24634, 0, 0, 110, &tp->twp->pos);
                            }
                            break;
                        case 19: {
                            int taste = GET_CHAOPARAM(tp)->Emotion.Taste;
                            switch(taste) {
                                case TASTE_LT_DS:
                                case TASTE_LC_DS:
                                    bhv->Mode = MD_SUTERU_START;
                                    AL_ParameterAddUserLike(tp, -1, -GET_GLOBAL()->FruitDislikeSubAttr);
                                    sub_8C05B020(24634, 0, 0, 110, &tp->twp->pos);
                                    break;
                            }
                            break;
                        }
                        case 20: {
                            int taste = GET_CHAOPARAM(tp)->Emotion.Taste;
                            switch(taste) {
                                case TASTE_LS_DT:
                                case TASTE_LC_DT:
                                    bhv->Mode = MD_SUTERU_START;
                                    AL_ParameterAddUserLike(tp, -1, -GET_GLOBAL()->FruitDislikeSubAttr);
                                    sub_8C05B020(24634, 0, 0, 110, &tp->twp->pos);
                                    break;
                            }
                            break;
                        }
                        case 21: {
                            int taste = GET_CHAOPARAM(tp)->Emotion.Taste;
                            switch(taste) {
                                case TASTE_LS_DC:
                                case TASTE_LT_DC:
                                    bhv->Mode = MD_SUTERU_START;
                                    AL_ParameterAddUserLike(tp, -1, -GET_GLOBAL()->FruitDislikeSubAttr);
                                    sub_8C05B020(24634, 0, 0, 110, &tp->twp->pos);
                                    break;
                            }
                        }
                    }
                }
            }
            break;

        case MD_KUNKUN:
            if(bhv->SubTimer > 0) {
                bhv->SubTimer--;
            }
            else {
                if(AL_IsMotionEnd(tp)) {
                    bhv->SubTimer = 60;

                    AL_SetMotionLink(tp, ALM_EAT_KUNKUN_UPPER);
                    AL_FaceChangeEye(tp, AL_EYE_NUM_NIKO);

                    bhv->Mode = MD_UUUUN;

                    sub_8C05B020(24633, 0, 0, 110, &tp->twp->pos);
                }
            }
            break;

        case MD_UUUUN:
            if(bhv->SubTimer >0) {
                bhv->SubTimer--;
            }
            else {
                if(AL_IsMotionEnd(tp)) {
                    bhv->Mode = MD_DECIDE_MOTION;
                }
            }

            break;

        case MD_DECIDE_MOTION: {
            switch(AL_BehaviorGetFreeWork(tp)) {
                case FREE_EAT_NONE:
                    if(AL_EmotionGetValue(tp, EM_PER_GLUTTON) > 50 || AL_EmotionGetValue(tp, EM_ST_HUNGER) > 8000) {
                        AL_SetMotionLink(tp, ALM_EAT_GATUGATU);
                        AL_BehaviorSetFreeWork(tp, FREE_EAT_GATUGATU);
                        sub_8C05B020(24636, 0, 0, 110, &tp->twp->pos);
                    }
                    else {
                        AL_SetMotionLink(tp, ALM_EAT);
                        AL_BehaviorSetFreeWork(tp, FREE_EAT_NORMAL);
                        sub_8C05B020(24635, 0, 0, 110, &tp->twp->pos);
                    }
                    break;

                case FREE_EAT_NORMAL:
                    AL_SetMotionLink(tp, ALM_EAT);
                    sub_8C05B020(24635, 0, 0, 110, &tp->twp->pos);
                    break;
                
                case FREE_EAT_GATUGATU:
                    AL_SetMotionLink(tp, ALM_EAT_GATUGATU);
                    sub_8C05B020(24636, 0, 0, 110, &tp->twp->pos);
                    break;   
            }

            bhv->Mode = MD_TABERU;
            AL_FaceChangeEye(tp, AL_EYE_NUM_NIKO);
        }
        case MD_TABERU:
            ALW_SendCommand(tp, ALW_CMD_EAT);

            if(ALW_RecieveCommand(tp) == ALW_CMD_FINISH) {
                bhv->SubTimer = 180 + (Uint16)(njRandom() * 200.f);
                AL_SetMotionLinkStep(tp, ALM_MANPUKU_LOOP, 60);

                bhv->Mode = MD_MANPUKU;
                
                ALW_CommunicationOff(tp);

                AL_FaceChangeEye(tp, AL_EYE_NUM_NIKO);
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_HOYO);

                sub_8C05B020(24642, 0, 0, 110, &tp->twp->pos);
            }
            else {
                if(--bhv->SubTimer <= 0) {
                    if(AL_EmotionGetValue(tp, EM_PER_GLUTTON) > 50 && njRandom() < 0.4f) {
                        AL_SetMotionLink(tp, ALM_EAT_TUMARU);

                        bhv->SubTimer = 2 + (Uint16)(njRandom() * 5);
                        bhv->Mode = MD_TUMARU;

                        sub_8C05B020(24637, 0, 0, 110, &tp->twp->pos);

                        AL_FaceChangeEye(tp, AL_EYE_NUM_KYA);
                    }
                    else {
                        
                        if(njRandom() < 0.5f) {
                            AL_SetMotionLink(tp, ALM_EAT_LOOKUP_L);
                        }
                        else {
                            AL_SetMotionLink(tp, ALM_EAT_LOOKUP_R);
                        }

                        if(njRandom() < 0.3f) {
                            bhv->SubTimer = 10;
                        }
                        else {
                            bhv->SubTimer = 40 + (Uint16)(njRandom() * 200);
                        }
                        
                        AL_FaceChangeEye(tp, AL_EYE_NUM_NORMAL);
                        
                        bhv->Mode = MD_MIAGERU;
                    }
                }

                if (AL_IsEmotionTimerReset(tp)) {
                    AL_EmotionAdd(tp, EM_ST_HUNGER, -GET_GLOBAL()->ParamSubHunger);

                    // mistakenly uses ParamSubHunger? (all versions)
                    AL_EmotionAdd(tp, EM_ST_TIRE, -GET_GLOBAL()->ParamSubHunger);
                    
                    if (AL_EmotionGetValue(tp, EM_ST_HUNGER) <= 0) {
                        bhv->Mode = MD_SUTERU_START;
                    }
                }
            }
            break;

        case MD_TUMARU:
            if(AL_IsMotionEnd(tp)) {
                if (--bhv->SubTimer <= 0) {
                    AL_SetMotionLink(tp, ALM_TABERU_MODORU);
                    bhv->Mode = MD_MODORU;
                }
            }
            break;

        case MD_MIAGERU:
            if(AL_IsMotionEnd(tp)) {
                if(--bhv->SubTimer <= 0) {
                    if(njRandom() < 0.65f) {
                        AL_SetMotionLink(tp, ALM_TABERU_MODORU);
                        bhv->Mode = 8;
                    }
                    else {
                        switch(AL_GetMotionNum(tp)) {
                            case ALM_EAT_LOOKUP_L:
                                AL_SetMotionLinkStep(tp, ALM_EAT_LOOKUP_R, 25);
                                break;
                            case ALM_EAT_LOOKUP_R:
                                AL_SetMotionLinkStep(tp, ALM_EAT_LOOKUP_L, 25);
                                break;
                        }

                        if(njRandom() < 0.3f) {
                            bhv->SubTimer = 10;
                        }
                        else {
                            bhv->SubTimer = 40 + (Uint16)(njRandom() * 200);
                        }
                    }
                }
            }
            break;

        case MD_MODORU:
            if(AL_IsMotionEnd(tp)) {
                bhv->Mode = MD_DECIDE_MOTION;
                bhv->SubTimer = 180 + (Uint16)(njRandom() * 400);
            }
            break;

        case MD_SUTERU_START:
            AL_SetMotionLink(tp, ALM_SUTERU_SIT);
            bhv->Mode = MD_SUTERU;
        case MD_SUTERU:
            if(AL_GetMotionNum(tp) == ALM_SUTERU_SIT && AL_GetMotionFrame(tp) > 4) {
                al_entry_work* pFruitEntry = ALW_IsCommunication(tp);

                bhv->Mode = MD_TATSU;

                if(pFruitEntry) {
                    NJS_VECTOR vec;
                    
                    task* pFruit = pFruitEntry->tp;
                    NJS_POINT3* pPos = &tp->twp->pos;
                    NJS_POINT3* pFruitPos = &pFruit->twp->pos;
                    int ang = tp->twp->ang.y + NJM_DEG_ANG(160);

                    vec.x = 0.3f * njSin(ang);
                    vec.y = 0.7f;
                    vec.z = 0.3f * njCos(ang);

                    MOV_SetVelo(pFruit, &vec);
                }

                // CloseParameterFukidasi here on sa2b
                ALW_CommunicationOff(tp);
            }
            break;

        case MD_MANPUKU:
            if(bhv->SubTimer-- <= 0) {
                if(njRandom() < 0.6f) {
                    AL_SetMotionLink(tp, ALM_MANPUKU_SURI);

                    bhv->Mode = MD_MANPUKU_SURISURI;
                    bhv->Timer = 0;
                    bhv->SubTimer = 0;

                    AL_FaceChangeEye(tp, AL_EYE_NUM_NIKO);
                    AL_FaceChangeMouth(tp, AL_MOUTH_NUM_HOYO);
                }
                else {
                    if(njRandom() < 0.2f) {
                        AL_SetMotionLink(tp, ALM_STANDUP);

                        bhv->Mode = MD_TATSU;
                    }
                    else {
                        AL_ClearIntention(tp);

                        return BHV_RET_FINISH;
                    }
                }
            }
            break;

        case MD_MANPUKU_SURISURI:
            if(AL_IsMotionEnd(tp)) {
                AL_SetMotionLink(tp, ALM_MANPUKU_LOOP);
                
                bhv->Mode = MD_MANPUKU;
                bhv->SubTimer = 60 + (Uint16)(njRandom() * 100.f);

                AL_FaceChangeEye(tp, AL_EYE_NUM_NORMAL);
                AL_FaceChangeMouth(tp, AL_MOUTH_NUM_NIKO);
            }
            break;

        case MD_TATSU:
            if(AL_IsMotionStop(tp)) {
                AL_ClearIntention(tp);
                return BHV_RET_FINISH;
            }
            break;
    }

    if(!AL_MoveHoldingObject(tp)) {
        if(ALW_RecieveCommand(tp) == ALW_CMD_BYE) {
            // bit of an overreaction...

            AL_ParameterAddUserLike(tp, -1, -GET_GLOBAL()->FruitPakuriSubAttr);

            AL_EmotionAdd(tp, EM_MD_ANGER, 200);
            AL_EmotionAdd(tp, EM_MD_SORROW, 200);
        }

        return BHV_RET_FINISH;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_PickUpLockOn(task* tp) {
    enum {
        MD_PULO_PICKUP = 0x0,
        MD_PULO_PICKUP2 = 0x1,
        MD_PULO_PICKUP3 = 0x2,
    };

    chaowk* work = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &work->Behavior;

    switch(bhv->Mode) {
        case MD_PULO_PICKUP:
            AL_SetMotionLink(tp, 188);

            sub_8C05B020(4097, 0, 0, 0, &tp->twp->pos);

            bhv->LimitTimer = 1800;
            bhv->Mode++;
            
            break;
        case MD_PULO_PICKUP2:
            if (AL_IsMotionEnd(tp)) {
                task* pLock = ALW_GetLockOnTask(tp);
                
                if (!ALW_IsSheAttentionOtherOne(tp, pLock)) {
                    AL_GrabObjectBothHands(tp, pLock);
                    AL_MoveHoldingObject(tp);
                    
                    bhv->Mode++;
                    bhv->Timer = 0;
                    bhv->SubTimer = 0;
                }
            }
        case MD_PULO_PICKUP3:
            AL_MoveHoldingObject(tp);

            if(AL_IsMotionEnd(tp)) {
                return BHV_RET_FINISH;
            }
            
            break;
    }

    if(--bhv->LimitTimer <= 0) {
        return BHV_RET_BREAK;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_GoToEat(task* tp) {
    task* pFruit = AL_GetFoundFruitTask(tp);

    if(pFruit) {
        if(!ALW_IsSheAttentionOtherOne(tp, pFruit)) {
            ALW_LockOn(tp, pFruit);
    
            AL_SetBehavior(tp, ALBHV_PostureChangeStand);
            AL_SetNextBehavior(tp, ALBHV_Notice);
            AL_SetNextBehavior(tp, ALBHV_GoToLockOn);
            AL_SetNextBehavior(tp, ALBHV_PickUpLockOn);
            AL_SetNextBehavior(tp, ALBHV_Eat);
            
            return BHV_RET_CONTINUE;
        }
    }
    else {
        return BHV_RET_FINISH;
    }

    // no default return, undefined behavior!   
}