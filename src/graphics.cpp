/*
This file intended for different needs depending on the graphics or mouse and keyboard used
*/

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "graphics.h"
#include "chunk.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	auto* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	camera->UpdateWindowSize(window);
	glViewport(0, 0, width, height);
}

bool MiddleMousePressed = false;
bool LeftMousePressed = false;
bool RPressed = false;
bool EPressed = false;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

    if(key == GLFW_KEY_W && action == GLFW_PRESS)
    {
		camera->brush.SetState(camera->brush.type, 0, camera->brush.flip, camera->brush.page, camera->brush.paste);
    }
    if(key == GLFW_KEY_D && action == GLFW_PRESS)
    {
		camera->brush.SetState(camera->brush.type, 1, camera->brush.flip, camera->brush.page, camera->brush.paste);
    }
    if(key == GLFW_KEY_S && action == GLFW_PRESS)
    {
		camera->brush.SetState(camera->brush.type, 2, camera->brush.flip, camera->brush.page, camera->brush.paste);
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS && (mods & GLFW_MOD_CONTROL))
    {
        camera->Save();
    }
    if(key == GLFW_KEY_A && action == GLFW_PRESS)
    {
		camera->brush.SetState(camera->brush.type, 3, camera->brush.flip, camera->brush.page, camera->brush.paste);
    }
    if(key == GLFW_KEY_C && action == GLFW_PRESS)
    {
    	if(camera->selection)
		camera->CopySelection();
		camera->selection = false;
    }
    if(key == GLFW_KEY_X && action == GLFW_PRESS)
    {
        if(camera->selection)
		camera->CutSelection();
		camera->UpdateRenderArray();
    }
    if(key == GLFW_KEY_V && action == GLFW_PRESS)
    {
		camera->brush.SetState(0, camera->brush.direction, camera->brush.flip, 0, true);
    }
    if(key == GLFW_KEY_R && action == GLFW_PRESS)
    {
		RPressed = true;
		camera->brush.Draw(*camera,true);
		camera->UpdateRenderArray();
    }
    if(key == GLFW_KEY_R && action == GLFW_RELEASE)
    {
    	RPressed = false;
    }
    if(key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
		camera->Dropper(xpos, ypos);
    }
    if(key == GLFW_KEY_E && action == GLFW_PRESS)
    {
    	EPressed = true;
		camera->SelectionStarted();
    }    
    if(key == GLFW_KEY_E && action == GLFW_RELEASE)
    {
    	EPressed = false;
		camera->SelectionCompleted();
    }
    if(key == GLFW_KEY_COMMA && action == GLFW_PRESS)
    {
    	if(camera->delay > 0)
    	camera->delay -= 10;

    	std::cout << BOLD << YELLOW << "The delay between ticks is: " << WHITE << camera->delay << RESET << std::endl;
    }
    if(key == GLFW_KEY_PERIOD && action == GLFW_PRESS) 
    {
        if(camera->delay < 100)
    	camera->delay += 10;

    	std::cout << BOLD << YELLOW << "The delay between ticks is: " << WHITE << camera->delay << RESET << std::endl;
    }
    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) 
    {
    	camera->stop = !camera->stop;
    }
    if(key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
    	camera->Undo();
    	camera->UpdateRenderArray();
    }
    if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
    {
		camera->DeleteSelection();
		camera->UpdateRenderArray();
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
		camera->brush.SetState(camera->brush.type, camera->brush.direction, !camera->brush.flip, camera->brush.page, camera->brush.paste);
    }
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		camera->brush.SetState(1, camera->brush.direction, camera->brush.flip, camera->brush.page, false);
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		camera->brush.SetState(2, camera->brush.direction, camera->brush.flip, camera->brush.page, false);
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
		camera->brush.SetState(3, camera->brush.direction, camera->brush.flip, camera->brush.page, false);
    }
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
		camera->brush.SetState(4, camera->brush.direction, camera->brush.flip, camera->brush.page, false);
    }
    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
		camera->brush.SetState(5, camera->brush.direction, camera->brush.flip, camera->brush.page, false);
    }
    if (key == GLFW_KEY_MINUS && action == GLFW_PRESS)
    {
		if(camera->brush.page > 0)
			camera->brush.SetState(camera->brush.type, camera->brush.direction, camera->brush.flip, camera->brush.page-1, camera->brush.paste);
    }
    if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS)
    {
    	if(camera->brush.page < 4)
			camera->brush.SetState(camera->brush.type, camera->brush.direction, camera->brush.flip, camera->brush.page+1, camera->brush.paste);
    }
    if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS)
    {
		camera->brush.SetState(0, 0, 0, 0, false);
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	auto* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	
	if(action == GLFW_PRESS)
	{
        if(button == GLFW_MOUSE_BUTTON_LEFT)
        {
            camera->CheckButtonArrows(xpos, ypos);
        	LeftMousePressed = true;
			camera->brush.Draw(*camera,false);
           	camera->UpdateRenderArray();
		}
		if(button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			
		}
        if(button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
         	MiddleMousePressed = true;
            camera->mouse_position = {xpos, ypos};
            camera->old_position = camera->position;
		}
    }

	if(action == GLFW_RELEASE)
	{
        if (button == GLFW_MOUSE_BUTTON_LEFT)
 			LeftMousePressed = false;
        if (button == GLFW_MOUSE_BUTTON_MIDDLE)
			MiddleMousePressed = false;
	}
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	auto* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	camera->CalculateMousePosInTiles(xpos, ypos);
    if (MiddleMousePressed)
    {
    	camera->UpdateWindowSize(window);
		camera->MoveCamera(xpos, ypos);
    }
    if(LeftMousePressed)
    {
		camera->brush.Draw(*camera,false);
    	camera->UpdateRenderArray();
    }
    if(RPressed)
    {
		camera->brush.Draw(*camera,true);
    	camera->UpdateRenderArray();
    }
    if(EPressed)
    {
    	camera->SelectionGoing();
    }
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	auto* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	
    if (yoffset > 0 && camera->zoom > 0.2f)
        camera->zoom-=0.1f;

    if (yoffset < 0 && camera->zoom < 10)
        camera->zoom+=0.1f;

    camera->UpdateWindowSize(window);
}

