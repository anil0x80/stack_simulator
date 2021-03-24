#pragma once
#include <cstdint>
#include <string>

/* represents a single node in x86 stack. */
class stack_node
{
public:
	/* will construct a node as start = 0, ready to be pushed into stack */
	stack_node(uint32_t size, std::string tag, bool is_uninitialized);
	/* allows you to create node with arbitary start position */
	stack_node(uint32_t start, uint32_t size, std::string tag, bool is_uninitialized);

	void update_position(int32_t offset); // shifts the start of this node by given offset
	void set_size(uint32_t size);

	uint32_t get_size() const;
	uint32_t get_start() const;
	std::string get_tag() const;
	bool get_is_uninitialized() const;

private:

	uint32_t start;			// start offset regarding to esp ([esp + n]). always positive.
	uint32_t size;			// size of this node.
	std::string tag;		// tag(name) of this node 
	bool is_uninitialized;	// this node is an allocation done for local variables, and might be divided in future.
};
