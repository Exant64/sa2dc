#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <task.h>

typedef struct CharObj2Base
{
  char PlayerNum;
  char CharID;
  char Costume;
  char CharID2;
  char ActionWindowItems[8];
  char ActionWindowItemCount;
  char field_D[3];
  Sint16 Powerups;
  Sint16 field_12;
  Sint16 DisableControlTimer;
  Sint16 UnderwaterTime;
  Sint16 IdleTime;
  Sint16 ConfuseTime;
  char gap1C[8];
  int Upgrades;
  float field_28;
  int field_2C;
  float PathDist;
  float Up;
  char field_38[8];
  float SomeMultiplier;
  int field_44;
  float MechHP;
  NJS_POINT3 Eff;
  NJS_POINT3 Acceleration;
  NJS_VECTOR Speed;
  NJS_POINT3 WallNormal;
  NJS_POINT3 FloorNormal;
  int CurrentSurfaceFlags;
  int PreviousSurfaceFlags;
  void *DynColInfo;
  task *HeldObject;
  char gap98[4];
  task *HoldTarget;
} CharObj2Base;

extern taskwk* playertwp[];
extern CharObj2Base* playerpwp[];

void HoldTaskP(int, task*);
int GetAngToPlayer(task*, int index);

#endif