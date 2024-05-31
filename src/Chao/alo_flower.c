#include <Chao/Chao.h>

void ALO_NestFlowerExecutor(task* tp) {
    task* pNestTask = tp->ptp;
    taskwk* work = tp->twp;

    if (pNestTask->twp->ang.x) {
        work->scl.x += 0.1f;
        if (work->scl.x > 1) {
            work->scl.x = 1;
        }
    } else {
        work->scl.x -= 0.03f;
        if (work->scl.x < 0) {
            work->scl.x = 0;
            FreeTask(tp);
        }
    }
}

extern NJS_CNK_OBJECT object_nest_flower_C602200[];
extern NJS_CNK_OBJECT object_nest_flower_C6024F4[];
void ALO_NestFlowerDisplayer(task* tp) {
    taskwk* work = tp->twp;
    float scl = work->scl.x * 0.5f;

    njSetTexture(&AL_OBJECT_TEXLIST);

    njPushMatrixEx();
    njTranslateEx(&work->pos);
    njRotateY(NULL, work->ang.y);
    njScale(NULL, scl, scl, scl);
    switch (work->smode) {
        case 0:
            njCnkEasyDrawObject(object_nest_flower_C602200);
            break;
        default:
            njCnkEasyDrawObject(object_nest_flower_C6024F4);
            break;
    }

    njPopMatrixEx();
}

void ALO_NestFlowerDestructor(task* tp) {
}

task* ALO_CreateNestFlower(task* pNestTask, NJS_POINT3* pPos) {
    task* tp = CreateChildTask(2, ALO_NestFlowerExecutor, pNestTask);
    taskwk* work;

    tp->disp = ALO_NestFlowerDisplayer;
    tp->dest = ALO_NestFlowerDestructor;

    work = tp->twp;
    work->pos = *pPos;
    work->ang.y = NJM_DEG_ANG(njRandom() * 360.f);

    if (njRandom() < 0.5)
        work->smode = 0;
    else
        work->smode = 1;

    return tp;
}

float GetShadowPos(float x, float y, float z, NJS_VECTOR* pPos);

void ALO_NestExecutor(task* tp) {
    taskwk* work = tp->twp;
    work->ang.x = 0;
    switch (work->mode) {
        case 0:
            if (work->wtimer++ > GET_GLOBAL()->NestFlowerInterval) {
                NJS_POINT3* pos = &work->pos;
                NJS_POINT3 flower_pos, out_vec;

                int ang = work->ang.y;

                float range = GET_GLOBAL()->NestRange * (0.9f + njRandom() * 0.1f);
                work->ang.y -= NJM_DEG_ANG(360.0f / GET_GLOBAL()->nbNestFlower);

                flower_pos.x = pos->x + njSin(ang) * range;
                flower_pos.y = pos->y + 2.0f;
                flower_pos.z = pos->z + njCos(ang) * range;

                flower_pos.y = GetShadowPos(flower_pos.x, flower_pos.y, flower_pos.z, &out_vec);

                ALO_CreateNestFlower(tp, &flower_pos);

                work->id++;
                if (work->id >= GET_GLOBAL()->nbNestFlower) {
                    work->mode = 1;
                    work->smode = 0;
                }
                work->wtimer = 0;
            }
            break;
        case 1:
        default:
            break;
    }

    if (GET_CHAOWK(tp->ptp)->NestFlag) {
        GET_CHAOWK(tp->ptp)->NestFlag = FALSE;
        work->ang.x = 1;
    } else {
        tp->exec = DeleteTaskSub;
    }
}

void ALO_NestDestructor(task* tp) {
}

task* ALO_CreateNest(task* pChaoTask) {
    task* tp = CreateChildTask(2, ALO_NestExecutor, pChaoTask);
    if (tp) {
        tp->dest = ALO_NestDestructor;
        tp->twp->pos = GET_CHAOWK(pChaoTask)->pos;
        GET_CHAOWK(pChaoTask)->NestFlag = TRUE;
    }
    return tp;
}