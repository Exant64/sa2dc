#ifndef _CHAO_H_
#define _CHAO_H_

#include <task.h>
#include <motion.h>
#include <move.h>

#include "Chao/al_global.h"
#include "Chao/al_behavior.h"
#include "Chao/al_emotion.h"
#include "Chao/al_knowledge.h"
#include "Chao/al_intention.h"
#include "Chao/al_parameter.h"
#include <Chao/al_chao_info.h>
#include <Chao/al_shape.h>
#include <Chao/al_colli_kind.h>
#include <Chao/al_motion.h>
#include <Chao/al_world.h>
#include <Chao/al_face.h>
#include <Chao/al_icon.h>
#include <Chao/al_texlist.h>
#include <Chao/al_perception.h>
#include <Chao/al_parts.h>
#include <Chao/al_behavior/albhv.h>
#include <c_colli.h>

enum {
    AL_COLLI_DAKKO_FIELD = 0x0,
    AL_COLLI_BODY = 0x1,
    AL_COLLI_NADE_FIELD = 0x2,
    AL_COLLI_ATTACK = 0x3,
    AL_COLLI_HELD = 0x4,
};

float AL_Dist2FromAim(task* tp);

// ChaoFlag
#define CHAO_FLAG_0 BIT_0
#define CHAO_FLAG_MOVE_ENABLE BIT_1
#define CHAO_FLAG_MOTION_ENABLE BIT_2
#define CHAO_FLAG_CCL_ENABLE BIT_3
#define CHAO_FLAG_BHV_ENABLE BIT_4
#define CHAO_FLAG_BUYO_ENABLE BIT_5
#define CHAO_FLAG_UNK_6 BIT_6
#define CHAO_FLAG_UNK_7 BIT_7
#define CHAO_FLAG_UNK_8 BIT_8
#define CHAO_FLAG_DRAW_ENABLE BIT_9
#define CHAO_FLAG_HOLD_LEFT BIT_10
#define CHAO_FLAG_HOLD_RIGHT BIT_11
#define CHAO_FLAG_IS_VISIBLE BIT_12
#define CHAO_FLAG_RACE_BHV_ENABLE BIT_13
#define CHAO_FLAG_UNK_17 BIT_17
#define CHAO_FLAG_UNK_18 BIT_18

enum {
    AL_FORM_NORMAL = 0x0,
    AL_FORM_EGG_FOOT = 0x1,
    AL_FORM_OMOCHAO = 0x2,
    AL_FORM_MINIMAL = 0x3,
    AL_FORM_CHIBI = 0x4,
};

enum {
    EM_MD_PLEASURE = 0x0,
    EM_MD_ANGER = 0x1,
    EM_MD_SORROW = 0x2,
    EM_MD_FEAR = 0x3,
    EM_MD_SURPRISE = 0x4,
    EM_MD_PAIN = 0x5,
    EM_MD_RELAX = 0x6,
    EM_MD_TOTAL = 0x7,
    EM_ST_SLEEPY = 0x8,
    EM_ST_SLEEP_DEPTH = 0x9,
    EM_ST_HUNGER = 0xA,
    EM_ST_BREED = 0xB,
    EM_ST_TEDIOUS = 0xC,
    EM_ST_LONELY = 0xD,
    EM_ST_TIRE = 0xE,
    EM_ST_STRESS = 0xF,
    EM_ST_NOURISHMENT = 0x10,
    EM_ST_CONDITION = 0x11,
    EM_ST_THIRSTY = 0x12,
    EM_PER_CURIOSITY = 0x13,
    EM_PER_KINDNESS = 0x14,
    EM_PER_AGRESSIVE = 0x15,
    EM_PER_SLEEPY_HEAD = 0x16,
    EM_PER_SOLITUDE = 0x17,
    EM_PER_VITALITY = 0x18,
    EM_PER_GLUTTON = 0x19,
    EM_PER_REGAIN = 0x1A,
    EM_PER_SKILLFUL = 0x1B,
    EM_PER_CHARM = 0x1C,
    EM_PER_CHATTY = 0x1D,
    EM_PER_CALM = 0x1E,
    EM_PER_FICKLE = 0x1F,
    NB_EMOTION = 0x20
};
typedef struct AL_GROUP_OBJECT_LIST {
    NJS_CNK_OBJECT* Zero[40];
    NJS_CNK_OBJECT* Normal[40];
    NJS_CNK_OBJECT* Swim[40];
    NJS_CNK_OBJECT* Fly[40];
    NJS_CNK_OBJECT* Run[40];
    NJS_CNK_OBJECT* Power[40];
} AL_GROUP_OBJECT_LIST;

