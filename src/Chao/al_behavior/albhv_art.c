#include <Chao/Chao.h>

#include <Chao/al_misc.h>
#include <Chao/al_field.h>
#include <playsound.h>

enum {
    RAKUGAKI_SONIC,
    RAKUGAKI_SHADOW,
    RAKUGAKI_TAILS,
    RAKUGAKI_EGGMAN,
    RAKUGAKI_KNUCKLES,
    RAKUGAKI_ROUGE,
    RAKUGAKI_N_CHAO,
    RAKUGAKI_H_CHAO,
    RAKUGAKI_D_CHAO,
    RAKUGAKI_CAKE,
    RAKUGAKI_CAR,
    RAKUGAKI_FLOWER,
    RAKUGAKI_BONE,
    RAKUGAKI_AMY,
    RAKUGAKI_TIKAL,
    RAKUGAKI_BIG,
};

float GetShadowPos(float x, float y, float z, Angle3 *ang);

extern AL_ITEM_INFO CrayonBoxInfo;
extern AL_ITEM_INFO CrayonInfo;
extern int nbRakugaki;

static const CCL_INFO colli_info[] = {
    CCL_SPHERE(0, 0x70, 0xC, 0x648000, 0, 1, 0, 2, 0, 0)
};

void ALO_RakugakiExecutor(task* tp) {
    taskwk* twk = tp->twp;

    if(!(twk->wtimer++ % 580) && twk->id < 3 && AL_IsHitKindWithNum(tp, 0, CI_KIND_AL_RAKUGAKI)) {
        twk->id++;
    }

    if(twk->id < 3 && twk->wtimer < 1800) {
        CCL_Entry(tp);
    }
    else {
        CCL_ClearEntry(tp);
    }
}

void ALO_RakugakiDisplayer(task* tp) {
    taskwk* twk = tp->twp;

    // varying transparency for art "fading in" to completion
    const Uint32 col_table[] = {
        0x00FFFFFF,
        0x3FFFFFFF,
        0xAFFFFFFF,
        0xFFFFFFFF
    };

    Uint32 col = col_table[twk->id];

    NJS_TEXTURE_VTX poly[4];

    poly[0].x = -2;
    poly[0].y = 0.1f;
    poly[0].z = 2;
    poly[0].u = 0;
    poly[0].v = 0;
    poly[0].col = col;

    poly[1].x = -2;
    poly[1].y = 0.1f;
    poly[1].z = -2;
    poly[1].u = 0;
    poly[1].v = 1;
    poly[1].col = col;

    poly[2].x = 2;
    poly[2].y = 0.1f;
    poly[2].z = 2;
    poly[2].u = 1;
    poly[2].v = 0;
    poly[2].col = col;

    poly[3].x = 2;
    poly[3].y = 0.1f;
    poly[3].z = -2;
    poly[3].u = 1;
    poly[3].v = 1;
    poly[3].col = col;

    njSetTexture(&AL_OBJECT_TEXLIST);
    njSetTextureNum(twk->smode + 0x24);

    njPushMatrixEx();
    njTranslateEx(&twk->pos);
    njRotateZ(NULL, twk->ang.z);
    njRotateX(NULL, twk->ang.x);
    njRotateY(NULL, twk->ang.y);

    njDrawTexture3DEx(poly, 4, 1);

    njPopMatrixEx();
}

void ALO_RakugakiDestructor(task* tp) {
    nbRakugaki--;
}

task* ALO_CreateRakugaki(int num, NJS_POINT3 *pPos, int AngY) {
    task* tp = CreateElementalTask(2, 2, ALO_RakugakiExecutor, "ALO_RakugakiExecutor");
    taskwk* twk;

    nbRakugaki++;

    twk = tp->twp;
    CCL_Init(tp, colli_info, 1, 5);

    twk->pos = *pPos;
    twk->pos.y = GetShadowPos(pPos->x, pPos->y + 10, pPos->z, &twk->ang);

    twk->ang.y = AngY;

    twk->smode = num;

    tp->disp = ALO_RakugakiDisplayer;
    tp->dest = ALO_RakugakiDestructor;
}

