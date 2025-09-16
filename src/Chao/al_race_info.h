#ifndef _AL_RACE_INFO_H_
#define _AL_RACE_INFO_H_

typedef enum eRACE_ACTIVE_FLAG
{
  RACE_ACTIVE_FLAG_NONE = 0x0,
  RACE_ACTIVE_FLAG_OPEN = 0x1,
  RACE_ACTIVE_FLAG_NEW = 0x2,
  RACE_ACTIVE_FLAG_PLAY = 0x3,
} eRACE_ACTIVE_FLAG;

typedef struct AL_RECORD
{
  char RecordFlag;
  AL_TIME time;
  AL_SHAPE_ELEMENT ShapeElement;
} AL_RECORD;

typedef struct race_save_info {
    char RaceActiveFlag[6];
    char CourseChallengedLevel[13];
    char CourseClearedLevel[13];
    AL_RECORD CourseRecord[10];
} race_save_info;
race_save_info* AL_GetRaceInfo();

#endif