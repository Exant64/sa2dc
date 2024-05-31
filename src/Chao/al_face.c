#include <Chao/Chao.h>
#include "Chao/al_material.h"

const Uint16 AL_EyeTexIdList[] = { AL_EYE_TEXID_NORMAL,   AL_EYE_TEXID_KYA,      AL_EYE_TEXID_NAMU,
                                   AL_EYE_TEXID_TOHOHO,   AL_EYE_TEXID_NIKO,     AL_EYE_TEXID_BIKKURI,
                                   AL_EYE_TEXID_GURUGURU, AL_EYE_TEXID_SUYASUYA, AL_EYE_TEXID_BIKKURI,
                                   AL_EYE_TEXID_NORMAL,   AL_EYE_TEXID_NORMAL,   AL_EYE_TEXID_NCHAOS,
                                   AL_EYE_TEXID_HCHAOS,   AL_EYE_TEXID_DCHAOS };
const Uint16 AL_MouthTexIdList[13][2] = { { 0, 0 }, { 2, 1 },   { 3, 0 },   { 4, 0 },  { 5, 0 },   { 6, 0 },  { 7, 0 },
                                          { 9, 8 }, { 11, 10 }, { 13, 12 }, { 14, 0 }, { 16, 15 }, { 18, 17 } };
const Uint16 EyeColorTexIdList[] = { AL_EYE_TEXID_NORMAL, AL_EYE_TEXID_HERO,   AL_EYE_TEXID_DARK,
                                     AL_EYE_TEXID_NCHAOS, AL_EYE_TEXID_HCHAOS, AL_EYE_TEXID_DCHAOS,
                                     AL_EYE_TEXID_NORMAL, AL_EYE_TEXID_NORMAL, AL_EYE_TEXID_NORMAL };

void AL_FaceInit(task* tp) {
    chaowk* cwk = GET_CHAOWK(tp);
    AL_SHAPE* pShape = &cwk->Shape;
    AL_FACE_CTRL* pFace = &cwk->Face;
    CHAO_PARAM* pParam = &cwk->param;

    pFace->EyePosX = 0;
    pFace->EyePosY = 0;
    pFace->EyeSclX = 1;
    pFace->EyeSclY = 1;

    pFace->EyeLidExpressionMode = eEyeLidExpressionMode_Normal;
    pFace->EyeLidExpressionDefaultCloseAng = 0;
    pFace->EyeLidExpressionCurrCloseAng = 0;
    pFace->EyeLidExpressionAimCloseAng = 0;
    pFace->EyeLidExpressionDefaultSlopeAng = 0;
    pFace->EyeLidExpressionCurrSlopeAng = 0;
    pFace->EyeLidExpressionAimSlopeAng = 0;

    if (pParam->type >= TYPE_N_NORMAL && pParam->type <= TYPE_D_POWER) {
        switch ((pParam->type - TYPE_N_NORMAL) % 3) {
            case 0:
                pFace->EyeColorNum = AL_EYE_COLOR_NEUT;
                break;
            case 1:
                pFace->EyeColorNum = AL_EYE_COLOR_HERO;
                break;
            case 2:
                pFace->EyeColorNum = AL_EYE_COLOR_DARK;
                break;
        }
    } else {
        pFace->EyeColorNum = AL_EYE_COLOR_NEUT;
        switch (pParam->type) {
            case TYPE_N_CHAOS:
                pFace->EyeColorNum = AL_EYE_COLOR_N_CHAOS;
                break;
            case TYPE_H_CHAOS:
                pFace->EyeColorNum = AL_EYE_COLOR_H_CHAOS;
                break;
            case TYPE_D_CHAOS:
                pFace->EyeColorNum = AL_EYE_COLOR_D_CHAOS;
                break;
        }
    }

    if (AL_IsDark2(tp))
        pParam->body.DefaultEyeNum = AL_EYE_NUM_ANGER;

    switch (pParam->type) {
        case TYPE_N_CHAOS:
            pFace->EyeCurrNum = pFace->EyeDefaultNum = AL_EYE_NUM_NCHAOS;
            pFace->MouthCurrNum = pFace->MouthDefaultNum = 0;
            break;
        case TYPE_H_CHAOS:
            pFace->EyeCurrNum = pFace->EyeDefaultNum = AL_EYE_NUM_HCHAOS;
            pFace->MouthCurrNum = pFace->MouthDefaultNum = 0;
            break;
        case TYPE_D_CHAOS:
            pFace->EyeCurrNum = pFace->EyeDefaultNum = AL_EYE_NUM_DCHAOS;
            pFace->MouthCurrNum = pFace->MouthDefaultNum = 0;
            break;
        default:
            pFace->EyeCurrNum = pFace->EyeDefaultNum = pParam->body.DefaultEyeNum;
            pFace->MouthCurrNum = pFace->MouthDefaultNum = pParam->body.DefaultMouthNum;
            break;
    }

    pFace->pEyeObject[0] = pShape->CurrObjectList[AL_PART_EYE_L];
    pFace->pEyeObject[1] = pShape->CurrObjectList[AL_PART_EYE_R];
    pFace->pMouthObject = pShape->CurrObjectList[AL_PART_MOUTH];

    pFace->EyeLidBlinkMode = eEyeLidBlinkMode_Wait;
    pFace->EyeLidBlinkTimer = njRandom() * 300;
    pFace->EyeLidBlinkAng = 0;

    AL_FaceChangeEye(tp, pFace->EyeCurrNum);
    AL_FaceChangeMouth(tp, pFace->MouthCurrNum);
}

