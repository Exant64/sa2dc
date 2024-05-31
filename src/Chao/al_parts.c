#include <Chao/Chao.h>
extern int PartsObjectTreeNumber[8][2];
extern int PartsObjectListNumber[8][2];

extern NJS_CNK_OBJECT** pPartsObjectListChild[];
extern NJS_CNK_OBJECT** pPartsObjectListAdult[];

Bool AL_IsExistPartsChild(int MinimalNum, int PartsKind) {
    if (pPartsObjectListChild[MinimalNum][PartsObjectListNumber[PartsKind][0]])
        return TRUE;

    return FALSE;
}

Bool AL_IsExistPartsAdult(int MinimalNum, int PartsKind) {
    if (pPartsObjectListAdult[MinimalNum][PartsObjectListNumber[PartsKind][0]])
        return TRUE;

    return FALSE;
}

void AL_ClearPartsSub(al_object* pObject) {
    if (!pObject)
        return;

    pObject->pPartsObject = NULL;

    if (pObject->pChild)
        AL_ClearPartsSub(pObject->pChild);

    if (!pObject->pSibling)
        return;

    AL_ClearPartsSub(pObject->pSibling);
}

void AL_ClearParts(task* tp) {
    chaowk* work = GET_CHAOWK(tp);
    int i;

    for (i = 0; i < 8; i++) {
        work->param.Parts.MinimalParts[i] = -1;
    }

    AL_ClearPartsSub(work->Shape.pObject);
}

int AL_GetMinimalPart(task* tp, int PartsKind) {
    chaowk* work = GET_CHAOWK(tp);
    return work->param.Parts.MinimalParts[PartsKind];
}

void AL_SetMiminalParts(task* tp, int PartsKind, int MinimalType) {
    chaowk* work = GET_CHAOWK(tp);
    int i;

    if (MinimalType >= 21)
        MinimalType = -1;

    if (MinimalType < 0) {
        work->param.Parts.MinimalParts[PartsKind] = -1;
    } else {
        work->param.Parts.MinimalParts[PartsKind] = MinimalType;
    }

    for (i = 0; i < 2; i++) {
        int tree = PartsObjectTreeNumber[PartsKind][i];
        int listNumber = PartsObjectListNumber[PartsKind][i];
        if (tree != -1) {
            if (MinimalType >= 0) {
                if (work->param.type == TYPE_CHILD) {
                    work->Shape.CurrObjectList[tree]->pPartsObject = pPartsObjectListChild[MinimalType][listNumber];
                } else {
                    work->Shape.CurrObjectList[tree]->pPartsObject = pPartsObjectListAdult[MinimalType][listNumber];
                }
            } else {
                work->Shape.CurrObjectList[tree]->pPartsObject = NULL;
            }
        }
    }
}

void AL_RemoveMinimalParts(task* tp, int PartsKind) {
    AL_SetMiminalParts(tp, PartsKind, -1);
}

void AL_SetObakeHead(task* tp, int HeadType) {
    CHAO_PARAM* Param = GET_CHAOPARAM(tp);
    Param->body.ObakeHead = HeadType;
}

void AL_RemoveObakeHead(task* tp) {
    CHAO_PARAM* Param = GET_CHAOPARAM(tp);
    Param->body.ObakeHead = 0;
}

///????
void AL_SetObakeHead_(task* tp, int HeadType) {
    CHAO_PARAM* Param = GET_CHAOPARAM(tp);
    Param->body.ObakeHead = HeadType;
}

void AL_RemoveObakeHead_(task* tp) {
    CHAO_PARAM* Param = GET_CHAOPARAM(tp);
    Param->body.ObakeHead = 0;
}

void AL_SetItem(task* tp, int TreeNum, NJS_CNK_OBJECT* pObject, NJS_TEXLIST* pTexlist) {
    chaowk* work = GET_CHAOWK(tp);

    if (work->Shape.CurrObjectList[TreeNum]->pItemObject != pObject) {
        work->Shape.CurrObjectList[TreeNum]->ItemScale = 0;
        work->Shape.CurrObjectList[TreeNum]->pItemObject = pObject;
    }
    work->Shape.CurrObjectList[TreeNum]->pItemTexlist = pTexlist;
    work->Shape.CurrObjectList[TreeNum]->ItemActiveFlag = TRUE;
    work->Shape.CurrObjectList[TreeNum]->ItemOffsetFlag = FALSE;
}

