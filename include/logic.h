#pragma once

#include "chunk.h"
#include "common.h"

extern std::vector<std::vector<Chunk*>> chunks;

void AddChunk(std::vector<std::vector<Chunk*>>& chunks, int32_t x, int32_t y, uint8_t type, uint8_t position, uint8_t direction, bool flip);
//void ExecuteCommands();

