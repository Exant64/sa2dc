#include <task.h>
#include <Chao/Chao.h>
#include <effect2.h>

extern NJS_CNK_OBJECT object_al_pumpkinhead_pumpkinhead[];
extern NJS_CNK_OBJECT object_al_skullhead_skullhead[];
extern NJS_CNK_OBJECT object_al_egghead_egghead[];
extern NJS_CNK_OBJECT* ObakeBodyObjectList[];

extern NJS_CNK_OBJECT* pMedalObjectList[];

extern short Chao_NodeIndex;
extern int Chao_UseEasy;

static void AL_DrawSub(task* tp, al_object* pObject) {
    chaowk* cwk = GET_CHAOWK(tp);
    AL_SHAPE* pShape = &cwk->Shape;
    CHAO_PARAM* pParam = &cwk->param;
    al_model* pModel = pObject->pModel;

    njPushMatrixEx();
    AL_CalcMotionMartix(pObject);

    if (pObject->pItemObject) {
        float scale = pObject->ItemScale;

        njSetTexture(pObject->pItemTexlist);

        njPushMatrixEx();
        if (pObject->ItemOffsetFlag) {
            Angle3* Ang = &pObject->ItemOffsetAng;
            njTranslateEx(&pObject->ItemOffsetPos);
            njRotateXYZ(0, Ang->x, Ang->y, Ang->z);
        }
        njScale(0, scale, scale, scale);
        njCnkSimpleDrawObject(pObject->pItemObject);
        njPopMatrixEx();
    }

    if (Chao_NodeIndex == AL_PART_HEAD && pParam->body.ObakeHead) {
        njSetTexture(&AL_BODY_TEXLIST);
        switch (GET_CHAOPARAM(tp)->body.ObakeHead) {
            case 1:
                njCnkEasyDrawObject(object_al_pumpkinhead_pumpkinhead);
                break;
            case 2:
                njCnkEasyDrawObject(object_al_skullhead_skullhead);
                break;
            case 3:
            default:
                njCnkEasyDrawObject(object_al_egghead_egghead);
                break;
        }
    } else if (Chao_NodeIndex == AL_PART_BODY && pParam->body.ObakeBody && pParam->type < TYPE_N_CHAOS) {
        njSetTexture(&AL_BODY_TEXLIST);
        alpalSetBank(tp, cwk->id);
        OnControl3D(NJD_CONTROL_3D_CONSTANT_MATERIAL);

        {
            Uint8* PList = (Uint8*)(pModel->PList) + 4;
            float b = PList[0] / 255.0f;
            float g = PList[1] / 255.0f;
            float r = PList[2] / 255.0f;
            float a = PList[3] / 255.0f;
            SetMaterial(a, r, g, b);
        }

        njCnkSimpleDrawObject(ObakeBodyObjectList[pParam->type]);

        OffControl3D(NJD_CONTROL_3D_CONSTANT_MATERIAL);
        SetMaterial(0, 0, 0, 0);
    } else if (Chao_NodeIndex == AL_PART_NAVEL && pParam->body.MedalNum > 0) {
        njSetTexture(&AL_JEWEL);
        njCnkSimpleDrawObject(pMedalObjectList[pParam->body.MedalNum]);
    }
#define NODECHECK(a1) Chao_NodeIndex == a1
    else if (pParam->body.ObakeHead &&
             (NODECHECK(AL_PART_EYE_L) || NODECHECK(AL_PART_EYE_R) || NODECHECK(AL_PART_EYELID_L) ||
              NODECHECK(AL_PART_EYELID_R) || NODECHECK(AL_PART_FEELER_L) || NODECHECK(AL_PART_FEELER_R) ||
              NODECHECK(AL_PART_EAR_L) || NODECHECK(AL_PART_EAR_R) || NODECHECK(AL_PART_HORN_L) ||
              NODECHECK(AL_PART_HORN_R) || NODECHECK(AL_PART_MOUTH) || NODECHECK(AL_PART_FOREHEAD))) {

    } else if (pParam->body.ObakeBody && pParam->type < TYPE_N_CHAOS &&
               (NODECHECK(AL_PART_WING_L) || NODECHECK(AL_PART_WING_R) || NODECHECK(AL_PART_REG_L) ||
                NODECHECK(AL_PART_REG_R) || NODECHECK(AL_PART_TAIL))) {

    } else if (pObject->pPartsObject && pParam->type < TYPE_N_CHAOS) {
        njSetTexture(&AL_MINI_PARTS_TEX);
        njCnkEasyDrawObject(pObject->pPartsObject);
    } else if (pModel) {
        int slope = AL_FaceEyelidGetSlopeAng(tp);
        int close = AL_FaceEyelidGetCloseAng(tp);

        if (NODECHECK(AL_PART_EYELID_L)) {
            if (pShape->Flag & AL_SHAPE_FLAG_ENV)
                njSetTexture(&AL_ENV_TEX);
            else
                njSetTexture(&AL_BODY_TEXLIST);

            alpalSetBank(tp, cwk->id);
            njRotateZ(0, slope);
            njRotateX(0, close);
        } else if (NODECHECK(AL_PART_EYELID_R)) {
            if (pShape->Flag & AL_SHAPE_FLAG_ENV)
                njSetTexture(&AL_ENV_TEX);
            else
                njSetTexture(&AL_BODY_TEXLIST);

            alpalSetBank(tp, cwk->id);

            slope = -slope;

            njRotateZ(0, slope);
            njRotateX(0, close);
        } else if (NODECHECK(AL_PART_MOUTH)) {
            njSetTexture(&AL_MOUTH);
        } else if (NODECHECK(AL_PART_EYE_L) || NODECHECK(AL_PART_EYE_R)) {
            njSetTexture(&AL_EYE);
            if (Chao_NodeIndex == AL_PART_EYE_L) {
                AL_FACE_CTRL* pFace = &GET_CHAOWK(tp)->Face;
                njTranslate(0, pFace->EyePosX, pFace->EyePosY, 0);
            } else {
                AL_FACE_CTRL* pFace = &GET_CHAOWK(tp)->Face;
                njTranslate(0, -pFace->EyePosX, pFace->EyePosY, 0);
            }

        } else {
            if (pShape->Flag & AL_SHAPE_FLAG_ENV)
                njSetTexture(&AL_ENV_TEX);
            else
                njSetTexture(&AL_BODY_TEXLIST);

            alpalSetBank(tp, cwk->id);
        }

        if (Chao_NodeIndex == AL_PART_EYELID_L || Chao_NodeIndex == AL_PART_EYELID_R) {
            int close = AL_FaceEyelidGetCloseAng(tp);
            if (close != 0xFFFFC000) {
                if (Chao_UseEasy)
                    njCnkEasyDrawModel(pModel);
                else
                    njCnkSimpleDrawModel(pModel);
            }
        } else {
            if (Chao_UseEasy)
                njCnkEasyDrawModel(pModel);
            else
                njCnkSimpleDrawModel(pModel);
        }
    }

    Chao_NodeIndex++;
    if (pObject->pChild)
        AL_DrawSub(tp, pObject->pChild);

    njPopMatrixEx();

    if (pObject->pSibling)
        AL_DrawSub(tp, pObject->pSibling);
}

