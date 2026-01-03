#include <shinobi.h>
#include <Chao/al_world.h>
#include <Chao/al_material.h>
#include <Chao/al_texlist.h>
#include <Chao/al_misc.h>
#include <Chao/alo_growtree.h>
#include <playsound.h>
#include <effect2.h>

#define GET_TREE_WORK(tp) ((TREE_WORK*)tp->twp)
#define GET_MOVE_WORK(tp) ((MOVE_WORK*)tp->Data2)

enum {
    CI_TREE_FOR = 0,
    CI_TREE_LMO = 1,
    CI_TREE_SHADOW = 2,
    CI_TREE_PLANT = 3,
    CI_TREE_SENSOR = 4,
    CI_TREE_THIRSTY = 5,
    CI_TREE_SAPLING = 6,
    CI_TREE_SPHERE = 7,
    CI_TREE_SAPLING_SPHERE = 8,
};

enum {
    MD_PLANT = 0,
    MD_SEED = 1,
    MD_SEED_TO_SAPLING = 2,
    MD_SEED_TO_SAPLING2 = 3,
    MD_SAPLING = 4,
    MD_SAPLING_GROW = 5,
    MD_SAPLING_GROW2 = 6,
    MD_ADULT = 7,
    MD_DEAD = 8,
    MD_DEAD2 = 9,
    MD_FADEOUT = 10,
};

static const CCL_INFO colli_info[9] = {
    CCL_CYLINDER(0, 0x70, 0, 0x2000, 0, 16, 0, 8, 16, 0),
    CCL_CYLINDER(0, 0x77, 0, 0x208000, 0, 14, 0, 2, 14, 0),
    CCL_CYLINDER(CI_KIND_AL_SHADOW, 0x70, 0, 0x3C8000, 0, 15, 0, 12, 15, 0),
    CCL_SPHERE(CI_KIND_AL_PLANT_POS, 0x70, 0, 0x3C8000, 0, 0, 0, 30, 0, 0),
    CCL_SPHERE(0, 0x70, 0, 0x248000, 0,0,0, 6, 0,0),
    CCL_SPHERE(CI_KIND_AL_THIRSTY, 0x70, 0, 0x3C8000, 0,0,0,20,0,0),
    CCL_CYLINDER(0, 0x77, 0, 0x20A000, 0, 5, 0, 0.4, 5,0),
    CCL_SPHERE(0,0x77,0,0x8000,0,28,0,2.5,0,0),
    CCL_SPHERE(0,0x77,0,0x8000,0,10,0,2.5,0,0),
};

// belongs to this file:
extern NJS_CNK_OBJECT lbl_0C6181BC;
extern NJS_CNK_OBJECT lbl_0C6189A8;
extern NJS_CNK_OBJECT lbl_0C619038;
extern NJS_CNK_OBJECT lbl_0C619104;

extern NJS_CNK_OBJECT* TreeObjectList[1];
extern int FruitCounterSwitch;
extern int FruitCounter[2];
extern int FruitCount;
extern int FruitPhaseList[];
extern int LeafPhaseList[];

// doesn't:
extern NJS_CNK_OBJECT* FruitObjectList[];
extern NJS_CNK_OBJECT* SeedObjectList[];

extern NJS_CNK_MODEL model_kage_marukage_marukage[];

extern NJS_TEXLIST lbl_0C60080C;
extern NJS_TEXLIST lbl_0C600AEC;
extern NJS_TEXLIST lbl_0C600964;

void lbl_0C525100(task* tp) {
    Uint32 i;
    
    i = CI_TREE_SAPLING_SPHERE + 1;

    do {
        i--;
        CCL_Disable(tp, i);
    } while(i);
}

void AL_ClearFruitNum() {
    if (FruitCounterSwitch) {
        FruitCounterSwitch = 0;
        FruitCounter[1] = 0;
    }
    else {
        FruitCounterSwitch = 1;
        FruitCounter[0] = 0;
    }
}

void lbl_0C525140() {
    if (FruitCounterSwitch) {
        FruitCounter[0]++;
    }
    else {
        FruitCounter[1]++;
    }
}

int lbl_0C52515C() {
    Uint32 count2 = FruitCounter[FruitCounterSwitch] + ALW_CountEntry(ALW_CATEGORY_FRUIT);
    
    return count2;
}

void lbl_0C52517C(TREE_WORK* pWork) {
    pWork->pCopyObject = AL_MatCopyObject(TreeObjectList[3 * (pWork->kind - 1)]);
}

void lbl_0C5251AE() {}

void lbl_0C5251B2(TREE_WORK* pWork) {
    if(pWork->pCopyObject) {
        AL_MatFreeObject(pWork->pCopyObject);
        pWork->pCopyObject = NULL;
    }
}

