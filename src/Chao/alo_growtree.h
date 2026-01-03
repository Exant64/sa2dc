#ifndef _ALO_GROWTREE_H_
#define _ALO_GROWTREE_H_

#include <task.h>

enum eTREE_STATE {
    TREE_ST_PLANT = 0,
    TREE_ST_SEED = 1,
    TREE_ST_SAPLING = 2,
    TREE_ST_ADULT = 3,
    TREE_ST_DEAD = 4,
    TREE_ST_LOCAL = 5,
};

#define FRUIT_COUNT 3

typedef struct TREE_SAVE_INFO {
    Uint8 kind;
    Uint8 state;
    Uint8 param;
    Uint8 water;
    char FruitGrowth[FRUIT_COUNT];
    Uint8 angy; // was called angy_pos in real symbols, and used an & 0xF0 to get the rot
} TREE_SAVE_INFO;

typedef struct FRUIT_INFO {
    float growth;
    float AimGrowth;
    int SwingCounter;
    NJS_POINT3 pos;
    int angx;
    int angy;
} FRUIT_INFO;

typedef struct TREE_WORK {
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

    Uint8 kind;
    Uint8 state;
    float growth;
    int life;
    int water;
    //char pos_num;
    FRUIT_INFO fruit[FRUIT_COUNT];
    float scale;
    float ScaleSpd;
    int ThirstyFlag;
    int WaterFlag;
    int LeafRotAng;
    float LeafWidth;
    int LeafDispNum;
    int FruitRotAng;
    float FruitWidth;
    int FruitDispNum;
    int SwingFlag;
    int SwingCountFlag;
    float SwingDist;
    int RollAngle;
    int TouchAngle;
    int ChaoSwingPhase;
    int RollPhase;
    int garden;
    void *pMySaveInfo;
    NJS_TEXLIST *texlist;
    NJS_CNK_OBJECT *pLocalObject;
    NJS_CNK_OBJECT *pShadowObject; //?
    NJS_CNK_OBJECT *pCopyObject;
} TREE_WORK;

#endif