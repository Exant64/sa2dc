#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <task.h>

// thanks to shad's SAMT for the first 8 symbols(ish?) members
// eventually we should port over the rest to
typedef struct playerwk
{
  Sint8 pl_num;          // Player Number
  Sint8 ch_num;          // Base Character Number
  Sint8 costume;         // Costume Number
  Sint8 ch_num_multi;    // Characer Number + 2P Alts

  Sint8 action_list[8];
  Sint8 action_num;
  Sint8 action_sel;      // Currently selected action index (with Y)
  Sint8 action_last;     // ? Set on pressing action
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
} playerwk;

extern taskwk* playertwp[];
extern playerwk* playerpwp[];

void HoldTaskP(int, task*);

#endif