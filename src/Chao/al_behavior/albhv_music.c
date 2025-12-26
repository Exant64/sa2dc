#include <Chao/Chao.h>
#include <Chao/al_misc.h>

int ALBHV_ListenMusic(task* tp);

extern BHV_FUNC ALBHV_MusicFunc[];
extern NJS_CNK_OBJECT object_alo_maracas_pos_l_maracas1[];
extern NJS_CNK_OBJECT object_alo_maracas_pos_r_maracas[];
extern NJS_CNK_OBJECT object_alo_dram_pos_l_batil[];
extern NJS_CNK_OBJECT object_alo_dram_pos_r_batir[];
extern NJS_CNK_OBJECT object_alo_dram_pos_c_dram[];
extern NJS_CNK_OBJECT object_alo_rappa_pos_pos[];
extern NJS_CNK_OBJECT object_alo_sinbal_pos_l_sinbal2[];
extern NJS_CNK_OBJECT object_alo_sinbal_pos_r_sinbal1[];
extern NJS_CNK_OBJECT object_alo_tambarin_pos_tambarin[];
extern AL_ITEM_INFO BellItemInfo, FueItemInfo, CastanetsItemInfo;

void AL_CalcIntentionScore_JoinMusic(task* tp, float* pMaxScore) {
    float score = 0.0f;
    Uint32 trigger = GET_GLOBAL()->IntentionHimaTrigger >> 1;
    Uint32 value = AL_EmotionGetValue(tp, EM_ST_TEDIOUS);
    task* pField = AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_RANDOM_MUSIC);

    int indices[8];
    int len = 0;

    if (*pMaxScore < 1.0) {
        if (pField) {
            int i;

            for (i = 0; i < 8; i++) {
                if (AL_KW_IsMusicFlagOn(tp, i)) {
                    indices[len] = i;
                    len++;
                }
            }

            if (value > trigger) {
                if (value < 7000) {
                    value += 3000;
                }
                score = AL_CalcScoreTypeA(value, trigger);
                score *= GET_GLOBAL()->IntentionHimaMaxScore;
                AL_ScoreRandomise(&score);
            }

            if (score > *pMaxScore) {
                if (len > 0) {
                    int func_index = indices[(int)(njRandom() * (len - 0.00001f))];
                    MOV_SetAimPos(tp, &pField->twp->pos);
                    AL_SetBehavior(tp, ALBHV_TurnToAim);
                    AL_SetNextBehavior(tp, ALBHV_MusicFunc[func_index]);
                    AL_EmotionAdd(tp, EM_ST_TEDIOUS, -4000);
                    AL_EmotionAdd(tp, EM_ST_LONELY, -4000);
                    AL_ClearIntention(tp);

                    if (pField->ptp) {
                        AL_EmotionAdd(pField->ptp, EM_ST_LONELY, -4000);
                    }
                } else {
                    if (pField->ptp) {
                        MOV_SetAimPos(tp, &pField->ptp->twp->pos);
                    }
                    AL_SetBehavior(tp, ALBHV_TurnToAim);
                    AL_SetNextBehavior(tp, ALBHV_PostureChangeSit);
                    AL_SetNextBehavior(tp, ALBHV_ListenMusic);
                }
                *pMaxScore = 0.99f;
            }
        }
    }
}

Bool AL_DecideBehaviorMusic(task* tp) {
    chaowk* work = GET_CHAOWK(tp);

    int indices[8];
    int len = 0;

    // this is a mistake, AL_IsHitKindWithNum calls it too
    CCL_ClearSearch();
    if (!AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_RANDOM_MUSIC)) {
        int i;
        for (i = 0; i < 8; i++) {
            if (AL_KW_IsMusicFlagOn(tp, i)) {
                indices[len] = i;
                len++;
            }
        }

        if (len > 0) {
            NJS_VECTOR pos;
            Uint32 func_index = indices[(int)(njRandom() * (len - 0.00001f))];

            int ang = work->ang.y;
            pos.x = njSin(ang) * 8 + work->pos.x;
            pos.y = work->pos.y;
            pos.z = njCos(ang) * 8 + work->pos.z;

            AL_SetBehavior(tp, ALBHV_MusicFunc[func_index]);
            AL_ChildFieldCreateT(tp, CI_KIND_AL_RANDOM_MUSIC, &pos, 20.0, RAND_RANGE(1800, 1800 + 2400));
            return TRUE;
        }
    }
    return FALSE;
}

