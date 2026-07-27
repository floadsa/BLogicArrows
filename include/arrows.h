#pragma once

#include "common.h"

struct Chunk;
struct ChunkCommands;

void Arrow(Chunk& chunk, uint8_t position);
void Source(Chunk& chunk, uint8_t position);
void Breaker(Chunk& chunk, uint8_t position);
void Delayer(Chunk& chunk, uint8_t position);
void Gruber(Chunk& chunk, uint8_t position);
