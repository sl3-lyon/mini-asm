Mini ASM [![Build Status](https://travis-ci.org/tyr-sl3/mini-asm.svg)](https://travis-ci.org/tyr-sl3/mini-asm)
===========
Custom mini ASM language &amp; CPU emulator written in C++ (C++14). <br />
This is a custom mini ASM language based on 6502 and conventional instructions (MOV, etc...). <br />
###Registers
* **A** *(Accumulator)* <br />
* **X** *(X index)* <br />
* **Y** *(Y index)* <br />
* **P** *(Program status)* <br />
* **PC** *(Program Counter)* <br />
* **S** *(Stack pointer)* <br />

###Modes
####Shell mode
Command-line interpreter
```asm
Mini ASM version 0.1.0
Created by Vincent P.
Shell mode - Type 'exit' to stop
> mov A, 42
> print A
42
```
####Interpreter (coming soon)
Reads an ASM file