ChunkMesh::ChunkMesh()
	: VAO(0), VBO(0), EBO(0)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 256 * 4 * sizeof(Point), nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 256 * 6 * sizeof(unsigned int), nullptr, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)(offsetof(Point, u)));
	glEnableVertexAttribArray(1);

	int index = 0;

	for (int i = 0; i < 256; i++)
	{
	    int offset = i * 4;
	
	    indices[index++] = 0 + offset;
	    indices[index++] = 1 + offset;
	    indices[index++] = 3 + offset;
	
	    indices[index++] = 0 + offset;
	    indices[index++] = 2 + offset;
	    indices[index++] = 3 + offset;
	}

	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,256 * 6 * sizeof(unsigned int),indices);
}

ChunkMesh::~ChunkMesh()
{
    if (EBO != 0)
        glDeleteBuffers(1, &EBO);

    if (VBO != 0)
        glDeleteBuffers(1, &VBO);

    if (VAO != 0)
        glDeleteVertexArrays(1, &VAO);
}

void ChunkMesh::ChangeState(Chunk* chunk, uint8_t position, bool state)
{
   	int base = position * 4;
   	float offset = state ? -0.5f : 0.5f;
    	
   	if((!state && vertices[base].v <= 0.5f && vertices[base + 3].v <= 0.5f) || (state && vertices[base].v >= 0.5f && vertices[base + 3].v >= 0.5f))
   	{
   	    for (int i = 0; i < 4; ++i)
   	        vertices[base + i].v += offset;
   	}
}
void ChunkMesh::CalculateCellMesh(Chunk* chunk, uint8_t position, uint8_t _direction, bool _flip)
{
	if(chunk->type[position] > 0)
	{
		int x = position % 16;
		int y = position / 16;
		int uvx = (chunk->type[position] -1) % 8;
		int uvy = (chunk->type[position] -1) / 8;
		float px = chunk->x * 16 + x;
   		float py = chunk->y * 16 - y;
		float u0 = uvx * (1 / 8.0f);
		float v0 = (1 - (1 / 8.0f) * uvy) - (1 / 8.0f);
		float u1 = (uvx * (1 / 8.0f)) + (1 / 8.0f);
		float v1 = 1 - (1 / 8.0f) * uvy;
		float U[4] = {u0,u1,u0,u1};
		float V[4] = {v1,v1,v0,v0};
		
		constexpr uint8_t uv[2][4][4] =
		{
			{
				{0,1,2,3},
				{2,0,3,1},
				{3,2,1,0},
				{1,3,0,2},
			},
			{
				{1,0,3,2},
				{3,1,2,0},
				{2,3,0,1},
				{0,2,1,3},
			}
		};
		vertices[position*4] =   Point(px,  py+1, 0, U[uv[_flip][_direction][0]], V[uv[_flip][_direction][0]]);
    	vertices[position*4+1] = Point(px+1,py+1, 0, U[uv[_flip][_direction][1]], V[uv[_flip][_direction][1]]);
    	vertices[position*4+2] = Point(px,  py,   0, U[uv[_flip][_direction][2]], V[uv[_flip][_direction][2]]);
    	vertices[position*4+3] = Point(px+1,py,   0, U[uv[_flip][_direction][3]], V[uv[_flip][_direction][3]]);
	}
	else
	{
		vertices[position*4]= Point();
		vertices[position*4+1]= Point();
		vertices[position*4+2]= Point();
		vertices[position*4+3]= Point();
	}
	this->Update();
	
}
void BrushMesh::CalculateBrushMesh(Brush* brush)
{
	vertices.clear();
	if(!brush->paste)
	{
		uint8_t type = brush->page * 5 + brush->type;
		if(type > 0)
		{
			int uvx = (type -1) % 8;
			int uvy = (type -1) / 8;
			float px = 0;
   			float py = 0;
			float u0 = uvx * (1 / 8.0f);
			float v0 = (1 - (1 / 8.0f) * uvy) - (1 / 8.0f);
			float u1 = (uvx * (1 / 8.0f)) + (1 / 8.0f);
			float v1 = 1 - (1 / 8.0f) * uvy;
			float U[4] = {u0,u1,u0,u1};
			float V[4] = {v1,v1,v0,v0};
		
			constexpr uint8_t uv[2][4][4] =
			{
				{
					{0,1,2,3},
					{2,0,3,1},
					{3,2,1,0},
					{1,3,0,2},
				},
				{
					{1,0,3,2},
					{3,1,2,0},
					{2,3,0,1},
					{0,2,1,3},
				}
			};
			uint8_t d = brush->direction;
			uint8_t f = brush->flip;
			vertices.push_back(Point(px,  py+1, 0, U[uv[f][d][0]], V[uv[f][d][0]]));
	 		vertices.push_back(Point(px+1,py+1, 0, U[uv[f][d][1]], V[uv[f][d][1]]));
			vertices.push_back(Point(px,  py,   0, U[uv[f][d][2]], V[uv[f][d][2]]));
		 	vertices.push_back(Point(px+1,py,   0, U[uv[f][d][3]], V[uv[f][d][3]]));
		}
		else
		{
			vertices.push_back(Point());
			vertices.push_back(Point());
			vertices.push_back(Point());
			vertices.push_back(Point());
		}
	}
	else
	{
		for(uint32_t i = 0; i < brush->data.size(); i++)
	 	{
			uint8_t type = brush->data[i]->type;
			int uvx = (type -1) % 8;
			int uvy = (type -1) / 8;
			
			float px = brush->data[i]->x;
			float py = brush->data[i]->y;
			uint8_t f = brush->data[i]->flip;
			uint8_t d = brush->data[i]->direction;
		
   			if(brush->flip)
			{
				px = -px;
				f ^= 1;
				d = (4 - d) & 3;
			}
			d = (d + brush->direction) & 3;
			for(int r = 0; r < brush->direction; r++)
			{
				float oldpx = px;
				float oldpy = py;

				px = oldpy;
				py = -oldpx;
			}
   			
			float u0 = uvx * (1 / 8.0f);
			float v0 = (1 - (1 / 8.0f) * uvy) - (1 / 8.0f);
			float u1 = (uvx * (1 / 8.0f)) + (1 / 8.0f);
			float v1 = 1 - (1 / 8.0f) * uvy;
			float U[4] = {u0,u1,u0,u1};
			float V[4] = {v1,v1,v0,v0};
			
			constexpr uint8_t uv[2][4][4] =
			{
				{
					{0,1,2,3},
					{2,0,3,1},
					{3,2,1,0},
					{1,3,0,2},
				},
				{
					{1,0,3,2},
					{3,1,2,0},
					{2,3,0,1},
					{0,2,1,3},
				}
			};

			vertices.push_back(Point(px,  py+1, 0, U[uv[f][d][0]], V[uv[f][d][0]]));
	 		vertices.push_back(Point(px+1,py+1, 0, U[uv[f][d][1]], V[uv[f][d][1]]));
			vertices.push_back(Point(px,  py,   0, U[uv[f][d][2]], V[uv[f][d][2]]));
			vertices.push_back(Point(px+1,py,   0, U[uv[f][d][3]], V[uv[f][d][3]]));
		}
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Point), vertices.data(), GL_DYNAMIC_DRAW);

	indices.clear();

	for (size_t i = 0; i < vertices.size() / 4; ++i)
	{
	    unsigned int offset = i * 4;
	
	    indices.push_back(offset + 0);
	    indices.push_back(offset + 1);
	    indices.push_back(offset + 3);
	
	    indices.push_back(offset + 0);
	    indices.push_back(offset + 2);
	    indices.push_back(offset + 3);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}


