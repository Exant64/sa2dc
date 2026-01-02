#include <task.h>
#include <player.h>
#include <Chao/Chao.h>
#include <Chao/al_garden_info.h>
#include <Chao/al_chao_info.h>
#include <sg_syrtc.h>

extern NJS_POINT3 ChaoSetPosition[3][16];
extern chao_info* pHoldingChaoSaveInfo;
extern int HoldingFront;

int DiffAngle(int a, int b);
int ALBHV_Sleep(task *tp);

task* CreateChaoEgg(AL_GENE* pGene, CHAO_PARAM* pParam, NJS_POINT3* pos, int, chao_info* pInfo);
task* CreateEgg(AL_GENE* pGene, NJS_POINT3* pos, int, chao_info* pInfo);
task* lbl_0C5029F0(chao_info* pInfo, NJS_POINT3* pos, int ang);

static chao_info* AL_GetHoldingChaoInfo();

Uint32 AL_InitChaoID(void) {
    SYS_RTC_DATE date;
    int retval = syRtcGetDate(&date);

    if (retval == SYD_RTC_ERR_OK) {
        Uint32 id = 0;

        id |= (date.second & 0x3F);
        id |= (date.minute & 0x3F) << 6;
        id |= (date.hour & 0x1F) << 12;
        id |= (date.day & 0x1F) << 17;
        id |= (date.month & 0xF) << 22;
        id |= ((date.year - 1950) & 0x3F) << 26; // 1950 is the minimum year it can store

        return id;
    } else {
        Uint32 id = 0;

        id |= ((Uint8)(njRandom() * 100));
        id |= ((Uint8)(njRandom() * 100)) << 8;
        id |= ((Uint8)(njRandom() * 100)) << 16;
        id |= ((Uint8)(njRandom() * 100)) << 24;

        return id;
    }
}

void AL_SetChaoID(task* tp) {
    CHAO_ID* pID = &GET_CHAOWK(tp)->gene.ChaoID;
    GARDEN_ID* gid = AL_GetGardenID();

    pID->gid = *gid;
    pID->id = AL_InitChaoID();
}

void AL_InitChaoSaveInfo(void) {
    int i;

    memset(ChaoInfo, 0, sizeof(ChaoInfo));
    
    for (i = 0; i < 2; i++) {
        int index = i + 0;
        CHAO_PARAM* pParam = &((chao_info_*)&ChaoInfo[index])->param;
            
        pParam->type = TYPE_EGG;
        pParam->place = CHAO_STG_NEUT;

        {
            AL_GENE* pGene = &((chao_info_*)&ChaoInfo[index])->gene_pad.gene;
            AL_GeneCreate(pGene);
        }
    }

    for(i = 0; i < 1; i++) {
        int index = i + 2;
        CHAO_PARAM* pParam = &((chao_info_*)&ChaoInfo[index])->param;
        
        pParam->type = TYPE_EGG;
        pParam->place = CHAO_STG_HERO;

        {
            AL_GENE* pGene = &((chao_info_*)&ChaoInfo[index])->gene_pad.gene;
            lbl_0C50BD26(pGene);
        }
    }

    for(i = 0; i < 1; i++) {
        int index = i + 3;
        CHAO_PARAM* pParam = &((chao_info_*)&ChaoInfo[index])->param;
        
        pParam->type = TYPE_EGG;
        pParam->place = CHAO_STG_DARK;
        
        {
            AL_GENE* pGene = &((chao_info_*)&ChaoInfo[index])->gene_pad.gene;
            lbl_0C50BD40(pGene);
        }
    }
}

void AL_ClearChaoParam(chao_info* pParam) {
    if(pParam) memset(pParam, 0, sizeof(*pParam));
}

void sub_C5025E0(task* tp, chao_info* pParamAgain) {
    if(tp && pParamAgain) {
        chaowk * work = GET_CHAOWK(tp);
        
        memset(pParamAgain, 0, sizeof(*pParamAgain));

        {
            CHAO_PARAM* pParam = &((chao_info_*)pParamAgain)->param;
            AL_GENE* pGene = &((chao_info_*)pParamAgain)->gene_pad.gene;
            
            *pParam = work->param;
            *pGene = work->gene;
        }
    }
}

