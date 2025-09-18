#include <task.h>
#include <Chao/Chao.h>

extern int PartsNumber;
extern AL_BUYO_PARAM BuyoParamFly[];
extern short tree_count;
extern NJS_CNK_OBJECT* AL_RootObject[];
extern int NullMergeTreeList[];

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

#define SHAPE_FREE(a1) \
    if (a1) {          \
        syFree(a1);    \
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

    if (pObject->pOldMatrix) {
        syFree(pObject->pOldMatrix);
    }

    syFree(pObject);
}

#define SetNext(a1) *(pSet++) = a1;
#define Chunk ((Uint8)cnk)
void func_0C517A6C(Sint16* plist2, Uint32 a2) {
    Sint16* plist = plist2;
    Sint16* pSet = plist2 + a2;

    Sint16* pSize;
    Sint16* pIndexCount;

    Sint16* pStart;

    if ((Uint32)pSet & 3) {
        SetNext(0);
    }

    SetNext(0x2111);
    SetNext(0x2);
    SetNext(0xC8C8);
    SetNext(0x50C8);
    SetNext(0xE408);
    SetNext(0x4009);
    SetNext(0x4B40);

    pStart = pSet;

    *(pSet++) = 1; // SetNext(1);
    *(pSet++) = 0;
    pSize = pStart;
    pIndexCount = pStart + 1;

    while (1) {
        int cnk;

        do {
            cnk = *plist;

            if (Chunk < NJD_BITSOFF) {
                plist += 1;
            }
            else if (Chunk < NJD_TINYOFF) {
                plist += 1;
            }
            else if (Chunk < NJD_MATOFF) {
                plist += 2;
            }
            else if (Chunk < NJD_VERTOFF) {
                plist += 2;

                switch (Chunk) {
                    case NJD_CM_D:
                    case NJD_CM_A:
                    case NJD_CM_S:
                        plist += 2;
                        break;
                    case NJD_CM_DA:
                    case NJD_CM_DS:
                    case NJD_CM_AS:
                        plist += 4;
                        break;
                    case NJD_CM_DAS:
                        plist += 6;
                        break;
                }
            }
            else if (Chunk < NJD_VOLOFF) {
            }
            else if (Chunk < NJD_STRIPOFF) {
            }
            else break;

        } while (1);

        if (Chunk < 0xFF) {
            Uint16 size;
            Uint16 thing2;

            // plist++;
            size = *++plist;

            // plist++;
            thing2 = (*++plist) & 0x3FFF;

            plist++;
            *pSize += (((thing2 * 2 + size) - 1) / 3);
            *pIndexCount += thing2;

            while (thing2--) {
                Sint16 count = *(plist++);
                SetNext(count);
                if (count < 0)
                    count = -count;

                while (count--) {
                    *(pSet++) = *(plist++);
                    // pSet++;
                    plist += 2;
                }
            }

            cnk = *plist;
            if (Chunk == NJD_NULLOFF) {
                plist++;
                cnk = *plist;
            }

            if (Chunk == NJD_ENDOFF) {
                break;
            }
        }
    }

    SetNext(0xFF);
    SetNext(0xFF);
}

