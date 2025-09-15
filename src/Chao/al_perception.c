#include <task.h>
#include <Chao/Chao.h>

void AL_ClearPerceptionAll(task* tp);

void AL_PerceptionInit(task* tp) {
    AL_PERCEPTION* pPerception = &GET_CHAOWK(tp)->Perception;

    AL_ClearPerceptionAll(tp);
    pPerception->SightRange = 50;
    pPerception->SightAngle = 0x4E38;
    pPerception->SightAngleHalf = pPerception->SightAngle >> 1;
    pPerception->HearRange = 40;
    pPerception->SmellRange = 40;
}

void AL_ClearPerceptionLink(al_perception_link* link) {
    link->info[0] = -1;
    link->info[1] = -1;
    link->info[2] = -1;
    link->info[3] = -1;

    link->dist = 100000;

    link->InSightFlag = 0;
    link->HearFlag = 0;
    link->SmellFlag = 0;
    link->pEntry = 0;
}

void AL_ClearPerceptionInfo(AL_PERCEPTION_INFO* pInfo) {
    al_perception_link *i, *count;

    pInfo->nbPerception = 0;
    pInfo->InSightFlag = 0;
    pInfo->HeardFlag = 0;
    pInfo->SmellFlag = 0;

    pInfo->NearestDist = 100000;
    pInfo->NearestNum = -1;

    i = pInfo->list;
    count = &pInfo->list[16];
    for (; i < count; i++) {
        AL_ClearPerceptionLink(i);
    }
}

void AL_ClearPerceptionAll(task* tp) {
    AL_PERCEPTION* pPerception = &GET_CHAOWK(tp)->Perception;

    AL_PERCEPTION_INFO* i = &pPerception->Player;
    AL_PERCEPTION_INFO* count = &pPerception->Sound;
    for (; i <= count; i++) {
        AL_ClearPerceptionInfo(i);
    }
}

extern task* playertp[];
Uint16 DiffAngle(int a, int b);
int AL_SearchPlayer(task* tp) {
    chaowk* cwk = GET_CHAOWK(tp); // 92
    NJS_POINT3* pos = &cwk->pos;

    int ang = cwk->ang.y;

    AL_PERCEPTION* pPerception = &GET_CHAOWK(tp)->Perception;

    AL_PERCEPTION_INFO* pInfo = &pPerception->Player;
    al_perception_link* pLink = pInfo->list;

    float SightRange = pPerception->SightRange;

    Uint32 SightAngle = pPerception->SightAngle;
    int SightAngleHalf = pPerception->SightAngleHalf;

    AL_ClearPerceptionInfo(pInfo);

    {
        task* player = playertp[0];
        bool flag;
        flag = 0;
        if (player) {
            taskwk* wk = player->twp;
            if (wk) {
                NJS_VECTOR* playerPos = &wk->pos;

                float dist = njDistanceP2P(pos, playerPos);
                if (dist < SightRange) {
                    Angle f = njArcTan2(playerPos->x - pos->x, playerPos->z - pos->z);
                    Uint16 diff = DiffAngle(f, ang);
                    if (diff < SightAngleHalf) {
                        pInfo->InSightFlag = 1;
                        pLink->InSightFlag = 1;
                        flag = pLink->InSightFlag;

                        pLink->dist = dist;
                        pLink->pEntry = 0;
                    }
                }

                if (flag) {
                    if (pInfo->NearestDist > dist) {
                        pInfo->NearestDist = dist;
                        pInfo->NearestNum = 0;
                    }

                    pInfo->nbPerception++;
                }
            }
        }
    }
}