typedef struct AL_SHAPE {
    al_object* pObject;
    al_object* CurrObjectList[40];
    AL_GROUP_OBJECT_LIST* pObjectList;
    AL_GROUP_OBJECT_LIST* pObjectListH;
    AL_GROUP_OBJECT_LIST* pObjectListD;
    NJS_POINT3 BodyPos;
    NJS_POINT3 HeadPos;
    NJS_POINT3 LeftHandPos;
    NJS_POINT3 RightHandPos;
    NJS_POINT3 LeftFootPos;
    NJS_POINT3 RightFootPos;
    NJS_POINT3 MouthPos;
    NJS_POINT3 MouthVec;
    NJS_POINT3 LeftEyePos;
    NJS_POINT3 LeftEyeVec;
    NJS_POINT3 RightEyePos;
    NJS_POINT3 RightEyeVec;
    NJS_CNK_OBJECT* pLeftHandItemObject;
    NJS_TEXLIST* pLeftHandItemTexlist;
    float LeftHandItemScale;
    float LeftHandItemActiveFlag;
    NJS_CNK_OBJECT* pRightHandItemObject;
    NJS_TEXLIST* pRightHandItemTexlist;
    float RightHandItemScale;
    float RightHandItemActiveFlag;
    Uint16 Flag;
    Sint16 ColorNum;
    Sint16 EnvNum;
    unsigned int IconColor;
    float SclH;
    float SclV;
    float CamDist;
} AL_SHAPE;

typedef struct AL_FACE_CTRL {
    int EyeTimer;
    Sint16 EyeColorNum;
    Sint16 EyeCurrNum;
    Sint16 EyeDefaultNum;
    Sint16 pad;
    int MouthTimer;
    Sint16 MouthCurrNum;
    Sint16 MouthDefaultNum;
    float EyePosX;
    float EyePosY;
    float EyeSclX;
    float EyeSclY;
    unsigned int Flag;
    al_object* pEyeObject[2];
    al_object* pMouthObject;
    int EyeLidBlinkMode;
    int EyeLidBlinkTimer;
    int EyeLidBlinkAng;
    int EyeLidExpressionMode;
    int EyeLidExpressionTimer;
    int EyeLidExpressionDefaultCloseAng;
    int EyeLidExpressionCurrCloseAng;
    int EyeLidExpressionAimCloseAng;
    int EyeLidExpressionDefaultSlopeAng;
    int EyeLidExpressionCurrSlopeAng;
    int EyeLidExpressionAimSlopeAng;
} AL_FACE_CTRL;

typedef struct KW_BHV_ENTRY {
    eKW_BHV_KIND bhv;
} KW_BHV_ENTRY;

typedef struct AL_BEHAVIOR {
    Uint16 Flag;
    Uint16 Mode;
    Uint16 SubMode;
    Uint16 MoveMode;
    int InterruptFlag;
    int Timer;
    int SubTimer;
    int LimitTimer;
    // int BehaviorTimer; only on GC
    Uint16 Intention;
    Uint16 IntentionMode;
    Uint16 IntentionSubMode;
    unsigned int IntentionTimer[18];
    unsigned int IntervalTimer[128];
    int FreeWork;
    float MoveRadius;
    NJS_POINT3 BasePos;
    int (*PrevFunc)(task*);
    int nbBhvFuncEntry;
    int CurrBhvFuncNum;
    int (*BhvFuncList[16])(task*);
    int ReserveTimerList[16];
    int CurrKwBhvNum;
    KW_BHV_ENTRY KwBhvList[4];
    unsigned int dummy[31];
} AL_BEHAVIOR;

