#ifndef _AL_FIELD_H_
#define _AL_FIELD_H_

#include <Chao/Chao.h>

task* AL_FieldCreate(unsigned int kind, NJS_POINT3* pPos, float radius);
task* AL_FieldCreateT(unsigned int kind, NJS_POINT3* pPos, float radius, int time);
task* AL_FieldCreateTT(unsigned int kind, NJS_POINT3* pPos, float radius, int WaitTime, int ActiveTime);

task* AL_AllFieldCreateTT(unsigned int kind, NJS_POINT3* pPos, float radius, int WaitTime, int ActiveTime);
task* AL_ChildFieldCreateT(task* pParentTask, unsigned int kind, NJS_POINT3* pPos, float radius, int time);
task* AL_AllFieldCreateT(unsigned int kind, NJS_POINT3* pPos, float radius, int time);

#endif