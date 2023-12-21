#include "Block.h"
#include "Visitor.h"

void Block::accept(Visitor* v) {
	v->visitBlock(this);
}