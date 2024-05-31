#ifndef _AL_BEHAVIOR_H_
#define _AL_BEHAVIOR_H_

#include <task.h>

enum {
    BHV_RET_CONTINUE = 0x0,
    BHV_RET_FINISH = 0x1,
    BHV_RET_THINK = 0x2,
    BHV_RET_BREAK = 0x3,
};

typedef enum eKW_BHV_KIND {
    KW_BHV_ART = 0x0,
    KW_BHV_DANCE = 0x1,
    KW_BHV_SING = 0x2,
    KW_BHV_MUSIC = 0x3,
    KW_BHV_MINI1 = 0x4,
    KW_BHV_MINI2 = 0x5,
    KW_BHV_MINI3 = 0x6,
    KW_BHV_MINI4 = 0x7,
    KW_BHV_MINI5 = 0x8,
    KW_BHV_MINI6 = 0x9,
    KW_BHV_MINI7 = 0xA,
    KW_BHV_MINI8 = 0xB,
    KW_BHV_MINI9 = 0xC,
    KW_BHV_MINI10 = 0xD,
    KW_BHV_MINI11 = 0xE,
    KW_BHV_MINI12 = 0xF,
    KW_BHV_MINI13 = 0x10,
    KW_BHV_MINI14 = 0x11,
    KW_BHV_MINI15 = 0x12,
    KW_BHV_MINI16 = 0x13,
    KW_BHV_MINI17 = 0x14,
    KW_BHV_MINI18 = 0x15,
    KW_BHV_TOY1 = 0x16,
    KW_BHV_TOY2 = 0x17,
    KW_BHV_TOY3 = 0x18,
    KW_BHV_TOY4 = 0x19,
    KW_BHV_TOY5 = 0x1A,
    KW_BHV_TOY6 = 0x1B,
    KW_BHV_TOY7 = 0x1C,
    KW_BHV_TOY8 = 0x1D,
    KW_BHV_TOY9 = 0x1E,
    KW_BHV_TOY10 = 0x1F,
    KW_BHV_TOY11 = 0x20,
    KW_BHV_FLY = 0x21,
    KW_BHV_SWIM = 0x22,
    KW_BHV_CLIMB_TREE = 0x23,
    KW_BHV_CLIMB_WALL = 0x24,
    KW_BHV_WATER = 0x25,
    KW_BHV_SWING = 0x26,
    KW_BHV_SIT = 0x27,
    KW_BHV_DENGURI = 0x28,
    KW_BHV_TOILET = 0x29,
    KW_BHV_PYON = 0x2A,
    KW_BHV_BOWLING = 0x2B,
    KW_BHV_FUKKIN = 0x2C,
    KW_BHV_SHIRIFURI = 0x2D,
    KW_BHV_OJIGI = 0x2E,
    KW_BHV_CHIWA = 0x2F,
    KW_BHV_NADERU = 0x30,
    KW_BHV_AGERU = 0x31,
    KW_BHV_TALK = 0x32,
    KW_BHV_PUNCH = 0x33,
    KW_BHV_OKOSU = 0x34,
    KW_BHV_TEFURI = 0x35,
    KW_BHV_HAKUSYU = 0x36,
    KW_BHV_SURIYORU = 0x37,
    KW_BHV_AKANBE = 0x38,
    KW_BHV_WA = 0x39,
    KW_BHV_NAGERU = 0x3A,
    KW_BHV_FIGHT = 0x3B,
    KW_BHV_IGAMI = 0x3C,
    KW_BHV_LISTEN = 0x3D,
    KW_BHV_WATCH = 0x3E
} eKW_BHV_KIND;

typedef int (*BHV_FUNC)(task*);

int ALBHV_Think(task* tp);
void AL_BehaviorSetFreeWork(task* tp, int info);
BHV_FUNC AL_GetBehavior(task* tp);
void AL_SetNextBehavior(task* tp, BHV_FUNC Func);
void AL_SetNextBehaviorWithTimer(task* tp, BHV_FUNC Func, int ReserveTimer);
void AL_SetBehaviorWithTimer(task* tp, BHV_FUNC Func, int ReserveTimer);
void AL_SetBehavior(task* tp, BHV_FUNC Func);

#endif