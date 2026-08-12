#include <thread>
#include <chrono>
#include "common.h"
#include "chunk.h"
#include "logic.h"
#include "graphics.h"

bool fileExists(const std::string& filename) 
{
    std::ifstream file(filename);
    return file.good();
}

int main(int argc, char* argv[])
{

if(argc != 2){std::cout << BOLD << RED << "incorrect arguments" << std::endl;return 1;}
filename = argv[1];

Camera* camera = new Camera;
GraphicsHandler* graphics = new GraphicsHandler(camera);
Background* background = new Background(*graphics);
camera->InitGraphics();

if (fileExists(filename)) 
{
	ARW_Export(filename);
}


using Clock = std::chrono::high_resolution_clock;
glfwSwapInterval(0);

auto last_time = Clock::now();

auto logic_timer = last_time;
auto render_timer = last_time;
auto undo_timer = last_time;
auto tps_timer = last_time;

int tick = 0;

//- - - - - - - - - - - - - - - - -//
//		      MAIN LOOP    	       //
//- - - - - - - - - - - - - - - - -//

while(!glfwWindowShouldClose(graphics->window))
{

if(camera->delay != 0)
std::this_thread::sleep_for(std::chrono::milliseconds(1));

auto now = Clock::now();

//HERE IS A LOGIC

if(now - undo_timer >= std::chrono::seconds(2))
{
	camera->newundo = true;
	undo_timer += std::chrono::seconds(2);
}

if(now - logic_timer >= std::chrono::milliseconds(camera->delay) && !camera->stop)
{

	for(uint32_t x = 0; x < chunks.size(); x++)
	{
		for(uint32_t y = 0; y < chunks[x].size(); y++)
		{
			if(chunks[x][y]->dirt)
				chunks[x][y]->TeakProcessing();
		}
	}
	for(uint32_t x = 0; x < chunks.size(); x++)
	{
		for(uint32_t y = 0; y < chunks[x].size(); y++)
		{
			chunks[x][y]->SwapBuffers();
			chunks[x][y]->UpdateDirtness();
		}
	}
	logic_timer += std::chrono::milliseconds(camera->delay);
	tick++;
}

if(now - tps_timer >= std::chrono::seconds(1))
{
	std::cout << BOLD << "TPS: " << tick << std::endl;
	tps_timer += std::chrono::seconds(1);
	tick=0;
}

//HERE IS A RENDERER

if(now - render_timer >= std::chrono::milliseconds(20)) //50 target FPS
{
	glClear(GL_COLOR_BUFFER_BIT);
	background->Render(*camera, *graphics);

	for(uint32_t i = 0; i < chunks_for_render.size(); i++)
	{
		chunks_for_render[i]->mesh->Render(camera->ortho, *graphics);
	}
	camera->RenderGraphics(*graphics);
	glfwSwapBuffers(graphics->window);
	render_timer += std::chrono::milliseconds(20);
}

glfwPollEvents();
}
return 0;
}