void lbl_0C525200(task* tp) {
    TREE_WORK* work = GET_TREE_WORK(tp);
    taskwk* twk = tp->twp;

    work->growth = 1.f;
    work->state = TREE_ST_ADULT;

    work->fruit[0].growth = -0.3f * njRandom();
    work->fruit[1].growth = -0.3f * njRandom();
    work->fruit[2].growth = -0.3f * njRandom();

    work->fruit[0].SwingCounter = (Uint16)((int)(njRandom() * 13.999999f) + (Uint16)11);
    work->fruit[1].SwingCounter = (Uint16)((int)(njRandom() * 13.999999f) + (Uint16)11);
    work->fruit[2].SwingCounter = (Uint16)((int)(njRandom() * 13.999999f) + (Uint16)11);

    lbl_0C525100(tp);

    CCL_Enable(tp, CI_TREE_FOR);
    CCL_Enable(tp, CI_TREE_LMO);
    CCL_Enable(tp, CI_TREE_SHADOW);
    CCL_Enable(tp, CI_TREE_SPHERE);

    ObjectMovableInitialize(tp->twp, tp->Data2, 0xD);

    twk->mode = MD_ADULT;
    twk->smode = 0;
    twk->wtimer = 0;
    twk->id = 0;

    work->ThirstyFlag = 0;
    work->life = 6000;
}

void CalcFruitPosSub(NJS_POINT3 *pPos, TREE_WORK *pTree, NJS_CNK_OBJECT *pCopyObject, int FruitNum) {
    njPushMatrixEx();
    
    njTranslateV(NULL, (NJS_POINT3*)pCopyObject->pos);
    njRotateXYZ(NULL, pCopyObject->ang[0], pCopyObject->ang[1], pCopyObject->ang[2]);
    
    if(!pCopyObject->model) {
        njRotateX(NULL, NJM_DEG_ANG( pTree->FruitWidth * njSin(FruitPhaseList[FruitCount] + pTree->FruitRotAng)));
        
        if(FruitCount == FruitNum) {
            njGetTranslation(NULL, pPos);
            pPos->y -= 1.5f;
        }
        
        FruitCount++;
    }

    // !BUG! sibling in parameters instead of child, this is there on sa2b too
    if(pCopyObject->child) CalcFruitPosSub(pPos, pTree, pCopyObject->sibling, FruitNum);

    njPopMatrixEx();
    
    if(pCopyObject->sibling) CalcFruitPosSub(pPos, pTree, pCopyObject->sibling, FruitNum);
}

void CalcFruitPos(NJS_POINT3 *pPos, TREE_WORK *pTree, int FruitNum) {
    NJS_CNK_OBJECT* pCopyObject;
    
    FruitCount = 0;

    if(pTree->garden != 2) {
        njPushMatrix(_nj_unit_matrix_);
        
        njTranslateEx(&pTree->pos);
        njRotateY(NULL, pTree->TouchAngle);
        njRotateX(NULL, pTree->RollAngle);
    
        njRotateY(NULL, pTree->ang.y - pTree->TouchAngle);
        
        if(pTree->pCopyObject) pCopyObject = pTree->pCopyObject;
        else if (pTree->pLocalObject) pCopyObject = pTree->pLocalObject;
        else return;
    
        njPushMatrixEx();
        njTranslateEx((NJS_POINT3*)pCopyObject->pos);
        njRotateXYZ(NULL, pCopyObject->ang[0], pCopyObject->ang[1], pCopyObject->ang[2]);
        
        if(pCopyObject->child) CalcFruitPosSub(pPos, pTree, pCopyObject->child, FruitNum);
    
        njPopMatrixEx();
        
        if(pCopyObject->sibling) CalcFruitPosSub(pPos, pTree, pCopyObject->sibling, FruitNum);
        
        njPopMatrixEx();
    }
    else {
        pPos->x = pTree->pos.x;
        pPos->y = pTree->pos.y + 29;
        pPos->z = pTree->pos.z;        
    }
}

