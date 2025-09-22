#include <task.h>
#include <Chao/Chao.h>

INLINE_ASM(_rodata, "asm/nonmatching/Chao/al_material/rodata.src");

enum JewelColor {
    JewelColor_Normal = 0x0,
    JewelColor_Gold = 0x1,
    JewelColor_Silver = 0x2,
    JewelColor_Ruby = 0x3,
    JewelColor_Sapphire = 0x4,
    JewelColor_Emerald = 0x5,
    JewelColor_Amethyst = 0x6,
    JewelColor_Aquamarine = 0x7,
    JewelColor_Garnet = 0x8,
    JewelColor_Onyx = 0x9,
    JewelColor_Peridot = 0xA,
    JewelColor_Topaz = 0xB,
    JewelColor_Pearl = 0xC,
    JewelColor_Env0 = 0xD,
    JewelColor_Env1 = 0xE,
    JewelColor_Env2 = 0xF,
    JewelColor_Env3 = 0x10,
    JewelColor_Env4 = 0x11,
};

typedef struct UV_SCROLL_INFO {
    NJS_CNK_OBJECT* pOrgObject;
    NJS_CNK_OBJECT* pCopyObject;
    float offsetU;
    float offsetV;
    float addU;
    float addV;
} UV_SCROLL_INFO;

extern Sint16 lbl_0C56F674[];
extern Uint16 lbl_0C56F654[];
extern const Uint32 ChaoColorList[];
extern Sint16 ChaoColSetList[];
extern Sint16 EnvSetList[];

void AL_MatChangeModelTexture(al_model* pModel, Uint16 TexId);
void AL_MatResetModelTexture(al_model* pModel);
void AL_MatChangeModelTexture2(al_model* pModel, const Uint16* TexIdList, int nbTex);
void AL_MatChangeModelTextureCount(al_model* pModel, Uint16 Count, Uint16 TexId);
void AL_MatSwitchModelEnvironmentMapping(al_model* pModel, Bool Enable);
void AL_MatSetChaoMultiOff(task* tp);

void AL_MatSetModelColor(al_object* pObject, Uint32 Color) {
    if (pObject && pObject->pModel) {
        Uint32* plist = pObject->pModel->PList;

        if (plist) {
            plist++;
            *(Uint32*)plist = Color;
        }
    }
}

void AL_MatSetChaoColor(task* tp, Sint16 ColorNum) {
    taskwk* twork = GET_CHAOWK(tp);
    chaowk* work = GET_CHAOWK(tp);
    AL_SHAPE* pShape = &work->Shape;
    al_object** pObjectList = pShape->CurrObjectList;
    CHAO_PARAM* pParam = GET_CHAOPARAM(tp);
    Sint16* pSetList = ChaoColSetList;
    Uint32 Color = ChaoColorList[ColorNum];
    int i;

    pParam->body.ColorNum = ColorNum;

    for (i = 0; i < 40; i++) {
        if (*pSetList == -1) {
            break;
        }

        if (*pSetList == i) {
            AL_MatSetModelColor(*pObjectList, Color);
            pSetList++;
        }

        pObjectList++;
    }
}

void AL_MatChangeObjectTexture(al_object* pObject, Uint16 TexId) {
    if (pObject) {
        if (pObject->pModel) {
            AL_MatChangeModelTexture(pObject->pModel, TexId);
        }
    }
}