void AL_SetItemOffset(task* tp, int TreeNum, AL_ITEM_INFO* pInfo) {
    chaowk* work = GET_CHAOWK(tp);

    if (work->Shape.CurrObjectList[TreeNum]->pItemObject != pInfo->pObject) {
        work->Shape.CurrObjectList[TreeNum]->ItemScale = 0;
        work->Shape.CurrObjectList[TreeNum]->pItemObject = pInfo->pObject;
    }
    work->Shape.CurrObjectList[TreeNum]->pItemObject = pInfo->pObject;
    work->Shape.CurrObjectList[TreeNum]->pItemTexlist = pInfo->pTexlist;
    work->Shape.CurrObjectList[TreeNum]->ItemOffsetPos = pInfo->OffsetPos;
    work->Shape.CurrObjectList[TreeNum]->ItemOffsetAng = pInfo->OffsetAng;
    work->Shape.CurrObjectList[TreeNum]->ItemActiveFlag = TRUE;
    work->Shape.CurrObjectList[TreeNum]->ItemOffsetFlag = TRUE;
}

void AL_SetItemParallelLeftHand(task* tp, NJS_CNK_OBJECT* pObject, NJS_TEXLIST* pTexlist) {
    chaowk* work = GET_CHAOWK(tp);
    work->Shape.pLeftHandItemObject = pObject;
    work->Shape.pLeftHandItemTexlist = pTexlist;
    work->Shape.LeftHandItemScale = 0;
    work->Shape.LeftHandItemActiveFlag = 1;
}

void AL_SetItemParallelRightHand(task* tp, NJS_CNK_OBJECT* pObject, NJS_TEXLIST* pTexlist) {
    chaowk* work = GET_CHAOWK(tp);
    work->Shape.pRightHandItemObject = pObject;
    work->Shape.pRightHandItemTexlist = pTexlist;
    work->Shape.RightHandItemScale = 0;
    work->Shape.RightHandItemActiveFlag = 1;
}

void AL_RemoveItem(al_object* pObject) {
    pObject->ItemActiveFlag = FALSE;
}

void AL_RemoveItemAllSub(al_object* pObject) {
    if (!pObject)
        return;

    AL_RemoveItem(pObject);

    if (pObject->pChild)
        AL_RemoveItemAllSub(pObject->pChild);

    if (!pObject->pSibling)
        return;

    AL_RemoveItemAllSub(pObject->pSibling);
}

void AL_RemoveItemAll(task* tp) {
    chaowk* work = GET_CHAOWK(tp);

    AL_RemoveItemAllSub(work->Shape.pObject);

    work->Shape.LeftHandItemActiveFlag = FALSE;
    work->Shape.RightHandItemActiveFlag = FALSE;
}

void AL_ClearItem(al_object* pObject) {
    pObject->pItemObject = NULL;
    pObject->pItemTexlist = NULL;
    pObject->ItemScale = 0;
    pObject->ItemActiveFlag = FALSE;
}

void AL_ClearItemAllSub(al_object* pObject) {
    if (!pObject)
        return;

    AL_ClearItem(pObject);

    if (pObject->pChild)
        AL_ClearItemAllSub(pObject->pChild);

    if (!pObject->pSibling)
        return;

    AL_ClearItemAllSub(pObject->pSibling);
}

void AL_ClearItemAll(task* tp) {
    chaowk* work = GET_CHAOWK(tp);
    AL_ClearItemAllSub(work->Shape.pObject);
}

void AL_PartsInit(task* tp) {
    chaowk* v1 = GET_CHAOWK(tp);
    int i;

    for (i = 0; i < 8; i++) {
        AL_SetMiminalParts(tp, i, v1->param.Parts.MinimalParts[i]);
    }

    AL_ClearItemAll(tp);
}

void AL_PartsMinimalFlagOn(task* tp, int MinimalType) {
    chaowk* work = GET_CHAOWK(tp);
    work->param.Parts.MinimalFlag |= 1 << MinimalType;
}

void AL_PartsMinimalFlagOff(task* tp, int MinimalType) {
    chaowk* work = GET_CHAOWK(tp);
    work->param.Parts.MinimalFlag = 0;
}

Bool AL_PartsIsMinimalFlagOn(task* tp, int MinimalType) {
    chaowk* work = GET_CHAOWK(tp);
    if (work->param.Parts.MinimalFlag & (1 << MinimalType))
        return TRUE;

    return FALSE;
}