void ALO_GrowTreeExecutor(task* tp) {
    taskwk* twk = tp->twp;
    TREE_WORK* pTree = GET_TREE_WORK(tp);

    switch(twk->mode) {
        case MD_PLANT:
            if(ALW_RecieveCommand(tp) == ALW_CMD_PLANTED) {
                pTree->kind = sub_C51E3B2(tp) + 1;

                CCL_Disable(tp, CI_TREE_PLANT);
                CCL_Enable(tp, CI_TREE_SENSOR);
                CCL_Enable(tp, CI_TREE_THIRSTY);

                pTree->ThirstyFlag = 0;
                pTree->water = 0;

                twk->mode = MD_SEED;
                twk->smode = 0;
                twk->wtimer = 0;
                twk->id = 0;

                pTree->state = TREE_ST_SEED;
            }
            break;
        case MD_SEED:
            if(AL_IsHitKindWithNum(tp, CI_TREE_SENSOR, CI_KIND_AL_WATER)) {
                pTree->water = 10000;
            }

            if(twk->wtimer++ >= GET_GLOBAL()->SeedClock) {
                twk->wtimer = 0;
                pTree->water += 100;
            }

            if(pTree->water >= 10000) {
                pTree->water = 6000;

                CCL_Disable(tp, 4);

                twk->mode = MD_SEED_TO_SAPLING;
                twk->smode = 0;

                pTree->state = TREE_ST_SAPLING;
            }
            
            break;

        case MD_SEED_TO_SAPLING:
            if(twk->wtimer++ > 180) {
                lbl_0C52517C(pTree);
                CCL_Enable(tp, CI_TREE_SAPLING);

                pTree->scale = 0.01f;
                pTree->ScaleSpd = 0.f;

                twk->mode = MD_SEED_TO_SAPLING2;
                twk->smode = 0;
                twk->wtimer = 0;
                twk->id = 0;
            }
            
            break;

        case MD_SEED_TO_SAPLING2:
            pTree->ScaleSpd += (1 - pTree->scale) * 0.15f;
            pTree->scale += pTree->ScaleSpd;
            pTree->ScaleSpd *= 0.85f;

            if(twk->wtimer++ > 200) {
                pTree->scale = 1.f;
                pTree->water = 10000;
                pTree->ThirstyFlag = 0;

                twk->mode = MD_SAPLING;
                twk->smode = 0;
                twk->wtimer = 0;
                twk->id = 0;
            }
            
            break;

        case MD_SAPLING:
            if(AL_IsHitKindWithNum(tp, CI_TREE_SENSOR, CI_KIND_AL_WATER)) {
                pTree->water = 10000;
                
                CCL_Disable(tp, CI_TREE_SENSOR);
                CCL_Disable(tp, CI_TREE_THIRSTY);
                
                twk->mode = MD_SAPLING_GROW;
                twk->smode = 0;
                twk->wtimer = 0;
                twk->id = 0;
            }

            if(twk->wtimer++ > GET_GLOBAL()->SaplingClock) {
                pTree->water -= GET_GLOBAL()->WaterSub;
                twk->wtimer = 0;
            }

            if(pTree->water > 5000) {
                pTree->growth += 0.00005f;
            }
            else {
                pTree->growth += 0.00002f;
                
                if(pTree->water < 0) {
                    pTree->water = 0;
                }
            }

            if (pTree->water < 6000 && !pTree->ThirstyFlag) {
                pTree->ThirstyFlag = 1;
                CCL_Enable(tp, CI_TREE_SENSOR);
                CCL_Enable(tp, CI_TREE_THIRSTY);
            }

            AL_InterpolateObject(
                TreeObjectList[3 * (pTree->kind - 1)],
                TreeObjectList[3 * (pTree->kind - 1) + 1],
                pTree->pCopyObject,
                pTree->growth
            );

            if(pTree->growth >= 1.f) {
                lbl_0C525200(tp);
            }
            
            break;

        case MD_SAPLING_GROW:
            if(twk->wtimer++ > 180) {
                pTree->ScaleSpd = 0.f;

                twk->mode = 6;
                twk->smode = 0;
                twk->wtimer = 0;
                twk->id = 0;

                sub_8C05B020(0x1014, 0, 0, 0, &tp->twp->pos);
            }
            break;    
        
        case MD_SAPLING_GROW2:
            pTree->ScaleSpd += (1 - pTree->growth) * 0.15f;
            pTree->growth += pTree->ScaleSpd;
            pTree->ScaleSpd *= 0.85f;

            AL_InterpolateObject(
                TreeObjectList[3 * (pTree->kind - 1)],
                TreeObjectList[3 * (pTree->kind - 1) + 1],
                pTree->pCopyObject,
                pTree->growth
            );

            if(twk->wtimer++ > 200) {
                lbl_0C525200(tp);
            }
            
            break;

        case MD_ADULT:
            pTree->WaterFlag = 0;

            if (AL_IsHitKindWithNum(tp, CI_TREE_SENSOR, CI_KIND_AL_WATER)) {
                pTree->water = 10000;

                CCL_Disable(tp, CI_TREE_SENSOR);
                CCL_Disable(tp, CI_TREE_THIRSTY);

                pTree->ThirstyFlag = FALSE;
                pTree->WaterFlag = TRUE;
            }
            
            if (twk->wtimer++ > GET_GLOBAL()->TreeClock) {
                if(pTree->water > 5000) {
                    pTree->life -= 1;
                }
                else if(pTree->water > 2000) {
                    pTree->life -= 2;
                }
                else {
                    pTree->life -= 3;
                }
               
                pTree->water -= GET_GLOBAL()->WaterSub;
                
                if (pTree->water < 0) {
                    pTree->water = 0;
                }
                
                if (pTree->state == TREE_ST_ADULT && pTree->life <= 0) {
                    pTree->state = TREE_ST_PLANT;
                    
                    twk->mode = MD_DEAD;
                    twk->smode = 0;
                    twk->wtimer = 0;
                    twk->id = 0;
                    
                    pTree->life = 0;
                }
                
                if (pTree->water < 6000 && !pTree->ThirstyFlag) {
                    pTree->ThirstyFlag = TRUE;

                    CCL_Enable(tp, CI_TREE_SENSOR);
                    CCL_Enable(tp, CI_TREE_THIRSTY);
                }
                
                twk->wtimer = 0;
            }
            
            {
                MOVE_WORK* movewk = GET_MOVE_WORK(tp);
                float scalor = njScalor(&movewk->Acc);
                
                pTree->SwingDist = scalor;
                if(scalor < 0.01f) {
                    pTree->ChaoSwingPhase = AdjustAngle(pTree->ChaoSwingPhase, 0, 1024);
                }
                
                {
                    int i;
                    MOVE_WORK* movewk2 = GET_MOVE_WORK(tp);
                    float swingDist = pTree->SwingDist;
                    
                    pTree->RollAngle = NJM_DEG_ANG(swingDist * 6.f);
                    pTree->TouchAngle = 0x4000 - njArcTan2(movewk2->Acc.z, movewk2->Acc.x);
    
                    movewk2->Acc.x = 0.f;
                    movewk2->Acc.y = 0.f;
                    movewk2->Acc.z = 0.f;
                    
                    pTree->SwingCountFlag = 0;
        
                    if (swingDist < 0.1f) {
                        if(pTree->SwingFlag) {
                            sub_8C05B2D4(0x1008, tp, 1, -15, 60, &twk->pos);
                            
                            pTree->SwingFlag = FALSE;
                            pTree->SwingCountFlag = TRUE;
                        }
                    }
    
                    if (swingDist > 0.95f && !pTree->SwingFlag) {
                        pTree->SwingFlag = TRUE;
                        pTree->SwingCountFlag = TRUE;
                    }
                    
                    for(i = 0; i < FRUIT_COUNT; i++) {
                        FRUIT_INFO* pFruit = &pTree->fruit[i];
    
                        if(pFruit->growth >= 0.f) {
                            pFruit->growth += GET_GLOBAL()->TreeFruitAddGrowth;
    
                            if(pFruit->growth > 2.0f) pFruit->growth = 2.0f;
                            if(pFruit->AimGrowth < pFruit->growth) pFruit->AimGrowth = pFruit->growth;
    
                            if(pTree->WaterFlag) {
                                if(pFruit->AimGrowth < 1.0f) {
                                    pFruit->AimGrowth += 0.6f;
    
                                    if(pFruit->AimGrowth > 1.f) pFruit->AimGrowth = 1.f;
                                }
                            }
    
                            if(pFruit->growth < pFruit->AimGrowth) {
                                pFruit->growth += 0.08f;
                                if(pFruit->growth > 2.0f) pFruit->growth = 2.0f;
                            }
    
                            lbl_0C525140();
                        }
                        else if(lbl_0C52515C() < GET_GLOBAL()->FruitMaxNum) {
                            pTree->fruit[i].growth += GET_GLOBAL()->TreeFruitAddGrowth;
                        }
    
                        if(pFruit->growth > 0.9f) {
                            Sint32 flag = false;
    
                            if(pTree->SwingCountFlag) {
                                pFruit->SwingCounter--;
                                
                                if(pFruit->SwingCounter <= 0) flag = 1;
                            }
    
                            if(pFruit->growth > 1.9f && ALW_CountEntry(3) < 4) {
                                if((twk->wtimer % 10) == 1 && njRandom() < 0.002f)
                                flag = 1;
                            }
    
                            if(flag) {
                                const NJS_VECTOR velo = {0,0,0};
                                
                                pFruit->SwingCounter = (Uint16)((int)(njRandom() * 13.999999f) + 11);
                                pFruit->growth = -0.3f * njRandom();
                                pFruit->AimGrowth = pFruit->growth;
    
                                {
                                    NJS_POINT3 pos;
                                    NJS_VECTOR velo;
    
                                    velo.x = (njSin(pTree->TouchAngle) * 0.03f) + (0.5f - njRandom()) / 100.f;
                                    velo.y = 0.f;
                                    velo.z = (njCos(pTree->TouchAngle) * 0.03f) + (0.5f - njRandom()) / 100.f;
     
                                    if(njRandom() < 0.5f) {
                                        velo.x *= -1.f;
                                        velo.z *= -1.f;
                                    }
    
                                    CalcFruitPos(&pos, pTree, i);

                                    //????
                                     //#define randtest NJM_RAD_ANG(njRandom() * (8*NJD_PI)/3.f)
                                    #define randtest NJM_DEG_ANG(njRandom() * 0.01086294654)
                                    //#define randtest NJM_DEG_ANG(njRandom() * 0.01086294654)
                                    
                                    switch(pTree->state){
                                        case 5:
                                            switch(pTree->garden) {
                                                case 1:
                                                    ALO_FruitCreate(9, &pos, randtest, &velo);
                                                    break;
                                                case 2:
                                                    ALO_FruitCreate(10, &pos, randtest, &velo);
                                                    break;
                                                case 3:
                                                    ALO_FruitCreate(11, &pos, randtest, &velo);
                                                    break;
                                            }
                                            break;
                                        default:
                                            ALO_FruitCreate(pTree->kind - 1, &pos, randtest, &velo);
                                    }
                                   
    
                                    sub_8C05B020(0x1009, 0, 0, 0, &tp->twp->pos);
                                }
                                
                            }
                        }
                    }
                    
                    pTree->FruitWidth += (swingDist * 0.9f);
                    if(pTree->FruitWidth > 14.f) pTree->FruitWidth = 14.f;
                    pTree->FruitRotAng += 1824;
                    pTree->FruitWidth *= 0.98f;
        
                    pTree->LeafWidth += (swingDist * 0.3f);
                    if(pTree->LeafWidth > 6.f) pTree->LeafWidth = 6.f;
                    pTree->LeafRotAng += 1536;
                    pTree->LeafWidth *= 0.986f;
                }

            }

            break;
        case MD_DEAD:
            pTree->growth += 0.01f;
            if(pTree->growth > 1.f) {
                pTree->growth = 1.f;  

                twk->mode = MD_DEAD2;
                twk->smode = 0;
                twk->wtimer = 0;
                twk->id = 0;
            }
            
            AL_InterpolateObject(
                TreeObjectList[3 * (pTree->kind - 1) + 1],
                TreeObjectList[3 * (pTree->kind - 1) + 2],
                pTree->pCopyObject,
                pTree->growth
            );
            break;
        
        case MD_DEAD2:
            if(twk->wtimer++ > 18000) {
                twk->mode = MD_FADEOUT;
                twk->smode = 0;
                twk->wtimer = 0;
                twk->id = 0;

                pTree->scale = 1.f;
            }
            
            break;

        case MD_FADEOUT:
            pTree->scale -= 0.01f;

            if(pTree->scale < 0.f) {
                pTree->scale = 0.f; 

                // it doesn't inline on its own
                // + there's also another part where it doesn't get inlined
                // and debug build shows that they didn't call it directly either
                // ...but why
                {
                    Uint32 i = CI_TREE_SAPLING_SPHERE + 1;
                
                    do {
                        i--;
                        CCL_Disable(tp, i);
                    } while(i);
                }
                
                CCL_Enable(tp, CI_TREE_PLANT);

                twk->mode = MD_PLANT;
                twk->smode = 0;
                twk->wtimer = 0;
                twk->id = 0;

                lbl_0C5251B2(pTree);
            }
            
            break;
    }

    CCL_Entry(tp);
}

