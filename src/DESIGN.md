# GPLGEN+ C Style Guide

This document is a developer tool used to write code for this program. Even though, at various points, the code within this document
may not follow this document to a tee, it *will* at some future point respect the rules layed bare.

***

## Creating Functions

The name of a function depends on where within the program it is declared. Each and every function which is not defined within a C file
must be prefixed with `gplp_` this ensures that there is no overlap with the user's included libraries. A static function however, shall
be prefixed with `gplp_NAME_` where `NAME` is the name (or psudonym) of the current file and suffixed with `_static`. This allows us to
immediately know whether or not this file is defined within the current file. Non-static functions which are defined shall be prefixed
with `gplp_NAME_` where, again, `NAME` is the name of the header. A functions name should also be of the form `<prefix><noun><verb><suffix>`,
so that a static function which reads from a file and is in gpl.c would be named: `gplp_gpl_file_read_static`. A function within `main.c`
may be named without any prefixes (as all functions must be static, except for main).

A functions arguments are here defined in the following order:

1. Input Data (pointers should be declared const)
2. IO Data    (should only be non-const pointers)
3. Output     (should only be non-const pointers)

A function, using this model, should either return `void` or `uint32_t` where its return value corresponds to some series of documented
error codes, this error code should be zero on success and nonzero on error (which number specifically encodes the kind of error). An
output pointer should be declared as `r<name>` or `ret` (the pointer declared `ret` shall be considered the main return of the function).

A non-static function should not return an allocated pointer, whenever possible, there should be a function which returns the size of a potential
allocation, which then may be allocated by the callee, and passed into a function which allocates its members. In cases where this is
impractacle the suffix `_allocates` must be used in the function name.

## Declaring variables

A variable must only be declared at the beginning of a scope. This allows us to instantly know what variables exist for any given block
of code, they may be defined as usual. Variables which are related must be defined within a struct which is local to the function. An
example which reads a file and returns the string below:

```C
/* 1 encodes invalid path error */
uint32_t gplp_path_read(const char *path, uint32_t max_length, char *ret) {
    struct {
        FILE *pointer;
        uint32_t length;
    } file;
    file.pointer = fopen(path, "r");
    if(!file.pointer) return 1;
    fseek(file.pointer, 0L, SEEK_END);
    file.length = ftell(file.pointer);
    fseek(file.pointer, 0L, SEEK_SET);
    if(file.length > file.max_length) file.length = file.max_length;
    fread(ret, 1, file.length, file.pointer);
    return 0;
}
```

## Structs, Enums, and Unions

An enum is the easiest here to define, they must be typedeffed with the following name form: `GPLP_<FILE>_<NAME>_ENUM` such that all elements
are uppercase, an element of the enum must be of the following form: `GPLP_<FILE>_<NAME>_<ELEMENT NAME> = <value>`, where value must be
hexadecimal and *must* be manually defined. The single exception to this manual definition rule is the element named `GPLP_<FILE>_<NAME>__MAX`
which must be the final element in the enum.

Structs are similar in the sense that (outside of local struct definitions within functions) a struct must be typedeffed, and of the following
form: `GPLP_<FILE>_<NAME>_DATA`, the elements may have regular variable names.

A union must *not* be typedeffed and may have a regular type name, prefixed with `gplp_<file>_`.

## Static Variables in Header Files

A static variable which is placed within a header file *must* be constant, and only used in place of "magic values". This forms the same purpose
as enums, but for more complicated data types (i.e strings). The name of any one of these variables must have the name plan of:
`GPLP_<FILE>_<NAME>_VAR`.

## Primitive Typedefs

A "primitive" type, is simply a type which aliases directly to another type, when defined in the "defs" header file they may simply be of the
form `GPLP_<NAME>_T` and then used as a regular type. Otherwise they must be defined as: `GPLP_<FILE>_<NAME>_TYPE`. A primitive typedef may have
values associated with it, these may be of the form `#define GPLP_<FILE>_<NAME>_<VALUE NAME>_VALUE <value>` when not in "defs". Otherwise they
may be of form `GPLP_<NAME>`.

## Header Guards

Header Guards are of the following form:

```C
#ifndef X__GPLP_<FILE>_H__X
#define X__GPLP_<FILE>_H__X

<code>

#endif
```


