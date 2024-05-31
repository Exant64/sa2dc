#ifndef _AL_STAGE_H_
#define _AL_STAGE_H_

typedef enum eCHAO_STAGE_NUMBER {
    CHAO_STG_NEUT = 1,
    CHAO_STG_HERO = 2,
    CHAO_STG_DARK = 3,
} eCHAO_STAGE_NUMBER;

eCHAO_STAGE_NUMBER AL_GetStageNumber();

#endif