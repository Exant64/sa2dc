#include <Chao/Chao.h>
#include <Chao/al_stage.h>
#include <Chao/al_garden_info.h>
#include <sg_sycfg.h>

extern CHAO_GARDEN_INFO GardenInfo;
extern OsanpoThing unk_C667418;

void AL_InitGardenID(void) {
    GARDEN_ID* gid = &GardenInfo.GardenID;

    {
        Sint8 iid[SYD_CFG_IID_SIZE];
        int retval = syCfgGetIndividualID(iid);
        if (retval == SYD_CFG_IID_OK) {
            Uint32 id1 = 0;
            Uint32 id2 = 0;

            id1 |= ~iid[4];
            id1 |= (iid[0] + 50) << 8;
            id1 |= (~iid[3] + 17) << 16;
            id1 |= ((Uint8)(njRandom() * 100)) << 24;

            id2 |= (~iid[1] + 0xA3);
            id2 |= (~iid[5] + 50) << 8;
            id2 |= (~iid[2] + 17) << 16;
            id2 |= ((Uint8)(njRandom() * 100)) << 24;

            gid->id[0] = id1;
            gid->id[1] = id2;
        } else {
            Uint32 id1 = 0;
            Uint32 id2 = 0;

            id1 |= ((Uint8)(njRandom() * 100));
            id1 |= ((Uint8)(njRandom() * 100)) << 8;
            id1 |= ((Uint8)(njRandom() * 100)) << 16;
            id1 |= ((Uint8)(njRandom() * 100)) << 24;

            id2 |= ((Uint8)(njRandom() * 100));
            id2 |= ((Uint8)(njRandom() * 100)) << 8;
            id2 |= ((Uint8)(njRandom() * 100)) << 16;
            id2 |= ((Uint8)(njRandom() * 100)) << 24;

            gid->id[0] = id1;
            gid->id[1] = id2;
        }
    }
    {
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

            gid->id[2] = id;
        } else {
            Uint32 id = 0;

            id |= ((Uint8)(njRandom() * 100));
            id |= ((Uint8)(njRandom() * 100)) << 8;
            id |= ((Uint8)(njRandom() * 100)) << 16;
            id |= ((Uint8)(njRandom() * 100)) << 24;

            gid->id[2] = id;
        }
    }
}

TREE_SAVE_INFO LocalTreeInitSaveInfo = { 10, 5 };
TREE_SAVE_INFO TreeZeroSave = { 0 };

void AL_InitTreeSaveInfo(void) {
    int i = 0;
    int k = 0;

    for (i = 0; i < 3; i++) {
        TREE_SAVE_INFO* pTree = (&GardenInfo)->tree[i];

        k = 2;
        do {
            *pTree = LocalTreeInitSaveInfo;

            pTree->FruitGrowth[0] = (Uint32)(njRandom() * 49.9999f);
            pTree->FruitGrowth[1] = (Uint32)(njRandom() * 49.9999f);
            pTree->FruitGrowth[2] = (Uint32)(njRandom() * 49.9999f);
            ++pTree;
        } while (--k);

        k = 4;
        do {
            *pTree = TreeZeroSave;
            ++pTree;
        } while (--k);
    }
}

void AL_InitRaceSaveInfo(void) {
    race_save_info* race = &GardenInfo.race;
    int i;

    race->RaceActiveFlag[0] = RACE_ACTIVE_FLAG_OPEN; // beginner race
    race->RaceActiveFlag[1] = RACE_ACTIVE_FLAG_NONE;
    race->RaceActiveFlag[2] = RACE_ACTIVE_FLAG_NONE;
    race->RaceActiveFlag[3] = RACE_ACTIVE_FLAG_NONE;
    race->RaceActiveFlag[4] = RACE_ACTIVE_FLAG_NONE;
    race->RaceActiveFlag[5] = RACE_ACTIVE_FLAG_OPEN; // exit?

    for (i = 0; i < 13; i++)
        race->CourseChallengedLevel[i] = -1;
    for (i = 0; i < 13; i++)
        race->CourseClearedLevel[i] = -1;
}

int sub_C50B6FE(void) {
    sub_C50B7A0();
    return 1;
}

extern CHAO_SAVE unk_C660CE0;
int sub_C50B70A(void) {
    int ret;

    unk_C667418.a4 = &unk_C660CE0;
    unk_C667418.a5 = sizeof(CHAO_SAVE);
    ret = sub_C51996E(&unk_C667418);
    return ret;
}

// load file?
int sub_C50B724(void) {
    int a1; // probably some unused errorcode thing
    njMemCopy(&GardenInfo, &unk_C660CE0.garden.garden, sizeof(GardenInfo));
    njMemCopy(ChaoInfo, unk_C660CE0.chao.chao, sizeof(ChaoInfo));
    return a1;
}

int sub_C50B7A0(void) {
    const int size = sizeof(ChaoInfo);

    int ret;

    AL_PackageAllSaveInfo();

    memset(&unk_C660CE0.garden, 0, sizeof(unk_C660CE0.garden));
    memset(&unk_C660CE0.chao, 0, sizeof(CHAO_SAVE_CHAO));

    njMemCopy(&unk_C660CE0.garden.garden, &GardenInfo, sizeof(GardenInfo));
    njMemCopy(unk_C660CE0.chao.chao, ChaoInfo, size);

    unk_C667418.a4 = &unk_C660CE0;
    unk_C667418.a5 = sizeof(CHAO_SAVE) - 0x240; // im so fucking confused
    ret = sub_C519920(&unk_C667418);
    return ret;
}