void sub_C502632(task* tp, chao_info* pParamAgain) {
    if(tp && pParamAgain) {
        chaowk * work = GET_CHAOWK(tp);
        
        memset(pParamAgain, 0, sizeof(*pParamAgain));

        {
            CHAO_PARAM* pParam = &((chao_info_*)pParamAgain)->param;
            AL_GENE* pGene = &((chao_info_*)pParamAgain)->gene_pad.gene;
            
            *pParam = work->param;
            *pGene = work->gene;
        }
    }
}

int AL_GetLocalChaoCount(int garden) {
    int maxChao = AL_GetMaxChao();
    int count = 0;
    int i;
    for(i = 0; i < maxChao; i++) {
        CHAO_PARAM* pParam = &((chao_info_*)&ChaoInfo[i])->param;
        if(pParam->type && pParam->place == garden) {
            count++;
        }
    }
    return count;
}

int AL_GetCurrGardenChaoCount() {
    return AL_GetLocalChaoCount(AL_GetStageNumber());
}

int AL_GetAllChaoCount() {
    int maxChao; 
    int i;
    int count;

    // maybe some hack to test something?
    if(0) {return 0;}
    
    maxChao = AL_GetMaxChao();
    count = 0;
    
    for(i = 0; i < maxChao; i++) {
        CHAO_PARAM* pParam = &((chao_info_*)&ChaoInfo[i])->param;
        if(pParam->type) {
            count++;
        }
    }
    
    return count;
}

chao_info* AL_GetNewChaoSaveInfo() {
    int maxChao; 
    int i;
    
    maxChao = AL_GetMaxChao();
    
    for(i = 0; i < maxChao; i++) {
        CHAO_PARAM* pParam = &((chao_info_*)&ChaoInfo[i])->param;
        if(!pParam->type) {
            return &ChaoInfo[i];
        }
    }
    
    return NULL;
}

void AL_SetChaoOnTheGarden(void) {
    int stage = AL_GetStageNumber();
    int maxChao = AL_GetMaxChao();

    chao_info* pInfo = ChaoInfo;
    chao_info* pInfoEnd = &ChaoInfo[maxChao];
    
    int flag = 0;
    int g2 = 0;
    int index = 0;
    int count = 0;
    
    if(stage >= CHAO_STG_NEUT && stage <= CHAO_STG_DARK) {
        chao_info* pHolding = AL_GetHoldingChaoInfo();
        if(pHolding && pHolding->param.place != stage && AL_GetLocalChaoCount(stage) >= 8) {
            int garden1;
            int garden2;
            int chaoCount;
            
            flag = 1;

            switch(stage) {
                case CHAO_STG_NEUT:
                    garden1 = 2;
                    garden2 = 3;
                    break;
                case CHAO_STG_HERO:
                    garden1 = 1;
                    garden2 = 3;
                    break;
                case CHAO_STG_DARK:
                    garden1 = 1;
                    garden2 = 2;
                    break;
            }

            index = (Uint32)(njRandom() * 7.9999f);

            chaoCount = AL_GetLocalChaoCount(garden1);
            if(chaoCount < 8) g2 = garden1;

            chaoCount = AL_GetLocalChaoCount(garden2);
            if(chaoCount < 8) {
                if(g2) {
                    if(njRandom() < 0.5f){
                        g2 = garden2;
                    }
                }
                else {
                    g2 = garden2;
                }
            }

            if(!g2) {
                g2 = pHolding->param.place;
            }
        }

        while(pInfo < pInfoEnd) {
            CHAO_PARAM* pParam = GET_INFO_PARAM(pInfo);
            
            if(pParam->place == stage) {
                if(pInfo != AL_GetHoldingChaoInfo()) {
                    Sint8 classNum = GET_INFO_PARAM(pInfo)->ClassNum;
                    Uint32 posIndex = (Uint32)(njRandom() * 15.9f);
                    NJS_POINT3* pos = &ChaoSetPosition[stage - 1][posIndex];
                    task* pChao = 0;
    
                    if(flag && count == index) {
                        pParam->place = g2;
                        count++;
                    }
                    else {
                        count++;
                        
                        switch(pParam->type) {
                            case 0:
                                break;
                            case 1:
                                lbl_0C5029F0(pInfo, pos, (int)(njRandom() * 65536.f));
                                break;
                            default:
                                if(classNum == -1)
                                pChao = lbl_0C5029F0(pInfo, pos, (int)(njRandom() * 65536.f));
                            break;
                        }
    
                        if(pChao) {
                            if(AL_EmotionGetValue(pChao, 9) > 0) {
                                AL_SetBehavior(pChao, ALBHV_Sleep);
                            }
                        }
                    }
                }
            }

            pInfo++;
        }
    }
}

