#pragma once

#include "logic.h"
#include "common.h"
#include "shader.h"
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
struct Brush;
struct Camera;

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
	
	void ChangeState(Chunk* chunk, uint8_t position, bool state);
	void CalculateCellMesh(Chunk* chunk, uint8_t position, uint8_t _direction, bool _flip);
	void Update();
	void Render(glm::mat4 ortho, GraphicsHandler& gh);

	ChunkMesh(const ChunkMesh&) = delete;
    ChunkMesh& operator=(const ChunkMesh&) = delete;

    ChunkMesh(ChunkMesh&&) = default;
    ChunkMesh& operator=(ChunkMesh&&) = default;
};

struct BrushMesh
{
	std::vector<Point> vertices;
	std::vector<unsigned int> indices;
	GLuint VAO, VBO, EBO;

	BrushMesh(Brush& brush);
	~BrushMesh();

	void CalculateBrushMesh(Brush* brush);
	void Update();
	void Render(glm::mat4 ortho, GraphicsHandler& gh);

	BrushMesh(const BrushMesh&) = delete;
    BrushMesh& operator=(const BrushMesh&) = delete;

    BrushMesh(BrushMesh&&) = default;
    BrushMesh& operator=(BrushMesh&&) = default;
};

struct BrushCell
{
	int x, y;
	uint8_t type;	
	uint8_t direction;	
	bool flip;

	BrushCell(int x, int y, uint8_t t, uint8_t d, bool f)
		: x(x), y(y), type(t), direction(d), flip(f)
	{}
};

struct Brush
{
	std::vector<BrushCell*> data;
	BrushMesh* mesh = nullptr;
	
	uint8_t page = 0;
	uint8_t type = 0;
	uint8_t direction = 0;
	bool flip = 0;
	bool paste = false;

	void Draw(Camera& cam, bool remove);
	void SetState(uint8_t t, uint8_t d, bool f, uint8_t p, bool pst);
};

struct Background
{
	Point vertices[4];
	unsigned int indices[6] = {0, 1, 2, 0, 2, 3};
	GLuint VAO, VBO, EBO;	

	Background(GraphicsHandler& gh);
	void Render(Camera& camera, GraphicsHandler& gh);
};

struct SelectionMesh
{
	Point vertices[4];
	unsigned int indices[6] = {0, 1, 2, 0, 2, 3};
	GLuint VAO, VBO, EBO;	

	SelectionMesh();
	void SetSize(int x1, int x2, int y1, int y2);
	void Render(Camera& camera, GraphicsHandler& gh);
};

struct Camera
{
	glm::vec2 position = {0,0};
	glm::vec2 screen;
	glm::vec2 old_position;
	glm::vec2 mouse_position = {0,0};

	int tileX;
	int tileY;

	bool newundo = true;

	bool selection = false;
	int SelectionBeginningX, SelectionBeginningY;
	int SelectionEndX, SelectionEndY;

	int delay = 100;
	bool stop = false;
	
	Brush brush;
	SelectionMesh* selection_mesh = nullptr;
	
    float zoom = 1;
    float baseHeight = 32.0f;
	glm::mat4 ortho;

	void Undo();
	void Save();
	void InitGraphics();
	void SelectionStarted();
	void SelectionGoing();
	void SelectionCompleted();
	void SelectionProcessing();
	void RenderGraphics(GraphicsHandler& gh);
	void DeleteSelection();
	void CopySelection();
	void CutSelection();
    void UpdateRenderArray();
    glm::mat4 GetProjection(int width, int height);
	void CalculateMousePosInTiles(double xpos, double ypos);
	void CheckButtonArrows(double xpos, double ypos);
	void Dropper(double xpos, double ypos);
    void SetArrow(double xpos, double ypos);
	void RemArrow(double xpos, double ypos);
    void MoveCamera(double xpos, double ypos);
    void UpdateWindowSize(GLFWwindow* window);
};

struct GraphicsHandler
{
GLFWwindow* window = nullptr;

GLuint chunk_shaderProgram;
GLuint net_shaderProgram;
GLuint brush_shaderProgram;

GLuint chunk_texLoc;
GLuint chunk_orthoLoc;

GLuint net_orthoLoc;
GLuint net_zoomLoc;
GLuint net_cameraPosLoc;

GLuint brush_orthoLoc;
GLuint brush_mosposLoc;

GLuint arrows_texture;
GLuint selection_texture;

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

	glfwMakeContextCurrent(window);
	
	if(window == NULL || !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){glfwTerminate();return 1;}
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	chunk_shaderProgram = CreateShaderProgram(VxShader, FgShader);	
	net_shaderProgram = CreateShaderProgram(NetVxShader, NetFgShader);	
	brush_shaderProgram = CreateShaderProgram(BrushVxShader, BrushFgShader);	
	arrows_texture = CreateTexture("textures/i.png");
	selection_texture = CreateTexture("textures/selection.png");

	chunk_texLoc = glGetUniformLocation(chunk_shaderProgram, "tex");
	chunk_orthoLoc = glGetUniformLocation(chunk_shaderProgram, "ortho");
	
	net_orthoLoc = glGetUniformLocation(net_shaderProgram, "ortho");
	net_zoomLoc = glGetUniformLocation(net_shaderProgram, "zoom");
	net_cameraPosLoc = glGetUniformLocation(net_shaderProgram, "cameraPos");

	brush_orthoLoc = glGetUniformLocation(brush_shaderProgram, "ortho");
	brush_mosposLoc = glGetUniformLocation(brush_shaderProgram, "mospos");
	
	return 0;
}

};
