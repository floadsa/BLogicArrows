#include "logic.h"
#include <iomanip>

std::vector<std::vector<Chunk*>> chunks;
std::vector<Chunk*> chunks_for_render;
std::vector<std::vector<UndoCell*>> undobuffer;
std::string filename;

static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

void base64Decode(const std::string& input, std::vector<uint8_t>& output)
{
    uint32_t val = 0;
    int bits = -8;

    for (unsigned char c : input)
    {
        if (c == '=') break;

        size_t pos = base64_chars.find(c);
        if (pos == std::string::npos)
            continue;

        val = (val << 6) | static_cast<uint32_t>(pos);
        bits += 6;

        if (bits >= 0)
        {
            output.push_back(static_cast<uint8_t>((val >> bits) & 0xFF));
            bits -= 8;
        }
    }
}
std::string base64Encode(const std::vector<uint8_t>& data)
{
    std::string result;
    result.reserve(((data.size() + 2) / 3) * 4);

    for (size_t i = 0; i < data.size(); i += 3) 
    {
        uint32_t triple = static_cast<uint32_t>(data[i]) << 16;

        if (i + 1 < data.size())
            triple |= static_cast<uint32_t>(data[i + 1]) << 8;

        if (i + 2 < data.size())
            triple |= static_cast<uint32_t>(data[i + 2]);

        result += base64_chars[(triple >> 18) & 0x3F];
        result += base64_chars[(triple >> 12) & 0x3F];
        result += (i + 1 < data.size()) ? base64_chars[(triple >> 6) & 0x3F] : '=';
        result += (i + 2 < data.size()) ? base64_chars[triple & 0x3F] : '=';
    }
    return result;
}

void ARW_Export(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) {std::cout << RED << "failed to open file\n" << RESET;}

	std::string line;

	if (std::getline(file, line)) 
	{
		std::vector<uint8_t> data;
		base64Decode(line, data);
		size_t counter = 2;

		uint16_t chunk_count = (uint16_t(data[counter+1]) << 8) | uint16_t(data[counter]);
		counter+=2;

		for(int chunk_index = 0; chunk_index < chunk_count; chunk_index++)
		{
			int chunk_x = (uint16_t(data[counter+1]) << 8) | uint16_t(data[counter]);counter+=2;
			int chunk_y = (uint16_t(data[counter+1]) << 8) | uint16_t(data[counter]);counter+=2;
			uint16_t types_count = data[counter++] + 1;

			for(int i = 0; i < types_count; i++)
			{
				uint8_t type = data[counter++];
				uint16_t arrows_count = data[counter++] + 1;
				for(int j = 0; j < arrows_count; j++)
				{
					uint8_t position = data[counter++];
					uint8_t x = position & 15;
					uint8_t y = position >> 4;
					uint8_t dir_and_flip = data[counter++];
					uint8_t direction = dir_and_flip & 0b011; 
					bool flip = (dir_and_flip & 0b100) != 0;
					AddChunk(chunk_x, -chunk_y, type, y*16+x, direction, flip, false);
				}
			}
		}
	}
	file.close();
}

struct CellType
{
	uint8_t count = 0;
	std::vector<uint16_t> cells; 
};
void ARW_Import(const std::string& filename)
{
	std::vector<uint8_t> data;

	data.push_back(0);
	data.push_back(0);

	uint16_t chunks_size = 0;

	for(int i = 0; i < chunks.size(); i++)
		chunks_size += chunks[i].size();

	data.push_back(uint8_t((chunks_size & 0xFF)));
	data.push_back(uint8_t(((chunks_size >> 8) & 0xFF)));

	int firstX = chunks[0][0]->x;

int firstY = INT_MIN;

for (int x = 0; x < chunks.size(); x++)
{
    if (chunks[x].back()->y > firstY)
        firstY = chunks[x].back()->y;
}
	
	for(int x = 0; x < chunks.size(); x++)
	{
		for(int y = 0; y < chunks[x].size(); y++)
		{
			uint32_t chunkX = chunks[x][y]->x - firstX;
			uint32_t chunkY = firstY - chunks[x][y]->y;
			
			data.push_back(uint8_t((chunkX & 0xFF)));
			data.push_back(uint8_t(((chunkX >> 8) & 0xFF)));
			data.push_back(uint8_t((chunkY & 0xFF)));
			data.push_back(uint8_t(((chunkY >> 8) & 0xFF)));

			CellType types_counter[256] = {0};
			uint8_t types_count = 0;
			
			for(int i = 0; i < 256; i++)
			{
				if(chunks[x][y]->type[i] == 0)
					continue;
				
				if(types_counter[chunks[x][y]->type[i]].count == 0)
				{
					types_count++;
				}
				types_counter[chunks[x][y]->type[i]].count++;
				
				uint8_t pos = i;
				uint8_t dir_flip = chunks[x][y]->direction[i];
				if((chunks[x][y]->flip[i/8] >> (7-i%8)) & 1) {dir_flip |= 0b100;}
				uint16_t cell = dir_flip;
				cell |= pos << 8;
				types_counter[chunks[x][y]->type[i]].cells.push_back(cell);
			}
			data.push_back(types_count-1);
			for(int i = 0; i < 256; i++)
			{
				if(types_counter[i].count == 0)
				continue;
				
				data.push_back(uint8_t(i));
				data.push_back(types_counter[i].count-1);

				for(int j = 0; j < types_counter[i].cells.size(); j++)
				{
						data.push_back(uint8_t(((types_counter[i].cells[j] >> 8) & 0xFF)));
						data.push_back(uint8_t((types_counter[i].cells[j] & 0xFF)));
				}
			}
		}
	}
	
	std::string str = base64Encode(data);
	std::ofstream file(filename, std::ios::trunc);
	file << str;
	file.close();
}

