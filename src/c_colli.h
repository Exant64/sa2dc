#ifndef _C_COLLI_H_
#define _C_COLLI_H_

#include <task.h>

enum CI_FORM {
    CI_FORM_SPHERE = 0x0,
    CI_FORM_CYLINDER = 0x1,
    CI_FORM_CYLINDER2 = 0x2,
    CI_FORM_RECTANGLE = 0x3,
    CI_FORM_RECTANGLE2 = 0x4,
    CI_FORM_RECTANGLE3 = 0x5,
    CI_FORM_CAPSULE = 0x6,
    CI_FORM_PERSON = 0x7,
    CI_FORM_LINE = 0x8,
    CI_FORM_WALL_PLANE = 0x9,
    CI_FORM_WALL_CIRCLE = 0xA,
};

typedef struct CCL_INFO {
    Uint8 kind;
    Uint8 form;
    Uint8 push;
    Uint8 damage;
    int attr;
    NJS_POINT3 center;
    float a;
    float b;
    float c;
    float d;
    int angx;
    int angy;
    int angz;
} CCL_INFO;

#define CCL_SPHERE(kind, push, damage, attr, cx, cy, cz, a, b, c) \
    { kind, CI_FORM_SPHERE, push, damage, attr, { cx, cy, cz }, a, b, c, 0, 0, 0, 0 }
#define CCL_CYLINDER(kind, push, damage, attr, cx, cy, cz, a, b, c) \
    { kind, CI_FORM_CYLINDER, push, damage, attr, { cx, cy, cz }, a, b, c, 0, 0, 0, 0 }

// thanks kell
#define CI_DMG_SET(att, def) (att | (def << 2)) /* Set the attack and defense priorities (0-3) */
#define CI_DMG_HIT 0x10                         /* Collision has been hurt */
#define CI_DMG_PLAYER 0x20                      /* Can hurt collisions in PLAYER and CHAO list */
#define CI_DMG_ENEMY 0x40                       /* Can hurt collisions in ENEMY and ENEMY2 lists */
#define CI_DMG_OBJECT 0x80                      /* Can hurt collisions in OBJECT and OBJECT2 lists */
#define CI_DMG_ALL                                                                                    \
    (CI_DMG_PLAYER | CI_DMG_ENEMY | CI_DMG_OBJECT) /* Can hurt all collisions except BULLET/ITEM ones \
                                                    */
#define CI_ATTR_NO_SEND 0x4                        /* Do not send information to other collisions */
#define CI_ATTR_NO_RECEIVE 0x8                     /* Do not receive information from other collisions */
#define CI_ATTR_IGNORE 0x10                        /* Do not run physical collision check */
#define CI_ATTR_POS_ABSOLUTE 0x20                  /* The position is no longer relative to the entity */
#define CI_ATTR_ANG_YZX 0x200                      /* Use YZX rotation order instead of ZXY */
#define CI_ATTR_DAMAGE 0x400                       /* Allow damage */
#define CI_ATTR_2000 0x2000                        /* ? */
#define CI_ATTR_NO_PUSH 0x4000                     /* Do not get pushed by other collisions */
#define CI_ATTR_ANG_ABSOLUTE 0x8000                /* The angle is no longer relative to the entity */
#define CI_ATTR_80000 0x80000                      /* ? */
#define CI_ATTR_100000 0x100000                    /* ? */
#define CI_ATTR_200000 0x200000                    /* ? */
#define CI_ATTR_400000 0x400000                    /* ? */
#define CI_ATTR_800000 0x800000                    /* ? */

#define CI_PUSH_PO_PLAYER 0x1 /* Collision can push a PLAYER or CHAO collision */
#define CI_PUSH_PO_ENEMY 0x2  /* Collision can push a ENEMY or ENEMY2 collision */
#define CI_PUSH_PO_OBJECT 0x4 /* Collision can push a OBJECT or OBJECT2 collision */
#define CI_PUSH_PO_ITEM 0x8   /* Collision can push a ITEM or ITEM2 collision */
#define CI_PUSH_PO_CMN                      \
    (CI_PUSH_PO_PLAYER | CI_PUSH_PO_ENEMY | \
     CI_PUSH_PO_OBJECT) /* Collision can push all collisions except ITEM, ITEM2 and BULLET */
#define CI_PUSH_PO_ALL (CI_PUSH_PO_CMN | CI_PUSH_PO_ITEM) /* Collision can push all collisions except BULLET */

#define CI_PUSH_TH_PLAYER 0x10 /* Collision can't be pushed by a CID_PLAYER or CID_CHAO collision */
#define CI_PUSH_TH_ENEMY 0x20  /* Collision can't be pushed by a CID_ENEMY or CID_ENEMY2 collision */
#define CI_PUSH_TH_OBJECT 0x40 /* Collision can't be pushed by a CID_OBJECT or CID_OBJECT2 collision */
#define CI_PUSH_TH_ITEM 0x80   /* Collision can't be pushed by a CID_ITEM or CID_ITEM2 collision */
#define CI_PUSH_TH_CMN                      \
    (CI_PUSH_TH_PLAYER | CI_PUSH_TH_ENEMY | \
     CI_PUSH_TH_OBJECT) /* Collision can't be pushed by all collisions except ITEM, ITEM2 and BULLET */
#define CI_PUSH_TH_ALL \
    (CI_PUSH_TH_CMN | CI_PUSH_TH_ITEM) /* Collision can't pushed by all collisions except ITEM, ITEM2 and BULLET */

typedef struct c_colli_hit_info {
    char my_num;
    char hit_num;
    Uint16 flag;
    taskwk* hit_twp;
} c_colli_hit_info;

task* CCL_IsHitKind2(task* tp, int kind);

task* CCL_IsHitPlayerWithNum(task* tp, int info_num);
c_colli_hit_info* CCL_IsHitPlayerWithNum2(taskwk* twp, int info_num);
CCL_INFO* CCL_GetInfo(task* tp, int info_num);

void CCL_Init(task* tp, CCL_INFO* info, int nbInfo, char id);

#endif