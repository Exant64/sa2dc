#ifndef _AL_INTENTION_H_
#define _AL_INTENTION_H_

#include <task.h>

typedef enum eINTENTION {
    INTENTION_NONE = 0x0,
    INTENTION_THINK = 0x1,
    INTENTION_AKUBI = 0x2,
    INTENTION_SLEEP = 0x3,
    INTENTION_GOO = 0x4,
    INTENTION_GET_FRUIT = 0x5,
    INTENTION_EAT = 0x6,
    INTENTION_HIMA = 0x7,
    INTENTION_PLAY = 0x8,
    INTENTION_WALKING = 0x9,
    INTENTION_MAYU = 0xA,
    INTENTION_NEST = 0xB,
    INTENTION_DEAD = 0xC,
    INTENTION_MUSIC = 0xD,
    INTENTION_ART = 0xE,
    INTENTION_STOY = 0xF,
    INTENTION_LTOY = 0x10,
    INTENTION_JOY = 0x11,
    NB_INTENTION = 0x12,
} eINTENTION;

void AL_SetIntention(task* tp, Uint16 intention);
void AL_ScoreRandomise(float*);
void AL_ClearIntention(task*);
float AL_CalcMoodScoreTypeA(int value, int trigger);
float AL_CalcScoreTypeA(int value, int trigger);
Uint16 AL_GetIntention(task* tp);
void AL_DecideBehavior(task* tp, eINTENTION intention);
void AL_DecideIntention(task* tp);
void AL_IntentionCheckInterrupt(task* tp);

#endif