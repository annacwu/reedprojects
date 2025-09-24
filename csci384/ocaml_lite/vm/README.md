# The OCaml-lite Virtual Machine

The OCaml-lite Virtual Machine (OVM) is designed to serve as a compilation
target for the language we'll be developing in this class, OCaml-lite. In this
context, a _virtual machine_ is a simulated computer where the "hardware" might
be quite different than the hardware of the real, physical computer it is
running on. The VM has it's own instruction language, typically called
_bytecode_. It is designed as a bridge between our high level programming
language and our actual computer--it is easier to compile code to the VM than it
is to compile to the hardware. At the same time, the VM will execute compiled
code much faster than a naive interpreter. Bytecode is also portable; while
native code needs to be re-compiled for each new operating system/architecture,
bytecode can be shared between computers as long as each one has the virtual
machine. Virtual machines are used by many programming languages including (the
reference implementation of) Python, Java, Erlang, and even (optionally) OCaml
itself.

The OVM is largely based on the OCaml VM, though it is modified to be simpler
for educational purposes.

## Using the VM

This directory includes a Makefile which you can use to compile the VM, just
like you've seen in 221. You should be able to just run `make` which will put
the executable in `release/ovm`. You can then run the VM by calling this
executable with the name of a bytecode file on the command line, like

```
./release/ovm <input>.bc
```

## Programs and Architecture

Much like programs for real hardware, OVM programs are sequences of
instructions. The VM runs a program by executing each instruction in sequence
one at a time while maintaining an internal state of the machine. In contrast to
most physical computers, the internal state of the OVM consists (mostly) of a
single stack, which values can be pushed and popped from. Most instructions pop
values from the top of the stack and push their result back onto the stack. Some
instructions also have arguments that are embedded directly into the code
instead of being read from the stack. This is generally the preferred strategy
for arguments whose values are always known at compile time.

As an example, the program below adds the numbers 2 and 3:

```
CONST 2
CONST 3
ADD
```

It consists of three OVM instructions. The first two are `CONST` instructions
which are followed in the code by a constant (in this case 2 for the first
instruction and 3 for the second one) and push that value onto the stack. The
`ADD` instruction pops two values from the stack, adds them, then pushes the sum
onto the stack. After executing these three instructions, the stack will
consist of a single value, 5.

Values on the OVM stack can be integers, strings, closures, or data objects.
Integers and strings are straightforward, closures and data objects are
discussed below. There is one strange twist to integers because OVM integers are
_63_ bits rather than 64. This is to allow for automatic memory management (see
`alloc.c`).

In addition to the stack, the OVM maintains a _program counter_ which tracks
the location in the code of the instruction that is currently being executed, a
_stack pointer_ which points to the bottom of the current stack frame, and an
_environment_ which stores additional values to be accessed by the ENV
instruction. These are all useful when compiling function calls, as explained
below.

### Closures and Function Calls

Since OCaml-lite is a functional programming language, the OVM is designed with
a few facilities to make it easier to handle functions as data. A _closure_ is a
data object that represents a function together with the environment in which
the function will be executed. In the OVM, closures have two components: an
instruction sequence that will be executed when the function is called and an
_environment array_ which holds the values of all captured variables and
arguments. In recursive functions, the environment array also holds a reference
to the closure itself for recursive calls. Consider the following code:

```
let f = let x = 2 in fun y => x + y ;;
let z = f 5 ;;
```

The function `f` will be represented by a closure object. The environment array
will initially be `[2]` because the definition of `f` _captures_ the value of
`x` when it is defined, so `x` will be added to the environment array. In the
second line, when `f` is called, the argument will be added to the array so the
new environment is `[2, 5]`. At that point, `f` is not expecting any more
arguments, so its code will be invoked. The code will get the captured variable
and argument from the environment (see the `ENV` instruction), then add them and
leave their sum on the stack.

Function code in the OVM is not stored directly on the stack. Instead, the OVM
bytecode file contains a _function table_ indicating the index (in the bytecode
file) of the first instruction of each function. The closure holds an integer
which is used as an index into this table. When the function is called, the
function table is consulted to find the code to execute.

