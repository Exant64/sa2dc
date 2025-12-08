#ifndef _ADV1_AL_STATUS_H_
#define _ADV1_AL_STATUS_H_

#include <task.h>

typedef struct ADV1_AL_IMPLESSION {
    char like;
    char meet;
} ADV1_AL_IMPLESSION;

typedef struct ADV1_AL_IMPLESSION2 {
    unsigned int id;
    char like;
    char meet;
    char pad1;
    char pad2;
} ADV1_AL_IMPLESSION2;

typedef struct ADV1_AL_MEMORY {
    ADV1_AL_IMPLESSION player[6];
    ADV1_AL_IMPLESSION2 chao[32];
} ADV1_AL_MEMORY;

typedef struct al_status {
    char type;
    char garden_num;
    char like;
    char key1;
    char name[8];
    Uint16 swim;
    Uint16 fly;
    Uint16 run;
    Uint16 power;
    Uint16 health;
    Uint16 health_max;
    char fruit_kind[8];
    float h_pos;
    float v_pos;
    float growth;
    Uint16 intimate;
    Uint16 life;
    Uint16 old;
    Uint16 get_minimal;
    char win;
    char key2;
    Uint16 flag;
    NJS_POINT3 pos;
    unsigned int age;
    unsigned int id;
    char animal_part_num[7];
    char key3;
    Uint16 pt_swim;
    Uint16 pt_fly;
    Uint16 pt_run;
    Uint16 pt_power;
    char kindness;
    char aggressive;
    char curiosity;
    char charm;
    char breed;
    char sleep;
    char hunger;
    char tedious;
    char tire;
    char stress;
    char narrow;
    char pleasure;
    char anger;
    char sorrow;
    char fear;
    char lonely;
    ADV1_AL_MEMORY memory;
    Uint8 nbSucceed;
    char key4;
    char exist;
    Uint16 mayu_timer;
    char race_time[20];
    Uint8 extra_num;
    char dummy[107];
} al_status;

#endif