#include <Chao/Chao.h>

void sub_C528EC0(task* tp) {
    taskwk* work = tp->twp;
    work->scl.x += 0.3f;
    if (work->scl.x > work->scl.y)
        FreeTask(tp);
}

void sub_C528EE6(task* tp) {
    NJS_VECTOR v2, v1;
    taskwk* work = tp->twp;

    v2 = work->pos;

    v1.x = work->pos.x;
    v1.y = work->pos.y + work->scl.x;
    v1.z = work->pos.z;

    DrawLineV(&v2, &v1);
}

void sub_C528F2A(void) {
}

// probably a create task function but unused static functions get optimized out so we only have this string left
// i also can't make a variable that contains the string because that generates a pointer to it in the data section,
// which we don't want
static void rodata_placeholder() {
    "ALEF_Ripple";
}