#include <thread>
#include <chrono>
#include "common.h"
#include "arrows.h"
#include "chunk.h"
#include "logic.h"
#include "graphics.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"

/*
void CommandsDebug()
{
	for(int i = 0; i < chunkCommands.size(); i++)
	{
		std::cout << std::endl << "x: " << chunkCommands[i].chunk->x << " y: " << chunkCommands[i].chunk->y << " | s: " <<
		 chunkCommands[i].start << " c: " << chunkCommands[i].count << std::endl << std::endl;
	}
	for (int i = 0; i < commands.size(); i++)
	{
	    uint16_t value = commands[i];

	    for (int bit = 15; bit >= 0; bit--)
 	    {
	        std::cout << ((value >> bit) & 1);
	    }

	    std::cout << std::endl << std::endl;
	}
}
*/
void ChunkDebug(const Chunk* chunk)
{
	for(int y = 0; y < 16; y++)
	{
		for(int x = 0; x < 16; x++)
		{
			if(chunk->type[16*y+x] != 0)
			{
			
			int _direction = (chunk->direction[(y*16+x)/4] >> (3-(y*16+x)%4)*2) & 3;
			char arrow;
			
			switch(_direction)
			{
				case 0:
					arrow = '^';
				break;
				case 1:
					arrow = '>';
				break;
				case 2:
					arrow = 'v';
				break;
				case 3:
					arrow = '<';
				break;
				default:
					arrow = '?';
			}
			
				if(((chunk->state[(16*y+x) / 4] >> (3 - ((16*y+x) % 4)) * 2)& 0b11) >= 1)
					std::cout << RED << arrow << RESET;
				else
					std::cout << arrow;
			}
			else
			{
				if(((chunk->state[(16*y+x) / 4] >> (3 - ((16*y+x) % 4)) * 2)& 0b11) >= 1)
				std::cout << RED << '0' << RESET;
				else
				std::cout << '0';
			}
		}
		
		if(chunk->neighbours[y] != nullptr && y < 8)
			std::cout << " " << chunk->neighbours[y] << " " << chunk->neighbours[y]->x << " " << chunk->neighbours[y]->y << std::endl;
		else if(y < 8)
			std::cout << " nullptr" << std::endl;
		else
			std::cout << std::endl;
	
	}

}