int ALBHV_Maracas(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    int mot_num;

    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_MARACAS_SHAKA_UP);

            AL_FaceSetEye(tp, AL_EYE_NUM_KYA, -1);
            AL_FaceSetMouth(tp, AL_MOUTH_NUM_SHARK, -1);

            AL_SetItem(tp, AL_PART_HAND_L, object_alo_maracas_pos_l_maracas1, &AL_TOY_TEXLIST);
            AL_SetItem(tp, AL_PART_HAND_R, object_alo_maracas_pos_r_maracas, &AL_TOY_TEXLIST);

            bhv->Timer = RAND_RANGE(300, 600);
            ++bhv->Mode;
        case 1:
            if (!bhv->Timer-- || AL_IsMotionStop(tp)) {
                if (njRandom() < 0.3f) {
                    AL_SetMotionLinkStep(tp, ALM_MARACAS_JUMP_B, 10);
                    sub_8C05B020(0x400B, 0, 0, 40, &GET_CHAOWK(tp)->pos);

                    if (njRandom() < 0.4f) {
                        sub_8C05B020(0x6033, 0, 0, 110, &GET_CHAOWK(tp)->pos);
                    }
                } else {
                    mot_num = AL_GetMotionNum(tp);
                    switch (mot_num) {
                        case ALM_MARACAS_SHAKA_UP:
                            AL_SetMotionLinkSync(tp, ALM_MARACAS_SHAKA_DOWN);
                            bhv->Timer = RAND_RANGE(60, 120);
                            break;
                        case ALM_MARACAS_SHAKA_DOWN:
                            AL_SetMotionLinkSync(tp, ALM_MARACAS_SHAKA_UP);
                            bhv->Timer = RAND_RANGE(60, 120);
                            break;
                        default:
                            AL_SetMotionLinkStep(tp, ALM_MARACAS_SHAKA_UP, 10);
                            bhv->Timer = RAND_RANGE(60, 120);
                            break;
                    }
                }
            }

            mot_num = AL_GetMotionNum(tp);
            switch (mot_num) {
                case ALM_MARACAS_SHAKA_UP:
                case ALM_MARACAS_SHAKA_DOWN:
                    if (bhv->SubTimer++ % 16 == 0) {
                        sub_8C05B020(0x400B, 0, 0, 100, &GET_CHAOWK(tp)->pos);
                    }
                    break;
            }
            break;
        default:
            break;
    }

    return AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_RANDOM_MUSIC) == NULL;
}

int ALBHV_Bell(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch (bhv->Mode) {
        case 0:
            if (njRandom() < 0.5f) {
                AL_SetMotionLink(tp, ALM_BELL_UP);
            } else {
                AL_SetMotionLink(tp, ALM_BELL_DOWN);
            }

            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);

            AL_SetItemOffset(tp, AL_PART_HAND_R, &BellItemInfo);

            bhv->Timer = RAND_RANGE(300, 600);
            ++bhv->Mode;
        case 1:
            if (bhv->SubTimer++ % 30 == 0) {
                sub_8C05B020(0x4006, 0, 0, 0, &GET_CHAOWK(tp)->pos);
            }

        default:
            break;
    }
    return AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_RANDOM_MUSIC) == NULL;
}

