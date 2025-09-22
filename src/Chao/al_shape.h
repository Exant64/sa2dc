#ifndef _AL_SHAPE_H_
#define _AL_SHAPE_H_

#include <task.h>

#define AL_SHAPE_FLAG_DEFORM BIT_1
#define AL_SHAPE_FLAG_SHADOW BIT_3
#define AL_SHAPE_FLAG_ENV BIT_4
#define AL_SHAPE_FLAG_MULTI BIT_6

typedef struct CNK_VN_VERTEX {
    NJS_POINT3 Pos;
    NJS_POINT3 Normal;
} CNK_VN_VERTEX;

typedef struct AL_VERTEX_INFO {
    NJS_POINT3 Velo;
    float Weight;
    float Spring;
    float Friction;
} AL_VERTEX_INFO;

typedef struct al_model {
    int* VList;
    Uint16* PList;
    NJS_POINT3 Center;
    float Radius;
    Sint16 OrgTexId[4];
    int PListSize;
    int nbVertex;
    CNK_VN_VERTEX* pVertex;
    NJS_POINT3* pOrgVertexPos;
    NJS_POINT3* pOrgVertexNormal;
    AL_VERTEX_INFO* pVertexInfo;
} al_model;

typedef struct AL_BUYO_PARAM {
    float Spring1;
    float Spring2;
    float Friction1;
    float Friction2;
    float MaxDiff;
    float MaxSpd;
    float Weight1;
    float Weight2;
    float Weight3;
    NJS_POINT3 Center;
} AL_BUYO_PARAM;

typedef struct al_object {
    Uint32 EvalFlags;
    al_model* pModel;
    float Pos[3];
    int Ang[3];
    float Scl[3];
    struct al_object* pChild;
    struct al_object* pSibling;
    struct al_object* pParent;
    NJS_POINT3 diff;
    NJS_POINT3 GlobalAimPos;
    NJS_POINT3 OrgPos;
    NJS_POINT3 Velo;
    NJS_POINT3 OrgAng;
    NJS_POINT3 AngSpd;
    float Weight;
    float Spring;
    float Friction;
    int NoBuyoFlag;
    int CalcBuyoPosFlag;
    NJS_CNK_OBJECT* pPartsObject;
    NJS_CNK_OBJECT* pItemObject;
    NJS_TEXLIST* pItemTexlist;
    float ItemScale;
    int ItemActiveFlag;
    int ItemOffsetFlag;
    NJS_POINT3 ItemOffsetPos;
    Angle3 ItemOffsetAng;
    NJS_MATRIX_PTR pOldMatrix;
    AL_BUYO_PARAM* pBuyoParam;
} al_object;

#endif