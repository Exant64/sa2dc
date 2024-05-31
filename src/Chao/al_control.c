#include <Chao/Chao.h>
#include <c_colli.h>
extern int alCtrlMove; // make not extern when we start doing rodata shit

void AL_EnableMove(void) {
    alCtrlMove = TRUE;
}

void AL_DisableMove(void) {
    alCtrlMove = FALSE;
}

int AL_IsMovable(void) {
    return alCtrlMove;
}

void AL_EnablePushout(task* tp) {
    CCL_INFO* info = CCL_GetInfo(tp, 1);
    info->push |= 7;
}

void AL_DisablePushout(task* tp) {
    CCL_INFO* info = CCL_GetInfo(tp, 1);
    info->push &= ~7;
}

void AL_FixPosition(task* tp) {
    CCL_INFO* info = CCL_GetInfo(tp, 1);
    info->push |= 0x70;
}

void AL_ReleasePosition(task* tp) {
    CCL_INFO* info = CCL_GetInfo(tp, 1);
    info->push &= ~0x70;
}

void AL_EnableTouch(task* tp) {
    tp->twp->flag |= 0x240u;
    CCL_Enable(tp, 0);
    CCL_Enable(tp, 2);
}

void AL_DisableTouch(task* tp) {
    tp->twp->flag &= ~0x240u;
    CCL_Disable(tp, 0);
    CCL_Disable(tp, 2);
}

// im guessing enablenade, but it was copypasted and never fixed cuz its never used
void AL_EnableNade(task* tp) {
    CCL_Disable(tp, 2);
}

void AL_DisableNade(task* tp) {
    CCL_Disable(tp, 2);
}