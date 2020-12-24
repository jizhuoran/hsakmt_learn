#pragma once

#include <amd-dbgapi.h>

class PCGetter {
public:
	PCGetter();
	uint64_t stop_and_get_pc_offset();
};