#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <task.h>

typedef struct _camcontwk
{
    NJS_POINT3 pos;
    Angle3     ang;
    NJS_VECTOR dir;
    NJS_POINT3 tgt;
    NJS_POINT3 diff;
    float      spd;
}
camcontwk;

extern camcontwk* cameraControlWork[2];

#endif