al_object* AL_CopyChaoObject(NJS_CNK_OBJECT* pSrcObject, NJS_CNK_OBJECT* pChildObject, al_object* pParentObject) {
    al_object* pObject;

    if (!pSrcObject)
        return NULL;

    pObject = (al_object*)syCalloc(1, sizeof(al_object));

    if (!pObject)
        return NULL;

    pObject->pOldMatrix = (NJS_MATRIX_PTR)syCalloc(1, sizeof(NJS_MATRIX));
    njMemCopy(pObject, pSrcObject, sizeof(NJS_CNK_OBJECT));

    pObject->OrgPos.x = pSrcObject->pos[0];
    pObject->OrgPos.y = pSrcObject->pos[1];
    pObject->OrgPos.z = pSrcObject->pos[2];
    pObject->diff.x = pSrcObject->pos[0] - pChildObject->pos[0];
    pObject->diff.y = pSrcObject->pos[1] - pChildObject->pos[1];
    pObject->diff.z = pSrcObject->pos[2] - pChildObject->pos[2];

    if (pSrcObject->model) {
        NJS_CNK_MODEL* pSrcModel = pSrcObject->model;
        al_model* pModel = (al_model*)syCalloc(1, sizeof(al_model));
        int i;

        if (!pModel) {
            AL_FreeChaoObject(pObject);
            return NULL;
        }

        njMemCopy(pModel, pSrcObject->model, sizeof(NJS_CNK_MODEL));

        pObject->pModel = pModel;

        pModel->OrgTexId[0] = -1;
        pModel->OrgTexId[1] = -1;
        pModel->OrgTexId[2] = -1;
        pModel->OrgTexId[3] = -1;

        pModel->Radius = 0.0f;

        if (pSrcModel->vlist) {
            Sint32* vlist = pSrcModel->vlist;
            Sint32* vlist2;
            Uint32 length = 1;

            while (*(vlist++) != (Uint8)NJD_ENDOFF) {
                ++length;
            }

            vlist = pSrcModel->vlist;
            vlist2 = (Sint32*)syMalloc(sizeof(Sint32) * length);

            if (!vlist2) {
                AL_FreeChaoObject(pObject);
                return NULL;
            }

            njMemCopy4(vlist2, vlist, length);
            pModel->VList = vlist2;

            pObject->pBuyoParam = &BuyoParamFly[PartsNumber++];
        }

        if (pSrcModel->plist) {
            Sint16* plist = pSrcModel->plist;
            Sint16* plist2;
            Uint32 length2;
            Uint32 length;

            length2 = 0;
            length = 0;

            while (TRUE) {
                Uint8 cnk = *plist;

                if (cnk < NJD_BITSOFF) {
                    length2++;
                    length++;

                    plist++;
                }

                else if (cnk < NJD_TINYOFF) {
                    length2++;
                    length++;

                    plist++;
                } else if ((cnk) < NJD_MATOFF) {
                    length2 += 2;
                    length += 2;

                    plist += 2;
                } else if (cnk < NJD_VERTOFF) {
                    length2 += 2;
                    length += 2;

                    plist += 2;
                    switch (cnk) {
                        case NJD_CM_D:
                        case NJD_CM_A:
                        case NJD_CM_S:
                            length2 += 2;
                            length += 2;

                            plist += 2;
                            break;
                        case NJD_CM_DA:
                        case NJD_CM_DS:
                        case NJD_CM_AS:
                            length2 += 4;
                            length += 4;

                            plist += 4;
                            break;
                        case NJD_CM_DAS:
                            length2 += 6;
                            length += 6;

                            plist += 6;
                            break;
                    }
                } else if (cnk < NJD_VOLOFF) {
                } else if (cnk < NJD_STRIPOFF) {
                } else if (cnk < (Uint8)-1) {
                    Uint16 size;
                    Uint16 thing2;

                    plist++;
                    size = *plist;

                    plist++;
                    thing2 = (*plist) & 0x3FFF;

                    plist += size;

                    length2 += size + 2;
                    length += size + 2;

                    length += (((thing2 * 2 + size) - 1) / 3) + 3; // 3 + (thing2 * 2 + (size-1)) / 3;
                } else {
                    length2 += 1;
                    length += 1;
                    length += 0x16;
                    break;
                }
            }

            plist = pSrcModel->plist;
            plist2 = (Sint16*)syCalloc(length, sizeof(Sint16));

            if (!plist2) {
                AL_FreeChaoObject(pObject);
                return NULL;
            }

            njMemCopy2(plist2, plist, length2);
            pModel->PList = plist2;
            pModel->PListSize = length2;
        } else {
            pModel->PList = NULL;
        }
    }

    pObject->pParent = pParentObject;

    if (pSrcObject->child)
        pObject->pChild = AL_CopyChaoObject(pSrcObject->child, pChildObject->child, pObject);

    if (pSrcObject->sibling)
        pObject->pSibling = AL_CopyChaoObject(pSrcObject->sibling, pChildObject->sibling, pParentObject);

    return pObject;
}

