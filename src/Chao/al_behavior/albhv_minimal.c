#include <Chao/Chao.h>

#include <Chao/alo_growtree.h>
#include <Chao/al_field.h>

int ALBHV_GoToLockOn(task*);
int ALBHV_TurnToLockOn(task*);

extern BHV_FUNC ALBHV_MinimalFunc[eMinimalType__end];

int ALBHV_Kaoarau(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_KAOARAU);
            
            bhv->Mode++;
            bhv->Timer = RAND_RANGE(2, 3.999999);
        case 1:
            if (AL_IsMotionEnd(tp)) {
                if(--bhv->Timer <= 0) {
                    return BHV_RET_FINISH;
                }
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_UnusedMinimal(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_UTAU_LEFT);
            
            bhv->Mode++;
            bhv->Timer = RAND_RANGE(4, 5.999999);
        case 1:
            if (AL_IsMotionStop(tp)) {
                if(--bhv->Timer <= 0) {
                    return BHV_RET_FINISH;
                }
                else {
                    AL_SetMotionLink(tp, ALM_UTAU_LEFT);
                }
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Bakuten(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_BAKUTEN);
            
            bhv->Mode++;
        case 1:
            if (AL_IsMotionStop(tp)) {
                return BHV_RET_FINISH;
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Draming(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_DRAMING);

            bhv->Timer = RAND_RANGE(120, 220);
            bhv->Mode++;
        case 1:
            if(bhv->Timer-- <= 0) {
                if(njRandom() < 0.4f) {
                    return BHV_RET_FINISH;
                }

                if(njRandom() < 0.4f) {
                    AL_SetMotionLink(tp, ALM_DRAMING_WOO);
                    sub_8C05B020(24711, 0, 0, 110, &tp->twp->pos);
                    
                    bhv->Mode++;
                }
                else {
                    AL_SetMotionLink(tp, ALM_DRAMING);
                    bhv->Timer = RAND_RANGE(160, 280);
                }
            }
            break;
        
        case 2:
            if (AL_IsMotionStop(tp)) {
                AL_SetMotionLink(tp, ALM_DRAMING);

                bhv->Timer = RAND_RANGE(160, 280);
                bhv->Mode--;
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Dosukoi(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_SUMO_DOSUKOI);

            bhv->Timer = RAND_RANGE(120, 240);
            bhv->Mode++;
        case 1:
            if(bhv->Timer-- <= 0) {
                return BHV_RET_FINISH;
            }
            
            AL_ForwardSpd(tp, 0.05f);
            
            break;
    }
    
    return BHV_RET_CONTINUE;
}

int ALBHV_InoDash(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_ON_YOUR_MARK);

            bhv->Mode++;
        case 1:
            if (AL_IsMotionEnd(tp)) {
                AL_SetMotionLink(tp, ALM_READY);
                bhv->Mode++;
            }
            break;

        case 2:
            if (AL_IsMotionEnd(tp)) {
                AL_SetMotionLink(tp, ALM_GO);
                bhv->Mode++;
            }
            break;

        case 3:
            if (AL_IsMotionEnd(tp)) {
                AL_SetMotionLink(tp, ALM_DASH);

                bhv->Timer = RAND_RANGE(120, 180);
                bhv->Mode++;
            }

            AL_ForwardSpd(tp, 0.3f);
            break;

        case 4:
            AL_ForwardSpd(tp, 0.3f);
            
            if(bhv->Timer-- <= 0) {
                return BHV_RET_FINISH;
            }
            break;
    }

    AL_SetMotionSpd(tp, 1.5f);
    
    return BHV_RET_CONTINUE;
}

int ALBHV_Otakai(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            if(njRandom() < 0.5f) {
                AL_SetMotionLink(tp, ALM_OTAKAI_L);
            }
            else {
                AL_SetMotionLink(tp, ALM_OTAKAI_R);
            }

            bhv->Timer = RAND_RANGE(600, 900);
            bhv->Mode++;
        case 1:
            if(bhv->Timer-- <= 0) {
                return BHV_RET_FINISH;
            }
            break;
    }
    
    return BHV_RET_CONTINUE;
}

int ALBHV_ShiriDance(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_HIP_DANCE);

            bhv->Timer = RAND_RANGE(300, 500);
            bhv->Mode++;
        case 1:
            if(bhv->Timer-- <= 0) {
                return BHV_RET_FINISH;
            }
            break;
    }
    
    return BHV_RET_CONTINUE;
}

