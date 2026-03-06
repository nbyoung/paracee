# Initial Analysis of the `metac` Mockup

## 1. Mockup Structure

The mockup comprises six C source files arranged in a directory hierarchy that doubles as a namespace tree:

```
metac/
  demo/helloworld/main.c          # Consumer: uses an interface via import
  lib/
    output/character.c             # Interface definition: IPutter
    output/character/stdout.c      # Implementation: Stdout (writes to stdout)
    output/character/null.c        # Implementation: Null (discards output)
    input/character.c              # Interface definition: IGetter
    input/character/stdin.c        # Implementation: Stdin (reads from stdin)
```

The directory path *is* the namespace. `lib/output/character` defines the namespace `output.character`; `lib/output/character/stdout` extends it with an implementation scope.

## 2. Mechanism Inventory

The mockup overloads three standard C mechanisms to carry namespace and interface semantics:

### 2.1 `#pragma metac` -- Module Declaration

Each file opens with a `#pragma` directive that declares what the module exports. The mockup uses two syntax variants:

**Single-line form** (`output/character.c`, `output/character/stdout.c`, `output/character/null.c`):
```c
#pragma metac _ { IPutter }
#pragma metac _ { Stdout iputter }
```

**Multi-line form** (`input/character/stdin.c`):
```c
#pragma | _ {
#pragma |   Stdin
#pragma |   igetter
#pragma | }
```

The `_` appears to be a self-reference placeholder (the current module). The braces delimit the export list. Uppercase identifiers denote types; lowercase identifiers denote interface instances.

Notably, `input/character.c` uses `#pragma |` rather than `#pragma metac`, suggesting the mockup is still exploring the pragma keyword itself.

### 2.2 `#include` -- Import/Export Binding

Standard `#include` is repurposed for two distinct roles:

- **Import**: `#include "import/output/character/stdout/_.ocs"` -- brings the `output.character.stdout` namespace into scope with the alias `ocs`.
- **Export**: `#include "export/lib/output/character/stdout/_"` -- publishes the current module's declarations into the namespace tree.

The alias appears to derive from the initials of the namespace path segments (`o`utput, `c`haracter, `s`tdout = `ocs`; `o`utput, `c`haracter = `oc`). The file extension on import (`.ocs`) encodes the alias.

### 2.3 `struct` -- Interface and State Separation

**Interface structs** are vtables -- structs of function pointers with a `void *` first parameter standing in for a generic `self`:
```c
typedef struct {
    int (*open)(void *, ...);
    int (*close)(void *, ...);
    int (*put)(void *, char);
} IPutter;
```

**State structs** hold per-instance data (empty in this mockup):
```c
typedef struct {
} Stdout;
```

**Implementation binding** is a `const` instance of the interface struct, with static functions cast to match the `void *` signature:
```c
const oc_IPutter iputter = {
    .open  = (int (*)(void *, ...)) open,
    .close = (int (*)(void *, ...)) close,
    .put   = (int (*)(void *, char c)) put,
};
```

The consumer accesses the interface through the alias pointer:
```c
ocs->iputter.put(&ocs_stdout, c);
```

## 3. Critique of Purpose

### Strengths

- **Real problem**: Identifier collision in large C codebases is a genuine productivity obstacle. The C standard offers only file-level `static` and naming conventions (e.g., `libfoo_` prefixes) as mitigations; both are brittle.
- **Parnas alignment**: Decomposing by information-hiding rather than by processing steps remains the strongest argument for modular software. Encoding this discipline into C conventions with tool support could meaningfully improve C maintainability.
- **Backwards compatibility**: Targeting existing C semantics (preprocessor, structs, function pointers) rather than a language extension means no compiler fork, no new ABI, and no barrier to incremental adoption in legacy codebases.
- **Competing implementations**: The vision of standardised interfaces with swappable implementations is compelling -- it is the mechanism behind POSIX, VFS layers, and driver models, but C has never had a source-level convention to express it uniformly.

### Risks and Tensions

- **Convention vs. enforcement**: Without compiler or tooling support, adherence depends on programmer discipline. The mockup acknowledges this by targeting tool support in the prototype phase, but the conventions must be simple enough that violations are obvious in code review even *before* tools exist.
- **Preprocessor fragility**: `#pragma` is implementation-defined; unknown pragmas are silently ignored by conforming compilers. This is both an advantage (backwards compatibility) and a hazard (silent misconfiguration). A pragma-based system cannot provide compile-time errors when the convention is violated.
- **Adoption threshold**: C programmers who want namespaces and interfaces have typically migrated to C++ or Rust. The audience for this project is programmers who *must stay in C* (embedded, kernel, safety-critical). The conventions need to be lightweight enough that they impose no runtime cost and minimal cognitive overhead for that audience.
- **Competing prior art**: GLib's GObject, Linux kernel's `struct file_operations`, COM/XPCOM, and OSAL patterns all provide object-based C conventions. The project should articulate what it offers beyond these (likely: namespace hierarchy and source-level interface standardisation without a runtime framework).

## 4. Namespace Conventions via C Mechanisms

### 4.1 `#pragma` as Declaration

**What it provides**: A place to declare module exports that is visible to both tools and human readers, ignored by compilers that lack `metac` support.

