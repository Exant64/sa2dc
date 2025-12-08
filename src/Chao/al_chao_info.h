#ifndef _AL_CHAO_INFO_H_
#define _AL_CHAO_INFO_H_

#include <task.h>

typedef struct GARDEN_ID {
    Uint32 id[3];
} GARDEN_ID;

typedef struct AL_TIME {
    char minute;
    char second;
    char frame;
} AL_TIME;

typedef struct AL_RACE_PERSONAL_INFO {
    AL_TIME PersonalRecord[10];
    char nbWin[10];
    Uint16 MedalFlag;
} AL_RACE_PERSONAL_INFO;

typedef struct AL_EMOTION {
    Uint16 Flag;
    Uint16 MoodTimer;
    Uint16 IllTimer;
    Uint16 Timer;
    char Mood[8];
    Uint16 State[11];
    char Personality[13];
    char Taste;
    char Tv;
    char Music;
    char IllState[6];
} AL_EMOTION;

typedef struct AL_PARTS {
    unsigned int MinimalFlag;
    char MinimalParts[8];
} AL_PARTS;

typedef struct AL_ITEM_INFO {
    NJS_CNK_OBJECT* pObject;
    NJS_TEXLIST* pTexlist;
    NJS_POINT3 OffsetPos;
    Angle3 OffsetAng;
} AL_ITEM_INFO;

typedef struct CHAO_ID {
    unsigned int id;
    GARDEN_ID gid;
} CHAO_ID;

typedef struct AL_GENE {
    CHAO_ID ChaoID;
    Uint8 Abl[7][2];
    Uint8 LifeTime[2];
    Sint8 HPos[2];
    Sint8 VPos[2];
    Sint8 APos[2];
    char Personality[13][2];
    char Taste[2];
    char Tv[2];
    char Music[2];
    char Color[2];
    char Jewel[2];
    char Multi[2];

} AL_GENE;

typedef struct AL_KNOWLEDGE_PLAYER {
    Sint8 like;
    Uint8 fear;
    Uint16 distance;
    Uint16 meet;
} AL_KNOWLEDGE_PLAYER;

typedef struct AL_KNOWLEDGE_CHAO {
    CHAO_ID id;
    char like;
    char fear;
    Uint16 distance;
    int meet;
} AL_KNOWLEDGE_CHAO;

typedef struct AL_KNOWLEDGE_OTHER {
    Sint8 like;
} AL_KNOWLEDGE_OTHER;

typedef struct AL_KNOWLEDGE {
    Uint8 ArtFlag;
    Uint8 DanceFlag;
    Uint8 SongFlag;
    Uint8 MusicFlag;
    Uint16 SToyFlag;
    Uint16 LToyFlag;
    int KwTimer;
    AL_KNOWLEDGE_PLAYER player[6];
    AL_KNOWLEDGE_CHAO chao[20];
    AL_KNOWLEDGE_OTHER bhv[120]; // 96?
} AL_KNOWLEDGE;

typedef struct AL_BODY_INFO {
    float HPos;
    float VPos;
    float APos;
    float AimHPos;
    float AimVPos;
    float AimAPos;
    float growth;
    char pad1;
    char unk[30];
    char pad2[10];
    Uint8 DefaultEyeNum;
    Uint8 DefaultMouthNum;
    char HonbuNum;
    char HonbuColorNum;
    char pad4[3];
    Uint8 ObakeHead;
    Uint8 ObakeBody;
    Uint8 MedalNum;
    Uint8 ColorNum;
    Uint8 JewelNum;
    Uint8 MultiNum;
    Uint8 BodyType;
    Uint8 FormSubNum;
    char a[3];
} AL_BODY_INFO;

typedef struct AL_SHAPE_ELEMENT {
  Sint8 type;
  Sint8 DefaultEyeNum;
  Sint8 DefaultMouthNum;
  Sint8 HonbuNum;
  Sint8 ObakeHead;
  Sint8 ObakeBody;
  Sint8 MedalNum;
  Sint8 ColorNum;
  Sint8 JewelNum;
  Sint8 MultiNum;
  Sint8 MinimalParts[8];
  Sint16 HPos;
  Sint16 VPos;
  Sint16 APos;
  Sint16 Growth;
  char name[8];
  Uint16 Skill[7];
} AL_SHAPE_ELEMENT;

typedef struct CHAO_PARAM {
    Uint8 type;
    Uint8 place;
    Sint16 like;

    char ClassNum;
    char unk_2;
    char pad_[2];
    char name[8];
    char SkillRelated[8];
    Uint16 Skill[7];
    Uint16 age;
    Uint16 nbSucceed;

    Uint16 LifeTimer;
    char unk_arr[8];
    char pad2[8];
    int thing; // used in al_gene GeneAnalyze Skill[6] thing
    char pad3[4];

    Uint16 Abl[7];
    Uint16 old;

    Uint16 life;
    Uint16 LifeMax;

    AL_BODY_INFO body;
    AL_PARTS Parts;
    AL_EMOTION Emotion;
    AL_KNOWLEDGE knowledge;
} CHAO_PARAM;

typedef struct CHAO_PARAM_PADDING {
    CHAO_PARAM param;
    char pad[0x380 - 0x368];
} CHAO_PARAM_PADDING;

typedef struct gene_padding {
    char pad[0x368];
    AL_GENE gene;
} gene_padding;

typedef union chao_info_ {
    CHAO_PARAM param;
    gene_padding gene_pad;
} chao_info_;

// this is 1000 bytes, and AL_GENE should be at 0x368, but its at 0x380 for some reason??
typedef struct chao_info {
    CHAO_PARAM param;

    AL_GENE gene;
    char pad[1000 - sizeof(CHAO_PARAM) - sizeof(AL_GENE)]; // pad to 1000 bytes?
} chao_info;

#define GET_INFO_PARAM(info) (&(((chao_info_*)info)->param))
#define GET_INFO_GENE(info) (&((&((chao_info_*)info)->gene_pad)->gene))

#endif