### Objects

OVM can represent more general data objects by bundling an integer _tag_ and a
number of _fields_. These can be used to represent algebraic data types by
assigning a tag to each constructor and combining an appropriate number of
values on the stack. For example, suppose we have an OCaml-lite list type

```
type List = | Nil | Cons of int * List ;;
```

and we assign tag `0` to `Nil` and `1` to `Cons`. Then a list value in the OVM
could either be an object with tag `0` and zero fields, or an object with tag
`1` and two fields. Examining the tag of the object allows the program to figure
out which constructor was used to build that object.

## Program File Format

An OVM bytecode file has several pieces:

- The first 8 bytes of the file are a magic number: "ReedPLDI" in ASCII,
  `0x 52 65 65 64 50 4C 44 49`.
- The next 8 bytes are the number of functions (including `main`).
- Following the number of functions is a number of pointers. Each pointer points
  to one function by indicating the byte-offset of the beginning of that
  function's code in the file. Each pointer is 8 bytes. The first pointer in
  this section _must_ point to the code for `main`.
- Following this is the code for all of the functions. The `main` function
  _must_ be the last function in the file. Other functions may be inserted into
  the program file in any order as long as the CALL instructions (see below) use
  the correct index into the function pointer array.

To start out, you won't need to compile any functions (besides main), so your
OVM code files will always look like this:

```
52 65 65 64 50 4C 44 49   # Magic number: "ReedPLDI"
00 00 00 00 00 00 00 01   # Number of functions: Just one for `main`
00 00 00 00 00 00 00 18   # Pointer to "main". There are 24 bytes of header so
                          # this is 24 in hex.
<instructions for your program...>
```

(Note that the comments above are for demonstration. An actual OVM code file is
just a sequence of bytes, so you cannot have comments and the data would not be
human readable.)

As an example, suppose we want to compile the OCaml-lite program

```
let _ = print_string (string_of_int (1 + 2)) ;;
```

The output would be (again, represented in hexadecimal):

```
# Header as before
52 65 65 64 50 4C 44 49 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 18
# The main code
07 00 00 00 00 00 00 00 01    # CONST 1
07 00 00 00 00 00 00 00 02    # CONST 2
20                            # ADD
F2                            # ITOS (integer to string)
F0                            # PRINT
```

## Opcodes

OVM instructions are encoded in a single byte. The instructions available for
OVM are as follows. A more compact reference without explanations is given in
[`Instructions.md`](Instructions.md). Note that all codes given in the
following tables are represented in hexadecimal.

### Stack Manipulation

The first set of instructions is for explicit stack manipulation.

| Code  | Mnemonic | Description                                |
|-------|----------|--------------------------------------------|
| 07    | CONST n  | Push a constant onto the stack.            |
| 0F    | SCONST s | Push a string constant onto the stack.     |
| 50    | OVER n   | Copy a value from the current stack frame. |
| 51    | POP1     | Delete the second-from-top element.        |

The `CONST` instruction pushes an integer constant onto the stack. The value of
the constant is contained in the next 8 bytes of the code file, so the
instruction `CONST 10` is represented in the code as

```
07 00 00 00 00 00 00 00 0A
```

Note that 8 bytes are used for the value of the constant even though OVM
integers are 63 bits. The most significant bit of the constant is ignored.

The `SCONST` instruction pushes a string constant onto the stack. The value of
this string is embedded into the instruction stream as a sequence of bytes
encoded in ASCII followed by a zero byte. For example, `SCONST "Hello"` is

```
0F 48 65 6C 6C 6F 00
```

The `OVER` instruction copies values from low on the stack to the top. It reads
the next byte of the instruction stream and interprets it as a number `n`. Then
it copies the `n`th (zero-indexed) value counting _up_ from the current stack
pointer. This is designed to help with retrieving locally bound names (as in a
`let` binding). The stack pointer is initially zero (pointing to the bottom of
the stack) and is set to the top of the stack every time a function is called
(then restored to its previous value when the function returns).

