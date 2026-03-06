#include "import/output/character/stdout/_.ocs"

void main(void) {
    char *hello = "Hello, world!\n";
    ocs_Stdout ocs_stdout;

    ocs->iputter.open(&ocs_stdout);
    for (char *c; *c; c++) {
        ocs->iputter.put(&ocs_stdout, c);
    }
    ocs->iputter.close(&ocs_stdout);
}