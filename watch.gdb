set pagination off
set trace-commands on
set logging on
break basic-stack-corruption-x86.c:70
commands
	watch *(int *) &thing1
	set $flubber=$bpnum
	continue
end
break basic-stack-corruption-x86.c:74
commands
	printf "deleting watchpoint %d\n", $flubber
	delete $flubber 
	continue
end
run
