#include <Chao/Chao.h>
#include <playsound.h>

int ALBHV_Mayu(task*);
int ALBHV_Dead(task*);

int ALBHV_Damage(task*);
int ALBHV_Bikkuri(task* tp);

int ALBHV_BornDamage(task* tp) {
    taskwk* work = tp->twp;
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLinkStep(tp, ALM_THROWN, 15);

            {
                MOVE_WORK* move = (MOVE_WORK*)tp->Data2;
                work->ang.y = njArcTan2(move->Velo.x, move->Velo.z);
            }
            
            AL_FaceChangeEye(tp, AL_EYE_NUM_KYA);
            AL_IconSet(tp, MD_ICON_MOJYA, 150);
            
            bhv->Mode++;
            break;
        case 1:
            if(work->flag & 1) {
                AL_SetMotionLinkStep(tp, ALM_KOKE, 15);

                bhv->Timer = RAND_RANGE(180, 300);
                bhv->Mode++;
            }
            break;
        case 2:
            if(--bhv->Timer <= 0) {
                return BHV_RET_FINISH;
            }
            break;
    }

    return BHV_RET_CONTINUE;    
}

Bool AL_CheckDamage(task* tp) {
    taskwk* work = tp->twp;
    BHV_FUNC func = AL_GetBehavior(tp);
    if(func == ALBHV_Damage || func == ALBHV_Mayu || func == ALBHV_Dead) {
        return FALSE;
    }

    if(work->flag & 4) {
        task* dmg = CCL_IsHitPlayerWithNum(tp, 1);
        
        work->flag &= ~4;

        if(dmg) {
            MOVE_WORK* move = (MOVE_WORK*)tp->Data2;
            const float range = 0.8f;
            
            NJS_VECTOR dir;
            dir.x = dmg->twp->pos.x - work->pos.x;
            dir.z = dmg->twp->pos.z - work->pos.z;

            njUnitVector(&dir);

            move->Velo.x = -dir.x * range;
            move->Velo.y = 1.5f;
            move->Velo.z = -dir.z * range;

            AL_SetBehavior(tp, ALBHV_Damage);

            if(GET_CHAOPARAM(tp)->body.ObakeHead) {
                ALO_ObakeHeadCreate(
                    GET_CHAOPARAM(tp)->body.ObakeHead,
                    &GET_CHAOWK(tp)->pos,
                    NJM_DEG_ANG(njRandom() * 360),
                    &move->Velo
                );

                GET_CHAOPARAM(tp)->body.ObakeHead = 0;
            }
            
            return TRUE;
        }
    }

    return FALSE;
}

