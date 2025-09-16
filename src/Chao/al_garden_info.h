#ifndef _AL_GARDEN_INFO_H_
#define _AL_GARDEN_INFO_H_

#include "Chao/al_race_info.h"
#include "Chao/al_stage.h"

typedef struct TREE_SAVE_INFO {
    char kind;
    char state;
    char param;
    char water;
    Uint8 FruitGrowth[3];
    char angy_pos;
} TREE_SAVE_INFO;

typedef struct KINDER_SAVE_INFO {
    char space[0x34];
} KINDER_SAVE_INFO;

typedef struct CHAO_GARDEN_INFO {
    GARDEN_ID GardenID;
    int MaxChao;
    int flag;
    int ToyOnFlag;
    int pad;
    TREE_SAVE_INFO tree[3][8];
    race_save_info race;
    KINDER_SAVE_INFO kinder;
} CHAO_GARDEN_INFO;

typedef struct OsanpoThing {
    int a1;
    int a2;
    int a3;
    void* a4;
    int a5;
} OsanpoThing;

// what the fuck
// theres like 5 separate versions of "storing chao"
// i see why they rewrote the saves completely on battle lol
// hopefully this will be all sorted out when we get to al_chao_info.c

typedef struct CHAO_SAVE_GARDEN {
    CHAO_GARDEN_INFO garden;
    char pad[0x48];
} CHAO_SAVE_GARDEN;

typedef struct CHAO_SAVE_CHAO {
    chao_info chao[24];
    char pad2[0x240]; // i wonder if this is done because of the VMU block system?
} CHAO_SAVE_CHAO;

typedef struct CHAO_SAVE {
    CHAO_SAVE_GARDEN garden;
    CHAO_SAVE_CHAO chao;
} CHAO_SAVE;

extern chao_info ChaoInfo[24];

void AL_PackageAllSaveInfo();
int sub_C50B7A0();
int sub_C519920(OsanpoThing*);

#endif