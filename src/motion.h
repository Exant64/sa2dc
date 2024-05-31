#ifndef _MOTION_H_
#define _MOTION_H_

#include <task.h>

typedef struct MOTION_INFO {
    Uint16 mode;
    float frame;
    float start;
    float end;
    float spd;
    NJS_MOTION* pMotion;
} MOTION_INFO;

typedef struct MOTION_TABLE {
    NJS_MOTION* pMotion;
    Sint16 mode;
    Sint16 posture;
    int next;
    int link_step;
    float start;
    float end;
    float spd;
} MOTION_TABLE;

typedef struct MOTION_CTRL {
    Uint16 flag;
    Uint16 posture;
    int curr_num;
    int next_num;
    float multi_spd;
    float link_spd;
    float ratio;
    MOTION_INFO minfo[2];
    MOTION_TABLE* table;
} MOTION_CTRL;

void MotionInit(MOTION_CTRL*, MOTION_TABLE*);
void MotionControl(MOTION_CTRL*);
void SetMotionLink(MOTION_CTRL* pMtnCtrl, int MtnNum);
void SetMotionLinkStep(MOTION_CTRL* pMtnCtrl, int MtnNum, Uint16 step);
void SetMotionTable(MOTION_CTRL*, MOTION_TABLE*);
float GetMotionFrame(MOTION_CTRL*);
float sub_8C033990(MOTION_CTRL*);

#endif