extern NJS_CNK_MODEL model_kage_marukage_marukage[];

static void AL_ShadowDraw(task* tp) {
    chaowk* entity = GET_CHAOWK(tp);
    chaowk* cwk = GET_CHAOWK(tp);
    AL_SHAPE* pShape = &cwk->Shape;
    int unused;
    float dist;
    al_entry_work* pEntry = ALW_ENTRY_WORK(tp);

    if(pEntry) dist = pEntry->CamDist;
    else dist = 300.f;

    if (AL_GetStageNumber() == 4) {
        njPushMatrixEx();
        njTranslateEx(&pShape->HeadPos);
        njRotateY(0, entity->ang.y);
        njScale(0, 1.6f, 0.7, 1.5);
        njCnkModDrawModel(model_kage_marukage_marukage);
        njPopMatrixEx();
        njPushMatrixEx();
        njTranslateEx(&pShape->BodyPos);
        njRotateY(0, entity->ang.y);
        njScale(0, 1.4f, 0.7, 1.4f);
        njCnkModDrawModel(model_kage_marukage_marukage);
        njPopMatrixEx();
    }
    else if (dist < GET_GLOBAL()->CamDistShadowCutLev4) {
        njPushMatrixEx();
        njTranslateEx(&pShape->HeadPos);
        njRotateY(0, entity->ang.y);
        njScale(0, 1.6f, 0.7, 1.5);
        njCnkModDrawModel(model_kage_marukage_marukage);
        njPopMatrixEx();

        if (dist < GET_GLOBAL()->CamDistShadowCutLev2) {
            njPushMatrixEx();
            if (pShape->Flag & AL_SHAPE_FLAG_SHADOW)
                pShape->LeftHandPos.y = entity->pos.y + 0.01f;
            njTranslateEx(&pShape->LeftHandPos);
            njRotateY(0, entity->ang.y);
            njScale(0, 0.65f, 0.6f, 0.65f);
            njCnkModDrawModel(model_kage_marukage_marukage);
            njPopMatrixEx();

            njPushMatrixEx();
            if (pShape->Flag & AL_SHAPE_FLAG_SHADOW)
                pShape->RightHandPos.y = entity->pos.y + 0.01f;
            njTranslateEx(&pShape->RightHandPos);
            njRotateY(0, entity->ang.y);
            njScale(0, 0.65f, 0.6f, 0.65f);
            njCnkModDrawModel(model_kage_marukage_marukage);
            njPopMatrixEx();
            if (dist < GET_GLOBAL()->CamDistShadowCutLev1) {
                njPushMatrixEx();
                njTranslateEx(&pShape->BodyPos);
                njRotateY(0, entity->ang.y);
                njScale(0, 1.4f, 0.7, 1.4f);
                njCnkModDrawModel(model_kage_marukage_marukage);
                njPopMatrixEx();

                if (!GET_CHAOPARAM(tp)->body.ObakeBody) {
                    njPushMatrixEx();
                    njTranslateEx(&pShape->LeftFootPos);
                    njRotateY(0, entity->ang.y);
                    njScale(0, 0.6f, 0.6f, 0.9f);
                    njCnkModDrawModel(model_kage_marukage_marukage);
                    njPopMatrixEx();

                    njPushMatrixEx();
                    njTranslateEx(&pShape->RightFootPos);
                    njRotateY(0, entity->ang.y);
                    njScale(0, 0.6f, 0.6f, 0.9f);
                    njCnkModDrawModel(model_kage_marukage_marukage);
                    njPopMatrixEx();
                }

                njPushMatrixEx();
                {
                    NJS_POINT3* IconPos = &GET_CHAOWK(tp)->Icon.Lower.Pos;
                    if ((pShape->Flag & AL_SHAPE_FLAG_SHADOW))
                        njTranslate(0, IconPos->x, entity->pos.y + 0.001f, IconPos->z);
                    else
                        njTranslateEx(IconPos);
                }
                njRotateY(0, entity->ang.y);
                njScale(0, 0.5, 0.7, 0.5);
                njCnkModDrawModel(model_kage_marukage_marukage);
                njPopMatrixEx();
            }
        }
    }
}

