#pragma once

#include "common.h"
#include "arrows.h"
//#include "graphics.h"

struct ChunkMesh;

struct ChunkCommands
{
    Chunk* chunk;
    uint32_t start;
    uint16_t count;

    ChunkCommands(Chunk* _chunk, uint32_t _start, uint16_t _count);
    
};

struct Chunk
{
	int32_t x, y;

	uint8_t state[64] = {0};
	uint8_t next_state[64] = {0};

	uint8_t type[256] = {0};
	uint8_t direction[64] = {0};
	uint8_t flip[32] = {0};

	uint8_t force_to_off[32] = {0};
	uint8_t special_state[32] = {0};
	
	Chunk* neighbours[8] = {nullptr};
	ChunkMesh* mesh;

	static constexpr void (*handler[])(Chunk&, uint8_t position) =
	{
	Arrow,
	Source,
	Breaker,
	Delayer,
	Gruber,
	};


	Chunk(int32_t _x, int32_t _y, uint8_t _type, uint8_t position, uint8_t _direction, bool _flip);
	~Chunk();
	
	void AddState(bool _state, uint8_t number);
	void AddNeighbours(Chunk* neighbour, uint8_t number);
	bool PlaceArrow(uint8_t _type, uint8_t position, uint8_t _direction, bool _flip);
	void SwapBuffers();
	void UpdateMesh();
	void TeakProcessing();

};
