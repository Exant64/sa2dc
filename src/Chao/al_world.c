#include "Chao/Chao.h"
#include "Chao/al_stage.h"
#include "Chao/al_garden_info.h"

#include <player.h>

// this is higher on sa2b/sadx iirc
#define NB_WORLD_ENTRY 16

extern int nbWorldEntry[];
extern al_entry_work WorldEntryList[NB_CATEGORY][NB_WORLD_ENTRY];
extern const Uint32 nbMaxEntry[];
extern Uint16 CommuID;
extern NJS_MATRIX cam_mat, inv_cam_mat;
extern int PackFlag;
extern task* WorldMasterTask;

// only ALW_Edit doesn't match but i had to include everything else (data section splits + function boundary recognition really screwed me)
#ifdef NON_MATCHING

void ALW_ResetEntry(al_entry_work* pEntry) {
    if (!pEntry)
        return;

    pEntry->kind = 0;
    pEntry->flag = 0;
    pEntry->command = 0;
    pEntry->command_value = -1;
    pEntry->state = 0;

    pEntry->tp = NULL;
    pEntry->pCommu = NULL;
    pEntry->CommuID = -1;
}

void ALW_ClearEntry(void) {
    int i;

    for (i = 0; i < NB_CATEGORY; i++) {
        int j;

        nbWorldEntry[i] = 0;

        for (j = 0; j < NB_WORLD_ENTRY; j++) {
            al_entry_work* pEntry = &WorldEntryList[i][j];
            pEntry->category = i;
            pEntry->num = j;
            ALW_ResetEntry(pEntry);
        }
    }
}

task* ALW_GetTask(Uint16 category, Uint16 num) {
    if (num > nbMaxEntry[category] - 1)
        return NULL;

    return WorldEntryList[category][num].tp;
}

task* ALW_GetTaskCount(Uint16 category, Uint16 count) {
    int i;
    int j = 0;

    if (count > nbMaxEntry[category] - 1)
        return NULL;

    for (i = 0; i < nbWorldEntry[category]; i++) {
        if (WorldEntryList[category][i].tp) {
            if (count == j)
                return WorldEntryList[category][i].tp;

            j++;
        }
    }

    return NULL;
}

al_entry_work* ALW_GetEntry(Uint16 category, Uint16 num) {
    if (num < nbMaxEntry[category]) {
        al_entry_work* pEntry = &WorldEntryList[category][num];

        if (pEntry->tp) {
            return pEntry;
        }
    }

    return NULL;
}

al_entry_work* ALW_GetEntryCount(Uint16 category, Uint16 count) {
    int j = 0;

    if (count < nbMaxEntry[category]) {
        int i;
        Uint16 nb = nbWorldEntry[category];

        for (i = 0; i < nb; i++) {
            if (WorldEntryList[category][i].tp) {
                if (count == j)
                    return &WorldEntryList[category][i];

                j++;
            }
        }
    }

    return NULL;
}

int ALW_CountEntry(Uint16 category) {
    return nbWorldEntry[category];
}

Bool ALW_HeldOn(al_entry_work* pEntry) {
    if (pEntry) {
        pEntry->flag |= 2;
        return TRUE;
    }

    return FALSE;
}

Bool ALW_HeldOff(al_entry_work* pEntry) {
    if (pEntry) {
        pEntry->flag &= ~2;
        return TRUE;
    }

    return FALSE;
}

Bool ALW_IsHeld(task* tp) {
    al_entry_work* pEntry = ALW_ENTRY_WORK(tp);

    if (pEntry) {
        if (pEntry->flag & 2)
            return TRUE;
    }

    return FALSE;
}

Bool ALW_SetHeldOffset(task* tp, float offset) {
    al_entry_work* pEntry = ALW_ENTRY_WORK(tp);

    if (pEntry) {
        pEntry->offset = offset;
        return TRUE;
    }

    return FALSE;
}

Bool ALW_SetHeldRadius(task* tp, float radius) {
    al_entry_work* pEntry = ALW_ENTRY_WORK(tp);

    if (pEntry) {
        pEntry->radius = radius;
        return TRUE;
    }

    return FALSE;
}

