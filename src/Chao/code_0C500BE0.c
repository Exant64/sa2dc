#include "task.h"

typedef struct be4wk {
    char mode;
    char smode;
    char btimer;
    char id;
    Sint16 flag;
    Sint16 wtimer;
    Angle3 ang;
    int padding[3];
    NJS_VECTOR pos;
    NJS_VECTOR scl;
    void* Collision;
} be4wk;

void nullsub_1(void) {
}

void sub_C500BE4(task* tp) {
    be4wk* wk = (be4wk*)tp->twp;
    switch (wk->mode) {
        case 0:
            wk->ang.x = 0;
            wk->mode++;
        case 1:
            wk->pos.x = (njSin(wk->ang.x) + 1) * 0.5f;
            wk->ang.x += 0x1B0;
            wk->ang.x &= 0xFFFF;
            break;
    }
}