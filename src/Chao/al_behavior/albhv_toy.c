#include <Chao/Chao.h>

#include <effect2.h>

extern NJS_TEXANIM anim[];

extern NJS_CNK_OBJECT object_alo_syabon_bottle_bottle[];
extern NJS_CNK_OBJECT object_alo_syabon_stllow_stllow[];
extern NJS_CNK_OBJECT object_alo_houki_pos_pos[];
extern NJS_CNK_OBJECT object_alo_gara_n_gara_n[];
extern NJS_CNK_OBJECT object_alo_hanabi_h_hanabi_h[];
extern NJS_CNK_OBJECT object_alo_book_pos_pos[];
extern NJS_MOTION motion_alo_book_pos[];
extern NJS_CNK_OBJECT object_alo_minicar_minicar[];
extern NJS_CNK_OBJECT object_alo_doll_doll[];
extern NJS_CNK_OBJECT object_alo_hopping_hopping[];

extern BHV_FUNC ALBHV_SToyFunc[];

void ALO_ShabonDisplayer(task* tp) {
    taskwk* twk = tp->twp;
    float scl = (0.2f * twk->scl.x) * twk->scl.y;
    NJS_SPRITE sprite;
    
    njColorBlendingMode(0, 8);
    njColorBlendingMode(1, 10);

    njSetTexture(&AL_TOY_TEXLIST);
    SetMaterial(1, 1, 1, 1);
    
    sprite.tlist = &AL_TOY_TEXLIST;
    sprite.tanim = anim;

    sprite.ang = 0;

    sprite.sx = 0.2f;
    sprite.sy = 0.2f;

    sprite.p.x = 0;
    sprite.p.y = 0;
    sprite.p.z = 0;

    njPushMatrixEx();
    njTranslateEx(&twk->pos);
    njDrawSprite3D(&sprite, 0, NJD_SPRITE_SCALE | NJD_SPRITE_ALPHA);
    njPopMatrixEx();

    SetMaterial(0, 0, 0, 0);

    njColorBlendingMode(0, 8);
    njColorBlendingMode(1, 6);
}

void ALO_ShabonExecutor(task* tp) {
    taskwk* work = tp->twp;
    MOVE_WORK* move = tp->Data2;

    if(njScalor(&move->Velo) > 0.05f) {
        move->Velo.x *= 0.9f;
        move->Velo.y *= 0.9f;
        move->Velo.z *= 0.9f;
    }

    work->scl.y += 0.15f;
    if(work->scl.y > 1) {
        work->scl.y = 0;
    }

    if(work->wtimer++ > 1800) {
        FreeTask(tp);
    }

    work->pos.y += njSin(work->ang.y) * 0.02f + 0.01f;
    work->ang.y += 256;

    MOV_Control(tp);
}

task* ALO_CreateShabon(NJS_POINT3 *pPos, NJS_VECTOR *pVelo) {
    task* tp = CreateElementalTask(2, 4, ALO_ShabonExecutor, "ALO_ShabonExecutor");
    taskwk* twk = tp->twp;
    MOVE_WORK* move;

    tp->disp = ALO_ShabonDisplayer;

    move = MOV_Init(tp);

    twk->pos = *pPos;

    if(pVelo) {
        move->Velo = *pVelo;
    }

    MOV_SetGravity(tp, 0.f);

    move->Offset.y = 3;
    move->BoundFloor = 0.8f;
    move->BoundFriction = 0.7f;

    twk->ang.y = NJM_DEG_ANG(njRandom() * 360.f);

    twk->scl.x = 0.7f + 0.6f * njRandom();
    twk->scl.y = 0;

    // they forgot to return
}