//Just kind of binary search
int32_t FindChunkX(int32_t x)
{
	if (chunks.empty())
		return 0;

	if (x <= chunks[0][0]->x)
		return 0;
		
	int32_t left = 0;
	int32_t right = chunks.size();
		
	while(right > left + 1)
	{
		int32_t midle = left + ((right - left) / 2);
	
		if(x <= chunks[midle][0]->x)
		{
			right = midle;
		}
		else
		{
			left = midle;
		}
	}

	return right;
}

int32_t FindChunkY(int32_t x_index, int32_t y)
{
	if (chunks.empty())
		return 0;

	if (y <= chunks[x_index][0]->y)
		return 0;
		
	int32_t left = 0;
	int32_t right = chunks[x_index].size();
		
	while(right > left + 1)
	{
		int32_t midle = left + ((right - left) / 2);
	
		if(y <= chunks[x_index][midle]->y)
		{
			right = midle;
		}
		else
		{
			left = midle;
		}
	}

	return right;
}
void FindingChunkNeighbours(Chunk* chunk, int32_t x_index, int32_t y_index)
{
//This is should work, probably
	if(y_index !=0)
	{
		if((chunk->y-1) == chunks[x_index][y_index-1]->y)
		{
			chunk->neighbours[7] = chunks[x_index][y_index-1];
			chunks[x_index][y_index-1]->neighbours[1] = chunk;
		}
		
	}
	if(y_index != chunks[x_index].size() - 1)
	{
		if((chunk->y+1) == chunks[x_index][y_index+1]->y)
		{
			chunk->neighbours[1] = chunks[x_index][y_index+1];
			chunks[x_index][y_index+1]->neighbours[7] = chunk;
		}
	}

	int32_t	ny_index;
	if(x_index !=0 && (chunk->x-1) == chunks[x_index-1][0]->x)
	{
		ny_index = FindChunkY(x_index-1, chunk->y-1);

		for(int i = 0; i < 3 && ny_index+i < chunks[x_index-1].size(); i++)
		{
			if(chunk->y-1 == chunks[x_index-1][ny_index+i]->y)
			{
				chunk->neighbours[6] = chunks[x_index-1][ny_index+i];
				chunks[x_index-1][ny_index+i]->neighbours[2] = chunk;
			}
			if(chunk->y   == chunks[x_index-1][ny_index+i]->y)
			{
				chunk->neighbours[3] = chunks[x_index-1][ny_index+i];
				chunks[x_index-1][ny_index+i]->neighbours[5] = chunk;
			}
			if(chunk->y+1 == chunks[x_index-1][ny_index+i]->y)
			{
				chunk->neighbours[0] = chunks[x_index-1][ny_index+i];
				chunks[x_index-1][ny_index+i]->neighbours[8] = chunk;
			}
		}
	}
	if(x_index != chunks.size() - 1 && (chunk->x+1) == chunks[x_index+1][0]->x)
	{
		ny_index = FindChunkY(x_index+1, chunk->y-1);

		for(int i = 0; i < 3 && ny_index+i < chunks[x_index+1].size(); i++)
		{
			if(chunk->y-1 == chunks[x_index+1][ny_index+i]->y)
			{
				chunk->neighbours[2] = chunks[x_index+1][ny_index+i];
				chunks[x_index+1][ny_index+i]->neighbours[6] = chunk;
			}
			if(chunk->y   == chunks[x_index+1][ny_index+i]->y)
			{
				chunk->neighbours[5] = chunks[x_index+1][ny_index+i];
				chunks[x_index+1][ny_index+i]->neighbours[3] = chunk;
			}
			if(chunk->y+1 == chunks[x_index+1][ny_index+i]->y)
			{
				chunk->neighbours[8] = chunks[x_index+1][ny_index+i];
				chunks[x_index+1][ny_index+i]->neighbours[0] = chunk;
			}
		}
	}
}
void AddChunk(int32_t x, int32_t y, uint8_t type, uint8_t position, uint8_t direction, bool flip, bool undo)
{
//This function adds and removes chunks and arrows inside
	if(undobuffer.empty())
 		undobuffer.emplace_back();

	if(chunks.empty())
	{
		if(type != 0)
		{
			chunks.push_back({new Chunk(x,y,type,position,direction,flip)});
			if(undo)undobuffer.back().push_back(new UndoCell(x,y,position,0, 0, 0));
		}
		return;
	}
	int32_t x_index = FindChunkX(x);
	if(x_index == chunks.size())
	{
		if(type!=0)
		{
			chunks.push_back({new Chunk(x,y,type,position,direction,flip)});
			if(undo)undobuffer.back().push_back(new UndoCell(x,y,position, 0, 0, 0));
			FindingChunkNeighbours(chunks.back()[0], x_index, 0);
		}
		return;
	}
	if(chunks[x_index][0]->x != x)
	{	
		if(type!=0)
		{
			chunks.insert(chunks.begin() + x_index, {new Chunk(x,y,type,position,direction,flip)});
			FindingChunkNeighbours(chunks[x_index][0], x_index, 0);
			if(undo)undobuffer.back().push_back(new UndoCell(x,y,position, 0, 0, 0));
		}
		return;
	}
	else
	{
		int32_t y_index = FindChunkY(x_index, y);
		if(y_index == chunks[x_index].size())
		{
			if(type!=0)
			{
				chunks[x_index].push_back(new Chunk(x,y,type,position,direction,flip));
				if(undo)undobuffer.back().push_back(new UndoCell(x,y,position, 0, 0, 0));
				FindingChunkNeighbours(chunks[x_index].back(), x_index, y_index);
			}
			return;
		}
		if(chunks[x_index][y_index]->y != y)
		{
			if(type!=0)
			{
				chunks[x_index].insert(chunks[x_index].begin() + y_index, new Chunk(x,y,type,position,direction,flip));
				if(undo)undobuffer.back().push_back(new UndoCell(x,y,position, 0, 0, 0));
				FindingChunkNeighbours(chunks[x_index][y_index], x_index, y_index);
			}
			return;
		}
		else
		{
			//PlaceArrows returns true or false depending on are there arrows in the current chunk or not
			if(chunks[x_index][y_index]->type[position] != type || chunks[x_index][y_index]->direction[position] != direction || ((chunks[x_index][y_index]->flip[position/8] >> (7-position%8)) & 1) != flip)
			{
				if(undo)undobuffer.back().push_back(new UndoCell(x,y,position, chunks[x_index][y_index]->type[position],chunks[x_index][y_index]->direction[position], ((chunks[x_index][y_index]->flip[position/8] >> (7-position%8)) & 1)) );
			}
			if(chunks[x_index][y_index]->PlaceArrow(type, position, direction, flip))
			{
				delete chunks[x_index][y_index];
				chunks[x_index].erase(chunks[x_index].begin() + y_index);
				if (chunks[x_index].empty())
				{	
				    chunks.erase(chunks.begin() + x_index);
				}
			}
			return;
		}
	}
}
void FillRenderArray(int x1, int x2, int y1, int y2)
{
	chunks_for_render.clear();		
	if(chunks.empty())
	{
		return;
	}
	int x_index = FindChunkX(x1);
	
	for(int i = x_index; i < chunks.size() && chunks[i][0]->x <= x2; i++)
	{
		int y_index = FindChunkY(i, y1);

		for(int j = y_index; j < chunks[i].size() && chunks[i][j]->y <= y2; j++)
		{
		chunks_for_render.push_back(chunks[i][j]);
		}
	}
}