int ALBHV_Art(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_FixPosition(tp);

            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            AL_SetMotionLinkStep(tp, ALM_RAKUGAKI_JYUNBI, 20);

            AL_SetItemOffset(tp, AL_PART_HAND_L, &CrayonBoxInfo);

            bhv->Mode++;
        case 1:
            if(AL_IsMotionEnd(tp)) {
                AL_SetItemOffset(tp, AL_PART_HAND_R, &CrayonInfo);

                bhv->Mode++;
            }
            break;

        case 2:
            if(AL_IsMotionStop(tp)) {
                NJS_POINT3* pPos = &tp->twp->pos;
                int ang = tp->twp->ang.y;
                NJS_POINT3 pos;

                pos.x = pPos->x + 3 * njSin(ang);
                pos.y = pPos->y;
                pos.z = pPos->z + 3 * njCos(ang);

                AL_ChildFieldCreateT(tp, CI_KIND_AL_RAKUGAKI, &pos, 2, 1800);
                ALO_CreateRakugaki(AL_BehaviorGetFreeWork(tp), &pos, ang);

                AL_SetMotionLinkStep(tp, ALM_RAKUGAKI, 20);

                bhv->Timer = 1800;
                bhv->Mode++;
            }
            break;
        
        case 3:
            if(bhv->Timer-- <= 0) {
                return BHV_RET_FINISH;
            }

            if(!(bhv->SubTimer++ % 120) && njRandom() < 0.3f) {
                sub_8C05B020(4134, 0, 0, 120, &tp->twp->pos);
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

int AL_DecideDrawPlayer(task* tp) {
    AL_KNOWLEDGE* kwlg = &GET_CHAOPARAM(tp)->knowledge;

    int i;

    int count = 0;
    int PlayerEntry[NB_BTL_AL_PLAYER];

    int threshold = 50;
    
    for(i = 0; i < NB_BTL_AL_PLAYER; ++i) {
        if(kwlg->player[i].like >= threshold) {
            // shad's theory: they wanted == here
            // to find the maximum likeness player
            // and to support multiple ones if they're all equally maximum
            if(kwlg->player[i].like = threshold) {
                PlayerEntry[count] = i;
                count++;
            }
            else {
                // ???
                PlayerEntry[0] = i;
                count = 1;
                threshold = kwlg->player[i].like;
            }
        }
    }

    if(count > 0 && njRandom() < 0.8f) {
        return PlayerEntry[(Uint32)((count - 0.0001f) * njRandom())];
    }
    else {
        CHAO_PARAM* pParam = GET_CHAOPARAM(tp);

        switch(pParam->type) {
            case TYPE_H_NORMAL:
            case TYPE_H_SWIM:
            case TYPE_H_FLY:
            case TYPE_H_RUN:
            case TYPE_H_POWER:
            case TYPE_H_CHAOS:
                return RAKUGAKI_H_CHAO;
            case TYPE_D_NORMAL:
            case TYPE_D_SWIM:
            case TYPE_D_FLY:
            case TYPE_D_RUN:
            case TYPE_D_POWER:
            case TYPE_D_CHAOS:
                return RAKUGAKI_D_CHAO;
            default:
                return RAKUGAKI_N_CHAO;
        }
    }
}

Bool AL_DecideBehaviorArt(task* tp) {
    float score = 0;
    taskwk* twk = tp->twp;

    int BhvList[8];
    int art_num = 0;

    if (nbRakugaki < 100) {
        int i;

        for(i = 0; i < NB_AL_ART; ++i) {
            if (AL_KW_IsArtFlagOn(tp, i)) {
                BhvList[art_num] = i;
                art_num++;
            }
        }

        if (art_num > 0 && njRandom() < 0.5f) {
            int kind = BhvList[(int)(njRandom() * (art_num - 0.00001f))];
            NJS_POINT3 center;
            int ang = twk->ang.y;
            int info;

            center.x = twk->pos.x + 8 * njSin(ang);
            center.y = twk->pos.y;
            center.z = twk->pos.z + 8 * njCos(ang);

            AL_SetBehavior(tp, ALBHV_Art);

            switch(kind) {
                case AL_ART_1:
                    info = RAKUGAKI_CAKE;
                    break;
                case AL_ART_2: {
                    CHAO_PARAM* pParam = GET_CHAOPARAM(tp);

                    switch(pParam->type) {
                        case TYPE_H_NORMAL:
                        case TYPE_H_SWIM:
                        case TYPE_H_FLY:
                        case TYPE_H_RUN:
                        case TYPE_H_POWER:
                        case TYPE_H_CHAOS:
                            info = RAKUGAKI_FLOWER;
                            break;
                        case TYPE_D_NORMAL:
                        case TYPE_D_SWIM:
                        case TYPE_D_FLY:
                        case TYPE_D_RUN:
                        case TYPE_D_POWER:
                        case TYPE_D_CHAOS:
                            info = RAKUGAKI_BONE;
                            break;
                        default:
                            info = RAKUGAKI_CAR;
                            break;
                    }
                } break;
                case AL_ART_3:
                    info = RAKUGAKI_AMY;
                    break;
                case AL_ART_4:
                    info = RAKUGAKI_BIG;
                    break;
                case AL_ART_5:
                    info = RAKUGAKI_TIKAL;
                    break;
            }

            AL_BehaviorSetFreeWork(tp, info);

            return TRUE;
        }
        else {
            if(AL_KW_IsSToyFlagOn(tp, AL_STOY_CRAYON)) {
                // sa2b fetches taskwk again here for some reason
                NJS_POINT3 center;
                int ang = twk->ang.y;
                int info;

                center.x = twk->pos.x + 8 * njSin(ang);
                center.y = twk->pos.y;
                center.z = twk->pos.z + 8 * njCos(ang);

                AL_SetBehavior(tp, ALBHV_Art);
                AL_BehaviorSetFreeWork(tp, AL_DecideDrawPlayer(tp));

                return TRUE;
            }
        }
        
    }
    
    return FALSE;
}