int ALBHV_Syabon(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_SYABON_JYABU);
            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);

            AL_SetItem(tp, AL_PART_HAND_L, object_alo_syabon_bottle_bottle, &AL_TOY_TEXLIST);
            AL_SetItem(tp, AL_PART_HAND_R, object_alo_syabon_stllow_stllow, &AL_TOY_TEXLIST);

            bhv->Mode++;
        case 1:
            if(bhv->Timer++ > 60) {
                if(!(bhv->Timer % 10) && njRandom() < 0.4f) {
                    AL_SHAPE* pShape = &GET_CHAOWK(tp)->Shape;

                    NJS_POINT3 pos;
                    NJS_VECTOR velo;

                    float f = njRandom() * 0.1f + 0.2f;

                    velo.x = pShape->MouthVec.x * f + njRandom() * 0.1f;
                    velo.y = pShape->MouthVec.y * f + 0.1f;
                    velo.z = pShape->MouthVec.z * f + njRandom() * 0.1f;

                    pos.x = pShape->RightHandPos.x + pShape->MouthVec.x * 0.5f;
                    pos.y = pShape->RightHandPos.y + pShape->MouthVec.y * 0.5f + 0.05f;
                    pos.z = pShape->RightHandPos.z + pShape->MouthVec.z * 0.5f;

                    ALO_CreateShabon(&pos, &velo);
                }
            }

            if(AL_IsMotionStop(tp)) {
                // sadx seems to have changed this to guarantee
                // atleast 4 runs (counting them here using SubTimer)
                if(njRandom() < 0.25f) {
                    return BHV_RET_FINISH;
                }
                else {
                    AL_SetMotionLink(tp, ALM_SYABON_JYABU);
                    bhv->Timer = 0;
                }
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Houki(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_HOUKI);
            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);

            AL_SetItem(tp, AL_PART_HAND_R, object_alo_houki_pos_pos, &AL_TOY_TEXLIST);

            bhv->Timer = RAND_RANGE(600, 800);
            bhv->Mode++;
        case 1:
            if(bhv->Timer-- <= 0) {
                return BHV_RET_FINISH;
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Garagara(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_GARAGARA);
            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);

            AL_SetItem(tp, AL_PART_HAND_R, object_alo_gara_n_gara_n, &AL_TOY_TEXLIST);

            bhv->Timer = RAND_RANGE(600, 800);
            bhv->Mode++;
        case 1:
            if(bhv->Timer-- <= 0) {
                return BHV_RET_FINISH;
            }
            break;
    }

    return BHV_RET_CONTINUE; 
}

int ALBHV_Hanabi(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_HANABI);
            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);

            AL_SetItem(tp, AL_PART_HAND_R, object_alo_hanabi_h_hanabi_h, &AL_TOY_TEXLIST);

            bhv->Timer = RAND_RANGE(600, 800);
            bhv->Mode++;
        case 1:
            if(bhv->Timer-- <= 0) {
                return BHV_RET_FINISH;
            }
            break;
    }

    return BHV_RET_CONTINUE; 
}

enum {
    MD_BOOK_EXPAND = 0,
    MD_BOOK_OPEN = 1,
    MD_BOOK_MEKURI = 2,
    MD_BOOK_MEKURI2 = 3,
    MD_BOOK_SHRINK = 4,
};

void BookExecutor(task* tp) {
    taskwk* twk = tp->twp;

    switch(twk->mode) {
        case MD_BOOK_EXPAND:
            twk->scl.y += 0.1f;
            if(twk->scl.y > 1) {
                twk->scl.y = 1;
                twk->mode = MD_BOOK_OPEN;
            }
            break;
        
        case MD_BOOK_OPEN:
            twk->scl.x = 15;
            break;

        case MD_BOOK_MEKURI:
            twk->scl.x += 0.15f;
            if(twk->scl.x > 23) {
                twk->scl.x = 23;
                twk->mode = MD_BOOK_OPEN;
            }
            break;

        case MD_BOOK_SHRINK:
            twk->scl.y -= 0.1f;
            if(twk->scl.y < 0) {
                twk->scl.y = 0;
                FreeTask(tp);
            }
            break;
    }
}

void BookDisplayer(task* tp) {
    taskwk* twk = tp->twp;
    float scl = twk->scl.y;

    if(scl >= 0.01f) {
        njSetTexture(&AL_TOY_TEXLIST);

        njPushMatrixEx();

        njTranslateV(NULL, &twk->pos);
        njRotateY(NULL, twk->ang.y);
        njScale(NULL, scl, scl, scl);

        njCnkEasyDrawMotion(object_alo_book_pos_pos, motion_alo_book_pos, twk->scl.x);

        njPopMatrixEx();
    }
}

void BookDestructor(task* tp) {

}

