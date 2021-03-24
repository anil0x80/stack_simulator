#pragma once
#include "stack_node.h"
#include <vector>

class stack
{
public:
	void push(uint32_t size, std::string tag, bool is_uninitialized);
	void pop(uint32_t count);
	void print(std::ostream& stream);
	void reset();
	void divide(uint32_t offset, std::string tag, uint32_t size);
	void show(std::ostream& stream, uint32_t offset);


private:
	std::vector<stack_node> nodes;
};



