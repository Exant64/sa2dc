#ifndef _TASK_H_
#define _TASK_H_

#include <ninja.h>

typedef void (*task_exec)(struct task*);
typedef struct _OBJ_EDITENTRY {
    Uint16 usID;
    Sint16 rotx;
    Sint16 roty;
    Sint16 rotz;
    float xpos;
    float ypos;
    float zpos;
    float xscl;
    float yscl;
    float zscl;
} _OBJ_EDITENTRY;

typedef struct Angle3 {
    int x;
    int y;
    int z;
} Angle3;

typedef struct _OBJ_SLEEPCON {
    Angle3 ang;
    NJS_POINT3 pos;
    NJS_POINT3 scl;
} _OBJ_SLEEPCON;

typedef union _UNION_STATUS {
    float fRangeOut;
    _OBJ_SLEEPCON* pObjSleepCondition;
} _UNION_STATUS;

typedef struct OBJ_CONDITION {
    char scCount;
    char scUserFlag;
    Uint16 ssCondition;
    struct task* pTask;
    _OBJ_EDITENTRY* pObjEditEntry;
    _UNION_STATUS unionStatus;
} OBJ_CONDITION;

typedef struct colliwk {
    Uint16 id;
    Sint16 nbHit;
    Uint16 flag;
    Uint16 nbInfo;
    float colli_range;
    struct CCL_INFO* info;
} colliwk;
typedef struct taskwk {
    char mode;
    char smode;
    char btimer;
    Uint8 id;
    Sint16 flag;
    Uint16 wtimer;
    Angle3 ang;
    NJS_VECTOR pos;
    NJS_VECTOR scl;
    colliwk* cwp;
} taskwk;

typedef struct task {
    struct task* last;
    struct task* next;
    struct task* ptp;
    struct task* ctp;
    task_exec exec;
    task_exec disp;
    task_exec dest;
    void* ocp;
    taskwk* twp;
    void* Data2;
    void* EntityData2;
    void* UnknownA_ptr;

} task;

#define TWK_FLAG_HELD 0x8000

void DeleteTaskSub(task* tp);

task* CreateElementalTask(Uint16 im, int level, task_exec exec, const char* name);
task* CreateChildTask(Uint16 im, task_exec exec, task* tp);

enum {
    CI_KIND_OSAMPO = 0x1,
    CI_KIND_PATTING_HAND = 0x2,
    CI_KIND_SAPLING = 0x3,
    CI_KIND_SAPLING_FIELD = 0x4,
    CI_KIND_KAME_FIELD = 0x5,
    CI_KIND_ENEMYBALL = 0x6,
    CI_KIND_PUNCH = 0x7,
    CI_KIND_2PLASER = 0x8,
    CI_KIND_2PSHOTS = 0x9,
    CI_KIND_2PMISSILES = 0xA,
    CI_KIND_NO_RIDE = 0xB,
    CI_KIND_NO_PUNCH = 0xC,
    CI_KIND_NO_LOCKON = 0xD,
    CI_KIND_NO_CATCH = 0xE,
    CI_KIND_DO_CATCH = 0xF,
    CI_KIND_NO_COLLIDE = 0x10,
    CI_KIND_G_CYLINDER = 0x11,
    CI_KIND_NO_HOLD_FIELD = 0x12,
    CI_KIND_BOMB_EXPLOSION = 0x13,
    CI_KIND_CHAOSDRIVE = 0x14,
    CI_KIND_DUMMY = 0x40,
};

#endif