int ALBHV_Drum(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    int mot_num;

    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_DRUM_LR);

            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);

            AL_SetItem(tp, AL_PART_HAND_L, object_alo_dram_pos_l_batil, &AL_TOY_TEXLIST);
            AL_SetItem(tp, AL_PART_HAND_R, object_alo_dram_pos_r_batir, &AL_TOY_TEXLIST);
            AL_SetItem(tp, AL_PART_NAVEL, object_alo_dram_pos_c_dram, &AL_TOY_TEXLIST);

            bhv->Timer = RAND_RANGE(300, 600);
            ++bhv->Mode;
        case 1:
            if (bhv->Timer-- <= 0) {
                mot_num = AL_GetMotionNum(tp);
                switch (mot_num) {
                    case ALM_DRUM_LR:
                        if (njRandom() < 0.6f) {
                            AL_SetMotionLinkSync(tp, ALM_DRUM_LR);
                        } else {
                            AL_SetMotionLinkStep(tp, ALM_DRUM_DOUBLE, 10);
                        }

                        bhv->Timer = RAND_RANGE(60, 240);
                        break;
                    case ALM_DRUM_DOUBLE:
                        AL_SetMotionLinkStep(tp, ALM_DRUM_LR, 10);

                        bhv->Timer = RAND_RANGE(60, 240);
                        break;
                }
            }

            mot_num = AL_GetMotionNum(tp);
            switch (mot_num) {
                case ALM_DRUM_LR:
                    if (bhv->SubTimer++ % 16 == 0) {
                        sub_8C05B020(0x4009, 0, 0, 100, &GET_CHAOWK(tp)->pos);
                    }
                    break;
                case ALM_DRUM_DOUBLE:
                    if (bhv->SubTimer++ % 12 == 0 && njRandom() < 0.4f) {
                        sub_8C05B020(0x4009, 0, 0, 100, &GET_CHAOWK(tp)->pos);
                    }
                    break;
            }
            break;
    }

    return AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_RANDOM_MUSIC) == NULL;
}

int ALBHV_Fue(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    int mot_num;

    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_FUE_LR);

            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);

            AL_SetItemOffset(tp, AL_PART_HAND_R, &FueItemInfo);

            bhv->Timer = RAND_RANGE(300, 600);
            ++bhv->Mode;
        case 1:
            if (bhv->Timer-- <= 0) {
                bhv->Mode = 0;
            }

            if (bhv->SubTimer++ % 180 == 0 && njRandom() < 0.5f) {
                sub_8C05B020(0x400A, 0, 0, -40, &GET_CHAOWK(tp)->pos);
            }
            break;
    }

    return AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_RANDOM_MUSIC) == NULL;
}

int ALBHV_Castanets(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    int mot_num;

    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_CASTANETS_WAIT);
            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);

            AL_SetItemOffset(tp, AL_PART_HAND_L, &CastanetsItemInfo);
            bhv->Timer = RAND_RANGE(300, 600);
            ++bhv->Mode;
        case 1:
            if (bhv->Timer-- <= 0) {
                mot_num = AL_GetMotionNum(tp);
                switch (mot_num) {
                    case ALM_CASTANETS_WAIT:
                        if (njRandom() < 0.6f) {
                            AL_SetMotionLinkStep(tp, ALM_CASTANETS_PON, 10);
                        } else {
                            AL_SetMotionLinkStep(tp, ALM_CASTANETS_PONPON, 10);
                        }

                        bhv->Timer = RAND_RANGE(60, 240);
                        bhv->SubTimer = 0;
                        break;
                    default:
                        AL_SetMotionLinkStep(tp, ALM_CASTANETS_WAIT, 10);

                        bhv->Timer = RAND_RANGE(60, 240);
                        bhv->SubTimer = 0;
                        break;
                }
            }

            mot_num = AL_GetMotionNum(tp);
            bhv->SubTimer++;
            switch (mot_num) {
                case ALM_CASTANETS_WAIT:
                    break;
                case ALM_CASTANETS_PON:
                    if (bhv->SubTimer == 30) {
                        sub_8C05B020(0x4007, 0, 0, 120, &GET_CHAOWK(tp)->pos);
                    }
                    break;
                default:
                    if (bhv->SubTimer == 30) {
                        sub_8C05B020(0x4007, 0, 0, 120, &GET_CHAOWK(tp)->pos);
                    }
                    if (bhv->SubTimer == 60) {
                        sub_8C05B020(0x4007, 0, 0, 120, &GET_CHAOWK(tp)->pos);
                    }
                    break;
            }
            break;
    }

    return AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_RANDOM_MUSIC) == NULL;
}

int ALBHV_Rappa(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    int mot_num;

    switch (bhv->Mode) {
        case 0:
            // mistake?
            if (njRandom() < 0.5f) {
                AL_SetMotionLink(tp, ALM_RAPPA_UP);
            } else {
                AL_SetMotionLink(tp, ALM_RAPPA_UP);
            }

            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);

            AL_SetItemParallelLeftHand(tp, object_alo_rappa_pos_pos, &AL_TOY_TEXLIST);

            bhv->Timer = RAND_RANGE(300, 600);
            ++bhv->Mode;
        case 1:
            if (bhv->Timer-- <= 0) {
                bhv->Mode = 0;
            }

            if (bhv->SubTimer++ % 180 == 0 && njRandom() < 0.5f) {
                sub_8C05B020(0x400C, 0, 0, -200, &GET_CHAOWK(tp)->pos);
            }
            break;
    }

    return AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_RANDOM_MUSIC) == NULL;
}

