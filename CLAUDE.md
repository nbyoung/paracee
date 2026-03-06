# Project Premise

The present `parcee` project aims to develop a critical technical component within a larger strategy to improve programmer productivity in the C programming language. It applies at least two concepts that have proven valuable in more modern languages. 
The strategy aims to realise the principles of Parnas' 1972 ACM-published paper, "On the criteria to be used in decomposing systems into modules" while retaining backwards compatibility with the massive existing corpus of C program implementations.

The first concept, _user-_ or _programmer-defined namespaces_, mitigates the problem of _identifier conflict_ in large C programs by allowing the programmer to define an hierarchical namespace representing the taxonomy of concerns in which the program realizes its architectural design. The _programmer-defined namespace_ concept includes an _alias_ mechanism for mapping arbitrarily-deep points in the hierarchy to a short, local identifier, analagous to the role of the `as` keyword for both the `import` statement in Python and the `use` statement in Rust.

The second concept, _interface definitions_, applies the programmer-defined namespaces described above to realize Parnas' principle of the _separation of concerns_ through _encapsulation_. Through the disciplined application of an idiomatic convention that explicitly declares encapsulated _state_ separately from the _state transformation functions_, programmers can achieve _object-based_ programming in native C.

Consistent adherence to the object-based convention introduces the possibility of component-based source code libraries. Published, well-defined library _interfaces_ would allow separate teams to independently develop distinct library _implementations_ with differentiated space-time complexity tradeoffs and underlying hardware support. Adoption of the object-based conventions could allow emergence of industry-level standard interfaces with competing and complementary library implementations, thereby achieving the benefits of widespread and interchangeable modular reuse, including among existing, well-proven historical C libraries.

# Development Strategy

## Mock-up Refinement

The `metac` folder provides a simple _Hello, world!_ mockup of both programmer-defined namespaces and interface definitions with state encapsulation. The present development stategy applies the `metac` mockup as a starting point in representing the essential concepts with one plausible realization. The first, _mock-up_ phase of the strategy aims to refine the realization to maximise expressiveness and human comprehension while accomodating the constraints of C language compatibility and tool adaptation.

## Prototype Development

The `paracee` project utility arises directly through its adoption. The _prototype_ phase aims to assess obstacles to adoption by illustrating the concepts through accessible but realistic example C programs. This phase develops the minimum tool support to show experienced programmers the potential benefits of modular, object-based C source libraries with examples that are large enough to reveal pragmatic requirements such as interface-definition version control, library implementation packaging and distribution, and IDE and debugger integration.