#include <Chao/Chao.h>

extern char aChaoSoundStdMl[];
extern char aChaoCharaSsMlt[];
extern char aChaoCharaKrMlt[];
extern char aChaoCharaTeMlt[];

int LoadMLT(const char*);
void sub_8C05D980(const char*, int);
void sub_8C059848(int, int);

void sub_C518860(void) {
    int val;
    const char* loadedMLT;
    bool val2;
    val = sub_8C027060(0);
    sub_8C05D8C0();

    if (LoadMLT(aChaoSoundStdMl) == 0)
        val2 = 1;
    else
        val2 = 0;

    sub_8C05D8A8(val);
    if (!val2) {
        sub_8C059848(0, 0);
        sub_8C059848(0, 7);
    }

    // probably cast to character enum
    switch ((Sint8)val) {
        case 0:
        case 1:
            loadedMLT = aChaoCharaSsMlt;
            break;
        case 4:
        case 5:
            loadedMLT = aChaoCharaKrMlt;
            break;
        case 2:
        case 3:
            loadedMLT = aChaoCharaTeMlt;
            break;
        default:
            loadedMLT = aChaoCharaSsMlt;
            break;
    }
    if (LoadMLT(loadedMLT)) {
        sub_8C059848(0, 2);
    }

    sub_8C059848(0xC565614, 1);
    sub_8C059848(0xC56569C, 4);
    sub_8C059848(0xC565BAC, 6);
    sub_8C05D8F2();
}

extern char aChaoNeutralGar[];
extern char aChaoHeroGarden[];
extern char aChaoDarkGarden[];
extern char aChaoKindergart[];
extern char aChaoRaceCircui[];
extern char aChaoRaceEntran[];

void sub_0C5188F4(void) {
    switch (AL_GetStageNumber()) {
        case 1:
            sub_8C05D980(aChaoNeutralGar, 0xC565BD8);
            break;
        case 2:
            sub_8C05D980(aChaoHeroGarden, 0xC565C2C);
            break;
        case 3:
            sub_8C05D980(aChaoDarkGarden, 0xC565C80);
            break;
        case 6:
            sub_8C05D980(aChaoKindergart, 0xC565C9C);
            break;
        case 5:
            sub_8C05D980(aChaoRaceEntran, 0xC565CC8);
            break;
        case 4:
            sub_8C05D980(aChaoRaceCircui, 0xC565D3C);
            break;
        case 7:
            sub_8C05D980(aChaoRaceCircui, 0xC565D3C);
            break;
    }
}