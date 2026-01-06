#ifndef _AL_PERCEPTION_H_
#define _AL_PERCEPTION_H_

#include <task.h>

typedef struct al_perception_link {
    Sint16 info[4];
    float dist;
    int InSightFlag;
    int HearFlag;
    int SmellFlag;
    struct al_entry_work* pEntry;
} al_perception_link;

typedef struct AL_PERCEPTION_INFO {
    Sint16 nbPerception;
    int InSightFlag;
    int HeardFlag;
    int SmellFlag;
    float NearestDist;
    Sint16 NearestNum;

    al_perception_link list[16];
} AL_PERCEPTION_INFO;

typedef struct AL_PERCEPTION {
    float SightRange;
    int SightAngle;
    int SightAngleHalf;
    float HearRange;
    float SmellRange;
    AL_PERCEPTION_INFO Player;
    AL_PERCEPTION_INFO Chao;
    AL_PERCEPTION_INFO Fruit;
    AL_PERCEPTION_INFO Tree;
    AL_PERCEPTION_INFO Toy;
    AL_PERCEPTION_INFO Sound;
} AL_PERCEPTION;

task* AL_GetFoundPlayerTask(task* tp);
task* AL_GetFoundFruitTask(task* tp);
task* AL_GetFoundToyTask(task* tp);

al_perception_link* AL_GetFoundTree(task *tp);

#endif