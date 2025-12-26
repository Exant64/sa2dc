#ifndef _ALO_BALL_H_
#define _ALO_BALL_H_

#include <task.h>

extern task* pBallTask; // C682CB0;
extern int BallFlag; // lbl_0C682CB4
extern int BallWaterFlag; // 0C682CB8

void ALO_BallCreate(NJS_POINT3* pPos, NJS_VECTOR* pVelo);

#endif