void BookCreate(task* pChaoTask) {
    task* tp;

    if(GET_CHAOWK(pChaoTask)->pBookTask) {
        FreeTask(GET_CHAOWK(pChaoTask)->pBookTask);
    }

    tp = CreateElementalTask(2, 2, BookExecutor, "BookExecutor");
    GET_CHAOWK(pChaoTask)->pBookTask = tp;

    tp->twp->pos = pChaoTask->twp->pos;
    tp->twp->ang.y = pChaoTask->twp->ang.y;

    tp->disp = BookDisplayer;
    tp->dest = BookDestructor;
}

int ALBHV_Book(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch (bhv->Mode) {
        case 0:
            AL_FixPosition(tp);
            AL_SetMotionLink(tp, ALM_BOOK_START);

            if(njRandom() < 0.5f) {
                AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            }

            bhv->Timer = RAND_RANGE(300, 400);
            BookCreate(tp);
            bhv->Mode++;
        case 1:
            if(bhv->Timer-- <= 0) {
                if(njRandom() < 0.3f) {
                    GET_CHAOWK(tp)->pBookTask->twp->mode = MD_BOOK_SHRINK;

                    return BHV_RET_FINISH;
                }
                else {
                    GET_CHAOWK(tp)->pBookTask->twp->mode = MD_BOOK_MEKURI;

                    AL_SetMotionLink(tp, ALM_BOOK_MEKURI);
                    bhv->Mode++;
                }
            }
            break;

        case 2:
            if(AL_IsMotionStop(tp)) {
                AL_SetMotionLink(tp, ALM_BOOK_YOMU);

                bhv->Timer = RAND_RANGE(120, 300);
                bhv->Mode--;
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Minicar(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch (bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_MINICAR);

            if(njRandom() < 0.5f) {
                AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
            }

            AL_SetItemParallelRightHand(tp, object_alo_minicar_minicar, &AL_TOY_TEXLIST);

            bhv->Timer = RAND_RANGE(300, 400);
            bhv->Mode++;
        case 1:
            if(bhv->Timer-- <= 0) {
                if(njRandom() < 0.2f) {
                    return BHV_RET_FINISH;
                }
                else {
                    if(njRandom() < 0.5f) {
                        AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
                    }

                    AL_SetMotionLink(tp, ALM_MINICAR_MIAGE);
                    bhv->Mode++;
                }
            }
            break;

        case 2:
            if(AL_IsMotionStop(tp)) {
                AL_SetMotionLink(tp, ALM_MINICAR);

                if(njRandom() < 0.5f) {
                    AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);
                }

                bhv->Timer = RAND_RANGE(380, 560);
                bhv->Mode--;
            }
            break;
    }

    return BHV_RET_CONTINUE;
}

int ALBHV_Doll(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_DOLL_YOSHIYOSHI);
            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);

            AL_SetItem(tp, AL_PART_HAND_L, object_alo_doll_doll, &AL_TOY_TEXLIST);

            bhv->Mode++;
        case 1:
            if(AL_IsMotionStop(tp)) {
                if(njRandom() < 0.2f) {
                    return BHV_RET_FINISH;
                }
                else {
                    if(njRandom() < 0.6f) {
                        AL_SetMotionLink(tp, ALM_DOLL_YOSHIYOSHI);
                    }
                    else {
                        AL_SetMotionLink(tp, ALM_DOLL_TAKAITAKAI);
                    }
                }
            }
            break;
    }

    return BHV_RET_CONTINUE; 
}

int ALBHV_Hopping(task* tp) {
    AL_BEHAVIOR* bhv = &GET_CHAOWK(tp)->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, ALM_HOPPING);
            AL_FaceSetEye(tp, AL_EYE_NUM_NIKO, -1);

            AL_SetItem(tp, AL_PART_HAND_R, object_alo_hopping_hopping, &AL_TOY_TEXLIST);

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

Bool AL_DecideBehaviorSToy(task* tp) {
    float score = 0;
    taskwk* work = tp->twp;

    int BhvList[32];
    int count = 0;

    int i;

    for(i = 0; i < NB_AL_STOY; i++) {
        if(AL_KW_IsSToyFlagOn(tp, i)) {
            if(ALBHV_SToyFunc[i]) {
                BhvList[count] = i;
                count++;
            }
        }
    }

    if(count > 0) {
        int num = BhvList[(Uint32)((count - 0.0001f) * njRandom())];
        
        AL_SetBehavior(tp, ALBHV_SToyFunc[num]);

        return TRUE;
    }

    return FALSE;
}