float AL_CalcBuyoWeight(al_object* pObject, al_object* pParent, al_object* pNullParent, float Dist) {
    Float weight1;
    Float weight2;
    Float weight3;
    Float outweight;

    if (pParent) {
        weight3 = pParent->pBuyoParam->Weight3;

        if (pNullParent) {
            weight2 = AL_CalcBuyoWeight(pParent, NULL, NULL, njScalor(pNullParent->Pos));
        } else {
            weight2 = AL_CalcBuyoWeight(pParent, NULL, NULL, njScalor(pObject->Pos));
        }
    } else {
        weight1 = pObject->pBuyoParam->Weight1;
        weight2 = pObject->pBuyoParam->Weight2;
        weight3 = pObject->pBuyoParam->Weight3;
    }

    // there's definitely/most likely an if(0) here, this is what i came up with to manipulate the registers to fix
    // themselves not sure if it qualifies as fake match
    if (0)
        pObject = NULL;

    if (Dist < pObject->pBuyoParam->Weight1) {
        outweight = weight2;
    } else {
        outweight = weight2 + ((Dist - weight1) * weight3);
    }

    return outweight;
}

float AL_CalcBuyoWeightMouth(al_object* pObject, al_object* pParent, al_object* pNullParent, Float dist) {
    Float weight1;
    Float weight2;
    Float weight3;

    Float outweight;

    if (pParent) {
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
    } else {
        outweight = weight2 + ((dist - weight1) * weight3);
    }

    return outweight;
}

Bool lbl_0C517FCE(int index) {
    int* p = NullMergeTreeList;
    while (*p != -1) {
        if (*p == index)
            return TRUE;

        p++;
    }

    return FALSE;
}

void AL_CalcBuyoParam(task* tp) {
    AL_SHAPE* pShape = &GET_CHAOWK(tp)->Shape;
    al_object** objectList = pShape->CurrObjectList;
    int i;

    for (i = 0; i < 40; i++, objectList++) {
        al_object* pObject = *objectList;
        al_model* pModel = pObject->pModel;

        if (pModel) {
            NJS_POINT3* pOrgVertexPos = pModel->pOrgVertexPos;
            AL_VERTEX_INFO* pVertexInfo = pModel->pVertexInfo;
            int nbVertex = pModel->nbVertex;
            CNK_VN_VERTEX* pVertex = pModel->pVertex;
            CNK_VN_VERTEX* pVertexEnd = pVertex + nbVertex;
            float Spring1 = pObject->pBuyoParam->Spring1;
            float Spring2 = pObject->pBuyoParam->Spring2;
            float Friction1 = pObject->pBuyoParam->Friction1;
            float Friction2 = pObject->pBuyoParam->Friction2;

            if (lbl_0C517FCE(i) && pObject->pParent) {
                NJS_POINT3* pCenter;
                al_object* pParent1 = pObject->pParent;
                al_object* pParent2 = NULL;

                if (!pParent1->pModel) {
                    pParent2 = pParent1;
                    pParent1 = pParent1->pParent;
                }

                if (pParent1->pBuyoParam)
                    pCenter = &pParent1->pBuyoParam->Center;
                else
                    pCenter = NULL;

                while (pVertex < pVertexEnd) {
                    float dist;
                    const NJS_POINT3 zero = { 0, 0, 0 };
                    NJS_POINT3 VertexPos;

                    njPushMatrix(&_nj_unit_matrix_);
                    if (pParent2) {
                        njTranslateEx((NJS_VECTOR*)pParent2->Pos);
                        njRotateXYZ(0, pParent2->Ang[0], pParent2->Ang[1], pParent2->Ang[2]);
                        if (!(pParent2->EvalFlags & 4))
                            njScale(0, pParent2->Scl[0], pParent2->Scl[1], pParent2->Scl[2]);
                    }
                    njTranslateEx((NJS_VECTOR*)pObject->Pos);
                    njRotateXYZ(0, pObject->Ang[0], pObject->Ang[1], pObject->Ang[2]);
                    njCalcPoint(0, pOrgVertexPos, &VertexPos);
                    njPopMatrixEx();

                    if (pCenter)
                        dist = njDistanceP2P(pCenter, &VertexPos);
                    else
                        dist = njScalor(&VertexPos);

                    if (i == AL_PART_MOUTH) {
                        pVertexInfo->Weight = AL_CalcBuyoWeightMouth(pObject, pParent1, pParent2, dist);
                    } else {
                        pVertexInfo->Weight = AL_CalcBuyoWeight(pObject, pParent1, pParent2, dist);
                    }

                    pVertexInfo->Spring = Spring1 + (Spring2 * pVertexInfo->Weight);
                    pVertexInfo->Spring *= 0.75f;

                    pVertexInfo->Friction = Friction1 + (Friction2 * pVertexInfo->Weight);

                    if (pVertexInfo->Friction > 0.999f)
                        pVertexInfo->Friction = 0.999f;
                    pVertexInfo->Friction = (1.0f - ((1.0f - pVertexInfo->Friction) * 0.6f));

                    ++pOrgVertexPos;
                    ++pVertexInfo;
                    ++pVertex;
                }
            } else {
                NJS_POINT3* pCenter = &pObject->pBuyoParam->Center;
                while (pVertex < pVertexEnd) {
                    Float dist = njDistanceP2P(pCenter, pOrgVertexPos);

                    pVertexInfo->Weight = AL_CalcBuyoWeight(pObject, NULL, NULL, dist);

                    pVertexInfo->Spring = Spring1 + (Spring2 * pVertexInfo->Weight);
                    pVertexInfo->Spring *= 0.75f;

                    pVertexInfo->Friction = Friction1 + (Friction2 * pVertexInfo->Weight);
                    if (pVertexInfo->Friction > 0.999f)
                        pVertexInfo->Friction = 0.999f;

                    pVertexInfo->Friction = (1.0f - ((1.0f - pVertexInfo->Friction) * 0.6f));

                    ++pOrgVertexPos;
                    ++pVertexInfo;
                    ++pVertex;
                }
            }
        } else {
            if (i == AL_PART_EYE_L_POS)
                ;
            else if (i == AL_PART_EYE_R_POS)
                ;
            if (0)
                ;
        }
    }
}