void AL_DrawTreeSub(TREE_WORK *pTree, NJS_CNK_OBJECT *pCopyObject) {    
    njPushMatrixEx();
    njTranslateV(NULL, (NJS_POINT3*)pCopyObject->pos);
    njRotateXYZ(NULL, pCopyObject->ang[0], pCopyObject->ang[1], pCopyObject->ang[2]);

    if(pCopyObject->model) {
        njRotateX(NULL, NJM_DEG_ANG(pTree->LeafWidth * njSin(pTree->LeafRotAng + LeafPhaseList[pTree->LeafDispNum])));
        njCnkEasyDrawModel(pCopyObject->model);

        pTree->LeafDispNum++;
    }
    else {
        FRUIT_INFO* pFruit = &pTree->fruit[pTree->FruitDispNum];
        float growth;
        float scl;
        const NJS_VECTOR vec = {0, -1.8, 0};

        growth = pFruit->growth;
        if(growth > 1) growth = 1;

        scl = growth * 1.5f;

        if(scl > 0) {
            if(pFruit->growth > 0.9f) {
                njPushMatrixEx();
                njGetTranslation(NULL, &pFruit->pos);
                njPopMatrixEx();

                njRotateX(NULL, NJM_DEG_ANG(pTree->FruitWidth * njSin(pTree->FruitRotAng + FruitPhaseList[pTree->FruitDispNum])));
            }
            else {
                njRotateX(NULL, NJM_DEG_ANG((pTree->FruitWidth * 0.2f) * njSin(pTree->FruitRotAng + FruitPhaseList[pTree->FruitDispNum])));
            }

            njScale(NULL, scl, scl, scl);
            njTranslateEx(&vec);
            
            switch(pTree->garden) {
                case 1:
                    njCnkEasyDrawModel(FruitObjectList[9]->model);
                    break;
                case 2:
                    njCnkEasyDrawModel(FruitObjectList[10]->model);
                    break;
                case 3:
                    njCnkEasyDrawModel(FruitObjectList[11]->model);
                    break;
                default:
                    if(pTree->kind == 1) {
                        njRotateX(NULL, 0x8000);
                    }
                    
                    njCnkEasyDrawModel(FruitObjectList[pTree->kind - 1]->model);
                    break;
            }
        }

        pTree->FruitDispNum++;
    }

    if(pCopyObject->child) AL_DrawTreeSub(pTree, pCopyObject->child);

    njPopMatrixEx();

    if(pCopyObject->sibling) AL_DrawTreeSub(pTree, pCopyObject->sibling);
}