int AL_FaceEyelidGetCloseAng(task* tp) {
    AL_FACE_CTRL* pFace = &GET_CHAOWK(tp)->Face;
    return pFace->EyeLidBlinkAng + pFace->EyeLidExpressionCurrCloseAng - 0x4000;
}

int AL_FaceEyelidGetSlopeAng(task* tp) {
    AL_FACE_CTRL* pFace = &GET_CHAOWK(tp)->Face;
    return pFace->EyeLidExpressionCurrSlopeAng;
}

void AL_FaceEyeLidControl(task* tp) {
    AL_FACE_CTRL* pFace = &GET_CHAOWK(tp)->Face;

    pFace->EyeLidExpressionCurrCloseAng =
        AdjustAngle(pFace->EyeLidExpressionCurrCloseAng, pFace->EyeLidExpressionAimCloseAng, 0x400);
    pFace->EyeLidExpressionCurrSlopeAng =
        AdjustAngle(pFace->EyeLidExpressionCurrSlopeAng, pFace->EyeLidExpressionAimSlopeAng, 0x200);

    switch (pFace->EyeLidExpressionMode) {
        case eEyeLidExpressionMode_Normal:
            switch (AL_FaceGetEyeCurrNum(tp)) {
                case AL_EYE_NUM_NORMAL:
                case AL_EYE_NUM_TRON:
                case AL_EYE_NUM_ANGER:
                    break;
                default:
                    pFace->EyeLidExpressionMode = eEyeLidExpressionMode_None;
                    pFace->EyeLidExpressionCurrCloseAng = 0;
                    pFace->EyeLidBlinkMode = eEyeLidBlinkMode_Stop;
                    pFace->EyeLidBlinkAng = 0;
                    break;
            }
            break;
        case eEyeLidExpressionMode_None:
            switch (AL_FaceGetEyeCurrNum(tp)) {
                case AL_EYE_NUM_NORMAL:
                case AL_EYE_NUM_TRON:
                case AL_EYE_NUM_ANGER:
                    pFace->EyeLidExpressionMode = eEyeLidExpressionMode_Normal;
                    break;
            }

            break;
    }

    switch (pFace->EyeLidBlinkMode) {
        case eEyeLidBlinkMode_Open:
            if (pFace->EyeLidBlinkAng < 0x1000) {
                pFace->EyeLidBlinkAng = 0;
                pFace->EyeLidBlinkMode = eEyeLidBlinkMode_Wait;
                if (njRandom() < 0.85) {
                    pFace->EyeLidBlinkTimer = (njRandom() * 400 + 100);
                } else {
                    pFace->EyeLidBlinkTimer = (njRandom() * 5);
                }
            } else
                pFace->EyeLidBlinkAng -= 0x1000;
            break;
        case eEyeLidBlinkMode_Close:
            pFace->EyeLidBlinkAng += 0x1000;
            if (pFace->EyeLidExpressionCurrCloseAng + pFace->EyeLidBlinkAng > 0x8000) {
                pFace->EyeLidBlinkAng = 0x8000 - pFace->EyeLidExpressionCurrCloseAng;
                pFace->EyeLidBlinkMode = eEyeLidBlinkMode_Open;
            }
            break;
        case eEyeLidBlinkMode_Wait:
            if (pFace->EyeLidBlinkTimer-- <= 0) {
                pFace->EyeLidBlinkTimer = 0;
                pFace->EyeLidBlinkMode = eEyeLidBlinkMode_Close;
            }
            break;
        case eEyeLidBlinkMode_Stop:
            if (pFace->EyeLidExpressionMode != eEyeLidExpressionMode_None) {
                pFace->EyeLidBlinkMode = eEyeLidBlinkMode_Wait;
            }
            break;
    }
}

