#include "arrows.h"
#include "chunk.h"

constexpr int8_t dx[4] = {0, 1, 0, -1};
constexpr int8_t dy[4] = {-1, 0, 1, 0};

void Arrow(Chunk& chunk, uint8_t position)
{
		uint8_t _direction = (chunk.direction[position/4] >> (3-position%4)*2)&3;
		int8_t x = position & 15;
		int8_t y = position >> 4;
		int8_t nx = x + dx[_direction];
		int8_t ny = y + dy[_direction];

		if(nx < 0)
		{
			nx = 15;
			if(chunk.neighbours[3])
			chunk.neighbours[3]->AddState(1, ny*16+nx);
		}
		else if(nx >= 16)
		{
			nx = 0;
			if(chunk.neighbours[4])
			chunk.neighbours[4]->AddState(1, ny*16+nx);
		}
		else if(ny < 0)
		{
			ny = 15;
			if(chunk.neighbours[1])
			chunk.neighbours[1]->AddState(1, ny*16+nx);
		}
		else if(ny >= 16)
		{
			ny = 0;
			if(chunk.neighbours[6])
			chunk.neighbours[6]->AddState(1, ny*16+nx);
		}
		else
		{
		chunk.AddState(1, ny*16+nx);
		}
}

void Source(Chunk& chunk, uint8_t position)
{
	int8_t x = position & 15;
	int8_t y = position >> 4;
	int8_t nx;
	int8_t ny;

	chunk.AddState(1, position);
	for(uint8_t i = 0; i < 4; i++)
	{
		nx = x + dx[i];
		ny = y + dy[i];
	
		if(nx < 0)
		{
			nx = 15;
			if(chunk.neighbours[3] != nullptr)
			chunk.neighbours[3]->AddState(1, ny*16+nx);
		}
		else if(nx >= 16)
		{
			nx = 0;
			if(chunk.neighbours[4] != nullptr)
			chunk.neighbours[4]->AddState(1, ny*16+nx);
			
		}
		else if(ny < 0)
		{
			ny = 15;
			if(chunk.neighbours[1] != nullptr)
			chunk.neighbours[1]->AddState(1, ny*16+nx);
		}
		else if(ny >= 16)
		{
			ny = 0;
			if(chunk.neighbours[6] != nullptr)
			chunk.neighbours[6]->AddState(1, ny*16+nx);
		}
		else
		{
		chunk.AddState(1, ny*16+nx);
		}
	}
}