// assumed name
void AL_DrawTree(TREE_WORK *pTree, NJS_CNK_OBJECT *pCopyObject) {
    pTree->LeafDispNum = 0;
    pTree->FruitDispNum = 0;
    
    njPushMatrixEx();
    njTranslateV(NULL, (NJS_POINT3*)pCopyObject->pos);
    njRotateXYZ(NULL, pCopyObject->ang[0], pCopyObject->ang[1], pCopyObject->ang[2]);
    njCnkEasyDrawModel(pCopyObject->model);

    if(pCopyObject->child) AL_DrawTreeSub(pTree, pCopyObject->child);
    
    njPopMatrixEx();

    if(pCopyObject->sibling) AL_DrawTreeSub(pTree, pCopyObject->sibling);

    if(pTree->garden == 2) {
        int i;
        
        njPushMatrixEx();
        njTranslate(NULL, 0, 28, 0);
        
        for(i = 0; i < FRUIT_COUNT; i++) {
            FRUIT_INFO* pFruit = &pTree->fruit[i];
            float growth = pFruit->growth;
            float scl;
            NJS_VECTOR vec = {0, -1.8, 0};
    
            if(growth > 1) growth = 1;
    
            scl = growth * 1.5f;

            njPushMatrixEx();
            
            njRotateY(NULL, 0x5555 * i);
            njRotateX(NULL, 0x1555);
            njTranslate(NULL, 0, 0.5f, 0);

            if (scl > 0) {
                if(pFruit->growth > 0.9f) {
                    njRotateX(NULL, NJM_DEG_ANG(pTree->FruitWidth * njSin(pTree->FruitRotAng + FruitPhaseList[i])));
                }
                else {
                    njRotateX(NULL, NJM_DEG_ANG((pTree->FruitWidth * 0.2f) * njSin(pTree->FruitRotAng + FruitPhaseList[i])));
                }

                njScale(NULL, scl, scl, scl);
                njTranslateEx(&vec);
                njCnkEasyDrawModel(FruitObjectList[10]->model);
            }
            
            njPopMatrixEx();
            
        }
        njPopMatrixEx();
    }
}

