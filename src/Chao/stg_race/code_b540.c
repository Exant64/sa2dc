#include <task.h>
#include <Chao/Chao.h>

int lbl_0C9CB540(task* tp){
    chaowk* cwk = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &cwk->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, 0x1EC);
            bhv->Mode++;
            bhv->Timer = 0;
            if(cwk->param.body.BodyType == 3) {
                SetMotionLink(&cwk->MiniMotionCtrl, 0);
            }
            
            break;
        case 1:
            break;
    }
    
    return 0;
}
MERGE_LIST([['_AL_SetMotionLink', '_lbl_0C9CB6E0'], ['_SetMotionLink', '_lbl_0C9CB6E4']]);
int lbl_0C9CB598(task* tp){
    chaowk* cwk = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &cwk->Behavior;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, 0x23E);
            GET_CHAOWK(tp)->ChaoFlag &= ~0x40;
            GET_CHAOWK(tp)->ChaoFlag |= 0x80;
            bhv->Mode++;
            bhv->Timer = 0;
            if(cwk->param.body.BodyType == 3) {
                SetMotionLink(&cwk->MiniMotionCtrl, 0);
            }
            
            break;
        case 1:
            break;
    }
    
    return 0;
}
MERGE_LIST([['_AL_SetMotionLink', '_lbl_0C9CB6E0'], ['_SetMotionLink', '_lbl_0C9CB6E4']]);
#if 0
int lbl_0C9CB614(task* tp){
    chaowk* cwk = GET_CHAOWK(tp);
    AL_BEHAVIOR* bhv = &cwk->Behavior;
    MOVE_WORK* move = (MOVE_WORK*)tp->Data2;
    MOVE_WORK* move2 = (MOVE_WORK*)tp->Data2;

    switch(bhv->Mode) {
        case 0:
            AL_SetMotionLink(tp, 0x64);
            GET_CHAOWK(tp)->ChaoFlag &= ~0x40;
            GET_CHAOWK(tp)->ChaoFlag |= 0x80;
            bhv->Mode++;
            bhv->Timer = 0;
            break;
        case 1:
            break;
    }

    if(!(move2->Flag & 0x400)) { } else {
        move2->Acc.x = njSin(cwk->ang.y) * 0.045;
        move2->Acc.z = njCos(cwk->ang.y) * 0.045;
    }
    
    return 0;
}
#endif
INLINE_ASM(_func_0C9CB614, "asm/nonmatching/Chao/stg_race/code_b540/_func_0C9CB614.src");

//MERGE_LIST([['_AL_SetMotionLink', '_lbl_0C9CB6E0'], ['_njCos', '_lbl_0C9CB6F0'], ['_njSin', '_lbl_0C9CB6EC'], ["h'3D3851EC", '_lbl_0C9CB6E8']]);
INLINE_ASM(_func_0C9CB6A6, "asm/nonmatching/Chao/stg_race/code_b540/_func_0C9CB6A6.src");