void PlaceDebug(const Chunk* chunk)
{
	for(int y = 0; y < 16; y++)
	{
		for(int x = 0; x < 16; x++)
		{
			if(chunk->type[16*y+x] != 0)
			{
			
			int _direction = (chunk->direction[(y*16+x)/4] >> (3-(y*16+x)%4)*2) & 3;
			char arrow;
			
			switch(_direction)
			{
				case 0:
					arrow = '^';
				break;
				case 1:
					arrow = '>';
				break;
				case 2:
					arrow = 'v';
				break;
				case 3:
					arrow = '<';
				break;
				default:
					arrow = '?';
			}
			
				if(((chunk->state[(16*y+x) / 4] >> (3 - ((16*y+x) % 4)) * 2)& 0b11) >= 1)
					std::cout << RED << arrow << RESET;
				else
					std::cout << arrow;
			}
			else
			{
				if(((chunk->state[(16*y+x) / 4] >> (3 - ((16*y+x) % 4)) * 2)& 0b11) >= 1)
				std::cout << RED << '0' << RESET;
				else
				std::cout << '0';
			}
		}


		for(int x = 0; x < 16; x++)
		{
			if(chunk->neighbours[4]->type[16*y+x] != 0)
			{
			
			int _direction = (chunk->neighbours[4]->direction[(y*16+x)/4] >> (3-(y*16+x)%4)*2) & 3;
			char arrow;
			
			switch(_direction)
			{
				case 0:
					arrow = '^';
				break;
				case 1:
					arrow = '>';
				break;
				case 2:
					arrow = 'v';
				break;
				case 3:
					arrow = '<';
				break;
				default:
					arrow = '?';
			}
			
				if(((chunk->neighbours[4]->state[(16*y+x) / 4] >> (3 - ((16*y+x) % 4)) * 2)& 0b11) >= 1)
					std::cout << RED << arrow << RESET;
				else
					std::cout << arrow;
			}
			else
			{
				if(((chunk->neighbours[4]->state[(16*y+x) / 4] >> (3 - ((16*y+x) % 4)) * 2)& 0b11) >= 1)
				std::cout << RED << '0' << RESET;
				else
				std::cout << '0';
			}
		}
	std::cout << std::endl;
	}

	for(int y = 0; y < 16; y++)
	{
		for(int x = 0; x < 16; x++)
		{
			if(chunk->neighbours[6]->type[16*y+x] != 0)
			{
			
			int _direction = (chunk->neighbours[6]->direction[(y*16+x)/4] >> (3-(y*16+x)%4)*2) & 3;
			char arrow;
			
			switch(_direction)
			{
				case 0:
					arrow = '^';
				break;
				case 1:
					arrow = '>';
				break;
				case 2:
					arrow = 'v';
				break;
				case 3:
					arrow = '<';
				break;
				default:
					arrow = '?';
			}
			
				if(((chunk->neighbours[6]->state[(16*y+x) / 4] >> (3 - ((16*y+x) % 4)) * 2)& 0b11) >= 1)
					std::cout << RED << arrow << RESET;
				else
					std::cout << arrow;
			}
			else
			{
				if(((chunk->neighbours[6]->state[(16*y+x) / 4] >> (3 - ((16*y+x) % 4)) * 2)& 0b11) >= 1)
				std::cout << RED << '0' << RESET;
				else
				std::cout << '0';
			}
		}


		for(int x = 0; x < 16; x++)
		{
			if(chunk->neighbours[7]->type[16*y+x] != 0)
			{
			
			int _direction = (chunk->neighbours[7]->direction[(y*16+x)/4] >> (3-(y*16+x)%4)*2) & 3;
			char arrow;
			
			switch(_direction)
			{
				case 0:
					arrow = '^';
				break;
				case 1:
					arrow = '>';
				break;
				case 2:
					arrow = 'v';
				break;
				case 3:
					arrow = '<';
				break;
				default:
					arrow = '?';
			}
			
				if(((chunk->neighbours[7]->state[(16*y+x) / 4] >> (3 - ((16*y+x) % 4)) * 2)& 0b11) >= 1)
					std::cout << RED << arrow << RESET;
				else
					std::cout << arrow;
			}
			else
			{
				if(((chunk->neighbours[7]->state[(16*y+x) / 4] >> (3 - ((16*y+x) % 4)) * 2)& 0b11) >= 1)
				std::cout << RED << '0' << RESET;
				else
				std::cout << '0';
			}
		}
	std::cout << std::endl;
	}
}