void lbl_0C526400(TREE_WORK* pTree) {
    NJS_POINT3 shadow_pos;
    
    njPushMatrix(_nj_unit_matrix_);
    njTranslate(NULL, pTree->pos.x, pTree->pos.y, pTree->pos.z);
    njRotateY(NULL, pTree->TouchAngle);
    njRotateX(NULL, pTree->RollAngle);
    njRotateY(NULL, -pTree->TouchAngle);
    njRotateY(NULL, pTree->ang.y);
    njTranslate(NULL, 0, 20, 0);
    njGetTranslation(NULL, &shadow_pos);
    njPopMatrixEx();
    
    njPushMatrixEx();
    njScale(NULL, 3, 2, 3);
    njTranslate(NULL, 0, 0, 0.85f);
    njCnkModDrawModel(model_kage_marukage_marukage);
    njPopMatrixEx();
}

void lbl_0C5264B2(TREE_WORK* pTree) {
    NJS_POINT3 shadow_pos;
    
    njPushMatrix(_nj_unit_matrix_);
    njTranslate(NULL, pTree->pos.x, pTree->pos.y, pTree->pos.z);
    njRotateY(NULL, pTree->TouchAngle);
    njRotateX(NULL, pTree->RollAngle);
    njRotateY(NULL, -pTree->TouchAngle);
    njRotateY(NULL, pTree->ang.y);
    njTranslate(NULL, 0, 20, 0);
    njGetTranslation(NULL, &shadow_pos);
    njPopMatrixEx();
    
    njPushMatrixEx();
    njTranslateV(NULL, &shadow_pos);
    njRotateY(NULL, pTree->ang.y + 0x2000);
    {
        Angle* pPhase = LeafPhaseList;
        Angle ang = pTree->LeafRotAng;
        float width = pTree->LeafWidth;

        njPushMatrixEx();
        {
            njScale(NULL, 3, 2, 7.7f + width * (0.15f * njSin(ang + *(pPhase++))));
            njTranslate(NULL, 0, 0, 0.85f);
            njCnkModDrawModel(model_kage_marukage_marukage);
        }
        njPopMatrixEx();

        njRotateY(NULL, NJM_DEG_ANG(78.75f));
        
        njPushMatrixEx();
        {
            njScale(NULL, 3, 2, 7.3f + width * (0.15f * njSin(ang + *(pPhase++))));
            njTranslate(NULL, 0, 0, 0.85f);
            njCnkModDrawModel(model_kage_marukage_marukage);
        }
        njPopMatrixEx();

        njRotateY(NULL, NJM_DEG_ANG(112.5f));
        
        njPushMatrixEx();
        {
            njScale(NULL, 4, 2, 7.8f + width * (0.15f * njSin(ang + *(pPhase++))));
            njTranslate(NULL, 0, 0, 0.85f);
            njCnkModDrawModel(model_kage_marukage_marukage);
        }
        njPopMatrixEx();

        // maybe this makes sense to someone?
        // i mean, i just pulled that division out of my ass
        // but its the only kinda rounded way i could come up with to get the number i wanted
        // (105.46875)
        // i think its more likely they just copied these out of an editor
        njRotateY(NULL, NJM_DEG_ANG(360 * (3/10.24)));
        
        njPushMatrixEx();
        {
            njScale(NULL, 4, 2, 7.1f + width * (0.15f * njSin(ang + *(pPhase++))));
            njTranslate(NULL, 0, 0, 0.85f);
            njCnkModDrawModel(model_kage_marukage_marukage);
        }
        njPopMatrixEx();
    }
    njPopMatrixEx();
}

