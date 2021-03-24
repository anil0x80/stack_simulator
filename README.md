# stack_simulator

-> command line interface.

-> will be simulating the stack of an assembly function. x86 and x86-64 support.

-> todo fpu support


commands:
* reset  => will clear the stack

* push <size> <name>     # push initilized data (register, constant, mem adr, etc.).  adjust stack pointer.

* allocate <size> <name> # push <size> bytes with name <name>. difference is this data is not initialized, and can be divided to further variables. adjust stack pointer.

* print   # will print out the stack.

* show n  # gives detailed information about where [esp + n] exists.

* pop <count> # <- will pop the top n element from stack.

* exit # close the program

* divide <offset> <tag> <size> # divide the allocation from offset [esp+offset] with size. the allocation might split into more allocations.