task* lbl_0C5029F0(chao_info* pInfo, NJS_POINT3* pos, int ang) {
    CHAO_PARAM* pParam = &((chao_info_*)pInfo)->param;
    AL_GENE* pGene = &((chao_info_*)pInfo)->gene_pad.gene;
    task* pChao;
    
    switch(pParam->type) {
        case 0:
            pChao = NULL;
            break;
        case 1:
            if(pParam->unk_2) {
                pChao = CreateChaoEgg(pGene, pParam, pos, 0, pInfo);
            }
            else {
                pChao = CreateEgg(pGene, pos, 0, pInfo);
            }
            break;
        default:
            pChao = CreateChao(pGene, pParam, NULL, pos, ang, pInfo);
            break;
    }

    return pChao;
}

static chao_info* AL_GetHoldingChaoInfo() {
    return pHoldingChaoSaveInfo;
}

static void AL_ResetHoldingChaoSaveInfo() {
    pHoldingChaoSaveInfo = NULL;
}

task* AL_CreateHoldingChao() {
    int playerAng = 0x4000 - playertwp[0]->ang.y;
    chao_info* pHeldChao;
    
    StopHoldingTaskP(0);
    playerpwp[0]->HeldObject = NULL;
    
    pHeldChao = AL_GetHoldingChaoInfo();
    if(pHeldChao) {
        NJS_POINT3 DummyPos;
        task* pChao;
        
        DummyPos = playertwp[0]->pos;
        DummyPos.x += njSin(playerAng) * 3.f;
        DummyPos.z += njCos(playerAng) * 3.f;

        pChao = lbl_0C5029F0(pHeldChao, &DummyPos, 0);
        
        if(HoldingFront) {
            GET_CHAOWK(pChao)->ang.y = playerAng;
        }
        else {
            GET_CHAOWK(pChao)->ang.y = playerAng + 0x8000;
        }

        switch(AL_GetStageNumber()) {
            case 7:
            case 6:
                if(pHeldChao->param.type != 1) {
                    GET_CHAOWK(pChao)->ChaoFlag |= CHAO_FLAG_RACE_BHV_ENABLE;
                    GET_CHAOWK(pChao)->ChaoFlag &= ~CHAO_FLAG_UNK_17;
                }
                break;
        }

        AL_ResetHoldingChaoSaveInfo();
        HoldTaskP(0, pChao);
        
        return pChao;
    }
    
    return NULL;
}

void AL_SetHoldingChaoInfo(task *pChaoTask) {
    if(pChaoTask) {
        pHoldingChaoSaveInfo = GET_CHAOWK(pChaoTask)->pInfo;
        if(DiffAngle(MOV_CalcPlayerAngle(pChaoTask, 0), GET_CHAOWK(pChaoTask)->ang.y) < 0x4000) {
            HoldingFront = 1;
        }
        else {
            HoldingFront = 0;
        }
    }
    else {
        AL_ResetHoldingChaoSaveInfo();
    }
}

void AL_SetHoldingEggInfo(task *pChaoTask) {
    pHoldingChaoSaveInfo = GET_CHAOWK(pChaoTask)->pInfo;
}