**Limitations**:
- The syntax is not validated by any existing compiler; typos like `iputtery` (seen in `null.c`) or a stray `2` pass silently.
- Two competing syntaxes (`#pragma metac` vs. `#pragma |`) already appear within the mockup, suggesting the notation is not yet settled.
- Multi-line `#pragma` requires line-continuation backslashes or repeated `#pragma` prefixes, both of which are noisy.

### 4.2 `#include` as Import/Export

**What it provides**: Reuses the most familiar C preprocessor directive to express dependency and publication. The path structure mirrors the namespace, making the mapping visible.

**Limitations**:
- The `import/` and `export/` paths do not correspond to real files on disk (no such files exist in the repository). A preprocessing step must either generate them or intercept them. This means the mockup *as written* cannot compile with a standard toolchain.
- Encoding the alias in the file extension (`.ocs`) is compact but obscure. A reader must reverse-engineer the convention to understand what `ocs` refers to.
- The export include appears *after* the pragma, creating a non-obvious ordering dependency.

### 4.3 `struct` as Interface / State

**What it provides**: A direct mapping to the vtable pattern used throughout systems C (POSIX, Linux kernel, embedded HALs). No new abstraction is introduced.

**Limitations**:
- The `void *` first parameter sacrifices type safety at the interface boundary. Calling `open` through the interface pointer provides no compile-time check that the `void *` actually points to a `Stdout`.
- Variadic `(void *, ...)` signatures (on `open` and `close`) defeat all parameter checking. These appear to accommodate implementation-specific extra arguments, but this undercuts the interface contract.
- The function-pointer casts in the implementation binding are technically undefined behaviour in C (casting between incompatible function pointer types). In practice they work on all mainstream ABIs, but a safety-critical audience may object.

## 5. Proposed Alternatives and Refinements

### 5.1 Namespace Declaration: Replace `#pragma` with a Structured Comment or Dedicated Header

**Option A -- Structured comment**:
```c
/* @metac exports { IPutter } */
```
Advantages: no dependence on `#pragma` semantics; trivially parseable by tools; survives any preprocessor. Disadvantage: invisible to the compiler entirely (but `#pragma` effectively is too, for unknown pragma names).

**Option B -- Dedicated `.metac` sidecar file**:
```
# output/character/stdout/_.metac
exports: Stdout, iputter
implements: output/character/IPutter
```
Advantages: separates the metac metadata from C source entirely, enabling tool processing without parsing C. Disadvantage: another file to maintain; drift risk between `.metac` and `.c`.

**Option C -- Retain `#pragma` but standardise strictly**:
```c
#pragma metac export type Stdout
#pragma metac export impl iputter : IPutter
#pragma metac import output/character as oc
```
Each pragma is one declaration with a fixed keyword grammar. This eliminates the brace/backslash syntax, the ambiguous `_` placeholder, and the overloaded `#include` for import binding. Tools can validate each line independently.

### 5.2 Import Aliases: Make Them Explicit

Rather than encoding the alias in a file extension:
```c
#pragma metac import output/character/stdout as ocs
```
or, if retaining `#include`:
```c
#include "metac/output/character/stdout.h"  /* generates: */
/* #define ocs_Stdout ... */
/* extern const oc_IPutter ocs_iputter; */
```
The alias derivation rule (path initials) should be documented and enforced by tooling, but the alias should also be *readable in the source* without requiring the reader to know the rule.

### 5.3 Type Safety at the Interface Boundary

Replace `void *` with a macro-generated typed wrapper or an opaque handle:

```c
/* In the interface definition */
typedef struct metac_handle { void *impl; } metac_handle;

/* In the consumer */
ocs_handle h;
ocs.iputter.open(&h);
ocs.iputter.put(&h, 'A');
```

Or, use `_Generic` (C11) to provide compile-time dispatch to the correct concrete type without casts. This aligns with the goal of zero runtime cost while recovering some type checking.

### 5.4 Eliminate Variadic Interface Signatures

The `(void *, ...)` signatures on `open` and `close` should be replaced with either:
- A fixed signature with an explicit options struct: `int (*open)(void *self, const Options *opts);`
- Separate `open` and `open_with` slots if extensibility is needed.

Variadic function pointers are hostile to both type safety and static analysis.

### 5.5 Implementation Binding Without Undefined-Behaviour Casts

Use a macro to generate a type-safe trampoline:

```c
#define METAC_BIND(iface_fn, impl_fn, StateType) \
    static int iface_fn##_trampoline(void *self) { \
        return impl_fn((StateType *)self); \
    }
```

Or accept the cast but document it as a project convention with a known-ABI constraint, and provide a CI check that flags new casts for review.

## 6. Incidental Bugs in the Mockup

These appear to be artefacts of the mockup's draft status rather than design issues:

| File | Line | Issue |
|------|------|-------|
| `demo/helloworld/main.c` | 8 | `char *c;` is uninitialised; should be `char *c = hello;` |
| `lib/input/character.c` | 1 | `#pragma \|` instead of `#pragma metac` (syntax variant or typo) |
| `lib/input/character.c` | 6 | `IGetter` contains `put` instead of `get` |
| `lib/input/character/stdin.c` | 17 | `getchar(stdin)` -- `getchar` takes no arguments; should be `getc(stdin)` or `fgetc(stdin)` |
| `lib/input/character/stdin.c` | 19 | `*c == character` -- comparison instead of assignment (`=`) |
| `lib/output/character/null.c` | 5 | `iputtery` -- appears to be a typo for `iputter` |
| `lib/output/character/null.c` | 6 | Unexplained `2` in the pragma export list |
