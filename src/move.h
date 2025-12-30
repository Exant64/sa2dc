#ifndef _MOVE_H_
#define _MOVE_H_

#include <task.h>

enum {
    XYZS_ASS_XL = 0x0,
    XYZS_ASS_XU = 0x1,
    XYZS_ASS_YL = 0x2,
    XYZS_ASS_YU = 0x3,
    XYZS_ASS_ZL = 0x4,
    XYZS_ASS_ZU = 0x5,
    XYZS_ASS_NMB = 0x6,
};

typedef struct xssunit {
    int findflag;
    int objatt;
    int angx;
    int angz;
    float onpos;
    NJS_POINT3 normal;
} xssunit;

typedef struct XYZ_SHADOW_WORK {
    NJS_POINT3 pos;
    xssunit hit[XYZS_ASS_NMB];
    xssunit pre_hit[XYZS_ASS_NMB];
} XYZ_SHADOW_WORK;

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
    XYZ_SHADOW_WORK Shadow;
} MOVE_WORK;

MOVE_WORK* MOV_Init(task* tp);
float MOV_Dist2FromPlayer(task* tp, int player_num);
float MOV_DistFromAim(task* tp);
float MOV_Dist2FromAim(task* tp);
int MOV_TurnToAim2(task* tp, int rot_spd);
int MOV_CalcPlayerAngle(task*, int index);

#endif