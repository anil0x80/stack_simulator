#include "stack.h"
#include <iostream>
#include <cassert>

void stack::push(uint32_t size, std::string tag, bool is_uninitialized)
{
	/* first update every node's start address so they now start from +size */
	for (auto& node : nodes)
		node.update_position(size);

	nodes.emplace_back(size, std::move(tag), is_uninitialized); // is_uninitialized is 0, because we are pushing, not allocating.
}

void stack::pop(uint32_t count)
{
	/*
	pop the first n item, add their byte size to a variable, update position of others by -size.
	*/
	
	int32_t popped_byte_size = 0;
	for (uint32_t i = 0; i < count; i++)
	{
		auto& p_last = nodes.back(); // reference to last element.
		popped_byte_size += p_last.get_size(); // add its size
		nodes.pop_back();
	}

	/* adjust the start positions by -popped_byte_size */
	for (auto& node : nodes)
		node.update_position(-popped_byte_size);
}

void stack::print(std::ostream& stream)
{
	/* we need to traverse from front to back [bottom to top], otherwise print order would be reverse */
	for (const auto& node : nodes)
	{
		uint32_t node_last_byte = node.get_start() + node.get_size() - 1;
		uint32_t node_first_byte = node.get_start();
		uint32_t node_size = node.get_size();
		std::string node_tag = node.get_tag();

		stream << node_last_byte << "\t<-\tLast byte of " << node_tag << std::endl;
		stream << node_first_byte << "\t<-\tFirst byte of " << node_tag << ", size of node: " << node_size << std::endl;
	}
}

void stack::reset()
{
	nodes.clear();
}

/*
nodes:
HIGHER MEMORY												LOWER MEMORY
front -> ret_adr[esp+24], old ebp, new_node, buffer, local_args[esp] <- back
*/

/*
* divides the allocation that contains the offset, creating a new node with size inside it.
* after operation, allocation might be destroyed, split, or gets its size reduced.
*/
void stack::divide(uint32_t offset, std::string tag, uint32_t size)
{
	/* find the uninitialized node that offset resides in */
	int target_allocation_idx = 0;
	stack_node* target_allocation = nullptr;
	for (auto& node : nodes)
	{
		if (node.get_is_uninitialized() && offset >= node.get_start() && offset < node.get_start() + node.get_size())
		{
			/* found the node */
			target_allocation = &node;
			break;
		}
		target_allocation_idx++;
	}

	if(target_allocation)
	{
		assert(size <= target_allocation->get_size());
		assert(offset >= target_allocation->get_start());
		
		std::string allocation_tag = target_allocation->get_tag();

		uint32_t allocation_bottom_size; // the size to be updated
		uint32_t allocation_upper_size;  // the size to be allocated as new block.
		allocation_bottom_size = offset - target_allocation->get_start();
		allocation_upper_size = target_allocation->get_start() + target_allocation->get_size() - offset - size;

		nodes.emplace(nodes.begin() + target_allocation_idx, offset, size, tag, false);
		/* do not use target_allocation anymore */
		

		if (allocation_bottom_size)
		{
			/* there are still some uninitialized bytes at bottom, update target_allocation's size accordingly */
			/* no need to touch start position*/
			stack_node* bottom_allocation = &nodes[target_allocation_idx + 1];
			bottom_allocation->set_size(allocation_bottom_size);
		}
		else
		{
			/* get rid of bottom_allocation, because the size already covers all allocation */
			nodes.erase(nodes.begin() + target_allocation_idx + 1);
		}
		if (allocation_upper_size)
		{
			/* there are still some uninitialized bytes at upper, create and insert a node for them */
			nodes.emplace(nodes.begin() + target_allocation_idx, offset + size, allocation_upper_size, allocation_tag + "+", true);
		}
		else
		{
			/* the upper allocation does not exists, because the size of new node already closes the gap */
		}
	}
	else
	{
		/* allocation not found */
	}
}

void stack::show(std::ostream& stream, uint32_t offset)
{
	stack_node const* found = nullptr;
	for (const auto& node : nodes)
	{
		if (offset >= node.get_start() && offset < node.get_start() + node.get_size())
		{
			found = &node;
			break;
		}
	}

	if (found)
	{
		uint32_t node_offset = offset - found->get_start();
		stream << "byte	[" + found->get_tag() + " + " << node_offset << "]" << std::endl;
		if (node_offset % 2 == 0)
		{
			stream << "16-bit " + found->get_tag() << "[" << node_offset/ 2 << "]" << std::endl;
		}
		if (node_offset % 4 == 0)
		{
			stream << "32-bit " + found->get_tag() << "[" << node_offset / 4 << "]" << std::endl;
		}

		if (node_offset % 8 == 0)
		{
			stream << "64-bit " + found->get_tag() << "[" << node_offset / 8 << "]" << std::endl;
		}

		// byte [<tag> + node_offset]				<- each time
		// 16-bit <tag>[node_offset/2]				<- if (node_offset % 2 == 0)
		// 32-bit <tag>[node_offset/4]              <- if (node_offset % 4 == 0)
		// 64-bit <tag>[node_offset/8]              <- if (node_offset % 8 == 0)
	}
}