Bool AL_CreateOrgVertexList(al_object* pObject) {
    al_model* pModel = pObject->pModel;
    if (pModel) {
        Sint16* vlist = (Uint16*)((int)pModel->VList + 6);
        Sint32 nbVertex = *(vlist++);
        CNK_VN_VERTEX* pVertex = (CNK_VN_VERTEX*)vlist;

        NJS_POINT3* pPos;
        NJS_POINT3* pNorm;
        AL_VERTEX_INFO* pVertInfo;

        pModel->nbVertex = nbVertex;
        pModel->pVertex = pVertex;

        pPos = (NJS_POINT3*)syCalloc(nbVertex, sizeof(NJS_POINT3));
        if (!pPos) {
            AL_FreeChaoObject(pObject);
            return FALSE;
        }

        pNorm = (NJS_POINT3*)syCalloc(nbVertex, sizeof(NJS_VECTOR));
        if (!pNorm) {
            AL_FreeChaoObject(pObject);
            return FALSE;
        }

        pVertInfo = (AL_VERTEX_INFO*)syCalloc(nbVertex, sizeof(AL_VERTEX_INFO));
        if (!pVertInfo) {
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
            while (pVertex < end) {
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

    if (pObject->pChild) {
        AL_CreateOrgVertexList(pObject->pChild);
    }

    if (pObject->pSibling) {
        AL_CreateOrgVertexList(pObject->pSibling);
    }

    return TRUE;
}

Bool AL_ShapeInit(task* tp) {
    chaowk* twp = GET_CHAOWK(tp);
    AL_SHAPE* asp = &twp->Shape;
    CHAO_PARAM* pParam = &twp->param;
    int index;

    PartsNumber = 0;
    tree_count = 0;

    index = 6 * (twp->param.type - 2);
    asp->pObject = (al_object*)AL_CopyChaoObject(AL_RootObject[index], AL_RootObject[2], NULL);

    if (!asp->pObject)
        return FALSE;

    AL_GetObjectList(asp->pObject, asp->CurrObjectList);

    asp->pObjectList = (AL_GROUP_OBJECT_LIST*)syCalloc(1, sizeof(AL_GROUP_OBJECT_LIST));

    if (!asp->pObjectList)
        return FALSE;

    AL_GetObjectList((al_object*)AL_RootObject[index++], (al_object**)asp->pObjectList->Zero);
    AL_GetObjectList((al_object*)AL_RootObject[index++], (al_object**)asp->pObjectList->Normal);
    AL_GetObjectList((al_object*)AL_RootObject[index++], (al_object**)asp->pObjectList->Swim);
    AL_GetObjectList((al_object*)AL_RootObject[index++], (al_object**)asp->pObjectList->Fly);
    AL_GetObjectList((al_object*)AL_RootObject[index++], (al_object**)asp->pObjectList->Run);
    AL_GetObjectList((al_object*)AL_RootObject[index++], (al_object**)asp->pObjectList->Power);

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

void AL_ShapeControl(task* tp) {
    chaowk* work = GET_CHAOWK(tp);
    AL_SHAPE* pShape = &work->Shape;
    CHAO_PARAM* pParam = &work->param;

    if (pShape->Flag & AL_SHAPE_FLAG_DEFORM) {
        AL_Deform(tp);
        AL_CalcBuyoParam(tp);
    }

    if (AL_IsHitKindWithNum(tp, 1, CI_KIND_AL_SHADOW))
        pShape->Flag |= AL_SHAPE_FLAG_SHADOW;
    else
        pShape->Flag &= ~AL_SHAPE_FLAG_SHADOW;

    if (pShape->pLeftHandItemObject) {
        if (pShape->LeftHandItemActiveFlag) {
            pShape->LeftHandItemScale += 0.1f;

            if (pShape->LeftHandItemScale > 1.0f)
                pShape->LeftHandItemScale = 1.0f;
        } else {
            pShape->LeftHandItemScale -= 0.1f;

            if (pShape->LeftHandItemScale < 0.0f) {
                pShape->LeftHandItemScale = 0.0f;
                pShape->pLeftHandItemObject = NULL;
            }
        }
    }

    if (pShape->pRightHandItemObject) {
        if (pShape->RightHandItemActiveFlag) {
            pShape->RightHandItemScale += 0.1f;

            if (pShape->RightHandItemScale > 1.0f)
                pShape->RightHandItemScale = 1.0f;
        } else {
            pShape->RightHandItemScale -= 0.1f;

            if (pShape->RightHandItemScale < 0.0f) {
                pShape->RightHandItemScale = 0.0f;
                pShape->pRightHandItemObject = NULL;
            }
        }
    }

    AL_BuyoBuyoControl(tp);
}

void AL_ShapeExit(task* tp) {
    AL_SHAPE* pShape = &GET_CHAOWK(tp)->Shape;

    if (pShape->pObjectList)
        syFree(pShape->pObjectList);

    if (pShape->pObjectListH)
        syFree(pShape->pObjectListH);

    if (pShape->pObjectListD)
        syFree(pShape->pObjectListD);

    // no palettefree
    AL_FreeChaoObject(pShape->pObject);
}

Bool AL_ShapeChangeType(task* tp, Uint32 type) {
    CHAO_PARAM* pParam = GET_CHAOPARAM(tp);

    // note, this is only sa2dc where chracter chao don't exist yet
    if (type >= TYPE_TAILS)
        return FALSE;

    switch (type) {
        case TYPE_NONE:
        case TYPE_EGG:
        case TYPE_DUMMY1:
        case TYPE_DUMMY2:
            return FALSE;
    }

    AL_ShapeExit(tp);

    pParam->type = type;

    AL_ShapeInit(tp);
    AL_FaceInit(tp);
    AL_IconInit(tp);
    AL_PartsInit(tp);
    AL_PerceptionInit(tp);

    return TRUE;
}