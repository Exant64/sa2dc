#ifndef _SUBPROGRAM_H_
#define _SUBPROGRAM_H_
#include <task.h>

typedef struct SUBPRG_HEADER {
    char Name[12];
    void (*Init)(void);
    void (*Free)(void);
    void (*subprgmanager)(task*);
} SUBPRG_HEADER;
#endif