void ALO_GrowTreeDisplayer(task* tp) {
    taskwk* twk = tp->twp;
    TREE_WORK* pTree = GET_TREE_WORK(tp);
    int kind = pTree->kind - 1;

    njSetTexture(pTree->texlist);
    njPushMatrixEx();
    njTranslateEx(&twk->pos);

    if(pTree->pShadowObject);
    
    switch(twk->mode) {
        case MD_PLANT:
            break;
        case MD_SEED:
        case MD_SEED_TO_SAPLING:
            njRotateY(NULL, twk->ang.y);
            njScale(NULL, 1.5f, 1.5f, 1.5f);
            njCnkEasyDrawModel(SeedObjectList[kind]->model);
            break;
        case MD_SEED_TO_SAPLING2:
            if(pTree->pCopyObject) {
                float scl = pTree->scale;
                njRotateY(NULL, twk->ang.y);
                njScale(NULL, scl, scl, scl);
                njCnkEasyDrawObject(pTree->pCopyObject);
            }
            break;
        case MD_ADULT:
            if(pTree->pCopyObject) {
                njRotateY(NULL, pTree->TouchAngle);
                njRotateX(NULL, pTree->RollAngle);
                njRotateY(NULL, twk->ang.y - pTree->TouchAngle);
                AL_DrawTree(pTree, pTree->pCopyObject);
            }
            else if(pTree->pLocalObject) {
                njRotateY(NULL, pTree->TouchAngle);
                njRotateX(NULL, pTree->RollAngle);
                njRotateY(NULL, twk->ang.y - pTree->TouchAngle);
                AL_DrawTree(pTree, pTree->pLocalObject);
            }
            break;
        case MD_DEAD2:
            SaveControl3D();
            SaveConstantAttr();
            OnControl3D(0x800);
            AddConstantAttr(0, 0x1800);
            RemoveConstantAttr(0, 0x6700);
            njColorBlendingMode(0, 8);
            njColorBlendingMode(1, 6);

            SetMaterial(pTree->scale - 1, 0, 0, 0);
            SetMaterial(0,0,0,0);

            LoadConstantAttr();
            LoadControl3D();
        default:
            if(pTree->pCopyObject) {
                njRotateY(NULL, pTree->TouchAngle);
                njRotateX(NULL, pTree->RollAngle);
                njRotateY(NULL, twk->ang.y - pTree->TouchAngle);
                njCnkEasyDrawObject(pTree->pCopyObject);
            }
            break;
    }
    
    njPopMatrixEx();

    if(pTree->state == TREE_ST_LOCAL) {
        if(!pTree->pShadowObject)
            lbl_0C5264B2(pTree);
    }
    else if(twk->mode > MD_ADULT) {
        lbl_0C526400(pTree);
    }
}

void ALO_GrowTreeDestructor(task *tp) {
    lbl_0C5251B2(tp->twp);
    ALW_CancelEntry(tp);
}

task* ALO_CreateGrowTree(NJS_POINT3 *pPos, TREE_SAVE_INFO *pInfo) {
    task* tp = CreateElementalTask(0, 2, ALO_GrowTreeExecutor, "ALO_GrowTreeExecutor");
    TREE_WORK* pTree = syCalloc(1, sizeof(TREE_WORK));
    taskwk* twk;

    tp->twp = pTree;
    twk = tp->twp;

    tp->Data2 = syCalloc(1, 64);
    tp->disp = ALO_GrowTreeDisplayer;
    tp->dest = ALO_GrowTreeDestructor;

    {
        int count = sizeof(colli_info)/sizeof(colli_info[0]);
        CCL_Init(tp, colli_info, count, 5);
    }
    
    lbl_0C525100(tp);

    pTree->kind = pInfo->kind;

    if(pInfo->kind == 0) pInfo->state = TREE_ST_PLANT;
    if(pInfo->kind == 10/* 8 on sa2 */) pInfo->state = TREE_ST_LOCAL;
    
    if(pInfo->state == TREE_ST_DEAD) pInfo->state = TREE_ST_PLANT;

    pTree->state = pInfo->state;
    pTree->water = 100 * pInfo->water;
    pTree->garden = 0;

    switch(pInfo->state) {
        case TREE_ST_PLANT:
            CCL_Enable(tp, CI_TREE_PLANT);
            
            twk->mode = MD_PLANT;
            twk->smode = 0;
            break;
        case TREE_ST_SEED:
            CCL_Enable(tp, CI_TREE_SENSOR);
            
            twk->mode = MD_SEED;
            twk->smode = 0;
            break;
        case TREE_ST_SAPLING: 
            CCL_Enable(tp, CI_TREE_SAPLING);
            CCL_Enable(tp, CI_TREE_SAPLING_SPHERE);
            
            pTree->growth = pInfo->param / 100.f;
            
            twk->mode = MD_SAPLING;
            twk->smode = 0;
            break;
        case TREE_ST_ADULT: {
            int i;
            
            CCL_Enable(tp, CI_TREE_FOR);
            CCL_Enable(tp, CI_TREE_LMO);
            CCL_Enable(tp, CI_TREE_SHADOW);
            CCL_Enable(tp, CI_TREE_SPHERE);
        
            pTree->growth = 1.f;
            pTree->life = 100 * pInfo->param;

            for(i = 0; i < FRUIT_COUNT; ++i) {
                pTree->fruit[i].growth = pInfo->FruitGrowth[i] / 100.f;
                pTree->fruit[i].SwingCounter = (Uint16)((int)(njRandom() * 13.999999f) + 11);
            }

            ObjectMovableInitialize(tp->twp, tp->Data2, 13);
            
            twk->mode = MD_ADULT;
            twk->smode = 0;
            
            break;
        }
        // cases are different after here on sa2b
        // case 4: break, case 5 is default i thiiiink?
        case TREE_ST_LOCAL: {
            int i;
            
            CCL_Enable(tp, CI_TREE_FOR);
            CCL_Enable(tp, CI_TREE_LMO);
            CCL_Enable(tp, CI_TREE_SHADOW);
            CCL_Enable(tp, CI_TREE_SPHERE);

            for(i = 0; i < FRUIT_COUNT; i++) {
                pTree->fruit[i].growth = pInfo->FruitGrowth[i] / 100.f;
                pTree->fruit[i].SwingCounter = (Uint16)((int)(njRandom() * 13.999999f) + 11);
            }

            pTree->garden = AL_GetStageNumber();
            pTree->water = 10000;

            ObjectMovableInitialize(tp->twp, tp->Data2, 13);
            
            twk->mode = MD_ADULT;
            twk->smode = 0;
            
            break;
        }
    }

    pTree->pos = *pPos;
    pTree->pos.y -= 1.1f;

    pTree->ang.y = pInfo->angy * 256;

    pTree->texlist = &AL_OBJECT_TEXLIST;
    
    if(pInfo->state >= TREE_ST_SAPLING) {
        if(pInfo->state == TREE_ST_LOCAL) {
            switch(pTree->garden) {
                case 1:
                    pTree->texlist = &lbl_0C60080C;
                    pTree->pLocalObject = &lbl_0C6181BC;
                    pTree->pShadowObject = NULL;
                    break;
                case 2:
                    pTree->texlist = &lbl_0C600964;
                    pTree->pLocalObject = &lbl_0C6189A8;
                    pTree->pShadowObject = NULL;
                    break;
                case 3:
                    pTree->texlist = &lbl_0C600AEC;
                    pTree->pLocalObject = &lbl_0C619038;
                    pTree->pShadowObject = &lbl_0C619104;
                    break;
            }
        }
        else {
            pTree->texlist = &AL_OBJECT_TEXLIST;
            
            lbl_0C52517C(pTree);
            
            AL_InterpolateObject(
                TreeObjectList[3 * (pTree->kind - 1)],
                TreeObjectList[3 * (pTree->kind - 1) + 1],
                pTree->pCopyObject,
                pTree->growth
            );
        }
    }

    pTree->pMySaveInfo = pInfo;
    ALW_Entry(ALW_CATEGORY_GROWTREE, tp, 0);
    
    return tp;
}

