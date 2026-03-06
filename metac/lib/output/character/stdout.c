#include "import/output/character/_.oc"

#pragma metac _ { \
    Stdout      \
    iputter     \
}
#include "export/lib/output/character/stdout/_"

typedef struct {
} Stdout;

#include <stdio.h>

static int open(Stdout *_) { return 1; }
static int close(Stdout *_) {
    return fflush(stdout) != EOF;
}
static int put(Stdout *_, char c) {
    return putchar(c) != EOF;
}

const oc_IPutter iputter = {
    .open = (int (*)(void *, ...)) open,
    .close = (int (*)(void *, ...)) close,
    .put = (int (*)(void *, char c)) put,
};
