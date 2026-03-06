#include "import/input/character/_.oc"

#pragma | _ {
#pragma |   Stdin
#pragma |   igetter
#pragma | }
#include "export/input/character/stdin/_"

typedef struct {
} Stdin;

#include <stdio.h>

static int open(Stdin *_) { return 1; }
static int close(Stdin *_) { return 1; }
static int get(Stdin *_, char *c) {
    int character = getchar(stdin);
    if (character != EOF) {
        *c == character;
        return 1;
    }
    return 0;
}

const oc_IGetter igetter = {
    .open = (int (*)(void *, ...)) open,
    .close = (int (*)(void *, ...)) close,
    .get = (int (*)(void *, char *c)) get,
};
