#ifndef _AL_KNOWLEDGE_H_
#define _AL_KNOWLEDGE_H_

#include <task.h>

typedef enum eAL_PLAYER_BTL {
    BTL_AL_PL_SONIC = 0,
    BTL_AL_PL_SHADOW = 1,
    BTL_AL_PL_TAILS = 2,
    BTL_AL_PL_EGGMAN = 3,
    BTL_AL_PL_KNUCKLES = 4,
    BTL_AL_PL_ROUGE = 5,
    NB_BTL_AL_PLAYER,
} eAL_PLAYER_BTL;

typedef enum eAL_ART {
    AL_ART_1 = 0,
    AL_ART_2 = 1,
    AL_ART_3 = 2,
    AL_ART_4 = 3,
    AL_ART_5 = 4,
    NB_AL_ART
} eAL_ART;

typedef enum eAL_DANCE {
    AL_DANCE_FURIRFURI = 0,
    AL_DANCE_KURUKURU = 1,
    AL_DANCE_BOX = 2,
    AL_DANCE_GOGO = 3,
    AL_DANCE_RADIO = 4,
    NB_AL_DANCE
} eAL_DANCE;

typedef enum eAL_STOY {
    AL_STOY_GARAGARA = 0,
    AL_STOY_CAR = 1,
    AL_STOY_BOOK = 2,
    AL_STOY_CRACKER = 3,
    AL_STOY_DOLL = 4,
    AL_STOY_HOUKI = 5,
    AL_STOY_FIREWORK = 6,
    AL_STOY_HOPPING = 7,
    AL_STOY_CRAYON = 8,
    AL_STOY_SYABON = 9,
    AL_STOY_SCOP = 10,
    AL_STOY_JYOURO = 11,
    NB_AL_STOY = 0xC,
} eAL_STOY;


Sint8 AL_KW_GetPlayerLike(task* tp);
Uint8 AL_KW_GetPlayerFear(task* pChaoTask);
Uint16 AL_KW_GetPlayerDistance(task* pChaoTask);
Uint16 AL_KW_GetPlayerMeet(task* pChaoTask);

void AL_KW_MeetPlayer(task* pChaoTask);
void AL_KW_AddPlayerLike(task* pChaoTask, int add);

#endif