void AL_Draw(task* tp) {
    chaowk* entity = GET_CHAOWK(tp);
    chaowk* cwk = GET_CHAOWK(tp);
    MOTION_CTRL* pMotion = &cwk->MotionCtrl;
    AL_SHAPE* pShape = &cwk->Shape;

    if (!AL_IsOnScreen3(tp, 5.2f, 4.5f, 2.9f)) {
        GET_CHAOWK(tp)->ChaoFlag &= ~CHAO_FLAG_IS_VISIBLE;
    } else {
        GET_CHAOWK(tp)->ChaoFlag |= CHAO_FLAG_IS_VISIBLE;

        if (GET_CHAOWK(tp)->ChaoFlag & CHAO_FLAG_DRAW_ENABLE) {
            CHAO_PARAM* pParam = &cwk->param;

            Chao_UseEasy = 0;
            switch (pParam->type) {
                case TYPE_H_NORMAL:
                case TYPE_H_SWIM:
                case TYPE_H_FLY:
                case TYPE_H_RUN:
                case TYPE_H_POWER:
                    if (pParam->body.JewelNum == AL_JEWEL_NONE) // 713
                        Chao_UseEasy = 1;                       // 714
                        break;
            }

            if (pParam->body.ColorNum) {
                Chao_UseEasy = 0;
            }
            else {}

            SaveControl3D();
            OffControl3D(0x204);
            SaveConstantAttr();
            SetMaterial(0, 0, 0, 0);
            Chao_NodeIndex = 0;
            AL_InitCalcMotionMatrix(tp);

            njSetTexture(&AL_BODY_TEXLIST);
            alpalSetBank(tp, entity->id);

            if (pShape->Flag & AL_SHAPE_FLAG_SHADOW) {
                OnControl3D(0x2400);
            }

            njPushMatrixEx();
            if (entity->flag & 0x8000) {
                njTranslate(0, 0, -1, 0);
            }
            njTranslateV(0, &entity->pos);
            njRotateZ(0, entity->ang.z);
            njRotateX(0, entity->ang.x);
            njRotateY(0, entity->ang.y);

            {
                float v = 1;
                njTranslate(0, 0, -v, 0);
                njRotateX(0, cwk->pitch);

                njTranslate(0, 0, v, 0);
            }

            AL_DrawSub(tp, cwk->Shape.pObject);
            njPopMatrixEx();

            if (pShape->pLeftHandItemObject) {
                float scale = pShape->LeftHandItemScale;

                njSetTexture(pShape->pLeftHandItemTexlist);

                njPushMatrixEx();
                njTranslateEx(&pShape->LeftHandPos);
                njRotateY(0, GET_CHAOWK(tp)->ang.y);
                njScale(0, scale, scale, scale);
                njCnkSimpleDrawObject(pShape->pLeftHandItemObject);
                njPopMatrixEx();
            }
            if (pShape->pRightHandItemObject) {
                float scale = pShape->RightHandItemScale;

                njSetTexture(pShape->pRightHandItemTexlist);

                njPushMatrixEx();
                njTranslateEx(&pShape->RightHandPos);
                njRotateY(0, GET_CHAOWK(tp)->ang.y);
                njScale(0, scale, scale, scale);
                njCnkSimpleDrawObject(pShape->pRightHandItemObject);
                njPopMatrixEx();
            }

            AL_IconDraw(tp);
            AL_ShadowDraw(tp);

            OffControl3D(0x2400);
            SetMaterial(0, 0, 0, 0);
            LoadConstantAttr();
            LoadControl3D();
        }
    }
}