void Breaker(Chunk& chunk, uint8_t position)
{
		uint8_t _direction = (chunk.direction[position/4] >> (3-position%4)*2)&3;
		int8_t x = position & 15;
		int8_t y = position >> 4;
		int8_t nx = x + dx[_direction];
		int8_t ny = y + dy[_direction];

		if(nx < 0)
		{
			nx = 15;
			if(chunk.neighbours[3])
			chunk.neighbours[3]->AddState(0, ny*16+nx);
		}
		else if(nx >= 16)
		{
			nx = 0;
			if(chunk.neighbours[4])
			chunk.neighbours[4]->AddState(0, ny*16+nx);
		}
		else if(ny < 0)
		{
			ny = 15;
			if(chunk.neighbours[1])
			chunk.neighbours[1]->AddState(0, ny*16+nx);
		}
		else if(ny >= 16)
		{
			ny = 0;
			if(chunk.neighbours[6])
			chunk.neighbours[6]->AddState(0, ny*16+nx);
		}
		else
		{
		chunk.AddState(0, ny*16+nx);
		}
}
void Delayer(Chunk& chunk, uint8_t position)
{
		uint8_t _direction = (chunk.direction[position/4] >> (3-position%4)*2)&3;
		int8_t x = position & 15;
		int8_t y = position >> 4;
		int8_t nx = x + dx[_direction];
		int8_t ny = y + dy[_direction];

		if(((chunk.special_state[position/8] >> (7-position%8)) & 1) == 0)
		{
			//std::cout << "ss: "<< int(((chunk.special_state[position/8] >> (7-position%8)) & 1)) << std::endl;
			//std::cout << "s: "<<int((chunk.state[position/4] >> (3-position%4)*2)&3) << std::endl;
			chunk.special_state[position/8] |= (1 << (7-position%8));
			chunk.AddState(1, position);
		}
		else
		{
			if(((chunk.next_state[position/4] >> (3-position%4)*2)&3) == 0)
				chunk.special_state[position/8] &= ~(1 << (7-position%8));

			if(nx < 0)
			{
				nx = 15;
				if(chunk.neighbours[3] != nullptr)
				chunk.neighbours[3]->AddState(1, ny*16+nx);
			}
			else if(nx >= 16)
			{
				nx = 0;
				if(chunk.neighbours[4] != nullptr)
				chunk.neighbours[4]->AddState(1, ny*16+nx);
			}
			else if(ny < 0)
			{
				ny = 15;
				if(chunk.neighbours[1] != nullptr)
				chunk.neighbours[1]->AddState(1, ny*16+nx);
			}
			else if(ny >= 16)
			{
				ny = 0;
				if(chunk.neighbours[6] != nullptr)
				chunk.neighbours[6]->AddState(1, ny*16+nx);
			}
			else
			{
				chunk.AddState(1, ny*16+nx);
			}
		
		}
}
void Gruber(Chunk& chunk, uint8_t position)
{
		uint8_t _direction = (chunk.direction[position/4] >> (3-position%4)*2)&3;
		int8_t x = position & 15;
		int8_t y = position >> 4;
		int8_t nx = x + dx[_direction];
		int8_t ny = y + dy[_direction];
		int8_t bx = x - dx[_direction];
		int8_t by = y - dy[_direction];
		bool handover = false;

		if(((chunk.special_state[position/8] >> (7-position%8)) & 1) == 1)
		{
		chunk.special_state[position/8] &= ~(1 << (7-position%8));
		if(nx < 0)
		{
			nx = 15;
			if(chunk.neighbours[3])
			chunk.neighbours[3]->AddState(1, ny*16+nx);
		}
		else if(nx >= 16)
		{
			nx = 0;
			if(chunk.neighbours[4])
			chunk.neighbours[4]->AddState(1, ny*16+nx);
		}
		else if(ny < 0)
		{
			ny = 15;
			if(chunk.neighbours[1])
			chunk.neighbours[1]->AddState(1, ny*16+nx);
		}
		else if(ny >= 16)
		{
			ny = 0;
			if(chunk.neighbours[6])
			chunk.neighbours[6]->AddState(1, ny*16+nx);
		}
		else
		{
		chunk.AddState(1, ny*16+nx);
		}
		}


		if(bx < 0)
		{
			bx = 15;
			if(chunk.neighbours[3])
			handover = (chunk.neighbours[3]->state[(by*16+bx)/4] >> (3-(by*16+bx)%4)*2)&3;
		}
		else if(bx >= 16)
		{
			bx = 0;
			if(chunk.neighbours[4])
			handover = (chunk.neighbours[4]->state[(by*16+bx)/4] >> (3-(by*16+bx)%4)*2)&3;
		}
		else if(by < 0)
		{
			by = 15;
			if(chunk.neighbours[1])
			handover = (chunk.neighbours[1]->state[(by*16+bx)/4] >> (3-(by*16+bx)%4)*2)&3;
		}
		else if(by >= 16)
		{
			by = 0;
			if(chunk.neighbours[6])
			handover = (chunk.neighbours[6]->state[(by*16+bx)/4] >> (3-(by*16+bx)%4)*2)&3;
		}
		else
		{
			handover = (chunk.state[(by*16+bx)/4] >> (3-(by*16+bx)%4)*2)&3;
		}
		if(handover)
		{
			chunk.special_state[position/8] |= (1 << (7-position%8));
		}
}