int AL_GetOsanpoThing(void) {
    return unk_C667418.a2;
}

void nullsub_4(void) {
}
void nullsub_5(void) {
}
void nullsub_6(void) {
}

NJS_VECTOR TreeSetPos[3][10] = {
    { { 50, 0, -80 }, { -20, 0, -35 }, { -21, 0, -143 }, { 8, 9.4f, 39 }, { 50, 0, -120 }, { 30, 0, -60 } },

    { { 100.0, 0.0, 50.0 },
      { -100.0, 0.0, 0.0 },
      { -75.0, 0.0, 50.0 },
      { -25.0, 0.0, -12.5 },
      { 75.0, 0.0, 100.0 },
      { 85.0, 0.0, 10.0 } },

    {
        { -80.0, 0.0, 2.0 },
        { -30.0, 0.0, -60.0 },
        { -82.0, 0.0, 95.0 },
        { -15.0, 0.0, -20.0 },
        { -43.0, 0.0, 20.0 },
        { -41.0, 0.0, -102.0 },
    },
};
void AL_SetObjectOnTheGarden(void) {
    eCHAO_STAGE_NUMBER area = AL_GetStageNumber();

    if (area >= CHAO_STG_NEUT || area <= CHAO_STG_DARK) {
        int i;

        area--;

        for (i = 0; i < 6; i++) {
            ALO_CreateGrowTree(&TreeSetPos[area][i], &GardenInfo.tree[area][i]);
        }
    }
}

void sub_C5025E0(task* tp, CHAO_PARAM* pParam);
void sub_C502632(task* tp, CHAO_PARAM* pParam);

void AL_PackageChaoSaveInfo(void) {
    int count = ALW_CountEntry(ALW_CATEGORY_CHAO);
    while (count) {
        task* tp;
        --count;
        tp = ALW_GetTaskCount(ALW_CATEGORY_CHAO, count);
        if (tp) {
            CHAO_PARAM* pParam = GET_CHAOWK(tp)->pParam;
            sub_C5025E0(tp, pParam);
        }
    }

    count = ALW_CountEntry(ALW_CATEGORY_EGG);
    while (count) {
        task* tp;
        --count;
        tp = ALW_GetTaskCount(ALW_CATEGORY_EGG, count);
        if (tp) {
            CHAO_PARAM* pParam = GET_CHAOWK(tp)->pParam;
            sub_C502632(tp, pParam);
        }
    }
}
void AL_PackageTreeSaveInfo(void) {
    eCHAO_STAGE_NUMBER area = AL_GetStageNumber();
    if (area >= CHAO_STG_NEUT || area <= CHAO_STG_DARK) {
        int count = ALW_CountEntry(ALW_CATEGORY_TREE);
        while (count) {
            task* tp;
            --count;
            tp = ALW_GetTaskCount(ALW_CATEGORY_TREE, count);
            if (tp) {
                sub_C526EBC(tp);
            }
        }

        count = ALW_CountEntry(ALW_CATEGORY_GROWTREE);
        while (count) {
            task* tp;
            --count;
            tp = ALW_GetTaskCount(ALW_CATEGORY_GROWTREE, count);
            if (tp) {
                sub_C526EBC(tp);
            }
        }
    }
}

void AL_PackageRaceSaveInfo(void) {
    GardenInfo.race = *AL_GetRaceInfo();
}

void AL_PackageAllSaveInfo(void) {
    AL_PackageChaoSaveInfo();
    AL_PackageTreeSaveInfo();
    AL_PackageRaceSaveInfo();
}

void AL_CreateGardenInfo(void) {
    memset(&GardenInfo, 0, sizeof(CHAO_GARDEN_INFO));
    GardenInfo.MaxChao = 24;

    AL_InitGardenID();
    AL_InitTreeSaveInfo();
    AL_InitRaceSaveInfo();
    AL_InitChaoSaveInfo();
}

CHAO_GARDEN_INFO* AL_GetCurrGardenInfo(void) {
    return &GardenInfo;
}

TREE_SAVE_INFO* AL_GetTreeSaveInfo(void) {
    return GardenInfo.tree;
}

race_save_info* AL_GetRaceSaveInfo(void) {
    return &GardenInfo.race;
}

KINDER_SAVE_INFO* AL_GetKinderSaveInfo(void) {
    return &GardenInfo.kinder;
}

GARDEN_ID* AL_GetGardenID(void) {
    return &GardenInfo.GardenID;
}

int AL_GetMaxChao(void) {
    return GardenInfo.MaxChao;
}

Bool AL_IsOnGardenFlag(Uint32 flag) {
    if (GardenInfo.flag & flag)
        return TRUE;

    return FALSE;
}

void AL_OnGardenFlag(Uint32 flag) {
    GardenInfo.flag |= flag;
}

void AL_OffGardenFlag(Uint32 flag) {
    GardenInfo.flag &= ~flag;
}

Bool AL_IsOnToyFlag(Uint32 flag) {
    int bit = (1 << flag);
    if (GardenInfo.ToyOnFlag & bit)
        return TRUE;

    return FALSE;
}

void AL_OnToyFlag(Uint32 flag) {
    int bit = (1 << flag);
    GardenInfo.ToyOnFlag |= bit;
}
