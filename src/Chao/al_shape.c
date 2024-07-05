#include <task.h>
#include <Chao/Chao.h>

int GetList(al_object* pSrcObject, al_object** List, int num) {
    List[num++] = pSrcObject;

    if (pSrcObject->pChild) {
        num = GetList(pSrcObject->pChild, List, num);
    }

    if (pSrcObject->pSibling) {
        num = GetList(pSrcObject->pSibling, List, num);
    }

    return num;
}

void AL_GetObjectList(al_object* pSrcObject, al_object** List) {
    int num = 0;

    List[0] = pSrcObject;
    num++;

    if (pSrcObject->pChild)
        num = GetList(pSrcObject->pChild, List, num);

    if (pSrcObject->pSibling)
        num = GetList(pSrcObject->pSibling, List, num);
}

#define SHAPE_FREE(a1)    \
    if (a1) {       \
        syFree(a1); \
        a1 = NULL;     \
    }
void AL_FreeChaoObject(al_object* pObject) {
    al_model* pModel;

    if (!pObject)
        return;

    if (pObject->pChild) {
        AL_FreeChaoObject(pObject->pChild);
        pObject->pChild = NULL;
    }

    if (pObject->pSibling) {
        AL_FreeChaoObject(pObject->pSibling);
        pObject->pSibling = NULL;
    }

    pModel = pObject->pModel;
    if (pModel) {
        SHAPE_FREE(pModel->VList);
        SHAPE_FREE(pModel->PList);
        SHAPE_FREE(pModel->pOrgVertexPos);
        SHAPE_FREE(pModel->pOrgVertexNormal);
        SHAPE_FREE(pModel->pVertexInfo);

        syFree(pModel);
    }

    if(pObject->pOldMatrix) {
        syFree(pObject->pOldMatrix);
    }

    syFree(pObject);
}

MERGE_LIST([['_syFree', '_lbl_0C517B20']]);
INLINE_ASM(_func_0C517A6C, "asm/nonmatching/Chao/al_shape/_func_0C517A6C.src");

INLINE_ASM(_AL_CopyChaoObject, "asm/nonmatching/Chao/al_shape/_AL_CopyChaoObject.src");

// MERGE_LIST([['__divls', '_lbl_0C517F20'], ['_syCalloc', '_lbl_0C517F24'], ["h'8C1119A8", '_lbl_0C517F28']]);
INLINE_ASM(_func_0C517ECC, "asm/nonmatching/Chao/al_shape/_func_0C517ECC.src");

// MERGE_LIST([['_njScalor', '_lbl_0C5180C0']]);
INLINE_ASM(_func_0C517F8C, "asm/nonmatching/Chao/al_shape/_func_0C517F8C.src");

INLINE_ASM(_func_0C517FCE, "asm/nonmatching/Chao/al_shape/_func_0C517FCE.src");

// MERGE_LIST([['_lbl_0C5FEE08', '_lbl_0C5180C4']]);
INLINE_ASM(_AL_CalcBuyoParam, "asm/nonmatching/Chao/al_shape/_AL_CalcBuyoParam.src");

INLINE_ASM(_AL_CreateOrgVertexList, "asm/nonmatching/Chao/al_shape/_AL_CreateOrgVertexList.src");

INLINE_ASM(_AL_ShapeInit, "asm/nonmatching/Chao/al_shape/_AL_ShapeInit.src");

// MERGE_LIST([['_lbl_0C67D0FC', '_lbl_0C518680'], ['_lbl_0C67D100', '_lbl_0C518684'], ['_lbl_0C5FEC10', '_lbl_0C518688'], ['_syCalloc', '_lbl_0C51868C'], ['_lbl_0C50A244', '_lbl_0C518690'], ['_lbl_0C501AC0', '_lbl_0C518694'], ["h'FF66BBFF", '_lbl_0C518698'], ['_lbl_0C510C3E', '_lbl_0C51869C']]);
INLINE_ASM(_AL_ShapeControl, "asm/nonmatching/Chao/al_shape/_AL_ShapeControl.src");

// MERGE_LIST([["h'0000FFF7", '_lbl_0C5187E0'], ["h'3DCCCCCD", '_lbl_0C5187E4'], ['_lbl_0C502340', '_lbl_0C5187E8']]);
INLINE_ASM(_AL_ShapeExit, "asm/nonmatching/Chao/al_shape/_AL_ShapeExit.src");

// MERGE_LIST([['_syFree', '_lbl_0C5187EC']]);
INLINE_ASM(_AL_ShapeChangeType, "asm/nonmatching/Chao/al_shape/_AL_ShapeChangeType.src");

