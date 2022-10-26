# Thrice Language Specification

_Thrice_ is a programing language that is at least equivalent to C language,
meaning anything that can be written in C can be written in Thrice with
equivalent semantics.

Thrice tries to add the fallowing features to C language:

- compile-time calculations,
- generics,
- modules,
- packages,
- closures,
- tagged unions,
- tuples,
- resource management with RAII,
- immutability and invisibility by default.

And it removes the fallowing features to have a cleaner language:

- macros,
- implicit narrowing conversions,
- implicit array decay,
- empty parameter list meaning any,
- untyped variadic function arguments.

## Symbol

A _symbol_ is a semantic object.

## Source

A _source_ is a file that has UTF-8 encoded text in it, which is a list of
symbol definitions.

File extension of a source is `.tr`. Name of a source can only contain the
lowercase English letters and decimal digits. It cannot start with a decimal
digit.

## Module

A _module_ is a directory, which contains at least one source or module.

Name of a module has to obey the same rules as the source names.

## Package

A _package_ is a Thrice library. It is also an executable if it has an entry
point.

Packages can be structured as a single source or module. In both of these cases,
the name of the source or the module becomes the name of the package.

If the package is formed out of a single source, that source can have an entry
point in it. Otherwise, the package can have a single source named `main` that
can have an entry point, which is called a _main file_. Main file has to be
directly under the module directory that forms the package.

## Visibility

_Visibility_ is whether a symbol can be accessed in the definition of another
symbol.

A symbol is always visible in the source it is defined in. Its visibility can be
increased using a _visibility modifier_. There are 3 visibility modifiers in
Thrice:

- `module` makes a symbol visible in the module it is defined in,
- `intern` makes a symbol visible in the package it is defined in,
- `extern` makes a symbol visible everywhere.

The visibility modifier must be the first thing in the definition of a symbol.

The main file is considered as a file outside of the package. Thus, it can only
access extern symbols of the executable package it is in. On the other hand, the
entry point in a package that is formed from a single source can access all the
symbols defined in that source as normal.