typedef struct AL_ICON_INFO {
    Uint16 Mode;
    Uint16 TexNum;
    Uint16 Timer;
    NJS_POINT3 Offset;
    NJS_POINT3 Pos;
    NJS_POINT3 Velo;
    NJS_POINT3 Scl;
    NJS_POINT3 SclSpd;
} AL_ICON_INFO;

typedef struct AL_ICON {
    Sint16 CurrType;
    Sint16 NextType;
    int Timer;
    int NextTimer;
    int PuniPhase;
    int PosPhase;
    Uint32 Color;
    Uint16 TexAnimNum;
    Uint16 TexAnimTimer;
    int ang;
    NJS_POINT3 Up;
    NJS_POINT3 Pos;
    AL_ICON_INFO Upper;
    AL_ICON_INFO Lower;
} AL_ICON;

typedef struct al_perception_link {
    Sint16 info[4];
    float dist;
    int InSightFlag;
    int HearFlag;
    int SmellFlag;
    al_entry_work* pEntry;
} al_perception_link;

typedef struct AL_PERCEPTION_INFO {
    Sint16 nbPerception;
    int InSightFlag;
    int HeardFlag;
    int SmellFlag;
    float NearestDist;
    Sint16 NearestNum;

    al_perception_link list[16];
} AL_PERCEPTION_INFO;

typedef struct AL_PERCEPTION {
    float SightRange;
    int SightAngle;
    int SightAngleHalf;
    float HearRange;
    float SmellRange;
    AL_PERCEPTION_INFO Player;
    AL_PERCEPTION_INFO Chao;
    AL_PERCEPTION_INFO Fruit;
    AL_PERCEPTION_INFO Tree;
    AL_PERCEPTION_INFO Toy;
    AL_PERCEPTION_INFO Sound;
} AL_PERCEPTION;

typedef struct chaowk {
    char mode;
    char smode;
    char btimer;
    Uint8 id;
    Sint16 flag;
    Sint16 wtimer;
    Angle3 ang;
    NJS_VECTOR pos;
    NJS_VECTOR scl;
    void* cwk;

    // 0x30
    CHAO_PARAM param;
    AL_GENE gene;

    unsigned int Timer;
    task* pMayu;
    task* pBookTask;
    int NestFlag;
    task* pAnyTask;
    task* pAimTask;
    int AimNum;
    int RememberNum;
    int pitch;
    float ClimbFirstPos;
    chao_info* pInfo;
    int field_428;
    int field_42C;
    int field_430;
    int val;
    int ChaoFlag;
    int ColliFormat;
    float CurrZone;
    MOTION_CTRL MotionCtrl;
    MOTION_CTRL MiniMotionCtrl;
    MOTION_TABLE MiniMotionTable[4];
    AL_BEHAVIOR Behavior;
    AL_SHAPE Shape;
    char pad[0x30 + 12];
    AL_FACE_CTRL Face;
    AL_ICON Icon;
    AL_PERCEPTION Perception;
} chaowk;

// includes minimum but not maximum
#define RAND_RANGE(min, max) ((Uint16)(min + (int)(njRandom() * ((max - min) + 1.0f))))

#define GET_CHAOWK(tp) ((chaowk*)tp->twp)
#define GET_CHAOPARAM(tp) (&GET_CHAOWK(tp)->param)

task* CreateChao(AL_GENE* pGene, CHAO_PARAM* pParam, AL_SHAPE_ELEMENT* pElement, NJS_POINT3* pPos, int angy,
                 chao_info* pInfo);

#endif