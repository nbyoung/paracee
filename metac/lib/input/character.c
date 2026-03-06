#pragma | _ { IGetter }
#include "export/input/character/_"

typedef struct {
    int (*open)(void *, ...);
    int (*close)(void *, ...);
    int (*put)(void *, char);
} IGetter;