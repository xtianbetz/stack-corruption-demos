# Stack Corruption Demos

## Overview

The code in this repo demonstrates how to write bad C code to subtly corrupt
the stack in (nearly) undetectable ways. For instance, you can run these
examples under valgrind and it will report 0 errors.

You might use this repo to better understand how values on the stack can become
corrupted and/or practice using watchpoints with gdb.

## Hardware watchpoints in GDB

```
x@clr-ac6c3d18a1e44dd9af337de503fff7df ~/code/stack-corruption-demos $ gdb ./basic-stack-corruption-x86-64
GNU gdb (GDB) 8.1
Copyright (C) 2018 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-generic-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from ./basic-stack-corruption-x86-64...done.
(gdb) b basic-stack-corruption-x86-64.c:65
Breakpoint 1 at 0x400671: file basic-stack-corruption-x86-64.c, line 65.
(gdb) run
Starting program: /home/x/code/stack-corruption-demos/basic-stack-corruption-x86-64
size of thing is 72, size of void* is 8
main (before foo1): thing0 is at address 0x7fffffffc170, thing->field1 = 1
foo1 (before foo2):  thing1 is a pointer at address 0x7fffffffc158 pointing to 0x7fffffffc170

Breakpoint 1, foo1 (thing1=0x7fffffffc170) at basic-stack-corruption-x86-64.c:65
65		foo2(thing1);
(gdb) p thing1
$1 = (head *) 0x7fffffffc170
(gdb) p &thing1
$2 = (head **) 0x7fffffffc158
(gdb) watch *(int *) $2
Hardware watchpoint 2: *(int *) $2
(gdb) c
Continuing.
foo2 (before smash): thing2 is a pointer at address 0x7fffffffc128 pointing to 0x7fffffffc170 [local variable x = 42]
Address of pointer we want to change: 0x7fffffffc158
Address of badpointer->field6: 0x7fffffffc158
Difference between addresses: 0

Hardware watchpoint 2: *(int *) $2

Old value = -16016
New value = -559038737
foo2 (thing2=0x7fffffffc170) at basic-stack-corruption-x86-64.c:48
48		printf("Address of other pointer we want to change: %p\n",
(gdb) p (int) 0xdeadbeef
$3 = -559038737
```

## GDB Scripting Example

An example watch.gdb script is included. The scripts dynamically adds and
removes a watchpoint using two breakpoints.

```
gdb -x watch.gdb ./basic-stack-corruption-x86
```

## TODO

* Demo that works on x86 32-bit
* Demo with loops and randomized smashing for testing gdb watchpoint scripting
* Single source file with ifdefs, etc if needed to work on all architectures.