int AL_SearchChao(task* tp) {
    int count;
    chaowk* cwk = GET_CHAOWK(tp); // 92
    NJS_POINT3* pos = &cwk->pos;
    
    int ang = cwk->ang.y;

    AL_PERCEPTION* pPerception = &GET_CHAOWK(tp)->Perception;

    AL_PERCEPTION_INFO* pInfo = &pPerception->Chao;
    al_perception_link* pLink = pInfo->list;

    float SightRange = pPerception->SightRange;

    int SightAngleHalf = pPerception->SightAngleHalf;
    
    float HearRange = pPerception->HearRange;
    float SmellRange = pPerception->SmellRange;
    
    Sint16 i = 0;

    AL_ClearPerceptionInfo(pInfo);
    count = ALW_CountEntry(0);

    while (count > 0) {
        int flag = 0;
        task* player;

        --count;
        player = ALW_GetTaskCount(0, count);

        if (player && player != tp) {
            taskwk* wk = player->twp;

            if (wk) {
                NJS_VECTOR* playerPos = &wk->pos;
                float dist = njDistanceP2P(pos, playerPos);

                if (dist < SightRange) {
                    Angle f = njArcTan2(playerPos->x - pos->x, playerPos->z - pos->z);
                    int diff = DiffAngle(f, ang);

                    if (diff < SightAngleHalf) {
                        pInfo->InSightFlag = 1;
                        pLink->InSightFlag = 1;
                        flag = 1;

                        pLink->dist = dist;
                        pLink->pEntry = player->EntityData2;
                    }
                }

                if (flag) {
                    if (pInfo->NearestDist > dist) {
                        pInfo->NearestDist = dist;
                        pInfo->NearestNum = i;
                    }

                    pInfo->nbPerception++;
                    pLink++;
                    i++;
                }
            }
        }
    }
}

int AL_SearchFruit(task* tp) {
    int count;
    chaowk* cwk = GET_CHAOWK(tp); // 92
    NJS_POINT3* pos = &cwk->pos;
    
    int ang = cwk->ang.y;

    AL_PERCEPTION* pPerception = &GET_CHAOWK(tp)->Perception;

    AL_PERCEPTION_INFO* pInfo = &pPerception->Fruit;
    al_perception_link* pLink = pInfo->list;

    float SightRange = pPerception->SightRange;

    int SightAngleHalf = pPerception->SightAngleHalf;
    
    float HearRange = pPerception->HearRange;
    float SmellRange = pPerception->SmellRange;
    
    Sint16 i = 0;

    AL_ClearPerceptionInfo(pInfo);
    count = ALW_CountEntry(3);

    while (count > 0) {
        int flag = 0;
        task* player;

        --count;
        player = ALW_GetTaskCount(3, count);

        if (player) {
            taskwk* wk = player->twp;

            if (wk) {
                NJS_VECTOR* playerPos = &wk->pos;
                float dist = njDistanceP2P(pos, playerPos);

                if (dist < SightRange) {
                    Angle f = njArcTan2(playerPos->x - pos->x, playerPos->z - pos->z);
                    int diff = DiffAngle(f, ang);

                    if (diff < SightAngleHalf) {
                        pInfo->InSightFlag = 1;
                        pLink->InSightFlag = 1;
                        flag = 1;

                        pLink->dist = dist;
                        pLink->pEntry = player->EntityData2;
                    }
                }

                if (flag) {
                    if (pInfo->NearestDist > dist) {
                        pInfo->NearestDist = dist;
                        pInfo->NearestNum = i;
                    }

                    pInfo->nbPerception++;
                    pLink++;
                    i++;
                }
            }
        }
    }
}
int AL_SearchTree(task* tp) {
    int count;
    chaowk* cwk = GET_CHAOWK(tp); // 92
    
    NJS_POINT3* pos = &cwk->pos;
    int ang = cwk->ang.y;

    AL_PERCEPTION* pPerception = &GET_CHAOWK(tp)->Perception;

    AL_PERCEPTION_INFO* pInfo = &pPerception->Tree;
    al_perception_link* pLink = pInfo->list;

    float SightRange = pPerception->SightRange;

    int SightAngleHalf = pPerception->SightAngleHalf;
    
    float HearRange = pPerception->HearRange;
    float SmellRange = pPerception->SmellRange;
    
    Sint16 i = 0;

    AL_ClearPerceptionInfo(pInfo);

    count = ALW_CountEntry(4);
    while (count > 0) {
        int flag = 0;
        task* player;

        --count;
        player = ALW_GetTaskCount(4, count);

        if (player) {
            taskwk* wk = player->twp;

            if (wk) {
                NJS_POINT3* playerPos = &wk->pos;
                float dist = njDistanceP2P(pos, playerPos);
    
                if (dist >= SightRange) {} else {
                    const Angle f = njArcTan2(playerPos->x - pos->x, playerPos->z - pos->z);
                    int diff = DiffAngle(f, ang);
                    
                    if (diff < SightAngleHalf) {
                        pInfo->InSightFlag = 1;
                        pLink->InSightFlag = 1;
                        flag = 1;
            
                        pLink->dist = dist;
                        pLink->pEntry = player->EntityData2;
                    }
                }

                if (flag) {
                    if (pInfo->NearestDist > dist) {
                        pInfo->NearestDist = dist;
                        pInfo->NearestNum = i;
                    }

                    pInfo->nbPerception++;
                    pLink++;
                    i++;
                }
            }
        }
    }

    count = ALW_CountEntry(5);
    while (count > 0) {
        int flag = 0;
        task* player;

        --count;
        player = ALW_GetTaskCount(5, count);

        if (player) {
            taskwk* wk = player->twp;

            if (wk) {
                NJS_POINT3* playerPos = &wk->pos;
                float dist = njDistanceP2P(pos, playerPos);
    
                if (dist >= SightRange) {} else {
                    const Angle f = njArcTan2(playerPos->x - pos->x, playerPos->z - pos->z);
                    int diff = DiffAngle(f, ang);
                    
                    if (diff < SightAngleHalf) {
                        pInfo->InSightFlag = 1;
                        pLink->InSightFlag = 1;
                        flag = 1;
            
                        pLink->dist = dist;
                        pLink->pEntry = player->EntityData2;
                    }
                }

                if (flag) {
                    if (pInfo->NearestDist > dist) {
                        pInfo->NearestDist = dist;
                        pInfo->NearestNum = i;
                    }

                    pInfo->nbPerception++;
                    pLink++;
                    i++;
                }
            }
        }
    }
}