The `POP1` instruction leaves the top value of the stack unchanged but removes
the second element from the top. This is useful in some cases for cleaning up
temporary values from the stack while leaving the result of a computation
undisturbed.

### Arithmetic and Logic

The OVM has a fairly standard suite of arithmetic and logic instructions. All of
these instructions pop one or two values off the stack, perform the appropriate
operation, then push the result back onto the stack. For two-argument
instructions, the left-hand argument is _lower_ on the stack. For example, to
compute `1 - 2`, the appropriate sequence of instructions is

```
CONST 1
CONST 2
SUB
```

because this will put the `1` on the stack first, and therefore it will be lower
than the `2`.

| Code  | Mnemonic | Description                       |
|-------|----------|-----------------------------------|
| 20    | ADD      | Addition                          |
| 21    | SUB      | Subtraction                       |
| 22    | MUL      | Multiplication                    |
| 23    | DIV      | Integer division                  |
| 24    | NEG      | Arithmetic negation/unary minus   |
| 25    | EQ       | Check if two values are equal     |
| 26    | NEQ      | Check if two values are not equal |
| 27    | LEQ      | Less than or equal                |
| 28    | LT       | Strictly less than                |
| 29    | AND      | Conjunction (boolean and)         |
| 2A    | OR       | Disjunction (boolean or)          |
| 2B    | NOT      | Logical negation                  |
| 2C    | MOD      | Remainder                         |
| 2D    | CONCAT   | String concatenation              |
| 2E    | CHARAT   | Extract a character from a string |

Note that the `EQ` and `NEQ` instructions work correctly for integers and
strings but not other kinds of objects. Calling these instructions on objects or
closures may cause very strange things to happen.

Also note that there are no `GEQ` and `GT` instructions.

The `CHARAT` instruction takes a string and an index and creates a new
single-character string consisting of the character at the given index of the
string. This is not used by any of the assignments in the course, but I added it
for an extension to OCaml-lite to support indexing into strings.

### Jumps

Any programming language needs a way to control when code is executed, so the
OVM provides a few instructions for branching and jumping.

| Code  | Mnemonic | Description              |
|-------|----------|--------------------------|
| 30    | BR       | Jump if a value is true. |
| 31    | JUMP     | Jump unconditionally.    |

The `JUMP` instruction pops a single value from the stack and increments the
program counter by that value. For example, `CONST 10; JUMP` will skip the next
10 bytes of the instruction sequence. Note that this does _not_ necessarily skip
10 instructions. For example, `CONST 9; JUMP; CONST 8` will _only_ skip the
`CONST` instruction: one byte skips the opcode and the remaining eight skip the
argument to `CONST`.

`BR` is like `JUMP` except that it additionally pops a value from the stack. The
program counter is incremented only if the value is non-zero. For example,
consider the two programs below:

```
CONST 1     CONST 0
CONST 19    CONST 19
BR          BR
CONST 0     CONST 0
CONST 9     CONST 9
JMP         JMP
CONST 1     CONST 1
```

The left program will leave a one on the stack: `BR` sees a non-zero value (1)
and skips the next 19 bytes, landing on the `CONST 1` instruction. The right
program will leave a zero on the stack: `BR` sees a zero value and does not
jump, so the `CONST 0` instruction is executed followed by a jump which skips
the `CONST 1` instruction at the end.

### Functions and Calls

These instructions are used to build function objects and to call those
functions.

| Code  | Mnemonic       | Description                                 |
|-------|----------------|---------------------------------------------|
| 40    | CLOSE s,n,f    | Construct a closure.                        |
| 41    | CALL           | Call a closure on an argument.              |
| 42    | RET            | Return from a function.                     |
| 43    | CLOSEREC s,n,f | Construct a recursive closure.              |
| 44    | CALLPOP n      | Call a closure and values.                  |
| 45    | CALLN n        | Call a function with multiple arguments.    |
| 46    | CALLNP n,p     | Call a function with multiple args and pop. |
| 52    | ENV n          | Retrieve ane environment value.             |

