#pragma once

#include "common.h"
#include "shader.h"
#include "logic.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLuint CompileShader(GLenum type, const char* source);
GLuint CreateShaderProgram(const char* vshader, const char* fshader);
GLuint CreateTexture(const char* filename);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

struct Chunk;
struct ChunkMesh;
struct GraphicsHandler;

struct Point
{
    float x, y, z, u, v;

	Point() 
    	: x(0), y(0), z(0), u(0), v(0){}
    
    Point(float x_, float y_, float z_)
        : x(x_), y(y_), z(z_) { u =0; v = 0;}

    Point(float x_, float y_, float z_, float u_, float v_)
        : x(x_), y(y_), z(z_), u(u_), v(v_) {}
	
};


struct ChunkMesh
{
	Point vertices[256 * 4];
	unsigned int indices[256 * 6];
	GLuint VAO, VBO, EBO;

	ChunkMesh();
	~ChunkMesh();
	
	void ChangeState(Chunk* chunk, uint8_t position, uint8_t state);
	void CalculateCellMesh(Chunk* chunk, uint8_t position, bool _flip);
	void Update();
	void Render(glm::mat4 ortho, GraphicsHandler& gh);

	ChunkMesh(const ChunkMesh&) = delete;
    ChunkMesh& operator=(const ChunkMesh&) = delete;

    ChunkMesh(ChunkMesh&&) = default;
    ChunkMesh& operator=(ChunkMesh&&) = default;
};



struct Camera
{

	glm::vec2 position = {32,0};
	glm::vec2 screen;
	glm::vec2 old_position;
	glm::vec2 mouse_position = {0,0};
	
	Chunk* current_chunk;
	uint8_t current_arrow;
	uint8_t mouse_direction = 0;
	uint8_t mouse_type = 0;
	bool mouse_flip = 0;
	
    float zoom = 1;
    float baseHeight = 32.0f;
	glm::mat4 ortho;

    glm::mat4 GetProjection(int width, int height)
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
    void SetArrow(double xpos, double ypos)
    {
		if(mouse_type != 0)
		{
			float aspect = (float)screen.x / screen.y;
			float worldHeight = baseHeight * zoom;
			float worldWidth  = worldHeight * aspect;
			float worldX = position.x - worldWidth * 0.5f + (xpos / screen.x) * worldWidth;
			float worldY = position.y + worldHeight * 0.5f - (ypos / screen.y) * worldHeight;
			int tileX = floor(worldX);
			int tileY = floor(worldY);
			int ChunkX = tileX / 16;
			int ChunkY = tileY / 16;
    
			if(tileX < 0)
			{
				ChunkX = ((tileX-15) / 16);
				tileX = 16 + (tileX % 16);
			}
			if(tileY > 0)
			{
				ChunkY = ((tileY+15) / 16);
				tileY = 16 - (tileY % 16);
			}
		
			AddChunk(chunks, ChunkX, ChunkY, mouse_type, abs(tileY%16) * 16 + abs(tileX%16), mouse_direction, mouse_flip);
		}
	}
	void RemArrow(double xpos, double ypos)
    {
		float aspect = (float)screen.x / screen.y;
		float worldHeight = baseHeight * zoom;
		float worldWidth  = worldHeight * aspect;
		float worldX = position.x - worldWidth * 0.5f + (xpos / screen.x) * worldWidth;
		float worldY = position.y + worldHeight * 0.5f - (ypos / screen.y) * worldHeight;
		int tileX = floor(worldX);
		int tileY = floor(worldY);
		int ChunkX = tileX / 16;
		int ChunkY = tileY / 16;
    
		if(tileX < 0)
		{
			ChunkX = ((tileX-15) / 16);
			tileX = 16 + (tileX % 16);
		}
		if(tileY > 0)
		{
			ChunkY = ((tileY+15) / 16);
			tileY = 16 - (tileY % 16);
		}
		
		AddChunk(chunks, ChunkX, ChunkY, 0, abs(tileY%16) * 16 + abs(tileX%16), mouse_direction, mouse_flip);
	}
    void UpdateWindowSize(GLFWwindow* window)
    {
    	int width, height;
    	glfwGetFramebufferSize(window, &width, &height);
    	this->GetProjection(width,height);
    }
    void MoveCamera(double xpos, double ypos)
    {
     	glm::vec2 delta_position = {(mouse_position.x - xpos)/(screen.x/( ((float)screen.x / screen.y) * baseHeight * zoom)), (ypos - mouse_position.y)/(screen.y/(baseHeight * zoom))};
		position = delta_position + old_position;
    }
};

struct GraphicsHandler
{
GLFWwindow* window = nullptr;

GLuint shaderProgram;
GLuint texture;

GraphicsHandler(Camera* camera)
{
	this->InitGraphics(camera);
}
int InitGraphics(Camera* camera)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	window = glfwCreateWindow(800, 600, "BLogicArrows", NULL, NULL);

	glfwSetWindowUserPointer(window, camera);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 
	glfwSetMouseButtonCallback(window, mouse_button_callback); 
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	if(!window){glfwTerminate();return 1;}
	
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
	//glfwSetWindowSizeCallback(window, window_size_callback);  
//	glfwSetKeyCallback(window, key_callback);
//	glfwSetCharCallback(window, char_callback);

	glfwMakeContextCurrent(window);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	if(window == NULL || !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){glfwTerminate();return 1;}
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	shaderProgram = CreateShaderProgram(VxShader, FgShader);	
	texture = CreateTexture("img.png");
	
	return 0;
}

};
