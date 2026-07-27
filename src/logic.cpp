#include "logic.h"

std::vector<std::vector<Chunk*>> chunks;

void AddChunk(std::vector<std::vector<Chunk*>>& chunks, int32_t x, int32_t y, uint8_t type, uint8_t position, uint8_t direction, bool flip)
{
	if(chunks.empty())
	{
		if(type == 0)
			return;
			
		chunks.push_back({new Chunk(x,y,type,position,direction,flip)});
			return;
	}

	int32_t left = 0;
	int32_t right = chunks.size();
	int32_t leftY = 0;
	int32_t rightY = 0;
	bool found = false;

	if(x < chunks[0][0]->x)
	{
		right = 0;
	}
	else if(x == chunks[0][0]->x)
	{
		right = 0;
		found = true;
	}
	while(right > left + 1 && !found)
	{
		int32_t midle = left + ((right - left) / 2);
	
		if(x < chunks[midle][0]->x)
		{
			right = midle;
		}
		else if(x > chunks[midle][0]->x)
		{
			left = midle;
		}
		else if(x == chunks[midle][0]->x)
		{
			right = midle;
			found = true;
		}
	}
	if(!found)
	{
		if(type==0)
			return;
			
		chunks.insert(chunks.begin() + right, {new Chunk(x,y,type,position,direction,flip)});
	}
	else
	{
		found = false;
		leftY = 0;
		rightY = chunks[right].size();
		
		if(y < chunks[right][0]->y)
		{
		rightY = 0;
		}
		else if(y == chunks[right][0]->y)
		{
		rightY = 0;
		found = true;
		}
		
		while(rightY > leftY + 1 && !found)
		{
			int32_t midleY = leftY + ((rightY - leftY) / 2);
	
			if(y < chunks[right][midleY]->y)
			{
				rightY = midleY;
			}
			else if(y > chunks[right][midleY]->y)
			{
				leftY = midleY;
			}
			else if(y == chunks[right][midleY]->y)
			{
				rightY = midleY;
				found = true;
			}
		}
		if(found)
		{
			if(chunks[right][rightY]->PlaceArrow(type, position, direction, flip))
			{
				delete chunks[right][rightY];
				if (chunks[right].empty() || chunks[right].size() == 1)
				{	
				    chunks.erase(chunks.begin() + right);
				}
				else
				{		
					chunks[right].erase(chunks[right].begin() + rightY);
				}
			}
			return;
		}
		else
		{
			if(type==0)
				return;
			chunks[right].insert(chunks[right].begin() + rightY, new Chunk(x,y,type,position,direction,flip));
		}
	}

	//NEIGHBOURS FINDER
	if(rightY !=0)
	{
		if(chunks[right][rightY]->y == chunks[right][rightY-1]->y+1)
		{
			chunks[right][rightY]->neighbours[6] = chunks[right][rightY-1];
			chunks[right][rightY-1]->neighbours[1] = chunks[right][rightY];
		}
	}
	if(rightY < chunks[right].size() - 1)
	{
		if(chunks[right][rightY]->y == chunks[right][rightY+1]->y-1)
		{
			chunks[right][rightY+1]->neighbours[6] = chunks[right][rightY];
			chunks[right][rightY]->neighbours[1] = chunks[right][rightY+1];
		}
	}
	int32_t central_rightY = rightY;

	if(right != 0 && chunks[right][0]->x == chunks[right-1][0]->x+1)
	{
		leftY = 0;
		rightY = chunks[right-1].size();

		if(y-1 < chunks[right-1][0]->y || y-1 == chunks[right-1][0]->y)
		{
			rightY = 0;
		}
		
		while(rightY > leftY + 1)
		{
			int32_t midleY = leftY + ((rightY - leftY) / 2);
	
			if(y-1 <= chunks[right-1][midleY]->y)
			{
				rightY = midleY;
			}
			else
			{
				leftY = midleY;
			}
		}

		for(int i = 0; i < 3 && rightY+i < chunks[right-1].size(); i++)
		{
			if(chunks[right-1][rightY+i]->y == chunks[right][central_rightY]->y-1)
			{
				chunks[right][central_rightY]->neighbours[5] = chunks[right-1][rightY+i];
				chunks[right-1][rightY+i]->neighbours[2] = chunks[right][central_rightY];
			}
			if(chunks[right-1][rightY+i]->y == chunks[right][central_rightY]->y)
			{
				chunks[right][central_rightY]->neighbours[3] = chunks[right-1][rightY+i];
				chunks[right-1][rightY+i]->neighbours[4] = chunks[right][central_rightY];
			}
			if(chunks[right-1][rightY+i]->y == chunks[right][central_rightY]->y+1)
			{
				chunks[right][central_rightY]->neighbours[0] = chunks[right-1][rightY+i];
				chunks[right-1][rightY+i]->neighbours[7] = chunks[right][central_rightY];
			}
		}
	}
	if(right < chunks.size() - 1 && chunks[right][0]->x == chunks[right+1][0]->x-1)
	{
		leftY = 0;
		rightY = chunks[right+1].size();
		
		if(y-1 < chunks[right+1][0]->y || y-1 == chunks[right+1][0]->y)
		{
			rightY = 0;
		}
		while(rightY > leftY + 1)
		{
			int32_t midleY = leftY + ((rightY - leftY) / 2);
	
			if(y-1 <= chunks[right+1][midleY]->y)
			{
				rightY = midleY;
			}
			else
			{
				leftY = midleY;
			}
		}

		for(int i = 0; i < 3 && rightY + i < chunks[right+1].size(); i++)
		{
			if(chunks[right+1][rightY+i]->y == chunks[right][central_rightY]->y-1)
			{
				chunks[right][central_rightY]->neighbours[7] = chunks[right+1][rightY+i];
				chunks[right+1][rightY+i]->neighbours[0] = chunks[right][central_rightY];
			}
			if(chunks[right+1][rightY+i]->y == chunks[right][central_rightY]->y)
			{
				chunks[right][central_rightY]->neighbours[4] = chunks[right+1][rightY+i];
				chunks[right+1][rightY+i]->neighbours[3] = chunks[right][central_rightY];
			}
			if(chunks[right+1][rightY+i]->y == chunks[right][central_rightY]->y+1)
			{
				chunks[right][central_rightY]->neighbours[2] = chunks[right+1][rightY+i];
				chunks[right+1][rightY+i]->neighbours[5] = chunks[right][central_rightY];
			}
		}
	}
}