A closure is constructed by the `CLOSE` instruction, which takes three
arguments, each occupying one byte of the instruction stream:

- (`s`) The total size of the closure's environment _at the time that it will
  execute code_. This is the sum of the number of captured variables and the
  number of arguments to the function.
- (`n`) The number of captured variables.
- (`f`) An index into the function pointer table indicating which code is
  associated with this closure.

The closure will pop the next `n` values from the stack, which should be the
captured variables. It will then push a closure object. The `CLOSEREC`
instruction is identical except that the closure environment contains a
reference to the closure itself for recursive calls. In this case, `s` should be
one more than the number of captured variables plus the number of arguments
because there needs to be space for the recursive reference.

The `CALL` instruction is used to invoke a closure. It pops a closure from the
top of the stack and then an argument, then it inserts the argument into the
closure's environment. If the environment is full (i.e., there are no more
arguments left for the closure), the `CALL` instruction then sets the stack
pointer to the top of the stack, sets the OVM environment to the closure
environment, and jumps to the function indicated by the `f` argument to the
closure. If there are more arguments to the closure, then a copy of the original
closure with the new argument added is pushed onto the stack.

The `RET` instruction returns from a function. `RET` assumes that the function
that was just executed left exactly one value on the stack (the return value).
It then restores the stack pointer, environment, and program counter that
existed prior to the `CALL` instruction used to invoke the function. It leaves
the return value on the top of the stack.

The `ENV` instruction is used by closures to access their captured variables and
arguments (and to make recursive calls if the closure is recursive). It takes a
single argument (`n`) from the instruction stream. It treats this as in index
into the current environment. For a closure constructed with arguments `s,n,f`,
the first `n` values in the environment are the values captured by the `CLOSE`
instruction, in order from bottom to top of the stack. If the closure is
recursive, this is followed by the recursive reference. The remaining slots of
the environment are the arguments in the order they were applied. For example,

```
CONST 200
CONST 100
CONST 10
CONST 20
CONST 30
CLOSE 5 3 17 # The closure captures 10, 20, and 30, leaves 100 and 200
CALL         # Doesn't execute code because closure expects another argument.
CALL
```

In the function called by the last `CALL` instruction, the environment array
will be `[10, 20, 30, 100, 200]`, so `ENV 2` would get `30`. If the closure were
recursive, the recursive closure reference would be inserted between `30` and
`100` in the environment.

A `CALLN` takes an argument `n` and acts like a sequence of `n` `CALL`
instructions. It is a little more efficient because it does not allocate
closures for the results of intermediate calls. It is provided _only_ for
efficiency; OCaml-lite code can be compiled correctly using `CALL` alone.

If a `CALL` is followed immediately by `RET` then the current environment, stack
pointer, and program counter are not saved. This effectively allows the called
closure to return immediately to the calling environment, skipping the current
stack frame. This is called tail-call optimization and is commonly performed by
compilers for functional programming languages. In the OVM, tall call
optimization is handled by the VM itself. In particular, this allows recursive
functions to call themselves many times without consuming much memory.

`CALLPOP` is a combination of `CALL` and a number of `POP1` instructions. It
reads one byte (`n`) from the instruction stream. It then pops a closure and an
argument from the stack then discards the top `n` elements of the stack before
calling the function. It is semantically equivalent a `CALL` instruction
followed by `n` instances of `POP1`. However it is more efficient in the case of
code with the form

```
CALL
POP1
...   (more POP1)
POP1
RET
```

In this case, `CALLPOP` will perform the pop operations _before_ the function
call, allowing the call to be tail-call optimized. As with `CALLN`, the
`CALLPOP` instruction is provided for efficiency and is not required to
correctly compile OCaml-lite code.

`CALLNP` is a combination of `CALLN` and `CALLPOP`. It pops a closure and  `n`
arguments from the top of the stack like `CALLN`, then it pops `p` values before
calling the function.

### Building Data

These instructions are designed to support algebraic data types.