int AL_SearchToy(task* tp) {
    int count;
    chaowk* cwk = GET_CHAOWK(tp); // 92
    NJS_POINT3* pos = &cwk->pos;
    
    int ang = cwk->ang.y;

    AL_PERCEPTION* pPerception = &GET_CHAOWK(tp)->Perception;

    AL_PERCEPTION_INFO* pInfo = &pPerception->Toy;
    al_perception_link* pLink = pInfo->list;

    float SightRange = pPerception->SightRange;

    int SightAngleHalf = pPerception->SightAngleHalf;
    
    float HearRange = pPerception->HearRange;
    float SmellRange = pPerception->SmellRange;
    
    Sint16 i = 0;

    AL_ClearPerceptionInfo(pInfo);
    count = ALW_CountEntry(6);

    while (count > 0) {
        int flag = 0;
        task* player;

        --count;
        player = ALW_GetTaskCount(6, count);

        if (player) {
            taskwk* wk = player->twp;

            if (wk) {
                NJS_VECTOR* playerPos = &wk->pos;
                float dist = njDistanceP2P(pos, playerPos);

                if (dist < SightRange) {
                    Angle f = njArcTan2(playerPos->x - pos->x, playerPos->z - pos->z);
                    int diff = DiffAngle(f, ang);

                    if (diff < SightAngleHalf) {
                        pInfo->InSightFlag = 1;
                        pLink->InSightFlag = 1;
                        flag = 1;

                        pLink->dist = dist;
                        pLink->pEntry = player->EntityData2;
                    }
                }

                if (flag) {
                    if (pInfo->NearestDist > dist) {
                        pInfo->NearestDist = dist;
                        pInfo->NearestNum = i;
                    }

                    pInfo->nbPerception++;
                    pLink++;
                    i++;
                }
            }
        }
    }
}

