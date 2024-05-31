#include <Chao/Chao.h>
#include <Chao/al_landmark.h>
#include <Chao/al_stage.h>

typedef struct POS_XZ {
    int x;
    int z;
} POS_XZ;

typedef struct LAND_ATTR_INDEX {
    int nbIndex;
    POS_XZ pos[512];
} LAND_ATTR_INDEX;

extern LAND_ATTR_INDEX AttrIndexList[16];
extern int LandAttrNeut[40][32];
extern int LandAttrHero[40][32];
extern int LandAttrDark[40][32];
void AL_InitLandMark(void) {
    int i, j;
    memset(AttrIndexList, 0, sizeof(AttrIndexList));

    for (i = 0; i < 40; i++) {
        for (j = 0; j < 32; j++) {
            int attr = LMA_NONE;

            switch (AL_GetStageNumber()) {
                case CHAO_STG_NEUT:
                    attr = LandAttrNeut[i][j];
                    break;
                case CHAO_STG_HERO:
                    attr = LandAttrHero[i][j];
                    break;
                case CHAO_STG_DARK:
                    attr = LandAttrDark[i][j];
                    break;
            }

            if (attr >= 1 && attr < 16) {
                LAND_ATTR_INDEX* pAttr = &AttrIndexList[attr];
                pAttr->pos[pAttr->nbIndex].z = i;
                pAttr->pos[pAttr->nbIndex].x = j;
                pAttr->nbIndex++;
            }
        }
    }
}

Bool AL_GetRandomAttrPos(eLANDMARK_ATTR attr, NJS_POINT3* pPos) {
    LAND_ATTR_INDEX* pAttrIndex = &AttrIndexList[attr];
    int nbIndex = pAttrIndex->nbIndex;

    if (nbIndex > 0) {
        float findex = (float)(njRandom() * (nbIndex - 0.00001f));
        int index = (int)findex;
        int x = pAttrIndex->pos[index].x;
        int z = pAttrIndex->pos[index].z;

        pPos->x = 10.0f * ((x - 16) + njRandom());
        pPos->z = 10.0f * ((z - 20) + njRandom());
    } else {
        return FALSE;
    }

    return TRUE;
}

// undefined behavior, no default return
int AL_GetCurrLandAttr(NJS_POINT3* pPos) {
    float x = pPos->x;
    float z = pPos->z;
    int attr = LMA_NONE;

    if (x >= -150 && x <= 150 && z >= -200 && z <= 150) {
        int ix = (int)(x * 0.1f + 100.0f) - 84;
        int iz = (int)(z * 0.1f + 100.0f) - 80;

        switch (AL_GetStageNumber()) {
            case 1:
                attr = LandAttrNeut[iz][ix];
                break;
            case 2:
                attr = LandAttrHero[iz][ix];
                break;
            case 3:
                attr = LandAttrDark[iz][ix];
                break;
        }

        return attr;
    }
}

