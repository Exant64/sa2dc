#ifndef _AL_PARAMETER_H_
#define _AL_PARAMETER_H_

#include <task.h>

typedef enum eBODY_PARTS {
    OBAKE_BODY_PARTS_NONE = 0x0,
    OBAKE_BODY_PARTS_SPECTOR = 0x1,
    OBAKE_BODY_PARTS_END = 0x2,
} eBODY_PARTS;

typedef enum eAL_JEWEL_NUM {
    AL_JEWEL_NONE = 0x0,
    AL_JEWEL_GOLD = 0x1,
    AL_JEWEL_SILVER = 0x2,
    AL_JEWEL_RUBY = 0x3,
    AL_JEWEL_SAPPHIRE = 0x4,
    AL_JEWEL_EMERALD = 0x5,
    AL_JEWEL_AMETHYST = 0x6,
    AL_JEWEL_AQUAMARINE = 0x7,
    AL_JEWEL_GARNET = 0x8,
    AL_JEWEL_ONYX = 0x9,
    AL_JEWEL_PERIDOT = 0xA,
    AL_JEWEL_TOPAZ = 0xB,
    AL_JEWEL_ENV0 = 0xC,
    AL_JEWEL_ENV1 = 0xD,
    AL_JEWEL_ENV2 = 0xE,
    AL_JEWEL_ENV3 = 0xF,
    AL_JEWEL_ENV4 = 0x10,
    NB_AL_JEWEL = 0x11,
} eAL_JEWEL_NUM;

enum {
    TYPE_NONE = 0x0,
    TYPE_EGG = 0x1,
    TYPE_CHILD = 0x2,
    TYPE_DUMMY1 = 0x3,
    TYPE_DUMMY2 = 0x4,
    TYPE_N_NORMAL = 0x5,
    TYPE_H_NORMAL = 0x6,
    TYPE_D_NORMAL = 0x7,
    TYPE_N_SWIM = 0x8,
    TYPE_H_SWIM = 0x9,
    TYPE_D_SWIM = 0xA,
    TYPE_N_FLY = 0xB,
    TYPE_H_FLY = 0xC,
    TYPE_D_FLY = 0xD,
    TYPE_N_RUN = 0xE,
    TYPE_H_RUN = 0xF,
    TYPE_D_RUN = 0x10,
    TYPE_N_POWER = 0x11,
    TYPE_H_POWER = 0x12,
    TYPE_D_POWER = 0x13,
    TYPE_N_CHAOS = 0x14,
    TYPE_H_CHAOS = 0x15,
    TYPE_D_CHAOS = 0x16,
    TYPE_TAILS = 0x17,
    TYPE_KNUCKLES = 0x18,
    TYPE_AMY = 0x19,
    TYPE_MINIMAL = 0x1A,
    TYPE_OMOCHAO = 0x1B,
    TYPE_EGG_FOOT = 0x1C,
};

typedef enum HDAttr {
    HDAttr_Neut = 0x0,
    HDAttr_Hero = 0x1,
    HDAttr_Dark = 0x2,
} HDAttr;

enum 
{
    SKILL_SWIM,
    SKILL_FLY,
    SKILL_RUN,
    SKILL_POWER,
    SKILL_GUTS,
    SKILL_INTELLECT,
    SKILL_STAMINA,
    NB_SKILL
};

Bool AL_IsDark2(task* pChaoTask);
void AL_ParameterAddUserLike(task* tp, int add, float AddAttr);
void AL_DrawMeter(int x, int y, unsigned int length, unsigned int color, unsigned int color2);
int AL_ParameterGetSkill(task* pChaoTask, Uint16 SkillKind);

#endif