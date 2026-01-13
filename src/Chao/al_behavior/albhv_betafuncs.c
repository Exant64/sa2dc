#include <Chao/Chao.h>

/*
    really weird file
    first off, did they use extern like this too to access albhv_music's models?
    the data isnt decomped yet but these functions have a table with an accompanying string table
    none of these functions ever finish
    and the instrument bhvs are slightly older versions of the real ones or something

    beta tambourine (last func) is on decomp.me, it somehow includes unreachable code
    which shc never does so i have no idea how to match it
*/

extern NJS_CNK_OBJECT object_alo_maracas_pos_l_maracas1[];
extern NJS_CNK_OBJECT object_alo_maracas_pos_r_maracas[];
extern NJS_CNK_OBJECT object_alo_dram_pos_l_batil[];
extern NJS_CNK_OBJECT object_alo_dram_pos_r_batir[];
extern NJS_CNK_OBJECT object_alo_dram_pos_c_dram[];
extern NJS_CNK_OBJECT object_alo_sinbal_pos_l_sinbal2[];
extern NJS_CNK_OBJECT object_alo_sinbal_pos_r_sinbal1[];
extern NJS_CNK_OBJECT object_alo_tambarin_pos_tambarin[];

int ALBHV_BetaOjigi(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            if(njRandom() < 0.4f) {
                AL_SetMotionLink(tp, ALM_OJIGI);
            }

            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);

            bhv->Timer = RAND_RANGE(120, 240);
            bhv->Mode++;
            break;
        case 1:
            if(bhv->Timer-- <= 0) {
                bhv->Mode--;
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_BetaLaugh(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            if(njRandom() < 0.4f) {
                AL_SetMotionLink(tp, ALM_LAUGH_SIT);
            }
            else {
                AL_SetMotionLink(tp, ALM_HIMA_SIT);
            }

            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);

            bhv->Timer = RAND_RANGE(60, 120);
            bhv->Mode++;
            break;
        case 1:
            if(bhv->Timer-- <= 0) {
                bhv->Mode--;
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_BetaSing(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_UTAU_LEFT);

            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);

            if(njRandom() < 0.5f) {
                AL_FaceSetMouth(tp, AL_MOUTH_NUM_NIKO, -1);
            }
            else {
                AL_FaceSetMouth(tp, AL_MOUTH_NUM_NONE, -1);
            }

            bhv->Mode++;
            break;
        case 1:
            if(AL_IsMotionStop(tp)) {
                bhv->Mode--;
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

// same thing as the original but with sounds
// (and without finishing)
int ALBHV_BetaMaracas(task* tp) {
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
            if (bhv->Timer-- <= 0 || AL_IsMotionStop(tp)) {
                if (njRandom() < 0.3f) {
                    AL_SetMotionLinkStep(tp, ALM_MARACAS_JUMP_B, 10);
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
            break;
        default:
            break;
    }

    return BHV_RET_CONTINUE;
}

// same thing as real one but without sound handler at bottom and with if-else hack for GetMotionNum regswap
// (and without finishing)
int ALBHV_BetaDrum(task* tp) {
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
            if (bhv->Timer-- > 0) {} else {
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
            break;
    }

    return BHV_RET_CONTINUE;
}

// no sounds but more anims! (+ if-else hack)
int ALBHV_BetaCymbal(task* tp) {
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
            if (bhv->Timer-- > 0) {} else {
                mot_num = AL_GetMotionNum(tp);
                switch (mot_num) {
                    case ALM_CYMBAL_WAIT:
                        if(njRandom() < 0.6f) {
                            if(njRandom() < 0.5f) {
                                AL_SetMotionLinkStep(tp, ALM_CYMBAL_PAN, 10);
                            }
                            else {
                                AL_SetMotionLinkStep(tp, ALM_CYMBAL_PANPAN, 10);
                            }
                        }
                        else {
                            AL_SetMotionLinkStep(tp, ALM_CYMBAL_PAPAPAN, 10);
                        }

                        bhv->Timer = RAND_RANGE(60, 240);
                        break;
                    default:
                        AL_SetMotionLinkStep(tp, ALM_CYMBAL_WAIT, 10);

                        bhv->Timer = RAND_RANGE(60, 240);
                        break;
                }
            }

            break;
    }

    return BHV_RET_CONTINUE;
}

// https://decomp.me/scratch/g4awI
INLINE_ASM(_func_0C53BB20, 0xe0, "asm/nonmatching/Chao/al_behavior/albhv_betafuncs/_func_0C53BB20.src");

INLINE_ASM(_func_0C53BC00, 0x28, "asm/nonmatching/Chao/al_behavior/albhv_betafuncs/_func_0C53BC00.src");

// MERGE_LIST([['_CameraMode', '_lbl_0C53BDC0']]);
INLINE_ASM(_rodata, "asm/nonmatching/Chao/al_behavior/albhv_betafuncs/rodata.src");