int AL_GetAllTreeCount(void) {
    int count = ALW_CountEntry(ALW_CATEGORY_GROWTREE);
    int counter = 0;
    while (count) {
        task* tp;
        TREE_WORK* pTree;
        
        --count;
        
        tp = ALW_GetTaskCount(ALW_CATEGORY_GROWTREE, count);
        pTree = GET_TREE_WORK(tp);
        
        if (pTree->state) {
            counter++;
        }
    }

    return counter;
}

int AL_GetGrowTreeCount(void) {
    int count = ALW_CountEntry(ALW_CATEGORY_GROWTREE);
    int counter = 0;
    while (count) {
        task* tp;
        TREE_WORK* pTree;
        int state;
        
        --count;
        
        tp = ALW_GetTaskCount(ALW_CATEGORY_GROWTREE, count);
        pTree = GET_TREE_WORK(tp);

        if (pTree->state != TREE_ST_PLANT && pTree->state != TREE_ST_LOCAL) {
            counter++;
        }
    }

    return counter;
}

int AL_GetEmptyTreeCount(void) {
    int count = ALW_CountEntry(ALW_CATEGORY_GROWTREE);
    int counter = 0;
    while (count) {
        task* tp;
        TREE_WORK* pTree;
        
        --count;
        
        tp = ALW_GetTaskCount(ALW_CATEGORY_GROWTREE, count);
        pTree = GET_TREE_WORK(tp);
        
        if (!pTree->state) {
            counter++;
        }
    }

    return counter;
}

void sub_C526EBC(task* tp) {
    TREE_WORK* pTree = GET_TREE_WORK(tp);
    TREE_SAVE_INFO* pInfo = pTree->pMySaveInfo;

    pInfo->kind = pTree->kind;

    switch(pTree->state) {
        case TREE_ST_PLANT:
        case TREE_ST_SEED:
            break;
        case TREE_ST_SAPLING:
            pInfo->param = pTree->growth * 100;
            break;
        case TREE_ST_ADULT:
            pInfo->param = pTree->life / 100;
            break;
        case TREE_ST_DEAD:
            pInfo->state = TREE_ST_PLANT;
            break;
        case TREE_ST_LOCAL:
            break;
    }

    pInfo->state = pTree->state;
    pInfo->water = pTree->water;

    pInfo->angy = (pTree->ang.y & 0xFF00) >> 8;

    pInfo->FruitGrowth[0] = pTree->fruit[0].growth * 100.f;
    pInfo->FruitGrowth[1] = pTree->fruit[1].growth * 100.f;
    pInfo->FruitGrowth[2] = pTree->fruit[2].growth * 100.f;
}