void ChunkMesh::Update()
{
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 256 * 4 * sizeof(Point), vertices);
	
	glBindVertexArray(0);
}
void ChunkMesh::Render(glm::mat4 ortho, GraphicsHandler& gh)
{
	glUseProgram(gh.chunk_shaderProgram);

	glUniform1i(gh.chunk_texLoc, 0);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gh.arrows_texture);
	
	glUniformMatrix4fv(gh.chunk_orthoLoc, 1, GL_FALSE, glm::value_ptr(ortho));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 256 * 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

BrushMesh::BrushMesh(Brush& brush)
	: VAO(0), VBO(0), EBO(0)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)(offsetof(Point, u)));
	glEnableVertexAttribArray(1);
}
BrushMesh::~BrushMesh()
{
    if (EBO != 0)
        glDeleteBuffers(1, &EBO);

    if (VBO != 0)
        glDeleteBuffers(1, &VBO);

    if (VAO != 0)
        glDeleteVertexArrays(1, &VAO);
}

SelectionMesh::SelectionMesh()
: VAO(0), VBO(0), EBO(0)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Point), nullptr, GL_DYNAMIC_DRAW);	

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), nullptr, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)0);
	glEnableVertexAttribArray(0);

	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6 * sizeof(unsigned int),indices);
}

