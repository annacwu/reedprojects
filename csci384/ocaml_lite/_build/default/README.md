# OCaml-Lite Starter Code

This is the starter code for the main programming project of the semester,
OCaml-lite. OCaml-lite is a stripped-down version of OCaml that we will be
implementing over the next few months.

## Overview

This folder contains a number of files. I've given a brief overview here and
I'll describe each piece in more detail below.

- `dune-project` is for controlling the build system, as are the files called
  `dune` which you'll find in (almost) every subdirectory.
- `lib/` holds most of the implementation code for our language.
- `test/` holds code for testing our implementation.
- `bin/` holds the code that actually runs our parser and interpreter (or
  compiler).
- `example_code.ol` is an OCaml-lite file you can use to see if your
  implementation can run successfully.

## Dune

This project uses a tool called Dune to automate a number of tasks including
compiling, running, and testing our code. You can think of Dune as a much more
powerful alternative to `make`, which you used in 221. Most modern programming
languages include some kind of system to help manage compilation and testing in
large software projects, and Dune is the one that works with OCaml.

I've set up all of the files to control Dune for this project, so you won't need
to modify anything. You'll just need to know a few important commands. Note that
all of these need to be run from the root directory of the project (i.e., the
directory containing the `dune-project` file).

- `dune build` will compile all of the OCaml files you've written.
- `dune test` will compile everything and then run the tests from the `test/`
  directory. You can also give an argument to `dune test` to control which tests
  are run. For example, `dune test test/expr/` will only run the tests in the
  `expr/` subdirectory.
- `dune exec ocaml_lite -- <args>` will run the main OCaml-lite program (defined
  in `bin/main.ml`) with the given arguments. On Windows you may need to run
  `dune exec ocaml_lite.exe -- <args>` instead.

## Implementation

Most of the code you'll need to write for the OCaml-lite implementation will be
in the `lib/` directory. The files are:

- `ast.ml` holds type definitions for the abstract syntax tree of OCaml-lite
  and code for interacting with those types.
- `lexer.mll` defines a lexer for transforming a stream of characters into a
  stream of tokens. You won't need to edit this file, although you will need to
  look at it to see what tokens can appear in the input.
- `parser.ml` is responsible for converting a token stream to an abstract
  syntax tree.
- `typecheck.ml` takes an abstract syntax tree and ensures that all of the types
  of various bindings and function calls match.
- `interpret.ml` contains the code needed to take an abstract syntax tree and
  evaluate the program defined by that tree.
- `compile.ml` takes an abstract syntax tree and outputs a bytecode file for the
  OCaml-lite virtual machine (see below).

The entry point to the program is defined in `bin/main.ml`. This file is nearly
complete, you will only have to make a minor edit to add a typechecking pass
once we have written a typechecker.

## The OCaml-lite Virtual Machine

We won't be focusing too much on compilation in this course since we have so
much ground to cover, but I did want to give you an opportunity to see some of
the techniques and issues that come up in compiler development. To make that
feasible, I have written a _virtual machine_ which is considerably simpler to
compile for than a real computer. This virtual machine is defined in `vm/`,
which has it's own README file to introduce the VM and its code.

## Tests

The `test/` directory contains tests for various pieces of the OCaml-lite
implementation. The tests are organized into subdirectories corresponding to the
different parts of the project implementation:

- `expr` holds tests for part 1.
- `conditionals` has tests for part 2.
- `bindings` has tests for part 3.
- `functions` has tests for parts 4 and 5.
- `datatypes` has tests for part 6.

There is one additional file in the test directory, `ocaml_lite_tests.ml`. This
has tests for the lexer, which is already complete. There are also two
additional directories. The first, `bytecode.t`, holds tests for the compiler,
organized into the same subdirectories as the other tests. The second, `utils`
has code for a number of utility functions that are shared by the tests for
different parts of the project. These are sufficiently filled in for the part 1
of the project but will need to be extended to handle new cases as your code
grows.

In general, I've provided fairly extensive test suites for the early parts of
the project and then fewer and fewer tests as we move through the course. For
example, the tests in the `expr` directory cover almost all cases already, so
if you pass the tests you can be reasonably confident that your code works. On
the other hand, the `datatypes` directory contains only the setup code needed
to define tests, but not any actual tests. As you go through the course, you'll
need to write more and more of the tests yourself.
