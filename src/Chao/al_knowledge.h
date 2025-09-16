#ifndef _AL_KNOWLEDGE_H_
#define _AL_KNOWLEDGE_H_

#include <task.h>

Sint8 AL_KW_GetPlayerLike(task* tp);
Uint8 AL_KW_GetPlayerFear(task* pChaoTask);
void AL_KW_MeetPlayer(task* pChaoTask);
void AL_KW_AddPlayerLike(task* pChaoTask, int add);

#endif