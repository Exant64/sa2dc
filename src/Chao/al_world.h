#ifndef _AL_WORLD_H_
#define _AL_WORLD_H_

#include <task.h>

enum {
    ALW_CATEGORY_CHAO = 0x0,
    ALW_CATEGORY_EGG = 0x1,
    ALW_CATEGORY_MINIMAL = 0x2,
    ALW_CATEGORY_FRUIT = 0x3,
    ALW_CATEGORY_TREE = 0x4,
    ALW_CATEGORY_GROWTREE = 0x5,
    ALW_CATEGORY_TOY = 0x6,
    ALW_CATEGORY_SEED = 0x7,
    ALW_CATEGORY_SOUND = 0x8,
    ALW_CATEGORY_MASK = 0x9,
    NB_CATEGORY
};

enum {
    ALW_CMD_ERROR = 0xFFFFFFFF,
    ALW_CMD_NONE = 0x0,
    ALW_CMD_GO = 0x1,
    ALW_CMD_EAT = 0x2,
    ALW_CMD_CHANGE = 0x3,
    ALW_CMD_FINISH = 0x4,
    ALW_CMD_PLANTED = 0x5,
    ALW_CMD_BYE = 0x6,
};

enum {
    ALW_KIND_NONE = 0x0,
    ALW_KIND_CDR = 0x1,
    ALW_KIND_TV = 0x2,
    ALW_KIND_RADICASE = 0x3,
    ALW_KIND_BALL = 0x4,
    ALW_KIND_PIANO = 0x5,
    ALW_KIND_ORGAN = 0x6,
    ALW_KIND_HORSE = 0x7,
    ALW_KIND_BOX = 0x8,
};

typedef struct al_entry_work {
    Uint16 category;
    Uint16 num;
    Uint16 kind;
    Uint16 flag;
    int CommuID;
    NJS_POINT3 pos;
    Angle3 ang;
    float radius;
    float offset;
    float CamDist;
    Sint16 command;
    Sint16 command_value;
    Sint16 state;
    task* tp;
    struct al_entry_work* pCommu;
    struct al_entry_work* pLockOn;
} al_entry_work;

#define ALW_ENTRY_WORK(tp) ((al_entry_work*)tp->EntityData2)

extern const Uint32 nbMaxEntry[];

task* ALW_GetTask(Uint16 category, Uint16 num);
al_entry_work* ALW_IsCommunication(task* tp);
int ALW_TurnToLockOn(task* pMyTask, int RotSpd);
float ALW_CalcDistFromLockOn(task* tp);
Bool ALW_Entry(Uint16 category, task* tp, Uint16 kind);
task* ALW_GetTaskCount(Uint16 category, Uint16 count);
int ALW_CountEntry(Uint16 category);
#endif