| Code  | Mnemonic | Description               |
|-------|----------|---------------------------|
| 60    | CONS t,n | Construct an object.      |
| 61    | GETTAG   | Get the tag of an object. |
| 62    | DECONS n | Deconstruct an object.    |

The `CONS` instruction takes a tag (`t`) and a number (`n`), each a single byte
from the instruction stream. It pops the next `n` values from the stack and
bundles them together into an object with tag `t`.

The `DECONS` instruction takes a single argument (`n`) and pops an object from
the stack. It assumes the object has `n` fields and pushes each field onto the
stack. The order of the fields on the stack is such that running `CONS t,n`
followed immediately by `DECONS n` leaves the stack in the same state it started
in.

`GETTAG` looks for an object at the top of the stack and pushes the tag of
that object. Note that `GETTAG` does _not_ pop the object from the top of the
stack.

### System Calls

Finally, there are a few higher-level utilities which are built directly into
the VM. These correspond to the built-in functions of OCaml-lite.

| Code  | Mnemonic | Description                                      |
|-------|----------|--------------------------------------------------|
| F0    | PRINT    | Print a string to stdout then push 0.            |
| F1    | STOI     | Convert a string to an integer.                  |
| F2    | ITOS     | Convert an integer to a string.                  |
| F3    | ERROR    | Pop a string x, print it as an error, then exit. |

## Assembler and Disassembler

As a convenience, I've included an assembler and a disassembler along with the
VM (in the Python files `assemble.py` and `disassemble.py`). The assembler takes
a file containing OVM code in textual form and converts it to the bytecode
format readable by the VM. The disassembler takes an OVM bytecode file and
represents it in a human-readable form. You can use these two files for
debugging and exploration to understand how the VM works and to see if your
compiler is producing the bytecode you expect. The `test/` directory contains a
number of assembly files which you can look at for examples of the syntax
expected by the assembler.

For debugging your compiler code, it will also be useful to be able to convert
binary files to a human-readable form. If you use Emacs, you can do this with
`hexl-mode`, otherwise the command line tools `xxd` and `hexdump` can help with
this.

## Code Structure

You don't need to look at the code of the VM at all for this class. But in case
it's interesting to you, I have tried to leave the code well-documented and I'll
include a brief description of some high-level ideas here. The code is also
annotated with comments in the format used by Doxygen, a popular documentation
generator. To build the documentation, install Doxygen then run `doxygen` in the
root directory of the VM code. This will generate a folder `html`. You can read
the documentation by opening a browser and looking at `html/index.html`.

The core of the OVM is the data stack which is defined in `stack.h`. This is a
fairly straightforward implementation of a stack based on a dynamically resized
array. In addition to the normal stack operations, we also allow a caller to
read the value at any index of the stack (`stack_peek`). This is to support the
`OVER` instruction. It also has special instructions for pushing and popping
integers which are needed because of how the OVM handles garbage collection (see
below).

The values on the stack are `object`s, defined in `types.h`. An object can be an
integer, a stack pointer, a string, a closure, or an environment (which is an
array of objects), so the type `object` represents a union of these types. Note
that data objects (as constructed by `CONS`) are not a separate type--they are
represented as environments where the first value is the tag. Closures are heap
allocated because this keeps the size of the `object` union small (almost
universally 8 bytes, though it technically depends on the CPU architecture) so
that the stack can be stored compactly.

The main code for the VM is contained in `main.c`, which is mostly a single loop
over the program instructions. This loop contains a big `switch` statement which
reads and instruction and handles it appropriately. It references a few utility
files. `readfile.c` has the code that actually reads and parses an OVM
bytecode file. `machine_state.c` is a tiny file that holds a few global
variables describing the state of the machine (the stack and the current
environment). These are separated out into a different file to support the
garbage collector.

Finally, memory management for the OVM is handled by `alloc.c`. See the comments
in that file (or its generated documentation) for a description of the
allocation and garbage collection strategies.

There are a number of tests in the `test/` directory to make sure the VM is
working correctly. These can be run by executing the `run_all_tests.sh` script
from inside the test directory.
