#ifndef _MOVE_H_
#define _MOVE_H_

#include <task.h>

typedef struct MOVE_WORK {
    NJS_POINT3 Velo;
    NJS_POINT3 Acc;
    Angle3 AimAng;
    Angle3 RotSpd;
    int rad;
    float height;
    float weight;
    int padding;
    Uint16 Flag;
    Uint16 Timer;
    float Spd;
    float Gravity;
    int ViewAngle;
    float ViewRange;

    NJS_POINT3 AimPos;
    NJS_POINT3 PrePos;
    NJS_POINT3 HomePos;
    Angle3 HomeAng;
    Angle3 Phase;
    NJS_LINE FrontWall;
    NJS_POINT3 Offset;
    float Top;
    float Side;
    float Bottom;
    float CliffHeight;
    float BoundSide;
    float BoundFloor;
    float BoundCeiling;
    float BoundFriction;
    float TopY;
    float BottomY;
    float WaterY;
    // XYZ_SHADOW_WORK Shadow;
} MOVE_WORK;

MOVE_WORK* MOV_Init(task* tp);
float MOV_Dist2FromPlayer(task* tp, int player_num);
float MOV_DistFromAim(task* tp);
int MOV_TurnToAim2(task* tp, int rot_spd);

#endif