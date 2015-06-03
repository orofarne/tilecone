#pragma once

#include <cstdint>

namespace tilecone {

struct Tile {
	uint64_t offset;
	uint32_t size;
	uint32_t capacity;
};

}
