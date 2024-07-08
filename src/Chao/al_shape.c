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

INLINE_ASM(_func_0C517A6C, 0x1d4, "asm/nonmatching/Chao/al_shape/_func_0C517A6C.src");

INLINE_ASM(_AL_CopyChaoObject, 0x28c, "asm/nonmatching/Chao/al_shape/_AL_CopyChaoObject.src");

// MERGE_LIST([['__divls', '_lbl_0C517F20'], ['_syCalloc', '_lbl_0C517F24'], ['_njMemCopy2', '_lbl_0C517F28']]);
INLINE_ASM(_AL_CalcBuyoWeight, 0xc0, "asm/nonmatching/Chao/al_shape/_AL_CalcBuyoWeight.src");

// MERGE_LIST([['_njScalor', '_lbl_0C5180C0']]);
float lbl_0C517F8C(al_object* pObject, al_object* pParent, Float dist) {
    Float weight1;
	Float weight2;
	Float weight3;

    Float outweight;

    if(pParent) {
        weight1 = pParent->pBuyoParam->Weight1;
		weight2 = pParent->pBuyoParam->Weight2;
		weight3 = pParent->pBuyoParam->Weight3;
    } else {
        weight1 = pObject->pBuyoParam->Weight1;
		weight2 = pObject->pBuyoParam->Weight2;
		weight3 = pObject->pBuyoParam->Weight3;
    }

    if (dist < pObject->pBuyoParam->Weight1) {
        outweight = weight2;
    }
    else {
        outweight = weight2 + ((dist - weight1) * weight3);
    }

    return outweight;
}

extern int NullMergeTreeList[];
Bool lbl_0C517FCE(int index) {
    int* p = NullMergeTreeList;
    while(*p != -1) {
        if(*p == index) return TRUE;

        p++;
    }
    
    return FALSE;
}

MERGE_LIST([['_NullMergeTreeList', '_lbl_0C5180C4']]);
INLINE_ASM(_AL_CalcBuyoParam, 0x312, "asm/nonmatching/Chao/al_shape/_AL_CalcBuyoParam.src");

#ifndef NONMATCHING
INLINE_ASM(_AL_CreateOrgVertexList, 0x120, "asm/nonmatching/Chao/al_shape/_AL_CreateOrgVertexList.src");
#else
extern short tree_count;
// it's matching, but it uses JSR instead of relative branch because of the inline asms between it and FreeChaoObj
Bool AL_CreateOrgVertexList(al_object *pObject) {
    al_model* pModel = pObject->pModel;
    if(pModel) {
        Sint16* vlist = (Uint16*)((int)pModel->VList + 6);
        Sint32 nbVertex = *(vlist++);
        CNK_VN_VERTEX* pVertex = (CNK_VN_VERTEX*)vlist;

        NJS_POINT3* pPos;
        NJS_POINT3* pNorm;
        AL_VERTEX_INFO* pVertInfo;

        pModel->nbVertex = nbVertex;
        pModel->pVertex = pVertex;

        pPos = (NJS_POINT3*)syCalloc(nbVertex, sizeof(NJS_POINT3));
        if(!pPos) {
            AL_FreeChaoObject(pObject);
            return FALSE;
        }

        pNorm = (NJS_POINT3*)syCalloc(nbVertex, sizeof(NJS_VECTOR));
        if(!pNorm) {
            AL_FreeChaoObject(pObject);
            return FALSE;
        }

        pVertInfo = (AL_VERTEX_INFO*)syCalloc(nbVertex, sizeof(AL_VERTEX_INFO));
        if(!pVertInfo) {
            syFree(pPos);
			syFree(pNorm);
            AL_FreeChaoObject(pObject);
            return FALSE;
        }

        pModel->pOrgVertexPos = pPos;
        pModel->pOrgVertexNormal = pNorm;
        pModel->pVertexInfo = pVertInfo;

        {
            CNK_VN_VERTEX* end = &pVertex[nbVertex];
            while(pVertex < end) {
                pVertInfo->Velo.x = 0;
                pVertInfo->Velo.y = 0;
                pVertInfo->Velo.z = 0;

                *pPos = pVertex->Pos;
                *pNorm = pVertex->Normal;
                ++pVertex;
                ++pNorm;

                pPos->x *= 1;
                pPos->y *= 1;
                pPos->z *= 1;

                ++pPos;
                
                ++pVertInfo;
            }
        }
    }

    tree_count++;

    if(pObject->pChild) {
        AL_CreateOrgVertexList(pObject->pChild);
    }

    if(pObject->pSibling) {
        AL_CreateOrgVertexList(pObject->pSibling);
    }

    return TRUE;
}
#endif

#if 1
INLINE_ASM(_AL_ShapeInit, 0x218, "asm/nonmatching/Chao/al_shape/_AL_ShapeInit.src");
#else

