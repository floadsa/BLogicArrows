#pragma once
#include "common.h"

struct ChunkMesh;
struct Chunk;
struct Cell;

struct ChunkCommands
{
    Chunk* chunk;
    uint32_t start;
    uint16_t count;

    ChunkCommands(Chunk* _chunk, uint32_t _start, uint16_t _count);
};

struct Cell
{
	uint8_t position;
    uint8_t type;

    uint8_t output_count = 0;
    uint8_t neighbours[4] = {4};
    uint8_t outputs[4] = {0};
};

struct Chunk
{
	int32_t x, y;
	uint32_t rand_seed = 12345689; // Randomness is bad but maybe i will improve it

	bool dirt = false;
	bool next_dirt = false;

	uint8_t state1[256] = {0};
	uint8_t state2[256] = {0};

	uint8_t special1[32] = {0};
	uint8_t special2[32] = {0};

	uint8_t* current_state = state1; //for faster swap
	uint8_t* next_state = state2;
	
	uint8_t* special_state = special1;
	uint8_t* current_special_state = special2;

	uint8_t type[256] = {0};
	uint8_t direction[256] = {0};
	uint8_t flip[32] = {0};

	std::vector<Cell*> type_list;

	Chunk* neighbours[9] = {nullptr};
	ChunkMesh* mesh;

	Chunk(int32_t _x, int32_t _y, uint8_t _type, uint8_t position, uint8_t _direction, bool _flip);
	~Chunk();
	
	inline void AddState(uint8_t neighbour_index, bool _state, uint8_t pos);
	inline bool IsActive(uint8_t neighbour_index,uint8_t type, uint8_t pos);
	void AddOutput(Cell& cell, uint8_t dx, uint8_t dy, bool two_cell_handover);
	void CellPreprocessing(Cell& cell, uint8_t _type, uint8_t position, uint8_t _direction, bool _flip);
	void AddNeighbours(Chunk* neighbour, uint8_t number);
	bool PlaceArrow(uint8_t _type, uint8_t position, uint8_t _direction, bool _flip);
	void SwapBuffers();
	void UpdateDirtness();
	void TeakProcessing();

};
