/*
 * gcc -fno-stack-protector -z execstack -no-pie undetectable_stack_smash.c
 */

#include <stdio.h>
#include <sys/types.h>

typedef struct _head {
	u_int64_t field1;
	u_int64_t field2;
	u_int64_t field3;
	u_int64_t field4;
	u_int64_t field5; // note: field6 starts 40 bytes into the struct
	u_int64_t field6;
	u_int64_t field7;
	u_int64_t field8;
	u_int64_t field9;
} head;

// These pointers help us cheat and locate good smashing spots
// We never manipulate them directly, but they are useful to see which direction
// we need to go and how many bytes to travel in order to smash our target.
void *the_pointer_to_smash;
void *the_other_pointer_to_smash;

void foo2(head *thing2) {
	u_int64_t x = 42;
	head *badpointer = (head *) &x;

	printf("foo2 (before smash): thing2 is a pointer "
	       "at address %p pointing to %p [local variable x = %lu]\n",
	       &thing2, thing2, x);

	printf("Address of pointer we want to change: %p\n",
	       the_pointer_to_smash);
	printf("Address of badpointer->field6: %p\n", &badpointer->field6);
	printf("Difference between addresses: %ld\n",
	       (unsigned long) the_pointer_to_smash - (unsigned long) &badpointer->field6);

	// It looks like we're just writing a struct field...
	// Oops we just overwrote the x variable on our own stack
	badpointer->field1 = 99;

	// The following code would crash because we smash a "bad" spot
	// badpointer->field2 = 0x1234;

	// ... Instead, we overwrite the value of foo1's thing1 pointer
	//  this is pointing some number of bytes up the stack from
	//  where x is: the value of foo'
	badpointer->field6 = 0xdeadbeef;

	printf("Address of other pointer we want to change: %p\n",
	       the_other_pointer_to_smash);
	printf("Address of badpointer->field9: %p\n", &badpointer->field9);
	printf("Difference between addresses: %ld\n",
	       (unsigned long) the_other_pointer_to_smash - (unsigned long) &badpointer->field9);

	badpointer->field9 = 37;

	printf("foo2 (after smash):  thing2 is a pointer "
	       "at address %p pointing to %p [local variable x = %lu]\n",
	       &thing2, thing2, x);
}

void foo1(head *thing1) {
	printf("foo1 (before foo2):  thing1 is a pointer "
	       "at address %p pointing to %p\n", &thing1, thing1);
	the_pointer_to_smash = &thing1;
	foo2(thing1);
	printf("foo1 (after foo2):   thing1 is a pointer "
	       "at address %p pointing to %p\n", &thing1, thing1);
}

int main() {
	head thing0 = {1,1,1,1,1,1};
	the_other_pointer_to_smash = &thing0.field1;
	printf("size of thing is %lu, size of void* is %lu\n",
	       sizeof(head), sizeof(void*));
	printf("main (before foo1): thing0 is at address %p, thing->field1 = %ld\n",
	       &thing0, thing0.field1);
	foo1(&thing0);
	printf("main (after foo1): thing0 is at address %p, thing->field1 = %ld\n",
	       &thing0, thing0.field1);
	return 0;
}
