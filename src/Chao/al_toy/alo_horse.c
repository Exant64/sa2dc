#include <Chao/Chao.h>
#include <c_colli.h>

const CCL_INFO ccl_C5674F0[] = { CCL_SPHERE(0, 119, 12, 0, 0, 1.2f, 0, 2, 0, 0) };

void ALO_HorseExecutor(task* tp) {
    taskwk* work = tp->twp;
    MOTION_CTRL* motion = (MOTION_CTRL*)tp->UnknownA_ptr;
    switch (work->mode) {
        case 0:
            if (CCL_IsHitKind2(tp, CI_KIND_AL_TOUCH)) {
                work->mode = 1;
                work->smode = 0;
                work->wtimer = 0;
                work->id = 0;
                SetMotionSkip(motion, 3);
                work->wtimer = 1800;
            }
            break;
        case 1:
            if (--work->wtimer <= 0) {
                SetMotionSkip(motion, 4);
                work->mode = 0;
                work->smode = 0;
                work->wtimer = 0;
                work->id = 0;
            }
            if (!ALW_IsCommunication(tp)) {
                SetMotionSkip(motion, 4);
                ALW_CommunicationOff(tp);
                work->mode = 0;
                work->smode = 0;
                work->wtimer = 0;
                work->id = 0;
            }
            break;
        case 2:
            break;
    }
    MotionControl(motion);
    CCL_Entry(tp);
}

extern NJS_CNK_OBJECT object_alo_horse[];
extern MOTION_TABLE off_C636050[];
extern NJS_CNK_MODEL model_kage_marukage_marukage[];
int AL_IsOnScreen3(task* tp, float x, float y, float z);
void ALO_HorseDisplayer(task* tp) {
    taskwk* work = tp->twp;
    MOTION_CTRL* motion = (MOTION_CTRL*)tp->UnknownA_ptr;

    if (AL_IsOnScreen3(tp, 3.4f, 3.2f, 0)) {
        njSetTexture(&AL_TOY_TEXLIST);
        njPushMatrixEx();
        njTranslateV(0, &work->pos);
        njRotateY(0, work->ang.y);
        njPushMatrixEx();
        njTranslate(0, 3, 0, 0);
        OnControl3D(0x2400);
        haCnkSimpleDrawMotion(object_alo_horse, motion);
        OffControl3D(0x2400);
        njPopMatrixEx();
        if (ALW_ENTRY_WORK(tp)->CamDist < GET_GLOBAL()->CamDistShadowCutLev3) {
            njTranslate(0, 0, 0.8f, 0);
            njScale(0, 0.7f, 0.7f, 1.8f);
            njCnkModDrawModel(model_kage_marukage_marukage);
        }
        njPopMatrixEx();
    }
}
void ALO_HorseDestroyer(task* tp) {
    ALW_CancelEntry(tp);
}

void ALO_HorseLoad(task* tp) {
    taskwk* work = tp->twp;
    MOTION_CTRL* motion = (MOTION_CTRL*)syCalloc(1, sizeof(MOTION_CTRL));
    MotionInit(motion, off_C636050);
    SetMotionSkip(motion, 4);
    tp->UnknownA_ptr = motion;

    MOV_Init(tp);
    MOV_SetGravity(tp, -0.05f);
    sub_8C033CEA(tp, 0xD00);
    CCL_Init(tp, ccl_C5674F0, 1, 5);
    tp->exec = ALO_HorseExecutor;
    tp->dest = ALO_HorseDestroyer;
    tp->disp = ALO_HorseDisplayer;
    work->mode = 0;
    work->smode = 0;
    ALW_Entry(ALW_CATEGORY_TOY, tp, ALW_KIND_HORSE);
}

void CreateALO_Horse(NJS_VECTOR* pos, int ang) {
    task* horse = CreateElementalTask(2, 3, ALO_HorseLoad, "ALO_Horse");
    taskwk* work = horse->twp;
    work->pos = *pos;
    work->ang.y = ang;
}