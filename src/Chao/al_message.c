#include <ninja.h>

typedef struct LangTest {
    char lang;
} LangTest;
extern LangTest Language;

extern const char* off_C56F698[5];
extern const Uint32* pMsgList;

const Uint32* LoadPRSFile(char* filename);

const Uint32* AL_LoadMessageTable(char* fileid) {
    char buffer[256];
    const Uint32* pOut;

    strcpy(buffer, fileid);
    strcat(buffer, off_C56F698[Language.lang]);
    strcat(buffer, (const char*)0x0C55DC84); //.prs

    pOut = LoadPRSFile(buffer);
    if (!pOut) {
        return NULL;
    }

    pMsgList = pOut;

    return pMsgList;
}

void AL_ReleaseMessageTable(void) {
    if (pMsgList) {
        syFree(pMsgList);
        pMsgList = NULL;
    }
}

const char* AL_GetMessage(int MessageId) {
    if (!pMsgList)
        return NULL;

    return (pMsgList[MessageId] + (const char*)pMsgList);
}

int sub_8C0A6B42(int, int);
int sub_8C0A599C(int, int, int, short*, short*);

// some msg font thing, sub_53B8E0 on sa2pc
int sub_C510D6C(int a1, short* a2, short* a3) {
    int v1 = sub_8C0A6B42(a1, Language.lang);

    if (!a2 || !a3) {
        short out1, out2;
        sub_8C0A599C(v1, a1, Language.lang, &out1, &out2);
    } else {
        sub_8C0A599C(v1, a1, Language.lang, a2, a3);
    }

    return v1;
}