Bool ALW_LockOn(task* pMyTask, task* pTargetTask) {
    al_entry_work* pMyEntry = ALW_ENTRY_WORK(pMyTask);
    al_entry_work* pTargetEntry = ALW_ENTRY_WORK(pTargetTask);

    if (pMyEntry && pTargetEntry) {
        pMyEntry->pLockOn = pTargetEntry;
        return TRUE;
    }

    return FALSE;
}

Bool sub_C51E0DA(task* tp) {
    int i;

    for (i = 0; i < 8; i++) {
        if (WorldEntryList[0][i].tp) {
            if (WorldEntryList[0][i].pLockOn) {
                if (WorldEntryList[0][i].pLockOn->tp == tp) {
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

task* ALW_GetLockOnTask(task* tp);
int ALW_TurnToLockOn(task* pMyTask, int RotSpd) {
    task* pLockOn = ALW_GetLockOnTask(pMyTask);
    return AL_TurnToTask(pMyTask, pLockOn, RotSpd);
}

task* ALW_GetLockOnTask(task* tp) {
    al_entry_work* pEntry = ALW_ENTRY_WORK(tp);
    if (pEntry) {
        if (pEntry->pLockOn)
            if (pEntry->pLockOn->tp)
                return pEntry->pLockOn->tp;
    }

    // no default return, undefined behavior
    // this caused us some trouble on sa2pc
    // it seems to have gotten fixed with sadx
}

float AL_CalcDistFromTask(task* tp1, task* tp2);
float ALW_CalcDistFromLockOn(task* pMyTask) {
    task* pLockOn = ALW_GetLockOnTask(pMyTask);
    return AL_CalcDistFromTask(pMyTask, pLockOn);
}

al_entry_work* ALW_GetLockOn(task* tp) {
    al_entry_work* pEntry = ALW_ENTRY_WORK(tp);
    if (pEntry) {
        if (pEntry->pLockOn)
            if (pEntry->pLockOn->tp)
                return pEntry->pLockOn;
    }

    // undefined behavior again
}

void ALW_UnkOn(task* tp) {
    al_entry_work* pEntry = ALW_ENTRY_WORK(tp);
    if (pEntry) {
        pEntry->flag |= 1;
    }
}

void ALW_UnkOff(task* tp) {
    al_entry_work* pEntry = ALW_ENTRY_WORK(tp);
    if (pEntry) {
        pEntry->flag &= ~1;
    }
}

Bool ALW_IsUnk(task* tp) {
    al_entry_work* pEntry = ALW_ENTRY_WORK(tp);
    if (pEntry) {
        if (pEntry->flag & 1)
            return TRUE;
    }

    return FALSE;
}

Bool ALW_AttentionOn(task* tp1, task* tp2) {
    al_entry_work* pEntry1 = ALW_ENTRY_WORK(tp1);
    al_entry_work* pEntry2 = ALW_ENTRY_WORK(tp2);
    if (pEntry1 && pEntry2) {
        pEntry1->pCommu = pEntry2;
        pEntry1->CommuID = CommuID;
        CommuID++;
        if (CommuID >= 0x100)
            CommuID = 0;

        return TRUE;
    }

    return FALSE;
}

void ALW_AttentionOff(task* tp) {
    al_entry_work* pEntry = ALW_ENTRY_WORK(tp);
    if (pEntry) {
        pEntry->flag = 0;
        pEntry->pCommu = 0;
        pEntry->CommuID = -1;
    }
}

al_entry_work* ALW_IsAttention(task* tp) {
    al_entry_work* pEntry = ALW_ENTRY_WORK(tp);
    if (pEntry)
        return pEntry->pCommu;

    return NULL;
}

Bool ALW_IsSheAttentionOtherOne(task* pMyTask, task* pHerTask) {
    al_entry_work* pEntry1 = ALW_ENTRY_WORK(pMyTask);
    al_entry_work* pEntry2 = ALW_ENTRY_WORK(pHerTask);
    if (pEntry2) {
        if (pEntry2->pCommu)
            if (pEntry2->pCommu != pEntry1)
                return TRUE;
    }
    return FALSE;
}

Bool ALW_CommunicationOn(task* tp1, task* tp2) {
    al_entry_work* pEntry1 = ALW_ENTRY_WORK(tp1);
    al_entry_work* pEntry2 = ALW_ENTRY_WORK(tp2);
    if (!pEntry1)
        return FALSE;
    if (!pEntry2)
        return FALSE;

    pEntry1->pCommu = pEntry2;
    pEntry2->pCommu = pEntry1;

    pEntry1->CommuID = CommuID;
    pEntry2->CommuID = CommuID;

    CommuID++;
    if (CommuID >= 0x100)
        CommuID = 0;

    return TRUE;
}

Bool ALW_CommunicationOff(task* tp) {
    al_entry_work* pEntry = ALW_ENTRY_WORK(tp);
    if (pEntry) {
        al_entry_work* pCommu = pEntry->pCommu;
        pEntry->flag = 0;
        pEntry->pCommu = NULL;
        pEntry->CommuID = -1;

        if (pCommu) {
            if (pCommu->pCommu == pEntry) {
                pCommu->flag = 0;
                pCommu->pCommu = NULL;
            }
            pCommu->CommuID = -1;
        }
    }

    return TRUE;
}

al_entry_work* ALW_IsCommunication(task* tp) {
    al_entry_work* pEntry = ALW_ENTRY_WORK(tp);
    if (pEntry) {
        al_entry_work* pCommu = pEntry->pCommu;
        if (pCommu) {
            if (pCommu->pCommu == pEntry)
                return pCommu;
        }
    }

    return NULL;
}

al_entry_work* ALW_IsCommunicationEx(task* tp, Uint16 category) {
    al_entry_work* pEntry = ALW_ENTRY_WORK(tp);
    if (pEntry) {
        al_entry_work* pCommu = pEntry->pCommu;
        if (pCommu) {
            if (pCommu->pCommu == pEntry && pCommu->category == category)
                return pCommu;
        }
    }

    return NULL;
}

Sint16 ALW_RecieveCommand(task* tp) {
    al_entry_work* pEntry = ALW_ENTRY_WORK(tp);
    Sint16 retval = 0;
    if (pEntry) {
        retval = pEntry->command;
        pEntry->command = 0;
    }

    return retval;
}

Bool ALW_SendCommand(task* tp, Sint16 command) {
    al_entry_work* pCommu = ALW_IsCommunication(tp);
    if (pCommu) {
        pCommu->command = command;
        return TRUE;
    }

    return FALSE;
}

Bool sub_C51E3A0(al_entry_work* pEntry, Sint16 command) {
    if (pEntry) {
        pEntry->command = command;
        return TRUE;
    }

    return FALSE;
}

int sub_C51E3B2(task* tp, Sint16 command) {
    al_entry_work* pEntry = ALW_ENTRY_WORK(tp);
    int retval = -1;
    if (pEntry) {
        retval = pEntry->command_value;
        pEntry->command_value = -1;
    }

    return retval;
}

Bool sub_C51E3C6(task* tp, Sint16 command, Sint16 command_value) {
    al_entry_work* pCommu = ALW_IsCommunication(tp);
    if (pCommu) {
        pCommu->command = command;
        pCommu->command_value = command_value;
        return TRUE;
    }

    return FALSE;
}

Bool sub_C51E3F8(al_entry_work* pEntry, Sint16 command, Sint16 command_value) {
    if (pEntry) {
        pEntry->command = command;
        pEntry->command_value = command_value;
        return TRUE;
    }

    return FALSE;
}

Bool ALW_Entry(Uint16 category, task* tp, Uint16 kind) {
    // doesn't match as a for loop, no clue why
    int i = 0;
    do {
        if (!WorldEntryList[category][i].tp) {
            WorldEntryList[category][i].tp = tp;
            WorldEntryList[category][i].kind = kind;
            nbWorldEntry[category]++;
            tp->EntityData2 = &WorldEntryList[category][i];

            switch (category) {
                case ALW_CATEGORY_CHAO:
                case ALW_CATEGORY_EGG:
                    tp->twp->id = WorldEntryList[category][i].num;
                    break;
            }

            return TRUE;
        }

        i++;
    } while (i <= nbMaxEntry[category] - 1);

    return FALSE;
}

Bool ALW_CancelEntry(task* tp) {
    al_entry_work* pEntry = ALW_ENTRY_WORK(tp);

    ALW_CommunicationOff(tp);

    if (pEntry) {
        if (pEntry->tp == tp) {
            ALW_ResetEntry(pEntry);
            nbWorldEntry[pEntry->category]--;
        }

        tp->EntityData2 = NULL;
    }

    return TRUE;
}

int ALW_GetCategory(task* tp) {
    al_entry_work* pEntry = ALW_ENTRY_WORK(tp);
    if (pEntry)
        return pEntry->category;

    return -1;
}

void ALW_Edit() {
    int k = 3;
    const char* CategoryNames[] = {
        "CHAO", "EGG", "MINIMAL", "FRUIT", "TREE", "GTREE", "TOY", "SEED", "SOUND", "HEAD"
    };

    const int CommuColor[] = { 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFFFFFF00,
                               0xFFFF00FF, 0xFF00FFFF, 0xFFFF7F7F, 0xFF7F7FFF };

    int i = 0;

    do {
        int j;
        // int k = ; //  int y = 3 + i;

#define yPos k
        njPrintC(NJM_LOCATION(1, yPos), CategoryNames[i]);
        njPrintD(NJM_LOCATION(7, yPos), ALW_CountEntry(i), 2);

        for (j = 0; j < nbMaxEntry[i]; j++) {
            task* tp = ALW_GetTask(i, j);
            // int pos = 3 + NJM_LOCATION(j + 10, i);
#define secondYPos NJM_LOCATION(j + 10, yPos)
            if (tp) {
                if (ALW_IsCommunication(tp)) {
                    if (ALW_ENTRY_WORK(tp)->CommuID >= 0) {
                        njPrintColor(CommuColor[ALW_ENTRY_WORK(tp)->CommuID % 8]);
                    } else
                        njPrintColor(0xFFFFFFFF);

                    njPrintC(secondYPos, "=");
                } else if (ALW_IsAttention(tp)) {
                    if (ALW_ENTRY_WORK(tp)->CommuID >= 0) {
                        njPrintColor(CommuColor[ALW_ENTRY_WORK(tp)->CommuID % 8]);
                    } else {
                        njPrintColor(0xFFFFFFFF);
                    }

                    njPrintC(secondYPos, "!");
                } else {
                    njPrintColor(0xFFFFFFFF);
                    njPrintC(secondYPos, "-");
                }
            } else {
                njPrintColor(0xFFCCCCCC);
                njPrintC(secondYPos, ".");
            }

            njPrintColor(0xFFFFFFFF);
        }
        i++;
        k++;
    } while (i <= NB_CATEGORY - 1);

#define counter i
    for (counter = 0; counter < 24; counter++) {
        CHAO_PARAM* pParam = &ChaoInfo[counter].chao.param;
        switch (pParam->place) {
            case CHAO_STG_NEUT:
                njPrintColor(0xFF7FFF7F);
                break;
            case CHAO_STG_HERO:
                njPrintColor(0xFF7F7FFF);
                break;
            case CHAO_STG_DARK:
                njPrintColor(0xFFFF7F7F);
                break;
            default:
                njPrintColor(0xFFFFFFFF);
                break;
        }

        switch (pParam->type) {
            case TYPE_NONE:
                njPrintC(NJM_LOCATION(counter, 14), ".");
                break;
            case TYPE_EGG:
                njPrintC(NJM_LOCATION(counter, 14), "E");
                break;
            default:
                njPrintC(NJM_LOCATION(counter, 14), "C");
        }
    }

    njPrintColor(0xFFFFFFFF);
}
extern int PrePlayerMode;
void ALW_Control(task* tp) {
    taskwk* work = tp->twp;
    eCHAO_STAGE_NUMBER stage;
    int obakeCount;

    switch (work->mode) {
        case 0:
            work->mode++;
            PrePlayerMode = -1;
            break;

        case 1:
            stage = AL_GetStageNumber();
            if (stage >= CHAO_STG_NEUT && stage <= CHAO_STG_DARK) {
                work->mode = 2;
            } else {
                work->mode = 3;
            }
            break;

        case 2:
            AL_ParameterControl();

            switch (GetCharID(0)) {
                case 4:
                case 5:
                    obakeCount = ALW_CountEntry(ALW_CATEGORY_MASK);
                    if (obakeCount < 8) {
                        if (PrePlayerMode == 61 && playertwp[0]->mode == 62) {
                            float count = (obakeCount + 1);
                            float count2 = count;
                            float chance = count2 * count2;
                            if (njRandom() < 0.1f / chance) {
                                int id;
                                NJS_POINT3 pos;
                                NJS_VECTOR velo;

                                if (AL_GetStageNumber() == CHAO_STG_DARK)
                                    id = 2;
                                else
                                    id = 1;

                                pos.x = playertwp[0]->pos.x;
                                pos.y = playertwp[0]->pos.y + 2;
                                pos.z = playertwp[0]->pos.z;

                                velo.x = 0;
                                velo.y = 1.5f;
                                velo.z = 0;

                                ALO_ObakeHeadCreate(id, &pos, NJM_DEG_ANG(njRandom() * 360), &velo);
                            }
                        }

                        PrePlayerMode = playertwp[0]->mode;
                    }
                    break;
            }
            break;
    }

    AL_ClearFruitNum();
}

void ALW_Displayer(task* tp) {
    njGetMatrix(cam_mat);
    njGetMatrix(inv_cam_mat);
    njInvertMatrix(inv_cam_mat);

    if (GetGameState() == 17) {
        if (!PackFlag) {
            AL_PackageAllSaveInfo();
            PackFlag = TRUE;
        }
    } else {
        PackFlag = FALSE;
    }
}

void ALW_Destructor(task* tp) {
    WorldMasterTask = NULL;
    ALW_ClearEntry();
}

Bool ALW_Create(void) {
    if (WorldMasterTask) {
        DestroyTask(WorldMasterTask);
        WorldMasterTask = NULL;
    }

    WorldMasterTask = CreateElementalTask(2, 1, ALW_Control, (const char*)0x0C5669AC);
    WorldMasterTask->disp = ALW_Displayer;
    WorldMasterTask->dest = ALW_Destructor;

    ALW_ClearEntry();
    AL_ParameterControlInit();
    AL_EnableMove();

    return TRUE;
}

int sub_0C51E994(void) {   
    return 0;
}

#else

#include <task.h>

INLINE_ASM(_ALW_ResetEntry, "asm/nonmatching/Chao/al_world/_ALW_ResetEntry.src");

INLINE_ASM(_ALW_ClearEntry, "asm/nonmatching/Chao/al_world/_ALW_ClearEntry.src");

INLINE_ASM(_ALW_GetTask, "asm/nonmatching/Chao/al_world/_ALW_GetTask.src");

INLINE_ASM(_ALW_GetTaskCount, "asm/nonmatching/Chao/al_world/_ALW_GetTaskCount.src");

INLINE_ASM(_ALW_GetEntry, "asm/nonmatching/Chao/al_world/_ALW_GetEntry.src");

INLINE_ASM(_ALW_GetEntryCount, "asm/nonmatching/Chao/al_world/_ALW_GetEntryCount.src");

INLINE_ASM(_ALW_CountEntry, "asm/nonmatching/Chao/al_world/_ALW_CountEntry.src");

INLINE_ASM(_ALW_HeldOn, "asm/nonmatching/Chao/al_world/_ALW_HeldOn.src");

INLINE_ASM(_ALW_HeldOff, "asm/nonmatching/Chao/al_world/_ALW_HeldOff.src");

INLINE_ASM(_ALW_IsHeld, "asm/nonmatching/Chao/al_world/_ALW_IsHeld.src");

INLINE_ASM(_ALW_SetHeldOffset, "asm/nonmatching/Chao/al_world/_ALW_SetHeldOffset.src");

INLINE_ASM(_ALW_SetHeldRadius, "asm/nonmatching/Chao/al_world/_ALW_SetHeldRadius.src");

INLINE_ASM(_ALW_LockOn, "asm/nonmatching/Chao/al_world/_ALW_LockOn.src");

INLINE_ASM(_sub_C51E0DA, "asm/nonmatching/Chao/al_world/_sub_C51E0DA.src");

INLINE_ASM(_ALW_TurnToLockOn, "asm/nonmatching/Chao/al_world/_ALW_TurnToLockOn.src");

INLINE_ASM(_ALW_GetLockOnTask, "asm/nonmatching/Chao/al_world/_ALW_GetLockOnTask.src");

INLINE_ASM(_ALW_CalcDistFromLockOn, "asm/nonmatching/Chao/al_world/_ALW_CalcDistFromLockOn.src");

INLINE_ASM(_ALW_GetLockOn, "asm/nonmatching/Chao/al_world/_ALW_GetLockOn.src");

INLINE_ASM(_ALW_UnkOn, "asm/nonmatching/Chao/al_world/_ALW_UnkOn.src");

INLINE_ASM(_ALW_UnkOff, "asm/nonmatching/Chao/al_world/_ALW_UnkOff.src");

INLINE_ASM(_ALW_IsUnk, "asm/nonmatching/Chao/al_world/_ALW_IsUnk.src");

INLINE_ASM(_ALW_AttentionOn, "asm/nonmatching/Chao/al_world/_ALW_AttentionOn.src");

INLINE_ASM(_ALW_AttentionOff, "asm/nonmatching/Chao/al_world/_ALW_AttentionOff.src");

INLINE_ASM(_func_0C51E236, "asm/nonmatching/Chao/al_world/_func_0C51E236.src");

INLINE_ASM(_ALW_IsSheAttentionOtherOne, "asm/nonmatching/Chao/al_world/_ALW_IsSheAttentionOtherOne.src");

INLINE_ASM(_ALW_CommunicationOn, "asm/nonmatching/Chao/al_world/_ALW_CommunicationOn.src");

INLINE_ASM(_ALW_CommunicationOff, "asm/nonmatching/Chao/al_world/_ALW_CommunicationOff.src");

INLINE_ASM(_ALW_IsCommunication, "asm/nonmatching/Chao/al_world/_ALW_IsCommunication.src");

INLINE_ASM(_func_0C51E312, "asm/nonmatching/Chao/al_world/_func_0C51E312.src");

INLINE_ASM(_ALW_RecieveCommand, "asm/nonmatching/Chao/al_world/_ALW_RecieveCommand.src");

INLINE_ASM(_ALW_SendCommand, "asm/nonmatching/Chao/al_world/_ALW_SendCommand.src");

INLINE_ASM(_sub_C51E3A0, "asm/nonmatching/Chao/al_world/_sub_C51E3A0.src");

INLINE_ASM(_sub_C51E3B2, "asm/nonmatching/Chao/al_world/_sub_C51E3B2.src");

INLINE_ASM(_sub_C51E3C6, "asm/nonmatching/Chao/al_world/_sub_C51E3C6.src");

INLINE_ASM(_sub_C51E3F8, "asm/nonmatching/Chao/al_world/_sub_C51E3F8.src");

INLINE_ASM(_ALW_Entry, "asm/nonmatching/Chao/al_world/_ALW_Entry.src");

INLINE_ASM(_ALW_CancelEntry, "asm/nonmatching/Chao/al_world/_ALW_CancelEntry.src");

INLINE_ASM(_ALW_GetCategory, "asm/nonmatching/Chao/al_world/_ALW_GetCategory.src");

INLINE_ASM(_func_0C51E4EC, "asm/nonmatching/Chao/al_world/_func_0C51E4EC.src");

INLINE_ASM(_ALW_Control, "asm/nonmatching/Chao/al_world/_ALW_Control.src");

INLINE_ASM(_ALW_Displayer, "asm/nonmatching/Chao/al_world/_ALW_Displayer.src");

INLINE_ASM(_ALW_Destructor, "asm/nonmatching/Chao/al_world/_ALW_Destructor.src");

INLINE_ASM(_ALW_Create, "asm/nonmatching/Chao/al_world/_ALW_Create.src");

INLINE_ASM(_sub_0C51E994, "asm/nonmatching/Chao/al_world/_sub_0C51E994.src");

INLINE_ASM(_func_0C51E998, "asm/nonmatching/Chao/al_world/_func_0C51E998.src");

// clang-format on
#endif
