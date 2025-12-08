#include <Chao/Chao.h>
#include <Chao/al_race_info.h>
#include <c_colli.h>

void ChaoExecutor(task* tp) {
    chaowk* work = GET_CHAOWK(tp);

    if (work->Timer <= 2) {
        AL_IconResetPos(tp);
    }

    if (AL_IsMovable()) {
        AL_BehaviorControl(tp);
        AL_MotionControl(tp);
        AL_IconControl(tp);
        AL_FaceControl(tp);
        AL_ShapeControl(tp);

        if (work->ChaoFlag & CHAO_FLAG_MOVE_ENABLE) {
            MOV_Control(tp);
        }

        AL_DetectCollision(tp);

        ++work->Timer;

        if (work->ChaoFlag & CHAO_FLAG_CCL_ENABLE) {
            CCL_Entry(tp);
        } else {
            CCL_ClearEntry(tp);
        }

        switch (work->param.body.BodyType) {
            case AL_FORM_MINIMAL:
                MotionControl(&work->MiniMotionCtrl);
                break;
        }
    }
}

void ChaoDestructor(task* tp) {
    AL_ShapeExit(tp);
    AL_BehaviorExit(tp);
    ALW_CancelEntry(tp);
}

extern NJS_CNK_OBJECT object_al_eggchao_eggchao[];
extern NJS_CNK_OBJECT object_al_omorunner_omorunner[];

extern NJS_CNK_OBJECT* off_C5A8128[];
void ChaoDisplayer(task* tp) {
    chaowk* work = GET_CHAOWK(tp);
    CHAO_PARAM* pParam = &work->param;

    switch (pParam->body.BodyType) {
        case AL_FORM_NORMAL:
        case AL_FORM_CHIBI:
            AL_Draw(tp);
            break;
        case AL_FORM_EGG_FOOT:
            njPushMatrixEx();
            njTranslateV(NULL, &work->pos);
            njRotateZ(NULL, work->ang.z);
            njRotateX(NULL, work->ang.x);
            njRotateY(NULL, work->ang.y);
            njSetTexture(&AL_BODY_TEXLIST);
            haCnkSimpleDrawMotion(object_al_eggchao_eggchao, &work->MotionCtrl);
            njPopMatrixEx();
            break;
        case AL_FORM_OMOCHAO:
            njPushMatrixEx();
            njTranslateV(NULL, &work->pos);
            njRotateZ(NULL, work->ang.z);
            njRotateX(NULL, work->ang.x);
            njRotateY(NULL, work->ang.y);
            njSetTexture(&AL_BODY_TEXLIST);
            haCnkSimpleDrawMotion(object_al_omorunner_omorunner, &work->MotionCtrl);
            njPopMatrixEx();
            break;
        case AL_FORM_MINIMAL:
            njPushMatrixEx();
            njTranslateV(NULL, &work->pos);
            njRotateZ(NULL, work->ang.z);
            njRotateX(NULL, work->ang.x);
            njRotateY(NULL, work->ang.y);
            njSetTexture(&off_C5FFDEC);
            haCnkSimpleDrawMotion(off_C5A8128[pParam->body.FormSubNum], &work->MiniMotionCtrl);
            njPopMatrixEx();
            break;
    }
}