int AL_SearchSound(task* tp) {}

int AL_CalcPerceptionAll(task* tp) {
    int val = 0;
    
    AL_ClearPerceptionAll(tp);

    val |= AL_SearchPlayer(tp);
    val |= AL_SearchChao(tp);
    val |= AL_SearchFruit(tp);
    val |= AL_SearchTree(tp);
    val |= AL_SearchToy(tp);

    return val;
}

al_perception_link* AL_GetFoundPlayer(task *tp) {
    AL_PERCEPTION* pPerception = &GET_CHAOWK(tp)->Perception;
    AL_PERCEPTION_INFO* pInfo = &pPerception->Player;
    al_perception_link* pFound = NULL;

    if (pInfo->InSightFlag) {
        Sint16 nearestNum = pInfo->NearestNum;
        if (nearestNum >= 0) {
            pFound = &pInfo->list[nearestNum];
        }
    }

    return pFound;
}

task* AL_GetFoundPlayerTask(task* tp) {
    al_perception_link* pFound = AL_GetFoundPlayer(tp);
    if(pFound) return playertp[0];

    return NULL;
}

al_perception_link* AL_GetFoundFruit(task *tp) {
    AL_PERCEPTION* pPerception = &GET_CHAOWK(tp)->Perception;
    AL_PERCEPTION_INFO* pInfo = &pPerception->Fruit;
    al_perception_link* pFound = NULL;

    if (pInfo->InSightFlag) {
        Sint16 nearestNum = pInfo->NearestNum;
        if (nearestNum >= 0) {
            pFound = &pInfo->list[nearestNum];
        }
    }

    return pFound;
}

task* AL_GetFoundFruitTask(task* tp) {
    al_perception_link* pFound = AL_GetFoundFruit(tp);
    al_entry_work* pEntry;
    if(!pFound) return NULL;

    pEntry = pFound->pEntry;
    if(!pEntry) return NULL;

    if(pEntry->tp) return pEntry->tp;

    return NULL;
}

al_perception_link* AL_GetFoundTree(task *tp) {
    AL_PERCEPTION* pPerception = &GET_CHAOWK(tp)->Perception;
    AL_PERCEPTION_INFO* pInfo = &pPerception->Tree;
    al_perception_link* pFound = NULL;

    if (pInfo->InSightFlag) {
        Sint16 nearestNum = pInfo->NearestNum;
        if (nearestNum >= 0) {
            pFound = &pInfo->list[nearestNum];
        }
    }

    return pFound;
}

task* AL_GetFoundTreeTask(task* tp) {
    al_perception_link* pFound = AL_GetFoundTree(tp);
    al_entry_work* pEntry;
    if(!pFound) return NULL;

    pEntry = pFound->pEntry;
    if(!pEntry) return NULL;

    if(pEntry->tp) return pEntry->tp;

    return NULL;
}

al_perception_link* AL_GetFoundToy(task *tp) {
    AL_PERCEPTION* pPerception = &GET_CHAOWK(tp)->Perception;
    AL_PERCEPTION_INFO* pInfo = &pPerception->Toy;
    al_perception_link* pFound = NULL;

    if (pInfo->InSightFlag) {
        Sint16 nearestNum = pInfo->NearestNum;
        if (nearestNum >= 0) {
            pFound = &pInfo->list[nearestNum];
        }
    }

    return pFound;
}

task* AL_GetFoundToyTask(task* tp) {
    al_perception_link* pFound = AL_GetFoundToy(tp);
    al_entry_work* pEntry;
    if(!pFound) return NULL;

    pEntry = pFound->pEntry;
    if(!pEntry) return NULL;

    if(pEntry->tp) return pEntry->tp;

    return NULL;
}
