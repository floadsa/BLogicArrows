#include "chunk.h"
#include "graphics.h"

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

	type[position] = _type;

	uint8_t shift = (3 - (position % 4)) * 2;
	direction[position / 4] &= ~(3 << shift);
	direction[position / 4] |= (_direction << shift);

	mesh = new ChunkMesh();
	mesh->CalculateCellMesh(this, position, _flip);

}

Chunk::~Chunk()
{

	constexpr int opposite[8] =
	{
	    7, 6,
	    5, 4,
	    3, 2,
	    1, 0
	};

	for (int i = 0; i < 8; ++i)
	{
	    if (neighbours[i])
	    {
	        neighbours[i]->neighbours[opposite[i]] = nullptr; 
	    }
	}
	
	delete mesh;

}


void Chunk::AddState(bool _state, uint8_t number)
{
	uint8_t shift = (3 - (number % 4)) * 2;
	if(_state)
	{
		if(((next_state[number / 4] >> shift) & 3) < 3)
			next_state[number / 4] += (1 << shift);
		else
			next_state[number / 4] -= (1 << shift);
	}
	else
	{
		next_state[number / 4] &= ~(3 << shift);
		force_to_off[number/8] |= (1 << (7-number%8));
	}
}

void Chunk::AddNeighbours(Chunk* neighbour, uint8_t number)
{
	neighbours[number] = neighbour;
}
bool Chunk::PlaceArrow(uint8_t _type, uint8_t position, uint8_t _direction, bool _flip)
{
	uint8_t shift = (3 - (position % 4)) * 2;
	if(_type != 0)
	{
		if(_type != type[position])
		{
			type[position] = _type;
			state[position / 4] &= ~(3 << shift);
		}
		direction[position / 4] &= ~(3 << shift);
		direction[position / 4] |= (_direction << shift);
		mesh->CalculateCellMesh(this, position, _flip);
	}
	else
	{
		type[position] = _type;
		bool type_counter = false;
		for(int i = 0; i < 256; i++)
		{
			if(type[i] > 0)
			{
				type_counter = true;
				break;
			}
		}
		if(!type_counter)
		{
			return true;
		}
		else
		{
			state[position / 4] &= ~(3 << shift);
			mesh->CalculateCellMesh(this, position, _flip);
		}
	}
	return false;
}
void Chunk::SwapBuffers()
{
	memset(state, 0, sizeof(state));
	memset(force_to_off, 0, sizeof(force_to_off));
	std::swap(state, next_state);
}
void Chunk::UpdateMesh()
{
	for(int i = 0; i < 256; i++)
	{
		if(type[i] != 0)
		{
			uint8_t shift = (3 - (i % 4)) * 2;
			if( ((next_state[i / 4] >> shift) & 3)  != ((state[i/4] >> shift) & 3))
			{
				mesh->ChangeState(this, i, uint8_t((next_state[i/4] >> shift) & 3));
			}
		}
	}
	mesh->Update();
}
void Chunk::TeakProcessing()
{
	bool breakers_counter = false;

	for(uint8_t i = 0; ; i++)
	{
	
		if(type[i] > 0)
		{
			if(type[i] == 3)
			breakers_counter = true;

			if(((state[i/4] >> (3-i%4)*2)&3) >= 1 || type[i] == 2 || type[i] == 5)
				Chunk::handler[type[i]-1](*this, i);
		}	

		if(i == UINT8_MAX)
			break;
	}
	
	if(breakers_counter)
	for(int i = 0; i < 32; i++)
	{
		if(force_to_off[i] > 0)
		{
			for(int j = 0; j < 8; j++)
			{
				if(force_to_off[i] & (1 << (7-j)))
					next_state[(i*8+j) / 4] &= ~(3 << (3-((i*8+j)%4))*2);
			}
		}
	}


}
