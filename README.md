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
bf program.bf "input string for the runtime"
bf program.bf -
```

The input stream could be a predefined string or just stdin.
If the bf program is syntactically correct the interpreter executes it, otherwise it prints and apporpriate error message and exits


## Requirements
The interpreter should ignore characters not in {+-><,.[]} when they appear in the source program
The interpreter should recognize when parenthesis are not balanced (eg: `[...[]` or `[...]]`), and should exit with an error message
The error messages about parenthesis should signal the position of the first error encountered


## Notes
At this point we should have a clear idea about the interaction with the system to be. It was hard not to think about implementation details, but this description should be sufficiently general to be implemented by multiple programmers using multiple technologies.
