# Brainfuck interpreter and compiler
Build a software that takes a brainfuck program and executes given appropriate input. Use structured approaches like a real software engineer


## Computational model
The bf computational model is made of:
  * a fixed size memory buffer of character values
  * an index to the memory
  * an input stream
  * an output stream

A bf program is a string of characters in the set {+-><,.[]} and they represent instruction for the machine
  * '+' increments the value at the current memory location
  * '-' decrements the value at the current memory location
  * '>' moves to the right the r/w head
  * '<' moves to the left  the r/w head
  * ',' reads a character from the input stream and writes into the current memory location
  * '.' writes the value at the current memory location into the output stream
  * '[' if the the value at the current memory location is     0, jump to the instruction next to the corresponding ']', otherwise move to the next instruction
  * ']' if the the value at the current memory location is NOT 0, jump to the instruction next to the corresponding '[', otherwise move to the next instruction


## Use case: launch interpreter
The user initiates the interpreter by providing a bf program and the input stream

```
bf program.b "input string for the runtime"
bf program.b -
```

The input stream could be a predefined string or just stdin.
If the bf program is syntactically correct the interpreter executes it, otherwise it prints and apporpriate error message and exits


## Requirements
The interpreter should ignore characters not in {+-><,.[]} when they appear in the source program
The interpreter should recognize when parenthesis are not balanced (eg: `[...[]` or `[...]]`), and should exit with an error message
The error messages about parenthesis should signal the position of the first error encountered


## Notes
At this point we should have a clear idea about the interaction with the system to be. It was hard not to think about implementation details, but this description should be sufficiently general to be implemented by multiple programmers using multiple technologies.

All the files in the samples directory are not mine, I will use them to test the interpreter/compiler


# What I understood from code generation
Compilers are foundational tools in my every day work. They handle complicated tasks like:
 * register allocation
 * assembling
 * linking

And also deal with the details of every architecture, for example do you know that in an ARM assembly, there can't be more than 4KB of distance from code that uses a static variable and its declaration? At least that's what I understood from google searches and gdb debugging, by the way a debugger is really useful when going at this depth in the code.

I will employ "duct tape" solutions to the pool distance problem: declaring the memory array in `runtime.c` and let the compiler figure it out. It has been an intense learning experience