void AL_FaceControl(task* tp) {
    AL_FACE_CTRL* pFace = &GET_CHAOWK(tp)->Face;

    if (pFace->EyeTimer > 0) {
        pFace->EyeTimer--;
    } else {
        if (pFace->EyeTimer == -1) {
            ;
        } else if (pFace->EyeTimer == 0) {
            AL_FaceReturnDefaultEye(tp);
        }
    }

    if (pFace->MouthTimer > 0) {
        pFace->MouthTimer--;
    } else {
        if (pFace->MouthTimer == -1) {
            ;
        } else if (pFace->MouthTimer == 0) {
            AL_FaceReturnDefaultMouth(tp);
        }
    }
    AL_FaceEyeLidControl(tp);

    switch (GET_CHAOPARAM(tp)->type) {
        case TYPE_N_CHAOS:
        case TYPE_H_CHAOS:
        case TYPE_D_CHAOS:
            break;
        default:
            if (AL_FaceGetEyeCurrNum(tp) == AL_EYE_NUM_TRON) {
                if (pFace->EyeLidExpressionCurrCloseAng > 0x3C00) {
                    Sint16 id = AL_EyeTexIdList[AL_EYE_NUM_BIKKURI];
                    AL_MatChangeObjectTexture(pFace->pEyeObject[0], id);
                    AL_MatChangeObjectTexture(pFace->pEyeObject[1], id);
                }
            }
            break;
    }
}

Sint16 AL_FaceGetEyeCurrNum(task* tp) {
    AL_FACE_CTRL* pFace = &GET_CHAOWK(tp)->Face;
    return pFace->EyeCurrNum;
}

void AL_FaceChangeEye(task* tp, eAL_EYE_NUM EyeNum) {
    AL_FaceSetEye(tp, EyeNum, -1);
}

