#include <Chao/Chao.h>
#include <c_colli.h>
#include <Chao/al_field.h>
#include <effect2.h>

void DestroyTask(task* tp);
void AL_FieldExecutor(task* tp) {
    taskwk* work = tp->twp;

    if (work->smode) {
        if (work->wtimer == 0) {
            DestroyTask(tp);
            return;
        } else {
            work->wtimer--;
        }
    }

    CCL_Entry(tp);
}
extern NJS_CNK_OBJECT debug_cube_model[];
void AL_FieldDisplayer(task* tp) {
    taskwk* work = tp->twp;
    float scale = work->scl.x;
    if (GET_GLOBAL()->DispField) {
        SaveControl3D();
        OnControl3D(0x10);
        OffControl3D(0x200);

        SetMaterial(0.25f, 0, 0, 1);

        njPushMatrixEx();
        njTranslateEx(&work->pos);
        njScale(NULL, scale, scale, scale);
        njCnkSimpleDrawObject(debug_cube_model);
        njPopMatrixEx();

        SetMaterial(1, 1, 1, 1);

        LoadControl3D();
    }
}

void AL_FieldDestructor(task* tp) {
}

void AL_ChildFieldDestructor(task* tp) {
    if (tp->ptp) {
        task* parent = tp->ptp;
        GET_CHAOWK(parent)->pAnyTask = 0;
    }
}

void AL_FieldSetScale(task* tp, float scl) {
    taskwk* work = tp->twp;
    work->scl.x = scl;
}

static void sub_C50ADD2(task* tp, int a2) {
    taskwk* work = tp->twp;
    if (a2 > 0) {
        work->smode = 1;
        work->wtimer = a2;
    } else {
        work->smode = 0;
    }
}

void AL_FieldSetKind(task* tp, int kind) {
    CCL_INFO* info = CCL_GetInfo(tp, 0);
    info->kind = kind;
}

task* AL_FieldCreate(unsigned int kind, NJS_POINT3* pPos, float radius) {
    return AL_FieldCreateT(kind, pPos, radius, 0);
}

task* AL_FieldCreateT(unsigned int kind, NJS_POINT3* pPos, float radius, int time) {
    return AL_FieldCreateTT(kind, pPos, radius, 0, time);
}

const CCL_INFO ccl_C55DA84[] = { CCL_SPHERE(0, 112, 12, 819200, 0, 0, 0, 10, 0, 0) };
task* AL_FieldCreateTT(unsigned int kind, NJS_POINT3* pPos, float radius, int WaitTime, int ActiveTime) {
    task* tp = CreateElementalTask(2, 2, AL_FieldExecutor, "AL_FieldExecutor");
    taskwk* work;
    CCL_INFO* info;

    tp->disp = AL_FieldDisplayer;
    tp->dest = AL_FieldDestructor;
    CCL_Init(tp, ccl_C55DA84, 1, 4);

    info = CCL_GetInfo(tp, 0);
    info->kind = kind;
    info->a = radius;

    work = tp->twp;
    work->pos = *pPos;
    work->scl.x = radius;

    sub_C50ADD2(tp, ActiveTime);
    work->ang.x = WaitTime;

    return tp;
}

task* AL_AllFieldCreate(unsigned int kind, NJS_POINT3* pPos, float radius) {
    return AL_AllFieldCreateT(kind, pPos, radius, 0);
}

task* AL_AllFieldCreateT(unsigned int kind, NJS_POINT3* pPos, float radius, int time) {
    return AL_AllFieldCreateTT(kind, pPos, radius, 0, time);
}

task* AL_AllFieldCreateTT(unsigned int kind, NJS_POINT3* pPos, float radius, int WaitTime, int ActiveTime) {
    task* tp = CreateElementalTask(2, 2, AL_FieldExecutor, "AL_FieldExecutor");
    taskwk* work;
    CCL_INFO* info;

    tp->disp = AL_FieldDisplayer;
    tp->dest = AL_FieldDestructor;
    CCL_Init(tp, ccl_C55DA84, 1, 3);

    info = CCL_GetInfo(tp, 0);
    info->kind = kind;
    info->a = radius;

    work = tp->twp;
    work->pos = *pPos;
    work->scl.x = radius;

    sub_C50ADD2(tp, ActiveTime);
    work->ang.x = WaitTime;

    return tp;
}

task* AL_ChildFieldCreate(task* pParentTask, unsigned int kind, NJS_POINT3* pPos, float radius) {
    return AL_ChildFieldCreateT(pParentTask, kind, pPos, radius, 0);
}

