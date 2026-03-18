# Paracee

The  _C Encapsulation Extension_ (CEE) uses C preprocessor directives to express modular encapsulation in ordinary C programs. A special `#pragma` directive declares a module and its interface. A special `#include` directive exports the module's definition into a programmer-defined module namespace. A different `#include` directive in a separate file imports the module from the namespace and maps its interface identifiers into local typedefs, variables and functions within the local scope.

CEE enables modular encapsulation in native C at the source level.  Combining CEE modular encapsulation with object-based programming (see _Orthocee_, below) introduces reusable C software components with zero run-time cost. Libraries of reusable Orthocee-based components achieve transparent portability across arbitrary CPU architectures and arbitrary peripheral devices. Meanwhile, Orthocee-based wrappers enable direct re-use of existing C libraries with complete backwards compatibility.

The _Paracee_ project develops tool support for CEE. The project's central tool, the _paracee_ translator, operates in two steps. First, it analyses C source files for CEE module definitions. Second, it generates the required _export_ and _import_ `#include` files to implement the modular encapsulation. Since the _paracee_ translator operates as a _pre-_ preprocessor, that is, _without_ altering the program source files, it allows for full source-level re-use of both object-based and legacy C libraries, with _zero_ changes to existing compiler toolchains.

## C Encapsulation Extension (CEE)

| Module | CEE statement |
|--|--|
| Declaration | `#pragma cee <name> { [<identifier>] ... }` |
| Export | `#include "export/<path>/<name>"` |
| Import | `#include "import/<path>/<name>.<prefix>"` |

| _where_ | | |
|--|--|--|
| `<name>` | Module name | Any C identifier, including `_` (underscore) |
| `[<identifier>] ...` | Module interface | Typedef, variable and function identifiers |
| `<path>` | Modular namespace path | `<segment1>/<segment2>/...` |
| `<prefix>` | Local identifier prefix | Any (short) C identifier |

### Module definition

The CEE language extension declares a module and exports its definition through the combination of two statements within the same _translation unit_, i.e., source file. A module interface _declaration_ comprises any `#pragma cee <name> { <identifier> ... }` statement. The declaration names the module and lists the typedef, variable and function identifiers from the originating source file that comprise the module interface. A module _definition_ comprises all of the typedef, variable and function definitions named by the identifiers listed in its interface declaration. A module name comprises any valid C identifier, including a single underscore (`_`). The underscore name conventionally names the originating source file's _eponymous_ module.

### Module export

A module _export_ statement comprises any `#include "export/<path>/<name>"` statement where _name_ names a declared module within the same source file.

## Module namespace

The `<path>` comprises zero or more path segments. The sequence of path segments identifies the module's location within the programmer-defined hierarchical modular _namespace_. By convention, a module's export path typically reflects its defining source file's path within the enclosing filesystem.

### Module import

The CEE language imports a module through a single `#include "import/<path>/<name>.<prefix>"` statement. The `<path>` and `<name>` together identify a module exported with the same name but from a different source file within the same program. The `<path>` typically reflects at least of an _export_ path but not necessarily. In particular, the CEE language extension does not specify the relationship between _export_ and _import_ paths, and leaves the details of any such relationship to the implementation of the CEE translator. The `<prefix>` comprises a C identifier string that prefixes the names of typedefs, variables and functions from the module interface but mapped to corresponding typedefs, variables and functions within the importing file scope.

## Orthocee

The _Orthocee_ project applies the _orthocee_ programming convention to combine the modular encapsulation provided by the CEE language extension with _object-based_ programming in C.

### Object-based programming

Object-based programming defines an object's _state_ separately from an object's _interface_ which defines the set of _functions_ or _methods_ that transform its state. Furthermore, object-based _encapsulation_ separates the function _implementations_ from the object interface. Modular programming through encapsulation allows programmers to select among various available object state and function implementations without changing the program's functional composition represented in its objects' interfaces. Object-based programming with modular encapsulation thereby allows flexible and space-time performance trade-offs and peripheral hardware selection among independent module implementations.

### Legacy C library API wrappers

 By applying the _prefix_ notation defined through the CEE module interface import statements, programmers can realize object-based _Application Programming Interface_ (API) source library wrappers for non-object-based legacy C programs.

