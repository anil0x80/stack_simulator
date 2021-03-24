#include "stack_node.h"

stack_node::stack_node(uint32_t size, std::string tag, bool is_uninitialized) : start(0), size(size),
tag(std::move(tag)), is_uninitialized(is_uninitialized)
{

}

stack_node::stack_node(uint32_t start, uint32_t size, std::string tag, bool is_uninitialized) : stack_node(size, tag, is_uninitialized)
{
	this->start = start;
}

void stack_node::update_position(int32_t offset)
{
	start += offset; // there won't be any overflow, unless you update the position of 
					//  last pushed/popped node (which caused the update in first place)
}

void stack_node::set_size(uint32_t size)
{
	this->size = size;
}

uint32_t stack_node::get_size() const
{
	return size;
}

uint32_t stack_node::get_start() const
{
	return start;
}

std::string stack_node::get_tag() const
{
	return tag;
}

bool stack_node::get_is_uninitialized() const
{
	return is_uninitialized;
}