int ALBHV_Cymbal(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    int mot_num;

    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_CYMBAL_WAIT);
            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);

            AL_SetItem(tp, AL_PART_HAND_L, object_alo_sinbal_pos_l_sinbal2, &AL_TOY_TEXLIST);
            AL_SetItem(tp, AL_PART_HAND_R, object_alo_sinbal_pos_r_sinbal1, &AL_TOY_TEXLIST);

            bhv->Timer = RAND_RANGE(300, 600);
            ++bhv->Mode;
        case 1:
            if (bhv->Timer-- <= 0) {
                mot_num = AL_GetMotionNum(tp);
                switch (mot_num) {
                    case ALM_CYMBAL_WAIT:
                        AL_SetMotionLinkStep(tp, ALM_CYMBAL_PAN, 10);

                        bhv->Timer = RAND_RANGE(60, 240);
                        bhv->SubTimer = 0;
                        break;
                    default:
                        AL_SetMotionLinkStep(tp, ALM_CYMBAL_WAIT, 10);

                        bhv->Timer = RAND_RANGE(60, 240);
                        break;
                }
            }

            mot_num = AL_GetMotionNum(tp);
            bhv->SubTimer++;
            switch (mot_num) {
                case ALM_CYMBAL_WAIT:
                    break;
                default:
                    if (bhv->SubTimer == 30) {
                        sub_8C05B020(0x4008, 0, 0, 100, &GET_CHAOWK(tp)->pos);
                    }

                    break;
            }
            break;
    }

    return AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_RANDOM_MUSIC) == NULL;
}

int ALBHV_Tambourine(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    int mot_num;

    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_TAMBOURINE_SHAKA);
            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);

            AL_SetItem(tp, AL_PART_HAND_L, object_alo_tambarin_pos_tambarin, &AL_TOY_TEXLIST);
            bhv->Timer = RAND_RANGE(60, 120);
            ++bhv->Mode;
        case 1:
            if (bhv->Timer-- <= 0) {
                mot_num = AL_GetMotionNum(tp);
                switch (mot_num) {
                    case ALM_TAMBOURINE_SHAKA:
                        AL_SetMotionLinkStep(tp, ALM_TAMBOURINE_PAN, 10);

                        bhv->Timer = RAND_RANGE(60, 120);
                        bhv->SubTimer = 0;
                        break;
                    default:
                        AL_SetMotionLinkStep(tp, ALM_TAMBOURINE_SHAKA, 10);

                        bhv->Timer = RAND_RANGE(60, 120);
                        bhv->SubTimer = 0;
                        break;
                }
            }

            mot_num = AL_GetMotionNum(tp);
            bhv->SubTimer++;
            switch (mot_num) {
                case ALM_TAMBOURINE_SHAKA:
                    break;
                default:
                    if (bhv->SubTimer == 30) {
                        sub_8C05B020(0x400D, 0, 0, 120, &GET_CHAOWK(tp)->pos);
                    }
                    break;
            }
            break;
    }

    return AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_RANDOM_MUSIC) == NULL;
}

int ALBHV_ListenMusic(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;
    int mot_num;

    switch (bhv->Mode) {
        case 0:
            if (njRandom() < 0.5) {
                AL_SetMotionLink(tp, ALM_MUSIC_SIT);
            } else {
                AL_SetMotionLink(tp, ALM_HIMA_SIT_GUDE);
            }

            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);

            bhv->Timer = RAND_RANGE(300, 600);
            ++bhv->Mode;
        case 1:
            if (bhv->Timer-- <= 0) {
                bhv->Mode = 0;
            }

            if (AL_IsHitKindWithNum(tp, AL_COLLI_BODY, CI_KIND_AL_RANDOM_MUSIC))
                break;

            AL_SetMotionLink(tp, ALM_HAKUSYU_SIT);
            bhv->Timer = RAND_RANGE(180, 300);
            bhv->Mode = 2;
            break;
        case 2:
            if (bhv->Timer-- <= 0) {
                return BHV_RET_FINISH;
            }
            break;
    }

    return BHV_RET_CONTINUE;
}