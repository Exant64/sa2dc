#include <Chao/Chao.h>

#include <Chao/al_knowledge.h>
#include <Chao/al_landmark.h>
#include <playsound.h>
#include <player.h>

int ALBHV_Awawa(task* tp);
int ALBHV_RunawayFromPlayer(task* tp);

void AL_CalcIntentionScore_Fear(task* tp, float* pMaxScore) {
    float score;
    Uint32 fear_trigger;
    Uint32 value;
    
    score = 0.f;
    fear_trigger = GET_GLOBAL()->IntentionFearTrigger;
    value = AL_EmotionGetValue(tp, EM_MD_FEAR);
    
    if (value > GET_GLOBAL()->IntentionFearTrigger) {
        Bool playerClose = FALSE;
        task* player = AL_GetFoundPlayerTask(tp);
        
        if (player) {
            int fear = AL_KW_GetPlayerFear(tp);

            fear_trigger = GET_GLOBAL()->IntentionFearPlayerTrigger;
            if (fear > fear_trigger) {
                score = AL_CalcMoodScoreTypeA(value, fear_trigger);
                score *= GET_GLOBAL()->IntentionFearMaxScore;
                AL_ScoreRandomise(&score);
            }

            playerClose = TRUE;
            if (score > *pMaxScore) {
                AL_SetBehavior(tp, ALBHV_Awawa);
                AL_SetNextBehavior(tp, ALBHV_RunawayFromPlayer);
                AL_ClearIntention(tp);
                *pMaxScore = score;
            }
        }
        
        if (!playerClose) {
            score = 1.0 -
                    ((100 + AL_EmotionGetValue(tp, EM_PER_AGRESSIVE)) * (1.0 - GET_GLOBAL()->IntentionFearMinScore) * 0.005f);
            
            if (score > *pMaxScore) {
                AL_SetBehavior(tp, ALBHV_Awawa);
                AL_ClearIntention(tp);
                *pMaxScore = score;
            }
        }
    }
}

int ALBHV_Awawa(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    switch (bhv->Mode) {
        case 0:
            if (njRandom() < 0.5f) {
                AL_SetMotionLink(tp, ALM_BURUBURU);
            } else {
                AL_SetMotionLink(tp, ALM_BURUBURU_AGO);
            }
            bhv->Mode++;
            AL_FaceChangeEye(tp, AL_EYE_NUM_KYA);
            AL_FaceChangeMouth(tp, AL_MOUTH_NUM_KOIKE);
            bhv->Timer = (Uint16)(180 + (int)(njRandom() * 301.0f));

        case 1:
            if (!(bhv->Timer % 150)) {
                if (njRandom() < 0.5f)
                    sub_8C05B020(24621, 0, 0, 110, &tp->twp->pos);
            }
            if (!--bhv->Timer)
                return BHV_RET_FINISH;
            break;
    }
    return BHV_RET_CONTINUE;
}

void AL_SetRunawayAimPos(task* tp) {
    chaowk* work = GET_CHAOWK(tp);
    MOVE_WORK* move = (MOVE_WORK*)tp->Data2;
    
    const int LandAttr = AL_GetCurrLandAttr(&work->pos);
    
    NJS_POINT3 AimPos;
    const float radius = 10.0f;
    
    if (LandAttr != LMA_NONE) {
        NJS_VECTOR dir;
        
        dir.x = work->pos.x - playertwp[0]->pos.x;
        dir.y = 0;
        dir.z = work->pos.z - playertwp[0]->pos.z;
        
        njUnitVector(&dir);
        
        AimPos.x = (radius * dir.x) + work->pos.x;
        AimPos.y = work->pos.y;
        AimPos.z = (radius * dir.z) + work->pos.z;
        
        if (LandAttr != AL_GetCurrLandAttr(&AimPos)) {
            if (njRandom() < 0.5f) {
                AimPos.x = work->pos.x + (radius * dir.z);
                AimPos.y = work->pos.y;
                AimPos.z = work->pos.z - (radius * dir.x);
            } 
            else {
                AimPos.x = work->pos.x - (radius * dir.z);
                AimPos.y = work->pos.y;
                AimPos.z = work->pos.z + (radius * dir.x);
            }

            if (LandAttr != AL_GetCurrLandAttr(&AimPos)) {
                AL_GetRandomAttrPos(LandAttr, &AimPos);
            }
        }
    } 
    else {
        AL_GetRandomAttrPos(LMA_GROUND1, &AimPos);
    }
    
    move->AimPos = AimPos;
}

int ALBHV_RunawayFromPlayer(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    chaowk* work = GET_CHAOWK(tp);
    MOVE_WORK* move = (MOVE_WORK*)tp->Data2;
    Uint16 skill = AL_ParameterGetSkill(tp, SKILL_SWIM);

    switch (bhv->Mode) {
        case 0: {
            // they definitely intended more anims here
            float randval = njRandom();
            if (randval < 0.5f) {
                AL_SetMotionLinkStep(tp, ALM_OIKAKE, 10);
            }
            else {
                AL_SetMotionLinkStep(tp, ALM_RUN_BANZAI, 10);
            }
            
            AL_FaceChangeEye(tp, AL_EYE_NUM_KYA);
            AL_FaceChangeMouth(tp, AL_MOUTH_NUM_KOIKE);

            // function used was changed on sa2
            // uses the other one
            sub_8C05B020(24622, 0, 0, 110, &GET_CHAOWK(tp)->pos);

            // fake match i believe but debug asm doesn't indicate anything
            // i'm really doubting that almost anything i decomp just needs a random if(0) somewhere
            // but until we know more its better than not having it matching
            if(0);
            
            AL_SetRunawayAimPos(tp);
            
            bhv->Mode++;
            bhv->Timer = 0;
            bhv->SubTimer = 0;
        }
        case 1:
            break;
    }
    
    if (AL_IsEmotionTimerReset(tp)) {
        AL_EmotionAdd(tp, EM_ST_TIRE, GET_GLOBAL()->ParamAddTireRun);
    }

    if (bhv->Timer++ > 1000) {
        return BHV_RET_FINISH;
    }

    if (!(bhv->Timer % 100)) {
        if (njRandom() < 0.03f) {
            AL_SetBehavior(tp, ALBHV_Koke);
        }
    }

    if (AL_Dist2FromAim(tp) < 8*8) {
        if (MOV_Dist2FromPlayer(tp, 0) < 50*50) {
            bhv->Mode = 0;
        } else {            
            return BHV_RET_FINISH;
        }
    }

    if (move->Flag & 0x4000) {
        bhv->Mode = 0;
    }
    
    MOV_TurnToAim2(tp, 288);

    if (move->Flag & 0x400) {        
        move->Acc.x = njSin(work->ang.y) * GET_GLOBAL()->RunAcc;
        move->Acc.z = njCos(work->ang.y) * GET_GLOBAL()->RunAcc;
    }
    
    {
        float velo = njScalor(&move->Velo);
        AL_SetMotionSpd(tp, velo * 8);
    }
    
    return BHV_RET_CONTINUE;
}