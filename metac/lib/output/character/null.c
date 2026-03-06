#include "import/output/character/_.oc"

#pragma metac _ { \
    Null        \
    iputtery     \
    2 \
}
#include "export/lib/output/character/null/_"


typedef struct {
} Null;

static int open(Null *null) { return 1; }
static int close(Null *null) { return 1; }
static int put(Null *null, char c) { return 1; }

const oc_IPutter iputter = {
    .open = (int (*)(void *, ...)) open,
    .close = (int (*)(void *, ...)) close,
    .put = (int (*)(void *, char c)) put,
};