extern int PartsNumber;
extern NJS_CNK_OBJECT* AL_RootObject[];
Bool AL_ShapeInit(task* tp)
{
	chaowk* twp = (chaowk*)tp->twp;
	AL_SHAPE* asp = &twp->Shape;
	CHAO_PARAM* pParam = &twp->param;
    Sint32 type;

	tree_count = 0;
	PartsNumber = 0;

	type = pParam->type - 2;

	asp->pObject = (al_object*)AL_CopyChaoObject(AL_RootObject[6 * type], AL_RootObject[2], 0);
	
	if (!asp->pObject)
		return FALSE;

	AL_GetObjectList(asp->pObject, asp->CurrObjectList);

	asp->pObjectList = (AL_GROUP_OBJECT_LIST*)syCalloc(1, sizeof(AL_GROUP_OBJECT_LIST));

	if (!asp->pObjectList)
		return FALSE;

	AL_GetObjectList((al_object*)AL_RootObject[6 * type + 0], (al_object**)asp->pObjectList->Zero);
	AL_GetObjectList((al_object*)AL_RootObject[6 * type + 1], (al_object**)asp->pObjectList->Normal);
	AL_GetObjectList((al_object*)AL_RootObject[6 * type + 2], (al_object**)asp->pObjectList->Swim);
	AL_GetObjectList((al_object*)AL_RootObject[6 * type + 3], (al_object**)asp->pObjectList->Fly);
	AL_GetObjectList((al_object*)AL_RootObject[6 * type + 4], (al_object**)asp->pObjectList->Run);
	AL_GetObjectList((al_object*)AL_RootObject[6 * type + 5], (al_object**)asp->pObjectList->Power);

	asp->pObjectListH = (AL_GROUP_OBJECT_LIST*)syCalloc(1, sizeof(AL_GROUP_OBJECT_LIST));

	if (!asp->pObjectListH)
		return FALSE;

	AL_GetObjectList((al_object*)AL_RootObject[6], (al_object**)asp->pObjectListH->Zero);
	AL_GetObjectList((al_object*)AL_RootObject[7], (al_object**)asp->pObjectListH->Normal);
	AL_GetObjectList((al_object*)AL_RootObject[8], (al_object**)asp->pObjectListH->Swim);
	AL_GetObjectList((al_object*)AL_RootObject[9], (al_object**)asp->pObjectListH->Fly);
	AL_GetObjectList((al_object*)AL_RootObject[10], (al_object**)asp->pObjectListH->Run);
	AL_GetObjectList((al_object*)AL_RootObject[11], (al_object**)asp->pObjectListH->Power);

	asp->pObjectListD = (AL_GROUP_OBJECT_LIST*)syCalloc(1, sizeof(AL_GROUP_OBJECT_LIST));

	if (!asp->pObjectListD)
		return FALSE;

	AL_GetObjectList((al_object*)AL_RootObject[12], (al_object**)asp->pObjectListD->Zero);
	AL_GetObjectList((al_object*)AL_RootObject[13], (al_object**)asp->pObjectListD->Normal);
	AL_GetObjectList((al_object*)AL_RootObject[14], (al_object**)asp->pObjectListD->Swim);
	AL_GetObjectList((al_object*)AL_RootObject[15], (al_object**)asp->pObjectListD->Fly);
	AL_GetObjectList((al_object*)AL_RootObject[16], (al_object**)asp->pObjectListD->Run);
	AL_GetObjectList((al_object*)AL_RootObject[17], (al_object**)asp->pObjectListD->Power);

	AL_CreateOrgVertexList(asp->pObject);
	AL_Deform(tp);
	AL_CalcBuyoParam(tp);
	AL_SetCurrMatrix(tp);
	asp->IconColor = 0xFF66BBFF;
	AL_MaterialInit(tp);

    return TRUE;
}
#endif
//MERGE_LIST([['_PartsNumber', '_lbl_0C518680'], ['_tree_count', '_lbl_0C518684'], ['_AL_RootObject', '_lbl_0C518688'], ['_syCalloc', '_lbl_0C51868C'], ['_AL_Deform', '_lbl_0C518690'], ['_AL_SetCurrMatrix', '_lbl_0C518694'], ["h'FF66BBFF", '_lbl_0C518698'], ['_lbl_0C510C3E', '_lbl_0C51869C']]);
INLINE_ASM(_AL_ShapeControl, 0x12e, "asm/nonmatching/Chao/al_shape/_AL_ShapeControl.src");

// MERGE_LIST([["h'0000FFF7", '_lbl_0C5187E0'], ["h'3DCCCCCD", '_lbl_0C5187E4'], ['_AL_BuyoBuyoControl', '_lbl_0C5187E8']]);
INLINE_ASM(_AL_ShapeExit, 0x46, "asm/nonmatching/Chao/al_shape/_AL_ShapeExit.src");

// MERGE_LIST([['_syFree', '_lbl_0C5187EC']]);
INLINE_ASM(_AL_ShapeChangeType, 0xb4, "asm/nonmatching/Chao/al_shape/_AL_ShapeChangeType.src");
