#pragma once

#include <fstream>
#include <string>
#include "chunk.h"
#include "graphics.h"
#include "common.h"

struct UndoCell;

extern std::vector<std::vector<Chunk*>> chunks;
extern std::vector<Chunk*> chunks_for_render;
extern std::vector<std::vector<UndoCell*>> undobuffer;
extern std::string filename;

struct UndoCell
{
	int32_t x, y;
	uint8_t position;
	uint8_t type;	
	uint8_t direction;	
	bool flip;

	UndoCell(int32_t x, int32_t y, uint8_t p, uint8_t t, uint8_t d, bool f)
		: x(x), y(y), type(t), direction(d), position(p), flip(f)
	{}
};

void ARW_Export(const std::string& filename);
void ARW_Import(const std::string& filename);
int32_t FindChunkX(int32_t x);
int32_t FindChunkY(int32_t x_index, int32_t y);
void AddChunk(int32_t x, int32_t y, uint8_t type, uint8_t position, uint8_t direction, bool flip, bool undo);
void FillRenderArray(int x1, int x2, int y1, int y2);
