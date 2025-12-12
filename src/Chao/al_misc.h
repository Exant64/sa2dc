#ifndef _AL_MISC_H_
#define _AL_MISC_H_

#include <task.h>

task* AL_IsHitKindWithNum(task* tp, int num, int kind);
void AL_InterpolateObject(NJS_CNK_OBJECT* pSrc1,NJS_CNK_OBJECT* pSrc2,NJS_CNK_OBJECT* pDst, float t);

#endif