CCL_INFO chao_colli_info[] = {
    CCL_CYLINDER(CI_KIND_AL_DAKKO_FIELD, CI_PUSH_TH_CMN, 0, 7872520, 0, 2, 0, 15, 4, 0),
    CCL_SPHERE(CI_KIND_AL_CHAO, CI_PUSH_PO_CMN, 0, CI_ATTR_2000 | CI_ATTR_DAMAGE, 0, 2, 0, 1.5f, 2.5f, 0.7f),
    CCL_CYLINDER(CI_KIND_AL_NADE_FIELD, CI_PUSH_TH_CMN, 0, 7872520, 0, 2, 0, 5, 4, 0),
    CCL_SPHERE(CI_KIND_AL_PUNCH, CI_PUSH_TH_CMN, CI_DMG_ALL | CI_DMG_SET(2, 3), 9232, 0, 2, 3.7f, 2.8f, 0, 0),
    CCL_SPHERE(0, CI_PUSH_TH_CMN, 0, CI_ATTR_2000 | CI_ATTR_DAMAGE | CI_ATTR_IGNORE, 0, 2, 0, 1.4f, 0, 0)
};
task* CreateChao(AL_GENE* pGene, CHAO_PARAM* pParam, AL_SHAPE_ELEMENT* pElement, NJS_POINT3* pPos, int angy,
                 chao_info* pInfo) {
    task* tp = CreateElementalTask(0, 2, ChaoExecutor, "CHAO");
    MOVE_WORK* move;
    int stage;
    chaowk* work;
    chaowk* work2;

    tp->twp = (chaowk*)syCalloc(1, 0x1700); // sizeof(chaowk)
    work = tp->twp;
    if (pPos)
        work->pos = *pPos;
    work->ang.y = angy;

    ALW_Entry(ALW_CATEGORY_CHAO, tp, 0);

    work2 = tp->twp;
    tp->dest = ChaoDestructor;
    tp->disp = ChaoDisplayer;

    CCL_Init(tp, chao_colli_info, 5, 9);
    work2->CurrZone = 0;

    move = MOV_Init(tp);
    MOV_SetGravity(tp, -0.055f);
    move->Timer = work->id;
    move->Offset.y = 3;
    move->Top = 1.5f;
    move->Bottom = -3;
    move->BoundFloor = GET_GLOBAL()->BoundFloor;
    move->BoundFriction = GET_GLOBAL()->BoundFriction;
    if (GetCharID(0) != 3) {
        move->rad = 0x1400;
    } else {
        move->rad = -0x1000;
    }
    move->Flag |= 8;

    if (pGene) {
        work2->gene = *pGene;
    } else {
        AL_GeneCreate(&work2->gene);
    }
    if (pParam) {
        work2->param = *pParam;
    } else {
        AL_GeneAnalyzeCommon(tp);
    }

    stage = AL_GetStageNumber();
    if (stage <= 3) {
        GET_CHAOPARAM(tp)->place = stage;
    }

    if (pElement) {
        AL_ShapeExpandElementToParam(&work2->param, pElement);
    }

    AL_PerceptionInit(tp);
    AL_BehaviorInit(tp);
    AL_MotionInit(tp);
    AL_ShapeInit(tp);
    AL_IconInit(tp);
    AL_FaceInit(tp);
    AL_PartsInit(tp);

    ObjectMovableInitialize(work, tp->Data2, 10);
    GET_CHAOWK(tp)->ChaoFlag |= CHAO_FLAG_DRAW_ENABLE;
    GET_CHAOWK(tp)->ChaoFlag |= CHAO_FLAG_CCL_ENABLE;
    GET_CHAOWK(tp)->ChaoFlag |= CHAO_FLAG_MOVE_ENABLE;
    GET_CHAOWK(tp)->ChaoFlag |= CHAO_FLAG_MOTION_ENABLE;
    GET_CHAOWK(tp)->ChaoFlag |= CHAO_FLAG_BHV_ENABLE;
    GET_CHAOWK(tp)->ChaoFlag |= CHAO_FLAG_UNK_17;
    GET_CHAOWK(tp)->ChaoFlag |= CHAO_FLAG_BUYO_ENABLE;
    GET_CHAOWK(tp)->ChaoFlag |= CHAO_FLAG_UNK_6;
    GET_CHAOWK(tp)->ChaoFlag &= ~CHAO_FLAG_UNK_7;
    GET_CHAOWK(tp)->ChaoFlag &= ~CHAO_FLAG_RACE_BHV_ENABLE;
    GET_CHAOWK(tp)->ChaoFlag |= CHAO_FLAG_UNK_18;
    // sa2b
    // GET_CHAOWK(tp)->ChaoFlag |= 0x80000;
    // GET_CHAOWK(tp)->ChaoFlag |= 0x100000;

    if (pInfo) {
        work2->pInfo = pInfo;
        sub_C5025E0(tp, work2->pInfo);
    }

    if (pParam && pParam->body.BodyType == AL_FORM_MINIMAL) {
        InitMtnTable(work2->MiniMotionTable, pParam->body.FormSubNum);
        MotionInit(&work2->MiniMotionCtrl, work2->MiniMotionTable);
        SetMotionLink(&work2->MiniMotionCtrl, 1);
    }

    return tp;
}

task* CreateChaoOnlyParam(AL_GENE* pGene, CHAO_PARAM* pParam, NJS_POINT3* pPos, int angy) {
    return CreateChao(pGene, pParam, 0, pPos, angy, 0);
}

task* CreateChaoElement(AL_SHAPE_ELEMENT* pElement, NJS_POINT3* pPos, int angy) {
    return CreateChao(0, 0, pElement, pPos, angy, 0);
}

void KillChao(task* tp) {
    chaowk* work = GET_CHAOWK(tp);

    AL_ClearChaoParam(work->pInfo);
    work->pInfo = 0;

    FreeTask(tp);
}