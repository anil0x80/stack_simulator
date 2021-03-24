#include <iostream>
#include <string>
#include "util.h"
#include "stack.h"

/*
* reset  => will clear the stack
* push <size> <name>     # push initilized data (register, constant, mem adr, etc.).  adjust stack pointer.
* allocate <size> <name> # push <size> bytes with name <name>. difference is this data is not initialized, and can be divided to further variables. adjust stack pointer.
* print   # will print out the stack.
* show n  # gives detailed information about where [esp + n] exists.
* pop <count> # <- will pop the top n element from stack.
* exit # close the program
* divide <offset> <tag> <size> # divide the allocation from offset [esp+offset] with size. the allocation might split into more allocations.

*/
int main()
{
	stack stack;

	/* main command loop */
	for (;;)
	{
		std::string command;
		std::getline(std::cin, command);
		auto words = util::tokenize(command);

		if (words[0] == "exit")
		{
			/* exit the loop */
			break;
		}
		else if (words[0] == "push")
		{
			stack.push(std::stoul(words[1]), words[2], false);
		}
		else if (words[0] == "pop")
		{
			stack.pop(std::stoul(words[1]));
		}
		else if (words[0] == "print")
		{
			stack.print(std::cout);
		}
		else if (words[0] == "reset")
		{
			stack.reset();
		}
		else if (words[0] == "allocate")
		{
			stack.push(std::stoul(words[1]), words[2], true);
		}
		else if (words[0] == "divide")
		{
			stack.divide(std::stoul(words[1]), words[2], std::stoul(words[3]));
		}
		else if (words[0] == "show")
		{
			stack.show(std::cout, std::stoul(words[1]));
		}
		else
		{
			std::cout << "Unsupported operation! " << std::endl;
		}
	}


	return 0;
}