int main()
{
Camera* camera = new Camera;
GraphicsHandler graphics(camera);

//AddChunk(chunks, 0, 0, 0, 0, 1, 0);
AddChunk(chunks, 1, 0, 1, 0, 1, 0);
AddChunk(chunks, 1, 1, 1, 0, 1, 0);
AddChunk(chunks, 0, 1, 1, 0, 1, 0);
AddChunk(chunks, 2, 1, 1, 0, 1, 0);


//rchunks[1][0]->AddState(1, 36);
//chunks[1][1]->SetState(1, 208);

//chunks[1][0]->PlaceArrow(1, 48, 3,1);
/*chunks[1][0]->PlaceArrow(1, 49, 3,1);
chunks[1][0]->PlaceArrow(1, 50, 3,1);
chunks[1][0]->PlaceArrow(1, 51, 3,0);
chunks[1][0]->PlaceArrow(1, 52, 3,0);
chunks[0][0]->PlaceArrow(1, 57, 3,0);
chunks[0][0]->PlaceArrow(1, 58, 3,0);
chunks[0][0]->PlaceArrow(1, 59, 3,0);
chunks[0][0]->PlaceArrow(1, 60, 3,0);
chunks[0][0]->PlaceArrow(1, 61, 3,0);
chunks[0][0]->PlaceArrow(1, 62, 3,0);
chunks[0][0]->PlaceArrow(1, 63, 3,0);
chunks[0][0]->PlaceArrow(1, 56, 0,0);
chunks[0][0]->PlaceArrow(1, 40, 0,0);
chunks[0][0]->PlaceArrow(1, 24, 0,0);
chunks[0][0]->PlaceArrow(1, 8, 0,0);
chunks[0][1]->PlaceArrow(1, 248, 0,0);
chunks[0][1]->PlaceArrow(1, 232, 0,0);
chunks[0][1]->PlaceArrow(1, 216, 1,0);
chunks[0][1]->PlaceArrow(1, 217, 1,0);
chunks[0][1]->PlaceArrow(1, 218, 1,0);
chunks[0][1]->PlaceArrow(1, 219, 1,0);
chunks[0][1]->PlaceArrow(1, 220, 1,0);
chunks[0][1]->PlaceArrow(1, 221, 1,0);
chunks[0][1]->PlaceArrow(1, 222, 1,0);
chunks[0][1]->PlaceArrow(1, 223, 1,0);
chunks[1][1]->PlaceArrow(1, 208, 1,0);
chunks[1][1]->PlaceArrow(1, 209, 1,0);
chunks[1][1]->PlaceArrow(1, 210, 1,0);
chunks[1][1]->PlaceArrow(1, 211, 1,0);
chunks[1][1]->PlaceArrow(1, 212, 2,0);
chunks[1][1]->PlaceArrow(1, 228, 2,0);
chunks[1][1]->PlaceArrow(1, 244, 2,0);
chunks[1][0]->PlaceArrow(1, 4, 2,0);
chunks[1][0]->PlaceArrow(1, 20, 2,0);
chunks[1][0]->PlaceArrow(1, 36, 2,0); 

chunks[1][0]->PlaceArrow(1, 0, 2,0); 
chunks[1][0]->PlaceArrow(1, 255, 2,0); 
chunks[1][0]->PlaceArrow(1, 240, 2,0); 
chunks[1][0]->PlaceArrow(1, 15, 2,0); */
//chunks[0][0]->PlaceArrow(2, 15, 0);
//chunks[0][1]->PlaceArrow(1, 255, 1);
//chunks[1][0]->PlaceArrow(1, 0, 3);
//chunks[1][1]->PlaceArrow(1, 240, 2);

//int width, height;
//glfwGetFramebufferSize(graphics.window, &width, &height);
//camera.GetProjection(width,height);
//camera->GetProjection(width,height);

using Clock = std::chrono::high_resolution_clock;
glfwSwapInterval(1);
auto timer = std::chrono::steady_clock::now();
glClearColor(0.5, 0.5, 0.5, 1.0f);

int tick = 0;

while(!glfwWindowShouldClose(graphics.window)) //MAIN LOOP
{
auto start = Clock::now();

auto time_now = std::chrono::steady_clock::now();
auto time_left = std::chrono::duration_cast<std::chrono::seconds>(time_now - timer);

//HERE IS A LOGIC

if(time_left.count() >= 1)//seconds
{

for(uint32_t x = 0; x < chunks.size(); x++)
{
	std::cout << x << ":";
	for(uint32_t y = 0; y < chunks[x].size(); y++)
	{
		std::cout << "|" << y;
		chunks[x][y]->TeakProcessing();
	}
	 std::cout << std::endl;
}
	 std::cout << std::endl;
//rPlaceDebug(chunks[0][1]);
timer = std::chrono::steady_clock::now();
}

//HERE IS RENDERER

if(tick == 0)
{
glClear(GL_COLOR_BUFFER_BIT);
for(uint32_t x = 0; x < chunks.size(); x++)
{
	for(uint32_t y = 0; y < chunks[x].size(); y++)
	{
		chunks[x][y]->mesh->Render(camera->ortho, graphics);
	}
}	
tick = 0;
}
//tick++;

//HERE IS SWAPS

if(time_left.count() >= 1)
{
	for(uint32_t x = 0; x < chunks.size(); x++)
	{
		for(uint32_t y = 0; y < chunks[x].size(); y++)
		{
			chunks[x][y]->UpdateMesh();
			chunks[x][y]->SwapBuffers();
		}
	}
}

glfwSwapBuffers(graphics.window);
glfwPollEvents();

std::this_thread::sleep_for(std::chrono::milliseconds(1));


auto end = Clock::now();
//std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
//          << " us\n"; 

}


return 0;
}