void AL_FaceSetEye(task* tp, eAL_EYE_NUM EyeNum, int timer) {
    AL_FACE_CTRL* pFace = &GET_CHAOWK(tp)->Face;
    Uint16 texid;

    pFace->EyeCurrNum = EyeNum;
    pFace->EyeTimer = timer;

    switch (AL_FaceGetEyeCurrNum(tp)) {
        default:
            pFace->EyeLidExpressionAimCloseAng = 0;
            pFace->EyeLidExpressionAimSlopeAng = 0;
            break;
        case AL_EYE_NUM_TRON:
            pFace->EyeLidExpressionAimCloseAng = NJM_DEG_ANG(90);
            pFace->EyeLidExpressionAimSlopeAng = 0;
            break;
        case AL_EYE_NUM_NORMAL:
            if (pFace->EyeDefaultNum == AL_EYE_NUM_ANGER) {
                pFace->EyeLidExpressionAimCloseAng = NJM_DEG_ANG(75);
                pFace->EyeLidExpressionAimSlopeAng = NJM_DEG_ANG(20);
            } else {
                pFace->EyeLidExpressionAimCloseAng = 0;
                pFace->EyeLidExpressionAimSlopeAng = 0;
            }
            break;
        case AL_EYE_NUM_ANGER:
            pFace->EyeLidExpressionAimCloseAng = NJM_DEG_ANG(75);
            pFace->EyeLidExpressionAimSlopeAng = NJM_DEG_ANG(20);
            break;
    }

    switch (AL_FaceGetEyeCurrNum(tp)) {
        case AL_EYE_NUM_NORMAL:
        case AL_EYE_NUM_TRON:
        case AL_EYE_NUM_ANGER:
            texid = EyeColorTexIdList[pFace->EyeColorNum];
            break;
        default:
            texid = AL_EyeTexIdList[EyeNum];
            break;
    }

    switch (GET_CHAOPARAM(tp)->type) {
        case TYPE_N_CHAOS:
        case TYPE_H_CHAOS:
        case TYPE_D_CHAOS:
            if (AL_FaceGetEyeCurrNum(tp) == AL_EYE_NUM_SUYASUYA)
                texid = AL_EyeTexIdList[EyeNum];
            else
                texid = EyeColorTexIdList[pFace->EyeColorNum];
            break;
    }

    AL_MatChangeObjectTexture(pFace->pEyeObject[0], texid);
    AL_MatChangeObjectTexture(pFace->pEyeObject[1], texid);
}

void AL_FaceChangeMouth(task* tp, eAL_MOUTH_NUM MouthNum) {
    AL_FaceSetMouth(tp, MouthNum, -1);
}

void AL_FaceSetMouth(task* tp, eAL_MOUTH_NUM MouthNum, int timer) {
    AL_FACE_CTRL* pFace = &GET_CHAOWK(tp)->Face;
    Uint16* texid;

    pFace->MouthTimer = timer;

    switch (GET_CHAOPARAM(tp)->type) {
        case TYPE_N_CHAOS:
        case TYPE_H_CHAOS:
        case TYPE_D_CHAOS:
            texid = AL_MouthTexIdList[0];
            AL_MatChangeObjectTexture2(pFace->pMouthObject, texid, 2);
            break;
        default:
            texid = AL_MouthTexIdList[MouthNum];
            AL_MatChangeObjectTexture2(pFace->pMouthObject, texid, 2);
            break;
    }
}

Sint16 AL_FaceGetEyeDefaultNum(task* tp) {
    chaowk* cwk = GET_CHAOWK(tp);
    AL_FACE_CTRL* pFace = &cwk->Face;
    return pFace->EyeDefaultNum;
}

Sint16 AL_FaceGetMouthDefaultNum(task* tp) {
    chaowk* cwk = GET_CHAOWK(tp);
    AL_FACE_CTRL* pFace = &cwk->Face;
    return pFace->MouthDefaultNum;
}

void AL_FaceReturnDefaultEye(task* tp) {
    chaowk* cwk = GET_CHAOWK(tp);
    AL_FACE_CTRL* pFace = &cwk->Face;
    AL_FaceChangeEye(tp, pFace->EyeDefaultNum);
}

void AL_FaceReturnDefaultMouth(task* tp) {
    chaowk* cwk = GET_CHAOWK(tp);
    AL_FACE_CTRL* pFace = &cwk->Face;
    AL_FaceChangeMouth(tp, pFace->MouthDefaultNum);
}