int ALBHV_Onara(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    taskwk* twk = tp->twp;

    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_OSYABURI);

            bhv->Timer = RAND_RANGE(120, 240);
            bhv->Mode++;
        case 1:
            if(bhv->Timer-- <= 0) {
                NJS_POINT3 pos;
                
                sub_8C05B020(4115, 0, 0, 0, &tp->twp->pos);

                bhv->Timer = RAND_RANGE(120, 240);

                pos.x = twk->pos.x - 3 * njSin(twk->ang.y);
                pos.y = twk->pos.y + 0.2f;
                pos.z = twk->pos.z - 3 * njCos(twk->ang.y);

                lbl_0C5406C0(&pos, 2.f, 0.1f, 2.f);
                AL_ChildFieldCreateT(tp, CI_KIND_AL_ONARA, &pos, 5, 60);

                bhv->Mode++;
            }
            break;
        
        case 2:
            if(bhv->Timer-- <= 0) {
                return BHV_RET_FINISH;
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Fire(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    AL_SHAPE* pShape = &GET_CHAOWK(tp)->Shape;

    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_FIRE);
            bhv->Mode++;
        case 1:
            if(bhv->Timer++ > 20) {
                NJS_VECTOR velo;

                velo.x = pShape->MouthVec.x * 0.5f;
                velo.y = pShape->MouthVec.y * 0.5f;
                velo.z = pShape->MouthVec.z * 0.5f;

                lbl_0C5451E0(&pShape->MouthPos, &velo, 0.25f);
            }

            if(AL_IsMotionStop(tp)) {
                return BHV_RET_FINISH;
            }
            
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_PyonPyon(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_PYONPYON);

            bhv->Mode++;
            bhv->Timer = RAND_RANGE(4, 6.999999);
        case 1:
            if(AL_IsMotionEnd(tp)) {
                if(--bhv->Timer <= 0) {
                    return BHV_RET_FINISH;
                }
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Fukkin2(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_FUKKIN);

            bhv->Mode++;
            bhv->Timer = RAND_RANGE(4, 6.999999);
        case 1:
            if(AL_IsMotionEnd(tp)) {
                if(--bhv->Timer <= 0) {
                    return BHV_RET_FINISH;
                }
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Fukkin(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    
    AL_SetBehavior(tp, ALBHV_PostureChangeSit);
    AL_SetNextBehavior(tp, ALBHV_Fukkin2);

    return BHV_RET_CONTINUE;
}

int ALBHV_Denguri(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_DENGURI);

            bhv->Mode++;
            bhv->Timer = RAND_RANGE(4, 6.999999);
        case 1:
            if(AL_IsMotionEnd(tp)) {
                bhv->Mode++;
            }
            break;
        case 2:
            if(AL_IsMotionEnd(tp)) {
                if(--bhv->Timer <= 0) {
                    return BHV_RET_FINISH;
                }
            }
            
            AL_ForwardSpd(tp, 0.1);
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_GoroGoro(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    MOVE_WORK* move = (MOVE_WORK*)tp->Data2;
    taskwk* twk = tp->twp;
    
    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_GOROGORO);

            bhv->Timer = RAND_RANGE(500, 800);
            bhv->Mode++;
        case 1:
            if(bhv->Timer-- <= 0) {
                return BHV_RET_FINISH;
            }

            twk->pos.x += njSin(twk->ang.y - NJM_DEG_ANG(90)) * 0.05f;
            twk->pos.z += njCos(twk->ang.y - NJM_DEG_ANG(90)) * 0.05f;
            
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_HaraSuberi(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_HAIHAI_SORU_UP);

            bhv->Mode++;
            bhv->Timer = RAND_RANGE(120, 180);
        case 1:
            if(AL_IsMotionEnd(tp)) {
                if(--bhv->Timer <= 0) {
                    return BHV_RET_FINISH;
                }
            }

            AL_ForwardSpd(tp, 0.15f);
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Shippo(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_TAIL);

            bhv->Mode++;
            bhv->Timer = RAND_RANGE(400, 600);

            sub_8C05B020(24708, 0, 0, 110, &tp->twp->pos);
        case 1:
            if(--bhv->Timer <= 0) {
                return BHV_RET_FINISH;
            }

            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_UnknownMinimal1(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_TOGU);

            bhv->Timer = RAND_RANGE(300, 500);
            bhv->Mode++;
        case 1:
            if(bhv->Timer-- <= 0) {
                return BHV_RET_FINISH;
            }

            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_UnknownMinimal2(task* tp) {
    al_perception_link* pTree = AL_GetFoundTree(tp);
    float chance;
    
    if(!pTree) {
        return BHV_RET_FINISH;
    }

    chance = 0.3f;
    
    if(njRandom() > chance && pTree->dist < 40 && pTree->pEntry) {
        task* pTreeTask = pTree->pEntry->tp;

        if(pTreeTask) {
            TREE_WORK* pTreeWork = pTreeTask->twp;
            int state = pTreeWork->state;
            
            switch(state) {
                case TREE_ST_ADULT:
                case TREE_ST_LOCAL:
                    ALW_LockOn(tp, pTree->pEntry->tp);
                    AL_SetBehavior(tp, ALBHV_GoToLockOn);
                    AL_SetNextBehavior(tp, ALBHV_TurnToLockOn);
                    AL_SetNextBehavior(tp, ALBHV_UnknownMinimal1);
                    
                    return BHV_RET_CONTINUE;
            }
        }
    }

    return BHV_RET_FINISH;
}

Bool AL_DecideBehaviorMinimal(task* tp) {
    float score = 0; // not necessarily score but no idea what else it would've been
    taskwk* twk = tp->twp;
    
    int i;

    int BhvList[32];
    int count;

    count = 0;

    for(i = 0; i < eMinimalType__end; i++) {
        if(AL_PartsIsMinimalFlagOn(tp, i)) {
            if(ALBHV_MinimalFunc[i]) {
                BhvList[count] = i;
                count++;
            }
        }
    }

    if(count > 0) {
        int num = BhvList[(Uint32)((count - 0.0001f) * njRandom())];
        AL_SetBehavior(tp, ALBHV_MinimalFunc[num]);
        
        return TRUE;
    }

    return FALSE;
}