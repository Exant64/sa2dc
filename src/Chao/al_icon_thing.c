#include <task.h>
#include <Chao/al_texlist.h>

extern NJS_TEXTURE_VTX stru_C6259D8[];

void sub_C528D60(NJS_POINT3* pPos, Uint32 color, float size) {
    NJS_POINT3 outPos;
    stru_C6259D8[0].col = color;
    stru_C6259D8[1].col = color;
    stru_C6259D8[2].col = color;
    stru_C6259D8[3].col = color;

    njCalcPoint(NULL, pPos, &outPos);

    njPushMatrix(_nj_unit_matrix_);

    njSetTexture(&AL_ICON_TEXLIST);
    njSetTextureNum(9);

    njTranslateV(NULL, &outPos);
    njScale(NULL, size, size, 1);

    njColorBlendingMode(0, 8);
    njColorBlendingMode(1, 0xA);
    njDrawTexture3DEx(stru_C6259D8, 4, 1);
    njColorBlendingMode(0, 8);
    njColorBlendingMode(1, 6);

    njPopMatrixEx();
}

void sub_C528DEA(NJS_POINT3* pPos, Uint32 color, float size) {
    NJS_POINT3 outPos;
    stru_C6259D8[0].col = color;
    stru_C6259D8[1].col = color;
    stru_C6259D8[2].col = color;
    stru_C6259D8[3].col = color;

    njCalcPoint(NULL, pPos, &outPos);

    njPushMatrix(_nj_unit_matrix_);

    njSetTexture(&AL_ICON_TEXLIST);
    njSetTextureNum(0xB);

    njTranslateV(NULL, &outPos);
    njScale(NULL, size, size, 1);

    njColorBlendingMode(0, 5);
    njColorBlendingMode(1, 7);
    njDrawTexture3DEx(stru_C6259D8, 4, 1);
    njColorBlendingMode(0, 8);
    njColorBlendingMode(1, 6);

    njPopMatrixEx();
}