void AL_MatChangeModelTexture(al_model* pModel, Uint16 TexId) {
    int i = 0;

    if (pModel) {
        Sint16* plist = pModel->PList;

        if (!plist)
            return;

        while (1) {
            Uint8 cnk = *plist;

            if (cnk < NJD_BITSOFF) {
                plist += 1;
            } else if (cnk < NJD_TINYOFF) {
                plist += 1;
            } else if (cnk < NJD_MATOFF) {
                switch (cnk) {
                    case 8: {
                        Uint16 tid;

                        plist++;
                        tid = *plist;

                        if (i >= 4) {
                        } else if (pModel->OrgTexId[i] < 0) {
                            pModel->OrgTexId[i] = tid & 0x1FFF;
                            i++;
                        }

                        tid &= 0xC000;
                        tid |= (TexId & 0x1FFF);

                        *plist = tid;

                        plist++;
                        break;
                    }
                    default: {
                        plist += 2;
                        // WARNING: FAKE?
                        if (0)
                            ;
                        break;
                    }
                }
            } else if (cnk < NJD_VERTOFF) {
                plist += 2;

                switch (cnk) {
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
            } else if (cnk < NJD_VOLOFF) {
            } else if (cnk < NJD_STRIPOFF) {
            } else if (cnk < NJD_ENDOFF) {
                Uint16 size;
                plist++;
                size = *(plist) + 1;

                plist += size;
            } else {
                break;
            }
        }
    }
}

void AL_MatResetObjectTexture(al_object* pObject) {
    if (pObject) {
        if (pObject->pModel) {
            AL_MatResetModelTexture(pObject->pModel);
        }
    }
}

void AL_MatResetModelTexture(al_model* pModel) {
    int i = 0;

    if (pModel) {
        Sint16* plist = pModel->PList;

        if (plist)
            while (1) {
                Uint8 cnk = *plist;

                if (cnk < NJD_BITSOFF) {
                    plist += 1;
                } else if (cnk < NJD_TINYOFF) {
                    plist += 1;
                } else if (cnk < NJD_MATOFF) {
                    switch (cnk) {
                        case 8: {
                            Uint16 tid;

                            plist++;
                            tid = *plist;

                            if (i >= 4) {
                            } else if (pModel->OrgTexId[i] >= 0) {
                                tid &= 0xC000;
                                tid |= pModel->OrgTexId[i] & 0x1FFF;

                                i++;
                            }

                            *plist = tid;
                            plist++;
                            break;
                        }
                        default: {
                            plist += 2;
                            break;
                        }
                    }
                } else if (cnk < NJD_VERTOFF) {
                    plist += 2;

                    switch (cnk) {
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
                } else if (cnk < NJD_VOLOFF) {
                } else if (cnk < NJD_STRIPOFF) {
                } else if (cnk < NJD_ENDOFF) {
                    Uint16 size;
                    plist++;
                    size = *(plist) + 1;

                    plist += size;
                } else {
                    break;
                }
            }
    }
}

void AL_MatChangeObjectTexture2(al_object* pObject, const Uint16* TexIdList, int nbTex) {
    if (pObject && pObject->pModel) {
        AL_MatChangeModelTexture2(pObject->pModel, TexIdList, nbTex);
    }
}

void AL_MatChangeModelTexture2(al_model* pModel, const Uint16* TexIdList, int nbTex) {
    int i = 0;

    if (pModel) {
        Sint16* plist = pModel->PList;

        if (!plist)
            return;

        while (1) {
            Uint8 cnk = *plist;

            if (cnk < NJD_BITSOFF) {
                plist += 1;
            } else if (cnk < NJD_TINYOFF) {
                plist += 1;
            } else if (cnk < NJD_MATOFF) {
                switch (cnk) {
                    case 8: {
                        Uint16 tid;

                        plist++;
                        tid = *plist;

                        tid &= 0xC000;
                        tid |= (*TexIdList & 0x1FFF);

                        TexIdList++;

                        *plist = tid;

                        --nbTex;
                        if (!nbTex)
                            return;

                        plist++;
                        break;
                    }
                    default: {
                        plist += 2;
                        break;
                    }
                }
            } else if (cnk < NJD_VERTOFF) {
                plist += 2;

                switch (cnk) {
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
            } else if (cnk < NJD_VOLOFF) {
            } else if (cnk < NJD_STRIPOFF) {
            } else if (cnk < NJD_ENDOFF) {
                Uint16 size;
                plist++;
                size = *(plist) + 1;

                plist += size;
            } else {
                break;
            }
        }
    }
}

void AL_MatChangeObjectTextureCount(al_object* pObject, Uint16 Count, Uint16 TexId) {
    if (pObject && pObject->pModel) {
        AL_MatChangeModelTextureCount(pObject->pModel, Count, TexId);
    }
}

void AL_MatChangeModelTextureCount(al_model* pModel, Uint16 Count, Uint16 TexId) {
    int i = 0;

    if (pModel) {
        Sint16* plist = pModel->PList;

        if (!plist)
            return;

        while (1) {
            Uint8 cnk = *plist;

            if (cnk < NJD_BITSOFF) {
                plist += 1;
            } else if (cnk < NJD_TINYOFF) {
                plist += 1;
            } else if (cnk < NJD_MATOFF) {
                switch (cnk) {
                    case 8: {
                        Uint16 tid;

                        // something causes a usual r2-r3 if(0) type regswap here
                        // you can put basically anything empty if related here and it matches
                        // maybe fake match, but i mean it most likely really was just an empty if
                        // so idk if it even matters
                        // debug xbox asm for this is at 0x8272B380
                        if (!i) {
                        } else {
                        }

                        if (!Count) {
                            plist++;
                            tid = *plist;
                            if (i >= 4) {
                            } else {
                            }
                            tid &= 0xC000;
                            tid |= (TexId & 0x1FFF);

                            *plist = tid;
                            return;
                        } else {
                        }
                        --Count;

                        plist++;
                        break;
                    }
                    default: {
                        plist += 2;
                        break;
                    }
                }
            } else if (cnk < NJD_VERTOFF) {
                plist += 2;

                switch (cnk) {
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
            } else if (cnk < NJD_VOLOFF) {
            } else if (cnk < NJD_STRIPOFF) {
            } else if (cnk < NJD_ENDOFF) {
                Uint16 size;
                plist++;
                size = *(plist) + 1;

                plist += size;
            } else {
                break;
            }
        }
    }
}

void AL_MatSwitchObjectEnvironmentMapping(al_object* pObject, Bool Enable) {
    if (pObject) {
        if (pObject->pModel)
            AL_MatSwitchModelEnvironmentMapping(pObject->pModel, Enable);
    }
}

void AL_MatSwitchModelEnvironmentMapping(al_model* pModel, Bool Enable) {
    int i = 0;

    if (pModel) {
        Sint16* plist = pModel->PList;

        if (!plist)
            return;

        while (1) {
            Uint8 cnk = *plist;

            if (cnk < NJD_BITSOFF) {
                plist += 1;
            } else if (cnk < NJD_TINYOFF) {
                plist += 1;
            } else if (cnk < NJD_MATOFF) {
                plist += 2;

            } else if (cnk < NJD_VERTOFF) {
                plist += 2;

                switch (cnk) {
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

            } else if (0)
                ; // ???
            else if (cnk < NJD_VOLOFF) {

            } else if (cnk < NJD_STRIPOFF) {
            } else if (cnk < NJD_ENDOFF) {
                Uint16 size;

                if (Enable > 0)
                    *plist |= 0x4000;
                else
                    *plist &= ~0x4000;

                plist++;
                size = *(plist) + 1;

                plist += size;
            } else {
                break;
            }
        }
    }
}

void AL_MatSetChaoEnvOn(task* tp, int EnvNum) {
    taskwk* twork = GET_CHAOWK(tp);
    chaowk* work = GET_CHAOWK(tp);
    AL_SHAPE* pShape = &work->Shape;
    al_object** pObjectList = pShape->CurrObjectList;
    Sint16* pSetList = EnvSetList;
    int i;

    if (pShape->Flag & 0x40) {
        AL_MatSetChaoMultiOff(tp);
    }

    pShape->Flag |= 0x10;

    for (i = 0; i < 40; i++) {
        if (*pSetList == -1) {
            break;
        }

        if (*pSetList == i) {
            AL_MatSwitchObjectEnvironmentMapping(*pObjectList, TRUE);
            AL_MatChangeObjectTexture(*pObjectList, EnvNum);
            pSetList++;
        }

        pObjectList++;
    }
}

void AL_MatSetChaoEnvOff(task* tp) {
    taskwk* twork = GET_CHAOWK(tp);
    chaowk* work = GET_CHAOWK(tp);
    AL_SHAPE* pShape = &work->Shape;
    al_object** pObjectList = pShape->CurrObjectList;
    Sint16* pSetList = EnvSetList;
    int i;

    pShape->Flag &= ~0x10;

    for (i = 0; i < 40; i++) {
        if (*pSetList == -1) {
            break;
        }

        if (*pSetList == i) {
            AL_MatSwitchObjectEnvironmentMapping(*pObjectList, FALSE);
            AL_MatResetObjectTexture(*pObjectList);
            pSetList++;
        }

        pObjectList++;
    }
}

void AL_MatSetModelMultiOn(al_model* pModel) {
    Sint16* PList = pModel->PList;

    if (PList) {
        PList += (pModel->PListSize - 1);
        *PList = NJD_NULLOFF;
    }
}

void AL_MatSetObjectMultiOn(al_object* pObject) {
    if (pObject) {
        if (pObject->pModel) {
            AL_MatSetModelMultiOn(pObject->pModel);
        }
    }
}

void AL_MatSetModelMultiOff(al_model* pModel) {
    Sint16* PList = pModel->PList;

    PList += (pModel->PListSize - 1);
    *PList = NJD_ENDOFF;
}

void AL_MatSetObjectMultiOff(al_object* pObject) {
    if (pObject) {
        if (pObject->pModel) {
            AL_MatSetModelMultiOff(pObject->pModel);
        }
    }
}

// made up name
void AL_MatSetChaoMultiOn(task* tp) {
    taskwk* twork = GET_CHAOWK(tp);
    chaowk* work = GET_CHAOWK(tp);
    AL_SHAPE* pShape = &work->Shape;
    al_object** pObjectList = pShape->CurrObjectList;
    Sint16* pSetList = EnvSetList;
    int i;

    if (pShape->Flag & 0x10)
        AL_MatSetChaoEnvOff(tp);
    pShape->Flag |= 0x40;

    for (i = 0; i < 40; i++) {
        if (*pSetList == -1) {
            break;
        }

        if (*pSetList == i) {
            AL_MatSetObjectMultiOn(*pObjectList);
            pSetList++;
        }

        pObjectList++;
    }
}

void AL_MatSetChaoMultiOff(task* tp) {
    taskwk* twork = GET_CHAOWK(tp);
    chaowk* work = GET_CHAOWK(tp);
    AL_SHAPE* pShape = &work->Shape;
    al_object** pObjectList = pShape->CurrObjectList;
    Sint16* pSetList = EnvSetList;
    int i;

    pShape->Flag &= ~0x40;

    for (i = 0; i < 40; i++) {
        if (*pSetList == -1) {
            break;
        }

        if (*pSetList == i) {
            AL_MatSetObjectMultiOff(*pObjectList);
            pSetList++;
        }

        pObjectList++;
    }
}

NJS_CNK_OBJECT* AL_MatCopyObject(NJS_CNK_OBJECT* pSrcObject) {
    NJS_CNK_OBJECT* pObject;

    if (!pSrcObject)
        return NULL;

    pObject = (NJS_CNK_OBJECT*)syCalloc(1, sizeof(NJS_CNK_OBJECT));

    if (!pObject)
        return NULL;

    njMemCopy(pObject, pSrcObject, sizeof(NJS_CNK_OBJECT));

    if (pSrcObject->model) {
        NJS_CNK_MODEL* pSrcModel = pSrcObject->model;
        NJS_CNK_MODEL* pModel = (al_model*)syCalloc(1, sizeof(NJS_CNK_MODEL));
        int i;

        njMemCopy(pModel, pSrcObject->model, sizeof(NJS_CNK_MODEL));

        pModel->r = 0.0f;
        pObject->model = pModel;

        if (pSrcModel->vlist) {
            Sint32* vlist = pSrcModel->vlist;
            Sint32* vlist2;
            Uint32 length = 1;

            while (*(vlist++) != (Uint8)NJD_ENDOFF) {
                ++length;
            }

            vlist = pSrcModel->vlist;
            vlist2 = (Sint32*)syMalloc(sizeof(Sint32) * length);

            njMemCopy4(vlist2, vlist, length);
            pModel->vlist = vlist2;
        }

        if (pSrcModel->plist) {
            Sint16* plist = pSrcModel->plist;
            Sint16* plist2;
            Uint32 length2;

            length2 = 0;

            while (TRUE) {
                Uint8 cnk = *plist;

                if (cnk < NJD_BITSOFF) {
                    length2++;

                    plist++;
                }

                else if (cnk < NJD_TINYOFF) {
                    length2++;

                    plist++;
                } else if ((cnk) < NJD_MATOFF) {
                    length2 += 2;

                    plist += 2;
                } else if (cnk < NJD_VERTOFF) {
                    length2 += 2;

                    plist += 2;
                    switch (cnk) {
                        case NJD_CM_D:
                        case NJD_CM_A:
                        case NJD_CM_S:
                            length2 += 2;

                            plist += 2;
                            break;
                        case NJD_CM_DA:
                        case NJD_CM_DS:
                        case NJD_CM_AS:
                            length2 += 4;

                            plist += 4;
                            break;
                        case NJD_CM_DAS:
                            length2 += 6;

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
                } else {
                    length2 += 1;
                    break;
                }
            }

            plist = pSrcModel->plist;
            plist2 = (Sint16*)syCalloc(length2, sizeof(Sint16));

            njMemCopy2(plist2, plist, length2);
            pModel->plist = plist2;
        } else {
            pModel->plist = NULL;
        }
    }

    if (pSrcObject->child)
        pObject->child = AL_MatCopyObject(pSrcObject->child);

    if (pSrcObject->sibling)
        pObject->sibling = AL_MatCopyObject(pSrcObject->sibling);

    return pObject;
}

void AL_MatFreeObject(NJS_CNK_OBJECT* pObject) {
    NJS_CNK_MODEL* pModel;
    if (!pObject)
        return;

    if (pObject->child) {
        AL_MatFreeObject(pObject->child);
        pObject->child = NULL;
    }

    if (pObject->sibling) {
        AL_MatFreeObject(pObject->sibling);
        pObject->sibling = NULL;
    }

    pModel = pObject->model;
    if (pModel) {
        if (pModel->vlist) {
            syFree(pModel->vlist);
            pModel->vlist = NULL;
        }

        if (pModel->plist) {
            syFree(pModel->plist);
            pModel->plist = NULL;
        }

        syFree(pModel);
    }

    syFree(pObject);
}

UV_SCROLL_INFO* AL_MatUVScrollInit(NJS_CNK_OBJECT* pOrgObject, float AddU, float AddV) {
    UV_SCROLL_INFO* pInfo = (UV_SCROLL_INFO*)syCalloc(1, sizeof(UV_SCROLL_INFO));

    pInfo->pOrgObject = pOrgObject;
    pInfo->pCopyObject = AL_MatCopyObject(pOrgObject);

    pInfo->addU = AddU;
    pInfo->addV = AddV;

    return pInfo;
}

void AL_MatUVScrollFree(UV_SCROLL_INFO* pInfo) {
    AL_MatFreeObject(pInfo->pCopyObject);
    syFree(pInfo);
}

void AL_MatUVScrollControl(UV_SCROLL_INFO* pInfo) {
    NJS_CNK_MODEL* pOrgModel = pInfo->pOrgObject->model;
    NJS_CNK_MODEL* pCopyModel = pInfo->pCopyObject->model;

    pInfo->offsetU += pInfo->addU;
    pInfo->offsetV += pInfo->addV;

    while (pInfo->offsetU > 256.f)
        pInfo->offsetU -= 256.f;
    while (pInfo->offsetV > 256.f)
        pInfo->offsetV -= 256.f;

    while (pInfo->offsetU < 0.f)
        pInfo->offsetU += 256.f;
    while (pInfo->offsetV < 0.f)
        pInfo->offsetV += 256.f;

    if (pOrgModel->plist) {
        Sint16* pOrgPlist = pOrgModel->plist;
        Sint16* pCopyPlist = pCopyModel->plist;

        while (1) {
            Uint8 cnk = *pOrgPlist;

            if (cnk < NJD_BITSOFF) {
                pOrgPlist += 1;
                pCopyPlist += 1;
            } else if (cnk < NJD_TINYOFF) {
                pOrgPlist += 1;
                pCopyPlist += 1;
            } else if (cnk < NJD_MATOFF) {
                pOrgPlist += 2;
                pCopyPlist += 2;
            } else if (cnk < NJD_VERTOFF) {
                pOrgPlist += 2;
                pCopyPlist += 2;

                switch (cnk) {
                    case NJD_CM_D:
                    case NJD_CM_A:
                    case NJD_CM_S:
                        pOrgPlist += 2;
                        pCopyPlist += 2;
                        break;
                    case NJD_CM_DA:
                    case NJD_CM_DS:
                    case NJD_CM_AS:
                        pOrgPlist += 4;
                        pCopyPlist += 4;
                        break;
                    case NJD_CM_DAS:
                        pOrgPlist += 6;
                        pCopyPlist += 6;
                        break;
                }
            } else if (0)
                ; // ????
            else if (cnk < NJD_VOLOFF) {
            } else if (cnk < NJD_STRIPOFF) {
            } else if (cnk < NJD_ENDOFF) {
                Uint16 thing2;

                pOrgPlist++;
                pOrgPlist++;
                pCopyPlist++;
                pCopyPlist++;

                thing2 = *pOrgPlist & 0x3FFF;

                pOrgPlist++;
                pCopyPlist++;

                while (thing2--) {
                    Sint16 indexCount = *pOrgPlist;
                    pOrgPlist++;
                    pCopyPlist++;

                    if (indexCount < 0)
                        indexCount = -indexCount;

                    while (indexCount--) {
                        pOrgPlist++;
                        pCopyPlist++;
                        *(pCopyPlist++) = *(pOrgPlist++) + pInfo->offsetU;
                        *(pCopyPlist++) = *(pOrgPlist++) + pInfo->offsetV;
                    }
                }
            } else {
                break;
            }
        }
    }
}

void AL_MaterialInit(task* tp) {
    chaowk* work = GET_CHAOWK(tp);
    CHAO_PARAM* pParam = &work->param;

    if (pParam->body.ColorNum > 0)
        AL_MatSetChaoColor(tp, pParam->body.ColorNum);

    if (pParam->body.MultiNum == 1) {
        AL_MatSetChaoMultiOn(tp);
    } else if (pParam->body.JewelNum > 0) {
        Sint16 ColorNum = lbl_0C56F674[pParam->body.JewelNum];
        Sint16 EnvNum = lbl_0C56F654[pParam->body.JewelNum];

        if (ColorNum != JewelColor_Pearl) {
            AL_MatSetChaoColor(tp, ColorNum);
        }

        AL_MatSetChaoEnvOn(tp, EnvNum);
    }
}