task* AL_ChildFieldCreateT(task* pParentTask, unsigned int kind, NJS_POINT3* pPos, float radius, int time) {
    task* tp = CreateChildTask(2, AL_FieldExecutor, pParentTask);
    taskwk* work;
    CCL_INFO* info;

    tp->disp = AL_FieldDisplayer;
    tp->dest = AL_ChildFieldDestructor;
    CCL_Init(tp, ccl_C55DA84, 1, 3);

    info = CCL_GetInfo(tp, 0);
    info->kind = kind;
    info->a = radius;

    work = tp->twp;
    work->pos = *pPos;
    work->scl.x = radius;

    sub_C50ADD2(tp, time);

    GET_CHAOWK(pParentTask)->pAnyTask = tp;

    return tp;
}

extern CCL_INFO ccl_C56B324[];
extern Sint8 AL_ColliKindList[];
extern int nbColliKindList;

void sub_C50B036(taskwk* twp) {
    CCL_INFO* info = twp->cwp->info;
    info->a = twp->scl.x + 10.0;
    info->b = twp->scl.y + 10.0;
    info->c = twp->scl.z + 10.0;
    CCL_CalcRange(twp->cwp);
}
void sub_C50B068(task* tp) {
    taskwk* work = tp->twp;
    CCL_INFO* info;

    if (!CheckRangeOutWithR(tp)) {
        switch (work->mode) {
            case 0:
                CCL_Init(tp, ccl_C56B324, 1, 3);
                sub_C50B036(work);

                info = CCL_GetInfo(tp, 0);
                info->kind = AL_ColliKindList[(Uint16)work->ang.x % nbColliKindList];
                work->mode = 1;
                break;
            case 1:
                CCL_Entry(tp);
                break;
            default:
                work->mode = 0;
                break;
        }
    }
}

void sub_C50B0D0(taskwk* twp) {
    CCL_INFO* info = twp->cwp->info;
    info->a = twp->scl.x + 10.0;
    info->b = twp->scl.y + 10.0;
    info->c = twp->scl.z + 10.0;
    info->angx = twp->ang.x;
    info->angy = twp->ang.y;
    info->angz = twp->ang.z;
    CCL_CalcRange(twp->cwp);
}
extern CCL_INFO ccl_C56B3A8[];
void sub_C50B160(task* tp) {
    taskwk* work = tp->twp;
    CCL_INFO* info;

    if (!CheckRangeOutWithR(tp)) {
        switch (work->mode) {
            case 0:
                CCL_Init(tp, ccl_C56B3A8, 1, 4);
                sub_C50B0D0(work);
                work->mode = 1;
                break;
            case 1:
                CCL_Entry(tp);
                break;
            default:
                work->mode = 0;
                break;
        }
    }
}
extern CCL_INFO ccl_C56B3D8[];
void sub_C50B1AC(task* tp) {
    taskwk* work = tp->twp;
    CCL_INFO* info;

    if (!CheckRangeOutWithR(tp)) {
        switch (work->mode) {
            case 0:
                CCL_Init(tp, ccl_C56B3D8, 1, 4);
                sub_C50B036(work);
                work->mode = 1;
                break;
            case 1:
                CCL_Entry(tp);
                break;
            default:
                work->mode = 0;
                break;
        }
    }
}
// this is the one with the debug menu i think
void sub_C50B1F8(task* tp) {
    taskwk* work = tp->twp;
    int flag;

    if (!CheckRangeOutWithR(tp)) {
        switch (work->mode) {
            case 0:
                CCL_Init(tp, ccl_C56B324, 1, 4);
                sub_C50B036(work);
                work->mode = 1;
                break;
            case 1:
                flag = FALSE;

                while (TRUE) {
                    c_colli_hit_info* pPlayer = CCL_IsHitPlayerWithNum2(tp, 0);
                    if (pPlayer) {
                        if (!pPlayer->hit_num) {
                            AL_ChangeStageLater(7);
                            work->mode = 2;
                            break;
                        }
                    } else {
                        flag = TRUE;
                        break;
                    }
                }

                if (flag) {
                    CCL_Entry(tp);
                }
                break;
            case 2:
                break;
            default:
                work->mode = 0;
                break;
        }
    }
}

void sub_C50B2C6(task* tp) {
    taskwk* work = tp->twp;
    int flag;

    if (!CheckRangeOutWithR(tp)) {
        switch (work->mode) {
            case 0:
                CCL_Init(tp, ccl_C56B324, 1, 4);
                sub_C50B036(work);
                work->mode = 1;
                break;
            case 1:
                flag = FALSE;

                while (1) {
                    c_colli_hit_info* pPlayer = CCL_IsHitPlayerWithNum2(tp, 0);
                    if (pPlayer) {
                        if (!pPlayer->hit_num) {
                            AL_ChangeStageLater(5);
                            work->mode = 2;
                            break;
                        }
                    } else {
                        flag = TRUE;
                        break;
                    }
                }

                if (flag) {
                    CCL_Entry(tp);
                }
                break;
            case 2:
                break;
            default:
                work->mode = 0;
                break;
        }
    }
}
 
INLINE_ASM(_rodata_1, "asm/nonmatching/Chao/al_field/rodata_1.src")