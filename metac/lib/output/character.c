#pragma metac _ { IPutter }
#include "export/lib/output/character/_"

typedef struct {
    int (*open)(void *, ...);
    int (*close)(void *, ...);
    int (*put)(void *, char);
} IPutter;