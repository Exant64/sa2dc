#include "Chao/Chao.h"
#include "Chao/al_behavior/albhv_move.h"
#include "playsound.h"
#include <Chao/al_landmark.h>
#include <player.h>

int ALBHV_Awawa(task* tp);
int ALBHV_RunawayFromPlayer(task* tp);
void AL_CalcIntentionScore_Fear(task* tp, float* pMaxScore) {
    float score = 0.0f;
    CHAO_GLOBAL* Global = GET_GLOBAL();
    int value = AL_EmotionGetValue(tp, EM_MD_FEAR);
    if (value > Global->IntentionFearTrigger) {
        bool trigger = FALSE;
        task* v10 = AL_GetFoundPlayerTask(tp);
        if (v10) {
            int fear = AL_KW_GetPlayerFear(tp);

            Uint32 feartrigger = Global->IntentionFearPlayerTrigger;
            if (fear > feartrigger) {
                score = AL_CalcMoodScoreTypeA(value, feartrigger);
                score *= Global->IntentionFearMaxScore;
                AL_ScoreRandomise(&score);
            }

            trigger = TRUE;
            if (score > *pMaxScore) {
                AL_SetBehavior(tp, ALBHV_Awawa);
                AL_SetNextBehavior(tp, ALBHV_RunawayFromPlayer);
                AL_ClearIntention(tp);
                *pMaxScore = score;
            }
        }
        if (!trigger) {
            score = 1.0 -
                    ((100 + AL_EmotionGetValue(tp, EM_PER_AGRESSIVE)) * (1.0 - Global->IntentionFearMinScore) * 0.005f);
            // AL_ScoreRandomise(&score);
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
    MOVE_WORK* v1 = (MOVE_WORK*)tp->Data2;

    NJS_VECTOR a1a;
    NJS_VECTOR a2;
    int v4 = AL_GetCurrLandAttr(&work->pos);
    float con = 10.0f;
    if (v4) {
        a2.x = work->pos.x - playertwp[0]->pos.x;
        a2.y = 0.0;
        a2.z = work->pos.z - playertwp[0]->pos.z;
        njUnitVector(&a2);
        a1a.x = (con * a2.x) + work->pos.x;
        a1a.y = work->pos.y;
        a1a.z = (con * a2.z) + work->pos.z;
        if (v4 != AL_GetCurrLandAttr(&a1a)) {
            if (njRandom() < 0.5f) {
                a1a.x = (con * a2.z) + work->pos.x;
                a1a.y = work->pos.y;
                a1a.z = work->pos.z - (con * a2.x);
            } else {
                a1a.x = work->pos.x - (con * a2.z);
                a1a.y = work->pos.y;
                a1a.z = (con * a2.x) + work->pos.z;
            }

            if (v4 != AL_GetCurrLandAttr(&a1a)) {
                AL_GetRandomAttrPos(v4, &a1a);
            }
        }
    } else {
        AL_GetRandomAttrPos(LMA_GROUND1, &a1a);
    }
    v1->AimPos = a1a;
}

int ALBHV_RunawayFromPlayer(task* tp) {

    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    chaowk* work = GET_CHAOWK(tp);
    MOVE_WORK* v2 = (MOVE_WORK*)tp->Data2;
    Uint32 skill = AL_ParameterGetSkill(tp, SKILL_SWIM);

    switch (bhv->Mode) {
        case 0: {
            float randval = njRandom();
            if (randval < 0.5f) {
                AL_SetMotionLinkStep(tp, ALM_OIKAKE, 10);
            } else {
                AL_SetMotionLinkStep(tp, ALM_RUN_BANZAI, 10);
            }
            AL_FaceChangeEye(tp, AL_EYE_NUM_KYA);
            AL_FaceChangeMouth(tp, AL_MOUTH_NUM_KOIKE);
            sub_8C05B020(24622, 0, 0, 110, &GET_CHAOWK(tp)->pos);
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

    if (AL_Dist2FromAim(tp) < 64.0f) {
        if (MOV_Dist2FromPlayer(tp, 0) < 2500.0f) {
            bhv->Mode = 0;
        } else {
            return BHV_RET_FINISH;
        }
    }

    if (v2->Flag & 0x4000) {
        bhv->Mode = 0;
    }

    MOV_TurnToAim2(tp, 288);

    if (v2->Flag & 0x400) {
        v2->Acc.x = GET_GLOBAL()->RunAcc * njSin(work->ang.y);
        v2->Acc.z = GET_GLOBAL()->RunAcc * njCos(work->ang.y);
    }

    AL_SetMotionSpd(tp, njScalor(&v2->Velo) * 8.0f);

    return BHV_RET_CONTINUE;
}
