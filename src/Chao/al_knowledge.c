#include <Chao/Chao.h>

Uint16 AL_KW_GetPlayerCharacterNum(void) {
    int retval;
    switch (GetCharID(0)) {
        case 0:
            retval = 0;
            break;
        case 1:
            retval = 1;
            break;
        case 2:
            retval = 2;
            break;
        case 3:
            retval = 3;
            break;
        case 4:
            retval = 4;
            break;
        case 5:
            retval = 5;
            break;
        default:
            retval = 0;
            break;
    }
    return retval;
}

Sint8 AL_KW_GetPlayerLike(task* pChaoTask) {
    AL_KNOWLEDGE* kwlg = &GET_CHAOPARAM(pChaoTask)->knowledge;
    Uint16 charNum = AL_KW_GetPlayerCharacterNum();
    return kwlg->player[charNum].like;
}

void AL_KW_AddPlayerLike(task* pChaoTask, int add) {
    AL_KNOWLEDGE* kwlg = &GET_CHAOPARAM(pChaoTask)->knowledge;
    Uint16 charNum = AL_KW_GetPlayerCharacterNum();
    AL_KNOWLEDGE_PLAYER* player;

    int val = kwlg->player[charNum].like;
    val += add;
    if (val > 100)
        val = 100;
    else if (val < -100)
        val = -100;

    player = &kwlg->player[charNum];
    player->like = val;

    if (add > 0) {
        int fear = player->fear;

        fear -= (GET_GLOBAL()->KwSubFear * (AL_EmotionGetValue(pChaoTask, EM_PER_AGRESSIVE) + 300)) / 300;
        fear -= (GET_GLOBAL()->KwSubFear * (AL_EmotionGetValue(pChaoTask, EM_PER_CALM) + 300)) / 300;
        if (fear < 0)
            fear = 0;
        else if (fear > 200)
            fear = 200;

        player->fear = fear;
    }

    AL_KW_MeetPlayer(pChaoTask);
}

Uint8 AL_KW_GetPlayerFear(task* pChaoTask) {
    AL_KNOWLEDGE* kwlg = &GET_CHAOPARAM(pChaoTask)->knowledge;
    Uint16 charNum = AL_KW_GetPlayerCharacterNum();
    return kwlg->player[charNum].fear;
}

void AL_KW_AddPlayerFear(task* pChaoTask, int add) {
    AL_KNOWLEDGE* kwlg = &GET_CHAOPARAM(pChaoTask)->knowledge;
    Uint16 charNum = AL_KW_GetPlayerCharacterNum();
    AL_KNOWLEDGE_PLAYER* player = &kwlg->player[charNum];
    int fear = player->fear;

    fear += add * (300 - AL_EmotionGetValue(pChaoTask, EM_PER_AGRESSIVE)) / 300;
    fear += add * (300 - AL_EmotionGetValue(pChaoTask, EM_PER_CALM)) / 300;
    if (fear < 0)
        fear = 0;
    else if (fear > 200)
        fear = 200;

    player->fear = fear;
}

Uint16 AL_KW_GetPlayerDistance(task* pChaoTask) {
    AL_KNOWLEDGE* kwlg = &GET_CHAOPARAM(pChaoTask)->knowledge;
    Uint16 charNum = AL_KW_GetPlayerCharacterNum();
    return kwlg->player[charNum].distance;
}

Uint16 AL_KW_GetPlayerMeet(task* pChaoTask) {
    AL_KNOWLEDGE* kwlg = &GET_CHAOPARAM(pChaoTask)->knowledge;
    Uint16 charNum = AL_KW_GetPlayerCharacterNum();
    return kwlg->player[charNum].meet;
}

void AL_KW_MeetPlayer(task* pChaoTask) {
    AL_KNOWLEDGE* kwlg = &GET_CHAOPARAM(pChaoTask)->knowledge;
    Uint16 charNum = AL_KW_GetPlayerCharacterNum();
    AL_KNOWLEDGE_PLAYER* player = &kwlg->player[charNum];

    if (player->meet < 0xFFFF)
        player->meet++;

    player->distance /= 3;
    player->distance *= 2;
}

void AL_KW_Control(task* pChaoTask) {
    AL_KNOWLEDGE* kwlg = &GET_CHAOPARAM(pChaoTask)->knowledge;
    if (kwlg->KwTimer++ >= GET_GLOBAL()->ChaoTimerKnowledgeInterval) {
        int i;
        kwlg->KwTimer = 0;

        for (i = 0; i < 6; i++) {
            kwlg->player[i].distance += GET_GLOBAL()->KwAddDistance;
            if (kwlg->player[i].distance > 50000)
                kwlg->player[i].distance = 50000;
        }

        for (i = 0; i < 20; i++) {
            kwlg->chao[i].distance += GET_GLOBAL()->KwAddDistance;
            if (kwlg->chao[i].distance > 50000)
                kwlg->chao[i].distance = 50000;
        }
    }
}