void SelectionMesh::SetSize(int x1, int x2, int y1, int y2)
{
	vertices[0] = Point(x1, y1, 0,0,0);
	vertices[1] = Point(x1, y2, 0,0,1);
	vertices[2] = Point(x2, y2, 0,1,1);
	vertices[3] = Point(x2, y1, 0,1,0);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(Point), vertices);
}

void SelectionMesh::Render(Camera& camera, GraphicsHandler& gh)
{
	glUseProgram(gh.chunk_shaderProgram);
	glUniform1i(gh.chunk_texLoc, 0);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gh.selection_texture);
	
	glUniformMatrix4fv(gh.chunk_orthoLoc, 1, GL_FALSE, glm::value_ptr(camera.ortho));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 256 * 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Background::Background(GraphicsHandler& gh)
: VAO(0), VBO(0), EBO(0)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Point), nullptr, GL_DYNAMIC_DRAW);	

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), nullptr, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)0);
	glEnableVertexAttribArray(0);

	vertices[0] = Point(-500, -300, 0);
	vertices[1] = Point(-500,  300, 0);
	vertices[2] = Point( 500,  300, 0);
	vertices[3] = Point( 500, -300, 0);

	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6 * sizeof(unsigned int),indices);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(Point), vertices);
}

void Background::Render(Camera& camera, GraphicsHandler& gh)
{
	glUseProgram(gh.net_shaderProgram);
	glUniformMatrix4fv(gh.net_orthoLoc, 1, GL_FALSE, glm::value_ptr(camera.ortho));
	glUniform1f(gh.net_zoomLoc, camera.zoom);
	glUniform2f(gh.net_cameraPosLoc, camera.position.x, camera.position.y);
	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
void Brush::SetState(uint8_t t, uint8_t d, bool f, uint8_t p, bool pst)
{
	type = t;
	direction = d;
	flip = f;
	page = p;
	paste = pst;
	mesh->CalculateBrushMesh(this);
}

void Brush::Draw(Camera& cam, bool remove)
{
	if(paste)
	{
		for(uint32_t i = 0; i < data.size(); i++)
	 	{
	 		uint8_t type = data[i]->type;
			float px = data[i]->x;
			float py = data[i]->y;
			uint8_t f = data[i]->flip;
			uint8_t d = data[i]->direction;
			
   			if(flip)
			{
				px = -px;
				f ^= 1;
				d = (4 - d) & 3;
			}
			d = (d + direction) & 3;
			for(int r = 0; r < direction; r++)
			{
				float oldpx = px;
				float oldpy = py;

				px = oldpy;
				py = -oldpx;
			}

	  		int tmptileX = cam.tileX + px;
			int tmptileY = cam.tileY + py;
			int ChunkX = tmptileX / 16;
			int ChunkY = tmptileY / 16;

			if(tmptileX < 0)
			{
				ChunkX = ((tmptileX-15) / 16);
				tmptileX = 16 + (tmptileX % 16);
			}
			if(tmptileY > 0)
			{
				ChunkY = ((tmptileY+15) / 16);
				tmptileY = 16 - (tmptileY % 16);
			}
			if(cam.newundo){undobuffer.emplace_back(); cam.newundo = false;}
			AddChunk(ChunkX, ChunkY, type, abs(tmptileY%16) * 16 + abs(tmptileX%16), d, f, true);
		}
	}
	else
	{
		if(type == 0 && !remove)
			return;
		int tmptileX = cam.tileX;
		int tmptileY = cam.tileY;
		int ChunkX = tmptileX / 16;
		int ChunkY = tmptileY / 16;

		if(tmptileX < 0)
		{
			ChunkX = ((tmptileX-15) / 16);
			tmptileX = 16 + (tmptileX % 16);
		}
		if(tmptileY > 0)
		{
			ChunkY = ((tmptileY+15) / 16);
			tmptileY = 16 - (tmptileY % 16);
		}
		if(cam.newundo){undobuffer.emplace_back(); cam.newundo = false;}
		AddChunk(ChunkX, ChunkY, !remove * (page * 5 + type), abs(tmptileY%16) * 16 + abs(tmptileX%16), direction, flip, true);
	}
}
void Camera::Undo()
{
	if(!undobuffer.empty() && !undobuffer.back().empty())
	{
        for (int i = undobuffer.back().size() - 1; i >= 0; i--)
        {
			AddChunk(undobuffer.back()[i]->x, undobuffer.back()[i]->y, undobuffer.back()[i]->type, undobuffer.back()[i]->position, undobuffer.back()[i]->direction, undobuffer.back()[i]->flip, false);
		}
		undobuffer.pop_back();
	}

}
void Camera::Save()
{
	std::cout << BOLD << "start saving .." << RESET << std::endl;
	ARW_Import(filename);
	std::cout << BOLD << GREEN << "saved" << RESET << std::endl;
}
void Camera::SelectionStarted()
{
	SelectionBeginningX = tileX;
	SelectionBeginningY = tileY;
}
void Camera::SelectionGoing()
{
	selection = true;
	int minX = std::min(SelectionBeginningX, tileX);
	int maxX = std::max(SelectionBeginningX, tileX);
	
	int minY = std::min(SelectionBeginningY, tileY);
	int maxY = std::max(SelectionBeginningY, tileY);

	selection_mesh->SetSize(minX, maxX + 1, minY, maxY + 1);
}
void Camera::SelectionCompleted()
{
	SelectionEndX = tileX;
	SelectionEndY = tileY;
}
void Camera::DeleteSelection()
{
	//deleting everything in this square
	int minX = std::min(SelectionBeginningX, SelectionEndX);
	int maxX = std::max(SelectionBeginningX, SelectionEndX);
	
	int minY = std::min(SelectionBeginningY, SelectionEndY);
	int maxY = std::max(SelectionBeginningY, SelectionEndY);

	for (int y = minY; y <= maxY; ++y)
	{
 		for (int x = minX; x <= maxX; ++x)
  		{
	    	int tmptileX = x;
			int tmptileY = y;
			int ChunkX = tmptileX / 16;
			int ChunkY = tmptileY / 16;

			if(tmptileX < 0)
			{
				ChunkX = ((tmptileX-15) / 16);
				tmptileX = 16 + (tmptileX % 16);
			}
			if(tmptileY > 0)
			{
				ChunkY = ((tmptileY+15) / 16);
				tmptileY = 16 - (tmptileY % 16);
			}
			if(newundo){undobuffer.emplace_back(); newundo = false;}
   	    	AddChunk(ChunkX, ChunkY, 0, abs(tmptileY%16) * 16 + abs(tmptileX%16), 1, 0, true);
   		}
	}
	selection = false;
}
void Camera::CopySelection()
{
	brush.data.clear();
	int minX = std::min(SelectionBeginningX, SelectionEndX);
	int maxX = std::max(SelectionBeginningX, SelectionEndX);
	
	int minY = std::min(SelectionBeginningY, SelectionEndY);
	int maxY = std::max(SelectionBeginningY, SelectionEndY);

	int firstX = INT_MAX;
	int firstY = INT_MIN;

	for (int y = minY; y <= maxY; ++y)
	{
 		for (int x = minX; x <= maxX; ++x)
  		{
  			int tmptileX = x;
			int tmptileY = y;
			int ChunkX = tmptileX / 16;
			int ChunkY = tmptileY / 16;

			if(tmptileX < 0)
			{
				ChunkX = ((tmptileX-15) / 16);
				tmptileX = 16 + (tmptileX % 16);
			}
			if(tmptileY > 0)
			{
				ChunkY = ((tmptileY+15) / 16);
				tmptileY = 16 - (tmptileY % 16);
			}
			if(chunks.empty())
				continue;
			int32_t x_index = FindChunkX(ChunkX);
			if(x_index == chunks.size() || chunks[x_index][0]->x != ChunkX)
			{brush.SetState(0, 0, 0, 0, false);continue;}
		
			int32_t y_index = FindChunkY(x_index, ChunkY);
			if(y_index == chunks[x_index].size() || chunks[x_index][y_index]->y != ChunkY)
			{brush.SetState(0, 0, 0, 0, false);continue;}

			uint8_t pos = abs(tmptileY%16) * 16 + abs(tmptileX%16);
			uint8_t type = chunks[x_index][y_index]->type[pos];

			if(type == 0)
			{brush.SetState(0, 0, 0, 0, false);continue;}
	
			uint8_t direction = chunks[x_index][y_index]->direction[pos];
			bool flip = (chunks[x_index][y_index]->flip[pos/8] >> (7-pos%8)) & 1;
			brush.data.push_back(new BrushCell(x,y,type,direction,flip));	

			if(x < firstX)
			firstX = x;		

			if(y > firstY)
			firstY = y;	
  		}
  	}
  	for(uint32_t i = 0; i < brush.data.size(); i++)
  	{
  		brush.data[i]->x -= firstX;
  		brush.data[i]->y -= firstY;
  	}
}
void Camera::CutSelection()
{
	CopySelection();
	DeleteSelection();
	selection = false;
}
void Camera::CalculateMousePosInTiles(double xpos, double ypos)
{
	float aspect = (float)screen.x / screen.y;
	float worldHeight = baseHeight * zoom;
	float worldWidth  = worldHeight * aspect;
	float worldX = position.x - worldWidth * 0.5f + (xpos / screen.x) * worldWidth;
	float worldY = position.y + worldHeight * 0.5f - (ypos / screen.y) * worldHeight;
	tileX = floor(worldX);
	tileY = floor(worldY);
}
void Camera::Dropper(double xpos, double ypos)
{
	int tmptileX = tileX;
	int tmptileY = tileY;
	int ChunkX = tmptileX / 16;
	int ChunkY = tmptileY / 16;

	if(tmptileX < 0)
	{
		ChunkX = ((tmptileX-15) / 16);
		tmptileX = 16 + (tmptileX % 16);
	}
	if(tmptileY > 0)
	{
		ChunkY = ((tmptileY+15) / 16);
		tmptileY = 16 - (tmptileY % 16);
	}

	if(chunks.empty())
		return;
	int32_t x_index = FindChunkX(ChunkX);
	if(x_index == chunks.size() || chunks[x_index][0]->x != ChunkX)
	{brush.SetState(0, 0, 0, 0, false);return;}
		
	int32_t y_index = FindChunkY(x_index, ChunkY);
	if(y_index == chunks[x_index].size() || chunks[x_index][y_index]->y != ChunkY)
	{brush.SetState(0, 0, 0, 0, false);return;}

	uint8_t pos = abs(tmptileY%16) * 16 + abs(tmptileX%16);
	uint8_t type = chunks[x_index][y_index]->type[pos];

	if(type == 0)
	{brush.SetState(0, 0, 0, 0, false);return;}
	
	uint8_t t = type % 5 == 0 ? 5 : type % 5;
	uint8_t p = (type - t)/5;
	uint8_t direction = chunks[x_index][y_index]->direction[pos];
	bool flip = (chunks[x_index][y_index]->flip[pos/8] >> (7-pos%8)) & 1;
	brush.SetState(t, direction, flip, p, false);
}
void Camera::CheckButtonArrows(double xpos, double ypos)
{
	if(brush.type != 0)
		return;

  	int tmptileX = tileX;
	int tmptileY = tileY;
	int ChunkX = tmptileX / 16;
	int ChunkY = tmptileY / 16;
	
	if(tmptileX < 0)
	{
		ChunkX = ((tmptileX-15) / 16);
		tmptileX = 16 + (tmptileX % 16);
	}
	if(tmptileY > 0)
	{
		ChunkY = ((tmptileY+15) / 16);
		tmptileY = 16 - (tmptileY % 16);
	}
	
	if(chunks.empty())
		return;
	int32_t x_index = FindChunkX(ChunkX);
	if(x_index == chunks.size() || chunks[x_index][0]->x != ChunkX)
		return;
	int32_t y_index = FindChunkY(x_index, ChunkY);
	if(y_index == chunks[x_index].size() || chunks[x_index][y_index]->y != ChunkY)
		return;
	uint8_t type = chunks[x_index][y_index]->type[abs(tmptileY%16) * 16 + abs(tmptileX%16)];
	uint16_t pos = (abs(tmptileY%16) * 16 + abs(tmptileX%16));
	if(type == 24)
	{
		chunks[x_index][y_index]->next_state[pos]++;
		chunks[x_index][y_index]->next_dirt = true;
	}
	if(type == 21)
	{
		chunks[x_index][y_index]->special_state[pos/8] |= (1 << (7-pos%8));
		chunks[x_index][y_index]->next_dirt = true;
	}
}
void Camera::RenderGraphics(GraphicsHandler& gh)
{
	if(selection)
	{
		selection_mesh->Render(*this, gh);
	}

	if(brush.type == 0 && brush.paste == false)
	return;
	
	glUseProgram(gh.brush_shaderProgram);
	glUniform1i(gh.chunk_texLoc, 0);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gh.arrows_texture);
	
	glUniformMatrix4fv(gh.brush_orthoLoc, 1, GL_FALSE, glm::value_ptr(ortho));
	glUniform2f(gh.brush_mosposLoc, tileX, tileY);

	glBindVertexArray(brush.mesh->VAO);
	glDrawElements(GL_TRIANGLES, brush.mesh->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
void Camera::InitGraphics()
{
	brush.mesh = new BrushMesh(brush);
	selection_mesh = new SelectionMesh();
}
glm::mat4 Camera::GetProjection(int width, int height)
{
	float aspect = (float)width / height;
	screen = {width, height};
	float worldHeight = baseHeight * zoom;
	float worldWidth = worldHeight * aspect;
	ortho = glm::ortho(
	position.x - worldWidth * 0.5f,
	position.x + worldWidth * 0.5f,
	position.y - worldHeight * 0.5f,
	position.y + worldHeight * 0.5f,
	-1.0f,
	1.0f
	);
	
	return ortho;
}
void Camera::UpdateRenderArray()
{
	float aspect = (float)screen.x / screen.y;
	float worldHeight = baseHeight * zoom;
	float worldWidth = worldHeight * aspect;
	int x1, x2;
	int y1, y2;
	x1 = (int)std::floor((position.x - worldWidth * 0.5f) / 16.0f);
	x2 = (int)std::floor((position.x + worldWidth * 0.5f) / 16.0f);	
	y1 = (int)std::floor((position.y - worldHeight * 0.5f) / 16.0f);
	y2 = (int)std::floor((position.y + worldHeight * 0.5f) / 16.0f);

	FillRenderArray(x1,x2,y1,y2+1);
}
void Camera::UpdateWindowSize(GLFWwindow* window)
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	baseHeight = height / 45.0f;
	this->GetProjection(width,height);
	this->UpdateRenderArray();
}
void Camera::MoveCamera(double xpos, double ypos)
{	
	glm::vec2 delta_position = {(mouse_position.x - xpos)/(screen.x/( ((float)screen.x / screen.y) * baseHeight * zoom)), (ypos - mouse_position.y)/(screen.y/(baseHeight * zoom))};
	position = delta_position + old_position;
}

GLuint CompileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
        if (!success)
        {
            char infoLog[1024];
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "\033[31m" << "Shader compile error:\n" << "\033[0m" << infoLog << std::endl;
        }
    return shader;
}

GLuint CreateShaderProgram(const char* vshader, const char* fshader) 
{
    GLuint vertex = CompileShader(GL_VERTEX_SHADER, vshader);
    GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, fshader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}
GLuint CreateTexture(const char* filename)
{
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
    //This should probably improve performance but i prefer GL_LINEAR

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
    return texture;
}
