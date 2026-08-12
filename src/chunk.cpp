#include "chunk.h"
#include "graphics.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"

ChunkCommands::ChunkCommands(Chunk* _chunk, uint32_t _start, uint16_t _count)
{
   	chunk = _chunk;
   	start = _start;
   	count = _count;
}

Chunk::Chunk(int32_t _x, int32_t _y, uint8_t _type, uint8_t position, uint8_t _direction, bool _flip)
{
	x = _x;
	y = _y;
	neighbours[4] = this;

	mesh = new ChunkMesh();
	this->PlaceArrow(_type, position, _direction, _flip);

}

Chunk::~Chunk()
{

	constexpr int opposite[9] =
	{
    	8, 7, 6,
    	5, 4, 3,
	    2, 1, 0
	};
	for (int i = 0; i < 9; ++i)
	{
		if(i == 4) continue;
		
	    if (neighbours[i])
	    {	
	        neighbours[i]->neighbours[opposite[i]] = nullptr; 
	    }
	}
	delete mesh;
	mesh = nullptr;
}

void Chunk::AddNeighbours(Chunk* neighbour, uint8_t number)
{
	neighbours[number] = neighbour;
}
void Chunk::AddOutput(Cell& cell, uint8_t dx, uint8_t dy, bool two_cell_handover)
{
	int8_t x = cell.position & 15;
	int8_t y = cell.position >> 4;

	int8_t nx = 0;
	int8_t ny = 0;

	uint8_t neighbour = 0;

	int8_t chunkX = 0;
	int8_t chunkY = 0;

	constexpr uint8_t neighbourTable[3][3] =
	{
	    {0,  1,  2},
	    {3,  4,  5}, //4 means this chunk
	    {6,  7,  8}
	};


	if(two_cell_handover)
	{
		nx = x + dx * 2;
		ny = y + dy * 2;
	}
	else
	{
		nx = x + dx;
		ny = y + dy;
	}
	
	if(nx < 0)
	{
		nx = 16 + nx;
		chunkX--;
	}
	if(nx >= 16)
	{
		nx = nx - 16;
		chunkX++;
	}
	if(ny < 0)
	{
		ny = 16 + ny;
		chunkY--;
	}
	if(ny >= 16)
	{
		ny = ny - 16;
		chunkY++;
	}
	else
	{
		//0
	}

	neighbour = neighbourTable[chunkY + 1][chunkX + 1];
		
	cell.outputs[cell.output_count] = ny*16+nx;
	cell.neighbours[cell.output_count] = neighbour;
	cell.output_count++;

}
void Chunk::CellPreprocessing(Cell& cell, uint8_t _type, uint8_t position, uint8_t _direction, bool _flip)
{
	cell.position = position;
	cell.type = _type;
	
	constexpr int8_t direct_dx[4] = {0, 1, 0, -1};
	constexpr int8_t direct_dy[4] = {-1, 0, 1, 0};

	constexpr int8_t diagonally_dx[4] = { 1, 1, -1, -1};
	constexpr int8_t diagonally_dy[4] = {-1, 1,  1, -1};

	switch(_type)
	{
		case 1:
		case 3:
		case 4:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 24:
			AddOutput(cell, direct_dx[_direction], direct_dy[_direction], false);
		break;
		case 2:
		case 21:
			AddOutput(cell, direct_dx[0], direct_dy[0], false);
			AddOutput(cell, direct_dx[1], direct_dy[1], false);
			AddOutput(cell, direct_dx[2], direct_dy[2], false);
			AddOutput(cell, direct_dx[3], direct_dy[3], false);
		break;
		case 5:
		case 6:
			AddOutput(cell, direct_dx[_direction], direct_dy[_direction], false);
			AddOutput(cell, -direct_dx[_direction], -direct_dy[_direction], false);
		break;
		case 7:
			if(_flip)
			{
				AddOutput(cell, direct_dx[_direction], direct_dy[_direction], false);
				AddOutput(cell, -direct_dx[(_direction+1) % 4], -direct_dy[(_direction+1) % 4], false);
			}
			else
			{
				AddOutput(cell, direct_dx[_direction], direct_dy[_direction], false);
				AddOutput(cell, direct_dx[(_direction+1) % 4], direct_dy[(_direction+1) % 4], false);
			}
		break;
		case 8:
			AddOutput(cell, direct_dx[_direction], direct_dy[_direction], false);
			AddOutput(cell, direct_dx[(_direction+1) % 4], direct_dy[(_direction+1) % 4], false);
			AddOutput(cell, direct_dx[(_direction+3) % 4], direct_dy[(_direction+3) % 4], false);
		break;
		case 9:
			AddOutput(cell, direct_dx[0], direct_dy[0], false);
			AddOutput(cell, direct_dx[1], direct_dy[1], false);
			AddOutput(cell, direct_dx[2], direct_dy[2], false);
			AddOutput(cell, direct_dx[3], direct_dy[3], false);
		break;
		case 10:
				AddOutput(cell, direct_dx[_direction], direct_dy[_direction], true);
		break;
		case 11:
			if(_flip)
			{
				AddOutput(cell, -diagonally_dx[(_direction+1) % 4], -diagonally_dy[(_direction+1) % 4], false);
			}
			else
			{
				AddOutput(cell, diagonally_dx[_direction], diagonally_dy[_direction], false);
			}
		break;
		case 12:
			AddOutput(cell, direct_dx[_direction], direct_dy[_direction], true);
			AddOutput(cell, direct_dx[_direction], direct_dy[_direction], false);
		break;
		case 13:
			if(_flip)
			{
				AddOutput(cell, direct_dx[_direction], direct_dy[_direction], true);
				AddOutput(cell, -direct_dx[(_direction+1) % 4], -direct_dy[(_direction+1) % 4], false);
			}
			else
			{
				AddOutput(cell, direct_dx[_direction], direct_dy[_direction], true);
				AddOutput(cell, direct_dx[(_direction+1) % 4], direct_dy[(_direction+1) % 4], false);
			}
		break;
		case 14:
			if(_flip)
			{
				AddOutput(cell, direct_dx[_direction], direct_dy[_direction], false);
				AddOutput(cell, -diagonally_dx[(_direction+1) % 4], -diagonally_dy[(_direction+1) % 4], false);
			}
			else
			{
				AddOutput(cell, direct_dx[_direction], direct_dy[_direction], false);
				AddOutput(cell, diagonally_dx[_direction], diagonally_dy[_direction], false);
			}
		break;
		case 25:
			AddOutput(cell, 0, 0, false);
		break;

	}
	
}
bool Chunk::PlaceArrow(uint8_t _type, uint8_t position, uint8_t _direction, bool _flip)
{
	next_dirt = true;
	if(_type != 0)
	{
		if(_type != type[position])
		{
			type[position] = _type;
			current_state[position] = 0;
		}
		
		direction[position] = _direction;
		if(_flip)
			flip[position / 8] |= (1 << (7 - position % 8));
		else
    		flip[position / 8] &= ~(1 << (7 - position % 8));

		for(int i = 0; i < type_list.size(); i++)
		{
			if(type_list[i]->position == position)
			{
				type_list.erase(type_list.begin() + i);
				break;
			}
		}
		
		Cell* cell = new Cell;
		CellPreprocessing(*cell, _type, position, _direction, _flip);
		type_list.push_back(cell);

		Cell* newCell = type_list.back();

		mesh->CalculateCellMesh(this, position, _direction, _flip);
		mesh->ChangeState(this, newCell->position, IsActive(4,newCell->type, newCell->position));
		this->mesh->Update();
	}
	else
	{
		type[position] = _type;
		direction[position] = 0;
		flip[position / 8] &= ~(1 << (7 - position % 8));

		for(int i =0; i < type_list.size(); i++)
		{
			if(type_list[i]->position == position)
			{
				type_list.erase(type_list.begin() + i);
			
			}
		}
		if(type_list.size() == 0)
		{
			return true;
		}
		next_state[position] = 0;
		current_state[position] = 0;
		special_state[position / 8] &= ~(1 << (7 - position % 8));
		mesh->CalculateCellMesh(this, position, 0, 0);
	}
	return false;
}
void Chunk::SwapBuffers()
{	
	if(next_dirt || dirt)
	{
		std::swap(current_state, next_state);
		std::swap(special_state, current_special_state);
	}
	
}
inline void Chunk::AddState(uint8_t neighbour_index, bool _state, uint8_t pos)
{
	Chunk* chunk = neighbours[neighbour_index];
	if(neighbour_index != 4)
		next_dirt = true;
	
	if(!chunk)
		return;

	if(neighbour_index != 4)
		chunk->next_dirt = true;
	
	if(_state)
		chunk->next_state[pos]++;
	else
		chunk->next_state[pos] = 128; // 128 and more means force to off
}
inline bool Chunk::IsActive(uint8_t neighbour_index, uint8_t type, uint8_t pos)
{

	Chunk* chunk = this->neighbours[neighbour_index];
	if(chunk->current_state[pos] < 128)
	{
		switch(type)
		{
			case 1:
			case 2:
			case 3:
			case 4:
			case 6:
			case 7:
			case 8:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 21:
			case 24:
			case 25:
				if(chunk->current_state[pos] > 0)
					return 1;
			break;
			case 5:
				if(((chunk->current_special_state[pos/8] >> (7-pos%8)) & 1))
					return 1;
			break;
			case 9:
				if(((chunk->current_special_state[pos/8] >> (7-pos%8)) & 1) || chunk->current_state[pos] > 0)
					return 1;
			break;
			case 15:
				if(chunk->current_state[pos] == 0)
					return 1;
			break;
			case 16:
				if(chunk->current_state[pos] > 1)
					return 1;
			break;
			case 17:
				if(chunk->current_state[pos] % 2 != 0)
					return 1;
			break;
			case 18:
				if(chunk->current_state[pos] == 1)
					return 0;
				if(((chunk->current_special_state[pos/8] >> (7-pos%8)) & 1) || chunk->current_state[pos] > 1)
					return 1;
			break;
			case 19:
				if(chunk->current_state[pos] > 0)
					return !((chunk->current_special_state[pos/8] >> (7-pos%8)) & 1);
				else
					return ((chunk->current_special_state[pos/8] >> (7-pos%8)) & 1);
			break;
			case 20:
	    		rand_seed ^= rand_seed << 13;
	    		rand_seed ^= rand_seed >> 17;
	    		rand_seed ^= rand_seed << 5;
	    		next_dirt = true;
	
				if(chunk->current_state[pos] > 0 && rand_seed & 1)
				return 1;
			break;
		}
	}
	return 0;
}
void Chunk::UpdateDirtness()
{
	bool active = false;
	if(dirt || next_dirt)
	{
		for(int i = 0; i < type_list.size(); i++)
		{
			uint8_t pos = type_list[i]->position;
			uint8_t spst_pos = pos/8;
			uint8_t shift = 7-pos%8;
			if(current_state[pos] != next_state[pos] || (((current_special_state[spst_pos] >> shift) & 1) != ((special_state[spst_pos] >> shift) & 1)) )
			{
				mesh->ChangeState(this, type_list[i]->position, IsActive(4, type_list[i]->type, pos));
				active = true;
			} 
		}
		if(active)
		mesh->Update();
	}
		
	dirt = active || next_dirt;
	next_dirt = 0;
	
	if(!dirt)
	return;
	
	memset(next_state, 0, 256);
	memset(special_state, 0, 32);
	
}
void Chunk::TeakProcessing()
{
	for(int i = 0; i < type_list.size(); i++)
	{
		Cell* cell = type_list[i];
		uint8_t pos = cell->position;
		bool cur_special_state = ((current_special_state[cell->position/8] >> (7-cell->position%8)) & 1);
		switch(cell->type)
		{
			case 1:
			case 10:
			case 11:
			case 15:
			case 16:
			case 17:
			case 20:
			case 24:
				if(IsActive(4, cell->type, pos))
					AddState(cell->neighbours[0], true, cell->outputs[0]);
			break;
			case 2:
				if(IsActive(4, cell->type, pos))
					for(int j = 0; j < 4; j++)
						AddState(cell->neighbours[j], true, cell->outputs[j]);
				AddState(4, true, cell->position);
			break;
			case 21:
				if(IsActive(4, cell->type, pos))
					for(int j = 0; j < 4; j++)
						AddState(cell->neighbours[j], true, cell->outputs[j]);
			break;
			case 3:
				if(IsActive(4,cell->type, pos))
					AddState(cell->neighbours[0], false, cell->outputs[0]);
			break;
			case 4:
				if(cur_special_state == 0 && current_state[pos] > 0)
				{
					special_state[pos/8] |= (1 << (7-pos%8));
					AddState(4, true, pos);
				}
				else if(cur_special_state == 1 && current_state[pos] > 0)
				{
					special_state[pos/8] |= (1 << (7-pos%8));
					AddState(cell->neighbours[0], true, cell->outputs[0]);
				}
			break;
			case 5:
				if(IsActive(4, cell->type, pos))
					AddState(cell->neighbours[0], true, cell->outputs[0]);
				if(this->neighbours[cell->neighbours[1]] && IsActive(cell->neighbours[1], this->neighbours[cell->neighbours[1]]->type[cell->outputs[1]], cell->outputs[1]))
				{
					special_state[pos/8] |= (1 << (7-pos%8));
				}
			break;
			case 6:
			case 7:
			case 12:
			case 13:
			case 14:
				if(IsActive(4, cell->type, pos))
					for(int j = 0; j < 2; j++)
						AddState(cell->neighbours[j], true, cell->outputs[j]);
			break;
			case 8:
				if(IsActive(4, cell->type, pos))
					for(int j = 0; j < 3; j++)
						AddState(cell->neighbours[j], true, cell->outputs[j]);
			break;
			case 9:
				if(cur_special_state == 0 && current_state[pos] > 0)
				{
					for(int j = 0; j < 4; j++)
						AddState(cell->neighbours[j], true, cell->outputs[j]);
				}
				AddState(4, true, pos);
				if(current_state[pos] < 128 && current_state[pos] > 0)
				{
				special_state[pos/8] |= (1 << (7-pos%8));
				}
			break;
			case 18:
			case 19:
				if(IsActive(4, cell->type, pos))
				{
					special_state[pos/8] |= (1 << (7-pos%8));
					AddState(cell->neighbours[0], true, cell->outputs[0]);
				}
			break;
		}
	}
}