extern PDS_PERIPHERAL* per[]; // 8C24F088
extern Uint32 SWDATA[];       // 8C24F14C - 8

#define BUTTON_ON(a1) ((SWDATA[2] & a1) != 0)
// editor func
void AL_KW_LikeEdit(task* tp) {
    taskwk* work = tp->twp;
    task* pChaoTask = ALW_GetTaskCount(0, work->ang.x);
    chaowk* chaowork = GET_CHAOWK(pChaoTask);
    CHAO_PARAM* pParam = &chaowork->param;

    Uint32 editParam[4] = { 0, 1, 7, 27 };
    int v14;

    AL_EnableMove();
    if (per[2]->press & NJD_DGT_TB) {
        AL_DisableMove();
    }

    if (work->smode == 0) {
        v14 = 0;
    } else if (work->smode < 7) {
        v14 = 1;
    } else if (work->smode < 27) {
        v14 = 2;
    } else if (work->smode < 147) {
        v14 = 3;
    }

    if (BUTTON_ON(NJD_DGT_KU)) {
        if (work->smode > 0)
            work->smode--;
    }
    if (BUTTON_ON(NJD_DGT_KD)) {
        if (work->smode < 145) {
            work->smode++;
        }
    }

    if (!(per[2]->on & NJD_DGT_TA)) {
        if (BUTTON_ON(PDD_DGT_KL)) {
            if (v14 == 0)
                v14 = 3;
            else
                v14--;
            work->smode = editParam[v14];
        }
        if (BUTTON_ON(NJD_DGT_KR)) {
            if (v14 == 3)
                v14 = 0;
            else
                v14++;
            work->smode = editParam[v14];
        }
    } else {
        int add;

        if (BUTTON_ON(PDD_DGT_KL)) {
            add = -1;
        } else if (BUTTON_ON(NJD_DGT_KR)) {
            add = 1;
        } else {
            add = 0;
        }

        switch (v14) {
            case 0:
                AL_ParameterAddUserLike(pChaoTask, add, 0);
                break;
            case 1:
                AL_KW_AddPlayerLike(pChaoTask, add);
                break;

            /////FAKE MATCH?
            /////not having the += there will not cause it to match
            /////cant figure out this one, but oh well, its if(0)'d out anyways
            case 2:
                if (0) {
                    pParam->knowledge.chao[0].like += 3;
                }
                break;
            case 3:
                if (0) {
                    pParam->knowledge.chao[0].like += 3;
                }
                break;
        }
    }
    switch (v14) {
        case 0:
            njPrintColor(0xDD66FF66);
            njPrintC(0x10003, "USER");
            njPrintColor(0xFFFFFFFF);
            njPrintD(0x90003, pParam->like, 3);
            AL_DrawMeter(14, 3, (pParam->like * 50) / 139, 0xFF55FF55, 0xFF008800);
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
    }
}

// this might seem stupid but copypasting the same function 3 times also seems stupid lol
#define AL_KW_FLAG_FUNC(flag)                                      \
    void AL_KW_##flag##FlagOn(task* pChaoTask, int Kind) {         \
        AL_KNOWLEDGE* kwlg = &GET_CHAOPARAM(pChaoTask)->knowledge; \
        kwlg->##flag##Flag |= 1 << Kind;                           \
    }                                                              \
    void AL_KW_##flag##FlagOff(task* pChaoTask, int Kind) {        \
        AL_KNOWLEDGE* kwlg = &GET_CHAOPARAM(pChaoTask)->knowledge; \
        kwlg->##flag##Flag = 0;                                    \
    }                                                              \
    Bool AL_KW_Is##flag##FlagOn(task* pChaoTask, int Kind) {       \
        AL_KNOWLEDGE* kwlg = &GET_CHAOPARAM(pChaoTask)->knowledge; \
        if (kwlg->##flag##Flag & (1 << Kind))                      \
            return TRUE;                                           \
        return FALSE;                                              \
    }

AL_KW_FLAG_FUNC(Art)
AL_KW_FLAG_FUNC(Dance)
AL_KW_FLAG_FUNC(Song)
AL_KW_FLAG_FUNC(Music)
AL_KW_FLAG_FUNC(SToy)
AL_KW_FLAG_FUNC(LToy)