int ALBHV_Damage(task* tp) {
    enum {
        MD_DAMAGE_INIT,
        MD_DAMAGE_KURU,
        MD_DAMAGE_PYU,
        MD_DAMAGE_KOKE,
    };

    taskwk* twk = tp->twp;
    chaowk* work = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &work->Behavior;

    switch(bhv->Mode) {
        case MD_DAMAGE_INIT:
            {
                MOVE_WORK* move = (MOVE_WORK*)tp->Data2;
                twk->ang.y = njArcTan2(move->Velo.x, move->Velo.z);
            }
            
            AL_FaceChangeEye(tp, AL_EYE_NUM_KYA);
            AL_IconSet(tp, MD_ICON_MOJYA, 150);

            if(njRandom() < 0.5f) {
                AL_SetMotionLinkStep(tp, ALM_HIMA_SIT, 15);
                bhv->Mode = MD_DAMAGE_KURU;
            }
            else {
                AL_SetMotionLinkStep(tp, ALM_THROWN, 15);
                bhv->Mode = MD_DAMAGE_PYU;
            }

            AL_ParameterAddUserLike(tp, -30, -GET_GLOBAL()->DamageSubAttr);
            AL_KW_AddPlayerFear(tp, 50);

            {
                Uint32 emotion;

                emotion = (Uint32)(njRandom() * 100.f + 50.f);
                AL_EmotionAdd(tp, EM_MD_ANGER, emotion);

                emotion = (Uint32)(njRandom() * 100.f + 50.f);
                AL_EmotionAdd(tp, EM_MD_SORROW, emotion);

                emotion = (Uint32)(njRandom() * 100.f + 50.f);
                AL_EmotionAdd(tp, EM_MD_FEAR, emotion);

                emotion = (Uint32)(njRandom() * 100.f + 50.f);
                AL_EmotionAdd(tp, EM_MD_SURPRISE, emotion);

                emotion = (Uint32)(njRandom() * 100.f + 50.f);
                AL_EmotionAdd(tp, EM_MD_PAIN, emotion);
            }
            
            break;
        case MD_DAMAGE_KURU:
            if(twk->flag & 1) {
                AL_SetMotionLinkStep(tp, ALM_KOKE, 15);

                bhv->Timer = RAND_RANGE(180, 300);
                work->pitch = 0;
                AL_IconSet(tp, MD_ICON_MOJYA, 150);

                bhv->Mode++;
            }
            else {
                work->pitch += 1024;
            }
            break;
        case MD_DAMAGE_PYU:
            if(twk->flag & 1) {
                AL_SetMotionLinkStep(tp, ALM_KOKE, 15);

                bhv->Timer = RAND_RANGE(180, 300);
                AL_IconSet(tp, MD_ICON_MOJYA, 150);

                bhv->Mode++;
            }
            break;
        case MD_DAMAGE_KOKE:
            if(--bhv->Timer <= 0) {
                return BHV_RET_FINISH;
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Thrown(task* tp) {
    enum {
        MD_DAMAGE_INIT,
        MD_DAMAGE_KURU,
        MD_DAMAGE_PYU,
        MD_DAMAGE_KOKE,
    };

    taskwk* twk = tp->twp;
    chaowk* work = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &work->Behavior;
    MOVE_WORK* move = tp->Data2;

    switch(bhv->Mode) {
        case MD_DAMAGE_INIT:
            {
                MOVE_WORK* move = (MOVE_WORK*)tp->Data2;
                twk->ang.y = njArcTan2(move->Velo.x, move->Velo.z);
            }
            
            AL_FaceChangeEye(tp, AL_EYE_NUM_KYA);
            AL_IconSet(tp, MD_ICON_MOJYA, 150);

            AL_SetMotionLinkStep(tp, ALM_THROWN, 15);
            bhv->Mode = MD_DAMAGE_PYU;

            AL_ParameterAddUserLike(tp, -5, -GET_GLOBAL()->ThrownSubAttr);
            AL_KW_AddPlayerFear(tp, 10);

            {
                Uint32 emotion;

                emotion = (Uint32)(njRandom() * 100.f + 50.f);
                AL_EmotionAdd(tp, EM_MD_ANGER, emotion);

                emotion = (Uint32)(njRandom() * 100.f + 50.f);
                AL_EmotionAdd(tp, EM_MD_SORROW, emotion);

                emotion = (Uint32)(njRandom() * 100.f + 50.f);
                AL_EmotionAdd(tp, EM_MD_FEAR, emotion);

                emotion = (Uint32)(njRandom() * 100.f + 50.f);
                AL_EmotionAdd(tp, EM_MD_SURPRISE, emotion);

                emotion = (Uint32)(njRandom() * 100.f + 50.f);
                AL_EmotionAdd(tp, EM_MD_PAIN, emotion);
            }
            
            break;
        case MD_DAMAGE_PYU:
            if(move->Flag & 0x800) {
                if(GET_CHAOPARAM(tp)->body.ObakeHead) {
                    ALO_ObakeHeadCreate(
                        GET_CHAOPARAM(tp)->body.ObakeHead,
                        &GET_CHAOWK(tp)->pos,
                        NJM_DEG_ANG(njRandom() * 360),
                        &move->Velo
                    );
    
                    GET_CHAOPARAM(tp)->body.ObakeHead = 0;
                }
            }
            
            if(twk->flag & 1) {
                AL_SetMotionLinkStep(tp, ALM_KOKE, 15);

                bhv->Timer = RAND_RANGE(180, 300);
                AL_IconSet(tp, MD_ICON_MOJYA, 150);

                bhv->Mode++;
            }
            break;
        case MD_DAMAGE_KOKE:
            if(--bhv->Timer <= 0) {
                return BHV_RET_FINISH;
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

Bool AL_CheckBikkuri(task* tp) {
    taskwk* work = tp->twp;
    BHV_FUNC func = AL_GetBehavior(tp);
    
    if(func == ALBHV_Bikkuri) {
        return FALSE;
    }

    if(CCL_IsHitKind2(tp, CI_KIND_AL_BIKKURI)) {
        AL_SetBehaviorWithTimer(tp, ALBHV_Bikkuri, 500);
        return TRUE;
    }

    return FALSE;
}

int ALBHV_Bikkuri(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    taskwk* twk = tp->twp;
    MOVE_WORK* move = tp->Data2;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_KUSAI_BATAN);
            
            AL_FaceSetEye(tp, AL_EYE_NUM_KYA, -1);
            AL_FaceSetMouth(tp, AL_MOUTH_NUM_KOIKE, -1);
            AL_IconSet(tp, MD_ICON_MOJYA, 300);

            bhv->Mode++;
            bhv->Timer = 0;

            if(njRandom() < 0.5f) {
                sub_8C05B020(24617, 0, 0, 110, &twk->pos);
            }
            else {
                sub_8C05B020(24685, 0, 0, 110, &GET_CHAOWK(tp)->pos);
            }
            break;
        case 1:
            break;
    }

    AL_Brake(tp, 0.9f);

    return BHV_RET_CONTINUE;
}