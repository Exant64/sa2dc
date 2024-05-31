#ifndef _AL_FACE_H_
#define _AL_FACE_H_

enum {
    eEyeLidExpressionMode_Normal = 0x0,
    eEyeLidExpressionMode_None = 0x1,
};

enum {
    eEyeLidBlinkMode_Open = 0x0,
    eEyeLidBlinkMode_Close = 0x1,
    eEyeLidBlinkMode_Wait = 0x2,
    eEyeLidBlinkMode_Stop = 0x3,
};

typedef enum eAL_EYE_TEXNUM {
    AL_EYE_TEXID_NORMAL = 0x0,
    AL_EYE_TEXID_KYA = 0x1,
    AL_EYE_TEXID_NAMU = 0x2,
    AL_EYE_TEXID_TOHOHO = 0x3,
    AL_EYE_TEXID_NIKO = 0x4,
    AL_EYE_TEXID_BIKKURI = 0x5,
    AL_EYE_TEXID_GURUGURU = 0x6,
    AL_EYE_TEXID_SUYASUYA = 0x7,
    AL_EYE_TEXID_DARK = 0x8,
    AL_EYE_TEXID_HERO = 0x9,
    AL_EYE_TEXID_NCHAOS = 10,
    AL_EYE_TEXID_HCHAOS = 11,
    AL_EYE_TEXID_DCHAOS = 12,
} eAL_EYE_TEXNUM;

typedef enum eAL_MOUTH_NUM {
    AL_MOUTH_NUM_NONE = 0x0,
    AL_MOUTH_NUM_SHARK = 0x1,
    AL_MOUTH_NUM_HOYO = 0x2,
    AL_MOUTH_NUM_NIKO = 0x3,
    AL_MOUTH_NUM_MUSU = 0x4,
    AL_MOUTH_NUM_WAAI = 0x5,
    AL_MOUTH_NUM_UEEN = 0x6,
    AL_MOUTH_NUM_NEE = 0x7,
    AL_MOUTH_NUM_KOIKE = 0x8,
    AL_MOUTH_NUM_GEE = 0x9,
    AL_MOUTH_NUM_MUU = 0xA,
    AL_MOUTH_NUM_UWAAN = 0xB,
    AL_MOUTH_NUM_INO = 0xC,
    AL_MOUTH_NUM_END = 0xD,
} eAL_MOUTH_NUM;

typedef enum eAL_EYE_NUM {
    AL_EYE_NUM_NORMAL = 0x0,
    AL_EYE_NUM_KYA = 0x1,
    AL_EYE_NUM_NAMU = 0x2,
    AL_EYE_NUM_TOHOHO = 0x3,
    AL_EYE_NUM_NIKO = 0x4,
    AL_EYE_NUM_BIKKURI = 0x5,
    AL_EYE_NUM_GURUGURU = 0x6,
    AL_EYE_NUM_SUYASUYA = 0x7,
    AL_EYE_NUM_SHIROME = 0x8,
    AL_EYE_NUM_TRON = 0x9,
    AL_EYE_NUM_ANGER = 10,
    AL_EYE_NUM_NCHAOS = 11,
    AL_EYE_NUM_HCHAOS = 12,
    AL_EYE_NUM_DCHAOS = 13,
    AL_EYE_NUM_END = 0xE,
} eAL_EYE_NUM;

typedef enum eAL_EYE_COLOR {
    AL_EYE_COLOR_NEUT = 0x0,
    AL_EYE_COLOR_HERO = 0x1,
    AL_EYE_COLOR_DARK = 0x2,
    AL_EYE_COLOR_N_CHAOS = 0x3,
    AL_EYE_COLOR_H_CHAOS = 0x4,
    AL_EYE_COLOR_D_CHAOS = 0x5,
    AL_EYE_COLOR_BLUE = 0x6,
    AL_EYE_COLOR_GREEN = 0x7,
    AL_EYE_COLOR_RED = 0x8,
} eAL_EYE_COLOR;

void AL_FaceChangeEye(task* tp, eAL_EYE_NUM EyeNum);
void AL_FaceSetEye(task* tp, eAL_EYE_NUM EyeNum, int timer);
void AL_FaceChangeMouth(task* tp, eAL_MOUTH_NUM MouthNum);
void AL_FaceSetMouth(task* tp, eAL_MOUTH_NUM MouthNum, int timer);

void AL_FaceReturnDefaultEye(task* tp);
void AL_FaceReturnDefaultMouth(task* tp);

Sint16 AL_FaceGetEyeCurrNum(task* tp);
Sint16 AL_FaceGetMouthDefaultNum(task* tp);

#endif