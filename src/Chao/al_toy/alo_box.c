#include <Chao/Chao.h>
#include <c_colli.h>
#include "playsound.h"

extern task* pBoxTask;

extern float float_C6352BC;
extern float float_C6352C0;
extern float float_C6352C4;
extern float float_C6352C8;
extern NJS_MOTION motion_alo_box[];

// only BoxExecutor is unmatching but since it fucks up the alignment in all kinds of ways
// i have to include basically everything around it too
#ifdef NON_MATCHING

Sint8 ALO_GetBoxMode(void) {
    if (!pBoxTask)
        return -1;
    else
        return pBoxTask->twp->mode;
}

void ALO_BoxExecutor(task* tp) {
    taskwk* work = tp->twp;

    switch (work->mode) {
        case 0: {
            task* touch = CCL_IsHitKind2(tp, CI_KIND_AL_TOUCH);
            if (touch) {
                if (touch->ptp) {
                    task* parent = touch->ptp;
                    Uint16 skill = AL_ParameterGetSkill(parent, 4);
                    if (50000 - (skill * 4) > njRandom() * 80000) {
                        work->scl.y = 0.01f;
                        work->scl.z = 0;
                        work->scl.x = 0;
                    };

                    CCL_Disable(tp, 1);
                    work->mode = 1;
                    work->smode = 0;
                    work->wtimer = 0;
                    work->id = 0;
                    work->wtimer = RAND_RANGE(30, 150);
                }
            }
        } break;
        case 1:
            if (--work->wtimer <= 0) {
                sub_8C05B020(0x102A, 0, 0, 0, &work->pos);

                work->mode = 2;
                work->smode = 0;
                work->wtimer = 0;
                work->id = 0;
            }
            break;
        case 2:
            work->scl.x += 0.3f;
            if (work->smode == 0) {
                if (work->scl.x > 1) {
                    if (work->scl.y > 0) {
                        AL_AllFieldCreateT(162, &work->pos, 5.0, 2);
                    }
                    work->smode = 1;
                }
            }
            if (work->scl.x > motion_alo_box[0].nbFrame - 1.0f) {
                work->scl.x = motion_alo_box[0].nbFrame - 1.0f;
                work->mode = 3;
                work->smode = 0;
                work->wtimer = 0;
                work->id = 0;
            }
            break;
        case 3:
            if (work->wtimer++ > 60) {
                work->mode = 4;
                work->smode = 0;
                work->wtimer = 0;
                work->id = 0;
            }
            break;
        case 4:
            work->scl.x -= 0.1f;
            if (work->scl.x < 0) {
                work->scl.x = 0;
                work->scl.y = 0;
                work->scl.z = 0;
                work->ang.x = 0;
                work->ang.z = 0;
                work->mode = 0;
                work->smode = 0;
                work->wtimer = 0;
                work->id = 0;
            }
            break;
    }
    switch (work->mode) {
        case 2:
        case 3:
            if (work->scl.y > 0) {
                // these 3 lines are the only unmatching part in the whole thing
                // i tried everything at this point, no clue

                //(5 - work->scl.y) * 0.15f
                // const float b = float_C6352C0;
                // const float a = float_C6352BC;

                work->scl.z += float_C6352C0 * -(-float_C6352BC + work->scl.y);

                work->scl.z *= float_C6352C4;
                work->scl.y += work->scl.z;

                work->ang.x += 4096;
                work->ang.z += 4864;
            }

            break;
        case 4:
            if (work->scl.y > 0 && work->scl.x < 10) {
                work->scl.z = 0;
                work->scl.y *= float_C6352C8;
            }

            work->ang.x += 4096, work->ang.z += 4864;
            break;
    }
    CCL_Entry(tp);
}
extern CCL_INFO ccl_C5674F0[];
extern NJS_CNK_OBJECT object_alo_box[];

extern NJS_CNK_MODEL model_C634768[];
extern NJS_CNK_MODEL model_kage_marukage_marukage[];
int AL_IsOnScreen2(task* tp, float x, float y);
void ALO_BoxDisplayer(task* tp) {
    taskwk* work = tp->twp;
    MOTION_CTRL* motion = (MOTION_CTRL*)tp->UnknownA_ptr;

    if (AL_IsOnScreen2(tp, 2.5f, 1)) {
        // wtf lol
        njSetTexture(&AL_TOY_TEXLIST);
        njSetTexture(&AL_TOY_TEXLIST);

        njPushMatrixEx();
        njTranslateEx(&work->pos);
        njRotateY(0, work->ang.y);

        njPushMatrixEx();
        njTranslate(0, 0, 0, 3);
        njRotateY(0, 0x8000);
        if (work->scl.x < 0.01f) {
            njCnkEasyDrawObject(object_alo_box);
        } else {
            njCnkEasyMotion(object_alo_box, motion_alo_box, work->scl.x);
        }
        njPopMatrixEx();

        if (work->scl.y > 0.01f) {
            int angX = NJM_DEG_ANG(njSin(work->ang.x) * 30);
            int angZ = NJM_DEG_ANG(njSin(work->ang.z) * 30);
            float scl;
            if (work->scl.y < 2.0) {
                scl = work->scl.y * 0.5 + 0.5;
            } else {
                scl = 1.5;
            }
            njPushMatrixEx();
            njTranslate(0, 0, work->scl.y, 0);
            njRotateX(0, angX);
            njRotateZ(0, angZ);
            njRotateY(0, 0x8000);
            njScale(0, scl, scl, scl);
            njCnkSimpleDrawModel(model_C634768);
            njPopMatrixEx();
        }

        njTranslate(0, 0, 0.5f, 0);
        njScale(0, 1.5f, 1, 1.5f);
        njCnkModDrawModel(model_kage_marukage_marukage);
        njPopMatrixEx();
    }
}
void ALO_BoxDestroyer(task* tp) {
    ALW_CancelEntry(tp);
    pBoxTask = 0;
}

void CreateALO_Box(NJS_VECTOR* pos, int ang) {
    task* box = CreateElementalTask(2, 4, ALO_BoxExecutor, (const char*)0x0C5674E0);
    taskwk* work;
    pBoxTask = box;
    work = box->twp;
    CCL_Init(box, (CCL_INFO*)0xC5674B0, 1, 4);

    ALW_Entry(6, box, 8);
    work->pos = *pos;
    work->ang.y = ang;

    work->scl.y = 0;
    work->scl.z = 0;
    work->ang.x = 0;
    work->ang.z = 0;

    box->disp = ALO_BoxDisplayer;
    box->dest = ALO_BoxDestroyer;
}
#else
INLINE_ASM(_func_0C53DC20, "asm/nonmatching/Chao/al_toy/alo_box/_func_0C53DC20.src");

INLINE_ASM(_ALO_BoxExecutor, "asm/nonmatching/Chao/al_toy/alo_box/_ALO_BoxExecutor.src");

INLINE_ASM(_func_0C53DF40, "asm/nonmatching/Chao/al_toy/alo_box/_func_0C53DF40.src");

INLINE_ASM(_func_0C53E090, "asm/nonmatching/Chao/al_toy/alo_box/_func_0C53E090.src");

INLINE_ASM(_func_0C53E0A2, "asm/nonmatching/Chao/al_toy/alo_box/_func_0C53E0A2.src");

INLINE_ASM(_func_0C53E1B4, "asm/nonmatching/Chao/al_toy/alo_box/_func_0C53E1B4.src");

#endif
