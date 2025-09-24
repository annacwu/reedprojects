| Code  | Mnemonic       | Description                                |
|-------|----------------|--------------------------------------------|
| 07    | CONST n        | Push a constant onto the stack.            |
| 0F    | SCONST s       | Push a string constant onto the stack.     |
| 20    | ADD            | Addition                                   |
| 21    | SUB            | Subtraction                                |
| 22    | MUL            | Multiplication                             |
| 23    | DIV            | Integer division                           |
| 24    | NEG            | Arithmetic negation/unary minus            |
| 25    | EQ             | Check if two values are equal              |
| 26    | NEQ            | Check if two values are not equal          |
| 27    | LEQ            | Less than or equal                         |
| 28    | LT             | Strictly less than                         |
| 29    | AND            | Conjunction (boolean and)                  |
| 2A    | OR             | Disjunction (boolean or)                   |
| 2B    | NOT            | Logical negation                           |
| 2C    | MOD            | Remainder                                  |
| 2D    | CONCAT         | String concatenation                       |
| 2E    | CHARAT         | Extract a character from a string          |
| 30    | BR             | Jump if a value is true.                   |
| 31    | JUMP           | Jump unconditionally.                      |
| 40    | CLOSE s,n,f    | Construct a closure.                       |
| 41    | CALL           | Call a closure on an argument.             |
| 42    | RET            | Return from a function.                    |
| 43    | CLOSEREC s,n,f | Construct a recursive closure.             |
| 44    | CALLPOP n      | Call and pop values from the stack.        |
| 45    | CALLN n        | Call with multiple arguments.              |
| 46    | CALLNP n,p     | Call with multiple arguments and pop.      |
| 52    | ENV n          | Retrieve ane environment value.            |
| 50    | OVER n         | Copy a value from the current stack frame. |
| 51    | POP1           | Delete the second-from-top element.        |
| 60    | CONS t,n       | Construct an object.                       |
| 61    | GETTAG         | Get the tag of an object.                  |
| 62    | DECONS n       | Deconstruct an object.                     |
| F0    | PRINT          | Print a string to stdout then push 0.      |
| F1    | STOI           | Convert a string to an integer.            |
| F2    | ITOS           | Convert an integer to a string.            |
| F3    | ERROR          | Pop a string x, print it, then exit.       |
