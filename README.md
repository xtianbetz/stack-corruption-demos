# Stack Corruption Demos

## Overview

The code in this repo demonstrates how to write bad C code to subtly corrupt
the stack in (nearly) undetectable ways. For instance, you can run these
examples under valgrind and it will report 0 errors.

You might use this repo to better understand how values on the stack can become
corrupted.

## TODO

* Demo that works on x86 32-bit
* Demo with loops and randomized smashing for testing gdb watchpoint scripting
* Single source file with ifdefs, etc if needed to work on all architectures.
