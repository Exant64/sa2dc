#include <Chao/Chao.h>
#include <Chao/al_behavior/albhv_move.h>
#include <Chao/al_landmark.h>

int ALBHV_JumpToPond(task* tp) {
    chaowk* work = GET_CHAOWK(tp);

    // fake match?
    AL_BEHAVIOR* bhv;
    MOVE_WORK* move;
    bhv = &work->Behavior, move = (MOVE_WORK*)tp->Data2;

    switch (bhv->Mode) {
        case 0: {
            int ang;

            AL_FaceSetEye(tp, AL_EYE_NUM_KYA, -1);
            AL_FaceSetMouth(tp, AL_MOUTH_NUM_HOYO, -1);
            AL_SetMotionLink(tp, ALM_HABATOBI_JUMP);
            ang = GET_CHAOWK(tp)->ang.y;
            move->Velo.x = njSin(ang) * 0.4f;
            move->Velo.y = 0.35f;
            move->Velo.z = njCos(ang) * 0.4f;
            bhv->Timer = 0;
            bhv->Mode++;
        } break;
        case 1:
            if (move->Flag & 0x400) {
                AL_SetBehavior(tp, ALBHV_Move);
            }
            break;
    }
    move->Acc.y = move->Gravity * -0.45f;
    return 0;
}

int ALBHV_GoToWater(task* tp) {
    MOVE_WORK* move = (MOVE_WORK*)tp->Data2;
    AL_GetRandomAttrPos(LMA_WATER, &move->AimPos);

    if (njRandom()) //??? lol
        sub_8C05B020(24643, 0, 0, 110, &GET_CHAOWK(tp)->pos);
    else
        sub_8C05B020(24644, 0, 0, 110, &GET_CHAOWK(tp)->pos);

    AL_WalkSelect(tp);

    return 0;
}